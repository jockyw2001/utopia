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
/// file    drvHVD_EX.c
/// @brief  HVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "drvHVD_Common.h"
#include "drvHVD_EX.h"
#include "drvHVD_def.h"

#include "fwHVD_if.h"
#include "halVPU_EX.h"
#include "halHVD_EX.h"
#include "drvSYS.h"

#if HVD_ENABLE_AUTO_SET_REG_BASE
#include "drvMMIO.h"
#endif

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
//#define SUPPORT_X_MODEL_FEATURE
//#define SUPPORT_X_MODEL_HVD_FEATURE //there is a side effect for CB when enable this define.should find the root cause then enable it
#define DISABLE_ISR_DETACH

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// Drv memory usage
#if defined(REDLION_LINUX_KERNEL_ENVI)
#define HVD_DTV_VIDEO_DELAY         0   // ms
#else
#define HVD_DTV_VIDEO_DELAY         0   // ms
#endif
#define HVD_FW_CMD_TIMEOUT_DEFAULT      100     // ms
#define HVD_FW_EXIT_ACTION_TIMEOUT      3500    // ms

#define HVD_MAX_STREAMS         2

#define HVD_CTRL_INIT_FINISHED          BIT(0)  // deflaut: 0
#define HVD_CTRL_PROCESSING             BIT(1)  // deflaut: 0
#define HVD_CTRL_DATA_END               BIT(2)  // deflaut: 0
#define HVD_CTRL_DISPLAY_CTRL           BIT(3)  // deflaut: 0
#define HVD_CTRL_DISP_INFO_RDY          BIT(4)  // deflaut: 0

#define HVD_CTRL_DISP_OUTSIDE           BIT(5)//BIT(9)  // deflaut: 0
typedef enum
{
    E_HVD_CHECK_CMD_NONE = 0,   //decode -> decode_finish
    E_HVD_CHECK_CMD_INIT,
    E_HVD_CHECK_CMD_TRIGGER_DISP,
    E_HVD_CHECK_CMD_SEEK2PTS,
    E_HVD_CHECK_CMD_MAX,
} HVD_Check_Cmd;

#define HVD_MIU_PROTECT_HVD     BIT(0)
#define HVD_MIU_PROTECT_MVD     BIT(1)
#define HVD_MIU_PROTECT_VPU     BIT(2)

#define _DRV_HVD_EX_Entry(u8DrvId)                                     \
    do                                                              \
    {                                                               \
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode |= HVD_CTRL_PROCESSING;    \
    } while (0)

#define _DRV_HVD_EX_RET(u8DrvId, _ret_)                             \
    do                                                              \
    {                                                               \
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode &= (~HVD_CTRL_PROCESSING); \
        return (_ret_);                                             \
    } while (0)

#define _DRV_HVD_Inited(u8DrvId,_ret_)  \
    do                          \
    {                           \
        if (!(pHVDDrvContext->bHVDIsInited[u8DrvId]))      \
        {                       \
            return _ret_;       \
        }                       \
    } while (0)

#define _DRV_HVD_Rsting(u8DrvId,_ret_)  \
    do                          \
    {                           \
        if (pHVDDrvContext->bHVDIsIniting[u8DrvId])      \
        {                       \
            return _ret_;       \
        }                       \
    } while (0)

#define _DRV_HVD_Ctrl(u8DrvId, x)    ( pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & (x) )

#define _DRV_HVD_SetCtrl(u8DrvId, x)                \
    do                                              \
    {                                               \
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode |= (x);    \
    } while (0)

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Functions Prototype
//-------------------------------------------------------------------------------------------------
static void            _HVD_EX_SetShareInfoAddr(MS_U32 u32Id);
static HVD_EX_Result   _HVD_EX_InitVariables(MS_U32 u32Id, HVD_EX_MemCfg *pStMemCfg, HVD_Init_Params *pStInitParams);
static HVD_EX_Result   _HVD_EX_Check_Cmd(MS_U32 u32Id, HVD_Check_Cmd eCmd);
static HVD_EX_Result   _HVD_EX_InitRegCPU(MS_U32 u32Id);
static HVD_EX_Result   _HVD_EX_SetSyncMode(MS_U32 u32Id, HVD_Sync_Tbl_Type eMode);
static HVD_EX_Result   _HVD_EX_InitFW_AVC(MS_U32 u32Id);
static HVD_EX_Result   _HVD_EX_InitFW_RM(MS_U32 u32Id);
static HVD_EX_Result   _HVD_EX_CheckFWVersion(MS_U32 u32Id);
static HVD_EX_Result   _HVD_EX_InitFW(MS_U32 u32Id);
static MS_BOOL      _HVD_EX_IsAllBufferEmpty(MS_U32 u32Id);
static MS_BOOL      _HVD_EX_IsAllBufferEmpty_VP8(MS_U32 u32Id);
static MS_U8        _HVD_EX_GetDrvId(MS_U32 u32Id);

#if defined(REDLION_LINUX_KERNEL_ENVI)
static MS_S32 _HVD_EX_ISRHandler(void);
#else
static void _HVD_EX_ISRHandler(void);
#endif

static MS_U32 _HVD_EX_ReportLow32BitPTS(MS_U32 u32Id, MS_U32 u32PTS);
static MS_U32 _HVD_EX_Map2HVDErrCode(MS_U32 u32ErrCode);
static MS_U32 _HVD_EX_Map2HVDESBufStatus(MS_U32 u32ESBufStatus);

#if 0
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 u32UartCtrl = (E_HVD_UART_CTRL_ERR | E_HVD_UART_CTRL_MUST);
MS_U32 u32InitSysTimeBase = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_hvd_version =
{
    .DDI = {HVD_DRV_VERSION,},
};
static MS_BOOL bHVDIsInited;
static MS_BOOL bHVDIsIniting;
static HVD_EX_DrvInfo DrvInfo;
static HVD_EX_Drv_Ctrl gHVDCtrl_EX[HVD_MAX_STREAMS] =
{
    {   .bUsed              = FALSE,
    },
    {   .bUsed              = FALSE,
    },
};

static MS_BOOL bHVDIsrAttached = FALSE;
#endif
MS_U32 u32UartCtrl = (E_HVD_UART_CTRL_ERR | E_HVD_UART_CTRL_MUST);

typedef struct
{
    MS_U32 u32InitSysTimeBase[HVD_MAX_STREAMS];
    MS_BOOL bHVDIsInited[HVD_MAX_STREAMS];
    MS_BOOL bHVDIsIniting[HVD_MAX_STREAMS];
    HVD_EX_Drv_Ctrl gHVDCtrl_EX[HVD_MAX_STREAMS];
    MS_BOOL bHVDIsIsrAttached;  //check HVD isr already attached, avoid HVD+HVD re-attached
    MS_BOOL bEVDIsIsrAttached;  //check EVD isr already attached, avoid HVD+HVD re-attached
    MS_BOOL bHVDDisableISRFlag;
    MS_BOOL bEVDDisableISRFlag;
    MS_BOOL bVPUIsSecureMode;
    //pre_set
    HVD_Pre_Ctrl gHVDPreCtrl[HVD_MAX_STREAMS];
} HVD_Drv_CTX;

//global variables
HVD_Drv_CTX* pHVDDrvContext = NULL;
HVD_Drv_CTX gHVDDrvContext;
MSIF_Version _drv_hvd_version =
{
    .DDI = {HVD_DRV_VERSION,},
};
HVD_EX_DrvInfo DrvInfo;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _HVD_EX_Context_Init(void)
{
    pHVDDrvContext->bHVDIsIsrAttached = FALSE;
    pHVDDrvContext->bEVDIsIsrAttached = FALSE;
    return;
}

static void _HVD_EX_SetShareInfoAddr(MS_U32 u32Id)
{
    MS_VIRT u32ShmAddr = HAL_VPU_EX_GetSHMAddr();

    HAL_VPU_EX_SetShareInfoAddr(u32Id, u32ShmAddr);

    return;
}

static HVD_EX_Result _HVD_EX_InitVariables(MS_U32 u32Id, HVD_EX_MemCfg *pStMemCfg, HVD_Init_Params *pStInitParams)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    memset((void *) pCtrl, 0, sizeof(HVD_EX_Drv_Ctrl));
    pCtrl->bFrmRateSupported = TRUE;

    // PreSetControl
    HAL_HVD_EX_SetPreCtrlVariables(u32Id,(MS_VIRT)(&pHVDDrvContext->gHVDPreCtrl[u8DrvId]));

    if (sizeof(HVD_Init_Params) == sizeof(HVD_EX_InitSettings))
    {
        HVD_memcpy((void *) &pCtrl->InitParams, pStInitParams, sizeof(HVD_Init_Params));
    }
    else
    {
        HVD_EX_MSG_ERR("HVD struct define is diff: HVD_Init_Params(%u) vs HVD_Init_Settings(%u)\n",
                    (MS_U32) sizeof(HVD_Init_Params), (MS_U32) sizeof(HVD_EX_InitSettings));
        return E_HVD_EX_FAIL;
    }

    // fill memory
    if (pStMemCfg->u32MIU1BaseAddr == 0)
    {
    // Get physical address from MIU selection= E_CHIP_MIU_2 and offset=0x10000000
        _miu_offset_to_phy(E_CHIP_MIU_1, 0, pCtrl->MemMap.u32MIU1BaseAddr);
        _miu_offset_to_phy(E_CHIP_MIU_2, 0, pCtrl->MemMap.u32MIU2BaseAddr);

    }
    else
    {
        pCtrl->MemMap.u32MIU1BaseAddr = pStMemCfg->u32MIU1BaseAddr; /// TODO : pStMemCfg need member u32MIU2BaseAddr
    }

    pCtrl->MemMap.eFWSourceType = (HVD_FWInputSourceType) pStMemCfg->eFWSourceType;
    pCtrl->MemMap.u32FWBinaryVAddr = pStMemCfg->u32FWBinaryVAddr;
    pCtrl->MemMap.u32FWBinaryAddr = (MS_U32) pStMemCfg->u32FWBinaryAddr;
    pCtrl->MemMap.u32FWBinarySize = pStMemCfg->u32FWBinarySize;
    pCtrl->MemMap.u32VLCBinaryVAddr = pStMemCfg->u32VLCBinaryVAddr;
    pCtrl->MemMap.u32VLCBinaryAddr = (MS_U32) pStMemCfg->u32VLCBinaryAddr;
    pCtrl->MemMap.u32VLCBinarySize = pStMemCfg->u32VLCBinarySize;
    pCtrl->MemMap.u32CodeBufVAddr = pStMemCfg->u32CodeBufVAddr;
    pCtrl->MemMap.u32CodeBufAddr = (MS_U32) pStMemCfg->u32CodeBufAddr;
    pCtrl->MemMap.u32CodeBufSize = pStMemCfg->u32CodeBufSize;
    pCtrl->MemMap.u32FrameBufVAddr = pStMemCfg->u32FrameBufVAddr;
    pCtrl->MemMap.u32FrameBufAddr = (MS_U32) pStMemCfg->u32FrameBufAddr;
    pCtrl->MemMap.u32FrameBufSize = pStMemCfg->u32FrameBufSize;

#if HVD_ENABLE_MVC
    if (((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC) &&
        ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_DUAL_ES_MASK) == HVD_INIT_DUAL_ES_ENABLE))
    {
        HVD_EX_Drv_Ctrl *pDrvCtrl_sub = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1]);
        memset((void *) pDrvCtrl_sub, 0, sizeof(HVD_EX_Drv_Ctrl));

        /// Copy to sub view ctrl
        HVD_memcpy((void *) &pDrvCtrl_sub->InitParams, pStInitParams, sizeof(HVD_Init_Params));

        // fill memory
        if (pStMemCfg->u32MIU1BaseAddr == 0)
        {
            _miu_offset_to_phy(E_CHIP_MIU_1, 0, pDrvCtrl_sub->MemMap.u32MIU1BaseAddr);
            _miu_offset_to_phy(E_CHIP_MIU_2, 0, pDrvCtrl_sub->MemMap.u32MIU2BaseAddr);
        }
        else
        {
            pDrvCtrl_sub->MemMap.u32MIU1BaseAddr = pStMemCfg->u32MIU1BaseAddr; /// TODO : pStMemCfg need member u32MIU2BaseAddr
        }

        pCtrl->MemMap.u32BitstreamBufVAddr= pStMemCfg->u32BitstreamBufVAddr;
        pCtrl->MemMap.u32BitstreamBufAddr = (MS_U32) pStMemCfg->u32BitstreamBufAddr ;
        #if 1
        pCtrl->MemMap.u32BitstreamBufSize = pStMemCfg->u32BitstreamBufSize/2 ;
        pDrvCtrl_sub->MemMap.u32BitstreamBufVAddr = pStMemCfg->u32BitstreamBufVAddr + pStMemCfg->u32BitstreamBufSize/2;
        pDrvCtrl_sub->MemMap.u32BitstreamBufAddr  = (MS_U32) pStMemCfg->u32BitstreamBufAddr + pStMemCfg->u32BitstreamBufSize/2;
        pDrvCtrl_sub->MemMap.u32BitstreamBufSize  = pStMemCfg->u32BitstreamBufSize/2 ;
        #else
        pCtrl->MemMap.u32BitstreamBufSize = (pStMemCfg->u32BitstreamBufSize*57/128) ;
        pDrvCtrl_sub->MemMap.u32BitstreamBufVAddr = pStMemCfg->u32BitstreamBufVAddr + (pStMemCfg->u32BitstreamBufSize*57/128);
        pDrvCtrl_sub->MemMap.u32BitstreamBufAddr  = (MS_U32) pStMemCfg->u32BitstreamBufAddr + (pStMemCfg->u32BitstreamBufSize*57/128);
        pDrvCtrl_sub->MemMap.u32BitstreamBufSize  = (pStMemCfg->u32BitstreamBufSize*71/128) ;
        #endif

        pCtrl->MemMap.u32DrvProcessBufVAddr = pDrvCtrl_sub->MemMap.u32DrvProcessBufVAddr = pStMemCfg->u32DrvProcessBufVAddr;
        pCtrl->MemMap.u32DrvProcessBufAddr  = pDrvCtrl_sub->MemMap.u32DrvProcessBufAddr  = (MS_U32) pStMemCfg->u32DrvProcessBufAddr;
        pCtrl->MemMap.u32DrvProcessBufSize  = pDrvCtrl_sub->MemMap.u32DrvProcessBufSize  = pStMemCfg->u32DrvProcessBufSize;

        pDrvCtrl_sub->MemMap.u32CodeBufVAddr = pStMemCfg->u32CodeBufVAddr;
        pDrvCtrl_sub->MemMap.u32CodeBufAddr = (MS_U32) pStMemCfg->u32CodeBufAddr;
        pDrvCtrl_sub->MemMap.u32CodeBufSize = pStMemCfg->u32CodeBufSize;

        HVD_EX_MSG_INF("[MVC] Bitstream buffer: 1st = [0x%lx, 0x%lx, 0x%lx], 2nd = [0x%lx, 0x%lx, 0x%lx].\n",
        pCtrl->MemMap.u32BitstreamBufVAddr,
        pCtrl->MemMap.u32BitstreamBufAddr,
        pCtrl->MemMap.u32BitstreamBufSize,
        pDrvCtrl_sub->MemMap.u32BitstreamBufVAddr,
        pDrvCtrl_sub->MemMap.u32BitstreamBufAddr,
        pDrvCtrl_sub->MemMap.u32BitstreamBufSize);
    }
    else
#endif
    {
        pCtrl->MemMap.u32BitstreamBufVAddr = pStMemCfg->u32BitstreamBufVAddr;
        pCtrl->MemMap.u32BitstreamBufAddr = (MS_U32) pStMemCfg->u32BitstreamBufAddr;
        pCtrl->MemMap.u32BitstreamBufSize = pStMemCfg->u32BitstreamBufSize;
        pCtrl->MemMap.u32DrvProcessBufVAddr = pStMemCfg->u32DrvProcessBufVAddr;
        pCtrl->MemMap.u32DrvProcessBufAddr = (MS_U32) pStMemCfg->u32DrvProcessBufAddr;
        pCtrl->MemMap.u32DrvProcessBufSize = pStMemCfg->u32DrvProcessBufSize;
    }

    HVD_EX_MSG_INF("HVD mmap: MIU1base:0x%lx, MIU2base:0x%lx,FW(type:%d addr=0x%lx size=0x%lx), VLC(addr=0x%lx size=0x%lx), Code(addr=0x%lx size=0x%lx), FB(addr=0x%lx size=0x%lx), Bit(addr=0x%lx size=0x%lx), Drv(addr=0x%lx size=0x%lx)\n",
         pCtrl->MemMap.u32MIU1BaseAddr,
         pCtrl->MemMap.u32MIU2BaseAddr,
         pCtrl->MemMap.eFWSourceType, pCtrl->MemMap.u32FWBinaryAddr, pCtrl->MemMap.u32FWBinarySize,
         pCtrl->MemMap.u32VLCBinaryAddr, pCtrl->MemMap.u32VLCBinarySize,
         pCtrl->MemMap.u32CodeBufAddr, pCtrl->MemMap.u32CodeBufSize,
         pCtrl->MemMap.u32FrameBufAddr, pCtrl->MemMap.u32FrameBufSize,
         pCtrl->MemMap.u32BitstreamBufAddr, pCtrl->MemMap.u32BitstreamBufSize,
         pCtrl->MemMap.u32DrvProcessBufAddr, pCtrl->MemMap.u32DrvProcessBufSize);

    pCtrl->InitParams.u16DecoderClock = HAL_HVD_EX_GetCorretClock(pStInitParams->u16DecoderClock);
    pCtrl->InitParams.u16ChipECONum = (MS_U16)MDrv_SYS_GetChipRev();


    HVD_EX_MSG_INF("[VDEC]pCtrl->InitParams.u16ChipECONum : %d \n ", pCtrl->InitParams.u16ChipECONum);

    MS_U32 i;
    pCtrl->CloseCaptionInfo.u8ParsingStatus = 0xff;
    pCtrl->CloseCaptionInfo.u8BufMiuSel[0] = pCtrl->CloseCaptionInfo.u8BufMiuSel[1] = E_CHIP_MIU_0;
    pCtrl->CloseCaptionInfo.b708Enable = FALSE;
    pCtrl->CloseCaptionInfo.u32FWUsrDataRIdx = pCtrl->CloseCaptionInfo.u32FWUsrDataWIdx = 0;
    pCtrl->CloseCaptionInfo.u32PktLen708 = 0;
    pCtrl->CloseCaptionInfo.u32PktHdrAddr708 = 0;


    for (i=0;i<2;i++)
    {
        pCtrl->CloseCaptionInfo.u32RingBufStartPAddr[i] = 0;
        pCtrl->CloseCaptionInfo.u32RingBufLen[i] = 0;
        pCtrl->CloseCaptionInfo.u32RingBufVacancy[i] = 0;
        pCtrl->CloseCaptionInfo.u32RingBufRPAddr[i] = 0;
        pCtrl->CloseCaptionInfo.u32RingBufWPAddr[i] = 0;
        pCtrl->CloseCaptionInfo.bOverFlow[i] = FALSE;
    }

    pCtrl->CloseCaptionInfo.b608InfoEnhance = FALSE;

#if HVD_ENABLE_STOP_ACCESS_OVER_256
    pCtrl->bCannotAccessMIU256 = TRUE;
#endif

    pCtrl->bAutoRmLastZeroByte = TRUE;
    pCtrl->u32CmdTimeout = HVD_FW_CMD_TIMEOUT_DEFAULT;

    if (pStInitParams->u8TurboInit & E_HVD_EX_TURBOINIT_FW_RELOAD)
    {
        pCtrl->bTurboFWMode = TRUE;
    }

    HAL_HVD_EX_SetMiuBurstLevel(pCtrl, E_HVD_BURST_CNT_DISABLE);

    pCtrl->u32FlushRstPtr = 0;

    return E_HVD_EX_OK;
}

static HVD_EX_Result _HVD_EX_Check_Cmd(MS_U32 u32Id, HVD_Check_Cmd eCmd)
{
    MS_U32 u32Tmp0 = 0;
    MS_U32 u32Tmp1 = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    switch (eCmd)
    {
        case E_HVD_CHECK_CMD_INIT:
        {
            if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_CHECK))
            {
                // Check mode
                u32Tmp0 = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK;

                if (!MDrv_HVD_EX_GetCaps((HVD_EX_Codec) u32Tmp0))
                {
                    HVD_EX_MSG_ERR("Init: HW not support type:%d\n", eCmd);
                    return E_HVD_EX_RET_UNSUPPORTED;
                }

                u32Tmp0 = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;

                if (u32Tmp0 == HVD_INIT_MAIN_LIVE_STREAM)
                {
                    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK) == HVD_INIT_INPUT_DRV)
                    {
                        HVD_EX_MSG_ERR("Init: Live stream mode can not use Drv input\n");
                        return E_HVD_EX_RET_INVALID_PARAMETER;
                    }
                    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & (HVD_INIT_UTOPIA_ENVI | HVD_INIT_DBG_FW)))
                    {
                        // need not to check this under debug mode
                        if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_START_CODE_MASK) ==
                            HVD_INIT_START_CODE_REMOVED)
                        {
                            HVD_EX_MSG_ERR("Init: Live stream mode must have start code\n");
                            return E_HVD_EX_RET_INVALID_PARAMETER;
                        }
                        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_NON)
                        {
                            HVD_EX_MSG_ERR("Init: Live stream mode can not use none ATS sync mode\n");
                            return E_HVD_EX_RET_INVALID_PARAMETER;
                        }
                    }
                }
                else if ((u32Tmp0 == HVD_INIT_MAIN_FILE_RAW) || (u32Tmp0 == HVD_INIT_MAIN_FILE_TS))
                {
                    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & (HVD_INIT_UTOPIA_ENVI | HVD_INIT_DBG_FW)))
                    {
                        if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_DTS)
                            && (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_STS)
                            && (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_PTS)
                            && (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType != E_HVD_SYNC_TBL_TYPE_NON))
                        {
                            HVD_EX_MSG_ERR("Init: sync mode is not set. use default value:%d\n",
                                        (MS_U16) E_HVD_SYNC_TBL_TYPE_PTS);
                            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8SyncType = E_HVD_SYNC_TBL_TYPE_PTS;
                        }
                    }
                }
                else if ((u32Tmp0 != HVD_INIT_MAIN_FILE_RAW) && (u32Tmp0 != HVD_INIT_MAIN_FILE_TS))
                {
                    HVD_EX_MSG_ERR("Init: main type can not be recognized:%ld\n", u32Tmp0);
                    return E_HVD_EX_RET_INVALID_PARAMETER;
                }

                // check memory map
                if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr == 0)
                {
                    HVD_EX_MSG_ERR("Init: MIU 1 Base addr should not be zero\n");
                    return E_HVD_EX_RET_INVALID_PARAMETER;
                }

                if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr & 0x7FF) != 0)
                {
                    HVD_EX_MSG_ERR("Init: Error HVD code address(0x%lx) must 2048 byte alignment\n",
                                pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr);
                    return E_HVD_EX_RET_INVALID_PARAMETER;
                }
            }

            u32Tmp0 = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;
            u32Tmp1 = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK;

            if (u32Tmp0 != HVD_INIT_MAIN_LIVE_STREAM)
            {
                if (u32Tmp1 == HVD_INIT_INPUT_DRV)
                {
                    // check if driver process buffer is in bitstream buffer.
                    MS_U32 tmpAddr = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufAddr;

                    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufAddr == 0)
                    {
                        HVD_EX_MSG_ERR("Init: Drv process buffer address should not be zero\n");
                        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bNoDrvProccBuf = TRUE;
                        //return E_HVD_EX_RET_INVALID_PARAMETER;
                    }

                    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr > tmpAddr)
                        || (tmpAddr >
                            (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr +
                             pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufSize)))
                    {
                        HVD_EX_MSG_ERR
                            ("Init: The driver process buffer shall be located in the bitstream buffer under file mode(TS or MP4) and Drv input.\n");
                        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bNoDrvProccBuf = TRUE;
                        //return E_HVD_EX_RET_INVALID_PARAMETER;
                    }

                    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufSize
                        <
                        (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) << 3) +
                         8))
                    {
                        HVD_EX_MSG_ERR
                            ("Init: File mode(TS or MP4) and Drv input must set the process buffer size and must be larger than %lu Bytes:%ld\n",
                             pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufSize,
                             (MS_U32) (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF +
                                       (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) << 3) + 8));
                        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bNoDrvProccBuf = TRUE;
                        //return E_HVD_EX_RET_OUTOF_MEMORY;
                    }

                    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufSize > 1)
                    {
                        tmpAddr =
                            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufAddr +
                            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufSize - 1;
                    }
                    else
                    {
                        tmpAddr = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32DrvProcessBufAddr;
                    }

                    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr > tmpAddr)
                        || (tmpAddr >
                            (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr +
                             pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufSize)))
                    {
                        HVD_EX_MSG_ERR
                            ("Init: The driver process buffer shall be located in the bitstream buffer under file mode(TS or PS) and Drv input.\n");
                        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bNoDrvProccBuf = TRUE;
                        //return E_HVD_EX_RET_INVALID_PARAMETER;
                    }
                }
                else
                {
                // TODO: check if the TSP MIU sel is the same with the MIU lacation of process buffer.
                }
            }
            break;
        }
        case E_HVD_CHECK_CMD_SEEK2PTS:
        case E_HVD_CHECK_CMD_TRIGGER_DISP:
        {
#if 0
            if (MDrv_HVD_EX_GetPlayMode(E_HVD_EX_GMODE_IS_SYNC_ON))
            {
                HVD_EX_MSG_ERR("Cmd type:%d Sync Active: %lx\n", (MS_U16) eCmd,
                            (MS_S32) MDrv_HVD_EX_GetPlayMode(E_HVD_EX_GMODE_IS_SYNC_ON));
                return E_HVD_EX_RET_ILLEGAL_ACCESS;
            }
#endif
            break;
        }
        default:
            return E_HVD_EX_OK;
    }

    return E_HVD_EX_OK;
}

static HVD_EX_Result _HVD_EX_InitRegCPU(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    // check MIU select
    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_CHECK))
    {
        HAL_HVD_EX_CheckMIUSel(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_UTOPIA_ENVI);
    }

    return (HVD_EX_Result) HAL_HVD_EX_InitRegCPU(u32Id);
}

static HVD_EX_Result _HVD_EX_RstVariables(MS_U32 u32Id)
{
    //MS_BOOL bBitMIU1 = FALSE;
    //MS_BOOL bCodeMIU1 = FALSE;
    MS_U8 u8BitMiuSel;
    MS_U32 u32BitStartOffset;
    MS_U8 u8CodeMiuSel;
    MS_U32 u32CodeStartOffset;
    MS_U8 u8FrmMiuSel;
    MS_U32 u32FrmStartOffset;
    MS_U8 u8DrvProccMiuSel;
    MS_U32 u32DrvProccStartOffset;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    // Init control flg
    pCtrl->u32CtrlMode = 0;

    #if 1
    _phy_to_miu_offset(u8CodeMiuSel, u32CodeStartOffset, pCtrl->MemMap.u32CodeBufAddr);
    pCtrl->u8CodeMiuSel = u8CodeMiuSel;
    _phy_to_miu_offset(u8BitMiuSel, u32BitStartOffset, pCtrl->MemMap.u32BitstreamBufAddr);
    pCtrl->u8ESMiuSel = u8BitMiuSel;
    _phy_to_miu_offset(u8FrmMiuSel, u32FrmStartOffset, pCtrl->MemMap.u32FrameBufAddr);
    pCtrl->u8FrmMiuSel = u8FrmMiuSel;
    _phy_to_miu_offset(u8DrvProccMiuSel, u32DrvProccStartOffset, pCtrl->MemMap.u32DrvProcessBufAddr);
    pCtrl->u8DrvProccMiuSel = u8DrvProccMiuSel;
    #else
    if (pCtrl->MemMap.u32CodeBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        pCtrl->u32CtrlMode |= HVD_CTRL_CODE_MIU_1;
        bCodeMIU1 = TRUE;
    }

    if (pCtrl->MemMap.u32BitstreamBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        pCtrl->u32CtrlMode |= HVD_CTRL_ES_MIU_1;
        bBitMIU1 = TRUE;
    }

    if (pCtrl->MemMap.u32FrameBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        pCtrl->u32CtrlMode |= HVD_CTRL_FRM_MIU_1;
    }

    if (pCtrl->MemMap.u32DrvProcessBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        pCtrl->u32CtrlMode |= HVD_CTRL_DRVPROCC_MIU_1;
    }
    #endif

    if (!pCtrl->bNoDrvProccBuf)
    {
        // init nal table buffer start address.
        if ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
        {
            if (u8BitMiuSel != u8CodeMiuSel)
            {
#if HVD_ENABLE_MVC
                if( ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC) &&
                    ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_DUAL_ES_MASK) == HVD_INIT_DUAL_ES_ENABLE) &&
                    (pCtrl->MemMap.u32DrvProcessBufSize > (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB)<<1 ) <<3) + 8) ))
                {
                    pCtrl->u32BBUTblInBitstreamBufAddr = HVD_BBU_ST_ADDR_IN_BITSTREAMBUF;
                    if( (( pCtrl->u32BBUTblInBitstreamBufAddr + pCtrl->MemMap.u32DrvProcessBufAddr) % 8) != 0)
                    {
                        pCtrl->u32BBUTblInBitstreamBufAddr = pCtrl->u32BBUTblInBitstreamBufAddr + pCtrl->MemMap.u32DrvProcessBufAddr + 7;
                        pCtrl->u32BBUTblInBitstreamBufAddr -= (pCtrl->u32BBUTblInBitstreamBufAddr ) % 8;
                        pCtrl->u32BBUTblInBitstreamBufAddr -= pCtrl->MemMap.u32DrvProcessBufAddr;
                        HVD_EX_MSG_ERR("Nal table in bitstream buffer start address is not aligned. old:%lx new:%lx" ,  (MS_U32)HVD_BBU_ST_ADDR_IN_BITSTREAMBUF  , pCtrl->u32BBUTblInBitstreamBufAddr  );
                    }

                    /// Setting BBU for MVC dual bbu input
                    HVD_EX_Drv_Ctrl *pDrvCtrl_sub = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1]);
                    pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr = HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) << 3); //// need to check
                    if( (( pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr + pDrvCtrl_sub->MemMap.u32DrvProcessBufAddr) % 8) != 0)
                    {
                        pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr = pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr + pDrvCtrl_sub->MemMap.u32DrvProcessBufAddr + 7;
                        pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr -= (pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr ) % 8;
                        pDrvCtrl_sub->u32BBUTblInBitstreamBufAddr -= pDrvCtrl_sub->MemMap.u32DrvProcessBufAddr;
                        HVD_EX_MSG_ERR("[MVC]: Nal table in bitstream buffer start address is not aligned. old:%lx new:%lx" ,  (MS_U32)HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) << 3)  , pCtrl->u32BBUTblInBitstreamBufAddr  );
                    }

                }
                else
#endif
                if (pCtrl->MemMap.u32DrvProcessBufSize >
                    (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) + 8)))
                {
                    pCtrl->u32BBUTblInBitstreamBufAddr = HVD_BBU_ST_ADDR_IN_BITSTREAMBUF;

                    if (((pCtrl->u32BBUTblInBitstreamBufAddr + pCtrl->MemMap.u32DrvProcessBufAddr) % 8) != 0)
                    {
                        pCtrl->u32BBUTblInBitstreamBufAddr =
                            pCtrl->u32BBUTblInBitstreamBufAddr + pCtrl->MemMap.u32DrvProcessBufAddr + 7;
                        pCtrl->u32BBUTblInBitstreamBufAddr -= (pCtrl->u32BBUTblInBitstreamBufAddr) % 8;
                        pCtrl->u32BBUTblInBitstreamBufAddr -= pCtrl->MemMap.u32DrvProcessBufAddr;
                        HVD_EX_MSG_ERR
                            ("Nal table in bitstream buffer start address is not aligned. old:%lx new:%lx",
                             (MS_U32) HVD_BBU_ST_ADDR_IN_BITSTREAMBUF, pCtrl->u32BBUTblInBitstreamBufAddr);
                    }
                }
                else
                {
                    HVD_EX_MSG_ERR
                        ("Driver process buffer size is not enough for driver input path. input:%lx required:%lx",
                         (MS_U32) pCtrl->MemMap.u32DrvProcessBufSize,
                         (HVD_BBU_ST_ADDR_IN_BITSTREAMBUF + (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB) << 3) +
                          8));
                    return E_HVD_EX_RET_OUTOF_MEMORY;
                }
            }
        }

        // init AVI NULL packet pattern && RM flush pattern
        if ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
        {
            MS_U8 *pNULLPattern = NULL;
            pCtrl->u32NULLPacketAddr = pCtrl->MemMap.u32DrvProcessBufAddr;
            // TODO: use other non-cachable VA addr
            //Use shared memory instead of f/w code buffer.
            pNULLPattern = (MS_U8 *) HAL_HVD_EX_GetShmAddr(u32Id);
            if(pNULLPattern == NULL)
            {
                HVD_EX_MSG_ERR("########## VDEC patch for Debug %s %d###########\n", __FUNCTION__, __LINE__);
                return E_HVD_EX_FAIL;
            }
            memset((void *) pNULLPattern, 0, 12);

            switch ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
            {
                case HVD_INIT_HW_AVC:
                case HVD_INIT_HW_AVS:
                case HVD_INIT_HW_MVC:
            #if SUPPORT_EVD
                case HVD_INIT_HW_VP9: // MStar VP9 or G2 VP9
                case HVD_INIT_HW_HEVC:
            #endif
                {
                    if (pCtrl->InitParams.u32ModeFlag & HVD_INIT_START_CODE_MASK)
                    {
                        // start code removed
                        pCtrl->u32NULLPacketSize = 8;

                        if (pCtrl->MemMap.u32DrvProcessBufSize < pCtrl->u32NULLPacketSize)
                        {
                            return E_HVD_EX_RET_OUTOF_MEMORY;
                        }

                        memset((void *) pNULLPattern, 0xAA, pCtrl->u32NULLPacketSize);
                        pNULLPattern[4] = 0X55;
                    }
                    else            // start code remained
                    {
                        pCtrl->u32NULLPacketSize = 12;

                        if (pCtrl->MemMap.u32DrvProcessBufSize < pCtrl->u32NULLPacketSize)
                        {
                            return E_HVD_EX_RET_OUTOF_MEMORY;
                        }

                        memset((void *) pNULLPattern, 0xAA, pCtrl->u32NULLPacketSize);

                        pNULLPattern[0] = 0;
                        pNULLPattern[1] = 0;
                        pNULLPattern[2] = 1;
                        pNULLPattern[3] = 0xFF;
                        pNULLPattern[8] = 0X55;
                    }

                    //if (bBitMIU1 != bCodeMIU1)
                    if(u8BitMiuSel != u8CodeMiuSel)
                    {
#if HVD_ENABLE_BDMA_2_BITSTREAMBUF
                        HAL_HVD_EX_FlushMemory();
                        HVD_dmacpy(pCtrl->MemMap.u32DrvProcessBufAddr, pCtrl->MemMap.u32CodeBufAddr,
                                   pCtrl->u32NULLPacketSize);
#else
                        HVD_memcpy((void *) (pCtrl->MemMap.u32DrvProcessBufVAddr), pNULLPattern,
                                   pCtrl->u32NULLPacketSize);
                        HAL_HVD_EX_FlushMemory();
#endif
                    }
                    else
                    {
                        HVD_memcpy((void *) (pCtrl->MemMap.u32DrvProcessBufVAddr), pNULLPattern,
                                   pCtrl->u32NULLPacketSize);
                        HAL_HVD_EX_FlushMemory();
                    }

                    break;
                }
                case HVD_INIT_HW_RM:
                {
                    // RM has no NULL packet
                    pCtrl->u32NULLPacketSize = 0;
                    pCtrl->u32NULLPacketAddr = 0;
#if HVD_ENABLE_RV_FEATURE
                    pCtrl->u32RV_FlushPacketAddr = pCtrl->MemMap.u32DrvProcessBufAddr;
                    pCtrl->u32RV_FlushPacketSize = 8;

                    if (pCtrl->MemMap.u32DrvProcessBufSize < pCtrl->u32RV_FlushPacketSize)
                    {
                        return E_HVD_EX_RET_OUTOF_MEMORY;
                    }
                    memset((void *) pNULLPattern, 0xFF, pCtrl->u32RV_FlushPacketSize);
                    {
#if HVD_ENABLE_BDMA_2_BITSTREAMBUF
                        HAL_HVD_EX_FlushMemory();
                        HVD_dmacpy(pCtrl->MemMap.u32DrvProcessBufAddr, pCtrl->MemMap.u32CodeBufAddr,
                                   pCtrl->u32RV_FlushPacketSize);
#else
                        HVD_memcpy((void *) (pCtrl->MemMap.u32DrvProcessBufVAddr), pNULLPattern,
                                   pCtrl->u32RV_FlushPacketSize);
                        HAL_HVD_EX_FlushMemory();
#endif
                    }
#endif
                    break;
                }
                default:
                    pCtrl->u32NULLPacketSize = 0;
                    pCtrl->u32NULLPacketAddr = 0;
                    break;
            }
        }
        else
        {
            pCtrl->u32NULLPacketSize = 0;
            pCtrl->u32NULLPacketAddr = 0;
        }
    }

    // reset other driver control variables
    pCtrl->u32StepDecodeCnt = 0;
    pCtrl->u32LastESRptr = 0;
    pCtrl->u32BBUPacketCnt = 0;
    pCtrl->u32BBUWptr_Fired = 0;
    pCtrl->u32LastErrCode = 0;
    pCtrl->bIsDispInfoChg = 0;

    memset((void *) &(pCtrl->LastNal), 0, sizeof(HVD_Nal_Entry));
    memset((void *) &(pCtrl->LivingStatus), 0, sizeof(HVD_Alive_Status));

#if HVD_ENABLE_MVC
    if((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC)
    {
        /// TODO: MVC want to used 2nd HVDCtrl data.
        //u8DrvId = _HVD_EX_GetDrvId(u32Id);
        //pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId + 1]);
        HVD_EX_Drv_Ctrl *pDrvCtrl_sub = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1]);

        pDrvCtrl_sub->u32StepDecodeCnt = 0;
        pDrvCtrl_sub->u32LastESRptr = 0;
        pDrvCtrl_sub->u32BBUPacketCnt = 0;
        pDrvCtrl_sub->u32BBUWptr_Fired = 0;
        pDrvCtrl_sub->u32LastErrCode = 0;
        pDrvCtrl_sub->bIsDispInfoChg = 0;

        memset((void *) &(pDrvCtrl_sub->LastNal), 0, sizeof(HVD_Nal_Entry));

        pDrvCtrl_sub->bNoDrvProccBuf        = pCtrl->bNoDrvProccBuf;
        pDrvCtrl_sub->bAutoRmLastZeroByte   = pCtrl->bAutoRmLastZeroByte;
        pDrvCtrl_sub->bCannotAccessMIU256   = pCtrl->bCannotAccessMIU256;
        pDrvCtrl_sub->u32CmdTimeout         = pCtrl->u32CmdTimeout;
        pDrvCtrl_sub->u32CtrlMode           = pCtrl->u32CtrlMode;
        pDrvCtrl_sub->u32DummyWriteBuf      = pCtrl->u32DummyWriteBuf;
        pDrvCtrl_sub->u32NULLPacketSize     = pCtrl->u32NULLPacketSize;
        pDrvCtrl_sub->u32NULLPacketAddr     = pCtrl->u32NULLPacketAddr;
    }
#endif
    // Init HAL variables
    return (HVD_EX_Result) HAL_HVD_EX_InitVariables(u32Id);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: _HVD_EX_SetSyncMode()
/// @brief \b Function \b Description:  specify the way to sync video time stamp and STC.
/// @param -eMode \b IN : sync type
/// @param -u32Arg \b IN : only work under (eMode == E_HVD_EX_SYNC_ATS) , video delay toward Audio time stamp. It's minimun value is 0, and maximun value is 4500 ( unit:ms)
/// @return -The result of command set sync type
//-----------------------------------------------------------------------------
static HVD_EX_Result _HVD_EX_SetSyncMode(MS_U32 u32Id, HVD_Sync_Tbl_Type eMode)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_TYPE, eMode);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

static HVD_EX_Result _HVD_EX_InitFW_AVC(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_U32 u32InitMode = pCtrl->InitParams.u32ModeFlag;

    // common settings
    if (!(pCtrl->InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_DISPLAY))
    {
        _HVD_EX_SetSyncMode(u32Id, (HVD_Sync_Tbl_Type) pCtrl->InitParams.u8SyncType);

        if (pCtrl->InitParams.u8MinFrmGap)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_MIN_FRAME_GAP, pCtrl->InitParams.u8MinFrmGap);
        }

        if (pCtrl->InitParams.u32MaxDecTick)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_MAX_DEC_TICK, pCtrl->InitParams.u32MaxDecTick);
        }

        if (pCtrl->InitParams.u16Pitch)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PITCH, pCtrl->InitParams.u16Pitch);
        }

        if (pCtrl->InitParams.bSyncEachFrm)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_EACH_FRM, TRUE);
        }

        if (pCtrl->InitParams.bFastDisplay)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FAST_DISP, TRUE);
        }

        if (pCtrl->InitParams.bDynamicScaling)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DYNAMIC_SCALE, TRUE);
        }

        if (pCtrl->InitParams.bUserData)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DPO_CC, TRUE);
        }
    }

    if (pCtrl->InitParams.u8TimeUnit)
    {
        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_TIME_UNIT_TYPE, pCtrl->InitParams.u8TimeUnit);
    }

    // specific settings
    switch (u32InitMode & HVD_INIT_MAIN_MASK)
    {
        case HVD_INIT_MAIN_FILE_RAW:
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_VDEAD, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_ERR_REF, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_RESET_HW, TRUE);

            if (!(pCtrl->InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_DISPLAY))
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_REPEAT_LAST_FIELD, TRUE);
            }

            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PARSER_BYPASS, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_GET_MORE_FRM_BUF, TRUE);

            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DYNAMIC_SCALE_RESV_N_BUFFER, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_CTRL_SPEED_IN_DISP_ONLY, TRUE);
            break;
        }
        case HVD_INIT_MAIN_FILE_TS:
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_VDEAD, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_ERR_REF, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_RESET_HW, TRUE);

            if (!(pCtrl->InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_DISPLAY))
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_REPEAT_LAST_FIELD, TRUE);
            }

            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ES_FULL_STOP, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AVOID_PTS_TBL_OVERFLOW, TRUE);

            if((u32InitMode & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC)
            {
                HAL_HVD_EX_SpareBandwidth(u32Id);  //// For MVC
            }
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DYNAMIC_SCALE_RESV_N_BUFFER, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_CTRL_SPEED_IN_DISP_ONLY, TRUE);
            break;
        }
        case HVD_INIT_MAIN_LIVE_STREAM:
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_ERR_REF, TRUE);
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_RESET_HW, TRUE);

            if (pCtrl->InitParams.u8TimeUnit)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, (MS_U32) HVD_DTV_VIDEO_DELAY);
            }
            else
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, (MS_U32) HVD_DTV_VIDEO_DELAY * 90);
            }

            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ES_FULL_STOP, TRUE);

            break;
        }
        default:
            break;
    }

    return E_HVD_EX_OK;
}

static HVD_EX_Result _HVD_EX_InitFW_RM(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_U32 u32InitMode = pCtrl->InitParams.u32ModeFlag;

    // common settings
    if (!(pCtrl->InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_DISPLAY))
    {
        _HVD_EX_SetSyncMode(u32Id, (HVD_Sync_Tbl_Type) pCtrl->InitParams.u8SyncType);

        if (pCtrl->InitParams.u16Pitch)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PITCH, pCtrl->InitParams.u16Pitch);
        }

        if (pCtrl->InitParams.bSyncEachFrm)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_EACH_FRM, TRUE);
        }

        if (pCtrl->InitParams.bFastDisplay)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FAST_DISP, TRUE);
        }

        if (pCtrl->InitParams.bDynamicScaling)
        {
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DYNAMIC_SCALE, TRUE);
        }
    }

    if (pCtrl->InitParams.u8TimeUnit)
    {
        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_TIME_UNIT_TYPE, pCtrl->InitParams.u8TimeUnit);
    }
    // specific settings
    switch (u32InitMode & HVD_INIT_MAIN_MASK)
    {
        case HVD_INIT_MAIN_FILE_RAW:
            break;
        case HVD_INIT_MAIN_FILE_TS:
        case HVD_INIT_MAIN_LIVE_STREAM:
        default:
            break;
    }

    // fix FFx4 display error
    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_RESET_HW, TRUE);

    return E_HVD_EX_OK;
}

static HVD_EX_Result _HVD_EX_CheckFWVersion(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    // check FW version ID
    if (!(pCtrl->InitParams.u8TurboInit & E_HVD_EX_TURBOINIT_CHECK))
    {
        if ((HVD_FW_IF_VERSION >> 16) != (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_IF_VERSION_ID) >> 16))
        {
            HVD_EX_MSG_ERR("fw interface and binary is not the same. fw ID: interface:%lx binary:%lx\n",
                        (MS_U32) HVD_FW_IF_VERSION, HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_IF_VERSION_ID));
            return E_HVD_EX_FAIL;
        }
    }

    return E_HVD_EX_OK;
}

static HVD_EX_Result _HVD_EX_InitFW(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    eRet = _HVD_EX_CheckFWVersion(u32Id);

    if (eRet != E_HVD_EX_OK)
    {
        return eRet;
    }

    switch (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK)
    {
        case HVD_INIT_HW_AVC:
        case HVD_INIT_HW_AVS:
        case HVD_INIT_HW_MVC:
     #if SUPPORT_EVD
        case HVD_INIT_HW_VP9: // MStar VP9 or G2 VP9
        case HVD_INIT_HW_HEVC:
     #endif
            eRet = _HVD_EX_InitFW_AVC(u32Id);
            break;
        case HVD_INIT_HW_RM:
            eRet = _HVD_EX_InitFW_RM(u32Id);
            break;
        default:
            break;
    }

    return eRet;
}

static MS_BOOL _HVD_EX_IsAllBufferEmpty(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32PlaybackType = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;
    MS_U32 u32FwIdleThreshold = 5000;
    MS_U32 u8DispCntThreshold = 0;
    if (u32PlaybackType == HVD_INIT_MAIN_FILE_RAW)
    {
        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB) != 0)
        {
            return FALSE;
        }
    }
    else                        //if( u32PlaybackType == HVD_INIT_MAIN_FILE_TS ) TSP input
    {
#if 0
        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR) != HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR))
        {
            return FALSE;
        }
#endif
    }
    if(_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISP_OUTSIDE))
    {
        if ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_Q_NUMB) == 0 && HAL_HVD_EX_DispFrameAllViewed(u32Id)) &&
            ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT) > u32FwIdleThreshold)||((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_FLUSH_STATUS) == E_HVD_EX_FLUSH_DONE) )))
        {
            HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_FLUSH_STATUS,  (MS_U32)E_HVD_EX_FLUSH_NONE);

            return TRUE;
        }

        return FALSE;
    }
    //other Queues
    //LOGD("R: %d %d %d", HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_Q_NUMB), HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_NUMB), HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT));
	if (( (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_Q_NUMB) == 0) && (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_NUMB) == u8DispCntThreshold) ) && 
	    ( (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT) > HVD_FW_IDLE_THRESHOLD) || ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_FLUSH_STATUS) == E_HVD_EX_FLUSH_DONE)) ))	  
    {
        //HVD_PRINT("Flush success, flush_status: %d !!!\n",HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_FLUSH_STATUS));
        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_FLUSH_STATUS,  (MS_U32)E_HVD_EX_FLUSH_NONE);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HVD_EX_IsAllBufferEmpty_VP8(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32PlaybackType = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK;
    MS_U32 u32FwIdleThreshold = 5000;
    MS_U32 u8DispCntThreshold = 0;

    if (u32PlaybackType == HVD_INIT_MAIN_FILE_RAW)
    {
        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB) != 0)
        {
            return FALSE;
        }
    }

#if 0
    if(_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISP_OUTSIDE))
    {
        u32FwIdleThreshold = 5;
    }
#endif
    if(_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISP_OUTSIDE))
    {
        if ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR) == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR) && HAL_HVD_EX_DispFrameAllViewed(u32Id)) &&
            ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT) > u32FwIdleThreshold)||((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_FLUSH_STATUS) == E_HVD_EX_FLUSH_DONE) )))
        {
            HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_FLUSH_STATUS,  (MS_U32)E_HVD_EX_FLUSH_NONE);

            return TRUE;
        }

        return FALSE;
    }
    // other Queues
    if (((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_Q_NUMB) == 0) && (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_NUMB) == u8DispCntThreshold)) &&
        ((HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT) > u32FwIdleThreshold) || (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_FLUSH_STATUS) == E_HVD_EX_FLUSH_DONE) ))
    {
        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_FLUSH_STATUS,  (MS_U32)E_HVD_EX_FLUSH_NONE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if defined(REDLION_LINUX_KERNEL_ENVI)
static MS_S32 _HVD_EX_ISRHandler(void)
{
    if (gHVDISRCtrl.pfnISRCallBack != NULL)
    {
        gHVDISRCtrl.bInISR = TRUE;
        gHVDISRCtrl.u32ISRInfo = HAL_HVD_EX_GetData(E_HVD_GDATA_HVD_ISR_STATUS);

        if (gHVDISRCtrl.u32ISRInfo)
        {
            HAL_HVD_EX_EnableISR(FALSE);
            gHVDISRCtrl.pfnISRCallBack();
            HAL_HVD_EX_EnableISR(TRUE);
        }
    }
    else
    {
        HVD_EX_MSG_DBG("ISR callback is NULL.\n");
    }

    gHVDISRCtrl.bInISR = FALSE;
    HAL_HVD_EX_SetClearISR();
    if(TRUE == OSAL_HVD_ISR_Enable())//enable cpu interrupt mask
    {
        _bHVDDisableISRFlag = FALSE;
    }
    return 1;
}
#else
static void _HVD_EX_ISRHandler(void)
{
    MS_U8 u8Idx;
    HVD_ISRCallBack pfnCb = NULL;
    MS_U8 hvd_index = 0;
    MS_U8 evd_index = 0;
    MS_BOOL hvd_exist = FALSE;
    MS_BOOL evd_exist = FALSE;
 

    //we can attach only one ISR, so we need to check the INT is from which decoder.
    for (u8Idx = 0; u8Idx < HVD_MAX_STREAMS; u8Idx++)
    {
        pfnCb = pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.pfnISRCallBack;

        if(pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.bIsHvdIsr == TRUE)
        {
            hvd_exist = TRUE;
            hvd_index = u8Idx;
        }
        else
        {
            evd_exist = TRUE;
            evd_index = u8Idx;
        }

        if (pfnCb)
        {
            pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.bInISR = TRUE;
            pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.u32ISRInfo =
                HAL_HVD_EX_GetData(pHVDDrvContext->gHVDCtrl_EX[u8Idx].u32Sid, E_HVD_GDATA_HVD_ISR_STATUS);

            if (pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.u32ISRInfo)
            {
            #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
                //make fake stream id to disable/enable ISR.(only for calling HAL_HVD_EX_EnableISR)
                MS_U32 u32tempIdx = 0;
                u32tempIdx |= (u8Idx << 8);
                HAL_HVD_EX_EnableISR(u32tempIdx, FALSE);
                pfnCb(pHVDDrvContext->gHVDCtrl_EX[u8Idx].u32Sid);
                HAL_HVD_EX_EnableISR(u32tempIdx, TRUE);
            #else
                HAL_HVD_EX_EnableISR(FALSE);
                pfnCb(pHVDDrvContext->gHVDCtrl_EX[u8Idx].u32Sid);
                HAL_HVD_EX_EnableISR(TRUE);
            #endif
            }
        }
        else
        {
            HVD_EX_MSG_DBG("ISR callback is NULL.\n");
        }

        pHVDDrvContext->gHVDCtrl_EX[u8Idx].HVDISRCtrl.bInISR = FALSE;
    }

    if(hvd_exist == TRUE)
    {
    #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
        HAL_HVD_EX_SetClearISR(hvd_index);
    #else
        HAL_HVD_EX_SetClearISR();
    #endif
        if(TRUE == OSAL_HVD_ISR_Enable(pHVDDrvContext->gHVDCtrl_EX[hvd_index].HVDISRCtrl.bIsHvdIsr))//enable cpu interrupt mask
        {
            pHVDDrvContext->bHVDDisableISRFlag = FALSE;
        }
    }

    if(evd_exist == TRUE)
    {
    #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
        HAL_HVD_EX_SetClearISR(evd_index);
    #else
        HAL_HVD_EX_SetClearISR();
    #endif
        if(TRUE == OSAL_HVD_ISR_Enable(pHVDDrvContext->gHVDCtrl_EX[evd_index].HVDISRCtrl.bIsHvdIsr))//enable cpu interrupt mask
        {
            pHVDDrvContext->bEVDDisableISRFlag = FALSE;
        }
     }
}
#endif

HVD_EX_Result _HVD_EX_RecoverySettings(MS_U32 u32Id)
{
// TODO: complete this function. and consider more cases. step decoder, display?
    HVD_EX_Result eRst = E_HVD_EX_OK;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    switch (pCtrl->InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK)
    {
        case HVD_INIT_MAIN_LIVE_STREAM:
        {
    // temp solution
            // must before play().
            if ((pCtrl->Settings.DispInfoTH.u32FrmrateLowBound != 0) ||
                (pCtrl->Settings.DispInfoTH.u32FrmrateUpBound != 0) ||
                (pCtrl->Settings.DispInfoTH.u32MvopLowBound != 0) || (pCtrl->Settings.DispInfoTH.u32MvopUpBound != 0))
            {
                HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DISP_INFO_TH, (MS_U32) (&pCtrl->Settings.DispInfoTH));
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISP_THRESHOLD, 0);
            }

            if (pCtrl->Settings.u32IsrEvent)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ISR_TYPE, pCtrl->Settings.u32IsrEvent);
            }

        #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
            HAL_HVD_EX_EnableISR(u32Id, pCtrl->Settings.bEnISR);
        #else
            HAL_HVD_EX_EnableISR(pCtrl->Settings.bEnISR);
        #endif
            // play()
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAY, 0);

            if (pCtrl->Settings.u8SkipMode)
            {
                if (E_HVD_EX_OK != (eRst = MDrv_HVD_EX_SetSkipDecMode(u32Id, (HVD_EX_SkipDecode) (pCtrl->Settings.u8SkipMode))))
                {
                    HVD_EX_MSG_ERR("Set Skip Mode fail!!.\n");
                    return eRst;
                }
            }

            if (pCtrl->Settings.bIsShowErrFrm)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_ERR_FRM, TRUE);
            }

            if (pCtrl->Settings.u8FrcMode)
            {
                if (E_HVD_EX_OK != (eRst = MDrv_HVD_EX_SetFrcMode(u8DrvId, (HVD_EX_FrmRateConvMode) (pCtrl->Settings.u8FrcMode))))
                {
                    HVD_EX_MSG_ERR("Set Frc Mode fail!!.\n");
                    return eRst;
                }
            }

            if (pCtrl->Settings.bIsErrConceal)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ERR_CONCEAL, TRUE);
            }

            if (pCtrl->Settings.bAutoFreeES)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AUTO_FREE_ES, TRUE);
            }

            if (pCtrl->Settings.bDisDeblocking)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_DBF, TRUE);
            }

            if (pCtrl->Settings.bDisQuarterPixel)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_QUART_PIXEL, TRUE);
            }

            if (pCtrl->Settings.bIsSyncOn)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_ACTIVE, TRUE);
            }

            if (pCtrl->Settings.u32SyncTolerance)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_TOLERANCE, pCtrl->Settings.u32SyncTolerance);
            }

            if (pCtrl->Settings.u32SyncRepeatTH)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_THRESHOLD, pCtrl->Settings.u32SyncRepeatTH);
            }

            if (pCtrl->Settings.u32SyncVideoDelay)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, pCtrl->Settings.u32SyncVideoDelay);
            }

            if (pCtrl->Settings.u32SyncFreeRunTH)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FREERUN_THRESHOLD, pCtrl->Settings.u32SyncFreeRunTH);
            }

            if (E_HVD_BURST_CNT_DISABLE != (HVD_MIU_Burst_Cnt_Ctrl) pCtrl->Settings.u32MiuBurstLevel)
            {
                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_MIU_BURST_CNT, pCtrl->Settings.u32MiuBurstLevel);
            }

            //HAL_HVD_EX_SetCmd(E_HVD_CMD_BLUE_SCREEN, FALSE);
            pCtrl->bStepDecoding = 0;

            break;
        }
        case HVD_INIT_MAIN_FILE_RAW:
        default:
            pCtrl->bStepDecoding = 0;
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAY, 0);
            break;
    }

    return eRst;
}

HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle)
{
    HVD_EX_Result eRst = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    if (bErrHandle == TRUE)
    {
        HVD_EX_MSG_ERR("HVD Not supported reset(TRUE) yet\n");
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    pHVDDrvContext->bHVDIsIniting[u8DrvId] = TRUE;

#if (defined(CHIP_EAGLE) || defined(CHIP_EMERALD) || defined(CHIP_EDISON) || defined(CHIP_AGATE) || defined(CHIP_NUGGET) || defined(CHIP_NIKON) || defined(CHIP_MUJI) || defined(CHIP_MONET))
    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_RM)
    {
        HAL_HVD_EX_RM_PATCH(u32Id);
    }
#endif

#if (defined(CHIP_NAPOLI))
    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_RM)
    {
        if(!HAL_HVD_EX_Is_RM_Supported(u32Id))
            return E_HVD_EX_RET_UNSUPPORTED;
    }
#endif


#if defined(HIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    HAL_HVD_EX_EnableISR(u32Id, FALSE);
#else
    HAL_HVD_EX_EnableISR(FALSE);
#endif

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    eRst = _HVD_EX_RstVariables(u32Id);

    if (eRst != E_HVD_EX_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    eRst = (HVD_EX_Result) HAL_HVD_EX_InitShareMem(u32Id);

    if (E_HVD_EX_OK != eRst)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    eRst = _HVD_EX_InitRegCPU(u32Id);

    if (eRst != E_HVD_EX_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    eRst = _HVD_EX_InitFW(u32Id);

    if (eRst != E_HVD_EX_OK)
    {
        goto DRV_HVD_Rst_Failed;
    }

    eRst = E_HVD_EX_OK;

DRV_HVD_Rst_Failed:
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    HAL_HVD_EX_EnableISR(u32Id, pCtrl->Settings.bEnISR);
#else
    HAL_HVD_EX_EnableISR(pCtrl->Settings.bEnISR);
#endif
    pHVDDrvContext->bHVDIsIniting[u8DrvId] = FALSE;

    return eRst;
}

static MS_U8 _HVD_EX_GetDrvId(MS_U32 u32Id)
{
    return (0xFF & (u32Id >> 16));
}

static HVD_EX_Result _HVD_EX_GetDispInfo(MS_U32 u32Id, HVD_EX_DispInfo *pInfo, MS_BOOL bClearSeqChg)
{
    MS_U32 u32Seqtimes = 10;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32DispInfoAddr = 0;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);

    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_DISP_INFO_RDY))
    {
        if (MDrv_HVD_EX_CheckDispInfoRdy(u32Id) == E_HVD_EX_RET_NOTREADY)
        {
            return E_HVD_EX_RET_NOTREADY;
        }
    }

    if (pInfo == NULL)
    {
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    _DRV_HVD_EX_Entry(u8DrvId);

    if (TRUE == bClearSeqChg)
    {
  HVD_GETDISPINFO_START:
        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_GET_DISP_INFO_START, 0);
        u32DispInfoAddr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_INFO_ADDR);
        if(u32DispInfoAddr == 0)
        {
            return E_HVD_EX_RET_NOTREADY;
        }
        HVD_memcpy((void *) pInfo, (void *) u32DispInfoAddr, sizeof(HVD_Display_Info));
        //Check if another SeqChg occurs
        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_DISP_INFO_UNCOPYED))
        {
            u32Seqtimes--;
            if (u32Seqtimes > 0)
            {
                goto HVD_GETDISPINFO_START;
            }
            else
            {
                HVD_EX_MSG_ERR("GetDispInfo Timeout:%d\n",
                            (MS_S16) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_DISP_INFO_UNCOPYED));
                _DRV_HVD_EX_RET(u8DrvId, E_HVD_EX_RET_TIMEOUT);
            }
        }
    }
    else
    {
        u32DispInfoAddr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_INFO_ADDR);
        if(u32DispInfoAddr == 0)
        {
            return E_HVD_EX_RET_NOTREADY;
        }
        HVD_memcpy((void *) pInfo, (void *)u32DispInfoAddr, sizeof(HVD_Display_Info));
    }

    eRet = E_HVD_EX_OK;

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bIsDispInfoChg)
    {
        HVD_EX_MSG_DBG("u32FrameRate=%lu, u8Interlace=%x, u16HorSize=%u, u16VerSize=%u, \
u16Crop R/L=%u/%u, u16Crop B/T=%u/%u, u8AspectRate=%u, u16SarWidth=%u \
u16SarHeight=%u, u16Pitch=%u, u8ColourPrimaries=%u\n",
            pInfo->u32FrameRate,
            pInfo->u8Interlace,
            pInfo->u16HorSize,
            pInfo->u16VerSize,
            pInfo->u16CropRight, pInfo->u16CropLeft,
            pInfo->u16CropBottom, pInfo->u16CropTop,
            pInfo->u8AspectRate,
            pInfo->u16SarWidth,
            pInfo->u16SarHeight,
            pInfo->u16Pitch,
            pInfo->u8ColourPrimaries);
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bIsDispInfoChg = FALSE;
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

static MS_U32 _HVD_EX_ReportLow32BitPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    MS_U32 u32RetPTS = u32PTS;
    MS_U32 u32UpperInMs = (HVD_U32_MAX/90);
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    if(u32RetPTS != HVD_U32_MAX)
    {
        if (pCtrl->InitParams.u8TimeUnit)
        {
            //unit: ms
            if (u32RetPTS > u32UpperInMs)
            {
                u32RetPTS = u32RetPTS - u32UpperInMs;
            }
        }
        else
        {
            //unit: 90KHz, bit 33 is not included in u32PTS, no need to handle
        }
    }
    return u32RetPTS;
}

static MS_U32 _HVD_EX_Map2HVDErrCode(MS_U32 u32ErrCode)
{

    MS_U32 u32Ret=E_HVD_EX_ERRCODE_GENERAL_BASE;


    switch (u32ErrCode)
    {
        case E_HVD_ERR_OUT_OF_SPEC:
            u32Ret = E_HVD_EX_ERRCODE_OUT_OF_SPEC;
            break;
        case E_HVD_ERR_UNKNOW_ERR:
            u32Ret = E_HVD_EX_ERRCODE_UNKNOW_ERR;
            break;
        case E_HVD_ERR_HW_BREAK_DOWN:
            u32Ret = E_HVD_EX_ERRCODE_HW_BREAK_DOWN;
            break;
        case E_HVD_ERR_HW_DEC_TIMEOUT:
            u32Ret = E_HVD_EX_ERRCODE_HW_DEC_TIMEOUT;
            break;
        case E_HVD_ERR_OUT_OF_MEMORY:
            u32Ret = E_HVD_EX_ERRCODE_OUT_OF_MEMORY;
            break;
        case E_HVD_ERR_UNKNOWN_CODEC:
            u32Ret = E_HVD_EX_ERRCODE_UNKNOWN_CODEC;
            break;
        case E_HVD_ERR_AVC_SPS_BROKEN:
            u32Ret = E_HVD_EX_ERRCODE_AVC_SPS_BROKEN;
            break;
        case E_HVD_ERR_AVC_SPS_NOT_IN_SPEC:
            u32Ret = E_HVD_EX_ERRCODE_AVC_SPS_NOT_IN_SPEC;
            break;
        case E_HVD_ERR_AVC_SPS_NOT_ENOUGH_FRM:
            u32Ret = E_HVD_EX_ERRCODE_AVC_SPS_NOT_ENOUGH_FRM;
            break;
        case E_HVD_ERR_AVC_PPS_BROKEN:
            u32Ret = E_HVD_EX_ERRCODE_AVC_PPS_BROKEN;
            break;
        case E_HVD_ERR_AVC_REF_LIST:
            u32Ret = E_HVD_EX_ERRCODE_AVC_REF_LIST;
            break;
        case E_HVD_ERR_AVC_NO_REF:
            u32Ret = E_HVD_EX_ERRCODE_AVC_NO_REF;
            break;
        case E_HVD_ERR_AVC_RES:
            u32Ret = E_HVD_EX_ERRCODE_AVC_RES;
            break;
        case E_HVD_ERR_AVS_RES:
            u32Ret = E_HVD_EX_ERRCODE_AVS_RES;
            break;
        case E_HVD_ERR_RM_PACKET_HEADER:
            u32Ret = E_HVD_EX_ERRCODE_RM_PACKET_HEADER;
            break;
        case E_HVD_ERR_RM_FRAME_HEADER:
            u32Ret = E_HVD_EX_ERRCODE_RM_FRAME_HEADER;
            break;
        case E_HVD_ERR_RM_SLICE_HEADER:
            u32Ret = E_HVD_EX_ERRCODE_RM_SLICE_HEADER;
            break;
        case E_HVD_ERR_RM_BYTE_CNT:
            u32Ret = E_HVD_EX_ERRCODE_RM_BYTE_CNT;
            break;
        case E_HVD_ERR_RM_DISP_TIMEOUT:
            u32Ret = E_HVD_EX_ERRCODE_RM_DISP_TIMEOUT;
            break;
        case E_HVD_ERR_RM_NO_REF:
            u32Ret = E_HVD_EX_ERRCODE_RM_NO_REF;
            break;
        case E_HVD_ERR_RM_RES:
            u32Ret = E_HVD_EX_ERRCODE_RM_RES;
            break;
        case E_HVD_ERR_RM_VLC:
            u32Ret = E_HVD_EX_ERRCODE_RM_VLC;
            break;
        case E_HVD_ERR_RM_SIZE_OUT_FB_LAYOUT:
            u32Ret = E_HVD_EX_ERRCODE_RM_SIZE_OUT_FB_LAYOUT;
            break;
        default:
            break;
    }
    return u32Ret;
}

static MS_U32 _HVD_EX_Map2HVDESBufStatus(MS_U32 u32ESBufStatus)
{

    MS_U32 u32Ret = E_HVD_EX_ES_BUF_STATUS_UNKNOWN;


    switch (u32ESBufStatus)
    {
        case E_HVD_ES_BUF_STATUS_UNDERFLOW:
            u32Ret = E_HVD_EX_ES_BUF_STATUS_UNDERFLOW;
            break;
        case E_HVD_ES_BUF_STATUS_OVERFLOW:
            u32Ret = E_HVD_EX_ES_BUF_STATUS_OVERFLOW;
            break;
        case E_HVD_ES_BUF_STATUS_NORMAL:
            u32Ret = E_HVD_EX_ES_BUF_STATUS_NORMAL;
            break;
        default:
            break;
    }
    return u32Ret;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_SetOSRegBase()
/// @brief \b Function \b Description:  Set system register base
/// @param -u32RegBaseAddr \b IN :  system register base
//-----------------------------------------------------------------------------
void MDrv_HVD_EX_SetOSRegBase(MS_U32 u32RegBaseAddr)
{
    HVD_EX_MSG_DBG("u32RiuBaseAddr=%lx\n", u32RegBaseAddr);
    HAL_HVD_EX_InitRegBase(u32RegBaseAddr);
}

void MDrv_HVD_EX_SetCtrlsBase(MS_U32 u32Id)
{
    // for MJPEG, need to be modified later
    MS_U8 u8Offset = HAL_VPU_EX_GetTaskId(u32Id);
    UNUSED(u8Offset);
    // The Driver Ctrl base should be set with parameter [0]
    HAL_HVD_EX_SetDrvCtrlsBase(&(pHVDDrvContext->gHVDCtrl_EX[0]));

}

void MDrv_HVD_EX_MJPEG_InitSharemem(MS_U32 u32Id, MS_U32 u32CodeBufVAddr)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl1 = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    memset((void *) pCtrl1, 0, sizeof(HVD_EX_Drv_Ctrl));
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufVAddr = u32CodeBufVAddr;
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr = MS_VA2PA(u32CodeBufVAddr);
    pHVDDrvContext->bHVDIsInited[u8DrvId] = TRUE;
    HVD_Pre_Ctrl *pCtrl = &(pHVDDrvContext->gHVDPreCtrl[u8DrvId]);

    memset((void *) pCtrl, 0, sizeof(HVD_Pre_Ctrl));

    // PreSetControl
    HAL_HVD_EX_SetPreCtrlVariables(u32Id,(MS_U32)(&pHVDDrvContext->gHVDPreCtrl[u8DrvId]));
    HVD_EX_MSG_INF("u8DrvId=%d, PA:[0x%lx, 0x%lx], VA:[0x%lx, 0x%lx], [0x%lx]\n", u8DrvId,
        pHVDDrvContext->gHVDCtrl_EX[0].MemMap.u32CodeBufAddr,
        pHVDDrvContext->gHVDCtrl_EX[1].MemMap.u32CodeBufAddr,
        pHVDDrvContext->gHVDCtrl_EX[0].MemMap.u32CodeBufVAddr,
        pHVDDrvContext->gHVDCtrl_EX[1].MemMap.u32CodeBufVAddr,
        HAL_HVD_EX_GetShmAddr(u32Id));

    HAL_HVD_EX_InitShareMem(u32Id);

}

void MDrv_HVD_EX_MJPEG_Exit(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    pHVDDrvContext->bHVDIsInited[u8DrvId] = FALSE;
}

HVD_EX_Result MDrv_HVD_Init_Share_Mem(void)
{
#if ((defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS)) && (!defined(SUPPORT_X_MODEL_FEATURE)))
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;


    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux HVD driver",
                                  sizeof(HVD_Drv_CTX),
                                  &u32ShmId,
                                  &u32Addr,
                                  &u32BufSize,
                                  MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux HVD driver",
                                     sizeof(HVD_Drv_CTX),
                                     &u32ShmId,
                                     &u32Addr,
                                     &u32BufSize,
                                     MSOS_SHM_CREATE))
        {
            HVD_EX_MSG_ERR("[%s]SHM allocation failed!!!use global structure instead!!!\n",__FUNCTION__);
            if(pHVDDrvContext == NULL)
            {
                pHVDDrvContext = &gHVDDrvContext;
                memset(pHVDDrvContext,0,sizeof(HVD_Drv_CTX));
                _HVD_EX_Context_Init();
                HVD_PRINT("[%s]Global structure init Success!!!\n",__FUNCTION__);
            }
            else
            {
                HVD_PRINT("[%s]Global structure exists!!!\n",__FUNCTION__);
            }
            //return E_HVD_EX_FAIL;
        }
        else
        {
            memset((MS_U8*)u32Addr,0,sizeof(HVD_Drv_CTX));
            pHVDDrvContext = (HVD_Drv_CTX*)u32Addr; // for one process
            _HVD_EX_Context_Init();
        }
    }
    else
    {
        pHVDDrvContext = (HVD_Drv_CTX*)u32Addr; // for another process
    }
#else
    if(pHVDDrvContext == NULL)
    {
        pHVDDrvContext = &gHVDDrvContext;
        memset(pHVDDrvContext,0,sizeof(HVD_Drv_CTX));
        _HVD_EX_Context_Init();
    }
#endif

    if(HAL_HVD_EX_Init_Share_Mem() != TRUE)
    {
        return E_HVD_EX_FAIL;
    }

    HAL_HVD_EX_SetDrvCtrlsBase(&(pHVDDrvContext->gHVDCtrl_EX[0]));

    if(HAL_VPU_EX_Init_Share_Mem() != TRUE)
    {
        return E_HVD_EX_FAIL;
    }

    return E_HVD_EX_OK;
}

// Action
HVD_EX_Result MDrv_HVD_EX_GetFreeStream(MS_U32 *pu32Id, HVD_EX_DRV_StreamType eStreamType)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U32 i = 0;
    MS_U32 u32VPUStreamId = 0;
    MS_U32 u32HVDStreamId = 0;

    HVD_EX_MSG_TRACE();

    //Get drvIdx
    switch (eStreamType)
    {
        case E_HVD_EX_DRV_MAIN_STREAM:
            i = 0;
            break;
        case E_HVD_EX_DRV_SUB_STREAM:
            i = 1;
            break;
        case E_HVD_EX_DRV_MVC_STREAM:
            i = 0;
            break;
        case E_HVD_EX_DRV_STREAM_NONE:
        default:
            i = HVD_MAX_STREAMS;
            break;
    }

    if (i == HVD_MAX_STREAMS)
    {
        return E_HVD_EX_FAIL;
    }

    if (E_HVD_EX_DRV_MAIN_STREAM == eStreamType)
    {
        u32VPUStreamId = (MS_U32) HAL_VPU_EX_GetFreeStream(E_HAL_VPU_MAIN_STREAM);
        u32HVDStreamId = (MS_U32) HAL_HVD_EX_GetFreeStream(E_HAL_HVD_MAIN_STREAM);

        if (u32VPUStreamId && u32HVDStreamId)
        {
            *pu32Id = (i << 16 | u32HVDStreamId << 8 | u32VPUStreamId);
            eRet = E_HVD_EX_OK;
        }
    }
    else if (E_HVD_EX_DRV_SUB_STREAM == eStreamType)
    {
        u32VPUStreamId = (MS_U32) HAL_VPU_EX_GetFreeStream(E_HAL_VPU_SUB_STREAM);
        if(u32VPUStreamId == E_HAL_VPU_STREAM_NONE)
        {
            return E_HVD_EX_FAIL;
        }
        u32HVDStreamId = (MS_U32) HAL_HVD_EX_GetFreeStream(E_HAL_HVD_SUB_STREAM);

        if (u32VPUStreamId && u32HVDStreamId)
        {
            *pu32Id = (i << 16 | u32HVDStreamId << 8 | u32VPUStreamId);
            eRet = E_HVD_EX_OK;
        }
    }
#if HVD_ENABLE_MVC
    else if (E_HVD_EX_DRV_MVC_STREAM == eStreamType)
    {
        u32VPUStreamId = (MS_U32) HAL_VPU_EX_GetFreeStream(E_HAL_VPU_MVC_STREAM);
        u32HVDStreamId = (MS_U32) HAL_HVD_EX_GetFreeStream(E_HAL_HVD_MVC_STREAM);

        if (u32VPUStreamId && u32HVDStreamId)
        {
            *pu32Id = (i << 16 | u32HVDStreamId << 8 | u32VPUStreamId);
            eRet = E_HVD_EX_OK;
        }
    }
#endif
    else
    {
        HVD_PRINT("%s:%s:Stream type=%d is unexpected\n", __FILE__, __FUNCTION__, eStreamType);
    }

    return eRet;
}





extern MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType);
extern HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id);

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Init()
/// @brief \b Function \b Description:  HVD driver initialization
/// @param -pStMemCfg \b IN :  pointer to the memory config of HVD driver
/// @param -pStInitSettings \b IN :  Initialization of HVD driver
/// @return -The result of initialization process
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Init(MS_U32 u32Id, HVD_EX_MemCfg *pStMemCfg, HVD_EX_InitSettings *pStInitSettings)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    HVD_EX_MSG_TRACE();

#if HVD_ENABLE_TIME_MEASURE
    pHVDDrvContext->u32InitSysTimeBase[u8DrvId] = HVD_GetSysTime_ms();
#endif

    if ((pStMemCfg == NULL) || (pStInitSettings == NULL))
    {
        HVD_EX_MSG_ERR("Init params are invalid\n");
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    if(_MDrv_HVD_AUTH_IPCheck(pStInitSettings->u32ModeFlag) == FALSE)
    {
        HVD_EX_MSG_ERR("[%s]User using invaild key !!!\n",__FUNCTION__);
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    HAL_HVD_EX_SetDrvCtrlsBase(&(pHVDDrvContext->gHVDCtrl_EX[0]));

#if HVD_ENABLE_AUTO_SET_REG_BASE
    {
        MS_U32 u32NonPMBankSize = 0, u32RiuBaseAdd = 0;

        if (!MDrv_MMIO_GetBASE(&u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
        {
            HVD_EX_MSG_ERR("MMIO_GetBASE failure\n");
            return eRet;
        }
        else
        {
            HVD_EX_MSG_DBG("u32RiuBaseAdd=0x%lx\n", u32RiuBaseAdd);
            HAL_HVD_EX_InitRegBase(u32RiuBaseAdd);
        }

    #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
        HAL_HVD_EX_SetHwRegBase(u32Id, pStInitSettings->u32ModeFlag); //HVD or EVD
    #endif
    }
#endif

    HVD_EX_MSG_DBG("system call type: Mutex:%d clock:%d delay:%d asm_sync:%d\n",
                 HVD_ENABLE_MUTEX_PROTECT, HVD_SYSTEM_CLOCK_TYPE, HVD_SYSTEM_DELAY_MS_TYPE, HVD_MEMORY_BARRIER_TYPE);

    if (u32UartCtrl & E_HVD_UART_CTRL_INFO)
    {
        MS_U32 u32delaytime = 5;
        MS_U32 u32FirstTime = HVD_GetSysTime_ms();
        MS_U32 u32SecondTime = 0;

        HVD_Delay_ms(u32delaytime);
        u32SecondTime = HVD_GetSysTime_ms();

        HVD_EX_MSG_DBG("MSOS API check: 1st:%lu 2nd:%lu delay:%lu dif:%lu\n",
                     u32FirstTime, u32SecondTime, u32delaytime, u32SecondTime - u32FirstTime);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bUsed)
    {
        HVD_EX_MSG_ERR("re-init HVD Driver\n");

#if HVD_ENABLE_REINIT_FAILED
        eRet = E_HVD_EX_RET_RE_INIT;
        return eRet;
#endif
    }

    // disable ISR first
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    HAL_HVD_EX_EnableISR(u32Id, FALSE);
#else
    HAL_HVD_EX_EnableISR(FALSE);
#endif
    // For TEE
    _HVD_EX_SetShareInfoAddr(u32Id);

    eRet = _HVD_EX_InitVariables(u32Id, pStMemCfg, (HVD_Init_Params *) pStInitSettings);

    if (eRet != E_HVD_EX_OK)
    {
        return eRet;
    }

#if SUPPORT_EVD
    if ((pStInitSettings->u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC ||
        (pStInitSettings->u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
        pCtrl->HVDISRCtrl.bIsHvdIsr = FALSE;
    else
#endif
        pCtrl->HVDISRCtrl.bIsHvdIsr = TRUE;
#if SUPPORT_G2VP9
    if ((pStInitSettings->u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
        pCtrl->HVDISRCtrl.bIsG2Vp9Isr = TRUE;
    else
#endif
        pCtrl->HVDISRCtrl.bIsG2Vp9Isr = FALSE;

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = _HVD_EX_Check_Cmd(u32Id, E_HVD_CHECK_CMD_INIT);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif


    eRet = _HVD_EX_Init_(u32Id);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase[u8DrvId], __FUNCTION__, __LINE__);
#endif

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32DummyWriteBuf = MsOS_PA2KSEG1(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr + HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DUMMY_WRITE_ADDR));
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode |= HVD_CTRL_INIT_FINISHED;
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bUsed = TRUE;
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32Sid = u32Id;
#if HVD_ENABLE_MVC
    if((pStInitSettings->u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1].u32CtrlMode |= HVD_CTRL_INIT_FINISHED;
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1].bUsed = TRUE;
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1].u32Sid = u32Id+0x00011000;
        pHVDDrvContext->bHVDIsInited[u8DrvId+1] = TRUE;
    }
#endif /// HVD_ENABLE_MVC
    pHVDDrvContext->bHVDIsInited[u8DrvId] = TRUE;
    eRet = E_HVD_EX_OK;

    HVD_EX_MSG_INF("driver Init successfully.\n");

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Rst()
/// @brief \b Function \b Description:  Reset HVD driver
/// @param -bErrHandle \b IN :  reset option HVD driver
///                 -FALSE(0): Reset HVD to clear mode.( do not recovery SPS)
///                 -TRUE(1): Reset HVD to clear mode, and recovery SPS.
/// @return -The result of reset process
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);

    if (bErrHandle == TRUE)
    {
        HVD_EX_MSG_ERR("HVD Not supported reset(TRUE) yet\n");
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) != HVD_INIT_MAIN_LIVE_STREAM)
    {
        HVD_EX_MSG_ERR("HVD rst() only support live stream mode\n");
        return E_HVD_EX_RET_INVALID_PARAMETER;
    }

    //In this function we should delete task and then create task.
    HVD_EX_SetRstFlag(TRUE);

    eRet = _HVD_EX_Rst(u32Id, bErrHandle);

    if (eRet != E_HVD_EX_OK)
    {
        return (eRet);
    }

    eRet = _HVD_EX_RecoverySettings(u32Id);

    if (eRet != E_HVD_EX_OK)
    {
        return (eRet);
    }

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode |= HVD_CTRL_INIT_FINISHED;
    pHVDDrvContext->bHVDIsInited[u8DrvId] = TRUE;

    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Play()
/// @brief \b Function \b Description:  Play HVD
/// @return -The result of command play
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Play(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if (MDrv_HVD_EX_GetPlayState(u32Id) != E_HVD_EX_GSTATE_PLAY || pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bStepDecoding)
#endif
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAY, 0);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
    }

    // step display off
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_TRIGGER_DISP, 0);

    if (eRet == E_HVD_EX_OK)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode &= ~HVD_CTRL_DISPLAY_CTRL;
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Exit()
/// @brief \b Function \b Description:  Stop HVD and release resource.
/// @return -The result of command stop
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Exit(MS_U32 u32Id)
{
    MS_U32 u32Timer = 0;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_Rsting(u8DrvId,eRet);

#if HVD_ENABLE_MVC
	if((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC)
	{
		pHVDDrvContext->bHVDIsInited[u8DrvId+1] = FALSE;
	}
#endif /// HVD_ENABLE_MVC

    do
    {
        u32Timer++;

        if (u32Timer >= HVD_FW_EXIT_ACTION_TIMEOUT)
        {
            eRet = E_HVD_EX_RET_TIMEOUT;
            HVD_EX_MSG_ERR("exit time out~~~`\n");
            //return eRet;
            break;
        }
        else
        {
            HVD_Delay_ms(1);
        }
    } while (_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_PROCESSING));

    // release ISR callback
    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].HVDISRCtrl.bRegISR)
    {
        if (E_HVD_EX_OK != MDrv_HVD_EX_SetISREvent(u32Id, E_HVD_EX_ISR_NONE, NULL))
        {
            HVD_EX_MSG_ERR("MDrv_HVD_EX_Exit: dettach isr fail\n");
        }
    }
    pHVDDrvContext->bHVDIsInited[u8DrvId] = FALSE;
    // close HVD FW
    eRet = (HVD_EX_Result) HAL_HVD_EX_DeInit(u32Id);

    if(eRet == E_HVD_EX_RET_TIMEOUT)
    {
        HVD_EX_MSG_ERR("force delete task  ,cause cmd timeout \n");
        eRet = E_HVD_EX_OK;
    }

#if HVD_ENABLE_MVC
    if((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_MVC)
    {
        memset((void *) &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId+1]), 0, sizeof(HVD_EX_Drv_Ctrl));
    }
#endif /// HVD_ENABLE_MVC

    // reset internal control variables
    memset((void *) &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]), 0, sizeof(HVD_EX_Drv_Ctrl));
    memset((void *) &(pHVDDrvContext->gHVDPreCtrl[u8DrvId]), 0, sizeof(HVD_Pre_Ctrl));

    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Pause()
/// @brief \b Function \b Description:  Pause HVD
/// @return -The result of command pause
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Pause(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PAUSE, 0);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    // step display off
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_TRIGGER_DISP, 0);

    if (eRet == E_HVD_EX_OK)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode &= ~HVD_CTRL_DISPLAY_CTRL;
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Flush()
/// @brief \b Function \b Description:  Flush queue and buffer
/// @param -bShowLast \b IN :  TRUE / FALSE
///                 -FALSE(0): show current displayed frame
///                 -TRUE(1): show last decoded frame
/// @return -The result of command flush
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Flush(MS_U32 u32Id, MS_BOOL bShowLast)
{
    MS_U32 u32Times = 0;
	MS_U32 u32FlushTime = 0;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    // pause first
    if (MDrv_HVD_EX_GetPlayState(u32Id) != E_HVD_EX_GSTATE_PAUSE)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PAUSE, 0);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        // check flush done
        while (1)
        {
            if (MDrv_HVD_EX_GetPlayState(u32Id) == E_HVD_EX_GSTATE_PAUSE)
            {
                break;
            }

            u32Times++;
            HVD_Delay_ms(1);

            if (u32Times > 100)
            {
                HVD_EX_MSG_INF("pause in flush() not finished.\n");
                break;
            }
        }
    }

    // step display off
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_TRIGGER_DISP, 0);

    if (eRet == E_HVD_EX_OK)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode &= ~HVD_CTRL_DISPLAY_CTRL;
    }

#if HVD_ENABLE_RV_FEATURE
    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_RM)
    {
        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].LastNal.bRVBrokenPacket)
        {
            HVD_BBU_Info bbuInfo = {0,0,0,0,0,0,0,0,0,0} ;

            HVD_EX_MSG_INF("push dummy packet for broken by us packet\n");

            bbuInfo.u32Length       = bbuInfo.u32AllocLength = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32RV_FlushPacketSize;
            bbuInfo.u32Staddr       = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32RV_FlushPacketAddr - pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr;
            bbuInfo.u32OriPktAddr   = bbuInfo.u32Staddr;
            bbuInfo.bRVBrokenPacket = FALSE;

            eRet = (HVD_EX_Result) HAL_HVD_EX_PushPacket(u32Id, (HVD_BBU_Info *) &bbuInfo);
        }
    }
#endif

    // fire all packet to FW
    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR_FIRED) != HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR))
    {
        HAL_HVD_EX_UpdateESWptr_Fire(u32Id);
    }

    // send command
    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_FLUSH_STATUS,  (MS_U32)E_HVD_EX_FLUSH_RUNNING);
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FLUSH, (MS_U32) bShowLast);

	u32FlushTime = 0;

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    // check flush done only for raw file mode
    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
    {
        while (1)
        {
            //_DRV_HVD_PushDummy();
            if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_VP8)
            {
                if (_HVD_EX_IsAllBufferEmpty_VP8(u32Id))
                {
                    eRet = E_HVD_EX_OK;
                    HVD_EX_MSG_INF("[Check Flush Done] flush queue empty\n");
                    break;
                }
            }
            else
            {
            if (_HVD_EX_IsAllBufferEmpty(u32Id) && (u32FlushTime >= 50))
            {
                eRet = E_HVD_EX_OK;
                break;
                }
            }
            u32Times++;
			u32FlushTime++;
            HVD_Delay_ms(1);
            if (u32Times > 1000)
            {
                HVD_EX_MSG_ERR("Flush() timeout failed: BBU:%lu Dec:%lu Disp:%lu Idle:%lu ESr:%lx ESw:%lx\n",
                            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB),
                            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_Q_NUMB),
                            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_NUMB),
                            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT),
                            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR), HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR));
                eRet = E_HVD_EX_FAIL;
                break;
            }
        }
        // reset byte_cnt
        HAL_HVD_EX_RstPTSCtrlVariable(u32Id);
    }
    switch ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
    {
        case HVD_INIT_HW_VP8:
            //HVD_PRINT("[DRV1] BBU WRp = %x, RDp = %x\n",HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR),HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR));
            //HAL_HVD_EX_SetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR, HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR));
            //HVD_PRINT("[DRV2] BBU WRp = %x, RDp = %x\n",HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR),HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR));
            HAL_HVD_EX_UpdateESWptr_Fire(u32Id);
            break;
        default:
            break;
    }

    HAL_HVD_EX_FlushRstShareMem(u32Id);
    pCtrl->u32FlushRstPtr = 1;

    // MediaCodec need this to let scan mode to be normal.
    if (0)                      //bPlayback)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAY, 0);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_StepDisp()
/// @brief \b Function \b Description:  Trigger HVD to show one frame
/// @return -The result of command trigger display
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_StepDisp(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    // check command
    eRet = _HVD_EX_Check_Cmd(u8DrvId, E_HVD_CHECK_CMD_TRIGGER_DISP);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    // step display on
    if (_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISPLAY_CTRL))
    {
        if (MDrv_HVD_EX_IsFrameShowed(u32Id))
        {
            eRet = (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_TRIGGER_DISP, 1);
        }
        else
        {
            HVD_EX_MSG_INF("Previous Step Display command is not finished\n");
            eRet = E_HVD_EX_OK;
        }
    }
    else
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_TRIGGER_DISP, 1);

        if (eRet == E_HVD_EX_OK)
        {
            _DRV_HVD_SetCtrl(u8DrvId, HVD_CTRL_DISPLAY_CTRL);
        }
    }
    // enter play mode
    if (1)                      //HAL_HVD_EX_GetData(E_HVD_GDATA_FW_STATE) == E_HVD_FW_PAUSE )
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAY, 0);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
    }
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_StepDecode()
/// @brief \b Function \b Description:  Step decode one frame
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_StepDecode(MS_U32 u32Id)
{
    MS_U32 u32Times = 0;
    MS_U32 FWState = 0;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    // check if step decoding
    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bStepDecoding)
    {
        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32StepDecodeCnt == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT))
        {
            eRet = E_HVD_EX_RET_NOTREADY;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
    }

    // Pause first
    FWState = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATE);

    if (FWState != (MS_U32) E_HVD_FW_PAUSE)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PAUSE, 0);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
        while (FWState != ((MS_U32) E_HVD_FW_PAUSE) && u32Times <= 10000)
        {
            FWState = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATE);
            u32Times++;
            HVD_Delay_ms(1);
        }
        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATE) != E_HVD_FW_PAUSE)
        {
            eRet = E_HVD_EX_FAIL;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
    }

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bStepDecoding = TRUE;
    // get decode cnt
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32StepDecodeCnt = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT);
    // step decode
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_STEP_DECODE, 0);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

// set command, action, status, input
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_PushQueue()
/// @brief \b Function \b Description:  push one entry into the decoding table(BBU table).
/// @param -pInfo \b IN :  Pointer to the information of input packet.
/// @return -The result of command push queue
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_PushQueue(MS_U32 u32Id, HVD_EX_PacketInfo *pInfo)
{
    HVD_EX_Result eRet      = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_BOOL bNULLPacket     = FALSE;
    MS_U8 u8DrvId           = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl  = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_BBU_Info bbuInfo;

    //******** for 4kx2k stream ********//
    MS_U8 u8Part = 0;
    MS_BOOL bPartToBeSend[QUANTITY_AFTER_BROKEN_BY_US]={0};
    MS_U32  u32PartLen[QUANTITY_AFTER_BROKEN_BY_US]={0};
    MS_U32  u32PartAllocLen[QUANTITY_AFTER_BROKEN_BY_US]={0};

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (pInfo == NULL)
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    else if ((pInfo->u32Length >= HVD_BBU_TAG_LIMITATION) && (pInfo->u32Length < MAX_QUANTITY))
    {
        HVD_EX_MSG_DBG("input packet size(0x%lx) larger than HW_bbu_tag_limit max packet size(0x%x), AllocLength(0x%lx); to play 4kx2k stream, break it by us \n",
                    pInfo->u32Length, HVD_BBU_TAG_LIMITATION, pInfo->u32AllocLength);

        bPartToBeSend[1] = TRUE;
        u32PartLen[1] = pInfo->u32Length - QUANTITY_LENGTH;
        u32PartAllocLen[1] = pInfo->u32AllocLength - QUANTITY_LENGTH;

        pInfo->u32Length = QUANTITY_LENGTH;
        pInfo->u32AllocLength = QUANTITY_LENGTH;
        pInfo->u32Staddr |= HVD_RV_BROKEN_BY_US_MASK;

        MS_U32 i;
        for(i=2; i< QUANTITY_AFTER_BROKEN_BY_US; i++)
        {
            if (u32PartLen[i-1] >= QUANTITY_LENGTH)
            {
                bPartToBeSend[i] = TRUE;
                u32PartLen[i] = u32PartLen[i-1] - QUANTITY_LENGTH;
                u32PartAllocLen[i] = u32PartAllocLen[i-1] - QUANTITY_LENGTH;
                u32PartLen[i-1] = QUANTITY_LENGTH;
                u32PartAllocLen[i-1] = QUANTITY_LENGTH;
            }
            else
            {
                break;
            }
        }

    }
    else if (pInfo->u32Length >= MAX_QUANTITY)
    {
        HVD_EX_MSG_ERR("input packet size(0x%lx) larger than SW_defined_4kx2k max packet size(0x%x), AllocLength(0x%lx) \n",
                    pInfo->u32Length, MAX_QUANTITY, pInfo->u32AllocLength);
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    else if ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_INPUT_MASK) == HVD_INIT_INPUT_TSP)
    {
        HVD_EX_MSG_ERR("Init mode is TSP input and PushQueue() is not supported in TSP input\n");
        eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    /*
    else if (((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_RM) &&
             (pInfo->u32Staddr >= pCtrl->MemMap.u32BitstreamBufSize))
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }*/

    bbuInfo.u32ID_H         = pInfo->u32ID_H;
    bbuInfo.u32ID_L         = pInfo->u32ID_L;
    bbuInfo.u32Length       = pInfo->u32Length;
    bbuInfo.u32TimeStamp    = pInfo->u32TimeStamp;
    bbuInfo.u32AllocLength  = pInfo->u32AllocLength;

    if (pInfo->u32Staddr & HVD_RV_BROKEN_BY_US_MASK)
    {
        bbuInfo.bRVBrokenPacket = TRUE;
        bbuInfo.u32OriPktAddr = pInfo->u32Staddr & (~HVD_RV_BROKEN_BY_US_MASK);
        bbuInfo.u32Staddr = pInfo->u32Staddr & (~HVD_RV_BROKEN_BY_US_MASK);
    }
    else
    {
        bbuInfo.bRVBrokenPacket = FALSE;
        bbuInfo.u32OriPktAddr = pInfo->u32Staddr;
        bbuInfo.u32Staddr = pInfo->u32Staddr;
    }


    if ( !bbuInfo.bRVBrokenPacket &&
             (pInfo->u32Staddr >= pCtrl->MemMap.u32BitstreamBufSize))
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }


    bbuInfo.u32Staddr2 = pInfo->u32Staddr2;
    bbuInfo.u32Length2 = pInfo->u32Length2;

    // invalid packet
    if ((bbuInfo.u32TimeStamp != HVD_U32_MAX) && (bbuInfo.u32Length == 0))
    {
        HVD_EX_MSG_INF("Invalid Packet(size:0x%lx PTS:0x%lx)\n ", bbuInfo.u32Length, bbuInfo.u32TimeStamp);
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    // AVI NULL packet
#if HVD_ENABLE_AUTO_AVI_NULL_PACKET
    else if ((bbuInfo.u32TimeStamp == HVD_U32_MAX) && (bbuInfo.u32Length == 0))
    {
        if (pCtrl->bNoDrvProccBuf)
        {
            HVD_EX_MSG_INF
                ("AVI Null Packet(size:0x%lx PTS:0x%lx), but do not have enough driver process buffer(0x%lx)\n ",
                 pInfo->u32Length, bbuInfo.u32TimeStamp, pCtrl->MemMap.u32DrvProcessBufSize);
            eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
        else
        {
            bNULLPacket = TRUE;
            bbuInfo.u32Length = pCtrl->u32NULLPacketSize;
            bbuInfo.u32Staddr = pCtrl->u32NULLPacketAddr - pCtrl->MemMap.u32BitstreamBufAddr;
        }
    }
#endif

    // the else are all normal cases.
    if (MDrv_HVD_EX_GetBBUVacancy(u32Id) != 0)
    {
        MS_U32 u32ESRptr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR);
        MS_U32 u32ESWptr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR);

        if ((u32ESRptr <= u32ESWptr) &&
            (pCtrl->MemMap.u32BitstreamBufSize < bbuInfo.u32Staddr + bbuInfo.u32Length))
        {
            HVD_EX_MSG_ERR("input packet (0x%lx 0x%lx 0x%lx) may cause bitstream buffer overflow (0x%lx 0x%lx) 0x%lx\n ",
                        bbuInfo.u32Staddr, bbuInfo.u32Length, bbuInfo.u32Staddr + bbuInfo.u32Length,
                        u32ESRptr, u32ESWptr, pCtrl->MemMap.u32BitstreamBufSize);
        }

        if ((u32ESWptr < u32ESRptr) &&
            (u32ESWptr < bbuInfo.u32Staddr) &&
            (bbuInfo.u32Staddr < u32ESRptr) &&
            (u32ESRptr <= bbuInfo.u32Staddr + bbuInfo.u32Length))
        {
            HVD_EX_MSG_ERR("input packet (staddr=0x%lx, len=0x%lx) may overwrite undecoded data (rptr=0x%lx wptr=0x%lx)\n ",
                        bbuInfo.u32Staddr, bbuInfo.u32Length, u32ESRptr, u32ESWptr);
        }

        if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB) &&
            (!bNULLPacket) &&
            (pCtrl->MemMap.u32DrvProcessBufSize != 0) &&
            (pCtrl->MemMap.u32BitstreamBufAddr <= pCtrl->MemMap.u32DrvProcessBufAddr) &&
            (pCtrl->MemMap.u32DrvProcessBufAddr < (pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->MemMap.u32BitstreamBufSize)))
        {
            MS_U32 u32Lower = pCtrl->MemMap.u32DrvProcessBufAddr - pCtrl->MemMap.u32BitstreamBufAddr;
            MS_U32 u32Upper = u32Lower + pCtrl->MemMap.u32DrvProcessBufSize;

            if (((u32Lower <= bbuInfo.u32Staddr) && (bbuInfo.u32Staddr < u32Upper)) ||
                ((u32Lower <= (bbuInfo.u32Staddr + bbuInfo.u32Length)) && ((bbuInfo.u32Staddr + bbuInfo.u32Length) < u32Upper)) ||
                ((bbuInfo.u32Staddr < u32Lower) && (u32Upper <= (bbuInfo.u32Staddr + bbuInfo.u32Length))))
            {
                HVD_EX_MSG_ERR("input packet (0x%lx 0x%lx 0x%lx) is located in HVD driver process buffer(0x%lx 0x%lx)\n ",
                     bbuInfo.u32Staddr, bbuInfo.u32Length, bbuInfo.u32Staddr + bbuInfo.u32Length,
                     u32Lower, u32Upper);
            }
        }

        //for debug
        #if 0
        {
            HVD_EX_MSG_INF("HVD : %lu (%lu %lu) ID:%lx input packet (%lx %lx %lx) (%lx %lx %lx) (%lu %lu %lu)\n ",
                         pCtrl->u32BBUPacketCnt, MDrv_HVD_EX_GetDataErrCnt(u32Id), MDrv_HVD_EX_GetDecErrCnt(u32Id),
                         bbuInfo.u32ID_L, bbuInfo.u32Staddr + bbuInfo.u32Length, bbuInfo.u32Length,
                         bbuInfo.u32Staddr, u32ESRptr, pCtrl->MemMap.u32BitstreamBufSize, u32ESWptr,
                         MDrv_HVD_EX_GetBBUVacancy(u32Id), HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR),
                         HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR));
        }
        #endif

        if ( (!bbuInfo.bRVBrokenPacket)&&
            ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_RM) &&
            ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_VP8) &&
            (pCtrl->bAutoRmLastZeroByte) &&
            ((!pCtrl->bCannotAccessMIU256) || ((pCtrl->bCannotAccessMIU256) && (pCtrl->MemMap.u32BitstreamBufAddr < pCtrl->MemMap.u32MIU1BaseAddr))))
        {
            if ((pCtrl->u8SecureMode == E_HVD_SECURE_MODE_NONE) && (!(pHVDDrvContext->bVPUIsSecureMode)))
            {
                    MS_U32 ModifyCnt = 0;
                    MS_U32 u32tmp = 0;
                    MS_U8 *pByte = NULL;
                    u32tmp = (bbuInfo.u32Staddr + bbuInfo.u32Length - 1);
                    if (u32tmp >= pCtrl->MemMap.u32BitstreamBufSize)
                    {
                        u32tmp -= pCtrl->MemMap.u32BitstreamBufSize;
                    }
                    u32tmp += pCtrl->MemMap.u32BitstreamBufVAddr;
                    pByte = (MS_U8 *) u32tmp;

                if((bbuInfo.u32Length > 2)
                && ((*pByte == 0) && (*(pByte-1) == 0) && (*(pByte-2) == 0)))
                {
                    //301385:
                    //more then 2 zero bytes after valid bytes.
                    //no need to remove pending zero bytes to prevent system busy
                    //printf("no need to remove pending zero~~\n");
                }
                else
                {
#if 1 //use if-condition instead of while-loop
                    if(*pByte == 0)
                    {
                        ModifyCnt++;
                        bbuInfo.u32Length--;
                        pByte--;

                        if(bbuInfo.u32Length && (*pByte == 0))
                        {
                            ModifyCnt++;
                            bbuInfo.u32Length--;
                        }
                    }
#else
                    while (1)           //for( ModifyCnt=0 ; ModifyCnt<3;ModifyCnt++ )
                    {
                        if (bbuInfo.u32Length)
                        {
                            if (*pByte == 0)
                            {
                                /*
                                   if( ModifyCnt == 2 )
                                   {
                                   gHVDPacket.u32Length+=ModifyCnt;
                                   }
                                   else
                                 */
                                {
                                    ModifyCnt++;
                                    bbuInfo.u32Length--;
                                    pByte--;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
#endif
                }

                if (ModifyCnt != 0)
                {
                    //HVD_MSG_INFO("HVD remove last zero byte:%lu\n" , ModifyCnt);
                }

                if (bbuInfo.u32Length == 0)
                {
                    HVD_EX_MSG_ERR("Packet with all zero bytes(staddr:0x%lx remove zero bytes:%lu)\n ",
                                bbuInfo.u32Staddr, ModifyCnt);
                    eRet = E_HVD_EX_OK;
                    _DRV_HVD_EX_RET(u8DrvId, eRet);
                }
            }
        }
        /*
           {
           MS_U8 *pByte=NULL;
           pByte = (MS_U8 *)((gHVDCtrl.MemMap.u32BitstreamBufVAddr) + gHVDPacket.u32Staddr );
           HVD_MSG_INFO("HVD remove last zero byte:%02x%02x%02x%02x\n" ,
           *pByte  , *(pByte+1) , *(pByte+2) , *(pByte+3) );
           }
         */
        eRet = (HVD_EX_Result) HAL_HVD_EX_PushPacket(u32Id, (HVD_BBU_Info *) &bbuInfo);
        pCtrl->u32FlushRstPtr = 0;
    }
    else
    {
        HVD_EX_MSG_DBG("Push queue full\n");
        eRet = E_HVD_EX_RET_QUEUE_FULL;
    }

    for ( u8Part = 1; u8Part < QUANTITY_AFTER_BROKEN_BY_US; u8Part++)
    {
        if (bPartToBeSend[u8Part])
        {
            pInfo->u32Staddr &= (~HVD_RV_BROKEN_BY_US_MASK);
            pInfo->u32Staddr += QUANTITY_LENGTH;

            bbuInfo.u32ID_H         = pInfo->u32ID_H;
            bbuInfo.u32ID_L         = pInfo->u32ID_L;
            bbuInfo.u32Length       = u32PartLen[u8Part];
            bbuInfo.u32TimeStamp    = pInfo->u32TimeStamp;
            bbuInfo.u32AllocLength  = u32PartAllocLen[u8Part];

            if (u8Part != (QUANTITY_AFTER_BROKEN_BY_US - 1))
            {
                if (bPartToBeSend[u8Part+1])
                {
                    bbuInfo.bRVBrokenPacket = TRUE;
                }
                else
                {
                    bbuInfo.bRVBrokenPacket = FALSE;
                }
            }
            else
            {
                bbuInfo.bRVBrokenPacket = FALSE;
            }

            bbuInfo.u32OriPktAddr = pInfo->u32Staddr;
            bbuInfo.u32Staddr = pInfo->u32Staddr;

            bbuInfo.u32Staddr2 = pInfo->u32Staddr2;
            bbuInfo.u32Length2 = pInfo->u32Length2;

            //if (MDrv_HVD_EX_GetBBUVacancy(u32Id) != 0)
            {
                MS_U32 u32ESRptr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR);
                MS_U32 u32ESWptr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR);

                if ((u32ESRptr <= u32ESWptr) &&
                    (pCtrl->MemMap.u32BitstreamBufSize < bbuInfo.u32Staddr + bbuInfo.u32Length))
                {
                    HVD_EX_MSG_ERR("input packet (0x%lx 0x%lx 0x%lx) may cause bitstream buffer overflow (0x%lx 0x%lx) 0x%lx\n ",
                                bbuInfo.u32Staddr, bbuInfo.u32Length, bbuInfo.u32Staddr + bbuInfo.u32Length,
                                u32ESRptr, u32ESWptr, pCtrl->MemMap.u32BitstreamBufSize);
                }

                if ((u32ESWptr < u32ESRptr) &&
                    (u32ESWptr < bbuInfo.u32Staddr) &&
                    (bbuInfo.u32Staddr < u32ESRptr) &&
                    (u32ESRptr <= bbuInfo.u32Staddr + bbuInfo.u32Length))
                {
                    HVD_EX_MSG_ERR("input packet (staddr=0x%lx, len=0x%lx) may overwrite undecoded data (rptr=0x%lx wptr=0x%lx)\n ",
                                bbuInfo.u32Staddr, bbuInfo.u32Length, u32ESRptr, u32ESWptr);
                }

                if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB) &&
                    (!bNULLPacket) &&
                    (pCtrl->MemMap.u32DrvProcessBufSize != 0) &&
                    (pCtrl->MemMap.u32BitstreamBufAddr <= pCtrl->MemMap.u32DrvProcessBufAddr) &&
                    (pCtrl->MemMap.u32DrvProcessBufAddr < (pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->MemMap.u32BitstreamBufSize)))
                {
                    MS_U32 u32Lower = pCtrl->MemMap.u32DrvProcessBufAddr - pCtrl->MemMap.u32BitstreamBufAddr;
                    MS_U32 u32Upper = u32Lower + pCtrl->MemMap.u32DrvProcessBufSize;

                    if (((u32Lower <= bbuInfo.u32Staddr) && (bbuInfo.u32Staddr < u32Upper)) ||
                        ((u32Lower <= (bbuInfo.u32Staddr + bbuInfo.u32Length)) && ((bbuInfo.u32Staddr + bbuInfo.u32Length) < u32Upper)) ||
                        ((bbuInfo.u32Staddr < u32Lower) && (u32Upper <= (bbuInfo.u32Staddr + bbuInfo.u32Length))))
                    {
                        HVD_EX_MSG_ERR("input packet (0x%lx 0x%lx 0x%lx) is located in HVD driver process buffer(0x%lx 0x%lx)\n ",
                             bbuInfo.u32Staddr, bbuInfo.u32Length, bbuInfo.u32Staddr + bbuInfo.u32Length,
                             u32Lower, u32Upper);
                    }
                }

                //for debug
        #if 0
                {
                    HVD_EX_MSG_INF("HVD : %lu (%lu %lu) ID:%lx input packet (%lx %lx %lx) (%lx %lx %lx) (%lu %lu %lu)\n ",
                                 pCtrl->u32BBUPacketCnt, MDrv_HVD_EX_GetDataErrCnt(u32Id), MDrv_HVD_EX_GetDecErrCnt(u32Id),
                                 bbuInfo.u32ID_L, bbuInfo.u32Staddr + bbuInfo.u32Length, bbuInfo.u32Length,
                                 bbuInfo.u32Staddr, u32ESRptr, pCtrl->MemMap.u32BitstreamBufSize, u32ESWptr,
                                 MDrv_HVD_EX_GetBBUVacancy(u32Id), HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR),
                                 HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR));
                }
        #endif

                if (((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_RM) &&
                    ((pCtrl->InitParams.u32ModeFlag & HVD_INIT_HW_MASK) != HVD_INIT_HW_VP8) &&
                    (pCtrl->bAutoRmLastZeroByte) &&
                    ((!pCtrl->bCannotAccessMIU256) || ((pCtrl->bCannotAccessMIU256) && (pCtrl->MemMap.u32BitstreamBufAddr < pCtrl->MemMap.u32MIU1BaseAddr))))
                {
                    MS_U32 ModifyCnt = 0;
                    while (1)           //for( ModifyCnt=0 ; ModifyCnt<3;ModifyCnt++ )
                    {
                        if (bbuInfo.u32Length)
                        {
                            MS_U8 *pByte = NULL;
                            MS_U32 u32tmp = 0;
                            u32tmp = (bbuInfo.u32Staddr + bbuInfo.u32Length - 1);
                            if (u32tmp >= pCtrl->MemMap.u32BitstreamBufSize)
                            {
                                u32tmp -= pCtrl->MemMap.u32BitstreamBufSize;
                            }
                            u32tmp += pCtrl->MemMap.u32BitstreamBufVAddr;
                            pByte = (MS_U8 *) u32tmp;
                            if (*pByte == 0)
                            {
                                /*
                                   if( ModifyCnt == 2 )
                                   {
                                   gHVDPacket.u32Length+=ModifyCnt;
                                   }
                                   else
                                 */
                                {
                                    ModifyCnt++;
                                    bbuInfo.u32Length--;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (ModifyCnt != 0)
                    {
                        //HVD_MSG_INFO("HVD remove last zero byte:%lu\n" , ModifyCnt);
                    }
                    if (bbuInfo.u32Length == 0)
                    {
                        HVD_EX_MSG_ERR("Packet with all zero bytes(staddr:0x%lx remove zero bytes:%lu)\n ",
                                    bbuInfo.u32Staddr, ModifyCnt);
                        eRet = E_HVD_EX_OK;
                        _DRV_HVD_EX_RET(u8DrvId, eRet);
                    }
                }
                /*
                   {
                   MS_U8 *pByte=NULL;
                   pByte = (MS_U8 *)((gHVDCtrl.MemMap.u32BitstreamBufVAddr) + gHVDPacket.u32Staddr );
                   HVD_MSG_INFO("HVD remove last zero byte:%02x%02x%02x%02x\n" ,
                   *pByte  , *(pByte+1) , *(pByte+2) , *(pByte+3) );
                   }
                 */
                eRet = (HVD_EX_Result) HAL_HVD_EX_PushPacket(u32Id, (HVD_BBU_Info *) &bbuInfo);
            }
        }
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_PushQueue_Fire()
/// @brief \b Function \b Description:  fire all waiting entry into the decoding table(BBU table).
/// @return -The result of command push queue fire
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_PushQueue_Fire(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    HAL_HVD_EX_UpdateESWptr_Fire(u32Id);

    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DecodeIFrame()
/// @brief \b Function \b Description:  Decode I frame only under driver input path.
/// @param -u32SrcSt \b IN :  The physical address if user has input packet.
/// @param -u32SrcSize \b IN :  The packet size if user has input packet.
/// @return -The result of command decode I frame.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_DecodeIFrame(MS_U32 u32Id, MS_PHYADDR u32SrcSt, MS_U32 u32SrcSize)
{
    MS_U32 timer = 300;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = MDrv_HVD_EX_Pause(u32Id);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    // skip decode I
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SKIP_DEC, E_HVD_EX_SKIP_DECODE_I);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_I_DIRECT, TRUE);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    eRet = MDrv_HVD_EX_StepDecode(u32Id);
    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    if (E_HVD_INIT_INPUT_DRV == (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK))
    {
        HVD_EX_PacketInfo packet = {0,0,0,0,0,0,0,0};

        if (u32SrcSize == 0)
        {
            eRet = E_HVD_EX_RET_INVALID_PARAMETER;
            HVD_EX_MSG_ERR("decode I frame input packet size is zero\n");
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        packet.u32Staddr = u32SrcSt - pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufAddr;
        packet.u32Length = u32SrcSize;

        if ((packet.u32Staddr + packet.u32Length) > pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32BitstreamBufSize)
        {
            eRet = E_HVD_EX_RET_INVALID_PARAMETER;
            HVD_EX_MSG_ERR
                ("decode I frame memory overflow, the packet end address is over ES buffer end address\n");
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }
        packet.u32TimeStamp = 0xFFFFFFFF;
        packet.u32ID_L = 0;
        packet.u32ID_H = 0;
        eRet = MDrv_HVD_EX_PushQueue(u32Id, &packet);
        if (E_HVD_EX_OK != eRet)
        {
            HVD_EX_MSG_ERR("decode I frame push queue fail %x\n", eRet);
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        HAL_HVD_EX_UpdateESWptr_Fire(u32Id);
    }
    while (timer)
    {
        HVD_Delay_ms(1);
        if (MDrv_HVD_EX_IsStepDecodeDone(u32Id))
        {
            break;
        }
        timer--;
    }
    if (timer == 0)
    {
        eRet = E_HVD_EX_FAIL;
        HVD_EX_MSG_ERR("decode I frame time out, not enough data\n");
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDataEnd()
/// @brief \b Function \b Description:  Upper layer set this to inform driver that there are no more data will be pushed.
/// @param -bEnd \b IN :  Enable/ Disable
///                 -FALSE(0): normal status( default )
///                 -TRUE(1): ending status
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDataEnd(MS_U32 u32Id, MS_BOOL bEnd)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (bEnd)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode |= HVD_CTRL_DATA_END;
    }
    else
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode &= ~HVD_CTRL_DATA_END;
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PLAYBACK_FINISH, bEnd);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDispErrFrm()
/// @brief \b Function \b Description:  Enable/ Disable to decode and show error(broken) frames
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): hide error frames
///                 -TRUE(1): show error frames
/// @return -The result of command set display error frames
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDispErrFrm(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_ERR_FRM, bEnable);
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bIsShowErrFrm = bEnable;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDispRepeatField()
/// @brief \b Function \b Description:  Enable/ Disable to show last field when FW needs to show repeated field
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): disable this mode
///                 -TRUE(1): enable this mode
/// @return -The result of command set display repeated field
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDispRepeatField(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_REPEAT_LAST_FIELD, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSkipDecMode()
/// @brief \b Function \b Description:  set the decoding frame type.
/// @param -eDecType \b IN : decoding frame type
/// @return -The result of command set skip decode mode
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSkipDecMode(MS_U32 u32Id, HVD_EX_SkipDecode eDecType)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if ((HVD_EX_SkipDecode) HAL_HVD_EX_GetData(E_HVD_GDATA_SKIP_MODE) != eDecType)
#endif
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SKIP_DEC, eDecType);

        if (eRet != E_HVD_EX_OK)
        {
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        if (eDecType == E_HVD_EX_SKIP_DECODE_I)
        {
            eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_I_DIRECT, TRUE);
        }
        else
        {
            eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_I_DIRECT, FALSE);
        }
    }

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u8SkipMode = (MS_U8) eDecType;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDispSpeed()
/// @brief \b Function \b Description:  specify the display speed type.
/// @param -eSpeed \b IN : display speed type
/// @return -The result of command set display speed type
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDispSpeed(MS_U32 u32Id, HVD_EX_DispSpeed eSpeed)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    HVD_EX_MSG_DBG("%d\n", (MS_S16) eSpeed);

#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if (HAL_HVD_EX_GetData(E_HVD_GDATA_DISPLAY_DURATION) != eSpeed)
#endif
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_SPEED, eSpeed);
    }
#if HVD_ENABLE_WAIT_CMD_FINISHED
    if (eRet == E_HVD_EX_OK)
    {
        MS_U32 timer = HVD_DRV_CMD_WAIT_FINISH_TIMEOUT;
        while (timer)
        {
            if (HAL_HVD_EX_GetData(E_HVD_GDATA_DISPLAY_DURATION) == eSpeed)
            {
                break;
            }
            HVD_Delay_ms(1);
            timer--;
        }
    }
#endif
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSyncActive()
/// @brief \b Function \b Description:  Enable/disable the sync of video time stamp and STC.
/// @param -bEnable \b IN : Enable/ Disable
///                 -FALSE(0): Disable sync mode
///                 -TRUE(1): Enable sync mode
/// @return -The result of command set sync active
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSyncActive(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

#if HVD_ENABLE_CHECK_STATE_BEFORE_SET_CMD
    if (HAL_HVD_EX_GetData(E_HVD_GDATA_IS_SYNC_ON) != bEnable)
#endif
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_ACTIVE, bEnable);
    }
#if HVD_ENABLE_WAIT_CMD_FINISHED
    if (eRet == E_HVD_EX_OK)
    {
        MS_U32 timer = HVD_DRV_CMD_WAIT_FINISH_TIMEOUT;
        while (timer)
        {
            if (HAL_HVD_EX_GetData(E_HVD_GDATA_IS_SYNC_ON) == bEnable)
            {
                break;
            }
            HVD_Delay_ms(1);
            timer--;
        }
    }
#endif
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bIsSyncOn = bEnable;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDropMode()
/// @brief \b Function \b Description:  specify the way to drop decoded frames.
/// @param -eMode \b IN : Drop display type.
/// @param -u32Arg \b IN : The argument of eMode
///     -( eMode == E_HVD_DROP_DISPLAY_AUTO)  , Enable: (u32Arg = TRUE); Disable: (u32Arg = FALSE)
///     -( eMode == E_HVD_EX_DROP_DISPLAY_ONCE)  , u32Arg = not zero
/// @return -The result of command set display speed type
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDropMode(MS_U32 u32Id, HVD_EX_DropDisp eMode, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (eMode == E_HVD_EX_DROP_DISPLAY_AUTO)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DROP_DISP_AUTO, u32Arg);
    }
    else if (eMode == E_HVD_EX_DROP_DISPLAY_ONCE)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DROP_DISP_ONCE, u32Arg);
    }
    else
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_RstPTS()
/// @brief \b Function \b Description:  Reset HVD sync table
/// @param -u32PTS \b IN : PTS base
/// @return -The result of command reset PTS
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_RstPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RESET_PTS, u32PTS);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetFrcMode()
/// @brief \b Function \b Description:  set the frame rate convert mode.
/// @param -eMode \b IN : mode type
/// @return -The result of command set frame rate convert mode
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetFrcMode(MS_U32 u32Id, HVD_EX_FrmRateConvMode eMode)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FRC_MODE, eMode);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u8FrcMode = (MS_U8) eMode;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSyncTolerance()
/// @brief \b Function \b Description:  Set the tolerance of FW reporting sync reach.
/// @param -u32Arg \b IN : tolerance.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSyncTolerance(MS_U32 u32Id, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_TOLERANCE, u32Arg);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u32SyncTolerance = u32Arg;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSyncVideoDelay()
/// @brief \b Function \b Description:  Set the video delay from STC when sync mode active.
/// @param -u32Arg \b IN : The video delay. unit:ms
/// @return -The result of command set sync video delay
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSyncVideoDelay(MS_U32 u32Id, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_LIVE_STREAM)
    {
        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u8TimeUnit)
        {
            eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, (MS_U32) (u32Arg + HVD_DTV_VIDEO_DELAY));
        }
        else
        {
            eRet =
                (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, u32Arg + ((MS_U32) HVD_DTV_VIDEO_DELAY * 90));
        }
    }
    else
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_VIDEO_DELAY, u32Arg);
    }

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u32SyncVideoDelay = u32Arg;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSyncFreeRunTH()
/// @brief \b Function \b Description:  Set the tolerance of FW reporting sync reach.
/// @param -u32Arg \b IN : theashold.
///                     - 0 : use FW default value
///                     - 0xFFFFFFFF : never free run, FW always do sync action.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSyncFreeRunTH(MS_U32 u32Id, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FREERUN_THRESHOLD, u32Arg);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u32SyncFreeRunTH = u32Arg;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSyncRepeatTH()
/// @brief \b Function \b Description:  Set the repeat threashold under sync mode.
/// @param -u32Arg \b IN : repeat times. 0x01 ~ 0xFF
///                 0xff - repeat current frame until STC catch up PTS.
/// @return -The result of command set sync repeat threashold
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSyncRepeatTH(MS_U32 u32Id, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SYNC_THRESHOLD, u32Arg);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u32SyncRepeatTH = u32Arg;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetErrConceal()
/// @brief \b Function \b Description:  Enable/Disable error concealment.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable error concealment.
///                 -TRUE(1): Enable error concealment.
/// @return -The result of command set sync tolerance
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetErrConceal(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ERR_CONCEAL, bEnable);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bIsErrConceal = bEnable;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDbgLevel()
/// @brief \b Function \b Description:  Set debug level
/// @param -elevel \b IN : debug level
//-----------------------------------------------------------------------------
void MDrv_HVD_EX_SetDbgLevel(HVD_EX_UartLevel eLevel)
{
    VPU_EX_UartLevel eVpuLevel = E_VPU_EX_UART_LEVEL_NONE;

    switch (eLevel)
    {
        case E_HVD_EX_UART_LEVEL_ERR:
        {
            u32UartCtrl = E_HVD_UART_CTRL_ERR;
            eVpuLevel = E_VPU_EX_UART_LEVEL_ERR;
            break;
        }
        case E_HVD_EX_UART_LEVEL_INFO:
        {
            u32UartCtrl = E_HVD_UART_CTRL_INFO | E_HVD_UART_CTRL_ERR;
            eVpuLevel = E_VPU_EX_UART_LEVEL_INFO;
            break;
        }
        case E_HVD_EX_UART_LEVEL_DBG:
        {
            u32UartCtrl = E_HVD_UART_CTRL_DBG | E_HVD_UART_CTRL_ERR | E_HVD_UART_CTRL_INFO;
            eVpuLevel = E_VPU_EX_UART_LEVEL_DBG;
            break;
        }
        case E_HVD_EX_UART_LEVEL_TRACE:
        {
            u32UartCtrl = E_HVD_UART_CTRL_TRACE | E_HVD_UART_CTRL_ERR | E_HVD_UART_CTRL_INFO | E_HVD_UART_CTRL_DBG;
            eVpuLevel = E_VPU_EX_UART_LEVEL_TRACE;
            break;
        }
        case E_HVD_EX_UART_LEVEL_FW:
        {
            u32UartCtrl = E_HVD_UART_CTRL_DISABLE;

            HAL_HVD_EX_UartSwitch2FW(TRUE);
            break;
        }
        default:
        {
            u32UartCtrl = E_HVD_UART_CTRL_DISABLE;
            eVpuLevel = E_VPU_EX_UART_LEVEL_ERR;
            break;
        }
    }
    HAL_VPU_EX_SetDbgLevel(eVpuLevel);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SeekToPTS()
/// @brief \b Function \b Description: Let FW decode to the input PTS by using full decoding speed. If FW reaches (equal to or larger than) input PTS, FW will pause automatically. This function can not work under MDrv_HVD_EX_SetSyncActive(TRUE).
/// @param -u32PTS \b IN : specific PTS.
///                 -0: disable this mode. FW will go back to previous status (play or pause).
///                 -any not zero: enable this mode
/// @return -The result of command seek to specific PTS.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SeekToPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = _HVD_EX_Check_Cmd(u8DrvId, E_HVD_CHECK_CMD_SEEK2PTS);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_JUMP_TO_PTS, u32PTS);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SkipToPTS()
/// @brief \b Function \b Description: Let FW skip to the input PTS by using full decoding speed and start decode again after input PTS.
/// @param -u32PTS \b IN : specific PTS.
///                 -0: disable this mode. FW will go back to previous status (play or pause).
///                 -any not zero: enable this mode
/// @return -The result of command seek to specific PTS.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SkipToPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SKIP_TO_PTS, u32PTS);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetFreezeImg()
/// @brief \b Function \b Description: Let FW stop updating frames when vsync, but decoding process is still going.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable freeze image.
///                 -TRUE(1): Enable freeze image.
/// @return -The result of command freeze image.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetFreezeImg(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FREEZE_IMG, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetBlueScreen()
/// @brief \b Function \b Description: Let FW stop updating frames when vsync and keep blue screen , but decoding process is still going.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable blue screen.
///                 -TRUE(1): Enable blue screen.
/// @return -The result of command set blue screen.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_BLUE_SCREEN, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDispOneField()
/// @brief \b Function \b Description: Let FW only show one field(top field only).
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable display one field.
///                 -TRUE(1): Enable display one field.
/// @return -The result of command display one field.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDispOneField(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (bEnable)
    {
        // force to show top field only.
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_ONE_FIELD, E_HVD_FIELD_CTRL_TOP);
    }
    else
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_ONE_FIELD, E_HVD_FIELD_CTRL_OFF);
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetISREvent()
/// @brief \b Function \b Description: Set the ISR event type sended by HVD fw.
/// @param -u32Event \b IN : event types
/// @param -fnISRHandler \b IN : function pointer to a interrupt handler.
/// @return -The result of command set ISR event.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetISREvent(MS_U32 u32Id, MS_U32 u32Event, HVD_InterruptCb fnISRHandler)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (u32Event == E_HVD_EX_ISR_NONE)
    {
        pCtrl->Settings.bEnISR = FALSE;
        ////OSAL_HVD_ISR_Disable();
        // Fix the ISR imbalance of Enable and Disable
        if(pCtrl->HVDISRCtrl.bIsHvdIsr == TRUE) // hvd case
        {
            if((pHVDDrvContext->bHVDDisableISRFlag != TRUE) && (FALSE == pHVDDrvContext->gHVDCtrl_EX[1-u8DrvId].bUsed))
            {
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
                HAL_HVD_EX_EnableISR(u32Id, FALSE);
#else
                HAL_HVD_EX_EnableISR(FALSE);
#endif
                pHVDDrvContext->bHVDDisableISRFlag = OSAL_HVD_ISR_Disable(TRUE);
                if(pHVDDrvContext->bHVDDisableISRFlag == FALSE)
                {
                    HVD_EX_MSG_ERR("%s Disable Interrupt Fail\n", __FUNCTION__);
                }
        //OSAL_HVD_ISR_Detach();
#if !defined(SUPPORT_X_MODEL_HVD_FEATURE) && !defined(DISABLE_ISR_DETACH)
                if(TRUE == pHVDDrvContext->bHVDIsIsrAttached)
                {
                    OSAL_HVD_ISR_Detach(pCtrl->HVDISRCtrl.bIsHvdIsr);
                    pHVDDrvContext->bHVDIsIsrAttached = FALSE;
                }
#endif
            }
        }
        else                                    // evd case
        {
            if((pHVDDrvContext->bEVDDisableISRFlag != TRUE) && (FALSE == pHVDDrvContext->gHVDCtrl_EX[1-u8DrvId].bUsed))
            {
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
                HAL_HVD_EX_EnableISR(u32Id, FALSE);
#else
                HAL_HVD_EX_EnableISR(FALSE);
#endif
                pHVDDrvContext->bEVDDisableISRFlag = OSAL_HVD_ISR_Disable(FALSE);
                if(pHVDDrvContext->bEVDDisableISRFlag == FALSE)
                {
                    HVD_EX_MSG_ERR("%s Disable Interrupt Fail\n", __FUNCTION__);
                }
        //OSAL_HVD_ISR_Detach();
#if !defined(SUPPORT_X_MODEL_HVD_FEATURE) && !defined(DISABLE_ISR_DETACH)
                if(TRUE == pHVDDrvContext->bEVDIsIsrAttached)
                {
                    OSAL_HVD_ISR_Detach(pCtrl->HVDISRCtrl.bIsHvdIsr);
                    pHVDDrvContext->bEVDIsIsrAttached = FALSE;
                }
#endif
            }
        }
        pCtrl->HVDISRCtrl.bRegISR = FALSE;
        pCtrl->HVDISRCtrl.pfnISRCallBack = NULL;
        pCtrl->Settings.u32IsrEvent = (MS_U32) E_HVD_EX_ISR_NONE;
        eRet = E_HVD_EX_OK;
    }
    else
    {
        if (fnISRHandler != NULL)
        {
#if 1 //If ISR handler has been registerred, we only need to reset event flag.
            if (pCtrl->HVDISRCtrl.bRegISR)
            {
                pCtrl->HVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack) fnISRHandler;
                eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ISR_TYPE, u32Event);

                if (eRet != E_HVD_EX_OK)
                {
                    _DRV_HVD_EX_RET(u8DrvId, eRet);
                }

                pCtrl->Settings.u32IsrEvent = u32Event;
                eRet = E_HVD_EX_OK;
            }
            else
            {
                pCtrl->HVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack) fnISRHandler;
                //OSAL_HVD_ISR_Attach((void *) _HVD_EX_ISRHandler);
                //OSAL_HVD_ISR_Enable();
                if (TRUE == pCtrl->HVDISRCtrl.bIsHvdIsr)
                {
                    if (FALSE == pHVDDrvContext->bHVDIsIsrAttached)
                    {
                        if (OSAL_HVD_ISR_Attach(pCtrl->HVDISRCtrl.bIsHvdIsr, (void*)_HVD_EX_ISRHandler) != TRUE)
                        {
                            HVD_EX_MSG_ERR("fail to attach HVD_IsrProc!\n");
                            return E_HVD_EX_FAIL;
                        }
                        HVD_PRINT("attach ISR number:%d\n", HVD_ISR_VECTOR);
                        pHVDDrvContext->bHVDIsIsrAttached = TRUE;
                    }
                }
                #if SUPPORT_EVD
                else //evd
                {
                    if (FALSE == pHVDDrvContext->bEVDIsIsrAttached)
                    {
                        if (OSAL_HVD_ISR_Attach(pCtrl->HVDISRCtrl.bIsHvdIsr, (void*)_HVD_EX_ISRHandler) != TRUE)
                        {
                            HVD_EX_MSG_ERR("fail to attach HVD_IsrProc!\n");
                            return E_HVD_EX_FAIL;
                        }
                #if SUPPORT_G2VP9
                        if (pCtrl->HVDISRCtrl.bIsG2Vp9Isr)
                        {
                            HVD_PRINT("attach ISR number:%d\n", -1/*VP9_ISR_VECTOR*/);
                        }
                        else
                #endif
                        {
                            HVD_PRINT("attach ISR number:%d\n", EVD_ISR_VECTOR);
                        }
                        pHVDDrvContext->bEVDIsIsrAttached = TRUE;
                    }
                }
                #endif

                if (OSAL_HVD_ISR_Enable(pCtrl->HVDISRCtrl.bIsHvdIsr) != TRUE)
                {
                    HVD_EX_MSG_ERR("fail to OSAL_HVD_IntEnable!\n");
                    return E_HVD_EX_FAIL;
                }

                eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ISR_TYPE, u32Event);

                if (eRet != E_HVD_EX_OK)
                {
                    _DRV_HVD_EX_RET(u8DrvId, eRet);
                }

                pCtrl->Settings.u32IsrEvent = u32Event;
                pCtrl->Settings.bEnISR = TRUE;
            #if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
                HAL_HVD_EX_EnableISR(u32Id, TRUE);
            #else
                HAL_HVD_EX_EnableISR(TRUE);
            #endif
                pCtrl->HVDISRCtrl.bRegISR = TRUE;
                eRet = E_HVD_EX_OK;
            }
#else

            HAL_HVD_EX_EnableISR(FALSE);

            if (pCtrl->HVDISRCtrl.bRegISR)
            {
                OSAL_HVD_ISR_Detach();
                pCtrl->HVDISRCtrl.bRegISR = FALSE;
            }

            pCtrl->HVDISRCtrl.pfnISRCallBack = (HVD_ISRCallBack) fnISRHandler;
            OSAL_HVD_ISR_Attach((void *) _HVD_EX_ISRHandler);
            OSAL_HVD_ISR_Enable();

            HVD_EX_MSG_INF("attach ISR number:%d\n", HVD_ISR_VECTOR);
            eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ISR_TYPE, u32Event);
            pCtrl->Settings.u32IsrEvent = u32Event;

            if (eRet != E_HVD_EX_OK)
            {
                _DRV_HVD_EX_RET(u8DrvId, eRet);
            }

            HAL_HVD_EX_EnableISR(TRUE);
            pCtrl->HVDISRCtrl.bRegISR = TRUE;
            eRet = E_HVD_EX_OK;
#endif
        }
        else
        {
            HVD_EX_MSG_ERR("SetISREvent with NULL pointer. ISR type:%lu\n", u32Event);
            eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        }
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetEnableISR()
/// @brief \b Function \b Description: Enable/Disable HVD interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Disable interrupt.
///                 -TRUE(1): Enable interrupt.
/// @return -The result of command set enable ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_SetEnableISR(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    HAL_HVD_EX_EnableISR(u32Id, bEnable);
#else
    HAL_HVD_EX_EnableISR(bEnable);
#endif
    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bEnISR = bEnable;

    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetForceISR()
/// @brief \b Function \b Description: test the HVD interrupt function. Force FW send one interrupt.
/// @param -bEnable \b IN : Enable/Disable
///                 -FALSE(0): Clear force interrupt status from HK.
///                 -TRUE(1): force one interrupt from HK.
/// @return -The result of command set force ISR.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_MSG_TRACE();
    //_DRV_HVD_Inited(FALSE);
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    HAL_HVD_EX_SetForceISR(u32Id, bEnable);
#else
    UNUSED(u32Id);
    HAL_HVD_EX_SetForceISR(bEnable);
#endif
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetMVOPDone()
/// @brief \b Function \b Description: tell FW MVOP is ready for futher decode.
/// @return -The result of command.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_SetMVOPDone(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_BLUE_SCREEN, FALSE);

    if (E_HVD_EX_OK == eRet)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetVirtualBox()
/// @brief \b Function \b Description:  Set DS width and Height to F/W
/// @param -u16Width \b IN :  frame width
/// @param -u16Height \b IN :  frame height
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_VIRTUAL_BOX_WIDTH, u16Width);
    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_VIRTUAL_BOX_HEIGHT, u16Height);

    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDynScalingParam()
/// @brief \b Function \b Description: Pass scalar parameters to decoder
/// @return -The result of command.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDynScalingParam(MS_U32 u32Id, void *pStAddr, MS_U32 u32Size)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U32 addr = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if ((pStAddr == NULL) || (u32Size == 0))
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_Inited(u8DrvId,eRet);
    }
    else
    {
        // 1. copy data input data array
        addr = MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_INFO_ADDR);

        if(addr == 0x0)
        {
            eRet = E_HVD_EX_FAIL;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        HVD_memcpy(MS_PA2KSEG1(addr), pStAddr, u32Size);

        // 2. while till FW finish it.
         eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SCALER_INFO_NOTIFY, ((MS_U8 *)pStAddr)[0]);
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDispInfoTH()
/// @brief \b Function \b Description:  Set the upper and lower limitation of a valid SPS.
/// @param -DispInfoTH \b IN : tolerance.
/// @return -The result of command set display inforation threshold
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDispInfoTH(MS_U32 u32Id, HVD_EX_DispInfoThreshold *DispInfoTH)
{
    HVD_Disp_Info_TH DispInfoTHTmp;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (DispInfoTH == NULL)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    DispInfoTHTmp.u32FrmrateLowBound = DispInfoTH->u32FrmrateLowBound;
    DispInfoTHTmp.u32FrmrateUpBound = DispInfoTH->u32FrmrateUpBound;
    DispInfoTHTmp.u32MvopLowBound = DispInfoTH->u32MvopLowBound;
    DispInfoTHTmp.u32MvopUpBound = DispInfoTH->u32MvopUpBound;

    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DISP_INFO_TH, (MS_U32) (&DispInfoTHTmp));

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISP_THRESHOLD, 0);

    HVD_memcpy((void *) &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.DispInfoTH), (void *) (DispInfoTH), sizeof(HVD_EX_DispInfoThreshold));

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetIgnoreErrRef()
/// @brief \b Function \b Description:  Turn on / off ignore error reference.
/// @param -b bFastDisplay \b IN :  Enable/ Disable
///                 -FALSE(0): handle error reference
///                 -TRUE(1): ignore error reference
/// @return -The result of command set ignore error reference
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetIgnoreErrRef(MS_U32 u32Id, MS_BOOL bIgnore)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_ERR_REF, bIgnore);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_ForceFollowDTVSpec()
/// @brief \b Function \b Description:  Turn on / off Force follow DTV Spec
/// @param -b bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): Disable
///                 -TRUE(1): Enable
/// @return -The result of command set force follow DTV spec
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_ForceFollowDTVSpec(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_DTV_SPEC, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetFastDisplay()
/// @brief \b Function \b Description:  Turn on / off fast display.
/// @param -b bFastDisplay \b IN :  Enable/ Disable
///                 -FALSE(0): normal display
///                 -TRUE(1): fast display
/// @return -The result of command set fast display
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetFastDisplay(MS_U32 u32Id, MS_BOOL bFastDisplay)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FAST_DISP, bFastDisplay);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DispOutsideMode()
/// @brief \b Function \b Description:  Turn on / off Disp control method which handle by app side
/// @param -b bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): Disable
///                 -TRUE(1): Enable
/// @return -
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_DispOutsideMode(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);
    _DRV_HVD_SetCtrl(u8DrvId, HVD_CTRL_DISP_OUTSIDE);
//change to open max setting ,wait f/w release new fwHVD_if
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ENABLE_DISP_OUTSIDE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}
// check status

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsISROccured()
/// @brief \b Function \b Description:  Check if the ISR is occured or not.
/// @return - Is occured or not
/// @retval     -FALSE(0): interrupt is not occured.
/// @retval     -TRUE(1): interrupt has been occured.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsISROccured(MS_U32 u32Id)
{
    HVD_EX_MSG_TRACE();
    //_DRV_HVD_Inited(FALSE);
#if defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MADISON) || defined(CHIP_MIAMI) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_MUJI) || defined(CHIP_MUNICH) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    return HAL_HVD_EX_IsISROccured(u32Id);
#else
    UNUSED(u32Id);
    return HAL_HVD_EX_IsISROccured();
#endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsDispFinish()
/// @brief \b Function \b Description:  Check this file mode display is finish or not. only work after MDrv_HVD_EX_SetDataEnd(TRUE)
/// @return - Is finish or not
/// @retval     -FALSE(0): Not finish or Not in file mode playback
/// @retval     -TRUE(1): Display Finished.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsDispFinish(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_LIVE_STREAM)
    {
        return FALSE;
    }

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_DATA_END)
    {
        if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK) == HVD_INIT_HW_VP8)
        {
            if (_HVD_EX_IsAllBufferEmpty_VP8(u32Id))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if (_HVD_EX_IsAllBufferEmpty(u32Id))
            {
                return TRUE;
            }
            else
            {
                //_DRV_HVD_PushDummy();
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsFrameShowed()
/// @brief \b Function \b Description:  Is HVD Frame showed after TriggerDisp(TRUE) or first frame showed
/// @return - Is frame showed or not
/// @retval     -FALSE(0): New Framed showed
/// @retval     -TRUE(1): Not showed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsFrameShowed(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (!HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_1ST_FRM_RDY))
    {
        // 1st frame not ready or AV sync not ready
        return FALSE;
    }
    else
    {
        // 1st frame showed or AV sync ready
        if (_DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISPLAY_CTRL))
        {
            // control display
            return (MS_BOOL) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_FRAME_SHOWED);
        }
        else
        {
            // cotrol decode
            return TRUE;
        }
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsStepDecodeDone()
/// @brief \b Function \b Description:  Is HVD step decode done after step decode command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send corresponding step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsStepDecodeDone(MS_U32 u32Id)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bStepDecoding)
    {
        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32StepDecodeCnt !=
            HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT))
        {
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32StepDecodeCnt = 0;
            bRet = TRUE;
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bStepDecoding = FALSE;
        }
    }

    return bRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_CheckDispInfoRdy()
/// @brief \b Function \b Description:  check display info ready and correct or not
/// @return -The result of command check display infor ready or not
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_CheckDispInfoRdy(MS_U32 u32Id)
{
    MS_U32 u32ErrCode = 0;
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);

    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_DISP_INFO_RDY))
    {
        if (!HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_DISP_INFO_CHANGE))
        {
            return E_HVD_EX_RET_NOTREADY;
        }
        else
        {
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bIsDispInfoChg = TRUE;
            _DRV_HVD_SetCtrl(u8DrvId, HVD_CTRL_DISP_INFO_RDY);
        }
    }

    // check if FW report error
    u32ErrCode = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ERROR_CODE);

    if (u32ErrCode != 0)
    {
        if (u32ErrCode == E_HVD_ERR_OUT_OF_MEMORY)
        {
            HVD_EX_MSG_ERR
                ("FW error that Out of Memory:%lx. Allocated frame buffer size is smaller than required.\n",
                 u32ErrCode);
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bIsDispInfoChg = FALSE;
            return E_HVD_EX_RET_OUTOF_MEMORY;
        }
    }

    return E_HVD_EX_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsDispInfoChg()
/// @brief \b Function \b Description:  check display info is changed or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): not changed
/// @retval     -TRUE(1): changed
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsDispInfoChg(MS_U32 u32Id)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    bRet = (MS_BOOL) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_DISP_INFO_CHANGE);

    if (bRet)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bIsDispInfoChg = TRUE;
        _DRV_HVD_SetCtrl(u8DrvId, HVD_CTRL_DISP_INFO_RDY);
    }
#if 0                           // debug use
    {
        static MS_U32 u32DecodeCnt = 0;
        static MS_U32 u32IdleCnt = 0;
        MS_U32 tmp = 0;
        tmp = MDrv_HVD_EX_GetDecodeCnt(u32Id);

        if (u32DecodeCnt != tmp)
        {
            HVD_PRINT("%lu decode cnt:%lx PTS:%lx\n", (MS_U32) bRet, tmp, MDrv_HVD_EX_GetPTS(u32Id));
            u32DecodeCnt = tmp;
        }
        else
        {
            u32IdleCnt++;
        }
        if (u32IdleCnt > 1000)
        {
            HVD_PRINT("HVD: seems IDLE: %lu decode cnt:%lx PTS:%lx\n", (MS_U32) bRet, tmp, MDrv_HVD_EX_GetPTS(u32Id));
            u32IdleCnt = 0;
        }
    }
#endif
    return bRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsIdle()
/// @brief \b Function \b Description:  check decoder is idle or not
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): decoder is not idle
/// @retval     -TRUE(1): decoder is idle
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsIdle(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT) > HVD_FW_IDLE_THRESHOLD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsSyncStart()
/// @brief \b Function \b Description:  check decoder starts to do sync action(drop or repeat) or not.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): decoder is not doing sync action
/// @retval     -TRUE(1): decoder is doing sync action
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsSyncStart(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if ((MS_BOOL) MDrv_HVD_EX_GetPlayMode(u32Id, E_HVD_EX_GMODE_IS_SYNC_ON) == FALSE)
    {
        return FALSE;
    }
    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_SYNC_START))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsSyncReach()
/// @brief \b Function \b Description: The distance of Video time stamp and STC is closed enough.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): The distance is not shorter than sync tolerance.
/// @retval     -TRUE(1): The distance is shorter than sync tolerance.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsSyncReach(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if ((MS_BOOL) MDrv_HVD_EX_GetPlayMode(u32Id, E_HVD_EX_GMODE_IS_SYNC_ON) == FALSE)
    {
        return FALSE;
    }

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_SYNC_REACH))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsLowDelay()
/// @brief \b Function \b Description:  check if current stream has low delay flag in SPS.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Low delay flag not found.
/// @retval     -TRUE(1): Low delay flag found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsLowDelay(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_AVC_LOW_DELAY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsIFrmFound()
/// @brief \b Function \b Description:  check if I frame found after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): I frame is not found.
/// @retval     -TRUE(1): I frame has been found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsIFrmFound(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_I_FRM_FOUND))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Is1stFrmRdy()
/// @brief \b Function \b Description:  check if first frame showed on screen after Init() or flush().
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): First frame is not showed.
/// @retval     -TRUE(1): First frame is showed.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_Is1stFrmRdy(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_1ST_FRM_RDY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsAllBufferEmpty()
/// @brief \b Function \b Description:  check if all of the buffers(display, decoded, BBU, bitstream) are empty.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Not Empty.
/// @retval     -TRUE(1): Empty.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_IsAllBufferEmpty(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    //HVD_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (_HVD_EX_IsAllBufferEmpty(u32Id))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#if 0
    if (u32Times > 200)
    {
        HVD_EX_MSG_INF("Flush() timeout failed:BBU:%lu Dec:%lu Disp:%lu Idle:%lu ESr:%lx ESw:%lx  \n",
                     HAL_HVD_EX_GetData(E_HVD_GDATA_BBU_Q_NUMB),
                     HAL_HVD_EX_GetData(E_HVD_GDATA_DEC_Q_NUMB),
                     HAL_HVD_EX_GetData(E_HVD_GDATA_DISP_Q_NUMB),
                     HAL_HVD_EX_GetData(E_HVD_GDATA_VPU_IDLE_CNT),
                     HAL_HVD_EX_GetData(E_HVD_GDATA_ES_READ_PTR), HAL_HVD_EX_GetData(E_HVD_GDATA_ES_WRITE_PTR));
        eRet = E_HVD_EX_FAIL;
        break;
    }
    return TRUE;
#endif
}

// get infomation

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetBBUVacancy()
/// @brief \b Function \b Description:  get the vacancy of BBU queue.
/// @return - TRUE / FALSE
/// @retval     -0: queue is full.
/// @retval     -not zero: queue is not full.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetBBUVacancy(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32BBUQnumb = 0;
    MS_U32 u32BBUTotal = 0;
    MS_U32 u32PTSQnumb = 0;
    MS_U32 u32PTSTotal = 0;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32BBUTotal = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_BBU_TOTAL_TBL_ENTRY);
    u32BBUQnumb = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB);

    if (u32BBUTotal < u32BBUQnumb)
    {
        HVD_EX_MSG_ERR("Total BBUs(%lx) is less than the current(%lx)\n", u32BBUTotal, u32BBUQnumb);
        u32BBUQnumb = 0;
    }
    else
    {
        u32BBUQnumb = u32BBUTotal - u32BBUQnumb;

        if (u32BBUQnumb < MIN_BBU_VACANCY_FOR_4K2K)
        {
            HVD_EX_MSG_DBG("BBU Vacancy(%lx) is less than MIN_BBU_VACANCY_FOR_4K2K(%x)\n", u32BBUQnumb, MIN_BBU_VACANCY_FOR_4K2K);
            u32BBUQnumb = 0;
        }
    }

#if HVD_ENABLE_MVC
    if ( u32BBUQnumb &&
         ( (HAL_HVD_EX_CheckMVCID(u32Id) == FALSE) ||
          ((HAL_HVD_EX_CheckMVCID(u32Id) == TRUE) && (E_VDEC_EX_MAIN_VIEW == HAL_HVD_EX_GetView(u32Id))) )
        )
#else
    if (u32BBUQnumb)
#endif
    {
        u32PTSTotal = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_PTS_TOTAL_ENTRY_NUMB) - 1; // add -1 for full case
        u32PTSQnumb = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_PTS_Q_NUMB);

        if (u32PTSTotal <= u32PTSQnumb)
        {
            HVD_EX_MSG_DBG
                ("Warn: GetBBUVacancy has error that total PTS Q number(%lx) is smaller than current PTS Q number(%lx)\n",
                 u32PTSTotal, u32PTSQnumb);
            u32BBUQnumb = 0;
        }
    }

    return u32BBUQnumb;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDispInfo()
/// @brief \b Function \b Description:  Get video display information
/// @param -pinfo \b OUT : pointer to video display information.
/// @return -The result of command get display information
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GetDispInfo(MS_U32 u32Id, HVD_EX_DispInfo *pInfo)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    eRet = _HVD_EX_GetDispInfo(u32Id, pInfo, TRUE);
    if (E_HVD_EX_OK == eRet)
    {
        HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
        pCtrl->bFrmRateSupported = HAL_HVD_EX_GetFrmRateIsSupported(u32Id, pInfo->u16HorSize, pInfo->u16VerSize, pInfo->u32FrameRate);
        HVD_EX_MSG_DBG("bFrmRateSupported[0x%x]=%x\n", u8DrvId, pCtrl->bFrmRateSupported);
    }

#if HVD_ENABLE_RV_FEATURE
    if (HVD_INIT_HW_RM == (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
    {
        //RM is always displayed by 60 frames per sec.
        pInfo->u32FrameRate = 60000;
        HVD_EX_MSG_DBG("rm force set 60fps\n");
    }
#endif

    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPtsStcDiff()
/// @brief \b Function \b Description: Get the difference of PTS and STC
/// @return - PTS
//-----------------------------------------------------------------------------
MS_S64 MDrv_HVD_EX_GetPtsStcDiff(MS_U32 u32Id)
{
    MS_S64 s64PTS = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    s64PTS = HAL_HVD_EX_GetData_EX(u32Id, E_HVD_GDATA_PTS_STC_DIFF);

    return s64PTS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPTS()
/// @brief \b Function \b Description:  get the pts of current displayed video frame. unit: ms
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetPTS(MS_U32 u32Id)
{
    MS_U32 u32PTS = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32PTS = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_PTS);

    u32PTS = _HVD_EX_ReportLow32BitPTS(u32Id, u32PTS);

    return u32PTS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPTS()
/// @brief \b Function \b Description:  get the 33 bits pts of current displayed video frame. unit: ms
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U64 MDrv_HVD_EX_GetU64PTS(MS_U32 u32Id)
{
    MS_U64 u64PTS = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    u64PTS = *((MS_U64*)(HAL_HVD_EX_GetData(u32Id,E_HVD_GDATA_U64PTS)));

    return u64PTS;
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetNextPTS()
/// @brief \b Function \b Description:  get the pts of next displayed video frame. unit: ms
/// @return - PTS
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetNextPTS(MS_U32 u32Id)
{
    MS_U32 u32PTS = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32PTS = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_NEXT_PTS);

    u32PTS = _HVD_EX_ReportLow32BitPTS(u32Id, u32PTS);

    return u32PTS;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetNextDispQPtr()
/// @brief \b Function \b Description:  get the pointer of next displayed video frame.
/// @return - Pointer in the display queue
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetNextDispQPtr(MS_U32 u32Id)
{
    MS_U32 u32Ptr = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    //HVD_PRINT("GetNextDispQPtr DqNum=%ld, DqPtr=%ld\n", HAL_HVD_EX_GetData(E_HVD_GDATA_DISP_Q_NUMB), HAL_HVD_EX_GetData(E_HVD_GDATA_DISP_Q_PTR));
    //if (HAL_HVD_EX_GetData(E_HVD_GDATA_DISP_Q_NUMB) != 0)
    {
        u32Ptr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_PTR);
    }

    return u32Ptr;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDataErrCnt()
/// @brief \b Function \b Description:  get accumulated data Error Count
/// @return -data error count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDataErrCnt(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DATA_ERROR_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDecErrCnt()
/// @brief \b Function \b Description:  get accumulated decode Error Count
/// @return -decode error count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDecErrCnt(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_ERROR_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetESWritePtr()
/// @brief \b Function \b Description:  Get Elementary Stream buffer write point
/// @return - ES buffer write point offset from bitstream buffer base
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetESWritePtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_WRITE_PTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetESReadPtr()
/// @brief \b Function \b Description:  Get Elementary Stream buffer read point
/// @return - ES buffer read point offset from bitstream buffer base
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetESReadPtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_READ_PTR);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetESLevel()
/// @brief \b Function \b Description:  Get Elementary Stream buffer level
/// @return - ES buffer level (ES data size in bitstream buffer)
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetESQuantity(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_QUANTITY);
}

#define FATAL_ERROR(x) ((x)==E_HVD_RETURN_OUTOF_MEMORY)

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetErrCode()
/// @brief \b Function \b Description:  get error code
/// @return - error code number
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetErrCode(MS_U32 u32Id)
{
// TODO: define driver error code  for upper layer
    MS_U32 u32Ret = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ERROR_CODE);

    u32Ret = _HVD_EX_Map2HVDErrCode(u32Ret);

#ifdef SUPPORT_X_MODEL_FEATURE //We using the common compile option to handle X model
    if (!FATAL_ERROR(u32Ret))
#else
    if (u32Ret)
#endif
    {
        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_ERROR_CODE, 0);
    }

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32LastErrCode = u32Ret;

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPlayMode()
/// @brief \b Function \b Description:  Get current play mode status.
/// @param -eMode \b IN : Mode type.
/// @return - mode status
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetPlayMode(MS_U32 u32Id, HVD_EX_GetModeStatus eMode)
{
    MS_U32 u32Ret = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    switch (eMode)
    {
        case E_HVD_EX_GMODE_IS_SHOW_ERR_FRM:
        case E_HVD_EX_GMODE_IS_REPEAT_LAST_FIELD:
        case E_HVD_EX_GMODE_IS_ERR_CONCEAL:
        case E_HVD_EX_GMODE_IS_SYNC_ON:
        case E_HVD_EX_GMODE_IS_PLAYBACK_FINISH:
        case E_HVD_EX_GMODE_SYNC_MODE:
        case E_HVD_EX_GMODE_SKIP_MODE:
        case E_HVD_EX_GMODE_DROP_MODE:
        case E_HVD_EX_GMODE_DISPLAY_SPEED:
        case E_HVD_EX_GMODE_FRC_MODE:
            // TODO: add isr type here
        case E_HVD_EX_GMODE_ISR_TYPE:
            u32Ret = HAL_HVD_EX_GetData(u32Id, (HVD_GetData) ((MS_U32) eMode + (MS_U32) E_HVD_GDATA_IS_SHOW_ERR_FRM));
            break;
        case E_HVD_EX_GMODE_IS_STEP_DISPLAY:
            u32Ret = _DRV_HVD_Ctrl(u8DrvId, HVD_CTRL_DISPLAY_CTRL);
            break;
        case E_HVD_EX_GMODE_STREAM_TYPE:
            u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK;
            break;
        default:
            break;
    }

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPlayState()
/// @brief \b Function \b Description:  get current play state
/// @return - play state
//-----------------------------------------------------------------------------
HVD_EX_GetPlayState MDrv_HVD_EX_GetPlayState(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32FWstate = 0;
    HVD_EX_GetPlayState eRet = E_HVD_EX_GSTATE_INIT;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);

    u32FWstate = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATE);
    u32FWstate &= E_HVD_FW_STATE_MASK;

    switch (u32FWstate)
    {
        case E_HVD_FW_INIT:
            eRet = E_HVD_EX_GSTATE_INIT;
            break;
        case E_HVD_FW_PLAY:
            eRet = E_HVD_EX_GSTATE_PLAY;
            break;
        case E_HVD_FW_PAUSE:
            eRet = E_HVD_EX_GSTATE_PAUSE;
            break;
        case E_HVD_FW_STOP:
            eRet = E_HVD_EX_GSTATE_STOP;
            break;
        default:
            break;
    }
    return eRet;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDecodeCnt()
/// @brief \b Function \b Description:  get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDecodeCnt(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetActiveFormat()
/// @brief \b Function \b Description:  Get current AFD ID
/// @return - AFD ID, 0xFF:invalid value
//-----------------------------------------------------------------------------
MS_U8 MDrv_HVD_EX_GetActiveFormat(MS_U32 u32Id)
{
    HVD_Display_Info *pDispInfo = NULL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();

    _DRV_HVD_Inited(u8DrvId,~0);

    if (!(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_DISP_INFO_RDY))
    {
        return 0;
    }

    pDispInfo = (HVD_Display_Info *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_INFO_ADDR);

    if (pDispInfo != NULL)
    {
        return pDispInfo->u8AFD;
    }
    else
    {
        return ~0;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetInfo()
/// @brief \b Function \b Description:  Get information of HVD driver.
/// @return - driver information
//-----------------------------------------------------------------------------
const HVD_EX_DrvInfo *MDrv_HVD_EX_GetInfo(void)
{
    DrvInfo.bAVC = MDrv_HVD_EX_GetCaps(E_HVD_EX_AVC);
    DrvInfo.bAVS = MDrv_HVD_EX_GetCaps(E_HVD_EX_AVS);
    DrvInfo.bRM = MDrv_HVD_EX_GetCaps(E_HVD_EX_RM);
    DrvInfo.FWversion = HVD_FW_VERSION;
    return &DrvInfo;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetLibVer()
/// @brief \b Function \b Description:  Get verion ID of HVD library.
/// @param -pVerString \b OUT : pointer to HVD driver version ID.
/// @return - driver library verion ID
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GetLibVer(const MSIF_Version ** ppVersion)
{
    if (!ppVersion)
    {
        return E_HVD_EX_FAIL;
    }

    *ppVersion = &_drv_hvd_version;
    return E_HVD_EX_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetStatus()
/// @brief \b Function \b Description:  Get status of HVD driver
/// @param -pstatus \b OUT : driver status
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Low delay flag not found.
/// @retval     -TRUE(1): Low delay flag found.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetStatus(MS_U32 u32Id, HVD_EX_DrvStatus *pstatus)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    if (pstatus == NULL)
    {
        return FALSE;
    }

    pstatus->bInit = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_INIT_FINISHED;
    pstatus->bBusy = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32CtrlMode & HVD_CTRL_PROCESSING;

    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetFrmInfo()
/// @brief \b Function \b Description:  Get current displayed or decoded frame information of HVD driver
/// @param -eType \b IN : Type of frame information
/// @param -pInfo \b OUT : frame information
/// @return -The result of command get frame information
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GetFrmInfo(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType, HVD_EX_FrameInfo * pInfo)
{
    HVD_Frm_Information *pFrmInfo = NULL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,E_HVD_EX_FAIL);

    if (pInfo == NULL)
    {
        return E_HVD_EX_FAIL;
    }

    if (eType == E_HVD_EX_GFRMINFO_DISPLAY)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_FRM_INFO);
    }
    else if (eType == E_HVD_EX_GFRMINFO_DECODE)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_FRM_INFO);
    }
    else if (eType == E_HVD_EX_GFRMINFO_NEXT_DISPLAY)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_NEXT_DISP_FRM_INFO);
    }
#if HVD_ENABLE_MVC
    else if (eType == E_HVD_EX_GFRMINFO_DISPLAY_SUB)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_FRM_INFO_SUB);
    }
    else if (eType == E_HVD_EX_GFRMINFO_DECODE_SUB)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_FRM_INFO_SUB);
    }
#endif ///HVD_ENABLE_MVC
    else if (eType == E_HVD_EX_GFRMINFO_LAST_DISPLAY)
    {
        pFrmInfo = (HVD_Frm_Information *) pHVDDrvContext->gHVDCtrl_EX[u8DrvId].pLastFrmInfo;
    }

    if (pFrmInfo != NULL)
    {
        pInfo->u32LumaAddr = (MS_PHYADDR) (pFrmInfo->u32LumaAddr);
        pInfo->u32ChromaAddr = (MS_PHYADDR) (pFrmInfo->u32ChromaAddr);

        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_1/*u32CtrlMode & HVD_CTRL_FRM_MIU_1*/)
        {
            pInfo->u32LumaAddr += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr;
            pInfo->u32ChromaAddr += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr;
        }
        else if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_2)
        {
            pInfo->u32LumaAddr += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU2BaseAddr;
            pInfo->u32ChromaAddr += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU2BaseAddr;
        }

        pInfo->u32TimeStamp = pFrmInfo->u32TimeStamp;
        pInfo->u32ID_L = pFrmInfo->u32ID_L;
        pInfo->u32ID_H = pFrmInfo->u32ID_H;
        pInfo->u32PrivateData = pFrmInfo->u32PrivateData; //[STB]only for AVC
        pInfo->u16Pitch = pFrmInfo->u16Pitch;
        pInfo->u16Height = pFrmInfo->u16Height;
        pInfo->u16Width = pFrmInfo->u16Width;
        pInfo->eFrmType = (HVD_EX_FrmType) (pFrmInfo->u8FrmType);
        pInfo->eFieldType = (HVD_EX_FieldType) (pFrmInfo->u8FieldType);

        pInfo->u32LumaAddr_2bit = (MS_PHYADDR) (pFrmInfo->u32LumaAddr_2bit);
        pInfo->u32ChromaAddr_2bit = (MS_PHYADDR) (pFrmInfo->u32ChromaAddr_2bit);

        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_1/*u32CtrlMode & HVD_CTRL_FRM_MIU_1*/)
        {
            pInfo->u32LumaAddr_2bit += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr;
            pInfo->u32ChromaAddr_2bit += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr;
        }
        else if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_2)
        {
            pInfo->u32LumaAddr_2bit += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU2BaseAddr;
            pInfo->u32ChromaAddr_2bit += pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU2BaseAddr;
        }

        pInfo->u16Pitch_2bit = pFrmInfo->u16Pitch_2bit;
        pInfo->u8LumaBitdepth = pFrmInfo->u8LumaBitdepth;
        pInfo->u8ChromaBitdepth = pFrmInfo->u8ChromaBitdepth;

        if ((pInfo->u16Pitch == 0) && (pInfo->u16Width == 0) && (pInfo->u16Height == 0))
        {
            return E_HVD_EX_FAIL;
        }

        if (eType == E_HVD_EX_GFRMINFO_NEXT_DISPLAY)
        {
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].pLastFrmInfo = pFrmInfo;
        }
    }
    else
    {
        return E_HVD_EX_FAIL;
    }

    return E_HVD_EX_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetISRInfo()
/// @brief \b Function \b Description:  Get information of HVD driver interrupt
/// @param -eType \b OUT : ISR information
/// @return -the result of get ISR information
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetISRInfo(MS_U32 u32Id, MS_U32 *eType)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    _DRV_HVD_Inited(u8DrvId,FALSE);

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].HVDISRCtrl.bInISR)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].HVDISRCtrl.u32ISRInfo |= HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_HVD_ISR_STATUS);
        *eType = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].HVDISRCtrl.u32ISRInfo;
    }
    else
    {
        *eType = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_HVD_ISR_STATUS);
    }

    HVD_EX_MSG_DBG("ISR=0x%lx\n", *eType);

    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_CalLumaSum()
/// @brief \b Function \b Description:  Get the sum of luma data in a frame.
/// @param -eType \b IN : Type of frame information
/// @return -the sum
/// @retval     -0xFFFFFFFF: error occer.
/// @retval     -not zero: the sum.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_CalLumaSum(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType)
{
    HVD_Frm_Information *pFrmInfo = NULL;
    MS_U32 u32Ret = HVD_U32_MAX;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,u32Ret);

    if (eType == E_HVD_EX_GFRMINFO_DISPLAY)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_FRM_INFO);
    }
    else if (eType == E_HVD_EX_GFRMINFO_DECODE)
    {
        pFrmInfo = (HVD_Frm_Information *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DEC_FRM_INFO);
    }

    if (pFrmInfo != NULL)
    {
        MS_U32 u32tmp = 0;
        MS_U32 u32PitchCnt = 0;
        MS_U32 u32HeightCnt = 0;
        volatile MS_U8 *pLumaData = NULL;
        // PA2VA
        u32tmp = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32FrameBufAddr;

        if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_1/*u32CtrlMode & HVD_CTRL_FRM_MIU_1*/)
        {
            u32tmp -= pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr;
        }
        else if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8FrmMiuSel == E_CHIP_MIU_2)
        {
            u32tmp -= pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU2BaseAddr;
        }

        pLumaData = (volatile MS_U8 *) (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32FrameBufVAddr + (pFrmInfo->u32LumaAddr - u32tmp));

        for (u32HeightCnt = 0; u32HeightCnt < pFrmInfo->u16Height; u32HeightCnt++)
        {
            for (u32PitchCnt = 0; u32PitchCnt < pFrmInfo->u16Pitch; u32PitchCnt++)
            {
                if (u32PitchCnt < pFrmInfo->u16Pitch)
                {
                    u32Ret += (MS_U32) (*pLumaData);
                }
                pLumaData++;
            }
        }
    }

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetUserData_Wptr()
/// @brief \b Function \b Description:  Get write pointer of user data.
/// @return -the information of write pointer of user data.
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetUserData_Wptr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32Ret = 0;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32Ret = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_WPTR);

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetUserData_Packet()
/// @brief \b Function \b Description:  Get information of user data packet.
/// @param -u32Idx \b IN : the pointer of required user data packet( valid range is 0 ~ 11 )
/// @param -u32Size \b OUT : the size of required user data packet
/// @return -the offset of user data packet form code buffer start address
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetUserData_Packet(MS_U32 u32Id, MS_U32 u32Idx, MS_U32 *u32Size)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32Ret = 0;
    MS_U32 tmp = 0;
    MS_U8 *pIdx = NULL;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    *u32Size = 0;
    tmp = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);

    if (u32Idx >= tmp)
    {
        HVD_EX_MSG_ERR("input user data index(%lu) is larger than max index(%lu)\n", u32Idx, tmp);
        return 0;
    }
    tmp = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_IDX_TBL_ADDR);
    if (tmp == 0)
    {
        HVD_EX_MSG_INF("HVD FW ERR: user data packet idx tbl base addr(%lx) is zero\n", tmp);
        return 0;
    }
    pIdx = (MS_U8 *) (tmp + u32Idx);
    tmp = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_PACKET_TBL_SIZE);
    if ((*pIdx) >= tmp)
    {
        HVD_EX_MSG_INF("HVD FW ERR: user data packet tbl ptr(%lu) is larger than max size(%lu)\n", (MS_U32) (*pIdx), tmp);
        return 0;
    }
    tmp = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_PACKET_TBL_ADDR);
    if (tmp == 0)
    {
        HVD_EX_MSG_INF("HVD FW ERR: user data packet packet tbl base offset(%lx) is zero\n", tmp);
        return 0;
    }
    u32Ret = tmp;
    tmp = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_PACKET_SIZE);
    if (tmp == 0)
    {
        HVD_EX_MSG_INF("HVD FW ERR: user data packet packet size(%lu) is zero\n", tmp);
        return 0;
    }
    *u32Size = tmp;
    u32Ret += (MS_U32) (*pIdx) * tmp;
    return u32Ret;
}

// VDEC Interal control
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GenPattern()
/// @brief \b Function \b Description:  Generate spcific pattern to support some special function.
/// @param -eType \b IN : the virtual address of spcific pattern
/// @param -u32VAddr \b IN : the virtual address of spcific pattern
/// @param -u32Size \b IN, OUT :
///                             IN: the input array size.
///                             OUT: the used array size.
/// @return -The result of command generate spcific pattern
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GenPattern(MS_U32 u32Id, HVD_EX_PatternType eType, MS_U32 u32VAddr, MS_U32 * pu32Size)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 *pDummyData = NULL;
    MS_U32 u32MinPatternSize = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (eType == E_HVD_EX_PATTERN_FLUSH)   // flush pattern
    {
        // Driver input need not to push flush pattern
        if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
        {
            *pu32Size = 0;
            eRet = E_HVD_EX_OK;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        // TSP input process
        if (u32VAddr == 0)
        {
            *pu32Size = 8 + 144;
            HVD_EX_MSG_ERR("Flush Pattern address shall not be zero\n");
            eRet = E_HVD_EX_RET_INVALID_PARAMETER;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        switch ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
        {
            case HVD_INIT_HW_AVC:
            case HVD_INIT_HW_MVC:
            case HVD_INIT_HW_VP8:
            case HVD_INIT_HW_AVS:
        #if SUPPORT_EVD
            case HVD_INIT_HW_VP9:
            case HVD_INIT_HW_HEVC:
        #endif
            {
                u32MinPatternSize = 8 + 144;
                break;
            }
            case HVD_INIT_HW_RM:
            default:
                u32MinPatternSize = 0;
                break;
        }

        if (*pu32Size < u32MinPatternSize)
        {
            HVD_EX_MSG_ERR("Flush Pattern must have at least %lu bytes, input:%lu\n", u32MinPatternSize,
                        (MS_U32) (*pu32Size));
            *pu32Size = u32MinPatternSize;
            eRet = E_HVD_EX_RET_OUTOF_MEMORY;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        *pu32Size = u32MinPatternSize;
        pDummyData = (MS_U8 *) u32VAddr;

        memset((void *) pDummyData, 0, *pu32Size);

        switch ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
        {
            case HVD_INIT_HW_AVC:
            case HVD_INIT_HW_MVC:
            case HVD_INIT_HW_VP8:
        #if SUPPORT_EVD
            case HVD_INIT_HW_VP9:
            case HVD_INIT_HW_HEVC:
        #endif
            {
                pDummyData[0] = 0;
                pDummyData[1] = 0;
                pDummyData[2] = 1;
                pDummyData[3] = 0xFF;
                pDummyData[4] = 0xAA;
                pDummyData[5] = 0x55;
                pDummyData[6] = 0xAA;
                pDummyData[7] = 0x55;

                break;
            }
            case HVD_INIT_HW_AVS:
            {
                pDummyData[0] = 0;
                pDummyData[1] = 0;
                pDummyData[2] = 1;
                pDummyData[3] = 0xB4;
                pDummyData[4] = 0xAA;
                pDummyData[5] = 0x55;
                pDummyData[6] = 0xAA;
                pDummyData[7] = 0x55;

                break;
            }
            case HVD_INIT_HW_RM:
            default:
                break;
        }
    }
    else if (eType == E_HVD_EX_PATTERN_FILEEND)    // dummy pattern
    {
        // Driver input need not to push dummy pattern
        if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
        {
            *pu32Size = 0;
            eRet = E_HVD_EX_OK;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        // TSP input process
        if (u32VAddr == 0)
        {
            *pu32Size = 8 + 144;
            HVD_EX_MSG_ERR("Dummy Pattern address shall not be zero\n");
            eRet = E_HVD_EX_RET_INVALID_PARAMETER;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        switch ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
        {
            case HVD_INIT_HW_AVC:
            case HVD_INIT_HW_MVC:
            case HVD_INIT_HW_VP8:
            case HVD_INIT_HW_AVS:
        #if SUPPORT_EVD
            case HVD_INIT_HW_VP9:
            case HVD_INIT_HW_HEVC:
        #endif
                u32MinPatternSize = 8 + 144;
                break;
            case HVD_INIT_HW_RM:
            default:
                u32MinPatternSize = 0;
                break;
        }

        if (*pu32Size < u32MinPatternSize)
        {
            HVD_EX_MSG_ERR("Dummy Pattern must have at least %lu bytes, input:%lu\n", u32MinPatternSize,
                        (MS_U32) (*pu32Size));
            *pu32Size = u32MinPatternSize;
            eRet = E_HVD_EX_RET_OUTOF_MEMORY;
            _DRV_HVD_EX_RET(u8DrvId, eRet);
        }

        pDummyData = (MS_U8 *) u32VAddr;

        memset((void *) pDummyData, 0, *pu32Size);

        switch ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_HW_MASK))
        {
            case HVD_INIT_HW_AVC:
            case HVD_INIT_HW_MVC:
            case HVD_INIT_HW_VP8:
        #if SUPPORT_EVD
            case HVD_INIT_HW_VP9:
            case HVD_INIT_HW_HEVC:
        #endif
            {
                pDummyData[0] = 0;
                pDummyData[1] = 0;
                pDummyData[2] = 1;
                pDummyData[3] = 0xFF;
                pDummyData[4] = 0xFF;
                pDummyData[5] = 0xFF;
                pDummyData[6] = 0xFF;
                pDummyData[7] = 0xFF;

                break;
            }
            case HVD_INIT_HW_AVS:
            {
                pDummyData[0] = 0;
                pDummyData[1] = 0;
                pDummyData[2] = 1;
                pDummyData[3] = 0xB4;
                pDummyData[4] = 0xAA;
                pDummyData[5] = 0x66;
                pDummyData[6] = 0xAA;
                pDummyData[7] = 0x66;

                break;
            }
            case HVD_INIT_HW_RM:
            default:
                *pu32Size = u32MinPatternSize;
                break;
        }
    }

    eRet = E_HVD_EX_OK;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetPatternInfo()
/// @brief \b Function \b Description:  Get driver specific data information
/// @param -eType \b IN : the type of specific data information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetPatternInfo(MS_U32 u32Id, HVD_EX_PatternInfo eType)
{
    MS_U32 eRet = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    switch (eType)
    {
        case E_HVD_EX_FLUSH_PATTERN_SIZE:
        {
            if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
            {
                eRet = 0;
            }
            else if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_TS)
            {
                eRet = 8 + 144;
            }
            break;
        }
        case E_HVD_EX_DUMMY_HW_FIFO:
        {
            if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_RAW)
            {
                eRet = 0;
            }
            else if ((pHVDDrvContext->gHVDCtrl_EX[u8DrvId].InitParams.u32ModeFlag & HVD_INIT_MAIN_MASK) == HVD_INIT_MAIN_FILE_TS)
            {
                eRet = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_MAX_DUMMY_FIFO);
            }
            break;
        }
        default:
            break;
    }

    return eRet;
}


MS_U8 MDrv_HVD_EX_GetDSBufMiuSelect(MS_U32 u32Id)
{
    MS_U8 u8MiuSel;
    MS_U32 u32StartOffset;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);


    if(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8SettingMode & HVD_DRV_MODE_EXTERNAL_DS_BUFFER)
    {
        _phy_to_miu_offset(u8MiuSel, u32StartOffset, pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32ExternalDSbuf);
    }
    else
    {
        _phy_to_miu_offset(u8MiuSel, u32StartOffset, pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr);
    }
    return u8MiuSel;
}



//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDynamicScalingInfo()
/// @brief \b Function \b Description:  Get information of Dynamic Scaling
/// @param -eType \b IN : the type of specific information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDynamicScalingInfo(MS_U32 u32Id, HVD_EX_DynamicScalingInfo eType)
{
    MS_U32 u32Ret = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U8 u8MiuSel;
    MS_U32 u32StartOffset;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,FALSE);

    switch (eType)
    {
        case E_HVD_EX_DS_BUF_MIUSEL:
            if(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8SettingMode & HVD_DRV_MODE_EXTERNAL_DS_BUFFER)
            {
                if(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32ExternalDSbuf >= pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr)
                {
                    u32Ret = TRUE;
                }
                else
                {
                    u32Ret = FALSE;
                }

                _phy_to_miu_offset(u8MiuSel, u32StartOffset, pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32ExternalDSbuf);

                if(u8MiuSel >= E_CHIP_MIU_2)
                {

                    HVD_EX_MSG_ERR("u8MiuSel is %d !!, but return type is only Boolean. Please use MDrv_HVD_EX_GetDSBufMiuSelect() to get right miu select of DS buffer!\n",u8MiuSel);
                }
            }
            else
            {
                if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr >= pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32MIU1BaseAddr)
                {
                    u32Ret = TRUE;
                }
                else
                {
                    u32Ret = FALSE;
                }

                _phy_to_miu_offset(u8MiuSel, u32StartOffset, pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr);

                if(u8MiuSel >= E_CHIP_MIU_2)
                {

                    HVD_EX_MSG_ERR("u8MiuSel is %d !!, but return type is only Boolean. Please use MDrv_HVD_EX_GetDSBufMiuSelect() to get right miu select of DS buffer!\n",u8MiuSel);
                }
            }
            break;
        case E_HVD_EX_DS_BUF_ADDR:
            if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8SettingMode & HVD_DRV_MODE_EXTERNAL_DS_BUFFER)
            {
                u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32ExternalDSbuf;
                ///printf("[EDS] E_HVD_EX_DS_BUF_ADDR 0x%lx.\n",u32Ret);
            }
            else
            {
                u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr + HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_BUF_ADDR);

                ///printf("[DS] DS_BUF_ADDR 0x%lx \n", u32Ret);
                ///printf("[EDS] E_HVD_EX_DS_BUF_ADDR Old.\n");
            }
            break;
        case E_HVD_EX_DS_BUF_SIZE:
            u32Ret = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_BUF_SIZE);
            break;
        case E_HVD_EX_DS_VECTOR_DEPTH:
            u32Ret = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_VECTOR_DEPTH);
            break;
        case E_HVD_EX_DS_INFO_ADDR:
            if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8SettingMode & HVD_DRV_MODE_EXTERNAL_DS_BUFFER)
            {
                u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32ExternalDSbuf + HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_BUF_SIZE);//0xC00;
                ///printf("[EDS] E_HVD_EX_DS_INFO_ADDR 0x%lx.\n",u32Ret);
            }
            else
            {
                u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr + HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_INFO_ADDR);

                ///printf("[DS] DS_INFO_ADDR 0x%lx \n", u32Ret);
            }
            break;
        case E_HVD_EX_DS_IS_ENABLED:
            u32Ret = (MS_U32) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_DS_IS_ENABLED);
            break;
       default:
            break;
    }
    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetData()
/// @brief \b Function \b Description:  Get target data from HVD driver
/// @param -eType \b IN : the type of the target data
/// @return -the value of the target data
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetData(MS_U32 u32Id, HVD_EX_GDataType eType)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32Ret = 0;

    _DRV_HVD_Inited(u8DrvId,FALSE);

    switch (eType)
    {
        case E_HVD_EX_GDATA_TYPE_DISP_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_SKIP_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_SKIP_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_DROP_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DROP_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_IDLE_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_VSYNC_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VSYNC_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_MAIN_LOOP_CNT:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_MAIN_LOOP_CNT);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_AVC_LEVEL_IDC:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_AVC_LEVEL_IDC);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_DISP_Q_SIZE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_Q_NUMB);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_ES_LEVEL:
        {
            u32Ret = (MS_U32) (HVD_EX_ESLevel) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_ES_LEVEL);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_AVC_VUI_DISP_INFO:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_AVC_VUI_DISP_INFO);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_DISP_STC:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_STC);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_USERDATA_IDX_TBL_SIZE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_USERDATA_PACKET_SIZE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_PACKET_SIZE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_REAL_FRAMERATE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_REAL_FRAMERATE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_IS_ORI_INTERLACE_MODE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_IS_ORI_INTERLACE_MODE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_FRAME_MBS_ONLY_FLAG:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_TYPE_FRAME_MBS_ONLY_FLAG);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_IS_LEAST_DISPQ_SIZE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_TYPE_IS_LEAST_DISPQ_SIZE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_FIELD_PIC_FLAG:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FIELD_PIC_FLAG);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_FW_CODEC_TYPE:
        {
            //u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_CODEC_TYPE);
            switch(HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_CODEC_TYPE))
            {
                case E_HVD_Codec_AVC:
                {
                    u32Ret = E_HVD_EX_AVC;
                    break;
                }
                case E_HVD_Codec_AVS:
                {
                    u32Ret = E_HVD_EX_AVS;
                    break;
                }
                case E_HVD_Codec_RM:
                {
                    u32Ret = E_HVD_EX_RM;
                    break;
                }
                case E_HVD_Codec_MVC:
                {
                    u32Ret = E_HVD_EX_MVC;
                    break;
                }
                case E_HVD_Codec_VP8:
                {
                    u32Ret = E_HVD_EX_VP8;
                    break;
                }
                case E_HVD_Codec_MJPEG:
                {
                    u32Ret = E_HVD_EX_NONE;
                    break;
                }
                case E_HVD_Codec_HEVC:
                {
                    u32Ret = E_HVD_EX_HEVC;
                    break;
                }
                case E_HVD_EX_VP9:
                {
                    u32Ret = E_HVD_EX_VP9;
                    break;
                }
                default:
                {
                    u32Ret = E_HVD_EX_NONE;
                    break;
                }
            }
            break;
        }
        case E_HVD_EX_GDATA_TYPE_FRC_MODE:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FRC_MODE);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_VSYNC_BRIDGE_ADDR:
        {
            u32Ret = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr + HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_VSYNC_BRIDGE_ADDR);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_FW_STATUS_FLAG:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATUS_FLAG);
            break;
        }
        case E_HVD_EX_GDATA_TYPE_HVD_HW_MAX_PIXEL:
        {
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_HVD_HW_MAX_PIXEL);
            break;
        }
        default:
            break;
    }

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetMem_Dbg()
/// @brief \b Function \b Description:  Get any data from any memory address
/// @param -u32Addr \b IN : the memory address of the target data
/// @return -the value of the memory
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_U32 u32Ret = 0;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,u32Ret);

    switch (u32Addr)
    {
        case 1:
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_READ_PTR);
            break;
        case 2:
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_PC_CNT);
            break;
        case 3:
            u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_WRITE_PTR);
            break;
        default:
            break;
    }

    return u32Ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DbgDumpStatus()
/// @brief \b Function \b Description:  Dump specific information to standard output.
/// @param -eFlag \b IN : the type of information.
//-----------------------------------------------------------------------------
void MDrv_HVD_EX_DbgDumpStatus(MS_U32 u32Id, HVD_EX_DumpStatus eFlag)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    if (!(pHVDDrvContext->bHVDIsInited[u8DrvId]))
    {
        return;
    }

    if (eFlag & E_HVD_EX_DUMP_STATUS_FW)
    {
        HAL_HVD_EX_Dump_FW_Status(u32Id);
    }

    if (eFlag & E_HVD_EX_DUMP_STATUS_HW)
    {
        HAL_HVD_EX_Dump_HW_Status(HVD_U32_MAX);
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetMem_Dbg()
/// @brief \b Function \b Description:  set any data into any memory address
/// @param -u32Addr \b IN : the memory address of the target destination
/// @param -u32Arg \b IN : the value of input content
//-----------------------------------------------------------------------------
void MDrv_HVD_EX_SetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Arg)
{
    HVD_EX_MSG_TRACE();
    HAL_HVD_EX_SetData_Dbg(u32Addr, u32Arg);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_SetData_Dbg()
/// @brief \b Function \b Description:  set any FW debug command
/// @param -u32Cmd \b IN : specify the FW command ID.
/// @param -u32Arg \b IN : specify the argument of FW command.
/// @return -the result of debug command
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetCmd_Dbg(MS_U32 u32Id, MS_U32 u32Cmd, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    // todo: consider more...
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, (HVD_User_Cmd) u32Cmd, u32Arg);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Set_Err_Tolerance()
/// @brief \b Function \b Description:  set display error tolerance
/// @param -u32Arg \b IN : //[15:8]+[7:0] = (err_tolerance(0%~100%)+enable or disable)
/// @return -the result of set err tolerance
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Set_Err_Tolerance(MS_U32 u32Id, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_DISP_ERROR_TOLERANCE, u32Arg);

    _DRV_HVD_EX_RET(u8DrvId, eRet);

}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetSettings_Pro()
/// @brief \b Function \b Description:  set any FW debug command
/// @param -eType \b IN : specify the type of setting.
/// @param -u32Arg \b IN : specify the argument of the setting.
/// @return -the result of set professional settings.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetSettings_Pro(MS_U32 u32Id, HVD_EX_SSettingsType eType, MS_U32 u32Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    HVD_User_Cmd eCmd = E_HVD_CMD_INVALID_CMD;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    switch (eType)
    {
        case E_HVD_EX_SSET_TIME_UNIT:
            eCmd = E_HVD_CMD_TIME_UNIT_TYPE;
            break;
        case E_HVD_EX_SSET_PITCH:
            eCmd = E_HVD_CMD_PITCH;
            break;
        case E_HVD_EX_SSET_SYNC_EACH_FRM:
            eCmd = E_HVD_CMD_SYNC_EACH_FRM;
            break;
        case E_HVD_EX_SSET_MAX_DEC_TICK:
            eCmd = E_HVD_CMD_MAX_DEC_TICK;
            break;
        case E_HVD_EX_SSET_AUTO_FREE_ES:
            eCmd = E_HVD_CMD_AUTO_FREE_ES;
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bAutoFreeES = u32Arg;
            break;
        case E_HVD_EX_SSET_MIN_FRAME_GAP:
            eCmd = E_HVD_CMD_MIN_FRAME_GAP;
            break;
        case E_HVD_EX_SSET_DISABLE_DEBLOCKING:
            eCmd = E_HVD_CMD_DIS_DBF;
            if (u32Arg > 2)
            {
                u32Arg = 1;
            }
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bDisDeblocking = u32Arg;
            break;
        case E_HVD_EX_SSET_DISABLE_QUARTER_PIXEL:
            eCmd = E_HVD_CMD_DIS_QUART_PIXEL;
            if (u32Arg > 2)
            {
                u32Arg = 1;
            }
            pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.bDisQuarterPixel = u32Arg;
            break;
        case E_HVD_EX_SSET_MIU_BURST_CNT_LEVEL:
            {
                pHVDDrvContext->gHVDCtrl_EX[u8DrvId].Settings.u32MiuBurstLevel = u32Arg;
                eCmd = E_HVD_CMD_MIU_BURST_CNT;

                break;
            }
        default:
            break;
    }

    if (eCmd != E_HVD_CMD_INVALID_CMD)
    {
        eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, eCmd, u32Arg);
    }
    else
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetCaps()
/// @brief \b Function \b Description:  check if HW support this format
/// @param -u32Type \b IN : specify the format type
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): not supported by HW
/// @retval     -TRUE(1): supported by HW
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetCaps(HVD_EX_Codec u32Type)
{
    if(E_HVD_EX_HEVC == u32Type)
#if SUPPORT_EVD
        return TRUE;
#else
        return FALSE;
#endif
#if SUPPORT_G2VP9
    if(E_HVD_EX_VP9 == u32Type)
        return TRUE;
#endif

#if ( HVD_HW_VERSION == HVD_HW_HVD)
    MS_U32 verID = HAL_HVD_EX_GetHWVersionID();
    verID = verID >> 12;
    switch (u32Type)
    {
        case E_HVD_EX_AVC:
        case E_HVD_EX_AVS:
        case E_HVD_EX_RM:
        case E_HVD_EX_MVC:
            if ((verID & BIT(u32Type)) == 0)
            {
                return FALSE;
            }
            break;
        case E_HVD_EX_VP8:
        #if !SUPPORT_G2VP9 && SUPPORT_EVD && defined(VDEC3)
        case E_HVD_EX_VP9: // Mstar VP9
        #endif
            return TRUE;
        default:
            return FALSE;
    }
#else
    if (u32Type != E_HVD_EX_AVC)
    {
        return FALSE;
    }
#endif
    return TRUE;
}

MS_BOOL MDrv_HVD_LinkWeakSymbolPatch(void)
{
    return TRUE;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetAutoRmLstZeroByte
/// @brief \b Function \b Description: Turn on/off auto remove last zero byte
/// @param -bOn \b IN : Turn on/off auto remove last zero byte
/// @return -the result of turn on/off auto remove last zero byte
///\b NOTE: The default mode after initialization is On.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetAutoRmLstZeroByte(MS_U32 u32Id, MS_BOOL bOn)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].bAutoRmLastZeroByte = bOn;

    return E_HVD_EX_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IsAlive
/// @brief \b Function \b Description:Check Whether HVD is alive or not(check cnt)
/// @return -the result of HVD alive status(E_HVD_EX_OK/E_HVD_EX_RET_NOT_RUNNING)
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_IsAlive(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,E_HVD_EX_FAIL);

    if (HAL_HVD_EX_IsAlive(u32Id))
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].LivingStatus.u32DecCnt = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT);
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].LivingStatus.u32SkipCnt = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_SKIP_CNT);
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].LivingStatus.u32IdleCnt = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT);
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].LivingStatus.u32MainLoopCnt = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_MAIN_LOOP_CNT);

        return E_HVD_EX_OK;
    }
    else
    {
        return E_HVD_EX_RET_NOT_RUNNING;
    }
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetBalanceBW
/// @brief \b Function \b Description: bandwidth adjustment
/// @param -qp_cnt \b IN : QP threshold for overtime counter
/// @param -db_cnt \b IN : Deblocking threshod for overtime counter
/// @param -upper \b IN : upper bound for overtime counter
/// @return -the result of command E_HVD_CMD_BALANCE_BW
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetBalanceBW(MS_U32 u32Id, MS_U8 u8QPCnt, MS_U8 u8DBCnt, MS_U8 u8Upper)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_BALANCE_BW, u8QPCnt | (u8DBCnt << 8) | (u8Upper << 16));

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetFdMaskDelayCnt()
/// @brief \b Function \b Description: Set fd mask muting count
/// @param -u8DelayCnt \b IN : 0~0xFF, Fdmask delay count, arg >= 0xFF -> use default
/// @return -The result of command setting fd_mask muting count
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetFdMaskDelayCnt(MS_U32 u32Id, MS_U8 u8DelayCnt)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FDMASK_DELAY_CNT, u8DelayCnt);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetOutputFRCMode()
/// @brief \b Function \b Description: Set output frame rate convert mode.
/// @param -u8FrameRate \b IN : output vsync count.
/// @param -u8Interlace \b IN : output scan type: 0:progress, 1:interlace.
/// @return -The result of command setting output FRC mode
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetOutputFRCMode(MS_U32 u32Id, MS_U8 u8FrameRate, MS_U8 u8Interlace)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if ((u8Interlace != 0) && (u8Interlace != 1))
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FRC_OUTPUT_FRAMERATE, u8FrameRate);

    if (eRet != E_HVD_EX_OK)
    {
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FRC_OUTPUT_INTERLACE, u8Interlace);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

HVD_EX_Result MDrv_HVD_EX_DispFrame(MS_U32 u32Id, MS_U32 u32FrmIdx)
{
    HVD_EX_MSG_TRACE();

    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DISPQ_STATUS_DISP, u32FrmIdx);

    return E_HVD_EX_OK;
}

HVD_EX_Result MDrv_HVD_EX_FreeFrame(MS_U32 u32Id, MS_U32 u32FrmIdx)
{
    HVD_EX_MSG_TRACE();

    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DISPQ_STATUS_FREE, u32FrmIdx);

    return E_HVD_EX_OK;
}

HVD_EX_Result MDrv_HVD_EX_EnableDispQue(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_MSG_TRACE();

    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ENABLE_DISP_QUEUE, bEnable);

    return E_HVD_EX_OK;
}

HVD_EX_Result MDrv_HVD_EX_EnableVSizeAlign(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_MSG_TRACE();

    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_ALIGN_VSIZE, bEnable);

    return E_HVD_EX_OK;
}

HVD_EX_Result MDrv_HVD_EX_ShowDecodeOrder(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SHOW_DECODE_ORDER, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Disp_Ignore_Crop()
/// @brief \b Function \b Description: ingore corp info
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command setting fd_mask muting count
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Disp_Ignore_Crop(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DISP_IGNORE_CROP, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

HVD_EX_Result MDrv_HVD_EX_SuspendDynamicScale(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SUSPEND_DYNAMIC_SCALE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

HVD_EX_Result MDrv_HVD_EX_RmEnablePtsTbl(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RM_ENABLE_PTS_TBL, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


HVD_EX_Result MDrv_HVD_EX_AutoExhaustESMode(MS_U32 u32Id, MS_U32 u32ESbound)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AUTO_EXHAUST_ES_MODE, u32ESbound);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


HVD_EX_Result MDrv_HVD_EX_SetMinTspSize(MS_U32 u32Id, MS_U32 u32Size)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_MIN_TSP_DATA_SIZE, u32Size);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// Set HVD FRC drop type.
/// @param u8DropType \b IN : drop type. 0:drop frame, 1:drop field. default:0
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetFRCDropType(MS_U32 u32Id, MS_U8 u8DropType)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if (u8DropType != 0 && u8DropType != 1)
    {
        eRet = E_HVD_EX_RET_INVALID_PARAMETER;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FRC_DROP_MODE, u8DropType);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDrvFwVer()
/// @brief \b Function \b Description: Get driver's FW version
/// @return - Driver's FW version
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDrvFwVer(void)
{
    return HVD_FW_VERSION;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetFwVer()
/// @brief \b Function \b Description: Get driver's FW version
/// @return - FW version obtained by querying FW
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetFwVer(MS_U32 u32Id)
{
    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_VERSION_ID);
}

MS_BOOL MDrv_HVD_SetSingleDecodeMode(MS_BOOL bEnable)
{
    return HAL_VPU_EX_SetSingleDecodeMode(bEnable);
}

MS_BOOL MDrv_HVD_SetDecodeMode(HVD_EX_DecModCfg *pstCfg)
{
    return HAL_VPU_EX_SetDecodeMode((VPU_EX_DecModCfg*)pstCfg);
}

void MDrv_HVD_EX_SetBurstMode(MS_U32 u32Id, MS_BOOL bBurst)
{
    HVD_Return eCtrlRet = E_HVD_RETURN_FAIL;

    eCtrlRet = HAL_HVD_EX_SetCmd(u32Id, E_DUAL_BURST_MODE, bBurst);
    if (E_HVD_RETURN_SUCCESS != eCtrlRet)
    {
        HVD_EX_MSG_ERR("E_DUAL_BURST_MODE NG eCtrlRet=%x\n", eCtrlRet);
    }
    else
    {
        HVD_EX_MSG_DBG("MJPEG!!! Set burst mode =%d success!!!!\n", bBurst);
    }
}

//------------------------------------------------------------------------------
/// Force into interlace mode
/// @param u8Mode \b IN : 0: Disable, 1: Enable
/// @return -E_HVD_OK for success; E_HVD_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_ForceInterlaceMode(MS_U32 u32Id, MS_U8 u8Mode)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FORCE_INTERLACE, u8Mode);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// Push Disp Q with Ref num
/// @param u8Mode \b IN : 0: Disable, 1: Enable
/// @return -E_HVD_OK for success; E_HVD_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_PushDispQWithRefNum(MS_U32 u32Id, MS_U8 u8Mode)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PUSH_DISPQ_WITH_REF_NUM, u8Mode);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// Ignore HW error - PIC_OVERRUN
/// @param bEnable \b IN : 0: Disable, 1: Enable
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_IgnorePicOverrun(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_PIC_OVERRUN, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// Control speed in displaying time only.
/// @param bEnable \b IN : 0: Original, control in decoding and displaying time, 1: Enable,control speed in displaying time only
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_CtlSpeedInDispOnly(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_CTRL_SPEED_IN_DISP_ONLY, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// AVC support reference number over max DPB size when frame buffer is enough.
/// @param bEnable \b IN : 0: Disable, 1: Enable
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_AVCSupportRefNumOverMaxDBPSize(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

HVD_EX_Result MDrv_HVD_EX_ReturnInvalidAFD(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RETURN_INVALID_AFD, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


HVD_EX_Result MDrv_HVD_EX_AVCForceBrokenByUs(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AVC_FORCE_BROKEN_BY_US, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Support_AVC2MVC()
/// @brief \b Function \b Description: Set Support AVC to MVC
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command setting fd_mask muting count
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Support_AVC2MVC(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SUPPORT_AVC_TO_MVC, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_3DLR_View_Exchange()
/// @brief \b Function \b Description: view exchange
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command setting fd_mask muting count
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_3DLR_View_Exchange(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_3DLR_VIEW_EXCHANGE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetFrmRateIsSupported()
/// @brief \b Function \b Description:  Get if the framerate is supported
/// @return -The result of supported or not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_BOOL bRet = pCtrl->bFrmRateSupported;

    return bRet;
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_Enable_New_Slow_Motion()
/// @brief \b Function \b Description: view exchange
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command setting fd_mask muting count
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_Enable_New_Slow_Motion(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ENABLE_NEW_SLOW_MOTION, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//------------------------------------------------------------------------------
/// Get SEI USER DATA Info
/// @param pUsrInfo \b OUT : Get CC USER Data info
/// @return -E_HVD_OK for success; E_HVD_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GetUserDataInfo(MS_U32 u32Id,HVD_EX_UserData_Info* pUsrInfo)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U32 u32UserDataIdxSize = 0;
    MS_U32 u32UsrDataIdxWptr = 0xFFFFFFFF;
    MS_U32 u32UserDataSize = 0;
    MS_U32 u32UserDataAddr = 0;
    MS_U32 u32CodeVAddr = 0;
    DTV_BUF_type* pHVD_User_Data = NULL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    u32UserDataIdxSize = (MS_U32)HAL_HVD_EX_GetData(u32Id,E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);
    u32UsrDataIdxWptr = MDrv_HVD_EX_GetUserData_Wptr(u32Id);

    if (u32UsrDataIdxWptr != 0xFFFFFFFF)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataWr = u32UsrDataIdxWptr;
    }

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd == pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataWr)
    {   // User Data Buffer Empty
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    u32CodeVAddr = pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufVAddr;
    if(u32CodeVAddr == 0x0)
    {
        eRet = E_HVD_EX_FAIL;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    u32UserDataAddr = MDrv_HVD_EX_GetUserData_Packet(u32Id,pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd, (MS_U32*)&u32UserDataSize );
    if(u32UserDataAddr == 0x0)
    {
        eRet = E_HVD_EX_FAIL;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }
    u32UserDataAddr += u32CodeVAddr; // change to virtual address

    pHVD_User_Data = (DTV_BUF_type*)u32UserDataAddr;

    pUsrInfo->u16TmpRef = pHVD_User_Data->u16TempRefCnt;
    pUsrInfo->u8PicStruct = pHVD_User_Data->pic_struct;
    pUsrInfo->u8PicType = pHVD_User_Data->type;
    pUsrInfo->u32Pts = pHVD_User_Data->pts;
    pUsrInfo->u8ByteCnt = pHVD_User_Data->len;
    pUsrInfo->u32DataBuf = (MS_U32)pHVD_User_Data->buf;

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd++;

    if (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd >= u32UserDataIdxSize)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd = 0;
    }

    eRet = E_HVD_EX_OK;

    _DRV_HVD_EX_RET(u8DrvId, eRet);

}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetUsrDataIsAvailable()
/// @brief \b Function \b Description: Is there new user data info.
/// @return - TRUE/ FALSE
/// @retval     -FALSE(0): no new user data info
/// @retval     -TRUE(1): has new user data info
//------------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetUsrDataIsAvailable(MS_U32 u32Id)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    MS_BOOL bRet = FALSE;
    HVD_EX_MSG_TRACE();
    _DRV_HVD_EX_Entry(u8DrvId);
    MS_U32 u32UsrDataIdxWptr = 0xFFFFFFFF;
    u32UsrDataIdxWptr = MDrv_HVD_EX_GetUserData_Wptr(u32Id);

    if (u32UsrDataIdxWptr != 0xFFFFFFFF)
    {
        pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataWr = u32UsrDataIdxWptr;
    }

    bRet = (pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataWr == pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u32UsrDataRd)?FALSE:TRUE;
    _DRV_HVD_EX_RET(u8DrvId, bRet);
}

//------------------------------------------------------------------------------
/// Set HVD DTV User Data Packet Mode
/// @param u8UserDataMode \b IN : User Data Packet Mode. 0: DVB normal, 1: ATSC DirectTV. default:0
/// @return -E_HVD_OK for success; E_HVD_FAIL for failure.
//------------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDTVUserDataMode(MS_U32 u32Id,MS_U8 u8UserDataMode)
{
    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);


    if (u8UserDataMode == 0)
    {
        // Noraml DVB USER DATA
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, 0);
    }
    else if (u8UserDataMode == 1)
    {
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, 1);
    }
    else if (u8UserDataMode == 2)
    {
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, 2);
    }
    else if (u8UserDataMode == 3)
    {
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, 3);
    }
    else if (u8UserDataMode == 7)
    {
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, 7);
    }
    else
    {
        eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id,E_HVD_CMD_SET_USERDATA_MODE, u8UserDataMode);
        HVD_PRINT("Debug Mode for set user data mode \n");
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_GetFrmPackingArrSEI()
/// @brief \b Function \b Description:  Get if the Frame packing arrangement SEI data
/// @param -bEnable \b IN : The frame packing SEI struct
/// @return -The result of got the info. or not.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_GetFrmPackingArrSEI(MS_U32 u32Id, HVD_EX_FrmPackingSEI *pFrmPacking)
{

    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U32 u32FPAFrameAddr = 0 ;
    MS_U32 u32CodeVaddr = 0 ;
    HVD_Frame_packing_SEI *pFrmPackingSrc = NULL;
    MS_BOOL bIn = FALSE;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    u32CodeVaddr = MS_PA2KSEG1((MS_U32)(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr));
    if(u32CodeVaddr == 0x0)
    {
        eRet = E_HVD_EX_FAIL;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    u32FPAFrameAddr = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FRM_PACKING_SEI_DATA);

    //u32FPAFrameAddr = HAL_HVD_EX_GetData(u8DrvId,E_HVD_GDATA_FRM_PACKING_SEI_DATA);
    if(u32FPAFrameAddr == 0x0)
    {
        eRet = E_HVD_EX_FAIL;
        _DRV_HVD_EX_RET(u8DrvId, eRet);
    }

    u32FPAFrameAddr = MS_PA2KSEG1((MS_U32)(pHVDDrvContext->gHVDCtrl_EX[u8DrvId].MemMap.u32CodeBufAddr+u32FPAFrameAddr)); // change to virtual address

    pFrmPackingSrc = (HVD_Frame_packing_SEI *)u32FPAFrameAddr;

    if(pFrmPacking != NULL)
    {
        if(pFrmPackingSrc->bvaild == TRUE)
        {
            bIn = TRUE;
            pFrmPacking->bvaild                         = pFrmPackingSrc->bvaild;
            pFrmPacking->bUsed                          = pFrmPackingSrc->bUsed;
            pFrmPacking->u8Frm_packing_arr_cnl_flag     = pFrmPackingSrc->u8Frm_packing_arr_cnl_flag;
            pFrmPacking->u8Frm_packing_arr_type         = pFrmPackingSrc->u8Frm_packing_arr_type;
            pFrmPacking->u8content_interpretation_type  = pFrmPackingSrc->u8content_interpretation_type;
            pFrmPacking->u1Quincunx_sampling_flag       = pFrmPackingSrc->u1Quincunx_sampling_flag;
            pFrmPacking->u1Spatial_flipping_flag        = pFrmPackingSrc->u1Spatial_flipping_flag;
            pFrmPacking->u1Frame0_flipping_flag         = pFrmPackingSrc->u1Frame0_flipping_flag;
            pFrmPacking->u1Field_views_flag             = pFrmPackingSrc->u1Field_views_flag;
            pFrmPacking->u1Current_frame_is_frame0_flag = pFrmPackingSrc->u1Current_frame_is_frame0_flag;
            pFrmPacking->u1Frame0_self_contained_flag   = pFrmPackingSrc->u1Frame0_self_contained_flag;
            pFrmPacking->u1Frame1_self_contained_flag   = pFrmPackingSrc->u1Frame1_self_contained_flag;
            pFrmPacking->u4Frame0_grid_position_x       = pFrmPackingSrc->u4Frame0_grid_position_x;
            pFrmPacking->u4Frame0_grid_position_y       = pFrmPackingSrc->u4Frame0_grid_position_y;
            pFrmPacking->u4Frame1_grid_position_x       = pFrmPackingSrc->u4Frame1_grid_position_x;
            pFrmPacking->u4Frame1_grid_position_y       = pFrmPackingSrc->u4Frame1_grid_position_y;
            pFrmPacking->u16CropRight                   = pFrmPackingSrc->u16CropRight;
            pFrmPacking->u16CropLeft                    = pFrmPackingSrc->u16CropLeft;
            pFrmPacking->u16CropBottom                  = pFrmPackingSrc->u16CropBottom;
            pFrmPacking->u16CropTop                     = pFrmPackingSrc->u16CropTop;

            if(pFrmPackingSrc->u8payload_len > 32)
            {
                pFrmPacking->u8payload_len = 32;
            }
            else
            {
                pFrmPacking->u8payload_len = pFrmPackingSrc->u8payload_len;
            }

            pFrmPacking->u8WaitSPS  = pFrmPackingSrc->u8WaitSPS;
            pFrmPacking->u32payload = (MS_U32)pFrmPackingSrc->u8payload;

            eRet = E_HVD_EX_OK;
        }
        else
        {
            eRet = E_HVD_EX_FAIL;
        }
        HVD_EX_MSG_DBG("SEI status (%d): %d %d %d %d %d %d %d %d\n",bIn,
                pFrmPacking->bvaild,pFrmPacking->bUsed,pFrmPacking->u8Frm_packing_arr_cnl_flag,pFrmPacking->u8Frm_packing_arr_type,
                pFrmPacking->u16CropRight,pFrmPacking->u16CropLeft,pFrmPacking->u16CropBottom,pFrmPacking->u16CropTop);
    }
    else
    {
        eRet = E_HVD_EX_FAIL;
    }
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DynamicScalingResvNBuffer()
/// @brief \b Function \b Description: use N buffers for Dynamic Scaling
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable N buffer for DS
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_DynamicScalingResvNBuffer(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    //HVD_PRINT("MDrv_HVD_EX_DynamicScalingResvNBuffer bEnable = %d\n", bEnable);
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DYNAMIC_SCALE_RESV_N_BUFFER, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetBBUQNum()
/// @brief \b Function \b Description:  Get BBU Q number
/// @return - BBU Q number
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetBBUQNum(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_BBU_Q_NUMB);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetDispFrmNum()
/// @brief \b Function \b Description:  Get Disp frame number
/// @return - Disp frame number
//-----------------------------------------------------------------------------
MS_U32 MDrv_HVD_EX_GetDispFrmNum(MS_U32 u32Id)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    _DRV_HVD_Inited(u8DrvId,FALSE);

    return HAL_HVD_EX_GetDispFrmNum(u32Id);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetHVDClock()
/// @brief \b Function \b Description:  set hvd clock speed
/// @return - The result of setting hvd clock speed
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetHVDClockSpeed(HVD_EX_ClockSpeed eClockSpeed)
{
#if defined(CHIP_A3)
    HAL_HVD_EX_ClockSpeed eHVDClockSpeed = E_HVD_EX_CLOCK_SPEED_NONE;
    switch (eClockSpeed)
    {
        case E_HVD_EX_CLOCK_SPEED_HIGHEST:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_HIGHEST;
            break;
        case E_HVD_EX_CLOCK_SPEED_HIGH:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_HIGH;
            break;
        case E_HVD_EX_CLOCK_SPEED_MEDIUM:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_MEDIUM;
            break;
        case E_HVD_EX_CLOCK_SPEED_LOW:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_LOW;
            break;
        case E_HVD_EX_CLOCK_SPEED_LOWEST:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_LOWEST;
            break;
        case E_HVD_EX_CLOCK_SPEED_DEFAULT:
            eHVDClockSpeed = E_HAL_HVD_EX_CLOCK_SPEED_DEFAULT;
            break;
        default:
            HVD_EX_MSG_ERR("hvd clock setting is wrong(%d)\n", eClockSpeed);
            return E_HVD_EX_FAIL;
            break;
    }

    if(E_HVD_RETURN_SUCCESS != HAL_HVD_EX_SetClockSpeed(eHVDClockSpeed))
    {
        return E_HVD_EX_FAIL;
    }

    return E_HVD_EX_OK;
#else
    UNUSED(eClockSpeed);
    HVD_PRINT("This chip does not support hvd clock presetting~\n");
    return E_HVD_EX_RET_UNSUPPORTED;
#endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetVPUClockSpeed()
/// @brief \b Function \b Description:  set vpu clock speed
/// @return - The result of setting vpu clock speed
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetVPUClockSpeed(HVD_EX_ClockSpeed eClockSpeed)
{
#if defined(CHIP_A3)
    HAL_VPU_EX_ClockSpeedType eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_NONE;
    switch (eClockSpeed)
    {
        case E_HVD_EX_CLOCK_SPEED_HIGHEST:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_HIGHEST;
            break;
        case E_HVD_EX_CLOCK_SPEED_HIGH:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_HIGH;
            break;
        case E_HVD_EX_CLOCK_SPEED_MEDIUM:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_MEDIUM;
            break;
        case E_HVD_EX_CLOCK_SPEED_LOW:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_LOW;
            break;
        case E_HVD_EX_CLOCK_SPEED_LOWEST:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_LOWEST;
            break;
        case E_HVD_EX_CLOCK_SPEED_DEFAULT:
            eVPUClockSpeed = E_HAL_VPU_EX_CLOCK_SPEED_DEFAULT;
            break;
        default:
            HVD_EX_MSG_ERR("vpu clock setting is wrong(%d)\n", eVPUClockSpeed);
            return E_HVD_EX_FAIL;
            break;
    }

    if(FALSE == HAL_VPU_EX_SetClockSpeedType(eVPUClockSpeed))
    {
        return E_HVD_EX_FAIL;
    }

    return E_HVD_EX_OK;
#else
    UNUSED(eClockSpeed);
    HVD_PRINT("This chip does not support vpu clock presetting~\n");
    return E_HVD_EX_RET_UNSUPPORTED;
#endif
}

HVD_EX_Result MDrv_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_U8 bEnable)
{
#if (defined(CHIP_EDISON) || defined(CHIP_EINSTEIN) || defined(CHIP_EINSTEIN3)) || defined(CHIP_NAPOLI) || defined(CHIP_MONACO) || defined(CHIP_MUJI) || defined(CHIP_CLIPPERS) || defined(CHIP_MONET)
    if(E_HVD_RETURN_SUCCESS != HAL_HVD_EX_SetHVDColBBUMode(u32Id, bEnable))
        return E_HVD_EX_FAIL;
#endif
    return E_HVD_EX_OK;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DSReportDispInfoChange()
/// @brief \b Function \b Description: Setting report display info change info. when DS enabled.
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable Report display info change info.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_DSReportDispInfoChange(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DS_RESET_XC_DISP_WIN, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);

}

HVD_EX_Result MDrv_HVD_EX_SetSecureMode(MS_U32 u32Id, MS_U32 u32SecureMode)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    pHVDDrvContext->gHVDCtrl_EX[u8DrvId].u8SecureMode = (MS_U8)u32SecureMode;

     _DRV_HVD_EX_RET(u8DrvId, eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SupportRefNumOverMaxDpbSize()
/// @brief \b Function \b Description: Support h264 FHD ref frame up to 6
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable support h264 FHD ref frame up to 6
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SupportRefNumOverMaxDpbSize(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_RVU_Setting_Mode()
/// @brief \b Function \b Description: RVU settings
/// @param -u32Param \u32 IN : 0: diable, 1: Drop B-frame and force IDR.
/// @return -The result of command setting E_HVD_CMD_RVU_SETTING_MODE
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_RVU_Setting_Mode(MS_U32 u32Id, MS_U32 u32Param)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RVU_SETTING_MODE, u32Param);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_FramerateHandling()
/// @brief \b Function \b Description: FramerateHandling
/// @brief \b A.Case of (timing_info_present_flag == 1)
/// @brief \b   i.Use num_units_in_tick and time_scale
/// @brief \b   ii.framerate = time_scale / num_units_in_tick / 2
/// @brief \b   iii.If calculated framerate from ii.
/// @brief \b   iv.If there is no time_scale or num_units_in_tick, framerate should be 30fps as default vaule.
/// @brief \b B.Case of timing_info_present_flag == 0
/// @brief \b   i.Use framerate from application
/// @brief \b C.If there is no VUI information, use framerate from application
/// @param -bu32FrameRate \b IN : framerate from application
/// @return -The result of command for handling framerate
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_FramerateHandling(MS_U32 u32Id, MS_U32 u32FrameRate)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_FRAMERATE_HANDLING, u32FrameRate);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_DualNonBlockMode()
/// @brief \b Function \b Description: handle dual non blocking mode
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command setting E_HVD_CMD_DUAL_NON_BLOCK_MODE
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_DualNonBlockMode(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DUAL_NON_BLOCK_MODE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_IgnorePicStructDisplay()
/// @brief \b Function \b Description: Ignore Pic_struct when display progressive frame.
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable ignore Pic_struct when display progressive frame.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_IgnorePicStructDisplay(MS_U32 u32Id, MS_U32 param)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_IGNORE_PIC_STRUCT_DISPLAY, param);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_InputPtsFreerunMode()
/// @brief \b Function \b Description: video free run when the difference between input PTS and current STC is large than E_HVD_CMD_FREERUN_THRESHOLD + 1s.
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable video free run when the difference between input PTS and current STC is large than E_HVD_CMD_FREERUN_THRESHOLD + 1s.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_InputPtsFreerunMode(MS_U32 u32Id, MS_U32 param)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_INPUT_PTS_FREERUN_MODE, param);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_ErrConcealStartSlice1stMB()
/// @brief \b Function \b Description: Error concealment from current slice first MB.(Need enable E_HVD_CMD_ERR_CONCEAL)
/// @param -bEnable \b IN : turn on / off.
/// @return -The result of command to enable/disable Error concealment from current slice first MB.(Need enable E_HVD_CMD_ERR_CONCEAL)
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_ErrConcealStartSlice1stMB(MS_U32 u32Id, MS_U32 param)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ERR_CONCEAL_SLICE_1ST_MB, param);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetExternalDSBuffer()
/// @brief \b Function \b Description: Set External DS buffer
/// @param -bEnable \b IN : External DS buffer info.
/// @return -The result of command to set external DS buffer.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetExternalDSBuffer(MS_U32 u32Id, HVD_EX_ExternalDSBuf *pExternalBuf)
{
#define SIZE_3K 0xC00
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_BOOL bFWInMIU1 = FALSE;
    MS_BOOL bExternalInMIU1 = FALSE;
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_VIRT u32DSBufAddr = pExternalBuf->u32DSBufAddr;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    if((pShm == NULL) || (pCtrl == NULL) || (pExternalBuf->u32DSBufSize < SIZE_3K)) /// DS buffer size need more than 3K (0xC00)
    {
        return E_HVD_EX_FAIL;
    }

    #if defined(CHIP_MONACO)
    if(VPU_FORCE_MIU_MODE == 1)
    {
        HVD_EX_MSG_ERR("this chip does not support set external DS buffer since VPU_FORCE_MIU_MODE == 1 !!\n");
        return E_HVD_EX_RET_UNSUPPORTED;
    }
    #endif

    ///printf("[EDS] 0x%lx, 0x%lx.\n",pCtrl->MemMap.u32CodeBufAddr,pCtrl->MemMap.u32MIU1BaseAddr);
    if((pCtrl->MemMap.u32CodeBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr) && (pCtrl->MemMap.u32MIU1BaseAddr != 0))
    {
        bFWInMIU1 = TRUE;
    }

    if((pExternalBuf->u32DSBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)  && (pCtrl->MemMap.u32MIU1BaseAddr != 0) )
    {
        bExternalInMIU1 = TRUE;
        u32DSBufAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }
    //printf("[EDS] MIU %d, %d.\n",bFWInMIU1,bExternalInMIU1);

    /// If FW and external buf is in the same MIU, the External buffer do nothing.
    if(bFWInMIU1 == bExternalInMIU1)
    {
        pShm->u32DSbufferAddr = u32DSBufAddr;
    }
    else
    {
        /// If FW and external buf is not in the same MIU, the External buffer add the VPU Miu base size
        pShm->u32DSbufferAddr = u32DSBufAddr + HAL_VPU_EX_MIU1BASE();
    }
    pShm->u32DSBuffSize = pExternalBuf->u32DSBufSize;

    MsOS_FlushMemory();
    ///printf("[EDS] DS addr = 0x%lx, size = 0x%lx.\n",pShm->u32DSbufferAddr,pShm->u32DSBuffSize);
    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_EXTERNAL_DS_BUF, TRUE);

    pCtrl->u8SettingMode |= HVD_DRV_MODE_EXTERNAL_DS_BUFFER;
    pCtrl->u32ExternalDSbuf = pExternalBuf->u32DSBufAddr;

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

MS_U32 MDrv_HVD_EX_GetESBufferStatus(MS_U32 u32Id)
{
    MS_U32 u32Ret = 0;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    _DRV_HVD_Inited(u8DrvId,FALSE);

    u32Ret = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_ES_BUF_STATUS);
    u32Ret = _HVD_EX_Map2HVDESBufStatus(u32Ret);

    return u32Ret;
}

static MS_U32 _HVD_EX_CC_Update_UserData_Wptr(MS_U32 u32Id)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);
    MS_U32 u32UsrDataIdxWptr = MDrv_HVD_EX_GetUserData_Wptr(u32Id);

    if (u32UsrDataIdxWptr != 0xFFFFFFFF)
        pCCInfo->u32FWUsrDataWIdx = u32UsrDataIdxWptr;

    return pCCInfo->u32FWUsrDataWIdx;
}

static void _HVD_EX_CC_Parse_AFD_data(MS_U8 **pUserDataBuf)
{

    (*pUserDataBuf)++;
    (*pUserDataBuf)++;

}

static void _HVD_EX_CC_Parse_BAR_data(MS_U8 **pUserDataBuf)
{

    MS_BOOL bTopBarFlag, bBottomBarFlag, bLeftBarFlag, bRightBarlag;

    bTopBarFlag    = (((**pUserDataBuf) & 0x80) == 0x80); //u_f(1,"top_bar_flag");
    bBottomBarFlag = (((**pUserDataBuf) & 0x40) == 0x40); //u_f(1,"bottom_bar_flag");
    bLeftBarFlag   = (((**pUserDataBuf) & 0x20) == 0x20); //u_f(1,"left_bar_flag");
    bRightBarlag  = ((*((*pUserDataBuf)++) & 0x10) == 0x10); //u_f(1,"right_bar_flag"); u_f(4,"reserved");

    if (bTopBarFlag)
    {
        *pUserDataBuf += 2;
    }

    if (bBottomBarFlag)
    {
        *pUserDataBuf += 2;
    }

    if (bLeftBarFlag)
    {
        *pUserDataBuf += 2;
    }

    if (bRightBarlag)
    {
        *pUserDataBuf += 2;
    }

}

static void _HVD_EX_CC_Parse_CC_data(MS_U32 u32Id, MS_U8 u8CCMode,MS_U8 **pSrc, MS_U8 *pDst608, MS_U32 *pu32Dst608Len, MS_U8 *pDst708, MS_U32 *pu32Dst708Len, HVD_CC_608EnhanceInfo *pEhInfo608)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);

    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 i,j;
    MS_U32 u32CC_Count;
    MS_U8 u8CC_Code;
    MS_U8 u8CC_Type;
    MS_U8 u8CC_Data1, u8CC_Data2;
    MS_U32 u32Pad;



    u8CC_Code = *((*pSrc)++); //u_f(8,"cc_data_flag cc_count");

    u32CC_Count = u8CC_Code & 0x1F;            // cc_count
    if (!(u8CC_Code & 0x40)) return;        // process_cc_data_flag



    (*pSrc)++; //u_f(8,"reserved 1");



    for (i=0;i<u32CC_Count;i++)
    {
        MS_U8 u8Code;


        u8Code = *((*pSrc)++); // u_f(8,"cc_valid and cc_type");


        u8CC_Type  = u8Code & 0x03;

        u8CC_Data1 = *((*pSrc)++); // u_f(8,"cc_data_1");
        u8CC_Data2 = *((*pSrc)++); // u_f(8,"cc_data_2");

        if (!(u8Code & 0x04)) continue; // cc_valid

        //NTSC CC 608
        if ((u8CC_Type == 0) || (u8CC_Type == 1))
        {
            //g_DTV_CC.ntsc_cc_data[g_DTV_CC.NTSC_element++] = (cc_type << 16) | (cc_data2 << 8) | (cc_data1);

            if(pCCInfo->b608InfoEnhance)
            {
                if( ( (*pu32Dst608Len) + 16) > MAX_608_CC_LEN)
                {
                    HVD_EX_MSG_ERR("u32Dst608Len+16: %ld is larger than MAX_608_CC_LEN !! \n",((*pu32Dst608Len)+16));
                    return;
                }
            }
            else
            {
                if( ( (*pu32Dst608Len) + 4) > MAX_608_CC_LEN)
                {
                    HVD_EX_MSG_ERR("u32Dst608Len+4: %ld is larger than MAX_608_CC_LEN !! \n",((*pu32Dst608Len)+4));
                    return;
                }
            }

            pDst608[(*pu32Dst608Len)++] = u8CC_Data1;
            pDst608[(*pu32Dst608Len)++] = u8CC_Data2;
            pDst608[(*pu32Dst608Len)++] = u8CC_Type;
            pDst608[(*pu32Dst608Len)++] = 0;

            if (pCCInfo->b608InfoEnhance)
            {
                //   PTS searched_pts;
                //search_pts(pContext, &searched_pts);
                pDst608[(*pu32Dst608Len)++] = (pEhInfo608->u16TmpRef&0xff);
                pDst608[(*pu32Dst608Len)++] = ((pEhInfo608->u16TmpRef>>8)&0xff);
                pDst608[(*pu32Dst608Len)++] = (pEhInfo608->u16PicStruct&0xff);
                pDst608[(*pu32Dst608Len)++] = ((pEhInfo608->u16PicStruct>>8)&0xff);
                pDst608[(*pu32Dst608Len)++] = (pEhInfo608->u32Pts&0xff);
                pDst608[(*pu32Dst608Len)++] = ((pEhInfo608->u32Pts>>8)&0xff);
                pDst608[(*pu32Dst608Len)++] = ((pEhInfo608->u32Pts>>16)&0xff);
                pDst608[(*pu32Dst608Len)++] = ((pEhInfo608->u32Pts>>24)&0xff);
                pDst608[(*pu32Dst608Len)++] = pEhInfo608->u8UsrDataCnt;

                // for enhance cc-608 data dump, 16bytes/pkt
                u32Pad = (*pu32Dst608Len) & 0xf; //align to 16 byte
                if(u32Pad) u32Pad = 16-u32Pad;
                for(j=0;j<u32Pad;j++)
                    pDst608[(*pu32Dst608Len)++] = 0xff;

            }




        }
        else // ATSC_CC 708
        {
            if( ( (*pu32Dst708Len) + 4) > MAX_708_CC_LEN)
            {
                HVD_EX_MSG_ERR(" u32Dst708Len+4: %ld is larger than MAX_708_CC_LEN !! \n",((*pu32Dst708Len)+4));
                return;
            }
            pDst708[(*pu32Dst708Len)++] = u8CC_Data1;
            pDst708[(*pu32Dst708Len)++] = u8CC_Data2;
            pDst708[(*pu32Dst708Len)++] = u8CC_Type;
            pDst708[(*pu32Dst708Len)++] = 0;

        }
        //new DTV CC header

    }



    //padding cc608


    if(u8CCMode & (CC_NTSC1|CC_NTSC2))
    {
        if (pCCInfo->b608InfoEnhance==0)
        {   // for original cc-608 data dump, 16bytes/pkt


            u32Pad = (*pu32Dst608Len) & 0x7;
            if(u32Pad) u32Pad = 8 - u32Pad;

            if((u32Pad + (*pu32Dst608Len)) > MAX_608_CC_LEN)
            {
                HVD_EX_MSG_ERR("u32Dst608Len:%ld + u32Pad: %ld is larger than MAX_608_CC_LEN !! \n",(*pu32Dst608Len), u32Pad);
                return;
            }
            for(i=0;i<u32Pad;i++)
                pDst608[(*pu32Dst608Len)++] = 0xff;
        }
    }


}


static void _HVD_EX_CC_Extract_CCdata(MS_U32 u32Id, MS_U8 *pDst608Buf ,MS_U32 *pu32Dst608Len,MS_U8 *pDst708Buf ,MS_U32 *pu32Dst708Len, DTV_BUF_type* pUserData)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);



    MS_U8 * pUserDataStartBuf = &pUserData->buf[0];//&pSrcBuf[0];
    MS_U8 * pUserDataBuf;

    pUserDataBuf = &pUserData->buf[0];


    HVD_CC_608EnhanceInfo stEhInfo608;
    stEhInfo608.u16TmpRef = pUserData->u16TempRefCnt;
    stEhInfo608.u16PicStruct = (MS_U16)pUserData->pic_struct;
    stEhInfo608.u32Pts = pUserData->pts;
    stEhInfo608.u8UsrDataCnt = 0;


    {
        MS_U32 dvb_code;

        dvb_code = (*pUserDataBuf << 24) | (*(pUserDataBuf+1) << 16) | (*(pUserDataBuf+2) << 8) | *(pUserDataBuf+3); //(u_f(16,"provider_code") << 16) | u_f(16,"provider_code");
        pUserDataBuf += 4;

        if (dvb_code == DTV_DTB1_CODE)
        {
            MS_U8 code;

            code = *pUserDataBuf++; //u_f(8,"code");

            if (code == DTV_CC_CODE)
            {
                _HVD_EX_CC_Parse_CC_data(u32Id, pCCInfo->u8ParsingStatus,&pUserDataBuf, pDst608Buf, pu32Dst608Len, pDst708Buf, pu32Dst708Len, &stEhInfo608);
            }
            else if (code == DTV_BAR_CODE)
            {
                _HVD_EX_CC_Parse_BAR_data(&pUserDataBuf);
            }

        }
        else if (dvb_code == DTV_AFD_CODE)
        {
            _HVD_EX_CC_Parse_AFD_data(&pUserDataBuf);
        }

    }


    if((pUserDataBuf - pUserDataStartBuf) > pUserData->len)
        HVD_EX_MSG_ERR("ERROR!!!!  (pUserDataBuf - pUserDataStartBuf) > u32SrcLen\n");


}


static MS_BOOL _HVD_EX_CC_RB_Have_Avail_Space(MS_U32 u32Id, MS_U8 u8CC608, MS_U32 u32QuerySize)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 u32RingBufVacancy = pCCInfo->u32RingBufVacancy[u8CC608];
    MS_U32 u32AlignCCLen;

    if ((u32QuerySize % HVD_CCRB_PACKET_LENGTH) != 0)
    {
        u32AlignCCLen = HVD_CCRB_PACKET_LENGTH * ((u32QuerySize / HVD_CCRB_PACKET_LENGTH)+1);
    }
    else
    {
        u32AlignCCLen = u32QuerySize;
    }


    if(u32RingBufVacancy >= u32AlignCCLen)
        return TRUE;
    else
        return FALSE;


}

static MS_BOOL _HVD_EX_CC_Copy_CCdata_To_RB(MS_U32 u32Id, MS_U8 u8CC608,MS_U8 *pSrcBuf, MS_U32 u32SrcLen)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 u32RingBufRPAddr = pCCInfo->u32RingBufRPAddr[u8CC608];
    MS_U32 u32RingBufWPAddr = pCCInfo->u32RingBufWPAddr[u8CC608];
    MS_U32 u32AlignCCLen;



    if(u32SrcLen == 0)
        return FALSE;



    u32AlignCCLen = u32SrcLen;


    if(pCCInfo->u32RingBufVacancy[u8CC608] < u32AlignCCLen)
    {
        HVD_EX_MSG_ERR("overflow !!!, u32RingBufVacancy: %ld, u32AlignLen: %ld\n",pCCInfo->u32RingBufVacancy[u8CC608], u32AlignCCLen);
        return TRUE;
    }


    if(u32RingBufWPAddr >= u32RingBufRPAddr)
    {
        if( (u32RingBufWPAddr + u32AlignCCLen) <= (pCCInfo->u32RingBufStartPAddr[u8CC608] + pCCInfo->u32RingBufLen[u8CC608]))
        {

            HVD_memcpy((void *)(HVD_PA2VA(u32RingBufWPAddr)), (void *)pSrcBuf, u32AlignCCLen);


            if((pCCInfo->u32RingBufWPAddr[u8CC608] + u32AlignCCLen) == (pCCInfo->u32RingBufStartPAddr[u8CC608] + pCCInfo->u32RingBufLen[u8CC608]))
                pCCInfo->u32RingBufWPAddr[u8CC608] = pCCInfo->u32RingBufStartPAddr[u8CC608];
            else
                pCCInfo->u32RingBufWPAddr[u8CC608] += u32AlignCCLen;

            pCCInfo->u32RingBufVacancy[u8CC608] -= u32AlignCCLen;

        }
        else
        {
            MS_U32 part1_len = (pCCInfo->u32RingBufStartPAddr[u8CC608] + pCCInfo->u32RingBufLen[u8CC608]) - u32RingBufWPAddr;
            MS_U32 part2_len = u32AlignCCLen - part1_len;




            {


                HVD_memcpy((void *)(HVD_PA2VA(u32RingBufWPAddr)),(void *)pSrcBuf, part1_len);

                pCCInfo->u32RingBufWPAddr[u8CC608] = u32RingBufWPAddr = pCCInfo->u32RingBufStartPAddr[u8CC608];

                HVD_memcpy((void *)(HVD_PA2VA(u32RingBufWPAddr)),(void *)(&pSrcBuf[part1_len]), part2_len);

                pCCInfo->u32RingBufWPAddr[u8CC608] = u32RingBufWPAddr = pCCInfo->u32RingBufStartPAddr[u8CC608] + part2_len;
                pCCInfo->u32RingBufVacancy[u8CC608] -= u32AlignCCLen;
            }

        }

    }
    else
    {


        {

            HVD_memcpy((void *)(HVD_PA2VA(u32RingBufWPAddr)), (void *)pSrcBuf, u32AlignCCLen);
            pCCInfo->u32RingBufWPAddr[u8CC608] += u32AlignCCLen;
            pCCInfo->u32RingBufVacancy[u8CC608] -= u32AlignCCLen;
        }


    }

    return FALSE;
}

static MS_BOOL _HVD_EX_CC_Add_PaddingData_To_608(MS_U32 u32Id, MS_U8 *pSrcBuf, MS_U32 u32SrcLen)
{

    MS_U32 u32AlignCCLen;
    MS_U32 i;
    MS_BOOL b608OverFlow;

    if ((u32SrcLen % HVD_CCRB_PACKET_LENGTH) != 0)
    {
        u32AlignCCLen = HVD_CCRB_PACKET_LENGTH * ((u32SrcLen / HVD_CCRB_PACKET_LENGTH)+1);
    }
    else
    {
        u32AlignCCLen = u32SrcLen;
    }



    if(u32AlignCCLen > MAX_608_CC_LEN)
    {
        HVD_EX_MSG_ERR("u32AlignCCLen:%ld > MAX_608_CC_LEN", u32AlignCCLen);
        return TRUE;
    }



    for(i =0; i < (u32AlignCCLen-u32SrcLen);i++)
        pSrcBuf[u32SrcLen+i] = 0xff;// add padinng data

    b608OverFlow =_HVD_EX_CC_Copy_CCdata_To_RB(u32Id, 1, &(pSrcBuf[0]), u32AlignCCLen);
    return b608OverFlow;

}

static MS_BOOL _HVD_EX_CC_Add_PaddingData_To_708(MS_U32 u32Id, MS_U8 *pSrcBuf, MS_U32 u32SrcLen)
{
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 *u32RingBufWAddr = (MS_U32 *)(HVD_PA2VA(pCCInfo->u32RingBufWPAddr[0]));

    MS_U8  u8CC_Type, u8CC_Data1, u8CC_Data2,u8Temp;
    MS_U16 i,j,u16Pad,u16DataCount;
    MS_U8  *pAddr;
    MS_U8 u8CCPading[MAX_708_CC_LEN];
    MS_BOOL b708OverFlow = FALSE;
    u16DataCount = 0;

    for(j=0;j<u32SrcLen;j+=4)
    {
        u8CC_Data1 = *pSrcBuf++;
        u8CC_Data2 = *pSrcBuf++;
        u8CC_Type  = *pSrcBuf++;
        u8Temp     = *pSrcBuf++;

        if(u8CC_Type==3) //padd_out();
        {
            if( pCCInfo->u32PktLen708 > 0 )
            {
                u16Pad = pCCInfo->u32PktLen708;
                for (i=0;i<u16Pad;i++)
                    u8CCPading[u16DataCount++] = 0xff;
                pCCInfo->u32PktLen708 = 0;
#if 1

                pAddr = (MS_U8 *)pCCInfo->u32PktHdrAddr708;
                if(pAddr)
                {
                    u8Temp = *pAddr;
                    u16Pad = (u8Temp&0x3f)<<1;
                    if((i!=0)&&(u16Pad > i))
                    {
                        i = i>>1;
                        u8Temp = u8Temp - i;
                        *pAddr = (u8Temp&0xff);
                    }
                }
#endif
            }

            u16Pad = (pCCInfo->u32RingBufWPAddr[0] + u16DataCount) & 7;
            if(u16Pad) u16Pad = HVD_CCRB_PACKET_LENGTH-u16Pad;
            for(i=0;i<u16Pad;i++)
                u8CCPading[u16DataCount++] = 0xff;


            b708OverFlow =_HVD_EX_CC_Copy_CCdata_To_RB(u32Id, 0, u8CCPading, u16DataCount);
            if(b708OverFlow)
                return TRUE;
            pCCInfo->u32PktLen708 = (u8CC_Data1 & 0x3f)*2;
            if(pCCInfo->u32PktLen708 == 0) pCCInfo->u32PktLen708 = 128;
            u16DataCount = 0;

            pCCInfo->u32PktHdrAddr708 = (MS_U32)(u32RingBufWAddr);
        }

        if(pCCInfo->u32PktLen708 > 0)
        {
            u8CCPading[u16DataCount++] = u8CC_Data1;
            u8CCPading[u16DataCount++] = u8CC_Data2;
            pCCInfo->u32PktLen708 = pCCInfo->u32PktLen708 - 2;
        }
        else
        {
            pCCInfo->u32PktLen708 = 0;
        }
    }

    if(u16DataCount > 0)
    {
        if( pCCInfo->u32PktLen708 ==0 )
        {
            u16Pad = (pCCInfo->u32RingBufWPAddr[0] + u16DataCount) & 7;
            if(u16Pad) u16Pad = HVD_CCRB_PACKET_LENGTH-u16Pad;
            for(i=0;i<u16Pad;i++)
                u8CCPading[u16DataCount++] = 0xff;

        }

        b708OverFlow = _HVD_EX_CC_Copy_CCdata_To_RB(u32Id, 0, u8CCPading, u16DataCount);
        if(b708OverFlow)
            return TRUE;
    }

    return FALSE;

}


///////////////////////////////////////////////////////////////////////////////
/// Closed Caption Driver Initialization
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_Init(MS_U32 u32Id)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);


    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    pCCInfo->u8UserDataMode = (USER_DATA_MODE_DIRECTTV_CC|USER_DATA_MODE_ATSC_CC_RAW);

    eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_USERDATA_MODE, pCCInfo->u8UserDataMode);

    if (pCCInfo->u8ParsingStatus == 0xff)
        pCCInfo->u8ParsingStatus = 0x00;

    //eRet = E_HVD_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

///////////////////////////////////////////////////////////////////////////////
/// Set Closed Caption config
/// @param u8Operation \b IN Digital Closed Caption Type
/// @param u16BufferSize \b IN buffer size of the given hardware parser
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_SetCfg(MS_U32 u32Id, MS_U8 u8Operation, MS_U16 u16BufferSize, MS_U8 u8CC608)
{

    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);



    pCCInfo->u32RingBufLen[u8CC608] =  pCCInfo->u32RingBufVacancy[u8CC608] = u16BufferSize;
    pCCInfo->u8ParsingStatus |= u8Operation;
    pCCInfo->u8ParsingStatus &= 0x07;

    HVD_EX_MSG_DBG("u8Operation: %x, u16BufferSize:%x, u8CC608:%d\n",u8Operation, u16BufferSize,u8CC608);


    if (u8CC608==0x00)
        pCCInfo->b708Enable = TRUE;

    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);

}


///////////////////////////////////////////////////////////////////////////////
/// Set HVD Ring Buffer's Start Address
/// @param u32StartAddress \b IN Ring Buffer's Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_Set_RB_StartAddr(MS_U32 u32Id, MS_U32 u32StartPAddress, MS_U8 u8CC608)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U8 u8MiuSel;
    MS_U32 u32StartOffset;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    _phy_to_miu_offset(u8MiuSel,u32StartOffset,u32StartPAddress);

    pCCInfo->u8BufMiuSel[u8CC608] = u8MiuSel;
    /*
    if ( (u32StartPAddress  &  pCtrl->MemMap.u32MIU1BaseAddr) == pCtrl->MemMap.u32MIU1BaseAddr)
    {
        pCCInfo->bBufMiu1[u8CC608] = TRUE;
    }
    else
    {
        pCCInfo->bBufMiu1[u8CC608] = FALSE;
    }*/


    HVD_EX_MSG_DBG("u32StartPAddress: 0x%lx ,u8CC608: 0x%x, pCCInfo->u8BufMiuSel[u8CC608]: %d, pCtrl->MemMap.u32MIU1BaseAddr: 0x%lx, pCtrl->MemMap.u32MIU2BaseAddr: 0x%lx\n",
        u32StartPAddress , u8CC608,pCCInfo->u8BufMiuSel[u8CC608], pCtrl->MemMap.u32MIU1BaseAddr, pCtrl->MemMap.u32MIU2BaseAddr);


    pCCInfo->u32RingBufStartPAddr[u8CC608] = pCCInfo->u32RingBufRPAddr[u8CC608] = pCCInfo->u32RingBufWPAddr[u8CC608] = u32StartPAddress;
    if(pCCInfo->u32RingBufLen[u8CC608] > 0)
        pCCInfo->u32RingBufVacancy[u8CC608] = pCCInfo->u32RingBufLen[u8CC608];


    _HVD_EX_CC_Update_UserData_Wptr(u32Id);
    pCCInfo->u32FWUsrDataRIdx = /*pCCInfo->u32FWUsrDataPrevWIdx =*/ pCCInfo->u32FWUsrDataWIdx;


    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);

}


///////////////////////////////////////////////////////////////////////////////
/// Set HVD Ring Buffer's Read Address as Write Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_SyncRB_RdAddr2WrAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);


    pCCInfo->u32RingBufRPAddr[u8CC608] = pCCInfo->u32RingBufWPAddr[u8CC608];//u32ReadAddress;
    _HVD_EX_CC_Update_UserData_Wptr(u32Id);
    pCCInfo->u32FWUsrDataRIdx = pCCInfo->u32FWUsrDataWIdx;
    if(pCCInfo->u32RingBufLen[u8CC608] > 0)
        pCCInfo->u32RingBufVacancy[u8CC608] = pCCInfo->u32RingBufLen[u8CC608];
    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);

}

///////////////////////////////////////////////////////////////////////////////
/// Advance Read Address of HVD CC Ring Buffer by u32EachPacketSize
/// @param u32EachPacketSize \b IN
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_Adv_RB_ReadAddr(MS_U32 u32Id, MS_U32 u32EachPacketSize, MS_U8 u8CC608)
{

    HVD_EX_Result eRet = E_HVD_EX_FAIL;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 u32ParsedDataSize;

    // Get the Parsed Data Size
    if ((u32EachPacketSize % HVD_CCRB_PACKET_LENGTH) != 0)
    {
        u32ParsedDataSize = HVD_CCRB_PACKET_LENGTH * ((u32EachPacketSize / HVD_CCRB_PACKET_LENGTH)+1); //HVD_CCRB_PACKET_LENGTH bytes alignment
    }
    else
    {
        u32ParsedDataSize = u32EachPacketSize;
    }

    if(u32ParsedDataSize > (pCCInfo->u32RingBufLen[u8CC608] - pCCInfo->u32RingBufVacancy[u8CC608]))
    {
        HVD_EX_MSG_ERR("MDrv_HVD_CC_Adv_RB_ReadAddr fail!, no enough data!, vacancy: %ld, u32ParsedDataSize: %ld, u32RingBufLen:%ld, u32RingBufRPAddr: 0x%lx, u32RingBufWPAddr: 0x%lx\n",
                        pCCInfo->u32RingBufVacancy[u8CC608], u32ParsedDataSize, pCCInfo->u32RingBufLen[u8CC608], pCCInfo->u32RingBufRPAddr[u8CC608],pCCInfo->u32RingBufWPAddr[u8CC608]);
        return E_HVD_EX_FAIL;
    }


    if ((pCCInfo->u32RingBufRPAddr[u8CC608] + u32ParsedDataSize) < (pCCInfo->u32RingBufStartPAddr[u8CC608] + pCCInfo->u32RingBufLen[u8CC608]))
    {
        pCCInfo->u32RingBufRPAddr[u8CC608] += u32ParsedDataSize;
    }
    else
    {
        pCCInfo->u32RingBufRPAddr[u8CC608] = (u32ParsedDataSize + pCCInfo->u32RingBufRPAddr[u8CC608] - pCCInfo->u32RingBufLen[u8CC608]);
    }

    pCCInfo->u32RingBufVacancy[u8CC608] += u32ParsedDataSize ;

    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);

}

///////////////////////////////////////////////////////////////////////////////
/// Stop HVD CC parsing
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
HVD_EX_Result MDrv_HVD_EX_CC_DisableParsing(MS_U32 u32Id, MS_U8 u8CC608)
{

    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    if (u8CC608==0)
    {
        pCCInfo->u8ParsingStatus &= ~0x04;
        pCCInfo->b708Enable = FALSE;
    }
    else
        pCCInfo->u8ParsingStatus &= ~0x03;

    pCCInfo->u8ParsingStatus &= 0x07;

    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


HVD_EX_Result MDrv_HVD_EX_CC_GetInfo(MS_U32 u32Id, MS_U32 selector, MS_U8 type, MS_U32 *p1, MS_U32 *p2)
{

    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    // Functionality select
    switch (selector)
    {
        case HVD_EX_CC_SELECTOR_708_SW:
            *p1 = (MS_U32)pCCInfo->b708Enable;
            break;

#if defined( MSOS_TYPE_LINUX )
        case HVD_EX_CC_SELECTOR_RINGBUFFER:
            {
                MS_U32 u32PAddr;
                u32PAddr = pCCInfo->u32RingBufStartPAddr[type];
                *p1 = u32PAddr;
                *p2 = (MS_U32) pCCInfo->u32RingBufLen[type];
            }
            break;
#endif

        default:
            *p1 = *p2 = 0;
            break;
    }
    eRet = E_HVD_EX_OK;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


MS_BOOL MDrv_HVD_EX_CC_IsHvdRstDone(MS_U32 u32Id, MS_U8 type)
{
    MS_BOOL bRet = FALSE;

    bRet = TRUE;

    return bRet;
}

///////////////////////////////////////////////////////////////////////////////
/// Get Overflow Status of HVD CC Ring Buffer
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: Overflow Status Flag
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_HVD_EX_CC_GetOverflowStatus(MS_U32 u32Id, MS_U8 u8CC608)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_BOOL bOverFlow = pCCInfo->bOverFlow[u8CC608];
    pCCInfo->bOverFlow[u8CC608] = FALSE;
    return (bOverFlow);
}





///////////////////////////////////////////////////////////////////////////////
/// Get Write Address of HVD CC Ring Buffer
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: Write Address
///////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_HVD_EX_CC_Get_RB_WriteAddr(MS_U32 u32Id, MS_U8 u8CC608)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_U32 u32UserDataIdxSize = 0;
    MS_U32 u32UsrDataIdxWptr = 0xFFFFFFFF;
    MS_U32 u32UserDataSize = 0;
    MS_U32 u32UserDataAddr = 0;
    DTV_BUF_type* pHVD_User_Data = NULL;

    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);
    MS_U32 u32FwParseCnt;
    MS_U32 u32PAddr;


    if ((pCCInfo->u8ParsingStatus == 0xff) || (pCCInfo->u8ParsingStatus == 0x00))
    {

         u32PAddr = pCCInfo->u32RingBufWPAddr[u8CC608];
        /*
        if(!pCCInfo->bBufMiu1[u8CC608])
            u32PAddr = u32PAddr & (pCtrl->MemMap.u32MIU1BaseAddr - 1);
        */
        if(pCCInfo->u8BufMiuSel[u8CC608] == E_CHIP_MIU_0)
            u32PAddr = u32PAddr & (pCtrl->MemMap.u32MIU1BaseAddr - 1);


        return u32PAddr;
    }

    u32UserDataIdxSize = (MS_U32)HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_USERDATA_IDX_TBL_SIZE);

    u32UsrDataIdxWptr =_HVD_EX_CC_Update_UserData_Wptr(u32Id);



    if (pCCInfo->u32FWUsrDataRIdx == u32UsrDataIdxWptr)
    {
        // User Data Buffer Empty
        HVD_EX_MSG_DBG("RIdx(%ld) == WIdx(%ld) , empty !!!\n", pCCInfo->u32FWUsrDataRIdx,u32UsrDataIdxWptr);
        u32PAddr = pCCInfo->u32RingBufWPAddr[u8CC608];

        if(pCCInfo->u8BufMiuSel[u8CC608] == E_CHIP_MIU_0)
            u32PAddr = u32PAddr & (pCtrl->MemMap.u32MIU1BaseAddr - 1);

        return u32PAddr;

    }
    else if (u32UsrDataIdxWptr > pCCInfo->u32FWUsrDataRIdx)
    {
        u32FwParseCnt = u32UsrDataIdxWptr - pCCInfo->u32FWUsrDataRIdx;
    }
    else// pCCInfo->u32FWUsrDataWIdx < pCCInfo->u32FWUsrDataRIdx
    {
        u32FwParseCnt =  u32UserDataIdxSize - (pCCInfo->u32FWUsrDataRIdx - u32UsrDataIdxWptr);
    }

    HVD_EX_MSG_DBG("u32FwParseCnt(%ld)  !!!,  pCCInfo->u32RingBufWPAddr[%d]: 0x%lx\n", u32FwParseCnt, u8CC608,pCCInfo->u32RingBufWPAddr[u8CC608] );

    MS_BOOL b608OverFlow = FALSE, b708OverFlow = FALSE;
    MS_U8 *pCC608buf = &(pCCInfo->u8CC608buf[0]);
    MS_U8 *pCC708buf = &(pCCInfo->u8CC708buf[0]);
    MS_U32 u32CC608Len = 0;
    MS_U32 u32CC708Len = 0;




    while(u32FwParseCnt)
    {



        if( (!_HVD_EX_CC_RB_Have_Avail_Space(u32Id, 1, MAX_608_CC_LEN)) || (!_HVD_EX_CC_RB_Have_Avail_Space(u32Id, 0, MAX_708_CC_LEN)))
        {
            HVD_EX_MSG_DBG("not have available space!!!\n");
            break;
        }
        u32UserDataAddr = MDrv_HVD_EX_GetUserData_Packet(u32Id, pCCInfo->u32FWUsrDataRIdx, (MS_U32*)&u32UserDataSize);
        u32UserDataAddr += pCtrl->MemMap.u32CodeBufVAddr; // change to virtual address

        pHVD_User_Data = (DTV_BUF_type*)u32UserDataAddr;

        u32CC608Len  = 0;
        u32CC708Len  = 0;



        _HVD_EX_CC_Extract_CCdata(u32Id, pCC608buf, &u32CC608Len,pCC708buf, &u32CC708Len, pHVD_User_Data);

        if(u32CC608Len > 0)
            b608OverFlow =_HVD_EX_CC_Add_PaddingData_To_608(u32Id, &(pCCInfo->u8CC608buf[0]), u32CC608Len);




        if(u32CC708Len > 0)
            b708OverFlow = _HVD_EX_CC_Add_PaddingData_To_708(u32Id, &(pCCInfo->u8CC708buf[0]), u32CC708Len);
            //b708OverFlow =_HVD_CC_Copy_CCdata_To_RB(0, &CC708buf[0], u32CC708Len);




        if(b608OverFlow)
        {
            HVD_EX_MSG_ERR("##### Critical error!!!! b608OverFlow: %d!!!\n",b608OverFlow);
            pCCInfo->bOverFlow[1] = TRUE;
        }

        if(b708OverFlow)
        {
            HVD_EX_MSG_ERR("##### Critical error!!!! b708OverFlow:%d !!!\n",b708OverFlow);
            pCCInfo->bOverFlow[0] = TRUE;
        }



        pCCInfo->u32FWUsrDataRIdx++;

        if (pCCInfo->u32FWUsrDataRIdx >= u32UserDataIdxSize)
        {
            pCCInfo->u32FWUsrDataRIdx = 0;
        }


        u32FwParseCnt--;
    }


    u32PAddr = pCCInfo->u32RingBufWPAddr[u8CC608];
    if(pCCInfo->u8BufMiuSel[u8CC608] == E_CHIP_MIU_0)
            u32PAddr = u32PAddr & (pCtrl->MemMap.u32MIU1BaseAddr - 1);

    return u32PAddr;



}


///////////////////////////////////////////////////////////////////////////////
/// Get Read Address of HVD CC Ring Buffer
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: Read Address
///////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_HVD_EX_CC_Get_RB_ReadAddr(MS_U32 u32Id, MS_U8 u8CC608)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    MS_U32 u32PAddr = pCCInfo->u32RingBufRPAddr[u8CC608];
    if(pCCInfo->u8BufMiuSel[u8CC608] == E_CHIP_MIU_0)
            u32PAddr = u32PAddr & (pCtrl->MemMap.u32MIU1BaseAddr - 1);

    return u32PAddr;

}

///////////////////////////////////////////////////////////////////////////////
/// Enhance CC info mode, only support cc608
/// @param bEnable \b IN
///   - # TRUE for enhance to dump the pts/tmp_ref info
///   - # FALSE for original
///////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HVD_EX_CC_InfoEnhanceMode(MS_U32 u32Id, MS_BOOL bEnable)
{

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    HVD_CC_Info *pCCInfo = &(pCtrl->CloseCaptionInfo);

    pCCInfo->b608InfoEnhance = bEnable;

    return TRUE;
}




HVD_EX_Result MDrv_HVD_EX_SetExternal_CC608_Buffer(MS_U32 u32Id, MS_U32 u32Addr, MS_U32 u32Len)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_U8 u8UserDataMode = (USER_DATA_MODE_DIRECTTV_CC|USER_DATA_MODE_ATSC_CC_RAW|USER_DATA_MODE_EXTERNAL_CC_BUFFER);
    MS_U32 u32CCinfo;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    u32Addr <<= 10;// 1kbytes unit
    u32Len  <<= 10; // 1kbytes unit

    // Read Ptr (4 bytes), Write Ptr (4 bytes), Latch Bit (2 bytes), overflow (2 bytes), Miu Base Address (4 bytes)
    MS_U32 *u32ReadPtr = (MS_U32 *)(HVD_PA2VA(u32Addr));
    MS_U32 *u32WritePtr = (MS_U32 *)(HVD_PA2VA(u32Addr+4));
    MS_U32 *u32LatchOverflow = (MS_U32 *)(HVD_PA2VA(u32Addr+8));
    MS_U32 *u32FWBinaryAddr = (MS_U32 *)(HVD_PA2VA(u32Addr+12));

    *u32ReadPtr = *u32WritePtr = u32Addr+EXT_608_CC_DATA_ALIGN;
    *u32LatchOverflow = 0;


    u32Addr -= pCtrl->MemMap.u32FWBinaryAddr;
    *u32FWBinaryAddr = pCtrl->MemMap.u32FWBinaryAddr;

    u32CCinfo =  ((u32Addr >> 10) << 8) & 0xffffff00; //arg[31:8] * 1kbytes
    u32CCinfo |= ((u32Len >> 10) & 0xff); //arg[7:0] *1kbytes

    eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_USERDATA_MODE, u8UserDataMode);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ENABLE_EXTERNAL_CC_608_BUF, u32CCinfo);

    _DRV_HVD_EX_RET(u8DrvId, eRet);


}

HVD_EX_Result MDrv_HVD_EX_SetExternal_CC708_Buffer(MS_U32 u32Id, MS_U32 u32Addr, MS_U32 u32Len)
{

    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = &(pHVDDrvContext->gHVDCtrl_EX[u8DrvId]);
    MS_U8 u8UserDataMode = (USER_DATA_MODE_DIRECTTV_CC|USER_DATA_MODE_ATSC_CC_RAW|USER_DATA_MODE_EXTERNAL_CC_BUFFER);
    MS_U32 u32CCinfo;

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    u32Addr <<= 10;// 1kbytes unit
    u32Len  <<= 10; // 1kbytes unit

    // Read Ptr (4 bytes), Write Ptr (4 bytes), Latch Bit (2 bytes), overflow (2 bytes), Miu Base Address (4 bytes)
    MS_U32 *u32ReadPtr = (MS_U32 *)(HVD_PA2VA(u32Addr));
    MS_U32 *u32WritePtr = (MS_U32 *)(HVD_PA2VA(u32Addr+4));
    MS_U32 *u32LatchOverflow = (MS_U32 *)(HVD_PA2VA(u32Addr+8));
    MS_U32 *u32FWBinaryAddr = (MS_U32 *)(HVD_PA2VA(u32Addr+12));


    MS_U32 i;
    MS_U8 *u8ResvAddr = (MS_U8 *)((HVD_PA2VA(u32Addr))+EXT_CC_INFO_LENGTH);
    for(i=0;i< (EXT_708_CC_DATA_ALIGN- EXT_CC_INFO_LENGTH) ;i++)
    {
        u8ResvAddr[i] = 0xff;
    }



    *u32ReadPtr  = *u32WritePtr = u32Addr+EXT_708_CC_DATA_ALIGN;
    *u32LatchOverflow = 0;

    u32Addr -= pCtrl->MemMap.u32FWBinaryAddr;
    *u32FWBinaryAddr = pCtrl->MemMap.u32FWBinaryAddr;


    u32CCinfo =  ((u32Addr >> 10) << 8) & 0xffffff00; //arg[31:8] * 1kbytes
    u32CCinfo |= ((u32Len >> 10) & 0xff); //arg[7:0] *1kbytes

    eRet = (HVD_EX_Result)HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_USERDATA_MODE, u8UserDataMode);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_ENABLE_EXTERNAL_CC_708_BUF, u32CCinfo);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_ShowFirstFrameDirect()
/// @brief \b Function \b Description: Push First Frame to Display Queue Directly
/// @param -bEnable \b IN : Enable/Disable
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_ShowFirstFrameDirect(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SHOW_FIRST_FRAME_DIRECT, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_AVCResizeDosDispPendBuf()
/// @brief \b Function \b Description: Resize disp pending buffer size for display outside mode(dos),
/// @param -u32Size \b IN : size of display pending buffer
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_AVCResizeDosDispPendBuf(MS_U32 u32Id, MS_U32 u32Size)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_AVC_RESIZE_DOS_DISP_PEND_BUF, u32Size);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}
//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDmxFrameRate()
/// @brief \b Function \b Description:  set the demuxer prefered frame rate.
/// @param -eMode \b IN : mode type
/// @return -The result is successful or not
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDmxFrameRate(MS_U32 u32Id, MS_U32 u32Value)
{
    return (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DMX_FRAMERATE, u32Value);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetDmxFrameRateBase()
/// @brief \b Function \b Description:  set the demuxer prefered frame rate.
/// @param -eMode \b IN : mode type
/// @return -The result is successful or not
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetDmxFrameRateBase(MS_U32 u32Id, MS_U32 u32Value)
{
    return (HVD_EX_Result) HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_DMX_FRAMERATEBASE, u32Value);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_GetSupport2ndMVOPInterface()
/// @brief \b Function \b Description:  Get if support 2nd MVOP interface.
/// @param : none
/// @return -The result is true or false
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_GetSupport2ndMVOPInterface(void)
{
    return HAL_HVD_EX_GetSupport2ndMVOPInterface();
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetVPUSecureMode()
/// @brief \b Function \b Description:  Set secure mode for HVD Drv.
/// @param : none
/// @return -The result is true or false
//-----------------------------------------------------------------------------
MS_BOOL MDrv_HVD_EX_SetVPUSecureMode(MS_BOOL bEnable)
{
    pHVDDrvContext->bVPUIsSecureMode = bEnable;
    return TRUE;
}

HVD_EX_Result MDrv_HVD_EX_OnePendingBufferMode(MS_U32 u32Id,MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    pHVDDrvContext->gHVDPreCtrl[u8DrvId].bOnePendingBuffer = bEnable;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

HVD_EX_Result MDrv_HVD_EX_TsInBbuMode(MS_U32 u32Id,MS_BOOL bDisable)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    pHVDDrvContext->gHVDPreCtrl[u8DrvId].bDisableTspInBbuMode = bDisable;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}
HVD_EX_Result MDrv_HVD_EX_IapGnBufShareBWMode(MS_U32 u32Id,MS_BOOL bEnable, MS_U32 u32IapGnBufAddr, MS_U32 u32IapGnBufSize)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;

    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    pHVDDrvContext->gHVDPreCtrl[u8DrvId].stIapGnShBWMode.bEnable = bEnable;
    pHVDDrvContext->gHVDPreCtrl[u8DrvId].stIapGnShBWMode.u32IapGnBufAddr = u32IapGnBufAddr;
    pHVDDrvContext->gHVDPreCtrl[u8DrvId].stIapGnShBWMode.u32IapGnBufSize = u32IapGnBufSize;
    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_SetPTSUsecMode()
/// @brief \b Function \b Description: enable/disable PTS output by micro second level,
/// @param -bEnable \b IN :enable/disable
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_SetPTSUsecMode(MS_U32 u32Id, MS_BOOL bEnable)
{
    HVD_EX_Result eRet = E_HVD_EX_OK;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_SET_PTS_US_MODE, bEnable);

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_EX_PVRTimeShiftSeamlessMode()
/// @brief \b Function \b Description:
/// @param -bEnable \b IN :
/// @return -E_HVD_EX_OK for success; E_HVD_EX_FAIL for failure.
//-----------------------------------------------------------------------------
HVD_EX_Result MDrv_HVD_EX_PVRTimeShiftSeamlessMode(MS_U32 u32Id, MS_U8 u8Arg)
{
    HVD_EX_Result eRet = E_HVD_EX_RET_ILLEGAL_ACCESS;
    MS_U8 u8DrvId = _HVD_EX_GetDrvId(u32Id);

    HVD_EX_MSG_TRACE();
    _DRV_HVD_Inited(u8DrvId,eRet);
    _DRV_HVD_EX_Entry(u8DrvId);

    eRet = (HVD_EX_Result) HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PVR_SEAMLESS_TIMESHIFT, u8Arg);

    if ((u8Arg == 1) || (u8Arg == 2))  //u8Arg=1 needs about 50ms, u8Arg=2 needs about 30ms
    {
        MS_U32 u32Status = (u8Arg==1)?(E_HVD_SEAMLESS_PAUSE_DECODE):(E_HVD_SEAMLESS_RESET_HW_DONE);
        MS_U32 u32Times = 0;

        HVD_EX_MSG_DBG("TS Seamless. Arg=%d, polling status start at %ld \n", u8Arg, HVD_GetSysTime_ms());

        while (1)
        {
            if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_TS_SEAMLESS_STATUS) & u32Status)
            {
                HVD_EX_MSG_DBG("TS Seamless status change done. Arg=%d. Status=0x%lx at %ld \n", u8Arg, u32Status, HVD_GetSysTime_ms());
                break;
            }

            u32Times++;
            HVD_Delay_ms(1);

            if (u32Times > 500)
            {
                HVD_EX_MSG_ERR("Timeout!!! TS Seamless status change is not finished. Arg=%d. Status=0x%lx at %ld \n", u8Arg, u32Status, HVD_GetSysTime_ms());
                eRet = E_HVD_EX_FAIL;
                break;
            }
        }
    }

    _DRV_HVD_EX_RET(u8DrvId, eRet);
}
void MDrv_HVD_EX_BBU_Proc(MS_U32 u32Id)
{
    HAL_HVD_EX_BBU_Proc(u32Id);
}
void MDrv_HVD_EX_BBU_StopProc(MS_U32 u32Id)
{
    HAL_HVD_EX_BBU_StopProc(u32Id);
}

MS_U8 MDrv_HVD_EX_CHIP_Capability(void* pHWCap)
{
    return HAL_VPU_EX_CHIP_Capability(pHWCap);
}

#else
MS_BOOL MDrv_HVD_EX_LoadCodeInSecure(MS_U32 addr)
{
    return HAL_VPU_EX_LoadCodeInSecure(addr);
}

MS_BOOL MDrv_HVD_EX_SetLockDownRegister(void* param)
{
    if(param == NULL)
    {
        return FALSE;
    }
    return HAL_VPU_EX_SetLockDownRegister(param);
}

#endif




