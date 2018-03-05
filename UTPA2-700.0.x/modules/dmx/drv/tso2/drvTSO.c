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
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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
/// file    drvTSO.c
/// @brief  TS I/O Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSO DDI are not allowed to use in any interrupt context other than TSO ISR and Callback
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
#include "regTSO.h"
#include "halTSO.h"
#include "drvTSO.h"

#include "ULog.h"

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
#define TSO_MUTEX_WAIT_TIME                            0xFFFFFFFFUL

#define TSO_DBGERR(x)                                  {if(_tsodbglevel >= E_DRVTSO_DBG_ERR) x;  }
#define TSO_DBGINFO(x)                                 {if(_tsodbglevel >= E_DRVTSO_DBG_INFO) x;  }

#define TSO_ASSERT(_bool)                               if(!(_bool))   \
                                                        {               \
                                                            if(_tsodbglevel >= E_DRVTSO_DBG_INFO)    \
                                                              {  ULOGD("TSO", "ASSERT ERR\n");  }           \
                                                            return DRVTSO_FAIL;                    \
                                                        }

#define _TSO_ENTRY()                                    if (!MsOS_ObtainMutex(_ptso_res->_s32TSOMutexId, TSO_MUTEX_WAIT_TIME))      \
                                                        {               \
                                                            return DRVTSO_FAIL;                                           \
                                                        }

#define _TSO_RETURN(_ret)                               do{                                                                \
                                                            MsOS_ReleaseMutex(_ptso_res->_s32TSOMutexId);         \
                                                            return _ret;                                                   \
                                                        }while(0);

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

typedef struct _TSO_RESOURCE_PRIVATE
{
    MS_S32                       _s32TSOMutexId;

    DrvTSO_FltState              _FltState[TSO_ENGINE_NUM][TSO_PIDFLT_NUM];
    MS_U32                       _u32FltChSrc[TSO_ENGINE_NUM][TSO_PIDFLT_NUM];
    MS_U16                       _u16FltPid[TSO_ENGINE_NUM][TSO_PIDFLT_NUM];

    DrvTSO_FltState              _RepFltState[TSO_ENGINE_NUM][TSO_REP_PIDFLT_NUM];
    MS_U16                       _u16RepFltOrgPid[TSO_ENGINE_NUM][TSO_REP_PIDFLT_NUM];
    MS_U16                       _u16RepFltNewPid[TSO_ENGINE_NUM][TSO_REP_PIDFLT_NUM];
    MS_U16                       _u16RepFltChSrc[TSO_ENGINE_NUM][TSO_REP_PIDFLT_NUM];

    MS_BOOL                      _bSWInit;
}TSO_RESOURCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifndef TSO_UTOPIA_20
static TSO_RESOURCE_PRIVATE _tso_res = {
    ._s32TSOMutexId                        = -1,
    ._bSWInit                              = FALSE,
};

static TSO_RESOURCE_PRIVATE*    _ptso_res = &_tso_res;
#else
static TSO_RESOURCE_PRIVATE*    _ptso_res = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static DrvTSO_DbgLevel                     _tsodbglevel = E_DRVTSO_DBG_Release;
static MS_BOOL                             _bBankInit   = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _TSO_InitResource(TSO_RESOURCE_PRIVATE* presource)
{
    if(presource == NULL)
    {
        return FALSE;
    }

    presource->_s32TSOMutexId = -1;
    presource->_bSWInit       = FALSE;

    return TRUE;
}

static MS_U8 _TSO_TSIF2ChID(DrvTSO_If eIf)
{
    switch(eIf)
    {
        case E_DRVTSO_IF_LIVE0:
            return HAL_TSO_TSIF_LIVE1;
        case E_DRVTSO_IF_LIVE1:
            return HAL_TSO_TSIF_LIVE2;
        case E_DRVTSO_IF_LIVE2:
            return HAL_TSO_TSIF_LIVE3;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case E_DRVTSO_IF_LIVE3:
            return HAL_TSO_TSIF_LIVE4;
        case E_DRVTSO_IF_LIVE4:
            return HAL_TSO_TSIF_LIVE5;
        case E_DRVTSO_IF_LIVE5:
            return HAL_TSO_TSIF_LIVE6;
    #endif
        case E_DRVTSO_IF_FILE0:
            return HAL_TSO_TSIF_FILE1;
        case E_DRVTSO_IF_FILE1:
            return HAL_TSO_TSIF_FILE2;
#ifdef TSO_PVR_SUPPORT
        case E_DRVTSO_IF_MMT0:
            return HAL_TSO_TSIF_LIVE_MMT;
#endif
        default:
            return 0xFF;
    }
}

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_InitLibResource(void* pResMemAddr)
{
    MS_VIRT virtBank;
    MS_PHY u32BankSize;

    if(pResMemAddr == 0)
        return DRVTSO_FAIL;

    _ptso_res = (TSO_RESOURCE_PRIVATE*)pResMemAddr;

    if(_ptso_res->_bSWInit != TRUE)
    {
        if(_TSO_InitResource(_ptso_res) == FALSE)
            return DRVTSO_FAIL;
    }

    // For multi-process use case. (different process should set the value of bank again)
    if(_bBankInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSO))
        {
            TSO_DBGERR(ULOGE("TSO", "MDrv_TSO_GetBASE failed\n"));
            return DRVTSO_FAIL;
        }
        TSO_DBGINFO((ULOGD("TSO", "TSO Base Bank 0x%08x\n", (unsigned int)virtBank)));
        HAL_TSO_SetBank(virtBank);
        _bBankInit = TRUE;
    }

    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSO driver
/// @return TSO_Result
/// @note
/// It should be called before calling any other TSO DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Init(void)
{
    MS_VIRT virtBank;
    MS_PHY u32BankSize;
    MS_U16 u16ii;
    MS_U8  u8jj;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSO))
    {
        TSO_DBGERR(ULOGE("TSO", "MDrv_TSO_GetBASE failed\n"));
        return DRVTSO_FAIL;
    }
    TSO_DBGINFO((ULOGD("TSO", "TSO Base Bank 0x%08x\n", (unsigned int)virtBank)));
    HAL_TSO_SetBank(virtBank);

    _ptso_res->_s32TSOMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex TSO", MSOS_PROCESS_SHARED);
    if(_ptso_res->_s32TSOMutexId == -1)
    {
        TSO_DBGERR(ULOGE("TSO", "MsOS_CreateMutex failed\n"));
        return DRVTSO_FAIL;
    }

    _TSO_ENTRY();

    HAL_TSO_PowerCtrl(TRUE);

    HAL_TSO_Init();

    for(u8jj = 0; u8jj < (MS_U8)TSO_ENGINE_NUM; u8jj++)
    {
        HAL_TSO_Reset_All(u8jj);
        HAL_TSO_Set_SVQRX_PktMode(u8jj, TSO_SVQ_RX_CFG_MODE_CIPL);
        HAL_TSO_Set_SVQRX_ArbitorMode(u8jj, TSO_SVQ_RX_CFG_ARBMODE_RUNROBIN, NULL);

        for(u16ii = 0; u16ii < (MS_U16)TSO_PIDFLT_NUM; u16ii++)
        {
            HAL_TSO_Flt_SetInputSrc(u8jj, u16ii, 0);
            HAL_TSO_Flt_SetPid(u8jj, u16ii, TSO_PID_NULL);
            _ptso_res->_FltState[u8jj][u16ii] = E_DRVTSO_FLT_STATE_FREE;
            _ptso_res->_u32FltChSrc[u8jj][u16ii] = 0;
            _ptso_res->_u16FltPid[u8jj][u16ii] = TSO_PID_NULL;
        }

        for(u16ii = 0; u16ii < (MS_U16)TSO_REP_PIDFLT_NUM; u16ii++)
        {
            _ptso_res->_RepFltState[u8jj][u16ii] = E_DRVTSO_FLT_STATE_FREE;
            _ptso_res->_u16RepFltOrgPid[u8jj][u16ii] = TSO_PID_NULL;
            _ptso_res->_u16RepFltNewPid[u8jj][u16ii] = TSO_PID_NULL;
            _ptso_res->_u16RepFltChSrc[u8jj][u16ii] = 0;
        }

        for(u16ii = 0; u16ii < (MS_U16)TSO_FILE_IF_NUM; u16ii++)
        {
            HAL_TSO_Filein_Rate(u8jj, (MS_U8)u16ii, 0x0A);
        }
    }

    _ptso_res->_bSWInit = TRUE;

    _TSO_RETURN(DRVTSO_OK);

}

//-------------------------------------------------------------------------------------------------
/// Close TSO driver
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Exit(void)
{
    MS_U8 u8ii, u8jj;
    MS_U8 u8TsIf[TSO_TSIF_NUM] = {HAL_TSO_TSIF_LIVE1, HAL_TSO_TSIF_LIVE2, HAL_TSO_TSIF_LIVE3};
    MS_BOOL bFlag = FALSE;
    MS_U16 u16Data = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    for(u8jj = 0; u8jj < TSO_ENGINE_NUM; u8jj++)
    {
        HAL_TSO_Cfg1_Enable(u8jj, (TSO_CFG1_TSO_OUT_EN|TSO_CFG1_TSO_TSIF1_EN|TSO_CFG1_TSO_TSIF5_EN|TSO_CFG1_TSO_TSIF6_EN), FALSE);

        for(u8ii = 0; u8ii < TSO_TSIF_NUM; u8ii++)
        {
            HAL_TSO_Set_InClk(u8jj, u8TsIf[u8ii], 0, FALSE, FALSE);
        }
        HAL_TSO_OutClk(u8jj, &u16Data, &bFlag, &bFlag, TRUE);
    }

    MsOS_ReleaseMutex(_ptso_res->_s32TSOMutexId);
    MsOS_DeleteMutex(_ptso_res->_s32TSOMutexId);

    _ptso_res->_s32TSOMutexId = -1;
    _ptso_res->_bSWInit       = FALSE;

    HAL_TSO_PowerCtrl(FALSE);

    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO HW interrupt status
/// @param  u8Eng                               \b IN: TSO engine ID
/// @param  pu16status                        \b OUT: pointer of storing HW interrupt status value
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Get_HWIntStatus(MS_U8 u8Eng, MS_U16* pu16status)
{
    *pu16status = HAL_TSO_HWInt_Status(u8Eng);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set debug level of TSO
/// @param  DbgLevel                     \b IN: Debug Level Value
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetDbgLevel(DrvTSO_DbgLevel DbgLevel)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _tsodbglevel = DbgLevel;
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set TSO operation mode
/// @param  u8Eng                               \b IN: TSO engine ID
/// @param  TSOCtrlMode                         \b IN: Ctrol mode
/// @param  bClkInv                             \b IN: TSO_IN_Clk Invert
/// @param  bParallel                           \b IN: TSO_IN_Interface paralleled
/// @return TSO_Result
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetOperateMode(MS_U8 u8Eng, DrvTSO_CtrlMode TSOCtrlMode, MS_BOOL bBypassAll, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((u8Eng < TSO_ENGINE_NUM));
    _TSO_ENTRY();

    MS_U8  u8TsIf = HAL_TSO_TSIF_LIVE1;
    MS_U16 u16ValidCount = 0xBB;
    MS_U16 u16InValidCount = 0x0;

    switch(TSOCtrlMode)
    {
        case E_DRVTSO_CTRL_MODE_TS0:
            u8TsIf = HAL_TSO_TSIF_LIVE1;
            HAL_TSO_PktChkSize_Set(u8Eng, HAL_TSO_TSIF_LIVE1, 0xBB);
            break;
        case E_DRVTSO_CTRL_MODE_TS1:
            u8TsIf = HAL_TSO_TSIF_LIVE2;
            HAL_TSO_PktChkSize_Set(u8Eng, HAL_TSO_TSIF_LIVE2, 0xBB);
            HAL_TSO_Filein_Enable(u8Eng, 0, FALSE);
            break;
        case E_DRVTSO_CTRL_MODE_TS2:
            u8TsIf = HAL_TSO_TSIF_LIVE3;
            HAL_TSO_PktChkSize_Set(u8Eng, HAL_TSO_TSIF_LIVE3, 0xBB);
            HAL_TSO_Filein_Enable(u8Eng, 1, FALSE);
            break;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case E_DRVTSO_CTRL_MODE_TS3:
            u8TsIf = HAL_TSO_TSIF_LIVE4;
            HAL_TSO_PktChkSize_Set(u8Eng, HAL_TSO_TSIF_LIVE4, 0xBB);
            break;
    #endif
        case E_DRVTSO_CTRL_MODE_MEM:
            u8TsIf = HAL_TSO_TSIF_FILE1;
            HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0, 0xBB);
            HAL_TSO_Filein_Enable(u8Eng, 0, TRUE);
            break;

        case E_DRVTSO_CTRL_MODE_MEM1:
            u8TsIf = HAL_TSO_TSIF_FILE2;
            HAL_TSO_Filein_PktChkSize_Set(u8Eng, 1, 0xBB);
            HAL_TSO_Filein_Enable(u8Eng, 1, TRUE);
            break;

#ifdef TSO_PVR_SUPPORT
        case E_DRVTSO_CTRL_MODE_MMT:
            u8TsIf = HAL_TSO_TSIF_LIVE_MMT;
            HAL_TSO_PVR_Src(HAL_TSO_PVR_MMT);
            break;
#endif

        default:
            ULOGE("TSO", "Not supported TSOCtrlMode = %d! \n", TSOCtrlMode);
            _TSO_RETURN(DRVTSO_INVALID_PARAM);
    }

    if(TSOCtrlMode != E_DRVTSO_CTRL_MODE_MMT)
    {
        HAL_TSO_RW_ValidBlock_Count(0, TRUE, &u16ValidCount);
        HAL_TSO_RW_InvalidBlock_Count(0, TRUE, &u16InValidCount);
        HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_ENABLE_SYS_TIMESTAMP, bEnable);

        HAL_TSO_ChIf_Cfg(u8Eng, u8TsIf, TSO_CHCFG_PIDFLT_REC_ALL|TSO_CHCFG_PIDFLT_REC_NULL, bBypassAll); //PID bypass mode

        HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_OUT_EN, bEnable);
    }

    HAL_TSO_ChIf_Enable(u8Eng, u8TsIf, bEnable);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set TSIF Input enable
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  eIf                                    \b IN: TSO TS interface
/// @param  bEnable                              \b IN: TRUE is enabled; FALSE is disabled
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Ch_InputEnable(MS_U8 u8Eng, DrvTSO_If eIf, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    MS_U8 u8ChId = 0;

    if((u8ChId = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();
    HAL_TSO_ChIf_Enable(u8Eng, u8ChId, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set TSO Output enable
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  bEnable                              \b IN: TRUE is enabled; FALSE is disabled
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_ENABLE_SYS_TIMESTAMP, bEnable);
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_OUT_EN, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set TSO File-in Byte Timer
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u8FileEng                            \b IN: Filein Engine
/// @param  u16timer                            \b IN: Timer value
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Rate(MS_U8 u8FileEng, MS_U16 u16timer)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    HAL_TSO_Filein_Rate(0, u8FileEng, u16timer);
    HAL_TSO_FileinTimer_Enable(0, u8FileEng, (u16timer != 0));

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set TSO Out Clock
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eOutClk                                       \b IN: TSO out clock select
/// @param  eOutClkSrc                                   \b IN: TSO out clock source select
/// @param  u16DivNum                                  \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv                                        \b IN: If Out Clock invert
/// @return TSO_Result
/// @note
/// Out Clk 5.4M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 15
/// Out Clk 6.17M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 13
/// Out Clk 6.64M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 12
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetOutClk(MS_U8 u8Eng, DrvTSO_OutClk eOutClk, DrvTSO_OutClkSrc eOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    if(eOutClk == E_DRVTSO_OUTCLK_DIVN)
    {
        TSO_DBGERR(ULOGE("TSO", "[%d]%s TSO not support selected clk. \n", __LINE__, __FUNCTION__));
        _TSO_RETURN(DRVTSO_NOT_SUPPORT);
    }
#endif

    MS_U16 u16OutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
    HalTSOOutClk stTsoOutClk = {0xFFFF, 0xFFFF, 0, 0xFFFF, bClkInv, TRUE};

    //ULOGD("TSO", "[%s] eOutClk %x, eOutClkSrc %x, u16DivNum %x\n", __FUNCTION__, eOutClk,  eOutClkSrc, u16DivNum);

    switch(eOutClkSrc)
    {
        case E_DRVTSO_OUTCLKSRC_172M_2N:
            u16OutClkSrc = HAL_TSO_OUT_DIV_SEL_172M_2N;
            break;
        case E_DRVTSO_OUTCLKSRC_288M_2N:
            u16OutClkSrc = HAL_TSO_OUT_DIV_SEL_288M_2N;
            break;
        case E_DRVTSO_OUTCLKSRC_216M_N:
            u16OutClkSrc = HAL_TSO_OUT_DIV_SEL_216M_N;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS0IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS0IN;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS1IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS1IN;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS2IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS2IN;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS3IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS3IN;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS4IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS4IN;
            break;
        case E_DRVTSO_OUTCLKSRC_P_TS5IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS5IN;
            break;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case E_DRVTSO_OUTCLKSRC_P_TS6IN:
            u16OutClkSrc = HAL_PRE_TSO_OUT_SEL_TS6IN;
            break;
    #endif
        default:
            break;
    }

    stTsoOutClk.bClkInvert = bClkInv;
    switch(eOutClk)
    {
        case E_DRVTSO_OUTCLK_DIV2N:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_DIV2N;
            stTsoOutClk.u16OutDivSrc = u16OutClkSrc;
            stTsoOutClk.u16OutDivNum = u16DivNum;
            break;
        case E_DRVTSO_OUTCLK_DIVN:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_DIV;
            stTsoOutClk.u16OutDivSrc = u16OutClkSrc;
            stTsoOutClk.u16OutDivNum = u16DivNum;
            break;
        case E_DRVTSO_OUTCLK_62M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_62MHz;
            break;
        case E_DRVTSO_OUTCLK_54M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_54MHz;
            break;
        case E_DRVTSO_OUTCLK_PTSOOUT:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            stTsoOutClk.u16PreTsoOutClk = u16OutClkSrc;
            break;
        case E_DRVTSO_OUTCLK_PTSOOUT_DIV8:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8;
            stTsoOutClk.u16PreTsoOutClk = u16OutClkSrc;
            break;
        case E_DRVTSO_OUTCLK_Dmd:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD;
            break;
        case E_DRVTSO_OUTCLK_27M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_27MHz;
            break;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case E_DRVTSO_OUTCLK_86M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_86MHz;
            break;
        case E_DRVTSO_OUTCLK_108M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_108MHz;
            break;
        case E_DRVTSO_OUTCLK_123M:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_123MHz;
            break;
    #endif
        default:
            _TSO_RETURN(DRVTSO_INVALID_PARAM);
    }

    //ULOGD("TSO", "[%s] u16OutClk %x, u16PreTsoOutClk %x\n", __FUNCTION__, stTsoOutClk.u16OutClk,  stTsoOutClk.u16PreTsoOutClk );

    HAL_TSO_OutputClk(u8Eng, &stTsoOutClk, TRUE);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get TSO Out Clock
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eOutClk                                       \b IN: TSO out clock select
/// @param  eOutClkSrc                                   \b IN: TSO out clock source select
/// @param  u16DivNum                                  \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv                                        \b IN: If Out Clock invert
/// @return TSO_Result
/// @note
/// Out Clk 5.4M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 15
/// Out Clk 6.17M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 13
/// Out Clk 6.64M,
///         eOutClkSrc = E_DRVTSO_OUTCLK_DIV2N
///         eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N
///         u16DivNum = 12
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetOutClk(MS_U8 u8Eng, DrvTSO_OutClk* peOutClk, DrvTSO_OutClkSrc* peOutClkSrc, MS_U16* pu16DivNum, MS_BOOL* pbClkInv)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    HalTSOOutClk stTsoOutClk = {0xFFFF, 0xFFFF, 0, 0xFFFF, FALSE, TRUE};

    //ULOGD("TSO", "[%s] eOutClk %x, eOutClkSrc %x, u16DivNum %x\n", __FUNCTION__, eOutClk,  eOutClkSrc, u16DivNum);

    HAL_TSO_OutputClk(u8Eng, &stTsoOutClk, FALSE);
    *pbClkInv = stTsoOutClk.bClkInvert;

    switch(stTsoOutClk.u16OutClk)
    {
        case HAL_TSO_OUT_SEL_TSO_OUT_DIV2N:
            *peOutClk = E_DRVTSO_OUTCLK_DIV2N;
            *peOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
            *pu16DivNum = stTsoOutClk.u16OutDivNum;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_DIV:
            *peOutClk = E_DRVTSO_OUTCLK_DIVN;
            *peOutClkSrc = E_DRVTSO_OUTCLKSRC_216M_N;
            *pu16DivNum = stTsoOutClk.u16OutDivNum;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_62MHz:
            *peOutClk = E_DRVTSO_OUTCLK_62M;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_54MHz:
            *peOutClk = E_DRVTSO_OUTCLK_54M;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT:
            *peOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8:
            *peOutClk = E_DRVTSO_OUTCLK_PTSOOUT_DIV8;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD:
            *peOutClk = E_DRVTSO_OUTCLK_Dmd;
            break;
        case HAL_TSO_OUT_SEL_TSO_OUT_27MHz:
            *peOutClk = E_DRVTSO_OUTCLK_27M;
            break;
        default:
            *peOutClk = 0xFF;
            break;
    }

    *peOutClkSrc = 0xFF;
    if((stTsoOutClk.u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT) || (stTsoOutClk.u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8))
    {
        switch(stTsoOutClk.u16PreTsoOutClk)
        {
            case HAL_PRE_TSO_OUT_SEL_TS0IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
                break;
            case HAL_PRE_TSO_OUT_SEL_TS1IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
                break;
            case HAL_PRE_TSO_OUT_SEL_TS2IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
                break;
            case HAL_PRE_TSO_OUT_SEL_TS3IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS3IN;
                break;
            case HAL_PRE_TSO_OUT_SEL_TS4IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS4IN;
                break;
            case HAL_PRE_TSO_OUT_SEL_TS5IN:
                *peOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS5IN;
                break;
            default:
                break;
        }
    }

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get TSO Interface status
/// @param  u8Eng                                  \b IN: TSO engine ID
/// @param  eIf                                \b IN: TSO input IF, enum item of DrvTSO_If
/// @param  pIfSet                                 \b OUT: Pointer to store TSO interface status
/// @param  pbBypassAll                           \b OUT: Pointer to store bypass mode status of TSO Channel interface
/// @param  pbEnable                               \b OUT: Pointer to store enable status of TSO Channel interface
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetTSIFStatus(MS_U8 u8Eng, DrvTSO_If eIf, DrvTSO_If_Set* pIfSet, MS_BOOL* pbBypassAll, MS_BOOL* pbEnable)
{
    MS_U8 u8Id = 0;
    MS_U16 u16Data = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d][%s]\n", __LINE__, __FUNCTION__));

    if((eIf == E_DRVTSO_IF_FILE0) || (eIf == E_DRVTSO_IF_FILE1))
    {
        pIfSet->ePad = E_DRVTSO_MEM;
        pIfSet->bClkInv = FALSE;
        pIfSet->bExtSync = FALSE;
        pIfSet->bParallel = FALSE;
        return DRVTSO_OK;
    }

    if((u8Id = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    HAL_TSO_GetInputTSIF_Status(u8Eng, u8Id, &u16Data, &(pIfSet->bClkInv), &(pIfSet->bExtSync), &(pIfSet->bParallel));

    switch(u16Data)
    {
        case HAL_TSOIN_MUX_TS0:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT0;
            break;
        case HAL_TSOIN_MUX_TS1:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT1;
            break;
        case HAL_TSOIN_MUX_TS2:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT2;
            break;
        case HAL_TSOIN_MUX_TS3:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT3;
            break;
        case HAL_TSOIN_MUX_TS4:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT4;
            break;
        case HAL_TSOIN_MUX_TS5:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT5;
            break;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case HAL_TSOIN_MUX_TS6:
            pIfSet->ePad = E_DRVTSO_PAD_EXT_INPUT6;
            break;
    #endif
        case HAL_TSOIN_MUX_TSDEMOD0:
            pIfSet->ePad = E_DRVTSO_PAD_DEMOD;
            break;
        default:
            pIfSet->ePad = HAL_TSOIN_MUX_MEM;
            break;
    }

    HAL_TSO_Get_ChIf_Cfg(u8Eng, u8Id, &u16Data, pbEnable);

    *pbBypassAll = ((u16Data & TSO_CHCFG_PIDFLT_REC_ALL) == TSO_CHCFG_PIDFLT_REC_ALL);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSO PID filter
/// @param  u8Eng                                  \b IN: TSO engine ID
/// @param  pu16PidFltId                         \b OUT: pointer of PID filter ID for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_Alloc(MS_U8 u8Eng, MS_U16 *pu16PidFltId)
{
    MS_U16 u16ii;
    *pu16PidFltId = 0xFF;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    for(u16ii = 0; u16ii < TSO_PIDFLT_NUM; u16ii++)
    {
        if(_ptso_res->_FltState[u8Eng][u16ii] == E_DRVTSO_FLT_STATE_FREE)
        {
            TSO_DBGERR(ULOGE("TSO", "Allocate PID Filter %d\n", u16ii));
            _ptso_res->_FltState[u8Eng][u16ii] = E_DRVTSO_FLT_STATE_ALLOC;
            *pu16PidFltId = u16ii;
            _TSO_RETURN(DRVTSO_OK);
        }
    }

    _TSO_RETURN(DRVTSO_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Free TSO PID filter
/// @param u16PidFltId                           \b IN: PID filter ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_Free(MS_U8 u8Eng, MS_U16 u16PidFltId)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(_ptso_res->_FltState[u8Eng][u16PidFltId] == E_DRVTSO_FLT_STATE_FREE)
    {
        _TSO_RETURN(DRVTSO_OK);
    }

    HAL_TSO_Flt_SetPid(u8Eng, u16PidFltId, TSO_PID_NULL);
    HAL_TSO_Flt_SetInputSrc(u8Eng, u16PidFltId, 0);

    //reset resource parameters
    _ptso_res->_u16FltPid[u8Eng][u16PidFltId] = TSO_PID_NULL;
    _ptso_res->_u32FltChSrc[u8Eng][u16PidFltId] = 0;
    _ptso_res->_FltState[u8Eng][u16PidFltId] = E_DRVTSO_FLT_STATE_FREE;

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to PID filter
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u16PidFltId                       \b IN: PID filter ID
/// @param  u16Pid                             \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_SetPid(MS_U8 u8Eng, MS_U16 u16PidFltId, MS_U16 u16Pid)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(_ptso_res->_FltState[u8Eng][u16PidFltId] != E_DRVTSO_FLT_STATE_ALLOC)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    HAL_TSO_Flt_SetPid(u8Eng, u16PidFltId, u16Pid);
    _ptso_res->_u16FltPid[u8Eng][u16PidFltId] = u16Pid;
    _TSO_RETURN(DRVTSO_OK);
}

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
//-------------------------------------------------------------------------------------------------
/// Set Channel source to PID filter
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u16PidFltId                         \b IN: PID filter ID
/// @param  u8Sid                                \b IN: Service ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_SetSid(MS_U8 u8Eng, MS_U16 u16PidFltId, MS_U8 u8Sid)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(_ptso_res->_FltState[u8Eng][u16PidFltId] != E_DRVTSO_FLT_STATE_ALLOC)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    HAL_TSO_Flt_SetSid(u8Eng, u16PidFltId, u8Sid);
    _TSO_RETURN(DRVTSO_OK);
}
#endif

//-------------------------------------------------------------------------------------------------
/// Set Channel source to PID filter
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u16PidFltId                       \b IN: PID filter ID
/// @param  eIf                                  \b IN: PID filter source from which TSIF
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_SetChSrc(MS_U8 u8Eng, MS_U16 u16PidFltId, DrvTSO_If eIf)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    MS_U16 u16ChSrc = 0;

    if((u16ChSrc = (MS_U16)_TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_FAIL;
    }

    _TSO_ENTRY();

    if(_ptso_res->_FltState[u8Eng][u16PidFltId] != E_DRVTSO_FLT_STATE_ALLOC)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    HAL_TSO_Flt_SetInputSrc(u8Eng, u16PidFltId, u16ChSrc);
    _ptso_res->_u32FltChSrc[u8Eng][u16PidFltId] = u16ChSrc;
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSO Replace PID filter
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  pu16PidFltId                         \b OUT: pointer of PID filter ID for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_RepPidFlt_Alloc(MS_U8 u8Eng, MS_U16 *pu16PidFltId)
{
    MS_U16 u16ii;
    *pu16PidFltId = 0xFF;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    for(u16ii = 0; u16ii < TSO_REP_PIDFLT_NUM; u16ii++)
    {
        if(_ptso_res->_RepFltState[u8Eng][u16ii] == E_DRVTSO_FLT_STATE_FREE)
        {
            TSO_DBGERR(ULOGE("TSO", "Allocate PID Filter %d\n", u16ii));
            _ptso_res->_RepFltState[u8Eng][u16ii] = E_DRVTSO_FLT_STATE_ALLOC;
            *pu16PidFltId = u16ii;
            _TSO_RETURN(DRVTSO_OK);
        }
    }

    _TSO_RETURN(DRVTSO_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Set Original PID and new PID to replace PID filter
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u16PidFltId                       \b IN: PID filter ID
/// @param  eIf                                  \b IN: PID filter source from which TSIF
/// @param  u16OrgPid                        \b IN: Original PID
/// @param  u16NewPid                       \b IN: New PID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_RepPidFlt_Set(MS_U8 u8Eng, MS_U16 u16PidFltId, DrvTSO_If eIf, MS_U16 u16OrgPid, MS_U16 u16NewPid)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    MS_U8 u8ChSrc = 0;
    if((u8ChSrc = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    if(_ptso_res->_RepFltState[u8Eng][u16PidFltId] != E_DRVTSO_FLT_STATE_ALLOC)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    if(HAL_TSO_ReplaceFlt_SetPktPid(u8Eng, u16PidFltId, u8ChSrc, u16OrgPid, u16NewPid) == FALSE)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    _ptso_res->_u16RepFltOrgPid[u8Eng][u16PidFltId] = u16OrgPid;
    _ptso_res->_u16RepFltNewPid[u8Eng][u16PidFltId] = u16NewPid;
    _ptso_res->_u16RepFltChSrc[u8Eng][u16PidFltId] = (MS_U16)u8ChSrc;

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Replace PID filter enable
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  u16PidFltId                          \b IN: PID filter ID
/// @param  bEnable                              \b IN: TRUE is enabled; FALSE is disabled
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_RepPidFlt_Enable(MS_U8 u8Eng, MS_U16 u16PidFltId, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(_ptso_res->_RepFltState[u8Eng][u16PidFltId] != E_DRVTSO_FLT_STATE_ALLOC)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    if(HAL_TSO_ReplaceFlt_Enable(u8Eng, u16PidFltId, bEnable) == TRUE)
    {
        _ptso_res->_RepFltState[u8Eng][u16PidFltId] = E_DRVTSO_FLT_STATE_ENABLE;
        _TSO_RETURN(DRVTSO_OK);
    }
    else
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
/// Free TSO Replace PID filter
/// @param u16PidFltId                           \b IN: PID filter ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_RepPidFlt_Free(MS_U8 u8Eng, MS_U16 u16PidFltId)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(_ptso_res->_RepFltState[u8Eng][u16PidFltId] == E_DRVTSO_FLT_STATE_FREE)
    {
        _TSO_RETURN(DRVTSO_OK);
    }

    if(_ptso_res->_RepFltState[u8Eng][u16PidFltId] == E_DRVTSO_FLT_STATE_ENABLE)
    {
        if(HAL_TSO_ReplaceFlt_Enable(u8Eng, u16PidFltId, FALSE) == FALSE)
        {
            _TSO_RETURN(DRVTSO_FAIL);
        }
        _ptso_res->_RepFltState[u8Eng][u16PidFltId] = E_DRVTSO_FLT_STATE_ALLOC;
    }
    if(HAL_TSO_ReplaceFlt_SetPktPid(u8Eng, u16PidFltId, 0, TSO_PID_NULL, TSO_PID_NULL) == FALSE)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

    //reset resource parameters
    _ptso_res->_u16RepFltOrgPid[u8Eng][u16PidFltId] = TSO_PID_NULL;
    _ptso_res->_u16RepFltNewPid[u8Eng][u16PidFltId] = TSO_PID_NULL;
    _ptso_res->_u16RepFltChSrc[u8Eng][u16PidFltId] = 0;
    _ptso_res->_RepFltState[u8Eng][u16PidFltId] = E_DRVTSO_FLT_STATE_FREE;

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Command Queue Reset
/// @param  u8FileEng                            \b IN: Finein Engine
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQ_Reset(MS_U8 u8FileEng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_CmdQ_Reset(0, u8FileEng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Write Level value
/// @param  u8FileEng                            \b IN: Finein Engine
/// @param  pu8FifoLevel                       \b OUT: pointer of Command Queue Write Level value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQFifo_Status(MS_U8 u8FileEng, MS_U8 *pu8FifoLevel)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    *pu8FifoLevel = HAL_TSO_CmdQ_FIFO_Get_WRLevel(0, u8FileEng);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Empty Slot value
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  pu32EmptySlot                       \b OUT: pointer of Command Queue Empty Slot value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQ_GetSlot(MS_U8 u8FileEng, MS_U32 *pu32EmptySlot)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    *pu32EmptySlot = 0;
    *pu32EmptySlot = (MS_U32)((TSO_CMDQ_SIZE - HAL_TSO_CmdQ_FIFO_Get_WRCnt(0, u8FileEng)) & 0xFFFF);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set playback timestamp
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  u32timestamp                        \b IN: timestamp value
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetPlaybackTimeStamp(MS_U8 u8FileEng, MS_U32 u32timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_LPcr2_Set(0, u8FileEng, u32timestamp);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get playback timestamp
/// @param  u8Eng                                    \b IN: TSO engine ID
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param pu32timestamp                        \b OUT: pointer of timestamp value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetPlaybackTimeStamp(MS_U8 u8FileEng, MS_U32 *pu32timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pu32timestamp != 0));

    _TSO_ENTRY();
    *pu32timestamp = HAL_TSO_LPcr2_Get(0, u8FileEng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get File-in timestamp value
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  pu32Timestamp                       \b OUT: pointer of Timestamp value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetFileInTimeStamp(MS_U8 u8FileEng, MS_U32* pu32Timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pu32Timestamp != 0));

    _TSO_ENTRY();
    *pu32Timestamp = HAL_TSO_TimeStamp_Get(0, u8FileEng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get TSO File-in read address
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  pphyReadAddr                        \b OUT: Pointer to store file-in current reand pointer
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_GetReadAddr(MS_U8 u8FileEng, MS_PHY *pphyReadAddr)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pphyReadAddr != 0));

    _TSO_ENTRY();
    *pphyReadAddr = HAL_TSO_Get_Filein_ReadAddr(0, u8FileEng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Start Address
/// @param  u8FileEng                                  \b IN: Finein Engine
/// @param  phyStreamAddr                       \b IN: MIU Physical Address of File-in Read Start Address.
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_SetAddr(MS_U8 u8FileEng, MS_PHY phyStreamAddr)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_ReadAddr(0, u8FileEng, phyStreamAddr);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Packet Mode
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  ePktMode                                \b IN: Select packet mode
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetPacketMode(MS_U8 u8FileEng, DrvTSO_PacketMode ePktMode)
{
    //MS_U16 u16OutPktSize = 0xBB;
    MS_U16 u16ValidCount = 0xBB;
    MS_U16 u16InValidCount = 0x0;
    MS_U8 u8Size = 0;

    switch (ePktMode)
    {
        case E_DRVTSO_PKTMODE_188:
            u8Size = 0xBB;
            break;
        case E_DRVTSO_PKTMODE_192:
            u8Size = 0xBF;
            break;
        case E_DRVTSO_PKTMODE_204:
            u8Size = 0xCB;
            break;
        default:
            return DRVTSO_INVALID_PARAM;
    }

    u16ValidCount = (MS_U16)u8Size;

    _TSO_ENTRY();

    HAL_TSO_Filein_PktChkSize_Set(0, u8FileEng, u8Size);
    HAL_TSO_RW_ValidBlock_Count(0, TRUE, &u16ValidCount);
    HAL_TSO_RW_InvalidBlock_Count(0, TRUE, &u16InValidCount);
    ///HAL_TSO_RW_OutputPktSize(0, TRUE, &u16OutPktSize);

    _TSO_RETURN(DRVTSO_OK);

}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Size
/// @param  u8FileEng                                \b IN: Finein Engine
/// @param  u32StreamSize                        \b IN: File-in Read Size.
/// @return TSO_Result
//   @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_SetSize(MS_U8 u8FileEng, MS_U32 u32StreamSize)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if(u32StreamSize <= 16) //HW limitation
        return DRVTSO_FAIL;

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_ReadLen(0, u8FileEng, u32StreamSize);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Input packet size
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  eIf                                   \b IN: Enum of Input TSO IF
/// @param  u16InPktSize                   \b IN: Input packet size (192, 204, 188)
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Set_InputPktSize(MS_U8 u8Eng, DrvTSO_If eIf, MS_U16 u16InPktSize)
{
    MS_U8  u8Id = 0;
    MS_U16 u16ValidCount = u16InPktSize - 1;
    MS_U16 u16InValidCount = 0x0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if((u8Id = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    if(eIf >= E_DRVTSO_IF_FILE0)
    {
        u8Id = (MS_U8)(eIf - E_DRVTSO_IF_FILE0);
        HAL_TSO_Filein_PktChkSize_Set(u8Eng, u8Id, u16InPktSize-1);
    }
    else
    {
        HAL_TSO_PktChkSize_Set(u8Eng, u8Id, u16InPktSize-1);
    }
    HAL_TSO_RW_ValidBlock_Count(0, TRUE, &u16ValidCount);
    HAL_TSO_RW_InvalidBlock_Count(0, TRUE, &u16InValidCount);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set/Get output packet size
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  u16OutPktSize                 \b IN: Output packet size (192, 204, 188)
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_OutputPktSize(MS_U8 u8Eng, MS_U16* pu16OutPktSize, MS_BOOL bSet)
{
    MS_U16 u16PktMode = HAL_TSO_SVQRX_MODE_CIP;
    MS_U16 u16OutSize = *pu16OutPktSize-1;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if(bSet == TRUE)
    {
        switch(*pu16OutPktSize)
        {
            case 188:
                u16PktMode = HAL_TSO_SVQRX_MODE_CIP;
                break;
            case 192:
                u16PktMode = HAL_TSO_SVQRX_MODE_192;
                break;
            case 200:
                u16PktMode = HAL_TSO_SVQRX_MODE_OPENCABLE;
                break;
            default:
                return DRVTSO_INVALID_PARAM;
        }
        _TSO_ENTRY();
        HAL_TSO_Set_SVQRX_PktMode(u8Eng, u16PktMode);
        HAL_TSO_RW_OutputPktSize(u8Eng, TRUE, &u16OutSize);
        _TSO_RETURN(DRVTSO_OK);
    }
    else
    {
        HAL_TSO_RW_OutputPktSize(u8Eng, FALSE, &u16OutSize);
        *pu16OutPktSize = u16OutSize + 1;
        return DRVTSO_OK;
    }
}

//-------------------------------------------------------------------------------------------------
/// TSO configuration
/// @param  u32TsoEng                      \b IN: TSO engine ID
/// @param eCfgCmd                         \b IN: Configure command.
/// @param pu32CmdData0                    \b IN/OUT: General data parameter depend on eCfgCmd.
/// @param bSet                            \b IN: TRUE: (set), FALSE: (get)
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Configure(DrvTSO_Cfg* pstTsoCfg)
{
    TSO_DBGERR(printf("[%d]%s\n", __LINE__, __FUNCTION__));

    TSO_Result eRes = DRVTSO_OK;
    MS_U16 u16OpifValue = (MS_U16)pstTsoCfg->u32CmdData0;

    _TSO_ENTRY();

    switch(pstTsoCfg->eCfgCmd)
    {
        case E_DRVTSO_CFG_OPIF:
            HAL_TSO_RW_OutputPktSize((MS_U8)pstTsoCfg->u32TsoEng, pstTsoCfg->bSet, &u16OpifValue);
            break;
        default:
            printf("Not supported TSO configure Cmd = 0x%x! \n", pstTsoCfg->eCfgCmd);
            eRes = DRVTSO_INVALID_PARAM;
            break;
    }

    if(pstTsoCfg->bSet == FALSE)
    {
        switch(pstTsoCfg->eCfgCmd)
        {
            case E_DRVTSO_CFG_OPIF:
                pstTsoCfg->u32CmdData0 = ((MS_U32)u16OpifValue) & 0xFFFFUL;
                break;
            default:
                eRes = DRVTSO_INVALID_PARAM;
                break;
        }
    }

    _TSO_RETURN(eRes);

}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Mode
/// @param  u8FileEng                       \b IN: Finein Engine
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_192Mode_En(MS_U8 u8FileEng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Filein_192Mode_Enable(0, u8FileEng, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Block Scheme
/// @param  u8FileEng                       \b IN: Finein Engine
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_192BlockScheme_En(MS_U8 u8FileEng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Filein_192BlockMode_Enable(0, u8FileEng, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Skip TEL packet
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  eIf                                 \b IN: TSO IF
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_TEI_SkipPkt(MS_U8 u8Eng, DrvTSO_If eIf, MS_BOOL bEnable)
{
    MS_U8 u8If = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if((u8If = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();
    HAL_TSO_ChIf_Cfg(u8Eng, u8If, TSO_CHCFG_SKIP_TEI_PKT, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Clear PID filter byte counter
/// @param  u8Eng                            \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Clr_ByteCnt(MS_U8 u8Eng, DrvTSO_If eIf)
{
    MS_U8 u8If = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if((u8If = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();
    HAL_TSO_ChIf_ClrByteCnt(u8Eng, u8If);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get TSO Hardware capability
/// @param  eCap                                 \b IN: Capability item to get
/// @param  pOutput                             \b IN: Pointer to store getting result
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetCap(DrvTSO_Cap eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case E_DRVTSO_CAP_FILTER_NUM:
            *((MS_U32*)pOutput) = TSO_PIDFLT_NUM_ALL;
            break;
        case E_DRVTSO_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = (MS_U32)sizeof(TSO_RESOURCE_PRIVATE);
            break;
        case E_DRVTSO_CAP_ENGINE_NUM:
            *((MS_U32*)pOutput) = TSO_ENGINE_NUM;
            break;
        case E_DRVTSO_CAP_ENGINE_TSIF_NUM:
            *((MS_U32*)pOutput) = TSO_TSIF_NUM;
            break;
        default:
            return DRVTSO_INVALID_PARAM;
    }

    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue state
/// @param  u8FileEng                            \b IN: File engine ID
/// @param  peFileinState                       \b OUT: Pointer of Filein state to return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_GetState(MS_U8 u8FileEng, DrvTSO_FileinState *peFileinState)
{
    MS_U16 u16state = 0xFF, u16cmdcnt = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    u16state = HAL_TSO_Get_Filein_Ctrl(0, u8FileEng);
    u16cmdcnt = HAL_TSO_CmdQ_FIFO_Get_WRCnt(0, u8FileEng);

    if((u16state & TSO_FILEIN_RSTART) || u16cmdcnt)
        *peFileinState = E_DRVTSO_STATE_BUSY;
    else
        *peFileinState = E_DRVTSO_STATE_IDLE;
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue interrupt state
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  u8FileEng                             \b IN: Filein engine
/// @param  pStatus                               \b OUT: Pointer of Filein status to return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_ChkStatus(MS_U8 u8Eng, MS_U8 u8FileEng, DrvTSO_Status* pStatus)
{
    MS_U16 u32temp = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    u32temp = HAL_TSO_HWInt_Status(u8Eng);

    *pStatus = E_DRVTSO_STATUS_UNKNOWN;

    if(u8FileEng == 0)
    {
        if(u32temp & TSO_INT_DMA_DONE)
            *pStatus |= E_DRVTSO_STATUS_FILEINDONE;
    }
    else if(u8FileEng == 1)
    {
        if(u32temp & TSO_INT_DMA_DONE1)
            *pStatus |= E_DRVTSO_STATUS_FILEINDONE;
    }

    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Clear Command queue interrupt state
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  u16int                                 \b OUT: Interrupt bit to clear
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_ClearHWInt(MS_U8 u8Eng, MS_U16 u16int)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_HWInt_Clear(u8Eng, u16int);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Start
/// @param  u8FileEng                             \b IN: Filein engine
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Start(MS_U8 u8FileEng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    //HAL_TSO_FileinTimer_Enable(0, u8FileEng, TRUE);

    //valid cfg parameters
    HAL_TSO_Cfg1_Enable(0, TSO_CFG1_PKT_PARAM_LD, TRUE);
    HAL_TSO_Cfg1_Enable(0, TSO_CFG1_PKT_PARAM_LD, FALSE);

    HAL_TSO_Set_Filein_Ctrl(0, u8FileEng, TSO_FILEIN_RSTART);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Stop
/// @param  u8FileEng                             \b IN: Filein engine
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Stop(MS_U8 u8FileEng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    MS_U8 u8TsIf = ((u8FileEng == 0)?HAL_TSO_TSIF_FILE1 : HAL_TSO_TSIF_FILE2);

    _TSO_ENTRY();

    HAL_TSO_ChIf_Enable(0, u8TsIf, FALSE);
    HAL_TSO_Cfg1_Enable(0, TSO_CFG1_TSO_OUT_EN, FALSE);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Abort
/// @param  u8FileEng                             \b IN: Filein engine
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Abort(MS_U8 u8FileEng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_Ctrl(0, u8FileEng, TSO_FILEIN_ABORT);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_ResetAll(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Reset_All(u8Eng);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_SelPad(MS_U8 u8Eng, DrvTSO_If eIf, DrvTSO_If_Set* pIfSet)
{
    MS_U8 u8ChId = 0;
    MS_U16 u16Pad = 0, u16clk = 0;
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    MS_BOOL bIs3Wire = FALSE;
#endif

    if((u8ChId = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    TSO_DBGERR(ULOGE("TSO", "[%s] eIf %d, ePad %x\n", __FUNCTION__, (int)eIf, (int)pIfSet->ePad));

    switch(pIfSet->ePad)
    {
        case E_DRVTSO_PAD_EXT_INPUT0:
            u16Pad = HAL_TSOIN_MUX_TS0;
            u16clk = TSO_CLKIN_TS0;
            break;
        case E_DRVTSO_PAD_EXT_INPUT1:
            u16Pad = HAL_TSOIN_MUX_TS1;
            u16clk = TSO_CLKIN_TS1;
            break;
        case E_DRVTSO_PAD_EXT_INPUT2:
            u16Pad = HAL_TSOIN_MUX_TS2;
            u16clk = TSO_CLKIN_TS2;
            break;
        case E_DRVTSO_PAD_EXT_INPUT3:
            u16Pad = HAL_TSOIN_MUX_TS3;
            u16clk = TSO_CLKIN_TS3;
            break;
        case E_DRVTSO_PAD_EXT_INPUT4:
            u16Pad = HAL_TSOIN_MUX_TS4;
            u16clk = TSO_CLKIN_TS4;
            break;
        #ifdef TSO_CLKIN_TS5
        case E_DRVTSO_PAD_EXT_INPUT5:
            u16Pad = HAL_TSOIN_MUX_TS5;
            u16clk = TSO_CLKIN_TS5;
            break;
        #endif
        case E_DRVTSO_PAD_DEMOD:
            u16Pad = HAL_TSOIN_MUX_TSDEMOD0;
            u16clk = TSO_CLKIN_DMD;
            break;
    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
        case E_DRVTSO_PAD_EXT_INPUT0_3WIRE:
            HAL_TSO_3WirePadMapping(0, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT1_3WIRE:
            HAL_TSO_3WirePadMapping(1, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT2_3WIRE:
            HAL_TSO_3WirePadMapping(2, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT3_3WIRE:
            HAL_TSO_3WirePadMapping(3, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT4_3WIRE:
            HAL_TSO_3WirePadMapping(4, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT5_3WIRE:
            HAL_TSO_3WirePadMapping(5, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
        case E_DRVTSO_PAD_EXT_INPUT6_3WIRE:
            HAL_TSO_3WirePadMapping(6, &u16Pad, &u16clk);
            bIs3Wire = TRUE;
            break;
    #endif
        default:
            return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    if(HAL_TSO_SelPad(u8Eng, u8ChId, u16Pad, pIfSet->bParallel) == FALSE)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }
    if(HAL_TSO_Set_InClk(u8Eng, u8ChId, u16clk, pIfSet->bClkInv, TRUE) == FALSE)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    if(bIs3Wire)
    {
        HAL_TSO_Livein_3Wire(u8Eng, u8ChId, TRUE);
    }
    else
    {
        HAL_TSO_Livein_3Wire(u8Eng, u8ChId, FALSE);
    }
#endif

    HAL_TSO_ChIf_Cfg(u8Eng, u8ChId, (TSO_CHCFG_TS_SIN_C0 | TSO_CHCFG_TS_SIN_C1), (pIfSet->bParallel == FALSE));
    HAL_TSO_ChIf_Cfg(u8Eng, u8ChId, TSO_CHCFG_P_SEL, pIfSet->bParallel);
    HAL_TSO_ChIf_Cfg(u8Eng, u8ChId, TSO_CHCFG_EXT_SYNC_SEL, pIfSet->bExtSync);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set/Get  TSO output pad
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  eOutPad                             \b IN: Output pad
/// @param  bSet                                  \b IN: TRUE is setting output pad; FALSE is getting output pad
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_OutputPad(MS_U8 u8Eng, DrvTSO_PadOut* peOutPad, MS_BOOL bSet)
{
    MS_U16 u16pad = (MS_U16)(*peOutPad);

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(HAL_TSO_OutPad(u8Eng, &u16pad, bSet) == TRUE)
    {
        _TSO_RETURN(DRVTSO_OK);
    }
    else
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }
}

TSO_Result MDrv_TSO_Set_SVQBuf(MS_U8 u8Eng, MS_PHY phyBufAddr, MS_U32 u32Size)
{
    MS_PHY phyBufStart = phyBufAddr;
    MS_U32 u32VQSize = ((u32Size >> TSO_MIU_BUS) / TSO_TSIF_NUM) << TSO_MIU_BUS;
    MS_U8  u8ii, u8ChId;

    _TSO_ENTRY();

    //TSO LIVE-IN
    for(u8ii = 0; u8ii < TSO_TSIF_NUM; u8ii++)
    {
        u8ChId = _TSO_TSIF2ChID(u8ii);
        HAL_TSO_SVQBuf_Set(u8Eng, u8ChId, phyBufStart, u32VQSize);
        HAL_TSO_SVQ_TX_Reset(u8Eng, u8ChId);
        phyBufStart += u32VQSize;
    }

    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32timestamp)
{
    _TSO_ENTRY();
    HAL_TSO_Set_SVQ_LocalSysTimestamp(u8Eng, u32timestamp);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_LocalStreamId(MS_U8 u8Eng, DrvTSO_If eIf, MS_U8 *pu8Id, MS_BOOL bSet)
{
    MS_U8 u8ChId = 0;

    if((u8ChId = _TSO_TSIF2ChID(eIf)) == 0xFF)
    {
        return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    if(HAL_TSO_LocalStreamID(u8Eng, u8ChId, pu8Id, bSet) == FALSE)
    {
        _TSO_RETURN(DRVTSO_FAIL);
    }
    else
    {
        _TSO_RETURN(DRVTSO_OK);
    }
}

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
TSO_Result MDrv_TSO_TsioMode_En(MS_U8 u8Eng, MS_BOOL bEnable)
{
    _TSO_ENTRY();
    HAL_TSO_TsioMode_En(bEnable);
    _TSO_RETURN(DRVTSO_OK);
}
#endif

MS_BOOL MDrv_TSO_Check_IsSVQTxBusy(MS_U8 u8Eng, DrvTSO_If eIf)
{
    MS_U8  u8ChId = _TSO_TSIF2ChID(eIf);
    MS_U16 u16Status = 0;

    HAL_TSO_Get_SVQ_Status(u8Eng, u8ChId, &u16Status);

    if((u16Status & TSO_SVQ_STS_BUSY) == TSO_SVQ_STS_BUSY)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MDrv_TSO_Check_IsSVQTxEmpty(MS_U8 u8Eng, DrvTSO_If eIf)
{
    MS_U8  u8ChId = _TSO_TSIF2ChID(eIf);
    MS_U16 u16Status = 0;

    HAL_TSO_Get_SVQ_Status(u8Eng, u8ChId, &u16Status);

    if((u16Status & TSO_SVQ_STS_EMPTY) == TSO_SVQ_STS_EMPTY)
        return TRUE;
    else
        return FALSE;
}

#ifdef TSO_PVR_SUPPORT
TSO_Result MDrv_TSO_PVR_SelSrc(MS_U8 u8PVRId, DrvTSO_PvrSrc ePvrSrc)
{
    _TSO_ENTRY();
    if(ePvrSrc == E_DRVTSO_PVR_SRC_SVQ)
    {
        HAL_TSO_PVR_Src(HAL_TSO_PVR_SVQ);
    }
    else if(ePvrSrc == E_DRVTSO_PVR_SRC_MMT)
    {
        HAL_TSO_PVR_Src(HAL_TSO_PVR_MMT);
    }
    else
    {
        HAL_TSO_PVR_Src(HAL_TSO_PVR_SVQ);
    }

    _TSO_RETURN(DRVTSO_OK);

}

TSO_Result MDrv_TSO_PVR_SetBuf(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    _TSO_ENTRY();
    HAL_TSO_PVR_SetBuffer(u8PVRId, phyBufStart0, phyBufStart1, u32BufSize0, u32BufSize1);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_SetRecordStampClk(MS_U8 u8PVRId, MS_U32 u32Clk)
{
    _TSO_ENTRY();
    HAL_TSO_SetPVRTimeStampClk(u8PVRId, u32Clk);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_SetPacketMode(MS_U8 u8PVRId, MS_BOOL bSet)
{
    _TSO_ENTRY();
    HAL_TSO_PVR_PacketMode(u8PVRId, bSet);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_TimeStampSetRecordStamp(MS_U8 u8PVRId, MS_U32 u32Stamp)
{
    _TSO_ENTRY();
    HAL_TSO_SetPVRTimeStamp(u8PVRId, u32Stamp);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_TimeStampGetRecordStamp(MS_U8 u8PVRId, MS_U32* pu32Stamp)
{
    _TSO_ENTRY();
    *pu32Stamp = HAL_TSO_GetPVRTimeStamp(u8PVRId);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_Start(MS_U8 u8PVRId, DrvTSO_RecMode eRecMode, MS_BOOL bStart)
{
    _TSO_ENTRY();
    HAL_TSO_PVR_Enable(u8PVRId, bStart);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_PVR_GetWriteAddr(MS_U8 u8PVRId, MS_PHY *pphy2WriteAddr)
{
    _TSO_ENTRY();
    *pphy2WriteAddr = HAL_TSO_PVR_GetBufWrite(u8PVRId);
    _TSO_RETURN(DRVTSO_OK);
}

#endif  //TSO_PVR_SUPPORT


#ifdef MSOS_TYPE_LINUX_KERNEL
TSO_Result MDrv_TSO_Suspend(void)
{
    HAL_TSO_SaveRegs();
    HAL_TSO_PowerCtrl(FALSE);

    return DRVTSO_OK;
}

TSO_Result MDrv_TSO_Resume(void)
{
    MS_U16 u16ii, u16jj;

    HAL_TSO_Reset_All(0);

    HAL_TSO_RestoreRegs();

    //restart filter
    for(u16ii = 0; u16ii < TSO_ENGINE_NUM; u16ii++)
    {
        for(u16jj = 0; u16jj < TSO_PIDFLT_NUM; u16jj++)
        {
            if(_ptso_res->_FltState[u16ii][u16jj] == E_DRVTSO_FLT_STATE_ALLOC)
            {
                HAL_TSO_Flt_SetPid((MS_U8)u16ii, u16jj, _ptso_res->_u16FltPid[u16ii][u16jj]);
                HAL_TSO_Flt_SetInputSrc((MS_U8)u16ii, u16jj, _ptso_res->_u32FltChSrc[u16ii][u16jj]);
            }
        }
    }

    return DRVTSO_OK;
}
#endif  //MSOS_TYPE_LINUX_KERNEL

