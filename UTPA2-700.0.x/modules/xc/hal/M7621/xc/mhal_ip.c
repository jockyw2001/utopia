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
#define MHAL_IP_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "mvideo_context.h"
#include "xc_Analog_Reg.h"
#include "mhal_ip.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "drvXC_HDMI_if.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "mhal_adc.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"
#include "hwreg_ipmux.h"
#include "XC_private.h"
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DRVSCIP_DBG(x)  //x


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
///software reset for scaler
///@param u16Sel \b IN
///- software reset VDFE: BIT(15)
///- software reset VDCOMBF: BIT(14)
///- software reset EMCU: BIT(13)
///- software reset GMC: BIT(12)
///- software reset REG: BIT(7)
///- software reset ADC: BIT(6)
///- software reset DIGITAL: BIT(5)
///- software reset SCALER: BIT(4)
///- software reset DISPLAY: BIT(3)
///- software reset OSD: BIT(1)
///- software reset ALL: BIT(0)
///@param u8DelayTime \b IN:
///- software reset delay time
/******************************************************************************/
void Hal_SC_ip_software_reset(void *pInstance, MS_U8 u8Reset, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
        MS_U8  u8CLK1Mux = MDrv_ReadByte(REG_CKG_IDCLK1); //Sub window
        MS_U8  u8CLK2Mux = MDrv_ReadByte(REG_CKG_IDCLK2); //Main window

        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, u8Reset, 0xFF );

        MsOS_DelayTask(1);
        if(u8Reset == REST_IP_F1) //Sub window
        {
            MDrv_WriteByteMask(REG_CKG_IDCLK1, CKG_IDCLK1_XTAL, CKG_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_IDCLK1, u8CLK1Mux, CKG_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
        }
        else if(u8Reset == REST_IP_F2) //Main window
        {
            MDrv_WriteByteMask(REG_CKG_IDCLK2, CKG_IDCLK2_XTAL, CKG_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_IDCLK2, u8CLK2Mux, CKG_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
        }
        else if(u8Reset == REST_IP_ALL) //Main and sub windows
        {
            MDrv_WriteByteMask(REG_CKG_IDCLK1, CKG_IDCLK1_XTAL, CKG_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_IDCLK1, u8CLK1Mux, CKG_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_IDCLK2, CKG_IDCLK2_XTAL, CKG_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_IDCLK2, u8CLK2Mux, CKG_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
        }

        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, 0x00, 0xFF );

        if (MDrv_ReadByte(REG_CHIP_REVISION) == 0)//u01 pip hw bug
        {
            if(Hal_SC_Is_subwindow_enable(pInstance))//when sub was enabled, set REG_SC_BK46_3F_bit0  "0", because hw reset "1" on this bit.
            {
                SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK46_3F_L, 0, BIT(0));
            }
        }

        MsOS_DelayTask(1);
    }
    else
    {
        MS_U8  u8CLK1Mux = MDrv_ReadByte(REG_CKG_S2_IDCLK1); //Sub window
        MS_U8  u8CLK2Mux = MDrv_ReadByte(REG_CKG_S2_IDCLK2); //Main window

        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, u8Reset, 0xFF );
        MsOS_DelayTask(1);
        if(u8Reset == REST_IP_F1) //Sub window
        {
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK1, CKG_S2_IDCLK1_XTAL, CKG_S2_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK1, u8CLK1Mux, CKG_S2_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
        }
        else if(u8Reset == REST_IP_F2) //Main window
        {
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, CKG_S2_IDCLK2_XTAL, CKG_S2_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, u8CLK2Mux, CKG_S2_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
        }
        else if(u8Reset == REST_IP_ALL) //Main and sub windows
        {
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK1, CKG_S2_IDCLK1_XTAL, CKG_S2_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK1, u8CLK1Mux, CKG_S2_IDCLK1_MASK); // Sub window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, CKG_S2_IDCLK2_XTAL, CKG_S2_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
            MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, u8CLK2Mux, CKG_S2_IDCLK2_MASK); // Main window reset to XTAL when ADC input no signal
        }
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, 0x00, 0xFF );
        MsOS_DelayTask(1);
    }
}

MS_U8 Hal_SC_ip_get_sync_flag(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    MS_U8 u8SyncFlag;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //u8SyncFlag = MDrv_ReadByte(L_BK_IP1F2(0x1E));
        u8SyncFlag = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1E_L) & 0x00FF;
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //u8SyncFlag = MDrv_ReadByte(L_BK_IP1F1(0x1E));
        u8SyncFlag = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_1E_L) & 0x00FF;
    }
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
    return u8SyncFlag;
}

MS_U8 Hal_SC_ip_get_sync_detect_status(void *pInstance, SCALER_WIN eWindow)
{
    MS_U8 u8DetectStatus;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    /* mode detect status */
    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //u8DetectStatus = MDrv_ReadByte(H_BK_IP1F2(0x1E));
        u8DetectStatus = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1E_L) & 0xFF00)>>8;
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //u8DetectStatus = MDrv_ReadByte(H_BK_IP1F1(0x1E));
        u8DetectStatus = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_1E_L) & 0xFF00)>>8;
    }

    //MDrv_WriteByte( BK_SELECT_00, u8Bank );
    return u8DetectStatus;
}

MS_U8 Hal_SC_ip_get_input_vsync_polarity(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1E_L) & BIT(8)) ? 1:0;
}

MS_U8 Hal_SC_ip_get_input_vsync_value(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (Hal_SC_ip_get_sync_flag(pInstance, eWindow) & BIT(2)) ? 1:0;
}

MS_U8 Hal_SC_ip_get_output_vsync_value(void *pInstance, SCALER_WIN eWindow)
{
    // For Active Low case, BIT(0) = 0  means Vsync, so return true means get Vsync
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (Hal_SC_ip_get_sync_flag(pInstance, eWindow) & BIT(0)) ? 0 : 1;
}

void Hal_SC_ip_set_reg_usr_vspolmd(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow )
{
    //MS_U8 u8Bank;
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_WriteRegBit(H_BK_IP1F2(0x21), u8Enable, BIT(4));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, u8Enable ? BIT(12) : 0, BIT(12));
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //MDrv_WriteRegBit(H_BK_IP1F1(0x21), u8Enable, BIT(4));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, u8Enable ? BIT(12) : 0, BIT(12));
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

void Hal_SC_ip_init_reg_in_timingchange(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_WriteByte(L_BK_IP1F2(0x21), 0x00);
        //MDrv_WriteByteMask(H_BK_IP1F2(0x21), 0x00 , BIT(0) );
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L,0x0000,0x01FF);

        // reset 3D setting
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_7F_L, 0x00, BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x00, BIT(0));
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //MDrv_WriteByte(L_BK_IP1F1(0x21), 0x00);
        //MDrv_WriteByteMask(H_BK_IP1F1(0x21), 0x00 , BIT(0) );
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L,0x0000,0x01FF);
    }

    // restore coast window to default settings
    Hal_SC_ip_set_coast_window( pInstance, 0x0C, 0x0C, eWindow );

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

/******************************************************************************/
///This function will return Horizontal period value
///@return
///- MS_U16 Horizontal Period
/******************************************************************************/
MS_U16 Hal_SC_ip_get_horizontalPeriod(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    MS_U16 u16HorizontalPeriod;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //u16HorizontalPeriod = ( MDrv_Read2Byte( L_BK_IP1F2(0x20)) & MST_H_PERIOD_MASK );
        u16HorizontalPeriod = ( SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_20_L) & MST_H_PERIOD_MASK );
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //u16HorizontalPeriod = ( MDrv_Read2Byte( L_BK_IP1F1(0x20)) & MST_H_PERIOD_MASK );
        u16HorizontalPeriod = ( SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_20_L) & MST_H_PERIOD_MASK );
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

    return u16HorizontalPeriod;
}


/******************************************************************************/
///This function will return Vertical total value
///@return
///- MS_U16 Vertical total
/******************************************************************************/
MS_U16 Hal_SC_ip_get_verticaltotal(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    MS_U16 u16VerticalTotal;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //u16VerticalTotal = ( MDrv_Read2Byte( L_BK_IP1F2(0x1f)) & MST_V_TOTAL_MASK );
        u16VerticalTotal = ( SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_1F_L) & MST_V_TOTAL_MASK );
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //u16VerticalTotal = ( MDrv_Read2Byte( L_BK_IP1F1(0x1f)) & MST_V_TOTAL_MASK );
        u16VerticalTotal = ( SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_1F_L) & MST_V_TOTAL_MASK );
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

    return u16VerticalTotal;
}
#if 0
/******************************************************************************/
/// Get the horizontal period of auto postion
/// @return the horizontal period of auto postion
/******************************************************************************/
MS_U16 MDrv_Scaler_GetHorizontalDEStart(void)
{
    MS_U8 u8Bank;
    MS_U16 u16HorizontalDE;

    u8Bank = MDrv_ReadByte ( BK_SELECT_00 );
    MDrv_WriteByte ( BK_SELECT_00, REG_BANK_IP1F2 );

    u16HorizontalDE = MDrv_Read2Byte ( L_BK_IP1F2 ( 0x13 ) );

    MDrv_WriteByte ( BK_SELECT_00, u8Bank );

    return u16HorizontalDE;
}


/******************************************************************************/
/// Get the horizontal period of auto postion
/// @return the horizontal period of auto postion
/******************************************************************************/
MS_U16 MDrv_Scaler_GetHorizontalDE(void)
{
    MS_U8 u8Bank;
    MS_U16 u16HorizontalDE;

    u8Bank = MDrv_ReadByte ( BK_SELECT_00 );
    MDrv_WriteByte ( BK_SELECT_00, REG_BANK_IP1F2 );

    u16HorizontalDE = MDrv_Read2Byte ( L_BK_IP1F2 ( 0x15 ) ) -
                      MDrv_Read2Byte ( L_BK_IP1F2 ( 0x13 ) ) + 1;

    MDrv_WriteByte ( BK_SELECT_00, u8Bank );

    return u16HorizontalDE;
}


/******************************************************************************/
/// Get the vertical period of auto postion
/// @return the vertical period of auto postion
/******************************************************************************/
MS_U16 MDrv_Scaler_GetVerticalDEStart(void)
{
    MS_U8 u8Bank;
    MS_U16 u16VerticalDE;

    u8Bank = MDrv_ReadByte ( BK_SELECT_00 );
    MDrv_WriteByte ( BK_SELECT_00, REG_BANK_IP1F2 );

    u16VerticalDE = MDrv_Read2Byte ( L_BK_IP1F2 ( 0x12 ) );

    MDrv_WriteByte ( BK_SELECT_00, u8Bank );

    return u16VerticalDE;
}

/******************************************************************************/
/// Get the vertical period of auto postion
/// @return the vertical period of auto postion
/******************************************************************************/
MS_U16 MDrv_Scaler_GetVerticalDEEnd(void)
{
    MS_U8 u8Bank;
    MS_U16 u16VerticalDE;

    u8Bank = MDrv_ReadByte ( BK_SELECT_00 );
    MDrv_WriteByte ( BK_SELECT_00, REG_BANK_IP1F2 );

    u16VerticalDE = MDrv_Read2Byte ( L_BK_IP1F2 ( 0x14 ) );

    MDrv_WriteByte ( BK_SELECT_00, u8Bank );

    return u16VerticalDE;
}


/******************************************************************************/
/// Get the vertical period of auto postion
/// @return the vertical period of auto postion
/******************************************************************************/
MS_U16 MDrv_Scaler_GetVerticalDE(void)
{
    MS_U8 u8Bank;
    MS_U16 u16VerticalDE;

    u8Bank = MDrv_ReadByte ( BK_SELECT_00 );
    MDrv_WriteByte ( BK_SELECT_00, REG_BANK_IP1F2 );

    u16VerticalDE = MDrv_Read2Byte ( L_BK_IP1F2 ( 0x14 ) ) -
                    MDrv_Read2Byte ( L_BK_IP1F2 ( 0x12 ) ) + 1;

    // SC_PATCH_02 start ===============================
    // 2008-07-24.Daniel: Patch Interlace mode
    if( Hal_SC_ip_get_interlace_status() )
    {
        if( u16VerticalDE&1 )
            u16VerticalDE += 1;
    }
    // SC_PATCH_02 end =================================

    MDrv_WriteByte ( BK_SELECT_00, u8Bank );

    return u16VerticalDE;
}
#endif
//=========================================================//
// Function : Hal_SC_ip_de_hstart_info
// Description:
//=========================================================//
MS_U16 Hal_SC_ip_de_hstart_info(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_13_L, IP_DE_HSTART_MASK);
    else
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_13_L, IP_DE_HSTART_MASK);
}
//=========================================================//
// Function : Hal_SC_ip_de_hend_info
// Description:
//=========================================================//
MS_U16 Hal_SC_ip_de_hend_info(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_15_L, IP_DE_HEND_MASK);
    else
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_15_L, IP_DE_HEND_MASK);
}

//=========================================================//
// Function : Hal_SC_ip_de_vstart_info
// Description:
//=========================================================//
MS_U16 Hal_SC_ip_de_vstart_info(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_12_L, IP_DE_VSTART_MASK);
    else
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_12_L, IP_DE_VSTART_MASK);
}
//=========================================================//
// Function : Hal_SC_ip_de_vend_info
// Description:
//=========================================================//
MS_U16 Hal_SC_ip_de_vend_info(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_14_L, IP_DE_VEND_MASK);
    else
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_14_L, IP_DE_VEND_MASK);
}

/******************************************************************************/
///Set H position for PC mode
///@param u16Position \b IN
///- H position
/******************************************************************************/
void Hal_SC_ip_set_pc_h_position (void *pInstance, MS_U16 u16Position, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_W=rite2Byte(L_BK_IP1F2(0x05), u16Position) ;
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, u16Position) ;
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //MDrv_Write2Byte(L_BK_IP1F1(0x05), u16Position) ;
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, u16Position) ;
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}


/******************************************************************************/
///Set V position for PC mode
///@param u16Position \b IN
///- V position
/******************************************************************************/
void Hal_SC_ip_set_pc_v_position (void *pInstance, MS_U16 u16Position, SCALER_WIN eWindow )
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    u16Position = (u16Position==0)?1:u16Position;

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_Write2Byte(L_BK_IP1F2(0x04), u16Position );
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, u16Position) ;
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //MDrv_Write2Byte(L_BK_IP1F1(0x04), u16Position );
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, u16Position);
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

/******************************************************************************/
///Set Macrovision filter
///@param bEnable \b IN
///- ENABLE Filter
/******************************************************************************/
void Hal_SC_ip_set_ms_filter(void *pInstance, MS_BOOL bEnable, MS_U16 u16FilterRange, SCALER_WIN eWindow )
{
    //Macrovision filter is not used right now
    UNUSED(u16FilterRange);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if ( bEnable )
    {
        if( eWindow == MAIN_WINDOW )
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_35_L, BIT(12) , BIT(12) | BIT(13) );
        }
        else
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_35_L, BIT(12) , BIT(12) | BIT(13) );
        }
    }
    else
    {
        if( eWindow == MAIN_WINDOW )
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_35_L, 0x00 , BIT(12) | BIT(13) );
        }
        else
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_35_L, 0x00 , BIT(12) | BIT(13) );
        }
    }

}

/******************************************************************************/
///Set coast window information.
///@param u8Start \b IN
///- Coast start from n HSYNC leading edge
///@param u8End \n IN
///- Coast end at n HSYNC leading edge
/******************************************************************************/
void Hal_SC_ip_set_coast_window(void *pInstance, MS_U8 u8Start, MS_U8 u8End, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_24_L, u8Start<<8, 0xFF00 );
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_25_L, u8End, 0x00FF );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_24_L, u8Start<<8, 0xFF00 );
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_25_L, u8End, 0x00FF );
    }
}


void Hal_SC_ip_sog_detect(void* pInstance)
{
    MS_U8 u8SOGState;
    MS_U8 u8SogDetectStatus;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u8SogDetectStatus = HAL_SC_GetSogDetectStatus(pInstance);

    u8SogDetectStatus &= ~(0x0001);
    u8SOGState = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L)& 0x0070;
    // check Separated -> Composite -> SOG -> Separated
    if(u8SOGState & BIT(4))
    {
        // in detecting SOG, turn SOG detect off and set back to auto-detect
        u8SOGState &= ~0x70;    // set BIT[6..4] to 0 to turn off SOG detect
        u8SOGState |= 0x00;     // switch to detect separated H/V Sync
    }
    else if((u8SOGState & 0x60) == 0x40)
    {
        // detecting separated H/V sync, switch to detect composite sync
        u8SOGState &= ~0x70;
        u8SOGState |= 0x20;     // switch to detect composite sync
    }
    else if((u8SOGState & 0x60) == 0x20)
    {
        // detecting composite sync, switch to detect SOG
        u8SogDetectStatus |= (0x0001);
        u8SOGState |= 0x70;     // set BIT[6..4] to 0x7 to turn SOG detect on
    }
    else
    {
        // detecting separated H/V sync, switch to detect composite sync
        u8SOGState &= ~0x70;
        u8SOGState |= 0x40;     // switch to detect composite sync
    }

    HAL_SC_SetSogDetectStatus(pInstance, u8SogDetectStatus);
    //MDrv_WriteByte(L_BK_IP1F2(0x02), u8SOGState);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L,u8SOGState,0x0070);
    Hal_SC_ip_software_reset(pInstance, REST_IP_F2, MAIN_WINDOW);
    Hal_ADC_reset(pInstance, REST_ADC);
    // adjust ADC bandwidth
    if((u8SOGState & 0x70) == 0x70)
    {
        // SOG
        //MDrv_WriteRegBit(L_BK_IP1F2(0x03), DISABLE, BIT(5));  // Delay 1/4 input HSYNC
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0 ,BIT(5));
        MDrv_WriteRegBit(REG_ADC_ATOP_1C_L, DISABLE, BIT(5));
        MDrv_WriteByte(REG_ADC_ATOP_1C_H, 0x38);          // Turn on SOG input low bandwidth filter
        MDrv_WriteRegBit(REG_ADC_DTOP_07_L, ENABLE, BIT(6));   // ADC PLL lock source is SOG
    }
    else
    {
        // Non SOG
        //MDrv_WriteRegBit(L_BK_IP1F2(0x03), DISABLE, BIT(5));  // No delay input HSYNC
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0 ,BIT(5));
        MDrv_WriteRegBit(REG_ADC_ATOP_1C_L, ENABLE, BIT(5));
        MDrv_WriteByte(REG_ADC_ATOP_1C_H, 0x30);           // Turn off SOG input low bandwidth filter
        MDrv_WriteRegBit(REG_ADC_DTOP_07_L, DISABLE, BIT(6));   // ADC PLL lock source is HSync
    }
}

/******************************************************************************/
///get interlace detecting result
///@return MS_U8
///- 1:interlace
///- 0:no interlace
/******************************************************************************/
MS_U8 Hal_SC_ip_get_interlace_status (void *pInstance, SCALER_WIN eWindow )
{
    //MS_U8 u8Bank;
    MS_BOOL bInterlace;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    bInterlace = 0;

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //if ( MDrv_ReadByte(H_BK_IP1F2(0x1E)) & BIT(3) )
        if ( SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1E_L) & BIT(11) )
        {
            bInterlace = 1;
        }
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        //if ( MDrv_ReadByte(H_BK_IP1F1(0x1E)) & BIT(3) )
        if ( SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_1E_L) & BIT(11) )
        {
            bInterlace = 1;
        }
    }


    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
    return bInterlace;
}

void Hal_SC_IPMux_Gen_SpecificTiming(void *pInstance, XC_Internal_TimingType timingtype )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    switch (timingtype)
    {
        case E_XC_480P:
        case E_XC_480I:
            W2BYTE(REG_IPMUX_12_L, 0x02D0);  //HDE
            W2BYTEMSK(REG_IPMUX_13_L, 010, LBMASK);   //H front porch
            W2BYTEMSK(REG_IPMUX_14_L, 0x3E, LBMASK) ;   //H sync pulse width
            W2BYTE(REG_IPMUX_15_L, 0x05F0);  //Htotal     0x035A
            W2BYTE(REG_IPMUX_16_L, 0x01E0) ;  //VDE
            W2BYTEMSK(REG_IPMUX_17_L, 0x06, LBMASK) ;    //V front porch
            W2BYTEMSK(REG_IPMUX_18_L, 0x09, LBMASK) ;    //V sync pulse width
            W2BYTE(REG_IPMUX_19_L, 0x020d) ;  //Vtotal
            break;

        case E_XC_576P:
        case E_XC_576I:
            W2BYTE(REG_IPMUX_12_L, 0x02D0);  //HDE
            W2BYTEMSK(REG_IPMUX_13_L, 010, LBMASK);   //H front porch
            W2BYTEMSK(REG_IPMUX_14_L, 0x3E, LBMASK) ;   //H sync pulse width
            W2BYTE(REG_IPMUX_15_L, 0x035A);  //Htotal
            W2BYTE(REG_IPMUX_16_L, 0x0240) ;  //VDE
            W2BYTEMSK(REG_IPMUX_17_L, 0x06, LBMASK) ;    //V front porch
            W2BYTEMSK(REG_IPMUX_18_L, 0x09, LBMASK) ;    //V sync pulse width
            W2BYTE(REG_IPMUX_19_L, 0x0271) ;  //Vtotal
            break;

        case E_XC_720P:
        case E_XC_720I:
            W2BYTE(REG_IPMUX_12_L, 0x0500);  //HDE
            W2BYTEMSK(REG_IPMUX_13_L, 0x6E, LBMASK);   //H front porch
            W2BYTEMSK(REG_IPMUX_14_L, 0x28, LBMASK) ;   //H sync pulse width
            W2BYTE(REG_IPMUX_15_L, 0x0690);  //Htotal
            W2BYTE(REG_IPMUX_16_L, 0x02D0) ;  //VDE
            W2BYTEMSK(REG_IPMUX_17_L, 0x05, LBMASK) ;    //V front porch
            W2BYTEMSK(REG_IPMUX_18_L, 0x05, LBMASK) ;    //V sync pulse width
            W2BYTE(REG_IPMUX_19_L, 0x02EE) ;  //Vtotal
            break;

        case E_XC_1080P:
        case E_XC_1080I:

            W2BYTE(REG_IPMUX_12_L, 0x0780);  //HDE
            W2BYTEMSK(REG_IPMUX_13_L, 0x58, LBMASK);   //H front porch
            W2BYTEMSK(REG_IPMUX_14_L, 0x2C, LBMASK) ;   //H sync pulse width
            W2BYTE(REG_IPMUX_15_L, 0x0898);  //Htotal
            W2BYTE(REG_IPMUX_16_L, 0x0438) ;  //VDE
            W2BYTEMSK(REG_IPMUX_17_L, 0x02, LBMASK) ;    //V front porch
            W2BYTEMSK(REG_IPMUX_18_L, 0x05, LBMASK) ;    //V sync pulse width
            W2BYTE(REG_IPMUX_19_L, 0x0465) ;  //Vtotal

            break;
        case E_XC_OFF:
        default:
            break;
    }

    if((timingtype == E_XC_480I) || (timingtype == E_XC_576I) || (timingtype == E_XC_720I) || (timingtype == E_XC_1080I))
    {
        W2BYTEMSK(REG_IPMUX_10_L, BIT(1), BIT(1));
    }
    else
    {
        W2BYTEMSK(REG_IPMUX_10_L, 0x00, BIT(1));
    }
    if ( timingtype == E_XC_OFF)
    {
        // Turn off timing gen
        W2BYTEMSK(REG_IPMUX_10_L, 0x00, BIT(0));
    }
    else
    {
        W2BYTEMSK(REG_IPMUX_10_L, BIT(15), BIT(15));
        W2BYTEMSK(REG_IPMUX_10_L, BIT(0), BIT(0));
    }


}

MS_BOOL Hal_SC_Check_IP_Gen_Timing(void *pInstance)
{
    return (MDrv_ReadByte(REG_IPMUX_10_L) & 0x01);
}

void Hal_SC_ip_set_input_source(void *pInstance, MS_U8 u8InputSrcSel, MS_U8 u8SyncSel, MS_U8 u8VideoSel, MS_U8 u8isYPbPr, SCALER_WIN eWindow )
{
    MS_U16 u16RegVal;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u16RegVal = (u8InputSrcSel&0x07) |
                ((u8SyncSel&0x07)  << 4) |
                ((u8VideoSel&0x03) << 8) |
                ((u8isYPbPr&0x01)  << 10);

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, u16RegVal,  0x0777);
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, u16RegVal,  0x0777);
    }

}


void hal_ip_set_input_10bit(void *pInstance, MS_BOOL bInput10Bit, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        if(bInput10Bit)
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(11), BIT(11));
        }
        else //Input is 8bit
        {
            //when BIT(11)=0 --> BIT(10)=1, 8bit.5;  BIT(10)=0, 8bit.0
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(10), (BIT(11)|BIT(10)) );
        }
    }
    else
    {
        if(bInput10Bit)
        {
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, BIT(11), BIT(11));
        }
        else //Input is 8bit
        {
            //when BIT(11)=0 --> BIT(10)=1, 8bit.5;  BIT(10)=0, 8bit.0
            SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, BIT(10), (BIT(11)|BIT(10)) );
        }
    }
}


void Hal_SC_ip_set_image_wrap(void *pInstance, MS_BOOL bHEnable, MS_BOOL bVEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, ((bVEnable<<1)|bHEnable), (BIT(1)|BIT(0)) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, ((bVEnable<<1)|bHEnable), (BIT(1)|BIT(0)) );
    }
}


void Hal_SC_ip_set_input_sync_reference_edge(void *pInstance, MS_BOOL bHRef, MS_BOOL bVRef, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, ((bHRef<<4)|(bVRef<<3)), (BIT(4)|BIT(3)) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, ((bHRef<<4)|(bVRef<<3)), (BIT(4)|BIT(3)) );
    }
}


void Hal_SC_ip_set_input_vsync_delay(void *pInstance, MS_BOOL bDelay, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, bDelay<<5, BIT(5) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, bDelay<<5, BIT(5) );
    }
}


void Hal_SC_ip_set_de_only_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Turn on BIT(7) to let H/V Start being programable.
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, (BIT(7)|(bEnable<<6)), (BIT(7)|BIT(6)) );
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L, bEnable<<4, BIT(4) ); // SW patch for HDMI switch, Auto search HST jistter mask filter force off
        if(psXCInstPri->u32DeviceID)
            MDrv_WriteRegBit(REG_S2_DE_ONLY_F2, bEnable, S2_DE_ONLY_F2_MASK);
        else
            MDrv_WriteRegBit(REG_DE_ONLY_F2, bEnable, DE_ONLY_F2_MASK);
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, (BIT(7)|(bEnable<<6)), (BIT(7)|BIT(6)) );
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_2F_L, bEnable<<4, BIT(4) ); // SW patch for HDMI switch, Auto search HST jistter mask filter force off
        if(psXCInstPri->u32DeviceID)
            MDrv_WriteRegBit(REG_S2_DE_ONLY_F1, bEnable, S2_DE_ONLY_F1_MASK);
        else
            MDrv_WriteRegBit(REG_DE_ONLY_F1, bEnable, DE_ONLY_F1_MASK);
    }
}

void Hal_SC_ip_set_coast_input(void *pInstance, MS_BOOL bInputSel, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //BIT(0): Coast Polarity to PAD  --TODO
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_24_L, ((bInputSel<<5)|BIT(0)), 0xFF );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_24_L, ((bInputSel<<5)|BIT(0)), 0xFF );
    }
}

void Hal_SC_ip_set_DE_Mode_Glitch(void *pInstance, MS_U8 u8Setting , SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_27_L, u8Setting, 0xFF );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_27_L, u8Setting, 0xFF );
    }
}

MS_BOOL Hal_SC_ip_get_DE_mode_glitch_protect_enabled(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_27_L, BIT(2)) >> 2);
    }
    else
    {
        return (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_27_L, BIT(2)) >> 2);
    }
}

void Hal_SC_ip_set_input_sync_sample_mode(void *pInstance, MS_BOOL bMode, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, (bMode<<2), BIT(2) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, (bMode<<2), BIT(2) );
    }
}

void Hal_SC_ip_set_de_bypass_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L, (bEnable<<15), BIT(15) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_2F_L, (bEnable<<15), BIT(15) );
    }
}

void Hal_SC_ip_set_de_lock_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_BOOL bLastLockMode = FALSE;

    if( eWindow == MAIN_WINDOW )
    {

        if(SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, BIT(6) ))
        {
            bLastLockMode = TRUE;
        }

        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, (bEnable<<6), BIT(6) );

        if(bLastLockMode != bEnable)
        {
            //change mode, then sw reset
            Hal_SC_ip_software_reset(pInstance, REST_IP_F2, MAIN_WINDOW);
        }

    }
    else
    {
        if(SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_1D_L, BIT(6) ))
        {
            bLastLockMode = TRUE;
        }

        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_1D_L, (bEnable<<6), BIT(6) );

        if(bLastLockMode != bEnable)
        {
            //change mode, then sw reset
            Hal_SC_ip_software_reset(pInstance, REST_IP_F1, MAIN_WINDOW);
        }

    }
}

MS_U16 Hal_SC_get_hsd_pixel_count(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK02_3E_L, BITMASK(11 : 0) );
    }
    else
    {
        return SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK04_3E_L, BITMASK(11 : 0) );
    }
}

MS_U16 Hal_SC_get_vsd_line_count(void *pInstance, SCALER_WIN eWindow )
{
    MS_U16 u16Count;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        u16Count = SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK02_3F_L, BITMASK(10 : 0) );
    }
    else
    {
        u16Count = SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK04_3F_L, BITMASK(10 : 0) );
    }
    if(MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow) > DOUBLEHD_1080X2P_VSIZE)
    {
        u16Count |= 0x800; //Complement for HW bits missing
    }
    return u16Count;
}

void Hal_SC_ip_set_post_glitch_removal(void *pInstance, MS_BOOL bEnble, MS_U8 u8Range, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, ((bEnble<<7)|((u8Range&0x07)<<4)), 0xF0 );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, ((bEnble<<7)|((u8Range&0x07)<<4)), 0xF0 );
    }
}

MS_U8 Hal_SC_ip_get_post_glitch_removal(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bEnable = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if( eWindow == MAIN_WINDOW )
    {
        bEnable = (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, BIT(7)) >> 7);
        if (bEnable)
        {
            return (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, BIT(6)|BIT(5)|BIT(4)) >> 4);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        bEnable = (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, BIT(7)) >> 7);
        if (bEnable)
        {
            return (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, BIT(6)|BIT(5)|BIT(4)) >> 4);
        }
        else
        {
            return 0;
        }
    }
}

//////////////////////////////////
// Capture window
void Hal_SC_ip_get_capture_window(void *pInstance, MS_WINDOW_TYPE* capture_win,SCALER_WIN eWindow)
{

    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if( eWindow == MAIN_WINDOW )
    {
       // MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);

        /*capture_win->y =  MDrv_Read2Byte( L_BK_IP1F2(0x04) ) ;  // Vstart
        capture_win->x =  MDrv_Read2Byte( L_BK_IP1F2(0x05) ) ;  // Hstart
        capture_win->height=  MDrv_Read2Byte( L_BK_IP1F2(0x06) ) ;  // Vsize
        capture_win->width=  MDrv_Read2Byte( L_BK_IP1F2(0x07) ) ;  // Hsize
        */
        capture_win->y =  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_04_L ) ;  // Vstart
        capture_win->x =  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_05_L ) ;  // Hstart
        capture_win->height=  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_06_L ) ;  // Vsize
        capture_win->width=  SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_07_L , 0x3FFF) ;  // Hsize
    }
    else
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);

        capture_win->y =  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_04_L ) ;  // Vstart
        capture_win->x =  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_05_L ) ;  // Hstart
        capture_win->height=  SC_R2BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_06_L ) ;  // Vsize
        capture_win->width=  SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x3FFF ) ;  // Hsize
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

#if 0
static void Hal_SC_ip_set_capture_window(MS_U32 u32Reg, MS_U16 u16Value , SCALER_WIN eWindow)
{
    if( eWindow == MAIN_WINDOW )
    {
        // There are 11bits for each setting of capture window
        SC_W2BYTEMSK( u32Reg , u16Value , 0x07FF );
    }
    else
    {
        MS_ASSERT(0);
    }
}
#endif

void Hal_SC_ip_set_capture_v_start(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, u16Vstart, 0x1FFF);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, u16Vstart, 0x1FFF);
}

void Hal_SC_ip_set_capture_v_start_with_Menuload(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);

        if(eWindow == MAIN_WINDOW)
        {
            MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_IP_MAIN_SYNC);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_04_L, u16Vstart, 0x1FFF);
        }
        else
        {
            MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_IP_SUB_SYNC);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_04_L, u16Vstart, 0x1FFF);
        }
        if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
        {
                printf("Error: Set MLoad Fire Error!!!!\n ");
        }
        MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_OP_SYNC);

        _MLOAD_RETURN(pInstance);
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, u16Vstart, 0x1FFF);
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, u16Vstart, 0x1FFF);
    }
}

void Hal_SC_ip_set_capture_h_start(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, u16Hstart, 0x1FFF);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, u16Hstart, 0x1FFF);
}

void Hal_SC_ip_set_capture_h_start_with_Menuload(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);

        if(eWindow == MAIN_WINDOW)
        {
            MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_IP_MAIN_SYNC);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_05_L, u16Hstart, 0x1FFF);
        }
        else
        {
            MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_IP_SUB_SYNC);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_05_L, u16Hstart, 0x1FFF);
        }
        if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
        {
                printf("Error: Set MLoad Fire Error!!!!\n ");
        }
        MDrv_XC_MLoad_set_trigger_sync(pInstance, MLOAD_TRIGGER_BY_OP_SYNC);

        _MLOAD_RETURN(pInstance);
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, u16Hstart, 0x1FFF);
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, u16Hstart, 0x1FFF);
    }
}

void Hal_SC_ip_set_capture_v_size(void *pInstance, MS_U16 u16Vsize , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, u16Vsize, 0x1FFF);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, u16Vsize, 0x1FFF);
}

void Hal_SC_ip_set_capture_h_size(void *pInstance, MS_U16 u16Hsize , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, u16Hsize, 0x3FFF);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, u16Hsize, 0x3FFF);
}

void Hal_SC_ip_set_fir_down_sample_divider(void *pInstance, MS_U8 u8Enable, MS_U16 u16OverSampleCount,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_0D_L, (u8Enable?( BIT(7) | (u16OverSampleCount & 0xF ) ):0) , (BIT(7) | BIT(0) |BIT(1) | BIT(2) |BIT(3)) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_0D_L, (u8Enable?( BIT(7) | (u16OverSampleCount & 0xF ) ):0) , (BIT(7) | BIT(0) |BIT(1) | BIT(2) |BIT(3)) );
    }
}

MS_BOOL Hal_XC_MApi_XC_IPAutoNoSignal_GetCaps(void)
{
    return TRUE;
}

void Hal_SC_ip_set_IPAutoNoSignal(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0B_L, bEnable? BIT(5):0, BIT(5));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, bEnable? BIT(12):0, BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_23_L, bEnable? BIT(14):0, BIT(14));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0B_L, bEnable? BIT(5):0, BIT(5));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_1D_L, bEnable? BIT(12):0, BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_23_L, bEnable? BIT(13):0, BIT(13));
    }
}

MS_BOOL Hal_SC_ip_get_IPAutoNoSignal(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, BIT(12) ) )? TRUE:FALSE;
    }
    else
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_1D_L, BIT(12) ) )? TRUE:FALSE;
    }
}

void Hal_SC_ip_enable_turnoff_OP1_for_AutoNoSignal(void *pInstance, MS_BOOL bEnable)
{
    SC_W2BYTEMSK(0, REG_SC_BK20_24_L, bEnable? BIT(10):0, BIT(10));
}

//////////////////////////////////
// Auto gain
void Hal_SC_ip_set_auto_gain_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, (u8Enable?(BIT(0) | BIT(4)):0) , (BIT(0) | BIT(4)) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, (u8Enable?(BIT(0) | BIT(4)):0) , (BIT(0) | BIT(4)) );
    }
}

MS_BOOL Hal_SC_ip_is_auto_gain_result_ready(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, BIT(1) ) )? TRUE:FALSE;
    }
    else
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, BIT(1) ) )? TRUE:FALSE;
    }
}

// bit 0  : B min
// bit 1  : G min
// bit 2  : R min
MS_U8 Hal_SC_ip_auto_gain_min_value_status(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, ( BIT(10) | BIT(9) | BIT(8) ) ) ) >> 8 );
    }
    else
    {
        return ( (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, ( BIT(10) | BIT(9) | BIT(8) ) ) ) >> 8 );
    }
}

// Make sure
// bit 0  : B max
// bit 1  : G max
// bit 2  : R max
MS_U8 Hal_SC_ip_auto_gain_max_value_status(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, ( BIT(5) | BIT(6) | BIT(7) ) ) ) >> 5 );
    }
    else
    {
        return ( (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, ( BIT(5) | BIT(6) | BIT(7) ) ) ) >> 5 );
    }
}

//////////////////////////////////
// Auto phase
MS_U32 Hal_SC_ip_get_auto_phase_value(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1A_L) );
    }
    else
    {
        return ( SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_1A_L) );
    }
}

MS_BOOL Hal_SC_ip_is_auto_phase_result_ready(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_19_L, BIT(1) ) )? TRUE:FALSE;
    }
    else
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_19_L, BIT(1) ) )? TRUE:FALSE;
    }
}

//////////////////////////////////
// Auto position
MS_BOOL Hal_SC_ip_is_auto_position_result_ready(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_10_L, BIT(1) ) )? TRUE:FALSE;
    }
    else
    {
        return ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_10_L, BIT(1) ) )? TRUE:FALSE;
    }
}
void Hal_SC_ip_set_auto_position_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_10_L, (u8Enable?BIT(0):0) , BIT(0) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_10_L, (u8Enable?BIT(0):0) , BIT(0) );
    }
}

// Threshold is from 2'b0000 ~ 2'b1111
void Hal_SC_ip_set_valid_data_threshold(void *pInstance, MS_U8 u8Threshold, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_10_L, (u8Threshold & 0x0F) << 12 , 0xF000 );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_10_L, (u8Threshold & 0x0F) << 12 , 0xF000 );
    }
}

#if 0
static MS_U16 Hal_SC_ip_get_auto_position(MS_U32 u32Reg, SCALER_WIN eWindow)
{
    if( eWindow == MAIN_WINDOW )
    {
        return ( SC_R2BYTE(u32Reg) & 0xFFF );
    }
    else
    {
        MS_ASSERT(0);
        return 0;
    }
}
#endif

MS_U16 Hal_SC_ip_get_auto_position_v_start(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_12_L) & 0xFFF);
    else
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_12_L) & 0xFFF);
}

MS_U16 Hal_SC_ip_get_auto_position_h_start(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_13_L) & 0xFFF);
    else
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_13_L) & 0xFFF);
}

MS_U16 Hal_SC_ip_get_auto_position_v_end(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_14_L) & 0xFFF);
    else
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_14_L) & 0xFFF);
}

MS_U16 Hal_SC_ip_get_auto_position_h_end(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_15_L) & 0xFFF);
    else
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_15_L) & 0xFFF);
}

//////////////////////////////////
// Auto range
void Hal_SC_ip_set_auto_range_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow )
{
    #if 0
    if (u8Enable)
    {
        // Debug mode.
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_60_L, 0xFF00 , 0xFF00); // Bit(15) is enable.
    }
    else
    {
        // Debug mode.
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_60_L, 0x0000 , 0xFF00); // Bit(15) is enable.
    }
    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_29_L, (u8Enable?BIT(8):0) , BIT(8) );
    }
    else
    {
        SC_W2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_29_L, (u8Enable?BIT(8):0) , BIT(8) );
    }
}

void Hal_SC_ip_set_auto_range_window(void *pInstance, MS_U16 u16Vstart, MS_U16 u16Hstart, MS_U16 u16Vsize , MS_U16 u16Hsize, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_2A_L, u16Vstart  );  //vstart
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_2B_L, u16Hstart  );  //hstart
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_2C_L, u16Vsize  );  //vsize
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_2D_L, u16Hsize  );  //hsize
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_2A_L, u16Vstart  );  //vstart
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_2B_L, u16Hstart  );  //hstart
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_2C_L, u16Vsize  );  //vsize
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_2D_L, u16Hsize  );  //hsize
    }

}

MS_U8 HAL_SC_ip_get_ipmux(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)MDrv_ReadByte(REG_IPMUX_01_L)&0xF0;
}
void HAL_SC_ip_set_ipmux(void *pInstance, MS_U8 u8Val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MDrv_WriteByteMask(REG_IPMUX_01_L, u8Val,0x00F0);
}

MS_U8 HAL_SC_ip_get_ipclk(void *pInstance)
{
    return (MS_U8)MDrv_ReadByte(0x1E3F)&0x3F;
}

void HAL_SC_ip_set_ipclk(void *pInstance, MS_U8 u8Val)
{
    MDrv_WriteByteMask(0x1E3F, u8Val, 0x3F); // 1E1F[13:8]
}

MS_U8 HAL_SC_ip_get_capturesource(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x0007);
}

void HAL_SC_ip_set_capturesource(void *pInstance, MS_U8 u8Val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, (MS_U16)u8Val, 0x0007);
}

MS_U8 Hal_SC_ip_get_user_def_interlace_status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, 0x0003);
}
void Hal_SC_ip_set_user_def_interlace_status(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsInterlace,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, (bEnable?BIT(0):0),BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, (bIsInterlace?BIT(1):0),BIT(1));
    }
    else if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, (bEnable?BIT(0):0),BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, (bIsInterlace?BIT(1):0),BIT(1));
    }
}

MS_U8 HAL_SC_ip_get_capture_method(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x00FF);
}

MS_BOOL HAL_SC_ip_get_capture_format(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bIsRGBfmt;
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x0008))
        bIsRGBfmt = TRUE;
    else
        bIsRGBfmt = FALSE;

    return bIsRGBfmt;
}

void HAL_SC_ip_set_capture_format(void *pInstance, MS_BOOL bIsRGB)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, (bIsRGB?BIT(3):0), 0x0008);
}

MS_BOOL HAL_SC_ip_get_h_predown_info(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, 0x8000))
        return TRUE;
    else
        return FALSE;
}
MS_BOOL HAL_SC_ip_get_v_predown_info(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, 0x8000))
        return TRUE;
    else
        return FALSE;
}
void HAL_SC_ip_set_h_predown_info(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, (bEnable?BIT(15):0), BIT(15));
}
void HAL_SC_ip_set_v_predown_info(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, (bEnable?BIT(15):0),BIT(15));
}

MS_BOOL HAL_SC_ip_get_444to422_filter_mod(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(7)))
        return TRUE;
    else
        return FALSE;
}
void HAL_SC_ip_set_444to422_filter_mod(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, (bEnable?BIT(7):0), BIT(7));
}

//F2 memory data format, BK12_01[10:8]
MS_U8 HAL_SC_get_memory_fmt(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, 0x0700)>>8);
}

//F2 IP memory data format, BK12_2[3:0]
MS_U8 HAL_SC_get_IPM_memory_fmt(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x000F));
}
void HAL_SC_set_IPM_memory_fmt(void *pInstance, MS_U8 u8Value)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, (((MS_U16)u8Value)|BIT(8)),0x010F);
}

//F2 OP memory data format, BK12_2[7:4]
MS_U8 HAL_SC_get_OPM_memory_fmt(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x00F0));
}
void HAL_SC_set_OPM_memory_fmt(void *pInstance, MS_U8 u8Value)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, (((MS_U16)u8Value)<<4|(BIT(9))),0x02F0);
}

//capture image to IP enable
void HAL_SC_set_capture_image2ip(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, (bEnable ? BIT(11):0), BIT(11));
}
MS_U8 HAL_SC_get_VOP_mux(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x00F0);
}

void HAL_SC_set_VOP_mux(void *pInstance, MS_U8 u8Val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, (MS_U16)u8Val, 0x00F0);
}

MS_U8 HAL_SC_get_memory_bit_fmt(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, BIT(5)|BIT(4));
    else
        return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, BIT(5)|BIT(4));
}

//F2 RGB/YUV 444 8-bits format
//F2 RGB/YUV 444 10-bits format
//(bEnable_8bit : bEnable_10bit)
//(0:0)YCbCr422, (1:0)RGB/YCbCr444_8bit, (0:1)RGB/YCbCr444_10bit
void HAL_SC_set_memory_bit(void *pInstance, MS_BOOL bEnable_8bit, MS_BOOL bEnable_10bit)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, (MS_U16)((bEnable_10bit<<5)|bEnable_8bit<<4), (BIT(5)|BIT(4)));
}

void HAL_SC_set_IPM_capture_start(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, (MS_U16)(bEnable?BIT(15):0), BIT(15));
}

MS_U32 HAL_SC_get_IRQ_mask_0_31(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (MS_U32)SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK00_14_L);
}

void HAL_SC_set_IRQ_mask_0_31(void *pInstance, MS_U32 u32Val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK00_14_L, u32Val);
}
MS_BOOL HAL_SC_set_GOP_Enable(void *pInstance, MS_U8 MaxGOP ,MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable)
{
    /* GOP OP Path enable to SC Setting
        T3: GOP OP Path blending with SC sequence
        mux0-->mux2-->mux3
    */
    MS_U16 u16regval;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(UseNum > MaxGOP)
        return FALSE;
    u16regval = (MS_U16)SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK00_0C_L);

    if(UseNum == (u8MuxNum & (BIT(0)|BIT(1))))  // Enable Mux0 to XC
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0C_L, bEnable?(u16regval|BIT(15)):(u16regval & (~ BIT(15))), BIT(15));
    }
    else if(UseNum == ((u8MuxNum & (BIT(4)|BIT(5)))>>4))  // Enable Mux2 to XC
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0C_L, bEnable?(u16regval|BIT(14)):(u16regval & (~ BIT(14))), BIT(14));
    }
    else if(UseNum == ((u8MuxNum & (BIT(6)|BIT(7)))>>6))  // Enable Mux3 to XC
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0C_L, bEnable?(u16regval|BIT(13)):(u16regval & (~ BIT(13))), BIT(13));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


/********************************************************************************/
/// Set GOP destination (OP/IP) setting to scaler
/// @param ipSelGop \b IN \copydoc MS_XC_IPSEL_GOP
/********************************************************************************/
void HAL_SC_ip_sel_for_gop(void *pInstance, MS_U8 u8MuxNum ,MS_XC_IPSEL_GOP ipSelGop)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    switch(ipSelGop)
    {
        default:
        case MS_IP0_SEL_GOP0:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0xA0, 0xA0);
            if(0 == (u8MuxNum & 0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(12), BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(13), BIT(13)); // mux 1
            break;
        case MS_IP0_SEL_GOP1:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0xA0, 0xA0);
            if(1 == (u8MuxNum & 0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(12), BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(13), BIT(13)); // mux 1
            break;
        case MS_IP0_SEL_GOP2:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0xA0, 0xA0);
            if(2 == (u8MuxNum & 0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(12), BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(13), BIT(13)); // mux 1
            break;
        case MS_NIP_SEL_GOP0:
            if(0==(u8MuxNum &0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(13)); // mux 1
            break;
        case MS_NIP_SEL_GOP1:
            if(1==(u8MuxNum &0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(13)); // mux 1
            break;
        case MS_NIP_SEL_GOP2:
            if(2==(u8MuxNum &0x0003))
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(12)); // mux 0
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0, BIT(13)); // mux 1
            break;
        case MS_MVOP_SEL:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, 0xA0, 0xA0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_0A_L, BIT(12), BIT(12)|BIT(13)); // mux 0
            break;
    }
}


/******************************************************************************/
/// Set Scaler VOP New blending level
/******************************************************************************/
void HAL_SC_SetVOPNBL(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_46_L, 0x20, 0x20);
}

/******************************************************************************/
/// Set handshaking mode
/******************************************************************************/
void Hal_SC_ip_set_handshaking_md(void *pInstance, MS_U8 u8MD, SCALER_WIN eWindow)
{
    UNUSED(pInstance);
    u8MD = u8MD;
    eWindow = eWindow;
}

//////////////////////////////////
// Set IP1 Test Pattern
void Hal_SC_ip_Set_TestPattern(void *pInstance, MS_U16 u16Enable, MS_U32 u32Pattern_type, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_40_L, u16Enable, 0xFFFF);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_42_L, (u32Pattern_type >> 8));
        if(u16Enable == PURE_COR_IP_PATTERN_VALUE)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_43_L, (u32Pattern_type & 0x000F));
        }
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_40_L, u16Enable, 0xFFFF);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_42_L, (u32Pattern_type >> 8));
        if(u16Enable == PURE_COR_IP_PATTERN_VALUE)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_43_L, (u32Pattern_type & 0x000F));
        }
    }
}

void Hal_SC_adc_Set_TestPattern(void *pInstance,MS_U8 u8EnableADCType,MS_U16 u16Ramp)
{
    W2BYTEMSK(REG_ADC_ATOP_75_L, u8EnableADCType,0x0F00);
    W2BYTEMSK(REG_ADC_ATOP_78_L, u16Ramp,0x03FF);
    W2BYTEMSK(REG_ADC_ATOP_75_L, u16Ramp? BIT(0):0,BIT(0));
}

void Hal_SC_ipmux_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_CR_Data, MS_U16 u16G_Y_Data ,MS_U16 u16B_CB_Data)
{
    W2BYTEMSK(REG_IPMUX_10_L, bEnable? BIT(9):0, BIT(9));
    W2BYTEMSK(REG_IPMUX_10_L, bEnable? BIT(12):0, BIT(12));
    W2BYTEMSK(REG_IPMUX_10_L, bEnable? BIT(13):0, BIT(13));
    W2BYTEMSK(REG_IPMUX_10_L, bEnable? BIT(14):0, BIT(14));

    if(bEnable == TRUE)
    {
        W2BYTEMSK(REG_IPMUX_1D_L, u16R_CR_Data, BMASK(9:0));
        W2BYTEMSK(REG_IPMUX_1E_L, u16G_Y_Data,  BMASK(9:0));
        W2BYTEMSK(REG_IPMUX_1F_L, u16B_CB_Data, BMASK(9:0));
    }
}

//////////////////////////////////
// Set OP Test Pattern
void Hal_SC_op_Set_TestPattern(void *pInstance,MS_BOOL bMiuLineBuff, MS_BOOL bLineBuffHVSP)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK20_10_L, bMiuLineBuff? BIT(15):0,BIT(15));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK20_1F_L, bLineBuffHVSP? BIT(10):0,BIT(10));
}

//////////////////////////////////
// Set VOP Test Pattern
void Hal_SC_vop_Set_TestPattern(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_13_L, bEnable? BIT(8):0,BIT(8));
}

void Hal_SC_vop2_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    #if (SUPPORT_OP2_TEST_PATTERN >0)
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if(bEnable == TRUE)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern == FALSE)
        {
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern = TRUE;
            pXCResourcePrivate->stdrvXC_MVideo.u16OP2_color_3x3 = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_2F_L,0xFFFF);
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2InputSrcDisabled = Hal_SC_Is_InputSource_Disable(pInstance,MAIN_WINDOW) >> 7;
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2Mute = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_19_L, BIT(1)) >> 1;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_19_L, 0,BIT(1));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_02_L, 0,BIT(7)); //input source disable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_2F_L,0x0000,0xFFFF); //disable 3x3
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK25_32_L, u16R_Data, BMASK(9:0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK25_33_L, u16G_Data, BMASK(9:0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK25_34_L, u16B_Data, BMASK(9:0));
    }
    else
    {
        if (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern == TRUE)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_2F_L, pXCResourcePrivate->stdrvXC_MVideo.u16OP2_color_3x3, 0xFFFF); //restore 3x3
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_19_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2Mute<< 1),BIT(1));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_02_L, (pXCResourcePrivate->stdrvXC_MVideo.bIsOP2InputSrcDisabled<< 7),BIT(7));
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern = FALSE;
        }
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK25_30_L, (bEnable), BIT(0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK25_31_L, (bEnable),  BIT(0));
    #endif
}

//////////////////////////////////
// Set MOD Test Pattern
void Hal_SC_mod_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data)
{
    MOD_W2BYTEMSK(REG_MOD_BK00_01_L, bEnable? BIT(15):0,BIT(15));
    MOD_W2BYTEMSK(REG_MOD_BK00_02_L, u16R_Data, 0x3FF);
    MOD_W2BYTEMSK(REG_MOD_BK00_03_L, u16G_Data, 0x3FF);
    MOD_W2BYTEMSK(REG_MOD_BK00_04_L, u16B_Data, 0x3FF);
}

void HAL_SC_ip_3DMainSub_IPSync(void *pInstance)
{
    MS_U16 value ;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //copy bk1 to bk3 and then reset fclk
    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_08_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_08_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_09_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_24_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_24_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_25_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_25_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_26_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, value);

    value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_2F_L, value);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, 1 <<6, BIT(6));
    MsOS_DelayTask(100);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, 0x0,     BIT(6));
}

void HAL_SC_ip_detect_mode(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
    {
        if(bEnable)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_29_L,BIT(5), BIT(5)|BIT(4));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_29_L, 0 , BIT(5)|BIT(4));
        }
    }
    else
    {
    if(bEnable)
    {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK03_29_L,BIT(5), BIT(5)|BIT(4));
    }
    else
    {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK03_29_L, 0 , BIT(5)|BIT(4));
        }
    }

}

/******************************************************************************/
/// get the h/v sync active
/******************************************************************************/
MS_U8 Hal_SC_ip_get_h_v_sync_active(void *pInstance, SCALER_WIN eWindow)
{
    MS_U8 u8val;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_33_L, BIT(8), BIT(8));

    u8val = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1E_L) & 0xC000) >> 8;

    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_33_L, 0, BIT(8));

    return u8val;
}

void Hal_SC_set_skp_fd(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
}

/******************************************************************************/
/// Get the VGA SOG on or off
/// @param  bEnable                \b IN: bEnable =1, Turn on the VGA SOG; bEnable =0, Turn off the VGA SOG
/******************************************************************************/
void HAL_SC_SetVGASogEn(void *pInstance, MS_BOOL bVGASogEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_IP._bSetVGASogEn = bVGASogEn;
}

/******************************************************************************/
/// Get the VGA SOG on or off
/// @param  u8                \b IN: bEnable =1, Turn on the VGA SOG; bEnable =0, Turn off the VGA SOG
/******************************************************************************/
MS_BOOL HAL_SC_GetVGASogEn(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_IP._bSetVGASogEn;
}

/******************************************************************************/
/// Set the VGA SOG Detect Status
/******************************************************************************/
void HAL_SC_SetSogDetectStatus(void *pInstance, MS_U8  u8SogDetectStatus)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_IP._u8SetSogDetectStatus = u8SogDetectStatus;
}

/******************************************************************************/
/// Get the VGA SOG Detect Status
/******************************************************************************/
MS_U8  HAL_SC_GetSogDetectStatus(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_IP._u8SetSogDetectStatus;
}

/******************************************************************************/
///This function will return input frame rate
///@return
///- MS_U16 Vertical total
/******************************************************************************/
MS_U32 HAL_SC_ip_get_verticalperiod(void *pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32verticalperiod=0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == MAIN_WINDOW)
    {
        u32verticalperiod = SC_R4BYTE( psXCInstPri->u32DeviceID, REG_SC_BK01_3B_L );
    }
    else
    {
        u32verticalperiod = SC_R4BYTE( psXCInstPri->u32DeviceID, REG_SC_BK03_3B_L );
    }

    return u32verticalperiod;
}

/******************************************************************************/
///Set xc fd mask on/off by window
/******************************************************************************/
void HAL_SC_ip_Set_FD_Mask_ByWin(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, (bEnable?BIT(8):0), BIT(8));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, (bEnable?BIT(8):0), BIT(8));
    }

#if (HW_DESIGN_4K2K_VER == 4)
    // for FD_mask can control FRCM write
    Hal_SC_set_frcm_to_FD_mask(pInstance, bEnable, eWindow);
#endif
}

/******************************************************************************/
///Get xc fd mask by window
///@return MS_BOOL fd mask on/off
/******************************************************************************/
MS_BOOL HAL_SC_ip_Get_FD_Mask_ByWin(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_26_L, BIT(8)))
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_26_L, BIT(8)))
            return TRUE;
        else
            return FALSE;
    }
}

/******************************************************************************/
///Set Force IP interlace when it is progressive timing
///@return EN_FORCEi_SUPPORT_TYPE ForceI_SW/ForceI_HW/ForceI_Auto
/******************************************************************************/
EN_FORCEi_SUPPORT_TYPE HAL_SC_ip_get_forceI_support_type(void *pInstance)
{
    return ForceI_HW;
}

/******************************************************************************/
///Set H/V sync status check enable/disable
///@return
/******************************************************************************/
void HAL_SC_ip_set_hv_sync_status_check(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, bEnable << 6, BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, bEnable << 6, BIT(6));
    }
}

/******************************************************************************/
///Set new mode interlaced detect enable/disable
///@return
/******************************************************************************/
void HAL_SC_ip_set_new_mode_interlaced_detect(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, bEnable, BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, bEnable, BIT(0));
    }
}

/******************************************************************************/
///Set H/V sync source select for mode detection
///@return
/******************************************************************************/
void HAL_SC_ip_set_hv_sync_source_select(void *pInstance, MS_U8 u8Source, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, (u8Source & 0x3) << 1, BIT(2)|BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, (u8Source & 0x3) << 1, BIT(2)|BIT(1));
    }
}

/******************************************************************************/
///Set V total count by pixel clock enable/disable
///@return
/******************************************************************************/
void HAL_SC_ip_set_vtotal_count_by_pixel_clock(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, bEnable << 3, BIT(3));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, bEnable << 3, BIT(3));
    }
}

/******************************************************************************/
///Set V sync invert
///@return
/******************************************************************************/
void HAL_SC_ip_set_vsync_invert(void *pInstance, MS_BOOL bInvert, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, bInvert << 4, BIT(4));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, bInvert << 4, BIT(4));
    }
}

/******************************************************************************/
///Set H sync invert
///@return
/******************************************************************************/
void HAL_SC_ip_set_hsync_invert(void *pInstance, MS_BOOL bInvert, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK01_48_L, bInvert << 5, BIT(5));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK03_48_L, bInvert << 5, BIT(5));
    }
}

