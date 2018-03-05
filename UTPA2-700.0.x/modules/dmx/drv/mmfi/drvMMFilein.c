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
/// @brief  Multimedia File In (MMFILEIN) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All MMFILEIN DDI are not allowed to use in any interrupt context other than MMFILEIN ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"

// Internal Definition
#include "asmCPU.h"
#include "halMMFilein.h"
#include "drvMMFilein.h"

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


#define _MMFI_ENTRY()                                   if (!MsOS_ObtainMutex(_pmmfi_res->_s32MMFIMutexId, MMFI_MUTEX_WAIT_TIME))      \
                                                        {               \
                                                            return DRVMMFI_FAIL;                                           \
                                                        }

#define _MMFI_RETURN(_ret)                              do{                                                                \
                                                            MsOS_ReleaseMutex(_pmmfi_res->_s32MMFIMutexId);         \
                                                            return _ret;                                                   \
                                                        }while(0);
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _DrvMMFI_State
{
    DrvMMFI_FltType   FltType[MMFI_PIDFLT_NUM_ALL];
}DRVMMFI_State;


typedef struct _MMFI_RESOURCE_PRIVATE
{
    MS_S32                       _s32MMFIMutexId;
    DRVMMFI_State                _MMFIState;

    MS_BOOL                      _bSWInit;
}MMFI_RESOURCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

#ifndef MMFI_UTOPIA_20
static MMFI_RESOURCE_PRIVATE _mmfi_res = {
    ._s32MMFIMutexId                        = -1,
    ._MMFIState                             = {
        .FltType                            = {E_DRVMMFI_FLTTYPE_NONE},
    },
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
static MS_BOOL                   _bBankInit = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MMFI_InitResource(MMFI_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii;

    if(presource == NULL)
        return FALSE;

    presource->_s32MMFIMutexId = -1;

    for(u32ii = 0; u32ii < MMFI_PIDFLT_NUM_ALL; u32ii++)
    {
        presource->_MMFIState.FltType[u32ii] = E_DRVMMFI_FLTTYPE_NONE;
    }

    presource->_bSWInit = FALSE;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_InitLibResource(void* pResMemAddr)
{
    MMFI_ASSERT(pResMemAddr != 0);
    MS_VIRT virtBank;
    MS_PHY u32BankSize;

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
        MMFI_DBGINFO((printf("MMFI Base Bank 0x%08x\n", (int)virtBank)));
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
    MS_PHY u32BankSize;
    MS_U8 u8ii;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_MMFILEIN))
    {
        MMFI_DBGERR(printf("MDrv_MMIO_GetBASE failed\n"));
        return DRVMMFI_FAIL;
    }
    MMFI_DBGINFO((printf("MMFI Base Bank 0x%08x\n", (unsigned int)virtBank)));
    HAL_MMFI_SetBank(virtBank);

    if(_pmmfi_res->_bSWInit)
        return DRVMMFI_OK;

    _pmmfi_res->_s32MMFIMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_MMFI", MSOS_PROCESS_SHARED);
    if(_pmmfi_res->_s32MMFIMutexId == -1)
    {
        MMFI_DBGERR(printf("MsOS_CreateMutex failed\n"));
        return DRVMMFI_FAIL;
    }

    for(u8ii = 0; u8ii < (MS_U8)MMFI_PIDFLT_NUM_ALL; u8ii++)
        _pmmfi_res->_MMFIState.FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;

    _MMFI_ENTRY();
    HAL_MMFI_Reset_All();

    HAL_MMFI_AU_Cfg_Set(MMFI_WBDMA_ECO);

    for(u8ii = 0; u8ii < (MS_U8)MMFI_AUDPIDFLT_NUM; u8ii++)
    {
        HAL_MMFI_AudPidFlt_Reset(u8ii);
    }

    HAL_MMFI_AU_Set_FileinTimer(0x0A);

#ifdef MMFI_VD3D

    HAL_MMFI_V3D_Cfg_Set(MMFI_WBDMA_ECO);

    for(u8ii = 0; u8ii < (MS_U8)MMFI_V3DPIDFLT_NUM; u8ii++)
    {
        HAL_MMFI_VD3DPidFlt_Reset(u8ii);
    }
    HAL_MMFI_V3D_Set_FileinTimer(0x0A);
#endif //MMFI_VD3D

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
    MS_U8 u8ii;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    if(_pmmfi_res->_bSWInit == FALSE)
        return DRVMMFI_OK;

    for(u8ii = 0; u8ii < (MS_U8)MMFI_PIDFLT_NUM_ALL; u8ii++)
        _pmmfi_res->_MMFIState.FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;

    _MMFI_ENTRY();
    for(u8ii = 0; u8ii < (MS_U8)MMFI_AUDPIDFLT_NUM; u8ii++)
    {
        HAL_MMFI_AudPidFlt_Reset(u8ii);
        _pmmfi_res->_MMFIState.FltType[u8ii] = E_DRVMMFI_FLTTYPE_NONE;
    }

    HAL_MMFI_AU_Cfg_Enable(MMFI_USE_AUD_PATH, FALSE);

#ifdef MMFI_VD3D
    for(u8ii = 0; u8ii < (MS_U8)MMFI_V3DPIDFLT_NUM; u8ii++)
    {
        HAL_MMFI_VD3DPidFlt_Reset(u8ii);
    }
    HAL_MMFI_V3D_Cfg_Enable(MMFI_USE_VD3D_PATH, FALSE);
#endif //MMFI_VD3D

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
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_AudPath_Enable(MS_BOOL bEnable)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();
    HAL_MMFI_AU_Cfg_Enable((MMFI_USE_AUD_PATH|MMFI_USE_AUDB_PATH), bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Set_FileinTimer(u8timer);

#ifdef MMFI_VD3D
    if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Set_FileinTimer(u8timer);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_BYTE_TIMER_EN, bEnable);

#ifdef MMFI_VD3D
    if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_BYTE_TIMER_EN, bEnable);
#endif

    _MMFI_RETURN(DRVMMFI_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate MMFI PID filter
/// @param  DrvMMFI_FltType                \b IN: PID filter type
/// @param  pu8PidFltId                        \b OUT: pointer of PID filter ID for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_PidFlt_Alloc(DrvMMFI_FltType eFilterType, MS_U8 *pu8PidFltId)
{
    MS_U8 u8ii;
    *pu8PidFltId = 0xFF;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT(eFilterType > E_DRVMMFI_FLTTYPE_NONE);

    _MMFI_ENTRY();
    if(eFilterType <= E_DRVMMFI_FLTTYPE_AUDB)
    {
        for(u8ii = 0; u8ii < (MS_U8)MMFI_AUDPIDFLT_NUM; u8ii++)
        {
            if(_pmmfi_res->_MMFIState.FltType[u8ii] == E_DRVMMFI_FLTTYPE_NONE)
            {
                MMFI_DBGINFO(printf("Allocate PID Filter %d\n", (int)u8ii));
                *pu8PidFltId = u8ii;
                _pmmfi_res->_MMFIState.FltType[u8ii] = eFilterType;
                _MMFI_RETURN(DRVMMFI_OK);
            }
        }
    }
#ifdef MMFI_VD3D
    else if(eFilterType == E_DRVMMFI_FLTTYPE_VD3D)
    {
        for(u8ii = 0; u8ii < (MS_U8)MMFI_V3DPIDFLT_NUM; u8ii++)
        {
            if(_pmmfi_res->_MMFIState.FltType[u8ii + MMFI_AUDPIDFLT_NUM] == E_DRVMMFI_FLTTYPE_NONE)
            {
                *pu8PidFltId = u8ii + MMFI_AUDPIDFLT_NUM;
                _pmmfi_res->_MMFIState.FltType[u8ii + MMFI_AUDPIDFLT_NUM] = eFilterType;
                _MMFI_RETURN(DRVMMFI_OK);
            }
        }
    }
#endif

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

    if(_pmmfi_res->_MMFIState.FltType[u8PidFltId] == E_DRVMMFI_FLTTYPE_NONE)
        return DRVMMFI_OK;

    _MMFI_ENTRY();

    if(_pmmfi_res->_MMFIState.FltType[u8PidFltId] <= E_DRVMMFI_FLTTYPE_AUDB)
    {
        MS_U8 u8ii = 0;
        MS_U32 u32AUFlag = 0;

        HAL_MMFI_AudPidFlt_Reset(u8PidFltId);
        _pmmfi_res->_MMFIState.FltType[u8PidFltId] = E_DRVMMFI_FLTTYPE_NONE;

        //disable audio path
        u32AUFlag = HAL_MMFI_AU_Cfg_Get() & (~(MMFI_USE_AUD_PATH | MMFI_USE_AUDB_PATH));
        for(u8ii = 0; u8ii < MMFI_AUDPIDFLT_NUM; u8ii++)
        {
            if(_pmmfi_res->_MMFIState.FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUD)
                u32AUFlag |= MMFI_USE_AUD_PATH;
            else if(_pmmfi_res->_MMFIState.FltType[u8ii] == E_DRVMMFI_FLTTYPE_AUDB)
                u32AUFlag |= MMFI_USE_AUDB_PATH;
        }
        HAL_MMFI_AU_Cfg_Set(u32AUFlag);

        _MMFI_RETURN(DRVMMFI_OK);
    }

#ifdef MMFI_VD3D
    else if(_pmmfi_res->_MMFIState.FltType[u8PidFltId] == E_DRVMMFI_FLTTYPE_VD3D)
    {
        HAL_MMFI_VD3DPidFlt_Reset(u8PidFltId - E_DRVMMFI_FLTTYPE_AUDB);
        HAL_MMFI_V3D_Cfg_Enable(MMFI_USE_VD3D_PATH, FALSE);
        _pmmfi_res->_MMFIState.FltType[u8PidFltId] = E_DRVMMFI_FLTTYPE_NONE;
        _MMFI_RETURN(DRVMMFI_OK);
    }
#endif

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
    MS_U16 u16type = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));
    MMFI_ASSERT(_pmmfi_res->_MMFIState.FltType[u8PidFltId] > E_DRVMMFI_FLTTYPE_NONE);

    switch(_pmmfi_res->_MMFIState.FltType[u8PidFltId])
    {
        case E_DRVMMFI_FLTTYPE_AUD:
            u16type = MMFI_PIDFLT_AFIFO_EN;
            HAL_MMFI_AU_Cfg_Enable(MMFI_USE_AUD_PATH, bEnable);
            break;
        case E_DRVMMFI_FLTTYPE_AUDB:
            #ifdef MMFI_PIDFLT_AFIFOB_EN                      
            u16type = MMFI_PIDFLT_AFIFOB_EN;
            HAL_MMFI_AU_Cfg_Enable(MMFI_USE_AUDB_PATH, bEnable);
            break;
            #else
            return DRVMMFI_INVALID_PARAM;
            #endif

        #ifdef MMFI_VD3D
        case E_DRVMMFI_FLTTYPE_VD3D:
            u16type = MMFI_PIDFLT_VD3D_EN;
            HAL_MMFI_V3D_Cfg_Enable(MMFI_USE_VD3D_PATH, bEnable);
            break;
        #endif
        
        default:
            return DRVMMFI_INVALID_PARAM;
    }

    _MMFI_ENTRY();
    
    if(_pmmfi_res->_MMFIState.FltType[u8PidFltId] <= E_DRVMMFI_FLTTYPE_AUDB)
    {
        HAL_MMFI_AudPidFlt_Enable(u8PidFltId, u16type, bEnable);
        _MMFI_RETURN(DRVMMFI_OK);
    }    
    #ifdef MMFI_VD3D
    else if(_pmmfi_res->_MMFIState.FltType[u8PidFltId] == E_DRVMMFI_FLTTYPE_VD3D)
    {
        HAL_MMFI_VD3DPidFlt_Enable(u8PidFltId - MMFI_AUDPIDFLT_NUM, u16type, bEnable);
        _MMFI_RETURN(DRVMMFI_OK);
    }
    #endif
    
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

    _MMFI_ENTRY();

    if(u8PidFltId < MMFI_AUDPIDFLT_NUM)
    {
        HAL_MMFI_AudPidFlt_SetPid(u8PidFltId, u16Pid);
        _MMFI_RETURN(DRVMMFI_OK);
    }
#ifdef MMFI_VD3D
    else if(u8PidFltId < MMFI_PIDFLT_NUM_ALL)
    {
        HAL_MMFI_VD3DPidFlt_SetPid(u8PidFltId, u16Pid);
        _MMFI_RETURN(DRVMMFI_OK);
    }
#endif

    _MMFI_RETURN(DRVMMFI_INVALID_PARAM);
}

//-------------------------------------------------------------------------------------------------
/// Command Queue Reset
/// @param  epath                         \b IN: MM Filein Path
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_CmdQ_Reset(DrvMMFI_Path epath)
{
    MS_U16 u32value = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        u32value |= MMFI_RST_CMDQ_AU;

#ifdef MMFI_VD3D
    if(epath == E_DRVMMFI_PATH_VD3D)
        u32value |= MMFI_RST_CMDQ_VD;
#endif

    HAL_MMFI_Reset_SubItem(u32value);

    //check R done
    #ifdef MMFI_FILEIN_DONE
    if(HAL_MMFI_Chk_CmdQResetDone(u32value) == FALSE)
    {
        _MMFI_RETURN(DRVMMFI_FAIL);
    }
    #endif

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

    *pu8FifoLevel = 0xFFUL;

    if(epath == E_DRVMMFI_PTH_AUDIO)
        *pu8FifoLevel = HAL_MMFI_AU_CmdQ_FIFO_Get_WRLevel();

#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        *pu8FifoLevel = HAL_MMFI_V3D_CmdQ_FIFO_Get_WRLevel();
#endif

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

    *pu32EmptySlot = 0;

    if(epath == E_DRVMMFI_PTH_AUDIO)
        *pu32EmptySlot = MMFI_CMDQ_SIZE - (MS_U32)HAL_MMFI_AU_CmdQ_FIFO_Get_WRCnt();
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        *pu32EmptySlot = MMFI_CMDQ_SIZE - (MS_U32)HAL_MMFI_V3D_CmdQ_FIFO_Get_WRCnt();
#endif

    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set playback timestamp
/// @param  epath                            \b IN: MM Filein Path
/// @param  u8PidFltId                       \b IN: LPCR32 value
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_SetPlaybackTimeStamp(DrvMMFI_Path epath, MS_U32 u32lpcr2)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_LPcr2_Set(u32lpcr2);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_LPcr2_Set(u32lpcr2);
#endif

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

    _MMFI_ENTRY();

    *pu32lpcr2 = 0;

    if(epath == E_DRVMMFI_PTH_AUDIO)
        *pu32lpcr2 = HAL_MMFI_AU_LPcr2_Get();
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        *pu32lpcr2 = HAL_MMFI_V3D_LPcr2_Get();
#endif

    _MMFI_RETURN(DRVMMFI_OK);

    return DRVMMFI_OK;
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

    *pu32TSLen = 0;

    if(epath == E_DRVMMFI_PTH_AUDIO)
        *pu32TSLen = HAL_MMFI_AU_TimeStamp_Get();
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        *pu32TSLen = HAL_MMFI_V3D_TimeStamp_Get();
#endif

    return DRVMMFI_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Start Address
/// @param  epath                                   \b IN: MM Filein Path
/// @param  u32StreamAddr                       \b IN: MIU Physical Address of File-in Read Start Address.
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_File_SetAddr(DrvMMFI_Path epath, MS_PHY phyStreamAddr)
{
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Set_Filein_ReadAddr(phyStreamAddr);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Set_Filein_ReadAddr(phyStreamAddr);
#endif

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
        return DRVMMFI_FAIL;

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Set_Filein_ReadLen(u32StreamSize);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Set_Filein_ReadLen(u32StreamSize);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
    {
        switch (PacketMode)
        {
            case E_DRVMMFI_PKTMODE_188:
                HAL_MMFI_AU_PktChkSize_Set(0xBB);
                HAL_MMFI_AU_Cfg_Enable(MMFI_ALT_TS_SIZE, FALSE);
                break;
            case E_DRVMMFI_PKTMODE_192:
                HAL_MMFI_AU_PktChkSize_Set(0xBF);
                HAL_MMFI_AU_Cfg_Enable(MMFI_ALT_TS_SIZE, TRUE);
                break;
            case E_DRVMMFI_PKTMODE_204:
                HAL_MMFI_AU_PktChkSize_Set(0xCB);
                HAL_MMFI_AU_Cfg_Enable(MMFI_ALT_TS_SIZE, TRUE);
                break;
            default:
                break;
        }
    }
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
    {
        switch (PacketMode)
        {
            case E_DRVMMFI_PKTMODE_188:
                HAL_MMFI_V3D_PktChkSize_Set(0xBB);
                HAL_MMFI_V3D_Cfg_Enable(MMFI_ALT_TS_SIZE, FALSE);
                break;
            case E_DRVMMFI_PKTMODE_192:
                HAL_MMFI_V3D_PktChkSize_Set(0xBF);
                HAL_MMFI_V3D_Cfg_Enable(MMFI_ALT_TS_SIZE, TRUE);
                break;
            case E_DRVMMFI_PKTMODE_204:
                HAL_MMFI_V3D_PktChkSize_Set(0xCB);
                HAL_MMFI_V3D_Cfg_Enable(MMFI_ALT_TS_SIZE, TRUE);
                break;
            default:
                break;
        }
    }
#endif

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
    return DRVMMFI_FAIL;
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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_PKT192_EN, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_PKT192_EN, bEnable);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_PKT192_BLK_DISABLE, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_PKT192_BLK_DISABLE, bEnable);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_DUP_PKT_SKIP, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_DUP_PKT_SKIP, bEnable);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_TEI_SKIP_PKTF, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_TEI_SKIP_PKTF, bEnable);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_CLR_PIDFLT_BYTE_CNT, TRUE);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_CLR_PIDFLT_BYTE_CNT, TRUE);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_MEM_TSDATA_ENDIAN_AU, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_MEM_TSDATA_ENDIAN_V3D, bEnable);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Cfg_Enable(MMFI_MEM_TSORDER_ENDIAN_AU, bEnable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Cfg_Enable(MMFI_MEM_TSORDER_ENDIAN_V3D, bEnable);
#endif

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
        case E_DRVMMFI_CAP_FILTER_AU_NUM:
            *((MS_U32*)pOutput) += MMFI_AUDPIDFLT_NUM;
            return DRVMMFI_OK;
        case E_DRVMMFI_CAP_FILTER_ALL_NUM:
            *((MS_U32*)pOutput) += MMFI_AUDPIDFLT_NUM;
#ifdef MMFI_VD3D
        case E_DRVMMFI_CAP_FILTER_VD_NUM:
            *((MS_U32*)pOutput) += MMFI_V3DPIDFLT_NUM;
#endif
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
    MS_U32 u32state = 0xFF, u32cmdcnt = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
    {
        u32state = HAL_MMFI_AU_Get_Filein_Ctrl();
        u32cmdcnt = HAL_MMFI_AU_CmdQ_FIFO_Get_WRCnt();
    }
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
    {
        u32state = HAL_MMFI_V3D_Get_Filein_Ctrl();
        u32cmdcnt = HAL_MMFI_V3D_CmdQ_FIFO_Get_WRCnt();
    }
#endif

    if((u32state & MMFI_FILEIN_RSTART) || u32cmdcnt)
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
    MS_U32 u32temp = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    u32temp = HAL_MMFI_HWInt_Status();

    *pStatus = E_DRVMMFI_STATUS_UNKNOWN;
    if(u32temp & MMFI_HWINT_STS_FILEIN_DONE_AU)
        *pStatus |= E_DRVMMFI_STATUS_AU_FILEINDONE;
    if(u32temp & MMFI_HWINT_STS_AU_ERR)
        *pStatus |= E_DRVMMFI_STATUS_AU_PKTERR;
#ifdef MMFI_VD3D
    if(u32temp & MMFI_HWINT_STS_FILEIN_DONE_VD)
        *pStatus |= E_DRVMMFI_STATUS_VD_FILEINDONE;
    if(u32temp & MMFI_HWINT_STS_VD3D_ERR)
        *pStatus |= E_DRVMMFI_STATUS_VD_PKTERR;
#endif

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
    HAL_MMFI_HWInt_Clear(u32int);
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

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Get_TsHeaderInfo((MS_U32*)pheader);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Get_TsHeaderInfo((MS_U32*)pheader);
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
    {
        HAL_MMFI_AU_Cfg_Enable(MMFI_BYTE_TIMER_EN, TRUE);
        HAL_MMFI_AU_Set_Filein_Ctrl(MMFI_FILEIN_RSTART);
    }
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
    {
        HAL_MMFI_V3D_Cfg_Enable(MMFI_BYTE_TIMER_EN, TRUE);
        HAL_MMFI_V3D_Set_Filein_Ctrl(MMFI_FILEIN_RSTART);
    }
#endif

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

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_Set_Filein_Ctrl(MMFI_FILEIN_ABORT);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_Set_Filein_Ctrl(MMFI_FILEIN_ABORT);
#endif

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
    MS_U32 u32cfg = 0;

    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    _MMFI_ENTRY();

    if((fileinmode == E_DRVMMFI_MODE_APID_BYPASS) || (fileinmode == E_DRVMMFI_MODE_NO_BYPASS_AU) ||
        (fileinmode == E_DRVMMFI_MODE_PS_AU) || (fileinmode == E_DRVMMFI_MODE_PS_AUB) ||
        (fileinmode == E_DRVMMFI_MODE_APIDB_BYPASS))
    {
        u32cfg = HAL_MMFI_AU_Cfg_Get() & ~MMFI_AU_CFG_MASK;

        if(fileinmode == E_DRVMMFI_MODE_APID_BYPASS)
            u32cfg |= MMFI_APID_BYPASS;

        #ifdef MMFI_APIDB_BYPASS
        if(fileinmode == E_DRVMMFI_MODE_APIDB_BYPASS)
            u32cfg |= MMFI_APIDB_BYPASS;
        #endif
        
        HAL_MMFI_AU_Cfg_Set(u32cfg);
    }
    
    #ifdef MMFI_VD3D
    if((fileinmode == E_DRVMMFI_MODE_V3DPID_BYPASS) || (fileinmode == E_DRVMMFI_MODE_NO_BYPASS_V3D) ||
        (fileinmode == E_DRVMMFI_MODE_PS_V3D))
    {
        u32cfg = HAL_MMFI_V3D_Cfg_Get() & ~MMFI_VD_CFG_MASK;

        if(fileinmode == E_DRVMMFI_MODE_V3DPID_BYPASS)
            u32cfg |= MMFI_VPID3D_BYPASS;
        HAL_MMFI_V3D_Cfg_Set(u32cfg);
    }
    #endif

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
    HAL_MMFI_AU_RemoveDupPkt(bEnable);

#ifdef MMFI_VD3D
    HAL_MMFI_V3D_RemoveDupPkt(bEnable);
#endif //MMFI_VD3D

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
    MMFI_DBGINFO(printf("[%d]%s\n", __LINE__, __FUNCTION__));

#ifdef MMFI_VD3D
    _MMFI_ENTRY();
    HAL_MMFI_V3D_Cfg_Enable(MMFI_USE_VD3D_PATH, bEnable);
    _MMFI_RETURN(DRVMMFI_OK);
#else    //MMFI_VD3D
    return DRVMMFI_FUNC_ERROR;
#endif
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

#ifdef MM_MOBF

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_MOBF_Set_FileinKey(u32Key);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_MOBF_Set_FileinKey(u32Key);
#endif //MMFI_VD3D

    _MMFI_RETURN(DRVMMFI_OK);

#else

    return DRVMMFI_FUNC_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MOBF decrypt mode
/// @param  benable                \b IN: Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
MMFI_Result MDrv_MMFI_MOBF_FileIn_Enable(DrvMMFI_Path epath, MS_BOOL benable)
{
#ifdef MM_MOBF

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_MOBF_Enable(benable);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_MOBF_Enable(benable);
#endif //MMFI_VD3D

    _MMFI_RETURN(DRVMMFI_OK);

#else

    return DRVMMFI_FUNC_ERROR;

#endif

}

MMFI_Result MDrv_MMFI_MOBF_FileIn_SetLevel(DrvMMFI_Path epath, MS_U8 u8level)
{
#ifdef MM_MOBF

    _MMFI_ENTRY();

    if(epath == E_DRVMMFI_PTH_AUDIO)
        HAL_MMFI_AU_MOBF_SetLevel(u8level);
#ifdef MMFI_VD3D
    else if(epath == E_DRVMMFI_PATH_VD3D)
        HAL_MMFI_V3D_MOBF_SetLevel(u8level);
#endif //MMFI_VD3D

    _MMFI_RETURN(DRVMMFI_OK);

#else

    return DRVMMFI_FUNC_ERROR;

#endif

}



