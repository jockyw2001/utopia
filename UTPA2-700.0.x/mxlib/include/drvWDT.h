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
/// @file   drvWDT.h
/// @brief  Piu Watch Dog Timer Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! 
	\defgroup G_WDT WDT interface
    \ingroup  G_PERIPHERAL

	WDT(WatchDog) is a program to moniter every registered programs, and will restart these programs while thy are up and down.

	<b> Features </b>
	- 32-bit counter;
	- WDT clock uses live crystal clock;
	- Adjustable length of the WDT reset;
	- Interrupt asserted (if enabled) when counter hits the reg_wdt_int??
	- A flag for WDT tracking (un-touchable by global IP reset).

	\image html WDT.png

	<b> Usage </b>
	-# WDT on/off/restart:
		WDT will be enabled after system is turned-on;
		WDT will be disabled when set reg_wdt_max??to zero;
		WDT will be re-started when set reg_wdt_clr??
	-# Set  reg_wdt_max??for length of WDT period;
	-# Set  reg_wdt_int??to enable interrupt if needed;
		Interrupt asserted when "WDT counter [31:16]" is equal to reg_wdt_int??and "WDT counter[15:0]" is equal to 0x0000
	-# Set  reg_wdt_rst_len??to adjust the length of WDT reset.
	-# Read reg_wdt_rst_flag??to see if WDT reset ever happened.


    \defgroup G_WDT_INIT Initialization Task relative
    \ingroup  G_WDT
    \defgroup G_WDT_COMMON Common Task relative
    \ingroup  G_WDT
    \defgroup G_WDT_CONTROL Control relative
    \ingroup  G_WDT
    \defgroup G_WDT_OTHER  other relative
    \ingroup  G_WDT
    \defgroup G_WDT_ToBeModified WDT api to be modified
    \ingroup  G_WDT
    \defgroup G_WDT_ToBeRemove WDT api to be removed
    \ingroup  G_WDT
*/

#ifndef _DRV_WDT_H_
#define _DRV_WDT_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

////////////////////////////////////////////////////////////////////////////////
//  Type and Structure
////////////////////////////////////////////////////////////////////////////////
#define MSIF_WDT_LIB_CODE              {'W','D','T','_'}    //Lib code
#define MSIF_WDT_LIBVER                {'0','1'}            //LIB version
#define MSIF_WDT_BUILDNUM              {'0','4'}            //Build Number
#define MSIF_WDT_CHANGELIST            {'0','0','3','8','2','1','7','1'} //P4 ChangeList Number

#define WDT_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_WDT_LIB_CODE,                  /* IP__                                             */  \
    MSIF_WDT_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_WDT_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_WDT_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define MAX_TIMER_NUM			2

typedef enum _WDT_DbgLv
{
    E_WDT_DBGLV_NONE,			/* no debug message */
    E_WDT_DBGLV_ERR_ONLY,		/* show error only */
    E_WDT_DBGLV_INFO,			/* show error & informaiton */
    E_WDT_DBGLV_ALL			/* show error, information & funciton name */
}WDT_DbgLv;

typedef enum
{
    E_WDT_FAIL,
    E_WDT_OK,
} WDT_Result;

typedef enum _E_PIU_Timer
{
    E_TIMER_0,
    E_TIMER_1,
} E_PIU_Timer;

typedef struct _tmr_interrupt {
	MS_U32 		u32TmrInit;						/* Initial value of timer 						*/
 	MS_U32 		u32TmrMax;						/* Max value of timer 						*/
	void 		(*TmrFnct)(void *, void *, void *);	/* Function to execute when timer times out	*/
	void			*TmrFnctArg0;					/* Arguments supplied to user defined function 	*/
	void			*TmrFnctArg1;					/* Arguments supplied to user defined function 	*/
	void			*TmrFnctArg2;					/* Arguments supplied to user defined function 	*/
	MS_BOOL 	bTmrEn;							/* Flag indicating whether timer is enable 		*/
}tmr_interrupt;

////////////////////////////////////////////////////////////////////////////////
//  Function and Variable
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_WDT_GetLibVer
/// @brief \b Function \b Description: Show the WDT driver version
/// @ingroup G_WDT_COMMON
/// @param <IN>        \b None:
/// @param <OUT>       \b **pVerString : output WDT driver version
/// @param <RET>       \b eRETCODE :
/// @param <GLOBAL>    \b VER: Library version string
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_GetLibVer(const MSIF_Version **ppVersion);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Init
/// @brief \b Function  \b Description: Init and Start WDT
/// @ingroup G_WDT_INIT
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Init(WDT_DbgLv eLevel);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Stop
/// @brief \b Function  \b Description: Init WDT Stop
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Stop(WDT_DbgLv eLevel);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Clear
/// @brief \b Function  \b Description: clear WDT.
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_Clear(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_ClearRstFlag
/// @brief \b Function  \b Description: clear WDT reset flag.
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_ClearRstFlag(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsReset
/// @brief \b Function  \b Description: check if WDT is reset
/// @ingroup G_WDT_COMMON
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_IsReset(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsEnable
/// @brief \b Function  \b Description: check if WDT is Enable
/// @ingroup G_WDT_COMMON
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_IsEnable(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer 
/// @brief \b Function  \b Description: Set Timer
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer(WDT_DbgLv eLevel, MS_U16 sec);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer_ms
/// @brief \b Function  \b Description: Set Timer
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: msec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_ms(WDT_DbgLv eLevel, MS_U16 msec);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer_us
/// @brief \b Function  \b Description: Set Timer
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: msec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetTimer_us(WDT_DbgLv eLevel, MS_U16 usec);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetIntTimer
/// @brief \b Function  \b Description: Set INT Timer
/// @ingroup G_WDT_CONTROL
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_SetIntTimer(WDT_DbgLv eLevel, MS_U16 sec);

//Attribute
//muti-paremeter
//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @ingroup G_WDT_INIT
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_CfgFnct(E_PIU_Timer eTimer, void (*fnct)(void *, void *, void *), void *arg0, void *arg1, void *arg2 );
//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @ingroup G_WDT_COMMON
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_Count(E_PIU_Timer eTimer, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_INT(E_PIU_Timer eTimer, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_Rst(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_SetMaxMatch(E_PIU_Timer eTimer, MS_U32 u32MaxTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TIMER_HitMaxMatch(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TIMER_GetMaxMatch(E_PIU_Timer eTimer);
//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @ingroup G_WDT_COMMON
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TIMER_GetCounter(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TIMER_GetSecond(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TIMER_GetMs(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TIMER_GetUs(E_PIU_Timer eTimer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_Delay(E_PIU_Timer eTimer, MS_U32 u32Second);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_DelayMs(E_PIU_Timer eTimer, MS_U32 u32MS);
/// MOBF Encrypt
/// @ingroup G_WDT_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_DelayUs(E_PIU_Timer eTimer, MS_U32 u32US);
//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @ingroup G_WDT_COMMON
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
void MDrv_TIMER_Exit(void);

//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @ingroup G_WDT_COMMON
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_WDT_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef __cplusplus
}
#endif


#endif // _DRV_WDT_H_

