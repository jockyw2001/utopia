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
//        including without limitation, you and yor any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitra«–cordance
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
/// file    drvDMD_ATSC.c
/// @brief  DMD ATSC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif

#include "drvDMD_ATSC.h"
#ifdef UTPA2
#include "drvDMD_ATSC_v2.h"
#endif

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#include "MsOS.h"

#include "drvMMIO.h"
#endif

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
static MS_BOOL _DEFAULT_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs)
{
    #ifdef REMOVE_HAL_INTERN_ATSC
    printf("LINK ERROR: REMOVE_HAL_INTERN_ATSC \n");
    #else
    printf("LINK ERROR: Please link ext demod library \n");
    #endif
    
    return TRUE;
}

#ifdef REMOVE_HAL_INTERN_ATSC
MS_BOOL HAL_INTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_INTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs);
#endif
MS_BOOL HAL_EXTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_EXTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#if DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN
 #define DMD_LOCK()      \
     do {                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_ATSC_Mutex == -1) return FALSE; \
        if (_u8DMD_ATSC_DbgLevel == DMD_ATSC_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_ATSC_Mutex, MSOS_WAIT_FOREVER);\
        } while(0)

 #define DMD_UNLOCK()      \
     do {                         \
        MsOS_ReleaseMutex(_s32DMD_ATSC_Mutex);\
        if (_u8DMD_ATSC_DbgLevel == DMD_ATSC_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        } while(0)
#elif (!DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN && DMD_ATSC_MULTI_THREAD_SAFE)
 #define DMD_LOCK()    do { pRes->sDMD_ATSC_InitData.LockDMD(TRUE); } while(0)
 #define DMD_UNLOCK()  do { pRes->sDMD_ATSC_InitData.LockDMD(FALSE); } while(0)
#else
 #define DMD_LOCK()    
 #define DMD_UNLOCK()  
#endif

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_U8 u8DMD_ATSC_DMD_ID = 0;

#if !DMD_ATSC_UTOPIA2_EN
DMD_ATSC_ResData sDMD_ATSC_ResData[DMD_ATSC_MAX_DEMOD_NUM] = { { {0} , {0} , {0} } };

DMD_ATSC_ResData *psDMD_ATSC_ResData = sDMD_ATSC_ResData;
#else
DMD_ATSC_ResData *psDMD_ATSC_ResData;
#endif

#ifdef UTPA2
static void* _ppATSCInstant = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
static MSIF_Version _drv_dmd_atsc_version;/* = {
    .MW = { DMD_ATSC_VER, },
};*/
#endif

#if DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN
static MS_S32 _s32DMD_ATSC_Mutex = -1;
#endif

#ifdef UTPA2
static MS_U32 _u32ATSCopen = 0;
#endif

static DMD_ATSC_DbgLv _u8DMD_ATSC_DbgLevel = DMD_ATSC_DBGLV_NONE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static DMD_ATSC_LOCK_STATUS _VSB_CheckLock(void)
{
    MS_BOOL (*ioctl)(DMD_ATSC_HAL_COMMAND eCmd, void *pPara);
    
    DMD_ATSC_ResData  *pRes  = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    DMD_ATSC_InitData *pInit = &(pRes->sDMD_ATSC_InitData);
    DMD_ATSC_Info     *pInfo = &(pRes->sDMD_ATSC_Info);
    
    ioctl = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD;
     
    if (ioctl(DMD_ATSC_HAL_CMD_Vsb_FEC_Lock, NULL))
    {
        pInfo->u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_FEC_LOCK;
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        pInfo->u32ATSCFECLockTime = MsOS_GetSystemTime();
        #else
        pInfo->u32ATSCFECLockTime = pInit->GetSystemTimeMS();
        #endif
        return DMD_ATSC_LOCK;
    }
    else //FEC UNLOCK
    {
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        if ((pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_FEC_LOCK) && ((MsOS_GetSystemTime()-pInfo->u32ATSCFECLockTime) < 100))
        #else
        if ((pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_FEC_LOCK) && ((pInit->GetSystemTimeMS()-pInfo->u32ATSCFECLockTime) < 100))
        #endif
        {
            return DMD_ATSC_LOCK;
        }
        else//Step 0 or 5
        {
            pInfo->u32ATSCLockStatus &= (~DMD_ATSC_LOCK_VSB_FEC_LOCK);
        }

        if (pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_PRE_LOCK)//Step 3,4
        {
            if (pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_VSB_FSYNC_LOCK)//Step 4
            {
                #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBFECLockCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBFECLockCheckTime)
                #endif
                {
                    return DMD_ATSC_CHECKING;
                }
            }
            else //Step 3
            {
                if (ioctl(DMD_ATSC_HAL_CMD_Vsb_FSync_Lock, NULL))
                {
                    pInfo->u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_FSYNC_LOCK;
                    
                    #ifdef MS_DEBUG
                    if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                    {
                        printf("DMD_ATSC_LOCK_VSB_FSYNC_LOCK");
                    }
                    #endif
                    return DMD_ATSC_CHECKING;
                }
                else
                {
                    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
                    if ((MsOS_GetSystemTime()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBFSyncLockCheckTime)
                    #else
                    if ((pInit->GetSystemTimeMS()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBFSyncLockCheckTime)
                    #endif
                    {
                        return DMD_ATSC_CHECKING;
                    }
                }
            }
        }
        else //Step 1,2
        {
            if (ioctl(DMD_ATSC_HAL_CMD_Vsb_PreLock, NULL))
            {
                pInfo->u32ATSCLockStatus |= DMD_ATSC_LOCK_VSB_PRE_LOCK;
                
                #ifdef MS_DEBUG
                if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                {
                    printf("DMD_ATSC_LOCK_VSB_PRE_LOCK\n");
                }
                #endif
                return DMD_ATSC_CHECKING;
            }
            else
            {
                #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBPreLockCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32ATSCScanTimeStart) < pInit->u16VSBPreLockCheckTime)
                #endif
                {
                    return DMD_ATSC_CHECKING;
                }
            }
        }
        return DMD_ATSC_UNLOCK;
    }
}

static DMD_ATSC_LOCK_STATUS _QAM_CheckLock(void)
{
    MS_BOOL (*ioctl)(DMD_ATSC_HAL_COMMAND eCmd, void *pPara);
    
    DMD_ATSC_ResData  *pRes  = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    DMD_ATSC_InitData *pInit = &(pRes->sDMD_ATSC_InitData);
    DMD_ATSC_Info     *pInfo = &(pRes->sDMD_ATSC_Info);
    
    ioctl = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD;
    
    if (ioctl(DMD_ATSC_HAL_CMD_QAM_Main_Lock, NULL))
    {
        pInfo->u32ATSCLockStatus |= DMD_ATSC_LOCK_QAM_MAIN_LOCK;
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        pInfo->u32ATSCFECLockTime = MsOS_GetSystemTime();
        #else
        pInfo->u32ATSCFECLockTime = pInit->GetSystemTimeMS();
        #endif
        return DMD_ATSC_LOCK;
    }
    else
    {
        #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
        if ((pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_QAM_MAIN_LOCK) && ((MsOS_GetSystemTime()-pInfo->u32ATSCFECLockTime) < 100))
        #else
        if ((pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_QAM_MAIN_LOCK) && ((pInit->GetSystemTimeMS()-pInfo->u32ATSCFECLockTime) < 100))
        #endif
        {
            return DMD_ATSC_LOCK;
        }
        else
        {
            pInfo->u32ATSCLockStatus &= (~DMD_ATSC_LOCK_QAM_MAIN_LOCK);
        }

        if (pInfo->u32ATSCLockStatus & DMD_ATSC_LOCK_QAM_PRE_LOCK)//STEP 3
        {
            #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
            if ((MsOS_GetSystemTime()-pInfo->u32ATSCScanTimeStart) < pInit->u16QAMMainLockCheckTime)
            #else
            if ((pInit->GetSystemTimeMS()-pInfo->u32ATSCScanTimeStart) < pInit->u16QAMMainLockCheckTime)
            #endif
            {
                return DMD_ATSC_CHECKING;
            }
        }
        else //STEP 1,2
        {
            if (ioctl(DMD_ATSC_HAL_CMD_QAM_PreLock, NULL))
            {
                pInfo->u32ATSCLockStatus |= DMD_ATSC_LOCK_QAM_PRE_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_DEBUG)
                {
                    printf("DMD_ATSC_LOCK_QAM_PRE_LOCK\n");
                }
                #endif
                return DMD_ATSC_CHECKING;
            }
            else
            {
                #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32ATSCScanTimeStart) < pInit->u16QAMPreLockCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32ATSCScanTimeStart) < pInit->u16QAMPreLockCheckTime)
                #endif
                {
                    return DMD_ATSC_CHECKING;
                }
            }
        }
        return DMD_ATSC_UNLOCK;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel)
#else
MS_BOOL MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel)
#endif
{
    _u8DMD_ATSC_DbgLevel = u8DbgLevel;
    
    return TRUE;
}

#ifdef UTPA2
DMD_ATSC_Info* _MDrv_DMD_ATSC_GetInfo(void)
#else
DMD_ATSC_Info* MDrv_DMD_ATSC_GetInfo(void)
#endif
{
    psDMD_ATSC_ResData->sDMD_ATSC_Info.u8Version = 0;
    
    return &(psDMD_ATSC_ResData->sDMD_ATSC_Info);
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion)
#else
MS_BOOL MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion)
#endif
{
    if (!ppVersion) return FALSE;
    
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    *ppVersion = &_drv_dmd_atsc_version;
    #else
    *ppVersion = NULL;
    #endif
    
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_ATSC_Init(DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen)
{
    return MDrv_DMD_ATSC_MD_Init(0, pDMD_ATSC_InitData, u32InitDataLen);
}

MS_BOOL MDrv_DMD_ATSC_Exit(void)
{
    return MDrv_DMD_ATSC_MD_Exit(0);
}

MS_U32 MDrv_DMD_ATSC_GetConfig(DMD_ATSC_InitData *psDMD_ATSC_InitData)  
{
    return MDrv_DMD_ATSC_MD_GetConfig(0, psDMD_ATSC_InitData);
}

MS_BOOL MDrv_DMD_ATSC_SetConfig(DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    return MDrv_DMD_ATSC_MD_SetConfig(0, eType, bEnable);
}

MS_BOOL MDrv_DMD_ATSC_SetReset(void)
{
    return MDrv_DMD_ATSC_MD_SetReset(0);
}

MS_BOOL MDrv_DMD_ATSC_Set_QAM_SR(DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate)
{
    return MDrv_DMD_ATSC_MD_Set_QAM_SR(0, eType, symbol_rate);
}

MS_BOOL MDrv_DMD_ATSC_SetActive(MS_BOOL bEnable)
{
    return MDrv_DMD_ATSC_MD_SetActive(0, bEnable);
}

#if DMD_ATSC_STR_EN
MS_U32 MDrv_DMD_ATSC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    return MDrv_DMD_ATSC_MD_SetPowerState(0, u16PowerState);
}
#endif

DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_GetLock(DMD_ATSC_GETLOCK_TYPE eType)
{
    return MDrv_DMD_ATSC_MD_GetLock(0, eType);
}

DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_GetModulationMode(void)
{
    return MDrv_DMD_ATSC_MD_GetModulationMode(0);
}

MS_BOOL MDrv_DMD_ATSC_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_ATSC_MD_GetSignalStrength(0, u16Strength);
}

DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_GetSignalQuality(void)
{
    return MDrv_DMD_ATSC_MD_GetSignalQuality(0);
}

MS_U8 MDrv_DMD_ATSC_GetSNRPercentage(void)
{
    return MDrv_DMD_ATSC_MD_GetSNRPercentage(0);
}

MS_BOOL MDrv_DMD_ATSC_GET_QAM_SNR(float *f_snr)
{
    return MDrv_DMD_ATSC_MD_GET_QAM_SNR(0, f_snr);
}

MS_BOOL MDrv_DMD_ATSC_Read_uCPKT_ERR(MS_U16 *u16PacketErr)
{
    return MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(0, u16PacketErr);
}

MS_BOOL MDrv_DMD_ATSC_GetPreViterbiBer(float *ber)
{
    return MDrv_DMD_ATSC_MD_GetPreViterbiBer(0, ber);
}

MS_BOOL MDrv_DMD_ATSC_GetPostViterbiBer(float *ber)
{
    return MDrv_DMD_ATSC_MD_GetPostViterbiBer(0, ber);
}

MS_BOOL MDrv_DMD_ATSC_ReadFrequencyOffset(MS_S16 *cfo)
{
    return MDrv_DMD_ATSC_MD_ReadFrequencyOffset(0, cfo);
}

MS_BOOL MDrv_DMD_ATSC_SetSerialControl(MS_U8 u8TsConfigData)
{
    return MDrv_DMD_ATSC_MD_SetSerialControl(0, u8TsConfigData);
}

MS_BOOL MDrv_DMD_ATSC_IIC_BYPASS_MODE(MS_BOOL bEnable)
{
    return MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(0, bEnable);
}

MS_BOOL MDrv_DMD_ATSC_SWITCH_SSPI_GPIO(MS_BOOL bEnable)
{
    return MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(0, bEnable);
}

MS_BOOL MDrv_DMD_ATSC_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    return MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_ATSC_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    return MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_ATSC_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    return MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(0, u8Pin, bEnableOut);
}

MS_BOOL MDrv_DMD_ATSC_DoIQSwap(MS_BOOL bIsQPad)
{
    return MDrv_DMD_ATSC_MD_DoIQSwap(0, bIsQPad);
}

MS_BOOL MDrv_DMD_ATSC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_ATSC_MD_GetReg(0, u16Addr, pu8Data);
}

MS_BOOL MDrv_DMD_ATSC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_ATSC_MD_SetReg(0, u16Addr, u8Data);
}
#endif // #ifndef MSOS_TYPE_LINUX_KERNEL

////////////////////////////////////////////////////////////////////////////////
///                      BACKWARD COMPATIBLE API                             ///
////////////////////////////////////////////////////////////////////////////////

#ifndef UTPA2
MS_BOOL MDrv_DMD_ATSC_SEL_DMD(eDMD_SEL eDMD_NUM)
{
    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_LoadFW(eDMD_SEL DMD_NUM)
{
    return TRUE;
}

MS_BOOL MDrv_DMD_ATSC_SetVsbMode(void)
{   
    return MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_VSB, TRUE);
}

MS_BOOL MDrv_DMD_ATSC_Set256QamMode(void)
{
    return MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_256QAM, TRUE);
}

MS_BOOL MDrv_DMD_ATSC_Set64QamMode(void)
{
    return MDrv_DMD_ATSC_MD_SetConfig(0, DMD_ATSC_DEMOD_ATSC_64QAM, TRUE);
}
#endif // #ifndef UTPA2

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                               ///
////////////////////////////////////////////////////////////////////////////////

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_Init(MS_U8 id, DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen)
#else
MS_BOOL MDrv_DMD_ATSC_MD_Init(MS_U8 id, DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRFAGCTristateEnable = 0;
    MS_PHY phyNonPMBankSize;
    
    #if DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN
    if (_s32DMD_ATSC_Mutex == -1)
    {
        if ((_s32DMD_ATSC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD ATSC", MSOS_PROCESS_SHARED)) == -1)
        {
            DMD_DBG(printf("MDrv_DMD_ATSC_Init Create Mutex Fail\n"));
            return FALSE;
        }
    }
    #elif (!DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN && DMD_ATSC_MULTI_THREAD_SAFE)
    if (!pRes->sDMD_ATSC_InitData.CreateMutex(TRUE))
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init Create Mutex Fail\n"));
        return FALSE;
    }
    #endif
    
    DMD_LOCK();
    if (pRes->sDMD_ATSC_PriData.bInit)
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init more than once\n"));
        return FALSE;
    }
    
    #ifdef MS_DEBUG
    if (_u8DMD_ATSC_DbgLevel >= DMD_ATSC_DBGLV_INFO)
    {
        printf("MDrv_DMD_ATSC_Init\n");
    }
    #endif
    
    #ifndef MSOS_TYPE_LINUX_KERNEL
    if (sizeof(DMD_ATSC_InitData) == u32InitDataLen)
    {
        memcpy(&(pRes->sDMD_ATSC_InitData), pDMD_ATSC_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_ATSC_Init input data structure incorrect\n"));
        return FALSE;
    }
    #else
    memcpy(&(pRes->sDMD_ATSC_InitData), pDMD_ATSC_InitData, sizeof(DMD_ATSC_InitData));
    #endif
    
    if (!MDrv_MMIO_GetBASE(&(pRes->sDMD_ATSC_PriData.virtDMDBaseAddr), &phyNonPMBankSize, MS_MODULE_PM))
    {
        #ifdef MS_DEBUG
        printf("HAL_DMD_RegInit failure to get MS_MODULE_PM\n");
        #endif
        return FALSE;
    }
    
    pRes->sDMD_ATSC_PriData.bInit = TRUE;
    
    #if DMD_ATSC_STR_EN
    pRes->sDMD_ATSC_PriData.eLastState = E_POWER_RESUME;
    #endif
    
    pRes->sDMD_ATSC_PriData.eLastType  = DMD_ATSC_DEMOD_ATSC_VSB;
    pRes->sDMD_ATSC_PriData.u16SymRate = 10762;
    
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    if (pRes->sDMD_ATSC_InitData.bIsExtDemod)
        pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD = HAL_EXTERN_ATSC_IOCTL_CMD;
    else pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD = HAL_INTERN_ATSC_IOCTL_CMD;
    #else
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD = HAL_EXTERN_ATSC_IOCTL_CMD;
    #endif
    
    u8DMD_ATSC_DMD_ID = id;
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_InitClk, &bRFAGCTristateEnable);
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Download, NULL);
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_FWVERSION, NULL);
    DMD_UNLOCK();
    
    return TRUE;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_Exit(MS_U8 id)
#else
MS_BOOL MDrv_DMD_ATSC_MD_Exit(MS_U8 id)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Exit, NULL);
    
    pRes->sDMD_ATSC_PriData.bInit = FALSE;
    DMD_UNLOCK();
    
    #if DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN
    MsOS_DeleteMutex(_s32DMD_ATSC_Mutex);
    
    _s32DMD_ATSC_Mutex = -1;
    #elif (!DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN && DMD_ATSC_MULTI_THREAD_SAFE)
    pRes->sDMD_ATSC_InitData.CreateMutex(FALSE);
    #endif
    
    return bRet;
}

#ifdef UTPA2                  
MS_U32 _MDrv_DMD_ATSC_MD_GetConfig(MS_U8 id, DMD_ATSC_InitData *psDMD_ATSC_InitData)
#else
MS_U32 MDrv_DMD_ATSC_MD_GetConfig(MS_U8 id, DMD_ATSC_InitData *psDMD_ATSC_InitData)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;

    DMD_LOCK();
    memcpy (psDMD_ATSC_InitData, &(pRes->sDMD_ATSC_InitData), sizeof(DMD_ATSC_InitData));
	DMD_UNLOCK();
		
	return UTOPIA_STATUS_SUCCESS;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SetConfig(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SetConfig(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_SoftReset, NULL);
    
    if (bEnable)
    {
        switch (eType)
        {
            case DMD_ATSC_DEMOD_ATSC_VSB:
                pRes->sDMD_ATSC_PriData.eLastType = DMD_ATSC_DEMOD_ATSC_VSB;
                pRes->sDMD_ATSC_PriData.u16SymRate = 10762;
                bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_SetVsbMode, NULL);
                break;
            case DMD_ATSC_DEMOD_ATSC_64QAM:
                pRes->sDMD_ATSC_PriData.eLastType = DMD_ATSC_DEMOD_ATSC_64QAM;
                pRes->sDMD_ATSC_PriData.u16SymRate = 5056;
                bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Set64QamMode, NULL);
                break;
            case DMD_ATSC_DEMOD_ATSC_256QAM:
                pRes->sDMD_ATSC_PriData.eLastType = DMD_ATSC_DEMOD_ATSC_256QAM;
                pRes->sDMD_ATSC_PriData.u16SymRate = 5360;
                bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Set256QamMode, NULL);
                break;
            default:
                pRes->sDMD_ATSC_PriData.eLastType = DMD_ATSC_DEMOD_NULL;
                pRes->sDMD_ATSC_PriData.u16SymRate = 0;
                bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_SetModeClean, NULL);
                break;
        }
    }
    
    #if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
    pRes->sDMD_ATSC_Info.u32ATSCScanTimeStart = MsOS_GetSystemTime();
    #else
    pRes->sDMD_ATSC_Info.u32ATSCScanTimeStart = psDMD_ATSC_ResData->sDMD_ATSC_InitData.GetSystemTimeMS();
    #endif
    pRes->sDMD_ATSC_Info.u32ATSCLockStatus    = 0;
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SetReset(MS_U8 id)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SetReset(MS_U8 id)
#endif
{
    return TRUE;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_Set_QAM_SR(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate)
#else
MS_BOOL MDrv_DMD_ATSC_MD_Set_QAM_SR(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    pRes->sDMD_ATSC_PriData.eLastType  = eType;
    pRes->sDMD_ATSC_PriData.u16SymRate = symbol_rate;
    
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Set_QAM_SR, NULL);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
#endif
{
    return TRUE;
}

#if DMD_ATSC_STR_EN
#ifdef UTPA2
MS_U32 _MDrv_DMD_ATSC_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#else
MS_U32 MDrv_DMD_ATSC_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    
    if (u16PowerState == E_POWER_SUSPEND)
    {
        pRes->sDMD_ATSC_PriData.bDownloaded = FALSE;
        pRes->sDMD_ATSC_PriData.bIsDTV      = pRes->sDMD_ATSC_PriData.bInit;
        pRes->sDMD_ATSC_PriData.eLastState  = u16PowerState;
        
        if (pRes->sDMD_ATSC_PriData.bInit)
        {
            #ifdef UTPA2
            _MDrv_DMD_ATSC_MD_Exit(id);
            #else
            MDrv_DMD_ATSC_MD_Exit(id);
            #endif
        }
        
        u32Return = UTOPIA_STATUS_SUCCESS;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (pRes->sDMD_ATSC_PriData.eLastState == E_POWER_SUSPEND)
        {
        	printf("\nVT: (Check Mode In DRV:) DTV Mode=%d\n", pRes->sDMD_ATSC_PriData.bIsDTV);
        	
            if (pRes->sDMD_ATSC_PriData.bIsDTV)
            {
                #ifdef UTPA2
                _MDrv_DMD_ATSC_MD_Init(id, &(pRes->sDMD_ATSC_InitData), sizeof(DMD_ATSC_InitData));
                _MDrv_DMD_ATSC_MD_DoIQSwap(id, pRes->sDMD_ATSC_PriData.bIsQPad);
                #else
                MDrv_DMD_ATSC_MD_Init(id, &(pRes->sDMD_ATSC_InitData), sizeof(DMD_ATSC_InitData));
                MDrv_DMD_ATSC_MD_DoIQSwap(id, pRes->sDMD_ATSC_PriData.bIsQPad);
                #endif
                
                if (pRes->sDMD_ATSC_PriData.eLastType != DMD_ATSC_DEMOD_ATSC_VSB)
                {
                    if (pRes->sDMD_ATSC_PriData.eLastType == DMD_ATSC_DEMOD_ATSC_64QAM &&
                        pRes->sDMD_ATSC_PriData.u16SymRate == 5056)
                    {
                        #ifdef UTPA2
                        _MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                        #else
                        MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                        #endif
                    }
                    else if (pRes->sDMD_ATSC_PriData.eLastType == DMD_ATSC_DEMOD_ATSC_256QAM &&
                             pRes->sDMD_ATSC_PriData.u16SymRate == 5360)
                    {
                        #ifdef UTPA2
                        _MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                        #else
                        MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                        #endif
                    }
                    else
                    {
                        #ifdef UTPA2
                        _MDrv_DMD_ATSC_MD_Set_QAM_SR(id, pRes->sDMD_ATSC_PriData.eLastType,
                                                    pRes->sDMD_ATSC_PriData.u16SymRate);
                        #else
                        MDrv_DMD_ATSC_MD_Set_QAM_SR(id, pRes->sDMD_ATSC_PriData.eLastType,
                                                    pRes->sDMD_ATSC_PriData.u16SymRate);
                        #endif
                    }
                }
                else
                {
                    #ifdef UTPA2
                    _MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                    #else
                    MDrv_DMD_ATSC_MD_SetConfig(id, pRes->sDMD_ATSC_PriData.eLastType, TRUE);
                    #endif
                }
            }
            
            pRes->sDMD_ATSC_PriData.eLastState = u16PowerState;
            
            u32Return = UTOPIA_STATUS_SUCCESS;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            
            u32Return = UTOPIA_STATUS_FAIL;  
	    }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__, u16PowerState);
    	
    	u32Return = UTOPIA_STATUS_FAIL;
    }
    
    return u32Return;
}
#endif

#ifdef UTPA2
DMD_ATSC_LOCK_STATUS _MDrv_DMD_ATSC_MD_GetLock(MS_U8 id, DMD_ATSC_GETLOCK_TYPE eType)
#else
DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_MD_GetLock(MS_U8 id, DMD_ATSC_GETLOCK_TYPE eType)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_LOCK_STATUS status  = DMD_ATSC_UNLOCK;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    switch (eType)
    {
        case DMD_ATSC_GETLOCK:
            switch (pRes->sDMD_ATSC_PriData.eLastType)
            {
                case DMD_ATSC_DEMOD_ATSC_VSB:
                    status = _VSB_CheckLock();
                    break;
                case DMD_ATSC_DEMOD_ATSC_16QAM:
                case DMD_ATSC_DEMOD_ATSC_32QAM:
                case DMD_ATSC_DEMOD_ATSC_64QAM:
                case DMD_ATSC_DEMOD_ATSC_128QAM:
                case DMD_ATSC_DEMOD_ATSC_256QAM:
                    status = _QAM_CheckLock();
                    break;
                default:
                    status = DMD_ATSC_UNLOCK;
                    break;
            }
            break;
        case DMD_ATSC_GETLOCK_VSB_AGCLOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_AGCLock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_VSB_PRELOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Vsb_PreLock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_VSB_FSYNCLOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Vsb_FSync_Lock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
        	break;
        case DMD_ATSC_GETLOCK_VSB_CELOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Vsb_CE_Lock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_VSB_FECLOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Vsb_FEC_Lock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_QAM_AGCLOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_AGCLock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_QAM_PRELOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_QAM_PreLock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        case DMD_ATSC_GETLOCK_QAM_MAINLOCK:
            status = (pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_QAM_Main_Lock, NULL)) ? DMD_ATSC_LOCK : DMD_ATSC_UNLOCK;
            break;
        default:
            status = DMD_ATSC_UNLOCK;
            break;
    }
    DMD_UNLOCK();
    
    return status;
}

#ifdef UTPA2
DMD_ATSC_DEMOD_TYPE _MDrv_DMD_ATSC_MD_GetModulationMode(MS_U8 id)
#else
DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_MD_GetModulationMode(MS_U8 id)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_DEMOD_TYPE eType = DMD_ATSC_DEMOD_NULL;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_Check8VSB64_256QAM, &eType);
    DMD_UNLOCK();
    
    return eType;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_ReadIFAGC, u16Strength);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
DMD_ATSC_SIGNAL_CONDITION _MDrv_DMD_ATSC_MD_GetSignalQuality(MS_U8 id)
#else
DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_MD_GetSignalQuality(MS_U8 id)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_SIGNAL_CONDITION eSignal = DMD_ATSC_SIGNAL_NO;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_CheckSignalCondition, &eSignal);
    DMD_UNLOCK();
    
    return eSignal;
}

#ifdef UTPA2
MS_U8 _MDrv_DMD_ATSC_MD_GetSNRPercentage(MS_U8 id)
#else
MS_U8 MDrv_DMD_ATSC_MD_GetSNRPercentage(MS_U8 id)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_U8 percentage = 0;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_ReadSNRPercentage, &percentage);
    DMD_UNLOCK();
    
    return percentage;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, DMD_ATSC_SNR_DATA *f_snr)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, float *f_snr)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GET_QAM_SNR, f_snr);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(MS_U8 id, MS_U16 *u16PacketErr)
#else
MS_BOOL MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(MS_U8 id, MS_U16 *u16PacketErr)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_ReadPKTERR, u16PacketErr);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, float *ber)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GetPreViterbiBer, ber);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, DMD_ATSC_BER_DATA *ber)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, float *ber)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GetPostViterbiBer, ber);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, DMD_ATSC_CFO_DATA *cfo)
#else
MS_BOOL MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, MS_S16 *cfo)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_ReadFrequencyOffset, cfo);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK(); 
    pRes->sDMD_ATSC_InitData.u1TsConfigByte_SerialMode = u8TsConfigData;
    pRes->sDMD_ATSC_InitData.u1TsConfigByte_DataSwap   = u8TsConfigData >> 1;
    pRes->sDMD_ATSC_InitData.u1TsConfigByte_ClockInv   = u8TsConfigData >> 2;
    pRes->sDMD_ATSC_InitData.u5TsConfigByte_DivNum     = u8TsConfigData >> 3;
    
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_TS_INTERFACE_CONFIG, NULL);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_IIC_Bypass_Mode, &bEnable);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_SSPI_TO_GPIO, &bEnable);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;
    
    sPin.u8Pin = u8Pin;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GPIO_GET_LEVEL, &sPin);
    DMD_UNLOCK();
    
    *bLevel = sPin.bLevel; 
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;
    
    sPin.u8Pin  = u8Pin;
    sPin.bLevel = bLevel;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GPIO_SET_LEVEL, &sPin);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;
    
    sPin.u8Pin  = u8Pin;
    sPin.bIsOut = bEnableOut;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GPIO_OUT_ENABLE, &sPin);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#else
MS_BOOL MDrv_DMD_ATSC_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    MS_BOOL bRet = TRUE;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_DoIQSwap, &bIsQPad);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#else
MS_BOOL MDrv_DMD_ATSC_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_REG_DATA reg;
    MS_BOOL bRet = TRUE;
    
    reg.u16Addr = u16Addr;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_GET_REG, &reg);
    DMD_UNLOCK();
    
    *pu8Data = reg.u8Data;
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ATSC_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#else
MS_BOOL MDrv_DMD_ATSC_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#endif
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + id;
    DMD_ATSC_REG_DATA reg;
    MS_BOOL bRet = TRUE;
    
    reg.u16Addr = u16Addr;
    reg.u8Data  = u8Data;
    
    DMD_LOCK();
    u8DMD_ATSC_DMD_ID = id;
    
    bRet = pRes->sDMD_ATSC_PriData.HAL_DMD_ATSC_IOCTL_CMD(DMD_ATSC_HAL_CMD_SET_REG, &reg);
    DMD_UNLOCK();
    
    return bRet;
}

#ifdef UTPA2

////////////////////////////////////////////////////////////////////////////////
///                              UTOPIA 2K API                               ///
////////////////////////////////////////////////////////////////////////////////
#ifndef MSOS_TYPE_LINUX_KERNEL

#ifndef MSOS_TYPE_LINUX
static const float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static const float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#endif // #ifndef MSOS_TYPE_LINUX

MS_BOOL MDrv_DMD_ATSC_SetDbgLevel(DMD_ATSC_DbgLv u8DbgLevel)
{
    ATSC_DBG_LEVEL_PARAM DbgLevelParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    DbgLevelParam.u8DbgLevel = u8DbgLevel;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_SetDbgLevel, &DbgLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DMD_ATSC_Info* MDrv_DMD_ATSC_GetInfo(void)
{
    ATSC_GET_INFO_PARAM GetInfoParam = {0};
    
    if (!_u32ATSCopen) return NULL;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_GetInfo, &GetInfoParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetInfoParam.pInfo;
    }
    else
    {
        return NULL;
    }
}

MS_BOOL MDrv_DMD_ATSC_GetLibVer(const MSIF_Version **ppVersion)
{
    ATSC_GET_LIB_VER_PARAM GetLibVerParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_GetLibVer, &GetLibVerParam) == UTOPIA_STATUS_SUCCESS)
    {
        //*ppVersion = *GetLibVerParam.ppVersion;
        *ppVersion = NULL;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_Init(MS_U8 id, DMD_ATSC_InitData *pDMD_ATSC_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;
    ATSC_INIT_PARAM InitParam = {0};
    
    if (_u32ATSCopen == 0)
    {
        if (UtopiaOpen(MODULE_ATSC /*| KERNEL_MODE*/, &_ppATSCInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
            _u32ATSCopen = 1;
        else
        {
            return FALSE;
        }
    } 

    if (!_u32ATSCopen) return FALSE;
    
    InitParam.id = id;
    InitParam.pDMD_ATSC_InitData = pDMD_ATSC_InitData;
    InitParam.u32InitDataLen = u32InitDataLen;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_Init, &InitParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    } 
}

MS_BOOL MDrv_DMD_ATSC_MD_Exit(MS_U8 id)
{
    ATSC_ID_PARAM IdParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    IdParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_Exit, &IdParam) == UTOPIA_STATUS_SUCCESS)
    {
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32 MDrv_DMD_ATSC_MD_GetConfig(MS_U8 id, DMD_ATSC_InitData *psDMD_ATSC_InitData)  
{
	ATSC_INIT_PARAM InitParam = {0};
		
	if (!_u32ATSCopen) return FALSE;
	  
	InitParam.id = id;
	InitParam.pDMD_ATSC_InitData = psDMD_ATSC_InitData;
	  
	if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetConfig, &InitParam) == UTOPIA_STATUS_SUCCESS)
  	{
  	    return UTOPIA_STATUS_SUCCESS;
  	}
  	else
	{
		return UTOPIA_STATUS_ERR_NOT_AVAIL;
	}
}

MS_BOOL MDrv_DMD_ATSC_MD_SetConfig(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    ATSC_SET_CONFIG_PARAM SetConfigParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SetConfigParam.id = id;
    SetConfigParam.eType = eType;
    SetConfigParam.bEnable = bEnable;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetConfig, &SetConfigParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_SetReset(MS_U8 id)
{
    ATSC_ID_PARAM IdParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    IdParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetReset, &IdParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_Set_QAM_SR(MS_U8 id, DMD_ATSC_DEMOD_TYPE eType, MS_U16 symbol_rate)
{
    ATSC_SET_QAM_SR_PARAM SetQamSrParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SetQamSrParam.id = id;
    SetQamSrParam.eType = eType;
    SetQamSrParam.symbol_rate = symbol_rate;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_Set_QAM_SR, &SetQamSrParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
{
    ATSC_SET_ACTIVE_PARAM SetActiveParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SetActiveParam.id = id;
    SetActiveParam.bEnable = bEnable;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetActive, &SetActiveParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if DMD_ATSC_STR_EN
MS_U32 MDrv_DMD_ATSC_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
{
    ATSC_SET_POWER_STATE_PARAM SetPowerStateParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SetPowerStateParam.id = id;
    SetPowerStateParam.u16PowerState = u16PowerState;
    
    return UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetPowerState, &SetPowerStateParam);
}
#endif

DMD_ATSC_LOCK_STATUS MDrv_DMD_ATSC_MD_GetLock(MS_U8 id, DMD_ATSC_GETLOCK_TYPE eType)
{
    ATSC_GET_LOCK_PARAM GetLockParam = {0};
    
    if (!_u32ATSCopen) return DMD_ATSC_NULL;
    
    GetLockParam.id = id;
    GetLockParam.eType = eType;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetLockParam.status;
    }
    else
    {
        return DMD_ATSC_NULL;
    }
}

DMD_ATSC_DEMOD_TYPE MDrv_DMD_ATSC_MD_GetModulationMode(MS_U8 id)
{
    ATSC_GET_MODULATION_MODE_PARAM GetModulationModeParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetModulationModeParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetModulationModeParam.eType;
    }
    else
    {
        return DMD_ATSC_DEMOD_NULL;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
{
    ATSC_GET_SIGNAL_STRENGTH_PARAM GetSignalStrengthParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetSignalStrengthParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetSignalStrength, &GetSignalStrengthParam) == UTOPIA_STATUS_SUCCESS)
    {
        *u16Strength = GetSignalStrengthParam.u16Strength;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DMD_ATSC_SIGNAL_CONDITION MDrv_DMD_ATSC_MD_GetSignalQuality(MS_U8 id)
{
    ATSC_GET_SIGNAL_QUALITY_PARAM GetSignalQualityParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetSignalQualityParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetSignalQuality, &GetSignalQualityParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetSignalQualityParam.eQuality;
    }
    else
    {
        return DMD_ATSC_SIGNAL_NO;
    }
}

MS_U8 MDrv_DMD_ATSC_MD_GetSNRPercentage(MS_U8 id)
{
    ATSC_GET_SNR_PERCENTAGE_PARAM GetSnrPercentageParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetSnrPercentageParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetSNRPercentage, &GetSnrPercentageParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetSnrPercentageParam.u8Percentage;
    }
    else
    {
        return 0;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GET_QAM_SNR(MS_U8 id, float *f_snr)
{
    ATSC_GET_SNR_PARAM GetSnrParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetSnrParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GET_QAM_SNR, &GetSnrParam) == UTOPIA_STATUS_SUCCESS)
    {
        #ifdef MSOS_TYPE_LINUX
        *f_snr = 10.0f*log10f(((float)GetSnrParam.snr.sym_num)/(float)(GetSnrParam.snr.noisepower+1));
        #else
        *f_snr = 10.0f*Log10Approx(((float)GetSnrParam.snr.sym_num)/(float)(GetSnrParam.snr.noisepower+1));
        #endif
        
        return TRUE;
    }
    else
    {
        *f_snr = 0;
        
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_Read_uCPKT_ERR(MS_U8 id, MS_U16 *u16PacketErr)
{
    ATSC_GET_UCPKT_ERR_PARAM GetUcpktErrParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetUcpktErrParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_Read_uCPKT_ERR, &GetUcpktErrParam) == UTOPIA_STATUS_SUCCESS)
    {
        *u16PacketErr = GetUcpktErrParam.u16PacketErr;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GetPreViterbiBer(MS_U8 id, float *ber)
{
    ATSC_GET_BER_PARAM GetBerParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetBerParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetPreViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
    {
        *ber = 0;
        
        return TRUE;
    }
    else
    {
        *ber = 0;
        
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GetPostViterbiBer(MS_U8 id, float *ber)
{
    ATSC_GET_BER_PARAM GetBerParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GetBerParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetPostViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
    {
        if (!GetBerParam.ber.BitErr && !GetBerParam.ber.Error_window && !GetBerParam.ber.Win_unit)
            *ber = 0;
        else if (GetBerParam.ber.BitErr <=0 )
            *ber = 0.5f / ((float)(GetBerParam.ber.Error_window)*(GetBerParam.ber.Win_unit));
        else *ber = (float)(GetBerParam.ber.BitErr) / ((float)(GetBerParam.ber.Error_window)*(GetBerParam.ber.Win_unit));
        
        return TRUE;
    }
    else
    {
        *ber = 0;
        
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_ReadFrequencyOffset(MS_U8 id, MS_S16 *cfo)
{
    ATSC_READ_FREQ_OFFSET_PARAM ReadFreqOffsetParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    ReadFreqOffsetParam.id = id;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_ReadFrequencyOffset, &ReadFreqOffsetParam) == UTOPIA_STATUS_SUCCESS)
    {
        if ((ReadFreqOffsetParam.cfo.Mode & 0x7F) == DMD_ATSC_DEMOD_ATSC_VSB)
        {
            *cfo = ((float)(-(ReadFreqOffsetParam.cfo.FF))*0.04768);
            
            if ((MS_U8)(ReadFreqOffsetParam.cfo.Rate) == 0x07)
                *cfo = *cfo - 100;
            else if ((MS_U8)(ReadFreqOffsetParam.cfo.Rate) == 0x08)
                *cfo = *cfo - 500;
        }
        else if (ReadFreqOffsetParam.cfo.Mode & 0x80)
        {
            if ((ReadFreqOffsetParam.cfo.Mode & 0x7F) == DMD_ATSC_DEMOD_ATSC_256QAM)
                *cfo = (float)((ReadFreqOffsetParam.cfo.FF)*0.0000199); //5.360537E6/2^28*1000
            else if ((ReadFreqOffsetParam.cfo.Mode & 0x7F) == DMD_ATSC_DEMOD_ATSC_64QAM)
                *cfo = (float)((ReadFreqOffsetParam.cfo.FF)*0.0000188); //5.056941E6/2^21*1000
            else *cfo = 0;
            
            *cfo = *cfo + (float)((ReadFreqOffsetParam.cfo.Rate)-0x3A07)/330.13018; //(0.001/25.41*2^27/16)???
        }
        else
        {
            if ((ReadFreqOffsetParam.cfo.Mode & 0x7F) == DMD_ATSC_DEMOD_ATSC_256QAM)
                *cfo = (float)((ReadFreqOffsetParam.cfo.FF)*0.0025561); //5.360537E6/2^21*1000
            else if ((ReadFreqOffsetParam.cfo.Mode & 0x7F) == DMD_ATSC_DEMOD_ATSC_64QAM)
                *cfo = (float)((ReadFreqOffsetParam.cfo.FF)*0.00241134); //5.056941E6/2^21*1000
            else *cfo = 0;
            
            *cfo = *cfo + (float)((ReadFreqOffsetParam.cfo.Rate)-0x3D70)/2.62144; //(0.001/25*2^20/16)
        }
        
        return TRUE;
    }
    else
    {
        *cfo = 0;
        
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
{
    ATSC_SET_SERIAL_CONTROL_PARAM SetSerialControlParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SetSerialControlParam.id = id;
    SetSerialControlParam.u8TsConfigData = u8TsConfigData;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetSerialControl, &SetSerialControlParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
{
    ATSC_IIC_BYPASS_MODE_PARAM IicBypassModeParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    IicBypassModeParam.id = id;
    IicBypassModeParam.bEnable = bEnable;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_IIC_BYPASS_MODE, &IicBypassModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
{
    ATSC_SWITCH_SSPI_GPIO_PARAM SwitchSspiGpioParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    SwitchSspiGpioParam.id = id;
    SwitchSspiGpioParam.bEnable = bEnable;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SWITCH_SSPI_GPIO, &SwitchSspiGpioParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
{
    ATSC_GPIO_LEVEL_PARAM GpioLevelParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GPIO_GET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        *bLevel = GpioLevelParam.bLevel;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
{
    ATSC_GPIO_LEVEL_PARAM GpioLevelParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;
    GpioLevelParam.bLevel = bLevel;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GPIO_SET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    ATSC_GPIO_OUT_ENABLE_PARAM GpioOutEnableParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    GpioOutEnableParam.id = id;
    GpioOutEnableParam.u8Pin = u8Pin;
    GpioOutEnableParam.bEnableOut = bEnableOut;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GPIO_OUT_ENABLE, &GpioOutEnableParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
{
    ATSC_DO_IQ_SWAP_PARAM DoIQSwapParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    DoIQSwapParam.id = id;
    DoIQSwapParam.bIsQPad = bIsQPad;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_DoIQSwap, &DoIQSwapParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    ATSC_REG_PARAM RegParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    RegParam.id = id;
    RegParam.u16Addr = u16Addr;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_GetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
    {
        *pu8Data = RegParam.u8Data;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ATSC_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
{
    ATSC_REG_PARAM RegParam = {0};
    
    if (!_u32ATSCopen) return FALSE;
    
    RegParam.id = id;
    RegParam.u16Addr = u16Addr;
    RegParam.u8Data = u8Data;
    
    if (UtopiaIoctl(_ppATSCInstant, DMD_ATSC_DRV_CMD_MD_SetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif // #ifndef MSOS_TYPE_LINUX_KERNEL

#endif // #ifdef UTPA2
