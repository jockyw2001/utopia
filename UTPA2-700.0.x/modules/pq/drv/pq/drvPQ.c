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

#define  _MDRV_PQ_C_

#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include "stdio.h"
#include <sys/stat.h>
#define do_div(x,y) ((x)/=(y))
#else
#include <linux/string.h>
#include <asm/div64.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#endif

#include "MsCommon.h"
//#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_v2.h"
#include "apiXC_Ace.h"
#include "drvMVOP.h"
#include "drvSYS.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "drvPQ_cus.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "mhal_pq_cus.h"
#include "drvPQ_Bin.h"
#include "apiPNL.h"

#include "QualityMode.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"
#include "mhal_pq_adaptive.h"
#include "drvSYS.h"

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#ifndef PQ_HAL_EWINDOW_PARAM
#define PQ_HAL_EWINDOW_PARAM 0
#endif

#ifndef PQ_DISABLE_UC_FOR_PREVSD
#define PQ_DISABLE_UC_FOR_PREVSD  0
#endif

#ifndef PQ_DISABLE_UC_FOR_PIP
#define PQ_DISABLE_UC_FOR_PIP             0
#endif

#ifndef PQ_GRULE_SLZ_ENABLE
#define PQ_GRULE_SLZ_ENABLE   0
#endif

#ifndef PQ_DDR_SELECT_VER
#define PQ_DDR_SELECT_VER 1
#endif

#ifndef PQ_BW_4K2KPIP_SUPPORT
#define PQ_BW_4K2KPIP_SUPPORT 1
#endif

#ifndef PQ_SUPPORT_SUB_POSTCCS
#define PQ_SUPPORT_SUB_POSTCCS 1
#endif

#ifndef PQ_SUPPORT_SUB_DHD
#define PQ_SUPPORT_SUB_DHD 1
#endif

#ifndef PQ_GRULE_3D_ENABLE
#define PQ_GRULE_3D_ENABLE 0
#endif

/********************************************************************************
// PQ_IOCTL_SET_UC_FEATURE is flag for set UCNR  During SetWin(if enable this switch please sync CL641359)
// and please check  PQ excel skipRule(UC_CTL/UCDi/MADi_EODiW/MADi_SSTMADi_Motion need skip)item to prevent to set UCNR twice
**********************************************************************************/
#ifndef PQ_IOCTL_SET_UC_FEATURE
#define PQ_IOCTL_SET_UC_FEATURE   0
#endif

#ifndef Y2Y_709_601_VERSION
#define Y2Y_709_601_VERSION     0
#endif

#ifndef REG_CHIP_EXECUTOR_DUMMY
#define REG_CHIP_EXECUTOR_DUMMY 0
#endif
#ifndef REG_CHIP_EXECUTOR_KEY_DUMMY
#define REG_CHIP_EXECUTOR_KEY_DUMMY 0
#endif

#ifndef REG_CHIP_EXECUTOR_KEY
#define REG_CHIP_EXECUTOR_KEY 0
#endif

#ifndef PQ_SUPPORT_ADAPTIVE_VERSION
#define PQ_SUPPORT_ADAPTIVE_VERSION FALSE
#endif

#ifndef PQ_DEMO_MODE_ENABLE
#define PQ_DEMO_MODE_ENABLE 0
#endif

#ifndef PQ_SCALER_GAMMA_ENABLE
#define PQ_SCALER_GAMMA_ENABLE 0
#endif

#ifndef PQ_P_MODE8_3FRAME_MODE_ENABLE
#define PQ_P_MODE8_3FRAME_MODE_ENABLE 0
#endif

#define PQ_ADAPTIVE_INVALID_VERSION (0xFFFFFFFF)

static MSIF_Version _drv_pq_version = {
    .DDI = { PQ_DRV_VERSION },
};

MS_U16 _u16PQSrcType[PQ_MAX_WINDOW];
MS_U16 _u16PQSrcType_UFSC[PQ_MAX_WINDOW];
#if (ENABLE_PQ_BIN_CF)
MS_U16 _u16PQSrcType_CF[PQ_MAX_WINDOW];
#endif
MS_U16 _u16LastPQSrcType[PQ_MAX_WINDOW] = {0xffff,0xffff};
MS_BOOL _bSkipDuplicateSetting = FALSE;
MS_U16 _u16PQSrcType_DBK_Detect[PQ_MAX_WINDOW];     //For Auto_DBK SW driver used

static PQ_INPUT_SOURCE_TYPE _enInputSourceType[PQ_MAX_WINDOW];
static MS_BOOL _bColorRange0_255[PQ_MAX_WINDOW];
static MS_BOOL _bGameModeOn[PQ_MAX_WINDOW];
static MS_U8 _u8RWmappingNum[PQ_MAX_WINDOW];
static PQ_YUV_STD _enYUVStandard[PQ_MAX_WINDOW];
MS_BOOL _bRWNumIsReset[PQ_MAX_WINDOW];
static MS_U16 _u16SRAM1 = PQ_BIN_IP_NULL;
static MS_U16 _u16SRAM2 = PQ_BIN_IP_NULL;
static MS_BOOL _bNeedRestoreWRBankMappingNum = FALSE;

MS_BOOL _bCustomerMainPQEn = FALSE ;
MS_BOOL _bCustomerSubPQEn = FALSE ;
MS_BOOL _bUFSCPQEn = FALSE;
#if (ENABLE_PQ_BIN_CF)
MS_BOOL _bCFMainPQEn = FALSE;
MS_BOOL _bCFSubPQEn = FALSE;
#endif

//whether current status is Point-to-point mode
static MS_BOOL _bPointToPointMode = FALSE;
static MS_BOOL _bPqBypassMode = FALSE;
//whether current status is Dualview display mode
MS_BOOL _bDualViewEnable = FALSE;

//whether current status is in force video mode
static MS_BOOL _bForceVideoInputMode[PQ_MAX_WINDOW] = {FALSE, FALSE};

// Enable/Disable P2P force to do CSC
MS_BOOL _bP2pForceToDoCsc = FALSE;

/// Soc output color format
PQ_COLOR_FORMAT _enOutputColorFmt = E_PQ_COLOR_YUV444;

MS_U16 _u16PQDbgSwitch = FALSE;
//MS_U16 _u16PQDbgSwitch = PQ_DBG_PQTAB | PQ_DBG_SRULE | PQ_DBG_CSCRULE | PQ_DBG_GRULE | PQ_DBG_BW | PQ_DBG_MADI | PQ_DBG_INFO | PQ_DBG_IOCTL | PQ_DBG_P2P;
MS_BOOL _bDS_En = FALSE;
static MS_BOOL _bDMSV12L_PQConfigEn = FALSE; //this flag is coming from PQ file
#if PQ_EN_DMS_SW_CTRL
static MS_BOOL _bDMSV12L_PIPEn = TRUE; //TRUE: it's not PIP/POP; FALSE: PIP/POP case want to disable DMS12L
#endif
MS_U16 _u16RW_Method = 0;

static MS_U16 _u16PQDelyaTime = 0;
static MS_U16 _u16PQSuggestedFrameNum[PQ_MAX_WINDOW];
static MS_BOOL _bSensioMode = FALSE;
static MS_U8 u8PQDelayCount = 0;
static MS_BOOL _bFrameBuffLess = FALSE;

static MS_PQ_INFO   _info = {.u16Input_Src_Num = 128, .u8IP_Num = 217,};
static MS_PQ_Status _status = {.bIsInitialized = FALSE, .bIsRunning = FALSE, };
static MS_PQ_RFBL_INFO _stRFBL_Info = {.bEnable = FALSE, .bFilm = FALSE , .u8MADiType = 0, };
BW_INFO_t bw_info;


MS_BOOL gbPQBinEnable = 0;
MS_PQBin_Header_Info stPQBinHeaderInfo[MAX_PQ_BIN_NUM];
extern MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
extern MS_BOOL gbEnablePQTextBin;

MS_S32 _PQ_Mutex = -1;

static EN_POWER_MODE _enPrevPowerState = E_POWER_SUSPEND;


#ifdef MSOS_TYPE_LINUX
pthread_mutex_t _PQ_MLoad_Mutex;
#endif



#define PQ_BIN_CUSTOMER_PANEL_INDEX        0
#define PQ_BIN_UFSC_PANEL_INDEX        0
#if (ENABLE_PQ_BIN_CF)
#define PQ_BIN_CF_PANEL_INDEX          0
#endif
#define PQ_IS_4K_INPUT(HSize,Vsize)  (((HSize) > 3000) && ((Vsize) >= 1050)) //Load 4k2k PQ mode if input is 4k2k or 4k1k.

MS_PQ_Hdmi_Info _stHDMI_Info[PQ_MAX_WINDOW];
MS_PQ_Vd_Info _stVD_Info[PQ_MAX_WINDOW];
MS_PQ_Mode_Info  _stMode_Info[PQ_MAX_WINDOW];
MS_U8 _u8ModeIndex[PQ_MAX_WINDOW];
MS_PQ_MuliMedia_Info _stMultiMedia_Info[PQ_MAX_WINDOW];
MS_PQ_Dtv_Info _stDTV_Info[PQ_MAX_WINDOW];
MS_PQ_Init_Info _stPQ_Info;
MS_BOOL _gbMemfmt422[PQ_MAX_WINDOW]={1, 1};
MS_U16 _gu16SetMemfmt_Madi[PQ_MAX_WINDOW]={0xFFFF, 0xFFFF};
MS_U16 _gu16SetMemfmt_Madi_Motion[PQ_MAX_WINDOW]={0xFFFF, 0xFFFF};

MS_BOOL _gIsSrcHDMode[PQ_MAX_WINDOW];
PQ_FOURCE_COLOR_FMT _gForceColorFmt[PQ_MAX_WINDOW];
MS_BOOL _gIs3D_En = 0;
MS_BOOL _bBypassModeOn[PQ_MAX_WINDOW]={0, 0};
MS_BOOL _gIsEnable8LBMode = FALSE;

MS_S32 _s32LastPQIPMADiMain = -1;
MS_S32 _s32LastPQIPMemFormatMain = -1;
MS_S32 _s32LastPQIPHSDYMain = -1;
MS_S32 _s32LastPQIPHSDCMain = -1;
MS_S32 _s32LastPQIPMADiSub = -1;
MS_S32 _s32LastPQIPMemFormatSub = -1;
MS_S32 _s32LastPQIPMADiMotMain = -1;
MS_S32 _s32LastPQIPMADiMotSub = -1;

static MS_U16 _upLayerConfig = 0;

#if PQ_UC_CTL
static MS_U16 _u16MADi_DFK_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16MADi_SST_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16MADi_EODiW_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16UCNR_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16UCDi_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16UCCTL_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16SwDriver_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16PostCCS_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
static MS_U16 _u16DHD_Current[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};

#endif

extern MS_BOOL _gIsMVC4kx1k;
PQ_DISPLAY_TYPE _enPQDisplayType = PQ_DISPLAY_ONE;
#if !defined(MSOS_TYPE_LINUX_KERNEL)
extern void MApi_XC_Set_OPWriteOffEnableToReg(MS_BOOL bEnable, SCALER_WIN eWindow) __attribute__((weak));
#endif
extern MS_U16 QM_InputSourceToIndex(void* pInstance, PQ_WIN eWindow, MS_U8 enInputSourceType);
extern MS_U16 QM_InputSourceToIndex_UFSC(void* pInstance,PQ_WIN eWindow, MS_U8 enInputSourceType);
#if (ENABLE_PQ_BIN_CF)
extern MS_U16 QM_InputSourceToIndex_CF(void* pInstance,PQ_WIN eWindow, MS_U8 enInputSourceType);
#endif
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
static MS_U8 u8BK22_78_L = 0x00;
static MS_U8 u8BK22_78_H = 0x00;
static MS_U8 u8BK22_79_L = 0x00;
static MS_U8 u8BK22_79_H = 0x00;

//////////////////////////////////////////////////////////////////
// set MADi force Y/C motion enable bit
// function:
//           SAVE/RESTORE/SET
//
#define SAVE_FORCE_YC_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_78_L = Hal_PQ_get_force_y_motion(pInstance,bMainWin); \
        u8BK22_79_L = Hal_PQ_get_force_c_motion(pInstance,bMainWin);\
    }\
    else\
    {\
        u8BK22_78_H = Hal_PQ_get_force_y_motion(pInstance,bMainWin); \
        u8BK22_79_H = Hal_PQ_get_force_c_motion(pInstance,bMainWin);\
    }\
} while(0)

#define SAVE_FORCE_Y_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_78_L = Hal_PQ_get_force_y_motion(pInstance,bMainWin); \
    }\
    else\
    {\
        u8BK22_78_H = Hal_PQ_get_force_y_motion(pInstance,bMainWin); \
    }\
} while(0)

#define SAVE_FORCE_C_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        u8BK22_79_L = Hal_PQ_get_force_c_motion(pInstance,bMainWin);\
    }\
    else\
    {\
        u8BK22_79_H = Hal_PQ_get_force_c_motion(pInstance,bMainWin);\
    }\
} while(0)

#define RESTORE_FORCE_YC_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(pInstance, 1, (MS_U16)u8BK22_78_L);\
        Hal_PQ_set_force_c_motion(pInstance, 1, (MS_U16)u8BK22_79_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(pInstance, 0, (((MS_U16)u8BK22_78_H)<<8));\
        Hal_PQ_set_force_c_motion(pInstance, 0, (((MS_U16)u8BK22_79_H))<<8);\
    }\
} while(0)

#define RESTORE_FORCE_Y_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_y_motion(pInstance, TRUE, (MS_U16)u8BK22_78_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_y_motion(pInstance, FALSE, (((MS_U16)u8BK22_78_H)<<8));\
    }\
} while(0)

#define RESTORE_FORCE_C_MOTION_ENABLE(pInstance,bMainWin) \
do{ \
    if(bMainWin)\
    {\
        Hal_PQ_set_force_c_motion(pInstance, TRUE, (MS_U16)u8BK22_79_L);\
    }\
    else\
    {\
        Hal_PQ_set_force_c_motion(pInstance, FALSE, (((MS_U16)u8BK22_79_H))<<8);\
    }\
} while(0)
#endif
//////////////////////////////////////////////////////////////////
// PQ Patch
MS_BOOL _bOSD_On = FALSE;

MS_BOOL bSetFrameCount = TRUE;

#if 0 //def _PDEBUG
#define PTH_PQ_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if (r != 0 && r != ETIMEDOUT) \
            fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_PQ_RET_CHK(_pf_) //_pf_
#endif


#ifdef MSOS_TYPE_LINUX
#define PQ_MLOAD_ENTRY() PTH_PQ_RET_CHK(pthread_mutex_lock(&_PQ_MLoad_Mutex))
#define PQ_MLOAD_RETURN() PTH_PQ_RET_CHK(pthread_mutex_unlock(&_PQ_MLoad_Mutex))
#else
#define PQ_MLOAD_ENTRY()
#define PQ_MLOAD_RETURN()
#endif

#ifndef PQ_VIP_RGBCMY_CTL_VER2
#define PQ_VIP_RGBCMY_CTL_VER2 0
#endif

//////////////////////////////////////////////////////////////////
// OS related
//
//
#define XC_PQ_WAIT_MUTEX              (TICK_PER_ONE_MS * 50)              // 50 ms

//#define TRACE_DBG
/*
// Mutex & Lock
static MS_S32 _s32XC_PQMutex = -1;


#define XC_PQ_ENTRY()                 {   if (_s32XC_PQMutex < 0) { return E_XC_ACE_FAIL; }        \
                                        if(OS_OBTAIN_MUTEX(_s32XC_PQMutex, XC_PQ_WAIT_MUTEX) == FALSE) return E_XC_ACE_OBTAIN_MUTEX_FAIL;  }

#define XC_PQ_RETURN(_ret)            {   OS_RELEASE_MUTEX(_s32XC_PQMutex); return _ret; }
*/

//////////////////////////////////////////////////////////////////

#define PQ_FILE_NAME_LENGTH 128
#define IsVBY1_16LANE(x)   (((x)== E_XC_PNL_LPLL_VBY1_10BIT_16LANE)||((x)== E_XC_PNL_LPLL_VBY1_8BIT_16LANE))

char pm_PQBinFilePathName[PQ_FILE_NAME_LENGTH] = {0};
const char DefaltPath_Ini[] = "/config/pq/";
const char CustomerPath_Ini[] = "/Customer/pq/";
const char MainBinName[]     = "Main.bin";
const char MainExBinName[]   = "Main_Ex.bin";
const char MainColorBinName[]= "Main_Color.bin";
const char UFSCBinName[]= "UFSC.bin";
const char SubBinName[]      = "Sub.bin";
const char SubExBinName[]    = "Sub_Ex.bin";
const char SubColorBinName[] = "Sub_Color.bin";
#if (ENABLE_PQ_BIN_CF)
const char MainCFBinName[]= "Main_CF.bin";
const char SubCFBinName[]= "Sub_CF.bin";
#endif

void* g_pPQResource[E_PQ_DEVICE_ID_MAX];
static MS_U16 MDrv_PQ_GetXRuleIPNum(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType);
static MS_BOOL _MDrv_PQ_LoadScalingTable(void* pInstance,PQ_WIN  eWindow,MS_U8 eXRuleType,MS_U8 u8XRuleIP,MS_BOOL bPreV_ScalingDown,MS_BOOL bInterlace,MS_BOOL bColorSpaceYUV,MS_U16 u16InputSize,MS_U16 u16SizeAfterScaling);
static MS_U16 MDrv_PQ_GetXRuleIPIndex(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP);
static MS_U16 MDrv_PQ_GetXRuleTableIndex(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP);
static void _MDrv_PQ_EnableRequest_FrameBufferLess(MS_BOOL bEnable, PQ_WIN eWindow);
static MS_BOOL _MDrv_PQ_SetGRuleStatus(void *pInstance,PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16PQ_GroupIdx);
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
static MS_BOOL _MDrv_PQ_LoadGameModeTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_GameMode_Idx);
#endif
#if defined(PQ_GRULE_HDR_ENABLE) && (PQ_GRULE_HDR_ENABLE == 1)
static MS_BOOL _MDrv_PQ_LoadHDRModeTable(PQ_WIN eWindow, MS_U16 u16GRuleLevelIndex);
#endif
#if (defined (UFO_XC_DS_PQ))
extern MS_BOOL _gIsNetworkMM;
extern MS_BOOL MApi_XC_Is_SupportSWDS(void);
extern void MDrv_PQBin_DS_GetTable(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, PQ_ENGINE_TYPE ePQEngineType);

MS_BOOL imode_PQDS = FALSE;
void MDrv_PQ_set_imode_PQDS(MS_BOOL imode)
{
    imode_PQDS = imode;
}
MS_U16 MDrv_PQ_GetGRule_TableIndex_PQDS(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
#ifndef MSOS_TYPE_LINUX_KERNEL
            if(_gIsNetworkMM)
            {
                _u16PQSrcType[eWindow] = QM_NetWork_4K2K_DS_Main;
            }
            else
            {
                if(!imode_PQDS)
                {
                    _u16PQSrcType[eWindow] = QM_MM_4K2K_DS_Main;
                }
                else
                {
                    _u16PQSrcType[eWindow] = QM_Multimedia_video_HD_interlace_Main;
                }
            }
#endif
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}

MS_BOOL _MDrv_PQ_LoadDSModeTable(void* pInstance, SCALER_WIN eWindow, MS_U16 u16PQ_GameMode_Idx, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if(eWindow == SUB_WINDOW)
    {
        return FALSE;
    }

    switch(ePQEngineType)
    {
        case PQ_XC0_STD:
            if (u16PQ_GameMode_Idx > PQ_GRULE_DS_PARAMETERS_LVL_NUM_Main)
            {
                MS_ASSERT(0);
                return FALSE;
            }

            for(i=0; i<PQ_GRULE_DS_PARAMETERS_IP_NUM_Main; i++)
            {
                u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_Main, i);

                if (u16PQ_GameMode_Idx == PQ_GRULE_DS_PARAMETERS_NUM_Main) // see it as default
                    u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, u16IPIdx);
                else
                    u16TabIdx = MDrv_PQ_GetGRule_TableIndex_PQDS(pInstance,eWindow, PQ_GRule_DS_PARAMETERS_Main, u16PQ_GameMode_Idx, i);

                //XC_PRINTF("\033[1;32m[%s:%d]u16PQ_GameModeIdx = %d,u16IPIdx = %d,u16TabIdx = %d\033[m\n",__FUNCTION__,__LINE__,u16PQ_GameMode_Idx,u16IPIdx,u16TabIdx);
                MDrv_PQBin_DS_GetTable(pInstance,eWindow, u16TabIdx, u16IPIdx,PQ_XC0_STD);
            }
            break;
        case PQ_XC0_UFSC:
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE

            _u16PQSrcType_UFSC[eWindow]= QM_InputSourceToIndex_UFSC(pInstance,eWindow, _enInputSourceType[eWindow]);

            if (u16PQ_GameMode_Idx > PQ_GRULE_DS_PARAMETERS_LVL_NUM_UFSC)
            {
                MS_ASSERT(0);
                return FALSE;
            }
            for(i=0; i<PQ_GRULE_DS_PARAMETERS_IP_NUM_UFSC; i++)
            {
                u16IPIdx = MDrv_PQ_GetGRule_IPIndex_Ex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_UFSC, i, PQ_XC0_UFSC);

                if (u16PQ_GameMode_Idx == PQ_GRULE_DS_PARAMETERS_NUM_UFSC) // see it as default
                    u16TabIdx = MDrv_PQ_GetTableIndex_Ex_U2(pInstance, eWindow, u16IPIdx, PQ_XC0_UFSC);
                else
                    u16TabIdx = MDrv_PQ_GetGRule_TableIndex_Ex_U2(pInstance,eWindow, PQ_GRule_DS_PARAMETERS_UFSC, u16PQ_GameMode_Idx, i, PQ_XC0_UFSC);

                //XC_PRINTF("\033[1;32m[%s:%d]u16PQ_GameModeIdx = %d,u16IPIdx = %d,u16TabIdx = %d\033[m\n",__FUNCTION__,__LINE__,u16PQ_GameMode_Idx,u16IPIdx,u16TabIdx);
                MDrv_PQBin_DS_GetTable(pInstance,eWindow, u16TabIdx, u16IPIdx,PQ_XC0_UFSC);
            }
#endif
            break;
        default:
            break;
    }
    //MDrv_PQ_LoadScalingTable_U2
    return TRUE;
}
void _MDrv_PQ_Load_DS_Table(void* pInstance,PQ_WIN eWindow,MS_U32 V_Size, MS_U32 H_Size)
{
    MS_U16 u16PQ_GameModeIdx = 0;
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
    MS_U16 u16PQ_GameModeUFSCIdx = 0;
#endif
    if(V_Size >=2160)
    {
        u16PQ_GameModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_Main, PQ_GRule_Lvl_DS_PARAMETERS_4K_Main);
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
        u16PQ_GameModeUFSCIdx = MDrv_PQ_GetGRule_LevelIndex_Ex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_UFSC, PQ_GRule_Lvl_DS_PARAMETERS_4K_UFSC,PQ_XC0_UFSC);
#endif
    }else if( (V_Size < 2160) && (V_Size >= 720) )
    {
        u16PQ_GameModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_Main, PQ_GRule_Lvl_DS_PARAMETERS_FHD_Main);
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
        u16PQ_GameModeUFSCIdx = MDrv_PQ_GetGRule_LevelIndex_Ex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_UFSC, PQ_GRule_Lvl_DS_PARAMETERS_FHD_UFSC,PQ_XC0_UFSC);
#endif
    }else if( (V_Size < 720) && (V_Size >= 480) )
    {
        u16PQ_GameModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_Main, PQ_GRule_Lvl_DS_PARAMETERS_SD_Main);
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
        u16PQ_GameModeUFSCIdx = MDrv_PQ_GetGRule_LevelIndex_Ex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_UFSC, PQ_GRule_Lvl_DS_PARAMETERS_SD_UFSC,PQ_XC0_UFSC);
#endif
    }
    else
    {
        u16PQ_GameModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_Main, PQ_GRule_Lvl_DS_PARAMETERS_L_SD_Main);
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
        u16PQ_GameModeUFSCIdx = MDrv_PQ_GetGRule_LevelIndex_Ex_U2(pInstance, eWindow, PQ_GRule_DS_PARAMETERS_UFSC, PQ_GRule_Lvl_DS_PARAMETERS_L_SD_UFSC,PQ_XC0_UFSC);
#endif
    }
    //XC_PRINTF("\033[1;32m[%s:%d]DS-PQ = %d [%d:%d]\033[m\n",__FUNCTION__,__LINE__,u16PQ_GameModeIdx,V_Size,H_Size);
    _MDrv_PQ_LoadDSModeTable(pInstance,eWindow,u16PQ_GameModeIdx,PQ_XC0_STD);
#if defined(PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE) && PQ_GRULE_UFSC_DS_PARAMETERS_ENABLE
    _MDrv_PQ_LoadDSModeTable(pInstance,eWindow,u16PQ_GameModeIdx,PQ_XC0_UFSC);
#endif
}
#endif

MS_U32 MDrv_PQ_Get_Semaphore(void *pInstance,E_PQ_POOL_ID eID_TYPE)
{
    E_PQ_POOL_ID eID=eID_TYPE;
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if(pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }
    else
    {
        UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    }

    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, eID, &g_pPQResource[eID]) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 MDrv_PQ_Release_Semaphore(void *pInstance,E_PQ_POOL_ID eID_TYPE)
{
    E_PQ_POOL_ID eID=eID_TYPE;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;

    if(pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }
    else
    {
        UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    }
    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    u32Return = UtopiaResourceRelease(g_pPQResource[eID]);
    return u32Return;
}

MS_U8 * Open_PQ_Bin(MS_U8 * pName)
{
    long lFileSize;
    MS_U32 u32Rst;
    const char * Name;
    MS_U8 *pu8Buffer;

    Name = (const char *)pName;
#if !defined(MSOS_TYPE_LINUX_KERNEL)
    FILE * pFile = NULL;
    int fd;
    struct stat fileStat;

    pFile = fopen(Name, "rb");
    if(pFile == NULL)
    {
        printf("Open %s Failed\n", pName);
        return NULL;
    }

    fd = fileno(pFile);
    if((fstat(fd, &fileStat) != 0))
    {
        printf("get file stat %s Failed\n", pName);
        return NULL;
    }
    lFileSize = fileStat.st_size;

    if (lFileSize <= 0)
    {
        fclose(pFile);
        return NULL;
    }

    pu8Buffer = (MS_U8*) malloc(lFileSize);
    if(pu8Buffer == NULL)
    {
        printf("out of memory \n");
        fclose(pFile);
        return NULL;
    }

    u32Rst = fread(pu8Buffer, 1, lFileSize, pFile);
    if(u32Rst != (MS_U32)lFileSize)
    {
        printf("Read Main.bin Error %d %ld\n", u32Rst, lFileSize);
        free(pu8Buffer);
        fclose(pFile);
        return NULL;
    }

    fclose(pFile);
#else
    struct file *pFile = NULL;

    mm_segment_t cur_mm_seg;
    pFile = filp_open(Name, O_RDONLY, 0);
    if(pFile == NULL)
    {
        printk("Open %s Failed\n", pName);

        return NULL;
    }
    vfs_llseek(pFile, 0, SEEK_END);
    lFileSize = pFile->f_pos;
    if (lFileSize <= 0)
    {
        filp_close(pFile, NULL);

        return NULL;
    }
    loff_t pos = vfs_llseek(pFile, 0, SEEK_SET);
    pu8Buffer = (MS_U8*)vmalloc(lFileSize);

    if(pu8Buffer == NULL)
    {
        printk("out of memory \n");
        filp_close(pFile, NULL);

        return NULL;
    }
    cur_mm_seg = get_fs();
    set_fs(KERNEL_DS);
    u32Rst = vfs_read(pFile, pu8Buffer, lFileSize, &pos);
    set_fs(cur_mm_seg);
    if(u32Rst != (MS_U32)lFileSize)
    {
        printk("Read Main.bin Error %d %ld\n", u32Rst, lFileSize);
        vfree(pu8Buffer);
        filp_close(pFile, NULL);

        return NULL;
    }

    filp_close(pFile, NULL);

#endif

    return pu8Buffer;

}


#if !defined(MSOS_TYPE_LINUX_KERNEL)
void _MDrv_PQ_LoadPQBin(void* pInstance,MS_PQ_Init_Info *pstPQInitInfo)
{
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    MDrv_PQ_Get_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
    UtopiaResourceGetPrivate(g_pPQResource[E_PQ_DEVICE_ID_0],(void**)(&pPQResourcePrivate));

    if (MDrv_SYS_GetChipType() == E_SYS_CHIP_STB)
    {
        pstPQInitInfo->u8PQBinCnt = 0;
        pstPQInitInfo->u8PQTextBinCnt = 0;
        pstPQInitInfo->u8PQBinCustomerCnt = 0;
    }
    else
    {
#if(ENABLE_PQ_BIN)
        MS_PQ_CAP_INFO stPQCaps;
        MS_U8 *pu8MainBuf= NULL, *pu8MainExBuf= NULL;

        char DefaltPath[PQ_FILE_NAME_LENGTH]={0};
        char CustomerPath[PQ_FILE_NAME_LENGTH]={0};
        char IniPath[PQ_FILE_NAME_LENGTH]={0};

        ///// SET PQ BIN PATH
        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH[0] !=NULL) //Set DefaltPath
        {
            strncpy(DefaltPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH,PQ_FILE_PATH_LENGTH);
        }
        else
        {
            strncpy(DefaltPath,DefaltPath_Ini,strlen(DefaltPath_Ini));
        }

        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH[0] !=NULL) //Set CustomerPath
        {
            strncpy(CustomerPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH,PQ_FILE_PATH_LENGTH);
        }
        else
        {
            strncpy(CustomerPath,CustomerPath_Ini,strlen(CustomerPath_Ini));
        }

        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH[0] !=NULL) //Set IniPath
        {
            strncpy(IniPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH,PQ_FILE_PATH_LENGTH);
        }



        FILE *pFile = NULL;

        // if Main.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
        pFile = fopen(pm_PQBinFilePathName, "r");
        if(pFile == NULL)
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
            pFile = fopen(pm_PQBinFilePathName, "r");
            if(pFile == NULL)
            {
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
            }
            if (pFile != NULL)
            {
                fclose(pFile);
                pFile = NULL;
                pu8MainBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
            else
            {
               pFile = NULL;
            }
        }
        else
        {
            // read from /Customer/pq
            fclose(pFile);
            pFile = NULL;
            pu8MainBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }
        printf("Load PQ bin file, PQ_BIN_STD_MAIN path [%s]\n", pm_PQBinFilePathName);

        // if Main_Ex.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
        pFile = fopen(pm_PQBinFilePathName, "r");
        if(pFile == NULL)
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
            pFile = fopen(pm_PQBinFilePathName, "r");
            if(pFile == NULL)
            {
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
            }
            if (pFile != NULL)
            {
                fclose(pFile);
                pFile = NULL;
                pu8MainExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
            else
            {
                pFile = NULL;
            }
        }
        else
        {
            // read from /Customer/pq
            fclose(pFile);
            pFile = NULL;
            pu8MainExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }
        printf("Load PQ bin file, PQ_BIN_EXT_MAIN path [%s]\n", pm_PQBinFilePathName);

        if(pu8MainBuf && pu8MainExBuf)
        {
            pstPQInitInfo->u8PQBinCnt = 2;
            pstPQInitInfo->stPQBinInfo[0].u8PQID = (MS_U8) PQ_BIN_STD_MAIN;
            pstPQInitInfo->stPQBinInfo[0].PQBin_PhyAddr = (MS_PHYADDR) pu8MainBuf;
            pstPQInitInfo->stPQBinInfo[1].u8PQID = (MS_U8) PQ_BIN_EXT_MAIN;
            pstPQInitInfo->stPQBinInfo[1].PQBin_PhyAddr = (MS_PHYADDR)pu8MainExBuf;
        }
        else
            {
            pstPQInitInfo->u8PQBinCnt = 0;

            }

        MS_U8 *pu8MainCustomerBuf = NULL;
        pstPQInitInfo->u8PQBinCustomerCnt = 0 ;
        // if Main_Color.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainColorBinName, strlen(MainColorBinName));
        pFile = fopen(pm_PQBinFilePathName, "r");
        if(pFile == NULL)
        {
            // read from default path
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
            strncat(pm_PQBinFilePathName, MainColorBinName, strlen(MainColorBinName));
            pFile = fopen(pm_PQBinFilePathName, "r");
            if (pFile != NULL)
            {
                fclose(pFile);
                pFile = NULL;
                pu8MainCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
        }
        else
        {
            // read from /Customer/pq
            fclose(pFile);
            pFile = NULL;
            pu8MainCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }
        printf("Load PQ bin file, PQ_BIN_CUSTOMER_MAIN path [%s]\n", pm_PQBinFilePathName);

        if(pu8MainCustomerBuf)
        {
            pstPQInitInfo->u8PQBinCustomerCnt = 1;
            pstPQInitInfo->stPQBinInfo[4].u8PQID = (MS_U8) PQ_BIN_CUSTOMER_MAIN;
            pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr = (MS_PHYADDR) pu8MainCustomerBuf;
        }
        else
        {
            pstPQInitInfo->u8PQBinCustomerCnt = 0;

        }

       //Skip USFC now, which may cause kernel panic, wait PQ Driver complated.

        MS_U8 *pu8UFSCBuf = NULL;
        pstPQInitInfo->u8PQBinUFSCCnt = 0;
        // if UFSC.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
        pFile = fopen(pm_PQBinFilePathName, "r");
        if(pFile == NULL)
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
            pFile = fopen(pm_PQBinFilePathName, "r");
            if(pFile == NULL)
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
            }
            if (pFile != NULL)
            {
                fclose(pFile);
                pFile = NULL;
                pu8UFSCBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
            else
            {
                pFile = NULL;
            }
        }
        else
        {
            // read from /Customer/pq
            fclose(pFile);
            pFile = NULL;
            pu8UFSCBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8UFSCBuf)
        {
            printf("Load PQ bin file, PQ_BIN_UFSC path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinUFSCCnt = 1;
            pstPQInitInfo->stPQBinInfo[6].u8PQID = (MS_U8) PQ_BIN_UFSC;
            pstPQInitInfo->stPQBinInfo[6].PQBin_PhyAddr = (MS_PHYADDR) pu8UFSCBuf;
        }
        else
        {
            pstPQInitInfo->u8PQBinUFSCCnt = 0;
            XC_INITMISC sXC_Init_Misc;
            memset(&sXC_Init_Misc, 0, sizeof(XC_INITMISC));
            MApi_XC_GetMISCStatus(&sXC_Init_Misc);
            sXC_Init_Misc.u32MISC_B |= E_XC_INIT_MISC_B_SKIP_SR;
            MApi_XC_Init_MISC(&sXC_Init_Misc, sizeof(XC_INITMISC));
        }
        #if (ENABLE_PQ_BIN_CF)
        // load Main CF Qmap bin

        MS_U8 *pu8MainCFBuf = NULL;
        pstPQInitInfo->u8PQBinCFCnt = 0;
        // if Main_CF.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
        pFile = fopen(pm_PQBinFilePathName, "r");
        if(pFile == NULL)
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
            pFile = fopen(pm_PQBinFilePathName, "r");
            if(pFile == NULL)
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
            }
            if (pFile != NULL)
            {
                fclose(pFile);
                pFile = NULL;
                pu8MainCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
            else
            {
                pFile = NULL;
            }
        }
        else
        {
            // read from /Customer/pq
            fclose(pFile);
            pFile = NULL;
            pu8MainCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8MainCFBuf)
        {
            printf("Load PQ bin file, PQ_BIN_CF_MAIN path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinCFCnt = 1;
            pstPQInitInfo->stPQBinInfo[7].u8PQID = (MS_U8) PQ_BIN_CF_MAIN;
            pstPQInitInfo->stPQBinInfo[7].PQBin_PhyAddr = (MS_PHYADDR) pu8MainCFBuf;
        }
#endif
        if(MDrv_PQ_GetCaps(&stPQCaps))
        {
            if(stPQCaps.b3DVideo_Supported || stPQCaps.bPIP_Supported)
            {
                MS_U8 *pu8SubBuf= NULL, *pu8SubExBuf= NULL ;

                // check if Sub.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubBinName, strlen(SubBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
                if(pFile == NULL)
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubBinName, strlen(SubBinName));
                    pFile = fopen(pm_PQBinFilePathName, "r");
                    if(pFile == NULL)
                    {
                        // read from default path
                        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                        strncpy(pm_PQBinFilePathName,DefaltPath,strlen(DefaltPath));
                        strncat(pm_PQBinFilePathName,SubBinName,strlen(SubBinName));
                        pFile = fopen(pm_PQBinFilePathName, "r");
                    }
                    if (pFile != NULL)
                    {
                        fclose(pFile);
                        pFile = NULL;
                        pu8SubBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                    else
                    {
                        pFile = NULL;
                    }
                }
                else
                {
                    // read from /Customer/pq
                    fclose(pFile);
                    pFile = NULL;
                    pu8SubBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }
                printf("Load PQ bin file, PQ_BIN_STD_SUB path [%s]\n", pm_PQBinFilePathName);

                // check if Sub_Ex.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubExBinName, strlen(SubExBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
                if(pFile == NULL)
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubExBinName, strlen(SubExBinName));
                    pFile = fopen(pm_PQBinFilePathName, "r");
                    if(pFile == NULL)
                    {
                    // read from default path
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName,DefaltPath,strlen(DefaltPath));
                    strncat(pm_PQBinFilePathName,SubExBinName,strlen(SubExBinName));
                    pFile = fopen(pm_PQBinFilePathName, "r");
                    }
                    if (pFile != NULL)
                    {
                        fclose(pFile);
                        pFile = NULL;
                        pu8SubExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                    else
                    {
                        pFile = NULL;
                    }
                }
                else
                {
                    // read from /Customer/pq
                    fclose(pFile);
                    pFile = NULL;
                    pu8SubExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }
                printf("Load PQ bin file, PQ_BIN_EXT_SUB path [%s]\n", pm_PQBinFilePathName);

                if(pu8SubBuf && pu8SubExBuf)
                {
                    pstPQInitInfo->u8PQBinCnt = 4;
                    pstPQInitInfo->stPQBinInfo[2].u8PQID = (MS_U8) PQ_BIN_STD_SUB;
                    pstPQInitInfo->stPQBinInfo[2].PQBin_PhyAddr = (MS_PHYADDR) pu8SubBuf;
                    pstPQInitInfo->stPQBinInfo[3].u8PQID = (MS_U8) PQ_BIN_EXT_SUB;
                    pstPQInitInfo->stPQBinInfo[3].PQBin_PhyAddr = (MS_PHYADDR)pu8SubExBuf;
                }

                MS_U8  *pu8SubCustomerBuf = NULL;
                // check if Sub_Color.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubColorBinName, strlen(SubColorBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
                if(pFile == NULL)
                {
                    // read from default path
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                    strncat(pm_PQBinFilePathName, SubColorBinName, strlen(SubColorBinName));
                    pFile = fopen(pm_PQBinFilePathName, "r");
                    if (pFile != NULL)
                    {
                        fclose(pFile);
                        pFile = NULL;
                        pu8SubCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                }
                else
                {
                    // read from /Customer/pq
                    fclose(pFile);
                    pFile = NULL;
                    pu8SubCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }

                if(pu8MainCustomerBuf && pu8SubCustomerBuf)
                {
                    printf("Load PQ bin file, PQ_BIN_CUSTOMER_SUB path [%s]\n", pm_PQBinFilePathName);
                    pstPQInitInfo->u8PQBinCustomerCnt = 2;
                    pstPQInitInfo->stPQBinInfo[5].u8PQID = (MS_U8) PQ_BIN_CUSTOMER_SUB;
                    pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr = (MS_PHYADDR) pu8SubCustomerBuf;
                }
            }
            #if (ENABLE_PQ_BIN_CF)
                // load Sub CF Qmap bin

                MS_U8 *pu8SubCFBuf = NULL;
                // if Sub_CF.bin exists in /Customer/pq, read it
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                pFile = fopen(pm_PQBinFilePathName, "r");
                if(pFile == NULL)
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                    pFile = fopen(pm_PQBinFilePathName, "r");
                    if(pFile == NULL)
                    {
                        // read from default path
                        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                        strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                        strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                        pFile = fopen(pm_PQBinFilePathName, "r");
                    }
                    if (pFile != NULL)
                    {
                        fclose(pFile);
                        pFile = NULL;
                        pu8SubCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                    else
                    {
                        pFile = NULL;
                    }
                }
                else
                {
                    // read from /Customer/pq
                    fclose(pFile);
                    pFile = NULL;
                    pu8SubCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }

                if(pu8SubCFBuf)
                {
                    printf("Load PQ bin file, PQ_BIN_CF_SUB path [%s]\n", pm_PQBinFilePathName);
                    pstPQInitInfo->u8PQBinCFCnt = 2;
                    pstPQInitInfo->stPQBinInfo[8].u8PQID = (MS_U8) PQ_BIN_CF_SUB;
                    pstPQInitInfo->stPQBinInfo[8].PQBin_PhyAddr = (MS_PHYADDR) pu8SubCFBuf;
                }
#endif
        }
#else
        pstPQInitInfo->u8PQBinCnt = 0;
        pstPQInitInfo->u8PQTextBinCnt = 0;
        pstPQInitInfo->u8PQBinCustomerCnt = 0;
        pstPQInitInfo->u8PQBinUFSCCnt = 0;
#if (ENABLE_PQ_BIN_CF)
        pstPQInitInfo->u8PQBinCFCnt = 0;
#endif

#endif
    }
    MDrv_PQ_Release_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
}
#else
void _MDrv_PQ_LoadPQBin(void* pInstance,MS_PQ_Init_Info *pstPQInitInfo)
{
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    MDrv_PQ_Get_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
    UtopiaResourceGetPrivate(g_pPQResource[E_PQ_DEVICE_ID_0],(void**)(&pPQResourcePrivate));

    /*if (MDrv_SYS_GetChipType() == E_SYS_CHIP_STB)
    {
        pstPQInitInfo->u8PQBinCnt = 0;
        pstPQInitInfo->u8PQTextBinCnt = 0;
        pstPQInitInfo->u8PQBinCustomerCnt = 0;
    }
    else   //TV case
    */
    {
#if(ENABLE_PQ_BIN)
        MS_PQ_CAP_INFO stPQCaps;
        MS_U8 *pu8MainBuf = NULL, *pu8MainExBuf = NULL;

        char DefaltPath[PQ_FILE_NAME_LENGTH]={0};
        char CustomerPath[PQ_FILE_NAME_LENGTH]={0};
        char IniPath[PQ_FILE_NAME_LENGTH]={0};

        ///// SET PQ BIN PATH
        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH[0] !=NULL) //Set DefaltPath
        {
            strncpy(DefaltPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH,PQ_FILE_PATH_LENGTH);
        }
        else
        {
            strncpy(DefaltPath,DefaltPath_Ini,strlen(DefaltPath_Ini));
        }

        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH[0] !=NULL) //Set CustomerPath
        {
            strncpy(CustomerPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH,PQ_FILE_PATH_LENGTH);
        }
        else
        {
            strncpy(CustomerPath,CustomerPath_Ini,strlen(CustomerPath_Ini));
        }
        if(pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH[0] !=NULL) //Set IniPath
        {
            strncpy(IniPath,pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH,PQ_FILE_PATH_LENGTH);
        }

        struct file *pFile = NULL;

        // if Main.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);

        if(IS_ERR(pFile))
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
            pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            if(IS_ERR(pFile))
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainBinName, strlen(MainBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            }
            if(IS_ERR(pFile))
            {
                pFile = NULL;
            }
            else
            {
                filp_close(pFile, NULL);
                pFile = NULL;
                pu8MainBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }

        }
        else
        {
            // read from /Customer/pq
            filp_close(pFile, NULL);
            pFile = NULL;
            pu8MainBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }
        printf( "Load PQ bin file, PQ_BIN_STD_MAIN path [%s]\n", pm_PQBinFilePathName);

        // if Main_Ex.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
        if(IS_ERR(pFile))
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
            pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            if(IS_ERR(pFile))
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainExBinName, strlen(MainExBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            }
            if(IS_ERR(pFile))
            {
                pFile = NULL;
            }
            else
            {
                filp_close(pFile, NULL);
                pFile = NULL;
                pu8MainExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
        }
        else
        {
            // read from /Customer/pq
            filp_close(pFile, NULL);
            pFile = NULL;
            pu8MainExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8MainBuf && pu8MainExBuf)
        {
            printk("Load PQ bin file, PQ_BIN_EXT_MAIN path [%s]\n", pm_PQBinFilePathName);

            pstPQInitInfo->u8PQBinCnt = 2;
            pstPQInitInfo->stPQBinInfo[0].u8PQID = (MS_U8) PQ_BIN_STD_MAIN;
            pstPQInitInfo->stPQBinInfo[0].PQBin_PhyAddr = (MS_PHYADDR) pu8MainBuf;

            pstPQInitInfo->stPQBinInfo[1].u8PQID = (MS_U8) PQ_BIN_EXT_MAIN;
            pstPQInitInfo->stPQBinInfo[1].PQBin_PhyAddr = (MS_PHYADDR)pu8MainExBuf;

        }
        else
        {
            printk("!! Error Load PQ bin file, PQ_BIN_EXT_MAIN path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinCnt = 0;
        }

        MS_U8 *pu8MainCustomerBuf = NULL;
        pstPQInitInfo->u8PQBinCustomerCnt = 0 ;
        // if Main_Color.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainColorBinName, strlen(MainColorBinName));
        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
        if(IS_ERR(pFile))
        {
            // read from default path
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
            strncat(pm_PQBinFilePathName, MainColorBinName, strlen(MainColorBinName));

            pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            if (!IS_ERR(pFile))
            {

                filp_close(pFile, NULL);
                pFile = NULL;
                pu8MainCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
            else
            {
                pFile = NULL;
            }
        }
        else
        {
            // read from /Customer/pq
            filp_close(pFile, NULL);
            pFile = NULL;
            pu8MainCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8MainCustomerBuf)
        {
            printk("Load PQ bin file, PQ_BIN_CUSTOMER_MAIN path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinCustomerCnt = 1;
            pstPQInitInfo->stPQBinInfo[4].u8PQID = (MS_U8) PQ_BIN_CUSTOMER_MAIN;
            pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr = (MS_PHYADDR) pu8MainCustomerBuf;
        }
        else
        {
            pstPQInitInfo->u8PQBinCustomerCnt = 0;
            printk( "!!Error Load PQ bin file, PQ_BIN_CUSTOMER_MAIN path [%s]\n", pm_PQBinFilePathName);
        }

        //Skip USFC now, which may cause kernel panic, wait PQ Driver complated.
        MS_U8 *pu8UFSCBuf = NULL;
        pstPQInitInfo->u8PQBinUFSCCnt = 0;
        // if UFSC.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
        if(IS_ERR(pFile))
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
            pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            if(IS_ERR(pFile))
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, UFSCBinName, strlen(UFSCBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            }
            if(IS_ERR(pFile))
            {
                pFile = NULL;
            }
            else
            {
                filp_close(pFile, NULL);
                pFile = NULL;
                pu8UFSCBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
        }
        else
        {
            // read from /Customer/pq
            filp_close(pFile, NULL);
            pFile = NULL;
            pu8UFSCBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8UFSCBuf)
        {
            printk("Load PQ bin file, PQ_BIN_UFSC path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinUFSCCnt = 1;
            pstPQInitInfo->stPQBinInfo[6].u8PQID = (MS_U8) PQ_BIN_UFSC;
            pstPQInitInfo->stPQBinInfo[6].PQBin_PhyAddr = (MS_PHYADDR) pu8UFSCBuf;
        }
        else
        {
            //pstPQInitInfo->u8PQBinUFSCCnt = 0;

            XC_INITMISC sXC_Init_Misc;
            memset(&sXC_Init_Misc, 0, sizeof(XC_INITMISC));
            MApi_XC_GetMISCStatus(&sXC_Init_Misc);
            sXC_Init_Misc.u32MISC_B |= E_XC_INIT_MISC_B_SKIP_SR;
            MApi_XC_Init_MISC(&sXC_Init_Misc, sizeof(XC_INITMISC));
        }
#if (ENABLE_PQ_BIN_CF)
        MS_U8 *pu8MainCFBuf = NULL;
        pstPQInitInfo->u8PQBinCFCnt = 0;
        // if CF_MAIN.bin exists in /Customer/pq, read it
        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
        strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
        strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
        if(IS_ERR(pFile))
        {
            memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
            strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
            strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
            pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            if(IS_ERR(pFile))
            {
                // read from default path
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                strncat(pm_PQBinFilePathName, MainCFBinName, strlen(MainCFBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
            }
            if(IS_ERR(pFile))
            {
                pFile = NULL;
            }
            else
            {
                filp_close(pFile, NULL);
                pFile = NULL;
                pu8MainCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
            }
        }
        else
        {
            // read from /Customer/pq
            filp_close(pFile, NULL);
            pFile = NULL;
            pu8MainCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
        }

        if(pu8MainCFBuf)
        {
            printk("Load PQ bin file, PQ_BIN_CF_MAIN path [%s]\n", pm_PQBinFilePathName);
            pstPQInitInfo->u8PQBinCFCnt = 1;
            pstPQInitInfo->stPQBinInfo[7].u8PQID = (MS_U8) PQ_BIN_CF_MAIN;
            pstPQInitInfo->stPQBinInfo[7].PQBin_PhyAddr = (MS_PHYADDR) pu8MainCFBuf;
        }
#endif
        if(MDrv_PQ_GetCaps(&stPQCaps))
        {
            if(stPQCaps.b3DVideo_Supported || stPQCaps.bPIP_Supported)
            {
                MS_U8 *pu8SubBuf = NULL, *pu8SubExBuf = NULL;

                // check if Sub.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubBinName, strlen(SubBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                if(IS_ERR(pFile))
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubBinName, strlen(SubBinName));
                    pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    if(IS_ERR(pFile))
                    {
                        // read from default path
                        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                        strncpy(pm_PQBinFilePathName,DefaltPath,strlen(DefaltPath));
                        strncat(pm_PQBinFilePathName,SubBinName,strlen(SubBinName));
                        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    }
                    if(IS_ERR(pFile))
                    {
                        pFile = NULL;
                    }
                    else
                    {
                        filp_close(pFile, NULL);
                        pFile = NULL;
                        pu8SubBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }

                }
                else
                {
                    // read from /Customer/pq
                    filp_close(pFile, NULL);
                    pFile = NULL;
                    pu8SubBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }
                printk("Load PQ bin file, PQ_BIN_STD_SUB path [%s]\n", pm_PQBinFilePathName);

                // check if Sub_Ex.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubExBinName, strlen(SubExBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                if(IS_ERR(pFile))
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubExBinName, strlen(SubExBinName));
                    pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    if(IS_ERR(pFile))
                    {
                        // read from default path
                        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                        strncpy(pm_PQBinFilePathName,DefaltPath,strlen(DefaltPath));
                        strncat(pm_PQBinFilePathName,SubExBinName,strlen(SubExBinName));
                        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    }
                    if(IS_ERR(pFile))
                    {
                        pFile = NULL;
                    }
                    else
                    {
                        filp_close(pFile, NULL);
                        pFile = NULL;
                        pu8SubExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }

                }
                else
                {
                    // read from /Customer/pq
                    filp_close(pFile, NULL);
                    pFile = NULL;
                    pu8SubExBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }
                printf("Load PQ bin file, PQ_BIN_EXT_SUB path [%s]\n", pm_PQBinFilePathName);

                if(pu8SubBuf && pu8SubExBuf)
                {
                    pstPQInitInfo->u8PQBinCnt = 4;
                    pstPQInitInfo->stPQBinInfo[2].u8PQID = (MS_U8) PQ_BIN_STD_SUB;
                    pstPQInitInfo->stPQBinInfo[2].PQBin_PhyAddr = (MS_PHYADDR) pu8SubBuf;
                    pstPQInitInfo->stPQBinInfo[3].u8PQID = (MS_U8) PQ_BIN_EXT_SUB;
                    pstPQInitInfo->stPQBinInfo[3].PQBin_PhyAddr = (MS_PHYADDR)pu8SubExBuf;
                }
                else
                {
                     pstPQInitInfo->u8PQBinCnt = 0;
                }

                MS_U8  *pu8SubCustomerBuf = NULL;
                // check if Sub_Color.bin exists in /Customer/pq/
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubColorBinName, strlen(SubColorBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                if(IS_ERR(pFile))
                {
                    // read from default path
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                    strncat(pm_PQBinFilePathName, SubColorBinName, strlen(SubColorBinName));
                    pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    if (!IS_ERR(pFile))
                    {
                        filp_close(pFile, NULL);
                        pFile = NULL;
                        pu8SubCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                    else
                    {
                        pFile = NULL;
                    }
                }
                else
                {
                    // read from /Customer/pq
                    filp_close(pFile, NULL);
                    pFile = NULL;
                    pu8SubCustomerBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }

                if(pu8MainCustomerBuf && pu8SubCustomerBuf)
                {
                    printk("Load PQ bin file, PQ_BIN_CUSTOMER_SUB path [%s]\n", pm_PQBinFilePathName);
                    pstPQInitInfo->u8PQBinCustomerCnt = 2;
                    pstPQInitInfo->stPQBinInfo[5].u8PQID = (MS_U8) PQ_BIN_CUSTOMER_SUB;
                    pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr = (MS_PHYADDR) pu8SubCustomerBuf;
                }
                else
                {
                     pstPQInitInfo->u8PQBinCustomerCnt = 0;
                    printf( " !!Error Load PQ bin file, PQ_BIN_CUSTOMER_MAIN path [%s]\n", pm_PQBinFilePathName);
                }
            }
#if (ENABLE_PQ_BIN_CF)
                MS_U8 *pu8SubCFBuf = NULL;
                // if Sub_CF.bin exists in /Customer/pq, read it
                memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                strncpy(pm_PQBinFilePathName, CustomerPath, strlen(CustomerPath));
                strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                if(IS_ERR(pFile))
                {
                    memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                    strncpy(pm_PQBinFilePathName, IniPath, strlen(IniPath));
                    strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                    pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    if(IS_ERR(pFile))
                    {
                        // read from default path
                        memset(pm_PQBinFilePathName, 0, PQ_FILE_NAME_LENGTH);
                        strncpy(pm_PQBinFilePathName, DefaltPath, strlen(DefaltPath));
                        strncat(pm_PQBinFilePathName, SubCFBinName, strlen(SubCFBinName));
                        pFile = filp_open(pm_PQBinFilePathName, O_RDONLY, 0);
                    }
                    if(IS_ERR(pFile))
                    {
                        pFile = NULL;
                    }
                    else
                    {
                        filp_close(pFile, NULL);
                        pFile = NULL;
                        pu8SubCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                    }
                }
                else
                {
                    // read from /Customer/pq
                    filp_close(pFile, NULL);
                    pFile = NULL;
                    pu8SubCFBuf = Open_PQ_Bin((MS_U8 *)pm_PQBinFilePathName);
                }

                if(pu8SubCFBuf)
                {
                    printk("Load PQ bin file, PQ_BIN_CF_SUB path [%s]\n", pm_PQBinFilePathName);
                    pstPQInitInfo->u8PQBinCFCnt = 2;
                    pstPQInitInfo->stPQBinInfo[8].u8PQID = (MS_U8) PQ_BIN_CF_SUB;
                    pstPQInitInfo->stPQBinInfo[8].PQBin_PhyAddr = (MS_PHYADDR) pu8SubCFBuf;
                }
#endif
        }
#else
        pstPQInitInfo->u8PQBinCnt = 0;
        pstPQInitInfo->u8PQTextBinCnt = 0;
        pstPQInitInfo->u8PQBinCustomerCnt = 0;
        pstPQInitInfo->u8PQBinUFSCCnt = 0;
#if (ENABLE_PQ_BIN_CF)
        pstPQInitInfo->u8PQBinCFCnt = 0;
#endif
#endif
    }
    MDrv_PQ_Release_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
}
#endif

void MDrv_PQ_Init_U2(void* pInstance,MS_PQ_Init_Info *pstPQInitInfo)
{
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(UFO_XC_DS_PQ))
    if(MApi_XC_Is_SupportSWDS())
    {
        printf("Mdrv_PQ_Init for PQ-DS, ignore!\n");
        return;
    }
#endif

#if (defined(MSOS_TYPE_LINUX_KERNEL))
    //Because _status.bIsInitialized is Global variable,
    //In linux kernel driver only one instance.
    //We must return here to avoid re-init write other global variable to init value
    if(_status.bIsInitialized == TRUE)
    {
        printk("_status.bIsInitialized %d, return.\033[0m\n",__FUNCTION__,__LINE__,_status.bIsInitialized);
        return;
    }
#endif

    PQTABLE_INFO PQTableInfo;
    MS_U8 i;
    MS_U8 u8ID;

    _s32LastPQIPMADiMain = -1;
    _s32LastPQIPMemFormatMain = -1;
    _s32LastPQIPHSDYMain = -1;
    _s32LastPQIPHSDCMain = -1;
    _s32LastPQIPMADiSub = -1;
    _s32LastPQIPMemFormatSub = -1;
    _s32LastPQIPMADiMotMain = -1;
    _s32LastPQIPMADiMotSub = -1;

    _bCustomerMainPQEn = FALSE ;
    _bCustomerSubPQEn = FALSE ;
    _bUFSCPQEn = FALSE;
    _u16LastPQSrcType[PQ_MAIN_WINDOW] = 0xFFFF;
    _u16LastPQSrcType[PQ_SUB_WINDOW] = 0xFFFF;
    _bNeedRestoreWRBankMappingNum = FALSE;

    _PQ_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_Mutex", MSOS_PROCESS_SHARED);

    if(_PQ_Mutex == -1)
    {
        (printf("[MAPI PQ][%06d] create mutex fail\n", __LINE__));
        return;
    }

#ifdef UFO_XC_PQ_SUPPORT_INITVAR_INTO_SHAREMEMORY
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    MDrv_PQ_Get_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
    UtopiaResourceGetPrivate(g_pPQResource[E_PQ_DEVICE_ID_0],(void**)(&pPQResourcePrivate));

    memcpy(&pPQResourcePrivate->PQ_InitInfo, pstPQInitInfo, sizeof(MS_PQ_Init_Info));

    MDrv_PQ_Release_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
#endif

    memset(&PQTableInfo, 0, sizeof(PQTableInfo));

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    PTH_PQ_RET_CHK(pthread_mutexattr_t attr);
    PTH_PQ_RET_CHK(pthread_mutexattr_init(&attr));
    PTH_PQ_RET_CHK(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    PTH_PQ_RET_CHK(pthread_mutex_init(&_PQ_MLoad_Mutex, &attr));
#endif
#endif

    _status.bIsInitialized = TRUE;
    _status.bIsRunning     = TRUE;

    _MDrv_PQ_LoadPQBin(pInstance,pstPQInitInfo);

    //pstPQInitInfo->u8PQTextBinCnt = 0;
    if(pstPQInitInfo->u8PQTextBinCnt)
    {
        gbEnablePQTextBin = 1;
        for(i = 0; i < pstPQInitInfo->u8PQTextBinCnt; i++)
        {
            u8ID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
            stPQTextBinHeaderInfo[u8ID].u8BinID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
            //This is just for NonOS , Linux kernel driver shouldn't in this case, so marked here.
            //stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
#else
            stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
#endif
            MDrv_PQTextBin_Parsing(pInstance,&stPQTextBinHeaderInfo[u8ID]);
        }
    }
    else
    {
        //printf("NonPQBin_Text !!\n");
        gbEnablePQTextBin = 0;
    }
    //pstPQInitInfo->u8PQBinCnt = 0;
    if(pstPQInitInfo->u8PQBinCnt)
    {
        gbPQBinEnable = 1;
        MDrv_PQ_Init_DisplayType_U2(pInstance, pstPQInitInfo->u16PnlWidth, PQ_BIN_DISPLAY_ONE);
        //because main and sub's common table may be different,
        //so we will take main's common table as correct one.
        //so here load sub first and then use main's overwrite sub's
        //Sub first
        for(i = 2; i < pstPQInitInfo->u8PQBinCnt; i++)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
            //stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(pInstance,&stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance), &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif
        }

        if(pstPQInitInfo->u8PQBinCustomerCnt == 2)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[5].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[5].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr);
           // stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(pInstance,&stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif
            _bCustomerSubPQEn = TRUE ;
        }


        //Main second
        for(i = 0; i < ((pstPQInitInfo->u8PQBinCnt <= 2) ? pstPQInitInfo->u8PQBinCnt : 2); i++)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
           // stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(pInstance,&stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance), &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif
        }

        if(pstPQInitInfo->u8PQBinCustomerCnt >= 1)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[4].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[4].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr);
            //stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(pInstance,&stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif
            _bCustomerMainPQEn = TRUE ;
        }

        //UFSC
        if(pstPQInitInfo->u8PQBinUFSCCnt == 1)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[6].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[6].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[6].PQBin_PhyAddr);
            //stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[6].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[6].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(pInstance, &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance, PQ_BIN_UFSC_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif

            _bUFSCPQEn = TRUE ;
        }
#if (ENABLE_PQ_BIN_CF)
        //CF
        if(pstPQInitInfo->u8PQBinCFCnt >= 1)
        {
            _bCFMainPQEn = TRUE ;

            if(pstPQInitInfo->u8PQBinCFCnt > 1)
            _bCFSubPQEn = TRUE ;

            for(i = 7; i < ((pstPQInitInfo->u8PQBinCFCnt <= 2) ? 7+pstPQInitInfo->u8PQBinCFCnt : 9); i++)
            {
                u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
                stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
                stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
                //stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#else
                stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#endif
                MDrv_PQBin_Parsing(pInstance,&stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            if(!MApi_XC_Is_SupportSWDS())
            {
#endif
                MDrv_PQBin_LoadCommTable(pInstance,PQ_BIN_CF_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
#if (defined(UFO_XC_DS_PQ))
            }
#endif

           }
        }
#endif
    }
    else
    {
        //printf("NonPQBin !! \n");
        gbPQBinEnable = 0;
        MDrv_PQ_Init_DisplayType_U2(pInstance, pstPQInitInfo->u16PnlWidth, PQ_DISPLAY_ONE);

#if(ENABLE_PQ_LOAD_TABLE_INFO)
        MDrv_PQ_PreInitLoadTableInfo(MAIN,pInstance);
        MDrv_PQ_PreInitLoadTableInfo(MAINEX,pInstance);

#if PQ_ENABLE_PIP
        MDrv_PQ_PreInitLoadTableInfo(SUB,pInstance);
        MDrv_PQ_PreInitLoadTableInfo(SUBEX,pInstance);
#endif

#endif
    }
#if PQ_ENABLE_PIP
    //dump sub c_sram3, c_sram4 to avoid garbage when enable pip, add these two into skip rule
#if (defined(UFO_XC_DS_PQ))
        if(!MApi_XC_Is_SupportSWDS())
        {
#endif

#ifdef PQ_PIP_C_SRAM_HW_WITH_SAME_INTERFACE
//If 4 c_sram with sram read/write interface(HW artitecture)
//We can not write c_sram1 when c_sram3 is reading(using)
//HW suggest load all c_sram when PQ initialize
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_C121_Main, PQ_IP_C_SRAM1_Main);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_C_SRAM2_C2121_Main, PQ_IP_C_SRAM2_Main);
#endif
            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_C_SRAM3_C121_Sub, PQ_IP_C_SRAM3_Sub);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_C_SRAM4_C2121_Sub, PQ_IP_C_SRAM4_Sub);
#if (defined(UFO_XC_DS_PQ))
        }
#endif
#endif


#if 1
    // Set BK12 [40] [1:0] = b'11 to enable hw double buffer write
    // at blanking area for changing memory format by L_BK_SCMI(0x02)
    // this register is added after T2 U04
    Hal_PQ_set_memfmt_doublebuffer(pInstance,ENABLE);

    memcpy(&_stPQ_Info, pstPQInitInfo, sizeof(MS_PQ_Init_Info));

    memset(&bw_info, 0x00, sizeof(bw_info));
    bw_info.bDDR2 = pstPQInitInfo->bDDR2;
    bw_info.u32DDRFreq = pstPQInitInfo->u32DDRFreq;
    bw_info.u8BusWidth = pstPQInitInfo->u8BusWidth;
    bw_info.u16output_vtotal = pstPQInitInfo->u16Pnl_vtotal;
    bw_info.u16output_hsize = pstPQInitInfo->u16PnlWidth;
    bw_info.u16OSD_hsize = pstPQInitInfo->u16OSD_hsize;
    bw_info.u32Miu0MemSize = pstPQInitInfo->u32miu0em_size;
    bw_info.u32Miu1MemSize = pstPQInitInfo->u32miu1em_size;

    for(i = 0; i < PQ_MAX_WINDOW; i++)
    {
        _bColorRange0_255[i] = TRUE;
        _bGameModeOn[i] = FALSE;
        _u8RWmappingNum[i] = 0;
        _bRWNumIsReset[i] = TRUE;
        _u16PQSuggestedFrameNum[i] = 0;
    }
#endif
#if 0//defined(ANDROID) && defined(UFO_XC_DS_PQ)
    if(MApi_XC_Is_SupportSWDS())
    {
        //PQ_Function_Info* function_Info = NULL;
        PQ_Function_Info function_Info;
        memset(&function_Info, 0, sizeof(function_Info));
        MS_U32 u32InitDataLen = 0;
        function_Info.pq_deside_srctype = MDrv_PQ_DesideSrcType;
        //function_Info.pq_disable_filmmode = MDrv_PQ_DisableFilmMode;
        function_Info.pq_load_scalingtable = MDrv_PQ_LoadScalingTable;
        //function_Info.pq_set_420upsampling = MDrv_PQ_Set420upsampling;
        //function_Info.pq_set_csc = MDrv_PQ_SetCSC;
        //function_Info.pq_set_memformat = MDrv_PQ_SetMemFormat;
        function_Info.pq_set_modeInfo = MDrv_PQ_Set_ModeInfo;
        function_Info.pq_get_memyuvfmt = MDrv_PQ_Get_MemYUVFmt;
#if(PQ_ENABLE_IOCTL)
        //function_Info.pq_ioctl = MDrv_PQ_IOCTL;
#endif
        MApi_XC_PQ_LoadFunction(&function_Info, u32InitDataLen);
    }
#endif



}
#if (defined  (ANDROID) && defined (UFO_XC_DS_PQ))
extern void* pu32XCInst;
#endif
void MDrv_PQ_Init(MS_PQ_Init_Info *pstPQInitInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return;
#endif
#if (defined (ANDROID) && defined (UFO_XC_DS_PQ))
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }
    static XC_INITMISC tmp_Init_Misc;
    stXC_GET_MISC_STATUS XCArgs1;
    XCArgs1.pXC_Init_Misc = &tmp_Init_Misc;
    XCArgs1.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs1) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }

    if(!(tmp_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID))
    {
        tmp_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_IS_ANDROID;

        stXC_INIT_MISC XCArgs2;
        XCArgs2.pXC_Init_Misc = &tmp_Init_Misc;
        XCArgs2.u32InitMiscDataLen = sizeof(XC_INITMISC);
        XCArgs2.eReturnValue = E_APIXC_RET_FAIL;

        if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT_MISC, (void*)&XCArgs2) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Obtain XC engine fail\n");
            return;
        }
    }
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
        printf("\33[1;33m[MARCOS](%s %d) OPEN FINE\33[m\n", __FUNCTION__, __LINE__);
    }

    stPQ_INIT PQArgs;
    PQArgs.pstPQInitInfo = pstPQInitInfo;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_INIT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
#if (defined  (ANDROID) && defined (UFO_XC_DS_PQ))
        stXC_GET_MISC_STATUS XCArgs3;
        XCArgs3.pXC_Init_Misc = &tmp_Init_Misc;
        XCArgs3.eReturnValue = E_APIXC_RET_FAIL;

        if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs3) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Obtain XC engine fail\n");
            return;
        }

        if(tmp_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID)
        {
            tmp_Init_Misc.u32MISC_A = tmp_Init_Misc.u32MISC_A & (~E_XC_INIT_MISC_A_IS_ANDROID);
            stXC_INIT_MISC XCArgs4;
            XCArgs4.pXC_Init_Misc = &tmp_Init_Misc;
            XCArgs4.u32InitMiscDataLen = sizeof(XC_INITMISC);
            XCArgs4.eReturnValue = E_APIXC_RET_FAIL;

            if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT_MISC, (void*)&XCArgs4) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain XC engine fail\n");
                return;
            }
        }
#endif
        return ;
    }
}

MS_BOOL MDrv_PQ_GetConfig_U2(void* pInstance, MS_PQ_Init_Info *pstPQInitInfo)
{
#ifdef UFO_XC_PQ_SUPPORT_INITVAR_INTO_SHAREMEMORY
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    if(_status.bIsInitialized == TRUE)
    {
        MDrv_PQ_Get_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
        UtopiaResourceGetPrivate(g_pPQResource[E_PQ_DEVICE_ID_0],(void**)(&pPQResourcePrivate));

        memcpy(pstPQInitInfo, &pPQResourcePrivate->PQ_InitInfo, sizeof(MS_PQ_Init_Info));

        MDrv_PQ_Release_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);

        return E_PQ_RET_OK;
    }
    else
    {
        return E_PQ_RET_FAIL;
    }
#else
    return E_PQ_RET_FAIL;
#endif
}

MS_BOOL MDrv_PQ_GetConfig(MS_PQ_Init_Info *pstPQInitInfo)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_GetConfig PQArgs;
    PQArgs.pstPQInitInfo = pstPQInitInfo;
    PQArgs.eRet = E_PQ_RET_FAIL;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GETCONFIG, (void*)&PQArgs) != 0)
    {
        printf("PQ engine GET_CONFIG Ioctl fail\n");
        return E_PQ_RET_FAIL;
    }

    if(E_PQ_RET_OK == PQArgs.eRet)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_PQ_Exit_U2(void* pInstance)
{
    return true;
}
MS_BOOL MDrv_PQ_Exit(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_EXIT, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static const MS_S16 S16DACColorCorrectionMatrix[32] =
{
    0x0400, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000,
    0x0400, -0x02E6, 0x0288, -0x05BB, 0x07A4, -0x062C, 0x06F3, -0x073C,
    -0x0024, 0x01BF, 0x07EF, -0x0116, 0x01EE, 0x052C, -0x03BB, 0x00B1,
    -0x0831, 0x0100, -0x0000, 0x0000, -0x0000, 0x0000, 0x0000, 0x0000,
};

MS_U32 MDrv_PQ_SetPowerState_U2(void* pInstance,EN_POWER_MODE enPowerState)
{
    MS_U32 u32Ret = E_PQ_RET_FAIL;
    MS_U8 i = 0;
    MS_U8 u8ID = 0;

    if (enPowerState == E_POWER_SUSPEND)
    {
        MDrv_PQ_Str_Reset_Sram_Table(MAIN,pInstance);
        _enPrevPowerState = enPowerState;
        SC_BK_RESTORE_MUTEX;        //while STR,PQ get chacne to lock mutex without return it. THerefore, force return before suspend.
        u32Ret = E_PQ_RET_OK;
    }
    else if (enPowerState == E_POWER_RESUME)
    {
        _enPrevPowerState = enPowerState;

        _s32LastPQIPMADiMain = -1;
        _s32LastPQIPMemFormatMain = -1;
        _s32LastPQIPHSDYMain = -1;
        _s32LastPQIPHSDCMain = -1;
        _s32LastPQIPMADiSub = -1;
        _s32LastPQIPMemFormatSub = -1;
        _s32LastPQIPMADiMotMain = -1;
        _s32LastPQIPMADiMotSub = -1;

        _u16LastPQSrcType[PQ_MAIN_WINDOW] = 0xFFFF;
        _u16LastPQSrcType[PQ_SUB_WINDOW] = 0xFFFF;

        if(_stPQ_Info.u8PQBinCnt)
        {
            //Sub first
            for(i = 2; i < _stPQ_Info.u8PQBinCnt; i++)
            {
                u8ID = _stPQ_Info.stPQBinInfo[i].u8PQID;
                MDrv_PQBin_LoadCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance), &stPQBinHeaderInfo[u8ID]);
            }
            if(_stPQ_Info.u8PQBinCustomerCnt == 2)
            {
                u8ID = _stPQ_Info.stPQBinInfo[5].u8PQID;
                MDrv_PQBin_LoadCommTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
            }

            //Main second
            for(i = 0; i < ((_stPQ_Info.u8PQBinCnt <= 2) ? _stPQ_Info.u8PQBinCnt : 2); i++)
            {
                u8ID = _stPQ_Info.stPQBinInfo[i].u8PQID;
                MDrv_PQBin_LoadCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance), &stPQBinHeaderInfo[u8ID]);
            }

            if(_stPQ_Info.u8PQBinCustomerCnt >= 1)
            {
                u8ID = _stPQ_Info.stPQBinInfo[4].u8PQID;
                MDrv_PQBin_LoadCommTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
            }

            //UFSC
            if(_stPQ_Info.u8PQBinUFSCCnt == 1)
            {
                u8ID = _stPQ_Info.stPQBinInfo[6].u8PQID;
                MDrv_PQBin_LoadCommTable(pInstance, PQ_BIN_UFSC_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
            }
        }
        else
        {
#if PQ_ENABLE_PIP
            MDrv_PQ_LoadCommTable_(SUB,pInstance);
#if (ENABLE_PQ_EX)
            MDrv_PQ_LoadCommTable_(SUBEX,pInstance);
#endif
#endif
            MDrv_PQ_LoadCommTable_(MAIN,pInstance);
#if (ENABLE_PQ_EX)
            MDrv_PQ_LoadCommTable_(MAINEX,pInstance);
#endif
        }

#if PQ_ENABLE_PIP
        //dump sub c_sram3, c_sram4 to avoid garbage when enable pip, add these two into skip rule
        MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_C_SRAM3_C121_Sub, PQ_IP_C_SRAM3_Sub);
        MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_C_SRAM4_C2121_Sub, PQ_IP_C_SRAM4_Sub);
#endif

        // Set BK12 [40] [1:0] = b'11 to enable hw double buffer write
        // at blanking area for changing memory format by L_BK_SCMI(0x02)
        // this register is added after T2 U04
        Hal_PQ_set_memfmt_doublebuffer(pInstance,ENABLE);

        //common bw table
        MDrv_BW_LoadInitTable();
        u32Ret = E_PQ_RET_OK;
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %u\n",__FUNCTION__,__LINE__,enPowerState);
        u32Ret = E_PQ_RET_FAIL;
    }
    return u32Ret;
}

MS_U32 MDrv_PQ_SetPowerState(EN_POWER_MODE enPowerState)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetPowerState PQArgs;
    PQArgs.enPowerState = enPowerState;
    PQArgs.u32Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_POWER_STATE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u32Ret;
    }
}

MS_U16 MDrv_PQ_GetIPNum_U2(void* pInstance,PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(pInstance,&stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            return MDrv_PQ_GetIPNum_(MAIN,pInstance);
        }
    }

#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(pInstance,&stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            return MDrv_PQ_GetIPNum_(SUB,pInstance);
        }
    }
#endif

    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}
MS_U16 MDrv_PQ_GetIPNum(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetIPNum PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_IP_NUM, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetTableNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetTableNum(pInstance,u16PQIPIdx, &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            return MDrv_PQ_GetTableNum_(MAIN,pInstance, (MS_U8)u16PQIPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetTableNum(pInstance,u16PQIPIdx, &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            return MDrv_PQ_GetTableNum_(SUB,pInstance, (MS_U8)u16PQIPIdx);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}
MS_U16 MDrv_PQ_GetTableNum(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetTableNum PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_TABLE_NUM, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetCurrentTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),  u16PQIPIdx);
        }
        else
        {
            return MDrv_PQ_GetCurrentTableIndex_(MAIN,pInstance, (MS_U8)u16PQIPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),  u16PQIPIdx);
        }
        else
        {
            return MDrv_PQ_GetCurrentTableIndex_(SUB,pInstance, (MS_U8)u16PQIPIdx);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}
MS_U16 MDrv_PQ_GetCurrentTableIndex(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetCurrentTableIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CURRENT_TABLE_INDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}
//Get main page setting from u16IPIdx(column), according to current input source type(row)
MS_U16 MDrv_PQ_GetTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx)
{
    MS_U8 u16TabIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(pInstance),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16TabIdx = MDrv_PQ_GetTableIndex_(MAIN,pInstance, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(pInstance),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16TabIdx = MDrv_PQ_GetTableIndex_(SUB,pInstance, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}
MS_U16 MDrv_PQ_GetTableIndex(PQ_WIN eWindow, MS_U16 u16IPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetTableIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16PQIPIdx = u16IPIdx;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_TABLE_INDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}


//Get main page setting from u16IPIdx(column), according to current input source type(row) by ePQEngineType
MS_U16 MDrv_PQ_GetTableIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 u16TabIdx = PQ_BIN_IP_NULL, u16SrcIdx = 0;
    MS_U8 u8PanelIdx = 0;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                u16SrcIdx = _u16PQSrcType[eWindow];
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_MAIN;
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetTableIndex_(MAIN,pInstance, u16SrcIdx, (MS_U8)u16IPIdx);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
                }
                break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
                {
                    u16SrcIdx = _u16PQSrcType_UFSC[eWindow];
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx_UFSC(pInstance);
                    enPQBinType = PQ_BIN_UFSC;
                }
                break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                u16SrcIdx = _u16PQSrcType[eWindow];
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetTableIndex_(SUB,pInstance, u16SrcIdx, (MS_U8)u16IPIdx);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_SUB;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
                }
                break;
            case PQ_XC0_UFSC:
                break;
            default:
                break;
        }

    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,
                                            u16SrcIdx,
                                            u16IPIdx,
                                            u8PanelIdx,
                                            &stPQBinHeaderInfo[enPQBinType]);
    }

    return u16TabIdx;
}

//Get main page setting from u16IPIdx(column), according to current input source type(row) by ePQEngineType
MS_U16 MDrv_PQ_GetTableIndex_Ex(PQ_WIN eWindow, MS_U16 u16IPIdx, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetTableIndex_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16IPIdx = u16IPIdx;
    PQArgs.ePQEngineType = ePQEngineType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_TABLE_INDEX_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

//Get main page setting from u16IPIdx(column), according to current input source type(row)
MS_U16 MDrv_PQ_GetCustomerTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx)
{
    MS_U8 u16TabIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 PQ_BIN_CUSTOMER_PANEL_INDEX,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 PQ_BIN_CUSTOMER_PANEL_INDEX,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}

MS_U16 MDrv_PQ_GetCustomerTableIndex(PQ_WIN eWindow, MS_U16 u16IPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetCustomerTableIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16PQIPIdx = u16IPIdx;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CUSTOMER_TABLE_INDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}
static MS_U16 MDrv_PQ_GetXRuleIPIndex(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP)
//MS_U16 MDrv_PQ_GetXRuleIPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP)
{
    MS_U8 u16IPIdx = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(pInstance,u16XRuleType,
                                                  u16XRuleIP,
                                                  &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(MAIN,pInstance, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(pInstance,u16XRuleType,
                                                  u16XRuleIP,
                                                  &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(SUB,pInstance, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPIdx;
}
//static MS_U16 MDrv_PQ_GetXRuleIPIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP)
//{
//#ifdef TRACE_DBG
//        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
//#endif
//    if (pu32PQInst == NULL)
//    {
//        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
//        return FALSE;
//    }
//
//    stPQ_GetXRuleIPIndex PQArgs;
//    PQArgs.eWindow = eWindow;
//    PQArgs.u16XRuleType = u16XRuleType;
//    PQArgs.u16XRuleIP = u16XRuleIP;
//    PQArgs.u16Ret = 0;
//    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_XRULE_IP_INDEX, (void*)&PQArgs) != 0)
//    {
//        printf("Obtain PQ engine fail\n");
//        return 0;
//    }
//    else
//    {
//        return PQArgs.u16Ret;
//    }
//}

static MS_U16 MDrv_PQ_GetXRuleTableIndex(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP)
//MS_U16 MDrv_PQ_GetXRuleTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP)
{
    MS_U16 u16TabIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(pInstance,u16XRuleType,
                        u16XRuleIdx,
                        u16XRuleIP,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(MAIN,pInstance, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(pInstance,u16XRuleType,
                        u16XRuleIdx,
                        u16XRuleIP,
                        &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(SUB,pInstance, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}
//static MS_U16 MDrv_PQ_GetXRuleTableIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP)
//{
//#ifdef TRACE_DBG
//        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
//#endif
//    if (pu32PQInst == NULL)
//    {
//        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
//        return FALSE;
//    }
//
//    stPQ_GetXRuleTableIndex PQArgs;
//    PQArgs.eWindow = eWindow;
//    PQArgs.u16XRuleType = u16XRuleType;
//    PQArgs.u16XRuleIdx = u16XRuleIdx;
//    PQArgs.u16XRuleIP = u16XRuleIP;
//    PQArgs.u16Ret = 0;
//    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_XRULE_TABLE_INDEX, (void*)&PQArgs) != 0)
//    {
//        printf("Obtain PQ engine fail\n");
//        return 0;
//    }
//    else
//    {
//        return PQArgs.u16Ret;
//    }
//}

static MS_U16 MDrv_PQ_GetXRuleIPNum(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType)
//MS_U16 MDrv_PQ_GetXRuleIPNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType)
{
    MS_U16 u16IPNum = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum(pInstance,u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(MAIN,pInstance, (MS_U8)u16XRuleType);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum(pInstance,u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(SUB,pInstance, (MS_U8)u16XRuleType);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPNum;
}
//static MS_U16 MDrv_PQ_GetXRuleIPNum(PQ_WIN eWindow, MS_U16 u16XRuleType)
//{
//#ifdef TRACE_DBG
//        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
//#endif
//    if (pu32PQInst == NULL)
//    {
//        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
//        return FALSE;
//    }
//
//    stPQ_GetXRuleIPNum PQArgs;
//    PQArgs.eWindow = eWindow;
//    PQArgs.u16XRuleType = u16XRuleType;
//    PQArgs.u16Ret = 0;
//    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_XRULE_IP_NUM, (void*)&PQArgs) != 0)
//    {
//        printf("Obtain PQ engine fail\n");
//        return 0;
//    }
//    else
//    {
//        return PQArgs.u16Ret;
//    }
//}
void MDrv_PQ_LoadTableData_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                     u16TabIdx,
                                     u16PQIPIdx,
                                     &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                     pTable,
                                     u16TableSize);
        }
        else
        {
            MDrv_PQ_LoadTableData_(MAIN,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                     u16TabIdx,
                                     u16PQIPIdx,
                                     &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                     pTable,
                                     u16TableSize);
        }
        else
        {
            MDrv_PQ_LoadTableData_(SUB,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_LoadTableData(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadTableData PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx = u16TabIdx;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    PQArgs.pTable = pTable;
    PQArgs.u16TableSize = u16TableSize;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_TABLE_DATA, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_CloneTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQMainIPIdx, MS_U16 u16PQSubIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_CloneTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                 u16TabIdx,
                                 u16PQMainIPIdx,
                                 u16PQSubIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            MDrv_PQ_CloneTable_(MAIN,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQMainIPIdx, (MS_U8)u16PQSubIPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        //not support sub
        MS_ASSERT(0);
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_CloneTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQMainIPIdx, MS_U16 u16PQSubIPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_CloneTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx = u16TabIdx;
    PQArgs.u16PQMainIPIdx = u16PQMainIPIdx;
    PQArgs.u16PQSubIPIdx = u16PQSubIPIdx;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CLONE_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_LoadTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        switch (u16PQIPIdx)
        {
            case PQ_IP_MADi_Main:
                if (_s32LastPQIPMADiMain == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMADiMain = u16TabIdx;

                break;
#if (PQ_MADI_DFK == 1)
                case PQ_IP_MADi_Motion_Main:
                if (_s32LastPQIPMADiMotMain == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMADiMotMain = u16TabIdx;

                break;
#endif
            case PQ_IP_MemFormat_Main:
                if (_s32LastPQIPMemFormatMain == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMemFormatMain = u16TabIdx;
                break;
            case PQ_IP_HSD_Y_Main:
                if (_s32LastPQIPHSDYMain == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPHSDYMain = u16TabIdx;
                break;
            case PQ_IP_HSD_C_Main:
                if (_s32LastPQIPHSDCMain == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPHSDCMain = u16TabIdx;
                break;
            default:
                break;
         }
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],eWindow);
        }
        else
        {
            MDrv_PQ_LoadTable_(MAIN,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        switch (u16PQIPIdx)
        {
            case PQ_IP_MADi_Sub:
                if (_s32LastPQIPMADiSub == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMADiSub = u16TabIdx;
                break;
            case PQ_IP_MADi_Motion_Sub:
                if (_s32LastPQIPMADiMotSub == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMADiMotSub = u16TabIdx;
                break;
            case PQ_IP_MemFormat_Sub:
                if (_s32LastPQIPMemFormatSub == (MS_S32)u16TabIdx)
                {
                    return;
                }
                _s32LastPQIPMemFormatSub = u16TabIdx;
                break;
            default:
                break;
         }
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB],eWindow);
        }
        else
        {
            MDrv_PQ_LoadTable_(SUB,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_LoadTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx = u16TabIdx;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_LoadCustomerTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX,
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN],eWindow);
        }
        else
        {
            MDrv_PQ_LoadTable_(MAIN,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTable(pInstance,PQ_BIN_CUSTOMER_PANEL_INDEX,
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB],eWindow);
        }
        else
        {
            MDrv_PQ_LoadTable_(SUB,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_LoadCustomerTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadCustomerTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx = u16TabIdx;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_CUSTOMER_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_LoadTable_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U8 u8PanelIdx = 0;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                switch (u16PQIPIdx)
                {
                    case PQ_IP_MADi_Main:
                        if (_s32LastPQIPMADiMain == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMADiMain = u16TabIdx;
                        break;
#if (PQ_MADI_DFK == 1)
                        case PQ_IP_MADi_Motion_Main:
                        if (_s32LastPQIPMADiMotMain == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMADiMotMain = u16TabIdx;
                        break;
#endif
                    case PQ_IP_MemFormat_Main:
                        if (_s32LastPQIPMemFormatMain == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMemFormatMain = u16TabIdx;
                        break;
                    case PQ_IP_HSD_Y_Main:
                        if (_s32LastPQIPHSDYMain == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPHSDYMain = u16TabIdx;
                        break;
                    case PQ_IP_HSD_C_Main:
                        if (_s32LastPQIPHSDCMain == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPHSDCMain = u16TabIdx;
                        break;
                    default:
                        break;
                }
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_MAIN;
                }
                else
                {
                    MDrv_PQ_LoadTable_(MAIN, pInstance,(MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
                }
                break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
                {
                    u8PanelIdx = PQ_BIN_UFSC_PANEL_INDEX;
                    enPQBinType = PQ_BIN_UFSC;
                }
                break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                switch (u16PQIPIdx)
                {
                    case PQ_IP_MADi_Sub:
                        if (_s32LastPQIPMADiSub == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMADiSub = u16TabIdx;
                        break;
                    case PQ_IP_MADi_Motion_Sub:
                        if (_s32LastPQIPMADiMotSub == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMADiMotSub = u16TabIdx;
                        break;
                    case PQ_IP_MemFormat_Sub:
                        if (_s32LastPQIPMemFormatSub == (MS_S32)u16TabIdx)
                        {
                            return;
                        }
                        _s32LastPQIPMemFormatSub = u16TabIdx;
                        break;
                    default:
                        break;
                }
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                else
                {
                    MDrv_PQ_LoadTable_(SUB,pInstance, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_SUB;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
                }
                break;
            case PQ_XC0_UFSC:
                break;
            default:
                break;
        }

    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        MDrv_PQBin_LoadTable(pInstance,
                            u8PanelIdx,
                            u16TabIdx,
                            u16PQIPIdx,
                            &stPQBinHeaderInfo[enPQBinType],eWindow);
    }
}

void MDrv_PQ_LoadTable_Ex(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadTable_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx = u16TabIdx;
    PQArgs.u16PQIPIdx = u16PQIPIdx;
    PQArgs.u16PQIPIdx = ePQEngineType;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_TABLE_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
    }
}

void MDrv_PQ_CheckSettings_U2(void* pInstance,PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            //ToDo MAINEX
            MDrv_PQBin_CheckCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);

            MDrv_PQBin_CheckTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(pInstance),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                           eWindow);
        }
        else
        {
#if(ENABLE_PQ_EX)
            MDrv_PQ_CheckCommTable_(MAINEX,pInstance);
            MDrv_PQ_CheckTableBySrcType_(MAINEX,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
            MDrv_PQ_CheckCommTable_(MAIN,pInstance);
            MDrv_PQ_CheckTableBySrcType_(MAIN,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            //ToDo SUBEX
            MDrv_PQBin_CheckCommTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);

            MDrv_PQBin_CheckTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(pInstance),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                           eWindow);
        }
        else
        {
        #if(ENABLE_PQ_EX)
            MDrv_PQ_CheckCommTable_(SUBEX,pInstance);
            MDrv_PQ_CheckTableBySrcType_(SUBEX,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
        #endif
            MDrv_PQ_CheckCommTable_(SUB,pInstance);
            MDrv_PQ_CheckTableBySrcType_(SUB,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_CheckSettings(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_CheckSettings PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CHECK_SETTINGS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


#if(ENABLE_PQ_MLOAD)
void MDrv_PQ_Set_MLoadEn_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEn)
{
    if(MApi_XC_MLoad_GetStatus() != E_MLOAD_ENABLED)
    {
        bEn = FALSE;
    }

    if(bEn)
    {
        PQ_MLOAD_ENTRY();
    }

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(pInstance,bEn);
        }
        else
        {
            MDrv_PQ_Set_MLoadEn_(MAIN,pInstance, bEn);
        }

    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(pInstance,bEn);
        }
        else
        {
            MDrv_PQ_Set_MLoadEn_(SUB,pInstance, bEn);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }


    if(bEn == FALSE)
    {
        PQ_MLOAD_RETURN();
    }
}
void MDrv_PQ_Set_MLoadEn(PQ_WIN eWindow, MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_MLoadEn PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEn = bEn;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_MLOAD_EN, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

MS_BOOL MDrv_PQ_SkipDuplicatedSetting_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bSkip)
{
    UNUSED(eWindow);
    _bSkipDuplicateSetting = bSkip;
    return TRUE;
}
MS_BOOL MDrv_PQ_SkipDuplicatedSetting(PQ_WIN eWindow, MS_BOOL bSkip)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return FALSE;
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SkipDuplicatedSetting PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bSkip = bSkip;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SKIP_DUPLICATED_SETTING, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetSkipDuplicatedSettingStatus_U2(void* pInstance,PQ_WIN eWindow)
{
    UNUSED(eWindow);
    return _bSkipDuplicateSetting;
}
MS_BOOL MDrv_PQ_GetSkipDuplicatedSettingStatus(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetSkipDuplicatedSettingStatus PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_SKIP_DUPLICATED_SETTING_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
void MDrv_PQ_SetFrcmCbCrSwap_U2(void* pInstance,PQ_WIN eWindow)
{
    // FRCM mirror: BK32_03[12](main), BK32_43[12](sub)
    // FRCM cbcy swap: BK31_19[0](main),BK31_59[0](sub)
    // Bypass CE(compression): BK33_01[0](main),BK33_01[1](sub), 0: ENABLE CE; 1: disable CE
    // --------------------------------------------------------------------------------------------
    // | regs         |  not h_mirror |  h_mirror without compression | h_mirror with compression |
    // | BK32_03[12]  |       0       |              1                |              1            |
    // | BK33_01[0]   |     0 or 1    |              1                |              0            |
    // | BK31_19[0]   |       0       |              1                |              0            |
    // --------------------------------------------------------------------------------------------

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if (MApi_XC_R2BYTEMSK(REG_SC_BK32_03_L, BIT(12)) == 0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK31_19_L, 0, BIT(0));
        }
        else
        {
            if (MApi_XC_R2BYTEMSK(REG_SC_BK33_01_L, BIT(0)) != 0)
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK31_19_L, BIT(0), BIT(0));
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK31_19_L, 0, BIT(0));
            }
        }
    }
    else
    {
        if (MApi_XC_R2BYTEMSK(REG_SC_BK32_43_L, BIT(12)) == 0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK31_59_L, 0, BIT(0));
        }
        else
        {
            if (MApi_XC_R2BYTEMSK(REG_SC_BK33_01_L, BIT(1)) != 0)
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK31_59_L, BIT(0), BIT(0));
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK31_59_L, 0, BIT(0));
            }
        }
    }
}
void MDrv_PQ_SetFrcmCbCrSwap(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetFrcmCbCrSwap PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_FRCM_CB_CR_SWAP, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

void MDrv_PQ_LoadSettings_U2(void* pInstance,PQ_WIN eWindow)
{
    if((_u16LastPQSrcType[eWindow] == _u16PQSrcType[eWindow]) && _bSkipDuplicateSetting)
    {
        PQTAB_DBG(printf("\t !!!!!!!!Skip PQ,[PQ timming = %d, eWindow = %d]\n",_u16PQSrcType[eWindow],eWindow));
        return;
    }

    _u16LastPQSrcType[eWindow] = _u16PQSrcType[eWindow];

    if(eWindow == PQ_MAIN_WINDOW)
    {
        //load swdriver here before all the others PQ settings.
        //Pls don't add swDriver in to skip rule, otherwise below code cannot set swDriver.
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_SwDriver_Main), PQ_IP_SwDriver_Main);

        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(pInstance),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                          eWindow);

            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(pInstance),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_MAIN],
                                          eWindow);

        }
        else
        {
            PQTAB_DBG(printf("MAIN table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAIN,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
#if(ENABLE_PQ_EX == 1)
            PQTAB_DBG(printf("MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAINEX,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
            PQTAB_DBG(printf("...done\n"));
        }

        if(_gIsMVC4kx1k == TRUE)
        {
#if PQ_LCE_CTL_FOR_MVC4KX1K
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_LCE_OFF_Main, PQ_IP_VIP_LCE_Main);
#endif
#if PQ_DLC_CTL_FOR_MVC4KX1K
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_DLC_OFF_Main, PQ_IP_VIP_DLC_Main);
#endif
        }

    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(pInstance),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                          eWindow);

            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(pInstance),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_SUB],
                                          eWindow);
        }
        else
        {
            PQTAB_DBG(printf("SUB table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUB,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
#if(ENABLE_PQ_EX == 1)
            PQTAB_DBG(printf("SUB MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUBEX,pInstance, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
            PQTAB_DBG(printf("...done\n"));
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

#if (PQ_IOCTL_SET_UC_FEATURE == 0)
   MDrv_PQ_IOCTL((PQ_WIN)eWindow,
                    E_IOCTL_SET_UCFEATURE,
                    NULL,
                    NULL);
#endif

#if(ENABLE_PQ_LOAD_TABLE_INFO)
    if(eWindow == PQ_MAIN_WINDOW)
    {
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_MAIN);
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_MAIN_EX);
    }
#if PQ_ENABLE_PIP
    else if(eWindow == PQ_SUB_WINDOW)
    {
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_SUB);
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_SUB_EX);
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
#endif
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
    MDrv_PQ_SetFrcmCbCrSwap_U2(pInstance, eWindow);
#endif

#ifdef PQ_DISABLE_BYPASS_MODE2_BY_SW
    if(MApi_XC_R2BYTE(REG_CHIP_REVISION) < 1)//Monaco U02 fix .
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK12_44_L, 0x00, BIT(4));  //Disable bypass mode2, for sub video I mode flicker issue
    }
#endif

}
void MDrv_PQ_LoadSettings(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadSettings PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_SETTINGS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_LoadCustomerSettings_U2(void* pInstance,PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          PQ_BIN_CUSTOMER_PANEL_INDEX,
                                          &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN],
                                          eWindow);

        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          PQ_BIN_CUSTOMER_PANEL_INDEX,
                                          &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB],
                                          eWindow);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}
void MDrv_PQ_LoadCustomerSettings(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadCustomerSettings PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_CUSTONER_SETTINGS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_Set_DTVInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo)
{
    memcpy(&_stDTV_Info[eWindow], pstPQDTVInfo, sizeof(MS_PQ_Dtv_Info));

    PQINFO_DBG(printf("PQ DTV Info:Win=%u, type=%u\n", eWindow, _stDTV_Info[eWindow].eType));
}
void MDrv_PQ_Set_DTVInfo(PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_DTVInfo PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pstPQDTVInfo = pstPQDTVInfo;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DTVINFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_Set_MultiMediaInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo)
{
    memcpy(&_stMultiMedia_Info[eWindow], pstPQMMInfo, sizeof(MS_PQ_MuliMedia_Info));
    PQINFO_DBG(printf("PQ MM Info:Win=%u, type=%u\n", eWindow, _stMultiMedia_Info[eWindow].eType));
}
void MDrv_PQ_Set_MultiMediaInfo(PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_MultiMediaInfo PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pstPQMMInfo = pstPQMMInfo;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_MULTIMEDIAINFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_Set_VDInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_Vd_Info *pstPQVDInfo)
{
    memcpy(&_stVD_Info[eWindow], pstPQVDInfo, sizeof(MS_PQ_Vd_Info));

    PQINFO_DBG(printf("PQ VD Info:Win=%u, SigType=%u, bSCARTRGB=%u, VIFIn=%u\n",
                      eWindow,
                      _stVD_Info[eWindow].enVideoStandard,
                      _stVD_Info[eWindow].bIsSCART_RGB,
                      _stVD_Info[eWindow].bIsVIFIN));

}
void MDrv_PQ_Set_VDInfo(PQ_WIN eWindow, MS_PQ_Vd_Info *pstPQVDInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_VDInfo PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pstPQVDInfo = pstPQVDInfo;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_VDINFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_Set_ModeInfo_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{

    memcpy(&_stMode_Info[eWindow], pstPQModeInfo, sizeof(MS_PQ_Mode_Info));

    //printf("PQ Set Mode Info: %s, Src=%x\n", (eWindow)?("SubWin"):("MainWin"), enInputSourceType);

    if(QM_IsSourceYPbPr(enInputSourceType))
    {
        if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 720))) &&
                (pstPQModeInfo->u16input_vsize < 500) &&
                (pstPQModeInfo->u16input_vfreq < 650))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x480_60I :
                                    PQ_MD_720x480_60P;

        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 720))) &&
                (pstPQModeInfo->u16input_vsize < 600) &&
                (pstPQModeInfo->u16input_vfreq < 550))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x576_50I :
                                    PQ_MD_720x576_50P;
        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 1280))) &&
                (pstPQModeInfo->u16input_vsize < 800) &&
                (pstPQModeInfo->bInterlace == FALSE))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                    PQ_MD_1280x720_50P :
                                    PQ_MD_1280x720_60P;

        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 1920))) &&
                (pstPQModeInfo->u16input_vsize < 1100))
        {
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                        PQ_MD_1920x1080_50I :
                                        PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 245) ?  PQ_MD_1920x1080_24P :
                                        (pstPQModeInfo->u16input_vfreq < 270) ?  PQ_MD_1920x1080_25P :
                                        (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                                        (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                                        PQ_MD_1920x1080_60P;

            }
        }
        else
        {
            _u8ModeIndex[eWindow] = PQ_MD_720x576_50I;
        }
    }
    else if(QM_IsSourceHDMI(enInputSourceType))
    {
        if((pstPQModeInfo->u16input_hsize < 1500) &&
                (pstPQModeInfo->u16input_vsize < 500) &&
                (pstPQModeInfo->u16input_vfreq < 650))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x480_60I :
                                    PQ_MD_720x480_60P;

        }
        else if((pstPQModeInfo->u16input_hsize < 1500) &&
                (pstPQModeInfo->u16input_vsize < 600) &&
                (pstPQModeInfo->u16input_vfreq <  550))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x576_50I :
                                    PQ_MD_720x576_50P;
        }
        else if((pstPQModeInfo->u16input_hsize < 1300) &&
                (pstPQModeInfo->u16input_vsize < 800) &&
                (pstPQModeInfo->bInterlace == FALSE))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                    PQ_MD_1280x720_50P :
                                    PQ_MD_1280x720_60P;

        }
        else if((pstPQModeInfo->u16input_hsize < 1930) &&
                (pstPQModeInfo->u16input_vsize < 1100))
        {
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                        PQ_MD_1920x1080_50I :
                                        PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 245) ?  PQ_MD_1920x1080_24P :
                                        (pstPQModeInfo->u16input_vfreq < 270) ?  PQ_MD_1920x1080_25P :
                                        (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                                        (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                                        PQ_MD_1920x1080_60P;

            }
        }
        else if(pstPQModeInfo->u16input_hsize < 1350 &&
                pstPQModeInfo->u16input_vsize < 1550)
        {
            //special handle 1280X1470p
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1280x720_50P :
                               PQ_MD_1280x720_60P;
            }
        }
        else if(pstPQModeInfo->u16input_hsize < 1930 &&
                pstPQModeInfo->u16input_vsize < 2300)
        {
            //special handle 1920X2205p
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                               PQ_MD_1920x1080_50I :
                               PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 250) ?  PQ_MD_1920x1080_24P :
                               (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                               (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                               PQ_MD_1920x1080_60P;

            }
        }
        else
        {
            _u8ModeIndex[eWindow] = PQ_MD_1920x1080_60P;
        }
    }
    else
    {
        _u8ModeIndex[eWindow] = PQ_MD_Num;
    }

    if(QM_IsSourceYPbPr(enInputSourceType) || QM_IsSourceHDMI(enInputSourceType))
    {
        if(_u8ModeIndex[eWindow] <= (MS_U8)PQ_MD_720x576_50P)
        _gIsSrcHDMode[eWindow] = 0;
        else
            _gIsSrcHDMode[eWindow] = 1;

    }
    else if(QM_IsSourceVD(enInputSourceType))
    {
            _gIsSrcHDMode[eWindow] = 0;
    }
    else
    {
        if(pstPQModeInfo->u16input_hsize >= 1200)
            _gIsSrcHDMode[eWindow] = 1;
        else
            _gIsSrcHDMode[eWindow] = 0;
            }

    PQINFO_DBG(
        printf("PQ ModeInfo:%d input(%d, %d), disp(%d, %d), ModeIdx=%d, FBL=%u, Interlace=%u, InV=%u, OutV=%u, inVtt=%u\n",
               eWindow,
               _stMode_Info[eWindow].u16input_hsize,
               _stMode_Info[eWindow].u16input_vsize,
               _stMode_Info[eWindow].u16display_hsize,
               _stMode_Info[eWindow].u16display_vsize,
               _u8ModeIndex[eWindow],
               _stMode_Info[eWindow].bFBL,
               _stMode_Info[eWindow].bInterlace,
               _stMode_Info[eWindow].u16input_vfreq,
               _stMode_Info[eWindow].u16ouput_vfreq,
               _stMode_Info[eWindow].u16input_vtotal);
    );
}
void MDrv_PQ_Set_ModeInfo(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_Set_ModeInfo PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enInputSourceType = enInputSourceType;
    PQArgs.pstPQModeInfo = pstPQModeInfo;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_MODEINFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_SetHDMIInfo_U2(void* pInstance,PQ_WIN eWindow, const MS_PQ_Hdmi_Info* const pstPQHDMIInfo)
{
    _stHDMI_Info[eWindow].bIsHDMI = pstPQHDMIInfo->bIsHDMI;
    _stHDMI_Info[eWindow].enColorFmt = pstPQHDMIInfo->enColorFmt;

    PQINFO_DBG(printf("PQ HDMI, bHDMI=%u, colorfmt=%u\n",
                      _stHDMI_Info[eWindow].bIsHDMI,
                      _stHDMI_Info[eWindow].enColorFmt));
}
void MDrv_PQ_SetHDMIInfo(PQ_WIN eWindow, const MS_PQ_Hdmi_Info* const pstPQHDMIInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetHDMIInfo PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pstPQHDMIInfo.bIsHDMI = pstPQHDMIInfo->bIsHDMI;
    PQArgs.pstPQHDMIInfo.bIsHDMIPC = pstPQHDMIInfo->bIsHDMIPC;
    PQArgs.pstPQHDMIInfo.enColorFmt = pstPQHDMIInfo->enColorFmt;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_HDMIINFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_SetHDMI_PC_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bIsTrue)
{
    _stHDMI_Info[eWindow].bIsHDMIPC = bIsTrue;
}
void MDrv_PQ_SetHDMI_PC(PQ_WIN eWindow, MS_BOOL bIsTrue)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetHDMI_PC PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bIsTrue = bIsTrue;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_HDMI_PC, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_GetHDMI_PC_Status_U2(void* pInstance,PQ_WIN eWindow)
{
    return _stHDMI_Info[eWindow].bIsHDMIPC;
}
MS_BOOL MDrv_PQ_GetHDMI_PC_Status(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetHDMI_PC_Status PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_HDMI_PC_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_DesideSrcType_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType)
{

    _enInputSourceType[eWindow] = enInputSourceType;
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(pInstance, eWindow, enInputSourceType);
    _u16PQSrcType_DBK_Detect[eWindow] = _u16PQSrcType[eWindow];                     //For Auto_DBK SW driver used
    PQTAB_DBG(printf("[PQ_DesideSrcType] window=%u, enInputSrcType=%u, SrcType=%u\n",
                     eWindow, enInputSourceType, _u16PQSrcType[eWindow]));

#if(ENABLE_PQ_LOAD_TABLE_INFO)

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {

        }
        else
        {
            MDrv_PQ_Set_LoadTableInfo_SrcType(MAIN,pInstance, _u16PQSrcType[eWindow]);
            MDrv_PQ_Set_LoadTableInfo_SrcType(MAINEX,pInstance, _u16PQSrcType[eWindow]);
        }
    }
#if PQ_ENABLE_PIP
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {

        }
        else
        {
            MDrv_PQ_Set_LoadTableInfo_SrcType(SUB,pInstance, _u16PQSrcType[eWindow]);
            MDrv_PQ_Set_LoadTableInfo_SrcType(SUBEX,pInstance, _u16PQSrcType[eWindow]);
        }
    }
#endif

#endif //#if(ENABLE_PQ_LOAD_TABLE_INFO)

}
void MDrv_PQ_DesideSrcType(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_DesideSrcType PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enInputSourceType = enInputSourceType;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_DESIDE_SRC_TYPE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType_U2(void* pInstance,PQ_WIN eWindow)
{
    return _enInputSourceType[eWindow];
}
PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetInputSourceType PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = PQ_INPUT_SOURCE_NONE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_INPUTSOURCE_TYPE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return PQ_INPUT_SOURCE_NONE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U16 MDrv_PQ_GetSrcType_U2(void* pInstance,PQ_WIN eWindow)
{
    return _u16PQSrcType[eWindow];
}
MS_U16 MDrv_PQ_GetSrcType(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetSrcType PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_SRC_TYPE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}


MS_U16 MDrv_PQ_GetDelayTimeByMemc_U2(void* pInstance, SCALER_WIN eWindow, MS_U32 u32MemcFrameDelay)
{
    MS_U8 u8FrameNum=0;
    MS_U8 u8WRBankMappingNum=0;
    MS_U8 u8PQDelayCountNum=u8PQDelayCount;

    u8FrameNum = MApi_XC_Get_FrameNumFactor(eWindow);
    u8WRBankMappingNum = MApi_XC_GetWRBankMappingNum(eWindow);


    if(u8FrameNum > (u8PQDelayCount +u8WRBankMappingNum ))
    {
        u8PQDelayCountNum = u8PQDelayCount +u8WRBankMappingNum;
    }
    else
    {
        u8PQDelayCountNum = u8FrameNum;
    }

    XC_INITMISC sXC_Init_Misc;
    memset(&sXC_Init_Misc, 0, sizeof(XC_INITMISC));
    MApi_XC_GetMISCStatus(&sXC_Init_Misc);

    if ((QM_GetInputVFreq(eWindow) != 0) && (QM_GetInputVFreq(eWindow) != 1))
    {

        if ( sXC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
        {
            _u16PQDelyaTime = (u8PQDelayCountNum + u32MemcFrameDelay) * ((10000 + (QM_GetInputVFreq(eWindow)/2))/QM_GetInputVFreq(eWindow));  // frame duration is (1 second/input V freq)

        }
        else
        {
            _u16PQDelyaTime = u8PQDelayCountNum * ((10000 + (QM_GetInputVFreq(eWindow)/2))/QM_GetInputVFreq(eWindow));  // frame duration is (1 second/input V freq)
        }
    }
    else
    {
        _u16PQDelyaTime = u8PQDelayCountNum * 20; // default keep original rule, use 20ms as frame duration
    }
    return _u16PQDelyaTime;
}

MS_U16 MDrv_PQ_GetDelayTimeByMemc(SCALER_WIN eWindow, MS_U32 u32MemcFrameDelay)
{
#ifdef TRACE_DBG
    printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetDelayTimeByMemc PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u32MemcFrameDelay = u32MemcFrameDelay;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_DELAY_TIME_BY_MEMC, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetDelayTime_U2(void* pInstance,SCALER_WIN eWindow)
{
    MS_U8 u8FrameNum=0;
    MS_U8 u8WRBankMappingNum=0;
    MS_U8 u8PQDelayCountNum=u8PQDelayCount;

    u8FrameNum = MApi_XC_Get_FrameNumFactor(eWindow);
    u8WRBankMappingNum = MApi_XC_GetWRBankMappingNum(eWindow);

    if(u8FrameNum > (u8PQDelayCount +u8WRBankMappingNum ))
    {
        u8PQDelayCountNum = u8PQDelayCount +u8WRBankMappingNum;
    }
    else
    {
        u8PQDelayCountNum = u8FrameNum;
    }

    //Because write memory not just in vsync blanking, so we add one vsync time for avoid garbage.
    u8PQDelayCountNum += 1;

    XC_INITMISC sXC_Init_Misc;
    memset(&sXC_Init_Misc, 0, sizeof(XC_INITMISC));
    MApi_XC_GetMISCStatus(&sXC_Init_Misc);

    XC_ApiStatusEx sXC_ApiStatusEx;
    memset(&sXC_ApiStatusEx, 0, sizeof(XC_ApiStatusEx));
    sXC_ApiStatusEx.u16ApiStatusEX_Length = sizeof(XC_ApiStatusEx);
    sXC_ApiStatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION;
    MApi_XC_GetStatusEx(&sXC_ApiStatusEx, eWindow);

    MS_BOOL bIs120Hz = FALSE;
    if(sXC_ApiStatusEx.u32ApiStatusEx_Version >= 9)
    {
        bIs120Hz = IsVBY1_16LANE(sXC_ApiStatusEx.u16PanelInterfaceType);
    }

    if ((QM_GetInputVFreq(eWindow) != 0) && (QM_GetInputVFreq(eWindow) != 1))
    {
        if (( sXC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE) &&(sXC_ApiStatusEx.bFRCEnabled == true)&&(bIs120Hz != TRUE))
        {
            _u16PQDelyaTime = (u8PQDelayCountNum + 5) * ((10000 + (QM_GetInputVFreq(eWindow)/2))/QM_GetInputVFreq(eWindow));  // frame duration is (1 second/input V freq)
        }
        else
        {
        _u16PQDelyaTime = u8PQDelayCountNum * ((10000 + (QM_GetInputVFreq(eWindow)/2))/QM_GetInputVFreq(eWindow));  // frame duration is (1 second/input V freq)
        }
    }
    else
        _u16PQDelyaTime = u8PQDelayCountNum * 20; // default keep original rule, use 20ms as frame duration

    return _u16PQDelyaTime;
}
MS_U16 MDrv_PQ_GetDelayTime(SCALER_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetDelayTime PQArgs;
    PQArgs.eWindow=eWindow;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_DELAY_TIME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}
MS_PQ_EXECUTOR MDrv_PQ_GetQmapExecutor_U2(void* pInstance,MS_PQ_FUNCTION_TYPE eFunctionType)
{
    MS_PQ_EXECUTOR enChipExecutor = EN_PQ_EXECUTOR_SOC;

    if( REG_CHIP_EXECUTOR_DUMMY != 0)
    {
        MS_U8 u8ExecutorDummy = (MS_U8)MApi_XC_R2BYTEMSK(REG_CHIP_EXECUTOR_DUMMY, 0xFF);
        MS_U8 u8ExecutorKeyDummy = (MS_U8)MApi_XC_R2BYTEMSK(REG_CHIP_EXECUTOR_KEY_DUMMY, 0xFF);

        if(u8ExecutorKeyDummy==REG_CHIP_EXECUTOR_KEY)
        {
            switch (eFunctionType)
            {
                case EN_PQ_ACE:
                    {
                        if(u8ExecutorDummy && BIT(0))
                            enChipExecutor= EN_PQ_EXECUTOR_BACKEND;
                        break;
                    }
                case EN_PQ_COLOR_TEMPERATURE:
                    {
                        if(u8ExecutorDummy && BIT(1))
                            enChipExecutor= EN_PQ_EXECUTOR_BACKEND;
                        break;
                    }
                case EN_PQ_NOISE_REDUCTION:
                    {
                        if(u8ExecutorDummy && BIT(2))
                            enChipExecutor= EN_PQ_EXECUTOR_BACKEND;
                        break;
                    }
                case EN_PQ_MPEG_NOISE_REDUCTION:
                    {
                        if(u8ExecutorDummy && BIT(3))
                            enChipExecutor= EN_PQ_EXECUTOR_BACKEND;
                        break;
                    }
                case EN_PQ_DYNAMIC_CONTRAST:
                    {
                        if(u8ExecutorDummy && BIT(4))
                            enChipExecutor= EN_PQ_EXECUTOR_BACKEND;
                        break;
                    }
                default:
                    enChipExecutor= EN_PQ_NONE;
                    break;
            }
        }
    }
    return enChipExecutor;
}
MS_PQ_EXECUTOR MDrv_PQ_GetQmapExecutor(MS_PQ_FUNCTION_TYPE eFunctionType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetQmapExecutor PQArgs;
    PQArgs.bReturnValue = EN_PQ_NONE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_QMAP_EXECUTOR, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return EN_PQ_NONE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}
// if not HSD case, ignore return value
static MS_BOOL _MDrv_PQ_LoadScalingTable(
        void* pInstance,
        PQ_WIN  eWindow,
        MS_U8 eXRuleType,
        MS_U8 u8XRuleIP,
        MS_BOOL bPreV_ScalingDown,
        MS_BOOL bInterlace,
        MS_BOOL bColorSpaceYUV,
        MS_U16 u16InputSize,
        MS_U16 u16SizeAfterScaling)
{
    MS_U32 u32Ratio;
    MS_U16 u16IPIdx;
#if PQ_ENABLE_PIP
    MS_U16 u16SFilter = 0;
#endif

    MS_U16 u16TabIdx = gbPQBinEnable == 0 ? PQ_IP_NULL : PQ_BIN_IP_NULL;
    MS_U8 u8XRuleIdx = 0xFF;

    if(u16InputSize == 0)
        u32Ratio = 2000;
    else
        u32Ratio = ((MS_U32) u16SizeAfterScaling * 1000) / u16InputSize;

    u16IPIdx = MDrv_PQ_GetXRuleIPIndex(pInstance, eWindow, eXRuleType, u8XRuleIP);
    SRULE_DBG(printf("SRuleIP=%d, IPIdx=%d, input=%d, output=%d, ratio=%td, ",
                     (MS_U16)u8XRuleIP, (MS_U16)u16IPIdx,
                     u16InputSize, u16SizeAfterScaling,(ptrdiff_t) u32Ratio));

    if(bPreV_ScalingDown && bInterlace)
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Interlace_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(pInstance, eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }
    else if(bPreV_ScalingDown && (!bInterlace))
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Progressive_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(pInstance, eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }


    if(((gbPQBinEnable == 0) && (u16TabIdx != PQ_IP_NULL)) ||
            ((gbPQBinEnable == 1) && (u16TabIdx != PQ_BIN_IP_NULL)))
    {

        SRULE_DBG(printf("u8XRuleIdx: PreV down, interlace:%u", bInterlace));
        SRULE_DBG(printf("(a)tabidx=%u\n", (MS_U16)u16TabIdx));
    }

    if(((gbPQBinEnable == 1) && (u16TabIdx == PQ_BIN_IP_NULL)) ||
            ((gbPQBinEnable == 0) && (u16TabIdx == PQ_IP_NULL)))
    {
        if(u32Ratio > 1000)
        {
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, u16IPIdx);
            SRULE_DBG(printf("u8XRuleIdx: >x1, "));
            SRULE_DBG(printf("(c)tabidx=%u\n", (MS_U16)u16TabIdx));
        }
        else
        {
            if(bColorSpaceYUV)
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_YUV_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_YUV_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_YUV_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_YUV_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_YUV_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_YUV_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_YUV_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_YUV_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_YUV_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_YUV_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_YUV_Main;
            }
            else
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_RGB_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_RGB_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_RGB_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_RGB_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_RGB_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_RGB_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_RGB_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_RGB_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_RGB_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_RGB_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_RGB_Main;
            }

            SRULE_DBG(printf("u8XRuleIdx=%u, ", (MS_U16)u8XRuleIdx));
            if(u8XRuleIdx == 0xFF)
            {
                MS_ASSERT(0);
                return 1;
            }

            u16TabIdx = MDrv_PQ_GetXRuleTableIndex(pInstance, eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
            if(((gbPQBinEnable == 1) && (u16TabIdx == PQ_BIN_IP_NULL)) ||
                    ((gbPQBinEnable == 0) && (u16TabIdx == PQ_IP_NULL)))
            {
                u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, u16IPIdx);
                SRULE_DBG(printf("(d)tabidx=%u\n", u16TabIdx));
            }
            else
            {
                SRULE_DBG(printf("(e)tabidx=%u\n", u16TabIdx));
            }
        }
    }
if(MApi_XC_Is_SupportSWDS() == FALSE)
{
#if PQ_EN_DMS_SW_CTRL
    if((u16IPIdx == PQ_IP_DMS_V_12L_Main) && (PQ_MAIN_WINDOW == eWindow))
    {
        if(u16TabIdx == PQ_IP_DMS_V_12L_OFF_Main)
        {
            _bDMSV12L_PQConfigEn = FALSE;
        }
        else
        {
            _bDMSV12L_PQConfigEn = TRUE;
        }
    }

    // App will call halt_subwin to disconnet sub window.
    // It will casue PQ display type to be PQ_DISPLAY_ONE.
    // But, sub window is still on and DMS_LV_12 will be set
    // to On in HSD rulee for main window.
    // For fix this issue, we need to check sub window wheter
    // on or off to descide DMS_LV_12 off or on.

    //Disable DMS_V12_L
    // 1.VSD : V prescaling, 2. PIP, 3. FBL
        if((((eXRuleType == E_XRULE_VSD)&&(bPreV_ScalingDown))
            ||(_stMode_Info[eWindow].bFBL)
            || (E_XC_3D_INPUT_MODE_NONE != MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW))
#if PQ_ENABLE_PIP
            || (PQ_SUB_WINDOW == eWindow)
            || ((PQ_MAIN_WINDOW == eWindow) && (_bDMSV12L_PIPEn == FALSE))
#endif
            )
          &&
            ((PQ_IP_DMS_V_12L_Main == MDrv_PQ_GetXRuleIPIndex(pInstance, eWindow, eXRuleType, u8XRuleIP))
//#if PQ_ENABLE_PIP
//             || (PQ_IP_DMS_V_12L_Sub == MDrv_PQ_GetXRuleIPIndex(pInstance, eWindow, eXRuleType, u8XRuleIP))
//#endif
             )
        )
        {
            //Disable De-Mosquito
            u16TabIdx = PQ_IP_DMS_V_12L_OFF_Main;
            u16IPIdx = PQ_IP_DMS_V_12L_Main;
            eWindow = PQ_MAIN_WINDOW;
        }
#endif
#if PQ_ENABLE_PIP
    if(((u16IPIdx == PQ_IP_SRAM3_Sub) || (u16IPIdx == PQ_IP_SRAM4_Sub) ) &&(PQ_SUB_WINDOW == eWindow)
     &&((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())))
    {
            u16SFilter = MApi_XC_R2BYTE(REG_SC_BK23_0B_L);
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, 0x0303 , 0xFFFF );
    }
#endif
}
#if (defined (UFO_XC_DS_PQ))
    if(MApi_XC_Is_SupportSWDS() == TRUE)
    {
        //XC_PRINTF("\033[1;32m[%s:%d]u16PQ_GameModeIdx = %d,u16IPIdx = %d,u16TabIdx = %d\033[m\n",__FUNCTION__,__LINE__,u16PQ_GameMode_Idx,u16IPIdx,u16TabIdx);
        if(eWindow == PQ_MAIN_WINDOW)
        {
            switch (u16IPIdx)
            {
                case PQ_IP_MADi_Main:
                    if (_s32LastPQIPMADiMain == (MS_S32)u16TabIdx)
                    {
                        return FALSE;
                    }
                    _s32LastPQIPMADiMain = u16TabIdx;

                    break;
#if (PQ_MADI_DFK == 1)
                    case PQ_IP_MADi_Motion_Main:
                    if (_s32LastPQIPMADiMotMain == (MS_S32)u16TabIdx)
                    {
                        return FALSE;
                    }
                    _s32LastPQIPMADiMotMain = u16TabIdx;

                    break;
#endif
                case PQ_IP_MemFormat_Main:
                    if (_s32LastPQIPMemFormatMain == (MS_S32)u16TabIdx)
                    {
                        return FALSE;
                    }
                    _s32LastPQIPMemFormatMain = u16TabIdx;
                    break;
                case PQ_IP_HSD_Y_Main:
                    if (_s32LastPQIPHSDYMain == (MS_S32)u16TabIdx)
                    {
                        return FALSE;
                    }
                    _s32LastPQIPHSDYMain = u16TabIdx;
                    break;
                case PQ_IP_HSD_C_Main:
                    if (_s32LastPQIPHSDCMain == (MS_S32)u16TabIdx)
                    {
                        return FALSE;
                    }
                    _s32LastPQIPHSDCMain = u16TabIdx;
                    break;
                default:
                    break;
             }
            MDrv_PQBin_DS_GetTable(pInstance,eWindow, u16TabIdx, u16IPIdx,PQ_XC0_STD);  //Only Main has X rule
        }
    }
    else
#endif
    {
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }

    if(PQ_MAIN_WINDOW == eWindow)
    {
        switch (u16IPIdx)
        {
            case PQ_IP_SRAM1_Main:
                _u16SRAM1 = u16TabIdx;
                break;

            case PQ_IP_SRAM2_Main:
                _u16SRAM2 = u16TabIdx;
                break;

            default:
                break;
        }
    }

#if PQ_ENABLE_PIP
    if(MApi_XC_Is_SupportSWDS() == FALSE)
    {
        if(((u16IPIdx == PQ_IP_SRAM3_Sub) || (u16IPIdx == PQ_IP_SRAM4_Sub)) &&(PQ_SUB_WINDOW == eWindow)
            &&((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
            || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
            || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
            || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
            || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())))
        {
                MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, u16SFilter , 0xFFFF );
        }
    }
#endif

    if(eXRuleType == E_XRULE_VSD)
        return (u16TabIdx  == PQ_IP_VSD_Bilinear_Main) ; // PreVSDMode: 0:Cb, 1:Bilinear
    else
        return (u16TabIdx != PQ_IP_HSD_Y_CB_Main); // PreHSDMode - 0:Cb, 1:Adv

}

MS_BOOL MDrv_PQ_LoadScalingTable_U2(void* pInstance,
                                 PQ_WIN eWindow,
                                 MS_U8 eXRuleType,
                                 MS_BOOL bPreV_ScalingDown,
                                 MS_BOOL bInterlace,
                                 MS_BOOL bColorSpaceYUV,
                                 MS_U16 u16InputSize,
                                 MS_U16 u16SizeAfterScaling)
{
    MS_BOOL bRet = 0; // default is CB mode
    MS_U16 i;

    if(eXRuleType > 3)
        MS_ASSERT(0);

    SRULE_DBG(printf("[PQ_LoadScalingTable] HSD/VSD/HSP/VSP:%u\n", (MS_U16)eXRuleType));

#if (defined (UFO_XC_DS_PQ))//PQ DS not support pre scaling
    if(MApi_XC_Is_SupportSWDS() == TRUE)
    {
        if((eXRuleType == E_XRULE_HSD)||(eXRuleType== E_XRULE_VSD))
        {
            return bRet;
        }
    }
#endif

    if(MApi_XC_Is_SupportSWDS() == FALSE)
    {
#if (ENABLE_PQ_MLOAD)
        if(eXRuleType == E_XRULE_HSD)
        {
            MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        }
#endif
    }

    for(i = 0; i < MDrv_PQ_GetXRuleIPNum(pInstance, eWindow, (MS_U16)eXRuleType); i++)
    {
        MS_BOOL bSDMode;
        bSDMode = _MDrv_PQ_LoadScalingTable(pInstance,
                                            eWindow,
                                            eXRuleType,
                                            i,
                                            bPreV_ScalingDown,
                                            bInterlace,
                                            bColorSpaceYUV,
                                            u16InputSize,
                                            u16SizeAfterScaling);
        bRet |= (bSDMode << (1 * i));
    }

    if(MApi_XC_Is_SupportSWDS() == FALSE)
    {
#if (ENABLE_PQ_MLOAD)
        if(eXRuleType == E_XRULE_HSD)
        {
            MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        }
#endif
    }

    if(eXRuleType == E_XRULE_VSD)
    {
        bRet &= 0x01;
    }
#if 0
    //121 filter
    if(((MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) ||
        (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM) ||
        (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)) &&
        (MApi_XC_Get_3D_HW_Version() > 0)&&
        (MDrv_PQ_Get_SensioMode() == TRUE))
    {
        if((u16InputSize >= u16SizeAfterScaling) && (eXRuleType == E_XRULE_VSP))
        {
            //if post V down, we will load 121 filter, and when it's post scaling case
            //reload 121 filter's VSP_Y=SRAM_1_4Tap, VSP_C=C_SRAM_1,
            //                    SRAM1=InvSinc4Tc4p4Fc50Apass01Astop55, C_SRAM1=C121 for main-win
            //reload 121 filter's VSP_Y=SRAM_3_4Tap, VSP_C=C_SRAM_3,
            //                    SRAM3=InvSinc4Tc4p4Fc50Apass01Astop55, C_SRAM3=C121 for sub-win
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VSP_Y_SRAM_1_4Tap_Main, PQ_IP_VSP_Y_Main);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VSP_C_C_SRAM_1_Main, PQ_IP_VSP_C_Main);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_SRAM1_InvSinc4Tc4p4Fc50Apass01Astop55_Main, PQ_IP_SRAM1_Main);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_C121_Main, PQ_IP_C_SRAM1_Main);

            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_VSP_Y_SRAM_3_4Tap_Sub, PQ_IP_VSP_Y_Sub);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_VSP_C_C_SRAM_3_Sub, PQ_IP_VSP_C_Sub);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_SRAM3_InvSinc4Tc4p4Fc50Apass01Astop55_Sub, PQ_IP_SRAM3_Sub);
            MDrv_PQ_LoadTable_U2(pInstance,PQ_SUB_WINDOW, PQ_IP_C_SRAM3_C121_Sub, PQ_IP_C_SRAM3_Sub);
        }
    }
#endif

    return bRet;
}
MS_BOOL MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
        MS_U8 eXRuleType,
        MS_BOOL bPreV_ScalingDown,
        MS_BOOL bInterlace,
        MS_BOOL bColorSpaceYUV,
        MS_U16 u16InputSize,
        MS_U16 u16SizeAfterScaling)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ, &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_LoadScalingTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.eXRuleType = eXRuleType;
    PQArgs.bPreV_ScalingDown = bPreV_ScalingDown;
    PQArgs.bInterlace = bInterlace;
    PQArgs.bColorSpaceYUV = bColorSpaceYUV;
    PQArgs.u16InputSize = u16InputSize;
    PQArgs.u16SizeAfterScaling = u16SizeAfterScaling;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_SCALING_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//////////////////////////////////////////////////////////////////
//  set photo YUV standard
//////////////////////////////////////////////////////////////////
void MDrv_PQ_SetPhotoYUVStandard_U2(void* pInstance,PQ_WIN eWindow, PQ_YUV_STD enStd)
{
    _enYUVStandard[eWindow] = enStd;
}
void MDrv_PQ_SetPhotoYUVStandard(PQ_WIN eWindow, PQ_YUV_STD enStd)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return;
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_SetPhotoYUVStandard PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enStd = enStd;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_PHOTO_YUV_STD, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
//////////////////////////////////////////////////////////////////
// set color range of input source and take effect immediately
//
void MDrv_PQ_SetColorRange_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bColorRange0_255)
{
    _bColorRange0_255[PQ_MAIN_WINDOW] = bColorRange0_255;
    _bColorRange0_255[PQ_SUB_WINDOW] = bColorRange0_255;
    MDrv_PQ_SetCSC_U2(pInstance, eWindow, _gForceColorFmt[eWindow]);
}
void MDrv_PQ_SetColorRange(PQ_WIN eWindow, MS_BOOL bColorRange0_255)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return;
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_SetColorRange PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bColorRange0_255 = bColorRange0_255;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_COLOR_RANGE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_Get_CSC_XRuleIdx_U2(void* pInstance,MS_BOOL *pbInputColorSpaceRGB,
                                            MS_U16 * pu16DoCSC,
                                            PQ_WIN eWindow,
                                            PQ_FOURCE_COLOR_FMT enForceColor,
                                            MS_BOOL bLoadPQTable)
{
    MS_U16 u16XRuleIdx = 0, u16XRuleIP = 0;
    MS_U16 u16IPIdx = 0, u16TabIdx = 0;
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_BOOL bInputTypeVideo = FALSE;
    MS_BOOL bInputResolutionHD = _gIsSrcHDMode[eWindow];

    _gForceColorFmt[eWindow] = enForceColor;

#if ENABLE_VGA_EIA_TIMING
    if ((QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
        QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow)) && !QM_IsInterlaced(eWindow))
#else
    if(QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow) && !QM_IsInterlaced(eWindow)))
#endif
    {
        CSCRULE_DBG(printf("PC mode\n"));
        bInputTypeVideo = FALSE;

#if (PQ_ENABLE_PIP == 1)
        if((eWindow == PQ_SUB_WINDOW) && MDrv_PQ_GetVGASubCaseForceTo422Mode_U2(pInstance))
        {
            printf("[%s][%d] VGA in sub video case, force PC mode to video mode \n", __FUNCTION__, __LINE__);
            bInputTypeVideo = TRUE;
        }
#endif
    }
    else
    {
        CSCRULE_DBG(printf("VIDEO mode\n"));
        bInputTypeVideo = TRUE;
    }

    if(MDrv_PQ_IsForceVideoInputMode_U2(pInstance, eWindow) == TRUE)
    {
        CSCRULE_DBG(printf("Force VIDEO mode\n"));
        bInputTypeVideo = TRUE;
    }

    if( QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow],eWindow)) ||
       (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow],eWindow) && (QM_HDMIPC_COLORRGB(eWindow))) )
    {
        CSCRULE_DBG(printf("Input RGB\n"));
        (*pbInputColorSpaceRGB) = TRUE;
    }
    else
    {
        CSCRULE_DBG(printf("Input YUV\n"));
        (*pbInputColorSpaceRGB) = FALSE;
    }

    CSCRULE_DBG(printf("VIDEO mode=%u, FourColor=%u, HD%u, InputRGB=%u, _bColorRange0_255=%u\n", bInputTypeVideo, enForceColor, bInputResolutionHD, *pbInputColorSpaceRGB, _bColorRange0_255[eWindow]));
    if (bInputTypeVideo)
    {
        if (*pbInputColorSpaceRGB)
        {
            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Main;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Main;
                    }
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Sub : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Sub :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Sub;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Sub :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Sub :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Sub;
                    }
                }
            }
#endif
        }
        else // InputColorSpaceYUV
        {
            if(enForceColor == PQ_FOURCE_COLOR_RGB )
            {
                if(_bP2pForceToDoCsc == FALSE)
                {
                    //MS_ASSERT(0);
                }
                PQTAB_DBG(printf("InputColorSpace is YUV, Force Color Space is RGB!!!\n"));
            }

            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Main;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Main;
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Sub;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Sub;
                }
            }
#endif
        }
    }
    else // InputTypePC
    {
        if (*pbInputColorSpaceRGB)
        {
            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Main;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Main;
                    }
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Sub :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Sub :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Sub;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Sub :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enForceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Sub :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Sub;
                    }
                }
            }
#endif
        }
        else // InputColorSpaceYUV
        {
            if(enForceColor == PQ_FOURCE_COLOR_RGB )
            {
                if(_bP2pForceToDoCsc == FALSE)
                {
                    //MS_ASSERT(0);
                }
                PQTAB_DBG(printf("InputColorSpace is YUV, Force Color Space is RGB!!!\n"));
            }

            if(eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Main;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Main;
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Sub;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Sub;
                }
            }
#endif
        }
    }

    for(u16XRuleIP=0; u16XRuleIP<MDrv_PQ_GetXRuleIPNum(pInstance, eWindow, eXRuleType); u16XRuleIP++)
    {
        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(pInstance, eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(pInstance, eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        //for ptp, no csc
        if(MDrv_PQ_Get_PointToPoint_U2(pInstance, eWindow))
        {
            if(_bP2pForceToDoCsc == FALSE)
            {
                u16TabIdx = 0x00; //PQ_IP_CSC_OFF_Main and PQ_IP_VIP_CSC_OFF_Main
            }
            CSCRULE_DBG(printf("PointToPoint \n"));
        }
        CSCRULE_DBG(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        if(bLoadPQTable)
        {
#ifdef PQ_CFD_INFO
#else
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
#endif
            //if(u16XRuleIP==0)
            //    MDrv_PQ_LoadTable_U2(pInstance,eWindow, 0, 6);
            //else if(u16XRuleIP==1)
            //    MDrv_PQ_LoadTable_U2(pInstance,eWindow, 5, 122);
            //else
            //    MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx, u16IPIdx);
        }

        if(u16TabIdx)
        {
#if PQ_HAL_EWINDOW_PARAM
            if(u16IPIdx == Hal_PQ_get_csc_ip_idx(pInstance,MS_CSC_IP_CSC,eWindow))
#else
            if(u16IPIdx == Hal_PQ_get_csc_ip_idx(pInstance,MS_CSC_IP_CSC))
#endif
            {
                (*pu16DoCSC) |= 0x1;
            }
#if PQ_HAL_EWINDOW_PARAM
            else if(u16IPIdx == Hal_PQ_get_csc_ip_idx(pInstance,MS_CSC_IP_VIP_CSC,eWindow))
#else
            else if(u16IPIdx == Hal_PQ_get_csc_ip_idx(pInstance,MS_CSC_IP_VIP_CSC))
#endif
            {
                (*pu16DoCSC) |= 0x02;
            }
            else
            {
                (*pu16DoCSC) |= 0x00;
            }
        }
    }
    return true;
}
MS_BOOL MDrv_PQ_Get_CSC_XRuleIdx(MS_BOOL *pbInputColorSpaceRGB,
                                            MS_U16 * pu16DoCSC,
                                            PQ_WIN eWindow,
                                            PQ_FOURCE_COLOR_FMT enForceColor,
                                            MS_BOOL bLoadPQTable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_CSC_XRuleIdx PQArgs;
    PQArgs.pbInputColorSpaceRGB = pbInputColorSpaceRGB;
    PQArgs.pu16DoCSC = pu16DoCSC;
    PQArgs.eWindow = eWindow;
    PQArgs.enForceColor = enForceColor;
    PQArgs.bLoadPQTable = bLoadPQTable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CSC_XRULEIDX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//////////////////////////////////////////////////////////////////
// get memory color format
// Note: return true if CSC is TRUE
// this case is for video rgb csc case
// return:
//        TRUE: use YUV color space, by front-end csc
//        FALSE: otherwise
MS_BOOL MDrv_PQ_Get_MemYUVFmt_U2(void* pInstance,PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor)
{
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputColorSpaceRGB;
    MS_BOOL bMemYUV;

    MDrv_PQ_Get_CSC_XRuleIdx_U2(pInstance, &bInputColorSpaceRGB, &u16DoCSC, eWindow, enForceColor, FALSE);

    if(bInputColorSpaceRGB)
    {
        if(u16DoCSC & 0x01)
        {
            bMemYUV = TRUE;
        }
        else
        {
            bMemYUV = FALSE;
        }
    }
    else
    {
        bMemYUV = TRUE;
    }

    return bMemYUV;
}
MS_BOOL MDrv_PQ_Get_MemYUVFmt(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return PQ_DEINT_OFF;
        }
    }

    stPQ_Get_MemYUVFmt PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enForceColor = enForceColor;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_MEM_YUVFMT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


//////////////////////////////////////////////////////////////////
// do color space conversion from RGB to YUV according to input sourcce and context
// Note: return true if CSC or VIP_CSC is TRUE
// this case is for (video rgb and pc rgb) csc case
// return:
//        TRUE: use YUV color space, for front-end and back-end csc both
//        FALSE: otherwise
//
MS_BOOL MDrv_PQ_SetCSC_U2(void* pInstance,PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor)
{
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputColorSpaceRGB;


    MDrv_PQ_Get_CSC_XRuleIdx_U2(pInstance, &bInputColorSpaceRGB, &u16DoCSC, eWindow, enForceColor, TRUE);

    u16DoCSC = (u16DoCSC != 0);
    if ((bInputColorSpaceRGB && u16DoCSC) || (!bInputColorSpaceRGB))
    {
        return TRUE;    // use YUV space
    }
    else
    {
        return FALSE;   // use RGB space
    }
}
MS_BOOL MDrv_PQ_SetCSC(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_SetCSC PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enForceColor = enForceColor;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_CSC, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

#if (PQ_XRULE_DB_ENABLE == 1)
void MDrv_PQ_Set_DBRule_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16MADi_Idx)
{
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;

    eXRuleType = (_stMode_Info[eWindow].u16input_vfreq > 550) ? E_XRULE_DB_NTSC : E_XRULE_DB_PAL;

    if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE8_Main;
    }
#if (PQ_P_MODE8_3FRAME_MODE_ENABLE == 1)
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE8_3Frame) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE8_Main;
    }
#endif

    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE10_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT8_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT10_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_6R_MC) == u16MADi_Idx)   //Add New MADi mode
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_14F_6R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else
    {
        u16XRuleIdx = 0xFF;
    }

    for(u16XRuleIP = 0; u16XRuleIP < MDrv_PQ_GetXRuleIPNum(pInstance, eWindow, eXRuleType); u16XRuleIP++)
    {
        if(u16XRuleIdx == 0xFF)
            continue;

        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(pInstance, eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(pInstance, eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        //(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
void MDrv_PQ_Set_DBRule(PQ_WIN eWindow, MS_U16 u16MADi_Idx)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_DBRule PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16MADi_Idx = u16MADi_Idx;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DBRULE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

void MDrv_PQ_EnableMADIForce_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bFullMotion)
{
#if PQ_ENABLE_FORCE_MADI
    if(bFullMotion)
    {
        if(PQ_MAIN_WINDOW == eWindow)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_MADi_Force_YC_FullMotion_Main, PQ_IP_MADi_Force_Main);
        }
//#if (ENABLE_LITE_SN == 0)
        else if( (PQ_SUB_WINDOW == eWindow) && ( _upLayerConfig & E_PQ_MISC_A_LITESN_ENABLE ) )
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_MADi_Force_YC_FullMotion_Sub, PQ_IP_MADi_Force_Sub);
        }
//#endif
    }
    else
    {
        if(PQ_MAIN_WINDOW == eWindow)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_MADi_Force_YC_FullStill_Main, PQ_IP_MADi_Force_Main);
        }
//#if (ENABLE_LITE_SN == 0)
        else if( (PQ_SUB_WINDOW == eWindow) && ( _upLayerConfig & E_PQ_MISC_A_LITESN_ENABLE ) )
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_MADi_Force_YC_FullStill_Sub, PQ_IP_MADi_Force_Sub);
        }
//#endif
    }
#else
UNUSED(eWindow);
UNUSED(bFullMotion);
#endif
}
void MDrv_PQ_EnableMADIForce(PQ_WIN eWindow, MS_BOOL bFullMotion)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_EnableMADIForce PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bFullMotion = bFullMotion;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_ENABLE_MADI_FORCE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_ReduceBW_ForPVR_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bPVR_On)
{
    UNUSED(eWindow);
    UNUSED(bPVR_On);
}
void MDrv_PQ_ReduceBW_ForPVR(PQ_WIN eWindow, MS_BOOL bPVR_On)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_ReduceBW_ForPVR PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bPVR_On = bPVR_On;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_REDUCE_BW_FOR_PVR, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void _MDrv_PQ_Set_MVOP_UVShift(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = MApi_XC_ReadByte(VOP_MPG_JPG_SWITCH);

    if(((regval & 0x10) == 0x10) && ((regval & 0x3) == 0x2))
    {
        // 422 with MCU control mode
        if(bEnable)
        {
            MS_ASSERT(0);
        }
    }

    // output 420 and interlace
    //[IP - Sheet] : Main Page --- 420CUP
    //[Project] :  Titania2
    //[Description]:   Chroma artifacts when 420to422 is applied duplicate method.
    //[Root cause]: Apply 420to422 average algorithm to all DTV input cases.
    //The average algorithm must cooperate with MVOP.
    MApi_XC_WriteByteMask(VOP_UV_SHIFT, (bEnable) ? 1 : 0, 0x3);
}

void MDrv_PQ_Set420upsampling_U2(void* pInstance,
                              PQ_WIN eWindow,
                              MS_BOOL bFBL,
                              MS_BOOL bPreV_ScalingDown,
                              MS_U16 u16V_CropStart)
{
    MS_U16 u16TabIdx;

    u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_420CUP_Main);

    PQTAB_DBG(printf("[PQ_Set420upsampling]: SrcType:%u, FBL:%u, PreV down:%u, V_CropStart:%u, u8TabIdx=%u, ",
                     _u16PQSrcType[eWindow], bFBL, bPreV_ScalingDown, u16V_CropStart, u16TabIdx));

#if PQ_HAL_EWINDOW_PARAM
    if((u16TabIdx == Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_ON, eWindow)) && (!bPreV_ScalingDown) && (!bFBL))
#else
    if((u16TabIdx == Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_ON)) && (!bPreV_ScalingDown) && (!bFBL))
#endif
    {
        PQTAB_DBG(printf("UVShift: on\n"));
        _MDrv_PQ_Set_MVOP_UVShift(ENABLE);
#if PQ_HAL_EWINDOW_PARAM
        MDrv_PQ_LoadTable_U2(pInstance,
                            eWindow, Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_ON, eWindow),
                            eWindow == PQ_MAIN_WINDOW? PQ_IP_420CUP_Main:PQ_IP_420CUP_Sub);
#else
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_ON), PQ_IP_420CUP_Main);
#endif
    }
    else    // P mode should not do UV shift
    {
        PQTAB_DBG(printf("UVShift: off\n"));
        _MDrv_PQ_Set_MVOP_UVShift(DISABLE);
#if PQ_HAL_EWINDOW_PARAM
        MDrv_PQ_LoadTable_U2(pInstance,
                            eWindow, Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_OFF, eWindow),
                            eWindow == PQ_MAIN_WINDOW? PQ_IP_420CUP_Main:PQ_IP_420CUP_Sub);
#else
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, Hal_PQ_get_420_cup_idx(pInstance,MS_420_CUP_OFF), PQ_IP_420CUP_Main);
#endif
    }

    if((u16V_CropStart & 0x3) == 0)       // crop lines are multiple of 4
        Hal_PQ_set_420upSample(pInstance,0x6666);
    else if((u16V_CropStart & 0x1) == 0)  // crop lines are multiple of 2
        Hal_PQ_set_420upSample(pInstance,0x9999);
//    else
//        MS_ASSERT(0);
}
void MDrv_PQ_Set420upsampling(PQ_WIN eWindow,
                              MS_BOOL bFBL,
                              MS_BOOL bPreV_ScalingDown,
                              MS_U16 u16V_CropStart)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_Set420upsampling PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bFBL = bFBL;
    PQArgs.bPreV_ScalingDown = bPreV_ScalingDown;
    PQArgs.u16V_CropStart = u16V_CropStart;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_420UP_SAMPLEING, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_SetFilmMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;

    PQTAB_DBG(printf("[PQ_SetFilmMode]: PQTabType=%u, enable=%u\n", eWindow, bEnable));

    if(bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_Film32_Main);
    else
        u16TabIdx = PQ_IP_Film32_OFF_Main;

    MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, PQ_IP_Film32_Main);

    if(bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_Film22_Main);
    else
        u16TabIdx = PQ_IP_Film22_OFF_Main;

    MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, PQ_IP_Film22_Main);
}
void MDrv_PQ_SetFilmMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetFilmMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_FILEMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


void MDrv_PQ_SetNonLinearScaling_U2(void* pInstance,PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;
    PQTAB_DBG(printf("[PQ_SetNonLinearScaling]: Level=%u, enable=%u\n", u8Level, bEnable));

    if(eWindow == PQ_SUB_WINDOW)
    {
        //sub window don't need nonLinear scaling
        return;
    }

    if(bEnable)
    {
        switch(_stPQ_Info.u16PnlWidth)
        {
            case 3840:
                switch(u8Level)
                {
                    case 2:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_3840_2);
                        break;
                    case 1:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_3840_1);
                        break;
                    case 0:
                    default:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_3840_0);
                        break;
                }
                break;
            case 1920:
                switch(u8Level)
                {
                    case 2:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1920_2);
                        break;
                    case 1:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1920_1);
                        break;
                    case 0:
                    default:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1920_0);
                        break;
                }
                break;
            case 1680:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1680);
                break;
            case 1440:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1440);
                break;
            case 1366:
                switch(u8Level)
                {
                    case 2:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1366_2);
                        break;
                    case 1:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1366_1);
                        break;
                    case 0:
                    default:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_1366_0);
                        break;

                }
                break;
            default:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_OFF);
                break;

                break;
        }
    }
    else
    {
        u16TabIdx = Hal_PQ_get_hnonlinear_idx(pInstance,MS_HNONLINEAR_OFF);
    }
    MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, PQ_IP_HnonLinear_Main);
}
void MDrv_PQ_SetNonLinearScaling(PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetNonLinearScaling PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Level = u8Level;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_NONLINEAR_SCALING, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void _MDrv_PQ_CloneSram(void *pInstance)
{
    MS_U16 u16CSRAM1 = MDrv_PQ_GetTableIndex_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_Main);
    MS_U16 u16CSRAM2 = MDrv_PQ_GetTableIndex_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_C_SRAM2_Main);

    if(gbPQBinEnable)
    {
        MS_PQBin_IP_Table_Info stTableInfo;
        memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

        MDrv_PQBin_GetTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                        _u16SRAM1,
                        PQ_IP_SRAM1_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_SRAM3;
        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);

        MDrv_PQBin_GetTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                        _u16SRAM2,
                        PQ_IP_SRAM2_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_SRAM4;
        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);

        MDrv_PQBin_GetTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                        u16CSRAM1,
                        PQ_IP_C_SRAM1_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_C_SRAM3;
        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);

        MDrv_PQBin_GetTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                        u16CSRAM2,
                        PQ_IP_C_SRAM2_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_C_SRAM4;
        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);
    }
    else
    {
        EN_IP_Info stIpInfo;
        memset(&stIpInfo, 0, sizeof(EN_IP_Info));
        stIpInfo = MDrv_PQ_GetTable_(MAIN,pInstance, _u16SRAM1, PQ_IP_SRAM1_Main);
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM3, (void *)((char *)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(pInstance,SC_FILTER_Y_SRAM1)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN,pInstance, _u16SRAM2, PQ_IP_SRAM2_Main);
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM4, (void *)((char *)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(pInstance,SC_FILTER_Y_SRAM2)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN,pInstance, u16CSRAM1, PQ_IP_C_SRAM1_Main);
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM3, (void *)((char *)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(pInstance,SC_FILTER_C_SRAM1)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN,pInstance, u16CSRAM2, PQ_IP_C_SRAM2_Main);
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM4, (void *)((char *)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(pInstance,SC_FILTER_C_SRAM2)]));
    }
}

void MDrv_PQ_3DCloneforPIP_U2(void* pInstance,MS_BOOL bIpSync)
{
    MVOP_Handle stHdl = { E_MVOP_MODULE_MAIN };
    MS_BOOL bEnAltOutputState = FALSE;

    if (E_MVOP_OK == MDrv_MVOP_GetCommand(&stHdl, E_MVOP_CMD_GET_3DLR_ALT_OUT, &bEnAltOutputState, sizeof(bEnAltOutputState)))
    {
        //printf("%s Get3DLRAltOutput=0x%x\n", __FUNCTION__, bEnAltOutputState);
    }
    else
    {
        //printf("Fail to query E_MVOP_CMD_GET_3DLR_ALT_OUT!!\n");
    }

    if((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_LEFT_LEFT == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_RIGHT_RIGHT == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_TOP == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_BOTTOM_BOTTOM == MApi_XC_Get_3D_Output_Mode()))
    {
        _MDrv_PQ_CloneSram(pInstance);
#if PQ_QM_3D_CLONE_ENABLE
        MDrv_PQ_3D_CloneWindow(E_PQ_3DTYPE_XC, PQ_MAIN_WINDOW);
#else
        MApi_XC_ACE_3DClonePQMap(E_ACE_WEAVETYPE_NONE);
#endif

        _s32LastPQIPMADiSub = -1;
        _s32LastPQIPMemFormatSub = -1;
        _u16LastPQSrcType[PQ_SUB_WINDOW] = 0xFFFF;

        //printf("3D:confirm to clone main/sub win\n");

        if((MApi_XC_Get_3D_HW_Version() < 2) && bIpSync)
        {
            MApi_XC_3DMainSub_IPSync();
            //printf("3D:confirm to do main/sub win sync\n");
        }
    }
    else if(((E_XC_3D_INPUT_TOP_BOTTOM == MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW)) && (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode()))
    || ((E_XC_3D_INPUT_NORMAL_2D == MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW)) && (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())))
    {
        if(MApi_XC_Get_3D_HW_Version() < 2)
        {
            MDrv_PQ_EnableMADIForce(PQ_MAIN_WINDOW, TRUE);
#if PQ_MADI_DFK
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_MADi_DFK_OFF_Main, PQ_IP_MADi_DFK_Main);
#endif
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_EODi_OFF_Main, PQ_IP_EODi_Main);
        }
    }
    else if(MApi_XC_IsCurrentFrameBufferLessMode() && bEnAltOutputState)
    {
        MApi_XC_ACE_3DClonePQMap(E_ACE_WEAVETYPE_H);
    }
    MApi_XC_3D_PostPQSetting(MAIN_WINDOW);

#if(PQ_GRULE_3D_ENABLE == 1)
    if(E_XC_3D_OUTPUT_MODE_NONE != MApi_XC_Get_3D_Output_Mode())
    {
        MDrv_PQ_SetGRuleStatus(PQ_MAIN_WINDOW, PQ_GRule_3D_Main, PQ_GRule_3D_On_Main);
    }
    else
    {
        MDrv_PQ_SetGRuleStatus(PQ_MAIN_WINDOW, PQ_GRule_3D_Main, PQ_GRule_3D_Off_Main);
    }
#endif
}
void MDrv_PQ_3DCloneforPIP(MS_BOOL bIpSync)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_3DCloneforPIP PQArgs;
    PQArgs.bIpSync = bIpSync;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_3D_CLONE_FOR_PIP, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_DisableFilmMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bOn)
{
    if(bOn)
    {
        MDrv_PQ_SetFilmMode_U2(pInstance, eWindow, DISABLE);
    }
    else
    {
        MDrv_PQ_SetFilmMode_U2(pInstance, eWindow, ENABLE);
    }
}
void MDrv_PQ_DisableFilmMode(PQ_WIN eWindow, MS_BOOL bOn)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return;
        }
    }

    stPQ_DisableFilmMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bOn = bOn;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_DISABLE_FILEMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
static MS_BOOL _MDrv_PQ_LoadGameModeTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_GameMode_Idx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }

    if (u16PQ_GameMode_Idx > PQ_GRULE_GAME_MODE_LVL_NUM_Main)
    {
        MS_ASSERT(0);
        return FALSE;
    }
#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif

    for(i=0; i<PQ_GRULE_GAME_MODE_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_GAME_MODE_Main, i);

        if (u16PQ_GameMode_Idx == PQ_GRULE_GAME_MODE_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_GAME_MODE_Main, u16PQ_GameMode_Idx, i);

        PQGRULE_DBG(printf("[GameMode]SRC: %u, u16PQ_GameMode_Idx: %u, GameModeIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_GameMode_Idx, i, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
    return TRUE;
}
#endif
//void _MDrv_PQ_LoadPTPTable(PQ_WIN eWindow, MS_U16 u16PQ_PTP_Idx);

#if defined(PQ_GRULE_HDR_ENABLE) && (PQ_GRULE_HDR_ENABLE == 1)
static MS_BOOL _MDrv_PQ_LoadHDRModeTable(PQ_WIN eWindow, MS_U16 u16GRuleLevelIndex)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;
    MS_U16 u16PQ_HDRMode_Idx=0;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }

    u16PQ_HDRMode_Idx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_HDR_Main, u16GRuleLevelIndex);

    if (u16PQ_HDRMode_Idx > PQ_GRULE_HDR_NUM_Main)
    {
        MS_ASSERT(0);
        return FALSE;
    }
#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif

    for(i=0; i<PQ_GRULE_HDR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_HDR_Main, i);

        if (u16PQ_HDRMode_Idx == PQ_GRULE_HDR_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_HDR_Main, u16PQ_HDRMode_Idx, i);

        PQGRULE_DBG(printf("[HDRMode]SRC: %u, u16PQ_HDRMode_Idx: %u, HDRModeIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_HDRMode_Idx, i, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

#if defined(PQ_GRULE_UFSC_HDR_ENABLE) && (PQ_GRULE_UFSC_HDR_ENABLE == 1)
    u16PQ_HDRMode_Idx = MDrv_PQ_GetGRule_LevelIndex_Ex(eWindow, PQ_GRule_HDR_UFSC,u16GRuleLevelIndex, PQ_XC0_UFSC);

    if(u16PQ_HDRMode_Idx > PQ_GRULE_HDR_NUM_UFSC)
    {
        MS_ASSERT(0);
        return FALSE;
    }

    for(i = 0; i < PQ_GRULE_HDR_IP_NUM_UFSC; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_Ex(eWindow, PQ_GRule_HDR_UFSC, i, PQ_XC0_UFSC);

        if(u16PQ_HDRMode_Idx == PQ_GRULE_HDR_NUM_UFSC)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_Ex(eWindow, u16TabIdx, PQ_XC0_UFSC);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex_Ex(eWindow, PQ_GRule_HDR_UFSC, u16PQ_HDRMode_Idx, i,PQ_XC0_UFSC);

        PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_HDRMode_Idx, i, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable_Ex(eWindow, u16TabIdx, u16IPIdx, PQ_XC0_UFSC);
    }
#endif

    return TRUE;
}
#endif

MS_BOOL MDrv_PQ_EnableHDRMode(PQ_WIN eWindow ,MS_U16 u16GRuleLevelIndex)
{
    UNUSED(eWindow);
    UNUSED(u16GRuleLevelIndex);

#if defined(PQ_GRULE_HDR_ENABLE) && (PQ_GRULE_HDR_ENABLE == 1)
    _MDrv_PQ_LoadHDRModeTable(eWindow, u16GRuleLevelIndex);
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_DisableUCFeature_U2(void* pInstance,PQ_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    UNUSED(eWindow);
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    PQGRULE_DBG(printf("[PQ_LoadGameModeTable] "));
    MS_U16 u16PQ_GameModeIdx;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }
    PQMADi_DBG(printf("Disable UC feature by loading Game mode Grule\n");)

    u16PQ_GameModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_GAME_MODE_Main, PQ_GRule_Lvl_GAME_MODE_On_Main);
    bRet = _MDrv_PQ_LoadGameModeTable(pInstance, eWindow, u16PQ_GameModeIdx);

    if(_gIsEnable8LBMode)
    {
        MDrv_PQBin_Enable8LBMode(pInstance,TRUE);
        if(MApi_XC_Set_OPWriteOffEnableToReg)
        {
            MApi_XC_Set_OPWriteOffEnableToReg(ENABLE, MAIN_WINDOW); //default
        }
    }

#else
#if PQ_UC_CTL
    bRet = MDrv_PQ_SetUCFeature(PQ_MAIN_WINDOW,
                                0xFFFF,
                                0xFFFF,
                                PQ_IP_MADi_DFK_DFK1_Main,
                                PQ_IP_MADi_SST_SST3_Main,
                                PQ_IP_MADi_EODiW_OFF_Main,
                                PQ_IP_UCNR_OFF_Main,
                                PQ_IP_UCDi_OFF_Main,
                                PQ_IP_UC_CTL_OFF_Main,
                                0xFFFF,
                                0xFFFF,
                                0xFFFF
                                );
#endif
#endif
    return bRet;
}
MS_BOOL MDrv_PQ_DisableUCFeature(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_DisableUCFeature PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_DISABLE_UC_FEATURE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


MS_BOOL MDrv_PQ_GetCurrentUCFeature_U2(void* pInstance,
                                           PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi,
                                           MS_U16 *pu16TabIdx_MADi_Mot,
                                           MS_U16 *pu16TabIdx_MADi_DFK,
                                           MS_U16 *pu16TabIdx_MADi_SST,
                                           MS_U16 *pu16TabIdx_MADi_EODiW,
                                           MS_U16 *pu16TabIdx_UCNR,
                                           MS_U16 *pu16TabIdx_UCDi,
                                           MS_U16 *pu16TabIdx_UCCTL,
                                           MS_U16 *pu16TabIdx_SwDriver,
                                           MS_U16 *pu16TabIdx_PostCCS,
                                           MS_U16 *pu16TabIdx_DHD
                                           )
{
    if( (pu16TabIdx_MADi_DFK == NULL)
      ||(pu16TabIdx_MADi_SST == NULL)
      ||(pu16TabIdx_MADi_EODiW == NULL)
      ||(pu16TabIdx_UCDi == NULL)
      ||(pu16TabIdx_UCCTL == NULL)
      ||(pu16TabIdx_UCNR == NULL)
      )
    {
        printf("MDrv_PQ_GetCurrentUCFeature(), don't pass in NULL pointer!");
        return FALSE;
    }
#if PQ_UC_CTL

    *pu16TabIdx_MADi        = (eWindow==PQ_MAIN_WINDOW)?_s32LastPQIPMADiMain:_s32LastPQIPMADiSub;
    *pu16TabIdx_MADi_Mot    = (eWindow==PQ_MAIN_WINDOW)?_s32LastPQIPMADiMotMain:_s32LastPQIPMADiMotSub;
    *pu16TabIdx_MADi_DFK    = _u16MADi_DFK_Current[eWindow];
    *pu16TabIdx_MADi_SST    = _u16MADi_SST_Current[eWindow];
    *pu16TabIdx_MADi_EODiW  = _u16MADi_EODiW_Current[eWindow];
    *pu16TabIdx_UCDi        = _u16UCDi_Current[eWindow];
    *pu16TabIdx_UCCTL       = _u16UCCTL_Current[eWindow];
    *pu16TabIdx_UCNR        = _u16UCNR_Current[eWindow];
    *pu16TabIdx_SwDriver    = _u16SwDriver_Current[eWindow];
    *pu16TabIdx_PostCCS     = _u16PostCCS_Current[eWindow];
    *pu16TabIdx_DHD         = _u16DHD_Current[eWindow];
    PQMADi_DBG(printf("MDrv_PQ_GetCurrentUCFeature(%u, %u, %u,%u,%u,%u,%u,%u,%u,%u,%u)\n",
                                           *pu16TabIdx_MADi,
                                           *pu16TabIdx_MADi_Mot,
                                           *pu16TabIdx_MADi_DFK,
                                           *pu16TabIdx_MADi_SST,
                                           *pu16TabIdx_MADi_EODiW,
                                           *pu16TabIdx_UCNR,
                                           *pu16TabIdx_UCDi,
                                           *pu16TabIdx_UCCTL,
                                           *pu16TabIdx_SwDriver,
                                           *pu16TabIdx_PostCCS,
                                           *pu16TabIdx_DHD));

    return TRUE;
#else
    return FALSE;
#endif
}
MS_BOOL MDrv_PQ_GetCurrentUCFeature(PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi,
                                           MS_U16 *pu16TabIdx_MADi_Mot,
                                           MS_U16 *pu16TabIdx_MADi_DFK,
                                           MS_U16 *pu16TabIdx_MADi_SST,
                                           MS_U16 *pu16TabIdx_MADi_EODiW,
                                           MS_U16 *pu16TabIdx_UCNR,
                                           MS_U16 *pu16TabIdx_UCDi,
                                           MS_U16 *pu16TabIdx_UCCTL,
                                           MS_U16 *pu16TabIdx_SwDriver,
                                           MS_U16 *pu16TabIdx_PostCCS,
                                           MS_U16 *pu16TabIdx_DHD
                                           )
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetCurrentUCFeature PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pu16TabIdx_MADi = pu16TabIdx_MADi;
    PQArgs.pu16TabIdx_MADi_Mot = pu16TabIdx_MADi_Mot;
    PQArgs.pu16TabIdx_MADi_DFK = pu16TabIdx_MADi_DFK;
    PQArgs.pu16TabIdx_MADi_SST = pu16TabIdx_MADi_SST;
    PQArgs.pu16TabIdx_MADi_EODiW = pu16TabIdx_MADi_EODiW;
    PQArgs.pu16TabIdx_UCNR = pu16TabIdx_UCNR;
    PQArgs.pu16TabIdx_UCDi = pu16TabIdx_UCDi;
    PQArgs.pu16TabIdx_UCCTL = pu16TabIdx_UCCTL;
    PQArgs.pu16TabIdx_SwDriver =pu16TabIdx_SwDriver ;
    PQArgs.pu16TabIdx_PostCCS =pu16TabIdx_PostCCS ;
    PQArgs.pu16TabIdx_DHD =pu16TabIdx_DHD ;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CURRENT_UC_FEATURE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetUCFeatureFromPQ_U2(void* pInstance,
                                           PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi_DFK,
                                           MS_U16 *pu16TabIdx_MADi_SST,
                                           MS_U16 *pu16TabIdx_MADi_EODiW,
                                           MS_U16 *pu16TabIdx_UCNR,
                                           MS_U16 *pu16TabIdx_UCDi,
                                           MS_U16 *pu16TabIdx_UCCTL,
                                           MS_U16 *pu16TabIdx_SwDriver,
                                           MS_U16 *pu16TabIdx_PostCCS,
                                           MS_U16 *pu16TabIdx_DHD)
{
    if( (pu16TabIdx_MADi_DFK == NULL)
      ||(pu16TabIdx_MADi_SST == NULL)
      ||(pu16TabIdx_MADi_EODiW == NULL)
      ||(pu16TabIdx_UCDi == NULL)
      ||(pu16TabIdx_UCCTL == NULL)
      ||(pu16TabIdx_UCNR == NULL)
      ||(pu16TabIdx_SwDriver == NULL)
      ||(pu16TabIdx_PostCCS == NULL)
      ||(pu16TabIdx_DHD == NULL)
      )
    {
        printf(" \r\n MDrv_PQ_GetUCFeatureFromPQ , don't pass in NULL pointer!");
        return FALSE;
    }

    *pu16TabIdx_MADi_DFK =0xFFFF;
    *pu16TabIdx_MADi_SST = 0xFFFF;
    *pu16TabIdx_MADi_EODiW = 0xFFFF;
    *pu16TabIdx_UCNR = 0xFFFF;
    *pu16TabIdx_UCDi = 0xFFFF;
    *pu16TabIdx_UCCTL = 0xFFFF;
    *pu16TabIdx_SwDriver = 0xFFFF;
    *pu16TabIdx_PostCCS = 0xFFFF;
    *pu16TabIdx_DHD = 0xFFFF;
#if PQ_UC_CTL

#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
#if (PQ_DDR_SELECT_VER == 1)
    //do nothing, because ddr select rule has no uc item in version1
#elif (PQ_DDR_SELECT_VER == 2)
    MS_U16 u16PQ_DDRSELIdx = 0;

    if(bw_info.bDDR2 == FALSE) // DDR 3
    {
         if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
         {
             PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 mirror\n"));
             u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Mirror_Main);
         }
         else
         {
             PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 non-mirror\n"));
             u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Main);
         }
    }
    else // DDR 2
    {
         if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
         {
             PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 mirror\n"));
             u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Mirror_Main);
         }
         else
         {
             PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 non-mirror\n"));
             u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Main);
         }
    }
    *pu16TabIdx_MADi_DFK = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 2);
    *pu16TabIdx_MADi_SST = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 3);
    *pu16TabIdx_MADi_EODiW = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 4);
    *pu16TabIdx_UCCTL = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 5);
#endif
#else //DDR select disable
    if(eWindow == PQ_MAIN_WINDOW)
    {
        *pu16TabIdx_MADi_DFK    = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_MADi_DFK_Main); //y MADi_DFK ip
        *pu16TabIdx_MADi_SST    = MDrv_PQ_GetTableIndex_U2(pInstance, eWindow, PQ_IP_MADi_SST_Main); //y MADi_SST ip
        *pu16TabIdx_MADi_EODiW  = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_EODiW_Main); //y MADi_EODiW ip

        *pu16TabIdx_UCDi        = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_UCDi_Main); //y UCDi ip
        *pu16TabIdx_UCCTL       = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_UC_CTL_Main); //y UCCTL ip
    }
    #if PQ_ENABLE_PIP
    else
    {
        *pu16TabIdx_MADi_DFK    = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_DFK_Sub); // MADi_DFK ip
        *pu16TabIdx_MADi_SST    = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_SST_Sub); // MADi_SST ip
    }
    #endif


    //printf("$$[%s]:%d, %s, u16TabIdx_UCCTL=%u\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", *pu16TabIdx_UCCTL);
#endif

#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    if(eWindow == PQ_MAIN_WINDOW)
    {
        *pu16TabIdx_SwDriver    = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_SwDriver_Main);
        *pu16TabIdx_PostCCS     = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_PostCCS_Main);
        *pu16TabIdx_DHD         = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_DHD_Main);
    }
    #if PQ_ENABLE_PIP
    else
    {
        *pu16TabIdx_SwDriver   = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_SwDriver_Sub);
        #if PQ_SUPPORT_SUB_POSTCCS
        *pu16TabIdx_PostCCS    = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_PostCCS_Sub);
        #endif

        #if PQ_SUPPORT_SUB_DHD
        *pu16TabIdx_DHD        = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_DHD_Sub);
        #endif
    }
    #endif
#endif

#if (PQ_GRULE_SLZ_ENABLE == 1)
     MS_U16 u16PQ_SLZIdx = 0;
     MS_BOOL bSeamlessZappingEnable;

     if(MApi_XC_GetSeamlessZappingStatus(eWindow, &bSeamlessZappingEnable) == E_APIXC_RET_OK)
     {
        if(bSeamlessZappingEnable)
        {
            if(eWindow == PQ_MAIN_WINDOW)
            {
                 u16PQ_SLZIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_SLZ_Main, PQ_GRule_Lvl_SLZ_On_Main);
                 PQMADi_DBG(printf("MDrv_PQ_GetUCFeatureFromPQ: Main LSZ =%u\n", u16PQ_SLZIdx));
                 *pu16TabIdx_MADi_DFK    = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 2); //y MADi_DFK ip
                 *pu16TabIdx_MADi_SST    = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 3); //y MADi_SST ip
                 *pu16TabIdx_MADi_EODiW  = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 4); //y MADi_EODiW ip

                 *pu16TabIdx_UCDi        = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 5); //y UCDi ip
                 *pu16TabIdx_UCCTL       = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 6); //y UCCTL ip
             }
            #if PQ_ENABLE_PIP
            else
            {
                 u16PQ_SLZIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_SLZ_Sub, PQ_GRule_Lvl_SLZ_On_Sub);
                 PQMADi_DBG(printf("MDrv_PQ_GetUCFeatureFromPQ: Sub LSZ =%u\n", u16PQ_SLZIdx));
                 *pu16TabIdx_MADi_DFK    = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Sub, u16PQ_SLZIdx, 2); // MADi_DFK ip
                 *pu16TabIdx_MADi_SST    = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Sub, u16PQ_SLZIdx, 3); // MADi_SST ip
            }
            #endif
        }
        else
        {
            //it's not slz case
        }
     }
     else
     {
         printf("MApi_XC_GetStatusEx () version is incorrect at %s %d, the code's purpose is not fulfilled.\n",__FILE__,(int)__LINE__);
     }

     //printf("$$[%s]:%d, %s, u16TabIdx_UCCTL=%u\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", *pu16TabIdx_UCCTL);

    UNUSED(pu16TabIdx_UCNR);
#else
    UNUSED(pu16TabIdx_UCNR);
    UNUSED(pu16TabIdx_UCDi);
    UNUSED(pu16TabIdx_UCCTL);
    UNUSED(pu16TabIdx_MADi_DFK);
    UNUSED(pu16TabIdx_MADi_SST);
    UNUSED(pu16TabIdx_MADi_EODiW);
    UNUSED(pu16TabIdx_SwDriver);
    UNUSED(pu16TabIdx_PostCCS);
    UNUSED(pu16TabIdx_DHD);
#endif

#endif
    PQMADi_DBG(printf("MDrv_PQ_GetUCFeatureFromPQ(%u,%u,%u,%u,%u,%u,%u,%u,%u)\n",*pu16TabIdx_MADi_DFK,
                                           *pu16TabIdx_MADi_SST,
                                           *pu16TabIdx_MADi_EODiW,
                                           *pu16TabIdx_UCNR,
                                           *pu16TabIdx_UCDi,
                                           *pu16TabIdx_UCCTL,
                                           *pu16TabIdx_SwDriver,
                                           *pu16TabIdx_PostCCS,
                                           *pu16TabIdx_DHD));
    return TRUE;
}

MS_BOOL MDrv_PQ_GetUCFeatureFromPQ(PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi_DFK,
                                           MS_U16 *pu16TabIdx_MADi_SST,
                                           MS_U16 *pu16TabIdx_MADi_EODiW,
                                           MS_U16 *pu16TabIdx_UCNR,
                                           MS_U16 *pu16TabIdx_UCDi,
                                           MS_U16 *pu16TabIdx_UCCTL,
                                           MS_U16 *pu16TabIdx_SwDriver,
                                           MS_U16 *pu16TabIdx_PostCCS,
                                           MS_U16 *pu16TabIdx_DHD)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetUCFeatureFromPQ PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pu16TabIdx_MADi_DFK = pu16TabIdx_MADi_DFK;
    PQArgs.pu16TabIdx_MADi_SST = pu16TabIdx_MADi_SST;
    PQArgs.pu16TabIdx_MADi_EODiW = pu16TabIdx_MADi_EODiW;
    PQArgs.pu16TabIdx_UCNR = pu16TabIdx_UCNR;
    PQArgs.pu16TabIdx_UCDi = pu16TabIdx_UCDi;
    PQArgs.pu16TabIdx_UCCTL = pu16TabIdx_UCCTL;
    PQArgs.pu16TabIdx_SwDriver =pu16TabIdx_SwDriver ;
    PQArgs.pu16TabIdx_PostCCS =pu16TabIdx_PostCCS ;
    PQArgs.pu16TabIdx_DHD =pu16TabIdx_DHD ;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_UC_FEATURE_FROM_PQ, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

#if(PQ_GRULE_GAME_MODE_ENABLE == 1)
MS_BOOL MDrv_PQ_EnableUCFeature_U2(void* pInstance,PQ_WIN eWindow)
{
    MS_BOOL bRet = TRUE;
    MS_U8 i = 0 ;
    MS_U16 u16IPIdx = 0xFFFF;
    MS_U16 u16IPTabIdx_Mode = 0xFFFF;
    MS_U16 u16TabIdx_UCCTL= 0xFFFF;

#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
#if (PQ_DDR_SELECT_VER == 1)
        //do nothing, because ddr select rule has no uc item in version1
#elif (PQ_DDR_SELECT_VER == 2)
        MS_U16 u16PQ_DDRSELIdx = 0;
        MS_U16 u16TabIdx_MADi_DFK_DDR = 0xFFFF;
        MS_U16 u16TabIdx_MADi_SST_DDR = 0xFFFF;
        MS_U16 u16TabIdx_MADi_EODiW_DDR = 0xFFFF;
        MS_U16 u16TabIdx_UCCTL_DDR = 0xFFFF;
        if(bw_info.bDDR2 == FALSE) // DDR 3
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Mirror_Main);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 non-mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Main);
             }
        }
        else // DDR 2
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Mirror_Main);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 non-mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Main);
             }
        }

        u16TabIdx_MADi_DFK_DDR = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 2);
        u16TabIdx_MADi_SST_DDR = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 3);
        u16TabIdx_MADi_EODiW_DDR = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 4);
        u16TabIdx_UCCTL_DDR = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 5);
#endif
#endif
        for(i=0; i<PQ_GRULE_GAME_MODE_IP_NUM_Main; i++)
        {
            u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, PQ_MAIN_WINDOW, PQ_GRule_GAME_MODE_Main, i);
            if(u16IPIdx ==  PQ_IP_MADi_Main)
            {
                u16IPTabIdx_Mode =_gu16SetMemfmt_Madi[eWindow];
            }
            else if(u16IPIdx ==  PQ_IP_MADi_Motion_Main)
            {
               u16IPTabIdx_Mode =  _gu16SetMemfmt_Madi_Motion[eWindow] ;
            }
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
#if (PQ_DDR_SELECT_VER == 1)
            //do nothing, because ddr select rule has no uc item in version1
#elif (PQ_DDR_SELECT_VER == 2)
            else if (u16IPIdx ==  PQ_IP_MADi_DFK_Main)
            {
                u16IPTabIdx_Mode = u16TabIdx_MADi_DFK_DDR;
            }
            else if (u16IPIdx ==  PQ_IP_MADi_SST_Main)
            {
                u16IPTabIdx_Mode =  u16TabIdx_MADi_SST_DDR;
            }
            else if (u16IPIdx ==  PQ_IP_MADi_EODiW_Main)
            {
                u16IPTabIdx_Mode = u16TabIdx_MADi_EODiW_DDR;
            }
            else if (u16IPIdx ==  PQ_IP_UC_CTL_Main)
            {
                u16IPTabIdx_Mode = u16TabIdx_UCCTL_DDR;
            }
#endif
#endif
            else
            {
                u16IPTabIdx_Mode = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
            }

            PQGRULE_DBG(printf("[UC FEATURE]SRC: %u,  uc feature IPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
                _u16PQSrcType[eWindow], i, u16IPIdx, u16IPTabIdx_Mode));
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16IPTabIdx_Mode, u16IPIdx);

            if (u16IPIdx == PQ_IP_UC_CTL_Main)
            {
                u16TabIdx_UCCTL = u16IPTabIdx_Mode;
            }
        }
        if(_gIsEnable8LBMode)
        {
            if(u16TabIdx_UCCTL == PQ_IP_UC_CTL_OFF_Main)
            {
                if(MApi_XC_Set_OPWriteOffEnableToReg)
                {
                    MApi_XC_Set_OPWriteOffEnableToReg(ENABLE, MAIN_WINDOW);
                }
            }
            else
            {
                if(MApi_XC_Set_OPWriteOffEnableToReg)
                {
                    MApi_XC_Set_OPWriteOffEnableToReg(DISABLE, MAIN_WINDOW);
                }
            }
        }
    }
#if(PQ_ENABLE_PIP == 1)
    else // Sub Window
    {
       // Sub wibndow not support UC Feature, Do Nothing
    }
#endif
    if(_gIsEnable8LBMode)
    {
        MDrv_PQBin_Enable8LBMode(pInstance,TRUE);
    }

#if(ENABLE_PQ_MLOAD == 1)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
    return bRet;
}
MS_BOOL MDrv_PQ_EnableUCFeature(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_EnableUCFeature PQArgs;
    PQArgs.eWindow = eWindow;
       PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_ENABLE_UC_FEATURE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}
#endif


MS_BOOL MDrv_PQ_SetUCFeature_U2(void* pInstance,
                                           PQ_WIN eWindow,
                                           MS_U16 u16TabIdx_MADi,
                                           MS_U16 u16TabIdx_MADi_Mot,
                                           MS_U16 u16TabIdx_MADi_DFK,
                                           MS_U16 u16TabIdx_MADi_SST,
                                           MS_U16 u16TabIdx_MADi_EODiW,
                                           MS_U16 u16TabIdx_UCNR,
                                           MS_U16 u16TabIdx_UCDi,
                                           MS_U16 u16TabIdx_UCCTL,
                                           MS_U16 u16TabIdx_SwDriver,
                                           MS_U16 u16TabIdx_PostCCS,
                                           MS_U16 u16TabIdx_DHD
                                           )
{
    MS_BOOL bRet = TRUE;
#if PQ_UC_CTL

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif

    PQMADi_DBG(printf("MDrv_PQ_SetUCFeature(%u, %u, %u,%u,%u,%u,%u,%u,%u,%u,%u)\n",
                                           u16TabIdx_MADi,
                                           u16TabIdx_MADi_Mot,
                                           u16TabIdx_MADi_DFK,
                                           u16TabIdx_MADi_SST,
                                           u16TabIdx_MADi_EODiW,
                                           u16TabIdx_UCNR,
                                           u16TabIdx_UCDi,
                                           u16TabIdx_UCCTL,
                                           u16TabIdx_SwDriver,
                                           u16TabIdx_PostCCS,
                                           u16TabIdx_DHD));

    //no need to store madi, madi_mot here, because MDrv_PQ_LoadTable_U2(pInstance,) has store it.
    _u16MADi_DFK_Current[eWindow] = u16TabIdx_MADi_DFK;
    _u16MADi_SST_Current[eWindow] = u16TabIdx_MADi_SST;
    _u16MADi_EODiW_Current[eWindow] = u16TabIdx_MADi_EODiW;
    _u16UCDi_Current[eWindow] = u16TabIdx_UCDi;
    _u16UCCTL_Current[eWindow] = u16TabIdx_UCCTL;
    _u16UCNR_Current[eWindow] = u16TabIdx_UCNR;
    _u16SwDriver_Current[eWindow] = u16TabIdx_SwDriver;
    _u16PostCCS_Current[eWindow] = u16TabIdx_PostCCS;
    _u16DHD_Current[eWindow] = u16TabIdx_DHD;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(0xFFFF != u16TabIdx_MADi)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi, PQ_IP_MADi_Main);
        }

#if (PQ_MADI_DFK == 1)
        if(0xFFFF != u16TabIdx_MADi_Mot)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_Mot, PQ_IP_MADi_Motion_Main);
        }
#endif

        if(0xFFFF != u16TabIdx_MADi_DFK)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_DFK, PQ_IP_MADi_DFK_Main);
        }

        if(0xFFFF != u16TabIdx_MADi_SST)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_SST, PQ_IP_MADi_SST_Main);
        }

        if(0xFFFF != u16TabIdx_MADi_EODiW)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_EODiW, PQ_IP_MADi_EODiW_Main);
        }

        if(0xFFFF != u16TabIdx_UCNR)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_UCNR, PQ_IP_UCNR_Main);
        }

        if(0xFFFF != u16TabIdx_UCDi)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_UCDi, PQ_IP_UCDi_Main);
        }

        if(0xFFFF != u16TabIdx_UCCTL)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_UCCTL, PQ_IP_UC_CTL_Main);
        }

        if(0xFFFF != u16TabIdx_SwDriver)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_SwDriver, PQ_IP_SwDriver_Main);
        }

        if(0xFFFF != u16TabIdx_PostCCS)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_PostCCS, PQ_IP_PostCCS_Main);
        }

        if(0xFFFF != u16TabIdx_DHD)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_DHD, PQ_IP_DHD_Main);
        }

        if(_gIsEnable8LBMode)
        {
            if(u16TabIdx_UCCTL == PQ_IP_UC_CTL_OFF_Main)
            {
                if(MApi_XC_Set_OPWriteOffEnableToReg)
                {
                    MApi_XC_Set_OPWriteOffEnableToReg(ENABLE, MAIN_WINDOW);
                }
            }
            else
            {
                if(MApi_XC_Set_OPWriteOffEnableToReg)
                {
                    MApi_XC_Set_OPWriteOffEnableToReg(DISABLE, MAIN_WINDOW);
                }
            }
        }
    }
#if(PQ_ENABLE_PIP)
    else
    {
        if(0xFFFF != u16TabIdx_MADi)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi, PQ_IP_MADi_Sub);
        }

        if(0xFFFF != u16TabIdx_MADi_Mot)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_MADi_Mot, PQ_IP_MADi_Motion_Sub);
        }

        if(0xFFFF != u16TabIdx_MADi_DFK)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_MADi_DFK, PQ_IP_MADi_DFK_Sub);
        }

        if(0xFFFF != u16TabIdx_MADi_SST)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_MADi_SST, PQ_IP_MADi_SST_Sub);
        }

        if(0xFFFF != u16TabIdx_SwDriver)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_SwDriver, PQ_IP_SwDriver_Sub);
        }

        #if PQ_SUPPORT_SUB_POSTCCS
        if(0xFFFF != u16TabIdx_PostCCS)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_PostCCS, PQ_IP_PostCCS_Sub);
        }
        #endif

        #if PQ_SUPPORT_SUB_DHD
        if(0xFFFF != u16TabIdx_DHD)
        {
            MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx_DHD, PQ_IP_DHD_Sub);
        }
        #endif
    }
#endif

    if(_gIsEnable8LBMode)
    {
        MDrv_PQBin_Enable8LBMode(pInstance,TRUE);
    }

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

    bRet = TRUE;

#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
    //adjust wrbankmapping for special madi mode such as:25_4R_MC
    if((u16TabIdx_UCNR == PQ_IP_UCNR_OFF_Main)
       && (u16TabIdx_UCDi == PQ_IP_UCDi_OFF_Main)
       && (u16TabIdx_UCCTL == PQ_IP_UC_CTL_OFF_Main))
    {
        if(MApi_XC_GetWRBankMappingNum(eWindow) == 0x0)
        {
            if(_stMode_Info[eWindow].bInterlace)
            {
                MApi_XC_SetWRBankMappingNum(0x2, eWindow);
            }
            else
            {
                MApi_XC_SetWRBankMappingNum(0x1, eWindow);
            }
            _bNeedRestoreWRBankMappingNum = TRUE;
        }
    }
    else if(_bNeedRestoreWRBankMappingNum)
    {
        MApi_XC_SetWRBankMappingNum(0x0, eWindow);
        _bNeedRestoreWRBankMappingNum = FALSE;
    }
#endif

#else
    bRet = FALSE;
#endif
    return bRet;
}
MS_BOOL MDrv_PQ_SetUCFeature(PQ_WIN eWindow,
                                           MS_U16 u16TabIdx_MADi,
                                           MS_U16 u16TabIdx_MADi_Mot,
                                           MS_U16 u16TabIdx_MADi_DFK,
                                           MS_U16 u16TabIdx_MADi_SST,
                                           MS_U16 u16TabIdx_MADi_EODiW,
                                           MS_U16 u16TabIdx_UCNR,
                                           MS_U16 u16TabIdx_UCDi,
                                           MS_U16 u16TabIdx_UCCTL,
                                           MS_U16 u16TabIdx_SwDriver,
                                           MS_U16 u16TabIdx_PostCCS,
                                           MS_U16 u16TabIdx_DHD
                                           )
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetUCFeature PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pu16TabIdx_MADi = u16TabIdx_MADi;
    PQArgs.pu16TabIdx_MADi_Mot = u16TabIdx_MADi_Mot;
    PQArgs.pu16TabIdx_MADi_DFK = u16TabIdx_MADi_DFK;
    PQArgs.pu16TabIdx_MADi_SST = u16TabIdx_MADi_SST;
    PQArgs.pu16TabIdx_MADi_EODiW = u16TabIdx_MADi_EODiW;
    PQArgs.pu16TabIdx_UCNR = u16TabIdx_UCNR;
    PQArgs.pu16TabIdx_UCDi = u16TabIdx_UCDi;
    PQArgs.pu16TabIdx_UCCTL = u16TabIdx_UCCTL;
    PQArgs.pu16TabIdx_SwDriver =u16TabIdx_SwDriver ;
    PQArgs.pu16TabIdx_PostCCS =u16TabIdx_PostCCS ;
    PQArgs.pu16TabIdx_DHD =u16TabIdx_DHD ;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_UC_FEATURE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetDMSV12L_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    MS_U32 u32IsSupportSC1PIP = 0;
    if (MApi_XC_GetChipCaps(E_XC_SUPPORT_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB, &u32IsSupportSC1PIP, sizeof(MS_U32)) != E_APIXC_RET_OK)
    {
        printf("[%s][%d]  MApi_XC_GetChipCaps return fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (u32IsSupportSC1PIP == 1)
    {
        // this is not need to control 12 line mode when sc1 as pip
        return TRUE;
    }
#endif

#if PQ_EN_DMS_SW_CTRL
    MS_U16 u16TabIdx, u16IPIdx;

    if(bEnable == TRUE)
    {
        u16TabIdx = PQ_IP_DMS_V_12L_ON_Main;
    }
    else
    {
        u16TabIdx = PQ_IP_DMS_V_12L_OFF_Main;
    }
    u16IPIdx = PQ_IP_DMS_V_12L_Main;
    eWindow = PQ_MAIN_WINDOW;

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif

    MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx, u16IPIdx);

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
    _bDMSV12L_PIPEn = bEnable;

#else
    UNUSED(eWindow);
    UNUSED(bEnable);
#endif

    return TRUE;
}
MS_BOOL MDrv_PQ_SetDMSV12L(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetDMSV12L PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DMSV12L, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetDMSV12LFromXRuleTable_U2(void* pInstance,PQ_WIN eWindow)
{
    UNUSED(eWindow);

    return _bDMSV12L_PQConfigEn;
}
MS_BOOL MDrv_PQ_GetDMSV12LFromXRuleTable(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetDMSV12LFromXRuleTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_DMSV12L_FROM_XRULE_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

E_DRVPQ_ReturnValue MDrv_PQ_GetLibVer_U2(void* pInstance,const MSIF_Version **ppVersion)
{
    if(ppVersion == NULL)
    {
        return E_DRVPQ_RET_FAIL;
    }

    *ppVersion = &_drv_pq_version;
    return E_DRVPQ_RET_OK;
}
E_DRVPQ_ReturnValue MDrv_PQ_GetLibVer(const MSIF_Version **ppVersion)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetLibVer PQArgs;
    PQArgs.ppVersion = ppVersion;
    PQArgs.bReturnValue = E_DRVPQ_RET_FAIL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_LIB_VER, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return E_DRVPQ_RET_FAIL;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


PQ_ReturnValue MDrv_PQ_GetInfo_U2(void* pInstance,MS_PQ_INFO *pInfo)
{
    memcpy((void*)pInfo, (void*)&_info, sizeof(MS_PQ_INFO));
    return E_PQ_RET_OK;
}
PQ_ReturnValue MDrv_PQ_GetInfo(MS_PQ_INFO *pInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetInfo PQArgs;
    PQArgs.pInfo = pInfo;
    PQArgs.bReturnValue = E_PQ_RET_FAIL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_INFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return E_PQ_RET_FAIL;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL  MDrv_PQ_GetStatus_U2(void* pInstance,MS_PQ_Status *pStatus)
{
    memcpy((void*)pStatus, (void*)&_status, sizeof(MS_PQ_Status));
    return FALSE;
}
MS_BOOL  MDrv_PQ_GetStatus(MS_PQ_Status *pStatus)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetStatus PQArgs;
    PQArgs.pStatus = pStatus;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL  MDrv_PQ_SetDbgLevel_U2(void* pInstance,MS_U16 u16DbgSwitch)
{
    _u16PQDbgSwitch = u16DbgSwitch;
    return TRUE;
}
MS_BOOL  MDrv_PQ_SetDbgLevel(MS_U16 u16DbgSwitch)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return FALSE;
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetDbgLevel PQArgs;
    PQArgs.u16DbgSwitch = u16DbgSwitch;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DBGLEVEL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_MADiForceMotionC_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16DataC = 0;
    u16DataC = Hal_PQ_get_force_c_motion(pInstance,eWindow == PQ_MAIN_WINDOW);

    if(eWindow == PQ_SUB_WINDOW)
    {
        u16DataC  <<= 8;
    }

    if(bEnable)
    {
        u16DataC |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16DataC &= (eWindow == PQ_MAIN_WINDOW ? 0x000F : 0x0F00);
    }
    Hal_PQ_set_force_c_motion(pInstance,eWindow == PQ_MAIN_WINDOW, u16DataC);

}
void MDrv_PQ_MADiForceMotionC(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_MADiForceMotionC PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MADI_FORCE_MOTIONC, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
void MDrv_PQ_MADiForceMotion_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnableY, MS_U16 u16DataY,
                             MS_BOOL bEnableC, MS_U16 u16DataC)
{
    // Y
    if (bEnableY)
    {
        SAVE_FORCE_Y_MOTION_ENABLE(pInstance, eWindow == PQ_MAIN_WINDOW);
        if (eWindow == PQ_SUB_WINDOW)
        {
            u16DataY <<= 8;
        }
        u16DataY |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
        Hal_PQ_set_force_y_motion(pInstance, eWindow == PQ_MAIN_WINDOW, u16DataY);
    }
    else
    {
        RESTORE_FORCE_Y_MOTION_ENABLE(pInstance, eWindow == PQ_MAIN_WINDOW);
    }

    // C
    if (bEnableC)
    {
        SAVE_FORCE_C_MOTION_ENABLE(pInstance, eWindow == PQ_MAIN_WINDOW);
        if (eWindow == PQ_SUB_WINDOW)
        {
            u16DataC <<= 8;
        }
        u16DataC |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
        Hal_PQ_set_force_c_motion(pInstance, eWindow == PQ_MAIN_WINDOW, u16DataC);
    }
    else
    {
        RESTORE_FORCE_C_MOTION_ENABLE(pInstance, eWindow == PQ_MAIN_WINDOW);
    }
}


void MDrv_PQ_MADiForceMotion(PQ_WIN eWindow, MS_BOOL bEnableY, MS_U16 u16DataY,
                             MS_BOOL bEnableC, MS_U16 u16DataC)
{
#ifdef TRACE_DBG
    printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_MADiForceMotion PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnableY = bEnableY;
    PQArgs.u16DataY = u16DataY;
    PQArgs.bEnableC = bEnableC;
    PQArgs.u16DataC = u16DataC;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MADI_FORCE_MOTION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

void MDrv_PQ_MADiForceMotionY_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16Data = 0;

    u16Data = Hal_PQ_get_force_y_motion(pInstance,eWindow == PQ_MAIN_WINDOW);

    if(eWindow == PQ_SUB_WINDOW)
    {
        u16Data <<= 8;
    }
    if(bEnable)
    {
        u16Data |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16Data &= (eWindow == PQ_MAIN_WINDOW ? 0x003F : 0x3F00);
    }

    Hal_PQ_set_force_y_motion(pInstance,eWindow == PQ_MAIN_WINDOW, u16Data);
}
void MDrv_PQ_MADiForceMotionY(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_MADiForceMotionY PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MADI_FORCE_MOTIONY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_SetRFblMode_U2(void* pInstance,MS_BOOL bEnable, MS_BOOL bFilm)
{
    _stRFBL_Info.bEnable = bEnable;
    _stRFBL_Info.bFilm = bFilm;

    if(!bEnable)    //default mode
    {
        MDrv_PQ_MADiForceMotionY_U2(pInstance,PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC_U2(pInstance,PQ_MAIN_WINDOW, FALSE);

        _stRFBL_Info.u8MADiType = MS_MADI_24_2R;
        //printf("[%s]-[%d] : Y/C motion : ( 0 , 0 ) \n", __FUNCTION__, __LINE__);
    }
    else
    {
        _stRFBL_Info.u8MADiType = (bFilm) ? (MS_U8)MS_MADI_24_RFBL_FILM : (MS_U8)MS_MADI_24_RFBL_NFILM;
        MDrv_PQ_MADiForceMotionY_U2(pInstance,PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC_U2(pInstance,PQ_MAIN_WINDOW, (!bFilm));

        //printf("[%s]-[%d] : Y/C motion : ( 0 , %d) \n", __FUNCTION__, __LINE__,(!bFilm));
    }
}
void MDrv_PQ_SetRFblMode(MS_BOOL bEnable, MS_BOOL bFilm)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetRFblMode PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.bFilm = bFilm;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_RFBL_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_GetRFblMode_U2(void* pInstance,MS_PQ_RFBL_INFO * pstInfo)
{
    memcpy(pstInfo, &_stRFBL_Info, sizeof(MS_PQ_RFBL_INFO));
    return;
}
void MDrv_PQ_GetRFblMode(MS_PQ_RFBL_INFO * pstInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_GetRFblMode PQArgs;
    PQArgs.pstInfo = pstInfo;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_RFBL_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#if (PQ_ENABLE_VD_SAMPLING)

MS_BOOL MDrv_PQ_Get_VDSampling_Info_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo)
{
    PQ_VIDEOSTANDARD_TYPE eCurStandard = _stVD_Info[eWindow].enVideoStandard;
    MS_U16 u16CurSrc = _u16PQSrcType[eWindow];
    MS_U16 u16TableIdx;
#if PQ_HAL_EWINDOW_PARAM
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_VD_SAMPLING, eWindow);
#else
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_VD_SAMPLING);
#endif
    MS_U8 u8SampleTable[10];

    memset(u8SampleTable, 0, sizeof(u8SampleTable));

    PQINFO_DBG(printf("[PQ] GetVDSampling: win:%d, src:%d, standard:%d, PQIP=%x\n", eWindow, enInputSourceType, eStandard, u16IPIdx));
    if((((enInputSourceType >= PQ_INPUT_SOURCE_TV) && (enInputSourceType <= PQ_INPUT_SOURCE_SVIDEO))||(enInputSourceType == PQ_INPUT_SOURCE_SCART)) &&
            (eStandard < E_PQ_VIDEOSTANDARD_NOTSTANDARD) &&
            (u16IPIdx != 0xFFFF))
    {
        _stVD_Info[eWindow].enVideoStandard = eStandard;
        _u16PQSrcType[eWindow] = QM_InputSourceToIndex(pInstance, eWindow, enInputSourceType);



        u16TableIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
        PQINFO_DBG(printf("[PQ] VDSampling: TableIdx=%d\n", u16TableIdx));

        if(((gbPQBinEnable == 1) && (u16TableIdx == PQ_BIN_IP_NULL))
                || ((gbPQBinEnable == 0) && (u16TableIdx == PQ_IP_NULL)))
        {
            pInfo->eType = PQ_VD_SAMPLING_OFF;
        }
        else
        {
            MDrv_PQ_LoadTableData_U2(pInstance, eWindow, u16TableIdx, u16IPIdx, u8SampleTable, 10);
            pInfo->u16Vstart = ((MS_U16)u8SampleTable[0]) | (((MS_U16)u8SampleTable[1])<<8);
            pInfo->u16Hstart = ((MS_U16)u8SampleTable[2]) | (((MS_U16)u8SampleTable[3])<<8);
            pInfo->u16Hsize  = ((MS_U16)u8SampleTable[4]) | (((MS_U16)u8SampleTable[5]) << 8);
            pInfo->u16Vsize  = ((MS_U16)u8SampleTable[6]) | (((MS_U16)u8SampleTable[7]) << 8);
            pInfo->u16Htt    = ((MS_U16)u8SampleTable[8]) | (((MS_U16)u8SampleTable[9]) << 8);
            pInfo->eType = PQ_VD_SAMPLING_ON;

            PQINFO_DBG(printf("VD x:%x, y:%x, w:%x, h:%x, Htt=%x \n",
                    pInfo->u16Hstart, pInfo->u16Vstart, pInfo->u16Hsize, pInfo->u16Vsize, pInfo->u16Htt));
        }

        _stVD_Info[eWindow].enVideoStandard  = eCurStandard;
        _u16PQSrcType[eWindow] = u16CurSrc;
    }
    else
    {
        pInfo->eType = PQ_VD_SAMPLING_OFF;
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_Get_VDSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_Get_VDSampling_Info PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enInputSourceType = enInputSourceType;
    PQArgs.eStandard = eStandard;
    PQArgs.pInfo = pInfo;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_VDSAMPLING_INFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

#endif


MS_BOOL _MDrv_PQ_Get_HSDSampling(void *pInstance,PQ_WIN eWindow, MS_U32 *pu32ratio, MS_BOOL *bADVMode)
{
#if PQ_HAL_EWINDOW_PARAM
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_HSD_SAMPLING, eWindow);
#else
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_HSD_SAMPLING);
#endif
    MS_U16 u16TableIdx;
    MS_BOOL bret = TRUE;

    if(u16IPIdx != 0xFFFF)
    {
        u16TableIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
    }
    else
    {
        u16TableIdx = (gbPQBinEnable) ? PQ_BIN_IP_NULL : PQ_IP_NULL;
    }


    if(((gbPQBinEnable == 1) && (u16TableIdx == PQ_BIN_IP_NULL)) ||
            ((gbPQBinEnable == 0) && (u16TableIdx == PQ_IP_NULL)))
    {
        *pu32ratio = 0;
        bret = FALSE;
    }
    else if( (( gbPQBinEnable == 1) &&
       (( MDrv_PQBin_GetSkipRule(pInstance,PQ_IP_HSD_Y_Main, &stPQBinHeaderInfo[PQ_BIN_STD_MAIN])) ||
            ( MDrv_PQBin_GetSkipRule(pInstance,PQ_IP_HSD_C_Main, &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]))) )
            ||
       (( gbPQBinEnable == 1) &&
        ((MST_SkipRule_IP_Main[PQ_IP_HSD_Y_Main] == 1) || (MST_SkipRule_IP_Main[PQ_IP_HSD_C_Main] == 1))))
    {
        SRULE_DBG(printf("HSD_Y, HSD_C is True in SKipRule, driver consider HSDSampling is not defined in QMap.\n"));
        *pu32ratio = 0;
        bret = FALSE;
    }
    else
    {
#if PQ_NEW_HSD_SAMPLING_TYPE
#if PQ_HAL_EWINDOW_PARAM
        if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_000, eWindow))
#else
        if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_000))
#endif
        {
            *pu32ratio = 1000;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_125, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_125))
#endif
        {
            *pu32ratio = 875;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_250, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_250))
#endif
        {
            *pu32ratio = 750;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_375, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_375))
#endif
        {
            *pu32ratio = 625;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_500, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_500))
#endif
        {
            *pu32ratio = 500;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_625, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_625))
#endif
        {
            *pu32ratio = 375;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_750, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_750))
#endif
        {
            *pu32ratio = 250;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_875, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_1_875))
#endif
        {
            *pu32ratio = 125;
        }
#if PQ_HAL_EWINDOW_PARAM
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_2_000, eWindow))
#else
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(pInstance,MS_HSD_SAMPLING_Div_2_000))
#endif
        {
            *pu32ratio = 1;
        }
        else
#endif
        {
            *pu32ratio = 0;
        }

        bret = TRUE;
    }

    // Load HSD_Y & HSD_C
    if(bret)
    {
        MS_U16 u16HSD_Y_idx, u16HSD_C_idx;

        u16HSD_Y_idx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_HSD_Y_Main);
        MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16HSD_Y_idx, PQ_IP_HSD_Y_Main);

        u16HSD_C_idx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_HSD_C_Main);
        MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16HSD_C_idx, PQ_IP_HSD_C_Main);

        SRULE_DBG(printf("HSD_Y:%d, HSD_C:%d\n", u16HSD_Y_idx, u16HSD_C_idx));

        *bADVMode = ((u16HSD_Y_idx == PQ_IP_HSD_Y_CB_Main) &&
                     (u16HSD_C_idx == PQ_IP_HSD_C_CB_Main)) ? FALSE : TRUE;
    }
    else
    {
        *bADVMode = FALSE;
    }


    SRULE_DBG(printf("[PQ] GetHSDSampling, IPIdx=%d, Table=%d, ratio=%td ret=%d\n", u16IPIdx, u16TableIdx, (ptrdiff_t) *pu32ratio, bret));

    return bret;
}

//get_figure
MS_BOOL MDrv_PQ_IOCTL_U2(void* pInstance,PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bret = TRUE;

    switch(u32Flag)
    {
        case E_IOCTL_HSD_SAMPLING:
        {
            if(u32BufSize == sizeof(PQ_HSD_SAMPLING_INFO))
            {
                MS_U32 u32ratio;
                MS_BOOL bADVMode;

                bret = _MDrv_PQ_Get_HSDSampling(pInstance,eWindow, &u32ratio, &bADVMode);
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = u32ratio;
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = bADVMode;
                //printf("ratio %lx, bADVmode=%d\n\n",((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio, ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode);
            }
            else
            {
                //printf("Size is not correct, in=%ld, %d\n", u32BufSize, sizeof(PQ_HSD_SAMPLING_INFO));
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = 0;
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = 0;
                bret = FALSE;
            }
            break;
        }

        case E_IOCTL_PREVSD_BILINEAR:
            bret = TRUE;
            *((MS_BOOL *)pBuf) = TRUE;
            break;

        case E_IOCTL_ADC_SAMPLING:
            if(u32BufSize == sizeof(PQ_ADC_SAMPLING_INFO))
            {

                PQ_INPUT_SOURCE_TYPE enInputSrc = ((PQ_ADC_SAMPLING_INFO *)pBuf)->enPQSourceType;
                MS_PQ_Mode_Info      *pModeInfo = &(((PQ_ADC_SAMPLING_INFO *)pBuf)->stPQModeInfo);
                MS_PQ_ADC_SAMPLING_TYPE enADCSamplingType = MDrv_PQ_Get_ADCSampling_Info(eWindow, enInputSrc, pModeInfo);

                if((enADCSamplingType == E_PQ_ADC_SAMPLING_NOT_SUPPORT) || (enADCSamplingType == E_PQ_ADC_SAMPLING_NONE))
                {
                    ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = 1;
                    bret = FALSE;
                }
                else
                {
                    ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = (MS_U16)enADCSamplingType;
                    bret = TRUE;
                }
            }
            else
            {
                bret = FALSE;
                ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = 1;
            }
            break;
        case E_IOCTL_RFBL_CTRL:
            //printf("IOCTL %lx\n", u32Flag);
#if PQ_ENABLE_RFBL
            bret = MDrv_PQ_Get_RFBL_Info(eWindow);
#else
            bret = FALSE;
#endif
            UNUSED(pBuf);
            UNUSED(u32BufSize);
            break;

        case E_IOCTL_PQ_SUGGESTED_FRAMENUM:
        {
            if(u32BufSize == sizeof(MS_U16))
            {
                *((MS_U16 *)pBuf) = _u16PQSuggestedFrameNum[eWindow];
                bret = TRUE;
            }
            else
            {
                *((MS_U16 *)pBuf) = 0;
                bret = FALSE;
            }
            break;
        }
        case E_IOCTL_SET_UCFEATURE:
        {
#if PQ_UC_CTL
#if(PQ_GRULE_GAME_MODE_ENABLE == 0)
            MS_U16 u16MADi_DFK_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16MADi_SST_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16MADi_EODiW_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16UCNR_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16UCDi_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16UCCTL_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16PostCCS_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16SwDriver_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
            MS_U16 u16DHD_ToSet[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
#endif
            MS_BOOL bDisableUCFeature = FALSE;

            if(u32BufSize == sizeof(MS_BOOL))
            {
                _gIsEnable8LBMode = *((MS_BOOL *)pBuf);
            }
            else
            {
                _gIsEnable8LBMode = FALSE;
            }
#if (PQ_DISABLE_UC_FOR_PREVSD)
            XC_ApiStatus stXCApiStatus;
            memset(&stXCApiStatus, 0, sizeof(XC_ApiStatus));
            if (MApi_XC_GetStatus(&stXCApiStatus, MAIN_WINDOW) == FALSE)
            {
                printf("MApi_XC_GetStatus failed.\n");
            }
            if(stXCApiStatus.bPreV_ScalingDown)
            {
                bDisableUCFeature = TRUE;
            }
            else
#endif
            if(MApi_SC_IsForceFreerun() || MApi_XC_IsCurrentFrameBufferLessMode())//can not support ucnr when freerun or fbl
            {
                bDisableUCFeature = TRUE;
            }
            else if((((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_FRAME_R == MApi_XC_Get_3D_Output_Mode())
                            || (E_XC_3D_OUTPUT_FRAME_L == MApi_XC_Get_3D_Output_Mode())
                          )&& (1 <= MApi_XC_Get_3D_HW_Version()))
                        || ((MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)&& (5 == MApi_XC_Get_3D_HW_Version()))//monaco 3d disable uc
                   )  // 3d version >=2 && using sub window
            {
                bDisableUCFeature = TRUE;
            }
#if PQ_DISABLE_UC_FOR_PIP
            else if(MApi_XC_Is_SubWindowEanble()
                   || _enPQDisplayType != PQ_DISPLAY_ONE) //disable ucnr when sub window bit is setting(pip/pop)
            {
                bDisableUCFeature = TRUE;
            }
            else if(eWindow == PQ_SUB_WINDOW)//sub window need disable ucnr for  dual view/travelingmode enable sub window bit after setwindow
            {
                bDisableUCFeature = TRUE;
            }
#endif
            PQMADi_DBG(printf("E_IOCTL_SET_UCFEATURE: Disable UC=%u\n", bDisableUCFeature));
            if(bDisableUCFeature == TRUE)
            {
                MDrv_PQ_DisableUCFeature(PQ_MAIN_WINDOW);
            }
            else
            {
#if(PQ_GRULE_GAME_MODE_ENABLE == 1)
                MDrv_PQ_EnableUCFeature_U2(pInstance, eWindow);
#else
                MDrv_PQ_GetUCFeatureFromPQ(eWindow, &u16MADi_DFK_ToSet[eWindow], &u16MADi_SST_ToSet[eWindow], &u16MADi_EODiW_ToSet[eWindow],
                    &u16UCNR_ToSet[eWindow], &u16UCDi_ToSet[eWindow], &u16UCCTL_ToSet[eWindow],
                    &u16SwDriver_ToSet[eWindow], &u16PostCCS_ToSet[eWindow], &u16DHD_ToSet[eWindow]);
#if PQ_DISABLE_UC_FOR_PIP
                MDrv_PQ_SetUCFeature(eWindow, _gu16SetMemfmt_Madi[eWindow], _gu16SetMemfmt_Madi_Motion[eWindow], u16MADi_DFK_ToSet[eWindow],
                    u16MADi_SST_ToSet[eWindow], u16MADi_EODiW_ToSet[eWindow], u16UCNR_ToSet[eWindow], u16UCDi_ToSet[eWindow],
                    u16UCCTL_ToSet[eWindow], u16SwDriver_ToSet[eWindow], u16PostCCS_ToSet[eWindow], u16DHD_ToSet[eWindow]);
#else
                if(eWindow == PQ_SUB_WINDOW)
                {
                    //no need to set madi/madi_mot again, because setmemformat has done it. so here pass 0xffff in
                    MDrv_PQ_SetUCFeature(eWindow, 0xFFFF, 0xFFFF, u16MADi_DFK_ToSet[eWindow],
                        u16MADi_SST_ToSet[eWindow], u16MADi_EODiW_ToSet[eWindow], u16UCNR_ToSet[eWindow], u16UCDi_ToSet[eWindow],
                        u16UCCTL_ToSet[eWindow], u16SwDriver_ToSet[eWindow], u16PostCCS_ToSet[eWindow], u16DHD_ToSet[eWindow]);
                }
                else
                {
                    MDrv_PQ_SetUCFeature(eWindow, _s32LastPQIPMADiMain, _s32LastPQIPMADiMotMain, u16MADi_DFK_ToSet[eWindow],
                        u16MADi_SST_ToSet[eWindow], u16MADi_EODiW_ToSet[eWindow], u16UCNR_ToSet[eWindow], u16UCDi_ToSet[eWindow],
                        u16UCCTL_ToSet[eWindow], u16SwDriver_ToSet[eWindow], u16PostCCS_ToSet[eWindow], u16DHD_ToSet[eWindow]);
                }
#endif
#endif
            }
#endif
            break;
        }
        default:
            printf("unknown IOCTL %tx\n", (ptrdiff_t) u32Flag);
            UNUSED(pBuf);
            UNUSED(u32BufSize);
            bret = FALSE;
            break;
    }

    //printf("[PQ] IOCTL : win:%x, Flag=%08lx, size=%ld, ret:%x\n", eWindow, u32Flag, u32BufSize, bret);

    return bret;
}
MS_BOOL MDrv_PQ_IOCTL(PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_INTER_IOCTL PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u32Flag = u32Flag;
    PQArgs.pBuf = pBuf;
    PQArgs.u32BufSize = u32BufSize;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_INTER_IOCTL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{
    MS_PQ_ADC_SAMPLING_TYPE enRetType;
    MS_U16 u16PQSrcType;
    MS_U16 u16TableIdx;
    MS_PQ_Mode_Info stModeInfo;

    u16PQSrcType = _u16PQSrcType[eWindow];
    memcpy(&stModeInfo, &_stMode_Info[eWindow], sizeof(MS_PQ_Mode_Info));

    MDrv_PQ_Set_ModeInfo_U2(pInstance, eWindow, enInputSourceType, pstPQModeInfo);
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(pInstance, eWindow, enInputSourceType);

#if PQ_HAL_EWINDOW_PARAM
    if(Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_ADC_SAMPLING, eWindow) == 0xFFFF)
#else
    if(Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_ADC_SAMPLING) == 0xFFFF)
#endif
    {
        return E_PQ_ADC_SAMPLING_NOT_SUPPORT;
    }

#if PQ_HAL_EWINDOW_PARAM
    u16TableIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_ADC_SAMPLING, eWindow));
#else
    u16TableIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, Hal_PQ_get_ip_idx(pInstance,MS_PQ_IP_ADC_SAMPLING));
#endif

#if PQ_HAL_EWINDOW_PARAM
    if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_1,eWindow))
#else
    if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_1))
#endif
    {
        enRetType = E_PQ_ADC_SAMPLING_X_1;
    }
#if PQ_HAL_EWINDOW_PARAM
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_2,eWindow))
#else
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_2))
#endif
    {
        enRetType = E_PQ_ADC_SAMPLING_X_2;
    }
#if PQ_HAL_EWINDOW_PARAM
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_4,eWindow))
#else
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(pInstance,MS_ADC_SAMPLING_X_4))
#endif
    {
        enRetType = E_PQ_ADC_SAMPLING_X_4;
    }
    else
    {
        enRetType = E_PQ_ADC_SAMPLING_NONE;
    }

    memcpy(&_stMode_Info[eWindow], &stModeInfo, sizeof(MS_PQ_Mode_Info));
    _u16PQSrcType[eWindow] = u16PQSrcType;

    return enRetType;
}
MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_ADCSampling_Info PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enInputSourceType = enInputSourceType;
    PQArgs.pstPQModeInfo = pstPQModeInfo;
    PQArgs.bReturnValue = E_PQ_ADC_SAMPLING_NONE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_ADC_SAMPLING_INTO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return E_PQ_ADC_SAMPLING_NONE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Set3D_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gIs3D_En = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_Set3D_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set3D_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_3D_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetCaps_U2(void* pInstance,MS_PQ_CAP_INFO *pInfo)
{
    MS_BOOL bRet = TRUE;
#if(PQ_ENABLE_PIP)
    const XC_ApiInfo* pXC_ApiInfo = MApi_XC_GetInfo();
    if (pXC_ApiInfo != NULL)
    {
        pInfo->bPIP_Supported = ((pXC_ApiInfo->u8MaxWindowNum) >= 2) ? TRUE : FALSE;
    }
    else
    {
        pInfo->bPIP_Supported = FALSE;
        bRet = FALSE;
    }
#else
    pInfo->bPIP_Supported = FALSE;
#endif

#if(PQ_ENABLE_3D_VIDEO)
    pInfo->b3DVideo_Supported = TRUE;
#else
    pInfo->b3DVideo_Supported = FALSE;
#endif

    if(pInfo->bPIP_Supported)
    {
        pInfo->b4K2KPIP_Supported = PQ_BW_4K2KPIP_SUPPORT;
    }
    else
    {
        pInfo->b4K2KPIP_Supported = FALSE;
    }

    return bRet;
}
MS_BOOL MDrv_PQ_GetCaps(MS_PQ_CAP_INFO *pInfo)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return FALSE;
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_GetCaps PQArgs;
    PQArgs.pInfo = pInfo;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CAPS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDRV_PQ_PrintLoadTableInfo_U2(void* pInstance,MS_PQ_PQLOADTABLE_WIN enPQWin)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)

    MS_U16 u16IPIdx;
    MS_U8  u8TabIdx;

    if(enPQWin == PQ_LOAD_TABLE_MAIN)
        printf("[TableInfo::Main] \n");
    else if(enPQWin == PQ_LOAD_TABLE_MAIN_EX)
        printf("[TableInfo::Main Ex] \n");
    else if(enPQWin == PQ_LOAD_TABLE_SUB)
        printf("[TableInfo::SUB] \n");
    else if(enPQWin == PQ_LOAD_TABLE_SUB_EX)
        printf("[TableInfo::SUB Ex] \n");
    else
        return;

    for(u16IPIdx = 0; u16IPIdx < PQ_IP_NUM_Main; ++ u16IPIdx)
    {
        printf(" %u(%s)=", u16IPIdx, MDrv_PQ_GetIPName(u16IPIdx));  // IP Name

        if(enPQWin == PQ_LOAD_TABLE_MAIN)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(MAIN,pInstance, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("%u(%s)\n", u8TabIdx, MDrv_PQ_GetTableName(u16IPIdx, u8TabIdx));
        }
        else if(enPQWin == PQ_LOAD_TABLE_MAIN_EX)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(MAINEX,pInstance, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("Tab%u\n", u8TabIdx);
        }

#if PQ_ENABLE_PIP
        else if(enPQWin == PQ_LOAD_TABLE_SUB)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(SUB,pInstance, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("%u(%s)\n", u8TabIdx, MDrv_PQ_GetTableName(u16IPIdx, u8TabIdx));
        }
        else if(enPQWin == PQ_LOAD_TABLE_SUB_EX)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(SUBEX,pInstance, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("Tab%u\n", u8TabIdx);

        }
#endif
        else
        {
        }
    }
#else
    UNUSED(enPQWin);
#endif
}
void MDRV_PQ_PrintLoadTableInfo(MS_PQ_PQLOADTABLE_WIN enPQWin)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_PrintLoadTableInfo PQArgs;
    PQArgs.enPQWin = enPQWin;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_PRINT_LOADTABLE_INFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_SetDS_OnOFF_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEn)
{
    //store the original value
    _u16RW_Method = Hal_PQ_get_rw_method(pInstance,eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
    if(bEn)
    {
        _bDS_En = TRUE;
    }
    else
    {
        _bDS_En = FALSE;
        Hal_PQ_set_rw_method(pInstance,eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE, _u16RW_Method);
    }

    return TRUE;
}
MS_BOOL MDrv_PQ_SetDS_OnOFF(PQ_WIN eWindow, MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetDS_OnOFF PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SETDS_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Get_RFBL_Info_U2(void* pInstance,PQ_WIN eWindow)
{
    //useless
    UNUSED(eWindow);
    return FALSE;
}
MS_BOOL MDrv_PQ_Get_RFBL_Info(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_RFBL_Info PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_RFBL_INFO, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


MS_BOOL MDrv_PQ_CheckHDMode_U2(void* pInstance,PQ_WIN eWindow)
{
    return _gIsSrcHDMode[eWindow];
}
MS_BOOL MDrv_PQ_CheckHDMode(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_CheckHDMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CHECK_HD_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//for Super Resolution interlace and SD mode detected use
MS_BOOL MDrv_PQ_Get_Interlace_SD_mode_U2(void* pInstance)
{
    if(QM_IsInterlaced(PQ_MAIN_WINDOW) && (QM_GetInputHSize(PQ_MAIN_WINDOW) < 1500))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
MS_BOOL MDrv_PQ_Get_Interlace_SD_mode(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_Interlace_SD_mode PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_INTERLACE_SD_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetDualViewState_U2(void* pInstance,MS_BOOL bEnable)
{
    _bDualViewEnable = bEnable;
    MDrv_PQ_ForceVideoInputMode(bEnable, PQ_MAIN_WINDOW);
    MDrv_PQ_ForceVideoInputMode(bEnable, PQ_SUB_WINDOW);
    return TRUE;
}
MS_BOOL MDrv_PQ_SetDualViewState(MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetDualViewState PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DUAL_VIEW_STATE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetVGASubCaseForceTo422Mode_U2(void* pInstance)
{
    return PQ_VGASubCaseForceTo422Mode;
}
MS_BOOL MDrv_PQ_GetVGASubCaseForceTo422Mode(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetVGASubCaseForceTo422Mode PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_VGASUBCASE_FORCETO_422MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetDualViewState_U2(void* pInstance)
{
    return _bDualViewEnable;
}
MS_BOOL MDrv_PQ_GetDualViewState(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetDualViewState PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_DUAL_VIEW_STATE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//Is Force Input to do IP_CSC
MS_BOOL MDrv_PQ_ForceVideoInputMode_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow)
{
    _bForceVideoInputMode[eWindow] = bEnable;
    return TRUE;
}
MS_BOOL MDrv_PQ_ForceVideoInputMode(MS_BOOL bEnable, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_ForceVideoInputMode PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_FORCE_VIDEO_INPUT_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//Force Input to do IP_CSC
MS_BOOL MDrv_PQ_IsForceVideoInputMode_U2(void* pInstance,PQ_WIN eWindow)
{
    if(_bForceVideoInputMode[eWindow])
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
MS_BOOL MDrv_PQ_IsForceVideoInputMode(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_IsForceVideoInputMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_IS_FORCE_VIDEO_INPUTMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Set_PointToPoint_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,
                               MS_BOOL bEnable,
                               PQ_WIN eWindow)
{
    //sub window don't support point to point
    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }

    if(bEnable)
    {
        //Center the display window
        if(g_IPanel.Width() >= pstXC_SetWin_Info->stCropWin.width)
        {
            pstXC_SetWin_Info->stDispWin.x = (g_IPanel.Width() - pstXC_SetWin_Info->stCropWin.width)/2;
            pstXC_SetWin_Info->stDispWin.width = pstXC_SetWin_Info->stCropWin.width;
        }
        else
        {
            pstXC_SetWin_Info->stDispWin.x = 0;
            pstXC_SetWin_Info->stDispWin.width = g_IPanel.Width();
        }

        if(g_IPanel.Height() >= pstXC_SetWin_Info->stCropWin.height)
        {
            pstXC_SetWin_Info->stDispWin.y = (g_IPanel.Height() - pstXC_SetWin_Info->stCropWin.height)/2;
            pstXC_SetWin_Info->stDispWin.height = pstXC_SetWin_Info->stCropWin.height;
        }
        else
        {
            pstXC_SetWin_Info->stDispWin.y = 0;
            pstXC_SetWin_Info->stDispWin.height = g_IPanel.Height();
        }

        //RGB HDMI, DVI, VGA
        if((((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType))) &&
             (!pstXC_SetWin_Info->bInterlace))
        {
            if(_bP2pForceToDoCsc && _enOutputColorFmt == E_PQ_COLOR_YUV444)
            {
                MApi_XC_EnableForceRGBin(FALSE,  MAIN_WINDOW);
            }
            else
            {
                MApi_XC_EnableForceRGBin(TRUE,  MAIN_WINDOW);
            }
        }
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_444, eWindow);
        MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_444, eWindow);
    }
    else
    {
        MApi_XC_EnableForceRGBin(FALSE,  MAIN_WINDOW);
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_AUTO, eWindow);
        MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_AUTO, eWindow);
    }

    PQP2P_DBG(printf("[MDrv_PQ_Set_PointToPoint] True or False: %u\n",bEnable);)
    _bPointToPointMode = bEnable;

    return TRUE;
}
MS_BOOL MDrv_PQ_Set_PointToPoint(XC_SETWIN_INFO *pstXC_SetWin_Info,
                               MS_BOOL bEnable,
                               PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set_PointToPoint PQArgs;
    PQArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_POINT2POINT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Get_PointToPoint_U2(void* pInstance,PQ_WIN eWindow)
{
    //Only Main window support point to point
    if(eWindow == PQ_MAIN_WINDOW)
    {
        return _bPointToPointMode;
    }
    else
    {
        return FALSE;
    }
}
MS_BOOL MDrv_PQ_Get_PointToPoint(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_PointToPoint PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_POINT2POINT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Set_PqBypass_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,
                               MS_BOOL bEnable,
                               PQ_WIN eWindow)
{
    //sub window don't support PQ bypass
    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }

    if(bEnable)
    {
        //RGB HDMI, DVI, VGA
        if((((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType))) &&
             (!pstXC_SetWin_Info->bInterlace))
        {
            MApi_XC_EnableForceRGBin(TRUE,  MAIN_WINDOW);
        }
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_444, eWindow);
        MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_444, eWindow);
    }
    else
    {
        MApi_XC_EnableForceRGBin(FALSE,  MAIN_WINDOW);
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_AUTO, eWindow);
        MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_AUTO, eWindow);
    }

    printf("[MDrv_PQ_Set_PqBypass] True or False: %u\n",bEnable);
    _bPqBypassMode = bEnable;

    return TRUE;
}

MS_BOOL MDrv_PQ_Set_PqBypass(XC_SETWIN_INFO *pstXC_SetWin_Info,
                               MS_BOOL bEnable,
                               PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set_PqBypass PQArgs;
    PQArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_PQBYPASS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Get_PqBypass_U2(void* pInstance,PQ_WIN eWindow)
{
    //Only Main window support PQ bypass
    if(eWindow == PQ_MAIN_WINDOW)
    {
        return _bPqBypassMode;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_PQ_Get_PqBypass(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_PqBypass PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_POINT2POINT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Update_MemFormat_U2(void* pInstance)
{
    MS_U8 u8BitPerPixel = 0;
    MDrv_PQ_SetMemFormat(
        PQ_MAIN_WINDOW,
        _gbMemfmt422[PQ_MAIN_WINDOW],
        _stMode_Info[PQ_MAIN_WINDOW].bFBL,
        &u8BitPerPixel);

    return TRUE;
}
MS_BOOL MDrv_PQ_Update_MemFormat(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Update_MemFormat PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_UPDATE_MEMFORMAT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U16 MDrv_PQ_GetHsize_U2(void* pInstance,PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].u16input_hsize;
}
MS_U16 MDrv_PQ_GetHsize(PQ_WIN enPQWin)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetHsize PQArgs;
    PQArgs.enPQWin = enPQWin;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_HSIZE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetVsize_U2(void* pInstance,PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].u16input_vsize;
}
MS_U16 MDrv_PQ_GetVsize(PQ_WIN enPQWin)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_GetVsize PQArgs;
    PQArgs.enPQWin = enPQWin;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_VSIZE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_BOOL MDrv_PQ_IsInterlace_U2(void* pInstance,PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].bInterlace;
}
MS_BOOL MDrv_PQ_IsInterlace(PQ_WIN enPQWin)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_IsInterlace PQArgs;
    PQArgs.enPQWin = enPQWin;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_IS_INTERLACE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_LOW_3dQuality_U2(void* pInstance,MS_BOOL bEnable)
{
    if(bEnable)
    {
#if PQ_VIP_CTL
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_Peaking_band_AV_NTSC_Main, PQ_IP_VIP_Peaking_band_Main);
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_VNMR_S2_Main, PQ_IP_VIP_VNMR_Main);
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_VLPF_coef1_0x6_Main, PQ_IP_VIP_VLPF_coef1_Main);
#endif
    }
    else
    {
#if PQ_VIP_CTL
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_Peaking_band_Main), PQ_IP_VIP_Peaking_band_Main);
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_VNMR_Main), PQ_IP_VIP_VNMR_Main);
        MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_VLPF_coef1_Main), PQ_IP_VIP_VLPF_coef1_Main);
#endif
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_LOW_3dQuality(MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_LOW_3dQuality PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOW_3D_QUALITY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_MWE_SetEnhanceQuality_U2(void* pInstance)
{
#if PQ_VIP_RGBCMY_CTL
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_R_INC_3_Main, PQ_IP_VIP_ICC_R_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_G_INC_8_Main, PQ_IP_VIP_ICC_G_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_B_INC_6_Main, PQ_IP_VIP_ICC_B_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_C_INC_6_Main, PQ_IP_VIP_ICC_C_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_M_INC_6_Main, PQ_IP_VIP_ICC_M_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_Y_INC_7_Main, PQ_IP_VIP_ICC_Y_Main);
#elif PQ_VIP_RGBCMY_CTL_VER2
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_SETTING_Enhance_6Color_Main, PQ_IP_VIP_ICC_SETTING_Main);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_MWE_SetEnhanceQuality(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_MWE_SetEnhanceQuality PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MWE_SET_ENHANCE_QUALITY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_MWE_RestoreEnhanceQuality_U2(void* pInstance)
{
#if PQ_VIP_RGBCMY_CTL
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_R_Main), PQ_IP_VIP_ICC_R_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_G_Main), PQ_IP_VIP_ICC_G_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_B_Main), PQ_IP_VIP_ICC_B_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_C_Main), PQ_IP_VIP_ICC_C_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_M_Main), PQ_IP_VIP_ICC_M_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_Y_Main), PQ_IP_VIP_ICC_Y_Main);
#elif PQ_VIP_RGBCMY_CTL_VER2
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_SETTING_Main), PQ_IP_VIP_ICC_SETTING_Main);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_MWE_RestoreEnhanceQuality(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_MWE_RestoreEnhanceQuality PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MWE_RESTORE_ENHANCE_QUALITY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_MWE_RestoreOffQuality_U2(void* pInstance)
{
#if PQ_VIP_CTL
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_ON_Main, PQ_IP_VIP_ICC_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_IBC_ON_Main, PQ_IP_VIP_IBC_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_IHC_ON_Main, PQ_IP_VIP_IHC_Main);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_MWE_RestoreOffQuality(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_MWE_RestoreOffQuality PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MWE_RESTORE_OFF_QUALITY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Demo_CloneWindow_U2(void* pInstance, PQ_WIN eWindow, MS_U8 u8Mode)
{
#if (PQ_DEMO_MODE_ENABLE == 1)
    if(u8Mode >= PQ_IP_MWE_diff_no_comm_NUMS_Main)
    {
        printf("%s %d: Not support the PQ Mode:%d\n",__FUNCTION__,__LINE__,u8Mode);
        return FALSE;
    }

#if (PQ_QM_MWE_CLONE_VER1 == 1)
    MDrv_PQ_CloneTable_U2(pInstance, eWindow, PQ_IP_MWE_Clone_main_no_comm_OFF_Main, PQ_IP_MWE_Clone_main_no_comm_Main, PQ_IP_MWE_Clone_sub_no_comm_Main);
    MApi_XC_ACE_SetMWELoadFromPQ(TRUE);
    MDrv_PQ_LoadTable_U2(pInstance, eWindow, u8Mode, PQ_IP_MWE_diff_no_comm_Main);
#elif (PQ_QM_MWE_CLONE_VER2 == 1)
    MDrv_PQ_CloneTable(pInstance, eWindow, PQ_IP_Clone_main_no_comm_OFF_Main, PQ_IP_Clone_main_no_comm_Main, PQ_IP_Clone_sub_no_comm_Main);
    MApi_XC_ACE_SetMWELoadFromPQ(TRUE);
    MDrv_PQ_LoadTable_U2(pInstance, eWindow, u8Mode, PQ_IP_MWE_diff_no_comm_Main);
#else
    UNUSED(eWindow);
#endif
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_Demo_CloneWindow(PQ_WIN eWindow, MS_U8 u8Mode)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Demo_CloneWindow PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Mode = u8Mode;
    PQArgs.bRet = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_DEMO_CLONE_WINDOW, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bRet;
    }
}

void MDrv_PQ_EnableScalerGamma_U2(void* pInstance, PQ_WIN eWindow, MS_BOOL bEnable)
{
#if (PQ_SCALER_GAMMA_ENABLE == 1)
    PQ_IP_Gamma_Group_Main ePQIPGamma;
    ePQIPGamma = (bEnable == TRUE)?PQ_IP_Gamma_ON_Main:PQ_IP_Gamma_OFF_Main;
    printf("%s :enable gamma: %d \n",__FUNCTION__,ePQIPGamma);
    MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, ePQIPGamma, PQ_IP_Gamma_Main);
#endif
}
void MDrv_PQ_EnableScalerGamma(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
    }

    stPQ_EnableScalerGamma PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_ENABLE_SCALER_GAMMA, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
    }
}
void MDrv_PQ_SetGammaTbl_U2(void* pInstance, APIPNL_GAMMA_TYPE enGammType, MS_U8 **pu8GammTable, APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode)
{
     g_IPanel.SetGammaTbl(enGammType, pu8GammTable, enGammaMappingMode);
}
void MDrv_PQ_SetGammaTbl(APIPNL_GAMMA_TYPE enGammType, MS_U8 **pu8GammTable, APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
    }

    stPQ_SetGammaTbl PQArgs;
    PQArgs.pu8GammTable = pu8GammTable;
    PQArgs.enGammaMappingMode = enGammaMappingMode;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_GAMMA_TBl, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
    }
}

MS_BOOL MDrv_PQ_MWE_SetOffQuality_U2(void* pInstance)
{
#if PQ_VIP_CTL
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_OFF_Main, PQ_IP_VIP_ICC_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_IBC_OFF_Main, PQ_IP_VIP_IBC_Main);
    MDrv_PQ_LoadTable_U2(pInstance,PQ_MAIN_WINDOW, PQ_IP_VIP_IHC_OFF_Main, PQ_IP_VIP_IHC_Main);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_MWE_SetOffQuality(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_MWE_SetOffQuality PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MWE_SET_OFF_QUALITY, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_MWE_CloneWindow_U2(void* pInstance,PQ_WIN eWindow)
{
#if (PQ_QM_MWE_CLONE_VER1 == 1)
    MDrv_PQ_CloneTable(eWindow, PQ_IP_MWE_Clone_main_no_comm_OFF_Main, PQ_IP_MWE_Clone_main_no_comm_Main, PQ_IP_MWE_Clone_sub_no_comm_Main);
    MApi_XC_ACE_SetMWELoadFromPQ(TRUE);
    MDrv_PQ_LoadTable_U2(pInstance,eWindow, PQ_IP_MWE_diff_no_comm_ON_Main, PQ_IP_MWE_diff_no_comm_Main);
#elif (PQ_QM_MWE_CLONE_VER2 == 1)
    MDrv_PQ_CloneTable(eWindow, PQ_IP_Clone_main_no_comm_OFF_Main, PQ_IP_Clone_main_no_comm_Main, PQ_IP_Clone_sub_no_comm_Main);
    MApi_XC_ACE_SetMWELoadFromPQ(TRUE);
    MDrv_PQ_LoadTable_U2(pInstance,eWindow, PQ_IP_MWE_diff_no_comm_ON_Main, PQ_IP_MWE_diff_no_comm_Main);
#else
    UNUSED(eWindow);
#endif
}
void MDrv_PQ_MWE_CloneWindow(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_MWE_CloneWindow PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_MWE_CLONE_WINDOW, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_3D_SettingForLBL_U2(void* pInstance,PQ_WIN eWindow)
{
#if PQ_QM_3D_CLONE_ENABLE
    MDrv_PQ_LoadTable_U2(pInstance,eWindow, PQ_IP_3DSettingForLBL_no_comm_ON_Main, PQ_IP_3DSettingForLBL_no_comm_Main);
    return TRUE;
#else
    UNUSED(eWindow);
    return FALSE;
#endif
}
MS_BOOL MDrv_PQ_3D_SettingForLBL(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_3D_SettingForLBL PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_3D_SETTING_FOR_LBL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_3D_CloneWindow_U2(void* pInstance,PQ_3DTYPE enPQ3DType, PQ_WIN eWindow)
{
#if PQ_QM_3D_CLONE_ENABLE
    if(E_PQ_3DTYPE_XC == enPQ3DType)
    {
        MDrv_PQ_CloneTable(eWindow, PQ_IP_Clone_main_no_comm_OFF_Main, PQ_IP_Clone_main_no_comm_Main, PQ_IP_Clone_sub_no_comm_Main);
        if(((MApi_XC_Get_3D_HW_Version() >= 2) && (E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode()))
             || ((MApi_XC_Get_3D_HW_Version() < 2) && (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
                  && MApi_XC_3D_Is_LR_Sbs2Line()))
        {
            MDrv_PQ_3D_SettingForLBL(eWindow);
        }
    }
    else
    {
        printf("[%s %d]Attention: not support clone this PQ 3D type:%u from qmap!\n", __FILE__, (int)__LINE__, enPQ3DType);
    }
#else
    UNUSED(enPQ3DType);
    UNUSED(eWindow);
#endif
}
void MDrv_PQ_3D_CloneWindow(PQ_3DTYPE enPQ3DType, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_3D_CloneWindow PQArgs;
    PQArgs.enPQ3DType = enPQ3DType;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_3D_CLONE_WINDOW, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_Get_SensioMode_U2(void* pInstance)
{
    return _bSensioMode;
}
MS_BOOL MDrv_PQ_Get_SensioMode(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Get_SensioMode PQArgs;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_SENSIOMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void  MDrv_PQ_Set_SensioMode_U2(void* pInstance,MS_BOOL  bEnable)
{
     _bSensioMode= bEnable;
}
void  MDrv_PQ_Set_SensioMode(MS_BOOL  bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_SensioMode PQArgs;
    PQArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_SENSIOMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifndef MirrorYpbpr1080iNoiseReduce
#define MirrorYpbpr1080iNoiseReduce 0
#endif

void  MDrv_PQ_Set_DIPF_U2(void* pInstance)
{
#if MirrorYpbpr1080iNoiseReduce
      if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
     {
            //Mirror
            MApi_XC_W2BYTEMSK(REG_SC_BK22_20_L, (MS_U16)0x0800, 0x0800 );
      }
      else
      {
           //Non_Mirror
            MApi_XC_W2BYTEMSK(REG_SC_BK22_20_L, (MS_U16)0x0000, 0x0800 );
      }
#endif
}
void  MDrv_PQ_Set_DIPF(void)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DIPF, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_PQ_SetSelectCSC_U2(void* pInstance,MS_U16 u16selection, PQ_WIN eWindow)
{
    HAL_PQ_set_SelectCSC(pInstance,u16selection, (eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE));
}
void MDrv_PQ_SetSelectCSC(MS_U16 u16selection, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetSelectCSC PQArgs;
    PQArgs.u16selection = u16selection;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_SELECT_CSC, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_PQ_GetR2YEqSelect_U2(void* pInstance,PQ_WIN eWindow)
{
#if (Y2Y_709_601_VERSION == 0)
    if(PQ_MAIN_WINDOW == eWindow)
    {
        return ((MApi_XC_R2BYTEMSK(REG_SC_BK18_6F_L, BIT(2)) == 0)? FALSE : TRUE);
    }
    else
    {
        return ((MApi_XC_R2BYTEMSK(REG_SC_BK18_6F_L, BIT(6)) == 0)? FALSE : TRUE);
    }
#elif (Y2Y_709_601_VERSION == 1)

    MS_BOOL bIs709to601 = FALSE;
    if(PQ_MAIN_WINDOW == eWindow)
    {
        if ((MApi_XC_R2BYTEMSK(REG_SC_BK18_06_L, 0x0001) ==  0x0001) &&
            (MApi_XC_R2BYTEMSK(REG_SC_BK18_0A_L, 0x00F0) ==  0x00C0))
        {
            bIs709to601 = TRUE;
        }
    }
    else
    {
        if ((MApi_XC_R2BYTEMSK(REG_SC_BK18_76_L, 0x0001) ==  0x0001) &&
            (MApi_XC_R2BYTEMSK(REG_SC_BK18_7A_L, 0x00F0) ==  0x00C0))
        {
            bIs709to601 = TRUE;
        }
    }
    return bIs709to601;

#endif
}
MS_BOOL MDrv_PQ_GetR2YEqSelect(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetR2YEqSelect PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_R2YEQ_SELECT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetMemFmtInGeneral_U2(void* pInstance,
                                            PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444
                                            )
{
    if(bMemFmt422)
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MemFormat_Main);

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
                *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Main;
            else
                *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;

#if PQ_HAL_EWINDOW_PARAM
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422), PQ_MAIN_WINDOW);
#else
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422));
#endif
        }
#if PQ_ENABLE_PIP
        else
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MemFormat_Sub);

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
                *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Sub;
            else
                *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Sub;

#if PQ_HAL_EWINDOW_PARAM
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422), PQ_SUB_WINDOW);
#else
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422));
#endif
        }
#endif
    }
    else
    {
        // use 444_10BIT mode only if < 1440x1080
        // frame buffer 1920x1080x3x2 == 1440x1080x4x2
        // also bandwidth not enough for 1920x1080 444_10BIT
        if(eWindow == PQ_MAIN_WINDOW)
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MemFormat_Main);
            *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Main;
#if PQ_HAL_EWINDOW_PARAM
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422), PQ_MAIN_WINDOW);
#else
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422));
#endif
        }
#if PQ_ENABLE_PIP
        else
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MemFormat_Sub);
            *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Sub;
#if PQ_HAL_EWINDOW_PARAM
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422), PQ_SUB_WINDOW);
#else
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx(pInstance,(MS_422To444_TYPE)Hal_PQ_get_422To444_mode(pInstance,bMemFmt422));
#endif
        }
#endif
    }

    return TRUE;
}
MS_BOOL MDrv_PQ_GetMemFmtInGeneral(PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444
                                            )
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetMemFmtInGeneral PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bMemFmt422 = bMemFmt422;
    PQArgs.pu16TabIdx_MemFormat = pu16TabIdx_MemFormat;
    PQArgs.pu16TabIdx_444To422 = pu16TabIdx_444To422;
    PQArgs.pu16TabIdx_422To444 = pu16TabIdx_422To444;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_MEMFMT_IN_GENERAL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


MS_BOOL MDrv_PQ_GetMADiForRFBL_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bFBL, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi)
{
    UNUSED(pu16TabIdx_MADi_Motion);
    if((_stRFBL_Info.bEnable)&&(!bFBL))
    {
        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, (MS_MADI_TYPE)_stRFBL_Info.u8MADiType);
        if(!_stMode_Info[eWindow].bInterlace)
        {
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_NO_MIU);

            if(eWindow == PQ_MAIN_WINDOW)
            {
                MDrv_PQ_MADiForceMotionY_U2(pInstance,PQ_MAIN_WINDOW, FALSE);
                MDrv_PQ_MADiForceMotionC_U2(pInstance,PQ_MAIN_WINDOW, FALSE);
            }
            else
            {
                MDrv_PQ_MADiForceMotionY_U2(pInstance,PQ_SUB_WINDOW, FALSE);
                MDrv_PQ_MADiForceMotionC_U2(pInstance,PQ_SUB_WINDOW, FALSE);
            }
            //printf("[%s]-[%d] :  disable Y/C motion\n", __FUNCTION__, __LINE__);
        }
        //printf("[%s]-[%d] : u16TabIdx_MADi = %d\n",__FUNCTION__,__LINE__,u16TabIdx_MADi);
    }

    return TRUE;
}
MS_BOOL MDrv_PQ_GetMADiForRFBL(PQ_WIN eWindow, MS_BOOL bFBL, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetMADiForRFBL PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bFBL = bFBL;
    PQArgs.pu16TabIdx_MADi_Motion = pu16TabIdx_MADi_Motion;
    PQArgs.pu16TabIdx_MADi = pu16TabIdx_MADi;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_MADI_FOR_RFBL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx_MADi)
{
#if PQ_GRULE_OSD_BW_ENABLE
    // Patch: in OSD turn off case, when 2R-->4R, J2, M10 with new OPM system will flash(4R-->2R is OK)
    // we need to set 4R first, and wait some time, and do other MAdi setting, the flash will be ok.
    // which is only for non-OS platform patch, since OS platform won't turn off OSD for DFB system.
    // Only FieldPackingMode need this solution
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if((MApi_XC_IsFieldPackingModeSupported() &&
            (_stMode_Info[eWindow].u16input_hsize>=1280)&&
            (_stMode_Info[eWindow].u16input_vsize>=720) &&
            (!_bOSD_On)) &&
            (  (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_24_4R))
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_25_4R))
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_26_4R))
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_27_4R))
#if PQ_MADI_88X_MODE
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_24_4R_880))
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_25_4R_880))
            || (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,TRUE, MS_MADI_25_4R_884))
#endif
        ))
        {
            //set to 4R first,
            MApi_XC_W2BYTEMSK(REG_SC_BK12_03_L, 0 , 0x0002 );
            //delay
            MApi_XC_WaitOutputVSync(1, 90, MAIN_WINDOW);
        }
    }
#else
    UNUSED(eWindow);
    UNUSED(u16TabIdx_MADi);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_Patch2Rto4RForFieldPackingMode(PQ_WIN eWindow, MS_U16 u16TabIdx_MADi)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Patch2Rto4RForFieldPackingMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx_MADi = u16TabIdx_MADi;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_PATCH2RTO4R_FOR_FIELDPACKINGMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

static void _MDrv_PQ_EnableRequest_FrameBufferLess(MS_BOOL bEnable, PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
        MApi_XC_EnableRequest_FrameBufferLess(bEnable);
    }
    else
    {
        PQMADi_DBG(printf("%s  %d,  sub have no FLB mode!\n",__FUNCTION__,__LINE__));
    }
}

void MDrv_PQ_SetFrameBuffMode(MS_BOOL bFrameBuffLess)
{
    stPQ_SetFrameBuffMode PQArgs;

    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    PQArgs.bFrameBuffLess = bFrameBuffLess;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_FRAMEBUFF_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
void MDrv_PQ_SetFrameBuffMode_U2(void* pInstance,MS_BOOL bFrameBuffLess)
{
    _bFrameBuffLess = bFrameBuffLess;
}
MS_BOOL MDrv_PQ_GetBPPInfoFromMADi_U2(void* pInstance,
                                              PQ_WIN eWindow,
                                              MS_BOOL bMemFmt422,
                                              MS_U16 u16TabIdx_MADi,
                                              MS_U16 u16TabIdx_MemFormat,
                                              MS_U8 *pu8BitsPerPixel,
                                              PQ_DEINTERLACE_MODE *peDeInterlaceMode,
                                              MS_U8 *pu8FrameCount
                                              )
{
    _u16PQDelyaTime= 0;

    MApi_XC_EnableRequest_FrameBufferLess(_bFrameBuffLess);
    PQMADi_DBG(printf("[PQ] MADI: DISABLE RFBL\n"));

    if(bMemFmt422)
    {
        // decide bits/pixel and deinterlace mode flag
        if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("24_4R PQ_DEINT_3DDI_HISTORY \n"));
        }
        else if( (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R)) ||
            (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R_880)))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("24_2R PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 4R PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_4R_720i_Main))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 4R 720i PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 4R MC_PQ_DEINT_3DDI_HISTORY\n"));

            if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
            {
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Error: Cannot set MS_MADI_25_4R_MC(4 fields mode) for Mirror(at least 6 fields). Fix your PQ excel!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_4R_M_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 4R M MC_PQ_DEINT_3DDI_HISTORY\n"));
            MirrorMode_t enVideoMirror = MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW);
            if((enVideoMirror == MIRROR_HV) || (enVideoMirror == MIRROR_V_ONLY))
            {
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Error: Cannot set MS_MADi_25_4R_M_MC(4 fields mode) for Mirror(at least 6 fields). Fix your PQ excel!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_6F_4R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 3;
            *pu8FrameCount = 6;
            PQMADi_DBG(printf("25 6F 4R MC_PQ_DEINT_3DDI_HISTORY\n"));

        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC))
        {
            *pu8BitsPerPixel = 24;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_6R_MC))  //Add New MADi mode
        {
            *pu8BitsPerPixel = 24;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 14;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 14F 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 14;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 14F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_12F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 12;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 12F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_16F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_16F_6R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_16F_8R_M_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 8R M MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC_NW))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 8;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_FilmPreDet))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 6R FilmPreDet_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_FilmPreDet_PAL))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 6R FilmPreDet PAL_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_6R_FilmPreDet))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("24 6R FilmPreDet_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_6R_FilmPreDet_PAL))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("24 6R FilmPreDet PAL_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_8F_4R_MC))
        {
            *pu8BitsPerPixel = 24;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 8F 4R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_8F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 8F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
#if PQ_ENABLE_RFBL
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_2D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_2D\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_25D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_25D\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_3D\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE8_MOT)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _MDrv_PQ_EnableRequest_FrameBufferLess(ENABLE, eWindow);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_P_MODE8_MOT\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE10_MOT)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _MDrv_PQ_EnableRequest_FrameBufferLess(ENABLE, eWindow);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_P_MODE10_MOT\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_YC))
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_3D_YC\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_FILM))
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_3D_FILM\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_MC))
        {
            *pu8BitsPerPixel = 24;
            u8PQDelayCount = 6;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_MC\n"));
        }
#endif
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 2 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_26_4R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("26 4 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_26_2R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("26 2 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_4R))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_3DDI;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("27 4 PQ_DEINT_3DDI\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_2R))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_3DDI;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("27 2 PQ_DEINT_3DDI\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 8 PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
#if (PQ_P_MODE8_3FRAME_MODE_ENABLE == 1)
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE8_3Frame))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("P 8 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 3;
        }
#endif
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 10 PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT8))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P M 8 PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }
            else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P M 10 PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
#if PQ_ENABLE_RFBL
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_RFBL_FILM))
        {
            *pu8BitsPerPixel = 12;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            PQMADi_DBG(printf("24 RFBL FILM PQ_DEINT_3DDI_HISTORY\n"));
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_24_RFBL_FILM\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_RFBL_NFILM))
        {
            *pu8BitsPerPixel = 12;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            PQMADi_DBG(printf("24 RFBL NFILM PQ_DEINT_3DDI_HISTORY\n"));
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_24_RFBL_NFILM\n"));
        }
#endif
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_FBL_DNR))
        {
            *pu8BitsPerPixel = 16;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQMADi_DBG(printf("FBL DNR PQ_DEINT_2DDI_AVG\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_FBL_MIU))
        {
            *pu8BitsPerPixel = 16;
            u8PQDelayCount = 3;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQMADi_DBG(printf("FBL MIU PQ_DEINT_2DDI_AVG\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_NO_MIU))
        {
            *pu8BitsPerPixel = 16;
            u8PQDelayCount = 2;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            PQMADi_DBG(printf("P 8 NO MIU PQ_DEINT_2DDI_AVG\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10_8Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 8Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 8;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10_4Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 4Frame PQ_DEINT_2DDI_BOB\n"));
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_3Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC 3Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 3;
            if((MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL) &&
               (QM_GetInputVFreq(eWindow) > 235) && (QM_GetInputVFreq(eWindow) < 245))
            {
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Error: Cannot set MS_MADI_P_MODE_MC_3Frame for mirror 24p(at least 4 frame number). Fix your PQ excel!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_4Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC 4Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MC_4Frame_Yonly_Main))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC 4Frame Yonly Main PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_5Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC 5Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 5;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_6Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC 6Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 6;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_6Frame_6R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 6Frame 6R PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 6;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_8Frame_6R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 8Frame 6R PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 8;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_12Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 12Frame 8R PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 12;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MC_12Frame_8R_M))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 12Frame 8R M PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 12;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MC_8Frame_8R_M))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 8Frame 8R M PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 8;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_16Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P MC 16Frame 8R PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 16;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_N_MC_12Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P M N MC 12 8Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 12;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_P_MC_12Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            PQMADi_DBG(printf("P M P MC 12 8Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 12;
        }
#if PQ_ENABLE_RFBL
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE10_MOT))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("RFBL P M 10 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_P_MODE10_MOT\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE8_MOT))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("RFBL P M 8 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_P_MODE8_MOT\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 6;
            PQMADi_DBG(printf("RFBL P M MC PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
            MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            PQMADi_DBG(printf("[PQ] MADI: ENABLE RFBL: MS_MADI_RFBL_P_MODE_MC\n"));
        }
#endif
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6F_4R_MC_I))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 6;
            PQMADi_DBG(printf("I 25 4R MC_PQ_DEINT_3DDI_HISTORY\n"));
//#if (STB_ENABLE == 0)
//            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
//            *pu8FrameCount = 8;
//#else
            *pu8FrameCount = 6;
//#endif
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6F_4R_MC_P))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 6;
            PQMADi_DBG(printf("P 25 4R MC_PQ_DEINT_3DDI_HISTORY\n"));
//#if (STB_ENABLE == 0)
//            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
//            *pu8FrameCount = 8;
//#else
            *pu8FrameCount = 6;
//#endif
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_4R_I))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("I 25 4R PQ_DEINT_3DDI_HISTORY\n"));
            *pu8FrameCount = 6;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_4R_P))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("P 25 4R PQ_DEINT_3DDI_HISTORY\n"));
            *pu8FrameCount = 6;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_16F_6R_M_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 6R M MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MC_M_4F))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            u8PQDelayCount = 6; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P MC M 4Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE8_M))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            printf("dirk  24_2R MS_MADi_P_MODE8_M\n");
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE10_M))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 10 M PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }
            else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_25_4R_M))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            PQMADi_DBG(printf("25 4R M PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MOT10_M))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P Mot 10 M PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MOT10_M_4K))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P Mot 10 M 4K PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }
            else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MOT8_M))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P M 8 PQ_DEINT_2DDI_BOB\n"));

            if ( _upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }
            else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_24_8F_4R_M_MC))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            u8PQDelayCount = 3;
            *pu8FrameCount = 4;
            printf("24 8F 4R M MC PQ_DEINT_3DDI_HISTORY 4frame\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance, (eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MOT10_NR_Yonly_M))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P Mot 10 M PQ_DEINT_2DDI_BOB MS_MADi_P_MODE_MOT10_NR_Yonly_M\n"));

            if(_upLayerConfig & E_PQ_MISC_A_HDMITX_ENABLE )
            {
                // HDMITX uses freerun
                // and when in some special case like 29.97fps in, 30fps out
                // sometimes we can see tearing because it is not locked yet.
                // so at least 3 frames is needed.
                *pu8FrameCount = 3;
            }
            else
            {
                if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
                {
                    *pu8FrameCount = 3;
                }
                else
                {
                    *pu8FrameCount = 2;
                }
            }
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE_MOT10_NO_DNR))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P Mot 10 M PQ_DEINT_2DDI_BOB MS_MADi_P_MODE_MOT10_NO_DNR\n"));

            if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                *pu8FrameCount = 3;
            }
            else
            {
                *pu8FrameCount = 2;
            }

        }
        else
        {
            MS_ASSERT(0);
            PQMADi_DBG(printf("None\n"));
        }

    }
    else
    {
        //in order to disable user mode,
        if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 8 444 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE8_M_444))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P MODE8 M 444 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10_444))
        {
            *pu8BitsPerPixel = 30;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 10 444 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE10_M_444))
        {
            *pu8BitsPerPixel = 30;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 10 M 444 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADi_P_MODE10_M_444_4K))
        {
            *pu8BitsPerPixel = 30;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            u8PQDelayCount = 2;
            PQMADi_DBG(printf("P 10 M 444 4K PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 2;
        }
        if((u16TabIdx_MemFormat == PQ_IP_MemFormat_444_10BIT_Main)
#if PQ_ENABLE_PIP
            ||(u16TabIdx_MemFormat == PQ_IP_MemFormat_444_10BIT_Sub)
#endif
            )
        {
            *pu8BitsPerPixel = 32;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("PQ_IP_MemFormat_444_10BIT\n"));

            if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                *pu8FrameCount = 3;
                u8PQDelayCount = 3;

            }
            else
            {
                *pu8FrameCount = 2;
                u8PQDelayCount = 2;
            }
        }
        else if((u16TabIdx_MemFormat == PQ_IP_MemFormat_444_8BIT_Main)
#if PQ_ENABLE_PIP
            ||(u16TabIdx_MemFormat == PQ_IP_MemFormat_444_8BIT_Sub)
#endif
            )
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("PQ_IP_MemFormat_444_8BIT\n"));

            if(MApi_XC_GetIPAutoNoSignal((SCALER_WIN)eWindow)==ENABLE)
            {
                *pu8FrameCount = 3;
                u8PQDelayCount = 3;
            }
            else
            {
                *pu8FrameCount = 2;
                u8PQDelayCount = 2;
            }
        }
        else
        {
            *pu8BitsPerPixel = 24;
            PQMADi_DBG(printf("PQ_IP_MemFormat_422MF_Main\n"));
            *pu8FrameCount = 2;
        }

#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
        if(MApi_XC_GetUCEnabled((SCALER_WIN)eWindow))   //if on need disable.
        {
            MDrv_PQ_DisableUCFeature(PQ_MAIN_WINDOW);
        }
#endif
    }

    if ((QM_GetInputVFreq(eWindow) != 0) && (QM_GetInputVFreq(eWindow) != 1))
        _u16PQDelyaTime = u8PQDelayCount * ((10000 + (QM_GetInputVFreq(eWindow)/2))/QM_GetInputVFreq(eWindow));  // frame duration is (1 second/input V freq)
    else
        _u16PQDelyaTime = u8PQDelayCount * 20; // default keep original rule, use 20ms as frame duration

    MApi_XC_EnableRequest_FrameBufferLess(_bFrameBuffLess);

    return TRUE;
}
MS_BOOL MDrv_PQ_GetBPPInfoFromMADi(PQ_WIN eWindow,
                                              MS_BOOL bMemFmt422,
                                              MS_U16 u16TabIdx_MADi,
                                              MS_U16 u16TabIdx_MemFormat,
                                              MS_U8 *pu8BitsPerPixel,
                                              PQ_DEINTERLACE_MODE *peDeInterlaceMode,
                                              MS_U8 *pu8FrameCount
                                              )
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetBPPInfoFromMADi PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bMemFmt422 = bMemFmt422;
    PQArgs.u16TabIdx_MADi = u16TabIdx_MADi;
    PQArgs.u16TabIdx_MemFormat = u16TabIdx_MemFormat;
    PQArgs.pu8BitsPerPixel = pu8BitsPerPixel;
    PQArgs.peDeInterlaceMode = peDeInterlaceMode;
    PQArgs.pu8FrameCount = pu8FrameCount;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_BPPINFO_FROM_MADI, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_ChangeMemConfigFor3D_U2(void* pInstance,
                                            PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444,
                                            MS_U8 *pu8BitsPerPixel,
                                            MS_U16 *pu16TabIdx_MADi_Motion,
                                            MS_U16 *pu16TabIdx_MADi
                                            )
{
    UNUSED(pu16TabIdx_MADi_Motion);
    if(eWindow == PQ_MAIN_WINDOW)
    {
        //reduce mem size, put it into 16M, 2 frame mode.
        if((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING)
            && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            && (_stMode_Info[eWindow].u16input_hsize == 1920)
            && (_stMode_Info[eWindow].u16input_vsize == 2205))
        {
#if PQ_EN_UCNR_3D_MADI
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_2BYTE_3D);
#else
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444);
#endif
            *pu8BitsPerPixel = 16;

            // DVI is 444 input, we need to conver it to 422, otherwise DVI will use 32 bpp instead, not 16
            // because 444 format combine to the setting of pu16TabIdx_MemFormat
            if(!bMemFmt422)
            {
                //PQ_IP_MemFormat_Main 422
                *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                //PQ_IP_422To444_Main on
                *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
            }

            printf("3D: modify progressive memory fmt for 1080p framepacking\n");
        }

        if(MApi_XC_Get_3D_HW_Version() < 2)
        {
            //PQ main page maybe MADi mode to 8 field mode, but it cannot do madi for line by line output.
            //so we force it back to normal 25_2R like mode.
            if(((MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM) && (TRUE == _stMode_Info[eWindow].bInterlace))
               || ((MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM_HW) && (TRUE == _stMode_Info[eWindow].bInterlace)))
            {
                // we must use MS_MADI_25_4R or 2R to avoid MC madi mode for dnr offset calcuation problem.
                if(TRUE == _stMode_Info[eWindow].bInterlace)
                {
                    if((_stMode_Info[eWindow].u16input_vsize >= 900) &&
                        (_stMode_Info[eWindow].u16input_hsize >= 1440) &&
                        (bw_info.u32Miu1MemSize==0))// 1ddr case, should save bw
                    {
                        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R);
                        PQBW_DBG(printf("3D: force to 25_2R\n");)
                    }
                    else
                    {
                        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R);
                        PQBW_DBG(printf("3D: force to 25_4R\n");)
                    }
                    if(eWindow == PQ_MAIN_WINDOW)
                    {
                        MApi_XC_Set_OPWriteOffEnable(ENABLE, MAIN_WINDOW); //default
                    }
                    else
                    {
                        PQINFO_DBG(printf("No need to enable OP write with sub window, it would change main window setting \n"));
                    }
                }

                //Madi do line by line must use 422 format
                if(!bMemFmt422)
                {
                    //PQ_IP_MemFormat_Main 422
                    *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                    *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                    //PQ_IP_422To444_Main on
                    *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                    PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
                }

                // we can only process 422 format for E_XC_3D_INPUT_TOP_BOTTOM.
                // so, if it's 444 format, we have to force it to be 422 and the u8BitsPerPixel is also changed to 24.
                *pu8BitsPerPixel = 24;
                PQBW_DBG(printf("3D:force u8BitsPerPixel to 24\n");)
            }

            //frame alterntive must use bob mode
            if((E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC == MApi_XC_Get_3D_Output_Mode()) ||
               (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode()))
            {
                MDrv_PQ_EnableMADIForce(PQ_MAIN_WINDOW, TRUE);
#if PQ_MADI_DFK
                MDrv_PQ_LoadTable_U2(pInstance,eWindow, PQ_IP_MADi_DFK_OFF_Main, PQ_IP_MADi_DFK_Main);
#endif
                MDrv_PQ_LoadTable_U2(pInstance,eWindow, PQ_IP_EODi_OFF_Main, PQ_IP_EODi_Main);
            }
            //line by line must use weave mode
            else if(E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
            {
                MDrv_PQ_EnableMADIForce(eWindow, FALSE);
            }

#if 0
            //None 3D case: by Ethan.Lee, saveing memory size for 1920x1200
            if ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_MODE_NONE) &&
                (MApi_XC_Get_3D_Output_Mode()== E_XC_3D_OUTPUT_MODE_NONE) )
            {
                // Saving memory size for 1920x1200P HDMI 20110421EL
                if (  (_stMode_Info[eWindow].u16input_vsize > 1100) && (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow], eWindow)))
                {
                    *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444);
                    *pu8BitsPerPixel = 16;
                    if(!bMemFmt422)
                    {
                        //PQ_IP_MemFormat_Main 422
                        *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                        *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                        //PQ_IP_422To444_Main on
                        *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                        PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
                    }
                }
            }
#endif
        }

        //Frame packing interlace must use 25_2R for
        // A. Madi for line by line output for old 3D engine. Or,
        // B. better quality. (P mode will shaking a little)
        if(((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_L) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_R) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            )
        {
            //framepacking interlace 1080i
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R);
            _stMode_Info[eWindow].bInterlace = TRUE;
            *pu8BitsPerPixel = 24;

            PQBW_DBG(printf("3D: force to 25_2R\n");)
        }
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_ChangeMemConfigFor3D(PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444,
                                            MS_U8 *pu8BitsPerPixel,
                                            MS_U16 *pu16TabIdx_MADi_Motion,
                                            MS_U16 *pu16TabIdx_MADi
                                            )
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_ChangeMemConfigFor3D PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bMemFmt422 = bMemFmt422;
    PQArgs.pu16TabIdx_MemFormat = pu16TabIdx_MemFormat;
    PQArgs.pu16TabIdx_444To422 = pu16TabIdx_444To422;
    PQArgs.pu16TabIdx_422To444 = pu16TabIdx_422To444;
    PQArgs.pu8BitsPerPixel = pu8BitsPerPixel;
    PQArgs.pu16TabIdx_MADi_Motion = pu16TabIdx_MADi_Motion;
    PQArgs.pu16TabIdx_MADi = pu16TabIdx_MADi;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CHANGE_MEMCONFIG_FOR3D, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_ForceBPPForDynamicMemFmt_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_U8 *pu8BitsPerPixel)
{
    //force Memory FMT and use the same pixel number to avoid the garbage when memory mode is changed.
    if(bMemFmt422)
    {
        MS_U16 u16MemFMT= 0;

        if(!_stMode_Info[eWindow].bInterlace)
        {
            // for dynamically memory format changing, we need to keep it as 24 bits
            if(SUPPORT_DYNAMIC_MEM_FMT)//STB could be false
            {
                //Please refer to "MDrv_PQ_ChangeMemConfigFor3D"
                //reduce mem size, put it into 16M, 2 frame mode.
                if(!((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING)
                    && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
                    && (_stMode_Info[eWindow].u16input_hsize == 1920)
                    && (_stMode_Info[eWindow].u16input_vsize == 2205)))
                *pu8BitsPerPixel = 24;
            }
        }

        if (eWindow == PQ_MAIN_WINDOW)
        {
            u16MemFMT = MApi_XC_R2BYTEMSK(REG_SC_BK12_02_L, 0xFF);
        }
        else
        {
            u16MemFMT = MApi_XC_R2BYTEMSK(REG_SC_BK12_42_L, 0xFF);
        }
        if((u16MemFMT == 0xBB) || (u16MemFMT == 0xAA) || (u16MemFMT == 0x99) || (u16MemFMT == 0x88))
        {
                *pu8BitsPerPixel = 24;
        }
        else if((u16MemFMT == 0x55) || (u16MemFMT == 0x66) || (u16MemFMT == 0x44))
        {
                *pu8BitsPerPixel = 20;
        }
        else
        {
            //decide by original setting
        }
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_ForceBPPForDynamicMemFmt(PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_U8 *pu8BitsPerPixel)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_ForceBPPForDynamicMemFmt PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bMemFmt422 = bMemFmt422;
    PQArgs.pu8BitsPerPixel = pu8BitsPerPixel;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_FORCE_BPP_FOR_DYNAMIC_MEMFMT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SuggestFrameNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx_MADi, MS_U8 u8FrameCount)
{
    //the chips, who are older than SCMI_V2, set frame count reg by Qmap.
    //the chips, who are SCMI_V2, the frame count is suggested by Qmap, but sw code judge and replace the value.
    //      and then, set to hw reg
    //note: SUPPORT_SCMI_V2 is corresponding withe utopia flag: _FIELD_PACKING_MODE_SUPPORTED
    //      we can remove SUPPORT_SCMI_V2 later
    if(MApi_XC_IsFieldPackingModeSupported())
    {
        //Set Frame number
        UNUSED(u16TabIdx_MADi);
        if(bSetFrameCount )
        {
            _u16PQSuggestedFrameNum[eWindow] = u8FrameCount;
            PQMADi_DBG(printf("[%s %d] _u16PQSuggestedFrameNum[%u] = %u\n", __FILE__, (int)__LINE__, eWindow, u8FrameCount));
        }
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_SuggestFrameNum(PQ_WIN eWindow, MS_U16 u16TabIdx_MADi, MS_U8 u8FrameCount)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SuggestFrameNum PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16TabIdx_MADi = u16TabIdx_MADi;
    PQArgs.u8FrameCount = u8FrameCount;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SUGGEST_FRAMENUM, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetGameMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
#if PQ_UC_CTL
#if(PQ_GRULE_GAME_MODE_ENABLE == 0)
    static MS_U16 _u16TabIdx_MADi_DFK_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_MADi_SST_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_MADi_EODiW_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_UCNR_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_UCDi_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_UCCTL_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_MADi_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_MADi_Mot_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_SwDriver_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_PostCCS_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
    static MS_U16 _u16TabIdx_DHD_Gamemode[PQ_MAX_WINDOW] = {0xFFFF, 0xFFFF};
#endif
#endif
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;

    #if(ENABLE_PQ_MLOAD == 0)
        printf("*** [Game mode] please enable PQ Mload \n");
    #endif

    PQMADi_DBG(printf("MDrv_PQ_SetGameMode() bEnable: %u\n", bEnable));

#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
    MS_U8  u8RWmappingNum = 0;
#endif
    _bGameModeOn[eWindow] = bEnable;

    if(bEnable)
    {
#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
        if(_bRWNumIsReset[eWindow])
        {
            _u8RWmappingNum[eWindow] = MApi_XC_GetWRBankMappingNum(eWindow);
            _bRWNumIsReset[eWindow] = FALSE;
        }

        // set the R/W bank mapping
        if (_stMode_Info[eWindow].bInterlace
        && (_stMode_Info[eWindow].u16input_vtotal <= 750)
        && (QM_IsSourceYPbPr(_enInputSourceType[eWindow]) || QM_IsSourceSV(_enInputSourceType[eWindow]) || QM_IsSourceAV(_enInputSourceType[eWindow])))
        {
            u8RWmappingNum = 0x01;
        }
        else
        {
            u8RWmappingNum = _u8RWmappingNum[eWindow];
        }
#endif

#if PQ_UC_CTL
#if(PQ_GRULE_GAME_MODE_ENABLE == 0)
        MDrv_PQ_GetCurrentUCFeature( eWindow,
                                    &_u16TabIdx_MADi_Gamemode[eWindow],
                                    &_u16TabIdx_MADi_Mot_Gamemode[eWindow],
                                    &_u16TabIdx_MADi_DFK_Gamemode[eWindow],
                                    &_u16TabIdx_MADi_SST_Gamemode[eWindow],
                                    &_u16TabIdx_MADi_EODiW_Gamemode[eWindow],
                                    &_u16TabIdx_UCNR_Gamemode[eWindow],
                                    &_u16TabIdx_UCDi_Gamemode[eWindow],
                                    &_u16TabIdx_UCCTL_Gamemode[eWindow],
                                    &_u16TabIdx_SwDriver_Gamemode[eWindow],
                                    &_u16TabIdx_PostCCS_Gamemode[eWindow],
                                    &_u16TabIdx_DHD_Gamemode[eWindow]
                                    );
 #endif
        MDrv_PQ_DisableUCFeature(eWindow);
#endif
    }
    else
    {
#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
        // Restore original value
        u8RWmappingNum = _u8RWmappingNum[eWindow];
#endif

#if PQ_UC_CTL
#if(PQ_GRULE_GAME_MODE_ENABLE == 1)
        MDrv_PQ_EnableUCFeature_U2(pInstance, eWindow);
#else
        MDrv_PQ_SetUCFeature(eWindow,
                             _u16TabIdx_MADi_Gamemode[eWindow],
                             _u16TabIdx_MADi_Mot_Gamemode[eWindow],
                             _u16TabIdx_MADi_DFK_Gamemode[eWindow],
                             _u16TabIdx_MADi_SST_Gamemode[eWindow],
                             _u16TabIdx_MADi_EODiW_Gamemode[eWindow],
                             _u16TabIdx_UCNR_Gamemode[eWindow],
                             _u16TabIdx_UCDi_Gamemode[eWindow],
                             _u16TabIdx_UCCTL_Gamemode[eWindow],
                             _u16TabIdx_SwDriver_Gamemode[eWindow],
                             _u16TabIdx_PostCCS_Gamemode[eWindow],
                             _u16TabIdx_DHD_Gamemode[eWindow]
                             );
#endif
#endif
    }

#if (PQ_GRULE_GAME_MODE_ENABLE == 0)
    MApi_XC_SetWRBankMappingNum(u8RWmappingNum,eWindow);
#endif
    return TRUE;
}
MS_BOOL MDrv_PQ_SetGameMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetGameMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_GAMEMODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}


MS_BOOL MDrv_PQ_GetGameMode_Status_U2(void* pInstance,PQ_WIN eWindow)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;
    else
        return _bGameModeOn[eWindow];
}
MS_BOOL MDrv_PQ_GetGameMode_Status(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGameMode_Status PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GAMEMODE_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetBypassMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;

    _bBypassModeOn[eWindow] = bEnable;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetBypassMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetBypassMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_BYPASS_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetBypassModeStatus_U2(void* pInstance,PQ_WIN eWindow)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;
    else
        return _bBypassModeOn[eWindow];
}
MS_BOOL MDrv_PQ_GetBypassModeStatus(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetBypassModeStatus PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_BYPASS_MODE_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

char*  MDrv_PQ_GetVersion_U2(void* pInstance,PQ_WIN eWindow)
{
    if(gbPQBinEnable == 1)
    {
        char* PQ_Version;

        if(PQ_MAIN_WINDOW == eWindow)
        {
            PQ_Version = stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u8Version;
        }
        else
        {
            PQ_Version = stPQBinHeaderInfo[PQ_BIN_STD_SUB].u8Version;
        }
        return PQ_Version;
    }
    else
    {
        return NULL;

    }
}
char*  MDrv_PQ_GetVersion(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetVersion PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_VERSION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}
char* MDrv_PQ_Cus_GetVersion_U2(void *pInstance,PQ_WIN eWindow)
{
    if(_bCustomerMainPQEn == 1)
    {
        char* Cus_PQ_Version = NULL;

        if(PQ_MAIN_WINDOW == eWindow)
        {
            Cus_PQ_Version = stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u8Version;
        }
        else
        {
            if(_bCustomerSubPQEn == 1)
            {
                Cus_PQ_Version = stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u8Version;
            }
        }
        return Cus_PQ_Version;
    }
    else
    {
        return NULL;
    }
}
char* MDrv_PQ_Cus_GetVersion(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Cus_GetVersion PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CUS_GET_VERSION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}
static MS_BOOL _MDrv_PQ_SetGRuleStatus(void *pInstance,PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16PQ_GroupIdx)
{
    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif

    if(gbPQBinEnable)
    {
        MS_U16 i, u16IPIdx = 0, u16TabIdx = 0, u16GroupNum = 0, u16IPNum = 0;

        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
            {
                if(_bCustomerMainPQEn)
                {
                    printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                            __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                    u16GruleType = u16GruleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
                    enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
                    if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
                    {
                        printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                        return FALSE;
                    }
                }
                else
                {
                    printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                    return FALSE;
                }
            }
            u16GroupNum = MDrv_PQBin_GetGRule_GroupNum(pInstance,u16GruleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
            u16IPNum = MDrv_PQBin_GetGRule_IPNum(pInstance,u16GruleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            if(_bCustomerSubPQEn)
            {
                printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                        __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                u16GruleType = u16GruleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
                enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
                if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
                {
                    printf("CustomerPQ: ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                    return 0;
                }
            }
            else
            {
                printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                return FALSE;
            }
            u16GroupNum = MDrv_PQBin_GetGRule_GroupNum(pInstance,u16GruleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
            u16IPNum = MDrv_PQBin_GetGRule_IPNum(pInstance,u16GruleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
#endif

        if ((PQ_BIN_IP_NULL == u16GroupNum) || (PQ_BIN_IP_NULL == u16IPNum))
        {
            return FALSE;
        }

        if(u16PQ_GroupIdx >= u16GroupNum)
        {
            printf("=ERROR!!!=%s, %d, u16PQ_GroupIdx:%d, out of range:%d=\n", __FUNCTION__, __LINE__, u16PQ_GroupIdx, u16GroupNum);
            return FALSE;
        }

        if(((_bCustomerMainPQEn) && (enPQBinTypeMain == PQ_BIN_CUSTOMER_MAIN))
#if (PQ_ENABLE_PIP)
                || ((_bCustomerSubPQEn) && (enPQBinTypeSub == PQ_BIN_CUSTOMER_SUB))
#endif
          )
        {
            for(i = 0; i < u16IPNum; i++)
            {
                u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, u16GruleType, i);

                if(u16PQ_GroupIdx == u16GroupNum)  // see it as default
                {
                    u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16TabIdx);
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, u16GruleType, u16PQ_GroupIdx, i);
                }
                PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                                   _u16PQSrcType[eWindow], u16PQ_GroupIdx, i, u16IPIdx, u16TabIdx));
                MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
            }
        }
        else
        {
            for(i = 0; i < u16IPNum; i++)
            {
                u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, u16GruleType, i);

                if(u16PQ_GroupIdx == u16GroupNum)  // see it as default
                    u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16TabIdx);
                else
                    u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, u16GruleType, u16PQ_GroupIdx, i);

                PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                               _u16PQSrcType[eWindow], u16PQ_GroupIdx, i, u16IPIdx, u16TabIdx));
                MDrv_PQ_LoadTable_U2(pInstance,eWindow, u16TabIdx, u16IPIdx);
            }
        }
    return TRUE;
    }
    else
    {
       return FALSE;
    }
}

MS_BOOL MDrv_PQ_SetGRuleStatus_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16GruleStatus)
{
    MS_U16 u16PQ_CurLvlIdx = 0;
    u16PQ_CurLvlIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16GruleType, u16GruleStatus);

    return _MDrv_PQ_SetGRuleStatus(pInstance,eWindow, u16GruleType, u16PQ_CurLvlIdx);
}
MS_BOOL MDrv_PQ_SetGRuleStatus(PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16GruleStatus)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetGRuleStatus PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GruleType = u16GruleType;
    PQArgs.u16GruleStatus = u16GruleStatus;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_GRULE_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U16 MDrv_PQ_GetGRule_LvlNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType)
{
    MS_U16 u16Ret = 0;

    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif

    if(gbPQBinEnable)
    {
        MS_U16 u16PanelIdx = 0;
        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
            {
                if(_bCustomerMainPQEn)
                {
                    PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum));
                    u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
                    enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
                    u16PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
                    {
                        printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                        return 0;
                    }
                }
                else
                {
                    printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                    return 0;
                }
            }
            else
            {
                u16PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
            }

            u16Ret = MDrv_PQBin_GetGRule_LvlNum(pInstance,u16PanelIdx, u16GRuleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum)
            {
                if(_bCustomerSubPQEn)
                {
                    PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum));
                    u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
                    enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
                    u16PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
                    {
                        printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                        return 0;
                    }
                }
                else
                {
                    printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                    return 0;
                }
            }
            else
            {
                u16PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
            }

            u16Ret = MDrv_PQBin_GetGRule_LvlNum(pInstance,u16PanelIdx, u16GRuleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
#endif
        else
        {
            printf("=Error=%s, %d, =Not support case \n", __FUNCTION__, __LINE__);
        }

        if (PQ_BIN_IP_NULL == u16Ret)
        {
            return 0;
        }
    }
    return u16Ret;
}
MS_U16 MDrv_PQ_GetGRule_LvlNum(PQ_WIN eWindow, MS_U16 u16GRuleType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_LvlNum PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_LV1NUM, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_LvlNum_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 u16Ret = PQ_BIN_IP_NULL;
    MS_U8 u8PanelIdx = 0;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_MAIN;
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
                }
                break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
                {
                    u8PanelIdx = PQ_BIN_UFSC_PANEL_INDEX;
                    enPQBinType = PQ_BIN_UFSC;
                }
                break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_SUB;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
                }
                break;
            case PQ_XC0_UFSC:
                break;
            default:
                break;
        }

    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        if(u16GRuleType >= stPQBinHeaderInfo[enPQBinType].u16GRule_RuleNum)
        {
            printf("[PQ ERROR] %s:%d\nenPQBinType = %d, u16GruleType = %d out of %d\n", __FUNCTION__, __LINE__, enPQBinType, u16GRuleType, stPQBinHeaderInfo[enPQBinType].u16GRule_RuleNum);
        }
        else
        {
            u16Ret = MDrv_PQBin_GetGRule_LvlNum(pInstance,u8PanelIdx, u16GRuleType, &stPQBinHeaderInfo[enPQBinType]);
        }
    }

    return u16Ret;
}

MS_U16 MDrv_PQ_GetGRule_LvlNum_Ex(PQ_WIN eWindow, MS_U16 u16GRuleType, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_LvlNum_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.ePQEngineType = ePQEngineType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_LV1NUM_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_GRuleNum_U2(void* pInstance,PQ_WIN eWindow)
{
    MS_U16 u16Ret = 0;

    if(gbPQBinEnable)
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            u16Ret = stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            u16Ret = stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
        }
        else
        {
            printf("=Error=%s, %d, =Not support case \n", __FUNCTION__, __LINE__);
        }
#endif
    }

    return u16Ret;
}
MS_U16 MDrv_PQ_GetGRule_GRuleNum(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_GRuleNum PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_GRULENUM, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_LevelIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex)
{
    MS_U16 u16Ret = 0;
    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif
    MS_U16 u16PanelIdx = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {

        if(gbPQBinEnable)
        {

            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
            {
                if(_bCustomerMainPQEn)
                {
                    PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum));
                    u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
                    enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
                    u16PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
                    {
                        printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                        return 0;
                    }
                }
                else
                {
                    printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                    return 0;
                }
            }
            else
            {
                u16PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
            }

            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(pInstance,u16PanelIdx,
                     u16GRuleType,
                     u16GRuleLevelIndex,
                     &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum)
            {
                if(_bCustomerSubPQEn)
                {
                    PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum));
                    u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
                    enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
                    u16PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
                    {
                        printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                        return 0;
                    }
                }
                else
                {
                    printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                    return 0;
                }
            }
            else
            {
                u16PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
            }

            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(pInstance,u16PanelIdx,
                     u16GRuleType,
                     u16GRuleLevelIndex,
                     &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(SUB,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16Ret;
}
MS_U16 MDrv_PQ_GetGRule_LevelIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_LevelIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16GRuleLevelIndex = u16GRuleLevelIndex;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_LEVELINDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_LevelIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 u16Ret = PQ_BIN_IP_NULL;
    MS_U8 u8PanelIdx = 0;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_MAIN;
                }
                else
                {
                    u16Ret = MDrv_PQ_GetGRule_LevelIndex_(MAIN, pInstance,(MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
                }
                break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx_UFSC(pInstance);
                    enPQBinType = PQ_BIN_UFSC;
                }
                break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                else
                {
                    u16Ret = MDrv_PQ_GetGRule_LevelIndex_(SUB, pInstance,(MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u8PanelIdx = MDrv_PQBin_GetPanelIdx(pInstance);
                    enPQBinType = PQ_BIN_EXT_SUB;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
                {
                    u8PanelIdx = PQ_BIN_CUSTOMER_PANEL_INDEX;
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
                }
                break;
            case PQ_XC0_UFSC:
                break;
            default:
                break;
        }

    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        if(u16GRuleType >= stPQBinHeaderInfo[enPQBinType].u16GRule_RuleNum)
        {
            printf("[PQ ERROR] %s:%d\nenPQBinType = %d, u16GruleType = %d out of %d\n", __FUNCTION__, __LINE__, enPQBinType, u16GRuleType, stPQBinHeaderInfo[enPQBinType].u16GRule_RuleNum);
        }
        else
        {
            u16Ret = MDrv_PQBin_GetGRule_LevelIndex(pInstance,
                                                    u8PanelIdx,
                                                    u16GRuleType,
                                                    u16GRuleLevelIndex,
                                                    &stPQBinHeaderInfo[enPQBinType]);
        }
    }

    return u16Ret;
}

MS_U16 MDrv_PQ_GetGRule_LevelIndex_Ex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_LevelIndex_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16GRuleLevelIndex = u16GRuleLevelIndex;
    PQArgs.ePQEngineType = ePQEngineType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_LEVELINDEX_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_IPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(pInstance,u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(pInstance,u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(SUB,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}
MS_U16 MDrv_PQ_GetGRule_IPIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_IPIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_IPINDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_IPIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 u16Ret = PQ_BIN_IP_NULL;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    enPQBinType = PQ_BIN_STD_MAIN;
    }
    else
    {
                    u16Ret = MDrv_PQ_GetGRule_IPIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
    }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
    {
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
            {
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
            }
            break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
            {
                    enPQBinType = PQ_BIN_UFSC;
            }
            break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
            {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                if (gbPQBinEnable)
                {
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                else
                {
                    u16Ret = MDrv_PQ_GetGRule_IPIndex_(SUB,pInstance, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
            }
            break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
            {
                    enPQBinType = PQ_BIN_EXT_SUB;
            }
            break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
            {
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
            }
            break;
            case PQ_XC0_UFSC:
                break;
        default:
            break;
    }

}
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        u16Ret = MDrv_PQBin_GetGRule_IPIndex(pInstance,
                                            u16GRuleType,
                                            u16GRuleIPIndex,
                                            &stPQBinHeaderInfo[enPQBinType]);
    }

    return u16Ret;
}

MS_U16 MDrv_PQ_GetGRule_IPIndex_Ex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_IPIndex_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.ePQEngineType = ePQEngineType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_IPINDEX_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_TableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
        {
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}
MS_U16 MDrv_PQ_GetGRule_TableIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_GetGRule_TableIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16PQ_NRIdx = u16PQ_NRIdx;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_TABLEINDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetGRule_TableIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType)
{
    MS_U16 u16Ret = PQ_BIN_IP_NULL, u16SrcIdx = 0;
    PQ_BIN_TYPE enPQBinType = MAX_PQ_BIN_NUM;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                u16SrcIdx = _u16PQSrcType[eWindow];
                if (gbPQBinEnable)
                {
                    enPQBinType = PQ_BIN_STD_MAIN;
                }
                else
                {
                    u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN, pInstance,(MS_U8)u16GRuleType, u16SrcIdx, (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    enPQBinType = PQ_BIN_EXT_MAIN;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerMainPQEn)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    enPQBinType = PQ_BIN_CUSTOMER_MAIN;
                }
                break;
            case PQ_XC0_UFSC:
                if (_bUFSCPQEn)
                {
                    u16SrcIdx = _u16PQSrcType_UFSC[eWindow];
                    enPQBinType = PQ_BIN_UFSC;
                }
                break;
            default:
                break;
        }
    }
#if (PQ_ENABLE_PIP)
    else if (eWindow == PQ_SUB_WINDOW)
    {
        switch (ePQEngineType)
        {
            case PQ_XC0_STD:
                u16SrcIdx = _u16PQSrcType[eWindow];
                if (gbPQBinEnable)
                {
                    enPQBinType = PQ_BIN_STD_SUB;
                }
                else
                {
                    u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB,pInstance, (MS_U8)u16GRuleType, u16SrcIdx, (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
                }
                break;
            case PQ_XC0_EXT:
                if (gbPQBinEnable)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    enPQBinType = PQ_BIN_EXT_SUB;
                }
                break;
            case PQ_XC0_CUSTOMER:
                if (_bCustomerSubPQEn)
                {
                    u16SrcIdx = _u16PQSrcType[eWindow];
                    enPQBinType = PQ_BIN_CUSTOMER_SUB;
                }
                break;
            case PQ_XC0_UFSC:
                break;
            default:
                break;
        }

    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    if (enPQBinType != MAX_PQ_BIN_NUM)
    {
        u16Ret = MDrv_PQBin_GetGRule_TableIndex(pInstance,
                                                u16GRuleType,
                                                u16SrcIdx,
                                                u16PQ_NRIdx,
                                                u16GRuleIPIndex,
                                                &stPQBinHeaderInfo[enPQBinType]);
    }

    return u16Ret;
}

MS_U16 MDrv_PQ_GetGRule_TableIndex_Ex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
    if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_GetGRule_TableIndex_Ex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16PQ_NRIdx = u16PQ_NRIdx;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.ePQEngineType = ePQEngineType;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_GRULE_TABLEINDEX_EX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_U16 MDrv_PQ_GetCustomerGRule_IPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
    if(_bCustomerMainPQEn || _bCustomerSubPQEn)
    {
        MS_U16 u16Ret = 0;
        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(gbPQBinEnable)
            {
                u16Ret = MDrv_PQBin_GetGRule_IPIndex(pInstance,u16GRuleType,
                        u16GRuleIPIndex,
                        &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
            }
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            if(gbPQBinEnable)
            {
                u16Ret = MDrv_PQBin_GetGRule_IPIndex(pInstance,u16GRuleType,
                        u16GRuleIPIndex,
                        &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
            }
        }
#endif
        else
        {
            MS_ASSERT(0);
        }

        return u16Ret;
    }
    else
    {
        MS_ASSERT(0);
        return 0;
    }
}
MS_U16 MDrv_PQ_GetCustomerGRule_IPIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetCustomerGRule_IPIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CUSTOMERGRULE_IPINDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}


MS_U16 MDrv_PQ_GetGRule_CustomerTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    if(_bCustomerMainPQEn || _bCustomerSubPQEn)
    {
        MS_U16 u16Ret = 0;
        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(gbPQBinEnable)
            {
                u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                        _u16PQSrcType[eWindow],
                        u16PQ_NRIdx,
                        u16GRuleIPIndex,
                        &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
            }
            else
            {
                u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
            }
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            if(gbPQBinEnable)
            {
                u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(pInstance,u16GRuleType,
                        _u16PQSrcType[eWindow],
                        u16PQ_NRIdx,
                        u16GRuleIPIndex,
                        &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
            }
            else
            {
                u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB,pInstance, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
            }
        }
#endif
        else
        {
            MS_ASSERT(0);
        }

        return u16Ret;
    }
    else
    {
        return 0;
    }
}
MS_U16 MDrv_PQ_GetGRule_CustomerTableIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetGRule_CustomerTableIndex PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16GRuleType = u16GRuleType;
    PQArgs.u16PQ_NRIdx = u16PQ_NRIdx;
    PQArgs.u16GRuleIPIndex = u16GRuleIPIndex;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_CUSTOMERGRULE_TABLEINDEX, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_BOOL MDrv_PQ_Set_xvYCC_MatrixCoefficient_U2(void* pInstance,
                                            MS_U64 u64Red_x,   MS_U64 u64Red_y,
                                            MS_U64 u64Green_x, MS_U64 u64Green_y,
                                            MS_U64 u64Blue_x,  MS_U64 u64Blue_y,
                                            MS_U64 u64White_x, MS_U64 u64White_y,
                                            PQ_WIN eWindow)
{
    MS_U32 u32idx, s32H_idx,u32V_idx, u32cnt;
    MS_U64 s64Mat[3][ 3] =  { {0},{0},{0}};
    MS_S64 s64sRGBMat[3][3];
    MS_S64 s64RGB3x3[ 3][ 3];
    MS_S32 s32RGB3x3[ 3][ 3];
    MS_U32 __attribute__((__unused__))u32data_invalid[ 9];
    MS_S32 s32fix_value; //format s2.10
    MS_S64 dbUp_value, dbDown_value;
    MS_S64 dbHistogramSum[3];
    MS_S64 dbMaxHistogramSum;

    if (eWindow >= PQ_MAX_WINDOW)
    {
        printf("Invalid window type, please check ! \n");
        return FALSE;
    }

    for(u32idx=0; u32idx<9; u32idx++)
    {
        u32data_invalid[ u32idx] = 0;
    }

    // calculation inverse Matrix Mat[3][3]
    // u32data_invalid is used when the inverse of matrix is not existed
    // for (1,1)
    dbUp_value   = (MS_S64)(u64White_y*(-u64Blue_y + u64Green_x*u64Blue_y + u64Green_y - u64Blue_x*u64Green_y) ) ;
    dbDown_value = (MS_S64)(-u64Green_x*u64White_y + u64Blue_x*u64White_y + u64Green_x*u64Blue_y - u64Blue_y*u64White_x + u64Green_y*u64White_x - u64Blue_x*u64Green_y);
    if( dbDown_value ==0)\
    {
        u32data_invalid[ 0] = 1;
    }
    else
    {
        u32data_invalid[ 0] =0;
        s64Mat[ 0][ 0] = do_div(dbUp_value,dbDown_value);
    }

    // for (1,2)
    dbUp_value   = (MS_S64)(u64White_y*(u64Green_x*u64Blue_y -u64Green_x + u64Blue_x - u64Blue_x*u64Green_y)) ;
    dbDown_value = (MS_S64)(-u64Green_x*u64White_y + u64Blue_x*u64White_y + u64Green_x*u64Blue_y - u64Blue_y*u64White_x + u64Green_y*u64White_x - u64Blue_x*u64Green_y);
    if( dbDown_value ==0)
    {
        u32data_invalid[ 1] = 1;
    }
    else
    {
        u32data_invalid[ 1] =0;
        s64Mat[ 0][ 1] = do_div(dbUp_value,dbDown_value);
    }

    // for (1,3)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Green_x*u64Blue_y - u64Blue_x*u64Green_y) ) ;
    dbDown_value = (MS_S64) ( -u64Green_x*u64White_y + u64Blue_x*u64White_y + u64Green_x*u64Blue_y - u64Blue_y*u64White_x + u64Green_y*u64White_x - u64Blue_x*u64Green_y);
    if( dbDown_value ==0)
    {
        u32data_invalid[ 2] = 1;
    }
    else
    {
        u32data_invalid[ 2] =0;
        s64Mat[ 0][ 2] = do_div(dbUp_value,dbDown_value);
    }

    // for (2,1)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Red_y - u64Red_y*u64Blue_x - u64Blue_y + u64Red_x*u64Blue_y) ) ;
    dbDown_value = (MS_S64) ( u64Red_y*u64White_x + u64Red_x*u64Blue_y - u64Red_y*u64Blue_x - u64Blue_y*u64White_x + u64Blue_x*u64White_y - u64Red_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 3] = 1;
    }
    else
    {
        u32data_invalid[ 3] =0;
        s64Mat[ 1][ 0] = do_div(dbUp_value,dbDown_value);
    }

    // for (2,2)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Red_x*u64Blue_y - u64Red_x + u64Blue_x - u64Red_y*u64Blue_x) ) ;
    dbDown_value = (MS_S64) ( u64Red_y*u64White_x + u64Red_x*u64Blue_y - u64Red_y*u64Blue_x - u64Blue_y*u64White_x + u64Blue_x*u64White_y - u64Red_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 4] = 1;
    }
    else
    {
        u32data_invalid[ 4] =0;
        s64Mat[ 1][ 1] = do_div(dbUp_value,dbDown_value);
    }

    // for (2,3)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Red_x*u64Blue_y - u64Red_y*u64Blue_x) ) ;
    dbDown_value = (MS_S64) ( u64Red_y*u64White_x + u64Red_x*u64Blue_y - u64Red_y*u64Blue_x - u64Blue_y*u64White_x + u64Blue_x*u64White_y - u64Red_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 5] = 1;
    }
    else
    {
        u32data_invalid[ 5] =0;
        s64Mat[ 1][ 2] = do_div(dbUp_value,dbDown_value);
    }

    // for (3,1)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Red_x*u64Green_y + u64Red_y - u64Green_y -u64Green_x*u64Red_y) ) ;
    dbDown_value = (MS_S64) ( -u64Green_x*u64Red_y + u64Red_y*u64White_x -u64Green_y*u64White_x -u64Red_x*u64White_y + u64Red_x*u64Green_y + u64Green_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 6] = 1;
    }
    else
    {
        u32data_invalid[ 6] =0;
        s64Mat[ 2][ 0] = do_div(dbUp_value,dbDown_value);
    }

    // for (3,2)
    dbUp_value   = (MS_S64) ( u64White_y* ( -u64Red_x + u64Red_x*u64Green_y + u64Green_x - u64Green_x*u64Red_y) ) ;
    dbDown_value = (MS_S64) ( -u64Green_x*u64Red_y + u64Red_y*u64White_x -u64Green_y*u64White_x -u64Red_x*u64White_y + u64Red_x*u64Green_y + u64Green_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 7] = 1;
    }
    else
    {
        u32data_invalid[ 7] =0;
        s64Mat[ 2][ 1] = do_div(dbUp_value,dbDown_value);
    }

    // for (3,3)
    dbUp_value   = (MS_S64) ( u64White_y* ( u64Red_x*u64Green_y - u64Green_x*u64Red_y) ) ;
    dbDown_value = (MS_S64) ( -u64Green_x*u64Red_y + u64Red_y*u64White_x -u64Green_y*u64White_x -u64Red_x*u64White_y + u64Red_x*u64Green_y + u64Green_x*u64White_y) ;
    if( dbDown_value ==0)
    {
        u32data_invalid[ 8] = 1;
    }
    else
    {
        u32data_invalid[ 8] =0;
        s64Mat[ 2][ 2] = do_div(dbUp_value,dbDown_value);
    }

    #if 0   // this is used for debugging matrix
    int k = 0, o=0;
    for(k=0;k<3;k++)
    {
        for (o=0;o<3;o++)
        {
            printf("Mat[%d][%d] = %f\n",k,o,flMat[k][o]);
        }
    }
    #endif

    //sRGB Matrix
    s64sRGBMat[0][0]= 412390;//0.412390799;
    s64sRGBMat[0][1]= 357584;//0.357584339;
    s64sRGBMat[0][2]= 180480;//0.180480788;
    s64sRGBMat[1][0]= 212639;//0.212639006;
    s64sRGBMat[1][1]= 715168;//0.715168679;
    s64sRGBMat[1][2]= 72192; //0.072192315;
    s64sRGBMat[2][0]= 19330; //0.019330819;
    s64sRGBMat[2][1]= 119194;//0.11919478;
    s64sRGBMat[2][2]= 950532;//0.950532152;
    //--------------------------------------------------------------------

    // RGB2XYZ * Mat( XYZ2RGB)
    for( u32V_idx =0; u32V_idx<3; u32V_idx++)
    {
        dbHistogramSum[u32V_idx]=0;
        for( s32H_idx=0; s32H_idx<3; s32H_idx++)
        {
            MS_U64 s32Sum=0;
            for( u32cnt=0; u32cnt<3; u32cnt++)
            {
                s32Sum += s64Mat[ u32V_idx][ u32cnt] * s64sRGBMat[ u32cnt][ s32H_idx];
            }

            s64RGB3x3[ u32V_idx][ s32H_idx] = s32Sum;

            //printf("rgb3x3[%ld][%ld] = %f\n",u32V_idx,s32H_idx,flRGB3x3[u32V_idx][s32H_idx]);

            dbHistogramSum[u32V_idx]=dbHistogramSum[u32V_idx]+s32Sum;
        }
    }



    //Normalize
    dbMaxHistogramSum=0;
    for( u32V_idx =0; u32V_idx<3; u32V_idx++)
    {
        if (dbHistogramSum[u32V_idx]>=dbMaxHistogramSum)
        {
            dbMaxHistogramSum=dbHistogramSum[u32V_idx];
        }

    }

    MS_S64 s64_tmp=0;
    //convert to s2.10 format
    for( u32V_idx =0; u32V_idx<3; u32V_idx++)
    {
        for( s32H_idx=0; s32H_idx<3; s32H_idx++)
        {
            s64_tmp = (MS_S64) s64RGB3x3[u32V_idx][s32H_idx];
            do_div(s64_tmp,(dbMaxHistogramSum*ENLARGE_GATE));
            s64RGB3x3[u32V_idx][s32H_idx]=s64_tmp;

            //MS_S64 round_value;
            //if (s64RGB3x3[u32V_idx][s32H_idx]>=0)
            //{
            //    round_value= s64RGB3x3[u32V_idx][s32H_idx] *1024 + 0.5;
            //}
            //else
            //{
            //    round_value= s64RGB3x3[u32V_idx][s32H_idx] *1024 - 0.5;
            //}

            s32fix_value = (MS_S32)(s64RGB3x3[u32V_idx][s32H_idx]);
            MS_U32 abs_fix =(MS_U32) abs(s64RGB3x3[u32V_idx][s32H_idx]);

            if( (abs_fix !=0) && (s32fix_value<0) )
            {
                MS_S32 sbits = 1<<12;
                s32RGB3x3[u32V_idx][ s32H_idx] = sbits + abs_fix ;
            }
            else
            {
                s32RGB3x3[ u32V_idx][ s32H_idx] = s32fix_value;
            }

         //printf("s32RGB3x3[%ld][%ld] = %ld\n",u32V_idx,s32H_idx,s32RGB3x3[u32V_idx][s32H_idx]);

         // convert signed matrix of s2.10 format to 2's complement for negative value
         MS_U32 u32current_value = s32RGB3x3[u32V_idx][s32H_idx] & 0xFFF; // remove carry bit
         if((s32RGB3x3[u32V_idx][s32H_idx] >> 12) == TRUE) // it is negative
         {
            s32RGB3x3[u32V_idx][s32H_idx] = ((~u32current_value)& 0x1FFF )+1;
         }
        }
    }

    Hal_PQ_set_xvycc_matrix_coefficient(pInstance,s32RGB3x3, eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
    return TRUE;
}
MS_BOOL MDrv_PQ_Set_xvYCC_MatrixCoefficient(MS_U64 u64Red_x,   MS_U64 u64Red_y,
                                            MS_U64 u64Green_x, MS_U64 u64Green_y,
                                            MS_U64 u64Blue_x,  MS_U64 u64Blue_y,
                                            MS_U64 u64White_x, MS_U64 u64White_y,
                                            PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set_xvYCC_MatrixCoefficient PQArgs;
    PQArgs.u64Red_x = u64Red_x;
    PQArgs.u64Red_y = u64Red_y;
    PQArgs.u64Green_x = u64Green_x;
    PQArgs.u64Green_y = u64Green_y;
    PQArgs.u64Blue_x = u64Blue_x;
    PQArgs.u64Blue_y = u64Blue_y;
    PQArgs.u64White_x = u64White_x;
    PQArgs.u64White_y = u64White_y;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_XVYCC_MATRIXCOEFFICIENT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Set_xvYCC_MatrixEnable_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow)
{
    if (eWindow >= PQ_MAX_WINDOW)
    {
        printf("Invalid window type, please check ! \n");
        return FALSE;
    }

    Hal_PQ_set_xvycc_matrix_enable(pInstance,bEnable, eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
    return TRUE;
}
MS_BOOL MDrv_PQ_Set_xvYCC_MatrixEnable(MS_BOOL bEnable, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set_xvYCC_MatrixEnable PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_XVYCC_MATRIXENABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U16 MDrv_PQ_Get_DNR_Whole_Reg_U2(void* pInstance,PQ_WIN eWindow)
{
    return 0;//Hal_PQ_get_dnr_whole_reg(eWindow);
}

MS_U16 MDrv_PQ_Get_DNR_Whole_Reg(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m(%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetDnrWholeReg PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u16Ret = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_DNR_WHOLE_REG, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return 0;
    }
    else
    {
        return PQArgs.u16Ret;
    }
}

MS_BOOL MDrv_PQ_Up_Layer_Set_Config_U2(void* pInstance,MS_U16 uplayerSetting, PQ_MISC_GROUP group)
{
    switch(group)
    {
        case E_PQ_GROUP_A:
            _upLayerConfig = uplayerSetting;
        break;
        //case E_PQ_GROUP_B: ....
        default:
            return FALSE;
        break;
    }
    return TRUE;
}

MS_BOOL MDrv_PQ_Up_Layer_Set_Config(MS_U16 uplayerSetting, PQ_MISC_GROUP group)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Up_Layer_Set_Config PQArgs;
    PQArgs.uplayerSetting = uplayerSetting;
    PQArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_UP_LAYER_SET_CONFIG, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U32 _MDrv_PQ_GET_DEVICE_NUM(void)
{
    return PQ_SUPPORT_DEVICE_NUM;
}
char* MDrv_PQ_Ex_GetVersion_U2(void* pInstance,PQ_WIN eWindow)
{
    if(gbPQBinEnable == 1)
    {
        char* Ex_PQ_Version;

        if(PQ_MAIN_WINDOW == eWindow)
        {
            Ex_PQ_Version = stPQBinHeaderInfo[PQ_BIN_EXT_MAIN].u8Version;
        }
        else
        {
            Ex_PQ_Version = stPQBinHeaderInfo[PQ_BIN_EXT_SUB].u8Version;
        }
        return Ex_PQ_Version;
    }
    else
    {
        return NULL;
    }
}
char* MDrv_PQ_Ex_GetVersion(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return NULL;
    }

    stPQ_Ex_GetVersion PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_EX_GET_VERSION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_U32  MDrv_PQ_GetAdaptiveVersion_U2(void* pInstance,PQ_WIN eWindow)
{
        MS_U32  PQ_Adaptive_Version = PQ_ADAPTIVE_INVALID_VERSION;
#if (PQ_SUPPORT_ADAPTIVE_VERSION)
        PQ_Adaptive_Version = Get_PQ_Adaptive_Version();
#else
        PQ_Adaptive_Version = PQ_ADAPTIVE_INVALID_VERSION;
#endif
        return PQ_Adaptive_Version;
}
MS_U32  MDrv_PQ_GetAdaptiveVersion(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return NULL;
    }

    stPQ_GetAdaptiveVersion PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = 0;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_ADAPTVIE_VERSION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}
void MDrv_PQ_LoadUFSCSettings_U2(void *pInstance,PQ_WIN eWindow)
{
    if(_bUFSCPQEn && (eWindow == PQ_MAIN_WINDOW))
    {
        _u16PQSrcType_UFSC[eWindow] = QM_InputSourceToIndex_UFSC(pInstance,eWindow, _enInputSourceType[eWindow]);
        MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType_UFSC[eWindow],
                                      PQ_BIN_IP_ALL,
                                      MDrv_PQBin_GetPanelIdx_UFSC(pInstance),
                                      &stPQBinHeaderInfo[PQ_BIN_UFSC],
                                      eWindow);
    }
}

void MDrv_PQ_LoadUFSCSettings(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadUFSCSettings PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_UFSC_SETTING, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}
#if (ENABLE_PQ_BIN_CF)
void MDrv_PQ_LoadCFSettings_U2(void *pInstance,PQ_WIN eWindow)
{
    if(_bCFMainPQEn && (eWindow == PQ_MAIN_WINDOW))
    {
        _u16PQSrcType_CF[eWindow] = QM_InputSourceToIndex_CF(pInstance,eWindow, _enInputSourceType[eWindow]);
        MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType_CF[eWindow],
                                      PQ_BIN_IP_ALL,
                                      PQ_BIN_CF_PANEL_INDEX,
                                      &stPQBinHeaderInfo[PQ_BIN_CF_MAIN],
                                      eWindow);
    }
#if (PQ_ENABLE_PIP)
    else if(_bCFSubPQEn &&(eWindow == PQ_SUB_WINDOW))
    {
        _u16PQSrcType_CF[eWindow] = QM_InputSourceToIndex_CF(pInstance,eWindow, _enInputSourceType[eWindow]);
        MDrv_PQBin_LoadTableBySrcType(pInstance,_u16PQSrcType_CF[eWindow],
                                      PQ_BIN_IP_ALL,
                                      PQ_BIN_CF_PANEL_INDEX,
                                      &stPQBinHeaderInfo[PQ_BIN_CF_SUB],
                                      eWindow);
    }
#endif
    else
    {
        printf("NO CF QMAP!!!");
    }
}

void MDrv_PQ_LoadCFSettings(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m(%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadCFSettings PQArgs;
    PQArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_CF_SETTING, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}
#endif

void MDrv_PQ_SetP2pForceToDoCsc(MS_BOOL bEnable)
{
    printf("[%s:%d] bEnable=%d\n", __FUNCTION__, __LINE__, bEnable);
    _bP2pForceToDoCsc = bEnable;
}

void MDrv_PQ_SetOutputColorFormat(PQ_COLOR_FORMAT enColorFmt)
{
    printf("[%s:%d] enColorFmt=%d\n", __FUNCTION__, __LINE__, enColorFmt);
    _enOutputColorFmt = enColorFmt;
}

MS_BOOL MDrv_PQ_GetPQPathStatus(E_PQ_Path_Type ePqPathType, MS_U16 u16Width, MS_U16 u16Height)
{
#ifdef UFO_XC_PQ_PATH
    return MApi_XC_GetPQPathStatus((E_XC_PQ_Path_Type)ePqPathType, u16Width, u16Height);
#else
    return PQ_IS_4K_INPUT(u16Width, u16Height);
#endif
}

void MDrv_PQ_SetPQBinPath_U2(void* pInstance,PQ_BIN_PATH ePqBinPath,MS_U8 u8size,char* b_PQBinFilePath)
{
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPQInstPri);
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    MDrv_PQ_Get_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
    UtopiaResourceGetPrivate(g_pPQResource[E_PQ_DEVICE_ID_0],(void**)(&pPQResourcePrivate));

    if(b_PQBinFilePath == NULL)
    {
        printf("ERROR!! NULL BIN PATH!!!!!\n");
    }
    else
    {
        switch(ePqBinPath)
        {
            case E_PQ_BIN_PATH_CUSTOMER:
            {
                memset(pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH,0,PQ_FILE_PATH_LENGTH);
                strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH,b_PQBinFilePath,PQ_FILE_PATH_LENGTH - 1);
                break;
            }
            case E_PQ_BIN_PATH_DEFAULT:
            {
                memset(pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH,0,PQ_FILE_PATH_LENGTH);
                strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH,b_PQBinFilePath,PQ_FILE_PATH_LENGTH - 1);
                break;
            }
            case E_PQ_BIN_PATH_INI:
            {
                memset(pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH,0,PQ_FILE_PATH_LENGTH);
                strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH,b_PQBinFilePath,PQ_FILE_PATH_LENGTH - 1);
                break;
            }
            case E_PQ_BIN_PATH_BANDWIDTH:
            {
                memset(pPQResourcePrivate->stPQBin_Path.pm_PQ_BANDWIDTH_PATH,0,PQ_FILE_PATH_LENGTH);
                strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_BANDWIDTH_PATH,b_PQBinFilePath,PQ_FILE_PATH_LENGTH - 1);
                break;
            }
            default:
                printf("ERROR!! SET WRONG PQ_BIN_PATH TYPE!!!!!\n");
                break;

        }
    }
    MDrv_PQ_Release_Semaphore(pInstance,E_PQ_POOL_ID_INTERNAL);
}
void MDrv_PQ_SetPQBinPath(PQ_BIN_PATH ePqBinPath,MS_U8 u8size,char* b_PQBinFilePath)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
        //return;
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
        printf("\33[1;33m[MARCOS](%s %d) OPEN FINE\33[m\n", __FUNCTION__, __LINE__);
    }

    stPQ_SetPQBinPath PQArgs;
    PQArgs.ePqBinPath = ePqBinPath;
    PQArgs.u8size = u8size;
    PQArgs.b_PQBinFilePath = b_PQBinFilePath;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_PQBIN_PATH, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
}

EXPORT_SYMBOL(MDrv_PQ_SetCSC);
EXPORT_SYMBOL(MDrv_PQ_Init);
EXPORT_SYMBOL(MDrv_PQ_DesideSrcType);
EXPORT_SYMBOL(MDrv_PQ_SetMemFormat);

EXPORT_SYMBOL(MDrv_PQ_Exit);
EXPORT_SYMBOL(MDrv_PQ_LoadScalingTable);
EXPORT_SYMBOL(MDrv_PQ_IOCTL);
EXPORT_SYMBOL(MDrv_PQ_DisableFilmMode);

EXPORT_SYMBOL(MDrv_PQ_Set420upsampling);
EXPORT_SYMBOL(MDrv_PQ_Get_MemYUVFmt);

EXPORT_SYMBOL(MDrv_PQ_Set_ModeInfo);

EXPORT_SYMBOL(MDrv_PQ_LoadSettings);
EXPORT_SYMBOL(MDrv_PQ_SkipDuplicatedSetting);
EXPORT_SYMBOL(MDrv_PQ_IsForceVideoInputMode);
EXPORT_SYMBOL(MDrv_PQ_LoadCustomerSettings);
EXPORT_SYMBOL(MDrv_PQ_EnableScalerGamma);
EXPORT_SYMBOL(MDrv_PQ_GetDelayTime);
EXPORT_SYMBOL(MDrv_PQ_Set_MultiMediaInfo);
EXPORT_SYMBOL(MDrv_PQ_SetFrameBuffMode);

