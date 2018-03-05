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
////////////////////////////////////////////////////////////////////////////////////////////////////
/// file   drvFQ.c
/// @brief  FQ Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"
#include "regFQ.h"
#include "halFQ.h"

#ifdef FQ_HW_BOX_SERIES
    #include "regTSP.h"
    #include "halTSP.h"
#endif

#include "drvFQ.h"
#include "drvMMIO.h"
#include "asmCPU.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

//--------------------------------------------------------------------------------------------------
//  Debug Macro
//--------------------------------------------------------------------------------------------------
#define MS_DEBUG 1
#if MS_DEBUG
#define FQ_DBG(_fmt,_args...)             {printf("[%s][%d]",__FUNCTION__,__LINE__); printf(_fmt,##_args);}
#else
#define FQ_DBG(_args)                     { }
#endif

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define _FQ_ENTRY()       if (!MsOS_ObtainMutex(_s32FQMutexId, MSOS_WAIT_FOREVER))      \
                            {                                                               \
                                _u32LastErr = DRVFQ_FUNC_ERROR;                           \
                                return DRVFQ_FUNC_ERROR;                                  \
                            }                                                               \
                            if (_bSuspend)                                                  \
                            {                                                               \
                                _FQ_RETURN(DRVFQ_FAIL);                                 \
                            }

#define _FQ_RETURN(_ret)  if(MsOS_ReleaseMutex(_s32FQMutexId)) {return _ret;}           \
                            else                                                            \
                            {                                                               \
                                _u32LastErr = DRVFQ_FUNC_ERROR;                           \
                                return DRVFQ_FUNC_ERROR;                                  \
                            }

#define FQ_NUM_CHECK_RET(idx) if(idx >= FQ_NUM ) \
                            { printf("[FQ][ERR] FQ Engine [%d] not exist ! \n", (unsigned int)idx);   \
                             return 0; }

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------
static MS_S32                           _s32FQMutexId = -1;
static MS_U32                           _u32LastErr;
static MS_BOOL                          _bSuspend = FALSE;
//MS_U32                                  _u32FQIntNum = ; //for intr
static MS_BOOL                          _bFQInitReady = FALSE;
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------
static void _FQ_RegStateReset(MS_U32 u32FQEng)
{
    HAL_FQ_PVR_SetBuf(u32FQEng, 0, 0);
    HAL_FQ_SkipRushData(u32FQEng, FIQ_CFG11_SKIP_RUSH_DATA_PATH_NON);
    HAL_FQ_PVR_SetRushAddr(u32FQEng, 0);
}

static void _FQ_Init(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
    HAL_FQ_SWReset(u32FQEng, FALSE);
    HAL_FQ_Bypass(u32FQEng, FALSE);
    HAL_FQ_AddrMode(u32FQEng, u8AddrMode);
}

static void _FQ_Exit(MS_U32 u32FQEng)
{
    HAL_FQ_SWReset(u32FQEng, TRUE);
    HAL_FQ_Bypass(u32FQEng, TRUE);
    HAL_FQ_AddrMode(u32FQEng, FALSE);
}

// General API
//--------------------------------------------------------------------------------------------------
/// Initialize FQ driver and FQ engine
/// @return FQ_Result
/// @note
/// It should be called before calling any other FQ DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//--------------------------------------------------------------------------------------------------
FQ_Result MDrv_FQ_Init(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif
    MS_VIRT virtBank;
    MS_PHY  phyBankSize;

    FQ_NUM_CHECK_RET(u32MapingFQEng);

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &phyBankSize, MS_MODULE_HW))
    {
        FQ_DBG("MDrv_MMIO_GetBASE fail\n");
        MS_ASSERT(0);
    }

    if(!HAL_FQ_SetBank(virtBank))
    {
        _FQ_RETURN(DRVFQ_FAIL);
    }

    _FQ_Init(u32MapingFQEng, u8AddrMode);

    _FQ_RegStateReset(u32MapingFQEng);
    //default for PCR filter & RASP/PVR, we skip rush data to avoid the older data.
#ifdef FQ_HW_BOX_SERIES
    HAL_FQ_SkipRushData(u32MapingFQEng,   FIQ_CFG11_SKIP_PCR_RUSH_DATA | FIQ_CFG11_SKIP_PVR1_RUSH_DATA
                                        | FIQ_CFG11_SKIP_PVR2_RUSH_DATA | FIQ_CFG11_SKIP_PVR3_RUSH_DATA
                                        | FIQ_CFG11_SKIP_PVR4_RUSH_DATA);
#else
    HAL_FQ_SkipRushData(u32FQEng, FIQ_CFG11_SKIP_PCR_RUSH_DATA | FIQ_CFG11_SKIP_APES_RUSH_DATA | FIQ_CFG11_SKIP_APES_B_RUSH_DATA
                                | FIQ_CFG11_SKIP_PVR1_RUSH_DATA|FIQ_CFG11_SKIP_SEC_RUSH_DATA|FIQ_CFG11_SKIP_ADP_RUSH_DATA|FIQ_CFG11_SKIP_PCR1_RUSH_DATA);
#endif

    if (-1 == _s32FQMutexId)
        _s32FQMutexId = MsOS_CreateMutex(E_MSOS_FIFO , "FQ_Mutex" , MSOS_PROCESS_SHARED);
    else
        FQ_DBG("Create _s32FQMutexId more than once\n");

    _FQ_ENTRY();
    _bFQInitReady = TRUE;
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_IsInit(MS_U32 u32FQEng, MS_BOOL* pbIsInit)
{
    _FQ_ENTRY();
    *pbIsInit = _bFQInitReady;
    _FQ_RETURN(DRVFQ_OK);
}

//--------------------------------------------------------------------------------------------------
/// Close FQ driver and reset software/register state
/// @return FQ_Result
/// @note
//--------------------------------------------------------------------------------------------------
FQ_Result MDrv_FQ_Exit(MS_U32 u32FQEng)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    if (_s32FQMutexId != -1)
        _FQ_ENTRY();

    if(_bFQInitReady == TRUE)
    {
        _FQ_RegStateReset(u32MapingFQEng);
        _FQ_Exit(u32MapingFQEng);

        _bFQInitReady = FALSE;
    }

    if (_s32FQMutexId != -1)
    {
        MsOS_ReleaseMutex(_s32FQMutexId);
        MsOS_DeleteMutex(_s32FQMutexId);
        _s32FQMutexId = -1;
    }

    return DRVFQ_OK;
}

FQ_Result MDrv_FQ_SetBuffer(MS_U32 u32FQEng, MS_PHY phyBufStart, MS_U32 u32BufSize)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();
#ifdef FQ_HW_BOX_SERIES
    HAL_TSP_FQ_MMFI_MIU_Sel(E_TSP_HAL_MIU_SEL_FQ, u32MapingFQEng, phyBufStart);
#endif
    HAL_FQ_PVR_SetBuf(u32MapingFQEng, phyBufStart, u32BufSize);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_Start(MS_U32 u32FQEng, MS_BOOL bStart)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();

    if (!bStart)
        HAL_FQ_PVR_Stop(u32MapingFQEng);
    else
        HAL_FQ_PVR_Start(u32MapingFQEng);

    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();
    HAL_FQ_PVR_SetRushAddr(u32MapingFQEng, phyRushAddr);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_SkipRushData(MS_U32 u32FQEng, MS_U32 u32SkipPath)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);

    _FQ_ENTRY();

    HAL_FQ_SkipRushData(u32MapingFQEng, u32SkipPath);

    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_RushEnable(MS_U32 u32FQEng)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();
    HAL_FQ_Rush_Enable(u32MapingFQEng);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_TimeStampSetRecordStamp(MS_U32 u32FQEng, MS_U32 u32Stamp)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();
    HAL_FQ_SetPVRTimeStamp(u32MapingFQEng, u32Stamp);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_TimeStampGetRecordStamp(MS_U32 u32FQEng, MS_U32* u32Stamp)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);
    _FQ_ENTRY();
    *u32Stamp = HAL_FQ_GetPVRTimeStamp(u32MapingFQEng);
    _FQ_RETURN(DRVFQ_OK);
}


FQ_Result MDrv_FQ_GetReadAddr(MS_U32 u32FQEng, MS_PHY* pphyReadAddr)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);

    _FQ_ENTRY();

    *pphyReadAddr = HAL_FQ_GetRead(u32MapingFQEng);

    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_GetWriteAddr(MS_U32 u32FQEng, MS_PHY* pphyWriteAddr)
{
#ifdef FQ_HW_BOX_SERIES
    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
#else
    MS_U32 u32MapingFQEng = u32FQEng;
#endif

    FQ_NUM_CHECK_RET(u32MapingFQEng);

    _FQ_ENTRY();

    *pphyWriteAddr = HAL_FQ_GetWrite(u32MapingFQEng);

    _FQ_RETURN(DRVFQ_OK);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
FQ_Result MDrv_FQ_Suspend(void)
{
    if(_bFQInitReady == TRUE)
    {
        HAL_FQ_SaveRegs();
    }

    return DRVFQ_OK;
}

FQ_Result MDrv_FQ_Resume(void)
{
    if(_bFQInitReady == TRUE)
    {
        HAL_FQ_RestoreRegs();
    }

    return DRVFQ_OK;
}
#endif  //MSOS_TYPE_LINUX_KERNEL


