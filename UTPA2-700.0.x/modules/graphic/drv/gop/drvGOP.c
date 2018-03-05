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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!MStar Confidential Information!L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsTypes.h"
#include "drvGOP.h"
#include "halGOP.h"
#include "regGOP.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "drvGFLIP.h"
#include "halCHIP.h"
#include "drvGOP_priv.h"

#ifdef XC_FSC_FRC_PATCH
extern void MDrv_SC_Get_GOPOffset(MS_U16 *u16OffsetH, MS_U16 *u16OffsetV);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0)
#endif

 //check 1 time/1ms, maximum vsync time is 1000/48 ms(scaler 48hz output), so check 22 times at most

#ifdef GOP_4K2K30
#define GOP_TIMEOUT_CNT_OS              40UL
#else
#define GOP_TIMEOUT_CNT_OS              22UL
#endif
#define GOP_TIMEOUT_CNT_OS_YIELD  	    0x10000UL
#define GOP_TIMEOUT_CNT_NOS     	    0x100000UL

#define PALETTE_BY_REGDMA               1UL
#define msWarning(c)                    do {} while (0)
#define msFatal(c)                      do {} while (0)
#define ERR_FB_ID_OUT_OF_RANGE          0x0300UL
#define ERR_FB_ID_NOT_ALLOCATED         0x0301UL
#define ERR_FB_ID_ALREADY_ALLOCATED     0x0302UL
#define ERR_FB_OUT_OF_MEMORY            0x0303UL
#define ERR_FB_OUT_OF_ENTRY             0x0304UL
#define PAR_IGNORE                      0xFFUL               // PARAM constant for ignore

//=============================================================
// Debug Log
#include "ULog.h"
MS_U32 u32GOPDbgLevel_drv;

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define GOP_D_INFO(x, args...) if(u32GOPDbgLevel_drv >= E_GOP_Debug_Level_HIGH) {ULOGI("GOP DRV", x, ##args);}
// Warning, illegal paramter but can be self fixed in functions
#define GOP_D_WARN(x, args...) if(u32GOPDbgLevel_drv >= E_GOP_Debug_Level_HIGH) {ULOGW("GOP DRV", x, ##args);}
//  Need debug, illegal paramter.
#define GOP_D_DBUG(x, args...) if(u32GOPDbgLevel_drv >= E_GOP_Debug_Level_MED) {ULOGD("GOP DRV", x, ##args);}
// Error, function will be terminated but system not crash
#define GOP_D_ERR(x, args...) if(u32GOPDbgLevel_drv >= E_GOP_Debug_Level_LOW) {ULOGE("GOP DRV", x, ##args);}
// Critical, system crash. (ex. assert)
#define GOP_D_FATAL(x, args...) if(u32GOPDbgLevel_drv >= E_GOP_Debug_Level_LOW) {ULOGF("GOP DRV", x, ##args);}
//=============================================================

#if GOP_LOCK_SUPPORT
//Move GOP mutex define here for gflip to access
//But we do not suggest to use it in DRV layer in normal case, try use it in API layer
    #define  _ObtainMutex() \
    {\
        if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared == NULL){return FALSE;} \
        if(FALSE == MsOS_ObtainMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutex, MSOS_WAIT_FOREVER)){APIGOP_ASSERT(0, (GOP_D_FATAL("DRV_GOP_ENTRY: Fatal Error, Obtain GOP mutex fail\n")));}  \
        APIGOP_ASSERT(0==pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt, (GOP_D_FATAL("DRV_GOP_ENTRY: Fatal Error, No GOP mutex to release\n")));   \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt++;  \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPLock=MsOS_GetOSThreadID();  \
        if((pGOPDrvLocalCtx->apiCtxLocal.u32GOPClientId!=pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u32LstGOPClientId)  \
            &&pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u32LstGOPClientId)   \
        {   \
            GOP_GOP_Restore_Ctx(&(pGOPDrvLocalCtx->apiCtxLocal));  \
            pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u32LstGOPClientId = pGOPDrvLocalCtx->apiCtxLocal.u32GOPClientId;  \
        }  \
    }

    //Warning: we do not suggest to use it in DRV layer in normal case, try use it in API layer
    #define DRV_GOP_ENTRY()   \
    {     \
        if (pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutex < 0)  \
        {   \
            GOP_D_DBUG("DRV_GOP_ENTRY: Fatal Error, No GOP mutex to obtain\n"); \
        }   \
        if(0 < pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt) \
        {    \
            if (pGOPDrvLocalCtx->apiCtxLocal.s32GOPLock!=MsOS_GetOSThreadID())\
            {_ObtainMutex(); }\
            else\
            {pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt++;}  \
        }    \
        else \
        {_ObtainMutex();} \
     }

    //Warning: we do not suggest to use it in DRV layer in normal case, try use it in API layer
    #define DRV_GOP_RETURN() \
    {   \
        APIGOP_ASSERT((pGOPDrvLocalCtx->apiCtxLocal.s32GOPLock==MsOS_GetOSThreadID()), (GOP_D_FATAL("\nDRV_GOP_RETURN: Fatal Error, Task ID mismatch[%d]->[%d] \n", pGOPDrvLocalCtx->apiCtxLocal.s32GOPLock, (MS_S32)MsOS_GetOSThreadID())));    \
        APIGOP_ASSERT((0<pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt), (GOP_D_FATAL("\nDRV_GOP_RETURN: Fatal Error, No Mutex to release[Cnt=%d]\n", pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt)));    \
        if(1 < pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt)  \
        {pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt--;}  \
        else if(0 < pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt)  \
        {   \
          pGOPDrvLocalCtx->apiCtxLocal.s32GOPLock = -1;  \
          pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutexCnt--;\
          if(FALSE == MsOS_ReleaseMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPMutex)){APIGOP_ASSERT(0, (GOP_D_FATAL("DRV_GOP_ENTRY: Fatal Error, Release GOP mutex fail\n")));}  \
        }   \
        else{GOP_D_DBUG("\nDRV_GOP_RETURN: Fatal Error, No mutex to release\n");}   \
    }
#else
    #define DRV_GOP_ENTRY() while(0)
    #define DRV_GOP_RETURN()  while(0)
#endif

#if GOP_XC_LOCK_SUPPORT
#if GOP_XC_LOCK_DEBUG
///XC MUTEX for GOP driver access SC registers
///Place it in Drv layer NOT the API Layer is to avoid Mutex rase condition, because GOP functions will use some XC call back functions
    #define  _Obtain_XCMutex() \
    {\
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt++;  \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock=MsOS_GetOSThreadID();  \
        GOP_D_INFO("[%s][%05d][%d]XC_LOCK+++[%d]\n",__FUNCTION__,__LINE__, pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock, pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt);    \
        MsOS_ObtainMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Mutex, MSOS_WAIT_FOREVER);  \
    }

    #define  _Release_XCMutex() \
    {   \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt--;\
        GOP_D_INFO("[%s][%05d][%d]XC_LOCK---[%d]\n",__FUNCTION__,__LINE__, pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock, pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt);    \
        MsOS_ReleaseMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Mutex);  \
    }
#else
    #define  _Obtain_XCMutex() \
    {\
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt++;  \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock=MsOS_GetOSThreadID();  \
        MsOS_ObtainMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Mutex, MSOS_WAIT_FOREVER);  \
    }

    #define  _Release_XCMutex() \
    {   \
        pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt--;\
        MsOS_ReleaseMutex(pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Mutex);  \
    }
#endif

#define DRV_GOP_XC_ENTRY()   \
{     \
    if(0 < pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt) \
    {    \
        if (pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock!=MsOS_GetOSThreadID())   \
        {_Obtain_XCMutex(); }   \
        else   \
        {pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt++;}  \
    }    \
    else \
    {_Obtain_XCMutex();} \
}

#define DRV_GOP_XC_RETURN() \
{   \
    if(pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock != MsOS_GetOSThreadID())  \
        {GOP_D_DBUG("DRV_GOP_XC_RETURN: Fatal Error, Task ID mismatch [%d]->[%d]\n", pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_Lock, (MS_S32)MsOS_GetOSThreadID());}    \
    if(0<pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt){    \
    if(1 < pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt)  \
    {pGOPDrvLocalCtx->apiCtxLocal.s32GOPXC_MutexCnt--;}  \
    else   \
    {   \
        _Release_XCMutex();   \
    }   \
    }else{GOP_D_DBUG("DRV_GOP_XC_RETURN: Fatal Error, No mutex to release\n");}   \
}
#else
#define DRV_GOP_XC_ENTRY() while(0)
#define DRV_GOP_XC_RETURN()  while(0)
#endif

#if GOP_XC_SEMPHORE_PROTECT
#define E_XC_POOL_ID_INTERNAL_REGISTER  0
#if GOP_XC_LOCK_DEBUG
#define DRV_GOP_XC_GET_SEMPHORE()   \
{     \
    if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bXCDirrectBankSupport)   \
    {       \
        if(_GOP_Get_XCSemaphore() != UTOPIA_STATUS_SUCCESS)                 \
        {GOP_D_DBUG("DRV_GOP_XC_GET_SEMPHORE: Fatal Error, %s get XC semaphore fail\n", __FUNCTION__);} \
    }   \
}

#define DRV_GOP_XC_RELEASE_SEMPHORE() \
{   \
    if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bXCDirrectBankSupport)   \
    {       \
        if (_GOP_Release_XCSemaphore() != UTOPIA_STATUS_SUCCESS) \
        {GOP_D_DBUG("DRV_GOP_XC_RELEASE_SEMPHORE: Fatal Error, %s release XC semaphore fail\n",__FUNCTION__);}  \
    }   \
}
#else
#define DRV_GOP_XC_GET_SEMPHORE()   \
{     \
    if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bXCDirrectBankSupport)   \
    {       \
        _GOP_Get_XCSemaphore();                         \
    }   \
}

#define DRV_GOP_XC_RELEASE_SEMPHORE() \
{   \
    if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bXCDirrectBankSupport)   \
    {       \
        _GOP_Release_XCSemaphore();     \
    }   \
}
#endif
#else
#define DRV_GOP_XC_GET_SEMPHORE() while(0)
#define DRV_GOP_XC_RELEASE_SEMPHORE()  while(0)
#endif
#define TESTPATTERN_CALC(x) ((x&0xF0)<<4)+(x&0xF)

// Define return values of check align
#define CHECKALIGN_SUCCESS              1UL
#define CHECKALIGN_FORMAT_FAIL          2UL
#define CHECKALIGN_PARA_FAIL            3UL

#define FRAMEPACKING_1080P_GAP (45)
#define FRAMEPACKING_720P_GAP (30)

#define GOP_ASSERT(x) {printf("\33[0;36m   error in %s:%d    \33[m \n",__FUNCTION__,__LINE__);}
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if GOP_XC_SEMPHORE_PROTECT
extern void* pInstantGOP_XC;
extern void* g_pXCResource[];
#endif
extern MS_BOOL (*_fpXCIsInterlace)(void);
extern MS_U16 (*_fpXCGetCapHStart)(void);

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#ifdef INSTANT_PRIVATE
#define g_gopDrvCtxLocal  psGOPInstPri->g_gopDrvCtxLocal
#else
GOP_CTX_DRV_LOCAL g_gopDrvCtxLocal;
#endif

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
#else
GOP_CTX_DRV_SHARED g_gopDrvCtxShared;
#endif

#if GOP_XC_SEMPHORE_PROTECT
static MS_BOOL _XC_Inited = FALSE;
#endif

MS_U16 _GopVStretchTable[MAX_GOP_VSTRETCH_MODE_SUPPORT][GOP_VSTRETCH_TABLE_NUM] =
{
        //liner
        {0x14,0x3c,0x13,0x3d,
         0x11,0x3f,0x10,0x00,
         0x0e,0x02,0x0b,0x05,
         0x09,0x07,0x08,0x08,
         0x14,0x3c,0x13,0x3d,
         0x11,0x3f,0x10,0x00,
         0x0e,0x02,0x0b,0x05,
         0x09,0x07,0x08,0x08,
        },
        //
        {0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
         0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
        },
        //
        {0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
         0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
        },
        //liner Gain0
        {0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
         0x0f,0x01,0x0e,0x02,
         0x0d,0x03,0x0c,0x04,
         0x0b,0x05,0x0a,0x06,
         0x09,0x07,0x08,0x08,
        },
        //liner Gain1
        {0x10,0x00,0x0F,0x01,
         0x0E,0x02,0x0D,0x03,
         0x0C,0x04,0x0B,0x05,
         0x0A,0x06,0x08,0x08,
         0x10,0x00,0x0F,0x01,
         0x0E,0x02,0x0D,0x03,
         0x0C,0x04,0x0B,0x05,
         0x0A,0x06,0x08,0x08,
        },
};

MS_U16 _GopHStretchTable[MAX_GOP_HSTRETCH_MODE_SUPPORT][GOP_STRETCH_TABLE_NUM] =
{

    //6-tap Default
    { 0x03,0x01,0x16,0x01,0x03,
      0x00,0x03,0x02,0x16,0x04,
      0x04,0x01,0x02,0x03,0x13,
      0x07,0x04,0x01,0x02,0x04,
      0x10,0x0b,0x04,0x01,0x03,
      0x01,0x16,0x01,0x03,0x00,
      0x03,0x02,0x16,0x04,0x04,
      0x01,0x02,0x03,0x13,0x07,
      0x04,0x01,0x02,0x04,0x10,
      0x0b,0x04,0x01,0x00,0x01
    },

    //Duplicate ->Set as default
    { 0x03,0x01,0x16,0x01,0x03,
      0x00,0x03,0x02,0x16,0x04,
      0x04,0x01,0x02,0x03,0x13,
      0x07,0x04,0x01,0x02,0x04,
      0x10,0x0b,0x04,0x01,0x03,
      0x01,0x16,0x01,0x03,0x00,
      0x03,0x02,0x16,0x04,0x04,
      0x01,0x02,0x03,0x13,0x07,
      0x04,0x01,0x02,0x04,0x10,
      0x0b,0x04,0x01,0x00,0x01
    },

    //6-tap Linear
    { 0x00,0x00,0x10,0x00,0x00,
      0x00,0x00,0x00,0x0E,0x02,
      0x00,0x00,0x00,0x00,0x0C,
      0x04,0x00,0x00,0x00,0x00,
      0x09,0x07,0x00,0x00,0x00,
      0x00,0x10,0x00,0x00,0x00,
      0x00,0x00,0x0e,0x02,0x00,
      0x00,0x00,0x00,0x0c,0x04,
      0x00,0x00,0x00,0x00,0x09,
      0x07,0x00,0x00,0x00,0x00
    },

    //6-tap Nearest
    { 0x00,0x00,0x10,0x00,0x00,
      0x00,0x00,0x00,0x10,0x00,
      0x00,0x00,0x00,0x00,0x10,
      0x00,0x00,0x00,0x00,0x00,
      0x10,0x00,0x00,0x00,0x00,
      0x00,0x10,0x00,0x00,0x00,
      0x00,0x00,0x10,0x00,0x00,
      0x00,0x00,0x00,0x10,0x00,
      0x00,0x00,0x00,0x00,0x10,
      0x00,0x00,0x00,0x00,0x00
    },

    //6-tap Gain0
    { 0x00,0x01,0x10,0x01,0x00,
      0x00,0x00,0x02,0x0f,0x04,
      0x01,0x00,0x01,0x02,0x0d,
      0x05,0x01,0x00,0x00,0x02,
      0x0c,0x08,0x02,0x00,0x00,
      0x01,0x10,0x01,0x00,0x00,
      0x00,0x02,0x0f,0x04,0x01,
      0x00,0x01,0x02,0x0d,0x05,
      0x01,0x00,0x00,0x02,0x0c,
      0x08,0x02,0x00,0x00,0x00
    },

    //6-tap Gain1
    { 0x02,0x01,0x14,0x01,0x02,
      0x00,0x02,0x02,0x12,0x05,
      0x03,0x00,0x00,0x03,0x10,
      0x06,0x02,0x01,0x01,0x03,
      0x0e,0x0a,0x03,0x01,0x02,
      0x01,0x14,0x01,0x02,0x00,
      0x02,0x02,0x12,0x05,0x03,
      0x00,0x00,0x03,0x10,0x06,
      0x02,0x01,0x01,0x03,0x0e,
      0x0a,0x03,0x01,0x00,0x00
    },

    //6-tap Gain2
    { 0x03,0x01,0x16,0x01,0x03,
      0x00,0x03,0x02,0x16,0x04,
      0x04,0x01,0x02,0x03,0x13,
      0x07,0x04,0x01,0x02,0x04,
      0x10,0x0b,0x04,0x01,0x03,
      0x01,0x16,0x01,0x03,0x00,
      0x03,0x02,0x16,0x04,0x04,
      0x01,0x02,0x03,0x13,0x07,
      0x04,0x01,0x02,0x04,0x10,
      0x0b,0x04,0x01,0x00,0x00
    },

    //6-tap Gain3
    { 0x05,0x01,0x19,0x02,0x05,
      0x00,0x04,0x03,0x18,0x06,
      0x06,0x01,0x03,0x04,0x15,
      0x08,0x04,0x02,0x04,0x04,
      0x13,0x0d,0x06,0x02,0x05,
      0x01,0x19,0x02,0x05,0x00,
      0x04,0x03,0x18,0x06,0x06,
      0x01,0x03,0x04,0x15,0x08,
      0x04,0x02,0x04,0x04,0x13,
      0x0d,0x06,0x02,0x00,0x00
    },

    //6-tap Gain4
    { 0x00,0x01,0x11,0x00,0x00,
      0x00,0x00,0x02,0x0f,0x04,
      0x01,0x00,0x00,0x01,0x0d,
      0x05,0x01,0x00,0x00,0x02,
      0x0c,0x08,0x02,0x00,0x03,
      0x01,0x16,0x01,0x03,0x00,
      0x03,0x02,0x16,0x04,0x04,
      0x01,0x02,0x03,0x13,0x07,
      0x04,0x01,0x02,0x04,0x10,
      0x0b,0x04,0x01,0x00,0x00
    },

    //6-tap Gain5
    { 0x00,0x00,0x10,0x00,0x00,
      0x00,0x00,0x00,0x0E,0x02,
      0x00,0x00,0x00,0x00,0x0C,
      0x04,0x00,0x00,0x00,0x00,
      0x08,0x08,0x00,0x00,0x00,
      0x00,0x10,0x00,0x00,0x00,
      0x00,0x00,0x0E,0x02,0x00,
      0x00,0x00,0x00,0x0C,0x04,
      0x00,0x00,0x00,0x00,0x08,
      0x08,0x00,0x00,0x00,0x00
    },
    //4-tap ->Set as default
    { 0x03,0x01,0x16,0x01,0x03,
      0x00,0x03,0x02,0x16,0x04,
      0x04,0x01,0x02,0x03,0x13,
      0x07,0x04,0x01,0x02,0x04,
      0x10,0x0b,0x04,0x01,0x03,
      0x01,0x16,0x01,0x03,0x00,
      0x03,0x02,0x16,0x04,0x04,
      0x01,0x02,0x03,0x13,0x07,
      0x04,0x01,0x02,0x04,0x10,
      0x0b,0x04,0x01,0x00,0x01
    },
    //2-tap
    { 0x00,0x00,0x10,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,
      0x08,0x08,0x00,0x00,0x00,
      0x00,0x10,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x08,
      0x08,0x00,0x00,0x00,0x00
    },


};
#ifdef GOP_CMDQ_ENABLE
MS_U16 u16MIUSelect[MAX_GOP_SUPPORT]={0};
MS_BOOL bMIUSelect[MAX_GOP_SUPPORT]={0};

MS_U16 u16AFBCMIUSelect[MAX_GOP_SUPPORT]={0};
MS_BOOL bAFBCMIUSelect[MAX_GOP_SUPPORT]={0};
#endif
MS_BOOL bAFBCTrigger=FALSE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
#if GOP_XC_SEMPHORE_PROTECT
MS_U32 _GOP_Get_XCSemaphore(void)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    _XC_Inited = (pInstantGOP_XC != NULL);
    if (!_XC_Inited)
    {
        GOP_D_ERR("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    void* pModule = NULL;
    UtopiaInstanceGetModule(&pInstantGOP_XC, &pModule);
    if(pModule==NULL)
        return UTOPIA_STATUS_ERR_RESOURCE;
    if(UtopiaResourceObtain(pModule, E_XC_POOL_ID_INTERNAL_REGISTER, &g_pXCResource[E_XC_POOL_ID_INTERNAL_REGISTER]) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_D_ERR("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 _GOP_Release_XCSemaphore(void)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    if (!_XC_Inited)
    {
        GOP_D_ERR("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_SUCCESS;
    }
    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    u32Return = UtopiaResourceRelease(g_pXCResource[E_XC_POOL_ID_INTERNAL_REGISTER]);
    return u32Return;
}
#endif

static MS_BOOL _IsGwinIdValid(MS_GOP_CTX_LOCAL*pDrvCtx, MS_U8 u8GwinID)
{
    if (u8GwinID>=pDrvCtx->pGopChipProperty->TotalGwinNum)
        return FALSE;
    else
        return TRUE;
}

static MS_BOOL _IsGopNumVaild(MS_GOP_CTX_LOCAL*pDrvCtx, MS_U8 u8GopNum)
{
    if (u8GopNum>=MDrv_GOP_GetMaxGOPNum(pDrvCtx))
        return FALSE;
    else
        return TRUE;
}

static MS_BOOL _IsMuxSelVaild(MS_GOP_CTX_LOCAL*pDrvCtx, MS_U8 u8GopNum)
{
    if (u8GopNum>=MDrv_GOP_GetMaxMuxSel(pDrvCtx))
        return FALSE;
    else
        return TRUE;
}

static void _GOP_Load_Stretch_Mode_Table(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx,MS_U8 u8GOP, MS_U16* pGopHStretchTable)
{
    MS_U8 idx=0,u8Reg;
    MS_U16 u16RegVal;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    while(idx<GOP_STRETCH_TABLE_NUM)
    {
        u16RegVal = ((pGopHStretchTable[idx] <<8) | pGopHStretchTable[idx+1]);

        if(idx == (GOP_STRETCH_TABLE_NUM-2))
            u8Reg = 0x30;  // reg_clamp_thrd & reg_clamp_num
        else
            u8Reg = idx/2;

        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(2,u8Reg),u16RegVal ,GOP_REG_WORD_MASK);

        idx+=2;
    }

}

static void _GOP_Load_VStretch_Mode_Table(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx,MS_U8 u8GOP, MS_U16* pGopVStretchTable)
{
    MS_U8 idx=0,u8Reg;
    MS_U16 u16RegVal;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    while(idx<GOP_VSTRETCH_TABLE_NUM)
    {
        u8Reg = 0x40 + idx/2;
#ifdef V_STRETCH_TABLE_ISSUE
        if((u8Reg == 0x41)||(u8Reg == 0x43)||(u8Reg == 0x44)||(u8Reg == 0x45)||(u8Reg == 0x46)||(u8Reg == 0x47))
            u16RegVal = ((pGopVStretchTable[idx+1] <<8) | pGopVStretchTable[idx]);
        else
            u16RegVal = ((pGopVStretchTable[idx] <<8) | pGopVStretchTable[idx+1]);
#else
        u16RegVal = ((pGopVStretchTable[idx] <<8) | pGopVStretchTable[idx+1]);
#endif
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(2,u8Reg),u16RegVal ,GOP_REG_WORD_MASK);
        idx+=2;
    }

}


static MS_U16 GOP_SetFmt(DRV_GOPColorType colorfmt)
{
    MS_U16 ret = GOP_FMT_GENERIC;

    switch(colorfmt)
    {
        case E_DRV_GOP_COLOR_RGB555_BLINK :
            ret = GOP_FMT_RGB555_BLINK;
            break;
        case E_DRV_GOP_COLOR_RGB565 :
            ret = GOP_FMT_RGB565;
            break;
        case E_DRV_GOP_COLOR_ARGB4444 :
            ret = GOP_FMT_ARGB4444;
            break;
        case E_DRV_GOP_COLOR_2266:
            ret = GOP_FMT_FaBaFgBg2266;
            break;
        case E_DRV_GOP_COLOR_I8 :
            ret = GOP_FMT_I8;
            break;
        case E_DRV_GOP_COLOR_ARGB8888 :
            ret = GOP_FMT_ARGB8888;
            break;
        case E_DRV_GOP_COLOR_ARGB1555:
            ret = GOP_FMT_ARGB1555;
            break;
        case E_DRV_GOP_COLOR_ABGR8888 :
            ret = GOP_FMT_ABGR8888;
            break;
        case E_DRV_GOP_COLOR_RGB555YUV422:/*Nos Chakra2*/
            ret = GOP_FMT_ARGB1555;
            break;
        case E_DRV_GOP_COLOR_YUV422:
            ret = GOP_FMT_YUV422;
            break;
        case E_DRV_GOP_COLOR_RGBA5551:
            ret = GOP_FMT_RGBA5551;
            break;
        case E_DRV_GOP_COLOR_RGBA4444 :
            ret = GOP_FMT_RGBA4444;
            break;
        case E_DRV_GOP_COLOR_RGBA8888 :
            ret = GOP_FMT_RGBA8888;
            break;
        case E_DRV_GOP_COLOR_BGR565 :
            ret = GOP_FMT_BGR565;
            break;
        case E_DRV_GOP_COLOR_ABGR4444 :
            ret = GOP_FMT_ABGR4444;
            break;
        case E_DRV_GOP_COLOR_AYUV8888 :
            ret = GOP_FMT_ARGB8888;
            break;
        case E_DRV_GOP_COLOR_ABGR1555 :
            ret = GOP_FMT_ABGR1555;
            break;
        case E_DRV_GOP_COLOR_BGRA5551 :
            ret = GOP_FMT_BGRA5551;
            break;
        case E_DRV_GOP_COLOR_BGRA4444 :
            ret = GOP_FMT_BGRA4444;
            break;
        case E_DRV_GOP_COLOR_BGRA8888 :
            ret = GOP_FMT_BGRA8888;
            break;
        default :
            GOP_ASSERT(0);
            ret = GOP_FMT_GENERIC;
            break;
    }
        return ret;
}

static DRV_GOPColorType GOP_GetFmt(MS_U16 colorfmt)
{
    DRV_GOPColorType ret = E_DRV_GOP_COLOR_INVALID;

    switch(colorfmt)
    {
        case GOP_FMT_RGB555_BLINK :
            ret = E_DRV_GOP_COLOR_RGB555_BLINK;
            break;
        case GOP_FMT_RGB565 :
            ret = E_DRV_GOP_COLOR_RGB565;
            break;
        case GOP_FMT_ARGB4444:
            ret = E_DRV_GOP_COLOR_ARGB4444;
            break;
        case GOP_FMT_FaBaFgBg2266:
            ret = E_DRV_GOP_COLOR_2266;
            break;
        case GOP_FMT_I8:
            ret = E_DRV_GOP_COLOR_I8;
            break;
        case GOP_FMT_ARGB8888:
            ret = E_DRV_GOP_COLOR_ARGB8888;
            break;
        case GOP_FMT_ARGB1555:
            ret = E_DRV_GOP_COLOR_ARGB1555;
            break;
        case GOP_FMT_ABGR8888:
            ret = E_DRV_GOP_COLOR_ABGR8888;
            break;
        case GOP_FMT_ARGB1555_DST:
            ret = E_DRV_GOP_COLOR_RGB555YUV422;
            break;
        case GOP_FMT_YUV422:
            ret = E_DRV_GOP_COLOR_YUV422;
            break;
        case GOP_FMT_RGBA5551:
            ret = E_DRV_GOP_COLOR_RGBA5551;
            break;
        case GOP_FMT_RGBA4444:
            ret = E_DRV_GOP_COLOR_RGBA4444;
            break;
        case GOP_FMT_RGBA8888 :
            ret = E_DRV_GOP_COLOR_RGBA8888;
            break;
        case GOP_FMT_BGR565:
            ret = E_DRV_GOP_COLOR_BGR565;
            break;
        case GOP_FMT_ABGR4444:
            ret = E_DRV_GOP_COLOR_ABGR4444;
            break;
        case GOP_FMT_ABGR1555 :
            ret = E_DRV_GOP_COLOR_ABGR1555;
            break;
        case GOP_FMT_BGRA5551 :
            ret = E_DRV_GOP_COLOR_BGRA5551;
            break;
        case GOP_FMT_BGRA4444 :
            ret = E_DRV_GOP_COLOR_BGRA4444;
            break;
        case GOP_FMT_BGRA8888 :
            ret = E_DRV_GOP_COLOR_BGRA8888;
            break;
        default :
            GOP_ASSERT(0);
            ret = E_DRV_GOP_COLOR_INVALID;
            break;
    }
        return ret;
}
static MS_BOOL _GetGOPAckDelayTimeAndCnt(MS_U32* pu32DelayTimems, MS_U32* pu32TimeoutCnt)
{
    if ((NULL == pu32DelayTimems) || (NULL == pu32TimeoutCnt))
    {
        GOP_ASSERT(FALSE);
        return FALSE;
    }
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
	#if GOP_VSYNC_WAIT_BYSLEEP
    *pu32DelayTimems = 1;
    *pu32TimeoutCnt = GOP_TIMEOUT_CNT_OS;
	#else
	*pu32DelayTimems = 1;
    *pu32TimeoutCnt = GOP_TIMEOUT_CNT_OS_YIELD;
    #endif
#else
    *pu32DelayTimems = 0;
    *pu32TimeoutCnt = GOP_TIMEOUT_CNT_NOS;
#endif
    return TRUE;
}

/*
//Return GWIN index in GOP's all GWin by normal gwin id
static MS_U8 _MapGWinIDToGWinIndexinGOP(MS_U8 u8GwinID)
{
    if(u8GwinID < GOP1_GwinIdBase)
    {
        return u8GwinID;//GOP 0
    }
    else if(u8GwinID >= GOP1_GwinIdBase && u8GwinID < GOP2_GwinIdBase)
    {
        return u8GwinID-GOP1_GwinIdBase; //GOP1
    }
    else
    {
        return 0; //GOP2 and gop3 only have 1 gwin
    }
}
*/

#if GOP_LOCK_SUPPORT
static void GOP_GOP_Restore_Ctx(MS_GOP_CTX_LOCAL*pGOPCtx)
{
     MDrv_GOP_Restore_Ctx(pGOPCtx);
}
#endif

/****************************************************************************************/
///Merge
/***************************************************************************************/
#ifdef CONFIG_GOP_TEST_PATTERN
static void Merge(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 x1, MS_U32 x2, MS_U32 x3, MS_U16 v1, MS_U16 v2, MS_U16 v3, MS_U16 m1, MS_U16 m2, MS_U16 m3)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(x1==x2)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x1, v1|v2, m1|m2);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x3, v3, m3);
    }
    else
    {
        if(x2==x3)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x2, v2|v3, m2|m3);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x1, v1, m1);
        }
        else
        {
            if(x3==x1)
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x3, v3|v1, m3|m1);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x2, v2, m2);
            }
            else
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x1, v1, m1);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x2, v2, m2);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, x3, v3, m3);
            }
        }
    }
}
#endif
void GOP_GWIN_TriggerRegWriteIn(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, E_GOP_TYPE u8GopType, MS_BOOL bForceWriteIn, MS_BOOL bSync)
{
    MS_U16 reg_val =0, u16GopAck=0,u16GOPOfst = 0;
    MS_U32 goptimeout = 0,u32BankOffSet = 0;
    MS_U16 IsIpSrcDisable=0;
    MS_BOOL IsBnkExist = FALSE;

    IsBnkExist = _GetBnkOfstByGop(u8GopType,&u32BankOffSet);

    if(!IsBnkExist)
    {
        GOP_D_ERR("[%s][%d]  GOP not support this GOPType = %d\n",__FUNCTION__,__LINE__,u8GopType);
        return;
    }

    if(pGOPDrvLocalCtx->bGOPBankFwr[u8GopType])
    {
     /*Defination use for warning user the bnkForceWrite function support or not.*/
        if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bBnkForceWrite)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BANK_FWR, GOP_BIT0 , GOP_BIT0);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BANK_FWR, 0 , GOP_BIT0);
        }
        else
        {
            GOP_D_DBUG("[%s][%d]  GOP%d not support BankForceWrite\n",__FUNCTION__,__LINE__,u8GopType);
        }
    }
    else if(bForceWriteIn)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, GOP_VAL_FWR, GOP_REG_HW_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);

        if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bAFBC_Merge_GOP_Trig ==FALSE)
        {
            //AFBC
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_TRIGGER, 0, GOP_BIT0);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_TRIGGER, GOP_BIT0, GOP_BIT0);
        }
    }
    else
    {
        u16GOPOfst = (u32BankOffSet>>16);

#ifdef GOP_BANK_SHIFT
        if(u8GopType == E_GOP2)
            u16GOPOfst -= 1;
#endif

        if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bPixelModeSupport)
        {
#ifdef GOP_CMDQ_ENABLE
            if(bMIUSelect[u8GopType] == TRUE || bAFBCMIUSelect[u8GopType] == TRUE || bAFBCTrigger==TRUE )
            {
                HAL_GOP_CMDQ_SetGOPACK(&pGOPDrvLocalCtx->halCtxLocal, u8GopType);
                return;
            }
#endif
            HAL_GOP_SetGOPACK(&pGOPDrvLocalCtx->halCtxLocal, u8GopType);
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, u16GOPOfst | GOP_BIT10 , GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, u16GOPOfst, GOP_REG_WORD_MASK);
        }

#if GOP_SW_SCROLL
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_INT, &reg_val);
        if((reg_val & GOP_BIT0) ==0)
        {
            return;   // gop ack will be process at interrupt routine
        }
#endif
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &reg_val);
        if(u8GopType == E_GOP_Dwin)
        {
            DRV_GOP_XC_GET_SEMPHORE();
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_IP_SYNC, &IsIpSrcDisable);
            DRV_GOP_XC_RELEASE_SEMPHORE();
            if(IsIpSrcDisable & 0x80)
            {
                bSync = FALSE;
            }
        }
        else if (u8GopType == E_GOP_MIXER)
        {
            bSync = TRUE;   //always update double buffer
        }
        else
        {
            if((reg_val & GOP_BIT0) ==1)
            {
                bSync = FALSE;
            }
        }

        if(bSync == TRUE)
        {
            MS_U32 u32DelayTimems = 0;
            MS_U32 u32TimeoutCnt = 0;
            _GetGOPAckDelayTimeAndCnt(&u32DelayTimems, &u32TimeoutCnt);
            do
            {
                goptimeout++;
                u16GopAck = HAL_GOP_GetGOPACK(&pGOPDrvLocalCtx->halCtxLocal,u8GopType);
                if (0 != u32DelayTimems)
                {
					#if GOP_VSYNC_WAIT_BYSLEEP
                    	MsOS_DelayTask(u32DelayTimems); //delay 1 ms
                    #else
						MsOS_YieldTask(); //Customer request
					#endif
                }
            } while((!u16GopAck) &&(goptimeout <= u32TimeoutCnt) );

            // Perform force write if wr timeout.
            if(goptimeout > u32TimeoutCnt)
            {
                GOP_D_INFO("[%s][%d]Perform fwr if wr timeout!!\n",__FUNCTION__,__LINE__);
                if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bBnkForceWrite)
                {
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BANK_FWR, GOP_BIT0 , GOP_BIT0);
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BANK_FWR, 0 , GOP_BIT0);
                }
                else
                {
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, GOP_VAL_FWR, GOP_REG_HW_MASK);
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);
                }
            }

#ifdef CONFIG_MSTAR_SRAMPD
            if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bGOPPowerDown[u8GopType]==TRUE)
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT11, GOP_BIT11);
            }
#endif
        }
    }
}

void GOP_GWIN_UpdateReg(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, E_GOP_TYPE u8GopType)
{
    if((FALSE == pGOPDrvLocalCtx->apiCtxLocal.bUpdateRegOnce[GOP_PUBLIC_UPDATE]) && (FALSE == pGOPDrvLocalCtx->apiCtxLocal.bUpdateRegOnce[u8GopType]))
    {
        GOP_GWIN_TriggerRegWriteIn(pGOPDrvLocalCtx, u8GopType, MDrv_GOP_GWIN_IsForceWrite((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GopType), TRUE);
    }
}

static void GOP_RegdmaEnable(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8GOPNum)
{
    MS_U32 u32BankOffSet=0;
    MS_U16 reg_val =0;
    MS_U32 goptimeout = 0;
    MS_U32 u32DelayTimems = 0;
    MS_U32 u32TimeoutCnt = 0;

    if(pGOPDrvLocalCtx->u16REGMAP_Offset[u8GOPNum] == 0)
        return;

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!", __FUNCTION__,u8GOPNum);
        return;
    }

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);  //sel pal control by u8GOPNum

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_END, \
        pGOPDrvLocalCtx->u16REGMAP_Offset[u8GOPNum]/GOP_WordUnit -1 , 0x01ff);
    pGOPDrvLocalCtx->u16REGMAP_Offset[u8GOPNum]  = 0;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL1, 0x4100, 0xff00);     // Set REGDMA interval
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_EN, 0x0003, 0x0003);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOPNum);

    _GetGOPAckDelayTimeAndCnt(&u32DelayTimems, &u32TimeoutCnt);
    do
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_EN, &reg_val);
        goptimeout++;
        if(reg_val&(0x0100) )
        {
            break;
        }
        if (0 != u32DelayTimems)
        {
            MsOS_DelayTask(u32DelayTimems);
        }
    } while( goptimeout <= u32TimeoutCnt );

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_EN, 0x0000, 0x0003);

    if(goptimeout > u32TimeoutCnt)
    {
        GOP_D_DBUG("GOP wr timeout in [%s]!!\n", __FUNCTION__);
    }
   // GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOPNum);
}


static MS_U32 _GOP_GWIN_AlignChecker(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_U16 *width, MS_U16 fmtbpp)
{
    MS_U16 alignfactor =0;

    if(pGOPCtx->pGOPCtxShared->bPixelMode[u8GOPNum] == TRUE)
    {
        alignfactor = 1;
    }
    else
    {
        alignfactor = (MS_U16)(((MS_U16)MDrv_GOP_GetWordUnit(pGOPCtx, u8GOPNum))/(fmtbpp>>3) );
    }

    if ((alignfactor != 0) && (*width % alignfactor !=0)) // Not Aligned
    {
        GOP_D_ERR("\n\n%s, This FB format needs to %d-pixels alignment !!!\n\n",__FUNCTION__, alignfactor);
        //*width = (*width + alignfactor - 1) & (~(alignfactor - 1));
        return CHECKALIGN_PARA_FAIL;
    }
    else
    {
        return CHECKALIGN_SUCCESS;
    }
}

static MS_U32 _GOP_GWIN_2PEngineAlignChecker(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY *width)
{
    MS_U16 alignfactor =0;

    if(pGOPCtx->pGopChipProperty->b2Pto1PSupport==TRUE)
    {
        alignfactor=2;

        if ((alignfactor != 0) && (*width % alignfactor !=0)) // Not Aligned
        {
            GOP_D_ERR("\n\n%s, Not mach to %d-pixels alignment !!!\n\n",__FUNCTION__, alignfactor);
             return CHECKALIGN_PARA_FAIL;
         }
         else
         {
             return CHECKALIGN_SUCCESS;
         }
     }
    return CHECKALIGN_SUCCESS;
}

static MS_U32 _GOP_GWIN_FB_AddrAlignChecker(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY Address, MS_U16 fmtbpp)
{
    MS_U32 alignfactor=0;

    if(pGOPCtx->pGOPCtxShared->bPixelMode[u8GOPNum] == TRUE)
    {
        alignfactor = (MS_U32)((MDrv_GOP_GetWordUnit(pGOPCtx,u8GOPNum)*(fmtbpp>>3))&0xFF);
    }
    else
    {
        alignfactor = (MS_U32)((MDrv_GOP_GetWordUnit(pGOPCtx,u8GOPNum))&0xFF);
    }

    if ((Address&(alignfactor-1))!=0)
    {
        GOP_D_ERR("%s,%d FB=0x%tx address need %d-bytes aligned!\n",__FUNCTION__,__LINE__, (ptrdiff_t)Address, (MS_U32)alignfactor);
        return CHECKALIGN_PARA_FAIL;
    }
    else
    {
        return CHECKALIGN_SUCCESS;
    }
}


void GOP_SetGopGwinDelPixel(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, MS_U8 u8GOP, MS_U16 del_pixel, E_DRV_GOP_DelDirect direct)
{
#if GOP_PIXEL_ALIGN
    MS_U16 Pixel_Shift;
    if(direct ==E_DRV_GOP_Deldirect_Front)
    {
        Pixel_Shift = 0;
        switch(u8GOP)
        {
            case E_GOP0:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DEL_PIXEL(u8win), del_pixel<<Pixel_Shift, GOP_REG_LW_MASK);
                break;
            case E_GOP1:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DEL_PIXEL(u8win), del_pixel<<Pixel_Shift, GOP_REG_LW_MASK);
                break;
            case E_GOP2:
            case E_GOP3:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_DEL_PIXEL:GOP_1GX_HSTR, del_pixel<<Pixel_Shift, GOP_REG_LW_MASK);    // word pixels
                break;
            case E_GOP4:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_DEL_PIXEL, del_pixel<<Pixel_Shift, GOP_REG_LW_MASK);
                break;
            case E_GOP5:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_DEL_PIXEL, del_pixel<<Pixel_Shift, GOP_REG_LW_MASK);
                break;
            default:
                break;
        }
    }
    else
    {
        Pixel_Shift = GOP_WordUnit;
    switch(u8GOP)
    {
        case E_GOP0:
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DEL_PIXEL(u8win), del_pixel<<Pixel_Shift, GOP_REG_HW_MASK);
            break;
        case E_GOP1:
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DEL_PIXEL(u8win), del_pixel<<Pixel_Shift, GOP_REG_HW_MASK);
            break;
        case E_GOP2:
        case E_GOP3:
             HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_DEL_PIXEL:GOP_1GX_HSTR, del_pixel<<Pixel_Shift, GOP_REG_HW_MASK);    // word pixels
            break;
        case E_GOP4:
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_DEL_PIXEL, del_pixel<<Pixel_Shift, GOP_REG_HW_MASK);
            break;
        case E_GOP5:
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_DEL_PIXEL, del_pixel<<Pixel_Shift, GOP_REG_HW_MASK);
            break;
            default:
            break;
        }
    }


#endif

}

void GOP_GetGopGwinHVPixel(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, MS_U8 u8GOP, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U16 u16tmp=0, u8Bpp=INVALID_BPP;
    u8Bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);

    switch(u8GOP)
    {
        case E_GOP0:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_HSTR(u8win), &(pinfo->u16DispHPixelStart));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_HEND(u8win), &(pinfo->u16DispHPixelEnd));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_VSTR(u8win), &(pinfo->u16DispVPixelStart));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_VEND(u8win), &(pinfo->u16DispVPixelEnd));
            break;

        case E_GOP1:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_HSTR(u8win), &(pinfo->u16DispHPixelStart));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_HEND(u8win), &(pinfo->u16DispHPixelEnd));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_VSTR(u8win), &(pinfo->u16DispVPixelStart));    // 1 pixel
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_VEND(u8win), &(pinfo->u16DispVPixelEnd));    // 1 pixel
            break;

        case E_GOP2:
        case E_GOP3:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_HSTR:GOP_1GX_HSTR, &(pinfo->u16DispHPixelStart));    // word pixels
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_HEND:GOP_1GX_HEND, &(pinfo->u16DispHPixelEnd));    // word pixels
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_VSTR:GOP_1GX_VSTR, &(pinfo->u16DispVPixelStart));    // 1 pixel
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_VEND:GOP_1GX_VEND, &(pinfo->u16DispVPixelEnd));    // 1 pixel
            break;

        case E_GOP4:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_HSTR, &(pinfo->u16DispHPixelStart));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_HEND, &(pinfo->u16DispHPixelEnd));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_VSTR, &(pinfo->u16DispVPixelStart));    // 1 pixel
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_VEND, &(pinfo->u16DispVPixelEnd));    // 1 pixel
            break;

        case E_GOP5:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_HSTR, &(pinfo->u16DispHPixelStart));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_HEND, &(pinfo->u16DispHPixelEnd));
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_VSTR, &(pinfo->u16DispVPixelStart));    // 1 pixel
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_VEND, &(pinfo->u16DispVPixelEnd));    // 1 pixel
            break;

        default:
            GOP_D_ERR("invalid Gwin number:%d\n",u8win);
            break;
    }

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType == E_DRV_3D_DUP_HALF)
    {
        switch(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOP_StereoMode[u8GOP])
        {
            case E_DRV_GOP_3D_SIDE_BY_SYDE:
                u16tmp = (pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart)>>1;
                pinfo->u16DispHPixelEnd -= u16tmp;
                break;
            case E_DRV_GOP_3D_TOP_BOTTOM:
            case E_DRV_GOP_3D_LINE_ALTERNATIVE:
                u16tmp = (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart)>>1;
                pinfo->u16DispVPixelEnd -= u16tmp;
                break;
            case E_DRV_GOP_3D_LINE_FRAMEPACKING:
                u16tmp = (pinfo->u16DispVPixelEnd - pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap - pinfo->u16DispVPixelStart)>>1;
                pinfo->u16DispVPixelEnd -= u16tmp;
                break;
            default:
                break;
        }
    }

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP]!= TRUE)
    {
        pinfo->u16DispHPixelStart *= (GOP_WordUnit/(u8Bpp>>3));
        pinfo->u16DispHPixelEnd *= (GOP_WordUnit/(u8Bpp>>3));
    }
}

void GOP_SetGopGwinHVPixel(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, MS_U8 u8GOP, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U16 u8Bpp=INVALID_BPP;
    MS_U16 align_start,align_end, u16VEnd;
    MS_U32 u32BankOffSet=0;
    MS_U16 u16StretchWidth=0;

    u8Bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(u8Bpp == FB_FMT_AS_DEFAULT)
    {
        GOP_D_DBUG("[%s] [%d]  GOP not support the color format = %d    \n",__FUNCTION__,__LINE__,pinfo->clrType);
    }
    u16VEnd = pinfo->u16DispVPixelEnd;
#if GOP_PIXEL_ALIGN
    MS_U16 del_pixel=0;
    MS_U16 align_offset;
    MS_U16 u16tmp;
    MS_U16 u16w = pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart;
    MS_U16 u16Mask;
    MS_U16 GOP_Unit = 0;

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        GOP_Unit = 1;
    }
    else
    {
        GOP_Unit = (GOP_WordUnit/(u8Bpp>>3));
    }
    u16Mask = ~(GOP_Unit -1);

/*
    if(0)//!pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bHMirror)
    {
        align_end = (pinfo->u16DispHPixelEnd) /GOP_Unit;
        if((pinfo->u16DispHPixelEnd) % GOP_Unit)
            align_end+=1;

        align_offset = align_end * GOP_Unit - pinfo->u16DispHPixelEnd;

        u16tmp = (pinfo->u16DispHPixelEnd + align_offset - u16w );
        align_start = u16tmp /GOP_Unit;

        del_pixel = u16tmp - align_start * GOP_Unit;
    }
    else*/
    {
        align_start = ((pinfo->u16DispHPixelStart)/GOP_Unit);
        align_offset = pinfo->u16DispHPixelStart % GOP_Unit;


        u16tmp = ((pinfo->u16DispHPixelStart - align_offset + u16w ) + (GOP_Unit -1))& u16Mask;
        del_pixel = u16tmp - (pinfo->u16DispHPixelStart - align_offset + u16w );
        align_end = u16tmp /GOP_Unit;
    }

    if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bHMirror)
        GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel, E_DRV_GOP_Deldirect_Front);
    else
        GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel, E_DRV_GOP_Deldirect_Back);


    if((align_end*GOP_Unit) != pinfo->u16RBlkHRblkSize)
    {

        if((align_end*GOP_Unit) > u16w)
        {
            if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bHMirror)
            {
                del_pixel = (align_end*GOP_Unit) - u16w;
                GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel,E_DRV_GOP_Deldirect_Back);
            }
        }
        else
        {
            del_pixel = 0;
            GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel,E_DRV_GOP_Deldirect_Back);
        }
    }

    if(pinfo->u16WinX % GOP_Unit)
    {
        del_pixel = pinfo->u16WinX - ((pinfo->u16WinX / GOP_Unit)*GOP_Unit);
        GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel,E_DRV_GOP_Deldirect_Front);
    }
    else
    {
        del_pixel = 0;
        GOP_SetGopGwinDelPixel(pGOPDrvLocalCtx, u8win, u8GOP ,del_pixel,E_DRV_GOP_Deldirect_Front);
    }

#else
    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        align_start = (pinfo->u16DispHPixelStart);
        align_end   = (pinfo->u16DispHPixelEnd);
    }
    else
    {
        align_start = (pinfo->u16DispHPixelStart) /(GOP_WordUnit/(u8Bpp>>3));
        align_end   = (pinfo->u16DispHPixelEnd) /(GOP_WordUnit/(u8Bpp>>3));
    }
#endif
   _GetBnkOfstByGop(u8GOP, &u32BankOffSet);  //sel pal control by u8GOPNum

   if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType == E_DRV_3D_DUP_HALF)
   {
        switch(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOP_StereoMode[u8GOP])
        {
            case E_DRV_GOP_3D_SIDE_BY_SYDE:
                align_end += (align_end -align_start);
                break;
            case E_DRV_GOP_3D_TOP_BOTTOM:
            case E_DRV_GOP_3D_LINE_ALTERNATIVE:
                u16VEnd += (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart);
                break;
            case E_DRV_GOP_3D_LINE_FRAMEPACKING:
                u16VEnd += (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart)+ pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap;
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN_MIDDLE(u8win), \
                    (pinfo->u16DispVPixelEnd + pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap), GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN_MIDDLE(u8win), GOP_BIT15 , GOP_BIT15);
                break;
            default:
                break;
        }
   }
   else
   {
        if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOP_StereoMode[u8GOP] == E_DRV_GOP_3D_LINE_FRAMEPACKING)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SVM_HEND, pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap , GOP_REG_LW_MASK);
        }
   }

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+ GOP_4G_STRCH_HSZ, &u16StretchWidth);
    u16StretchWidth *= GOP_STRETCH_WIDTH_UNIT;
    if(align_end > u16StretchWidth)
    {
        printf("[GOP]%s,%s,%d GWIN Hend > StretchWidth!!!align_end=%d,u16StretchWidth=%d\n",__FILE__,__FUNCTION__,__LINE__,(int)align_end,(int)u16StretchWidth);
        align_end = u16StretchWidth;
     }

    HAL_GOP_SetGopGwinHVPixel(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, u8win, align_start, align_end, pinfo->u16DispVPixelStart, u16VEnd);
}


static void GOP_SetGopExtendGwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_PHY u64tmp;
    MS_U32 u32DRAM_VSTR_Addr, u32DRAM_RBLK_L_Addr, u32DRAM_RBLK_HSIZE_Addr;
    MS_U16 bpp;
    MS_U16 u16GOP_Unit=0;
    MS_U8 u8GOP;
    MS_U16 u16Pitch=0;
    MS_U8 u8MiuSel=0;
    MS_PHY Width=0;

    if (FALSE== _IsGwinIdValid((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }

    switch(u8win)
    {
        case GOP2_Gwin0Id:
            u8GOP = 2;
            u32DRAM_VSTR_Addr = GOP_1G_DRAM_VSTR_L;
            u32DRAM_RBLK_L_Addr = GOP_1G_DRAM_RBLK_L;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1G_DRAM_RBLK_HSIZE;
            break;
        case GOP3_Gwin0Id:
            u8GOP = 3;
            u32DRAM_VSTR_Addr = GOP_1GX_DRAM_VSTR_L;
            u32DRAM_RBLK_L_Addr = GOP_1GX_DRAM_RBLK_L;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GX_DRAM_RBLK_HSIZE;
            break;
        case GOP4_Gwin0Id:
            u8GOP = 4;
            u32DRAM_VSTR_Addr = GOP_1GS0_DRAM_VSTR_L;
            u32DRAM_RBLK_L_Addr = GOP_1GS0_DRAM_RBLK_L;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GS0_DRAM_RBLK_HSIZE;
            break;
        case GOP5_Gwin0Id:
            u8GOP = 5;
            u32DRAM_VSTR_Addr = GOP_1GS1_DRAM_VSTR_L;
            u32DRAM_RBLK_L_Addr = GOP_1GS1_DRAM_RBLK_L;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GS1_DRAM_RBLK_HSIZE;
            break;
        default:
            GOP_D_ERR("[%s] invalud GwinNum\n",__FUNCTION__);
            return;
    }

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support GOP id:%d in this chip version",__FUNCTION__, u8GOP);
        return;
    }

    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP);
    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        GOP_D_ERR("[%s] invalud color format\n",__FUNCTION__);
        return;
    }

	_GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &(pinfo->u16WinX), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &(pinfo->u16DispHPixelStart), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &(pinfo->u16DispHPixelEnd), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &u16Pitch, bpp);
    _GOP_GWIN_FB_AddrAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, pinfo->u64DRAMRBlkStart, bpp);

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        /*Pixel Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit/(bpp/8);
    }
    else
    {
        /*Word Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit;
    }

    if(pinfo->u16DispHPixelEnd > pinfo->u16DispHPixelStart)
    {
        Width = (MS_PHY)(pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart);
    }
    else
    {
    	if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bGopHasInitialized[u8GOP] == TRUE)
    	{
        	GOP_D_DBUG("[%s]WidthEnd 0x%x >WidthStart 0x%x \n",__FUNCTION__, pinfo->u16DispHPixelEnd, pinfo->u16DispHPixelStart);
    	}
    }
    if (CHECKALIGN_PARA_FAIL ==_GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &Width))
    {
        GOP_D_WARN("[%s][%d] Size not Align!!! Original u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
        pinfo->u16DispHPixelStart = ((pinfo->u16DispHPixelStart+1 )& ~(1));
        pinfo->u16DispHPixelEnd = ((pinfo->u16DispHPixelEnd+1 )& ~(1));
        GOP_D_WARN("[%s][%d] Size not Align!!! Align after u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
    }
    _GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP, &(pinfo->u64DRAMRBlkStart));

    if(pGOPDrvLocalCtx->b32BPPTileMode[u8GOP] && pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bVMirror)
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_VSTR_Addr, (pinfo->u16DispVPixelEnd- pinfo->u16DispVPixelStart-1));
    }
    else
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_VSTR_Addr, 0);  //clear the VSTR value
    }

    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64tmp, pinfo->u64DRAMRBlkStart);
    u64tmp /= u16GOP_Unit;

    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_RBLK_L_Addr, (MS_U32)u64tmp);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_RBLK_HSIZE_Addr, u16Pitch, GOP_REG_WORD_MASK);   // word

    GOP_SetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, u8GOP ,pinfo);

    GOP_D_INFO("GWIN_SetWin(%d): [adr, RBsz, offset](Unit:Byte) = [%td, %d, %d]\n", \
                u8win,\
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,\
                pinfo->u16RBlkHPixSize * pinfo->u16RBlkVPixSize / (u16GOP_Unit/(bpp>>3)),\
                (pinfo->u16WinY * pinfo->u16RBlkHPixSize + pinfo->u16WinX)/(u16GOP_Unit/(bpp>>3))   );
    GOP_D_INFO("\t[Vst, Vend, Hst, Hend, GwinHsz](Unit:Pixel) = [%d, %d, %d, %d, %d]\n",\
                pinfo->u16DispVPixelStart, \
                pinfo->u16DispVPixelEnd, \
                pinfo->u16DispHPixelStart, \
                pinfo->u16DispHPixelEnd,\
                pinfo->u16RBlkHPixSize);

}

static void GOP_SetGop1GwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_PHY u64tmp;
    MS_U16 bpp;
    MS_U16 u16GOP_Unit=0;
    MS_U16 u16Pitch=0;
    MS_U8 u8MiuSel;
    MS_PHY Width=0;

    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1);

    u8win -= MAX_GOP0_GWIN; //Reset win number to 0~3 in GOP1
    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        GOP_D_ERR("[%s] invalud color format\n",__FUNCTION__);
        return;
    }

	_GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &(pinfo->u16WinX), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &(pinfo->u16DispHPixelStart), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &(pinfo->u16DispHPixelEnd), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &u16Pitch, bpp);
    _GOP_GWIN_FB_AddrAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, pinfo->u64DRAMRBlkStart, bpp);

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[E_GOP1] == TRUE)
    {
        /*Pixel Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit/(bpp/8);
    }
    else
    {
        /*Word Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit;
    }

    if(pinfo->u16DispHPixelEnd > pinfo->u16DispHPixelStart)
    {
        Width = (MS_PHY)(pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart);
    }
    else
    {
    	if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bGopHasInitialized[E_GOP1] == TRUE)
    	{
        	GOP_D_DBUG("[%s]WidthEnd 0x%x >WidthStart 0x%x \n",__FUNCTION__, pinfo->u16DispHPixelEnd, pinfo->u16DispHPixelStart);
    	}
    }
    if (CHECKALIGN_PARA_FAIL ==_GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &Width))
    {
        GOP_D_WARN("[%s][%d] Size not Align!!! Original u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
        pinfo->u16DispHPixelStart = ((pinfo->u16DispHPixelStart+1 )& ~(1));
        pinfo->u16DispHPixelEnd = ((pinfo->u16DispHPixelEnd+1 )& ~(1));
        GOP_D_WARN("[%s][%d] Size not Align!!! Align after u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
    }
    _GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1, &(pinfo->u64DRAMRBlkStart));

    if(pGOPDrvLocalCtx->b32BPPTileMode[1] && pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bVMirror)
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_2G_DRAM_VSTR_L(u8win), (pinfo->u16DispVPixelEnd- pinfo->u16DispVPixelStart-1));
    }
    else
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_2G_DRAM_VSTR_L(u8win), 0x0);  //clear the VSTR value
    }
    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64tmp, pinfo->u64DRAMRBlkStart);
    u64tmp /= u16GOP_Unit;
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_2G_DRAM_RBLK_L(u8win), u64tmp);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DRAM_RBLK_HSIZE(u8win), u16Pitch, GOP_REG_WORD_MASK);   // word

    GOP_SetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, E_GOP1 ,pinfo);

    GOP_D_INFO("GWIN_SetWin(%d): [adr, RBsz, offset](Unit: Byte) = [%td, %d, %d]\n", \
                u8win,\
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,\
                pinfo->u16RBlkHPixSize * pinfo->u16RBlkVPixSize / (u16GOP_Unit/(bpp>>3)),\
                (pinfo->u16WinY * pinfo->u16RBlkHPixSize + pinfo->u16WinX)/(u16GOP_Unit/(bpp>>3))  );
    GOP_D_INFO("\t[Vst, Vend, Hst, Hend, GwinHsz] = [%d, %d, %d, %d, %d](Unit:Pixel)\n",\
                pinfo->u16DispVPixelStart, \
                pinfo->u16DispVPixelEnd, \
                pinfo->u16DispHPixelStart, \
                pinfo->u16DispHPixelEnd,\
                pinfo->u16RBlkHPixSize);

}

static void GOP_SetGop0GwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_PHY u64tmp=0;
    MS_U32 u32tmp=0;
    MS_U16 u16tmp =0;
    MS_U16 regval =0;
    MS_U16 bpp;
    MS_U16 u16GOP_Unit=0;
    MS_U16 u16Pitch=0;
    MS_U8 u8MiuSel;
    MS_PHY Width=0;

    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        GOP_D_ERR("[%s] invalud color format\n",__FUNCTION__);
        return;
    }

    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &(pinfo->u16WinX), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &(pinfo->u16DispHPixelStart), bpp);
    _GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &(pinfo->u16DispHPixelEnd), bpp);
	_GOP_GWIN_AlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &(pinfo->u16RBlkHRblkSize), bpp);

    _GOP_GWIN_FB_AddrAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, pinfo->u64DRAMRBlkStart, bpp);

    if(pinfo->u16DispHPixelEnd > pinfo->u16DispHPixelStart)
    {
        Width = (MS_PHY)(pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart);
    }
    else
    {
    	if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bGopHasInitialized[E_GOP0] == TRUE)
    	{
        	GOP_D_DBUG("[%s]WidthEnd 0x%x >WidthStart 0x%x \n",__FUNCTION__, pinfo->u16DispHPixelEnd, pinfo->u16DispHPixelStart);
    	}
    }
    if (CHECKALIGN_PARA_FAIL ==_GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &Width))
    {
        GOP_D_WARN("[%s][%d] Size not Align!!! Original u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
        pinfo->u16DispHPixelStart = ((pinfo->u16DispHPixelStart+1 )& ~(1));
        pinfo->u16DispHPixelEnd = ((pinfo->u16DispHPixelEnd+1 )& ~(1));
        GOP_D_WARN("[%s][%d] Size not Align!!! Align after u16DispHPixelStart =0x%x; u16DispHPixelEnd=0x%x  \n",__FUNCTION__,__LINE__,pinfo->u16DispHPixelStart, pinfo->u16DispHPixelEnd );
    }
    _GOP_GWIN_2PEngineAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, &(pinfo->u64DRAMRBlkStart));

    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0);
    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[E_GOP0] == TRUE)
    {
        /*Pixel Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit/(bpp/8);
        #if ENABLE_GOP0_RBLK_MIRROR
            u16tmp = pinfo->u16WinX;
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HSTR(u8win), u16tmp, GOP_REG_WORD_MASK);
        #endif
    }
    else
    {
        /*Word Unit*/
        u16Pitch = pinfo->u16RBlkHRblkSize/u16GOP_Unit;
        #if ENABLE_GOP0_RBLK_MIRROR
            u16tmp = pinfo->u16WinX*(bpp>>3) / u16GOP_Unit;
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HSTR(u8win), u16tmp, GOP_REG_WORD_MASK);
        #endif
    }

#if ENABLE_GOP0_RBLK_MIRROR
    u32tmp = (MS_U32)pinfo->u16WinY * (MS_U32)pinfo->u16RBlkHRblkSize;     // Convert pixel into bytes
    u32tmp  /= u16GOP_Unit;  // Convert to word.
#else
    u32tmp = ((MS_U32)pinfo->u16WinY * (MS_U32)pinfo->u16RBlkHRblkSize  + (MS_U32)pinfo->u16WinX*(bpp>>3));     // Convert pixel into bytes
    u32tmp  /= u16GOP_Unit;  // Convert to word.
#endif //ENABLE_GOP0_RBLK_MIRROR
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_L(u8win), (MS_U16*)&regval);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_H(u8win), (MS_U16*)&u16tmp);

    if(pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bVMirror && pGOPDrvLocalCtx->b32BPPTileMode[0])
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_4G_DRAM_VSTR_L(u8win), pinfo->u16RBlkVPixSize-1);
    }
#if ENABLE_GOP0_RBLK_MIRROR
    else if (u32tmp != (MS_U32)(u16tmp << 16) + regval)
#else
    else if (pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.bVMirror &&(u32tmp != (MS_U32)(u16tmp << 16) + regval))
#endif
    {
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_4G_DRAM_VSTR_L(u8win), u32tmp);
    }
    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64tmp, pinfo->u64DRAMRBlkStart);
    u64tmp /= u16GOP_Unit;
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_4G_DRAM_RBLK_L(u8win), u64tmp);
    u64tmp = ((MS_U32)pinfo->u16RBlkHRblkSize * (MS_U32)pinfo->u16RBlkVPixSize)/u16GOP_Unit;

    _GOP_GWIN_FB_AddrAlignChecker((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0, u64tmp, bpp);
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_4G_DRAM_RBLK_SIZE_L(u8win), u64tmp);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_HSIZE(u8win), u16Pitch, GOP_REG_WORD_MASK);

    GOP_SetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, E_GOP0 ,pinfo);

    GOP_D_INFO("GWIN_SetWin(%d): [adr, RBsz, offset](Unit:Byte) = [%td, %d, %d]\n", \
                u8win,\
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,\
                pinfo->u16RBlkHPixSize * pinfo->u16RBlkVPixSize / (u16GOP_Unit/(bpp>>3)),\
                (pinfo->u16WinY * pinfo->u16RBlkHPixSize + pinfo->u16WinX)/(u16GOP_Unit/(bpp>>3)) );
    GOP_D_INFO("\t[Vst, Vend, Hst, Hend, GwinHsz](Unit:Pixel) = [%d, %d, %d, %d, %d]\n",\
                pinfo->u16DispVPixelStart, \
                pinfo->u16DispVPixelEnd, \
                pinfo->u16DispHPixelStart, \
                pinfo->u16DispHPixelEnd, \
                pinfo->u16RBlkHPixSize);
}

static void GOP_ReadGopExtendGwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U32 u32tmp =0;
    MS_U32 u32DRAM_RBLK_H_Addr, u32DRAM_RBLK_L_Addr, u32DRAM_RBLK_HSIZE_Addr, u32GWIN0_CTRL_Addr =0;
    MS_U16 bpp, u16tmp =0;
    MS_U8 u8GOP;
    MS_U16 u16GOP_Unit=0;

    if (FALSE== _IsGwinIdValid((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }

    switch(u8win)
    {
        case GOP2_Gwin0Id:
            u8GOP = 2;
            u32GWIN0_CTRL_Addr = GOP_1G_GWIN0_CTRL;
            u32DRAM_RBLK_L_Addr = GOP_1G_DRAM_RBLK_L;
            u32DRAM_RBLK_H_Addr = GOP_1G_DRAM_RBLK_H;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1G_DRAM_RBLK_HSIZE;
            break;
        case GOP3_Gwin0Id:
            u8GOP = 3;
            u32GWIN0_CTRL_Addr = GOP_1GX_GWIN0_CTRL;
            u32DRAM_RBLK_L_Addr = GOP_1GX_DRAM_RBLK_L;
            u32DRAM_RBLK_H_Addr = GOP_1GX_DRAM_RBLK_H;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GX_DRAM_RBLK_HSIZE;
            break;
        case GOP4_Gwin0Id:
            u8GOP = 4;
            u32GWIN0_CTRL_Addr = GOP_1GS0_GWIN0_CTRL;
            u32DRAM_RBLK_L_Addr = GOP_1GS0_DRAM_RBLK_L;
            u32DRAM_RBLK_H_Addr = GOP_1GS0_DRAM_RBLK_H;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GS0_DRAM_RBLK_HSIZE;
            break;
        case GOP5_Gwin0Id:
            u8GOP = 5;
            u32GWIN0_CTRL_Addr = GOP_1GS1_GWIN0_CTRL;
            u32DRAM_RBLK_L_Addr = GOP_1GS1_DRAM_RBLK_L;
            u32DRAM_RBLK_H_Addr = GOP_1GS1_DRAM_RBLK_H;
            u32DRAM_RBLK_HSIZE_Addr = GOP_1GS1_DRAM_RBLK_HSIZE;
            break;
        default:
            GOP_D_ERR("[%s] invalud GwinNum\n",__FUNCTION__);
            return;
    }

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support GOP id:%d in this chip version",__FUNCTION__, u8GOP);
        return;
    }

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32GWIN0_CTRL_Addr, (MS_U16*)&u16tmp);
    pinfo->clrType = (DRV_GOPColorType)GOP_GetFmt((u16tmp>>GOP_REG_COLORTYPE_SHIFT)&GOP_REG_COLORTYPE_MASK);
    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        return;
    }

    GOP_GetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, u8GOP, pinfo);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_RBLK_HSIZE_Addr, (MS_U16*)&u16tmp);
    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOP);
    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        pinfo->u16RBlkHRblkSize = u16tmp*(bpp/8);
    }
    else
    {
        pinfo->u16RBlkHRblkSize = u16tmp*u16GOP_Unit;
    }
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_RBLK_L_Addr, (MS_U16*)&u16tmp);
    u32tmp = u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32DRAM_RBLK_H_Addr, (MS_U16*)&u16tmp);
    u32tmp += (u16tmp << 16);
    pinfo->u64DRAMRBlkStart = u32tmp*u16GOP_Unit;

    GOP_D_INFO("GWIN_GetWin(%d): [adr(B), Hsz, Vsz, Hsdrm, winX, winY ] = [%td, %d, %d, %d, %d, %d]\n",
                u8win,
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,
                pinfo->u16RBlkHPixSize,
                pinfo->u16RBlkVPixSize,
                pinfo->u16RBlkHRblkSize,
                pinfo->u16WinX,
                pinfo->u16WinY);


}

static void GOP_ReadGop1GwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U32 u32tmp =0;
    MS_U16 u16tmp =0;
    MS_U16 bpp;
    MS_U16 u16GOP_Unit=0;

    u8win -= MAX_GOP0_GWIN; //Reset win number to 0~3 in GOP1
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_GWIN_CTRL(u8win), (MS_U16*)&u16tmp);
    pinfo->clrType = (DRV_GOPColorType)GOP_GetFmt((u16tmp>>GOP_REG_COLORTYPE_SHIFT)&GOP_REG_COLORTYPE_MASK);
    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        return;
    }

    GOP_GetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, E_GOP1, pinfo);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DRAM_RBLK_HSIZE(u8win), (MS_U16*)&u16tmp);
    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP1);
    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[E_GOP1] == TRUE)
    {
        pinfo->u16RBlkHRblkSize = u16tmp*(bpp/8);
    }
    else
    {
        pinfo->u16RBlkHRblkSize = u16tmp*u16GOP_Unit;
    }
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DRAM_RBLK_L(u8win), (MS_U16*)&u16tmp);
    u32tmp = u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_DRAM_RBLK_H(u8win), (MS_U16*)&u16tmp);
    u32tmp += (u16tmp << 16);
    pinfo->u64DRAMRBlkStart = u32tmp*u16GOP_Unit;

    GOP_D_INFO("GWIN_GetWin(%d): [adr(B), Hsz, Vsz, Hsdrm, winX, winY ] = [%td, %d, %d, %d, %d, %d]\n",
                u8win,
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,
                pinfo->u16RBlkHPixSize,
                pinfo->u16RBlkVPixSize,
                pinfo->u16RBlkHRblkSize,
                pinfo->u16WinX,
                pinfo->u16WinY);
}

static void GOP_ReadGop0GwinInfo(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U32 u32tmp =0;
    MS_U16 u16tmp =0;
    MS_U16 bpp;
    MS_U16 u16GOP_Unit=0;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win), (MS_U16*)&u16tmp);
    pinfo->clrType = (DRV_GOPColorType)GOP_GetFmt((u16tmp>>GOP_REG_COLORTYPE_SHIFT)&GOP_REG_COLORTYPE_MASK);
    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        return;
    }

    GOP_GetGopGwinHVPixel(pGOPDrvLocalCtx, u8win, E_GOP0, pinfo);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_HSIZE(u8win), (MS_U16*)&u16tmp);
    u16GOP_Unit = MDrv_GOP_GetWordUnit((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, E_GOP0);
    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[E_GOP0] == TRUE)
    {
        pinfo->u16RBlkHRblkSize = u16tmp*(bpp/8);
    }
    else
    {
        pinfo->u16RBlkHRblkSize = u16tmp*u16GOP_Unit;
    }
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_L(u8win), (MS_U16*)&u16tmp);
    u32tmp = u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_H(u8win), (MS_U16*)&u16tmp);
    u32tmp += (u16tmp << 16);
    pinfo->u64DRAMRBlkStart = u32tmp*u16GOP_Unit;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_SIZE_L(u8win), (MS_U16*)&u16tmp);
    u32tmp = u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_SIZE_H(u8win), (MS_U16*)&u16tmp);
    u32tmp += (u16tmp << 16);

    if (pinfo->u16RBlkHRblkSize!=0)
        pinfo->u16RBlkVPixSize = ((u32tmp *u16GOP_Unit) / pinfo->u16RBlkHRblkSize);

    //2009.03.14 Fix the TTX,CC position error
    #if 0
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_L(u8win), (MS_U16*)&u16tmp);
    u32tmp= u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_H(u8win), (MS_U16*)&u16tmp);
    u32tmp += (u16tmp << 16);
    pinfo->u16WinY = (MS_U16)((u32tmp * (64/bpp)) / pinfo->u16RBlkHPixSize);  // pix
    pinfo->u16WinX= (MS_U16)((u32tmp * (64/bpp)) / pinfo->u16RBlkHPixSize);  // pix
    #endif
    GOP_D_INFO("GWIN_GetWin(%d): [adr(B), Hsz, Vsz, Hsdrm ] = [%td, %d, %d, %d]\n",
                u8win,
                (ptrdiff_t)pinfo->u64DRAMRBlkStart,
                pinfo->u16RBlkHPixSize,
                pinfo->u16RBlkVPixSize,
                pinfo->u16RBlkHRblkSize
                    );
}

static MS_U8 _GOP_MAP_DRVHscale2Reg_Emun(DRV_GOPStrchHMode eGOPHscal)
{
    MS_U8 eRet=0xFF;
    switch(eGOPHscal)
    {
        case E_DRV_GOP_HSTRCH_6TAPE:
        default:
            eRet = 0x0;
            break;
        case E_DRV_GOP_HSTRCH_DUPLICATE:
            eRet = 0x1;
            break;
        case E_DRV_GOP_HSTRCH_4TAPE:
            eRet = 0x3;
            break;
    }

    return eRet;
}

static MS_U8 _GOP_MAP_DRVVscale2Reg_Emun(DRV_GOPStrchVMode eGOPVscal)
{
    MS_U8 eRet=0xFF;

    switch(eGOPVscal)
    {
        case E_DRV_GOP_VSTRCH_LINEAR:
        default:
            eRet = 0x0;
            break;
        case E_DRV_GOP_VSTRCH_DUPLICATE:
            eRet = 0x1;
            break;
        case E_DRV_GOP_VSTRCH_NEAREST:
            eRet = 0x2;
            break;
        case E_DRV_GOP_VSTRCH_LINEAR_GAIN2:
        case E_DRV_GOP_VSTRCH_4TAP:
            eRet = 0x3;
            break;
    }

    return eRet;
}

MS_BOOL _GWIN_ADDR_Invalid_Check(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win)
{

    return TRUE;
}

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U8 MDrv_DumpGopByGwinId(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U32 GwinID)
{
    MS_U8 gop_num=0;
    MS_U8 gop_gwinnum[MAX_GOP_SUPPORT];
    MS_U8 gop_start[MAX_GOP_SUPPORT];
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    for (gop_num=0; gop_num < MAX_GOP_SUPPORT; gop_num++)
    {
        gop_start[gop_num]=0;
        gop_gwinnum[gop_num]=0;
    }

    for (gop_num=0; gop_num < MAX_GOP_SUPPORT; gop_num++)
    {
        gop_gwinnum[gop_num] = MDrv_GOP_GetGwinNum((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx,gop_num);
    }

    gop_start[0] = 0;
    for (gop_num=1; gop_num < MAX_GOP_SUPPORT; gop_num++)
    {
        gop_start[gop_num] = gop_start[gop_num-1] + gop_gwinnum[gop_num-1];
    }

    for (gop_num=0; gop_num < MAX_GOP_SUPPORT; gop_num++)
    {
        if( gop_num == 0 )
        {
            if(GwinID < gop_start[gop_num+1])
                return gop_num;
        }
        else if( gop_num ==  (MAX_GOP_SUPPORT-1) )
        {
            if( (GwinID >= gop_start[gop_num]) && (GwinID < pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->TotalGwinNum) )
                return gop_num;
        }
        else
        {
            if( (GwinID >= gop_start[gop_num]) && (GwinID < gop_start[gop_num+1]) )
                return gop_num;
        }
    }

    GOP_ASSERT(0);
    return INVAILD_GOP_NUM;
}

MS_BOOL MDrv_GOP_GetGOPEnum(MS_GOP_CTX_LOCAL*pGOPCtx, GOP_TYPE_DEF* GOP_TYPE)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_GetGOPEnum(&pGOPDrvLocalCtx->halCtxLocal, GOP_TYPE);
    return TRUE;
}

MS_U16 MDrv_GOP_GetGOPACK(MS_GOP_CTX_LOCAL *pstGOPCtx, MS_U8 enGopType)
{
    MS_BOOL bGopAck = FALSE;
    GOP_CTX_DRV_LOCAL*pstGOPDrvLocalCtx = NULL;
#ifdef CONFIG_MSTAR_SRAMPD
    MS_U32 u32BankOffSet=0;
#endif
    if (NULL == pstGOPCtx)
    {
        return bGopAck;
    }

    pstGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pstGOPCtx;
    if (MDrv_GOP_GWIN_IsForceWrite(pstGOPCtx, enGopType))
    {
        bGopAck = TRUE;
    }
    else
    {
        bGopAck = (MS_BOOL)HAL_GOP_GetGOPACK(&pstGOPDrvLocalCtx->halCtxLocal, enGopType);
    }
#ifdef CONFIG_MSTAR_SRAMPD
    if((bGopAck==TRUE)&&(pstGOPCtx->pGOPCtxShared->bGOPPowerDown[enGopType]==TRUE))
    {
        _GetBnkOfstByGop(enGopType, &u32BankOffSet);
        HAL_GOP_Write16Reg(&pstGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT11, GOP_BIT11);
    }
#endif
    return bGopAck;
}

MS_U8 MDrv_GOP_GetMaxGOPNum(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    return (MS_U8)MAX_GOP_SUPPORT;
}

MS_U8 MDrv_GOP_GetMaxMuxSel(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    return (MS_U8)MAX_GOP_MUX_SEL;
}

MS_U8 MDrv_GOP_GetGwinNum(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopNum)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, u8GopNum);
}

MS_U8 MDrv_GetMaxMuxOPNum(void)
{
    return MAX_GOP_MUX_OPNum;
}

MS_BOOL MDrv_GetAFBCCapsSupport(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    MS_U8       i=0;
    MS_BOOL     bAFBCStatus=FALSE;

    for(i=0;i<MDrv_GOP_GetMaxGOPNum(pGOPCtx);i++)
    {
        bAFBCStatus |= pGOPCtx->pGopChipProperty->bAFBC_Support[i];
    }
    return bAFBCStatus;
}

MS_U8 MDrv_GOP_Get(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return pGOPDrvLocalCtx->current_gop;
}

GOP_Result MDrv_GOP_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    pGOPDrvLocalCtx->current_gop = u8GOPNum;
    return HAL_GOP_GOPSel(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum);
}

void MDrv_GOP_MIU_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,E_DRV_GOP_SEL_TYPE miusel)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 u16regval=0;
    MS_U32 u32BankOffSet = 0;

    if(miusel>MAX_GOP_MIUSEL)
    {
        GOP_D_DBUG("ERROR Over gop miu select!\n");
    }

    if(pGOPCtx->pGopChipProperty->bAFBC_Support[u8GOPNum]==TRUE)
    {
        _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);
        if((pGOPCtx->pGopChipProperty->bAFBCMIUSelDoubleBuffer==TRUE)&&(pGOPCtx->pGopChipProperty->bInternalMIUSelect[u8GOPNum]==TRUE))
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_MIU, miusel<<4 , GOP_BIT4|GOP_BIT5);
            HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
            GOP_GWIN_UpdateReg((GOP_CTX_DRV_LOCAL*)pGOPCtx, (E_GOP_TYPE)u8GOPNum);
        }
        else if((pGOPCtx->pGopChipProperty->bAFBCMIUSelDoubleBuffer==FALSE)&&(pGOPCtx->pGopChipProperty->bInternalMIUSelect[u8GOPNum]==TRUE))
        {
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+ GOP_4G_MIU_SEL, &u16regval);
            if(u16regval&GOP_BIT12)
            {
#ifdef GOP_CMDQ_ENABLE
                u16AFBCMIUSelect[u8GOPNum]= miusel;
                bAFBCMIUSelect[u8GOPNum] = TRUE;
#else
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_MIU, miusel<<4 , GOP_BIT4|GOP_BIT5);
#endif
            }
            else
            {
                HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
                GOP_GWIN_UpdateReg((GOP_CTX_DRV_LOCAL*)pGOPCtx, (E_GOP_TYPE)u8GOPNum);
            }
        }
        else if((pGOPCtx->pGopChipProperty->bAFBCMIUSelDoubleBuffer==TRUE)&&(pGOPCtx->pGopChipProperty->bInternalMIUSelect[u8GOPNum]==FALSE))
        {
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+ GOP_4G_MIU_SEL, &u16regval);
            if(u16regval&GOP_BIT12)
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_MIU, miusel<<4 , GOP_BIT4|GOP_BIT5);
                if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bAFBC_Merge_GOP_Trig ==FALSE)
                {
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_TRIGGER, GOP_BIT1, GOP_BIT1);
                }
            }
            else
            {
#ifdef GOP_CMDQ_ENABLE
                u16MIUSelect[u8GOPNum]= miusel;
                bMIUSelect[u8GOPNum] = TRUE;
#else
                HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
#endif
            }
        }
        else
        {
#ifdef GOP_CMDQ_ENABLE
            u16AFBCMIUSelect[u8GOPNum]= miusel;
            bAFBCMIUSelect[u8GOPNum] = TRUE;
            u16MIUSelect[u8GOPNum]= miusel;
            bMIUSelect[u8GOPNum] = TRUE;
#else
            HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_MIU, miusel<<4 , GOP_BIT4|GOP_BIT5);
#endif
        }
    }
    else
    {
        if(pGOPCtx->pGopChipProperty->bInternalMIUSelect[u8GOPNum]==TRUE)
        {
            HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
            GOP_GWIN_UpdateReg((GOP_CTX_DRV_LOCAL*)pGOPCtx, (E_GOP_TYPE)u8GOPNum);
        }
        else
        {
#ifndef GOP_CMDQ_ENABLE
            HAL_GOP_Set_MIU(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, miusel);
#else

#ifndef GOP_MIU_GROUP2
    if(miusel == 2)
        return;
#endif
    u16MIUSelect[u8GOPNum]= miusel;
    bMIUSelect[u8GOPNum] = TRUE;
#endif
       }
   }
}


MS_U8 MDrv_GOP_Get_MIU_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 regmiu =0;

    regmiu=HAL_GOP_GetMIUDst(&pGOPDrvLocalCtx->halCtxLocal,u8GOPNum);

    if(regmiu == 0xFF)
    {
        GOP_D_DBUG("ERROR gop miu client\n");
    }

    return regmiu;
}

MS_BOOL MDrv_GOP_SetIOMapBase(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    MS_VIRT MMIOBaseAdr;
    MS_PHY u32NonPMBankSize;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if( !MDrv_MMIO_GetBASE(&MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_GOP))
    {
        GOP_D_FATAL("Get GOP IOMap failure\n");
        GOP_ASSERT(0);
        return FALSE;
    }
     if(pGOPDrvLocalCtx == NULL)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }
    HAL_GOP_SetIOMapBase(&pGOPDrvLocalCtx->halCtxLocal, MMIOBaseAdr);

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bFRCSupport == TRUE)
    {
        if( !MDrv_MMIO_GetBASE(&MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_FRC))
        {
            GOP_D_FATAL("Get OC IOMap failure\n");
            GOP_ASSERT(0);
            return FALSE;
        }
        HAL_GOP_SetIOFRCMapBase(&pGOPDrvLocalCtx->halCtxLocal, MMIOBaseAdr);
    }

    return TRUE;
}

// for identify GOP reg format
MS_U32 MDrv_GOP_GetRegForm(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8gop, MS_U8 u8gwin)
{
    MS_U32 ret = 0xffff;

    if(u8gop != INVAILD_GOP_NUM)
    {
        switch(u8gop)
        {
            case E_GOP0:
                ret = GOP0_REG_FORM;
                break;
            case E_GOP1:
                ret = GOP1_REG_FORM;
                break;
            case E_GOP2:
                ret = GOP2_REG_FORM;
                break;
            case E_GOP3:
                ret = GOP3_REG_FORM;
                break;
            case E_GOP4:
                ret = GOP4_REG_FORM;
                break;
            case E_GOP5:
                ret = GOP5_REG_FORM;
                break;
            case E_GOP_Dwin:
                ret = GOPD_REG_FORM;
                break;
            default:
                ret = 0xffff;
        }
    }
    else if(u8gwin != INVALID_GWIN_ID)
    {
        switch(MDrv_DumpGopByGwinId(pGOPCtx,u8gwin))
        {
            case E_GOP0:
                ret = GOP0_REG_FORM;
                break;
            case E_GOP1:
                ret = GOP1_REG_FORM;
                break;
            case E_GOP2:
                ret = GOP2_REG_FORM;
                break;
            case E_GOP3:
                ret = GOP3_REG_FORM;
                break;
            case E_GOP4:
                ret = GOP4_REG_FORM;
                break;
            case E_GOP5:
                ret = GOP5_REG_FORM;
                break;
            case E_GOP_Dwin:
                ret = GOPD_REG_FORM;
                break;
            default:
                ret = 0xffff;
        }

    }

    return ret ;

}

void* MDrv_GOP_GetShareMemory(MS_BOOL *pbNeedInitShared)
{
    GOP_CTX_DRV_SHARED *pDrvGOPShared=NULL;
    MS_BOOL bNeedInitShared = FALSE;

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
    MS_U32 u32ShmId;
    MS_VIRT VAddr;
    MS_U32 u32BufSize;
    char SHM_Id[] = "Linux GOP driver";

    if (FALSE == MsOS_SHM_GetId((MS_U8*)SHM_Id, sizeof(GOP_CTX_DRV_SHARED), &u32ShmId, &VAddr, &u32BufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)SHM_Id, sizeof(GOP_CTX_DRV_SHARED), &u32ShmId, &VAddr, &u32BufSize, MSOS_SHM_CREATE))
        {
            GOP_D_ERR("SHM allocation failed!\n");
            return NULL;
        }
            GOP_D_INFO("[%s][%d] This is first initial 0x%08x\n", __FUNCTION__, __LINE__, (MS_U32)VAddr);
            memset( (MS_U8*)VAddr, 0, sizeof(GOP_CTX_DRV_SHARED));
            pDrvGOPShared = (GOP_CTX_DRV_SHARED*)VAddr;
            pDrvGOPShared->apiCtxShared.bInitShared = TRUE;
            bNeedInitShared = TRUE;
    }
    pDrvGOPShared = (GOP_CTX_DRV_SHARED*)VAddr;
#else
    pDrvGOPShared =  &g_gopDrvCtxShared;
    bNeedInitShared = TRUE;
#endif

    *pbNeedInitShared = (bNeedInitShared | pDrvGOPShared->apiCtxShared.bInitShared);
    return (void*)pDrvGOPShared;
}


MS_GOP_CTX_LOCAL*  Drv_GOP_Init_Context(void* pInstance,MS_BOOL *pbNeedInitShared)
{
    GOP_CTX_DRV_SHARED *pDrvGOPShared;
    MS_BOOL bNeedInitShared = FALSE;

#ifdef INSTANT_PRIVATE
   GOP_INSTANT_PRIVATE* psGOPInstPri = NULL;
   UtopiaInstanceGetPrivate(pInstance, (void*)&psGOPInstPri);
#endif

    pDrvGOPShared = (GOP_CTX_DRV_SHARED*)MDrv_GOP_GetShareMemory(&bNeedInitShared);

    *pbNeedInitShared = bNeedInitShared;
    memset(&g_gopDrvCtxLocal, 0, sizeof(g_gopDrvCtxLocal));
#ifdef MSOS_TYPE_LINUX
    g_gopDrvCtxLocal.s32FdGFlip = -1;
#endif
    if(bNeedInitShared)
        memset(pDrvGOPShared, 0, sizeof(GOP_CTX_DRV_SHARED));

    g_gopDrvCtxLocal.pDrvCtxShared = pDrvGOPShared;
    g_gopDrvCtxLocal.apiCtxLocal.pGOPCtxShared = & pDrvGOPShared->apiCtxShared;

    HAL_GOP_Init_Context(&g_gopDrvCtxLocal.halCtxLocal, &pDrvGOPShared->halCtxShared, bNeedInitShared);
    g_gopDrvCtxLocal.apiCtxLocal.pGopChipProperty = g_gopDrvCtxLocal.halCtxLocal.pGopChipPro;

    g_gopDrvCtxLocal.halCtxLocal.User_ConsAlpha_bits = DRV_VALID_8BITS; //const alpha : 8bits
    g_gopDrvCtxLocal.apiCtxLocal.pGopConsAlphaBits = &(g_gopDrvCtxLocal.halCtxLocal.User_ConsAlpha_bits);

    if(bNeedInitShared)
    {
//#ifdef MSOS_TYPE_LINUX
        MS_U32 gId;
        MS_GOP_CTX_LOCAL *pGOPCtxLocal=&g_gopDrvCtxLocal.apiCtxLocal;
        *pbNeedInitShared = bNeedInitShared;

        if(g_gopDrvCtxLocal.apiCtxLocal.pGopChipProperty->TotalGwinNum>=SHARED_GWIN_MAX_COUNT)
        {
            //assert here!!
            GOP_D_DBUG("Error - TotalGwinNum >= SHARED_GWIN_MAX_COUNT!!\n");
        }

        for( gId=0; gId<g_gopDrvCtxLocal.apiCtxLocal.pGopChipProperty->TotalGwinNum; gId++)
        {
            pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId = INVALID_GWIN_ID;
            pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].bIsShared = FALSE;
            pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u16SharedCnt = 0x0;
        }


        for( gId=0; gId<MDrv_GOP_GetMaxGOPNum(pGOPCtxLocal); gId++)
        {
            if(gId>=SHARED_GOP_MAX_COUNT)
            {
                //assert here!!
            }
            pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[gId] = -1;
            pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[gId] = FALSE;
        }
        // chip proprity is shared, init once is ok.
        HAL_GOP_Chip_Proprity_Init(&g_gopDrvCtxLocal.halCtxLocal);
//#endif //MSOS_TYPE_LINUX
    }

       return (MS_GOP_CTX_LOCAL*)&g_gopDrvCtxLocal;

}

void MDrv_GOP_Init(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen, MS_BOOL bEnableVsyncIntFlip)
{
    MS_U32 bankoffset=0;
    MS_U8 u8Idx;
    MS_U8 u8MiuSel;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }
    _phy_to_miu_offset(u8MiuSel, u64GOP_REGDMABASE, u64GOP_REGDMABASE);
    _GetBnkOfstByGop(u8GOPNum, &bankoffset);

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL0, 0x1, GOP_BIT0);                      // GOP rst
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL0, 0x0,(GOP_BIT0|GOP_BIT1|GOP_BIT2));   // clear H/V sync reserve
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL1, 0x4100, GOP_REG_HW_MASK);            // Set REGDMA interval

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_HSTRCH, 0x1000, GOP_REG_WORD_MASK);         // Sanger 070713 For REGDMA Ready issue.
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_VSTRCH, 0x1000, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL0, GOP_BIT14, GOP_BIT14);               // Set mask Hsync when VFDE is low

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_RATE, 0x0098, GOP_REG_WORD_MASK);           // enable blink capability - for ttx usage
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL0, 0x0000, GOP_BIT10);                  // Enable RGB output

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_BW, GOP_FIFO_BURST_ALL, GOP_FIFO_BURST_MASK );  //set GOP DMA Burst length to "32"
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_BW, GOP_FIFO_THRESHOLD, GOP_REG_LW_MASK );  //set DMA FIFO threshold to 3/4 FIFO length

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_OLDADDR, (GOP_BIT1 | GOP_BIT0), (GOP_BIT1 | GOP_BIT0) );
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_OLDADDR, 0, (GOP_BIT1 | GOP_BIT0) );

    /*set clamping value between 2 pixels, it can avoid some cases garbage happen.*/
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, (bankoffset+0x20000)+GOP_4G_STRCH_HSZ, 0, GOP_BIT0);
    DRV_GOP_XC_GET_SEMPHORE();
    HAL_GOP_Init(&pGOPDrvLocalCtx->halCtxLocal,  u8GOPNum);
    DRV_GOP_XC_RELEASE_SEMPHORE();

#ifdef GOP_SUPPORT_SPLIT_MODE
    // this should be improved.....
    pGOPDrvLocalCtx->halCtxLocal.pHALShared->u16GopSplitMode_LRWIDTH[u8GOPNum] = pGOPCtx->pGOPCtxShared->u16PnlWidth[u8GOPNum];
#endif

    if (u8GOPNum==0)
    {
        for (u8Idx=0; u8Idx<MAX_GOP0_GWIN; u8Idx++)
        {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_L(u8Idx),0, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_VSTR_H(u8Idx),0, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HSTR(u8Idx),0, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_L(u8Idx),0, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_H(u8Idx),0, GOP_REG_WORD_MASK);
        }
    }

    if (pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bSetHSyncInverse)
    {
        /*Set Hsync Inverse, only valid in T3*/
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_CTRL0, GOP_BIT2, GOP_BIT2);
    }

    if(pGOPCtx->pGopChipProperty->bPixelModeSupport)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, bankoffset+GOP_4G_BANK_FWR, GOP_BIT7, GOP_BIT7);
    }

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOPNum);
}

void MDrv_GOP_DeInit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    //De-Init GFlip:
    MDrv_GFLIP_DeInit(pGOPCtx, (MS_U32)u8GOPNum);
}

void MDrv_GOP_PerClientInit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen, MS_BOOL bEnableVsyncIntFlip)
{
    //Init GFlip:
#if GFLIP_ENABLE
    MDrv_GFLIP_Init(pGOPCtx, (MS_U32)u8GOPNum, bEnableVsyncIntFlip);
#endif
    //Init RegDMA:
    MDrv_GOP_SetRegDMA(pGOPCtx,u8GOPNum, u64GOP_REGDMABASE, u32GOP_RegdmaLen);

    GOP_GWIN_UpdateReg((GOP_CTX_DRV_LOCAL*)pGOPCtx, (E_GOP_TYPE)u8GOPNum);
}

void MDrv_GOP_SetRegDMA(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8MiuSel;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    //* PA->VA for CPU acdess
    pGOPDrvLocalCtx->pu8REGMAP_Base[u8GOP] = (MS_U8*)MS_PA2KSEG1(u64GOP_REGDMABASE);
    pGOPDrvLocalCtx->u16REGMAP_Offset[u8GOP] = 0;
    pGOPDrvLocalCtx->u32REGMAP_Len[u8GOP] = u32GOP_RegdmaLen;
    //printf("This RegDMA address is invalid for HW, Auto-adjusting...\n");
    _phy_to_miu_offset(u8MiuSel, u64GOP_REGDMABASE, u64GOP_REGDMABASE);

    if(pGOPCtx->pGopChipProperty->bInternalMIUSelect[u8GOP]==TRUE)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MIU_SEL, u8MiuSel<<4, GOP_BIT4|GOP_BIT5 );//GWIN Palette MIU Select
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_END, 0x1ff, 0x01ff);          // Setup REGDMA address
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_END, (u64GOP_REGDMABASE/(512*GOP_WordUnit)&0x7f) << 9, 0xFE00);          // Setup REGDMA address
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_REGDMA_STR, (u64GOP_REGDMABASE/(512*GOP_WordUnit))>>7, 0xffff);          // Setup REGDMA address
}

/********************************************************************************/
/// Set GOP progressive mode
/// @param bEnable \b IN
///   - # TRUE  Progressive (read out the DRAM graphic data by FIELD)
///   - # FALSE Interlaced (not care FIELD)
/// @internal please verify the register document and the code
/********************************************************************************/
void MDrv_GOP_GWIN_EnableProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnable)
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if(bEnable)
        regval = 0x8;
    else
        regval = 0;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, regval, 0x8);
}

/********************************************************************************/
/// Get GOP progressive mode
/// @return
///   - # TRUE  Progressive (read out the DRAM graphic data by FIELD)
///   - # FALSE Interlaced (not care FIELD)
/// @internal please verify the register document and the code
/********************************************************************************/
MS_BOOL MDrv_GOP_GWIN_IsProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &regval);
    if ((regval & 8) == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/********************************************************************************/
/// Set the time when new GWIN settings take effect
/// @param bEnable \b IN
///   - # TRUE the new setting moved from internal register buffer
///            to active registers immediately
///   - # FALSE new settings take effect when next VSYNC is coming
/********************************************************************************/
void MDrv_GOP_GWIN_SetForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    pGOPDrvLocalCtx->gop_gwin_frwr = bEnable;
}

void MDrv_GOP_GWIN_SetBnkForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    pGOPDrvLocalCtx->bGOPBankFwr[u8GOP] = bEnable;
}

void MDrv_GOP_GWIN_ForceWrite_Update(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bBnkForceWrite)
        pGOPDrvLocalCtx->bGOPBankFwr[u8GOP] = bEnable;
    pGOPDrvLocalCtx->gop_gwin_frwr = bEnable;
}

/********************************************************************************/
/// Get the status for GWIN settings take effect
/// @param bEnable \b IN
///   - # TRUE the new setting moved from internal register buffer
///            to active registers immediately
///   - # FALSE new settings take effect when next VSYNC is coming
/********************************************************************************/
MS_BOOL MDrv_GOP_GWIN_IsForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return (MS_BOOL)(pGOPDrvLocalCtx->gop_gwin_frwr || pGOPDrvLocalCtx->bGOPBankFwr[u8GOP]);
}

/********************************************************************************/
/// Set GWIN output color domain
/// @param type \b IN YUV or RGB
/********************************************************************************/
void MDrv_GOP_GWIN_OutputColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, GOP_OupputColor type)
{
    MS_U16 regval = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;
    MS_BOOL bHDREnable=FALSE;

    HAL_GOP_IsHDREnabled(&pGOPDrvLocalCtx->halCtxLocal, &bHDREnable);
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if(bHDREnable == FALSE)
    {
        if(type == DRV_GOPOUT_YUV)
            regval |= 0x400;

        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, regval, 0x400);
    }
    else
    {
        GOP_D_INFO("[%s][%d]HDR ON, only Set GOP RGB Domain==\n",__FUNCTION__,__LINE__);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, 0x000, 0x400);
    }
}

/********************************************************************************/
/// Set transparent color for the GWIN in ARGB8888 format
/// @param clr \b IN transparent color
/// @param mask \b IN mask for trs color
/********************************************************************************/
void MDrv_GOP_GWIN_SetTransClr_8888(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U32 clr, MS_U32 mask)
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    regval = (MS_U16)(clr&0xffff);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_L, regval, GOP_REG_WORD_MASK);

    regval = (MS_U16)((clr>>16&0xff))|(MS_U16)((mask&0xff)<<8) ;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_H, regval, GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}
/********************************************************************************/
/// Set transparent color for the GWIN in YUV format
/// @param u8GOP \b IN GOP number
/// @param clr \b IN transparent color
/// @param mask \b IN mask for trs color
/********************************************************************************/

void MDrv_GOP_GWIN_SetTransClr_YUV(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U32 clr, MS_U32 mask)
{
     MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    regval = (MS_U16)(clr&0xffff);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_TUV_L, regval, GOP_REG_WORD_MASK);

    regval = (MS_U16)((clr>>16&0xff))|(MS_U16)((mask&0xff)<<8) ;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_TUV_H, regval, GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}
/********************************************************************************/
/// Enable or Disable transparent color
/// @param fmt \b IN @copydoc EN_GOP_TRANSCLR_FMT
/// @param bEnable \b IN
///   - # TRUE enable transparent color
///   - # FALSE disable transparent color
/********************************************************************************/
void MDrv_GOP_GWIN_EnableTransClr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, GOP_TransClrFmt fmt, MS_BOOL bEnable)
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &regval);

    if(bEnable)
    {
        if(fmt == DRV_GOPTRANSCLR_FMT3)
            regval |= 0x020;
        else
            regval |= 0x0800;
    }
    else
        regval = 0x0000;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, regval, 0x820);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}

void MDrv_GOP_GWIN_GetGwinPriority(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gopnum, GOP_GwinPriority *GwinPri)
{
    MS_U16 u16GetGwinPri =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (u8Gopnum==0)
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PRI0, &u16GetGwinPri);
        GwinPri->Gop0_GwinPri.u81stGwin = (u16GetGwinPri&0x3);
        GwinPri->Gop0_GwinPri.u82stGwin = (u16GetGwinPri&0x30)>>4;
        GwinPri->Gop0_GwinPri.u83stGwin = (u16GetGwinPri&0x300)>>8;
        GwinPri->Gop0_GwinPri.u84stGwin = (u16GetGwinPri&0x3000)>>12;
    }
    else if (u8Gopnum==1)
    {
        if(4 == MDrv_GOP_GetGwinNum(pGOPCtx,1))
        {
            GOP_D_DBUG("[%s][%d]=====TRACE!!!!!!!!!!!!====\n",__FUNCTION__,__LINE__);
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PRI0, &u16GetGwinPri);
            GwinPri->Gop0_GwinPri.u81stGwin = (u16GetGwinPri&0x3);
            GwinPri->Gop0_GwinPri.u82stGwin = (u16GetGwinPri&0x30)>>4;
            GwinPri->Gop0_GwinPri.u83stGwin = (u16GetGwinPri&0x300)>>8;
            GwinPri->Gop0_GwinPri.u84stGwin = (u16GetGwinPri&0x3000)>>12;
        }
        else
        {
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PRI0, &u16GetGwinPri);
            GwinPri->Gop1_1stGwin = GwinPri->Gop1_1stGwin&0x1;
        }
    }
    else
    {
        //only gop0 and gop1 need to get gwin Priority
    }
}

void MDrv_GOP_GWIN_SetHSPipe(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_U16 u16HSPipe)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    DRV_GOPDstType pGopDst = E_DRV_GOP_DST_INVALID;
    MS_U32 u32BankOffSet = 0;
    MS_U16 u16NonVS_PD_Delay=0;

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }

    HAL_GOP_GetGOPDst(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, &pGopDst);

    if (pGOPCtx->pGopChipProperty->bGOPVscalePipeDelay[u8GOPNum])
    {
        if (pGopDst == E_DRV_GOP_DST_OP_DUAL_RATE)
        {
            u16NonVS_PD_Delay = pGOPCtx->pGopChipProperty->GOP_NonVS_DualOpPD_Offset;
            u16HSPipe+= u16NonVS_PD_Delay;
        }
        else if(pGopDst != E_DRV_GOP_DST_IP0)
        {
            u16NonVS_PD_Delay = (pGOPCtx->pGopChipProperty->b2Pto1PSupport==TRUE)?\
                pGOPCtx->pGopChipProperty->GOP_NonVS_PD_Offset*2:  pGOPCtx->pGopChipProperty->GOP_NonVS_PD_Offset;
            u16HSPipe+= u16NonVS_PD_Delay;
        }
        else
        {
            if(!pGOPCtx->pGopChipProperty->bIgnoreIPHPD)
            {
                u16NonVS_PD_Delay = (pGOPCtx->pGopChipProperty->b2Pto1PSupport==TRUE)?\
                    pGOPCtx->pGopChipProperty->GOP_NonVS_PD_Offset*2:  pGOPCtx->pGopChipProperty->GOP_NonVS_PD_Offset;
                u16HSPipe+= u16NonVS_PD_Delay;
            }
        }
    }

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);
#ifdef XC_FSC_FRC_PATCH
    MS_U16 XC_X_Offset = 0, XC_Y_Offset = 0;
    MDrv_SC_Get_GOPOffset(&XC_X_Offset, &XC_Y_Offset);
    u16HSPipe += XC_X_Offset;
#endif
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HS_PIPE, u16HSPipe, 0x1FFF);

}

void MDrv_GOP_GWIN_GetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8* u8GOPNum, Gop_MuxSel eGopMux)
{

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_GWIN_GetMUX(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, eGopMux);

}

void MDrv_GOP_GWIN_SetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, Gop_MuxSel eGopMux)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (!_IsMuxSelVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }
    HAL_GOP_GWIN_SetMUX(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, eGopMux);

}

void MDrv_GOP_MapLayer2Mux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32Layer, MS_U8 u8GopNum, MS_U32 *pu32Mux)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    DRV_GOPDstType gopDst=E_DRV_GOP_DST_OP0;

    MDrv_GOP_GWIN_GetDstPlane(pGOPCtx, u8GopNum, &gopDst);

    *pu32Mux = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MapLayer2Mux[u32Layer];
}

void MDrv_GOP_GWIN_SetGwinPriority(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopNum, GOP_GwinPriority *GwinPri)
{
    MS_U16 u16Regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GopNum);
        return;
    }
    if (u8GopNum==0)
    {
        u16Regval = (GwinPri->Gop0_GwinPri.u81stGwin&0x3) | ((GwinPri->Gop0_GwinPri.u82stGwin&0x3)<<4) | \
            ((GwinPri->Gop0_GwinPri.u83stGwin&0x3)<<8)   | ((GwinPri->Gop0_GwinPri.u84stGwin&0x3)<<12);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PRI0, u16Regval, GOP_REG_WORD_MASK);
    }
    else if (u8GopNum==1)
    {
        if(4 == MDrv_GOP_GetGwinNum(pGOPCtx,1))
        {
            u16Regval = (GwinPri->Gop0_GwinPri.u81stGwin&0x3) | ((GwinPri->Gop0_GwinPri.u82stGwin&0x3)<<4) | \
            ((GwinPri->Gop0_GwinPri.u83stGwin&0x3)<<8)   | ((GwinPri->Gop0_GwinPri.u84stGwin&0x3)<<12);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PRI0, u16Regval, GOP_REG_WORD_MASK);
        }
        else
        {
            u16Regval = (GwinPri->Gop1_1stGwin) | ((~GwinPri->Gop1_1stGwin &0x1)<<4);  //Begrin from t8
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PRI0, u16Regval, GOP_REG_LW_MASK);
        }
    }
    else
    {
        //note: only gop0 and gop1 need to set gwin priority
    }

}

/******************************************************************************/
/// Enable/Disable multi alpha for one gop
/// @param u32GopIdx \b IN: gop ID
/// @param bEnable \b IN: TRUE/FALSE
/********************************************************************************/
void MDrv_GOP_GWIN_EnableMultiAlpha(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GopIdx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, bEnable<<7, GOP_BIT7);
}

/******************************************************************************/
/// get multi alpha status for one gop
/// @param u32GopIdx \b IN: gop ID
/// @return multi alpha status
/******************************************************************************/
MS_BOOL MDrv_GOP_GWIN_IsMultiAlphaEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GopIdx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 u16Value = 0;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, &u16Value);
    if ((u16Value & GOP_BIT7) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef CONFIG_GOP_GWIN_MISC
/********************************************************************************/
/// Set GWIN Blink
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param u8rate \b IN Blink frame rate
/********************************************************************************/
void MDrv_GOP_GWIN_SetBlink(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8rate)
{
    MS_U16 regval;
    MS_U8 u8GOP;
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if(bEnable)
    {
        regval = 0x8000 | u8rate <<8;
    }
    else
    {
        regval = 0;
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_RATE, regval, 0xff00);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}

#endif
GOP_Result MDrv_GOP_SetGOPHighPri(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    GOP_Result GOPRet;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return GOP_FAIL;
    }
    GOPRet = HAL_GOP_SetGOPHighPri(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum);
    return GOPRet;
}


/********************************************************************************/
/// Set GOP0 and GOP1 scaler setting
/// @param gopNum \b IN  0: GOP0  1:GOP1
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/********************************************************************************/
void MDrv_GOP_SetGOPEnable2SC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (!_IsMuxSelVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return;
    }
    DRV_GOP_XC_GET_SEMPHORE();
    HAL_GOP_SetGOPEnable2SC(&pGOPDrvLocalCtx->halCtxLocal, gopNum, bEnable);
    DRV_GOP_XC_RELEASE_SEMPHORE();
}
#ifdef CONFIG_GOP_GWIN_MISC
void MDrv_GOP_SetGOPEnable2Mode1(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    DRV_GOPDstType enGopDst = E_DRV_GOP_DST_INVALID;
    if (!_IsMuxSelVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return;
    }
    DRV_GOP_XC_GET_SEMPHORE();
    HAL_GOP_SetGOPEnable2Mode1(&pGOPDrvLocalCtx->halCtxLocal, gopNum, bEnable);
    DRV_GOP_XC_RELEASE_SEMPHORE();

    HAL_GOP_GetGOPDst(&pGOPDrvLocalCtx->halCtxLocal, gopNum, &enGopDst);
    if (enGopDst == E_DRV_GOP_DST_OP0)
    {
        MDrv_GOP_GWIN_SetAlphaInverse(pGOPCtx, gopNum, !bEnable);
    }
}

void MDrv_GOP_GetGOPEnable2Mode1(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL *pbEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (!_IsMuxSelVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return;
    }
    DRV_GOP_XC_GET_SEMPHORE();
    HAL_GOP_GetGOPAlphaMode1(&pGOPDrvLocalCtx->halCtxLocal, gopNum, pbEnable);
    DRV_GOP_XC_RELEASE_SEMPHORE();
}
#endif

GOP_Result MDrv_GOP_MIXER_SetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_U8 muxNum, MS_BOOL bEnable)
{
    GOP_Result GOPRet;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    GOPRet = HAL_GOP_MIXER_SetMux(&pGOPDrvLocalCtx->halCtxLocal, gopNum, muxNum, bEnable);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_MIXER);
    return GOPRet;
}

GOP_Result MDrv_GOP_MIXER_SetGOPEnable2Mixer(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable)
{
    GOP_Result GOPRet;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return GOP_FAIL;
    }
    GOPRet = HAL_GOP_MIXER_SetGOPEnable2Mixer(&pGOPDrvLocalCtx->halCtxLocal, gopNum, bEnable);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_MIXER);
    return GOPRet;
}

GOP_Result MDrv_GOP_MIXER_EnableOldBlendMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_MIXER_EnableOldBlendMode(&pGOPDrvLocalCtx->halCtxLocal, gopNum, bEnable);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)gopNum);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_MIXER);
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_Set_VE_MUX(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_U8 bEn)
{
    GOP_Result GOPRet;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (!_IsMuxSelVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return GOP_FAIL;
    }
    GOPRet = HAL_GOP_SetGOPToVE(&pGOPDrvLocalCtx->halCtxLocal, gopNum, bEn);
    return GOPRet;
}

/********************************************************************************/
/// Set GOP destination (OP/IP) setting to scaler
/// @param ipSelGop \b IN \copydoc MS_IPSEL_GOP
/********************************************************************************/
void MDrv_GOP_SetIPSel2SC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_IPSEL_GOP ipSelGop)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    DRV_GOP_XC_GET_SEMPHORE();
    HAL_GOP_SetIPSel2SC(&pGOPDrvLocalCtx->halCtxLocal, ipSelGop);
    DRV_GOP_XC_RELEASE_SEMPHORE();
}

#ifdef CONFIG_GOP_FADE
GOP_Result MDrv_GOP_GWIN_SetFadeInOut(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gId, DRV_GOP_FADE_TYPE eGwinFadeType, MS_BOOL bEnable, MS_U8 u8FadeRate)
{
#define FADE_RATE_MASK 0xF

    GOP_Result ret = GOP_SUCCESS;
    MS_U8 u8GOP, u8Oftgid;
    MS_BOOL bIsFrwrBackup;
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, gId))
    {
        GOP_D_ERR("\ngId invalid in [%s]", __FUNCTION__);
        return GOP_FAIL;
    }

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,gId);
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    bIsFrwrBackup = MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP);
    MDrv_GOP_GWIN_ForceWrite_Update(pGOPCtx, u8GOP, TRUE);

    u8Oftgid = gId;
    if ( (gId==GOP3_GwinIdBase) || (gId==GOP2_GwinIdBase) || (gId==GOP4_GwinIdBase) || (gId==GOP5_GwinIdBase))
    {
        u8Oftgid = 0;
    }
    else if (gId>=GOP1_GwinIdBase && gId<GOP2_GwinIdBase)
    {
        u8Oftgid -= GOP1_GwinIdBase;
    }

    if (bEnable)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            GOP_BIT5, GOP_BIT5);
        if (eGwinFadeType==E_DRV_GOP_GWIN_FADE_IN)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            GOP_BIT6, GOP_BIT6);
        }
        else if (eGwinFadeType==E_DRV_GOP_GWIN_FADE_OUT)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            0, GOP_BIT6);
        }
        else
        {
            GOP_D_DBUG("[%s] unknow gop fade type !\n", __FUNCTION__);
            ret = GOP_FAIL;
        }
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            u8FadeRate&FADE_RATE_MASK, FADE_RATE_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            GOP_BIT4, GOP_BIT4);
        GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,gId));
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            0, GOP_BIT5);
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8Oftgid), \
            0, GOP_REG_LW_MASK);
    }

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,gId));
    MDrv_GOP_GWIN_ForceWrite_Update(pGOPCtx, u8GOP, bIsFrwrBackup);
    return ret;
}
#endif

/********************************************************************************/
/// Set GOP destination clock
/// @param gopNum \b IN 0:GOP0  1:GOP1
/// @param eDstType \b IN \copydoc EN_GOP_DST_TYPE
/********************************************************************************/
GOP_Result MDrv_GOP_SetGOPClk(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, DRV_GOPDstType eDstType)
{
    GOP_Result GopRet;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, gopNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, gopNum);
        return GOP_FAIL;
    }
    GopRet = HAL_GOP_SetGOPClk(&pGOPDrvLocalCtx->halCtxLocal, gopNum, eDstType);
    return GopRet;
}

void MDrv_GOP_SetClkForCapture(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_SRC_SEL enSrcSel)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_SetClkForCapture(&pGOPDrvLocalCtx->halCtxLocal, enSrcSel);
}

//-------------------------------------------------------------------------------------------------
/// Set GOP0 and GOP1 field inverse
//-------------------------------------------------------------------------------------------------
void MDrv_GOP_GWIN_SetFieldInver(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL enable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if (enable)
    {
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, 1<<4, 0x10);
    }
    else
    {
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, 0<<4, 0x10);
    }
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}


/********************************************************************************/
/// Set GOP alpha inverse
/// @param bEnable \b IN
///   - # TRUE enable alpha inverse
///   - # FALSE disable alpha inverse
/********************************************************************************/
void MDrv_GOP_GWIN_SetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8gopNum, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet=0;
    DRV_GOPDstType enGopDst = E_DRV_GOP_DST_INVALID;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8gopNum, &u32BankOffSet);
    HAL_GOP_GetGOPDst(&pGOPDrvLocalCtx->halCtxLocal, u8gopNum, &enGopDst);
    if (enGopDst == E_DRV_GOP_DST_OP0)
    {
        MS_BOOL bPreAlpha = FALSE;

        MDrv_GOP_GetGOPEnable2Mode1(pGOPCtx, u8gopNum, &bPreAlpha);
        if(bPreAlpha)
        {
            if(bEnable)
            {
                GOP_D_WARN("GOP%d AlphaInverse will be set to false dueto new alpha mode,please ignore this message. %s\n",u8gopNum,__FUNCTION__);
            }
            bEnable = FALSE;
        }
    }

    if (bEnable)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_CTRL0, (MS_U16)GOP_BIT15, GOP_BIT15);
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_CTRL0, (MS_U16)~GOP_BIT15, GOP_BIT15);
}

/********************************************************************************/
/// Set GOP VSync inverse
/// @param bEnable \b IN
///   - # TRUE enable VSync inverse
///   - # FALSE disable VSync inverse
/********************************************************************************/
void MDrv_GOP_GWIN_SetVSyncinverse(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if (bEnable)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, (MS_U16)GOP_BIT1, GOP_BIT1);
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, (MS_U16)~GOP_BIT1, GOP_BIT1);
}


/********************************************************************************/
/// Get GOP alpha inverse
///   - # TRUE alpha inverse is on
///   - # FALSE alpha inverse is off
/********************************************************************************/
void MDrv_GOP_GWIN_GetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL *bBool)
{
    MS_U16 regval=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    switch(u8GOP)
    {
        case E_GOP0:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_CTRL0, &regval);
            break;
        case E_GOP1:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_CTRL0, &regval);
            break;
        case E_GOP2:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1G_CTRL0, &regval);
            break;
        case E_GOP3:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GX_CTRL0, &regval);
            break;
        case E_GOP4:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_CTRL0, &regval);
            break;
        case E_GOP5:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_CTRL0, &regval);
            break;
        default:
            //??
            break;
    }
    if (regval&0x8000)
        *bBool = TRUE;
    else
        *bBool = FALSE;

}

//-------------------------------------------------------------------------------------------------
/// Set GOP Destination DisplayPlane
/// @param u8GOP_num \b IN: GOP number: 0 or 1
/// @param eDstType \b IN: GOP Destination DisplayPlane Type
/// @return TRUE: sucess / FALSE: fail
//-------------------------------------------------------------------------------------------------
GOP_Result MDrv_GOP_GWIN_SetDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType eDstType,MS_BOOL bOnlyCheck)
{
    GOP_Result ret;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return GOP_FAIL;
    }
    ret = HAL_GOP_GWIN_SetDstPlane(&pGOPDrvLocalCtx->halCtxLocal,u8GOPNum, eDstType,bOnlyCheck);
    if(bOnlyCheck == FALSE)
    {
        //Adjust field by dst
        HAL_GOP_AdjustField(&pGOPDrvLocalCtx->halCtxLocal,u8GOPNum,eDstType);

        GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOPNum);
    }
    return ret;
}

GOP_Result MDrv_GOP_GWIN_GetDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType *pGopDst)
{
    GOP_Result ret;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return GOP_FAIL;
    }
    ret = HAL_GOP_GetGOPDst(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, pGopDst);
    return ret;
}


GOP_Result MDrv_GOP_GWIN_SetMixerDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType eDstType)
{
    GOP_Result ret;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return GOP_FAIL;
    }

    ret = HAL_GOP_SetMixerDst(&pGOPDrvLocalCtx->halCtxLocal,eDstType);
    return ret;
}


GOP_Result MDrv_GOP_GWIN_GetMixerDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType *pGopDst)
{
    GOP_Result ret;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return GOP_FAIL;
    }
    ret = HAL_GOP_GetMixerDst(&pGOPDrvLocalCtx->halCtxLocal,pGopDst);
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Set GOP 3D Middle, for 3D SBS/TPB etc
/// @param u8GOP \b IN: GOP number: 0,1,2,3
/// @return TRUE: sucess / FALSE: fail
//-------------------------------------------------------------------------------------------------
void MDrv_GOP_Set3DOSD_Middle(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    MS_U16 u16Middle=0, u16Ratio=0x1000;
	MS_U32 u32BankOffset = 0;
    GOP_CTX_DRV_LOCAL *pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

	_GetBnkOfstByGop(u8GOP, &u32BankOffset);

    switch(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType)
    {
        case E_DRV_3D_DUP_FULL:
        {
            switch(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->GOP_StereoMode[u8GOP])
            {
                case E_DRV_GOP_3D_SIDE_BY_SYDE:
                    //Agate: middle = PNL Size/Scaling Ratio/2
                    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffset+GOP_4G_HSTRCH, &u16Ratio);
                    u16Middle = ((pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP]*u16Ratio)/SCALING_MULITPLIER + 1)>>1;
                    HAL_GOP_3D_SetMiddle(&pGOPDrvLocalCtx->halCtxLocal,u8GOP,u16Middle);
                    break;
                case E_DRV_GOP_3D_TOP_BOTTOM:
                case E_DRV_GOP_3D_LINE_ALTERNATIVE:
                    //middle = PNL Size/Scaling Ratio/2
                    if (pGOPCtx->pGopChipProperty->bGOPWithVscale[u8GOP])
                    {
                        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffset+GOP_4G_VSTRCH, &u16Ratio);
                        u16Middle = ((pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]*u16Ratio)/SCALING_MULITPLIER + 1)>>1;
                    }
                    else
                    {
                        MS_U16 u16Vsize = 0;
                        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffset+GOP_4G_STRCH_VSZ,&u16Vsize);
                        if (pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16VScaleDst[u8GOP] != 0)
                        {
                            u16Middle = (pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]*u16Vsize/pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16VScaleDst[u8GOP] + 1)>>1;
                        }
                        else
                        {
                            u16Middle = (pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP])>>1;
                        }
                    }
                    HAL_GOP_3D_SetMiddle(&pGOPDrvLocalCtx->halCtxLocal,u8GOP,u16Middle);
                    break;
                case E_DRV_GOP_3D_LINE_FRAMEPACKING:
                    if (FRAMEPACKING_720P_GAP == pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap)
                    {
                        u16Middle = 720;
                    }
                    else if (FRAMEPACKING_1080P_GAP == pGOPDrvLocalCtx->apiCtxLocal.GOP_FramePacking_Gap)
                    {
                        u16Middle = 1080;
                    }
                    HAL_GOP_3D_SetMiddle(&pGOPDrvLocalCtx->halCtxLocal,u8GOP,u16Middle);
                    break;
                default:
                    break;
            }
            break;
        }

        case E_DRV_3D_DUP_HALF:
        {
            switch(pGOPCtx->pGOPCtxShared->GOP_StereoMode[u8GOP])
            {
                case E_DRV_GOP_3D_SIDE_BY_SYDE:
                    u16Middle = pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP]/2;
                    HAL_GOP_3D_SetMiddle(&pGOPDrvLocalCtx->halCtxLocal,u8GOP,u16Middle);
                    break;
                case E_DRV_GOP_3D_TOP_BOTTOM:
                case E_DRV_GOP_3D_LINE_ALTERNATIVE:
                case E_DRV_GOP_3D_LINE_FRAMEPACKING:
                    u16Middle = pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]/2;
                    HAL_GOP_3D_SetMiddle(&pGOPDrvLocalCtx->halCtxLocal,u8GOP,u16Middle);
                    break;
                default:
                    break;
            }
            break;
        }

        default:
            break;
    }
    pGOPDrvLocalCtx->apiCtxLocal.u16GOPGWinMiddle[u8GOP] = u16Middle;
}

/********************************************************************************/
/// Set stretch window property
/// @param u8GOP_num \b IN 0: GOP0  1:GOP1
/// @param eDstType \b IN \copydoc EN_GOP_DST_TYPE
/// @param x \b IN stretch window horizontal start position
/// @param y \b IN stretch window vertical start position
/// @param width \b IN stretch window width
/// @param height \b IN stretch window height
/********************************************************************************/
void MDrv_GOP_GWIN_SetStretchWin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP,MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height)
{
    MS_U32 u32BankOffSet=0;
    MS_U16 u16HStretchRatio = SCALING_MULITPLIER;
    MS_U16 u16VStretchRatio = SCALING_MULITPLIER;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
#ifdef GOP_SUPPORT_SPLIT_MODE
    DRV_GOPDstType GopDst;
#endif

    if ( (x > pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP])
      || (y > pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]))
    {
        APIGOP_ASSERT(FALSE, GOP_D_FATAL("[%s]invalid parameters:x= %u,y= %u,PNL width= %u,PNL height= %u!'\n",__FUNCTION__,
            x, y, pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP], pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]));
    }
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSTR, x, GOP_REG_WORD_MASK);
#ifdef XC_FSC_FRC_PATCH
    MS_U16 XC_X_Offset = 0, XC_Y_Offset = 0;
    MDrv_SC_Get_GOPOffset(&XC_X_Offset, &XC_Y_Offset);
    y += XC_Y_Offset;
#endif
#ifdef GOP_SUPPORT_SPLIT_MODE
    HAL_GOP_GetGOPDst(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, &GopDst);
    if ((pGOPCtx->pGopChipProperty->bGOPNeedSplitMode4DualRate[u8GOP] == TRUE) &&
        (GopDst == E_DRV_GOP_DST_OP_DUAL_RATE)
        )
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSZ, GOP_SPLIT_SUPPORT_MAXW/GOP_STRETCH_WIDTH_UNIT, GOP_REG_WORD_MASK);
    }
    else
#endif
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSZ, (width/GOP_STRETCH_WIDTH_UNIT) , GOP_REG_WORD_MASK);
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_VSTR, y , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_VSZ, height , GOP_REG_WORD_MASK);

	//H size change need to check reg_valid_pix_num_q (0x0c:GOP_4G_SVM_VSTR) value
	//GOP_4G_SVM_VSTR must be the size after scaling for gop_rdy

	HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HSTRCH, &u16HStretchRatio);
	if(u16HStretchRatio == SCALING_MULITPLIER)
	{
#ifndef OUTPUT_VAILD_SIZE_PATCH
		HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, (x+width), GOP_REG_WORD_MASK);
#else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#endif
	}
	else
	{
#ifndef OUTPUT_VAILD_SIZE_PATCH
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  u32BankOffSet + GOP_4G_SVM_VSTR, (x+pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16HScaleDst[u8GOP]), GOP_REG_WORD_MASK);
#else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#endif
	}

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_VSTRCH, &u16VStretchRatio);
    if(u16VStretchRatio == SCALING_MULITPLIER)
	{
#ifndef OUTPUT_VAILD_SIZE_PATCH
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, y+height, GOP_REG_WORD_MASK);
#else
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#endif
	}
    else
    {
#ifndef OUTPUT_VAILD_SIZE_PATCH
		HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, (y+pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16VScaleDst[u8GOP]), GOP_REG_WORD_MASK);
#else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#endif
    }
    //GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);

    if (E_DRV_GOP_DST_OP_DUAL_RATE == pGOPDrvLocalCtx->halCtxLocal.pHALShared->GOP_Dst[u8GOP])
    {
        Hal_SetCropWindow(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, EN_GOP_CROP_ENABLE);
    }
    else
    {
        Hal_SetCropWindow(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, EN_GOP_CROP_DISABLE);
    }
}

/********************************************************************************/
/// Set stretch window property
/// @param u8GOP_num \b IN gop id
/// @param eDstType \b IN \copydoc EN_GOP_DST_TYPE
/// @param x \b IN stretch window horizontal start position
/// @param y \b IN stretch window vertical start position
/********************************************************************************/
void MDrv_GOP_GWIN_SetStretchWinPosition(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP,MS_U16 u16x, MS_U16 u16y)
{
    MS_U32 u32BankOffSet=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if ( (u16x > pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP])
      || (u16y > pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]))
    {
        APIGOP_ASSERT(FALSE, GOP_D_FATAL("[%s]invalid parameters:x= %u,y= %u,PNL width= %u,PNL height= %u!'\n",__FUNCTION__,
            u16x, u16y, pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlWidth[u8GOP], pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->u16PnlHeight[u8GOP]));
    }
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSTR, u16x, GOP_REG_WORD_MASK);
#ifdef XC_FSC_FRC_PATCH
    MS_U16 XC_X_Offset = 0, XC_Y_Offset = 0;
    MDrv_SC_Get_GOPOffset(&XC_X_Offset, &XC_Y_Offset);
    u16y += XC_Y_Offset;
#endif
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_VSTR, u16y , GOP_REG_WORD_MASK);
}

void MDrv_GOP_GWIN_SetHTotal(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 width)
{
    MS_U32 u32BankOffSet=0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_RDMA_HT, (width+3)/2, 0x07ff);
}

//Internal use only, get the real HW register setting on stretch win
//For API usage, please use g_pGOPCtxLocal->pGOPCtxShared->u16StretchWinWidth
void MDrv_GOP_GWIN_Get_StretchWin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP,MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height)
{
    MS_U16 u16RegWidth=0;
    MS_U32 u32BankOffSet=0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSTR, x);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_VSTR, y);
#ifdef XC_FSC_FRC_PATCH
    MS_U16 XC_X_Offset = 0, XC_Y_Offset = 0;
    MDrv_SC_Get_GOPOffset(&XC_X_Offset, &XC_Y_Offset);
    *y -= XC_Y_Offset;
#endif
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSZ, &u16RegWidth);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_VSZ, height);
    *width = u16RegWidth * GOP_STRETCH_WIDTH_UNIT;
}

/********************************************************************************/
/// Get stretch window H/V scale ratio
/// @param u8GOP_num \b IN gop id
/// @param HScaleRat \b IN h ratio value
/// @param VScaleRat \b IN v ratio value
/********************************************************************************/
MS_BOOL MDrv_GOP_GWIN_Get_HVScaleRatio(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP_num, MS_U16 *pu16HScaleRat, MS_U16 *pu16VScaleRat)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP_num, &u32BankOffSet);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH, pu16HScaleRat);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH, pu16VScaleRat);
    return TRUE;
}

/********************************************************************************/
/// Set stretch window H-Stretch
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN original stretch size
/// @param dst \b IN stretch out size
/********************************************************************************/
void MDrv_GOP_GWIN_Set_HSCALE(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst )
{
    MS_U32 hratio;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;
    MS_U32 u32RegVal = 0;
    MS_U16 u16RegVal = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    if(dst > src)
    {
        if (bEnable)
        {
            hratio = (MS_U32)(src) * SCALING_MULITPLIER;

            if(hratio %(MS_U32)dst !=0)
          	{
          		if(pGOPCtx->pGopChipProperty->bPixelModeSupport)
					hratio = (hratio/(MS_U32)dst );
				else
                	hratio = (hratio/(MS_U32)dst )+1;
          	}
            else
                hratio /= (MS_U32)dst;
        }
        else
            hratio = SCALING_MULITPLIER;

	    //H size change need to check reg_valid_pix_num_q (0x0c:GOP_4G_SVM_VSTR) value
	    //GOP_4G_SVM_VSTR must be the size after scaling for gop_rdy
	    if(hratio == SCALING_MULITPLIER)
	    {
		    MS_U16 u16StretchWidth = 1920;
            MS_U16 u16StretchX = 0;
    	    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSZ, &u16StretchWidth);
		    u16StretchWidth *= 2;
    	    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSTR, &u16StretchX);
    	    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, (u16StretchWidth + u16StretchX), GOP_REG_WORD_MASK);
	    }
	    else
	    {
            MS_U16 u16StretchX = 0;
    	    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_STRCH_HSTR, &u16StretchX);
		    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, (u16StretchX + dst), GOP_REG_WORD_MASK);
	    }

		HAL_GOP_HScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,FALSE, src,dst);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH, hratio , GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH_INI, 0 , GOP_REG_WORD_MASK);
    }
    else if(dst == src)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH, SCALING_MULITPLIER , GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH_INI, 0 , GOP_REG_WORD_MASK);

        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_VRATIO_L, &u16RegVal);
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_VRATIO_H, (MS_U16*)&u32RegVal);
        if( ((u32RegVal<<16)+u16RegVal)== 0)
        {
            HAL_GOP_HScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,FALSE, src,dst);
        }
        else
        {
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_HRATIO_L, 0);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, 0 , GOP_BIT0);
        }
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH, SCALING_MULITPLIER , GOP_REG_WORD_MASK);
        HAL_GOP_HScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,bEnable, src,dst);
    }

#ifdef OUTPUT_VAILD_SIZE_PATCH
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#else
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VSTR, dst, GOP_REG_WORD_MASK);
#endif
}

/********************************************************************************/
/// Set stretch window V-Stretch
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN original stretch size
/// @param dst \b IN stretch out size
/********************************************************************************/
void MDrv_GOP_GWIN_Set_VSCALE(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst )
{
    MS_U32 ratio = 0;
    MS_U16  u16x = 0,u16y = 0,u16w = 0,u16h = 0;
    MS_U16 u16RegVal = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;
    MS_U32 u32RegVal = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    if(dst > src)
    {
        if (bEnable)
        {
            if(!pGOPCtx->pGopChipProperty->bGOPWithVscale[u8GOP])
            {
                if((src<1)||(dst<1))
                {
                    GOP_D_ERR("%s,Invalid Vscale size,u8GOP%d, src=0x%x, dst=0x%x\n",__FUNCTION__,u8GOP,src,dst);
                    return;
                }
                else
                {
                    src=src-1;
                    dst=dst-1;
                }
            }
            ratio = (MS_U32)(src) * SCALING_MULITPLIER;

            if(ratio %(MS_U32)dst !=0)
          	{
          	    if(pGOPCtx->pGopChipProperty->bPixelModeSupport)
					ratio = (ratio/(MS_U32)dst );
				else
                	ratio = (ratio/(MS_U32)dst )+1;
            }
            else
                ratio /= (MS_U32)dst;
        }
        else
            ratio = SCALING_MULITPLIER;


        MDrv_GOP_GWIN_Get_StretchWin(pGOPCtx, u8GOP,&u16x,&u16y,&u16w,&u16h);

        if((u16w > 1376) && (pGOPCtx->pGOPCtxShared->GOP_StereoMode[u8GOP] == E_DRV_GOP_3D_SIDE_BY_SYDE) && (ratio != SCALING_MULITPLIER))
        {
            //enable v duplicate mode on 3D side by side.
            u16RegVal = GOP_BIT10;
        }
        else
        {
            u16RegVal = 0;
        }

		HAL_GOP_VScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,FALSE, src,dst);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, u16RegVal, GOP_BIT10);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH, ratio , GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH_INI, 0 , GOP_REG_WORD_MASK);
    }
    else if(dst == src)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH, SCALING_MULITPLIER, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH_INI, 0 , GOP_REG_WORD_MASK);

        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_HRATIO_L, &u16RegVal);
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_HRATIO_H, (MS_U16*)&u32RegVal);
        if( ((u32RegVal<<16)+u16RegVal)== 0)
        {
            HAL_GOP_VScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,FALSE, src,dst);
        }
        else
        {
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_VRATIO_L, 0);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, 0 , GOP_BIT4);
        }
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH, SCALING_MULITPLIER , GOP_REG_WORD_MASK);
        HAL_GOP_VScalingDown(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,bEnable, src,dst);
    }

#ifdef OUTPUT_VAILD_SIZE_PATCH
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
#else
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_SVM_VEND, dst, GOP_REG_WORD_MASK);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Set GOP H-Stretch Mode
/// @param HStrchMode \b IN \copydoc EN_GOP_STRETCH_HMODE
//-------------------------------------------------------------------------------------------------
void MDrv_GOP_GWIN_Set_HStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchHMode HStrchMode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HVSTRCHMD, _GOP_MAP_DRVHscale2Reg_Emun(HStrchMode), GOP_BIT0|GOP_BIT1);
}


//-------------------------------------------------------------------------------------------------
/// Set GOP V-Stretch Mode
/// @param VStrchMode \b IN \copydoc EN_GOP_STRETCH_VMODE
//-------------------------------------------------------------------------------------------------
void MDrv_GOP_GWIN_Set_VStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchVMode VStrchMode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HVSTRCHMD, _GOP_MAP_DRVVscale2Reg_Emun(VStrchMode)<<2, GOP_BIT2|GOP_BIT3);
    #ifdef SUPPORT_V4TAP_MODE
        if(VStrchMode == E_DRV_GOP_VSTRCH_4TAP)
        {
            if(u8GOP == 0 || u8GOP == 2)
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HVSTRCHMD, GOP_BIT6, GOP_BIT6);
            else
                GOP_D_INFO("\nGOP_%d Not Support 4-Tap V stretch mode!'\n",u8GOP);
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HVSTRCHMD, 0, GOP_BIT6);
        }
    #endif
}

//-------------------------------------------------------------------------------------------------
/// Set GOP Transparent color Stretch Mode
/// @param HStrchMode \b IN \copydoc DRV_GOPSTRCH_TRANSPCOLORMODE
//-------------------------------------------------------------------------------------------------
GOP_Result MDrv_GOP_GWIN_Set_TranspColorStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPSTRCH_TRANSPCOLORMODE TColorStrchMode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if(TColorStrchMode < MAX_GOP_TRANSPCOLOR_STRETCH_MODE_SUPPORT)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_HVSTRCHMD, TColorStrchMode<<4, GOP_BIT4);
        return GOP_SUCCESS;
    }
    else
    {
        return GOP_ENUM_NOT_SUPPORTED;
    }
}
/********************************************************************************/
/// Write optional palette values to GOP0 registers
/// Note that this GOP0 palette table only support 256 group palette data
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/// @param ePalType \b IN Pallete format, true/false ARGB8888
/********************************************************************************/
void MDrv_GOP_GWIN_SetPaletteOpt(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, DRV_GopPalType ePalType)
{

    //----------------------------------------------------------------------
    // Write palette
    //----------------------------------------------------------------------
    MS_U32 regform;
    MS_U32 u32BankOffSet=0;
    MS_U8 j=0;
    MS_U16 i;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 u16ClkGated=0, u16tmp =0;

    regform = MDrv_GOP_GetRegForm(pGOPCtx, u8GOP, PAR_IGNORE);
    if((regform & E_GOP_PAL_SIZE_MASK) == E_GOP_PAL_SIZE_256)
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    }
    else
    {
        GOP_D_ERR("\nNot support GOP_%d palette!'\n",u8GOP);
        return;
    }

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, &u16ClkGated); //Get GOP clk dynamical gated
        if(u16ClkGated & 0x03)
        {
            //Disable clk gated when R/W palette
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, 0x00, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
        }
    }

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, &u16tmp);
    if ( ((u16tmp&0x3000)>>12) < 2 ) // internal palette
    {
        if (MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, E_DRV_GOP_PAL_GOP_RIU<<12, 0x3000);    // Set RIU access
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, E_DRV_GOP_PAL_GOP_REGDMA<<12, 0x3000);    // Set REGDMA access
        }
    }

    for(i=u32PalStart; i<=u32PalEnd ; i++)
    {
        #if 0
        printf("MDrv_GOP_GWIN_SetPaletteOpt :  Array[%03d]. u8A %02bX | u8R %02bX | u8G %02bX |u8B %02bX\n",i,
                                                pPalArray[j].RGB.u8A,
                                                pPalArray[j].RGB.u8R,
                                                pPalArray[j].RGB.u8G,
                                                pPalArray[j].RGB.u8B);
        #endif

        if(MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        {
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALDATA_L, ((pPalArray[j].RGB.u8A<<24)|\
                (pPalArray[j].RGB.u8R<<16)|(pPalArray[j].RGB.u8G<<8)|(pPalArray[j].RGB.u8B)));
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, i, GOP_REG_LW_MASK);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, GOP_BIT8, GOP_BIT8);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, 0, GOP_BIT8);
        }
        else
        {
            HAL_GOP_Write32Pal(&pGOPDrvLocalCtx->halCtxLocal, (MS_U8*)pGOPDrvLocalCtx->pu8REGMAP_Base[u8GOP], &pGOPDrvLocalCtx->u16REGMAP_Offset[u8GOP], pGOPDrvLocalCtx->u32REGMAP_Len[u8GOP],
                       i, pPalArray[j].RGB.u8A,
                      pPalArray[j].RGB.u8R,
                      pPalArray[j].RGB.u8G,
                      pPalArray[j].RGB.u8B);
        }

        pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOPPaletteInfo[u8GOP].PalSWArray[i]=((((MS_U32)pPalArray[j].RGB.u8A)<<24)|\
                (((MS_U32)pPalArray[j].RGB.u8R)<<16)|(((MS_U16)pPalArray[j].RGB.u8G)<<8)|((MS_U32)pPalArray[j].RGB.u8B));
        j++;
    }

    if(!MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
    {
        //pGOPDrvLocalCtx->pu8REGMAP_Base[u8CurGOP] is VA,
        //to check this address aligned must use VA2PA
        if((MS_VA2PA((MS_VIRT)pGOPDrvLocalCtx->pu8REGMAP_Base[u8GOP]) % (512*GOP_WordUnit)) != 0)
        {
            //If not align, use message to warn user.
            //Do not return here because of excuting GOP_RegdmaEnable
            GOP_D_DBUG("\n[%s] GOP_REGDMABASE_ADR need %dK aligment !!", __FUNCTION__, ((512*GOP_WordUnit)/1024));
        }
        GOP_RegdmaEnable(pGOPDrvLocalCtx, u8GOP);
    }


    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
    {
        if(u16ClkGated & 0x03)
        {
            //Restore clk gated when R/W palette
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, u16ClkGated, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
        }
    }

}

/********************************************************************************/
/// Set GOP0 palette table read out control
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/********************************************************************************/
void MDrv_GOP_GWIN_SetPaletteRead(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPalReadType type)
{
    MS_U32 u32CurGOPRegForm, u32BankOffSet=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    u32CurGOPRegForm = MDrv_GOP_GetRegForm(pGOPCtx, u8GOP, PAR_IGNORE);
    if(((u32CurGOPRegForm & E_GOP_PAL_SIZE_MASK) == E_GOP_PAL_SIZE_256) || ((u32CurGOPRegForm & E_GOP_PAL_SIZE_MASK) == E_GOP_PAL_SIZE_64))
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    }
    else if( type == E_DRV_GOP1_PAL_GOP_RIU || type==E_DRV_GOP1_PAL_GOP_REGDMA )  //use other GOP's Palette(4G) table
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    }
    else
    {
        GOP_D_FATAL("\nNot support GOP_%d palette!'\n",u8GOP);
        GOP_ASSERT(0);
        return;
    }

    //Set cur gop's palette
    if(pGOPCtx->pGopChipProperty->bNewPalette)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, type<<12, 0x3000);
        if(u32BankOffSet == (GOP_4G_OFST<<16))
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, type<<12, 0x3000);
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PALCTRL, type<<12, 0x3000);
        }
    }
    else
    {
        if ( (type == E_DRV_GOP1_PAL_GOP_RIU) || (type==E_DRV_GOP1_PAL_GOP_REGDMA ))
        {
            //For GOP1 use GOP0's palette, GOP0_PALCTRL should not config as "GOP0 use palette"
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, (type>>1)<<12, 0x3000);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PALCTRL, type<<12, 0x3000);
        }
        else
        {
            //For GOP0 use GOP0's palette, GOP1_PALCTRL should not config as "GOP1 use palette"
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, type<<12, 0x3000);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, type<<12|GOP_BIT13, 0x3000);
        }
    }

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}


/********************************************************************************/
/// Read GOP4G(GOP0) palette values From Pallete
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/********************************************************************************/
void MDrv_GOP_GWIN_ReadPalette(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U8 idx, MS_U32* clr)
{
    MS_U16 regval =0,regval1 =0, u16ClkGated=0;
    MS_U32 regform,u32BankOffSet=0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    regform = MDrv_GOP_GetRegForm(pGOPCtx, u8GOP, PAR_IGNORE);
    if((regform & E_GOP_PAL_SIZE_MASK) == E_GOP_PAL_SIZE_256)
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    }
    else
    {
        GOP_D_FATAL("\nNot support GOP_%d palette!'\n",u8GOP);
        GOP_ASSERT(0);
        return;
    }

    //----------------------------------------------------------------------
    // Read palette
    //----------------------------------------------------------------------

    *clr = 0;

    regval = idx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, &u16ClkGated); //Get GOP clk dynamical gated
        if(u16ClkGated & 0x03)
        {
            //Disable clk gated when R/W palette
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, 0x00, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
        }
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 1<<9, 1<<9);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, E_DRV_GOP_PAL_GOP_RIU<<12, 0x3000);    // Set RIU access
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, idx, GOP_REG_LW_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, 1<<9, 1<<9);   // Enable pallete read
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALCTRL, 0<<9, 1<<9);   // Clear pallete read
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0<<9, 1<<9);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALDATA_L, &regval);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_PALDATA_H, &regval1);

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
    {
        if(u16ClkGated & 0x03)
        {
            //Restore clk gated when R/W palette
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, u16ClkGated, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
        }
    }


    *clr = pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOPPaletteInfo[u8GOP].PalSWArray[idx];
}

#ifdef CONFIG_GOP_2ND_PAL
/********************************************************************************/
/// Write optional palette values to GOP1 registers
/// Note that this GOP1 palette table only support 64 group palette data
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/********************************************************************************/
void MDrv_GOP_GWIN_2GSetPaletteOpt(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
    MS_U16 i, u16ClkGated=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    //----------------------------------------------------------------------
    // Write palette
    //----------------------------------------------------------------------
    MS_U8 j=0;
    MS_U32 regform ;

    regform = MDrv_GOP_GetRegForm(pGOPCtx, E_GOP1, PAR_IGNORE);
    if((regform & E_GOP_PAL_SIZE_MASK) == E_GOP_PAL_SIZE_NONE)
    {
        GOP_D_ERR("\nNot support 2G palette!'\n");
        return;
    }

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, &u16ClkGated); //Get GOP clk dynamical gated
    if(u16ClkGated & 0x03)
    {
        //Disable clk gated when R/W palette
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, 0x00, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
    }

    if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bNewMux)
    {
        if (MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, E_DRV_GOP_PAL_GOP_RIU<<12, 0x3000);    // Set RIU access
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, E_DRV_GOP_PAL_GOP_REGDMA<<12, 0x3000);    // Set REGDMA access
        }
    }else
    {
        if (MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, E_DRV_GOP1_PAL_GOP_RIU<<12, 0x3000);    // Set RIU access
        }
        else
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, E_DRV_GOP1_PAL_GOP_REGDMA<<12, 0x3000);    // Set REGDMA access
        }
    }

    for(i=u32PalStart; i<=u32PalEnd ; i++)
    {
        #if 0
        printf("MDrv_GOP_GWIN_SetPaletteOpt :  Array[%03d]. u8A %02bX | u8R %02bX | u8G %02bX |u8B %02bX\n",i,
                                                pPalArray[j].RGB.u8A,
                                                pPalArray[j].RGB.u8R,
                                                pPalArray[j].RGB.u8G,
                                                pPalArray[j].RGB.u8B);
        #endif

        if (MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        {
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALDATA_L, (pPalArray[j].RGB.u8A<<24)|\
                (pPalArray[j].RGB.u8R<<16)|(pPalArray[j].RGB.u8G<<8)|(pPalArray[j].RGB.u8B));
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, i, GOP_REG_LW_MASK);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, GOP_BIT8, GOP_BIT8);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, 0, GOP_BIT8);
        }
        else
        {
        HAL_GOP_Write32Pal(&pGOPDrvLocalCtx->halCtxLocal,
                          pGOPDrvLocalCtx->pu8REGMAP_Base[1], &pGOPDrvLocalCtx->u16REGMAP_Offset[1], pGOPDrvLocalCtx->u32REGMAP_Len[1],
                          i, pPalArray[j].RGB.u8A,
                          pPalArray[j].RGB.u8R,
                          pPalArray[j].RGB.u8G,
                          pPalArray[j].RGB.u8B);
        }

        pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOPPaletteInfo[u8GOP].PalSWArray[i]=((((MS_U32)pPalArray[j].RGB.u8A)<<24)|\
                (((MS_U32)pPalArray[j].RGB.u8R)<<16)|(((MS_U32)pPalArray[j].RGB.u8G)<<8)|((MS_U32)pPalArray[j].RGB.u8B));
        j++;

    }

    if (!MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP))
        GOP_RegdmaEnable(pGOPDrvLocalCtx, 1);

    if(u16ClkGated & 0x03)
    {
        //Restore clk gated when R/W palette
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, u16ClkGated, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
    }
}


/********************************************************************************/
/// Set GOP1 palette table read out control
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/********************************************************************************/
void MDrv_GOP_GWIN_2GSetPaletteRead(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPalReadType type)
{
    MS_U16 u16tmp =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if ( type == E_DRV_GOP1_PAL_GOP_RIU || type==E_DRV_GOP1_PAL_GOP_REGDMA )
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PALCTRL, &u16tmp);
        if ( ((u16tmp&0x3000)>>12)==E_DRV_GOP_PAL_GOP_RIU || ((u16tmp&0x3000)>>12)==E_DRV_GOP_PAL_GOP_REGDMA)
        {
            //if GOP0 and GOP1 palette control are set for GOP0 GWIN at the same time, GOP0 palette will be used for GOP0 GWIN
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_PALCTRL, E_DRV_GOP1_PAL_GOP_RIU<<12, 0x3000);
        }
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, type<<12, 0x3000);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}


/********************************************************************************/
/// Read GOP2G(GOP1) palette values From Pallete
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/********************************************************************************/
void MDrv_GOP_GWIN_2GReadPalette(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U8 idx, MS_U32* clr)
{
    MS_U16 regval =0,regval1 =0, u16ClkGated=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    //----------------------------------------------------------------------
    // Read palette
    //----------------------------------------------------------------------
    *clr = 0;

    regval = idx;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, &u16ClkGated); //Get GOP clk dynamical gated
    if(u16ClkGated & 0x03)
    {
        //Disable clk gated when R/W palette
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, 0x00, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 1<<9, 1<<9);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, E_DRV_GOP1_PAL_GOP_RIU<<12, E_DRV_GOP1_PAL_GOP_RIU);    // Set RIU access
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, idx, GOP_REG_LW_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, 1<<9, 1<<9);   // Enable pallete read
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALCTRL, 0<<9, 1<<9);   // Clear pallete read
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0<<9, 1<<9);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALDATA_L, &regval);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_PALDATA_H, &regval1);
    if(u16ClkGated & 0x03)
    {
        //Restore clk gated when R/W palette
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_OLDADDR, u16ClkGated, (GOP_BIT1 | GOP_BIT0)); //enable GOP clk dynamical gated
    }
    *clr = pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.GOPPaletteInfo[u8GOP].PalSWArray[idx];
}
#endif

#ifdef CONFIG_GOP_SCROLL
/********************************************************************************/
/// Set GWIN auto scrolling frame rate (0~63)
/// @param u8rate \b IN frame rate
/********************************************************************************/
void MDrv_GOP_GWIN_SetScrollRate(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8rate)
{
    /*only gop0 has scrolling*/
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    regval = u8rate;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_RATE, regval&0x3f, 0x3f);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)pGOPDrvLocalCtx->current_gop);
}


/********************************************************************************/
/// Set horizontal auto scroll
/// @param u8win   \b IN GWIN id 0 ~ (MAX_GWIN_SUPPORT-1)
/// @param type    \b IN @copydoc GOPSCROLLTYPE
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/********************************************************************************/
void MDrv_GOP_GWIN_SetHScroll(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, E_GOP_SCROLL_TYPE type, MS_U32 offset, MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    MS_U16 regval = 0x0;
    MS_U16 u16GOP_Unit=0;
    MS_U32 u32regval =0;
    MS_U16 u16VaildMask=0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
    u16GOP_Unit = MDrv_GOP_GetWordUnit(pGOPCtx, u8GOP);

    if(type == GOP_DRV_SCROLL_KERNEL)
    {
        if(bEnable)
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_INT, 0x0, GOP_BIT0);
        else
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_INT, GOP_BIT0, GOP_BIT0);  //disable gop isr
    }
    else if(type == GOP_DRV_SCROLL_SW)
    {
        MS_U32 u32RegAddr;
        MS_U16 u16RegVal;
        MS_U16 MaxScrollNum, idx;
        MS_U32 u32ScrollAddr;

        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win),&regval );
        regval &= ~0x8;
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win), regval, GOP_REG_WORD_MASK);


        u32RegAddr = GOP_4G_INT;
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32RegAddr, &u16RegVal);

        MaxScrollNum = pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.ScrollBufNum;
        idx = pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.ScrollBufIdx;

        if(idx > MaxScrollNum)
            idx = 0;

        idx++;
        idx %= MaxScrollNum;

        if(idx == 0)
        {
            offset /= 8;
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HSTR(u8win), offset, GOP_REG_WORD_MASK);
            //printf("[%s (%d) MaxScrollNum:%d idx:%d offset:%d\n",__FUNCTION__,__LINE__,MaxScrollNum, idx,offset);

        }

        u32ScrollAddr = pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.ScrollBuffer[idx];
        u32ScrollAddr /= u16GOP_Unit;
        HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_L(u8win), u32ScrollAddr);

        pGOPDrvLocalCtx->pDrvCtxShared->apiCtxShared.ScrollBufIdx = idx;

        //clear int
        u16RegVal = GOP_BIT1;
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_INT, u16RegVal, GOP_BIT1);


        u16RegVal = 0x0;
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_INT, u16RegVal, GOP_BIT1);


        //HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0, GOP_REG_LW_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, (GOP_BIT10 | 0x0), GOP_REG_HW_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);


        //HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, GOP_VAL_WR, GOP_REG_HW_MASK);
        //HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);


        if(bEnable)
        {
            MsOS_EnableInterrupt(E_INT_IRQ_GOP); //Need check if needed.
        }
        else
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_INT, GOP_BIT1, GOP_BIT1);



    }
    else
    {
        if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
        {
            offset &= 0xFFF;  //horizontal use [12:0], 12 is sign bit.
            u16VaildMask = 0x1FFF;
        }
        else
        {
            offset &= 0x7F;  //Horizontal use [7:0], [7] is sign bit
            u16VaildMask = 0xFF;
        }

        u32regval = offset;
        if(type == GOP_DRV_SCROLL_RIGHT)
        {
            u32regval = (MS_U32)u16VaildMask - u32regval+1;
        }

        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_L(u8win), (u32regval&u16VaildMask), GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_H(u8win), (u32regval>>16), GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win), bEnable==TRUE?GOP_BIT3:0, GOP_BIT3);

        GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));
}
}

/********************************************************************************/
/// Set GWIN vertical auto scrolling
/// @param u8win \b IN GWIN id 0 ~ (MAX_GWIN_SUPPORT - 1)
/// @param type  \b IN \copydoc GOPSCROLLTYPE
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/********************************************************************************/
void MDrv_GOP_GWIN_SetVScroll(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, E_GOP_SCROLL_TYPE type, MS_U32 offset, MS_BOOL bEnable)
{
    MS_U16 regval =0;
    MS_U32 Ringoffset=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_HSIZE(u8win), &regval);
    Ringoffset = regval * offset;
    if(type == GOP_DRV_SCROLL_DOWN)
    {
        Ringoffset = 0x0 - Ringoffset;
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_L(u8win), (Ringoffset&0xFF), GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_H(u8win), (Ringoffset>>16), GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win),  bEnable== TRUE?GOP_BIT2:0, GOP_BIT2);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));
}


//-------------------------------------------------------------------------------------------------
/// Set Scroll auto stop
/// @param u8Wid    \b IN: GWin ID
/// @param bTrue    \b IN: TRUE: enable auto-stop mode, FALSE: disable auto-stop modea
//-------------------------------------------------------------------------------------------------
void MDrv_GOP_GWIN_Scroll_EnableAutoStop(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win), bEnable==TRUE?GOP_BIT1:0, GOP_BIT1);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));
}


//-------------------------------------------------------------------------------------------------
/// Set Scroll auto stop Horizontal offset
/// @param u8Wid             \b IN: GWin ID
/// @param u32ScrollAutoHStop \b IN: scorll auto stop H offset (GWIN H scrolling will stop when moving count meet this offset)
/// @return TRUE: sucess / FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GOP_GWIN_Scroll_AutoStop_HSet(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_U32 u32AutoHStopOffset)
{
    MS_U8 u8GOP;
    MS_U16 u16Meet;
    MS_U16 u16DramHStop;
    MS_U16 u16RegVal =0,u16tmp =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return false;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_L(u8win), &u16RegVal);

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        u16Meet = u16RegVal & GOP_BIT12;
    }
    else
    {
        u16Meet = u16RegVal & GOP_BIT7;
    }

    if(u16Meet)  //scroll right to left
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_HSIZE(u8win), &u16tmp); //pixel
        u16DramHStop = u16tmp - (((0-u16RegVal)&0xFF)*(MS_U16)u32AutoHStopOffset);
    }
    else    //scroll left to right
    {
        u16DramHStop = (MS_U32)u16RegVal*u32AutoHStopOffset;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_L(u8win), u16DramHStop , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_H(u8win), 0 , GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set Scroll auto stop Verticall offset
/// @param u8Wid             \b IN: GWin ID
/// @param u32AutoVStopOffset  \b IN: scorll auto stop V offset (GWIN V scrolling will stop when moving count meet this offset)
/// @return TRUE: sucess / FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GOP_GWIN_Scroll_AutoStop_VSet(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_U32 u32AutoVStopOffset)
{
    MS_U8 u8GOP;
    MS_U16 u32RegVal =0, u16tmp =0, u16tmp1 =0;
    MS_U32 u32DramVStop, u32RBLKSize;
    MS_U32 u32Meet,u32Mask;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return false;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_L(u8win), &u16tmp);
    u32RegVal = u16tmp;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RLEN_H(u8win), &u16tmp);
    u32RegVal += (u16tmp<<16);

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u8GOP] == TRUE)
    {
        u32Meet = u32RegVal & 0x100000; //BIT20
        u32Mask = 0x1FFF;
    }
    else
    {
        u32Meet = u32RegVal & GOP_BIT15;
        u32Mask = 0xFF;
    }

    if(u32Meet)  //scroll up to down
    {
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_SIZE_L(u8win), &u16tmp);
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_RBLK_SIZE_H(u8win), &u16tmp1);
        u32RBLKSize = (MS_U32)u16tmp1<<16 | (MS_U32)u16tmp;
        u32DramVStop = u32RBLKSize-(((0-u32RegVal)&u32Mask)*u32AutoVStopOffset);
    }
    else    //scroll down to up
    {
        u32DramVStop = (MS_U32)u32RegVal*u32AutoVStopOffset;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_L(u8win), u32DramVStop&0xffff , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_HVSTOP_H(u8win), u32DramVStop >> 16 , GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));

    return TRUE;
}
#endif

/********************************************************************************/
/// Set GWIN alpha blending
/// @param u8win \b IN \copydoc GWINID
/// @param bEnable \b IN
///   - # TRUE enable alpha blending
///   - # FALSE disable alpha blending
/// @param u8coef \b IN alpha blending coefficient (0-7)
/********************************************************************************/
void MDrv_GOP_GWIN_SetBlending(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
	DRV_GOP_CONSALPHA_BITS*	pVal;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }
    pVal = pGOPCtx->pGopConsAlphaBits;
#ifndef MSOS_TYPE_NOS
    if(DRV_NONE_SETTING == *pVal)
    {
        *pVal = pGOPCtx->pGopChipProperty->Default_ConsAlpha_bits;
        GOP_D_DBUG("\n[%s] because User_ConsAlpha_bits is not set by user, so drv set it to the default bits enum(%d)!!!\n",__FUNCTION__,
                    pGOPCtx->pGopChipProperty->Default_ConsAlpha_bits);
     }
#endif
    pGOPDrvLocalCtx->halCtxLocal.User_ConsAlpha_bits = *pVal;
    HAL_GOP_GWIN_SetBlending(&pGOPDrvLocalCtx->halCtxLocal, u8win, bEnable, u8coef);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));
}

/********************************************************************************/
/// Set GWIN data format to GOP registers
/// @param u8win \b IN \copydoc GWINID
/// @param clrtype \b IN \copydoc EN_GOP_COLOR_TYPE
/********************************************************************************/
void MDrv_GOP_GWIN_SetWinFmt(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOPColorType clrtype)
{
    MS_U8 u8GOP = 0;
    MS_U8 regform = 0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    //  add GOP reg struct type.
    regform = MDrv_GOP_GetRegForm(pGOPCtx, PAR_IGNORE, u8win);

    HAL_GOP_SetWinFmt(&pGOPDrvLocalCtx->halCtxLocal, regform, u8GOP, u8win, GOP_SetFmt(clrtype)<<4);
}

/********************************************************************************/
/// Enable GWIN for display
/// @param u8win \b IN GWIN id
/// @param bEnable \b IN
///   - # TRUE Show GWIN
///   - # FALSE Hide GWIN
/********************************************************************************/
void MDrv_GOP_GWIN_EnableGwin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable)
{
    MS_U8 u8GOP =0;
    MS_U32 u32BankOffSet = 0;
    MS_U8 regform;
#ifdef CONFIG_MSTAR_SRAMPD
    MS_U8 u8Idx=0,u8MaxWinNum=0;
    MS_U16 u16tmp=0;
    MS_BOOL bGOPUsed=FALSE;
#endif

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }
    GOP_D_INFO("MDrv_GOP_GWIN_Enable(gId=%d) == %d\n",u8win, bEnable);

    pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bGWINEnable[u8win] = bEnable;
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);


    if(bEnable == TRUE)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, 0, GOP_BIT11);
#ifdef CONFIG_MSTAR_SRAMPD
        pGOPCtx->pGOPCtxShared->bGOPPowerDown[u8GOP]=FALSE;
#endif
    }

	//do gwin addr check to avoid invalid addr display
    if(_GWIN_ADDR_Invalid_Check(pGOPCtx,u8win) == FALSE)
    {
        GOP_D_ERR("\n[%s] Current Gwin(%d) Addr is invalid XXXX\n",__FUNCTION__, u8win);
        return;
    }

    regform = MDrv_GOP_GetRegForm(pGOPCtx, PAR_IGNORE, u8win);

#ifdef GOP_AUTO_CLK_GATING_PATCH
    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, 0x0, (GOP_BIT1 | GOP_BIT0) );
#endif

    if (((regform & E_GOP_REG_FORM_MASK) == E_GOP_REG_FORM_T21G) || ((regform & E_GOP_REG_FORM_MASK) == E_GOP_REG_FORM_T81G))
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN0_CTRL(Gop23_GwinCtl_Ofet), \
            bEnable?TRUE:FALSE, 0x0001);
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN0_CTRL(u8win), \
            bEnable?TRUE:FALSE, 0x0001);
    }
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));

#ifdef GOP_AUTO_CLK_GATING_PATCH
    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGOPAutoClkGating)
    {
        if(bEnable == FALSE)
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, (GOP_BIT1 | GOP_BIT0), (GOP_BIT1 | GOP_BIT0) );
    }
#endif

#ifdef CONFIG_MSTAR_SRAMPD
    bGOPUsed = FALSE;
    if(bEnable == FALSE)
    {
        u8MaxWinNum = HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, u8GOP);
        for (u8Idx=0; u8Idx<u8MaxWinNum; u8Idx++)
        {
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN0_CTRL(u8Idx),&u16tmp);
            if(u16tmp&GOP_BIT0)
            {
                bGOPUsed=TRUE;
            }
        }
        if(bGOPUsed==TRUE)
            pGOPCtx->pGOPCtxShared->bGOPPowerDown[u8GOP]=FALSE;
        else
            pGOPCtx->pGOPCtxShared->bGOPPowerDown[u8GOP]=TRUE;
    }
#endif
}

GOP_Result MDrv_GOP_InitMux(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_GOP_InitMux(&pGOPDrvLocalCtx->halCtxLocal);
}

MS_BOOL MDrv_GOP_HasGop1GPalette(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGop1GPalette;
}
static GOP_Result _GOPFBAddCheck(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 gopnum,DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U8 u8MiuSel;

    _phy_to_miu_offset(u8MiuSel, pinfo->u64DRAMRBlkStart, pinfo->u64DRAMRBlkStart);
    if (FALSE==_IsGopNumVaild(pGOPCtx, gopnum))
    {
        GOP_D_ERR("\n[%s] not support gop id:%d in this chip version",__FUNCTION__, gopnum);
        return GOP_FAIL;
    }

    if(pGOPCtx->pGOPCtxShared->bTLB[gopnum] != TRUE)
    {
        if(MDrv_GOP_Get_MIU_Sel(pGOPCtx,gopnum) != u8MiuSel)
        {
            GOP_D_DBUG("\n %s  GOP[%d] address(0x%tx) select from %d to %d\n",__FUNCTION__,gopnum,(ptrdiff_t)pinfo->u64DRAMRBlkStart,MDrv_GOP_Get_MIU_Sel(pGOPCtx,gopnum),u8MiuSel);
        }
    }
    return GOP_SUCCESS;
}
GOP_Result MDrv_GOP_GWIN_SetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8GOP;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return GOP_FAIL;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support gop id:%d in this chip version",__FUNCTION__, u8GOP);
        return GOP_FAIL;
    }

    switch(u8GOP)
    {
        case E_GOP0:
            if (GOP_SUCCESS ==_GOPFBAddCheck(pGOPCtx,0,pinfo))
                GOP_SetGop0GwinInfo(pGOPDrvLocalCtx, u8win, pinfo);
            else
                return GOP_FAIL;

            break;
        case E_GOP1:
            if (GOP_SUCCESS ==_GOPFBAddCheck(pGOPCtx,1,pinfo))
                GOP_SetGop1GwinInfo(pGOPDrvLocalCtx, u8win, pinfo);
            else
                return GOP_FAIL;

            break;
        case E_GOP2:
        case E_GOP3:
        case E_GOP4:
        case E_GOP5:
            if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGop1GPalette && (pinfo->clrType==E_DRV_GOP_COLOR_I8 || pinfo->clrType==E_DRV_GOP_COLOR_2266\
                ||pinfo->clrType==E_DRV_GOP_COLOR_RGB555_BLINK))
            {
                GOP_D_ERR("\n[%s] u8win:%d not support index mode",__FUNCTION__, u8win);
                return GOP_FAIL;
            }

            if (GOP_SUCCESS ==_GOPFBAddCheck(pGOPCtx,u8GOP,pinfo))
                GOP_SetGopExtendGwinInfo(pGOPDrvLocalCtx, u8win, pinfo);
            else
                return GOP_FAIL;
            break;
        default:
            break;
    }

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_GetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8GOP;
    GOP_WinFB_INFO pwinFB;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return GOP_FAIL;
    }

    if(pGOPCtx->pGOPCtxShared->gwinMap[u8win].u32CurFBId > DRV_MAX_GWIN_FB_SUPPORT)
    {
        GOP_D_ERR("[%s][%d] WrongFBID=%td\n",__FUNCTION__,__LINE__,(ptrdiff_t)pGOPCtx->pGOPCtxShared->gwinMap[u8win].u32CurFBId);
        return GOP_FAIL;
    }

    #if WINFB_INSHARED
            pwinFB = pGOPCtx->pGOPCtxShared->winFB[pGOPCtx->pGOPCtxShared->gwinMap[u8win].u32CurFBId];
    #else
            pwinFB = pGOPCtx->winFB[pGOPCtx->pGOPCtxShared->gwinMap[u8win].u32CurFBId];
    #endif

    pinfo->u16RBlkHPixSize = pwinFB.width;
    pinfo->u16RBlkVPixSize = pwinFB.height;

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
    switch(u8GOP)
    {
        case E_GOP0:
            GOP_ReadGop0GwinInfo(pGOPDrvLocalCtx, u8win,  pinfo);
            HAL_ConvertAPIAddr(&pGOPDrvLocalCtx->halCtxLocal, u8win, &(pinfo->u64DRAMRBlkStart));
            break;
        case E_GOP1:
            GOP_ReadGop1GwinInfo(pGOPDrvLocalCtx, u8win,  pinfo);
            HAL_ConvertAPIAddr(&pGOPDrvLocalCtx->halCtxLocal, u8win, &(pinfo->u64DRAMRBlkStart));
            break;
        case E_GOP2:
        case E_GOP3:
        case E_GOP4:
        case E_GOP5:
            if (!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGop1GPalette  && (pinfo->clrType==E_DRV_GOP_COLOR_I8 || pinfo->clrType==E_DRV_GOP_COLOR_2266))
            {
                GOP_D_ERR("\n[%s] u8win:%d not support index mode",__FUNCTION__, u8win);
                return GOP_FAIL;
            }
            GOP_ReadGopExtendGwinInfo(pGOPDrvLocalCtx, u8win,  pinfo);
            HAL_ConvertAPIAddr(&pGOPDrvLocalCtx->halCtxLocal, u8win, &(pinfo->u64DRAMRBlkStart));
            break;
        default:
            break;
    }

    return GOP_SUCCESS;
}

#ifdef CONFIG_GOP_BG
/********************************************************************************/
/// Enable GWIN background window
/// @param bgwID \b IN GWIN ID
/// @param MS_BOOL \b IN
///   - # TRUE enable
///   - # FALSE disable
/********************************************************************************/
GOP_Result MDrv_GOP_GWIN_Enable_BGWIN(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    /*only gop4g has background window*/
    if(bgwID >= HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, 0))
    {
        GOP_ASSERT(0);
        return GOP_FAIL;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_FADE(bgwID), bEnable==TRUE?GOP_BIT13:0, GOP_BIT13);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP0);
    return GOP_SUCCESS;
}


/********************************************************************************/
/// Set GWIN background window property
/// @param bgwID \b IN GWIN ID
/// @param x0 \b IN background window horizontal start position
/// @param y0 \b IN  background window vertical start position
/// @param w \b IN  background window width
/// @param h \b IN  background window height
/// @param color \b IN background window color
/********************************************************************************/
void MDrv_GOP_GWIN_Set_BGWIN(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_U16 x0, MS_U16 y0, MS_U16 w, MS_U16 h, MS_U16 color)
{
     GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(bgwID >= HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, 0))
    {
        GOP_ASSERT(0);
        return;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_BG_VSTR(bgwID), y0 , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_BG_HSTR(bgwID), x0 , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_BG_VEND(bgwID), y0 + h , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_BG_HEND(bgwID), x0 + w , GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_BG_CLR(bgwID), color , GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP0);
}

void MDrv_GOP_GWIN_Set_BGWIN_Alpha(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_BOOL bEnable, MS_U8 alpha)
{
    MS_U16 u16reg;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(bgwID >= HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, 0))
    {
        GOP_ASSERT(0);
        return;
    }
    u16reg = (alpha|(bEnable<<4))<<8;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_DRAM_FADE(bgwID), u16reg , 0x1F00);

}
#endif
void MDrv_GOP_GWIN_IsGWINEnabled(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL* pbEnable )
{
    MS_U16 regval=0;
    MS_U8 u8GOP;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
    switch(u8GOP)
    {
        case E_GOP0:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GWIN0_CTRL(u8win), &regval);
            break;
        case E_GOP1:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_2G_GWIN0_CTRL(u8win-HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, 0)), &regval);
            break;
        case E_GOP2:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1G_GWIN0_CTRL, &regval);
            break;
        case E_GOP3:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GX_GWIN0_CTRL, &regval);
            break;
        case E_GOP4:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS0_GWIN0_CTRL, &regval);
            break;
        case E_GOP5:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_1GS1_GWIN0_CTRL, &regval);
            break;
        default:
            break;
    }

    if(regval & 0x0001)
    {
        *pbEnable = TRUE;
    }
    else
    {
        *pbEnable = FALSE;
    }
}

void MDrv_GOP_IsGOPMirrorEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL *bHMirror, MS_BOOL *bVMirror)
{
    MS_U16 u16RegVal = 0;
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }
    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &u16RegVal);
    *bHMirror = (u16RegVal & GOP_BIT12)>>12;
    *bVMirror = (u16RegVal & GOP_BIT13)>>13;
}

void MDrv_GOP_GWIN_EnableHMirror(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }
    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if(bEnable == TRUE)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, GOP_BIT12, GOP_BIT12);
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, 0, GOP_BIT12);
}


void MDrv_GOP_GWIN_EnableVMirror(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild((MS_GOP_CTX_LOCAL*)pGOPDrvLocalCtx, u8GOPNum))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!",__FUNCTION__, u8GOPNum);
        return;
    }
    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if(bEnable == TRUE)
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, GOP_BIT13, GOP_BIT13);
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, 0, GOP_BIT13);

}

GOP_Result MDrv_GOP_TLBEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bTLBEn)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_EnableTLB(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, bTLBEn);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_SetTLBAddr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_PHY u64TLBAddr, MS_BOOL bHMirror,MS_BOOL bVMirror,DRV_GOP_GWIN_INFO* pinfo)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32offsetsize=0;
    MS_U16 bpp;

    if(!pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bTLBSupport[u8GOP])
    {
        GOP_D_ERR("\n[%s] not support TLB feature in this chip version",__FUNCTION__);
        return GOP_FAIL;
    }

    bpp = HAL_GOP_GetBPP(&pGOPDrvLocalCtx->halCtxLocal, pinfo->clrType);
    if(bpp == FB_FMT_AS_DEFAULT)
    {
        return GOP_FAIL;
    }

    if((bHMirror)&&(bVMirror))
    {
        u32offsetsize = (pinfo->u16RBlkHPixSize)*(pinfo->u16RBlkVPixSize)*(bpp/PER_BYTE_BITS);
    }
    else if(bHMirror)
    {
        u32offsetsize = pinfo->u16RBlkHPixSize*(bpp/PER_BYTE_BITS);
    }
    else if(bVMirror)
    {
        u32offsetsize = (pinfo->u16RBlkHPixSize)*((pinfo->u16RBlkVPixSize)-1)*(bpp/PER_BYTE_BITS);
    }
    else
    {
        u32offsetsize=0;
    }

    u32offsetsize=(u32offsetsize/(PER_MIU_TLB_ENTRY_COUNT*PAGE_SIZE))*(PER_MIU_TLB_ENTRY_COUNT*PAGE_SIZE);

    u64TLBAddr /=ADDRESSING_8BYTE_UNIT;
    u32offsetsize /=ADDRESSING_8BYTE_UNIT;

    HAL_GOP_SetTLBAddr(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, u64TLBAddr,u32offsetsize);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_SetTLBSubAddr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_PHY u64TLBAddr)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(!pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bTLBSupport[u8GOP])
    {
        GOP_D_ERR("\n[%s] not support TLB feature in this chip version",__FUNCTION__);
        return GOP_FAIL;
    }

    u64TLBAddr /=ADDRESSING_8BYTE_UNIT;

    HAL_GOP_SetTLBSubAddr(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, u64TLBAddr);

    return GOP_SUCCESS;
}

/******************************************************************************/
/// Set the time when new DWIN settings take effect
/// @param bEnable \b IN
///   - # TRUE the new setting moved from internal register buffer
///            to active registers immediately
///   - # FALSE new settings take effect when next VSYNC is coming
/******************************************************************************/
void MDrv_GOP_DWIN_SetForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    pGOPDrvLocalCtx->gop_dwin_frwr = bEnable;
}


/******************************************************************************/
/// Capture every frames
/// @param bEnable \b IN
///   - # TRUE start capture
///   - # FALSE stop capture
/******************************************************************************/
void MDrv_GOP_DWIN_EnableCaptureStream(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &regval);

    if(bEnable)
    {
        regval &= ~0x0080;
        regval |= 0x0001;
    }
    else
    {
        regval |= 0x0080;
        regval &= ~0x0001;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx,E_GOP_Dwin);
}


/******************************************************************************/
/// Capture one frame
/******************************************************************************/
#define DWIN_CAPTURE_TIMEOUT_CNT_I 80UL //frame rate 50, maximum time is button + top + button = 20+20+20 = 60ms, and give extra 20ms
#define DWIN_CAPTURE_TIMEOUT_CNT_P 40UL //frame rate 50, maximum time is 20ms, and give extra 20ms
void MDrv_GOP_DWIN_CaptureOneFrame(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    MS_U16 regval =0, u16DWinState=0;
    MS_U32 u32StartT = 0,u32EndT = 0;
    MS_BOOL bInterlace = FALSE, bTOPOK= FALSE;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_BOOL bForceWrite= MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, E_GOP0);

    //mj@ 2008.09.22 avoid DWIN lack the most left line
    //HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, GOP_BIT11, GOP_BIT11);

    // Reset
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &regval);
    bInterlace = !((regval & BIT(4)) >> 4);
    regval &= ~0x01;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);

    if(bInterlace)
    {
        regval &= ~0x80; //Use stream capture mode to capture 2 field continuously
        regval |= 0x01;
    }
    else
    {
        regval |= 0x81; //Stop capture if one frame/field is done
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);

    u32StartT = MsOS_GetSystemTime();
    if(bInterlace)
    {
        do
        {
            MsOS_DelayTask(1);//check every 1ms
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_DEBUG, &u16DWinState);
            u16DWinState = u16DWinState & 0x7;
            if(bTOPOK == FALSE)
            {
                if(u16DWinState == 0x2)
                {
                    bTOPOK = TRUE; //TOP field is done
                }
            }
            else
            {
                if(u16DWinState == 0x4)
                {
                    u32EndT = MsOS_GetSystemTime();
                    break; //Bottom field is done
                }
            }
            u32EndT = MsOS_GetSystemTime() ;
        }while( (u32EndT - u32StartT)< DWIN_CAPTURE_TIMEOUT_CNT_I);
    }
    else
    {
        do
        {
            MsOS_DelayTask(1);//check every 1ms
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_DEBUG, &u16DWinState);
            u16DWinState = u16DWinState & 0x7;
            if(u16DWinState == 0x1 || u16DWinState == 0x2 || u16DWinState == 0x4)
            {
                u32EndT = MsOS_GetSystemTime();
                break;
            }
            u32EndT = MsOS_GetSystemTime() ;
        }while( (u32EndT - u32StartT)< DWIN_CAPTURE_TIMEOUT_CNT_P );
    }

    regval |= 0x80;
    regval &= ~0x01;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);

    if(!bForceWrite)
    {
        MDrv_GOP_GWIN_ForceWrite_Update(pGOPCtx, E_GOP0, TRUE);//Force write to turn off DWIN
    }
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);

    if(!bForceWrite)
    {
        MDrv_GOP_GWIN_ForceWrite_Update(pGOPCtx, E_GOP0, bForceWrite);
    }

    if(  ((bInterlace == TRUE) && ((u32EndT - u32StartT) >= DWIN_CAPTURE_TIMEOUT_CNT_I))
       ||((bInterlace == FALSE) && ((u32EndT - u32StartT) >= DWIN_CAPTURE_TIMEOUT_CNT_P)))
    {
        GOP_D_DBUG("[%s][%d]!!!Alert !!! DWIN Capture[i=%u, bTOPOK=%u, Reg_DW_CTL0=%x], wait ack time out!\n",__FUNCTION__,__LINE__, bInterlace, bTOPOK, regval);
    }
}

void MDrv_GOP_DWIN_CaptureOneFrame2(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    MDrv_GOP_GWIN_SetForceWrite(pGOPCtx, TRUE);

    // Reset
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &regval);

    regval |= 0x80;
    regval &= ~0x01;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);

    regval |= 0x0c81;      // NULL Mode for 1/2 down scaling, for shift 1 pixl
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_REG_WORD_MASK);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);

    MDrv_GOP_GWIN_SetForceWrite(pGOPCtx, FALSE);
}

GOP_Result MDrv_GOP_DWIN_SetSourceSel(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_SRC_SEL enSrcSel)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    GOP_Result enRet = GOP_FAIL;

    DRV_GOP_XC_GET_SEMPHORE();
    enRet = HAL_GOP_DWIN_SetSourceSel(&pGOPDrvLocalCtx->halCtxLocal, enSrcSel);
    DRV_GOP_XC_RELEASE_SEMPHORE();
    return enRet;
}

/******************************************************************************/
/// Set DWIN setting to registers
/// @param pinfo \b IN \copydoc GOP_DwinInfo
/******************************************************************************/
void MDrv_GOP_DWIN_SetWinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_INFO* pinfo)
{
    MS_U32 u32tmp =0,u32tmp1 =0;
    MS_U16 u16tmp =0;
    MS_U8 bpp; // byte per pixel
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8MiuSel;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN,&u16tmp);
    if ((u16tmp >> 14) == 0x2) //ARGB8888
        bpp=4;
    else
        bpp=2;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_LSTR_WBE, 0xff, GOP_REG_LW_MASK);

    u16tmp = pinfo->u16VPixelStart;      // pix
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_VSTR, u16tmp, GOP_REG_WORD_MASK);

    u16tmp = pinfo->u16VPixelEnd;        // pix
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_VEND, u16tmp, GOP_REG_WORD_MASK);

    u16tmp = pinfo->u16HPixelStart/(GOP_DWIN_WordUnit/bpp);     // 4 pixels
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_HSTR, u16tmp, GOP_REG_WORD_MASK);

    u16tmp = pinfo->u16HPixelEnd/(GOP_DWIN_WordUnit/bpp);       // 4 pixels
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_HEND, u16tmp, GOP_REG_WORD_MASK);

    u16tmp = (pinfo->u16HPixelEnd - pinfo->u16HPixelStart)/(GOP_DWIN_WordUnit/bpp);     // 4 pixels
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_HSIZE, u16tmp, GOP_REG_WORD_MASK);

    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u32tmp, pinfo->u64TFDRAMAddr);
    u32tmp /= GOP_DWIN_WordUnit;    // word(8-byte)
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_DW_DSTR_L, u32tmp);
    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u32tmp, pinfo->u64BFDRAMAddr);
    u32tmp /= GOP_DWIN_WordUnit;      // word(8-byte)
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_DW_UB_L, u32tmp);

    //Ring size
	u16tmp = (pinfo->u16HPixelEnd - pinfo->u16HPixelStart)/(GOP_DWIN_WordUnit/bpp);
    u32tmp = u16tmp * (pinfo->u16VPixelEnd - pinfo->u16VPixelStart);
    //Buffer size
	u32tmp1 = (pinfo->u64BFDRAMAddr - pinfo->u64TFDRAMAddr)/GOP_DWIN_WordUnit;
    HAL_GOP_DWIN_SetRingBuffer(&pGOPDrvLocalCtx->halCtxLocal,u32tmp,u32tmp1);

    u16tmp = pinfo->u16DRAMJumpLen/GOP_DWIN_WordUnit;     // word(8-byte)
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_JMPLEN, u16tmp, GOP_REG_WORD_MASK);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);
}


/******************************************************************************/
/// Get DWIN setting fromo registers
/// @param pinfo \b IN \copydoc GOP_DwinInfo
/******************************************************************************/
void MDrv_GOP_DWIN_GetWinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_INFO* pinfo)
{
    MS_U16 u16tmp =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8MiuSel;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_VSTR, &u16tmp);
    pinfo->u16VPixelStart = u16tmp;      // pix

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_VEND, &u16tmp);
    pinfo->u16VPixelEnd = u16tmp;        // pix

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_HSTR, &u16tmp);
    pinfo->u16HPixelStart = u16tmp << 2;     // 4 pixels

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_HEND, &u16tmp);
    pinfo->u16HPixelEnd = u16tmp << 2;       // 4 pixels

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_DSTR_H, &u16tmp);
    pinfo->u64TFDRAMAddr = u16tmp;      // word
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_DSTR_L, &u16tmp);
    pinfo->u64TFDRAMAddr <<= 16;
    pinfo->u64TFDRAMAddr |= u16tmp;      // word
    pinfo->u64TFDRAMAddr = pinfo->u64TFDRAMAddr*GOP_DWIN_WordUnit;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_UB_H, &u16tmp);
    pinfo->u64BFDRAMAddr = u16tmp;      // word
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_UB_L, &u16tmp);
    pinfo->u64BFDRAMAddr <<= 16;
    pinfo->u64BFDRAMAddr |= u16tmp ;      // word
    pinfo->u64BFDRAMAddr = pinfo->u64BFDRAMAddr*GOP_DWIN_WordUnit;

    u8MiuSel = HAL_GOP_GetDWINMIU(&pGOPDrvLocalCtx->halCtxLocal);
    _miu_offset_to_phy(u8MiuSel, pinfo->u64BFDRAMAddr, pinfo->u64BFDRAMAddr);
    _miu_offset_to_phy(u8MiuSel, pinfo->u64TFDRAMAddr, pinfo->u64TFDRAMAddr);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_JMPLEN, &u16tmp);
    pinfo->u16DRAMJumpLen = u16tmp *GOP_DWIN_WordUnit;     // word
    MDrv_GOP_DWIN_GetDataFmt(pGOPCtx, &(pinfo->u8fmt));
}


/******************************************************************************/
/// Set DWIN capture in interlaced or progressive mode
/// @param bEnable \b IN
///   - # TRUE  DWIN progressive mode
///   - # FALSE DWIN interlaced mode
/// @see MDrv_GOP_DWIN_SetMode
/******************************************************************************/
void MDrv_GOP_DWIN_EnableProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &regval);

    if(bEnable)
        regval |= 0x10;
    else
        regval &= ~0x10;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_BIT4);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);
}


/******************************************************************************/
/// Set DWIN data source
/// @param enScan \b IN \copydoc EN_GOP_DWIN_DATA_SRC
/// @see MDrv_GOP_DWIN_EnableProgressive
/// @see MDrv_GOP_DWIN_SetMode
/******************************************************************************/
void MDrv_GOP_DWIN_SelectSource(MS_GOP_CTX_LOCAL*pGOPCtx, EN_GOP_DWIN_DATA_SRC enSrc)
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (enSrc == GOPDWIN_DATA_SRC_SCALAR)
    {
        regval = 0x0000;
    }
    else if (enSrc == GOPDWIN_DATA_SRC_EXTRGB)
    {
        regval = 0x0200;
    }
    else
    {
        regval = 0x0300;
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, 0x0300);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);
}


/******************************************************************************/
/// Set DWIN source scan type
/// @param enScan \b IN \copydoc EN_GOP_DWIN_SCAN_TYPE
/******************************************************************************/
void MDrv_GOP_DWIN_SelectSourceScanType(MS_GOP_CTX_LOCAL*pGOPCtx, EN_GOP_DWIN_SCAN_TYPE enScan)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &regval);

    if (enScan == GOPDWIN_SCAN_MODE_PROGRESSIVE)
    {
        regval |= 0x10;
        regval &= ~0x20; //off field inverse
    }
    else if (enScan == GOPDWIN_SCAN_MODE_INTERLACE)
    {
        regval &= ~0x10;
        regval |= 0x20; //Interlace, always turn on field inverse(Tested HDMI/DTV/AV)
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, regval, GOP_BIT4);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);

}


/******************************************************************************/
/// Set interrupt mask of GOP DWIN.
/// @param u8mask \b IN bitmask of interrupt type
///   - bit7 DWIN VSYNC interrupt mask
///   - bit6 Dump Window interlace Bottom Field ACK Interrupt Mask
///   - bit5 Dump Window interlace Top Field ACK Interrupt Mask
///   - bit4 Dump Window Progressive ACK Interrupt Mask
///   - bit3:1 Reserved
///   - bit0 GWIN VSYNC Interrupt Mask
/// @param bEnable \b IN
///   - # TRUE enable interrupts specified by u8mask
///   - # FALSE disable interrupts specified by u8mask
/// @see MDrv_GOP_ClearIntr
/// @see MDrv_GOP_GetIntrStatus
/******************************************************************************/
void MDrv_GOP_DWIN_EnableIntr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u8mask, MS_BOOL bEnable)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_INT_MASK, &regval);

    if(bEnable)
        regval &= ~u8mask;
    else
        regval |= u8mask;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_INT_MASK, regval, GOP_REG_LW_MASK);
}


/******************************************************************************/
/// Clear Interrupt
/// @param u8mask \b IN see u8mask in MDrv_GOP_EnableIntr()
/// @see MDrv_GOP_GetIntrStatus
/// @see MDrv_GOP_EnableIntr
/******************************************************************************/
void MDrv_GOP_DWIN_ClearIntr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u8mask)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_INT_MASK, u8mask&0xff, GOP_REG_LW_MASK);
}

/******************************************************************************/
/// Get DWin Interrupt Information
/// @param pDWinIntInfo \b OUT: dwin interrup flag
/******************************************************************************/
GOP_Result MDrv_GOP_DWIN_GetDWinIntInfo(MS_GOP_CTX_LOCAL *pGOPCtx, DRV_GOP_DWinIntInfo *pGopDWinIntInfo, MS_U32 u32Timeout)
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;

    gflipDWinIntInfo.u8DWinIntInfo = 0x0;

    if(MDrv_GFLIP_GetDWinIntInfo(pGOPCtx, &gflipDWinIntInfo,u32Timeout))
    {
        pGopDWinIntInfo->u8DWinIntInfo = gflipDWinIntInfo.u8DWinIntInfo;
        return GOP_SUCCESS;
    }

    return GOP_FAIL;
}

/******************************************************************************/
/// Set DWIN control word
/// @param val \b IN value wanna write to control word
/// @param mask \b IN mask of control word
/******************************************************************************/
void MDrv_GOP_DWIN_Set_CTRL0(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 val, MS_U16 mask)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, val, mask);
  //  GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, E_GOP_Dwin);
}

GOP_Result MDrv_GOP_ConvertAPIAddr2HAL(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gwinid, MS_PHY* u64ApiAdr)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_ConvertAPIAddr(&pGOPDrvLocalCtx->halCtxLocal, u8Gwinid, u64ApiAdr);
}

/******************************************************************************/
/// Set DWIN output data format
/// @param fmt \b IN DWIN output format(RGB565/YUV...)
/******************************************************************************/
void MDrv_GOP_DWIN_SetDataFmt(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_DATA_FMT fmt)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, (MS_U16)fmt<<14, 0xC000);
}


/******************************************************************************/
/// Get DWIN output data format
/// @param fmt \b IN DWIN output format(RGB565/YUV...)
/******************************************************************************/
void MDrv_GOP_DWIN_GetDataFmt(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 *fmt)
{
    MS_U16 u16Temp=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, &u16Temp);
    *fmt = (DRV_GOP_DWIN_DATA_FMT)(u16Temp>>14);
}

/******************************************************************************/
/// Set DWIN output data constant alpha value for ARGB8888
/// @param u8AlphaVal \b IN: constant alpha value
/******************************************************************************/
void MDrv_GOP_DWIN_SetAlphaValue(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8AlphaVal)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, ((MS_U16)u8AlphaVal)<<8, 0xFF00);
}

/******************************************************************************/
/// Set DWIN output Alpha source
/// @param source \b IN: alpha source
/******************************************************************************/
void MDrv_GOP_DWIN_SetAlphaSrc(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 source)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, source<<1, GOP_BIT1);
}

/******************************************************************************/
/// Set DWIN output Alpha inverse
/// @param source \b IN: enable alpha inverse
/******************************************************************************/
void MDrv_GOP_DWIN_SetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, bEnable<<7, GOP_BIT7);
}

/******************************************************************************/
/// Set DWIN UVSample mode
/// @param mode \b IN: UVSample mode
/******************************************************************************/
void MDrv_GOP_DWIN_SetUVSample(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 mode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, mode<<2, (GOP_BIT2|GOP_BIT3));
}

/******************************************************************************/
/// Set DWIN skipframe mode
/// @param mode \b IN: skipframe count
/******************************************************************************/
void MDrv_GOP_DWIN_SetSkipFrame(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 framecount)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_FRAME_CTRL, framecount<<4, (GOP_BIT4|GOP_BIT5|GOP_BIT6|GOP_BIT7));
}

/******************************************************************************/
/// Set DWIN output data format
/// @param fmt \b IN DWIN output format(RGB565/YUV...)
/******************************************************************************/
void MDrv_GOP_DWIN_EnableDither(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (bEnable)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, (MS_U16)0x0020, 0x0020);
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, ~(MS_U16)0x0020, 0x0020);
    }
}


/******************************************************************************/
/// Get Interrupt Status
/// @return see u8mask in MDrv_GOP_EnableIntr()
/// @see MDrv_GOP_EnableIntr
/// @see MDrv_GOP_ClearIntr
/******************************************************************************/
MS_U16 MDrv_GOP_DWIN_GetIntrStatus(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    MS_U16 regval =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_INT_MASK, &regval);
    return (regval>>8);
}
/******************************************************************************/
/// Set DWIN output data format
/// @param fmt \b IN DWIN output format(RGB565/YUV...)
/******************************************************************************/
GOP_Result MDrv_GOP_DWIN_EnableR2YCSC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_GOP_DWIN_EnableR2YCSC(&pGOPDrvLocalCtx->halCtxLocal,bEnable);
}

/******************************************************************************/
/// Set DWIN output data UV SWAP
/// @param ENABLE
///   - # TRUE enable UV SWAP
///   - # FALSE disable UV SWAP
/******************************************************************************/
void MDrv_GOP_DWIN_SetUVSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (bEnable)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, (MS_U16)GOP_BIT11, GOP_BIT11);
    }
    else
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_CTL0_EN, (~(MS_U16)GOP_BIT11), GOP_BIT11);
    }
}


/******************************************************************************/
/// Set Scaler VOP New blending level
/******************************************************************************/
void MDrv_GOP_SetVOPNBL(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    DRV_GOP_XC_GET_SEMPHORE();

	if(bEnable == TRUE)
	{
    	HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_VOPNBL, 0x20, 0x20);
    	HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_OSD_CHECK_ALPHA, 0x0, 0x40);
	}
	else
	{
		HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_VOPNBL, 0x0, 0x20);
		HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_OSD_CHECK_ALPHA, 0x40, 0x40);
	}
	DRV_GOP_XC_RELEASE_SEMPHORE();
}


//******************************************************************************
//  [Doxygen]
/// Get DWIN property
/// @param pproperty \b OUT buffer to store DWIN property
//******************************************************************************
void MDrv_GOP_DWIN_GetWinProperty(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DwinProperty *pproperty)
{
    DRV_GOP_DWIN_INFO info;

    MDrv_GOP_DWIN_GetWinInfo(pGOPCtx, &info);
    pproperty->u64fbaddr0 = info.u64TFDRAMAddr;
    pproperty->u64fbaddr1 = info.u64BFDRAMAddr;

    pproperty->u16x = info.u16HPixelStart;
    pproperty->u16w = info.u16HPixelEnd - info.u16HPixelStart ;
    pproperty->u16y = info.u16VPixelStart;
    pproperty->u16h = info.u16VPixelEnd - info.u16VPixelStart ;
    pproperty->u16fbw = pproperty->u16w - info.u16DRAMJumpLen/2;
}

//******************************************************************************
//  [Doxygen]
/// Set DWIN property
/// @param pproperty \b IN buffer to store DWIN property
//******************************************************************************
void MDrv_GOP_DWIN_SetWinProperty(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DwinProperty *pproperty)
{
    DRV_GOP_DWIN_INFO info;

    info.u16HPixelStart = pproperty->u16x;// + PANEL_H_DE;
    info.u16HPixelEnd = info.u16HPixelStart + pproperty->u16w ;
    info.u16VPixelStart = (pproperty->u16y == 0) ? (pproperty->u16y+1) : pproperty->u16y;// + PANEL_H_DE;
    info.u16VPixelEnd = info.u16VPixelStart + pproperty->u16h;// - 1;
    info.u64TFDRAMAddr = pproperty->u64fbaddr0;
    info.u64BFDRAMAddr = pproperty->u64fbaddr1;
    info.u16DRAMJumpLen = (pproperty->u16fbw - pproperty->u16w) * 2;  // 1 pixel occupy 2 bytes
    MDrv_GOP_DWIN_SetWinInfo(pGOPCtx, &info);
}

MS_BOOL MDrv_GOP_SwitchGWIN2Addr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gWinId, MS_PHY u64MainAddress, MS_PHY u64SubAddress, MS_U32 u32WaitTagID,  MS_U32 *pU32QueueCnt)
{
    MS_U32 u32GwinIdx=0;
    MS_U8 u8GOP;

    if (FALSE== _IsGwinIdValid(pGOPCtx, gWinId))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, gWinId);
        return false;
    }

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,gWinId);
    switch(u8GOP)
    {
        case E_GOP0:
            u32GwinIdx=gWinId;
            break;
        case E_GOP1:
            u32GwinIdx=gWinId-MAX_GOP0_GWIN;
            break;
        case E_GOP2:
            u32GwinIdx=gWinId-GOP2_Gwin0Id;
            break;
        case E_GOP3:
            u32GwinIdx=gWinId-GOP3_Gwin0Id;
            break;
        case E_GOP4:
            u32GwinIdx=gWinId-GOP4_Gwin0Id;
            break;
        case E_GOP5:
            u32GwinIdx=gWinId-GOP5_Gwin0Id;
            break;
        default:
            break;
    }

    return MDrv_GFLIP_SetFlipInfo(pGOPCtx, (MS_U32)MDrv_DumpGopByGwinId(pGOPCtx,gWinId), u32GwinIdx, u64MainAddress, u64SubAddress, u32WaitTagID, pU32QueueCnt);
}

MS_BOOL MDrv_GOP_GWIN_SendGwinInfo2Flip(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo)
{
    MS_U8 u8GOP,u8GwinIdx=0;


    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return false;
    }
    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support gop id:%d in this chip version",__FUNCTION__, u8GOP);
        return GOP_FAIL;
    }

    switch(u8GOP)
    {
        case E_GOP0:
            u8GwinIdx=u8win;
            break;
        case E_GOP1:
            u8GwinIdx=u8win-MAX_GOP0_GWIN;
            break;
        case E_GOP2:
            u8GwinIdx=u8win-GOP2_Gwin0Id;
            break;
        case E_GOP3:
            u8GwinIdx=u8win-GOP3_Gwin0Id;
            break;
        case E_GOP4:
            u8GwinIdx=u8win-GOP4_Gwin0Id;
            break;
        case E_GOP5:
            u8GwinIdx=u8win-GOP5_Gwin0Id;
            break;
        default:
            break;
    }

    return MDrv_GFLIP_SetGwinInfo(pGOPCtx,u8GOP,u8GwinIdx,pinfo);
}


void MDrv_GOP_GWIN_UpdateReg(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);
}

void MDrv_GOP_GWIN_UpdateRegWithSync(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bSync)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    GOP_GWIN_TriggerRegWriteIn(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP, MDrv_GOP_GWIN_IsForceWrite(pGOPCtx, u8GOP), bSync);
}

void MDrv_GOP_TriggerRegWriteIn(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bForceWriteIn, MS_BOOL bSync)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    GOP_GWIN_TriggerRegWriteIn(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP, bForceWriteIn, bSync);
}

void MDrv_GOP_GWIN_UpdateRegWithMaskSync(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u16GopMask, MS_BOOL bSync)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    MS_U16 u16GopAck=0;
    MS_U32 goptimeout = 0;
    MS_U16 u16GopIdx = 0;
#ifdef GOP_CMDQ_ENABLE
    MS_BOOL bGOPMIUSelect = FALSE;
    MS_BOOL bGOPAFBCMIUSelect = FALSE;
#endif
#ifdef CONFIG_MSTAR_SRAMPD
    MS_U8 u8GOP=0;
    MS_U32 u32BankOffSet =0;
#endif

    for (u16GopIdx = 0; u16GopIdx < MAX_GOP_SUPPORT; u16GopIdx++)
    {
        if ((u16GopMask | (1<<u16GopIdx))!=0)
        {
            break;
        }
    }
    if (u16GopIdx == MAX_GOP_SUPPORT)
    {
        return;
    }
#ifdef GOP_CMDQ_ENABLE
    for (u16GopIdx = 0; u16GopIdx < MAX_GOP_SUPPORT; u16GopIdx++)
    {
        if ((u16GopMask | (1<<u16GopIdx))!=0)
        {
            if(bMIUSelect[u16GopIdx] == TRUE)
            {
                bGOPMIUSelect = TRUE;
            }
            if(bAFBCMIUSelect[u16GopIdx] == TRUE)
            {
                bGOPAFBCMIUSelect = TRUE;
            }
        }
    }
#endif
    if(pGOPDrvLocalCtx->gop_gwin_frwr)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, GOP_VAL_FWR, GOP_REG_HW_MASK);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);

        if(pGOPDrvLocalCtx->apiCtxLocal.pGopChipProperty->bAFBC_Merge_GOP_Trig ==FALSE)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_TRIGGER, 0, GOP_BIT0);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_TRIGGER, GOP_BIT0, GOP_BIT0);
        }
    }
    else
    {
#ifdef GOP_CMDQ_ENABLE
        if(bGOPMIUSelect == TRUE || bGOPAFBCMIUSelect == TRUE || bAFBCTrigger==TRUE)
        {
            HAL_GOP_CMDQ_SetGOPACKMask(&pGOPDrvLocalCtx->halCtxLocal, u16GopMask);
            bAFBCTrigger= FALSE;
            return;
        }
        else
#endif
        {
            HAL_GOP_SetGOPACKMask(&pGOPDrvLocalCtx->halCtxLocal, u16GopMask);
        }

        if(bSync == TRUE)
        {
            MS_U32 u32DelayTimems = 0;
            MS_U32 u32TimeoutCnt = 0;
            _GetGOPAckDelayTimeAndCnt(&u32DelayTimems, &u32TimeoutCnt);
            do
            {
                goptimeout++;
                u16GopAck = HAL_GOP_GetGOPACK(&pGOPDrvLocalCtx->halCtxLocal,u16GopIdx);
                if (0 != u32DelayTimems)
                {
                    MsOS_DelayTask(u32DelayTimems); //delay 1 ms
                }
            } while((!u16GopAck) &&(goptimeout <= u32TimeoutCnt) );

            // Perform force write if wr timeout.
            if(goptimeout > u32TimeoutCnt)
            {
                //printf("Perform fwr if wr timeout!!\n");
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, GOP_VAL_FWR, GOP_REG_HW_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);
            }
#ifdef CONFIG_MSTAR_SRAMPD
    for(u8GOP=0;u8GOP<MAX_GOP_SUPPORT;u8GOP++)
    {
        if((u16GopMask&(GOP_BIT0<<u8GOP))&& (pGOPCtx->pGOPCtxShared->bGOPPowerDown[u8GOP]==TRUE))
        {
            _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT11, GOP_BIT11);
        }
    }
#endif
        }
    }
}

void MDrv_GOP_Restore_Ctx(MS_GOP_CTX_LOCAL*pGOPCtx)
{

}

MS_U8 MDrv_GOP_GetWordUnit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum)
{
    MS_U16 u16GOP_Unit=0;

    if(pGOPCtx->pGOPCtxShared->bPixelMode[u8GOPNum])
    {
        u16GOP_Unit = 1;
    }
    else
    {
        u16GOP_Unit = GOP_WordUnit;
    }
    return u16GOP_Unit;
}

GOP_Result MDrv_GOP_SetClock(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_GOP_SetClock(&pGOPDrvLocalCtx->halCtxLocal,bEnable);
}
MS_U32 MDrv_GOP_GetMIULen(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    return GOP_MIU0_LENGTH;
}


void MDrv_GOP_SetGOPBWStrength(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, EN_GOP_BURST_LEN eGOPBWStr)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet =0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    switch (eGOPBWStr)
    {
        case E_GOP_BURSTLEN_LONG:
            if(u8GOP != 0xC)
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BW, GOP_FIFO_BURST_ALL, GOP_FIFO_BURST_MASK );  //set GOP DMA Burst length to "all"
            else
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_BW, (GOP_BIT2|GOP_BIT1|GOP_BIT0), (GOP_BIT2|GOP_BIT1|GOP_BIT0) );  //set GOP DMA Burst length to "all"
            break;

        case E_GOP_BURSTLEN_MIDDLE:
            if(u8GOP != 0xC)
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BW, GOP_FIFO_BURST_MIDDLE, GOP_FIFO_BURST_MASK );  //set GOP DMA Burst length to "32"
            else
                 HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_BW, (GOP_BIT2|GOP_BIT0), (GOP_BIT2|GOP_BIT1|GOP_BIT0) );  //set GOP DMA Burst length to "32"
            break;

        case E_GOP_BURSTLEN_SHORT:
            if(u8GOP != 0xC)
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BW, GOP_FIFO_BURST_SHORT, GOP_FIFO_BURST_MASK);  //set GOP DMA Burst length to "4"
            else
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_BW, GOP_BIT1, (GOP_BIT2|GOP_BIT1|GOP_BIT0));  //set GOP DMA Burst length to "4"
            break;
        default:
            GOP_ASSERT(0);
            break;
    }
}


EN_GOP_BURST_LEN MDrv_GOP_GetGOPBWStrength(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    MS_U16 u16RegVal =0;
    MS_U32 u32BankOffSet = 0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL1, &u16RegVal);
    u16RegVal &= (GOP_BIT3|GOP_BIT2);
    u16RegVal >>= 2;

    switch (u16RegVal)
    {
        case 1:
        case 0:
            return E_GOP_BURSTLEN_SHORT;
        case 3:
            return E_GOP_BURSTLEN_LONG;
        case 2:
        default:
            return E_GOP_BURSTLEN_MIDDLE;
    }

}
#ifdef CONFIG_GOP_GWIN_MISC
void MDrv_GOP_SetGOPYUV(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_DW_ALPHA, GOP_BIT6, GOP_BIT6);
}
#endif
//-------------------------------------------------------------------------------------------------
/// Get GOP Current Hsync Pipeline Delay Offset
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_GOP_GetCurrentHSPipe(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 u16HSPD = 0;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_HS_PIPE, &u16HSPD);
#ifdef XC_FSC_FRC_PATCH
    MS_U16 XC_X_Offset = 0, XC_Y_Offset = 0;
    MDrv_SC_Get_GOPOffset(&XC_X_Offset, &XC_Y_Offset);
    u16HSPD -= XC_X_Offset;
#endif
    return u16HSPD;
}

//-------------------------------------------------------------------------------------------------
/// Set GOP Hsync Pipeline Delay Offset
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_GOP_GetHPipeOfst(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP_num, DRV_GOPDstType GopDst)
{
    MS_U8 u8Gop=0, i=0;
    MS_U8 u8Offset=0;
    MS_BOOL bHDREnable=FALSE;
    MS_U8 u8mux_sel=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (!_IsGopNumVaild(pGOPCtx, u8GOP_num))
    {
        GOP_D_ERR("\n[%s] not support gop id:%d in this chip version",__FUNCTION__, u8GOP_num);
        return GOP_FAIL;
    }

    switch(GopDst)
    {
      case E_DRV_GOP_DST_FRC:
      case E_DRV_GOP_DST_OP1:
      case E_DRV_GOP_DST_VE:
            break;
      case E_DRV_GOP_DST_BYPASS:
        for (i=E_GOP_FRC_MUX0; i<=E_GOP_FRC_MUX3; i++)
        {
            MDrv_GOP_GWIN_GetMux(pGOPCtx, &u8Gop, (Gop_MuxSel)i);
            if (u8Gop==u8GOP_num)
            {
                u8Offset = i/E_GOP_FRC_MUX2* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
                break;
            }
        }
        break;

      case E_DRV_GOP_DST_OP_DUAL_RATE:
        for (i=E_GOP_DUALRATE_OP_MUX0; i<=E_GOP_DUALRATE_OP_MUX2; i++)
        {
            MDrv_GOP_GWIN_GetMux(pGOPCtx, &u8Gop, (Gop_MuxSel)i);
            if (u8Gop==u8GOP_num)
            {
                u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_DualRateMux_Offset[(i-E_GOP_DUALRATE_OP_MUX0)];
                break;
            }
        }
        break;

      case E_DRV_GOP_DST_OP0:
      {
          MDrv_GOP_IsHDREnabled(pGOPCtx, &bHDREnable);
          if(bHDREnable==FALSE)
          {
              for (i=0; i<MAX_GOP_MUX; i++)
              {
                  MDrv_GOP_GWIN_GetMux(pGOPCtx, &u8Gop, (Gop_MuxSel)i);
                  if (u8Gop==u8GOP_num)
                  {
#ifdef GOP_ZORDER_PD_PATCH
                      MS_U16 u16tmp =0;
                      HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GOP_INTERNAL_ZORDER, &u16tmp);
                      //check OSD layer
                      if((u16tmp & 0xF) != 0)
                      {
                          u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Zorder_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
                      }
                      else
                      {
                          u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta + GOP_ZORDER_LAYER0_MUX_OFFSET;
                      }
#else
                      u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
#endif
                      break;
                  }
              }
          }
          else
          {
              for (i=0; i<MAX_GOP_MUX; i++)
              {
                  MDrv_GOP_GWIN_GetMux(pGOPCtx, &u8mux_sel, (Gop_MuxSel)i);
                  if (u8mux_sel==GOP_MIXER_MUX)
                  {
#ifdef GOP_ZORDER_PD_PATCH
                      MS_U16 u16tmp =0;
                      HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GOP_INTERNAL_ZORDER, &u16tmp);
                      //check OSD layer
                      if((u16tmp & 0xF) != 0)
                      {
                          u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Zorder_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
                      }
                      else
                      {
                          u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta + GOP_ZORDER_LAYER0_MUX_OFFSET;
                      }
#else
                      u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
#endif
                      break;
                  }
              }
          }
          break;
      }
      default:
        for (i=0; i<MAX_GOP_MUX; i++)
        {
            MDrv_GOP_GWIN_GetMux(pGOPCtx, &u8Gop, (Gop_MuxSel)i);
            if (u8Gop==u8GOP_num)
            {
#ifdef GOP_ZORDER_PD_PATCH
                MS_U16 u16tmp =0;
                HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GOP_INTERNAL_ZORDER, &u16tmp);
                //check OSD layer
                if((u16tmp & 0xF) != 0)
                {
                    u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Zorder_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
                }
                else
                {
                    u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta + GOP_ZORDER_LAYER0_MUX_OFFSET;
                }
#else
                u8Offset = pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
#endif
                break;
            }
        }
            break;
   }

    if ((!pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bGop2VStretch) && u8GOP_num==2)
    {
        /*if gop2 no v stretch, gop2 will have 2 level HSPD*/
        u8Offset+=5;
    }
    return u8Offset;
}

MS_U8 MDrv_GOP_GetDWINMIU(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if (HAL_GOP_GetDWINMIU(&pGOPDrvLocalCtx->halCtxLocal)==MIU_1)
    {
        return MIU_1;
    }
    else
    {
        return MIU_0;
    }
}

GOP_Result MDrv_GOP_SetDWINMIU(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 miu)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return (GOP_Result)HAL_GOP_SetDWINMIU(&pGOPDrvLocalCtx->halCtxLocal,miu);
}

#ifdef CONFIG_GOP_VE_CAPTURE
GOP_Result MDrv_GOP_VE_SetOutputTiming(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32mode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(u32mode >= E_GOP_VE_TIMEING_MAX)
        return GOP_INVALID_PARAMETERS;

    HAL_GOP_VE_SetOutputTiming(&pGOPDrvLocalCtx->halCtxLocal, u32mode);
    return GOP_SUCCESS;
}
#endif

GOP_Result MDrv_GOP_MIXER_SetOutputTiming(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32mode, GOP_DRV_MixerTiming *pTM)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(u32mode >= E_GOP_MIXER_TIMEING_MAX)
        return GOP_INVALID_PARAMETERS;

    HAL_GOP_MIXER_SetOutputTiming(&pGOPDrvLocalCtx->halCtxLocal, u32mode, pTM);
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_MIXER_EnableVfilter(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    HAL_GOP_MIXER_EnableVfilter(&pGOPDrvLocalCtx->halCtxLocal, bEnable);
    return GOP_SUCCESS;
}

MS_BOOL MDrv_GOP_RestoreFromVsyncLimitation(MS_GOP_CTX_LOCAL *pGOPCtx)
{
    return MDrv_GFLIP_RestoreFromVsyncLimitation(pGOPCtx);
}

MS_U8 MDrv_GOP_SelGwinIdByGOP(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U8 u8GWinIdx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    return HAL_GOP_SelGwinIdByGOP(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, u8GWinIdx);
}

GOP_Result MDrv_GOP_SetGOPBrightness(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 u16BriVal,MS_BOOL bMSB)
{
    MS_U32 u32BankOffSet =0;
    MS_U16 u16RegVal =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    u16RegVal = (bMSB<<8) | u16BriVal;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BRI, u16RegVal, 0x1FF );

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);

    return GOP_SUCCESS;
}
#ifdef CONFIG_GOP_GWIN_MISC
GOP_Result MDrv_GOP_EnableLBCouple(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support gop%d in this chip version!!", __FUNCTION__, u8GOP);
        return GOP_FAIL;
    }

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_NEW_BW, bEnable << 15, GOP_BIT15);

    return GOP_SUCCESS;
}
#endif
GOP_Result MDrv_GOP_GetGOPBrightness(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP,MS_U16* u16BriVal,MS_BOOL* bMSB)
{
    MS_U32 u32BankOffSet =0;
    MS_U16 u16RegVal =0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BRI, &u16RegVal);

    *bMSB = (MS_BOOL)((u16RegVal&0x100)>>8);
    *u16BriVal = u16RegVal & 0xFF;

    return GOP_SUCCESS;
}
GOP_Result MDrv_GOP_GWIN_EnableTileMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8win, MS_BOOL bEnable, E_GOP_TILE_DATA_TYPE tilemode)
{
    MS_U8 u8GOP;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    if (pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bhastilemode)// Currently, T8,Janus support this function
    {
        if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
        {
            GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
            return GOP_FAIL;
        }
        GOP_D_INFO("[%s](gId=%d) == %d\n",__FUNCTION__, u8win, bEnable);

        u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
/*
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_FADE(u8win), \
        tilemode?GOP_BIT7:~GOP_BIT7, GOP_BIT7);

        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_GWIN0_CTRL(u8win), \
        bEnable?GOP_BIT15:~GOP_BIT15, GOP_BIT15);
*/

        u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);
        if(u8GOP > MAX_GOP_SUPPORT)
        {
            GOP_D_ERR("\n[%s] not support gop num:%d in this chip version",__FUNCTION__, u8GOP);
            return GOP_FAIL;
        }

        if(tilemode == E_DRV_GOP_TILE_DATA_32BPP)
        {
            pGOPDrvLocalCtx->b32BPPTileMode[u8GOP] = bEnable;
            if(bEnable)
                 pGOPDrvLocalCtx->b16BPPTileMode[u8GOP] = FALSE;
        }
        else
        {
            pGOPDrvLocalCtx->b16BPPTileMode[u8GOP] = bEnable;
            if(bEnable)
                  pGOPDrvLocalCtx->b32BPPTileMode[u8GOP] =FALSE;
        }

        HAL_GOP_GWIN_EnableTileMode(&pGOPDrvLocalCtx->halCtxLocal, u8win, bEnable, tilemode);
        //GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));

        return GOP_SUCCESS;
    }
    else
    {
        return GOP_FUN_NOT_SUPPORTED;
    }

}

void MDrv_GOP_GWIN_Load_HStretchModeTable(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP,DRV_GOPStrchHMode HStrchMode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    switch(HStrchMode)
    {
        case E_DRV_GOP_HSTRCH_6TAPE:
        case E_DRV_GOP_HSTRCH_6TAPE_LINEAR:
        case E_DRV_GOP_HSTRCH_6TAPE_NEAREST:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN0:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN1:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN2:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN3:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN4:
        case E_DRV_GOP_HSTRCH_6TAPE_GAIN5:
        case E_DRV_GOP_HSTRCH_2TAPE:
            _GOP_Load_Stretch_Mode_Table(pGOPDrvLocalCtx,u8GOP,_GopHStretchTable[HStrchMode]);
            break;
        case E_DRV_GOP_HSTRCH_DUPLICATE:
        case E_DRV_GOP_HSTRCH_4TAPE:
        default:
            break;

    }


}

void MDrv_GOP_GWIN_Load_VStretchModeTable(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchVMode VStrchMode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    switch(VStrchMode)
    {
        case E_DRV_GOP_VSTRCH_LINEAR:
            _GOP_Load_VStretch_Mode_Table(pGOPDrvLocalCtx,u8GOP,_GopVStretchTable[VStrchMode]);
            break;
        case E_DRV_GOP_VSTRCH_LINEAR_GAIN0:
        case E_DRV_GOP_VSTRCH_LINEAR_GAIN1:
            _GOP_Load_VStretch_Mode_Table(pGOPDrvLocalCtx,u8GOP,_GopVStretchTable[VStrchMode]);
            break;
        case E_DRV_GOP_VSTRCH_DUPLICATE:
        case E_DRV_GOP_VSTRCH_NEAREST:
            break;
        default:
            break;

    }
}
GOP_Result MDrv_GOP_Set_PINPON(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn, E_DRV_GOP_PINPON_MODE pinpon_mode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Set_PINPON(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum, bEn, pinpon_mode);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_Set_BufferPINPON(MS_GOP_CTX_LOCAL*pGOPCtx,MS_PHY u64fbaddr0,MS_PHY u64fbaddr1)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_PHY u64tmp;
    MS_U8 u8MiuSel;

    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64tmp, u64fbaddr0);
    u64tmp /= GOP_WordUnit;      // word(8-byte)
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_DW_UB_L, u64tmp);

    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64tmp, u64fbaddr1);
    u64tmp /= GOP_WordUnit;      // word(8-byte)
    HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal,GOP_DW_UB_H, u64tmp);
    return GOP_SUCCESS;

}

GOP_Result MDrv_GOP_Set_16bBus(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet =0;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    //pGOPDrvLocalCtx->b16BPPTileMode = bEn;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CTRL1, bEn<<4, GOP_BIT4);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_OLDADDR, bEn<<3, GOP_BIT3);

    return GOP_SUCCESS;

}

GOP_Result MDrv_GOP_GWIN_SetGPUTileMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, EN_DRV_GOP_GPU_TILE_MODE mode)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_GWIN_SetGPUTileMode(&pGOPDrvLocalCtx->halCtxLocal, u8win, (EN_DRV_GOP_GPU_TILE_MODE)mode);
    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));
    return GOP_SUCCESS;
}

#ifdef CONFIG_GOP_YUV_SWAP

GOP_Result MDrv_GOP_SetUVSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_SetUVSwap(&pGOPDrvLocalCtx->halCtxLocal,u8GOPNum,bEn);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_SetYCSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_SetYCSwap(&pGOPDrvLocalCtx->halCtxLocal,u8GOPNum,bEn);

    return GOP_SUCCESS;
}
#endif
//-------------------------------------------------------------------------------------------------
/// Set GWIN New Alpha Mode
/// @param u8Wid    \b IN: GWin ID
/// @param bTrue    \b IN: TRUE: enable new alpha mode, FALSE: disable new alpha mode
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_GOP_GWIN_MISC
GOP_Result MDrv_GOP_GWIN_GetGwinNewAlphaModeEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL* pEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_GWIN_GetNewAlphaMode(&pGOPDrvLocalCtx->halCtxLocal, u8win, pEnable);
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_SetNewAlphaMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_GWIN_SetNewAlphaMode(&pGOPDrvLocalCtx->halCtxLocal, u8win, bEnable);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)MDrv_DumpGopByGwinId(pGOPCtx,u8win));

    return GOP_SUCCESS;
}
#endif

#ifdef CONFIG_GOP_CONTRAST
GOP_Result MDrv_GOP_SetGOPContrast(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP
    , MS_U16 u16YContrast, MS_U16 u16UContrast, MS_U16 u16VContrast )
{
    MS_U32 u32BankOffSet =0;
    MS_U16 u16YMask = 0x7E00;
    MS_U16 u16UMask = 0x003F;
    MS_U16 u16VMask = 0x3F00;
    GOP_CTX_DRV_LOCAL* pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if ( ((u16YContrast) > 0x3F) | ((u16UContrast) > 0x3F) | ((u16VContrast) > 0x3F) ) // Y, U, V = 6, 6, 6 bits each
    {
        return GOP_FAIL ;
    }

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    // if gain = 1, we set 16, which is << (#+4)
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BRI, (u16YContrast) << 9, u16YMask);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CON
        , (u16UContrast)|((u16VContrast) << 8), u16UMask|u16VMask);
    //HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CON, u16VContrast << (8+4), u16VMask);

    GOP_GWIN_UpdateReg(pGOPDrvLocalCtx, (E_GOP_TYPE)u8GOP);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GetGOPContrast(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP
    , MS_U32* u32YContrast, MS_U32* u32UContrast, MS_U32* u32VContrast )
{
    MS_U32 u32BankOffSet =0;
    MS_U16 u16RegVal =0;
    MS_U16 u16YMask = 0x7E00; // by spec
    MS_U16 u16UMask = 0x003F; // by spec
    MS_U16 u16VMask = 0x3F00; // by spec
    GOP_CTX_DRV_LOCAL* pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BRI, &u16RegVal);
    *u32YContrast= (u16RegVal & u16YMask) >> 9; // y starts at bit 9

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CON, &u16RegVal);
    *u32UContrast= (u16RegVal & u16UMask); // u starts at bit 0

    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_CON, &u16RegVal);
    *u32VContrast= (u16RegVal & u16VMask) >> 8; // u starts at bit 8

    return GOP_SUCCESS;
}
#endif
#ifdef CONFIG_GOP_VE_CAPTURE
MS_BOOL MDrv_GOP_GetVECaptureState(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    return MDrv_GFLIP_GetVECaptureState(pGOPCtx, pstVECapState);
}

MS_BOOL MDrv_GOP_VECaptureWaitOnFrame(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    return MDrv_GFLIP_VECaptureWaitOnFrame(pGOPCtx, pstVECapState);
}

MS_BOOL MDrv_GOP_EnaVECapture(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    return MDrv_GFLIP_EnaVECapture(pGOPCtx, pstVECapState);
}
#endif
GOP_Result MDrv_GOP_Set3D_LR_FrameExchange(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP)
{
    MS_U32 u32BankOffSet =0, u32MainBase=0, u32SubBase=0;
    MS_U16 u16Tmp=0;
    MS_U8 u8Win = 0, u8MaxWinNum=0;
    GOP_CTX_DRV_LOCAL* pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType == E_DRV_3D_NONE)
    {
        return GOP_FUN_NOT_SUPPORTED;
    }

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType != E_DRV_3D_NONE)
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
        u8MaxWinNum = HAL_GOP_GetMaxGwinNumByGOP(&pGOPDrvLocalCtx->halCtxLocal, u8GOP);

        for(u8Win=0; u8Win<u8MaxWinNum; u8Win++)
        {
            //Read back main/sub base registers
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_L(u8Win), (MS_U16*)&u16Tmp);
            u32MainBase = ((MS_U32)u16Tmp) & GOP_REG_WORD_MASK;
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_H(u8Win), (MS_U16*)&u16Tmp);
            u32MainBase += (((MS_U32)u16Tmp) << 16);

            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_3DOSD_SUB_RBLK_L(u8Win), (MS_U16*)&u16Tmp);
            u32SubBase = ((MS_U32)u16Tmp) & GOP_REG_WORD_MASK;
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_3DOSD_SUB_RBLK_H(u8Win), (MS_U16*)&u16Tmp);
            u32SubBase += (((MS_U32)u16Tmp) << 16);
            //Swap main/sub base register value
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_3DOSD_SUB_RBLK_L(u8Win), u32MainBase);
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_L(u8Win), u32SubBase);
        }
    }
    pGOPCtx->pGOPCtxShared->bGOP3DLRSwitch[u8GOP] = !pGOPCtx->pGOPCtxShared->bGOP3DLRSwitch[u8GOP];  //Invert L/R swap flag
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_Set3DOSD_Mode(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP ,EN_DRV_GOP_3D_MODE en3DOSDMode)
{
    MS_U32 u32BankOffSet = 0;
    MS_U16 u16GOPScalingDownEnable;
    GOP_Result enGOPRet = GOP_SUCCESS;

    GOP_CTX_DRV_LOCAL* pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->enGOP3DType == E_DRV_3D_NONE)
    {
        return GOP_FUN_NOT_SUPPORTED;
    }

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    pGOPCtx->pGOPCtxShared->GOP_StereoMode[u8GOP] = en3DOSDMode;

    // 4K120Hz only support frame alternative / line alternative;
    switch(en3DOSDMode)
    {
        case E_DRV_GOP_3D_SWITH_BY_FRAME:
            //Frame by Frame output, default enable GOP 3D invert to match with scaler
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, (((MS_U16)en3DOSDMode)<<1)|GOP_BIT0, GOP_BIT3|GOP_BIT2|GOP_BIT1|GOP_BIT0);
            HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, TRUE);
            break;
        case E_DRV_GOP_3D_LINE_FRAMEPACKING:
            if (pGOPDrvLocalCtx->halCtxLocal.pHALShared->GOP_Dst[u8GOP] != E_DRV_GOP_DST_OP_DUAL_RATE)
            {
            	en3DOSDMode = E_DRV_GOP_3D_TOP_BOTTOM;
            	HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ((MS_U16)en3DOSDMode)<<1, GOP_BIT3|GOP_BIT2|GOP_BIT1);
            	HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, TRUE);
            }
            else
            {
                enGOPRet = GOP_FAIL;
            }
            break;
        case E_DRV_GOP_3D_SIDE_BY_SYDE:
            if (pGOPDrvLocalCtx->halCtxLocal.pHALShared->GOP_Dst[u8GOP] != E_DRV_GOP_DST_OP_DUAL_RATE)
            {
                HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, &u16GOPScalingDownEnable);
                if( u16GOPScalingDownEnable & (GOP_BIT0|GOP_BIT4))
                {
                    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, GOP_BIT5 ,GOP_BIT5);
                }
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ((MS_U16)en3DOSDMode)<<1, GOP_BIT3|GOP_BIT2|GOP_BIT1);
                HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, FALSE);
            }
            else
            {
                enGOPRet = GOP_FAIL;
            }
            break;
        case E_DRV_GOP_3D_LINE_ALTERNATIVE:
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, &u16GOPScalingDownEnable);
            if( u16GOPScalingDownEnable & (GOP_BIT0|GOP_BIT4))
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_SCALING_CFG, GOP_BIT5 ,GOP_BIT5);
            }
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ((MS_U16)en3DOSDMode)<<1, GOP_BIT3|GOP_BIT2|GOP_BIT1);
            HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, FALSE);
            break;
        case E_DRV_GOP_3D_TOP_BOTTOM:
            if (pGOPDrvLocalCtx->halCtxLocal.pHALShared->GOP_Dst[u8GOP] != E_DRV_GOP_DST_OP_DUAL_RATE)
            {
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ((MS_U16)en3DOSDMode)<<1, GOP_BIT3|GOP_BIT2|GOP_BIT1);
                HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, TRUE);
            }
            else
            {
                enGOPRet = GOP_FAIL;
            }
            break;
        case E_DRV_GOP_3D_DISABLE:
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ((MS_U16)en3DOSDMode)<<1, GOP_BIT3|GOP_BIT2|GOP_BIT1);
            HAL_GOP_EnableTwoLineBufferMode(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, TRUE);
            break;
        default:
            GOP_D_ERR("\n[%s] not support 3D mode:%d in this chip version",__FUNCTION__, en3DOSDMode);
            enGOPRet = GOP_INVALID_PARAMETERS;
            break;
    }

    return enGOPRet;
}

GOP_Result MDrv_GOP_Set3DOSD_Sub(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8Gwin, MS_PHY u64SubAddr)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8GOP;
    MS_U8 u8MiuSel=0xFF;
    UNUSED(u8MiuSel);

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8Gwin);
    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64SubAddr, u64SubAddr);
    HAL_GOP_GWiN_Set3DOSD_Sub(&pGOPDrvLocalCtx->halCtxLocal,u8GOP ,u8Gwin,u64SubAddr);

    return GOP_SUCCESS;

}


GOP_Result MDrv_GOP_GWIN_ClearFlipQueue(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8win)
{

    MS_U32 u32GwinIdx=0;
    MS_U8 u8GOP;

    if (FALSE== _IsGwinIdValid(pGOPCtx, u8win))
    {
        GOP_D_ERR("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, u8win);
        return GOP_FAIL;
    }

    u8GOP = MDrv_DumpGopByGwinId(pGOPCtx,u8win);

    if (FALSE== _IsGopNumVaild(pGOPCtx, u8GOP))
    {
        GOP_D_ERR("\n[%s] not support GOPNum:%d in this chip version",__FUNCTION__, u8GOP);
        return GOP_FAIL;
    }

    switch(u8GOP)
    {
        case E_GOP0:
            u32GwinIdx=u8win;
            break;
        case E_GOP1:
            u32GwinIdx=u8win-MAX_GOP0_GWIN;
            break;
        case E_GOP2:
            u32GwinIdx=u8win-GOP2_Gwin0Id;
            break;
        case E_GOP3:
            u32GwinIdx=u8win-GOP3_Gwin0Id;
            break;
        case E_GOP4:
            u32GwinIdx=u8win-GOP4_Gwin0Id;
            break;
        case E_GOP5:
            u32GwinIdx=u8win-GOP5_Gwin0Id;
            break;
        default:
            break;
    }
    MDrv_GFLIP_ClearFlipQueue(pGOPCtx,u8GOP,u32GwinIdx);
    return GOP_SUCCESS;
}

/********************************************************************************/
/// To Mask 1st Hsync
/// @param bMask \b IN
///   - # TRUE  Mask first Hsync when field is odd
///   - # FALSE UnMask first Hsync when field is odd
/// @internal please verify the register document and the code
/********************************************************************************/
void MDrv_GOP_Mask_First_Hsync(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bMask )
{
    MS_U16 regval;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if(bMask)
        regval = 0x4;
    else
        regval = 0;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_BOT_HS, regval, 0x4);
}

MS_U16 MDrv_GOP_GetBPP(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOPColorType fbFmt)
{
    return HAL_GOP_GetBPP(&(((GOP_CTX_DRV_LOCAL*)pGOPCtx)->halCtxLocal), fbFmt);
}

void Mdrv_GOP_GetDWinCapability(MS_GOP_CTX_LOCAL*pGOPCtx, PDRV_GOP_DWIN_CAP pDwinCap)
{
    pDwinCap->bSupportWindowDECapture = DWIN_SUPPORT_WINDOWDE_CAPTURE;
    pDwinCap->bSupportOSDCapture = DWIN_SUPPORT_OSD_CAPTURE;
}

//Return an array of mux, sorted by mux priority, the first is the highest
MS_BOOL Mdrv_GOP_GetMuxPriority(MS_GOP_CTX_LOCAL*pGOPCtx, PST_DRV_GOP_MUX_CAP_EX pstMuxPriEx)
{
    MS_U8 i=0,j=0;
    MS_U16 u16MuxTemp[2]={0,0};
    GOP_CTX_DRV_LOCAL *pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 au16Mux[MAX_GOP_MUX_OPNum][2] ={};

    if(  (pGOPDrvLocalCtx == NULL)
       ||(pstMuxPriEx == NULL))
    {
        return FALSE;
    }

    //u8Mux[x][0]=Mux number, u8Mux[x][1]=Mux's offset
    for(i=0; i<MAX_GOP_MUX_OPNum-1; i++)
    {
#ifdef GOP_ZORDER_PD_PATCH
        MS_U16 u16tmp =0;
        HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_4G_GOP_INTERNAL_ZORDER, &u16tmp);
        //check OSD layer
        if((u16tmp & 0xF) != 0)
        {
            au16Mux[i][0]= i;
            au16Mux[i][1]= pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Zorder_Mux_Offset[i]* pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
        }
        else
        {
            au16Mux[i][0]= i;
            au16Mux[i][1]= pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i] * pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta + GOP_ZORDER_LAYER0_MUX_OFFSET;
        }
#else
        au16Mux[i][0]= i;
        au16Mux[i][1]= pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_Mux_Offset[i] * pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->GOP_MUX_Delta;
#endif
    }

    //Use Mux offset to sort array of u8Mux[][]
    for(i=0; i<MAX_MUX_NUMBER_SUPPORT-1; i++)
    {
        for(j=0; j<MAX_MUX_NUMBER_SUPPORT-i-1; j++)
        {
            //Sort Descending by GOP_Mux Offset
            if(au16Mux[j][1] < au16Mux[j+1][1])
            {
                memcpy(u16MuxTemp, au16Mux[j+1], sizeof(au16Mux[0]));
                memcpy(au16Mux[j+1], au16Mux[j], sizeof(au16Mux[0]));
                memcpy(au16Mux[j], u16MuxTemp, sizeof(au16Mux[0]));
            }
        }
    }

    if(pstMuxPriEx->u8MuxNumber > MAX_MUX_NUMBER_SUPPORT)
    {
        pstMuxPriEx->u8MuxNumber = MAX_MUX_NUMBER_SUPPORT;
    }

    for(i=0; i<pstMuxPriEx->u8MuxNumber; i++)
    {
        if(i>=MAX_GOP_INFO_COUNT)
            return FALSE;
        pstMuxPriEx->pu8MuxProrityArray[i] = (MS_U8)(au16Mux[i][0]); //Store sorted mux number
    }
    return TRUE;
}
#ifdef CONFIG_GOP_GWIN_MISC
MS_BOOL MDrv_GOP_SetGPIO3DPin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GPIO3DPin)
{
    return MDrv_GFLIP_SetGPIO3DPin(pGOPCtx, u32GPIO3DPin);
}
#endif

E_GOP_VIDEOTIMING_MIRRORTYPE Mdrv_GOP_GetVideoTimingMirrorType(MS_GOP_CTX_LOCAL* pGOPCtx, MS_BOOL bHorizontal)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    return HAL_GOP_GetVideoTimingMirrorType(&pGOPDrvLocalCtx->halCtxLocal, bHorizontal);
}

/********************************************************************************/
/// Set GOP OC(OSD Compression)
/********************************************************************************/
GOP_Result MDrv_GOP_OC_SetOCEn(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bOCEn)
{
    GOP_Result ret;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    ret = HAL_GOP_OC_SetOCEn(&pGOPDrvLocalCtx->halCtxLocal, u8GOPNum,  bOCEn);

    return ret;

}

GOP_Result MDrv_GOP_OC_SetOCInfo(MS_GOP_CTX_LOCAL*pGOPCtx,DRV_GOP_OC_INFO* OCinfo)
{
    MS_U8 regmiu =0;
    MS_U8 u8MiuSel;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    //OC HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, OCinfo->u64GOP_OC_DRAM_RBLK, OCinfo->u64GOP_OC_DRAM_RBLK);
    HAL_GOP_OC_Get_MIU_Sel(&pGOPDrvLocalCtx->halCtxLocal, &regmiu);
    if( regmiu != u8MiuSel)
    {
        GOP_D_ERR("\n %s, Wrong MIU setting! Your address(0x%tx) is across MIU0, but OC MIU setting is still MIU:%d\n",__FUNCTION__,\
        (ptrdiff_t)OCinfo->u64GOP_OC_DRAM_RBLK , regmiu);
        return GOP_FAIL;
    }

    HAL_GOP_OC_SetOCInfo(&pGOPDrvLocalCtx->halCtxLocal, OCinfo);

    return GOP_SUCCESS;
}
/********************************************************************************/
///GOP Bank Backup/Restore
/********************************************************************************/
#ifdef CONFIG_GOP_TEST_PATTERN
GOP_Result MDrv_GOP_BNK(MS_GOP_CTX_LOCAL*pGOPCtx, E_DRV_GOP_BNK_STATUS bBnkOP)
{
    MS_U32 u32BankOffSet = 0x0;
    MS_U16 reg_val = 0x0;
    MS_U8 u8CurrentGOP;
    MS_U8 i;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    u8CurrentGOP = MDrv_GOP_Get(pGOPCtx);
    _GetBnkOfstByGop(u8CurrentGOP, &u32BankOffSet);

    if( bBnkOP == E_DRV_GOP_BNK_READ)
    {
        for(i =0; i<GOP_REG_MAX ; i++)
        {
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST,i), &reg_val);
            pGOPCtx->GOPBnk[u8CurrentGOP][i] = reg_val;
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST+1,i), &reg_val);
            pGOPCtx->GWINBnk[u8CurrentGOP][i] = reg_val;
            HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST+2,i), &reg_val);
            pGOPCtx->GOPStBnk[u8CurrentGOP][i] = reg_val;

        }
    }
    else
    {
        MS_U16 tmp;
        for(i =0; i<GOP_REG_MAX ; i++)
        {
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST,i),  pGOPCtx->GOPBnk[u8CurrentGOP][i], GOP_REG_WORD_MASK);

            if(i == 0)
            {
                tmp = pGOPCtx->GWINBnk[u8CurrentGOP][i] & 0xFFFE;
            }
            else
                tmp = pGOPCtx->GWINBnk[u8CurrentGOP][i];

            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST+1,i),  tmp, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST+2,i),  pGOPCtx->GOPStBnk[u8CurrentGOP][i], GOP_REG_WORD_MASK);
        }
        if((pGOPCtx->GWINBnk[u8CurrentGOP][0] & 0x1) == TRUE)  // if gwin is enable, enable it. otherwise, skip gwin enable.
            HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_REG(GOP_4G_OFST+1,i),  0x1, 0x1);

        MDrv_GOP_GWIN_UpdateReg(pGOPCtx, u8CurrentGOP);

    }

    return GOP_SUCCESS;
}

/********************************************************************************/
/// Set Test Pattern Type
/********************************************************************************/
GOP_Result MDrv_GOP_TestPattern_IsVaild(MS_GOP_CTX_LOCAL*pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    GOP_Result ret;
    MS_U8 u8CurrentGOP;

    u8CurrentGOP= MDrv_GOP_Get(pGOPCtx);
    ret = HAL_GOP_TestPattern_IsVaild(&pGOPDrvLocalCtx->halCtxLocal,u8CurrentGOP);
    if( ret == GOP_FUN_NOT_SUPPORTED)
    {
        GOP_D_ERR("\n[%s]    GOP%d not support Test Pattern\n",__FUNCTION__, u8CurrentGOP);
    }
    return ret;
}

GOP_Result MDrv_GOP_TestPatternAlpha_Enable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bTSTClr_Alpha_En)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_ALPHA_EN, (!bTSTClr_Alpha_En)<<SHIFT_TSTCLR_ALPHA_EN, MASK_TSTCLR_ALPHA_EN);
    }

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bTSTClr_En, MS_U8 u8TSTClr_Alpha)
{
    MS_U16 u16TSTClr_Alpha=0;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16TSTClr_Alpha = TESTPATTERN_CALC(u8TSTClr_Alpha);
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_ALPHA, (u16TSTClr_Alpha<<SHIFT_TSTCLR_ALPHA), MASK_TSTCLR_ALPHA);
    }

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_EN, bTSTClr_En<<SHIFT_TSTCLR_EN, MASK_TSTCLR_EN);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetStartClr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8StartR,MS_U8 u8StartG,MS_U8 u8StartB)
{
    MS_U16 u16StartR=u8StartR, u16StartG=u8StartG, u16StartB=u8StartB;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    u16StartR = u8StartR & MASK_RGB_STC_VALID;
    u16StartG = u8StartG & MASK_RGB_STC_VALID;
    u16StartB = u8StartB & MASK_RGB_STC_VALID;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16StartR = TESTPATTERN_CALC(u16StartR);
        u16StartG = TESTPATTERN_CALC(u16StartG);
        u16StartB = TESTPATTERN_CALC(u16StartB);
    }

    Merge(pGOPCtx, REG_R_STC, REG_G_STC, REG_B_STC,                                                                  \
            u16StartR<<SHIFT_R_STC, u16StartG<<SHIFT_G_STC,u16StartB<<SHIFT_B_STC,                                      \
            MASK_R_STC, MASK_G_STC, MASK_B_STC);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_ALPHA_EN, GOP_BIT0, MASK_INI_TSTCLR_EN);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetHInitColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HIniR,MS_U8 u8HIniG,MS_U8 u8HIniB)
{
    MS_U16 u16HIniR =u8HIniR, u16HIniG =u8HIniG, u16HIniB =u8HIniB;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16HIniR = TESTPATTERN_CALC(u8HIniR);
        u16HIniG = TESTPATTERN_CALC(u8HIniG);
        u16HIniB = TESTPATTERN_CALC(u8HIniB);
    }

    Merge(pGOPCtx, REG_HR_INC, REG_HG_INC, REG_HB_INC,                                                                  \
            u16HIniR<<SHIFT_HR_INC, u16HIniG<<SHIFT_HG_INC, u16HIniB<<SHIFT_HB_INC,                                     \
            MASK_HR_INC, MASK_HG_INC, MASK_HB_INC);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetVInitColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8VIniR,MS_U8 u8VIniG,MS_U8 u8VIniB)
{
    MS_U16 u16VIniR =u8VIniR, u16VIniG =u8VIniG, u16VIniB =u8VIniB;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16VIniR = TESTPATTERN_CALC(u8VIniR);
        u16VIniG = TESTPATTERN_CALC(u8VIniG);
        u16VIniB = TESTPATTERN_CALC(u8VIniB);
    }

    Merge(pGOPCtx, REG_VR_INC, REG_VG_INC, REG_VB_INC,                                                                  \
            u16VIniR<<SHIFT_VR_INC, u16VIniG<<SHIFT_VG_INC, u16VIniB<<SHIFT_VB_INC,                                        \
            MASK_VR_INC, MASK_VG_INC, MASK_VB_INC);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetHIncremental_Signz(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bHIncreSigR,MS_BOOL bHIncreSigG,MS_BOOL bHIncreSigB)
{
    Merge(pGOPCtx, REG_HR_INC, REG_HG_INC, REG_HB_INC,                                                                  \
            bHIncreSigR<<SHIFT_HR_INC_SIGNZ, bHIncreSigG<<SHIFT_HG_INC_SIGNZ, bHIncreSigB<<SHIFT_HB_INC_SIGNZ,          \
            MASK_HR_INC_SIGNZ, MASK_HG_INC_SIGNZ, MASK_HB_INC_SIGNZ);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetVIncremental_Signz(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bVIncreSigR,MS_BOOL bVIncreSigG,MS_BOOL bVIncreSigB)
{
    Merge(pGOPCtx, REG_VR_INC, REG_VG_INC, REG_VB_INC,                                                                  \
            bVIncreSigR<<SHIFT_VR_INC_SIGNZ, bVIncreSigG<<SHIFT_VG_INC_SIGNZ, bVIncreSigB<<SHIFT_VB_INC_SIGNZ,          \
            MASK_VR_INC_SIGNZ, MASK_VG_INC_SIGNZ, MASK_VB_INC_SIGNZ);

    return GOP_SUCCESS;
}


GOP_Result MDrv_GOP_TestPattern_SetHStep(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HStepR,MS_U8 u8HStepG,MS_U8 u8HStepB)
{
    MS_U16 u16HStepR =u8HStepR, u16HStepG =u8HStepG, u16HStepB =u8HStepB;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16HStepR = TESTPATTERN_CALC(u8HStepR);
        u16HStepG = TESTPATTERN_CALC(u8HStepG);
        u16HStepB = TESTPATTERN_CALC(u8HStepB);
    }

    Merge(pGOPCtx, REG_HR_STEP, REG_HG_STEP, REG_HB_STEP,                                                               \
            u16HStepR<<SHIFT_HR_STEP, u16HStepG<<SHIFT_HG_STEP, u16HStepB<<SHIFT_HB_STEP,                                  \
            MASK_HR_STEP, MASK_HG_STEP, MASK_HB_STEP);

    return GOP_SUCCESS;
}
GOP_Result MDrv_GOP_TestPattern_SetVStep(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8VStepR,MS_U8 u8VStepG,MS_U8 u8VStepB)
{
    MS_U16 u16VStepR =u8VStepR, u16VStepG =u8VStepG, u16VStepB =u8VStepB;

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPDrvLocalCtx->halCtxLocal.pGopChipPro->bTstPatternAlpha== TRUE)
    {
        u16VStepR = TESTPATTERN_CALC(u8VStepR);
        u16VStepG = TESTPATTERN_CALC(u8VStepG);
        u16VStepB = TESTPATTERN_CALC(u8VStepB);
    }

    Merge(pGOPCtx, REG_VR_STEP, REG_VG_STEP, REG_VB_STEP,                                                               \
            u16VStepR<<SHIFT_VR_STEP, u16VStepG<<SHIFT_VG_STEP, u16VStepB<<SHIFT_VB_STEP,                                  \
            MASK_VR_STEP, MASK_VG_STEP, MASK_VB_STEP);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_TestPattern_SetHVDuplicate(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HDup,MS_U8 u8VDup)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_HDUP, u8HDup<<SHIFT_TSTCLR_HDUP, MASK_TSTCLR_HDUP);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_TSTCLR_VDUP, u8VDup<<SHIFT_TSTCLR_VDUP, MASK_TSTCLR_VDUP);
    return GOP_SUCCESS;
}
#endif
MS_BOOL MDrv_GOP_GWIN_BeginDraw(void)
{
#if 0//GOP_LOCK_SUPPORT
    if (g_gopDrvCtxLocal.apiCtxLocal.s32GOPMutex >= 0)
    {
        DRV_GOP_ENTRY();
    }
    else
    {
        printf("%s: Error, no mutex to obtain\n", __FUNCTION__);
        return FALSE;
    }
#endif
    return TRUE;
}

MS_BOOL MDrv_GOP_GWIN_EndDraw(void)
{
#if 0//GOP_LOCK_SUPPORT
    if (g_gopDrvCtxLocal.apiCtxLocal.s32GOPMutex >= 0)
    {
        DRV_GOP_RETURN();
    }
    else
    {
        printf("%s: Error, no mutex to obtain\n", __FUNCTION__);
        return FALSE;
    }
#endif
    return TRUE;
}

GOP_Result MDrv_GOP_AFBC_Core_Reset(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP)
{
    MS_U8 u8Core=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPCtx->pGopChipProperty->bAFBC_Support[u8GOP] ==FALSE)
    {
        GOP_D_ERR("[%s] GOP AFBC mode not support GOP %d \n",__FUNCTION__, u8GOP);
        return GOP_FUN_NOT_SUPPORTED;
    }

    HAL_GOP_AFBC_GetCore(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, &u8Core);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_AFBC_MIU,GOP_BIT15, GOP_BIT15);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_AFBC_CORE_EN(u8Core),GOP_BIT8, GOP_BIT8);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_AFBC_MIU,0, GOP_BIT15);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_AFBC_CORE_EN(u8Core),0, GOP_BIT8);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_AFBC_Core_Enable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEna)
{
    MS_U8 u8Core=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(pGOPCtx->pGopChipProperty->bAFBC_Support[u8GOP] ==FALSE)
    {
        GOP_D_ERR("[%s] GOP AFBC mode not support GOP %d \n",__FUNCTION__, u8GOP);
        return GOP_FUN_NOT_SUPPORTED;
    }
    HAL_GOP_AFBC_GetCore(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, &u8Core);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,REG_AFBC_CORE_EN(u8Core),bEna, GOP_BIT0);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_AFBCMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL u8GOP, MS_BOOL bEnable, EN_DRV_GOP_AFBC_CNTL eCTL)
{
    MS_U32 u32BankOffSet=0;
    MS_U8 u8Core=0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    if(pGOPCtx->pGopChipProperty->bAFBC_Support[u8GOP] ==FALSE)
    {
        GOP_D_ERR("[%s] GOP AFBC mode not support GOP %d \n",__FUNCTION__, u8GOP);
        return GOP_FUN_NOT_SUPPORTED;
    }

    HAL_GOP_AFBC_GetCore(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, &u8Core);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_FMT(u8Core), (eCTL&E_DRV_GOP_AFBC_SPILT)? GOP_BIT6: 0 , GOP_BIT6);    		        //Spilt
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_FMT(u8Core), (eCTL&E_DRV_GOP_AFBC_YUV_TRANSFER)? GOP_BIT4: 0 , GOP_BIT4);            //YUV transfer
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_CORE_EN(u8Core), bEnable, GOP_BIT0);                         //CoreEn
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+ GOP_4G_YUV_SWAP, bEnable<<14, GOP_BIT14);              //GOP U/V swap
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+ GOP_4G_MIU_SEL, bEnable<<12, GOP_BIT12);  		        //GOP afbc en

    if(pGOPCtx->pGopChipProperty->bAFBC_Merge_GOP_Trig ==FALSE)
    {
        bAFBCTrigger=TRUE;
    }
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_AFBCSetWindow(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, DRV_GOP_AFBC_Info* pinfo, MS_BOOL bChangePitch)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U8 u8Core=0,alignfactor=0;
    MS_U16 u16Pitch=0,u16Height=0;

    u16Pitch = pinfo->u16Pitch/4;

    //16*16 block
    alignfactor =16;
    u16Pitch = (u16Pitch + alignfactor - 1) & (~(alignfactor - 1));
    u16Height= pinfo->u16VPixelEnd-pinfo->u16VPixelStart;
    u16Height = (u16Height + alignfactor - 1) & (~(alignfactor - 1));

    if(pGOPCtx->pGopChipProperty->bAFBC_Support[u8GOP] ==FALSE)
    {
        GOP_D_ERR("[%s] GOP AFBC mode not support GOP %d \n",__FUNCTION__, u8GOP);
        return GOP_FUN_NOT_SUPPORTED;
    }

    if(pinfo->u16Pitch ==0 || u16Height==0)
    {
        GOP_D_ERR("[%s][%d]  Fail!!! Width=%d, Height=%d,Pitch=%d  \n",__FUNCTION__,__LINE__,\
            pinfo->u16HPixelEnd-pinfo->u16HPixelStart, pinfo->u16VPixelEnd-pinfo->u16VPixelStart, pinfo->u16Pitch);
        return GOP_FAIL;
    }

    HAL_GOP_AFBC_GetCore(&pGOPDrvLocalCtx->halCtxLocal, u8GOP, &u8Core);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_ADDR_L(u8Core), pinfo->u64DRAMAddr, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_ADDR_H(u8Core), pinfo->u64DRAMAddr>>16, GOP_REG_WORD_MASK);
    if(bChangePitch==TRUE)
    {
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_WIDTH(u8Core), u16Pitch-1, GOP_REG_WORD_MASK);
    }
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_HEIGHT(u8Core), u16Height-1, GOP_REG_WORD_MASK);
    if((pinfo->u8Fmt&E_DRV_GOP_AFBC_ARGB8888) ==E_DRV_GOP_AFBC_ARGB8888)
    	HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, REG_AFBC_FMT(u8Core), 0x5 , GOP_BIT0|GOP_BIT1|GOP_BIT2|GOP_BIT3); //Fmt

#ifdef GOP_AFBC_PRELOAD_PATCH
    if((u16Pitch==560)||((u16Pitch==1920)&&(u16Height<416)))
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_CORE_EN(u8Core), 0, GOP_BIT4);
    else
        HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal,  REG_AFBC_CORE_EN(u8Core), GOP_BIT4, GOP_BIT4);
#endif

    if(pGOPCtx->pGopChipProperty->bAFBC_Merge_GOP_Trig ==FALSE)
    {
        bAFBCTrigger=TRUE;
    }
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_DeleteWinHVSize(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize)
{

    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_DeleteWinHVSize(&pGOPDrvLocalCtx->halCtxLocal,u8GOP, u16HSize, u16VSize);
    return GOP_SUCCESS;
}

void MDrv_GOP_SelfFirstHs(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gop, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet = 0;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    _GetBnkOfstByGop(u8Gop, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_RATE, bEnable<<7, BIT(7));
}

GOP_Result MDrv_GOP_GetIPCaptureHStart(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U32 *u32Hstart,E_DRV_GOP_SCALER_WIN eWindow)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U16 reg_val = 0;

    DRV_GOP_XC_GET_SEMPHORE();

	if(eWindow == E_DRV_GOP_MAIN_WINDOW)
	{
	    HAL_GOP_Read16Reg(&pGOPDrvLocalCtx->halCtxLocal, GOP_SC_IP_MAIN_HSTART, &reg_val);
        *u32Hstart = (MS_U32)reg_val;
	}

	DRV_GOP_XC_RELEASE_SEMPHORE();
#else
    if( _fpXCGetCapHStart != NULL )
        *u32Hstart = (MS_U32)_fpXCGetCapHStart();
    else
        printf("[%s] [%d]Callback function fpXCGetCapHStart is NULL\n",__FUNCTION__,__LINE__);
#endif
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GetIPInterlace(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL *bInterlace,E_DRV_GOP_SCALER_WIN eWindow)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    DRV_GOP_XC_GET_SEMPHORE();

	if(eWindow == E_DRV_GOP_MAIN_WINDOW)
	{
        //GOP_CTX_DRV_LOCAL *pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
        HAL_GOP_GetIPInterlace(&pGOPDrvLocalCtx->halCtxLocal, bInterlace);
	}

	DRV_GOP_XC_RELEASE_SEMPHORE();
#else
    if( _fpXCIsInterlace != NULL )
    {
        *bInterlace = _fpXCIsInterlace();
    }else{
        // _fpXCIsInterlace is NULL

        GOP_CTX_DRV_LOCAL *pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
        DRV_GOP_XC_GET_SEMPHORE();

        if(eWindow == E_DRV_GOP_MAIN_WINDOW)
        {
            //GOP_CTX_DRV_LOCAL *pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
            HAL_GOP_GetIPInterlace(&pGOPDrvLocalCtx->halCtxLocal, bInterlace);
        }

        DRV_GOP_XC_RELEASE_SEMPHORE();
    }
#endif
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_IsHDREnabled(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL* pbHDREnable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    HAL_GOP_IsHDREnabled(&pGOPDrvLocalCtx->halCtxLocal, pbHDREnable);

    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_SetDbgLevel(EN_GOP_DEBUG_LEVEL level)
{
    u32GOPDbgLevel_drv = level;
	HAL_GOP_SetDbgLevel(level);
    return GOP_SUCCESS;
}

GOP_Result MDrv_GOP_GWIN_PowerState(void* pInstance, MS_U32 u32PowerState, void* pModule)
{
    MS_U8 i=0,j=0;
    MS_U8 GopIdx=0xF, GopBks=0xF;
    MS_U32 u32BankOffSet=0;
    MS_VIRT MMIOBaseAdr=0xFFFF;
    MS_PHY u32NonPMBankSize=0xFFFF;
    GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate=NULL;
    GOP_CTX_DRV_SHARED* pDrvGOPShared=NULL;
    MS_BOOL bNeedInitShared = FALSE;

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pGOP_STRPrivate);
#else
    GOP_INSTANT_PRIVATE* psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGOPInstPri);
    pGOP_STRPrivate =&(psGOPInstPri->pGOP_STRPrivate);
#endif

    if( !MDrv_MMIO_GetBASE(&MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_GOP))
    {
        GOP_D_FATAL("[%s][%d]Get GOP IOMap failure\n",__FUNCTION__,__LINE__);
        GOP_ASSERT(0);
        return FALSE;
    }
    HAL_GOP_SetIOMapBase(&pGOP_STRPrivate->GOPHalSTRCtx, MMIOBaseAdr);
    pDrvGOPShared = (GOP_CTX_DRV_SHARED*)MDrv_GOP_GetShareMemory(&bNeedInitShared);
    pGOP_STRPrivate->GOPHalSTRCtx.pHALShared = &(pDrvGOPShared->halCtxShared);
    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
        {
                for(i=0;i<GFLIP_REG_BANKS;i++)
                {
                    if (i < 12)
                    {
                        GopIdx=i/GOP_BANK_OFFSET;
                        GopBks=i%GOP_BANK_OFFSET;
                    }
                    else if (i == 12)
                    {
                        //DWIN
                        continue;
                    }
                    else if (i == 13)
                    {
                        //MIXER
                        continue;
                    }
                    else
                    {
                        GopIdx=(i-2)/GOP_2G_OFST;
                        GopBks=(i-2)%GOP_2G_OFST;
                    }

                    //GOP reg
                    for(j=0;j<GFLIP_REG16_NUM_PER_BANK;j++)
                    {
                        HAL_GOP_DumpGOPReg(&pGOP_STRPrivate->GOPHalSTRCtx, GopIdx,
                            GopBks, j, &(pGOP_STRPrivate->BankReg[i][j]));
                    }
                }
                HAL_GOP_PowerState(&pGOP_STRPrivate->GOPHalSTRCtx, u32PowerState, pGOP_STRPrivate);
                //Close Win
                for(i=0; i<MAX_GOP_SUPPORT;i++)
                {
                    _GetBnkOfstByGop(i, &u32BankOffSet);
                    HAL_GOP_Write16Reg(&pGOP_STRPrivate->GOPHalSTRCtx, (u32BankOffSet + GOP_4G_CTRL0), 0, GOP_BIT0);
                }
        }
            break;
        case E_POWER_RESUME:
        {
                HAL_GOP_PowerState(&pGOP_STRPrivate->GOPHalSTRCtx, u32PowerState, pGOP_STRPrivate);
                for(i=0;i<GFLIP_REG_BANKS;i++)
                {
                    if (i < 12)
                    {
                        GopIdx=i/GOP_2G_OFST;
                        GopBks=i%GOP_2G_OFST;
                    }
                    else if (i == 12)
                    {
                        //DWIN
                        continue;
                    }
                    else if (i == 13)
                    {
                        //MIXER
                        continue;
                    }
                    else
                    {
                        GopIdx=(i-2)/GOP_2G_OFST;
                        GopBks=(i-2)%GOP_2G_OFST;
                    }

                    for(j=0;j<GFLIP_REG16_NUM_PER_BANK;j++)
                    {
                        HAL_GOP_RestoreGOPReg(&pGOP_STRPrivate->GOPHalSTRCtx, GopIdx,
                            GopBks, j, (pGOP_STRPrivate->BankReg[i][j]));
                    }
                }

                //ForceWrite
                HAL_GOP_Write16Reg(&pGOP_STRPrivate->GOPHalSTRCtx, GOP_BAK_SEL, GOP_BIT9, GOP_BIT9);
                HAL_GOP_Write16Reg(&pGOP_STRPrivate->GOPHalSTRCtx, GOP_BAK_SEL, 0, GOP_BIT9);
        }
            break;
        default:
            break;
    }
    return GOP_SUCCESS;
}


void MDrv_GOP_GWIN_Interrupt(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8Gop,MS_BOOL bEable)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32BankOffSet=0;

    _GetBnkOfstByGop(u8Gop, &u32BankOffSet);
    HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+GOP_4G_INT, (bEable?0:GOP_BIT0) , GOP_BIT0);
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void MDrv_GOP_MDCMD_GETINFO(void* pInstance, MS_U64* u64ReqHdl)
{
    MS_U8 gop=0, Bpp = 0, u8Miu=0xff;
    MS_U16 u16temp = 0x0, u16temp1 = 0x0, u16Regval = 0x0, u16Regval1= 0x0;
    MS_U32 u32colorInfo = 0x0, u32BankOffSet = 0, DramOffset=0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE* psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psGOPInstPri);
#endif

    MdbPrint(u64ReqHdl, "Total GOP: %d\n", MAX_GOP_SUPPORT);
    HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_MUX, &u16Regval);
    MdbPrint(u64ReqHdl, "OP mux setting: %d:%d:%d:%d:%d\n", ((u16Regval & 0x7000)>>12), ((u16Regval & 0xE00)>>9), ((u16Regval & 0x1C0)>>6), ((u16Regval & 0x38)>>3), (u16Regval & 0x7));

    for(gop = 0; gop < MAX_GOP_SUPPORT; gop++)
    {
        _GetBnkOfstByGop(gop, &u32BankOffSet);
        MdbPrint(u64ReqHdl, "----------MStar GOP%d Info----------\n\n", gop);
        MdbPrint(u64ReqHdl, "GOP%d Initialized: %d\n", gop, g_gopDrvCtxLocal.apiCtxLocal.pGOPCtxShared->bGopHasInitialized[gop]);

        //===== Stretch Window info=====
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_STRCH_HSZ,&u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_STRCH_VSZ,&u16Regval1);
        MdbPrint(u64ReqHdl, "Stretch Original Window(%d, %d)\n", (u16Regval*2), u16Regval1);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_HSTRCH,&u16temp);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_VSTRCH,&u16temp1);
        MdbPrint(u64ReqHdl, "Stretch Output Window(%d, %d)\n", ((u16Regval*2*4096)/u16temp), ((u16Regval1*4096)/u16temp1));

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_STRCH_HSTR, &u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_STRCH_VSTR, &u16Regval1);
        MdbPrint(u64ReqHdl, "Stretch Window Position(%d,%d)\n", u16Regval, u16Regval1);

        //===== GOP Destination info=====
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL1, &u16Regval);
        switch(u16Regval&0xF)
        {
            case 0:
                MdbPrint(u64ReqHdl, "Destination: IP0\n");
            break;
            case 1:
                MdbPrint(u64ReqHdl, "Destination: IP0_SUB\n");
            break;
            case 2:
                MdbPrint(u64ReqHdl, "Destination: OP0\n");
            break;
            case 3:
                MdbPrint(u64ReqHdl, "Destination: VOP\n");
            break;
            case 4:
                MdbPrint(u64ReqHdl, "Destination: VOP_SUB\n");
            break;
            case 6:
                MdbPrint(u64ReqHdl, "Destination: FRC\n");
            break;
            case 0xB:
                MdbPrint(u64ReqHdl, "Destination: OP_DUAL_RATE\n");
            break;
            case 8:
                MdbPrint(u64ReqHdl, "Destination: DIP\n");
            break;
            default:
                MdbPrint(u64ReqHdl, "Destination: INVALID\n");
            break;
        }

        u8Miu = HAL_GOP_GetMIUDst(&g_gopDrvCtxLocal.halCtxLocal, gop);
        MdbPrint(u64ReqHdl, "Address MIU of GOP%d: %d\n", gop,u8Miu);

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &u16Regval);
        MdbPrint(u64ReqHdl, "H Mirror: %d\n", (u16Regval & GOP_BIT12)>>12);
        MdbPrint(u64ReqHdl, "V Mirror: %d\n", (u16Regval & GOP_BIT13)>>13);

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_GWIN_ALPHA01(0), &u16Regval);
        MdbPrint(u64ReqHdl, "Alpha pre-multiplex mode: %d\n", (u16Regval>>15));

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_HS_PIPE, &u16Regval);
        MdbPrint(u64ReqHdl, "Pipe Value: 0x%lx\n", u16Regval);

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &u16Regval);
        MdbPrint(u64ReqHdl, "Progressive: %d\n", (u16Regval & 8));

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &u16Regval);
        if(u16Regval&GOP_BIT10)
            MdbPrint(u64ReqHdl, "Color Space: YUV\n");
        else
            MdbPrint(u64ReqHdl, "Color Space: RGB\n");

        //===== GOP Transparent ColorKey info=====
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, &u16Regval);
        if(u16Regval&GOP_BIT11)
        {
            MdbPrint(u64ReqHdl, "RGB Color key Enable: Enable\n");
            HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_L, &u16Regval);
            HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_TRSCLR_H, &u16Regval1);
            u32colorInfo = (MS_U32)((u16Regval1&GOP_REG_LW_MASK)<<16) | (MS_U32)(u16Regval&GOP_REG_WORD_MASK);
            MdbPrint(u64ReqHdl, "RGB Color key value: 0x%lx\n", u32colorInfo);
        }
        else
            MdbPrint(u64ReqHdl, "Color key Enable: Disable\n");

        //===== GOP GWIN info=====
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_GWIN0_CTRL(0), &u16Regval);
        if(u16Regval&GOP_BIT0)
            MdbPrint(u64ReqHdl, "GWIN Enable: Enable\n");
        else
            MdbPrint(u64ReqHdl, "GWIN Enable: Disable\n");


        switch((u16Regval>>GOP_REG_COLORTYPE_SHIFT)&GOP_REG_COLORTYPE_MASK)
        {
            case GOP_FMT_RGB555_BLINK:
                MdbPrint(u64ReqHdl, "GWIN Format: RGB555_BLINK\n");
                Bpp = 2;
                break;
            case GOP_FMT_RGB565:
                MdbPrint(u64ReqHdl, "GWIN Format: RGB565\n");
                Bpp = 2;
                break;
            case GOP_FMT_ARGB4444:
                MdbPrint(u64ReqHdl, "GWIN Format: ARGB4444\n");
                Bpp = 2;
                break;
            case GOP_FMT_FaBaFgBg2266:
                MdbPrint(u64ReqHdl, "GWIN Format: FaBaFgBg2266\n");
                Bpp = 2;
                break;
            case GOP_FMT_I8:
                MdbPrint(u64ReqHdl, "GWIN Format: I8\n");
                Bpp = 1;
                break;
            case GOP_FMT_ARGB8888:
                MdbPrint(u64ReqHdl, "GWIN Format: ARGB8888\n");
                Bpp = 4;
                break;
            case GOP_FMT_ARGB1555:
                MdbPrint(u64ReqHdl, "GWIN Format: ARGB1555\n");
                Bpp = 2;
                break;
            case GOP_FMT_ABGR8888:
                MdbPrint(u64ReqHdl, "GWIN Format: ABGR8888\n");
                Bpp = 4;
                break;
            case GOP_FMT_ARGB1555_DST:
                MdbPrint(u64ReqHdl, "GWIN Format: ARGB1555\n");
                Bpp = 2;
                break;
            case GOP_FMT_YUV422:
                MdbPrint(u64ReqHdl, "GWIN Format: YUV422\n");
                Bpp = 2;
                break;
            case GOP_FMT_RGBA5551:
                MdbPrint(u64ReqHdl, "GWIN Format: RGBA5551\n");
                Bpp = 2;
                break;
            case GOP_FMT_RGBA4444:
                MdbPrint(u64ReqHdl, "GWIN Format: RGBA4444\n");
                Bpp = 2;
                break;
            case GOP_FMT_RGBA8888:
                MdbPrint(u64ReqHdl, "GWIN Format: RGBA8888\n");
                Bpp = 4;
                break;
            case GOP_FMT_BGR565:
                MdbPrint(u64ReqHdl, "GWIN Format: BGR565\n");
                Bpp = 2;
                break;
            case GOP_FMT_ABGR4444:
                MdbPrint(u64ReqHdl, "GWIN Format: ABGR4444\n");
                Bpp = 2;
                break;
            case GOP_FMT_ABGR1555:
                MdbPrint(u64ReqHdl, "GWIN Format: ABGR1555\n");
                Bpp = 2;
                break;
            case GOP_FMT_BGRA5551:
                MdbPrint(u64ReqHdl, "GWIN Format: BGRA5551\n");
                Bpp = 2;
                break;
            case GOP_FMT_BGRA4444:
                MdbPrint(u64ReqHdl, "GWIN Format: BGRA4444\n");
                Bpp = 2;
                break;
            case GOP_FMT_BGRA8888:
                MdbPrint(u64ReqHdl, "GWIN Format: BGRA8888\n");
                Bpp = 4;
                break;
            default:
                MdbPrint(u64ReqHdl, "GWIN Format: Invalid\n");
                break;
        }

        if(u16Regval&GOP_BIT14)
            MdbPrint(u64ReqHdl, "GWIN Pixel Alpha: True\n");
        else
        {
            MdbPrint(u64ReqHdl, "GWIN Pixel Alpha: False\n");
            HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_GWIN_ALPHA01(0), &u16Regval);
            MdbPrint(u64ReqHdl, "GWIN Global Alpha Value: %x\n", (MS_U8)(u16Regval&0xFF));
        }

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_HSTR(0), &u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_VSTR(0), &u16Regval1);
        MdbPrint(u64ReqHdl, "GWIN Position(%d,%d)\n", u16Regval, u16Regval1);

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_HSIZE(0), &u16Regval);
        MdbPrint(u64ReqHdl, "GWIN Pitch: %d\n", u16Regval);

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_HSTR(0), &u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_HEND(0), &u16Regval1);
        u16temp = u16Regval1 - u16Regval;
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_VSTR(0), &u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_VEND(0), &u16Regval1);
        u16temp1 = u16Regval1 - u16Regval;
        if(g_gopDrvCtxLocal.halCtxLocal.pGopChipPro->bPixelModeSupport)
        {
            MdbPrint(u64ReqHdl, "GWIN Resolution(%d,%d)\n", u16temp, u16temp1);
        }
        else
        {
            u16temp = (u16temp*GOP_WordUnit)/Bpp;
            MdbPrint(u64ReqHdl, "GWIN Resolution(%d,%d)\n", u16temp, u16temp1);
        }
        MdbPrint(u64ReqHdl, "GWIN Buffer Size: 0x%lx\n", (MS_U64)(u16temp*u16temp1*Bpp));

        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_L(0), &u16Regval);
        HAL_GOP_Read16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_DRAM_RBLK_H(0), &u16Regval1);
        DramOffset = ((MS_U32)u16Regval1 << 16) | ((MS_U32)u16Regval);
        MdbPrint(u64ReqHdl, "GWIN miu offset address: 0x%lx\n\n", DramOffset);
    }

}

void MDrv_GOP_MDCMD_EchoCmd(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine)
{
    MS_BOOL enable = FALSE;
    MS_U8 GOPnum = 0;
    MS_U32 u32BankOffSet = 0;
    char pch[] = "=,";
    char* psep;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE* psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psGOPInstPri);
#endif

    psep = strsep(&pcCmdLine,pch);

    if(strncmp("GOPOn", psep, 5)==0)
    {
        psep = strsep(&pcCmdLine,pch);
        GOPnum = (MS_U8)atoi(psep);
        psep = strsep(&pcCmdLine,pch);
        enable = (MS_BOOL)atoi(psep);

        if(GOPnum < MAX_GOP_SUPPORT)
        {
            _GetBnkOfstByGop(GOPnum, &u32BankOffSet);
            HAL_GOP_Write16Reg(&g_gopDrvCtxLocal.halCtxLocal, u32BankOffSet+GOP_4G_CTRL0, (enable?0:GOP_BIT0) , GOP_BIT0);
            if(enable == TRUE)
                MdbPrint(u64ReqHdl,"Enable GOP%d\n", GOPnum);
            else
                MdbPrint(u64ReqHdl,"Disable GOP%d\n", GOPnum);
        }
        else
        {
            MdbPrint(u64ReqHdl,"error GOP num %d, the arg should follow this form [GOPOnOff num=x,x] \n",GOPnum);
        }
    }

    if(strncmp("testpattern", psep, 11)==0)
    {
        psep = strsep(&pcCmdLine,pch);
        enable = (MS_BOOL)atoi(psep);
        HAL_GOP_Write16Reg(&g_gopDrvCtxLocal.halCtxLocal,REG_TSTCLR_EN, enable<<SHIFT_TSTCLR_EN, MASK_TSTCLR_EN);
        if(enable==1)
            MdbPrint(u64ReqHdl,"Enable GOP Test Pattern\n");
        else
            MdbPrint(u64ReqHdl,"Disable GOP Test Pattern\n");
    }
}
#endif
