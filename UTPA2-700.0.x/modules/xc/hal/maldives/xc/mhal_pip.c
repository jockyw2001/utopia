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
// [mhal_sc.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_PIP_C

#include "MsCommon.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"
#include "mvideo_context.h"
#include "drv_sc_menuload.h"
//==============================================================================
void Hal_SC_Sub_SetDisplayWindow_burst(void *pInstance,MS_WINDOW_TYPE *pstDispWin)
{
    MS_U16 u16extra_req;

    _MLOAD_ENTRY(pInstance);

    // When Main is foreground, sub is background, extra must enable.

    u16extra_req = 0x8000 | 0x01;

    //this is moved outside this function to mdrv_sc_pip.c
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_10_L, (TRUE<<1), BIT(1));

    // HW PIP architeucture
    // Becasue BK3_02[8] and BK20_11[15] can not enable toghter, otherwise garbage will be showed,
    // we need to use BK12_47[0] to instead. And the default value of BK3_02[8] is 1. So, we need
    // to set it to 0 in this function.
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, u16extra_req , 0x800F);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_02_L, 0, 0x0080);


    //Display window
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_07_L, pstDispWin->x, 0xFFFF);                             // Display H start
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_08_L, pstDispWin->width + pstDispWin->x - 1, 0xFFFF);    // Display H end
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_09_L, pstDispWin->y, 0xFFFF);                             // Display V start
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_0A_L, pstDispWin->height + pstDispWin->y - 1, 0xFFFF);   // Display V end

    MDrv_XC_MLoad_Fire(pInstance,TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_Sub_SetDisplayWindow(void *pInstance,MS_WINDOW_TYPE *pstDispWin)
{
//    SC_W2BYTEMSK(0, REG_SC_BK20_10_L, (TRUE<<1), BIT(1));

    //Display window
    SC_W2BYTE(0,REG_SC_BK0F_07_L, pstDispWin->x);                             // Display H start
    SC_W2BYTE(0,REG_SC_BK0F_08_L, pstDispWin->width + pstDispWin->x - 1);    // Display H end
    SC_W2BYTE(0,REG_SC_BK0F_09_L, pstDispWin->y);                             // Display V start
    SC_W2BYTE(0,REG_SC_BK0F_0A_L, pstDispWin->height + pstDispWin->y - 1);   // Display V end
}


void Hal_SC_subwindow_disable_burst(void *pInstance)
{
    _MLOAD_ENTRY(pInstance);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, (FALSE<<1), BIT(1));
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, 0x0000 , 0x800F);

    MDrv_XC_MLoad_Fire(pInstance,TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_subwindow_disable(void *pInstance)
{
    SC_W2BYTEMSK(0, REG_SC_BK20_10_L, (FALSE<<1), BIT(1));
    SC_W2BYTEMSK(0,REG_SC_BK20_11_L, 0x0000 , 0x800F);
}

MS_BOOL Hal_SC_Is_subwindow_enable(void)
{
    return SC_R2BYTEMSK(0, REG_SC_BK20_10_L, BIT(1));
}


void Hal_SC_set_border_format(void *pInstance, MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, SCALER_WIN eWindow )
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_02_L, ((MS_U16)u8Left<<8), 0xFF00);
        SC_W2BYTEMSK(0, REG_SC_BK0F_03_L, ((MS_U16)u8Right<<8), 0xFF00);
        SC_W2BYTEMSK(0, REG_SC_BK0F_04_L, ((MS_U16)u8Up<<8), 0xFF00);
        SC_W2BYTEMSK(0, REG_SC_BK0F_05_L, ((MS_U16)u8Down<<8), 0xFF00);

        SC_W2BYTEMSK(0, REG_SC_BK10_24_L, ((MS_U16)u8color<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_02_L, (MS_U16)u8Left, 0x00FF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_03_L, (MS_U16)u8Right, 0x00FF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_04_L, (MS_U16)u8Up, 0x00FF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_05_L, (MS_U16)u8Down, 0x00FF);

        SC_W2BYTEMSK(0, REG_SC_BK0F_17_L, (MS_U16)u8color, 0x00FF);
    }
}

void Hal_SC_border_enable(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK10_19_L, (bEnable<<2), BIT(2));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_01_L, (bEnable<<7), BIT(7));
    }
}

void Hal_SC_set_Zorder_main_first(void *pInstance,MS_BOOL bMainFirst)
{
    SC_W2BYTEMSK(0, REG_SC_BK20_10_L, (bMainFirst << 8), BIT(8) );

    if (bMainFirst)
        SC_W2BYTEMSK(0, REG_SC_BK19_14_L, BIT(3) , BIT(3) );
    else
        SC_W2BYTEMSK(0, REG_SC_BK19_14_L, 0x00 , BIT(3) );
}

void Hal_SC_set_Zorder_main_first_burst(void *pInstance,MS_BOOL bMainFirst)
{

    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, (bMainFirst << 8), BIT(8) );

    if (bMainFirst)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK19_14_L, BIT(3) , BIT(3) );
    else
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK19_14_L, 0x00 , BIT(3) );

    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_SetPIP_FRC(void *pInstance,MS_BOOL enable, MS_U8 u8IgonreLinesNum)
{
    // Enable FRC for Subwindow
    SC_W2BYTEMSK(0,REG_SC_BK12_45_L, ( enable ? BIT(3):0 ), BIT(3));

    // Set Threadhold
    SC_W2BYTEMSK(0,REG_SC_BK12_45_L, u8IgonreLinesNum, BIT(2)|BIT(1)|BIT(0));
}

void Hal_SC_set_Zextra_y_half_burst(void *pInstance,MS_BOOL bInterlace)
{
    _MLOAD_ENTRY(pInstance);

    if (bInterlace)
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, BIT(13), BIT(13));
    else
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, 0, BIT(13));

    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_Zextra_y_half(void *pInstance,MS_BOOL bInterlace)
{

    if (bInterlace)
        SC_W2BYTEMSK(0,REG_SC_BK20_11_L, BIT(13), BIT(13));
    else
        SC_W2BYTEMSK(0,REG_SC_BK20_11_L, 0, BIT(13));
}
void Hal_SC_enable_extra_request(void *pInstance, MS_BOOL bEnable)
{
}
#undef  MHAL_PIP_C
