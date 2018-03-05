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
#include "UFO.h"
#include "halPWS.h"
#include "MsOS.h"
#include "regPWS.h"
#include "mhal_pws_table.h"
#include "mhal_pws_setting_info_table.h"
#include "mhal_pws_scene_info_table.h"

#if ((defined(UFO_XC_DAC_POWERCONTROL)) && (!defined(CONFIG_MBOOT)))
#include "apiDAC.h"
#include "apiXC_EX.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include <pthread.h>
#endif

//------------------------------------------------------------------------------
// Define & data type
//------------------------------------------------------------------------------
#define DEBUG_INFO(y)    //y

#define RunBefore   0
#define RunAfter    1
//------------------------------------------------------------------------------
// Local variable
//------------------------------------------------------------------------------
static MS_PHYADDR _gPWS_MapBase = 0xA0000000;
static MS_U8 _gu8MainWindow = E_PWS_IP_OFF;
static MS_U8 _gu8SubWindow = E_PWS_IP_OFF;
//------------------------------------------------------------------------------
// Local Function
//------------------------------------------------------------------------------
MS_U8 HAL_PWS_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gPWS_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_U16 HAL_PWS_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gPWS_MapBase))[u32RegAddr];
}

MS_U32 HAL_PWS_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_PWS_Read2Byte(u32RegAddr) | HAL_PWS_Read2Byte(u32RegAddr+2) << 16);
}

MS_BOOL HAL_PWS_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gPWS_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL HAL_PWS_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gPWS_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

MS_BOOL HAL_PWS_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    HAL_PWS_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_PWS_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

MS_BOOL HAL_PWS_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8BitPos)
{
    MS_U8 u8Val = HAL_PWS_ReadByte(u32RegAddr);
    if (!u32RegAddr)
    {
        return FALSE;
    }

    u8Val = HAL_PWS_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8BitPos) : (u8Val & ~u8BitPos);
    HAL_PWS_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

E_PWS_Result HAL_PWS_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    return (E_PWS_Result)HAL_PWS_WriteByte(u32RegAddr, (HAL_PWS_ReadByte(u32RegAddr) & (~u8Mask)) | (u8Val & u8Mask));
}

void HAL_PWS_RunCallback(MS_U8 Position,MS_U8 OnOff_flag, MS_U16 index)
{

}

E_PWS_Result HAL_PWS_SetXXX(MS_U8 OnOff_flag, MS_U16 index)
{
    return PWS_SUCCESS;
}

#if ((defined(UFO_XC_DAC_POWERCONTROL)) && (!defined(CONFIG_MBOOT)))

MS_BOOL bThreadIsOpen = FALSE;
static void _SetDACFullVolt(MS_BOOL bFullOut, MS_BOOL bHDDAC)
{
    MApi_DAC_SetIHalfOutput(!bFullOut, bHDDAC);
    MApi_DAC_SetQuartOutput(!bFullOut, bHDDAC);
}

static pthread_t hPowerMonitorThread;
static void *PwsPowerMonitorThread(void *pThreadInfo)
{
    MS_BOOL bVEIsClockOn = FALSE;
    MS_BOOL bCVBSIsPlugIn = TRUE;
    MS_BOOL bHDGENIsClockOn = FALSE;
    MS_BOOL bYPbPrIsPlugIn = TRUE;

    while(1)
    {
        MsOS_DelayTaskUs(500000);

        if(((HAL_PWS_Read2Byte(REG_CKG_VE)&(CKG_VE_CLK_DISABLE|CKG_VE_13_CLK_DISABLE|CKG_VEDEC_CLK_DISABLE))!= 0x00) || ((HAL_PWS_Read2Byte(REG_CKG_VE_IN) & (CKG_VE_IN_CLK_DISABLE)) != 0x00))
            bVEIsClockOn = FALSE;
        else
            bVEIsClockOn = TRUE;

        if(bVEIsClockOn)  //Detect Plug Out
        {
            _SetDACFullVolt(TRUE, FALSE);
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGOUT, &bCVBSIsPlugIn);

            if(!bCVBSIsPlugIn)
            {
                _SetDACFullVolt(FALSE, FALSE);

                HAL_PWS_WriteByte(REG_CKG_DAC1, ((HAL_PWS_ReadByte(REG_CKG_DAC1) & ~CKG_DAC1_MASK) | CKG_DAC1_XTALI));

                HAL_PWS_Write2Byte((REG_CKG_VE), HAL_PWS_Read2Byte(REG_CKG_VE) | (CKG_VE_CLK_DISABLE | CKG_VE_13_CLK_DISABLE | CKG_VEDEC_CLK_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_VE_IN), HAL_PWS_Read2Byte(REG_CKG_VE_IN)|(CKG_VE_IN_CLK_DISABLE));

                HAL_PWS_Write2Byte((REG_CKG_SC1_IDCLK), HAL_PWS_Read2Byte(REG_CKG_SC1_IDCLK)|(CKG_SC1_IDCLK_F2_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_SC1_SID_ED_F_CLK), HAL_PWS_Read2Byte(REG_CKG_SC1_SID_ED_F_CLK) | (CKG_SC1_EDCLK_DISABLE | CKG_SC1_FCLK_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_SC1_SID2_OD_CLK), HAL_PWS_Read2Byte(REG_CKG_SC1_SID2_OD_CLK) | (CKG_SC1_SIDCLK_F2_DISABLE | CKG_SC1_OD_CLK_DISABLE));

                printf("CVBS Plug-out, Close VE SC1 clock\n");
            }
        }
        else              //Detect Plug In
        {
            _SetDACFullVolt(FALSE, FALSE);
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_SD, E_DAC_DETECT_PLUGIN, &bCVBSIsPlugIn);

            if(bCVBSIsPlugIn)
            {
                _SetDACFullVolt(TRUE, FALSE);

                HAL_PWS_WriteByte(REG_CKG_DAC1, (HAL_PWS_ReadByte(REG_CKG_DAC1) & ~CKG_DAC1_MASK) | CKG_DAC1_CLK_VEDAC);

                HAL_PWS_Write2Byte((REG_CKG_VE), HAL_PWS_Read2Byte(REG_CKG_VE) & ~(CKG_VE_CLK_DISABLE | CKG_VE_13_CLK_DISABLE | CKG_VEDEC_CLK_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_VE_IN), HAL_PWS_Read2Byte(REG_CKG_VE_IN) & ~(CKG_VE_IN_CLK_DISABLE));

                HAL_PWS_Write2Byte((REG_CKG_SC1_IDCLK), HAL_PWS_Read2Byte(REG_CKG_SC1_IDCLK) & ~ (CKG_SC1_IDCLK_F2_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_SC1_SID_ED_F_CLK), HAL_PWS_Read2Byte(REG_CKG_SC1_SID_ED_F_CLK) & ~(CKG_SC1_EDCLK_DISABLE | CKG_SC1_FCLK_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_SC1_SID2_OD_CLK), HAL_PWS_Read2Byte(REG_CKG_SC1_SID2_OD_CLK) & ~(CKG_SC1_SIDCLK_F2_DISABLE | CKG_SC1_OD_CLK_DISABLE));

                printf("CVBS Plug-in, Open VE SC1  clock\n");
            }
        }

        if(((HAL_PWS_Read2Byte(REG_CKG_HDGEN) & (CKG_HDGEN_CLK_DISABLE|CKG_HDGEN_FILTER_CLK_DISABLE)) != 0x00) || ((HAL_PWS_Read2Byte(REG_CKG_HDGEN_IN) & (CKG_HDGEN_IN_CLK_DISABLE)) != 0x00))
            bHDGENIsClockOn = FALSE;
        else
            bHDGENIsClockOn = TRUE;

        if(bHDGENIsClockOn)  //Detect Plug Out
        {
            _SetDACFullVolt(TRUE, TRUE);
            MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_LOW, TRUE);
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_HD, E_DAC_DETECT_PLUGOUT, &bYPbPrIsPlugIn);

            if(!bYPbPrIsPlugIn)
            {
                _SetDACFullVolt(FALSE, TRUE);

                HAL_PWS_WriteByte(REG_CKG_DAC2, ((HAL_PWS_ReadByte(REG_CKG_DAC2) & ~CKG_DAC2_MASK) | CKG_DAC2_XTALI));

                HAL_PWS_Write2Byte(REG_CKG_HDGEN, HAL_PWS_Read2Byte(REG_CKG_HDGEN) | (CKG_HDGEN_CLK_DISABLE | CKG_HDGEN_FILTER_CLK_DISABLE));
                HAL_PWS_Write2Byte(REG_CKG_HDGEN_IN, HAL_PWS_Read2Byte(REG_CKG_HDGEN_IN) | (CKG_HDGEN_IN_CLK_DISABLE));

                printf("YPbPr Plug-out, Close HDGEN clock\n");
            }
        }
        else              //Detect Plug In
        {
            _SetDACFullVolt(FALSE, TRUE);
            MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, TRUE);
            MApi_DAC_HotPlugDetect(E_DAC_DETECT_HD, E_DAC_DETECT_PLUGIN, &bYPbPrIsPlugIn);

            if(bYPbPrIsPlugIn)
            {
                _SetDACFullVolt(TRUE, TRUE);

                HAL_PWS_WriteByte(REG_CKG_DAC2, (HAL_PWS_ReadByte(REG_CKG_DAC2) & ~CKG_DAC2_MASK) | CKG_DAC2_CLK_HDGEN_FILTER);

                HAL_PWS_Write2Byte((REG_CKG_HDGEN), HAL_PWS_Read2Byte(REG_CKG_HDGEN) & ~(CKG_HDGEN_CLK_DISABLE | CKG_HDGEN_FILTER_CLK_DISABLE));
                HAL_PWS_Write2Byte((REG_CKG_HDGEN_IN), HAL_PWS_Read2Byte(REG_CKG_HDGEN_IN) & ~(CKG_HDGEN_IN_CLK_DISABLE));

                printf("YPbPr Plug-in, Open HDGEN clock\n");
            }
        }
    }
    return NULL;
}
#endif

void HAL_PWS_MonitorPowerProc(void)
{
    #if ((defined(UFO_XC_DAC_POWERCONTROL)) && (!defined(CONFIG_MBOOT)))
    int dwErrorCode = 0;
    char sPwsThreadName[25] = "MstarMonitorPowerThread\0";
    pthread_attr_t attr;
    if(!bThreadIsOpen)
    {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        dwErrorCode = pthread_create(
                                     &hPowerMonitorThread,
                                     &attr,
                                     PwsPowerMonitorThread,
                                     (void *) sPwsThreadName);
        if(dwErrorCode)
        {
            bThreadIsOpen = FALSE;
            printf("\033[33m[ERROR] !!Create MonitorPower Thread Failed\033[m\n");
        }
        else
        {
            bThreadIsOpen = TRUE;
        }
    }
    #endif
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

E_PWS_Result HAL_PWS_ScenePowerControl(E_PWS_SCENE_name eSceneName, E_PWS_IpPowerCtrl pwr_type)
{
    MS_U32 i;
    MS_BOOL bWriteRet;

    bWriteRet = TRUE;

    if (eSceneName == E_PWS_SCENE_MM_MAIN)
    {
        _gu8MainWindow = pwr_type;
    }

    if (eSceneName == E_PWS_SCENE_MM_SUB)
    {
        _gu8SubWindow = pwr_type;
    }

    if (_gu8MainWindow == E_PWS_IP_ON || _gu8SubWindow == E_PWS_IP_ON)
    {
        eSceneName = E_PWS_SCENE_MM_MAIN;
        pwr_type = E_PWS_IP_ON;
    }
    if (_gu8MainWindow == E_PWS_IP_OFF && _gu8SubWindow == E_PWS_IP_OFF)
    {
        eSceneName = E_PWS_SCENE_MM_MAIN;
        pwr_type = E_PWS_IP_OFF;
    }

    for (i = 0; pws_scene_info_table[i].eSceneName != E_PWS_SCENE_MAX; ++i)
    {
        if ( pws_scene_info_table[i].eSceneName == eSceneName && pws_scene_info_table[i].eIpPowerCtrl == pwr_type)
        {
            bWriteRet = HAL_PWS_WriteByteMask(pws_scene_info_table[i].u32RegAddr, pws_scene_info_table[i].u8RegVal, pws_scene_info_table[i].u8RegMask);

            if (bWriteRet != TRUE)
            {
                return PWS_FAIL;
            }
        }
    }
    for (i = 0; pws_scene_cb[i].eSceneName != E_PWS_SCENE_MAX; ++i)
    {
        if ( pws_scene_cb[i].eSceneName == eSceneName && pws_scene_cb[i].eIpPowerCtrl == pwr_type)
        {
            if (pws_scene_cb[i].callback != NULL)
                pws_scene_cb[i].callback();
		}
	}
    return PWS_SUCCESS;
}
