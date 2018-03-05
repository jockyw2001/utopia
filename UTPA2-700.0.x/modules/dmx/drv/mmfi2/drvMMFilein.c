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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010-2012 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMMFilein.c
/// @brief  Multimedia File In (MMFILEIN 2) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All MMFILEIN DDI are not allowed to use in any interrupt context other than MMFILEIN ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsVersion.h"
#include "MsCommon.h"
#include "drvMMIO.h"

// Internal Definition
#include "asmCPU.h"
#include "halMMFilein.h"
#include "drvMMFilein.h"

#ifdef MMFI_HW_BOX_SERIES
    #include "regTSP.h"
    #include "halTSP.h"
#endif

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MMFI_MUTEX_WAIT_TIME                            0xFFFFFFFFUL

#define MMFI_DBGERR(x)                                  {if(_dbglevel >= E_DRVMMFI_DBG_ERR) x;  }
#define MMFI_DBGINFO(x)                                 {if(_dbglevel >= E_DRVMMFI_DBG_INFO) x;  }

#define MMFI_ASSERT(_bool)                              if(!(_bool))   \
                                                        {               \
                                                            if(_dbglevel >= E_DRVMMFI_DBG_INFO)    \
                                                              {  printf("ASSERT ERR\n");  }           \
                                                            return DRVMMFI_FAIL;                    \
                                                        }

#define TSP_RES_ALLOC_MMFIPATH_CHECK(PhyMmfiPath)       {                                                                                                                       \
                                                            if((_pmmfi_res->_bIsMmfiPathEverAlloc == TRUE) && (_pmmfi_res->_stMmfiPathState[PhyMmfiPath].bUsed == FALSE))       \
                                                            {                                                                                                                   \
                                                                printf("[%s][%d]: Mix old & new TSP Path resource allocation scheme !!\n", __FUNCTION__, __LINE__);             \
                                                            }                                                                                                                   \
                                                        }

#define _MMFI_ENTRY()                                   if (!MsOS_ObtainMutex(_pmmfi_res->_s32MMFIMutexId, MMFI_MUTEX_WAIT_TIME))      \
                                                        {               \
                                                            return DRVMMFI_FAIL;                                           \
                                                        }

#define _MMFI_RETURN(_ret)                              do{                                                                \
                                                            MsOS_ReleaseMutex(_pmmfi_res->_s32MMFIMutexId);         \
                                                            return _ret;                                                   \
                                                        }while(0);

#define _MMFI_GET_ENGID(x)                              (x <  (MS_U8)MMFI_PIDFLT0_NUM ? 0 : 1);
#define _MMFI_GET_FLTID(x)                              (x %  (MS_U8)MMFI_PIDFLT0_NUM);
#define _MMFI_GET_FLTLISTID(e,f)                        (e == 0 ? f : ((MS_U8)MMFI_PIDFLT0_NUM+f))
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _DrvMMFI_State
{
    DrvMMFI_FltType   FltType[MMFI_PIDFLT0_NUM];
}DRVMMFI_State;

typedef struct
{
    MS_BOOL     bUsed;
} DrvMMFI_Path_State;

typedef struct _MMFI_RESOURCE_PRIVATE
{
    MS_S32                       _s32MMFIMutexId;
    DRVMMFI_State                _MMFIState[MMFI_ENGINE_NUM];

    DrvMMFI_Path_State           _stMmfiPathState[MMFI_ENGINE_NUM];
    MS_BOOL                      _bIsMmfiPathEverAlloc;

    MS_BOOL                      _bSWInit;
}MMFI_RESOURCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifndef MMFI_UTOPIA_20
static MMFI_RESOURCE_PRIVATE _mmfi_res = {
    ._s32MMFIMutexId                        = -1,
    ._MMFIState                             = {
        {.FltType                            = {E_DRVMMFI_FLTTYPE_NONE},}
    },

    ._stMmfiPathState                       = {[0 ... (MMFI_ENGINE_NUM-1)] = {FALSE}},
    ._bIsMmfiPathEverAlloc                  = FALSE,

    ._bSWInit                               = FALSE,
};

static MMFI_RESOURCE_PRIVATE*    _pmmfi_res = &_mmfi_res;
#else
static MMFI_RESOURCE_PRIVATE*    _pmmfi_res = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
DrvMMFI_DbgLevel                    _dbglevel = E_DRVMMFI_DBG_Release;
static MS_BOOL                      _bBankInit = FALSE;
static MS_U16                       _u16MMFIMobfKey[MMFI_ENGINE_NUM] = {[0 ... (MMFI_ENGINE_NUM-1)] = 0};


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static MS_BOOL _MMFI_InitResource(MMFI_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii, u32jj;

    if(presource == NULL)
        return FALSE;

    presource->_s32MMFIMutexId = -1;

    for(u32jj = 0; u32jj < MMFI_ENGINE_NUM; u32jj++)
    {
        for(u32ii = 0; u32ii < MMFI_PIDFLT0_NUM; u32ii++)
        {

            presource->_MMFIState[u32jj].FltType[u32ii] = E_DRVMMFI_FLTTYPE_NONE;
        }
    }

    for(u32ii = 0; u32ii < MMFI_ENGINE_NUM; u32ii++)
    {
        presource->_stMmfiPathState[u32ii].bUsed = FALSE;
    }

    presource->_bIsMmfiPathEverAlloc = FALSE;

    presource->_bSWInit = FALSE;

    return TRUE;
}

static MMFI_Result _MMFI_Path_Alloc(MS_U32 *pu32PathId)
{
    DrvMMFI_Path  eMmfiPath = E_DRVMMFI_PATH0;

    *pu32PathId = (MS_U32)-1;
    for(eMmfiPath = 0; eMmfiPath < MMFI_ENGINE_NUM; eMmfiPath++)
    {
        if(_pmmfi_res->_stMmfiPathState[eMmfiPath].bUsed == FALSE)
        {
            _pmmfi_res->_stMmfiPathState[eMmfiPath].bUsed = TRUE;
            *pu32PathId = (MS_U32)eMmfiPath;
            return DRVMMFI_OK;
        }
    }

    return DRVMMFI_FAIL;
}

static MMFI_Result _MMFI_Path_Free(DrvMMFI_Path epath)
{
    if(_pmmfi_res->_stMmfiPathState[epath].bUsed == FALSE)
        return DRVMMFI_FAIL;

    _pmmfi_res->_stMmfiPathState[epath].bUsed = FALSE;
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_InitLibResource(void* pResMemAddr)
{
    MS_VIRT virtBank;
    MS_PHY u32BankSize;
    MMFI_ASSERT(pResMemAddr != 0);

    _pmmfi_res = (MMFI_RESOURCE_PRIVATE*)pResMemAddr;

    if(_pmmfi_res->_bSWInit != TRUE)
    {
        if(_MMFI_InitResource(_pmmfi_res) == FALSE)
            return DRVMMFI_FAIL;
    }

    // For multi-process use case. (different process should set the value of bank again)
    if(_bBankInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_MMFILEIN))
        {
            MMFI_DBGERR(printf("MDrv_MMIO_GetBASE failed\n"));
            return DRVMMFI_FAIL;
        }
        MMFI_DBGINFO((printf("MMFI Base Bank 0x%x\n", (unsigned int)virtBank)));
        HAL_MMFI_SetBank(virtBank);
        _bBankInit = TRUE;
    }

    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize MMFilein driver
/// @return MMFI_Result
/// @note
/// It should be called before calling any other MMFilein DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Init(void)
{
    MS_VIRT virtBank;
    MS_PHY  u32BankSize;
    MS_U8   u8ii, u8jj;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_MMFILEIN))
    {
        MMFI_DBGERR(printf("MDrv_MMIO_GetBASE failed\n"));
        return DRVMMFI_FAIL;
    }
    MMFI_DBGINFO((printf("MMFI Base Bank 0x%x\n", (unsigned int)virtBank)));
    HAL_MMFI_SetBank(virtBank);

    if(_pmmfi_res->_bSWInit)
        return DRVMMFI_OK;

    _pmmfi_res->_s32MMFIMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MMFI", MSOS_PROCESS_SHARED);
    if(_pmmfi_res->_s32MMFIMutexId == -1)
    {
        MMFI_DBGERR(printf("MsOS_CreateMutex failed\n"));
        return DRVMMFI_FAIL;
    }

    _MMFI_ENTRY();

    HAL_MMFI_Reset_All();

    for(u8jj = 0; u8jj < (MS_U8)MMFI_ENGINE_NUM; u8jj++)
    {
        for(u8ii = 0; u8ii < (MS_U8)MMFI_PIDFLT0_NUM; u8ii++)
        {
            _pmmfi_res->_MMFIState[u8jj].FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;
            HAL_MMFI_PidFlt_Reset(u8jj, u8ii);
        }
        HAL_MMFI_Set_FileinTimer(u8jj, 0x0A);
        HAL_MMFI_Cfg_Enable(u8ii, MMFI_CFG_ALT_TS_SIZE, TRUE);
    }

    HAL_MMFI_Cfg_Enable(0, MMFI_CFG_RADDR_READ, TRUE);
    HAL_MMFI_Cfg_Enable(1, MMFI_CFG_RADDR_READ, TRUE);

    _pmmfi_res->_bSWInit = TRUE;

    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Close MMFilein
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Exit(void)
{
    MS_U8 u8ii, u8jj;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    if(_pmmfi_res->_bSWInit == FALSE)
        return DRVMMFI_OK;

    _MMFI_ENTRY();

    for(u8jj = 0; u8jj < (MS_U8)MMFI_ENGINE_NUM; u8jj++)
    {
        for(u8ii = 0; u8ii < (MS_U8)MMFI_PIDFLT0_NUM; u8ii++)
        {
            _pmmfi_res->_MMFIState[u8jj].FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;
            HAL_MMFI_PidFlt_Reset(u8jj, u8ii);
        }
    }

    MsOS_ReleaseMutex(_pmmfi_res->_s32MMFIMutexId);
    MsOS_DeleteMutex(_pmmfi_res->_s32MMFIMutexId);
    _pmmfi_res->_s32MMFIMutexId = -1;

    _pmmfi_res->_bSWInit = FALSE;

    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get MM File-in HW interrupt status
/// @param  pu32status                        \b OUT: pointer of storing HW interrupt status value
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Get_HWIntStatus(MS_U32* pu32status)
{
    *pu32status = HAL_MMFI_HWInt_Status();
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set debug level of MMFilein
/// @param  DbgLevel                     \b IN: Debug Level Value
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_SetDbgLevel(DrvMMFI_DbgLevel DbgLevel)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _dbglevel = DbgLevel;
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set Audio path to be MMFI
/// @param  bEnable             \b IN: Enable Flag
/// @return MMFI_Result
/// @note
///   Not support in MMFI2
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_AudPath_Enable(MS_BOOL bEnable)
{
    return DRVMMFI_NOTSUPPORT;
}

//-------------------------------------------------------------------------------------------------
/// Set MM File-in Timer
/// @param  epath                         \b IN: MM Filein Path
/// @param  u8timer                       \b IN: Filein Timer
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetTimer(DrvMMFI_Path epath, MS_U8 u8timer)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Set_FileinTimer((MS_U8)epath, u8timer);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable File-in Byte Timer
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_ByteTimer_En(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_BYTETIMER_EN, TRUE);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate MMFI Path Id
/// @param  pu32PathId                  \b OUT: pointer of Path ID for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Path_Alloc(MS_U32 *pu32PathId)
{
    MMFI_Result eRet = DRVMMFI_FAIL;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    eRet = _MMFI_Path_Alloc(pu32PathId);

    if(eRet == DRVMMFI_OK)
    {
        _pmmfi_res->_bIsMmfiPathEverAlloc = TRUE;
    }

    _MMFI_RETURN(eRet);
}

//-------------------------------------------------------------------------------------------------
/// Free MMFI Path Id
/// @param  ePathId                     \b IN: Path ID
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Path_Free(DrvMMFI_Path epath)
{
    MMFI_Result eRet = DRVMMFI_FAIL;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    eRet = _MMFI_Path_Free(epath);

    _MMFI_RETURN(eRet);
}

//-------------------------------------------------------------------------------------------------
/// Allocate MMFI PID filter
/// @param  epath                                \b IN: MM Filein Path
/// @param  DrvMMFI_FltType               \b IN: PID filter type
/// @param  pu8PidFltId                        \b OUT: pointer of PID filter ID for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_PidFlt_Alloc(DrvMMFI_Path epath, DrvMMFI_FltType eFilterType, MS_U8 *pu8PidFltId)
{
    MS_U8 u8ii, u8jj;
    *pu8PidFltId = 0xFF;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT((eFilterType > E_DRVMMFI_FLTTYPE_NONE) && (eFilterType < E_DRVMMFI_FLTTYPE_MAX));
    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    u8jj = (MS_U8)epath;
    for(u8ii = 0; u8ii < (MS_U8)MMFI_PIDFLT0_NUM; u8ii++)
    {
        if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_NONE)
        {
            MMFI_DBGINFO(printf("Allocate PID Filter %d\n", (int)u8ii));
            _pmmfi_res->_MMFIState[u8jj].FltType[u8ii] = eFilterType;
            *pu8PidFltId = _MMFI_GET_FLTLISTID(u8jj, u8ii);
            _MMFI_RETURN(DRVMMFI_OK);
        }
    }

    _MMFI_RETURN(DRVMMFI_FAIL);

}

//-------------------------------------------------------------------------------------------------
/// Free MMFI PID filter
/// @param u8PidFltId                     \b IN: PID filter ID
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_PidFlt_Free(MS_U8 u8PidFltId)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT(u8PidFltId < MMFI_PIDFLT_NUM_ALL);

    MS_U8 u8jj = _MMFI_GET_ENGID(u8PidFltId);
    MS_U8 u8ii = _MMFI_GET_FLTID(u8PidFltId);

    TSP_RES_ALLOC_MMFIPATH_CHECK(u8jj);

    if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_NONE)
        return DRVMMFI_OK;

    _MMFI_ENTRY();

    if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] < E_DRVMMFI_FLTTYPE_MAX)
    {
        _pmmfi_res->_MMFIState[u8jj].FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;
        HAL_MMFI_PidFlt_Reset(u8ii, u8jj);
        _MMFI_RETURN(DRVMMFI_OK);
    }

    _MMFI_RETURN(DRVMMFI_INVALID_PARAM);
}

//-------------------------------------------------------------------------------------------------
/// Enable PID filter
/// @param  u8PidFltId                       \b IN: PID filter ID
/// @param  bEnable                          \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_PidFlt_Enable(MS_U8 u8PidFltId, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT(u8PidFltId < MMFI_PIDFLT_NUM_ALL);

    MS_U32 u32type = 0;
    MS_U8 u8jj = _MMFI_GET_ENGID(u8PidFltId);
    MS_U8 u8ii = _MMFI_GET_FLTID(u8PidFltId);

    MMFI_ASSERT(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] > E_DRVMMFI_FLTTYPE_NONE);

    TSP_RES_ALLOC_MMFIPATH_CHECK(u8jj);

    if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUD)
        u32type = MMFI_PIDFLT_AFIFO_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDB)
        u32type = MMFI_PIDFLT_AFIFOB_EN;
#ifdef MMFI2_EXTRA_AUFIFO
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDC)
        u32type = MMFI_PIDFLT_AFIFOC_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDD)
        u32type = MMFI_PIDFLT_AFIFOD_EN;
#endif
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD3D)
        u32type = MMFI_PIDFLT_V3DFIFO_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD)
        u32type = MMFI_PIDFLT_VFIFO_EN;
#ifdef MMFI_HW_BOX_SERIES
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDE)
        u32type = MMFI_PIDFLT_AFIFOE_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDF)
        u32type = MMFI_PIDFLT_AFIFOF_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD3)
        u32type = MMFI_PIDFLT_VFIFO3_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD4)
        u32type = MMFI_PIDFLT_VFIFO4_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD5)
        u32type = MMFI_PIDFLT_VFIFO5_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD6)
        u32type = MMFI_PIDFLT_VFIFO6_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD7)
        u32type = MMFI_PIDFLT_VFIFO7_EN;
    else if(_pmmfi_res->_MMFIState[u8jj].FltType[u8ii] == E_DRVMMFI_FLTTYPE_VD8)
        u32type = MMFI_PIDFLT_VFIFO8_EN;
#endif

    _MMFI_ENTRY();
    HAL_MMFI_PidFlt_Enable(u8jj, u8ii, u32type, bEnable);
    _MMFI_RETURN(DRVMMFI_INVALID_PARAM);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to PID filter
/// @param  u8PidFltId                       \b IN: PID filter ID
/// @param  u16Pid                           \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_PidFlt_SetPid(MS_U8 u8PidFltId, MS_U16 u16Pid)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT(u8PidFltId < MMFI_PIDFLT_NUM_ALL);

    MS_U8 u8jj = _MMFI_GET_ENGID(u8PidFltId);
    MS_U8 u8ii = _MMFI_GET_FLTID(u8PidFltId);

    TSP_RES_ALLOC_MMFIPATH_CHECK(u8jj);

    _MMFI_ENTRY();
    HAL_MMFI_PidFlt_SetPid(u8jj, u8ii, u16Pid);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Command Queue Reset
/// @param  epath                         \b IN: MM Filein Path
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_CmdQ_Reset(DrvMMFI_Path epath)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    if(epath == E_DRVMMFI_PATH0)
        HAL_MMFI_Reset_SubItem(MMFI_RST_CMDQ0);
    else if(epath == E_DRVMMFI_PATH1)
        HAL_MMFI_Reset_SubItem(MMFI_RST_CMDQ1);
    else
        _MMFI_RETURN(DRVMMFI_INVALID_PARAM);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Write Level value
/// @param  epath                                \b IN: MM Filein Path
/// @param  pu8FifoLevel                       \b OUT: pointer of Command Queue Write Level value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_CmdQFifo_Status(DrvMMFI_Path epath, MS_U8 *pu8FifoLevel)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    *pu8FifoLevel = (MS_U8)HAL_MMFI_CmdQ_FIFO_Get_WRLevel((MS_U8)epath);
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Empty Slot value
/// @param  epath                                   \b IN: MM Filein Path
/// @param  pu32EmptySlot                       \b OUT: pointer of Command Queue Empty Slot value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_CmdQ_GetSlot(DrvMMFI_Path epath, MS_U32 *pu32EmptySlot)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    *pu32EmptySlot = 0;
    *pu32EmptySlot = MMFI_CMDQ_SIZE - ((MS_U32)HAL_MMFI_CmdQ_FIFO_Get_WRCnt((MS_U8)epath) & 0xFFFFUL);
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set Playback timestamp
/// @param  epath                            \b IN: MM Filein Path
/// @param  u8PidFltId                       \b IN: LPCR32 value
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_SetPlaybackTimeStamp(DrvMMFI_Path epath, MS_U32 u32lpcr2)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_LPcr2_Set((MS_U8)epath, u32lpcr2);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get playback timestamp
/// @param  epath                            \b IN: MM Filein Path
/// @param pu32lpcr2                        \b OUT: pointer of LPCR32 value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_GetPlaybackTimeStamp(DrvMMFI_Path epath, MS_U32 *pu32lpcr2)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT((pu32lpcr2 != 0));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    *pu32lpcr2 = HAL_MMFI_LPcr2_Get((MS_U8)epath);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get File-in timestamp value
/// @param  epath                              \b IN: MM Filein Path
/// @param  pu32TSLen                       \b OUT: pointer of Timestamp value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_GetFileInTimeStamp(DrvMMFI_Path epath, MS_U32* pu32TSLen)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    *pu32TSLen = 0;
    *pu32TSLen = HAL_MMFI_TimeStamp_Get((MS_U8)epath);
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Start Address
/// @param  epath                                       \b IN: MM Filein Path
/// @param  phyStreamAddr                             \b IN: MIU Physical Address of File-in Read Start Address.
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetAddr(DrvMMFI_Path epath, MS_PHY phyStreamAddr)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
#ifdef MMFI_HW_BOX_SERIES
    HAL_TSP_FQ_MMFI_MIU_Sel(E_TSP_HAL_MIU_SEL_MMFI, (MS_U8)epath, phyStreamAddr);
#endif
    HAL_MMFI_Set_Filein_ReadAddr((MS_U8)epath, phyStreamAddr);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Size
/// @param  epath                                   \b IN: MM Filein Path
/// @param  u32StreamSize                       \b IN: File-in Read Size.
/// @return MMFI_Result
//   @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetSize(DrvMMFI_Path epath, MS_U32 u32StreamSize)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    if(u32StreamSize <= 16UL) //HW limitation
    {
        printf("[%s] MMFI size must large 16 bytes\n", __FUNCTION__);
        return DRVMMFI_FAIL;
    }

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Set_Filein_ReadLen((MS_U8)epath, u32StreamSize);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set packet size to MMFilein file
/// @param  epath                         \b IN: MM Filein Path
/// @param  PacketMode                 \b IN: Mode of MMFilein packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetPacketMode(DrvMMFI_Path epath, DrvMMFI_PacketMode PacketMode)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    switch (PacketMode)
    {
        case E_DRVMMFI_PKTMODE_188:
            HAL_MMFI_PktChkSize_Set((MS_U8)epath, 0xBB);
            HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_ALT_TS_SIZE, TRUE);
            break;
        case E_DRVMMFI_PKTMODE_192:
            HAL_MMFI_PktChkSize_Set((MS_U8)epath, 0xBF);
            HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_ALT_TS_SIZE, TRUE);
            break;
        case E_DRVMMFI_PKTMODE_204:
            HAL_MMFI_PktChkSize_Set((MS_U8)epath, 0xCB);
            HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_ALT_TS_SIZE, TRUE);
            break;
        default:
            break;
    }
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in time stamp clock source
/// @param  epath                         \b IN: MM Filein Path
/// @param  u32Clksrc                     \b IN: clock source
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetTimeStampClk(DrvMMFI_Path ePath, MS_U32 u32ClkSrc)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(ePath);

    HAL_MMFI_TimeStampClk_Set((MS_U8)ePath, u32ClkSrc);
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get File-in HW read address
/// @param  epath                                    \b IN: MM Filein Path
/// @param  pphyReadAddr                           \b OUT: pointer to store read address of file
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_GetReadAddr(DrvMMFI_Path epath, MS_PHY* pphyReadAddr)
{
    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    *pphyReadAddr = HAL_MMFI_Get_Filein_WriteAddr((MS_U8)epath);

    return DRVMMFI_OK;
}
//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Mode
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_192Mode_En(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_PKT192_EN, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);

}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Block Scheme
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_192BlockScheme_En(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_PKT192_BLK_DISABLE, !bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Drop duplicate packets
/// @param  epath                          \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_DupPktSkip(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_DUP_PKT_SKIP, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Skip TEL packet
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_TEI_SkipPktf(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_TEI_SKIP, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Clear PID filter byte counter
/// @param  epath                         \b IN: MM Filein Path
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Clr_Pidflt_ByteCnt(DrvMMFI_Path epath)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_CLR_PIDFLT_BYTE_CNT, TRUE);
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_CLR_PIDFLT_BYTE_CNT, FALSE);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable TS Data Endian Swap
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_TSDataEndian_Swap(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_MEM_TS_DATA_ENDIAN, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable TS Order Swap
/// @param  epath                         \b IN: MM Filein Path
/// @param  bEnable                       \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_TSOrder_Swap(DrvMMFI_Path epath, MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_MEM_TS_ORDER, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

MMFI_Result MDrv_MMFI_GetCap(DrvMMFI_Cap eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case E_DRVMMFI_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = (MS_U32)sizeof(MMFI_RESOURCE_PRIVATE);
            return DRVMMFI_OK;
        case E_DRVMMFI_CAP_FILTER0_NUM:
            *((MS_U32*)pOutput) = MMFI_PIDFLT0_NUM;
            return DRVMMFI_OK;
        case E_DRVMMFI_CAP_FILTER1_NUM:
            *((MS_U32*)pOutput) = MMFI_PIDFLT1_NUM;
            return DRVMMFI_OK;
        case E_DRVMMFI_CAP_FILTER_ALL_NUM:
            *((MS_U32*)pOutput) = MMFI_PIDFLT_NUM_ALL;
            return DRVMMFI_OK;
        default:
            return DRVMMFI_INVALID_PARAM;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue state
/// @param  epath                                 \b IN: MM Filein Path
/// @param  peFileinState                       \b OUT: Pointer of Filein state to return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_GetState(DrvMMFI_Path epath, DrvMMFI_FileinState *peFileinState)
{
    MS_U16 u16state = 0xFF, u16cmdcnt = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();

    u16state = HAL_MMFI_Get_Filein_Ctrl((MS_U8)epath);
    u16cmdcnt = HAL_MMFI_CmdQ_FIFO_Get_WRCnt((MS_U8)epath);

    if((u16state & MMFI_FILEIN_CTRL_START) || u16cmdcnt)
        *peFileinState = E_DRVMMFI_STATE_BUSY;
    else
        *peFileinState = E_DRVMMFI_STATE_IDLE;

    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue interrupt state
/// @param  peFileinState                       \b OUT: Pointer of Filein state to return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_ChkStatus(DrvMMFI_Status* pStatus)
{
    MS_U16 u32temp = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    u32temp = HAL_MMFI_HWInt_Status();

    *pStatus = E_DRVMMFI_STATUS_UNKNOWN;
    if(u32temp & MMFI_HWINT_STS_FILEIN_DONE0)
        *pStatus |= E_DRVMMFI_STATUS_FILEINDONE0;
    if(u32temp & MMFI_HWINT_STS_AUAUB_ERR0)
        *pStatus |= E_DRVMMFI_STATUS_AU_PKTERR0;
    if(u32temp & MMFI_HWINT_STS_VD3D_ERR0)
        *pStatus |= E_DRVMMFI_STATUS_VD_PKTERR0;
    if(u32temp & MMFI_HWINT_STS_FILEIN_DONE1)
        *pStatus |= E_DRVMMFI_STATUS_FILEINDONE1;
    if(u32temp & MMFI_HWINT_STS_AUAUB_ERR1)
        *pStatus |= E_DRVMMFI_STATUS_AU_PKTERR1;
    if(u32temp & MMFI_HWINT_STS_VD3D_ERR1)
        *pStatus |= E_DRVMMFI_STATUS_VD_PKTERR1;
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Clear Command queue interrupt state
/// @param  u32int                       \b OUT: Interrupt bit to clear
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_ClearHWInt(MS_U32 u32int)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();
    HAL_MMFI_HWInt_Clear((MS_U16)u32int);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get TS Header information
/// @param  epath                          \b IN: MM Filein Path
/// @param  pheader                       \b OUT: Poniter to store TS Header information
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Get_TsHeader(DrvMMFI_Path epath, DrvPIDFLT_HEADER *pheader)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    HAL_MMFI_Get_TsHeaderInfo((MS_U8)epath, (MS_U32*)pheader);
    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Start
/// @param  epath                          \b IN: MM Filein Path
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_Start(DrvMMFI_Path epath)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();

    HAL_MMFI_Cfg_Enable((MS_U8)epath, MMFI_CFG_BYTETIMER_EN, TRUE);
    HAL_MMFI_MOBF_Set_FileinKey((MS_U8)epath, _u16MMFIMobfKey[epath]);
    HAL_MMFI_Set_Filein_Ctrl((MS_U8)epath, MMFI_FILEIN_CTRL_START, TRUE);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Abort
/// @param  epath                          \b IN: MM Filein Path
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_Abort(DrvMMFI_Path epath)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Set_Filein_Ctrl((MS_U8)epath, MMFI_FILEIN_CTRL_ABORT, TRUE);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MM Filein Path
/// @param  eFileinMode                       \b IN: File-in Mode
/// @param  bEnable                            \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_ModeEnable(DrvMMFI_FileinMode fileinmode, MS_BOOL bEnable)
{
#ifdef MMFI_HW_BOX_SERIES
    MS_U16 u16cfg3 = 0;
#endif
#ifdef MMFI2_EXTRA_AUFIFO
    MS_U16 u16cfg2 = 0;
#endif
    MS_U32 u32cfg  = 0UL;
    MS_U8  u8EngID = ((fileinmode & E_DRVMMFI_MODE_MMFI1_MASK) ? 1: 0);

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(u8EngID);

    _MMFI_ENTRY();

    fileinmode &= (~E_DRVMMFI_MODE_MMFI1_MASK);

    if(bEnable == FALSE)
    {
        HAL_MMFI_Cfg_Set(u8EngID, (HAL_MMFI_Cfg_Get(u8EngID) & ~MMFI_CFG_FILEIN_MODE_MASK));

#ifdef MMFI2_EXTRA_AUFIFO
        HAL_MMFI_Cfg2_Set(u8EngID, (HAL_MMFI_Cfg2_Get(u8EngID) & ~MMFI_CFG2_FILEIN_MODE_MASK));
#endif
    }
    else
    {
        switch (fileinmode)
        {
            case E_DRVMMFI_MODE_APID_BYPASS:
                u32cfg = MMFI_CFG_APID_BYPASS;
                break;
            case E_DRVMMFI_MODE_APIDB_BYPASS:
                u32cfg = MMFI_CFG_APIDB_BYPASS;
                break;
            case E_DRVMMFI_MODE_VDPID_BYPASS:
                u32cfg = MMFI_CFG_VPID_BYPASS;
                break;
            case E_DRVMMFI_MODE_V3DPID_BYPASS:
                u32cfg = MMFI_CFG_VPID3D_BYPASS;
                break;
            case E_DRVMMFI_MODE_PS_AU:
                u32cfg = MMFI_CFG_PS_AUD_EN;
                break;
            case E_DRVMMFI_MODE_PS_AUB:
                u32cfg = MMFI_CFG_PS_AUDB_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD:
                u32cfg = MMFI_CFG_PS_VD_EN;
                break;
            case E_DRVMMFI_MODE_PS_V3D:
                u32cfg = MMFI_CFG_PS_V3D_EN;
                break;
#ifdef MMFI2_EXTRA_AUFIFO
            case E_DRVMMFI_MODE_PS_AUC:
                u16cfg2 = MMFI_CFG2_MMFI_PS_AUDC_EN;
                break;
            case E_DRVMMFI_MODE_PS_AUD:
                u16cfg2 = MMFI_CFG2_MMFI_PS_AUDD_EN;
                break;
#endif
#ifdef MMFI_HW_BOX_SERIES
            case E_DRVMMFI_MODE_PS_AUE:
                u16cfg2 = MMFI_CFG2_MMFI_PS_AUDE_EN;
                break;
            case E_DRVMMFI_MODE_PS_AUF:
                u16cfg2 = MMFI_CFG2_MMFI_PS_AUDF_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD3:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD3_EN;
                u16cfg3 = MMFI_CFG3_MMFI_PS_VD3_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD4:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD4_EN;
                u16cfg3 = MMFI_CFG3_MMFI_PS_VD4_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD5:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD5_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD6:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD6_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD7:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD7_EN;
                break;
            case E_DRVMMFI_MODE_PS_VD8:
                u16cfg2 = MMFI_CFG2_MMFI_PS_VD8_EN;
                break;
#endif
            case E_DRVMMFI_MODE_NO_BYPASS_TS:
            case E_DRVMMFI_MODE_NO_BYPASS_AU:
            case E_DRVMMFI_MODE_NO_BYPASS_V3D:
            case E_DRVMMFI_MODE_NO_BYPASS_VD:
                break;
            default:
                MMFI_DBGERR(printf("MDrv_MMFI_File_ModeEnable, Bad fileinmode!\n"));
                _MMFI_RETURN(DRVMMFI_FAIL);
        }

    HAL_MMFI_Cfg_Set(u8EngID, (HAL_MMFI_Cfg_Get(u8EngID) & ~MMFI_CFG_FILEIN_MODE_MASK) | u32cfg);

#ifdef MMFI2_EXTRA_AUFIFO
    HAL_MMFI_Cfg2_Set(u8EngID, (HAL_MMFI_Cfg2_Get(u8EngID) & ~MMFI_CFG2_FILEIN_MODE_MASK) | u16cfg2);
#endif

#ifdef MMFI_HW_BOX_SERIES
    HAL_MMFI_Cfg3_Set(u8EngID, (HAL_MMFI_Cfg3_Get(u8EngID) & ~MMFI_CFG3_FILEIN_MODE_MASK) | u16cfg3);
#endif
    }
    _MMFI_RETURN(DRVMMFI_OK);
}

MMFI_Result MDrv_MMFI_ResetAll(void)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();
    HAL_MMFI_Reset_All();
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_RemoveDupAVPkt(MS_BOOL bEnable)
{
    _MMFI_ENTRY();
    HAL_MMFI_Cfg_Enable(0, MMFI_CFG_DUP_PKT_SKIP, bEnable);
    HAL_MMFI_Cfg_Enable(1, MMFI_CFG_DUP_PKT_SKIP, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Video 3D path to be MMFI
/// @param  bEnable             \b IN: Enable Flag
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_VD3DPath_Enable(MS_BOOL bEnable)
{
    return DRVMMFI_NOTSUPPORT;
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF filein key
/// @param  eFileinMode                        \b IN: File-in Mode
/// @param  u32Key                      \b IN: MOBF key value
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_Set_MOBF_FileinKey(DrvMMFI_Path epath, MS_U32 u32Key)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_MOBF_Set_FileinKey((MS_U8)epath, (MS_U16)u32Key);
    _u16MMFIMobfKey[epath] = (MS_U16)u32Key;
    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MOBF decrypt mode
/// @param  benable                \b IN: Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_MOBF_FileIn_Enable(DrvMMFI_Path epath, MS_BOOL benable)
{
    TSP_RES_ALLOC_MMFIPATH_CHECK(epath);

    _MMFI_ENTRY();
    HAL_MMFI_Set_Filein_Ctrl((MS_U8)epath, MMFI_FILEIN_CTRL_MOBF_EN, benable);
    _MMFI_RETURN(DRVMMFI_OK);
}

MMFI_Result MDrv_MMFI_MOBF_FileIn_SetLevel(DrvMMFI_Path epath, MS_U8 u8level)
{
    return DRVMMFI_NOTSUPPORT;
}

#ifdef MSOS_TYPE_LINUX_KERNEL

MMFI_Result MDrv_MMFI_Suspend(void)
{
    HAL_MMFI_SaveRegs();

    return DRVMMFI_OK;
}

MMFI_Result MDrv_MMFI_Resume(void)
{
    HAL_MMFI_Reset_All();

    HAL_MMFI_RestoreRegs();

    return DRVMMFI_OK;
}

#endif //MSOS_TYPE_LINUX_KERNEL


