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
/// file    drvDMD_ISDBT.c
/// @brief  DMD ISDBT Driver Interface
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

#include "drvDMD_ISDBT.h"
#ifdef UTPA2
#include "drvDMD_ISDBT_v2.h"
#endif

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
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
#endif // #ifndef DONT_USE_CMA

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
static MS_BOOL _DEFAULT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs)
{
    #ifdef REMOVE_HAL_INTERN_ISDBT
    printf("LINK ERROR: REMOVE_HAL_INTERN_ISDBT \n");
    #else
    printf("LINK ERROR: Please link ext demod library \n");
    #endif

    return TRUE;
}

#ifdef REMOVE_HAL_INTERN_ISDBT
MS_BOOL HAL_INTERN_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_INTERN_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs);
#endif
MS_BOOL HAL_EXTERN_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs)  __attribute__((weak, alias ("_DEFAULT_IOCTL_CMD")));
#else
MS_BOOL HAL_EXTERN_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#if DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
 #define DMD_LOCK()      \
     do {                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_ISDBT_Mutex == -1) return FALSE; \
        if (_u8DMD_ISDBT_DbgLevel == DMD_ISDBT_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_ISDBT_Mutex, MSOS_WAIT_FOREVER);\
        } while(0)

 #define DMD_UNLOCK()      \
     do {                         \
        MsOS_ReleaseMutex(_s32DMD_ISDBT_Mutex);\
        if (_u8DMD_ISDBT_DbgLevel == DMD_ISDBT_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        } while(0)
#elif (!DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN && DMD_ISDBT_MULTI_THREAD_SAFE)
 #define DMD_LOCK()    do { pRes->sDMD_ISDBT_InitData.LockDMD(TRUE); } while(0)
 #define DMD_UNLOCK()  do { pRes->sDMD_ISDBT_InitData.LockDMD(FALSE); } while(0)
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

MS_U8 u8DMD_ISDBT_DMD_ID = 0;

#if !DMD_ISDBT_UTOPIA2_EN
DMD_ISDBT_ResData sDMD_ISDBT_ResData[DMD_ISDBT_MAX_DEMOD_NUM] = { { {0} , {0} , {0} } };

DMD_ISDBT_ResData *psDMD_ISDBT_ResData = sDMD_ISDBT_ResData;
#else
DMD_ISDBT_ResData *psDMD_ISDBT_ResData;
#endif

#ifdef UTPA2
static void* _ppISDBTInstant = NULL;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
static MSIF_Version _drv_dmd_isdbt_version;/* = {
    .MW = { DMD_ISDBT_VER, },
};*/
#endif

#if DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
static MS_S32 _s32DMD_ISDBT_Mutex = -1;
#endif

#ifndef DONT_USE_CMA
static struct CMA_Pool_Init_Param  _ISDBT_CMA_Pool_Init_PARAM; // for MApi_CMA_Pool_Init
static struct CMA_Pool_Alloc_Param _ISDBT_CMA_Alloc_PARAM;     // for MApi_CMA_Pool_GetMem
static struct CMA_Pool_Free_Param  _ISDBT_CMA_Free_PARAM;      // for MApi_CMA_Pool_PutMem
#endif

#ifdef UTPA2
static MS_U32 _u32ISDBTopen = 0;
#endif

static DMD_ISDBT_DbgLv _u8DMD_ISDBT_DbgLevel = DMD_ISDBT_DBGLV_NONE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static DMD_ISDBT_LOCK_STATUS _ISDBT_CheckLock(void)
{
    MS_BOOL (*ioctl)(DMD_ISDBT_HAL_COMMAND eCmd, void *pPara);

    DMD_ISDBT_ResData  *pRes  = psDMD_ISDBT_ResData + u8DMD_ISDBT_DMD_ID;
    DMD_ISDBT_InitData *pInit = &(pRes->sDMD_ISDBT_InitData);
    DMD_ISDBT_Info     *pInfo = &(pRes->sDMD_ISDBT_Info);

    ioctl = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD;

    if (ioctl(DMD_ISDBT_HAL_CMD_Check_FEC_Lock, NULL))
    {
        pInfo->u32ISDBTLockStatus |= DMD_ISDBT_LOCK_FEC_LOCK;
        #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
        pInfo->u32ISDBTFECLockTime = MsOS_GetSystemTime();
        #else
        pInfo->u32ISDBTFECLockTime = pInit->GetSystemTimeMS();
        #endif
        return DMD_ISDBT_LOCK;
    }
    else
    {
        #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
        if ((pInfo->u32ISDBTLockStatus & DMD_ISDBT_LOCK_FEC_LOCK) && ((MsOS_GetSystemTime()-pInfo->u32ISDBTFECLockTime) < 100))
        #else
        if ((pInfo->u32ISDBTLockStatus & DMD_ISDBT_LOCK_FEC_LOCK) && ((pInit->GetSystemTimeMS()-pInfo->u32ISDBTFECLockTime) < 100))
        #endif
        {
            return DMD_ISDBT_LOCK;
        }
        else
        {
            pInfo->u32ISDBTLockStatus &= (~DMD_ISDBT_LOCK_FEC_LOCK);
        }

        if (pInfo->u32ISDBTLockStatus & DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK) //STEP 3
        {
            #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
            if ((MsOS_GetSystemTime()-pInfo->u32ISDBTScanTimeStart) < pInit->u16ISDBTFECLockCheckTime)
            #else
            if ((pInit->GetSystemTimeMS()-pInfo->u32ISDBTScanTimeStart) < pInit->u16ISDBTFECLockCheckTime)
            #endif
            {
                return DMD_ISDBT_CHECKING;
            }
        }
        else //STEP 1,2
        {
            if (ioctl(DMD_ISDBT_HAL_CMD_Check_ICFO_CH_EXIST_Lock, NULL))
            {
                pInfo->u32ISDBTLockStatus |= DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK;
                #ifdef MS_DEBUG
                if (_u8DMD_ISDBT_DbgLevel >= DMD_ISDBT_DBGLV_DEBUG)
                {
                    printf("DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK\n");
                }
                #endif
                return DMD_ISDBT_CHECKING;
            }
            else
            {
                #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
                if ((MsOS_GetSystemTime()-pInfo->u32ISDBTScanTimeStart) < pInit->u16ISDBTIcfoChExistCheckTime)
                #else
                if ((pInit->GetSystemTimeMS()-pInfo->u32ISDBTScanTimeStart) < pInit->u16ISDBTIcfoChExistCheckTime)
                #endif
                {
                    return DMD_ISDBT_CHECKING;
                }
            }
        }
        return DMD_ISDBT_UNLOCK;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel)
#else
MS_BOOL MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel)
#endif
{
    _u8DMD_ISDBT_DbgLevel = u8DbgLevel;

    return TRUE;
}

#ifdef UTPA2
DMD_ISDBT_Info* _MDrv_DMD_ISDBT_GetInfo(void)
#else
DMD_ISDBT_Info* MDrv_DMD_ISDBT_GetInfo(void)
#endif
{
    psDMD_ISDBT_ResData->sDMD_ISDBT_Info.u8Version = 0;

    return &(psDMD_ISDBT_ResData->sDMD_ISDBT_Info);
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion)
#else
MS_BOOL MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion)
#endif
{
    if (!ppVersion) return FALSE;

    #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
    *ppVersion = &_drv_dmd_isdbt_version;
    #else
    *ppVersion = NULL;
    #endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_ISDBT_Init(DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen)
{
    return MDrv_DMD_ISDBT_MD_Init(0, pDMD_ISDBT_InitData, u32InitDataLen);
}

MS_BOOL MDrv_DMD_ISDBT_Exit(void)
{
    return MDrv_DMD_ISDBT_MD_Exit(0);
}

MS_U32 MDrv_DMD_ISDBT_GetConfig(DMD_ISDBT_InitData *psDMD_ISDBT_InitData)
{
    return MDrv_DMD_ISDBT_MD_GetConfig(0, psDMD_ISDBT_InitData);
}

MS_BOOL MDrv_DMD_ISDBT_GetFwVer(MS_U16 *pu16FwVer)
{
    return TRUE;
}

void MDrv_DMD_ISDBT_SetReset(void)
{
    return MDrv_DMD_ISDBT_MD_SetReset(0);
}

MS_BOOL MDrv_DMD_ISDBT_AdvSetConfig(DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    return MDrv_DMD_ISDBT_MD_AdvSetConfig(0, eType, bEnable);
}

MS_BOOL MDrv_DMD_ISDBT_SetConfig(MS_BOOL bEnable)
{
    return MDrv_DMD_ISDBT_MD_SetConfig(0, bEnable);
}

MS_BOOL MDrv_DMD_ISDBT_SetActive(MS_BOOL bEnable)
{
    return MDrv_DMD_ISDBT_MD_SetActive(0, bEnable);
}

#if DMD_ISDBT_STR_EN
MS_U32 MDrv_DMD_ISDBT_SetPowerState(EN_POWER_MODE u16PowerState)
{
    return MDrv_DMD_ISDBT_MD_SetPowerState(0, u16PowerState);
}
#endif

DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_TYPE eType)
{
    return MDrv_DMD_ISDBT_MD_GetLock(0, eType);
}

MS_BOOL MDrv_DMD_ISDBT_GetModulationMode(EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode)
{
    return MDrv_DMD_ISDBT_MD_GetModulationMode(0, eLayerIndex, sIsdbtModulationMode);
}

MS_BOOL MDrv_DMD_ISDBT_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_ISDBT_MD_GetSignalStrength(0, u16Strength);
}

MS_BOOL MDrv_DMD_ISDBT_GetFreqOffset(float *pFreqOff)
{
    return MDrv_DMD_ISDBT_MD_GetFreqOffset(0, pFreqOff);
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQuality(void)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQuality(0);
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerA(void)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(0);
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerB(void)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(0);
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerC(void)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(0);
}

MS_U16 MDrv_DMD_ISDBT_GetSignalQualityCombine(void)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(0);
}

MS_BOOL MDrv_DMD_ISDBT_GetSNR(float *f_snr)
{
    return MDrv_DMD_ISDBT_MD_GetSNR(0, f_snr);
}

MS_BOOL MDrv_DMD_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber)
{
    return MDrv_DMD_ISDBT_MD_GetPreViterbiBer(0, eLayerIndex, fber);
}

MS_BOOL MDrv_DMD_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber)
{
    return MDrv_DMD_ISDBT_MD_GetPostViterbiBer(0, eLayerIndex, fber);
}

MS_BOOL MDrv_DMD_ISDBT_Read_PKT_ERR(EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr)
{
    return MDrv_DMD_ISDBT_MD_Read_PKT_ERR(0, eLayerIndex, u16PacketErr);
}

MS_BOOL MDrv_DMD_ISDBT_SetSerialControl(MS_U8 u8TsConfigData)
{
    return MDrv_DMD_ISDBT_MD_SetSerialControl(0, u8TsConfigData);
}

MS_BOOL MDrv_DMD_ISDBT_IIC_BYPASS_MODE(MS_BOOL bEnable)
{
    return MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(0, bEnable);
}

MS_BOOL MDrv_DMD_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL bEnable)
{
    return MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(0, bEnable);
}

MS_BOOL MDrv_DMD_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel)
{
    return MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel)
{
    return MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(0, u8Pin, bLevel);
}

MS_BOOL MDrv_DMD_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    return MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(0, u8Pin, bEnableOut);
}

MS_BOOL MDrv_DMD_ISDBT_DoIQSwap(MS_BOOL bIsQPad)
{
    return MDrv_DMD_ISDBT_MD_DoIQSwap(0, bIsQPad);
}

MS_BOOL MDrv_DMD_ISDBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_ISDBT_MD_GetReg(0, u16Addr, pu8Data);
}

MS_BOOL MDrv_DMD_ISDBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_ISDBT_MD_SetReg(0, u16Addr, u8Data);
}
#endif // #ifndef MSOS_TYPE_LINUX_KERNEL

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                               ///
////////////////////////////////////////////////////////////////////////////////

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_PHY phyNonPMBankSize;
    #ifndef DONT_USE_CMA
    MS_U32  u32PhysicalAddr_FromVA = 0, u32HeapStartPAAddress = 0;
    #endif

    #if DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
    if (_s32DMD_ISDBT_Mutex == -1)
    {
        if ((_s32DMD_ISDBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD ISDBT", MSOS_PROCESS_SHARED)) == -1)
        {
            DMD_DBG(printf("MDrv_DMD_ISDBT_Init Create Mutex Fail\n"));
            return FALSE;
        }
    }
    #elif (!DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN && DMD_ISDBT_MULTI_THREAD_SAFE)
    if (!pRes->sDMD_ISDBT_InitData.CreateMutex(TRUE))
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init Create Mutex Fail\n"));
        return FALSE;
    }
    #endif

    DMD_LOCK();
    if (pRes->sDMD_ISDBT_PriData.bInit)
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init more than once\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_ISDBT_DbgLevel >= DMD_ISDBT_DBGLV_INFO)
    {
        printf("MDrv_DMD_ISDBT_Init\n");
    }
    #endif

    #ifndef MSOS_TYPE_LINUX_KERNEL
    if (sizeof(DMD_ISDBT_InitData) == u32InitDataLen)
    {
        memcpy(&(pRes->sDMD_ISDBT_InitData), pDMD_ISDBT_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_ISDBT_Init input data structure incorrect\n"));
        return FALSE;
    }
    #else
    memcpy(&(pRes->sDMD_ISDBT_InitData), pDMD_ISDBT_InitData, sizeof(DMD_ISDBT_InitData));
    #endif

    if (!MDrv_MMIO_GetBASE(&(pRes->sDMD_ISDBT_PriData.virtDMDBaseAddr), &phyNonPMBankSize, MS_MODULE_PM))
    {
        #ifdef MS_DEBUG
        printf("HAL_DMD_RegInit failure to get MS_MODULE_PM\n");
        #endif
        return FALSE;
    }

    pRes->sDMD_ISDBT_PriData.bInit = TRUE;

    #ifndef DONT_USE_CMA
    _ISDBT_CMA_Pool_Init_PARAM.heap_id = 26;
    _ISDBT_CMA_Pool_Init_PARAM.flags = CMA_FLAG_MAP_VMA;

    if (MApi_CMA_Pool_Init(&_ISDBT_CMA_Pool_Init_PARAM) == FALSE)
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get MApi_CMA_Pool_Init ERROR!!!\n", __PRETTY_FUNCTION__, __LINE__));
    }
    else
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Pool_Init_PARAM.pool_handle_id));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Pool_Init_PARAM.miu));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get heap_miu_start_offset is 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Pool_Init_PARAM.heap_miu_start_offset));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Pool_Init_PARAM.heap_length));
    }

    _miu_offset_to_phy(_ISDBT_CMA_Pool_Init_PARAM.miu, _ISDBT_CMA_Pool_Init_PARAM.heap_miu_start_offset, u32HeapStartPAAddress);

    _ISDBT_CMA_Alloc_PARAM.pool_handle_id = _ISDBT_CMA_Pool_Init_PARAM.pool_handle_id;
    _ISDBT_CMA_Alloc_PARAM.offset_in_pool = (pRes->sDMD_ISDBT_InitData.u32TdiStartAddr * 16) - u32HeapStartPAAddress;	// offset_in_pool means the start_ask_addr with respect to the pool_start_addr(BA) => this will specify the start_ask_ba_addr
    _ISDBT_CMA_Alloc_PARAM.length = 0x500000UL;
    _ISDBT_CMA_Alloc_PARAM.flags = CMA_FLAG_VIRT_ADDR;										// we will get a start_va with respect to the start_ask_ba_addr if using CMA_FLAG_VIRT_ADDR flag

    if(MApi_CMA_Pool_GetMem(&_ISDBT_CMA_Alloc_PARAM) == FALSE)
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, CMA_Pool_GetMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
    }
    else
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get from heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Alloc_PARAM.pool_handle_id));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask offset 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Alloc_PARAM.offset_in_pool));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask length 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Alloc_PARAM.length));
        DMD_DBG(printf("\033[37mFunction = %s, Line = %d, return va is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Alloc_PARAM.virt_addr));
    }

    u32PhysicalAddr_FromVA = MsOS_MPool_VA2PA(_ISDBT_CMA_Alloc_PARAM.virt_addr);

    DMD_DBG(printf("#######u32PhysicalAddr_FromVA1 = [0x%x]\n", u32PhysicalAddr_FromVA));

    _miu_offset_to_phy(_ISDBT_CMA_Pool_Init_PARAM.miu,
                       _ISDBT_CMA_Pool_Init_PARAM.heap_miu_start_offset + _ISDBT_CMA_Alloc_PARAM.offset_in_pool,
                       u32PhysicalAddr_FromVA);

    DMD_DBG(printf("#######u32PhysicalAddr_FromVA2 = [0x%x]\n", u32PhysicalAddr_FromVA));

    DMD_DBG(printf("#######pRes->sDMD_ISDBT_InitData.u32TdiStartAddr = [0x%x]\n", (pRes->sDMD_ISDBT_InitData.u32TdiStartAddr * 16)));
    #endif // #ifndef DONT_USE_CMA

    #if DMD_ISDBT_STR_EN
    pRes->sDMD_ISDBT_PriData.eLastState = E_POWER_RESUME;
    #endif

    pRes->sDMD_ISDBT_PriData.eLastType  = DMD_ISDBT_DEMOD;

    #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
    if (pRes->sDMD_ISDBT_InitData.bIsExtDemod)
        pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD = HAL_EXTERN_ISDBT_IOCTL_CMD;
    else
        pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD = HAL_INTERN_ISDBT_IOCTL_CMD;
    #else
    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD = HAL_EXTERN_ISDBT_IOCTL_CMD;
    #endif

    u8DMD_ISDBT_DMD_ID = id;
    if (!MDrv_MMIO_GetBASE(&(pRes->sDMD_ISDBT_PriData.virtDMDBaseAddr), &phyNonPMBankSize, MS_MODULE_PM))
    {
        #ifdef MS_DEBUG
        printf("HAL_DMD_RegInit failure to get MS_MODULE_PM\n");
        #endif
        return FALSE;
    }

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_InitClk, NULL);

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Download, NULL);

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_FWVERSION, NULL);
    DMD_UNLOCK();

    return TRUE;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_Exit(MS_U8 id)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_Exit(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Exit, NULL);

    pRes->sDMD_ISDBT_PriData.bInit = FALSE;

    #ifndef DONT_USE_CMA
    _ISDBT_CMA_Free_PARAM.pool_handle_id = _ISDBT_CMA_Pool_Init_PARAM.pool_handle_id;
    _ISDBT_CMA_Free_PARAM.offset_in_pool = _ISDBT_CMA_Alloc_PARAM.offset_in_pool;
    _ISDBT_CMA_Free_PARAM.length = 0x500000UL;;

    if(MApi_CMA_Pool_PutMem(&_ISDBT_CMA_Free_PARAM) == FALSE)
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, MsOS_CMA_Pool_Release ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
    }
    else
    {
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, get from heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Free_PARAM.pool_handle_id));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask offset 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Free_PARAM.offset_in_pool));
        DMD_DBG(printf("\033[35mFunction = %s, Line = %d, ask length 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, _ISDBT_CMA_Free_PARAM.length));
    }
    #endif

    DMD_UNLOCK();

    #if DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
    MsOS_DeleteMutex(_s32DMD_ISDBT_Mutex);

    _s32DMD_ISDBT_Mutex = -1;
    #elif (!DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN && DMD_ISDBT_MULTI_THREAD_SAFE)
    pRes->sDMD_ISDBT_InitData.CreateMutex(FALSE);
    #endif

    return bRet;
}

#ifdef UTPA2
MS_U32 _MDrv_DMD_ISDBT_MD_GetConfig(MS_U8 id, DMD_ISDBT_InitData *psDMD_ISDBT_InitData)
#else
MS_U32 MDrv_DMD_ISDBT_MD_GetConfig(MS_U8 id, DMD_ISDBT_InitData *psDMD_ISDBT_InitData)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;

    DMD_LOCK();
    memcpy (psDMD_ISDBT_InitData, &(pRes->sDMD_ISDBT_InitData), sizeof(DMD_ISDBT_InitData));
    DMD_UNLOCK();

    return UTOPIA_STATUS_SUCCESS;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetFwVer(MS_U8 id, MS_U16 *pu16FwVer)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetFwVer(MS_U8 id, MS_U16 *pu16FwVer)
#endif
{
    return TRUE;
}

void MDrv_DMD_ISDBT_MD_SetReset(MS_U8 id)
{
    return;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SoftReset, NULL);

    if (bEnable)
    {
        switch (eType)
        {
            case DMD_ISDBT_DEMOD_6M:
            case DMD_ISDBT_DEMOD_7M:
            case DMD_ISDBT_DEMOD_8M:
                if (eType != pRes->sDMD_ISDBT_PriData.eLastType)
                {
                    pRes->sDMD_ISDBT_PriData.eLastType = DMD_ISDBT_DEMOD_6M;
                    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SetACICoef, NULL);
                }
                bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SetISDBTMode, NULL);
                break;
            default:
                pRes->sDMD_ISDBT_PriData.eLastType = DMD_ISDBT_DEMOD_NULL;
                bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SetModeClean, NULL);
                break;
        }
    }

    #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
    pRes->sDMD_ISDBT_Info.u32ISDBTScanTimeStart = MsOS_GetSystemTime();
    #else
    pRes->sDMD_ISDBT_Info.u32ISDBTScanTimeStart = psDMD_ISDBT_ResData->sDMD_ISDBT_InitData.GetSystemTimeMS();
    #endif
    pRes->sDMD_ISDBT_Info.u32ISDBTLockStatus    = 0;
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL MDrv_DMD_ISDBT_MD_SetConfig(MS_U8 id, MS_BOOL bEnable)
{
    #ifdef UTPA2
    return _MDrv_DMD_ISDBT_MD_AdvSetConfig(id , DMD_ISDBT_DEMOD_6M, bEnable);
    #else
    return MDrv_DMD_ISDBT_MD_AdvSetConfig(id , DMD_ISDBT_DEMOD_6M, bEnable);
    #endif
}

MS_BOOL MDrv_DMD_ISDBT_MD_SetActive(MS_U8 id, MS_BOOL bEnable)
{
    return TRUE;
}

#if DMD_ISDBT_STR_EN
#ifdef UTPA2
MS_U32 _MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#else
MS_U32 MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        pRes->sDMD_ISDBT_PriData.bDownloaded = FALSE;
        pRes->sDMD_ISDBT_PriData.bIsDTV      = pRes->sDMD_ISDBT_PriData.bInit;
        pRes->sDMD_ISDBT_PriData.eLastState  = u16PowerState;

        if(pRes->sDMD_ISDBT_PriData.bInit)
        {
            #ifdef UTPA2
            _MDrv_DMD_ISDBT_MD_Exit(id);
            #else
            MDrv_DMD_ISDBT_MD_Exit(id);
            #endif
        }

        u32Return = UTOPIA_STATUS_SUCCESS;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (pRes->sDMD_ISDBT_PriData.eLastState == E_POWER_SUSPEND)
        {
            printf("\nVT: (Check Mode In DRV:) DTV Mode=%d\n", pRes->sDMD_ISDBT_PriData.bIsDTV);

            if (pRes->sDMD_ISDBT_PriData.bIsDTV)
            {
                #ifdef UTPA2
                _MDrv_DMD_ISDBT_MD_Init(id, &(pRes->sDMD_ISDBT_InitData), sizeof(DMD_ISDBT_InitData));
                _MDrv_DMD_ISDBT_MD_DoIQSwap(id, pRes->sDMD_ISDBT_PriData.bIsQPad);
                _MDrv_DMD_ISDBT_MD_AdvSetConfig(id, pRes->sDMD_ISDBT_PriData.eLastType, TRUE);
                #else
                MDrv_DMD_ISDBT_MD_Init(id, &(pRes->sDMD_ISDBT_InitData), sizeof(DMD_ISDBT_InitData));
                MDrv_DMD_ISDBT_MD_DoIQSwap(id, pRes->sDMD_ISDBT_PriData.bIsQPad);
                MDrv_DMD_ISDBT_MD_AdvSetConfig(id, pRes->sDMD_ISDBT_PriData.eLastType, TRUE);
                #endif
            }

            pRes->sDMD_ISDBT_PriData.eLastState = u16PowerState;

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
DMD_ISDBT_LOCK_STATUS _MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType)
#else
DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_LOCK_STATUS status  = DMD_ISDBT_UNLOCK;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    switch (eType)
    {
        case DMD_ISDBT_GETLOCK:
            status = _ISDBT_CheckLock();
            break;
        case DMD_ISDBT_GETLOCK_FSA_TRACK_LOCK:
            status = (pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Check_FSA_TRACK_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_PSYNC_LOCK:
            status = (pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Check_PSYNC_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_ICFO_CH_EXIST_LOCK:
            status = (pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Check_ICFO_CH_EXIST_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        case DMD_ISDBT_GETLOCK_FEC_LOCK:
            status = (pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Check_FEC_Lock, NULL)) ? DMD_ISDBT_LOCK : DMD_ISDBT_UNLOCK;
            break;
        default:
            status = DMD_ISDBT_UNLOCK;
            break;
    }
    DMD_UNLOCK();

    return status;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_ISDBT_GET_MODULATION       sIsdbtGetModulationMode;
    DMD_ISDBT_GET_TimeInterleaving sIsdbtGetTimeInterleaving;
    DMD_ISDBT_GET_CodeRate         sIsdbtGetCodeRate;

    sIsdbtGetModulationMode.eIsdbtLayer   = eLayerIndex;
    sIsdbtGetTimeInterleaving.eIsdbtLayer = eLayerIndex;
    sIsdbtGetCodeRate.eIsdbtLayer         = eLayerIndex;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet &= pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalCodeRate, &sIsdbtGetCodeRate);
    sIsdbtModulationMode->eIsdbtCodeRate = sIsdbtGetCodeRate.eCodeRate;
    bRet &= pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalGuardInterval, &(sIsdbtModulationMode->eIsdbtGI));
    bRet &= pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalTimeInterleaving, &sIsdbtGetTimeInterleaving);
    sIsdbtModulationMode->eIsdbtTDI = sIsdbtGetTimeInterleaving.eTimeInterleaving;
    bRet &= pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalFFTValue, &(sIsdbtModulationMode->eIsdbtFFT));
    bRet &= pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalModulation, &sIsdbtGetModulationMode);
    sIsdbtModulationMode->eIsdbtConstellation = sIsdbtGetModulationMode.eConstellation;
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_ReadIFAGC, u16Strength);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, DMD_ISDBT_CFO_DATA *cfo)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, float *cfo)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetFreqOffset, cfo);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id)
#else
MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U16 u16SignalQuality = 0;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalQuality, &u16SignalQuality);
    DMD_UNLOCK();

    return u16SignalQuality;
}

#ifdef UTPA2
MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id)
#else
MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U16 u16SignalQuality = 0;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerA, &u16SignalQuality);
    DMD_UNLOCK();

    return u16SignalQuality;
}

#ifdef UTPA2
MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id)
#else
MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U16 u16SignalQuality = 0;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerB, &u16SignalQuality);
    DMD_UNLOCK();

    return u16SignalQuality;
}

#ifdef UTPA2
MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id)
#else
MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U16 u16SignalQuality = 0;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerC, &u16SignalQuality);
    DMD_UNLOCK();

    return u16SignalQuality;
}

#ifdef UTPA2
MS_U16 _MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id)
#else
MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_U16 u16SignalQuality = 0;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSignalQualityCombine, &u16SignalQuality);
    DMD_UNLOCK();

    return u16SignalQuality;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, DMD_ISDBT_SNR_DATA *f_snr)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, float *f_snr)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetSNR, f_snr);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, DMD_ISDBT_GET_BER_VALUE *ber)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    #ifndef UTPA2
    DMD_ISDBT_GET_BER_VALUE sIsdbtGetBerValue;
    sIsdbtGetBerValue.eIsdbtLayer = eLayerIndex;
    #endif

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    #ifdef UTPA2
    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetPreViterbiBer, ber);
    #else
    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetPreViterbiBer, &sIsdbtGetBerValue);
    #endif
    DMD_UNLOCK();

    #ifndef UTPA2
    *fber = sIsdbtGetBerValue.fBerValue;
    #endif

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, DMD_ISDBT_GET_BER_VALUE *ber)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    #ifndef UTPA2
    DMD_ISDBT_GET_BER_VALUE sIsdbtGetBerValue;
    sIsdbtGetBerValue.eIsdbtLayer = eLayerIndex;
    #endif

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    #ifdef UTPA2
    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetPostViterbiBer, ber);
    #else
    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GetPostViterbiBer, &sIsdbtGetBerValue);
    #endif
    DMD_UNLOCK();

    #ifndef UTPA2
    *fber = sIsdbtGetBerValue.fBerValue;
    #endif
    
    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_ISDBT_GET_PKT_ERR sIsdbtGetPktErr;

    sIsdbtGetPktErr.eIsdbtLayer = eLayerIndex;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_Read_PKT_ERR, &sIsdbtGetPktErr);
    DMD_UNLOCK();

    *u16PacketErr = sIsdbtGetPktErr.u16PacketErr;

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    pRes->sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = u8TsConfigData;
    pRes->sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap   = u8TsConfigData >> 1;
    pRes->sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv   = u8TsConfigData >> 2;
    pRes->sDMD_ISDBT_InitData.u5TsConfigByte_DivNum     = u8TsConfigData >> 3;

    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_TS_INTERFACE_CONFIG, NULL);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_IIC_Bypass_Mode, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SSPI_TO_GPIO, &bEnable);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin = u8Pin;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GPIO_GET_LEVEL, &sPin);
    DMD_UNLOCK();

    *bLevel = sPin.bLevel;

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin  = u8Pin;
    sPin.bLevel = bLevel;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GPIO_SET_LEVEL, &sPin);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_GPIO_PIN_DATA sPin;
    MS_BOOL bRet = TRUE;

    sPin.u8Pin  = u8Pin;
    sPin.bIsOut = bEnableOut;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GPIO_OUT_ENABLE, &sPin);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    MS_BOOL bRet = TRUE;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_DoIQSwap, &bIsQPad);
    DMD_UNLOCK();

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_REG_DATA reg;
    MS_BOOL bRet = TRUE;

    reg.u16Addr = u16Addr;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_GET_REG, &reg);
    DMD_UNLOCK();

    *pu8Data = reg.u8Data;

    return bRet;
}

#ifdef UTPA2
MS_BOOL _MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#else
MS_BOOL MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
#endif
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + id;
    DMD_ISDBT_REG_DATA reg;
    MS_BOOL bRet = TRUE;

    reg.u16Addr = u16Addr;
    reg.u8Data  = u8Data;

    DMD_LOCK();
    u8DMD_ISDBT_DMD_ID = id;

    bRet = pRes->sDMD_ISDBT_PriData.HAL_DMD_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_CMD_SET_REG, &reg);
    DMD_UNLOCK();

    return bRet;
}


#ifdef UTPA2
////////////////////////////////////////////////////////////////////////////////
///                            Utopia2 Interface                             ///
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

static MS_U16 CALCULATE_SQI(float fber)
{
    float flog_ber;
    MS_U16 u16SQI;

    #ifdef MSOS_TYPE_LINUX
    flog_ber = (float)log10((double)fber);
    #else
    if (fber != 0.0)
        flog_ber = (float)(-1.0*Log10Approx((double)(1.0 / fber)));
    else
        flog_ber = -8.0;//when fber=0 means u16SQI=100
    #endif

    if (flog_ber <= ( - 7.0))
    {
        u16SQI = 100;    //*quality = 100;
    }
    else if (flog_ber < -6.0)
    {
        u16SQI = (90+((( - 6.0) - flog_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if (flog_ber < -5.5)
    {
        u16SQI = (80+((( - 5.5) - flog_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if (flog_ber < -5.0)
    {
        u16SQI = (70+((( - 5.0) - flog_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if (flog_ber < -4.5)
    {
        u16SQI = (60+((( - 4.5) - flog_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if (flog_ber < -4.0)
    {
        u16SQI = (50+((( - 4.0) - flog_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if (flog_ber < -3.5)
    {
        u16SQI = (40+((( - 3.5) - flog_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if (flog_ber < -3.0)
    {
        u16SQI = (30+((( - 3.0) - flog_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if (flog_ber < -2.5)
    {
        u16SQI = (20+((( - 2.5) - flog_ber) / (( - 2.5) - ( -3.0))*(30-20)));
    }
    else if (flog_ber < -2.0)
    {
        u16SQI = (0+((( - 2.0) - flog_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
    else
    {
        u16SQI = 0;
    }

    return u16SQI;
}


MS_BOOL MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel)
{
    ISDBT_DBG_LEVEL_PARAM DbgLevelParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    DbgLevelParam.u8DbgLevel = u8DbgLevel;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_SetDbgLevel, &DbgLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DMD_ISDBT_Info* MDrv_DMD_ISDBT_GetInfo(void)
{
    ISDBT_GET_INFO_PARAM GetInfoParam = {0};

    if (!_u32ISDBTopen) return NULL;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_GetInfo, &GetInfoParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetInfoParam.pInfo;
    }
    else
    {
        return NULL;
    }
}

MS_BOOL MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion)
{
    ISDBT_GET_LIB_VER_PARAM GetLibVerParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_GetLibVer, &GetLibVerParam) == UTOPIA_STATUS_SUCCESS)
    {
        *ppVersion = NULL;//*GetLibVerParam.ppVersion;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;
    ISDBT_INIT_PARAM InitParam = {0};

    if (_u32ISDBTopen == 0)
    {
        if (UtopiaOpen(MODULE_ISDBT /*| KERNEL_MODE*/, &_ppISDBTInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
            _u32ISDBTopen = 1;
        else
            return FALSE;
    }

    if (!_u32ISDBTopen) return FALSE;

    InitParam.id = id;
    InitParam.pDMD_ISDBT_InitData = pDMD_ISDBT_InitData;
    InitParam.u32InitDataLen = u32InitDataLen;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_Init, &InitParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_Exit(MS_U8 id)
{
    ISDBT_ID_PARAM IdParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    IdParam.id = id;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_Exit, &IdParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32 MDrv_DMD_ISDBT_MD_GetConfig(MS_U8 id, DMD_ISDBT_InitData *psDMD_ISDBT_InitData)
{
    ISDBT_INIT_PARAM InitParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    InitParam.id = id;
    InitParam.pDMD_ISDBT_InitData = psDMD_ISDBT_InitData;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetConfig, &InitParam) == UTOPIA_STATUS_SUCCESS)
    {
        return UTOPIA_STATUS_SUCCESS;
    }
    else
    {
        return UTOPIA_STATUS_ERR_NOT_AVAIL;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable)
{
    ISDBT_SET_CONFIG_PARAM SetConfigParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    SetConfigParam.id = id;
    SetConfigParam.eType = eType;
    SetConfigParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_AdvSetConfig, &SetConfigParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if DMD_ISDBT_STR_EN
MS_U32 MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState)
{
    ISDBT_SET_POWER_STATE_PARAM SetPowerStateParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    SetPowerStateParam.id = id;
    SetPowerStateParam.u16PowerState = u16PowerState;

    return UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_SetPowerState, &SetPowerStateParam);
}
#endif

DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType)
{
    ISDBT_GET_LOCK_PARAM GetLockParam = {0};

    if (!_u32ISDBTopen) return DMD_ISDBT_NULL;

    GetLockParam.id = id;
    GetLockParam.eType = eType;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        return GetLockParam.status;
    }
    else
    {
        return DMD_ISDBT_NULL;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode)
{
    ISDBT_GET_MODULATION_MODE_PARAM GetModulationModeParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GetModulationModeParam.eLayerIndex = eLayerIndex;
    GetModulationModeParam.id = id;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        *sIsdbtModulationMode = GetModulationModeParam.IsdbtModulationMode;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength)
{
    ISDBT_GET_SIGNAL_STRENGTH_PARAM GetSignalStrengthParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GetSignalStrengthParam.id = id;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetSignalStrength, &GetSignalStrengthParam) == UTOPIA_STATUS_SUCCESS)
    {
        *u16Strength = GetSignalStrengthParam.u16Strength;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, float *cfo)
{
    ISDBT_GET_FREQ_OFFSET_PARAM GetFreqOffsetParam = {0};
    float   fTdCfoFreq = 0.0;
    float   fFdCfoFreq = 0.0;
    float   fICfoFreq = 0.0;

    if (!_u32ISDBTopen) return FALSE;

    GetFreqOffsetParam.id = id;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetFreqOffset, &GetFreqOffsetParam) == UTOPIA_STATUS_SUCCESS)
    {
        fTdCfoFreq = ((float)GetFreqOffsetParam.cfo.TdCfoRegValue) / 17179869184.0; //<25,34>
        fTdCfoFreq = fTdCfoFreq * 8126980.0;

        fFdCfoFreq = ((float)GetFreqOffsetParam.cfo.FdCfoRegValue) / 17179869184.0;
        fFdCfoFreq = fFdCfoFreq * 8126980.0;

        if((GetFreqOffsetParam.cfo.FFT_Mode & 0x30) == 0x0000) // 2k
            fICfoFreq = (float)GetFreqOffsetParam.cfo.IcfoRegValue*250000.0/63.0;
        else if((GetFreqOffsetParam.cfo.FFT_Mode & 0x0030) == 0x0010)	// 4k
            fICfoFreq = (float)GetFreqOffsetParam.cfo.IcfoRegValue*125000.0/63.0;
        else //if(u16data & 0x0030 == 0x0020) // 8k
            fICfoFreq = (float)GetFreqOffsetParam.cfo.IcfoRegValue*125000.0/126.0;

        *cfo = fTdCfoFreq + fFdCfoFreq + fICfoFreq;

        return TRUE;
    }
    else
    {
        *cfo = 0;

        return FALSE;
    }
}

MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id)
{
    return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(id);
}

MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id)
{
    ISDBT_GET_BER_PARAM GetBerParam = {0};
    ISDBT_GET_LOCK_PARAM GetLockParam = {0};

    if (!_u32ISDBTopen) return 0;

    GetLockParam.id = GetBerParam.id = id;
    GetLockParam.eType = DMD_ISDBT_GETLOCK_FEC_LOCK;
    GetBerParam.ber.eIsdbtLayer= E_ISDBT_Layer_A;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        if(GetLockParam.status == DMD_ISDBT_LOCK)
        {
            if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
            {
                if(GetBerParam.ber.BerPeriod == 0) GetBerParam.ber.BerPeriod = 1;

                return CALCULATE_SQI((float)GetBerParam.ber.BerValue/GetBerParam.ber.BerPeriod/(128.0*188.0*8.0));
            }
        }
    }

    return 0;

}

MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id)
{
    ISDBT_GET_BER_PARAM GetBerParam = {0};
    ISDBT_GET_LOCK_PARAM GetLockParam = {0};

    if (!_u32ISDBTopen) return 0;

    GetLockParam.id = GetBerParam.id = id;
    GetLockParam.eType = DMD_ISDBT_GETLOCK_FEC_LOCK;
    GetBerParam.ber.eIsdbtLayer= E_ISDBT_Layer_B;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        if(GetLockParam.status == DMD_ISDBT_LOCK)
        {
            if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
            {
                if(GetBerParam.ber.BerPeriod == 0) GetBerParam.ber.BerPeriod = 1;

                return CALCULATE_SQI((float)GetBerParam.ber.BerValue/GetBerParam.ber.BerPeriod/(128.0*188.0*8.0));
            }
        }
    }

    return 0;

}

MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id)
{
    ISDBT_GET_BER_PARAM GetBerParam = {0};
    ISDBT_GET_LOCK_PARAM GetLockParam = {0};

    if (!_u32ISDBTopen) return 0;

    GetLockParam.id = GetBerParam.id = id;
    GetLockParam.eType = DMD_ISDBT_GETLOCK_FEC_LOCK;
    GetBerParam.ber.eIsdbtLayer= E_ISDBT_Layer_C;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetLock, &GetLockParam) == UTOPIA_STATUS_SUCCESS)
    {
        if(GetLockParam.status == DMD_ISDBT_LOCK)
        {
            if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
            {
                if(GetBerParam.ber.BerPeriod == 0) GetBerParam.ber.BerPeriod = 1;

                return CALCULATE_SQI((float)GetBerParam.ber.BerValue/GetBerParam.ber.BerPeriod/(128.0*188.0*8.0));
            }
        }
    }

    return 0;

}

MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id)
{
    ISDBT_GET_MODULATION_MODE_PARAM GetModulationModeParam = {0};
    MS_S8  s8LayerAValue = 0, s8LayerBValue = 0, s8LayerCValue = 0;
    EN_ISDBT_CONSTEL_TYPE eIsdbtConstellationA = E_ISDBT_QAM_INVALID, eIsdbtConstellationB = E_ISDBT_QAM_INVALID, eIsdbtConstellationC = E_ISDBT_QAM_INVALID;

    if (!_u32ISDBTopen) return FALSE;

    GetModulationModeParam.id = id;

    GetModulationModeParam.eLayerIndex = E_ISDBT_Layer_A;
    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) != UTOPIA_STATUS_SUCCESS) return 0;
    s8LayerAValue = eIsdbtConstellationA == E_ISDBT_QAM_INVALID ? -1 : (MS_S8)GetModulationModeParam.IsdbtModulationMode.eIsdbtConstellation;

    GetModulationModeParam.eLayerIndex = E_ISDBT_Layer_B;
    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) != UTOPIA_STATUS_SUCCESS) return 0;
    s8LayerBValue = eIsdbtConstellationB == E_ISDBT_QAM_INVALID ? -1 : (MS_S8)GetModulationModeParam.IsdbtModulationMode.eIsdbtConstellation;

    GetModulationModeParam.eLayerIndex = E_ISDBT_Layer_C;
    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetModulationMode, &GetModulationModeParam) != UTOPIA_STATUS_SUCCESS) return 0;
    s8LayerCValue = eIsdbtConstellationC == E_ISDBT_QAM_INVALID ? -1 : (MS_S8)GetModulationModeParam.IsdbtModulationMode.eIsdbtConstellation;

    if (s8LayerAValue >= s8LayerBValue)
    {
        if (s8LayerCValue >= s8LayerAValue)
        {
            return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(id);
        }
        else  //A>C
        {
            return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(id);
        }
    }
    else  // B >= A
    {
        if (s8LayerCValue >= s8LayerBValue)
        {
            return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(id);
        }
        else  //B>C
        {
            return MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(id);
        }
    }

}

MS_BOOL MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, float *f_snr)
{
    ISDBT_GET_SNR_PARAM GetSnrParam = {0};
    float fSNRAvg = 0.0;

    if (!_u32ISDBTopen) return FALSE;

    GetSnrParam.id = id;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetSNR, &GetSnrParam) == UTOPIA_STATUS_SUCCESS)
    {
        fSNRAvg = (float)GetSnrParam.snr.RegSNR/GetSnrParam.snr.RegSnrObsNum;

        if (fSNRAvg == 0)
            fSNRAvg = 0.01;

        #ifdef MSOS_TYPE_LINUX
        *f_snr = 10*(float)log10f((double)fSNRAvg/2);
        #else
        *f_snr = 10*(float)Log10Approx((double)fSNRAvg/2);
        #endif

        return TRUE;
    }
    else
    {
        *f_snr = 0;

        return FALSE;
    }}

MS_BOOL MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber)
{
    ISDBT_GET_BER_PARAM GetBerParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GetBerParam.id = id;
    GetBerParam.ber.eIsdbtLayer= eLayerIndex;
    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
    {
        GetBerParam.ber.BerPeriod <<= 8; // *256

        if(GetBerParam.ber.BerPeriod == 0) GetBerParam.ber.BerPeriod = 1;

        *fber = (float)GetBerParam.ber.BerValue/GetBerParam.ber.BerPeriod;

        return TRUE;
    }
    else
    {
        *fber = 0;

        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber)
{
    ISDBT_GET_BER_PARAM GetBerParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GetBerParam.id = id;
    GetBerParam.ber.eIsdbtLayer= eLayerIndex;
    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer, &GetBerParam) == UTOPIA_STATUS_SUCCESS)
    {
        if(GetBerParam.ber.BerPeriod == 0) GetBerParam.ber.BerPeriod = 1;

        *fber = (float)GetBerParam.ber.BerValue/GetBerParam.ber.BerPeriod/(128.0*188.0*8.0);

        return TRUE;
    }
    else
    {
        *fber = 0;

        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr)
{
    ISDBT_READ_PKT_ERR_PARAM ReadpktErrParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    ReadpktErrParam.id = id;
    ReadpktErrParam.eIsdbtLayer = eLayerIndex;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_Read_PKT_ERR, &ReadpktErrParam) == UTOPIA_STATUS_SUCCESS)
    {
        *u16PacketErr = ReadpktErrParam.u16PacketErr;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData)
{
    ISDBT_SET_SERIAL_CONTROL_PARAM SetSerialControlParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    SetSerialControlParam.id = id;
    SetSerialControlParam.u8TsConfigData = u8TsConfigData;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_SetSerialControl, &SetSerialControlParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable)
{
    ISDBT_IIC_BYPASS_MODE_PARAM IicBypassModeParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    IicBypassModeParam.id = id;
    IicBypassModeParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_IIC_BYPASS_MODE, &IicBypassModeParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable)
{
    ISDBT_SWITCH_SSPI_GPIO_PARAM SwitchSspiGpioParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    SwitchSspiGpioParam.id = id;
    SwitchSspiGpioParam.bEnable = bEnable;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_SWITCH_SSPI_GPIO, &SwitchSspiGpioParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel)
{
    ISDBT_GPIO_LEVEL_PARAM GpioLevelParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GPIO_GET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        *bLevel = GpioLevelParam.bLevel;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel)
{
    ISDBT_GPIO_LEVEL_PARAM GpioLevelParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GpioLevelParam.id = id;
    GpioLevelParam.u8Pin = u8Pin;
    GpioLevelParam.bLevel = bLevel;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GPIO_SET_LEVEL, &GpioLevelParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut)
{
    ISDBT_GPIO_OUT_ENABLE_PARAM GpioOutEnableParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    GpioOutEnableParam.id = id;
    GpioOutEnableParam.u8Pin = u8Pin;
    GpioOutEnableParam.bEnableOut = bEnableOut;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GPIO_OUT_ENABLE, &GpioOutEnableParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_DoIQSwap(MS_U8 id, MS_BOOL bIsQPad)
{
    ISDBT_DO_IQ_SWAP_PARAM DoIQSwapParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    DoIQSwapParam.id = id;
    DoIQSwapParam.bIsQPad = bIsQPad;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_DoIQSwap, &DoIQSwapParam) == UTOPIA_STATUS_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    ISDBT_REG_PARAM RegParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    RegParam.id = id;
    RegParam.u16Addr = u16Addr;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_GetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
    {
        *pu8Data = RegParam.u8Data;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data)
{
    ISDBT_REG_PARAM RegParam = {0};

    if (!_u32ISDBTopen) return FALSE;

    RegParam.id = id;
    RegParam.u16Addr = u16Addr;
    RegParam.u8Data = u8Data;

    if (UtopiaIoctl(_ppISDBTInstant, DMD_ISDBT_DRV_CMD_MD_SetReg, &RegParam) == UTOPIA_STATUS_SUCCESS)
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

