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
/// file    drvPVR_IframeLUT.c
/// @brief  Secure PVR Iframe LUT Driver Interface
/// @author MStar Semiconductor,Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsVersion.h"
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "asmCPU.h"
#include "drvMMIO.h"
#include "drvPVR_IframeLUT.h"
#include "drvPVR_IframeLUT_private.h"
#include "drvPVR_IframeLUT_v2.h"
#include "halPVR_IframeLUT.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_PVR_IFRAMELUT_UTPA2_ENABLE)
#define PVR_IFRAMELUT_UTOPIA20 (1)
#else
#define PVR_IFRAMELUT_UTOPIA20 (0)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PVR_IFRAMELUT_MUTEX_WAIT_TIME                     0xFFFFFFFFUL

#define PVR_IFRAMELUT_DBGERR(_f,_a...)                    {if(_iframelutdbglevel >= E_PVR_IFRAMELUT_DBG_ERR) {printf(_f, ##_a);}}
#define PVR_IFRAMELUT_DBGINFO(_f,_a...)                   {if(_iframelutdbglevel >= E_PVR_IFRAMELUT_DBG_INFO){printf(_f, ##_a);}}

#define PVR_IFRAMELUT_ASSERT(_bool)                        if(!(_bool))   \
                                                           {              \
                                                               if(_iframelutdbglevel >= E_PVR_IFRAMELUT_DBG_INFO)    \
                                                               {  printf("[%s][%04d] ASSERT ERR\n", __FUNCTION__, __LINE__);  }         \
                                                               return E_PVR_IFRAMELUT_FAIL;                          \
                                                           }

#define _PVR_IFRAMELUT_ENTRY()                             if(!MsOS_ObtainMutex(_lut_res->s32PVR_IframeLUTMutexId, PVR_IFRAMELUT_MUTEX_WAIT_TIME)) \
                                                           {                                                                       \
                                                               return E_PVR_IFRAMELUT_RESULT_FAIL;                                 \
                                                           }

#define _PVR_IFRAMELUT_RETURN(_ret)                        do{                                                                \
                                                               MsOS_ReleaseMutex(_lut_res->s32PVR_IframeLUTMutexId);           \
                                                               return _ret;                                                   \
                                                           }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _PVR_IFRAMELUT_RESOURCE_PRIVATE
{
    MS_S32 s32PVR_IframeLUTMutexId;
    MS_BOOL bLUTBankInit;
    MS_BOOL bLUTUsed[LUT_NUM];
    MS_BOOL bSWInit;
} PVR_IFRAMELUT_RESOURCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifndef PVR_IFRAMELUT_UTOPIA20
static PVR_IFRAMELUT_RESOURCE_PRIVATE _tmp_res =
{
    .s32PVR_IframeLUTMutexId = -1,
    .bLUTBankInit = FALSE,
    .bLUTUsed = {[0 ... (LUT_NUM-1)] = FALSE},
    .bSWInit = TRUE
};
static PVR_IFRAMELUT_RESOURCE_PRIVATE *_lut_res = &_tmp_res;
#else
static PVR_IFRAMELUT_RESOURCE_PRIVATE *_lut_res = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static EN_PVR_IFRAMELUT_DBGLEVEL _iframelutdbglevel = E_PVR_IFRAMELUT_DBG_INFO;

#if PVR_IFRAMELUT_UTOPIA20
static void *_pInstantancePVRIframeLUT = NULL;
static void *_pAttributePVRIframeLUT = NULL;
#endif

#if PVR_IFRAMELUT_UTOPIA20
static MS_BOOL _PVR_IframeLUT_UtopiaOpen(void)
{
    if(_pInstantancePVRIframeLUT == NULL)
    {
        // RegisterToUtopia will be called in Utopia.c
        //PVR_IFRAMELUTRegisterToUtopia();

        #ifdef MSOS_TYPE_LINUX_KERNEL
        PVR_IFRAMELUT_DBGINFO("[%s][%04d] Kernel Init\n", __FUNCTION__, __LINE__);
        #else
        PVR_IFRAMELUT_DBGINFO("[%s][%04d] User Init\n", __FUNCTION__, __LINE__);
        #endif

        if(UtopiaOpen(MODULE_PVR_IFRAMELUT, &_pInstantancePVRIframeLUT, 0, _pAttributePVRIframeLUT) !=  UTOPIA_STATUS_SUCCESS)
        {
            PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaOpen fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        PVR_IFRAMELUT_DBGINFO("[%s][%04d] _pInstantancePVRIframeLUT = %p\n", __FUNCTION__, __LINE__, _pInstantancePVRIframeLUT);
    }
    return TRUE;
}
#endif

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Init(void)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_Init, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_Init();
#endif
}

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Open(PVR_IFRAMELUT_Cfg *pstPVR_IframeLUTCfg)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    PVR_IFRAMELUT_OPEN_PARAM *pArgs = pstPVR_IframeLUTCfg;

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_Open, (void *)pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_Open(pstPVR_IframeLUTCfg);
#endif
}

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_SetVdecType(MS_U32 u32PvrEng, EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    PVR_IFRAMELUT_SETVDECTYPE_PARAM args;
    args.u32PvrEng = u32PvrEng;
    args.eVdecType = eVdecType;

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_SetVdecType, (void *)&args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_SetVdecType(u32PvrEng, eVdecType);
#endif
}

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_GetWritePtr(MS_U32 u32PvrEng, MS_U32 *pu32WritePtr)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    PVR_IFRAMELUT_GETWRITEPTR_PARAM args;
    args.u32PvrEng = u32PvrEng;
    args.pu32WritePtr = pu32WritePtr;

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_GetWritePtr, (void *)&args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_GetWritePtr(u32PvrEng, pu32WritePtr);
#endif
}

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Close(MS_U32 u32PvrEng)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    PVR_IFRAMELUT_CLOSE_PARAM args = u32PvrEng;

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_Close, (void *)&args);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_Close(u32PvrEng);
#endif
}

EN_PVR_IFRAMELUT_RESULT MDrv_PVR_IframeLUT_Exit(void)
{
#if PVR_IFRAMELUT_UTOPIA20
    if(_pInstantancePVRIframeLUT == NULL)
    {
        _PVR_IframeLUT_UtopiaOpen();
    }

    MS_U32 u32Ret = UtopiaIoctl(_pInstantancePVRIframeLUT, E_MDrv_CMD_PVR_IframeLUT_Exit, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] UtopiaIoctl return value 0x%X\n", __FUNCTION__, __LINE__, (unsigned int)u32Ret);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
#else
    return _MDrv_PVR_IframeLUT_Exit();
#endif
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_Init(void)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%04d]\n", __FUNCTION__, __LINE__);

    // Set bank only once
    if(!_lut_res->bLUTBankInit)
    {
        MS_VIRT u32Base;
        MS_PHY u32Size;
        MDrv_MMIO_Init();
        if(FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
        {
            PVR_IFRAMELUT_DBGERR("[%s][%04d] Get MS_MODULE_PM fail\n", __FUNCTION__, __LINE__);
            MS_ASSERT(0);
        }

        // init LUT module
        if(!HAL_PVR_IframeLUT_Init(u32Base))
        {
            PVR_IFRAMELUT_DBGERR("[%s][%04d] HAL_PVR_IframeLUT_Init fail 0x%08X\n", __FUNCTION__, __LINE__, u32Base);
            return E_PVR_IFRAMELUT_RESULT_NOT_SUPPORT;
        }

        // Create mutex if init LUT module success
        if(_lut_res->s32PVR_IframeLUTMutexId == -1)
        {
            _lut_res->s32PVR_IframeLUTMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex Iframe LUT", MSOS_PROCESS_SHARED);
            if(_lut_res->s32PVR_IframeLUTMutexId == -1)
            {
                PVR_IFRAMELUT_DBGERR("[%s][%04d] MsOS_CreateMutex failed", __FUNCTION__, __LINE__);
                return E_PVR_IFRAMELUT_RESULT_FAIL;
            }
        }

        _lut_res->bLUTBankInit = TRUE;

        PVR_IFRAMELUT_DBGINFO("[%s][%04d] Module set bank 0x%08X\n", __FUNCTION__, __LINE__, u32Base);
        return E_PVR_IFRAMELUT_RESULT_OK;
    }

    PVR_IFRAMELUT_DBGINFO("[%s][%04d] Module has been init once\n", __FUNCTION__, __LINE__);
    return E_PVR_IFRAMELUT_RESULT_OK;
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_Open(PVR_IFRAMELUT_Cfg *pstPVR_IframeLUTCfg)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%04d]\n", __FUNCTION__, __LINE__);
    _PVR_IFRAMELUT_ENTRY();

    if(_lut_res->bLUTUsed[pstPVR_IframeLUTCfg->u32PvrEng] == TRUE)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] LUT %d has been opened\n", __FUNCTION__, __LINE__, pstPVR_IframeLUTCfg->u32PvrEng);
        _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
    }

    if(!HAL_PVR_IframeLUT_Open(pstPVR_IframeLUTCfg->u32PvrEng, pstPVR_IframeLUTCfg->u32BufferStartAddr, pstPVR_IframeLUTCfg->u32BufferSize, pstPVR_IframeLUTCfg->eVdecType, 1))
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] HAL_PVR_IframeLUT_Open fail\n", __FUNCTION__, __LINE__);
        _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
    }
    _lut_res->bLUTUsed[pstPVR_IframeLUTCfg->u32PvrEng] = TRUE;
    _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_OK);
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_SetVdecType(MS_U32 u32PvrEng, EN_PVR_IFRAMELUT_VDEC_TYPE eVdecType)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%04d]\n", __FUNCTION__, __LINE__);

    _PVR_IFRAMELUT_ENTRY();

    PVR_IFRAMELUT_DBGINFO("[%s][%04d] PvrEng %d set vdec %d\n", __FUNCTION__, __LINE__, u32PvrEng, eVdecType);

    HAL_PVR_IframeLUT_VDEC_TYPE eHalVdecType;
    switch(eVdecType)
    {
        case E_PVR_IFRAMELUT_VDEC_TYPE_MPEG:
            eHalVdecType = E_HAL_PVR_IframeLUT_MPEG;
            break;
        case E_PVR_IFRAMELUT_VDEC_TYPE_H264:
            eHalVdecType = E_HAL_PVR_IframeLUT_H264;
            break;
        case E_PVR_IFRAMELUT_VDEC_TYPE_HEVC:
            eHalVdecType = E_HAL_PVR_IframeLUT_HEVC;
            break;
        case E_PVR_IFRAMELUT_VDEC_TYPE_AVS:
            eHalVdecType = E_HAL_PVR_IframeLUT_AVS;
            break;
        default:
            PVR_IFRAMELUT_DBGERR("[%s][%04d] Invalid Video Codec Type: %d\n", __FUNCTION__, __LINE__, eVdecType);
            _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_INVALID_PARAM);
    }
    if(!HAL_PVR_IframeLUT_SetVdecType(u32PvrEng, eHalVdecType))
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] HAL_PVR_IframeLUT_SetVdec fail\n", __FUNCTION__, __LINE__);
        _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
    }
    _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_OK);
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_GetWritePtr(MS_U32 u32PvrEng, MS_U32 *pu32WritePtr)
{
    // Check if this PVR engine (LUT channel) is currently in use
    if(_lut_res->bLUTUsed[u32PvrEng] == FALSE)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] PvrEng %d is not active!\n", __FUNCTION__, __LINE__, u32PvrEng);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }

    *pu32WritePtr = HAL_PVR_IframeLUT_GetWritePtr(u32PvrEng);
    if(*pu32WritePtr == 0)
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] PvrEng %d get write ptr fail!\n", __FUNCTION__, __LINE__, u32PvrEng);
        return E_PVR_IFRAMELUT_RESULT_FAIL;
    }
    PVR_IFRAMELUT_DBGINFO("[%s][%04d] PvrEng %d get write ptr 0x%08X\n", __FUNCTION__, __LINE__, u32PvrEng, *pu32WritePtr);
    return E_PVR_IFRAMELUT_RESULT_OK;
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_Close(MS_U32 u32PvrEng)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%04d]\n", __FUNCTION__, __LINE__);
    _PVR_IFRAMELUT_ENTRY();
    if(!HAL_PVR_IframeLUT_Close(u32PvrEng))
    {
        PVR_IFRAMELUT_DBGERR("[%s][%04d] HAL_PVR_IframeLUT_Close fail\n", __FUNCTION__, __LINE__);
        _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
    }
    _lut_res->bLUTUsed[u32PvrEng] = FALSE;
    _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_OK);
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_Exit(void)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%04d]\n", __FUNCTION__, __LINE__);
    _PVR_IFRAMELUT_ENTRY();

    MS_U8 u8LUTIndex = 0;
    for(u8LUTIndex = 0;u8LUTIndex < LUT_NUM;u8LUTIndex++)
    {
        if(_lut_res->bLUTUsed[u8LUTIndex] == TRUE)
        {
            PVR_IFRAMELUT_DBGINFO("[%s][%04d] LUT %u still in use\n", __FUNCTION__, __LINE__, u8LUTIndex);
            _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
        }
    }

    if(!HAL_PVR_IframeLUT_Exit())
    {
        PVR_IFRAMELUT_DBGINFO("[%s][%04d] HAL_PVR_IframeLUT_Exit fail\n", __FUNCTION__, __LINE__);
        _PVR_IFRAMELUT_RETURN(E_PVR_IFRAMELUT_RESULT_FAIL);
    }

    for(u8LUTIndex = 0;u8LUTIndex < LUT_NUM;u8LUTIndex++)
    {
        _lut_res->bLUTUsed[u8LUTIndex] = FALSE;
    }

    MsOS_ReleaseMutex(_lut_res->s32PVR_IframeLUTMutexId);
    MsOS_DeleteMutex(_lut_res->s32PVR_IframeLUTMutexId);
    _lut_res->s32PVR_IframeLUTMutexId = -1;
    return E_PVR_IFRAMELUT_RESULT_OK;
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_GetCap(PVR_IFRAMELUT_QUERY_TYPE eQueryType, void* pOutput)
{
    if(eQueryType == PVR_IFRAMELUT_CAP_RESOURCE_SIZE)
    {
        *((MS_U32*)pOutput) = sizeof(PVR_IFRAMELUT_RESOURCE_PRIVATE);
        return E_PVR_IFRAMELUT_RESULT_OK;
    }
    return E_PVR_IFRAMELUT_RESULT_FAIL;
}

EN_PVR_IFRAMELUT_RESULT _MDrv_PVR_IframeLUT_InitResource(void *pResourceAddr)
{
    _lut_res = (PVR_IFRAMELUT_RESOURCE_PRIVATE *)pResourceAddr;

    if(_lut_res->bSWInit != TRUE)
    {
        PVR_IFRAMELUT_DBGINFO("[%s][%04d] Init _lut_res\n", __FUNCTION__, __LINE__);
        _lut_res->s32PVR_IframeLUTMutexId = -1;
        _lut_res->bLUTBankInit = FALSE;
        MS_U8 u8Index = 0;
        for(u8Index = 0;u8Index < LUT_NUM;u8Index++)
        {
            _lut_res->bLUTUsed[u8Index] = FALSE;
        }
        _lut_res->bSWInit = TRUE;
    }
    else
    {
        PVR_IFRAMELUT_DBGINFO("[%s][%04d] Has Inited _lut_res\n", __FUNCTION__, __LINE__);
    }
    return E_PVR_IFRAMELUT_RESULT_OK;
}
