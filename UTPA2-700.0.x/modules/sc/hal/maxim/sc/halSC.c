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
/// file    drvSC.c
/// @brief  SmartCard Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvSC.h"

// Internal Definition
#include "regSC.h"
#include "halSC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SC_DBG_ENABLE               2UL
#define SC_DEV_NUM_MAX				2UL
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if SC_DBG_ENABLE
#define SC_DBG(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#define SC_ERR(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#else
#define SC_DBG(fmt, args...)        {}
#define SC_ERR(fmt, args...)        printf("[DEV][SMART][%06d] " fmt, __LINE__, ##args)
#endif

// Setting
//#define SC_EVENT()                  MsOS_SetEvent(s32CardEventId, SC_EVENT_CARD)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

MS_VIRT                              _regSCBase[SC_DEV_NUM_MAX];
MS_VIRT                              _regSCHWBase = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void HAL_SC_RegMap(MS_VIRT u32RegBase)
{
    _regSCBase[0] = u32RegBase;
}

void HAL_SC_HW_RegMap(MS_VIRT u32RegBase)
{
    _regSCHWBase = u32RegBase;
}

void HAL_SC_SetClk(MS_U8 u8SCID,MS_U16 u16clk)
{
    MS_U16              reg;

    reg = HW_READ(REG_TOP_CKG_SM_CA);
    if (u8SCID == 0)
    {
        reg &= (~TOP_CKG_SM_CA0_CLK_MASK);
        switch (u16clk)
        {
            case E_SC_CLK_3M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D8;
                break;
            case E_SC_CLK_4P5M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D6;
                break;
            case E_SC_CLK_6M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D4;
                break;
            case E_SC_CLK_13M :
                reg |= TOP_CKG_SM_CA0_CLK_27M_D2;
                break;
            default:
                SC_ERR("unknown SC_Set_Clk\n");
            break;
        }
    }
    else if (u8SCID == 1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
    HW_WRITE(REG_TOP_CKG_SM_CA, reg);
}

void HAL_SC_Init(MS_U8 u8SCID)
{
    //reg_allpad_in
    printf("HAL_SC_Init In\n");
    HW_WRITE(REG_TOP_ALLPAD_IN, HW_READ(REG_TOP_ALLPAD_IN) & ~TOP_ALLPAD_IN_EN);

    HAL_SC_PowerCtrl(u8SCID, TRUE);

    // Let the related I/O Low
    SC_WRITE(u8SCID,UART_LCR, UART_LCR_SBC); //I/O
    // mask tx level int and set tx level to 31->32 trigger int mode.
    // mask BGWT and CGWT at initialization
    SC_WRITE(u8SCID, UART_CTRL2, SC_READ(u8SCID, UART_CTRL2)|0x3E);    
    // BGT default value will be 0x16, reset to 0 here to disable the BGT int
    SC_WRITE(u8SCID, UART_SCBGT, 0x00)
    if (u8SCID == 0)
    {
    	printf("Do reg_sm0_open\n");
        //reg_sm0_open
        //HW_WRITE(REG_TOP_TS1_PE, HW_READ(REG_TOP_TS1_PE)|BIT(3)|BIT(6)); //for Edison new pad
        //HW_WRITE(REG_TOP_SM0,HW_READ(REG_TOP_SM0)|0x20);
        //HW_WRITE(REG_TOP_TEST_MODE,HW_READ(REG_TOP_TEST_MODE)&(~(BIT(2)|BIT(1)|BIT(0))));
        //HW_WRITE(REG_TOP_CONFIG,HW_READ(REG_TOP_CONFIG)&(~(BIT(13)|BIT(12)|BIT(11))));

    }
    if (u8SCID ==1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
}

void HAL_SC_Exit(MS_U8 u8SCID)
{
    HAL_SC_PowerCtrl(u8SCID, FALSE);
}

void HAL_SC_PowerCtrl(MS_U8 u8SCID, MS_BOOL bEnable)
{   
    if (u8SCID == 0)
    {
        if (bEnable)
        {
            // SM0: enable switch to specified clk
            HW_WRITE(REG_TOP_CKG_UART_CLK, (HW_READ(REG_TOP_CKG_UART_CLK) & ~(TOP_CKG_UART1_CLK_DIS | TOP_CKG_UART1_CLK_MASK)) | TOP_CKG_UART1_CLK_144M);
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS); //how
        }
        else
        {
            // SM0: disable clk
            HW_WRITE(REG_TOP_CKG_UART_CLK, (HW_READ(REG_TOP_CKG_UART_CLK) | TOP_CKG_UART1_CLK_DIS));//how
            HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); //how
        }
    }
    if (u8SCID ==1)
    {
        SC_ERR("[SC] not support SM1\n");
    }
}

void HAL_SC_CardInvert(MS_U8 u8SCID, SC_CardDetType eTpye)
{
    if (eTpye == E_SC_LOW_ACTIVE)
    {
        SC_OR(u8SCID,UART_CTRL3, UART_INVERT_CD);
    } 
    else 
    {
        SC_AND(u8SCID,UART_CTRL3, ~UART_INVERT_CD);
    }
}

MS_BOOL HAL_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage)
{
    if (E_SC_VOLTAGE_MAX <= eVoltage)
        return FALSE;
    if (E_SC_VOLTAGE_3_POINT_3V == eVoltage)
    {
        HW_WRITE(REG_TOP_3V_5V_SELECT, (HW_READ(REG_TOP_3V_5V_SELECT) & ~(0x1)));
    }
    else if (E_SC_VOLTAGE_5V == eVoltage)
    {
        HW_WRITE(REG_TOP_3V_5V_SELECT, (HW_READ(REG_TOP_3V_5V_SELECT) | (0x1)));
    }
    else
        return FALSE;
    return TRUE;
}

