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
#ifndef _API_XC_PANEL_EX_C_
#define _API_XC_PANEL_EX_C_

/******************************************************************************/
/*                      Includes                                              */
/******************************************************************************/
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "apiPNL_EX.h"
#include "drvPNL.h"
#include "PNL_private.h"

// for Utopia 2.0 to Utopia 1.0 compatibility
extern void* pu32PNLInst;
void* pu32PNLInst1 = NULL;

#define _PNL_SELECT_INSTANCE(u32Id) ((u32Id == 0)? pu32PNLInst : pu32PNLInst1)
#define _PNL_SELECT_INSTANCE_P(u32Id) ((u32Id == 0)? &pu32PNLInst : &pu32PNLInst1)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define PNL_DBG_MSG(fmt,...)                                                                 \
    MS_DEBUG_MSG(do{                                                                                      \
        if(_u16PnlDbgSwitch){                                                                   \
           printf( "[PNL_DBG_MSG]: %s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
          }                                                                                  \
      } while(0))


//
//  Dump m_panel data
//
static void _MApi_XC_DumpPanelData(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_DUMPPANELDATA, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}
//
//
void _SetSSC(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetSSC PNLArgs;
    PNLArgs.u16Fmodulation = u16Fmodulation;
    PNLArgs.u16Rdeviation = u16Rdeviation;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETSSC, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}
//
//
static void  _EmptyMethod(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    UNUSED(bEnable);
    PNL_DBG_MSG(" Error!! <==============  Calling a non-registered function!!");
}
//
//
static MS_U16 _getPanelHstart(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelHStart;
    }
}

static MS_U16 _getPanelVstart(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelVStart;
    }
}

static MS_U16 _getPanelWidth (const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelWidth;
    }
}

static MS_U16 _getPanelHeight(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelHeight;
    }
}

static MS_U16 _getPanelhtotal(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelHTotal;
    }
}

static MS_U16 _getPanelVtotal(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_wPanelVTotal;
    }
}

static MS_U32 _getPanelMinSET(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_dwPanelMinSET;
    }
}

static MS_U32 _getPanelMaxSET(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_dwPanelMaxSET;
    }
}

static MS_U16 _getPanelDefVFreq(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetDefVFreq PNLArgs;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETDEFVFREQ, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
}

static MS_U8 _getPanel_LPLL_Mode(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_MODE_SINGLE;
        }
    }

    stPNL_GetLPLLMode PNLArgs;
    PNLArgs.u8Return = E_PNL_MODE_SINGLE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETLPLLMODE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_MODE_SINGLE;
    }
    else
    {
        return PNLArgs.u8Return;
    }
}

static MS_U8 _getPanel_LPLL_Type(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_LINK_TTL;
        }
    }

    stPNL_Get_LPLL_Type PNLArgs;
    PNLArgs.u8Return = LINK_TTL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GET_LPLL_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_LINK_TTL;
    }
    else
    {
        return (PNL_EX_LINK_TYPE)PNLArgs.u8Return;
    }
}

static MS_U8 _getPanelHSyncWidth(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_ucPanelHSyncWidth;
    }
}

static MS_U8 _getPanelHSyncBackPorch(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_ucPanelHSyncBackPorch;
    }
}

static MS_U8 _getPanelVSyncBackPorch(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return stPanelData.m_ucPanelVBackPorch;
    }
}

static MS_U16 _getPanelOnTiming(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_POWER_TIMING_SEQ seq)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelOnTiming PNLArgs;
    PNLArgs.seq = (APIPNL_POWER_TIMING_SEQ)seq;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELONTIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
}

static MS_U16 _getPanelOffTiming(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_POWER_TIMING_SEQ seq)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelOffTiming PNLArgs;
    PNLArgs.seq = (APIPNL_POWER_TIMING_SEQ)seq;
    PNLArgs.u16Return = 0;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELOFFTIMING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u16Return;
    }
}

static MS_U8 _getPanelDimCTRL(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_DIMMING_CTRL dim_type)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return 0;
        }
    }

    stPNL_GetPanelDimCtrl PNLArgs;
    PNLArgs.dim_type = (APIPNL_DIMMING_CTRL)dim_type;
    PNLArgs.u8Return = 0;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDIMCTRL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return 0;
    }
    else
    {
        return PNLArgs.u8Return;
    }
}

static const char* _getPanelName(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return NULL;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return stPanelData.m_pPanelName;
    }
}

static PNL_EX_ASPECT_RATIO _getPanel_ARC(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_ASPECT_RATIO_4_3;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_ASPECT_RATIO_4_3;
    }
    else
    {
        return ((PNL_EX_ASPECT_RATIO)stPanelData.m_ucPanelAspectRatio);
    }
}

static MS_U8** _getGammaTbl(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return NULL;
        }
    }

    stPNL_GetAllGammaTbl PNLArgs;
    PNLArgs.ppu8Return = NULL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETALLGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return PNLArgs.ppu8Return;
    }
}

static PNL_EX_OUT_TIMING_MODE _getOutTimingMode(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL
)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_CHG_DCLK;
        }
    }

    PanelType stPanelData;
    stPNL_GetPanelData PNLArgs;
    memset(&stPanelData, 0, sizeof(PanelType));
    PNLArgs.pstPNLData = &stPanelData;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETPANELDATA, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_CHG_DCLK;
    }
    else
    {
        return ((PNL_EX_OUT_TIMING_MODE)stPanelData.m_ucOutTimingMode);
    }
}

static MS_BOOL _EnablePanel(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bPanelOn)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_EnablePanel PNLArgs;
    PNLArgs.bPanelOn = bPanelOn;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_ENABLEPANEL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

static MS_BOOL _SetGammaTbl(const PNL_DeviceId *pPNL_DeviceId,
                                        PNL_EX_GAMMA_TYPE eGammaType,
                                        MS_U8* pu8GammaTab[3],
                                        PNL_EX_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetGammaTbl PNLArgs;
    PNLArgs.eGammaType = (APIPNL_GAMMA_TYPE)eGammaType;
    PNLArgs.Gamma_Map_Mode = (APIPNL_GAMMA_MAPPEING_MODE)Gamma_Map_Mode;
    PNLArgs.bReturn = FALSE;
    PNLArgs.pu8GammaTab[0] = pu8GammaTab[0];
    PNLArgs.pu8GammaTab[1] = pu8GammaTab[1];
    PNLArgs.pu8GammaTab[2] = pu8GammaTab[2];
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

static MS_BOOL _GetGammaTbl( const PNL_DeviceId *pPNL_DeviceId,
                         PNL_EX_GAMMA_TYPE eGammaType,
                         MS_U8* pu8GammaTab[3],
                         PNL_EX_GAMMA_MAPPEING_MODE Gamma_Map_Mode)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetGammaTbl PNLArgs;
    PNLArgs.eGammaType = (APIPNL_GAMMA_TYPE)eGammaType;
    PNLArgs.Gamma_Map_Mode = (APIPNL_GAMMA_MAPPEING_MODE)Gamma_Map_Mode;
    PNLArgs.bReturn = FALSE;
    PNLArgs.pu8GammaTab[0] = pu8GammaTab[0];
    PNLArgs.pu8GammaTab[1] = pu8GammaTab[1];
    PNLArgs.pu8GammaTab[2] = pu8GammaTab[2];
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETGAMMATBL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

static MS_BOOL _SetGammaValue(const PNL_DeviceId *pPNL_DeviceId, MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
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
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETGAMMAVALUE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

XC_PNL_EX_OBJ g_IPanelEx =
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
    .SetSSC          = _SetSSC,
    .Enable          = _EnablePanel,
    .SetGammaTbl     = _SetGammaTbl,
    .GammaTab        = _getGammaTbl,
    .Dump            = _MApi_XC_DumpPanelData,
    .DimCtrl         = _getPanelDimCTRL,
    .OnTiming        = _getPanelOnTiming,
    .OffTiming       = _getPanelOffTiming,
    //
    // Custimized methods, can be provided by clinets.
    //
    .TurnBackLightOn = _EmptyMethod,
    .SetGammaValue   = _SetGammaValue,
    .GetGammaTbl     = _GetGammaTbl,
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
PNL_EX_Result MApi_PNL_EX_GetLibVer(const MSIF_Version **ppVersion)
{
    if(pu32PNLInst == NULL)
    {
        if(UtopiaOpen(MODULE_PNL, &pu32PNLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_GetLibVer PNLArgs;
    PNLArgs.ppVersion = ppVersion;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(pu32PNLInst, E_PNL_CMD_GETLIBVER, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_FAIL;
    }
    else
    {
        return (PNL_EX_Result)PNLArgs.eResult;
    }
}

const PNL_EX_ApiInfo * MApi_PNL_EX_GetInfo(const PNL_DeviceId *pPNL_DeviceId)          ///< Get info from driver
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return NULL;
        }
    }

    stPNL_GetInfo PNLArgs;
    PNLArgs.pApiInfo = NULL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETINFO, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return NULL;
    }
    else
    {
        return (PNL_EX_ApiInfo*)PNLArgs.pApiInfo;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief Extended API of getting current panel status
/// @param[in] pPnlExtStatus @ref PNL_ApiExtStatus
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_EX_GetStatusEx(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_ApiExtStatus *pPnlExtStatus)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetStatusEx PNLArgs;
    PNLArgs.pPnlExtStatus = (PNL_ApiExtStatus*)pPnlExtStatus;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETSTATUSEX, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Get current panel status
/// @param[in] pPnlStatus @ref PNL_ApiStatus
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_EX_GetStatus(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_ApiStatus *pPnlStatus)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetStatus PNLArgs;
    PNLArgs.pPnlStatus = (PNL_ApiStatus*)pPnlStatus;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETSTATUS, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

/// Set debug level (without Mutex protect)
MS_BOOL MApi_PNL_EX_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if(pu32PNLInst == NULL)
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
}

MS_BOOL MApi_PNL_EX_IOMapBaseInit(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_IOMapBaseInit PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_IOMAPBASEINIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

//////////////////////////////////////////////////////////////////////
/**
*
* @brief  Give some options for panel init.
*
* @param[in] pSelPanelType  a given panel type choosed by user.
*
*/
MS_BOOL  MApi_PNL_EX_PreInit(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_PREINIT_OPTIONS eInitParam)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_PreInit PNLArgs;
    PNLArgs.eInitParam = (E_PNL_PREINIT_OPTIONS)eInitParam;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_PREINIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

//////////////////////////////////////////////////////////////////////
/**
*
* @brief  Initialize the global panel object with a given PanelType
*
* @param[in] pSelPanelType  a given panel type choosed by user.
*
*/
MS_BOOL MApi_PNL_EX_Init(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_PanelType *pSelPanelType/* <in > */)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Init PNLArgs;
    PNLArgs.pSelPanelType = (PanelType*)pSelPanelType;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_INIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////
MS_BOOL MApi_PNL_EX_Init_Ex(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_PanelType *pSelPanelType/* <in > */, MSIF_Version LIBVER)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Init_Ex PNLArgs;
    PNLArgs.pSelPanelType = (PanelType*)pSelPanelType;
    PNLArgs.LIBVER = LIBVER;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_INIT_EX, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

PNL_EX_Result MApi_PNL_EX_GetConfig(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_PanelType *pSelPanelType)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }
    stPNL_GetConfig PNLArgs;
    PNLArgs.pSelPanelType = (PanelType*)pSelPanelType;
    PNLArgs.eReturn = E_APIPNL_FAIL;

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GET_CONFIG, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_FAIL;
    }
    else
    {
        return (PNL_EX_Result)PNLArgs.eReturn;
    }
}

MS_BOOL MApi_PNL_EX_Control_Out_Swing(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16Swing_Level)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Control_Out_Swing PNLArgs;
    PNLArgs.u16Swing_Level = u16Swing_Level;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_CONTROL_OUT_SWING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

MS_BOOL MApi_PNL_EX_ForceSetPanelDCLK(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16PanelDCLK ,MS_BOOL bSetDCLKEnable )
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ForceSetPanelDCLK PNLArgs;
    PNLArgs.u16PanelDCLK = u16PanelDCLK;
    PNLArgs.bSetDCLKEnable = bSetDCLKEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_FORCESETPANELDCLK, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

MS_BOOL MApi_PNL_EX_ForceSetPanelHStart(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16PanelHStart ,MS_BOOL bSetHStartEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ForceSetPanelHStart PNLArgs;
    PNLArgs.u16PanelHStart = u16PanelHStart;
    PNLArgs.bSetHStartEnable = bSetHStartEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_FORCESETPANELHSTART, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

void MApi_PNL_EX_SetOutput(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_OUTPUT_MODE eOutputMode)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetOutput PNLArgs;
    PNLArgs.eOutputMode = (APIPNL_OUTPUT_MODE)eOutputMode;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETOUTPUT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

MS_BOOL MApi_PNL_EX_ChangePanelType(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_PanelType *pSelPanelType)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_ChangePanelType PNLArgs;
    PNLArgs.pSelPanelType = (PanelType*)pSelPanelType;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_CHANGEPANELTYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

MS_BOOL MApi_PNL_EX_TCONMAP_DumpTable(const PNL_DeviceId *pPNL_DeviceId, MS_U8 *pTCONTable, MS_U8 u8Tcontype)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_TCONMAP_DumpTable PNLArgs;
    PNLArgs.pTCONTable = pTCONTable;
    PNLArgs.u8Tcontype = u8Tcontype;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_TCONMAP_DUMPTABLE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

MS_BOOL MApi_PNL_EX_TCONMAP_Power_Sequence(const PNL_DeviceId *pPNL_DeviceId, MS_U8 *pTCONTable, MS_BOOL bEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_TCONMAP_Power_Sequence PNLArgs;
    PNLArgs.pTCONTable = pTCONTable;
    PNLArgs.bEnable = bEnable;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_TCONMAP_POWER_SEQUENCE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

void MApi_PNL_EX_SetOutputPattern(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
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
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SET_OUTPUT_PATTERN, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}
//-------------------------------------------------------------------------------------------------
/// Setup TCON Count Reset ON/OFF
/// @param  bEnable      \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MApi_PNL_EX_TCON_Count_Reset ( const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bEnable )
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_TCON_Count_Reset PNLArgs;
    PNLArgs.bEnable = bEnable;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_TCON_COUNT_RESET, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

void MApi_PNL_EX_TCON_Init( const PNL_DeviceId *pPNL_DeviceId )
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_TCON_INIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

MS_BOOL MApi_PNL_EX_GetDstInfo(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    if (_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_GetDstInfo PNLArgs;
    PNLArgs.pDstInfo = (MS_PNL_DST_DispInfo*)pDstInfo;
    PNLArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GETDSTINFO, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}


void MApi_PNL_EX_Mod_Calibration_Setting(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_ModCaliInfo *pstModCaliInfo)
{
	if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

	if(pstModCaliInfo == NULL)
	{
		printf("MOD Calibration config setting is NULL\n");
		return;
	}

    stPNL_Mod_Calibration_Setting PNLArgs;
    PNLArgs.pstModCaliInfo = (MS_PNL_ModCaliInfo*)pstModCaliInfo;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_MOD_CALIBRATION_SETTING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

MS_BOOL MApi_PNL_EX_Mod_Do_Calibration(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Mod_Do_Calibration PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_MOD_DO_CALIBRATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

void MApi_PNL_EX_BD_LVDS_Output_Type(const PNL_DeviceId *pPNL_DeviceId, MS_U16 Type)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_BD_LVDS_Output_Type PNLArgs;
    PNLArgs.Type = Type;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_BD_LVDS_OUTPUT_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

int MApi_PNL_EX_SetSSC_En(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_SetSSC_En PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETSSC_EN, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
}

int MApi_PNL_EX_SetSSC_Fmodulation(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16Fmodulation)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_SetSSC_Fmodulation PNLArgs;
    PNLArgs.u16Fmodulation = u16Fmodulation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETSSC_FMODULATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
}

int MApi_PNL_EX_SetSSC_Rdeviation(const PNL_DeviceId *pPNL_DeviceId, MS_U16 u16Rdeviation)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_SetSSC_Rdeviation PNLArgs;
    PNLArgs.u16Rdeviation = u16Rdeviation;
    PNLArgs.eReturn = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETSSC_RDEVIATION, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_APIPNL_FAIL;
    }
    else
    {
        return PNLArgs.eReturn;
    }
}

void MApi_PNL_EX_SetLPLLTypeExt(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_LINK_EXT_TYPE eLPLL_TypeExt)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetLPLLTypeExt PNLArgs;
    PNLArgs.eLPLL_TypeExt = (APIPNL_LINK_EXT_TYPE)eLPLL_TypeExt;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETLPLLTYPEEXT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

void MApi_PNL_EX_Init_MISC(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_MISC ePNL_MISC)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_Init_MISC PNLArgs;
    PNLArgs.ePNL_MISC = (APIPNL_MISC)ePNL_MISC;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_INIT_MISC, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

void MApi_PNL_EX_MOD_OutputConfig_User(const PNL_DeviceId *pPNL_DeviceId, MS_U32 u32OutputCFG0_7, MS_U32 u32OutputCFG8_15, MS_U32 u32OutputCFG16_21)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_OutputConfig_User PNLArgs;
    PNLArgs.u32OutputCFG0_7 = u32OutputCFG0_7;
    PNLArgs.u32OutputCFG8_15 = u32OutputCFG8_15;
    PNLArgs.u32OutputCFG16_21 = u32OutputCFG16_21;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_MOD_OUTPUTCONFIG_USER, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Flag to bypass timing change in MApi_PNL_Init()
/// @param  bFlag               \b IN: TRUE: initialize XC by skipping SW reset; FALSE: initialize XC in normal case
/// @return APIPNL_Result
//-------------------------------------------------------------------------------------------------
PNL_EX_Result MApi_PNL_EX_SkipTimingChange(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bFlag)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_SkipTimingChange PNLArgs;
    PNLArgs.bFlag = bFlag;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SKIPTIMINGCHANGE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_FAIL;
    }
    else
    {
        return (PNL_EX_Result)PNLArgs.eResult;
    }
}

void MApi_PNL_EX_HWLVDSReservedtoLRFlag(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_HW_LVDSResInfo lvdsresinfo)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_HWLVDSReservedtoLRFlag PNLArgs;
    PNLArgs.lvdsresinfo.u32pair = lvdsresinfo.u32pair;
    PNLArgs.lvdsresinfo.u16channel = lvdsresinfo.u16channel;
    PNLArgs.lvdsresinfo.bEnable = lvdsresinfo.bEnable;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
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
PNL_EX_Result MApi_PNL_EX_OverDriver_Init(const PNL_DeviceId *pPNL_DeviceId, PNL_EX_OD_INITDATA *pPNL_ODInitData, MS_U32 u32ODInitDataLen)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_OverDriver_Init PNLArgs;
    PNLArgs.pPNL_ODInitData = (MS_PNL_OD_INITDATA*)pPNL_ODInitData;
    PNLArgs.u32ODInitDataLen = u32ODInitDataLen;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_OVERDRIVER_INIT, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_FAIL;
    }
    else
    {
        return (PNL_EX_Result)PNLArgs.eResult;
    }
}

//-------------------------------------------------------------------------------------------------
/// OverDrive Enable
/// @param  bEnable               \b IN: TRUE: Enable OverDrive; FALSE: Disable OverDrive
/// @return E_APIPNL_OK or E_APIPNL_FAIL
//-------------------------------------------------------------------------------------------------
PNL_EX_Result MApi_PNL_EX_OverDriver_Enable(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bEnable)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_OverDriver_Enable PNLArgs;
    PNLArgs.bEnable = bEnable;
    PNLArgs.eResult = E_APIPNL_FAIL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_OVERDRIVER_ENABLE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_FAIL;
    }
    else
    {
        return (PNL_EX_Result)PNLArgs.eResult;
    }
}


void MApi_PNL_EX_MOD_PVDD_Power_Setting(const PNL_DeviceId *pPNL_DeviceId, MS_BOOL bIs2p5)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_MOD_PVDD_Power_Setting PNLArgs;
    PNLArgs.bIs2p5 = bIs2p5;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_MOD_PVDD_POWER_SETTING, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
///-obosolte!! use MApi_PNL_Control_Out_Swing instead
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_EX_SetDiffSwingLevel(const PNL_DeviceId *pPNL_DeviceId, MS_U8 u8Swing_Level)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_SetDiffSwingLevel PNLArgs;
    PNLArgs.u8Swing_Level = u8Swing_Level;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETDIFFSWINGLEVEL, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get real LPLL type from driver
/// @return MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_Pnl_EX_Get_LPLL_Type(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return E_PNL_EX_FAIL;
        }
    }

    stPNL_Get_LPLL_Type PNLArgs;
    PNLArgs.u8Return = LINK_TTL;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GET_LPLL_TYPE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return E_PNL_EX_LINK_TTL;
    }
    else
    {
        return (PNL_EX_LINK_TYPE)PNLArgs.u8Return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get TCON capability
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_PNL_EX_Get_TCON_Capability(const PNL_DeviceId *pPNL_DeviceId)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return FALSE;
        }
    }

    stPNL_Get_TCON_Capability PNLArgs;
    PNLArgs.bReturn = FALSE;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_GET_TCON_CAPABILITY, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
        return FALSE;
    }
    else
    {
        return PNLArgs.bReturn;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set FRC MOD pair swap
/// @param  bEnable               \b IN: u8Polarity, (d:c:b:a)=([15:14],[13:12],[11:10],[9:8]) => (10,00,11,01), => (2,0,3,1)
//-------------------------------------------------------------------------------------------------
void MApi_PNL_EX_SetPairSwap(const PNL_DeviceId *pPNL_DeviceId, MS_U32 u32Polarity)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_SetPairSwap PNLArgs;
    PNLArgs.u32Polarity = u32Polarity;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_SETPAIRSWAP, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
/// Cal Ext LPLL Set by DCLK
/// @param  ldHz               \b IN: ldHz = Htt*Vtt*Vfreq
//-------------------------------------------------------------------------------------------------
void MApi_PNL_EX_CalExtLPLLSETbyDClk(const PNL_DeviceId *pPNL_DeviceId, MS_U32 ldHz)
{
    if(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id) == NULL)
    {
        MS_U32 u32DevideID = pPNL_DeviceId->u32Id;
        if(UtopiaOpen(MODULE_PNL, _PNL_SELECT_INSTANCE_P(pPNL_DeviceId->u32Id), 0, &u32DevideID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PNL failed\n");
            return;
        }
    }

    stPNL_CalExtLPLLSETbyDClk PNLArgs;
    PNLArgs.ldHz = ldHz;
    if(UtopiaIoctl(_PNL_SELECT_INSTANCE(pPNL_DeviceId->u32Id), E_PNL_CMD_CALEXTLPLLSETBYDCLK, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain PNL engine fail\n");
    }
}

//} // Marked it by coverity_0553

#endif
