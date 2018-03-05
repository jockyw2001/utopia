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
#define DRV_SCALER_NR_C
/******************************************************************************/
/*                           Header Files                                     */
/******************************************************************************/
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
//#include "Debug.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "mvideo_context.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "mhal_xc_chip_config.h"
#include "mhal_sc.h"

#if TEST_NEW_DYNAMIC_NR

/******************************************************************************/
/*                           Constants                                        */
/******************************************************************************/

/******************************************************************************/
/*                           Local Prototypes                                 */
/******************************************************************************/
#define DynamicNR_DBG(x) //x

/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/


/******************************************************************************/
///Enable/Disable 3DNR
///@param bEnable \b IN: enable/disable
/******************************************************************************/
void MDrv_Scaler_Enable3DNR(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (gSrcInfo[eWindow].bForceNRoff) // force NR to be disabled when GWIN is consuming bandwidth in HD modes
    {
        bEnable = DISABLE;
    }

    HAL_SC_enable_dnr(pInstance, 0x01, eWindow);
    HAL_SC_enable_field_avg_y(pInstance, bEnable, eWindow); //Main Window Y average mode when dotline cycle
    HAL_SC_enable_field_avg_c(pInstance, DISABLE, eWindow); //Main Window C average mode when dotline cycle
                                                 //always DISABLE
}


void MDrv_Scaler_EnableNR(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow)
{
    if (bEn == 0)
    {
        mvideo_sc_set_3dnr(pInstance, DISABLE, eWindow);
        MDrv_Scaler_Enable3DNR(pInstance, DISABLE, eWindow);
    }
    else
    {
        mvideo_sc_set_3dnr(pInstance, ENABLE, eWindow);
        // NR start to work immediately, but it does not fetch memory until next frame
        // enable NR in vblank to avoid wrong result
        MDrv_XC_wait_input_vsync(pInstance, 1, 50, eWindow);
        MDrv_Scaler_Enable3DNR(pInstance, ENABLE, eWindow);
    }

//T2 doesn't have Pre CCS
//    if( MApi_XC_IsYUVSpace() )
//        MDrv_Scaler_SetCNR(MS_CNR_ON);
//    else
//        MDrv_Scaler_SetCNR(MS_CNR_OFF);

//    MDrv_Scaler_SetDNR(MS_DNR_OFF);
}

#endif  //TEST_NEW_DYNAMIC_NR

