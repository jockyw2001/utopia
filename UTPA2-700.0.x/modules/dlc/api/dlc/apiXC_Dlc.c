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

#ifndef _API_XC_DLC_C_
#define _API_XC_DLC_C_

/******************************************************************************/
/*                   Header Files                                             */
/* ****************************************************************************/
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvDLC.h"
#include "drvDLC_Cus.h"
#include "drvDBC_Cus.h"
#include "apiXC_Dlc.h"
#include "apiXC_DBC.h"
// Internal Definition
#include "drvMMIO.h"

#include "utopia.h"
#include "apiXC_Dlc_v2.h"

/******************************************************************************/
/*                      Debug information                                      */
/******************************************************************************/

#define XC_DCL_DBG 0

#if XC_DCL_DBG
#define XC_DLC_DBG_MSG(fmt,...) \
    MS_DEBUG_MSG(do{printf( "[PNL_DBG_MSG]: %s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);} while(0))

#define XC_DLC_PRINT_VAR(var)  \
    do{\
        XC_DLC_DBG_MSG("%30s: %d\n", #var, (var));\
        }while(0)

#define XC_DLC_PRINT_FUN(fun)  \
    do{\
        XC_DLC_DBG_MSG("\n");\
        XC_DLC_DBG_MSG("**************************************************\n");\
        XC_DLC_DBG_MSG("*\t\t%s \t\t\n", #fun);\
        XC_DLC_DBG_MSG("**************************************************\n");\
        fun;\
        XC_DLC_DBG_MSG("*                                                *\n");\
        XC_DLC_DBG_MSG("*                                                *\n");\
        XC_DLC_DBG_MSG("**************************************************\n");\
        }while(0)

#else
  #define XC_DLC_DBG_MSG(str, ...)
  #define XC_DLC_PRINT_VAR(var)
  #define XC_DLC_PRINT_FUN(fun) fun
#endif

#if defined (__aarch64__)
#define VERSION_COMPATIBLE_CHECK(pstInput,stValid,copy_Length,version_arg,length_arg,current_version) do { \
    if(pstInput == NULL)\
        return FALSE;\
    /*new AP + old lib, only the length corresponding to old lib has meaning.*/\
    if((pstInput)->version_arg > (current_version))\
    {\
        /*We consider compatible operation from this version, so reject the info init when version invalid*/\
        printf("[%s: %d]: this old version XC lib has only length:%lu driver status!!\n",\
                     __FUNCTION__, __LINE__, sizeof(stValid));\
        /*we still copy the min size of both structure, but AP should take care of it.*/\
        (copy_Length) = sizeof(stValid);\
    }\
    \
    /*old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure*/\
    if(((pstInput)->version_arg < (current_version)) || ((pstInput)->length_arg < sizeof(stValid)))\
    {\
        /*We consider compatible operation from this version, so reject the info init when version invalid*/\
        printf("[%s: %d]: new version XC lib shouldn't access to the space which doesn't exist in old structure!!\n",\
        __FUNCTION__, __LINE__);\
        (copy_Length) = (pstInput)->length_arg;\
    }\
}while(0)
#else
#define VERSION_COMPATIBLE_CHECK(pstInput,stValid,copy_Length,version_arg,length_arg,current_version) do { \
    if(pstInput == NULL)\
        return FALSE;\
    /*new AP + old lib, only the length corresponding to old lib has meaning.*/\
    if((pstInput)->version_arg > (current_version))\
    {\
        /*We consider compatible operation from this version, so reject the info init when version invalid*/\
        printf("[%s: %d]: this old version XC lib has only length:%u driver status!!\n",\
                     __FUNCTION__, __LINE__, sizeof(stValid));\
        /*we still copy the min size of both structure, but AP should take care of it.*/\
        (copy_Length) = sizeof(stValid);\
    }\
    \
    /*old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure*/\
    if(((pstInput)->version_arg < (current_version)) || ((pstInput)->length_arg < sizeof(stValid)))\
    {\
        /*We consider compatible operation from this version, so reject the info init when version invalid*/\
        printf("[%s: %d]: new version XC lib shouldn't access to the space which doesn't exist in old structure!!\n",\
        __FUNCTION__, __LINE__);\
        (copy_Length) = (pstInput)->length_arg;\
    }\
}while(0)
#endif

#define DLC_MUTEX
#define DLC_MUTEX_DBG    0

#ifdef DLC_MUTEX
    #if(DLC_MUTEX_DBG)
    #define Dlc_FuncEnter() \
        printf("1,==========================\n");                              \
        printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
        if(!MsOS_ObtainMutex(_DLC_Mutex, MSOS_WAIT_FOREVER)) \
        {                                                     \
            printf("==========================\n");            \
            printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
        }
    #define Dlc_FuncExit()    \
        printf("0,==========================\n");                              \
        printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
        MsOS_ReleaseMutex(_DLC_Mutex);
    #else
    #define Dlc_FuncEnter() \
        if(!MsOS_ObtainMutex(_DLC_Mutex, MSOS_WAIT_FOREVER)) \
        {                                                     \
        }

    #define Dlc_FuncExit()    \
        MsOS_ReleaseMutex(_DLC_Mutex);

    #endif
#else
    #define Dlc_FuncEnter() while(0)
    #define Dlc_FuncExit()  while(0)
#endif

static MSIF_Version _api_xc_dlc_version = {
    .DDI = { XC_DLC_API_VERSION },
};



/******************************************************************************/
/*                   Local Function Prototypes                                */
/******************************************************************************/

/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/

/******************************************************************************/
/*                   Constant                                                 */
/******************************************************************************/

#define DLC_DBG(x)     // x

static const XC_DLC_ApiInfo _stXC_DLC_ApiInfo = {};
//static MS_U16 _u16XC_DLCDbgSwitch = 0;
static MS_BOOL g_bDBCEnable;

/******************************************************************************/
// Initialize DLC
/******************************************************************************/
MS_S32 _DLC_Mutex = -1;

static MS_BOOL g_bDLC_CGC_ready = FALSE;
static MS_BOOL g_bDLC_DBC_ready = FALSE;
static MS_BOOL g_bDLC_CGC_CGainPQCom;
static MS_BOOL g_bDLC_CGC_YGainPQCom;
static MS_U8   g_u8DLC_CGC_CGain;
static MS_U8   g_u8DLC_CGC_YGain;
static MS_U8   g_u8DLC_CGC_YGain_Backup;
static MS_U8   g_u8DLC_CGC_CGain_Backup;
static StuDbc_CAPTURE_Range g_DlcCapRange_Suspend = {0};

// for Utopia 2.0 to Utopia 1.0 compatibility.
void* pu32DLCInst = NULL;
void* pu32DBCInst = NULL;


MS_BOOL MApi_XC_DLC_Init_Ex_U2(void * pInstance,XC_DLC_init * pstXC_DLC_InitData,MS_U32 u32InitDataLen)
{
    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit;
    MS_PHY u32NonPMBankSize = 0;
	MS_VIRT ptrDLCRiuBaseAddr=0;
    DLC_init_Ext DLC_InitExt;


    // get MMIO base
    if(MDrv_MMIO_GetBASE( &ptrDLCRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PM ) != TRUE)
    {
        //OS_DELETE_MUTEX(_s32ACEMutex);
        //PNL_ASSERT(0, "%s\n", "Get base address failed\n");
        //printf("XC_DLC GetBase failed\n");
        return FALSE;
    }
    else
    {
    }
    memset(&DLC_InitExt, 0 , sizeof(DLC_InitExt) );
    MDrv_DLC_init_riu_base( ptrDLCRiuBaseAddr);

    if(u32InitDataLen != sizeof(XC_DLC_init))
    {
        return FALSE;
    }
    memset(&DLC_MFinit, 0 , sizeof(DLC_MFinit) );

    for (i=0; i<16; ++i)
    {
        DLC_InitExt.uwLumaCurve[i]     = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve[i])<<2;
        DLC_InitExt.uwLumaCurve2_a[i]  = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve2_a[i])<<2;
        DLC_InitExt.uwLumaCurve2_b[i]  = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve2_b[i])<<2;

        DLC_MFinit.ucLumaCurve[i]     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve[i];
        DLC_MFinit.ucLumaCurve2_a[i]  = pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve2_a[i];
        DLC_MFinit.ucLumaCurve2_b[i]  = pstXC_DLC_InitData->DLC_MFinit_Ex.ucLumaCurve2_b[i];
    }


#if (ENABLE_10_BIT_DLC)
    DLC_InitExt.b10BitsCruveEn        = pstXC_DLC_InitData->b10BitsEn;
#else
    DLC_InitExt.b10BitsCruveEn        = 0;
#endif

    DLC_MFinit.u8_L_L_U               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_L_L_U;
    DLC_MFinit.u8_L_L_D               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_L_L_D;
    DLC_MFinit.u8_L_H_U               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_L_H_U;
    DLC_MFinit.u8_L_H_D               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_L_H_D;
    DLC_MFinit.u8_S_L_U               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_S_L_U;
    DLC_MFinit.u8_S_L_D               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_S_L_D;
    DLC_MFinit.u8_S_H_U               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_S_H_U;
    DLC_MFinit.u8_S_H_D               = pstXC_DLC_InitData->DLC_MFinit_Ex.u8_S_H_D ;

    DLC_MFinit.ucDlcPureImageMode     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcPureImageMode;
    DLC_MFinit.ucDlcLevelLimit        = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcLevelLimit;
    DLC_MFinit.ucDlcAvgDelta          = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcAvgDelta;
    DLC_MFinit.ucDlcAvgDeltaStill     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcAvgDeltaStill;
    DLC_MFinit.ucDlcFastAlphaBlending = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcFastAlphaBlending;
    DLC_MFinit.ucDlcYAvgThresholdL    = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcYAvgThresholdL;
    DLC_MFinit.ucDlcYAvgThresholdH    = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcYAvgThresholdH;
    DLC_MFinit.ucDlcBLEPoint          = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcBLEPoint;
    DLC_MFinit.ucDlcWLEPoint          = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcWLEPoint;
    DLC_MFinit.bEnableBLE             = pstXC_DLC_InitData->DLC_MFinit_Ex.bEnableBLE;
    DLC_MFinit.bEnableWLE             = pstXC_DLC_InitData->DLC_MFinit_Ex.bEnableWLE;
    DLC_MFinit.ucCGCYth               = pstXC_DLC_InitData->DLC_MFinit_Ex.ucCGCYth;
    DLC_MFinit.bCGCCGainCtrl          = pstXC_DLC_InitData->DLC_MFinit_Ex.bCGCCGainCtrl;

    // CGC
    DLC_MFinit.ucCGCCGain_offset      = pstXC_DLC_InitData->DLC_MFinit_Ex.ucCGCCGain_offset;
    DLC_MFinit.ucCGCChroma_GainLimitH = pstXC_DLC_InitData->DLC_MFinit_Ex.ucCGCChroma_GainLimitH;
    DLC_MFinit.ucCGCChroma_GainLimitL = pstXC_DLC_InitData->DLC_MFinit_Ex.ucCGCChroma_GainLimitL;
    DLC_MFinit.ucCGCYCslope           = pstXC_DLC_InitData->DLC_MFinit_Ex.ucCGCYCslope;


    if(pstXC_DLC_InitData->DLC_MFinit_Ex.u32DLC_MFinit_Ex_Version>=1)
    {
#if (DLC_MFINIT_EX_VERSION>=1)

        for (i=0; i<DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM; ++i)
        {
            DLC_MFinit.ucDlcHistogramLimitCurve[i]     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcHistogramLimitCurve[i];
        }

        DLC_MFinit.ucDlcYAvgThresholdM    = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcYAvgThresholdM;
        DLC_MFinit.ucDlcCurveMode         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcCurveMode;
        DLC_MFinit.ucDlcCurveModeMixAlpha = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcCurveModeMixAlpha;
        DLC_MFinit.ucDlcAlgorithmMode     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcAlgorithmMode;
        DLC_MFinit.ucDlcSepPointH         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcSepPointH;
        DLC_MFinit.ucDlcSepPointL         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcSepPointL;
        DLC_MFinit.uwDlcBleStartPointTH   = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBleStartPointTH;
        DLC_MFinit.uwDlcBleEndPointTH     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBleEndPointTH;
        DLC_MFinit.ucDlcCurveDiff_L_TH    = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcCurveDiff_L_TH;
        DLC_MFinit.ucDlcCurveDiff_H_TH     = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcCurveDiff_H_TH;
        DLC_MFinit.uwDlcBLESlopPoint_1     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBLESlopPoint_1;
        DLC_MFinit.uwDlcBLESlopPoint_2     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBLESlopPoint_2;
        DLC_MFinit.uwDlcBLESlopPoint_3     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBLESlopPoint_3;
        DLC_MFinit.uwDlcBLESlopPoint_4     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBLESlopPoint_4;
        DLC_MFinit.uwDlcBLESlopPoint_5     = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcBLESlopPoint_5;
        DLC_MFinit.uwDlcDark_BLE_Slop_Min  = pstXC_DLC_InitData->DLC_MFinit_Ex.uwDlcDark_BLE_Slop_Min;
        DLC_MFinit.ucDlcCurveDiffCoringTH  = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcCurveDiffCoringTH;
        DLC_MFinit.ucDlcAlphaBlendingMin   = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcAlphaBlendingMin;
        DLC_MFinit.ucDlcAlphaBlendingMax   = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcAlphaBlendingMax;
        DLC_MFinit.ucDlcFlicker_alpha      = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcFlicker_alpha;
        DLC_MFinit.ucDlcYAVG_L_TH          = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcYAVG_L_TH;
        DLC_MFinit.ucDlcYAVG_H_TH          = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcYAVG_H_TH;

        DLC_MFinit.ucDlcDiffBase_L         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcDiffBase_L;
        DLC_MFinit.ucDlcDiffBase_M         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcDiffBase_M;
        DLC_MFinit.ucDlcDiffBase_H         = pstXC_DLC_InitData->DLC_MFinit_Ex.ucDlcDiffBase_H;
#endif
#if (DLC_MFINIT_EX_VERSION>=2)
        DLC_MFinit.ucLMaxThreshold         = pstXC_DLC_InitData->DLC_MFinit_Ex.u8LMaxThreshold;
        DLC_MFinit.ucLMinThreshold         = pstXC_DLC_InitData->DLC_MFinit_Ex.u8LMinThreshold;
        DLC_MFinit.ucLMaxCorrection        = pstXC_DLC_InitData->DLC_MFinit_Ex.u8LMaxCorrection;
        DLC_MFinit.ucLMinCorrection        = pstXC_DLC_InitData->DLC_MFinit_Ex.u8LMinCorrection;
        DLC_MFinit.ucRMaxThreshold         = pstXC_DLC_InitData->DLC_MFinit_Ex.u8RMaxThreshold;
        DLC_MFinit.ucRMinThreshold         = pstXC_DLC_InitData->DLC_MFinit_Ex.u8RMinThreshold;
        DLC_MFinit.ucRMaxCorrection        = pstXC_DLC_InitData->DLC_MFinit_Ex.u8RMaxCorrection;
        DLC_MFinit.ucRMinCorrection        = pstXC_DLC_InitData->DLC_MFinit_Ex.u8RMinCorrection;
        DLC_MFinit.ucAllowLoseContrast     = pstXC_DLC_InitData->DLC_MFinit_Ex.u8AllowLoseContrast;

#endif
    }

    MDrv_DLC_Init(DLC_MFinit);
    MDrv_DLC_Init_Ext(&DLC_InitExt);

    g_DlcCapRange_Suspend.wHStart = pstXC_DLC_InitData->u16CurveHStart;
    g_DlcCapRange_Suspend.wHEnd   = pstXC_DLC_InitData->u16CurveHEnd;
    g_DlcCapRange_Suspend.wVStart = pstXC_DLC_InitData->u16CurveVStart;
    g_DlcCapRange_Suspend.wVEnd   = pstXC_DLC_InitData->u16CurveVEnd;

    Dlc_FuncEnter();
    MDrv_DLC_InitCurve (pstXC_DLC_InitData->u16CurveHStart, pstXC_DLC_InitData->u16CurveHEnd, pstXC_DLC_InitData->u16CurveVStart, pstXC_DLC_InitData->u16CurveVEnd);
    Dlc_FuncExit();

    return TRUE;
}


MS_BOOL MApi_XC_DLC_Init_Ex(XC_DLC_init *pstXC_DLC_InitData, MS_U32 u32InitDataLen)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_Init_Ex DLCArgs;
    DLCArgs.pstXC_DLC_InitData = pstXC_DLC_InitData;
    DLCArgs.u32InitDataLen = u32InitDataLen;
    DLCArgs.bReturn = FALSE ;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_INIT_EX, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


//-------------------------------------------------------------------------------------------------
/// Setting DLC
// @param  DLC_MFinit             \b IN: DLC data
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_SetSetting_Ex_U2(void * pInstance,XC_DLC_MFinit_Ex * DLC_MFinit_Ex)
{

    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit_new;

    memset(&DLC_MFinit_new, 0 , sizeof(DLC_MFinit_new) );
    for (i=0; i<16; ++i)
    {
        DLC_MFinit_new.ucLumaCurve[i]     = DLC_MFinit_Ex->ucLumaCurve[i];
        DLC_MFinit_new.ucLumaCurve2_a[i]  = DLC_MFinit_Ex->ucLumaCurve2_a[i];
        DLC_MFinit_new.ucLumaCurve2_b[i]  = DLC_MFinit_Ex->ucLumaCurve2_b[i];
    }

    DLC_MFinit_new.u8_L_L_U               = DLC_MFinit_Ex->u8_L_L_U;
    DLC_MFinit_new.u8_L_L_D               = DLC_MFinit_Ex->u8_L_L_D;
    DLC_MFinit_new.u8_L_H_U               = DLC_MFinit_Ex->u8_L_H_U;
    DLC_MFinit_new.u8_L_H_D               = DLC_MFinit_Ex->u8_L_H_D;
    DLC_MFinit_new.u8_S_L_U               = DLC_MFinit_Ex->u8_S_L_U;
    DLC_MFinit_new.u8_S_L_D               = DLC_MFinit_Ex->u8_S_L_D;
    DLC_MFinit_new.u8_S_H_U               = DLC_MFinit_Ex->u8_S_H_U;
    DLC_MFinit_new.u8_S_H_D               = DLC_MFinit_Ex->u8_S_H_D ;

    DLC_MFinit_new.ucDlcPureImageMode     = DLC_MFinit_Ex->ucDlcPureImageMode;
    DLC_MFinit_new.ucDlcLevelLimit        = DLC_MFinit_Ex->ucDlcLevelLimit;
    DLC_MFinit_new.ucDlcAvgDelta          = DLC_MFinit_Ex->ucDlcAvgDelta;
    DLC_MFinit_new.ucDlcAvgDeltaStill     = DLC_MFinit_Ex->ucDlcAvgDeltaStill;
    DLC_MFinit_new.ucDlcFastAlphaBlending = DLC_MFinit_Ex->ucDlcFastAlphaBlending;
    DLC_MFinit_new.ucDlcYAvgThresholdL    = DLC_MFinit_Ex->ucDlcYAvgThresholdL;
    DLC_MFinit_new.ucDlcYAvgThresholdH    = DLC_MFinit_Ex->ucDlcYAvgThresholdH;
    DLC_MFinit_new.ucDlcBLEPoint          = DLC_MFinit_Ex->ucDlcBLEPoint;
    DLC_MFinit_new.ucDlcWLEPoint          = DLC_MFinit_Ex->ucDlcWLEPoint;
    DLC_MFinit_new.bEnableBLE             = DLC_MFinit_Ex->bEnableBLE;
    DLC_MFinit_new.bEnableWLE             = DLC_MFinit_Ex->bEnableWLE;

    DLC_MFinit_new.ucCGCYth               = DLC_MFinit_Ex->ucCGCYth;
    DLC_MFinit_new.bCGCCGainCtrl          = DLC_MFinit_Ex->bCGCCGainCtrl;

    // CGC
    DLC_MFinit_new.ucCGCCGain_offset      = DLC_MFinit_Ex->ucCGCCGain_offset;
    DLC_MFinit_new.ucCGCChroma_GainLimitH = DLC_MFinit_Ex->ucCGCChroma_GainLimitH;
    DLC_MFinit_new.ucCGCChroma_GainLimitL = DLC_MFinit_Ex->ucCGCChroma_GainLimitL;
    DLC_MFinit_new.ucCGCYCslope           = DLC_MFinit_Ex->ucCGCYCslope;

    if(DLC_MFinit_Ex->u32DLC_MFinit_Ex_Version>=1)
    {
        for (i=0; i<DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM; ++i)
        {
            DLC_MFinit_new.ucDlcHistogramLimitCurve[i]  = DLC_MFinit_Ex->ucDlcHistogramLimitCurve[i];
        }

        DLC_MFinit_new.ucDlcYAvgThresholdM    = DLC_MFinit_Ex->ucDlcYAvgThresholdM;
        DLC_MFinit_new.ucDlcCurveMode         = DLC_MFinit_Ex->ucDlcCurveMode;
        DLC_MFinit_new.ucDlcCurveModeMixAlpha = DLC_MFinit_Ex->ucDlcCurveModeMixAlpha;
        DLC_MFinit_new.ucDlcAlgorithmMode     = DLC_MFinit_Ex->ucDlcAlgorithmMode;
        DLC_MFinit_new.ucDlcSepPointH         = DLC_MFinit_Ex->ucDlcSepPointH;
        DLC_MFinit_new.ucDlcSepPointL         = DLC_MFinit_Ex->ucDlcSepPointL;
        DLC_MFinit_new.uwDlcBleStartPointTH   = DLC_MFinit_Ex->uwDlcBleStartPointTH;
        DLC_MFinit_new.uwDlcBleEndPointTH     = DLC_MFinit_Ex->uwDlcBleEndPointTH;
        DLC_MFinit_new.ucDlcCurveDiff_L_TH    = DLC_MFinit_Ex->ucDlcCurveDiff_L_TH;
        DLC_MFinit_new.ucDlcCurveDiff_H_TH    = DLC_MFinit_Ex->ucDlcCurveDiff_H_TH;
        DLC_MFinit_new.uwDlcBLESlopPoint_1    = DLC_MFinit_Ex->uwDlcBLESlopPoint_1;
        DLC_MFinit_new.uwDlcBLESlopPoint_2    = DLC_MFinit_Ex->uwDlcBLESlopPoint_2;
        DLC_MFinit_new.uwDlcBLESlopPoint_3    = DLC_MFinit_Ex->uwDlcBLESlopPoint_3;
        DLC_MFinit_new.uwDlcBLESlopPoint_4    = DLC_MFinit_Ex->uwDlcBLESlopPoint_4;
        DLC_MFinit_new.uwDlcBLESlopPoint_5    = DLC_MFinit_Ex->uwDlcBLESlopPoint_5;
        DLC_MFinit_new.uwDlcDark_BLE_Slop_Min = DLC_MFinit_Ex->uwDlcDark_BLE_Slop_Min;
        DLC_MFinit_new.ucDlcCurveDiffCoringTH = DLC_MFinit_Ex->ucDlcCurveDiffCoringTH;
        DLC_MFinit_new.ucDlcAlphaBlendingMin  = DLC_MFinit_Ex->ucDlcAlphaBlendingMin;
        DLC_MFinit_new.ucDlcAlphaBlendingMax  = DLC_MFinit_Ex->ucDlcAlphaBlendingMax;
        DLC_MFinit_new.ucDlcFlicker_alpha     = DLC_MFinit_Ex->ucDlcFlicker_alpha;
        DLC_MFinit_new.ucDlcYAVG_L_TH         = DLC_MFinit_Ex->ucDlcYAVG_L_TH;
        DLC_MFinit_new.ucDlcYAVG_H_TH         = DLC_MFinit_Ex->ucDlcYAVG_H_TH;

        DLC_MFinit_new.ucDlcDiffBase_L        = DLC_MFinit_Ex->ucDlcDiffBase_L;
        DLC_MFinit_new.ucDlcDiffBase_M        = DLC_MFinit_Ex->ucDlcDiffBase_M;
        DLC_MFinit_new.ucDlcDiffBase_H        = DLC_MFinit_Ex->ucDlcDiffBase_H;
    }

    MDrv_DLC_Init(DLC_MFinit_new);
}

void MApi_XC_DLC_SetSetting_Ex(XC_DLC_MFinit_Ex *DLC_MFinit_Ex)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetSetting_Ex DLCArgs;
    DLCArgs.DLC_MFinit_Ex = DLC_MFinit_Ex;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_SETTING_EX, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_SetCurve_U2(void * pInstance,MS_U8 * pNormal,MS_U8 * pLight,MS_U8 * pDark)
{

    MDrv_DLC_SendDlcInitCurveChangeInfo(ENABLE, pNormal, pLight, pDark );
    MDrv_DLC_SetCurve(pNormal, pLight, pDark);
}

void MApi_XC_DLC_SetCurve(MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetCurve DLCArgs;
    DLCArgs.pNormal = pNormal;
    DLCArgs.pLight = pLight;
    DLCArgs.pDark = pDark;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_CURVE, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return ;
    }

    return ;
}


void MApi_XC_DLC_SetBleSlopPoint_U2(void * pInstance,MS_U16 * pBLESlopPoint)
{
    MDrv_DLC_SendBleChangeInfo(ENABLE, pBLESlopPoint);
}

void MApi_XC_DLC_SetBleSlopPoint(MS_U16 *pBLESlopPoint)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetBleSlopPoint DLCArgs;
    DLCArgs.pBLESlopPoint = pBLESlopPoint;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_BLE_SLOP_POINT, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return ;
    }

    return ;
}

MS_BOOL MApi_XC_DLC_SetHDRInit_U2(void * pInstance ,XC_DLC_HDRinit * pstDLC_HDRInitData)
{
    MS_BOOL bReturn=FALSE;
    MS_U16 u16CopiedLength = sizeof(XC_DLC_HDRinit);

    if((NULL == pstDLC_HDRInitData) || (pstDLC_HDRInitData->u16HDRInitLength == 0))
    {
        //We cosider compatible operation form version2 , so reject the info init when version invalid
        printf("MApi_XC_DLC_SetHDRInit: Null paramter or Wrong u16HDRInitLength!!\n");
        return bReturn;
    }

    // the version control is coming in with version 1
    if(pstDLC_HDRInitData->u16HDRVerInfo < 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        printf("MApi_XC_DLC_SetHDRInit: please check your u16HDRVerInfo, it should not set to 0!!\n");
        return bReturn;
    }

    if(pstDLC_HDRInitData->u16HDRVerInfo > XC_DLC_HDR_VERSION)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
#if defined (__aarch64__)
        printf("MApi_XC_DLC_SetHDRInit: this old version DLC lib has only length:%lu driver status!!\n",sizeof(XC_DLC_HDRinit));
#else
        printf("MApi_XC_DLC_SetHDRInit: this old version DLC lib has only length:%u driver status!!\n",sizeof(XC_DLC_HDRinit));
#endif
        //we still copy the min size of both structure, but AP should take care of it.
        u16CopiedLength = sizeof(XC_DLC_HDRinit);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pstDLC_HDRInitData->u16HDRVerInfo < XC_DLC_HDR_VERSION) || (pstDLC_HDRInitData->u16HDRInitLength < sizeof(XC_DLC_HDRinit)))
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        printf("MApi_XC_DLC_SetHDRInit: new version DLC lib shouldn't access to the space which doesn't exist in old structure!!\n");
        u16CopiedLength = pstDLC_HDRInitData->u16HDRInitLength;
    }

    DLC_HDRinit DLC_HDRInitInfo;
    memset(&DLC_HDRInitInfo, 0 , sizeof(DLC_HDRinit));
    memcpy(&DLC_HDRInitInfo, pstDLC_HDRInitData, u16CopiedLength);
    DLC_HDRInitInfo.u16HDRInitLength = u16CopiedLength;
    bReturn=MDrv_DLC_SetHDRInit(&DLC_HDRInitInfo);
    return bReturn;
}

MS_BOOL SYMBOL_WEAK MApi_XC_DLC_SetHDRInit(XC_DLC_HDRinit *pstDLC_HDRInitData)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_SetHDRInitData DLCArgs;
    DLCArgs.pstDLC_HDRInitData = pstDLC_HDRInitData;

     DLCArgs.bReturn = FALSE ;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_HDR_INFO, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE ;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
MS_BOOL MApi_XC_DLC_SetTMOInfo_U2(void * pInstance, ST_XC_DLC_TMO_INFO *pstDLCTmoInfo)
{
    MS_BOOL bReturn = FALSE;
    MS_U16 u16CopiedLength = sizeof(ST_XC_DLC_TMO_INFO);
    ST_DLC_TMO_INFO stDLCTMOInfo;

    memset(&stDLCTMOInfo, 0, sizeof(stDLCTMOInfo));
    VERSION_COMPATIBLE_CHECK(pstDLCTmoInfo, stDLCTMOInfo, u16CopiedLength, u32TMOInfo_Version, u16TMOInfo_Length, DLC_TMO_INFO_VERSION);
    pstDLCTmoInfo->u32TMOInfo_Version = DLC_TMO_INFO_VERSION; /*Return the actual version of the returned value for APP to use*/
    pstDLCTmoInfo->u16TMOInfo_Length = u16CopiedLength; /*Return the actual length of the returned value for APP to know whick info is valid*/
    memcpy(&stDLCTMOInfo, pstDLCTmoInfo, u16CopiedLength);

    bReturn = MDrv_DLC_SetTMOInfo(&stDLCTMOInfo);
    return bReturn;
}

MS_BOOL SYMBOL_WEAK MApi_XC_DLC_SetTMOInfo(ST_XC_DLC_TMO_INFO *pstDLCTmoInfo)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_SetTMOInfo DLCArgs;
    DLCArgs.pstDLC_TMOInfo = pstDLCTmoInfo;

    DLCArgs.bReturn = FALSE ;

    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_TMOINFO, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE ;
    }
    else
    {
        return DLCArgs.bReturn;
    }

    return DLCArgs.bReturn;
}
#endif
#endif

MS_BOOL MApi_XC_DLC_Exit_U2(void * pInstance)
{
    return TRUE;
}

MS_BOOL MApi_XC_DLC_Exit(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_Exit DLCArgs;
    DLCArgs.bReturn = FALSE ;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_EXIT, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }


}


/******************************************************************************/
//Enable/disable DLC to control MDrv_DLC_SetOnOff ,used DLC enbale register to control.
//@param bSwitch \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
/******************************************************************************/
void MApi_XC_DLC_SetOnOff_U2(void * pInstance,MS_BOOL bSwitch,MS_BOOL bWindow)
{
    Dlc_FuncEnter();
    MDrv_DLC_SetOnOff( bSwitch, bWindow );
    Dlc_FuncExit();
}

void MApi_XC_DLC_SetOnOff ( MS_BOOL bSwitch, MS_BOOL bWindow )
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetOnOff DLCArgs;
    DLCArgs.bSwitch = bSwitch;
    DLCArgs.bWindow = bWindow;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_ON_OFF, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


/******************************************************************************/
//Enable/disable DLC to control MDrv_DLC_Handler function.
//@param bSwitch \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
/******************************************************************************/
MS_BOOL MApi_XC_DLC_SetDlcHandlerOnOff_U2(void * pInstance,MS_BOOL bDlcEnable)
{
    MDrv_DLC_SetDlcHandlerOnOff( bDlcEnable);
    return TRUE;
}

MS_BOOL MApi_XC_DLC_SetDlcHandlerOnOff( MS_BOOL bDlcEnable )
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_SetDlcHandlerOnOff DLCArgs;
    DLCArgs.bDlcEnable = bDlcEnable;
    DLCArgs.bReturn = FALSE ;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_HANDLER_ON_OFF, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


/******************************************************************************/
//Enable/disable BLE to control BLE function.
//@param bSwitch \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
/******************************************************************************/
MS_BOOL MApi_XC_DLC_SetBleOnOff_U2(void * pInstance,MS_BOOL bSwitch)
{
    MDrv_DLC_SetBleOnOff( bSwitch);
    return TRUE;
}

MS_BOOL MApi_XC_DLC_SetBleOnOff( MS_BOOL bSwitch )
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_SetBleOnOff DLCArgs;
    DLCArgs.bSwitch = bSwitch;
    DLCArgs.bReturn = FALSE ;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_BLE_ON_OFF, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


/******************************************************************************/
//Enable/disable DLC Set Curve Both for main and sub.
//@param bEnable \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
/******************************************************************************/
void MApi_XC_DLC_EnableMainSubCurveSynchronization_U2(void * pInstance,MS_BOOL bEnable)
{
    Dlc_FuncEnter();
    MDrv_DLC_EnableMainSubCurveSynchronization( bEnable );
    Dlc_FuncExit();
}

void MApi_XC_DLC_EnableMainSubCurveSynchronization (MS_BOOL bEnable)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_EnableMainSubCurveSynchronization DLCArgs;
    DLCArgs.bEnable = bEnable;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


 /******************************************************************************/
// DLC handler
// @param bWindow \b IN:
// - @see
/******************************************************************************/
 void MApi_XC_DLC_Handler_U2(void * pInstance,MS_BOOL bWindow)
 {
     Dlc_FuncEnter();
     MDrv_DLC_Handler(bWindow);
     Dlc_FuncExit();
 }

void MApi_XC_DLC_Handler(MS_BOOL bWindow)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_Handler DLCArgs;
    DLCArgs.bWindow = bWindow;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_HANDLER, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


 /******************************************************************************/
// DLC get histogram handler
// @param bWindow \b IN:
// - @see
/******************************************************************************/
MS_BOOL MApi_XC_DLC_GetHistogramHandler_U2(void * pInstance,MS_BOOL bWindow)
{
    MS_BOOL bResult;

    Dlc_FuncEnter();

    bResult = MDrv_DLC_GetHistogram(bWindow);
    Dlc_FuncExit();

    return bResult;
}

MS_BOOL MApi_XC_DLC_GetHistogramHandler(MS_BOOL bWindow)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_GetHistogramHandler DLCArgs;
    DLCArgs.bWindow = bWindow;
    DLCArgs.bReturn = FALSE ;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_HISTOGRAM_HANDLER, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


/******************************************************************************/
//Get average value of histogram
//@return Average value from 0 to 255
/******************************************************************************/
MS_U8 MApi_XC_DLC_GetAverageValue_U2(void * pInstance)
{
    MS_U8 bResult;

    Dlc_FuncEnter();

    bResult = MDrv_DLC_GetAverageValue();

    Dlc_FuncExit();
    return bResult;
}

MS_U8 MApi_XC_DLC_GetAverageValue(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return 0;
        }
    }

    stDLC_GetAverageValue DLCArgs;
    DLCArgs.u8Return = 0 ;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_AVERAGE_VALUE, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return 0;
    }
    else
    {
        return DLCArgs.u8Return;
    }
}


/******************************************************************************/
//Get average value exactness of histogram
//@return Average value from 0 to 1023
/******************************************************************************/
MS_U16 MApi_XC_DLC_GetAverageValue_Ex_U2(void * pInstance)
{
    MS_U16 bResult;

    Dlc_FuncEnter();

    bResult = MDrv_DLC_GetAverageValue_x4();

    Dlc_FuncExit();
    return bResult;
}

MS_U16 MApi_XC_DLC_GetAverageValue_Ex(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return 0;
        }
    }

    stDLC_GetAverageValue_Ex DLCArgs;
    DLCArgs.u16Return = 0 ;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_AVERAGE_VALUE_EX, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return 0;
    }
    else
    {
        return DLCArgs.u16Return;
    }
}


void MApi_XC_DLC_InitCurve_U2(void * pInstance,MS_U16 u16HStart,MS_U16 u16HEnd,MS_U16 u16VStart,MS_U16 u16VEnd)
{
    Dlc_FuncEnter();
    MDrv_DLC_InitCurve ( u16HStart, u16HEnd, u16VStart, u16VEnd );
    Dlc_FuncExit();
}

void MApi_XC_DLC_InitCurve (MS_U16 u16HStart, MS_U16 u16HEnd, MS_U16 u16VStart, MS_U16 u16VEnd)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_InitCurve DLCArgs;
    DLCArgs.u16HStart = u16HStart;
    DLCArgs.u16HEnd = u16HEnd;
    DLCArgs.u16VStart = u16VStart;
    DLCArgs.u16VEnd = u16VEnd;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_INIT_CURVE, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_SpeedupTrigger_U2(void * pInstance,MS_U8 u8Loop)
{
    Dlc_FuncEnter();
    MDrv_DLC_SpeedupTrigger ( u8Loop );
    Dlc_FuncExit();
}

void MApi_XC_DLC_SpeedupTrigger (MS_U8 u8Loop)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SpeedupTrigger DLCArgs;
    DLCArgs.u8Loop = u8Loop;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SPEEDUP_TRIGGER, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


MS_BOOL MApi_XC_DLC_GetLumaCurveStatus_U2(void * pInstance)
{
    MS_BOOL bResult;

    Dlc_FuncEnter();
    bResult = MDrv_DLC_GetLumaCurveStatus();
    Dlc_FuncExit();
    return bResult;
}

MS_BOOL MApi_XC_DLC_GetLumaCurveStatus( void )
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_GetLumaCurveStatus DLCArgs;
    DLCArgs.bReturn = FALSE;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_LUMA_CURVE_STATUS, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


void MApi_XC_DLC_CGC_ResetCGain_U2(void * pInstance)
{
    Dlc_FuncEnter();
    g_u8DLC_CGC_CGain_Backup = MDrv_DLC_CGC_GetCGain();
    MDrv_DLC_CGC_ResetCGain();
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_ResetCGain(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_ResetCGain DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_RESET_C_GAIN, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_CheckCGainInPQCom_U2(void * pInstance)
{
    Dlc_FuncEnter();
    g_u8DLC_CGC_CGain = MDrv_DLC_CGC_GetCGain();
    if((g_u8DLC_CGC_CGain == 0xFF))
    {
        g_bDLC_CGC_CGainPQCom = FALSE;
        MDrv_DLC_CGC_SetCGain(g_u8DLC_CGC_CGain_Backup);
        g_u8DLC_CGC_CGain = g_u8DLC_CGC_CGain_Backup;
    }
    else
    {
        g_bDLC_CGC_CGainPQCom = TRUE;
#if 0
        if(g_u8DLC_CGC_CGain == 0x00)
        {
            g_bDLC_CGC_CGainPQCom = TRUE;
            MDrv_DLC_CGC_SetCGain(g_u8DLC_CGC_CGain_Backup);
            g_u8DLC_CGC_CGain = g_u8DLC_CGC_CGain_Backup;
        }
        else
        {
        g_bDLC_CGC_CGainPQCom = FALSE;
        }
#endif
    }
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_CheckCGainInPQCom(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_CheckCGainInPQCom DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_CHECK_C_GAIN_IN_PQ_COM, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_ResetYGain_U2(void * pInstance)
{
    Dlc_FuncEnter();
    g_u8DLC_CGC_YGain_Backup = MDrv_DLC_CGC_GetYGain();
    MDrv_DLC_CGC_ResetYGain();
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_ResetYGain(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_ResetYGain DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_RESET_Y_GAIN, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_CheckYGainInPQCom_U2(void * pInstance)
{
    Dlc_FuncEnter();
    g_u8DLC_CGC_YGain = MDrv_DLC_CGC_GetYGain();
    if((g_u8DLC_CGC_YGain == 0xFF))
    {
        g_bDLC_CGC_YGainPQCom = FALSE;
        MDrv_DLC_CGC_SetYGain(g_u8DLC_CGC_YGain_Backup);
        g_u8DLC_CGC_YGain = g_u8DLC_CGC_YGain_Backup;
    }
    else
    {
        g_bDLC_CGC_YGainPQCom = TRUE;
#if 0
        if(g_u8DLC_CGC_YGain == 0x00)
        {
            g_bDLC_CGC_YGainPQCom = TRUE;
            MDrv_DLC_CGC_SetYGain(g_u8DLC_CGC_YGain_Backup);
            g_u8DLC_CGC_YGain = g_u8DLC_CGC_YGain_Backup;
        }
        else
        {
        g_bDLC_CGC_YGainPQCom = FALSE;
        }
#endif
    }
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_CheckYGainInPQCom(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_CheckYGainInPQCom DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_CHECK_Y_GAIN_IN_PQ_COM, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_Reset_U2(void * pInstance)
{
    g_bDLC_CGC_ready = FALSE;
}

void MApi_XC_DLC_CGC_Reset(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_Reset DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_RESET, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_Init_U2(void * pInstance)
{
    Dlc_FuncEnter();
    if(g_bDLC_CGC_CGainPQCom)
    {
        msDlc_CGC_SetGain(g_u8DLC_CGC_CGain); //MDrv_DLC_CGC_SetCGain(g_u8DLC_CGC_CGain);
    }
    g_bDLC_CGC_ready = TRUE;
    MDrv_DLC_CGC_Init();
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_Init(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_Init DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_INIT, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_ReInit_U2(void * pInstance)
{
    Dlc_FuncEnter();
    MDrv_DLC_CGC_ReInit();
    Dlc_FuncExit();
}

void MApi_XC_DLC_CGC_ReInit(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_ReInit DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_REINIT, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_CGC_Handler_U2(void * pInstance)
{
    Dlc_FuncEnter();
    if(g_bDLC_CGC_ready)
    {
        MDrv_DLC_CGC_Handler();
    }
    Dlc_FuncExit();
}


void MApi_XC_DLC_CGC_Handler(void)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_CGC_Handler DLCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_CGC_HANDLER, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


E_XC_DLC_RESULT MApi_XC_DLC_GetLibVer_U2(void * pInstance,const MSIF_Version * * ppVersion)
{
    if (!ppVersion)
    {
        return E_XC_DLC_FAIL;
    }

    *ppVersion = &_api_xc_dlc_version;
    return E_XC_DLC_OK;
}

E_XC_DLC_RESULT MApi_XC_DLC_GetLibVer(const MSIF_Version **ppVersion)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return E_XC_DLC_FAIL;
        }
    }

    stDLC_GetLibVer DLCArgs;
    DLCArgs.ppVersion = ppVersion;
    DLCArgs.eResult = E_XC_DLC_FAIL;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_LIB_VER, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return E_XC_DLC_FAIL;
    }
    else
    {
        return DLCArgs.eResult;
    }
}


const XC_DLC_ApiInfo * MApi_XC_DLC_GetInfo_U2(void * pInstance)                             ///< Get info from driver
{
    return &_stXC_DLC_ApiInfo;
}

const XC_DLC_ApiInfo * MApi_XC_DLC_GetInfo(void)                                   ///< Get info from driver
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return 0;
        }
    }

    stDLC_GetInfo DLCArgs;
    DLCArgs.eResult=0;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_INFO, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return 0;
    }
    else
    {
        return DLCArgs.eResult;
    }
}


MS_BOOL MApi_XC_DLC_GetStatus_Ex_U2(void * pInstance,XC_DLC_ApiStatus * pDrvStatus,MS_BOOL bWindow)
{
    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit;
//    StuDbc_FinetuneParamaters_Lib DLC_DBC_MFinit;
    DLC_LUMADATA DLCLumaData;

    Dlc_FuncEnter();
    if(bWindow == 0)
    {
        MDrv_DLC_GetDLCInfo(&DLC_MFinit, sizeof(StuDlc_FinetuneParamaters));
        MDrv_DLC_GetLumaInfo( &DLCLumaData, sizeof(DLC_LUMADATA));

        for (i=0; i<16; ++i)
        {
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucLumaCurve[i]     = DLC_MFinit.ucLumaCurve[i];
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucLumaCurve2_a[i]  = DLC_MFinit.ucLumaCurve2_a[i];
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucLumaCurve2_b[i]  = DLC_MFinit.ucLumaCurve2_b[i];
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucLumaCurve2[i]    = DLC_MFinit.ucLumaCurve2[i];
        }

        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_L_L_U               = DLC_MFinit.u8_L_L_U;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_L_L_D               = DLC_MFinit.u8_L_L_D;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_L_H_U               = DLC_MFinit.u8_L_H_U;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_L_H_D               = DLC_MFinit.u8_L_H_D;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_S_L_U               = DLC_MFinit.u8_S_L_U;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_S_L_D               = DLC_MFinit.u8_S_L_D;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_S_H_U               = DLC_MFinit.u8_S_H_U;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.u8_S_H_D               = DLC_MFinit.u8_S_H_D ;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcPureImageMode     = DLC_MFinit.ucDlcPureImageMode;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcLevelLimit        = DLC_MFinit.ucDlcLevelLimit;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcAvgDelta          = DLC_MFinit.ucDlcAvgDelta;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcAvgDeltaStill     = DLC_MFinit.ucDlcAvgDeltaStill;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcFastAlphaBlending = DLC_MFinit.ucDlcFastAlphaBlending;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcSlowEvent         = DLC_MFinit.ucDlcSlowEvent;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcTimeOut           = DLC_MFinit.ucDlcTimeOut;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcFlickAlphaStart   = DLC_MFinit.ucDlcFlickAlphaStart;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcYAvgThresholdH    = DLC_MFinit.ucDlcYAvgThresholdH;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcYAvgThresholdL    = DLC_MFinit.ucDlcYAvgThresholdL;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcBLEPoint          = DLC_MFinit.ucDlcBLEPoint;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcWLEPoint          = DLC_MFinit.ucDlcWLEPoint;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.bEnableBLE             = DLC_MFinit.bEnableBLE;
        pDrvStatus->DLCinit.DLC_MFinit_Ex.bEnableWLE             = DLC_MFinit.bEnableWLE;

        if(pDrvStatus->DLCinit.DLC_MFinit_Ex.u32DLC_MFinit_Ex_Version>=1)
        {
            for (i=0; i<DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM; ++i)
            {
                pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcHistogramLimitCurve[i]  = DLC_MFinit.ucDlcHistogramLimitCurve[i];
            }

            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcYAvgThresholdM    = DLC_MFinit.ucDlcYAvgThresholdM;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcCurveMode         = DLC_MFinit.ucDlcCurveMode;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcCurveModeMixAlpha = DLC_MFinit.ucDlcCurveModeMixAlpha;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcAlgorithmMode     = DLC_MFinit.ucDlcAlgorithmMode;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcSepPointH         = DLC_MFinit.ucDlcSepPointH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcSepPointL         = DLC_MFinit.ucDlcSepPointL;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBleStartPointTH   = DLC_MFinit.uwDlcBleStartPointTH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBleEndPointTH     = DLC_MFinit.uwDlcBleEndPointTH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcCurveDiff_L_TH    = DLC_MFinit.ucDlcCurveDiff_L_TH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcCurveDiff_H_TH     = DLC_MFinit.ucDlcCurveDiff_H_TH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBLESlopPoint_1     = DLC_MFinit.uwDlcBLESlopPoint_1;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBLESlopPoint_2     = DLC_MFinit.uwDlcBLESlopPoint_2;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBLESlopPoint_3     = DLC_MFinit.uwDlcBLESlopPoint_3;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBLESlopPoint_4     = DLC_MFinit.uwDlcBLESlopPoint_4;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcBLESlopPoint_5     = DLC_MFinit.uwDlcBLESlopPoint_5;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.uwDlcDark_BLE_Slop_Min  = DLC_MFinit.uwDlcDark_BLE_Slop_Min;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcCurveDiffCoringTH  = DLC_MFinit.ucDlcCurveDiffCoringTH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcAlphaBlendingMin   = DLC_MFinit.ucDlcAlphaBlendingMin;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcAlphaBlendingMax   = DLC_MFinit.ucDlcAlphaBlendingMax;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcFlicker_alpha      = DLC_MFinit.ucDlcFlicker_alpha;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcYAVG_L_TH          = DLC_MFinit.ucDlcYAVG_L_TH;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcYAVG_H_TH          = DLC_MFinit.ucDlcYAVG_H_TH;

            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcDiffBase_L         = DLC_MFinit.ucDlcDiffBase_L;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcDiffBase_M         = DLC_MFinit.ucDlcDiffBase_M;
            pDrvStatus->DLCinit.DLC_MFinit_Ex.ucDlcDiffBase_H         = DLC_MFinit.ucDlcDiffBase_H;
        }

        for(i=0; i<32; i++)
        {
            pDrvStatus->DLCluma.g_wLumaHistogram32H[i]        = DLCLumaData.g_wLumaHistogram32H[i];
        }
        for(i=0; i<16; i++)
        {
           pDrvStatus->DLCluma.g_ucTable[i]                   = DLCLumaData.g_ucTable[i];
        }
        pDrvStatus->DLCluma.g_wLumiTotalCount                 = DLCLumaData.g_wLumiTotalCount;
        pDrvStatus->DLCluma.g_wLumiAverageTemp                = DLCLumaData.g_wLumiAverageTemp;
        pDrvStatus->DLCluma.g_ucHistogramMax                  = DLCLumaData.g_ucHistogramMax;
        pDrvStatus->DLCluma.g_ucHistogramMin                  = DLCLumaData.g_ucHistogramMin;
        pDrvStatus->u16DLC_MFVer                              = MDrv_DLC_GetMFVer();
        pDrvStatus->u16DLC_CGC_MFVer                          = MDrv_DLC_CGC_GetMFVer();

        // get DBC Info, this function has problem, mark out temprary
        //MDrv_DLC_GetDBCInfo( &DLC_DBC_MFinit, sizeof(StuDbc_FinetuneParamaters_Lib));
        pDrvStatus->u16DLC_DBC_MFVer                          = MDrv_DLC_DBC_GetMFVer();
    }
    Dlc_FuncExit();
    return TRUE;
}

MS_BOOL MApi_XC_DLC_GetStatus_Ex(XC_DLC_ApiStatus *pDrvStatus, MS_BOOL bWindow)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_GetStatus_Ex DLCArgs;
    DLCArgs.pDrvStatus = pDrvStatus;
    DLCArgs.bWindow = bWindow;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_STATUS_EX, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


MS_BOOL MApi_XC_DLC_SetDbgLevel_U2(void * pInstance,MS_U16 u16DbgSwitch)
{
    UNUSED(u16DbgSwitch);
    //_u16XC_DLCDbgSwitch = u16DbgSwitch;
//    _u16XC_DLCDbgSwitch = _u16XC_DLCDbgSwitch;
    return TRUE;
}

MS_BOOL MApi_XC_DLC_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_SetDbgLevel DLCArgs;
    DLCArgs.u16DbgSwitch = u16DbgSwitch;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_DBG_LEVEL, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


MS_BOOL MApi_XC_DLC_WriteCurve_U2(void * pInstance,MS_U8 * pu8Table)
{
    Dlc_FuncEnter();
    MDrv_DLC_WriteCurve(pu8Table);
    Dlc_FuncExit();
    return TRUE;
}

MS_BOOL MApi_XC_DLC_WriteCurve(MS_U8 *pu8Table)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_WriteCurve DLCArgs;
    DLCArgs.pu8Table = pu8Table;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_WRITE_CURVE, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


MS_BOOL MApi_XC_DLC_WriteCurve_Sub_U2(void * pInstance,MS_U8 * pu8Table)
{
    Dlc_FuncEnter();
    MDrv_DLC_WriteCurve_Sub(pu8Table);
    Dlc_FuncExit();
    return TRUE;
}

MS_BOOL MApi_XC_DLC_WriteCurve_Sub(MS_U8 *pu8Table)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_WriteCurve_Sub DLCArgs;
    DLCArgs.pu8Table = pu8Table;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_WRITE_CURVE_SUB, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


MS_BOOL MApi_XC_DLC_GetHistogram_U2(void * pInstance,MS_U16 * pu16Histogram,E_XC_DLC_HISTOGRAM_TYPE enHistogramType)
{
    MS_BOOL bret = FALSE;

    Dlc_FuncEnter();
    if(enHistogramType == E_XC_DLC_HISTOGRAM_32)
    {
        bret = MDrv_DLC_get_histogram32(pu16Histogram);
    }
    Dlc_FuncExit();
    return bret;
}

MS_BOOL MApi_XC_DLC_GetHistogram(MS_U16 *pu16Histogram, E_XC_DLC_HISTOGRAM_TYPE enHistogramType)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_GetHistogram DLCArgs;
    DLCArgs.pu16Histogram = pu16Histogram;
    DLCArgs.enHistogramType = enHistogramType;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_HISTOGRAM, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


MS_U16 gu16PNL_Width      = 0;
MS_U16 gu16PNL_Height     = 0;
MApi_XC_DLC_Print_Callback gfnPutchar = NULL;

void MApi_XC_DLC_DecodeExtCmd_U2(void * pInstance,tDLC_CONTROL_PARAMS * params)
{
    gu16PNL_Width      = params->u16PNL_Width;
    gu16PNL_Height     = params->u16PNL_Height;
    gfnPutchar         = params->fnDLC_Putchar;

    Dlc_FuncEnter();

    msDLC_ParametersTool(params->pCmdBuff);
    Dlc_FuncExit();
}

void MApi_XC_DLC_DecodeExtCmd(tDLC_CONTROL_PARAMS* params)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_DecodeExtCmd DLCArgs;
    DLCArgs.params = params;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_DECODE_EXT_CMD, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_SetCaptureRange_U2(void * pInstance,XC_DLC_CAPTURE_Range * pu16_Range)
{
    StuDbc_CAPTURE_Range DlcCapRange = {0};

    memcpy(&DlcCapRange, pu16_Range,sizeof(XC_DLC_CAPTURE_Range));

    Dlc_FuncEnter();

    MDrv_DLC_set_CaptureRange(&DlcCapRange);
    Dlc_FuncExit();

}

void MApi_XC_DLC_SetCaptureRange(XC_DLC_CAPTURE_Range *pu16_Range)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetCaptureRange DLCArgs;
    DLCArgs.pu16_Range = pu16_Range;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_CAPTURE_RANGE, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;

}


MS_BOOL MApi_XC_DLC_Init_U2(void * pInstance,XC_DLC_init * pstXC_DLC_InitData,MS_U32 u32InitDataLen)
{
    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit;
    MS_PHY u32NonPMBankSize = 0;
	MS_VIRT ptrDLCRiuBaseAddr=0;
    DLC_init_Ext DLC_InitExt;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &ptrDLCRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PM ) != TRUE)
    {
        //OS_DELETE_MUTEX(_s32ACEMutex);
        //PNL_ASSERT(0, "%s\n", "Get base address failed\n");
        //printf("XC_DLC GetBase failed\n");
        return FALSE;
    }
    else
    {
    }
    memset(&DLC_InitExt, 0 , sizeof(DLC_InitExt) );
    MDrv_DLC_init_riu_base( ptrDLCRiuBaseAddr);

    if(u32InitDataLen != sizeof(XC_DLC_init))
    {
        return FALSE;
    }
    memset(&DLC_MFinit, 0 , sizeof(DLC_MFinit) );

    for (i=0; i<16; ++i)
    {
        DLC_InitExt.uwLumaCurve[i]     = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve[i])<<2;
        DLC_InitExt.uwLumaCurve2_a[i]  = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve2_a[i])<<2;
        DLC_InitExt.uwLumaCurve2_b[i]  = ((MS_U16)pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve2_b[i])<<2;

        DLC_MFinit.ucLumaCurve[i]     = pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve[i];
        DLC_MFinit.ucLumaCurve2_a[i]  = pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve2_a[i];
        DLC_MFinit.ucLumaCurve2_b[i]  = pstXC_DLC_InitData->DLC_MFinit.ucLumaCurve2_b[i];
    }

#if (ENABLE_10_BIT_DLC)
    DLC_InitExt.b10BitsCruveEn        = pstXC_DLC_InitData->b10BitsEn;
#else
    DLC_InitExt.b10BitsCruveEn        = 0;
#endif

    DLC_MFinit.u8_L_L_U               = pstXC_DLC_InitData->DLC_MFinit.u8_L_L_U;
    DLC_MFinit.u8_L_L_D               = pstXC_DLC_InitData->DLC_MFinit.u8_L_L_D;
    DLC_MFinit.u8_L_H_U               = pstXC_DLC_InitData->DLC_MFinit.u8_L_H_U;
    DLC_MFinit.u8_L_H_D               = pstXC_DLC_InitData->DLC_MFinit.u8_L_H_D;
    DLC_MFinit.u8_S_L_U               = pstXC_DLC_InitData->DLC_MFinit.u8_S_L_U;
    DLC_MFinit.u8_S_L_D               = pstXC_DLC_InitData->DLC_MFinit.u8_S_L_D;
    DLC_MFinit.u8_S_H_U               = pstXC_DLC_InitData->DLC_MFinit.u8_S_H_U;
    DLC_MFinit.u8_S_H_D               = pstXC_DLC_InitData->DLC_MFinit.u8_S_H_D ;

    DLC_MFinit.ucDlcPureImageMode     = pstXC_DLC_InitData->DLC_MFinit.ucDlcPureImageMode;
    DLC_MFinit.ucDlcLevelLimit        = pstXC_DLC_InitData->DLC_MFinit.ucDlcLevelLimit;
    DLC_MFinit.ucDlcAvgDelta          = pstXC_DLC_InitData->DLC_MFinit.ucDlcAvgDelta;
    DLC_MFinit.ucDlcAvgDeltaStill     = pstXC_DLC_InitData->DLC_MFinit.ucDlcAvgDeltaStill;
    DLC_MFinit.ucDlcFastAlphaBlending = pstXC_DLC_InitData->DLC_MFinit.ucDlcFastAlphaBlending;
    DLC_MFinit.ucDlcYAvgThresholdL    = pstXC_DLC_InitData->DLC_MFinit.ucDlcYAvgThresholdL;
    DLC_MFinit.ucDlcYAvgThresholdH    = pstXC_DLC_InitData->DLC_MFinit.ucDlcYAvgThresholdH;
    DLC_MFinit.ucDlcBLEPoint          = pstXC_DLC_InitData->DLC_MFinit.ucDlcBLEPoint;
    DLC_MFinit.ucDlcWLEPoint          = pstXC_DLC_InitData->DLC_MFinit.ucDlcWLEPoint;
    DLC_MFinit.bEnableBLE             = pstXC_DLC_InitData->DLC_MFinit.bEnableBLE;
    DLC_MFinit.bEnableWLE             = pstXC_DLC_InitData->DLC_MFinit.bEnableWLE;
    DLC_MFinit.ucCGCYth               = pstXC_DLC_InitData->DLC_MFinit.ucCGCYth;
    DLC_MFinit.bCGCCGainCtrl          = pstXC_DLC_InitData->DLC_MFinit.bCGCCGainCtrl;

    // CGC
    DLC_MFinit.ucCGCCGain_offset      = pstXC_DLC_InitData->DLC_MFinit.ucCGCCGain_offset;
    DLC_MFinit.ucCGCChroma_GainLimitH = pstXC_DLC_InitData->DLC_MFinit.ucCGCChroma_GainLimitH;
    DLC_MFinit.ucCGCChroma_GainLimitL = pstXC_DLC_InitData->DLC_MFinit.ucCGCChroma_GainLimitL;
    DLC_MFinit.ucCGCYCslope           = pstXC_DLC_InitData->DLC_MFinit.ucCGCYCslope;

    MDrv_DLC_Init(DLC_MFinit);
    MDrv_DLC_Init_Ext(&DLC_InitExt);

    g_DlcCapRange_Suspend.wHStart = pstXC_DLC_InitData->u16CurveHStart;
    g_DlcCapRange_Suspend.wHEnd   = pstXC_DLC_InitData->u16CurveHEnd;
    g_DlcCapRange_Suspend.wVStart = pstXC_DLC_InitData->u16CurveVStart;
    g_DlcCapRange_Suspend.wVEnd   = pstXC_DLC_InitData->u16CurveVEnd;

    Dlc_FuncEnter();
    MDrv_DLC_InitCurve (pstXC_DLC_InitData->u16CurveHStart, pstXC_DLC_InitData->u16CurveHEnd, pstXC_DLC_InitData->u16CurveVStart, pstXC_DLC_InitData->u16CurveVEnd);
    Dlc_FuncExit();

    return TRUE;
}

MS_BOOL MApi_XC_DLC_Init(XC_DLC_init *pstXC_DLC_InitData, MS_U32 u32InitDataLen)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_Init DLCArgs;
    DLCArgs.pstXC_DLC_InitData = pstXC_DLC_InitData;
    DLCArgs.u32InitDataLen = u32InitDataLen;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_INIT, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


//-------------------------------------------------------------------------------------------------
/// Setting DLC
// @param  DLC_MFinit             \b IN: DLC data
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_SetSetting_U2(void * pInstance,XC_DLC_MFinit DLC_MFinit)
{

    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit_new;

    memset(&DLC_MFinit_new, 0 , sizeof(DLC_MFinit_new) );
    for (i=0; i<16; ++i)
    {
        DLC_MFinit_new.ucLumaCurve[i]     = DLC_MFinit.ucLumaCurve[i];
        DLC_MFinit_new.ucLumaCurve2_a[i]  = DLC_MFinit.ucLumaCurve2_a[i];
        DLC_MFinit_new.ucLumaCurve2_b[i]  = DLC_MFinit.ucLumaCurve2_b[i];
    }

    DLC_MFinit_new.u8_L_L_U               = DLC_MFinit.u8_L_L_U;
    DLC_MFinit_new.u8_L_L_D               = DLC_MFinit.u8_L_L_D;
    DLC_MFinit_new.u8_L_H_U               = DLC_MFinit.u8_L_H_U;
    DLC_MFinit_new.u8_L_H_D               = DLC_MFinit.u8_L_H_D;
    DLC_MFinit_new.u8_S_L_U               = DLC_MFinit.u8_S_L_U;
    DLC_MFinit_new.u8_S_L_D               = DLC_MFinit.u8_S_L_D;
    DLC_MFinit_new.u8_S_H_U               = DLC_MFinit.u8_S_H_U;
    DLC_MFinit_new.u8_S_H_D               = DLC_MFinit.u8_S_H_D ;

    DLC_MFinit_new.ucDlcPureImageMode     = DLC_MFinit.ucDlcPureImageMode;
    DLC_MFinit_new.ucDlcLevelLimit        = DLC_MFinit.ucDlcLevelLimit;
    DLC_MFinit_new.ucDlcAvgDelta          = DLC_MFinit.ucDlcAvgDelta;
    DLC_MFinit_new.ucDlcAvgDeltaStill     = DLC_MFinit.ucDlcAvgDeltaStill;
    DLC_MFinit_new.ucDlcFastAlphaBlending = DLC_MFinit.ucDlcFastAlphaBlending;
    DLC_MFinit_new.ucDlcYAvgThresholdL    = DLC_MFinit.ucDlcYAvgThresholdL;
    DLC_MFinit_new.ucDlcYAvgThresholdH    = DLC_MFinit.ucDlcYAvgThresholdH;
    DLC_MFinit_new.ucDlcBLEPoint          = DLC_MFinit.ucDlcBLEPoint;
    DLC_MFinit_new.ucDlcWLEPoint          = DLC_MFinit.ucDlcWLEPoint;
    DLC_MFinit_new.bEnableBLE             = DLC_MFinit.bEnableBLE;
    DLC_MFinit_new.bEnableWLE             = DLC_MFinit.bEnableWLE;

    DLC_MFinit_new.ucCGCYth                  = DLC_MFinit.ucCGCYth;
    DLC_MFinit_new.bCGCCGainCtrl             = DLC_MFinit.bCGCCGainCtrl;

    // CGC
    DLC_MFinit_new.ucCGCCGain_offset         = DLC_MFinit.ucCGCCGain_offset;
    DLC_MFinit_new.ucCGCChroma_GainLimitH    = DLC_MFinit.ucCGCChroma_GainLimitH;
    DLC_MFinit_new.ucCGCChroma_GainLimitL    = DLC_MFinit.ucCGCChroma_GainLimitL;
    DLC_MFinit_new.ucCGCYCslope              = DLC_MFinit.ucCGCYCslope;

    MDrv_DLC_Init(DLC_MFinit_new);
}

void MApi_XC_DLC_SetSetting(XC_DLC_MFinit DLC_MFinit)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return ;
        }
    }

    stDLC_SetSetting DLCArgs;
    DLCArgs.DLC_MFinit = DLC_MFinit;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_SET_SETTING, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
    }

    return ;
}


MS_BOOL MApi_XC_DLC_GetStatus_U2(void * pInstance,XC_DLC_ApiStatus * pDrvStatus,MS_BOOL bWindow)
{
    MS_U8 i;
    StuDlc_FinetuneParamaters DLC_MFinit;
//    StuDbc_FinetuneParamaters_Lib DLC_DBC_MFinit;
    DLC_LUMADATA DLCLumaData;

    Dlc_FuncEnter();
    if(bWindow == 0)
    {
        MDrv_DLC_GetDLCInfo(&DLC_MFinit, sizeof(StuDlc_FinetuneParamaters));
        MDrv_DLC_GetLumaInfo( &DLCLumaData, sizeof(DLC_LUMADATA));

        for (i=0; i<16; ++i)
        {
            pDrvStatus->DLCinit.DLC_MFinit.ucLumaCurve[i]     = DLC_MFinit.ucLumaCurve[i];
            pDrvStatus->DLCinit.DLC_MFinit.ucLumaCurve2_a[i]  = DLC_MFinit.ucLumaCurve2_a[i];
            pDrvStatus->DLCinit.DLC_MFinit.ucLumaCurve2_b[i]  = DLC_MFinit.ucLumaCurve2_b[i];
            pDrvStatus->DLCinit.DLC_MFinit.ucLumaCurve2[i]    = DLC_MFinit.ucLumaCurve2[i];
        }

        pDrvStatus->DLCinit.DLC_MFinit.u8_L_L_U               = DLC_MFinit.u8_L_L_U;
        pDrvStatus->DLCinit.DLC_MFinit.u8_L_L_D               = DLC_MFinit.u8_L_L_D;
        pDrvStatus->DLCinit.DLC_MFinit.u8_L_H_U               = DLC_MFinit.u8_L_H_U;
        pDrvStatus->DLCinit.DLC_MFinit.u8_L_H_D               = DLC_MFinit.u8_L_H_D;
        pDrvStatus->DLCinit.DLC_MFinit.u8_S_L_U               = DLC_MFinit.u8_S_L_U;
        pDrvStatus->DLCinit.DLC_MFinit.u8_S_L_D               = DLC_MFinit.u8_S_L_D;
        pDrvStatus->DLCinit.DLC_MFinit.u8_S_H_U               = DLC_MFinit.u8_S_H_U;
        pDrvStatus->DLCinit.DLC_MFinit.u8_S_H_D               = DLC_MFinit.u8_S_H_D ;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcPureImageMode     = DLC_MFinit.ucDlcPureImageMode;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcLevelLimit        = DLC_MFinit.ucDlcLevelLimit;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcAvgDelta          = DLC_MFinit.ucDlcAvgDelta;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcAvgDeltaStill     = DLC_MFinit.ucDlcAvgDeltaStill;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcFastAlphaBlending = DLC_MFinit.ucDlcFastAlphaBlending;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcSlowEvent         = DLC_MFinit.ucDlcSlowEvent;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcTimeOut           = DLC_MFinit.ucDlcTimeOut;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcFlickAlphaStart   = DLC_MFinit.ucDlcFlickAlphaStart;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcYAvgThresholdH    = DLC_MFinit.ucDlcYAvgThresholdH;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcYAvgThresholdL    = DLC_MFinit.ucDlcYAvgThresholdL;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcBLEPoint          = DLC_MFinit.ucDlcBLEPoint;
        pDrvStatus->DLCinit.DLC_MFinit.ucDlcWLEPoint          = DLC_MFinit.ucDlcWLEPoint;
        pDrvStatus->DLCinit.DLC_MFinit.bEnableBLE             = DLC_MFinit.bEnableBLE;
        pDrvStatus->DLCinit.DLC_MFinit.bEnableWLE             = DLC_MFinit.bEnableWLE;

        for(i=0; i<32; i++)
        {
            pDrvStatus->DLCluma.g_wLumaHistogram32H[i]        = DLCLumaData.g_wLumaHistogram32H[i];
        }
        for(i=0; i<16; i++)
        {
           pDrvStatus->DLCluma.g_ucTable[i]                   = DLCLumaData.g_ucTable[i];
        }
        pDrvStatus->DLCluma.g_wLumiTotalCount                 = DLCLumaData.g_wLumiTotalCount;
        pDrvStatus->DLCluma.g_wLumiAverageTemp                = DLCLumaData.g_wLumiAverageTemp;
        pDrvStatus->DLCluma.g_ucHistogramMax                  = DLCLumaData.g_ucHistogramMax;
        pDrvStatus->DLCluma.g_ucHistogramMin                  = DLCLumaData.g_ucHistogramMin;
        pDrvStatus->u16DLC_MFVer                              = MDrv_DLC_GetMFVer();
        pDrvStatus->u16DLC_CGC_MFVer                          = MDrv_DLC_CGC_GetMFVer();

        // get DBC Info, this function has problem, mark out temprary
        //MDrv_DLC_GetDBCInfo( &DLC_DBC_MFinit, sizeof(StuDbc_FinetuneParamaters_Lib));
        pDrvStatus->u16DLC_DBC_MFVer                          = MDrv_DLC_DBC_GetMFVer();
    }
    Dlc_FuncExit();
    return TRUE;
}

MS_BOOL MApi_XC_DLC_GetStatus(XC_DLC_ApiStatus *pDrvStatus, MS_BOOL bWindow)
{
    if (pu32DLCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DLCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DLC failed\n");
            return FALSE;
        }
    }

    stDLC_GetStatus DLCArgs;
    DLCArgs.pDrvStatus = pDrvStatus;
    DLCArgs.bWindow = bWindow;
    DLCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DLCInst, E_DLC_CMD_GET_STATUS, (void*)&DLCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DLC engine fail\n");
        return FALSE;
    }
    else
    {
        return DLCArgs.bReturn;
    }
}


/******************************************************************************/
// API DLC DBC library start
/******************************************************************************/
void MApi_XC_DLC_DBC_Init_U2(void * pInstance,XC_DLC_DBC_MFinit DLC_DBC_MFinit)
{
    StuDbc_FinetuneParamaters_Lib DbcParameters = {0};
    DbcParameters.ucMax_Video           = DLC_DBC_MFinit.ucMax_Video;
    DbcParameters.ucMid_Video           = DLC_DBC_MFinit.ucMid_Video;
    DbcParameters.ucMin_Video           = DLC_DBC_MFinit.ucMin_Video;
    DbcParameters.ucMaxPWM              = DLC_DBC_MFinit.ucMaxPWM;
    DbcParameters.ucBackLight_Thres     = DLC_DBC_MFinit.ucBackLight_Thres;
    DbcParameters.ucMinPWM              = DLC_DBC_MFinit.ucMinPWM;
    DbcParameters.ucAlpha               = DLC_DBC_MFinit.ucAlpha;
    DbcParameters.ucAvgDelta            = DLC_DBC_MFinit.ucAvgDelta;
    DbcParameters.ucFastAlphaBlending   = DLC_DBC_MFinit.ucFastAlphaBlending;
    DbcParameters.ucLoop_Dly_H_Init     = DLC_DBC_MFinit.ucLoop_Dly_H_Init;
    DbcParameters.ucLoop_Dly_MH_Init    = DLC_DBC_MFinit.ucLoop_Dly_MH_Init;
    DbcParameters.ucLoop_Dly_ML_Init    = DLC_DBC_MFinit.ucLoop_Dly_ML_Init;
    DbcParameters.ucLoop_Dly_L_Init     = DLC_DBC_MFinit.ucLoop_Dly_L_Init;
    DbcParameters.bYGainCtrl            = DLC_DBC_MFinit.bYGainCtrl;
    DbcParameters.bCGainCtrl            = DLC_DBC_MFinit.bCGainCtrl;


    MDrv_DLC_DBC_Init(DbcParameters);
    g_bDLC_DBC_ready = FALSE;
    g_bDBCEnable = FALSE;
}

void MApi_XC_DLC_DBC_Init(XC_DLC_DBC_MFinit DLC_DBC_MFinit)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_DLC_Init DBCArgs;
    DBCArgs.DLC_DBC_MFinit = DLC_DBC_MFinit;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_DLC_INIT, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DBC_Init_U2(void * pInstanc)
{
    msDBCInit();
}

void MApi_XC_DBC_Init(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_Init DBCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_INIT, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_DBC_Setstatus_U2(void * pInstance,MS_BOOL bDBCEnable)
{
    g_bDBCEnable = bDBCEnable;
}

void MApi_XC_DLC_DBC_Setstatus(MS_BOOL bDBCEnable)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_Setstatus DBCArgs;
    DBCArgs.bDBCEnable = bDBCEnable;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_SET_STATUS, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


MS_BOOL MApi_XC_DLC_DBC_Getstatus_U2(void * pInstance)
{
    return g_bDBCEnable;
}

MS_BOOL MApi_XC_DLC_DBC_Getstatus(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return FALSE;
        }
    }

    stDBC_Getstatus DBCArgs;
    DBCArgs.bReturn =FALSE;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_GET_STATUS, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
        return FALSE;
    }
    else
    {
        return DBCArgs.bReturn;
    }
}


void MApi_XC_DLC_DBC_SetReady_U2(void * pInstance,MS_BOOL bDBCReady)
{
    g_bDLC_DBC_ready = bDBCReady;
}

void MApi_XC_DLC_DBC_SetReady(MS_BOOL bDBCReady)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_SetReady DBCArgs;
    DBCArgs.bDBCReady = bDBCReady;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_SET_READY, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_DBC_SetDebugMode_U2(void * pInstance,MS_U8 ucDBC_DebugMode)
{
    MDrv_DLC_DBC_SetDebugMode(ucDBC_DebugMode);
}

void MApi_XC_DLC_DBC_SetDebugMode(MS_U8 ucDBC_DebugMode)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_SetDebugMode DBCArgs;
    DBCArgs.ucDBC_DebugMode = ucDBC_DebugMode;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_SET_DEBUG_MODE, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


MS_U8 MApi_XC_DLC_DBC_GetDebugMode_U2(void * pInstance)
{
    return MDrv_DLC_DBC_GetDebugMode();
}

MS_U8 MApi_XC_DLC_DBC_GetDebugMode(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return 0;
        }
    }

    stDBC_GetDebugMode DBCArgs;
    DBCArgs.u8Return =0;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_GET_DEBUG_MODE, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
        return 0;
    }
    else
    {
        return DBCArgs.u8Return;
    }
}


void MApi_XC_DLC_DBC_UpdatePWM_U2(void * pInstance,MS_U8 u8PWMvalue)
{
    MDrv_DLC_DBC_UpdatePWM(u8PWMvalue);
}

void MApi_XC_DLC_DBC_UpdatePWM(MS_U8 u8PWMvalue)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_UpdatePWM DBCArgs;
    DBCArgs.u8PWMvalue = u8PWMvalue;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_UPDATE_PWM, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


MS_U16 MApi_XC_DLC_DBC_Handler_U2(void * pInstance)
{
    if(g_bDLC_DBC_ready && g_bDBCEnable)
        return MDrv_DLC_DBC_Handler();
    else
        return 0xFFFF;
}

MS_U16 MApi_XC_DLC_DBC_Handler(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return 0;
        }
    }

    stDBC_Handler DBCArgs;
    DBCArgs.u16Return =0;
    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_HANDLER, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
        return 0;
    }
    else
    {
        return DBCArgs.u16Return;
    }
}


void MApi_XC_DLC_DBC_AdjustYCGain_U2(void * pInstance)
{
        MDrv_DLC_DBC_AdjustYCGain();
}

void MApi_XC_DLC_DBC_AdjustYCGain(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_AdjustYCGain DBCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_ADJUST_YC_GAIN, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_DBC_YCGainInit_U2(void * pInstance,MS_U8 u8YGain_M,MS_U8 u8YGain_L,MS_U8 u8CGain_M,MS_U8 u8CGain_L)
{
    MS_U8 u8YGain_H, u8CGain_H;
    if(g_bDLC_CGC_YGainPQCom)
    {
        u8YGain_H = g_u8DLC_CGC_YGain;
    }
    else
    {
        u8YGain_H = MDrv_DLC_CGC_GetYGain();
    }
    if(g_bDLC_CGC_CGainPQCom)
    {
        u8CGain_H = g_u8DLC_CGC_CGain;
    }
    else
    {
        u8CGain_H = MDrv_DLC_CGC_GetCGain();
    }
    msAdjustYCGain(u8YGain_H, u8CGain_H);

    MDrv_DLC_DBC_YCGainInit(u8YGain_M, u8YGain_L, u8CGain_M, u8CGain_L, u8YGain_H, u8CGain_H);
    g_bDLC_DBC_ready = TRUE;
}

void MApi_XC_DLC_DBC_YCGainInit(MS_U8 u8YGain_M, MS_U8 u8YGain_L, MS_U8 u8CGain_M, MS_U8 u8CGain_L)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_YCGainInit DBCArgs;
    DBCArgs.u8YGain_M = u8YGain_M;
    DBCArgs.u8YGain_L = u8YGain_L;
    DBCArgs.u8CGain_M = u8CGain_M;
    DBCArgs.u8CGain_L = u8CGain_L;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_YC_GAIN_INIT, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DLC_DBC_Reset_U2(void * pInstanc)
{
    g_bDLC_DBC_ready = FALSE;
}

void MApi_XC_DLC_DBC_Reset(void)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_Reset DBCArgs;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_RESET, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}


void MApi_XC_DBC_DecodeExtCmd_U2(void * pInstance,tDBC_CONTROL_PARAMS * pValue)
{
    msDBC_ParametersTool(pValue->pCmdBuff, pValue->fnDBC_AP_OnOff, pValue->fnDBC_AdjustBacklight, pValue->fnDBC_Putchar);
}

void MApi_XC_DBC_DecodeExtCmd(tDBC_CONTROL_PARAMS* pValue )
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return ;
        }
    }

    stDBC_DecodeExtCmd DBCArgs;
    DBCArgs.params = pValue;


    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DBC_CMD_DECODE_EXT_CMD, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
    }

    return ;
}

#define LOGTIMECOST  FALSE
MS_U32 MApi_XC_DLC_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState)
{
    static EN_POWER_MODE _enPrevPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;

#if LOGTIMECOST
    MS_U32 u32Begin = MsOS_GetSystemTime();
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
            MDrv_DLC_InitCurve(g_DlcCapRange_Suspend.wHStart, g_DlcCapRange_Suspend.wHEnd, g_DlcCapRange_Suspend.wVStart, g_DlcCapRange_Suspend.wVEnd); //to avoid dead lock , need to verify futhur
            _enPrevPowerState = enPowerState;
            u32Ret = UTOPIA_STATUS_SUCCESS;
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

MS_U32 MApi_XC_DLC_SetPowerState(EN_POWER_MODE enPowerState)
{
    if (pu32DBCInst == NULL)
    {
        if(UtopiaOpen(MODULE_DLC, &pu32DBCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DBC failed\n");
            return UTOPIA_STATUS_FAIL;
        }
    }

    stDLC_SetPowerState DBCArgs;
    DBCArgs.enPowerState = enPowerState;
    DBCArgs.u32Return = UTOPIA_STATUS_FAIL;

    // wait until Jesse.Huang set the definition of return value, I will exchange this with another return value
    // instead of 0
    if(UtopiaIoctl(pu32DBCInst, E_DLC_CMD_SET_POWERSTATE, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DBC engine fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    return DBCArgs.u32Return;
}

/******************************************************************************/
// API DLC DBC library end
/******************************************************************************/

#undef _API_XC_DLC_C_
#endif  // _API_XC_DLC_C_

