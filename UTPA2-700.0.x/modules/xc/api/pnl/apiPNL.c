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
#ifndef _API_XC_PANEL_C_
#define _API_XC_PANEL_C_

/******************************************************************************/
/*                      Includes                                              */
/******************************************************************************/
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
// wyc.test-131206
#include "halPNL.h"

#include "PNL_private.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

/******************************************************************************/
/*                      Debug information                                      */
/******************************************************************************/

#define PNL_DBG_MSG(fmt,...)                                                                 \
    MS_DEBUG_MSG(do{                                                                                      \
        if(_u16PnlDbgSwitch){                                                                   \
           printf( "[PNL_DBG_MSG]: %s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
          }                                                                                  \
      } while(0))

#define PNL_PRINT_VAR(var)                                                                   \
    do{                                                                                      \
        if(_u16PnlDbgSwitch){                                                                   \
          PNL_DBG_MSG("%30s: %d\n", #var, (var));                                            \
          }                                                                                  \
      }while(0)

#define PNL_PRINT_FUN(fun)                                                                   \
    do{                                                                                      \
        if(_u16PnlDbgSwitch){                                                                   \
            PNL_DBG_MSG("\n");                                                               \
            PNL_DBG_MSG("**************************************************\n");             \
            PNL_DBG_MSG("*\t\t%s \t\t\n", #fun);                                             \
            PNL_DBG_MSG("**************************************************\n");             \
            fun;                                                                             \
            PNL_DBG_MSG("*                                                *\n");             \
            PNL_DBG_MSG("*                                                *\n");             \
            PNL_DBG_MSG("**************************************************\n");             \
          }                                                                                  \
       }while(0)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#ifndef XC_SUPPORT_AUTO_VSYNC
#define XC_SUPPORT_AUTO_VSYNC    0
#endif

#define PNL_SEMAPHORE_DBG 0
#define _pnl_dbg_semaphore E_PNL_POOL_ID_INTERNAL
// if anyone wants to see log inside android,
// please replace the "printf" with "ALOGE" so that it can be printed in android process
// OS related
#if (PNL_SEMAPHORE_DBG)
    #define _PNL_SEMAPHORE_ENTRY(pInstance,POOL_ID)                                                              \
            if (POOL_ID == _pnl_dbg_semaphore) \
                printf("1 [%s,%5d]==========================Prepare to get semaphore, PNL ID = %d\n",__func__,__LINE__,POOL_ID);      \
            if(MDrv_PNL_Get_Semaphore(pInstance,POOL_ID) != UTOPIA_STATUS_SUCCESS)                               \
            {                                                                                         \
                printf("[%s,%5d] Get PNL Semaphore failed\n",__FUNCTION__,__LINE__);                   \
            }                                                                                          \
            if (POOL_ID == _pnl_dbg_semaphore) \
                printf("[%s,%5d]Semaphore Got\n\n",__func__,__LINE__);

    #define _PNL_SEMAPHORE_RETURN(pInstance,POOL_ID)                                                             \
            if (POOL_ID == _pnl_dbg_semaphore) \
                printf("0 [%s,%5d]==========================prepare to release semaphore, PNL ID = %d\n",__func__,__LINE__,POOL_ID);  \
            MDrv_PNL_Release_Semaphore(pInstance,POOL_ID);                                                             \
            if (POOL_ID == _pnl_dbg_semaphore) \
                printf("[%s,%5d]Semaphore Returned\n\n",__func__,__LINE__);

#else
    #define _PNL_SEMAPHORE_ENTRY(pInstance,POOL_ID)                                                              \
            if(MDrv_PNL_Get_Semaphore(pInstance,POOL_ID) != UTOPIA_STATUS_SUCCESS)                               \
            {      }

    #define _PNL_SEMAPHORE_RETURN(pInstance,POOL_ID)                                                             \
                MDrv_PNL_Release_Semaphore(pInstance,POOL_ID);
#endif


#define PNL_WAIT_MUTEX              (TICK_PER_ONE_MS * 300)              // 300 ms

// Mutex function
#define _PNL_ENTRY()     _PNL_ENTRY_MUTEX(_s32PNLMutex)
#define _PNL_RETURN()    _PNL_RETURN_MUTEX(_s32PNLMutex)

#define _PNL_ENTRY_MUTEX(_mutex_)                                                                  \
        if(!OS_OBTAIN_MUTEX(_mutex_, PNL_WAIT_MUTEX))                     \
        {                                                                        \
            printf("==========================\n");                              \
            printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
        }
#define _PNL_RETURN_MUTEX(_mutex_)    OS_RELEASE_MUTEX(_mutex_);

/******************************************************************************/
/*                      Type Definition                                       */
/******************************************************************************/

/******************************************************************************/
/*                      Macro and Define                                      */
/******************************************************************************/
#define MAX_BACKLIGHT      100
#define PANEL_DE_VSTART    0 //8    // for STB, the DE Vstart must be 0

/******************************************************************************/
/*                      Private varialbes                                     */
/******************************************************************************/

    // Mutex & Lock
static MS_S32 _s32PNLMutex = -1;

static const MSIF_Version _api_pnl_version = { .DDI = { PNL_API_VERSION }, };

/******************************************************************************/
/*                      Function                                              */
/******************************************************************************/
extern MS_BOOL MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub(void);
/******************************************************************************/
/*                      Private functions                                     */
/******************************************************************************/
//
// set params from user defined panel
//
static void _MakeCompatibile(void* pInstance)
{
    MS_U8 VerSel[3], VerLib[3];

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    memcpy(VerLib, _api_pnl_version.DDI.version,     2);
    memcpy(VerSel, pPNLResourcePrivate->_api_pnl_version_sel.DDI.version, 2);

    VerLib[2] = VerSel[2] = '\0';

    if(strcmp( (const char*)VerSel, (const char*)VerLib ) != 0)
    {
        if ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_TTL)
        || (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_P)
        || ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_TYPE_MINILVDS)
            && (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type <= E_PNL_TYPE_DIGITAL_MINILVDS))
        || ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_EXT)
            && (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_TYPE_TTL_TCON))
        )
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOutTimingMode = E_PNL_CHG_HTOTAL;
        }
        else
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOutTimingMode = E_PNL_CHG_VTOTAL;
        }
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8PanelNoiseDith = TRUE;

        if( strcmp( (const char*)VerSel, "00" ) == 0 )
        {

            MS_CRITICAL_MSG(printf("wrong PanelType version, enter compatitlbe mode\n"));
        }
        else
        {
            MS_CRITICAL_MSG(printf("Panel Library mismatch(%s), please update to version %s\n", VerSel, VerLib));
        }
    }
    //Temp patch for MBOOT pnl lib and SN/Chara2 pnl lib are not same with DE Vstart
    if(pPNLResourcePrivate->stapiPNL._bSkipTimingChange == TRUE)
    {
        MS_U16 u16DEStart = MDrv_PNL_Get_DEVstart(pInstance);
        //Check DE Vstart
        if(u16DEStart != pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart)
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart = u16DEStart;
            printf("[Warning] %s:%d, Too old MBOOT PNL lib found, change DE Vstart as MBOOT's setting[%d]\n", __FUNCTION__, __LINE__, u16DEStart);
        }

        //Check DE Hstart
        u16DEStart = MDrv_PNL_Get_DEHstart(pInstance);
        if(u16DEStart != pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart)
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart = u16DEStart;
            printf("[Warning] %s:%d, Panel config mismatch, change DE Hstart as MBOOT's setting[%d]\n", __FUNCTION__, __LINE__, u16DEStart);
        }

    }
}

static PNL_TYPE _TransToDriver_APIPNL_LINK_TYPE_To_PNL_TYPE(APIPNL_LINK_TYPE enAPILinkType)
{
    PNL_TYPE enDriverLinkType = E_PNL_TYPE_TTL;

    switch(enAPILinkType)
    {
        case LINK_TTL:
            enDriverLinkType = E_PNL_TYPE_TTL;
            break;
        case LINK_RSDS:
            enDriverLinkType = E_PNL_TYPE_RSDS;
            break;
        case LINK_MINILVDS:
        case LINK_ANALOG_MINILVDS:
        case LINK_DIGITAL_MINILVDS:
            enDriverLinkType = E_PNL_TYPE_MINILVDS;
            break;
        case LINK_DAC_I:
            enDriverLinkType = E_PNL_TYPE_DAC_I;
            break;
        case LINK_DAC_P:
            enDriverLinkType = E_PNL_TYPE_DAC_P;
            break;
        default:
        case LINK_LVDS:
        case LINK_PDPLVDS:
            enDriverLinkType = E_PNL_TYPE_LVDS;
            break;
        case LINK_EXT:
        {
            printf("LINK TYPE = EXT \n");
            enDriverLinkType = E_PNL_TYPE_EXT;
        }
        break;
    }
    return enDriverLinkType;
}

static PNL_TYPE _TransToDriver_APIPNL_LINK_EXT_TYPE_To_PNL_TYPE(APIPNL_LINK_EXT_TYPE enLPLL_TypeExt)
{
    PNL_TYPE enDriverLinkType = E_PNL_TYPE_EXT;

    switch(enLPLL_TypeExt)
    {
        default:
        case LINK_MINILVDS_5P_2L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_5P_2L;
            break;

        case LINK_MINILVDS_4P_2L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_4P_2L;
            break;

        case LINK_MINILVDS_3P_2L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_3P_2L;
            break;

        case LINK_MINILVDS_6P_1L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_6P_1L;
            break;

        case LINK_MINILVDS_5P_1L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_5P_1L;
            break;

        case LINK_MINILVDS_4P_1L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_4P_1L;
            break;

        case LINK_MINILVDS_3P_1L:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_3P_1L;
            break;

        case LINK_HF_LVDS:
            enDriverLinkType = E_PNL_TYPE_HF_LVDS;
            break;

        case LINK_HS_LVDS:
            enDriverLinkType = E_PNL_TYPE_HS_LVDS;
            break;

        case LINK_TTL_TCON:
            enDriverLinkType = E_PNL_TYPE_TTL_TCON;
            break;

        case LINK_MINILVDS_1CH_3P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_3P_6BIT;
            break;

        case LINK_MINILVDS_1CH_4P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_4P_6BIT;
            break;

        case LINK_MINILVDS_1CH_5P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_5P_6BIT;
            break;

        case LINK_MINILVDS_1CH_6P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_6P_6BIT;
            break;

        case LINK_MINILVDS_2CH_3P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_3P_6BIT;
            break;

        case LINK_MINILVDS_2CH_4P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_4P_6BIT;
            break;

        case LINK_MINILVDS_2CH_5P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_5P_6BIT;
            break;

        case LINK_MINILVDS_2CH_6P_6BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_6P_6BIT;
            break;

        case LINK_MINILVDS_1CH_3P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_3P_8BIT;
            break;

        case LINK_MINILVDS_1CH_4P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_4P_8BIT;
            break;

        case LINK_MINILVDS_1CH_5P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_5P_8BIT;
            break;

        case LINK_MINILVDS_1CH_6P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_1CH_6P_8BIT;
            break;

        case LINK_MINILVDS_2CH_3P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_3P_8BIT;
            break;

        case LINK_MINILVDS_2CH_4P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_4P_8BIT;
            break;

        case LINK_MINILVDS_2CH_5P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_5P_8BIT;
            break;

        case LINK_MINILVDS_2CH_6P_8BIT:
            enDriverLinkType = E_PNL_LPLL_MINILVDS_2CH_6P_8BIT;
            break;

        case LINK_EPI34_8P:
            enDriverLinkType = E_PNL_LPLL_EPI34_8P;
            break;

        case LINK_EPI28_8P:
            enDriverLinkType = E_PNL_LPLL_EPI28_8P;
            break;

        case LINK_EPI34_6P:
            enDriverLinkType = E_PNL_LPLL_EPI34_6P;
            break;

        case LINK_EPI28_6P:
            enDriverLinkType = E_PNL_LPLL_EPI28_6P;
            break;

        case LINK_HDMI_BYPASS_MODE:
            enDriverLinkType = E_PNL_LPLL_HDMI_BYPASS_MODE;
            break;

        case LINK_VBY1_10BIT_8LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_8LANE;
            break;

        case LINK_VBY1_8BIT_8LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_8LANE;
            break;
        //Following codes need check!!!
        //In current refine, disable these codes for keeping logic.
        //To be checked:Following link type can't be transfered to driver in fact.

        case LINK_EPI34_2P:
            enDriverLinkType = E_PNL_LPLL_EPI34_2P;
            break;

        case LINK_EPI34_4P:
            enDriverLinkType = E_PNL_LPLL_EPI34_4P;
            break;

        case LINK_EPI28_2P:
            enDriverLinkType = E_PNL_LPLL_EPI28_2P;
            break;

        case LINK_EPI28_4P:
            enDriverLinkType = E_PNL_LPLL_EPI28_4P;
            break;

        case LINK_EPI28_12P:
            enDriverLinkType = E_PNL_LPLL_EPI28_12P;
            break;

        case LINK_EPI24_12P:
            enDriverLinkType = E_PNL_LPLL_EPI24_12P;
            break;

        case LINK_VBY1_10BIT_16LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_16LANE;
            break;

        case LINK_VBY1_10BIT_4LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_4LANE;
            break;

        case LINK_VBY1_10BIT_2LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_2LANE;
            break;

        case LINK_VBY1_10BIT_1LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_1LANE;
            break;

        case LINK_VBY1_8BIT_16LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_16LANE;
            break;

        case LINK_VBY1_8BIT_4LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_4LANE;
            break;

        case LINK_VBY1_8BIT_2LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_2LANE;
            break;

        case LINK_VBY1_8BIT_1LANE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_1LANE;
            break;

        case LINK_HS_LVDS_2CH_BYPASS_MODE:
            enDriverLinkType = E_PNL_LPLL_HS_LVDS_BYPASS_MODE;
            break;

        case LINK_VBY1_8BIT_4LANE_BYPASS_MODE:
            enDriverLinkType = E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE;
            break;

        case LINK_VBY1_10BIT_4LANE_BYPASS_MODE:
            enDriverLinkType = E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE;
            break;

        case LINK_USI_T_8BIT_12P:
            enDriverLinkType = E_PNL_LPLL_USI_T_8BIT_12P;
            break;

        case LINK_USI_T_10BIT_12P:
            enDriverLinkType = E_PNL_LPLL_USI_T_10BIT_12P;
            break;

        case LINK_ISP_8BIT_12P:
            enDriverLinkType = E_PNL_LPLL_ISP_8BIT_12P;
            break;

        case LINK_ISP_8BIT_6P_D:
            enDriverLinkType = E_PNL_LPLL_ISP_8BIT_6P_D;
            break;
    }
    return enDriverLinkType;
}

static MS_BOOL _SetPanelParams(void* pInstance, PanelType *pSelPanelType)
{
    /*  Make sure following equalities right, avoid m_wPanel Start not fit SyncWidth and BackPorch
    **  m_wPanelHStart = m_ucPanelHSyncWidth + m_ucPanelHSyncBackPorch
    **  m_wPanelVStart = m_ucPanelVSyncWidth + m_ucPanelVBackPorch
    */
    //pSelPanelType->m_wPanelHStart = pSelPanelType->m_ucPanelHSyncWidth + pSelPanelType->m_ucPanelHSyncBackPorch;
    //pSelPanelType->m_wPanelVStart = pSelPanelType->m_ucPanelVSyncWidth + pSelPanelType->m_ucPanelVBackPorch;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    /*  Calculate panel info according to panel_raw_data.
    *   can be factor out to diff functions.
    */
    // output Sync timing
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HTotal = pSelPanelType->m_wPanelHTotal;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VTotal = pSelPanelType->m_wPanelVTotal;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32MinSET = pSelPanelType->m_dwPanelMinSET;//devPanel_MinSET();  // output MinSET
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32MaxSET = pSelPanelType->m_dwPanelMaxSET;//devPanel_MaxSET();  // output MaxSET
    //#if (ENABLE_LVDSTORGB_CONVERTER == ENABLE)
   // panel_Info.u8HSyncWidth = 0x8D + 1;  // hsync width
   // #else
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8HSyncWidth = pSelPanelType->m_ucPanelHSyncWidth;//pnlGetHSyncWidth();  // hsync width
   // #endif

    // display timing
    if(ENABLE == pPNLResourcePrivate->stapiPNL._bForceSetHStartEnable)
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart = pPNLResourcePrivate->stapiPNL._u16PanelHStart;
        pPNLResourcePrivate->stapiPNL._bForceSetHStartEnable = DISABLE;
    }
    else
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart = pSelPanelType->m_wPanelHStart;//devPanel_HSTART(); // DE H start
    }
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width  = pSelPanelType->m_wPanelWidth;  // DE H width

    if( (pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & E_APIPNL_MISC_PIXELSHIFT_ENABLE) == 0 )
    {
        if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart = MDrv_PNL_GetPanelVStart();
        }
        else
        {
            //when do not enable the pixel shift feature
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart = PANEL_DE_VSTART;   // devPanel_VSTART(); // DE V start
        }
    }
    else
    {
        //when enable the pixel shift feature
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart = MDrv_PNL_GetPanelVStart();//pSelPanelType->m_wPanelVStart;//PANEL_DE_VSTART;   // devPanel_VSTART(); // DE V start
    }

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height = pSelPanelType->m_wPanelHeight;//devPanel_HEIGHT(); // DE V height

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VSyncStart = pSelPanelType->m_wPanelVStart; //devPanel_VSTART();  // vsync start
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8VSyncWidth  = pSelPanelType->m_ucPanelVSyncWidth; //PANEL_VSYNC_WIDTH;  // vsync width
    // output control
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OCTRL = (MS_U16)(pSelPanelType->m_bPanelDualPort << 8)
                            | pSelPanelType->m_bPanelSwapEven_RB;

    //devPanel_OCTRL();
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OSTRL = (0xF700) | (pSelPanelType->m_ucPanelDCLKDelay << 4)
                            | pSelPanelType->m_bPanelInvHSync;

    // output driving current
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16ODRV = (MS_U16)(pSelPanelType->m_ucPanelDCKLCurrent << 6)
                            | (pSelPanelType->m_ucPanelDECurrent << 4)
                            | (pSelPanelType->m_ucPanelODDDataCurrent << 2)
                            | pSelPanelType->m_ucPanelEvenDataCurrent; //devPanel_ODRV();

    //for MOD
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode = pSelPanelType->m_bPanelDoubleClk == 0 ? E_PNL_MODE_SINGLE : E_PNL_MODE_DUAL; // 0: single clock mode, 1: double clock mode
    // !! it should not use the different type to assign

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type = _TransToDriver_APIPNL_LINK_TYPE_To_PNL_TYPE(pSelPanelType->m_ePanelLinkType);
    if (E_PNL_TYPE_EXT == pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type)
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext;
    }

    //printf("@@ %s, %d, pSelPanelType->m_ePanelLinkType=%u\n", __FUNCTION__, __LINE__, pSelPanelType->m_ePanelLinkType);
    //printf("@@ %s, %d, _stPnlInitData.eLPLL_Type=%u\n", __FUNCTION__, __LINE__, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type);

    //we should set VSync manually to satisfy HDMITx's requirement.
    //auto-Vsync will have several-vertical-line error.
    switch(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type)
    {
        case E_PNL_TYPE_DAC_I:
        case E_PNL_TYPE_DAC_P:
        case E_PNL_TYPE_PDPLVDS:
        //case E_PNL_TYPE_EXT:
        case E_PNL_TYPE_TTL:
#if (XC_SUPPORT_AUTO_VSYNC == 0)
        case E_PNL_LPLL_VBY1_10BIT_16LANE:
        case E_PNL_LPLL_VBY1_10BIT_4LANE:
        case E_PNL_LPLL_VBY1_10BIT_2LANE:
        case E_PNL_LPLL_VBY1_10BIT_1LANE:
        case E_PNL_LPLL_VBY1_8BIT_16LANE:
        case E_PNL_LPLL_VBY1_8BIT_4LANE:
        case E_PNL_LPLL_VBY1_8BIT_2LANE:
        case E_PNL_LPLL_VBY1_8BIT_1LANE:
        case E_PNL_LPLL_VBY1_10BIT_8LANE:
        case E_PNL_LPLL_VBY1_8BIT_8LANE:
        case E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE:
        case E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE:
#endif
        ///TCON Panel
        case E_PNL_LPLL_EPI34_8P:
        case E_PNL_LPLL_EPI28_8P:
        case E_PNL_LPLL_EPI34_6P:
        case E_PNL_LPLL_EPI28_6P:
        case E_PNL_LPLL_EPI34_2P:
        case E_PNL_LPLL_EPI34_4P:
        case E_PNL_LPLL_EPI28_2P:
        case E_PNL_LPLL_EPI28_4P:
        case E_PNL_LPLL_EPI28_12P:
        case E_PNL_LPLL_EPI24_12P:
        case E_PNL_TYPE_TTL_TCON:
        case E_PNL_TYPE_MINILVDS:
        case E_PNL_LPLL_MINILVDS_5P_2L:
        case E_PNL_LPLL_MINILVDS_4P_2L:
        case E_PNL_LPLL_MINILVDS_3P_2L:
        case E_PNL_LPLL_MINILVDS_6P_1L:
        case E_PNL_LPLL_MINILVDS_5P_1L:
        case E_PNL_LPLL_MINILVDS_4P_1L:
        case E_PNL_LPLL_MINILVDS_3P_1L:
        case E_PNL_LPLL_MINILVDS_2CH_3P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_4P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_5P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_6P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_3P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_4P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_5P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_6P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_3P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_4P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_5P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_6P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_3P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_4P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_5P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_6P_6BIT:
        case E_PNL_LPLL_USI_T_8BIT_12P:
        case E_PNL_LPLL_USI_T_10BIT_12P:
        case E_PNL_LPLL_ISP_8BIT_12P:
        case E_PNL_LPLL_ISP_8BIT_6P_D:
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.bManuelVSyncCtrl = TRUE;
            break;
        default:
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.bManuelVSyncCtrl = FALSE;
            break;
    }

    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL)
       == E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL)
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.bManuelVSyncCtrl = TRUE;
    }

    //MOD_REG(0x40)
    //PANEL_DCLK_DELAY:8, PANEL_SWAP_LVDS_CH:6, PANEL_SWAP_LVDS_POL:5, PANEL_LVDS_TI_MODE:2,
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRL0 = ((/*PANEL_DCLK_DELAY*/pSelPanelType->m_ucPanelDCLKDelay & 0xf) << 8)
                                | ((/*PANEL_SWAP_LVDS_CH*/   pSelPanelType->m_bPanelSwapLVDS_CH) ? BIT(6):0)
                                | ((/*~PANEL_SWAP_LVDS_POL*/ !pSelPanelType->m_bPanelSwapLVDS_POL) ? BIT(5):0)
                                | ((/*PANEL_LVDS_PDP_10BIT*/ pSelPanelType->m_bPanelPDP10BIT) ? BIT(3):0)
                                | ((/*PANEL_LVDS_TI_MODE*/   pSelPanelType->m_bPanelLVDS_TI_MODE) ? BIT(2):0);
    //MOD_REG(0x49)
    //PANEL_SWAP_EVEN_ML:14, PANEL_SWAP_EVEN_RB:13, PANEL_SWAP_ODD_ML:12, PANEL_SWAP_ODD_RB:11
    // [7,6] : output formate selction 10: 8bit, 01: 6bit :other 10bit
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRL9 = ((/*  PANEL_SWAP_EVEN_ML */ pSelPanelType->m_bPanelSwapEven_ML)? BIT(14):0)
                                | ((/* PANEL_SWAP_EVEN_RB */ pSelPanelType->m_bPanelSwapEven_RB)? BIT(13):0)
                                | ((/* PANEL_SWAP_ODD_ML */  pSelPanelType->m_bPanelSwapOdd_ML)? BIT(12):0)
                                | ((/* PANEL_SWAP_ODD_RB */  pSelPanelType->m_bPanelSwapOdd_RB)? BIT(11):0)
                                | ((/* PANEL_OUTPUT_FORMAT */pSelPanelType->m_ucOutputFormatBitMode) << 6 )
                                | ((/* PANEL_SWAP_EVEN_RG */  pSelPanelType->m_bPanelSwapEven_RG)? BIT(5):0)
                                | ((/* PANEL_SWAP_EVEN_GB */  pSelPanelType->m_bPanelSwapEven_GB)? BIT(4):0)
                                | ((/* PANEL_SWAP_ODD_RG */  pSelPanelType->m_bPanelSwapOdd_RG)? BIT(3):0)
                                | ((/* PANEL_SWAP_ODD_GB */  pSelPanelType->m_bPanelSwapOdd_GB)? BIT(2):0);
    //MOD_REG(0x4A)
    //PANEL_INV_HSYNC:12, PANEL_INV_DCLK:4, PANEL_INV_VSYNC:3, PANEL_INV_DE:2, PANEL_DUAL_PORT:1, PANEL_SWAP_PORT:0,

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA = ((pSelPanelType->m_bPanelInvHSync)? BIT(12):0)
                                | ((pSelPanelType->m_bPanelInvDCLK)? BIT(4):0)
                                | ((pSelPanelType->m_bPanelInvVSync)? BIT(3):0)
                                | ((pSelPanelType->m_bPanelInvDE)? BIT(2):0)
                                | ((pSelPanelType->m_bPanelDualPort) ? BIT(1):0)
                                | ((pSelPanelType->m_bPanelSwapPort) ? BIT(0):0);

    //MOD_REG(0x4B)
    //[1:0]ti_bitmode=00(10bit)
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8MOD_CTRLB = pSelPanelType->m_ucTiBitMode;

#define MOVE_OUTSIDE TRUE
#if MOVE_OUTSIDE
    /*printf( "Move board related params to (custimized panel file)Panel.c  (LVDSTxSwapValue)%d\n",
             pSelPanelType->m_u16LVDSTxSwapValue
    );*/
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16LVDSTxSwapValue  = pSelPanelType->m_u16LVDSTxSwapValue;

#else
    #if defined(ENABLE_LVDS_PN_SWAP)
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16LVDSTxSwapValue  = (LVDS_PN_SWAP_H<<8) | LVDS_PN_SWAP_L;
    #else
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16LVDSTxSwapValue  = 0;
    #endif

#endif
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOutTimingMode = pSelPanelType->m_ucOutTimingMode;

    if(ENABLE==pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable)
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq =( (MS_U32)(pPNLResourcePrivate->stapiPNL._u16PanelDCLK/2*10000000)/(pSelPanelType->m_wPanelHTotal*pSelPanelType->m_wPanelVTotal))*2;
        pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = DISABLE;
    }
    else
    {
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq = ((MS_U32)pSelPanelType->m_dwPanelDCLK*10000000)/(pSelPanelType->m_wPanelHTotal*pSelPanelType->m_wPanelVTotal);
    }
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq = MDrv_PNL_GetDefaultVfreq(pInstance, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq);

    //In FRC 120Hz Panel case, the DCLK should be double
    if(MDrv_PNL_Is_SupportFRC(pInstance) && (pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC == (MS_U32)E_APIPNL_MISC_MFC_ENABLE))
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq *= 2;

    if ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC == (MS_U32)E_APIPNL_MISC_4K2K_ENABLE_60HZ))
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq *= 2;

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8PanelNoiseDith = pSelPanelType->m_bPanelNoiseDith;

    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOnTiming1 = pSelPanelType->m_wPanelOnTiming1;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOnTiming2 = pSelPanelType->m_wPanelOnTiming2;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOffTiming1 = pSelPanelType->m_wPanelOffTiming1;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOffTiming2 = pSelPanelType->m_wPanelOffTiming2;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelMinPWMVal = pSelPanelType->m_ucMinPWMVal;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelMaxPWMVal = pSelPanelType->m_ucMaxPWMVal;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelDimmingCtl = pSelPanelType->m_ucDimmingCtl;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelHSyncBackPorch = pSelPanelType->m_wPanelHStart - pSelPanelType->m_ucPanelHSyncWidth;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelVBackPorch = pSelPanelType->m_wPanelVStart - pSelPanelType->m_ucPanelVSyncWidth;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.pPanelName = pSelPanelType->m_pPanelName;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelAspectRatio= pSelPanelType->m_ucPanelAspectRatio;

    /// set default value to E_PNL_TYPE_HS_LVDS
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Ext_LpllType = E_PNL_TYPE_HS_LVDS;

    /// set default setting for HW training mode
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En = TRUE;//FALSE;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.bOSD_HW_Training_En   = TRUE;//FALSE;

    // for Panel lib compatibility
    _MakeCompatibile(pInstance);

    if(MDrv_PNL_Is_SupportFRC(pInstance))
    {
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC == (MS_U32)E_APIPNL_MISC_MFC_ENABLE)
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode = E_PNL_MODE_QUAD;
        }
    }

    if(pPNLResourcePrivate->stapiPNL._bSkipTimingChange == FALSE)
    {
        //ignore timing change if Mboot Logo
        MDrv_PNL_SetPanelType(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    }
#if 0
    if(_stPnlInitData.eLPLL_Type == E_PNL_TYPE_MFC)
    {
        MDrv_PNL_MFC(TRUE);
    }
#endif

    // Save panel type parameters
    memcpy(&pPNLResourcePrivate->stapiPNL._stPanelTypeSaved, pSelPanelType, sizeof(PanelType));
    pPNLResourcePrivate->stapiPNL._stPanelTypeSaved.m_wPanelHStart = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart;
    pPNLResourcePrivate->stapiPNL._stPanelTypeSaved.m_wPanelVStart = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart;
    pPNLResourcePrivate->stapiPNL._stPanelTypeSaved.m_ucOutTimingMode = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOutTimingMode;
    pPNLResourcePrivate->stapiPNL._stPanelTypeSaved.m_bPanelNoiseDith = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8PanelNoiseDith;

    return TRUE;
}

//
//  DumpData
//
static void _DumpData(void* pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    //PNL_PRINT_VAR(panel_info);
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HTotal         );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VTotal         );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8HSyncWidth      );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width          );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height         );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart         );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart         );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OCTRL          );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OSTRL          );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16ODRV           );

    //PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DITHCTRL       );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode        );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type        );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRL0      );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRL9      );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA      );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8MOD_CTRLB       );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16LVDSTxSwapValue);
    //PNL_PRINT_VAR(_stPnlInitData.u16PanelOutTimingMode);
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq     );
    //PNL_PRINT_VAR(panel_info->u16DefaultHTotal     );
    //PNL_PRINT_VAR(panel_info->u16DefaultVTotal     );
    //PNL_PRINT_VAR(panel_info->u16MinHTotal         );
    //PNL_PRINT_VAR(panel_info->u16MaxHTotal         );
    //PNL_PRINT_VAR(panel_info->u16MinVTotal         );
    //PNL_PRINT_VAR(panel_info->u16MaxVTotal         );
    //PNL_PRINT_VAR(panel_info->u32MinDCLK           );
    //PNL_PRINT_VAR(panel_info->u32MaxDCLK           );

    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VSyncStart      );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8VSyncWidth       );
    PNL_PRINT_VAR(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8PanelNoiseDith   );

}

//
//  Dump m_panel data
//
static void  _EmptyMethod(MS_BOOL bEnable)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }
    UNUSED(bEnable);
    PNL_DBG_MSG(" Error!! <==============  Calling a non-registered function!!");
}

MS_U16 MAPI_PNL_GetPNLHstart_U2(void *pInstance)
{
    MS_U16 u16HStart = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16HStart = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HStart;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16HStart;
}
//
//
static MS_U16 _getPanelHstart(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetHstart PNLArgs;
    PNLArgs.u16Return= 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETHSTART, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLHstart_U2(pu32PNLInst);
#endif
}

MS_U16 MAPI_PNL_GetPNLVstart_U2(void *pInstance)
{
    MS_U16 u16VStart = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16VStart = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VStart;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16VStart;
}

static MS_U16 _getPanelVstart(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetVstart PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETVSTART, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLVstart_U2(pu32PNLInst);
#endif
}

MS_U16 MAPI_PNL_GetPNLWidth_U2(void *pInstance)
{
    MS_U16 u16Width = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16Width = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16Width;
}

static MS_U16 _getPanelWidth (void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetWidth PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETWIDTH, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLWidth_U2(pu32PNLInst);
#endif
}

MS_U16 MAPI_PNL_GetPNLHeight_U2(void *pInstance)
{
    MS_U16 u16Height = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16Height = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16Height;
}

static MS_U16 _getPanelHeight(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetHeight PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETHEIGHT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLHeight_U2(pu32PNLInst);
#endif
}

MS_U16 MAPI_PNL_GetPNLHtotal_U2(void *pInstance)
{
    MS_U16 u16HTotal = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16HTotal = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HTotal;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16HTotal;
}

static MS_U16 _getPanelhtotal(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetHtotal PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETHTOTAL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLHtotal_U2(pu32PNLInst);
#endif
}

MS_U16 MAPI_PNL_GetPNLVtotal_U2(void *pInstance)
{
    MS_U16 u16VTotal = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16VTotal = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VTotal;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16VTotal;
}

static MS_U16 _getPanelVtotal(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetVtotal PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETVTOTAL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MAPI_PNL_GetPNLVtotal_U2(pu32PNLInst);
#endif
}

MS_U32 MApi_PNL_GetMinSET_U2(void *pInstance)
{
    MS_U32 u32MinSET = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u32MinSET = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32MinSET;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u32MinSET;
}

static MS_U32 _getPanelMinSET(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetMinSET PNLArgs;
    PNLArgs.u32Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETMINSET, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u32Return;
    }
#else
    return MApi_PNL_GetMinSET_U2(pu32PNLInst);
#endif
}

MS_U32 MApi_PNL_GetMaxSET_U2(void *pInstance)
{
    MS_U32 u32MaxSET = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u32MaxSET = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32MaxSET;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u32MaxSET;
}

static MS_U32 _getPanelMaxSET(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetMaxSET PNLArgs;
    PNLArgs.u32Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETMAXSET, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.u32Return;
    }
#else
    return MApi_PNL_GetMaxSET_U2(pu32PNLInst);
#endif
}

static MS_U16 _getPanelDefVFreq(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetDefVFreq PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETDEFVFREQ, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MApi_PNL_GetDefVFreq_U2(pu32PNLInst);
#endif
}

static MS_U8 _getPanel_LPLL_Mode(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetLPLLMode PNLArgs;
    PNLArgs.u8Return = E_PNL_MODE_SINGLE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETLPLLMODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_MODE_SINGLE;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MApi_PNL_GetLPLLMode_U2(pu32PNLInst);
#endif
}

MS_U8 MApi_PNL_GetLPLLType_U2(void *pInstance)
{
    MS_U8 u8LPLL_Type = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u8LPLL_Type = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8LPLL_Type;
}

static MS_U8 _getPanel_LPLL_Type(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetLPLLType PNLArgs;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETLPLLTYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_MODE_SINGLE;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MApi_PNL_GetLPLLType_U2(pu32PNLInst);
#endif
}

MS_U8 MAPI_PNL_GetPNLHsyncWidth_U2(void *pInstance)
{
    MS_U8 u8HSyncWidth = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u8HSyncWidth = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8HSyncWidth;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8HSyncWidth;
}

static MS_U8 _getPanelHSyncWidth(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetHsyncWidth PNLArgs;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETHSYNCWIDTH, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MAPI_PNL_GetPNLHsyncWidth_U2(pu32PNLInst);
#endif
}

MS_U8 MAPI_PNL_GetPNLHsyncBackPorch_U2(void *pInstance)
{
    MS_U8 u8PanelHSyncBackPorch = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u8PanelHSyncBackPorch = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelHSyncBackPorch;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8PanelHSyncBackPorch;
}

static MS_U8 _getPanelHSyncBackPorch(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetHsyncBackPorch PNLArgs;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETHSYNCBACKPORCH, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MAPI_PNL_GetPNLHsyncBackPorch_U2(pu32PNLInst);
#endif
}

MS_U8 MAPI_PNL_GetPNLVsyncBackPorch_U2(void *pInstance)
{
    MS_U8 u8PanelVBackPorch = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u8PanelVBackPorch = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelVBackPorch;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8PanelVBackPorch;
}

static MS_U8 _getPanelVSyncBackPorch(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetVsyncBackPorch PNLArgs;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETVSYNCBACKPORCH, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MAPI_PNL_GetPNLVsyncBackPorch_U2(pu32PNLInst);
#endif
}

const char* MApi_PNL_GetName_U2(void *pInstance)
{
    const char* pPanelName;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPanelName = pPNLResourcePrivate->stdrvPNL._stPnlInitData.pPanelName;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return pPanelName;
}

static const char* _getPanelName(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }
    static char panelName[20];
    stPNL_GetName PNLArgs;
    PNLArgs.pPNLName = panelName;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETPANELNAME, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return PNLArgs.pPNLName;
    }
#else
    return MApi_PNL_GetName_U2(pu32PNLInst);
#endif
}

E_PNL_ASPECT_RATIO MApi_PNL_GetARC_U2(void *pInstance)
{
    E_PNL_ASPECT_RATIO ePanelAspectRatio = E_PNL_ASPECT_RATIO_OTHER;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    ePanelAspectRatio = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelAspectRatio;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return ePanelAspectRatio;
}

static E_PNL_ASPECT_RATIO  _getPanel_ARC(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetARC PNLArgs;
    PNLArgs.ePanelAspectRatio = E_PNL_ASPECT_RATIO_OTHER;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETARC, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_MODE_SINGLE;
    }
    else
    {
        return PNLArgs.ePanelAspectRatio;
    }
#else
    return MApi_PNL_GetARC_U2(pu32PNLInst);
#endif
}

APIPNL_OUT_TIMING_MODE MApi_PNL_GetOutTimingMode_U2(void *pInstance)
{
    APIPNL_OUT_TIMING_MODE ePanelOutTimingMode;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    ePanelOutTimingMode = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOutTimingMode;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return ePanelOutTimingMode;
}

static APIPNL_OUT_TIMING_MODE _getOutTimingMode(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetOutTimingMode PNLArgs;
    PNLArgs.ePanelOutTimingMode = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETOUTTIMINGMODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.ePanelOutTimingMode;
    }
#else
    return MApi_PNL_GetOutTimingMode_U2(pu32PNLInst);
#endif
}


XC_PNL_OBJ g_IPanel =
{
    //
    //  Data
    //
    .Name            = _getPanelName,
    .HStart          = _getPanelHstart,
    .VStart          = _getPanelVstart,
    .Width           = _getPanelWidth,
    .Height          = _getPanelHeight,
    .HTotal          = _getPanelhtotal,
    .VTotal          = _getPanelVtotal,
    .HSynWidth       = _getPanelHSyncWidth,
    .HSynBackPorch   = _getPanelHSyncBackPorch,
    .VSynBackPorch   = _getPanelVSyncBackPorch,
    .DefaultVFreq    = _getPanelDefVFreq,
    .LPLL_Mode       = _getPanel_LPLL_Mode,            /* 0: single, 1: dual mode */
    .LPLL_Type       = _getPanel_LPLL_Type,
    .AspectRatio     = _getPanel_ARC,
    .MinSET          = _getPanelMinSET,
    .MaxSET          = _getPanelMaxSET,
    .OutTimingMode   = _getOutTimingMode,
    // Manipulation
    //
    .SetSSC          = MApi_PNL_SetSSC,
    .Enable          = MApi_PNL_EnablePanel,
    .SetGammaTbl     = MApi_PNL_SetGammaTbl,
    .GammaTab        = MApi_PNL_GetAllGammaTbl,
    .Dump            = MApi_PNL_DumpPanelData,
    .DimCtrl         = MApi_PNL_GetPanelDimCtrl,
    .OnTiming        = MApi_PNL_GetPanelOnTiming,
    .OffTiming       = MApi_PNL_GetPanelOffTiming,
    //
    // Custimized methods, can be provided by clinets.
    //
    .TurnBackLightOn = _EmptyMethod,
    .SetGammaValue   = MApi_PNL_SetGammaValue,
    .GetGammaTbl     = MApi_PNL_GetGammaTbl,
};


/******************************************************************************/
//        Public function
//
/******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the version of apiPNL lib, (without Mutex protect)
/// @param[out] ppVersion \b output apiPNL lib version
/// @return             @ref  APIPNL_Result
////////////////////////////////////////////////////////////////////////////////
APIPNL_Result MApi_PNL_GetLibVer_U2(void *pInstance, const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_APIPNL_FAIL;
    }

    *ppVersion = &_api_pnl_version;
    return E_APIPNL_OK;
}

APIPNL_Result MApi_PNL_GetLibVer(const MSIF_Version **ppVersion)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_GetLibVer PNLArgs;
    PNLArgs.ppVersion = ppVersion;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETLIBVER, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eResult;
    }
#else
    return MApi_PNL_GetLibVer_U2(pu32PNLInst, ppVersion);
#endif
}

const PNL_ApiInfo* MApi_PNL_GetInfo_U2(void *pInstance)          ///< Get info from driver
{
    ST_PNL_DRV_INFO stPNLDRVInfo = MDrv_PNL_GetInfo(pInstance);
    PNL_ApiInfo* pstReturnInfo = NULL;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if ((stPNLDRVInfo.u8SupportGammaType & E_DRVPNL_GAMMA_10BIT) && (stPNLDRVInfo.u8SupportGammaType & E_DRVPNL_GAMMA_12BIT))
    {
        pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_ALL;
    }
    else if ( stPNLDRVInfo.u8SupportGammaType & E_DRVPNL_GAMMA_12BIT )
    {
        pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_12BIT;
    }
    else if ( stPNLDRVInfo.u8SupportGammaType & E_DRVPNL_GAMMA_10BIT )
    {
        pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_10BIT;
    }
    else
    {
        PNL_ASSERT(0, "[%s] fails Invalid eSupportGammaType( %d ) found, please check your library.\n",
                    __FUNCTION__, stPNLDRVInfo.u8SupportGammaType );
    }

    pstReturnInfo = &pPNLResourcePrivate->stapiPNL._cstPnlInfo;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return pstReturnInfo;
}

const PNL_ApiInfo* MApi_PNL_GetInfo(void)          ///< Get info from driver
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return NULL;
        }
    }

    stPNL_GetInfo PNLArgs;
    PNLArgs.pApiInfo = NULL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETINFO, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return PNLArgs.pApiInfo;
    }
#else
    return MApi_PNL_GetInfo_U2(pu32PNLInst);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Extended API of getting current panel status
/// @param[in] pPnlExtStatus @ref PNL_ApiExtStatus
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_GetStatusEx_U2(void *pInstance, PNL_ApiExtStatus *pPnlExtStatus)
{
    PNL_TimingInfo stPNLTiming;
    PNL_ApiExtStatus stPnlExtStatus;
    MS_U16 u16CopyLength = sizeof(PNL_ApiExtStatus);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if((NULL == pPnlExtStatus) || (pPnlExtStatus->u16ApiStatusEX_Length == 0))
    {
        //Invalid input parameter
        return FALSE;
    }
    memset(&stPNLTiming,0,sizeof(PNL_TimingInfo));
    memset(&stPnlExtStatus,0,sizeof(PNL_ApiExtStatus));

    if(pPnlExtStatus->u16ApiStatusEX_Length < u16CopyLength) //Lib structure size > AP's using structure size ?
    {
        //Only copy the AP recongnized member
        u16CopyLength = pPnlExtStatus->u16ApiStatusEX_Length;
    }

    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    stPnlExtStatus.bPNLInitialize= pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized;
    stPnlExtStatus.bPNLEnable    = pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled;

    if(MDrv_PNL_GetDataFromRegister(pInstance, &stPNLTiming))
    {
        // Vertical
        stPnlExtStatus.u16VTotal    =            stPNLTiming.u16VTotal+1; //Total +1
        stPnlExtStatus.u16DEVStart  =            stPNLTiming.u16DEVStart;
        stPnlExtStatus.u16DEVEnd    =            stPNLTiming.u16DEVEnd;
        stPnlExtStatus.u16VSyncStart=            stPNLTiming.u16VSyncStart;
        stPnlExtStatus.u16VSyncEnd  =            stPNLTiming.u16VSyncEnd+1;//For SyncWidth need + 1

        // Horizontal
        stPnlExtStatus.u16HTotal    =            stPNLTiming.u16HTotal+1; //Total +1
        stPnlExtStatus.u16DEHStart  =            stPNLTiming.u16DEHStart  ;
        stPnlExtStatus.u16DEHEnd    =            stPNLTiming.u16DEHEnd    ;
        stPnlExtStatus.u16HSyncWidth=            stPNLTiming.u16HSyncWidth+1;//For SyncWidth need + 1
        stPnlExtStatus.bIsPanelManualVysncMode = stPNLTiming.bIsPanelManualVysncMode ;
        stPnlExtStatus.bInterlaceOutput =        stPNLTiming.bInterlaceOutput;
        stPnlExtStatus.bYUVOutput   =            MDrv_PNL_isYUVOutput(pInstance);
    }
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    memcpy(pPnlExtStatus, &stPnlExtStatus, u16CopyLength);
    return TRUE;
}

MS_BOOL MApi_PNL_GetStatusEx(PNL_ApiExtStatus *pPnlExtStatus)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetStatusEx PNLArgs;
    PNLArgs.pPnlExtStatus = pPnlExtStatus;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETSTATUSEX, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_GetStatusEx_U2(pu32PNLInst, pPnlExtStatus);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get current panel status
/// @param[in] pPnlStatus @ref PNL_ApiStatus
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_GetStatus_U2(void *pInstance, PNL_ApiStatus *pPnlStatus)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    pPnlStatus->bPanel_Initialized = pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized;
    pPnlStatus->bPanel_Enabled = pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled;

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return TRUE;
}

MS_BOOL MApi_PNL_GetStatus(PNL_ApiStatus *pPnlStatus)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetStatus PNLArgs;
    PNLArgs.pPnlStatus = pPnlStatus;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETSTATUS, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_GetStatus_U2(pu32PNLInst, pPnlStatus);
#endif
}

/// Set debug level (without Mutex protect)
MS_BOOL MApi_PNL_SetDbgLevel_U2(void *pInstance, MS_U16 u16DbgSwitch)
{
    _u16PnlDbgSwitch = u16DbgSwitch;
    return TRUE;
}

MS_BOOL MApi_PNL_SetDbgLevel(MS_U16 u16DbgSwitch)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetDbgLevel PNLArgs;
    PNLArgs.u16DbgSwitch = u16DbgSwitch;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETDBGLEVEL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_SetDbgLevel_U2(pu32PNLInst, u16DbgSwitch);
#endif
}

MS_BOOL MApi_PNL_IOMapBaseInit_U2(void *pInstance)
{
    MS_BOOL bResult = FALSE;
    if (-1==_s32PNLMutex)
    {
        _s32PNLMutex = MsOS_CreateMutex(E_MSOS_FIFO, "_PNL_Mutex", MSOS_PROCESS_SHARED);
        if (-1 == _s32PNLMutex)
        {
            MS_CRITICAL_MSG(printf("[MAPI PNL][%06d] create mutex fail\n", __LINE__));
            return FALSE;
        }
    }

    bResult =(MDrv_PNL_Init(pInstance) == E_PNL_OK);

    return bResult;
}

#ifdef MSOS_TYPE_LINUX
//those code is temporary just for MWB warning software patch.
//if utopia_init will has be refined in the future, those code will be deleted.
extern void XCRegisterToUtopia(FUtopiaOpen ModuleType);
extern void GOPRegisterToUtopia(void);
extern void GFXRegisterToUtopia(FUtopiaOpen ModuleType);
#endif

MS_BOOL MApi_PNL_IOMapBaseInit(void)
{

#ifdef MSOS_TYPE_LINUX
    //those code is temporary just for MWB warning software patch.
    //if utopia_init will has be refined in the future, those code will be deleted.
    PNLRegisterToUtopia(NULL);
    XCRegisterToUtopia((FUtopiaOpen)XCOpenFull);
    GOPRegisterToUtopia();
    GFXRegisterToUtopia((FUtopiaOpen) GFXOpen);
#endif

#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_IOMapBaseInit PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_IOMAPBASEINIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_IOMapBaseInit_U2(pu32PNLInst);
#endif
}

//////////////////////////////////////////////////////////////////////
/**
*
* @brief  Give some options for panel init.
*
* @param[in] pSelPanelType  a given panel type choosed by user.
*
*/
MS_BOOL  MApi_PNL_PreInit_U2(void *pInstance, E_PNL_PREINIT_OPTIONS eInitParam)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    if ( eInitParam == E_PNL_NO_OUTPUT)
    {
        MDrv_PNL_PreInit(pInstance, E_PNL_OUTPUT_NO_OUTPUT);
    }
    else if ( eInitParam == E_PNL_CLK_ONLY)
    {
        MDrv_PNL_PreInit(pInstance, E_PNL_OUTPUT_CLK_ONLY);
    }
    else if ( eInitParam == E_PNL_CLK_DATA)
    {
        MDrv_PNL_PreInit(pInstance, E_PNL_OUTPUT_CLK_DATA);
    }
    else
    {
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return FALSE;
    }

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return TRUE;
}

MS_BOOL  MApi_PNL_PreInit(E_PNL_PREINIT_OPTIONS eInitParam)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

#ifdef UTOPIA_20
    stPNL_PreInit PNLArgs;
    PNLArgs.eInitParam = eInitParam;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_PREINIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_PreInit_U2(pu32PNLInst, eInitParam);
#endif
}

//////////////////////////////////////////////////////////////////////
/**
*
* @brief  Initialize the global panel object with a given PanelType
*
* @param[in] pSelPanelType  a given panel type choosed by user.
*
*/
MS_BOOL MApi_PNL_Init_U2(void *pInstance, PanelType *pSelPanelType/* <in > */)
{
    if (-1==_s32PNLMutex)
    {
        _s32PNLMutex = MsOS_CreateMutex(E_MSOS_FIFO, "_PNL_Mutex", MSOS_PROCESS_SHARED);
        if (-1 == _s32PNLMutex)
        {
            MS_CRITICAL_MSG(printf("[MAPI PNL][%06d] create mutex fail\n", __LINE__));
            return FALSE;
        }
    }
#if (KERNEL_DRIVER_PNL_STR_PATCH == 1)
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    PNL_REGS_SAVE_AREA *pPNLResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pPNLResourceStr);
    memcpy(&pPNLResourceStr->pSelPanelType,pSelPanelType,sizeof(PanelType));
#endif

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    //MApi_XC_PNL_Init(&g_IPanel, outPanelInfo);//_MApi_XC_Panel_Init(&(sXC_InitData.sPanelInfo));
    if(MDrv_PNL_Init(pInstance) != E_PNL_OK) //Init MMIO and gamma clk
    {
        OS_DELETE_MUTEX(_s32PNLMutex);

        MS_CRITICAL_MSG(printf("Panel init error, check the message from driver\n"));

        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return FALSE;
    }

    if(pSelPanelType)
    {
        _PNL_ENTRY();
        _SetPanelParams(pInstance, pSelPanelType);
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized = TRUE;
        _PNL_RETURN();
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return TRUE;
    }
    else
    {
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return FALSE;
    }
}

MS_BOOL MApi_PNL_Init(PanelType *pSelPanelType/* <in > */)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }
#ifdef UTOPIA_20
    stPNL_Init PNLArgs;
    PNLArgs.pSelPanelType = pSelPanelType;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_INIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_Init_U2(pu32PNLInst, pSelPanelType);
#endif

}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_Init_Ex_U2(void *pInstance, PanelType *pSelPanelType/* <in > */, MSIF_Version LIBVER)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    // save version info
    {
        memcpy(( MS_U8* )&pPNLResourcePrivate->_api_pnl_version_sel, ( MS_U8* )(&LIBVER), sizeof( MSIF_Version) );
    }

    return MApi_PNL_Init_U2(pInstance, pSelPanelType);
}

MS_BOOL MApi_PNL_Init_Ex(PanelType *pSelPanelType/* <in > */, MSIF_Version LIBVER)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

#ifdef UTOPIA_20
    stPNL_Init_Ex PNLArgs;
    PNLArgs.pSelPanelType = pSelPanelType;
    PNLArgs.LIBVER = LIBVER;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_INIT_EX, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_Init_Ex_U2(pu32PNLInst, pSelPanelType, LIBVER);
#endif
}

APIPNL_Result MApi_PNL_GetConfig_U2(void *pInstance, PanelType *pSelPanelType)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized)
    {
        memcpy(pSelPanelType, &(pPNLResourcePrivate->stapiPNL._stPanelTypeSaved), sizeof(PanelType));
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return E_APIPNL_OK;
    }
    else
    {
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return E_APIPNL_FAIL;

    }
}

APIPNL_Result MApi_PNL_GetConfig(PanelType *pSelPanelType)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_GetConfig PNLArgs;
    PNLArgs.pSelPanelType = pSelPanelType;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_CONFIG, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_GetConfig_U2(pu32PNLInst, pSelPanelType);
#endif
}

MS_BOOL MApi_PNL_Control_Out_Swing_U2(void *pInstance, MS_U16 u16Swing_Level)
{
#if 0
    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
    ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE))
    {
        u16Swing_Level = 600;
    }
#endif
    MS_BOOL bReturn = FALSE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    bReturn = MDrv_PNL_Control_Out_Swing(pInstance, u16Swing_Level);
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return bReturn;
}

MS_BOOL MApi_PNL_Control_Out_Swing(MS_U16 u16Swing_Level)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Control_Out_Swing PNLArgs;
    PNLArgs.u16Swing_Level = u16Swing_Level;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_CONTROL_OUT_SWING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_Control_Out_Swing_U2(pu32PNLInst, u16Swing_Level);
#endif
}

MS_BOOL MApi_PNL_ForceSetPanelDCLK_U2(void *pInstance, MS_U16 u16PanelDCLK ,MS_BOOL bSetDCLKEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stapiPNL._u16PanelDCLK = u16PanelDCLK;
    pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = bSetDCLKEnable;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return TRUE;
}

MS_BOOL MApi_PNL_ForceSetPanelDCLK(MS_U16 u16PanelDCLK ,MS_BOOL bSetDCLKEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ForceSetPanelDCLK PNLArgs;
    PNLArgs.u16PanelDCLK = u16PanelDCLK;
    PNLArgs.bSetDCLKEnable = bSetDCLKEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_FORCESETPANELDCLK, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_ForceSetPanelDCLK_U2(pu32PNLInst, u16PanelDCLK, bSetDCLKEnable);
#endif
}

MS_BOOL MApi_PNL_ForceSetPanelHStart_U2(void *pInstance, MS_U16 u16PanelHStart ,MS_BOOL bSetHStartEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stapiPNL._u16PanelHStart = u16PanelHStart;
    pPNLResourcePrivate->stapiPNL._bForceSetHStartEnable = bSetHStartEnable;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return TRUE;
}

MS_BOOL MApi_PNL_ForceSetPanelHStart(MS_U16 u16PanelHStart ,MS_BOOL bSetHStartEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ForceSetPanelHStart PNLArgs;
    PNLArgs.u16PanelHStart = u16PanelHStart;
    PNLArgs.bSetHStartEnable = bSetHStartEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_FORCESETPANELHSTART, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_ForceSetPanelHStart_U2(pu32PNLInst, u16PanelHStart, bSetHStartEnable);
#endif
}

void MApi_PNL_SetOutput_U2(void *pInstance, APIPNL_OUTPUT_MODE eOutputMode)
{
    PNL_OUTPUT_MODE ePnlOutputMode;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    switch(eOutputMode)
    {
        case E_APIPNL_OUTPUT_NO_OUTPUT:
            ePnlOutputMode = E_PNL_OUTPUT_NO_OUTPUT;
            break;

        case E_APIPNL_OUTPUT_CLK_ONLY:
            ePnlOutputMode = E_PNL_OUTPUT_CLK_ONLY;
            break;

        case E_APIPNL_OUTPUT_DATA_ONLY:
            ePnlOutputMode = E_PNL_OUTPUT_DATA_ONLY;
            break;

        case E_APIPNL_OUTPUT_CLK_DATA:
        default:
            ePnlOutputMode = E_PNL_OUTPUT_CLK_DATA;
            break;
    }
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    _PNL_ENTRY();
    if(pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized == TRUE)
    {
        MDrv_PNL_SetOutputType(pInstance, ePnlOutputMode, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type);
        MDrv_PNL_MISC_Control(pInstance, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC);
    }
    else
    {
        MS_CRITICAL_MSG(printf("Must call MApi_PNL_Init() first\n"));
    }
    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_SetOutput(APIPNL_OUTPUT_MODE eOutputMode)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetOutput PNLArgs;
    PNLArgs.eOutputMode = eOutputMode;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETOUTPUT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetOutput_U2(pu32PNLInst, eOutputMode);
#endif
}

MS_BOOL MApi_PNL_ChangePanelType_U2(void *pInstance, PanelType *pSelPanelType)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if (pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized == FALSE)
    {
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return FALSE;
    }

    if(pSelPanelType == NULL)
    {
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return FALSE;
    }

    _PNL_ENTRY();
    _SetPanelParams(pInstance, pSelPanelType);
    _PNL_RETURN();

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return TRUE;
}

MS_BOOL MApi_PNL_ChangePanelType(PanelType *pSelPanelType)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ChangePanelType PNLArgs;
    PNLArgs.pSelPanelType = pSelPanelType;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_CHANGEPANELTYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_ChangePanelType_U2(pu32PNLInst, pSelPanelType);
#endif
}

MS_BOOL MApi_PNL_TCONMAP_DumpTable_U2(void *pInstance, MS_U8 *pTCONTable, MS_U8 u8Tcontype)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    switch(u8Tcontype)
    {
        case E_APIPNL_TCON_TAB_TYPE_GENERAL:
        case E_APIPNL_TCON_TAB_TYPE_GPIO:
            MDrv_PNL_TCON_DumpGENRegTab(pInstance, pTCONTable);
            break;
        case E_APIPNL_TCON_TAB_TYPE_SCALER:
            MDrv_PNL_TCON_DumpSCRegTab(pInstance, pTCONTable);
            break;
        case E_APIPNL_TCON_TAB_TYPE_MOD:
            _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
            MDrv_PNL_TCON_DumpMODRegTab(pInstance, pTCONTable);
            _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
            break;

        case E_APIPNL_TCON_TAB_TYPE_POWER_SEQUENCE_ON:
        case E_APIPNL_TCON_TAB_TYPE_POWER_SEQUENCE_OFF:
            //_MDrv_PNL_TCON_DumpPSRegTab(pTCONTable);
            break;

        case E_APIPNL_TCON_TAB_TYPE_GAMMA:
            break;

        default:
            printf("GetTable: unknown tcon type=%u> \n", u8Tcontype);
            break;
    }

    return TRUE;
}

MS_BOOL MApi_PNL_TCONMAP_DumpTable(MS_U8 *pTCONTable, MS_U8 u8Tcontype)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_TCONMAP_DumpTable PNLArgs;
    PNLArgs.pTCONTable = pTCONTable;
    PNLArgs.u8Tcontype = u8Tcontype;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_TCONMAP_DUMPTABLE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_TCONMAP_DumpTable_U2(pu32PNLInst, pTCONTable, u8Tcontype);
#endif
}

MS_BOOL MApi_PNL_TCONMAP_Power_Sequence_U2(void *pInstance, MS_U8 *pTCONTable, MS_BOOL bEnable)
{
// Move to APP
#if 0
    MDrv_PNL_TCONMAP_Power_Sequence(pTCONTable, bEnable);
#endif
    printf("\n\n!![Alert]Don't ued this function. It is removed!!\n\n");
    return FALSE;
}

MS_BOOL MApi_PNL_TCONMAP_Power_Sequence(MS_U8 *pTCONTable, MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_TCONMAP_Power_Sequence PNLArgs;
    PNLArgs.pTCONTable = pTCONTable;
    PNLArgs.bEnable = bEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_TCONMAP_POWER_SEQUENCE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_TCONMAP_Power_Sequence_U2(pu32PNLInst, pTCONTable, bEnable);
#endif
}

void MApi_PNL_SetOutputPattern_U2(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{
    MDrv_PNL_SetOutputPattern(pInstance, bEnable,u16Red,u16Green,u16Blue);
}

void MApi_PNL_SetOutputPattern(MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetOutputPattern PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.u16Red = u16Red;
    PNLArgs.u16Green = u16Green;
    PNLArgs.u16Blue = u16Blue;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SET_OUTPUT_PATTERN, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetOutputPattern_U2(pu32PNLInst, bEnable, u16Red, u16Green, u16Blue);
#endif
}
//-------------------------------------------------------------------------------------------------
/// Setup TCON Count Reset ON/OFF
/// @param  bEnable      \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MApi_PNL_TCON_Count_Reset_U2 (void *pInstance, MS_BOOL bEnable )
{
    MDrv_PNL_TCON_Count_Reset(pInstance, bEnable);
}

void MApi_PNL_TCON_Count_Reset (MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_TCON_Count_Reset PNLArgs;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_TCON_COUNT_RESET, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_TCON_Count_Reset_U2(pu32PNLInst, bEnable);
#endif
}

void MApi_PNL_TCON_Init_U2(void *pInstance)
{
    MDrv_PNL_TCON_Init(pInstance);
}

void MApi_PNL_TCON_Init( void )
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_TCON_INIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_TCON_Init_U2(pu32PNLInst);
#endif
}

MS_BOOL MApi_PNL_GetDstInfo_U2(void *pInstance, MS_PNL_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    PNL_TimingInfo PNL_Data;
    MS_BOOL bReturn = FALSE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _PNL_ENTRY();

    if(NULL == pDstInfo)
    {
        PNL_DBG_MSG("MApi_PNL_GetDstInfo():pDstInfo is NULL\n");
        bReturn = FALSE;
    }
    else if(u32SizeofDstInfo != sizeof(MS_PNL_DST_DispInfo))
    {
        PNL_DBG_MSG("MApi_PNL_GetDstInfo():u16SizeofDstInfo is different from the MS_PNL_DST_DispInfo defined, check header file!\n");
        bReturn = FALSE;
    }
    else if(MDrv_PNL_GetDataFromRegister(pInstance, &PNL_Data))
    {
        pDstInfo->VDTOT = PNL_Data.u16VTotal;
        pDstInfo->HDTOT = PNL_Data.u16HTotal;
        pDstInfo->DEHST = PNL_Data.u16DEHStart;
        pDstInfo->DEHEND= PNL_Data.u16DEHEnd;
        pDstInfo->DEVST= PNL_Data.u16DEVStart;
        pDstInfo->DEVEND= PNL_Data.u16DEVEnd;
        pDstInfo->bYUVOutput = MDrv_PNL_isYUVOutput(pInstance);
        pDstInfo->bInterlaceMode = PNL_Data.bInterlaceOutput;
        bReturn = TRUE;
    }
    else
    {
        pDstInfo->VDTOT = 0;
        pDstInfo->HDTOT = 0;
        pDstInfo->DEHST = 0;
        pDstInfo->DEHEND= 0;
        pDstInfo->DEVST= 0;
        pDstInfo->DEVEND= 0;
        pDstInfo->bInterlaceMode = FALSE;
        pDstInfo->bYUVOutput = FALSE;
        bReturn = FALSE;
    }

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bReturn;
}

MS_BOOL MApi_PNL_GetDstInfo(MS_PNL_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetDstInfo PNLArgs;
    PNLArgs.pDstInfo = pDstInfo;
    PNLArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETDSTINFO, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_GetDstInfo_U2(pu32PNLInst, pDstInfo, u32SizeofDstInfo);
#endif
}

MS_BOOL MApi_Mod_Calibration_Setting_U2(void *pInstance, MS_PNL_ModCaliInfo *pstModCaliInfo)
{
    PNL_ModCali_InitData stModCaliInitData;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    memset(&stModCaliInitData, 0 , sizeof(PNL_ModCali_InitData));

    if(pstModCaliInfo == NULL)
    {
        return FALSE;
    }

    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _PNL_ENTRY();
    stModCaliInitData.u16ExpectSwingLevel = pstModCaliInfo->m_u16ExpectSwingLevel;
    stModCaliInitData.u8ModCaliPairSel    = pstModCaliInfo->m_u8ModCaliPairSel;
    stModCaliInitData.u8ModCaliTarget     = pstModCaliInfo->m_u8ModCaliTarget;
    stModCaliInitData.s8ModCaliOffset     = pstModCaliInfo->m_s8ModCaliOffset;
    stModCaliInitData.bPVDD_2V5           = pstModCaliInfo->m_bPVDD_2V5;
    MDrv_Mod_Calibration_Init(pInstance, &stModCaliInitData);
    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return TRUE;
}

MS_BOOL MApi_Mod_Calibration_Setting(MS_PNL_ModCaliInfo *pstModCaliInfo)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    if(pstModCaliInfo == NULL)
    {
        printf("MOD Calibration config setting is NULL\n");
        return FALSE;
    }

    stPNL_Mod_Calibration_Setting PNLArgs;
    PNLArgs.pstModCaliInfo = pstModCaliInfo;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_MOD_CALIBRATION_SETTING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_Mod_Calibration_Setting_U2(pu32PNLInst, pstModCaliInfo);
#endif
}

MS_BOOL MApi_Mod_Do_Calibration_U2(void *pInstance)
{
    MS_BOOL bResult = FALSE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _PNL_ENTRY();
    if ( E_PNL_OK == MDrv_PNL_MOD_Calibration(pInstance) )
        bResult = TRUE;
    else
        bResult = FALSE;
    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bResult;
}

MS_BOOL MApi_Mod_Do_Calibration(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Mod_Do_Calibration PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_MOD_DO_CALIBRATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_Mod_Do_Calibration_U2(pu32PNLInst);
#endif
}

void MApi_BD_LVDS_Output_Type_U2(void *pInstance, MS_U16 Type)
{
#if (KERNEL_DRIVER_PNL_STR_PATCH == 1)
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    PNL_REGS_SAVE_AREA *pPNLResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pPNLResourceStr);
    pPNLResourceStr->BD_LVDS_Output_Type = Type;
#endif
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    MDrv_BD_LVDS_Output_Type(pInstance, Type);
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_BD_LVDS_Output_Type(MS_U16 Type)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_BD_LVDS_Output_Type PNLArgs;
    PNLArgs.Type = Type;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_BD_LVDS_OUTPUT_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_BD_LVDS_Output_Type_U2(pu32PNLInst, Type);
#endif
}

APIPNL_Result MApi_PNL_SetSSC_En_U2(void *pInstance, MS_BOOL bEnable)
{
#if 1
    _PNL_ENTRY();
    MDrv_PNL_SetSSC_En(pInstance, bEnable);
    _PNL_RETURN();
    return E_APIPNL_OK;
#endif
}

APIPNL_Result MApi_PNL_SetSSC_En(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetSSC_En PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETSSC_EN, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetSSC_En_U2(pu32PNLInst, bEnable);
#endif
}

APIPNL_Result MApi_PNL_SetSSC_Fmodulation_U2(void *pInstance, MS_U16 u16Fmodulation)
{
#if 1
    if(u16Fmodulation)
    {
        _PNL_ENTRY();

        MDrv_PNL_SetSSC_Fmodulation(pInstance, u16Fmodulation);

        _PNL_RETURN();
        return E_APIPNL_OK;
    }
    else
    {
        printf("\n\n!![Alert]zero u16Fmodulation (Lvds_SscSpan)!!\n\n");
        return E_APIPNL_FAIL;
    }
#endif
}

APIPNL_Result MApi_PNL_SetSSC_Fmodulation(MS_U16 u16Fmodulation)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetSSC_Fmodulation PNLArgs;
    PNLArgs.u16Fmodulation = u16Fmodulation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETSSC_FMODULATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetSSC_Fmodulation_U2(pu32PNLInst, u16Fmodulation);
#endif
}

APIPNL_Result MApi_PNL_SetSSC_Rdeviation_U2(void *pInstance, MS_U16 u16Rdeviation)
{
#if 1
    if(u16Rdeviation)
    {
        _PNL_ENTRY();
        MDrv_PNL_SetSSC_Rdeviation(pInstance, u16Rdeviation);
        _PNL_RETURN();
        return E_APIPNL_OK;
    }
    else
    {
        printf("\n\n!![Alert]zero u16Rdeviation (Lvds_SscStep)!!\n\n");
        return E_APIPNL_FAIL;
    }
#endif
}

APIPNL_Result MApi_PNL_SetSSC_Rdeviation(MS_U16 u16Rdeviation)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetSSC_Rdeviation PNLArgs;
    PNLArgs.u16Rdeviation = u16Rdeviation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETSSC_RDEVIATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetSSC_Rdeviation_U2(pu32PNLInst, u16Rdeviation);
#endif
}

APIPNL_Result MApi_PNL_SetOSDSSC_En_U2(void *pInstance, MS_BOOL bEnable)
{
#if 1
    _PNL_ENTRY();
    MDrv_PNL_SetOSDSSC_En(pInstance, bEnable);
    _PNL_RETURN();
    return E_APIPNL_OK;
#endif
}

APIPNL_Result MApi_PNL_SetOSDSSC_En(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetOSDSSC_En PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETOSDSSC_EN, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetOSDSSC_En_U2(pu32PNLInst, bEnable);
#endif
}

APIPNL_Result MApi_PNL_SetOSDSSC_Fmodulation_U2(void *pInstance, MS_U16 u16Fmodulation)
{
#if 1
    if(u16Fmodulation)
    {
        _PNL_ENTRY();

        MDrv_PNL_SetOSDSSC_Fmodulation(pInstance, u16Fmodulation);

        _PNL_RETURN();
        return E_APIPNL_OK;
    }
    else
    {
        printf("\n\n!![Alert]zero OSD u16Fmodulation (Lvds_SscSpan)!!\n\n");
        return E_APIPNL_FAIL;
    }
#endif
}

APIPNL_Result MApi_PNL_SetOSDSSC_Fmodulation(MS_U16 u16Fmodulation)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetOSDSSC_Fmodulation PNLArgs;
    PNLArgs.u16Fmodulation = u16Fmodulation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETOSDSSC_FMODULATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetOSDSSC_Fmodulation_U2(pu32PNLInst, u16Fmodulation);
#endif
}

APIPNL_Result MApi_PNL_SetOSDSSC_Rdeviation_U2(void *pInstance, MS_U16 u16Rdeviation)
{
#if 1
    if(u16Rdeviation)
    {
        _PNL_ENTRY();
        MDrv_PNL_SetOSDSSC_Rdeviation(pInstance, u16Rdeviation);
        _PNL_RETURN();
        return E_APIPNL_OK;
    }
    else
    {
        printf("\n\n!![Alert]zero OSD u16Rdeviation (Lvds_SscStep)!!\n\n");
        return E_APIPNL_FAIL;
    }
#endif
}

APIPNL_Result MApi_PNL_SetOSSSC_Rdeviation(MS_U16 u16Rdeviation)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetOSDSSC_Rdeviation PNLArgs;
    PNLArgs.u16Rdeviation = u16Rdeviation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETOSDSSC_RDEVIATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetOSDSSC_Rdeviation_U2(pu32PNLInst, u16Rdeviation);
#endif
}

void MApi_PNL_SetLPLLTypeExt_U2(void *pInstance, APIPNL_LINK_EXT_TYPE eLPLL_TypeExt)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext = _TransToDriver_APIPNL_LINK_EXT_TYPE_To_PNL_TYPE(eLPLL_TypeExt);
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_SetLPLLTypeExt(APIPNL_LINK_EXT_TYPE eLPLL_TypeExt)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetLPLLTypeExt PNLArgs;
    PNLArgs.eLPLL_TypeExt = eLPLL_TypeExt;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETLPLLTYPEEXT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetLPLLTypeExt_U2(pu32PNLInst, eLPLL_TypeExt);
#endif
}

void MApi_PNL_Init_MISC_U2(void *pInstance, APIPNL_MISC ePNL_MISC)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC = ePNL_MISC;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_Init_MISC(APIPNL_MISC ePNL_MISC)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_Init_MISC PNLArgs;
    PNLArgs.ePNL_MISC = ePNL_MISC;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_INIT_MISC, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_Init_MISC_U2(pu32PNLInst, ePNL_MISC);
#endif
}

MS_U32 MApi_PNL_GetMiscStatus_U2(void* pInstance)
{
    MS_U32 u32PNL_Misc = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u32PNL_Misc = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return u32PNL_Misc;
}

MS_U32 MApi_PNL_GetMiscStatus(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }
    stPNL_Get_MISC_Status PNLArgs;
    PNLArgs.u32Status = 0;

    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_MISC_STATUS, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u32Status;
    }
#else
    return MApi_PNL_GetMiscStatus_U2(pu32PNLInst);
#endif
}

void MApi_PNL_MOD_OutputConfig_User_U2(void *pInstance, MS_U32 u32OutputCFG0_7, MS_U32 u32OutputCFG8_15, MS_U32 u32OutputCFG16_21)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u32OutputCFG0_7   = %tx\n",(ptrdiff_t)u32OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u32OutputCFG8_15  = %tx\n",(ptrdiff_t)u32OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u32OutputCFG16_21 = %tx\n",(ptrdiff_t)u32OutputCFG16_21);
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7 = u32OutputCFG0_7;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15 = u32OutputCFG8_15;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21 = u32OutputCFG16_21;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_MOD_OutputConfig_User(MS_U32 u32OutputCFG0_7, MS_U32 u32OutputCFG8_15, MS_U32 u32OutputCFG16_21)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_OutputConfig_User PNLArgs;
    PNLArgs.u32OutputCFG0_7 = u32OutputCFG0_7;
    PNLArgs.u32OutputCFG8_15 = u32OutputCFG8_15;
    PNLArgs.u32OutputCFG16_21 = u32OutputCFG16_21;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_MOD_OUTPUTCONFIG_USER, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_MOD_OutputConfig_User_U2(pu32PNLInst, u32OutputCFG0_7, u32OutputCFG8_15, u32OutputCFG16_21);
#endif
}

void MApi_PNL_MOD_OutputChannelOrder_U2(void*  pInstance,
                                        MS_U8  u8OutputOrderType,
                                        MS_U16 u16OutputOrder0_3,
                                        MS_U16 u16OutputOrder4_7,
                                        MS_U16 u16OutputOrder8_11,
                                        MS_U16 u16OutputOrder12_13)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u8OutputOrderType   = %x\n", (unsigned int)u8OutputOrderType);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16OutputOrder0_3   = %x\n", (unsigned int)u16OutputOrder0_3);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16OutputOrder4_7   = %x\n", (unsigned int)u16OutputOrder4_7);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16OutputOrder8_11  = %x\n", (unsigned int)u16OutputOrder8_11);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16OutputOrder12_13 = %x\n", (unsigned int)u16OutputOrder12_13);
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType = u8OutputOrderType;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3 = u16OutputOrder0_3;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7 = u16OutputOrder4_7;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11 = u16OutputOrder8_11;
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13 = u16OutputOrder12_13;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_MOD_OutputChannelOrder(MS_U8  u8OutputOrderType,
                                     MS_U16 u16OutputOrder0_3,
                                     MS_U16 u16OutputOrder4_7,
                                     MS_U16 u16OutputOrder8_11,
                                     MS_U16 u16OutputOrder12_13)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_OutputChannelOrder PNLArgs;
    PNLArgs.u8OutputOrderType = u8OutputOrderType;
    PNLArgs.u16OutputOrder0_3 = u16OutputOrder0_3;
    PNLArgs.u16OutputOrder4_7 = u16OutputOrder4_7;
    PNLArgs.u16OutputOrder8_11 = u16OutputOrder8_11;
    PNLArgs.u16OutputOrder12_13 = u16OutputOrder12_13;

    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_MOD_OUTPUT_CHANNEL_ORDER, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_MOD_OutputChannelOrder_U2(pu32PNLInst, u8OutputOrderType, u16OutputOrder0_3, u16OutputOrder4_7, u16OutputOrder8_11, u16OutputOrder12_13);
#endif
}
//-------------------------------------------------------------------------------------------------
/// Set Flag to bypass timing change in MApi_PNL_Init()
/// @param  bFlag               \b IN: TRUE: initialize XC by skipping SW reset; FALSE: initialize XC in normal case
/// @return APIPNL_Result
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_SkipTimingChange_U2(void *pInstance, MS_BOOL bFlag)
{
    MS_BOOL bCap = MDrv_PNL_SkipTimingChange_GetCaps(pInstance);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if( (bFlag == TRUE) && (bCap == TRUE) )
    {
        pPNLResourcePrivate->stapiPNL._bSkipTimingChange = TRUE;
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return E_APIPNL_OK;
    }
    else
    {
        //else, not supported
        pPNLResourcePrivate->stapiPNL._bSkipTimingChange = FALSE;
        _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
        return E_APIPNL_FAIL;
    }
}

APIPNL_Result MApi_PNL_SkipTimingChange(MS_BOOL bFlag)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SkipTimingChange PNLArgs;
    PNLArgs.bFlag = bFlag;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SKIPTIMINGCHANGE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eResult;
    }
#else
    return MApi_PNL_SkipTimingChange_U2(pu32PNLInst, bFlag);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Pre Set Mode On
/// @param  bSetMode               \b IN: TRUE: when set mode on  t; FALSE: when set mode off
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_PreSetModeOn_U2(void *pInstance, MS_BOOL bSetModeOn)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
    {
        MDrv_PNL_PreSetModeOn(pInstance, bSetModeOn);
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        MDrv_PNL_PreSetModeOn(pInstance, bSetModeOn);
    }
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return E_APIPNL_OK;
}

APIPNL_Result MApi_PNL_PreSetModeOn(MS_BOOL bSetModeOn)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_PreSetModeOn PNLArgs;
    PNLArgs.bSetMode = bSetModeOn;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_PRE_SET_MODE_ON, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eResult;
    }
#else
    return MApi_PNL_PreSetModeOn_U2(pu32PNLInst, bSetModeOn);
#endif
}

void MApi_PNL_HWLVDSReservedtoLRFlag_U2(void *pInstance, MS_PNL_HW_LVDSResInfo lvdsresinfo)
{
    PNL_DrvHW_LVDSResInfo drv_lvdsinfo;
    drv_lvdsinfo.u32pair = lvdsresinfo.u32pair;
    drv_lvdsinfo.u16channel = lvdsresinfo.u16channel;
    drv_lvdsinfo.bEnable = lvdsresinfo.bEnable;
    MDrv_PNL_HWLVDSReservedtoLRFlag(pInstance, drv_lvdsinfo);
}

void MApi_PNL_HWLVDSReservedtoLRFlag(MS_PNL_HW_LVDSResInfo lvdsresinfo)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_HWLVDSReservedtoLRFlag PNLArgs;
    PNLArgs.lvdsresinfo = lvdsresinfo;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_HWLVDSReservedtoLRFlag_U2(pu32PNLInst, lvdsresinfo);
#endif
}

////////////////////////////////////////////////////////////////////////
// Turn OD function
////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Initialize OverDrive
/// @param  pXC_ODInitData                  \b IN: the Initialized Data
/// @param  u32ODInitDataLen                \b IN: the length of the initialized data
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_OverDriver_Init_U2(void *pInstance, MS_PNL_OD_INITDATA *pPNL_ODInitData, MS_U32 u32ODInitDataLen)
{
    if(u32ODInitDataLen != sizeof(MS_PNL_OD_INITDATA))
    {
        return E_APIPNL_FAIL;
    }

    _PNL_ENTRY();
    MDrv_PNL_OverDriver_Init(pInstance, pPNL_ODInitData->u32OD_MSB_Addr, pPNL_ODInitData->u32OD_LSB_Addr, pPNL_ODInitData->u8ODTbl);
    _PNL_RETURN();

    return E_APIPNL_OK;
}

APIPNL_Result MApi_PNL_OverDriver_Init(MS_PNL_OD_INITDATA *pPNL_ODInitData, MS_U32 u32ODInitDataLen)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_OverDriver_Init PNLArgs;
    PNLArgs.pPNL_ODInitData = pPNL_ODInitData;
    PNLArgs.u32ODInitDataLen = u32ODInitDataLen;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_OVERDRIVER_INIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eResult;
    }
#else
    return MApi_PNL_OverDriver_Init_U2(pu32PNLInst, pPNL_ODInitData, u32ODInitDataLen);
#endif
}

//-------------------------------------------------------------------------------------------------
/// OverDrive Enable
/// @param  bEnable               \b IN: TRUE: Enable OverDrive; FALSE: Disable OverDrive
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_OverDriver_Enable_U2(void *pInstance, MS_BOOL bEnable)
{
    _PNL_ENTRY();
    MDrv_PNL_OverDriver_Enable(pInstance, bEnable);
    _PNL_RETURN();
    return E_APIPNL_OK;
}

APIPNL_Result MApi_PNL_OverDriver_Enable(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_OverDriver_Enable PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_OVERDRIVER_ENABLE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eResult;
    }
#else
    return MApi_PNL_OverDriver_Enable_U2(pu32PNLInst, bEnable);
#endif
}

void MApi_MOD_PVDD_Power_Setting_U2(void *pInstance, MS_BOOL bIs2p5)
{
    _PNL_ENTRY();
    MDrv_MOD_PVDD_Power_Setting(pInstance, bIs2p5);
    _PNL_RETURN();
}

void MApi_MOD_PVDD_Power_Setting(MS_BOOL bIs2p5)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_PVDD_Power_Setting PNLArgs;
    PNLArgs.bIs2p5 = bIs2p5;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_MOD_PVDD_POWER_SETTING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_MOD_PVDD_Power_Setting_U2(pu32PNLInst, bIs2p5);
#endif
}

//-------------------------------------------------------------------------------------------------
///-obosolte!! use MApi_PNL_Control_Out_Swing instead
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_SetDiffSwingLevel_U2(void *pInstance, MS_U8 u8Swing_Level)
{
    return FALSE;
}

MS_BOOL MApi_PNL_SetDiffSwingLevel(MS_U8 u8Swing_Level)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetDiffSwingLevel PNLArgs;
    PNLArgs.u8Swing_Level = u8Swing_Level;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETDIFFSWINGLEVEL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_SetDiffSwingLevel_U2(pu32PNLInst, u8Swing_Level);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get real LPLL type from driver
/// @return MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_Pnl_Get_LPLL_Type_U2(void *pInstance)
{
    MS_U8 u8Value = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_TYPE_EXT)
    {
        u8Value = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext;
    }
    else
    {
        u8Value = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type;
    }
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8Value;
}

MS_U8 MApi_Pnl_Get_LPLL_Type(void)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return LINK_TTL;
        }
    }

    stPNL_Get_LPLL_Type PNLArgs;
    PNLArgs.u8Return = LINK_TTL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_LPLL_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return LINK_TTL;
    }
    else
    {
        return PNLArgs.u8Return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get TCON capability
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_Get_TCON_Capability_U2(void *pInstance)
{
    return MDrv_PNL_Is_SupportTCON(pInstance);
}

MS_BOOL MApi_PNL_Get_TCON_Capability(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Get_TCON_Capability PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_TCON_CAPABILITY, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_Get_TCON_Capability_U2(pu32PNLInst);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set FRC MOD pair swap
/// @param  bEnable               \b IN: u8Polarity, (d:c:b:a)=([15:14],[13:12],[11:10],[9:8]) => (10,00,11,01), => (2,0,3,1)
//-------------------------------------------------------------------------------------------------
void MApi_PNL_SetPairSwap_U2(void *pInstance, MS_U32 u32Polarity)
{
    if(MDrv_PNL_Is_SupportFRC(pInstance))
        MDrv_FRC_MOD_ForcePairSwap(pInstance, u32Polarity);
}

void MApi_PNL_SetPairSwap(MS_U32 u32Polarity)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetPairSwap PNLArgs;
    PNLArgs.u32Polarity = u32Polarity;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETPAIRSWAP, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
     MApi_PNL_SetPairSwap_U2(pu32PNLInst, u32Polarity);
#endif
}

void MApi_PNL_SetExt_LPLL_Type_U2(void *pInstance, MS_U16 u16Ext_lpll_type)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Ext_LpllType = u16Ext_lpll_type;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_SetExt_LPLL_Type(MS_U16 u16Ext_lpll_type)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetExt_LPLL_Type PNLArgs;
    PNLArgs.u16Ext_lpll_type = u16Ext_lpll_type;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SET_EXT_LPLL_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetExt_LPLL_Type_U2(pu32PNLInst, u16Ext_lpll_type);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Cal Ext LPLL Set by DCLK
/// @param  ldHz               \b IN: ldHz = Htt*Vtt*Vfreq
//-------------------------------------------------------------------------------------------------
void MApi_PNL_CalExtLPLLSETbyDClk_U2(void *pInstance, MS_U32 ldHz)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    MDrv_PNL_CalExtLPLLSETbyDClk(pInstance, (MS_U8)pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode, (MS_U8)pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Ext_LpllType, ldHz);
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_CalExtLPLLSETbyDClk(MS_U32 ldHz)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_CalExtLPLLSETbyDClk PNLArgs;
    PNLArgs.ldHz = ldHz;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_CALEXTLPLLSETBYDCLK, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_CalExtLPLLSETbyDClk_U2(pu32PNLInst, ldHz);
#endif
}

MS_U32 MApi_PNL_SetPowerState_U2(void *pInstance, EN_POWER_MODE ePowerState)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    MS_U8 *tmp[3];

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
#if (KERNEL_DRIVER_PNL_STR_PATCH == 1)
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    PNL_REGS_SAVE_AREA *pPNLResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pPNLResourceStr);
#endif
    if (ePowerState == E_POWER_SUSPEND)
    {
        pPNLResourcePrivate->stapiPNL.ePrevPowerState = ePowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;
    }
    else if (ePowerState == E_POWER_RESUME)
    {
        if (pPNLResourcePrivate->stapiPNL.ePrevPowerState == E_POWER_SUSPEND)
        {
            _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)); //can not entry semaphore twice!
#if (KERNEL_DRIVER_PNL_STR_PATCH == 1)
            if((pPNLResourceStr->pSelPanelType.m_ePanelLinkType == LINK_EXT)&&
                (pPNLResourceStr->pSelPanelType.m_wPanelWidth >= 3840)&&(pPNLResourceStr->pSelPanelType.m_wPanelHeight >= 2160))
            {
                MApi_PNL_MOD_OutputConfig_User_U2(pInstance,pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                                pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                                pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
                MApi_BD_LVDS_Output_Type_U2(pInstance,pPNLResourceStr->BD_LVDS_Output_Type);
                MApi_PNL_ForceSetPanelDCLK_U2(pInstance,pPNLResourcePrivate->stapiPNL._u16PanelDCLK,pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable);
            }
            if(MApi_PNL_GetVideoHWTraining_Status_U2(pInstance))
                MApi_PNL_SetVideoHWTraining_U2(pInstance, TRUE);
            MApi_PNL_Init_U2(pInstance, &pPNLResourceStr->pSelPanelType);
            MApi_PNL_EnablePanel_U2(pInstance, TRUE);
#endif
            if (!(pPNLResourcePrivate->stapiPNL._pu8GammaTab[0] == NULL ||
                pPNLResourcePrivate->stapiPNL._pu8GammaTab[1] == NULL ||
                pPNLResourcePrivate->stapiPNL._pu8GammaTab[2] == NULL))
            {
                tmp[0]=&(pPNLResourcePrivate->stapiPNL._pu8GammaTab[0][0]);
                tmp[1]=&(pPNLResourcePrivate->stapiPNL._pu8GammaTab[1][0]);
                tmp[2]=&(pPNLResourcePrivate->stapiPNL._pu8GammaTab[2][0]);
                MApi_PNL_SetGammaTbl_U2(pInstance,
                                        pPNLResourcePrivate->stapiPNL._eGammaType,
                                        tmp,
                                        pPNLResourcePrivate->stapiPNL._eGammaMapMode);
            }

            _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

            pPNLResourcePrivate->stapiPNL.ePrevPowerState = ePowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n", __FUNCTION__, __LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n", __FUNCTION__, __LINE__, ePowerState);
        u32Return = UTOPIA_STATUS_NOT_SUPPORTED;
    }

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u32Return;
}

MS_U32 MApi_PNL_SetPowerState(EN_POWER_MODE ePowerState)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return UTOPIA_STATUS_FAIL;
        }
    }

    stPNL_SetPowerState PNLArgs;
    PNLArgs.ePowerState = ePowerState;
    PNLArgs.u32Return = UTOPIA_STATUS_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETPOWERSTATE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return PNLArgs.u32Return;
    }
#else
    return MApi_PNL_SetPowerState_U2(pu32PNLInst, ePowerState);
#endif
}

MS_BOOL MApi_PNL_EnableInternalTermination_U2(void *pInstance, MS_BOOL bEnable)
{
    return MDrv_PNL_EnableInternalTermination(pInstance, bEnable);
}

MS_BOOL MApi_PNL_EnableInternalTermination(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_EnableInternalTermination PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_ENABLEINTERNALTERMINATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_EnableInternalTermination_U2(pu32PNLInst, bEnable);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Do handshake for special output device, ex. VB1
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_OutputDeviceHandshake_U2(void *pInstance)
{
    MS_BOOL bResult = TRUE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    _PNL_ENTRY();

    // For VB1 type, need to do handshake
    if( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
    {
        bResult = MDrv_PNL_VBY1_Handshake(pInstance);
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        bResult = MDrv_PNL_VBY1_Handshake(pInstance);
    }

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return bResult;

}

MS_BOOL MApi_PNL_OutputDeviceHandshake(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_OutputDeviceHandshake PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_OUTPUTDEVICEHANDSHAKE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_OutputDeviceHandshake_U2(pu32PNLInst);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Do OC handshake for special output device, ex. VB1
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_OutputDeviceOCHandshake_U2(void *pInstance)
{
    MS_BOOL bResult = TRUE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    _PNL_ENTRY();

    // For VB1 type, need to do handshake
    if( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
    {
        bResult = MDrv_PNL_VBY1_OC_Handshake(pInstance);
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        bResult = MDrv_PNL_VBY1_OC_Handshake(pInstance);
    }

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bResult;

}

MS_BOOL MApi_PNL_OutputDeviceOCHandshake(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_OutputDeviceHandshake PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_OUTPUTDEVICEOCHANDSHAKE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_OutputDeviceOCHandshake_U2(pu32PNLInst);
#endif
}



//-------------------------------------------------------------------------------------------------
/// set necessary setting for outputing interlace timing to rear
/// @return APIPNL_Result
//-------------------------------------------------------------------------------------------------
APIPNL_Result MApi_PNL_SetOutputInterlaceTiming_U2(void* pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    APIPNL_Result bReturn = E_APIPNL_FAIL;
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _PNL_ENTRY();
    bReturn = (APIPNL_Result) MDrv_PNL_SetOutputInterlaceTiming(pInstance, bEnable);
    _PNL_RETURN();
     _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bReturn;
}

APIPNL_Result MApi_PNL_SetOutputInterlaceTiming(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_APIPNL_FAIL;
        }
    }

    stPNL_SetOutputInterlaceTiming PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETOUTPUTINTERLACETIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.eReturn;
    }
#else
    return MApi_PNL_SetOutputInterlaceTiming_U2(pu32PNLInst, bEnable);
#endif
}

MS_BOOL MApi_PNL_GetOutputInterlaceTiming_U2(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    MS_BOOL bIsInterlace = FALSE;
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _PNL_ENTRY();

    MDrv_PNL_GetOutputInterlaceTiming(pInstance, &bIsInterlace);

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bIsInterlace;

}

MS_BOOL MApi_PNL_GetOutputInterlaceTiming(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetOutputInterlaceTiming PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETOUTPUTINTERLACETIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_GetOutputInterlaceTiming_U2(pu32PNLInst);
#endif
}

void MApi_PNL_GetPanelData_U2(void* pInstance, PanelType* pstPNLData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    if(NULL == pstPNLData)
    {
        printf("NULL input parameter pstPNLData\n");
        return;
    }

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    memcpy(pstPNLData, &(pPNLResourcePrivate->stapiPNL._stPanelTypeSaved), sizeof(PanelType));
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_GetPanelData(PanelType* pstPNLData)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_GetPanelData PNLArgs;
    PNLArgs.pstPNLData = pstPNLData;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_GetPanelData_U2(pu32PNLInst, pstPNLData);
#endif
}

void MApi_PNL_DumpPanelData_U2(void* pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    _DumpData(pInstance);
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_DumpPanelData(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_DUMPPANELDATA, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_DumpPanelData_U2(pu32PNLInst);
#endif
}

void MApi_PNL_SetSSC_U2(void* pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    _PNL_ENTRY();
    MDrv_PNL_SetSSC(pInstance, u16Fmodulation, u16Rdeviation, bEnable);
    _PNL_RETURN();
}

void MApi_PNL_SetSSC(MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetSSC PNLArgs;
    PNLArgs.u16Fmodulation = u16Fmodulation;
    PNLArgs.u16Rdeviation = u16Rdeviation;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETSSC, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetSSC_U2(pu32PNLInst, u16Fmodulation, u16Rdeviation, bEnable);
#endif
}

MS_U16 MApi_PNL_GetPanelOnTiming_U2(void* pInstance, APIPNL_POWER_TIMING_SEQ seq)
{
    MS_U16 u16PanelOnTiming = E_APIPNL_POWER_TIMING_NA;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(E_APIPNL_POWER_TIMING_1 == seq)
    {
        u16PanelOnTiming = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOnTiming1;
    }
    else if(E_APIPNL_POWER_TIMING_2 == seq)
    {
        u16PanelOnTiming = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOnTiming2;
    }
    else
    {
        u16PanelOnTiming = E_APIPNL_POWER_TIMING_NA;
    }

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return u16PanelOnTiming ;
}

MS_U16 MApi_PNL_GetPanelOnTiming(APIPNL_POWER_TIMING_SEQ seq)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelOnTiming PNLArgs;
    PNLArgs.seq = seq;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETPANELONTIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MApi_PNL_GetPanelOnTiming_U2(pu32PNLInst, seq);
#endif
}

MS_U16 MApi_PNL_GetPanelOffTiming_U2(void* pInstance, APIPNL_POWER_TIMING_SEQ seq)
{
    MS_U16 u16PanelOffTiming = E_APIPNL_POWER_TIMING_NA;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(E_APIPNL_POWER_TIMING_1 == seq)
    {
        u16PanelOffTiming = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOffTiming1;
    }
    else if(E_APIPNL_POWER_TIMING_2 == seq)
    {
        u16PanelOffTiming = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelOffTiming2;
    }
    else
    {
        u16PanelOffTiming = E_APIPNL_POWER_TIMING_NA;
    }

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return u16PanelOffTiming;
}

MS_U16 MApi_PNL_GetPanelOffTiming(APIPNL_POWER_TIMING_SEQ seq)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelOffTiming PNLArgs;
    PNLArgs.seq = seq;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETPANELOFFTIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
#else
    return MApi_PNL_GetPanelOffTiming_U2(pu32PNLInst, seq);
#endif
}

MS_U8 MApi_PNL_GetPanelDimCtrl_U2(void* pInstance, APIPNL_DIMMING_CTRL dim_type)
{
    MS_U8 u8PanelDimmingCtrl = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(E_APIPNL_DIMMING_MAX == dim_type )
    {
        u8PanelDimmingCtrl = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelMaxPWMVal;
    }
    else if(E_APIPNL_DIMMING_MIN == dim_type )
    {
        u8PanelDimmingCtrl = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelMinPWMVal;
    }
    else
    {
        u8PanelDimmingCtrl = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16PanelDimmingCtl;
    }

    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return u8PanelDimmingCtrl;
}

MS_U8 MApi_PNL_GetPanelDimCtrl(APIPNL_DIMMING_CTRL dim_type)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelDimCtrl PNLArgs;
    PNLArgs.dim_type = dim_type;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETPANELDIMCTRL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u8Return;
    }
#else
    return MApi_PNL_GetPanelDimCtrl_U2(pu32PNLInst, dim_type);
#endif
}

MS_U8** MApi_PNL_GetAllGammaTbl_U2(void* pInstance)
{
    MS_U8** ppallGammaTbls;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    ppallGammaTbls = pPNLResourcePrivate->stapiPNL.m_ppAllGammaTbls;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return ppallGammaTbls;
}

MS_U8** MApi_PNL_GetAllGammaTbl(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return NULL;
        }
    }

    stPNL_GetAllGammaTbl PNLArgs;
    PNLArgs.ppu8Return = NULL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETALLGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return PNLArgs.ppu8Return;
    }
#else
    return MApi_PNL_GetAllGammaTbl_U2(pu32PNLInst);
#endif
}

MS_BOOL MApi_PNL_EnablePanel_U2(void* pInstance, MS_BOOL bPanelOn)
{
    MS_BOOL bResult = FALSE;
    MS_BOOL bCalEn = ENABLE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    _PNL_ENTRY();

    if( !pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized )
    {
        PNL_PRINT_VAR(pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized);
        PNL_DBG_MSG("Usage of uninitialized function.!!");
    }

    if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & (MS_U32)E_APIPNL_MISC_SKIP_CALIBRATION)
        bCalEn = DISABLE;

    if( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
    {
        bCalEn = DISABLE;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        bCalEn = DISABLE;
    }

    if ( E_PNL_OK == MDrv_PNL_En(pInstance, bPanelOn, bCalEn) )
    {
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled = bPanelOn;
        bResult = TRUE;
    }
    else
    {
        bResult = FALSE;
    }

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bResult;
}

MS_BOOL MApi_PNL_EnablePanel(MS_BOOL bPanelOn)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_EnablePanel PNLArgs;
    PNLArgs.bPanelOn = bPanelOn;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_ENABLEPANEL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_EnablePanel_U2(pu32PNLInst, bPanelOn);
#endif
}

MS_BOOL MApi_PNL_SetGammaTbl_U2(void* pInstance,
                                            APIPNL_GAMMA_TYPE eGammaType,
                                            MS_U8* pu8GammaTab[3],
                                            APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
    MS_BOOL bResult = FALSE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stapiPNL.m_ppAllGammaTbls = pu8GammaTab;
    pPNLResourcePrivate->stapiPNL._eGammaType = eGammaType;
    pPNLResourcePrivate->stapiPNL._eGammaMapMode = Gamma_Map_Mode;
    memcpy(pPNLResourcePrivate->stapiPNL._pu8GammaTab[0] , pu8GammaTab[0] ,sizeof(MS_U8)*GammaTblSize);
    memcpy(pPNLResourcePrivate->stapiPNL._pu8GammaTab[1] , pu8GammaTab[1] ,sizeof(MS_U8)*GammaTblSize);
    memcpy(pPNLResourcePrivate->stapiPNL._pu8GammaTab[2] , pu8GammaTab[2] ,sizeof(MS_U8)*GammaTblSize);

    _PNL_ENTRY();

    bResult = ( E_PNL_OK == MDrv_PNL_SetGammaTbl(
    pInstance,
    (DRVPNL_GAMMA_TYPE)eGammaType,
    pu8GammaTab,
    Gamma_Map_Mode == E_APIPNL_GAMMA_8BIT_MAPPING ? E_DRVPNL_GAMMA_8BIT_MAPPING : E_DRVPNL_GAMMA_10BIT_MAPPING ));

    if(bResult != TRUE)
    {
        PNL_ASSERT(0, "[%s] fails: ( APIPNL_GAMMA_TYPE: %d, APIPNL_GAMMA_MAPPEING_MODE %d) \n.", __FUNCTION__, eGammaType, Gamma_Map_Mode);
    }

    _PNL_RETURN();
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

    return bResult;
}

MS_BOOL MApi_PNL_SetGammaTbl(APIPNL_GAMMA_TYPE eGammaType,
                                        MS_U8* pu8GammaTab[3],
                                        APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetGammaTbl PNLArgs;
    PNLArgs.eGammaType = eGammaType;
    PNLArgs.Gamma_Map_Mode = Gamma_Map_Mode;
    PNLArgs.bReturn = FALSE;
    PNLArgs.pu8GammaTab[0] = pu8GammaTab[0];
    PNLArgs.pu8GammaTab[1] = pu8GammaTab[1];
    PNLArgs.pu8GammaTab[2] = pu8GammaTab[2];
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_SetGammaTbl_U2(pu32PNLInst, eGammaType, pu8GammaTab, Gamma_Map_Mode);
#endif
}

MS_BOOL MApi_PNL_GetGammaTbl_U2(void* pInstance,
                                            APIPNL_GAMMA_TYPE eGammaType,
                                            MS_U8* pu8GammaTab[3],
                                            APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
    MS_BOOL bResult = FALSE;

    _PNL_ENTRY();
    bResult = ( E_PNL_OK == MDrv_PNL_GetGammaTbl(
    pInstance,
    (DRVPNL_GAMMA_TYPE)eGammaType,
    pu8GammaTab,
    Gamma_Map_Mode == E_APIPNL_GAMMA_8BIT_MAPPING ? E_DRVPNL_GAMMA_8BIT_MAPPING : E_DRVPNL_GAMMA_10BIT_MAPPING ));

    if(bResult != TRUE)
    {
        PNL_ASSERT(0, "[%s] fails: ( APIPNL_GAMMA_TYPE: %d, APIPNL_GAMMA_MAPPEING_MODE %d) \n.", __FUNCTION__, eGammaType, Gamma_Map_Mode);
    }

    _PNL_RETURN();
    return bResult;
}

MS_BOOL MApi_PNL_GetGammaTbl(APIPNL_GAMMA_TYPE eGammaType,
                                        MS_U8* pu8GammaTab[3],
                                        APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetGammaTbl PNLArgs;
    PNLArgs.eGammaType = eGammaType;
    PNLArgs.Gamma_Map_Mode = Gamma_Map_Mode;
    PNLArgs.bReturn = FALSE;
    PNLArgs.pu8GammaTab[0] = pu8GammaTab[0];
    PNLArgs.pu8GammaTab[1] = pu8GammaTab[1];
    PNLArgs.pu8GammaTab[2] = pu8GammaTab[2];
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_GetGammaTbl_U2(pu32PNLInst, eGammaType, pu8GammaTab, Gamma_Map_Mode);
#endif
}

MS_BOOL MApi_PNL_SetGammaValue_U2(void* pInstance, MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue)
{
    MS_BOOL bResult = FALSE;

    _PNL_ENTRY();

    bResult = ( E_PNL_OK == MDrv_PNL_SetGammaValue(pInstance, u8Channel, u16Offset, u16GammaValue));

    _PNL_RETURN();
    return bResult;
}

MS_BOOL MApi_PNL_SetGammaValue(MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetGammaValue PNLArgs;
    PNLArgs.u8Channel = u8Channel;
    PNLArgs.u16Offset = u16Offset;
    PNLArgs.u16GammaValue = u16GammaValue;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SETGAMMAVALUE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
#else
    return MApi_PNL_SetGammaValue_U2(pu32PNLInst, u8Channel, u16Offset, u16GammaValue);
#endif
}

MS_U8 MApi_PNL_GetLPLLMode_U2(void *pInstance)
{
    MS_U8 u8LPLLMode = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u8LPLLMode = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u8LPLLMode;
}

MS_U8 MApi_PNL_GetLPLLMode(void)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_MODE_SINGLE;
        }
    }

    stPNL_GetLPLLMode PNLArgs;
    PNLArgs.u8Return = E_PNL_MODE_SINGLE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETLPLLMODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_MODE_SINGLE;
    }
    else
    {
        return PNLArgs.u8Return;
    }
}

MS_U16 MApi_PNL_GetDefVFreq_U2(void *pInstance)
{
    MS_U16 u16DefaultVFreq = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16DefaultVFreq = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return u16DefaultVFreq;
}

MS_U16 MApi_PNL_GetDefVFreq(void)
{
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetDefVFreq PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETDEFVFREQ, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
}

MS_U32 _XC_PNL_GET_DEVICE_NUM(void)
{
    return MDrv_PNL_GetDeviceNum();
}

MS_BOOL MApi_PNL_Check_VBY1_Handshake_Status_U2(void *pInstance)
{
    MS_BOOL bResult = false;

    _PNL_ENTRY();
        bResult = MDrv_PNL_Check_VBY1_Handshake_Status(pInstance);
    _PNL_RETURN();

    return bResult;
}

MS_BOOL MApi_PNL_Check_VBY1_Handshake_Status(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_MODE_SINGLE;
        }
    }
    stPNL_CheckVBY1HandshakeStatus PNLArgs;
    PNLArgs.bResult = FALSE;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_CHECKVBY1HANDSHAKESTATUS, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.bResult;
    }
#else
    return MApi_PNL_Check_VBY1_Handshake_Status_U2(pu32PNLInst);
#endif
}

void MApi_PNL_SetVideoHWTraining_U2(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En = bEnable;
    MDrv_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE, bEnable);
    printf("\nEnable Video HW Training mode\n");
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_SetVideoHWTraining(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_Video_HWTrainingMode PNLArgs;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SET_VIDEOHWTRAINING_MODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetVideoHWTraining_U2(pu32PNLInst, bEnable);
#endif
}

void MApi_PNL_SetOSDHWTraining_U2(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->stdrvPNL._stPnlInitData.bOSD_HW_Training_En = bEnable;
    MDrv_PNL_VBY1_Hardware_TrainingMode_En(pInstance, FALSE, bEnable);
    printf("Enable OSD HW Training mode\n");
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
}

void MApi_PNL_SetOSDHWTraining(MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_OSD_HWTrainingMode PNLArgs;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_SET_OSDHWTRAINING_MODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
#else
    MApi_PNL_SetOSDHWTraining_U2(pu32PNLInst, bEnable);
#endif
}

MS_BOOL MApi_PNL_GetVideoHWTraining_Status_U2(void *pInstance)
{
    MS_BOOL bStatus = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    bStatus = pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bStatus;
}


MS_BOOL MApi_PNL_GetVideoHWTraining_Status(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetVideoHWTraining_status PNLArgs;
    PNLArgs.bStatus = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_VIDEOHWTRAINING_MODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.bStatus;
    }
#else
    return MApi_PNL_GetVideoHWTraining_Status_U2(pu32PNLInst);
#endif
}

MS_BOOL MApi_PNL_GetOSDHWTraining_Status_U2(void *pInstance)
{
    MS_BOOL bStatus = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    bStatus = pPNLResourcePrivate->stdrvPNL._stPnlInitData.bOSD_HW_Training_En;
    _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
    return bStatus;
}

MS_BOOL MApi_PNL_GetOSDHWTraining_Status(void)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetOSDHWTraining_status PNLArgs;
    PNLArgs.bStatus = 0;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GET_OSDHWTRAINING_MODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.bStatus;
    }
#else
    return MApi_PNL_GetOSDHWTraining_Status_U2(pu32PNLInst);
#endif
}

APIPNL_Result MApi_PNL_Setting_U2(void* pInstance,MS_U32 u32Cmd,void *pCmdArgs,MS_U32 u32CmdArgsSize)
{

    MS_BOOL bResult = E_APIPNL_FAIL;
    switch(u32Cmd)
    {
        case E_PNL_MOD_PECURRENT_SETTING:
        {
            if(u32CmdArgsSize == sizeof(ST_PNL_MOD_PECURRENT_SETTING))
            {
                ST_PNL_MOD_PECURRENT_SETTING *stPnlModPecurrentSetting = (ST_PNL_MOD_PECURRENT_SETTING *)pCmdArgs;

                PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
                UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
                _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

                MDrv_PNL_MOD_PECurrent_Setting(pInstance, stPnlModPecurrentSetting->u16Current_Level, stPnlModPecurrentSetting->u16Channel_Select);

                _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

                bResult = E_APIPNL_OK;
            }
            else
            {
                //PNL_DBG(PNL_DBGLEVEL_INIT,"E_PNL_MOD_PECURRENT_SETTING Struct Size No Match\n");
                return bResult;
            }
            break;
        }
        case E_PNL_CONTROL_OUT_SWING:
        {
            if(u32CmdArgsSize == sizeof(ST_PNL_CONTROL_OUT_SWING))
            {
                ST_PNL_CONTROL_OUT_SWING *stPnlControlOutSwing = (ST_PNL_CONTROL_OUT_SWING *)pCmdArgs;

                PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
                UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
                _PNL_SEMAPHORE_ENTRY(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));

                bResult = MDrv_PNL_Control_Out_Swing(pInstance, stPnlControlOutSwing->u16Swing_Level);

                _PNL_SEMAPHORE_RETURN(pInstance,PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID));
            }
            else
            {
                //PNL_DBG(PNL_DBGLEVEL_INIT,"E_PNL_CONTROL_OUT_SWING Struct Size No Match\n");
                return bResult;
            }
            break;
        }
        default:
            printf("Unknow Handle ID!\n");
            break;
    }

    return bResult;
}

APIPNL_Result MApi_PNL_Setting(MS_U32 u32Cmd,void *pCmdArgs,MS_U32 u32CmdArgsSize)
{
#ifdef UTOPIA_20
    if (pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_Setting PNLArgs;
    PNLArgs.u32Cmd = u32Cmd;
    PNLArgs.pCmdArgs = pCmdArgs;
    PNLArgs.u32CmdArgsSize = u32CmdArgsSize;
    PNLArgs.eReturnValue = E_APIPNL_FAIL;

    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_Setting, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturnValue;
    }
#else
    return MApi_PNL_Setting_U2(pu32PNLInst, u32Cmd, pCmdArgs, u32CmdArgsSize);
#endif
}


//} // Marked it by coverity_0553

#endif
