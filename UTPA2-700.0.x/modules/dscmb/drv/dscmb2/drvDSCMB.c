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
#include <linux/limits.h>
#else
#include <limits.h>
#include <string.h>
#endif

#include "drvSYS.h"
#include "drvMMIO.h"

#include "drvDSCMB.h"
#include "regDSCMB.h"
#include "halDSCMB.h"
#include "asmCPU.h"
#include "utopia.h"

#ifdef DSCMB_UTOPIA_20
#define  DSCMB_UTOPIA20
#else
#undef DSCMB_UTOPIA20
#endif

#ifdef DSCMB_UTOPIA20
#include "drvDSCMB_v2.h"
#endif

#include "drvDSCMB_private.h"

// Threshold for KL calculation //
#define DSCMB_KL_WAIT_THRESHOLD 300

#define FLEXIBLE_PIDSLOTMAP
//=========================================================================================
//  Data Structure Declaration
//=========================================================================================
#define DSCMB_LOCK(_a_)      MsOS_ObtainMutex ( (_a_) , MSOS_WAIT_FOREVER );    HAL_DSCMB_KTE_HW_SEM_Lock(DSCMB_KL_WAIT_THRESHOLD);
#define DSCMB_UNLOCK(_a_)    HAL_DSCMB_KTE_HW_SEM_Unlock(); MsOS_ReleaseMutex( (_a_) )

static MS_U32 _u32DSCMB2DbgLv = DSCMB2_DBGLV_WARNING;
static MS_BOOL _bDSCMB2Drv_Inited = FALSE;
static MS_BOOL _bKLDrv_Inited = FALSE;

#ifdef DSCMB_NO_PRINTF
#define DRVDSCMB2_DBG(lv, x, args...)
#else
#define DRVDSCMB2_DBG(lv, x, args...) \
    if (lv <= _u32DSCMB2DbgLv ) \
    {   \
        if(lv == DSCMB2_DBGLV_ERR){ printf("[ERROR]"); } \
        else if(lv == DSCMB2_DBGLV_WARNING){ printf("[WARNING]"); }\
        else{ printf("[DEBUG]"); }\
        if(_u32DSCMB2DbgLv >= DSCMB2_DBGLV_DEBUG){ printf("[%05d][%-35s]", __LINE__, __FUNCTION__); }\
        printf(" ");\
        printf(x, ##args); \
    }
#endif

#define DSCMB_ASSERT(x, p, ret)  if (!(x)){DRVDSCMB2_DBG(DSCMB2_DBGLV_ALERT, "ALERT FAIL: %s\n", p); return ret;}
#define DSCMB_INIT_CHECK(ret)    DSCMB_ASSERT(_bDSCMB2Drv_Inited == TRUE, "The drvDSCMB2 is not initialized\n", ret)
#define KL_INIT_CHECK(ret)    DSCMB_ASSERT(_bKLDrv_Inited == TRUE, "The Key Ladder is not initialized\n", ret)
#define OBSOLETE_FUNCTION(x, args...)    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "[%05d][%-35s]"x, __LINE__, __FUNCTION__, ##args)

static MSIF_Version _drv_dscmb_version =
{
    .DDI = { DSCMB_DRV_VERSION, },
};




#ifdef DSCMB_UTOPIA20
    extern stTspFltInfo*  _u32PidFlt2Dscmb;
    extern MS_BOOL*       _bFreeSlot;
    extern stDscmbTspMap (*DscmbTspMap)[HAL_DSCMB_KTE_MAX];
    extern MS_BOOL*       _bFreeRIVSlot;
#else
    static stTspFltInfo  _u32PidFlt2Dscmb[HAL_DSCMB_PIDFLT_NUM];
    static stDscmbTspMap DscmbTspMap[HAL_DSCMB_ENG_NUM][HAL_DSCMB_KTE_MAX];
    static MS_BOOL       _bFreeSlot[HAL_DSCMB_KTE_MAX];
    static MS_BOOL       _bFreeRIVSlot[HAL_DSCMB_RIV_MAX];
#endif

// TODO: Fix me, need to put to resource private?
static MS_S32 _s32DscMutexId = -1 ;
static MS_S32 _s32KLMutexId = -1 ;

// TODO: Does it nessarary?
MS_U8  _u8KLPreLevel = 0;

static MS_BOOL  _bUsedCAFlt[DSCMB_CAPVR_PIDTABLE_NUM][HAL_DSCMB_SPSPVR_FLT_NUM];


#ifdef DSCMB_UTOPIA20
static void *pu32DscmbInst = NULL;
#endif

#define SLOTUSED 1
#define SLOTEMPTY 0

//--------------------------------------------------------------------------------------------------
MS_U32 _DSCMB_SlotEmptyCount(void)
{
    MS_U32 i = 0 ;
    MS_U32 count = 0 ;

    for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i++)
    {
        if (_bFreeSlot[i] == SLOTEMPTY )
        {
            count++;
        }
    }
    return count;
}

MS_BOOL _DSCMB_SlotAlloc(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32* pu32SlotId)
{
    MS_U32 i = 0 ;
    *pu32SlotId = 0 ;

    for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i++)
    {
        if (_bFreeSlot[i] == SLOTEMPTY )
        {
            _bFreeSlot[i] = SLOTUSED ;
            *pu32SlotId = i ;
            DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "DSCMB Filter 0x%x Key 0x%x has index 0x%x\n", (MS_U16)u32DscmbId,
            (MS_U16)KeyType, (MS_U16)i);
            return TRUE;
        }
    }
    return FALSE ;
}


MS_BOOL _DSCMB_SlotFree(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId)
{
    if(u32SlotId < HAL_DSCMB_KTE_MAX)
    {
        _bFreeSlot[u32SlotId] = SLOTEMPTY ;
        return TRUE ;
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Free an empty keyslot out of range\n");

    return FALSE;
}

MS_BOOL _DSCMB_SlotRIVAlloc(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32* pu32SlotId)
{
    MS_U32 i = 0 ;
    *pu32SlotId = 0 ;

    for ( i = 0 ; i < HAL_DSCMB_RIV_MAX ; i++)
    {
        if (_bFreeRIVSlot[i] == SLOTEMPTY )
        {
            _bFreeRIVSlot[i] = SLOTUSED ;
            *pu32SlotId = i ;
            DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "DSCMB 0x%x has RIV index 0x%x\n", (MS_U16)u32DscmbId,(MS_U16)i);
            return TRUE;
        }
    }
    return FALSE ;
}


MS_BOOL _DSCMB_SlotRIVFree(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32SlotId)
{
    if(u32SlotId < HAL_DSCMB_RIV_MAX)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Free u32SlotId 0x%x\n", u32SlotId);
        _bFreeRIVSlot[u32SlotId] = SLOTEMPTY ;
        return TRUE ;
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Free an empty keyslot out of range\n");

    return FALSE;
}

// Find a filter which connect to target dscmb ID
MS_BOOL _DSCMB_Dscmb2Flt(MS_U32 u32DscmbId , MS_U32* pFltId)
{
#ifdef FLEXIBLE_PIDSLOTMAP
    *pFltId = u32DscmbId;
    return TRUE;
#else
    *pFltId = 0 ;
    MS_U32 i ;

    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if ( _u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId )
        {
            break ;
        }
    }

    if ( i == HAL_DSCMB_PIDFLT_NUM )
    {
        return FALSE ;
    }

    *pFltId = i ;

    return TRUE ;
#endif /* FLEXIBLE_PIDSLOTMAP */
}

void _DumpMapInfo(MS_U32 idx)
{
    stDscmbTspMap* pMap = NULL;

    pMap = &DscmbTspMap[0][idx];
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Index:     %u\n", idx);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Used:      %d\n", pMap->bUsed);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Even  Idx: %u\n", pMap->u32SlotIdEven);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Odd   Idx: %u\n", pMap->u32SlotIdOdd);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Clear Idx: %u\n", pMap->u32SlotIdClear);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "RIV   Idx: %u\n", pMap->u32RIVIdx);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "PidFlt #:  %u\n", pMap->u32PidFltIdNum);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Dscmb:     %d\n", pMap->bDscmb);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, " %s\n", pMap->eFltType  == E_DSCMB_FLT_2_KEYS ?"2 Keys":"3 Keys");
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "CAVid:     %u\n", pMap->u32CAVid);
}

//Enable all PIDSlotMap in the Dscmb filter
MS_BOOL _DSCMB_EnableDSCMB(MS_U32 u32DscmbId)
{
    MS_U32 i;

    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if ( _u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId )
        {
            if(FALSE == HAL_DSCMB_PidIdx_EnableSlot(i))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "FAIL\n");
                return FALSE;
            }
            else
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Enable PIDSlotMap %d !!!!!!!!!!!\n", i);
            }
        }
    }

    return TRUE;
}

//Disable all PIDSlotMap in the Dscmb filter
MS_BOOL _DSCMB_DisableDSCMB(MS_U32 u32DscmbId)
{
    MS_U32 i;

    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if ( _u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId )
        {
            if(FALSE == HAL_DSCMB_PidIdx_DisableSlot(i))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "FAIL\n");
                return FALSE;
            }
            else
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Disable PIDSlotMap %d !!!!!!!!!!!\n", i);
            }

        }
    }

    return TRUE;
}

MS_BOOL _DSCMB_IsFltConnectDSCMB(MS_U32 u32DscmbId, MS_U32 u32FltId)
{
    if(u32DscmbId >= HAL_DSCMB_KTE_MAX || u32FltId >= HAL_DSCMB_PIDFLT_NUM)
    {
        return FALSE;
    }

    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId == u32DscmbId)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32  _MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U16 _ret = FALSE;

    switch (u16PowerState)
    {
        case E_POWER_SUSPEND:
            _ret = _MDrv_DSCMB2_Exit();
            break;

        case E_POWER_RESUME:
            _ret = _MDrv_DSCMB2_Init();
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__);
            break;
    }

    return _ret;
}

MS_BOOL _MDrv_DSCMB2_KLadder_Init(void)
{
    MS_VIRT u32Bank;
    MS_PHY u32BankSize;

    if(_bKLDrv_Inited == TRUE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_NOTICE, "Key Ladder driver already init\n");
        return TRUE;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Get MMIO base fail\n");
        return FALSE;
    }

    HAL_DSCMB_SetBank(u32Bank);

    _s32KLMutexId  = MsOS_CreateMutex(E_MSOS_FIFO,"KeyLadder_Mutex", MSOS_PROCESS_SHARED);
    _bKLDrv_Inited = TRUE;
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Initialize dscrambler driver and descrambler engine by init flag
/// @param bSWInit\b IN: If TRUE, do SW init only. If FALSE do HW&SW init.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// It should be called before calling any other dscrambler DDI functions.
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_InitBySWFlag(MS_BOOL bSWInit)
{
    MS_VIRT u32Bank;
    MS_PHY u32BankSize;
    MS_U32 i , j;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG,"u32CAVid = %d ++++\n", u32CAVid);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO,"ChipRev=%d\n", MDrv_SYS_GetChipRev());
    if(_bDSCMB2Drv_Inited == TRUE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_NOTICE, "DSCMB driver already init\n");
        return TRUE;
    }

    if(u32CAVid == 0)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Invalid CAVid\n");
        return FALSE;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Get MMIO base fail\n");
        return FALSE;
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "Bank = 0x%X\n", (unsigned int)u32Bank);
    HAL_DSCMB_SetBank(u32Bank);

    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "CAVid = 0x%X\n", (unsigned int)u32CAVid);
    HAL_DSCMB_SetCAVid(u32CAVid);

    _s32DscMutexId = MsOS_CreateMutex(E_MSOS_FIFO,"DSCMB_Mutex", MSOS_PROCESS_SHARED);
    if( FALSE == _MDrv_DSCMB2_KLadder_Init() )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Init Keyladder Fail\n");
        return FALSE;
    }
    if(TRUE == bSWInit)
    {
        goto _MDrv_DSCMB2_InitBySWFlag_RET;
    }

    HAL_DSCMB_Init();
    memset(_bFreeSlot, 0x0, sizeof(MS_BOOL)*HAL_DSCMB_KTE_MAX);
    memset(DscmbTspMap , 0x0, sizeof(stDscmbTspMap)*HAL_DSCMB_ENG_NUM*HAL_DSCMB_KTE_MAX);
    memset(_bFreeRIVSlot, 0x0, sizeof(MS_BOOL)*HAL_DSCMB_RIV_MAX);
    memset(_bUsedCAFlt, 0x0, sizeof(_bUsedCAFlt));
    memset(_u32PidFlt2Dscmb, 0x0, sizeof(stTspFltInfo)*HAL_DSCMB_PIDFLT_NUM);

    for( j = 0 ; j < HAL_DSCMB_ENG_NUM ; j ++)
    {
        DscmbTspMap[j][0].bUsed = TRUE;//0 will not be used forever

        for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i ++)
        {
            DscmbTspMap[j][i].u32SlotIdOdd   = DRV_DSCMB_FLT_NULL;
            DscmbTspMap[j][i].u32SlotIdEven  = DRV_DSCMB_FLT_NULL;
            DscmbTspMap[j][i].u32SlotIdClear = DRV_DSCMB_FLT_NULL;
            DscmbTspMap[j][i].u32CAVid       = u32CAVid;
            DscmbTspMap[j][i].u32RIVIdx      = HAL_DSCMB_RIV_MAX;
            DscmbTspMap[j][i].u32Tsid        = HAL_DSCMB_TSID_MAX;
        }
    }

    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i++)
    {
        _u32PidFlt2Dscmb[i].u8DscmbId= HAL_DSCMB_KTE_MAX ;
        _u32PidFlt2Dscmb[i].u8CaVid = u32CAVid;
        _u32PidFlt2Dscmb[i].u8TsId = HAL_DSCMB_TSID_MAX;
    }

    //Only if DSCMB is enable by OTP, we connect the path to DSCMB
    if( HAL_DSCMB_OTPEnDSCMB() == TRUE)
    {
        //go through DSCMB
        for (i = 0 ; i < REG_DSCMB_PATH_CNT ; i++)
        {
            // set recbuf source to DSCMB
            HAL_DSCMB_ConnectPath(i,TRUE);
        }
        #ifndef DSCMB_NO_PRINTF
        printf("\033[31mGo through DSCMB\n\033[m");
        #endif
    }
    else
    {
        #ifndef DSCMB_NO_PRINTF
        printf("\033[33m ByPass DSCMB\n\033[m");
        #endif
    }

_MDrv_DSCMB2_InitBySWFlag_RET:

    _bDSCMB2Drv_Inited = TRUE;

    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG,"[%s][%d] ++++\n", __FUNCTION__, __LINE__);

    return TRUE;
}


//--------------------------------------------------------------------------------------------------
/// Initialize dscrambler driver and descrambler engine
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// It should be called before calling any other dscrambler DDI functions.
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_Init(void)
{
    return _MDrv_DSCMB2_InitBySWFlag(FALSE);
}


//--------------------------------------------------------------------------------------------------
/// Exit dscrambler driver and descrambler engine
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// It should be called after finish any other dscrambler DDI functions.
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_Exit(void)
{
    if (_s32DscMutexId != -1)
    {
        MsOS_DeleteMutex(_s32DscMutexId);
        _s32DscMutexId = -1;
    }

    if (_s32KLMutexId != -1)
    {
        MsOS_DeleteMutex(_s32KLMutexId);
        _s32KLMutexId = -1;
    }

    _bDSCMB2Drv_Inited = FALSE;
    _bKLDrv_Inited = FALSE;
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter
/// @param eFltType \b IN: type for descramber filter
/// @return Descrambler filter ID - Success
/// @return DRV_DSCMB_FLT_NULL - Failure
//--------------------------------------------------------------------------------------------------
MS_U32 _MDrv_DSCMB2_FltAllocWithCAVid(MS_U32 u32EngId, DSCMB_Flt_Type eFltType, MS_U32 u32CAVid)
{
    DSCMB_INIT_CHECK(DRV_DSCMB_FLT_NULL);
    MS_BOOL bSecureSlot = (( E_DSCMB_FLT_SECURE_KEYS_ENABLE & eFltType) != 0) ? TRUE : FALSE;
    eFltType = eFltType & (~(E_DSCMB_FLT_SECURE_KEYS_ENABLE));
//#define  E_DSCMB_FLT_PRIV_KEYS_ENABLE   (1 << 6)
//    MS_BOOL bPrivilegeSlot = (( E_DSCMB_FLT_PRIV_KEYS_ENABLE & eFltType) != 0) ? TRUE : FALSE;
//    eFltType = eFltType & (~(E_DSCMB_FLT_SECURE_KEYS_ENABLE | E_DSCMB_FLT_PRIV_KEYS_ENABLE));
    DSCMB_ASSERT(u32EngId == 0, "EngId input error", DRV_DSCMB_FLT_NULL);
    DSCMB_ASSERT((eFltType == E_DSCMB_FLT_3_KEYS) || (eFltType == E_DSCMB_FLT_2_KEYS) || (eFltType == E_DSCMB_FLT_1_KEYS), "FltType input error", DRV_DSCMB_FLT_NULL);
    DSCMB_ASSERT((u32CAVid != 0) && (u32CAVid <= DEFAULT_CAVID), "CAVid input error", DRV_DSCMB_FLT_NULL);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Allocate with %d CAVid=%u\n", eFltType, u32CAVid);

    MS_U32 i = 0;
    stDscmbTspMap* pMap = NULL;

    DSCMB_LOCK(_s32DscMutexId);

    // Find free dscmb slots for a DSCMB flt //
    for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i ++)
    {
        pMap = &DscmbTspMap[u32EngId][i];
        if ( pMap->bUsed == FALSE )
        {
            pMap->bUsed = TRUE ;
            // For 1 Key for a Dscmb filter //
            if(eFltType == E_DSCMB_FLT_1_KEYS)
            {
                MS_U32 u32IgoreModeKeyIdx = 0;
                if (FALSE == _DSCMB_SlotAlloc( u32EngId , i , E_DSCMB_KEY_CLEAR , &u32IgoreModeKeyIdx))
                {
                    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find SlotMap FAIL\n");
                    goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
                }

                pMap->u32SlotIdClear = u32IgoreModeKeyIdx;
                pMap->u32SlotIdEven  = u32IgoreModeKeyIdx;
                pMap->u32SlotIdOdd   = u32IgoreModeKeyIdx;

                break;
            }
            // For 3 Keys for a Dscmb filter //
            if ( E_DSCMB_FLT_3_KEYS == eFltType)
            {
                // Clear key is init in BypassMode
                if (FALSE == _DSCMB_SlotAlloc( u32EngId , i , E_DSCMB_KEY_CLEAR , &(pMap->u32SlotIdClear)))
                {
                    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find SlotMap FAIL\n");
                    goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
                }
            }

            // 2 Keys for a Dscmb filter //
            if (FALSE == _DSCMB_SlotAlloc( u32EngId , i , E_DSCMB_KEY_EVEN , &(pMap->u32SlotIdEven)))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find SlotMap FAIL\n");
                goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
            }
            if (FALSE == _DSCMB_SlotAlloc( u32EngId , i , E_DSCMB_KEY_ODD , &(pMap->u32SlotIdOdd)))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find SlotMap FAIL\n");
                goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
            }

            break; // successfully find a Free DSCMB filter
        }
    }

    // Check No free slot
    if( i == HAL_DSCMB_KTE_MAX )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find SlotMap FAIL\n");
        goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
    }

    // Set default value to Map object
    pMap->u32PidFltIdNum = 0;
    pMap->bDscmb = TRUE; //Descramble  or Scramble
    pMap->eFltType = eFltType;
    pMap->u32CAVid = u32CAVid;
    pMap->u32RIVIdx = HAL_DSCMB_RIV_MAX;
    pMap->u32Tsid = HAL_DSCMB_TSID_MAX;
    pMap->bSecure = bSecureSlot;

    _DumpMapInfo(i);

#ifdef FLEXIBLE_PIDSLOTMAP
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_EVEN);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_ODD);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_CLEAR);

    HAL_DSCMB_PidIdx_SetCAVid(i, u32CAVid);
    HAL_DSCMB_PidIdx_SetSecure(i, pMap->bSecure);
    //HAL_DSCMB_PidIdx_SetPrivilege(i, bPrivilegeSlot);
    if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( i , E_DSCMB_KEY_EVEN  , pMap->u32SlotIdEven) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
        goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
    }

    if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( i , E_DSCMB_KEY_ODD   , pMap->u32SlotIdOdd) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
        goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
    }

    if(pMap->eFltType == E_DSCMB_FLT_3_KEYS || (pMap->eFltType == E_DSCMB_FLT_1_KEYS))
    {
        if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( i , E_DSCMB_KEY_CLEAR , pMap->u32SlotIdClear) )
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
            goto MDrv_DSCMB2_FltAlloc_Ex_FAIL;
        }
    }
#endif /* FLEXIBLE_PIDSLOTMAP */

    DSCMB_UNLOCK(_s32DscMutexId);

    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Allocate DSCMB Filter ID: %u\n", i);

    return i;

MDrv_DSCMB2_FltAlloc_Ex_FAIL:

#ifdef FLEXIBLE_PIDSLOTMAP
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_EVEN);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_ODD);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_CLEAR);

    HAL_DSCMB_PidIdx_SetSecure(i, FALSE);
#endif /* FLEXIBLE_PIDSLOTMAP */

    if(i != HAL_DSCMB_KTE_MAX)
    {
        // Free key Slots //
        if (pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
        {
            _DSCMB_SlotFree(u32EngId, i , E_DSCMB_KEY_CLEAR  ,pMap->u32SlotIdClear);
            pMap->u32SlotIdClear = DRV_DSCMB_FLT_NULL ;
        }
        if (pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
        {
            _DSCMB_SlotFree(u32EngId, i , E_DSCMB_KEY_ODD  ,pMap->u32SlotIdOdd);
            pMap->u32SlotIdOdd = DRV_DSCMB_FLT_NULL ;
        }
        if (pMap->u32SlotIdEven != DRV_DSCMB_FLT_NULL)
        {
            _DSCMB_SlotFree(u32EngId, i , E_DSCMB_KEY_EVEN  ,pMap->u32SlotIdEven);
            pMap->u32SlotIdEven = DRV_DSCMB_FLT_NULL ;
        }

        pMap->bUsed = FALSE;
        pMap->bSecure = FALSE;
    }
    DSCMB_UNLOCK(_s32DscMutexId);
    return DRV_DSCMB_FLT_NULL;
}

//--------------------------------------------------------------------------------------------------
/// Allocate a dscrambler filter
/// @return Descrambler filter ID - Success
/// @return DRV_DSCMB_FLT_NULL - Failure
//--------------------------------------------------------------------------------------------------
MS_U32 _MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType)
{
    return _MDrv_DSCMB2_FltAllocWithCAVid(u32EngId, eFltType, HAL_DSCMB_GetCAVid());
}

MS_U32 _MDrv_DSCMB2_FltAlloc(MS_U32 u32EngId)
{
    return _MDrv_DSCMB2_FltAlloc_Ex(u32EngId, E_DSCMB_FLT_2_KEYS);
}

//--------------------------------------------------------------------------------------------------
/// Free the specific DSCMB filter by ID
/// @return TRUE - SUCCESS
/// @return FALSE - Failure
/// @note
///
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Free with u32DscmbId=%u\n", u32DscmbId);

    MS_U32 i = 0;
    stDscmbTspMap* pMap = NULL;
    stDscmbTspMap* pMapTmp = NULL;
    MS_BOOL bDisablePES = TRUE;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    if(pMap->bUsed == FALSE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Free an unused DSCMB filter\n");
    }

    //Check if two or more u32DSCMB use PES decrypt in the same tsid
    // If there exist a dscmb id that use PES decrypt, do not disable PES
    for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i ++)
    {
        if(i == u32DscmbId)
        {
            continue;
        }
        pMapTmp = &DscmbTspMap[u32EngId][i];
        if ( (pMapTmp->bUsed == TRUE) && (pMapTmp->u32Tsid == pMap->u32Tsid))
        {
            if(pMapTmp->u32RIVIdx != HAL_DSCMB_RIV_MAX)
            {
                bDisablePES = FALSE;
            }
        }
    }

    if(pMap->u32RIVIdx != HAL_DSCMB_RIV_MAX)
    {
        if(bDisablePES == TRUE)
        {
            HAL_DSCMB_PES_Enable (0, pMap->u32Tsid, FALSE);
        }
    }

#ifdef FLEXIBLE_PIDSLOTMAP
    // Reset the Key index
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_CLEAR);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_EVEN);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_ODD);
    HAL_DSCMB_PidIdx_SetSecure(u32DscmbId, FALSE);
    HAL_DSCMB_KTE_Clear_KeyFSCB(u32DscmbId);
#endif /* FLEXIBLE_PIDSLOTMAP */

    // Disable Pid Slot
    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if (_u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId)
        {
#ifndef FLEXIBLE_PIDSLOTMAP
            // Reset the Key index
            HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_CLEAR);
            HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_EVEN);
            HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i, E_DSCMB_KEY_ODD);
            HAL_DSCMB_PidIdx_SetSecure(i, FALSE);
            HAL_DSCMB_KTE_Clear_KeyFSCB(i);
#endif /* FLEXIBLE_PIDSLOTMAP */

            HAL_DSCMB_PidIdx_DisableSlot(i);
            pMap->u32PidFltIdNum--;
            _u32PidFlt2Dscmb[i].u8DscmbId= HAL_DSCMB_KTE_MAX ;
            _u32PidFlt2Dscmb[i].u8TsId = HAL_DSCMB_TSID_MAX;
            _u32PidFlt2Dscmb[i].u8CaVid = DEFAULT_CAVID;

        }
    }

    // Free key Slots //
    if (pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_CLEAR  ,pMap->u32SlotIdClear);
        pMap->u32SlotIdClear = DRV_DSCMB_FLT_NULL ;
    }
    if (pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_ODD  ,pMap->u32SlotIdOdd);
        pMap->u32SlotIdOdd = DRV_DSCMB_FLT_NULL ;
    }
    if (pMap->u32SlotIdEven != DRV_DSCMB_FLT_NULL)
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_EVEN  ,pMap->u32SlotIdEven);
        pMap->u32SlotIdEven = DRV_DSCMB_FLT_NULL ;
    }
    if(pMap->u32RIVIdx != HAL_DSCMB_RIV_MAX)
    {
        _DSCMB_SlotRIVFree (u32EngId, u32DscmbId, pMap->u32RIVIdx);
        pMap->u32RIVIdx = HAL_DSCMB_RIV_MAX;
    }
    pMap->u32CAVid = DEFAULT_CAVID ;
    pMap->u32Tsid = HAL_DSCMB_TSID_MAX;
    pMap->bUsed = FALSE ;
    pMap->bSecure = FALSE;

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;
}


MS_BOOL _MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT((u32FltId < HAL_DSCMB_PIDIDX_MAX) && (u32FltId != 0), "FltId input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Connect u32FltId=%u to u32DscmbId=%u\n", u32FltId, u32DscmbId);

    stDscmbTspMap* pMap = NULL;
    MS_U32 u32Tsid = 0;
    MS_U32 i = 0;
    MS_U32 u32CAVid = DEFAULT_CAVID;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if(pMap->bUsed == FALSE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "DSCMB filter does not allocated yet\n");
        goto MDrv_DSCMB2_FltConnectFltId_FAIL;
    }

    for(i = 0 ; i < HAL_DSCMB_KTE_MAX; i++)
    {
        if(_DSCMB_IsFltConnectDSCMB(i, u32FltId))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter already connect to another dscmb_id\n");
            goto MDrv_DSCMB2_FltConnectFltId_FAIL;
        }
    }

    HAL_DSCMB_Get_TsidInput (u32FltId, &u32Tsid);

    if(pMap->u32PidFltIdNum == 0)
    {
        pMap->u32Tsid = u32Tsid;
    }
    else
    {
        if(u32Tsid != pMap->u32Tsid)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter=%d tsid=%d not match tsid=%d\n",
                u32FltId, u32Tsid, pMap->u32Tsid);
            goto MDrv_DSCMB2_FltConnectFltId_FAIL;

        }
    }

#ifdef FLEXIBLE_PIDSLOTMAP
    HAL_DSCMB_SetTSPCADst_PidSlotMap(u32FltId, _u32PidFlt2Dscmb[u32FltId].u8CaDestUpp, _u32PidFlt2Dscmb[u32FltId].u8CaDestLow, u32DscmbId);
#else
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_EVEN);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_ODD);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_CLEAR);

    HAL_DSCMB_PidIdx_SetCAVid(u32FltId, u32CAVid);

    if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( u32FltId , E_DSCMB_KEY_EVEN  , pMap->u32SlotIdEven) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
        goto MDrv_DSCMB2_FltConnectFltId_FAIL;
    }

    if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( u32FltId , E_DSCMB_KEY_ODD   , pMap->u32SlotIdOdd) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
        goto MDrv_DSCMB2_FltConnectFltId_FAIL;
    }

    if(pMap->eFltType == E_DSCMB_FLT_3_KEYS || (pMap->eFltType == E_DSCMB_FLT_1_KEYS))
    {
        if ( FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx( u32FltId , E_DSCMB_KEY_CLEAR , pMap->u32SlotIdClear) )
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set SlotMap Fail\n");
            goto MDrv_DSCMB2_FltConnectFltId_FAIL;
        }
    }

    HAL_DSCMB_PidIdx_SetSecure(u32FltId, pMap->bSecure);
#endif /* FLEXIBLE_PIDSLOTMAP */

    HAL_DSCMB_PidIdx_EnableSlot(u32FltId);

    pMap->u32PidFltIdNum++;
    _u32PidFlt2Dscmb[u32FltId].u8DscmbId = u32DscmbId;
    _u32PidFlt2Dscmb[u32FltId].u8TsId    = u32Tsid;

#ifdef ENABLE_KEY_FSCB
  #ifdef FLEXIBLE_PIDSLOTMAP
    // do nothing
  #else
    // Copy KeyFSCBs from existed KeySlot
    for( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i++ )
    {
        if( i != u32FltId && _u32PidFlt2Dscmb[i].u8DscmbId==u32DscmbId )
        {
            HAL_DSCMB_KTE_Copy_KeyFSCB( i, u32FltId);
            break;
        }
    }
  #endif
#endif

    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Connect TSP flt %u to DSCMB flt %u\n", u32FltId, u32DscmbId);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "DSCMB ID: %u has %u TSP filter connected\n", u32DscmbId, pMap->u32PidFltIdNum);

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE ;

MDrv_DSCMB2_FltConnectFltId_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT((u32FltId < HAL_DSCMB_PIDIDX_MAX) && (u32FltId != 0), "FltId input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Disconnect u32FltId=%u from u32DscmbId=%u\n", u32FltId, u32DscmbId);

    stDscmbTspMap* pMap = NULL;

    // [FIXME] Discoonect Filter , Disable PidIdx reference.
    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    // Check DSCMB filter data
    if((pMap->bUsed == FALSE) || (pMap->u32PidFltIdNum <= 0))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter content error\n");
        goto MDrv_DSCMB2_FltDisconnectFltId_FAIL;
    }

    // Check if the TSP filter is mapped to the DSCMB filter
    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != u32DscmbId)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter mapping error\n");
        goto MDrv_DSCMB2_FltDisconnectFltId_FAIL;
    }

#ifdef FLEXIBLE_PIDSLOTMAP
    HAL_DSCMB_SetTSPCADst_PidSlotMap(u32FltId, _u32PidFlt2Dscmb[u32FltId].u8CaDestUpp, _u32PidFlt2Dscmb[u32FltId].u8CaDestLow, 0);
#else
    // Reset the Key index
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_CLEAR);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_EVEN);
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32FltId, E_DSCMB_KEY_ODD);
    HAL_DSCMB_PidIdx_SetSecure(u32FltId, FALSE);
#endif /* FLEXIBLE_PIDSLOTMAP*/

    // disable Keytable SlotMap
    HAL_DSCMB_PidIdx_DisableSlot(u32FltId);

    // Update the internal data structure
    pMap->u32PidFltIdNum--;
    _u32PidFlt2Dscmb[u32FltId].u8DscmbId= HAL_DSCMB_KTE_MAX;
    _u32PidFlt2Dscmb[u32FltId].u8TsId = 0xff;
    _u32PidFlt2Dscmb[u32FltId].u8CaVid = 0xff;

#ifdef ENABLE_KEY_FSCB
  #ifdef FLEXIBLE_PIDSLOTMAP
    // do nothing
  #else
    // Clear all KeyFSCBs of given u32FltId
    HAL_DSCMB_KTE_Clear_KeyFSCB(u32FltId);
  #endif
#endif

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE ;

MDrv_DSCMB2_FltDisconnectFltId_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;
}


MS_BOOL _MDrv_DSCMB2_FltSwitchType(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Flt_Type eFltType)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);

    stDscmbTspMap* pMap = NULL;
#ifndef FLEXIBLE_PIDSLOTMAP
    MS_U32 i = 0;
#endif /* FLEXIBLE_PIDSLOTMAP */
    MS_U32 u32OldSlotIdOdd = DRV_DSCMB_FLT_NULL;
    MS_U32 u32OldSlotIdClear = DRV_DSCMB_FLT_NULL;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    if(pMap->bUsed == FALSE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter does not allocated yet\n");
        goto MDrv_DSCMB2_FltSwitchType_FAIL;
    }

    if((eFltType == E_DSCMB_FLT_2_KEYS_SHARE) || (pMap->eFltType == E_DSCMB_FLT_2_KEYS_SHARE))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter does not support the type switch\n");
        goto MDrv_DSCMB2_FltSwitchType_FAIL;
    }

    if(eFltType == pMap->eFltType)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "DSCMB filter has the same type\n");
        goto MDrv_DSCMB2_FltSwitchType_OK;
    }

    //make sure keyslot is enough, then SW operation never fail
    if(_DSCMB_SlotEmptyCount()<2)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "KeySlot not enough\n");
        goto MDrv_DSCMB2_FltSwitchType_FAIL;
    }

    ////////////////////////////////////////////////////////////////
    ////  SW operation (6 cases)
    ///////////////////////////////////////////////////////////////
    u32OldSlotIdOdd = pMap->u32SlotIdOdd;
    u32OldSlotIdClear = pMap->u32SlotIdClear;

    //case +1 slot odd key
    if((eFltType==E_DSCMB_FLT_2_KEYS) && (pMap->eFltType==E_DSCMB_FLT_1_KEYS))
    {
        _DSCMB_SlotAlloc( u32EngId , u32DscmbId , E_DSCMB_KEY_ODD , &(pMap->u32SlotIdOdd));
        pMap->u32SlotIdClear = DRV_DSCMB_FLT_NULL;

    }
    //case +1 slot clear key
    else if((eFltType==E_DSCMB_FLT_3_KEYS) && (pMap->eFltType==E_DSCMB_FLT_2_KEYS))
    {
        _DSCMB_SlotAlloc( u32EngId , u32DscmbId , E_DSCMB_KEY_CLEAR , &(pMap->u32SlotIdClear));
    }
    //case +2 slot odd & clear key
    else if((eFltType==E_DSCMB_FLT_3_KEYS) && (pMap->eFltType==E_DSCMB_FLT_1_KEYS))
    {
        _DSCMB_SlotAlloc( u32EngId , u32DscmbId , E_DSCMB_KEY_ODD , &(pMap->u32SlotIdOdd));
        _DSCMB_SlotAlloc( u32EngId , u32DscmbId , E_DSCMB_KEY_CLEAR , &(pMap->u32SlotIdClear));
    }
    //case -1 slot clear key
    else if((eFltType==E_DSCMB_FLT_2_KEYS) && (pMap->eFltType==E_DSCMB_FLT_3_KEYS))
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_CLEAR  ,pMap->u32SlotIdClear);
        pMap->u32SlotIdClear= DRV_DSCMB_FLT_NULL;
    }
    //case -1 slot (enable odd & clear & even index all the same)
    else if((eFltType==E_DSCMB_FLT_1_KEYS) && (pMap->eFltType==E_DSCMB_FLT_2_KEYS))
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_ODD  ,pMap->u32SlotIdOdd);
        pMap->u32SlotIdOdd = pMap->u32SlotIdEven;
        pMap->u32SlotIdClear = pMap->u32SlotIdEven;

    }
    //case -2 slot (enable odd & clear & even index all the same)
    else if((eFltType==E_DSCMB_FLT_1_KEYS) && (pMap->eFltType==E_DSCMB_FLT_3_KEYS))
    {
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_ODD  ,pMap->u32SlotIdOdd);
        _DSCMB_SlotFree(u32EngId, u32DscmbId , E_DSCMB_KEY_CLEAR  ,pMap->u32SlotIdClear);
        pMap->u32SlotIdOdd = pMap->u32SlotIdEven;
        pMap->u32SlotIdClear = pMap->u32SlotIdEven;
    }

    // if no one connected, we don't need to update HW
    if(pMap->u32PidFltIdNum == 0)
    {
        goto MDrv_DSCMB2_FltSwitchType_OK;
    }

    ///////////////////////////////////////////////////////////////
    ////    HW operation (6 cases)
    /////////////////////////////////////////////////////////////
#ifdef FLEXIBLE_PIDSLOTMAP
    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_ODD);
    if(pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
    {
        if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd))
        {
            goto MDrv_DSCMB2_FltSwitchType_HWFAIL;
        }
    }

    HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_CLEAR);
    if(pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
    {
        if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear))
        {
            goto MDrv_DSCMB2_FltSwitchType_HWFAIL;
        }
    }
#else
    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if (_u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId)
        {
            HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i,E_DSCMB_KEY_ODD);
            if(pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
            {
                if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(i,E_DSCMB_KEY_ODD,pMap->u32SlotIdOdd))
                {
                    goto MDrv_DSCMB2_FltSwitchType_HWFAIL;
                }
            }

            HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i,E_DSCMB_KEY_CLEAR);
            if(pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
            {
                if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(i,E_DSCMB_KEY_CLEAR,pMap->u32SlotIdClear))
                {
                    goto MDrv_DSCMB2_FltSwitchType_HWFAIL;
                }
            }
        }
    }
#endif /* FLEXIBLE_PIDSLOTMAP */

MDrv_DSCMB2_FltSwitchType_OK:
    pMap->eFltType = eFltType;
    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_FltSwitchType_HWFAIL:
    // revert the original setting
    pMap->u32SlotIdOdd = u32OldSlotIdOdd;
    pMap->u32SlotIdClear = u32OldSlotIdClear;
#ifdef FLEXIBLE_PIDSLOTMAP
    if(pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
    {
        HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_ODD);
        if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_ODD, pMap->u32SlotIdOdd))
        {
            goto MDrv_DSCMB2_FltSwitchType_FAIL;
        }
    }
    if(pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
    {
        HAL_DSCMB_PidIdx_ClearSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_CLEAR);
        if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(u32DscmbId, E_DSCMB_KEY_CLEAR, pMap->u32SlotIdClear))
        {
            goto MDrv_DSCMB2_FltSwitchType_FAIL;
        }
    }
#else
    for ( i = 0 ; i < HAL_DSCMB_PIDFLT_NUM ; i ++)
    {
        if (_u32PidFlt2Dscmb[i].u8DscmbId== u32DscmbId)
        {
            if(pMap->u32SlotIdOdd != DRV_DSCMB_FLT_NULL)
            {
                HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i,E_DSCMB_KEY_ODD);
                if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(i,E_DSCMB_KEY_ODD,pMap->u32SlotIdOdd))
                {
                    goto MDrv_DSCMB2_FltSwitchType_FAIL;
                }
            }
            if(pMap->u32SlotIdClear != DRV_DSCMB_FLT_NULL)
            {
                HAL_DSCMB_PidIdx_ClearSlotKeyIdx(i,E_DSCMB_KEY_CLEAR);
                if(FALSE == HAL_DSCMB_PidIdx_SetSlotKeyIdx(i,E_DSCMB_KEY_CLEAR,pMap->u32SlotIdClear))
                {
                    goto MDrv_DSCMB2_FltSwitchType_FAIL;
                }
            }
        }
    }
#endif /* FLEXIBLE_PIDSLOTMAP */
MDrv_DSCMB2_FltSwitchType_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;

}


MS_BOOL _MDrv_DSCMB2_EngEnableKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_BOOL bEnable)
{
    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_FltEnableKey_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if (!HAL_DSCMB_KTE_Key_Ctrl (u32FltId, eKeyType , eEngType | eSecureKeyType, u32CAVid, bEnable ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Enable Key Fail\n");
        goto MDrv_DSCMB2_FltEnableKey_FAIL;
    }
    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_FltEnableKey_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}


MS_BOOL _MDrv_DSCMB2_EngSetFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_FSCB eForceSCB )
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eForceSCB <= DSCMB_FSCB_ODD, "ForceSCB input error", FALSE);

    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set DscmbId=%u FSCB=%d\n", u32DscmbId, eForceSCB);

    stDscmbTspMap* pMap = NULL;
    MS_U32 u32FltId = 0 ;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    MS_BOOL bRet = TRUE;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetFSCB_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    bRet &= HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType),   u32CAVid, eForceSCB);
    bRet &= HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType),  u32CAVid, eForceSCB);

    if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
    {
        bRet &= HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), u32CAVid, eForceSCB);
    }

    if (!bRet)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set FSCB Fail\n");
        goto MDrv_DSCMB2_EngSetFSCB_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetFSCB_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

// Set Upper & lower switch to dscmb filter //
//--------------------------------------------------------------------------------------------------
/// Set upper switch and lower switch to choose which engine will go into.
/// @param u32DscmbId\b IN: The descrambler id which is get from MDrv_DSCMB2_FltAlloc.
/// @param eUppSwitch\b IN: An Enum type for choosing descrambler engine when go through upper path.
/// @param eLowSwitch\b IN: An Enum type for choosing descrambler engine when go through lower path.
/// @return TRUE - SUCCESS
/// @return FALSE - Failure
/// @note
///
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_EngSetSwitch(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eUppSwitch ,DSCMB_Eng_Type eLowSwitch)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eUppSwitch <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "UppSwitch input error", FALSE);
    DSCMB_ASSERT(eLowSwitch <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "LowSwitch input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set DscmbId=%u UppSwitch=%d LowSwitch=%d\n", u32DscmbId, eUppSwitch, eLowSwitch);

    stDscmbTspMap* pMap = NULL;
    MS_U32 u32FltId = 0 ;
    MS_BOOL bRet = TRUE;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetSwitch_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    bRet &= HAL_DSCMB_KTE_Write_PacketSwitch(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType),   u32CAVid, eUppSwitch, eLowSwitch);
    bRet &= HAL_DSCMB_KTE_Write_PacketSwitch(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType),    u32CAVid, eUppSwitch, eLowSwitch);

    if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
    {
        bRet  &= HAL_DSCMB_KTE_Write_PacketSwitch(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType),  u32CAVid, eUppSwitch, eLowSwitch);
    }

    if(!bRet)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Switch Fail\n");
        goto MDrv_DSCMB2_EngSetSwitch_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetSwitch_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_EngSetAlgo(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Algo_Cfg stConfig)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eEngType <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "EngType input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set Algo DscmbId=%u eEngType=%d\n", u32DscmbId, eEngType);

    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    MS_U32 u32Variant = 0x0;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;
    DSCMB_LOCK(_s32DscMutexId);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetAlgo_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if((eEngType & E_DSCMB_ENG_LSAS ) == E_DSCMB_ENG_LSAS)
    {
        if ((!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType), E_DSCMB_ENG_LSAS, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))||
            (!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType) , E_DSCMB_ENG_LSAS, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt )))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS Engine Algorithm Fail\n");
            goto MDrv_DSCMB2_EngSetAlgo_FAIL;
        }

        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            if(!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), E_DSCMB_ENG_LSAS, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS Engine Algorithm Fail\n");
                goto MDrv_DSCMB2_EngSetAlgo_FAIL;
            }
        }
    }

    if((eEngType & E_DSCMB_ENG_ESA ) == E_DSCMB_ENG_ESA)
    {
        if ((!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType), E_DSCMB_ENG_ESA, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))||
            (!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType) , E_DSCMB_ENG_ESA, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt )))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set ESA Engine Algorithm Fail\n");
            goto MDrv_DSCMB2_EngSetAlgo_FAIL;
        }

        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            if(!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), E_DSCMB_ENG_ESA, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS Engine Algorithm Fail\n");
                goto MDrv_DSCMB2_EngSetAlgo_FAIL;
            }
        }
    }

    if((eEngType & E_DSCMB_ENG_LSAD ) == E_DSCMB_ENG_LSAD)
    {
        if ((!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType), E_DSCMB_ENG_LSAD, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))||
            (!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType) , E_DSCMB_ENG_LSAD, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt )))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAD Engine Algorithm Fail\n");
            goto MDrv_DSCMB2_EngSetAlgo_FAIL;
        }

        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            if(!HAL_DSCMB_KTE_Write_Algo(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), E_DSCMB_ENG_LSAD, u32CAVid, stConfig.eMainAlgo,
                                       stConfig.eSubAlgo, stConfig.eResAlgo, stConfig.eSBAlgo, stConfig.bDecrypt ))
            {
                DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS Engine Algorithm Fail\n");
                goto MDrv_DSCMB2_EngSetAlgo_FAIL;
            }
        }
    }

    if((eEngType & E_DSCMB_ENG_ESA ) == E_DSCMB_ENG_ESA)
    {
        if((stConfig.eMainAlgo == E_DSCMB_MAIN_ALGO_CSA2) ||(stConfig.eMainAlgo == E_DSCMB_MAIN_ALGO_CSA2_CONF))
        {
            u32Variant = 0x1;
        }

        HAL_DSCMB_KTE_Write_SBOX( u32FltId , (E_DSCMB_KEY_ODD|eSecureKeyType)  , u32CAVid, u32Variant, FALSE );
        HAL_DSCMB_KTE_Write_SBOX( u32FltId , (E_DSCMB_KEY_EVEN|eSecureKeyType) , u32CAVid, u32Variant, FALSE );
        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            HAL_DSCMB_KTE_Write_SBOX( u32FltId , (E_DSCMB_KEY_CLEAR|eSecureKeyType), u32CAVid, u32Variant, FALSE );
        }
    }

    if(stConfig.bDecrypt == TRUE)
    {
        HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType),    u32CAVid, DSCMB_FSCB_CLEAR);
        HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType),   u32CAVid, DSCMB_FSCB_CLEAR);
        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType),  u32CAVid, DSCMB_FSCB_CLEAR);
        }
    }
    else
    {
        HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_ODD|eSecureKeyType),    u32CAVid, DSCMB_FSCB_UNCHG);
        HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_EVEN|eSecureKeyType),   u32CAVid, DSCMB_FSCB_UNCHG);
        if(pMap->eFltType == E_DSCMB_FLT_3_KEYS)
        {
            HAL_DSCMB_KTE_Write_FSCB(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType),  u32CAVid, DSCMB_FSCB_UNCHG);
        }
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE ;

MDrv_DSCMB2_EngSetAlgo_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;
}

MS_BOOL _MDrv_DSCMB2_EngSetIV(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eKeyType <= E_DSCMB_KEY_ODD, "KeyType input error", FALSE);
    DSCMB_ASSERT(pu8IV != NULL, "Key data input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set IV DscmbId=%u eKeyType=%d\n", u32DscmbId, eKeyType);

    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetIV_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if((pMap->eFltType == E_DSCMB_FLT_2_KEYS) && (eKeyType == E_DSCMB_KEY_CLEAR))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "User Try to use 3 Keys.\n");
        goto MDrv_DSCMB2_EngSetIV_FAIL;
    }

    if (!HAL_DSCMB_KTE_Write_IV(u32FltId, eKeyType | eSecureKeyType , u32CAVid, pu8IV))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set IV Fail\n");
        goto MDrv_DSCMB2_EngSetIV_FAIL;
    }

    if (!HAL_DSCMB_KTE_IV_Ctrl (u32FltId, eKeyType | eSecureKeyType , u32CAVid, TRUE ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set IV Fail\n");
        goto MDrv_DSCMB2_EngSetIV_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetIV_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;
}


MS_BOOL _MDrv_DSCMB2_EngSetIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eEngType <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "EngType input error", FALSE);
    DSCMB_ASSERT(eKeyType <= E_DSCMB_KEY_ODD, "KeyType input error", FALSE);
    DSCMB_ASSERT(pu8IV != NULL, "Key data input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set IV DscmbId=%u eKeyType=%d\n", u32DscmbId, eKeyType);

    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if((pMap->eFltType == E_DSCMB_FLT_2_KEYS) && (eKeyType == E_DSCMB_KEY_CLEAR))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "User Try to use 3 Keys.\n");
        goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
    }

    //Deal with LSAS engine
    if((eEngType & E_DSCMB_ENG_LSAS) == E_DSCMB_ENG_LSAS )
    {
        if (!HAL_DSCMB_KTE_Write_IV_Ex(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAS, u32CAVid, pu8IV))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }

        if (!HAL_DSCMB_KTE_IV_Ctrl_Ex (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAS, u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAS IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }
    }

    //Deal with ESA engine
    if((eEngType & E_DSCMB_ENG_ESA) == E_DSCMB_ENG_ESA )
    {
        if (!HAL_DSCMB_KTE_Write_IV_Ex(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_ESA, u32CAVid, pu8IV))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set ESA IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }

        if (!HAL_DSCMB_KTE_IV_Ctrl_Ex (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_ESA, u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set ESA IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }
    }

    //Deal with LSAD engine
    if((eEngType & E_DSCMB_ENG_LSAD) == E_DSCMB_ENG_LSAD )
    {
        if (!HAL_DSCMB_KTE_Write_IV_Ex(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAD, u32CAVid, pu8IV))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAD IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }

        if (!HAL_DSCMB_KTE_IV_Ctrl_Ex (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAD, u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set LSAD IV Fail\n");
            goto MDrv_DSCMB2_EngSetIV_Ex_FAIL;
        }
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetIV_Ex_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;
}

MS_BOOL _MDrv_DSCMB2_EngSetRIV(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(pu8RIV != NULL, "Key data input error", FALSE);

    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;
    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;
    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetRIV_FAIL;
    }

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if(pMap->eFltType == E_DSCMB_FLT_2_KEYS)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "User Try to use 3 Keys.\n");
        goto MDrv_DSCMB2_EngSetRIV_FAIL;
    }

    if(pMap->u32RIVIdx == HAL_DSCMB_RIV_MAX)
    {
        if(_DSCMB_SlotRIVAlloc (u32EngId, u32DscmbId, &pMap->u32RIVIdx) == FALSE)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Find RIV Slot FAIL\n");
            goto MDrv_DSCMB2_EngSetRIV_FAIL;
        }
    }

    if (!HAL_DSCMB_KTE_Write_RIV(u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), u32CAVid, pu8RIV, pMap->u32RIVIdx))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set RIV Fail\n");
        goto MDrv_DSCMB2_EngSetRIV_FAIL;
    }

    if (!HAL_DSCMB_KTE_IV_Ctrl_Ex (u32FltId, (E_DSCMB_KEY_CLEAR|eSecureKeyType), E_DSCMB_ENG_ESA, u32CAVid, TRUE ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set RIV Fail\n");
        goto MDrv_DSCMB2_EngSetRIV_FAIL;
    }

    // TODO: need to fix channel number
    HAL_DSCMB_PES_Enable (0, pMap->u32Tsid, TRUE);

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetRIV_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;

}

MS_BOOL _MDrv_DSCMB2_EngSetKeyFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, DSCMB_FSCB eForceSCB)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eEngType <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "EngType input error", FALSE);
    DSCMB_ASSERT(eKeyType <= E_DSCMB_KEY_ODD, "KeyType input error", FALSE);

    MS_BOOL bRet      = TRUE;

#ifdef ENABLE_KEY_FSCB
    MS_U32  u32KteSel = REG_KTE_SEL_LSAD;
    MS_U32  u32i      = 0;
    MS_U32 u32FltId = 0;

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        return FALSE;
    }

    switch(eEngType)
    {
        case E_DSCMB_ENG_LSAS:
            u32KteSel = E_HAL_DSCMB_ENG_LSAS;
            break;
        case E_DSCMB_ENG_ESA:
            u32KteSel = E_HAL_DSCMB_ENG_ESA;
            break;
        case  E_DSCMB_ENG_LSAD:
            u32KteSel = E_HAL_DSCMB_ENG_LSAD;
            break;
        default:
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "EngType input error\n");
            return FALSE;
    }
  #ifdef FLEXIBLE_PIDSLOTMAP
    bRet = HAL_DSCMB_KTE_Write_KeyFSCB( u32FltId, u32KteSel, eKeyType, eForceSCB);
    u32i = u32i;
  #else
    for(u32i = 0 ; u32i < HAL_DSCMB_PIDFLT_NUM ; u32i++ )
    {
        if ( _u32PidFlt2Dscmb[u32i].u8DscmbId==u32DscmbId )
        {
            bRet |= HAL_DSCMB_KTE_Write_KeyFSCB( u32i, u32KteSel, eKeyType, eForceSCB);
        }

        if( TRUE!= bRet )
        {
            break;
        }
    }
  #endif
#else
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING,"Not support.\n");
    bRet = FALSE;
#endif

    return bRet;
}

MS_BOOL _MDrv_DSCMB2_EngSetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eEngType <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "EngType input error", FALSE);
    DSCMB_ASSERT(eKeyType <= E_DSCMB_KEY_ODD, "KeyType input error", FALSE);
    DSCMB_ASSERT(pu8Key != NULL, "Key data input error", FALSE);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set Key DscmbId=%u eEngType=%d, eKeyType=%d\n", u32DscmbId, eEngType, eKeyType);

    MS_U32 u32FltId = 0;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngSetKey_FAIL;
    }

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if((pMap->eFltType == E_DSCMB_FLT_2_KEYS) && (eKeyType == E_DSCMB_KEY_CLEAR))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "User Try to use 3 Keys.\n");
        goto MDrv_DSCMB2_EngSetKey_FAIL;
    }

    //Deal with LSAS engine
    if((eEngType & E_DSCMB_ENG_LSAS) == E_DSCMB_ENG_LSAS )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType , E_DSCMB_ENG_LSAS , u32CAVid, pu8Key))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with LSAS Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }

        if (!HAL_DSCMB_KTE_Key_Ctrl (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAS , u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with LSAS Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }
    }
    //Deal with ESA engine
    if((eEngType & E_DSCMB_ENG_ESA) == E_DSCMB_ENG_ESA )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_ESA , u32CAVid, pu8Key))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with ESA Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }

        if (!HAL_DSCMB_KTE_Key_Ctrl (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_ESA , u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with ESA Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }
    }
    //Deal with LSAD engine
    if((eEngType & E_DSCMB_ENG_LSAD) == E_DSCMB_ENG_LSAD )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAD , u32CAVid, pu8Key))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with LSAD Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }

        if (!HAL_DSCMB_KTE_Key_Ctrl (u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAD , u32CAVid, TRUE ))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Key Fail with LSAD Engine.\n");
            goto MDrv_DSCMB2_EngSetKey_FAIL;
        }
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetKey_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE ;
}

MS_BOOL _MDrv_DSCMB2_EngResetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(eEngType <= (E_DSCMB_ENG_LSAS|E_DSCMB_ENG_ESA|E_DSCMB_ENG_LSAD), "EngType input error", FALSE);
    DSCMB_ASSERT(eKeyType <= E_DSCMB_KEY_ODD, "KeyType input error", FALSE);

    MS_U8 ClrKey[16] = {0};
    MS_U32 u32FltId = 0 ;
    stDscmbTspMap* pMap = NULL;
    MS_U32 u32CAVid = DEFAULT_CAVID;
    DSCMB_Key_Type eSecureKeyType = E_DSCMB_KEY_CLEAR;

    DSCMB_LOCK(_s32DscMutexId);

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_EngResetKey_FAIL;
    }

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    u32CAVid = pMap->u32CAVid;

    if(TRUE == pMap->bSecure)
    {
        eSecureKeyType = E_DSCMB_KEY_SECURE_KEYS_ENABLE;
    }

    if((pMap->eFltType == E_DSCMB_FLT_2_KEYS) && (eKeyType == E_DSCMB_KEY_CLEAR))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "User Try to use 3 Keys.\n");
        goto MDrv_DSCMB2_EngResetKey_FAIL;
    }

    //Deal with LSAS engine
    if((eEngType & E_DSCMB_ENG_LSAS) == E_DSCMB_ENG_LSAS )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAS, u32CAVid, ClrKey))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Reset Key Fail with LSAS Engine.\n");
            goto MDrv_DSCMB2_EngResetKey_FAIL;
        }
    }
    //Deal with ESA engine
    if((eEngType & E_DSCMB_ENG_ESA) == E_DSCMB_ENG_ESA )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_ESA, u32CAVid, ClrKey))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Reset Key Fail with ESA Engine.\n");
            goto MDrv_DSCMB2_EngResetKey_FAIL;
        }
    }
    //Deal with LSAD engine
    if((eEngType & E_DSCMB_ENG_LSAD) == E_DSCMB_ENG_LSAD )
    {
        if (!HAL_DSCMB_KTE_Write_Key(u32FltId, eKeyType | eSecureKeyType, E_DSCMB_ENG_LSAD, u32CAVid, ClrKey))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Reset Key Fail with LSAD Engine.\n");
            goto MDrv_DSCMB2_EngResetKey_FAIL;
        }
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE ;
MDrv_DSCMB2_EngResetKey_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_EngSetCAVid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32CAVid)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 i = 0;
    stDscmbTspMap* pMap = NULL;

    DSCMB_LOCK(_s32DscMutexId);
    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    for(i = 0 ; i < HAL_DSCMB_PIDFLT_NUM; i++)
    {
        if(_DSCMB_IsFltConnectDSCMB(u32DscmbId, i))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB filter connect more than one filter\n");
            goto MDrv_DSCMB2_EngSetCAVid_FAIL;
        }
    }

    pMap->u32CAVid = u32CAVid;

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_EngSetCAVid_FAIL:

    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT((u32CAVid != 0) && (u32CAVid <= DEFAULT_CAVID), "CAVid input error", FALSE);

    MS_U32 i = 0;
    stDscmbTspMap* pMap = NULL;

    DSCMB_LOCK(_s32DscMutexId);
#ifdef FLEXIBLE_PIDSLOTMAP
    for ( i = 1 ; i < HAL_DSCMB_KTE_MAX ; i ++)
#else
    for ( i = 0 ; i < HAL_DSCMB_KTE_MAX ; i ++)
#endif
    {
        pMap = &DscmbTspMap[u32EngId][i];
        if ( (pMap->bUsed == TRUE) && (pMap->u32CAVid != u32CAVid))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Please release dscmb id %u\n", i);
            goto MDrv_DSCMB2_SetDefaultCAVid_FAIL;
        }
    }

    //set default cavid
    HAL_DSCMB_SetCAVid (u32CAVid);

    //set tsp cavid and packet out cavid
    for(i = 0 ; i < HAL_DSCMB_TSID_MAX; i++)
    {
        HAL_DSCMB_SetTSPCAVid(i, u32CAVid);
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;
MDrv_DSCMB2_SetDefaultCAVid_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_U32  _MDrv_DSCMB2_GetPidSlotMapIndex(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
    DSCMB_INIT_CHECK(DRV_DSCMB_FLT_NULL);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);

    MS_U32 u32FltId = 0 ;

    DSCMB_LOCK(_s32DscMutexId);

    if (!_DSCMB_Dscmb2Flt( u32DscmbId , &u32FltId ) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No filter connect to DSCMB\n");
        goto MDrv_DSCMB2_GetPidSlotMapIndex_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return u32FltId;

MDrv_DSCMB2_GetPidSlotMapIndex_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return DRV_DSCMB_FLT_NULL;

}

MS_BOOL _MDrv_DSCMB2_IsKTEValid(MS_U32 u32FltId, DSCMB_Key_Type eKeyType)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 KeyStatus = 0;

    DSCMB_LOCK(_s32DscMutexId);


    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
#ifdef FLEXIBLE_PIDSLOTMAP
        HAL_DSCMB_KTE_GetStatus(_u32PidFlt2Dscmb[u32FltId].u8DscmbId, eKeyType, &KeyStatus);
#else
        HAL_DSCMB_KTE_GetStatus(u32FltId, eKeyType, &KeyStatus);
#endif /* FLEXIBLE_PIDSLOTMAP */
    }

    DSCMB_UNLOCK(_s32DscMutexId);

    return (KeyStatus & DSCMB_KTE_VALID)?TRUE:FALSE;
}


MS_BOOL _MDrv_DSCMB2_SetTSPCAVid(DSCMB_TSIF eSrcTSIf, MS_U32 u32CAVid)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32Tsid = HAL_DSCMB_FltSrc2PktDmx(eSrcTSIf);

    HAL_DSCMB_SetTSPCAVid(u32Tsid, u32CAVid);

    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_SetTSPCADst(MS_U32 u32FltId, MS_U32 u32UpDst, MS_U32 u32LowDst)
{
    DSCMB_INIT_CHECK(FALSE);
    MS_U32 u32PidSlotMapNo=0;
    DSCMB_LOCK(_s32DscMutexId);

#ifdef FLEXIBLE_PIDSLOTMAP
    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
        u32PidSlotMapNo =_u32PidFlt2Dscmb[u32FltId].u8DscmbId;
    }
#else
    u32PidSlotMapNo = u32FltId;
#endif

    _u32PidFlt2Dscmb[u32FltId].u8CaDestUpp = u32UpDst;
    _u32PidFlt2Dscmb[u32FltId].u8CaDestLow = u32LowDst;
    HAL_DSCMB_SetTSPCADst_PidSlotMap(u32FltId, u32UpDst, u32LowDst, u32PidSlotMapNo);

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_DualPath_Enable(MS_U32 u32EngId, MS_U32 u32FltId)
{
    DSCMB_INIT_CHECK(FALSE);
    MS_BOOL bRet = FALSE;

    DSCMB_LOCK(_s32DscMutexId);
    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
#ifdef FLEXIBLE_PIDSLOTMAP
        bRet = HAL_DSCMB_PidIdx_DualPath(_u32PidFlt2Dscmb[u32FltId].u8DscmbId, TRUE);   //enable small switch pid_no dual path
#else
        bRet = HAL_DSCMB_PidIdx_DualPath(u32FltId, TRUE);
#endif /* FLEXIBLE_PIDSLOTMAP */
    }
    else
    {
        bRet = FALSE;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return bRet;
}

MS_BOOL _MDrv_DSCMB2_DualPath_Disable(MS_U32 u32EngId, MS_U32 u32FltId)
{
    DSCMB_INIT_CHECK(FALSE);
    MS_BOOL bRet = FALSE;

    DSCMB_LOCK(_s32DscMutexId);

    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
#ifdef FLEXIBLE_PIDSLOTMAP
        bRet = HAL_DSCMB_PidIdx_DualPath(_u32PidFlt2Dscmb[u32FltId].u8DscmbId, FALSE);   //disable small switch pid_no dual path
#else
        bRet = HAL_DSCMB_PidIdx_DualPath(u32FltId, FALSE);
#endif /* FLEXIBLE_PIDSLOTMAP */
    }
    else
    {
        bRet = FALSE;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return bRet;
}

MS_BOOL _MDrv_DSCMB2_SPD_Enable(DSCMB_TSIF ePvrSrcTSIf)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32Tsif = HAL_DSCMB_FltSrc2TSIF(ePvrSrcTSIf);

    if( u32Tsif >= E_HAL_DSCMB_TSIF_NUM)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING,"Not support.\n");
        return FALSE;
    }

    HAL_DSCMB_SPD_Enable(u32Tsif);
    HAL_DSCMB_SPD_Reset(u32Tsif);

    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_SPD_Disable(DSCMB_TSIF ePvrSrcTSIf)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32Tsif = HAL_DSCMB_FltSrc2TSIF(ePvrSrcTSIf);

    if( u32Tsif >= E_HAL_DSCMB_TSIF_NUM)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING,"Not support.\n");
        return FALSE;
    }

    HAL_DSCMB_SPD_Disable(u32Tsif);
    HAL_DSCMB_SPD_Reset(u32Tsif);

    return TRUE;
}

MS_BOOL MDrv_DSCMB2_SPS_Enable (DSCMB_CAPVR_MODE eCaMode)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32ProgId  = HAL_DSCMB_GetProgId(eCaMode);

    return HAL_DSCMB_Cipher_SPS_Enable(u32ProgId, TRUE);
}

MS_BOOL MDrv_DSCMB2_SPS_Disable (DSCMB_CAPVR_MODE eCaMode)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32ProgId  = HAL_DSCMB_GetProgId(eCaMode);

    return HAL_DSCMB_Cipher_SPS_Enable(u32ProgId, FALSE);
}

/*//////////////////////////////////////////////////////////////////////

  Arg: ePvrSrcTSIf,  need to identify with TSP DMX_TSIF when use this function

//////////////////////////////////////////////////////////////////////*/
MS_BOOL _MDrv_DSCMB2_CAPVR_FlowSet(MS_U32 u32EngId, DSCMB_CAPVR_MODE eCaMode, DSCMB_TSIF ePvrSrcTsif)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 u32Tsid    = HAL_DSCMB_FltSrc2PktDmx(ePvrSrcTsif); //genereate from ePvrSrcTSIF
    MS_U32 u32CaVid   = HAL_DSCMB_GetCAVid();
    MS_U32 u32Channel = HAL_DSCMB_GetChannlId(ePvrSrcTsif);
    MS_U32 u32ProgId  = HAL_DSCMB_GetProgId(eCaMode);

    HAL_DSCMB_Cipher_Output_Ctrl(u32ProgId, u32Channel, TRUE, FALSE, u32Tsid, u32CaVid);
    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_SetPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_LOCK(_s32DscMutexId);

    if(!HAL_DSCMB_PidFlt_SetPidPair(u32FltIdPri, u32FltIdSec) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Set Pid Pair Fail\n");
        goto MDrv_DSCMB2_SetPidPair_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_SetPidPair_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_ClearPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_LOCK(_s32DscMutexId);

    if(!HAL_DSCMB_PidFlt_ClearPidPair(u32FltIdPri, u32FltIdSec) )
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Clear Pid Pair Fail\n");
        goto MDrv_DSCMB2_Clear_FAIL;
    }

    DSCMB_UNLOCK(_s32DscMutexId);
    return TRUE;

MDrv_DSCMB2_Clear_FAIL:
    DSCMB_UNLOCK(_s32DscMutexId);
    return FALSE;
}


//----------------------------------------------------------------------//
//                   Old Interface for compatibility                        //
//----------------------------------------------------------------------//
MS_BOOL _MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type DscmbType)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_ASSERT(u32EngId == 0, "EngId input error", FALSE);
    DSCMB_ASSERT(u32DscmbId < HAL_DSCMB_KTE_MAX, "DscmbId input error", FALSE);
    DSCMB_ASSERT(DscmbType <= E_DSCMB_TYPE_CSA_CONF, "Dscmb Type input error", FALSE);

    stDscmbTspMap* pMap = NULL;

    DSCMB_MainAlgo_Type  eAlgo    = E_DSCMB_MAIN_ALGO_AES;
    DSCMB_SubAlgo_Type   eSubAlgo = E_DSCMB_SUB_ALGO_NONE;
    DSCMB_ResSBAlgo_Type eRes     = E_DSCMB_RESSB_ALGO_NONE;
    DSCMB_ResSBAlgo_Type eSB      = E_DSCMB_RESSB_ALGO_NONE;

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];

    switch (DscmbType)
    {
        case E_DSCMB_TYPE_CSA:
            eAlgo    = E_DSCMB_MAIN_ALGO_CSA2;
            break;
        case E_DSCMB_TYPE_CSA_CONF:
            eAlgo    = E_DSCMB_MAIN_ALGO_CSA2_CONF;
            break;
        case E_DSCMB_TYPE_NSA_AS_ESA:
            //[FIXME] Ask Designer to update this setting.
            eAlgo    = E_DSCMB_MAIN_ALGO_CSA2 ;
            break;
        case E_DSCMB_TYPE_DES:
            eAlgo    = E_DSCMB_MAIN_ALGO_DES ;
            break;
        case E_DSCMB_TYPE_AES: // AES - CBC
            eAlgo    = E_DSCMB_MAIN_ALGO_AES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_CBC ;
            break;
        case E_DSCMB_TYPE_AES_ECB: // AES ECB
            eAlgo    = E_DSCMB_MAIN_ALGO_AES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_ECB ;
            break;
        case E_DSCMB_TYPE_AES_CTR:
            eAlgo    = E_DSCMB_MAIN_ALGO_AES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_CTR ;
            eRes     = E_DSCMB_RESSB_ALGO_CTR ;
            eSB      = E_DSCMB_RESSB_ALGO_CTR ;
            break;
        case E_DSCMB_TYPE_AES_SCTE52: //AES - SCTE 52 - CBC
            eAlgo    = E_DSCMB_MAIN_ALGO_AES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_CBC ;
            eRes     = E_DSCMB_RESSB_ALGO_SCTE52 ;
            eSB      = E_DSCMB_RESSB_ALGO_SCTE52 ;
            break;
        case E_DSCMB_TYPE_TDES_ECB:
            eAlgo    = E_DSCMB_MAIN_ALGO_TDES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_ECB ;
            break;
        case E_DSCMB_TYPE_TDES_SCTE52:
            eAlgo    = E_DSCMB_MAIN_ALGO_TDES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_CBC ;
            eRes     = E_DSCMB_RESSB_ALGO_SCTE52 ;
            eSB      = E_DSCMB_RESSB_ALGO_SCTE52 ;
            break;
        case E_DSCMB_TYPE_SYN_AES:
            eAlgo    = E_DSCMB_MAIN_ALGO_AES ;
            eSubAlgo = E_DSCMB_SUB_ALGO_LEADING_CLEAR ;
            break;
        case E_DSCMB_TYPE_MULTI2: //[FIXME] check setting
            eAlgo    = E_DSCMB_MAIN_ALGO_MULTI2 ;
            eSubAlgo = E_DSCMB_SUB_ALGO_CBC ;
            eRes     = E_DSCMB_RESSB_ALGO_SCTE52 ;
            eSB      = E_DSCMB_RESSB_ALGO_CLR ;
            break;
        case E_DSCMB_TYPE_CSA3:
            eAlgo    = E_DSCMB_MAIN_ALGO_CSA3 ;
            break;
        default:
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Invalid Algorithm Type\n");
            return FALSE;
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Algo info %d %d %d %d %d \n", pMap->bDscmb, eAlgo, eSubAlgo, eRes, eSB);

    DSCMB_Algo_Cfg stAlgoConfig =
    {
        .eMainAlgo= eAlgo,
        .eSubAlgo = eSubAlgo,
        .eResAlgo = eRes,
        .eSBAlgo  = eSB,
        .bDecrypt = pMap->bDscmb
    };

    DSCMB_FSCB eFSCB = ((stAlgoConfig.bDecrypt == TRUE)?DSCMB_FSCB_CLEAR : DSCMB_FSCB_UNCHG);

    if(FALSE == _MDrv_DSCMB2_EngSetAlgo (u32EngId, u32DscmbId, E_DSCMB_ENG_ESA, stAlgoConfig) )
    {
        return FALSE;
    }

    if(FALSE == _MDrv_DSCMB2_EngSetSwitch (0, u32DscmbId, 0, E_DSCMB_ENG_ESA))
    {
        return FALSE;
    }

    if(FALSE == _MDrv_DSCMB2_EngSetFSCB (0, u32DscmbId, eFSCB))
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    return _MDrv_DSCMB2_EngSetKey (u32EngId, u32DscmbId, E_DSCMB_ENG_ESA, eKeyType, pu8Key);
}

MS_BOOL _MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    return _MDrv_DSCMB2_EngSetIV (u32EngId, u32DscmbId, eKeyType, pu8IV);
}

MS_BOOL _MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType)
{
    return _MDrv_DSCMB2_EngResetKey(u32EngId, u32DscmbId, E_DSCMB_ENG_ESA, eKeyType);
}

MS_BOOL _MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb)
{
    DSCMB_INIT_CHECK(FALSE);

    stDscmbTspMap* pMap = NULL;
    DSCMB_LOCK(_s32DscMutexId);

    pMap = &DscmbTspMap[u32EngId][u32DscmbId];
    pMap->bDscmb = bDscmb ;

    DSCMB_UNLOCK(_s32DscMutexId);

    return TRUE ;
}

#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
MS_BOOL _MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 j = 0;
    MS_BOOL bConnected = FALSE;

    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    for(j = 1 ; j < HAL_DSCMB_PIDFLT_NUM; j ++)
    {
        if(HAL_DSCMB_PidFlt_GetPid(j) == u32Pid)
        {
            if(_MDrv_DSCMB2_FltConnectFltId(u32EngId, u32DscmbId, j) == FALSE)
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }
            bConnected = TRUE;
        }
    }

    return bConnected;

}


MS_BOOL _MDrv_DSCMB2_FltDisconnectPid_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 j = 0;
    MS_BOOL bDisconnected = FALSE;

    DSCMB_ASSERT_AND_RETURN(TRUE == DscmbTspMap[u32EngId][u32DscmbId].bUsed);

    for(j = 1 ; j < HAL_DSCMB_PIDFLT_NUM; j ++)
    {
        if(HAL_DSCMB_PidFlt_GetPid(j) == u32Pid)
        {
            if(_MDrv_DSCMB2_FltDisconnectFltId(u32EngId, u32DscmbId, j) == FALSE)
            {
                DSCMB_ASSERT_AND_RETURN(0);
                return FALSE;
            }
            bDisconnected = TRUE;
        }
    }

    return bDisconnected;

}
#endif//MSOS_TYPE_NUTTX , MSOS_TYPE_NUTTX

//------------------------------------- ------------------//
//----------------------Keyladder API -----------------------//
//-------------------------------------------------------//
//--------------------------------------------------------------------------------------------------
/// An atomic Key ladder function for Key generation, including common KL use cases.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status )
{
    KL_INIT_CHECK(FALSE);
    DSCMB_ASSERT(KLCfg != NULL, "KLCfg input error", FALSE);
    DSCMB_ASSERT(u32Status != NULL, "u32Status input error", FALSE);

    MS_U32 i = 0;
    MS_U32 u32ErrMsg = 0;
    //  MS_BOOL bKeepRound=FALSE;
    MS_U32 u32FltId = 0;
    MS_U32 u32Field = 0;
    MS_U32 u32CAVid = KLCfg->u32CAVid;

    DSCMB_KL_SelEng eKLEng = KLCfg->eKLSel;
    DSCMB_KLDst eKLDst = KLCfg->eDst & (~E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_KLSrc eKLSrc = KLCfg->eSrc & (~E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnableKDF = ((KLCfg->eSrc & E_DSCMB_KL_SRC_KDF_ENABLE) == E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnablePidslotmap = ((KLCfg->eDst & E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE) == E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_Key_Type eKeyType = KLCfg->eKeyType & (~E_DSCMB_KEY_FSCB_ENABLE);
#ifdef ENABLE_KEY_FSCB
    MS_BOOL bEnableKeyFscb = ((KLCfg->eKeyType & E_DSCMB_KEY_FSCB_ENABLE) == E_DSCMB_KEY_FSCB_ENABLE);
#endif
    MS_BOOL bKLDstSecure = ((KLCfg->eKeyType & E_DSCMB_KEY_SECURE_KEYS_ENABLE) == E_DSCMB_KEY_SECURE_KEYS_ENABLE);

    if(_MDrv_DSCMB2_KLadder_Init () == FALSE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Keyladder Init Fail\n");
        return FALSE;
    }

    MDrv_DSCMB_KLadder_Lock();
    if ( FALSE == HAL_DSCMB_KL_HW_SEM_Lock(eKLEng, DSCMB_KL_WAIT_THRESHOLD)) //TODO: fix constant value
    {
        *u32Status = KL_STATUS_WAIT_KL_READY_TIMEOUT;
        goto KL_Fail;
    }
    *u32Status = KL_STATUS_KEY_OK ;

    ///translate cavid to Default CAVID if user set input cavid to 0x0
    if(u32CAVid == 0)
    {
        u32CAVid = DEFAULT_CAVID;
    }

    /////////////// Check Source and acpu data ////////////////////
    if ((E_DSCMB_KL_SRC_ACPU == eKLSrc) && (NULL == KLCfg->u8KeyACPU))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    /////////////// Check KL level ////////////////////
    if((KLCfg->u32Level >5) && (KLCfg->u32Level != 9) && (KLCfg->u32Level != 0xD))
    {
        //bKeepRound = TRUE ;
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }
    else if(( 0 == KLCfg->u32Level ) && (0==(DSCMB_KL_DST_CLASS_DMA & eKLDst)))
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    ////////////// check Key destination ///////////////
    /// Compatible and translate old version to new version
    if( ((eKLDst & 0xF0) == 0) && ( (eKLDst & 0x0F) > 0 )) //OLD Destination
    {
        switch(eKLDst)
        {
            case E_DSCMB_KL_DST_ACPU:
                eKLDst = E_DSCMB_KL_DST_CPU_ACPU;
                break;
            case E_DSCMB_KL_DST_KT_NSA:
                eKLDst = E_DSCMB_KL_DST_KT_ESA;
                break;
            case E_DSCMB_KL_DST_DMA_AES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_DMA_TDES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_PRIVATE0:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_0;
                break;
            case E_DSCMB_KL_DST_PRIVATE1:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_1;
                break;
            case E_DSCMB_KL_DST_PRIVATE2:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_2;
                break;
            default:
                ;
        }
    }

    // If destination is key table, and use dscmbId to get pidslotmap entry,
    // need to check descrambler driver init
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnablePidslotmap == FALSE)
        {
            DSCMB_INIT_CHECK(FALSE);
        }
    }

    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        //KL to ACPU
        if(NULL == ACPU_Out)
        {
            *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
            goto KL_Fail;
        }
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        //KL to KT
        if(bEnablePidslotmap)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination KT class\n");
            u32FltId = KLCfg->u32DscID;
        }
        else
        {
            if ( FALSE == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].bUsed )
            {
                //The keyslot is not allocated for the DSCMB ID
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
           }
            if( 0 == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].u32PidFltIdNum )
            {
                //No pid filter is connected to the DSCMB ID
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
            }

            // Get Filter ID
            if(FALSE == _DSCMB_Dscmb2Flt(KLCfg->u32DscID, &u32FltId))
            {
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
            }

        }
        // Get Field

    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        //KL to DMA
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination DMA class\n");
    }
    else if(DSCMB_KL_DST_CLASS_PVT &eKLDst)
    {
        //KL to PVT
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination Private Key class\n");
    }
    else if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        //KL to TSIO
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination TSIO\n");
        u32FltId = KLCfg->u32DscID; //TSIO service id
    }
    else if(DSCMB_KL_DST_CLASS_CFB& eKLDst)
    {
        //KL to CFB
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination CFB\n");
    }
    else
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Unknown Keyladder destination\n");
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    //Check KeyLadder Input
    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    // TODO: Here need to check key property if support KDF or not

    if((E_DSCMB_KL_TYPE_INF == _u8KLPreLevel) || ((E_DSCMB_KL_TYPE_INF == _u8KLPreLevel) && (E_DSCMB_KL_TYPE_LV1 == KLCfg->u32Level)))
    {
        _u8KLPreLevel = KLCfg->u32Level;
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder keep round and not reset \n");
    }
    else
    {
        _u8KLPreLevel = KLCfg->u32Level;
        HAL_DSCMB_KL_Reset(eKLEng);
        HAL_DSCMB_KL_KDF_Disable(eKLEng);
    }

    if(E_DSCMB_KL_SRC_ACPU == eKLSrc)
    {
        HAL_DSCMB_KL_ACPURootKey(eKLEng, KLCfg->u8KeyACPU);
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "u32CAVid=%X\n", u32CAVid);
    //FIXME: If Key from OTP, should we set usage & entropy ?
    if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 1 /*Entropy*/);
    }
    else
    {
        HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 0 /*Entropy*/);
    }

    //Enable KDF
    if(bEnableKDF)
    {
        HAL_DSCMB_KL_KDF_Enable( eKLEng, KLCfg->stKDF.eKDFType, KLCfg->stKDF.u16AppId, KLCfg->stKDF.eHWKeyId);
    }

    // Set KL Level input //
    if(E_DSCMB_KL_TYPE_INF == KLCfg->u32Level) //Keep round, 1 stage
    {
        HAL_DSCMB_KL_Input(eKLEng, 0, KLCfg->pu8KeyKLIn);
    }
    else if(E_DSCMB_KL_TYPE_S5 == KLCfg->u32Level) //SMI Keyladder
    {
        for( i = 0 ; i < 5 ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (4 - i), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else if(KLCfg->u32Level <= 5) //0~5 level keyladder
    {
        for( i = 0 ; i < KLCfg->u32Level ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Unknown Keyladder type(level) setting\n");
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    // Set KL Ctrl
    if(FALSE == HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo, KLCfg->u32Level, eKLSrc, eKLDst))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Invalid Keyladder control setting\n");
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    //KL to secure key slot
    HAL_DSCMB_KL_DstSecure(eKLEng, bKLDstSecure);

    if (DSCMB_KL_DST_CLASS_CPU & eKLDst)
    {
        // KL flow for ACPU output //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0);               /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
#ifdef ENABLE_KEY_FSCB
        if(bEnableKeyFscb){
            // KL flow for output  to KTE (with KeyFSCB) //
            HAL_DSCMB_KL_KeyBus_Ex(eKLEng,
                                  u32FltId,               /*For Dst KT*/
                                  u32Field,               /*For Dst KT*/
                                  eKeyType,               /*For Dst KT*/
                                  eKLDst,                 /*For Dst KT*/
                                  KLCfg->eFSCB);          /*For Dst KT*/
        }
        else{
#endif
        // KL flow for output  to KTE //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst KT*/
                            u32Field,               /*For Dst KT*/
                            eKeyType,               /*For Dst KT*/
                            eKLDst);                /*For Dst KT*/
#ifdef ENABLE_KEY_FSCB
        }
#endif
    }
    else if (DSCMB_KL_DST_CLASS_PVT & eKLDst)
    {
        // KL flow for output  to PVT //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0);              /*For Dst DMA*/
    }
    else if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        // KL flow for output  to TSIO //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst TSIO*/
                            0,                      /*For Dst TSIO*/
                            KLCfg->eKeyType,        /*For Dst TSIO*/
                            eKLDst);                /*For Dst TSIO*/
    }
    else if(DSCMB_KL_DST_CLASS_CFB& eKLDst)
    {
        // KL flow for output  to CFB  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst CFB*/
    }

    if(HAL_DSCMB_KL_Start(eKLEng) == FALSE)
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);
        goto KL_Fail;
    }

    if ((DSCMB_KL_DST_CLASS_SPSSPD & eKLDst) == DSCMB_KL_DST_CLASS_SPSSPD)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_SP_KeyCtrl(eKLEng, eKLDst);
        if(HAL_DSCMB_KL_SP_Start(eKLEng) == FALSE)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL to SPS SPD Fail\n\033[m");
            goto KL_Fail;
        }
    }

    // enable key
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        DSCMB_Eng_Type eEngType = E_DSCMB_ENG_ESA;
        if(eKLDst == E_DSCMB_KL_DST_KT_ESA )
        {
            eEngType = E_DSCMB_ENG_ESA;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAS )
        {
            eEngType = E_DSCMB_ENG_LSAS;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAD )
        {
            eEngType = E_DSCMB_ENG_LSAD;
        }

        if (FALSE == HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId,
                                            eKeyType,
                                            eEngType,
                                            u32CAVid,
                                            TRUE,
                                            TRUE))
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //Output to ACPU
    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        MS_U32 u32OutKeySize = 0;
        if(KLCfg->eOutsize == E_DSCMB_KL_64_BITS)
        {
            u32OutKeySize = 8;
        }
        else if(KLCfg->eOutsize == E_DSCMB_KL_128_BITS)
        {
            u32OutKeySize = 16;
        }
        memset(ACPU_Out , 0x0 , u32OutKeySize );
        HAL_DSCMB_KL_GetACPUOut(eKLEng,ACPU_Out,u32OutKeySize);
    }

    HAL_DSCMB_KL_HW_SEM_Unlock(eKLEng);
    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;

KL_Fail:

    HAL_DSCMB_KL_HW_SEM_Unlock(eKLEng);
    MDrv_DSCMB_KLadder_Unlock();
    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB KL Fail = %08X \n",*u32Status);
    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;
}

#if 0
//--------------------------------------------------------------------------------------------------
/// An atomic Key ladder function for TCSA3 Key generation.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_KLadder_TCSA3(DSCMB_KLCfg_All* KLCfg , MS_U8 *pu8ActCode, MS_U32 u32CHSel,DSCMB_KL_Status* u32Status )
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 i;
    *u32Status = KL_STATUS_KEY_OK ;
    DSCMB_KL_SelEng eKLEng = E_DSCMB_KL_SEL_CW;
    MS_U32 u32FltId = 0;
    MS_U32 u32ErrMsg = 0;
    MS_U32 u32Step2Level = KLCfg->u32Level-1;  /*level - 1*/
    MS_BOOL bKDFBusy = TRUE;
    DSCMB_Key_Type eKeyType = KLCfg->eKeyType & (~E_DSCMB_KEY_FSCB_ENABLE);
    //MS_BOOL bEnableKeyFscb = ((KLCfg->eKeyType & E_DSCMB_KEY_FSCB_ENABLE) == E_DSCMB_KEY_FSCB_ENABLE);

    MDrv_DSCMB_KLadder_Lock();

    // TODO: This global cavid may not work, dscmb id has its own cavid, use it in the future.
    if( E_HAL_DSCMB_CAVID4 != HAL_DSCMB_GetCAVid())
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    if(FALSE == HAL_DSCMB_KL_KDF_Busy(E_DSCMB_KL_SEL_CW, &bKDFBusy))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }
    else
    {
        if(bKDFBusy == TRUE)
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //////////////////////////////////////////////////
    ////////////Step 1, KL Output Clear CW to KT  ////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_Reset(eKLEng);

    for( i = 0 ; i < KLCfg->u32Level ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }

    HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo,
                      KLCfg->u32Level,
                      KLCfg->eSrc,
                      E_DSCMB_KL_DST_KT_ESA/*output to KT ESA*/);

    // Get Filter ID
    if(FALSE == _DSCMB_Dscmb2Flt(KLCfg->u32DscID, &u32FltId))
    {
        *u32Status = KL_STATUS_IVALID_DSCMB_ID;
        goto KL_Fail;
    }
    DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Filter ID %u\n\033[m", u32FltId);

    HAL_DSCMB_KL_KeyBus(eKLEng,
                        u32FltId,                            /*For Dst KT*/
                        0,                                   /*For Dst KT*/ /*No use*/
                        eKeyType,                            /*For Dst KT*/
                        E_HAL_DSCMB_KL_DSTKT_ESA);           /*For Dst DMA*/

    //Enable the key slot
    if (FALSE == HAL_DSCMB_KTE_Key_Ctrl(u32FltId,
                                        eKeyType,
                                        E_DSCMB_ENG_ESA,
                                        KLCfg->u32CAVid,
                                        TRUE))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    if(FALSE == HAL_DSCMB_KL_Start(eKLEng))
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);
    }

    //////////////////////////////////////////////////
    ////////////Step 2, KL Output to CSSK////////////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_Reset(eKLEng);

    // Set KL Level input    /*level - 1*/
    for( i = 0 ; i < (u32Step2Level) ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (u32Step2Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }

    HAL_DSCMB_KL_KeyBus(eKLEng,
                        0,                              /*For Dst KT*/
                        0,                              /*For Dst KT*/ /*No use*/
                        0,                              /*For Dst KT*/
                        E_DSCMB_KL_DST_DMA_CSSK);    /*For Dst DMA*/

    // Set KL Ctrl
    HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo,
                      u32Step2Level, /*level - 1*/
                      KLCfg->eSrc,
                      E_DSCMB_KL_DST_DMA_CSSK /*FIXME to CSSK*/);


    if(FALSE == HAL_DSCMB_KL_Start(eKLEng))
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);
    }

    //////////////////////////////////////////////////
    ////////////Step 3, KL Output IV & RK to CSA3 ////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_TCSA3_ActCode(pu8ActCode,16);
    HAL_DSCMB_KL_TCSA3_CHSel(u32CHSel);
    HAL_DSCMB_KL_TCSA3_Start();

    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);

    if(FALSE == HAL_DSCMB_Cipher_TCSA3(u32CHSel,TRUE))
    {
        *u32Status = KL_STATUS_IVALID_DSCMB_ID;
        goto KL_Fail;
    }

    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;

KL_Fail:


#if 0

    i = HAL_DSCMB_KLadder_Status();
    if ( i != 0 )
    {
        //printf("KL Satus = %x \n",i );
        *u32Status |= i ;
    }

    MDrv_DSCMB_KLadder_Stop();
#endif

    MDrv_DSCMB_KLadder_Unlock();
    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB KL Fail = %08X \n",*u32Status);
    DSCMB_ASSERT_AND_RETURN(FALSE);
    return FALSE;

}
#endif

//--------------------------------------------------------------------------------------------------
/// An ETSI Key ladder function for Key generation, including common KL use cases.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param pu8Nonce\b IN: A 16 bytes array pointer for Nonce input
/// @param pu8Response\n OUT: A 16 byte array pointer for Response output
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 *pu8Response, DSCMB_KL_Status* u32Status )
{
    KL_INIT_CHECK(FALSE);
    DSCMB_ASSERT(KLCfg != NULL, "KLCfg input error", FALSE);
    DSCMB_ASSERT(u32Status != NULL, "u32Status input error", FALSE);
    DSCMB_ASSERT(pu8Nonce != NULL, "pu8Nonce input error", FALSE);

    MS_U32 i = 0;
    MS_U32 u32ErrMsg = 0;
    MS_U32 u32FltId = 0;
    MS_U32 u32Field = 0;
    MS_U32 u32CAVid = KLCfg->u32CAVid;

    DSCMB_KL_SelEng eKLEng = KLCfg->eKLSel;
    DSCMB_KLDst eKLDst = KLCfg->eDst & (~E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_KLSrc eKLSrc = KLCfg->eSrc & (~E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnableKDF = ((KLCfg->eSrc & E_DSCMB_KL_SRC_KDF_ENABLE) == E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnablePidslotmap = ((KLCfg->eDst & E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE) == E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_Key_Type eKeyType = KLCfg->eKeyType & (~E_DSCMB_KEY_FSCB_ENABLE);
#ifdef ENABLE_KEY_FSCB
    MS_BOOL bEnableKeyFscb = ((KLCfg->eKeyType & E_DSCMB_KEY_FSCB_ENABLE) == E_DSCMB_KEY_FSCB_ENABLE);
#endif
    MS_BOOL bKLDstSecure = ((KLCfg->eKeyType & E_DSCMB_KEY_SECURE_KEYS_ENABLE) == E_DSCMB_KEY_SECURE_KEYS_ENABLE);

    if(_MDrv_DSCMB2_KLadder_Init () == FALSE)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Keyladder Init Fail\n");
        return FALSE;
    }

    MDrv_DSCMB_KLadder_Lock();
    if ( FALSE == HAL_DSCMB_KL_HW_SEM_Lock(eKLEng, DSCMB_KL_WAIT_THRESHOLD)) //TODO: fix constant value
    {
        *u32Status = KL_STATUS_WAIT_KL_READY_TIMEOUT;
        goto KL_Fail;
    }

    *u32Status = KL_STATUS_KEY_OK ;

    ///translate cavid to Default CAVID if user set input cavid to 0x0
    if(u32CAVid == 0)
    {
        u32CAVid = DEFAULT_CAVID;
    }

    /////////////// Check Source and acpu data ////////////////////
    if ((E_DSCMB_KL_SRC_ACPU == eKLSrc) && (NULL == KLCfg->u8KeyACPU))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    /////////////// Check KL level ////////////////////
    if(KLCfg->u32Level != 3) //ETSI keyladder, must be 3 level!
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    ////////////// check Key destination ///////////////
    /// Compatible and translate old version to new version
    if( ((eKLDst & 0xF0) == 0) && ( (eKLDst & 0x0F) > 0 )) //OLD Destination
    {
        switch(eKLDst)
        {
            case E_DSCMB_KL_DST_ACPU:
                eKLDst = E_DSCMB_KL_DST_CPU_ACPU;
                break;
            case E_DSCMB_KL_DST_KT_NSA:
                eKLDst = E_DSCMB_KL_DST_KT_ESA;
                break;
            case E_DSCMB_KL_DST_DMA_AES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_DMA_TDES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_PRIVATE0:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_0;
                break;
            case E_DSCMB_KL_DST_PRIVATE1:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_1;
                break;
            case E_DSCMB_KL_DST_PRIVATE2:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_2;
                break;
            default:
                ;
        }
    }

    // If destination is key table, and use dscmbId to get pidslotmap entry,
    // need to check descrambler driver init
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnablePidslotmap == FALSE)
        {
            DSCMB_INIT_CHECK(FALSE);
        }
    }

    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        //KL to ACPU
        if(NULL == ACPU_Out)
        {
            *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
            goto KL_Fail;
        }
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        //KL to KT
        if(bEnablePidslotmap)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination KT class\n");
            u32FltId = KLCfg->u32DscID;
        }
        else
        {
            if ( FALSE == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].bUsed )
            {
                    //The keyslot is not allocated for the DSCMB ID
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
            }
            if( 0 == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].u32PidFltIdNum )
            {
                    //No pid filter is connected to the DSCMB ID
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
            }

            // Get Filter ID
            if(FALSE == _DSCMB_Dscmb2Flt(KLCfg->u32DscID, &u32FltId))
            {
                *u32Status = KL_STATUS_IVALID_DSCMB_ID;
                goto KL_Fail;
            }

        }
        // Get Field

    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        //KL to DMA
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination DMA class\n");
    }
    else if(DSCMB_KL_DST_CLASS_PVT &eKLDst)
    {
        //KL to PVT
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination Private Key class\n");
    }
    else if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        //KL to TSIO
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination TSIO\n");
        u32FltId = KLCfg->u32DscID; //TSIO service id
    }
    else if(DSCMB_KL_DST_CLASS_CFB& eKLDst)
    {
        //KL to CFB
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination CFB\n");
    }
    else
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Unknown Keyladder destination\n");
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    //Check KeyLadder Input
    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    // TODO: Here need to check key property if support KDF or not

    HAL_DSCMB_KL_Reset(eKLEng);
    HAL_DSCMB_KL_KDF_Disable(eKLEng);


    if(E_DSCMB_KL_SRC_ACPU == eKLSrc)
    {
        HAL_DSCMB_KL_ACPURootKey(eKLEng, KLCfg->u8KeyACPU);
    }

    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "u32CAVid=%X\n", u32CAVid);
    //FIXME: If Key from OTP, should we set usage & entropy ?
    if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 1 /*Entropy*/);
    }
    else
    {
        HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 0 /*Entropy*/);
    }

    //Enable KDF
    if(bEnableKDF)
    {
        HAL_DSCMB_KL_KDF_Enable( eKLEng, KLCfg->stKDF.eKDFType, KLCfg->stKDF.u16AppId, KLCfg->stKDF.eHWKeyId);
    }

    // Set KL Level input //
    for( i = 0 ; i < KLCfg->u32Level ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }


    //Set ETSI KL Nonce input
    HAL_DSCMB_KL_ETSI_Nonce(eKLEng, pu8Nonce);

    // Set KL Ctrl
    if(FALSE == HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo, KLCfg->u32Level, eKLSrc, eKLDst))
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Invalid Keyladder control setting\n");
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;

    }

    //KL to secure key slot
    HAL_DSCMB_KL_DstSecure(eKLEng, bKLDstSecure);

    if (DSCMB_KL_DST_CLASS_CPU & eKLDst)
    {
        // KL flow for ACPU output //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0);     /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
#ifdef ENABLE_KEY_FSCB
        if(bEnableKeyFscb){
            // KL flow for output  to KTE (with KeyFSCB) //
            HAL_DSCMB_KL_KeyBus_Ex(eKLEng,
                                  u32FltId,               /*For Dst KT*/
                                  u32Field,               /*For Dst KT*/
                                  eKeyType,               /*For Dst KT*/
                                  eKLDst,                 /*For Dst KT*/
                                  KLCfg->eFSCB);          /*For Dst KT*/
        }
        else{
#endif
        // KL flow for output  to KTE //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst KT*/
                            u32Field,               /*For Dst KT*/
                            eKeyType,               /*For Dst KT*/
                            eKLDst);                 /*For Dst KT*/
#ifdef ENABLE_KEY_FSCB
        }
#endif
    }
    else if (DSCMB_KL_DST_CLASS_PVT & eKLDst)
    {
        // KL flow for output  to PVT //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0);              /*For Dst DMA*/
    }
    else if(DSCMB_KL_DST_CLASS_TSIO & eKLDst)
    {
        // KL flow for output  to TSIO //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst TSIO*/
                            0,                      /*For Dst TSIO*/
                            KLCfg->eKeyType,        /*For Dst TSIO*/
                            eKLDst);                /*For Dst TSIO*/
    }
    else if(DSCMB_KL_DST_CLASS_CFB& eKLDst)
    {
        // KL flow for output  to CFB  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst CFB*/
    }

    if(HAL_DSCMB_KL_Start(eKLEng) == FALSE)
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);
        goto KL_Fail;
    }

    if ((DSCMB_KL_DST_CLASS_SPSSPD & eKLDst) == DSCMB_KL_DST_CLASS_SPSSPD)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_SP_KeyCtrl(eKLEng, eKLDst);
        if(HAL_DSCMB_KL_SP_Start(eKLEng) == FALSE)
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL to SPS SPD Fail\n\033[m");
            goto KL_Fail;
        }
    }

    // enable key
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        DSCMB_Eng_Type eEngType = E_DSCMB_ENG_ESA;
        if(eKLDst == E_DSCMB_KL_DST_KT_ESA )
        {
            eEngType = E_DSCMB_ENG_ESA;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAS )
        {
            eEngType = E_DSCMB_ENG_LSAS;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAD )
        {
            eEngType = E_DSCMB_ENG_LSAD;
        }

        if (FALSE == HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId,
                                            eKeyType,
                                            eEngType,
                                            u32CAVid,
                                            TRUE,
                                            TRUE))
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //Output to ACPU
    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        MS_U32 u32OutKeySize = 0;
        if(KLCfg->eOutsize == E_DSCMB_KL_64_BITS)
        {
            u32OutKeySize = 8;
        }
        else if(KLCfg->eOutsize == E_DSCMB_KL_128_BITS)
        {
            u32OutKeySize = 16;
        }
        memset(ACPU_Out , 0x0 , u32OutKeySize );
        HAL_DSCMB_KL_GetACPUOut(eKLEng,ACPU_Out,u32OutKeySize);
    }

    HAL_DSCMB_KL_ETSI_Response(eKLEng, pu8Response);

    HAL_DSCMB_KL_HW_SEM_Unlock(eKLEng);
    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;

KL_Fail:

    HAL_DSCMB_KL_HW_SEM_Unlock(eKLEng);
    MDrv_DSCMB_KLadder_Unlock();
    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB KL Fail = %08X \n",*u32Status);
    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;
}

#if 0
DLL_PUBLIC MS_BOOL MDrv_DSCMB_KLadder_TCSA3_TA(DSCMB_KLCfg_All* KLCfg , MS_U8 *pu8ActCode, MS_U32 u32CHSel, MS_U8 *
        pu8EncCW, DSCMB_KL_TA_LUT3* LUT3Tab, DSCMB_KL_Status* u32Status)
{
    MS_U32 i = 0;
    *u32Status = KL_STATUS_KEY_OK ;
    DSCMB_KL_SelEng eKLEng = E_DSCMB_KL_SEL_CW;
    MS_U32 u32FltId = 0;
    MS_U32 u32ErrMsg = 0;
    MS_U32 u32Step2Level = KLCfg->u32Level-1;  /*level - 1*/
    MS_BOOL bKDFBusy = TRUE;

    MDrv_DSCMB_KLadder_Lock();

    if( DRV_DSCMB_CAVID_IRD != HAL_DSCMB_GetCavid())
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    if(FALSE == HAL_DSCMB_KL_KDF_Busy(E_DSCMB_KL_SEL_CW, &bKDFBusy))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }
    else
    {
        if(bKDFBusy == TRUE)
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //////////////////////////////////////////////////
    ////////////Step 1, KL Output Clear CW to KT  ////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_Reset(eKLEng);

    for( i = 0 ; i < KLCfg->u32Level ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }

    //Unlock LUT3 table
    HAL_DSCMB_KL_TA_EN(eKLEng, FALSE);

    //Set Transformed Algorithm EncCW
    if(FALSE == HAL_DSCMB_KL_TA_EncCW(eKLEng, pu8EncCW))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }
    //////////////////////////////////////////////////
    ////Set Transformed Algorithm LUT3 table address  //////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_TA_LUT3(eKLEng, LUT3Tab);

    //Enable TA and lock LUT3 table
    HAL_DSCMB_KL_TA_EN(eKLEng, TRUE);

    HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo,
                      KLCfg->u32Level,
                      KLCfg->eSrc,
                      E_DSCMB_KL_DST_KT_ESA/*output to KT ESA*/);

    // Get Filter ID
    if(FALSE == _DSCMB_Dscmb2Flt(KLCfg->u32DscID, &u32FltId))
    {
        *u32Status = KL_STATUS_IVALID_DSCMB_ID;
        goto KL_Fail;
    }
    DSCMB_DBG("\033[31m LINE %d, filter ID %lu\n\033[m", __LINE__, u32FltId);


    HAL_DSCMB_KL_KeyBus(eKLEng,
                        u32FltId,                            /*For Dst KT*/
                        0,                                   /*For Dst KT*/ /*No use*/
                        KLCfg->eKeyType,                     /*For Dst KT*/
                        E_HAL_DSCMB_KL_DSTKT_ESA);           /*For Dst DMA*/

    //Enable the key slot
    if (FALSE == HAL_DSCMB_KTE_Key_Ctrl(u32FltId,
                                        KLCfg->eKeyType,
                                        E_DSCMB_ENG_KEY_ESA,
                                        TRUE))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    HAL_DSCMB_KL_Start(eKLEng);

    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);

    DSCMB_DBG("\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);

    //////////////////////////////////////////////////
    ////////////Step 2, KL Output to CSSK////////////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_Reset(eKLEng);

    // Set KL Level input    /*level - 1*/
    for( i = 0 ; i < (u32Step2Level) ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (u32Step2Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }

    HAL_DSCMB_KL_KeyBus(eKLEng,
                        0,                              /*For Dst KT*/
                        0,                              /*For Dst KT*/ /*No use*/
                        0,                              /*For Dst KT*/
                        E_DSCMB_KL_DST_DMA_CSSK);    /*For Dst DMA*/

    // Set KL Ctrl
    HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo,
                      u32Step2Level, /*level - 1*/
                      KLCfg->eSrc,
                      E_DSCMB_KL_DST_DMA_CSSK /*FIXME to CSSK*/);

    HAL_DSCMB_KL_Start(eKLEng);

    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
    DSCMB_DBG("\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);

    //////////////////////////////////////////////////
    ////////////Step 3, KL Output IV & RK to CSA3 ////////
    //////////////////////////////////////////////////
    HAL_DSCMB_KL_TCSA3_ActCode(pu8ActCode,16);
    HAL_DSCMB_KL_TCSA3_CHSel(u32CHSel);
    HAL_DSCMB_KL_TCSA3_Start();

    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
    DSCMB_DBG("\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);

    if(FALSE == HAL_DSCMB_Cipher_TCSA3(u32CHSel,TRUE))
    {
        *u32Status = KL_STATUS_IVALID_DSCMB_ID;
        goto KL_Fail;
    }

    //Unlock LUT3 table and KL SRAM
    HAL_DSCMB_KL_TA_EN(eKLEng, FALSE);

    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;

KL_Fail:

    HAL_DSCMB_KL_TA_EN(eKLEng, FALSE);
    MDrv_DSCMB_KLadder_Unlock();
    printf("[%s][%d]TCSA3 TA DSCMB KL Fail = %08lX \n",__FUNCTION__,__LINE__,*u32Status );
    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;
}
#endif

MS_BOOL _MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput)
{
    HAL_DSCMB_Query_Type u32Cap;

    DSCMB_ASSERT_AND_RETURN(u32EngId == 0);
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
            u32Cap = E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM;
            break;
        case E_DSCMB_CAP_SUPPORT_KEY_TYPE:
            DSCMB_ASSERT_AND_RETURN(pInput != NULL);
            if((*(DSCMB_Key_Type *)pInput) <= E_DSCMB_KEY_ODD)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
            break;
        case E_DSCMB_CAP_PIDFILTER_MAP_START:
            u32Cap = E_HAL_DSCMB_CAP_PIDFILTER_MAP_START;
            break;
        case E_DSCMB_CAP_PIDFILTER_MAP_END:
            u32Cap = E_HAL_DSCMB_CAP_PIDFILTER_MAP_END;
            break;
        case E_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_NUM:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_START:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START;
            break;
        case E_DSCMB_CAP_SHARE_KEY_REGION_END:
            u32Cap = E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END;
            break;
        default:
    return FALSE;
            break;
    }

    return HAL_DSCMB_GetCap(u32EngId, u32Cap, pInput, pOutput);

}

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver library information
/// @param ppVersion \b OUT: Descrambler library version infomation
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_GetLibVer(const MSIF_Version **ppVersion)
{
    *ppVersion = &_drv_dscmb_version;
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Set Dscmb driver debug level
/// @param u32Level \b IN: debug level
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_SetDBGLevel(MS_U32 u32Level)
{
    _u32DSCMB2DbgLv = u32Level;
    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "%s level: %x\n", __FUNCTION__, u32Level);
    HAL_DSCMB_SetDBGLevel(u32Level);
    return TRUE;
}
MS_BOOL _MDrv_DSCMB2_PVR_RecCtrl(MS_U32 u32EngId, MS_BOOL Enable)
{
    //For compatible, always return TRUE because PVR MW will check the return status
    OBSOLETE_FUNCTION ("Not support\n");
    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel)
{
    OBSOLETE_FUNCTION ("Not support\n");
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_LOCK(_s32DscMutexId);
    HAL_DSCMB_KTE_Write_MULTI2_Round(u32Round);
    DSCMB_UNLOCK(_s32DscMutexId);

    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8Syskey)
{
    DSCMB_INIT_CHECK(FALSE);
    DSCMB_ASSERT(u8Syskey != NULL, "System key input NULL", FALSE);

    DSCMB_LOCK(_s32DscMutexId);
    HAL_DSCMB_KTE_Write_MULTI2_SysKey(u8Syskey, HAL_DSCMB_MULTI2_SYSKEY_MAX);
    DSCMB_UNLOCK(_s32DscMutexId);

    return TRUE;
}

MS_BOOL _MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV)
{
    OBSOLETE_FUNCTION ("Obsolete function\n");
    return FALSE;
}

MS_BOOL _MDrv_DSCMB2_HDCP2_SetRIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
    return _MDrv_DSCMB2_EngSetRIV(u32EngId, u32DscmbId, pu8RIV);
}

MS_BOOL _MDrv_DSCMB2_ES_SetStaticKey(MS_U32 u32EngId, MS_U8* pu8Statickey)
{
    DSCMB_INIT_CHECK(FALSE);

    DSCMB_LOCK(_s32DscMutexId);
    HAL_DSCMB_Cipher_ES_SetStaticKey(u32EngId, pu8Statickey, HAL_DSCMB_ES_STATICKEY_MAX);
    DSCMB_UNLOCK(_s32DscMutexId);

    return TRUE;
}

//------------------------------------- ------------------//
//----------------------Debug API -------------------------//
//-------------------------------------------------------//
MS_BOOL _MDrv_DSCMB2_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId)
{
    DSCMB_INIT_CHECK(FALSE);

    return _DSCMB_IsFltConnectDSCMB(u32DscmbId, u32TspFltId);
}

void _MDrv_DSCMB2_ReadSwitch(MS_U32 u32FltId, DSCMB_Key_Type type, MS_U32 u32CAVid)
{
    DSCMB_INIT_CHECK();

    DSCMB_LOCK(_s32DscMutexId);

    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
#ifdef FLEXIBLE_PIDSLOTMAP
        HAL_DSCMB_PrintSwitchSetup(_u32PidFlt2Dscmb[u32FltId].u8DscmbId, type, u32CAVid);
#else
        HAL_DSCMB_PrintSwitchSetup(u32FltId, type, u32CAVid);
#endif /* FLEXIBLE_PIDSLOTMAP */
    }

    DSCMB_UNLOCK(_s32DscMutexId);
}

void _MDrv_DSCMB2_ReadPidSlotMap(MS_U32 u32FltId)
{
    DSCMB_INIT_CHECK();

    PidSlotMapRead_t tReadData;
    memset(&tReadData, 0, sizeof(PidSlotMapRead_t));

    DSCMB_LOCK(_s32DscMutexId);

    if(_u32PidFlt2Dscmb[u32FltId].u8DscmbId != HAL_DSCMB_KTE_MAX)
    {
#ifdef FLEXIBLE_PIDSLOTMAP
        HAL_DSCMB_ReadPidSlotMap(_u32PidFlt2Dscmb[u32FltId].u8DscmbId, &tReadData);
#else
        HAL_DSCMB_ReadPidSlotMap(u32FltId, &tReadData);
#endif /* FLEXIBLE_PIDSLOTMAP */
    }

    DSCMB_UNLOCK(_s32DscMutexId);
}

void _MDrv_DSCMB2_PacketView(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32 Mode, MS_U32 u32Range, MS_BOOL bRst)
{
    DSCMB_INIT_CHECK();

    MS_U32 u32pktparser = 0;
    MS_U32 u32tsif = 0;
    Mode = Mode >= 1? TRUE: FALSE;

    //Mode = 1, enable    snap mode, stop when pktcnt is finished
    //Mode = 0, enable trigger mode, stop when badpkt occurs

    HAL_DSCMB_Get_TsidInput(u32FltId, &u32tsif);
    //printf("%s, u32tsif = %lx, reset = %d\n", __FUNCTION__, u32tsif, bRst);

    if(bRst == TRUE)
    {
        //reset....
        u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, TRUE, u32Range, 0, Mode);
        return;
    }

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_EVENT, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Event = 0x%08x\n", u32pktparser);
    HAL_DSCMB_PktParser_PrintEvent(u32pktparser);

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_SCB, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "SCB_00 = 0x%x\n", (u32pktparser & 0xFF000000) >>24);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "SCB_01 = 0x%x\n", (u32pktparser & 0x00FF0000) >>16);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "SCB_10 = 0x%x\n", (u32pktparser & 0x0000FF00) >> 8);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "SCB_11 = 0x%x\n", (u32pktparser & 0x000000FF) >> 0);

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_BADPKT, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "BADPKT_UPP = 0x%x\n", (u32pktparser & 0xFF000000) >>24);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "BADPKT_LOW = 0x%x\n", (u32pktparser & 0x00FF0000) >>16);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "PKT_UPP    = 0x%x\n", (u32pktparser & 0x0000FF00) >> 8);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "PKT_LOW    = 0x%x\n", (u32pktparser & 0x000000FF) >> 0);

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_2NDHIT, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "2NDHIT = 0x%x\n", u32pktparser);

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_ESA, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "ESA    = 0x%x\n", u32pktparser);

    u32pktparser = HAL_DSCMB_PktParser_Ctrl(u32Eng, u32tsif, FALSE, u32Range, E_HAL_DSCMB_PARSER_LSA, Mode);
    DRVDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "LSA    = 0x%x\n", u32pktparser);
}

MS_U32  MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    EN_POWER_MODE pArgs = u16PowerState;
    if(pu32DscmbInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, &pu32DscmbInst, 0x0, NULL))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_SetPowerState, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_SetPowerState(u16PowerState);
#endif
}


MS_BOOL MDrv_DSCMB2_Init(void)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32DscmbInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, &pu32DscmbInst, 0x0, NULL))
        {
            DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "UtopiaOpen DSCMB fail\n");
            return FALSE;
        }
    }

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Init, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_Init();
#endif
}

MS_BOOL MDrv_DSCMB2_Exit(void)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Exit, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_Exit();
#endif
}

MS_U32 MDrv_DSCMB2_FltAllocWithCAVid(MS_U32 u32EngId, DSCMB_Flt_Type eFltType, MS_U32 u32CAVid)
{
    return _MDrv_DSCMB2_FltAllocWithCAVid(u32EngId, eFltType, u32CAVid);
}

MS_U32 MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_DSCMB_FLT_NULL;
    }

    return pArgs.u32DscmbId;

#else
    return _MDrv_DSCMB2_FltAlloc_Ex(u32EngId, eFltType);
#endif
}

MS_U32 MDrv_DSCMB2_FltAlloc(MS_U32 u32EngId)
{
    return MDrv_DSCMB2_FltAlloc_Ex(u32EngId, E_DSCMB_FLT_2_KEYS);
}

//--------------------------------------------------------------------------------------------------
/// Free the specific DSCMB filter by ID
/// @return TRUE - SUCCESS
/// @return FALSE - Failure
/// @note
///
//--------------------------------------------------------------------------------------------------

MS_BOOL MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_FLTFREE pArgs;

    memset(&pArgs, 0, sizeof(DSCMB_FLTFREE));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32DscmbId = u32DscmbId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltFree, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_FltFree(u32EngId, u32DscmbId);
#endif
}


MS_BOOL MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_CONNECTFLT pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_CONNECTFLT));

    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32DmxFltId = u32FltId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltConnectFltId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltConnectFltId(u32EngId, u32DscmbId, u32FltId);
#endif
}



MS_BOOL MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DISCONNECTFLT pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DISCONNECTFLT));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32DmxFltId = u32FltId;
    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltDisconnectFltId, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDisconnectFltId(u32EngId, u32DscmbId, u32FltId);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_FSCB eForceSCB )
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGFSCB pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGFSCB));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId   = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.eForceSCB  = eForceSCB;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetFSCB, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetFSCB(u32EngId, u32DscmbId, eForceSCB);
#endif
}

// Set Upper & lower switch to dscmb filter //
//--------------------------------------------------------------------------------------------------
/// Set upper switch and lower switch to choose which engine will go into.
/// @param u32DscmbId\b IN: The descrambler id which is get from MDrv_DSCMB2_FltAlloc.
/// @param eUppSwitch\b IN: An Enum type for choosing descrambler engine when go through upper path.
/// @param eLowSwitch\b IN: An Enum type for choosing descrambler engine when go through lower path.
/// @return TRUE - SUCCESS
/// @return FALSE - Failure
/// @note
///
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB2_EngSetSwitch(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eUppSwitch, DSCMB_Eng_Type eLowSwitch)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGSWITCH pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGSWITCH));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eUppSwitch  = eUppSwitch;
    pArgs.eLowSwitch  = eLowSwitch;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetSwitch, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetSwitch(u32EngId, u32DscmbId, eUppSwitch, eLowSwitch);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetAlgo(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Algo_Cfg stConfig)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGALGO pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGALGO));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = eEngType;
    pArgs.stConfig    = stConfig;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetAlgo, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetAlgo(u32EngId, u32DscmbId, eEngType, stConfig);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetRIV(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
    return _MDrv_DSCMB2_EngSetRIV(u32EngId, u32DscmbId, pu8RIV);
}

MS_BOOL MDrv_DSCMB2_EngSetIV(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGIV pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGIV));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eKeyType    = eKeyType;
    pArgs.pu8IV       = pu8IV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetIV, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetIV(u32EngId, u32DscmbId, eKeyType, pu8IV);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId,  DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGIV_EX pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGIV_EX));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = eEngType;
    pArgs.eKeyType    = eKeyType;
    pArgs.pu8IV       = pu8IV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetIV_Ex, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetIV_Ex(u32EngId, u32DscmbId, eEngType, eKeyType, pu8IV);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetKeyFSCB(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, DSCMB_FSCB eForceSCB)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGKEYFSCB pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = eEngType;
    pArgs.eKeyType    = eKeyType;
    pArgs.eForceSCB   = eForceSCB;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetKeyFSCB, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetKeyFSCB(u32EngId, u32DscmbId, eEngType, eKeyType, eForceSCB);
#endif
}


MS_BOOL MDrv_DSCMB2_EngSetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGKEY pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = eEngType;
    pArgs.eKeyType    = eKeyType;
    pArgs.pu8Key      = pu8Key;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngSetKey, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngSetKey(u32EngId, u32DscmbId, eEngType, eKeyType, pu8Key);
#endif
}

MS_BOOL MDrv_DSCMB2_EngResetKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ENGRESETKEY pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ENGRESETKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.u32DscmbId  = u32DscmbId;
    pArgs.eEngType    = eEngType;
    pArgs.eKeyType    = eKeyType;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_EngResetKey, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_EngResetKey(u32EngId, u32DscmbId, eEngType, eKeyType);
#endif
}

MS_BOOL MDrv_DSCMB2_EngSetCAVid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32CAVid)
{
    return _MDrv_DSCMB2_EngSetCAVid(u32EngId, u32DscmbId, u32CAVid);
}

MS_BOOL MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DEFAULT_CAVID pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DEFAULT_CAVID));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId   = u32EngId;
    pArgs.u32CAVid   = u32CAVid;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_SetDefaultCAVid, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_SetDefaultCAVid(u32EngId, u32CAVid);
#endif
}

MS_BOOL MDrv_DSCMB2_IsKTEValid(MS_U32 u32FltId, DSCMB_Key_Type eKeyType)
{
    return _MDrv_DSCMB2_IsKTEValid (u32FltId, eKeyType);
}

MS_BOOL MDrv_DSCMB2_SetTSPCAVid(DSCMB_TSIF eSrcTSIf, MS_U32 u32CAVid)
{
    return _MDrv_DSCMB2_SetTSPCAVid(eSrcTSIf, u32CAVid);
}

MS_BOOL MDrv_DSCMB2_SetTSPCADst(MS_U32 u32FltId, MS_U32 u32UpDst, MS_U32 u32LowDst)
{
    return _MDrv_DSCMB2_SetTSPCADst(u32FltId, u32UpDst, u32LowDst);
}

MS_BOOL MDrv_DSCMB2_DualPath_Enable(MS_U32 u32EngId, MS_U32 u32FltId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DUALPATH pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DUALPATH));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId     = u32EngId;
    pArgs.u32DmxFltId  = u32FltId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_DualPath_Enable, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_DualPath_Enable(u32EngId, u32FltId);
#endif
}

MS_BOOL MDrv_DSCMB2_DualPath_Disable(MS_U32 u32EngId, MS_U32 u32FltId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_DUALPATH pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_DUALPATH));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId     = u32EngId;
    pArgs.u32DmxFltId  = u32FltId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_DualPath_Disable, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_DualPath_Disable(u32EngId, u32FltId);   //enable small switch pid_no dual path
#endif
}

MS_BOOL MDrv_DSCMB2_SPD_Enable(DSCMB_TSIF ePvrSrcTSIf)
{
    return _MDrv_DSCMB2_SPD_Enable(ePvrSrcTSIf);
}

MS_BOOL MDrv_DSCMB2_SPD_Disable(DSCMB_TSIF ePvrSrcTSIf)
{
    return _MDrv_DSCMB2_SPD_Disable(ePvrSrcTSIf);
}

MS_BOOL MDrv_DSCMB2_CAPVR_FlowSet(MS_U32 u32EngId, DSCMB_CAPVR_MODE eCaMode, DSCMB_TSIF ePvrSrcTsif)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_CAPVR_FLOWSET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_CAPVR_FLOWSET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId    = u32EngId;
    pArgs.eCaMode     = eCaMode;
    pArgs.ePvrSrcTsif = ePvrSrcTsif;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_CAPVR_FlowSet, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_CAPVR_FlowSet(u32EngId, eCaMode, ePvrSrcTsif);
#endif
}

MS_BOOL MDrv_DSCMB2_SetPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    return _MDrv_DSCMB2_SetPidPair(u32FltIdPri, u32FltIdSec);
}

MS_BOOL MDrv_DSCMB2_ClearPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    return _MDrv_DSCMB2_ClearPidPair(u32FltIdPri, u32FltIdSec);
}


MS_BOOL MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type DscmbType)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_TYPESET pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_TYPESET));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId   = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.eType = DscmbType;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_FltTypeSet, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltTypeSet(u32EngId, u32DscmbId, DscmbType);
#endif
}



MS_BOOL MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltKeySet(u32EngId, u32DscmbId, eKeyType, pu8Key);
#endif
}

MS_BOOL MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltIVSet(u32EngId, u32DscmbId, eKeyType, pu8IV);
#endif
}


MS_BOOL MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltKeyReset(u32EngId, u32DscmbId, eKeyType);
#endif
}

MS_BOOL MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDscmb(u32EngId, u32DscmbId, bDscmb);
#endif
}

#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
DLL_PUBLIC MS_BOOL MDrv_DSCMB2_FltDisconnectPid_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltDisconnectPid_Ex(u32EngId, u32DscmbId, u32Pid);
#endif

}

MS_BOOL MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_FltConnectPid(u32EngId, u32DscmbId, u32Pid);
#endif
}


MS_BOOL MDrv_DSCMB2_FltDisconnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid)
{
    return MDrv_DSCMB2_FltDisconnectPid_Ex(u32EngId, u32DscmbId, u32Pid);
}
#endif

//----------------------------------------------------------------------//
//                   New Interface for K3 Series                        //
//----------------------------------------------------------------------//
MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_KL_ATOMICEXEC pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_KL_ATOMICEXEC));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.KLCfg     = KLCfg;
    pArgs.ACPU_Out  = ACPU_Out;
    pArgs.u32Status = u32Status;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_KLadder_AtomicExec, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_KLadder_AtomicExec(KLCfg, ACPU_Out, u32Status);
#endif
}


//--------------------------------------------------------------------------------------------------
/// An atomic Key ladder function for TCSA3 Key generation.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
#if 0
MS_BOOL MDrv_DSCMB2_KLadder_TCSA3(DSCMB_KLCfg_All* KLCfg , MS_U8 *pu8ActCode, MS_U32 u32CHSel, DSCMB_KL_Status* u32Status )
{
    return _MDrv_DSCMB2_KLadder_TCSA3(KLCfg, pu8ActCode, u32CHSel, u32Status);
}
#endif

MS_BOOL MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 *pu8Response, DSCMB_KL_Status* u32Status )
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_KL_ETSI pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_KL_ETSI));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.KLCfg       = KLCfg;
    pArgs.ACPU_Out    = ACPU_Out;
    pArgs.pu8Nonce    = pu8Nonce;
    pArgs.pu8Response = pu8Response;
    pArgs.u32Status   = u32Status;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_KLadder_ETSI, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_KLadder_ETSI(KLCfg, ACPU_Out, pu8Nonce, pu8Response, u32Status);
#endif
}

#if 0
MS_BOOL MDrv_DSCMB2_KLadder_TA(DSCMB_KLCfg_All* KLCfg , MS_U8* ACPU_Out, DSCMB_KL_Status* u32Status)
{
    DSCMB_INIT_CHECK(FALSE);

    MS_U32 i = 0;
    MS_U32 u32ErrMsg = 0;
    *u32Status = KL_STATUS_KEY_OK ;
    DSCMB_KL_SelEng eKLEng = KLCfg->eKLSel;
    MS_U32 u32FltId = 0;
    MS_U32 u32Field = 0;
    DSCMB_KLDst eKLDst = KLCfg->eDst;
    MS_U32 u32CAVid = KLCfg->u32CAVid;

    MDrv_DSCMB_KLadder_Lock();
    ///translate cavid to Default CAVID if user set input cavid to 0x0
    if(u32CAVid == 0)
    {
        u32CAVid = DEFAULT_CAVID;
    }

    /////////////// Check Source and acpu data ////////////////////
    if ((E_DSCMB_KL_SRC_ACPU == KLCfg->eSrc) && (NULL == KLCfg->u8KeyACPU))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    /////////////// Check KL level ////////////////////
    if((KLCfg->u32Level >3) && (KLCfg->u32Level != 9) && (KLCfg->u32Level != 0xD))
    {
        //bKeepRound = TRUE ;
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }
    else if(( 0 == KLCfg->u32Level ) && (0==(DSCMB_KL_DST_CLASS_DMA & eKLDst)))
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    ////////////// check Key destination ///////////////
    /// Compatible and translate old version to new version
    if( ((KLCfg->eDst & 0xF0) == 0) && ( (KLCfg->eDst & 0x0F) > 0 )) //OLD Destination
    {
        switch(KLCfg->eDst)
        {
            case E_DSCMB_KL_DST_ACPU:
                eKLDst = E_DSCMB_KL_DST_CPU_ACPU;
                break;
            case E_DSCMB_KL_DST_KT_NSA:
                eKLDst = E_DSCMB_KL_DST_KT_ESA;
                break;
            case E_DSCMB_KL_DST_DMA_AES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_DMA_TDES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_PRIVATE0:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_0;
                break;
            case E_DSCMB_KL_DST_PRIVATE1:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_1;
                break;
            case E_DSCMB_KL_DST_PRIVATE2:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_2;
                break;
            default:
                ;
        }
    }

    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        //KL to ACPU
        if(NULL == ACPU_Out)
        {
            *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
            goto KL_Fail;
        }
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        //KL to KT
        if ( FALSE == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].bUsed )
        {
            //The keyslot is not allocated for the DSCMB ID
            *u32Status = KL_STATUS_IVALID_DSCMB_ID;
            goto KL_Fail;
        }
        if( 0 == DscmbTspMap[KLCfg->u32EngID][KLCfg->u32DscID].u32PidFltIdNum )
        {
            //No pid filter is connected to the DSCMB ID
            *u32Status = KL_STATUS_IVALID_DSCMB_ID;
            goto KL_Fail;
        }

        // Get Filter ID
        if(FALSE == _DSCMB_Dscmb2Flt(KLCfg->u32DscID, &u32FltId))
        {
            *u32Status = KL_STATUS_IVALID_DSCMB_ID;
            goto KL_Fail;
        }

        // Get Field

    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination DMA class\n");
    }
    else if(DSCMB_KL_DST_CLASS_PVT &eKLDst)
    {
        //KL to PVT
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder destination Private Key class\n");
    }
    else
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Unknown Keyladder destination\n");
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    //Check KeyLadder Input
    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    if((_u8KLPreLevel == 9) || ((_u8KLPreLevel == 9) && (KLCfg->u32Level == 1)))
    {
        _u8KLPreLevel = KLCfg->u32Level;
        DRVDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Keyladder keep round and not reset \n");
    }
    else
    {
        _u8KLPreLevel = KLCfg->u32Level;
        HAL_DSCMB_KL_Reset(eKLEng);
    }

    if(E_DSCMB_KL_SRC_ACPU == KLCfg->eSrc)
    {
        HAL_DSCMB_KL_ACPURootKey(eKLEng, KLCfg->u8KeyACPU);
    }


    DRVDSCMB2_DBG(DSCMB2_DBGLV_INFO, "u32CAVid=%lX\n", (unsigned long)u32CAVid);
    //FIXME: If Key from OTP, should we set usage & entropy ?
    HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 0 /*Entropy*/);

    // Set KL Level input //
    if(KLCfg->u32Level == 9) //Keep round, 1 stage
    {
        HAL_DSCMB_KL_Input(eKLEng, 0, KLCfg->pu8KeyKLIn);
    }
    else if(KLCfg->u32Level == 0xD) //SMI Keyladder
    {
        for( i = 0 ; i < 5 ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (4 - i), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else if(KLCfg->u32Level <= 3) //0~3 level keyladder
    {
        for( i = 0 ; i < KLCfg->u32Level ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Unknown Keyladder type(level) setting\n");
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    //Set Transformed Algorithm EncCW
    if(FALSE == HAL_DSCMB_KL_TA_EncCW(eKLEng, KLCfg->pu8KeyKLIn+(16*(KLCfg->u32Level-1)) ))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }
    //Set Transformed Algorithm LUT3 table address
    HAL_DSCMB_KL_TA_LUT3(eKLEng, NULL);

    //Enable TA and lock LUT3 table and KL SRAM
    HAL_DSCMB_KL_TA_EN(eKLEng, TRUE);

    // Set KL Ctrl
    HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo,
                      KLCfg->u32Level,
                      KLCfg->eSrc,
                      eKLDst);

    if (DSCMB_KL_DST_CLASS_CPU & eKLDst)
    {
        // KL flow for ACPU output //

        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0);               /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        // KL flow for output  to KTE //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst KT*/
                            u32Field,               /*For Dst KT*/
                            KLCfg->eKeyType,        /*For Dst KT*/
                            eKLDst);                 /*For Dst KT*/
    }
    else if (DSCMB_KL_DST_CLASS_PVT & eKLDst)
    {
        // KL flow for output  to PVT //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0);              /*For Dst DMA*/
    }

    if(HAL_DSCMB_KL_Start(eKLEng) == FALSE)
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "\033[31m KL error message 0x%X\n\033[m", (unsigned int)u32ErrMsg);
        goto KL_Fail;
    }

    // enable key
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        DSCMB_Eng_Type eEngType = E_DSCMB_ENG_ESA;
        if(eKLDst == E_DSCMB_KL_DST_KT_ESA )
        {
            eEngType = E_DSCMB_ENG_ESA;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAS )
        {
            eEngType = E_DSCMB_ENG_LSAS;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAD )
        {
            eEngType = E_DSCMB_ENG_LSAD;
        }

        if (FALSE == HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId,
                                            KLCfg->eKeyType,
                                            eEngType,
                                            KLCfg->u32CAVid,
                                            TRUE,
                                            TRUE))
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //Output to ACPU
    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        MS_U32 u32OutKeySize = 0;
        if(KLCfg->eOutsize == E_DSCMB_KL_64_BITS)
        {
            u32OutKeySize = 8;
        }
        else if(KLCfg->eOutsize == E_DSCMB_KL_128_BITS)
        {
            u32OutKeySize = 16;
        }
        memset(ACPU_Out , 0x0 , u32OutKeySize );
        HAL_DSCMB_KL_GetACPUOut(eKLEng,ACPU_Out,u32OutKeySize);
    }

    //Unlock LUT3 table and KL SRAM
    HAL_DSCMB_KL_TA_EN(eKLEng, FALSE);
    MDrv_DSCMB_KLadder_Unlock();
    return TRUE;


KL_Fail:


#if 0

    i = HAL_DSCMB_KLadder_Status();
    if ( i != 0 )
    {
        //printf("KL Satus = %x \n",i );
        *u32Status |= i ;
    }

    MDrv_DSCMB_KLadder_Stop();
#endif

    MDrv_DSCMB_KLadder_Unlock();
    DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "DSCMB KL Fail = %08lX \n",(unsigned long)*u32Status);

    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;

}
#endif

MS_BOOL MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_QUERYCAP pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_QUERYCAP));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId   = u32EngId;
    pArgs.eQueryType = eQueryType;
    pArgs.pInput     = pInput;
    pArgs.pOutput    = pOutput;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_QueryCap, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_GetCap(u32EngId, eQueryType, pInput, pOutput);
#endif
}

MS_BOOL MDrv_DSCMB2_GetLibVer(const MSIF_Version **ppVersion)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    const MSIF_Version** pArgs = ppVersion;

    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_GetLibVer , pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_GetLibVer(ppVersion);
#endif
}

//--------------------------------------------------------------------------------------------------
/// Set Dscmb driver debug level
/// @param u32Level \b IN: debug level
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB2_SetDBGLevel(MS_U32 u32Level)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    MS_U32 pArgs = u32Level;

    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_SetDBGLevel , &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_SetDBGLevel(u32Level);
#endif
}

MS_BOOL MDrv_DSCMB2_PVR_RecCtrl(MS_U32 u32EngId, MS_BOOL Enable)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_PVR_RECCTRL pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_PVR_RECCTRL));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }

    pArgs.u32EngId = u32EngId;
    pArgs.bEnable  = Enable;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_PVR_RecCtrl, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_PVR_RecCtrl(u32EngId, Enable);
#endif
}

MS_BOOL MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel)
{
#ifdef DSCMB_UTOPIA20
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
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_DSCMB2_PidFlt_ScmbStatus(u32EngId, u32PidFltId, pScmbLevel);
#endif
}

MS_BOOL MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_MULTI2_SETROUND pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_MULTI2_SETROUND));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId = u32EngId;
    pArgs.u32Round = u32Round;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Multi2_SetRound, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_Multi2_SetRound(u32EngId, u32Round);
#endif
}

MS_BOOL MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* pu8Syskey)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_MULTI2_SETSYSKEY pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_MULTI2_SETSYSKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId  = u32EngId;
    pArgs.pu8SysKey = pu8Syskey;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_Multi2_SetSysKey, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_Multi2_SetSystemKey(u32EngId, pu8Syskey);
#endif
}

MS_BOOL MDrv_DSCMB2_ES_SetStaticKey(MS_U32 u32EngId, MS_U8* pu8Statickey)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_ES_SETSTATICKEY pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_ES_SETSTATICKEY));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId     = u32EngId;
    pArgs.pu8StaticKey = pu8Statickey;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_ES_SetStaticKey, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_ES_SetStaticKey(u32EngId, pu8Statickey);
#endif
}

MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_HDCP2_SETRIV pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_HDCP2_SETRIV));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId = u32EngId;
    pArgs.pu8Riv = pu8RIV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_HDCP2_SetRiv, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_HDCP2_SetRIV(u32EngId, pu8RIV);
#endif

}

MS_BOOL MDrv_DSCMB2_HDCP2_SetRIV_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U8* pu8RIV)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_HDCP2_SETRIV_EX pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_HDCP2_SETRIV_EX));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.pu8Riv = pu8RIV;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_HDCP2_SetRiv_Ex, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DRVDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_HDCP2_SetRIV_Ex(u32EngId, u32DscmbId, pu8RIV);
#endif
}


//------------------------------------- ------------------//
//----------------------Debug API -------------------------//
//-------------------------------------------------------//
MS_BOOL MDrv_DSCMB2_GetConnectStatus(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32TspFltId)
{
#ifdef DSCMB_UTOPIA20
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DSCMB_CONNECTSTATUS pArgs;
    memset(&pArgs, 0, sizeof(DSCMB_CONNECTSTATUS));
    if(pu32DscmbInst == NULL)
    {
        return FALSE;
    }
    pArgs.u32EngId = u32EngId;
    pArgs.u32DscmbId = u32DscmbId;
    pArgs.u32DmxFltId = u32TspFltId;

    u32Ret = UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_GetConnectStatus, &pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        return FALSE;
    }

    return TRUE;

#else
    return _MDrv_DSCMB2_GetConnectStatus(u32EngId, u32DscmbId, u32TspFltId);
#endif
}

void MDrv_DSCMB2_ReadSwitch(MS_U32 u32FltId, DSCMB_Key_Type type, MS_U32 u32CAVid)
{
    _MDrv_DSCMB2_ReadSwitch(u32FltId, type, u32CAVid);
}

void MDrv_DSCMB2_ReadPidSlotMap(MS_U32 u32FltId)
{
    _MDrv_DSCMB2_ReadPidSlotMap(u32FltId);
}

void MDrv_DSCMB2_PacketView(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32 Mode, MS_U32 u32Range, MS_BOOL bRst)
{
    _MDrv_DSCMB2_PacketView(u32Eng, u32FltId, Mode, u32Range, bRst);
}

//------------------------------------- ------------------//
//------------------------Depreted API ---------------------//
//-------------------------------------------------------//
MS_BOOL MDrv_DSCMB_Init(void)
{
    return MDrv_DSCMB2_Init();
}

MS_BOOL MDrv_DSCMB_Exit(void)
{
    return MDrv_DSCMB2_Exit();
}

//--------------------------------------------------------------------------------------------------
/// Dscmb lock API for supporting multithreading
/// @param u
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Lock(void  )
{
    return MsOS_ObtainMutex(_s32KLMutexId, MSOS_WAIT_FOREVER);
}

//--------------------------------------------------------------------------------------------------
/// Dscmb unlock API for supporting multithreading
/// @param u
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Unlock(void)
{
    return MsOS_ReleaseMutex(_s32KLMutexId);
}

//--------------------------------------------------------------------------------------------------
/// Check if key ladder operation is complete
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_IsComplete(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Check if key ladder output is complete
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Output_IsComplete(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
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
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder stop output
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Output_Stop(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Reset key ladder
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Reset(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB_KLadder_ResetAcpuAck(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder start operating
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Start(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Make key ladder stop operating
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_Stop(void)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
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
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
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
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder configurations
/// @param pConfig \b IN: key ladder configurations
//--------------------------------------------------------------------------------------------------
void MDrv_DSCMB_KLadder_SetConfig(DSCMB_KLCfg *KLCfg)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return;
}

//--------------------------------------------------------------------------------------------------
/// Set key ladder destination
/// @param KLDst \b IN: key ladder key destination
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_KLadder_SetDst(DSCMB_KLDst KLDst)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB_KLadder_ReadDataFromAcpu(MS_U8* pu8Out)
{
    OBSOLETE_FUNCTION("Obsolete function\n");
    return FALSE;
}

MS_U32  MApi_DSCMB_SetPowerState(EN_POWER_MODE u16PowerState)
{
    return MDrv_DSCMB2_SetPowerState(u16PowerState);
}

MS_BOOL MDrv_DSCMB_SlotSwitchWrite(MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U32 u32SW0, MS_U32 u32SW1, MS_U32 u32SW2)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB_GetLibVer(const MSIF_Version **ppVersion)
{
    return MDrv_DSCMB2_GetLibVer (ppVersion);
}

MS_BOOL MDrv_DSCMB_SetDBGLevel(MS_U32 u32Level)
{
    return MDrv_DSCMB2_SetDBGLevel(u32Level);
}

MS_BOOL MDrv_DSCMB_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status)
{
    return MDrv_DSCMB2_KLadder_AtomicExec(KLCfg, ACPU_Out, u32Status);
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

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver status
/// @param pStatus \b OUT: Descrambler status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_GetStatus(DSCMB_Status *pStatus)
{
    OBSOLETE_FUNCTION("Not Implement\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Get Dscmb driver information
/// @return DSCMB_Info
//--------------------------------------------------------------------------------------------------
const DSCMB_Info*  MDrv_DSCMB_GetInfo(void)
{
    OBSOLETE_FUNCTION("Not Implement\n");
    return NULL;
}

//--------------------------------------------------------------------------------------------------
/// Set the PID to be replaced
/// @param u32PidPrim\b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID byu32Pid2ndary
/// @param u32Pid2ndary\b IN: The packet with this target PID(u32PidPrim) will be replaced in the field of PID byu32Pid2ndary
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// Reset the PID to be replaced
/// @param u32PidPrim\b IN: Reset the replacement of PID (u32PidPrim) set by MDrv_DSCMB_2ndarySet
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DSCMB_2ndaryReset(MS_U32 u32PidPrim)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_SetRecBuf(MS_U32 u32EngId , MS_U32 u32Start, MS_U32 u32Size)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_RecCtrl(MS_U32 u32EngId, MS_BOOL bEnable)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_GetRecWptr(MS_U32 u32EngId, MS_U32* pu32Write)
{
    OBSOLETE_FUNCTION("Not support\n");
    return FALSE;
}

MS_BOOL MDrv_DSCMB2_EngEnableKey(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Eng_Type eEngType, DSCMB_Key_Type eKeyType, MS_BOOL bEnable)
{
    return _MDrv_DSCMB2_EngEnableKey(u32EngId, u32DscmbId, eEngType, eKeyType, bEnable);
}


MS_BOOL MDrv_DSCMB2_FltSwitchType(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Flt_Type eFltType)
{
    return _MDrv_DSCMB2_FltSwitchType(u32EngId, u32DscmbId, eFltType);
}

MS_U32  MDrv_DSCMB2_GetPidSlotMapIndex(MS_U32 u32EngId, MS_U32 u32DscmbId)
{
    return _MDrv_DSCMB2_GetPidSlotMapIndex(u32EngId, u32DscmbId);
}

