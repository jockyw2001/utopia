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
// file    drvCIPHER.c
// @brief  CIPHER Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvCIPHER.h"
#include "drvMMIO.h"
#include "halRSA.h"
#include "halCIPHER.h"
#include "utopia.h"

#include "drvAESDMA_v2.h"
#include "drvAESDMA_private.h"

#ifdef CIPHER_UTOPIA_20
#define CIPHER_UTOPIA2 1UL
#else
#define CIPHER_UTOPIA2 0UL
#endif

#if CIPHER_UTOPIA2
#include "drvCIPHER_v2.h"
#endif
#include "drvCIPHER_private.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define CIPHERIntEnable()                       MsOS_EnableInterrupt (CRYPTODMA_IRQ)
#define CIPHERIntDisable()                      MsOS_DisableInterrupt(CRYPTODMA_IRQ)
#define CIPHERIntAttach()                       MsOS_AttachInterrupt (CRYPTODMA_IRQ, (InterruptCb )_CIPHER_Isr)
#define CIPHERIntDetach()                       MsOS_DetachInterrupt (CRYPTODMA_IRQ)

#define AESDMAPA2KSEG1(addr)                    ((void *)(((MS_U32)addr) | 0xa0000000UL)) //physical -> unchched


#define RSAInitChk()                               if(_bRSA_Init == FALSE){ DEBUG_CIPHER(CIPHER_DBG_ERR,"Not initialized"); return DRVAESDMA_FAIL; }
//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define CIPHER_TASK_STACK_SIZE   (4096UL)
#define CIPHER_CAVID             (0x0FUL)
#define HASH_HMAC_KEYPAD_SIZE    (64UL)
#define HASH_RPT_SIZE_ALL        (10UL)
#define HASH_RPT_SIZE_STATUS     (2UL)

#define HASH_MD5_DIGEST_SIZE     (16UL)
#define HASH_SHA1_DIGEST_SIZE    (20UL)
#define HASH_SHA256_DIGEST_SIZE  (32UL)
#define HASH_SM3_DIGEST_SIZE     (32UL)
#define HASH_BLOCK_SIZE          (64UL)

#define DRV_CIPHER_TA_SHA_CHECK_SIZE    (8UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_LUT   (0x1UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_M     (0x2UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_BC    (0x4UL)
#define DRV_CIPHER_TA_KCV_SIZE          (16UL)   //HW limitation, need padding to 16 bytes
#define DRV_CIPHER_TA_LUT_SIZE          (256UL)
#define DRV_CIPHER_TA_M_SIZE            (128UL)
#define DRV_CIPHER_TA_BC_SIZE           (16UL)

#define DRV_CIPHER_TA_SHA_FAIL          (0x00000010UL)
#define DRV_CIPHER_TA_KCV_FAIL          (0x00000020UL)
#define DRV_CIPHER_TA_LUT_FAIL          (0x00000030UL)
#define DRV_CIPHER_TA_M_FAIL            (0x00000040UL)
#define DRV_CIPHER_TA_BC_FAIL           (0x00000050UL)

#define DRV_CIPHER_NO_REPORT_BUFFER     (0x00000003UL)

//Interrupt
#define DRV_CIPHER_DMA_DATA_INIT        (0x00000000UL)
#define DRV_CIPHER_DONE                 (0x00000100UL)
#define DRV_CIPHER_CALLBACK             (0x0000000FUL)
#define DRV_CIPHER_CLOSE                (0x80000000UL)

//Report status
#define DRV_CIPHER_REPORT_OK            (0x80000000UL)

typedef struct
{
    MS_U32 u32LUT;  //LUT3 fill in 512 bytes table, each has 9 bits to describe
    MS_U32 u32M;
    MS_U32 u32BC;
}DRV_CIPHER_TA_LUT3;


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
static MS_BOOL _bCIPHERDrv_Inited = FALSE;
static MS_BOOL _bCIPHERDrv_KeyCtrlEn = FALSE;

#ifdef CIPHER_NO_PRINTF
#define DRV_CIPHER_ASSERT( _bool_, _msg_, ret) if(!(_bool_)){return (ret);}
#else
#define DRV_CIPHER_ASSERT( _bool_, _msg_, ret) if(!(_bool_)){ printf(_msg_); return (ret);}
#endif
#define DRV_CIPHER_INIT_CHECK(ret) DRV_CIPHER_ASSERT(TRUE == _bCIPHERDrv_Inited, "drvCIPHER is not initialized\n", ret)
#define DRV_CIPHER_KEYCTRL_CHECK(ret) DRV_CIPHER_ASSERT(TRUE == _bCIPHERDrv_KeyCtrlEn, "drvCIPHER key control mode is not enable\n", ret)

MS_S32  _CipherMtx = 0;
MS_S32  _CipherRSAMtx;
MS_S32  _CipherSemaphore;

MS_S32  _AESDMAMtx = 0;

MS_BOOL bDES_Key = FALSE;

#if CIPHER_UTOPIA2
static void *pu32AesdmaInst = NULL;
static void *pu32CipherInst = NULL;
#endif

static CIPHER_DBGMSG_LEVEL _CIPHERDBGLevel = CIPHER_DBG_ERR;
static MSIF_Version _drv_cipher_version = {
    .DDI = { CIPHER_DRV_VERSION, },
};

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)
//Isr proc
static void*                            _pCIPHERTaskStack;
static MS_U8                            _u8CIPHER_StackBuffer[CIPHER_TASK_STACK_SIZE];
static MS_S32                           _s32CIPHERTaskId;

static MS_U32                           _u32LastErr;

typedef enum
{
    E_CIPHER_DMA_DONE,
    E_CIPHER_HASH_DONE,
}CIPHER_WAIT;

static CIPHER_WAIT _eCipherWait;

#endif

#define CIPHER_REPORT_BUFFER_MAX 2UL

typedef struct
{
    MS_U32  u32Report;
    MS_U32  u32DoneBytes;
    MS_U32  u32ExcMsg;
    MS_U8*  pu8Digest_Buf;
    MS_U32  u32Digest_Buf_Size;
    P_DrvCIPHER_EvtCallback pfCallback;
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8   u8KeyIdx;
    MS_BOOL bUsed;
}CIPHER_REPORT;

typedef struct
{
    DRV_CIPHER_DMACFG stConfig;
    MS_U8           u8Key[HAL_CRYPTODMA_KEYLEN_MAX];
    MS_U8           u8IV[HAL_CRYPTODMA_KEYLEN_MAX];
    MS_BOOL         bUsed;
}CIPHER_RESOURCE;

static CIPHER_REPORT _CIPHER_ReportBuffer[CIPHER_REPORT_BUFFER_MAX];
static CIPHER_RESOURCE _CIPHER_Resource[HAL_CRYPTODMA_DMA_KEY_SLOT];

#if CIPHER_UTOPIA2
    extern MS_U32 _u32CallbackID;
    extern MS_U16 _u16ThreadID;
#else
    static MS_U32 _u32CallbackID = 1;
    static MS_U16 _u16ThreadID = 1;
#endif

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

static MS_BOOL                        _bRSA_Init = FALSE;

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)
static MS_S32                           _s32CIPHEREventId;

#define _CIPHER_SetEvent(flag)             MsOS_SetEvent(_s32CIPHEREventId, (flag))
#define _CIPHER_GetEvent(events)           MsOS_WaitEvent(_s32CIPHEREventId, DRV_CIPHER_DONE, &events, E_OR_CLEAR, MSOS_WAIT_FOREVER)

#endif
//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#ifdef CIPHER_NO_PRINTF
#define DEBUG_CIPHER(debug_level, fmt, args...)
#define DEBUG_CIPHER_KEYCTRL(debug_level, fmt, args...)
#else
#define DEBUG_CIPHER(debug_level, fmt, args...) do{ if (_CIPHERDBGLevel >= (debug_level))\
        printf("[%s][%d]"fmt"\n", __FUNCTION__, __LINE__,  ## args);}\
        while(0)
#define DEBUG_CIPHER_KEYCTRL(debug_level, fmt, args...) do{ if (_CIPHERDBGLevel >= (debug_level))\
        printf("[%s][%d][KeyCtrl] "fmt"\n", __FUNCTION__, __LINE__,  ## args);}\
        while(0)
#endif


//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)
static void _CIPHER_Isr(void)
{
    MS_U32  u32Rpt[10] = {0};
    MS_U32  u32ExcMsg = 0;
    MS_U32  u32RptId = 0;
    MS_U8   u8BufNum     = 0;
    MS_U8*  pu8Digest_Buf  = NULL;
    MS_U32  u32Digest_Buf_Size = 0;

    HAL_CIPHER_IntClear();

    if(E_CIPHER_DMA_DONE == _eCipherWait)
    {
        HAL_CIPHER_DMA_GetRpt(u32Rpt);
        HAL_CIPHER_GetException(&u32ExcMsg);

        u32RptId = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;
        u8BufNum = u32RptId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

        _CIPHER_ReportBuffer[u8BufNum].u32Report    = u32Rpt[0];
        _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = u32Rpt[1];
        _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg    = u32ExcMsg;

    }
    else  //E_CIPHER_HASH_DONE
    {
        //Get all register report, include status and digest
        HAL_CIPHER_Hash_GetRpt(u32Rpt, HASH_RPT_SIZE_ALL);
        HAL_CIPHER_GetException(&u32ExcMsg);

        u32RptId = u32Rpt[0] & REG_CIPHER_RPT_THREAD_MSK;
        u8BufNum = u32RptId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

        _CIPHER_ReportBuffer[u8BufNum].u32Report    = u32Rpt[0];
        _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = u32Rpt[1];
        _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg    = u32ExcMsg;

        pu8Digest_Buf      = _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf;
        u32Digest_Buf_Size = _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size;

        if((pu8Digest_Buf != NULL) && (u32Digest_Buf_Size != 0))
        {
            //Avoid status report, only copy digest to output address
            memcpy((MS_U8 *)pu8Digest_Buf, u32Rpt+HASH_RPT_SIZE_STATUS, u32Digest_Buf_Size);
        }

    }

    _u32CallbackID = u32RptId;

    _CIPHER_SetEvent(DRV_CIPHER_DONE); // Trigger task
    CIPHERIntEnable();

}

static void _CIPHER_Isr_Proc(void)
{
    MS_U32  u32Events = 0;
    MS_U32  u32Rpt    = 0;
    MS_BOOL bSuccess  = FALSE;
    MS_U32  u32ExcMsg = 0;
    MS_U32  u32ExcTmp = 0;
    MS_U32  u32RptId  = 0;
    MS_U8   u8BufNum  = 0;
    MS_U8   u8KeyIdx  = 0;
    P_DrvCIPHER_EvtCallback pfCallBack = NULL;
    CIPHER_KEY_SRC  eKeySrc = 0;

    while (1)
    {
        _CIPHER_GetEvent(u32Events);

        if (HAS_FLAG(u32Events, DRV_CIPHER_DONE))
        {
            //Handling callback
            u8BufNum   = _u32CallbackID % (MS_U32)CIPHER_REPORT_BUFFER_MAX;
            u32Rpt     = _CIPHER_ReportBuffer[u8BufNum].u32Report;
            pfCallBack = _CIPHER_ReportBuffer[u8BufNum].pfCallback;
            u32ExcTmp  = _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg;
            eKeySrc    = _CIPHER_ReportBuffer[u8BufNum].eKeySrc;
            u8KeyIdx   = _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx;

			HAL_CIPHER_Hash_ExceptFilter(&u32ExcTmp, eKeySrc, u8KeyIdx);
            u32ExcMsg  = HAL_CIPHER_ReadException(u32ExcTmp);

            u32RptId   = u32Rpt & REG_CIPHER_RPT_THREAD_MSK;

            bSuccess = FALSE;

            if(u32Rpt & DRV_CIPHER_REPORT_OK)
            {
                bSuccess = TRUE;
            }

            if(u32ExcMsg != 0)
            {
                HAL_CIPHER_ResetException();
            }

            _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
            _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
            _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
            _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
            _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
            _CIPHER_ReportBuffer[u8BufNum].pfCallback         = NULL;
            _CIPHER_ReportBuffer[u8BufNum].eKeySrc            = 0;
            _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx           = 0;
            _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

            MsOS_ReleaseSemaphore(_CipherSemaphore);
            if(pfCallBack != NULL)
            {
                pfCallBack(u32RptId, bSuccess, u32ExcMsg);

            }
        }

        if (HAS_FLAG(u32Events, DRV_CIPHER_CLOSE))
        {
            break;
        }

    } // Task while loop
}

#endif

MS_BOOL _MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret     = DRV_CIPHER_OK;
    MS_U32  u32Rpt[2]  = {0};
    MS_U8   u8BufNum   = 0;
    MS_BOOL bDone      = FALSE;

    DRV_CIPHER_INIT_CHECK(FALSE);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    //Check register report
    if(FALSE == HAL_CIPHER_DMA_CmdDone(u32CmdId, &u32Ret))
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:DMA cmd[%u] NOT done!", u32CmdId);
        bDone = FALSE;
        goto ISDMADONE_FINISH;
    }

    HAL_CIPHER_DMA_GetRpt(u32Rpt);

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA fail[%u]!", u32CmdId);

		if(u32Ret != 0)
    	{
        	*pu32Exception = HAL_CIPHER_ReadException(u32Ret);
        	HAL_CIPHER_ResetException();
    	}
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISDMADONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_ReadMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_ISDMADONE pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_ISDMADONE));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    pArgs.u32CmdId = u32CmdId;
    pArgs.pu32Exception = pu32Exception;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_IsDMADone, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        //DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CIPHER_IsDMADone(u32CmdId, pu32Exception);
#endif
}

MS_BOOL _MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret         = DRV_CIPHER_OK;
    MS_U32  u32Rpt[10]     = {0};
    MS_U8   u8BufNum       = 0;
    MS_BOOL bDone          = FALSE;

    MS_U8*  pu8Digest_Buf = NULL;
    MS_U32  u32Digest_Buf_Size = 0;
    CIPHER_KEY_SRC  eKeySrc = 0;
    MS_U8   u8KeyIdx        = 0;

    DRV_CIPHER_INIT_CHECK(FALSE);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    if(FALSE == HAL_CIPHER_Hash_CmdDone(u32CmdId, &u32Ret))
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:HASH cmd[%u] NOT done!", u32CmdId);
        bDone = FALSE;
        goto ISHASHDONE_FINISH;
    }

    //Get all register report, include status and digest
    HAL_CIPHER_Hash_GetRpt(u32Rpt, HASH_RPT_SIZE_ALL);

    pu8Digest_Buf      = _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf;
    u32Digest_Buf_Size = _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size;
    eKeySrc            = _CIPHER_ReportBuffer[u8BufNum].eKeySrc;
    u8KeyIdx           = _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx;

    //Avoid status report, only copy digest to output address
    if((pu8Digest_Buf != NULL) && (u32Digest_Buf_Size > 0))
    {
        memcpy((MS_U8 *)pu8Digest_Buf, u32Rpt+HASH_RPT_SIZE_STATUS, u32Digest_Buf_Size);
    }

    //Reset exception flag for polling mode
    if(u32Ret != 0)
    {
        HAL_CIPHER_Hash_ExceptFilter(&u32Ret, eKeySrc, u8KeyIdx);
        *pu32Exception = HAL_CIPHER_ReadException(u32Ret);
        HAL_CIPHER_ResetException();
    }

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH fail[%u]!", u32CmdId);
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc            = 0;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx           = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISHASHDONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_FlushMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_ISHASHDONE pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_ISHASHDONE));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    pArgs.u32CmdId = u32CmdId;
    pArgs.pu32Exception = pu32Exception;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_IsHASHDone, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        //DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }

    return TRUE;
#else
    return _MDrv_CIPHER_IsHASHDone(u32CmdId, pu32Exception);
#endif
}

#if 0
static void _dump(MS_U8 *ptr, MS_U32 size)
{
    int i = 0;
    printf("[DUMP Start]\n");
    for(i = 0; i < size; i++)
    {
        printf("0x%02x ", *(ptr+i));
    }
    printf("[DUMP Finish]\n");
}
#endif

MS_U32 _AESDMA_MSBF2Native(MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

void _ReverseU8Buf(MS_U8 *buf, MS_U32 len)
{
    MS_U8 i = 0;
    MS_U8 j = len-1;
    MS_U8 temp = 0;

    if((buf==NULL)||(len<=1)) return;

    while(i<j)
    {
         temp   = buf[i];
         buf[i] = buf[j];
         buf[j] = temp;
         i++;
         j--;
     }
}

CIPHER_DBGMSG_LEVEL _CIPHER_TransDbgLevel(DrvAESDMA_DbgLevel eDbgLevel)
{
    CIPHER_DBGMSG_LEVEL eCipherDbgLevel = CIPHER_DBG_NONE;
    switch(eDbgLevel)
    {
        case E_DRVAESDMA_DBG_L1:
            eCipherDbgLevel = CIPHER_DBG_ERR;
            break;

        case E_DRVAESDMA_DBG_L2:
            eCipherDbgLevel = CIPHER_DBG_INFO;
            break;

        default:
            eCipherDbgLevel = CIPHER_DBG_NONE;
            break;
    }

    return eCipherDbgLevel;

}

//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DRV_CIPHER_RET _MDrv_CIPHER_InitBySWFlag(MS_BOOL bSWInit)
{
    MS_VIRT u32Bank;
    MS_PHY  u32BankSize;
    MS_U32 i = 0;

    if(_bCIPHERDrv_Inited == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:Double Init!");
        return DRV_CIPHER_OK;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Get MMIO base fail");
        return DRV_CIPHER_FAIL;
    }

    HAL_CIPHER_SetBank(u32Bank);
    HAL_CIPHER_ResetStatus(TRUE, TRUE);

    if(FALSE == bSWInit)
    {
        HAL_CIPHER_SWReset();

        _u16ThreadID   = 1;
        _u32CallbackID = 1;
    }

    for(i=0;i<CIPHER_REPORT_BUFFER_MAX;i++)
    {
        _CIPHER_ReportBuffer[i].u32Report          = 0;
        _CIPHER_ReportBuffer[i].u32DoneBytes       = 0;
        _CIPHER_ReportBuffer[i].u32ExcMsg          = 0;
        _CIPHER_ReportBuffer[i].pu8Digest_Buf      = NULL;
        _CIPHER_ReportBuffer[i].u32Digest_Buf_Size = 0;
        _CIPHER_ReportBuffer[i].eKeySrc     = 0;
        _CIPHER_ReportBuffer[i].u8KeyIdx    = 0;
        _CIPHER_ReportBuffer[i].bUsed              = FALSE;
    }

    _CipherMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"CIPHER Mutex", MSOS_PROCESS_SHARED);
	_AESDMAMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"AESDMA Mutex", MSOS_PROCESS_SHARED);
    _CipherSemaphore = MsOS_CreateSemaphore(1, E_MSOS_FIFO, (char *)"CIPHER Semaphore");

	_bCIPHERDrv_Inited = TRUE;
#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    //Isr event
    _s32CIPHEREventId = MsOS_CreateEventGroup("CIPHER_Event");
    if (_s32CIPHEREventId < 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "_s32CIPHEREventId < 0 ");
        _u32LastErr = DRV_CIPHER_FAIL;
        return _u32LastErr;
    }

    //Task stack
    _pCIPHERTaskStack = _u8CIPHER_StackBuffer;

    //Isr Task
    _s32CIPHERTaskId = MsOS_CreateTask((TaskEntry)_CIPHER_Isr_Proc,
                                        0,
                                        E_TASK_PRI_SYS,
                                        TRUE,
                                        _pCIPHERTaskStack,
                                        CIPHER_TASK_STACK_SIZE,
                                        "CIPHER_Task");

    if (_s32CIPHERTaskId < 0)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "_s32CIPHERTaskId < 0 ");
        MsOS_DeleteEventGroup(_s32CIPHEREventId);
        _u32LastErr = DRV_CIPHER_FAIL;
        return _u32LastErr;
    }

    HAL_CIPHER_IntEnable();

    CIPHERIntDisable();
    CIPHERIntDetach();
    CIPHERIntAttach();
    CIPHERIntEnable();

#endif

    return DRV_CIPHER_OK;
}

DRV_CIPHER_RET _MDrv_CIPHER_Init(void)
{
    return _MDrv_CIPHER_InitBySWFlag(FALSE);
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Init(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CipherInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_CIPHER, &pu32CipherInst, 0x0, NULL))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "UtopiaOpen CIPHER fail\n");
            return DRV_CIPHER_FAIL;
        }
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_Init, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return DRV_CIPHER_OK;
#else
    return _MDrv_CIPHER_Init();
#endif
}

DRV_CIPHER_RET _MDrv_CIPHER_Reset(void)
{
    MS_U32 i = 0;
    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

	MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE, TRUE);
    HAL_CIPHER_SWReset();
    HAL_CIPHER_ResetException();

    _u16ThreadID   = 1;
    _u32CallbackID = 1;

    for(i=0;i<CIPHER_REPORT_BUFFER_MAX;i++)
    {
        _CIPHER_ReportBuffer[i].u32Report          = 0;
        _CIPHER_ReportBuffer[i].u32DoneBytes       = 0;
        _CIPHER_ReportBuffer[i].u32ExcMsg          = 0;
        _CIPHER_ReportBuffer[i].pu8Digest_Buf      = NULL;
        _CIPHER_ReportBuffer[i].u32Digest_Buf_Size = 0;
        _CIPHER_ReportBuffer[i].pfCallback         = NULL;
        _CIPHER_ReportBuffer[i].eKeySrc            = 0;
        _CIPHER_ReportBuffer[i].u8KeyIdx           = 0;
        _CIPHER_ReportBuffer[i].bUsed              = FALSE;
    }

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
        {
            memset(&_CIPHER_Resource[i], 0, sizeof(CIPHER_RESOURCE));
        }

        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "INFO:Reset Key Ctrl!\n");
    }

	DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Cipher Reset!\n");

    MsOS_ReleaseMutex(_CipherMtx);

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Reset(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return DRV_CIPHER_OK;
#else
    return _MDrv_CIPHER_Reset();
#endif
}

DRV_CIPHER_RET _MDrv_CIPHER_ResetKey(MS_U32 u32CAVid, MS_U32 u32KeyIdx)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    DRV_CIPHER_ALGO algo = {0};
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);
    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE , FALSE );

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)u32CAVid);
        goto RESETKEY_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo(algo);

    if(FALSE == HAL_CIPHER_ResetKey(u32KeyIdx))
    {
       ret =  DRV_CIPHER_BAD_PARAM;
       DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Reset Key Index fail!");
       goto RESETKEY_FINISH;
    }

    HAL_CIPHER_DMA_Set_InputSrcFrom(E_CIPHER_DIRECT, NULL, 0);
    HAL_CIPHER_DMA_Set_OutputDstTo(E_CIPHER_DIRECT, NULL, 0);
    HAL_CIPHER_DMA_Set_FileinDesc ( 0, HAL_CRYPTODMA_DIRDATA_MAX);
    HAL_CIPHER_DMA_Set_FileoutDesc( 0, (HAL_CRYPTODMA_DIRDATA_MAX - 1));

    if(FALSE == HAL_CIPHER_DMA_Start(FALSE, E_DMA_INT_NONE, TRUE, 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Reset Key fail!\n");
        ret =  DRV_CIPHER_FAIL;
        goto RESETKEY_FINISH;
    }

    //Check register report
    while((FALSE == HAL_CIPHER_DMA_CmdDone(0, &u32Ret)) && (u32WaitCnt < HAL_CIPHER_RESETKEY_TIMEOUT_VALUE))
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:Reset Key NOT done!");
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HAL_CIPHER_RESETKEY_TIMEOUT_VALUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:Reset Key timeout!");
        ret = DRV_CIPHER_FAIL;
        goto RESETKEY_FINISH;
    }

    if(u32Ret != 0)
    {
        //Ignore exception here, HW still clear session key
        HAL_CIPHER_ResetException();
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Cipher Reset Key success!\n");

RESETKEY_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    MsOS_ReleaseSemaphore(_CipherSemaphore);


    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_ResetKey(MS_U32 u32CAVid, MS_U32 u32KeyIdx)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_RESETKEY pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_RESETKEY));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    pArgs.u32CAVid = u32CAVid;
    pArgs.u32KeyIdx = u32KeyIdx;
    pArgs.ret = DRV_CIPHER_OK;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_ResetKey, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return pArgs.ret;
#else
    return _MDrv_CIPHER_ResetKey(u32CAVid, u32KeyIdx);
#endif
}

DRV_CIPHER_RET _MDrv_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel)
{
    _CIPHERDBGLevel = eDBGMsgLevel;
    HAL_CIPHER_SetDbgLevel(eDBGMsgLevel);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Debug level[%d]!", eDBGMsgLevel);

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_DBGLEVEL pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_DBGLEVEL));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    pArgs.eDBGMsgLevel = eDBGMsgLevel;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_DbgLevel, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return DRV_CIPHER_OK;
#else
    return _MDrv_CIPHER_SetDbgLevel(eDBGMsgLevel);
#endif
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return DRV_CIPHER_FAIL;
    }

    *ppVersion = &_drv_cipher_version;

    return DRV_CIPHER_OK;
}

DRV_CIPHER_RET _MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8 u8BufNum = 0;

	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        if(HAL_CRYPTODMA_DMA_KEY_SLOT <= stCfg.stKey.u8KeyIdx)
        {
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Invalid Key Index!");
            return DRV_CIPHER_BAD_PARAM;
        }

        if(E_CIPHER_KSRC_KL == stCfg.stKey.eKeySrc && FALSE == _CIPHER_Resource[stCfg.stKey.u8KeyIdx % HAL_CRYPTODMA_DMA_KEY_SLOT].bUsed)
        {
           DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Not used Cipher Id[%u]!", stCfg.stKey.u8KeyIdx);
           return DRV_CIPHER_BAD_PARAM;
        }
    }

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.stOutput.u32Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA data size is zero!");
        return DRV_CIPHER_BAD_PARAM;
    }

    //Check Key length
    if((stCfg.stKey.u8KeyLen > HAL_CRYPTODMA_KEYLEN_MAX) || (stCfg.stKey.u8IVLen > HAL_CRYPTODMA_KEYLEN_MAX))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Invalid Key length!\n");
        return DRV_CIPHER_BAD_PARAM;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA CmdId is NULL!");
        return DRV_CIPHER_BAD_PARAM;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set CAVid [0x%x]", (unsigned int)stCfg.u32CAVid);

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto DMA_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo( stCfg.stAlgo);

    if(FALSE == HAL_CIPHER_DMA_Set_Key(stCfg.stKey))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set Key fail!");
        goto DMA_FINISH;
    }

    HAL_CIPHER_DMA_Set_IV(stCfg.stKey.pu8IVData, stCfg.stKey.u8IVLen);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]",
        stCfg.stAlgo.eMainAlgo, stCfg.stAlgo.eSubAlgo, stCfg.stAlgo.eResAlgo, stCfg.stAlgo.eSBAlgo);

	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stCfg.stInput.eMemoryType, NULL, 0);
    if(FALSE == HAL_CIPHER_DMA_Set_OutputDstTo(stCfg.stOutput.eMemoryType, NULL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set Output destination fail!");
        goto DMA_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Input source from [%d], Output destination to [%d]",
        stCfg.stInput.eMemoryType, stCfg.stOutput.eMemoryType);

    // Set File-in Addr //
    HAL_CIPHER_DMA_Set_FileinDesc ( stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
    HAL_CIPHER_DMA_Set_FileoutDesc( stCfg.stOutput.u32Addr, stCfg.stOutput.u32Addr + stCfg.stOutput.u32Size - 1);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Input address [0x%08x], size[%u], Output address [0x%08x], size[%u]",
        (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size, (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size);

    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CIPHER report buffer is used!");
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto DMA_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_DMA_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif
    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:DMA decrypt operation[%d]", stCfg.bDecrypt);

    if(FALSE == HAL_CIPHER_DMA_Start(stCfg.bDecrypt, eIntMode , TRUE , _u16ThreadID))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA Fail!\n");
        ret =  DRV_CIPHER_FAIL;
        goto DMA_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]", eIntMode, _u16ThreadID);

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

DMA_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_DMACALC pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_DMACALC));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    memcpy(&pArgs.stCfg, &stCfg, sizeof(DRV_CIPHER_DMACFG));
    pArgs.pu32CmdId = pu32CmdId;
    pArgs.ret = DRV_CIPHER_OK;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_DMACalc, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return pArgs.ret;
#else
    return _MDrv_CIPHER_DMACalc(stCfg, pu32CmdId);
#endif
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_OTPHASHCalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8 u8BufNum = 0;
    MS_U32 u32OTPHashRound = 0;
    MS_U32 u32CurrentRound = 0;

	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    // Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.stOutput.u32Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA data size is zero!");
        return DRV_CIPHER_BAD_PARAM;
    }

    // Check CmdId
    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA CmdId is NULL!");
        return DRV_CIPHER_BAD_PARAM;
    }
    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set CAVid [0x%x]", (unsigned int)stCfg.u32CAVid);

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto DMA_FINISH;
    }

    // Set Algorithm
    HAL_CIPHER_DMA_Set_Algo( stCfg.stAlgo);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]",
        stCfg.stAlgo.eMainAlgo, stCfg.stAlgo.eSubAlgo, stCfg.stAlgo.eResAlgo, stCfg.stAlgo.eSBAlgo);

	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stCfg.stInput.eMemoryType, NULL, 0);
    HAL_CIPHER_DMA_Set_OutputDstTo(stCfg.stOutput.eMemoryType, NULL, 0);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Input source from [%d], Output destination to [%d]",
        stCfg.stInput.eMemoryType, stCfg.stOutput.eMemoryType);

    // Not suppoet OTPHash (size low than 32 bytes)
    if(stCfg.stInput.u32Size < HAL_CRYPTODMA_OTPHASH_SIZE_MIN)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:OTPHASH input size error!");
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:OTPHASH support input size should lagger than %lu bytes!", HAL_CRYPTODMA_OTPHASH_SIZE_MIN);
        return DRV_CIPHER_BAD_PARAM;
    }

    // Check OTPHash round
    while(1)
    {
        if(stCfg.stInput.u32Size <= (u32OTPHashRound+1)*HAL_CRYPTODMA_OTPHASH_UNIT)
        {
            break;
        }
        else
        {
            u32OTPHashRound++;
        }
    }

    // Set File-out Addr //
    HAL_CIPHER_DMA_Set_FileoutDesc( stCfg.stOutput.u32Addr, stCfg.stOutput.u32Addr + stCfg.stOutput.u32Size - 1);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Output address [0x%08x], size[%u]",
        (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size);

    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CIPHER report buffer is used!");
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto DMA_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_DMA_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    // Start OTPHash
    for(u32CurrentRound=0; u32CurrentRound<=u32OTPHashRound; u32CurrentRound++)
    {
        // Set OTPHash configuration //
        HAL_CIPHER_DMA_Set_OTPHash( u32CurrentRound, u32OTPHashRound);

        // Set OTPHash File-in Addr //
        HAL_CIPHER_OTPHash_Set_FileinDesc( stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32CurrentRound, stCfg.stInput.eMemoryType);

        if(FALSE == HAL_CIPHER_OTPHash_Start(eIntMode , TRUE , _u16ThreadID))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA Fail!\n");
            ret =  DRV_CIPHER_FAIL;
            goto DMA_FINISH;
        }

        DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]", eIntMode, _u16ThreadID);

        *pu32CmdId = (MS_U32)_u16ThreadID;

        //Check thread id max
        if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
        {
            _u16ThreadID = 1;
        }
        else
        {
            _u16ThreadID++;
        }
    }

DMA_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;

}

DRV_CIPHER_RET _MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret  = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen    = 32;
    MS_U32 u32HashSrc   = 0;
    MS_U32 u32HashDst   = 0;
    MS_U8  u8BufNum    = 0;

	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH data size is zero!");
        goto HASHAUTO_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH CmdId is NULL!");
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SM3)      u32OutLen = HASH_SM3_DIGEST_SIZE;    //256 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Wrong buffer size[%u]!", stCfg.u32Digest_Buf_Size);
        goto HASHAUTO_FINISH;
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid
    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set CAVid [0x%x]\n", (unsigned int)stCfg.u32CAVid);

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto HASHAUTO_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Algorithm [%d]", stCfg.eAlgo);

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
     {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HASH IV fail!");
        goto HASHAUTO_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set init word count fail!");
        goto HASHAUTO_FINISH;
    }

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set source memory type [%d] fail", stCfg.stInput.eMemoryType);
        goto HASHAUTO_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Input address[0x%08x], size[%u]",
            (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
        goto HASHAUTO_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO;Set Input address[0x%08x], size[%u]",
        (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

#if 0 //Default output to Register
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA((MS_U32)stCfg.pu8Digest_Buf), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHAUTO_FINISH;
    }
#endif

	//Set HOS = 1 , Output will be MSB First
	HAL_CIPHER_Hash_SetHOS(TRUE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CIPHER report buffer is used!");
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto HASHAUTO_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Digest buffer address[0x%08lx], buffer size[%u]",
                (long unsigned int)stCfg.pu8Digest_Buf,
                stCfg.u32Digest_Buf_Size);

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_HASH_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif
    MsOS_FlushMemory();

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHAUTO_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]", eIntMode, _u16ThreadID);

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHAUTO_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    CIPHER_HASHCALC pArgs;
    memset(&pArgs, 0, sizeof(CIPHER_HASHCALC));

    if(pu32CipherInst == NULL)
    {
        return DRV_CIPHER_FAIL;
    }

    memcpy(&pArgs.stCfg, &stCfg, sizeof(DRV_CIPHER_HASHCFG));
    pArgs.pu32CmdId = pu32CmdId;
    pArgs.ret = DRV_CIPHER_OK;

    u32Ret = UtopiaIoctl(pu32CipherInst, E_MDRV_CMD_CIPHER_HASH, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return DRV_CIPHER_FAIL;
    }

    return pArgs.ret;
#else
    return _MDrv_CIPHER_HASH(stCfg, pu32CmdId);
#endif
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASHManual(DRV_CIPHER_HASHCFG_MANUAL stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    HAL_CIPHER_IWCTYPE  eIWCType = E_CIPHER_HASH_IWC_MANUAL; //Init Word Count
    MS_U32  u32IVSel    = CMD_HASH_IV_FIPS;
    MS_U8* pu8IV       = NULL;
    MS_U32  u32DataSize = 0;
    MS_U32  u32IVSize   = 0;
    MS_BOOL bAutoPad    = FALSE;
    MS_U32  u32OutLen   = 32;
    MS_U32  u32HashSrc  = 0;
    MS_U32  u32HashDst  = 0;
    MS_U8   u8BufNum    = 0;

	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    u32DataSize = stCfg.stInput.u32Size;

    if((0 == u32DataSize) && (E_CIPHER_HASH_STAGE_LAST != stCfg.eStage))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH data size is [%u]!\n", u32DataSize);
        goto HASHMANUAL_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH CmdId is NULL!");
        goto HASHMANUAL_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SM3)      u32OutLen = HASH_SM3_DIGEST_SIZE;    //256 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    //Check buffer and size
    if((stCfg.pu8Digest_Buf != NULL) && (stCfg.u32Digest_Buf_Size != u32OutLen))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Wrong buffer size[%u]!\n", stCfg.u32Digest_Buf_Size);
        goto HASHMANUAL_FINISH;
    }

    //Check IV setting
    if(stCfg.pu8SetIV != NULL)
    {
        pu8IV = stCfg.pu8SetIV;
        u32IVSel = CMD_HASH_IV_CMD;

        if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)
        {
            u32IVSize = HASH_SHA1_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)
        {
            u32IVSize = HASH_SHA256_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)
        {
            u32IVSize = HASH_MD5_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SM3)
        {
            u32IVSize = HASH_SM3_DIGEST_SIZE;
        }
    }
    else
    {
        //NULL IV
        if(stCfg.eIVMode == E_CIPHER_HASH_IV_CMD)
        {
            ret = DRV_CIPHER_BAD_PARAM;
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:NULL IV!");
            goto HASHMANUAL_FINISH;
        }
    }

    //IV and IWC is from previous report
    if(stCfg.eIVMode == E_CIPHER_HASH_IV_PRV)
    {
        u32IVSel = CMD_HASH_IV_PRV_RPT;
        eIWCType = E_CIPHER_HASH_IWC_PRV;
    }

    //AutoPad only works on last stage, previous stage is 64 bytes alignment
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        bAutoPad = TRUE;
    }
    else
    {
        if((u32DataSize % (MS_U32)HASH_BLOCK_SIZE) != 0 )
        {
            ret = DRV_CIPHER_BAD_PARAM;
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH block is 64 bytes alignment!");
            goto HASHMANUAL_FINISH;
        }
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid
    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set CAVid [0x%x]", (unsigned int)stCfg.u32CAVid);

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto HASHMANUAL_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, bAutoPad, FALSE);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Algorithm[%d], AutoPad[%d]", stCfg.eAlgo, bAutoPad);

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(pu8IV, u32IVSize , u32IVSel))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set IV fail, IV size[%u]", u32IVSize);
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set IV size[%u], IV Mode[%d]", u32IVSize, stCfg.eIVMode);

    //Set Init Word Count
    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(eIWCType, stCfg.u32StartByte))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set IWC fail, IWC[%d], StartBytes[%u]", eIWCType, stCfg.u32StartByte);
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set IWC[%d], StartBytes[%u]", eIWCType, stCfg.u32StartByte);

    //Set HMAC Key
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, stCfg.eHMACKPad, FALSE))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set HMAC key fail, HMAC key source[%d], key index[%u]", stCfg.stHMACKey.eKeySrc, stCfg.stHMACKey.u8KeyIdx);
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set HMAC Key Source[%d], Key Index[%u], KeyPad[%d]",
        stCfg.stHMACKey.eKeySrc, stCfg.stHMACKey.u8KeyIdx, stCfg.eHMACKPad);

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set source memory type [%d] fail", stCfg.stInput.eMemoryType);
        goto HASHMANUAL_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Input address[0x%08x], size[%u]",
            (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set Input address[0x%08x], size[%u]",
            (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }
#if 0
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHMANUAL_FINISH;
    }
#endif
	//Set HOS = 1, Output will be MSB First
	//IV auto mode use previous report digest as IV, report must be LSB first
    if(stCfg.pu8Digest_Buf == NULL)
    {
        HAL_CIPHER_Hash_SetHOS(FALSE);
    }
    else
    {
        HAL_CIPHER_Hash_SetHOS(TRUE);
    }

    //Set total size if last command
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        HAL_CIPHER_Hash_SetMsgLength(stCfg.u32StartByte + stCfg.stInput.u32Size);
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Message length, StartBytes[0x%08x], Input Size[%u]",
        (unsigned int)stCfg.u32StartByte, stCfg.stInput.u32Size);


    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CIPHER report buffer[%u] is used!", u8BufNum);
       ret =  DRV_CIPHER_NO_REPORT_BUFFER;
       goto HASHMANUAL_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Digest buffer address[0x%08lx], buffer size[%u]",
                (long unsigned int)stCfg.pu8Digest_Buf, stCfg.u32Digest_Buf_Size);


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_HASH_DONE;
#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHMANUAL_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]\n", eIntMode, _u16ThreadID);

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHMANUAL_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HMAC(DRV_CIPHER_HMACCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen   = 0;
    MS_U32 u32HashSrc  = 0;
    MS_U32 u32HashDst  = 0;
    MS_U8  u8BufNum    = 0;

#if defined (MSOS_TYPE_NOS) || defined (MCU_AEON) || defined (MSOS_TYPE_OPTEE)
    MS_U32 Rpt[10]     = {0};
    MS_U32 u32WaitCnt  = 0;
    MS_U32 u32ErrorMsg = 0;
#endif

	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        if(E_CIPHER_KSRC_KL == stCfg.stHMACKey.eKeySrc)
        {
           DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Not support!");
           return DRV_CIPHER_FAIL;
        }
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH data size is zero!");
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH CmdId is NULL!");
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SM3)      u32OutLen = HASH_SM3_DIGEST_SIZE;    //256 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Wrong buffer size[%u]!", stCfg.u32Digest_Buf_Size);
        goto HASHHMAC_FINISH;
    }
    //------------Inner hash----------------------------
    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);
    //Set CAVid
    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto HASHHMAC_FINISH;
    }
    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Algorithm [%d]", stCfg.eAlgo);

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HMAC IV fail!");
        goto HASHHMAC_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set init word count fail!");
        goto HASHHMAC_FINISH;
    }

    //Set HMAC Key
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_IKPAD, FALSE))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set HMAC key fail, HMAC key source[%d], key index[%u]", stCfg.stHMACKey.eKeySrc, stCfg.stHMACKey.u8KeyIdx);
        goto HASHHMAC_FINISH;
    }

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Set source memory type [%d] fail", stCfg.stInput.eMemoryType);
        goto HASHHMAC_FINISH;
    }
    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Input address[0x%08x], size[%u]",
            (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
        goto HASHHMAC_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Input address[0x%08x], size[%u]",
        (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);

    //Set output destination, HMAC inner hash output to register
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }
#if 0
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }
#endif
	//Set HOS = 0, HW limitation
	HAL_CIPHER_Hash_SetHOS(FALSE);

    HAL_CIPHER_Hash_SetMsgLength(stCfg.stInput.u32Size + HASH_HMAC_KEYPAD_SIZE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_HASH_DONE;
    eIntMode     = E_DMA_INT_ENABLE;

#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]", eIntMode, _u16ThreadID);

#if defined (MSOS_TYPE_NOS) || defined (MCU_AEON) || defined (MSOS_TYPE_OPTEE)

    while(FALSE == HAL_CIPHER_Hash_CmdDone(_u16ThreadID, &u32ErrorMsg) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
    #ifdef MSOS_TYPE_NUTTX
        MsOS_DelayTaskUs(200);
    #elif MSOS_TYPE_OPTEE

    #else
        MsOS_DelayTask(20);
    #endif
        u32WaitCnt++;
    }
    //First time should print HMAC error flag
    if(u32ErrorMsg != 0)
    {
        HAL_CIPHER_Hash_ExceptFilter(&u32ErrorMsg, stCfg.stHMACKey.eKeySrc, stCfg.stHMACKey.u8KeyIdx);
        HAL_CIPHER_ReadException(u32ErrorMsg);
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning;HASH timeout!");
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    HAL_CIPHER_Hash_GetRpt(Rpt, HASH_RPT_SIZE_STATUS);

    if((Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
        ret = DRV_CIPHER_FAIL;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HMAC Inner Hash Fail!");
        goto HASHHMAC_FINISH;
    }

    MsOS_ReleaseSemaphore(_CipherSemaphore);
#endif

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

    MsOS_ReleaseMutex(_CipherMtx);

    //----------------Outer hash------------------------------

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    eIntMode = E_DMA_INT_NONE;

    //Set CAVid
    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!", (unsigned int)stCfg.u32CAVid);
        goto HASHHMAC_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HMAC IV fail!");
        goto HASHHMAC_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set init word count fail!");
        goto HASHHMAC_FINISH;
    }


    //Set HMAC Key,
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_OKPAD, FALSE))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HMAC key fail!");
        goto HASHHMAC_FINISH;
    }

    //No need to set Hash message source
    //HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc);

    //Set input message address, outer hash from previous report
    if(FALSE == HAL_CIPHER_Hash_SetMsg(NULL, 0,  CMD_HASH_SRC_PRV_RPT))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HMAC message fail!");
        goto HASHHMAC_FINISH;
    }

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set HMAC output destination fail!");
        goto HASHHMAC_FINISH;
    }

#if 0 //Default output to Register
    //Set output address
	if(FALSE == HAL_CIPHER_Hash_Set_OuputAddr(MsOS_VA2PA(stCfg.stOutput.u32Addr), u32HashDst))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }
#endif

	//Set HOS = 1
	HAL_CIPHER_Hash_SetHOS(TRUE);

    HAL_CIPHER_Hash_SetMsgLength(u32OutLen + HASH_HMAC_KEYPAD_SIZE); //Hash output size + O_keypad_size

    u8BufNum = _u16ThreadID % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Digest buffer address[0x%08lx], buffer size[%u]\n",
                (long unsigned int)stCfg.pu8Digest_Buf, stCfg.u32Digest_Buf_Size);


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) &&  !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_HASH_DONE;

    if(stCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

     DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IntMode[%d], CmdId[%hu]\n", eIntMode, _u16ThreadID);

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHHMAC_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_KeyCtrl(MS_BOOL bEnable)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    MS_U32 i = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    if(bEnable)
    {
        for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
        {
            memset(&_CIPHER_Resource[i], 0, sizeof(CIPHER_RESOURCE));
        }

        _bCIPHERDrv_KeyCtrlEn = TRUE;
    }
    else
    {
        for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
        {
            memset(&_CIPHER_Resource[i], 0, sizeof(CIPHER_RESOURCE));
        }

        _bCIPHERDrv_KeyCtrlEn = FALSE;
    }

    DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Key slot control mode[%d]", _bCIPHERDrv_KeyCtrlEn);

    MsOS_ReleaseMutex(_CipherMtx);

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Alloc(MS_U32 *pu32CipherId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    MS_U32 i = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);
    DRV_CIPHER_KEYCTRL_CHECK(DRV_CIPHER_FAIL);

    if(NULL == pu32CipherId)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Cipher Id is NULL!");
        return DRV_CIPHER_BAD_PARAM;
    }

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
    {
        if(FALSE == _CIPHER_Resource[i].bUsed)
        {
            _CIPHER_Resource[i].bUsed = TRUE;
            *pu32CipherId = i;
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Allocate Cipher Id[%u]", i);
            goto CIPHER_ALLOC_DONE;
        }
    }

    ret = DRV_CIPHER_FAIL;
    DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Allocate Cipher Id fail");

CIPHER_ALLOC_DONE:

    MsOS_ReleaseMutex(_CipherMtx);
    return ret;

}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Free(MS_U32 u32CipherId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);
    DRV_CIPHER_KEYCTRL_CHECK(DRV_CIPHER_FAIL);

    if(HAL_CRYPTODMA_DMA_KEY_SLOT <= u32CipherId)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Invalid Cipher Id[%u]!", u32CipherId);
        return DRV_CIPHER_BAD_PARAM;
    }

    u32CipherId %= HAL_CRYPTODMA_DMA_KEY_SLOT;

    if(FALSE == _CIPHER_Resource[u32CipherId].bUsed)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_WARN, "Warning:Not used Cipher Id[%u]!", u32CipherId);
        return DRV_CIPHER_BAD_PARAM;
    }

    if(E_CIPHER_KSRC_KL == _CIPHER_Resource[u32CipherId].stConfig.stKey.eKeySrc)
    {
        if(DRV_CIPHER_OK != MDrv_CIPHER_ResetKey(_CIPHER_Resource[u32CipherId].stConfig.u32CAVid, _CIPHER_Resource[u32CipherId].stConfig.stKey.u8KeyIdx))
        {
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Reset key slot fail!");
            return DRV_CIPHER_FAIL;

        }
    }

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    memset(&_CIPHER_Resource[u32CipherId], 0, sizeof(CIPHER_RESOURCE));
    DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Free Cipher Id[%u]", u32CipherId);

    MsOS_ReleaseMutex(_CipherMtx);

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAConfigure(MS_U32 u32CipherId, DRV_CIPHER_DMACFG stCfg)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);
    DRV_CIPHER_KEYCTRL_CHECK(DRV_CIPHER_FAIL);

    if(HAL_CRYPTODMA_DMA_KEY_SLOT <= u32CipherId)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Invalid Cipher Id[%u]!", u32CipherId);
        return DRV_CIPHER_BAD_PARAM;
    }

    u32CipherId %= HAL_CRYPTODMA_DMA_KEY_SLOT;

    if(FALSE == _CIPHER_Resource[u32CipherId].bUsed)
    {
       DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Not used Cipher Id[%u]!", u32CipherId);
       return DRV_CIPHER_BAD_PARAM;
    }

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    if(0 != stCfg.u32CAVid)
    {
        _CIPHER_Resource[u32CipherId].stConfig.u32CAVid = stCfg.u32CAVid;
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "INFO:Set CAVid [0x%x]", (unsigned int)stCfg.u32CAVid);
    }

    //Update Algorithm and Encrypt/Decrypt operation
    if(0 != stCfg.stAlgo.eMainAlgo || 0 != stCfg.stAlgo.eSubAlgo || 0 != stCfg.stAlgo.eResAlgo || 0 != stCfg.stAlgo.eSBAlgo)
    {
        _CIPHER_Resource[u32CipherId].stConfig.stAlgo = stCfg.stAlgo;
        _CIPHER_Resource[u32CipherId].stConfig.bDecrypt = stCfg.bDecrypt;
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "INFO:Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]",
        stCfg.stAlgo.eMainAlgo, stCfg.stAlgo.eSubAlgo, stCfg.stAlgo.eResAlgo, stCfg.stAlgo.eSBAlgo);
    }

    if(0 != stCfg.stInput.u32Addr || 0 != stCfg.stInput.u32Size || 0 != stCfg.stInput.eMemoryType)
    {
        _CIPHER_Resource[u32CipherId].stConfig.stInput = stCfg.stInput;
        DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Input address [0x%08x], size[%u]",
        (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size);
    }

    if(0 != stCfg.stOutput.u32Addr || 0 != stCfg.stOutput.u32Size || 0 != stCfg.stOutput.eMemoryType)
    {
        _CIPHER_Resource[u32CipherId].stConfig.stOutput = stCfg.stOutput;
        DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Set Output address [0x%08x], size[%u]",
        (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size);
    }

    if(0 != stCfg.stKey.eKeySrc || 0 != stCfg.stKey.u8KeyIdx || NULL != stCfg.stKey.pu8KeyData)
    {
        if(E_CIPHER_KSRC_CPU == stCfg.stKey.eKeySrc)
        {
            if(NULL != stCfg.stKey.pu8KeyData && 0 != stCfg.stKey.u8KeyLen)
            {
                memset(&_CIPHER_Resource[u32CipherId].u8Key, 0, HAL_CRYPTODMA_KEYLEN_MAX);
                memcpy(&_CIPHER_Resource[u32CipherId].u8Key, (MS_U8 *)stCfg.stKey.pu8KeyData, stCfg.stKey.u8KeyLen);

                _CIPHER_Resource[u32CipherId].stConfig.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
                _CIPHER_Resource[u32CipherId].stConfig.stKey.pu8KeyData = _CIPHER_Resource[u32CipherId].u8Key;
                _CIPHER_Resource[u32CipherId].stConfig.stKey.u8KeyLen = stCfg.stKey.u8KeyLen;
                DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Set ACPU key in CipherId[%u]", u32CipherId);
            }
            else
            {
                DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Key is NULL!");
                ret = DRV_CIPHER_BAD_PARAM;
                goto DMA_CONFIGURE_DONE;
            }
        }

        if(E_CIPHER_KSRC_KL == stCfg.stKey.eKeySrc)
        {
            _CIPHER_Resource[u32CipherId].stConfig.stKey.eKeySrc = E_CIPHER_KSRC_KL;
            _CIPHER_Resource[u32CipherId].stConfig.stKey.u8KeyIdx = (MS_U8)u32CipherId;
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Set KL index in CipherId[%u]", u32CipherId);
        }

        if(E_CIPHER_KSRC_OTP == stCfg.stKey.eKeySrc)
        {
            _CIPHER_Resource[u32CipherId].stConfig.stKey.eKeySrc = E_CIPHER_KSRC_OTP;
            _CIPHER_Resource[u32CipherId].stConfig.stKey.u8KeyIdx = stCfg.stKey.u8KeyIdx;
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Set OTP key index in CipherId[%u]", u32CipherId);
        }

        if(E_CIPHER_KSRC_CAIP == stCfg.stKey.eKeySrc)
        {
            _CIPHER_Resource[u32CipherId].stConfig.stKey.eKeySrc = E_CIPHER_KSRC_CAIP;
            DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Set CAIP key in CipherId[%u]", u32CipherId);
        }

    }

    if(NULL != stCfg.stKey.pu8IVData && 0 != stCfg.stKey.u8IVLen)
    {
        memset(&_CIPHER_Resource[u32CipherId].u8IV, 0, HAL_CRYPTODMA_KEYLEN_MAX);
        memcpy(&_CIPHER_Resource[u32CipherId].u8IV, (MS_U8 *)stCfg.stKey.pu8IVData, stCfg.stKey.u8IVLen);
        _CIPHER_Resource[u32CipherId].stConfig.stKey.pu8IVData = _CIPHER_Resource[u32CipherId].u8IV;
        _CIPHER_Resource[u32CipherId].stConfig.stKey.u8IVLen = stCfg.stKey.u8IVLen;
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "Set IV in CipherId[%u]", u32CipherId);
    }

DMA_CONFIGURE_DONE:

    MsOS_ReleaseMutex(_CipherMtx);
    return ret;

}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAStart(MS_U32 u32CipherId, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    DRV_CIPHER_DMACFG stCfg;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);
    DRV_CIPHER_KEYCTRL_CHECK(DRV_CIPHER_FAIL);

    if(HAL_CRYPTODMA_DMA_KEY_SLOT <= u32CipherId)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Invalid Cipher Id[%u]!", u32CipherId);
        return DRV_CIPHER_BAD_PARAM;
    }

    u32CipherId %= HAL_CRYPTODMA_DMA_KEY_SLOT;

    if(FALSE == _CIPHER_Resource[u32CipherId].bUsed)
    {
       DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:Not used Cipher Id[%u]!", u32CipherId);
       return DRV_CIPHER_BAD_PARAM;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_ERR, "Error:DMA CmdId is NULL!");
        return DRV_CIPHER_BAD_PARAM;
    }

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    memset(&stCfg, 0 , sizeof(DRV_CIPHER_DMACFG));

    stCfg = _CIPHER_Resource[u32CipherId].stConfig;

    DEBUG_CIPHER_KEYCTRL(CIPHER_DBG_INFO, "DMA use CipherId[%u]", u32CipherId);

    MsOS_ReleaseMutex(_CipherMtx);

    ret = MDrv_CIPHER_DMACalc(stCfg, pu32CmdId);

    return ret;

}


#if 0
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMAParser(CIPHER_PARSERCFG stParserCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8  u8BufNum  = 0;

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    if((stParserCfg.stInput.u32Size == 0) || (stParserCfg.stOutput.u32Size == 0))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA data size is zero!\n");
        ret =  DRV_CIPHER_BAD_PARAM;
        goto PARSER_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:DMA CmdId is NULL!\n");
        ret =  DRV_CIPHER_BAD_PARAM;
        goto PARSER_FINISH;
    }

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set CAVid [0x%x]\n", (unsigned int)stParserCfg.u32CAVid);

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stParserCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error: Set CAVid [0x%x]fail!\n", (unsigned int)stParserCfg.u32CAVid);
        goto PARSER_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo( stParserCfg.stAlgo);
    HAL_CIPHER_DMA_Set_Key ( stParserCfg.stKey);
    HAL_CIPHER_DMA_Set_IV(stParserCfg.stKey.pu8IVData, stParserCfg.stKey.u8IVLen);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set algorithm: MAIN[%d], SUB[%d], RES[%d], SB[%d]\n"
            ,stParserCfg.stAlgo.eMainAlgo, stParserCfg.stAlgo.eSubAlgo, stParserCfg.stAlgo.eResAlgo, stParserCfg.stAlgo.eSBAlgo);

	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stParserCfg.stInput.eMemoryType, NULL, 0);
    HAL_CIPHER_DMA_Set_OutputDstTo(stParserCfg.stOutput.eMemoryType, NULL, 0);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set Input source from [%d], Output source from [%d]\n",
            stParserCfg.stInput.eMemoryType, stParserCfg.stOutput.eMemoryType);

    // Set File-in Addr //
    HAL_CIPHER_DMA_Set_FileinDesc ( stParserCfg.stInput.u32Addr, stParserCfg.stInput.u32Size);
    HAL_CIPHER_DMA_Set_FileoutDesc( stParserCfg.stOutput.u32Addr, stParserCfg.stOutput.u32Addr + stParserCfg.stOutput.u32Size - 1);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set Input address [0x%08x], size[%u], Output address [0x%08x], size[%u]\n",
           (unsigned int)stParserCfg.stInput.u32Addr, stParserCfg.stInput.u32Size, (unsigned int)stParserCfg.stOutput.u32Addr, stParserCfg.stOutput.u32Size);

    // Set Hardware Parser
    HAL_CIPHER_PARSER_Set_Mode(stParserCfg.eParserMode);

    HAL_CIPHER_PARSER_Set_PID(stParserCfg.stPID);
    HAL_CIPHER_PARSER_Set_SCB(stParserCfg.stTSCfg.eSCB);

    HAL_CIPHER_PARSER_Set_ForceSCB(stParserCfg.stTSCfg.bInScrb, stParserCfg.stTSCfg.eFSCB);
    HAL_CIPHER_PARSER_Rmv_SCB(stParserCfg.stTSCfg.bRmvScrb);

    HAL_CIPHER_PARSER_Set_MaskSCB(stParserCfg.stTSCfg.bTsScrbMask);

    HAL_CIPHER_PARSER_Set_Key2(stParserCfg.stKey2);
    HAL_CIPHER_PARSER_Set_IV2(stParserCfg.stKey2.pu8IVData, stParserCfg.stKey2.u8IVLen);

    HAL_CIPHER_PARSER_Set_ClearHeaderMode(stParserCfg.bClearHead);

    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CIPHER report buffer is used!\n");
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto PARSER_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)

    _eCipherWait = E_CIPHER_DMA_DONE;

    if(stParserCfg.pfCallback != NULL)
    {
        _CIPHER_ReportBuffer[u8BufNum].pfCallback = stParserCfg.pfCallback;
        eIntMode = E_DMA_INT_ENABLE;
    }
#endif

    if(FALSE == HAL_CIPHER_DMA_Start(stParserCfg.bDecrypt, eIntMode , TRUE , _u16ThreadID))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:PARSER Fail!\n");
        ret =  DRV_CIPHER_FAIL;
        goto PARSER_FINISH;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "IntMode[%d], CmdId[%hu]\n", eIntMode, _u16ThreadID);

    *pu32CmdId = (MS_U32)_u16ThreadID;

    _u16ThreadID++;

PARSER_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;

}
#endif

#ifdef ENABLE_BGC
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Enable(MS_U32 u32EngId,MS_U32 u32Algo, MS_U8 *pu8Golden, MS_U32 u32SAddr, MS_U32 u32Size)
{
     //Set BGC Algo
     HAL_CIPHER_BGC_Algo(u32EngId, u32Algo);

     //Set BGC Period
     HAL_CIPHER_BGC_Period(u32EngId, E_CIPHER_BGC_PERIOD_1MS);

     //Set BGC Range
     HAL_CIPHER_BGC_Range(u32EngId, u32SAddr, (u32SAddr + u32Size-1) );

     //Set BGC Golden
     HAL_CIPHER_BGC_Golden(u32EngId, pu8Golden);

     //Set BGC Enable
     HAL_CIPHER_BGC_Start(u32EngId, TRUE);


    return 0;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_BGC_Check(MS_U32 u32EngId, MS_BOOL *bOK)
{
    if(HAL_CIPHER_BGC_Check(u32EngId) == FALSE)
    {
        *bOK = FALSE;
    }
    else
    {
        *bOK = TRUE;
    }
    return 0;
}

#endif
//////////////////////////////////////////////////////////////////
static DRV_CIPHER_KEY _CipherKey = {
        E_CIPHER_KSRC_CPU,
            0,
            0,
            0,
            0,
            0
};
static MS_BOOL _isDecrypt = TRUE;
static DRV_CIPHER_ALGO _stAlgo = {0};
static DRV_CIPHER_DATA _stInput  = {0};
static DRV_CIPHER_DATA _stOutput = {0};
static MS_U32 _RetId = 0;
static MS_U8 _u8IV[16] = {0};
static MS_U8 _u8Key[16] = {0};
static MS_U8 _u8ZeroData[16] = {0};
static MS_U32 _u32CAVid = 0x0F;

DRVAESDMA_RESULT _MDrv_AESDMA_InitBySWFlag(MS_BOOL bSWInit)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;

    ret = _MDrv_CIPHER_InitBySWFlag(bSWInit);

    if(DRV_CIPHER_OK != ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:_MDrv_CIPHER_InitBySWFlag fail");
        return DRVAESDMA_FAIL;
    }

    // Default Key from ACPU
    _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;
    _CipherKey.pu8KeyData = _u8ZeroData;
    _CipherKey.u8KeyLen = 16;
    _CipherKey.u8KeyIdx = 0;

    //Clear IV
    _CipherKey.pu8IVData = _u8ZeroData;
    _CipherKey.u8IVLen = 16;

    //Clear Algo
    _stAlgo.eMainAlgo = 0;
    _stAlgo.eSubAlgo  = 0;
    _stAlgo.eResAlgo  = 0;
    _stAlgo.eSBAlgo   = 0;

    //Clear Input
    _stInput.u32Addr = 0;
    _stInput.u32Size = 0;
    _stInput.eMemoryType = 0;

    //Clear Output
    _stOutput.u32Addr = 0;
    _stOutput.u32Size = 0;
    _stOutput.eMemoryType = 0;

    memset(_u8Key, 0, sizeof(_u8Key));
    memset(_u8IV, 0, sizeof(_u8IV));

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_PHY u32miu0addr , MS_PHY u32miu1addr , MS_U32 u32miunum)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    MS_U32 i = 0;

    ret = MDrv_CIPHER_Init();

    for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
    {
        memset(&_CIPHER_Resource[i], 0, sizeof(CIPHER_RESOURCE));
    }

    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Init(MS_PHY u32miu0addr , MS_PHY u32miu1addr , MS_U32 u32miunum)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_INIT pArgs;

    if(pu32AesdmaInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pu32AesdmaInst, 0x0, NULL))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaOpen CIPHER fail");
            return DRVAESDMA_FAIL;
        }
    }

    pArgs.u32miu0addr = u32miu0addr;
    pArgs.u32miu1addr = u32miu1addr;
    pArgs.u32miunum = u32miunum;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Init, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Init(u32miu0addr, u32miu1addr, u32miunum);
#endif
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}


DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
    //DRV_CIPHER_ALGO stAlgo;
    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Cipher mode[%d], Decrypt[%d]!", eMode, bDescrypt);

    bDES_Key = FALSE;
    switch(eMode)
    {
        case E_DRVAESDMA_CIPHER_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_DES_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_DES_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_DES_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_TDES_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTR:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CTR;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_DES_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;

        case E_DRVAESDMA_CIPHER_DES_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_DES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            bDES_Key = TRUE;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTS_CBC:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        case E_DRVAESDMA_CIPHER_TDES_CTS_ECB:
            _stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES;
            _stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            _stAlgo.eResAlgo = E_CIPHER_RES_CTS;
            _stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;
        default:
            _stAlgo.eMainAlgo = 0;
            _stAlgo.eSubAlgo = 0;
            _stAlgo.eResAlgo = 0;
            _stAlgo.eSBAlgo = 0;
            break;
    }

    _isDecrypt = bDescrypt;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Algo 0x%x 0x%x 0x%x 0x%x",
        _stAlgo.eMainAlgo, _stAlgo.eSubAlgo,_stAlgo.eResAlgo,_stAlgo.eSBAlgo);

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SEL_ENG pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.bDescrypt = bDescrypt;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SelEng(eMode, bDescrypt);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
	DRVAESDMA_RESULT ret = DRVAESDMA_OK;
	if(pu8Key && (u32Len <= HAL_CRYPTODMA_KEYLEN_MAX) && (u32Len > 0))
    {
        // Key from ACPU
        _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;

        memcpy(_u8Key, (MS_U8*)pu8Key, u32Len);
        _CipherKey.pu8KeyData = _u8Key;

        _CipherKey.u8KeyLen = u32Len;
        _CipherKey.u8KeyIdx = 0;
    }
    else
    {
        //Key from KL
        _CipherKey.eKeySrc = E_CIPHER_KSRC_KL;
        _CipherKey.pu8KeyData = 0;
        _CipherKey.u8KeyLen = 0;
        _CipherKey.u8KeyIdx = 0;  // always use 0
    }

	if(u32Len > HAL_CRYPTODMA_KEYLEN_MAX)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Invalid Key Length!");
        ret = DRVAESDMA_INVALID_PARAM;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Key Source[%d], Key Length[%u], Key Index[%u]",
        _CipherKey.eKeySrc, _CipherKey.u8KeyLen, _CipherKey.u8KeyIdx);

    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_KEY_EX pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.pu8Key = pu8Key;
    pArgs.u32Len = u32Len;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetKey_Ex, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey_Ex(pu8Key, u32Len);
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
   	 DRVAESDMA_RESULT ret = DRVAESDMA_OK;

	if(pu8IV && (u32Len <= HAL_CRYPTODMA_KEYLEN_MAX) && (u32Len > 0))
    {
        memcpy(_u8IV, pu8IV, u32Len);
        _CipherKey.pu8IVData = _u8IV;
        _CipherKey.u8IVLen = u32Len;
    }
    else
    {
        _CipherKey.pu8IVData = 0;
        _CipherKey.u8IVLen = 0;
    }

	if(u32Len > HAL_CRYPTODMA_KEYLEN_MAX)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Invalid IV Length!");
        ret = DRVAESDMA_INVALID_PARAM;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IV Length[%u]", _CipherKey.u8IVLen);

    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_IV_EX pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.pu8IV = pu8IV;
    pArgs.u32Len = u32Len;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetIV_Ex, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetIV_Ex(pu8IV, u32Len);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
    if(pCipherKey)
    {
        MS_U32 u32Tmp[4] = {0};
        if(!bDES_Key)
        {
		    u32Tmp[3] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey);
            u32Tmp[2] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 4);
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 8);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 12);
        }
        else
        {
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pCipherKey + 4);

        }
		// Key from ACPU
        _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;

        memcpy(_u8Key,(MS_U8 *)u32Tmp, 16);
        _CipherKey.pu8KeyData = _u8Key;
        _CipherKey.u8KeyLen = 16;
        _CipherKey.u8KeyIdx = 0;
    }
    else
    {
        //Key from KL
        memset(_u8Key, 0, 16);
        _CipherKey.eKeySrc = E_CIPHER_KSRC_KL;
        _CipherKey.pu8KeyData = 0;
        _CipherKey.u8KeyLen = 16;
        _CipherKey.u8KeyIdx = 0;
    }

	DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Key Source[%d], Key Length[%u], Key Index[%u], DES Key[%d]",
        _CipherKey.eKeySrc, _CipherKey.u8KeyLen, _CipherKey.u8KeyIdx, bDES_Key);

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetKey, pCipherKey);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey(pCipherKey);
#endif
}

DRVAESDMA_RESULT MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetSecureKey(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetSecureKey, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetSecureKey();
#endif
}

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetSecuredKeyIndex (MS_U8 u8KeyIdx)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
    if(pInitVector)
    {
		MS_U32 u32Tmp[4] = {0};
        if(!bDES_Key)
        {
		    u32Tmp[3] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector);
            u32Tmp[2] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 4);
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 8);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 12);
        }
        else
        {
            u32Tmp[1] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector);
            u32Tmp[0] = _AESDMA_MSBF2Native((MS_U8 *)pInitVector + 4);
        }
		memcpy(_u8IV,(MS_U8 *)u32Tmp, 16);
        _CipherKey.pu8IVData = _u8IV;
        _CipherKey.u8IVLen = 16;
    }
    else
    {
        memset(_u8IV, 0, 16);
        _CipherKey.pu8IVData = 0;
        _CipherKey.u8IVLen = 0;
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:IV Length[%u], DES IV[%d]", _CipherKey.u8IVLen, bDES_Key);

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetIV, pInitVector);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetIV(pInitVector);
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_SET_PS pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.u32PTN = u32PTN;
    pArgs.u32Mask = u32Mask;
    pArgs.bPSin_Enable = bPSin_Enable;
    pArgs.bPSout_Enable = bPSout_Enable;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetPS, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetPS(u32PTN, u32Mask, bPSin_Enable, bPSout_Enable);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetDefaultCAVid(MS_U32 u32CAVid)
{
    DEBUG_CIPHER(CIPHER_DBG_INFO, "Set CAVid [0x%x]",(unsigned int)u32CAVid);

    if((u32CAVid > HAL_CRYPTODMA_CAVID_MAX) || (u32CAVid == 0))
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    _u32CAVid = u32CAVid;

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetDefaultCAVid(MS_U32 u32CAVid)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_CAVID pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.u32CAVid = u32CAVid;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetDefaultCAVid, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetDefaultCAVid(u32CAVid);
#endif
}


DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_PSRelease(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_PSRelease, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_PSRelease();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_PHY u32FileinAddr, MS_U32 u32FileInNum, MS_PHY u32FileOutSAddr, MS_PHY u32FileOutEAddr)
{

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:FileIn address[0x%x], FileIn num[0x%x], FileOut start address[0x%x], FileOut end address[0x%x]",
        (unsigned int)u32FileinAddr,
        (unsigned int)u32FileInNum,
        (unsigned int)u32FileOutSAddr,
        (unsigned int)u32FileOutEAddr);

    _stInput.u32Addr = u32FileinAddr;
    _stInput.u32Size = u32FileInNum;
    _stInput.eMemoryType = E_CIPHER_DRAM;

    _stOutput.u32Addr = u32FileOutSAddr;
    _stOutput.u32Size = (u32FileOutEAddr - u32FileOutSAddr) + 1;
    _stOutput.eMemoryType = E_CIPHER_DRAM;

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_SetFileInOut(MS_PHY u32FileinAddr, MS_U32 u32FileInNum, MS_PHY u32FileOutSAddr, MS_PHY u32FileOutEAddr)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_FILE_INOUT pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.u32FileinAddr = u32FileinAddr;
    pArgs.u32FileInNum  = u32FileInNum;
    pArgs.u32FileOutSAddr = u32FileOutSAddr;
    pArgs.u32FileOutEAddr = u32FileOutEAddr;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetFileInOut(u32FileinAddr, u32FileInNum, u32FileOutSAddr, u32FileOutEAddr);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart)
{
    DRV_CIPHER_DMACFG stCfg = {{0}};
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    MsOS_FlushMemory();

    stCfg.stAlgo   = _stAlgo;
    stCfg.u32CAVid = _u32CAVid;
    stCfg.stKey    = _CipherKey;
    stCfg.stInput  = _stInput;
    stCfg.stOutput = _stOutput;

    stCfg.bDecrypt = _isDecrypt;

    ret = MDrv_CIPHER_DMACalc(stCfg, &_RetId);

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Decrypt[%d], CmdId[%u]!", _isDecrypt, _RetId);

    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Start(MS_BOOL bStart)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Start, &bStart);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Start(bStart);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    ret = MDrv_CIPHER_Reset();

    if(DRV_CIPHER_OK != ret)
        return ret;

    // Default Key from ACPU
    _CipherKey.eKeySrc = E_CIPHER_KSRC_CPU;
    _CipherKey.pu8KeyData = _u8ZeroData;
    _CipherKey.u8KeyLen = 16;
    _CipherKey.u8KeyIdx = 0;

    //Clear IV
    _CipherKey.pu8IVData = _u8ZeroData;
    _CipherKey.u8IVLen = 16;

    //Clear Algo
    _stAlgo.eMainAlgo = 0;
    _stAlgo.eSubAlgo  = 0;
    _stAlgo.eResAlgo  = 0;
    _stAlgo.eSBAlgo   = 0;

    //Clear Input
    _stInput.u32Addr = 0;
    _stInput.u32Size = 0;
    _stInput.eMemoryType = 0;

    //Clear Output
    _stOutput.u32Addr = 0;
    _stOutput.u32Size = 0;
    _stOutput.eMemoryType = 0;

    memset(_u8Key, 0, sizeof(_u8Key));
    memset(_u8IV, 0, sizeof(_u8IV));

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:AESDMA Reset");

    return 0;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Reset(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;

    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Reset();
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(MS_U32 *u32RetVal)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_GetStatus(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(MS_U32 *u32RetVal)
{
    MS_BOOL bDone = FALSE;
    MS_U32 u32Ret = 0;

    bDone = MDrv_CIPHER_IsDMADone(_RetId, &u32Ret);

    if(TRUE == bDone)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:CmdId[%u] Not Done!", _RetId);
        return DRVAESDMA_FAIL;
    }
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_IsFinished(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_IsFinished, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    MS_U32 u32Tmp = 0;
    return _MDrv_AESDMA_IsFinished(&u32Tmp);
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(MS_U32 *u32RetVal)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC MS_U32 MDrv_AESDMA_GetPSMatchedByteCNT(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC MS_U32 MDrv_AESDMA_GetPSMatchedPTN(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_GetPSMatchedPTN, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_GetPSMatchedPTN();
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_NOTIFY pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.eEvents = eEvents;
    pArgs.pfCallback = pfCallback;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Notify, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Notify(eEvents, pfCallback);
#endif
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel)
{
    CIPHER_DBGMSG_LEVEL eDbgLevel = _CIPHER_TransDbgLevel(DbgLevel);

    MDrv_CIPHER_SetDbgLevel(eDbgLevel);

    return DRVAESDMA_OK;
}

DLL_PUBLIC  DRVAESDMA_RESULT MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
    if(NULL == u32PABuf || 0 == u32Size)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Buffer or Size is zero!");
        return DRVAESDMA_INVALID_PARAM;
    }

    ptrdiff_t n = (ptrdiff_t)u32PABuf;
    MS_VIRT u8VABuf = (MS_VIRT)MsOS_PA2KSEG1((MS_PHY)n);

    if( FALSE == HAL_CIPHER_Misc_Random((MS_U8 *)u8VABuf,u32Size))
    {
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_RAND pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.u32PABuf = u32PABuf;
    pArgs.u32Size = u32Size;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Rand, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Rand(u32PABuf, u32Size);
#endif
}


DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
    DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:Not Support!");
    return 0;
}

DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY u32PAInBuf, MS_U32 u32Size, MS_PHY u32PAOutBuf)
{
    DRV_CIPHER_HASHCFG stCfg = {0};
    CIPHER_HASH_ALGO eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    MS_U32 u32DgstSize = 20;
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:eMode[%d], InBuffer[0x%08x], Size[%u], OutBuffer[0x%08x]!",
        eMode, (unsigned int)u32PAInBuf, u32Size, (unsigned int)u32PAOutBuf);

    switch(eMode)
    {
    case E_DRVAESDMA_SHA1:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
        break;
    case E_DRVAESDMA_SHA256:
        eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        u32DgstSize = 32;
        break;
    default:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
    }

    stCfg.eAlgo              = eAlgo;
    stCfg.stInput.u32Addr    = u32PAInBuf;
    stCfg.stInput.u32Size    = u32Size;
    stCfg.pu8Digest_Buf      = (MS_U8*)(MsOS_PA2KSEG1(u32PAOutBuf));
    stCfg.u32Digest_Buf_Size = u32DgstSize;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASH(stCfg, &_RetId))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH Fail!");
        return DRVAESDMA_FAIL;
    }

    while((FALSE == MDrv_CIPHER_IsHASHDone(_RetId, &u32Ret)) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
    #ifdef MSOS_TYPE_NUTTX
        MsOS_DelayTaskUs(200);
    #elif defined MSOS_TYPE_OPTEE

    #else
        MsOS_DelayTask(20);
    #endif
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning;HASH timeout!");
       return DRVAESDMA_FAIL;
    }

    //Reverse MSB to LSB for old API compatible
    _ReverseU8Buf((MS_U8 *)stCfg.pu8Digest_Buf, u32DgstSize );

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY u32PAInBuf, MS_U32 u32Size, MS_PHY u32PAOutBuf)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.u32PAInBuf = u32PAInBuf;
    pArgs.u32Size = u32Size;
    pArgs.u32PAOutBuf = u32PAOutBuf;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_SHA_Calculate, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_SHA_Calculate(eMode, u32PAInBuf, u32Size, u32PAOutBuf);
#endif
}

DRVAESDMA_RESULT _MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{
    DRV_CIPHER_HASHCFG_MANUAL stCipherCfg = {0};
    CIPHER_HASH_ALGO eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    MS_U32 u32DgstSize = 20;
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:eMode[%d], Stage[%d], Done Bytes[%u]!",
        stCfg.eMode, eStage, u32DoneBytes);
    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:InAddress[0x%08x], InSize[%u], OutAddress[0x%08x], OutSize[%u]!",
        (unsigned int)stCfg.stInput.u32Addr, stCfg.stInput.u32Size, (unsigned int)stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size);

    switch(stCfg.eMode)
    {
    case E_DRVAESDMA_SHA1:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
        break;
    case E_DRVAESDMA_SHA256:
        eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        u32DgstSize = 32;
        break;
    default:
        eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        u32DgstSize = 20;
    }

    //Algo
    stCipherCfg.eAlgo = eAlgo;

    //Input
    stCipherCfg.stInput.u32Addr = stCfg.stInput.u32Addr;
    stCipherCfg.stInput.u32Size = stCfg.stInput.u32Size;

    //Output
    stCipherCfg.pu8Digest_Buf      = (MS_U8*)stCfg.stOutput.u32Addr;
    stCipherCfg.u32Digest_Buf_Size = stCfg.stOutput.u32Size;

    //Stage
    stCipherCfg.eStage = eStage;

    //StartBytes
    stCipherCfg.u32StartByte = u32DoneBytes;

    //IV
    stCipherCfg.pu8SetIV = pu8SetIV;

    if(DRV_CIPHER_OK != MDrv_CIPHER_HASHManual(stCipherCfg, &_RetId))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:HASH Fail!");
        return DRVAESDMA_FAIL;
    }

    while((FALSE == MDrv_CIPHER_IsHASHDone(_RetId, &u32Ret)) && u32WaitCnt < HASH_TIMEOUT_VALUE)
    {
    #ifdef MSOS_TYPE_NUTTX
        MsOS_DelayTaskUs(200);
    #elif defined MSOS_TYPE_OPTEE

    #else
        MsOS_DelayTask(20);
    #endif
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HASH_TIMEOUT_VALUE)
    {
       DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning;HASH timeout!");
       return DRVAESDMA_FAIL;
    }

    //Reverse MSB to LSB for old API compatible
    if(eStage == E_DRVAESDMA_HASH_STAGE_LAST)
    {
        _ReverseU8Buf((MS_U8 *)stCipherCfg.pu8Digest_Buf, u32DgstSize );
    }

    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE_MANUAL pArgs;
    if(pu32AesdmaInst == NULL)
    {
         DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    if(NULL != stCfg.stInput.u32Addr)
    {
        stCfg.stInput.u32Addr = MsOS_VA2PA(stCfg.stInput.u32Addr);
    }
    pArgs.stCfg = stCfg;
    pArgs.eStage = eStage;
    pArgs.u32DoneBytes = u32DoneBytes;
    pArgs.pu8SetIV = pu8SetIV;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_SHA_CalculateManual, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else

    if(NULL != stCfg.stInput.u32Addr)
    {
        stCfg.stInput.u32Addr = MsOS_VA2PA(stCfg.stInput.u32Addr);
    }
    return _MDrv_SHA_CalculateManual(stCfg, eStage, u32DoneBytes, pu8SetIV);
#endif
}


#ifdef MOBF_ENABLE
DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_DmaOnly(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_MOBF_OneWay(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}
#endif

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC MS_U8 MDrv_AESDMA_Parser_QueryPidCount(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DLL_PUBLIC MS_BOOL MDrv_AESDMA_Lock(void)
{
    MsOS_ObtainMutex(_AESDMAMtx, MSOS_WAIT_FOREVER);
    return TRUE;
}

DLL_PUBLIC MS_BOOL MDrv_AESDMA_Unlock(void)
{
    MsOS_ReleaseMutex(_AESDMAMtx);
    return TRUE;
}

MS_U32 MDrv_AESDMA_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE u16PreAESDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Go to SUSPEND\n");

#if !defined (MSOS_TYPE_NOS) && !defined (MCU_AEON) && !defined (MSOS_TYPE_OPTEE)
        MsOS_DeleteEventGroup(_s32CIPHEREventId);
#endif
       // MsOS_DeleteTask(_s32AESDMATaskId);
        MsOS_DeleteSemaphore(_CipherSemaphore);
        MsOS_DeleteMutex(_CipherMtx);
        MsOS_DeleteMutex(_AESDMAMtx);

        u16PreAESDMAPowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;

		_bCIPHERDrv_Inited = FALSE;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreAESDMAPowerState == E_POWER_SUSPEND)
        {
            DEBUG_CIPHER(CIPHER_DBG_WARN, "Go to RESUME\n");
            _bCIPHERDrv_Inited = FALSE;
            MDrv_CIPHER_Init();


            u16PreAESDMAPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success

}

DRVAESDMA_RESULT MDrv_AESDMA_Exit(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_IsSecretKeyInNormalBank(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_ParserStart(MS_BOOL bStart)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_NormalReset(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_NormalReset(void)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_HDCP_ProcessCipher(MS_U8 u8Idx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_HDCP_GetHdcpCipherState(MS_U8 u8Idx, MS_U8 *pu8State)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_HDMI_GetM0(MS_U32 u32InputPortType, MS_U8 *pu8Data)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey)
{
    DRVAESDMA_RESULT ret = DRVAESDMA_OK;

    if(HAL_CIPHER_KEYSLOT_BASE > u32Index || (HAL_CIPHER_KEYSLOT_BASE + HAL_CRYPTODMA_DMA_KEY_SLOT - 1) < u32Index)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Invalid Key index[0x%04u]!", (unsigned int)u32Index);
        return DRVAESDMA_INVALID_PARAM;
    }

    //Key from KL
    _CipherKey.eKeySrc = E_CIPHER_KSRC_KL;
    _CipherKey.pu8KeyData = 0;
    _CipherKey.u8KeyLen = 0;
    _CipherKey.u8KeyIdx = u32Index - HAL_CIPHER_KEYSLOT_BASE; //SK0:0x10, SK1:0x11

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:Key Source[%d], Key Length[%u], Key Index[%u]",
        _CipherKey.eKeySrc, _CipherKey.u8KeyLen, _CipherKey.u8KeyIdx);

    return ret;
}

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_KEY_INDEX pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.bOddKeyEn = bIsOddKey;
    pArgs.u32KeyIndex = u32Index;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_Set_Key_Index, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKeyIndex(u32Index, bIsOddKey);
#endif
}

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_SetAesCtr64 (MS_BOOL bCtr64En)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_AllocKeySlot (MS_U32 *pu32Index)
{
    MS_U32 i = 0;

    if(NULL == pu32Index)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:pu32Index is NULL");
        return DRVAESDMA_INVALID_PARAM;
    }

    for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
    {
        if(FALSE == _CIPHER_Resource[i].bUsed)
        {
            _CIPHER_Resource[i].bUsed = TRUE;
            *pu32Index = i + HAL_CIPHER_KEYSLOT_BASE;
            break;
        }
    }

    if(HAL_CRYPTODMA_DMA_KEY_SLOT == i)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:No empty key slot");
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_AllocKeySlot (MS_U32 *pu32Index)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_ALLOC_KEY_SLOT pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.pu32KeyIndex = pu32Index;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_AllocKeySlot, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_AllocKeySlot(pu32Index);
#endif

}

DRVAESDMA_RESULT _MDrv_AESDMA_FreeKeySlot (MS_U32 u32Index)
{
    MS_U32 i = 0;

    if(HAL_CIPHER_KEYSLOT_BASE > u32Index || (HAL_CIPHER_KEYSLOT_BASE + HAL_CRYPTODMA_DMA_KEY_SLOT - 1) < u32Index)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Invalid Key index[0x%04u]!", (unsigned int)u32Index);
        return DRVAESDMA_INVALID_PARAM;
    }

    i = u32Index - HAL_CIPHER_KEYSLOT_BASE;

    if(DRV_CIPHER_OK != MDrv_CIPHER_ResetKey(_u32CAVid, i))
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Reset key fail, CAVid=[0x%04u], index=[0x%04u]!", (unsigned int)_u32CAVid, (unsigned int)u32Index);
        return DRVAESDMA_FAIL;
    }

    _CIPHER_Resource[i].bUsed = FALSE;

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT SYMBOL_WEAK MDrv_AESDMA_FreeKeySlot (MS_U32 u32Index)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_KEY_INDEX pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.u32KeyIndex = u32Index;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_AESDMA_FreeKeySlot, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_FreeKeySlot(u32Index);
#endif
}

#ifndef MSOS_TYPE_NUTTX
DRVAESDMA_RESULT MDrv_AESDMA_SetSecurityInfo(DrvAESDMA_SecureInfo *pSecureInfo)
{
    DEBUG_CIPHER(CIPHER_DBG_ERR, "Not support!");
    return DRVAESDMA_OK;
}
#endif

MS_BOOL _MDrv_RSA_Init(void)
{
    MS_VIRT u32Bank;
    MS_PHY  u32BankSize;

    if(_bRSA_Init == FALSE)
    {
        if(FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Get MMIO base fail");
            return FALSE;
        }

        HAL_RSA_SetBank(u32Bank);

        _CipherRSAMtx = MsOS_CreateMutex( E_MSOS_FIFO, (char*)"CIPHER RSA mutex", MSOS_PROCESS_SHARED);
        _bRSA_Init = TRUE;
    }
    return TRUE;
}

DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 u32KeyLen = 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;

    if( pstSign==NULL || pstKey==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:RSA Init fail");
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);

    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "RSA Mode[%d]", eMode);

    HAL_RSA_LoadSram((pstKey->u32KeyE), E_RSA_ADDRESS_E);
    HAL_RSA_LoadSram((pstKey->u32KeyN), E_RSA_ADDRESS_N);
    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);

    HAL_RSA_SetKeyLength((u32KeyLen/RSA_UNIT_SIZE)-1);
    HAL_RSA_SetKeyType(E_RSA_USE_SW_KEY, u8Public); //sw key, public key
    HAL_RSA_ExponetialStart();

RSA_CAL_FINISH:
    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    RSA_CALCULATE pArgs;

    if(pu32AesdmaInst == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pu32AesdmaInst, 0x0, NULL))
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaOpen CIPHER fail");
            return DRVAESDMA_FAIL;
        }
    }

    pArgs.pstSign = pstSign;
    pArgs.pstKey = pstKey;
    pArgs.eMode = eMode;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_RSA_Calculate, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Calculate(pstSign, pstKey, eMode);
#endif
}
DRVAESDMA_RESULT MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U8  u8KeySrc= 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;
    MS_U32 u32SwitchValue;

    if( pstSign==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_HW_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:RSA Init fail");
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_HW_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);

    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA_HWKEY_PRIVATE:
        {
            u8KeySrc = E_RSA_HW_PRI_KEY;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC1:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY1;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC2:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY2;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_ROM_PRIVATE:
        {
            u8KeySrc = E_RSA_HW_PRI_KEY;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_ROM_PUBLIC:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY1;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_HW_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:RSA Mode[%d], Key Source[%u], Public[%u]",
        eMode, u8KeySrc, u8Public);

    if(eMode == E_DRVAESDMA_RSA_HWKEY_ROM_PRIVATE || eMode == E_DRVAESDMA_RSA_HWKEY_ROM_PUBLIC)
    {
        HAL_RSA_Hwkey_Switch();

        u32SwitchValue = HAL_RSA_Read_Hwkey_Switch();

        if( u32SwitchValue != 1 )
        {
            DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:RSA Hwkey Switch fail");
            ret = DRVAESDMA_FAIL;
            goto RSA_CAL_HW_FINISH;
        }
    }

    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);

    HAL_RSA_SetKeyType(E_RSA_USE_HW_KEY, u8Public);
    HAL_RSA_Rootkey_Sel(u8KeySrc);
    HAL_RSA_ExponetialStart();

RSA_CAL_HW_FINISH:
    return ret;
}

DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void)
{
    RSAInitChk();

    if(HAL_RSA_Get_RSA_IsFinished()==0)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:RSA Not Done!");
        return DRVAESDMA_FAIL;
    }
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_IsFinished(void)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    MS_U32 u32TimeOut = RSA_DEFAULT_TIMEOUT_VALUE;

    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_RSA_IsFinished, (void*)&u32TimeOut);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_WARN, "Warning:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_IsFinished();
#endif
}

DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 i = 0;
    MS_U32 u32KeyLen = 0;
    MS_U32 *pu32RSAOut = (MS_U32 *)(pstRSAOut->u32RSAOut);

    RSAInitChk();

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_OUTPUT_FINISH;
        }
    }

    DEBUG_CIPHER(CIPHER_DBG_INFO, "INFO:RSA Mode[%d], Key length[%u]", eMode, u32KeyLen);

    HAL_RSA_SetFileOutAddr(64-(u32KeyLen/sizeof(MS_U32)));
    HAL_RSA_Ind32Ctrl(RAM_READ);
    HAL_RSA_FileOutStart();
	for( i = 0; i<(u32KeyLen/RSA_UNIT_SIZE); i++)
	{
		pu32RSAOut[(u32KeyLen/RSA_UNIT_SIZE)-1-i] = HAL_RSA_FileOut();
	}

RSA_OUTPUT_FINISH:
    MsOS_ReleaseMutex(_CipherRSAMtx);
    return ret;
}
DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
#if CIPHER_UTOPIA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    RSA_OUTPUT pArgs;
    if(pu32AesdmaInst == NULL)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:Cipher instance NULL");
        return DRVAESDMA_FAIL;
    }

    pArgs.eMode = eMode;
    pArgs.pstRSAOut = pstRSAOut;

    u32Ret = UtopiaIoctl(pu32AesdmaInst, MDrv_CMD_RSA_Output, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DEBUG_CIPHER(CIPHER_DBG_ERR, "Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Output(eMode, pstRSAOut);
#endif
}


