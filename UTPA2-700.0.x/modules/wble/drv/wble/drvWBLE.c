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
#define  _DRV_WBLE_C_


#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"

#include "MsOS.h"
#include "MsTypes.h"
#include "wble_hwreg_utility2.h"

#include "drvMMIO.h"
#include "drvWBLE.h"
#include "mhal_wble.h"

/******************************************************************************/
/*                           Constant                                         */
/******************************************************************************/
#define BLE_DEFLICK_BLEND_FACTOR        32UL
#define BLE_DEFLICK_PRECISION_SHIFT     64UL


#define BLE_METHOD_FIX_START          1   // Fixed starting point, variable slop
#define BLE_METHOD_FIX_SLOP           2   // Variable staring point, fixed slop
#define BLE_METHOD_DYNAMIC_START_SLOP 3   // Variable staring point, variable slop

#define BLE_SLOP_PARAM  0
#define BLE_START_PARAM 1


/******************************************************************************/
/*                           Local                                            */
/******************************************************************************/

static MS_U8 g_u8BLE_OriSlop = 0x80;
static MS_U8 g_u8WLE_OriSlop = 0x80;

static MS_U16 u16PreHistogramTotal;
static MS_U16 u16PreBLESlope;
static MS_U16 u16PreBLEStart;
static MS_U8  u8SlopeDefalut;

static MSIF_Version _api_xc_wble_version = {
    .DDI = { WBLE_API_VERSION },
};

//-------------------------------------------------------------------------------------------------
/// Get XC library version
/// @param  ppVersion                  \b OUT: store the version in this member
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_DRVWBLE_ReturnValue MDrv_WBLE_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_DRVWBLE_RET_FAIL;
    }

    *ppVersion = &_api_xc_wble_version;
    return E_DRVWBLE_RET_OK;
}


void MDrv_WBLE_Init(void)
{
    u16PreHistogramTotal = 0;
    u16PreBLESlope = Hal_WBLE_get_black_slop() * BLE_DEFLICK_PRECISION_SHIFT;
    u16PreBLEStart = Hal_WBLE_get_black_start() * BLE_DEFLICK_PRECISION_SHIFT;
}

void MDrv_WBLE_EnableBLE(MS_BOOL bEnable)
{
   Hal_WBLE_set_ble(bEnable);
}


void MDrv_WBLE_EnableWLE(MS_BOOL bEnable)
{
   Hal_WBLE_set_wle(bEnable);
}


void MDrv_WBLE_SetBLE(MS_U8 u8Start, MS_U8 u8Slop)
{
    Hal_WBLE_set_ble_start(u8Start);
    Hal_WBLE_set_ble_slop(u8Slop);
    g_u8BLE_OriSlop = u8Slop;
}

void MDrv_WBLE_SetWLE(MS_U8 u8Start, MS_U8 u8Slop)
{
    Hal_WBLE_set_wle_start(u8Start);
    Hal_WBLE_set_wle_slop(u8Slop);
    g_u8WLE_OriSlop = u8Slop;
}


void MDrv_WBLE_Handler_BLEAvgLuma(MS_U8 u8AvgY, MS_U16 u16YTotalSum)
{
    MS_U8 u8Reg;
    MS_BOOL  bEnableBLE;
    MS_U8 u8Method, u8Strength, u8NewTarget;
    MS_U8 u8FlickAlpha;
    MS_U8 u8Step;
    MS_U16 u16Blend;

    MS_U8 u8Max[2], u8Min[2];
    MS_U16 u16PreTarget[2], u16TmpTarget[2];
    MS_U8 u8Start, u8End;
    MS_U8 i;
    MS_U16 u16Tmp;

    // control register
    u8Reg = Hal_WBLE_get_sw_ble_method();

    bEnableBLE = u8Reg & BIT(0);
    switch(u8Reg & (BIT(1)|BIT(2)))
    {
    case 0x00:
        u8Method = BLE_METHOD_FIX_START;
        u8Start = BLE_SLOP_PARAM;
        u8End   = BLE_SLOP_PARAM;
        break;
    case BIT(1):
        u8Method = BLE_METHOD_FIX_SLOP;
        u8Start = BLE_START_PARAM;
        u8End   = BLE_START_PARAM;
        Hal_WBLE_set_ble_slop(u8SlopeDefalut);
        break;

    default:
    case BIT(2):
        u8Method = BLE_METHOD_DYNAMIC_START_SLOP;
        u8Start = BLE_SLOP_PARAM;
        u8End   = BLE_START_PARAM;
        break;
    }

    if(bEnableBLE)
    {
        u8Strength = Hal_WBLE_get_ble_stregth();
        u8Step = Hal_WBLE_get_ble_step();

        u16Tmp = Hal_WBLE_get_ble_slop_boundary();
        u8Max[BLE_SLOP_PARAM]  = (MS_U8)((u16Tmp & 0xFF00)>>8);
        u8Min[BLE_SLOP_PARAM]  = (MS_U8)(u16Tmp & 0x00FF);

        u16Tmp = Hal_WBLE_get_ble_start_boundary();
        u8Max[BLE_START_PARAM] = (MS_U8)((u16Tmp & 0xFF00)>>8);
        u8Min[BLE_START_PARAM] = (MS_U8)(u16Tmp & 0x00FF);

        if(u8Method & BLE_METHOD_FIX_SLOP)
        { // fix slope
            MS_U8 u8Tmp;
            u8Tmp = (u8AvgY > 0x10) ? (u8AvgY - 0x10) : 0x00;
            u16TmpTarget[BLE_START_PARAM]= (MS_U32)u8Tmp * (MS_U32)u8Strength / 128 + 0x00;
            u16PreTarget[BLE_START_PARAM] = u16PreBLEStart;
        }

        if(u8Method & BLE_METHOD_FIX_START)
        { // fix start
            u16TmpTarget[BLE_SLOP_PARAM] = (MS_U32)u8AvgY * (MS_U32)u8Strength / 128 + 0x70;
            u16PreTarget[BLE_SLOP_PARAM] = u16PreBLESlope;
        }


        // de-flicker
        if (u16PreHistogramTotal >= u16YTotalSum)
        {
            if ((u16PreHistogramTotal - u16YTotalSum) < Hal_WBLE_get_ble_delta())
                u8FlickAlpha = Hal_WBLE_get_ble_flickalpha();
            else
                u8FlickAlpha = BLE_DEFLICK_BLEND_FACTOR;
        }
        else
        {
            if ((u16YTotalSum - u16PreHistogramTotal) < Hal_WBLE_get_ble_delta())
                u8FlickAlpha = Hal_WBLE_get_ble_flickalpha();
            else
                u8FlickAlpha = BLE_DEFLICK_BLEND_FACTOR;
        }

        u16PreHistogramTotal = u16YTotalSum;

        for(i=u8Start; i<=u8End; i++)
        {
            if(u16TmpTarget[i] > (MS_U16)u8Max[i])
                u8NewTarget = u8Max[i];
            else if(u16TmpTarget[i] < (MS_U16)u8Min[i])
                u8NewTarget = u8Min[i];
            else
                u8NewTarget = u16TmpTarget[i];

            u16Blend =(MS_U16)( (((MS_U32)u8FlickAlpha * (MS_U32)u8NewTarget * (MS_U32)BLE_DEFLICK_PRECISION_SHIFT) +
                              (((MS_U32)BLE_DEFLICK_BLEND_FACTOR-(MS_U32)u8FlickAlpha) * (MS_U32)u16PreTarget[i])) / BLE_DEFLICK_BLEND_FACTOR);

            // the difference can't be too big
            if(u16Blend > u16PreTarget[i])
            {
                if((u16Blend - u16PreTarget[i])> (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step))
                    u16Blend = u16PreTarget[i] + (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step);
            }
            else if(u16Blend < u16PreTarget[i])
            {
                if((u16PreTarget[i] - u16Blend)> (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step))
                    u16Blend = u16PreTarget[i] - (BLE_DEFLICK_PRECISION_SHIFT * (MS_U16)u8Step);
            }

            if(i == BLE_SLOP_PARAM)
            {
                u16PreBLESlope = u16Blend;
            }
            else
            {
                u16PreBLEStart = u16Blend;
            }


            u8NewTarget = (MS_U8)(u16Blend / BLE_DEFLICK_PRECISION_SHIFT);

            if(i == BLE_SLOP_PARAM)
            {
                Hal_WBLE_set_ble_slop(u8NewTarget);
            }
            if(i == BLE_START_PARAM)
            {
               Hal_WBLE_set_ble_start(u8NewTarget);
            }
        }
    }
    else
    {
        Hal_WBLE_set_ble_slop(0x80);
        Hal_WBLE_set_ble_start(0x70);
    }
}


void MDrv_WBLE_Set_SlopeValue(MS_U8 u8Slope)
{
    u8SlopeDefalut = u8Slope;

}

