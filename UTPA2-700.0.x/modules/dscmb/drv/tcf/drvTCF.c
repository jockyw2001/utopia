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
////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvTCF.c
// @brief  TCF Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "drvTCF.h"
#include "halTCF.h"
#include "regTCF.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_BOOL      _bTCF_Init     = FALSE;
static MS_S32       _s32TCFMutexId = -1;
static MS_BOOL      _bActivateCFB  = TRUE;

#define TCF_TIMEOUT_VALUE (500)


//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define _TCF_DBG(fmt, args...)  //printf("\033[32m""[DBG]"fmt"\033[m", ## args)
#define _TCF_ERR(fmt, args...)  printf("\033[31m""[ERR]"fmt"\033[m", ## args)

//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_CF_Ready(void)
{
    _TCF_DBG("[%s][%d] Tracing...........\n", __FUNCTION__, __LINE__);
    CF_TRANS_STATUS_VALUES eTransStatus = E_CF_TRANS_STATUS_BUSY_INIT;
    MS_U32                 u32Retries   = 0;
    MS_U8                  u8Ret        = DRV_TCF_OK;

    eTransStatus = HAL_CF_Get_Trans_Status();
    switch(eTransStatus)
    {
    case E_CF_TRANS_STATUS_INPUT:
        u32Retries = CF_CMD_WORDS;
        do {
            HAL_CF_Write_Input( 0);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        } while( E_CF_TRANS_STATUS_INPUT==eTransStatus );
    case E_CF_TRANS_STATUS_BUSY_OP:
        u32Retries = 30;
        while( E_CF_TRANS_STATUS_BUSY_OP==eTransStatus )
        {
            MsOS_DelayTask(1);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        }
    case E_CF_TRANS_STATUS_DONE_OK:
    case E_CF_TRANS_STATUS_DONE_ERROR:
    case E_CF_TRANS_STATUS_DONE_MFR_TEST:
        u32Retries = CF_CWC_WORDS;
        do {
            HAL_CF_Read_Output();
            HAL_CF_Write_Output( 0);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        } while( (E_CF_TRANS_STATUS_DONE_OK==eTransStatus) ||
                 (E_CF_TRANS_STATUS_DONE_ERROR==eTransStatus) ||
                 (E_CF_TRANS_STATUS_DONE_MFR_TEST==eTransStatus) );
    case E_CF_TRANS_STATUS_BUSY_INIT:
    case E_CF_TRANS_STATUS_BUSY_SETUP:
        u32Retries = 30;
        while( E_CF_TRANS_STATUS_READY > eTransStatus )
        {
            MsOS_DelayTask(1);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        }
    default:
        break;
    }

    return u8Ret;
}

#ifdef TCF_CFKE
static MS_BOOL _MDrv_CFKE_Ready(void)
{
    _TCF_DBG("[%s][%d] Tracing...........\n", __FUNCTION__, __LINE__);
    CFKE_TRANS_STATUS_VALUES eTransStatus = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U32                   u32Retries   = 0;
    MS_U8                    u8Ret        = DRV_TCF_OK;


    eTransStatus = HAL_CFKE_Get_OperationStatus();
    switch(eTransStatus){
        case E_CFKE_TRANS_STATUS_DONE_ERROR:
        case E_CFKE_TRANS_STATUS_DONE_TEST:
            _TCF_DBG("[%s][%d] Perform hard-coded \"CFKE_OP_CLRKE_CMD\"\n", __FUNCTION__, __LINE__);
            HAL_CFKE_Write_Command( CFKE_OP_CLRKE_CMD);
            eTransStatus = HAL_CFKE_Get_OperationStatus();
        case E_CFKE_TRANS_STATUS_BUSY_OP:
            u32Retries = 30;
            while( E_CFKE_TRANS_STATUS_BUSY_OP==eTransStatus ){
                MsOS_DelayTask(1);
                eTransStatus = HAL_CFKE_Get_OperationStatus();

                if( 0>=u32Retries-- )
                {
                    _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_FAIL;
                    break;
                }
            }
        case E_CFKE_TRANS_STATUS_BUSY_INIT:
            u32Retries = 30;
            while( (CFKE_TRANS_STATUS_VALUES)E_CF_TRANS_STATUS_READY>eTransStatus ){
                MsOS_DelayTask(1);
                eTransStatus = HAL_CFKE_Get_OperationStatus();

                if( 0>=u32Retries-- ){
                    _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_FAIL;
                    break;
                }
            }
        default:
            break;
    }

    return u8Ret;
}

static void _MDrv_CFKE_WriteSlots(DRV_CFKE_SOURCE *pSrc)
{
    MS_U8    u8i = 0;

    switch(pSrc->eSrcType){
        case E_CFKE_SRC_SLOTA:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotA( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTB:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotB( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTC:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotC( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTD:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotD( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        default:
            break;
    }

}
#endif


//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC DRV_TCF_RET MDrv_TCF_Init(void)
{
    MS_VIRT u32Bank     = 0;
    MS_PHY u32BankSize = 0;

    if( !_bTCF_Init ){
        if (FALSE == MDrv_MMIO_GetBASE( (MS_VIRT*)&u32Bank, (MS_PHY*)&u32BankSize, MS_MODULE_HW)){
            _TCF_ERR("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__);
            return DRV_TCF_FAIL;
        }
        //printf("[%s] >>>>>> u32Bank= 0x%08lx\n", __FUNCTION__, u32Bank);

        HAL_CF_SetBank( u32Bank);
        HAL_CF_Clk(TRUE);
        do
        {
            if(HAL_CF_Get_Trans_Status() != 0)
            {
                break;
            }
        }while(1);
        _s32TCFMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"TCF mutex", MSOS_PROCESS_SHARED);
        _bTCF_Init = TRUE;
        _TCF_DBG("[%s][%d] MDrv_TCF_Init Done!\n", __FUNCTION__, __LINE__);
    }
    else{
        _TCF_DBG("[%s][%d] TCF was already inited!\n", __FUNCTION__, __LINE__);
    }

    return DRV_TCF_OK;
}


// CF
//
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Version_Info(DRV_CF_VERSION *pstVersion)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_VERSION_INFO *_pstVersion= (HAL_CF_VERSION_INFO *) pstVersion;

    HAL_CF_Version_Info( _pstVersion);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_Trans_Status(DRV_CF_TRANS_STATUS *pstTransStatus)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_TRANS_STATUS *_pstTransStatus= (HAL_CF_TRANS_STATUS *) pstTransStatus;

    HAL_CF_Trans_Status( _pstTransStatus);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_Cf_Status(DRV_CF_CF_STATUS *pstCfStatus)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_CF_STATUS *_pstCfStatus= (HAL_CF_CF_STATUS *) pstCfStatus;

    HAL_CF_Cf_Status( _pstCfStatus);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_FeatureVector(DRV_CF_FEATURE_VECTOR *pstCfFeature)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_FEATURE_VECTOR *_pstCfFeature= (HAL_CF_FEATURE_VECTOR *) pstCfFeature;

    HAL_CF_Cf_FeatureVector( _pstCfFeature);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_Issue_Op(DRV_CF_OPERATION *pCfOperations)
{
    CF_TRANS_STATUS_VALUES eTransStatus = E_CF_TRANS_STATUS_BUSY_INIT;
    CF_OPERATION_TYPE      eOpType      = E_CF_OP_TYPE_OP_CWC;
    MS_U32                 u32Cmd       = 0;
    MS_U32                 u32WaitCnt   = 0;
    MS_U8                  u8Ret        = DRV_TCF_OK;
    MS_U32                 u32i         = 0;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    eTransStatus = HAL_CF_Get_Trans_Status();
    switch(eTransStatus)
    {
    case E_CF_TRANS_STATUS_BUSY_INIT:
    case E_CF_TRANS_STATUS_BUSY_SETUP:
    case E_CF_TRANS_STATUS_INPUT:         // note: same as vendor
    case E_CF_TRANS_STATUS_BUSY_OP:
    case E_CF_TRANS_STATUS_DONE_OK:
        if( DRV_TCF_OK!=(u8Ret= _MDrv_CF_Ready()) )
        {
            _TCF_ERR("[%s][%d] Set CF to E_CF_TRANS_STATUS_READY fail!!\n", __FUNCTION__, __LINE__);
            goto CF_ISSUE_OP_FINISH;
        }
    case E_CF_TRANS_STATUS_READY:
        #if 1 // patch of K3S HW bug
        eOpType = (pCfOperations->Op[0]&CF_OP_TYPE_MASK)>>CF_OP_TYPE_POS;
        _TCF_DBG("[%s] _bActivateCFB = %s\n", __FUNCTION__, _bActivateCFB? "True":"Flase");
        _TCF_DBG("[%s] Operation Type= %u\n", __FUNCTION__, eOpType);
        if( _bActivateCFB && (eOpType==E_CF_OP_TYPE_OP_CWC) )
        {
            _TCF_DBG("[%s] E_CF_OP_TYPE_OP_CWC => MDrv_CFB_Enable()\n", __FUNCTION__);
            MDrv_CFB_Enable();

            // Wait reg_cgb_ready
            while( MDrv_CFB_Is_Ready()!=DRV_TCF_OK )
                ;
        }
        #endif

        for( u32i=0 ; u32i<CF_CMD_WORDS ; u32i++ )
        {
            HAL_CF_Write_Input( pCfOperations->Op[u32i]);
            u32Cmd = HAL_CF_Read_Input();
            if( u32Cmd!=pCfOperations->Op[u32i] )
            {
                _TCF_ERR("[%s][%d] Write cmd fail!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
            }
        }

        #if 1 // patch of K3S HW bug
        if( _bActivateCFB && (eOpType==E_CF_OP_TYPE_OP_CWC) )
        {
            // Wait reg_cfb_done
            u32WaitCnt = 0;
            while( (MDrv_CFB_Is_Done()!=DRV_TCF_OK) && (u32WaitCnt<TCF_TIMEOUT_VALUE) )
            {
                MsOS_DelayTask(10);
                u32WaitCnt++;
            }
            if( u32WaitCnt>=TCF_TIMEOUT_VALUE )
            {
               _TCF_ERR("[%s] CFB timeout!!\n", __FUNCTION__);
               u8Ret = DRV_TCF_FAIL;
            }

            _TCF_DBG("[%s] E_CF_OP_TYPE_OP_CWC => MDrv_CFB_Reset()\n", __FUNCTION__);
            MDrv_CFB_Reset();
        }

        if( (_bActivateCFB==FALSE) && (eOpType==E_CF_OP_TYPE_OP_CWC) )
        {
            _TCF_DBG("[%s] Activate CFB when Next OP_CWC comes()\n", __FUNCTION__);
            _bActivateCFB = TRUE;
        }
        #endif
        break;
    default:
        _TCF_ERR("[%s][%d] CF is busy!!\n", __FUNCTION__, __LINE__);
        u8Ret = DRV_TCF_FAIL;
        break;
    }

CF_ISSUE_OP_FINISH:
    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_IsFinished(void)
{
    MS_U8 u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( HAL_CF_Get_CF_IsFinished()==TRUE )
    {
        u8Ret = DRV_TCF_OK;
    }
    else
    {
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DLL_PUBLIC DRV_TCF_RET MDrv_CF_Read_Op_Result(DRV_CF_RESULT *pCfResult)
{
    MS_U32 u32i  = 0;
    MS_U8  u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    memset( pCfResult, 0x00, sizeof(DRV_CF_RESULT));
    pCfResult->eTransStatus   = HAL_CF_Get_Trans_Status();
    pCfResult->eOperationType = HAL_CF_Get_Operation_Type();
    pCfResult->u8CwcValid     = HAL_CF_Get_CwcValid();

    if( E_CF_TRANS_STATUS_BUSY_OP < pCfResult->eTransStatus ) // E_CF_TRANS_STATUS_DONE_*
    {
        for( u32i=0 ; u32i<CF_CWC_WORDS ; u32i++ )
        {
            pCfResult->Shv[u32i] = HAL_CF_Read_Output();
            HAL_CF_Write_Output( 0);
        }
    }
    else
    {
        switch(pCfResult->eTransStatus)
        {
        case E_CF_TRANS_STATUS_BUSY_INIT:
        case E_CF_TRANS_STATUS_BUSY_SETUP:
        case E_CF_TRANS_STATUS_READY:
        case E_CF_TRANS_STATUS_INPUT:
            _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
            u8Ret = DRV_TCF_FAIL;
            break;
        default:
            _TCF_ERR("[%s][%d] Timed out!!\n", __FUNCTION__, __LINE__);
            u8Ret = DRV_TCF_FAIL;
            break;
        }
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}


// CFB
//
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Init(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    MS_BOOL bRet = DRV_TCF_FAIL;

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    bRet = HAL_CFB_Init();

    MsOS_ReleaseMutex(_s32TCFMutexId);
    if( bRet!=TRUE ){
        return DRV_TCF_FAIL;
    }
    else{
        return DRV_TCF_OK;
    }
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Enable(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    MS_BOOL bRet = DRV_TCF_FAIL;

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }

    bRet = HAL_CFB_Enable();

    if( bRet!=TRUE ){
        return DRV_TCF_FAIL;
    }
    else{
        return DRV_TCF_OK;
    }
    #endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Reset(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    MS_BOOL bRet = DRV_TCF_FAIL;

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }

    _bActivateCFB = TRUE;
    bRet = HAL_CFB_Reset();

    if( bRet!=TRUE ){
        return DRV_TCF_FAIL;
    }
    else{
        return DRV_TCF_OK;
    }
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Setup(DRV_CFB_CFBCFG *pstCFBCfg)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    HAL_CFB_CWCCFG *_pstCwcCfg = (HAL_CFB_CWCCFG *)&(pstCFBCfg->stCwcCfg);

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( (pstCFBCfg->u8UseCase==E_CFB_USE_CASE_3_1_2) || (pstCFBCfg->u8UseCase==E_CFB_USE_CASE_3_5_1) )
    {
        _TCF_DBG("[%s]] It's AES_KEY Use_Case!!!\n", __FUNCTION__);
        _bActivateCFB = FALSE; // obmit CFB_enable when first OP_AES comes
    }

    HAL_CFB_Set_Use_Case( pstCFBCfg->u8UseCase);
    HAL_CFB_Set_Otp_Key( pstCFBCfg->u8OtpKeyIdx);
    HAL_CFB_Set_CACWC_Src( pstCFBCfg->u8CAcwcSrc);

    if( NULL!=pstCFBCfg->CACWC )
        HAL_CFB_SET_CACWC( pstCFBCfg->CACWC);
    if( NULL!=pstCFBCfg->EPK )
        HAL_CFB_SET_EPK( pstCFBCfg->EPK);
    if( NULL!=pstCFBCfg->EFUV )
        HAL_CFB_SET_EFUV( pstCFBCfg->EFUV);

    HAL_CFB_Set_Cwc_Cfg( _pstCwcCfg);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Ready(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    MS_BOOL bRet = DRV_TCF_FAIL;

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }

    bRet = HAL_CFB_Is_Ready();

    if( bRet!=TRUE ){
        return DRV_TCF_FAIL;
    }
    else{
        return DRV_TCF_OK; // ready
    }
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Done(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#else
    MS_BOOL bRet = DRV_TCF_FAIL;

    if( MDrv_TCF_Init()==DRV_TCF_FAIL )
    {
        _TCF_ERR("[%s][%d] MDrv_TCF_Init fail\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }

    bRet = HAL_CFB_Is_Done();

    if( bRet!=TRUE ){
        return DRV_TCF_FAIL;
    }
    else{
        return DRV_TCF_OK; // done
    }
#endif
}

DLL_PUBLIC void MDrv_CFB_DBG_KT_Response(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
#else
    HAL_CFB_DBG_KT_Response();
#endif
}

DLL_PUBLIC MS_U8 MDrv_CFB_DBG_CFB_FSM(void)
{
#ifdef TCF_CFKE
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return 0;
#else
    return HAL_CFB_DBG_CFB_FSM();
#endif
}


// CFKE
//
DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_Cmd_Exe(DRV_CFKE_OPERATION *pCfkeOp)
{
#ifdef TCF_CFKE
    CFKE_TRANS_STATUS_VALUES eTransStatus  = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U32                   u32Cmd        = 0;
//    MS_U8                    uDesireStatus = CFKE_STATUS_DONE;
//    MS_U8                    u8i           = 0;
    MS_U8                    u8Ret         = DRV_TCF_OK;


    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);


    // check CFKE_STATUS
    eTransStatus = HAL_CFKE_Get_OperationStatus();
    switch(eTransStatus){
        case E_CFKE_TRANS_STATUS_BUSY_INIT:
        case E_CFKE_TRANS_STATUS_DONE_ERROR:
        case E_CFKE_TRANS_STATUS_DONE_TEST:
            if( DRV_TCF_OK!=(u8Ret= _MDrv_CFKE_Ready()) ){
                _TCF_ERR("[%s][%d] Set CF to E_CF_TRANS_STATUS_READY fail!!\n", __FUNCTION__, __LINE__);
                goto CFKE_CMD_EXE_FINISH;
            }
        case E_CFKE_TRANS_STATUS_READY:
        case E_CFKE_TRANS_STATUS_DONE_OK:
            // write the input data based on operation request
            switch(pCfkeOp->eOpType){
                case E_CFKE_OP_DCF:
                    _TCF_DBG("[%s][%d] Direct CF operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 and src2 should be null. dest should not be null.
                    if( E_CFKE_OP_DCF!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_DCF> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_DCF> SRC1 & SRC2 values should be NULL, DEST should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_XOR:
                    _TCF_DBG("[%s][%d] XOR operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_XOR!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_XOR> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_XOR> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_XOR> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_XOR> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_AES:
                    _TCF_DBG("[%s][%d] AES operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_AES!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_AES> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_AES> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_AES> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_AES> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_UNW:
                    _TCF_DBG("[%s][%d] UNW operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_UNW!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_UNW> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_UNW> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_UNW> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_UNW> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Write unwrap data to the registers ~//
                    // (unwrap data) Write the value to the unwrap path register
                    HAL_CFKE_Write_UnwPath( pCfkeOp->UnwData.u32UnwPathData);
                    // Read back the data to verify whether the data is written successfully
                    if( pCfkeOp->UnwData.u32UnwPathData!=HAL_CFKE_Read_UnwPath() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write UNW Path Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the value [31:0] to the validator register
                    HAL_CFKE_Write_Validator0( pCfkeOp->UnwData.pu832ValidatorData[0]);
                    if( pCfkeOp->UnwData.pu832ValidatorData[0]!=HAL_CFKE_Read_Validator0() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write Validator 0 Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the value [63:32]to the validator1 register
                    HAL_CFKE_Write_Validator1( pCfkeOp->UnwData.pu832ValidatorData[1]);
                    if( pCfkeOp->UnwData.pu832ValidatorData[1]!=HAL_CFKE_Read_Validator1() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write Validator 1 Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_HWDECM:
                    _TCF_DBG("[%s][%d] HWDECM operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 should not be null. dest and src2 must be null
                    if( E_CFKE_OP_HWDECM!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_HWDECM> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL!=pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_HWDECM> SRC1/SRC2/Dest values are not valid.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the data to the key registers for src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_HWDECM> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_TEST:
                    _TCF_DBG("[%s][%d] TEST operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 src2 and dest must be null
                    if( E_CFKE_OP_TEST!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_TEST> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL!=pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_TEST> SRC1/SRC2/Dest values should be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_CLRKE:
                    _TCF_DBG("[%s][%d] CLRKE operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 src2 and dest are ignored
                    if( E_CFKE_OP_CLRKE!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_CLRKE> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    break;

                default: // default of 'switch(pCfkeOp->eOpType)'
                    _TCF_ERR("[%s][%d] Invalid operation type requested!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_INVALID_INPUT;
                	break;
            }// end of 'switch(pCfkeOp->eOpType)'


            // Write the command data to the command register, Read back the data to verify whether the command is written successfully
            HAL_CFKE_Write_Command( u32Cmd);
            if( u32Cmd!=HAL_CFKE_Read_Command() ){
                _TCF_ERR("[%s][%d] CFKE Write Command Error!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                goto CFKE_CMD_EXE_FINISH;
            }
            _TCF_DBG("[%s][%d] CFKE Write Command OK-- 0x%08lx\n\n", __FUNCTION__, __LINE__, u32Cmd);
            break;

        default: // default of 'switch(eTransStatus)'
            _TCF_ERR("[%s][%d] Busy..\n", __FUNCTION__, __LINE__);
           	u8Ret = DRV_TCF_BUSY;
           	break;
    }// end of 'switch(eTransStatus)'


CFKE_CMD_EXE_FINISH:
    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_IsDone(CFKE_ERROR_CODE *eCfkeErr)
{
#ifdef TCF_CFKE
    CFKE_TRANS_STATUS_VALUES eTransStatus  = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U8                    u8Ret         = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( CFKE_STATUS_DONE&(eTransStatus= HAL_CFKE_Get_OperationStatus()) ){
        *eCfkeErr = HAL_CFKE_Get_ErrorCode();
        if( *eCfkeErr ){
            _TCF_ERR("[%s] CFKE Error!! (ErrorCode= 0x%x)\n", __FUNCTION__, *eCfkeErr);
        }
        u8Ret = DRV_TCF_OK;
    }
    else{
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_IsReady(void)
{
#ifdef TCF_CFKE
    MS_U8 u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( E_CFKE_TRANS_STATUS_READY==HAL_CFKE_Get_OperationStatus() ){
        u8Ret = DRV_TCF_OK;
    }
    else{
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CFKE_UserHash_Setup(DRV_CFKE_USER_HASH_DATA *pUserHash)
{
#ifdef TCF_CFKE
    MS_U8    u8i = 0;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    // Write the value to the user hash register
    for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
        HAL_CFKE_Write_UserHash( pUserHash->pu32UsrHashData[u8i]);
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);

    return DRV_TCF_OK;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#endif
}

// CRIKL
//
DLL_PUBLIC MS_U32 MDrv_CRIKL_Mirror(CRIKL_MIRROR_MEM eCandidate)
{
#ifdef TCF_CFKE
    MS_U32  u32Value = 0;

    switch(eCandidate){
        case E_CRIKL_MIRROR_CF_STATUS:
            u32Value = HAL_CRIKL_Read_CfStatus();
            break;
        case E_CRIKL_MIRROR_CFKE_STATUS:
            u32Value = HAL_CRIKL_Read_CfkeStatus();
            break;
        case E_CRIKL_MIRROR_CF_PLATFORM:
            u32Value = HAL_CRIKL_Read_CfPlatform();
            break;
        case E_CRIKL_MIRROR_CF_FEATURE:
            u32Value = HAL_CRIKL_Read_CfFeature();
            break;
        default:
            _TCF_ERR("[%s][%d] Invalid Mirror Member\n", __FUNCTION__, __LINE__);
            break;
    }

    return u32Value;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return 0;
#endif
}

DLL_PUBLIC DRV_TCF_RET MDrv_CRIKL_Set_KeyConfig(DRV_CRIKL_KEY_CONFIG *stKeyCfg)
{
#ifdef TCF_CFKE
    MS_BOOL    _bDscmbKey = stKeyCfg->bDscmbKey;


    /// kt_index | ks_index
    if(_bDscmbKey){
        HAL_CRIKL_Set_KTIndex( stKeyCfg->eScmbCtl, stKeyCfg->u8DscmbId);
    }
    else{
        HAL_CRIKL_Set_KSIndex( stKeyCfg->u8KeySlotIdx);
    }

    /// kt_para | ks_para
    HAL_CRIKL_Reset_KeyPara();

    // High_HalfWord
    HAL_CRIKL_Setup_VendorNo( _bDscmbKey, stKeyCfg->u8VendorNo);
    HAL_CRIKL_Setup_VendorAtt( _bDscmbKey, stKeyCfg->u16VendorAtt);

    // 2nd_HalfWord
    HAL_CRIKL_Setup_OutputAllowed( _bDscmbKey, stKeyCfg->u8OutputAllowed);
    HAL_CRIKL_Setup_bMultiEng( _bDscmbKey, stKeyCfg->bMultiEng);
    HAL_CRIKL_Setup_bLsb64( _bDscmbKey, stKeyCfg->bLsb64);
    HAL_CRIKL_Setup_KeySize( _bDscmbKey, stKeyCfg->eKeySize);

    // 3rd_HalfWord
    HAL_CRIKL_Setup_KeyEngine( _bDscmbKey, stKeyCfg->u8KlEng);
    HAL_CRIKL_Setup_Drop( _bDscmbKey, stKeyCfg->u8Drop);
    HAL_CRIKL_Setup_Cnt( _bDscmbKey, stKeyCfg->u8Cnt);

    // Low_HalfWord
    HAL_CRIKL_Setup_KeyDest( _bDscmbKey, stKeyCfg->u8KlDest);
    HAL_CRIKL_Setup_OpType( _bDscmbKey, stKeyCfg->eOpSel);
    if(_bDscmbKey){
        HAL_CRIKL_Setup_KteSel( _bDscmbKey, stKeyCfg->eKteSel);
    }
    HAL_CRIKL_Setup_bLock( _bDscmbKey, stKeyCfg->bLock);
    HAL_CRIKL_Setup_bParaValid( _bDscmbKey, stKeyCfg->bParaVld);
    HAL_CRIKL_Setup_bKlEn( _bDscmbKey, stKeyCfg->bKlEn);

    HAL_CRIKL_Write_KeyPara( _bDscmbKey);

    return DRV_TCF_OK;
#else
    _TCF_ERR("[%s][%d] NOT Supported!!\n", __FUNCTION__, __LINE__);
    return DRV_TCF_FAIL;
#endif
}

