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
/// file    drvPWM.c
/// @brief  Pulse Width Modulation Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsVersion.h"
#include "drvPWM.h"
#include "halPWM.h"
#include "drvMMIO.h"
#include "MsDevice.h"
#include "MsOS.h"
#include "drvSEM.h"
#include "ULog.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////
#define TAG_PWM "PWM"

#define PWM_DBG_FUNC()            if (_geDbfLevel >= E_PWM_DBGLV_ALL)                   \
                                       {MS_DEBUG_MSG(ULOGD(TAG_PWM, "\t====   %s   ====\n", __FUNCTION__));}

#define PWM_DBG_INFO(x, args...)  if (_geDbfLevel >= E_PWM_DBGLV_INFO )                 \
                                       {MS_DEBUG_MSG(ULOGI(TAG_PWM, x, ##args));}

#define PWM_DBG_ERR(x, args...)   if (_geDbfLevel >= E_PWM_DBGLV_ERR_ONLY)				\
                                       {MS_DEBUG_MSG(ULOGE(TAG_PWM, x, ##args));}
                                       
                                       
#define PWM_SEM_LOCK() MDrv_SEM_Lock(E_SEM_PM, SEM_WAIT_FOREVER);
#define PWM_SEM_UNLOCK() MDrv_SEM_Unlock(E_SEM_PM);

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL  _gbInitPWM = FALSE;
static PWM_DbgLv _gbInitPWM_DbgLv = E_PWM_DBGLV_NONE;
//static PWM_Arg _gsPWMDesc;
static PWM_DbgLv _geDbfLevel = E_PWM_DBGLV_NONE;
static MSIF_Version _drv_pwm_version = {
    .DDI = { PWM_DRV_VERSION },
};

static MS_BOOL g_Enable_Record[PWM_Num] ={1, 1, 1, 1};
static MS_U32 g_Shift_Record[PWM_Num] = {0, 0, 0, 0};
static MS_U32 g_Period_Record[PWM_Num] = {0, 0, 0, 0};
static MS_U32 g_Duty_Record[PWM_Num] = {0, 0, 0, 0};
static MS_BOOL g_Vsync_Record[PWM_Num] ={0, 0, 0, 0};

static PWM_DrvStatus   _pwmStatus =
                        {
                            .PWM0 = FALSE,
                            .PWM1 = FALSE,
                            .PWM2 = FALSE,
                            .PWM3 = FALSE,
                            .PWM4 = FALSE,
                            .PWM5 = FALSE,
                            .PWM6 = FALSE,
                            .PWM7 = FALSE,
                            .PWM8 = FALSE,
                        };

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _PWM_IsInit(void);

//-------------------------------------------------------------------------------------------------
/// Description: Set debug level for debug message
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_SetDbgLevel(PWM_DbgLv eLevel)
{
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, eLevel);

    _geDbfLevel = eLevel;

    return E_PWM_OK;
}

//------------------------------------------------------------------------------
/// Description : Show the PWM driver version
/// @param  ppVersion \b OUT: output PWM driver version
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetLibVer(const MSIF_Version **ppVersion)
{
    PWM_DBG_FUNC();

    if (!ppVersion)
        return E_PWM_FAIL;

    *ppVersion = &_drv_pwm_version;
    return E_PWM_OK;
}

//------------------------------------------------------------------------------
/// Description : Show the PWM_PAD is PWM(True) or GPIO(false)
/// @param  pStatus \b OUT: output PWM driver status
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetStatus(PWM_DrvStatus *pStatus)
{
    memcpy(pStatus, &_pwmStatus, sizeof(PWM_DrvStatus));
    return E_PWM_OK;
}

//-------------------------------------------------------------------------------------------------
/// Description : Check PWM is initial or not
/// @return TRUE : PWM was initied
/// @return FALSE : PWM was not initied
//-------------------------------------------------------------------------------------------------
static MS_BOOL _PWM_IsInit(void)
{
    PWM_DBG_FUNC();
    if (!_gbInitPWM)
        PWM_DBG_ERR("Call MDrv_PWM_Init first!\n");
    return _gbInitPWM;
}

//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Init(PWM_DbgLv eLevel)
{
    MS_VIRT virtBaseAddr = 0;
    MS_VIRT virtBaseAddr1 = 0;
    MS_VIRT virtBaseAddr2 = 0;
    MS_PHY u32BaseSize = 0; /* No use */

	_gbInitPWM_DbgLv = eLevel; //store for STD resume
    MDrv_PWM_SetDbgLevel(eLevel);

    if(!_gbInitPWM)
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PWM))
        {
            PWM_DBG_ERR("Get IOMAP Base faill!\n");
            return E_PWM_FAIL;
        }

        if(!MDrv_MMIO_GetBASE(&virtBaseAddr1, &u32BaseSize, MS_MODULE_CHIPTOP))
        {
            PWM_DBG_ERR("Get CHIPTOP Base faill!\n");
            return E_PWM_FAIL;
        }

        if(!MDrv_MMIO_GetBASE(&virtBaseAddr2, &u32BaseSize, MS_MODULE_PM))
        {
            PWM_DBG_ERR("Get PM Base faill!\n");
            return E_PWM_FAIL;
        }
        HAL_PWM_SetIOMapBase(virtBaseAddr, virtBaseAddr2);
        HAL_PWM_SetChipTopIOMapBase(virtBaseAddr1);
        PWM_DBG_INFO("%s has MMIO base 0x%08X\r\n", __FUNCTION__,  virtBaseAddr);
        /* Set All pad output and Is PWM. But it still needs to be enable */
        if( !HAL_PWM_Init() )
        {
            PWM_DBG_ERR("PWM Hal Initial Fail\n");
            return E_PWM_FAIL;
        }
        
        MDrv_SEM_Init(); // for sc4 bank protect

        _gbInitPWM = TRUE;
        return E_PWM_OK;
    }
    else
    {
        PWM_DBG_ERR("PWM has be initiated!\n");
        return E_PWM_OK;
    }
}

//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM)
{
    PWM_Result ret = E_PWM_OK;
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bOenPWM);

    switch(u8IndexPWM)
    {
        case E_PWM_CH0:
            _pwmStatus.PWM0 = TRUE;
            break;
        case E_PWM_CH1:
            _pwmStatus.PWM1 = TRUE;
            break;
        case E_PWM_CH2:
            _pwmStatus.PWM2 = TRUE;
            break;
        case E_PWM_CH3:
            _pwmStatus.PWM3 = TRUE;
            break;
        case E_PWM_CH4:
            _pwmStatus.PWM4 = TRUE;
            break;
        case E_PWM_CH5:
            _pwmStatus.PWM5 = TRUE;
            break;
        case E_PWM_CH6:
            _pwmStatus.PWM6 = TRUE;
            break;
        case E_PWM_CH7:
            _pwmStatus.PWM7 = TRUE;
            break;
        case E_PWM_CH8:
            _pwmStatus.PWM8 = TRUE;
            break;
        default:
            MS_ASSERT(0);
    }

    if( !(HAL_PWM_Oen(u8IndexPWM, bOenPWM)||_PWM_IsInit()) )
    {
        ret = E_PWM_FAIL;
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Period(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int)u32PeriodPWM);

    PWM_SEM_LOCK();
    do{
        HAL_PWM_Period(u8IndexPWM, u32PeriodPWM);
        ret = E_PWM_OK;
        if (g_Enable_Record[u8IndexPWM] == 1)
        {
            g_Period_Record[u8IndexPWM]  = u32PeriodPWM;
        }

    }while(0);
    PWM_SEM_UNLOCK();

    return ret;
}


PWM_Result MDrv_PWM_GetProperty(PWM_Property eProperty, PWM_ChNum u8IndexPWM, MS_U32 *u32PWMVar)
{
    PWM_SEM_LOCK();
    switch(eProperty)
    {
        case E_PWM_GetPeriod:
            *u32PWMVar = HAL_PWM_GetPeriod(u8IndexPWM);
            break;
        case E_PWM_GetDutyCycle:
            *u32PWMVar = HAL_PWM_GetDutyCycle(u8IndexPWM);
            break;
        case E_PWM_GetShift:
            *u32PWMVar = HAL_PWM_GetShift(u8IndexPWM);
            break;
        case E_PWM_GetDiv:
            *u32PWMVar = (MS_U32)HAL_PWM_GetDiv(u8IndexPWM);
            break;
        case E_PWM_GetPolarity:
            *u32PWMVar = (MS_U32)HAL_PWM_GetPolarity(u8IndexPWM);
            break;
        case E_PWM_GetVdben:
            *u32PWMVar = (MS_U32)HAL_PWM_GetVDBen(u8IndexPWM);
            break;
        case E_PWM_GetRsten:
            *u32PWMVar = (MS_U32)HAL_PWM_GetVrest(u8IndexPWM);
            break;
        case E_PWM_GetDben:
            *u32PWMVar = (MS_U32)HAL_PWM_GetDBen(u8IndexPWM);
            break;
        case E_PWM_GetOen:
            *u32PWMVar = (MS_U32)HAL_PWM_GetOen(u8IndexPWM);
            break;
        default:
            ULOGE(TAG_PWM, "[Utopia] PWM is not support!!\n");
            PWM_SEM_UNLOCK();
            return E_PWM_FAIL;
    }
    PWM_SEM_UNLOCK();
    return E_PWM_OK;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int)u32DutyPWM);

    PWM_SEM_LOCK();
    do{
        HAL_PWM_DutyCycle(u8IndexPWM, u32DutyPWM);
        ret = E_PWM_OK;
        if (g_Enable_Record[u8IndexPWM] == 1)
        {
            g_Duty_Record[u8IndexPWM]  = u32DutyPWM;
        }

    }while(0);
    PWM_SEM_UNLOCK();
    
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Unit_Div of the pwm
/// @param u16UnitDivPWM    \b the Unit Div value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_UnitDiv(MS_U16 u16UnitDivPWM)
{
    PWM_Result ret = E_PWM_OK;
    PWM_DBG_INFO("%s(0x%04X)\r\n", __FUNCTION__, u16UnitDivPWM);

    PWM_SEM_LOCK();
    if(!HAL_PWM_UnitDiv(u16UnitDivPWM))
    {
        ret = E_PWM_FAIL;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Div(PWM_ChNum u8IndexPWM, MS_U16 u16DivPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, 0x%04X)\r\n", __FUNCTION__, u8IndexPWM, u16DivPWM);
    HAL_PWM_Div(u8IndexPWM, u16DivPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, MS_BOOL bPolPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bPolPWM);
    HAL_PWM_Polarity(u8IndexPWM, bPolPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Vsync Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bVdbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bVdbenPWM);
    HAL_PWM_VDBen(u8IndexPWM, bVdbenPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Hsync reset of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bRstPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_ResetEn(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bRstPWM);
    HAL_PWM_Vrest(u8IndexPWM, bRstPWM);
    if (g_Enable_Record[u8IndexPWM] == 1)
    {
        g_Vsync_Record[u8IndexPWM]  = bRstPWM;
    }
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bDbenPWM);
    HAL_PWM_DBen(u8IndexPWM, bDbenPWM);
    PWM_SEM_UNLOCK();
}

void MDrv_PWM_IMPULSE_EN(PWM_ChNum u8IndexPWM, MS_BOOL bdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bdbenPWM);
    HAL_PWM_IMPULSE_EN(u8IndexPWM, bdbenPWM);
    PWM_SEM_UNLOCK();
}

void MDrv_PWM_ODDEVEN_SYNC(PWM_ChNum u8IndexPWM, MS_BOOL bdbenPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bdbenPWM);
    HAL_PWM_ODDEVEN_SYNC(u8IndexPWM, bdbenPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Rst Mux of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bMuxPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstMux(PWM_ChNum u8IndexPWM, MS_BOOL bMuxPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bMuxPWM);
    HAL_PWM_RstMux(u8IndexPWM, bMuxPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Rst_Cnt of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u8RstCntPWM    \b the Rst_Cnt value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstCnt(PWM_ChNum u8IndexPWM, MS_U8 u8RstCntPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, 0x%02X)\r\n", __FUNCTION__, u8IndexPWM, u8RstCntPWM);
    HAL_PWM_RstCnt(u8IndexPWM, u8RstCntPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Bypass Unit of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bBypassPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_BypassUnit(PWM_ChNum u8IndexPWM, MS_BOOL bBypassPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bBypassPWM);
    HAL_PWM_BypassUnit(u8IndexPWM, bBypassPWM);
    PWM_SEM_UNLOCK();
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM0 and PWM1
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM1 donate internal divider to PWM0    \n
///     10: PWM0 donate internal divider to PWM1    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM01_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( HAL_PWM01_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM2 and PWM3
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM3 donate internal divider to PWM2    \n
///     10: PWM2 donate internal divider to PWM3    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM23_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( HAL_PWM23_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM6 and PWM7
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM7 donate internal divider to PWM6    \n
///     10: PWM6 donate internal divider to PWM7    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM67_CntMode(PWM_CntMode u8CntModePWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u)\r\n", __FUNCTION__, u8CntModePWM);
    PWM_SEM_LOCK();
    if( HAL_PWM67_CntMode(u8CntModePWM) )
    {
		ret = E_PWM_OK;
    }
    PWM_SEM_UNLOCK();
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Shift of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Shift value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Shift(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    PWM_DBG_INFO("%s(%u, 0x%08X)\r\n", __FUNCTION__, u8IndexPWM, (int) u32DutyPWM);
    PWM_SEM_LOCK();
    if ( HAL_PWM_Shift(u8IndexPWM, u32DutyPWM) )
    {
        ret = E_PWM_OK;
        if (g_Enable_Record[u8IndexPWM] == 1)
        {
            g_Shift_Record[u8IndexPWM]  = u32DutyPWM;
        }

    }
    PWM_SEM_UNLOCK();
    return ret;
}


void MDrv_PWM_Nvsync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bNvsPWM);
    HAL_PWM_Nvsync(u8IndexPWM, bNvsPWM);
    PWM_SEM_UNLOCK();
}


void MDrv_PWM_Align(PWM_ChNum u8IndexPWM, MS_BOOL bAliPWM)
{
    PWM_SEM_LOCK();
    PWM_DBG_INFO("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bAliPWM);
    HAL_PWM_Align(u8IndexPWM, bAliPWM);
    PWM_SEM_UNLOCK();
}

MS_BOOL MDrv_PWM_Set3DMode(PWM_ChNum u8IndexPWM, PWM_3D_Arg *pArraySeting, MS_U8 u8ArraySize)
{
    PWM_3D_Arg PWM_Param[MAX_3DPWM_NUM];
    MS_U8 u8Index=0;
    MS_U32 u32Shift=0 , u32Duty=0;
    MS_U32 u32AccumulativeLine=0;
	

    /* 
        Error control for handling parameter error 
    */
    //Judge for which PWM to support 3D function
    if (HAL_PWM_IsSupport3D(u8IndexPWM) != TRUE)
    {
        PWM_DBG_ERR("%s, pwm index[%d] can not support 3D Mode\n", __FUNCTION__, u8IndexPWM);
        return FALSE;
    }

    //Judge for how many different waveform to support
    if ((u8ArraySize > MAX_3DPWM_NUM) || (MAX_3DPWM_NUM<=0))
    {
        PWM_DBG_ERR("%s, pwm index[%d]-[%d] : illeage support number, exceed max number:%d \n", __FUNCTION__, u8IndexPWM, u8ArraySize, MAX_3DPWM_NUM);
        return FALSE;
    }

    /*
        "u8ArraySize=0" : Disable 3D Mode
        "u8ArraySize>0" : Set 3D mode
    */
    g_Enable_Record[u8IndexPWM] = 0;
    if (u8ArraySize == 0)
    {   
        //ULOGD(TAG_PWM, "==%s, %d\n", __FUNCTION__, __LINE__);
        HAL_PWM_SetMultiDiff(FALSE);
        MDrv_PWM_ResetEn(u8IndexPWM, g_Vsync_Record[u8IndexPWM]);
        MDrv_PWM_Period(u8IndexPWM, g_Period_Record[u8IndexPWM]);
        MDrv_PWM_DutyCycle(u8IndexPWM, g_Duty_Record[u8IndexPWM]);
        MDrv_PWM_Shift(u8IndexPWM, g_Shift_Record[u8IndexPWM]);
        MDrv_PWM_Div(u8IndexPWM, 0);       
        //ULOGD(TAG_PWM, "==%s, %d\n", __FUNCTION__, __LINE__);
        g_Enable_Record[u8IndexPWM] = 1;
        return TRUE;
    }

    //Period Max set
    MDrv_PWM_Period(u8IndexPWM, 0x3FFFF);
    MDrv_PWM_Div(u8IndexPWM, (PWM_DIV>0?(PWM_DIV-1):0) );

    /*
        Initalize Parameter	
    */
    memset(PWM_Param, 0, sizeof(PWM_Param));
    memcpy(PWM_Param, (PWM_3D_Arg *)pArraySeting , sizeof(PWM_3D_Arg)*u8ArraySize);

    /*
        Start to set 3D parameter
    */
    u32AccumulativeLine = 0;
    for (u8Index = 0; u8Index<MAX_3DPWM_NUM; u8Index++)
    {
        if ((PWM_Param[u8Index].Shift == 0) && (PWM_Param[u8Index].Duty == 0))
        {
            u32Shift = 0x0;
            u32Duty  = 0x0;
        }
        else
        {
            u32Shift = u32AccumulativeLine + PWM_Param[u8Index].Shift;
            u32AccumulativeLine = u32Shift;
            u32Duty = u32AccumulativeLine + PWM_Param[u8Index].Duty;
            u32AccumulativeLine = u32Duty;
        }

        u32Shift = (MS_U32) u32Shift / PWM_DIV;
        u32Duty =  (MS_U32) u32Duty / PWM_DIV;

        if (u8Index == 0  && u32Shift == 0)
            u32Shift = 1;

        if (HAL_PWM_Set3D_DiffWaveform(u8IndexPWM, u8Index, u32Shift, u32Duty) == FALSE)
            return FALSE;
    }

    /*
        Trigger 3D mode Config
    */
    //Set 3D Diff Enable
    HAL_PWM_SetMultiDiff(TRUE);
    //Set Vsync reset Enable
    MDrv_PWM_ResetEn(u8IndexPWM, TRUE);
    g_Enable_Record[u8IndexPWM] = 1;
    return TRUE;
}

//-------------PM BASE---------------

void MDrv_PM_PWM_Enable(void)
{
    HAL_PM_PWM_Enable();
}

void MDrv_PM_PWM_Period(MS_U16 u16PeriodPWM)
{
   HAL_PM_PWM_Period(u16PeriodPWM);
}

void MDrv_PM_PWM_DutyCycle(MS_U16 u16DutyPWM)
{
   HAL_PM_PWM_DutyCycle(u16DutyPWM);
}

void MDrv_PM_PWM_Div(MS_U8 u8DivPWM)
{
    HAL_PM_PWM_Div(u8DivPWM);
}

void MDrv_PM_PWM_Polarity(MS_BOOL bPolPWM)
{
    HAL_PM_PWM_Polarity(bPolPWM);
}

void MDrv_PM_PWM_Dben(MS_BOOL bDbenPWM)
{
    HAL_PM_PWM_DBen(bDbenPWM);
}
MS_U16 MDrv_PWM_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitPWM = FALSE;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_PWM_Init(_gbInitPWM_DbgLv);

			_prev_u16PowerState = u16PowerState;
			u16Return = 1;//RESUME_OK;
		}
		else
		{
			ULOGE(TAG_PWM, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = 3;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE(TAG_PWM, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = FALSE;
	}

	return u16Return;// for success
}

//-------------------------------------------------------------------------------------------------
/// Description: inverse 3D flag
/// @param bInvPWM   \b True/False for Enable/Disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_INV_3D_Flag(MS_BOOL bInvPWM)
{
    PWM_Result ret = E_PWM_FAIL;
    if ( HAL_PWM_INV_3D_Flag(bInvPWM))
    {
        ret = E_PWM_OK;
    }
    return ret;
}

void MDrv_PWM_3D_LR_Sync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM, N_LR_SYNC_SEL LrSyncSel, MS_U8 u8RstCnt)
{
    MDrv_PWM_RstCnt(u8IndexPWM, u8RstCnt);
    MDrv_PWM_RstMux(u8IndexPWM, bNvsPWM);
    HAL_PWM_LR_RST_SEL(u8IndexPWM, (bNvsPWM ? LrSyncSel : E_VSYNC));
    MDrv_PWM_Nvsync(u8IndexPWM, bNvsPWM);
    MDrv_PWM_Align(u8IndexPWM, FALSE);
}
