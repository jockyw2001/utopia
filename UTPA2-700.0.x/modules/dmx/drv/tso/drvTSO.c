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
    MS_BOOL                      _bFilterUsed[TSO_ENGINE_NUM][TSO_PIDFLT_NUM];

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
        TSO_DBGINFO((ULOGD("TSO", "TSO Base Bank 0x%x\n", (unsigned int)virtBank)));
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
    MS_U8 u8ii, u8jj;

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

    HAL_TSO_Init();

    for(u8jj = 0; u8jj < (MS_U8)TSO_ENGINE_NUM; u8jj++)
    {
        HAL_TSO_Reset_All(u8jj);

        for(u8ii = 0; u8ii < (MS_U8)TSO_PIDFLT_NUM; u8ii++)
        {
            _ptso_res->_bFilterUsed[u8jj][u8ii] = FALSE;
        }

        //HAL_TSO_HWInt_Enable(u8jj, TSO_INT_SRC_MASK, TRUE);
        HAL_TSO_Filein_Rate(u8jj, 0x0A);
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

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    for(u8jj = 0; u8jj < TSO_ENGINE_NUM; u8jj++)
    {
        HAL_TSO_Cfg1_Enable(u8jj, TSO_CFG1_PKT_NULL_EN, FALSE);
        HAL_TSO_Cfg4_Enable(u8jj, TSO_CFG4_TS_OUT_EN, FALSE);

        HAL_TSO_Set_InClk(u8jj, 0, FALSE, FALSE);
        HAL_TSO_Set_OutClk(u8jj, 0, FALSE, FALSE);

        for(u8ii = 0; u8ii < TSO_PIDFLT_NUM; u8ii++)
        {
            _ptso_res->_bFilterUsed[u8jj][u8ii] = FALSE;
        }
    }

    MsOS_ReleaseMutex(_ptso_res->_s32TSOMutexId);
    MsOS_DeleteMutex(_ptso_res->_s32TSOMutexId);
    _ptso_res->_s32TSOMutexId = -1;
    _ptso_res->_bSWInit = FALSE;

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
/// @param  TSOCtrlMode                     \b IN: Ctrol mode
/// @param  bClkInv                            \b IN: TSO_IN_Clk Invert
/// @param  bParallel                           \b IN: TSO_IN_Interface paralleled
/// @return TSO_Result
//-------------------------------------------------------------------------------------------------
#ifdef TSO_HW_BOX_SERIES
TSO_Result MDrv_TSO_SetOperateMode(MS_U8 u8Eng, DrvTSO_CtrlMode TSOCtrlMode, MS_BOOL bClkInv, MS_BOOL bParallel)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((u8Eng < TSO_ENGINE_NUM));
    _TSO_ENTRY();

    HalTSOOutClk stTsoOutClk = {0xFFFF, 0xFFFF, 0, 0xFFFF, bClkInv, TRUE};

    HAL_TSO_PktChkSize_Set(u8Eng, 0xBB);
    HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0xBB);
    HAL_TSO_Output_PktChkSize_Set(u8Eng, 0xBB);

    MS_U16 u16InPad = HAL_TSOIN_MUX_TS0;
    MS_U16 u16InClk = HAL_TSO_CLKIN_TS0;
    switch(TSOCtrlMode)
    {
        case E_DRVTSO_CTRL_MODE_TS0:
            u16InPad = HAL_TSOIN_MUX_TS0;
            u16InClk = HAL_TSO_CLKIN_TS0;
            break;
        case E_DRVTSO_CTRL_MODE_TS1:
            u16InPad = HAL_TSOIN_MUX_TS1;
            u16InClk = HAL_TSO_CLKIN_TS1;
            break;
        case E_DRVTSO_CTRL_MODE_TS2:
            u16InPad = HAL_TSOIN_MUX_TS2;
            u16InClk = HAL_TSO_CLKIN_TS2;
            break;
        case E_DRVTSO_CTRL_MODE_TS3:
            u16InPad = HAL_TSOIN_MUX_TS3;
            u16InClk = HAL_TSO_CLKIN_TS3;
            break;
        case E_DRVTSO_CTRL_MODE_TSCB:
            u16InPad = HAL_TSOIN_MUX_TSCB;
            u16InClk = HAL_TSO_CLKIN_TSCB;
            break;
        case E_DRVTSO_CTRL_MODE_DEMOD0:
            u16InPad = HAL_TSOIN_MUX_TSDEMOD0;
            u16InClk = HAL_TSO_CLKIN_TSDEMOD0;
            break;
        case E_DRVTSO_CTRL_MODE_DEMOD1:
            u16InPad = HAL_TSOIN_MUX_TSDEMOD1;
            u16InClk = HAL_TSO_CLKIN_TSDEMOD1;
            break;
        case E_DRVTSO_CTRL_MODE_MEM:
            u16InPad = HAL_TSOIN_MUX_MEM;
            u16InClk = 0x0;
            break;

        default:
            ULOGE("TSO", "Not supported TSOCtrlMode = %d! \n", TSOCtrlMode);
            _TSO_RETURN(DRVTSO_INVALID_PARAM);
    }

    HAL_TSO_OutClk_DefSelect(u8Eng, u16InPad, FALSE, &stTsoOutClk);
    HAL_TSO_SelPad(u8Eng, u16InPad, bParallel);
    //HAL_TSO_Set_InClk(u8Eng, u16InClk, FALSE, FALSE);
    HAL_TSO_ConfigOutputClk(u8Eng, &stTsoOutClk);

    if(TSOCtrlMode == E_DRVTSO_CTRL_MODE_MEM)
    {
        HAL_TSO_Set_InClk(u8Eng, u16InClk, FALSE, FALSE);
        HAL_TSO_Cfg0_Enable(u8Eng, TSO_CFG0_DATA_CHK_2T_EN|TSO_CFG0_SOURCE_FILE|TSO_CFG0_FILEIN_ENABLE, TRUE);
        HAL_TSO_Cfg0_Enable(u8Eng, TSO_CFG0_TS_SIN_C0_EN, FALSE);
        HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_ALT_TS_SIZE|TSO_CFG1_PID_BYPASS|TSO_CFG1_PKT_NULL_EN, TRUE);
    }
    else
    {
        MS_U16 u16Cfg = TSO_CFG0_EXTERNAL_SYNC_SEL | TSO_CFG0_SOURCE_LIVE;
        if(bParallel)
            u16Cfg |= TSO_CFG0_PARALLEL_SEL;

        HAL_TSO_Set_InClk(u8Eng, u16InClk, FALSE, TRUE);
        HAL_TSO_Cfg0_Enable(u8Eng, u16Cfg, TRUE);
        HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_FORCE_SYNC_EN|TSO_CFG1_PID_BYPASS|TSO_CFG1_ALT_TS_SIZE, TRUE);
    }

    HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_TS_OUT_EN, TRUE);

    _TSO_RETURN(DRVTSO_OK);
}
#else
TSO_Result MDrv_TSO_SetOperateMode(MS_U8 u8Eng, DrvTSO_CtrlMode TSOCtrlMode, MS_BOOL bClkInv, MS_BOOL bParallel)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((u8Eng < TSO_ENGINE_NUM));
    _TSO_ENTRY();

    HAL_TSO_PktChkSize_Set(u8Eng, 0xBB);
    HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0xBB);
    HAL_TSO_Output_PktChkSize_Set(u8Eng, 0xBB);

    if(TSOCtrlMode == E_DRVTSO_CTRL_MODE_MEM)
    {
        HAL_TSO_Cfg0_Enable(u8Eng, TSO_CFG0_DATA_CHK_2T_EN|TSO_CFG0_SOURCE_FILE|TSO_CFG0_FILEIN_ENABLE, TRUE);
        HAL_TSO_Cfg0_Enable(u8Eng, TSO_CFG0_TS_SIN_C0_EN, FALSE);
        HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_ALT_TS_SIZE|TSO_CFG1_PID_BYPASS|TSO_CFG1_PKT_NULL_EN, TRUE);
    }
    else if(TSOCtrlMode == E_DRVTSO_CTRL_MODE_TS0)
    {
        HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_FORCE_SYNC_EN|TSO_CFG1_PID_BYPASS|TSO_CFG1_ALT_TS_SIZE, TRUE);
    }
    else
    {
        _TSO_RETURN(DRVTSO_INVALID_PARAM);
    }

    HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_TS_OUT_EN, TRUE);

    _TSO_RETURN(DRVTSO_OK);
}

#endif

//-------------------------------------------------------------------------------------------------
/// Set TSO File-in Byte Timer
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  u16timer                            \b IN: Timer value
/// @param  bEnable                             \b IN: Enable flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Rate(MS_U8 u8Eng, MS_U16 u16timer)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    
    HAL_TSO_Filein_Rate(u8Eng, u16timer);
    if(u16timer == 0)
    {
        HAL_TSO_FileinTimer_Enable(u8Eng, FALSE);
    }
    else
    {
        HAL_TSO_FileinTimer_Enable(u8Eng, TRUE);
    }
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

#ifndef TSO_HW_BOX_SERIES

    //ULOGD("TSO", "[%s] eOutClk %x, eOutClkSrc %x, u16DivNum %x \n", __FUNCTION__, eOutClk, eOutClkSrc, u16DivNum);

    MS_U16 u16OutClkSrc = E_DRVTSO_OUTCLK_DIV2N;
    HalTSOOutClk stTsoOutClk = {0xFFFF, 0xFFFF, u16DivNum, 0xFFFF, bClkInv, TRUE};

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
        default:
            _TSO_RETURN(DRVTSO_INVALID_PARAM);
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
            stTsoOutClk.u16OutDivSrc = u16OutClkSrc;
            break;
        case E_DRVTSO_OUTCLK_PTSOOUT_DIV8:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8;
            stTsoOutClk.u16OutDivSrc = u16OutClkSrc;
            break;
        case E_DRVTSO_OUTCLK_Dmd:
            stTsoOutClk.u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD;
            break;
        default:
            _TSO_RETURN(DRVTSO_INVALID_PARAM);
    }

    //ULOGD("TSO", "[%s] stTsoOutClk eOutClk %x, eOutClkSrc %x, u16DivNum %x \n", __FUNCTION__, stTsoOutClk.u16OutClk, stTsoOutClk.u16OutDivSrc, stTsoOutClk.u16OutDivNum);

    HAL_TSO_ConfigOutputClk(u8Eng, &stTsoOutClk);
#endif

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSO PID filter
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  pu8PidFltId                         \b OUT: pointer of PID filter ID for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_Alloc(MS_U8 u8Eng, MS_U8 *pu8PidFltId)
{
    MS_U8 u8ii;
    *pu8PidFltId = 0xFF;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    for(u8ii = 0; u8ii < TSO_PIDFLT_NUM; u8ii++)
    {
        if(_ptso_res->_bFilterUsed[u8Eng][u8ii] == FALSE)
        {
            TSO_DBGERR(ULOGE("TSO", "Allocate PID Filter %d\n", (int)u8ii));
            _ptso_res->_bFilterUsed[u8Eng][u8ii] = TRUE;
            *pu8PidFltId = u8ii;
            _TSO_RETURN(DRVTSO_OK);
        }
    }

    _TSO_RETURN(DRVTSO_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Free TSO PID filter
/// @param u8PidFltId                           \b IN: PID filter ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_Free(MS_U8 u8Eng, MS_U8 u8PidFltId)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if(_ptso_res->_bFilterUsed[u8Eng][u8PidFltId] == FALSE)
        return DRVTSO_OK;

    _TSO_ENTRY();
    HAL_TSO_Flt_SetPid(u8Eng, u8PidFltId, TSO_PID_NULL);
    _ptso_res->_bFilterUsed[u8Eng][u8PidFltId] = FALSE;
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to PID filter
/// @param  u8PidFltId                       \b IN: PID filter ID
/// @param  u16Pid                           \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_PidFlt_SetPid(MS_U8 u8Eng, MS_U8 u8PidFltId, MS_U16 u16Pid)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Flt_SetPid(u8Eng, u8PidFltId, u16Pid);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Command Queue Reset
/// @param  u8Eng                                \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQ_Reset(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Reset_SubItem(u8Eng, TSO_SW_RSTZ_CMDQ);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Write Level value
/// @param  u8Eng                                \b IN: TSO engine ID
/// @param  pu8FifoLevel                       \b OUT: pointer of Command Queue Write Level value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQFifo_Status(MS_U8 u8Eng, MS_U8 *pu8FifoLevel)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    *pu8FifoLevel = HAL_TSO_CmdQ_FIFO_Get_WRLevel(u8Eng);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command Queue Empty Slot value
/// @param  u8Eng                                    \b IN: TSO engine ID
/// @param  pu32EmptySlot                       \b OUT: pointer of Command Queue Empty Slot value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_CmdQ_GetSlot(MS_U8 u8Eng, MS_U32 *pu32EmptySlot)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    *pu32EmptySlot = 0;
    *pu32EmptySlot = (MS_U32)((TSO_CMDQ_SIZE - HAL_TSO_CmdQ_FIFO_Get_WRCnt(u8Eng)) & 0xFFFF);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set playback timestamp
/// @param  u8Eng                                    \b IN: TSO engine ID
/// @param  u32timestamp                        \b IN: timestamp value
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_LPcr2_Set(u8Eng, u32timestamp);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get playback timestamp
/// @param  u8Eng                                    \b IN: TSO engine ID
/// @param pu32timestamp                        \b OUT: pointer of timestamp value for return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 *pu32timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pu32timestamp != 0));

    _TSO_ENTRY();
    *pu32timestamp = HAL_TSO_LPcr2_Get(u8Eng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get File-in timestamp value
/// @param  u8Eng                                     \b IN: TSO engine ID
/// @param  pu32Timestamp                       \b OUT: pointer of Timestamp value for return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32* pu32Timestamp)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pu32Timestamp != 0));
    *pu32Timestamp = HAL_TSO_TimeStamp_Get(u8Eng);
    return DRVTSO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO File-in read address
/// @param  u8Eng                                     \b IN: TSO engine ID
/// @param  pu32ReadAddr                        \b OUT: Pointer to store file-in current reand pointer
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_PHY *pu32ReadAddr)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));
    TSO_ASSERT((pu32ReadAddr != 0));

    _TSO_ENTRY();
    *pu32ReadAddr = HAL_TSO_Get_Filein_ReadAddr(u8Eng);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Start Address
/// @param  u8Eng                                     \b IN: TSO engine ID
/// @param  u32StreamAddr                       \b IN: MIU Physical Address of File-in Read Start Address.
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_SetAddr(MS_U8 u8Eng, MS_PHY u32StreamAddr)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_ReadAddr(u8Eng, u32StreamAddr);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set File-in Packet Mode
/// @param  u8Eng                                     \b IN: TSO engine ID
/// @param  ePktMode                                \b IN: Select packet mode
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetPacketMode(MS_U8 u8Eng, DrvTSO_PacketMode ePktMode)
{
    MS_U16 u16OutPktSize = 0xBB;
    MS_U16 u16ValidCount = 0xBB;
    MS_U16 u16InValidCount = 0x0;

    _TSO_ENTRY();

    switch (ePktMode)
    {
        case E_DRVTSO_PKTMODE_188:
            HAL_TSO_PktChkSize_Set(u8Eng, 0xBB);
            HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0xBB);
            break;
        case E_DRVTSO_PKTMODE_192:
            HAL_TSO_PktChkSize_Set(u8Eng, 0xBF);
            HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0xBF);
            break;
        case E_DRVTSO_PKTMODE_204:
            HAL_TSO_PktChkSize_Set(u8Eng, 0xCB);
            HAL_TSO_Filein_PktChkSize_Set(u8Eng, 0xCB);
            break;
        default:
            break;
    }

    HAL_TSO_RW_ValidBlock_Count(u8Eng, TRUE, &u16ValidCount);
    HAL_TSO_RW_InvalidBlock_Count(u8Eng, TRUE, &u16InValidCount);
    HAL_TSO_RW_OutputPKTSize(u8Eng, TRUE, &u16OutPktSize);

    _TSO_RETURN(DRVTSO_OK);

}

//-------------------------------------------------------------------------------------------------
/// Set File-in Read Size
/// @param  u8Eng                                     \b IN: TSO engine ID
/// @param  u32StreamSize                        \b IN: File-in Read Size.
/// @return TSO_Result
//   @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_SetSize(MS_U8 u8Eng, MS_U32 u32StreamSize)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    if(u32StreamSize <= 16) //HW limitation
        return DRVTSO_FAIL;

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_ReadLen(u8Eng, u32StreamSize);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set packet size
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  bFileinMode                    \b IN: If TRUE, means file-in mode setting, otherwise means live mode setting
/// @param  u16InPktSize                  \b IN: Input packet size (192, 204, 188)
/// @param  u16OutPktSize                \b IN: Output packet size (192, 204, 188)
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_SetPacketSize(MS_U8 u8Eng, MS_BOOL bFileinMode, MS_U16 u16InPktSize, MS_U16 u16OutPktSize)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    if(bFileinMode)
        HAL_TSO_Filein_PktChkSize_Set(u8Eng, u16InPktSize-1);
    else
        HAL_TSO_PktChkSize_Set(u8Eng, u16InPktSize-1);

    HAL_TSO_Output_PktChkSize_Set(u8Eng, u16OutPktSize-1);

    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Mode
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_192Mode_En(MS_U8 u8Eng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_PKT192_ENABLE, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable File-in 192 Block Scheme
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_192BlockScheme_En(MS_U8 u8Eng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_PKT192_BLOCK_DISABLE, (!bEnable));
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Skip TEL packet
/// @param  u8Eng                            \b IN: TSO engine ID
/// @param  bEnable                         \b IN: Enable Flag
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_TEI_SkipPkt(MS_U8 u8Eng, MS_BOOL bEnable)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TEI_SKIP_PKT, bEnable);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Clear PID filter byte counter
/// @param  u8Eng                            \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Clr_Pidflt_ByteCnt(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_CLEAR_PIDFLT_BYTE_CNT, TRUE);
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_CLEAR_PIDFLT_BYTE_CNT, FALSE);
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
            *((MS_U32*)pOutput) = sizeof(TSO_RESOURCE_PRIVATE);
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
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  peFileinState                       \b OUT: Pointer of Filein state to return
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_GetState(MS_U8 u8Eng, DrvTSO_FileinState *peFileinState)
{
    MS_U16 u16state = 0xFF, u16cmdcnt = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    u16state = HAL_TSO_Get_Filein_Ctrl(u8Eng);
    u16cmdcnt = HAL_TSO_CmdQ_FIFO_Get_WRCnt(u8Eng);

    if((u16state & TSO_FILEIN_RSTART) || u16cmdcnt)
        *peFileinState = E_DRVTSO_STATE_BUSY;
    else
        *peFileinState = E_DRVTSO_STATE_IDLE;
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue interrupt state
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @param  pStatus                               \b OUT: Pointer of Filein status to return
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_ChkStatus(MS_U8 u8Eng, DrvTSO_Status* pStatus)
{
    MS_U16 u32temp = 0;

    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    u32temp = HAL_TSO_HWInt_Status(u8Eng);

    *pStatus = E_DRVTSO_STATUS_UNKNOWN;
    if(u32temp & TSO_INT_SRC_DMA_DOME)
        *pStatus |= E_DRVTSO_STATUS_FILEINDONE;
    if(u32temp & TSO_INT_SRC_PKT_ERR_FLAG)
        *pStatus |= E_DRVTSO_STATUS_PKTERR;
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
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Start(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    //HAL_TSO_FileinTimer_Enable(u8Eng, FALSE);
    //valid cfg parameters
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_PKT_WAVEFORM_PARAM_LD, TRUE);
    HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_PKT_WAVEFORM_PARAM_LD, FALSE);

    HAL_TSO_Set_Filein_Ctrl(u8Eng, TSO_FILEIN_RSTART);
    _TSO_RETURN(DRVTSO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Control Filein to Stop
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Stop(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();

    HAL_TSO_Recover_TSOutMode(u8Eng);
    HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_TS_OUT_EN, FALSE);

    _TSO_RETURN(DRVTSO_OK);
}


//-------------------------------------------------------------------------------------------------
/// Control Filein to Abort
/// @param  u8Eng                                 \b IN: TSO engine ID
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSO_Result MDrv_TSO_Filein_Abort(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Set_Filein_Ctrl(u8Eng, TSO_FILEIN_ABORT);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_ResetAll(MS_U8 u8Eng)
{
    TSO_DBGERR(ULOGE("TSO", "[%d]%s\n", __LINE__, __FUNCTION__));

    _TSO_ENTRY();
    HAL_TSO_Reset_All(u8Eng);
    _TSO_RETURN(DRVTSO_OK);
}

TSO_Result MDrv_TSO_SelPad(MS_U8 u8Eng, DrvTSO_If_Set* pIfSet)
{
#ifndef TSO_HW_BOX_SERIES
    MS_U16 u16Pad = 0;
    MS_U16 u16clk = 0, u16Cfg = 0;

    switch(pIfSet->ePad)
    {
        case E_DRVTSO_PAD_EXT_INPUT0:
            u16Pad = HAL_TSOIN_MUX_TS0;
            u16clk = HAL_TSO_CLKIN_TS0;
            break;
        case E_DRVTSO_PAD_EXT_INPUT1:
            u16Pad = HAL_TSOIN_MUX_TS1;
            u16clk = HAL_TSO_CLKIN_TS1;
            break;
        case E_DRVTSO_PAD_EXT_INPUT2:
            u16Pad = HAL_TSOIN_MUX_TS2;
            u16clk = HAL_TSO_CLKIN_TS2;
            break;
        case E_DRVTSO_PAD_DEMOD:
            u16Pad = HAL_TSOIN_MUX_TSDEMOD0;
            u16clk = HAL_TSO_CLKIN_TSDEMOD0;
            break;
        case E_DRVTSO_MEM:
            u16Pad = HAL_TSOIN_MUX_MEM;
            break;
        default:
            return DRVTSO_INVALID_PARAM;
    }

    _TSO_ENTRY();

    HAL_TSO_SelPad(u8Eng, u16Pad, pIfSet->bParallel);
    HAL_TSO_Set_InClk(u8Eng, u16clk, FALSE, FALSE);

    u16Cfg = TSO_CFG0_EXTERNAL_SYNC_SEL | TSO_CFG0_SOURCE_LIVE;
    if(pIfSet->bParallel)
    {
        u16Cfg |= TSO_CFG0_PARALLEL_SEL;
    }
    HAL_TSO_Cfg0_Enable(u8Eng, u16Cfg, TRUE);
#endif
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
	HAL_TSO_Cfg4_Enable(u8Eng, TSO_CFG4_TS_OUT_EN, bEnable);    
    _TSO_RETURN(DRVTSO_OK);
}


