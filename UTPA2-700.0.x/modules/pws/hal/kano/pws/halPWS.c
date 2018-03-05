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
////////////////////////////////////////////////////////////////////////////////
#define _HAL_PWS_C

////////////////////////////////////////////////////////////////////////////////
/// @file halPWS.c
/// @author MStar Semiconductor Inc.
/// @brief power saving driver
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "MsCommon.h"
#include "MsTypes.h"
#include "halPWS.h"
#include "MsOS.h"

#include "mhal_pws_table.h"
#include "mhal_pws_setting_info_table.h"
//------------------------------------------------------------------------------
// Define & data type
//------------------------------------------------------------------------------
#define DEBUG_INFO(y)    //y

#define RunBefore   0
#define RunAfter    1
//------------------------------------------------------------------------------
// Local variable
//------------------------------------------------------------------------------
static MS_U32 _gPWS_MapBase = 0xA0000000;

//------------------------------------------------------------------------------
// Local Function
//------------------------------------------------------------------------------

MS_U16 HAL_PWS_Read2Byte(MS_U32 u32RegAddr)
{
    return 0;
}

E_PWS_Result HAL_PWS_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    return PWS_SUCCESS;
}

void HAL_PWS_RunCallback(MS_U8 Position,MS_U8 OnOff_flag, MS_U16 index)
{

}

E_PWS_Result HAL_PWS_SetXXX(MS_U8 OnOff_flag, MS_U16 index)
{
    return PWS_SUCCESS;
}

//------------------------------------------------------------------------------
// Global Function
//------------------------------------------------------------------------------
void HAL_PWS_SetIOMapBase(MS_U32 u32Base)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(u32Base == 0)
    {
        printf("Get PWS RIU base address failed\r\n");
    }
    else
    {
        _gPWS_MapBase = u32Base;
        DEBUG_INFO(printf("PWS_MAP_BASE=%lx\n",_gPWS_MapBase));
    }
}

MS_U32 HAL_PWS_GetIOMapBase(void)
{
    return _gPWS_MapBase;
}

MS_U8 HAL_PWS_CheckChipID(void)
{
    return TRUE;
}

E_PWS_SouceInfo  HAL_PWS_GetSourceInfo(void)
{;
    return _NO_SOURCE_;
}

void HAL_PWS_Init(E_PWS_VIF_type eVifType)
{

}

E_PWS_Result HAL_PWS_HandleSource(E_PWS_Operation operation,E_PWS_SouceInfo source)
{
    return PWS_SUCCESS;
}

E_PWS_Result HAL_PWS_HandleIP(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName)
{
    return PWS_SUCCESS;
}

E_PWS_Result HAL_PWS_RegisterCallback(MS_U8 index, E_PWS_CallbackCtrl cb_ctrl, unsigned char *RegName, P_PWS_Callback pfCallback)
{
    return PWS_SUCCESS;
}

E_PWS_Result HAL_PWS_CancelCallback(MS_U8 index)
{
    return PWS_SUCCESS;
}

void HAL_PWS_Stop_VDMCU(void)
{

}

E_PWS_Result HAL_PWS_IpPowerControl(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type)
{
    return PWS_SUCCESS;
}
