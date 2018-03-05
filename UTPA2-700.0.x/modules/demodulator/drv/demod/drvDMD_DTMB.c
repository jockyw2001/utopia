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
/// file    drvDMD_DTMB.c
/// @brief  DMD DTMB Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#ifdef UTPA2
#undef DONT_USE_CMA
#define DONT_USE_CMA
#endif

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

#include "drvDMD_DTMB.h"
#ifdef UTPA2
#include "drvDMD_DTMB_v2.h"
#endif

#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
#include "MsOS.h"
#include "drvMMIO.h"
#endif

#ifndef DONT_USE_CMA
#ifdef MSOS_TYPE_LINUX_KERNEL
#include "drvCMAPool_v2.h"
#else
#include "drvCMAPool.h"
#endif //#ifdef MSOS_TYPE_LINUX_KERNEL
#include "halCHIP.h"
#endif //#ifndef DONT_USE_CMA

#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
static MS_BOOL _DEFAULT_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs)
{
    #ifdef REMOVE_HAL_INTERN_DTMB
    printf("LINK ERROR: REMOVE_HAL_INTERN_DTMB \n");
    #else
    printf("LINK ERROR: Please link ext demod library \n");
    #endif

    return TRUE;
}

#ifdef REMOVE_HAL_INTERN_DTMB
MS_BOOL HAL_INTERN_DTMB_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_INTERN_DTMB_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs);
#endif
MS_BOOL HAL_EXTERN_DTMB_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_EXTERN_DTMB_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#if DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN
 #define DMD_LOCK()      \
     do {                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_DTMB_Mutex == -1) return FALSE; \
        if (_u8DMD_DTMB_DbgLevel == DMD_DTMB_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DTMB_Mutex, MSOS_WAIT_FOREVER);\
        } while(0)

 #define DMD_UNLOCK()      \
     do {                         \
        MsOS_ReleaseMutex(_s32DMD_DTMB_Mutex);\
        if (_u8DMD_DTMB_DbgLevel == DMD_DTMB_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        } while(0)
#elif (!DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN && DMD_DTMB_MULTI_THREAD_SAFE)
 #define DMD_LOCK()    do { pRes->sDMD_DTMB_InitData.LockDMD(TRUE); } while(0)
 #define DMD_UNLOCK()  do { pRes->sDMD_DTMB_InitData.LockDMD(FALSE); } while(0)
#else
 #define DMD_LOCK()
 #define DMD_UNLOCK()
#endif

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          // (x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_U8 u8DMD_DTMB_DMD_ID = 0;

#if !DMD_DTMB_UTOPIA2_EN
DMD_DTMB_ResData sDMD_DTMB_ResData[DMD_DTMB_MAX_DEMOD_NUM] = { { {0} , {0} , {0} } };

DMD_DTMB_ResData *psDMD_DTMB_ResData = sDMD_DTMB_ResData;
#else
DMD_DTMB_ResData *psDMD_DTMB_ResData;
#endif

#ifdef UTPA2
static void* _ppDTMBInstant = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
static MSIF_Version _drv_dmd_dtmb_version;/* = {
    .MW = { DMD_DTMB_VER, },
};*/
#endif

#if DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN
static MS_S32 _s32DMD_DTMB_Mutex = -1;
#endif

#ifndef DONT_USE_CMA
static struct CMA_Pool_Init_Param  _DTMB_CMA_Pool_Init_PARAM; // for MApi_CMA_Pool_Init
static struct CMA_Pool_Alloc_Param _DTMB_CMA_Alloc_PARAM;     // for MApi_CMA_Pool_GetMem
static struct CMA_Pool_Free_Param  _DTMB_CMA_Free_PARAM;      // for MApi_CMA_Pool_PutMem
#endif

#ifdef UTPA2
static MS_U32 _u32DTMBopen = 0;
#endif

static DMD_DTMB_DbgLv _u8DMD_DTMB_DbgLevel = DMD_DTMB_DBGLV_NONE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static DMD_DTMB_LOCK_STATUS _DTMB_CheckLock(void)
{
    MS_BOOL (*ioctl)(DMD_DTMB_HAL_COMMAND eCmd, void *pPara);

    DMD_DTMB_ResData  *pRes  = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;
    DMD_DTMB_InitData *pInit = &(pRes->sDMD_DTMB_InitData);
    DMD_DTMB_Info     *pInfo = &(pRes->sDMD_DTMB_Info);

    ioctl = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD;

    if (ioctl(DMD_DTMB_HAL_CMD_DTMB_FEC_Lock, NULL))
    {
        pInfo->u32DTMBLockStatus |= DMD_DTMB_LOCK_DTMB_FEC_LOCK;
        #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
        pInfo->u32DTMBFECLockTime = MsOS_GetSystemTime();
        #else
        pInfo->u32DTMBFECLockTime = pInit->GetSystemTimeMS();
        #endif
        return DMD_DTMB_LOCK;
    }
    else
    {
        #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
        if ((pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_FEC_LOCK) && ((MsOS_GetSystemTime()-pInfo->u32DTMBFECLockTime) < 500))
        #else
        if ((pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_FEC_LOCK) && ((pInit->GetSystemTimeMS()-pInfo->u32DTMBFECLockTime) < 500))
        #endif
        {
            return DMD_DTMB_LOCK;
        }
        else
        {
            pInfo->u32DTMBLockStatus &= (~DMD_DTMB_LOCK_DTMB_FEC_LOCK);
        }

        if (pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DTMB_PNP_LOCK)//STEP 3
        {
            #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
            if ((MsOS_GetSystemTime()-pInfo->u32DTMBScanTimeStart) < pInit->u16DTMBFECLockCheckTime)
            #else
            if ((pInit->GetSystemTimeMS()-pInfo->u32DTMBScanTimeStart) < pInit->u16DTMBFECLockCheckTime)
            #endif
            {
                return DMD_DTMB_CHECKING;
            }
        }
        else //STEP 1,2
        {
            if (ioctl(DMD_DTMB_HAL_CMD_DTMB_PNP_Lock, NULL))
            {
                pInfo->u32DTMBLockStatus |= DMD_DTMB_LOCK_DTMB_PNP_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_DEBUG)
                {
                    printf("DMD_DTMB_LOCK_DTMB_PNP_LOCK\n");
                }
                #endif
                return DMD_DTMB_CHECKING;
            }
            else
            {
                #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32DTMBScanTimeStart) < pInit->u16DTMBPNMLockCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32DTMBScanTimeStart) < pInit->u16DTMBPNMLockCheckTime)
                #endif
                {
                    return DMD_DTMB_CHECKING;
                }
            }
        }
        return DMD_DTMB_UNLOCK;
    }
}

static DMD_DTMB_LOCK_STATUS _DVBC_CheckLock(void)
{
    MS_BOOL (*ioctl)(DMD_DTMB_HAL_COMMAND eCmd, void *pPara);

    DMD_DTMB_ResData  *pRes  = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;
    DMD_DTMB_InitData *pInit = &(pRes->sDMD_DTMB_InitData);
    DMD_DTMB_Info     *pInfo = &(pRes->sDMD_DTMB_Info);

    ioctl = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD;

    if (ioctl(DMD_DTMB_HAL_CMD_DVBC_Main_Lock, NULL))
    {
        pInfo->u32DTMBLockStatus |= DMD_DTMB_LOCK_DVBC_MAIN_LOCK;
        #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
        pInfo->u32DTMBFECLockTime = MsOS_GetSystemTime();
        #else
        pInfo->u32DTMBFECLockTime = pInit->GetSystemTimeMS();
        #endif
        return DMD_DTMB_LOCK;
    }
    else
    {
        #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
        if ((pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DVBC_MAIN_LOCK) && ((MsOS_GetSystemTime()-pInfo->u32DTMBFECLockTime) < 100))
        #else
        if ((pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DVBC_MAIN_LOCK) && ((pInit->GetSystemTimeMS()-pInfo->u32DTMBFECLockTime) < 100))
        #endif
        {
            return DMD_DTMB_LOCK;
        }
        else
        {
            pInfo->u32DTMBLockStatus &= (~DMD_DTMB_LOCK_DVBC_MAIN_LOCK);
        }

        if (pInfo->u32DTMBLockStatus & DMD_DTMB_LOCK_DVBC_PRE_LOCK)//STEP 3
        {
            #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
            if ((MsOS_GetSystemTime()-pInfo->u32DTMBScanTimeStart) < pInit->u16QAMMainLockCheckTime)
            #else
            if ((pInit->GetSystemTimeMS()-pInfo->u32DTMBScanTimeStart) < pInit->u16QAMMainLockCheckTime)
            #endif
            {
                return DMD_DTMB_CHECKING;
            }
        }
        else //STEP 1,2
        {
            if (ioctl(DMD_DTMB_HAL_CMD_DVBC_PreLock, NULL))
            {
                pInfo->u32DTMBLockStatus |= DMD_DTMB_LOCK_DVBC_PRE_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_DEBUG)
                {
                    printf("DMD_DTMB_LOCK_DVBC_PRE_LOCK\n");
                }
                #endif
                return DMD_DTMB_CHECKING;
            }
            else
            {
                #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32DTMBScanTimeStart) < pInit->u16QAMPreLockCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32DTMBScanTimeStart) < pInit->u16QAMPreLockCheckTime)
                #endif
                {
                    return DMD_DTMB_CHECKING;
                }
            }
        }
        return DMD_DTMB_UNLOCK;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_SetDbgLevel(DMD_DTMB_DbgLv u8DbgLevel)
#else
MS_BOOL MDrv_DMD_DTMB_SetDbgLevel(DMD_DTMB_DbgLv u8DbgLevel)
#endif
{
    _u8DMD_DTMB_DbgLevel = u8DbgLevel;

    return TRUE;
}

#ifdef UTPA2
DMD_DTMB_Info* _MDrv_DMD_DTMB_GetInfo(void)
#else
DMD_DTMB_Info* MDrv_DMD_DTMB_GetInfo(void)
#endif
{
    psDMD_DTMB_ResData->sDMD_DTMB_Info.u8Version = 0;

    return &(psDMD_DTMB_ResData->sDMD_DTMB_Info);
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_GetLibVer(const MSIF_Version **ppVersion)
#else
MS_BOOL MDrv_DMD_DTMB_GetLibVer(const MSIF_Version **ppVersion)
#endif
{
    if (!ppVersion) return FALSE;

    #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
    *ppVersion = &_drv_dmd_dtmb_version;
    #else
    *ppVersion = NULL;
    #endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DTMB_Init(DMD_DTMB_InitData *pDMD_DTMB_InitData, MS_U32 u32InitDataLen)
{
    return MDrv_DMD_DTMB_MD_Init(0, pDMD_DTMB_InitData, u32InitDataLen);
}

MS_BOOL MDrv_DMD_DTMB_Exit(void)
{
    return MDrv_DMD_DTMB_MD_Exit(0);
}

MS_U32 MDrv_DMD_DTMB_GetConfig(DMD_DTMB_InitData *psDMD_DTMB_InitData)
{
    return MDrv_DMD_DTMB_MD_GetConfig(0, psDMD_DTMB_InitData);
}

MS_BOOL MDrv_DMD_DTMB_SetConfig(DMD_DTMB_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    return MDrv_DMD_DTMB_MD_SetConfig(0, eType, bEnable);
}

MS_BOOL MDrv_DMD_DTMB_SetReset(void)
{
    return MDrv_DMD_DTMB_MD_SetReset(0);
}

MS_BOOL MDrv_DMD_DTMB_Set_QAM_SR(DMD_DTMB_DEMOD_TYPE eType, MS_U16 symbol_rate)
{
    return MDrv_DMD_DTMB_MD_Set_QAM_SR(0, eType, symbol_rate);
}

MS_BOOL MDrv_DMD_DTMB_SetActive(MS_BOOL bEnable)
{
    return MDrv_DMD_DTMB_MD_SetActive(0, bEnable);
}

#if DMD_DTMB_STR_EN
MS_U32 MDrv_DMD_DTMB_SetPowerState(EN_POWER_MODE u16PowerState)
{
    return MDrv_DMD_DTMB_MD_SetPowerState(0, u16PowerState);
}
#endif

DMD_DTMB_LOCK_STATUS MDrv_DMD_DTMB_GetLock(DMD_DTMB_GETLOCK_TYPE eType)
{
    return MDrv_DMD_DTMB_MD_GetLock(0, eType);
}

MS_BOOL MDrv_DMD_DTMB_GetModulationMode(DMD_DTMB_MODULATION_INFO *sDtmbModulationMode)
{
    return MDrv_DMD_DTMB_MD_GetModulationMode(0, sDtmbModulationMode);
}

MS_BOOL MDrv_DMD_DTMB_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DTMB_MD_GetSignalStrength(0, u16Strength);
}

MS_BOOL MDrv_DMD_DTMB_ReadFrequencyOffset(MS_S16 *cfo)
{
    return MDrv_DMD_DTMB_MD_ReadFrequencyOffset(0, cfo);
}

MS_U8 MDrv_DMD_DTMB_GetSignalQuality(void)
{
    return MDrv_DMD_DTMB_MD_GetSignalQuality(0);
}

MS_BOOL MDrv_DMD_DTMB_GetPreLdpcBer(float *pber)
{
    return MDrv_DMD_DTMB_MD_GetPreLdpcBer(0, pber);
}

MS_BOOL MDrv_DMD_DTMB_GetPreViterbiBer(float *ber)
{
    return MDrv_DMD_DTMB_MD_GetPreViterbiBer(0, ber);
}

MS_BOOL MDrv_DMD_DTMB_GetPostViterbiBer(float *ber)
{
    return MDrv_DMD_DTMB_MD_GetPostViterbiBer(0, ber);
}

MS_BOOL MDrv_DMD_DTMB_GetSNR(float *snr)
{
    return MDrv_DMD_DTMB_MD_GetSNR(0, snr);
}

MS_BOOL MDrv_DMD_DTMB_SetSerialControl(MS_U8 u8TsConfigData)
{
    return MDrv_DMD_DTMB_MD_SetSerialControl(0, u8TsConfigData);
}

MS_BOOL MDrv_DMD_DTMB_IIC_BYPASS_MODE(MS_BOOL bEnable)
{
    return MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE(0, bEnable);
}

MS_BOOL MDrv_DMD_DTMB_SWITCH_SSPI_GPIO(MS_BOOL bEnable)
{
    return MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO(0, bEnable);
}

MS_BOOL MDrv_DMD_DTMB_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    return MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_DTMB_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    return MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_DTMB_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    return MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE(0, u8Pin, bEnableOut);
}

MS_BOOL MDrv_DMD_DTMB_DoIQSwap(MS_BOOL bIsQPad)
{
    return MDrv_DMD_DTMB_MD_DoIQSwap(0, bIsQPad);
}

MS_BOOL MDrv_DMD_DTMB_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_DTMB_MD_GetReg(0, u16Addr, pu8Data);
}

MS_BOOL MDrv_DMD_DTMB_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_DTMB_MD_SetReg(0, u16Addr, u8Data);
}
#endif // #ifndef MSOS_TYPE_LINUX_KERNEL

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                               ///
////////////////////////////////////////////////////////////////////////////////

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_Init(MS_U8 id, DMD_DTMB_InitData *pDMD_DTMB_InitData, MS_U32 u32InitDataLen)
#else
MS_BOOL MDrv_DMD_DTMB_MD_Init(MS_U8 id, DMD_DTMB_InitData *pDMD_DTMB_InitData, MS_U32 u32InitDataLen)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRFAGCTristateEnable = 0;
    MS_VIRT virtNonPMBank;
    MS_PHY phyNonPMBankSize;
    #ifndef DONT_USE_CMA
    MS_U32  u32PhysicalAddr_FromVA = 0, u32HeapStartPAAddress = 0;
    #endif

    #if DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN
    if (_s32DMD_DTMB_Mutex == -1)
    {
        if ((_s32DMD_DTMB_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD DTMB", MSOS_PROCESS_SHARED)) == -1)
        {
            DMD_DBG(printf("MDrv_DMD_DTMB_Init Create Mutex Fail\n"));
            return FALSE;
        }
    }
    #elif (!DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN && DMD_DTMB_MULTI_THREAD_SAFE)
    if (!pRes->sDMD_DTMB_InitData.CreateMutex(TRUE))
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init Create Mutex Fail\n"));
        return FALSE;
    }
    #endif

    DMD_LOCK();
    if (pRes->sDMD_DTMB_PriData.bInit)
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init more than once\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DTMB_DbgLevel >= DMD_DTMB_DBGLV_INFO)
    {
        printf("MDrv_DMD_DTMB_Init\n");
    }
    #endif

    if (!MDrv_MMIO_GetBASE(&virtNonPMBank, &phyNonPMBankSize, MS_MODULE_PM))
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init Get MS_MODULE_PM Fail\n"));
        return FALSE;
    }

    pRes->sDMD_DTMB_PriData.virtDMDBaseAddr = virtNonPMBank;

    #ifndef MSOS_TYPE_LINUX_KERNEL
    if (sizeof(DMD_DTMB_InitData) == u32InitDataLen)
    {
        memcpy(&(pRes->sDMD_DTMB_InitData), pDMD_DTMB_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DTMB_Init input data structure incorrect\n"));
        return FALSE;
    }
    #else
    memcpy(&(pRes->sDMD_DTMB_InitData), pDMD_DTMB_InitData, sizeof(DMD_DTMB_InitData));
    #endif

    pRes->sDMD_DTMB_PriData.bInit = TRUE;

    #ifndef DONT_USE_CMA
    _DTMB_CMA_Pool_Init_PARAM.heap_id = 26;
    _DTMB_CMA_Pool_Init_PARAM.flags = CMA_FLAG_MAP_VMA;

    if (MApi_CMA_Pool_Init(&_DTMB_CMA_Pool_Init_PARAM) == FALSE)
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get MApi_CMA_Pool_Init ERROR!!!\n", __PRETTY_FUNCTION__, __LINE__));
    }
    else
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Pool_Init_PARAM.pool_handle_id));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Pool_Init_PARAM.miu));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get heap_miu_start_offset is 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Pool_Init_PARAM.heap_miu_start_offset));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Pool_Init_PARAM.heap_length));
    }

    _miu_offset_to_phy(_DTMB_CMA_Pool_Init_PARAM.miu, _DTMB_CMA_Pool_Init_PARAM.heap_miu_start_offset, u32HeapStartPAAddress);

	_DTMB_CMA_Alloc_PARAM.pool_handle_id = _DTMB_CMA_Pool_Init_PARAM.pool_handle_id;
    _DTMB_CMA_Alloc_PARAM.offset_in_pool = (pRes->sDMD_DTMB_InitData.u32TdiStartAddr * 16) - u32HeapStartPAAddress;	// offset_in_pool means the start_ask_addr with respect to the pool_start_addr(BA) => this will specify the start_ask_ba_addr
    _DTMB_CMA_Alloc_PARAM.length = 0x500000UL;
    _DTMB_CMA_Alloc_PARAM.flags = CMA_FLAG_VIRT_ADDR;										// we will get a start_va with respect to the start_ask_ba_addr if using CMA_FLAG_VIRT_ADDR flag

    if(MApi_CMA_Pool_GetMem(&_DTMB_CMA_Alloc_PARAM) == FALSE)
	{
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, CMA_Pool_GetMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
	}
	else
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get from heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Alloc_PARAM.pool_handle_id));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask offset 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Alloc_PARAM.offset_in_pool));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask length 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Alloc_PARAM.length));
        DMD_DBG(printf("\033[37mFunction = %s, Line = %d, return va is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Alloc_PARAM.virt_addr));
  	}

    u32PhysicalAddr_FromVA = MsOS_MPool_VA2PA(_DTMB_CMA_Alloc_PARAM.virt_addr);

    DMD_DBG(printf("#######u32PhysicalAddr_FromVA1 = [0x%x]\n", u32PhysicalAddr_FromVA));

    _miu_offset_to_phy(_DTMB_CMA_Pool_Init_PARAM.miu,
                       _DTMB_CMA_Pool_Init_PARAM.heap_miu_start_offset + _DTMB_CMA_Alloc_PARAM.offset_in_pool,
                       u32PhysicalAddr_FromVA);

    DMD_DBG(printf("#######u32PhysicalAddr_FromVA2 = [0x%x]\n", u32PhysicalAddr_FromVA));

    DMD_DBG(printf("#######pRes->sDMD_DTMB_InitData.u32TdiStartAddr = [0x%x]\n", (pRes->sDMD_DTMB_InitData.u32TdiStartAddr * 16)));
    #endif // #ifndef DONT_USE_CMA

    #if DMD_DTMB_STR_EN
    pRes->sDMD_DTMB_PriData.eLastState = E_POWER_RESUME;
    #endif

    pRes->sDMD_DTMB_PriData.eLastType  = DMD_DTMB_DEMOD_DTMB;
    pRes->sDMD_DTMB_PriData.u16SymRate = 7560;

    #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
    if (pRes->sDMD_DTMB_InitData.bIsExtDemod)
        pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD = HAL_EXTERN_DTMB_IOCTL_CMD;
    else pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD = HAL_INTERN_DTMB_IOCTL_CMD;
    #else
    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD = HAL_EXTERN_DTMB_IOCTL_CMD;
    #endif

    u8DMD_DTMB_DMD_ID = id;

    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_InitClk, &bRFAGCTristateEnable);

    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_Download, NULL);

    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_TS_INTERFACE_CONFIG, NULL);

    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_FWVERSION, NULL);
    DMD_UNLOCK();

    return TRUE;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_Exit(MS_U8 id)
#else
MS_BOOL MDrv_DMD_DTMB_MD_Exit(MS_U8 id)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_Exit, NULL);

    pRes->sDMD_DTMB_PriData.bInit = FALSE;

    #ifndef DONT_USE_CMA
    _DTMB_CMA_Free_PARAM.pool_handle_id = _DTMB_CMA_Pool_Init_PARAM.pool_handle_id;
    _DTMB_CMA_Free_PARAM.offset_in_pool = _DTMB_CMA_Alloc_PARAM.offset_in_pool;
    _DTMB_CMA_Free_PARAM.length = 0x500000UL;;

    if(MApi_CMA_Pool_PutMem(&_DTMB_CMA_Free_PARAM) == FALSE)
    {
      	 DMD_DBG(printf("\033[35mFunction = %s, Line = %d, MsOS_CMA_Pool_Release ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
    }
    else
    {
      	 DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get from heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Free_PARAM.pool_handle_id));
      	 DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask offset 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Free_PARAM.offset_in_pool));
         DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask length 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _DTMB_CMA_Free_PARAM.length));
    }
    #endif

    DMD_UNLOCK();

    #if DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN
    MsOS_DeleteMutex(_s32DMD_DTMB_Mutex);

    _s32DMD_DTMB_Mutex = -1;
    #elif (!DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN && DMD_DTMB_MULTI_THREAD_SAFE)
    pRes->sDMD_DTMB_InitData.CreateMutex(FALSE);
    #endif

    return bRet;
}

#ifdef UTPA2
MS_U32 _MDrv_DMD_DTMB_MD_GetConfig(MS_U8 id, DMD_DTMB_InitData *psDMD_DTMB_InitData)
#else
MS_U32 MDrv_DMD_DTMB_MD_GetConfig(MS_U8 id, DMD_DTMB_InitData *psDMD_DTMB_InitData)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;

    DMD_LOCK();
    memcpy (psDMD_DTMB_InitData, &(pRes->sDMD_DTMB_InitData), sizeof(DMD_DTMB_InitData));
    DMD_UNLOCK();

    return UTOPIA_STATUS_SUCCESS;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SetConfig(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SetConfig(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_BOOL bEnable)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SoftReset, NULL);

    if (bEnable)
    {
        switch (eType)
        {
            case DMD_DTMB_DEMOD_DTMB:
            case DMD_DTMB_DEMOD_DTMB_7M:
            case DMD_DTMB_DEMOD_DTMB_5M:
                if (eType != pRes->sDMD_DTMB_PriData.eLastType)
                {
                    pRes->sDMD_DTMB_PriData.bDownloaded = FALSE;
                    pRes->sDMD_DTMB_PriData.eLastType = DMD_DTMB_DEMOD_DTMB;
                    pRes->sDMD_DTMB_PriData.u16SymRate = 7560;
                    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_Download, NULL);
                    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DoIQSwap, &(pRes->sDMD_DTMB_PriData.bIsQPad));
                    //pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SetACICoef, NULL);
                }
                bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SetDTMBMode, NULL);
                break;
            case DMD_DTMB_DEMOD_DTMB_6M:
                if (eType != pRes->sDMD_DTMB_PriData.eLastType)
                {
                    pRes->sDMD_DTMB_PriData.bDownloaded = FALSE;
                    pRes->sDMD_DTMB_PriData.eLastType = DMD_DTMB_DEMOD_DTMB_6M;
                    pRes->sDMD_DTMB_PriData.u16SymRate = 5670;
                    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_Download, NULL);
                    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DoIQSwap, &(pRes->sDMD_DTMB_PriData.bIsQPad));
                    //pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SetACICoef, NULL);
                }
                bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SetDTMBMode, NULL);
                break;
            default:
                pRes->sDMD_DTMB_PriData.eLastType = DMD_DTMB_DEMOD_NULL;
                pRes->sDMD_DTMB_PriData.u16SymRate = 0;
                bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SetModeClean, NULL);
                break;
        }
    }

    #if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
    pRes->sDMD_DTMB_Info.u32DTMBScanTimeStart = MsOS_GetSystemTime();
    #else
    pRes->sDMD_DTMB_Info.u32DTMBScanTimeStart = psDMD_DTMB_ResData->sDMD_DTMB_InitData.GetSystemTimeMS();
    #endif
    pRes->sDMD_DTMB_Info.u32DTMBLockStatus    = 0;
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SetReset(MS_U8 id)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SetReset(MS_U8 id)
#endif
{
    return TRUE;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_Set_QAM_SR(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_U16 symbol_rate)
#else
MS_BOOL MDrv_DMD_DTMB_MD_Set_QAM_SR(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_U16 symbol_rate)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    pRes->sDMD_DTMB_PriData.eLastType  = eType;
    pRes->sDMD_DTMB_PriData.u16SymRate = symbol_rate;

    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_Set_QAM_SR, NULL);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
#endif
{
    return TRUE;
}

#if DMD_DTMB_STR_EN
#ifdef UTPA2
MS_U32 _MDrv_DMD_DTMB_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#else
MS_U32 MDrv_DMD_DTMB_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        pRes->sDMD_DTMB_PriData.bDownloaded = FALSE;
        pRes->sDMD_DTMB_PriData.bIsDTV      = pRes->sDMD_DTMB_PriData.bInit;
        pRes->sDMD_DTMB_PriData.eLastState  = u16PowerState;

        if (pRes->sDMD_DTMB_PriData.bInit)
        {
            #ifdef UTPA2
            _MDrv_DMD_DTMB_MD_Exit(id);
            #else
            MDrv_DMD_DTMB_MD_Exit(id);
            #endif
        }

        u32Return = UTOPIA_STATUS_SUCCESS;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (pRes->sDMD_DTMB_PriData.eLastState == E_POWER_SUSPEND)
        {
        	printf("\nVT: (Check Mode In DRV:) DTV Mode=%d\n", pRes->sDMD_DTMB_PriData.bIsDTV);

            if (pRes->sDMD_DTMB_PriData.bIsDTV)
            {
                #ifdef UTPA2
                _MDrv_DMD_DTMB_MD_Init(id, &(pRes->sDMD_DTMB_InitData), sizeof(DMD_DTMB_InitData));
                _MDrv_DMD_DTMB_MD_DoIQSwap(id, pRes->sDMD_DTMB_PriData.bIsQPad);
                #else
                MDrv_DMD_DTMB_MD_Init(id, &(pRes->sDMD_DTMB_InitData), sizeof(DMD_DTMB_InitData));
                MDrv_DMD_DTMB_MD_DoIQSwap(id, pRes->sDMD_DTMB_PriData.bIsQPad);
                #endif

                if (pRes->sDMD_DTMB_PriData.eLastType != DMD_DTMB_DEMOD_DTMB &&
                    pRes->sDMD_DTMB_PriData.eLastType != DMD_DTMB_DEMOD_DTMB_7M &&
                    pRes->sDMD_DTMB_PriData.eLastType != DMD_DTMB_DEMOD_DTMB_6M &&
                    pRes->sDMD_DTMB_PriData.eLastType != DMD_DTMB_DEMOD_DTMB_5M)
                {
                    #ifdef UTPA2
                    _MDrv_DMD_DTMB_MD_Set_QAM_SR(id, pRes->sDMD_DTMB_PriData.eLastType,pRes->sDMD_DTMB_PriData.u16SymRate);
                    #else
                    MDrv_DMD_DTMB_MD_Set_QAM_SR(id, pRes->sDMD_DTMB_PriData.eLastType,pRes->sDMD_DTMB_PriData.u16SymRate);
                    #endif
                }
                else
                {
                    #ifdef UTPA2
                    _MDrv_DMD_DTMB_MD_SetConfig(id, pRes->sDMD_DTMB_PriData.eLastType, TRUE);
                    #else
                    MDrv_DMD_DTMB_MD_SetConfig(id, pRes->sDMD_DTMB_PriData.eLastType, TRUE);
                    #endif
                }
            }

            pRes->sDMD_DTMB_PriData.eLastState = u16PowerState;

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
DMD_DTMB_LOCK_STATUS _MDrv_DMD_DTMB_MD_GetLock(MS_U8 id, DMD_DTMB_GETLOCK_TYPE eType)
#else
DMD_DTMB_LOCK_STATUS MDrv_DMD_DTMB_MD_GetLock(MS_U8 id, DMD_DTMB_GETLOCK_TYPE eType)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_LOCK_STATUS status  = DMD_DTMB_UNLOCK;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    switch (eType)
    {
        case DMD_DTMB_GETLOCK:
            switch (pRes->sDMD_DTMB_PriData.eLastType)
            {
                case DMD_DTMB_DEMOD_DTMB:
                case DMD_DTMB_DEMOD_DTMB_7M:
                case DMD_DTMB_DEMOD_DTMB_6M:
                case DMD_DTMB_DEMOD_DTMB_5M:
                    status = _DTMB_CheckLock();
                    break;
                case DMD_DTMB_DEMOD_DVBC_16QAM:
                case DMD_DTMB_DEMOD_DVBC_32QAM:
                case DMD_DTMB_DEMOD_DVBC_64QAM:
                case DMD_DTMB_DEMOD_DVBC_128QAM:
                case DMD_DTMB_DEMOD_DVBC_256QAM:
                    status = _DVBC_CheckLock();
                    break;
                default:
                    status = DMD_DTMB_UNLOCK;
                    break;
            }
            break;
        case DMD_DTMB_GETLOCK_DTMB_AGCLOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_DTMB_PNPLOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DTMB_PNP_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_DTMB_FECLOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DTMB_FEC_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
        	break;
        case DMD_DTMB_GETLOCK_DVBC_AGCLOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_AGCLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_DVBC_PRELOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DVBC_PreLock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        case DMD_DTMB_GETLOCK_DVBC_MAINLOCK:
            status = (pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DVBC_Main_Lock, NULL)) ? DMD_DTMB_LOCK : DMD_DTMB_UNLOCK;
            break;
        default:
            status = DMD_DTMB_UNLOCK;
            break;
    }
    DMD_UNLOCK();

    return status;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetModulationMode(MS_U8 id, DMD_DTMB_MODULATION_INFO *sDtmbModulationMode)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetModulationMode(MS_U8 id, DMD_DTMB_MODULATION_INFO *sDtmbModulationMode)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GetModulation, sDtmbModulationMode);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_ReadIFAGC, u16Strength);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_ReadFrequencyOffset(MS_U8 id, DMD_DTMB_CFO_DATA *cfo)
#else
MS_BOOL MDrv_DMD_DTMB_MD_ReadFrequencyOffset(MS_U8 id, MS_S16 *cfo)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_ReadFrequencyOffset, cfo);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_U8 _MDrv_DMD_DTMB_MD_GetSignalQuality(MS_U8 id)
#else
MS_U8 MDrv_DMD_DTMB_MD_GetSignalQuality(MS_U8 id)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_U8 percentage = 0;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_ReadSNRPercentage, &percentage);
    DMD_UNLOCK();

    return percentage;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetPreLdpcBer(MS_U8 id, DMD_DTMB_BER_DATA *pber)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetPreLdpcBer(MS_U8 id, float *pber)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GetPreLdpcBer, pber);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetPreViterbiBer(MS_U8 id, DMD_DTMB_BER_DATA *ber)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetPreViterbiBer(MS_U8 id, float *ber)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GetPreViterbiBer, ber);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetPostViterbiBer(MS_U8 id, DMD_DTMB_BER_DATA *ber)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetPostViterbiBer(MS_U8 id, float *ber)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GetPostViterbiBer, ber);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetSNR(MS_U8 id, DMD_DTMB_SNR_DATA *snr)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetSNR(MS_U8 id, float *snr)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GetSNR, snr);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    pRes->sDMD_DTMB_InitData.u1TsConfigByte_SerialMode = u8TsConfigData;
    pRes->sDMD_DTMB_InitData.u1TsConfigByte_DataSwap   = u8TsConfigData >> 1;
    pRes->sDMD_DTMB_InitData.u1TsConfigByte_ClockInv   = u8TsConfigData >> 2;
    pRes->sDMD_DTMB_InitData.u5TsConfigByte_DivNum     = u8TsConfigData >> 3;

    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_TS_INTERFACE_CONFIG, NULL);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_IIC_Bypass_Mode, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SSPI_TO_GPIO, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin = u8Pin;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GPIO_GET_LEVEL, &sPin);
    DMD_UNLOCK();

    *bLevel = sPin.bLevel;

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin  = u8Pin;
    sPin.bLevel = bLevel;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GPIO_SET_LEVEL, &sPin);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin  = u8Pin;
    sPin.bIsOut = bEnableOut;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GPIO_OUT_ENABLE, &sPin);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#else
MS_BOOL MDrv_DMD_DTMB_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_DoIQSwap, &bIsQPad);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#else
MS_BOOL MDrv_DMD_DTMB_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_REG_DATA reg;
    MS_BOOL bRet = TRUE;

    reg.u16Addr = u16Addr;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_GET_REG, &reg);
    DMD_UNLOCK();

    *pu8Data = reg.u8Data;

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_DTMB_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#else
MS_BOOL MDrv_DMD_DTMB_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + id;
    DMD_DTMB_REG_DATA reg;
    MS_BOOL bRet = TRUE;

    reg.u16Addr = u16Addr;
    reg.u8Data  = u8Data;

    DMD_LOCK();
    u8DMD_DTMB_DMD_ID = id;

    bRet = pRes->sDMD_DTMB_PriData.HAL_DMD_DTMB_IOCTL_CMD(DMD_DTMB_HAL_CMD_SET_REG, &reg);
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
#endif

MS_BOOL MDrv_DMD_DTMB_SetDbgLevel(DMD_DTMB_DbgLv u8DbgLevel)
{
    DTMB_DBG_LEVEL_PARAM DbgLevelParam = {0};

    if (!_u32DTMBopen) return FALSE;

    DbgLevelParam.u8DbgLevel = u8DbgLevel;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_SetDbgLevel, &DbgLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DMD_DTMB_Info* MDrv_DMD_DTMB_GetInfo(void)
{
    DTMB_GET_INFO_PARAM GetInfoParam = {0};

    if (!_u32DTMBopen) return NULL;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_GetInfo, &GetInfoParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetInfoParam.pInfo;
    }
    else
    {
        return NULL;
    }
}

MS_BOOL MDrv_DMD_DTMB_GetLibVer(const MSIF_Version **ppVersion)
{
    DTMB_GET_LIB_VER_PARAM GetLibVerParam = {0};

    if (!_u32DTMBopen) return FALSE;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_GetLibVer, &GetLibVerParam) == UTOPIA_STATUS_SUCCESS)
    {
        //*ppVersion = *(GetLibVerParam.ppVersion);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_Init(MS_U8 id, DMD_DTMB_InitData *pDMD_DTMB_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;
    DTMB_INIT_PARAM InitParam = {0};

    if (_u32DTMBopen == 0)
    {
        if (UtopiaOpen(MODULE_DTMB, &_ppDTMBInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
            _u32DTMBopen = 1;
        else
        {
            return FALSE;
        }
    }

    if (!_u32DTMBopen) return FALSE;

    InitParam.id = id;
    InitParam.pDMD_DTMB_InitData = pDMD_DTMB_InitData;
    InitParam.u32InitDataLen = u32InitDataLen;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_Init, &InitParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_Exit(MS_U8 id)
{
    DTMB_ID_PARAM IdParam = {0};

    if (!_u32DTMBopen) return FALSE;

    IdParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_Exit, &IdParam) == UTOPIA_STATUS_SUCCESS)
    {

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32 MDrv_DMD_DTMB_MD_GetConfig(MS_U8 id, DMD_DTMB_InitData *psDMD_DTMB_InitData)
{
    DTMB_INIT_PARAM InitParam = {0};

	if (!_u32DTMBopen) return FALSE;

	InitParam.id = id;
    InitParam.pDMD_DTMB_InitData = psDMD_DTMB_InitData;

	if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetConfig, &InitParam) == UTOPIA_STATUS_SUCCESS)
	{
  	    return UTOPIA_STATUS_SUCCESS;
  	}
  	else
  	{
  	    return UTOPIA_STATUS_ERR_NOT_AVAIL;
  	}
}

MS_BOOL MDrv_DMD_DTMB_MD_SetConfig(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    DTMB_SET_CONFIG_PARAM SetConfigParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SetConfigParam.id = id;
    SetConfigParam.eType = eType;
    SetConfigParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetConfig, &SetConfigParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_SetReset(MS_U8 id)
{
    DTMB_ID_PARAM IdParam = {0};

    if (!_u32DTMBopen) return FALSE;

    IdParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetReset, &IdParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_Set_QAM_SR(MS_U8 id, DMD_DTMB_DEMOD_TYPE eType, MS_U16 symbol_rate)
{
    DTMB_SET_QAM_SR_PARAM SetQamSrParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SetQamSrParam.id = id;
    SetQamSrParam.eType = eType;
    SetQamSrParam.symbol_rate = symbol_rate;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_Set_QAM_SR, &SetQamSrParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
{
    DTMB_SET_ACTIVE_PARAM SetActiveParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SetActiveParam.id = id;
    SetActiveParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetActive, &SetActiveParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if DMD_DTMB_STR_EN
MS_U32 MDrv_DMD_DTMB_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
{
    DTMB_SET_POWER_STATE_PARAM SetPowerStateParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SetPowerStateParam.id = id;
    SetPowerStateParam.u16PowerState = u16PowerState;

    return UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetPowerState, &SetPowerStateParam);
}
#endif

DMD_DTMB_LOCK_STATUS MDrv_DMD_DTMB_MD_GetLock(MS_U8 id, DMD_DTMB_GETLOCK_TYPE eType)
{
    DTMB_GET_LOCK_PARAM GetLockParam = {0};

    if (!_u32DTMBopen) return DMD_DTMB_NULL;

    GetLockParam.id = id;
    GetLockParam.eType = eType;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetLockParam.status;
    }
    else
    {
        return DMD_DTMB_NULL;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GetModulationMode(MS_U8 id, DMD_DTMB_MODULATION_INFO *sDtmbModulationMode)
{
    DTMB_GET_MODULATION_MODE_PARAM GetModulationModeParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetModulationModeParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        *sDtmbModulationMode = GetModulationModeParam.info;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
{
    DTMB_GET_SIGNAL_STRENGTH_PARAM GetSignalStrengthParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetSignalStrengthParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetSignalStrength, &GetSignalStrengthParam) == UTOPIA_STATUS_SUCCESS)
    {
        *u16Strength = GetSignalStrengthParam.u16Strength;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_ReadFrequencyOffset(MS_U8 id, MS_S16 *cfo)
{
    DTMB_READ_FREQ_OFFSET_PARAM ReadFreqOffsetParam = {0};

    if (!_u32DTMBopen) return FALSE;

    ReadFreqOffsetParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_ReadFrequencyOffset, &ReadFreqOffsetParam) == UTOPIA_STATUS_SUCCESS)
    {
        *cfo = (MS_S16)((((double)(ReadFreqOffsetParam.cfo.fftfirstCfo)/0x10000+(double)(ReadFreqOffsetParam.cfo.fftSecondCfo)/0x20000))*(double)(ReadFreqOffsetParam.cfo.sr));

        return TRUE;
    }
    else
    {
        *cfo = 0;

        return FALSE;
    }
}

MS_U8 MDrv_DMD_DTMB_MD_GetSignalQuality(MS_U8 id)
{
    DTMB_GET_SIGNAL_QUALITY_PARAM GetSignalQualityParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetSignalQualityParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetSignalQuality, &GetSignalQualityParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetSignalQualityParam.u8Percentage;
    }
    else
    {
        return 0;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GetPreLdpcBer(MS_U8 id, float *pber)
{
    DTMB_GET_BER_PARAM GetBerParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetBerParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetPreLdpcBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
    {
        *pber = (float)(GetBerParam.ber.BitErr)/7488.0/(float)(GetBerParam.ber.Error_window);

        return TRUE;
    }
    else
    {
        *pber = 0;

        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GetPreViterbiBer(MS_U8 id, float *ber)
{
    DTMB_GET_BER_PARAM GetBerParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetBerParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetPreViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
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

MS_BOOL MDrv_DMD_DTMB_MD_GetPostViterbiBer(MS_U8 id, float *ber)
{
    DTMB_GET_BER_PARAM GetBerParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetBerParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetPostViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
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

MS_BOOL MDrv_DMD_DTMB_MD_GetSNR(MS_U8 id, float *snr)
{
    DTMB_GET_SNR_PARAM GetSnrParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GetSnrParam.id = id;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetSNR, &GetSnrParam) == UTOPIA_STATUS_SUCCESS)
    {
        #ifdef MSOS_TYPE_LINUX
        *snr = 10.0f*log10f((float)(GetSnrParam.snr.snr)/((float)GetSnrParam.snr.sym_num));
        #else
        *snr = 10.0f*Log10Approx((float)(GetSnrParam.snr.snr)/((float)GetSnrParam.snr.sym_num));
        #endif

        return TRUE;
    }
    else
    {
        *snr = 0;

        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
{
    DTMB_SET_SERIAL_CONTROL_PARAM SetSerialControlParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SetSerialControlParam.id = id;
    SetSerialControlParam.u8TsConfigData = u8TsConfigData;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetSerialControl, &SetSerialControlParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
{
    DTMB_IIC_BYPASS_MODE_PARAM IicBypassModeParam = {0};

    if (!_u32DTMBopen) return FALSE;

    IicBypassModeParam.id = id;
    IicBypassModeParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_IIC_BYPASS_MODE, &IicBypassModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
{
    DTMB_SWITCH_SSPI_GPIO_PARAM SwitchSspiGpioParam = {0};

    if (!_u32DTMBopen) return FALSE;

    SwitchSspiGpioParam.id = id;
    SwitchSspiGpioParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SWITCH_SSPI_GPIO, &SwitchSspiGpioParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
{
    DTMB_GPIO_LEVEL_PARAM GpioLevelParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GPIO_GET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        *bLevel = GpioLevelParam.bLevel;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
{
    DTMB_GPIO_LEVEL_PARAM GpioLevelParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;
    GpioLevelParam.bLevel = bLevel;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GPIO_SET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    DTMB_GPIO_OUT_ENABLE_PARAM GpioOutEnableParam = {0};

    if (!_u32DTMBopen) return FALSE;

    GpioOutEnableParam.id = id;
    GpioOutEnableParam.u8Pin = u8Pin;
    GpioOutEnableParam.bEnableOut = bEnableOut;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GPIO_OUT_ENABLE, &GpioOutEnableParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
{
    DTMB_DO_IQ_SWAP_PARAM DoIQSwapParam = {0};

    if (!_u32DTMBopen) return FALSE;

    DoIQSwapParam.id = id;
    DoIQSwapParam.bIsQPad = bIsQPad;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_DoIQSwap, &DoIQSwapParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    DTMB_REG_PARAM RegParam = {0};

    if (!_u32DTMBopen) return FALSE;

    RegParam.id = id;
    RegParam.u16Addr = u16Addr;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_GetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
    {
        *pu8Data = RegParam.u8Data;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_DTMB_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
{
    DTMB_REG_PARAM RegParam = {0};

    if (!_u32DTMBopen) return FALSE;

    RegParam.id = id;
    RegParam.u16Addr = u16Addr;
    RegParam.u8Data = u8Data;

    if (UtopiaIoctl(_ppDTMBInstant, DMD_DTMB_DRV_CMD_MD_SetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
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
