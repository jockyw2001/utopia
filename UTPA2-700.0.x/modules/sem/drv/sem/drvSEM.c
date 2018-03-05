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
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvSEM.h"
#include "drvMMIO.h"
#include "halSEM.h"

#include "utopia_dapi.h"
#include "utopia.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static SEM_DbgLvl _gSEMDbgLevel = E_SEM_DBGLVL_ERROR;
static MS_BOOL _gbSEMInitialized = FALSE;
#define SEM_MAX_NUM                       (16)
#define TAG_SEM "SEM"

#ifdef CONFIG_MSTAR_UTPA2K_SEM_ENABLE
#define SEM_UTOPIA20 (1)
#else
#define SEM_UTOPIA20 (0)
#endif
//#define SEM_UTOPIA20 1
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define SEM_PRINT()               if (_gSEMDbgLevel >= E_SEM_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SEM, "\t====   %s   ====\n", __FUNCTION__);)}
#define SEM_DEBUG(x, args...)     if (_gSEMDbgLevel >= E_SEM_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SEM, "[SEM USER DEBUG][%s]: ", __FUNCTION__); ULOGD(TAG_SEM, x, ##args);)}
#define SEM_ERROR(x, args...)      if (_gSEMDbgLevel >= E_SEM_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SEM, "[SEM USER ERR][%s]: ", __FUNCTION__); ULOGE(TAG_SEM, x, ##args);)}
#define SEM_WARN(x, args...)     if (_gSEMDbgLevel >= E_SEM_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_SEM, "[SEM USER WARN][%s]: ", __FUNCTION__); ULOGW(TAG_SEM, x, ##args);)}
										
#define SEM_PM51_ID                       0x01
#define SEM_AEON_ID                       0x02
#define SEM_ARM_MIPS_ID                   0x03

#if (defined(MCU_AEON))
    #define    SEM_RESOURCE_ID            SEM_AEON_ID
#else
    #define    SEM_RESOURCE_ID            SEM_ARM_MIPS_ID
#endif

//define mutex
#define SEM_MUTEX_CREATE(_index)          _s32SEMMutex[_index] = MsOS_CreateMutex(E_MSOS_FIFO, _SEMMutexName[_index], MSOS_PROCESS_SHARED)
#define SEM_MUTEX_LOCK(_index, _waitms)   OS_OBTAIN_MUTEX(_s32SEMMutex[_index], _waitms)
#define SEM_MUTEX_UNLOCK(_index)          OS_RELEASE_MUTEX(_s32SEMMutex[_index])
#define SEM_MUTEX_DELETE(_index)          OS_DELETE_MUTEX(_s32SEMMutex[_index])

static MS_S32 _s32SEMMutex[SEM_MAX_NUM + 1] = {-1};
static char _SEMMutexName[SEM_MAX_NUM][13] = {"SEMMUTEX0","SEMMUTEX1","SEMMUTEX2","SEMMUTEX3","SEMMUTEX4","SEMMUTEX5","SEMMUTEX6","SEMMUTEX7",\
                                              "SEMMUTEX8","SEMMUTEX9","SEMMUTEX10","SEMMUTEX11","SEMMUTEX12","SEMMUTEX13","SEMMUTEX14","SEMMUTEX15"};

static MSIF_Version _drv_sem_version = {
    .DDI = { SEM_DRV_VERSION },
};

#if (SEM_UTOPIA20)
#include "drvSEM_priv.h"
MS_BOOL bU20SemOpened = FALSE;
MS_BOOL _MDrv_SEM_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	if (FALSE == bU20SemOpened)
	{
		if(UtopiaOpen(MODULE_SEM|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
		{
			//WDT_DBG_ERR("Open WDT fail\n");
			return FALSE;
		}
		bU20SemOpened= TRUE;
	}
	return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
// Gloabal Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Attempt to get mmio base and create mutex
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------

void* pInstantSem ;
void* pAttributeSEM ;


MS_BOOL MDrv_SEM_Init(void)
{
    MS_VIRT u32BaseAddr;
	MS_PHY u32BaseSize;
    MS_S32 s32Index;

    if (_gbSEMInitialized)
        return TRUE;

    if (!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_SEM))
    {
        MS_CRITICAL_MSG(ULOGE(TAG_SEM, "[%s][%d] Get register base fail\n", __FUNCTION__, __LINE__););
        return FALSE;
    }

    // make sure MMIO base is correct
    // a more appropriate solution might be let MDrv_MMIO_GetBASE return FALSE before getting correct base
    if (u32BaseSize==0)
        return FALSE;

    for(s32Index = 0; s32Index < SEM_MAX_NUM; s32Index++)
    {
        SEM_MUTEX_CREATE(s32Index);
        SEM_DEBUG("_s32SEMMutex[%ld] = %ld\n", s32Index, _s32SEMMutex[s32Index]);
    }

    _gbSEMInitialized = TRUE;

    return HAL_SEM_SetBank(u32BaseAddr);
}


//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SEM_Get_Resource_U2K(MS_U8 u8SemID, MS_U16 u16ResId)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    return HAL_SEM_Get_Resource(u8SemID, u16ResId);
}

MS_BOOL MDrv_SEM_Get_Resource(MS_U8 u8SemID, MS_U16 u16ResId)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_GETRESOURCE_PARAM pSemGetResourceParam = NULL;
	
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;
	
	pSemGetResourceParam = (PSEM_GETRESOURCE_PARAM)malloc(sizeof(SEM_GETRESOURCE_PARAM));
	
	pSemGetResourceParam->u8SemID = u8SemID;
	pSemGetResourceParam->u16ResId = u16ResId;
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Get_Resource,(MS_U32*)pSemGetResourceParam);
	free(pSemGetResourceParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Get_Resource_U2K(u8SemID,u16ResId);
#endif	
}

//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SEM_Free_Resource_U2K(MS_U8 u8SemID, MS_U16 u16ResId)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    return HAL_SEM_Free_Resource(u8SemID, u16ResId);
}

MS_BOOL MDrv_SEM_Free_Resource(MS_U8 u8SemID, MS_U16 u16ResId)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_FREERESOURCE_PARAM pSemFreeResourceParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;
	pSemFreeResourceParam = (PSEM_FREERESOURCE_PARAM)malloc(sizeof(SEM_FREERESOURCE_PARAM));
	pSemFreeResourceParam->u8SemID = u8SemID;
	pSemFreeResourceParam->u16ResId = u16ResId;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Free_Resource,(MS_U32*)pSemFreeResourceParam);
	free(pSemFreeResourceParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Free_Resource_U2K(u8SemID,u16ResId);
#endif	
}


//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Reset_Resource_U2K(MS_U8 u8SemID)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    return HAL_SEM_Reset_Resource(u8SemID);
}


MS_BOOL MDrv_SEM_Reset_Resource(MS_U8 u8SemID)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_RESETRESOURCE_PARAM pSemResetResourceParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;	
	pSemResetResourceParam = (PSEM_RESETRESOURCE_PARAM)malloc(sizeof(SEM_RESETRESOURCE_PARAM));
	pSemResetResourceParam->u8SemID = u8SemID;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Reset_Resource,(MS_U32*)pSemResetResourceParam);
	free(pSemResetResourceParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Reset_Resource_U2K(u8SemID);
#endif	
}

//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Get_ResourceID_U2K(MS_U8 u8SemID, MS_U16* pu16ResId)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    return HAL_SEM_Get_ResourceID(u8SemID, pu16ResId);
}

MS_BOOL MDrv_SEM_Get_ResourceID(MS_U8 u8SemID, MS_U16* pu16ResId)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_GETRESOURCEID_PARAM pSemGetResourceIDParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;	
	pSemGetResourceIDParam = (PSEM_GETRESOURCEID_PARAM)malloc(sizeof(SEM_GETRESOURCEID_PARAM));
	pSemGetResourceIDParam->u8SemID = u8SemID;
	pSemGetResourceIDParam->pu16ResId = pu16ResId;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Get_ResourceID,(MS_U32*)pSemGetResourceIDParam);
	free(pSemGetResourceIDParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Get_ResourceID_U2K(u8SemID, pu16ResId);
#endif	
}

//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------

MS_U32 MDrv_SEM_Get_Num(void)
{
    return HAL_SEM_Get_Num();
}

//-------------------------------------------------------------------------------------------------
/// 
/// 
/// 
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SEM_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_sem_version;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to lock a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @param  u32WaitMs   \b IN: 0 ~ SEM_WAIT_FOREVER: suspend time (ms) if the mutex is locked
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SEM_Lock_U2K(eSemId SemId, MS_U32 u32WaitMs)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_S16 s16SemId;
    MS_BOOL bRet;
    MS_U32  u32SysOldTime, u32Interval;

    SEM_DEBUG("Lock SemId = %d\n", SemId);
    SEM_DEBUG("Lock u32WaitMs = %ld\n", u32WaitMs);
  
    bRet = FALSE;
    s16SemId = HAL_SEM_GetSemId(SemId);
    u32SysOldTime = MsOS_GetSystemTime();
    u32Interval = 0;
    
    if(s16SemId < 0)
    {
        SEM_ERROR("Lock SemId%d invalid\n", SemId);
        return FALSE;
    }

    SEM_DEBUG("Lock s16SemId = %d\n", s16SemId);

    /*blocking*/
    if(u32WaitMs == SEM_WAIT_FOREVER) 
    {
        bRet = SEM_MUTEX_LOCK(s16SemId, MSOS_WAIT_FOREVER); 

        if(bRet == FALSE)
        {
           SEM_ERROR("Obtain mutex %s failed\n", _SEMMutexName[s16SemId]); 
        }
        else
        {
            do
            {
                bRet = HAL_SEM_Get_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);
            }while(bRet!= TRUE);

            if(bRet == FALSE)
            {
                SEM_MUTEX_UNLOCK(s16SemId);
                SEM_ERROR("Obtain hardware semaphore %d failed\n", s16SemId); 
            }
        }
    }
    /*blocking with timeout*/
    else
    {
    
        bRet = SEM_MUTEX_LOCK(s16SemId, (u32WaitMs - u32Interval)); 

        if(bRet == FALSE)
        {
           SEM_ERROR("Obtain mutex %s failed\n", _SEMMutexName[s16SemId]); 
        }
        else
        {
            do
            {
                bRet = HAL_SEM_Get_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);
                u32Interval = MsOS_GetSystemTime() - u32SysOldTime ;
            }while((bRet!= TRUE) && (u32Interval < u32WaitMs));

            if(bRet == FALSE)
            {
                SEM_MUTEX_UNLOCK(s16SemId);
                SEM_ERROR("Obtain hardware semaphore %d failed, timeout=%ld\n", s16SemId, u32WaitMs); 
            }
        }
    }

    return bRet;
}


MS_BOOL MDrv_SEM_Lock(eSemId SemId, MS_U32 u32WaitMs)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_LOCK_PARAM pSemLockParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;	
	pSemLockParam = (PSEM_LOCK_PARAM)malloc(sizeof(SEM_LOCK_PARAM));
	pSemLockParam->u32SemID = SemId;
	pSemLockParam->u32WaitMs = u32WaitMs;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Lock,(MS_U32*)pSemLockParam);
	free(pSemLockParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Lock_U2K(SemId, u32WaitMs);
#endif	
}


//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SEM_Unlock_U2K(eSemId SemId)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_S16 s16SemId;
    MS_BOOL bRet;

    bRet = FALSE;
    SEM_DEBUG("Unlock SemId = %d\n", SemId);
    s16SemId = HAL_SEM_GetSemId(SemId); 
    
    if(s16SemId < 0)
    {
        SEM_ERROR("Unlock SemId%d invalid\n", SemId);
        return FALSE;
    }

    SEM_DEBUG("Unlock s16SemId = %d\n", s16SemId);

    bRet = HAL_SEM_Free_Resource((MS_U8)s16SemId, SEM_RESOURCE_ID);

    if(bRet == FALSE)
    {
        SEM_ERROR("Release hardware semaphore %d failed\n", s16SemId); 
    }
    
    SEM_MUTEX_UNLOCK(s16SemId);

    return bRet;
}

MS_BOOL MDrv_SEM_Unlock(eSemId SemId)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_UNLOCK_PARAM pSemUnlockParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;	
	pSemUnlockParam = (PSEM_UNLOCK_PARAM)malloc(sizeof(SEM_UNLOCK_PARAM));
	pSemUnlockParam->u32SemID = SemId;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Unlock,(MS_U32*)pSemUnlockParam);
	free(pSemUnlockParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Unlock_U2K(SemId);
#endif	
}

//-------------------------------------------------------------------------------------------------
/// Attempt to delete a hardware semaphore
/// @param  SemId       \b IN: hardware semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SEM_Delete_U2K(eSemId SemId)
{
    if (!_gbSEMInitialized)
    {
        SEM_WARN("%s is called before init\n", __FUNCTION__);
        return FALSE;
    }

    MS_S16 s16SemId;
    MS_BOOL bRet;

    bRet = FALSE;
    SEM_DEBUG("Delete SemId = %d\n", SemId);
    s16SemId = HAL_SEM_GetSemId(SemId); 
    
    if(s16SemId < 0)
    {
        return FALSE;
    }

     bRet = SEM_MUTEX_UNLOCK(s16SemId);

    if(bRet == FALSE)
    {
        SEM_ERROR("Release mutex %s failed\n", _SEMMutexName[s16SemId]); 
    }
    else
    {
        bRet = HAL_SEM_Reset_Resource((MS_U8)s16SemId);

        if(bRet == FALSE)
        {
            SEM_ERROR("Reset hardware semaphore %d failed\n", s16SemId); 
        }
    }

    return bRet;
}

MS_BOOL MDrv_SEM_Delete(eSemId SemId)
{
#if (SEM_UTOPIA20)
	MS_U32 u32Ret;
	PSEM_DELETE_PARAM pSemDeleteParam = NULL;
	if (FALSE == _MDrv_SEM_CheckUtopia20Open(&pInstantSem, 0, pAttributeSEM))
		return 0;	
	pSemDeleteParam = (PSEM_DELETE_PARAM)malloc(sizeof(SEM_DELETE_PARAM));
	pSemDeleteParam->u32SemID = SemId;
	
	u32Ret = UtopiaIoctl(pInstantSem,MDrv_CMD_SEM_Delete,(MS_U32*)pSemDeleteParam);
	free(pSemDeleteParam);
	return (MS_BOOL)u32Ret;
#else
	return MDrv_SEM_Delete_U2K(SemId);
#endif	
}

MS_U32 MDrv_SEM_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS; //SUSPEND_OK
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
       
           //MDrv_SEM_Get_Resource -> Need to store the Regiser value?
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS; //RESUME_OK
        }
        else
        {
            ULOGE(TAG_SEM, "[%s,%5d]It is not suspended yet. We should not resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL; //SUSPEND_FAILED
        }
    }
    else
    {
        ULOGE(TAG_SEM, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;
}

MS_BOOL MDrv_SEM_SetDbgLevel(SEM_DbgLvl eLevel)
{
    SEM_DEBUG("Debug level: %u\n", eLevel);

    _gSEMDbgLevel = eLevel;
    return TRUE;
}
