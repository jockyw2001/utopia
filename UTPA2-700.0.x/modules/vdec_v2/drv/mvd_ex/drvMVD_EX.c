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
/// file  drvMVD.c
/// @brief MPEG-2/4 Video Decoder Driver
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include "drvMIU.h"
#include "drvMVD_EX.h"

// Internal Definition
#include "halMVD_EX.h"
#include "asmCPU.h"
#include "osalMVD_EX.h"
#include "drvmvd_cc.h"      //ATSC Closed Caption
#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////
//Different level debug message
#if ((defined(CHIP_A1) || defined(CHIP_A7) || defined(CHIP_AMETHYST)) && defined (__aeon__))
#define MVD_DEBUGVERBAL(x)
#define MVD_DEBUGINFO(x)
#else
#define MVD_DEBUGVERBAL(x)    if (_u8DbgLevel>4)  { (x); }
#define MVD_DEBUGINFO(x)      if (_u8DbgLevel>1)  { (x); }
#endif
#define MVD_DEBUGERROR(x)     if (_u8DbgLevel>0)  { (x); }
#define MVD_FUNC_ENTRY()      {}//MVD_PRINT(" $$$ drvMVD::%s\n", __FUNCTION__)

#ifndef ANDROID
#define MVD_PRINT  printf
#define MVD_ERR printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>
#ifndef LOGI // android 4.1 rename LOGx to ALOGx
#define MVD_PRINT ALOGI
#else
#define MVD_PRINT LOGI
#endif
#ifndef LOGE // android 4.1 rename LOGx to ALOGx
#define MVD_ERR ALOGE
#else
#define MVD_ERR LOGE
#endif
#endif

#define DISABLE_ISR_DETACH

#define _DUMP_FRMINFO         FALSE

#define _MS_TO_90K(x)         (x*90)  //ms ==> 90k counter
#define _90K_TO_MS(x)         (x/90)  //90k counter ==> ms

#define _MVD_GET_IDX(x)       (MS_U8)(((x) >>16) & 0xff)
#define _MVD_GET_HAL_SID(x)   (HAL_MVD_StreamId)(((x) >>8) & 0xff)
#define _MVD_GET_VPU_SID(x)   (HAL_VPU_StreamId)((x) & 0xff)

#define MVD_U32_MAX                 0xffffffffUL
#define MVD_U32_MASK               0xf0000000UL
#define PB_ONLY_UNMUTE_VSYNC_COUNT          30

#define MVD_MAX_STREAMS         2

////////////////////////////////////////////////////////////////////////////////
// Local Global Variables
////////////////////////////////////////////////////////////////////////////////
#if 0
/// Version string
static MSIF_Version _drv_mvd_version = {
    .DDI = { MVD_DRV_VERSION, },
};

static MVD_DrvInfo _stDrvInfo = {
                                    1,      //1 MVD HW
                                    0,      //Device#   fixme
                                    MVD_U32_MAX,        //firmware version
                                    { FALSE, FALSE, FALSE } //capability
                                };

static MS_U8 _u8DbgLevel = 0;

static MS_BOOL bFrmRateSupported[MVD_MAX_STREAMS];
static MVD_FrameInfo stPreFrmInfo[MVD_MAX_STREAMS];
static MVD_AVSyncCfg stNFPSyncCfg[MVD_MAX_STREAMS];
static MVD_AVSyncCfg stSyncCfg[MVD_MAX_STREAMS];

#ifdef MVD_ENABLE_ISR
static MVD_InterruptCb _pfnCallback[MVD_MAX_STREAMS];
static MS_U32 _eEventFlag[MVD_MAX_STREAMS];
static MS_U32 _eCurEvent[MVD_MAX_STREAMS];
static MS_U32 _u32CallbackPara[MVD_MAX_STREAMS];
#endif

static MS_U32 u32SyncDoneCnt = 0;
static MS_U32 u32VSyncCnt = 0;
static MS_BOOL bMVDIsrAttached = FALSE;
static MS_BOOL bSuspendDS_Ftime=TRUE; //to check 3d<->2d mode first time

typedef struct
{
    MS_BOOL bUsed;      //updated when init/exit
    MS_U8   u8HalIdx;   //index used by HAL. use it to communicate with HAL.
    MS_U16  u16Rsvd;    //reserved bytes

    MS_U32  u32VpuSid;  //StreamID obtained from VPU by HAL_VPU_GetFreeStream()
    MS_U32  u32MvdSid;  //StreamID obtained from VPU by HAL_MVD_GetFreeStream()
} MVD_Drv_Ctrl;
static MVD_Drv_Ctrl gMVDCtrl_EX[MVD_MAX_STREAMS]
    = { { FALSE, 0, 0, 0, 0},
        { FALSE, 0, 0, 0, 0},
      };//ToDo: check when to init/reset!
#endif
typedef struct
{
    MS_BOOL bUsed;      //updated when init/exit
    MS_U8   u8HalIdx;   //index used by HAL. use it to communicate with HAL.
    MS_U16  u16Rsvd;    //reserved bytes

    MS_U32  u32VpuSid;  //StreamID obtained from VPU by HAL_VPU_GetFreeStream()
    MS_U32  u32MvdSid;  //StreamID obtained from VPU by HAL_MVD_GetFreeStream()
} MVD_Drv_Ctrl;

typedef struct
{
    MS_BOOL bFrmRateSupported[MVD_MAX_STREAMS];
    MVD_FrameInfo stPreFrmInfo[MVD_MAX_STREAMS];
    MVD_AVSyncCfg stNFPSyncCfg[MVD_MAX_STREAMS];
    MVD_AVSyncCfg stSyncCfg[MVD_MAX_STREAMS];

#ifdef MVD_ENABLE_ISR
    MVD_InterruptCb _pfnCallback[MVD_MAX_STREAMS];
    MS_U32 _eEventFlag[MVD_MAX_STREAMS];
    MS_U32 _eCurEvent[MVD_MAX_STREAMS];
    MS_U32 _u32CallbackPara[MVD_MAX_STREAMS];
    MS_BOOL _bDisableFlag;
#endif

    MS_U32 u32SyncDoneCnt[MVD_MAX_STREAMS];
    MS_U32 u32VSyncCnt[MVD_MAX_STREAMS];
    MS_BOOL bMVDIsrAttached;
    MS_BOOL bSuspendDS_Ftime[MVD_MAX_STREAMS];

    MVD_Drv_Ctrl gMVDCtrl_EX[MVD_MAX_STREAMS];//ToDo: check when to init/reset!

    MS_U8 u8Afd[MVD_MAX_STREAMS];//MDrv_MVD_GetActiveFormat()
#if (MVD_TURBO_INIT)
    MS_U32 u32FrmCnt[MVD_MAX_STREAMS];//MDrv_MVD_GetActiveFormat()
#endif
    MS_BOOL bSelf_SeqChange[MVD_MAX_STREAMS];
    //pre_set
    MVD_Pre_Ctrl gMVDPreCtrl[MVD_MAX_STREAMS];

} MVD_Drv_CTX;

// global variables
MVD_Drv_CTX* pMVDDrvContext = NULL;
MVD_Drv_CTX gMVDDrvContext;
MSIF_Version _drv_mvd_version = {
                                  .DDI = { MVD_DRV_VERSION, },
                                };
MVD_DrvInfo _stDrvInfo = {
                           1,      //1 MVD HW
                           0,      //Device#   fixme
                           MVD_U32_MAX,        //firmware version
                           {FALSE, FALSE, FALSE} //capability
                         };
MS_U8 _u8DbgLevel = 0;


////////////////////////////////////////////////////////////////////////////////
// Local functions
////////////////////////////////////////////////////////////////////////////////

static void MVD_Context_Init(void)
{
    pMVDDrvContext->bSuspendDS_Ftime[0] = TRUE;
    pMVDDrvContext->bSuspendDS_Ftime[1] = TRUE;
    pMVDDrvContext->u8Afd[0] = 0xff;
    pMVDDrvContext->u8Afd[1] = 0xff;
    pMVDDrvContext->_bDisableFlag = TRUE;
    pMVDDrvContext->bMVDIsrAttached = FALSE;
}


static void MVD_InitVar(MS_U8 u8DrvIdx,MS_U8 u8HalIdx);

static MS_BOOL MVD_SaveStreamId(MS_U8 u8DrvIdx, MS_U32 u32MvdSid, MS_U32 u32VpuSid)
{
    if (pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].bUsed)
    {
        MVD_ERR("%s err IsUsed: drv=%x mvd=%x vpu=%x\n", __FUNCTION__,
            u8DrvIdx, u32MvdSid, u32VpuSid);
        return FALSE;
    }
    pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u32MvdSid = u32MvdSid;
    pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u32VpuSid = u32VpuSid;
    pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u8HalIdx = HAL_MVD_SidToIdx((HAL_MVD_StreamId)u32MvdSid);
    return TRUE;
}

MS_U8 MVD_GetHalIdx(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u8HalIdx;
    return u8HalIdx;
}

static MS_U32 MVD_GetVpuStreamId(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U32 u32VpuSid = pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u32VpuSid;
    return u32VpuSid;
}

static MS_BOOL MVD_SetIsUsed(MS_U8 u8DrvIdx, MS_BOOL bUsed)
{
    pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].bUsed = bUsed;
    //MVD_PRINT("%s u8DrvIdx=0x%x bUsed=%x\n", __FUNCTION__, u8DrvIdx, bUsed);
    return TRUE;
}

static MS_BOOL MVD_GetIsUsed(MS_U8 u8DrvIdx)
{
    MS_BOOL bUsed = pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].bUsed;
    //MVD_PRINT("%s u8DrvIdx=0x%x bUsed=%x\n", __FUNCTION__, u8DrvIdx, bUsed);
    return bUsed;
}

#if _DUMP_FRMINFO
static void MVD_DumpFrmInfo(MVD_FrmInfo* pInfo)
{
    if (NULL == pInfo)
    {
        MVD_ERR("%s: pInfo invalid!\n", __FUNCTION__);
        return;
    }

    MVD_PRINT("u32LumaAddr  =0x%lx\n", pInfo->u32LumaAddr  );
    MVD_PRINT("u32ChromaAddr=0x%lx\n", pInfo->u32ChromaAddr);
    MVD_PRINT("u32TimeStamp =0x%lx\n", pInfo->u32TimeStamp );
    MVD_PRINT("u32ID_L      =0x%lx\n", pInfo->u32ID_L      );
    MVD_PRINT("u32ID_H      =0x%lx\n", pInfo->u32ID_H      );
    MVD_PRINT("u16Pitch     =0x%x\n", pInfo->u16Pitch      );
    MVD_PRINT("u16Width     =0x%x\n", pInfo->u16Width      );
    MVD_PRINT("u16Height    =0x%x\n", pInfo->u16Height     );
    MVD_PRINT("eFrmType     =0x%x\n", pInfo->eFrmType      );
    return;
}
#endif


//------------------------------------------------------------------------------
/// Get MVD driver information
/// @return -the pointer to the driver information
//------------------------------------------------------------------------------
const MVD_DrvInfo* MDrv_MVD_GetInfo(void)
{
    MDrv_MVD_GetCaps(&(_stDrvInfo.stCaps));
    _stDrvInfo.u32FWVersion = HAL_MVD_GetDrvFwVer();
    return (&_stDrvInfo);
}


//------------------------------------------------------------------------------
/// Get MVD driver version
/// @return -the pointer to the driver version
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_MVD_RET_FAIL;

    *ppVersion = &_drv_mvd_version;
    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Set detailed level of MVD driver debug message
/// 0: None, 1: MVD_DEBUGERROR, 2: MVD_DEBUGINFO
/// @param level  \b IN  level from 0 to 2
//------------------------------------------------------------------------------
void MDrv_MVD_SetDbgLevel(MS_U8 level)
{
    _u8DbgLevel = level;
    VPU_EX_UartLevel eVpuDbgLevel[6] =
    {
        E_VPU_EX_UART_LEVEL_NONE,
        E_VPU_EX_UART_LEVEL_ERR,
        E_VPU_EX_UART_LEVEL_INFO,
        E_VPU_EX_UART_LEVEL_DBG,
        E_VPU_EX_UART_LEVEL_TRACE,
        E_VPU_EX_UART_LEVEL_FW,
    };

    if (level >= 1)
    {
        HAL_MVD_SetDbgLevel(level);
    }
    if (level<6) HAL_VPU_EX_SetDbgLevel(eVpuDbgLevel[level]);

    return;
}


//------------------------------------------------------------------------------
/// Get MVD firmware version
/// @return -firmware version
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetFWVer(MS_U32 u32Id)
{
    MS_U32 u32VpuSid = MVD_GetVpuStreamId(u32Id);
    return HAL_MVD_GetFWVer(u32VpuSid);
}

//------------------------------------------------------------------------------
/// Initialize variables for MVD driver
//------------------------------------------------------------------------------
static void MVD_InitVar(MS_U8 u8DrvIdx,MS_U8 u8HalIdx)
{
    MVD_FrameInfo* pPreInfo = &(pMVDDrvContext->stPreFrmInfo[u8DrvIdx]);
    HAL_MVD_InitVar(u8HalIdx);

    pPreInfo->u16HorSize   = 0;
    pPreInfo->u16VerSize   = 0;
    pPreInfo->u8AspectRate = 0;
    pPreInfo->u32FrameRate = 0;
    pPreInfo->u8Interlace  = 0;

    memset(&(pMVDDrvContext->stSyncCfg[u8DrvIdx]), 0, sizeof(MVD_AVSyncCfg));

    pMVDDrvContext->bFrmRateSupported[u8DrvIdx] = TRUE;

    pMVDDrvContext->u32SyncDoneCnt[0] = 0;
    pMVDDrvContext->u32SyncDoneCnt[1] = 0;
    pMVDDrvContext->u32VSyncCnt[0] = 0;
    pMVDDrvContext->u32VSyncCnt[1] = 0;

    return;
}

#if 0
static void MVD_DumpMemCfg(MVD_MEMCfg* pCfg)
{
    if (pCfg)
    {
        MVD_PRINT("u32FWBinAddr          = 0x%lx\n", pCfg->u32FWBinAddr);
        MVD_PRINT("u32FWBinSize          = 0x%lx\n", pCfg->u32FWBinSize);
        MVD_PRINT("u32FWCodeAddr         = 0x%lx\n", pCfg->u32FWCodeAddr);
        MVD_PRINT("u32FWCodeSize         = 0x%lx\n", pCfg->u32FWCodeSize);
        MVD_PRINT("u32FBAddr             = 0x%lx\n", pCfg->u32FBAddr);
        MVD_PRINT("u32FBSize             = 0x%lx\n", pCfg->u32FBSize);
        MVD_PRINT("u32BSAddr             = 0x%lx\n", pCfg->u32BSAddr);
        MVD_PRINT("u32BSSize             = 0x%lx\n", pCfg->u32BSSize);
        MVD_PRINT("u32DrvBufAddr         = 0x%lx\n", pCfg->u32DrvBufAddr);
        MVD_PRINT("u32DrvBufSize         = 0x%lx\n", pCfg->u32DrvBufSize);
        MVD_PRINT("u32DynSacalingBufAddr = 0x%lx\n", pCfg->u32DynSacalingBufAddr);
        MVD_PRINT("u32DynSacalingBufSize = 0x%lx\n", pCfg->u32DynSacalingBufSize);
        MVD_PRINT("u32Miu1BaseAddr       = 0x%lx\n", pCfg->u32Miu1BaseAddr);
        MVD_PRINT("bFWMiuSel             = 0x%x\n", pCfg->bFWMiuSel);
        MVD_PRINT("bHWMiuSel             = 0x%x\n", pCfg->bHWMiuSel);
    }
}
#endif

//------------------------------------------------------------------------------
/// Configure MVD for memory and firmware and info.
/// Notice:
///     (1) u32FWAddr & u32DrvBufAddr should be on the same MIU
///     (2) u32FBAddr & u32BSAddr should be on the same MIU
/// @param -fwCfg  \b IN : pointer to firmware configuration
/// @param -memCfg \b IN : pointer to memory configuration
/// @param -InfoCfg \b IN : pointer to mvd info configuration
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetCfg(MS_U32 u32Id, MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg)
{
    MS_U8 u8DrvBuffMiuSel = 0;
    MS_U8  u8HalIdx = MVD_GetHalIdx(u32Id);

    MVD_FUNC_ENTRY();
    if (!fwCfg || !memCfg)
        return E_MVD_RET_INVALID_PARAM;

    MVD_DEBUGINFO(MVD_PRINT("%s u32Id=0x%x, u8HalIdx=0x%x\n", __FUNCTION__, u32Id, u8HalIdx));
    //1.1 Determine framebuffer mode, frame number, used size.
    if (TRUE != HAL_MVD_CheckFrmBuffSize(u8HalIdx, fwCfg, memCfg))
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    MVD_DEBUGINFO(MVD_PRINT("bNotReload=0x%x\n", fwCfg->bNotReload));
    //1.2 Save the FWCfg in hal
    HAL_MVD_SetFWCfg(u8HalIdx, fwCfg);

    MS_U32 u32StartOffset;
    MS_U8  u8MiuSel;

    _phy_to_miu_offset(u8MiuSel, u32StartOffset, memCfg->u32FWCodeAddr);

    memCfg->u8FWMiuSel = u8MiuSel;

    if(u8MiuSel >= 1)
    {
        memCfg->bFWMiuSel = 1;
    }
    else
    {
        memCfg->bFWMiuSel = 0;
    }

    _phy_to_miu_offset(u8MiuSel, u32StartOffset, memCfg->u32FBAddr);

    memCfg->u8HWMiuSel = u8MiuSel;
    if(u8MiuSel >= 1)
    {
        memCfg->bHWMiuSel = 1;
    }
    else
    {
        memCfg->bHWMiuSel = 0;
    }

    _phy_to_miu_offset(u8MiuSel, u32StartOffset, memCfg->u32DrvBufAddr);

    u8DrvBuffMiuSel = u8MiuSel;

    _miu_offset_to_phy(E_CHIP_MIU_1,0,memCfg->u32Miu1BaseAddr);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,memCfg->u32Miu2BaseAddr);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,memCfg->u32Miu3BaseAddr);

    MVD_DEBUGINFO(MVD_PRINT("MIUSEL:: FW=%x HW=%x, miu1base=0x%lx,miu2base=0x%lx,miu3base=0x%lx\n",
                  memCfg->u8FWMiuSel, memCfg->u8HWMiuSel, memCfg->u32Miu1BaseAddr,memCfg->u32Miu2BaseAddr,memCfg->u32Miu3BaseAddr));
    MVD_DEBUGINFO(MVD_PRINT("bDrvBuffMiuSel = %x\n", u8DrvBuffMiuSel));

    if(fwCfg->eSrcMode == E_MVD_SLQ_TBL_MODE)
    {
        MS_ASSERT(u8DrvBuffMiuSel == memCfg->u8FWMiuSel);
    }

    //addr of memCfg are physical addr, except field u32FWSrcVAddr
    if (memCfg->eFWSrcType == E_MVD_FW_SOURCE_DRAM)
    {
        memCfg->u32FWSrcVAddr = HAL_MVD_PA2NonCacheSeg(memCfg->u32FWBinAddr);
    }
    else
    {
        memCfg->u32FWSrcVAddr = NULL;
    }
    //2.2 Save the memory config in hal
    HAL_MVD_SetMEMCfg(u8HalIdx, memCfg);

    return E_MVD_RET_OK;
}

void MDrv_MVD_RegSetBase(MS_VIRT u32RegBaseAddr)
{
    HAL_MVD_RegSetBase(u32RegBaseAddr);
}

//------------------------------------------------------------------------------
/// MVD driver initialization
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Init(MS_U32 u32Id,MVD_CodecType eCodecType)
{
    MS_U8  u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8  u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32VpuSid = MVD_GetVpuStreamId(u32Id);
    MS_VIRT u32ShmAddr;

    MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Init:start u32Id=0x%x, sid=0x%x, halIdx=0x%x\n",
        u32Id, _MVD_GET_HAL_SID(u32Id), u8HalIdx));

    MVD_InitVar(u8DrvIdx,u8HalIdx);

    u32ShmAddr = HAL_VPU_EX_GetSHMAddr();

    HAL_VPU_EX_SetShareInfoAddr(u32Id, u32ShmAddr);

    if (!HAL_MVD_Init(u8HalIdx,eCodecType,u32VpuSid))
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Init:_MVD_Init failed\n"));
        return FALSE;
    }
    else
    {
        MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Init:_MVD_Init success\n"));
    }

    MVD_SetIsUsed(u8DrvIdx, TRUE);

    return TRUE;
}


//------------------------------------------------------------------------------
/// MVD driver exit
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Exit(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Exit:start\n"));
    MVD_DEBUGINFO(MVD_PRINT("%s u8DrvIdx=0x%x u8HalIdx=0x%x u32Id=0x%x\n",__FUNCTION__, u8DrvIdx, u8HalIdx, u32Id));

    //do nothing if driver is not initialized
    if (FALSE == MVD_GetIsUsed(u8DrvIdx))
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Exit: not-init yet\n"));
        goto _HW_TILE_DOWN;
        //HAL_MVD_PowerCtrl(DISABLE);
        //HAL_VPU_EX_PowerCtrl(DISABLE);
        return FALSE;
    }

    //stop and reset FW/HW
    if (HAL_MVD_Stop(u8HalIdx) != TRUE)
    {
        MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Exit: stop fail\n"));
    }

#ifdef MVD_ENABLE_ISR
    //dettach isr
    if (pMVDDrvContext->_eEventFlag[u8DrvIdx])
    {
        if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Id, E_MVD_EVENT_DISABLE_ALL, (MVD_InterruptCb)NULL)) //FIXME!
        {
            MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Exit: dettach isr fail\n"));
        }
    }
#endif

    if (!HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MVD_GetVpuStreamId(u32Id)))
    {
        MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Exit: fail to delete task\n"));
    }

_HW_TILE_DOWN:
    if (HAL_MVD_Exit(u8HalIdx) != TRUE)
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Exit: fail\n"));
        //return FALSE;
    }

    //reset internal variables.
    MVD_InitVar(u8DrvIdx,u8HalIdx);

    MVD_DEBUGINFO(MVD_PRINT("%s i=0x%x u32Id=0x%x\n", __FUNCTION__, u8DrvIdx, u32Id));
    MVD_SetIsUsed(u8DrvIdx, FALSE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// Reset MVD
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_Rst(MS_U32 u32Id,MVD_CodecType eCodecType)
{
	  MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_FUNC_ENTRY();
    if (TRUE == HAL_MVD_SoftRstHW())
    {
        //notice: T2 MVD3 does not support soft-reset HW.
        //Thus, MDrv_MVD_Rst()==MDrv_MVD_Init() for T2.
        return E_MVD_RET_OK;
    }
    if (!HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MVD_GetVpuStreamId(u32Id)))
    {
        MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_Rst: fail to delete task\n"));
    }
    //For T3&Euclid MVD4, do re-init only when SoftRst does not work.
    if (FALSE == MDrv_MVD_Init(u32Id,eCodecType))
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Rst:_MVD_Init failed\n"));
        return E_MVD_RET_FAIL;
    }
    else
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_Rst:_MVD_Init success\n"));
    }

    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Issue Play command to MVD
//------------------------------------------------------------------------------
void MDrv_MVD_Play(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (TRUE != HAL_MVD_Play(u8HalIdx))
    {
        MVD_DEBUGERROR(MVD_ERR("%s fail\n", __FUNCTION__));
    }
    return;
}

//------------------------------------------------------------------------------
/// Set frame buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void MDrv_MVD_SetFrameBuffAddr(MS_U32 u32Id, MS_VIRT u32addr)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    HAL_MVD_SetFrameBuffAddr(u8HalIdx, u32addr, HAL_MVD_GetFBMode(u8HalIdx));
}

//------------------------------------------------------------------------------
/// Set codec type.
/// @param -u8CodecType \b IN : 0: mpeg4, 1: mpeg4 with short_video_header, 2: DivX311
/// @param -u8BSProviderMode \b IN : TS live stream, file, SLQ, SLQ table link and TS file mode.
//------------------------------------------------------------------------------
void MDrv_MVD_SetCodecInfo(MS_U32 u32Id, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    //MVD_PRINT("u8CodecType=0x%x\n", u8CodecType);
    //MVD_PRINT("u8BSProviderMode=0x%x\n", u8BSProviderMode);
    if (HAL_MVD_SetCodecInfo(u8HalIdx, u8CodecType, u8BSProviderMode, bDisablePESParsing) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("%s fail!!\r\n", __FUNCTION__));
        return;
    }

    return;
}

//------------------------------------------------------------------------------
/// Set DivX patch.
/// @param -u8MvAdjust \b IN : chroma adjustment
/// @param -u8IdctSel \b IN : idct algorithm selection
//------------------------------------------------------------------------------
void MDrv_MVD_SetDivXCfg(MS_U32 u32Id, MS_U8 u8MvAdjust, MS_U8 u8IdctSel)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetDivXCfg(u8HalIdx, u8MvAdjust, u8IdctSel);
    return;
}


//------------------------------------------------------------------------------
/// Get MVD decoded picture counter
/// @return -decoded picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPicCounter(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetPicCounter(u8HalIdx);
}


//------------------------------------------------------------------------------
/// Get MVD skipped picture counter
/// @return -skipped picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetSkipPicCounter(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetSkipPicCounter(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD picture type
/// @return - picture type
//------------------------------------------------------------------------------
MVD_PicType MDrv_MVD_GetPicType(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetPicType(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD bit rate (bits/sec)
/// @return -bit rate
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetBitsRate(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetBitsRate(u8HalIdx);
}


//------------------------------------------------------------------------------
/// Get video_range flag.
/// Supported after MVD FW release v00720390
/// Color team need this information for better color quality.
/// When video_range = 0  gives a range of Y from 16 to 235 and Cb , Cr from 16 to 240
/// When video_range = 1  gives a range of Y from  0 to 255 and Cb , Cr from  0 to 255
/// @return -video_range
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetVideoRange(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetVideoRange(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get LowDelay flag
/// @return -True/False
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetLowDelayFlag(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetLowDelayFlag(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MPEG 3:2 pull down flag
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIs32PullDown(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIs32PullDown(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get if Dynamic Scaling is enabled (reported by firmware)
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsDynScalingEnabled(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8HalIdx);
    MS_BOOL bEnDynScale = FALSE;

    bEnDynScale = (MS_BOOL)HAL_MVD_GetIsDynScalingEnabled(u8HalIdx);
    if ((FALSE == pstMemCfg->bEnableDynScale) && (bEnDynScale))
    {
        MVD_DEBUGERROR(MVD_ERR("bEnDynScale err: %x\n", bEnDynScale));
    }

    //MVD_PRINT("%s bEnDS: %x, %x\n", __FUNCTION__, bEnDynScale, pstMemCfg->bEnableDynScale);
    return bEnDynScale;
}

//------------------------------------------------------------------------------
/// Get the difference of PTS and STC
/// @return -(PTS-STC)
//------------------------------------------------------------------------------
MS_S32 MDrv_MVD_GetPtsStcDiff(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetPtsStcDiff(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD SLQ read pointer
/// @return -SLQ read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetSLQReadPtr(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetSLQReadPtr(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD SLQ write pointer
/// @return -SLQ write pointer
//------------------------------------------------------------------------------
void MDrv_MVD_SetSLQWritePtr(MS_U32 u32Id, MS_BOOL bCheckData)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetSLQWritePtr(u8HalIdx, bCheckData);
    return;
}

//------------------------------------------------------------------------------
/// Get MVD decoded frame index.  For debug.
/// @return -frame index of the decoded frame
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetDecodedFrameIdx(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetDecodedFrameIdx(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD VLD error count
/// @return -VLD error count
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetVldErrCount(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetVldErrCount(u8HalIdx);
}


//------------------------------------------------------------------------------
/// Get MVD error info.
/// This function can be used to diagnosis when the 1st DispReady doesn't occur.
///
/// @param - errorCode \b OUT : error code
/// @param - errorStatus \b OUT : error status
//------------------------------------------------------------------------------
void MDrv_MVD_GetErrInfo(MS_U32 u32Id, MVD_ErrCode *errCode, MVD_ErrStatus *errStatus)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_GetErrInfo(u8HalIdx, errCode, errStatus);
    return;
}

//------------------------------------------------------------------------------
/// Get valid MVD stream detected flag
/// For MPEG2/4,
///     1. width<16(1 macroblock), height<16, width>1920, or height>1080
///     2. next start code not found
/// the stream would be considered as invalid.
/// For VC1/RCV, not impelment, default: valid
/// @return -decoded flag
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetValidStreamFlag(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetValidStreamFlag(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get video frame information from MVD
/// (SourceW * ParH) : (SourceH * ParW) = DarW : DarH
/// E.g. Source PAL 704x576, Par(Pixel aspect ratio)=12/11,
///      Dar(Display AR) = (704x12):(576x11) = 4:3
/// @param -pinfo \b IN : pointer to video frame information
//------------------------------------------------------------------------------
void MDrv_MVD_GetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_GetFrameInfo(u8HalIdx, pinfo);
}

//------------------------------------------------------------------------------
/// Set video frame information from MVD
/// @param -pinfo \b IN : pointer to video frame information
//------------------------------------------------------------------------------
void MDrv_MVD_SetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo )
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetFrameInfo(u8HalIdx, pinfo);
    return;
}

//------------------------------------------------------------------------------
/// Reset for I-frame decoding
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_RstIFrameDec(MS_U32 u32Id,MVD_CodecType eCodecType)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (HAL_MVD_Stop(u8HalIdx) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("%s fail!!\n", __FUNCTION__));
    }

    MDrv_MVD_Rst(u32Id,eCodecType);
    return;
}

//------------------------------------------------------------------------------
/// Decode I-frame
/// @param -u32FrameBufAddr \b IN : start address of frame buffer
/// @param -u32StreamBufAddr \b IN : start address of stream buffer
/// @return -return decode I-frame success or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DecodeIFrame(MS_U32 u32Id, MS_PHY u32FrameBufAddr, MS_PHY u32StreamBufAddr, MS_PHY u32StreamBufEndAddr )
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_DEBUGINFO(MVD_PRINT("%s input  FBAdd=0x%lx streamStart=0x%lx streamEnd=0x%lx\n",
            __FUNCTION__, u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr));
    u32FrameBufAddr     = HAL_MVD_GetMemOffset(u32FrameBufAddr);
    u32StreamBufAddr    = HAL_MVD_GetMemOffset(u32StreamBufAddr);
    u32StreamBufEndAddr = HAL_MVD_GetMemOffset(u32StreamBufEndAddr);
    MVD_DEBUGINFO(MVD_PRINT("%s offset FBAdd=0x%lx streamStart=0x%lx streamEnd=0x%lx\n",
            __FUNCTION__, u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr));

    bRet = HAL_MVD_DecodeIFrame(u8HalIdx, u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr);
    return bRet;
}

//------------------------------------------------------------------------------
/// Query if MVD is decoding frame
/// @return -MVD is decoding frame or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsIFrameDecoding(MS_U32 u32Id)
{
    MS_BOOL bDecodeIFrame = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg)
    {
        bDecodeIFrame = pCtrlCfg->bDecodeIFrame;
    }
    return bDecodeIFrame;
}

//------------------------------------------------------------------------------
/// Get MVD active format
/// @return -active format
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetActiveFormat(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    //set default as invalid AFD that is ignored by AP.
    //static MS_U8 u8Afd[MVD_MAX_STREAMS] = { 0xff, 0xff};
    MS_U8  u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
#if (MVD_TURBO_INIT)
    //static MS_U32 u32FrmCnt[MVD_MAX_STREAMS] = { 0, 0};

    if (pMVDDrvContext->u32FrmCnt[u8DrvIdx] == HAL_MVD_GetPicCounter(u8HalIdx))
    {
        return pMVDDrvContext->u8Afd[u8DrvIdx];
    }
    pMVDDrvContext->u32FrmCnt[u8DrvIdx] = HAL_MVD_GetPicCounter(u8HalIdx);
#endif
    pMVDDrvContext->u8Afd[u8DrvIdx] = HAL_MVD_GetActiveFormat(u8HalIdx);
    return pMVDDrvContext->u8Afd[u8DrvIdx];
}

//------------------------------------------------------------------------------
/// Enable or disable AV synchronization.
/// Delay u32Delay ms if AVSync is enabled.
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSync(MS_U32 u32Id, MS_BOOL bEnable, MS_U32 u32Delay)
{
    //MVD_PRINT("%s bEnable=%d u32Delay=%ld\n",__FUNCTION__,bEnable,u32Delay);
    MVD_FUNC_ENTRY();
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (TRUE != HAL_MVD_EnableAVSync(u8HalIdx, bEnable))
    {
        MVD_DEBUGERROR(MVD_ERR("%s: fail to set avsync on\n", __FUNCTION__));
        return;
    }
    //bAVSyncOn = bEnable;    //for IsAVSyncOn

    if (bEnable && (u32Delay != 0))
    {
        if (TRUE != HAL_MVD_SetAVSyncDelay(u8HalIdx, u32Delay))
        {
            MVD_DEBUGERROR(MVD_ERR("%s: fail to set avsync delay\n", __FUNCTION__));
            return;
        }
    }

    pMVDDrvContext->stSyncCfg[u8DrvIdx].bEnable = bEnable;
    pMVDDrvContext->stSyncCfg[u8DrvIdx].u32Delay = u32Delay;
    return;
}

//------------------------------------------------------------------------------
/// Set the maximum repeat times for one frame when av is not sync.
/// E.g. Arg5=0x01 mean that frame will be repeated once if av is not sync.
///      Arg5=0xff mean that frame will be always repeated when av is not sync.
/// And customer can re-define the avsync skip behavior,
/// i.e., Arg4 = 1 for skip-b-frame only and 0 for skip display(default)
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSyncThreshold(MS_U32 u32Id, MS_U32 u32Th)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetAVSyncThreshold(u8HalIdx, u32Th);
    return;
}

//------------------------------------------------------------------------------
/// Set the threshold for VC1 avsync (live mode & file mode). Unit: 90K
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSyncFreerunThreshold(MS_U32 u32Id, MS_U32 u32Th)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetAVSyncFreerunThreshold(u8HalIdx, u32Th);
    return;
}

//------------------------------------------------------------------------------
/// Get MVD AVSync delay (ms)
/// @return -avsync delay
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetAVSyncDelay(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    return (pMVDDrvContext->stSyncCfg[u8DrvIdx].u32Delay);
}

//------------------------------------------------------------------------------
/// Get if AVSync is turned on
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsAVSyncOn(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIsAVSyncOn(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get if firmware is repeating frame for AVSync.
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsSyncRep(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIsSyncRep(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get if firmware is skipping frame for AVSync.
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsSyncSkip(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIsSyncSkip(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Change PTS threshold for doing AVSync.
/// Scenario: When disable black screen earlier than normal case, use this function
/// to avoid frame skip too fast or video lag.
/// @param -bEnable \b IN : enable this configuration
/// @param -u16PTS \b IN : PTS threshold (unit: 90k counter, i.e. x/90 ms, max 728ms)
/// @return -TRUE for success or FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ChangeAVsync(MS_U32 u32Id, MS_BOOL bEnable, MS_U16 u16PTS)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (HAL_MVD_ChangeAVsync(u8HalIdx, bEnable, u16PTS) == TRUE)
    {
        pMVDDrvContext->stSyncCfg[u8DrvIdx].u16Tolerance = u16PTS;
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// Get address of the first I-frame.
/// @return -address of the first I-frame
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetIsIPicFound(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIsIPicFound(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get sync status to know whether sync is complete or not
/// @return - 1        : sync complete
////        - otherwise: sync not complete
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetSyncStatus(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetIsSyncReach(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Report if decoder is freerun or not
/// @return - TRUE  : freerun
////        - FALSE : not freerun
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsFreerun(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return (HAL_MVD_GetAVSyncStatus(u8HalIdx) == 0);
}

//------------------------------------------------------------------------------
/// Get PTS (Presentation Time Stamp)
/// @return -PTS (unit: ms)
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPTS(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetPTS(u8HalIdx);
}


//------------------------------------------------------------------------------
/// Get 33 bits PTS (Presentation Time Stamp)
/// @return -PTS (unit: ms)
//------------------------------------------------------------------------------
MS_U64 MDrv_MVD_GetU64PTS(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetU64PTS(u8HalIdx);
}



//------------------------------------------------------------------------------
/// Get Next PTS (Presentation Time Stamp)
/// @return -Next PTS (unit: ms)
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetNextPTS(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetNextPTS(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get Chroma format (MDrv_MVD_GetChromaFormat)
/// @return -Chroma format ()
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetChromaFormat(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetChromaFormat(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Query if MVD is ready to display
/// @return -MS_U8  0: not ready, !0: ready.
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetDispRdy(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_BOOL bDispRdy = HAL_MVD_GetDispRdy(u8HalIdx);
    MS_BOOL bIFound = HAL_MVD_GetIsIPicFound(u8HalIdx);

    return (bDispRdy || bIFound);
}

//------------------------------------------------------------------------------
/// Query if the first frame is showed after play function is called.
/// Whenever the first display frame is displayed, f/w will set this flag to 1.
/// @return VDEC_Result
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Is1stFrmRdy(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_Is1stFrmRdy(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get picture count of current GOP (Group of Picture)
/// @return -picture count of GOP
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetGOPCount(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetGOPCount(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Enable or disable dropping error frames
/// @return -command is set successfully or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DropErrorFrame(MS_U32 u32Id, MS_BOOL bDrop)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (HAL_MVD_DropErrorFrame(u8HalIdx, bDrop) == FALSE)
    {
        MVD_DEBUGERROR( MVD_ERR( "%s fail!!\n", __FUNCTION__));
        return FALSE;
    }
    pstCtrlCfg->bDropErrFrm = bDrop;
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get byte count of parser.
/// @return -byte count of parser
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetParserByteCnt(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetParserByteCnt(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get the decode status.
/// @return -the decode status
//------------------------------------------------------------------------------
MVD_DecStat MDrv_MVD_GetDecodeStatus(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetDecodeStatus(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get the last command to firmware.
/// @return -the last command
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetLastCmd(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetLastCmd(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Skip data in ES buffer.  It is used for flushing ES buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipData(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SkipData(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Skip to I frame.
/// Used for MediaCodec when input data is not continuous or in the beginning of
/// streams.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipToIFrame(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SkipToIFrame(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Display control for decoding order, enabling drop error frame, dropping
/// display and setting FRC (frame rate conversion) mode.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DispCtrl(MS_U32 u32Id, MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_DispCtrl(u8HalIdx, bDecOrder, bDropErr, bDropDisp, eFrcMode);
}

//------------------------------------------------------------------------------
/// Enable/Disable to repeat the last field when repeat for avsync or display pause.
/// E.g. Top field first: T-B-B-B-...; Bottom field first: B-T-T-T-...
/// Usage scenario: scaler framebufferless mode to avoid frame flicker
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DispRepeatField(MS_U32 u32Id, MS_BOOL bEnable)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_DispRepeatField(u8HalIdx, bEnable);
}

//------------------------------------------------------------------------------
/// Get color format.
/// @return - value (0~8)
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetColorFormat(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetColorFormat(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get matrix coefficients in 13818-2 sequence_display_extension().
/// @return - value (0~255)
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetMatrixCoef(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetMatrixCoef(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Issue PauseDisplay command.
//------------------------------------------------------------------------------
void MDrv_MVD_Pause(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_PauseDisp(u8HalIdx);
    return;
}

//------------------------------------------------------------------------------
/// Issue Resume command.
//------------------------------------------------------------------------------
void MDrv_MVD_Resume(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (HAL_MVD_Resume(u8HalIdx) == FALSE)
    {
        MVD_DEBUGERROR( MVD_ERR( "Command: HAL_MVD_Resume fail!!\r\n" ) );
    }

    return;
}

//-----------------------------------------------------------------------------
/// Is MVD step decode done after step decode command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStepDecodeDone(MS_U32 u32Id)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    MVD_FUNC_ENTRY();
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg->bStepDecode)
    {
        if (HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_SINGLE_STEP))
        {
            MVD_PRINT( "MVD_HANDSHAKE_SINGLE_STEP -------: 0x%x \n", HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_SINGLE_STEP));
            pCtrlCfg->bStepDecode = FALSE;
            bRet = TRUE;
        }
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// Issue StepPlay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_StepDecode(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_ResetHandShake(u8HalIdx, MVD_HANDSHAKE_SINGLE_STEP);
    if (HAL_MVD_StepDecode(u8HalIdx) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("Command: HAL_MVD_StepDecode fail!!\r\n"));
        return FALSE;
    }

    if (HAL_MVD_Resume(u8HalIdx) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("Command: HAL_MVD_Resume fail!!\r\n"));
        return FALSE;
    }

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg)
    {
        pCtrlCfg->bStepDecode = TRUE;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/// Is MVD step display done after step display command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStepDispDone(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_BOOL bRet = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);

    if (FALSE == pCtrlCfg->bStepDisp)
    {
        MVD_DEBUGERROR(MVD_ERR("%s err: hasn't issue StepDisp\n", __FUNCTION__));
        return FALSE;
    }

    bRet = HAL_MVD_IsStepDispDone(u8HalIdx);
    if (bRet)
    {
        pCtrlCfg->bStepDisp = FALSE;
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// Issue StepDisplay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_StepDisp(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_DEFAULT, 1);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (HAL_MVD_StepDisp(u8HalIdx) == FALSE)
    {
        MVD_DEBUGERROR( MVD_ERR( "%s fail!!\n", __FUNCTION__ ) );
        return FALSE;
    }

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg)
    {
        pCtrlCfg->bStepDisp = TRUE;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/// Is MVD step display done after step display command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStep2PtsDone(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_IsStep2PtsDone(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Step to the specific PTS (u32Pts ms)
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SeekToPTS(MS_U32 u32Id, MS_U32 u32Pts)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);

    MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_DEFAULT, 1);
    pCtrlCfg->bStep2Pts = HAL_MVD_SeekToPTS(u8HalIdx, _MS_TO_90K(u32Pts));
    return (pCtrlCfg->bStep2Pts);
}

//------------------------------------------------------------------------------
/// Skip to the specific PTS (u32Pts ms)
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipToPTS(MS_U32 u32Id, MS_U32 u32Pts)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);

    pCtrlCfg->bSkip2Pts = HAL_MVD_SkipToPTS(u8HalIdx, _MS_TO_90K(u32Pts));
    return (pCtrlCfg->bSkip2Pts);
}

//------------------------------------------------------------------------------
/// Issue TrickPlay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_TrickPlay(MS_U32 u32Id, MVD_TrickDec trickDec, MS_U8 u8DispDuration)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    //eTrickMode = trickDec;    //for MDrv_MVD_GetTrickMode
    if (HAL_MVD_TrickPlay(u8HalIdx, trickDec, u8DispDuration) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("HAL_MVD_TrickPlay fail!!\r\n"));
        return FALSE;
    }
    MDrv_MVD_SetAVSync(u32Id, FALSE, 0);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get the trick mode which has been set.
/// @return MVD_TrickDec
//------------------------------------------------------------------------------
MVD_TrickDec MDrv_MVD_GetTrickMode(MS_U32 u32Id)
{
    MVD_TrickDec eTrickMode = E_MVD_TRICK_DEC_UNKNOWN;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg)
    {
        eTrickMode = pCtrlCfg->eTrickMode;
    }

    return eTrickMode;
}

//------------------------------------------------------------------------------
/// Flush display buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FlushDisplayBuf(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_FlushDisplayBuf(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get capabilities of MPEG Video Decoder.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetCaps(MVD_Caps* pCaps)
{
    MS_U8 caps = HAL_MVD_GetCaps();
    if (!pCaps)
        return FALSE;

    pCaps->bMPEG2 = ((caps & MVD_SUPPORT_MPEG2) == MVD_SUPPORT_MPEG2);
    pCaps->bMPEG4 = ((caps & MVD_SUPPORT_MPEG4) == MVD_SUPPORT_MPEG4);
    pCaps->bVC1   = ((caps & MVD_SUPPORT_VC1) == MVD_SUPPORT_VC1);

    //MVD_PRINT("MP2=%d, MP4=%d, VC1=%d\n", pCaps->bMPEG2, pCaps->bMPEG4, pCaps->bVC1);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get if MVD decoder is playing.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsPlaying(MS_U32 u32Id)
{
    MS_BOOL bIsPlaying = FALSE;
    bIsPlaying = ( MDrv_MVD_GetDecodeStatus(u32Id) == E_MVD_STAT_WAIT_DECODEDONE );

    return bIsPlaying;
}

//------------------------------------------------------------------------------
/// Get if MVD decoder is in idle state.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsIdle(MS_U32 u32Id)
{
    return (MDrv_MVD_GetDecodeStatus(u32Id) == E_MVD_STAT_IDLE);
}

//------------------------------------------------------------------------------
/// Enable/Disable error concealment function
/// @param -bDisable \b IN : enable/disable this function
/// @return -return E_MVD_Result success/fail to enable/disable
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DisableErrConceal(MS_U32 u32Id, MS_BOOL bDisable)
{
    return E_MVD_RET_FAIL;
}

//------------------------------------------------------------------------------
/// Push queue.
/// @param -u32StAddr \b IN : the start address of the queue
/// @param -u32Size \b IN : the data size to be pushed
/// @param -u32TimeStamp \b IN : the corresponding PTS
/// @return -return E_MVD_Result success/fail to push queue
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_PushQueue(MS_U32 u32Id, MVD_PacketInfo* pInfo)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    //check input parameters
    if (pInfo == NULL)
    {
        MVD_DEBUGERROR(MVD_ERR("PushQueue NULL pInfo\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

    return HAL_MVD_PushQueue(u8HalIdx, pInfo);
}

//------------------------------------------------------------------------------
/// Flush queue: flush ES buffer, reset SLQ TABLE, flush decoded frame, and
///              keep the display frame.
/// @return -return E_MVD_Result success/fail to flush queue
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_FlushQueue(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_FlushQueue(u8HalIdx);
}


//------------------------------------------------------------------------------
/// Get the queue vacancy (unit in slq entry).
/// @param -pQueueVacancy \b IN : pointer to the queue vacancy
/// @return -return E_MVD_Result success/fail to get the queue vacancy
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetQueueVacancy(MS_U32 u32Id, MS_BOOL bCached)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetQueueVacancy(u8HalIdx, bCached);
}

//------------------------------------------------------------------------------
/// Get read pointer in ElementaryStream buffer
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetESReadPtr(MS_U32 u32Id)
{
    MS_U32 u32ESR = 0;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8HalIdx);

    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        u32ESR = MVD_GetSlqTblESReadPtr(u8HalIdx);
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode || E_MVD_TS_MODE == curSrcMode)
    {
        u32ESR = HAL_MVD_GetTsFileESReadPtr(u8HalIdx);
    }
    return u32ESR;
}


//------------------------------------------------------------------------------
/// Get write pointer in ElementaryStream buffer
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetESWritePtr(MS_U32 u32Id)
{
    MS_U32 u32ESW = 0;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8HalIdx);

    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        u32ESW = MVD_GetSlqTblESWritePtr(u8HalIdx);
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode || E_MVD_TS_MODE == curSrcMode)
    {
        u32ESW = HAL_MVD_GetTsFileESWritePtr(u8HalIdx);
    }
    return u32ESW;
}


//------------------------------------------------------------------------------
/// Enable/Disable driver to show the last frame
/// @param -bEnable \b IN : enable/disable this function
/// @return -return E_MVD_Result success/fail to enable/disable
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_EnableLastFrameShow(MS_U32 u32Id, MS_BOOL bEnable)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (HAL_MVD_EnableLastFrameShow(u8HalIdx, bEnable) == FALSE)
    {
        MVD_DEBUGERROR(MVD_ERR("%s fail!!\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }
    pCtrlCfg->bEnableLastFrmShow = bEnable;
    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Get if MVD decoder finish display.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_IsDispFinish(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    if (pCtrlCfg->bEnableLastFrmShow != TRUE)
    {
        MVD_DEBUGINFO(MVD_PRINT("%s: bEnableLastFrmShow!=TRUE\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }
    return HAL_MVD_IsDispFinish(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Set speed.
/// @param -eSpeedType \b IN : specify the speed
/// @param -u8Multiple \b IN :
/// @return -return E_MVD_Result success/fail to set the speed
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetSpeed(MS_U32 u32Id, MVD_SpeedType eSpeedType, MS_U8 u8Multiple)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    MVD_AVSyncCfg* pstNFPSyncCfg = &(pMVDDrvContext->stNFPSyncCfg[u8DrvIdx]);

    if ((u8Multiple > 32) || (eSpeedType==E_MVD_SPEED_TYPE_UNKNOWN))
    {
        MVD_DEBUGINFO(MVD_PRINT("%s: invalid para!\n", __FUNCTION__));
        return E_MVD_RET_INVALID_PARAM;
    }

    if (pstCtrlCfg->ePreSpeedType == E_MVD_SPEED_DEFAULT)
    {   //save avsync config when normal play for restoring it later
        memcpy(pstNFPSyncCfg, &(pMVDDrvContext->stSyncCfg[u8DrvIdx]), sizeof(MVD_AVSyncCfg));
        MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_SetSpeed save avsync: bEnable=0x%x u32Delay=0x%x u16Tolerance=0x%x\n",
                  pstNFPSyncCfg->bEnable, pstNFPSyncCfg->u32Delay, pstNFPSyncCfg->u16Tolerance));
    }

    if (E_MVD_SPEED_DEFAULT != eSpeedType) //fast or slow forward
    {
        //disable avsyn when fast/slow forward
        MDrv_MVD_SetAVSync(u32Id, FALSE, 0);
    }

    if (HAL_MVD_SetSpeed(u8HalIdx, eSpeedType, u8Multiple) == TRUE)
    {
        if (E_MVD_SPEED_DEFAULT == eSpeedType) //Normal Play
        {
            if (pstCtrlCfg->ePreSpeedType != E_MVD_SPEED_DEFAULT)
            {
                //set AVSync again for firmware doesn't remember AVSync settings after FF
                MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_SetSpeed reset avsync: bEnable=0x%x u32Delay=0x%x u16Tolerance=0x%x\n",
                              pstNFPSyncCfg->bEnable, pstNFPSyncCfg->u32Delay, pstNFPSyncCfg->u16Tolerance));

                MDrv_MVD_SetAVSync(u32Id, pstNFPSyncCfg->bEnable, pstNFPSyncCfg->u32Delay);
                if ((pstNFPSyncCfg->u16Tolerance!=0) &&
                    (FALSE == MDrv_MVD_ChangeAVsync(u32Id, pstNFPSyncCfg->bEnable, pstNFPSyncCfg->u16Tolerance)))
                {
                    return E_MVD_RET_FAIL;
                }
            }
        }

        pstCtrlCfg->ePreSpeedType = eSpeedType;
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
   }
}

//------------------------------------------------------------------------------
/// Reset Presentation Time Stamp according to u32PtsBase
/// @param -u32PtsBase \b IN : the PTS base specified by user
/// @return -return E_MVD_Result success/fail to reset PTS base
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32PtsBaseIn90K = 0;
    u32PtsBaseIn90K = _MS_TO_90K(u32PtsBase);
    HAL_MVD_SetPTSBase(u8HalIdx, u32PtsBaseIn90K);
    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Get if sequence change (width/height/framerate/interlace) occurs.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsSeqChg(MS_U32 u32Id)
{
    MVD_FrameInfo stFrmInfo;
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8HalIdx);
    MVD_FrameInfo* pPreInfo = &(pMVDDrvContext->stPreFrmInfo[u8DrvIdx]);
    MS_BOOL bRet = FALSE;
    MS_BOOL bResolutionChange = FALSE;
    memset(&stFrmInfo, 0, sizeof(stFrmInfo));

    if (
#ifdef MVD_ENABLE_ISR
        (MDrv_MVD_GetIsrEvent(u32Id) & E_MVD_EVENT_SEQ_FOUND) ||
#endif
        (MDrv_MVD_GetDispRdy(u32Id))
        )
    {
        MDrv_MVD_GetFrameInfo(u32Id, &stFrmInfo);

        if(pMVDDrvContext->bSelf_SeqChange[u8DrvIdx]==FALSE)
        {
            if (FALSE == pstMemCfg->bEnableDynScale)
            {
                if((pPreInfo->u16HorSize   != stFrmInfo.u16HorSize) ||
                   (pPreInfo->u16VerSize   != stFrmInfo.u16VerSize) ||
                   (pPreInfo->u32FrameRate != stFrmInfo.u32FrameRate) ||
                   (pPreInfo->u8Interlace  != stFrmInfo.u8Interlace))
                {
#if 0
                    (MVD_PRINT( "MDrv_MVD_IsSeqChg::Previous\n" ));
                    (MVD_PRINT( "H=%u\n", pPreInfo->u16HorSize ));
                    (MVD_PRINT( "V=%u\n", pPreInfo->u16VerSize ));
                    (MVD_PRINT( "F=%lu\n", pPreInfo->u32FrameRate ));
                    (MVD_PRINT( "A=%u\n", pPreInfo->u8AspectRate ));
                    (MVD_PRINT( "I=%u\n", pPreInfo->u8Interlace ));
#endif
#if 0
                    (MVD_PRINT( "MDrv_MVD_IsSeqChg\n" ));
                    (MVD_PRINT( "H=%u\n", stFrmInfo.u16HorSize ));
                    (MVD_PRINT( "V=%u\n", stFrmInfo.u16VerSize ));
                    (MVD_PRINT( "F=%lu\n", stFrmInfo.u32FrameRate ));
                    (MVD_PRINT( "A=%u\n", stFrmInfo.u8AspectRate ));
                    (MVD_PRINT( "I=%u\n", stFrmInfo.u8Interlace ));
#endif

                    //memcpy(&(pMVDDrvContext->stPreFrmInfo), &stFrmInfo, sizeof(MVD_FrameInfo));
                    pPreInfo->u16HorSize   = stFrmInfo.u16HorSize;
                    pPreInfo->u16VerSize   = stFrmInfo.u16VerSize;
                    pPreInfo->u8AspectRate = stFrmInfo.u8AspectRate;
                    pPreInfo->u32FrameRate = stFrmInfo.u32FrameRate;
                    pPreInfo->u8Interlace  = stFrmInfo.u8Interlace;
#if 0
                    (MVD_PRINT( "MDrv_MVD_IsSeqChg===>\n" ));
                    (MVD_PRINT( "H=%u\n", stFrmInfo.u16HorSize ));
                    (MVD_PRINT( "V=%u\n", stFrmInfo.u16VerSize ));
                    (MVD_PRINT( "F=%u\n", stFrmInfo.u32FrameRate ));
                    (MVD_PRINT( "A=%u\n", stFrmInfo.u8AspectRate ));
                    (MVD_PRINT( "I=%u\n", stFrmInfo.u8Interlace ));
#endif
                    bRet = TRUE;
                }
            }
            else
            {
                //When dynamic scaling is enabled, f/w will handle the width/height change.
                //Therefore, only report SeqChg if framerate or interlace flag changes.
                if((pPreInfo->u32FrameRate != stFrmInfo.u32FrameRate) ||
                   (pPreInfo->u8Interlace  != stFrmInfo.u8Interlace))
                {
                    pPreInfo->u8AspectRate = stFrmInfo.u8AspectRate;
                    pPreInfo->u32FrameRate = stFrmInfo.u32FrameRate;
                    pPreInfo->u8Interlace  = stFrmInfo.u8Interlace;
                    bRet = TRUE;
                }
            }
        }
        else
        {

            if((pPreInfo->u16HorSize   != stFrmInfo.u16HorSize) ||
               (pPreInfo->u16VerSize   != stFrmInfo.u16VerSize) ||
               (pPreInfo->u32FrameRate != stFrmInfo.u32FrameRate) ||
               (pPreInfo->u8Interlace  != stFrmInfo.u8Interlace) )
            {
                pPreInfo->u16HorSize   = stFrmInfo.u16HorSize;
                pPreInfo->u16VerSize   = stFrmInfo.u16VerSize;
                pPreInfo->u8AspectRate = stFrmInfo.u8AspectRate;
                pPreInfo->u32FrameRate = stFrmInfo.u32FrameRate;
                pPreInfo->u8Interlace  = stFrmInfo.u8Interlace;
                bRet = TRUE;
            }
        }
    }

    bResolutionChange = bRet;

    if(HAL_MVD_GetSuspendDynamicScale(u8HalIdx) == TRUE)
    {
        if(pMVDDrvContext->bSuspendDS_Ftime[u8DrvIdx] ==TRUE) //happen in first time suspend ds
        {
            pMVDDrvContext->bSuspendDS_Ftime[u8DrvIdx] = FALSE;
            if(pMVDDrvContext->bSelf_SeqChange[u8DrvIdx]==TRUE)
            {
               bRet = bResolutionChange;
            }
            else
            {
               bRet = TRUE;
            }
        }
        else
        {
            if(bResolutionChange == TRUE) //happen in suspend ds and resolution change
            {
                bRet = TRUE;
            }
            else
            {
                bRet = FALSE;
            }
        }
    }
    else //normal case
    {
        if(pMVDDrvContext->bSuspendDS_Ftime[u8DrvIdx] == FALSE)//happen in close suspend ds,it will reset bSuspendDS_Ftime flag
        {
            pMVDDrvContext->bSuspendDS_Ftime[u8DrvIdx] = TRUE;
            bRet = TRUE;
        }
        else
        {
            bRet = bResolutionChange;
        }
    }

    if (TRUE == bRet)
    {
        //Update bFrmRateSupported when IsSeqChg
        pMVDDrvContext->bFrmRateSupported[u8DrvIdx] = HAL_MVD_GetFrmRateIsSupported(stFrmInfo.u16HorSize, stFrmInfo.u16VerSize, stFrmInfo.u32FrameRate);
        MVD_DEBUGINFO(MVD_PRINT("%s bFrmRateSupported[0x%x]=%x\n", __FUNCTION__, u8DrvIdx, pMVDDrvContext->bFrmRateSupported[u8DrvIdx]));
    }

    return bRet;
}

//------------------------------------------------------------------------------
/// Set Debug Data which will be queried by MDrv_MVD_DbgGetData()
/// @param -u32Addr \b IN : address of debug data
/// @param -u32Data \b IN : the debug data
/// @return -return E_MVD_Result success/fail to set debug data
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DbgSetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Data)
{
    return E_MVD_RET_FAIL;
}

//------------------------------------------------------------------------------
/// Get Debug Data
/// @param -u32Addr \b IN : address of debug data
/// @param -u32Data \b IN : pointer to the debug data
/// @return -return E_MVD_Result success/fail to get debug data
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DbgGetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32* u32Data)
{
    if (!u32Data)
    {
        return E_MVD_RET_INVALID_PARAM;
    }
    return HAL_MVD_DbgGetData(u32Addr, u32Data);
}


//------------------------------------------------------------------------------
/// Set MVD firmware command
/// @param -u8cmd \b IN : MVD command
/// @param -pstCmdArg \b IN : pointer to command argument
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg )
{
    return HAL_MVD_MVDCommand(u8cmd, pstCmdArg);
}


#ifdef MVD_ENABLE_ISR
///Notice: This function only works when being called by fnHandler, which was
///registered by AP using MDrv_MVD_SetIsrEvent()
MS_U32 MDrv_MVD_GetIsrEvent(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    return pMVDDrvContext->_eCurEvent[u8DrvIdx];
}

void MVD_IsrProc(void)
{
    MS_U32 u32IntStat = 0;
    MS_U8  u8DrvIdx = 0xff;
    MS_U8  u8HalIdx = 0;
    MS_U32* pu32EventFlag;
    MS_U32* pu32CurEvent;
    MVD_InterruptCb pfnCb = NULL;
    MS_U32 u32CbData = MVD_U32_MAX;

    //we can attach only one ISR, so we need to check the INT is from which decoder.
    for (u8DrvIdx=0; u8DrvIdx<MVD_MAX_STREAMS; u8DrvIdx++)
    {
        if(MVD_GetIsUsed(u8DrvIdx) == FALSE)
        {
            continue;
        }

        pu32EventFlag = &(pMVDDrvContext->_eEventFlag[u8DrvIdx]);
        if (*pu32EventFlag == E_MVD_EVENT_DISABLE_ALL)
        {
            MVD_DEBUGINFO(MVD_PRINT("%s skip 0x%x for no registered events.\n", __FUNCTION__, u8DrvIdx));
            continue;
        }

        u8HalIdx = pMVDDrvContext->gMVDCtrl_EX[u8DrvIdx].u8HalIdx;

        if (FALSE == HAL_MVD_GetHasInt(u8HalIdx))
        {
            MVD_DEBUGINFO(MVD_PRINT("%s no INT for %x\n", __FUNCTION__, u8DrvIdx));
            continue;
        }

        pu32CurEvent = &(pMVDDrvContext->_eCurEvent[u8DrvIdx]);
        pfnCb = pMVDDrvContext->_pfnCallback[u8DrvIdx];
        u32CbData = pMVDDrvContext->_u32CallbackPara[u8DrvIdx];

        u32IntStat = HAL_MVD_GetIntState(u8HalIdx);
        if (u32IntStat != 0)
        {
            //MVD_DEBUGINFO(MVD_PRINT("MVD_IsrProc u32IntStat=%lx\n", u32IntStat));
            *pu32CurEvent = E_MVD_EVENT_DISABLE_ALL;

            if ((*pu32EventFlag & E_MVD_EVENT_USER_DATA) == E_MVD_EVENT_USER_DATA)
            {
                if (MVD_IntHasUsrData(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_USER_DATA;
                    MVD_DEBUGINFO(MVD_PRINT("===> UsrData!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_USER_DATA_DISP) == E_MVD_EVENT_USER_DATA_DISP)
            {
                if (MVD_IntHasUsrDataDisp(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_USER_DATA_DISP;
                    MVD_DEBUGINFO(MVD_PRINT("===> DispUsrData!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_DISP_RDY) == E_MVD_EVENT_DISP_RDY)
            {
                if (MVD_IntIsDispRdy(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DISP_RDY;
                    MVD_DEBUGINFO(MVD_PRINT("===> DispRdy!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_SEQ_FOUND) == E_MVD_EVENT_SEQ_FOUND)
            {
                if (MVD_IntHasSeqHdr(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_SEQ_FOUND;
                    MVD_DEBUGINFO(MVD_PRINT("===> SeqHdrFound!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_FIRST_FRAME) == E_MVD_EVENT_FIRST_FRAME)
            {
                if (MVD_IntHas1stFrame(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_FIRST_FRAME;
                    MVD_DEBUGINFO(MVD_PRINT("===> 1stFrame!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_XC_LOW_DEALY) == E_MVD_EVENT_XC_LOW_DEALY)
            {
                if (MVD_IntHasXcLowDelay(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_XC_LOW_DEALY;
                    MVD_DEBUGINFO(MVD_PRINT("===> E_MVD_EVENT_XC_LOW_DEALY!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_DEC_I) == E_MVD_EVENT_DEC_I)
            {
                if (MVD_IntHasDecodeIframe(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DEC_I;
                    MVD_DEBUGINFO(MVD_PRINT("===> E_MVD_EVENT_DEC_I!!!\n"));
                }
            }
            if ((*pu32EventFlag & E_MVD_EVENT_DISP_VSYNC) == E_MVD_EVENT_DISP_VSYNC)
            {
                if (MVD_IntVSyncInt(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DISP_VSYNC;
                    MVD_DEBUGINFO(MVD_PRINT("===> VSyncInt!!!\n"));
                }
            }

            if ((*pu32EventFlag & E_MVD_EVENT_DEC_ONE_FRAME) == E_MVD_EVENT_DEC_ONE_FRAME)
            {
                if (MVD_IntDecOneFrmInt(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DEC_ONE_FRAME;
                    MVD_DEBUGINFO(MVD_PRINT("===> DecOneFrameInt!!!\n"));
                }
            }


            if ((*pu32EventFlag & E_MVD_EVENT_UNMUTE) == E_MVD_EVENT_UNMUTE)
            {
                MVD_DEBUGINFO(MVD_PRINT("E_MVD_EVENT_UNMUTE...\n"));
                if (MVD_IntVSyncInt(u32IntStat))
                {
                    MVD_DEBUGINFO(MVD_PRINT("===> UNMUTE?!\n"));
                    if (HAL_MVD_GetIsIPicFound(u8HalIdx) != NULL)
                    {
                        (pMVDDrvContext->u32VSyncCnt[u8DrvIdx])++;
                        if ((HAL_MVD_GetIsSyncReach(u8HalIdx)==1) ||
                                (pMVDDrvContext->u32VSyncCnt[u8DrvIdx] > 60) ||
                                (HAL_MVD_GetIsAVSyncOn(u8HalIdx) == FALSE))
                        {
                            pMVDDrvContext->u32VSyncCnt[u8DrvIdx] = 0;
                            *pu32CurEvent |= E_MVD_EVENT_UNMUTE;
                            MVD_DEBUGINFO(MVD_PRINT("UnMute!!!\n"));
                            *pu32EventFlag &= (~E_MVD_EVENT_UNMUTE);
                            HAL_MVD_EnableInt(u8HalIdx, *pu32EventFlag);
                        }
                    }
                    else // for P/B frame only stream
                    {
                        if (HAL_MVD_GetIsSyncReach(u8HalIdx) == 1) //sync done
                        {
                            (pMVDDrvContext->u32SyncDoneCnt[u8DrvIdx])++;
                        }

                        if (pMVDDrvContext->u32SyncDoneCnt[u8DrvIdx] > PB_ONLY_UNMUTE_VSYNC_COUNT) //sync done
                        {
                            MVD_DEBUGINFO(MVD_PRINT("UnMute after %d vsync!!!\n", PB_ONLY_UNMUTE_VSYNC_COUNT));
                            *pu32CurEvent |= E_MVD_EVENT_UNMUTE;
                            *pu32EventFlag &= (~E_MVD_EVENT_UNMUTE);
                            HAL_MVD_EnableInt(u8HalIdx, *pu32EventFlag);
                            pMVDDrvContext->u32SyncDoneCnt[u8DrvIdx] = 0;
                        }
                    }
                }
            }

            if ((*pu32EventFlag & E_MVD_EVENT_DEC_ERR) == E_MVD_EVENT_DEC_ERR)
            {
                if (MVD_IntHasDecodeErr(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DEC_ERR;
                    MVD_DEBUGINFO(MVD_PRINT("===> Decode error!!!\n"));
                }
            }

            if ((*pu32EventFlag & E_MVD_EVENT_DEC_DATA_ERR) == E_MVD_EVENT_DEC_DATA_ERR)
            {
                if (MVD_IntHasESDataInvalid(u32IntStat))
                {
                    *pu32CurEvent |= E_MVD_EVENT_DEC_DATA_ERR;
                    MVD_DEBUGINFO(MVD_PRINT("===> ES data invaild!!!\n"));
                }
            }

            //Events that user registered occurred, call user's callback function
            if ((*pu32CurEvent!=E_MVD_EVENT_DISABLE_ALL) && (pfnCb!=NULL))
            {
                MVD_DEBUGINFO(MVD_PRINT("%s Callback for *pu32CurEvent=0x%x, cbData=0x%x\n",
                    __FUNCTION__, *pu32CurEvent, u32CbData));
                pfnCb(u32CbData); //FIXME: need to transfer u32Id(24-bit)
            }
        }

        //clear events
        *pu32CurEvent = E_MVD_EVENT_DISABLE_ALL;
    }

    HAL_MVD_ClearIRQ(); //clear interrupt
    if(TRUE == OSAL_MVD_IntEnable())//enable cpu interrupt mask
    {
        pMVDDrvContext->_bDisableFlag = FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_SetISREvent()
/// @brief \b Function \b Description: Set the ISR event type sended by HVD fw.
/// @param -eEvent \b IN : event types
/// @param -fnISRHandler \b IN : function pointer to a interrupt handler.
/// @return -The result of command set ISR event.
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetIsrEvent(MS_U32 u32Id, MS_U32 eEvent, MVD_InterruptCb fnHandler)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);

    if (FALSE == MVD_GetIsUsed(u8DrvIdx))
    {
        MVD_DEBUGERROR(MVD_ERR("Call %s before Init\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    if (eEvent == E_MVD_EVENT_DISABLE_ALL)
    {
        // Fix the ISR imbalance of Enable and Disable
        // this is the rootcase of MVD ISR fail.
        if((pMVDDrvContext->_bDisableFlag != TRUE) && (FALSE == pMVDDrvContext->gMVDCtrl_EX[1-u8DrvIdx].bUsed))
        {
            pMVDDrvContext->_bDisableFlag = OSAL_MVD_IntDisable();
            if(pMVDDrvContext->_bDisableFlag == FALSE)
            {
                MVD_DEBUGERROR(printf("%s Disable Interrupt Fail\n", __FUNCTION__));
            }
#if !defined(SUPPORT_X_MODEL_FEATURE) && !defined(DISABLE_ISR_DETACH)
            if(TRUE == pMVDDrvContext->bMVDIsrAttached)
            {
                OSAL_MVD_IsrDetach();
                pMVDDrvContext->bMVDIsrAttached = FALSE;
            }
#endif
        }
        pMVDDrvContext->_pfnCallback[u8DrvIdx] = NULL;
        pMVDDrvContext->_u32CallbackPara[u8DrvIdx] = MVD_U32_MAX;
        pMVDDrvContext->_eEventFlag[u8DrvIdx] = E_MVD_EVENT_DISABLE_ALL;
    }
    else
    {
        if(fnHandler != NULL)
        {
            //disable int & dettach isr?

            pMVDDrvContext->_pfnCallback[u8DrvIdx] = (MVD_InterruptCb)fnHandler;
            pMVDDrvContext->_u32CallbackPara[u8DrvIdx] = u32Id;
            MVD_DEBUGINFO(MVD_PRINT("%s Hook callback u32Id=0x%x u8HalIdx=0x%x eEvent=0x%x\n",
                __FUNCTION__, u32Id, u8DrvIdx, eEvent));
            pMVDDrvContext->_eEventFlag[u8DrvIdx] = eEvent;

#if !defined(SUPPORT_X_MODEL_FEATURE) && !defined(DISABLE_ISR_DETACH)
            if (FALSE == pMVDDrvContext->bMVDIsrAttached)
            {
                if (OSAL_MVD_IsrAttach((void*)MVD_IsrProc) != TRUE)
                {
                    MVD_DEBUGERROR(MVD_ERR("fail to attach MVD_IsrProc!\n"));
                    return E_MVD_RET_FAIL;
                }
                pMVDDrvContext->bMVDIsrAttached = TRUE;
                if (OSAL_MVD_IntEnable() != TRUE)
                {
                    MVD_DEBUGERROR(MVD_ERR("fail to OSAL_MVD_IntEnable!\n"));
                    return E_MVD_RET_FAIL;
                }
                pMVDDrvContext->_bDisableFlag = FALSE;
            }
#else
            if (FALSE == pMVDDrvContext->bMVDIsrAttached)
            {
                if (OSAL_MVD_IsrAttach((void*)MVD_IsrProc) != TRUE)
                {
                    MVD_DEBUGERROR(MVD_ERR("fail to attach MVD_IsrProc!\n"));
                    return E_MVD_RET_FAIL;
                }
                pMVDDrvContext->bMVDIsrAttached = TRUE;
            }

            if (OSAL_MVD_IntEnable() != TRUE)
            {
                MVD_DEBUGERROR(MVD_ERR("fail to OSAL_MVD_IntEnable!\n"));
                return E_MVD_RET_FAIL;
            }
            pMVDDrvContext->_bDisableFlag = FALSE;
#endif
            MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_SetIsrEvent eEventFlag=0x%x\n", pMVDDrvContext->_eEventFlag[u8DrvIdx]));
            MDrv_MVD_EnableInt(u32Id, pMVDDrvContext->_eEventFlag[u8DrvIdx]);
            MVD_DEBUGINFO(MVD_PRINT("attach ISR number:%d\n" , E_INT_IRQ_MVD));
            return E_MVD_RET_OK;
        }
        else
        {
            MVD_DEBUGERROR(MVD_ERR( "SetISREvent with NULL pointer. ISR type:%u\n", (MS_U32)eEvent));
            return E_MVD_RET_INVALID_PARAM;
        }
    }
    return E_MVD_RET_OK;
}

#endif //MVD_ENABLE_ISR


//------------------------------------------------------------------------------
/// Set AVSync mode for file mode.
/// @param -eSyncMode: avsync mode for file mode
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetFileModeAVSync(MS_U32 u32Id, MVD_TIMESTAMP_TYPE eSyncMode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetFileModeAVSync(u8HalIdx, eSyncMode);
}

//-----------------------------------------------------------------------------
/// @brief Check if all of the buffers(display, decoded, bitstream) are empty.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Not Empty.
/// @retval     -TRUE(1): Empty.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsAllBufferEmpty(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_DEBUGINFO(MVD_PRINT("%s stat=0x%x, cmd=0x%x\n", __FUNCTION__,
                  MDrv_MVD_GetDecodeStatus(u32Id), MDrv_MVD_GetLastCmd(u32Id)));
    return HAL_MVD_IsAllBufferEmpty(u8HalIdx);
}

//-----------------------------------------------------------------------------
/// @brief Generate specific pattern to support some special function.
/// @param -u32VAddr \b IN : the virtual address of specific pattern
/// @param -u32Size \b IN, OUT :
///                             IN: the input array size.
///                             OUT: the used array size.
/// @return -The result of command generate specific pattern
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GenPattern(MS_U32 u32Id, MVD_PatternType ePattern, MS_PHY u32PAddr, MS_U32* pu32Size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (!pu32Size)
    {
        return FALSE;
    }
    return HAL_MVD_GenPattern(u8HalIdx, ePattern, u32PAddr, pu32Size);
}

//-----------------------------------------------------------------------------
/// @brief Get driver specific data information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPatternInfo(void)
{
    return MVD_U32_MAX;
}

//-----------------------------------------------------------------------------
/// @brief Pass scalar parameters to f/w
/// @return -The result of command.
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetDynScalingParam(MS_U32 u32Id, MS_PHY u32StAddr, MS_VIRT u32Size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if ((u32StAddr==NULL) || (u32Size==0))
    {
        MVD_DEBUGERROR(MVD_ERR("%s invalid para u32StAddr=0x%lx, u32Size=0x%lx\n",
                       __FUNCTION__, u32StAddr, u32Size));
        return E_MVD_RET_INVALID_PARAM;
    }
    return HAL_MVD_SetDynScalingParam(u8HalIdx, u32StAddr, u32Size);
}

//------------------------------------------------------------------------------
/// Set virtual box width/height to F/W.
/// F/W will use the same w/h as scaler to calculate scaling factor.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_DEBUGINFO(MVD_PRINT("%s: w=0x%x h=0x%x\n", __FUNCTION__, u16Width, u16Height));
    return HAL_MVD_SetVirtualBox(u8HalIdx, u16Width, u16Height);
}

//------------------------------------------------------------------------------
/// Set blue screen
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEn)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetBlueScreen(u8HalIdx, bEn);
}

//------------------------------------------------------------------------------
/// Enable/Disable VSync interrupt
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_EnableInt(MS_U32 u32Id, MS_U32 bEn)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_EnableInt(u8HalIdx, bEn);
}

//------------------------------------------------------------------------------
/// Enable FW only show one field.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_EnableDispOneField(MS_U32 u32Id, MS_BOOL bEn)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_EnableDispOneField(u8HalIdx, bEn);
}

//------------------------------------------------------------------------------
/// Get extension info of the display .
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetExtDispInfo(MS_U32 u32Id, MVD_ExtDispInfo* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (NULL == pInfo)
    {
        MVD_DEBUGERROR(MVD_ERR("GetExtDispInfo NULL pInfo!\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

    HAL_MVD_GetExtDispInfo(u8HalIdx, pInfo);
    return eRet;
}

//------------------------------------------------------------------------------
/// Get info of the decoded/displaying frame.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetFrmInfo(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_FrmInfo* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (NULL == pInfo)
    {
        MVD_DEBUGERROR(MVD_ERR("GetFrmInfo NULL pInfo!\n"));
        return E_MVD_RET_INVALID_PARAM;
    }
    eRet = HAL_MVD_GetFrmInfo(u8HalIdx, eType, pInfo);
    //MVD_DumpFrmInfo(pInfo);
    return eRet;
}

//------------------------------------------------------------------------------
/// Get time_code of the decoded/displaying frame.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetTimeCode(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_TimeCode* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (NULL == pInfo)
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    if (TRUE != HAL_MVD_GetTimeCode(u8HalIdx, eType, pInfo))
    {
        MVD_DEBUGERROR(MVD_ERR("MDrv_MVD_GetTimeCode fail\n"));
        return E_MVD_RET_FAIL;
    }

    if ((E_MVD_FRMINFO_DECODE != eType) || (E_MVD_FRMINFO_DISPLAY == eType))
    {
        eRet = E_MVD_RET_INVALID_PARAM;
    }

    return eRet;
}

MS_BOOL MDrv_MVD_GetUsrDataIsAvailable(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetUsrDataIsAvailable(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get info of user data
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetUsrDataInfo(MS_U32 u32Id, MVD_UsrDataInfo* pUsrInfo)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (!pUsrInfo)
    {
        MVD_DEBUGERROR(MVD_ERR("%s: NULL ptr.\n", __FUNCTION__));
        return FALSE;
    }
    return HAL_MVD_GetUsrDataInfo(u8HalIdx, pUsrInfo);
}

//------------------------------------------------------------------------------
/// Enable/Disable freezing display
/// Once this flag is set, firmware continue decode new frame
/// but not to push this frame into display queue.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFreezeDisp(MS_U32 u32Id, MS_BOOL bEn)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_DEFAULT, 1);
    if (TRUE == HAL_MVD_SetFreezeDisp(u8HalIdx, bEn))
    {
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
}

//------------------------------------------------------------------------------
/// Get MVD displayed picture counter
/// @return - displayed picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetDispCount(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetDispCnt(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Set MVD fd mask delay count
/// 16bits and unit in vsync for mute the fd_mask
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFdMaskDelayCount(MS_U32 u32Id, MS_U16 u16Cnt)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetFdMaskDelayCount(u8HalIdx, u16Cnt);
}

//------------------------------------------------------------------------------
/// Set MVD fd mask delay count
/// 16bits and unit in vsync for mute the fd_mask
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetXcLowDelayIntState(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetXcLowDelayIntState(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Set MVD output frame rate convert
/// @param u8FrameRate \b IN : output frame rate of 8bits and unit in vsync
/// @param u8Interlace \b IN : scan style 0:progress, 1:interlace
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetOutputFRCMode(MS_U32 u32Id, MS_U8 u8FrameRate, MS_U8 u8Interlace)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetOutputFRCMode(u8HalIdx, u8FrameRate, u8Interlace);
}

//------------------------------------------------------------------------------
/// Set MVD FRC drop type.
/// @param u8DropType \b IN : drop type. 0:drop frame, 1:drop field. default:0
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFRCDropType(MS_U32 u32Id, MS_U8 u8DropType)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetFRCDropType(u8HalIdx, u8DropType);
}

//------------------------------------------------------------------------------
/// Set disable resolution change.
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetDisableSeqChange(MS_U32 u32Id, MS_BOOL bEnable)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetDisableSeqChange(u8HalIdx, bEnable);
}

//------------------------------------------------------------------------------
/// Debug function to dump useful info.
//------------------------------------------------------------------------------
void MDrv_MVD_DbgDump(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_FrameInfo* pPreInfo = &(pMVDDrvContext->stPreFrmInfo[u8DrvIdx]);
    MVD_AVSyncCfg* pAVSyncCfg  = &(pMVDDrvContext->stSyncCfg[u8DrvIdx]);

    if (FALSE == MVD_GetIsUsed(u8DrvIdx))
    {
        MVD_PRINT("%s: _bDrvInit false!\n", __FUNCTION__);
        return;
    }

    HAL_MVD_DbgDump(u8HalIdx);
    MVD_PRINT("avsync on=%x, delay=%d, tolerance=%d, done=%x, skip=%x, repeat=%x, pts=%dms\n",
            pAVSyncCfg->bEnable, pAVSyncCfg->u32Delay, pAVSyncCfg->u16Tolerance, (MDrv_MVD_GetSyncStatus(u32Id)==1),
            MDrv_MVD_GetIsSyncSkip(u32Id), MDrv_MVD_GetIsSyncRep(u32Id), MDrv_MVD_GetPTS(u32Id));
    MVD_PRINT("seq(%d): w=%d, h=%d, i/p=%x, fps=%d\n", MDrv_MVD_GetDispRdy(u32Id),
        pPreInfo->u16HorSize, pPreInfo->u16VerSize,
        pPreInfo->u8Interlace, pPreInfo->u32FrameRate);
}

//------------------------------------------------------------------------------
/// Dump the bitstream to predefined buffer address.
/// Before PLAY command, set the bitstream base & bitstream length, and then
/// CPU would continue to dump bitstream at the base address.
/// @param -u32base \b IN : start address (MIU offset, e.g. 128M==>0MB)
/// @param -u32size \b IN : size (bytes)
//------------------------------------------------------------------------------
void MDrv_MVD_DbgDumpBits(MS_U32 u32Id, MS_PHY u32base, MS_U32 u32size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_DbgDumpBits(u8HalIdx, u32base, u32size);
    return;
}

// only for VDEC internal link patch
MS_BOOL MDrv_MVD_LinkWeakSymbolPatch(void)
{
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set firmware as MStreamer mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetMStreamerMode(MS_U32 u32Id, MS_U8 u8Mode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetMStreamerMode(u8HalIdx, u8Mode);
}

//-----------------------------------------------------------------------------
/// set firmware as MCU mode
/// @return -TRUE for success; FALSE for failure.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetMcuMode(MS_U32 u32Id, MS_U8 u8Mode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetMcuMode(u8HalIdx, u8Mode);
}

//-----------------------------------------------------------------------------
/// set firmware as I/P clip mode mode
/// @return -TRUE for success; FALSE for failure.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ShowDecodeOrder(MS_U32 u32Id, MS_U8 u8Mode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_ShowDecodeOrder(u8HalIdx, u8Mode);
}



//------------------------------------------------------------------------------
/// Flip the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameFlip(MS_U32 u32Id, MS_U8 u8FrmIdx)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (TRUE != HAL_MVD_IsMStreamerMode(u8HalIdx))
    {
        MVD_DEBUGINFO(MVD_PRINT("Need to SetMStreamerMode before using this function\n"));
        return FALSE;
    }

    if (TRUE == HAL_MVD_IsMcuMode(u8HalIdx))
    {
        MVD_DEBUGINFO(MVD_PRINT("It's MCU mode, doesn't need to using this fuction\n"));
        return FALSE;
    }

    return HAL_MVD_FrameOpt(u8HalIdx, u8FrmIdx, E_MVD_FRAME_FLIP);
}

//------------------------------------------------------------------------------
/// Release the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameRelease(MS_U32 u32Id, MS_U8 u8FrmIdx)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    if (TRUE != (HAL_MVD_IsMStreamerMode(u8HalIdx) || HAL_MVD_IsMcuMode(u8HalIdx)) )
    {
        MVD_DEBUGINFO(MVD_PRINT("Need to SetMStreamerMode or SetMcuMode before using this function\n"));
        return FALSE;
    }
    return HAL_MVD_FrameOpt(u8HalIdx, u8FrmIdx, E_MVD_FRAME_RELEASE);
}

//------------------------------------------------------------------------------
/// Enable/Disable to capture the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameCapture(MS_U32 u32Id, MS_U8 u8FrmIdx, MS_BOOL bEnable)
{
    MS_BOOL ret = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    if (TRUE != HAL_MVD_IsMStreamerMode(u8HalIdx))
    {
        MVD_DEBUGINFO(MVD_PRINT("Need to SetMStreamerMode before using this function\n"));
        return FALSE;
    }

    ret = HAL_MVD_FrameCapture(u8HalIdx, u8FrmIdx, bEnable);

    //firmware will keep this frame until we release it.
    if (ret && !bEnable)
    {
        ret = MDrv_MVD_FrameRelease(u32Id, u8FrmIdx);
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Allocate share memory from MSOS for Drv MVD
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_Init_Share_Mem(void)
{
#if ((defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS)) && (!defined(SUPPORT_X_MODEL_FEATURE)))
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;


    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux MVD driver",
                                  sizeof(MVD_Drv_CTX),
                                  &u32ShmId,
                                  &u32Addr,
                                  &u32BufSize,
                                  MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVD driver",
                                     sizeof(MVD_Drv_CTX),
                                     &u32ShmId,
                                     &u32Addr,
                                     &u32BufSize,
                                     MSOS_SHM_CREATE))
        {
            MVD_DEBUGERROR(MVD_ERR("[%s]SHM allocation failed!!! use global structure instead!!!\n",__FUNCTION__));
            if(pMVDDrvContext == NULL)
            {
                pMVDDrvContext = &gMVDDrvContext;
                memset(pMVDDrvContext,0,sizeof(MVD_Drv_CTX));
                MVD_Context_Init();
                MVD_PRINT("[%s]Global structure init Success!!!\n",__FUNCTION__);
            }
            else
            {
                MVD_PRINT("[%s]Global structure exists!!!\n",__FUNCTION__);
            }
            //return E_MVD_RET_FAIL;
        }
        else
        {
            memset((MS_U8*)u32Addr,0,sizeof(MVD_Drv_CTX));
            pMVDDrvContext = (MVD_Drv_CTX*)u32Addr; // for one process
            MVD_Context_Init();
        }
    }
    else
    {
        pMVDDrvContext = (MVD_Drv_CTX*)u32Addr; // for another process
    }
#else
    if(pMVDDrvContext == NULL)
    {
        pMVDDrvContext = &gMVDDrvContext;
        memset(pMVDDrvContext,0,sizeof(MVD_Drv_CTX));
        MVD_Context_Init();
    }
#endif

    if(HAL_MVD_Init_Share_Mem() != TRUE)
    {
        return E_MVD_RET_FAIL;
    }

    if(HAL_VPU_EX_Init_Share_Mem() != TRUE)
    {
        return E_MVD_RET_FAIL;
    }

    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Get the available streamID for the specified stream type
/// @return u32ID as (drvIdx << 16 | u32MVDStreamId << 8 | u32VPUStreamId)
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetFreeStream(MS_U32 *pu32Id, MVD_DRV_StreamType eStreamType)
{
    MS_U8 i = 0xff;
    MS_U32 u32VPUStreamId = 0;
    MS_U32 u32MVDStreamId = 0;
    HAL_VPU_StreamType eVpuStreamType = E_HAL_VPU_MAIN_STREAM;
    HAL_MVD_StreamType eHalStreamType = E_HAL_MVD_MAIN_STREAM;

    if(pu32Id == NULL)
    {
        MVD_DEBUGERROR(MVD_ERR("%s err: NULL pu32Id\n", __FUNCTION__));
        return E_MVD_RET_INVALID_PARAM;
    }
    else
    {
        *pu32Id = 0;
    }

    //Get drvIdx
    switch (eStreamType)
    {
        // drvIdx is fixed for one MVD_DRV_StreamType
      case E_MVD_DRV_MAIN_STREAM:
          i=0;
          break;
      case E_MVD_DRV_SUB_STREAM:
          i=1;
          break;
      case E_MVD_DRV_STREAM_NONE:
      default:
          i=0xff;
          break;
    }
    if ((i>=MVD_MAX_STREAMS) || (TRUE == MVD_GetIsUsed(i)))
    {
        MVD_DEBUGERROR(MVD_ERR("eStreamType=%x is used or invalid, i=%x\n", eStreamType, i));
        *pu32Id = 0;
        return E_MVD_RET_FAIL;
    }

    //Get VPU & HAL stream id
    if (E_MVD_DRV_MAIN_STREAM == eStreamType)
    {
        eVpuStreamType = E_HAL_VPU_MAIN_STREAM;
        eHalStreamType = E_HAL_MVD_MAIN_STREAM;
    }
    else if (E_MVD_DRV_SUB_STREAM == eStreamType)
    {
        eVpuStreamType = E_HAL_VPU_SUB_STREAM;
        eHalStreamType = E_HAL_MVD_SUB_STREAM;
    }
    u32VPUStreamId = (MS_U32) HAL_VPU_EX_GetFreeStream(eVpuStreamType);
    if(u32VPUStreamId == E_HAL_VPU_STREAM_NONE)
    {
       return E_MVD_RET_FAIL;
    }
    u32MVDStreamId = (MS_U32) HAL_MVD_GetFreeStream(eHalStreamType);

    MVD_DEBUGVERBAL(MVD_PRINT("%s:: i=0x%x, mvd=0x%x, vpu=0x%x\n", __FUNCTION__,
            i, u32MVDStreamId, u32VPUStreamId));
    if (u32VPUStreamId && u32MVDStreamId)
    {
        MVD_DEBUGINFO(MVD_PRINT("%s   i=0x%x, mvd=0x%x, vpu=0x%x\n", __FUNCTION__,
            i, u32MVDStreamId, u32VPUStreamId));
        *pu32Id = (i << 16 | u32MVDStreamId << 8 | u32VPUStreamId);
        if (FALSE == MVD_SaveStreamId(i, u32MVDStreamId, u32VPUStreamId))
        {
            return E_MVD_RET_FAIL;
        }
    }
    else
    {
        MVD_DEBUGINFO(MVD_PRINT("%s(%d) NG  i=0x%x, mvd=0x%x, vpu=0x%x\n", __FUNCTION__, __LINE__,
            i, u32MVDStreamId, u32VPUStreamId));
    }

    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Set bitstream buffer overflow threshold
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetOverflowTH(MS_U32 u32Id, MS_U32 u32Threshold)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetOverflowTH(u8HalIdx, u32Threshold);
    return;
}

//------------------------------------------------------------------------------
/// Set bitstream buffer underflow threshold
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetUnderflowTH(MS_U32 u32Id, MS_U32 u32Threshold)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    HAL_MVD_SetUnderflowTH(u8HalIdx, u32Threshold);
    return;
}

MS_BOOL MDrv_MVD_SetSingleDecodeMode(MS_BOOL bEnable)
{
    return HAL_VPU_EX_SetSingleDecodeMode(bEnable);
}

//-----------------------------------------------------------------------------
/// Release fd mask
/// @return -TRUE for success; FALSE for failure.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ReleaseFdMask(MS_U32 u32Id, MS_BOOL bRls)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_ReleaseFdMask(u8HalIdx, bRls);
}

//------------------------------------------------------------------------------
/// Reset MVD parser .  It is used to reset MVD Parser.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ParserRstDone(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_ParserRstDone(u8HalIdx, bEnable);
}
//------------------------------------------------------------------------------
/// Flush PTS buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FlushPTSBuf(MS_U32 u32Id,MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_FlushPTSBuf(u8HalIdx ,bEnable);
}

//------------------------------------------------------------------------------
/// Get SLQ number
/// @return -the SLQ number
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetSLQNum(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetSLQNum(u8HalIdx);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_GetFrmRateIsSupported()
/// @brief \b Function \b Description:  Get if the framerate is supported
/// @return -The result of supported or not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetFrmRateIsSupported(MS_U32 u32Id)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_BOOL bSupported = pMVDDrvContext->bFrmRateSupported[u8DrvIdx];

    return bSupported;
}

//------------------------------------------------------------------------------
/// set fw auto mute
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetAutoMute(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_BOOL bRet = HAL_MVD_SetAutoMute(u8HalIdx, bEnable);
    return ((TRUE == bRet) ? E_MVD_RET_OK : E_MVD_RET_FAIL);
}

MS_BOOL MDrv_MVD_SetVSizeAlign(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetVSizeAlign(u8HalIdx, bEnable);
}

//------------------------------------------------------------------------------
/// Set codec type.
/// @param -pCrcIn \b IN : the struct of generate CRC
/// @param -pCrcOut \b OUT : CRC value from mvd hw
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetCrcValue(MS_U32 u32Id, MVD_CrcIn *pCrcIn, MVD_CrcOut *pCrcOut)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    //check input parameters
    if (pCrcIn == NULL)
    {
        MVD_DEBUGERROR(MVD_ERR("PushQueue NULL pInfo\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

    return HAL_MVD_GetCrcValue(u8HalIdx, pCrcIn, pCrcOut);
}

//------------------------------------------------------------------------------
/// set debug mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetDbgMode(MS_U32 u32Id, MVD_DbgMode enDbgMode, MS_BOOL bEn)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    E_MVD_Result eRet=E_MVD_RET_FAIL;

    MVD_DEBUGINFO(MVD_PRINT("MDrv_MVD_SetDbgMode...%x\n",(unsigned int)pstCtrlCfg->stDbgModeCfg.value));
    switch (enDbgMode)
    {
        case E_MVD_EX_DBG_MODE_BYPASS_INSERT_START_CODE:
            {
                pstCtrlCfg->stDbgModeCfg.bBypassInsertStartCode=bEn;
                eRet=E_MVD_RET_OK;
            }
            break;
        case E_MVD_EX_DBG_MODE_BYPASS_DIVX_MC_PATCH:
            {
                pstCtrlCfg->stDbgModeCfg.bBypassDivxMCPatch=bEn;
                eRet=E_MVD_RET_OK;
            }
            break;
        default:
            eRet=E_MVD_RET_INVALID_PARAM;
            break;
    }
    MVD_DEBUGINFO(MVD_PRINT("%s   stDbgModeCfg=%x\n", __FUNCTION__, (unsigned int)pstCtrlCfg->stDbgModeCfg.value));
    return eRet;
}

//------------------------------------------------------------------------------
/// Get SLQ number
/// @return -the SLQ number
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetDispQNum(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetDispQNum(u8HalIdx);
}

//------------------------------------------------------------------------------
/// set fw fask suspend
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------

MS_BOOL MDrv_MVD_SuspendDynamicScale(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SuspendDynamicScale(u8HalIdx, bEnable);
}

MS_U8 MDrv_MVD_GetSuspendDynamicScale(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetSuspendDynamicScale(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Get MVD DivX plus Version,
/// @return -non DivX plus  \b OUT 0
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetDivxVer(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetDivxVer(u8HalIdx);
}

//------------------------------------------------------------------------------
/// Set IDCT mode
/// @param -u8Mode \b IN : 0 for original IDCT, 1 for new IDCT
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetIdctMode(MS_U32 u32Id, MS_U8 u8Mode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    return HAL_MVD_SetIdctMode(u8HalIdx, u8Mode);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_EX_SetMVDClockSpeed()
/// @brief \b Function \b Description:  set mvd clock speed
/// @return - The result of setting mvd clock speed
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_EX_SetMVDClockSpeed(MVD_EX_ClockSpeed eClockSpeed)
{
    HAL_MVD_EX_ClockSpeed eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_NONE;
    switch (eClockSpeed)
    {
        case E_MVD_EX_CLOCK_SPEED_HIGHEST:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_HIGHEST;
            break;
        case E_MVD_EX_CLOCK_SPEED_HIGH:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_HIGH;
            break;
        case E_MVD_EX_CLOCK_SPEED_MEDIUM:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_MEDIUM;
            break;
        case E_MVD_EX_CLOCK_SPEED_LOW:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_LOW;
            break;
        case E_MVD_EX_CLOCK_SPEED_LOWEST:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_LOWEST;
            break;
        case E_MVD_EX_CLOCK_SPEED_DEFAULT:
            eMVDClockSpeed = E_HAL_MVD_EX_CLOCK_SPEED_DEFAULT;
            break;
        default:
            MVD_DEBUGERROR(MVD_ERR("mvd clock setting is wrong(%d)\n", eClockSpeed));
            return E_MVD_RET_FAIL;
            break;
    }

    return HAL_MVD_EX_SetClockSpeed(eMVDClockSpeed);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_HWBuffer_ReMappingMode()
/// @brief \b Function \b Description:   Enable/ Disable to push first frame to display queue directly
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): disable this mode
///                 -TRUE(1): enable this mode
/// @return -The result of command MDrv_MVD_HWBuffer_ReMappingMode
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_HWBuffer_ReMappingMode(MS_U32 u32Id,MS_BOOL bEnable)
{

   E_MVD_Result eRet = E_MVD_RET_OK;
   MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

   pMVDDrvContext->gMVDPreCtrl[u8HalIdx].bHWBufferReMapping = bEnable;
   eRet=HAL_MVD_HWBuffer_ReMappingMode(u8HalIdx,bEnable);
    return eRet;
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_ShowFirstFrameDirect()
/// @brief \b Function \b Description:   Enable/ Disable to push first frame to display queue directly
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): disable this mode
///                 -TRUE(1): enable this mode
/// @return -The result of command MDrv_MVD_ShowFirstFrameDirect
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ShowFirstFrameDirect(MS_U32 u32Id,MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    return HAL_MVD_ShowFirstFrameDirect(u8HalIdx, bEnable);
}

//------------------------------------------------------------------------------
/// Set xc_low_delay parameter for xc_low_delay mechanism
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetXCLowDelayPara(MS_U32 u32Id,MS_U32 u32Para)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    return HAL_MVD_SetXCLowDelayPara(u8HalIdx, u32Para);

}

//------------------------------------------------------------------------------
/// Set self seqchange mode
/// @param -bEnable \b IN : 0 for original seqchange, 1 for self seqchange
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetSelfSeqChange(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8DrvIdx = _MVD_GET_IDX(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_BOOL ret;

    ret = HAL_MVD_EnableDynamicScale(u8HalIdx,bEnable);

    if(ret == TRUE)
    {
        pMVDDrvContext->bSelf_SeqChange[u8DrvIdx] = bEnable;
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
}


//------------------------------------------------------------------------------
/// Set external DS buffer info.
/// @param -pExternalBuf \b IN : External DS buffer info.
/// @return -The result of setting external DS buffer
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetExternalDSBuffer(MS_U32 u32Id, MVD_EX_ExternalDSBuf *pExternalBuf)
{
#define SIZE_3K 0xC00
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8HalIdx);

    //MS_U32 u32DrvAddr = NULL;
    if((pstMemCfg == NULL) || (pExternalBuf->u32DSBufSize < SIZE_3K)) /// DS buffer size need more than 3K (0xC00)
    {
        return E_MVD_RET_FAIL;
    }

#if defined(CHIP_MONACO)
    if(VPU_FORCE_MIU_MODE == 1)
    {
        printf("this chip does not support set external DS buffer since VPU_FORCE_MIU_MODE == 1 !!\n");
        return E_MVD_RET_FAIL;
    }
#endif

#if (defined(CHIP_EDISON) || defined(CHIP_EINSTEIN) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO))
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8HalIdx);
    MS_BOOL bRet = FALSE;
    MS_BOOL bExternalInMIU1 = FALSE;
    MS_VIRT u32DSAddr = pExternalBuf->u32DSBufAddr;
    MS_VIRT u32VPUAddr = NULL;

    if((pExternalBuf->u32DSBufAddr >= HAL_MVD_GetMiu1BaseAdd()) && (HAL_MVD_GetMiu1BaseAdd() != 0))
    {
        bExternalInMIU1 = TRUE;
        u32DSAddr -= HAL_MVD_GetMiu1BaseAdd();
    }

    /// If FW and external buf is in the same MIU, the External buffer do nothing.
    if(HAL_MVD_GetFWSelMiu1() == bExternalInMIU1)
    {
        u32VPUAddr = u32DSAddr;
    }
    else
    {
        /// If FW and external buf is not in the same MIU, the External buffer add the VPU Miu base size
        u32VPUAddr = u32DSAddr + HAL_VPU_EX_MIU1BASE();
    }

    ///printf("[EDS] VPU addr 0x%lx, drv addr 0x%lx, enable %d.\n",u32VPUAddr,pExternalBuf->u32DSBufAddr,pExternalBuf->bEnable);
    bRet = HAL_MVD_SetExternalDSBuff(u8HalIdx,u32VPUAddr,pExternalBuf->u32DSBufAddr);

    pstCtrlCfg->bExternalDSBuf = TRUE;

    if(bRet == TRUE)
    {
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
 #endif

    return E_MVD_RET_FAIL;
}

MS_U8 MDrv_MVD_GetESBufferStatus(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    return HAL_MVD_GetESBufferStatus(u8HalIdx);
}

MS_BOOL MDrv_MVD_Field_Polarity_Display_One_field(MS_U32 u32Id, MS_BOOL bEnable,MS_U8 top_bottom)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_Field_Polarity_Display_One_field(u8HalIdx, bEnable,top_bottom);
}


//------------------------------------------------------------------------------
/// set share memory base between driver and fw
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetShareMemoryBase(MS_U32 u32Id, MS_U32 u32base)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    if((u32base & MVD_U32_MASK) == HAL_MIU1_BASE)
    {
        u32base -= HAL_MIU1_BASE;
        return HAL_MVD_SetShareMemoryBase(u8HalIdx, u32base,MIU_SEL_1); //in miu1
    }
    else
    {
        return HAL_MVD_SetShareMemoryBase(u8HalIdx, u32base,MIU_SEL_0); // in miu0
    }
}

MS_U32 MDrv_MVD_GetShareMemoryOffset(MS_U32 u32Id, MS_U32 *u32base)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetShareMemoryOffset(u8HalIdx, u32base);
}

MS_BOOL MDrv_MVD_EnableVPUSecurityMode(MS_BOOL enable)
{
    return HAL_VPU_EX_EnableSecurityMode(enable);
}

MS_BOOL MDrv_MVD_GetSupport2ndMVOPInterface(void)
{
    MVD_FUNC_ENTRY();

    return HAL_MVD_Support2ndMVOPInterface();
}

MS_U32 MDrv_MVD_GetVsyncBridgeAddr(MS_U32 u32Id)
{
    MVD_FUNC_ENTRY();
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_GetVsyncBridgeAddr(u8HalIdx);
}


MS_BOOL MDrv_MVD_REE_RegisterMBX(void)
{
    return HAL_VPU_EX_REE_RegisterMBX();
}

MS_BOOL MDrv_MVD_REE_SetSHMBaseAddr(void)
{
    return HAL_VPU_EX_REE_SetSHMBaseAddr();
}

//------------------------------------------------------------------------------
/// set CC 608 or 708 share memory base between driver and fw
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetExternal_CC608_Buffer(MS_U32 u32Id, MS_U32 u32base,MS_U8 u8size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetExternal_CC_Buffer(u8HalIdx,u32base,u8size,1);
}

MS_BOOL MDrv_MVD_SetExternal_CC708_Buffer(MS_U32 u32Id, MS_U32 u32base,MS_U8 u8size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetExternal_CC_Buffer(u8HalIdx,u32base,u8size,0);
}

MS_BOOL MDrv_MVD_SetPrebufferSize(MS_U32 u32Id, MS_U32 size)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetPrebufferSize(u8HalIdx,size);
}

void MDrv_MVD_REE_GetSHMInformation(MS_VIRT* u32SHMAddr,MS_U32* u32VsyncSHMOffset)
{
    *u32SHMAddr = HAL_VPU_EX_GetSHMAddr();
    *u32VsyncSHMOffset = HAL_MVD_GetVsyncAddrOffset();
}

//------------------------------------------------------------------------------
/// Set mvd/mpeg4 time_inc predict parameter...
/// @param u32Para \b IN :
///          -Arg0(enable), 1 for enable, default is 0.
///          -Arg1(vop_time_incr_predict_count), 1 for predict once, 2 for twice...0xff for always guess the vop_time_incr even with the vol_header, default is 0...
///          -Arg2(vop_time_incr_follow_vol_header), // 0 for follow vol_header...1 for bypass vol_header, default is 0...
///          -Arg3, reserve...
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetTimeIncPredictParam(MS_U32 u32Id,MS_U32 u32Para)
{
#if defined(CHIP_T2) || defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetTimeIncPredictParam(u8HalIdx,u32Para);
#endif
}

MS_BOOL MDrv_MVD_SetDcodeTimeoutParam(MS_U32 u32Id,MS_BOOL enable,MS_U32 u32timeout)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetDecodeTimeoutParam(u8HalIdx,enable,u32timeout);
}

MS_BOOL MDrv_MVD_SetFramebufferAutoMode(MS_U32 u32Id,MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return _MVD_SetDynamicAllocateFB(u8HalIdx,bEnable);
}

//------------------------------------------------------------------------------
/// set smooth rewind cmd to fw
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_Set_Smooth_Rewind(MS_U32 u32Id, MS_U8 btype)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_Set_Smooth_Rewind(u8HalIdx, btype);
}

//------------------------------------------------------------------------------
/// get mvd is alive or not
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------

E_MVD_Result MDrv_MVD_IsAlive(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_IsAlive(u8HalIdx);
}

//------------------------------------------------------------------------------
/// enable/disable and set error tolerance value
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_Set_Err_Tolerance(MS_U32 u32Id, MS_U16 u16Para)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_Set_Err_Tolerance(u8HalIdx,u16Para);
}


//------------------------------------------------------------------------------
/// enable/disable Auto insert dummy pattern in SLQ mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
void MDrv_MVD_EnableAutoInsertDummyPattern(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_EnableAutoInsertDummyPattern(u8HalIdx,bEnable);
}

void MDrv_MVD_Drop_One_PTS(MS_U32 u32Id)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_Drop_One_PTS(u8HalIdx);
}
E_MVD_Result MDrv_MVD_PVR_Seamless_mode(MS_U32 u32Id, MS_U8 u8Arg)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_PVR_Seamless_mode(u8HalIdx,u8Arg);
}

// Please set it before cmd "CMD_ENABLE_LAST_FRAME_SHOW"
// 0(default): for original just assert the last_frame_show_done when push to dispQ
// 1: for strict qualify the last_frame_show_done after the last_frame been displayed by mvop, 20120309
E_MVD_Result MDrv_MVD_SetDispFinishMode(MS_U32 u32Id,MS_U8 u8Mode)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    return HAL_MVD_SetDisplayFinishMode(u8HalIdx,u8Mode);
}

#endif
