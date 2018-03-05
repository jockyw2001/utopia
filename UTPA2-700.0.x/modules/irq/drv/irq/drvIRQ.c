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
/// file    drvIRQ.c
/// @brief  IRQ Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvIRQ.h"
#include "halIRQ.h"
#include "drvMMIO.h"
#include "MsVersion.h"
#include "regIRQ.h"

#if defined(MSOS_TYPE_NUTTX)
#include "debug.h"
#endif
#include "utopia.h"

#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL gbIRQ_InitFg=FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_drvirq_version = {
    .DDI = { DRVIRQ_DRV_VERSION },
};
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MSOS_GetLibVer
/// @brief \b Function  \b Description: Show the MSOS driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b Result
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_IRQ_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_drvirq_version;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRQ_Init()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    MS_VIRT u32Base = 0;
    MS_PHY u32Size = 0;

    if (gbIRQ_InitFg)
        return true;
    if (MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_IRQ))
    {
        HAL_IRQ_Set_IOMap(u32Base);
        HAL_IRQ_Init();
        gbIRQ_InitFg=true;
        return TRUE;
    }
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    return FALSE;
}

void MDrv_IRQ_Attach(MS_U32 u32Vector, void *pIntCb)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Attach(u32Vector, pIntCb,0);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Detech(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Detech(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Restore()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Restore();
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_MaskAll()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_MaskAll(TRUE);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_Mask(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_Mask(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_UnMaskAll( )
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_MaskAll(FALSE);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_UnMask(MS_U32 u32Vector)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_UnMask(u32Vector);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

void MDrv_IRQ_NotifyCpu(IRQ_CPU_TYPE eType)
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    HAL_IRQ_NotifyCpu(eType);
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

MS_BOOL MDrv_IRQ_InISR()
{
#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
    return HAL_IRQ_InISR();
#else
    return FALSE;
#endif // #if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)
}

MS_U32 MDrv_IRQ_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_IRQ_Init();

			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
			ULOGE("IRQ", "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE("IRQ", "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}



