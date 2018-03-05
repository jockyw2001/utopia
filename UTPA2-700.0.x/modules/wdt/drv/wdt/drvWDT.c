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
/// file    drvWDT.c
/// @brief  Piu Watch Dog Timer Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsDevice.h"
#include "MsVersion.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvMMIO.h"
#include "drvWDT.h"
#include "halWDT.h"
#include "regWDT.h"
#include "MsOS.h"

#include "ULog.h"

#define WDT_DBG_FUNC()            if (_geDbfLevel >= E_WDT_DBGLV_ALL) \
                                           {MS_DEBUG_MSG(ULOGI("WDT", "\t====   %s   ====\n", __FUNCTION__));}
#define WDT_DBG_INFO(x, args...)  if (_geDbfLevel >= E_WDT_DBGLV_INFO ) \
                                           {MS_DEBUG_MSG(ULOGI("WDT", x, ##args));}
#define WDT_DBG_ERR(x, args...)   if (_geDbfLevel >= E_WDT_DBGLV_ERR_ONLY) \
										   {MS_DEBUG_MSG(ULOGE("WDT", x, ##args));}

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
#ifdef CONFIG_MSTAR_UTPA2K_WDT_ENABLE
#define WDT_UTOPIA20 (1)
#else
#define WDT_UTOPIA20 (0)
#endif

#ifdef CONFIG_MSTAR_WDT_KERNEL_MODE
#define WDT_KERNEL_MODE (1)
#else
#define WDT_KERNEL_MODE (0)
#endif

static tmr_interrupt tTmrTbl[MAX_TIMER_NUM];	/* Table of timers managed by this module		*/
static MS_BOOL  _gbInitWDT = FALSE;
static WDT_DbgLv _geDbfLevel = E_WDT_DBGLV_ERR_ONLY;
static MSIF_Version _drv_wdt_version = {
    .DDI = { WDT_DRV_VERSION },
};

#if (WDT_UTOPIA20)
MS_BOOL bU20WdtOpened = FALSE;
void* pInstantWdt;
void* pAttributeWdt;
#include "drvWDT_private.h"
#include "utopia.h"
MS_BOOL _MDrv_WDT_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	if (FALSE == bU20WdtOpened)
	{
#if (WDT_KERNEL_MODE)
		if(UtopiaOpen(MODULE_WDT|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
#else
		if(UtopiaOpen(MODULE_WDT, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
#endif
		{
			WDT_DBG_ERR("Open WDT fail\n");
			return FALSE;
		}
		bU20WdtOpened= TRUE;
	}
	return TRUE;
}
#endif

inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_WDT_GetLibVer
/// @brief \b Function \b Description: Show the WDT driver version
/// @param <IN>        \b None:
/// @param <OUT>       \b **pVerString : output WDT driver version
/// @param <RET>       \b eRETCODE :
/// @param <GLOBAL>    \b VER: Library version string
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_GetLibVer(const MSIF_Version **ppVersion)
{
    WDT_DBG_FUNC();

    if (!ppVersion)
        return E_WDT_FAIL;

    *ppVersion = &_drv_wdt_version;
    return E_WDT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsEnable
/// @brief \b Function  \b Description: check if WDT is Enable
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_IsEnable_U2K(void)
{
    WDT_DBG_FUNC();

    _gbInitWDT = HAL_WDT_IsEnable();

    if (!_gbInitWDT)
        WDT_DBG_ERR("Call MDrv_WDT_Start first!\n");
    return (WDT_Result)_gbInitWDT;
}

WDT_Result MDrv_WDT_IsEnable(void)
{
#if (WDT_UTOPIA20)
	WDT_Result enstatus;
	PWDT_IS_EN_PARAM pWdtIsEnParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtIsEnParam = (PWDT_IS_EN_PARAM)malloc(sizeof(WDT_IS_EN_PARAM));
	pWdtIsEnParam->status=0;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_IsEnable,(MS_U32*)pWdtIsEnParam);
	enstatus = pWdtIsEnParam->status;
	free(pWdtIsEnParam);
	return (WDT_Result)enstatus;
#else
	return MDrv_WDT_IsEnable_U2K();
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Init
/// @brief \b Function  \b Description: Init and Start WDT
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Init_U2K(WDT_DbgLv eLevel)
{
    MS_VIRT virtBaseAddr;
    MS_PHY u32BaseSize = 0; /* No use */

    _geDbfLevel = eLevel;
    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);
    /*  This is not really necessary, WDT is enabled after MCU reset
        *   but it is important to get the IOMAP Base to access RIU.
        */
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return E_WDT_FAIL;
        }

        HAL_WDT_SetIOMapBase(virtBaseAddr);
        HAL_WDT_Start();

        _gbInitWDT = TRUE;
        return E_WDT_OK;
    }
    else
    {
        WDT_DBG_ERR("WDT had initial!\n");
        return E_WDT_OK;
    }
}

WDT_Result MDrv_WDT_Init(WDT_DbgLv eLevel)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	PWDT_INIT_PARAM pWdtInitParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtInitParam = (PWDT_INIT_PARAM)malloc(sizeof(WDT_INIT_PARAM));
	pWdtInitParam->eLevel=eLevel;
	u32Ret = UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_Init,(MS_U32*)pWdtInitParam);
	free(pWdtInitParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_WDT_Init_U2K(eLevel);
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Stop
/// @brief \b Function  \b Description: Init WDT Stop
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Stop_U2K(WDT_DbgLv eLevel)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_Stop();

    return eWDTRe;
}

WDT_Result MDrv_WDT_Stop(WDT_DbgLv eLevel)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	PWDT_STOP_PARAM pWdtStopParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtStopParam = (PWDT_STOP_PARAM)malloc(sizeof(WDT_STOP_PARAM));
	pWdtStopParam->eLevel=eLevel;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_Stop,(MS_U32*)pWdtStopParam);
	free(pWdtStopParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_WDT_Stop_U2K(eLevel);
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Clear
/// @brief \b Function  \b Description: clear WDT.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Clear_U2K(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_Clear();

    return eWDTRe;
}

WDT_Result MDrv_WDT_Clear(void)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	u32Ret = UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_Clear,(MS_U32*)NULL);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_Clear_U2K();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_ClearRstFlag
/// @brief \b Function  \b Description: clear WDT reset flag.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_ClearRstFlag_U2K(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_ClearRstFlag();

    return eWDTRe;
}

WDT_Result MDrv_WDT_ClearRstFlag(void)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	u32Ret = UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_ClearRstFlag,(MS_U32*)NULL);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_ClearRstFlag_U2K();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsReset
/// @brief \b Function  \b Description: check if WDT is reset
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////

WDT_Result MDrv_WDT_IsReset_U2K(void)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_IsReset();

    return eWDTRe;
}

WDT_Result MDrv_WDT_IsReset(void)
{
#if (WDT_UTOPIA20)
	WDT_Result StatusVal;
	PWDT_IS_RST_PARAM pWdtIsRstParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtIsRstParam = (PWDT_IS_RST_PARAM)malloc(sizeof(WDT_IS_RST_PARAM));
	pWdtIsRstParam->status=0;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_IsReset,(MS_U32*)pWdtIsRstParam);
	StatusVal = pWdtIsRstParam->status;
	free(pWdtIsRstParam);
	return (WDT_Result)StatusVal;
#else
	return MDrv_WDT_IsReset_U2K();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_U2K(WDT_DbgLv eLevel, MS_U16 sec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer(sec);

    return eWDTRe;
}

WDT_Result MDrv_WDT_SetTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
#if (WDT_UTOPIA20)
	WDT_Result u32Ret;
	PWDT_SET_S_PARAM pWdtSetSecParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtSetSecParam = (PWDT_SET_S_PARAM)malloc(sizeof(WDT_SET_S_PARAM));
	pWdtSetSecParam->eLevel=eLevel;
	pWdtSetSecParam->sec=sec;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_SetTimer,(MS_U32*)pWdtSetSecParam);
	free(pWdtSetSecParam);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_SetTimer_U2K(eLevel,sec);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: msec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_ms_U2K(WDT_DbgLv eLevel, MS_U16 msec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_ms(msec);

    return eWDTRe;
}

WDT_Result MDrv_WDT_SetTimer_ms(WDT_DbgLv eLevel, MS_U16 msec)
{
#if (WDT_UTOPIA20)
	WDT_Result u32Ret;
	PWDT_SET_MS_PARAM pWdtSetMsParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtSetMsParam = (PWDT_SET_MS_PARAM)malloc(sizeof(WDT_SET_MS_PARAM));
	pWdtSetMsParam->eLevel=eLevel;
	pWdtSetMsParam->msec=msec;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_SetTimer_ms,(MS_U32*)pWdtSetMsParam);
	free(pWdtSetMsParam);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_SetTimer_ms_U2K(eLevel,msec);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: usec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_us_U2K(WDT_DbgLv eLevel, MS_U16 usec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_us(usec);

    return eWDTRe;
}

WDT_Result MDrv_WDT_SetTimer_us(WDT_DbgLv eLevel, MS_U16 usec)
{
#if (WDT_UTOPIA20)
	WDT_Result u32Ret;
	PWDT_SET_US_PARAM pWdtSetUsParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtSetUsParam = (PWDT_SET_US_PARAM)malloc(sizeof(WDT_SET_US_PARAM));
	pWdtSetUsParam->eLevel=eLevel;
	pWdtSetUsParam->usec=usec;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_SetTimer_us,(MS_U32*)pWdtSetUsParam);
	free(pWdtSetUsParam);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_SetTimer_us_U2K(eLevel,usec);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetIntTimer
/// @brief \b Function  \b Description: Set INT Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetIntTimer_U2K(WDT_DbgLv eLevel, MS_U16 sec)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return E_WDT_FAIL;
    }

    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetIntTimer(sec);

    return eWDTRe;
}

WDT_Result MDrv_WDT_SetIntTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
#if (WDT_UTOPIA20)
	WDT_Result u32Ret;
	PWDT_SETTIMER_PARAM pWdtSetTimerParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0;
	pWdtSetTimerParam = (PWDT_SETTIMER_PARAM)malloc(sizeof(WDT_SETTIMER_PARAM));
	pWdtSetTimerParam->eLevel=eLevel;
	pWdtSetTimerParam->sec=sec;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_WDT_SetIntTimer,(MS_U32*)pWdtSetTimerParam);
	free(pWdtSetTimerParam);
	return (WDT_Result)u32Ret;
#else
	return MDrv_WDT_SetIntTimer_U2K(eLevel,sec);
#endif
}

//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_WDT_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;
	
	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitWDT = FALSE;
		MDrv_TIMER_Exit();
		u16Return = 2;				// suspend OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
			MDrv_WDT_Init(E_WDT_DBGLV_ERR_ONLY);
			MDrv_TIMER_Init();
			_prev_u16PowerState = u16PowerState;
            u16Return = 1;		// resume OK
		}
		else
		{
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u16Return = 3;		// resume failed		
		}
	}
	else
	{
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
	}
	
	return u16Return;
}
// PIU TIMER
inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count)
{
	if ( eTimer == E_TIMER_0 )
	{
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_TRIGGER);
	}
	else
	{
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_TRIGGER);
	}
	while(!MDrv_TIMER_HitMaxMatch(eTimer));
}

#if !defined(CONFIG_FRC)
static void MDrv_TIMER_Stop(E_PIU_Timer eTimer)
{
    if ( eTimer == E_TIMER_0 )
    {   
        HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_0_MATCH_REG,HAL_WDT_ReadByte(TIMER_0_MATCH_REG)|(TIMER_MATCH));
    }
    else
    {   
        HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_1_MATCH_REG,HAL_WDT_ReadByte(TIMER_1_MATCH_REG)|(TIMER_MATCH));
    }
}
#endif

void MDrv_TIMER_Count_U2K(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    tmr_interrupt *ptmr;

    ptmr = &tTmrTbl[eTimer];
	ptmr->bTmrEn = bEnable;

	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
	}
}

void MDrv_TIMER_Count(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
#if (WDT_UTOPIA20)
	WDT_Result u32Ret;
	PTIMER_COUNT_PARAM pTimerCntrParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_Count Open Fail \n");
	pTimerCntrParam = (PTIMER_COUNT_PARAM)malloc(sizeof(TIMER_COUNT_PARAM));
	pTimerCntrParam->eTimer=eTimer;
	pTimerCntrParam->bEnable=bEnable;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_Count,(MS_U32*)pTimerCntrParam);
	free(pTimerCntrParam);
#else
	MDrv_TIMER_Count_U2K(eTimer,bEnable);
#endif
}

void MDrv_TIMER_INT_U2K(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)&(~TIMER_INTEN));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)&(~TIMER_INTEN));
	}
}

void MDrv_TIMER_INT(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
#if (WDT_UTOPIA20)
	PTIMER_INT_PARAM pTimerInitParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("Timer INT Open Fail \n");
	pTimerInitParam = (PTIMER_INT_PARAM)malloc(sizeof(TIMER_INT_PARAM));
	pTimerInitParam->eTimer=eTimer;
	pTimerInitParam->bEnable=bEnable;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_Init,(MS_U32*)pTimerInitParam);
	free(pTimerInitParam);
#else
	MDrv_TIMER_INT_U2K(eTimer,bEnable);
#endif
}

MS_BOOL MDrv_TIMER_HitMaxMatch_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

	if ( eTimer == E_TIMER_0 )
	{
		return (HAL_WDT_ReadByte(TIMER_0_MATCH_REG)&TIMER_MATCH);
	}
	else
	{
		return (HAL_WDT_ReadByte(TIMER_1_MATCH_REG)&TIMER_MATCH);
	}
}

MS_BOOL MDrv_TIMER_HitMaxMatch(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_BOOL u32Ret;
	PTIMER_HIT_MAX_MATCH_PARAM pTimerHitMaxMatchParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return FALSE;
	pTimerHitMaxMatchParam = (PTIMER_HIT_MAX_MATCH_PARAM)malloc(sizeof(TIMER_HIT_MAX_MATCH_PARAM));
	pTimerHitMaxMatchParam->eTimer=eTimer;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_HitMaxMatch,(MS_U32*)pTimerHitMaxMatchParam);
	free(pTimerHitMaxMatchParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_TIMER_HitMaxMatch_U2K(eTimer);
#endif
}

void MDrv_TIMER_Rst_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	if ( eTimer == E_TIMER_0 )
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
	else
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
}

void MDrv_TIMER_Rst(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	PTIMER_RST_PARAM pTimerRstParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_Rst Open Fail \n");
	pTimerRstParam = (PTIMER_RST_PARAM)malloc(sizeof(TIMER_RST_PARAM));
	pTimerRstParam->eTimer=eTimer;
	UtopiaIoctl(pInstantWdt,  MDrv_CMD_TIMER_Rst,(MS_U32*)pTimerRstParam);
	free(pTimerRstParam);
#else
	MDrv_TIMER_Rst_U2K(eTimer);
#endif
}

void MDrv_TIMER_SetMaxMatch_U2K(E_PIU_Timer eTimer, MS_U32 u32MaxTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    tmr_interrupt *ptmr;

    ptmr = &tTmrTbl[eTimer];
	ptmr->u32TmrMax = u32MaxTimer;

	if ( eTimer == E_TIMER_0 )
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG, u32MaxTimer);
	else
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG, u32MaxTimer);
}

void MDrv_TIMER_SetMaxMatch(E_PIU_Timer eTimer, MS_U32 u32MaxTimer)
{
#if (WDT_UTOPIA20)
	PTIMER_SET_MAX_MATCH_PARAM pTimerSetMaxMatchParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_SetMaxMatch Open Fail \n");
	pTimerSetMaxMatchParam = (PTIMER_SET_MAX_MATCH_PARAM)malloc(sizeof(TIMER_SET_MAX_MATCH_PARAM));
	pTimerSetMaxMatchParam->eTimer=eTimer;
	pTimerSetMaxMatchParam->u32MaxTimer=u32MaxTimer;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_SetMaxMatch,(MS_U32*)pTimerSetMaxMatchParam);
	free(pTimerSetMaxMatchParam);
#else
	MDrv_TIMER_SetMaxMatch_U2K(eTimer,u32MaxTimer);
#endif
}

MS_U32 MDrv_TIMER_GetMaxMatch_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_MAX_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_MAX_REG));
}

MS_U32 MDrv_TIMER_GetMaxMatch(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_BOOL u32Ret;
	MS_U32 u32MaxMatchVal;
	PTIMER_GET_MAX_MATCH_PARAM pTimerGetMaxMatchParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0xFFFFFFFF;
	pTimerGetMaxMatchParam = (PTIMER_GET_MAX_MATCH_PARAM)malloc(sizeof(TIMER_GET_MAX_MATCH_PARAM));
	pTimerGetMaxMatchParam->eTimer=eTimer;
	pTimerGetMaxMatchParam->u32MaxMatchVal=0;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_GetMaxMatch,(MS_U32*)pTimerGetMaxMatchParam);
	if(u32Ret!= 0)
	{
		free(pTimerGetMaxMatchParam);
		return 0xFFFFFFFF;
	}
	u32MaxMatchVal = pTimerGetMaxMatchParam->u32MaxMatchVal;
	free(pTimerGetMaxMatchParam);
	return (MS_U32)u32MaxMatchVal;
#else
	return MDrv_TIMER_GetMaxMatch_U2K(eTimer);
#endif
}

MS_U32 MDrv_TIMER_GetCounter_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG));
}

MS_U32 MDrv_TIMER_GetCounter(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	MS_U32 u32cntval;
	PTIMER_GET_COUNTER_PARAM pTimerGetCntParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0xFFFFFFFF;
	pTimerGetCntParam = (PTIMER_GET_COUNTER_PARAM)malloc(sizeof(TIMER_GET_COUNTER_PARAM));
	pTimerGetCntParam->eTimer=eTimer;
	pTimerGetCntParam->u32Cnt=0;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_GetCounter,(MS_U32*)pTimerGetCntParam);
	if(u32Ret!= 0)
	{
		free(pTimerGetCntParam);
		return 0xFFFFFFFF;
	}
	u32cntval = pTimerGetCntParam->u32Cnt;
	free(pTimerGetCntParam);
	return (MS_U32)u32cntval;
#else
	return MDrv_TIMER_GetCounter_U2K(eTimer);
#endif
}

MS_U32 MDrv_TIMER_GetSecond_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_HZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_HZ);
}

MS_U32 MDrv_TIMER_GetSecond(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	MS_U32 u32SecVal;
	PTIMER_GET_SEC_PARAM pTimerSecParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0xFFFFFFFF;
	pTimerSecParam = (PTIMER_GET_SEC_PARAM)malloc(sizeof(TIMER_GET_SEC_PARAM));
	pTimerSecParam->eTimer=eTimer;
	pTimerSecParam->u32Sec=0;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_GetSecond,(MS_U32*)pTimerSecParam);
	if(u32Ret!= 0)
	{
		free(pTimerSecParam);
		return 0xFFFFFFFF;
	}
	u32SecVal = pTimerSecParam->u32Sec;
	free(pTimerSecParam);
	return (MS_U32)u32SecVal;
#else
	return MDrv_TIMER_GetSecond_U2K(eTimer);
#endif
}

MS_U32 MDrv_TIMER_GetMs_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
}

MS_U32 MDrv_TIMER_GetMs(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	MS_U32 u32MsecVal;
	PTIMER_GET_MS_PARAM pTimerMSecParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0xFFFFFFFF;
	pTimerMSecParam = (PTIMER_GET_MS_PARAM)malloc(sizeof(TIMER_GET_MS_PARAM));
	pTimerMSecParam->eTimer=eTimer;
	pTimerMSecParam->u32Msec=0;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_GetMs,(MS_U32*)pTimerMSecParam);
	if(u32Ret!= 0)
	{
		free(pTimerMSecParam);
		return 0xFFFFFFFF;
	}
	u32MsecVal = pTimerMSecParam->u32Msec;
	free(pTimerMSecParam);
	return (MS_U32)u32MsecVal;
#else
	return MDrv_TIMER_GetMs_U2K(eTimer);
#endif
}

MS_U32 MDrv_TIMER_GetUs_U2K(E_PIU_Timer eTimer)
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return 0;
    }

	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
}

MS_U32 MDrv_TIMER_GetUs(E_PIU_Timer eTimer)
{
#if (WDT_UTOPIA20)
	MS_U32 u32Ret;
	MS_U32 u32USecVal;
	PTIMER_GET_US_PARAM pTimerUSecParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		return 0xFFFFFFFF;
	pTimerUSecParam = (PTIMER_GET_US_PARAM)malloc(sizeof(TIMER_GET_US_PARAM));
	pTimerUSecParam->eTimer=eTimer;
	pTimerUSecParam->u32Usec=0;
	u32Ret = UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_GetUs,(MS_U32*)pTimerUSecParam);
	if(u32Ret!= 0)
	{
		free(pTimerUSecParam);
		return 0xFFFFFFFF;
	}
	u32USecVal = pTimerUSecParam->u32Usec;
	free(pTimerUSecParam);
	return (MS_U32)u32USecVal;
#else
	return MDrv_TIMER_GetUs_U2K(eTimer);
#endif
}

void MDrv_TIMER_Delay_U2K(E_PIU_Timer eTimer, MS_U32 u32Second)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    MS_U32 u32Salt = HAL_WDTCycles(u32Second);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void MDrv_TIMER_Delay(E_PIU_Timer eTimer, MS_U32 u32Second)
{
#if (WDT_UTOPIA20)
	PTIMER_DELAY_PARAM pTimerDelayParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_Delay Open Fail \n");
	pTimerDelayParam = (PTIMER_DELAY_PARAM)malloc(sizeof(TIMER_DELAY_PARAM));
	pTimerDelayParam->eTimer=eTimer;
	pTimerDelayParam->u32Second=u32Second;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_Delay,(MS_U32*)pTimerDelayParam);
	free(pTimerDelayParam);
#else
	MDrv_TIMER_Delay_U2K(eTimer,u32Second);
#endif
}

void MDrv_TIMER_DelayMs_U2K(E_PIU_Timer eTimer, MS_U32 u32MS)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

	MS_U32 u32Salt = HAL_WDTCyclesM(u32MS);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void MDrv_TIMER_DelayMs(E_PIU_Timer eTimer, MS_U32 u32MS)
{
#if (WDT_UTOPIA20)
	PTIMER_DELAY_MS_PARAM pTimerDelayMsParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_DelayMs Open Fail \n");
	pTimerDelayMsParam = (PTIMER_DELAY_MS_PARAM)malloc(sizeof(TIMER_DELAY_MS_PARAM));
	pTimerDelayMsParam->eTimer=eTimer;
	pTimerDelayMsParam->u32ms=u32MS;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_DelayMs,(MS_U32*)pTimerDelayMsParam);
	free(pTimerDelayMsParam);
#else
	MDrv_TIMER_DelayMs_U2K(eTimer,u32MS);
#endif
}

void MDrv_TIMER_DelayUs_U2K(E_PIU_Timer eTimer, MS_U32 u32US)//@@@need restore status
{
    if (!_gbInitWDT)
    {
        WDT_DBG_ERR("%s is called before init\n", __FUNCTION__);
        return;
    }

    printf("TIMER_DelayUs unSupported!! \n");

    if (0)
    {
      MS_U32 u32Salt = HAL_WDTCyclesU(u32US);
      MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

      _MDrv_TIMER_Trigger(eTimer,u32Salt);
      MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
      MDrv_TIMER_Count(eTimer, ENABLE);
    }

}

void MDrv_TIMER_DelayUs(E_PIU_Timer eTimer, MS_U32 u32US)
{
#if (WDT_UTOPIA20)
	PTIMER_DELAY_US_PARAM pTimerDelayUsParam = NULL;
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("TIMER_DelayUs Open Fail \n");
	pTimerDelayUsParam = (PTIMER_DELAY_US_PARAM)malloc(sizeof(TIMER_DELAY_US_PARAM));
	pTimerDelayUsParam->eTimer=eTimer;
	pTimerDelayUsParam->u32us=u32US;
	UtopiaIoctl(pInstantWdt, MDrv_CMD_TIMER_DelayUs,(MS_U32*)pTimerDelayUsParam);
	free(pTimerDelayUsParam);
#else
	MDrv_TIMER_DelayUs_U2K(eTimer,u32US);
#endif
}

void MDrv_TIMER_Init_U2K(void)
{
    MS_VIRT virtBaseAddr;
    MS_PHY u32BaseSize = 0, idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    ptmr = &tTmrTbl[0];
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		ptmr++;
    }

    // Get Memory Base
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return;
        }

        HAL_WDT_SetIOMapBase(virtBaseAddr);

        _gbInitWDT = TRUE;
        return;
    }
    else
    {
        WDT_DBG_ERR("PIU timer had initial!\n");
        return;
    }
}

void MDrv_TIMER_Init(void)
{
#if (WDT_UTOPIA20)
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("Timer Init Fail \n");
	UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_Initial,(MS_U32*)NULL);
#else
	return MDrv_TIMER_Init_U2K();
#endif
}

#if !defined(CONFIG_FRC)
static void DRV_TIMER0_PIU_Isr(void)
{
	tmr_interrupt *ptmr;

	ptmr = &tTmrTbl[E_TIMER_0];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_0))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_0);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_0);
	}
}
static void DRV_TIMER1_PIU_Isr(void)
{
 	tmr_interrupt *ptmr;
    
	ptmr = &tTmrTbl[E_TIMER_1];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_1))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_1);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_1);
	}
}
#endif

void MDrv_TIMER_CfgFnct(E_PIU_Timer eTimer, void (*fnct)(void *, void *, void *), void *arg0, void *arg1, void *arg2 )
{
	tmr_interrupt *ptmr;
    
	if( fnct )
	{
		if( eTimer < MAX_TIMER_NUM )
		{
			ptmr = &tTmrTbl[eTimer];
			//OS_ENTER_CRITICAL();
			ptmr->TmrFnct		= fnct;
			ptmr->TmrFnctArg0 	= arg0;
			ptmr->TmrFnctArg1 	= arg1;
			ptmr->TmrFnctArg2	= arg2;
			//OS_EXIT_CRITICAL();
			MDrv_TIMER_INT(eTimer, 1);
#if !defined(CONFIG_FRC)
			if( eTimer )
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER1, (InterruptCb)DRV_TIMER1_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER1);
			}
			else
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER0, (InterruptCb)DRV_TIMER0_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER0);
			}
#endif
		}
	}
}

void MDrv_TIMER_Exit_U2K(void)
{
    MS_U32 idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
        ptmr = &tTmrTbl[idx];
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		MDrv_TIMER_Count((E_PIU_Timer) idx, 0);
		MDrv_TIMER_SetMaxMatch((E_PIU_Timer) idx, ptmr->u32TmrMax);
		MDrv_TIMER_INT((E_PIU_Timer) idx, 0);
    }
}

void MDrv_TIMER_Exit(void)
{
#if (WDT_UTOPIA20)
	if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		printf("Timer Exit Open Fail \n");
	UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_Exit,(MS_U32*)NULL);
#else
	return MDrv_TIMER_Exit_U2K();
#endif
}

WDT_Result MDrv_WDT_GetConfig(WDT_DbgLv* eLevel)
{
	MS_U32 u32Ret=0;
#if (WDT_UTOPIA20)
	  if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
		  return 0;
	  u32Ret = UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_GetConfig,eLevel);
#else
#endif
  return (WDT_Result)u32Ret;
}