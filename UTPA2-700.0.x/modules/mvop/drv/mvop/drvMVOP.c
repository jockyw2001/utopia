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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// drvMVOP.c
/// @brief MPEG Video Output Processor (MVOP)
/// @author MStarSemi Inc.
///
/// Display controller(DC) unit (also called Video Output Processor(VOP)).
/// DC read DRAM MVD decode data and send it to back-end processor.
/// The frame rate could be programmed by registers.
/// The sync. generator(SG) generates the SYNC signal like HSYNC, VSYNC, FIELD.
/// It also generate the standard CCIR656 data.
///
/// Features
///  - Output mode : interlace or progressive
///  - Maximum Display resolution 1920x1080
///  - Support MVD interlace or progressive YUV420 image
///  - one read channel to MIU ,  each request always work with 16 words( 8Y8UV)
///  - high MIU read priority when UV buffer data depth under 32 or 16.
///  - CCIR656 support
///  - Horizontal, Vertical duplicate support.
///  - MVD Image size resize mode
///
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsDevice.h"
#include "utopia.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#else
#include <string.h>
#endif
#include "drvMVOP.h"

// Internal Definition
#include "regMVOP.h"

#define MVOP_ENABLE_LINK_WEAK_SYMBOL    1
#if MVOP_ENABLE_LINK_WEAK_SYMBOL
#include "_drvMVOP_weak.h"
#else
#include "halMVOP.h"
#endif

#if ENABLE_UTOPIA2_INTERFACE
#include "drvMVOP_v2.h"
#include "utopia.h"
#endif

#include "drvMMIO.h"
#include "drvMIU.h"
#include "osalMVOP.h"
#include "drvSYS.h"
#include "utopia_dapi.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
//#define _ENABLE_DC1_FRAMESYNC //Notice: Scaler should disable auto-detection when using this.


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define ENABLE_EXTRA_BLANKING     0
// define MAX blanking area reserved for video shift down right for frame bufferless usage
#if ENABLE_EXTRA_BLANKING
#define EXTRA_H_BLANKING          0
#define EXTRA_V_BLANKING          64
#else
#define EXTRA_H_BLANKING          0
#define EXTRA_V_BLANKING          0
#endif

#define MVOP_VBlank_Min 45
#define MVOP_HBlank_Min 32
//for variable v/h blanking
#if 0
#define MVOP_VBlank               45
#define MVOP_HBlank_SD            200
#define MVOP_HBlank_HD            300
#endif
#define DC_BaseClock              86400 //86.4*1000

#define HSize_4K                  3840
#define VSize_4K                  2160
#define FrameRate_60000           60000
#define FrameRate_59940           59940
#define FrameRate_50000           50000
#define FrameRate_48000           48000
#define FrameRate_30000           30000
#define FrameRate_29970           29970
#define FrameRate_25000           25000
#define FrameRate_24000           24000
#define FrameRate_23976           23976

#define Optimized_VBlank_4K_60000 53
#define Optimized_VBlank_4K_59940 70
#define Optimized_VBlank_4K_50000 340
#define Optimized_VBlank_4K_48000 282
#define Optimized_VBlank_4K_23976 257

#define MIN_MVOP_FREQ             27000000ul
#define MAX_MVOP_FREQ             86400000ul
#define MVOP_FREQ_144M           144000000ul
#define MVOP_FREQ_160M           160000000ul
#define MVOP_FREQ_320M           320000000ul
#define MIN_MVOP_FREERUN_FREQ     31000000ul
#if defined(STB_DC_MODE)||(STB_DC)
#define STB_OUTPUT_MAX_FPS       60000
#endif

#define MVOP_MIN_FRAMERATE       1000   //1 frame/second

#define MVOP_VBlank_Min_P 20
#define MVOP_VBlank_Min_I 35

#define ALIGN_UPTO_16(x)  ((((x) + 15) >> 4) << 4)
#define ALIGN_UPTO_4(x)   ((((x) +  3) >> 2) << 2)
#define ALIGN_UPTO_2(x)   ((((x) +  1) >> 1) << 1)
#define ALIGN_DOWNTO_8(x) (((x) >> 3) << 3)
#define ALIGN_DOWNTO_4(x) (((x) >> 2) << 2)

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#if 1
#include "ULog.h"
#define DMVOP_PRINTF(format, args...) ULOGD("Utopia", format, ##args)
#define IMVOP_PRINTF(format, args...) ULOGI("Utopia", format, ##args)
#define EMVOP_PRINTF(format, args...) ULOGE("Utopia", format, ##args)
#define MVOP_PRINTF(format, args...) ULOGE("Utopia", format, ##args)
#else

#ifndef ANDROID
#define MVOP_PRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define MVOP_PRINTF ALOGD
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U16 MVOP_HBlank_HD = 300;
MS_U16 MVOP_HBlank_SD = 200;
MS_U16 MVOP_VBlank  = 45;
MS_BOOL EnablePresetTable = FALSE;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/// Version string
static MSIF_Version _drv_mvop_version =
{
    .DDI = { MVOP_DRV_VERSION },
};

typedef MS_BOOL (*HAL_SetU16Para)(MS_U16 u16Para);
typedef void (*HAL_RegSetBase)(MS_VIRT u32Base);

typedef struct __attribute__((packed))
{
    MVOP_DrvStatus _stMVOPDrvStat;
    MVOP_Timing    _mvopTiming;
    MS_U16         _u16FixVtt;
    MS_U16         _u16SrcHSize;        //source width (may be unaligned)
    MS_U16         _u16SrcVSize;        //source height(may be unaligned)
    MS_U16         _u16InsVBlank;       //inserted V-Blank between 3D L & R
    MS_BOOL        _bEnableFreerunMode;
    MS_BOOL        _bEnMinVSize576;     //used by MVOP_AutoGenMPEGTiming
    MS_BOOL        _bEnVSizeDup;        //used by MVOP_AutoGenMPEGTiming
    MS_BOOL        _bEnVSizex4Dup;        //used by MVOP_AutoGenMPEGTiming
    MS_BOOL        _bEnHSizex4Dup;        //used by MVOP_AutoGenMPEGTiming
    MVOP_RgbFormat _eRgbFmt;
    MVOP_InputSel  _eInputSel;
    HAL_SetU16Para _fpHalSet3DLRInsVBlank;
#if !defined (__aarch64__)
    MS_U32 u32AlignDummy;
#endif
    MVOP_InputCfg  _mvopInputCfg;
    MVOP_ComdFlag  _mvopComdFlag;
    MVOP_VidStat   _mvopOutputCfg;
    MS_BOOL        _bEnHx2Dup;
    MS_BOOL        _bEnHDS;
    MS_BOOL        _bEn265DV;
    MS_BOOL        _bGOPPatch;
    MS_BOOL        _bIsDVDual;
}
MVOP_DrvCtrlSt;

static MS_U8 _u8DebugMsgLevel = 0;

typedef struct __attribute__((packed))
{
    MVOP_DrvCtrlSt _stMVOPDrvCtrl[HAL_MVOP_MODULE_CNT];
    MVOP_DrvInfo _stMVOPDrvInfo;
#if defined(STB_DC_MODE)||(STB_DC)
    MS_BOOL _bRegSizeFromMVDStatus;
    MS_U8 reserved8_1[3];
    MS_U16 _u16Width;
    MS_U16 _u16Height;
#endif
    MS_U8 _u8DebugMsgLevel;
    MS_U8 reserved8_0[3];
#ifdef SUPPORT_KERNAL_STR
    MVOP_STR_SAVE_AREA pMVOP_STRPrivate;
// if pointer need it
//#if !defined (__aarch64__)
    //MS_U32 u32AlignDummy;
//#endif
#endif
}
MVOP_CTX_DRV;

static MVOP_CTX_DRV *p_gDrvMVOPCtx = NULL;
static MVOP_CTX_DRV *p_gDrvSubMVOPCtx = NULL;

#ifndef MSOS_TYPE_LINUX
static MVOP_CTX_DRV g_stmvopDrvCtx;
static MVOP_CTX_DRV g_stsubmvopDrvCtx;
#endif


#if 0
static MVOP_DrvCtrlSt _stMVOPDrvCtrl[HAL_MVOP_MODULE_CNT];
static MVOP_DrvInfo _stMVOPDrvInfo;
#if STB_DC
static MS_BOOL _bRegSizeFromMVDStatus =TRUE;
static MS_U16 _u16Width = 0;
static MS_U16 _u16Height = 0;
#endif
#endif

#define MVOP_GetDrvCtrl(_eDevID) ((_eDevID < HAL_MVOP_MODULE_CNT) ? (&p_gDrvMVOPCtx->_stMVOPDrvCtrl[(_eDevID)]) : NULL)
#define MVOP_GetSubDrvCtrl(_eDevID) ((_eDevID < HAL_MVOP_MODULE_CNT) ? (&p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[(_eDevID)]) : NULL)

#define MVOP_IsInit()((NULL == p_gDrvMVOPCtx)?FALSE:(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit == TRUE))
#define MVOP_CheckIsInit()                                                     \
        do {                                                                   \
            if (!MVOP_IsInit())                                                \
            {                                                                  \
                MVOP_DBG(MVOP_PRINTF("%s: drvMVOP is not init!\n", __FUNCTION__);)  \
                return;                                                        \
            }                                                                  \
        } while(0)

#if 1//MVOP_SUPPORT_SUB
#define MVOP_SubIsInit()(((MVOP_SUPPORT_SUB == 1) && (NULL !=p_gDrvSubMVOPCtx))?(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit == TRUE):FALSE)
#define MVOP_SubCheckIsInit()                                                     \
        do {                                                                      \
            if (!MVOP_SubIsInit())                                                \
            {                                                                     \
                MVOP_DBG(MVOP_PRINTF("%s: drvMVOPSub is not init!\n", __FUNCTION__);)  \
                return;                                                           \
            }                                                                     \
        } while(0)
#define MVOP_SubCheckIsInit_RetZero()                                                     \
        do {                                                                      \
            if (!MVOP_SubIsInit())                                                \
            {                                                                     \
                MVOP_DBG(MVOP_PRINTF("%s: drvMVOPSub is not init!\n", __FUNCTION__);)  \
                return 0;                                                         \
            }                                                                     \
        } while(0)
#endif

#if 1//MVOP_SUPPORT_3RD
#define MVOP_EX_IsInit(_eDevID) (((MVOP_GetDrvCtrl(_eDevID))&&(NULL != p_gDrvMVOPCtx)) ? (p_gDrvMVOPCtx->_stMVOPDrvCtrl[_eDevID]._stMVOPDrvStat.bIsInit == TRUE)  : FALSE)
#define MVOP_EX_CheckIsInit(_eDevID)                                              \
        do {                                                                      \
            if (!MVOP_EX_IsInit(_eDevID))                                         \
            {                                                                     \
                MVOP_DBG(MVOP_PRINTF("%s: drvMVOP3rd is not init!\n", __FUNCTION__);)  \
                return E_MVOP_NOT_INIT;                                           \
            }                                                                     \
        } while(0)
#endif

typedef enum
{
    E_MVOP_INIT_OK            = 0,
    E_MVOP_INIT_FAIL          = 1,
    E_MVOP_INIT_ALREADY_EXIST = 2
} MVOP_DrvInitCtxResults;

//Utopia 2.0
#define MVOP_MAX_SUPPORT_STREAM_NUM     1
void* pu32MVOP_Inst[MVOP_MAX_SUPPORT_STREAM_NUM] = {NULL};

//Utopia Kernel Mode
#ifdef MVOP_UTOPIA2K
#define MVOP_DRIVER_BASE KERNEL_MODE
#else
#define MVOP_DRIVER_BASE 0
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#if 1
#define MVOP_DBG(x)                if (_u8DebugMsgLevel>0)  { D##x };
#define MVOP_TIMING_DBG(x)         if (_u8DebugMsgLevel>1)  { D##x };
#define MVOP_INPUT_DBG(x)          if (_u8DebugMsgLevel>2)  { D##x };
#define MVOP_DBG_VERBAL(x)         if (_u8DebugMsgLevel>3)  { D##x };
#define MVOP_DBG_TRACE(x)          if (_u8DebugMsgLevel>8)  { I##x };
#define MVOP_ERR(x)                { E##x };
#else
#define MVOP_DBG(x)                if (_u8DebugMsgLevel>0)  { x };
#define MVOP_TIMING_DBG(x)         if (_u8DebugMsgLevel>1)  { x };
#define MVOP_INPUT_DBG(x)          if (_u8DebugMsgLevel>2)  { x };
#define MVOP_DBG_VERBAL(x)         if (_u8DebugMsgLevel>3)  { x };
#define MVOP_DBG_TRACE(x)          if (_u8DebugMsgLevel>8)  { x };
#define MVOP_ERR(x)                { x };
#endif
//---------------------------------- Mutex settings -----------------------------------------
#ifdef ENABLE_MUTEX
#if MVOP_SUPPORT_SUB
static MS_S32 s32MVOPMutexID[HAL_MVOP_MODULE_CNT] = {-1,-1};
static MS_U8 _u8MVOP_Mutex[HAL_MVOP_MODULE_CNT][32] = { "MVOP_Mutex","MVOP_SubMutex"};
#elif  MVOP_SUPPORT_3RD
static MS_S32 s32MVOPMutexID[HAL_MVOP_MODULE_CNT] = {-1,-1,-1};
static MS_U8 _u8MVOP_Mutex[HAL_MVOP_MODULE_CNT][32] = { "MVOP_Mutex","MVOP_SubMutex","MVOP_3rdMutex" };
#else
static MS_S32 s32MVOPMutexID[HAL_MVOP_MODULE_CNT] = {-1};
static MS_U8 _u8MVOP_Mutex[HAL_MVOP_MODULE_CNT][32] = { "MVOP_Mutex" };
#endif

#define _DRV_MVOP_MutexCreate(_eDevID)                                  \
    do                                                          \
    {                                                           \
        if (s32MVOPMutexID[_eDevID] < 0)                                  \
        {                                                       \
            s32MVOPMutexID[_eDevID] = OSAL_MVOP_MutexCreate(_u8MVOP_Mutex[_eDevID]); \
        }                                                       \
    } while (0)

#define _DRV_MVOP_MutexDelete(_eDevID)                                  \
    do                                                          \
    {                                                           \
        if (s32MVOPMutexID[_eDevID] >= 0)                                 \
        {                                                       \
            OSAL_MVOP_MutexDelete(s32MVOPMutexID[_eDevID]);                \
            s32MVOPMutexID[_eDevID] = -1;                                 \
        }                                                       \
    } while (0)

#define  _DRV_MVOP_Entry(_eDevID)                                                       \
    do                                                                          \
    {                                                                           \
        if (s32MVOPMutexID[_eDevID] >= 0)                                                 \
        {                                                                       \
            if (!OSAL_MVOP_MutexObtain(s32MVOPMutexID[_eDevID], OSAL_MVOP_MUTEX_TIMEOUT))   \
            {                                                                   \
                MVOP_PRINTF("[HAL MVOP][%06d] Mutex taking timeout\n", __LINE__);     \
            }                                                                   \
        }                                                                       \
    } while (0)

#define _DRV_MVOP_Return(_eDevID,_ret_)                                  \
    do                                                          \
    {                                                           \
        if (s32MVOPMutexID[_eDevID] >= 0)                                 \
        {                                                       \
            OSAL_MVOP_MutexRelease(s32MVOPMutexID[_eDevID]);               \
        }                                                       \
        return _ret_;                                           \
    } while(0)

#define _DRV_MVOP_Release(_eDevID)                                      \
    do                                                          \
    {                                                           \
        if (s32MVOPMutexID[_eDevID] >= 0)                                 \
        {                                                       \
            OSAL_MVOP_MutexRelease(s32MVOPMutexID[_eDevID]);               \
        }                                                       \
    } while (0)

#else

#define _DRV_MVOP_MutexCreate(_eDevID)
#define _DRV_MVOP_MutexDelete(_eDevID)
#define _DRV_MVOP_Entry(_eDevID)
#define _DRV_MVOP_Return(_eDevID,_ret)      {return _ret;}
#define _DRV_MVOP_Release(_eDevID)

#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MVOP_DrvInitCtxResults _MVOP_SubInit_Context(MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bNeedInitShared = FALSE;

    if (p_gDrvSubMVOPCtx)
    {
        // The context instance has been created already
        // before somewhere sometime in the same process.
        *pbFirstDrvInstant = bNeedInitShared;
        return E_MVOP_INIT_OK;
    }

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)

    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;

    //MsOS_SHM_Init(); init in msos_init
    if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux Sub MVOP DRV driver", sizeof(MVOP_CTX_DRV), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux Sub MVOP DRV driver", sizeof(MVOP_CTX_DRV), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            MVOP_DBG(MVOP_PRINTF("MVOP: SHM allocation failed!\n"););

            return E_MVOP_INIT_FAIL;
        }
        else
        {
#if defined (__aarch64__)
            MVOP_DBG(MVOP_PRINTF("MVOP: [%s][%d] This is first initial 0x%lx\n", __FUNCTION__, __LINE__, u32Addr););
#else
            MVOP_DBG(MVOP_PRINTF("MVOP: [%s][%d] This is first initial 0x%x\n", __FUNCTION__, __LINE__, (int)u32Addr););
#endif
            //memset((MS_U8 *)u32Addr, 0, sizeof(MVOP_CTX_DRV));
            bNeedInitShared = TRUE;
            p_gDrvSubMVOPCtx = (MVOP_CTX_DRV *)u32Addr;
            memset(p_gDrvSubMVOPCtx, 0, sizeof(*p_gDrvMVOPCtx));
        }
    }
    else
    {
        p_gDrvSubMVOPCtx = (MVOP_CTX_DRV *)u32Addr;
        bNeedInitShared = TRUE;
        *pbFirstDrvInstant = bNeedInitShared;

        return E_MVOP_INIT_ALREADY_EXIST;
    }
#else
    p_gDrvSubMVOPCtx =  &g_stsubmvopDrvCtx;
    bNeedInitShared = TRUE;
#endif
    *pbFirstDrvInstant = bNeedInitShared;
    if (bNeedInitShared)
    {
        memset(p_gDrvSubMVOPCtx, 0, sizeof(*p_gDrvMVOPCtx));
    }
    //init MVOP Value

    return E_MVOP_INIT_OK;
}

#if MVOP_SUPPORT_SUB
static void _MVOP_SubInitVar(MVOP_DevID eID)
{
    if (NULL == p_gDrvSubMVOPCtx)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetSubDrvCtrl(eID);
    MVOP_DrvStatus* pstDrvStat = NULL;
#if defined(STB_DC_MODE)||(STB_DC)
    p_gDrvSubMVOPCtx->_bRegSizeFromMVDStatus =TRUE;
    p_gDrvSubMVOPCtx->_u16Width = 0;
    p_gDrvSubMVOPCtx->_u16Height = 0;
#endif
    //p_gDrvMVOPCtx->_u8DebugMsgLevel = &_u8DebugMsgLevel_tmp;

    if (pstDrvCtrl)
    {
        pstDrvStat = &pstDrvCtrl->_stMVOPDrvStat;
        pstDrvStat->bIsInit    = FALSE;
        pstDrvStat->bIsEnable  = FALSE;
        pstDrvStat->bIsUVShift = FALSE;
        pstDrvStat->bIsBlackBG = FALSE;
        pstDrvStat->bIsMonoMode = FALSE;
        pstDrvStat->bIsSetTiming = FALSE;
        pstDrvCtrl->_bEnableFreerunMode = FALSE;
        pstDrvCtrl->_bEnMinVSize576 = TRUE;
        pstDrvCtrl->_bEnVSizeDup = FALSE;
        pstDrvCtrl->_bEnVSizex4Dup = FALSE;
        pstDrvCtrl->_bEnHSizex4Dup = FALSE;
        pstDrvCtrl->_u16FixVtt = 0;
        memset(&pstDrvCtrl->_mvopTiming, 0, sizeof(MVOP_Timing));
        pstDrvCtrl->_u16SrcHSize = 0;
        pstDrvCtrl->_u16SrcVSize = 0;
        pstDrvCtrl->_u16InsVBlank= 0;
        pstDrvCtrl->_eRgbFmt = E_MVOP_RGB_NONE;
        pstDrvCtrl->_eInputSel = MVOP_INPUT_UNKNOWN;
        memset(&pstDrvCtrl->_mvopOutputCfg, 0, sizeof(MVOP_VidStat));
        pstDrvCtrl->_bEnHx2Dup = FALSE;
        pstDrvCtrl->_bEnHDS = FALSE;
        pstDrvCtrl->_bEn265DV = FALSE;
        pstDrvCtrl->_bGOPPatch = TRUE;
        pstDrvCtrl->_bIsDVDual = TRUE;

#if SUPPORT_3DLR_INST_VBLANK
        HAL_SetU16Para fpSetU16Para = NULL;
        switch (eID)
        {
            case E_MVOP_DEV_0:
                fpSetU16Para = HAL_MVOP_Set3DLRInsVBlank;
                break;
            case E_MVOP_DEV_1:
                fpSetU16Para = HAL_MVOP_SubSet3DLRInsVBlank;
                break;
            case E_MVOP_DEV_2:
                fpSetU16Para = HAL_MVOP_EX_Set3DLRInsVBlank;
                break;
            default:
                fpSetU16Para = HAL_MVOP_Set3DLRInsVBlank;
                break;
        }
        pstDrvCtrl->_fpHalSet3DLRInsVBlank = fpSetU16Para;
#endif
    }
}
#endif

MVOP_DrvInitCtxResults _MVOP_Init_Context(MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bNeedInitShared = FALSE;

#if 0
    if (p_gDrvMVOPCtx)
    {
        // The context instance has been created already
        // before somewhere sometime in the same process.
        *pbFirstDrvInstant = bNeedInitShared;
        return E_MVOP_INIT_OK;
    }
#endif

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)

    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;

    //MsOS_SHM_Init(); init in msos_init
    if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVOP DRV driver", sizeof(MVOP_CTX_DRV), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVOP DRV driver", sizeof(MVOP_CTX_DRV), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            MVOP_DBG(MVOP_PRINTF("MVOP: SHM allocation failed!\n"););

            return E_MVOP_INIT_FAIL;
        }
        else
        {
#if defined (__aarch64__)
            MVOP_DBG(MVOP_PRINTF("MVOP: [%s][%d] This is first initial 0x%lx\n", __FUNCTION__, __LINE__, u32Addr););
#else
            MVOP_DBG(MVOP_PRINTF("MVOP: [%s][%d] This is first initial 0x%x\n", __FUNCTION__, __LINE__, (int)u32Addr););
#endif
            //memset( (MS_U8*)u32Addr, 0, sizeof(MVOP_CTX_DRV));
            bNeedInitShared = TRUE;
            p_gDrvMVOPCtx = (MVOP_CTX_DRV *)u32Addr;
            memset(p_gDrvMVOPCtx, 0, sizeof(*p_gDrvMVOPCtx));
        }
    }
    else
    {
        MVOP_DBG(MVOP_PRINTF("MVOP: Not first initialized\n");)
        p_gDrvMVOPCtx = (MVOP_CTX_DRV *)u32Addr;
        bNeedInitShared = FALSE;
        *pbFirstDrvInstant = bNeedInitShared;

        return E_MVOP_INIT_ALREADY_EXIST;
    }
#else
    p_gDrvMVOPCtx =  &g_stmvopDrvCtx;
    bNeedInitShared = TRUE;
#endif
    *pbFirstDrvInstant = bNeedInitShared;
    if (bNeedInitShared)
    {
        memset(p_gDrvMVOPCtx, 0, sizeof(*p_gDrvMVOPCtx));
    }

    return E_MVOP_INIT_OK;
}

static void _MVOP_InitVar(MVOP_DevID eID)
{
    if (NULL == p_gDrvMVOPCtx)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    MVOP_DrvStatus* pstDrvStat = NULL;
#if defined(STB_DC_MODE)||(STB_DC)
    p_gDrvMVOPCtx->_bRegSizeFromMVDStatus =TRUE;
    p_gDrvMVOPCtx->_u16Width = 0;
    p_gDrvMVOPCtx->_u16Height = 0;
#endif
    //p_gDrvMVOPCtx->_u8DebugMsgLevel = &_u8DebugMsgLevel_tmp;

    if (pstDrvCtrl)
    {
        pstDrvStat = &pstDrvCtrl->_stMVOPDrvStat;
        pstDrvStat->bIsInit    = FALSE;
        pstDrvStat->bIsEnable  = FALSE;
        pstDrvStat->bIsUVShift = FALSE;
        pstDrvStat->bIsBlackBG = FALSE;
        pstDrvStat->bIsMonoMode = FALSE;
        pstDrvStat->bIsSetTiming = FALSE;
        pstDrvCtrl->_bEnableFreerunMode = FALSE;
        pstDrvCtrl->_bEnMinVSize576 = TRUE;
        pstDrvCtrl->_bEnVSizeDup = FALSE;
        pstDrvCtrl->_bEnVSizex4Dup = FALSE;
        pstDrvCtrl->_bEnHSizex4Dup = FALSE;
        pstDrvCtrl->_u16FixVtt = 0;
        memset(&pstDrvCtrl->_mvopTiming, 0, sizeof(MVOP_Timing));
        pstDrvCtrl->_u16SrcHSize = 0;
        pstDrvCtrl->_u16SrcVSize = 0;
        pstDrvCtrl->_u16InsVBlank= 0;
        pstDrvCtrl->_eRgbFmt = E_MVOP_RGB_NONE;
        pstDrvCtrl->_eInputSel = MVOP_INPUT_UNKNOWN;
        memset(&pstDrvCtrl->_mvopOutputCfg, 0, sizeof(MVOP_VidStat));
        pstDrvCtrl->_bEnHx2Dup = FALSE;
        pstDrvCtrl->_bGOPPatch = TRUE;
        pstDrvCtrl->_bIsDVDual = TRUE;

#if SUPPORT_3DLR_INST_VBLANK
        HAL_SetU16Para fpSetU16Para = NULL;
        switch (eID)
        {
            case E_MVOP_DEV_0:
                fpSetU16Para = HAL_MVOP_Set3DLRInsVBlank;
                break;
            case E_MVOP_DEV_1:
                fpSetU16Para = HAL_MVOP_SubSet3DLRInsVBlank;
                break;
            case E_MVOP_DEV_2:
                fpSetU16Para = HAL_MVOP_EX_Set3DLRInsVBlank;
                break;
            default:
                fpSetU16Para = HAL_MVOP_Set3DLRInsVBlank;
                break;
        }
        pstDrvCtrl->_fpHalSet3DLRInsVBlank = fpSetU16Para;
#endif
    }
}

static MS_BOOL MVOP_SetRegBaseAdd(HAL_RegSetBase fpRegSetBase)
{
    MS_VIRT u32NonPMBankAdd;
    MS_PHY u32NonPMBankSize;

    if (TRUE == MDrv_MMIO_GetBASE(&u32NonPMBankAdd, &u32NonPMBankSize, MS_MODULE_MVOP))
    {
#if defined (__aarch64__)
        MVOP_DBG(MVOP_PRINTF("%s u32RiuBaseAdd = %lx\n", __FUNCTION__, u32NonPMBankAdd););
#else
        MVOP_DBG(MVOP_PRINTF("%s u32RiuBaseAdd = %x\n", __FUNCTION__, (int)u32NonPMBankAdd););
#endif

        if (fpRegSetBase)
        {
            fpRegSetBase(u32NonPMBankAdd);
            return TRUE;
        }
        else
        {
            MVOP_DBG(MVOP_PRINTF("%s IOMap failure due to null fptr\n", __FUNCTION__););
        }
    }
    else
    {
        MVOP_DBG(MVOP_PRINTF("%s IOMap failure due to MDrv_MMIO_GetBASE\n", __FUNCTION__););
    }
    return FALSE;
}

static MS_U16 MVOP_CalculateHSize(MS_U16* pu16SrcHSize, MS_U16 u16OriHSize, MS_U16 u16HOffset)
{
    MS_U16 u16HSize = 0;

    if (pu16SrcHSize == NULL)
    {
        return 0;
    }

#if (MVOP_BUS_WIDTH == _MVOP_128BIT_BUS)
    {
        u16HSize = ALIGN_UPTO_16(u16OriHSize);
        if (u16HSize != u16OriHSize)
        {
            *pu16SrcHSize = u16OriHSize;
        }
    }
#else
    {
        u16HSize = ALIGN_DOWNTO_8(u16OriHSize);
    }
#endif

#ifdef _SUPPORT_IMG_OFFSET_
    u16HSize += (u16HOffset * 2);
#endif

    return u16HSize;
}

static MS_BOOL MVOP_NeedAdjustVSize(MVOP_DevID eID)
{
    MVOP_DrvMirror eMir = E_VOPMIRROR_NONE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;
    MS_BOOL bNeedAdjust = FALSE;

    if(eID == E_MVOP_DEV_0)
    {
        if (NULL == p_gDrvMVOPCtx)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
    else if(eID == E_MVOP_DEV_1)
    {
        if (NULL == p_gDrvSubMVOPCtx)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
    if(eID == E_MVOP_DEV_0)
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    else if(eID ==E_MVOP_DEV_1)
        pstDrvCtrl = MVOP_GetSubDrvCtrl(eID);

    if (NULL == pstDrvCtrl)
    {
        MVOP_DBG(MVOP_PRINTF("%s null pstDrvCtrl for eID=%x\n", __FUNCTION__, eID););
        return bNeedAdjust;
    }

    if (HAL_MVOP_GetMirrorMode)
    {
        eMir = HAL_MVOP_GetMirrorMode(eID);
    }

    //For MCU mode and VMirror is on, VSize need to be 4 multiple.
    bNeedAdjust = ( ((MVOP_INPUT_DRAM == pstDrvCtrl->_eInputSel) ||
                     (MVOP_INPUT_DRAM_3DLR == pstDrvCtrl->_eInputSel)) &&
                    ((E_VOPMIRROR_VERTICAL == eMir) || (E_VOPMIRROR_HVBOTH == eMir)) );

    return bNeedAdjust;
}

static MS_U16 MVOP_CalculateVSize(MVOP_DevID eID, MS_U16* pu16SrcVSize, MS_U16 u16OriVSize, MS_U16 u16VOffset)
{
    MS_U16 u16VSize = 0;

    if (pu16SrcVSize == NULL)
    {
        return 0;
    }

    if (TRUE == MVOP_NeedAdjustVSize(eID))
    {
        u16VSize = ALIGN_UPTO_4(u16OriVSize);
        if (u16VSize != u16OriVSize)
        {
            *pu16SrcVSize = u16OriVSize;
        }
        MVOP_DBG(MVOP_PRINTF("%s %d %d\n", __FUNCTION__, u16OriVSize, u16VSize););
    }
    else
    {
        u16VSize = u16OriVSize;
    }

#ifdef _SUPPORT_IMG_OFFSET_
    u16VSize += (u16VOffset * 2);
#endif

    return u16VSize;
}

//-------------------------------------------------------------------------------------------------
// Calculate MaxFactor.
// @return MaxFactor
//-------------------------------------------------------------------------------------------------
static MS_U32 _MaxFactor(MS_U32 u32Num1, MS_U32 u32Num2)
{
    MS_U32 remainder, quotient;

#if defined (__aarch64__)
    MVOP_DBG_VERBAL(MVOP_PRINTF("u32Num1=%x u32Num2=%x\n", u32Num1, u32Num2);)
#else
    MVOP_DBG_VERBAL(MVOP_PRINTF("u32Num1=%x u32Num2=%x\n", (int)u32Num1, (int)u32Num2);)
#endif
    MS_ASSERT(u32Num1 != 0);
    if (u32Num1 == 0) //no need to check u32Num2 because u32Num2 is always 86400
    {
        MVOP_ERR(MVOP_PRINTF("%s(%d): invalid parameter!\n", __FILE__, __LINE__);)
        return 0; //to avoid infinite loop
    }

    while (1)
    {
        if (u32Num1 > u32Num2)
        {
            if ((u32Num1%u32Num2) == 0)
            {
#if defined (__aarch64__)
                MVOP_DBG_VERBAL(MVOP_PRINTF("ret0 u32Num1=%x u32Num2=%x\n", u32Num1, u32Num2);)
#else
                MVOP_DBG_VERBAL(MVOP_PRINTF("ret0 u32Num1=%x u32Num2=%x\n", (int)u32Num1, (int)u32Num2);)
#endif
                return u32Num2;
            }
            else
            {
                quotient = u32Num1/u32Num2;
                remainder = u32Num1- (u32Num2*quotient);

                u32Num1= u32Num2;
                u32Num2= remainder;
            }
        }
        else
        {
            if ((u32Num2%u32Num1) == 0)
            {
#if defined (__aarch64__)
                MVOP_DBG_VERBAL(MVOP_PRINTF("ret1 u32Num1=%x u32Num2=%x\n", u32Num1, u32Num2);)
#else
                MVOP_DBG_VERBAL(MVOP_PRINTF("ret1 u32Num1=%x u32Num2=%x\n", (int)u32Num1, (int)u32Num2);)
#endif
                return u32Num1;
            }
            else
            {
                quotient = u32Num2/u32Num1;
                remainder = u32Num2 - (u32Num1*quotient);

                u32Num2 = u32Num1;
                u32Num1 = remainder;
            }
        }
    }
}

static MS_U16 MVOP_GetMaxVOffset(MVOP_DevID eID, HALMVOPFREQUENCY eFreq, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_U16 u16VttMax, u16OffsetMax = 0;
    MS_U16* pu16FixVtt = NULL;

    if(eID >= HAL_MVOP_MODULE_CNT)
    {
        MVOP_DBG(MVOP_PRINTF("%s MVOP Device ID is out of MVOP module number\n", __FUNCTION__);)
        return 0;
    }
    else if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return 0;
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return 0;
        }
    }
#endif

    else if((u16Fps == 0) || (u16HSize == 0) || (u16VSize == 0))
    {
        MVOP_DBG(MVOP_PRINTF("[WARNING]%s framerate/Hsize/Vsize is 0\n", __FUNCTION__);)
        return 0;
    }

    if(eID == E_MVOP_DEV_0)
        pu16FixVtt = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
        pu16FixVtt = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#endif

    if(pu16FixVtt == NULL)
    {
        MVOP_ERR(MVOP_PRINTF("Err: FATAL  deference problem in MVOP_EX_AutoGenTiming[%d]!\n",__LINE__);)
        return 0; //CID 53640
    }
    if(*pu16FixVtt > 0)
    {
        u16VttMax = *pu16FixVtt;
    }
    else
    {
        u16VttMax = eFreq / u16Fps*1000 / (u16HSize + MVOP_HBlank_HD);
    }
    if(u16VttMax < (MVOP_VBlank + u16VSize))
    {
        MVOP_DBG(MVOP_PRINTF("[WARNING]%s The mvop clk is not support\n", __FUNCTION__);)
        return 0;
    }
    u16OffsetMax = (u16VttMax - MVOP_VBlank - u16VSize)/2;

    //MVOP_PRINTF("V_OffsetMax=%d\n", u16OffsetMax);
    return u16OffsetMax;
}

static void MVOP_EX_AutoGenTiming(MVOP_DevID eID,
                                  MVOP_Timing *pVOPTiming,
                                  MVOP_VidStat *pstVideoStatus,
                                  HALMVOPFREQUENCY eClkFreq,
                                  MS_BOOL bEn3DLR)
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 bInterlace;
    MS_U16 VB0, VB1;
    MS_U16 u8VSyncOffset = 10;
    MS_S16 iVBlank = 0;
    MS_U16 *pu16SrcHSize = NULL;
    MS_U16 *pu16SrcVSize = NULL;
    MS_U16* pu16FixVtt = NULL;

    if(eID >= HAL_MVOP_MODULE_CNT)
    {
        MVOP_DBG(MVOP_PRINTF("%s MVOP Device ID is out of MVOP module number\n", __FUNCTION__);)
        return;
    }
    else if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    }
#endif

    if(eID == E_MVOP_DEV_0)
    {
        pu16SrcHSize = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        pu16SrcHSize = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#endif

    u16HSize = MVOP_CalculateHSize(pu16SrcHSize, pstVideoStatus->u16HorSize, pstVideoStatus->u16HorOffset);
    u16VSize = MVOP_CalculateVSize(eID, pu16SrcVSize, pstVideoStatus->u16VerSize, pstVideoStatus->u16VerOffset);
    pVOPTiming->u16Width  = u16HSize;
    pVOPTiming->u16Height = u16VSize;

    if(pstVideoStatus->u8Interlace > E_MVOP_PRO)
        bInterlace = TRUE;
    else
        bInterlace = FALSE;

    MS_ASSERT(pstVideoStatus->u16FrameRate!=0);
    if(pstVideoStatus->u16FrameRate == 0)// for parasoft check divide by zero.
    {
        return;
    }

    {
        //to avoid (Vtotal-Vde)>(Vde/10) for scaler FBL mode.
        if(eID == E_MVOP_DEV_0)
            pu16FixVtt = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#if MVOP_SUPPORT_SUB
        else if(eID == E_MVOP_DEV_1)
            pu16FixVtt = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#endif

        if(pu16FixVtt == NULL)
        {
            MVOP_ERR(MVOP_PRINTF("Err: FATAL  deference problem in MVOP_EX_AutoGenTiming[%d]!\n",__LINE__);)
            return;
        }
        if(*pu16FixVtt > 0)
        {
            pVOPTiming->u16V_TotalCount = *pu16FixVtt;
            *pu16FixVtt = 0;
        }
        else
        {
            pVOPTiming->u16V_TotalCount = u16VSize + MVOP_VBlank;
        }

        pVOPTiming->u16H_TotalCount = (MS_U32)eClkFreq / pstVideoStatus->u16FrameRate
                                      *1000 / pVOPTiming->u16V_TotalCount;
        pVOPTiming->u16H_TotalCount = (pVOPTiming->u16H_TotalCount + 0x1) & ~0x1;
        pVOPTiming->u16HActive_Start = pVOPTiming->u16H_TotalCount - u16HSize;
        //MVOP_PRINTF("=== Htt=%d Has=%d Hsize=%d\n", pVOPTiming->u16H_TotalCount, pVOPTiming->u16HActive_Start, u16HSize);
        //Aiken's protection for FBL ZOOM, Horizontal Blanking can not less than MVOP_HBlank_Min
        if((int)pVOPTiming->u16HActive_Start < MVOP_HBlank_Min) // avoid unsigned rounding, may be negative
        {
            MVOP_ERR(MVOP_PRINTF(" Error !! Horizontal Blanking can not less than %d \n", MVOP_HBlank_Min);)
            pVOPTiming->u16HActive_Start = MVOP_HBlank_Min;
            pVOPTiming->u16H_TotalCount =  pVOPTiming->u16HActive_Start + u16HSize;
            pVOPTiming->u16V_TotalCount = (MS_U32)eClkFreq / pstVideoStatus->u16FrameRate * 1000 / pVOPTiming->u16H_TotalCount;
            iVBlank = pVOPTiming->u16V_TotalCount - u16VSize;
            // interlace video need ensure thant VB = VB0 + VB1
            // here, also need ensure vop's freqency should less than VOP_FREQ_144M/VOP_FREQ_160M
            if(bInterlace && ((iVBlank & 0x1) == 0))
            {
                pVOPTiming->u16V_TotalCount--;
                iVBlank--;
            }
            if(iVBlank < MVOP_VBlank_Min)
            {
                MVOP_ERR(MVOP_PRINTF(" Error !! Vertical Blanking can not less than %d \n", MVOP_VBlank_Min);)
                u16VSize = u16VSize - (MVOP_VBlank_Min - iVBlank);
                pVOPTiming->u16Height = u16VSize;
            }
        }
#ifdef _SUPPORT_IMG_OFFSET_
        pVOPTiming->u16HImg_Start = pVOPTiming->u16HActive_Start + pstVideoStatus->u16HorOffset;
        //MVOP_PRINTF("=== His=%d\n", pVOPTiming->u16HImg_Start);
#endif
        if(pVOPTiming->u16H_TotalCount >= 4095)
            MVOP_PRINTF("[Warning] xc support pVOPTiming->u16H_TotalCount >= 4095 after CL 712803\n");
    }

    if(pstVideoStatus->u8Interlace > E_MVOP_PRO)
        pVOPTiming->bInterlace = TRUE;
    else
        pVOPTiming->bInterlace = FALSE;

    if (bInterlace)
    {
        VB0 = (pVOPTiming->u16V_TotalCount - u16VSize) / 2 + 1; // VOP_VBlank/2+1 + EXTRA_V_BLANKING/2;
        VB1 = VB0 - 1;

        if (ENABLE == bEn3DLR)
        {
            VB1 = VB0;
            pVOPTiming->u16V_TotalCount++;
            MVOP_DBG(MVOP_PRINTF("%s 3DLR interlace: force VB0=VB1=0x%x, Vtt+1=0x%x\n", __FUNCTION__, VB1, pVOPTiming->u16V_TotalCount);)
        }

        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = pVOPTiming->u16VBlank0_End + (u16VSize/2);
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
    }
    else
    {
        VB1 = VB0 = (pVOPTiming->u16V_TotalCount - u16VSize);   //VB1 = VB0 for progressive
        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = 1;
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
    }
#ifdef _SUPPORT_IMG_OFFSET_
    if (bInterlace)
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset/2;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset/2;
    }
    else
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset;
    }
#endif
    pVOPTiming->u16TopField_VS = pVOPTiming->u16VBlank0_Start + u8VSyncOffset;
    pVOPTiming->u16BottomField_VS = pVOPTiming->u16VBlank1_Start + u8VSyncOffset;
    if (pVOPTiming->bInterlace)
    {
        //Do not adjust BottomField_VStart for 3D L/R interlace source
        if (ENABLE != bEn3DLR)
        {
            pVOPTiming->u16BottomField_VS -=1;
        }
    }

    pVOPTiming->u16TopField_Start = pVOPTiming->u16TopField_VS;
    pVOPTiming->u16BottomField_Start = pVOPTiming->u16BottomField_VS;

    // u8Framerate & u16H_Freq will be assigned to g_SrcInfo.*
    // in other place and cannot be removed
    pVOPTiming->u8Framerate = pstVideoStatus->u16FrameRate/1000;
    pVOPTiming->u16ExpFrameRate = pstVideoStatus->u16FrameRate;

    // u16H_Freq (unit: 100Hz)
    pVOPTiming->u16H_Freq = (MS_U16)
                            ((MS_U32)pVOPTiming->u16V_TotalCount *
                             (MS_U32)pstVideoStatus->u16FrameRate/100000);

    // for dc1 ...
    pVOPTiming->u16Num = 1;
    pVOPTiming->u16Den = 1;

    MVOP_TIMING_DBG(MVOP_PRINTF("\n>>>>> MVOP_EX output eClkFreq=%d\n", eClkFreq);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> w=%d, h=%d, i=%d, fr=%d\n", pVOPTiming->u16Width,
                                pVOPTiming->u16Height, pVOPTiming->bInterlace, pVOPTiming->u16ExpFrameRate);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hTotal=%d, vTotal=%d\n",
                                pVOPTiming->u16H_TotalCount, pVOPTiming->u16V_TotalCount);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hActive=%d VB0=%d VB1=%d\n",
                                pVOPTiming->u16HActive_Start, VB0, VB1);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> ImgHSt=%d ImgVSt0=%d ImgVSt1=%d\n", pVOPTiming->u16HImg_Start, pVOPTiming->u16VImg_Start0, pVOPTiming->u16VImg_Start1);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hOffset=%d vOffset=%d\n", pstVideoStatus->u16HorOffset, pstVideoStatus->u16VerOffset);)
}


static void MVOP_AutoGenTiming(MS_BOOL bIsMain,
                               MVOP_Timing *pVOPTiming,
                               MVOP_VidStat *pstVideoStatus,
                               MS_U64 u64CLK,
                               MS_BOOL b4k2k2p)
{
#ifdef _ENABLE_DC1_FRAMESYNC
    MS_U32 u32R, u32Q;
    MS_U32 u32MaxFactor;
    MS_U32 MVOP_Clock;
#endif
#if ENABLE_3D_LR_MODE
    MS_BOOL b3DTB = DISABLE;
#endif
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 bInterlace;
    MS_U16 VB0, VB1;
    MS_U16 u8VSyncOffset = 10;
    MS_S16 iVBlank = 0;
    MVOP_DevID eID = E_MVOP_DEV_0;
    MS_U16 *pu16SrcHSize = NULL;
    MS_U16 *pu16SrcVSize = NULL;
    MS_U16 *pu16FixVtt = NULL;
    MS_U32 u32MVOPCLK = 0;
    MS_U16 u16HSizeTiming = 0;


#if MVOP_SUPPORT_SUB
    if (FALSE == bIsMain)
    {
        eID = E_MVOP_DEV_1;
    }
#endif

    if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    }
#endif

    if(eID == E_MVOP_DEV_0)
    {
        pu16SrcHSize = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        pu16SrcHSize = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#endif

    u16HSize = MVOP_CalculateHSize(pu16SrcHSize, pstVideoStatus->u16HorSize, pstVideoStatus->u16HorOffset);
    u16VSize = MVOP_CalculateVSize(eID, pu16SrcVSize, pstVideoStatus->u16VerSize, pstVideoStatus->u16VerOffset);
    pVOPTiming->u16Width  = u16HSize;
    pVOPTiming->u16Height = u16VSize;
    pVOPTiming->bHDuplicate = 0;
    if(pstVideoStatus->u8Interlace > E_MVOP_PRO)
        bInterlace = TRUE;
    else
        bInterlace = FALSE;
    u32MVOPCLK = (MS_U32)u64CLK;
    if(b4k2k2p)
        u16HSizeTiming = u16HSize >> 1;
    else
        u16HSizeTiming = u16HSize;

    MS_ASSERT(pstVideoStatus->u16FrameRate!=0);
    if(pstVideoStatus->u16FrameRate == 0)// for parasoft check divide by zero.
    {
        return;
    }

    {
        //to avoid (Vtotal-Vde)>(Vde/10) for scaler FBL mode.

        if(eID == E_MVOP_DEV_0)
            pu16FixVtt = & p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#if MVOP_SUPPORT_SUB
        else if(eID == E_MVOP_DEV_1)
            pu16FixVtt = & p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#endif

        if(*pu16FixVtt > 0)
        {
            pVOPTiming->u16V_TotalCount = *pu16FixVtt;
            *pu16FixVtt = 0;
        }
        else
        {
            pVOPTiming->u16V_TotalCount = u16VSize + MVOP_VBlank;
        }
        if (bIsMain)
        {
            if(EnablePresetTable)
            {
                pVOPTiming->u16H_TotalCount = u32MVOPCLK*10/ pstVideoStatus->u16FrameRate
                                               *100 / pVOPTiming->u16V_TotalCount;
            }
            else

            {
                pVOPTiming->u16H_TotalCount = u32MVOPCLK / pstVideoStatus->u16FrameRate
                                               *1000 / pVOPTiming->u16V_TotalCount;
            }
        }
        else
        {
#if MVOP_SUPPORT_SUB
            pVOPTiming->u16H_TotalCount = u32MVOPCLK / pstVideoStatus->u16FrameRate
                                          *1000 / pVOPTiming->u16V_TotalCount;
#else
            MVOP_ERR(MVOP_PRINTF("Err: Not support SubMVOP!\n");)
#endif
        }
        pVOPTiming->u16H_TotalCount = (pVOPTiming->u16H_TotalCount + 0x1) & ~0x1;
        //MVOP HW Htt/Vtt limitation = 0x1fff
        if(pVOPTiming->u16H_TotalCount > 8190)
        {
            pVOPTiming->u16H_TotalCount = 8190;
            pVOPTiming->u16V_TotalCount = u32MVOPCLK / pstVideoStatus->u16FrameRate
                                          *1000 / pVOPTiming->u16H_TotalCount;
        }
        pVOPTiming->u16HActive_Start = pVOPTiming->u16H_TotalCount - u16HSizeTiming;
        //MVOP_PRINTF("=== Htt=%d Has=%d Hsize=%d\n", pVOPTiming->u16H_TotalCount, pVOPTiming->u16HActive_Start, u16HSize);
        //Aiken's protection for FBL ZOOM, Horizontal Blanking can not less than MVOP_HBlank_Min
        if((int)pVOPTiming->u16HActive_Start < MVOP_HBlank_Min) // avoid unsigned rounding, may be negative
        {
            if(pVOPTiming->u16H_TotalCount == 8190)
            {
                MVOP_PRINTF(" Error !! Horizontal Blanking can not less than %d \n", MVOP_HBlank_Min);
                return;
            }
            pVOPTiming->u16HActive_Start = MVOP_HBlank_Min;
            pVOPTiming->u16H_TotalCount =  pVOPTiming->u16HActive_Start + u16HSizeTiming;
            if (bIsMain)
            {
                pVOPTiming->u16V_TotalCount = u32MVOPCLK / pstVideoStatus->u16FrameRate * 1000 / pVOPTiming->u16H_TotalCount;
            }
            else
            {
#if MVOP_SUPPORT_SUB
                pVOPTiming->u16V_TotalCount = u32MVOPCLK / pstVideoStatus->u16FrameRate * 1000 / pVOPTiming->u16H_TotalCount;
#else
                MVOP_ERR(MVOP_PRINTF("Err: Not support SubMVOP!\n");)
#endif
            }
            if(pVOPTiming->u16V_TotalCount > 8191)
            {
                MVOP_ERR(MVOP_PRINTF("Err: Vtt overflow -- maybe resolution too large or framerate too low!!\n");)
            }
            iVBlank = pVOPTiming->u16V_TotalCount - u16VSize;
            // interlace video need ensure thant VB = VB0 + VB1
            // here, also need ensure vop's freqency should less than VOP_FREQ_144M/VOP_FREQ_160M
            if(bInterlace && ((iVBlank & 0x1) == 0))
            {
                pVOPTiming->u16V_TotalCount--;
                iVBlank--;
            }
            if(iVBlank < MVOP_VBlank_Min)
            {
                MVOP_ERR(MVOP_PRINTF(" Error !! Vertical Blanking can not less than %d \n", MVOP_VBlank_Min);)
                u16VSize = u16VSize - (MVOP_VBlank_Min - iVBlank);
                pVOPTiming->u16Height = u16VSize;
            }
        }
#ifdef _SUPPORT_IMG_OFFSET_
        pVOPTiming->u16HImg_Start = pVOPTiming->u16HActive_Start + pstVideoStatus->u16HorOffset;
        //MVOP_PRINTF("=== His=%d\n", pVOPTiming->u16HImg_Start);
#endif
        if(pVOPTiming->u16H_TotalCount >= 4095)
            MVOP_ERR(MVOP_PRINTF("[Warning] xc support pVOPTiming->u16H_TotalCount >= 4095 after CL 712830\n");)
        }

    if(pstVideoStatus->u8Interlace > E_MVOP_PRO)
        pVOPTiming->bInterlace = TRUE;
    else
        pVOPTiming->bInterlace = FALSE;

    if (bInterlace)
    {
        VB0 = (pVOPTiming->u16V_TotalCount - u16VSize) / 2 + 1; // VOP_VBlank/2+1 + EXTRA_V_BLANKING/2;
        VB1 = VB0 - 1;

#if ENABLE_3D_LR_MODE
        if (TRUE == bIsMain)
        {
            b3DTB = (HAL_MVOP_GetOutput3DType() == E_MVOP_OUTPUT_3D_TB);
        }
        else
        {
#if MVOP_SUPPORT_SUB
            if (HAL_MVOP_SubGetOutput3DType)
            {
                b3DTB = (HAL_MVOP_SubGetOutput3DType() == E_MVOP_OUTPUT_3D_TB);
            }
#endif
        }

        if (ENABLE == b3DTB)
        {
            VB1 = VB0;
            pVOPTiming->u16V_TotalCount++;
            MVOP_DBG(MVOP_PRINTF("%s 3DLR interlace: force VB0=VB1=0x%x, Vtt+1=0x%x\n", __FUNCTION__, VB1, pVOPTiming->u16V_TotalCount);)
        }
#endif

        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = pVOPTiming->u16VBlank0_End + (u16VSize/2);
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
    }
    else
    {
        VB1 = VB0 = (pVOPTiming->u16V_TotalCount - u16VSize);   //VB1 = VB0 for progressive
        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = 1;
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
    }
#ifdef _SUPPORT_IMG_OFFSET_
    if (bInterlace)
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset/2;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset/2;
    }
    else
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset;
    }
#endif
    pVOPTiming->u16TopField_VS = pVOPTiming->u16VBlank0_Start + u8VSyncOffset;
    pVOPTiming->u16BottomField_VS = pVOPTiming->u16VBlank1_Start + u8VSyncOffset;
    if (pVOPTiming->bInterlace)
    {
#if ENABLE_3D_LR_MODE
        //Do not adjust BottomField_VStart for 3D L/R interlace source
        if (ENABLE != b3DTB)
#endif
        {
            pVOPTiming->u16BottomField_VS -=1;
        }
    }

    pVOPTiming->u16TopField_Start = pVOPTiming->u16TopField_VS;
    pVOPTiming->u16BottomField_Start = pVOPTiming->u16BottomField_VS;

    // u8Framerate & u16H_Freq will be assigned to g_SrcInfo.*
    // in other place and cannot be removed
    pVOPTiming->u8Framerate = pstVideoStatus->u16FrameRate/1000;
    pVOPTiming->u16ExpFrameRate = pstVideoStatus->u16FrameRate;

    // u16H_Freq (unit: 100Hz)
    pVOPTiming->u16H_Freq = (MS_U16)
                            ((MS_U32)pVOPTiming->u16V_TotalCount *
                             (MS_U32)pstVideoStatus->u16FrameRate/100000);

#ifndef _ENABLE_DC1_FRAMESYNC
    // for dc1 ...
    pVOPTiming->u16Num = 1;
    pVOPTiming->u16Den = 1;
#else   //calculate clock for dc1 synthesizer
//calculate clock
    MS_U32 u32tmp_num, u32tmp_den;
    u32R = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount%1000;
    u32Q = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount/1000;
    u32R *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q += u32R/1000;
    MVOP_Clock = u32Q;
    u32MaxFactor = _MaxFactor(MVOP_Clock, DC_BaseClock*1000);

    if (u32MaxFactor == 1)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/2000);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/2);
    }
    else if (u32MaxFactor != 0)
    {
        u32tmp_num = (MS_U16)(MVOP_Clock/u32MaxFactor);
        u32tmp_den = (MS_U16)(DC_BaseClock*1000/u32MaxFactor);
        while(u32tmp_den > 65535)
        {
            u32tmp_den = u32tmp_den >> 1;
            u32tmp_num = u32tmp_num >> 1;

        }
        pVOPTiming->u16Num = (MS_U16)(u32tmp_num);
        pVOPTiming->u16Den = (MS_U16)(u32tmp_den);
    }
    else
    {
        pVOPTiming->u16Num = 0;
        pVOPTiming->u16Den = 0;
    }
#if 0
    //calculate clock
    u32R = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount%1000;
    u32Q = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount/1000;
    u32R *= pstVideoStatus->u16FrameRate;
    u32Q *= pstVideoStatus->u16FrameRate;
    u32Q += u32R/1000;
    u32Q /= 1000; // u32Q is the result
    MVOP_Clock = u32Q;
    u32MaxFactor = _MaxFactor(MVOP_Clock, DC_BaseClock);

    if (u32MaxFactor == 1)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/2);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/2);
    }
    else if (u32MaxFactor != 0)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/u32MaxFactor);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/u32MaxFactor);
    }
    else
    {
        pVOPTiming->u16Num = 0;
        pVOPTiming->u16Den = 0;
    }
#endif
#endif

    MVOP_TIMING_DBG(MVOP_PRINTF("\n>>>>> MVOP output 50/60\n");)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> w=%d, h=%d, i=%d, fr=%d w_timing=%d\n", pVOPTiming->u16Width,
                                pVOPTiming->u16Height, pVOPTiming->bInterlace, pVOPTiming->u16ExpFrameRate, u16HSizeTiming);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hTotal=%d, vTotal=%d\n",
                                pVOPTiming->u16H_TotalCount, pVOPTiming->u16V_TotalCount);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hActive=%d VB0=%d VB1=%d\n",
                                pVOPTiming->u16HActive_Start, VB0, VB1);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> ImgHSt=%d ImgVSt0=%d ImgVSt1=%d\n", pVOPTiming->u16HImg_Start, pVOPTiming->u16VImg_Start0, pVOPTiming->u16VImg_Start1);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hOffset=%d vOffset=%d\n", pstVideoStatus->u16HorOffset, pstVideoStatus->u16VerOffset);)
}

static MS_U32 MVOP_CalculatePixClk(MVOP_DevID eID,
                                   MVOP_Timing *pVOPTiming,
                                   MVOP_VidStat *pstVideoStatus,
                                   MS_BOOL bEnHDup,
                                   EN_MVOP_Output_3D_TYPE en3DType)
{
    MS_U32 u32PixClk = 0;
    MS_U32 u32R, u32Q;
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 V_Blanking = MVOP_VBlank+EXTRA_V_BLANKING;
    MS_U16 u16MinVBlank = 0;
#ifdef _SUPPORT_IMG_OFFSET_
    MS_U16 u16HMaxOffset = 0;
    MS_U16 u16VMaxOffset = 0;
#endif
    MS_U16* pu16FixVtt = NULL;
    MS_BOOL* pbEnMinVSize576 = NULL;
    MS_BOOL bEnVDup = FALSE;
    MS_BOOL bEnVx4Dup = FALSE;
    MS_BOOL bEnHx4Dup = FALSE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return 0;
        }
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return 0;
        }
        pstDrvCtrl = MVOP_GetSubDrvCtrl(eID);
    }
#endif
    if (pstDrvCtrl == NULL)
    {
        MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> NULL pstDrvCtrl!\n");)
        return 0;
    }

    //MS_BOOL* pbIsSetTiming = &pstDrvCtrl->_stMVOPDrvStat.bIsSetTiming;

    pu16FixVtt = &pstDrvCtrl->_u16FixVtt;
    pbEnMinVSize576 = &pstDrvCtrl->_bEnMinVSize576;
    bEnVDup = pstDrvCtrl->_bEnVSizeDup;
    bEnVx4Dup = pstDrvCtrl->_bEnVSizex4Dup;
    bEnHx4Dup = pstDrvCtrl->_bEnHSizex4Dup;

    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> MVOP in\n");)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>> HOffset=%d VOffset=%d fixVtt=%d EnMinVSize576=%x bEnVDup=%x bEnVx4Dup=%x bEnHx4Dup=%x\n",
                                pstVideoStatus->u16HorOffset, pstVideoStatus->u16VerOffset,
                                *pu16FixVtt, *pbEnMinVSize576, bEnVDup, bEnVx4Dup, bEnHx4Dup);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> w=%d, h=%d, i=%d, fr=%d\n",
                                pstVideoStatus->u16HorSize, pstVideoStatus->u16VerSize,
                                pstVideoStatus->u8Interlace, pstVideoStatus->u16FrameRate);)

    MS_ASSERT(pstVideoStatus->u16FrameRate != 0);
    if (pstVideoStatus->u16FrameRate == 0)
    {
        MVOP_PRINTF(">>>>> MVOP fr==0!\n");
        return 0;
    }

#if ENABLE_3D_LR_MODE
    if (E_MVOP_OUTPUT_3D_TB == en3DType)
    {
        pstVideoStatus->u16VerSize *= 2;
        MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> 3D L/R enable, multiple VSize=%d\n", pstVideoStatus->u16VerSize);)
    }
#if SUPPORT_3DLR_ALT_SBS
    else if (E_MVOP_OUTPUT_3D_SBS == en3DType)
    {
        pstVideoStatus->u16HorSize *= 2;
        MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> 3D L/R enable, Side By Side output, multiple HSize=%d\n", pstVideoStatus->u16HorSize);)
    }
#endif
#endif

#ifdef _SUPPORT_IMG_OFFSET_
    if ((pstVideoStatus->u16HorOffset%2) != 0)
    {
        //HorOffset needs to be even, or video color will be very strange.
        pstVideoStatus->u16HorOffset += 1;
        MVOP_TIMING_DBG(MVOP_PRINTF(">>>> Change HOffset as %d \n\n", pstVideoStatus->u16HorOffset);)
    }
    if(E_MVOP_DEV_0 == eID)
    {
#if defined(STB_DC_MODE)||(STB_DC)
        if((E_MVOP_OUTPUT_3D_NONE != en3DType) || (HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE)))
            u16HMaxOffset = MDrv_MVOP_GetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
        else
		    u16HMaxOffset = MDrv_MVOP_GetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,STB_OUTPUT_MAX_FPS);
#else
        u16HMaxOffset = MDrv_MVOP_GetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
#endif
    }
#if MVOP_SUPPORT_SUB
    else if (E_MVOP_DEV_1 == eID)
    {
#if defined(STB_DC_MODE)||(STB_DC)
        if((E_MVOP_OUTPUT_3D_NONE != en3DType) || (HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE)))
            u16HMaxOffset = MDrv_MVOP_SubGetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
        else
            u16HMaxOffset = MDrv_MVOP_SubGetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,STB_OUTPUT_MAX_FPS);
#else
        u16HMaxOffset = MDrv_MVOP_SubGetMaxHOffset(pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
#endif
    }
#endif
    HALMVOPFREQUENCY eFreq = HALMVOP_160MHZ;
    if (E_MVOP_DEV_0 == eID)
    {
        if(HALMVOP_NOT_SUPPORT != HAL_MVOP_Get4k2kClk())
        {
            eFreq = HAL_MVOP_Get4k2kClk();
        }
        else
        {
            eFreq = HAL_MVOP_GetMaxFreerunClk();
        }
    }
#if MVOP_SUPPORT_SUB
    else if (E_MVOP_DEV_1 == eID)
    {
        if(HALMVOP_NOT_SUPPORT != HAL_MVOP_SubGet4k2kClk())
        {
            eFreq = HAL_MVOP_SubGet4k2kClk();
        }
        else if (HAL_MVOP_SubGetMaxFreerunClk)
        {
            eFreq = HAL_MVOP_SubGetMaxFreerunClk();
        }
    }
#endif
#if MVOP_SUPPORT_3RD
    else if (E_MVOP_DEV_2 == eID)
    {
        if (HAL_MVOP_EX_GetMaxFreerunClk)
        {
            eFreq = HAL_MVOP_EX_GetMaxFreerunClk(eID);
        }
    }
#endif
    else
    {
        MVOP_PRINTF("%s invalid eID %x\n", __FUNCTION__, eID);
        return 0;
    }
    if(HAL_MVOP_SetEnable4k2k2P)
    {
#if defined(STB_DC_MODE)||(STB_DC)
        if((E_MVOP_OUTPUT_3D_NONE != en3DType) || (HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE)))
            u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, (pstVideoStatus->u16HorSize/2),pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
        else
            u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, (pstVideoStatus->u16HorSize/2),pstVideoStatus->u16VerSize,STB_OUTPUT_MAX_FPS);
#else
        u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, (pstVideoStatus->u16HorSize/2),pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
#endif
    }
    else
    {
#if defined(STB_DC_MODE)||(STB_DC)
        if((E_MVOP_OUTPUT_3D_NONE != en3DType) || (HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE)))
            u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
        else
            u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,STB_OUTPUT_MAX_FPS);
#else
        u16VMaxOffset = MVOP_GetMaxVOffset(eID, eFreq, pstVideoStatus->u16HorSize,pstVideoStatus->u16VerSize,pstVideoStatus->u16FrameRate);
#endif
    }
    if (pstVideoStatus->u16HorOffset > u16HMaxOffset)
    {
        MVOP_TIMING_DBG(MVOP_PRINTF("Change HorOffset %d --> %d\n", pstVideoStatus->u16HorOffset, u16HMaxOffset);)
        pstVideoStatus->u16HorOffset = u16HMaxOffset;
    }
    if (pstVideoStatus->u16VerOffset > u16VMaxOffset)
    {
        MVOP_TIMING_DBG(MVOP_PRINTF("Change u16VerOffset %d --> %d\n", pstVideoStatus->u16VerOffset, u16VMaxOffset);)
        pstVideoStatus->u16VerOffset = u16VMaxOffset;
    }
#endif

    MS_U16* pu16SrcHSize = NULL;
    MS_U16* pu16SrcVSize = NULL;

    if(eID == E_MVOP_DEV_0)
    {
        pu16SrcHSize = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        pu16SrcHSize = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
        pu16SrcVSize = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcVSize;
    }
#endif
    u16HSize = MVOP_CalculateHSize(pu16SrcHSize, pstVideoStatus->u16HorSize, pstVideoStatus->u16HorOffset);
    u16VSize = MVOP_CalculateVSize(eID, pu16SrcVSize, pstVideoStatus->u16VerSize, pstVideoStatus->u16VerOffset);

    if ((TRUE == *pbEnMinVSize576) && (u16VSize < 576) && (576 - u16VSize > V_Blanking))
    {
        V_Blanking = 576 - u16VSize;
    }

    //*pbIsSetTiming = TRUE;

    if (pstVideoStatus->u8Interlace >= E_MVOP_INT_TB_ONE_FRAME)
    {
        u16MinVBlank = MVOP_VBlank_Min_I;
    }
    else
    {
        u16MinVBlank = MVOP_VBlank_Min_P;
    }

    if((u16VSize >= *pu16FixVtt) || ((*pu16FixVtt - u16VSize) < u16MinVBlank))
    {
        // Restore to original flow
        *pu16FixVtt = 0;
        MVOP_DBG(MVOP_PRINTF("fix vtt feature is not enabled\n");)
    }

    if(bEnHDup && (u16HSize < 704))
    {
        u16HSize *= 2;
        pVOPTiming->bHDuplicate = 1;
    }
    else
    {
        pVOPTiming->bHDuplicate = 0;
    }

    if (bEnVDup)
    {
        u16VSize *= 2;
        MVOP_DBG(MVOP_PRINTF("VDup VSize*2=>%d\n", u16VSize);)
    }

    if (bEnVx4Dup)
    {
        u16VSize *= 4;
        MVOP_DBG(MVOP_PRINTF("VDup VSize*4=>%d\n", u16VSize);)
    }

    if (bEnHx4Dup)
    {
        u16HSize *= 4;
        MVOP_DBG(MVOP_PRINTF("VDup HSize*4=>%d\n", u16HSize);)
    }

    pVOPTiming->u16Width  = u16HSize;
    pVOPTiming->u16Height = u16VSize;

    if (u16HSize > 720) // HD
    {
        pVOPTiming->u16H_TotalCount = u16HSize + MVOP_HBlank_HD+EXTRA_H_BLANKING*2;
        pVOPTiming->u16HActive_Start = MVOP_HBlank_HD+EXTRA_H_BLANKING*2;
    }
    else    //SD
    {
        pVOPTiming->u16H_TotalCount = u16HSize + MVOP_HBlank_SD+EXTRA_H_BLANKING*2;
        pVOPTiming->u16HActive_Start = MVOP_HBlank_SD+EXTRA_H_BLANKING*2;
    }
    if(*pu16FixVtt > 0)
    {
        pVOPTiming->u16V_TotalCount = *pu16FixVtt;
    }
    else
    {
        pVOPTiming->u16V_TotalCount = u16VSize + V_Blanking;

#if (ENABLE_3D_LR_MODE && SUPPORT_3DLR_INST_VBLANK)
        MS_U16 u16InsVBlank = 0;
        if (E_MVOP_OUTPUT_3D_TB == en3DType)
        {
            if(eID == E_MVOP_DEV_0)
                u16InsVBlank = p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16InsVBlank;
#if MVOP_SUPPORT_SUB
            else if(eID == E_MVOP_DEV_1)
                u16InsVBlank = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16InsVBlank;
#endif
            if  (u16InsVBlank != 0)
            {
                pVOPTiming->u16V_TotalCount += u16InsVBlank;
                if(eID == E_MVOP_DEV_0)
                {
                    if (p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._fpHalSet3DLRInsVBlank)
                    {
                        p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._fpHalSet3DLRInsVBlank(u16InsVBlank);
                    }
                }
#if MVOP_SUPPORT_SUB
                else if(eID == E_MVOP_DEV_1)
                {
                    if (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._fpHalSet3DLRInsVBlank)
                    {
                        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._fpHalSet3DLRInsVBlank(u16InsVBlank);
                    }
                }
#endif
                MVOP_DBG(MVOP_PRINTF(">>>>> 3D L/R enable, Vtt => %d, InsVB=%d\n", pVOPTiming->u16V_TotalCount, u16InsVBlank);)
            }
        }
#endif

    }

    u32Q = (MS_U32)pVOPTiming->u16H_TotalCount * (MS_U32)pVOPTiming->u16V_TotalCount;
    u32R = u32Q % 1000;
    u32Q = u32Q / 1000;
#if defined(STB_DC_MODE)||(STB_DC)
    if((E_MVOP_OUTPUT_3D_NONE != en3DType) || (HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE)))
	{
		u32PixClk = u32Q * (MS_U32)pstVideoStatus->u16FrameRate +
                u32R * (MS_U32)pstVideoStatus->u16FrameRate / 1000;
	}
	else
	{
        u32PixClk = u32Q * (MS_U32)STB_OUTPUT_MAX_FPS +
                u32R * (MS_U32)STB_OUTPUT_MAX_FPS / 1000;
	}
#else
    u32PixClk = u32Q * (MS_U32)pstVideoStatus->u16FrameRate +
                u32R * (MS_U32)pstVideoStatus->u16FrameRate / 1000;
#endif
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> u32PixClk=%d, Htt=%d, Vtt=%d\n", u32PixClk,
                                pVOPTiming->u16H_TotalCount, pVOPTiming->u16V_TotalCount);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> u32PixClk=%d, Htt=%d, Vtt=%d\n", (int)u32PixClk,
                                pVOPTiming->u16H_TotalCount, pVOPTiming->u16V_TotalCount);)
#endif

    return u32PixClk;
}


static MS_BOOL MVOP_AutoGenMPEGTiming(MS_BOOL bIsMain,
                                      MVOP_Timing *pVOPTiming,
                                      MVOP_VidStat *pstVideoStatus,
                                      MS_BOOL bEnHDup)
{
    MS_U32 u32R, u32Q;
    MS_U32 u32MaxFactor;
    MS_U32 MVOP_Clock;
    MS_U16 bInterlace;
    MS_U16 VB0, VB1;
    MS_U16 u8VSyncOffset;
    MS_U16* pu16FixVtt = NULL;
    MS_BOOL bFreerunMode = FALSE;
    MVOP_DevID eID = E_MVOP_DEV_0;
    MS_BOOL bRefTSPCW = TRUE;
#if ENABLE_3D_LR_MODE
    MS_BOOL b3DTB = DISABLE;
#endif

    if (FALSE == bIsMain)
    {
#if MVOP_SUPPORT_SUB
        eID = E_MVOP_DEV_1;
#else
        MVOP_ERR(MVOP_PRINTF("Err no sub-mvop!\n");)
        return FALSE;
#endif
    }
    /*Check initialization*/
    if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
#endif
    //Just return if framerate is zero, because the pixel clock cannot be calculated.
    if (pstVideoStatus->u16FrameRate == 0)
    {
        MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> MVOP fr==0!\n");)
        return FALSE;
    }

    EN_MVOP_Output_3D_TYPE en3DType = E_MVOP_OUTPUT_3D_NONE;

#if ENABLE_3D_LR_MODE
    if (TRUE == bIsMain)
    {
        b3DTB = (HAL_MVOP_GetOutput3DType() == E_MVOP_OUTPUT_3D_TB);
    }
#if MVOP_SUPPORT_SUB
    else
    {
        if (HAL_MVOP_SubGetOutput3DType)
        {
            b3DTB = (HAL_MVOP_SubGetOutput3DType() == E_MVOP_OUTPUT_3D_TB);
        }
    }
#endif
    if (TRUE == bIsMain)
    {
        if(HAL_MVOP_GetOutput3DType && (HAL_MVOP_GetOutput3DType() <= E_MVOP_OUTPUT_3D_MAXNUM))
        {
            en3DType = HAL_MVOP_GetOutput3DType();
        }
    }
#if MVOP_SUPPORT_SUB
    else
    {
        if (HAL_MVOP_SubGetOutput3DType && (HAL_MVOP_SubGetOutput3DType() <= E_MVOP_OUTPUT_3D_MAXNUM))
        {
            en3DType = HAL_MVOP_SubGetOutput3DType();
        }
    }
#endif
#endif

    //Calculate the pixel clock
    MVOP_Clock = MVOP_CalculatePixClk(eID, pVOPTiming, pstVideoStatus, bEnHDup, en3DType);
#if defined(STB_DC_MODE)||(STB_DC)
    if(HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(eID) == FALSE)  && (MVOP_Clock > MAX_MVOP_FREQ))
    {
        MVOP_Clock = HAL_MVOP_SetFixClk(eID, MVOP_Clock);
        if((MVOP_Clock < HALMVOP_160MHZ) || (MVOP_Clock == HALMVOP_192MHZ))
        {
            if (bIsMain)
            {
                //default enable 2p mode: kano clippers
                if(HAL_MVOP_SetEnable4k2kClk)
                {
                    HAL_MVOP_SetEnable4k2kClk(FALSE);
                }
                HAL_MVOP_SetEnable60P(FALSE);
                MDrv_MVOP_EnableFreerunMode(FALSE);
            }
#if MVOP_SUPPORT_SUB
            else
            {
                if(HAL_MVOP_SubSetEnable4k2kClk)
                {
                    HAL_MVOP_SubSetEnable4k2kClk(FALSE);
                }
                HAL_MVOP_SubSetEnable60P(FALSE);
                MDrv_MVOP_SubEnableFreerunMode(FALSE);
            }
#endif
            //Need to re-calculate timing.
            MVOP_AutoGenTiming(bIsMain, pVOPTiming, pstVideoStatus, MVOP_Clock, TRUE);
#if defined (__aarch64__)
                MVOP_TIMING_DBG(MVOP_PRINTF("[MVOP][DBG] Set Fix clk = %d\n",MVOP_Clock);)
#else
                MVOP_TIMING_DBG(MVOP_PRINTF("[MVOP][DBG] Set Fix clk = %d\n",(int)MVOP_Clock);)
#endif
            return TRUE;
            }
    }
    else if(HAL_MVOP_SetDefaultClk && (MVOP_Clock <= MVOP_FREQ_160M))
    {
        if(HAL_MVOP_SetDefaultClk(eID) != HALMVOP_SYNCMODE)
        {
            MVOP_Clock = HAL_MVOP_SetDefaultClk(eID);
            MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> Set default clock");)
        }
    }
#endif
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> calculate need clk = %d\n",MVOP_Clock);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> calculate need clk = %d\n",(int)MVOP_Clock);)
#endif
    if (0 == MVOP_Clock)
    {
        MVOP_PRINTF(">>>>> MVOP_Clock==0!\n");
        return FALSE;
    }

#if defined(CHIP_MONET)|| defined(CHIP_MANHATTAN) || defined(CHIP_MASERATI) || defined(CHIP_MAXIM)
    //clear the register setting when the resolution change happens
    if (bIsMain)
    {
        if((HAL_MVOP_GetIsCurrentHSK) && (HAL_MVOP_GetIsCurrentHSK(E_MVOP_DEV_0) == FALSE))
        {
            if(HAL_MVOP_SetEnable4k2k2P)
            {
                HAL_MVOP_SetEnable4k2k2P(FALSE);
            }
            if(HAL_MVOP_SetEnable4k2kClk)
            {
                HAL_MVOP_SetEnable4k2kClk(FALSE);
            }
        }
    }
#if MVOP_SUPPORT_SUB
    else
    {
        if((HAL_MVOP_GetIsCurrentHSK) && (HAL_MVOP_GetIsCurrentHSK(E_MVOP_DEV_1) == FALSE))
        {
            if(HAL_MVOP_SubSetEnable4k2k2P)
            {
                HAL_MVOP_SubSetEnable4k2k2P(FALSE);
            }
            if(HAL_MVOP_SubSetEnable4k2kClk)
            {
                HAL_MVOP_SubSetEnable4k2kClk(FALSE);
            }
        }
    }
#endif
#endif

    //if pixel clock > synthesizer_max (86.4MHz), use the higher clock (160 or 144).
    if(MVOP_Clock >= MVOP_FREQ_320M)
    {
        if (bIsMain)
        {
            if(HAL_MVOP_SetEnable4k2k2P && HAL_MVOP_SetEnable4k2kClk)
            {
                HAL_MVOP_SetEnable4k2kClk(TRUE);
                HAL_MVOP_SetEnable4k2k2P(TRUE);
            }
            else
            {
#if defined (__aarch64__)
                MVOP_PRINTF("[Warning] MVOP_Clock == %d Not Support!\n",MVOP_Clock);
#else
                MVOP_PRINTF("[Warning] MVOP_Clock == %d Not Support!\n",(int)MVOP_Clock);
#endif
                return FALSE;
            }
            HAL_MVOP_SetEnable60P(FALSE);
            MDrv_MVOP_EnableFreerunMode(FALSE);
        }
#if MVOP_SUPPORT_SUB
        else
        {
            if(HAL_MVOP_SubSetEnable4k2k2P && HAL_MVOP_SubSetEnable4k2kClk)
            {
                HAL_MVOP_SubSetEnable4k2kClk(TRUE);
                HAL_MVOP_SubSetEnable4k2k2P(TRUE);
            }
            else
            {
                MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> MVOP Not Support 4k2k 30fps!\n");)
                return FALSE;
            }
            HAL_MVOP_SubSetEnable60P(FALSE);
            MDrv_MVOP_SubEnableFreerunMode(FALSE);
        }
#endif
        //Need to re-calculate timing.
        MVOP_AutoGenTiming(bIsMain, pVOPTiming, pstVideoStatus, MVOP_FREQ_320M, TRUE);
        return TRUE;
    }
    else if(MVOP_Clock > MVOP_FREQ_160M)
    {
        MVOP_DBG(MVOP_PRINTF("drvMVOP: Enable clk 320MHz!\n");)
        if (bIsMain)
        {
            if(HAL_MVOP_SetEnable4k2kClk)
            {
                HAL_MVOP_SetEnable4k2kClk(TRUE);
            }
            else
            {
                MVOP_PRINTF(">>>>> MVOP Not Support 320MHz(4k2k 30fps)!\n");
                return FALSE;
            }
            HAL_MVOP_SetEnable60P(FALSE);
            MDrv_MVOP_EnableFreerunMode(FALSE);
        }
#if MVOP_SUPPORT_SUB
        else
        {
            if(HAL_MVOP_SubSetEnable4k2kClk)
            {
                HAL_MVOP_SubSetEnable4k2kClk(TRUE);
            }
            else
            {
                MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> MVOP Not Support 4k2k 30fps!\n");)
                return FALSE;
            }
            HAL_MVOP_SubSetEnable60P(FALSE);
            MDrv_MVOP_SubEnableFreerunMode(FALSE);
        }
#endif
        //Need to re-calculate timing.
        MVOP_AutoGenTiming(bIsMain, pVOPTiming, pstVideoStatus, MVOP_FREQ_320M, FALSE);
        return TRUE;
    }
    else if (MVOP_Clock > MAX_MVOP_FREQ && MVOP_Clock <= MVOP_FREQ_160M)
    {
        MVOP_DBG(MVOP_PRINTF("drvMVOP: Enable clk 160MHz!\n");)
        if (bIsMain)
        {
            if(HAL_MVOP_SetEnable4k2kClk)
            {
                HAL_MVOP_SetEnable4k2kClk(FALSE);
            }
            HAL_MVOP_SetEnable60P(TRUE);
            MDrv_MVOP_EnableFreerunMode(FALSE);
        }
#if MVOP_SUPPORT_SUB
        else
        {
            if(HAL_MVOP_SubSetEnable4k2kClk)
            {
                HAL_MVOP_SubSetEnable4k2kClk(FALSE);
            }
            HAL_MVOP_SubSetEnable60P(TRUE);
            MDrv_MVOP_SubEnableFreerunMode(FALSE);
        }
#endif
        //Need to re-calculate timing.
        MVOP_AutoGenTiming(bIsMain, pVOPTiming, pstVideoStatus, MVOP_FREQ_160M, FALSE);
        return TRUE;
    }
    else
    {
        MVOP_DBG(MVOP_PRINTF("drvMVOP: Enable clk Synchronous!\n");)
        if (bIsMain)
        {
            if(HAL_MVOP_SetEnable4k2kClk)
            {
                HAL_MVOP_SetEnable4k2kClk(FALSE);
            }
            HAL_MVOP_SetEnable60P(FALSE);
        }
#if MVOP_SUPPORT_SUB
        else
        {
            if(HAL_MVOP_SubSetEnable4k2kClk)
            {
                HAL_MVOP_SubSetEnable4k2kClk(FALSE);
            }
            HAL_MVOP_SubSetEnable60P(FALSE);
        }
#endif

        /* If stc cw = 0x0 (TSP has not been initialized), mvop will use freerun clock.*/
        if (bIsMain)
        {
            if(HAL_MVOP_CheckSTCCW)
            {
                if(FALSE == HAL_MVOP_CheckSTCCW())
                {
                    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> STC cw = 0x0, MVOP enable freerun clock.\n");)
                    HAL_MVOP_EnableFreerunMode(TRUE);
                    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode = TRUE;
                    bRefTSPCW = FALSE;
                }
            }
        }
#if MVOP_SUPPORT_SUB
        else
        {
            if(HAL_MVOP_SubCheckSTCCW)
            {
                if(FALSE == HAL_MVOP_SubCheckSTCCW())
                {
                    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> STC cw = 0x0, MVOP enable freerun clock.\n");)
                    HAL_MVOP_SubEnableFreerunMode(TRUE);
                    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnableFreerunMode = TRUE;
                    bRefTSPCW = FALSE;
                }
            }
        }
#endif

        //reset fixVtt
        if(eID == E_MVOP_DEV_0)
            pu16FixVtt = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#if MVOP_SUPPORT_SUB
        if(eID == E_MVOP_DEV_1)
            pu16FixVtt = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt;
#endif
        if(*pu16FixVtt > 0)
        {
            *pu16FixVtt = 0;
        }
    }

#if MVOP_SUPPORT_SUB
    if(eID == E_MVOP_DEV_0)
        bFreerunMode = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode;
    else if(eID == E_MVOP_DEV_1)
        bFreerunMode = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnableFreerunMode;
#if 0
    bFreerunMode = ((TRUE == bIsMain) &&p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode) ||
                   ((FALSE == bIsMain) && p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnableFreerunMode);
#endif
#else
    bFreerunMode = ((TRUE == bIsMain) && p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode);
#endif
    if (((!bFreerunMode) && (MVOP_Clock < MIN_MVOP_FREQ))
        || (bFreerunMode && (MVOP_Clock < MIN_MVOP_FREERUN_FREQ))
       )
    {
        //finetune Htt & HActiveStart to enlarge pixel clock if pixel clock < min_clk
        MS_U32 u32MinMvopFreq = 0;
        u32MaxFactor = (MS_U32)(pstVideoStatus->u16FrameRate / 1000)*
                       (MS_U32)pVOPTiming->u16V_TotalCount;

        if (u32MaxFactor == 0)
        {
            MVOP_PRINTF("%s(%d) u32MaxFactor==0!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        if(bFreerunMode)
        {
            u32MinMvopFreq = MIN_MVOP_FREERUN_FREQ;
        }
        else
        {
            u32MinMvopFreq = MIN_MVOP_FREQ;
        }

        u32Q = u32MinMvopFreq / u32MaxFactor;
        u32R = u32MinMvopFreq % u32MaxFactor;
        pVOPTiming->u16H_TotalCount = (MS_U16)(u32Q);
        if (u32R != 0)
            pVOPTiming->u16H_TotalCount++;

        pVOPTiming->u16H_TotalCount = (pVOPTiming->u16H_TotalCount + 0x1) & ~0x1;

        //finetune Vtt if Htt is too large
        //xc limitation htt = 4095, but limitation discard after napoli.
#if 0
        if(pVOPTiming->u16H_TotalCount > 4094)
        {
            pVOPTiming->u16H_TotalCount = 4094;
            u32MaxFactor = (MS_U32)(pstVideoStatus->u16FrameRate / 1000)*
                           (MS_U32)pVOPTiming->u16H_TotalCount;
            u32Q = u32MinMvopFreq / u32MaxFactor;
            u32R = u32MinMvopFreq % u32MaxFactor;
            pVOPTiming->u16V_TotalCount = (MS_U16)(u32Q);
            if (u32R != 0)
                pVOPTiming->u16V_TotalCount++;

        }
#endif
        //MVOP HW Htt/Vtt limitation = 0x1fff
        if(pVOPTiming->u16H_TotalCount > 8190)
        {
            pVOPTiming->u16H_TotalCount = 8190;
            u32MaxFactor = (MS_U32)(pstVideoStatus->u16FrameRate / 1000)*
                           (MS_U32)pVOPTiming->u16H_TotalCount;
            u32Q = u32MinMvopFreq / u32MaxFactor;
            u32R = u32MinMvopFreq % u32MaxFactor;
            pVOPTiming->u16V_TotalCount = (MS_U16)(u32Q);
            if (u32R != 0)
                pVOPTiming->u16V_TotalCount++;

            if(pVOPTiming->u16V_TotalCount > 8191)
                MVOP_ERR(MVOP_PRINTF("Err: Vtt overflow -- maybe resolution too large or framerate too low!!\n");)
            }
        pVOPTiming->u16HActive_Start = pVOPTiming->u16H_TotalCount - pVOPTiming->u16Width;
    }
    // There are potential risks as setting freerun mode & synchronous clock
    MS_ASSERT(!(bFreerunMode && bRefTSPCW));

    //determine other timing setting
#ifdef _SUPPORT_IMG_OFFSET_
    pVOPTiming->u16HImg_Start = pVOPTiming->u16HActive_Start + pstVideoStatus->u16HorOffset;
#endif

    if(pstVideoStatus->u8Interlace > E_MVOP_PRO)
        bInterlace = TRUE;
    else
        bInterlace = FALSE;

    MS_U16 u16VSize = pVOPTiming->u16Height;
    if (bInterlace)
    {
        // interlace video need ensure thant VB = VB0 + VB1
        if(((pVOPTiming->u16V_TotalCount - u16VSize) & 0x1) == 0)
        {
            pVOPTiming->u16V_TotalCount++;
        }
        VB0 = (pVOPTiming->u16V_TotalCount - u16VSize) / 2 + 1;
        VB1 = VB0 - 1;
#if ENABLE_3D_LR_MODE
        if (ENABLE == b3DTB)
        {
            VB1 = VB0;
            pVOPTiming->u16V_TotalCount++;
            MVOP_DBG(MVOP_PRINTF("%s 3DLR interlace: force VB0=VB1=0x%x, Vtt+1=0x%x\n", __FUNCTION__, VB1, pVOPTiming->u16V_TotalCount);)
        }
#endif
        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = pVOPTiming->u16VBlank0_End + (u16VSize/2);
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
        pVOPTiming->bInterlace = TRUE;
    }
    else
    {
        VB1 = VB0 = (pVOPTiming->u16V_TotalCount - u16VSize);
        pVOPTiming->u16VBlank0_Start = 1;
        pVOPTiming->u16VBlank0_End   = pVOPTiming->u16VBlank0_Start + VB0;
        pVOPTiming->u16VBlank1_Start = 1;
        pVOPTiming->u16VBlank1_End   = pVOPTiming->u16VBlank1_Start + VB1;
        pVOPTiming->bInterlace = FALSE;
    }

#ifdef _SUPPORT_IMG_OFFSET_
    if(bInterlace)
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset / 2;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset / 2;
    }
    else
    {
        pVOPTiming->u16VImg_Start0 = pVOPTiming->u16VBlank0_End + pstVideoStatus->u16VerOffset;
        pVOPTiming->u16VImg_Start1 = pVOPTiming->u16VBlank1_End + pstVideoStatus->u16VerOffset;
    }
#endif

    u8VSyncOffset = VB0/2;
    pVOPTiming->u16TopField_VS = pVOPTiming->u16VBlank0_Start + u8VSyncOffset;
    pVOPTiming->u16BottomField_VS = pVOPTiming->u16VBlank1_Start + u8VSyncOffset;
    if (pVOPTiming->bInterlace)
    {
#if ENABLE_3D_LR_MODE
        if (ENABLE != b3DTB)
#endif
        {
            pVOPTiming->u16BottomField_VS -=1;
        }
    }

    if((p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eInputSel == MVOP_INPUT_DRAM)  && bInterlace)
    {
        pVOPTiming->u16TopField_Start = pVOPTiming->u16TopField_VS + 16; //mantis:1147303; for vsync and de being close, avoid xc to sample framerate incorrectly.
        pVOPTiming->u16BottomField_Start = pVOPTiming->u16BottomField_VS +16;
    }
    else
    {
        pVOPTiming->u16TopField_Start = pVOPTiming->u16TopField_VS;
        pVOPTiming->u16BottomField_Start = pVOPTiming->u16BottomField_VS;
    }

    // u8Framerate & u16H_Freq will be assigned to g_SrcInfo.*
    // in other place and cannot be removed
    pVOPTiming->u8Framerate = pstVideoStatus->u16FrameRate/1000;
    pVOPTiming->u16ExpFrameRate = pstVideoStatus->u16FrameRate;

    // u16H_Freq (unit: 100Hz)
    pVOPTiming->u16H_Freq = (MS_U16)
                            ((MS_U32)pVOPTiming->u16V_TotalCount *
                             (MS_U32)pstVideoStatus->u16FrameRate/100000);

    //calculate clock
    MS_U32 u32tmp_num, u32tmp_den;
    u32R = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount%1000;
    u32Q = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount/1000;
    u32R *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q += u32R/1000;
    MVOP_Clock = u32Q;
    u32MaxFactor = _MaxFactor(MVOP_Clock, DC_BaseClock*1000);
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>u32MaxFactor = %d\n",u32MaxFactor);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>u32MaxFactor = %d\n",(int)u32MaxFactor);)
#endif
    if (u32MaxFactor == 1)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/2000);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/2);
    }
    else if (u32MaxFactor != 0)
    {
        u32tmp_num = MVOP_Clock/u32MaxFactor;
        u32tmp_den = DC_BaseClock*1000/u32MaxFactor;
        while(u32tmp_den > 65535)
        {
            u32tmp_den = u32tmp_den >> 1;
            u32tmp_num = u32tmp_num >> 1;

        }
        pVOPTiming->u16Num = (MS_U16)(u32tmp_num);
        pVOPTiming->u16Den = (MS_U16)(u32tmp_den);
    }
    else
    {
        pVOPTiming->u16Num = 0;
        pVOPTiming->u16Den = 0;
    }

#if 0
    //calculate clock
    u32R = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount%1000;
    u32Q = (MS_U32)pVOPTiming->u16H_TotalCount*(MS_U32)pVOPTiming->u16V_TotalCount/1000;
    u32R *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q *= pstVideoStatus->u16FrameRate; // frameratex1000
    u32Q += u32R/1000;
    u32Q /= 1000; // u32Q is the result
    MVOP_Clock = u32Q;
    u32MaxFactor = _MaxFactor(MVOP_Clock, DC_BaseClock);

    if (u32MaxFactor == 1)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/2);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/2);
    }
    else if (u32MaxFactor != 0)
    {
        pVOPTiming->u16Num = (MS_U16)(MVOP_Clock/u32MaxFactor);
        pVOPTiming->u16Den = (MS_U16)(DC_BaseClock/u32MaxFactor);
    }
    else
    {
        pVOPTiming->u16Num = 0;
        pVOPTiming->u16Den = 0;
    }
#endif
    MVOP_TIMING_DBG(MVOP_PRINTF("\n>>>>> MVOP output\n");)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> w=%d, h=%d, i=%d, fr=%d\n", pVOPTiming->u16Width, pVOPTiming->u16Height,
                                pVOPTiming->bInterlace, pVOPTiming->u16ExpFrameRate);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>>>> hTotal=%d, vTotal=%d\n", pVOPTiming->u16H_TotalCount, pVOPTiming->u16V_TotalCount);)
    MVOP_TIMING_DBG(MVOP_PRINTF(">>> ImgHSt=%d ImgVSt0=%d ImgVSt1=%d\n", pVOPTiming->u16HImg_Start, pVOPTiming->u16VImg_Start0, pVOPTiming->u16VImg_Start1);)
    return TRUE;
}

HALMVOPMIUSEL _MVOP_MIUtypeSwitchMVOPEnum(eMIU_SelType eMiuSel)
{
    HALMVOPMIUSEL eRet = E_MVOP_SEL_NONE;

    switch(eMiuSel)
    {
        case MIU_SELTYPE_MIU0:
            eRet = E_MVOP_SEL_MIU0;
            break;
        case MIU_SELTYPE_MIU1:
            eRet = E_MVOP_SEL_MIU1;
            break;
        case MIU_SELTYPE_MIU2:
            eRet = E_MVOP_SEL_MIU2;
            break;
        case MIU_SELTYPE_MIU3:
            eRet = E_MVOP_SEL_MIU3;
            break;
        case MIU_SELTYPE_MIU_ALL:
            eRet = E_MVOP_SEL_NONE;
            break;
        default:
            break;
    }

    return eRet;

}

//Notice: This function is not supported for T2.
static MVOP_Result MVOP_MiuSwitch(MS_BOOL bIsMain, MS_U8 u8Miu)
{
    eMIUClientID eCid = MVOP_MIU_CLIENT_MAIN;
    eMIU_SelType eMiuSel = MIU_SELTYPE_MIU1;
    MVOP_DevID  eDevID = E_MVOP_DEV_0;

    if (!bIsMain)
    {
        eCid = MVOP_MIU_CLIENT_SUB;
        eDevID = E_MVOP_DEV_1;
    }

    if (u8Miu == 0)
    {
        eMiuSel = MIU_SELTYPE_MIU0;
    }
    else if (u8Miu == 1)
    {
        //wait for miu kernel mode
#if 1
        if (!MDrv_MIU_IsSupportMIU1())
        {
            return E_MVOP_INVALID_PARAM;
        }
#endif
        eMiuSel = MIU_SELTYPE_MIU1;
    }
    else if (u8Miu == 2)
    {
        eMiuSel = MIU_SELTYPE_MIU2;
    }
    else if (u8Miu == 3)
    {
        eMiuSel = MIU_SELTYPE_MIU3;
    }
    else
    {
        return E_MVOP_INVALID_PARAM;
    }
    MVOP_DBG(MVOP_PRINTF("%s eCid = %d, eMiuSel = %d\n",__FUNCTION__,eCid,eMiuSel);)

    /* MIU1 Bank MIU_SEL = 1(Group 1 ID 3: 0x100679[3] = 1)
    // MIU Switch to IP Control    */
    if(HAL_MVOP_GetIsMiuIPControl)
    {
        MIU_ClientInfo stMiuClientInfo;
        HALMVOPMIUCLIENTINFO stMvopMiuClientInfo;
        HALMVOPMIUSEL eMvopMiuSel = E_MVOP_SEL_MIU1;
        MS_BOOL bIsIPCtrl = 0;

        //wait for miu kernel mode
        MDrv_MIU_GetClientInfo(eMiuSel, eCid, &stMiuClientInfo);
        stMvopMiuClientInfo.u8BitPos = stMiuClientInfo.u8BitPos;
        stMvopMiuClientInfo.u8Gp = stMiuClientInfo.u8Gp;
        MVOP_DBG(MVOP_PRINTF("%s MIU group = %d, bit = %d\n",__FUNCTION__,stMvopMiuClientInfo.u8Gp,stMvopMiuClientInfo.u8BitPos);)

        eMvopMiuSel = _MVOP_MIUtypeSwitchMVOPEnum(eMiuSel);

        if(HAL_MVOP_GetIsOnlyMiuIPControl)
        {
            bIsIPCtrl = (HAL_MVOP_GetIsOnlyMiuIPControl() || HAL_MVOP_GetIsMiuIPControl(stMvopMiuClientInfo));
        }
        else
        {
            bIsIPCtrl = HAL_MVOP_GetIsMiuIPControl(stMvopMiuClientInfo);
        }

        if(bIsIPCtrl)
        {
            //wait for miu kernel mode
            MDrv_MIU_SelMIU(eCid, MIU_SELTYPE_MIU_ALL); // enable for only miu IP control
            MVOP_DBG(MVOP_PRINTF("MVOP_MiuSwitch IP control\n");)
            HAL_MVOP_SelMIU(eDevID, eMvopMiuSel, eMvopMiuSel, eMvopMiuSel, eMvopMiuSel);
            return E_MVOP_OK;
        }
    }

    MVOP_DBG(MVOP_PRINTF("MVOP_MiuSwitch eCid=0x%x, eMiuSel=0x%x\n", eCid, eMiuSel);)
    //wait for miu kernel mode
    MDrv_MIU_SelMIU(eCid, eMiuSel);

    return E_MVOP_OK;
}

static MS_BOOL MVOP_GetStatus(MVOP_DevID eID, MVOP_DrvStatus *pMVOPStat)
{
    MVOP_DrvStatus *pDrvStat = NULL;

    if(eID >= HAL_MVOP_MODULE_CNT)
    {
        MVOP_DBG(MVOP_PRINTF("%s MVOP Device ID is out of MVOP module number\n", __FUNCTION__);)
        return FALSE;
    }
    else if(eID == E_MVOP_DEV_0)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return FALSE;
        }
    }
#endif

    if(eID == E_MVOP_DEV_0)
        pDrvStat = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat;
#if MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
        pDrvStat = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat;
#endif

    if ((pMVOPStat ==  NULL) || (pDrvStat ==  NULL))
    {
        return FALSE;
    }

    pMVOPStat->bIsInit = pDrvStat->bIsInit;
    pMVOPStat->bIsEnable = pDrvStat->bIsEnable;
    pMVOPStat->bIsUVShift = pDrvStat->bIsUVShift;
    pMVOPStat->bIsBlackBG = pDrvStat->bIsBlackBG;
    pMVOPStat->bIsMonoMode = pDrvStat->bIsMonoMode;
    return TRUE;
}

void MVOP_ResetVar(MVOP_DevID eID)
{
    MVOP_DrvCtrlSt* pstDrvCtrl;
    MS_U16 u16ECOVersion;

    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)

    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();

    switch(eID)
    {
        case E_MVOP_DEV_0:
            if (p_gDrvMVOPCtx == NULL)
            {
                MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
                return;
            }
            HAL_MVOP_SetInputMode( VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            pstDrvCtrl = MVOP_GetDrvCtrl(E_MVOP_DEV_0);
            MDrv_MVOP_EnableFreerunMode(FALSE);
            break;
#if MVOP_SUPPORT_SUB
        case E_MVOP_DEV_1:
            if (p_gDrvSubMVOPCtx == NULL)
            {
                MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
                return;
            }
            HAL_MVOP_SubSetInputMode( VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            pstDrvCtrl = MVOP_GetSubDrvCtrl(E_MVOP_DEV_1);
            MDrv_MVOP_SubEnableFreerunMode(FALSE);
            break;
#endif
        default:
            MVOP_DBG(MVOP_PRINTF("%s MVOP Device ID is out of MVOP module number\n", __FUNCTION__);)
            return;
            break;
    }
    pstDrvCtrl->_bEnableFreerunMode = FALSE;
    pstDrvCtrl->_bEnMinVSize576 = TRUE;
    pstDrvCtrl->_bEnVSizeDup = FALSE;
    pstDrvCtrl->_bEnVSizex4Dup = FALSE;
    pstDrvCtrl->_bEnHSizex4Dup = FALSE;
    pstDrvCtrl->_eRgbFmt = E_MVOP_RGB_NONE;
    pstDrvCtrl->_u16FixVtt = 0;
    pstDrvCtrl->_u16SrcHSize = 0;
    pstDrvCtrl->_u16SrcVSize = 0;
    memset(&pstDrvCtrl->_mvopTiming, 0, sizeof(MVOP_Timing));
    pstDrvCtrl->_bEnHDS = FALSE;
    pstDrvCtrl->_bEn265DV = FALSE;
    pstDrvCtrl->_bGOPPatch = TRUE;
    HAL_MVOP_ResetReg(eID, u16ECOVersion);

}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// This function shows the version of MVOP driver.
///
/// @param *ppVersion : pointer to the driver version
////////////////////////////////////////////////////////////////////////////////
MVOP_Result MDrv_MVOP_GetLibVer_V2(void* pInstance, const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_MVOP_FAIL;

    *ppVersion = &_drv_mvop_version;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_GetLibVer(const MSIF_Version **ppVersion)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (!ppVersion)
        return E_MVOP_FAIL;

    *ppVersion = &_drv_mvop_version;
    return E_MVOP_OK;
}

/********************************************************************************/
/// Set detailed level of MVOP driver debug message (level)
/// This function is used for MVOP driver debug message setting
/// @param level  \b IN  level from 0 to 1
/********************************************************************************/
void MDrv_MVOP_SetDbgLevel_V2(void* pInstance, MS_U8 level)
{
    _u8DebugMsgLevel = level;
    return;
}

void MDrv_MVOP_SetDbgLevel(MS_U8 level)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_DBG_LEVEL IO_arg;
    IO_arg.level = level;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_DBG_LEVEL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return ;
    }
    return;
#else
    void* pInstance = NULL;
    MDrv_MVOP_SetDbgLevel_V2(pInstance,level));
#endif

}

//-----------------------------------------------------------------------------
/// Get information of MVOP driver.
/// @return driver information
//-----------------------------------------------------------------------------
const MVOP_DrvInfo* MDrv_MVOP_GetInfo_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    p_gDrvMVOPCtx->_stMVOPDrvInfo.u32MaxSynClk = MAX_MVOP_FREQ;
    p_gDrvMVOPCtx->_stMVOPDrvInfo.u32MinSynClk = MIN_MVOP_FREQ;
    p_gDrvMVOPCtx->_stMVOPDrvInfo.u32MaxFreerunClk = (MS_U32)(HAL_MVOP_GetMaxFreerunClk());

    _DRV_MVOP_Return(E_MVOP_DEV_0, &p_gDrvMVOPCtx->_stMVOPDrvInfo);
}

const MVOP_DrvInfo* MDrv_MVOP_GetInfo(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_INFO IO_arg;
    memset(&IO_arg,0x00,sizeof(stMVOP_GET_INFO)); //CID 51854


    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_INFO, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return IO_arg.pRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetInfo_V2(pInstance));
#endif
}

//-----------------------------------------------------------------------------
/// Get status of MVOP driver
/// @param -pstatus: driver status
/// @return - TRUE / FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_GetStatus_V2(void* pInstance, MVOP_DrvStatus *pMVOPStat)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    _DRV_MVOP_Return(E_MVOP_DEV_0,MVOP_GetStatus(E_MVOP_DEV_0, pMVOPStat));
}

MS_BOOL MDrv_MVOP_GetStatus(MVOP_DrvStatus *pMVOPStat)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_STATUS IO_arg;
    IO_arg.pMVOPStat = pMVOPStat;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_GET_STATUS, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }
    return IO_arg.bRet;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetStatus_V2(pInstance, pMVOPStat));
#endif
}

/********************************************************************************/
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return HStart
/********************************************************************************/
MS_U16 MDrv_MVOP_GetHStart_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, 0);
    }
#if defined(STB_DC_MODE)||(STB_DC)
        if(HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE))
        {
            MVOP_DBG(MVOP_PRINTF("%s get h satrt mvop gentiming\n", __FUNCTION__);)
            _DRV_MVOP_Return(E_MVOP_DEV_0, (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16HActive_Start)/2 + 2);
        }
        else
        {
            MVOP_DBG(MVOP_PRINTF("%s get h satrt xc gentiming\n", __FUNCTION__);)
            _DRV_MVOP_Return(E_MVOP_DEV_0, 0);
        }
#else
    _DRV_MVOP_Return(E_MVOP_DEV_0, (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16HActive_Start)/2 + 2);
#endif
}

MS_U16 MDrv_MVOP_GetHStart(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_HSTART IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_HSTART, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }
    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetHStart_V2(pInstance));
#endif
}

/********************************************************************************/
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return VStart
/********************************************************************************/
MS_U16 MDrv_MVOP_GetVStart_V2(void* pInstance)
{
    MS_U16 u16Vst = 0;

    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, u16Vst);
    }
#if defined(STB_DC_MODE)||(STB_DC)
    if(HAL_MVOP_GetIsCurrentXCGenTiming && (HAL_MVOP_GetIsCurrentXCGenTiming(E_MVOP_DEV_0) == FALSE))
    {
        u16Vst = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16VBlank0_End -  p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16TopField_VS;
        if (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.bInterlace==1)
        {
            u16Vst*=2;
            u16Vst-=18;
        }
        else
        {
            u16Vst-=5;
        }

        if (TRUE == MVOP_NeedAdjustVSize(E_MVOP_DEV_0))
        {
            if (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize != 0)
            {
                MVOP_DBG(MVOP_PRINTF("_u16SrcVSize = %d, report (VSt Plus N)  %d ",
                                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize, u16Vst);)
                MS_U16 u16VSize = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height;
                u16Vst += (ALIGN_UPTO_4(u16VSize) - u16VSize);
                MVOP_DBG(MVOP_PRINTF("==> %d \n", u16Vst);)
            }
        }

        _DRV_MVOP_Return(E_MVOP_DEV_0, u16Vst);
        }
        else
        {
            _DRV_MVOP_Return(E_MVOP_DEV_0, u16Vst);
        }
#else
    u16Vst = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16VBlank0_End -  p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16TopField_VS;
    if (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.bInterlace==1)
    {
        u16Vst*=2;
        u16Vst-=18;
    }
    else
    {
        u16Vst-=5;
    }

    if (TRUE == MVOP_NeedAdjustVSize(E_MVOP_DEV_0))
    {
        if (p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize != 0)
        {
            MVOP_DBG(MVOP_PRINTF("_u16SrcVSize = %d, report (VSt Plus N)  %d ",
                                 p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize, u16Vst);)
            MS_U16 u16VSize = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height;
            u16Vst += (ALIGN_UPTO_4(u16VSize) - u16VSize);
            MVOP_DBG(MVOP_PRINTF("==> %d \n", u16Vst);)
        }
    }

    _DRV_MVOP_Return(E_MVOP_DEV_0, u16Vst);
#endif
}

MS_U16 MDrv_MVOP_GetVStart(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_VSTART IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_VSTART, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }
    return IO_arg.u16Ret;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetVStart_V2(pInstance));
#endif
}

/******************************************************************************/
// Set MVOP clock enable
// @param bEnable \b IN
//   - # TRUE  Enable clock
//   - # FALSE Close clock
/******************************************************************************/
void MDrv_MVOP_SetClk_V2 (void* pInstance, MS_BOOL bEnable )
{
    HAL_MVOP_SetDCClk(0, bEnable);  //Clk DC0
    HAL_MVOP_SetDCClk(1, bEnable);  //Clk DC1
    if(HAL_MVOP_SetDCSRAMClk)
    {
        HAL_MVOP_SetDCSRAMClk(0, bEnable);  //Clk DC0 sraam clock
    }
#ifdef UFO_MVOP_FB_DEC_CLK
    if(HAL_MVOP_SetFBDecClk)
    {
        HAL_MVOP_SetFBDecClk(E_MVOP_DEV_0, bEnable); // clk FBDec
    }
#endif
}

static void MDrv_MVOP_SetClk ( MS_BOOL bEnable )
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    HAL_MVOP_SetDCClk(0, bEnable);  //Clk DC0
    HAL_MVOP_SetDCClk(1, bEnable);  //Clk DC1
    if(HAL_MVOP_SetDCSRAMClk)
    {
        HAL_MVOP_SetDCSRAMClk(0, bEnable);  //Clk DC0 sraam clock
    }
#ifdef UFO_MVOP_FB_DEC_CLK
    if(HAL_MVOP_SetFBDecClk)
    {
        HAL_MVOP_SetFBDecClk(E_MVOP_DEV_0, bEnable); // clk FBDec
    }
#endif
}

//-----------------------------------------------------------------------------
/// Set MVOP test pattern.
//-----------------------------------------------------------------------------
void MDrv_MVOP_SetPattern_V2(void* pInstance, MVOP_Pattern enMVOPPattern)
{
    static MVOP_Pattern s_enMVOPPattern = MVOP_PATTERN_NORMAL;

    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    if(enMVOPPattern == MVOP_PATTERN_DEFAULT)
    {
        enMVOPPattern = (MVOP_Pattern)(((MS_U32)(s_enMVOPPattern) + 1) % (MS_U32)MVOP_PATTERN_DEFAULT);
    }
    HAL_MVOP_SetPattern(enMVOPPattern);
    s_enMVOPPattern = enMVOPPattern;

    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_SetPattern(MVOP_Pattern enMVOPPattern)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_PATTERN IO_arg;
    IO_arg.enMVOPPattern = enMVOPPattern;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_PATTERN, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetPattern_V2(pInstance, enMVOPPattern));
#endif
}

//-----------------------------------------------------------------------------
/// Configure the tile format of the MVOP input.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SetTileFormat_V2(void* pInstance, MVOP_TileFormat eTileFmt)
{
    MS_BOOL bRet = FALSE;
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
    bRet = HAL_MVOP_SetTileFormat(eTileFmt);

    _DRV_MVOP_Return(E_MVOP_DEV_0, bRet);
    //return HAL_MVOP_SetTileFormat(eTileFmt);
}

MS_BOOL MDrv_MVOP_SetTileFormat(MVOP_TileFormat eTileFmt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_TILE_FORMAT IO_arg;
    IO_arg.eTileFmt = eTileFmt;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SET_TILE_FORMAT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }
    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetTileFormat_V2(pInstance, eTileFmt));
#endif
}


//-----------------------------------------------------------------------------
/// Set MMIO Base for MVOP.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SetMMIOMapBase_V2(void* pInstance)
{
    return MVOP_SetRegBaseAdd(HAL_MVOP_RegSetBase);
}

MS_BOOL MDrv_MVOP_SetMMIOMapBase(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    return MVOP_SetRegBaseAdd(HAL_MVOP_RegSetBase);
}

/******************************************************************************/
/// Initialize MVOP hardware and set it to hardwire mode
/******************************************************************************/
void MDrv_MVOP_Init_V2(void* pInstance)
{

    MVOP_DrvInitCtxResults eRet = E_MVOP_INIT_FAIL;
    MS_BOOL pbFirstDrvInstant;
    MS_U16 u16ECOVersion;

    _DRV_MVOP_MutexCreate(E_MVOP_DEV_0);

    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    eRet = _MVOP_Init_Context(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("[%s] Context Init FAILED!\n",__FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
        if(MDrv_MVOP_SetMMIOMapBase() == FALSE)
        {
            _DRV_MVOP_Return(E_MVOP_DEV_0, );
        }
        HAL_MVOP_Init();
        //1. enable clock
        MDrv_MVOP_SetClk(TRUE);
        //2. enable sram power
#ifdef CONFIG_MSTAR_SRAMPD
        if(HAL_MVOP_SetSramPower)
        {
            HAL_MVOP_SetSramPower(E_MVOP_DEV_0, TRUE);
        }
#endif
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit = TRUE;
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    _MVOP_InitVar(E_MVOP_DEV_0);
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();

    if(MDrv_MVOP_SetMMIOMapBase() == FALSE)
    {
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    HAL_MVOP_Init();
    if(HAL_MVOP_SetECONumber)
    {
        HAL_MVOP_SetECONumber(u16ECOVersion);
    }
    //HAL_MVOP_SetInputMode( VOPINPUT_HARDWIRE, NULL, u16ECOVersion);

    // Set fld inv & ofld_inv
    HAL_MVOP_SetFieldInverse(ENABLE, ENABLE);

    // Set Croma weighting off
    HAL_MVOP_SetChromaWeighting(ENABLE);
#if 0
#if ENABLE_3D_LR_MODE
    HAL_MVOP_Enable3DLR(DISABLE);
#endif
#if SUPPORT_3DLR_ALT_SBS
    HAL_MVOP_Set3DLRAltOutput(DISABLE);
    HAL_MVOP_Set3DLRAltSBSOutput(DISABLE);
#endif
    MDrv_MVOP_EnableFreerunMode(FALSE);
#endif
    //1. enable clock
    MDrv_MVOP_SetClk(TRUE);
    //2. enable sram power
#ifdef CONFIG_MSTAR_SRAMPD
    if(HAL_MVOP_SetSramPower)
    {
        HAL_MVOP_SetSramPower(E_MVOP_DEV_0, TRUE);
    }
#endif
    HAL_MVOP_LoadReg();

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit = TRUE;
    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_Init(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        if(UtopiaOpen(MODULE_MVOP|MVOP_DRIVER_BASE, &pu32MVOP_Inst[E_MVOP_MAIN_STREAM], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MVOP_PRINTF("[Fail] UtopiaOpen MVOP failed\n");
            return;
        }
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_INIT, (void*)(IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail] %s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_Init_V2(pInstance);
#endif
}

//------------------------------------------------------------------------------
///Map context for multi process use
//------------------------------------------------------------------------------
void MDrv_MVOP_GetConfig(MVOP_Handle* stHd)
{
    switch(stHd->eModuleNum)
    {
        case E_MVOP_MODULE_MAIN:
        {
            MDrv_MVOP_Init();
            break;
        }
        case E_MVOP_MODULE_SUB:
        {
#if MVOP_SUPPORT_SUB
            MDrv_MVOP_SubInit();
            break;
#endif
        }
        default:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s not support MVOP NUM\n", __FUNCTION__);)
            break;
        }
    }
}


/******************************************************************************/
/// Exit MVOP, turn off clock
/******************************************************************************/
void MDrv_MVOP_Exit_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    //1. disable mvop
    HAL_MVOP_SetMIUReqMask(TRUE);
    HAL_MVOP_Enable(FALSE, 25);
    HAL_MVOP_SetMIUReqMask(FALSE);
    if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEn265DV)
    {
#if MVOP_SUPPORT_SUB
        HAL_MVOP_SubSetMIUReqMask(TRUE);
        HAL_MVOP_SubEnable(FALSE, 25);
        HAL_MVOP_SubSetMIUReqMask(FALSE);
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual = TRUE;
#endif

    }
    //2.disable mvop sram
#ifdef CONFIG_MSTAR_SRAMPD
    if(HAL_MVOP_SetSramPower)
    {
        HAL_MVOP_SetSramPower(E_MVOP_DEV_0, FALSE);
    }
#endif
    //3.disable mvop clock
    MDrv_MVOP_SetClk(FALSE);
    if(HAL_MVOP_Exit)
    {
        HAL_MVOP_Exit(E_MVOP_DEV_0);
    }
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit = FALSE;

    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_Exit()
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_EXIT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_Exit_V2(pInstance);
#endif
}
/******************************************************************************/
/// Enable and Disable MVOP
/// @param bEnable \b IN
///   - # TRUE  Enable
///   - # FALSE Disable and reset
/******************************************************************************/
void MDrv_MVOP_Enable_V2 (void* pInstance, MS_BOOL bEnable )
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    MS_U8 u8FPS = 25; //Default fps = 25, set as normal case;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u8Framerate != 0)
    {
        u8FPS = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u8Framerate;
    }

    HAL_MVOP_SetMIUReqMask(TRUE);
    HAL_MVOP_Enable(bEnable, u8FPS);
    HAL_MVOP_SetMIUReqMask(FALSE);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsEnable = bEnable;
    if(FALSE == bEnable)
    {
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsSetTiming = bEnable;
    }
    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_Enable ( MS_BOOL bEnable )
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d arg:%d\n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_ENABLE IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_ENABLE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    MDrv_MVOP_Enable_V2(pInstance, bEnable);
#endif
}

/******************************************************************************/
/// Get if MVOP is enable or not.
/// @param pbEnable \b OUT
///   - # TRUE  Enable
///   - # FALSE Disable
/******************************************************************************/
MVOP_Result MDrv_MVOP_GetIsEnable_V2 (void* pInstance, MS_BOOL* pbEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    if ((NULL == pbEnable) || (TRUE != MVOP_IsInit()))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    *pbEnable = HAL_MVOP_GetEnableState();
    _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_OK);
}

MVOP_Result MDrv_MVOP_GetIsEnable (MS_BOOL* pbEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_IS_ENABLE IO_arg;
    IO_arg.pbEnable = pbEnable;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_IS_ENABLE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetIsEnable_V2(pInstance, pbEnable));
#endif
}
/******************************************************************************/
/// Set enable UVShift
/******************************************************************************/
void MDrv_MVOP_EnableUVShift_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_EnableUVShift(bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsUVShift = bEnable;

    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_EnableUVShift(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_ENABLE_UV_SHIFT IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_ENABLE_UV_SHIFT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_EnableUVShift_V2(pInstance, bEnable);
#endif
}

/******************************************************************************/
/// Set enable black background
/******************************************************************************/
void MDrv_MVOP_EnableBlackBG_V2 ( void* pInstance )
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_SetBlackBG();

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsBlackBG = TRUE;

    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_EnableBlackBG ( void )
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_ENABLE_BLACK_BG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_EnableBlackBG_V2(pInstance);
#endif
}


/******************************************************************************/
/// Set MVOP output timing
/// @param ptiming \b IN VOP timing setting
/******************************************************************************/
static void MVOP_DumpOutputTiming ( MVOP_Timing *ptiming )
{
    if (!ptiming)
    {
        return;
    }

    MVOP_DBG(MVOP_PRINTF("\nMVOP SetOutputTiming\n");)
    MVOP_DBG(MVOP_PRINTF(" VTot=%u,\t",ptiming->u16V_TotalCount);)
    MVOP_DBG(MVOP_PRINTF(" HTot=%u,\t",ptiming->u16H_TotalCount);)
    MVOP_DBG(MVOP_PRINTF(" I/P=%u\n",ptiming->bInterlace);)
    MVOP_DBG(MVOP_PRINTF(" W=%u,\t",ptiming->u16Width);)
    MVOP_DBG(MVOP_PRINTF(" H=%u,\t",ptiming->u16Height);)
    MVOP_DBG(MVOP_PRINTF(" FRate=%u,\t",ptiming->u8Framerate);)
    MVOP_DBG(MVOP_PRINTF(" HFreq=%u\n",ptiming->u16H_Freq);)
    MVOP_DBG(MVOP_PRINTF(" Num=0x%x,\t",ptiming->u16Num);)
    MVOP_DBG(MVOP_PRINTF(" Den=0x%x,\t",ptiming->u16Den);)
    MVOP_DBG(MVOP_PRINTF(" u16HImg_Start=%u\n",ptiming->u16HImg_Start);)
    MVOP_DBG(MVOP_PRINTF(" u16VImg_Start0=%u\n",ptiming->u16VImg_Start0);)
    MVOP_DBG(MVOP_PRINTF(" u16VImg_Start1=%u\n",ptiming->u16VImg_Start1);)
    MVOP_DBG(MVOP_PRINTF(" u16ExpFRate=%u #\n\n", ptiming->u16ExpFrameRate);)
}


/******************************************************************************/
/// MVOP mono mode (fix chroma value)
/******************************************************************************/
void MDrv_MVOP_SetMonoMode_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_SetMonoMode(bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsMonoMode = bEnable;

    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_SetMonoMode(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d\n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_MONO_MODE IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_MONO_MODE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SetMonoMode_V2(pInstance, bEnable);
#endif
}


//-----------------------------------------------------------------------------
/// Configure MVOP input.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SetInputCfg_V2 (void* pInstance, MVOP_InputSel in, MVOP_InputCfg* pCfg)
{
    MVOP_Result bRet = E_MVOP_OK;
    MS_U16 u16ECOVersion;

    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }
    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(E_MVOP_DEV_0);

    if (!MVOP_IsInit())
    {
        MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    //HAL_MVOP_SetMonoMode(FALSE);
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();
    if (E_MVOP_RGB_NONE != pstDrvCtrl->_eRgbFmt)
    {
        if (HAL_MVOP_SetRgbFormat)
        {
            HAL_MVOP_SetRgbFormat(pstDrvCtrl->_eRgbFmt);
        }
        else
        {
            MVOP_ERR(MVOP_PRINTF("%s not support HAL_MVOP_SetRgbFormat\n", __FUNCTION__);)
        }
    }
    MVOP_ResetVar(E_MVOP_DEV_0);

#if STB_DC
    _DRV_MVOP_Return(E_MVOP_DEV_0, HAL_MVOP_SetInputCfg( in,  pCfg));
#endif

    switch (in)
    {
#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_DRAM_3DLR:
            HAL_MVOP_Enable3DLR(ENABLE);
            if (!pCfg)
            {
                _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_INVALID_PARAM);
            }
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg = *pCfg;

            HAL_MVOP_SetInputMode(VOPINPUT_MCUCTRL, pCfg, u16ECOVersion);
            break;
#endif
        case MVOP_INPUT_DRAM:
            if (!pCfg)
            {
                _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_INVALID_PARAM);
            }
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg = *pCfg;

            HAL_MVOP_SetInputMode(VOPINPUT_MCUCTRL, pCfg, u16ECOVersion);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_HVD_3DLR:
            HAL_MVOP_Enable3DLR(ENABLE);
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetH264HardwireMode(u16ECOVersion);
            break;
#endif
        case MVOP_INPUT_H264:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetH264HardwireMode(u16ECOVersion);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_MVD_3DLR:
            HAL_MVOP_Enable3DLR(ENABLE);
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            break;
#endif
        case MVOP_INPUT_MVD:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            break;
        case MVOP_INPUT_RVD:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetRMHardwireMode(u16ECOVersion);
            break;
        case MVOP_INPUT_CLIP:
            if (!pCfg)
            {
                _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_INVALID_PARAM);
            }
            HAL_MVOP_SetCropWindow(pCfg);
            break;
        case MVOP_INPUT_JPD:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetJpegHardwireMode();
            break;
#if SUPPORT_EVD_MODE
#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_EVD_3DLR:
            HAL_MVOP_Enable3DLR(ENABLE);
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetEVDHardwireMode(u16ECOVersion);
            break;
#endif
        case MVOP_INPUT_EVD:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetEVDHardwireMode(u16ECOVersion);
            break;
#endif
#if SUPPORT_VP9_MODE
        case MVOP_INPUT_VP9:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetVP9HardwireMode(u16ECOVersion);
            break;
#endif
#ifdef UFO_MVOP_DOLBY_HDR
        case MVOP_INPUT_EVD_DV:
            HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SetEVDHardwireMode_DV();
            if (HAL_MVOP_EnableHDRSetting)
            {
                HAL_MVOP_EnableHDRSetting(ENABLE);
            }
            if(TRUE == p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual)
            {
                MS_BOOL pbFirstDrvInstant;

                _MVOP_SubInit_Context(&pbFirstDrvInstant);
                _MVOP_SubInitVar(E_MVOP_DEV_1);
                if(MDrv_MVOP_SubSetMMIOMapBase() == FALSE)
                {
                    bRet = E_MVOP_INVALID_PARAM;
                    break;
                }
                HAL_MVOP_SubInit();
                HAL_MVOP_SubSetFieldInverse(ENABLE, ENABLE);
                HAL_MVOP_SubSetDCClk(0, TRUE);  //Clk DC0
                HAL_MVOP_SubSetDCClk(1, TRUE);  //Clk DC1
                if(HAL_MVOP_SubSetDCSRAMClk)
                {
                    HAL_MVOP_SubSetDCSRAMClk(0, TRUE);  //Clk DC1 sraam clock
                }
                HAL_MVOP_SubLoadReg();
                p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit = TRUE;
                MVOP_ResetVar(E_MVOP_DEV_1);
                HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
                HAL_MVOP_SubSetEVDHardwireMode(u16ECOVersion);

                if (HAL_MVOP_SubEnableHDRSetting)
                {
                    HAL_MVOP_SubEnableHDRSetting(ENABLE);
                }
            }
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEn265DV = TRUE;
            break;
#endif
        default:
            bRet = E_MVOP_INVALID_PARAM;
            break;
    }

    if (E_MVOP_OK == bRet)
    {
        pstDrvCtrl->_eInputSel = in;
    }

    _DRV_MVOP_Return(E_MVOP_DEV_0, bRet);
}

MVOP_Result MDrv_MVOP_SetInputCfg (MVOP_InputSel in, MVOP_InputCfg* pCfg)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d\n", __FUNCTION__, __LINE__, in);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_INPUTCFG IO_arg;
    IO_arg.in = in;
    IO_arg.pCfg = pCfg;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_INPUTCFG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetInputCfg_V2(pInstance, in, pCfg));
#endif
}

static MS_BOOL MVOP_CheckOutputCfg(MVOP_VidStat *pstVideoStatus)
{
    if (pstVideoStatus == NULL)
    {
        MVOP_ERR(MVOP_PRINTF("%s invalid para NULL Ptr!\n", __FUNCTION__);)
        return FALSE;
    }

    //Designer suggests to set the minimum HSize as 16.
    //If supporting HSize<16, miu_request_timeout must be enabled.
    if ((pstVideoStatus->u16HorSize < 16) ||
        (pstVideoStatus->u16VerSize == 0) ||
        (pstVideoStatus->u16FrameRate < MVOP_MIN_FRAMERATE))
    {
        MVOP_ERR(MVOP_PRINTF("%s invalid para H=%d V=%d FR=%d\n", __FUNCTION__,
                             pstVideoStatus->u16HorSize, pstVideoStatus->u16VerSize, pstVideoStatus->u16FrameRate);)
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//-----------------------------------------------------------------------------
/// Configure MVOP output.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SetOutputCfg_V2(void* pInstance, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    MVOP_Timing* pstTiming = NULL;
    MS_U16 u16ECOVersion;

    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopOutputCfg = *pstVideoStatus;

    if(HAL_MVOP_AutoTimingHDS && (HAL_MVOP_AutoTimingHDS(E_MVOP_DEV_0) == TRUE))
    {
        if(pstVideoStatus->u16HorSize > 2048)
        {
            pstVideoStatus->u16HorSize >>= 1;
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnHDS = TRUE;
        }
    }

    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();

    if (!MVOP_IsInit())
    {
        MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    if (MVOP_CheckOutputCfg(pstVideoStatus) != TRUE)
    {
        MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_INVALID_PARAM);
    }

    MVOP_HBlank_HD = 300;
    MVOP_HBlank_SD = 200;
    MVOP_VBlank  = 45;
    EnablePresetTable = FALSE;

    if(pstVideoStatus->u16VerSize == VSize_4K)
    {
        if((pstVideoStatus->u16FrameRate == FrameRate_60000) || (pstVideoStatus->u16FrameRate == FrameRate_30000))
        {
            EnablePresetTable = TRUE;
            MVOP_TIMING_DBG(MVOP_PRINTF("%s: refine 4k 59940 timing\n", __FUNCTION__);)
            MVOP_VBlank = Optimized_VBlank_4K_60000;
        }
        else if((pstVideoStatus->u16FrameRate == FrameRate_59940) || (pstVideoStatus->u16FrameRate == FrameRate_29970))
        {
            EnablePresetTable = TRUE;
            MVOP_TIMING_DBG(MVOP_PRINTF("%s: refine 4k 59940/29970 timing\n", __FUNCTION__);)
            MVOP_VBlank = Optimized_VBlank_4K_59940;
        }
        else if((pstVideoStatus->u16FrameRate == FrameRate_50000) || (pstVideoStatus->u16FrameRate == FrameRate_25000))
        {
            EnablePresetTable = TRUE;
            MVOP_TIMING_DBG(MVOP_PRINTF("%s: refine 4k 50000/25000 timing\n", __FUNCTION__);)
            MVOP_VBlank = Optimized_VBlank_4K_50000;
        }
        else if((pstVideoStatus->u16FrameRate == FrameRate_48000) || (pstVideoStatus->u16FrameRate == FrameRate_24000))
        {
            EnablePresetTable = TRUE;
            MVOP_TIMING_DBG(MVOP_PRINTF("%s: refine 4k 24000/48000 timing\n", __FUNCTION__);)
            MVOP_VBlank = Optimized_VBlank_4K_48000;
        }

        else if(pstVideoStatus->u16FrameRate == FrameRate_23976)
        {
            EnablePresetTable = TRUE;
            MVOP_TIMING_DBG(MVOP_PRINTF("%s: refine 4k 23976 timing\n", __FUNCTION__);)
            MVOP_VBlank = Optimized_VBlank_4K_23976;
        }

    }

    if (MVOP_AutoGenMPEGTiming(TRUE, pstTiming, pstVideoStatus, bEnHDup) != TRUE)
    {
        MVOP_PRINTF("%s: fail to calculate timing!\n", __FUNCTION__);
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }
#ifdef UFO_MVOP_DOLBY_HDR
    HAL_MVOP_Enable(FALSE, 0); // for HAL_MVOP_Enable2DCTimingSync
#endif
    if(HAL_MVOP_SetInterlaceType)
    {
        HAL_MVOP_SetInterlaceType(E_MVOP_DEV_0, u16ECOVersion, pstVideoStatus->u8Interlace);
    }

    HAL_MVOP_SetOutputInterlace(pstTiming->bInterlace, u16ECOVersion);
    HAL_MVOP_SetOutputTiming(pstTiming);
    HAL_MVOP_SetSynClk(pstTiming);
#ifdef UFO_MVOP_DOLBY_HDR
    if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEn265DV && (TRUE == p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual))
    {
        MVOP_DBG(MVOP_PRINTF("%s dual dolby vision\n", __FUNCTION__);)
        HAL_MVOP_SubEnable(FALSE, 0);
        HAL_MVOP_SetEnable4k2k2P(TRUE);
        HAL_MVOP_SubSetEnable4k2k2P(TRUE);
        HAL_MVOP_SubSetOutputInterlace(pstTiming->bInterlace, u16ECOVersion);
        HAL_MVOP_SubSetOutputTiming(pstTiming);
        HAL_MVOP_SubSetSynClk(pstTiming);
        if(HAL_MVOP_Enable2DCTimingSync)
        {
            HAL_MVOP_Enable2DCTimingSync(TRUE);
        }
    }
    else if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEn265DV && (FALSE == p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual))
    {
        MVOP_DBG(MVOP_PRINTF("%s single dolby vision\n", __FUNCTION__);)
        HAL_MVOP_SetEnable4k2k2P(TRUE);
        if(HAL_MVOP_Enable2DCTimingSync)
        {
            HAL_MVOP_Enable2DCTimingSync(FALSE);
        }
    }
#endif
    //For VMirror, use Align4VSize to gen timing, but still report SrvVSize to XC.
    if ( p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize != 0)
    {
        MVOP_DBG(MVOP_PRINTF("%s: _mvopTiming.u16Height:  %d ==> %d\n", __FUNCTION__,
                             pstTiming->u16Height, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize);)
        pstTiming->u16Height = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcVSize;
    }

    if ( p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcHSize != 0)
    {
        //report the source width for scaler, intead of the 16-aligned width
        //MS_U16 u16Discard_Width = 0;
        //u16Discard_Width = _mvopTiming.u16Width - _u16SrcHSize;
        MVOP_DBG(MVOP_PRINTF("%s: _mvopTiming.u16Width :  %d ==> %d\n", __FUNCTION__,
                             pstTiming->u16Width, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcHSize);)
        pstTiming->u16Width = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16SrcHSize;
#if 0   //for H-mirror ON
        if (pstTiming->u16HActive_Start > u16Discard_Width)
        {
            pstTiming->u16HActive_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HActive_Start(%d) <= u16Discard_Width(%d)\n",
                        pstTiming->u16HActive_Start, u16Discard_Width);
        }
        if (pstTiming->u16HImg_Start > u16Discard_Width)
        {
            pstTiming->u16HImg_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HImg_Start(%d) <= u16Discard_Width(%d)\n",
                        pstTiming->u16HImg_Start, u16Discard_Width);
        }
#endif
    }

    MVOP_DumpOutputTiming(pstTiming);
    MS_BOOL* pbIsSetTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsSetTiming;
    *pbIsSetTiming = TRUE;
    _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_OK);
}

MVOP_Result MDrv_MVOP_SetOutputCfg(MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d\n", __FUNCTION__, __LINE__, bEnHDup);)

#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_OTPUTCFG IO_arg;
    IO_arg.pstVideoStatus = pstVideoStatus;
    IO_arg.bEnHDup = bEnHDup;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_OTPUTCFG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetOutputCfg_V2(pInstance, pstVideoStatus, bEnHDup));
#endif
}

//-----------------------------------------------------------------------------
/// Get MVOP output timing information.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_GetOutputTiming_V2(void* pInstance, MVOP_Timing* pMVOPTiming)
{
    MVOP_Timing* pstTiming = NULL;

    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }
    if (!p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsSetTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: Timing is not set yet!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    if (!pMVOPTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_INVALID_PARAM);
    }
    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;

#if !(defined(CHIP_A3) || defined(CHIP_EDISON))
    memcpy(pMVOPTiming, pstTiming, sizeof(MVOP_Timing));
#else
    pMVOPTiming->u16V_TotalCount      =  pstTiming->u16V_TotalCount;
    pMVOPTiming->u16H_TotalCount      =  pstTiming->u16H_TotalCount;
    pMVOPTiming->u16VBlank0_Start     =  pstTiming->u16VBlank0_Start;
    pMVOPTiming->u16VBlank0_End       =  pstTiming->u16VBlank0_End;
    pMVOPTiming->u16VBlank1_Start     =  pstTiming->u16VBlank1_Start;
    pMVOPTiming->u16VBlank1_End       =  pstTiming->u16VBlank1_End;
    pMVOPTiming->u16TopField_Start    =  pstTiming->u16TopField_Start;
    pMVOPTiming->u16BottomField_Start =  pstTiming->u16BottomField_Start;
    pMVOPTiming->u16TopField_VS       =  pstTiming->u16TopField_VS;
    pMVOPTiming->u16BottomField_VS    =  pstTiming->u16BottomField_VS;
    pMVOPTiming->u16HActive_Start     =  pstTiming->u16HActive_Start;

    pMVOPTiming->bInterlace           =  pstTiming->bInterlace;
    pMVOPTiming->u8Framerate          =  pstTiming->u8Framerate;
    pMVOPTiming->u16H_Freq            =  pstTiming->u16H_Freq;
    pMVOPTiming->u16Num               =  pstTiming->u16Num;
    pMVOPTiming->u16Den               =  pstTiming->u16Den;
    pMVOPTiming->u8MvdFRCType         =  pstTiming->u8MvdFRCType;
    pMVOPTiming->u16ExpFrameRate      =  pstTiming->u16ExpFrameRate;
    pMVOPTiming->u16Width             =  pstTiming->u16Width;
    pMVOPTiming->u16Height            =  pstTiming->u16Height;
    pMVOPTiming->bHDuplicate          =  pstTiming->bHDuplicate;
#endif

#if defined(CHIP_KANO)
    //patch for GOP: 0x28[7]=1 need to toggle mcu mode, kano HAL_MVOP_Rst doing mcu mode toggling.
    if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bGOPPatch == TRUE)
    {
        HAL_MVOP_Rst();
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bGOPPatch = FALSE;
    }
#endif
    _DRV_MVOP_Return(E_MVOP_DEV_0,E_MVOP_OK);
}

MVOP_Result MDrv_MVOP_GetOutputTiming(MVOP_Timing* pMVOPTiming)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d\n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_OUTPUT_TIMING IO_arg;
    IO_arg.pMVOPTiming = pMVOPTiming;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_GET_OUTPUT_TIMING, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetOutputTiming_V2(pInstance, pMVOPTiming));
#endif
}


//-----------------------------------------------------------------------------
/// Get Horizontal Size.
/// @return Horizontal Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_GetHSize_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, 0);
    }
    _DRV_MVOP_Return(E_MVOP_DEV_0,p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
}

MS_U16 MDrv_MVOP_GetHSize(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d\n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_HSIZE IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_GET_HSIZE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetHSize_V2(pInstance));
#endif
}


//-----------------------------------------------------------------------------
/// Get Vertical Size.
/// @return Vertical Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_GetVSize_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, 0);
    }
    _DRV_MVOP_Return(E_MVOP_DEV_0,p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
}

MS_U16 MDrv_MVOP_GetVSize(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d\n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_VSIZE IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_VSIZE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetVSize_V2(pInstance));
#endif
}


//-----------------------------------------------------------------------------
/// Get MVOP timing is interlace or progressive.
/// @return TRUE for interlace; FALSE for progressive
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_GetIsInterlace_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    _DRV_MVOP_Return(E_MVOP_DEV_0,p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.bInterlace);
}

MS_BOOL MDrv_MVOP_GetIsInterlace(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d\n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_IS_INTERLACE IO_arg;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_IS_INTERLACE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetIsInterlace_V2(pInstance));
#endif
}


//-----------------------------------------------------------------------------
/// Get MVOP timing is horizontal duplicated or not.
/// @return TRUE for yes; FALSE for not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_GetIsHDuplicate_V2(void* pInstance)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    _DRV_MVOP_Return(E_MVOP_DEV_0,p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.bHDuplicate);
}

MS_BOOL MDrv_MVOP_GetIsHDuplicate(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d\n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_IS_HDUPLICATE IO_arg;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_IS_HDUPLICATE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetIsHDuplicate_V2(pInstance));
#endif
}

/******************************************************************************************/
/// This function is used for dectection of MVOP's Capability
/// @param u16HSize    \b IN
/// @param u16VSize    \b IN
/// @param u16Fps      \b IN
/// @return TRUE if MVOP's frequency can support this video source, otherwise return FASLE
/******************************************************************************************/
MS_BOOL MDrv_MVOP_CheckCapability_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    return ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_GetMaxFreerunClk()));
}

MS_BOOL MDrv_MVOP_CheckCapability(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d arg1:%d, arg2:%d, arg3:%d\n", __FUNCTION__, __LINE__, u16HSize, u16VSize, u16Fps);)
    return ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_GetMaxFreerunClk()));
}

MS_U16 MDrv_MVOP_GetMaxHOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_U16 u16HttMax, u16OffsetMax = 0;

    u16HttMax = HAL_MVOP_GetMaxFreerunClk() / u16Fps*1000 / (u16VSize + MVOP_VBlank);
    u16OffsetMax = (u16HttMax - MVOP_HBlank_HD - u16HSize)/2;
    u16OffsetMax = u16OffsetMax & ~1;

    return u16OffsetMax;
}

MS_U16 MDrv_MVOP_GetMaxHOffset(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d arg1:%d, arg2:%d, arg3:%d\n", __FUNCTION__, __LINE__, u16HSize, u16VSize, u16Fps);)
    MS_U16 u16HttMax, u16OffsetMax = 0;

    u16HttMax = HAL_MVOP_GetMaxFreerunClk() / u16Fps*1000 / (u16VSize + MVOP_VBlank);
    u16OffsetMax = (u16HttMax - MVOP_HBlank_HD - u16HSize)/2;
    u16OffsetMax = u16OffsetMax & ~1;

    return u16OffsetMax;
}


MS_U16 MDrv_MVOP_GetMaxVOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    _DRV_MVOP_Return(E_MVOP_DEV_0, MVOP_GetMaxVOffset(E_MVOP_DEV_0, HAL_MVOP_GetMaxFreerunClk(), u16HSize, u16VSize, u16Fps));
}

MS_U16 MDrv_MVOP_GetMaxVOffset(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d arg1:%d, arg2:%d, arg3:%d\n", __FUNCTION__, __LINE__, u16HSize, u16VSize, u16Fps);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_MAX_VOFFSET IO_arg;
    IO_arg.u16HSize = u16HSize;
    IO_arg.u16VSize = u16VSize;
    IO_arg.u16Fps = u16Fps;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_MAX_VOFFSET, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetMaxVOffset_V2(pInstance, u16HSize, u16VSize, u16Fps));
#endif
}

//------------------------------------------------------------------------------
/// MVOP Get Destination Information for GOP mixer
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_GetDstInfo_V2(void* pInstance, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    if(NULL == pDstInfo)
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():pDstInfo is NULL\n");)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    if(u32SizeofDstInfo != sizeof(MVOP_DST_DispInfo))
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():u16SizeofDstInfo is different from the MVOP_DST_DispInfo defined, check header file!\n");)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    MVOP_TimingInfo_FromRegisters mvopTimingInfo;
    memset(&mvopTimingInfo, 0, sizeof(MVOP_TimingInfo_FromRegisters));
    if(HAL_MVOP_GetTimingInfoFromRegisters(&mvopTimingInfo) == FALSE)
    {
        MVOP_ERR(MVOP_PRINTF("encounter errors in HAL_MVOP_GetTimingInfoFromRegisters()!\n");)
        memset(pDstInfo, 0, u32SizeofDstInfo);
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
    pDstInfo->VDTOT = mvopTimingInfo.u16V_TotalCount;
    pDstInfo->HDTOT = mvopTimingInfo.u16H_TotalCount;
    pDstInfo->bInterlaceMode = mvopTimingInfo.bInterlace;
    pDstInfo->DEHST = mvopTimingInfo.u16HActive_Start / 2 + 2;
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) * 2 - 18;
    }
    else
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) - 5;
    }
    //HDeEnd=HDeStart+HDeWidth
    pDstInfo->DEHEND = pDstInfo->DEHST + pDstInfo->HDTOT - mvopTimingInfo.u16HActive_Start;
    //VDeEnd=VDeStart+VDeWidth
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (2 * mvopTimingInfo.u16VBlank0_End - 3);
    }
    else
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (mvopTimingInfo.u16VBlank0_End - 1);
    }
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, pDstInfo->HDTOT, pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", pDstInfo->DEHST, pDstInfo->DEHEND, pDstInfo->DEVST, pDstInfo->DEVEND);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, (int)pDstInfo->HDTOT, (int)pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", (int)pDstInfo->DEHST, (int)pDstInfo->DEHEND, (int)pDstInfo->DEVST, (int)pDstInfo->DEVEND);)
#endif
    _DRV_MVOP_Return(E_MVOP_DEV_0, TRUE);
}

MS_BOOL MDrv_MVOP_GetDstInfo(MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_DST_INFO IO_arg;
    IO_arg.pDstInfo = pDstInfo;
    IO_arg.u32SizeofDstInfo = u32SizeofDstInfo;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_DST_INFO, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetDstInfo_V2(pInstance, pDstInfo, u32SizeofDstInfo));
#endif
}


/********************************************************************************/
/// Used for Output Fix Vtotal of MVOP
/// @param u16FixVtt      \b IN
/********************************************************************************/
MS_BOOL MDrv_MVOP_SetFixVtt_V2(void* pInstance, MS_U16 u16FixVtt)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._u16FixVtt = u16FixVtt;

    _DRV_MVOP_Return(E_MVOP_DEV_0, TRUE);
}

MS_BOOL MDrv_MVOP_SetFixVtt(MS_U16 u16FixVtt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d\n", __FUNCTION__, __LINE__, u16FixVtt);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_FIXVTT IO_arg;
    IO_arg.u16FixVtt = u16FixVtt;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_FIXVTT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }
    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetFixVtt_V2(pInstance, u16FixVtt));
#endif
}

MS_BOOL MDrv_MVOP_EnableFreerunMode_V2(void* pInstance, MS_BOOL bEnable)
{
    HAL_MVOP_EnableFreerunMode(bEnable);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode = bEnable;
    return TRUE;
}

MS_BOOL MDrv_MVOP_EnableFreerunMode(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d\n", __FUNCTION__, __LINE__, bEnable);)
    HAL_MVOP_EnableFreerunMode(bEnable);
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnableFreerunMode = bEnable;
    return TRUE;
}

MVOP_Result MDrv_MVOP_MiuSwitch_V2(void* pInstance, MS_U8 u8Miu)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    _DRV_MVOP_Return(E_MVOP_DEV_0,MVOP_MiuSwitch(TRUE, u8Miu));
}

MVOP_Result MDrv_MVOP_MiuSwitch(MS_U8 u8Miu)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE

    stMVOP_MIU_SWITCH IO_arg;
    IO_arg.u8Miu = u8Miu;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_MIU_SWITCH, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_MiuSwitch_V2(pInstance, u8Miu));
#endif
}

MS_BOOL MDrv_MVOP_SetBaseAdd_V2(void* pInstance, MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }

    if (HAL_MVOP_SetYUVBaseAdd)
    {
        HAL_MVOP_SetYUVBaseAdd(u32YOffset, u32UVOffset, bProgressive, b422pack);
        HAL_MVOP_LoadReg();
        _DRV_MVOP_Return(E_MVOP_DEV_0, TRUE);
    }
    else
    {
        MVOP_ERR(MVOP_PRINTF("%s not support!\n", __FUNCTION__);)

        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
}

MS_BOOL MDrv_MVOP_SetBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
#if defined (__aarch64__)
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:0x%lx, arg2:0x%lx, arg3:%d, arg4:%d \n", __FUNCTION__, __LINE__,
                               u32YOffset, u32UVOffset, bProgressive, b422pack);)
#else
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:0x%x, arg2:0x%x, arg3:%d, arg4:%d \n", __FUNCTION__, __LINE__,
                               (int)u32YOffset, (int)u32UVOffset, bProgressive, b422pack);)
#endif
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_BASEADD IO_arg;
    IO_arg.u32YOffset = u32YOffset;
    IO_arg.u32UVOffset = u32UVOffset;
    IO_arg.bProgressive = bProgressive;
    IO_arg.b422pack = b422pack;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_BASEADD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetBaseAdd_V2(pInstance, u32YOffset, u32UVOffset, bProgressive, b422pack));
#endif
}

void MDrv_MVOP_SEL_OP_FIELD_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
#if defined(STB_DC_MODE)||(STB_DC)
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_MaskDBRegCtrl(E_MVOP_DEV_0, bEnable ,E_MVOP_SEL_OP_FIELD);
#endif

    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SEL_OP_FIELD (0x%x) \n",bEnable);)
}

void MDrv_MVOP_SEL_OP_FIELD(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SEL_OP_FIELD IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SEL_OP_FIELD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    MDrv_MVOP_SEL_OP_FIELD_V2(pInstance, bEnable);
#endif
}

void MDrv_MVOP_INV_OP_VS_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
#if STB_DC
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_MaskDBRegCtrl(E_MVOP_DEV_0, bEnable ,E_MVOP_INV_OP_VS);
#endif
    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_INV_OP_VS (0x%x) \n",bEnable);)
}

void MDrv_MVOP_INV_OP_VS(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_INV_OP_VS IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_INV_OP_VS, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_INV_OP_VS_V2(pInstance, bEnable);
#endif
}


void MDrv_MVOP_FORCE_TOP_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
#if STB_DC
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_ModeCtrl(bEnable ,E_MVOP_FORCE_TOP);
#endif
    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_FORCE_TOP (0x%x) \n",bEnable);)
}

void MDrv_MVOP_FORCE_TOP(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_FORCE_TOP IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_FORCE_TOP, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    MDrv_MVOP_FORCE_TOP_V2(pInstance, bEnable);
#endif
}


MS_BOOL MDrv_MVOP_SetImageWidthHight_V2(void* pInstance, MS_U16 u16Width ,MS_U16 u16Height)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, FALSE);
    }
#if defined(STB_DC_MODE)||(STB_DC)
    if(u16Width == 0 ||u16Height ==0)
    {
        HAL_MVOP_SetPicWidthMinus(E_MVOP_DEV_0, 0, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
        HAL_MVOP_SetPicHeightMinus(E_MVOP_DEV_0, 0, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
    }
    else
    {
        HAL_MVOP_SetPicWidthMinus(E_MVOP_DEV_0, u16Width-1, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
        HAL_MVOP_SetPicHeightMinus(E_MVOP_DEV_0, u16Height-1, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
    }
    p_gDrvMVOPCtx->_u16Width = u16Width;
    p_gDrvMVOPCtx->_u16Height =u16Height;
#else
    HAL_MVOP_SetPicWidthMinus(E_MVOP_DEV_0, u16Width, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
    HAL_MVOP_SetPicHeightMinus(E_MVOP_DEV_0, u16Height, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
#endif
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SetImageWidthHight (0x%x 0x%x) \n",u16Width ,u16Height);)
    _DRV_MVOP_Return(E_MVOP_DEV_0, TRUE);

}

MS_BOOL MDrv_MVOP_SetImageWidthHight(MS_U16 u16Width ,MS_U16 u16Height)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:%d, arg2:%d \n", __FUNCTION__, __LINE__, u16Width, u16Height);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_IMAGE_WIDTH_HIGHT IO_arg;
    IO_arg.u16Width = u16Width;
    IO_arg.u16Height = u16Height;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_IMAGE_WIDTH_HIGHT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetImageWidthHight_V2(pInstance, u16Width, u16Height));
#endif
}


void MDrv_MVOP_SetStartPos_V2(void* pInstance, MS_U16 u16Xpos ,MS_U16 u16Ypos)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx== NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    MVOP_Timing* pstTiming = NULL;
    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;

    HAL_MVOP_SetStartX(E_MVOP_DEV_0, u16Xpos);
    HAL_MVOP_SetStartY(E_MVOP_DEV_0, u16Ypos, pstTiming->bInterlace);
    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SetStartPos (0x%x 0x%x) \n",u16Xpos ,u16Ypos);)
}

void MDrv_MVOP_SetStartPos(MS_U16 u16Xpos ,MS_U16 u16Ypos)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:%d, arg2:%d \n", __FUNCTION__, __LINE__, u16Xpos, u16Ypos);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_START_POS IO_arg;
    IO_arg.u16Xpos = u16Xpos;
    IO_arg.u16Ypos = u16Ypos;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_START_POS, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SetStartPos_V2(pInstance, u16Xpos, u16Ypos);
#endif
}



void MDrv_MVOP_SetRegSizeFromMVD_V2(void* pInstance, MS_BOOL bEnable)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

#if defined(STB_DC_MODE)||(STB_DC)
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    p_gDrvMVOPCtx->_bRegSizeFromMVDStatus = bEnable;
    HAL_MVOP_SetDeb2MVDFrameModeCtrl(E_MVOP_DEV_0, p_gDrvMVOPCtx->_bRegSizeFromMVDStatus,E_MVOP_SIZE_FROM_MVD);
#endif
    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SetRegSizeFromMVD (0x%x) \n",bEnable);)
}

void MDrv_MVOP_SetRegSizeFromMVD(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_REGSIZE_FROM_MVD IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_REGSIZE_FROM_MVD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SetRegSizeFromMVD_V2(pInstance, bEnable);
#endif
}

//------------------------------------------------------------------------------
/// Set(Modify) MVOP clock in CLK_GEN0 0x4c
/// This command is effective after disable MVOP
/// (may cause bluescreen and framerate increased)
/// USAGE:
/// step1. MDrv_MVOP_Enable (FALSE)
/// step2. MDrv_MVOP_SetFrequency(MVOP_FREQUENCY)
/// step3. MDrv_MVOP_Enable (TRUE)
//------------------------------------------------------------------------------
void MDrv_MVOP_SetFrequency_V2(void* pInstance, MVOP_FREQUENCY eFreq)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (p_gDrvMVOPCtx== NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    HAL_MVOP_SetFrequency((HALMVOPFREQUENCY)eFreq);

    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP HAL_MVOP_SetFrequency (0x%x) \n",eFreq);)
}

void MDrv_MVOP_SetFrequency(MVOP_FREQUENCY eFreq)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, eFreq);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_FREQUENCY IO_arg;
    IO_arg.eFreq = eFreq;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SET_FREQUENCY, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    MDrv_MVOP_SetFrequency_V2(pInstance, eFreq);
#endif
}

//------------------------------------------------------------------------------
/// MVOP Enable/Disable mirror mode (chips after M10 support this feature)
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
void MDrv_MVOP_SetVOPMirrorMode_V2(void* pInstance, MS_BOOL bEnable,MVOP_DrvMirror eMirrorMode)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (HAL_MVOP_SetVerticalMirrorMode && HAL_MVOP_SetHorizontallMirrorMode)
    {
        if( eMirrorMode == E_VOPMIRROR_VERTICAL)
        {
            HAL_MVOP_SetVerticalMirrorMode(bEnable);
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVMirror= 1;
        }
        else if(eMirrorMode == E_VOPMIRROR_HORIZONTALL)
        {
            HAL_MVOP_SetHorizontallMirrorMode(bEnable);
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableHMirror= 1;
        }
        else if(eMirrorMode == E_VOPMIRROR_HVBOTH)
        {
            HAL_MVOP_SetVerticalMirrorMode(bEnable);
            HAL_MVOP_SetHorizontallMirrorMode(bEnable);
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVMirror= 1;
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableHMirror= 1;
        }
    }

    _DRV_MVOP_Release(E_MVOP_DEV_0);
    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SetVOPMirrorMode (eMirrorMode :0x%x , 0x%x) \n",eMirrorMode,bEnable);)
}

void MDrv_MVOP_SetVOPMirrorMode(MS_BOOL bEnable,MVOP_DrvMirror eMirrorMode)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:%d, arg2:%d \n", __FUNCTION__, __LINE__, bEnable, eMirrorMode);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_VOP_MIRROR_MODE IO_arg;
    IO_arg.bEnable = bEnable;
    IO_arg.eMirrorMode = eMirrorMode;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_VOP_MIRROR_MODE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SetVOPMirrorMode_V2(pInstance, bEnable, eMirrorMode);
#endif
}


//-----------------------------------------------------------------------------
/// Get the current displayed base address of MCU mode, unit of 8 bytes.
/// @param -u32YOffset: pointer to Y address
/// @param -u32UVOffset: pointer to UV address
//-----------------------------------------------------------------------------
void MDrv_MVOP_GetBaseAdd_V2(void* pInstance, MS_PHY* u64YOffset, MS_PHY* u64UVOffset)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, );
    }
    if (HAL_MVOP_GetYBaseAdd && HAL_MVOP_GetUVBaseAdd)
    {
        *u64YOffset =  HAL_MVOP_GetYBaseAdd();
        *u64UVOffset = HAL_MVOP_GetUVBaseAdd();
    }
    _DRV_MVOP_Release(E_MVOP_DEV_0);
}

void MDrv_MVOP_GetBaseAdd(MS_PHY* u64YOffset, MS_PHY* u64UVOffset)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_BASE_ADD IO_arg;
    IO_arg.u32YOffset = u64YOffset;
    IO_arg.u32UVOffset = u64UVOffset;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_BASE_ADD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_GetBaseAdd_V2(pInstance, u32YOffset, u32UVOffset);
#endif
}
#if 0
MS_BOOL MDrv_MVOP_SetBaseAddMultiView(MS_U32 u32YOffset, MS_U32 u32UVOffset,  MVOP_3DView eView, MS_BOOL bLoad)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

    if (HAL_MVOP_SetYUVBaseAddMultiView)
    {
        HAL_MVOP_SetYUVBaseAddMultiView(u32YOffset, u32YOffset, eView);
        if(bLoad)
        {
            HAL_MVOP_LoadReg();
        }
        _DRV_MVOP_Return(E_MVOP_DEV_0,TRUE);
    }
    else
    {
        MVOP_ERR(MVOP_PRINTF("%s not support!\n", __FUNCTION__);)

        _DRV_MVOP_Return(E_MVOP_DEV_0,FALSE);
    }
}

void MDrv_MVOP_GetBaseAddMultiView(MS_U32* u32YOffset, MS_U32* u32UVOffset, MVOP_3DView eView)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);
    if (HAL_MVOP_GetYBaseAddMultiView && HAL_MVOP_GetUVBaseAddMultiView)
    {
        *u32YOffset = HAL_MVOP_GetYBaseAddMultiView(eView);
        *u32UVOffset = HAL_MVOP_GetUVBaseAddMultiView(eView);
    }
    _DRV_MVOP_Release(E_MVOP_DEV_0);
}
#endif

//=============================================================================
// SUB MVOP
// Chips that support two MVOP modules: T12/T8, J2, A5.

//-----------------------------------------------------------------------------
/// Get status of MVOP driver
/// @param -pstatus: driver status
/// @return - TRUE / FALSE
//-----------------------------------------------------------------------------

void MDrv_MVOP_SubGetBaseAdd_V2(void* pInstance, MS_U32* u32YOffset, MS_U32* u32UVOffset)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    MVOP_SubCheckIsInit();
    if (HAL_MVOP_SubGetYBaseAdd && HAL_MVOP_SubGetUVBaseAdd)
    {
        *u32YOffset =  HAL_MVOP_SubGetYBaseAdd();
        *u32UVOffset = HAL_MVOP_SubGetUVBaseAdd();
    }
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubGetBaseAdd(MS_PHY* u32YOffset, MS_PHY* u32UVOffset)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_BASE_ADD IO_arg;
    IO_arg.u32YOffset = u32YOffset;
    IO_arg.u32UVOffset = u32UVOffset;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_BASEADD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

#else
    void* pInstance = NULL;
    MDrv_MVOP_SubGetBaseAdd_V2(pInstance, u32YOffset, u32UVOffset);
#endif
}

MS_BOOL MDrv_MVOP_SubGetStatus_V2(void* pInstance, MVOP_DrvStatus *pMVOPStat)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);

    if (p_gDrvSubMVOPCtx== NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }

    if (!MVOP_SubIsInit())
    {
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    _DRV_MVOP_Return(E_MVOP_DEV_1, MVOP_GetStatus(E_MVOP_DEV_1, pMVOPStat));
#else
    return FALSE;
#endif
}

/* Miss command: E_MVOP_CMD_SUB_GET_STATUS */
MS_BOOL MDrv_MVOP_SubGetStatus(MVOP_DrvStatus *pMVOPStat)
{
#if MVOP_SUPPORT_SUB
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvSubMVOPCtx== NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }

    if (!MVOP_SubIsInit())
    {
        return FALSE;
    }
    return MVOP_GetStatus(E_MVOP_DEV_1, pMVOPStat);
#else
    return FALSE;
#endif
}


//-----------------------------------------------------------------------------
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return HStart
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_SubGetHStart_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);

    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, 0);
    }
    MVOP_SubCheckIsInit_RetZero();
#if STB_DC
    return 0;
#else
    _DRV_MVOP_Return(E_MVOP_DEV_1, ((p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16HActive_Start)/2 + 2));
#endif
#else
    return 0;
#endif
}

MS_U16 MDrv_MVOP_SubGetHStart(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_HSTART IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_HSTART, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }
    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetHStart_V2(pInstance));
#endif
}

//-----------------------------------------------------------------------------
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return VStart
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_SubGetVStart_V2(void* pInstance)
{
    MS_U16 u16Vst = 0;
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, 0);
    }
    MVOP_SubCheckIsInit_RetZero();
#if STB_DC
    return u16Vst;
#else
    u16Vst = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16VBlank0_End - p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16TopField_VS;
    if (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.bInterlace==1)
    {
        u16Vst*=2;
        u16Vst-=18;
    }
    else
    {
        u16Vst-=5;
    }
    _DRV_MVOP_Return(E_MVOP_DEV_1, u16Vst);
#endif
#else
    return u16Vst;
#endif
}

MS_U16 MDrv_MVOP_SubGetVStart(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_VSTART IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_VSTART, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetVStart_V2(pInstance));
#endif
}

//-----------------------------------------------------------------------------
// Set MVOP clock enable
// @param bEnable \b IN
//   - # TRUE  Enable clock
//   - # FALSE Close clock
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubSetClk_V2 (void* pInstance, MS_BOOL bEnable )
{
#if MVOP_SUPPORT_SUB
    HAL_MVOP_SubSetDCClk(0, bEnable);  //Clk DC0
    HAL_MVOP_SubSetDCClk(1, bEnable);  //Clk DC1
    if(HAL_MVOP_SubSetDCSRAMClk)
    {
        HAL_MVOP_SubSetDCSRAMClk(0, bEnable);  //Clk DC1 sraam clock
    }
#ifdef UFO_MVOP_FB_DEC_CLK
    if(HAL_MVOP_SetFBDecClk)
    {
        HAL_MVOP_SetFBDecClk(E_MVOP_DEV_1, bEnable); // clk FBDec
    }
#endif
#endif
}

#if MVOP_SUPPORT_SUB
static void MDrv_MVOP_SubSetClk ( MS_BOOL bEnable )
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
    HAL_MVOP_SubSetDCClk(0, bEnable);  //Clk DC0
    HAL_MVOP_SubSetDCClk(1, bEnable);  //Clk DC1
    if(HAL_MVOP_SubSetDCSRAMClk)
    {
        HAL_MVOP_SubSetDCSRAMClk(0, bEnable);  //Clk DC1 sraam clock
    }
#ifdef UFO_MVOP_FB_DEC_CLK
    if(HAL_MVOP_SetFBDecClk)
    {
        HAL_MVOP_SetFBDecClk(E_MVOP_DEV_1, bEnable); // clk FBDec
    }
#endif
}
#endif

//-----------------------------------------------------------------------------
/// Set MVOP test pattern.
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubSetPattern_V2(void* pInstance, MVOP_Pattern enMVOPPattern)
{
    static MVOP_Pattern s_enMVOPPattern = MVOP_PATTERN_NORMAL;
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    if(enMVOPPattern == MVOP_PATTERN_DEFAULT)
    {
        enMVOPPattern = (MVOP_Pattern)(((MS_U32)(s_enMVOPPattern) + 1) % (MS_U32)MVOP_PATTERN_DEFAULT);
    }
    HAL_MVOP_SubSetPattern(enMVOPPattern);
    s_enMVOPPattern = enMVOPPattern;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#else
    UNUSED(s_enMVOPPattern);
#endif
}

void MDrv_MVOP_SubSetPattern(MVOP_Pattern enMVOPPattern)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, enMVOPPattern);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_PATTERN IO_arg;
    IO_arg.enMVOPPattern = enMVOPPattern;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_SET_PATTERN, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubSetPattern_V2(pInstance, enMVOPPattern);
#endif
}

//-----------------------------------------------------------------------------
/// Configure the tile format of the MVOP input.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SubSetTileFormat_V2(void* pInstance, MVOP_TileFormat eTileFmt)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    if (!MVOP_SubIsInit())
    {
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    _DRV_MVOP_Return(E_MVOP_DEV_1, HAL_MVOP_SubSetTileFormat(eTileFmt));
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubSetTileFormat(MVOP_TileFormat eTileFmt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, eTileFmt);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_TILE_FORMAT IO_arg;
    IO_arg.eTileFmt = eTileFmt;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_SET_TILE_FORMAT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }
    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubSetTileFormat_V2(pInstance, eTileFmt));
#endif
}

//-----------------------------------------------------------------------------
/// Set MMIO Base for MVOP.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SubSetMMIOMapBase_V2(void* pInstance)
{
    return MVOP_SetRegBaseAdd(HAL_MVOP_SubRegSetBase);
}

MS_BOOL MDrv_MVOP_SubSetMMIOMapBase(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    return MVOP_SetRegBaseAdd(HAL_MVOP_SubRegSetBase);
}

//-----------------------------------------------------------------------------
/// Initialize MVOP hardware and set it to hardwire mode
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubInit_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    MVOP_DrvInitCtxResults eRet = E_MVOP_INIT_FAIL;
    MS_BOOL pbFirstDrvInstant;
    MS_U16 u16ECOVersion;

    _DRV_MVOP_MutexCreate(E_MVOP_DEV_1);
    _DRV_MVOP_Entry(E_MVOP_DEV_1);

    eRet = _MVOP_SubInit_Context(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("MVOP_Init_Context FAILED!\n");)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
        if(MDrv_MVOP_SubSetMMIOMapBase() == FALSE)
        {
            _DRV_MVOP_Return(E_MVOP_DEV_1, );
        }
        //1. enable clock
        HAL_MVOP_SubInit();
        MDrv_MVOP_SubSetClk(TRUE);
        //2. enable sram power
#ifdef CONFIG_MSTAR_SRAMPD
        if(HAL_MVOP_SetSramPower)
        {
            HAL_MVOP_SetSramPower(E_MVOP_DEV_1, TRUE);
        }
#endif
        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit = TRUE;
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }

    _MVOP_SubInitVar(E_MVOP_DEV_1);
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();

    if(MDrv_MVOP_SubSetMMIOMapBase() == FALSE)
    {
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }

    if (HAL_MVOP_SubInit)
    {
        HAL_MVOP_SubInit();
    }
    else
    {
        MVOP_ERR(MVOP_PRINTF("***%s: SUB MVOP UNSUPPORTED!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    if(HAL_MVOP_SetECONumber && (!MVOP_IsInit()))
    {
        HAL_MVOP_SetECONumber(u16ECOVersion);
    }
    //HAL_MVOP_SubSetInputMode( VOPINPUT_HARDWIRE, NULL, u16ECOVersion);

    // Set fld inv & ofld_inv
    HAL_MVOP_SubSetFieldInverse(ENABLE, ENABLE);

    // Set Croma weighting off
    HAL_MVOP_SubSetChromaWeighting(ENABLE);
#if 0
#if ENABLE_3D_LR_MODE
    HAL_MVOP_SubEnable3DLR(DISABLE);
#endif
    MDrv_MVOP_SubEnableFreerunMode(FALSE);
    HAL_MVOP_SubEnableMVDInterface(FALSE);
#endif
    //1. enable clock
    MDrv_MVOP_SubSetClk(TRUE);
    //2. enable sram power
#ifdef CONFIG_MSTAR_SRAMPD
    if(HAL_MVOP_SetSramPower)
    {
        HAL_MVOP_SetSramPower(E_MVOP_DEV_1, TRUE);
    }
#endif
    HAL_MVOP_SubLoadReg();

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit = TRUE;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#else
    MVOP_ERR(MVOP_PRINTF("*** %s: SUB MVOP UNSUPPORTED!!\n", __FUNCTION__);)
#endif
}

void MDrv_MVOP_SubInit()
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        if(UtopiaOpen(MODULE_MVOP|MVOP_DRIVER_BASE, &pu32MVOP_Inst[E_MVOP_MAIN_STREAM], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MVOP_PRINTF("[Fail] UtopiaOpen MVOP failed\n");
            return;
        }
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_INIT, (void*)(IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail] %s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubInit_V2(pInstance);
#endif
}


//-----------------------------------------------------------------------------
/// Exit MVOP, turn off clock
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubExit_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    //1. disable mvop
    HAL_MVOP_SubSetMIUReqMask(TRUE);
    HAL_MVOP_SubEnable(FALSE, 25);
    HAL_MVOP_SubSetMIUReqMask(FALSE);
    //2.disable mvop sram
#ifdef CONFIG_MSTAR_SRAMPD
    if(HAL_MVOP_SetSramPower)
    {
        HAL_MVOP_SetSramPower(E_MVOP_DEV_1, FALSE);
    }
#endif
    //3.disable mvop clock
    MDrv_MVOP_SubSetClk(FALSE);
    if(HAL_MVOP_Exit)
    {
        HAL_MVOP_Exit(E_MVOP_DEV_1);
    }
    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit = FALSE;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubExit()
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE

    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_EXIT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubExit_V2(pInstance);
#endif
}

//-----------------------------------------------------------------------------
/// Enable and Disable MVOP
/// @param bEnable \b IN
///   - # TRUE  Enable
///   - # FALSE Disable and reset
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubEnable_V2(void* pInstance, MS_BOOL bEnable)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    MS_U8 u8FPS = 25; //Default fps = 25, set as normal case;


    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    if(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u8Framerate != 0)
    {
        u8FPS = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u8Framerate;
    }

    HAL_MVOP_SubSetMIUReqMask(TRUE);
    HAL_MVOP_SubEnable(bEnable, u8FPS);
    HAL_MVOP_SubSetMIUReqMask(FALSE);

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsEnable = bEnable;
    if(FALSE == bEnable)
    {
        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsSetTiming = bEnable;
    }
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubEnable ( MS_BOOL bEnable )
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_ENABLE IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_ENABLE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubEnable_V2(pInstance, bEnable);
#endif
}



//-----------------------------------------------------------------------------
/// Get if MVOP is enable or not.
/// @param pbEnable \b OUT
///   - # TRUE  Enable
///   - # FALSE Disable
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SubGetIsEnable_V2(void* pInstance, MS_BOOL * pbEnable)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);

    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if ((NULL == pbEnable) || (TRUE != MVOP_SubIsInit()))
    {
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    *pbEnable = HAL_MVOP_SubGetEnableState();
    _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_OK);
#else
    return E_MVOP_UNSUPPORTED;
#endif
}

MVOP_Result MDrv_MVOP_SubGetIsEnable (MS_BOOL* pbEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_IS_ENABLE IO_arg;
    IO_arg.pbEnable = pbEnable;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_IS_ENABLE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetIsEnable_V2(pInstance, pbEnable));
#endif
}

//-----------------------------------------------------------------------------
/// Set enable UVShift
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubEnableUVShift_V2(void* pInstance, MS_BOOL bEnable)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    HAL_MVOP_SubEnableUVShift(bEnable);

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsUVShift = bEnable;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubEnableUVShift(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_ENABLE_UV_SHIFT IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_ENABLE_UV_SHIFT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubEnableUVShift_V2(pInstance, bEnable);
#endif
}


//-----------------------------------------------------------------------------
/// Set enable black background
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubEnableBlackBG_V2(void * pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    HAL_MVOP_SubSetBlackBG();

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsBlackBG = TRUE;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubEnableBlackBG(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    void* IO_arg = NULL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_ENABLE_BLACK_BG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubEnableBlackBG_V2(pInstance);
#endif
}



//-----------------------------------------------------------------------------
/// MVOP mono mode (fix chroma value)
//-----------------------------------------------------------------------------
void MDrv_MVOP_SubSetMonoMode_V2(void* pInstance, MS_BOOL bEnable)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, );
    }
    MVOP_SubCheckIsInit();
    HAL_MVOP_SubSetMonoMode(bEnable);

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsMonoMode = bEnable;
    _DRV_MVOP_Release(E_MVOP_DEV_1);
#endif
}

void MDrv_MVOP_SubSetMonoMode(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnable);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_MONO_MODE IO_arg;
    IO_arg.bEnable = bEnable;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_SET_MONO_MODE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#else
    void* pInstance = NULL;
    MDrv_MVOP_SubSetMonoMode_V2(pInstance, bEnable);
#endif
}


//-----------------------------------------------------------------------------
/// Configure MVOP input.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SubSetInputCfg_V2(void * pInstance, MVOP_InputSel in, MVOP_InputCfg * pCfg)
{
    MVOP_Result ret = E_MVOP_OK;
#if MVOP_SUPPORT_SUB
    MS_U16 u16ECOVersion;

    _DRV_MVOP_Entry(E_MVOP_DEV_1);

    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }
    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetSubDrvCtrl(E_MVOP_DEV_1);

    if (NULL == pstDrvCtrl)
    {
        MVOP_DBG(MVOP_PRINTF("%s: Sub MVOP driver is not supported!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_UNSUPPORTED);
    }

    if (!MVOP_SubIsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();
    if (E_MVOP_RGB_NONE != pstDrvCtrl->_eRgbFmt)
    {
        if (HAL_MVOP_SubSetRgbFormat)
        {
            HAL_MVOP_SubSetRgbFormat(pstDrvCtrl->_eRgbFmt);
        }
        else
        {
            MVOP_ERR(MVOP_PRINTF("%s not support HAL_MVOP_SubSetRgbFormat\n", __FUNCTION__);)
        }
    }
    MVOP_ResetVar(E_MVOP_DEV_1);

    switch (in)
    {
#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_DRAM_3DLR:
            HAL_MVOP_SubEnable3DLR(ENABLE);
            //no break here to continue setting MCU mode
#endif
        case MVOP_INPUT_DRAM:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }

            HAL_MVOP_SubSetInputMode(VOPINPUT_MCUCTRL, pCfg, u16ECOVersion);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_HVD_3DLR:
            HAL_MVOP_SubEnable3DLR(ENABLE);
            //no break here to continue setting HVD hardwire mode
#endif
        case MVOP_INPUT_H264:
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SubSetH264HardwireMode(u16ECOVersion);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_MVD_3DLR:
            HAL_MVOP_SubEnable3DLR(ENABLE);
            //no break here to continue setting MVD hardwire mode
#endif
        case MVOP_INPUT_MVD:
            HAL_MVOP_SubEnableMVDInterface(TRUE);
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            break;
        case MVOP_INPUT_RVD:
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SubSetRMHardwireMode(u16ECOVersion);
            break;
        case MVOP_INPUT_CLIP:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }
            HAL_MVOP_SubSetCropWindow(pCfg);
            break;
        case MVOP_INPUT_JPD:
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SubSetJpegHardwireMode(u16ECOVersion);
            break;
#if SUPPORT_EVD_MODE
#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_EVD_3DLR:
            HAL_MVOP_SubEnable3DLR(ENABLE);
            //no break here to continue setting HVD hardwire mode
#endif
        case MVOP_INPUT_EVD:
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SubSetEVDHardwireMode(u16ECOVersion);
            break;
#endif
#if SUPPORT_VP9_MODE
        case MVOP_INPUT_VP9:
            HAL_MVOP_SubSetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
            HAL_MVOP_SubSetVP9HardwireMode(u16ECOVersion);
            break;
#endif
        default:
            ret = E_MVOP_INVALID_PARAM;
            break;
    }
    if (E_MVOP_OK == ret)
    {
        pstDrvCtrl->_eInputSel = in;
    }
    _DRV_MVOP_Return(E_MVOP_DEV_1, ret);
#else
    return ret;
#endif
}

MVOP_Result MDrv_MVOP_SubSetInputCfg (MVOP_InputSel in, MVOP_InputCfg* pCfg)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, in);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_INPUTCFG IO_arg;
    IO_arg.in = in;
    IO_arg.pCfg = pCfg;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_SET_INPUTCFG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubSetInputCfg_V2(pInstance, in, pCfg));
#endif
}


//-----------------------------------------------------------------------------
/// Configure MVOP output.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SubSetOutputCfg_V2(void* pInstance, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    MS_U16 u16ECOVersion;

    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if (!MVOP_SubIsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if (MVOP_CheckOutputCfg(pstVideoStatus) != TRUE)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_INVALID_PARAM);
    }

    MVOP_Timing* pSubTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming;
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();
    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopOutputCfg = *pstVideoStatus;

    if (MVOP_AutoGenMPEGTiming(FALSE, pSubTiming, pstVideoStatus, bEnHDup) != TRUE)
    {
        MVOP_DBG(MVOP_PRINTF("%s: fail to calculate timing!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if(HAL_MVOP_SetInterlaceType)
    {
        HAL_MVOP_SetInterlaceType(E_MVOP_DEV_1, u16ECOVersion, pstVideoStatus->u8Interlace);
    }

    HAL_MVOP_SubSetOutputInterlace(pSubTiming->bInterlace, u16ECOVersion);
    HAL_MVOP_SubSetOutputTiming(pSubTiming);
    HAL_MVOP_SubSetSynClk(pSubTiming);

    if (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._u16SrcHSize != 0)
    {
        //report the source width for scaler, intead of the 16-aligned width
        //MS_U16 u16Discard_Width = 0;
        //u16Discard_Width = _u16SubSrcHSize.u16Width - _u16SubSrcHSize;
        MVOP_DBG(MVOP_PRINTF("%s: _mvopSubTiming.u16Width: %d ==> %d\n", __FUNCTION__,
                             pSubTiming->u16Width, p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._u16SrcHSize);)
        pSubTiming->u16Width = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._u16SrcHSize;
#if 0   //for H-mirror ON
        if (_mvopTiming.u16HActive_Start > u16Discard_Width)
        {
            _mvopTiming.u16HActive_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HActive_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HActive_Start, u16Discard_Width);
        }
        if (_mvopTiming.u16HImg_Start > u16Discard_Width)
        {
            _mvopTiming.u16HImg_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HImg_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HImg_Start, u16Discard_Width);
        }
#endif
    }
    MVOP_DumpOutputTiming(pSubTiming);
    MS_BOOL* pbIsSetTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsSetTiming;
    *pbIsSetTiming = TRUE;
    _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_OK);
#else
    return E_MVOP_UNSUPPORTED;
#endif
}

MVOP_Result MDrv_MVOP_SubSetOutputCfg(MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, bEnHDup);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_OTPUTCFG IO_arg;
    IO_arg.pstVideoStatus = pstVideoStatus;
    IO_arg.bEnHDup = bEnHDup;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_SET_OTPUTCFG, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubSetOutputCfg_V2(pInstance, pstVideoStatus, bEnHDup));
#endif
}



//-----------------------------------------------------------------------------
/// Get MVOP output timing information.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SubGetOutputTiming_V2(void* pInstance, MVOP_Timing* pMVOPTiming)
{
    MVOP_Timing* pSubTiming = NULL;
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if (!MVOP_SubIsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }
    if (!p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsSetTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: Timing is not set yet!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_FAIL);
    }

    if (!pMVOPTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_INVALID_PARAM);
    }
    pSubTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming;
#if !(defined(CHIP_A3) || defined(CHIP_EDISON))
    memcpy(pMVOPTiming, pSubTiming, sizeof(MVOP_Timing));
#else
    pMVOPTiming->u16V_TotalCount      =  pSubTiming->u16V_TotalCount;
    pMVOPTiming->u16H_TotalCount      =  pSubTiming->u16H_TotalCount;
    pMVOPTiming->u16VBlank0_Start     =  pSubTiming->u16VBlank0_Start;
    pMVOPTiming->u16VBlank0_End       =  pSubTiming->u16VBlank0_End;
    pMVOPTiming->u16VBlank1_Start     =  pSubTiming->u16VBlank1_Start;
    pMVOPTiming->u16VBlank1_End       =  pSubTiming->u16VBlank1_End;
    pMVOPTiming->u16TopField_Start    =  pSubTiming->u16TopField_Start;
    pMVOPTiming->u16BottomField_Start =  pSubTiming->u16BottomField_Start;
    pMVOPTiming->u16TopField_VS       =  pSubTiming->u16TopField_VS;
    pMVOPTiming->u16BottomField_VS    =  pSubTiming->u16BottomField_VS;
    pMVOPTiming->u16HActive_Start     =  pSubTiming->u16HActive_Start;

    pMVOPTiming->bInterlace           =  pSubTiming->bInterlace;
    pMVOPTiming->u8Framerate          =  pSubTiming->u8Framerate;
    pMVOPTiming->u16H_Freq            =  pSubTiming->u16H_Freq;
    pMVOPTiming->u16Num               =  pSubTiming->u16Num;
    pMVOPTiming->u16Den               =  pSubTiming->u16Den;
    pMVOPTiming->u8MvdFRCType         =  pSubTiming->u8MvdFRCType;
    pMVOPTiming->u16ExpFrameRate      =  pSubTiming->u16ExpFrameRate;
    pMVOPTiming->u16Width             =  pSubTiming->u16Width;
    pMVOPTiming->u16Height            =  pSubTiming->u16Height;
    pMVOPTiming->bHDuplicate          =  pSubTiming->bHDuplicate;
#endif

#if defined(CHIP_KANO)
    //patch for GOP: 0x28[7]=1 need to toggle mcu mode, kano HAL_MVOP_Rst doing mcu mode toggling.
    if(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bGOPPatch == TRUE)
    {
        HAL_MVOP_SubRst();
        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bGOPPatch = FALSE;
    }
#endif
    _DRV_MVOP_Return(E_MVOP_DEV_1, E_MVOP_OK);
#else
    UNUSED(pSubTiming);
    return E_MVOP_UNSUPPORTED;
#endif
}

MVOP_Result MDrv_MVOP_SubGetOutputTiming(MVOP_Timing* pMVOPTiming)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_OUTPUT_TIMING IO_arg;
    IO_arg.pMVOPTiming = pMVOPTiming;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_OUTPUT_TIMING, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetOutputTiming_V2(pInstance, pMVOPTiming));
#endif
}


//-----------------------------------------------------------------------------
/// Get Horizontal Size.
/// @return Horizontal Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_SubGetHSize_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, 0);
    }
    MVOP_SubCheckIsInit_RetZero();
    _DRV_MVOP_Return(E_MVOP_DEV_1, (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16Width));
#else
    return 0;
#endif
}

MS_U16 MDrv_MVOP_SubGetHSize(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_HSIZE IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_HSIZE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetHSize_V2(pInstance));
#endif
}

//-----------------------------------------------------------------------------
/// Get Vertical Size.
/// @return Vertical Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_SubGetVSize_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, 0);
    }
    MVOP_SubCheckIsInit_RetZero();
    _DRV_MVOP_Return(E_MVOP_DEV_1, (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16Height));
#else
    return 0;
#endif
}

MS_U16 MDrv_MVOP_SubGetVSize(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_VSIZE IO_arg;
    IO_arg.u16Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_VSIZE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u16Ret;
    }

    return IO_arg.u16Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetVSize_V2(pInstance));
#endif
}

//-----------------------------------------------------------------------------
/// Get MVOP timing is interlace or progressive.
/// @return TRUE for interlace; FALSE for progressive
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SubGetIsInterlace_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    MVOP_SubCheckIsInit_RetZero();
    _DRV_MVOP_Return(E_MVOP_DEV_1, (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.bInterlace));
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubGetIsInterlace(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_IS_INTERLACE IO_arg;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_IS_INTERLACE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;

#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetIsInterlace_V2(pInstance));
#endif
}


//-----------------------------------------------------------------------------
/// Get MVOP timing is horizontal duplicated or not.
/// @return TRUE for yes; FALSE for not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SubGetIsHDuplicate_V2(void* pInstance)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    MVOP_SubCheckIsInit_RetZero();
    _DRV_MVOP_Return(E_MVOP_DEV_1, (p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.bHDuplicate));
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubGetIsHDuplicate(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_IS_HDUPLICATE IO_arg;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_IS_HDUPLICATE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetIsHDuplicate_V2(pInstance));
#endif
}

/******************************************************************************************/
/// This function is used for dectection of MVOP's Capability
/// @param u16HSize    \b IN
/// @param u16VSize    \b IN
/// @param u16Fps      \b IN
/// @return TRUE if MVOP's frequency can support this video source, otherwise return FASLE
/******************************************************************************************/
MS_BOOL MDrv_MVOP_SubCheckCapability_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_BOOL bSupport = FALSE;
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        bSupport = ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_SubGetMaxFreerunClk()));
    }
#endif
    return bSupport;
}

MS_BOOL MDrv_MVOP_SubCheckCapability(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_BOOL bSupport = FALSE;
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        bSupport = ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_SubGetMaxFreerunClk()));
    }
#endif
    return bSupport;
}


MS_U16 MDrv_MVOP_SubGetMaxHOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_U16 u16HttMax, u16OffsetMax = 0;
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        u16HttMax = HAL_MVOP_SubGetMaxFreerunClk() / u16Fps*1000 / (u16VSize + MVOP_VBlank);
        u16OffsetMax = (u16HttMax - MVOP_HBlank_HD - u16HSize)/2;
    }
#else
    UNUSED(u16HSize);
    UNUSED(u16VSize);
    UNUSED(u16Fps);
    UNUSED(u16HttMax);
#endif

    return u16OffsetMax;
}

MS_U16 MDrv_MVOP_SubGetMaxHOffset(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16HttMax, u16OffsetMax = 0;
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        u16HttMax = HAL_MVOP_SubGetMaxFreerunClk() / u16Fps*1000 / (u16VSize + MVOP_VBlank);
        u16OffsetMax = (u16HttMax - MVOP_HBlank_HD - u16HSize)/2;

    }
#else
    UNUSED(u16HSize);
    UNUSED(u16VSize);
    UNUSED(u16Fps);
    UNUSED(u16HttMax);
#endif

    return u16OffsetMax;
}

MS_U16 MDrv_MVOP_SubGetMaxVOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_U16 u16OffsetMax = 0;

#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        u16OffsetMax = MVOP_GetMaxVOffset(E_MVOP_DEV_1, HAL_MVOP_SubGetMaxFreerunClk(), u16HSize, u16VSize, u16Fps);
    }
#else
    UNUSED(u16HSize);
    UNUSED(u16VSize);
    UNUSED(u16Fps);
#endif

    return u16OffsetMax;
}

MS_U16 MDrv_MVOP_SubGetMaxVOffset(MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16OffsetMax = 0;

#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubGetMaxFreerunClk)
    {
        u16OffsetMax = MVOP_GetMaxVOffset(E_MVOP_DEV_1, HAL_MVOP_SubGetMaxFreerunClk(), u16HSize, u16VSize, u16Fps);
    }
#else
    UNUSED(u16HSize);
    UNUSED(u16VSize);
    UNUSED(u16Fps);
#endif

    return u16OffsetMax;
}

//------------------------------------------------------------------------------
/// MVOP Get Destination Information for GOP mixer
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_SubGetDstInfo_V2(void* pInstance, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }

    MVOP_SubCheckIsInit_RetZero();

    if(NULL == pDstInfo)
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():pDstInfo is NULL\n");)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }

    if(u32SizeofDstInfo != sizeof(MVOP_DST_DispInfo))
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():u16SizeofDstInfo is different from the MVOP_DST_DispInfo defined, check header file!\n");)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }

    MVOP_TimingInfo_FromRegisters mvopTimingInfo;
    memset(&mvopTimingInfo, 0, sizeof(MVOP_TimingInfo_FromRegisters));
    if(HAL_MVOP_SubGetTimingInfoFromRegisters(&mvopTimingInfo) == FALSE)
    {
        MVOP_ERR(MVOP_PRINTF("encounter errors in HAL_MVOP_SubGetTimingInfoFromRegisters()!\n");)
        memset(pDstInfo, 0, u32SizeofDstInfo);
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    pDstInfo->VDTOT = mvopTimingInfo.u16V_TotalCount;
    pDstInfo->HDTOT = mvopTimingInfo.u16H_TotalCount;
    pDstInfo->bInterlaceMode = mvopTimingInfo.bInterlace;
    pDstInfo->DEHST = mvopTimingInfo.u16HActive_Start / 2 + 2;
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) * 2 - 18;
    }
    else
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) - 5;
    }
    //HDeEnd=HDeStart+HDeWidth
    pDstInfo->DEHEND = pDstInfo->DEHST + pDstInfo->HDTOT - mvopTimingInfo.u16HActive_Start;
    //VDeEnd=VDeStart+VDeWidth
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (2 * mvopTimingInfo.u16VBlank0_End - 3);
    }
    else
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (mvopTimingInfo.u16VBlank0_End - 1);
    }
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, pDstInfo->HDTOT, pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", pDstInfo->DEHST, pDstInfo->DEHEND, pDstInfo->DEVST, pDstInfo->DEVEND);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, (int)pDstInfo->HDTOT, (int)pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", (int)pDstInfo->DEHST, (int)pDstInfo->DEHEND, (int)pDstInfo->DEVST, (int)pDstInfo->DEVEND);)
#endif
    _DRV_MVOP_Return(E_MVOP_DEV_1, TRUE);
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubGetDstInfo(MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_DST_INFO IO_arg;
    IO_arg.pDstInfo = pDstInfo;
    IO_arg.u32SizeofDstInfo = u32SizeofDstInfo;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_GET_DST_INFO, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetDstInfo_V2(pInstance, pDstInfo, u32SizeofDstInfo));
#endif
}


MS_BOOL MDrv_MVOP_SubSetFixVtt_V2(void* pInstance, MS_U16 u16FixVtt)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }

    p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._u16FixVtt = u16FixVtt;
    _DRV_MVOP_Return(E_MVOP_DEV_1, TRUE);
#else
    UNUSED(u16FixVtt);
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubSetFixVtt(MS_U16 u16FixVtt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, u16FixVtt);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_FIXVTT IO_arg;
    IO_arg.u16FixVtt = u16FixVtt;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_SET_FIXVTT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubSetFixVtt_V2(pInstance, u16FixVtt));
#endif
}

MS_BOOL MDrv_MVOP_SubEnableFreerunMode_V2(void* pInstance, MS_BOOL bEnable)
{
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubEnableFreerunMode)
    {
        HAL_MVOP_SubEnableFreerunMode(bEnable);
        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnableFreerunMode = bEnable;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    UNUSED(bEnable);
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubEnableFreerunMode(MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
#if MVOP_SUPPORT_SUB
    if (HAL_MVOP_SubEnableFreerunMode)
    {
        HAL_MVOP_SubEnableFreerunMode(bEnable);
        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnableFreerunMode = bEnable;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    UNUSED(bEnable);
    return FALSE;
#endif
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubEnableFreerunMode_V2(pInstance, bEnable));
#endif
}


MVOP_Result MDrv_MVOP_SubMiuSwitch_V2(void* pInstance, MS_U8 u8Miu)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    _DRV_MVOP_Return(E_MVOP_DEV_1, MVOP_MiuSwitch(FALSE, u8Miu));
#else
    return E_MVOP_UNSUPPORTED;
#endif
}

MVOP_Result MDrv_MVOP_SubMiuSwitch(MS_U8 u8Miu)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_MIU_SWITCH IO_arg;
    IO_arg.u8Miu = u8Miu;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_MIU_SWITCH, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubMiuSwitch_V2(pInstance, u8Miu));
#endif
}

MS_BOOL MDrv_MVOP_SubSetBaseAdd_V2(void* pInstance, MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
#if MVOP_SUPPORT_SUB
    _DRV_MVOP_Entry(E_MVOP_DEV_1);
    if (!MVOP_SubIsInit())
    {
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
    if (HAL_MVOP_SubSetYUVBaseAdd)
    {
        HAL_MVOP_SubSetYUVBaseAdd(u32YOffset, u32UVOffset, bProgressive, b422pack);
        HAL_MVOP_SubLoadReg();
        _DRV_MVOP_Return(E_MVOP_DEV_1, TRUE);
    }
    else
    {
        MVOP_ERR(MVOP_PRINTF("%s not support!\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_1, FALSE);
    }
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_MVOP_SubSetBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
#if defined (__aarch64__)
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:0x%lx, arg2:0x%lx, arg3:%d, arg4:%d \n", __FUNCTION__, __LINE__,
                               u32YOffset, u32UVOffset, bProgressive, b422pack);)
#else
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg1:0x%x, arg2:0x%x, arg3:%d, arg4:%d \n", __FUNCTION__, __LINE__,
                               (int)u32YOffset, (int)u32UVOffset, bProgressive, b422pack);)
#endif
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_SET_BASEADD IO_arg;
    IO_arg.u32YOffset = u32YOffset;
    IO_arg.u32UVOffset = u32UVOffset;
    IO_arg.bProgressive = bProgressive;
    IO_arg.b422pack = b422pack;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM],E_MVOP_CMD_SUB_SET_BASEADD, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubSetBaseAdd_V2(pInstance, u32YOffset, u32UVOffset, bProgressive, b422pack));
#endif
}

//------------------------------------------------------------------------------
/// MVOP Enable/Disable mirror mode (chips after M10 support this feature)
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
void MDrv_MVOP_SubSetVOPMirrorMode_V2(void* pInstance, MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode)
{
#if MVOP_SUPPORT_SUB
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    MVOP_SubCheckIsInit();

    if (HAL_MVOP_SubSetVerticalMirrorMode && HAL_MVOP_SubSetHorizontallMirrorMode)
    {
        if( eMirrorMode == E_VOPMIRROR_VERTICAL)
        {
            HAL_MVOP_SubSetVerticalMirrorMode(bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HORIZONTALL)
        {
            HAL_MVOP_SubSetHorizontallMirrorMode(bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HVBOTH)
        {
            HAL_MVOP_SubSetVerticalMirrorMode(bEnable);
            HAL_MVOP_SubSetHorizontallMirrorMode(bEnable);
        }
    }

    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SubSetVOPMirrorMode (eMirrorMode :0x%x , 0x%x) \n",eMirrorMode,bEnable);)
#endif
}
/*Miss E_MVOP_CMD_SUB_SET_VOP_MIRROR_MODE*/
void MDrv_MVOP_SubSetVOPMirrorMode(MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode)
{
#if MVOP_SUPPORT_SUB
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvSubMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    MVOP_SubCheckIsInit();

    if (HAL_MVOP_SubSetVerticalMirrorMode && HAL_MVOP_SubSetHorizontallMirrorMode)
    {
        if( eMirrorMode == E_VOPMIRROR_VERTICAL)
        {
            HAL_MVOP_SubSetVerticalMirrorMode(bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HORIZONTALL)
        {
            HAL_MVOP_SubSetHorizontallMirrorMode(bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HVBOTH)
        {
            HAL_MVOP_SubSetVerticalMirrorMode(bEnable);
            HAL_MVOP_SubSetHorizontallMirrorMode(bEnable);
        }
    }

    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_SubSetVOPMirrorMode (eMirrorMode :0x%x , 0x%x) \n",eMirrorMode,bEnable);)
#endif
}


MVOP_Result MDrv_MVOP_SendBlueScreen_V2(void* pInstance, MS_U16 u16Width, MS_U16 u16Height)
{
    _DRV_MVOP_Entry(E_MVOP_DEV_0);

#if STB_DC
    MS_U8 u8InitFieldCtl = (MVOP_EN_MASK_INIT_FIELD | MVOP_MASK_INIT_FIELD_CNT0 | MVOP_MASK_INIT_FIELD_CNT1
                            | MVOP_MASK_INIT_FIELD_CNT2 | MVOP_RST_MASK_INIT_FIELD | MVOP_FORCE_DEB_START);

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    if(u16Width == 0 || u16Height ==0)
    {
        MVOP_ERR(MVOP_PRINTF("\nMVOP the Input value could not be zero(0x%x ,0x%x) \n",u16Width ,u16Height);)
        _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
    }

    MDrv_MVOP_SEL_OP_FIELD(TRUE);
    //MDrv_MVOP_SetImageWidthHight(u16Width, u16Height);
    HAL_MVOP_SetPicWidthMinus(u16Width-1);
    HAL_MVOP_SetPicHeightMinus(u16Height-1);
    HAL_MVOP_SetDeb2MVDFrameModeCtrl(E_MVOP_DEV_0, FALSE, E_MVOP_SIZE_FROM_MVD);

    //Force clear STB_DC fb_mask
    HAL_MVOP_BaseFromIPCtrl(E_MVOP_DEV_0, TRUE, E_MVOP_FD_MASK_CLR);
    HAL_MVOP_SetInitFieldCtl(u8InitFieldCtl, u8InitFieldCtl);

    MsOS_DelayTask(90);

    HAL_MVOP_SetInitFieldCtl(MVOP_FORCE_DEB_START, u8InitFieldCtl);
    MDrv_MVOP_SEL_OP_FIELD(FALSE);
    HAL_MVOP_SetDeb2MVDFrameModeCtrl(E_MVOP_DEV_0, TRUE, E_MVOP_SIZE_FROM_MVD);
    HAL_MVOP_BaseFromIPCtrl(E_MVOP_DEV_0, FALSE, E_MVOP_FD_MASK_CLR);

    if(!p_gDrvMVOPCtx->_bRegSizeFromMVDStatus)
    {
        HAL_MVOP_SetDeb2MVDFrameModeCtrl(E_MVOP_DEV_0, FALSE, E_MVOP_SIZE_FROM_MVD);
        HAL_MVOP_SetPicWidthMinus(p_gDrvMVOPCtx->_u16Width-1);
        HAL_MVOP_SetPicHeightMinus(p_gDrvMVOPCtx->_u16Height-1);
    }
#endif

    _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_OK);
}

/*Error: should not bRet*/
MVOP_Result MDrv_MVOP_SendBlueScreen(MS_U16 u16Width, MS_U16 u16Height)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SEND_BLUE_SCREEN IO_arg;
    IO_arg.u16Width = u16Width;
    IO_arg.u16Height = u16Height;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SEND_BLUE_SCREEN, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SendBlueScreen_V2(pInstance, u16Width, u16Height));
#endif
}

static MVOP_DevID _MVOP_MapModNum2DevID(MVOP_Module eMod)
{
    MVOP_DevID  eDevID = E_MVOP_DEV_0; //default is to control main mvop

    switch (eMod)
    {
        case E_MVOP_MODULE_MAIN:
            eDevID = E_MVOP_DEV_0;
            break;

#if MVOP_SUPPORT_SUB
        case E_MVOP_MODULE_SUB:
            eDevID = E_MVOP_DEV_1;
            break;
#endif

        default:
            MVOP_DBG(MVOP_PRINTF("%s eMod=0x%x (set as main)\n", __FUNCTION__, eMod);)
            break;
    }

    return eDevID;
}


//-----------------------------------------------------------------------------
/// MVOP set command interface.
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_SetCommand_V2(void* pInstance, MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara)
{
    MVOP_Result eRet = E_MVOP_OK;
    MVOP_DevID  eDevID = E_MVOP_DEV_0; //default is to control main mvop
#if (STB_DC==0)
    MVOP_RgbFormat eRgbFmt = E_MVOP_RGB_NONE;
#endif
    MS_U16 u16ECOVersion;

    if (stHd != NULL)
    {
        eDevID = _MVOP_MapModNum2DevID(stHd->eModuleNum);
        MVOP_DBG(MVOP_PRINTF("%s eDevID=%x, eCmd=0x%x\n", __FUNCTION__, eDevID, eCmd);)
    }
    u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();
    _DRV_MVOP_Entry(eDevID);
    if(eDevID == E_MVOP_DEV_0)
    {
        if(eCmd != E_MVOP_CMD_SET_STREAM_INFO) //E_MVOP_CMD_SET_STREAM_INFO only set dram sharemem
        {
            if (!MVOP_IsInit())
            {
                MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
                _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
            }
        }
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eDevID == E_MVOP_DEV_1)
    {
        if(eCmd != E_MVOP_CMD_SET_STREAM_INFO)
        {
            if (!MVOP_SubIsInit())
            {
                MVOP_DBG(MVOP_PRINTF("%s:Sub MVOP driver is not initialized!\n", __FUNCTION__);)
                _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
            }
        }
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
    }
#endif

    if ((eCmd & E_MVOP_CMD_SET_TYPE) != E_MVOP_CMD_SET_TYPE)
    {
        MVOP_ERR(MVOP_PRINTF("%s: invalid set cmd 0x%x\n", __FUNCTION__, eCmd);)
        _DRV_MVOP_Return(eDevID, E_MVOP_INVALID_PARAM);
    }
    if (!pPara)
    {
        MVOP_ERR(MVOP_PRINTF("%s: null pPara\n", __FUNCTION__);)
        _DRV_MVOP_Return(eDevID, E_MVOP_INVALID_PARAM);
    }

    switch (eCmd)
    {
        case E_MVOP_CMD_SET_VSIZE_MIN:
            if(eDevID == E_MVOP_DEV_0)
            {
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnMinVSize576 = *(MS_BOOL*)pPara;
                MVOP_DBG(MVOP_PRINTF("Set _bEnMinVSize576 as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnMinVSize576);)
            }
#if MVOP_SUPPORT_SUB
            else if(E_MVOP_DEV_1 == eDevID)
            {
                p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._bEnMinVSize576 = *(MS_BOOL*)pPara;
                MVOP_DBG(MVOP_PRINTF("Set _bEnMinVSize576 as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnMinVSize576);)
            }
#endif
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;

        case E_MVOP_CMD_SET_STB_FD_MASK_CLR:
#if defined(STB_DC_MODE)||(STB_DC)
        {
            if(eDevID == E_MVOP_DEV_0)
            {
                MS_BOOL bEnable;
                bEnable =*(MS_BOOL*)pPara;
                HAL_MVOP_BaseFromIPCtrl(E_MVOP_DEV_0, bEnable, E_MVOP_FD_MASK_CLR);
                MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_ClearFDMask (0x%x) \n",bEnable);)
            }
        }
#else
        {
            MVOP_DBG(MVOP_PRINTF("\n not support this function!! \n");)
            eRet = E_MVOP_FAIL;
        }
#endif
        break;

#if (STB_DC==0) //Cmd supported for T-series chips
        case E_MVOP_CMD_SET_RGB_FMT:
            eRgbFmt = *(MVOP_RgbFormat*)pPara;
            if (HAL_MVOP_GetSupportRgbFormat && HAL_MVOP_GetSupportRgbFormat(eRgbFmt))
            {
                if(eDevID == E_MVOP_DEV_0)
                {
                    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._eRgbFmt= eRgbFmt;
                    MVOP_DBG(MVOP_PRINTF("Set _eRgbFmt as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._eRgbFmt);)
                }
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!!\n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;

//Commands for 3D L/R mode (default is frame packing output):
//  Frame packing output and insert vertical blanking between two views.
//  Need to set before calling MDrv_MVOP_SetOutputCfg() because timing will be different
        case E_MVOP_CMD_SET_3DLR_INST_VBLANK:
        {
            if(eDevID == E_MVOP_DEV_0)
            {
#if SUPPORT_3DLR_INST_VBLANK
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._u16InsVBlank= *(MS_U16*)pPara;
                MVOP_PRINTF("Set _u16InsVBlank as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._u16InsVBlank);
#else
                eRet = E_MVOP_FAIL;
#endif
            }
        }
        break;

//  Line alternative output
//  Support interlace or progressive output.
//  If interlace output, both view output the same field.
        case E_MVOP_CMD_SET_3DLR_ALT_OUT:
        {
            MS_BOOL bEnable;
            bEnable = *(MS_BOOL*)pPara;
            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_Set3DLRAltOutput_VHalfScaling)
            {
                HAL_MVOP_Set3DLRAltOutput_VHalfScaling(bEnable);
                HAL_MVOP_LoadReg();
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnable3DLRALT= 1;
            }
#if MVOP_SUPPORT_SUB
            else if ((E_MVOP_DEV_1 == eDevID) && HAL_MVOP_SubSet3DLRAltOutput_VHalfScaling)
            {
                HAL_MVOP_SubSet3DLRAltOutput_VHalfScaling(bEnable);
                HAL_MVOP_SubLoadReg();
                p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopComdFlag.bEnable3DLRALT= 1;
            }
#endif
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
        }
        break;

//  Line alternative read of 3D L/R mode, side-by-side output
//  Support interlace or progressive output.
//  If interlace output, both view output the same field.
//  Only support in Eagle.
        case E_MVOP_CMD_SET_3DLR_ALT_OUT_SBS:
        {
#if SUPPORT_3DLR_ALT_SBS
            MS_BOOL bEnable;
            bEnable = *(MS_BOOL*)pPara;
            if (E_MVOP_DEV_0 == eDevID)
            {
                HAL_MVOP_Set3DLRAltOutput(bEnable);
                HAL_MVOP_Set3DLRAltSBSOutput(bEnable);
                HAL_MVOP_LoadReg();
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnable3DLRSBS= 1;
            }
#if MVOP_SUPPORT_SUB
            else if (E_MVOP_DEV_1 == eDevID)
            {
                HAL_MVOP_SubSet3DLRAltOutput(bEnable);
                HAL_MVOP_SubSet3DLRAltSBSOutput(bEnable);
                HAL_MVOP_SubLoadReg();
                p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopComdFlag.bEnable3DLRSBS= 1;
            }
#endif
            else
#endif
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
        }
        break;

        case E_MVOP_CMD_SET_3DLR_2ND_CFG:
        {
            //Support 2nd pitch: A3 U02, A7P U02, Agate U01, Eagle, Elk U01, Emerald, Edison.
            //Support 2nd w & h: Eagle, Elk U01, Emerald, Edison.
            MS_BOOL bEnable;
            bEnable = *(MS_BOOL*)pPara;
            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_Set3DLR2ndCfg)
            {
                if (TRUE == HAL_MVOP_Set3DLR2ndCfg(bEnable))
                {
                    HAL_MVOP_LoadReg();
                }
                else
                {
                    MVOP_DBG(MVOP_PRINTF("\n %s hw not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                    eRet = E_MVOP_FAIL;
                }
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_UNSUPPORTED;
            }
        }
        break;

        case E_MVOP_CMD_SET_VSIZE_DUPLICATE:
        {
            //Supported after T12, except M12.
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnVSizeDup = bEnable;
            MVOP_DBG(MVOP_PRINTF("Set _bEnVSizeDup as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnVSizeDup);)

            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetVerDup)
            {
                HAL_MVOP_SetVerDup(bEnable);
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVdup= 1;
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }

        //Need to set before calling MDrv_MVOP_SetInputCfg()
        //For interlace source, single field input, such as H.264 decodeIframe of field type.
        case E_MVOP_CMD_SET_FIELD_DUPLICATE:
        {
            MVOP_RptFldMode eRptMode = E_MVOP_RPTFLD_NONE;
            eRptMode = *(MVOP_RptFldMode*)pPara;
            MVOP_DBG(MVOP_PRINTF("\n %s fieldRepeatMode 0x%x.\n", __FUNCTION__, eRptMode);)

            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetRepeatField)
            {
                HAL_MVOP_SetRepeatField(eRptMode);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }

        case E_MVOP_CMD_SET_VSYNC_MODE:
        {
            MS_U8 u8Mode = *(MS_U8*)pPara;
            MVOP_DBG(MVOP_PRINTF("\n %s VSyncMode 0x%x.\n", __FUNCTION__, u8Mode);)
            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetVSyncMode)
            {
                HAL_MVOP_SetVSyncMode(u8Mode);
            }
#if MVOP_SUPPORT_SUB
            else if((E_MVOP_DEV_1 == eDevID) && HAL_MVOP_SubSetVSyncMode)
            {
                HAL_MVOP_SubSetVSyncMode(u8Mode);
            }
#endif
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
#endif

        case E_MVOP_CMD_SET_SW_CTRL_FIELD_ENABLE:
#if STB_DC
        {
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            HAL_MVOP_SWFieldCtrl(ENABLE, E_MVOP_EN_SW_FIELD);
            HAL_MVOP_SWFieldCtrl(bEnable, E_MVOP_SW_FIELD_BOTTOM);
            MVOP_DBG(MVOP_PRINTF("\nMVOP HAL_MVOP_SWFieldCtrl (0x%x) \n", bEnable);)

        }
#else
        {
            MVOP_DBG(MVOP_PRINTF("\n not support this function!! \n");)
            eRet = E_MVOP_FAIL;
        }
#endif
        break;

        case E_MVOP_CMD_SET_SW_CTRL_FIELD_DSIABLE:
#if STB_DC
        {
            HAL_MVOP_SWFieldCtrl(DISABLE, E_MVOP_EN_SW_FIELD);
        }
#else
        {
            MVOP_DBG(MVOP_PRINTF("\n not support this function!! \n");)
            eRet = E_MVOP_FAIL;
        }
#endif
        break;

        case E_MVOP_CMD_SET_VSIZE_X4_DUPLICATE:
        {
            //Supported after Emerald.
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnVSizex4Dup = bEnable;
            MVOP_DBG(MVOP_PRINTF("Set _bEnVSizex4Dup as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnVSizex4Dup);)

            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetVerx4Dup)
            {
                HAL_MVOP_SetVerx4Dup(bEnable);
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVx4= 1;
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }

        case E_MVOP_CMD_SET_HSIZE_X4_DUPLICATE:
        {
            //Supported after Emerald.
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnHSizex4Dup = bEnable;
            MVOP_DBG(MVOP_PRINTF("Set _bEnHSizex4Dup as %x\n", p_gDrvMVOPCtx->_stMVOPDrvCtrl[eDevID]._bEnHSizex4Dup);)

            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetHorx4Dup)
            {
                HAL_MVOP_SetHorx4Dup(bEnable);
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableHx4= 1;
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }
        case E_MVOP_CMD_SET_BASE_ADD_MULTI_VIEW:
        {
#if ENABLE_3D_LR_MODE
            MVOP_BaseAddInput *stBaseAddInfo = (MVOP_BaseAddInput*)pPara;
            //MVOP_DBG(MVOP_PRINTF("\n %s fieldRepeatMode 0x%x.\n", __FUNCTION__, eRptMode);)
            if(stBaseAddInfo)
            {
                if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetYUVBaseAddMultiView)
                {
                    HAL_MVOP_SetYUVBaseAddMultiView(stBaseAddInfo);
                }
            }
            else
#endif
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_FIRE_MVOP:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            if(bEnable)
            {
                switch(eDevID)
                {
                    case E_MVOP_DEV_0:
                        HAL_MVOP_LoadReg();
                        break;
                    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
                        HAL_MVOP_SubLoadReg();
#endif
                        break;
                    case E_MVOP_DEV_2:
#if MVOP_SUPPORT_3RD
                        HAL_MVOP_EX_LoadReg(eDevID);
#endif
                        break;
                    default:
                        break;
                }
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s cmd 0x%x is not enable!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_OK;
            }
            break;
        }
        case E_MVOP_CMD_SET_VC1_RANGE_MAP:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_VC1RangeMapInfo *stVC1RangeMapInfo = (MVOP_VC1RangeMapInfo*)pPara;
            if(stVC1RangeMapInfo && HAL_MVOP_SetVC1RangeMap)
            {
                HAL_MVOP_SetVC1RangeMap(eDevID, stVC1RangeMapInfo);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_POWER_STATE:
        {
            // Please use API: MDrv_MVOP_SetPowerState
            break;
        }
        case E_MVOP_CMD_SET_420_BW_SAVING_MODE:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;
            if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_Set420BWSaveMode)
            {
                HAL_MVOP_Set420BWSaveMode(bEnable);
                p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableBWSave= 1;
                HAL_MVOP_LoadReg();
            }
#if MVOP_SUPPORT_SUB
            else if ((E_MVOP_DEV_1 == eDevID) && HAL_MVOP_SubSet420BWSaveMode)
            {
                HAL_MVOP_SubSet420BWSaveMode(bEnable, u16ECOVersion);
                p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopComdFlag.bEnableBWSave= 1;
                HAL_MVOP_SubLoadReg();
            }
#endif
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }
        case E_MVOP_CMD_SET_EVD_BASE_ADD:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_EVDBaseAddInput *stEVDBaseAddInfo = (MVOP_EVDBaseAddInput*)pPara;

            if(stEVDBaseAddInfo)
            {
                if ((E_MVOP_DEV_0 == eDevID) && HAL_MVOP_SetEVDYUVBaseAdd)
                {
                    HAL_MVOP_SetEVDYUVBaseAdd(stEVDBaseAddInfo);
                    HAL_MVOP_LoadReg();
                }
#if MVOP_SUPPORT_SUB
                else if ((E_MVOP_DEV_1 == eDevID) && HAL_MVOP_SubSetEVDYUVBaseAdd)
                {
                    HAL_MVOP_SubSetEVDYUVBaseAdd(stEVDBaseAddInfo);
                    HAL_MVOP_SubLoadReg();
                }
#endif
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_EVD_FEATURE:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)

            MVOP_EVDFeature *stEVDMCUInput = (MVOP_EVDFeature*)pPara;

            if(stEVDMCUInput)
            {
                if (HAL_MVOP_SetEVDFeature)
                {
                    HAL_MVOP_SetEVDFeature(eDevID, stEVDMCUInput);
                    HAL_MVOP_LoadReg();
                }
                else
                {
                    MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                }
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s Input data is NULL 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_MVD_LATE_REPEAT:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;

            if (HAL_MVOP_SetRptPreVsyncFrame)
            {
                HAL_MVOP_SetRptPreVsyncFrame(eDevID, bEnable);
                switch(eDevID)
                {
                    case E_MVOP_DEV_0:
                        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableRptPreVsync= bEnable;
                        break;
#if MVOP_SUPPORT_SUB
                    case E_MVOP_DEV_1:
                        p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopComdFlag.bEnableRptPreVsync= bEnable;
                        break;
#endif
                    default:
                        MVOP_DBG(MVOP_PRINTF("\n %s not support this device 0x%x!! \n", __FUNCTION__, eCmd);)
                        break;
                }

            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }

            break;
        }
        case E_MVOP_CMD_SET_HANDSHAKE_MODE:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            MS_U8 u8FPS = 25; //Default fps = 25, set as normal case;
            bEnable =*(MS_BOOL*)pPara;

            switch(eDevID)
            {
                case E_MVOP_DEV_0:
                {
                    if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u8Framerate != 0)
                    {
                        u8FPS = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u8Framerate;
                    }
                    break;
                }
#if MVOP_SUPPORT_SUB
                case E_MVOP_DEV_1:
                {
                    if(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u8Framerate != 0)
                    {
                        u8FPS = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u8Framerate;
                    }
                    break;
                }
#endif
                default:
                    break;
            }

            if (HAL_MVOP_SetHandShakeMode)
            {
                HAL_MVOP_SetHandShakeMode(eDevID, bEnable, u8FPS);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_FRC_OUTPUT:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_U32 u32FPS;
            u32FPS =*(MS_U32*)pPara;

            if (HAL_MVOP_SupportFRCOutputFPS)
            {
                switch(eDevID)
                {
                    case E_MVOP_DEV_0:
                    {
                        if(TRUE == HAL_MVOP_SupportFRCOutputFPS(E_MVOP_DEV_0))
                        {
                            MVOP_Timing* pstFRCTiming = NULL;
                            pstFRCTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;

                            MVOP_VidStat stFRCVideoStatus;
                            MS_BOOL bFRCEnHDup;
                            stFRCVideoStatus = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopOutputCfg;
                            stFRCVideoStatus.u16FrameRate = u32FPS;
                            bFRCEnHDup = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnHx2Dup;
                            if (MVOP_AutoGenMPEGTiming(TRUE, pstFRCTiming, &stFRCVideoStatus, bFRCEnHDup) != TRUE)
                            {
                                MVOP_PRINTF("%s: fail to calculate timing!\n", __FUNCTION__);
                                _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
                            }
                            HAL_MVOP_SetOutputInterlace(pstFRCTiming->bInterlace, u16ECOVersion);
                            HAL_MVOP_SetOutputTiming(pstFRCTiming);
                            HAL_MVOP_SetSynClk(pstFRCTiming);
                            MVOP_DumpOutputTiming(pstFRCTiming);
                            MS_BOOL* pbIsSetTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsSetTiming;
                            *pbIsSetTiming = TRUE;
#ifdef UFO_MVOP_DOLBY_HDR
                            if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEn265DV)
                            {
                                HAL_MVOP_SetEnable4k2k2P(TRUE);
                                HAL_MVOP_SubSetEnable4k2k2P(TRUE);
                                HAL_MVOP_SubSetOutputInterlace(pstFRCTiming->bInterlace, u16ECOVersion);
                                HAL_MVOP_SubSetOutputTiming(pstFRCTiming);
                                HAL_MVOP_SubSetSynClk(pstFRCTiming);
                                if(HAL_MVOP_Enable2DCTimingSync)
                                {
                                    HAL_MVOP_Enable2DCTimingSync(TRUE);
                                }
                                HAL_MVOP_Enable(TRUE, 0);
                            }
#endif
                        }
                    }
                    break;
                    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
                    {
                        if(TRUE == HAL_MVOP_SupportFRCOutputFPS(E_MVOP_DEV_1))
                        {
                            MVOP_Timing* pstFRCTiming = NULL;
                            pstFRCTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming;

                            MVOP_VidStat stFRCVideoStatus;
                            MS_BOOL bFRCEnHDup;
                            stFRCVideoStatus = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopOutputCfg;
                            stFRCVideoStatus.u16FrameRate = u32FPS;
                            bFRCEnHDup = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bEnHx2Dup;

                            if (MVOP_AutoGenMPEGTiming(TRUE, pstFRCTiming, &stFRCVideoStatus, bFRCEnHDup) != TRUE)
                            {
                                MVOP_PRINTF("%s: fail to calculate timing!\n", __FUNCTION__);
                                _DRV_MVOP_Return(E_MVOP_DEV_0, E_MVOP_FAIL);
                            }

                            HAL_MVOP_SubSetOutputInterlace(pstFRCTiming->bInterlace, u16ECOVersion);
                            HAL_MVOP_SubSetOutputTiming(pstFRCTiming);
                            HAL_MVOP_SubSetSynClk(pstFRCTiming);

                            MVOP_DumpOutputTiming(pstFRCTiming);
                            MS_BOOL* pbIsSetTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsSetTiming;
                            *pbIsSetTiming = TRUE;
                        }
                    }
#endif
                    break;
                    default:
                        break;
                }
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
        break;
        }
#if defined(STB_DC_MODE)||(STB_DC)
        /*******************************************************/
        // Usage: Dynamic change timing generate from xc or mvop
        // set command before mvop setoutputcfg
        /*******************************************************/
        case E_MVOP_CMD_SET_XC_GEN_TIMING:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;

            if (HAL_MVOP_SetTimingFromXC)
            {
                HAL_MVOP_SetTimingFromXC(eDevID, bEnable);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
#endif

#if (defined(STB_DC_MODE)||(STB_DC))&&(MVOP_SUPPORT_SUB)
        case E_MVOP_CMD_SET_SEL_OP_FIELD:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;

            if (HAL_MVOP_MaskDBRegCtrl)
            {
                HAL_MVOP_MaskDBRegCtrl(eDevID, bEnable ,E_MVOP_SEL_OP_FIELD);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_SIZE_FROM_MVD:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;

            p_gDrvMVOPCtx->_bRegSizeFromMVDStatus = bEnable;
            HAL_MVOP_SetDeb2MVDFrameModeCtrl(eDevID, p_gDrvMVOPCtx->_bRegSizeFromMVDStatus,E_MVOP_SIZE_FROM_MVD);
            break;
        }
        case E_MVOP_CMD_SET_CROP_START_POS:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_SetCropPos stCropPos;
            stCropPos =*(MVOP_SetCropPos*)pPara;

            MVOP_Timing* pstTiming = NULL;
            pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;

            HAL_MVOP_SetStartX(eDevID, stCropPos.u16Xpos);
            HAL_MVOP_SetStartY(eDevID, stCropPos.u16Ypos, pstTiming->bInterlace);

            break;
        }
        case E_MVOP_CMD_SET_IMAGE_WIDTH_HEIGHT:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_SetImageWH stCropSize;
            stCropSize =*(MVOP_SetImageWH*)pPara;

            if(stCropSize.u16Width == 0 || stCropSize.u16Height == 0)
            {
                HAL_MVOP_SetPicWidthMinus(eDevID, 0, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
                HAL_MVOP_SetPicHeightMinus(eDevID, 0, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
            }
            else
            {
                HAL_MVOP_SetPicWidthMinus(eDevID, stCropSize.u16Width-1, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width);
                HAL_MVOP_SetPicHeightMinus(eDevID, stCropSize.u16Height-1, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
            }
            p_gDrvMVOPCtx->_u16Width = stCropSize.u16Width;
            p_gDrvMVOPCtx->_u16Height = stCropSize.u16Height;
            break;
        }
#endif
        /*******************************************************/
        // Usage: set stream feature to mvop, only for AN vsyncbridge so far.
        // support setting as bIsInit = 0.
        // E_MVOP_SET_SI_DV_SINGLE: set dolby single layer
        // E_MVOP_SET_SI_DV_DUAL: set dolby dual layer
        /*******************************************************/
        case E_MVOP_CMD_SET_STREAM_INFO:
        {
            MVOP_SetStreamInfo eStreamInfo;
            eStreamInfo =*(MVOP_SetStreamInfo*)pPara;
            MVOP_DBG(MVOP_PRINTF("\n %s.E_MVOP_CMD_SET_STREAM_INFO, enum = 0x%x\n", __FUNCTION__,eStreamInfo);)

            switch(eDevID)
            {
                case E_MVOP_DEV_0:
                {
                    switch(eStreamInfo)
                    {
                        case E_MVOP_SET_SI_NONE:
                            break;
                        case E_MVOP_SET_SI_DV_SINGLE:
                        {
                            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual = FALSE;
                            break;
                        }
                        case E_MVOP_SET_SI_DV_DUAL:
                        {
#if MVOP_SUPPORT_SUB
                            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._bIsDVDual = TRUE;
#endif
                            break;
                        }
                    }
                    break;
                }
#if MVOP_SUPPORT_SUB
                case E_MVOP_DEV_1:
                {
                    switch(eStreamInfo)
                    {
                        case E_MVOP_SET_SI_NONE:
                            break;
                        case E_MVOP_SET_SI_DV_SINGLE:
                        case E_MVOP_SET_SI_DV_DUAL:
                            //do nothing in sub mvop
                            MVOP_PRINTF("\n %s do nothing in sub mvop\n", __FUNCTION__);
                            break;
                    }
                    break;
                }
#endif
                default:
                {
                    MVOP_PRINTF("\n %s error mvop id index\n", __FUNCTION__);
                }
            }
            break;
        }
        /*******************************************************/
        // Usage: set before MDrv_MVOP_SetInputCfg .
        // TRUE for yvyu.
        /*******************************************************/
        case E_MVOP_CMD_SET_UV_SWAP:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MS_BOOL bEnable;
            bEnable =*(MS_BOOL*)pPara;

            if (HAL_MVOP_UVSwapEnable)
            {
                HAL_MVOP_UVSwapEnable(eDevID, bEnable);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
        case E_MVOP_CMD_SET_MFDEC_INFO:
        {
            MVOP_MCUMFDInfo*  pMFDECInfo;
            pMFDECInfo =(MVOP_MCUMFDInfo*)pPara;
            HALMVOPMFDECINFO stHalMFDECInfo;
            stHalMFDECInfo.bMFDEC_EN = pMFDECInfo->bMFDEC_EN;
            stHalMFDECInfo.u8MFDEC_ID = pMFDECInfo->u8MFDEC_ID;
            stHalMFDECInfo.u32UNCOMPRESS_MODE = pMFDECInfo->u32UNCOMPRESS_MODE;
            stHalMFDECInfo.u32BITLEN_FB_ADDR = pMFDECInfo->u32BITLEN_FB_ADDR;
            stHalMFDECInfo.u8BITLEN_FB_MIU = pMFDECInfo->u8BITLEN_FB_MIU;
            stHalMFDECInfo.u32BITLEN_FB_PITCH = pMFDECInfo->u32BITLEN_FB_PITCH;
            stHalMFDECInfo.bBITLEN_SHT_8 = pMFDECInfo->bBITLEN_SHT_8;
            HAL_MVOP_SetMFDECInfo(eDevID, &stHalMFDECInfo);
            break;
        }
        default:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
            eRet = E_MVOP_INVALID_PARAM;
        }
            break;
    }

    _DRV_MVOP_Return(eDevID, eRet);
    //return eRet;
}

MVOP_Result MDrv_MVOP_SetCommand(MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:0x%x \n", __FUNCTION__, __LINE__, eCmd);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_COMMAND IO_arg;
    IO_arg.stHd = stHd;
    IO_arg.eCmd = eCmd;
    IO_arg.pPara = pPara;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_COMMAND, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SetCommand_V2(pInstance, stHd, eCmd, pPara));
#endif
}


//-----------------------------------------------------------------------------
/// MVOP get command interface.
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_GetCommand_V2(void* pInstance, MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara, MS_U32 u32ParaSize)
{
    MVOP_Result eRet = E_MVOP_OK;
    MVOP_DevID  eDevID = E_MVOP_DEV_0; //default is to control main mvop
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (stHd != NULL)
    {
        eDevID = _MVOP_MapModNum2DevID(stHd->eModuleNum);
        MVOP_DBG(MVOP_PRINTF("%s eDevID=%x, eCmd=0x%x\n", __FUNCTION__, eDevID, eCmd);)
    }
    _DRV_MVOP_Entry(eDevID);

    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
    }
    if ((eCmd & E_MVOP_CMD_GET_TYPE) != E_MVOP_CMD_GET_TYPE)
    {
        MVOP_ERR(MVOP_PRINTF("%s: invalid get cmd 0x%x\n", __FUNCTION__, eCmd);)
        _DRV_MVOP_Return(eDevID, E_MVOP_INVALID_PARAM);
    }
    if (!pPara)
    {
        MVOP_ERR(MVOP_PRINTF("%s: null pPara\n", __FUNCTION__);)
        _DRV_MVOP_Return(eDevID, E_MVOP_INVALID_PARAM);
    }

    if(eDevID == E_MVOP_DEV_0)
    {
        if (!MVOP_IsInit())
        {
            MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
    }
#if MVOP_SUPPORT_SUB
    else if(eDevID == E_MVOP_DEV_1)
    {
        if (!MVOP_SubIsInit())
        {
            MVOP_DBG(MVOP_PRINTF("%s:Sub MVOP driver is not initialized!\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvSubMVOPCtx is NULL pointer\n", __FUNCTION__);)
            _DRV_MVOP_Return(eDevID, E_MVOP_FAIL);
        }
    }
#endif
    switch (eCmd)
    {
#if (STB_DC==0) //Cmd supported for T-series chips
        case E_MVOP_CMD_GET_3DLR_ALT_OUT:
            if ((HAL_MVOP_Get3DLRAltOutput) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_Get3DLRAltOutput();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;

        case E_MVOP_CMD_GET_3DLR_2ND_CFG:
            if ((HAL_MVOP_Get3DLR2ndCfg) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_Get3DLR2ndCfg();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;

        case E_MVOP_CMD_GET_MIRROR_MODE:
            if ((HAL_MVOP_GetMirrorMode) && (sizeof(MVOP_DrvMirror) == u32ParaSize))
            {
                *(MVOP_DrvMirror*)pPara = HAL_MVOP_GetMirrorMode(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;

        case E_MVOP_CMD_GET_VSIZE_DUPLICATE:
            if ((HAL_MVOP_GetVerDup) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_GetVerDup();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;

        case E_MVOP_CMD_GET_BASE_ADD_BITS:
            if (sizeof(MS_U8) == u32ParaSize)
            {
                *(MS_U8*)pPara = MVOP_BASE_ADD_BITS;
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_TOTAL_MVOP_NUM:
            if (sizeof(MS_U32) == u32ParaSize)
            {
                *(MS_U32*)pPara = HAL_MVOP_MODULE_CNT;
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
#endif
        case E_MVOP_CMD_GET_MAXIMUM_CLK:
            if ((HAL_MVOP_GetMaximumClk) && (sizeof(MVOP_FREQUENCY) == u32ParaSize))
            {
                *(MVOP_FREQUENCY*)pPara = (MVOP_FREQUENCY)HAL_MVOP_GetMaximumClk();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_CURRENT_CLK:
            if ((HAL_MVOP_GetCurrentClk) && (sizeof(MVOP_FREQUENCY) == u32ParaSize))
            {
                *(MVOP_FREQUENCY*)pPara = (MVOP_FREQUENCY)HAL_MVOP_GetCurrentClk();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_BASE_ADD_MULTI_VIEW:
#if ENABLE_3D_LR_MODE
            if (HAL_MVOP_GetYBaseAddMultiView && HAL_MVOP_GetUVBaseAddMultiView && (sizeof(MVOP_BaseAddInput) == u32ParaSize))
            {
                if (pPara)
                {
                    MVOP_BaseAddInput *pBaseAddInfo = (MVOP_BaseAddInput *)pPara;
                    pBaseAddInfo->u32YOffset = (MS_PHY)HAL_MVOP_GetYBaseAddMultiView(pBaseAddInfo->eView);
                    pBaseAddInfo->u32UVOffset = (MS_PHY)HAL_MVOP_GetUVBaseAddMultiView(pBaseAddInfo->eView);
                }
            }
            else
#endif
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_TOP_FIELD_IMAGE_VSTART:
            if ((HAL_MVOP_GetTopVStart) && (sizeof(MS_U16) == u32ParaSize))
            {
                *(MS_U16*)pPara = HAL_MVOP_GetTopVStart(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_BOTTOM_FIELD_IMAGE_VSTART:
            if ((HAL_MVOP_GetBottomVStart) && (sizeof(MS_U16) == u32ParaSize))
            {
                *(MS_U16*)pPara = HAL_MVOP_GetBottomVStart(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_VCOUNT:
            if ((HAL_MVOP_GetVCount) && (sizeof(MS_U16) == u32ParaSize))
            {
                *(MS_U16*)pPara = HAL_MVOP_GetVCount(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_HANDSHAKE_MODE:
            if ((HAL_MVOP_GetHandShakeMode) && (sizeof(MVOP_HSMode) == u32ParaSize))
            {
                *(MVOP_HSMode*)pPara = HAL_MVOP_GetHandShakeMode(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
        case E_MVOP_CMD_GET_MAX_FPS:
            if ((HAL_MVOP_GetMaxFramerate) && (sizeof(MVOP_GetMaxFps) == u32ParaSize))
            {
                HAL_MVOP_GetMaxFramerate(eDevID, (MVOP_GetMaxFps*)pPara);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
#if 1
        case E_MVOP_CMD_GET_INPUT:
            pstDrvCtrl = MVOP_GetDrvCtrl(E_MVOP_DEV_0);
            if(pstDrvCtrl)
            {
                *(MVOP_InputSel*)pPara = pstDrvCtrl->_eInputSel;
                eRet = E_MVOP_OK;
            }
            else
            {
                eRet = E_MVOP_FAIL;
            }
            break;
        case E_MVOP_CMD_GET_SUB_INPUT:
            pstDrvCtrl = MVOP_GetSubDrvCtrl(E_MVOP_DEV_1);
            if(pstDrvCtrl)
            {
                *(MVOP_InputSel*)pPara = pstDrvCtrl->_eInputSel;
                eRet = E_MVOP_OK;
            }
            else
            {
                eRet = E_MVOP_FAIL;
            }
            break;
#endif


        case E_MVOP_CMD_GET_CROP_FOR_XC:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_XCGetCrop* stCropInfo;
            stCropInfo = (MVOP_XCGetCrop*)pPara;

            if (HAL_MVOP_SetCropforXC)
            {
                HAL_MVOP_SetCropforXC(eDevID, stCropInfo, p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width,
                                      p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;
        }
#ifdef UFO_MVOP_GET_IS_SW_CROP
        case E_MVOP_CMD_GET_MVOP_SW_CROP_ADD:
        {
            MVOP_DBG(MVOP_PRINTF("\n %s.\n", __FUNCTION__);)
            MVOP_XCGetCropMirAdd* stADDInfo;
            stADDInfo = (MVOP_XCGetCropMirAdd*)pPara;

            if (HAL_MVOP_GetCropAddbySW)
            {
                HAL_MVOP_GetCropAddbySW(eDevID, stADDInfo);
            }
            else
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_FAIL;
            }
            break;

        }
        case E_MVOP_CMD_GET_IS_SW_CROP:
        {
            if ((HAL_MVOP_GetIsSWCrop) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_GetIsSWCrop();
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;

        }
#endif
#ifdef UFO_MVOP_GET_HV_RATIO
        case E_MVOP_CMD_GET_OUTPUT_HV_RATIO:
        {
            MVOP_OutputHVRatio* stRatio;
            stRatio = (MVOP_OutputHVRatio*)pPara;
            switch(eDevID)
            {
                case E_MVOP_DEV_0:
                {
                    stRatio->fHratio = (float)(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Width) /
                                       (float)(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopOutputCfg.u16HorSize);
                    stRatio->fVratio = (float)(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming.u16Height) /
                                       (float)(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopOutputCfg.u16VerSize);
                }
                MVOP_DBG(MVOP_PRINTF("[MVOP][Debug] H rat = %f, V rat = %f \n",stRatio->fHratio, stRatio->fVratio);)
                break;
                case E_MVOP_DEV_1:
                {
                    stRatio->fHratio = (float)(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16Width) /
                                       (float)(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopOutputCfg.u16HorSize);
                    stRatio->fVratio = (float)(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming.u16Height) /
                                       (float)(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopOutputCfg.u16VerSize);
                }
                MVOP_DBG(MVOP_PRINTF("[MVOP][Debug] H rat = %f, V rat = %f \n",stRatio->fHratio, stRatio->fVratio);)
                break;
                default:
                    break;
            }

        }
        break;
#endif
#if defined(STB_DC_MODE)||(STB_DC)
        case E_MVOP_CMD_GET_IS_XC_GEN_TIMING:
            if ((HAL_MVOP_GetIsCurrentXCGenTiming) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_GetIsCurrentXCGenTiming(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
#endif
#ifdef UFO_MVOP_GET_IS_MVOP_AUTO_GEN_BLACK
         case E_MVOP_CMD_GET_IS_MVOP_AUTO_GEN_BLACK:
            if ((HAL_MVOP_GetIsMVOPSupportBLKBackground) && (sizeof(MS_BOOL) == u32ParaSize))
            {
                *(MS_BOOL*)pPara = HAL_MVOP_GetIsMVOPSupportBLKBackground(eDevID);
            }
            else
            {
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
#endif

        case E_MVOP_CMD_GET_TYPE:
            break;
        default:
            {
                MVOP_DBG(MVOP_PRINTF("\n %s not support this cmd 0x%x!! \n", __FUNCTION__, eCmd);)
                eRet = E_MVOP_INVALID_PARAM;
            }
            break;
    }

    _DRV_MVOP_Return(eDevID, eRet);
}

MVOP_Result MDrv_MVOP_GetCommand(MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara, MS_U32 u32ParaSize)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:0x%x \n", __FUNCTION__, __LINE__, eCmd);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_COMMAND IO_arg;
    IO_arg.stHd = stHd;
    IO_arg.eCmd = eCmd;
    IO_arg.pPara = pPara;
    IO_arg.u32ParaSize = u32ParaSize;
    IO_arg.eRet = E_MVOP_FAIL;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_COMMAND, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.eRet;
    }
    return IO_arg.eRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetCommand_V2(pInstance, stHd, eCmd, pPara, u32ParaSize));
#endif
}


//=============================================================================
// The 3rd MVOP
// Chips that support three MVOP modules: Agate.

//-----------------------------------------------------------------------------
/// Get status of MVOP driver
/// @param -pstatus: driver status
/// @return - TRUE / FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_EX_GetStatus_V2(void* pInstance, MVOP_DevID eID, MVOP_DrvStatus *pMVOPStat)
{
    return MVOP_GetStatus(eID, pMVOPStat);
}

MS_BOOL MDrv_MVOP_EX_GetStatus(MVOP_DevID eID, MVOP_DrvStatus *pMVOPStat)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    return MVOP_GetStatus(eID, pMVOPStat);
}

//-----------------------------------------------------------------------------
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return HStart
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_EX_GetHStart_V2(void* pInstance, MVOP_DevID eID)
{
    MS_U16 u16HStart = 0;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }

    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    u16HStart = (pstDrvCtrl) ? ((pstDrvCtrl->_mvopTiming.u16HActive_Start)/2 + 2) : 0;

    return u16HStart;
}

MS_U16 MDrv_MVOP_EX_GetHStart(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }

    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    MS_U16 u16HStart = (pstDrvCtrl) ? ((pstDrvCtrl->_mvopTiming.u16HActive_Start)/2 + 2) : 0;
    return u16HStart;
}

//-----------------------------------------------------------------------------
/// Get distance from HSync to DE for Scaler, unit: mvop clock cycle
/// @return VStart
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_EX_GetVStart_V2(void* pInstance, MVOP_DevID eID)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }

    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    MVOP_Timing* pstTiming = NULL;
    MS_U16 u16Vst = 0;
    if (pstDrvCtrl)
    {
        pstTiming = &pstDrvCtrl->_mvopTiming;
        u16Vst = pstTiming->u16VBlank0_End - pstTiming->u16TopField_VS;
        if (pstTiming->bInterlace==1)
        {
            u16Vst*=2;
            u16Vst-=18;
        }
        else
        {
            u16Vst-=5;
        }
    }
    return u16Vst;
}

MS_U16 MDrv_MVOP_EX_GetVStart(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }

    MVOP_DrvCtrlSt* pstDrvCtrl = MVOP_GetDrvCtrl(eID);
    MVOP_Timing* pstTiming = NULL;
    MS_U16 u16Vst = 0;
    if (pstDrvCtrl)
    {
        pstTiming = &pstDrvCtrl->_mvopTiming;
        u16Vst = pstTiming->u16VBlank0_End - pstTiming->u16TopField_VS;
        if (pstTiming->bInterlace==1)
        {
            u16Vst*=2;
            u16Vst-=18;
        }
        else
        {
            u16Vst-=5;
        }
    }
    return u16Vst;
}

//-----------------------------------------------------------------------------
// Set MVOP clock enable
// @param bEnable \b IN
//   - # TRUE  Enable clock
//   - # FALSE Close clock
//-----------------------------------------------------------------------------
void MDrv_MVOP_EX_SetClk_V2(void* pInstance, MVOP_DevID eID,  MS_BOOL bEnable)
{
    HAL_MVOP_EX_SetDCClk(eID, 0, bEnable);  //Clk DC0
    HAL_MVOP_EX_SetDCClk(eID, 1, bEnable);  //Clk DC1
}

static void MDrv_MVOP_EX_SetClk(MVOP_DevID eID,  MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    HAL_MVOP_EX_SetDCClk(eID, 0, bEnable);  //Clk DC0
    HAL_MVOP_EX_SetDCClk(eID, 1, bEnable);  //Clk DC1
}

//-----------------------------------------------------------------------------
/// Set MVOP test pattern.
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetPattern_V2(void* pInstance, MVOP_DevID eID, MVOP_Pattern enMVOPPattern)
{
    static MVOP_Pattern s_enMVOPPattern = MVOP_PATTERN_NORMAL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    MVOP_EX_CheckIsInit(eID);
    if(enMVOPPattern == MVOP_PATTERN_DEFAULT)
    {
        enMVOPPattern = (MVOP_Pattern)(((MS_U32)(s_enMVOPPattern) + 1) % (MS_U32)MVOP_PATTERN_DEFAULT);
    }
    HAL_MVOP_EX_SetPattern(eID, enMVOPPattern);
    s_enMVOPPattern = enMVOPPattern;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_SetPattern(MVOP_DevID eID, MVOP_Pattern enMVOPPattern)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    static MVOP_Pattern s_enMVOPPattern = MVOP_PATTERN_NORMAL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    MVOP_EX_CheckIsInit(eID);
    if(enMVOPPattern == MVOP_PATTERN_DEFAULT)
    {
        enMVOPPattern = (MVOP_Pattern)(((int)(s_enMVOPPattern) + 1) % (int)MVOP_PATTERN_DEFAULT);
    }
    HAL_MVOP_EX_SetPattern(eID, enMVOPPattern);
    s_enMVOPPattern = enMVOPPattern;
    return E_MVOP_OK;
}

//-----------------------------------------------------------------------------
/// Configure the tile format of the MVOP input.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetTileFormat_V2(void* pInstance, MVOP_DevID eID, MVOP_TileFormat eTileFmt)
{
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    MVOP_EX_CheckIsInit(eID);
    if ((eTileFmt != E_MVOP_TILE_8x32) && (eTileFmt != E_MVOP_TILE_16x32))
    {
        return E_MVOP_INVALID_PARAM;
    }
    if (TRUE == HAL_MVOP_EX_SetTileFormat(eID, eTileFmt))
    {
        eRet = E_MVOP_OK;
    }
    return eRet;
}

MVOP_Result MDrv_MVOP_EX_SetTileFormat(MVOP_DevID eID, MVOP_TileFormat eTileFmt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    MVOP_EX_CheckIsInit(eID);
    if ((eTileFmt != E_MVOP_TILE_8x32) && (eTileFmt != E_MVOP_TILE_16x32))
    {
        return E_MVOP_INVALID_PARAM;
    }
    if (TRUE == HAL_MVOP_EX_SetTileFormat(eID, eTileFmt))
    {
        eRet = E_MVOP_OK;
    }
    return eRet;
}


//-----------------------------------------------------------------------------
/// Set MMIO Base for MVOP.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_EX_SetMMIOMapBase_V2(void* pInstance, MVOP_DevID eID)
{
    return MVOP_SetRegBaseAdd(HAL_MVOP_EX_RegSetBase);
}

MS_BOOL MDrv_MVOP_EX_SetMMIOMapBase(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    return MVOP_SetRegBaseAdd(HAL_MVOP_EX_RegSetBase);
}
//-----------------------------------------------------------------------------
/// Initialize MVOP hardware and set it to hardwire mode
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_Init_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32InitParam)
{

    if (eID >= HAL_MVOP_MODULE_CNT)
    {
        return E_MVOP_INVALID_PARAM;
    }

    MVOP_DrvInitCtxResults eRet;
    MS_BOOL pbFirstDrvInstant;
    eRet = _MVOP_Init_Context(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("MVOP_Init_Context FAILED!\n");)
        return E_MVOP_FAIL;
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
        if(MDrv_MVOP_SetMMIOMapBase() == FALSE)
        {
            return E_MVOP_FAIL;
        }
        HAL_MVOP_Init();
        return E_MVOP_OK;
    }

    _MVOP_InitVar(eID);

    if (MDrv_MVOP_EX_SetMMIOMapBase(eID) == FALSE)
    {
        return E_MVOP_FAIL;
    }

    if (HAL_MVOP_EX_Init)
    {
        if (HAL_MVOP_EX_Init(eID) == FALSE)
        {
            return E_MVOP_FAIL;
        }
    }
    else
    {
        return E_MVOP_FAIL;
    }

    HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);

    // Set fld inv & ofld_inv
    HAL_MVOP_EX_SetFieldInverse(eID, ENABLE, ENABLE);

    // Set Croma weighting off
    HAL_MVOP_EX_SetChromaWeighting(eID, ENABLE);
#if ENABLE_3D_LR_MODE
    HAL_MVOP_EX_Enable3DLR(eID, DISABLE);
#endif
    MDrv_MVOP_EX_SetClk(eID, TRUE);
    MDrv_MVOP_EX_EnableFreerunMode(eID, FALSE);
    HAL_MVOP_EX_EnableMVDInterface(eID, FALSE);
    HAL_MVOP_EX_LoadReg(eID);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsInit = TRUE;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_Init(MVOP_DevID eID, MS_U32 u32InitParam)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (eID >= HAL_MVOP_MODULE_CNT)
    {
        return E_MVOP_INVALID_PARAM;
    }

    MVOP_DrvInitCtxResults eRet;
    MS_BOOL pbFirstDrvInstant;
    eRet = _MVOP_Init_Context(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("MVOP_Init_Context FAILED!\n");)
        return E_MVOP_FAIL;
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
        if(MDrv_MVOP_SetMMIOMapBase() == FALSE)
        {
            return E_MVOP_FAIL;
        }
        HAL_MVOP_Init();
        return E_MVOP_OK;
    }

    _MVOP_InitVar(eID);

    if (MDrv_MVOP_EX_SetMMIOMapBase(eID) == FALSE)
    {
        return E_MVOP_FAIL;
    }

    if (HAL_MVOP_EX_Init)
    {
        if (HAL_MVOP_EX_Init(eID) == FALSE)
        {
            return E_MVOP_FAIL;
        }
    }
    else
    {
        return E_MVOP_FAIL;
    }

    HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);

    // Set fld inv & ofld_inv
    HAL_MVOP_EX_SetFieldInverse(eID, ENABLE, ENABLE);

    // Set Croma weighting off
    HAL_MVOP_EX_SetChromaWeighting(eID, ENABLE);
#if ENABLE_3D_LR_MODE
    HAL_MVOP_EX_Enable3DLR(eID, DISABLE);
#endif
    MDrv_MVOP_EX_SetClk(eID, TRUE);
    MDrv_MVOP_EX_EnableFreerunMode(eID, FALSE);
    HAL_MVOP_EX_EnableMVDInterface(eID, FALSE);
    HAL_MVOP_EX_LoadReg(eID);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsInit = TRUE;
    return E_MVOP_OK;
}

//-----------------------------------------------------------------------------
/// Exit MVOP, turn off clock
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_Exit_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32ExitParam)
{
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return eRet;
    }
    if (MVOP_EX_IsInit(eID))
    {
        MDrv_MVOP_EX_SetClk(eID, FALSE);
        eRet = E_MVOP_OK;
    }

    return eRet;
}

MVOP_Result MDrv_MVOP_EX_Exit(MVOP_DevID eID, MS_U32 u32ExitParam)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return eRet;
    }
    if (MVOP_EX_IsInit(eID))
    {
        MDrv_MVOP_EX_SetClk(eID, FALSE);
        eRet = E_MVOP_OK;
    }

    return eRet;
}

//-----------------------------------------------------------------------------
/// Enable and Disable MVOP
/// @param bEnable \b IN
///   - # TRUE  Enable
///   - # FALSE Disable and reset
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_Enable_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);

    HAL_MVOP_EX_SetMIUReqMask(eID, TRUE);
    HAL_MVOP_EX_Enable(eID, bEnable);
    HAL_MVOP_EX_SetMIUReqMask(eID, FALSE);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsEnable = bEnable;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_Enable(MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);

    HAL_MVOP_EX_SetMIUReqMask(eID, TRUE);
    HAL_MVOP_EX_Enable(eID, bEnable);
    HAL_MVOP_EX_SetMIUReqMask(eID, FALSE);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsEnable = bEnable;
    return E_MVOP_OK;
}


//-----------------------------------------------------------------------------
/// Get if MVOP is enable or not.
/// @param pbEnable \b OUT
///   - # TRUE  Enable
///   - # FALSE Disable
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_GetIsEnable_V2(void * pInstance, MVOP_DevID eID, MS_BOOL * pbEnable)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    if ((NULL == pbEnable) || (TRUE != MVOP_EX_IsInit(eID)))
    {
        return E_MVOP_FAIL;
    }

    *pbEnable = HAL_MVOP_EX_GetEnableState(eID);
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_GetIsEnable (MVOP_DevID eID, MS_BOOL* pbEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    if ((NULL == pbEnable) || (TRUE != MVOP_EX_IsInit(eID)))
    {
        return E_MVOP_FAIL;
    }

    *pbEnable = HAL_MVOP_EX_GetEnableState(eID);
    return E_MVOP_OK;
}

//-----------------------------------------------------------------------------
/// Set enable UVShift
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_EnableUVShift_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_EnableUVShift(eID, bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsUVShift = bEnable;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_EnableUVShift(MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_EnableUVShift(eID, bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsUVShift = bEnable;
    return E_MVOP_OK;
}

//-----------------------------------------------------------------------------
/// Set enable black background
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_EnableBlackBG_V2(void* pInstance, MVOP_DevID eID)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_SetBlackBG(eID);
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsBlackBG = TRUE;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_EnableBlackBG (MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_SetBlackBG(eID);
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsBlackBG = TRUE;
    return E_MVOP_OK;
}


//-----------------------------------------------------------------------------
/// MVOP mono mode (fix chroma value)
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetMonoMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_SetMonoMode(eID, bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsMonoMode = bEnable;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_SetMonoMode(MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);
    HAL_MVOP_EX_SetMonoMode(eID, bEnable);

    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._stMVOPDrvStat.bIsMonoMode = bEnable;
    return E_MVOP_OK;
}
//-----------------------------------------------------------------------------
/// Configure MVOP input.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetInputCfg_V2(void * pInstance, MVOP_DevID eID, MVOP_InputSel in, MVOP_InputCfg * pCfg)
{
    MVOP_Result bRet = E_MVOP_OK;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    switch (in)
    {
        case MVOP_INPUT_DRAM:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_MCUCTRL, pCfg);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_HVD_3DLR:
            HAL_MVOP_EX_Enable3DLR(eID, ENABLE);
            //no break here to continue setting HVD hardwire mode
#endif
        case MVOP_INPUT_H264:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetH264HardwireMode(eID);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_MVD_3DLR:
            HAL_MVOP_EX_Enable3DLR(eID, ENABLE);
            //no break here to continue setting MVD hardwire mode
#endif
        case MVOP_INPUT_MVD:
            HAL_MVOP_EX_EnableMVDInterface(eID, TRUE);
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            break;
        case MVOP_INPUT_RVD:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetRMHardwireMode(eID);
            break;
        case MVOP_INPUT_CLIP:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }
            HAL_MVOP_EX_SetCropWindow(eID, pCfg);
            break;
        case MVOP_INPUT_JPD:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetJpegHardwireMode(eID);
            break;
        default:
            bRet = E_MVOP_INVALID_PARAM;
            break;
    }

    return bRet;
}

MVOP_Result MDrv_MVOP_EX_SetInputCfg (MVOP_DevID eID, MVOP_InputSel in, MVOP_InputCfg* pCfg)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Result bRet = E_MVOP_OK;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    switch (in)
    {
        case MVOP_INPUT_DRAM:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_MCUCTRL, pCfg);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_HVD_3DLR:
            HAL_MVOP_EX_Enable3DLR(eID, ENABLE);
            //no break here to continue setting HVD hardwire mode
#endif
        case MVOP_INPUT_H264:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetH264HardwireMode(eID);
            break;

#if ENABLE_3D_LR_MODE
        case MVOP_INPUT_MVD_3DLR:
            HAL_MVOP_EX_Enable3DLR(eID, ENABLE);
            //no break here to continue setting MVD hardwire mode
#endif
        case MVOP_INPUT_MVD:
            HAL_MVOP_EX_EnableMVDInterface(eID, TRUE);
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            break;
        case MVOP_INPUT_RVD:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetRMHardwireMode(eID);
            break;
        case MVOP_INPUT_CLIP:
            if (!pCfg)
            {
                return E_MVOP_INVALID_PARAM;
            }
            HAL_MVOP_EX_SetCropWindow(eID, pCfg);
            break;
        case MVOP_INPUT_JPD:
            HAL_MVOP_EX_SetInputMode(eID, VOPINPUT_HARDWIRE, NULL);
            HAL_MVOP_EX_SetJpegHardwireMode(eID);
            break;
        default:
            bRet = E_MVOP_INVALID_PARAM;
            break;
    }

    return bRet;
}


//-----------------------------------------------------------------------------
/// Configure MVOP output.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetOutputCfg_V2(void* pInstance, MVOP_DevID eID, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    MVOP_Timing* pstTiming = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (MVOP_CheckOutputCfg(pstVideoStatus) != TRUE)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        return E_MVOP_INVALID_PARAM;
    }

    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._mvopTiming;
    MS_BOOL b3DTB = FALSE;
    EN_MVOP_Output_3D_TYPE en3DType = E_MVOP_OUTPUT_3D_NONE;
#if ENABLE_3D_LR_MODE
    if(HAL_MVOP_EX_GetOutput3DType)
    {
        b3DTB = (HAL_MVOP_EX_GetOutput3DType(eID) == E_MVOP_OUTPUT_3D_TB);
        en3DType = HAL_MVOP_EX_GetOutput3DType(eID);
    }
#endif
    MS_U32 MVOP_Clock = MVOP_CalculatePixClk(eID, pstTiming, pstVideoStatus, bEnHDup, en3DType);
    if (0 == MVOP_Clock)
    {
        MVOP_DBG(MVOP_PRINTF("%s: MVOP_Clock==0\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    HALMVOPFREQUENCY eClkFreq = HAL_MVOP_EX_GetClk(MVOP_Clock);
    MVOP_EX_AutoGenTiming(eID, pstTiming, pstVideoStatus, eClkFreq, b3DTB);

    HAL_MVOP_EX_SetOutputInterlace(eID, pstTiming->bInterlace);
    HAL_MVOP_EX_SetOutputTiming(eID, pstTiming);
    HAL_MVOP_EX_SetSynClk(eID, pstTiming, eClkFreq);

    MS_U16 u16SrcHSize = p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
    if (u16SrcHSize != 0)
    {
        //report the source width for scaler, intead of the 16-aligned width
        //MS_U16 u16Discard_Width = 0;
        //u16Discard_Width = _u16SubSrcHSize.u16Width - _u16SubSrcHSize;
        MVOP_DBG(MVOP_PRINTF("%s: _mvopTiming.u16Width: %d ==> %d\n", __FUNCTION__,
                             pstTiming->u16Width, u16SrcHSize);)
        pstTiming->u16Width = u16SrcHSize;
#if 0   //for H-mirror ON
        if (_mvopTiming.u16HActive_Start > u16Discard_Width)
        {
            _mvopTiming.u16HActive_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HActive_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HActive_Start, u16Discard_Width);
        }
        if (_mvopTiming.u16HImg_Start > u16Discard_Width)
        {
            _mvopTiming.u16HImg_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HImg_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HImg_Start, u16Discard_Width);
        }
#endif
    }
    MVOP_DumpOutputTiming(pstTiming);
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_SetOutputCfg(MVOP_DevID eID, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Timing* pstTiming = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (MVOP_CheckOutputCfg(pstVideoStatus) != TRUE)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        return E_MVOP_INVALID_PARAM;
    }

    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._mvopTiming;
    MS_BOOL b3DTB = FALSE;
    EN_MVOP_Output_3D_TYPE en3DType = E_MVOP_OUTPUT_3D_NONE;
#if ENABLE_3D_LR_MODE
    if(HAL_MVOP_EX_GetOutput3DType)
    {
        b3DTB = (HAL_MVOP_EX_GetOutput3DType(eID) == E_MVOP_OUTPUT_3D_TB);
        en3DType = HAL_MVOP_EX_GetOutput3DType(eID);
    }
#endif
    MS_U32 MVOP_Clock = MVOP_CalculatePixClk(eID, pstTiming, pstVideoStatus, bEnHDup, en3DType);
    if (0 == MVOP_Clock)
    {
        MVOP_DBG(MVOP_PRINTF("%s: MVOP_Clock==0\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    HALMVOPFREQUENCY eClkFreq = HAL_MVOP_EX_GetClk(MVOP_Clock);
    MVOP_EX_AutoGenTiming(eID, pstTiming, pstVideoStatus, eClkFreq, b3DTB);

    HAL_MVOP_EX_SetOutputInterlace(eID, pstTiming->bInterlace);
    HAL_MVOP_EX_SetOutputTiming(eID, pstTiming);
    HAL_MVOP_EX_SetSynClk(eID, pstTiming, eClkFreq);

    MS_U16 u16SrcHSize = p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16SrcHSize;
    if (u16SrcHSize != 0)
    {
        //report the source width for scaler, intead of the 16-aligned width
        //MS_U16 u16Discard_Width = 0;
        //u16Discard_Width = _u16SubSrcHSize.u16Width - _u16SubSrcHSize;
        MVOP_DBG(MVOP_PRINTF("%s: _mvopTiming.u16Width: %d ==> %d\n", __FUNCTION__,
                             pstTiming->u16Width, u16SrcHSize);)
        pstTiming->u16Width = u16SrcHSize;
#if 0   //for H-mirror ON
        if (_mvopTiming.u16HActive_Start > u16Discard_Width)
        {
            _mvopTiming.u16HActive_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HActive_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HActive_Start, u16Discard_Width);
        }
        if (_mvopTiming.u16HImg_Start > u16Discard_Width)
        {
            _mvopTiming.u16HImg_Start -= u16Discard_Width;
        }
        else
        {
            MVOP_PRINTF("Warning: u16HImg_Start(%d) <= u16Discard_Width(%d)\n",
                        _mvopTiming.u16HImg_Start, u16Discard_Width);
        }
#endif
    }
    MVOP_DumpOutputTiming(pstTiming);
    return E_MVOP_OK;
}


//-----------------------------------------------------------------------------
/// Get MVOP output timing information.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_GetOutputTiming_V2(void* pInstance, MVOP_DevID eID, MVOP_Timing* pMVOPTiming)
{
    MVOP_Timing* pstTiming = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!pMVOPTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        return E_MVOP_INVALID_PARAM;
    }
    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._mvopTiming;
    memcpy(pMVOPTiming, pstTiming, sizeof(MVOP_Timing));
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_GetOutputTiming(MVOP_DevID eID, MVOP_Timing* pMVOPTiming)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Timing* pstTiming = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    if (!pMVOPTiming)
    {
        MVOP_DBG(MVOP_PRINTF("%s: invalid parameter!\n", __FUNCTION__);)
        return E_MVOP_INVALID_PARAM;
    }
    pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._mvopTiming;
    memcpy(pMVOPTiming, pstTiming, sizeof(MVOP_Timing));
    return E_MVOP_OK;
}

//-----------------------------------------------------------------------------
/// Get Horizontal Size.
/// @return Horizontal Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_EX_GetHSize_V2(void* pInstance, MVOP_DevID eID)
{
    MS_U16 u16HSize = 0;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        u16HSize = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.u16Width) : 0;
    }
    return u16HSize;
}

MS_U16 MDrv_MVOP_EX_GetHSize(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16HSize = 0;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        u16HSize = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.u16Width) : 0;
    }
    return u16HSize;
}

//-----------------------------------------------------------------------------
/// Get Vertical Size.
/// @return Vertical Size
//-----------------------------------------------------------------------------
MS_U16 MDrv_MVOP_EX_GetVSize_V2(void* pInstance, MVOP_DevID eID)
{
    MS_U16 u16VSize = 0;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        u16VSize = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.u16Height) : 0;
    }
    return u16VSize;
}

MS_U16 MDrv_MVOP_EX_GetVSize(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16VSize = 0;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return 0;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        u16VSize = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.u16Height) : 0;
    }
    return u16VSize;
}

//-----------------------------------------------------------------------------
/// Get MVOP timing is interlace or progressive.
/// @return TRUE for interlace; FALSE for progressive
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_EX_GetIsInterlace_V2(void* pInstance, MVOP_DevID eID)
{
    MS_BOOL bInterlaced = FALSE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        bInterlaced = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.bInterlace) : FALSE;
    }
    return bInterlaced;
}

MS_BOOL MDrv_MVOP_EX_GetIsInterlace(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_BOOL bInterlaced = FALSE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        bInterlaced = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.bInterlace) : FALSE;
    }
    return bInterlaced;
}

//-----------------------------------------------------------------------------
/// Get MVOP timing is horizontal duplicated or not.
/// @return TRUE for yes; FALSE for not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_EX_GetIsHDuplicate_V2(void* pInstance, MVOP_DevID eID)
{
    MS_BOOL bHDuplicated = FALSE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        bHDuplicated = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.bHDuplicate) : FALSE;
    }
    return bHDuplicated;
}

MS_BOOL MDrv_MVOP_EX_GetIsHDuplicate(MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_BOOL bHDuplicated = FALSE;
    MVOP_DrvCtrlSt* pstDrvCtrl = NULL;

    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    if (MVOP_EX_IsInit(eID))
    {
        pstDrvCtrl = MVOP_GetDrvCtrl(eID);
        bHDuplicated = (pstDrvCtrl) ? (pstDrvCtrl->_mvopTiming.bHDuplicate) : FALSE;
    }
    return bHDuplicated;
}

/******************************************************************************************/
/// This function is used for dectection of MVOP's Capability
/// @param u16HSize    \b IN
/// @param u16VSize    \b IN
/// @param u16Fps      \b IN
/// @return TRUE if MVOP's frequency can support this video source, otherwise return FALSE
/******************************************************************************************/
#if defined (__aarch64__)
MS_BOOL MDrv_MVOP_EX_CheckCapability_V2(void* pInstance, MVOP_DevID eID, MS_U64 u32InParam)
#else
MS_BOOL MDrv_MVOP_EX_CheckCapability_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32InParam)
#endif
{
    MS_U16 u16HSize = 0;
    MS_U16 u16VSize = 0;
    MS_U16 u16Fps = 0;
    MS_BOOL bRet = FALSE;
    if ((u32InParam != 0) && (((MVOP_CapInput*)u32InParam)->u8StrVer == 0))
    {
        MVOP_CapInput* pstInput = (MVOP_CapInput*)u32InParam;
        u16HSize = pstInput->u16HSize;
        u16VSize = pstInput->u16VSize;
        u16Fps = pstInput->u16Fps;
        bRet = ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_EX_GetMaxFreerunClk(eID)));
    }
    return bRet;
}

#if defined (__aarch64__)
MS_BOOL MDrv_MVOP_EX_CheckCapability(MVOP_DevID eID, MS_U64 u32InParam)
#else
MS_BOOL MDrv_MVOP_EX_CheckCapability(MVOP_DevID eID, MS_U32 u32InParam)
#endif
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16HSize = 0;
    MS_U16 u16VSize = 0;
    MS_U16 u16Fps = 0;
    MS_BOOL bRet = FALSE;
    if ((u32InParam != 0) && (((MVOP_CapInput*)u32InParam)->u8StrVer == 0))
    {
        MVOP_CapInput* pstInput = (MVOP_CapInput*)u32InParam;
        u16HSize = pstInput->u16HSize;
        u16VSize = pstInput->u16VSize;
        u16Fps = pstInput->u16Fps;
        bRet = ((MS_U32)((u16HSize + MVOP_HBlank_Min) * (u16VSize + MVOP_VBlank_Min)* u16Fps) <= (MS_U32)(HAL_MVOP_EX_GetMaxFreerunClk(eID)));
    }
    return bRet;
}

MS_U16 MDrv_MVOP_EX_GetMaxVOffset_V2(void* pInstance, MVOP_DevID eID, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MS_U16 u16OffsetMax = 0;

    if (HAL_MVOP_EX_GetMaxFreerunClk)
    {
        u16OffsetMax = MVOP_GetMaxVOffset(eID, HAL_MVOP_EX_GetMaxFreerunClk(eID), u16HSize, u16VSize, u16Fps);
    }

    return u16OffsetMax;
}

MS_U16 MDrv_MVOP_EX_GetMaxVOffset(MVOP_DevID eID, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U16 u16OffsetMax = 0;

    if (HAL_MVOP_EX_GetMaxFreerunClk)
    {
        u16OffsetMax = MVOP_GetMaxVOffset(eID, HAL_MVOP_EX_GetMaxFreerunClk(eID), u16HSize, u16VSize, u16Fps);
    }

    return u16OffsetMax;
}

//------------------------------------------------------------------------------
/// MVOP Get Destination Information for GOP mixer
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVOP_EX_GetDstInfo_V2(void* pInstance, MVOP_DevID eID, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return FALSE;
    }

    if(NULL == pDstInfo)
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():pDstInfo is NULL\n");)
        return FALSE;
    }

    if(u32SizeofDstInfo != sizeof(MVOP_DST_DispInfo))
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():u16SizeofDstInfo is different from the MVOP_DST_DispInfo defined, check header file!\n");)
        return FALSE;
    }

    MVOP_TimingInfo_FromRegisters mvopTimingInfo;
    memset(&mvopTimingInfo, 0, sizeof(MVOP_TimingInfo_FromRegisters));
    if(HAL_MVOP_EX_GetTimingInfoFromRegisters(eID, &mvopTimingInfo) == FALSE)
    {
        MVOP_ERR(MVOP_PRINTF("encounter errors in HAL_MVOP_EX_GetTimingInfoFromRegisters()!\n");)
        memset(pDstInfo, 0, u32SizeofDstInfo);
        return FALSE;
    }
    pDstInfo->VDTOT = mvopTimingInfo.u16V_TotalCount;
    pDstInfo->HDTOT = mvopTimingInfo.u16H_TotalCount;
    pDstInfo->bInterlaceMode = mvopTimingInfo.bInterlace;
    pDstInfo->DEHST = mvopTimingInfo.u16HActive_Start / 2 + 2;
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) * 2 - 18;
    }
    else
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) - 5;
    }
    //HDeEnd=HDeStart+HDeWidth
    pDstInfo->DEHEND = pDstInfo->DEHST + pDstInfo->HDTOT - mvopTimingInfo.u16HActive_Start;
    //VDeEnd=VDeStart+VDeWidth
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (2 * mvopTimingInfo.u16VBlank0_End - 3);
    }
    else
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (mvopTimingInfo.u16VBlank0_End - 1);
    }
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, pDstInfo->HDTOT, pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", pDstInfo->DEHST, pDstInfo->DEHEND, pDstInfo->DEVST, pDstInfo->DEVEND);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, (int)pDstInfo->HDTOT, (int)pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", (int)pDstInfo->DEHST, (int)pDstInfo->DEHEND, (int)pDstInfo->DEVST, (int)pDstInfo->DEVEND);)
#endif
    return TRUE;
}

MS_BOOL MDrv_MVOP_EX_GetDstInfo(MVOP_DevID eID, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s: driver is not initialized!\n", __FUNCTION__);)
        return FALSE;
    }

    if(NULL == pDstInfo)
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():pDstInfo is NULL\n");)
        return FALSE;
    }

    if(u32SizeofDstInfo != sizeof(MVOP_DST_DispInfo))
    {
        MVOP_ERR(MVOP_PRINTF("MApi_MVOP_GetDstInfo():u16SizeofDstInfo is different from the MVOP_DST_DispInfo defined, check header file!\n");)
        return FALSE;
    }

    MVOP_TimingInfo_FromRegisters mvopTimingInfo;
    memset(&mvopTimingInfo, 0, sizeof(MVOP_TimingInfo_FromRegisters));
    if(HAL_MVOP_EX_GetTimingInfoFromRegisters(eID, &mvopTimingInfo) == FALSE)
    {
        MVOP_ERR(MVOP_PRINTF("encounter errors in HAL_MVOP_EX_GetTimingInfoFromRegisters()!\n");)
        memset(pDstInfo, 0, u32SizeofDstInfo);
        return FALSE;
    }
    pDstInfo->VDTOT = mvopTimingInfo.u16V_TotalCount;
    pDstInfo->HDTOT = mvopTimingInfo.u16H_TotalCount;
    pDstInfo->bInterlaceMode = mvopTimingInfo.bInterlace;
    pDstInfo->DEHST = mvopTimingInfo.u16HActive_Start / 2 + 2;
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) * 2 - 18;
    }
    else
    {
        pDstInfo->DEVST = (mvopTimingInfo.u16VBlank0_End - mvopTimingInfo.u16TopField_VS) - 5;
    }
    //HDeEnd=HDeStart+HDeWidth
    pDstInfo->DEHEND = pDstInfo->DEHST + pDstInfo->HDTOT - mvopTimingInfo.u16HActive_Start;
    //VDeEnd=VDeStart+VDeWidth
    if(pDstInfo->bInterlaceMode)
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (2 * mvopTimingInfo.u16VBlank0_End - 3);
    }
    else
    {
        pDstInfo->DEVEND = pDstInfo->DEVST + pDstInfo->VDTOT - (mvopTimingInfo.u16VBlank0_End - 1);
    }
#if defined (__aarch64__)
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, pDstInfo->HDTOT, pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", pDstInfo->DEHST, pDstInfo->DEHEND, pDstInfo->DEVST, pDstInfo->DEVEND);)
#else
    MVOP_TIMING_DBG(MVOP_PRINTF("bInterlace=%u,HTotal=%u,VTotal=%u\n", pDstInfo->bInterlaceMode, (int)pDstInfo->HDTOT, (int)pDstInfo->VDTOT);)
    MVOP_TIMING_DBG(MVOP_PRINTF("HDeStart=%u,HDeEnd=%u,VDeStart=%u,VDeEnd=%u\n", (int)pDstInfo->DEHST, (int)pDstInfo->DEHEND, (int)pDstInfo->DEVST, (int)pDstInfo->DEVEND);)
#endif
    return TRUE;
}

MVOP_Result MDrv_MVOP_EX_SetFixVtt_V2(void* pInstance, MVOP_DevID eID, MS_U16 u16FixVtt)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt = u16FixVtt;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_SetFixVtt(MVOP_DevID eID, MS_U16 u16FixVtt)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._u16FixVtt = u16FixVtt;
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_EnableFreerunMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    eRet = HAL_MVOP_EX_EnableFreerunMode(eID, bEnable);
    if (E_MVOP_OK == eRet)
    {
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._bEnableFreerunMode = bEnable;
    }
    return eRet;
}

MVOP_Result MDrv_MVOP_EX_EnableFreerunMode(MVOP_DevID eID, MS_BOOL bEnable)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MVOP_Result eRet = E_MVOP_FAIL;
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }

    eRet = HAL_MVOP_EX_EnableFreerunMode(eID, bEnable);
    if (E_MVOP_OK == eRet)
    {
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[eID]._bEnableFreerunMode = bEnable;
    }
    return eRet;
}

MVOP_Result MDrv_MVOP_EX_MiuSwitch_V2(void* pInstance, MVOP_DevID eID, MS_U8 u8Miu)
{
    //Agate Sub and 3rd MVOP share one miu client.
    return MVOP_MiuSwitch(FALSE, u8Miu);
}

MVOP_Result MDrv_MVOP_EX_MiuSwitch(MVOP_DevID eID, MS_U8 u8Miu)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    //Agate Sub and 3rd MVOP share one miu client.
    return MVOP_MiuSwitch(FALSE, u8Miu);
}
//------------------------------------------------------------------------------
/// MVOP Enable/Disable mirror mode (chips after M10 support this feature)
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MVOP_Result MDrv_MVOP_EX_SetVOPMirrorMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode)
{
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);

    if (HAL_MVOP_EX_SetVerticalMirrorMode && HAL_MVOP_EX_SetHorizontallMirrorMode)
    {
        if( eMirrorMode == E_VOPMIRROR_VERTICAL)
        {
            HAL_MVOP_EX_SetVerticalMirrorMode(eID, bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HORIZONTALL)
        {
            HAL_MVOP_EX_SetHorizontallMirrorMode(eID, bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HVBOTH)
        {
            HAL_MVOP_EX_SetVerticalMirrorMode(eID, bEnable);
            HAL_MVOP_EX_SetHorizontallMirrorMode(eID, bEnable);
        }
    }

    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_EX_SetVOPMirrorMode (eMirrorMode :0x%x , 0x%x) \n",eMirrorMode,bEnable);)
    return E_MVOP_OK;
}

MVOP_Result MDrv_MVOP_EX_SetVOPMirrorMode(MVOP_DevID eID, MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if (p_gDrvMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return E_MVOP_FAIL;
    }
    MVOP_EX_CheckIsInit(eID);

    if (HAL_MVOP_EX_SetVerticalMirrorMode && HAL_MVOP_EX_SetHorizontallMirrorMode)
    {
        if( eMirrorMode == E_VOPMIRROR_VERTICAL)
        {
            HAL_MVOP_EX_SetVerticalMirrorMode(eID, bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HORIZONTALL)
        {
            HAL_MVOP_EX_SetHorizontallMirrorMode(eID, bEnable);
        }
        else if(eMirrorMode == E_VOPMIRROR_HVBOTH)
        {
            HAL_MVOP_EX_SetVerticalMirrorMode(eID, bEnable);
            HAL_MVOP_EX_SetHorizontallMirrorMode(eID, bEnable);
        }
    }

    MVOP_DBG(MVOP_PRINTF("\nMVOP MDrv_MVOP_EX_SetVOPMirrorMode (eMirrorMode :0x%x , 0x%x) \n",eMirrorMode,bEnable);)
    return E_MVOP_OK;
}

// Interrupt functions
//MS_BOOL MDrv_MVOP_EnableInterrupt(MVOP_IntType eIntType)
MS_BOOL MDrv_MVOP_EnableInterrupt_V2(void* pInstance, MS_U8 eIntType)
{
    MS_BOOL bRet = FALSE;
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return bRet;
    }
    if (HAL_MVOP_IntEnableMask)
    {
        bRet = HAL_MVOP_IntEnableMask(E_MVOP_DEV_0, eIntType);
    }

    return bRet;
}

MS_BOOL MDrv_MVOP_EnableInterrupt(MS_U8 eIntType)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_ENABLE_INTERRUPT IO_arg;
    IO_arg.eIntType = eIntType;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_ENABLE_INTERRUPT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_EnableInterrupt_V2(pInstance, eIntType));
#endif
    return IO_arg.bRet;
}

MS_BOOL MDrv_MVOP_ClearInterrupt(MS_BOOL bEnable)
{
    MS_U8 eIntType;
    MS_BOOL bRet = FALSE;
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    if(bEnable)
    {
        eIntType = E_MVOP_INT_NONE;
    }
    else
    {
        eIntType = E_MVOP_INT_VSYNC;
    }
    bRet = HAL_MVOP_IntEnableMask(E_MVOP_DEV_0, eIntType);
    return bRet;
}

MS_U8 MDrv_MVOP_GetIntStatus_V2(void* pInstance)
{
    MS_U8 u8IntSt = 0;
    if (!MVOP_IsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return u8IntSt;
    }
    if (HAL_MVOP_IntGetStatus)
    {
        u8IntSt = HAL_MVOP_IntGetStatus(E_MVOP_DEV_0);
    }
    return u8IntSt;
}

MS_U8 MDrv_MVOP_GetIntStatus(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_GET_INT_STATUS IO_arg;
    IO_arg.u8Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u8Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_GET_INT_STATUS, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u8Ret;
    }

    return IO_arg.u8Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_GetIntStatus_V2(pInstance));
#endif
}

MS_BOOL MDrv_MVOP_SubEnableInterrupt_V2(void* pInstance, MVOP_IntType eIntType)
{
    MS_BOOL bRet = FALSE;
#if MVOP_SUPPORT_SUB
    if (!MVOP_SubIsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return bRet;
    }
    if (HAL_MVOP_IntEnableMask)
    {
        bRet = HAL_MVOP_IntEnableMask(E_MVOP_DEV_1, eIntType);
    }
#endif
    return bRet;
}

MS_BOOL MDrv_MVOP_SubEnableInterrupt(MVOP_IntType eIntType)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d, arg:%d \n", __FUNCTION__, __LINE__, eIntType);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_ENABLE_INTERRUPT IO_arg;
    IO_arg.eIntType = eIntType;
    IO_arg.bRet = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_ENABLE_INTERRUPT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.bRet;
    }

    return IO_arg.bRet;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubEnableInterrupt_V2(pInstance, eIntType));
#endif
}

MS_U8 MDrv_MVOP_SubGetIntStatus_V2(void* pInstance)
{
    MS_U8 u8IntSt = 0;
#if MVOP_SUPPORT_SUB
    if (!MVOP_SubIsInit())
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return u8IntSt;
    }
    if (HAL_MVOP_IntGetStatus)
    {
        u8IntSt = HAL_MVOP_IntGetStatus(E_MVOP_DEV_1);
    }
#endif
    return u8IntSt;
}

MS_U8 MDrv_MVOP_SubGetIntStatus(void)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SUB_GET_INT_STATUS IO_arg;
    IO_arg.u8Ret = 0;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u8Ret;
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SUB_GET_INT_STATUS, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return IO_arg.u8Ret;
    }

    return IO_arg.u8Ret;
#else
    void* pInstance = NULL;
    return (MDrv_MVOP_SubGetIntStatus_V2(pInstance));
#endif
}


MS_BOOL MDrv_MVOP_EX_EnableInterrupt_V2(void* pInstance, MVOP_DevID eID, MVOP_IntType eIntType)
{
    MS_BOOL bRet = FALSE;
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return bRet;
    }
    if (HAL_MVOP_IntEnableMask)
    {
        bRet = HAL_MVOP_IntEnableMask(eID, eIntType);
    }

    return bRet;
}

MS_BOOL MDrv_MVOP_EX_EnableInterrupt(MVOP_DevID eID, MVOP_IntType eIntType)
{
    MS_BOOL bRet = FALSE;
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return bRet;
    }
    if (HAL_MVOP_IntEnableMask)
    {
        bRet = HAL_MVOP_IntEnableMask(eID, eIntType);
    }

    return bRet;
}

MS_U8 MDrv_MVOP_EX_GetIntStatus_V2(void* pInstance, MVOP_DevID eID)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
    MS_U8 u8IntSt = 0;
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return u8IntSt;
    }
    if (HAL_MVOP_IntGetStatus)
    {
        u8IntSt = HAL_MVOP_IntGetStatus(eID);
    }
    return u8IntSt;
}

MS_U8 MDrv_MVOP_EX_GetIntStatus(MVOP_DevID eID)
{
    MS_U8 u8IntSt = 0;
    if (!MVOP_EX_IsInit(eID))
    {
        MVOP_DBG(MVOP_PRINTF("%s:MVOP driver is not initialized!\n", __FUNCTION__);)
        return u8IntSt;
    }
    if (HAL_MVOP_IntGetStatus)
    {
        u8IntSt = HAL_MVOP_IntGetStatus(eID);
    }
    return u8IntSt;
}





//#define MVOP_BANK_REGNUM 0x80UL

MS_BOOL _MDrv_MVOP_Power_Mode(MVOP_DevID eID,EN_MVOP_POWER_MODE ePowerMode)
{
    MS_BOOL bRet = TRUE;
    //STR restored
    MS_U8 i=0UL;
#if 0
    MVOP_STR_SAVE_AREA* pMVOP_STRPrivate;
    void* pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pMVOP_STRPrivate);
#endif


    if(ePowerMode == E_MVOP_POWER_SUSPEND)
    {
        for(i =0; i<MVOP_BANK_REGNUM; i++ )
        {
            p_gDrvMVOPCtx->pMVOP_STRPrivate.MVOP_BANK[eID][i] = HAL_MVOP_ReadBank(eID,i);
        }
        for(i =0; i<MVOP_CLK_REGNUM; i++ )
        {
            p_gDrvMVOPCtx->pMVOP_STRPrivate.CLK_BANK[i] = HAL_MVOP_ReadClkBank(i);
        }

    }
    else if(ePowerMode == E_MVOP_POWER_RESUME)
    {
        for(i =0; i<MVOP_BANK_REGNUM; i++ )
        {
            HAL_MVOP_WriteBank(eID,i,p_gDrvMVOPCtx->pMVOP_STRPrivate.MVOP_BANK[eID][i]);
        }
        for(i =0; i<MVOP_CLK_REGNUM; i++ )
        {
            HAL_MVOP_WriteClkBank(i,p_gDrvMVOPCtx->pMVOP_STRPrivate.CLK_BANK[i]);
        }
    }
    else if(ePowerMode == E_MVOP_POWER_MECHANICAL)
    {

    }
    else if(ePowerMode == E_MVOP_POWER_SOFT_OFF)
    {

    }
    return bRet;
}


//----------------------------------------------------------------------
///  For Kernel mode suspend and resume
///
///
//----------------------------------------------------------------------
MS_U32 MDrv_MVOP_SetPowerState_Kernel_V2(void* pInstance, EN_POWER_MODE u16PowerState)
{
    //static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U16 u16Return = UTOPIA_STATUS_FAIL;
    MS_BOOL bRet = TRUE;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        MDrv_MVOP_Init();

        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("[kernel]%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return u16Return;
        }
        //check main/sub mvop be used or not

        if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit && bRet)
        {
            bRet =_MDrv_MVOP_Power_Mode(E_MVOP_DEV_0,E_MVOP_POWER_SUSPEND);
        }
#if MVOP_SUPPORT_SUB
        MDrv_MVOP_SubInit();

        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return u16Return;
        }
        if(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit && bRet)
        {
            bRet =_MDrv_MVOP_Power_Mode(E_MVOP_DEV_1,E_MVOP_POWER_SUSPEND);
        }
#endif
    }
    else if(u16PowerState == E_POWER_RESUME)
    {
        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return u16Return;
        }

        if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit && bRet)
        {
            bRet =_MDrv_MVOP_Power_Mode(E_MVOP_DEV_0,E_POWER_RESUME);
        }
#if MVOP_SUPPORT_SUB
        if (p_gDrvSubMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return u16Return;
        }

        if(p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit && bRet)
        {
            bRet =_MDrv_MVOP_Power_Mode(E_MVOP_DEV_1,E_POWER_RESUME);
        }
#endif
    }

    if(bRet)
    {
        u16Return = UTOPIA_STATUS_SUCCESS;
    }

    return u16Return;
}


//-----------------------------------------------------------------------------
/// For HK power suspend and resume.
/// MVOP keeps the status as suspend for staying the same status while HK resume.
/// @return 1 for RESUME_OK; 2 for SUSPEND_OK; 3 for SUSPEND_FAILED.
//-----------------------------------------------------------------------------

MS_U32 MDrv_MVOP_SetPowerState_V2(void* pInstance, EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U16 u16Return = UTOPIA_STATUS_FAIL;
#if 0
    MVOP_Timing* pstTiming = NULL;
    MVOP_InputCfg* InputCfg = NULL;
    MVOP_InputSel  InputSel;
    MS_U16 u16ECOVersion;
#endif
    MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] u16PowerState = %d\n",u16PowerState);)

    if (u16PowerState == E_POWER_SUSPEND)
    {
        //MDrv_MVOP_Init();
        MDrv_MVOP_Init_V2(pInstance);

        if (p_gDrvMVOPCtx == NULL)
        {
            MVOP_PRINTF("%s p_gDrvMVOPCtx is NULL pointer\n", __FUNCTION__);
            return u16Return;
        }

        _prev_u16PowerState = u16PowerState;
        //--------------------
        // Saving YUV address
        //--------------------
        if(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eInputSel == MVOP_INPUT_DRAM )
        {
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg.u32YOffset = HAL_MVOP_GetYBaseAdd();
            p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg.u32UVOffset = HAL_MVOP_GetUVBaseAdd();
        }
        p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit = FALSE;
        HAL_MVOP_PowerStateSuspend();

        u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            //MDrv_MVOP_Init();
            MDrv_MVOP_Init_V2(pInstance);
#if 0
            pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;
            InputCfg = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg;
            u16ECOVersion = (MS_U16)MDrv_SYS_GetChipRev();

            //---------------
            // Set InputCfg
            //---------------
            InputSel = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eInputSel;
            switch(InputSel)
            {
#if ENABLE_3D_LR_MODE
                case MVOP_INPUT_DRAM_3DLR:
                    HAL_MVOP_Enable3DLR(ENABLE);
                    //no break here to continue setting MCU mode
#endif
                case MVOP_INPUT_DRAM:
                    if (!InputCfg)
                    {
                        return FALSE;
                    }

                    if (E_MVOP_RGB_NONE != p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eRgbFmt)
                    {
                        if (HAL_MVOP_SetRgbFormat)
                        {
                            HAL_MVOP_SetRgbFormat(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eRgbFmt);
                        }
                        else
                        {
                            MVOP_ERR(MVOP_PRINTF("%s not support HAL_MVOP_SetRgbFormat\n", __FUNCTION__);)
                        }
                    }

                    HAL_MVOP_SetInputMode(VOPINPUT_MCUCTRL, InputCfg, u16ECOVersion);
                    break;

#if ENABLE_3D_LR_MODE
                case MVOP_INPUT_HVD_3DLR:
                    HAL_MVOP_Enable3DLR(ENABLE);
                    //no break here to continue setting HVD hardwire mode
#endif
                case MVOP_INPUT_H264:
                    HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
                    HAL_MVOP_SetH264HardwireMode();
                    break;

#if ENABLE_3D_LR_MODE
                case MVOP_INPUT_MVD_3DLR:
                    HAL_MVOP_Enable3DLR(ENABLE);
                    //no break here to continue setting MVD hardwire mode
#endif
                case MVOP_INPUT_MVD:
                    HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
                    break;
                case MVOP_INPUT_RVD:
                    HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
                    HAL_MVOP_SetRMHardwireMode();
                    break;
                case MVOP_INPUT_CLIP:
                    if (!InputCfg)
                    {
                        _DRV_MVOP_Return(E_MVOP_INVALID_PARAM);
                    }
                    HAL_MVOP_SetCropWindow(InputCfg);
                    break;
                case MVOP_INPUT_JPD:
                    HAL_MVOP_SetInputMode(VOPINPUT_HARDWIRE, NULL, u16ECOVersion);
                    HAL_MVOP_SetJpegHardwireMode();
                    break;
                default:
                    u16Return = E_MVOP_INVALID_PARAM;
                    break;
            }
            //---------------
            // Set OutputCfg
            //---------------
            HAL_MVOP_SetMIUReqMask(TRUE);
            HAL_MVOP_Enable(FALSE);
            HAL_MVOP_SetMIUReqMask(FALSE);

            HAL_MVOP_SetOutputInterlace(pstTiming->bInterlace);
            HAL_MVOP_SetOutputTiming(pstTiming);
            HAL_MVOP_SetSynClk(pstTiming);

            HAL_MVOP_SetMIUReqMask(TRUE);
            HAL_MVOP_Enable(TRUE);
            HAL_MVOP_SetMIUReqMask(FALSE);

            //-----------------
            // Restore Command
            //-----------------
            HAL_MVOP_Set3DLRAltOutput_VHalfScaling(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnable3DLRALT);
            HAL_MVOP_Set3DLRAltOutput(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnable3DLRSBS);
            HAL_MVOP_Set3DLRAltSBSOutput(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnable3DLRSBS);
            HAL_MVOP_LoadReg();

            HAL_MVOP_SetVerDup(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVdup);
            HAL_MVOP_SetVSyncMode(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVsyncMode);
            HAL_MVOP_SetVerx4Dup(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVx4);
            HAL_MVOP_SetHorx4Dup(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableHx4);

            HAL_MVOP_SetVerticalMirrorMode(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableVMirror);
            HAL_MVOP_SetHorizontallMirrorMode(p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopComdFlag.bEnableHMirror);

            _prev_u16PowerState = u16PowerState;
            u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
#endif
        }
        else
        {
            MVOP_PRINTF("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }

    }
    else
    {
        MVOP_PRINTF("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
    }

    return u16Return;// for success
}

MS_U32 MDrv_MVOP_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MVOP_DBG_TRACE(MVOP_PRINTF("[TRACE] %s, line:%d \n", __FUNCTION__, __LINE__);)
#if ENABLE_UTOPIA2_INTERFACE
    stMVOP_SET_POWER_STATE IO_arg;
    IO_arg.u16PowerState = u16PowerState;
    IO_arg.u32Ret = FALSE;

    if (pu32MVOP_Inst[E_MVOP_MAIN_STREAM] == NULL)
    {
        if(UtopiaOpen(MODULE_MVOP|MVOP_DRIVER_BASE, &pu32MVOP_Inst[E_MVOP_MAIN_STREAM], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MVOP_PRINTF("[Fail] UtopiaOpen MVOP failed\n");
            return IO_arg.u32Ret;
        }
    }

    if(UtopiaIoctl(pu32MVOP_Inst[E_MVOP_MAIN_STREAM], E_MVOP_CMD_SET_POWERSTATE, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        MVOP_PRINTF("[Fail] %s,%d\n",__FUNCTION__,__LINE__);
        return IO_arg.u32Ret;
    }

    IO_arg.u32Ret = TRUE;
    return IO_arg.u32Ret;
#else
    void* pInstance = NULL;
    return MDrv_MVOP_SetPowerState_V2(pInstance, u16PowerState);
#endif


}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void _MVOP_MdbInfoSWInput(MS_U64 *u64ReqHdl, MVOP_DevID eDevID, MVOP_InputCfg* stMvopInputCfg, MVOP_InputSel _eInputSel)
{
    MdbPrint(u64ReqHdl,"=== Input Config ===\n");

    if((NULL == stMvopInputCfg))
    {
        if(MVOP_INPUT_UNKNOWN == _eInputSel)
        {
            MdbPrint(u64ReqHdl,"Yoffset                   :    Na\n");
            MdbPrint(u64ReqHdl,"UVoffset                  :    Na\n");
            MdbPrint(u64ReqHdl,"Hsize                     :    Na\n");
            MdbPrint(u64ReqHdl,"Vsize                     :    Na\n");
            MdbPrint(u64ReqHdl,"strip                     :    Na\n");
            MdbPrint(u64ReqHdl,"422                       :    Na\n");
            MdbPrint(u64ReqHdl,"bProgressive              :    Na\n");
            MdbPrint(u64ReqHdl,"bDramContd                :    Na\n");
            MdbPrint(u64ReqHdl,"bField                    :    Na\n");
            MdbPrint(u64ReqHdl,"b422Pack                  :    Na\n");
            MdbPrint(u64ReqHdl,"YUVtype                   :    Na\n");
        }
        else
        {
            MdbPrint(u64ReqHdl,"Yoffset                   :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"UVoffset                  :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"Hsize                     :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"Vsize                     :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"strip                     :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"bProgressive              :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"bDramContd                :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"bField                    :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"b422Pack                  :    set from vdec fw\n");
            MdbPrint(u64ReqHdl,"YUVtype                   :    set from vdec fw\n");
        }
    }
    else
    {
        MdbPrint(u64ReqHdl,"Yoffset                   :    %d\n",stMvopInputCfg->u32YOffset);
        MdbPrint(u64ReqHdl,"UVoffset                  :    %d\n",stMvopInputCfg->u32UVOffset);
        MdbPrint(u64ReqHdl,"Hsize                     :    %d\n",stMvopInputCfg->u16HSize);
        MdbPrint(u64ReqHdl,"Vsize                     :    %d\n",stMvopInputCfg->u16VSize);
        MdbPrint(u64ReqHdl,"strip                     :    %d\n",stMvopInputCfg->u16StripSize);
        MdbPrint(u64ReqHdl,"bProgressive              :    %d\n",stMvopInputCfg->bProgressive);
        MdbPrint(u64ReqHdl,"bDramContd                :    %d\n",stMvopInputCfg->bDramRdContd);
        MdbPrint(u64ReqHdl,"bField                    :    %d\n",stMvopInputCfg->bField);
        MdbPrint(u64ReqHdl,"b422Pack                  :    %d\n",stMvopInputCfg->b422pack);
        switch(eDevID)
        {
            case E_MVOP_DEV_0:
            {
                if(E_MVOP_RGB_NONE == p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eRgbFmt)
                {
                    if(stMvopInputCfg->bYUV422)
                        MdbPrint(u64ReqHdl,"YUVtype                   :    422\n");
                    else
                        MdbPrint(u64ReqHdl,"YUVtype                   :    420\n");
                }
                else
                {
                    MdbPrint(u64ReqHdl,"YUVtype                   :    RGB\n");
                }
                break;
            }
#if MVOP_SUPPORT_SUB
            case E_MVOP_DEV_1:
            {
                if(E_MVOP_RGB_NONE == p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._eRgbFmt)
                {
                    if(stMvopInputCfg->bYUV422)
                        MdbPrint(u64ReqHdl,"YUVtype                   :    422\n");
                    else
                        MdbPrint(u64ReqHdl,"YUVtype                   :    420\n");
                }
                else
                {
                    MdbPrint(u64ReqHdl,"YUVtype                   :    RGB\n");
                }

                break;
            }
#endif
            default:
                break;
        }

    }
}

void _MVOP_MdbInfoSWOutput(MS_U64 *u64ReqHdl, MVOP_VidStat* stMvopOutputCfg, MS_BOOL bHdup)
{
    MdbPrint(u64ReqHdl,"\n=== Output Config ===\n");

    if(NULL == stMvopOutputCfg)
    {
        MdbPrint(u64ReqHdl,"u16HorSize                :    Na\n");
        MdbPrint(u64ReqHdl,"u16VerSize                :    Na\n");
        MdbPrint(u64ReqHdl,"u16FrameRate              :    Na\n");
        MdbPrint(u64ReqHdl,"u8AspectRate              :    Na\n");
        MdbPrint(u64ReqHdl,"u8Interlace               :    Na\n");
        MdbPrint(u64ReqHdl,"u16HorOffset              :    Na\n");
        MdbPrint(u64ReqHdl,"u16VerOffset              :    Na\n");
        MdbPrint(u64ReqHdl,"bEnHDup                   :    Na\n");
    }
    else
    {
        MdbPrint(u64ReqHdl,"u16HorSize                :    %d\n",stMvopOutputCfg->u16HorSize);
        MdbPrint(u64ReqHdl,"u16VerSize                :    %d\n",stMvopOutputCfg->u16VerSize);
        MdbPrint(u64ReqHdl,"u16FrameRate              :    %d\n",stMvopOutputCfg->u16FrameRate);
        MdbPrint(u64ReqHdl,"u8AspectRate              :    %d\n",stMvopOutputCfg->u8AspectRate);
        MdbPrint(u64ReqHdl,"u8Interlace               :    %d\n",stMvopOutputCfg->u8Interlace);
        MdbPrint(u64ReqHdl,"u16HorOffset              :    %d\n",stMvopOutputCfg->u16HorOffset);
        MdbPrint(u64ReqHdl,"u16VerOffset              :    %d\n",stMvopOutputCfg->u16VerOffset);
        MdbPrint(u64ReqHdl,"bEnHDup                   :    %d\n",bHdup);
    }
}

void _MVOP_MdbInfoFeature(MS_U64 *u64ReqHdl, MVOP_FeatureMdb* pstFeature)
{
    MdbPrint(u64ReqHdl,"\n=== Feature ===\n");

    if(NULL == pstFeature)
    {
        MdbPrint(u64ReqHdl,"Mirror                    :    Na\n");
        MdbPrint(u64ReqHdl,"3D type                   :    Na\n");
        MdbPrint(u64ReqHdl,"HSK mode                  :    Na\n");
        MdbPrint(u64ReqHdl,"XC gen timing             :    Na\n");
        MdbPrint(u64ReqHdl,"MVOP crop                 :    Na\n");
        MdbPrint(u64ReqHdl,"OneField                  :    Na\n");
        MdbPrint(u64ReqHdl,"2p mode                   :    Na\n");
        MdbPrint(u64ReqHdl,"Force P                   :    Na\n");
        MdbPrint(u64ReqHdl,"Ext FPS                   :    Na\n");
    }
    else
    {
        MdbPrint(u64ReqHdl,"Mirror                    :    %d\n",pstFeature->enMirror);
        MdbPrint(u64ReqHdl,"3D type                   :    %d\n",pstFeature->en3D);
        MdbPrint(u64ReqHdl,"HSK mode                  :    %d\n",pstFeature->bHSK);
        MdbPrint(u64ReqHdl,"XC gen timing             :    %d\n",pstFeature->bXCGenTiming);
        MdbPrint(u64ReqHdl,"MVOP crop start           :    %d,%d\n",pstFeature->stCropSt.u16Xpos,pstFeature->stCropSt.u16Ypos);
        MdbPrint(u64ReqHdl,"MVOP crop size            :    %d,%d\n",pstFeature->stCropSize.u16Width,pstFeature->stCropSize.u16Height);
        MdbPrint(u64ReqHdl,"OneField                  :    %d\n",pstFeature->stCropSize);
        MdbPrint(u64ReqHdl,"2p mode                   :    %d\n",pstFeature->b2P);
        MdbPrint(u64ReqHdl,"Force P                   :    %d\n",pstFeature->bForceP);
        MdbPrint(u64ReqHdl,"Ext FPS                   :    %d\n",pstFeature->u32Framerate);
    }
}


void _MVOP_MdbInfoSWOutputTiming(MS_U64 *u64ReqHdl, MVOP_Timing* pMVOPTiming)
{
    MdbPrint(u64ReqHdl,"///////// SW Output Info(to XC) /////////\n");

    if(NULL == pMVOPTiming)
    {
        MdbPrint(u64ReqHdl,"Vtotal                    :    Na\n");
        MdbPrint(u64ReqHdl,"Htotal                    :    Na\n");
        MdbPrint(u64ReqHdl,"u16VImg_Start0            :    Na\n");
        MdbPrint(u64ReqHdl,"u16HImg_Start             :    Na\n");
        MdbPrint(u64ReqHdl,"bInterlace                :    Na\n");
        MdbPrint(u64ReqHdl,"u8Framerate               :    Na\n");
        MdbPrint(u64ReqHdl,"u16ExpFrameRate           :    Na\n");
        MdbPrint(u64ReqHdl,"u16H_Freq                 :    Na\n");
        MdbPrint(u64ReqHdl,"u16Width                  :    Na\n");
        MdbPrint(u64ReqHdl,"u16Height                 :    Na\n");
        MdbPrint(u64ReqHdl,"bHDuplicate               :    Na\n");
        MdbPrint(u64ReqHdl,"Hstar                     :    Na\n");
        MdbPrint(u64ReqHdl,"Vstart                    :    Na\n");
    }
    else
    {
        MdbPrint(u64ReqHdl,"Vtotal                    :    %d\n",pMVOPTiming->u16V_TotalCount);
        MdbPrint(u64ReqHdl,"Htotal                    :    %d\n",pMVOPTiming->u16H_TotalCount);
        MdbPrint(u64ReqHdl,"u16VImg_Start0            :    %d\n",pMVOPTiming->u16VBlank1_Start);
        MdbPrint(u64ReqHdl,"u16HImg_Start             :    %d\n",pMVOPTiming->u16HImg_Start);
        MdbPrint(u64ReqHdl,"bInterlace                :    %d\n",pMVOPTiming->bInterlace);
        MdbPrint(u64ReqHdl,"u8Framerate               :    %d\n",pMVOPTiming->u8Framerate);
        MdbPrint(u64ReqHdl,"u16ExpFrameRate           :    %d\n",pMVOPTiming->u16ExpFrameRate);
        MdbPrint(u64ReqHdl,"u16H_Freq                 :    %d\n",pMVOPTiming->u16H_Freq);
        MdbPrint(u64ReqHdl,"u16Width                  :    %d\n",pMVOPTiming->u16Width);
        MdbPrint(u64ReqHdl,"u16Height                 :    %d\n",pMVOPTiming->u16Height);
        MdbPrint(u64ReqHdl,"bHDuplicate               :    %d\n",pMVOPTiming->bHDuplicate);
        MdbPrint(u64ReqHdl,"Hstar                     :    %d\n",(pMVOPTiming->u16HImg_Start/2)+2);
        MdbPrint(u64ReqHdl,"Vstart                    :    %d\n",0x6); //fixme
    }
}

void _MVOP_MdbInfoRegInputCfg(MS_U64 *u64ReqHdl, MVOP_DevID eDevID, MVOP_RegInputMdb* stRegInput)
{
    MdbPrint(u64ReqHdl,"\n=== Input Setting ===\n");

    if(NULL == stRegInput)
    {
        MdbPrint(u64ReqHdl,"Reg_Yoffset               :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_UVoffset              :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_Hsize                 :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_Vsize                 :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_strip                 :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_422                   :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bProgressive          :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bDramContd            :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bField                :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_b422Pack              :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_YUVtype               :    Na\n");

        MdbPrint(u64ReqHdl,"Reg_DS Index              :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_FDMask                :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bMfdecEn              :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bitlen address        :    Na\n");
    }
    else
    {
        memset(stRegInput, 0, sizeof(MVOP_RegInputMdb));
        //add hal api for get value
        MdbPrint(u64ReqHdl,"Reg_Yoffset               :    %d\n",stRegInput->u32RegYoffset);
        MdbPrint(u64ReqHdl,"Reg_UVoffset              :    %d\n",stRegInput->u32RegUVoffset);
        MdbPrint(u64ReqHdl,"Reg_Hsize                 :    %d\n",stRegInput->u16RegHsize);
        MdbPrint(u64ReqHdl,"Reg_Vsize                 :    %d\n",stRegInput->u16RegVsize);
        MdbPrint(u64ReqHdl,"Reg_strip                 :    %d\n",stRegInput->u16RegStrip);
        MdbPrint(u64ReqHdl,"Reg_422                   :    %d\n",stRegInput->bReg422);
        MdbPrint(u64ReqHdl,"Reg_bProgressive          :    %d\n",stRegInput->bRegPMode);
        MdbPrint(u64ReqHdl,"Reg_bDramContd            :    %d\n",stRegInput->bRegDramContd);
        MdbPrint(u64ReqHdl,"Reg_bField                :    %d\n",stRegInput->bRegField);
        MdbPrint(u64ReqHdl,"Reg_b422Pack              :    %d\n",stRegInput->bReg422Pack);
        MdbPrint(u64ReqHdl,"Reg_YUVtype               :    %d\n");

        MdbPrint(u64ReqHdl,"Reg_DS Index              :    %d\n",stRegInput->u8RegDSIndex);
        MdbPrint(u64ReqHdl,"Reg_FDMask                :    %d\n",stRegInput->u8RegFDMask);
        MdbPrint(u64ReqHdl,"Reg_bMfdecEn              :    %d\n",stRegInput->bRegMfdecEn);
        MdbPrint(u64ReqHdl,"Reg_bitlen address        :    %d\n",stRegInput->u32RegBitenAdd);
    }

}

void _MVOP_MdbInfoRegOutputCfg(MS_U64 *u64ReqHdl, MVOP_DevID eDevID, MVOP_RegOutputMdb* stRegOutput)
{
    MdbPrint(u64ReqHdl,"\n=== Output Setting ===\n");

    if(NULL == stRegOutput)
    {
        MdbPrint(u64ReqHdl,"Reg_Vtotal                :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_Htotal                :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16VImg_Start0        :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16HImg_Start         :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bInterlace            :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u8Framerate           :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16ExpFrameRate       :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16H_Freq             :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16Width              :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_u16Height             :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_bHDuplicate           :    Na\n");

        MdbPrint(u64ReqHdl,"Reg_b2P                   :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_Clock                 :    Na\n");
        MdbPrint(u64ReqHdl,"Reg_Vsync Forwarding Lines:    Na\n");
    }
    else
    {
        memset(stRegOutput, 0, sizeof(MVOP_RegOutputMdb));
        //add hal api for get value
        MdbPrint(u64ReqHdl,"Reg_Vtotal                :    %d\n",stRegOutput->u32RegVtt);
        MdbPrint(u64ReqHdl,"Reg_Htotal                :    %d\n",stRegOutput->u32RegHtt);
        MdbPrint(u64ReqHdl,"Reg_u16VImg_Start0        :    %d\n",stRegOutput->u16RegVImgst);
        MdbPrint(u64ReqHdl,"Reg_u16HImg_Start         :    %d\n",stRegOutput->u16RegHImgst);
        MdbPrint(u64ReqHdl,"Reg_bInterlace            :    %d\n",stRegOutput->bRegInterlace);
        MdbPrint(u64ReqHdl,"Reg_u8Framerate           :    %d\n",stRegOutput->u8RegFPS);
        MdbPrint(u64ReqHdl,"Reg_u16ExpFrameRate       :    %d\n",stRegOutput->u16RegExpFPS);
        MdbPrint(u64ReqHdl,"Reg_u16H_Freq             :    %d\n",stRegOutput->u16RegHFreq);
        MdbPrint(u64ReqHdl,"Reg_u16Width              :    %d\n",stRegOutput->u16RegWidth);
        MdbPrint(u64ReqHdl,"Reg_u16Height             :    %d\n",stRegOutput->u16RegHeight);
        MdbPrint(u64ReqHdl,"Reg_bHDuplicate           :    %d\n",stRegOutput->bRegHDup);

        MdbPrint(u64ReqHdl,"Reg_b2P                   :    %d\n",stRegOutput->bReg2P);
        MdbPrint(u64ReqHdl,"Reg_Clock                 :    %d\n",stRegOutput->enRegCLOCK);
        MdbPrint(u64ReqHdl,"Reg_Vsync Forwarding Lines:    %d\n",stRegOutput->u16RegVsForward);
    }
}

void _MVOP_MdbInfoRegFeature(MS_U64 *u64ReqHdl, MVOP_FeatureMdb* pstFeature)
{
    MdbPrint(u64ReqHdl,"\n=== Reg Feature ===\n");

    if(NULL == pstFeature)
    {
        MdbPrint(u64ReqHdl,"Mirror                    :    Na\n");
        MdbPrint(u64ReqHdl,"3D type                   :    Na\n");
        MdbPrint(u64ReqHdl,"HSK mode                  :    Na\n");
        MdbPrint(u64ReqHdl,"XC gen timing             :    Na\n");
        MdbPrint(u64ReqHdl,"MVOP crop                 :    Na\n");
        MdbPrint(u64ReqHdl,"OneField                  :    Na\n");
        MdbPrint(u64ReqHdl,"2p mode                   :    Na\n");
        MdbPrint(u64ReqHdl,"Force P                   :    Na\n");
        MdbPrint(u64ReqHdl,"Ext FPS                   :    Na\n");
        MdbPrint(u64ReqHdl,"YUV type                  :    Na\n");
    }
    else
    {
        memset(pstFeature, 0, sizeof(MVOP_FeatureMdb));
        //add hal api for get value
        MdbPrint(u64ReqHdl,"Mirror                    :    %d\n",pstFeature->enMirror);
        MdbPrint(u64ReqHdl,"3D type                   :    %d\n",pstFeature->en3D);
        MdbPrint(u64ReqHdl,"HSK mode                  :    %d\n",pstFeature->bHSK);
        MdbPrint(u64ReqHdl,"XC gen timing             :    %d\n",pstFeature->bXCGenTiming);
        MdbPrint(u64ReqHdl,"MVOP crop start           :    %d,%d\n",pstFeature->stCropSt.u16Xpos,pstFeature->stCropSt.u16Ypos);
        MdbPrint(u64ReqHdl,"MVOP crop size            :    %d,%d\n",pstFeature->stCropSize.u16Width,pstFeature->stCropSize.u16Height);
        MdbPrint(u64ReqHdl,"OneField                  :    %d\n",pstFeature->stCropSize);
        MdbPrint(u64ReqHdl,"2p mode                   :    %d\n",pstFeature->b2P);
        MdbPrint(u64ReqHdl,"Force P                   :    %d\n",pstFeature->bForceP);
        MdbPrint(u64ReqHdl,"Ext FPS                   :    %d\n",pstFeature->u32Framerate);
        MdbPrint(u64ReqHdl,"YUV type                  :    444\n");//fix me
    }
}

void MDrv_MVOP_GetMVOPMdbInfo_V2(MS_U64 *u64ReqHdl)
{
    MVOP_DevID  eDevID = E_MVOP_DEV_0; //default is to control main mvop
    MVOP_InputCfg* stMvopInputCfg = NULL;
    MVOP_InputSel _eInputSel = MVOP_INPUT_UNKNOWN;
    MVOP_VidStat* stMvopOutputCfg = NULL;
    MVOP_FeatureMdb* pstFeature = NULL;
    MVOP_Timing* pstTiming = NULL;
    MVOP_RegInputMdb* stRegInput = NULL;
    MVOP_RegOutputMdb* stRegOutput = NULL;
    MVOP_FeatureMdb* pstRegFeature = NULL;
    MS_BOOL  bIsInint = FALSE;
    MS_BOOL  bIsEnable = FALSE;

    MdbPrint(u64ReqHdl,"---------MStar MVOP Info---------\n\n");
    if(NULL != p_gDrvMVOPCtx)
    {
        //data collection
        switch(eDevID)
        {
            case E_MVOP_DEV_0:
            {
                bIsInint = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsInit;
                bIsEnable = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._stMVOPDrvStat.bIsEnable;
                _eInputSel = p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._eInputSel;
                stMvopInputCfg = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopInputCfg;
                stMvopOutputCfg = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopOutputCfg;
                MVOP_FeatureMdb stSWFtr;
                stSWFtr.enMirror = 0;
                stSWFtr.en3D = 0;
                stSWFtr.bHSK = 0;
                stSWFtr.bXCGenTiming = 0;
                stSWFtr.stCropSt.u16Xpos = 0;
                stSWFtr.stCropSt.u16Ypos = 0;
                stSWFtr.stCropSize.u16Width = 0;
                stSWFtr.stCropSize.u16Height = 0;
                stSWFtr.bOneField = 0;
                stSWFtr.b2P = 0;
                stSWFtr.bForceP = 0;
                stSWFtr.u32Framerate = 0;
                pstFeature = &stSWFtr;
                pstTiming = &p_gDrvMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_0]._mvopTiming;
                break;
            }
#if MVOP_SUPPORT_SUB
            case E_MVOP_DEV_1:
            {
                bIsInint = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsInit;
                bIsEnable = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._stMVOPDrvStat.bIsEnable;
                _eInputSel = p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._eInputSel;
                stMvopInputCfg = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopInputCfg;
                stMvopOutputCfg = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopOutputCfg;
                MVOP_FeatureMdb stSWFtr;
                stSWFtr.enMirror = 0;
                stSWFtr.en3D = 0;
                stSWFtr.bHSK = 0;
                stSWFtr.bXCGenTiming = 0;
                stSWFtr.stCropSt.u16Xpos = 0;
                stSWFtr.stCropSt.u16Ypos = 0;
                stSWFtr.stCropSize.u16Width = 0;
                stSWFtr.stCropSize.u16Height = 0;
                stSWFtr.bOneField = 0;
                stSWFtr.b2P = 0;
                stSWFtr.bForceP = 0;
                stSWFtr.u32Framerate = 0;
                pstFeature = &stSWFtr;
                pstTiming = &p_gDrvSubMVOPCtx->_stMVOPDrvCtrl[E_MVOP_DEV_1]._mvopTiming;
                break;
            }
#endif
            default:
                break;
        }
    }

    //print  info

    MdbPrint(u64ReqHdl,"IsInit                    :    %d\n",bIsInint);
    MdbPrint(u64ReqHdl,"Enable                    :    %d\n",bIsEnable);
    MdbPrint(u64ReqHdl,"Input Select              :    %d\n",_eInputSel);

    MdbPrint(u64ReqHdl,"///////// SW Input Info(from SDK) /////////\n");

    _MVOP_MdbInfoSWInput(u64ReqHdl, eDevID, stMvopInputCfg, _eInputSel);

    MdbPrint(u64ReqHdl,"RGB format                :    Na\n");//fixme

    _MVOP_MdbInfoSWOutput(u64ReqHdl, stMvopOutputCfg, FALSE);

    _MVOP_MdbInfoFeature(u64ReqHdl, pstFeature);

    _MVOP_MdbInfoSWOutputTiming(u64ReqHdl, pstTiming);

    MdbPrint(u64ReqHdl,"///////// Register Info /////////\n");
    _MVOP_MdbInfoRegInputCfg(u64ReqHdl, eDevID, stRegInput);

    _MVOP_MdbInfoRegOutputCfg(u64ReqHdl, eDevID, stRegOutput);

    _MVOP_MdbInfoRegFeature(u64ReqHdl, pstRegFeature);
}
#endif


