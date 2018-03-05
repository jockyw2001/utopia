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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvDSCMB.c
/// @brief  Descrambler (DSCMB) Driver Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <limits.h>
#endif
#include "drvMMIO.h"
#include "drvTSP.h"
#include "drvDSCMB.h"
#include "regDSCMB.h"
#include "halDSCMB.h"
#include "asmCPU.h"
#include "utopia.h"
#include "drvDSCMB_v2.h"
#include "ULog.h"

#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    #include "dscmb_private.h"

    #define DSCMB_UTOPIA2 1
#else
    #define DSCMB_UTOPIA2 0
#endif

// Threshold for KL calculation //
#define DSCMB_KL_WAIT_THRESHOLD 300
#define U32_MAX (0xFFFFFFFFUL)

static MSIF_Version _drv_dscmb_version = {
    .DDI = { DSCMB_DRV_VERSION, },
};

static DSCMB_Info                   DscmbInfo;

static DSCMB_Status _DscmbStatus;
MS_U32 _u32DscmbDGBLevel = 0;

#if DSCMB_UTOPIA2
    extern stDscmbTspMap (*DscmbTspMap)[MAX_NUM];
    extern MS_U32 * _u32FreeSlot;
#else
    stDscmbTspMap DscmbTspMap[ENG_NUM][MAX_NUM];
    static MS_U32 _u32FreeSlot[ENG_NUM];
#endif

static MS_S32 _s32DscMutexId = -1 ;

#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
static DSCMB_KLCfg KLCfgPrev;
#endif

#if DSCMB_UTOPIA2
static void *pu32DscmbInst = NULL;
#endif
extern void DSCMBRegisterToUtopia(void);
MS_BOOL _DSCMB_SlotAlloc(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32* pu32SlotId)
{
#if DSCMB_PARAMETER_TABLE
    *pu32SlotId = (E_DSCMB_KEY_EVEN == KeyType)?(u32DscmbId * 2):(u32DscmbId * 2 + 1);
#else
    MS_U32 u32Slot;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    // Oberon/Uranus should judge if u32DscmbId has been visited
    u32Slot = MAsm_CPU_GetTrailOne(_u32FreeSlot[u32EngId]);
    if (REG_DSCMB_MAX_SLOT <= u32Slot)
    {
        DSCMB_ASSERT_AND_RETURN(0);
        return FALSE;
    }

    _u32FreeSlot[u32EngId] &= ~(1 << u32Slot);
    *pu32SlotId = u32Slot;
#endif
    return TRUE;
}

MS_BOOL _DSCMB_SlotFree(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId)
{
#if DSCMB_PARAMETER_TABLE
#else
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(u32SlotId < REG_DSCMB_MAX_SLOT);
    DSCMB_ASSERT_AND_RETURN(0 == ((_u32FreeSlot[u32EngId]) & (1 << u32SlotId)));

    _u32FreeSlot[u32EngId] |= (1 << u32SlotId);
#endif
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Initialize dscrambler driver and descrambler engine
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// It should be called before calling any other dscrambler DDI functions.
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB_Init(void)
{
    MS_U32 i, j, eng;
    stDscmbTspMap* pMap;
    MS_PHY phyBankSize;
    MS_VIRT u32Bank;
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    MS_VIRT u32OTPBank,u32PMBank;
    MS_PHY phyOTPBankSize, phyPMBankSize;
#endif

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &phyBankSize, MS_MODULE_HW))
    {
        MS_DEBUG_MSG(ULOGE("DSCMB", "[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
        return FALSE;
    }

    HAL_DSCMB_SetBank(u32Bank);

#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    /////////// Set PM Bank & OTP Bank to HAL //////////////////////////////
    extern MS_BOOL HAL_DSCMB_SetOTPBank(MS_U32) __attribute__((weak));
    extern MS_BOOL HAL_DSCMB_SetPMBank(MS_U32) __attribute__((weak));
    if(HAL_DSCMB_SetOTPBank)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32OTPBank, &phyOTPBankSize, MS_MODULE_OTP))
        {
            MS_DEBUG_MSG(ULOGE("DSCMB", "[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
            return FALSE;
        }
        HAL_DSCMB_SetOTPBank(u32OTPBank);
    }

    if(HAL_DSCMB_SetPMBank)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32PMBank, &phyPMBankSize, MS_MODULE_PM))
        {
            MS_DEBUG_MSG(ULOGE("DSCMB", "[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__));
            return FALSE;
        }
        HAL_DSCMB_SetPMBank(u32PMBank);
    }
    //////////////////////////////////////////////////////////
#endif
    HAL_DSCMB_Init();

    for(eng = 0; eng < ENG_NUM; eng++)
    {
        _u32FreeSlot[eng] = (MS_U32)((1ULL << REG_DSCMB_MAX_SLOT) - 1); // no more than 32 slots
        memset((void*)&DscmbTspMap[eng], 0, sizeof(stDscmbTspMap));
        for (i= 0; i< MAX_NUM; i++)
        {
            pMap = &DscmbTspMap[eng][i];
            pMap->u32DscmbId = U32_MAX;
            for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
            {
                pMap->u32PidFltId[j] = U32_MAX;
            }
            pMap->u32SlotIdClear = pMap->u32SlotIdEven = pMap->u32SlotIdOdd = U32_MAX;
            pMap->bUsed = FALSE;
            pMap->bDscmb = TRUE;
            pMap->SlotType = E_SLOT_TYPE_NORMAL;
        }
    }

     _s32DscMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*) "DSCMB_Mutex", MSOS_PROCESS_SHARED);

    return TRUE;
}

MS_BOOL MDrv_DSCMB_Init(void)
{
    return MDrv_DSCMB2_Init();
}

MS_BOOL MDrv_DSCMB2_Init(void)
{
#if DSCMB_UTOPIA2
        MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
        if(pu32DscmbInst == NULL)
        {
            DSCMBRegisterToUtopia();

            #ifdef MSOS_TYPE_LINUX_KERNEL
            if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB | KERNEL_MODE, &pu32DscmbInst, 0x0, NULL))
            #else
            if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, &pu32DscmbInst, 0x0, NULL))
            #endif
            {
                ULOGE("DSCMB", "UtopiaOpen DSCMB fail\n");
                return FALSE;
            }
        }

        u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Init, NULL);
        if(UTOPIA_STATUS_SUCCESS != u32Ret)
        {
            ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
            return FALSE;
        }

        return TRUE;
#else
    return _MDrv_DSCMB_Init();
#endif
}
//--------------------------------------------------------------------------------------------------
/// Exit dscrambler driver and descrambler engine
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// It should be called after finish any other dscrambler DDI functions.
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB_Exit(void)
{
    if (_s32DscMutexId != -1)
    {
        MsOS_DeleteMutex(_s32DscMutexId);
    }
    return TRUE;
}

MS_BOOL MDrv_DSCMB_Exit(void)
{
    return MDrv_DSCMB2_Exit();
}

MS_BOOL MDrv_DSCMB2_Exit(void)
{
#if DSCMB_UTOPIA2
        MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
        if(pu32DscmbInst == NULL)
        {
            return FALSE;
        }

        u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Exit, NULL);
        if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
            ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
            return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB_Exit();
#endif
}

HAL_DSCMB_Type _DSCMB_TypeCasting(DSCMB_Type type)
{
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    switch (type)
    {
        case E_DSCMB_TYPE_CSA:
            return E_HAL_DSCMB_TYPE_CSA;
            break;
        case E_DSCMB_TYPE_CSA_CONF:
            return E_HAL_DSCMB_TYPE_CSA_CONF;
            break;
        case E_DSCMB_TYPE_NSA_AS_ESA:
            return E_HAL_DSCMB_TYPE_NSA_AS_ESA;
            break;
        case E_DSCMB_TYPE_DES:
            return E_HAL_DSCMB_TYPE_DES;
            break;
        case E_DSCMB_TYPE_AES:
            return E_HAL_DSCMB_TYPE_AES;
            break;
        case E_DSCMB_TYPE_AES_ECB:
            return E_HAL_DSCMB_TYPE_AES_ECB;
            break;
        case E_DSCMB_TYPE_AES_SCTE52:
            return E_HAL_DSCMB_TYPE_AES_SCTE52;
            break;
        case E_DSCMB_TYPE_AES_CTR:
            return E_HAL_DSCMB_TYPE_AES_CTR;
            break;
        case E_DSCMB_TYPE_TDES_ECB:
            return E_HAL_DSCMB_TYPE_TDES_ECB;
            break;
        case E_DSCMB_TYPE_TDES_SCTE52:
            return E_HAL_DSCMB_TYPE_TDES_SCTE52;
            break;
        case E_DSCMB_TYPE_SYN_AES:
            return E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC;
            break;
        case E_DSCMB_TYPE_MULTI2:
            return E_HAL_DSCMB_TYPE_MULTI2;
            break;
        case E_DSCMB_TYPE_OC:
            return E_HAL_DSCMB_TYPE_OC;
            break;
        default:
            return E_HAL_DSCMB_TYPE_INVALID;
    }
#else
    return E_HAL_DSCMB_TYPE_INVALID;
#endif
}

MS_BOOL _MDrv_DSCMB2_OptConfig(MS_U32 u32EngId, DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet)
{
    HAL_DSCMB_Option eOption = 0;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    
    switch (eOpt)
    {
        case E_DSCMB_CFG_HDCP2_TX_CONTENT_CATEGORY_SUPPORT:
            eOption = E_HAL_DSCMB_CFG_HDCP2_TX_CONTENT_CATEGORY_SUPPORT;
            break;
        
        case E_DSCMB_CFG_SCMB_STATUS_PUSI_ONLY:
            eOption = E_HAL_DSCMB_CFG_SCMB_STATUS_PUSI_ONLY;
            break;
            
        default:
            return FALSE;
    }
    return HAL_DSCMB_OptConfig(u32EngId, eOption, pInput, pOutput, bSet);
}

//--------------------------------------------------------------------------------------------------
/// query dscrambler capability information
/// @param u32EngId \b IN: dscrambler engine index
/// @param eQueryType \b IN: type for query
/// @param pOutput \b OUT: result for the query type
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput)
{
    HAL_DSCMB_Query_Type u32Cap;

    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(pOutput != NULL);

    switch (eQueryType)
    {
        case E_DSCMB_CAP_ENGINE_NUM:
            u32Cap = E_HAL_DSCMB_CAP_ENGINE_NUM;
            break;
        case E_DSCMB_CAP_FLT_NUM:
            u32Cap = E_HAL_DSCMB_CAP_FLT_NUM;
            break;
        case E_DSCMB_CAP_SUPPORT_ALGORITHM:
            DSCMB_ASSERT_AND_RETURN(pInput != NULL);
            *((HAL_DSCMB_Type *)pInput) = _DSCMB_TypeCasting(*((DSCMB_Type *)pInput));
            u32Cap = E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM;
            break;
        case E_DSCMB_CAP_SUPPORT_KEY_TYPE:
            DSCMB_ASSERT_AND_RETURN(pInput != NULL);
            if((*(DSCMB_Key_Type *)pInput) == E_DSCMB_KEY_CLEAR)
                return FALSE;
            else
                return TRUE;
            break;
        case E_DSCMB_CAP_PIDFILTER_MAP_START:
            u32Cap = E_HAL_DSCMB_CAP_PIDFILTER_MAP_START;
            break;
        case E_DSCMB_CAP_PIDFILTER_MAP_END:
            u32Cap = E_HAL_DSCMB_CAP_PIDFILTER_MAP_END;
            break;
#if ( defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
        case E_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_NUM:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_START:
            DSCMB_ASSERT_AND_RETURN((pInput != NULL) && (*((MS_U32*)pInput) < E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM));
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_END:
            DSCMB_ASSERT_AND_RETURN((pInput != NULL) && (*((MS_U32*)pInput) < E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM));
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END;
            break;
#endif
        default:
            return FALSE;
            break;
    }

    return HAL_DSCMB_GetCap(u32EngId, u32Cap, pInput, pOutput);
}

MS_BOOL MDrv_DSCMB2_OptConfig(MS_U32 u32EngId, DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_OPTCONFIG pArgs;
    if(pu32DscmbInst == NULL)
    {
        #ifdef MSOS_TYPE_LINUX_KERNEL
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB | KERNEL_MODE, (void**)&pu32DscmbInst, 0x0, NULL))
        #else
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, (void**)&pu32DscmbInst, 0x0, NULL))
        #endif
        {
            ULOGE("DSCMB", "UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }

    pArgs.u32EngId = u32EngId;
    pArgs.eOpt = eOpt;
    pArgs.pInput = pInput;
    pArgs.pOutput = pOutput;
    pArgs.bSet = bSet;
    
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_OptConfig, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_OptConfig(u32EngId, eOpt, pInput, pOutput, bSet);
#endif
}

MS_BOOL MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_QUERYCAP pArgs;
    if(pu32DscmbInst == NULL)
    {
        #ifdef MSOS_TYPE_LINUX_KERNEL
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB | KERNEL_MODE, (void**)&pu32DscmbInst, 0x0, NULL))
        #else
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, (void**)&pu32DscmbInst, 0x0, NULL))
        #endif
        {
            ULOGE("DSCMB", "UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }

    pArgs.u32EngId = u32EngId;
    pArgs.eQueryType = eQueryType;
    pArgs.pInput = pInput;
    pArgs.pOutput = pOutput;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_QueryCap, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_GetCap(u32EngId, eQueryType, pInput, pOutput);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter
/// @return Descrambler filter ID - Success
/// @return DRV_DSCMB_FLT_NULL - Failure
//--------------------------------------------------------------------------------------------------
MS_U32 MDrv_DSCMB2_FltAlloc(MS_U32 u32EngId)
{
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    return MDrv_DSCMB2_FltAlloc_Ex(u32EngId, E_DSCMB_FLT_3_KEYS);
#else
    return MDrv_DSCMB2_FltAlloc_Ex(u32EngId, E_DSCMB_FLT_2_KEYS);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter
/// @param eFltType \b IN: type for descramber filter
/// @return Descrambler filter ID - Success
/// @return DRV_DSCMB_FLT_NULL - Failure
//--------------------------------------------------------------------------------------------------
MS_U32 _MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType)
{
    MS_U32 i;
    stDscmbTspMap* pMap;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);

#if DSCMB_SHAREFLT_ENABLE
    // check share key slot already exist or not
    if((eFltType == E_DSCMB_FLT_2_KEYS_SHARE) && (DscmbTspMap[u32EngId][DSCMB_SHARE_SLOT_REV_IDX].bUsed == TRUE))
    {
        return DscmbTspMap[u32EngId][DSCMB_SHARE_SLOT_REV_IDX].u32DscmbId;
        }
#else
    if(eFltType == E_DSCMB_FLT_2_KEYS_SHARE)
    {
        return DRV_DSCMB_FLT_NULL;
    }
#endif

    // try to find free slot
    for (i= 0; i< MAX_NUM; i++)
    {
#if DSCMB_SHAREFLT_ENABLE
        if(eFltType == E_DSCMB_FLT_2_KEYS_SHARE)
        { // share type direct allocate reserved slot.
            i = DSCMB_SHARE_SLOT_REV_IDX;
        }
        else if((eFltType == E_DSCMB_FLT_2_KEYS) && (i == DSCMB_SHARE_SLOT_REV_IDX))
        { // individual type not allocate reserved slot.
            break;
        }
#endif

        if (DscmbTspMap[u32EngId][i].bUsed == FALSE)
        {
            pMap = &DscmbTspMap[u32EngId][i];
            pMap->bUsed = TRUE;
            pMap->u32DscmbId = i;
            pMap->u32PidFltIdNum = 0;

            if(eFltType == E_DSCMB_FLT_2_KEYS_SHARE)
                pMap->SlotType = E_SLOT_TYPE_SHARE;
            else
                pMap->SlotType = E_SLOT_TYPE_NORMAL;

#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
            if(eFltType == E_DSCMB_FLT_3_KEYS)
            {
                if (FALSE == _DSCMB_SlotAlloc(u32EngId, i, E_DSCMB_KEY_CLEAR, &(pMap->u32SlotIdClear)))
                {
                    DSCMB_ASSERT_NO_RETURN(0);
                    return DRV_DSCMB_FLT_NULL;
                }
            }
#endif
            if (FALSE == _DSCMB_SlotAlloc(u32EngId, i, E_DSCMB_KEY_EVEN, &(pMap->u32SlotIdEven)))
            {
                DSCMB_ASSERT_NO_RETURN(0);
                return DRV_DSCMB_FLT_NULL;
            }
            if (FALSE == _DSCMB_SlotAlloc(u32EngId, i, E_DSCMB_KEY_ODD, &(pMap->u32SlotIdOdd)))
            {
                DSCMB_ASSERT_NO_RETURN(0);
                return DRV_DSCMB_FLT_NULL;
            }
            return DscmbTspMap[u32EngId][i].u32DscmbId;
        }
    }
    return DRV_DSCMB_FLT_NULL;
}

MS_U32 MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTALLOC pArgs;
    if(pu32DscmbInst == NULL)
    {
        return DRV_DSCMB_FLT_NULL;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.eFltType = eFltType;
    pArgs.u32DscmbId = 0xFF;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltAlloc, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_DSCMB_FLT_NULL;
    }

    return pArgs.u32DscmbId;

#else
    return _MDrv_DSCMB2_FltAlloc_Ex(u32EngId, eFltType);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Free a dscrambler filter
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
    stDscmbTspMap* pMap;
    MS_U32 i;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    if (U32_MAX != pMap->u32DscmbId)
    {
        // can't free this slot, if there has the other filter connect to this one when use share key.
        if((pMap->SlotType == E_SLOT_TYPE_SHARE) && (pMap->u32PidFltIdNum != 0))
        {
            //ULOGE("DSCMB", "[Share key slot] Can't free this slot due to the PidFltIdNum not equal to 0 !!\n");
            return FALSE;
        }

#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
        if (U32_MAX != pMap->u32SlotIdClear)
        {
            _MDrv_DSCMB2_FltKeyReset(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR);
        }
#endif
        if (U32_MAX != pMap->u32SlotIdOdd)
        {
            _MDrv_DSCMB2_FltKeyReset(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD);
        }
        if (U32_MAX != pMap->u32SlotIdEven)
        {
            _MDrv_DSCMB2_FltKeyReset(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN);
        }
    }

    for (i= 0; i< MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (U32_MAX != pMap->u32PidFltId[i])
        {
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
            if (U32_MAX != pMap->u32SlotIdClear)
            {
                if(FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear, pMap->u32PidFltId[i]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
            }
#endif
            if (U32_MAX != pMap->u32SlotIdOdd)
            {
                if(FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, pMap->u32PidFltId[i]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
            }
            if (U32_MAX != pMap->u32SlotIdEven)
            {
                if(FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, pMap->u32PidFltId[i]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
            }
            pMap->u32PidFltId[i] = U32_MAX;
        }
    }


#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    if (U32_MAX != pMap->u32SlotIdClear)
    {
        if (FALSE == _DSCMB_SlotFree(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear))
        {
            DSCMB_ASSERT_AND_RETURN(0);
        }
    }
#endif
    if (FALSE == _DSCMB_SlotFree(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven))
    {
        DSCMB_ASSERT_AND_RETURN(0);
    }
    if (FALSE == _DSCMB_SlotFree(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd))
    {
        DSCMB_ASSERT_AND_RETURN(0);
    }

    if(pMap->SlotType == E_SLOT_TYPE_SHARE)
    {
        pMap->SlotType = E_SLOT_TYPE_NORMAL;
    }

    pMap->bDscmb= TRUE;
    pMap->bUsed = FALSE;
    pMap->u32PidFltIdNum = 0;
    pMap->u32SlotIdClear = pMap->u32SlotIdEven = pMap->u32SlotIdOdd = U32_MAX;
    pMap->u32DscmbId = U32_MAX;

    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTFREE pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_FLTFREE));

    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltFree, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltFree(u32EngId, u32DscmbId);
#endif
}

#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
//--------------------------------------------------------------------------------------------------
/// Make connections between the Dscmb ID with the targeted Pid to be descrambled
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param u32Pid \b IN: The targeted Pid to be descrambled
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    MS_BOOL bConnected = FALSE;
    MS_U32 u32Max_TspFltNum = 0;
    MS_U32 u32Min_TspFltNum = 0;
    MS_U32 u32Fltid = 0;
    MS_U32 u32Pid_Temp = 0x1FFF;
    MS_U32 j = 0;

    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)) )
    if(MDrv_TSP_GetCap(E_TSP_CAP_PIDFLT_NUM, &u32Max_TspFltNum) != E_TSP_OK)
        DSCMB_ASSERT_AND_RETURN(0);
#else
    MS_U32 i = 0;
    MS_U32 u32ShareRegionNum = 0;
    MS_U32 u32CapInput = 0;

    if(DscmbTspMap[u32EngId][u32DscmbId].SlotType == E_SLOT_TYPE_SHARE)
    {
        if(HAL_DSCMB_GetCap(u32EngId, E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM, &u32CapInput, &u32ShareRegionNum) != TRUE)
            DSCMB_ASSERT_AND_RETURN(0);
    }
    else
        u32ShareRegionNum = 1;

    for (i = 0; i < u32ShareRegionNum; i++)
    {
        if(DscmbTspMap[u32EngId][u32DscmbId].SlotType == E_SLOT_TYPE_SHARE)
        {// Search for share key region
            if(HAL_DSCMB_GetCap(u32EngId, E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START, &i, &u32Min_TspFltNum) != TRUE)
                DSCMB_ASSERT_AND_RETURN(0);
            if(HAL_DSCMB_GetCap(u32EngId, E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END, &i, &u32Max_TspFltNum) != TRUE)
                DSCMB_ASSERT_AND_RETURN(0);
        }
        else
#if ( defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0) )
        {// Search for individual region.
            if(MDrv_TSP_GetDscmbFltId_Range(u32EngId, &u32Min_TspFltNum, &u32Max_TspFltNum) != DRVTSP_OK)
                DSCMB_ASSERT_AND_RETURN(0);
        }
#else
        {// Search for individual region.
            if(HAL_DSCMB_GetCap(u32EngId, E_HAL_DSCMB_CAP_PIDFILTER_MAP_START, NULL, &u32Min_TspFltNum) != TRUE)
                DSCMB_ASSERT_AND_RETURN(0);
            if(HAL_DSCMB_GetCap(u32EngId, E_HAL_DSCMB_CAP_PIDFILTER_MAP_END, NULL, &u32Max_TspFltNum) != TRUE)
                DSCMB_ASSERT_AND_RETURN(0);
        }
#endif
#endif

        for (j = u32Min_TspFltNum; j < u32Max_TspFltNum; j++)
        {
// @TODO: it's not good to call driver by driver.
// Plan to add a new function for upper layer to assign pidfltid
            if ((DRVTSP_OK == MDrv_TSP_PidFlt_GetPid(0, j, &u32Pid_Temp)) && (u32Pid_Temp == u32Pid)) // find the match pid filter index
            {
                #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))
                u32Fltid = HAL_DSCMB_TransFltId2FltGroup0Id(j);
                #else
                u32Fltid = j;
                #endif
                if(_MDrv_DSCMB2_FltConnectFltId(u32EngId, u32DscmbId, u32Fltid) == FALSE)
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
                bConnected = TRUE;
            }
        }
#if !( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)) )
    }
#endif

    return bConnected;
}

MS_BOOL MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_CONNECTPID pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_CONNECTPID));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32Pid = u32Pid;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltConnectPid, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltConnectPid(u32EngId, u32DscmbId, u32Pid);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Break connections between the Dscmb ID with the targeted Pid
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param u32Pid \b IN: The targeted Pid to be descrambled
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltDisconnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    stDscmbTspMap* pMap;
    MS_U32 u32Pid_Temp = 0x1FFF;
    MS_U32 j = 0;
    TSP_Result result;
    MS_U32 u32Fltid = 0;

    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];


    DSCMB_ASSERT_AND_RETURN(U32_MAX!= pMap->u32SlotIdEven);
    DSCMB_ASSERT_AND_RETURN(U32_MAX!= pMap->u32SlotIdOdd);

    for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
    {
        if (U32_MAX != pMap->u32PidFltId[j])
        {
            // just dis-connect the filter which PID matched when use share key
            if(pMap->SlotType == E_SLOT_TYPE_SHARE)
            {
                #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))
                u32Fltid = HAL_DSCMB_TransFltGroup0Id2Fltd(pMap->u32PidFltId[j]);
                #else
                u32Fltid = pMap->u32PidFltId[j];
                #endif
                result = MDrv_TSP_PidFlt_GetPid(0, u32Fltid, &u32Pid_Temp);
                if ((DRVTSP_OK == result) && (u32Pid_Temp != u32Pid))
                    continue;
                else if(DRVTSP_FAIL == result)
                {
                    ULOGE("DSCMB", "Can't find the filter which PID:[0x%x] by FltIdx:[%d]!!\n", (int)u32Pid, (int)pMap->u32PidFltId[j]);
                    DSCMB_ASSERT_AND_RETURN(0);
                }
            }

            pMap->u32PidFltIdNum--;
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
            if (U32_MAX != pMap->u32SlotIdClear)
            {
                if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear, pMap->u32PidFltId[j]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
            }
#endif
            if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, pMap->u32PidFltId[j]))
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }
            if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, pMap->u32PidFltId[j]))
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }
        }
    }

    for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
    {
        pMap->u32PidFltId[j] = U32_MAX;
    }

    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltDisconnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DISCONNECTPID pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DISCONNECTPID));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32Pid = u32Pid;
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltDisconnectPid, (void*)&pArgs);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDisconnectPid(u32EngId, u32DscmbId, u32Pid);
#endif
}

#if 1
//--------------------------------------------------------------------------------------------------
/// Break connections between the Dscmb ID with the targeted Pid
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param u32Pid \b IN: The targeted Pid to be descrambled
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB2_FltDisconnectPid_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    stDscmbTspMap* pMap;
    MS_U32 j,u32Flt_Pid = 0 ;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];


    DSCMB_ASSERT_AND_RETURN(U32_MAX!= pMap->u32SlotIdEven);
    DSCMB_ASSERT_AND_RETURN(U32_MAX!= pMap->u32SlotIdOdd);

    for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
    {
        if ((U32_MAX != pMap->u32PidFltId[j]) )
        {
            u32Flt_Pid = 0 ;
            if ((DRVTSP_OK == MDrv_TSP_PidFlt_GetPid(0, j, &u32Flt_Pid) )&& ( u32Flt_Pid == u32Pid ))
            {
                pMap->u32PidFltIdNum--;
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
                if (U32_MAX != pMap->u32SlotIdClear)
                {
                    if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear, pMap->u32PidFltId[j]))
                    {
                        DSCMB_ASSERT_AND_RETURN(0);
                        return FALSE;
                    }
                }
#endif
                if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, pMap->u32PidFltId[j]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
                if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, pMap->u32PidFltId[j]))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
                pMap->u32PidFltId[j] = U32_MAX;
            }
        }
    }

    return TRUE;
}
#endif

#endif //MSOS_TYPE_NUTTX | MSOS_TYPE_OPTEE
//--------------------------------------------------------------------------------------------------
/// Make connections between the Dscmb ID with the targeted filter id to be descrambled
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param u32Pid \b IN: The targeted filter id to be descrambled
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
    stDscmbTspMap* pMap=NULL;
    MS_U32 i=0;

    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);
    DSCMB_ASSERT_AND_RETURN(MAX_DSCMB_PIDFLT_NUM > DscmbTspMap[u32EngId][u32DscmbId].u32PidFltIdNum);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    DSCMB_ASSERT_AND_RETURN(MAX_DSCMB_PIDFLT_NUM> pMap->u32PidFltIdNum);

#if DSCMB_SHAREFLT_ENABLE
    // check dscmb idx & pid flt idx range is match or not
    if(HAL_DSCMB_CheckShareConnect(u32EngId, u32FltId, pMap->SlotType) == FALSE)
    {
        DSCMB_ASSERT_AND_RETURN(0);
        return FALSE;
    }
#endif

    // check dscmb connected filters //
    for( i = 0 ; i < MAX_DSCMB_PIDFLT_NUM ; i ++)
    {
        if (pMap->u32PidFltId[i] == u32FltId)
        {
            //ULOGE("DSCMB", "[%s][%d] Dscmb %d is already connected to Filter %d \n", __FUNCTION__,__LINE__,u32DscmbId,u32FltId);
            return TRUE ;
        }
    }

#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    // return FALSE if tsp filter try to connect to another dscmbid ( duplicate dscmb connction)
    MS_U32 j=0;
    MS_U32* pu32FltId = NULL;

    for (i = 0; i < MAX_NUM; i++)
    {
        if( (DscmbTspMap[u32EngId][i].bUsed) && (i != u32DscmbId) )
        {
            pu32FltId = DscmbTspMap[u32EngId][i].u32PidFltId;

            for (j = 0; j < MAX_DSCMB_PIDFLT_NUM ; j++)
            {
                if(pu32FltId[j] == u32FltId)
                {
                    MS_DEBUG_MSG( ULOGE("DSCMB", "[%s][%d] Duplicate descramble connection from dscmbid=%lu to dscmbid=%lu\n", __FUNCTION__, __LINE__ ,u32DscmbId, i));
                    ULOGE("DSCMB", "duplicate connection\n");
                    return FALSE;
                }
            }
        }
    }

    if (U32_MAX != pMap->u32SlotIdClear)
    {
        if (FALSE == HAL_DSCMB_SlotConnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear, u32FltId))
        {
            DSCMB_ASSERT_AND_RETURN(0);
            return FALSE;
        }
    }
#endif
    if (FALSE == HAL_DSCMB_SlotConnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, u32FltId))
    {
        DSCMB_ASSERT_AND_RETURN(0);
        return FALSE;
    }
    if (FALSE == HAL_DSCMB_SlotConnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, u32FltId))
    {
        DSCMB_ASSERT_AND_RETURN(0);
        return FALSE;
    }

    for (i = 0; i < MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (pMap->u32PidFltId[i] == U32_MAX)
        {
            pMap->u32PidFltId[i] = u32FltId;
    pMap->u32PidFltIdNum++;
            break;
        }
    }

    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_CONNECTFLT pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_CONNECTFLT));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32DmxFltId = u32FltId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltConnectFltId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltConnectFltId(u32EngId, u32DscmbId, u32FltId);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Disconnect between the Dscmb ID with the targeted filter id to be descrambled
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param u32Pid \b IN: The targeted filter id to be descrambled
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
    stDscmbTspMap* pMap;
    MS_U32 j ;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    DSCMB_ASSERT_AND_RETURN(MAX_DSCMB_PIDFLT_NUM>= pMap->u32PidFltIdNum);

    for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
    {
        if (pMap->u32PidFltId[j] == u32FltId)
        {
            pMap->u32PidFltIdNum--;
            pMap->u32PidFltId[j] = U32_MAX;

            if((pMap->SlotType == E_SLOT_TYPE_SHARE) && (pMap->u32PidFltIdNum != 0))
                return TRUE;

#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
            if (U32_MAX != pMap->u32SlotIdClear)
            {
                if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear, u32FltId))
                {
                    DSCMB_ASSERT_AND_RETURN(0);
                    return FALSE;
                }
            }
#endif
            if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, u32FltId))
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }
            if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, u32FltId))
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }

        }

    }

    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DISCONNECTFLT pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DISCONNECTFLT));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32DmxFltId = u32FltId;
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltDisconnectFltId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDisconnectFltId(u32EngId, u32DscmbId, u32FltId);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Set descrambler filter as scrambling/decrambling mode
/// Default filter mode is descrambling if this function is not specified
/// This function should be invoked before MDrv_DSCMB_FltTypeSe
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param DscmbType \b IN: The descrambler algorithm
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb)
{
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    DscmbTspMap[u32EngId][u32DscmbId].bDscmb= (bDscmb)? TRUE: FALSE;
    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTDSCMB pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_FLTDSCMB));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.bDscmb = bDscmb;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltDscmb, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDscmb(u32EngId, u32DscmbId, bDscmb);
#endif
}

//--------------------------------------------------------------------------------------------------
// [RESERVED] The low-level implementation of @ref MDrv_DSCMB_FltTypeSet
// @param u32DscmbId \b IN: descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
// @param eKeyType \b IN: key type for this setting
// @param u32SW0 \b IN: The descramlber switch configuration
// @param u32SW1 \b IN: reserved
// @param u32SW2 \b IN: reserved
// @return TRUE - Success
// @return FALSE - Failure
// @sa MDrv_DSCMB_FltTypeSet
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_SlotSwitchWrite(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U32 u32SW0, MS_U32 u32SW1, MS_U32 u32SW2)
{
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    HAL_DSCMB_SlotSwitchWrite(u32DscmbId, eKeyType, u32SW0, REG_DSCMB_POS_SWITCH13);
    // u32SW1, u32SW2 are reserved

    return TRUE;
#else
    return FALSE;
#endif
}

//--------------------------------------------------------------------------------------------------
/// Enable Record stream after stream is descrambled, only supported when CA venders do not prohibat this feature.
/// @Note: Function can only be called before any record function is called.
/// @param u32EngId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param Enable \b IN: for enabe/disable record mechanism.
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB2_PVR_RecCtrl(MS_U32 u32EngId, MS_BOOL Enable)
{
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);

    HAL_DSCMB_PVR_RecCtrl( u32EngId,   Enable);

    return TRUE;
#else
    return FALSE;
#endif    
}
//--------------------------------------------------------------------------------------------------
/// Set descrambler algorithm by common predefined descramble type
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param DscmbType \b IN: The descrambler algorithm
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// both dscmb engines operate on same algorithm
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type DscmbType)
{
    HAL_DSCMB_Type hal_dscmb_type;
    stDscmbTspMap* pMap;
    MS_BOOL bSet = FALSE;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    pMap->DscmbType = DscmbType;

    switch (DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA;
        break;
    case E_DSCMB_TYPE_CSA_CONF:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA_CONF;
        break;
    case E_DSCMB_TYPE_NSA_AS_ESA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_NSA_AS_ESA;
        break;
    case E_DSCMB_TYPE_DES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_DES;
        break;
    case E_DSCMB_TYPE_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES;
        break;
    case E_DSCMB_TYPE_AES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_ECB;
        break;
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case E_DSCMB_TYPE_AES_CTR:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_CTR;
        break;
#endif
    case E_DSCMB_TYPE_AES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_SCTE52;
        break;
    case E_DSCMB_TYPE_TDES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_ECB;
        break;
    case E_DSCMB_TYPE_TDES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_SCTE52;
        break;
    case E_DSCMB_TYPE_SYN_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC;
        break;
#if defined(DSCMB_MULTI2_ENABLE)
    case E_DSCMB_TYPE_MULTI2:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_MULTI2;
        break;
#endif
    case E_DSCMB_TYPE_OC:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_OC;
        break;
    case E_DSCMB_TYPE_CSA3:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA3;
        break;
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }

#if DSCMB_PARAMETER_TABLE
    bSet = HAL_DSCMB_FltTypeSet(u32EngId, 0, pMap->u32SlotIdEven, hal_dscmb_type, pMap->bDscmb);
    if(FALSE == bSet)
        DSCMB_ASSERT_AND_RETURN(0);
#else
    MS_U32 i;
    for (i= 0; i< MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (U32_MAX != pMap->u32PidFltId[i])
        {
#if defined(NDS_IMPL)
//TODO: MDrv_DSCMB for NDS
#endif
            bSet = HAL_DSCMB_FltTypeSet(u32EngId, pMap->u32PidFltId[i], hal_dscmb_type, pMap->bDscmb);
            if(FALSE == bSet)
                DSCMB_ASSERT_AND_RETURN(0);
        }
    }
#endif
    return bSet;
}

MS_BOOL MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type DscmbType)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_TYPESET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_TYPESET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.eType = DscmbType;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltTypeSet, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltTypeSet(u32EngId, u32DscmbId, DscmbType);
#endif
}

MS_BOOL _MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType)
{
    DSCMB_Key_Type hal_dscmb_key_type;
    HAL_DSCMB_Type hal_dscmb_type;
    MS_U32 u32Slot;
    stDscmbTspMap* pMap;

    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    switch (eKeyType)
    {
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    case E_DSCMB_KEY_CLEAR:
        hal_dscmb_key_type = E_DSCMB_KEY_CLEAR;
        u32Slot = pMap->u32SlotIdClear;
        break;
#endif
    case E_DSCMB_KEY_ODD:
        hal_dscmb_key_type = E_DSCMB_KEY_ODD;
        u32Slot = pMap->u32SlotIdOdd;
        break;
    case E_DSCMB_KEY_EVEN:
        hal_dscmb_key_type = E_DSCMB_KEY_EVEN;
        u32Slot = pMap->u32SlotIdEven;
        break;
    default:
        return FALSE;
    }

    if (U32_MAX == u32Slot)
    {
        return FALSE;
    }

    switch (pMap->DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA;
        break;
    case E_DSCMB_TYPE_CSA_CONF:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA_CONF;
        break;
    case E_DSCMB_TYPE_NSA_AS_ESA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_NSA_AS_ESA;
        break;
    case E_DSCMB_TYPE_DES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_DES;
        break;
    case E_DSCMB_TYPE_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES;
        break;
    case E_DSCMB_TYPE_AES_CTR:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_CTR;
        break;
    case E_DSCMB_TYPE_AES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_ECB;
        break;
    case E_DSCMB_TYPE_AES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_SCTE52;
        break;
    case E_DSCMB_TYPE_TDES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_ECB;
        break;
    case E_DSCMB_TYPE_TDES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_SCTE52;
        break;
    case E_DSCMB_TYPE_SYN_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC;
        break;
    case E_DSCMB_TYPE_CSA3:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA3;
        break;
#if defined(DSCMB_MULTI2_ENABLE)
    case E_DSCMB_TYPE_MULTI2:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_MULTI2;
        break;
#endif
    case E_DSCMB_TYPE_OC:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_OC;
        break;

    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }

#if DSCMB_PARAMETER_TABLE
    HAL_DSCMB_FltKeyReset(u32EngId, 0, u32Slot, hal_dscmb_type, hal_dscmb_key_type);
#else
    for (i= 0; i< MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (U32_MAX != pMap->u32PidFltId[i])
        {
            HAL_DSCMB_FltKeyReset(u32EngId, pMap->u32PidFltId[i], u32Slot, hal_dscmb_type, hal_dscmb_key_type);
        }
    }
#endif
    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTKEYRESET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_FLTKEYRESET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.eKeyType = eKeyType;
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltKeyReset, (void*)&pArgs);

    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltKeyReset(u32EngId, u32DscmbId, eKeyType);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Set descrambler key of the Dscmb ID
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param eKeyType \b IN: Descrambler key type
/// @param pu8Key \b IN: pointer to the key
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    DSCMB_Key_Type hal_dscmb_key_type;
    HAL_DSCMB_Type hal_dscmb_type;
    MS_U32 u32Slot;
    stDscmbTspMap* pMap;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    switch (eKeyType)
    {
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    case E_DSCMB_KEY_CLEAR:
        hal_dscmb_key_type = E_DSCMB_KEY_CLEAR;
        u32Slot = pMap->u32SlotIdClear;
        break;
#endif
    case E_DSCMB_KEY_ODD:
        hal_dscmb_key_type = E_DSCMB_KEY_ODD;
        u32Slot = pMap->u32SlotIdOdd;
        break;
    case E_DSCMB_KEY_EVEN:
        hal_dscmb_key_type = E_DSCMB_KEY_EVEN;
        u32Slot = pMap->u32SlotIdEven;
        break;
    default:
        return FALSE;
    }

    if (U32_MAX == u32Slot)
    {
        return FALSE;
    }

    switch (pMap->DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA;
        break;
    case E_DSCMB_TYPE_CSA_CONF:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA_CONF;
        break;
    case E_DSCMB_TYPE_NSA_AS_ESA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_NSA_AS_ESA;
        break;
    case E_DSCMB_TYPE_DES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_DES;
        break;
    case E_DSCMB_TYPE_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES;
        break;
    case E_DSCMB_TYPE_AES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_ECB;
        break;
    case E_DSCMB_TYPE_AES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_SCTE52;
        break;
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case E_DSCMB_TYPE_AES_CTR:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_CTR;
        break;
#endif
    case E_DSCMB_TYPE_TDES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_ECB;
        break;
    case E_DSCMB_TYPE_TDES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_SCTE52;
        break;
    case E_DSCMB_TYPE_SYN_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC;
        break;
#if defined(DSCMB_MULTI2_ENABLE)
    case E_DSCMB_TYPE_MULTI2:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_MULTI2;
        break;
#endif
    case E_DSCMB_TYPE_OC:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_OC;
        break;
    case E_DSCMB_TYPE_CSA3:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA3;
        break;
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }

#if DSCMB_PARAMETER_TABLE
    if(hal_dscmb_type == E_HAL_DSCMB_TYPE_AES_CTR)
    {   
        MS_U32 u32TmpSlot = (hal_dscmb_key_type == E_DSCMB_KEY_ODD) ? pMap->u32SlotIdEven : pMap->u32SlotIdOdd ;
        DSCMB_Key_Type key_type = (hal_dscmb_key_type == E_DSCMB_KEY_ODD) ? E_DSCMB_KEY_EVEN : E_DSCMB_KEY_ODD;
        if(FALSE == HAL_DSCMB_FltKeySet(u32EngId, 0, u32TmpSlot, hal_dscmb_type, key_type, pu8Key))
            DSCMB_ASSERT_AND_RETURN(0);
    }
    if(FALSE == HAL_DSCMB_FltKeySet(u32EngId, 0, u32Slot, hal_dscmb_type, hal_dscmb_key_type, pu8Key)) // AES: IV: 0, 1, 2, 3.   CW: 4, 5, 6, 7
        DSCMB_ASSERT_AND_RETURN(0);
#else
    MS_U32 i;
    for (i= 0; i< MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (U32_MAX != pMap->u32PidFltId[i])
        {
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
            // for HDCP2, set EVEN/ODD key at the same time
            if(hal_dscmb_type == E_HAL_DSCMB_TYPE_AES_CTR)
            {   
                MS_U32 u32TmpSlot = (hal_dscmb_key_type == E_DSCMB_KEY_ODD) ? pMap->u32SlotIdEven : pMap->u32SlotIdOdd ;
                DSCMB_Key_Type key_type = (hal_dscmb_key_type == E_DSCMB_KEY_ODD) ? E_DSCMB_KEY_EVEN : E_DSCMB_KEY_ODD;
                if(FALSE == HAL_DSCMB_FltKeySet(u32EngId, pMap->u32PidFltId[i], u32TmpSlot, hal_dscmb_type, key_type, pu8Key))
                    DSCMB_ASSERT_AND_RETURN(0);
            }
#endif            
            if(FALSE == HAL_DSCMB_FltKeySet(u32EngId, pMap->u32PidFltId[i], u32Slot, hal_dscmb_type, hal_dscmb_key_type, pu8Key)) // AES: IV: 0, 1, 2, 3.   CW: 4, 5, 6, 7
                DSCMB_ASSERT_AND_RETURN(0);
        }
    }
#endif    
    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTKEYSET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_FLTKEYSET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId      = u32EngId;
    pArgs.u32DscmbId    = u32DscmbId;
    pArgs.eKeyType      = eKeyType;
    pArgs.pu8Key        = pu8Key;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltKeySet, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltKeySet(u32EngId, u32DscmbId, eKeyType, pu8Key);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Set descrambler initial vector of the Dscmb ID
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param eKeyType \b IN: Descrambler key type
/// @param pu8IV \b IN: pointer to the key
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// This API is only valid for descrambler algorithm, E_DSCMB_TYPE_AES
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    DSCMB_Key_Type hal_dscmb_key_type;
    MS_U32 u32Slot;
    stDscmbTspMap* pMap;
    HAL_DSCMB_Type DscmbType = (HAL_DSCMB_Type)E_DSCMB_TYPE_AES;
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    switch (eKeyType)
    {
#if ( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    case E_DSCMB_KEY_CLEAR:
        hal_dscmb_key_type = E_DSCMB_KEY_CLEAR;
        u32Slot = pMap->u32SlotIdClear;
        break;
#endif
    case E_DSCMB_KEY_ODD:
        hal_dscmb_key_type = E_DSCMB_KEY_ODD;
        u32Slot = pMap->u32SlotIdOdd;
        break;
    case E_DSCMB_KEY_EVEN:
        hal_dscmb_key_type = E_DSCMB_KEY_EVEN;
        u32Slot = pMap->u32SlotIdEven;
        break;
    default:
        return FALSE;
    }

    if (U32_MAX == u32Slot)
    {
        return FALSE;
    }

    switch (pMap->DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
    case E_DSCMB_TYPE_CSA_CONF:
    case E_DSCMB_TYPE_NSA_AS_ESA:
    case E_DSCMB_TYPE_DES:
    case E_DSCMB_TYPE_AES_ECB:
    case E_DSCMB_TYPE_TDES_ECB:
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    case E_DSCMB_TYPE_AES_SCTE52:
        DscmbType = (HAL_DSCMB_Type)E_HAL_DSCMB_TYPE_AES_SCTE52;
        break;
    case E_DSCMB_TYPE_AES:
        DscmbType = (HAL_DSCMB_Type)E_HAL_DSCMB_TYPE_AES;
        break;
#if defined(DSCMB_MULTI2_ENABLE)
    case E_DSCMB_TYPE_MULTI2:
        DscmbType = (HAL_DSCMB_Type)E_HAL_DSCMB_TYPE_MULTI2;
        break;
#endif
    case E_DSCMB_TYPE_TDES_SCTE52:
        DscmbType = (HAL_DSCMB_Type)E_HAL_DSCMB_TYPE_TDES_SCTE52;
        break;
    case E_DSCMB_TYPE_OC:
        DscmbType = (HAL_DSCMB_Type)E_HAL_DSCMB_TYPE_OC;
        break;

    case E_DSCMB_TYPE_SYN_AES:
        break;
    }

#if DSCMB_PARAMETER_TABLE
    if(FALSE == HAL_DSCMB_FltIVSet(u32EngId, 0, u32Slot, DscmbType, hal_dscmb_key_type, pu8IV)) // AES: IV: 0, 1, 2, 3.   CW: 4, 5, 6, 7
        DSCMB_ASSERT_AND_RETURN(0);
#else
    MS_U32 i;
    for (i= 0; i< MAX_DSCMB_PIDFLT_NUM; i++)
    {
        if (U32_MAX != pMap->u32PidFltId[i])
        {
            if(FALSE == HAL_DSCMB_FltIVSet(u32EngId, pMap->u32PidFltId[i], u32Slot, DscmbType, hal_dscmb_key_type, pu8IV)) // AES: IV: 0, 1, 2, 3.   CW: 4, 5, 6, 7
                DSCMB_ASSERT_AND_RETURN(0);
        }
    }
#endif
    return TRUE;
}

MS_BOOL MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTIVSET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_FLTIVSET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId      = u32EngId;
    pArgs.u32DscmbId    = u32DscmbId;
    pArgs.eKeyType      = eKeyType;
    pArgs.pu8IV         = pu8IV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltIVSet, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltIVSet(u32EngId, u32DscmbId, eKeyType, pu8IV);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Set rounds of descrambler algorithm MULTI2
/// @param u32EngId \b IN: cipher engine index
/// @param u32Round \b IN: # rounds to apply, multiple of 4 and maximum value is 252
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// This API is only valid for descrambler algorithm, E_DSCMB_TYPE_MULTI2
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    return HAL_DSCMB_Multi2_SetRound(u32EngId, u32Round);
#else
    return FALSE;
#endif    
}

MS_BOOL MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_MULTI2_SETROUND pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_MULTI2_SETROUND));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId      = u32EngId;
    pArgs.u32Round      = u32Round;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Multi2_SetRound, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_Multi2_SetRound(u32EngId, u32Round);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Set system key of descrambler algorithm MULTI2
/// @param u32EngId \b IN: cipher engine index
/// @param u32SysKey \b IN: the array of system key (u8SysKey[32]): there are 32 group system key(8-bit should be 32 group) should be set.
/// @[0-31][32-63][64-95][96-127][128-159][160-191][192-223][224-255] and the bit order is MSB.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// This API is only valid for descrambler algorithm, E_DSCMB_TYPE_MULTI2
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey)
{
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))    
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    return HAL_DSCMB_Multi2_SetSystemKey(u32EngId, u8SysKey);
#else
    return FALSE;
#endif    
}

MS_BOOL MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_MULTI2_SETSYSKEY pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_MULTI2_SETSYSKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId       = u32EngId;
    pArgs.pu8SysKey      = u8SysKey;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Multi2_SetSysKey, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_Multi2_SetSystemKey(u32EngId, u8SysKey);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Set Riv of HDCP2, which is 64-bits number generated from transmitter
/// @param u32EngId \b IN: cipher engine index
/// @param pu8RIV \b IN: pointer to 64bit Riv buffer in BIG ENDIAN order
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
///
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV)
{
#if (!( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))    
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    return HAL_DSCMB_HDCP2_SetRiv(u32EngId, pu8RIV);
#else
    return FALSE;
#endif    
}

MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
    return MDrv_DSCMB2_HDCP2_SetRIV(u32EngId, pu8RIV);
}

MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_HDCP2_SETRIV pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_HDCP2_SETRIV));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId      = u32EngId;
    pArgs.pu8Riv        = pu8RIV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_HDCP2_SetRiv, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_HDCP2_SetRIV(u32EngId, pu8RIV);
#endif
}


#if 0
// corrupt
MS_BOOL MDrv_DSCMB_EcmCorruptSet(MS_U8 u8CorruptDataByte, MS_U8 u8FromByte)
{
    return HAL_DSCMB_EcmCorruptSet(u8CorruptDataByte, u8FromByte);
}

MS_BOOL MDrv_DSCMB_EcmCorruptEnable(MS_U32 u32PidIn, MS_BOOL bEnable)
{
    MS_U32 i;
    MS_U32 u32Pid;

    for (i = 0; i< TSP_MAX_FLTNUM; i++)
    {
        if ((DRVTSP_OK == MDrv_TSP_PidFlt_GetPid(0, i, &u32Pid)) && (u32Pid == u32PidIn))
        {
            HAL_DSCMB_EcmCorruptEnable(i, bEnable);
        }
    }
    return FALSE;
}
#endif

//--------------------------------------------------------------------------------------------------
/// Get scrambler level of a pid filter ID
/// @param u32PidFltId \b IN: Pid filter ID obtained from MApi_DMX_Open/MDrv_TSP_PidFlt_Alloc
/// @param pScmbLevel \b OUT: The output scrambler level
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel)
{
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    HAL_Scmb_Level HalScmbLevel = E_HAL_DSCMB_NONE;
    if (FALSE == HAL_DSCMB_PidFlt_ScmbStatus(u32EngId, u32PidFltId, &HalScmbLevel))
    {
        return FALSE;
    }
    *pScmbLevel = E_DSCMB_NONE;
    if (HalScmbLevel & E_HAL_DSCMB_TS)
    {
        *pScmbLevel|= E_DSCMB_TS;
    }
    if (HalScmbLevel & E_HAL_DSCMB_PES)
    {
        *pScmbLevel|= E_DSCMB_PES;
    }
    return TRUE;
}

MS_BOOL MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel)
{
#if DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DSCMBSTATUS pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DSCMBSTATUS));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId      = u32EngId;
    pArgs.u32DmxFltId   = u32PidFltId;
    pArgs.pScmbLevel    = pScmbLevel;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_PidFlt_ScmbStatus(u32EngId, u32PidFltId, pScmbLevel);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Release key ladder key from ACPU
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_ResetAcpuAck(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    MS_BOOL  bRet=FALSE;
    bRet = HAL_DSCMB_KLadder_Acpu_Ack(TRUE);
    bRet |= HAL_DSCMB_KLadder_Acpu_Ack(FALSE);
    return bRet;
#else
    return FALSE;
#endif
}

//--------------------------------------------------------------------------------------------------
/// Read key ladder key from ACPU
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_ReadDataFromAcpu(MS_U8* pu8Out)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Acpu_Data(pu8Out);
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// Reset key ladder
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Reset(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Reset();
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder input
/// @param u32Level \b IN: key ladder level
/// @param pu8In \b IN: input of key ladder at u32Level level
/// @param u32InLen \b IN: input length
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_SetInput(MS_U32 u32Level, MS_U8* pu8In, MS_U32 u32InLen)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_SetInput(u32Level, pu8In, u32InLen);
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder input
/// @param KLSrc \b IN: key ladder key source
/// @param pu8Key \b IN: The target key of key ladder
/// @param u32KeyLen \b IN: the length of the target key of key ladder
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_SetKey(DSCMB_KLSrc KLSrc, MS_U8* pu8Key, MS_U32 u32KeyLen)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    HAL_DscmbKLadderSrc halKLSrc;

    switch (KLSrc)
    {
    case E_DSCMB_KL_SRC_ACPU:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_ACPU;
        break;
    case E_DSCMB_KL_SRC_SECRET_1:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_SECRET_KEY1;
        break;
    case E_DSCMB_KL_SRC_SECRET_2:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_SECRET_KEY2;
        break;
    case E_DSCMB_KL_SRC_SECRET_3:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_SECRET_KEY3;
        break;
    case E_DSCMB_KL_SRC_SECRET_4:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_SECRET_KEY4;
        break;
    case E_DSCMB_KL_SRC_VGK:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_VGK;
        break;
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case E_DSCMB_KL_SRC_PRIVATE_0:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_PRIVATE_KEY0;
        break;
    case E_DSCMB_KL_SRC_PRIVATE_1:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_PRIVATE_KEY1;
        break;
    case E_DSCMB_KL_SRC_PRIVATE_2:
        halKLSrc = E_HAL_DSCMB_KLAD_SRC_PRIVATE_KEY2;
        break;
#endif
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }
    return HAL_DSCMB_KLadder_SetKey(halKLSrc, pu8Key, u32KeyLen);
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder destination
/// @param KLDst \b IN: key ladder key destination
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_SetDst(DSCMB_KLDst KLDst)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    HAL_DscmbKLadderDst halKLDst;

    switch (KLDst)
    {
    case E_DSCMB_KL_DST_KT_ESA:
        halKLDst = E_HAL_DSCMB_KLAD_DST_KTAB_ESA;
        break;
    case E_DSCMB_KL_DST_KT_NSA:
        halKLDst = E_HAL_DSCMB_KLAD_DST_KTAB_NSA;
        break;
    case E_DSCMB_KL_DST_DMA_AES:
        halKLDst = E_HAL_DSCMB_KLAD_DST_AESDMA_AES;
        break;
    case E_DSCMB_KL_DST_DMA_TDES:
        halKLDst = E_HAL_DSCMB_KLAD_DST_AESDMA_TDES;
        break;
    case E_DSCMB_KL_DST_ACPU:
        halKLDst = E_HAL_DSCMB_KLAD_DST_ACPU;
        break;
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case E_DSCMB_KL_DST_PRIVATE0:
        halKLDst = E_HAL_DSCMB_KLAD_DST_PRIVATE0;
        break;
    case E_DSCMB_KL_DST_PRIVATE1:
        halKLDst = E_HAL_DSCMB_KLAD_DST_PRIVATE1;
        break;
    case E_DSCMB_KL_DST_PRIVATE2:
        halKLDst = E_HAL_DSCMB_KLAD_DST_PRIVATE2;
        break;
#endif
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }
    return HAL_DSCMB_KLadder_SetDst(halKLDst);
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder configurations
/// @param pConfig \b IN: key ladder configurations
//--------------------------------------------------------------------------------------------------
void MDrv_DSCMB_KLadder_SetConfig(DSCMB_KLCfg* pConfig)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    KLCfgPrev = *pConfig;

    HAL_DSCMB_KLadder_Decrypt(pConfig->bDecrypt);
    HAL_DSCMB_KLadder_ByteInv(pConfig->bInverse);
    HAL_DSCMB_KLadder_KeepRound(pConfig->bKeepRound);
    HAL_DSCMB_KLadder_Round(pConfig->u32Round);
    HAL_DSCMB_KLadder_EngMode(pConfig->u32EngMode);
#else
    return;
#endif
}

//--------------------------------------------------------------------------------------------------
/// Get previous set configurations
/// @param pConfig \b OUT: key ladder configurations
//--------------------------------------------------------------------------------------------------
void MDrv_DSCMB_KLadder_GetConfig(DSCMB_KLCfg* pConfig)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    *pConfig = KLCfgPrev;
#else
    return;
#endif        
}


//--------------------------------------------------------------------------------------------------
/// Make key ladder start operating
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Start(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Start();
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Check if key ladder operation is complete
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_IsComplete(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_IsComplete();
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder stop operating
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Stop(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Stop();
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder start output
/// @param u32DscmbId \b IN: Descrambler filter ID obtained from MDrv_DSCMB_FltAlloc
/// @param eKeyType \b IN: Descrambler key type
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Output_Start(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[0][u32DscmbId].bUsed);

    stDscmbTspMap* pMap;
    MS_U32 j;
    MS_U32 i = 0;
    MS_U32 u32Slot;
    HAL_DSCMB_Type hal_dscmb_type;
    DSCMB_Key_Type hal_dscmb_key_type;

    pMap = &DscmbTspMap[0][u32DscmbId];
    // check if there exist a tsp filter connction
    // if no connection exist, return FALSE
    for (i = 0; i < MAX_DSCMB_PIDFLT_NUM ; i++) {
        if(pMap->u32PidFltId[i] != U32_MAX){
            break;
        }
    }
    if (i == MAX_DSCMB_PIDFLT_NUM)    return FALSE;

    switch (eKeyType)
    {
    case E_DSCMB_KEY_CLEAR:
        hal_dscmb_key_type = E_DSCMB_KEY_CLEAR;
        u32Slot = pMap->u32SlotIdClear;
        break;
    case E_DSCMB_KEY_ODD:
        hal_dscmb_key_type = E_DSCMB_KEY_ODD;
        u32Slot = pMap->u32SlotIdOdd;
        break;
    case E_DSCMB_KEY_EVEN:
        hal_dscmb_key_type = E_DSCMB_KEY_EVEN;
        u32Slot = pMap->u32SlotIdEven;
        break;
    default:
        return FALSE;
    }

    if (U32_MAX == u32Slot)
    {
        return FALSE;
    }

    switch (pMap->DscmbType)
    {
    case E_DSCMB_TYPE_CSA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA;
        break;
    case E_DSCMB_TYPE_NSA_AS_ESA:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_NSA_AS_ESA;
        break;
    case E_DSCMB_TYPE_DES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_DES;
        break;
    case E_DSCMB_TYPE_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES;
        break;
    case E_DSCMB_TYPE_AES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_ECB;
        break;
    case E_DSCMB_TYPE_AES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_SCTE52;
        break;
    case E_DSCMB_TYPE_TDES_ECB:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_ECB;
        break;
    case E_DSCMB_TYPE_TDES_SCTE52:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_TDES_SCTE52;
        break;
    case E_DSCMB_TYPE_SYN_AES:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC;
        break;
    case E_DSCMB_TYPE_CSA3:
        hal_dscmb_type = E_HAL_DSCMB_TYPE_CSA3;
        break;
    default:
        DSCMB_ASSERT_AND_RETURN(0);
    }


    for (j= 0; j< MAX_DSCMB_PIDFLT_NUM; j++)
    {
        if (U32_MAX!= pMap->u32PidFltId[j])
        {
            HAL_DSCMB_KLadder_Output_Start(pMap->u32PidFltId[j], u32Slot, hal_dscmb_type, hal_dscmb_key_type);
            break;
        }
    }
    return TRUE;
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// Check if key ladder output is complete
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Output_IsComplete(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Output_IsComplete();
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder stop output
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Output_Stop(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_KLadder_Output_Stop();
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver information
/// @return DSCMB_Info
//--------------------------------------------------------------------------------------------------
const DSCMB_Info* MDrv_DSCMB_GetInfo(void)
{
    return (&DscmbInfo);
}

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver library information
/// @param ppVersion \b OUT: Descrambler library version infomation
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_GetLibVer(const MSIF_Version **ppVersion)
{
    return MDrv_DSCMB2_GetLibVer(ppVersion);
}

MS_BOOL MDrv_DSCMB2_GetLibVer(const MSIF_Version **ppVersion)
{
    *ppVersion = &_drv_dscmb_version;
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver status
/// @param pStatus \b OUT: Descrambler status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_GetStatus(DSCMB_Status *pStatus)
{
    if (sizeof(DSCMB_Status))
    {
        memcpy(pStatus, &_DscmbStatus, sizeof(DSCMB_Status));
    }
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Set Dscmb driver debug level
/// @param u32Level \b IN: debug level
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_SetDBGLevel(MS_U32 u32Level)
{
    return MDrv_DSCMB2_SetDBGLevel(u32Level);
}

MS_BOOL MDrv_DSCMB2_SetDBGLevel(MS_U32 u32Level)
{
    _u32DscmbDGBLevel = u32Level;
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Get the status between descramble id and tsp filter id
/// @param u32DscmbId \b IN: descramble id
/// @param u32TspFltId \b IN: tsp filter id
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId)
{
    return MDrv_DSCMB2_GetConnectStatus(u32EngId, u32DscmbId, u32TspFltId);
}

MS_BOOL MDrv_DSCMB2_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId)
{
    DSCMB_ASSERT_AND_RETURN(MAX_NUM > u32DscmbId);
    DSCMB_ASSERT_AND_RETURN(ENG_NUM > u32EngId);
    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    MS_U32 i = 0;
    stDscmbTspMap* pMap =  &DscmbTspMap[u32EngId][u32DscmbId];

    for(i = 0 ; i < MAX_DSCMB_PIDFLT_NUM ; i++){
        if (pMap->u32PidFltId[i] == u32TspFltId) {
            return TRUE;
        }
    }

    return FALSE;
}




// Only for scramble
//--------------------------------------------------------------------------------------------------
/// Set the PID to be replaced
/// @param u32PidPrim\b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID by u32Pid2ndary
/// @param u32Pid2ndary\b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID by u32Pid2ndary
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_2ndarySet(u32PidPrim, u32Pid2ndary);
#else
    return FALSE;
#endif        
}

//--------------------------------------------------------------------------------------------------
/// Reset the PID to be replaced
/// @param u32PidPrim\b IN: Reset the replacement of PID (u32PidPrim) set by MDrv_DSCMB_2ndarySet
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_2ndaryReset(MS_U32 u32PidPrim)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return HAL_DSCMB_2ndaryReset(u32PidPrim);
#else
    return FALSE;
#endif    
}


MS_BOOL MDrv_DSCMB2_SetRecBuf(MS_U32 u32EngId , MS_U32 u32Start, MS_U32 u32Size)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    HAL_DSCMB_SetRecStart(u32Start , u32Start + (u32Size/2) );
    HAL_DSCMB_SetRecEnd(u32Start+(u32Size/2) , u32Start + u32Size );
    HAL_DSCMB_SetRecMid(u32Start , u32Start + (u32Size/2) );

    return TRUE ;
#else
    return FALSE;
#endif    
}

MS_BOOL  MDrv_DSCMB2_RecCtrl(MS_U32 u32EngId, MS_BOOL bEnable)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    HAL_DSCMB_RecCtrl( bEnable);
    return TRUE ;
#else
    return FALSE;
#endif
}

MS_BOOL  MDrv_DSCMB2_GetRecWptr(MS_U32 u32EngId, MS_U32* pu32Write)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    
    DSCMB_ASSERT_AND_RETURN(u32EngId < ENG_NUM);
    HAL_DSCMB_GetRecWptr( pu32Write);
    return TRUE ;
#else
    return FALSE;
#endif
}




//--------------------------------------------------------------------------------------------------
/// Dscmb lock API for supporting multithreading
/// @param u
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------

MS_BOOL MDrv_DSCMB_KLadder_Lock(void  )
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return MsOS_ObtainMutex(_s32DscMutexId, 0xFFFFFFFF);;
#else
    return FALSE;
#endif    
}

MS_BOOL MDrv_DSCMB_KLadder_Unlock(void)
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    return MsOS_ReleaseMutex(_s32DscMutexId);;
#else
    return FALSE;
#endif    
}

//--------------------------------------------------------------------------------------------------
/// An atomic Key ladder function for Key generation, including common KL use cases.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status )
{
#if (( defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))

    MS_U32 i , u32Round= 0 , u32StartTime,u32EndTime ;
    MS_BOOL bKeepRound=FALSE;
    *u32Status = KL_STATUS_KEY_OK ;

    MDrv_DSCMB_KLadder_Lock(  );

    // Check DST and acpu data
    if ((E_DSCMB_KL_SRC_ACPU == KLCfg->eSrc) && (NULL == ACPU_Out))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    // Calculate KL round //
    if ((E_DSCMB_KL_64_BITS == KLCfg->eOutsize) && (E_DSCMB_KL_TDES == KLCfg->eAlgo))
    {
        u32Round = KLCfg->u32Level * 2 - 1;

        //[NOTE] special case for level 0 KL //
        if( KLCfg->u32Level == 0 ) u32Round = 0 ;
    }
    else if ((E_DSCMB_KL_128_BITS == KLCfg->eOutsize) && (E_DSCMB_KL_TDES == KLCfg->eAlgo))
    {
        u32Round = KLCfg->u32Level * 2 ;
    }
    else if ((E_DSCMB_KL_128_BITS == KLCfg->eOutsize) && (E_DSCMB_KL_AES == KLCfg->eAlgo))
    {
        u32Round = KLCfg->u32Level * 2 ;
    }
    else
    {
        *u32Status = KL_STATUS_INVALID_KEYROUND_PARAMETER;
        goto KL_Fail;
    }

    // Check KL level //
    if(KLCfg->u32Level >3)
    {
        //bKeepRound = TRUE ;
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }
    else if(( 0 == KLCfg->u32Level ) && (E_DSCMB_KL_DST_DMA_AES != KLCfg->eDst) && (E_DSCMB_KL_DST_DMA_TDES != KLCfg->eDst))
    { // for AESDMA, it's possible to run KL with level 0 //
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    // check Key destination //
    if ((E_DSCMB_KL_DST_ACPU == KLCfg->eDst) && (NULL == ACPU_Out))
    {
        *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
        goto KL_Fail;
    }
    else if ((E_DSCMB_KL_DST_KT_ESA== KLCfg->eDst) || (E_DSCMB_KL_DST_KT_NSA == KLCfg->eDst))
    {
        if ( FALSE == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].bUsed )
        {
             *u32Status = KL_STATUS_IVALID_DSCMB_ID;
             goto KL_Fail;
        }
        if( 0 == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].u32PidFltIdNum )
        {
             *u32Status = KL_STATUS_IVALID_DSCMB_ID;
             goto KL_Fail;
        }

    }

    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
             *u32Status = KL_STATUS_KL_INPUT_NULL;
             goto KL_Fail;
    }

    //ULOGD("DSCMB", " Round = %d , u32Status = %x \n",u32Round,*u32Status);
    // KL Reset and Reset ACPU Ack //
    HAL_DSCMB_KLadder_Reset();
    MDrv_DSCMB_KLadder_ResetAcpuAck();

    // Set KL config //
    HAL_DSCMB_KLadder_Decrypt(KLCfg->bDecrypt);
    HAL_DSCMB_KLadder_ByteInv(KLCfg->bInverse);
    HAL_DSCMB_KLadder_KeepRound(bKeepRound);
    HAL_DSCMB_KLadder_EngMode(KLCfg->eAlgo);
    HAL_DSCMB_KLadder_Round(u32Round);


    // Set KL Root Key //
    MDrv_DSCMB_KLadder_SetKey(KLCfg->eSrc,KLCfg->u8KeyACPU, 16  );

    // Set KL output Destination //
    MDrv_DSCMB_KLadder_SetDst(KLCfg->eDst);

    // Set KL Level input //
    for( i = 0 ; i < KLCfg->u32Level ; i ++ )
    {
        HAL_DSCMB_KLadder_SetInput(2-i, (MS_U8*)&(KLCfg->pu8KeyKLIn[i*16]), 16);

    #if 0
        ULOGD("DSCMB", "KL input Key[0-7] = %02X %02X %02X %02X %02X %02X %02X %02X\n",
            KLCfg->pu8KeyKLIn[16*i ],KLCfg->pu8KeyKLIn[16*i+1],KLCfg->pu8KeyKLIn[16*i+2],KLCfg->pu8KeyKLIn[i*16+3],
            KLCfg->pu8KeyKLIn[16*i+4],KLCfg->pu8KeyKLIn[i*16+5],KLCfg->pu8KeyKLIn[i*16+6],KLCfg->pu8KeyKLIn[i*16+7]);
        ULOGD("DSCMB", "KL input Key[8-15] = %02X %02X %02X %02X %02X %02X %02X %02X\n",
            KLCfg->pu8KeyKLIn[16*i +8],KLCfg->pu8KeyKLIn[16*i+9],KLCfg->pu8KeyKLIn[16*i+10],KLCfg->pu8KeyKLIn[i*16+11],
            KLCfg->pu8KeyKLIn[16*i+12],KLCfg->pu8KeyKLIn[i*16+13],KLCfg->pu8KeyKLIn[i*16+14],KLCfg->pu8KeyKLIn[i*16+15]);
    #endif
    }

    MDrv_DSCMB_KLadder_ResetAcpuAck();

    // KL start //
    u32StartTime = MsOS_GetSystemTime();
    MDrv_DSCMB_KLadder_Start();

    if (E_DSCMB_KL_DST_ACPU == KLCfg->eDst)
    {// KL flow for ACPU output //

        memset(ACPU_Out , 0x0 , 16 );

        while (!MDrv_DSCMB_KLadder_IsComplete())
        {
            u32EndTime = MsOS_GetSystemTime();
            if ((u32EndTime - u32StartTime) > DSCMB_KL_WAIT_THRESHOLD)
            {
                *u32Status = KL_STATUS_WAIT_CW_READY_TIMEOUT;
                goto KL_Fail;
            }
        };
        MDrv_DSCMB_KLadder_ReadDataFromAcpu(ACPU_Out);
        MDrv_DSCMB_KLadder_ResetAcpuAck();
    }
    else if ((E_DSCMB_KL_DST_DMA_AES == KLCfg->eDst) || (E_DSCMB_KL_DST_DMA_TDES == KLCfg->eDst))
    {// KL flow for output  to CryptoDMA  //
       while(!MDrv_DSCMB_KLadder_Output_IsComplete())
       {
            u32EndTime = MsOS_GetSystemTime();
            if ((u32EndTime - u32StartTime) > DSCMB_KL_WAIT_THRESHOLD)
            {
                *u32Status = KL_STATUS_WAIT_KL_READY_TIMEOUT;
                goto KL_Fail;
            }
       };
       //MDrv_DSCMB_KLadder_Stop();
    }
    else if ((E_DSCMB_KL_DST_KT_ESA== KLCfg->eDst) || (E_DSCMB_KL_DST_KT_NSA == KLCfg->eDst))
    {// KL flow for output  to KTE //
        while (!MDrv_DSCMB_KLadder_IsComplete())
        {
            u32EndTime = MsOS_GetSystemTime();
            if ((u32EndTime - u32StartTime) > DSCMB_KL_WAIT_THRESHOLD)
            {
                *u32Status = KL_STATUS_WAIT_CW_READY_TIMEOUT;
                goto KL_Fail;
            }
        };
        u32StartTime = MsOS_GetSystemTime();

        if(!MDrv_DSCMB_KLadder_Output_Start(KLCfg->u32DscID, KLCfg->eKeyType))
        {
             goto KL_Fail;
        }

        while(!MDrv_DSCMB_KLadder_Output_IsComplete())
        {
            u32EndTime = MsOS_GetSystemTime();
            if ((u32EndTime - u32StartTime) > DSCMB_KL_WAIT_THRESHOLD)
            {
                *u32Status = KL_STATUS_WAIT_KL_READY_TIMEOUT;
                goto KL_Fail;
            }
        };
        MDrv_DSCMB_KLadder_Output_Stop();
        //MDrv_DSCMB_KLadder_Stop();

    }

    if ( HAL_DSCMB_KLadder_Status() != 0 )
    {
        goto KL_Fail;
    }

    MDrv_DSCMB_KLadder_Stop();

    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;

KL_Fail:

    i = HAL_DSCMB_KLadder_Status();
    if ( i != 0 )
    {
        //ULOGD("DSCMB", "KL Satus = %x \n",i );
        *u32Status |= i ;
    }

    MDrv_DSCMB_KLadder_Stop();

    MDrv_DSCMB_KLadder_Unlock();
    //ULOGE("DSCMB", "[%s][%d]2. DSCMB KL Fail = %08lX \n",__FUNCTION__,__LINE__,*u32Status );
    DSCMB_ASSERT_AND_RETURN(FALSE);

#else
    return FALSE;
#endif
}

MS_BOOL MDrv_DSCMB_Suspend(void)
{
    stDscmbTspMap* pMap;
    MS_U32 i;
    MS_U32 u32Cnt;
    MS_U32 u32EngId;
    MS_U32 u32DscmbId;

    if(DscmbTspMap == NULL)
        return FALSE;

    for(u32EngId=0; u32EngId<ENG_NUM; u32EngId++)
    {
        for(u32DscmbId=0; u32DscmbId<MAX_NUM; u32DscmbId++)
        {
            pMap = &DscmbTspMap[u32EngId][u32DscmbId];

            if(pMap->bUsed == FALSE)
                continue;
            
            u32Cnt = pMap->u32PidFltIdNum;

            for (i = 0; i < MAX_DSCMB_PIDFLT_NUM; i++)
            {
                if(u32Cnt == 0) break;
                if (pMap->u32PidFltId[i] != U32_MAX)
                {
                    if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, pMap->u32PidFltId[i]))
                    {
                        DSCMB_ASSERT_AND_RETURN(0);
                        return FALSE;
                    }
                    if (FALSE == HAL_DSCMB_SlotDisconnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, pMap->u32PidFltId[i]))
                    {
                        DSCMB_ASSERT_AND_RETURN(0);
                        return FALSE;
                    }
                    u32Cnt--;
                }
            }
        }
    }    
    return TRUE;
}
MS_BOOL MDrv_DSCMB_Resume(void)
{
    stDscmbTspMap* pMap;
    MS_U32 i;
    MS_U32 u32Cnt;
    MS_U32 u32EngId;
    MS_U32 u32DscmbId;

    if(DscmbTspMap == NULL)
        return FALSE;

    HAL_DSCMB_Init();
    
    for(u32EngId=0; u32EngId<ENG_NUM; u32EngId++)
    {
        for(u32DscmbId=0; u32DscmbId<MAX_NUM; u32DscmbId++)
        {
            pMap = &DscmbTspMap[u32EngId][u32DscmbId];

            if(pMap->bUsed == FALSE)
                continue;
            
            u32Cnt = pMap->u32PidFltIdNum;

            for (i = 0; i < MAX_DSCMB_PIDFLT_NUM; i++)
            {
                if(u32Cnt == 0) break;
                if (pMap->u32PidFltId[i] != U32_MAX)
                {
                    if (FALSE == HAL_DSCMB_SlotConnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_EVEN, pMap->u32SlotIdEven, pMap->u32PidFltId[i]))
                    {
                        DSCMB_ASSERT_AND_RETURN(0);
                        return FALSE;
                    }
                    if (FALSE == HAL_DSCMB_SlotConnectPidFltId(u32EngId, pMap->u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd, pMap->u32PidFltId[i]))
                    {
                        DSCMB_ASSERT_AND_RETURN(0);
                        return FALSE;
                    }
                    u32Cnt--;
                }
            }
        }
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// STD general function
/// @param u16PowerState              \b IN: DSCMB power state
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_DSCMB_SetPowerState(EN_POWER_MODE u16PowerState)
{
    return MDrv_DSCMB2_SetPowerState(u16PowerState);
}

MS_U32 MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState)
{
#ifdef DSCMB_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    EN_POWER_MODE pArgs = u16PowerState;
    if(pu32DscmbInst == NULL)
    {
        #ifdef MSOS_TYPE_LINUX_KERNEL
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB | KERNEL_MODE, (void**)&pu32DscmbInst, 0x0, NULL))
        #else
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, (void**)&pu32DscmbInst, 0x0, NULL))
        #endif
        {
            ULOGE("DSCMB", "UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }
    
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_SetPowerState, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("DSCMB", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_SetPowerState(u16PowerState);
#endif
}

MS_U32 _MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U16 _ret = FALSE;

    switch (u16PowerState)
    {
        case E_POWER_SUSPEND:
            _ret = MDrv_DSCMB_Suspend();
            break;

        case E_POWER_RESUME:
            _ret = MDrv_DSCMB_Resume();
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            ULOGE("DSCMB", "[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__);
            break;
    }

    return ((TRUE == _ret)? UTOPIA_STATUS_SUCCESS: UTOPIA_STATUS_FAIL);
}

MS_BOOL MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetAlgo(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Algo_Cfg stConfig)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngResetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetIV(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetSwitch(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eUppSwitch, DSCMB_Eng_Type eLowSwitch)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_FSCB eForceSCB )
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 * pu8Response, DSCMB_KL_Status* u32Status )
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status )
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_FltSwitchType(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Flt_Type eFltType)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetKeyFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, DSCMB_FSCB eForceSCB)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngSetRIV(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngEnableKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_BOOL bEnable)
{
    return FALSE;
}
