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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// This file contains MStar ACE Application Interface.
/// brief API for Advanced Color Engine Control
/// author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _API_XC_ACE_C_
#define _API_XC_ACE_C_

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/

// Need to remove
//#include "msAPI_Timer.h"


// Must have
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "utopia.h"
#include "utopia_dapi.h"


#include "apiXC_Ace.h"
#include "drvACE.h"
// Internal Definition
#include "drvMMIO.h"
#include "apiXC_Ace_v2.h"
#include "ACE_private.h"
#include "apiXC.h"
#include "mvideo_context.h"

extern void MDrv_XC_SetMWEStatus(void *pInstance, MS_BOOL bMWEStatus);
extern MS_BOOL MDrv_XC_GetMWEStatus(void *pInstance);
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define MAIN_WINDOW     0
#define SUB_WINDOW      1

/******************************************************************************/
/*                      Debug information                                      */
/******************************************************************************/
#ifdef CONFIG_MS_DEBUG_XC_LOG
#ifndef XC_ACE_DBG
#define XC_ACE_DBG
#endif
#endif

//#define XC_ACE_DBG  0
#define XC_ACE_MUTEX_DBG 0

#if (defined CONFIG_MLOG)
#define ACE_PRINTF(fmt,_args...) \
        {\
            ULOGD("ACE_DBG_MSG","[%s,%5d] ",__FUNCTION__,__LINE__);\
            ULOGD("ACE_DBG_MSG",fmt,##_args);\
        }
#else
#define ACE_PRINTF(fmt,...) \
        {\
            printf("[ACE_DBG_MSG]:"); \
            printf("%s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
        }
#endif

#ifdef XC_ACE_DBG
#define XC_ACE_DBG_MSG(fmt,_args...) \
    do{\
        ACE_PRINTF(fmt,##_args);\
    } while(0)

#define XC_ACE_PRINT_VAR(var)  \
    do{\
        XC_ACE_DBG_MSG("%30s: %d\n", #var, (var));\
        }while(0)

#define XC_ACE_PRINT_FUN(fun)  \
    do{\
        XC_ACE_DBG_MSG("\n");\
        XC_ACE_DBG_MSG("**************************************************\n");\
        XC_ACE_DBG_MSG("*\t\t%s \t\t\n", #fun);\
        XC_ACE_DBG_MSG("**************************************************\n");\
        fun;\
        XC_ACE_DBG_MSG("*                                                *\n");\
        XC_ACE_DBG_MSG("*                                                *\n");\
        XC_ACE_DBG_MSG("**************************************************\n");\
        }while(0)

#define ACE_DBG_MSG(fmt,_args...)                                                                 \
    do{                                                                         \
        if(_u16XC_ACEDbgSwitch){                                                             \
           ACE_PRINTF(fmt,##_args); \
          }                                                                                  \
      } while(0)

#else
  #define XC_ACE_DBG_MSG(str, ...)
  #define XC_ACE_PRINT_VAR(var)
  #define XC_ACE_PRINT_FUN(fun) fun
  #define ACE_DBG_MSG(fmt,...)
#endif

#define MWE_TIMER           60
#define MWE_H_STEP          6
#define MWE_MOVE_RIGHT      1
#define MWE_MOVE_LEFT       0

#if 0
static E_XC_ACE_MWE_FUNC  _MWEFunc = E_XC_ACE_MWE_MODE_OFF;
static MS_U32       _MWETimer = 0;
static MS_U32       _MWEFlag = MWE_MOVE_LEFT;
static MS_BOOL bMWEStatus = FALSE;
static MWE_RECT gMWERect;

static XC_ACE_InitData g_XC_ACE_InitData;
static const XC_ACE_ApiInfo _stXC_ACE_ApiInfo = {};
#if XC_ACE_DBG
static MS_U16 _u16XC_ACEDbgSwitch = 0;
#endif
static MS_BOOL _bMWESkipDriverSetting = FALSE;
static E_XC_ACE_MWE_MIRROR_TYPE _enMirrorMode = E_XC_ACE_MWE_MIRROR_NORMAL;
#else
#define _MWEFunc psACEResPri->stapiACE._MWEFunc
#define _MWETimer psACEResPri->stapiACE._MWETimer
#define _MWEFlag psACEResPri->stapiACE._MWEFlag
//#define bMWEStatus psACEResPri->stapiACE.bMWEStatus
#define gMWERect psACEResPri->stapiACE.gMWERect
#define g_XC_ACE_InitData psACEResPri->stapiACE.g_XC_ACE_InitData
#define _bMWESkipDriverSetting psACEResPri->stapiACE._bMWESkipDriverSetting
#define _enMirrorMode psACEResPri->stapiACE._enMirrorMode
#define g_u8MotionStableCnt psACEResPri->stapiACE.g_u8MotionStableCnt
#define g_u8LumaStableCnt psACEResPri->stapiACE.g_u8LumaStableCnt
#define g_u8FlickStableCnt psACEResPri->stapiACE.g_u8FlickStableCnt
#define g_bACEModuleInited psACEResPri->stapiACE.g_bACEModuleInited

static const XC_ACE_ApiInfo _stXC_ACE_ApiInfo = {};
#ifdef XC_ACE_DBG
static MS_U16 _u16XC_ACEDbgSwitch = 0;
#endif

#endif

// OS related
#define XC_ACE_WAIT_MUTEX              (TICK_PER_ONE_MS * 50)              // 50 ms

// Mutex & Lock
static MS_S32 _s32XC_ACEMutex = -1;

#if XC_ACE_MUTEX_DBG
#define XC_ACE_ENTRY()      if(!MsOS_ObtainMutex(_s32XC_ACEMutex, MSOS_WAIT_FOREVER))                     \
                            {                                                                        \
                                printf("==========================\n");                              \
                                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                            }
#else
#define XC_ACE_ENTRY()      if(!MsOS_ObtainMutex(_s32XC_ACEMutex, MSOS_WAIT_FOREVER))                     \
                            {                                                                        \
                            }
#endif

#define XC_ACE_RETURN()     MsOS_ReleaseMutex(_s32XC_ACEMutex);


static MSIF_Version _api_xc_ace_version = {
    .DDI = { XC_ACE_API_VERSION },
};

// for Utopia 2.0 to Utopia 1.0 compatibility.

void* pu32ACEInst = NULL;
void* pu32ACERes[E_ACE_POOL_ID_MAX];

void* pu32ACE_XCInst = NULL;

MS_U32 MDrv_ACE_Get_Semaphore(void *pInstance)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);

    if (psACEInstPri->u32DeviceID >= E_ACE_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown ACE Pool ID\n",__func__, __LINE__);
        return u32Return;
    }

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, psACEInstPri->u32DeviceID, &pu32ACERes[psACEInstPri->u32DeviceID]) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 MDrv_ACE_Release_Semaphore(void *pInstance)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);
    if (psACEInstPri->u32DeviceID >= E_ACE_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown ACE Pool ID\n",__func__, __LINE__);
        return u32Return;
    }

    u32Return = UtopiaResourceRelease(pu32ACERes[psACEInstPri->u32DeviceID]);
    return u32Return;
}


//******************************************************************************
///-initialize ACE
//*************************************************************************

static MS_BOOL _MApi_XC_ACE_Init_WithoutCreateMutex(void *pInstance, XC_ACE_InitData *pstXC_ACE_InitData, MS_BOOL bSkipInitVar)
{
    MS_S16 s16HShiftStep = 0;
    _ACE_GET_VARIABLE();

    if(pstXC_ACE_InitData->S16ColorCorrectionMatrix == NULL ||
       pstXC_ACE_InitData->S16RGB == NULL)
    {
        return FALSE;
    }
    else
    {
        //STR case can not set var again
        if(!bSkipInitVar)
            MDrv_ACE_Init(pInstance, pstXC_ACE_InitData->eWindow, pstXC_ACE_InitData->S16ColorCorrectionMatrix, pstXC_ACE_InitData->S16RGB );
    }
    MDrv_ACE_SetColorCorrectionTable(pInstance,  pstXC_ACE_InitData->eWindow);
    MDrv_ACE_SetPCsRGBTable(pInstance,  pstXC_ACE_InitData->eWindow);

    if(pstXC_ACE_InitData->bMWE_Enable)
    {
        XC_ACE_ENTRY();

        MDrv_XC_ACE_MWEinit(pInstance, pstXC_ACE_InitData->u16MWEHstart, pstXC_ACE_InitData->u16MWEVstart, pstXC_ACE_InitData->u16MWEWidth, pstXC_ACE_InitData->u16MWEHeight);

        gMWERect.Xpos = 0;
        gMWERect.Ypos = 0;
        //for fix H shift step when Display is 1/2.
        s16HShiftStep = g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth;

        //w and h size
        if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_HV)
        {
            //for fix screen shift function in contrast expander function
            if((abs(g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth)) <= 10)
            {
                if (g_XC_ACE_InitData.u16MWE_Disp_Hstart == 0)
                    gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2;
                else
                    gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2 + s16HShiftStep;
            }
            else
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/2;
        }
        else
        {
            //for fix screen shift function in contrast expander function
            if((abs(g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth)) <= 10)
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2 + s16HShiftStep;
            else
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/2;
        }

        gMWERect.Ysize = g_XC_ACE_InitData.u16MWE_Disp_Height;

        //x and y start position
        if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_NORMAL || _enMirrorMode == E_XC_ACE_MWE_MIRROR_V_ONLY)
        {
            gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
            //for patch 4:3 in contrast expander function
            if (g_XC_ACE_InitData.u16MWEWidth > g_XC_ACE_InitData.u16MWE_Disp_Hstart)
                gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth/2;
            else
                gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart + g_XC_ACE_InitData.u16MWEWidth/2;
        }
        else if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_HV)
        {

            if( g_XC_ACE_InitData.u16MWEWidth > ( g_XC_ACE_InitData.u16MWE_Disp_Hstart + g_XC_ACE_InitData.u16MWE_Disp_Width) )
            {
                //for patch 4:3 in contrast expander function
                if (g_XC_ACE_InitData.u16MWEWidth > g_XC_ACE_InitData.u16MWE_Disp_Hstart)
                    gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth - ( gMWERect.Xpos + g_XC_ACE_InitData.u16MWE_Disp_Width ) - g_XC_ACE_InitData.u16MWE_Disp_Hstart;
                else
                    gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth - ( gMWERect.Xpos + g_XC_ACE_InitData.u16MWE_Disp_Width );
            }
            else
            {
                gMWERect.Xpos = 0;
            }

            if( g_XC_ACE_InitData.u16MWEHeight > ( g_XC_ACE_InitData.u16MWE_Disp_Vstart + g_XC_ACE_InitData.u16MWE_Disp_Height) )
            {
                //for patch smart view in contrast expander function
                if ((g_XC_ACE_InitData.u16MWE_Disp_Vstart != 0) && (g_XC_ACE_InitData.u16MWE_Disp_Height != g_XC_ACE_InitData.u16MWEHeight))
                    gMWERect.Ypos = g_XC_ACE_InitData.u16MWEHeight- ( gMWERect.Ypos + g_XC_ACE_InitData.u16MWE_Disp_Height ) - g_XC_ACE_InitData.u16MWE_Disp_Vstart;
                else
                    gMWERect.Ypos = g_XC_ACE_InitData.u16MWEHeight - ( gMWERect.Ypos + g_XC_ACE_InitData.u16MWE_Disp_Height );
            }
            else
            {
                gMWERect.Ypos = 0;
            }
        }
        else
        {
            gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart+g_XC_ACE_InitData.u16MWE_Disp_Width - gMWERect.Xsize;
            gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
        }

        MDrv_XC_ACE_MWESetWin(pInstance, &gMWERect);
        MDrv_XC_ACE_MWESetBorder(pInstance, SUB_WINDOW, ENABLE, MWE_BORDER_COLOR_BLUE,
                0x02, 0x11, 0x11, 0x11);

        MDrv_XC_SetMWEStatus(pInstance, FALSE);
        MDrv_XC_ACE_MWEEnable(pInstance, FALSE, _bMWESkipDriverSetting);

        XC_ACE_RETURN();
    }

    return TRUE;
}

MS_BOOL MApi_XC_ACE_Init_U2(void *pInstance, XC_ACE_InitData *pstXC_ACE_InitData, MS_U32 u32InitDataLen)
{
    MS_PHY u32NonPMBankSize;
	MS_VIRT ptrACERiuBaseAddr = 0;
    char word[] = {"_ACE_Mutex"};
    MS_BOOL bRet = TRUE;
    //if(u32InitDataLen != sizeof(XC_ACE_InitData) || pstXC_ACE_InitData == NULL)
    //{
    //    return FALSE;
    //}

#if 0
    if (_s32XC_ACEMutex != -1)
    {
        (printf("[MAPI ACE][%06d] Initialization more than once\n", __LINE__));
        return FALSE;
    }

    _s32XC_ACEMutex = MsOS_CreateMutex(E_MSOS_FIFO, word, MSOS_PROCESS_SHARED);
    if (_s32XC_ACEMutex == -1)
    {
        //(printf("[MAPI ACE][%06d] create mutex fail\n", __LINE__));
        return FALSE;
    }
#else
    if (_s32XC_ACEMutex == -1)
    {
        _s32XC_ACEMutex = MsOS_CreateMutex(E_MSOS_FIFO, word, MSOS_PROCESS_SHARED);
        if (_s32XC_ACEMutex == -1)
        {
            (printf("[MAPI ACE][%06d] create mutex fail\n", __LINE__));
            return FALSE;
        }
    }
#endif

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &ptrACERiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PM ) != TRUE)
    {
        //OS_DELETE_MUTEX(_s32ACEMutex);
        //PNL_ASSERT(0, "%s\n", "Get base address failed\n");
        //(printf("XC_ACE GetBase failed\n"));
        return FALSE;
    }
    else
    {
    }
    MDrv_ACE_init_riu_base( ptrACERiuBaseAddr);

    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    // MWE init start
    memcpy(&g_XC_ACE_InitData, pstXC_ACE_InitData, u32InitDataLen);
    _bMWESkipDriverSetting = FALSE;

    bRet = _MApi_XC_ACE_Init_WithoutCreateMutex(pInstance, &g_XC_ACE_InitData, FALSE);
    g_bACEModuleInited[pstXC_ACE_InitData->eWindow] = TRUE;

    _ACE_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_XC_ACE_Init(XC_ACE_InitData *pstXC_ACE_InitData, MS_U32 u32InitDataLen)
{
    if (pu32ACEInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }
    }
    if (pu32ACE_XCInst == NULL)
    {
        XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
        memset(&stXCInstantAttribute, 0, sizeof(stXCInstantAttribute));
        stXCInstantAttribute.u32DeviceID = 0;
        stXCInstantAttribute.eXCType     = E_XC_INSTANT_TYPE_ACE;
        if(UtopiaOpen(MODULE_XC, &pu32ACE_XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32ACE_XCInst failed\n");
            return E_XC_ACE_FAIL;
        }
    }

    stACE_Init ACEArgs;
    ACEArgs.pstXC_ACE_InitData = pstXC_ACE_InitData;
    ACEArgs.u32InitDataLen = u32InitDataLen;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_INIT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_Exit_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    memset(&g_XC_ACE_InitData, 0 , sizeof(XC_ACE_InitData) );
    _bMWESkipDriverSetting = FALSE;
    _ACE_SEMAPHORE_RETURN(pInstance);
    //_s32XC_ACEMutex = -1;
        MsOS_DeleteMutex(_s32XC_ACEMutex);
        _s32XC_ACEMutex = -1;
    return TRUE;
}

MS_BOOL MApi_XC_ACE_Exit(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_Exit ACEArgs;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_EXIT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        ACEArgs.bReturn = FALSE;
    }

    if(UtopiaClose(pu32ACEInst) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaClose ACE failed\n");
        return FALSE;
    }
    pu32ACEInst = NULL;

    if(UtopiaClose(pu32ACE_XCInst) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaClose ACE failed\n");
        return FALSE;
    }
    pu32ACE_XCInst = NULL;

    return ACEArgs.bReturn;
}

void MApi_XC_ACE_DMS_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bisATV)
{
    XC_ACE_ENTRY();
    MDrv_ACE_DMS(pInstance, eWindow, bisATV);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_DMS(MS_BOOL eWindow, MS_BOOL bisATV)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_DMS ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bisATV = bisATV;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DMS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetPCYUV2RGB_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bEnable)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    /*
    ** Save setwindow info for str
    */
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->bSetPCYUV2RGB_Enable = bEnable;
#endif
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PCForceYUVtoRGB(pInstance, eWindow , bEnable);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_SetPCYUV2RGB(MS_BOOL eWindow, MS_BOOL bEnable)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetPCYUV2RGB ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETPCYUV2RGB, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SelectYUVtoRGBMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SelectYUVtoRGBMatrix(pInstance, eWindow, ucMatrix, psUserYUVtoRGBMatrix );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_SelectYUVtoRGBMatrix(MS_BOOL eWindow, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SelectYUVtoRGBMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.ucMatrix = ucMatrix;
    ACEArgs.psUserYUVtoRGBMatrix = psUserYUVtoRGBMatrix;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SELECTYUVTORGBMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}


void MApi_XC_ACE_SetColorCorrectionTable_U2(void *pInstance, MS_BOOL bScalerWin)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SetColorCorrectionTable(pInstance, bScalerWin );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_SetColorCorrectionTable(MS_BOOL bScalerWin)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetColorCorrectionTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETCOLORCORRECTIONTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetPCsRGBTable_U2(void *pInstance, MS_BOOL bScalerWin)
{
    MS_BOOL bSettingByCFD = FALSE;
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2) && !defined(ACE_NOT_IN_KERNEL)
    XC_HDR_SUPPORTED_CAPS stHdrSupportedCaps;
    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_DOLBY;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bSettingByCFD = stHdrSupportedCaps.bSupported;

    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_OPEN;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bSettingByCFD |= stHdrSupportedCaps.bSupported;

#endif

    if (bSettingByCFD == FALSE)
    {
        _ACE_SEMAPHORE_ENTRY(pInstance);
        XC_ACE_ENTRY();
        MDrv_ACE_SetPCsRGBTable(pInstance, bScalerWin);
        XC_ACE_RETURN();
        _ACE_SEMAPHORE_RETURN(pInstance);
    }
}

void MApi_XC_ACE_SetPCsRGBTable(MS_BOOL bScalerWin)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetPCsRGBTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETPCSRGBTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_GetColorMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    XC_ACE_ENTRY();
    MDrv_ACE_GetColorMatrix(pInstance, eWindow, pu16Matrix);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_GetColorMatrix(MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_ColorMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pu16Matrix = pu16Matrix;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetColorMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    XC_ACE_ENTRY();
    MDrv_ACE_SetColorMatrix(pInstance, eWindow, pu16Matrix);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_SetColorMatrix( MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_ColorMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pu16Matrix = pu16Matrix;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

E_XC_ACE_RESULT MApi_XC_ACE_SetBypassColorMatrix_U2(void *pInstance, MS_BOOL bEnable)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    MDrv_ACE_SetBypassColorMatrix(pInstance, bEnable );
    _ACE_SEMAPHORE_RETURN(pInstance);
    return E_XC_ACE_OK;
}

E_XC_ACE_RESULT MApi_XC_ACE_SetBypassColorMatrix(MS_BOOL bEnable)
{
    if (pu32ACEInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }
    }

    stACE_SetBypassColorMatrix ACEArgs;
    ACEArgs.bEnable = bEnable;
    ACEArgs.eResult = E_XC_ACE_FAIL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETBYPASSCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        return ACEArgs.eResult;
    }
}

MS_BOOL MApi_XC_ACE_SetIHC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val)
{
    MS_BOOL bRet = FALSE;
    XC_ACE_ENTRY();
    bRet = MDrv_ACE_SetIHC(pInstance, bScalerWin, eIHC, u8Val);
    XC_ACE_RETURN();
    return bRet;
}

MS_BOOL MApi_XC_ACE_SetIHC(MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_SetIHC ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.eIHC = eIHC;
    ACEArgs.u8Val = u8Val;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETIHC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_SetICC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val)
{
    MS_BOOL bRet = FALSE;
    XC_ACE_ENTRY();
    bRet = MDrv_ACE_SetICC(pInstance, bScalerWin, eICC, u8Val);
    XC_ACE_RETURN();
    return bRet;
}

MS_BOOL SYMBOL_WEAK MApi_XC_ACE_SetICC(MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_SetICC ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.eICC = eICC;
    ACEArgs.u8Val = u8Val;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETICC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_SetIBC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val)
{
    MS_BOOL bRet = FALSE;
    XC_ACE_ENTRY();
    bRet = MDrv_ACE_SetIBC(pInstance, bScalerWin, eIBC, u8Val);
    XC_ACE_RETURN();
    return bRet;
}

MS_BOOL SYMBOL_WEAK MApi_XC_ACE_SetIBC(MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_SetIBC ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.eIBC = eIBC;
    ACEArgs.u8Val = u8Val;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETIBC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_PatchDTGColorChecker_U2(void *pInstance, MS_U8 u8Mode)
{
    XC_ACE_ENTRY();
    MDrv_ACE_PatchDTGColorChecker(pInstance, u8Mode);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_PatchDTGColorChecker(MS_U8 u8Mode)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PatchDTGColorChecker ACEArgs;
    ACEArgs.u8Mode = u8Mode;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PATCHDTGCOLORCHECKER, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U16 MApi_XC_ACE_GetACEInfo_U2(void *pInstance, MS_BOOL bWindow, E_XC_ACE_INFOTYPE eXCACEInfoType)
{
    MS_U16 u16ACEinfo;
    u16ACEinfo = eXCACEInfoType;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    u16ACEinfo = MDrv_ACE_GetACEInfo(pInstance, bWindow, u16ACEinfo);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);

    return u16ACEinfo;
}

MS_U16 MApi_XC_ACE_GetACEInfo(MS_BOOL bWindow, E_XC_ACE_INFOTYPE eXCACEInfoType)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_GetACEInfo ACEArgs;
    ACEArgs.bWindow = bWindow;
    ACEArgs.eXCACEInfoType = eXCACEInfoType;
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETACEINFO, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return ACEArgs.u16Return;
    }
}

void MApi_XC_ACE_GetACEInfoEx_U2(void *pInstance, MS_BOOL bWindow, XC_ACE_InfoEx *pInfoEx)
{
    memset(pInfoEx, 0, sizeof(XC_ACE_InfoEx));

    XC_ACE_ENTRY();

    if (MDrv_ACE_IsPostRGBGain(pInstance, bWindow))
    {
        pInfoEx->u32Mode |= ACE_INFOEX_MODE_POST_RGB_GAIN;
    }

    if (MDrv_ACE_IsPostRGBOffset(pInstance, bWindow))
    {
        pInfoEx->u32Mode |= ACE_INFOEX_MODE_POST_RGB_OFFSET;
    }

    XC_ACE_RETURN();
}

void MApi_XC_ACE_GetACEInfoEx( MS_BOOL bWindow, XC_ACE_InfoEx *pInfoEx)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_GetACEInfoEx ACEArgs;
    ACEArgs.bWindow = bWindow;
    ACEArgs.pInfoEx = pInfoEx;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETACEINFOEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/********************************************************************************/
/*                   msAPI_ACE_Ext.c                   */
/********************************************************************************/

/********************************************************************************/
/*                   Functions                      */
/********************************************************************************/


/******************************************************************************/
///-This function will set up brightness for YPbPr and digital port
///@param eWindow        \b IN: Window type
///@param u8Brightness_R \b IN: brightness R value
///@param u8Brightness_G \b IN: brightness G value
///@param u8Brightness_B \b IN: brightness B value
/******************************************************************************/
void MApi_XC_ACE_PicSetBrightness_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8Brightness_R, MS_U8 u8Brightness_G, MS_U8 u8Brightness_B)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->u8Brightness_R = u8Brightness_R;
    pACEResourceStr->u8Brightness_G = u8Brightness_G;
    pACEResourceStr->u8Brightness_B = u8Brightness_B;
#endif
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SetBrightness(pInstance, eWindow, u8Brightness_R, u8Brightness_G, u8Brightness_B);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetBrightness( MS_BOOL eWindow, MS_U8 u8Brightness_R, MS_U8 u8Brightness_G, MS_U8 u8Brightness_B)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetBrightness ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8Brightness_R = u8Brightness_R;
    ACEArgs.u8Brightness_G = u8Brightness_G;
    ACEArgs.u8Brightness_B = u8Brightness_B;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETBRIGHTNESS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-This function will set up Precise brightness for YPbPr and digital port
///@param eWindow        \b IN: Window type
///@param u16Brightness_R \b IN: brightness R value
///@param u16Brightness_G \b IN: brightness G value
///@param u16Brightness_B \b IN: brightness B value
/******************************************************************************/
void MApi_XC_ACE_PicSetBrightnessPrecise_U2(void *pInstance, MS_BOOL eWindow, MS_U16 u16Brightness_R, MS_U16 u16Brightness_G, MS_U16 u16Brightness_B)
{
    XC_ACE_ENTRY();
    MDrv_ACE_SetBrightnessPrecise(pInstance, eWindow, u16Brightness_R, u16Brightness_G, u16Brightness_B);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_PicSetBrightnessPrecise(MS_BOOL eWindow, MS_U16 u16Brightness_R, MS_U16 u16Brightness_G, MS_U16 u16Brightness_B)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetBrightnessPrecise ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u16Brightness_R = u16Brightness_R;
    ACEArgs.u16Brightness_G = u16Brightness_G;
    ACEArgs.u16Brightness_B = u16Brightness_B;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETBRIGHTNESSPRECISE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-This function will set Pre Y Offset
///@param eWindow        \b IN: Window type
///@param u8PreYOffset \b IN: Pre Y Offset value
/// @return @ref MS_BOOL
/******************************************************************************/
MS_BOOL MApi_XC_ACE_PicSetPreYOffset_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8PreYOffset)
{
    MS_BOOL bResult = FALSE;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    bResult = MDrv_ACE_SetPreYOffset(pInstance, eWindow, u8PreYOffset);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
    return bResult;
}

E_XC_ACE_RESULT MApi_XC_ACE_PicSetPreYOffset(MS_BOOL eWindow, MS_U8 u8PreYOffset)
{
    if (pu32ACEInst == NULL)
    {
        printf("[%s]:ACE not initialized!\n", __FUNCTION__);
        return E_XC_ACE_FAIL;
    }

    stACE_PreYOffset stACEArgs;
    stACEArgs.eWindow = eWindow;
    stACEArgs.u8PreYOffset = u8PreYOffset;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSET_PRE_Y_OFFSET, (void*)&stACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_OBTAIN_RESOURCE_FAIL;
    }

    return E_XC_ACE_OK;
}

/******************************************************************************/
///-This function will get Pre Y Offset
///@param eWindow        \b IN: Window type
/// @return @ref MS_U8
/******************************************************************************/
MS_U8 MApi_XC_ACE_PicGetPreYOffset_U2(void *pInstance, MS_BOOL eWindow)
{
    MS_U8 u8PreYOffset = 0;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    u8PreYOffset = MDrv_ACE_GetPreYOffset(pInstance, eWindow);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
    return u8PreYOffset;
}

MS_U8 MApi_XC_ACE_PicGetPreYOffset(MS_BOOL eWindow)
{
    if (pu32ACEInst == NULL)
    {
        printf("[%s]:ACE not initialized!\n", __FUNCTION__);
        return E_XC_ACE_FAIL;
    }

    stACE_PreYOffset stACEArgs;
    stACEArgs.eWindow = eWindow;
    stACEArgs.u8PreYOffset = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICGET_PRE_Y_OFFSET, (void*)&stACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_OBTAIN_RESOURCE_FAIL;
    }

    return stACEArgs.u8PreYOffset;

}

/******************************************************************************/
//-Adjust sharpness
///@param eWindow        \b IN: Window type
///@param u8Sharpness    \b IN: sharpness value
//- 0 -0x3f
/******************************************************************************/
void MApi_XC_ACE_PicSetSharpness_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8Sharpness )
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->u8Sharpness = u8Sharpness;
#endif
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SetSharpness(pInstance, eWindow, u8Sharpness);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetSharpness( MS_BOOL eWindow, MS_U8 u8Sharpness )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetSharpness ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8Sharpness = u8Sharpness;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETSHARPNESS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
//-Adjust contrast for YPbPr and digital port
//@param u8Contrast \b IN:
//- video color Contrast setting 0 -0xff
//@param u8SubContrast \b IN:
//- video color SubContrast setting 0 -0xff
/******************************************************************************/

void MApi_XC_ACE_PicSetContrast_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast)
{
    MS_BOOL bIgnoreColorSpace = FALSE;
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    XC_HDR_SUPPORTED_CAPS stHdrSupportedCaps;
    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_DOLBY;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace = stHdrSupportedCaps.bSupported;

    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_OPEN;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace |= stHdrSupportedCaps.bSupported;
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->bUseYUVSpace = bUseYUVSpace;
    pACEResourceStr->u8Contrast = u8Contrast;
#endif
    if (bIgnoreColorSpace == TRUE)
    {
        _ACE_SEMAPHORE_ENTRY(pInstance);
        XC_ACE_ENTRY();
        MDrv_ACE_PicSetContrast(pInstance, eWindow, bUseYUVSpace, u8Contrast);
        XC_ACE_RETURN();
        _ACE_SEMAPHORE_RETURN(pInstance);
    }
    else
    {
        if (bUseYUVSpace)
        {
            _ACE_SEMAPHORE_ENTRY(pInstance);
            XC_ACE_ENTRY();
            MDrv_ACE_PicSetContrast(pInstance, eWindow, bUseYUVSpace, u8Contrast);
            XC_ACE_RETURN();
            _ACE_SEMAPHORE_RETURN(pInstance);
        }
        else
        {
            ;
        }
    }

}

void MApi_XC_ACE_PicSetContrast( MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetContrast ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Contrast = u8Contrast;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETCONTRAST, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-Adjust Hue for YPbPr and digital port
///@param eWindow \b IN: Enable Hue
///@param bUseYUVSpace \b IN: Is Yuv space
///@param u8Hue \b IN: Hue value
/******************************************************************************/
void MApi_XC_ACE_PicSetHue_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Hue )
{

    MS_BOOL bIgnoreColorSpace = FALSE;
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    XC_HDR_SUPPORTED_CAPS stHdrSupportedCaps;
    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_DOLBY;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace = stHdrSupportedCaps.bSupported;

    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_OPEN;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace |= stHdrSupportedCaps.bSupported;
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->bUseYUVSpace = bUseYUVSpace;
    pACEResourceStr->u8Hue = u8Hue;
#endif
    if (bIgnoreColorSpace == TRUE)
    {
        _ACE_SEMAPHORE_ENTRY(pInstance);
        XC_ACE_ENTRY();
        MDrv_ACE_SetHue(pInstance, eWindow, u8Hue);
        XC_ACE_RETURN();
        _ACE_SEMAPHORE_RETURN(pInstance);
    }
    else
    {
        if(bUseYUVSpace)
        {
            _ACE_SEMAPHORE_ENTRY(pInstance);
            XC_ACE_ENTRY();
            MDrv_ACE_SetHue(pInstance, eWindow, u8Hue);
            XC_ACE_RETURN();
            _ACE_SEMAPHORE_RETURN(pInstance);
        }
        else
        {
            ;
        }
    }
}

void MApi_XC_ACE_PicSetHue(MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Hue)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetHue ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Hue = u8Hue;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETHUE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//-------------------------------------------------------------------------------------------------
// Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
E_XC_ACE_RESULT MApi_XC_ACE_SkipWaitVsync_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bIsSkipWaitVsyn)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    MDrv_ACE_SetSkipWaitVsync(pInstance, eWindow, bIsSkipWaitVsyn);
    _ACE_SEMAPHORE_RETURN(pInstance);
    return E_XC_ACE_OK;
}

E_XC_ACE_RESULT MApi_XC_ACE_SkipWaitVsync( MS_BOOL eWindow, MS_BOOL bIsSkipWaitVsyn)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return E_XC_ACE_FAIL;
    }

    stACE_SkipWaitVsync ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bIsSkipWaitVsyn = bIsSkipWaitVsyn;
    ACEArgs.eResult = E_XC_ACE_FAIL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SKIPWAITVSYNC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        return  ACEArgs.eResult;
    }
}

/******************************************************************************/
//-Adjust Saturation for YPbPr and digital port
//@param u8Saturation \b IN:
//- video color setting 0 -100
/******************************************************************************/
void MApi_XC_ACE_PicSetSaturation_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Saturation)
{
    MS_BOOL bIgnoreColorSpace = FALSE;
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    XC_HDR_SUPPORTED_CAPS stHdrSupportedCaps;
    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_DOLBY;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace = stHdrSupportedCaps.bSupported;

    stHdrSupportedCaps.enHDRType = E_XC_HDR_TYPE_OPEN;
    stHdrSupportedCaps.bSupported = FALSE;
    MApi_XC_GetChipCaps(E_XC_SUPPORT_HDR, (MS_U32*)&stHdrSupportedCaps, sizeof(XC_HDR_SUPPORTED_CAPS));
    bIgnoreColorSpace |= stHdrSupportedCaps.bSupported;
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    pACEResourceStr->bUseYUVSpace = bUseYUVSpace;
    pACEResourceStr->u8Saturation = u8Saturation;
#endif

    if (bIgnoreColorSpace == TRUE)
    {
        _ACE_SEMAPHORE_ENTRY(pInstance);
        XC_ACE_ENTRY();
        MDrv_ACE_SetSaturation(pInstance, eWindow, u8Saturation);
        XC_ACE_RETURN();
        _ACE_SEMAPHORE_RETURN(pInstance);
    }
    else
    {
        if(bUseYUVSpace)
        {
            _ACE_SEMAPHORE_ENTRY(pInstance);
            XC_ACE_ENTRY();
            MDrv_ACE_SetSaturation(pInstance, eWindow, u8Saturation);
            XC_ACE_RETURN();
            _ACE_SEMAPHORE_RETURN(pInstance);
        }
        else
        {
            ;
        }
    }

}

void MApi_XC_ACE_PicSetSaturation(MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Saturation )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetSaturation ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Saturation = u8Saturation;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETSATURATION, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetFleshTone_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bEn, MS_U8 u8FleshTone)
{
    XC_ACE_ENTRY();
    MDrv_ACE_SetFleshTone(pInstance, eWindow, bEn, u8FleshTone);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_SetFleshTone(MS_BOOL eWindow, MS_BOOL bEn, MS_U8 u8FleshTone)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetFleshTone ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bEn = bEn;
    ACEArgs.u8FleshTone = u8FleshTone;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETFLESHTONE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetBlackAdjust_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8BlackAdjust)
{
    XC_ACE_ENTRY();
    MDrv_ACE_SetBlackAdjust(pInstance, eWindow, u8BlackAdjust);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_SetBlackAdjust(MS_BOOL eWindow, MS_U8 u8BlackAdjust)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetBlackAdjust ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8BlackAdjust = u8BlackAdjust;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETBLACKADJUST, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetColorTemp_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetRGB(pInstance, eWindow, bUseYUVSpace , pstColorTemp->cRedColor, pstColorTemp->cGreenColor, pstColorTemp->cBlueColor );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetColorTemp(MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetColorTemp ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETCOLORTEMP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color ex
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetColorTempEx_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetRGB(pInstance, eWindow, bUseYUVSpace , pstColorTemp->cRedColor, pstColorTemp->cGreenColor, pstColorTemp->cBlueColor );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetColorTempEx( MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetColorTempEx ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETCOLORTEMPEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color 10Bits step length
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetColorTemp_10Bits_U2(void *pInstance,MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetRGB_10Bits(pInstance, eWindow, bUseYUVSpace , pstColorTemp->cRedColor, pstColorTemp->cGreenColor, pstColorTemp->cBlueColor );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetColorTemp_10Bits( MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetColorTempEx ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETCOLORTEMP_10BITS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///- set post color temp Version 2
///@param eWindow \b IN: Enable Color temp
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetPostColorTemp_V02_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex2* pstColorTemp)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
    memcpy(&(pACEResourceStr->stColorTemp),pstColorTemp,sizeof(XC_ACE_color_temp_ex2));
#endif
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetPostRGBGainOffset(pInstance, eWindow,
                                     pstColorTemp->cRedColor,  pstColorTemp->cGreenColor,  pstColorTemp->cBlueColor,
                                     pstColorTemp->cRedOffset, pstColorTemp->cGreenOffset, pstColorTemp->cBlueOffset, 0);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetPostColorTemp_V02(MS_BOOL eWindow, XC_ACE_color_temp_ex2* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetPostColorTemp_V02 ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETPOSTCOLORTEMP_V02, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-obosolte!! use MApi_XC_ACE_PicSetPostColorTemp_V02 instead
//*************************************************************************
void MApi_XC_ACE_PicSetPostColorTemp_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp* pstColorTemp)
{
    XC_ACE_color_temp_ex2 stColorTemp2;
    stColorTemp2.cRedOffset = pstColorTemp->cRedOffset<<3 ;
    stColorTemp2.cGreenOffset= pstColorTemp->cGreenOffset<<3;
    stColorTemp2.cBlueOffset= pstColorTemp->cBlueOffset<<3;

    stColorTemp2.cRedColor= pstColorTemp->cRedColor << 3;
    stColorTemp2.cGreenColor= pstColorTemp->cGreenColor << 3;
    stColorTemp2.cBlueColor= pstColorTemp->cBlueColor << 3;

    stColorTemp2.cRedScaleValue= pstColorTemp->cRedScaleValue;
    stColorTemp2.cGreenScaleValue= pstColorTemp->cGreenScaleValue;
    stColorTemp2.cBlueScaleValue= pstColorTemp->cBlueScaleValue;

    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetPostRGBGain(pInstance, eWindow, stColorTemp2.cRedColor, stColorTemp2.cGreenColor, stColorTemp2.cBlueColor);
    MDrv_ACE_PicSetPostRGBOffset(pInstance, eWindow, stColorTemp2.cRedOffset, stColorTemp2.cGreenOffset, stColorTemp2.cBlueOffset, 0);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetPostColorTemp(MS_BOOL eWindow, XC_ACE_color_temp* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetPostColorTemp ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETPOSTCOLORTEMP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-obosolte!! use MApi_XC_ACE_PicSetPostColorTemp_V02 instead
//*************************************************************************
void MApi_XC_ACE_PicSetPostColorTempEx_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex* pstColorTemp)
{
    XC_ACE_color_temp_ex2 stColorTemp2;
    stColorTemp2.cRedOffset = pstColorTemp->cRedOffset ;
    stColorTemp2.cGreenOffset= pstColorTemp->cGreenOffset;
    stColorTemp2.cBlueOffset= pstColorTemp->cBlueOffset;

    stColorTemp2.cRedColor= pstColorTemp->cRedColor << 3 ;
    stColorTemp2.cGreenColor= pstColorTemp->cGreenColor << 3;
    stColorTemp2.cBlueColor= pstColorTemp->cBlueColor << 3;

    stColorTemp2.cRedScaleValue= pstColorTemp->cRedScaleValue;
    stColorTemp2.cGreenScaleValue= pstColorTemp->cGreenScaleValue;
    stColorTemp2.cBlueScaleValue= pstColorTemp->cBlueScaleValue;

    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_PicSetPostRGBGain(pInstance, eWindow, stColorTemp2.cRedColor, stColorTemp2.cGreenColor, stColorTemp2.cBlueColor);
    MDrv_ACE_PicSetPostRGBOffset(pInstance, eWindow, stColorTemp2.cRedOffset, stColorTemp2.cGreenOffset, stColorTemp2.cBlueOffset, 0);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetPostColorTempEx(MS_BOOL eWindow, XC_ACE_color_temp_ex* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetPostColorTempEx ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETPOSTCOLORTEMPEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetPostColorTemp2_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp)
{
    XC_ACE_color_temp_ex2 stColorTemp2;

    // error handle & data prepare
    if(pstColorTemp == NULL)
        return;

    stColorTemp2.cRedOffset = pstColorTemp->cRedOffset<<3 ;
    stColorTemp2.cGreenOffset= pstColorTemp->cGreenOffset<<3;
    stColorTemp2.cBlueOffset= pstColorTemp->cBlueOffset<<3;

    stColorTemp2.cRedColor= pstColorTemp->cRedColor << 3;
    stColorTemp2.cGreenColor= pstColorTemp->cGreenColor << 3;
    stColorTemp2.cBlueColor= pstColorTemp->cBlueColor << 3;

    stColorTemp2.cRedScaleValue= pstColorTemp->cRedScaleValue;
    stColorTemp2.cGreenScaleValue= pstColorTemp->cGreenScaleValue;
    stColorTemp2.cBlueScaleValue= pstColorTemp->cBlueScaleValue;

    _ACE_SEMAPHORE_ENTRY(pInstance);

    XC_ACE_ENTRY();
    // bypass ACE color setting
    MDrv_ACE_PicSetRGB(pInstance, eWindow, bUseYUVSpace , XC_ACE_BYPASS_COLOR_GAIN, XC_ACE_BYPASS_COLOR_GAIN, XC_ACE_BYPASS_COLOR_GAIN );

    // use HW color gain setting
    MDrv_ACE_PicSetPostRGBGain(pInstance, eWindow, stColorTemp2.cRedColor, stColorTemp2.cGreenColor, stColorTemp2.cBlueColor);
    MDrv_ACE_PicSetPostRGBOffset(pInstance, eWindow, stColorTemp2.cRedOffset, stColorTemp2.cGreenOffset, stColorTemp2.cBlueOffset, 0);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetPostColorTemp2(MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetPostColorTemp ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETPOSTCOLORTEMP2, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color ex
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_PicSetPostColorTemp2Ex_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    XC_ACE_color_temp_ex2 stColorTemp2;

    // error handle & data prepare
    if(pstColorTemp == NULL)
        return;

    stColorTemp2.cRedOffset = pstColorTemp->cRedOffset ;
    stColorTemp2.cGreenOffset= pstColorTemp->cGreenOffset;
    stColorTemp2.cBlueOffset= pstColorTemp->cBlueOffset;

    stColorTemp2.cRedColor= pstColorTemp->cRedColor << 3 ;
    stColorTemp2.cGreenColor= pstColorTemp->cGreenColor << 3;
    stColorTemp2.cBlueColor= pstColorTemp->cBlueColor << 3;

    stColorTemp2.cRedScaleValue= pstColorTemp->cRedScaleValue;
    stColorTemp2.cGreenScaleValue= pstColorTemp->cGreenScaleValue;
    stColorTemp2.cBlueScaleValue= pstColorTemp->cBlueScaleValue;

    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    // bypass ACE color setting
    MDrv_ACE_PicSetRGB(pInstance, eWindow, bUseYUVSpace , XC_ACE_BYPASS_COLOR_GAIN, XC_ACE_BYPASS_COLOR_GAIN, XC_ACE_BYPASS_COLOR_GAIN );

    // use HW color gain setting
    MDrv_ACE_PicSetPostRGBGain(pInstance, eWindow, stColorTemp2.cRedColor, stColorTemp2.cGreenColor, stColorTemp2.cBlueColor);
    MDrv_ACE_PicSetPostRGBOffset(pInstance, eWindow, stColorTemp2.cRedOffset, stColorTemp2.cGreenOffset, stColorTemp2.cBlueOffset, 0);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_PicSetPostColorTemp2Ex(MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_PicSetPostColorTempEx ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.pstColorTemp = pstColorTemp;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_PICSETPOSTCOLORTEMP2EX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
/*                     Local                                                  */
/* ****************************************************************************/

E_XC_ACE_RESULT MApi_XC_ACE_GetLibVer_U2(void *pInstance, const MSIF_Version **ppVersion)              // Get version
{
    if (!ppVersion)
    {
        return E_XC_ACE_FAIL;
    }

    *ppVersion = &_api_xc_ace_version;
    return E_XC_ACE_OK;
}

E_XC_ACE_RESULT MApi_XC_ACE_GetLibVer(const MSIF_Version **ppVersion)              // Get version
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return E_XC_ACE_FAIL;
    }

    stACE_GetLibVer ACEArgs;
    ACEArgs.ppVersion = ppVersion;
    ACEArgs.eResult = E_XC_ACE_FAIL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETLIBVER, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        return  ACEArgs.eResult;
    }
}

const XC_ACE_ApiInfo * MApi_XC_ACE_GetInfo_U2(void *pInstance)          // Get info from driver
{
    return &_stXC_ACE_ApiInfo;
}

const XC_ACE_ApiInfo * MApi_XC_ACE_GetInfo(void)          // Get info from driver
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return NULL;
    }

    stACE_GetInfo ACEArgs;
    ACEArgs.pstApiInfo = NULL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETINFO, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return NULL;
    }
    else
    {
        return ACEArgs.pstApiInfo;
    }
}

MS_BOOL MApi_XC_ACE_GetStatus_U2(void *pInstance, XC_ACE_ApiStatus *pDrvStatus, MS_BOOL bWindow)    // Get DLC current status
{
    if(bWindow == 0)
    {
        _ACE_SEMAPHORE_ENTRY(pInstance);
        _ACE_GET_VARIABLE();
        pDrvStatus->ACEinit = g_XC_ACE_InitData;
        _ACE_SEMAPHORE_RETURN(pInstance);
    }
    return TRUE;
}

MS_BOOL MApi_XC_ACE_GetStatus(XC_ACE_ApiStatus *pDrvStatus, MS_BOOL bWindow)    // Get DLC current status
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_GetStatus ACEArgs;
    ACEArgs.pDrvStatus = pDrvStatus;
    ACEArgs.bWindow = bWindow;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_GETSTATUS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_SetDbgLevel_U2(void *pInstance, MS_U16 u16DbgSwitch)
{
#ifdef XC_ACE_DBG
    _u16XC_ACEDbgSwitch = u16DbgSwitch;
#endif
    return TRUE;
}

MS_BOOL MApi_XC_ACE_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if (pu32ACEInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("ACE not initialized!\n");
            return FALSE;
        }
    }
    stACE_SetDbgLevel ACEArgs;
    ACEArgs.u16DbgSwitch = u16DbgSwitch;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETDBGLEVEL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/

void MApi_XC_ACE_EnableMWE_U2(void *pInstance, MS_BOOL bEnable)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    MDrv_XC_SetMWEStatus(pInstance, bEnable);
    XC_ACE_ENTRY();
    MDrv_XC_ACE_MWEEnable(pInstance, bEnable, _bMWESkipDriverSetting);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_EnableMWE(MS_BOOL bEnable)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_EnableMWE ACEArgs;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_ENABLEMWE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_MWESetMirrorMode_U2(void *pInstance, E_XC_ACE_MWE_MIRROR_TYPE eMirrorMode)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    _enMirrorMode = eMirrorMode;
    _ACE_SEMAPHORE_RETURN(pInstance);

}

void MApi_XC_ACE_MWESetMirrorMode( E_XC_ACE_MWE_MIRROR_TYPE eMirrorMode )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_MWESetMirrorMode ACEArgs;
    ACEArgs.eMirrorMode = eMirrorMode;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWESETMIRRORMODE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_MWECloneVisualEffect_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    _ACE_GET_VARIABLE();
    if(!_bMWESkipDriverSetting)
    {
        MDrv_XC_ACE_MWECloneVisualEffect(pInstance);
    }
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_MWECloneVisualEffect(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWECLONEVISUALEFFECT, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_3DClonePQMap_U2(void *pInstance, XC_ACE_WEAVETYPE enWeaveType)
{
    XC_ACE_ENTRY();
    MDrv_ACE_3DClonePQMap(pInstance, enWeaveType);
    XC_ACE_RETURN();
}

void MApi_XC_ACE_3DClonePQMap(XC_ACE_WEAVETYPE enWeaveType)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_3DClonePQMap ACEArgs;
    ACEArgs.enWeaveType = enWeaveType;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_3DCLONEPQMAP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_MWESetRegTable_U2(void *pInstance, XC_ACE_TAB_Info *pMWETable)
{
    MS_U32 u32Addr;
    MS_U8  u8Mask;
    MS_U8  u8Value;
    MS_U16 i;
    MS_U8  u8DoNotSet;
    if((NULL == pMWETable->pTable) || (pMWETable->u8TabRows < 2))
    {
        XC_ACE_DBG_MSG("Invalid Table Format!\n");
        return FALSE;
    }

    if((ACE_TABINFO_REG_ADDR_SIZE + ACE_TABINFO_REG_MASK_SIZE + pMWETable->u8TabIdx * ACE_TABINFO_REG_DATA_SIZE) >= pMWETable->u8TabCols)
    {
        XC_ACE_DBG_MSG("Invalid Table Format!\n");
        return FALSE;
    }

    for(i=0; i < pMWETable->u8TabRows - 1; i++)
    {
        u32Addr = (MS_U32)((pMWETable->pTable[0] << 8) + pMWETable->pTable[1]);
        u8Mask  = pMWETable->pTable[ACE_TABINFO_REG_ADDR_SIZE];
        u8DoNotSet = pMWETable->pTable[ACE_TABINFO_REG_ADDR_SIZE + ACE_TABINFO_REG_MASK_SIZE + pMWETable->u8TabIdx * ACE_TABINFO_REG_DATA_SIZE];
        u8Value = pMWETable->pTable[ACE_TABINFO_REG_ADDR_SIZE + ACE_TABINFO_REG_MASK_SIZE + pMWETable->u8TabIdx * ACE_TABINFO_REG_DATA_SIZE + 1];
        XC_ACE_DBG_MSG("[addr=%04x, msk=%02x, enb=%02x, val=%02x]\n", u32Addr, u8Mask, u8DoNotSet, u8Value);
        if(u32Addr == 0xFFFF) //check end of table
        {
            XC_ACE_DBG_MSG("end of table!\n");
            break;
        }
        if(!u8DoNotSet)
        {
            XC_ACE_ENTRY();
            MDrv_XC_ACE_MWESetRegRow(pInstance, u32Addr, u8Value, u8Mask);
            XC_ACE_RETURN();
        }
        pMWETable->pTable += pMWETable->u8TabCols; //next
    }
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    _bMWESkipDriverSetting = TRUE;
    _ACE_SEMAPHORE_RETURN(pInstance);
    return TRUE;
}

MS_BOOL MApi_XC_ACE_MWESetRegTable(XC_ACE_TAB_Info *pMWETable)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_MWESetRegTable ACEArgs;
    ACEArgs.pMWETable = pMWETable;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWESETREGTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_SetMWELoadFromPQ_U2(void *pInstance, MS_BOOL bEnable)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    _bMWESkipDriverSetting = bEnable;
    _ACE_SEMAPHORE_RETURN(pInstance);
    return TRUE;
}

MS_BOOL MApi_XC_ACE_SetMWELoadFromPQ(MS_BOOL bEnable)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_SetMWELoadFromPQ ACEArgs;
    ACEArgs.bEnable = bEnable;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETMWELOADFROMPQ, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_MWESetDispWin_U2(void *pInstance, MS_U16 u16MWE_Disp_Hstart, MS_U16 u16MWE_Disp_Vstart, MS_U16 u16MWE_Disp_Width, MS_U16 u16MWE_Disp_Height)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    g_XC_ACE_InitData.u16MWE_Disp_Hstart = u16MWE_Disp_Hstart;
    g_XC_ACE_InitData.u16MWE_Disp_Vstart = u16MWE_Disp_Vstart;
    g_XC_ACE_InitData.u16MWE_Disp_Width  = u16MWE_Disp_Width;
    g_XC_ACE_InitData.u16MWE_Disp_Height = u16MWE_Disp_Height;
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_MWESetDispWin(MS_U16 u16MWE_Disp_Hstart, MS_U16 u16MWE_Disp_Vstart, MS_U16 u16MWE_Disp_Width, MS_U16 u16MWE_Disp_Height)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_MWESetDispWin ACEArgs;
    ACEArgs.u16MWE_Disp_Hstart = u16MWE_Disp_Hstart;
    ACEArgs.u16MWE_Disp_Vstart = u16MWE_Disp_Vstart;
    ACEArgs.u16MWE_Disp_Width = u16MWE_Disp_Width;
    ACEArgs.u16MWE_Disp_Height = u16MWE_Disp_Height;

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWESETDISPWIN, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_MWEStatus_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    MS_BOOL bMWEStatusRet = MDrv_XC_GetMWEStatus(pInstance);
    _ACE_SEMAPHORE_RETURN(pInstance);
    return bMWEStatusRet;
}

MS_BOOL MApi_XC_ACE_MWEStatus(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_MWEStatus ACEArgs;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWESTATUS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_MWEFuncSel_U2(void *pInstance, MS_BOOL eWindow, E_XC_ACE_MWE_FUNC  mwe_func)
{
    MS_U8 u8border_color;
    MS_U16 u16border_l, u16border_r, u16border_t, u16border_d;
    MS_BOOL border_en;
    MS_S16 s16HShiftStep;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    if (mwe_func == E_XC_ACE_MWE_MODE_OFF && _MWEFunc == E_XC_ACE_MWE_MODE_OFF)
    {
        _ACE_SEMAPHORE_RETURN(pInstance);
        return;
    }

    XC_ACE_ENTRY();

    _MWEFunc = mwe_func;
    u16border_l = 0x00;
    u16border_r = 0x00;
    u16border_t = 0x00;
    u16border_d = 0x00;

    switch (_MWEFunc)
    {
    default:
    case E_XC_ACE_MWE_MODE_OFF:
        gMWERect.Xsize = 0xFFF;
        gMWERect.Ysize = 0xFFF;
        gMWERect.Xpos = 0;
        gMWERect.Ypos = 0;
        u8border_color = MWE_BORDER_COLOR_BLACK;
        border_en = DISABLE;
        break;

    case E_XC_ACE_MWE_MODE_H_SCAN:
    case E_XC_ACE_MWE_MODE_H_SPLIT:

        if( _MWEFunc == E_XC_ACE_MWE_MODE_H_SPLIT )
        {
            gMWERect.Xpos = 0;
            gMWERect.Ypos = 0;
        }

        XC_ACE_DBG_MSG(" [MWE] H-SPLIT-RIGHT/H-SCAN Function \n");
        XC_ACE_DBG_MSG("[MWE]_in_panel;_enMirrorMode=%u,Ds-x[%u],Ds-y[%u],Ds-w[%u],Ds-h[%u].\n",_enMirrorMode,
            g_XC_ACE_InitData.u16MWEHstart,g_XC_ACE_InitData.u16MWEVstart,    g_XC_ACE_InitData.u16MWEWidth,g_XC_ACE_InitData.u16MWEHeight);
        XC_ACE_DBG_MSG("[MWE]_in_buffer;_enMirrorMode=%u,Ds-x[%u],Ds-y[%u],Ds-w[%u],Ds-h[%u].\n",_enMirrorMode,
                                    gMWERect.Xpos,gMWERect.Ypos,gMWERect.Xsize,gMWERect.Ysize);
        XC_ACE_DBG_MSG("[MWE]_in_disp;_enMirrorMode=%u,Ds-x[%u],Ds-y[%u],Ds-w[%u],Ds-h[%u].\n",_enMirrorMode,
                                    g_XC_ACE_InitData.u16MWE_Disp_Hstart,g_XC_ACE_InitData.u16MWE_Disp_Vstart,g_XC_ACE_InitData.u16MWE_Disp_Width/2,g_XC_ACE_InitData.u16MWE_Disp_Height);

        //for fix H shift step when Display is 1/2.
        s16HShiftStep = g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth;


        //w and h size
        if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_HV)
        {
            //for fix screen shift function in contrast expander function
            if((abs(g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth)) <= 10)
            {
                if (g_XC_ACE_InitData.u16MWE_Disp_Hstart == 0)
                    gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2;
                else
                    gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2 + s16HShiftStep;
            }
            else
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/2;
        }
        else
        {
            //for fix screen shift function in contrast expander function
            if((abs(g_XC_ACE_InitData.u16MWE_Disp_Width - g_XC_ACE_InitData.u16MWEWidth)) <= 10)
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWEWidth/2 + s16HShiftStep;
            else
                gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/2;
        }

        gMWERect.Ysize = g_XC_ACE_InitData.u16MWE_Disp_Height;

        //x and y start position
        if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_NORMAL || _enMirrorMode == E_XC_ACE_MWE_MIRROR_V_ONLY)
        {
            gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
            //for patch 4:3 in contrast expander function
            if (g_XC_ACE_InitData.u16MWEWidth > g_XC_ACE_InitData.u16MWE_Disp_Hstart)
                gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth/2;
            else
                gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart + g_XC_ACE_InitData.u16MWEWidth/2;
        }
        else if(_enMirrorMode == E_XC_ACE_MWE_MIRROR_HV)
        {
            if( g_XC_ACE_InitData.u16MWEWidth > ( g_XC_ACE_InitData.u16MWE_Disp_Hstart + g_XC_ACE_InitData.u16MWE_Disp_Width) )
            {
                //for patch 4:3 in contrast expander function
                if (g_XC_ACE_InitData.u16MWEWidth > g_XC_ACE_InitData.u16MWE_Disp_Hstart)
                    gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth - ( gMWERect.Xpos + g_XC_ACE_InitData.u16MWE_Disp_Width ) - g_XC_ACE_InitData.u16MWE_Disp_Hstart;
                else
                    gMWERect.Xpos = g_XC_ACE_InitData.u16MWEWidth - ( gMWERect.Xpos + g_XC_ACE_InitData.u16MWE_Disp_Width );
            }
            else
            {
                gMWERect.Xpos = 0;
            }

            if( g_XC_ACE_InitData.u16MWEHeight > ( g_XC_ACE_InitData.u16MWE_Disp_Vstart + g_XC_ACE_InitData.u16MWE_Disp_Height) )
            {
                //for patch smart view in contrast expander function
                if ((g_XC_ACE_InitData.u16MWE_Disp_Vstart != 0) && (g_XC_ACE_InitData.u16MWE_Disp_Height != g_XC_ACE_InitData.u16MWEHeight))
                    gMWERect.Ypos = g_XC_ACE_InitData.u16MWEHeight- ( gMWERect.Ypos + g_XC_ACE_InitData.u16MWE_Disp_Height ) - g_XC_ACE_InitData.u16MWE_Disp_Vstart;
                else
                    gMWERect.Ypos = g_XC_ACE_InitData.u16MWEHeight - ( gMWERect.Ypos + g_XC_ACE_InitData.u16MWE_Disp_Height );
            }
            else
            {
                gMWERect.Ypos = 0;
            }
        }
        else
        {
            gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart+g_XC_ACE_InitData.u16MWE_Disp_Width - gMWERect.Xsize;
            gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
        }

        XC_ACE_DBG_MSG("[MWE]_out2;_enMirrorMode=%u,Ds-x[%u],Ds-y[%u],Ds-w[%u],Ds-h[%u].\n",_enMirrorMode,gMWERect.Xpos,gMWERect.Ypos,
                                                    gMWERect.Xsize,gMWERect.Ysize);

        u8border_color = MWE_BORDER_COLOR_BLUE;
        u16border_l = 0x02;
        border_en = ENABLE;
        break;

    case E_XC_ACE_MWE_MODE_H_SPLIT_LEFT:
        ACE_DBG_MSG(" [MWE] H-SPLIT-LEFT\n");
        gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width / 2;
        gMWERect.Ysize = g_XC_ACE_InitData.u16MWE_Disp_Height;
        gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart;
        gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart;
        u8border_color = MWE_BORDER_COLOR_BLUE;
        u16border_l = 0x02;
        border_en = ENABLE;
        break;

    case E_XC_ACE_MWE_MODE_MOVE:
        gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/3;
        gMWERect.Ysize =g_XC_ACE_InitData.u16MWE_Disp_Height/3;
        gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart+g_XC_ACE_InitData.u16MWE_Disp_Width - gMWERect.Xsize;
        gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
        u8border_color = MWE_BORDER_COLOR_BLUE;
        u16border_l = 0x11;
        u16border_r = 0x11;
        u16border_t = 0x11;
        u16border_d = 0x11;
        border_en = ENABLE;
        break;

    case E_XC_ACE_MWE_MODE_ZOOM:
        gMWERect.Xsize = g_XC_ACE_InitData.u16MWE_Disp_Width/2;
        gMWERect.Ysize = g_XC_ACE_InitData.u16MWE_Disp_Height/2;
        gMWERect.Xpos = g_XC_ACE_InitData.u16MWE_Disp_Hstart+g_XC_ACE_InitData.u16MWE_Disp_Width - gMWERect.Xsize;
        gMWERect.Ypos = g_XC_ACE_InitData.u16MWE_Disp_Vstart+g_XC_ACE_InitData.u16MWE_Disp_Height - gMWERect.Ysize;
        u8border_color = MWE_BORDER_COLOR_BLUE;
        border_en = DISABLE;
        break;
    }

    //ToDo: need to use drvPQ to adjsut picture quality
    MDrv_XC_ACE_MWESetWin(pInstance, &gMWERect);
    MDrv_XC_ACE_MWESetBorder(pInstance, eWindow, border_en, u8border_color,
        u16border_l, u16border_r, u16border_t, u16border_d);
    _ACE_SEMAPHORE_RETURN(pInstance);

    XC_ACE_RETURN();
}

void MApi_XC_ACE_MWEFuncSel( MS_BOOL eWindow, E_XC_ACE_MWE_FUNC  mwe_func)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_MWEFuncSel ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.mwe_func = mwe_func;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWEFUNCSEL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

static void MApi_XC_ACE_MWEScan(void *pInstance)
{
    MS_U32 xpos;
    MS_U32 MWE_MAX_XPOS, MWE_MIN_XPOS;
    MWE_RECT *pRect;
    _ACE_GET_VARIABLE();

    pRect = &gMWERect;

    MWE_MIN_XPOS = g_XC_ACE_InitData.u16MWE_Disp_Hstart;
    MWE_MAX_XPOS = MWE_MIN_XPOS + g_XC_ACE_InitData.u16MWE_Disp_Width -1;
    xpos  = pRect->Xpos;

    if(_MWEFlag& MWE_MOVE_RIGHT)
    {
        if( (xpos  + MWE_H_STEP) > ((MS_U32)MWE_MAX_XPOS))
        {
            xpos = MWE_MAX_XPOS;
            _MWEFlag &= ~MWE_MOVE_RIGHT;
        }
        else
        {
            xpos += MWE_H_STEP;
        }
    }
    else
    {
        if(xpos > MWE_H_STEP)
        {
            if ( (xpos - MWE_H_STEP) > ((MS_U32)MWE_MIN_XPOS))
            {
                xpos -= MWE_H_STEP;
            }
            else
            {
                xpos = MWE_MIN_XPOS;
                _MWEFlag |= MWE_MOVE_RIGHT;
            }
        }
        else
        {
            _MWEFlag |= MWE_MOVE_RIGHT;
        }
    }

    if (xpos > pRect->Xpos)
    {
        pRect->Xsize -= (xpos-pRect->Xpos);
    }
    else
    {
        pRect->Xsize += (pRect->Xpos-xpos);
    }
    pRect->Xpos = xpos;

    ACE_DBG_MSG(" HScan %u, %u\n", pRect->Xpos, pRect->Xsize);

}

void MApi_XC_ACE_MWEHandle_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    if (_MWEFunc != E_XC_ACE_MWE_MODE_H_SCAN)
        return;

    if ((MsOS_GetSystemTime()-_MWETimer) < MWE_TIMER)
        return;
    _MWETimer = MsOS_GetSystemTime();

    switch (_MWEFunc)
    {
    case E_XC_ACE_MWE_MODE_H_SCAN:
        MApi_XC_ACE_MWEScan(pInstance);
        break;
    case E_XC_ACE_MWE_MODE_MOVE:
    case E_XC_ACE_MWE_MODE_ZOOM:
    default:
        return;
    }

    XC_ACE_ENTRY();

    //MApi_XC_WaitOutputVSync(1, 50);
    MDrv_XC_ACE_MWESetWin(pInstance, &gMWERect);

    XC_ACE_RETURN();

    _ACE_SEMAPHORE_RETURN(pInstance);

}

void MApi_XC_ACE_MWEHandle( void )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_MWEHANDLE, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_Set_IHC_SRAM_U2(void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    MS_BOOL bRet;
    XC_ACE_ENTRY();

    bRet = MDrv_XC_ACE_Set_IHC_SRAM(pInstance, pBuf, u16ByteSize);

    XC_ACE_RETURN();

    return  bRet;
}

MS_BOOL MApi_XC_ACE_Set_IHC_SRAM(MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_Set_IHC_SRAM ACEArgs;
    ACEArgs.pBuf = pBuf;
    ACEArgs.u16ByteSize = u16ByteSize;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SET_IHC_SRAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_Set_ICC_SRAM_U2(void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    MS_BOOL bRet;
    XC_ACE_ENTRY();

    bRet = MDrv_XC_ACE_Set_ICC_SRAM(pInstance, pBuf, u16ByteSize);

    XC_ACE_RETURN();

    return  bRet;
}

MS_BOOL MApi_XC_ACE_Set_ICC_SRAM(MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_Set_ICC_SRAM ACEArgs;
    ACEArgs.pBuf = pBuf;
    ACEArgs.u16ByteSize = u16ByteSize;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SET_ICC_SRAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  DynamicNR start
//
////////////////////////////////////////////////////////////////////////////////
#define NR_DBG(x)   //x

#define DYNAMIC_NR_TBL_REG_NUM                  8
#define DYNAMICNR_DEFLICKER_PRECISION_SHFIT     64UL
#define DYNAMICNR_DEFLICK_TOTAL                 32UL
#define DYNAMICNR_DEFLICK_STABLE_CNT            0x10

#if 0
MS_U8  g_u8MotionStableCnt;
MS_U8  g_u8LumaStableCnt;
MS_U8  g_u8FlickStableCnt;
#endif

MS_U16 MApi_XC_ACE_DNR_Get_PrecisionShift_U2(void *pInstance)
{
    return DYNAMICNR_DEFLICKER_PRECISION_SHFIT;
}

MS_U16 MApi_XC_ACE_DNR_Get_PrecisionShift(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_Get_PrecisionShift ACEArgs;
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GET_PRECISIONSHIFT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return ACEArgs.u16Return;
    }
}

void MApi_XC_ACE_DNR_Blending_NRTbl_U2(
    void *pInstance,
    MS_U8 *pu8NewNR,
    MS_U8 u8Weight,
    MS_U8 u8Step,
    MS_U16 *pu16PreY0,
    MS_U16 *pu16PreY1,
    MS_U8 *pu8NRTBL)
{
    MS_U8 i;
    MS_U16 u16TmpBlend[2];

    NR_DBG(printk("CurNR:"));
    for(i=0; i<DYNAMIC_NR_TBL_REG_NUM; i++)
    {
        u16TmpBlend[0] = (MS_U16)(((((MS_U32)u8Weight * ((MS_U32)pu8NewNR[i] & 0x0FL)) * DYNAMICNR_DEFLICKER_PRECISION_SHFIT) +
                    ((MS_U32)DYNAMICNR_DEFLICK_TOTAL - (MS_U32)u8Weight) * (MS_U32)pu16PreY0[i]) / DYNAMICNR_DEFLICK_TOTAL);

        u16TmpBlend[1] = (MS_U16)(((((MS_U32)u8Weight * (((MS_U32)pu8NewNR[i] & 0xF0L)>>4)) * DYNAMICNR_DEFLICKER_PRECISION_SHFIT) +
                    ((MS_U32)DYNAMICNR_DEFLICK_TOTAL - (MS_U32)u8Weight) * (MS_U32)pu16PreY1[i]) / DYNAMICNR_DEFLICK_TOTAL);


        if(u16TmpBlend[0] > pu16PreY0[i])
        {
            if( (u16TmpBlend[0] - (MS_U16)pu16PreY0[i]) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
                u16TmpBlend[0]  = pu16PreY0[i] + ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
        }
        else if(u16TmpBlend[0] < (MS_U16)pu16PreY0[i])
        {
            if( ((MS_U16)pu16PreY0[i] - u16TmpBlend[0]) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
                u16TmpBlend[0]  = pu16PreY0[i] - ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
        }
        pu16PreY0[i] = u16TmpBlend[0];

        if(u16TmpBlend[1] > pu16PreY1[i])
        {
            if( (u16TmpBlend[1] - (MS_U16)pu16PreY1[i]) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
                u16TmpBlend[1]  = pu16PreY1[i] + ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
        }
        else if(u16TmpBlend[1] < (MS_U16)pu16PreY1[i])
        {
            if( ((MS_U16)pu16PreY1[i] - u16TmpBlend[1]) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
                u16TmpBlend[1]  = pu16PreY1[i] - ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
        }
        pu16PreY1[i] = u16TmpBlend[1];
        pu8NRTBL[i] = (MS_U8)(((u16TmpBlend[1] / DYNAMICNR_DEFLICKER_PRECISION_SHFIT)<<4) | (u16TmpBlend[0] / DYNAMICNR_DEFLICKER_PRECISION_SHFIT));

        NR_DBG(printk(" %02bx, ", pu8NRTBL[i]));
    }
    NR_DBG(printk("\n"));
}

void MApi_XC_ACE_DNR_Blending_NRTbl(
    MS_U8 *pu8NewNR,
    MS_U8 u8Weight,
    MS_U8 u8Step,
    MS_U16 *pu16PreY0,
    MS_U16 *pu16PreY1,
    MS_U8 *pu8NRTBL)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_DNR_Blending_NRTbl ACEArgs;
    ACEArgs.pu8NewNR = pu8NewNR;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.pu16PreY0 = pu16PreY0;
    ACEArgs.pu16PreY1 = pu16PreY1;
    ACEArgs.pu8NRTBL = pu8NRTBL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_BLENDING_NRTBL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U8 MApi_XC_ACE_DNR_Blending_U2(void *pInstance, MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight)
{
    MS_U8 u8Blend;

    u8Blend = (MS_U8)(((((MS_U32)u8Weight * (MS_U32)u8NewItem)) +
                ((MS_U32)DYNAMICNR_DEFLICK_TOTAL - (MS_U32)u8Weight) * (MS_U32)u16PreItem) / DYNAMICNR_DEFLICK_TOTAL);
    return u8Blend;
}

MS_U8 MApi_XC_ACE_DNR_Blending(MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_Blending ACEArgs;
    ACEArgs.u8NewItem = u8NewItem;
    ACEArgs.u16PreItem = u16PreItem;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_BLENDING, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U16 MApi_XC_ACE_DNR_Blending_MISC_U2(
    void *pInstance,
    MS_U8 u8NewItem,
    MS_U16 u16PreItem,
    MS_U8 u8Weight,
    MS_U8 u8Step)
{
    MS_U16 u16Blend;

    u16Blend = ((((MS_U32)u8Weight * (MS_U32)u8NewItem) * DYNAMICNR_DEFLICKER_PRECISION_SHFIT) +
                ((MS_U32)DYNAMICNR_DEFLICK_TOTAL - (MS_U32)u8Weight) * (MS_U32)u16PreItem) / DYNAMICNR_DEFLICK_TOTAL;


    if(u16Blend > u16PreItem)
    {
        if( (u16Blend - u16PreItem) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
            u16Blend  = u16PreItem + ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
    }
    else if(u16Blend < u16PreItem)
    {
        if( ((MS_U16)u16PreItem - u16Blend) > ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step))
            u16Blend  = u16PreItem - ((MS_U16)DYNAMICNR_DEFLICKER_PRECISION_SHFIT * (MS_U16) u8Step);
    }

    return u16Blend;
}

MS_U16 MApi_XC_ACE_DNR_Blending_MISC(
    MS_U8 u8NewItem,
    MS_U16 u16PreItem,
    MS_U8 u8Weight,
    MS_U8 u8Step)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_Blending_MISC ACEArgs;
    ACEArgs.u8NewItem = u8NewItem;
    ACEArgs.u16PreItem = u16PreItem;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.u8Step = u8Step;
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_BLENDING_MISC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u16Return;
    }
}

MS_U8 MApi_XC_ACE_DNR_GetMotion_U2(void *pInstance)
{
    MS_U8 u8Ret;

    XC_ACE_ENTRY();

    u8Ret =  MDrv_XC_ACE_DNR_GetMotion(pInstance);

    XC_ACE_RETURN();

    return u8Ret;
}

MS_U8 MApi_XC_ACE_DNR_GetMotion(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_GetMotion ACEArgs;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GETMOTION, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U8 MApi_XC_ACE_DNR_GetMotion_Weight_U2(
    void *pInstance,
    MS_U8 u8CurMotion,
    MS_U8 u8PreMotion,
    MS_U8 u8CurMotionLvl,
    MS_U8 u8PreMotionLvl,
    MS_U8 u8DeFlick_Thre,
    MS_U8 u8DeFilick_Step)
{
    MS_U8 u8Weight;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    if(u8CurMotion > u8PreMotion)
    {
        if((u8CurMotion - u8PreMotion) < u8DeFlick_Thre)
        {
            if((u8CurMotionLvl - u8PreMotionLvl) <= 1)
            {
                if(g_u8MotionStableCnt != 0xFF)
                    g_u8MotionStableCnt++;
            }
            else
            {
                g_u8MotionStableCnt = 0;
            }
            u8Weight = (g_u8MotionStableCnt > DYNAMICNR_DEFLICK_STABLE_CNT) ?
                       DYNAMICNR_DEFLICK_TOTAL : u8DeFlick_Thre;
        }
        else
        {
            g_u8MotionStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }
    else
    {
        if((u8PreMotion - u8CurMotion) < u8DeFilick_Step)
        {
            if((u8CurMotionLvl - u8PreMotionLvl) <= 1)
            {
                if(g_u8MotionStableCnt != 0xFF)
                    g_u8MotionStableCnt++;
            }
            else
            {
                g_u8MotionStableCnt = 0;
            }
            u8Weight = (g_u8MotionStableCnt > DYNAMICNR_DEFLICK_STABLE_CNT) ?
                       DYNAMICNR_DEFLICK_TOTAL : u8DeFilick_Step;
        }
        else
        {
            g_u8MotionStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }

    if(u8Weight > DYNAMICNR_DEFLICK_TOTAL)
        u8Weight = DYNAMICNR_DEFLICK_TOTAL;
    _ACE_SEMAPHORE_RETURN(pInstance);

    return u8Weight;
}

MS_U8 MApi_XC_ACE_DNR_GetMotion_Weight(
    MS_U8 u8CurMotion,
    MS_U8 u8PreMotion,
    MS_U8 u8CurMotionLvl,
    MS_U8 u8PreMotionLvl,
    MS_U8 u8DeFlick_Thre,
    MS_U8 u8DeFilick_Step)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_GetMotion_Weight ACEArgs;
    ACEArgs.u8CurMotion = u8CurMotion;
    ACEArgs.u8PreMotion = u8PreMotion;
    ACEArgs.u8CurMotionLvl = u8CurMotionLvl;
    ACEArgs.u8PreMotionLvl = u8CurMotionLvl;
    ACEArgs.u8DeFlick_Thre = u8DeFlick_Thre;
    ACEArgs.u8DeFilick_Step = u8DeFilick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GETMOTION_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U8  MApi_XC_ACE_DNR_GetLuma_Weight_U2(
    void *pInstance,
    MS_U8 u8CurAvgLuma,
    MS_U8 u8PreAvgLuam,
    MS_U8 u8CurLumaLvl,
    MS_U8 u8PreLumaLvl,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    MS_U8 u8Weight;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    // Blending for Coring and SNR
    if(u8CurAvgLuma > u8PreAvgLuam)
    {
        if((u8CurAvgLuma - u8PreAvgLuam) < u8DeFlick_Th)
        {
            if((u8CurLumaLvl - u8PreLumaLvl)<=1)
            {
                if(g_u8LumaStableCnt != 0xFF)
                    g_u8LumaStableCnt++;
            }
            else
            {
                g_u8LumaStableCnt = 0;
            }
            u8Weight = (g_u8LumaStableCnt > DYNAMICNR_DEFLICK_STABLE_CNT) ?
                       DYNAMICNR_DEFLICK_TOTAL :
                       u8DeFlick_Step;
        }
        else
        {
            g_u8LumaStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }
    else
    {
        if((u8PreAvgLuam - u8CurAvgLuma) < u8DeFlick_Th)
        {
            if((u8CurLumaLvl - u8PreLumaLvl)<=1)
            {
                if(g_u8LumaStableCnt != 0xFF)
                    g_u8LumaStableCnt++;
            }
            else
            {
                g_u8LumaStableCnt = 0;
            }
            u8Weight = (g_u8LumaStableCnt > DYNAMICNR_DEFLICK_STABLE_CNT) ?
                       DYNAMICNR_DEFLICK_TOTAL :
                       u8DeFlick_Step;
        }
        else
        {
            g_u8LumaStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }
    _ACE_SEMAPHORE_RETURN(pInstance);

    return u8Weight;
}

MS_U8  MApi_XC_ACE_DNR_GetLuma_Weight(
    MS_U8 u8CurAvgLuma,
    MS_U8 u8PreAvgLuam,
    MS_U8 u8CurLumaLvl,
    MS_U8 u8PreLumaLvl,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_GetLuma_Weight ACEArgs;
    ACEArgs.u8CurAvgLuma = u8CurAvgLuma;
    ACEArgs.u8PreAvgLuam = u8PreAvgLuam;
    ACEArgs.u8CurLumaLvl = u8CurLumaLvl;
    ACEArgs.u8PreLumaLvl = u8PreLumaLvl;
    ACEArgs.u8DeFlick_Th = u8DeFlick_Th;
    ACEArgs.u8DeFlick_Step = u8DeFlick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GETLUMA_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U8 MApi_XC_ACE_DNR_GetNoise_Weight_U2(
    void *pInstance,
    MS_U8 u8CurNoise,
    MS_U8 u8PreNoise,
    MS_U8 u8Range,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    MS_U8 u8Weight;
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();

    if(u8CurNoise > u8PreNoise)
    {
        if((u8CurNoise - u8PreNoise) < (u8DeFlick_Th))
        {
            g_u8FlickStableCnt =
                ((u8CurNoise - u8PreNoise) < u8Range) ?
                g_u8FlickStableCnt + 1 : 0;

            u8Weight = (g_u8FlickStableCnt == DYNAMICNR_DEFLICK_STABLE_CNT) ?
                        DYNAMICNR_DEFLICK_TOTAL :
                        u8DeFlick_Step;
        }
        else
        {
            g_u8FlickStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }
    else
    {
        if((u8PreNoise - u8CurNoise) < (u8DeFlick_Th))
        {
            g_u8FlickStableCnt =
                ((u8CurNoise - u8PreNoise) < u8Range) ?
                g_u8FlickStableCnt + 1 : 0;
            u8Weight = (g_u8FlickStableCnt == DYNAMICNR_DEFLICK_STABLE_CNT) ?
                        DYNAMICNR_DEFLICK_TOTAL :
                        u8DeFlick_Step;
        }
        else
        {
            g_u8FlickStableCnt = 0;
            u8Weight = DYNAMICNR_DEFLICK_TOTAL;
        }
    }

    if(u8Weight > DYNAMICNR_DEFLICK_TOTAL)
        u8Weight = DYNAMICNR_DEFLICK_TOTAL;
    if(u8Weight == 0)
        u8Weight = 1;
    _ACE_SEMAPHORE_RETURN(pInstance);

    return u8Weight;
}

MS_U8 MApi_XC_ACE_DNR_GetNoise_Weight(
    MS_U8 u8CurNoise,
    MS_U8 u8PreNoise,
    MS_U8 u8Range,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_GetNoise_Weight ACEArgs;
    ACEArgs.u8CurNoise = u8CurNoise;
    ACEArgs.u8PreNoise = u8PreNoise;
    ACEArgs.u8Range = u8Range;
    ACEArgs.u8Range = u8Range;
    ACEArgs.u8DeFlick_Th = u8DeFlick_Th;
    ACEArgs.u8DeFlick_Step = u8DeFlick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GETNOISE_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

void MApi_XC_ACE_DNR_Init_Motion_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    g_u8MotionStableCnt = 0;
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_DNR_Init_Motion(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_INIT_MOTION, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_DNR_Init_Luma_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    g_u8LumaStableCnt = 0;
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_DNR_Init_Luma(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_INIT_LUMA, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_DNR_Init_Noise_U2(void *pInstance)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    _ACE_GET_VARIABLE();
    g_u8FlickStableCnt = 0;
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_DNR_Init_Noise(void)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_INIT_NOISE, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U8 MApi_XC_ACE_DNR_GetParam_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam)
{
    MS_U8 u8val;

    XC_ACE_ENTRY();

    switch(eParam)
    {
    case E_ACE_DNR_PEAKING_CORING_THRESHOLD:
        u8val = MDrv_ACE_DNR_GetCoringThreshold(pInstance, eWindow);
        break;

    case E_ACE_DNR_SHARPNESS_ADJUST:
        u8val = MDrv_ACE_DNR_GetSharpnessAdjust(pInstance, eWindow);
        break;

    case E_ACE_DNR_GUASSIN_SNR_THRESHOLD:
        u8val = MDrv_ACE_DNR_GetGuassin_SNR_Threshold(pInstance, eWindow);
        break;

    default:
        ACE_DBG_MSG("XC_ACE_DNR: unknown get param %u\n", eParam);
        u8val = 0;
        break;
    }

    XC_ACE_RETURN();

    return u8val;
}

MS_U8 MApi_XC_ACE_DNR_GetParam(MS_BOOL eWindow, XC_ACE_DNR_Param eParam)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return 0;
    }

    stACE_DNR_GetParam ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.eParam = eParam;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_GETPARAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

void MApi_XC_ACE_DNR_SetParam_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam, MS_U16 u16val)
{
    XC_ACE_ENTRY();

    switch(eParam)
    {
    case E_ACE_DNR_PEAKING_BANK_CORING:
        MDrv_ACE_DNR_SetBank_Coring(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_GUASSIN_SNR_THRESHOLD:
        MDrv_ACE_DNR_SetGuassin_SNR_Threshold(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_PEAKING_CORING_THRESHOLD:
        MDrv_ACE_DNR_SetCoringThreshold(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_SHARPNESS_ADJUST:
        MDrv_ACE_DNR_SetSharpnessAdjust(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_NM_V:
        MDrv_ACE_DNR_SetNM_V(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_GNR_0:
        MDrv_ACE_DNR_SetGNR_0(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_GNR_1:
        MDrv_ACE_DNR_SetGNR_1(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_CP:
        MDrv_ACE_DNR_SetCP(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_DP:
        MDrv_ACE_DNR_SetDP(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_NM_H_0:
        MDrv_ACE_DNR_SetNM_H_0(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_NM_H_1:
        MDrv_ACE_DNR_SetNM_H_1(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_GRAY_GROUND_GAIN:
        MDrv_ACE_DNR_SetGray_Ground_Gain(pInstance, u16val);
        break;

    case E_ACE_DNR_GRAY_GROUND_EN:
        MDrv_ACE_DNR_SetGray_Ground_En(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_SC_CORING:
        MDrv_ACE_DNR_SetSC_Coring(pInstance, eWindow, u16val);
        break;

    case E_ACE_DNR_SPIKE_NR_0:
        MDrv_ACE_DNR_SetSpikeNR_0(pInstance, u16val);
        break;

    case E_ACE_DNR_SPIKE_NR_1:
        MDrv_ACE_DNR_SetSpikeNR_1(pInstance, u16val);
        break;

    case E_ACE_DNR_SNR_NM:
        MDrv_ACE_DNR_SetSNR_NM(pInstance, u16val);
        break;

    default:
        ACE_DBG_MSG("XC_ACE_DNR: unknown set Param %u\n", eParam);
        break;
    }

    XC_ACE_RETURN();
}

void MApi_XC_ACE_DNR_SetParam(MS_BOOL eWindow, XC_ACE_DNR_Param eParam, MS_U16 u16val)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_DNR_SetParam ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.eParam = eParam;
    ACEArgs.u16val = u16val;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_SETPARAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_DNR_SetNRTbl_U2(void *pInstance, MS_U8 *pu8Tbl)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_DNR_SetNRTable(pInstance, pu8Tbl);
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_DNR_SetNRTbl(MS_U8 *pu8Tbl)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_DNR_SetNRTbl ACEArgs;
    ACEArgs.pu8Tbl = pu8Tbl;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_DNR_SETNRTBL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_ColorCorrectionTable_U2(void *pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_ColorCorrectionTable(pInstance, bScalerWin, psColorCorrectionTable );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_ColorCorrectionTable( MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_ColorCorrectionTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.psColorCorrectionTable = psColorCorrectionTable;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_COLORCORRECTIONTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetColorMatrixControl_U2(void *pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SetColorMatrixControl(pInstance, bScalerWin, bEnable );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_SetColorMatrixControl( MS_BOOL bScalerWin, MS_BOOL bEnable )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetColorMatrixControl ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETCOLORMATRIXCONTROL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_SetRBChannelRange_U2(void *pInstance, MS_BOOL bScalerWin, MS_BOOL bRange)
{
    _ACE_SEMAPHORE_ENTRY(pInstance);
    XC_ACE_ENTRY();
    MDrv_ACE_SetRBChannelRange(pInstance, bScalerWin, bRange );
    XC_ACE_RETURN();
    _ACE_SEMAPHORE_RETURN(pInstance);
}

void MApi_XC_ACE_SetRBChannelRange( MS_BOOL bScalerWin, MS_BOOL bRange )
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return;
    }

    stACE_SetRBChannelRange ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.bRange = bRange;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETRBCHANNELRANGE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

#define LOGTIMECOST  FALSE
MS_U32 MApi_XC_ACE_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState)
{
    static EN_POWER_MODE _enPrevPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;

#if LOGTIMECOST
    MS_U32 u32Begin = MsOS_GetSystemTime();
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);
#endif

    if (enPowerState == E_POWER_SUSPEND)
    {
        _enPrevPowerState = enPowerState;
        u32Ret = UTOPIA_STATUS_SUCCESS;
    }
    else if (enPowerState == E_POWER_RESUME)
    {
        if (_enPrevPowerState == E_POWER_SUSPEND)
        {
            _ACE_SEMAPHORE_ENTRY(pInstance);
            MDrv_ACE_SetSkipWaitVsync(pInstance,MAIN_WINDOW, TRUE);
            MDrv_ACE_SetSkipWaitVsync(pInstance,SUB_WINDOW, TRUE);
            _ACE_GET_VARIABLE();
            MS_BOOL bRet = _MApi_XC_ACE_Init_WithoutCreateMutex(pInstance, &g_XC_ACE_InitData, TRUE);
#ifdef MSOS_TYPE_LINUX_KERNEL
            XC_ACE_ENTRY();
            //arbiter STR
            //E_ACE_CMD_COLORCORRECTIONTABLE
            MDrv_ACE_ColorCorrectionTable(pInstance, MAIN_WINDOW, psACEResPri->stdrvACE.s_AceInfo[MAIN_WINDOW].psColorCorrectionMatrix);
            //E_ACE_CMD_SETPCYUV2RGB
            MDrv_ACE_PCForceYUVtoRGB(pInstance, MAIN_WINDOW , pACEResourceStr->bSetPCYUV2RGB_Enable);
            //E_ACE_CMD_PICSETPOSTCOLORTEMP_V02
            MDrv_ACE_PicSetPostRGBGainOffset(pInstance, MAIN_WINDOW,
                                     pACEResourceStr->stColorTemp.cRedColor,  pACEResourceStr->stColorTemp.cGreenColor,
                                     pACEResourceStr->stColorTemp.cBlueColor, pACEResourceStr->stColorTemp.cRedOffset,
                                     pACEResourceStr->stColorTemp.cGreenOffset, pACEResourceStr->stColorTemp.cBlueOffset, 0);
            if(pACEResourceStr->bUseYUVSpace)
            {
                //E_ACE_CMD_PICSETCONTRAST
                MDrv_ACE_PicSetContrast(pInstance, MAIN_WINDOW, pACEResourceStr->bUseYUVSpace, pACEResourceStr->u8Contrast);
            }
            //E_ACE_CMD_PICSETBRIGHTNESS
            MDrv_ACE_SetBrightness(pInstance, MAIN_WINDOW, pACEResourceStr->u8Brightness_R, pACEResourceStr->u8Brightness_G, pACEResourceStr->u8Brightness_B);
            //E_ACE_CMD_PICSETHUE
            //E_ACE_CMD_PICSETSATURATION
            if(pACEResourceStr->bUseYUVSpace)
            {
                MDrv_ACE_SetHue(pInstance, MAIN_WINDOW, pACEResourceStr->u8Hue);
                MDrv_ACE_SetSaturation(pInstance, MAIN_WINDOW, pACEResourceStr->u8Saturation);
            }
            //E_ACE_CMD_PICSETSHARPNESS
            MDrv_ACE_SetSharpness(pInstance, MAIN_WINDOW, pACEResourceStr->u8Sharpness);
            XC_ACE_RETURN();
#endif
            MDrv_ACE_SetSkipWaitVsync(pInstance,MAIN_WINDOW, FALSE);
            MDrv_ACE_SetSkipWaitVsync(pInstance,SUB_WINDOW, FALSE);
            _ACE_SEMAPHORE_RETURN(pInstance);
            if(bRet == TRUE)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                printf("[%s,%5d]ACE init fail!\n",__FUNCTION__,__LINE__);
                u32Ret = UTOPIA_STATUS_FAIL;
            }
            _enPrevPowerState = enPowerState;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Ret = UTOPIA_STATUS_FAIL;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %u\n",__FUNCTION__,__LINE__,enPowerState);
        u32Ret = UTOPIA_STATUS_FAIL;
    }


#if LOGTIMECOST
    printf("=========[%s,%5d] cost %lu ms========\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime() - u32Begin);
#endif

    return u32Ret;
}

MS_U32 MApi_XC_ACE_SetPowerState(EN_POWER_MODE enPowerState)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return UTOPIA_STATUS_FAIL;
    }

    stACE_Set_PowerState ACEArgs;
    ACEArgs.enPowerState = enPowerState;
    ACEArgs.u32Return = UTOPIA_STATUS_FAIL;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SET_POWERSTATE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return ACEArgs.u32Return;
}

MS_U32 _XC_ACE_GET_DEVICE_NUM(void)
{
    return MDrv_ACE_GetDeviceNum();
}

MS_BOOL MApi_XC_ACE_SetHDRInit_U2(void * pInstance ,XC_ACE_HDRinit * pstACE_HDRInitData)
{
    MS_BOOL bReturn=FALSE;
    MS_U16 u16CopiedLength = sizeof(XC_ACE_HDRinit);

    if((NULL == pstACE_HDRInitData) || (pstACE_HDRInitData->u16HDRInitLength == 0))
    {
        //We cosider compatible operation form version2 , so reject the info init when version invalid
        printf("MApi_XC_ACE_SetHDRInit: Null paramter or Wrong u16HDRInitLength!!\n");
        return bReturn;
    }

    // the version control is coming in with version 1
    if(pstACE_HDRInitData->u16HDRVerInfo < 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        printf("MApi_XC_ACE_SetHDRInit: please check your u16HDRVerInfo, it should not set to 0!!\n");
        return bReturn;
    }

    if(pstACE_HDRInitData->u16HDRVerInfo > XC_ACE_HDR_VERSION)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
#if defined (__aarch64__)
        printf("MApi_XC_ACE_SetHDRInit: this old version ACE lib has only length:%lu driver status!!\n",sizeof(XC_ACE_HDRinit));
#else
        printf("MApi_XC_ACE_SetHDRInit: this old version ACE lib has only length:%u driver status!!\n",sizeof(XC_ACE_HDRinit));
#endif
        //we still copy the min size of both structure, but AP should take care of it.
        u16CopiedLength = sizeof(XC_ACE_HDRinit);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pstACE_HDRInitData->u16HDRVerInfo < XC_ACE_HDR_VERSION) || (pstACE_HDRInitData->u16HDRInitLength < sizeof(XC_ACE_HDRinit)))
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        printf("MApi_XC_ACE_SetHDRInit : new version ACE lib shouldn't access to the space which doesn't exist in old structure!!\n");
        u16CopiedLength = pstACE_HDRInitData->u16HDRInitLength;
    }

    ACE_DRV_HDRinit ACE_HDRInitInfo;
    memset(&ACE_HDRInitInfo, 0 , sizeof(ACE_DRV_HDRinit));
    memcpy(&ACE_HDRInitInfo, pstACE_HDRInitData, u16CopiedLength);
    ACE_HDRInitInfo.u16HDRInitLength = u16CopiedLength;

    bReturn=MDrv_ACE_SetHDRInit(pInstance ,&ACE_HDRInitInfo);

    return bReturn;
}

MS_BOOL SYMBOL_WEAK MApi_XC_ACE_SetHDRInit(XC_ACE_HDRinit *pstACE_HDRInitData)
{
    if (pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_SetHDRInitData ACEArgs;
    ACEArgs.pstHDRInitData = pstACE_HDRInitData;

     ACEArgs.bReturn = FALSE ;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SET_HDR_INFO, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE ;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}


#undef _API_XC_ACE_C_
#endif  // _API_XC_ACE_C_

