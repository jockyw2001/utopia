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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "drvHVD_Common.h"

// Internal Definition
#include "drvHVD_def.h"
#include "fwHVD_if.h"
#include "halVPU_EX.h"
#include "halHVD_EX.h"
#include "regHVD_EX.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define RV_VLC_TABLE_SIZE           0x20000
/* Add for Mobile Platform by Ted Sun */
//#define HVD_DISPQ_PREFETCH_COUNT    2
#define HVD_FW_MEM_OFFSET           0x100000UL  // 1M
#define VPU_QMEM_BASE               0x20000000UL
#define HVD_HW_MAX_PIXEL (3840*2160*31000ULL) // 4kx2k@30p

#if 0
static HVD_AVC_VUI_DISP_INFO g_hvd_VUIINFO;
static MS_U8 g_hvd_nal_fill_pair[2][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0} };
static MS_U32 u32RV_VLCTableAddr = 0;   // offset from Frame buffer start address
static MS_U16 _u16DispQPtr = 0;
#endif

//---------------------------------- Mutex settings -----------------------------------------
#if HAL_HVD_ENABLE_MUTEX_PROTECT
#define _HAL_HVD_MutexCreate()                                  \
    do                                                          \
    {                                                           \
        if (s32HVDMutexID < 0)                                  \
        {                                                       \
            s32HVDMutexID = OSAL_HVD_MutexCreate((MS_U8*)(_u8HVD_Mutex)); \
        }                                                       \
    } while (0)

#define _HAL_HVD_MutexDelete()                                  \
    do                                                          \
    {                                                           \
        if (s32HVDMutexID >= 0)                                 \
        {                                                       \
            OSAL_HVD_MutexDelete(s32HVDMutexID);                \
            s32HVDMutexID = -1;                                 \
        }                                                       \
    } while (0)

#define  _HAL_HVD_Entry()                                                       \
    do                                                                          \
    {                                                                           \
        if (s32HVDMutexID >= 0)                                                 \
        {                                                                       \
            if (!OSAL_HVD_MutexObtain(s32HVDMutexID, OSAL_HVD_MUTEX_TIMEOUT))   \
            {                                                                   \
                printf("[HAL HVD][%06d] Mutex taking timeout\n", __LINE__);     \
            }                                                                   \
        }                                                                       \
    } while (0)

#define _HAL_HVD_Return(_ret_)                                  \
    do                                                          \
    {                                                           \
        if (s32HVDMutexID >= 0)                                 \
        {                                                       \
            OSAL_HVD_MutexRelease(s32HVDMutexID);               \
        }                                                       \
        return _ret_;                                           \
    } while(0)

#define _HAL_HVD_Release()                                      \
    do                                                          \
    {                                                           \
        if (s32HVDMutexID >= 0)                                 \
        {                                                       \
            OSAL_HVD_MutexRelease(s32HVDMutexID);               \
        }                                                       \
    } while (0)


#else // HAL_HVD_ENABLE_MUTEX_PROTECT

#define _HAL_HVD_MutexCreate()
#define _HAL_HVD_MutexDelete()
#define _HAL_HVD_Entry()
#define _HAL_HVD_Return(_ret)      {return _ret;}
#define _HAL_HVD_Release()

#endif // HAL_HVD_ENABLE_MUTEX_PROTECT

//------------------------------ MIU SETTINGS ----------------------------------
#define _MaskMiuReq_MVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ3_MASK, m, BIT(4))
#define _MaskMiuReq_MVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiuReq_HVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ3_MASK, m, BIT(2))
#define _MaskMiuReq_HVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ3_MASK, m, BIT(1))

#define _MaskMiu1Req_MVD_RW( m )        _HVD_WriteRegBit(MIU1_REG_RQ3_MASK, m, BIT(4))
#define _MaskMiu1Req_MVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiu1Req_HVD_RW( m )        _HVD_WriteRegBit(MIU1_REG_RQ3_MASK, m, BIT(2))
#define _MaskMiu1Req_HVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ3_MASK, m, BIT(1))

#define HVD_MVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(4)) == BIT(4))
#define HVD_MVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)) == BIT(12))
#define HVD_HVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(2)) == BIT(2))
#define HVD_HVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL3) & BIT(1)) == BIT(1))

#if SUPPORT_EVD
#define _MaskMiuReq_EVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(2))
#define _MaskMiuReq_EVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(3))
#define _MaskMiu1Req_EVD_RW( m )         _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(2))
#define _MaskMiu1Req_EVD_BBU_R( m )      _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(3))
#define HVD_EVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(2)) == BIT(2))
#define HVD_EVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(3)) == BIT(3))
#endif

#define _HVD_MIU_SetReqMask(miu_clients, mask)  \
    do                                          \
    {                                           \
        if (HVD_##miu_clients##_ON_MIU1 == 0)   \
        {                                       \
            _MaskMiuReq_##miu_clients(mask);    \
        }                                       \
        else                                    \
        {                                       \
            _MaskMiu1Req_##miu_clients(mask);   \
        }                                       \
    } while (0)

// check RM is supported or not
#define HVD_HW_RUBBER3      (HAL_HVD_EX_GetHWVersionID()& BIT(14))
#define HAL_HVD_EX_MAX_SUPPORT_STREAM   3
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions Prototype
//-------------------------------------------------------------------------------------------------
static MS_U16       _HVD_EX_GetBBUReadptr(MS_U32 u32Id);
static void         _HVD_EX_SetBBUWriteptr(MS_U32 u32Id, MS_U16 u16BBUNewWptr);
static MS_BOOL      _HVD_EX_MBoxSend(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 u32Msg);
static MS_BOOL      _HVD_EX_MBoxReady(MS_U32 u32Id, MS_U8 u8MBox);
static MS_BOOL      _HVD_EX_MBoxRead(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 *u32Msg);
//static void     _HVD_EX_MBoxClear(MS_U8 u8MBox);
static MS_U32       _HVD_EX_GetPC(void);
static MS_U32       _HVD_EX_GetESWritePtr(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetESReadPtr(MS_U32 u32Id, MS_BOOL bDbug);
static MS_BOOL      _HVD_EX_SetCMDArg(MS_U32 u32Id, MS_U32 u32Arg);
static MS_BOOL      _HVD_EX_SetCMD(MS_U32 u32Id, MS_U32 u32Cmd);
static HVD_Return   _HVD_EX_SendCmd(MS_U32 u32Id, MS_U32 u32Cmd, MS_U32 u32CmdArg);
static void         _HVD_EX_SetMIUProtectMask(MS_BOOL bEnable);
static void         _HVD_EX_SetBufferAddr(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetESLevel(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetESQuantity(MS_U32 u32Id);
static HVD_Return   _HVD_EX_UpdatePTSTable(MS_U32 u32Id, HVD_BBU_Info *pInfo);
static HVD_Return   _HVD_EX_UpdateESWptr(MS_U32 u32Id, MS_U32 u32NalOffset, MS_U32 u32NalLen);
static HVD_Return   _HVD_EX_UpdateESWptr_VP8(MS_U32 u32Id, MS_U32 u32NalOffset, MS_U32 u32NalLen, MS_U32 u32NalOffset2, MS_U32 u32NalLen2);
static MS_U32       _HVD_EX_GetVUIDispInfo(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetBBUQNumb(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetPTSQNumb(MS_U32 u32Id);
static HVD_EX_Drv_Ctrl *_HVD_EX_GetDrvCtrl(MS_U32 u32Id);
static MS_U8        _HVD_EX_GetStreamIdx(MS_U32 u32Id);
static HVD_Frm_Information *_HVD_EX_GetNextDispFrame(MS_U32 u32Id);
#if SUPPORT_EVD
static void HAL_EVD_EX_PowerCtrl(MS_BOOL bEnable);
static MS_BOOL HAL_EVD_EX_DeinitHW(void);
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if defined (__aeon__)
static MS_U32 u32HVDRegOSBase = 0xA0200000;
#else
static MS_U32 u32HVDRegOSBase = 0xBF200000;
#endif
#if HAL_HVD_ENABLE_MUTEX_PROTECT
MS_S32 s32HVDMutexID = -1;
MS_U8 _u8HVD_Mutex[] = { "HVD_Mutex" };
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
typedef struct
{

    HVD_AVC_VUI_DISP_INFO g_hvd_VUIINFO;
    MS_U8 g_hvd_nal_fill_pair[2][8];
    MS_U32 u32RV_VLCTableAddr;  // offset from Frame buffer start address
    MS_U16 _u16DispQPtr;

    //HVD_EX_Drv_Ctrl *_pHVDCtrls;
    MS_U32 u32HVDCmdTimeout;//same as HVD_FW_CMD_TIMEOUT_DEFAULT
    MS_U32 u32VPUClockType;
    MS_U32 u32HVDClockType;//160
#if SUPPORT_EVD
    MS_U32 u32EVDClockType;
#endif
    HVD_EX_Stream _stHVDStream[3];

    volatile HVD_Frm_Information *pHvdFrm;//_HVD_EX_GetNextDispFrame()
    MS_BOOL g_RstFlag;
    MS_U64 u64pts_real;
    MS_U32 u32VP8BBUWptr;
    //pre_set
    HVD_Pre_Ctrl *pHVDPreCtrl_Hal[2];

} HVD_Hal_CTX;

HVD_Hal_CTX* pHVDHalContext = NULL;
HVD_Hal_CTX gHVDHalContext;
HVD_EX_Drv_Ctrl *_pHVDCtrls = NULL;

static HVD_EX_PreSet _stHVDPreSet[HAL_HVD_EX_MAX_SUPPORT_STREAM] =
{
    {FALSE},
    {FALSE},
    {FALSE},
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
void HVD_EX_SetRstFlag(MS_BOOL bRst)
{
    pHVDHalContext->g_RstFlag = bRst;
}
MS_BOOL HVD_EX_GetRstFlag(void)
{
    return pHVDHalContext->g_RstFlag;
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _HVD_EX_Context_Init_HAL(void)
{
    pHVDHalContext->u32HVDCmdTimeout = 100;//same as HVD_FW_CMD_TIMEOUT_DEFAULT
    pHVDHalContext->u32VPUClockType = 240; //it should same as:_VPU_EX_InitAll() eClkSpeed
    pHVDHalContext->u32HVDClockType = 240;//160;
#if SUPPORT_EVD
    pHVDHalContext->u32EVDClockType = 384;
#endif
    pHVDHalContext->_stHVDStream[0].eStreamId = E_HAL_HVD_MAIN_STREAM0;
    pHVDHalContext->_stHVDStream[1].eStreamId = E_HAL_HVD_SUB_STREAM0;
    pHVDHalContext->_stHVDStream[2].eStreamId = E_HAL_HVD_SUB_STREAM1;
}

static MS_U16 _HVD_EX_GetBBUReadptr(MS_U32 u32Id)
{
    MS_U16 u16Ret = 0;
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

#if HVD_ENABLE_MVC
    if(HAL_HVD_EX_CheckMVCID(u32Id))
    {
        u8TaskId = (MS_U8) HAL_HVD_EX_GetView(u32Id);
    }
#endif /// HVD_ENABLE_MVC

    _HVD_WriteWordMask(HVD_REG_POLL_NAL_RPTR(u32RB), 0, HVD_REG_POLL_NAL_RPTR_BIT);
    _HVD_WriteWordMask(HVD_REG_POLL_NAL_RPTR(u32RB), HVD_REG_POLL_NAL_RPTR_BIT, HVD_REG_POLL_NAL_RPTR_BIT);

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))        // VP8
    {
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS4);
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS3);
    }
    else
#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        //if(pCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            u16Ret = pShm->u32ColocateBBUReadPtr;
        else
            u16Ret = _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI(u32RB));
    }
    else
    {
        //if(pCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            u16Ret = pShm->u32ColocateBBUReadPtr;
        else
            u16Ret = _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS2);
    }

    HVD_EX_MSG_DBG("Task0=%d, Task1=%d\n",
        _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI(u32RB)), _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS2));

    return u16Ret;
}

static MS_U16 _HVD_EX_GetBBUWritedptr(MS_U32 u32Id)
{
    MS_U16 u16Ret = 0;
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    HVD_EX_Drv_Ctrl *pDrvCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

#if HVD_ENABLE_MVC
    if (HAL_HVD_EX_CheckMVCID(u32Id))
    {
        u8TaskId = (MS_U8) HAL_HVD_EX_GetView(u32Id);
    }
#endif /// HVD_ENABLE_MVC
    _HVD_WriteWordMask(HVD_REG_POLL_NAL_RPTR(u32RB), 0, HVD_REG_POLL_NAL_RPTR_BIT);
    _HVD_WriteWordMask(HVD_REG_POLL_NAL_RPTR(u32RB), HVD_REG_POLL_NAL_RPTR_BIT, HVD_REG_POLL_NAL_RPTR_BIT);

    if ((pDrvCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP8)        // VP8
    {
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS4);
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS3);
    }
    else
#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pDrvCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        //if(pDrvCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            u16Ret = pShm->u32ColocateBBUWritePtr;
        else
            u16Ret = _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI(u32RB));
    }
    else
    {
        //if(pDrvCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            u16Ret = pShm->u32ColocateBBUWritePtr;
        else
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS2);
    }

    return u16Ret;
}

static void _HVD_EX_ResetMainSubBBUWptr(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI(u32RB), 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS2, 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS3, 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_RPTR_HI_BS4, 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
}

static void _HVD_EX_SetBBUWriteptr(MS_U32 u32Id, MS_U16 u16BBUNewWptr)
{
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

#if HVD_ENABLE_MVC
    if (HAL_HVD_EX_CheckMVCID(u32Id))
    {
        u8TaskId = (MS_U8) HAL_HVD_EX_GetView(u32Id);
    }
#endif /// HVD_ENABLE_MVC

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))        // VP8
    {
        _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS3, u16BBUNewWptr);
        _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS4, u16BBUNewWptr);
    }
    else
#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI(u32RB), u16BBUNewWptr);
        //if(pCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            pShm->u32ColocateBBUWritePtr = u16BBUNewWptr;
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS2, u16BBUNewWptr);
        //if(pCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            pShm->u32ColocateBBUWritePtr = u16BBUNewWptr;
    }

    HVD_EX_MSG_DBG("Task0=%d, Task1=%d\n",
        _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI(u32RB)), _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS2));

    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
}

static MS_BOOL _HVD_EX_MBoxSend(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    switch (u8MBox)
    {
        case E_HVD_HI_0:
        {
            _HVD_Write4Byte(HVD_REG_HI_MBOX0_L(u32RB), u32Msg);
            _HVD_WriteWordMask(HVD_REG_HI_MBOX_SET(u32RB), HVD_REG_HI_MBOX0_SET, HVD_REG_HI_MBOX0_SET);
            break;
        }
        case E_HVD_HI_1:
        {
            _HVD_Write4Byte(HVD_REG_HI_MBOX1_L(u32RB), u32Msg);
            _HVD_WriteWordMask(HVD_REG_HI_MBOX_SET(u32RB), HVD_REG_HI_MBOX1_SET, HVD_REG_HI_MBOX1_SET);
            break;
        }
        case E_HVD_VPU_HI_0:
        {
            bResult = HAL_VPU_EX_MBoxSend(VPU_HI_MBOX0, u32Msg);
            break;
        }
        case E_HVD_VPU_HI_1:
        {
            bResult = HAL_VPU_EX_MBoxSend(VPU_HI_MBOX1, u32Msg);
            break;
        }
        default:
        {
            bResult = FALSE;
            break;
        }
    }

    return bResult;
}

static MS_BOOL _HVD_EX_MBoxReady(MS_U32 u32Id, MS_U8 u8MBox)
{
    MS_BOOL bResult = TRUE;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    switch (u8MBox)
    {
        case E_HVD_HI_0:
            bResult = _HVD_ReadWordBit(HVD_REG_HI_MBOX_RDY(u32RB), HVD_REG_HI_MBOX0_RDY) ? FALSE : TRUE;
            break;
        case E_HVD_HI_1:
            bResult = _HVD_ReadWordBit(HVD_REG_HI_MBOX_RDY(u32RB), HVD_REG_HI_MBOX1_RDY) ? FALSE : TRUE;
            break;
        case E_HVD_RISC_0:
            bResult = _HVD_ReadWordBit(HVD_REG_RISC_MBOX_RDY(u32RB), HVD_REG_RISC_MBOX0_RDY) ? TRUE : FALSE;
            break;
        case E_HVD_RISC_1:
            bResult = _HVD_ReadWordBit(HVD_REG_RISC_MBOX_RDY(u32RB), HVD_REG_RISC_MBOX1_RDY) ? TRUE : FALSE;
            break;
        case E_HVD_VPU_HI_0:
            bResult = HAL_VPU_EX_MBoxRdy(VPU_HI_MBOX0);
            break;
        case E_HVD_VPU_HI_1:
            bResult = HAL_VPU_EX_MBoxRdy(VPU_HI_MBOX1);
            break;
        case E_HVD_VPU_RISC_0:
            bResult = HAL_VPU_EX_MBoxRdy(VPU_RISC_MBOX0);
            break;
        case E_HVD_VPU_RISC_1:
            bResult = HAL_VPU_EX_MBoxRdy(VPU_RISC_MBOX1);
            break;
        default:
            break;
    }

    return bResult;
}

static MS_BOOL _HVD_EX_MBoxRead(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    switch (u8MBox)
    {
        case E_HVD_HI_0:
        {
            *u32Msg = _HVD_Read4Byte(HVD_REG_HI_MBOX0_L(u32RB));
            break;
        }
        case E_HVD_HI_1:
        {
            *u32Msg = _HVD_Read4Byte(HVD_REG_HI_MBOX1_L(u32RB));
            break;
        }
        case E_HVD_RISC_0:
        {
            *u32Msg = _HVD_Read4Byte(HVD_REG_RISC_MBOX0_L(u32RB));
            break;
        }
        case E_HVD_RISC_1:
        {
            *u32Msg = _HVD_Read4Byte(HVD_REG_RISC_MBOX1_L(u32RB));
            break;
        }
        case E_HVD_VPU_RISC_0:
        {
            bResult = HAL_VPU_EX_MBoxRead(VPU_RISC_MBOX0, u32Msg);
            break;
        }
        case E_HVD_VPU_RISC_1:
        {
            bResult = HAL_VPU_EX_MBoxRead(VPU_RISC_MBOX1, u32Msg);
            break;
        }
        default:
        {
            bResult = FALSE;
            break;
        }
    }

    return bResult;
}

#if 0
static void _HVD_EX_MBoxClear(MS_U8 u8MBox)
{
    switch (u8MBox)
    {
        case E_HVD_RISC_0:
            _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_MBOX0_CLR, HVD_REG_RISC_MBOX0_CLR);
            break;
        case E_HVD_RISC_1:
            _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR, HVD_REG_RISC_MBOX1_CLR, HVD_REG_RISC_MBOX1_CLR);
            break;
        case E_HVD_VPU_RISC_0:
            HAL_VPU_EX_MBoxClear(VPU_RISC_MBOX0);
            break;
        case E_HVD_VPU_RISC_1:
            HAL_VPU_EX_MBoxClear(VPU_RISC_MBOX1);
            break;
        default:
            break;
    }
}
#endif

static MS_U32 _HVD_EX_GetPC(void)
{
    MS_U32 u32PC = 0;
    u32PC = HAL_VPU_EX_GetProgCnt();
//    HVD_MSG_DBG("<gdbg>pc0 =0x%lx\n",u32PC);
    return u32PC;
}

static MS_U32 _HVD_EX_GetESWritePtr(MS_U32 u32Id)
{
    MS_U32 u32Data = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        u32Data = pCtrl->LastNal.u32NalAddr + pCtrl->LastNal.u32NalSize;

        if (u32Data > pCtrl->MemMap.u32BitstreamBufSize)
        {
            u32Data -= pCtrl->MemMap.u32BitstreamBufSize;

            HVD_EX_MSG_ERR("app should not put this kind of packet\n");
        }
    }
    else
    {
#if HVD_ENABLE_MVC
        MS_U8 u8ViewIdx = 0;
        if(HAL_HVD_EX_CheckMVCID(u32Id))
        {
            u8ViewIdx = (MS_U8) HAL_HVD_EX_GetView(u32Id);
        }
        if(u8ViewIdx != 0)  /// 2nd ES ptr.
        {
            u32Data = pShm->u32ES2WritePtr;
        }
        else
        {
            u32Data = pShm->u32ESWritePtr;
        }
#else
            u32Data = pShm->u32ESWritePtr;
#endif
    }

    return u32Data;
}

#define NAL_UNIT_LEN_BITS   21
#define NAL_UNIT_OFT_BITS   30
#define NAL_UNIT_OFT_LOW_BITS (32-NAL_UNIT_LEN_BITS)
#define NAL_UNIT_OFT_HIGH_BITS (NAL_UNIT_OFT_BITS-NAL_UNIT_OFT_LOW_BITS)
#define NAL_UNIT_OFT_LOW_MASK (((unsigned int)0xFFFFFFFF)>>(32-NAL_UNIT_OFT_LOW_BITS))

static MS_U32 _HVD_EX_GetESReadPtr(MS_U32 u32Id, MS_BOOL bDbug)
{
    MS_U32 u32Data = 0;
    MS_U8 u8TaskId = 0;
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    MS_U32 u32VP8_BBU_DRAM_ST_ADDR_BS3 = pShm->u32HVD_BBU_DRAM_ST_ADDR;

    u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#if HVD_ENABLE_MVC
    if(HAL_HVD_EX_CheckMVCID(u32Id))
    {
        u8TaskId = (MS_U8) HAL_HVD_EX_GetView(u32Id);
    }
#endif /// HVD_ENABLE_MVC

    if (((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV) || (TRUE == bDbug))
    {
        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP8)
        {
           // MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
            MS_U16 u16ReadPtr = _HVD_EX_GetBBUReadptr(u32Id);
            MS_U16 u16WritePtr = _HVD_EX_GetBBUWritedptr(u32Id);
            MS_U32 *u32Adr;
            MS_U32 u32Tmp;

            if (u16ReadPtr == u16WritePtr)
            {
                u32Data = _HVD_EX_GetESWritePtr(u32Id);
            }
            else
            {
                if (u16ReadPtr)
                    u16ReadPtr--;
                else
                    u16ReadPtr = VP8_BBU_DRAM_TBL_ENTRY - 1;

                u32Adr = (MS_U32 *)(MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32VP8_BBU_DRAM_ST_ADDR_BS3 + (u16ReadPtr << 3)));

                u32Data = (*u32Adr) >> NAL_UNIT_LEN_BITS;
                u32Tmp = (*(u32Adr+1)) & (0xffffffff>>(32-(NAL_UNIT_OFT_BITS-(32-NAL_UNIT_LEN_BITS))));
                u32Tmp = u32Tmp << (32-NAL_UNIT_LEN_BITS);
                u32Data = u32Data | u32Tmp;

                //printf("[VP8] GetESRptr (%x,%x,%x,%x,%d,%d)\n", u32Adr, (*u32Adr), (*(u32Adr+1)) , u32Data, u16ReadPtr, u16WritePtr);
                //while(1);
            }
            goto EXIT;
        }
        // set reg_poll_nal_rptr 0
        _HVD_WriteWordMask(HVD_REG_ESB_RPTR(u32RB), 0, HVD_REG_ESB_RPTR_POLL);
        // set reg_poll_nal_rptr 1
        _HVD_WriteWordMask(HVD_REG_ESB_RPTR(u32RB), HVD_REG_ESB_RPTR_POLL, HVD_REG_ESB_RPTR_POLL);

        // read reg_nal_rptr_hi
#ifdef SUPPORT_EVD
        if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
        if (0 == u8TaskId)
#endif
        {
            u32Data = _HVD_Read2Byte(HVD_REG_ESB_RPTR(u32RB)) & 0xFFC0;
            u32Data >>= 6;
            u32Data |= _HVD_Read2Byte(HVD_REG_ESB_RPTR_H(u32RB)) << 10;
        }
        else
        {
            u32Data = _HVD_Read2Byte(HVD_REG_ESB_RPTR_L_BS2) & 0xFFC0;
            u32Data >>= 6;
            u32Data |= _HVD_Read2Byte(HVD_REG_ESB_RPTR_H_BS2) << 10;
        }

        u32Data <<= 3;             // unit

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
        {
            MS_U32 u32ESWptr = _HVD_EX_GetESWritePtr(u32Id);

            if ((pCtrl->u32LastESRptr < u32ESWptr) && (u32Data > u32ESWptr))
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32Data , pCtrl->u32LastESRptr, u32ESWptr  );
                u32Data = u32ESWptr;
            }
            else if ((pCtrl->u32LastESRptr == u32ESWptr) && (u32Data > u32ESWptr))
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32Data , pCtrl->u32LastESRptr, u32ESWptr  );
                u32Data = u32ESWptr;
            }
            else if ((_HVD_EX_GetBBUQNumb(u32Id) == 0) && ((u32Data - u32ESWptr) < 32)
                     && ((pShm->u32FwState & E_HVD_FW_STATE_MASK) == E_HVD_FW_PLAY))
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32Data , pCtrl->u32LastESRptr, u32ESWptr  );
                u32Data = u32ESWptr;
            }
            else if (((u32Data > u32ESWptr) && (pCtrl->u32LastESRptr > u32Data))
                && ((u32Data - u32ESWptr) < 32)
                && (pCtrl->u32FlushRstPtr == 1))
            {
                //HVD_MSG_INFO("444HVD Warn: ESRptr(%lx %lx) is running over ESWptr(%lx)\n" ,  u32Data , pCtrl->u32LastESRptr, u32ESWptr  );
                u32Data = u32ESWptr;
            }
        }

        // remove illegal pointer
#if 1
        if ((pCtrl->MemMap.u32DrvProcessBufSize != 0) && (pCtrl->MemMap.u32DrvProcessBufAddr != 0))
        {
            MS_U32 u32PacketStaddr = u32Data + pCtrl->MemMap.u32BitstreamBufAddr;

            if (((pCtrl->MemMap.u32DrvProcessBufAddr <= u32PacketStaddr) &&
                 (u32PacketStaddr <
                  (pCtrl->MemMap.u32DrvProcessBufAddr + pCtrl->MemMap.u32DrvProcessBufSize))))
            {
                //HVD_MSG_INFO("HVD Warn: ESRptr(%lx %lx) is located in drv process buffer(%lx %lx)\n" ,  u32Data , pCtrl->u32LastESRptr,  pCtrl->MemMap.u32DrvProcessBufAddr  ,   pCtrl->MemMap.u32DrvProcessBufSize  );
                u32Data = pCtrl->u32LastESRptr;
            }
        }
#endif
    }
    else
    {
#if HVD_ENABLE_MVC
        MS_U8 u8ViewIdx = 0;
        if(HAL_HVD_EX_CheckMVCID(u32Id))
        {
            u8ViewIdx = (MS_U8) HAL_HVD_EX_GetView(u32Id);
        }
        if(u8ViewIdx != 0)  /// 2nd ES ptr.
        {
            u32Data = pShm->u32ES2ReadPtr;
        }
        else
        {
            u32Data = pShm->u32ESReadPtr;
        }
#else
            u32Data = pShm->u32ESReadPtr;
#endif
    }

    EXIT:

    pCtrl->u32LastESRptr = u32Data;

    return u32Data;
}

static MS_BOOL _HVD_EX_SetCMDArg(MS_U32 u32Id, MS_U32 u32Arg)
{
    MS_U16 u16TimeOut = 0xFFFF;
    MS_BOOL bResult = FALSE;

    HVD_EX_MSG_DBG("Send ARG 0x%lx to HVD\n", u32Arg);

    while (--u16TimeOut)
    {
        if (_HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_MBOX) && _HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_ARG_MBOX))
        {
            bResult = _HVD_EX_MBoxSend(u32Id, HAL_HVD_CMD_ARG_MBOX, u32Arg);
            break;
        }
    }

    return bResult;
}

static MS_BOOL _HVD_EX_SetCMD(MS_U32 u32Id, MS_U32 u32Cmd)
{
    MS_U16 u16TimeOut = 0xFFFF;
    MS_BOOL bResult = FALSE;
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);

    HVD_EX_MSG_DBG("Send CMD 0x%lx to HVD \n", u32Cmd);

#if HVD_ENABLE_MVC
    if(E_HAL_VPU_MVC_STREAM_BASE == u8TaskId)
    {
        u8TaskId = E_HAL_VPU_MAIN_STREAM_BASE;
    }
#endif /// HVD_ENABLE_MVC

    while (--u16TimeOut)
    {
        if (_HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_MBOX))
        {
            u32Cmd |= (u8TaskId << 24);
            bResult = _HVD_EX_MBoxSend(u32Id, HAL_HVD_CMD_MBOX, u32Cmd);
            break;
        }
    }
    return bResult;
}

static HVD_Return _HVD_EX_SendCmd(MS_U32 u32Id, MS_U32 u32Cmd, MS_U32 u32CmdArg)
{
    MS_U32 u32timeout = HVD_GetSysTime_ms() + pHVDHalContext->u32HVDCmdTimeout;

    while (!_HVD_EX_SetCMDArg(u32Id, u32CmdArg))
    {
        if (HVD_GetSysTime_ms() > u32timeout)
        {
            HVD_EX_MSG_ERR("Timeout: cmd=0x%lx arg=0x%lx\n", u32Cmd, u32CmdArg);
            return E_HVD_RETURN_TIMEOUT;
        }

#if 0
        if (u32Cmd == E_HVD_CMD_STOP)
        {
            MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#if HVD_ENABLE_MVC
            if(E_HAL_VPU_MVC_STREAM_BASE == u8TaskId)
            {
                u8TaskId = E_HAL_VPU_MAIN_STREAM_BASE;
            }
#endif /// HVD_ENABLE_MVC
            MS_U32 u32Cmdtmp = (u8TaskId << 24) | E_HVD_CMD_STOP;

            _HVD_EX_MBoxSend(u32Id, HAL_HVD_CMD_MBOX, u32Cmdtmp);
            _HVD_EX_MBoxSend(u32Id, HAL_HVD_CMD_ARG_MBOX, 0);

            return E_HVD_RETURN_SUCCESS;
        }
#endif

        if(u32Cmd < E_DUAL_CMD_BASE)
        {
            //_HVD_EX_GetPC();
            HAL_HVD_EX_Dump_FW_Status(u32Id);
            HAL_HVD_EX_Dump_HW_Status(HVD_U32_MAX);
        }
    }

    u32timeout = HVD_GetSysTime_ms() + pHVDHalContext->u32HVDCmdTimeout;

    while (!_HVD_EX_SetCMD(u32Id, u32Cmd))
    {
        if (HVD_GetSysTime_ms() > u32timeout)
        {
            HVD_EX_MSG_ERR("cmd timeout: %lx\n", u32Cmd);
            return E_HVD_RETURN_TIMEOUT;
        }

        if(u32Cmd < E_DUAL_CMD_BASE)
        {
            //_HVD_EX_GetPC();
            HAL_HVD_EX_Dump_FW_Status(u32Id);
            HAL_HVD_EX_Dump_HW_Status(HVD_U32_MAX);
        }
    }

    return E_HVD_RETURN_SUCCESS;
}

static void _HVD_EX_SetMIUProtectMask(MS_BOOL bEnable)
{
#if HAL_HVD_ENABLE_MIU_PROTECT
    _HVD_MIU_SetReqMask(MVD_RW, bEnable);
    _HVD_MIU_SetReqMask(MVD_BBU_R, bEnable);
#if SUPPORT_EVD
    _HVD_MIU_SetReqMask(EVD_RW, bEnable);
    _HVD_MIU_SetReqMask(EVD_BBU_R, bEnable);
#endif
    _HVD_MIU_SetReqMask(HVD_RW, bEnable);
    _HVD_MIU_SetReqMask(HVD_BBU_R, bEnable);
    HAL_VPU_EX_MIU_RW_Protect(bEnable);
    //HVD_Delay_ms(1);
#endif
    return;
}

static void _HVD_EX_SetBufferAddr(MS_U32 u32Id)
{
    MS_U16 u16Reg = 0;
    MS_U32 u32StAddr = 0;
    MS_BOOL bBitMIU1 = FALSE;
    MS_BOOL bCodeMIU1 = FALSE;
    MS_U8 u8TaskId = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if(pCtrl == NULL) return;

    // nal table settngs
    if (pCtrl->MemMap.u32CodeBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        bCodeMIU1 = TRUE;
    }

    if (pCtrl->MemMap.u32BitstreamBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        bBitMIU1 = TRUE;
    }

    if (bBitMIU1 != bCodeMIU1)
    {
        u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->u32BBUTblInBitstreamBufAddr;

        if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }
    }
    else
    {
        u32StAddr = pCtrl->MemMap.u32CodeBufAddr + pShm->u32HVD_BBU_DRAM_ST_ADDR;

        if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }
    }

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_Write2Byte(HVD_REG_HK_VP8, HVD_REG_HK_PLAYER_FM);

        _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_L_BS3, (MS_U16)(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_H_BS3, (MS_U16)(u32StAddr >> 19));
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS3, (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));

        u32StAddr += 0x2000;

        _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_L_BS4, (MS_U16)(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_H_BS4, (MS_U16)(u32StAddr >> 19));
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS4, (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));

        // ES buffer
        u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr;

        if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }

        HVD_EX_MSG_DBG("ESB start addr=%lx\n", u32StAddr);

        _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_L_BS34, HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_H_BS34, HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS34, HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS34, HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));

        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BS34);
        u16Reg &= ~HVD_REG_BS34_TSP_INPUT;
        u16Reg &= ~HVD_REG_BS34_PASER_MASK;
        u16Reg |= HVD_REG_BS34_PASER_DISABLE;
        u16Reg |= HVD_REG_BS34_AUTO_NAL_TAB;
        _HVD_Write2Byte(HVD_REG_MIF_BS34, u16Reg);

        return;
    }

    u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);

    HVD_EX_MSG_DBG("NAL start addr=%lx\n", u32StAddr);

#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L(u32RB), (MS_U16) (u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H(u32RB), (MS_U16) (u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN(u32RB), (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L_BS2, (MS_U16) (u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H_BS2, (MS_U16) (u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS2, (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
    }

    // ES buffer
    u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr;

    if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }

    HVD_EX_MSG_DBG("ESB start addr=%lx, len=%lx\n", u32StAddr, pCtrl->MemMap.u32BitstreamBufSize);

#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L(u32RB), HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H(u32RB), HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2, HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2, HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2, HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2, HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }

    // others
#ifdef SUPPORT_EVD
    if ((0 == u8TaskId) || (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
#else
    if (0 == u8TaskId)
#endif
    {
        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU(u32RB));

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
        {
            u16Reg |= HVD_REG_BBU_TSP_INPUT;
        }
        else
        {
            u16Reg &= ~HVD_REG_BBU_TSP_INPUT;
        }

        u16Reg &= ~HVD_REG_BBU_PASER_MASK;

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)        // RM
        {
            u16Reg |= HVD_REG_BBU_PASER_DISABLE;    // force BBU to remove nothing, RM only
        }
        else                        // AVS or AVC
        {
            if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_START_CODE_MASK) == E_HVD_INIT_START_CODE_REMOVED)
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_03;
            }
            else                    // start code remained
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_ALL;
            }
        }

        u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB;

        _HVD_Write2Byte(HVD_REG_MIF_BBU(u32RB), u16Reg);
    }
    else
    {
        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2);

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
        {
            u16Reg |= HVD_REG_BBU_TSP_INPUT_BS2;
        }
        else
        {
            u16Reg &= ~HVD_REG_BBU_TSP_INPUT_BS2;
        }

        u16Reg &= ~HVD_REG_BBU_PASER_MASK_BS2;

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)        // RM
        {
            u16Reg |= HVD_REG_BBU_PASER_DISABLE_BS2;    // force BBU to remove nothing, RM only
        }
        else                        // AVS or AVC
        {
            if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_START_CODE_MASK) == E_HVD_INIT_START_CODE_REMOVED)
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_03_BS2;
            }
            else                    // start code remained
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_ALL_BS2;
            }
        }

        u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB_BS2;

        _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2, u16Reg);
    }

#if (HVD_ENABLE_MVC)
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC)
    {
        /// Used sub stream to record sub view data.
        HVD_EX_Drv_Ctrl *pDrvCtrl_Sub = _HVD_EX_GetDrvCtrl((u32Id+0x00011000));
        //printf("**************** Buffer setting for MVC dual-BBU *************\n");

        if (bBitMIU1 != bCodeMIU1)
        {
            u32StAddr = pDrvCtrl_Sub->MemMap.u32BitstreamBufAddr + pDrvCtrl_Sub->u32BBUTblInBitstreamBufAddr;

            if (u32StAddr >= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr)
            {
                u32StAddr -= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr;
            }
        }
        else
        {
            u32StAddr = pDrvCtrl_Sub->MemMap.u32CodeBufAddr + pShm->u32HVD_BBU2_DRAM_ST_ADDR;

            if (u32StAddr >= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr)
            {
                u32StAddr -= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr;
            }
        }


        HVD_EX_MSG_DBG("[MVC] _HAL_HVD_SetBuffer2Addr: nal StAddr:%lx \n", u32StAddr);
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L_BS2, (MS_U16)(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H_BS2, (MS_U16)(u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS2, (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUEntryNum - 1));

        // ES buffer
        u32StAddr = pDrvCtrl_Sub->MemMap.u32BitstreamBufAddr;
        if(u32StAddr >= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pDrvCtrl_Sub->MemMap.u32MIU1BaseAddr;
        }

        HVD_EX_MSG_DBG("[MVC] 2nd ES _HAL_HVD_SetBuffer2Addr: ESb StAddr:%lx, len:%lx.\n", u32StAddr, pDrvCtrl_Sub->MemMap.u32BitstreamBufSize);
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2, HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2, HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2, HVD_LWORD(pDrvCtrl_Sub->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2, HVD_HWORD(pDrvCtrl_Sub->MemMap.u32BitstreamBufSize >> 3));

        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2);
        if((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
        {
            u16Reg |= HVD_REG_BBU_TSP_INPUT_BS2;
            HVD_EX_MSG_DBG("[MVC] 2nd ES, TSP mode.\n");
        }
        else
        {
            u16Reg &= ~HVD_REG_BBU_TSP_INPUT_BS2;
            HVD_EX_MSG_DBG("[MVC] 2nd ES, BBU mode.\n");
        }
        u16Reg &= ~HVD_REG_BBU_PASER_MASK_BS2;
        if((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)   // RM
        {
            u16Reg |= HVD_REG_BBU_PASER_DISABLE_BS2;   // force BBU to remove nothing, RM only
        }
        else    // AVS or AVC
        {
            if((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_START_CODE_MASK) == E_HVD_INIT_START_CODE_REMOVED)
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_03_BS2;
            }
            else    // start code remained
            {
                u16Reg |= HVD_REG_BBU_PASER_ENABLE_ALL_BS2;
                ///HVD_MSG_DBG("[MVC] BBU Paser all.\n");
            }
        }
        u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB_BS2;
        ///HVD_MSG_DBG("[MVC] 2nd MIF BBU 0x%lx.\n",(MS_U32)u16Reg);
        _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2, u16Reg);
    }
#endif


    // MIF offset
#if 0
    {
        MS_U16 offaddr = 0;
        u32StAddr = pCtrl->MemMap.u32CodeBufAddr;
        if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }
        HVD_EX_MSG_DBG("MIF offset:%lx \n", u32StAddr);
        offaddr = (MS_U16) ((u32StAddr) >> 20);
      offaddr &= BMASK(HVD_REG_MIF_OFFSET_L_BITS:0);
                                //0x1FF;   // 9 bits(L + H)
        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU);
        u16Reg &= ~HVD_REG_MIF_OFFSET_H;
      u16Reg &= ~(BMASK(HVD_REG_MIF_OFFSET_L_BITS:0));
        if (offaddr & BIT(HVD_REG_MIF_OFFSET_L_BITS))
        {
            u16Reg |= HVD_REG_MIF_OFFSET_H;
        }
      _HVD_Write2Byte(HVD_REG_MIF_BBU, (u16Reg | (offaddr & BMASK(HVD_REG_MIF_OFFSET_L_BITS:0))));
    }
#endif
}

#if 0 //defined(SUPPORT_NEW_MEM_LAYOUT) || defined(SUPPORT_NEW_VDEC_FLOW)
// Note: For VP8 only. MVC ES buffer address will be set when _HVD_EX_SetBufferAddr() is called
static void _HVD_EX_SetESBufferAddr(MS_U32 u32Id)
{
    MS_U16 u16Reg = 0;
    MS_U32 u32StAddr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    if(pCtrl == NULL) return;

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_Write2Byte(HVD_REG_HK_VP8, HVD_REG_HK_PLAYER_FM);

        // ES buffer
        u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr;

        if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }

        _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_L_BS34, HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_H_BS34, HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS34, HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS34, HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));

        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BS34);
        u16Reg &= ~HVD_REG_BS34_TSP_INPUT;
        u16Reg &= ~HVD_REG_BS34_PASER_MASK;
        u16Reg |= HVD_REG_BS34_PASER_DISABLE;
        u16Reg |= HVD_REG_BS34_AUTO_NAL_TAB;
        _HVD_Write2Byte(HVD_REG_MIF_BS34, u16Reg);

        return;
    }

    // ES buffer
    u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr;

    if (u32StAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        u32StAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }

    HVD_EX_MSG_DBG("ESB start addr=%lx, len=%lx\n", u32StAddr, pCtrl->MemMap.u32BitstreamBufSize);

    if (0 == HAL_VPU_EX_GetTaskId(u32Id))
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L(u32RB), HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H(u32RB), HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2, HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2, HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2, HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2, HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }
}
#endif

static MS_U32 _HVD_EX_GetESLevel(MS_U32 u32Id)
{
    MS_U32 u32Wptr = 0;
    MS_U32 u32Rptr = 0;
    MS_U32 u32CurMBX = 0;
    MS_U32 u32ESsize = 0;
    MS_U32 u32Ret = E_HVD_ESB_LEVEL_NORMAL;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    u32Wptr = _HVD_EX_GetESWritePtr(u32Id);
    u32Rptr = _HVD_EX_GetESReadPtr(u32Id, FALSE);
    u32ESsize = pCtrl->MemMap.u32BitstreamBufSize;

    if (u32Rptr >= u32Wptr)
    {
        u32CurMBX = u32Rptr - u32Wptr;
    }
    else
    {
        u32CurMBX = u32ESsize - (u32Wptr - u32Rptr);
    }

    if (u32CurMBX == 0)
    {
        u32Ret = E_HVD_ESB_LEVEL_UNDER;
    }
    else if (u32CurMBX < HVD_FW_AVC_ES_OVER_THRESHOLD)
    {
        u32Ret = E_HVD_ESB_LEVEL_OVER;
    }
    else
    {
        u32CurMBX = u32ESsize - u32CurMBX;
        if (u32CurMBX < HVD_FW_AVC_ES_UNDER_THRESHOLD)
        {
            u32Ret = E_HVD_ESB_LEVEL_UNDER;
        }
    }

    return u32Ret;
}

static MS_U32 _HVD_EX_GetESQuantity(MS_U32 u32Id)
{
    MS_U32 u32Wptr      = 0;
    MS_U32 u32Rptr      = 0;
    MS_U32 u32ESsize    = 0;
    MS_U32 u32Ret       = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    u32Wptr = _HVD_EX_GetESWritePtr(u32Id);
    u32Rptr = _HVD_EX_GetESReadPtr(u32Id, FALSE);
    u32ESsize = pCtrl->MemMap.u32BitstreamBufSize;


    if(u32Wptr >= u32Rptr)
    {
        u32Ret = u32Wptr - u32Rptr;
    }
    else
    {
        u32Ret = u32ESsize - u32Rptr + u32Wptr;
    }
    //printf("ES Quantity <0x%lx> W:0x%lx, R:0x%lx, Q:0x%lx.\n",u32Id,u32Wptr,u32Rptr,u32Ret);
    return u32Ret;
}

static MS_BOOL _HVD_EX_SetRegCPU(MS_U32 u32Id)
{
    MS_U32 u32FirmVer = 0;
    MS_U32 u32Timeout = 20000;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    HVD_EX_MSG_DBG("HVD HW ver id: 0x%04lx\n", HAL_HVD_EX_GetHWVersionID());

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase, __FUNCTION__, __LINE__);
#endif

    HAL_VPU_EX_SetFWReload(!pCtrl->bTurboFWMode);

    VPU_EX_FWCodeCfg    fwCfg;
    VPU_EX_TaskInfo     taskInfo;
    VPU_EX_VLCTblCfg    vlcCfg;
    VPU_EX_NDecInitPara nDecInitPara;

    memset(&fwCfg,          0, sizeof(VPU_EX_FWCodeCfg));
    memset(&taskInfo,       0, sizeof(VPU_EX_TaskInfo));
    memset(&vlcCfg,         0, sizeof(VPU_EX_VLCTblCfg));
    memset(&nDecInitPara,   0, sizeof(VPU_EX_NDecInitPara));

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM) //rm
    {
        vlcCfg.u32DstAddr           = MsOS_PA2KSEG1(pCtrl->MemMap.u32FrameBufAddr + pHVDHalContext->u32RV_VLCTableAddr);
        vlcCfg.u32BinAddr           = pCtrl->MemMap.u32VLCBinaryVAddr;
        vlcCfg.u32BinSize           = pCtrl->MemMap.u32VLCBinarySize;
        vlcCfg.u32FrameBufAddr      = pCtrl->MemMap.u32FrameBufVAddr;
        vlcCfg.u32VLCTableOffset    = pHVDHalContext->u32RV_VLCTableAddr;
        nDecInitPara.pVLCCfg        = &vlcCfg;
    }

    nDecInitPara.pFWCodeCfg = &fwCfg;
    nDecInitPara.pTaskInfo  = &taskInfo;

    fwCfg.u8SrcType  = pCtrl->MemMap.eFWSourceType;
    fwCfg.u32DstAddr = pCtrl->MemMap.u32CodeBufVAddr;
    fwCfg.u32DstSize = pCtrl->MemMap.u32CodeBufSize;
    fwCfg.u32BinAddr = pCtrl->MemMap.u32FWBinaryVAddr;
    fwCfg.u32BinSize = pCtrl->MemMap.u32FWBinarySize;

    taskInfo.u32Id = u32Id;

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC)
    {
        taskInfo.eDecType = E_VPU_EX_DECODER_HVD; //E_VPU_EX_DECODER_MVC;
    }
    else
    {
        taskInfo.eDecType = E_VPU_EX_DECODER_HVD;
    }

    taskInfo.eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        taskInfo.eSrcType = E_VPU_EX_INPUT_FILE;
    }
    else
    {
        taskInfo.eSrcType = E_VPU_EX_INPUT_TSP;
    }
    taskInfo.u32HeapSize = HVD_DRAM_SIZE;

#ifdef SUPPORT_EVD
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC)
        taskInfo.u32HeapSize = EVD_DRAM_SIZE;
#endif

    if(TRUE == HVD_EX_GetRstFlag())
    {
        //Delete task for Rst
        if(!HAL_VPU_EX_TaskDelete(u32Id, &nDecInitPara))
        {
           HVD_EX_MSG_ERR("HAL_VPU_EX_TaskDelete fail\n");
        }
        HVD_EX_SetRstFlag(FALSE);
    }

    if (!HAL_VPU_EX_TaskCreate(u32Id, &nDecInitPara))
    {
        HVD_EX_MSG_ERR("Task create fail!\n");

        return FALSE;
    }

    while (u32Timeout)
    {
        u32FirmVer = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_INIT_DONE);

        if (u32FirmVer != 0)
        {
            u32FirmVer = HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_VERSION_ID);
            break;
        }
        u32Timeout--;
        HVD_Delay_ms(1);
    }

    if (u32Timeout > 0)
    {
        MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

        pHVDHalContext->_stHVDStream[u8Idx].bUsed = TRUE;

        HVD_EX_MSG_INF("FW version binary=0x%lx, if=0x%lx\n", u32FirmVer, (MS_U32) HVD_FW_VERSION);
    }
    else
    {
        HVD_EX_MSG_ERR("Cannot get FW version !!0x%x 0x%lx \n", (MS_S16) _HVD_Read2Byte(HVD_REG_RESET),
                    HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_VERSION_ID));

        if (TRUE != HAL_VPU_EX_TaskDelete(u32Id, &nDecInitPara))
        {
           HVD_EX_MSG_ERR("Task delete fail!\n");
        }

        return FALSE;
    }

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase, __FUNCTION__, __LINE__);
#endif

    return TRUE;
}

static MS_U32 _HVD_EX_GetPTSTableRptr(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    if (pShm->u32PTStableRptrAddr & VPU_QMEM_BASE)
    {
        return HAL_VPU_EX_MemRead(pShm->u32PTStableRptrAddr);
    }
    else
    {
        return *((MS_U32 *) MsOS_PA2KSEG1(pShm->u32PTStableRptrAddr + pCtrl->MemMap.u32CodeBufAddr));
    }
}

static MS_U32 _HVD_EX_GetPTSTableWptr(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (pShm->u32PTStableWptrAddr & VPU_QMEM_BASE)
    {
        return HAL_VPU_EX_MemRead(pShm->u32PTStableWptrAddr);
    }
    else
    {
        return *((MS_U32 *) MsOS_PA2KSEG1(pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr));
    }
}

static void _HVD_EX_SetPTSTableWptr(MS_U32 u32Id, MS_U32 u32Value)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (pShm->u32PTStableWptrAddr & VPU_QMEM_BASE)
    {
        if (!HAL_VPU_EX_MemWrite(pShm->u32PTStableWptrAddr, u32Value))
        {
            HVD_EX_MSG_ERR("PTS table SRAM write failed\n");
        }
    }
    else
    {
        *((MS_U32 *) MsOS_PA2KSEG1(pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr)) = u32Value;
    }
}

static HVD_Return _HVD_EX_UpdatePTSTable(MS_U32 u32Id, HVD_BBU_Info *pInfo)
{
    MS_U32 u32PTSWptr = HVD_U32_MAX;
    MS_U32 u32PTSRptr = HVD_U32_MAX;
    MS_U32 u32DestAddr = 0;
    HVD_PTS_Entry PTSEntry;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    // update R & W ptr
    u32PTSRptr = _HVD_EX_GetPTSTableRptr(u32Id);

    HVD_EX_MSG_DBG("PTS table rptr:0x%lx, wptr=0x%lx\n", u32PTSRptr, _HVD_EX_GetPTSTableWptr(u32Id));

    if (u32PTSRptr >= MAX_PTS_TABLE_SIZE)
    {
        HVD_EX_MSG_ERR("PTS table Read Ptr(%lx) > max table size(%lx) \n", u32PTSRptr,
                    (MS_U32) MAX_PTS_TABLE_SIZE);
        return E_HVD_RETURN_FAIL;
    }

    // check queue is full or not
    u32PTSWptr = pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr + 1;
    u32PTSWptr %= MAX_PTS_TABLE_SIZE;

    if (u32PTSWptr == u32PTSRptr)
    {
        HVD_EX_MSG_ERR("PTS table full. Read Ptr(%lx) == new Write ptr(%lx) ,Pre Wptr(%lx) \n", u32PTSRptr,
                    u32PTSWptr, pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr);
        return E_HVD_RETURN_FAIL;
    }

    // add one PTS entry
    PTSEntry.u32ByteCnt = pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt & HVD_BYTE_COUNT_MASK;
    PTSEntry.u32ID_L = pInfo->u32ID_L;
    PTSEntry.u32ID_H = pInfo->u32ID_H;
    PTSEntry.u32PTS = pInfo->u32TimeStamp;

    u32DestAddr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + pShm->u32HVD_PTS_TABLE_ST_OFFSET + (pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr * sizeof(HVD_PTS_Entry)));

    HVD_EX_MSG_DBG("PTS entry dst addr=0x%lx\n", MsOS_VA2PA(u32DestAddr));

    HVD_memcpy((void *) u32DestAddr, &PTSEntry, sizeof(HVD_PTS_Entry));

    HAL_HVD_EX_FlushMemory();

    // update Write ptr
    _HVD_EX_SetPTSTableWptr(u32Id, u32PTSWptr);

    pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr = u32PTSWptr;

    return E_HVD_RETURN_SUCCESS;
}

static HVD_Return _HVD_EX_UpdateESWptr(MS_U32 u32Id, MS_U32 u32NalOffset, MS_U32 u32NalLen)
{
    //---------------------------------------------------
    // item format in nal table:
    // reserved |borken| u32NalOffset | u32NalLen
    //    13 bits    |1bit     |  29 bits           | 21 bits   (total 8 bytes)
    //---------------------------------------------------
    MS_U32 u32Adr = 0;
    MS_U32 u32BBUNewWptr = 0;
    MS_U8 item[8];
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U32 u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU_DRAM_ST_ADDR;

#if HVD_ENABLE_MVC
    if(HAL_HVD_EX_CheckMVCID(u32Id))
    {
        // if MVC_BBU_ADDR and HVD_BBU_ADDR are different, we need to add MVC_BBU_DRAM_ST_ADDR and MVC_BBU2_DRAM_ST_ADDR in share memory
        u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU_DRAM_ST_ADDR; //pShm->u32MVC_BBU_DRAM_ST_ADDR;
        if(E_VDEC_EX_SUB_VIEW  == HAL_HVD_EX_GetView(u32Id))
        {
            u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU2_DRAM_ST_ADDR;  //pShm->u32MVC_BBU2_DRAM_ST_ADDR;
        }
    }
#endif /// HVD_ENABLE_MVC

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        u32BBUNewWptr = pHVDHalContext->u32VP8BBUWptr;
    }
    else
    {
        u32BBUNewWptr = pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr;
    }
    u32BBUNewWptr++;
    u32BBUNewWptr %= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum;

    // prepare nal entry

    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        // NAL len 22 bits  , HEVC level5 constrain
        item[0] = u32NalLen & 0xff;
        item[1] = (u32NalLen >> 8) & 0xff;
        item[2] = ((u32NalLen >> 16) & 0x3f) | ((u32NalOffset << 6) & 0xc0);
        item[3] = (u32NalOffset >> 2) & 0xff;
        item[4] = (u32NalOffset >> 10) & 0xff;
        item[5] = (u32NalOffset >> 18) & 0xff;
        item[6] = (u32NalOffset >> 26) & 0x0f;        //including broken bit
        item[7] = 0;
    }
    else
    {
        item[0] = u32NalLen & 0xff;
        item[1] = (u32NalLen >> 8) & 0xff;
        item[2] = ((u32NalLen >> 16) & 0x1f) | ((u32NalOffset << 5) & 0xe0);
        item[3] = (u32NalOffset >> 3) & 0xff;
        item[4] = (u32NalOffset >> 11) & 0xff;
        item[5] = (u32NalOffset >> 19) & 0xff;
        item[6] = (u32NalOffset >> 27) & 0x07;        //including broken bit
        item[7] = 0;
    }

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        u32Adr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32BBU_DRAM_ST_ADDR + (pHVDHalContext->u32VP8BBUWptr << 3));
    }
    else
    {
        // add nal entry
        u32Adr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32BBU_DRAM_ST_ADDR + (pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr << 3));
    }

    HVD_memcpy((void *) u32Adr, (void *) item, 8);

    HAL_HVD_EX_FlushMemory();

    HVD_EX_MSG_DBG("addr=0x%lx, bbu wptr=0x%lx\n", MsOS_VA2PA(u32Adr), pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr);

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        pHVDHalContext->u32VP8BBUWptr = u32BBUNewWptr;
    }
    else
    {
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr = u32BBUNewWptr;
    }

    return E_HVD_RETURN_SUCCESS;
}

static HVD_Return _HVD_EX_UpdateESWptr_VP8(MS_U32 u32Id, MS_U32 u32NalOffset, MS_U32 u32NalLen, MS_U32 u32NalOffset2, MS_U32 u32NalLen2)
{
    MS_U8 item[8];
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32Adr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U32 u32VP8_BBU_DRAM_ST_ADDR_BS4 = pShm->u32HVD_BBU2_DRAM_ST_ADDR;

    /*
    printf("nal2 offset=0x%x, len=0x%x\n",
        u32NalOffset2, u32NalLen2);
    */

    item[0] = u32NalLen2 & 0xff;
    item[1] = (u32NalLen2 >> 8) & 0xff;
    item[2] = ((u32NalLen2 >> 16) & 0x1f) | ((u32NalOffset2 << 5) & 0xe0);
    item[3] = (u32NalOffset2 >> 3) & 0xff;
    item[4] = (u32NalOffset2 >> 11) & 0xff;
    item[5] = (u32NalOffset2 >> 19) & 0xff;
    item[6] = (u32NalOffset2 >> 27) & 0x07;
    item[7] = 0;

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        u32Adr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32VP8_BBU_DRAM_ST_ADDR_BS4 + (pHVDHalContext->u32VP8BBUWptr << 3));
    }
    else
    {
        u32Adr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32VP8_BBU_DRAM_ST_ADDR_BS4 + (pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr << 3));
    }

    HVD_memcpy((void *) u32Adr, (void *) item, 8);

    HAL_HVD_EX_FlushMemory();

    return _HVD_EX_UpdateESWptr(u32Id, u32NalOffset, u32NalLen);
}

static MS_U32 _HVD_EX_GetVUIDispInfo(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if( ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC) ||
        ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC)  )
    {
        MS_U16 i;
        MS_U32 u32VUIAddr;
        MS_U32 *pData = (MS_U32 *) &(pHVDHalContext->g_hvd_VUIINFO);

        HAL_HVD_EX_ReadMemory();
        u32VUIAddr = pShm->u32AVC_VUIDispInfo_Addr;

        for (i = 0; i < sizeof(HVD_AVC_VUI_DISP_INFO); i += 4)
        {
            if (pShm->u32AVC_VUIDispInfo_Addr & VPU_QMEM_BASE)
            {
                *pData = HAL_VPU_EX_MemRead(u32VUIAddr + i);
            }
            else
            {
                *pData = *((MS_U32 *) MsOS_PA2KSEG1(u32VUIAddr + i + pCtrl->MemMap.u32CodeBufAddr));
            }
            pData++;
        }
    }
    else
    {
        memset(&(pHVDHalContext->g_hvd_VUIINFO), 0, sizeof(HVD_AVC_VUI_DISP_INFO));
    }

    return (MS_U32) &(pHVDHalContext->g_hvd_VUIINFO);
}

static MS_U32 _HVD_EX_GetBBUQNumb(MS_U32 u32Id)
{
    MS_U32 u32ReadPtr = 0;
    MS_U32 eRet = 0;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    u32ReadPtr = _HVD_EX_GetBBUReadptr(u32Id);
    MS_U32 u32WritePtr = 0;

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        u32WritePtr = pHVDHalContext->u32VP8BBUWptr;
    }
    else
    {
        u32WritePtr = pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr;
    }

    HVD_EX_MSG_DBG("idx=%x, bbu rptr=%lx, bbu wptr=%lx\n", u8Idx, u32ReadPtr, u32WritePtr);

    if (u32WritePtr >= u32ReadPtr)
    {
        eRet = u32WritePtr - u32ReadPtr;
    }
    else
    {
        eRet = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - (u32ReadPtr - u32WritePtr);
    }

#if 0
    if (pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr >= u32ReadPtr)
    {
        eRet = pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr - u32ReadPtr;
    }
    else
    {
        eRet = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - (u32ReadPtr - pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr);
    }

#endif
    return eRet;
}

static MS_U32 _HVD_EX_GetPTSQNumb(MS_U32 u32Id)
{
    MS_U32 u32ReadPtr = 0;
    MS_U32 eRet = 0;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    u32ReadPtr = _HVD_EX_GetPTSTableRptr(u32Id);

    if (u32ReadPtr >= MAX_PTS_TABLE_SIZE)
    {
        HVD_EX_MSG_ERR("PTS table Read Ptr(%lx) > max table size(%lx) \n", u32ReadPtr,
                    (MS_U32) MAX_PTS_TABLE_SIZE);
        return 0;
    }

    u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    if (pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr >= u32ReadPtr)
    {
        eRet = pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr - u32ReadPtr;
    }
    else
    {
        eRet = MAX_PTS_TABLE_SIZE - (u32ReadPtr - pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr);
    }

    return eRet;
}

static HVD_Frm_Information *_HVD_EX_GetNextDispFrame(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U16 u16QNum = pShm->u16DispQNumb;
    MS_U16 u16QPtr = pShm->u16DispQPtr;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    //static volatile HVD_Frm_Information *pHvdFrm = NULL;
#if (HVD_ENABLE_MVC)
    MS_BOOL bMVC = HAL_HVD_EX_CheckMVCID(u32Id);

    if(bMVC)
    {
        if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
        {
            MS_U16 u16RealQPtr = pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex;
            MS_U16 u16UsedFrm = 0;

            if (u16RealQPtr != u16QPtr)
            {
                if (u16RealQPtr > u16QPtr)
                {
                    u16UsedFrm = u16RealQPtr - u16QPtr;
                }
                else
                {
                    u16UsedFrm = pShm->u16DispQSize - (u16QPtr - u16RealQPtr);
                }
            }

            if (u16QNum > u16UsedFrm)
            {
                volatile HVD_Frm_Information *pHvdFrm;

                u16QNum -= u16UsedFrm;
                u16QPtr = u16RealQPtr;
                pHvdFrm = (volatile HVD_Frm_Information*)&pShm->DispQueue[u16QPtr];

                if ((u16QPtr%2) == 0) //For MVC mode, we must check the pair of display entry is ready or not
                {
                    volatile HVD_Frm_Information *pHvdFrmNext = (volatile HVD_Frm_Information*)&pShm->DispQueue[u16QPtr+1];

                    if (pHvdFrmNext->u32Status != E_HVD_DISPQ_STATUS_INIT)
                    {
                        return NULL;
                    }
                }

                if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT) // Must Be
                {
                    pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;

                    if ((u16QPtr%2) == 0)
                    {
                        //ALOGE("G1: %x", pHvdFrm->u32PrivateData);
                        pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData = pHvdFrm->u32PrivateData;
                    }
                    else
                    {
                        //ALOGE("G2: %x", (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData);
                        //pShm->UpdateQueue[pShm->u16UpdateQWtPtr] = (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData;
                        //pShm->u16UpdateQWtPtr = (pShm->u16UpdateQWtPtr + 1) % HVD_DISP_QUEUE_MAX_SIZE;
                        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData);
                    }

                    u16QPtr++;
                    if (u16QPtr == pShm->u16DispQSize) u16QPtr = 0;
                    pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = u16QPtr;

                    return (HVD_Frm_Information*)(MS_U32)pHvdFrm;
                }
            }

            return NULL;
        }

        /* Add for Mobile Platform by Ted Sun */
#if 0
        if (u16QNum > HVD_DISPQ_PREFETCH_COUNT*3)
        {
            u16QNum = HVD_DISPQ_PREFETCH_COUNT*3;
        }
#endif

        //printf("OQ:%d,DQ:%d.\n",pShm->u16DispQNumb,pShm->u16DecQNumb);
        //search the next frame to display
        while (u16QNum > 0)
        {
            //printf("Pr:%d,%d.[%ld,%ld,%ld,%ld].\n",u16QPtr,u16QNum,pShm->DispQueue[u16QPtr].u32Status,pShm->DispQueue[u16QPtr+1].u32Status,
            //                pShm->DispQueue[u16QPtr+2].u32Status,pShm->DispQueue[u16QPtr+3].u32Status);
            pHVDHalContext->pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

            //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                /// For MVC. Output views after the pair of (base and depend) views were decoded.
                /// Check the depned view was initial when Output the base view.
                if((u16QPtr%2) == 0)
                {
                    volatile HVD_Frm_Information *pHvdFrm_sub = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr+1];
                    //if(pHvdFrm_sub->u32Status != E_HVD_DISPQ_STATUS_INIT)
                    if(pHvdFrm_sub->u32Status == E_HVD_DISPQ_STATUS_NONE)
                    {
                        ///printf("[MVC] %d is not E_HVD_DISPQ_STATUS_INIT (%ld).\n",u16QPtr+1,pHvdFrm_sub->u32Status);
                        ///printf("Return NULL.\n");
                        return NULL;
                    }
                }

                //printf("V:%d.\n",u16QPtr);
                pHVDHalContext->_u16DispQPtr = u16QPtr;
                pHVDHalContext->pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;       /////Change its state!!
                HVD_EX_MSG_DBG("FrameDone: %d, pHvdFrm=0x%lx, timestamp=%ld\n", u16QPtr,
                           (MS_U32) pHVDHalContext->pHvdFrm, pShm->DispQueue[u16QPtr].u32TimeStamp);
                HVD_EX_MSG_INF("<<< halHVD pts,idH = %lu, %lu [%x]\n", pHVDHalContext->pHvdFrm->u32TimeStamp, pHVDHalContext->pHvdFrm->u32ID_H, u16QPtr);     //STS output
                return (HVD_Frm_Information *)(MS_U32) pHVDHalContext->pHvdFrm;
            }

            u16QNum--;
            //go to next frame in the dispQ
            u16QPtr++;

            if (u16QPtr >= pShm->u16DispQSize)
            {
                u16QPtr -= pShm->u16DispQSize;        //wrap to the begin
            }
        }
    }
    else
#endif ///HVD_ENABLE_MVC
    {
        volatile HVD_Frm_Information *pHvdFrm = (volatile HVD_Frm_Information*)&pShm->DispQueue[u16QPtr];

        if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
        {

            while (u16QNum != 0)
            {
                pHvdFrm = (volatile HVD_Frm_Information*)&pShm->DispQueue[u16QPtr];

                if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT) // Must Be
                {
                    pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, pHvdFrm->u32PrivateData);

                    return (HVD_Frm_Information*)(MS_U32)pHvdFrm;

                }
                u16QNum--;
                //go to next frame in the dispQ
                u16QPtr++;

                if (u16QPtr == pShm->u16DispQSize)
                {
                    u16QPtr = 0;        //wrap to the begin
                }

            }



            return NULL;
        }
        /* Add for Mobile Platform by Ted Sun */
#if 0
        if (u16QNum > HVD_DISPQ_PREFETCH_COUNT)
        {
            u16QNum = HVD_DISPQ_PREFETCH_COUNT;
        }
#endif
        //printf("Q: %d %d\n", u16QNum, u16QPtr);
        //search the next frame to display
        while (u16QNum != 0)
        {
            pHVDHalContext->pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

            //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                pHVDHalContext->_u16DispQPtr = u16QPtr;
                pHVDHalContext->pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;       /////Change its state!!
                HVD_EX_MSG_DBG("FrameDone: %d, pHvdFrm=0x%lx, timestamp=%ld\n", u16QPtr,
                            (MS_U32) pHVDHalContext->pHvdFrm, pShm->DispQueue[u16QPtr].u32TimeStamp);
                HVD_EX_MSG_INF("<<< halHVD pts,idH = %lu, %lu [%x]\n", pHVDHalContext->pHvdFrm->u32TimeStamp, pHVDHalContext->pHvdFrm->u32ID_H, u16QPtr);     //STS output
                return (HVD_Frm_Information *)(MS_U32) pHVDHalContext->pHvdFrm;
            }

            u16QNum--;
            //go to next frame in the dispQ
            u16QPtr++;

            if (u16QPtr == pShm->u16DispQSize)
            {
                u16QPtr = 0;        //wrap to the begin
            }
        }
    }

    return NULL;
}
MS_BOOL
HAL_HVD_EX_DispFrameAllViewed(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U16 u16QNum = pShm->u16DispQNumb;
    MS_U16 u16QPtr = pShm->u16DispQPtr;
    static volatile HVD_Frm_Information *pHvdFrm = NULL;

#if (HVD_ENABLE_MVC)
    if (HAL_HVD_EX_CheckMVCID(u32Id))
    {
        if (u16QNum == 1) return TRUE;
    }
#endif

    while (u16QNum != 0)
    {
        pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];
        if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
        {
            return FALSE;
        }
        u16QNum--;
        u16QPtr++;
        if (u16QPtr == pShm->u16DispQSize)
        {
            u16QPtr = 0;        //wrap to the begin
        }
    }

    return TRUE;
}
static HVD_EX_Drv_Ctrl *_HVD_EX_GetDrvCtrl(MS_U32 u32Id)
{
    MS_U8 u8DrvId = (0xFF & (u32Id >> 16));

    return &(_pHVDCtrls[u8DrvId]);
}

static MS_U8 _HVD_EX_GetStreamIdx(MS_U32 u32Id)
{
    MS_U8 u8OffsetIdx           = 0;
    MS_U8 u8SidBaseMask         = 0xF0;
    HAL_HVD_StreamId eSidBase   = (HAL_HVD_StreamId) (u32Id >> 8 & u8SidBaseMask);

    switch (eSidBase)
    {
        case E_HAL_HVD_MAIN_STREAM_BASE:
        {
            u8OffsetIdx = 0;
            break;
        }
        case E_HAL_VPU_SUB_STREAM_BASE:
        {
            u8OffsetIdx = 1;
            break;
    }
        case E_HAL_VPU_MVC_STREAM_BASE:
        {
            u8OffsetIdx = 0;
            break;
        }
        default:
        {
            u8OffsetIdx = 0;
            break;
        }
    }

    return u8OffsetIdx;
}
/*
static MS_BOOL _HAL_HVD_EX_HVDInUsed(void)
{
    MS_U32 i = 0;
    for(i = 0; i < HAL_HVD_EX_MAX_SUPPORT_STREAM; i++)
    {
        if(TRUE == pHVDHalContext->_stHVDStream[i].bUsed)
        {
            return TRUE;
        }
    }
    return FALSE;
}
*/

MS_U32 HAL_HVD_EX_GetShmAddr(MS_U32 u32Id)
{
    MS_U32 u32PhyAddr = 0x0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if (pCtrl->MemMap.u32CodeBufAddr == 0)
    {
        return 0;
    }

    u32PhyAddr = HAL_VPU_EX_GetShareInfoAddr(u32Id);

    if (u32PhyAddr == 0xFFFFFFFF)
    {
        u32PhyAddr = pCtrl->MemMap.u32CodeBufAddr + (HAL_VPU_EX_GetTaskId(u32Id) * HVD_FW_MEM_OFFSET) + HVD_SHARE_MEM_ST_OFFSET;
    }
    else
    {
        #if defined(SUPPORT_NEW_MEM_LAYOUT) || defined(SUPPORT_NEW_VDEC_FLOW)
        //u32PhyAddr += 0;         // if define HVD_OLD_LAYOUT_SHARE_MEM_BIAS under SUPPORT_NEW_MEM_LAYOUT, then we could refine the codes here
        #else
        u32PhyAddr += HVD_OLD_LAYOUT_SHARE_MEM_BIAS;
        #endif
    }

    return MsOS_PA2KSEG1(u32PhyAddr);
}


void HAL_HVD_MVDMiuClientSel(MS_U8 u8MiuSel)
{

    if (u8MiuSel == 0)
    {
        _HVD_WriteWordMask(MIU_CLIENT_SELECT_GP2, 0, MIU_CLIENT_SELECT_GP2_MVD);
    }
    else
    {
        _HVD_WriteWordMask(MIU_CLIENT_SELECT_GP2, MIU_CLIENT_SELECT_GP2_MVD, MIU_CLIENT_SELECT_GP2_MVD);
    }

}




MS_BOOL HAL_HVD_EX_InitHW(MS_U32 u32Id,VPU_EX_DecoderType DecoderType)
{
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    // power on / reset HVD; set nal, es rw, bbu parser, release HVD engine
    // re-setup clock.

    if (!HAL_VPU_EX_HVDInUsed())
    {
        printf("HVD power on\n");
        HAL_HVD_EX_PowerCtrl(TRUE);
    }

    #if SUPPORT_EVD
    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        printf("EVD power on\n");
        HAL_EVD_EX_PowerCtrl(TRUE);
    }
    #endif

    if ((!HAL_VPU_EX_HVDInUsed()) && (DecoderType != E_VPU_EX_DECODER_MVD))
    {
        pHVDHalContext->_stHVDStream[0].u32BBUWptr = 0; //main
        pHVDHalContext->_stHVDStream[1].u32BBUWptr = 0; //sub
        pHVDHalContext->u32VP8BBUWptr = 0; //VP8
        _HVD_EX_ResetMainSubBBUWptr(u32Id);

        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST, HVD_REG_RESET_SWRST);

        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_MC_MIU_256 , HVD_REG_MC_MIU_256);


        if((pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.bEnable) &&
           ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC))
        {
            if(pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufAddr >= HAL_MIU1_BASE)
            {
                _HAL_HVD_Entry();
                HAL_HVD_MVDMiuClientSel(1);
                _HAL_HVD_Release();
            }
            else
            {
                _HAL_HVD_Entry();
                HAL_HVD_MVDMiuClientSel(0);
                _HAL_HVD_Release();
            }
        }
    }

    #if SUPPORT_EVD
    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_SWRST, EVD_REG_RESET_SWRST);
    }
    #endif

    if(pCtrl == NULL)
    {
        HVD_EX_MSG_ERR("HAL_HVD_EX_InitHW Ctrl is NULL.\n");
        //return FALSE;
        goto RESET;
    }

#if SUPPORT_EVD
    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_HK_HEVC_MODE, EVD_REG_RESET_HK_HEVC_MODE);

        if ((E_HVD_INIT_MAIN_LIVE_STREAM == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK))
            ||(E_HVD_INIT_MAIN_FILE_TS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK)))
        {
            _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_HK_TSP2EVD_EN, EVD_REG_RESET_HK_TSP2EVD_EN);
        }
        goto RESET;
    }
#endif

    // HVD4, from JANUS and later chip
    switch ((pCtrl->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK)
    {
        case E_HVD_INIT_HW_AVS:
        {
            if (0 == u8TaskId)
            {
                _HVD_WriteWordMask(HVD_REG_RESET, 0,
                               HVD_REG_RESET_HK_AVS_MODE | HVD_REG_RESET_HK_RM_MODE);
            }
            else
            {
                _HVD_WriteWordMask(HVD_REG_MODE_BS2, 0,
                               HVD_REG_MODE_HK_AVS_MODE_BS2 | HVD_REG_MODE_HK_RM_MODE_BS2);
            }

            break;
        }
        case E_HVD_INIT_HW_RM:
        {
            if (0 == u8TaskId)
            {
                _HVD_WriteWordMask(HVD_REG_RESET, 0,
                                   HVD_REG_RESET_HK_AVS_MODE | HVD_REG_RESET_HK_RM_MODE);

                if (pCtrl->InitParams.pRVFileInfo->RV_Version) // RV 9,10
                {
                    _HVD_WriteWordMask(HVD_REG_RESET, 0, HVD_REG_RESET_HK_RV9_DEC_MODE);
                }
                else // RV 8
                {
                    _HVD_WriteWordMask(HVD_REG_RESET, 0, HVD_REG_RESET_HK_RV9_DEC_MODE);
                }
            }
            else
            {
                _HVD_WriteWordMask(HVD_REG_MODE_BS2, 0,
                                   HVD_REG_MODE_HK_AVS_MODE_BS2 | HVD_REG_MODE_HK_RM_MODE_BS2);

                if (pCtrl->InitParams.pRVFileInfo->RV_Version) // RV 9,10
                {
                    _HVD_WriteWordMask(HVD_REG_MODE_BS2, 0, HVD_REG_MODE_HK_RV9_DEC_MODE_BS2);
                }
                else // RV 8
                {
                    _HVD_WriteWordMask(HVD_REG_MODE_BS2, 0, HVD_REG_MODE_HK_RV9_DEC_MODE_BS2);
                }

            }

            break;
        }
        default:
        {
            if (0 == u8TaskId)
            {
                _HVD_WriteWordMask(HVD_REG_RESET, 0, HVD_REG_RESET_HK_AVS_MODE | HVD_REG_RESET_HK_RM_MODE);
            }
            else
            {
                _HVD_WriteWordMask(HVD_REG_MODE_BS2, 0, HVD_REG_MODE_HK_AVS_MODE_BS2 | HVD_REG_MODE_HK_RM_MODE_BS2);
            }
            break;
        }
    }

RESET:
    //T9: use miu128bit
    HVD_EX_MSG_DBG("(be)Miu128 bits Status = %x <<<<<<<\n", _HVD_Read2Byte(HVD_REG_RESET));

    if (!HAL_VPU_EX_HVDInUsed())
    {
        _HVD_Write2Byte(HVD_REG_RESET, (_HVD_Read2Byte(HVD_REG_RESET) | HVD_REG_RESET_MIU_128));
    }

     HVD_EX_MSG_DBG("(af)Miu128 bits Status = %x <<<<<<<\n", _HVD_Read2Byte(HVD_REG_RESET));

#if SUPPORT_EVD
    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        printf("EVD miu 256 bits\n");
        _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) | EVD_REG_RESET_MIU_256));
    }
#endif
#if 0 //defined(SUPPORT_NEW_MEM_LAYOUT) || defined(SUPPORT_NEW_VDEC_FLOW)
    // Only ES buffer addrress needs to be set for VP8
    _HVD_EX_SetESBufferAddr(u32Id);
#else
    if(DecoderType != E_VPU_EX_DECODER_MVD)
    {
    _HVD_EX_SetBufferAddr(u32Id);
    }
#endif
    if (!HAL_VPU_EX_HVDInUsed())
    {
        _HVD_WriteWordMask(HVD_REG_RESET, 0, HVD_REG_RESET_SWRST);
    }

#if SUPPORT_EVD
    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_WriteWordMask(EVD_REG_RESET, 0, EVD_REG_RESET_SWRST);
    }
#endif

    return TRUE;
}

MS_BOOL HAL_HVD_EX_DeinitHW(void)
{
    MS_U16 u16Timeout = 1000;

    _HVD_EX_SetMIUProtectMask(TRUE);

#if SUPPORT_EVD //EVD using HVD DIU, it should be turn off EVD first
    //We assume HEVC belong to HVD, so we can disable EVD_REG_RESET_HK_TSP2EVD_EN in DeinitHW.
    _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) & ~EVD_REG_RESET_HK_TSP2EVD_EN)); //0: tsp2hvd, coz EVD & HVD use the same MVD parser
    HAL_EVD_EX_DeinitHW();
#endif

    _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST, HVD_REG_RESET_SWRST);

    while (u16Timeout)
    {
        if ((_HVD_Read2Byte(HVD_REG_RESET) & (HVD_REG_RESET_SWRST_FIN)) == (HVD_REG_RESET_SWRST_FIN))
        {
            break;
        }
        u16Timeout--;
    }

    HAL_HVD_EX_PowerCtrl(FALSE);

    _HVD_EX_SetMIUProtectMask(FALSE);

    return TRUE;
}

void HAL_HVD_EX_FlushMemory(void)
{
    MsOS_FlushMemory();
}

void HAL_HVD_EX_ReadMemory(void)
{
    MsOS_ReadMemory();
}

void HAL_HVD_EX_SetDrvCtrlsBase(HVD_EX_Drv_Ctrl *pHVDCtrlsBase)
{
    _pHVDCtrls = pHVDCtrlsBase;
}

void HAL_HVD_EX_CheckMIUSel(MS_BOOL bChange)
{
    return;
}

MS_U32 HAL_HVD_EX_GetHWVersionID(void)
{
    return _HVD_Read2Byte(HVD_REG_REV_ID);
}


MS_BOOL HAL_HVD_EX_Init_Share_Mem(void)
{
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#if !defined(SUPPORT_X_MODEL_FEATURE)
    MS_U32 u32ShmId;
    MS_U32 u32Addr;
    MS_U32 u32BufSize;


    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux HVD HAL",
                                          sizeof(HVD_Hal_CTX),
                                          &u32ShmId,
                                          &u32Addr,
                                          &u32BufSize,
                                          MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux HVD HAL",
                                             sizeof(HVD_Hal_CTX),
                                             &u32ShmId,
                                             &u32Addr,
                                             &u32BufSize,
                                             MSOS_SHM_CREATE))
        {
            HVD_EX_MSG_ERR("[%s]SHM allocation failed!!!use global structure instead!!!\n",__FUNCTION__);
            if(pHVDHalContext == NULL)
            {
                pHVDHalContext = &gHVDHalContext;
                memset(pHVDHalContext,0,sizeof(HVD_Hal_CTX));
                _HVD_EX_Context_Init_HAL();
                HVD_PRINT("[%s]Global structure init Success!!!\n",__FUNCTION__);
            }
            else
            {
                HVD_PRINT("[%s]Global structure exists!!!\n",__FUNCTION__);
            }
            //return FALSE;
        }
        else
        {
            memset((MS_U8*)u32Addr,0,sizeof(HVD_Hal_CTX));
            pHVDHalContext = (HVD_Hal_CTX*)u32Addr; // for one process
            _HVD_EX_Context_Init_HAL();
        }
    }
    else
    {
        pHVDHalContext = (HVD_Hal_CTX*)u32Addr; // for another process
    }
#else
    if(pHVDHalContext == NULL)
    {
        pHVDHalContext = &gHVDHalContext;
        memset(pHVDHalContext,0,sizeof(HVD_Hal_CTX));
        _HVD_EX_Context_Init_HAL();
    }
#endif
    _HAL_HVD_MutexCreate();
#else
    if(pHVDHalContext == NULL)
    {
        pHVDHalContext = &gHVDHalContext;
        memset(pHVDHalContext,0,sizeof(HVD_Hal_CTX));
        _HVD_EX_Context_Init_HAL();
    }
#endif

    return TRUE;
}


HAL_HVD_StreamId HAL_HVD_EX_GetFreeStream(HAL_HVD_StreamType eStreamType)
{
    MS_U32 i = 0;

    if (eStreamType == E_HAL_HVD_MVC_STREAM)
    {
        if ((FALSE == pHVDHalContext->_stHVDStream[0].bUsed) && (FALSE == pHVDHalContext->_stHVDStream[1].bUsed))
            return pHVDHalContext->_stHVDStream[0].eStreamId;
    }
    else if (eStreamType == E_HAL_HVD_MAIN_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_HVD_MAIN_STREAM_MAX - E_HAL_HVD_MAIN_STREAM_BASE) +
              (E_HAL_HVD_SUB_STREAM_MAX - E_HAL_HVD_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_HVD_MAIN_STREAM_BASE & pHVDHalContext->_stHVDStream[i].eStreamId) && (FALSE == pHVDHalContext->_stHVDStream[i].bUsed))
            {
                return pHVDHalContext->_stHVDStream[i].eStreamId;
            }
        }
    }
    else if (eStreamType == E_HAL_HVD_SUB_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_HVD_MAIN_STREAM_MAX - E_HAL_HVD_MAIN_STREAM_BASE) +
              (E_HAL_HVD_SUB_STREAM_MAX - E_HAL_HVD_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_HVD_SUB_STREAM_BASE & pHVDHalContext->_stHVDStream[i].eStreamId) && (FALSE == pHVDHalContext->_stHVDStream[i].bUsed))
            {
                return pHVDHalContext->_stHVDStream[i].eStreamId;
            }
        }
    }

    return E_HAL_HVD_STREAM_NONE;
}

void HAL_HVD_EX_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteByteMask(REG_TOP_HVD, ~TOP_CKG_HVD_DIS, TOP_CKG_HVD_DIS);
    }
    else
    {
        _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_DIS, TOP_CKG_HVD_DIS);
    }

    // fix to not inverse
    _HVD_WriteByteMask(REG_TOP_HVD, ~TOP_CKG_HVD_INV, TOP_CKG_HVD_INV);

    switch (pHVDHalContext->u32HVDClockType)
    {
        case 240:
        {
            _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_240MHZ, TOP_CKG_HVD_CLK_MASK);

            break;
        }
        case 216:
        {
            _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_216MHZ, TOP_CKG_HVD_CLK_MASK);

            break;
        }
        case 172:
        {
            _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_172MHZ, TOP_CKG_HVD_CLK_MASK);

            break;
        }
        case 160:
        {
            _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_160MHZ, TOP_CKG_HVD_CLK_MASK);

            break;
        }
        default:
        {
            _HVD_WriteByteMask(REG_TOP_HVD, TOP_CKG_HVD_240MHZ, TOP_CKG_HVD_CLK_MASK);                                                                                                   // TODO://if(hvd_clk > miu_clk) set to TOP_CKG_HVD_IDB_CLK_HVD_MIU = 0
                                                                                                               // TODO://else set to TOP_CKG_HVD_IDB_CLK_HVD_MIU = 1
            break;
        }
    }

    return;
}

void HAL_HVD_EX_InitRegBase(MS_U32 u32RegBase)
{
    u32HVDRegOSBase = u32RegBase;
    HAL_VPU_EX_InitRegBase(u32RegBase);
}

void HAL_HVD_EX_SetPreCtrlVariables(MS_U32 u32Id,MS_U32 drvprectrl)
{
    HVD_Pre_Ctrl *pHVDPreCtrl_in = (HVD_Pre_Ctrl*)drvprectrl;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    pHVDHalContext->pHVDPreCtrl_Hal[u8Idx] = pHVDPreCtrl_in;
}

HVD_Return HAL_HVD_EX_InitVariables(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = NULL;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
#if HVD_ENABLE_MVC
    MS_BOOL bMVC = HAL_HVD_EX_CheckMVCID(u32Id);
#endif ///HVD_ENABLE_MVC

    pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr   = 0;
    pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt   = 0;
    pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum  = 0;
    pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex   = 0;
    pHVDHalContext->_stHVDStream[u8Idx].u32FreeData     = 0xFFFF;
#if HVD_ENABLE_MVC
    if(bMVC)
    {
        pHVDHalContext->_stHVDStream[u8Idx+1].u32PTSPreWptr   = 0;
        pHVDHalContext->_stHVDStream[u8Idx+1].u32PTSByteCnt   = 0;
        pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUWptr      = 0;
        pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUEntryNum  = 0;
    }
#endif ///HVD_ENABLE_MVC

    // set a local copy of FW code address; assuming there is only one copy of FW,
    // no matter how many task will be created.

    pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    memset((void *) (pHVDHalContext->g_hvd_nal_fill_pair), 0, 16);

    // global variables
    pHVDHalContext->u32HVDCmdTimeout = pCtrl->u32CmdTimeout;


//    pHVDHalContext->u32VPUClockType = (MS_U32) pCtrl->InitParams.u16DecoderClock;
//    pHVDHalContext->u32HVDClockType = (MS_U32) pCtrl->InitParams.u16DecoderClock;
    // Create mutex
    //_HAL_HVD_MutexCreate();

    // fill HVD init variables
    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum = VP8_BBU_DRAM_TBL_ENTRY;
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNumTH = VP8_BBU_DRAM_TBL_ENTRY_TH;
    }
    else
#if HVD_ENABLE_RV_FEATURE
    if (((pCtrl->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)
    {
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum = RVD_BBU_DRAM_TBL_ENTRY;
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNumTH = RVD_BBU_DRAM_TBL_ENTRY_TH;

        if (pCtrl->MemMap.u32FrameBufSize > RV_VLC_TABLE_SIZE)
        {
            pHVDHalContext->u32RV_VLCTableAddr = pCtrl->MemMap.u32FrameBufSize - RV_VLC_TABLE_SIZE;
            pCtrl->MemMap.u32FrameBufSize -= RV_VLC_TABLE_SIZE;
        }
        else
        {
            HVD_EX_MSG_ERR("HAL_HVD_EX_InitVariables failed: frame buffer size too small. FB:%lx min:%lx\n",
                        (MS_U32) pCtrl->MemMap.u32FrameBufSize, (MS_U32) RV_VLC_TABLE_SIZE);
            return E_HVD_RETURN_INVALID_PARAMETER;
        }
    }
    else
#endif
    {
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum = HVD_BBU_DRAM_TBL_ENTRY;
        pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNumTH = HVD_BBU_DRAM_TBL_ENTRY_TH;
#if HVD_ENABLE_MVC
        if(bMVC)
        {
            pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUEntryNum = MVC_BBU_DRAM_TBL_ENTRY;
            pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUEntryNumTH = MVC_BBU_DRAM_TBL_ENTRY_TH;
        }
#endif /// HVD_ENABLE_MVC
        pHVDHalContext->u32RV_VLCTableAddr = 0;
    }

    if ((HAL_VPU_EX_GetShareInfoAddr(u32Id) != 0xFFFFFFFF)
        || ((pCtrl->MemMap.u32CodeBufVAddr <= (MS_U32) pShm) && ((MS_U32) pShm <= (pCtrl->MemMap.u32CodeBufVAddr + pCtrl->MemMap.u32CodeBufSize)))
        || ((pCtrl->MemMap.u32BitstreamBufVAddr <= (MS_U32) pShm) && ((MS_U32) pShm <= (pCtrl->MemMap.u32BitstreamBufVAddr + pCtrl->MemMap.u32BitstreamBufSize)))
        || ((pCtrl->MemMap.u32FrameBufVAddr <= (MS_U32) pShm) && ((MS_U32) pShm <= (pCtrl->MemMap.u32FrameBufVAddr + pCtrl->MemMap.u32FrameBufSize))))
    {
        HVD_EX_MSG_DBG("input memory: Code addr=0x%lx, Bits addr=0x%lx, FB addr=0x%lx, Miubase=0x%lx\n",
                    pCtrl->MemMap.u32CodeBufAddr,
                    pCtrl->MemMap.u32FrameBufAddr,
                    pCtrl->MemMap.u32BitstreamBufAddr,
                    pCtrl->MemMap.u32MIU1BaseAddr);
#if HVD_ENABLE_MVC
        if(bMVC)
        {
            HVD_EX_Drv_Ctrl *pHVDCtrl_in_sub = _HVD_EX_GetDrvCtrl(u32Id+0x00011000);
            if (( (pHVDCtrl_in_sub->MemMap.u32BitstreamBufVAddr)<=  (MS_U32)pShm)&& ( (MS_U32)pShm <= ((pHVDCtrl_in_sub->MemMap.u32BitstreamBufVAddr )+ pHVDCtrl_in_sub->MemMap.u32BitstreamBufSize)))
            {
                HVD_EX_MSG_DBG("[MVC] Bitstream2: 0x%lx.\n", pCtrl->MemMap.u32BitstreamBufAddr);
            }
        }
#endif /// HVD_ENABLE_MVC

        return E_HVD_RETURN_SUCCESS;
    }
    else
    {
        HVD_EX_MSG_ERR("failed: Shm addr=0x%lx, Code addr=0x%lx, Bits addr=0x%lx, FB addr=0x%lx, Miubase=0x%lx\n",
                    MS_PA2KSEG1((MS_U32) pShm),
                    pCtrl->MemMap.u32CodeBufAddr,
                    pCtrl->MemMap.u32FrameBufAddr,
                    pCtrl->MemMap.u32BitstreamBufAddr,
                    pCtrl->MemMap.u32MIU1BaseAddr);
        return E_HVD_RETURN_INVALID_PARAMETER;
    }
}

HVD_Return HAL_HVD_EX_InitShareMem(MS_U32 u32Id)
{
    MS_U32 u32Addr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    memset(pShm, 0, sizeof(HVD_ShareMem));

    u32Addr = pCtrl->MemMap.u32FrameBufAddr;

    if (u32Addr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        u32Addr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }

    pShm->u32FrameRate = pCtrl->InitParams.u32FrameRate;
    pShm->u32FrameRateBase = pCtrl->InitParams.u32FrameRateBase;
    pShm->u32FrameBufAddr = u32Addr;
    pShm->u32FrameBufSize = pCtrl->MemMap.u32FrameBufSize;
    pShm->DispInfo.u16DispWidth = 1;
    pShm->DispInfo.u16DispHeight = 1;
    pShm->u32CodecType = pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK;
    pShm->u32CPUClock = pHVDHalContext->u32VPUClockType;
    pShm->u32UserCCIdxWrtPtr = 0xFFFFFFFF;
    pShm->DispFrmInfo.u32TimeStamp = 0xFFFFFFFF;
    //Chip info
    pShm->u16ChipID = E_MSTAR_CHIP_MUSTANG;
    pShm->u16ChipECONum = pCtrl->InitParams.u16ChipECONum;
    // PreSetControl
    if (pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->bOnePendingBuffer)
    {
        pShm->u32PreSetControl |= PRESET_ONE_PENDING_BUFFER;
    }


    if ((pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.bEnable) &&
        ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC))
    {
        pShm->u32PreSetControl |= PRESET_IAP_GN_SHARE_BW_MODE;

        if(pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufAddr >= HAL_MIU1_BASE)
        {
            pShm->u32IapGnBufAddr = pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufAddr - HAL_MIU1_BASE;
            pShm->u32IapGnBufSize = pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufSize;
        }
        else
        {
            pShm->u32IapGnBufAddr = pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufAddr;
            pShm->u32IapGnBufSize = pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufSize;
        }
    }


    //pShm->bColocateBBUMode = pCtrl->InitParams.bColocateBBUMode;//johnny.ko
    //pShm->bColocateBBUMode = _stHVDPreSet[u8Idx].bColocateBBUMode;//johnny.ko
    if(_stHVDPreSet[u8Idx].bColocateBBUMode)
        pShm->u8BBUMode = E_HVD_FW_AUTO_BBU_MODE;
    else
        pShm->u8BBUMode = E_HVD_DRV_AUTO_BBU_MODE;
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK) == E_HVD_INIT_MAIN_FILE_RAW)
    {
        if((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_DUAL_ES_MASK) == E_HVD_INIT_DUAL_ES_ENABLE)
        {
            pShm->u8SrcMode = E_HVD_SRC_MODE_FILE_DUAL_ES;
        }
        else
        {
            pShm->u8SrcMode = E_HVD_SRC_MODE_FILE;
        }
    }
    else if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK) == E_HVD_INIT_MAIN_FILE_TS)
    {
        if((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_DUAL_ES_MASK) == E_HVD_INIT_DUAL_ES_ENABLE)
        {
            pShm->u8SrcMode = E_HVD_SRC_MODE_TS_FILE_DUAL_ES;
        }
        else
        {
        pShm->u8SrcMode = E_HVD_SRC_MODE_TS_FILE;
    }
    }
    else
    {
        pShm->u8SrcMode = E_HVD_SRC_MODE_DTV;
    }

#if 1//From T4 and the later chips, QDMA can support the address more than MIU1 base.
    if(pCtrl->MemMap.u32CodeBufAddr >= HAL_MIU1_BASE)
    {
        pShm->u32FWBaseAddr = (pCtrl->MemMap.u32CodeBufAddr-HAL_MIU1_BASE) | 0x40000000; //Bit30 sel miu0/1
    }
    else
    {
        pShm->u32FWBaseAddr = pCtrl->MemMap.u32CodeBufAddr;
    }
    //printf("<DBG>QDMA Addr = %lx <<<<<<<<<<<<<<<<<<<<<<<<\n",pShm->u32FWBaseAddr);
#else
    u32Addr = pCtrl->MemMap.u32CodeBufAddr;
    if (u32Addr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        u32Addr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }
    pShm->u32FWBaseAddr = u32Addr;
#endif

    // RM only
#if HVD_ENABLE_RV_FEATURE
    if ((((pCtrl->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM)
        && (pCtrl->InitParams.pRVFileInfo != NULL))
    {
        MS_U32 i = 0;

        for (i = 0; i < HVD_RM_INIT_PICTURE_SIZE_NUMBER; i++)
        {
            pShm->pRM_PictureSize[i].u16Width = pCtrl->InitParams.pRVFileInfo->ulPicSizes_w[i];
            pShm->pRM_PictureSize[i].u16Height = pCtrl->InitParams.pRVFileInfo->ulPicSizes_h[i];
        }

        pShm->u8RM_Version = (MS_U8) pCtrl->InitParams.pRVFileInfo->RV_Version;
        pShm->u8RM_NumSizes = (MS_U8) pCtrl->InitParams.pRVFileInfo->ulNumSizes;
        u32Addr = pCtrl->MemMap.u32FrameBufAddr + pHVDHalContext->u32RV_VLCTableAddr;

        if (u32Addr >= pCtrl->MemMap.u32MIU1BaseAddr)
        {
            u32Addr -= pCtrl->MemMap.u32MIU1BaseAddr;
        }

        pShm->u32RM_VLCTableAddr = u32Addr;
    }
#endif

    if ((E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
     && (pCtrl->InitParams.pRVFileInfo != NULL))
    {
        pShm->pRM_PictureSize[0].u16Width = pCtrl->InitParams.pRVFileInfo->ulPicSizes_w[0];
        pShm->pRM_PictureSize[0].u16Height = pCtrl->InitParams.pRVFileInfo->ulPicSizes_h[0];
    }

    //if(pCtrl->InitParams.bColocateBBUMode)
    if(_stHVDPreSet[u8Idx].bColocateBBUMode)
    {
          pShm->u32ColocateBBUWritePtr = pShm->u32ColocateBBUReadPtr =  pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr;
    }

    HAL_HVD_EX_FlushMemory();

    return E_HVD_RETURN_SUCCESS;
}

HVD_Return HAL_HVD_EX_InitRegCPU(MS_U32 u32Id)
{
    MS_BOOL bInitRet = FALSE;

#if 0
    // check MVD power on
    if (_HVD_Read2Byte(REG_TOP_MVD) & (TOP_CKG_MHVD_DIS))
    {
        HVD_EX_MSG_INF("HVD warning: MVD is not power on before HVD init.\n");
        _HVD_WriteWordMask(REG_TOP_MVD, 0, TOP_CKG_MHVD_DIS);
        HVD_Delay_ms(1);
    }
    // Check VPU power on
    if (_HVD_Read2Byte(REG_TOP_VPU) & (TOP_CKG_VPU_DIS))
    {
        HVD_EX_MSG_INF("HVD warning: VPU is not power on before HVD init.\n");
        _HVD_WriteWordMask(REG_TOP_VPU, 0, TOP_CKG_VPU_DIS);
        HVD_Delay_ms(1);
    }
    // check HVD power on
    if (_HVD_Read2Byte(REG_TOP_HVD) & (TOP_CKG_HVD_DIS))
    {
        HVD_EX_MSG_INF("HVD warning: HVD is not power on before HVD init.\n");
        HAL_HVD_EX_PowerCtrl(TRUE);
        HVD_Delay_ms(1);
    }
#endif

    bInitRet = _HVD_EX_SetRegCPU(u32Id);

    if (!bInitRet)
    {
        return E_HVD_RETURN_FAIL;
    }

    bInitRet = HAL_HVD_EX_RstPTSCtrlVariable(u32Id);

    if (!bInitRet)
    {
        return E_HVD_RETURN_FAIL;
    }

    return E_HVD_RETURN_SUCCESS;
}

HVD_Return HAL_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    _stHVDPreSet[u8Idx].bColocateBBUMode = bEnable;

    return E_HVD_RETURN_SUCCESS;
}

HVD_Return HAL_HVD_EX_SetData(MS_U32 u32Id, HVD_SetData u32type, MS_U32 u32Data)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_BOOL bMVC = FALSE;
#if HVD_ENABLE_MVC
    bMVC = HAL_HVD_EX_CheckMVCID(u32Id);
#endif

    switch (u32type)
    {
    // share memory
        // switch
        case E_HVD_SDATA_FRAMEBUF_ADDR:
        {
            pShm->u32FrameBufAddr = u32Data;
            break;
        }
        case E_HVD_SDATA_FRAMEBUF_SIZE:
        {
            pShm->u32FrameBufSize = u32Data;
            break;
        }
        case E_HVD_SDATA_RM_PICTURE_SIZES:
        {
            HVD_memcpy((volatile void *) pShm->pRM_PictureSize, (void *) ((HVD_PictureSize *) u32Data),
                       HVD_RM_INIT_PICTURE_SIZE_NUMBER * sizeof(HVD_PictureSize));
            break;
        }
        case E_HVD_SDATA_ERROR_CODE:
        {
            pShm->u16ErrCode = (MS_U16) u32Data;
            break;
        }
        case E_HVD_SDATA_DISP_INFO_TH:
        {
            HVD_memcpy((volatile void *) &(pShm->DispThreshold), (void *) ((HVD_DISP_THRESHOLD *) u32Data),
                       sizeof(HVD_DISP_THRESHOLD));
            break;
        }
        case E_HVD_SDATA_FW_FLUSH_STATUS:
        {
            pShm->u8FlushStatus = (MS_U8)u32Data;
            break;
        }
        case E_HVD_SDATA_DMX_FRAMERATE:
        {
            pShm->u32DmxFrameRate = u32Data;
            break;
        }
        case E_HVD_SDATA_DMX_FRAMERATEBASE:
        {
            pShm->u32DmxFrameRateBase = u32Data;
            break;
        }
    // SRAM

    // Mailbox
        case E_HVD_SDATA_TRIGGER_DISP:     // HVD HI mbox 0
        {
            if (u32Data != 0)
            {
                pShm->bEnableDispCtrl   = TRUE;
                pShm->bIsTrigDisp       = TRUE;
            }
            else
            {
                pShm->bEnableDispCtrl   = FALSE;
            }

            break;
        }
        case E_HVD_SDATA_GET_DISP_INFO_START:
        {
            pShm->bSpsChange = FALSE;
            break;
        }
        case E_HVD_SDATA_VIRTUAL_BOX_WIDTH:
        {
            pShm->u32VirtualBoxWidth = u32Data;
            break;
        }
        case E_HVD_SDATA_VIRTUAL_BOX_HEIGHT:
        {
            pShm->u32VirtualBoxHeight = u32Data;
            break;
        }
        case E_HVD_SDATA_DISPQ_STATUS_VIEW:
        {
            if (pShm->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                //printf("DispFrame DqPtr: %d\n", u32Data);
                pShm->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_VIEW;
            }
            break;
        }
        case E_HVD_SDATA_DISPQ_STATUS_DISP:
        {
            if(!(pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide))
            {
                if (pShm->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_VIEW)
                {
                    //printf("DispFrame DqPtr: %ld\n", u32Data);
                    pShm->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_DISP;
                }
            }
            break;
        }
        case E_HVD_SDATA_DISPQ_STATUS_FREE:
        {
            if(pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
            {
                if (bMVC)
                {
                    if (pHVDHalContext->_stHVDStream[u8Idx].u32FreeData == 0xFFFF)
                    {
                        //ALOGE("R1: %x", u32Data);
                        pHVDHalContext->_stHVDStream[u8Idx].u32FreeData = u32Data;
                    }
                    else
                    {
                        //ALOGE("R2: %x", (u32Data << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32FreeData);
                        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RELEASE_DISPQ, (u32Data << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32FreeData);
                        //pShm->FreeQueue[pShm->u16FreeQWtPtr] = (u32Data << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32FreeData;
                        //pShm->u16FreeQWtPtr = (pShm->u16FreeQWtPtr + 1) % HVD_DISP_QUEUE_MAX_SIZE;
                        pHVDHalContext->_stHVDStream[u8Idx].u32FreeData = 0xFFFF;
                    }
                }
                else
                {
                    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RELEASE_DISPQ, u32Data);
                }
            }
            else
            {
                if (pShm->DispQueue[u32Data].u32Status == E_HVD_DISPQ_STATUS_VIEW)
                {
                    pShm->DispQueue[u32Data].u32Status = E_HVD_DISPQ_STATUS_FREE;
                }
            }
            break;
        }
        default:
            break;
    }

    HAL_HVD_EX_FlushMemory();

    return eRet;
}

MS_S64 HAL_HVD_EX_GetData_EX(MS_U32 u32Id, HVD_GetData eType)
{
    MS_S64 s64Ret = 0;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    HAL_HVD_EX_ReadMemory();

    switch (eType)
    {
        case E_HVD_GDATA_PTS_STC_DIFF:
            s64Ret = pShm->s64PtsStcDiff;
            break;
        default:
            break;
    }

    return s64Ret;
}

MS_U32 HAL_HVD_EX_GetData(MS_U32 u32Id, HVD_GetData eType)
{
    MS_U32 u32Ret = 0;
    //static MS_U64 u64pts_real = 0;
    MS_U64 u64pts_low = 0;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    HAL_HVD_EX_ReadMemory();

    if(pShm == NULL)
    {
        printf("########## VDEC patch for Debug ###########\n");
        return 0x0;
    }

    switch (eType)
    {
    // share memory
        // switch
        case E_HVD_GDATA_DISP_INFO_ADDR:
        {
            u32Ret = (MS_U32) (&pShm->DispInfo);
            break;
        }
        // report
        case E_HVD_GDATA_PTS:
        {
            u32Ret = pShm->DispFrmInfo.u32TimeStamp;
            break;
        }
        case E_HVD_GDATA_U64PTS:
        {
            u64pts_low = (MS_U64)(pShm->DispFrmInfo.u32TimeStamp);
            pHVDHalContext->u64pts_real = (MS_U64)(pShm->DispFrmInfo.u32ID_H);
            pHVDHalContext->u64pts_real = (pHVDHalContext->u64pts_real<<32)|u64pts_low;
            u32Ret = (MS_U32)(&(pHVDHalContext->u64pts_real));
            break;
        }
        case E_HVD_GDATA_DECODE_CNT:
        {
            u32Ret = pShm->u32DecodeCnt;
            break;
        }
        case E_HVD_GDATA_DATA_ERROR_CNT:
        {
            u32Ret = pShm->u32DataErrCnt;
            break;
        }
        case E_HVD_GDATA_DEC_ERROR_CNT:
        {
            u32Ret = pShm->u32DecErrCnt;
            break;
        }
        case E_HVD_GDATA_ERROR_CODE:
        {
            u32Ret = (MS_U32) (pShm->u16ErrCode);
            break;
        }
        case E_HVD_GDATA_VPU_IDLE_CNT:
        {
            u32Ret = pShm->u32VPUIdleCnt;
            break;
        }
        case E_HVD_GDATA_DISP_FRM_INFO:
        {
            u32Ret = (MS_U32) (&pShm->DispFrmInfo);
            break;
        }
        case E_HVD_GDATA_DEC_FRM_INFO:
        {
            u32Ret = (MS_U32) (&pShm->DecoFrmInfo);
            break;
        }
        case E_HVD_GDATA_ES_LEVEL:
        {
            u32Ret = (MS_U32) (_HVD_EX_GetESLevel(u32Id));
            break;
        }
#if HVD_ENABLE_MVC
        case E_HVD_GDATA_DISP_FRM_INFO_SUB:
        {
            u32Ret=  (MS_U32) (&(pShm->DispFrmInfo_Sub));
            break;
        }
        case E_HVD_GDATA_DEC_FRM_INFO_SUB:
        {
            u32Ret=  (MS_U32) (&(pShm->DecoFrmInfo_Sub));
            break;
        }
#endif

        // user data
        case E_HVD_GDATA_USERDATA_WPTR:
        {
            u32Ret = (MS_U32) (pShm->u32UserCCIdxWrtPtr);
            break;
        }
        case E_HVD_GDATA_USERDATA_IDX_TBL_ADDR:
        {
            u32Ret = (MS_U32) (pShm->u8UserCCIdx);
            break;
        }
        case E_HVD_GDATA_USERDATA_PACKET_TBL_ADDR:
        {
            u32Ret = (MS_U32) (pShm->u32UserCCBase);
            break;
        }
        case E_HVD_GDATA_USERDATA_PACKET_SIZE:
        {
            u32Ret = (MS_U32) (sizeof(DTV_BUF_type));
            break;
        }
        case E_HVD_GDATA_USERDATA_IDX_TBL_SIZE:
        {
            u32Ret = (MS_U32) (USER_CC_IDX_SIZE);
            break;
        }
        case E_HVD_GDATA_USERDATA_PACKET_TBL_SIZE:
        {
            u32Ret = (MS_U32) (USER_CC_DATA_SIZE);
            break;
        }
            // report - modes
        case E_HVD_GDATA_IS_SHOW_ERR_FRM:
        {
            u32Ret = pShm->ModeStatus.bIsShowErrFrm;
            break;
        }
        case E_HVD_GDATA_IS_REPEAT_LAST_FIELD:
        {
            u32Ret = pShm->ModeStatus.bIsRepeatLastField;
            break;
        }
        case E_HVD_GDATA_IS_ERR_CONCEAL:
        {
            u32Ret = pShm->ModeStatus.bIsErrConceal;
            break;
        }
        case E_HVD_GDATA_IS_SYNC_ON:
        {
            u32Ret = pShm->ModeStatus.bIsSyncOn;
            break;
        }
        case E_HVD_GDATA_IS_PLAYBACK_FINISH:
        {
            u32Ret = pShm->ModeStatus.bIsPlaybackFinish;
            break;
        }
        case E_HVD_GDATA_SYNC_MODE:
        {
            u32Ret = pShm->ModeStatus.u8SyncType;
            break;
        }
        case E_HVD_GDATA_SKIP_MODE:
        {
            u32Ret = pShm->ModeStatus.u8SkipMode;
            break;
        }
        case E_HVD_GDATA_DROP_MODE:
        {
            u32Ret = pShm->ModeStatus.u8DropMode;
            break;
        }
        case E_HVD_GDATA_DISPLAY_DURATION:
        {
            u32Ret = pShm->ModeStatus.s8DisplaySpeed;
            break;
        }
        case E_HVD_GDATA_FRC_MODE:
        {
            u32Ret = pShm->ModeStatus.u8FrcMode;
            break;
        }
        case E_HVD_GDATA_NEXT_PTS:
        {
            u32Ret = pShm->u32NextPTS;
            break;
        }
        case E_HVD_GDATA_DISP_Q_SIZE:
        {
            u32Ret = pShm->u16DispQSize;
            break;
        }
        case E_HVD_GDATA_DISP_Q_PTR:
        {
            u32Ret = (MS_U32) pHVDHalContext->_u16DispQPtr;
            break;
        }
        case E_HVD_GDATA_NEXT_DISP_FRM_INFO:
        {
            u32Ret = (MS_U32) _HVD_EX_GetNextDispFrame(u32Id);
            break;
        }
        case E_HVD_GDATA_REAL_FRAMERATE:
        {
            // return VPS/VUI timing info framerate, and 0 if timing info not exist
            u32Ret = pShm->u32RealFrameRate;
            break;
        }
        case E_HVD_GDATA_IS_ORI_INTERLACE_MODE:
            u32Ret=(MS_U32)pShm->DispInfo.u8IsOriginInterlace;
            break;
        case E_HVD_GDATA_FRM_PACKING_SEI_DATA:
            u32Ret=((MS_U32)(pShm->u32Frm_packing_arr_data_addr));
            break;
        case E_HVD_GDATA_TYPE_FRAME_MBS_ONLY_FLAG:
            u32Ret=((MS_U32)(pShm->u8FrameMbsOnlyFlag));
            break;

        // internal control
        case E_HVD_GDATA_IS_1ST_FRM_RDY:
        {
            u32Ret = pShm->bIs1stFrameRdy;
            break;
        }
        case E_HVD_GDATA_IS_I_FRM_FOUND:
        {
            u32Ret = pShm->bIsIFrmFound;
            break;
        }
        case E_HVD_GDATA_IS_SYNC_START:
        {
            u32Ret = pShm->bIsSyncStart;
            break;
        }
        case E_HVD_GDATA_IS_SYNC_REACH:
        {
            u32Ret = pShm->bIsSyncReach;
            break;
        }
        case E_HVD_GDATA_FW_VERSION_ID:
        {
            u32Ret = pShm->u32FWVersionID;
            break;
        }
        case E_HVD_GDATA_FW_IF_VERSION_ID:
        {
            u32Ret = pShm->u32FWIfVersionID;
            break;
        }
        case E_HVD_GDATA_BBU_Q_NUMB:
        {
            u32Ret = _HVD_EX_GetBBUQNumb(u32Id);
            break;
        }
        case E_HVD_GDATA_DEC_Q_NUMB:
        {
            u32Ret = pShm->u16DecQNumb;
            break;
        }
        case E_HVD_GDATA_DISP_Q_NUMB:
        {
            u32Ret = pShm->u16DispQNumb;
            break;
        }
        case E_HVD_GDATA_PTS_Q_NUMB:
        {
            u32Ret = _HVD_EX_GetPTSQNumb(u32Id);
            break;
        }
        case E_HVD_GDATA_FW_INIT_DONE:
        {
            u32Ret = pShm->bInitDone;
            break;
        }
            // debug
        case E_HVD_GDATA_SKIP_CNT:
        {
            u32Ret = pShm->u32SkipCnt;
            break;
        }
        case E_HVD_GDATA_GOP_CNT:
        {
            u32Ret = pShm->u32DropCnt;
            break;
        }
        case E_HVD_GDATA_DISP_CNT:
        {
            u32Ret = pShm->u32DispCnt;
            break;
        }
        case E_HVD_GDATA_DROP_CNT:
        {
            u32Ret = pShm->u32DropCnt;
            break;
        }
        case E_HVD_GDATA_DISP_STC:
        {
            u32Ret = pShm->u32DispSTC;
            break;
        }
        case E_HVD_GDATA_VSYNC_CNT:
        {
            u32Ret = pShm->u32VsyncCnt;
            break;
        }
        case E_HVD_GDATA_MAIN_LOOP_CNT:
        {
            u32Ret = pShm->u32MainLoopCnt;
            break;
        }

            // AVC
        case E_HVD_GDATA_AVC_LEVEL_IDC:
        {
            u32Ret = pShm->u16AVC_SPS_LevelIDC;
            break;
        }
        case E_HVD_GDATA_AVC_LOW_DELAY:
        {
            u32Ret = pShm->u8AVC_SPS_LowDelayHrdFlag;
            break;
        }
        case E_HVD_GDATA_AVC_VUI_DISP_INFO:
        {
            u32Ret = _HVD_EX_GetVUIDispInfo(u32Id);
            break;
        }
        case E_HVD_GDATA_FW_FLUSH_STATUS:
        {
            u32Ret = (MS_U32) (pShm->u8FlushStatus);
            break;
        }
        case E_HVD_GDATA_FW_CODEC_TYPE:
        {
            u32Ret = pShm->u32CodecType;
            break;
        }
        case E_HVD_GDATA_FW_ES_BUF_STATUS:
        {

            u32Ret = (MS_U32)pShm->u8ESBufStatus;
            break;
        }

    // SRAM

    // Mailbox
        case E_HVD_GDATA_FW_STATE: // HVD RISC MBOX 0 (esp. FW init done)
        {
            u32Ret = pShm->u32FwState;
            break;
        }
        case E_HVD_GDATA_IS_DISP_INFO_UNCOPYED:
        {
            u32Ret = pShm->bSpsChange;
            break;
        }
        case E_HVD_GDATA_IS_DISP_INFO_CHANGE:      // HVD RISC MBOX 1 (rdy only)
        {
            u32Ret = pShm->bSpsChange;
            break;
        }
        case E_HVD_GDATA_HVD_ISR_STATUS:   // HVD RISC MBOX 1 (value only)
        {
            HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

            if ((pCtrl->HVDISRCtrl.u32IntCount != pShm->u32IntCount) && pShm->u32FwInfo) // fetch ISR status
            {
                u32Ret = pShm->u32FwInfo;
                pCtrl->HVDISRCtrl.u32IntCount = pShm->u32IntCount;
            }
            break;
        }
        case E_HVD_GDATA_IS_FRAME_SHOWED:  // HVD HI mbox 0 ( showed: rdy cleared ; not show: rdy enable )
        {
            if (pShm->bIsTrigDisp) // not clear yet
            {
                u32Ret = FALSE;
            }
            else
            {
                u32Ret = TRUE;
            }
            break;
        }
        case E_HVD_GDATA_ES_READ_PTR:
        {
            u32Ret = _HVD_EX_GetESReadPtr(u32Id, FALSE);
            break;
        }
        case E_HVD_GDATA_ES_WRITE_PTR:
        {
            u32Ret = _HVD_EX_GetESWritePtr(u32Id);
            break;
        }
        case E_HVD_GDATA_BBU_READ_PTR:
        {
            u32Ret = _HVD_EX_GetBBUReadptr(u32Id);
            break;
        }
        case E_HVD_GDATA_BBU_WRITE_PTR:
        {
            HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
            if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
            {
                u32Ret = pHVDHalContext->u32VP8BBUWptr;
            }
            else
            {
                u32Ret = pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr;
            }
            break;
        }
        case E_HVD_GDATA_BBU_WRITE_PTR_FIRED:
        {
            HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

            u32Ret = pCtrl->u32BBUWptr_Fired;

            break;
        }
        case E_HVD_GDATA_VPU_PC_CNT:
        {
            u32Ret = _HVD_EX_GetPC();
            break;
        }
        case E_HVD_GDATA_ES_QUANTITY:
        {
            u32Ret=_HVD_EX_GetESQuantity(u32Id);
            break;
        }


    // FW def
        case E_HVD_GDATA_FW_MAX_DUMMY_FIFO:        // AVC: 256Bytes AVS: 2kB RM:???
            u32Ret = HVD_MAX3(HVD_FW_AVC_DUMMY_FIFO, HVD_FW_AVS_DUMMY_FIFO, HVD_FW_RM_DUMMY_FIFO);
            break;

        case E_HVD_GDATA_FW_AVC_MAX_VIDEO_DELAY:
            u32Ret = HVD_FW_AVC_MAX_VIDEO_DELAY;
            break;
        case E_HVD_GDATA_FW_BBU_TOTAL_TBL_ENTRY:
            u32Ret = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNumTH;
            break;
        case E_HVD_GDATA_FW_BBU_TBL_ENTRY_NUMB:
            u32Ret = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum;
            break;
        case E_HVD_GDATA_FW_PTS_TOTAL_ENTRY_NUMB:
            u32Ret = MAX_PTS_TABLE_SIZE;
            break;
        case E_HVD_GDATA_FW_DUMMY_WRITE_ADDR:
            u32Ret = pShm->u32HVD_DUMMY_WRITE_ADDR;
            break;
        case E_HVD_GDATA_FW_DS_BUF_ADDR:
            u32Ret = pShm->u32HVD_DYNAMIC_SCALING_ADDR;
            break;
        case E_HVD_GDATA_FW_DS_BUF_SIZE:
            //u32Ret = HVD_DYNAMIC_SCALING_SIZE;
            // ----------------------- yi-chun.pan: for Dynamic Scaling 3k/6k issue ----------20111213---
            // ----------------------- modify DRV and AP(SN/MM) first, and then update fw --------------
            u32Ret = (1024 * 3);
            break;
        case E_HVD_GDATA_FW_DS_VECTOR_DEPTH:
            u32Ret = HVD_DYNAMIC_SCALING_DEPTH;
            break;
        case E_HVD_GDATA_FW_DS_INFO_ADDR:
            u32Ret = pShm->u32HVD_SCALER_INFO_ADDR;
            break;
        case E_HVD_GDATA_FW_DS_IS_ENABLED:
        {
            if (pShm->bDSIsRunning)
            {
                u32Ret = TRUE;
            }
            else
            {
                u32Ret = FALSE;
            }
            break;
        }
        case E_HVD_GDATA_TYPE_IS_LEAST_DISPQ_SIZE:
            u32Ret = ((MS_U32)(pShm->bIsLeastDispQSize));
            break;
        case E_HVD_GDATA_FIELD_PIC_FLAG:
            u32Ret = ((MS_U32)(pShm->u8FieldPicFlag));
            break;
        case E_HVD_GDATA_FW_VSYNC_BRIDGE_ADDR:
            u32Ret = pShm->u32VSYNC_BRIGE_SHM_ADDR;
            break;
        case E_HVD_GDATA_TS_SEAMLESS_STATUS:
            u32Ret = pShm->u32SeamlessTSStatus;
            break;
        case E_HVD_GDATA_HVD_HW_MAX_PIXEL:
            u32Ret = (MS_U32)(((MS_U64)HVD_HW_MAX_PIXEL)/1000);
            break;
        default:
            break;
    }
    return u32Ret;
}

HVD_Return HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    MS_U32 u32Cmd = (MS_U32) eUsrCmd;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    _HAL_HVD_Entry();

    // check if old SVD cmds
    if (u32Cmd < E_HVD_CMD_SVD_BASE)
    {
        HVD_EX_MSG_ERR("Old SVD FW cmd(%lx %lx) used in HVD.\n", u32Cmd, u32CmdArg);

        _HAL_HVD_Return(E_HVD_RETURN_INVALID_PARAMETER);
    }

    if(u32Cmd == E_HVD_CMD_ENABLE_DISP_OUTSIDE)
    {
        pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide = (MS_BOOL)u32CmdArg;
    }

#if HVD_ENABLE_MVC
    if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
    {
        if (HAL_HVD_EX_CheckMVCID(u32Id) && u32Cmd == E_HVD_CMD_FLUSH)
        {
            pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = 0;
        }
    }
#endif

    HVD_EX_MSG_DBG("cmd=0x%lx, arg=0x%lx\n", u32Cmd, u32CmdArg);

    eRet = _HVD_EX_SendCmd(u32Id, u32Cmd, u32CmdArg);

    _HAL_HVD_Return(eRet);
}

HVD_Return HAL_HVD_EX_DeInit(MS_U32 u32Id)
{
    HVD_Return eRet         = E_HVD_RETURN_FAIL;
    MS_U8 u8Idx             = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U32 u32Timeout       = HVD_GetSysTime_ms() + 3000;

#if HVD_ENABLE_TIME_MEASURE
    MS_U32 ExitTimeCnt = 0;
    ExitTimeCnt = HVD_GetSysTime_ms();
#endif

    pCtrl->MemMap.u32CodeBufVAddr = MS_PA2KSEG1((MS_U32)pCtrl->MemMap.u32CodeBufAddr);

    eRet = HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_PAUSE, 0);

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        HVD_EX_MSG_ERR("HVD fail to PAUSE %d\n", eRet);
    }

    eRet = HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_STOP, 0);

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        HVD_EX_MSG_ERR("HVD fail to STOP %d\n", eRet);
    }

    // check FW state to make sure it's STOP DONE
    while (E_HVD_FW_STOP_DONE != (HVD_FW_State) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_STATE))
    {
        if (HVD_GetSysTime_ms() > u32Timeout)
        {
            HVD_EX_MSG_ERR("FW stop timeout, pc = 0x%lx\n", HAL_VPU_EX_GetProgCnt());

            //return E_HVD_RETURN_TIMEOUT;
            eRet =  E_HVD_RETURN_TIMEOUT;
            break;
        }
    }

    VPU_EX_FWCodeCfg       fwCfg;
    VPU_EX_TaskInfo        taskInfo;
    VPU_EX_NDecInitPara    nDecInitPara;

    nDecInitPara.pFWCodeCfg = &fwCfg;
    nDecInitPara.pTaskInfo = &taskInfo;

    fwCfg.u32DstAddr = pCtrl->MemMap.u32CodeBufVAddr;
    fwCfg.u8SrcType  = E_HVD_FW_INPUT_SOURCE_NONE;

    taskInfo.u32Id = u32Id;
    taskInfo.eDecType = E_VPU_EX_DECODER_HVD;
    taskInfo.eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        taskInfo.eSrcType = E_VPU_EX_INPUT_FILE;
    }
    else
    {
        taskInfo.eSrcType = E_VPU_EX_INPUT_TSP;
    }

    if(HAL_VPU_EX_TaskDelete(u32Id, &nDecInitPara) != TRUE)
    {
       HVD_EX_MSG_ERR("HAL_VPU_EX_TaskDelete fail\n");
    }

    /* clear es buffer */
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
    {
        //printf("Clear ES buffer\n");

        memset((void *) pCtrl->MemMap.u32BitstreamBufVAddr, 0, MIN(128, pCtrl->MemMap.u32BitstreamBufSize));
    }

    //_HAL_HVD_MutexDelete();

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_DBG("HVD Stop Time(Wait FW):%d\n", HVD_GetSysTime_ms() - ExitTimeCnt);
#endif

    pHVDHalContext->_stHVDStream[u8Idx].bUsed = FALSE;

    // reset bbu wptr
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        if(TRUE == HAL_VPU_EX_HVDInUsed())
        {
            if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))//apple
            {
                _HVD_EX_SetBBUWriteptr(u32Id, _HVD_EX_GetBBUReadptr(u32Id));
                pHVDHalContext->u32VP8BBUWptr = _HVD_EX_GetBBUReadptr(u32Id);
            }
            else
            {
                if(!_stHVDPreSet[u8Idx].bColocateBBUMode)
                {
                    _HVD_EX_SetBBUWriteptr(u32Id, _HVD_EX_GetBBUReadptr(u32Id));
                }
                pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr = _HVD_EX_GetBBUReadptr(u32Id);
            }
        }
        else
        {
            pHVDHalContext->_stHVDStream[0].u32BBUWptr = 0; //main
            pHVDHalContext->_stHVDStream[1].u32BBUWptr = 0; //sub
            pHVDHalContext->u32VP8BBUWptr = 0; //VP8
            if (E_HVD_INIT_HW_AVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
            {
                if(!_stHVDPreSet[u8Idx].bColocateBBUMode)
                {
                    _HVD_EX_ResetMainSubBBUWptr(u32Id);
                }
            }
            else
            {
                _HVD_EX_ResetMainSubBBUWptr(u32Id);
            }
        }
    }

    _stHVDPreSet[u8Idx].bColocateBBUMode = FALSE;

    if(pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.bEnable)
    {
        if(pCtrl->MemMap.u32FrameBufAddr >= HAL_MIU1_BASE)
        {
            _HAL_HVD_Entry();
            HAL_HVD_MVDMiuClientSel(1);
            _HAL_HVD_Release();
        }
        else
        {
            _HAL_HVD_Entry();
            HAL_HVD_MVDMiuClientSel(0);
            _HAL_HVD_Release();
        }
    }

    HVD_EX_MSG_DBG("success\n");

    return eRet;
}

HVD_Return HAL_HVD_EX_PushPacket(MS_U32 u32Id, HVD_BBU_Info *pInfo)
{
    HVD_Return eRet = E_HVD_RETURN_UNSUPPORTED;
    MS_U32 u32Addr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = NULL;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    //if (E_HVD_INIT_HW_VP8 != (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)) // VP8 PTS table is not ready yet
    {
        eRet = _HVD_EX_UpdatePTSTable(u32Id, pInfo);

        if (E_HVD_RETURN_SUCCESS != eRet)
        {
            return eRet;
        }
    }

    //printf(">>> halHVD pts,idH = %lu, %lu\n", pInfo->u32TimeStamp, pInfo->u32ID_H);    //STS input

    //T9: for 128 bit memory. BBU need to get 2 entry at a time.
    if (E_HVD_INIT_HW_VP8 != (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        eRet = _HVD_EX_UpdateESWptr(u32Id, 0, 0);

        if (E_HVD_RETURN_SUCCESS != eRet)
        {
            return eRet;
        }
    }

    u32Addr = pInfo->u32Staddr;

    if (pInfo->bRVBrokenPacket)
    {
        u32Addr = pInfo->u32Staddr | BIT(HVD_RV_BROKENBYUS_BIT);
    }

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))        // VP8
    {
        eRet = _HVD_EX_UpdateESWptr_VP8(u32Id, pInfo->u32Staddr, pInfo->u32Length, pInfo->u32Staddr2, pInfo->u32Length2);
    }
    else
    {
        eRet = _HVD_EX_UpdateESWptr(u32Id, u32Addr, pInfo->u32Length);
    }

    if (E_HVD_RETURN_SUCCESS != eRet)
    {
        return eRet;
    }

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        //eRet = _HVD_EX_UpdateESWptr_VP8(u32Id, 0, 0, 0, 0);
        eRet = _HVD_EX_UpdateESWptr_VP8(u32Id, pInfo->u32Staddr, 0, pInfo->u32Staddr2, 0);

        if (E_HVD_RETURN_SUCCESS != eRet)
        {
            return eRet;
        }
    }

    pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt += pInfo->u32Length;

    // do not add local pointer
    if ((pCtrl->MemMap.u32DrvProcessBufSize != 0) && (pCtrl->MemMap.u32DrvProcessBufAddr != 0))
    {
        MS_U32 u32PacketStAddr = pInfo->u32Staddr + pCtrl->MemMap.u32BitstreamBufAddr;

        if (!((pCtrl->MemMap.u32DrvProcessBufAddr <= u32PacketStAddr) &&
              (u32PacketStAddr <
               (pCtrl->MemMap.u32DrvProcessBufAddr + pCtrl->MemMap.u32DrvProcessBufSize))))
        {
            pCtrl->LastNal.u32NalAddr = pInfo->u32Staddr;
            pCtrl->LastNal.u32NalSize = pInfo->u32AllocLength;
        }
        else
        {
            //null packet
            pCtrl->LastNal.u32NalAddr = pInfo->u32OriPktAddr;
            pCtrl->LastNal.u32NalSize = 0;
        }
    }
    else
    {
        pCtrl->LastNal.u32NalAddr = pInfo->u32Staddr;
        pCtrl->LastNal.u32NalSize = pInfo->u32AllocLength;
    }

    pCtrl->LastNal.bRVBrokenPacket = pInfo->bRVBrokenPacket;
    pCtrl->u32BBUPacketCnt++;

    return eRet;
}

void HAL_HVD_EX_EnableISR(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    if (bEnable)
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), 0, HVD_REG_RISC_ISR_MSK);
    }
    else
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_RISC_ISR_MSK, HVD_REG_RISC_ISR_MSK);
    }
}

void HAL_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    if (bEnable)
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_RISC_ISR_FORCE, HVD_REG_RISC_ISR_FORCE);
    }
    else
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), 0, HVD_REG_RISC_ISR_FORCE);
    }
}

void HAL_HVD_EX_SetClearISR(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_RISC_ISR_CLR, HVD_REG_RISC_ISR_CLR);
}

MS_BOOL HAL_HVD_EX_IsISROccured(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    return (MS_BOOL) (_HVD_Read2Byte(HVD_REG_RISC_MBOX_RDY(u32RB)) & HVD_REG_RISC_ISR_VALID);
}

MS_BOOL HAL_HVD_EX_IsEnableISR(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    if (_HVD_Read2Byte(HVD_REG_RISC_MBOX_CLR(u32RB)) & HVD_REG_RISC_ISR_MSK)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL HAL_HVD_EX_IsAlive(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if (pCtrl)
    {
        if ((pCtrl->LivingStatus.u32DecCnt == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DECODE_CNT)) &&
            (pCtrl->LivingStatus.u32SkipCnt == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_SKIP_CNT)) &&
            (pCtrl->LivingStatus.u32IdleCnt == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_VPU_IDLE_CNT)) &&
            (pCtrl->LivingStatus.u32MainLoopCnt == HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_MAIN_LOOP_CNT)))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_HVD_EX_RstPTSCtrlVariable(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U8 u8Idx             = _HVD_EX_GetStreamIdx(u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        HAL_HVD_EX_ReadMemory();

        pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt = pShm->u32PTStableByteCnt;
        pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr = _HVD_EX_GetPTSTableWptr(u32Id);

        HVD_EX_MSG_DBG("PTS table: WptrAddr:%lx RptrAddr:%lx ByteCnt:%lx PreWptr:%lx\n",
            pShm->u32PTStableWptrAddr, pShm->u32PTStableRptrAddr, pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt, pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr);
    }

    return TRUE;
}

MS_BOOL HAL_HVD_EX_FlushRstShareMem(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = NULL;
    MS_U8 u8Idx             = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32Data;
    pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    memset(&pShm->DecoFrmInfo, 0, sizeof(HVD_Frm_Information));

    HAL_HVD_EX_FlushMemory();
    if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
    {
        u32Data = _HVD_EX_GetESReadPtr(u32Id, FALSE);
        pCtrl->LastNal.u32NalAddr = u32Data;
        pCtrl->LastNal.u32NalSize = 0;
    }
    return TRUE;
}

void HAL_HVD_EX_UartSwitch2FW(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_VD_MHEG5, REG_TOP_UART_SEL_0_MASK);
    }
    else
    {
#if defined (__aeon__)
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_MHEG5, REG_TOP_UART_SEL_0_MASK);
#else // defined (__mips__)
        _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_PIU_0, REG_TOP_UART_SEL_0_MASK);
#endif
    }
}

MS_U32 HAL_HVD_EX_GetData_Dbg(MS_U32 u32Addr)
{
    return 0;
}

void HAL_HVD_EX_SetData_Dbg(MS_U32 u32Addr, MS_U32 u32Data)
{
    return;
}

MS_U16 HAL_HVD_EX_GetCorretClock(MS_U16 u16Clock)
{
    //if( u16Clock == 0 )
    return pHVDHalContext->u32HVDClockType;                 //140;
    //if(  )
}

void HAL_HVD_EX_UpdateESWptr_Fire(MS_U32 u32Id)
{
    MS_BOOL bBitMIU1 = FALSE;
    MS_BOOL bCodeMIU1 = FALSE;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U32 u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU_DRAM_ST_ADDR;

#if HVD_ENABLE_MVC
    if(HAL_HVD_EX_CheckMVCID(u32Id))
    {
        // if MVC_BBU_ADDR and HVD_BBU_ADDR are different, we need to add MVC_BBU_DRAM_ST_ADDR and MVC_BBU2_DRAM_ST_ADDR in share memory
        u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU_DRAM_ST_ADDR;  //pShm->u32MVC_BBU_DRAM_ST_ADDR;
        if(E_VDEC_EX_SUB_VIEW == HAL_HVD_EX_GetView(u32Id))
        {
            u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU2_DRAM_ST_ADDR;  //pShm->u32MVC_BBU2_DRAM_ST_ADDR;
        }
    }
#endif /// HVD_ENABLE_MVC


    if (pCtrl->MemMap.u32CodeBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        bCodeMIU1 = TRUE;
    }

    if (pCtrl->MemMap.u32BitstreamBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        bBitMIU1 = TRUE;
    }

    if (bBitMIU1 != bCodeMIU1)
    {
#if HVD_ENABLE_BDMA_2_BITSTREAMBUF
        BDMA_Result bdmaRlt;
        MS_U32 u32DstAdd = 0, u32SrcAdd = 0, u32tabsize = 0;

        u32DstAdd = pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->u32BBUTblInBitstreamBufAddr;
        u32SrcAdd = pCtrl->MemMap.u32CodeBufAddr + u32BBU_DRAM_ST_ADDR;
        u32tabsize = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum << 3;

        bdmaRlt = HVD_dmacpy(u32DstAdd, u32SrcAdd, u32tabsize);

        if (E_BDMA_OK != bdmaRlt)
        {
            HVD_EX_MSG_ERR("MDrv_BDMA_MemCopy fail ret=%x!\n", bdmaRlt);
        }
#else
        MS_U32 u32DstAdd = 0, u32SrcAdd = 0, u32tabsize = 0;

        u32DstAdd = pCtrl->MemMap.u32BitstreamBufVAddr + pCtrl->u32BBUTblInBitstreamBufAddr;
        u32SrcAdd = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + u32BBU_DRAM_ST_ADDR);
        u32tabsize = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum << 3;

        HVD_memcpy(u32DstAdd, u32SrcAdd, u32tabsize);
#endif
    }

    //HVD_EX_MSG_DBG("%lu st:%lx size:%lx BBU: %lu\n", pCtrl->u32BBUPacketCnt, pCtrl->LastNal.u32NalAddr, pCtrl->LastNal.u32NalSize, _stHVDStream[u8Idx].u32BBUWptr);

    HAL_HVD_EX_FlushMemory();

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        _HVD_EX_SetBBUWriteptr(u32Id, HVD_LWORD(pHVDHalContext->u32VP8BBUWptr));
        pCtrl->u32BBUWptr_Fired = pHVDHalContext->u32VP8BBUWptr;
    }
    else
    {
    _HVD_EX_SetBBUWriteptr(u32Id, HVD_LWORD(pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr));

    pCtrl->u32BBUWptr_Fired = pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr;
    }
}

void HAL_HVD_EX_MVD_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_MVD, 0, TOP_CKG_MHVD_DIS);
        _HVD_WriteWordMask(REG_TOP_MVD2, 0, TOP_CKG_MHVD2_DIS);
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_MVD, TOP_CKG_MHVD_DIS, TOP_CKG_MHVD_DIS);
        _HVD_WriteWordMask(REG_TOP_MVD2, TOP_CKG_MHVD2_DIS, TOP_CKG_MHVD2_DIS);
    }
}

void HAL_HVD_EX_Dump_FW_Status(MS_U32 u32Id)
{
    MS_U32 tmp1 = 0;
    MS_U32 tmp2 = 0;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    HAL_HVD_EX_ReadMemory();

    _HVD_EX_MBoxRead(u32Id, HAL_HVD_CMD_MBOX, &tmp1);
    _HVD_EX_MBoxRead(u32Id, HAL_HVD_CMD_ARG_MBOX, &tmp2);

    if (u32UartCtrl & E_HVD_UART_CTRL_DBG)
    {
        MS_U32 u32Tmp = u32UartCtrl;

        HVD_EX_MSG_DBG("\n");
        u32UartCtrl = 0; // turn off debug message to prevent other function prints
        printf("\tSystime=%lu, FWVersionID=0x%lx, FwState=0x%lx, ErrCode=0x%lx, ProgCnt=0x%lx\n",
            HVD_GetSysTime_ms(), pShm->u32FWVersionID, pShm->u32FwState, (MS_U32) pShm->u16ErrCode, HAL_VPU_EX_GetProgCnt());

        printf("\tTime: DispSTC=%lu, DispT=%lu, DecT=%lu, CurrentPts=%lu, Last Cmd=0x%lx, Arg=0x%lx, Rdy1=0x%lx, Rdy2=0x%lx\n",
                pShm->u32DispSTC, pShm->DispFrmInfo.u32TimeStamp,
                pShm->DecoFrmInfo.u32TimeStamp, pShm->u32CurrentPts, tmp1, tmp2,
                (MS_U32) _HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_MBOX), (MS_U32) _HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_ARG_MBOX));

        printf("\tFlag: InitDone=%d, SpsChange=%d, IsIFrmFound=%d, 1stFrmRdy=%d, SyncStart=%d, SyncReach=%d\n",
                    pShm->bInitDone, pShm->bSpsChange, pShm->bIsIFrmFound,
                pShm->bIs1stFrameRdy, pShm->bIsSyncStart, pShm->bIsSyncReach);

        printf("\tQueue: BBUQNumb=%lu, DecQNumb=%d, DispQNumb=%d, ESR=%lu, ESRfromFW=%lu, ESW=%lu, ESLevel=%lu\n",
                _HVD_EX_GetBBUQNumb(u32Id), pShm->u16DecQNumb, pShm->u16DispQNumb,
                _HVD_EX_GetESReadPtr(u32Id, TRUE), pShm->u32ESReadPtr, _HVD_EX_GetESWritePtr(u32Id),
                _HVD_EX_GetESLevel(u32Id));

        printf("\tCounter: DecodeCnt=%lu, DispCnt=%lu, DataErrCnt=%lu, DecErrCnt=%lu, SkipCnt=%lu, DropCnt=%lu, idle=%lu, MainLoopCnt=%lu, VsyncCnt=%lu\n",
                pShm->u32DecodeCnt, pShm->u32DispCnt, pShm->u32DataErrCnt,
                pShm->u32DecErrCnt, pShm->u32SkipCnt, pShm->u32DropCnt,
                pShm->u32VPUIdleCnt, pShm->u32MainLoopCnt, pShm->u32VsyncCnt);
        printf
            ("\tMode: ShowErr=%d, RepLastField=%d, SyncOn=%d, FileEnd=%d, Skip=%d, Drop=%d, DispSpeed=%d, FRC=%d, BlueScreen=%d, FreezeImg=%d, 1Field=%d\n",
         pShm->ModeStatus.bIsShowErrFrm, pShm->ModeStatus.bIsRepeatLastField,
         pShm->ModeStatus.bIsSyncOn, pShm->ModeStatus.bIsPlaybackFinish,
         pShm->ModeStatus.u8SkipMode, pShm->ModeStatus.u8DropMode,
         pShm->ModeStatus.s8DisplaySpeed, pShm->ModeStatus.u8FrcMode,
         pShm->ModeStatus.bIsBlueScreen, pShm->ModeStatus.bIsFreezeImg,
         pShm->ModeStatus.bShowOneField);

        u32UartCtrl = u32Tmp; // recover debug level
    }
}

void HAL_HVD_EX_GetBBUEntry(MS_U32 u32Id, HVD_EX_Drv_Ctrl *pDrvCtrl, MS_U32 u32Idx, MS_U32 *u32NalOffset, MS_U32 *u32NalSize)
{
    MS_U8 *u32Addr = NULL;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (u32Idx >= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum)
    {
        return;
    }

    u32Addr = (MS_U8 *)(MsOS_PA2KSEG1(pDrvCtrl->MemMap.u32CodeBufAddr + pShm->u32HVD_BBU_DRAM_ST_ADDR + (u32Idx << 3)));

    *u32NalSize = *(u32Addr + 2) & 0x1f;
    *u32NalSize <<= 8;
    *u32NalSize |= *(u32Addr + 1) & 0xff;
    *u32NalSize <<= 8;
    *u32NalSize |= *(u32Addr) & 0xff;

    *u32NalOffset = ((MS_U32) (*(u32Addr + 2) & 0xe0)) >> 5;
    *u32NalOffset |= ((MS_U32) (*(u32Addr + 3) & 0xff)) << 3;
    *u32NalOffset |= ((MS_U32) (*(u32Addr + 4) & 0xff)) << 11;
    *u32NalOffset |= ((MS_U32) (*(u32Addr + 5) & 0xff)) << 19;
}

void HAL_HVD_EX_Dump_BBUs(MS_U32 u32Id, HVD_EX_Drv_Ctrl *pDrvCtrl, MS_U32 u32StartIdx, MS_U32 u32EndIdx, MS_BOOL bShowEmptyEntry)
{
    MS_U32 u32CurIdx = 0;
    MS_BOOL bFinished = FALSE;
    MS_U32 u32NalOffset = 0;
    MS_U32 u32NalSize = 0;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

    if ((u32StartIdx >= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum) || (u32EndIdx >= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum))
    {
        return;
    }

    u32CurIdx = u32StartIdx;

    do
    {
        if (u32CurIdx == u32EndIdx)
        {
            bFinished = TRUE;
        }

        HAL_HVD_EX_GetBBUEntry(u32Id, pDrvCtrl, u32CurIdx, &u32NalOffset, &u32NalSize);

        if ((bShowEmptyEntry == FALSE) || (bShowEmptyEntry && (u32NalOffset == 0) && (u32NalSize == 0)))
        {
            HVD_EX_MSG_DBG("HVD BBU Entry: Idx:%lu Offset:%lx Size:%lx\n", u32CurIdx, u32NalOffset, u32NalSize);
        }

        u32CurIdx++;

        if (u32CurIdx >= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum)
        {
            u32CurIdx %= pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum;
        }
    } while (bFinished == TRUE);
}

void HAL_HVD_EX_Dump_HW_Status(MS_U32 u32Num)
{
    MS_U32 i = 0;
    MS_U32 value = 0;

    if (u32UartCtrl & E_HVD_UART_CTRL_DBG)
    {
        HVD_EX_MSG_DBG("\n");

    for (i = 0; i <= u32Num; i++)
    {
        _HVD_Write2Byte(HVD_REG_DEBUG_SEL, i);
        value = _HVD_Read2Byte(HVD_REG_DEBUG_DAT_L);
        value |= ((MS_U32) _HVD_Read2Byte(HVD_REG_DEBUG_DAT_H)) << 16;

        if (value == 0)
        {
            break;
        }

            printf(" %08lx", value);

        if (((i % 8) + 1) == 8)
        {
                printf(" |%lu\n", i + 1);
        }
    }

        printf("\nHVD Dump HW status End: total number:%lu\n", i);
    }
}

void HAL_HVD_EX_SetMiuBurstLevel(HVD_EX_Drv_Ctrl *pDrvCtrl, HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl)
{
    if (pDrvCtrl)
    {
        pDrvCtrl->Settings.u32MiuBurstLevel = (MS_U32) eMiuBurstCntCtrl;
    }
}

#if HVD_ENABLE_MVC
MS_BOOL HAL_HVD_EX_CheckMVCID(MS_U32 u32Id)
{
    return  ( E_HAL_VPU_MVC_STREAM_BASE == (0xFF & u32Id) );
}

VDEC_EX_View HAL_HVD_EX_GetView(MS_U32 u32Id)
{
    if( (0xFF & (u32Id >> 8)) == 0x10)
        return  E_VDEC_EX_MAIN_VIEW;
    else
        return E_VDEC_EX_SUB_VIEW;
}
#endif ///HVD_ENABLE_MVC

void HAL_HVD_EX_SpareBandwidth(MS_U32 u32Id)    //// For MVC
{
    //HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_QUART_PIXEL, TRUE);
    //HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_DIS_DBF, TRUE);
    return;
}


MS_BOOL HAL_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id, MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate)
{
    HVD_EX_MSG_DBG("%s w:%d, h:%d, fr:%ld, MAX:%lld\n", __FUNCTION__,
                    u16HSize, u16VSize, u32FrmRate, HVD_HW_MAX_PIXEL);
    return (((MS_U64)u16HSize*(MS_U64)u16VSize*(MS_U64)u32FrmRate) <= HVD_HW_MAX_PIXEL);
}

MS_U32 HAL_HVD_EX_GetDispFrmNum(MS_U32 u32Id)
{
#if 1
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U16 u16QNum = pShm->u16DispQNumb;
    MS_U16 u16QPtr = pShm->u16DispQPtr;
//    MS_U16 u16QSize = pShm->u16DispQSize;
    //static volatile HVD_Frm_Information *pHvdFrm = NULL;
    MS_U32 u32DispQNum = 0;
#if HVD_ENABLE_MVC
    MS_BOOL bMVC = HAL_HVD_EX_CheckMVCID(u32Id);

    if(bMVC)
    {
#if 0
        if (u16QNum > HVD_DISPQ_PREFETCH_COUNT*3)
        {
            u16QNum = HVD_DISPQ_PREFETCH_COUNT*3;
        }
#endif

        //printf("OQ:%d,DQ:%d.\n",pShm->u16DispQNumb,pShm->u16DecQNumb);
        //search the next frame to display
        while (u16QNum > 0)
        {
            //printf("Pr:%d,%d.[%ld,%ld,%ld,%ld].\n",u16QPtr,u16QNum,pShm->DispQueue[u16QPtr].u32Status,pShm->DispQueue[u16QPtr+1].u32Status,
            //                pShm->DispQueue[u16QPtr+2].u32Status,pShm->DispQueue[u16QPtr+3].u32Status);
            pHVDHalContext->pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

            //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                /// For MVC. Output views after the pair of (base and depend) views were decoded.
                /// Check the depned view was initial when Output the base view.
                if((u16QPtr%2) == 0)
                {
                    volatile HVD_Frm_Information *pHvdFrm_sub = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr+1];
                    //if(pHvdFrm_sub->u32Status != E_HVD_DISPQ_STATUS_INIT)
                    if(pHvdFrm_sub->u32Status == E_HVD_DISPQ_STATUS_NONE)
                    {
                        ///printf("[MVC] %d is not E_HVD_DISPQ_STATUS_INIT (%ld).\n",u16QPtr+1,pHvdFrm_sub->u32Status);
                        ///printf("Return NULL.\n");
                        break;
                    }
                }
                u32DispQNum++;
            }

            u16QNum--;
            //go to next frame in the dispQ
            u16QPtr++;

            if (u16QPtr >= pShm->u16DispQSize)
            {
                u16QPtr -= pShm->u16DispQSize;        //wrap to the begin
            }
        }
    }
    else
#endif ///HVD_ENABLE_MVC
    {
#if 0
        if (u16QNum > HVD_DISPQ_PREFETCH_COUNT)
        {
            u16QNum = HVD_DISPQ_PREFETCH_COUNT;
        }
#endif
//        printf("Q: %d %d %d\n", u16QNum, u16QPtr, u16QSize);
        //search the next frame to display
        while (u16QNum != 0)
        {
            pHVDHalContext->pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

//            printf("Q2[%d]: %ld\n", u16QPtr, pShm->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                u32DispQNum++;
            }

            u16QNum--;
            //go to next frame in the dispQ
            u16QPtr++;

            if (u16QPtr == pShm->u16DispQSize)
            {
                u16QPtr = 0;        //wrap to the begin
            }
        }
    }

    //printf("dispQnum = %ld, pShm->u16DispQNumb = %d\n", u32DispQNum, pShm->u16DispQNumb);
    return u32DispQNum;
#else
    HVD_ShareMem *pShm = (HVD_ShareMem *) _HVD_EX_GetShmAddr(u32Id);
    return pShm->u16DispQNumb;
#endif
}

void HAL_HVD_EX_SetHwRegBase(MS_U32 u32Id, MS_U32 u32ModeFlag)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    pHVDHalContext->_stHVDStream[u8Idx].u32RegBase =
        ((u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC) ? REG_EVD_BASE : REG_HVD_BASE;
}

#if SUPPORT_EVD
void HAL_EVD_EX_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, ~TOP_CKG_EVD_PPU_DIS, TOP_CKG_EVD_PPU_DIS);
        _HVD_WriteWordMask(REG_TOP_CKG_EVD, ~TOP_CKG_EVD_DIS, TOP_CKG_EVD_DIS);
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_DIS, TOP_CKG_EVD_PPU_DIS);
        _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_DIS, TOP_CKG_EVD_DIS);
    }

    switch (pHVDHalContext->u32EVDClockType)
    {
        case 384:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_480MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_384MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 320:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_384MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_320MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 288:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_288MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_288MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 240:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_240MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_240MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 218:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_218MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_218MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 192:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_192MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_192MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 160:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_160MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_160MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        default:
        {
            _HVD_WriteByteMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_480MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteByteMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_384MHZ, TOP_CKG_EVD_MASK);
            break;
        }
    }

    return;
}

static MS_BOOL HAL_EVD_EX_DeinitHW(void)
{
    MS_U16 u16Timeout = 1000;

    _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_SWRST, EVD_REG_RESET_SWRST);

    while (u16Timeout)
    {
        if ((_HVD_Read2Byte(EVD_REG_RESET) & (EVD_REG_RESET_SWRST_FIN)) == (EVD_REG_RESET_SWRST_FIN))
        {
            break;
        }
        u16Timeout--;
    }

    HAL_EVD_EX_PowerCtrl(FALSE);

    return TRUE;
}
#endif
MS_BOOL HAL_HVD_EX_GetSupport2ndMVOPInterface(void)
{
    return TRUE;
}
void HAL_HVD_EX_SetBufferAddr(MS_U32 u32Id)
{
    _HVD_EX_SetBufferAddr(u32Id);
}


MS_BOOL HAL_HVD_EX_Is_RM_Supported(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if(pCtrl->InitParams.u16ChipECONum == 0)
        return FALSE;
    else
        return TRUE;

}

void HAL_HVD_EX_BBU_Proc(MS_U32 u32Id)
{

}


void HAL_HVD_EX_BBU_StopProc(MS_U32 u32Id)
{

}

#endif


