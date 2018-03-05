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
#define MHAL_PQ_CUS_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvPQ_Define.h"
#include "k6_Main.h"             // table config parameter
#include "k6_Sub.h"              // table config parameter
#include "k6_SC1_Main.h"         // table config parameter
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

extern MS_U16 _u16PQDbgSwitch;
#define PQ_CUS_HAL_DBG(x) do { if( _u16PQDbgSwitch) x; } while(0);

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

MS_U16 Hal_PQ_get_420_cup_idx(void *pInstance,MS_420_CUP_TYPE etype)
{
    MS_U16 u16ret;

    switch(etype)
    {
    case MS_420_CUP_OFF:
        u16ret = PQ_IP_420CUP_OFF_Main;
        break;

    case MS_420_CUP_ON:
        u16ret = PQ_IP_420CUP_ON_Main;
        break;

    default:
        u16ret = 0xFFFF;
        break;
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_422To444_idx(void *pInstance,MS_422To444_TYPE etype)
{
    MS_U16 u16ret;
    switch(etype)
    {
    case MS_422To444_ON:
        u16ret = PQ_IP_422To444_ON_Main;
        break;
    case MS_422To444_OFF:
        u16ret = PQ_IP_422To444_OFF_Main;
        break;

    default:
        u16ret = 0xFFFF;
        PQ_CUS_HAL_DBG(printf("!!Invalid!!, Please check the index in [%s]\n", __FUNCTION__));
        break;
    }
    return u16ret;
}

MS_U8 Hal_PQ_get_422To444_mode(void *pInstance,MS_BOOL bMemFmt422)
{
    if(bMemFmt422)
    {
        return MS_422To444_ON;
    }
    else
    {
        return MS_422To444_OFF;
    }
}

MS_U16 Hal_PQ_get_madi_idx(void *pInstance,MS_BOOL bMainWin, MS_MADI_TYPE etype)
{
    MS_U16 u16ret;

    if(bMainWin == FALSE)
    {
        switch(etype)
        {
        case MS_MADI_24_4R:
            u16ret = PQ_IP_MADi_24_4R_Sub;
            break;
        case MS_MADI_24_2R:
            u16ret = PQ_IP_MADi_24_2R_Sub;
            break;
        case MS_MADI_25_4R:
            u16ret = PQ_IP_MADi_25_4R_Sub;
            break;
        case MS_MADI_25_2R:
            u16ret = PQ_IP_MADi_25_2R_Sub;
            break;
        case MS_MADI_27_4R:
            u16ret = PQ_IP_MADi_27_4R_Sub;
            break;
        case MS_MADI_27_2R:
            u16ret = PQ_IP_MADi_27_2R_Sub;
            break;
        case MS_MADI_P_MODE8:
            u16ret = PQ_IP_MADi_P_MODE8_Sub;
            break;
        case MS_MADI_P_MODE10:
            u16ret = PQ_IP_MADi_P_MODE10_Sub;
            break;
#if 0
        case MS_MADI_P_MODE_MOT10:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_Sub;
            break;
        case MS_MADI_P_MODE_MOT8:
            u16ret = PQ_IP_MADi_P_MODE_MOT8_Sub;
            break;
        case MS_MADI_P_MODE10_444:
            u16ret = PQ_IP_MADi_P_MODE10_444_Sub;
            break;
        case MS_MADI_P_MODE8_444:
            u16ret = PQ_IP_MADi_P_MODE8_444_Sub;
            break;
        case MS_MADi_25_4R_I:
        	u16ret = PQ_IP_MADi_25_4R_I_Sub;
        	break;
        case MS_MADi_25_4R_P:
        	u16ret = PQ_IP_MADi_25_4R_P_Sub;
        	break;
#endif
        default:
            u16ret = 0xFFFF;
			PQ_CUS_HAL_DBG(printf("!!Invalid Sub!!, Please check the index in [%s]\n", __FUNCTION__));
            break;
        }
    }
    else
    {
        switch(etype)
        {
        case MS_MADI_25_4R:
            u16ret = PQ_IP_MADi_25_4R_Main;
            break;
#if 0
        case MS_MADI_25_4R_MC:
            u16ret = PQ_IP_MADi_25_4R_MC_Main;
            break;
        case MS_MADI_25_6R_MC:
            u16ret = PQ_IP_MADi_25_6R_MC_Main;
            break;
        case MS_MADI_25_14F_6R_MC:                     //Add New MADi mode
            u16ret = PQ_IP_MADi_25_14F_6R_MC_Main;
            break;
#endif
        case MS_MADI_25_2R:
            u16ret = PQ_IP_MADi_25_2R_Main;
            break;
        case MS_MADI_27_4R:
            u16ret = PQ_IP_MADi_27_4R_Main;
            break;
        case MS_MADI_27_2R:
            u16ret = PQ_IP_MADi_27_2R_Main;
            break;
        case MS_MADI_P_MODE8:
            u16ret = PQ_IP_MADi_P_MODE8_Main;
            break;
        case MS_MADI_P_MODE10:
            u16ret = PQ_IP_MADi_P_MODE10_Main;
            break;
        case MS_MADI_P_MODE8_NO_MIU:
            u16ret = PQ_IP_MADi_P_MODE8_NO_MIU_Main;
            break;
#if 0
        case MS_MADI_P_MODE_MOT10:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_Main;
            break;

        case MS_MADI_P_MODE_MC_3Frame:
            u16ret = PQ_IP_MADi_P_MODE_MC_3Frame_Main;
            break;

        case MS_MADI_P_MODE_MOT8:
            u16ret = PQ_IP_MADi_P_MODE_MOT8_Main;
            break;
#endif
        case MS_MADI_24_4R:
            u16ret = PQ_IP_MADi_24_4R_Main;
            break;

        case MS_MADI_24_2R:
            u16ret = PQ_IP_MADi_24_2R_Main;
            break;
#if 0
        case MS_MADI_26_4R:
            u16ret = PQ_IP_MADi_26_4R_Main;
            break;

        case MS_MADI_26_2R:
            u16ret = PQ_IP_MADi_26_2R_Main;
            break;

        case MS_MADI_24_4R_880:
            u16ret = PQ_IP_MADi_24_4R_880_Main;
            break;

        case MS_MADI_24_2R_880:
            u16ret = PQ_IP_MADi_24_2R_880_Main;
            break;
        case MS_MADI_25_4R_MC_NW:
            u16ret = PQ_IP_MADi_25_4R_MC_NW_Main;
            break;

        case MS_MADI_25_4R_884:
            u16ret = PQ_IP_MADi_25_4R_884_Main;
            break;

        case MS_MADI_25_4R_880:
            u16ret = PQ_IP_MADi_25_4R_880_Main;
            break;

        case MS_MADI_25_2R_884:
            u16ret = PQ_IP_MADi_25_2R_884_Main;
            break;

        case MS_MADI_25_2R_880:
            u16ret = PQ_IP_MADi_24_2R_880_Main;
            break;

        case MS_MADI_25_6R_MC_NW:
            u16ret = PQ_IP_MADi_25_6R_MC_NW_Main;
            break;

        case MS_MADI_25_12F_8R_MC:
            u16ret = PQ_IP_MADi_25_12F_8R_MC_Main;
            break;

        case MS_MADI_25_14F_8R_MC:
            u16ret = PQ_IP_MADi_25_14F_8R_MC_Main;
            break;

        case MS_MADI_25_16F_8R_MC:
            u16ret = PQ_IP_MADi_25_16F_8R_MC_Main;
            break;

        case MS_MADI_P_MODE8_444:
        	u16ret = PQ_IP_MADi_P_MODE8_444_Main;
        	break;
        case MS_MADI_P_MODE10_444:
        	u16ret = PQ_IP_MADi_P_MODE10_444_Main;
        	break;

        case MS_MADI_P_MODE_MOT10_8Frame:
        	u16ret = PQ_IP_MADi_P_MODE_MOT10_8Frame_Main;
        	break;

        case MS_MADI_P_MODE_MOT10_4Frame:
        	u16ret = PQ_IP_MADi_P_MODE_MOT10_4Frame_Main;
        	break;

        case MS_MADI_RFBL_3D:
        	u16ret = PQ_IP_MADi_RFBL_3D_Main;
        	break;

        case MS_MADI_25_6F_4R_MC_I:
        	u16ret = PQ_IP_MADi_25_6F_4R_MC_I_Main;
        	break;

        case MS_MADI_25_6F_4R_MC_P:
        	u16ret = PQ_IP_MADi_25_6F_4R_MC_P_Main;
        	break;
#endif
        default:
            u16ret = 0xFFFF;
			PQ_CUS_HAL_DBG(printf("!!Invalid Main!!, Please check the index in [%s]\n", __FUNCTION__));
            break;
        }
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_csc_ip_idx(void *pInstance,MS_CSC_IP_TYPE enCSC)
{
    MS_U16 u16Ret;

    switch(enCSC)
    {
    case MS_CSC_IP_CSC:
        u16Ret = PQ_IP_CSC_Main;
        break;

    default:
    case MS_CSC_IP_VIP_CSC:
        u16Ret = 0xFFFF;
        break;
    }
    return u16Ret;
}

MS_U16 Hal_PQ_get_ip_idx(void *pInstance,MS_PQ_IP_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_PQ_IP_VD_SAMPLING:
        u16ret = PQ_IP_VD_Sampling_no_comm_Main;
        break;
    case MS_PQ_IP_HSD_SAMPLING:
        u16ret = PQ_IP_HSD_Sampling_Main;
        break;
    case MS_PQ_IP_ADC_SAMPLING:
        u16ret = PQ_IP_ADC_Sampling_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}

MS_U16 Hal_PQ_get_hsd_sampling_idx(void *pInstance,MS_HSD_SAMPLING_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_HSD_SAMPLING_Div_1_000:
        u16ret = PQ_IP_HSD_Sampling_Div_1o000_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_125:
        u16ret = PQ_IP_HSD_Sampling_Div_1o125_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_250:
        u16ret = PQ_IP_HSD_Sampling_Div_1o250_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_375:
        u16ret = PQ_IP_HSD_Sampling_Div_1o375_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_500:
        u16ret = PQ_IP_HSD_Sampling_Div_1o500_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_625:
        u16ret = PQ_IP_HSD_Sampling_Div_1o625_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_750:
        u16ret = PQ_IP_HSD_Sampling_Div_1o750_Main;
        break;

    case MS_HSD_SAMPLING_Div_1_875:
        u16ret = PQ_IP_HSD_Sampling_Div_1o875_Main;
        break;

    case MS_HSD_SAMPLING_Div_2_000:
        u16ret = PQ_IP_HSD_Sampling_Div_2o000_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}

MS_U16 Hal_PQ_get_adc_sampling_idx(void *pInstance,MS_ADC_SAMPLING_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_ADC_SAMPLING_X_1:
        u16ret = PQ_IP_ADC_Sampling_x1_Main;
        break;

    case MS_ADC_SAMPLING_X_2:
        u16ret = PQ_IP_ADC_Sampling_x2_Main;
        break;
    case MS_ADC_SAMPLING_X_4:
        u16ret = PQ_IP_ADC_Sampling_x4_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}

