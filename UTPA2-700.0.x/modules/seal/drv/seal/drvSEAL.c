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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSEAL.c
/// @brief  SEAL Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsTypes.h"
#include "drvSEAL.h"
#include "halSEAL.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_SEAL "SEAL"
#define SEAL_ERROR(fmt, args...)           ULOGE(TAG_SEAL, "[SEAL USER ERR][%06d]     " fmt, __LINE__, ## args)
#define SEAL_WARN(fmt, args...)            ULOGW(TAG_SEAL, "[SEAL USER WARN][%06d]    " fmt, __LINE__, ## args)
#define SEAL_PRINT(fmt, args...)           ULOGD(TAG_SEAL, "[SEAL USER][%06d]         " fmt, __LINE__, ## args)

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

static SEAL_CB_FUNC  _pSEALCbFunc = NULL;


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_seal_version = {
    .DDI = { SEAL_DRV_VERSION },
};

static MS_BOOL _gbSEALInitialized = FALSE;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_SEAL_GetLibVer
/// @brief \b Function  \b Description: Show the SEAL driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b 0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SEAL_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_seal_version;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_SEAL_Init
/// @brief \b Function  \b Description: set SEAL I/O Map base address and parse occupied resource to software structure
/// @param None         \b IN :
/// @param None         \b OUT :
/// @param MS_BOOL      \b RET
/// @param None         \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SEAL_Init(void)
{
    MS_VIRT u32NonPmBaseAddr = 0;
    MS_PHY u32NonPmBankSize = 0;
    MS_VIRT u32PmBaseAddr = 0;
    MS_PHY u32PmBankSize = 0;

    if (_gbSEALInitialized == TRUE)
    {
        SEAL_WARN("%s is called before!\n", __FUNCTION__);
        return TRUE;
    }

    if( !MDrv_MMIO_GetBASE(&u32NonPmBaseAddr, &u32NonPmBankSize, MS_MODULE_MIU))
    {
        SEAL_ERROR("Get NonPM IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&u32PmBaseAddr, &u32PmBankSize, MS_MODULE_PM))
    {
        SEAL_ERROR("Get PM IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }

    HAL_SEAL_SetIOMapBase(u32NonPmBaseAddr, u32PmBaseAddr);
    _gbSEALInitialized = TRUE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureRangeSet
/// @brief \b Function       \b Description: set the secure range on MIU
/// @param u8SecureRangeId:  \b IN : Secure range ID
/// @param u64StartAddr      \b IN : Start address of secure range
/// @param u64EndAddr        \b IN : End address of secure range
/// @param u32Attribute      \b IN : Attribute of secure range
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureRangeSet(MS_U8 u8SecureRangeId ,MS_U64 u64StartAddr, MS_U64 u64EndAddr, MS_U32 u32Attribute)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(((u64StartAddr & (SEAL_SECURE_RANGE_ALIGN -1)) != 0) || ((u64EndAddr & (SEAL_SECURE_RANGE_ALIGN -1)) != 0))
    {
        SEAL_ERROR("Secure range address should be aligned to %lx\n",SEAL_SECURE_RANGE_ALIGN);
        bret = FALSE;
        return bret;
    }


    bret = HAL_Seal_SecureRangeSet(u8SecureRangeId, u64StartAddr, u64EndAddr, u32Attribute);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureRangeQuery
/// @brief \b Function       \b Description: Query the attribute of secure range
/// @param u8MiuDev          \b IN : 0:MIU0 1:MIU1 2:MIU2 3:MIU3
/// @param u8SecureRangeId:  \b IN : Secure range ID
/// @param pAttribute        \b OUT : Information of secure range
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureRangeQuery(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId, Seal_SecureRangeInfo *pSecureRangeInfo)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    pSecureRangeInfo->u8MiuDev = u8MiuDev;
    pSecureRangeInfo->u8SecureRangeId = u8SecureRangeId;
    bret = HAL_Seal_SecureRangeQuery(u8MiuDev,
                                     u8SecureRangeId,
                                     &(pSecureRangeInfo->u32Attribute),
                                     &(pSecureRangeInfo->u64StartAddr),
                                     &(pSecureRangeInfo->u64EndAddr));

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_IMI_RangeSet
/// @brief \b Function       \b Description: set the secure range on internel memory interface
/// @param u32StartAddr      \b IN : Start address of secure range on IMI
/// @param u32EndAddr        \b IN : End address of secure range on IMI
/// @param bEnable           \b IN : Enable/Disable secure range on IMI
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_IMI_RangeSet(MS_U32 u32StartAddr, MS_U32 u32EndAddr, MS_BOOL bEnable)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(((u32StartAddr & (SEAL_IMI_RANGE_ALIGN -1)) != 0) || ((u32EndAddr & (SEAL_IMI_RANGE_ALIGN -1)) != 0))
    {
        SEAL_ERROR("IMI range address should be aligned to %lx\n",SEAL_IMI_RANGE_ALIGN);
        bret = FALSE;
        return bret;
    }

    bret = HAL_Seal_IMI_RangeSet(u32StartAddr, u32EndAddr, bEnable);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_GetHittedInfo
/// @brief \b Function       \b Description: set the secure range on MIU
/// @param u8MiuDev          \b IN : 0:MIU0 1:MIU1 2:MIU2 3:MIU3
/// @param *pInfo            \b OUT : Hitted information
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_GetHittedInfo(MS_U8 u8MiuDev, Seal_PortectInfo *pInfo)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(u8MiuDev >= E_SEAL_MIU_NUM)
    {
        bret = FALSE;
        return bret;
    }

    pInfo->bIsHit = HAL_Seal_GetMiuHitFlag(u8MiuDev);
    pInfo->u8MiuDev = u8MiuDev;
    pInfo->u8SecureRangeId = HAL_Seal_GetMiuHitRangeID(u8MiuDev);
    pInfo->u8ClientId = HAL_Seal_GetMiuHitClientID(u8MiuDev);
    pInfo->bIsSecure = HAL_Seal_GetMiuSecureFlag(u8MiuDev);
    pInfo->bIsWrite = HAL_Seal_GetMiuWriteFlag(u8MiuDev);
    pInfo->u64HitAddr = HAL_Seal_GetMiuHitAddr(u8MiuDev);

    HAL_Seal_ClearMiuHitLog(u8MiuDev);

    if(pInfo->bIsHit == TRUE)
    {
        SEAL_PRINT("MIU:%u SecureRangeId:%u ClientId:0x%x IsSecure:%u IsWrite:%u HitAddr:0x%llx\n", u8MiuDev,
                   pInfo->u8SecureRangeId, pInfo->u8ClientId, pInfo->bIsSecure, pInfo->bIsWrite, (long long unsigned)pInfo->u64HitAddr);
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureRangeLock
/// @brief \b Function       \b Description: One-way setting to lock the specific secure range
/// @param u8MiuDev          \b IN : 0: MIU0 1:MIU1
/// @param u8SecureRangeId:  \b IN : Secure range ID
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureRangeLock(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(u8MiuDev >= E_SEAL_MIU_NUM)
    {
        bret = FALSE;
        return bret;
    }

    bret = HAL_Seal_SecureRangeLock(u8MiuDev, u8SecureRangeId);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_NonSecureProcessorSet
/// @brief \b Function       \b Description: Define the specific processor be secure or not
/// @param ProcessorId       \b IN : Processor ID
/// @param bSecure:          \b IN : 1: non-secure 0:secure
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_NonSecureProcessorSet(MS_U8 u8ProcessorId, MS_BOOL bNonSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(u8ProcessorId >= E_SEAL_PROCESSOR_NUM)
    {
        bret = FALSE;
        return bret;
    }

    bret = HAL_Seal_NonSecureProcessorSet(u8ProcessorId, bNonSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_NonSecureProcessorQuery
/// @brief \b Function       \b Description: Query specific processor be secure or not
/// @param ProcessorId       \b IN : Processor ID
/// @param bSecure:          \b OUT : 1: non-secure 0:secure
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_NonSecureProcessorQuery(MS_U8 u8ProcessorId, MS_BOOL *pNonSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    if(u8ProcessorId >= E_SEAL_PROCESSOR_NUM)
    {
        bret = FALSE;
        return bret;
    }

    bret = HAL_Seal_NonSecureProcessorQuery(u8ProcessorId, pNonSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureSlaveSet
/// @brief \b Function       \b Description: Define the specific slave IP be secure or not
/// @param u32SlaveId        \b IN : Slave ID
/// @param bSecure:          \b IN : 0: non-secure 1:secure
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureSlaveSet(MS_U32 u32SlaveId, MS_BOOL bSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    bret = HAL_Seal_SecureSlaveSet(u32SlaveId, bSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureSlaveQuery
/// @brief \b Function       \b Description: Query the specific slave IP be secure or not
/// @param u32SlaveId        \b IN : Slave ID
/// @param pSecure:          \b OUT : 0: non-secure 1:secure
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureSlaveQuery(MS_U32 u32SlaveId, MS_BOOL *pSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    bret = HAL_Seal_SecureSlaveQuery(u32SlaveId, pSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureMasterSet
/// @brief \b Function       \b Description: Define the specific master IP be secure or not
/// @param u32MasterId  \b IN : Master ID
/// @param bSecure:          \b IN : 0: non-secure 1:secure
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureMasterSet(MS_U32 u32MasterId, MS_BOOL bSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    bret = HAL_Seal_SecureMasterSet(u32MasterId, bSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SecureMasterQuery
/// @brief \b Function       \b Description: Query the specific master IP be secure or not
/// @param u32MasterId  \b IN : Master ID
/// @param pSecure:          \b OUT : 0: non-secure 1:secure
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_SecureMasterQuery(MS_U32 u32MasterId, MS_BOOL *pSecure)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    bret = HAL_Seal_SecureMasterQuery(u32MasterId, pSecure);

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_SetPowerState
/// @brief \b Function       \b Description: STR rountine
/// @param u16PowerState  \b IN : Power state
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_Seal_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = FALSE;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        _gbSEALInitialized = FALSE;
        HAL_SEAL_POWER_SUSPEND();
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            MDrv_SEAL_Init();
            HAL_SEAL_POWER_RESUME();
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_SEAL, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_SEAL, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_BufferLock
/// @brief \b Function       \b Description: Lock the specific buffer
/// @param <IN>              \b u8SecureRangeId: Secure buffer ID
/// @param <IN>              \b bLock: 0: unlock 1:lock
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_BufferLock(MS_U8 u8BufferLockId, MS_BOOL bLock)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_BOOL bret;

    bret = HAL_Seal_BufferLock(u8BufferLockId, bLock);

    return bret;
}


#if !defined(CONFIG_FRC)
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_ClearInterruptFlag
/// @brief \b Function       \b Description: Static function ,Clear  miu0/miu1 interrupt flag
/// @param void  \b IN : void:
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
static void  MDrv_Seal_ClearInterruptFlag(void)
{
	HAL_Seal_ClearMiuHitLog(E_SEAL_MIU_DEV0);
	HAL_Seal_ClearMiuHitLog(E_SEAL_MIU_DEV1);
	HAL_Seal_ClearMiuHitLog(E_SEAL_MIU_DEV2);
	HAL_Seal_ClearMiuHitLog(E_SEAL_MIU_DEV3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_MaskInterruptFlag
/// @brief \b Function       \b Description: Static function , Mask/UnMask miu0/miu1 interrupt bit
/// @param bMask  \b IN : bMask:True, Mask seal interrupt bit for not trigger interrupt; False, Unmaks seal interrupt bit for trigger interrupt
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_Seal_MaskInterruptFlag(MS_BOOL bMask)
{
	HAL_Seal_MaskInterruptFlag(E_SEAL_MIU_DEV0, bMask);
	HAL_Seal_MaskInterruptFlag(E_SEAL_MIU_DEV1, bMask);
	HAL_Seal_MaskInterruptFlag(E_SEAL_MIU_DEV2, bMask);
	HAL_Seal_MaskInterruptFlag(E_SEAL_MIU_DEV3, bMask);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_isr
/// @brief \b Function       \b Description:Static function , While Seal interrupt trigger, this function is handled
/// @param eIntNum  \b eIntNum : interrupt vector number
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
irqreturn_t MDrv_Seal_isr(InterruptNum eIntNum, void* dev_id)
#else
void MDrv_Seal_isr(InterruptNum eIntNum )
#endif
{
	Seal_PortectInfo sPInfo_miu0;
	Seal_PortectInfo sPInfo_miu1;

	if (_pSEALCbFunc != NULL)
	{
		_pSEALCbFunc(eIntNum);
	}
    else
    {
	    MDrv_Seal_GetHittedInfo(E_SEAL_MIU_DEV0, &sPInfo_miu0);
	    MDrv_Seal_GetHittedInfo(E_SEAL_MIU_DEV1, &sPInfo_miu1);
	    MDrv_Seal_GetHittedInfo(E_SEAL_MIU_DEV2, &sPInfo_miu1);
	    MDrv_Seal_GetHittedInfo(E_SEAL_MIU_DEV3, &sPInfo_miu1);
    }

	#if !defined(CONFIG_FRC)
	MDrv_Seal_ClearInterruptFlag();
	MsOS_EnableInterrupt(E_INT_IRQ_MIU_SECURITY);
	#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_ENInterrupt
/// @brief \b Function       \b Description: Enable/Disable SEAL interrupt
/// @param bEnable  \b IN : bEnable: True: Enable SEAL interrupt; False:Diable SEAL interrupt
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_ENInterrupt(MS_BOOL bEnable)
{
    if (!_gbSEALInitialized)
    {
        SEAL_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

#if !defined(CONFIG_FRC)
	if (bEnable == TRUE)
	{
		MsOS_AttachInterrupt(E_INT_IRQ_MIU_SECURITY, MDrv_Seal_isr);
		MDrv_Seal_ClearInterruptFlag();
		MDrv_Seal_MaskInterruptFlag(FALSE);
		MsOS_EnableInterrupt(E_INT_IRQ_MIU_SECURITY);
	}
	else
	{
		MDrv_Seal_MaskInterruptFlag(TRUE);
		MDrv_Seal_ClearInterruptFlag();
		MsOS_DisableInterrupt(E_INT_IRQ_MIU_SECURITY);
		MsOS_DetachInterrupt(E_INT_IRQ_MIU_SECURITY);
	}
#endif
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_AttachCallbackFunc
/// @brief \b Function       \b Description: Attach user call back function
/// @param void  \b IN : void
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_AttachCallbackFunc(SEAL_CB_FUNC pSEALCBFunc)
{
	_pSEALCbFunc = pSEALCBFunc;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_DispatchCallbackFunc
/// @brief \b Function       \b Description: Dispatch user call back function
/// @param void  \b IN : void
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_DispatchCallbackFunc(void)
{
	_pSEALCbFunc = NULL;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function       \b Name: MDrv_Seal_CheckSecureRange
/// @brief \b Function       \b Description: Is Overlap Secure Range
/// @param phyStartAddr      \b IN : start addr
/// @param u32Length         \b IN : prtect length
/// @param eCheckSecureRangeType     \b IN : check Overlap Type
/// @param None              \b OUT :
/// @param MS_BOOL           \b RET :
/// @param None              \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_Seal_CheckSecureRange(MS_PHY phyStartAddr, MS_U32 u32Length, eSeal_CheckSecureRangeType eCheckSecureRangeType)
{
    return HAL_Seal_CheckSecureRange(phyStartAddr, u32Length, eCheckSecureRangeType);
}

MS_BOOL MDrv_Seal_QueryBufferLocked(eSeal_Lock eLockId, MS_BOOL* bLocked)
{
    return HAL_Seal_QueryBufferLocked(eLockId, bLocked);
}

#ifdef MSOS_TYPE_OPTEE
MS_BOOL MDrv_Seal_ChangeIPSecureDMAAbillity(MS_U32 u32ModuleID, MS_U32 u32ModuleParameter, MS_U32 u32IsSecure)
{
    return HAL_Seal_ChangeIPSecureDMAAbillity(u32ModuleID, u32ModuleParameter, u32IsSecure);
}

MS_BOOL MDrv_Seal_SetSecureRange(MS_PHY u64phy, MS_U32 u32Length, MS_U32 u32IsSecure)
{
    return HAL_Seal_SetSecureRange(u64phy, u32Length, u32IsSecure);
}
#endif
