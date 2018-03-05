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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/io.h>
#include "chip_setup.h"
#include "include/mstar/mstar_chip.h"
#else
#include <string.h>
#endif

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
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define RV_VLC_TABLE_SIZE           0x20000
/* Add for Mobile Platform by Ted Sun */
//#define HVD_DISPQ_PREFETCH_COUNT    2
#define HVD_FW_MEM_OFFSET           0x100000UL  // 1M
#define VPU_QMEM_BASE               0x20000000UL

//max support pixel(by chip capacity)
#define HVD_HW_MAX_PIXEL            (3840*2160*31000ULL) // 4kx2k@30p
#define HEVC_HW_MAX_PIXEL           (4096*2160*61000ULL) // 4kx2k@60p
#define VP9_HW_MAX_PIXEL            (4096*2304*31000ULL) // 4kx2k@30p

#if 0
static HVD_AVC_VUI_DISP_INFO g_hvd_VUIINFO;
static MS_U8 g_hvd_nal_fill_pair[2][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0} };
static MS_U32 u32RV_VLCTableAddr = 0;   // offset from Frame buffer start address
static MS_U16 _u16DispQPtr = 0;
#endif

#define IS_TASK_ALIVE(id) ((id) != -1)
#ifndef UNUSED
#define UNUSED(x) (void)(x)
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

#define INC_VALUE(value, queue_sz) { (value) = ((++(value)) >= queue_sz) ? 0 : (value); }
#define IS_TASK_ALIVE(id) ((id) != -1)
#define NEXT_MULTIPLE(value, n) (((value) + (n) - 1) & ~((n)-1))

//------------------------------ MIU SETTINGS ----------------------------------
#define _MaskMiuReq_MVD_RW_0( m )       _HVD_WriteRegBit(MIU0_REG_RQ2_MASK, m, BIT(4))
#define _MaskMiuReq_MVD_RW_1( m )       _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(6))
#define _MaskMiuReq_MVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiuReq_HVD_RW_MIF0( m )    _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(2))
#define _MaskMiuReq_HVD_RW_MIF1( m )    _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(3))
#define _MaskMiuReq_HVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(0))

#define _MaskMiu1Req_MVD_RW_0( m )      _HVD_WriteRegBit(MIU1_REG_RQ2_MASK, m, BIT(4))
#define _MaskMiu1Req_MVD_RW_1( m )      _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(6))
#define _MaskMiu1Req_MVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ0_MASK+1, m, BIT(4))
#define _MaskMiu1Req_HVD_RW_MIF0( m )   _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(2))
#define _MaskMiu1Req_HVD_RW_MIF1( m )   _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(3))
#define _MaskMiu1Req_HVD_BBU_R( m )     _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(0))


#define HVD_MVD_RW_0_ON_MIU0            ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(4)) == 0)
#define HVD_MVD_RW_1_ON_MIU0            ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(6)) == 0)
#define HVD_MVD_BBU_R_ON_MIU0           ((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)) == 0)
#define HVD_HVD_RW_MIF0_ON_MIU0         ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(2)) == 0)
#define HVD_HVD_RW_MIF1_ON_MIU0         ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(3)) == 0)
#define HVD_HVD_BBU_R_ON_MIU0           ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(0)) == 0)

#define HVD_MVD_RW_0_ON_MIU1            ((_HVD_Read2Byte(MIU0_REG_SEL2) & BIT(4)) == BIT(4))
#define HVD_MVD_RW_1_ON_MIU1            ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(6)) == BIT(6))
#define HVD_MVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL0) & BIT(12)) == BIT(12))
#define HVD_HVD_RW_MIF0_ON_MIU1         ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(2)) == BIT(2))
#define HVD_HVD_RW_MIF1_ON_MIU1         ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(3)) == BIT(3))
#define HVD_HVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(0)) == BIT(0))

#if SUPPORT_EVD
#define _MaskMiuReq_EVD_RW( m )         _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(4))
#define _MaskMiuReq_EVD_BBU_R( m )      _HVD_WriteRegBit(MIU0_REG_RQ4_MASK, m, BIT(4))
#define _MaskMiu1Req_EVD_RW( m )         _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(4))
#define _MaskMiu1Req_EVD_BBU_R( m )      _HVD_WriteRegBit(MIU1_REG_RQ4_MASK, m, BIT(4))

#define HVD_EVD_RW_ON_MIU0              ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(4)) == 0)
#define HVD_EVD_BBU_R_ON_MIU0           ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(4)) == 0)
#define HVD_EVD_RW_ON_MIU1              ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(4)) == BIT(4))
#define HVD_EVD_BBU_R_ON_MIU1           ((_HVD_Read2Byte(MIU0_REG_SEL4) & BIT(4)) == BIT(4))
#endif

#define _HVD_MIU_SetReqMask(miu_clients, mask)  \
    do                                          \
    {                                           \
        if (HVD_##miu_clients##_ON_MIU0 == 1)   \
        {                                       \
            _MaskMiuReq_##miu_clients(mask);    \
        }                                       \
        else                                    \
        {                                       \
            if (HVD_##miu_clients##_ON_MIU1 == 1)   \
            {                                       \
                _MaskMiu1Req_##miu_clients(mask);   \
            }                                       \
        }                                       \
    } while (0)

// check RM is supported or not
#define HVD_HW_RUBBER3      (HAL_HVD_EX_GetHWVersionID()& BIT(14))
#ifdef VDEC3
#define HAL_HVD_EX_MAX_SUPPORT_STREAM   16
#else
#define HAL_HVD_EX_MAX_SUPPORT_STREAM   3
#endif

#define DIFF(a, b) (a > b ? (a-b) : (b-a))  // abs diff

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
static MS_VIRT       _HVD_EX_GetVUIDispInfo(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetBBUQNumb(MS_U32 u32Id);
static MS_U32       _HVD_EX_GetPTSQNumb(MS_U32 u32Id);
static HVD_EX_Drv_Ctrl *_HVD_EX_GetDrvCtrl(MS_U32 u32Id);
static HVD_Frm_Information *_HVD_EX_GetNextDispFrame(MS_U32 u32Id);
static void HAL_HVD_EX_VP8AECInUsed(MS_U32 u32Id, MS_BOOL *isVP8Used, MS_BOOL *isAECUsed, MS_BOOL *isAVCUsed);
static void HAL_AEC_PowerCtrl(MS_BOOL bEnable);
static void HAL_VP8_PowerCtrl(MS_BOOL bEnable);
#if SUPPORT_EVD
static void HAL_EVD_EX_PowerCtrl(MS_U32 u32Id, MS_BOOL bEnable);
#endif
#if SUPPORT_G2VP9 && defined(VDEC3)
static void HAL_VP9_EX_PowerCtrl(MS_BOOL bEnable);
#endif
static MS_U64 _HAL_EX_GetHwMaxPixel(MS_U32 u32Id);
#if SUPPORT_G2VP9 && defined(VDEC3)
static MS_BOOL _HAL_HVD_EX_PostProc_Task(MS_U32 u32Id);
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if defined (__aeon__)
static MS_VIRT u32HVDRegOSBase = 0xA0200000;
#else
static MS_VIRT u32HVDRegOSBase = 0xBF200000;
#endif
#if HAL_HVD_ENABLE_MUTEX_PROTECT
MS_S32 s32HVDMutexID = -1;
MS_U8 _u8HVD_Mutex[] = { "HVD_Mutex" };
#endif


#define HVD_EX_STACK_SIZE 4096
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
typedef struct
{

    HVD_AVC_VUI_DISP_INFO g_hvd_VUIINFO;
    MS_U8 g_hvd_nal_fill_pair[2][8];
    MS_VIRT u32RV_VLCTableAddr;  // offset from Frame buffer start address
    MS_U16 _u16DispQPtr;
    MS_U16 _u16DispOutSideQPtr[HAL_HVD_EX_MAX_SUPPORT_STREAM];

    //HVD_EX_Drv_Ctrl *_pHVDCtrls;
    MS_U32 u32HVDCmdTimeout;//same as HVD_FW_CMD_TIMEOUT_DEFAULT
    MS_U32 u32VPUClockType;
    MS_U32 u32HVDClockType;//160
#if SUPPORT_EVD
    MS_U32 u32EVDClockType;
#endif
#if SUPPORT_G2VP9 && defined(VDEC3)
    MS_U32 u32VP9ClockType;
#endif
    HVD_EX_Stream _stHVDStream[HAL_HVD_EX_MAX_SUPPORT_STREAM];

    volatile HVD_Frm_Information *pHvdFrm;//_HVD_EX_GetNextDispFrame()
    MS_BOOL g_RstFlag;
    MS_U64 u64pts_real;
    MS_PHY u32VP8BBUWptr;
    MS_PHY u32EVDBBUWptr;
    MS_BOOL bBBU_running[HAL_HVD_EX_MAX_SUPPORT_STREAM];
    MS_U32 u32BBUReadEsPtr[HAL_HVD_EX_MAX_SUPPORT_STREAM];
    MS_S32  _s32VDEC_BBU_TaskId[HAL_HVD_EX_MAX_SUPPORT_STREAM];
    MS_U8   u8VdecExBBUStack[HAL_HVD_EX_MAX_SUPPORT_STREAM][HVD_EX_STACK_SIZE];
    //pre_set
    HVD_Pre_Ctrl *pHVDPreCtrl_Hal[HAL_HVD_EX_MAX_SUPPORT_STREAM];
} HVD_Hal_CTX;

HVD_Hal_CTX* pHVDHalContext = NULL;
HVD_Hal_CTX gHVDHalContext;
HVD_EX_Drv_Ctrl *_pHVDCtrls = NULL;

static HVD_EX_PreSet _stHVDPreSet[HAL_HVD_EX_MAX_SUPPORT_STREAM] =
{
    {FALSE},
    {FALSE},
    {FALSE},
#ifdef VDEC3
    {FALSE},
#endif
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
#ifdef VDEC3
static MS_BOOL _HAL_EX_IS_EVD(MS_U32 u32ModeFlag)
{
    MS_U32 u32CodecType = u32ModeFlag & E_HVD_INIT_HW_MASK;

    if (u32CodecType == E_HVD_INIT_HW_HEVC || u32CodecType == E_HVD_INIT_HW_HEVC_DV
#if SUPPORT_MSVP9
     || u32CodecType == E_HVD_INIT_HW_VP9
#endif
       )
        return TRUE;

    return FALSE;
}

static MS_BOOL _HAL_EX_BBU_VP8_InUsed(void)
{
    if (!pHVDHalContext)
        return FALSE;

    MS_U32 i;
    MS_BOOL bRet = FALSE;

    for (i = 0; i < HAL_HVD_EX_MAX_SUPPORT_STREAM; i++)
    {
        if (pHVDHalContext->_stHVDStream[i].bUsed && pHVDHalContext->_stHVDStream[i].u32CodecType == E_HAL_HVD_VP8)
        {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

// This function will get decoder type not only MVD,HVD,EVD but more codec types.
// However, sometimes we don't use so deterministic infomation.
static MS_BOOL HAL_HVD_EX_GetTaskInfo(MS_U32 u32Id, VPU_EX_TaskInfo* pstTaskInfo)
{

    MS_U32 ret = TRUE;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if(pCtrl == NULL || pstTaskInfo == NULL)
        return FALSE;

    pstTaskInfo->u32Id = u32Id;

    switch(pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)
    {
        case E_HVD_INIT_HW_RM:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_RVD;
            break;
        case E_HVD_INIT_HW_VP8:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_VP8;
            break;
        case E_HVD_INIT_HW_MVC:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_HVD; //E_VPU_EX_DECODER_MVC;
            break;
        case E_HVD_INIT_HW_HEVC:
        case E_HVD_INIT_HW_HEVC_DV:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_EVD;
            break;
        #if SUPPORT_MSVP9
        case E_HVD_INIT_HW_VP9:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_EVD;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HVD_INIT_HW_VP9:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_G2VP9;
            break;
        #endif
        default:
            pstTaskInfo->eDecType = E_VPU_EX_DECODER_HVD;
            break;
    }

    pstTaskInfo->eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_DRV)
    {
        pstTaskInfo->eSrcType = E_VPU_EX_INPUT_FILE;
    }
    else
    {
        pstTaskInfo->eSrcType = E_VPU_EX_INPUT_TSP;
    }

    pstTaskInfo->u32HeapSize = HVD_DRAM_SIZE;

#ifdef SUPPORT_EVD
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC ||
        (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
        pstTaskInfo->u32HeapSize = EVD_DRAM_SIZE;
#endif
    return ret;

}

MS_U32 HAL_HVD_EX_GetBBUId(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U32 ret = HAL_HVD_INVALID_BBU_ID;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    _HAL_HVD_Entry();

    if(pCtrl == NULL)
        _HAL_HVD_Return(ret);

    VPU_EX_TaskInfo     taskInfo;
    memset(&taskInfo, 0, sizeof(VPU_EX_TaskInfo));

    HAL_HVD_EX_GetTaskInfo(u32Id,&taskInfo);

    taskInfo.u8HalId = u8Idx;
    ret = HAL_VPU_EX_GetBBUId(u32Id, &taskInfo, pCtrl->bShareBBU);

    HVD_EX_MSG_DBG("u32Id=0x%x eDecType=0x%x eSrcType=0x%x ret=0x%x\n", (unsigned int)taskInfo.u32Id,
        (unsigned int)taskInfo.eDecType, (unsigned int)taskInfo.eSrcType, (unsigned int)ret);

    _HAL_HVD_Return(ret);
}

MS_BOOL HAL_HVD_EX_FreeBBUId(MS_U32 u32Id, MS_U32 u32BBUId)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL ret = FALSE;
    _HAL_HVD_Entry();

     if(pCtrl == NULL)
        _HAL_HVD_Return(ret);
    VPU_EX_TaskInfo     taskInfo;
    memset(&taskInfo, 0, sizeof(VPU_EX_TaskInfo));

    HAL_HVD_EX_GetTaskInfo(u32Id,&taskInfo);

    ret = HAL_VPU_EX_FreeBBUId(u32Id,u32BBUId,&taskInfo);

    HVD_EX_MSG_DBG("u32Id=0x%x eDecType=0x%x eSrcType=0x%x ret=0x%x\n", (unsigned int)taskInfo.u32Id,
        (unsigned int)taskInfo.eDecType, (unsigned int)taskInfo.eSrcType, (unsigned int)ret);

    _HAL_HVD_Return(ret);
}

MS_BOOL HAL_HVD_EX_ClearBBUSetting(MS_U32 u32Id, MS_U32 u32BBUId)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL ret = FALSE;
    _HAL_HVD_Entry();

     if(pCtrl == NULL)
        _HAL_HVD_Return(ret);
    VPU_EX_TaskInfo     taskInfo;
    memset(&taskInfo, 0, sizeof(VPU_EX_TaskInfo));

    HAL_HVD_EX_GetTaskInfo(u32Id,&taskInfo);

    HAL_VPU_EX_ClearBBUSetting(u32Id, u32BBUId, taskInfo.eDecType);

    _HAL_HVD_Return(TRUE);
}
#endif

static void _HVD_EX_PpTask_Delete(HVD_EX_Stream *pstHVDStream)
{
    pstHVDStream->ePpTaskState = E_HAL_HVD_STATE_STOP;
    MsOS_DeleteTask(pstHVDStream->s32HvdPpTaskId);
    pstHVDStream->s32HvdPpTaskId = -1;
}

static void _HVD_EX_Context_Init_HAL(void)
{
    pHVDHalContext->u32HVDCmdTimeout = 100;//same as HVD_FW_CMD_TIMEOUT_DEFAULT
    pHVDHalContext->u32VPUClockType = 432;
    pHVDHalContext->u32HVDClockType = 384;
#if SUPPORT_EVD
    pHVDHalContext->u32EVDClockType = 576;
#endif
#if SUPPORT_G2VP9 && defined(VDEC3)
    pHVDHalContext->u32VP9ClockType = 384;
#endif
#ifdef VDEC3
    MS_U8 i;

    for (i = 0; i < HAL_HVD_EX_MAX_SUPPORT_STREAM; i++)
    {
        pHVDHalContext->_stHVDStream[i].eStreamId = E_HAL_HVD_N_STREAM0 + i;
        pHVDHalContext->_stHVDStream[i].ePpTaskState = E_HAL_HVD_STATE_STOP;
        pHVDHalContext->_stHVDStream[i].s32HvdPpTaskId = -1;
    }
#else
    pHVDHalContext->_stHVDStream[0].eStreamId = E_HAL_HVD_MAIN_STREAM0;
    pHVDHalContext->_stHVDStream[1].eStreamId = E_HAL_HVD_SUB_STREAM0;
    pHVDHalContext->_stHVDStream[2].eStreamId = E_HAL_HVD_SUB_STREAM1;
#endif
}

static MS_U16 _HVD_EX_GetBBUReadptr(MS_U32 u32Id)
{
    MS_U16 u16Ret = 0;
#if (HVD_ENABLE_MVC || (!VDEC3))
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
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
#ifdef VDEC3
    if (0 == pCtrl->u32BBUId)
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
            u16Ret = _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS2(u32RB));
    }

    HVD_EX_MSG_DBG("Task0=%d, Task1=%d\n",
        _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI(u32RB)), _HVD_Read2Byte(HVD_REG_NAL_RPTR_HI_BS2(u32RB)));

    return u16Ret;
}

static MS_U16 _HVD_EX_GetBBUWritedptr(MS_U32 u32Id)
{
    MS_U16 u16Ret = 0;
#if (HVD_ENABLE_MVC || (!VDEC3))
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
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
#ifdef VDEC3
    if (0 == pDrvCtrl->u32BBUId)
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
        u16Ret = _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS2(u32RB));
    }

    return u16Ret;
}

static void _HVD_EX_ResetMainSubBBUWptr(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI(u32RB), 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS2(u32RB), 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS3, 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
    _HVD_Write2Byte(HVD_REG_NAL_RPTR_HI_BS4, 0);
    _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_NAL_WPTR_SYNC, HVD_REG_NAL_WPTR_SYNC);    // set bit 3
}

static void _HVD_EX_SetBBUWriteptr(MS_U32 u32Id, MS_U16 u16BBUNewWptr)
{
#if (HVD_ENABLE_MVC || (!VDEC3))
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
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
#ifdef VDEC3
    if (0 == pCtrl->u32BBUId)
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
        _HVD_Write2Byte(HVD_REG_NAL_WPTR_HI_BS2(u32RB), u16BBUNewWptr);
        //if(pCtrl->InitParams.bColocateBBUMode)
        if(_stHVDPreSet[u8Idx].bColocateBBUMode)
            pShm->u32ColocateBBUWritePtr = u16BBUNewWptr;
    }

    HVD_EX_MSG_DBG("Task0=%d, Task1=%d\n",
        _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI(u32RB)), _HVD_Read2Byte(HVD_REG_NAL_WPTR_HI_BS2(u32RB)));

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
    MS_PHY u32VP8_BBU_DRAM_ST_ADDR_BS3 = pShm->u32HVD_BBU_DRAM_ST_ADDR;

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
#ifdef VDEC3
        if (0 == pCtrl->u32BBUId)
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
            u32Data = _HVD_Read2Byte(HVD_REG_ESB_RPTR_L_BS2(u32RB)) & 0xFFC0;
            u32Data >>= 6;
            u32Data |= _HVD_Read2Byte(HVD_REG_ESB_RPTR_H_BS2(u32RB)) << 10;
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

    HVD_EX_MSG_DBG("Send ARG 0x%x to HVD\n", u32Arg);

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

    HVD_EX_MSG_DBG("Send CMD 0x%x to HVD \n", u32Cmd);

#if HVD_ENABLE_MVC
    if(E_HAL_VPU_MVC_STREAM_BASE == u8TaskId)
    {
        u8TaskId = E_HAL_VPU_MAIN_STREAM_BASE;
    }
#endif /// HVD_ENABLE_MVC

    HVD_EX_MSG_DBG("Send CMD 0x%x to HVD u8TaskId = %X\n", u32Cmd,u8TaskId);

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
#ifdef VDEC3
    HVD_DRAM_COMMAND_QUEUE_SEND_STATUS SentRet = E_HVD_COMMAND_QUEUE_SEND_FAIL;
    MS_BOOL IsSent = FALSE;
    MS_BOOL IsMailBox = FALSE;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (HAL_VPU_EX_IsDisplayQueueCMD(u32Cmd))
    {
        do {
            SentRet = HAL_VPU_EX_DRAMStreamDispCMDQueueSend(u32Id, &pShm->cmd_queue, E_HVD_CMDQ_ARG, u32CmdArg);
            if (!SentRet)
                HVD_EX_MSG_DBG("^^^Display command ARG return=0x%X cmd=0x%x arg=0x%x\n", SentRet,u32Cmd, u32CmdArg);
            if (SentRet == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
                break;
            else if (SentRet == E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL) {
                IsSent = TRUE;
                break;
            }
            else if (HVD_GetSysTime_ms() > u32timeout)
            {
                 HVD_EX_MSG_ERR("^^^Display command ARG timeout: cmd=0x%x arg=0x%x\n", u32Cmd, u32CmdArg);
                 break;
            }
        }while (SentRet != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);
    }
    else if (!HAL_VPU_EX_IsMailBoxCMD(u32Cmd))
    {
        do {
            SentRet = HAL_VPU_EX_DRAMStreamCMDQueueSend(u32Id, &pShm->cmd_queue, E_HVD_CMDQ_ARG, u32CmdArg);
            if (!SentRet)
                HVD_EX_MSG_DBG("^^^Dram command ARG return=0x%X cmd=0x%x arg=0x%x\n", SentRet,u32Cmd, u32CmdArg);
            if (SentRet == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
                break;
            else if (SentRet == E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL) {
                IsSent = TRUE;
                break;
            }
            else if (HVD_GetSysTime_ms() > u32timeout)
            {
                 HVD_EX_MSG_ERR("^^^Dram command ARG timeout: cmd=0x%x arg=0x%x\n", u32Cmd, u32CmdArg);
                 break;
            }
        }while (SentRet != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);
    }
    if (!IsSent)
    {
        IsMailBox = TRUE;
        u32timeout = HVD_GetSysTime_ms() + HVD_DRV_MAILBOX_CMD_WAIT_FINISH_TIMEOUT;//pHVDHalContext->u32HVDCmdTimeout;
        while (!_HVD_EX_SetCMDArg(u32Id, u32CmdArg))
#else
    while (!_HVD_EX_SetCMDArg(u32Id, u32CmdArg))
#endif
    {
//#ifndef VDEC3 // FIXME: workaround fw response time is slow sometimes in multiple stream case so far
        if (HVD_GetSysTime_ms() > u32timeout)
        {
            HVD_EX_MSG_ERR("Timeout: cmd=0x%x arg=0x%x\n", u32Cmd, u32CmdArg);
            return E_HVD_RETURN_TIMEOUT;
        }
//#endif

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

#ifdef VDEC3
    }
    IsSent = FALSE;
    u32timeout = HVD_GetSysTime_ms() + pHVDHalContext->u32HVDCmdTimeout;
    if (HAL_VPU_EX_IsDisplayQueueCMD(u32Cmd) && !IsMailBox)
    {
        do {
            SentRet = HAL_VPU_EX_DRAMStreamDispCMDQueueSend(u32Id, &pShm->cmd_queue, E_HVD_CMDQ_CMD,u32Cmd);
            if (!SentRet)
                HVD_EX_MSG_DBG("^^^Display command CMD return=0x%X cmd=0x%x arg=0x%x\n", SentRet,u32Cmd, u32CmdArg);
            if (SentRet == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
                break;
            else if (SentRet == E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL){
                IsSent = TRUE;
                break;
            }
            else if (HVD_GetSysTime_ms() > u32timeout)
             {
                 HVD_EX_MSG_ERR("^^^Display command CMD timeout: cmd=0x%x arg=0x%x\n", u32Cmd, u32CmdArg);
                 break;
             }
        } while (SentRet != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);
    }
    else if(!HAL_VPU_EX_IsMailBoxCMD(u32Cmd) && !IsMailBox)
    {
        do {
            SentRet = HAL_VPU_EX_DRAMStreamCMDQueueSend(u32Id, &pShm->cmd_queue, E_HVD_CMDQ_CMD,u32Cmd);
            if (SentRet != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL) {
                HVD_EX_MSG_ERR("^^^Dram command CMD return=0x%X cmd=0x%x arg=0x%x\n", SentRet,u32Cmd, u32CmdArg);
            }
            if (SentRet == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
                break;
            else if (SentRet == E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL){
                IsSent = TRUE;
                break;
            }
            else if (HVD_GetSysTime_ms() > u32timeout)
             {
                 HVD_EX_MSG_ERR("^^^Dram command CMD timeout: cmd=0x%x arg=0x%x\n", u32Cmd, u32CmdArg);
                 break;
             }
        } while (SentRet != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);
    }
    if (!IsSent)
    {
        u32timeout = HVD_GetSysTime_ms() + HVD_DRV_MAILBOX_CMD_WAIT_FINISH_TIMEOUT;//pHVDHalContext->u32HVDCmdTimeout;
        while (!_HVD_EX_SetCMD(u32Id, u32Cmd))
#else
    u32timeout = HVD_GetSysTime_ms() + pHVDHalContext->u32HVDCmdTimeout;

    while (!_HVD_EX_SetCMD(u32Id, u32Cmd))
#endif
    {
    //#ifndef VDEC3 // FIXME: workaround fw response time is slow sometimes in multiple stream case so far
        if (HVD_GetSysTime_ms() > u32timeout)
        {
            HVD_EX_MSG_ERR("cmd timeout: %x\n", u32Cmd);
            return E_HVD_RETURN_TIMEOUT;
        }
    //#endif
        if(u32Cmd < E_DUAL_CMD_BASE)
        {
            //_HVD_EX_GetPC();
            HAL_HVD_EX_Dump_FW_Status(u32Id);
            HAL_HVD_EX_Dump_HW_Status(HVD_U32_MAX);
        }
    }
#ifdef VDEC3
    }
    else
    {
        HAL_HVD_EX_FlushMemory();
    }
#endif
    return E_HVD_RETURN_SUCCESS;
}

static void _HVD_EX_SetMIUProtectMask(MS_BOOL bEnable)
{
#if HAL_HVD_ENABLE_MIU_PROTECT
    _HVD_MIU_SetReqMask(MVD_RW_0, bEnable);
    _HVD_MIU_SetReqMask(MVD_RW_1, bEnable);
    _HVD_MIU_SetReqMask(MVD_BBU_R, bEnable);
#if SUPPORT_EVD
    _HVD_MIU_SetReqMask(EVD_RW, bEnable);
    _HVD_MIU_SetReqMask(EVD_BBU_R, bEnable);
#endif
    _HVD_MIU_SetReqMask(HVD_RW_MIF0, bEnable);
    _HVD_MIU_SetReqMask(HVD_RW_MIF1, bEnable);
    _HVD_MIU_SetReqMask(HVD_BBU_R, bEnable);
    HAL_VPU_EX_MIU_RW_Protect(bEnable);
    //HVD_Delay_ms(1);
#endif
    return;
}

static void _HVD_EX_SetBufferAddr(MS_U32 u32Id)
{
    MS_U16 u16Reg       = 0;
    MS_VIRT u32StAddr   = 0;
#ifdef VDEC3
    MS_U32 u32Length    = 0;
#endif
    MS_U8  u8BitMiuSel  = 0;
    MS_U8  u8CodeMiuSel = 0;
    MS_U8  u8FBMiuSel   = 0;
    MS_U8  u8TmpMiuSel  = 0;

    MS_U32 u32BitStartOffset;
    MS_U32 u32CodeStartOffset;
    MS_U32 u32FBStartOffset;

#ifndef VDEC3
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    _HAL_HVD_Entry();

    if (pCtrl == NULL)
    {
        _HAL_HVD_Return();
    }

    MS_BOOL bESBufferAlreadySet = FALSE;
    VPU_EX_TaskInfo taskInfo;
    memset(&taskInfo, 0, sizeof(VPU_EX_TaskInfo));

    HAL_HVD_EX_GetTaskInfo(u32Id, &taskInfo);

    bESBufferAlreadySet = HAL_VPU_EX_CheckBBUSetting(u32Id, pCtrl->u32BBUId, taskInfo.eDecType, VPU_BBU_ES_BUFFER);



    _phy_to_miu_offset(u8BitMiuSel, u32BitStartOffset, pCtrl->MemMap.u32BitstreamBufAddr);
    _phy_to_miu_offset(u8CodeMiuSel, u32CodeStartOffset, pCtrl->MemMap.u32CodeBufAddr);
    _phy_to_miu_offset(u8FBMiuSel, u32FBStartOffset, pCtrl->MemMap.u32FrameBufAddr);

    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_MIU_SEL,
                        (u8BitMiuSel << VDEC_BS_MIUSEL) |
                        (u8FBMiuSel << VDEC_LUMA8_MIUSEL) |
                        (u8FBMiuSel << VDEC_LUMA2_MIUSEL) |
                        (u8FBMiuSel << VDEC_CHROMA8_MIUSEL) |
                        (u8FBMiuSel << VDEC_CHROMA2_MIUSEL) |
                        (u8FBMiuSel << VDEC_HWBUF_MIUSEL) |
                        (u8FBMiuSel << VDEC_BUF1_MIUSEL) |
                        (u8FBMiuSel << VDEC_BUF2_MIUSEL) |
                        (u8FBMiuSel << VDEC_PPIN_MIUSEL) |
                        (u8FBMiuSel << VDEC_XCSHM_MIUSEL));

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        // ES buffer
#ifdef VDEC3
        if(pCtrl->bShareBBU)
            u32StAddr = pCtrl->MemMap.u32TotalBitstreamBufAddr; // NStream will share the same ES buffer
        else
#endif
            u32StAddr = u32BitStartOffset;

        _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, u32StAddr);

#ifdef VDEC3
        if (!_HAL_EX_BBU_VP8_InUsed())
#endif
        {
            HVD_EX_MSG_DBG("ESB start addr=%lx\n", (unsigned long)u32StAddr);

            _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_L_BS34, HVD_LWORD(u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_ESB_ST_ADR_H_BS34, HVD_HWORD(u32StAddr >> 3));

#ifdef VDEC3
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS34, HVD_LWORD(pCtrl->MemMap.u32TotalBitstreamBufSize >> 3));
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS34, HVD_HWORD(pCtrl->MemMap.u32TotalBitstreamBufSize >> 3));
#else
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS34, HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS34, HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
#endif

            u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BS34);
            u16Reg &= ~HVD_REG_BS34_TSP_INPUT;
            u16Reg &= ~HVD_REG_BS34_PASER_MASK;
            u16Reg |= HVD_REG_BS34_PASER_DISABLE;
            u16Reg |= HVD_REG_BS34_AUTO_NAL_TAB;
            _HVD_Write2Byte(HVD_REG_MIF_BS34, u16Reg);
        }

        _HAL_HVD_Return();
    }

    // ES buffer
#ifdef VDEC3
    if(!pCtrl->bShareBBU || E_HVD_INIT_INPUT_TSP == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK))
    {
        u32StAddr = pCtrl->MemMap.u32BitstreamBufAddr;
        u32Length = pCtrl->MemMap.u32BitstreamBufSize >> 3;
    }
    else
    {
        u32StAddr = pCtrl->MemMap.u32TotalBitstreamBufAddr;
        u32Length = pCtrl->MemMap.u32TotalBitstreamBufSize >> 3;
    }
#else
    u32StAddr = u32BitStartOffset;
#endif

    _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, u32StAddr);

    HVD_EX_MSG_DBG("ESB start addr=%lx, len=%x\n", (unsigned long)u32StAddr, pCtrl->MemMap.u32BitstreamBufSize);

#ifdef VDEC3
    if (!bESBufferAlreadySet)
    {
        if (pCtrl->u32BBUId == 0)
        {
            _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L(u32RB), HVD_LWORD(u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H(u32RB), HVD_HWORD(u32StAddr >> 3));

            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L(u32RB), HVD_LWORD(u32Length));
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H(u32RB), HVD_HWORD(u32Length));
        }
        else
        {
            _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2(u32RB), HVD_LWORD(u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2(u32RB), HVD_HWORD(u32StAddr >> 3));

            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2(u32RB), HVD_LWORD(u32Length));
            _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2(u32RB), HVD_HWORD(u32Length));
        }
    }
#else
    if (0 == u8TaskId)
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L(u32RB), HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H(u32RB), HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2(u32RB), HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2(u32RB), HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
    }
#endif

    // others
#ifdef VDEC3
    if (!bESBufferAlreadySet)
    {
        if (pCtrl->u32BBUId == 0)
            u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU(u32RB));
        else
            u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2(u32RB));

        if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
        {
            if (pCtrl->u32BBUId == 0)
                u16Reg |= HVD_REG_BBU_TSP_INPUT;
            else
                u16Reg |= HVD_REG_BBU_TSP_INPUT_BS2;
        }
        else
        {
            if (pCtrl->u32BBUId == 0)
                u16Reg &= ~HVD_REG_BBU_TSP_INPUT;
            else
                u16Reg &= ~HVD_REG_BBU_TSP_INPUT_BS2;
        }

        // do not set parsing setting in DRV, and we set it in FW (hvd_switch_bbu)
        if (pCtrl->u32BBUId == 0)
            u16Reg &= ~HVD_REG_BBU_PASER_MASK;
        else
            u16Reg &= ~HVD_REG_BBU_PASER_MASK_BS2;

        if (pCtrl->u32BBUId == 0)
        {
            u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB;
            _HVD_Write2Byte(HVD_REG_MIF_BBU(u32RB), u16Reg);
        }
        else
        {
            u16Reg |= HVD_REG_BBU_AUTO_NAL_TAB_BS2;
            _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2(u32RB), u16Reg);
        }
    }
#else
    if (0 == u8TaskId)
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
        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2(u32RB));

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

        _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2(u32RB), u16Reg);
    }
#endif

#if (HVD_ENABLE_MVC)
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC)
    {
        /// Used sub stream to record sub view data.
        HVD_EX_Drv_Ctrl *pDrvCtrl_Sub = _HVD_EX_GetDrvCtrl((u32Id+0x00011000));
        //printf("**************** Buffer setting for MVC dual-BBU *************\n");

        // ES buffer
        _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, (pDrvCtrl_Sub->MemMap.u32BitstreamBufAddr));

        HVD_EX_MSG_DBG("[MVC] 2nd ES _HAL_HVD_SetBuffer2Addr: ESb StAddr:%lx, len:%lx.\n", (unsigned long) u32StAddr, (unsigned long) pDrvCtrl_Sub->MemMap.u32BitstreamBufSize);
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2(u32RB), HVD_LWORD(pDrvCtrl_Sub->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2(u32RB), HVD_HWORD(pDrvCtrl_Sub->MemMap.u32BitstreamBufSize >> 3));

        u16Reg = _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2(u32RB));
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
        _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2(u32RB), u16Reg);
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

    if (!bESBufferAlreadySet)
    {
        HAL_VPU_EX_SetBBUSetting(u32Id, pCtrl->u32BBUId, taskInfo.eDecType, VPU_BBU_ES_BUFFER);
    }

    _HAL_HVD_Return();
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
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_L_BS2(u32RB), HVD_LWORD(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_ST_ADDR_H_BS2(u32RB), HVD_HWORD(u32StAddr >> 3));

        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_L_BS2(u32RB), HVD_LWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
        _HVD_Write2Byte(HVD_REG_ESB_LENGTH_H_BS2(u32RB), HVD_HWORD(pCtrl->MemMap.u32BitstreamBufSize >> 3));
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

#if (HVD_ENABLE_IQMEM)
MS_BOOL HAL_HVD_EX_IQMem_Init(MS_U32 u32Id)
{

    MS_U32 u32Timeout = 20000;

    if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_IQMEM_CTRL) == E_HVD_IQMEM_INIT_NONE)
    {

        HAL_VPU_EX_IQMemSetDAMode(TRUE);

        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_IQMEM_CTRL, E_HVD_IQMEM_INIT_LOADING);


        while (u32Timeout)
        {

            if (HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_IQMEM_CTRL) == E_HVD_IQMEM_INIT_LOADED)
            {
                break;
            }
            u32Timeout--;
            HVD_Delay_ms(1);
        }

        HAL_VPU_EX_IQMemSetDAMode(FALSE);

        HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_IQMEM_CTRL, E_HVD_IQMEM_INIT_FINISH);

        if (u32Timeout==0)
        {
            HVD_EX_MSG_ERR("Wait E_HVD_IQMEM_INIT_LOADED timeout !!\n");
            return FALSE;
        }


    }
    return TRUE;
}

#endif

#ifdef VDEC3
static MS_BOOL _HVD_EX_SetRegCPU(MS_U32 u32Id, MS_BOOL bFWdecideFB)
#else
static MS_BOOL _HVD_EX_SetRegCPU(MS_U32 u32Id)
#endif
{
    MS_U32 u32FirmVer = 0;
    MS_U32 u32Timeout = 20000;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    HVD_EX_MSG_DBG("HVD HW ver id: 0x%04x\n", HAL_HVD_EX_GetHWVersionID());

#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase, __FUNCTION__, __LINE__);
#endif

    HAL_VPU_EX_SetFWReload(!pCtrl->bTurboFWMode);

    VPU_EX_FWCodeCfg    fwCfg;
    VPU_EX_TaskInfo     taskInfo;
    VPU_EX_VLCTblCfg    vlcCfg;
#ifdef VDEC3
    VPU_EX_FBCfg        fbCfg;
#endif
    VPU_EX_NDecInitPara nDecInitPara;

    memset(&fwCfg,          0, sizeof(VPU_EX_FWCodeCfg));
    memset(&taskInfo,       0, sizeof(VPU_EX_TaskInfo));
    memset(&vlcCfg,         0, sizeof(VPU_EX_VLCTblCfg));
    memset(&nDecInitPara,   0, sizeof(VPU_EX_NDecInitPara));
#ifdef VDEC3_FB
    nDecInitPara.pVLCCfg        = NULL;
#else
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM) //rm
    {
        vlcCfg.u32DstAddr           = MsOS_PA2KSEG0(pCtrl->MemMap.u32FrameBufAddr + pHVDHalContext->u32RV_VLCTableAddr);
        vlcCfg.u32BinAddr           = pCtrl->MemMap.u32VLCBinaryVAddr;
        vlcCfg.u32BinSize           = pCtrl->MemMap.u32VLCBinarySize;
        vlcCfg.u32FrameBufAddr      = pCtrl->MemMap.u32FrameBufVAddr;
        vlcCfg.u32VLCTableOffset    = pHVDHalContext->u32RV_VLCTableAddr;
        nDecInitPara.pVLCCfg        = &vlcCfg;
    }
#endif
    nDecInitPara.pFWCodeCfg = &fwCfg;
    nDecInitPara.pTaskInfo  = &taskInfo;
#ifdef VDEC3
    fbCfg.u32FrameBufAddr = pCtrl->MemMap.u32FrameBufAddr;
    fbCfg.u32FrameBufSize = pCtrl->MemMap.u32FrameBufSize;

    if (fbCfg.u32FrameBufAddr >= pCtrl->MemMap.u32MIU1BaseAddr)
    {
        fbCfg.u32FrameBufAddr -= pCtrl->MemMap.u32MIU1BaseAddr;
    }

    nDecInitPara.pFBCfg = &fbCfg;
#endif

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
#ifdef VDEC3
    else if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC ||
        (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC_DV)
    {
        taskInfo.eDecType = E_VPU_EX_DECODER_EVD;
    }
    #if SUPPORT_MSVP9
    else if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
    {
        taskInfo.eDecType = E_VPU_EX_DECODER_EVD;
    }
    #endif
    #if SUPPORT_G2VP9
    else if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
    {
        taskInfo.eDecType = E_VPU_EX_DECODER_G2VP9;
    }
    #endif
#endif
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
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC ||
        (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9 )
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

    #if (HVD_ENABLE_IQMEM)
    HAL_HVD_EX_SetData(u32Id, E_HVD_SDATA_FW_IQMEM_ENABLE_IF_SUPPORT, (MS_U32)1);
    #endif

#ifdef VDEC3
    if (!HAL_VPU_EX_TaskCreate(u32Id, &nDecInitPara, bFWdecideFB, pCtrl->u32BBUId))
#else
    if (!HAL_VPU_EX_TaskCreate(u32Id, &nDecInitPara))
#endif
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

#ifdef VDEC3_FB
#if HVD_ENABLE_RV_FEATURE
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_RM) //rm
    {
        if(pShm->u32RM_VLCTableAddr == 0) {
            HVD_EX_MSG_ERR("[VDEC3_FB] Error!!!RM_VLCTableAddr is not ready\n");
        }
        else
        {
            vlcCfg.u32DstAddr           = MsOS_PA2KSEG1(MsOS_VA2PA(nDecInitPara.pFWCodeCfg->u32DstAddr + pShm->u32RM_VLCTableAddr));
            vlcCfg.u32BinAddr           = pCtrl->MemMap.u32VLCBinaryVAddr;
            vlcCfg.u32BinSize           = pCtrl->MemMap.u32VLCBinarySize;
            vlcCfg.u32FrameBufAddr      = pCtrl->MemMap.u32FrameBufVAddr; //this is frame buffer address is decided by player. In VDEC3_FB path, this variable could be zero or the start address of overall Frame buffer.
            vlcCfg.u32VLCTableOffset    = pShm->u32RM_VLCTableAddr; // offset from FW code  start address
            nDecInitPara.pVLCCfg        = &vlcCfg;
        }
    }

    if (nDecInitPara.pVLCCfg)
    {
        HVD_EX_MSG_DBG("[VDEC3_FB] Ready to load VLC Table DstAddr=0x%x FrameBufAddr=0x%x VLCTableOffset=0x%x\n", (unsigned int)vlcCfg.u32DstAddr, (unsigned int)vlcCfg.u32FrameBufAddr, (unsigned int)vlcCfg.u32VLCTableOffset);
        if (!HAL_VPU_EX_LoadVLCTable(nDecInitPara.pVLCCfg, nDecInitPara.pFWCodeCfg->u8SrcType))
        {
            HVD_EX_MSG_ERR("[VDEC3_FB] Error!!!Load VLC Table fail!\n");
            return FALSE;
        }
    }
#endif
#endif
    if (u32Timeout > 0)
    {
        MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);

        pHVDHalContext->_stHVDStream[u8Idx].bUsed = TRUE;

#ifdef VDEC3
        switch (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)
        {
            case E_HVD_INIT_HW_AVC:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_AVC;
                break;
            case E_HVD_INIT_HW_AVS:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_AVS;
                break;
            case E_HVD_INIT_HW_RM:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_RM;
                break;
            case E_HVD_INIT_HW_MVC:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_MVC;
                break;
            case E_HVD_INIT_HW_VP8:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_VP8;
                break;
            case E_HVD_INIT_HW_MJPEG:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_MJPEG;
                break;
            case E_HVD_INIT_HW_VP6:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_VP6;
                break;
            case E_HVD_INIT_HW_HEVC:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_HEVC;
                break;
            case E_HVD_INIT_HW_VP9:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_VP9;
                break;
            case E_HVD_INIT_HW_HEVC_DV:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_HEVC_DV;
                break;
            default:
                pHVDHalContext->_stHVDStream[u8Idx].u32CodecType = E_HAL_HVD_NONE;
                break;
        }
#endif

        HVD_EX_MSG_INF("FW version binary=0x%x, if=0x%x\n", u32FirmVer, (MS_U32) HVD_FW_VERSION);
    }
    else
    {
        HVD_EX_MSG_ERR("Cannot get FW version !!0x%x 0x%lx \n", (MS_S16) _HVD_Read2Byte(HVD_REG_RESET),
                    (unsigned long)HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_VERSION_ID));

        if (TRUE != HAL_VPU_EX_TaskDelete(u32Id, &nDecInitPara))
        {
           HVD_EX_MSG_ERR("Task delete fail!\n");
        }

        return FALSE;
    }



    #if (HVD_ENABLE_IQMEM)

    if( HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_FW_IS_IQMEM_SUPPORT))
    {

        HAL_HVD_EX_IQMem_Init(u32Id);
    }
    else{
        HVD_EX_MSG_DBG("not support IQMEM\n");
    }
    #endif






#if HVD_ENABLE_TIME_MEASURE
    HVD_EX_MSG_MUST("HVD Time Measure:%d (%s %d) \n", HVD_GetSysTime_ms() - pHVDDrvContext->u32InitSysTimeBase, __FUNCTION__, __LINE__);
#endif

    return TRUE;
}

static MS_VIRT _HVD_EX_GetPTSTableRptr(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    if (pShm->u32PTStableRptrAddr & VPU_QMEM_BASE)
    {
        return HAL_VPU_EX_MemRead(pShm->u32PTStableRptrAddr);
    }
    else
    {
        //return *((MS_VIRT *) MsOS_PA2KSEG1((MS_PHY) pShm->u32PTStableRptrAddr + pCtrl->MemMap.u32CodeBufAddr));
        return *((MS_U32 *) MsOS_PA2KSEG1((MS_PHY) pShm->u32PTStableRptrAddr + pCtrl->MemMap.u32CodeBufAddr));
    }
}

static MS_VIRT _HVD_EX_GetPTSTableWptr(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm      = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (pShm->u32PTStableWptrAddr & VPU_QMEM_BASE)
    {
        return HAL_VPU_EX_MemRead(pShm->u32PTStableWptrAddr);
    }
    else
    {
        //return *((MS_VIRT *) MsOS_PA2KSEG1((MS_PHY)pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr));
        return *((MS_U32 *) MsOS_PA2KSEG1((MS_PHY)pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr));
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
        //*((MS_VIRT *) MsOS_PA2KSEG1((MS_PHY)pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr)) = u32Value;
        *((MS_U32 *) MsOS_PA2KSEG1((MS_PHY)pShm->u32PTStableWptrAddr + pCtrl->MemMap.u32CodeBufAddr)) = u32Value;
    }
}

static HVD_Return _HVD_EX_UpdatePTSTable(MS_U32 u32Id, HVD_BBU_Info *pInfo)
{
    MS_VIRT u32PTSWptr = HVD_U32_MAX;
    MS_VIRT u32PTSRptr = HVD_U32_MAX;
    MS_VIRT u32DestAddr = 0;
    HVD_PTS_Entry PTSEntry;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    // update R & W ptr
    u32PTSRptr = _HVD_EX_GetPTSTableRptr(u32Id);

    HVD_EX_MSG_DBG("PTS table rptr:0x%lx, wptr=0x%lx\n", (unsigned long)u32PTSRptr, (unsigned long)_HVD_EX_GetPTSTableWptr(u32Id));

    if (u32PTSRptr >= MAX_PTS_TABLE_SIZE)
    {
        HVD_EX_MSG_ERR("PTS table Read Ptr(%lx) > max table size(%x) \n", (unsigned long)u32PTSRptr,
                    (MS_U32) MAX_PTS_TABLE_SIZE);
        return E_HVD_RETURN_FAIL;
    }

    // check queue is full or not
    u32PTSWptr = pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr + 1;
    u32PTSWptr %= MAX_PTS_TABLE_SIZE;

    if (u32PTSWptr == u32PTSRptr)
    {
        HVD_EX_MSG_ERR("PTS table full. Read Ptr(%lx) == new Write ptr(%lx) ,Pre Wptr(%lx) \n", (unsigned long)u32PTSRptr,
                    (unsigned long)u32PTSWptr, (unsigned long)pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr);
        return E_HVD_RETURN_FAIL;
    }

    // add one PTS entry
    PTSEntry.u32ByteCnt = pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt & HVD_BYTE_COUNT_MASK;
    PTSEntry.u32ID_L = pInfo->u32ID_L;
    PTSEntry.u32ID_H = pInfo->u32ID_H;
    PTSEntry.u32PTS = pInfo->u32TimeStamp;

    u32DestAddr = MsOS_PA2KSEG1(pCtrl->MemMap.u32CodeBufAddr + (MS_PHY)pShm->u32HVD_PTS_TABLE_ST_OFFSET + (pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr * sizeof(HVD_PTS_Entry)));

    HVD_EX_MSG_DBG("PTS entry dst addr=0x%lx\n", (unsigned long)MsOS_VA2PA(u32DestAddr));

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
    MS_VIRT u32Adr = 0;
    MS_U32 u32BBUNewWptr = 0;
    MS_U8 item[8];
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_PHY u32BBU_DRAM_ST_ADDR = pShm->u32HVD_BBU_DRAM_ST_ADDR;

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

    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) ||
        E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
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

    HVD_EX_MSG_DBG("addr=0x%lx, bbu wptr=0x%x\n", (unsigned long)MsOS_VA2PA(u32Adr), pHVDHalContext->_stHVDStream[u8Idx].u32BBUWptr);

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
    MS_VIRT u32Adr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_PHY u32VP8_BBU_DRAM_ST_ADDR_BS4 = pShm->u32HVD_BBU2_DRAM_ST_ADDR;

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

static MS_VIRT _HVD_EX_GetVUIDispInfo(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if( ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC) ||
        ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC) ||
        ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC) )
    {
        MS_U16 i;
        MS_PHY u32VUIAddr;
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

    return (MS_VIRT) &(pHVDHalContext->g_hvd_VUIINFO);
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

    HVD_EX_MSG_DBG("idx=%x, bbu rptr=%x, bbu wptr=%x\n", u8Idx, u32ReadPtr, u32WritePtr);

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
        HVD_EX_MSG_ERR("PTS table Read Ptr(%x) > max table size(%x) \n", u32ReadPtr,
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

static MS_BOOL _HVD_EX_IsHevcInterlaceField(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    return pShm->u32CodecType == E_HVD_Codec_HEVC && pShm->DispInfo.u8Interlace == 1;
}

static HEVC_PIC_STRUCT hevc_get_paired(HEVC_PIC_STRUCT pic_struct, int first_field)
{
    if (pic_struct == EVD_TOP_FIELD)
        return EVD_BOTTOM_FIELD;
    else if (pic_struct == EVD_BOTTOM_FIELD)
        return EVD_TOP_FIELD;

    if (first_field)
    {
        // pic_struct is the first field and must pair with the next one
        if (pic_struct == EVD_TOP_WITH_NEXT)
            return EVD_BOTTOM_WITH_PREV;
        else if (pic_struct == EVD_BOTTOM_WITH_NEXT)
            return EVD_TOP_WITH_PREV;
    }
    else
    {
        // pic_struct is the second field and must pair with the previous one
        if (pic_struct == EVD_TOP_WITH_PREV)
            return EVD_BOTTOM_WITH_NEXT;
        else if (pic_struct == EVD_BOTTOM_WITH_PREV)
            return EVD_TOP_WITH_NEXT;
    }

    return EVD_UNKNOWN_TYPE;
}

static HVD_Frm_Information *_HVD_EX_GetNextDispFrame(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL bDolbyVision = (E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK));
    HAL_HVD_EX_ReadMemory();
    MS_U16 u16QNum = pShm->u16DispQNumb;
    MS_U16 u16QPtr = pShm->u16DispQPtr;

    HVD_Frm_Information *pHvdFrm = (HVD_Frm_Information*)&pShm->DispQueue[u16QPtr];
    if (bDolbyVision)
    {
        if (pHVDHalContext->_stHVDStream[u8Idx].bfirstGetFrmInfoDone && u16QNum < 4) // first time we need to wait 4 pic to ensure we got the correct layer type
        {
            return NULL;
        }
        else
        {
            pHVDHalContext->_stHVDStream[u8Idx].bfirstGetFrmInfoDone = FALSE;
        }
    }

#if (HVD_ENABLE_MVC)
    MS_BOOL bMVC = HAL_HVD_EX_CheckMVCID(u32Id);
    if (bMVC || (bDolbyVision && !pShm->bSingleLayer))
    {
        if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
        {
            MS_U16 u16RealQPtr = pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex;
            MS_U16 u16UsedFrm = 0;
            MS_U16 u16ResvFrmNum = ((u16RealQPtr % 2) == 0) ? 1 : 0; // need to check the next frame num is exist when get first frame.
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

            if (u16QNum > (u16UsedFrm + u16ResvFrmNum))
            {
                u16QNum -= u16UsedFrm;
                u16QPtr = u16RealQPtr;
                pHvdFrm = (HVD_Frm_Information*)&pShm->DispQueue[u16QPtr];

                if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT) // Must Be
                {
                    if ((u16QPtr % 2) == 0)
                    {
                        HVD_Frm_Information *pHvdFrmNext = (HVD_Frm_Information*)&pShm->DispQueue[u16QPtr + 1];

                        if (pHvdFrmNext->u32Status != E_HVD_DISPQ_STATUS_INIT)
                        {
                            return NULL;
                        }

                        //ALOGE("G1: %x", pHvdFrm->u32PrivateData);
                        if(bDolbyVision)
                        {
                            HVD_PRINT("BL pts: %d, u16QPtr: %d, u16QNum:%d, u32PrivateData:%d %d %d %d\n",pHvdFrm->u32TimeStamp, u16QPtr, u16QNum, pHvdFrm->u32PrivateData, pShm->u16DispQNumb, pShm->u16DispQPtr, u16UsedFrm);
                        }
                        pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData = pHvdFrm->u32PrivateData;
                    }
                    else
                    {
                        //ALOGE("G2: %x", (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData);
                        //pShm->UpdateQueue[pShm->u16UpdateQWtPtr] = (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData;
                        //pShm->u16UpdateQWtPtr = (pShm->u16UpdateQWtPtr + 1) % HVD_DISP_QUEUE_MAX_SIZE;
                        HVD_Frm_Information *pHvdFrmPrv = (HVD_Frm_Information*)&pShm->DispQueue[u16QPtr - 1]; // must be odd

                        if(bDolbyVision)
                        {
                            HVD_PRINT("EL pts: %d, u16QPtr: %d, u16QNum:%d, uid:%d %d %d %d\n",pHvdFrm->u32TimeStamp, u16QPtr, u16QNum, pHvdFrm->u32PrivateData, pShm->u16DispQNumb, pShm->u16DispQPtr, u16UsedFrm);
#if 0 // dump dolby metadata calculated by FW
                            unsigned char *dump_addr = (unsigned char *)((void *)pShm + pShm->u32HVD_DBG_DUMP_ADDR - (u8Idx * 0x100000 + HVD_SHARE_MEM_ST_OFFSET));
                            HVD_Frm_Information_EXT_Entry *pFrmInfoExt = NULL;
                            HVD_Frm_Information_EXT *pVsyncBridgeExt = (HVD_Frm_Information_EXT *)HAL_HVD_EX_GetDispQExtShmAddr(u32Id);
                            unsigned int i = 0;
                            unsigned char arr[33] = {0};
                            if(pVsyncBridgeExt != NULL)
                            {
                                pFrmInfoExt = &(pVsyncBridgeExt->stEntry[u16QPtr]);
                            }
                            dump_addr += 32 * pFrmInfoExt->u8CurrentIndex;
                            for (i = 0; i < 32; i++)
                            {
                                arr[i] = *(dump_addr + i);
                            }
                            HVD_PRINT("[md5]%d=%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", (unsigned int)pFrmInfoExt->u8CurrentIndex, arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8], arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15], arr[16], arr[17], arr[18], arr[19], arr[20], arr[21], arr[22], arr[23], arr[24], arr[25], arr[26], arr[27], arr[28], arr[29], arr[30], arr[31]);
#endif
                            HVD_Frm_Information *pPrevHvdFrm = (HVD_Frm_Information*)&pShm->DispQueue[u16QPtr - 1];//BL
                            if(DIFF(pPrevHvdFrm->u32TimeStamp, pHvdFrm->u32TimeStamp) > 1000)
                                HVD_EX_MSG_ERR("BL pts: %d, EL pts: %d matched failed!!\n",pPrevHvdFrm->u32TimeStamp, pHvdFrm->u32TimeStamp);
                        }
                        pHvdFrmPrv->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                        pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, (pHvdFrm->u32PrivateData << 16) | pHVDHalContext->_stHVDStream[u8Idx].u32PrivateData);
                    }
                    pHVDHalContext->_u16DispOutSideQPtr[u8Idx] = u16QPtr;
                    u16QPtr++;
                    if (u16QPtr == pShm->u16DispQSize) u16QPtr = 0;
                    pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = u16QPtr;

                    return (HVD_Frm_Information*)(MS_VIRT)pHvdFrm;
                }
            }

            return NULL;
        }

        //printf("OQ:%d,DQ:%d.\n",pShm->u16DispQNumb,pShm->u16DecQNumb);
        //search the next frame to display
        while (u16QNum > 0)
        {
            //printf("Pr:%d,%d.[%ld,%ld,%ld,%ld].\n",u16QPtr,u16QNum,pShm->DispQueue[u16QPtr].u32Status,pShm->DispQueue[u16QPtr+1].u32Status,
            //                pShm->DispQueue[u16QPtr+2].u32Status,pShm->DispQueue[u16QPtr+3].u32Status);
            pHVDHalContext->pHvdFrm = (HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

            //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                /// For MVC. Output views after the pair of (base and depend) views were decoded.
                /// Check the depned view was initial when Output the base view.
                if((u16QPtr%2) == 0)
                {
                    HVD_Frm_Information *pHvdFrm_sub = (HVD_Frm_Information *) &pShm->DispQueue[u16QPtr+1];
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
                HVD_EX_MSG_DBG("FrameDone: %d, pHvdFrm=0x%lx, timestamp=%d\n", u16QPtr,
                           (unsigned long) pHVDHalContext->pHvdFrm, pShm->DispQueue[u16QPtr].u32TimeStamp);
                HVD_EX_MSG_INF("<<< halHVD pts,idH = %lu, %lu [%x]\n", (unsigned long) pHVDHalContext->pHvdFrm->u32TimeStamp, (unsigned long) pHVDHalContext->pHvdFrm->u32ID_H, u16QPtr);     //STS output
                return (HVD_Frm_Information *)(MS_VIRT) pHVDHalContext->pHvdFrm;
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
    // pShm->DispInfo.u8Interlace : 0 = progressive, 1 = interlace field, 2 = interlace frame
    if (_HVD_EX_IsHevcInterlaceField(u32Id))
    {
        MS_U32 first_field = pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex == 1 ? 0 : 1;
        HVD_Frm_Information *pHvdFrm_first = NULL;

        if ((first_field && u16QNum < 2) || (u16QNum == 0)) {
            return NULL;
        }

        while (u16QNum != 0)
        {
            pHvdFrm = (HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];
            if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT) // Must Be
            {
                if (!first_field) // second get frame, we will check at least one paired in disp queue.
                {
                    pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, pHvdFrm->u32PrivateData);
                    pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = 0;

                    if(pHvdFrm->u8FieldType == EVD_TOP_FIELD || pHvdFrm->u8FieldType == EVD_TOP_WITH_PREV || pHvdFrm->u8FieldType == EVD_TOP_WITH_NEXT)
                        pHvdFrm->u8FieldType = 1; // 1 = E_VDEC_EX_FIELDTYPE_TOP
                    else
                        pHvdFrm->u8FieldType = 2; // 2 = E_VDEC_EX_FIELDTYPE_BOTTOM
                    return pHvdFrm;
                }
                else // first get frame, we will check at least one paired in disp queue.
                {
                    if (pHvdFrm_first == NULL)
                    {
                        pHvdFrm_first = pHvdFrm;
                    }
                    else
                    {
                        pHvdFrm_first->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                        HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, pHvdFrm_first->u32PrivateData);

                        if (pHvdFrm_first->u8FieldType == EVD_TOP_WITH_PREV || pHvdFrm_first->u8FieldType == EVD_BOTTOM_WITH_PREV)
                        {
                            if (pHvdFrm_first->u8FieldType == EVD_TOP_WITH_PREV && pHvdFrm->u8FieldType == EVD_BOTTOM_WITH_NEXT)
                            {
                                pHvdFrm_first->u32ID_L |= (1 << 16);
                                pHvdFrm->u32ID_L |= (1 << 16);
                            }
                            else if (pHvdFrm_first->u8FieldType == EVD_BOTTOM_WITH_PREV && pHvdFrm->u8FieldType == EVD_TOP_WITH_NEXT)
                            {
                                pHvdFrm_first->u32ID_L &= (~(1 << 16));
                                pHvdFrm->u32ID_L &= (~(1 << 16));
                            }
                            else
                            {
                                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RELEASE_DISPQ, pHvdFrm_first->u32PrivateData);
                                return NULL;
                            }
                        }
                        else
                        {
                            if (hevc_get_paired(pHvdFrm->u8FieldType, 0) != pHvdFrm_first->u8FieldType)
                            {
                                HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_RELEASE_DISPQ, pHvdFrm_first->u32PrivateData);
                                return NULL;
                            }
                        }

                        pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = 1;
                        if (pHvdFrm_first->u8FieldType == EVD_TOP_FIELD || pHvdFrm_first->u8FieldType == EVD_TOP_WITH_PREV || pHvdFrm_first->u8FieldType == EVD_TOP_WITH_NEXT)
                            pHvdFrm_first->u8FieldType = 1; // 1 = E_VDEC_EX_FIELDTYPE_TOP
                        else
                            pHvdFrm_first->u8FieldType = 2; // 2 = E_VDEC_EX_FIELDTYPE_BOTTOM
                        return pHvdFrm_first;
                    }
                }
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
    else
    {
        if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
        {

            while (u16QNum != 0)
            {
                pHvdFrm = (HVD_Frm_Information*) &pShm->DispQueue[u16QPtr];

                if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT) // Must Be
                {
                    pHVDHalContext->_u16DispOutSideQPtr[u8Idx] = u16QPtr;
                    pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;
                    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_UPDATE_DISPQ, pHvdFrm->u32PrivateData);
                    return (HVD_Frm_Information*)(MS_VIRT)pHvdFrm;
                }
                u16QNum--;
                //go to next frame in the dispQ
                if (bDolbyVision)
                    u16QPtr += 2; // single layer must in even ptr
                else
                u16QPtr++;

                if (u16QPtr >= pShm->u16DispQSize)
                {
                    u16QPtr = 0;        //wrap to the begin
                }
            }

            return NULL;
        }

        //printf("Q: %d %d\n", u16QNum, u16QPtr);
        //search the next frame to display
        while (u16QNum != 0)
        {
            pHVDHalContext->pHvdFrm = (HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];

            //printf("Q2: %ld\n", pHVDShareMem->DispQueue[u16QPtr].u32Status);
            if (pHVDHalContext->pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                pHVDHalContext->_u16DispQPtr = u16QPtr;
                pHVDHalContext->pHvdFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;       /////Change its state!!
                HVD_EX_MSG_DBG("FrameDone: %d, pHvdFrm=0x%lx, timestamp=%d\n", u16QPtr,
                            (unsigned long) pHVDHalContext->pHvdFrm, pShm->DispQueue[u16QPtr].u32TimeStamp);
                HVD_EX_MSG_INF("<<< halHVD pts,idH = %u, %u [%x]\n", pHVDHalContext->pHvdFrm->u32TimeStamp, pHVDHalContext->pHvdFrm->u32ID_H, u16QPtr);     //STS output
                return (HVD_Frm_Information *)(MS_VIRT) pHVDHalContext->pHvdFrm;
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

static HVD_Frm_Information_EXT_Entry *_HVD_EX_GetNextDispFrameExt(MS_U32 u32Id)
{
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_Frm_Information_EXT_Entry *pFrmInfoExt = NULL;
    if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
    {
        HVD_Frm_Information_EXT *pVsyncBridgeExt = (HVD_Frm_Information_EXT *)HAL_HVD_EX_GetDispQExtShmAddr(u32Id);
        if(pVsyncBridgeExt != NULL)
        {
            pFrmInfoExt = &(pVsyncBridgeExt->stEntry[pHVDHalContext->_u16DispOutSideQPtr[u8Idx]]);
        }
    }
    return pFrmInfoExt;
}

static MS_U64 _HAL_EX_GetHwMaxPixel(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U64 u64Ret = 0;

#if SUPPORT_EVD
    MS_BOOL isEVD = _HAL_EX_IS_EVD(pCtrl->InitParams.u32ModeFlag);
    if (isEVD)
    {
        u64Ret = (MS_U64)HEVC_HW_MAX_PIXEL;
    }
    else
#endif
#if SUPPORT_G2VP9
    if (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        u64Ret = (MS_U64)VP9_HW_MAX_PIXEL;
    }
    else
#endif
    {
        u64Ret = (MS_U64)HVD_HW_MAX_PIXEL;
    }

    return u64Ret;
}

MS_BOOL
HAL_HVD_EX_DispFrameAllViewed(MS_U32 u32Id)
{
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U16 u16QNum = pShm->u16DispQNumb;
    MS_U16 u16QPtr = pShm->u16DispQPtr;
    static volatile HVD_Frm_Information *pHvdFrm = NULL;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL bDolbyVision = (E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK));
    MS_BOOL bMVC = FALSE;
#if HVD_ENABLE_MVC
    bMVC = HAL_HVD_EX_CheckMVCID(u32Id);
#endif


    if (bMVC || (bDolbyVision && !pShm->bSingleLayer) || _HVD_EX_IsHevcInterlaceField(u32Id))
    {
        if (u16QNum == 1) return TRUE;
    }

    while (u16QNum != 0)
    {
        pHvdFrm = (volatile HVD_Frm_Information *) &pShm->DispQueue[u16QPtr];
        if (pHvdFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
        {
            return FALSE;
        }
        u16QNum--;

        if (bDolbyVision)
            u16QPtr += 2; // single layer must in even ptr
        else
        u16QPtr++;

        if (u16QPtr >= pShm->u16DispQSize)
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

MS_U8 _HVD_EX_GetStreamIdx(MS_U32 u32Id)
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
#ifdef VDEC3
        case E_HAL_VPU_N_STREAM_BASE:
        {
            u8OffsetIdx = (u32Id>>8) & 0xF;
            break;
        }
#endif
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

MS_VIRT HAL_HVD_EX_GetShmAddr(MS_U32 u32Id)
{
    MS_PHY u32PhyAddr = 0x0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if (pCtrl->MemMap.u32CodeBufAddr == 0)
    {
        return 0;
    }

    u32PhyAddr = HAL_VPU_EX_GetShareInfoAddr(u32Id);

    if (u32PhyAddr == 0xFFFFFFFF) //boris
    {
        u32PhyAddr = pCtrl->MemMap.u32CodeBufAddr + (HAL_VPU_EX_GetTaskId(u32Id) * HVD_FW_MEM_OFFSET) + HVD_SHARE_MEM_ST_OFFSET;
    }
    else
    {
        // TEE, common + share_info
        u32PhyAddr += COMMON_AREA_SIZE;
    }

    return MsOS_PA2KSEG1(u32PhyAddr);
}

MS_VIRT HAL_HVD_EX_GetDispQExtShmAddr(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    if (pCtrl->MemMap.u32CodeBufAddr == 0 || pShm == NULL)
    {
        return 0;
    }

    MS_PHY u32PhyAddr = 0x0;
#if 0
    u32PhyAddr = HAL_VPU_EX_GetShareInfoAddr(u32Id);

    if (u32PhyAddr == 0xFFFFFFFF)
    {
        u32PhyAddr = pCtrl->MemMap.u32CodeBufAddr + (HAL_VPU_EX_GetTaskId(u32Id) * HVD_FW_MEM_OFFSET);
    }
#endif
    u32PhyAddr = pCtrl->MemMap.u32CodeBufAddr;
    u32PhyAddr += pShm->u32DISPQUEUE_EXT_ST_ADDR; //with HVD_FW_MEM_OFFSET

    return MsOS_PA2KSEG1(u32PhyAddr);
}

void HAL_HVD_MIF1_MiuClientSel(MS_U8 u8MiuSel)
{

    if (u8MiuSel == E_CHIP_MIU_0)
    {
        _HVD_WriteWordMask(MIU0_CLIENT_SELECT_GP4, 0, MIU0_CLIENT_SELECT_GP4_HVD_MIF1);
    }
    else if (u8MiuSel == E_CHIP_MIU_1)
    {
        _HVD_WriteWordMask(MIU0_CLIENT_SELECT_GP4, MIU0_CLIENT_SELECT_GP4_HVD_MIF1, MIU0_CLIENT_SELECT_GP4_HVD_MIF1);
    }
}

#if SUPPORT_G2VP9 && defined(VDEC3)
#ifdef __ARM_NEON__
#include <arm_neon.h>
static void tile4x4_to_raster_8(MS_U8* raster, MS_U8* tile, MS_U32 stride, MS_U32 tile_w, MS_U32 tile_h)
{
    uint32x4x4_t data, data2;
    MS_U8* raster2 = raster + tile_w * 4;

    data = vld4q_u32((const uint32_t *)tile);
    data2 = vld4q_u32((const uint32_t *)(tile + tile_w * tile_h * 4));

    vst1q_u32((uint32_t *)raster, data.val[0]);
    raster += stride;
    vst1q_u32((uint32_t *)raster, data.val[1]);
    raster += stride;
    vst1q_u32((uint32_t *)raster, data.val[2]);
    raster += stride;
    vst1q_u32((uint32_t *)raster, data.val[3]);


    vst1q_u32((uint32_t *)raster2, data2.val[0]);
    raster2 += stride;
    vst1q_u32((uint32_t *)raster2, data2.val[1]);
    raster2 += stride;
    vst1q_u32((uint32_t *)raster2, data2.val[2]);
    raster2 += stride;
    vst1q_u32((uint32_t *)raster2, data2.val[3]);
}
#else
static void tile4x4_to_raster_4(MS_U8* raster, MS_U8* tile, MS_U32 stride)
{
    MS_U8* tile0 = tile;
    MS_U8* tile1 = tile+16;
    MS_U8* tile2 = tile+32;
    MS_U8* tile3 = tile+48;
    int i;

    for (i=0; i<4; i++) {
        raster[i]              = tile0[i];
        raster[4+i]            = tile1[i];
        raster[8+i]            = tile2[i];
        raster[12+i]           = tile3[i];
    }

    for (i=0; i<4; i++) {
        raster[stride+i]       = tile0[4+i];
        raster[stride+4+i]     = tile1[4+i];
        raster[stride+8+i]     = tile2[4+i];
        raster[stride+12+i]    = tile3[4+i];
    }

    for (i=0; i<4; i++) {
        raster[2*stride+i]     = tile0[8+i];
        raster[2*stride+4+i]   = tile1[8+i];
        raster[2*stride+8+i]   = tile2[8+i];
        raster[2*stride+12+i]   = tile3[8+i];
    }

    for (i=0; i<4; i++) {
        raster[3*stride+i]     = tile0[12+i];
        raster[3*stride+4+i]   = tile1[12+i];
        raster[3*stride+8+i]   = tile2[12+i];
        raster[3*stride+12+i]   = tile3[12+i];
    }
}
#endif // #ifdef __ARM_NEON__

static MS_BOOL _HVD_EX_PpTask_Create(MS_U32 u32Id, HVD_EX_Stream *pstHVDStream)
{
    MS_S32 s32HvdPpTaskId = MsOS_CreateTask((TaskEntry)_HAL_HVD_EX_PostProc_Task,
                                            u32Id,
                                            E_TASK_PRI_MEDIUM,
                                            TRUE,
                                            NULL,
                                            32, // stack size..
                                            "HVD_PostProcess_task");

    if (s32HvdPpTaskId < 0)
    {
        HVD_EX_MSG_ERR("Pp Task create failed\n");

        return FALSE;
    }

    HVD_EX_MSG_DBG("Pp Task create success\n");
    pstHVDStream->s32HvdPpTaskId = s32HvdPpTaskId;

    return TRUE;
}

static MS_U32 tile_offset(MS_U32 x, MS_U32 y, MS_U32 w, MS_U32 h, MS_U32 stride)
{
    return y * stride * h + x * w * h;
}

static MS_U32 raster_offset(MS_U32 x, MS_U32 y, MS_U32 w, MS_U32 h, MS_U32 stride)
{
    return y * stride * h + x * w;
}

static void tile4x4_to_raster(MS_U8* raster, MS_U8* tile, MS_U32 stride)
{
    raster[0]              = tile[0];
    raster[1]              = tile[1];
    raster[2]              = tile[2];
    raster[3]              = tile[3];
    raster[stride]         = tile[4];
    raster[stride + 1]     = tile[5];
    raster[stride + 2]     = tile[6];
    raster[stride + 3]     = tile[7];
    raster[2 * stride]     = tile[8];
    raster[2 * stride + 1] = tile[9];
    raster[2 * stride + 2] = tile[10];
    raster[2 * stride + 3] = tile[11];
    raster[3 * stride]     = tile[12];
    raster[3 * stride + 1] = tile[13];
    raster[3 * stride + 2] = tile[14];
    raster[3 * stride + 3] = tile[15];
}

static void tiled4x4pic_to_raster_new(MS_U8* dst, MS_U8* src, MS_U32 w, MS_U32 h, MS_U32 raster_stride)
{
    const MS_U32 tile_w = 4;
    const MS_U32 tile_h = 4;
    MS_U32 tile_stride = w;
    MS_U32 x, y;
    MS_U8 *dst1, *dst2;
    MS_U8 *src1, *src2;

#ifdef __ARM_NEON__
    // To overlap load and store, handle two blocks at the same time.
    dst1 = dst;
    src1 = src;
    for (y = 0; y < h / tile_h; y++)
    {
        dst2 = dst1;
        src2 = src1;
        for (x = 0; x <= (w/tile_w - 8); x+=8)
        {
            tile4x4_to_raster_8(
                                dst2,
                                src2,
                                raster_stride, tile_w, tile_h);
            dst2 += tile_w * 8;
            src2 += tile_w * tile_h * 8;
        }
        dst1 += raster_stride * tile_h;
        src1 += tile_stride * tile_h;
        for (; x < w / tile_w; x++)
        {
            tile4x4_to_raster(
                            dst + raster_offset(x, y, tile_w, tile_h, raster_stride),
                            src + tile_offset(x, y, tile_w, tile_h, tile_stride),
                            raster_stride);
        }
    }
#else
    dst1 = NULL;
    src1 = NULL;
    dst2 = NULL;
    src2 = NULL;

    for (y = 0; y < h / tile_h; y++)
    {
        for (x = 0; x <= (w/tile_w - 4); x+=4)
        {
            tile4x4_to_raster_4(
                                dst + raster_offset(x, y, tile_w, tile_h, raster_stride),
                                src + tile_offset(x, y, tile_w, tile_h, tile_stride),
                                raster_stride);
        }
        for (; x < w / tile_w; x++)
        {
            tile4x4_to_raster(
                            dst + raster_offset(x, y, tile_w, tile_h, raster_stride),
                            src + tile_offset(x, y, tile_w, tile_h, tile_stride),
                            raster_stride);
        }
    }
#endif
}

#define FLUSH_CACHE_SIZE (256 * 1024)

static void _HAL_HVD_EX_Inv_Cache(void *pVA, MS_U32 u32Size)
{
    // To improve performance, just flush the first FLUSH_CACHE_SIZE bytes of data
    if (u32Size > FLUSH_CACHE_SIZE)
        u32Size = FLUSH_CACHE_SIZE;

    MsOS_MPool_Dcache_Flush((MS_VIRT)pVA, u32Size);
}

static void _HAL_HVD_EX_Flush_Cache(void *pVA, MS_U32 u32Size)
{
    MS_U32 u32SkipSize = 0;

    // To improve performance, just flush the last FLUSH_CACHE_SIZE bytes of data
    if (u32Size > FLUSH_CACHE_SIZE)
    {
        u32SkipSize = u32Size - FLUSH_CACHE_SIZE;
        u32Size = FLUSH_CACHE_SIZE;
    }

    MsOS_MPool_Dcache_Flush(((MS_VIRT)pVA) + u32SkipSize, u32Size);
}

static MS_BOOL _HAL_HVD_EX_PostProc_Task(MS_U32 u32Id)
{
    HVD_EX_Stream *pstHVDStream = pHVDHalContext->_stHVDStream + _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U32 u32SrcMiuSel, u32DstMiuSel;
    MS_U16 u16Width = 0, u16Height = 0, u16TileWidth = 0;

    HVD_EX_MSG_DBG("[%s-%d] Start\n", __FUNCTION__, __LINE__);

    pstHVDStream->ePpTaskState = E_HAL_HVD_STATE_RUNNING;

    while (pstHVDStream->ePpTaskState != E_HAL_HVD_STATE_STOP)
    {
        if (pstHVDStream->ePpTaskState == E_HAL_HVD_STATE_PAUSING)
            pstHVDStream->ePpTaskState = E_HAL_HVD_STATE_PAUSE_DONE;

        HVD_Delay_ms(1); // FIXME

        if (pstHVDStream->ePpTaskState != E_HAL_HVD_STATE_RUNNING)
            continue;

        HAL_HVD_EX_ReadMemory();

        while (pShm->u8PpQueueRPtr != pShm->u8PpQueueWPtr)
        {
            MS_U8 *pSrcVA, *pDstVA;
            MS_U32 u32SrcPA, u32DstPA;
            HVD_Frm_Information *pFrmInfo = (HVD_Frm_Information *)&pShm->DispQueue[pShm->u8PpQueueRPtr];
            //HVD_EX_MSG_DBG("[%s-%d] width: %d, height = %d, pitch = %d\n", __FUNCTION__, __LINE__, pFrmInfo->u16Width, pFrmInfo->u16Height, pFrmInfo->u16Pitch);

            if ((u16Width != pFrmInfo->u16Width) || (u16Height != pFrmInfo->u16Height))
            {
                HVD_Display_Info *pDispInfo = (HVD_Display_Info *) HAL_HVD_EX_GetData(u32Id, E_HVD_GDATA_DISP_INFO_ADDR);

                u16Width = pFrmInfo->u16Width;
                u16Height = pFrmInfo->u16Height;
                u16TileWidth = NEXT_MULTIPLE(pFrmInfo->u16Pitch - pDispInfo->u16CropRight, 8);
            }

            // Luma
            u32SrcMiuSel = (pShm->u32VDEC_MIU_SEL >> VDEC_PPIN_MIUSEL) & VDEC_MIUSEL_MASK;
            u32DstMiuSel = (pShm->u32VDEC_MIU_SEL >> VDEC_LUMA8_MIUSEL) & VDEC_MIUSEL_MASK;

            _miu_offset_to_phy(u32SrcMiuSel, pFrmInfo->u32PpInLumaAddr, u32SrcPA);
            _miu_offset_to_phy(u32DstMiuSel, pFrmInfo->u32LumaAddr, u32DstPA);

            pSrcVA = (MS_U8*) MS_PA2KSEG0(u32SrcPA);
            pDstVA = (MS_U8*) MS_PA2KSEG0(u32DstPA);

            _HAL_HVD_EX_Inv_Cache(pSrcVA, u16TileWidth * pFrmInfo->u16Height);

            tiled4x4pic_to_raster_new(pDstVA, pSrcVA, u16TileWidth, pFrmInfo->u16Height, pFrmInfo->u16Pitch);

            _HAL_HVD_EX_Flush_Cache(pDstVA, pFrmInfo->u16Pitch * pFrmInfo->u16Height);

            // Chroma
            u32SrcMiuSel = (pShm->u32VDEC_MIU_SEL >> VDEC_PPIN_MIUSEL) & VDEC_MIUSEL_MASK;
            u32DstMiuSel = (pShm->u32VDEC_MIU_SEL >> VDEC_CHROMA8_MIUSEL) & VDEC_MIUSEL_MASK;

            _miu_offset_to_phy(u32SrcMiuSel, pFrmInfo->u32PpInChromaAddr, u32SrcPA);
            _miu_offset_to_phy(u32DstMiuSel, pFrmInfo->u32ChromaAddr, u32DstPA);

            pSrcVA = (MS_U8*) MS_PA2KSEG0(u32SrcPA);
            pDstVA = (MS_U8*) MS_PA2KSEG0(u32DstPA);

            _HAL_HVD_EX_Inv_Cache(pSrcVA, u16TileWidth * pFrmInfo->u16Height / 2);

            tiled4x4pic_to_raster_new(pDstVA, pSrcVA, u16TileWidth, pFrmInfo->u16Height/2, pFrmInfo->u16Pitch);

            _HAL_HVD_EX_Flush_Cache(pDstVA, pFrmInfo->u16Pitch * pFrmInfo->u16Height / 2);

            pShm->DispQueue[pShm->u8PpQueueRPtr].u32Status = E_HVD_DISPQ_STATUS_INIT;
            HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_INC_DISPQ_NUM, 0);
            INC_VALUE(pShm->u8PpQueueRPtr, pShm->u8PpQueueSize);

            HAL_HVD_EX_FlushMemory();

            if (pstHVDStream->ePpTaskState == E_HAL_HVD_STATE_PAUSING)
                break;

            HAL_HVD_EX_ReadMemory();
        }
    }

    HVD_EX_MSG_DBG("[%s-%d] End\n", __FUNCTION__, __LINE__);

    return TRUE;
}
#endif

static void HAL_HVD_EX_VP8AECInUsed(MS_U32 u32Id, MS_BOOL *isVP8Used, MS_BOOL *isAECUsed , MS_BOOL *isAVCUsed)
{
    MS_U8 i ;
    MS_U8 u8DrvId = (0xFF & (u32Id >> 16));

    for (i = 0; i < HAL_HVD_EX_MAX_SUPPORT_STREAM ; i++)
    {
        if( _pHVDCtrls[i].bUsed && (i != u8DrvId))
        {
            MS_U32 u32TempModeFlag = (_pHVDCtrls[i].InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) ;
            if((E_HVD_INIT_HW_VP8 == u32TempModeFlag))
            {
                *isVP8Used = TRUE ;
            }
            else if((E_HVD_INIT_HW_VP9 == u32TempModeFlag) || (E_HVD_INIT_HW_AVS == u32TempModeFlag))
            {
                *isAECUsed = TRUE ;
            }
            else if((E_HVD_INIT_HW_AVC == u32TempModeFlag))
            {
                *isAVCUsed = TRUE ;
            }
        }
    }
}

MS_BOOL HAL_HVD_EX_InitHW(MS_U32 u32Id,VPU_EX_DecoderType DecoderType)
{
#ifndef VDEC3
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL isVP8Used = FALSE;
    MS_BOOL isAECUsed = FALSE;
    MS_BOOL isAVCUsed = FALSE;
    HAL_HVD_EX_VP8AECInUsed(u32Id, &isVP8Used, &isAECUsed, &isAVCUsed);
//    MS_U8 u8MiuSel;
//    MS_U32 u32StartOffset;

#if SUPPORT_EVD
    MS_BOOL isEVD = _HAL_EX_IS_EVD(pCtrl->InitParams.u32ModeFlag);
#else
    MS_BOOL isEVD = FALSE;
#endif
    MS_BOOL isHVD = !isEVD;

    //patch for enable evd in AVC because AVC may enable mf_codec which need evd registers
    isEVD = isEVD || (E_HVD_INIT_HW_AVC== (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK));

    // power on / reset HVD; set nal, es rw, bbu parser, release HVD engine
    // re-setup clock.
    #if SUPPORT_G2VP9 && defined(VDEC3)
    if (E_HVD_INIT_HW_VP9 != (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    #endif


    if (isHVD)
    {
        if (!HAL_VPU_EX_HVDInUsed())
        {
            printf("HVD power on\n");
            HAL_HVD_EX_PowerCtrl(u32Id, TRUE);
        }

        if(!isVP8Used && (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
        {
            HAL_VP8_PowerCtrl(TRUE);
        }
        else if(!isAECUsed && (E_HVD_INIT_HW_AVS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
        {
            HAL_AEC_PowerCtrl(TRUE);
        }

#ifdef CONFIG_MSTAR_SRAMPD
        _HVD_WriteByteMask(REG_HICODEC_SRAM_SD_EN, HICODEC_SRAM_HICODEC1, HICODEC_SRAM_HICODEC1);
        HVD_Delay_ms(1);
#endif
    }

#if SUPPORT_EVD
#ifdef VDEC3
    if (isEVD)  /// Disable it for disable H264 IMI
    {
        if (!HAL_VPU_EX_EVDInUsed())
        {
            printf("EVD power on\n");
            HAL_EVD_EX_PowerCtrl(u32Id, TRUE);
        }
        if(!isAECUsed && (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
        {
            HAL_AEC_PowerCtrl(TRUE);
        }
    }
#endif
#endif

    #if SUPPORT_G2VP9 && defined(VDEC3)
    if (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        if (!HAL_VPU_EX_G2VP9InUsed())
        {
            printf("G2 VP9 power on\n");
            HAL_VP9_EX_PowerCtrl(TRUE);
        }
    }
    #endif

    if ((!HAL_VPU_EX_HVDInUsed()) )
    {
        pHVDHalContext->_stHVDStream[0].u32BBUWptr = 0; //main
        pHVDHalContext->_stHVDStream[1].u32BBUWptr = 0; //sub
        pHVDHalContext->u32VP8BBUWptr = 0; //VP8
        _HVD_EX_ResetMainSubBBUWptr(u32Id);

        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST, HVD_REG_RESET_SWRST);

        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_IDB_MIU_256 , HVD_REG_RESET_IDB_MIU_256);
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_MC_MIU_256 , HVD_REG_MC_MIU_256);
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_MIU_256 , HVD_REG_RESET_MIU_256);
        _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_MIU1_256 , HVD_REG_RESET_MIU1_256);
        _HVD_WriteWordMask(HVD_REG_RESET, 0 , HVD_REG_RESET_MIU_128);
        _HVD_WriteWordMask(HVD_REG_RESET, 0 , HVD_REG_RESET_MIU1_128);

        #if 0
        if((pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.bEnable) &&
           ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC))
        {
            _phy_to_miu_offset(u8MiuSel, u32StartOffset, pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.u32IapGnBufAddr);

            _HAL_HVD_Entry();
            HAL_HVD_MIF1_MiuClientSel(u8MiuSel);
            _HAL_HVD_Release();

        }
        #endif
    }

    #if SUPPORT_EVD
    if (isEVD)
    {
#ifdef VDEC3
        if (!HAL_VPU_EX_EVDInUsed())
#endif
        _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_SWRST, EVD_REG_RESET_SWRST);
    }
    #endif

    #if SUPPORT_G2VP9 && defined(VDEC3)
    if (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        if (!HAL_VPU_EX_G2VP9InUsed())
            _HVD_WriteWordMask(VP9_REG_RESET, VP9_REG_RESET_SWRST, VP9_REG_RESET_SWRST);
    }
    #endif


    if(pCtrl == NULL)
    {
        HVD_EX_MSG_ERR("HAL_HVD_EX_InitHW Ctrl is NULL.\n");
        //return FALSE;
        goto RESET;
    }

#if SUPPORT_EVD
    if (isEVD && ((E_HVD_INIT_HW_AVC != (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)) ))
    {
#ifdef VDEC3
        if (!HAL_VPU_EX_EVDInUsed())
#endif
        {
            if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
                _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_HK_HEVC_MODE, EVD_REG_RESET_HK_HEVC_MODE);
        }

        if ((E_HVD_INIT_MAIN_LIVE_STREAM == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK))
            ||(E_HVD_INIT_MAIN_FILE_TS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_MAIN_MASK)))
        {
#ifdef VDEC3
            if (0 == pCtrl->u32BBUId)
#else
            if (0 == u8TaskId)
#endif
            {
                _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_HK_TSP2EVD_EN, EVD_REG_RESET_HK_TSP2EVD_EN); //for main-DTV mode
            }
            else
            {
                _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_USE_HVD_MIU_EN, EVD_REG_RESET_USE_HVD_MIU_EN);  //for sub-DTV mode
            }

        }
        goto RESET;
    }
#endif

    // HVD4, from JANUS and later chip
    switch ((pCtrl->InitParams.u32ModeFlag) & E_HVD_INIT_HW_MASK)
    {
        case E_HVD_INIT_HW_AVS:
        {
#ifdef VDEC3
            if (0 == pCtrl->u32BBUId)
#else
            if (0 == u8TaskId)
#endif
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
#ifdef VDEC3
            if (0 == pCtrl->u32BBUId)
#else
            if (0 == u8TaskId)
#endif
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
#ifdef VDEC3
            if (0 == pCtrl->u32BBUId)
#else
            if (0 == u8TaskId)
#endif
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

#if 0    //use miu256bit
    HVD_EX_MSG_DBG("(be)Miu128 bits Status = %x <<<<<<<\n", _HVD_Read2Byte(HVD_REG_RESET));

    if (!HAL_VPU_EX_HVDInUsed())
    {
        _HVD_Write2Byte(HVD_REG_RESET, (_HVD_Read2Byte(HVD_REG_RESET) | HVD_REG_RESET_MIU_128));
    }

     HVD_EX_MSG_DBG("(af)Miu128 bits Status = %x <<<<<<<\n", _HVD_Read2Byte(HVD_REG_RESET));
#endif

#if SUPPORT_EVD
    if (isEVD)
    {
#ifdef VDEC3
        if (!HAL_VPU_EX_EVDInUsed())
#endif
        {
            printf("EVD miu 256 bits\n");
            _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) & ~EVD_REG_RESET_MIU0_128 & ~EVD_REG_RESET_MIU1_128));
            _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) | EVD_REG_RESET_MIU0_256 | EVD_REG_RESET_MIU1_256));
            _HVD_Write2Byte(REG_CLK_EVD, (_HVD_Read2Byte(REG_CLK_EVD) & ~REG_CLK_EVD_SW_OV_EN & ~REG_CLK_EVD_PPU_SW_OV_EN));//set 0 firmware
            //_HVD_Write2Byte(REG_CLK_EVD, (_HVD_Read2Byte(REG_CLK_EVD) | REG_CLK_EVD_SW_OV_EN | REG_CLK_EVD_PPU_SW_OV_EN));//set 1 driver
            printf("EVD BBU 256 bits\n");
            _HVD_Write2Byte(EVD_BBU_MIU_SETTING, (_HVD_Read2Byte(EVD_BBU_MIU_SETTING) & ~REG_BBU_MIU_128));
            _HVD_Write2Byte(EVD_BBU_MIU_SETTING, (_HVD_Read2Byte(EVD_BBU_MIU_SETTING) | REG_BBU_MIU_256));
        }
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
    if (isEVD)
    {
#ifdef VDEC3
        if (!HAL_VPU_EX_EVDInUsed())
#endif
        _HVD_WriteWordMask(EVD_REG_RESET, 0, EVD_REG_RESET_SWRST);
    }
#endif

#if SUPPORT_G2VP9 && defined(VDEC3)
    if (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

        if (!HAL_VPU_EX_G2VP9InUsed())
            _HVD_WriteWordMask(VP9_REG_RESET, 0, VP9_REG_RESET_SWRST);

        if (pShm->u8FrmPostProcSupport & E_HVD_POST_PROC_DETILE)
            _HVD_EX_PpTask_Create(u32Id, &pHVDHalContext->_stHVDStream[_HVD_EX_GetStreamIdx(u32Id)]);
    }
#endif

    return TRUE;
}

MS_BOOL HAL_HVD_EX_DeinitHW(MS_U32 u32Id)
{
    MS_U16 u16Timeout = 1000;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL isVP8Used = FALSE;
    MS_BOOL isAECUsed = FALSE;
    MS_BOOL isAVCUsed = FALSE;
    HAL_HVD_EX_VP8AECInUsed(u32Id, &isVP8Used, &isAECUsed , &isAVCUsed);

#if SUPPORT_EVD
    if(!isAVCUsed && E_HVD_INIT_HW_AVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) && !HAL_VPU_EX_EVDInUsed())
    {
         HAL_EVD_EX_DeinitHW(u32Id);//no AVC/EVD use , close EVD power
    }
#endif

    if(TRUE == HAL_VPU_EX_HVDInUsed())
    {
         #if 0 //Power control close Vp8 register in dynamic ,but bs4-nal-ready could be off.(Patch for maxim and maserati)
         if(!isVP8Used && E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
             HAL_VP8_PowerCtrl(FALSE);
         }
         else
         #endif
         if(!isAECUsed && E_HVD_INIT_HW_AVS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
             HAL_AEC_PowerCtrl(FALSE);
         }
         return FALSE;
    }
    else
    {
         _HVD_EX_SetMIUProtectMask(TRUE);

         _HVD_WriteWordMask(HVD_REG_RESET, HVD_REG_RESET_SWRST, HVD_REG_RESET_SWRST);

         while (u16Timeout)
         {
             if ((_HVD_Read2Byte(HVD_REG_RESET) & (HVD_REG_RESET_SWRST_FIN)) == (HVD_REG_RESET_SWRST_FIN))
             {
                 break;
             }
             u16Timeout--;
         }

         HAL_HVD_EX_PowerCtrl(u32Id, FALSE);

         //Power control close Vp8 register in dynamic ,but bs4-nal-ready could be off.(Patch for maxim and maserati)
         HAL_VP8_PowerCtrl(FALSE);
         #if 0
         if(!isVP8Used && E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
             HAL_VP8_PowerCtrl(FALSE);
         }
         else
         #endif
         if(!isAECUsed && E_HVD_INIT_HW_AVS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
             HAL_AEC_PowerCtrl(FALSE);
         }

#ifdef CONFIG_MSTAR_SRAMPD
         _HVD_WriteByteMask(REG_HICODEC_SRAM_SD_EN, ~HICODEC_SRAM_HICODEC1, HICODEC_SRAM_HICODEC1);
         HVD_Delay_ms(1);
#endif

         _HVD_EX_SetMIUProtectMask(FALSE);

         return TRUE;
    }

    return FALSE;
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
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX_KERNEL))
#if !defined(SUPPORT_X_MODEL_FEATURE)
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
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
#ifdef VDEC3
    else if ((eStreamType >= E_HAL_HVD_N_STREAM) && (eStreamType < E_HAL_HVD_N_STREAM + HAL_HVD_EX_MAX_SUPPORT_STREAM))
    {
        i = eStreamType - E_HAL_HVD_N_STREAM;
        if (!pHVDHalContext->_stHVDStream[i].bUsed)
            return pHVDHalContext->_stHVDStream[i].eStreamId;
    }
#endif

    return E_HAL_HVD_STREAM_NONE;
}

static void HAL_VP8_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_VP8, ~TOP_CKG_VP8_DIS, TOP_CKG_VP8_DIS);

        switch (pHVDHalContext->u32HVDClockType)
        {
            case 384:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_288MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 345:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_288MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 320:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_288MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 288:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_288MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 240:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_240MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 216:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_216MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            case 172:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_216MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
            default:
            {
                _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_288MHZ,     TOP_CKG_VP8_CLK_MASK);
                break;
            }
        }
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_VP8, TOP_CKG_VP8_DIS, TOP_CKG_VP8_DIS);
    }

}

static void HAL_AEC_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_HVD_AEC, ~TOP_CKG_HVD_AEC_DIS, TOP_CKG_HVD_AEC_DIS);

        switch (pHVDHalContext->u32HVDClockType)
        {
            case 384:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_288MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 345:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_288MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 320:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_288MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 288:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_288MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 240:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_240MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 216:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_216MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            case 172:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_216MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
            default:
            {
                _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_288MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
                break;
            }
        }
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_DIS, TOP_CKG_HVD_AEC_DIS);
    }

}

void HAL_HVD_EX_PowerCtrl(MS_U32 u32Id, MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_HVD, ~TOP_CKG_HVD_DIS, TOP_CKG_HVD_DIS);
        //_HVD_WriteWordMask(REG_TOP_HVD_IDB, ~TOP_CKG_HVD_IDB_DIS, TOP_CKG_HVD_IDB_DIS);
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_HVD, TOP_CKG_HVD_DIS, TOP_CKG_HVD_DIS);
        //_HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_DIS, TOP_CKG_HVD_IDB_DIS);
    }

    // fix to not inverse
    _HVD_WriteWordMask(REG_TOP_HVD, ~TOP_CKG_HVD_INV, TOP_CKG_HVD_INV);

    switch (pHVDHalContext->u32HVDClockType)
    {
        #if 0  //for overclocking
        case 432:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_432MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_480MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_AEC, TOP_CKG_HVD_AEC_320MHZ, TOP_CKG_HVD_AEC_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_VP8,     TOP_CKG_VP8_320MHZ,     TOP_CKG_VP8_CLK_MASK);
            break;
        }
        #endif
        case 384:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_384MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_432MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 345:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_345MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_384MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 320:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_320MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_345MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 288:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_288MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_320MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 240:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_240MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_288MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 216:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_216MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_240MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
        case 172:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_172MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_216MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }

        default:
        {
            _HVD_WriteWordMask(REG_TOP_HVD,     TOP_CKG_HVD_384MHZ,     TOP_CKG_HVD_CLK_MASK);
            _HVD_WriteWordMask(REG_TOP_HVD_IDB, TOP_CKG_HVD_IDB_432MHZ, TOP_CKG_HVD_IDB_CLK_MASK);
            break;
        }
    }

    return;
}

void HAL_HVD_EX_InitRegBase(MS_VIRT u32RegBase)
{
    u32HVDRegOSBase = u32RegBase;
    HAL_VPU_EX_InitRegBase(u32RegBase);
}

void HAL_HVD_EX_SetPreCtrlVariables(MS_U32 u32Id,MS_VIRT drvprectrl)
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
    pHVDHalContext->_stHVDStream[u8Idx].bfirstGetFrmInfoDone = TRUE;
    int i;
    for(i = 0; i<HAL_HVD_EX_MAX_SUPPORT_STREAM;i++)
        pHVDHalContext->_s32VDEC_BBU_TaskId[i] = -1;
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
#ifdef VDEC3_FB
        pHVDHalContext->u32RV_VLCTableAddr = 0;
#else
        if (pCtrl->MemMap.u32FrameBufSize > RV_VLC_TABLE_SIZE)
        {
            pHVDHalContext->u32RV_VLCTableAddr = pCtrl->MemMap.u32FrameBufSize - RV_VLC_TABLE_SIZE;
            pCtrl->MemMap.u32FrameBufSize -= RV_VLC_TABLE_SIZE;
        }
        else
        {
            HVD_EX_MSG_ERR("HAL_HVD_EX_InitVariables failed: frame buffer size too small. FB:%x min:%x\n",
                        (MS_U32) pCtrl->MemMap.u32FrameBufSize, (MS_U32) RV_VLC_TABLE_SIZE);
            return E_HVD_RETURN_INVALID_PARAMETER;
        }
#endif
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
        || ((MS_VIRT) (pCtrl->MemMap.u32CodeBufVAddr <= (MS_VIRT) pShm) && ((MS_VIRT) pShm <= (pCtrl->MemMap.u32CodeBufVAddr + pCtrl->MemMap.u32CodeBufSize)))
        || ((MS_VIRT) (pCtrl->MemMap.u32BitstreamBufVAddr <= (MS_VIRT) pShm) && ((MS_VIRT) pShm <= (pCtrl->MemMap.u32BitstreamBufVAddr + pCtrl->MemMap.u32BitstreamBufSize)))
        || ((MS_VIRT) (pCtrl->MemMap.u32FrameBufVAddr <= (MS_VIRT) pShm) && ((MS_VIRT) pShm <= (pCtrl->MemMap.u32FrameBufVAddr + pCtrl->MemMap.u32FrameBufSize))))
    {
        HVD_EX_MSG_DBG("input memory: Code addr=0x%lx, Bits addr=0x%lx, FB addr=0x%lx, Miu1base=0x%lx, Miu2base=0x%lx\n",
                    (unsigned long)pCtrl->MemMap.u32CodeBufAddr,
                    (unsigned long)pCtrl->MemMap.u32FrameBufAddr,
                    (unsigned long)pCtrl->MemMap.u32BitstreamBufAddr,
                    (unsigned long)pCtrl->MemMap.u32MIU1BaseAddr,
                    (unsigned long)pCtrl->MemMap.u32MIU2BaseAddr);
#if HVD_ENABLE_MVC
        if(bMVC)
        {
            HVD_EX_Drv_Ctrl *pHVDCtrl_in_sub = _HVD_EX_GetDrvCtrl(u32Id+0x00011000);
            if (( (pHVDCtrl_in_sub->MemMap.u32BitstreamBufVAddr) <=  (MS_VIRT)pShm)&& ( (MS_VIRT)pShm <= ((pHVDCtrl_in_sub->MemMap.u32BitstreamBufVAddr )+ pHVDCtrl_in_sub->MemMap.u32BitstreamBufSize)))
            {
                HVD_EX_MSG_DBG("[MVC] Bitstream2: 0x%lx.\n", (unsigned long) pCtrl->MemMap.u32BitstreamBufAddr);
            }
        }
#endif /// HVD_ENABLE_MVC

        return E_HVD_RETURN_SUCCESS;
    }
    else
    {
        HVD_EX_MSG_ERR("failed: Shm addr=0x%lx, Code addr=0x%lx, Bits addr=0x%lx, FB addr=0x%lx, Miu1base=0x%lx, Miu2base=0x%lx\n",
                    (unsigned long)MS_VA2PA((MS_VIRT)pShm),
                    (unsigned long)pCtrl->MemMap.u32CodeBufAddr,
                    (unsigned long)pCtrl->MemMap.u32FrameBufAddr,
                    (unsigned long)pCtrl->MemMap.u32BitstreamBufAddr,
                    (unsigned long)pCtrl->MemMap.u32MIU1BaseAddr,
                    (unsigned long)pCtrl->MemMap.u32MIU2BaseAddr);
        return E_HVD_RETURN_INVALID_PARAMETER;
    }
}

#ifdef VDEC3
HVD_Return HAL_HVD_EX_InitShareMem(MS_U32 u32Id, MS_BOOL bFWdecideFB, MS_BOOL bCMAUsed)
#else
HVD_Return HAL_HVD_EX_InitShareMem(MS_U32 u32Id)
#endif
{
    MS_U32 u32Addr = 0;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    MS_U32 u32TmpStartOffset;
    MS_U8  u8TmpMiuSel;


    memset(pShm, 0, sizeof(HVD_ShareMem));

    _phy_to_miu_offset(u8TmpMiuSel, u32Addr, pCtrl->MemMap.u32FrameBufAddr);

    pShm->u32FrameRate = pCtrl->InitParams.u32FrameRate;
    pShm->u32FrameRateBase = pCtrl->InitParams.u32FrameRateBase;
#ifdef VDEC3
    if (bFWdecideFB || bCMAUsed)
    {
        pShm->u32FrameBufAddr = 0;
        pShm->u32FrameBufSize = 0;
    }
    else
#endif
    {
        pShm->u32FrameBufAddr = u32Addr;
        pShm->u32FrameBufSize = pCtrl->MemMap.u32FrameBufSize;
    }

    // FIXME: need to use the avaliable task resource instead of using next task resource
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC_DV)
        pShm->u8ExternalHeapIdx = u8Idx + 1;
    else
        pShm->u8ExternalHeapIdx = 0xFF;
    pShm->DispInfo.u16DispWidth = 1;
    pShm->DispInfo.u16DispHeight = 1;
    pShm->u32CodecType = pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK;
    pShm->u32CPUClock = pHVDHalContext->u32VPUClockType;
    pShm->u32UserCCIdxWrtPtr = 0xFFFFFFFF;
    pShm->DispFrmInfo.u32TimeStamp = 0xFFFFFFFF;
    //Chip info
    pShm->u16ChipID = E_MSTAR_CHIP_MAXIM;
    pShm->u16ChipECONum = pCtrl->InitParams.u16ChipECONum;
    // PreSetControl
    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->bOnePendingBuffer)
    {
        pShm->u32PreSetControl |= PRESET_ONE_PENDING_BUFFER;
    }

    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->bCalFrameRate)
    {
        pShm->u32PreSetControl |= PRESET_CAL_FRAMERATE;
    }

    pShm->bUseTSPInBBUMode = FALSE;


    if ((pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stIapGnShBWMode.bEnable) &&
        ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_AVC))
    {
        pShm->u32PreSetControl |= PRESET_IAP_GN_SHARE_BW_MODE;

        _phy_to_miu_offset(u8TmpMiuSel, u32Addr, pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stIapGnShBWMode.u32IapGnBufAddr);

        pShm->u32IapGnBufAddr = u32Addr;
        pShm->u32IapGnBufSize = pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stIapGnShBWMode.u32IapGnBufSize;

    }

    pShm->u8CodecFeature &= ~E_VDEC_MFCODEC_MASK;
    switch(pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->eMFCodecMode)
    {
        case E_HVD_DEF_MFCODEC_DEFAULT:
            pShm->u8CodecFeature |= E_VDEC_MFCODEC_DEFAULT;
            break;
        case E_HVD_DEF_MFCODEC_FORCE_ENABLE:
            pShm->u8CodecFeature |= E_VDEC_MFCODEC_FORCE_ENABLE;
            break;
        case E_HVD_DEF_MFCODEC_FORCE_DISABLE:
            pShm->u8CodecFeature |= E_VDEC_MFCODEC_FORCE_DISABLE;
            break;
        default:
            pShm->u8CodecFeature |= E_VDEC_MFCODEC_DEFAULT;
    }

    pShm->u8CodecFeature &= ~E_VDEC_DOLBY_VISION_SINGLE_LAYER_MODE;
    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->bDVSingleLayerMode)
        pShm->u8CodecFeature |= E_VDEC_DOLBY_VISION_SINGLE_LAYER_MODE;

    pShm->u8CodecFeature &= ~E_VDEC_FORCE_8BITS_MASK;
    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->bForce8BitMode)
        pShm->u8CodecFeature |= E_VDEC_FORCE_8BITS_MODE;
    pShm->u8CodecFeature &= ~E_VDEC_FORCE_MAIN_PROFILE_MASK;
    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->eVdecFeature & 1)
        pShm->u8CodecFeature |= E_VDEC_FORCE_MAIN_PROFILE;

    if ((pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stPreConnectDispPath.bEnable))
    {
        pShm->u32PreSetControl |= PRESET_CONNECT_DISPLAY_PATH;

        pShm->stDynmcDispPath.u8Connect = pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stPreConnectDispPath.stDynmcDispPath.bConnect;
        pShm->stDynmcDispPath.u8DispPath = (MS_U8)(pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stPreConnectDispPath.stDynmcDispPath.eMvopPath);
        pShm->stDynmcDispPath.u8ConnectStatus = E_DISP_PATH_DYNMC_HANDLING;

        HVD_EX_MSG_DBG("[NDec][0x%x][%d] preset mvop, connect %d, path 0x%x \n", u32Id, u8Idx, pShm->stDynmcDispPath.u8Connect, pShm->stDynmcDispPath.u8DispPath);
    }
    else
    {
        pShm->u32PreSetControl |= PRESET_CONNECT_DISPLAY_PATH;

        MS_U8 u8Connect = FALSE;
        MS_U8 u8Path = E_CTL_DISPLAY_PATH_NONE;
        switch (pCtrl->eStream)
        {
            case E_HVD_ORIGINAL_MAIN_STREAM:
                u8Connect = TRUE;
                u8Path = E_CTL_DISPLAY_PATH_MVOP_0;
                break;
            case E_HVD_ORIGINAL_SUB_STREAM:
                u8Connect = TRUE;
                u8Path = E_CTL_DISPLAY_PATH_MVOP_1;
                break;
            case E_HVD_ORIGINAL_N_STREAM:
            default:
                u8Connect = FALSE;
                u8Path = E_CTL_DISPLAY_PATH_NONE;
                break;
        }

        pShm->stDynmcDispPath.u8Connect = u8Connect;
        pShm->stDynmcDispPath.u8DispPath = u8Path;
        pShm->stDynmcDispPath.u8ConnectStatus = E_DISP_PATH_DYNMC_HANDLING;

        HVD_EX_MSG_DBG("[NDec][0x%x][%d] no preset mvop, connect %d, path 0x%x \n", u32Id, u8Idx, pShm->stDynmcDispPath.u8Connect, u8Path);
    }

    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_INPUT_MASK) == E_HVD_INIT_INPUT_TSP)
    {
        if ((pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stPreConnectInputTsp.bEnable))
        {
            pShm->u32PreSetControl |= PRESET_CONNECT_INPUT_TSP;
            pShm->u8InputTSP = pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->stPreConnectInputTsp.u8InputTsp;

            HVD_EX_MSG_DBG("[NDec][0x%x][%d] preset tsp, input %d \n", u32Id, u8Idx, pShm->u8InputTSP);
        }
        else
        {
            pShm->u32PreSetControl |= PRESET_CONNECT_INPUT_TSP;

            MS_U8 u8Input = E_CTL_INPUT_TSP_NONE;
            switch (pCtrl->eStream)
            {
                case E_HVD_ORIGINAL_MAIN_STREAM:
                    u8Input = E_CTL_INPUT_TSP_0;
                    break;
                case E_HVD_ORIGINAL_SUB_STREAM:
                    u8Input = E_CTL_INPUT_TSP_1;
                    break;
                case E_HVD_ORIGINAL_N_STREAM:
                default:
                    u8Input = E_CTL_INPUT_TSP_NONE;
                    break;
            }

            pShm->u8InputTSP = u8Input;

            HVD_EX_MSG_DBG("[NDec][0x%x][%d] no preset tsp, input %d \n", u32Id, u8Idx, pShm->u8InputTSP);
        }
    }
    else
    {
        HVD_EX_MSG_DBG("[NDec][0x%x][%d] not TSP input, ignore PRESET_CONNECT_INPUT_TSP \n", u32Id, u8Idx);
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

    if (E_HVD_INIT_HW_AVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
       pShm->bHVDIMIEnable = TRUE;  //AVC FW enable IMI only for 4k2k
    }

    #if 0
    if (pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->bEnableDynamicCMA)
    {
        pShm->u8CodecFeature |= E_VDEC_DYNAMIC_CMA_MODE;
    }
    #endif

    if((E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))  ||
       (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)) ||
       (E_HVD_INIT_HW_AVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))  ||
       (E_HVD_INIT_HW_AVS == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))  ||
       (E_HVD_INIT_HW_RM == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))   ||
       (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))  ||
       (E_HVD_INIT_HW_MJPEG== (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)) ||
       (E_HVD_INIT_HW_MVC== (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))   ||
       (E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)))
    {
        pShm->bUseWbMvop = 1;
    }

    if (E_HVD_INIT_HW_HEVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK)
       || E_HVD_INIT_HW_AVC == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        if(!(pHVDHalContext->pHVDPreCtrl_Hal[u8Idx]->eVdecFeature & E_HVD_DEF_FEATURE_DISABLE_TEMPORAL_SCALABILITY))
            pShm->u8CodecFeature |= E_VDEC_TEMPORAL_SCALABILITY_MODE;
    }

#if 1//From T4 and the later chips, QDMA can support the address more than MIU1 base.

    #if (VPU_FORCE_MIU_MODE)
    _phy_to_miu_offset(u8TmpMiuSel, u32TmpStartOffset, pCtrl->MemMap.u32CodeBufAddr);

    pShm->u32FWBaseAddr = u32TmpStartOffset;

    #else
    ///TODO:
    /*
    _phy_to_miu_offset(u8TmpMiuSel, u32TmpStartOffset, pCtrl->MemMap.u32CodeBufAddr);

    if(u8TmpMiuSel == E_CHIP_MIU_0)
    {
        pShm->u32FWBaseAddr = pCtrl->MemMap.u32CodeBufAddr;
    }
    else if(u8TmpMiuSel == E_CHIP_MIU_1)
    {
        pShm->u32FWBaseAddr = u32TmpStartOffset | 0x40000000; ///TODO:
    }
    else if(u8TmpMiuSel == E_CHIP_MIU_2)
    {
        pShm->u32FWBaseAddr = u32TmpStartOffset | 0x80000000; ///TODO:
    }
    */
    #endif
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
#ifdef VDEC3_FB
        pShm->u32RM_VLCTableAddr = 0;
//        HVD_EX_MSG_DBG("===== Set pShm->u32RM_VLCTableAddr = 0 in InitShareMem\n");
#else
        u32Addr = pCtrl->MemMap.u32FrameBufAddr + pHVDHalContext->u32RV_VLCTableAddr;

        _phy_to_miu_offset(u8TmpMiuSel, u32TmpStartOffset, u32Addr);
        u32Addr = u32TmpStartOffset;

        pShm->u32RM_VLCTableAddr = u32Addr;
#endif
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

#if SUPPORT_G2VP9
    // Enable SW detile support for G2 VP9
    if (E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
        pShm->u8FrmPostProcSupport |= E_HVD_POST_PROC_DETILE;
    }
#endif

    HAL_HVD_EX_FlushMemory();

    return E_HVD_RETURN_SUCCESS;
}
#ifdef VDEC3
HVD_Return HAL_HVD_EX_InitRegCPU(MS_U32 u32Id, MS_BOOL bFWdecideFB)
#else
HVD_Return HAL_HVD_EX_InitRegCPU(MS_U32 u32Id)
#endif
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
#ifdef VDEC3
    bInitRet = _HVD_EX_SetRegCPU(u32Id, bFWdecideFB);
#else
    bInitRet = _HVD_EX_SetRegCPU(u32Id);
#endif
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

HVD_Return HAL_HVD_EX_SetData(MS_U32 u32Id, HVD_SetData u32type, MS_VIRT u32Data)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_BOOL bMVC = FALSE;
#if HVD_ENABLE_MVC
    bMVC = HAL_HVD_EX_CheckMVCID(u32Id);
#endif

    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL bDolbyVision = (E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK));

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
        case E_HVD_SDATA_FRAMEBUF2_ADDR:
        {
            pShm->u32FrameBuf2Addr = u32Data;
            break;
        }
        case E_HVD_SDATA_FRAMEBUF2_SIZE:
        {
            pShm->u32FrameBuf2Size = u32Data;
            break;
        }
        case E_HVD_SDATA_MAX_CMA_SIZE:
        {
            pShm->u32MaxCMAFrameBufSize = u32Data;
            break;
        }
        case E_HVD_SDATA_MAX_CMA_SIZE2:
        {
            pShm->u32MaxCMAFrameBuf2Size = u32Data;
            break;
        }
        case E_HVD_SDATA_CMA_USED:
        {
            pShm->bCMA_Use = u32Data;
            break;
        }
        case E_HVD_SDATA_CMA_ALLOC_DONE:
        {
            pShm->bCMA_AllocDone = u32Data;
            break;
        }
        case E_HVD_SDATA_CMA_TWO_MIU:
        {
            pShm->bCMA_TwoMIU = u32Data;
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
        case E_HVD_SDATA_MIU_SEL:
        {
            pShm->u32VDEC_MIU_SEL = u32Data;
            break;
        }
        case E_HVD_SDATA_DV_XC_SHM_SIZE:
        {
            pShm->u32DolbyVisionXCShmSize = u32Data;
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
        case E_HVD_SDATA_DV_INFO:
        {
            pShm->u8DVLevelFromDriverAPI = (MS_U8)(u32Data & 0xff);
            pShm->u8DVProfileFromDriverAPI = (MS_U8)((u32Data >> 8) & 0xff);
            pShm->u8DolbyMetaReorder = (MS_U8)((u32Data >> 16) & 0xff);
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
                if (bMVC || (bDolbyVision && !pShm->bSingleLayer))
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
        case E_HVD_SDATA_HDR_PERFRAME:
        {
            if (u32Data != 0)
            {
                pShm->u8IsDoblyHDR10 = TRUE;
            }
            else
            {
                pShm->u8IsDoblyHDR10 = FALSE;
            }
            break;
        }
        #if (HVD_ENABLE_IQMEM)
        case E_HVD_SDATA_FW_IQMEM_CTRL:
        {
            pShm->u8IQmemCtrl= (MS_U8)u32Data;
            break;

        }
        case E_HVD_SDATA_FW_IQMEM_ENABLE_IF_SUPPORT:
        {
            if (u32Data != 0)
            {
                pShm->bIQmemEnableIfSupport= TRUE;
            }
            else
            {
                pShm->bIQmemEnableIfSupport= FALSE;
            }


            break;

        }
        #endif
        case E_HVD_SDATA_DYNMC_DISP_PATH_STATUS:
        {
            pShm->stDynmcDispPath.u8ConnectStatus = u32Data;
            break;
        }
        case E_HVD_SDATA_VP9HDR10INFO:
        {
            int i,j;
            HVD_Config_VP9HDR10* stVP9HDR10Info = (HVD_Config_VP9HDR10*) u32Data;

            pShm->VP9HDR10Info.u32Version = stVP9HDR10Info->u32Version;
            pShm->VP9HDR10Info.u8MatrixCoefficients = stVP9HDR10Info->u8MatrixCoefficients;
            pShm->VP9HDR10Info.u8BitsPerChannel = stVP9HDR10Info->u8BitsPerChannel;
            pShm->VP9HDR10Info.u8ChromaSubsamplingHorz = stVP9HDR10Info->u8ChromaSubsamplingHorz;
            pShm->VP9HDR10Info.u8ChromaSubsamplingVert = stVP9HDR10Info->u8ChromaSubsamplingVert;
            pShm->VP9HDR10Info.u8CbSubsamplingHorz = stVP9HDR10Info->u8CbSubsamplingHorz;
            pShm->VP9HDR10Info.u8CbSubsamplingVert = stVP9HDR10Info->u8CbSubsamplingVert;
            pShm->VP9HDR10Info.u8ChromaSitingHorz = stVP9HDR10Info->u8ChromaSitingHorz;
            pShm->VP9HDR10Info.u8ChromaSitingVert = stVP9HDR10Info->u8ChromaSitingVert;
            pShm->VP9HDR10Info.u8ColorRange = stVP9HDR10Info->u8ColorRange;
            pShm->VP9HDR10Info.u8TransferCharacteristics = stVP9HDR10Info->u8TransferCharacteristics;
            pShm->VP9HDR10Info.u8ColourPrimaries = stVP9HDR10Info->u8ColourPrimaries;
            pShm->VP9HDR10Info.u16MaxCLL = stVP9HDR10Info->u16MaxCLL;
            pShm->VP9HDR10Info.u16MaxFALL = stVP9HDR10Info->u16MaxFALL;
            pShm->VP9HDR10Info.u32MaxLuminance = stVP9HDR10Info->u32MaxLuminance;
            pShm->VP9HDR10Info.u32MinLuminance = stVP9HDR10Info->u32MinLuminance;

            for(i=0;i<2;i++)
            {
                pShm->VP9HDR10Info.u16WhitePoint[i] = stVP9HDR10Info->u16WhitePoint[i];
            }

            for(i=0;i<3;i++)
            {
                for(j=0;j<2;j++)
                {
                    pShm->VP9HDR10Info.u16Primaries[i][j] = stVP9HDR10Info->u16Primaries[i][j];
                }
            }
            pShm->u8VP9HDR10InfoVaild = TRUE;
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

MS_VIRT HAL_HVD_EX_GetData(MS_U32 u32Id, HVD_GetData eType)
{
    MS_VIRT u32Ret = 0;
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
            u32Ret = (MS_VIRT) (&pShm->DispInfo);
            break;
        }
        case E_HVD_GDATA_MIU_SEL:
            u32Ret = pShm->u32VDEC_MIU_SEL;
            break;
        case E_HVD_GDATA_FRAMEBUF_ADDR:
            u32Ret = pShm->u32FrameBufAddr;
            break;
        case E_HVD_GDATA_FRAMEBUF_SIZE:
            u32Ret = pShm->u32FrameBufSize;
            break;
        case E_HVD_GDATA_FRAMEBUF2_ADDR:
            u32Ret = pShm->u32FrameBuf2Addr;
            break;
        case E_HVD_GDATA_FRAMEBUF2_SIZE:
            u32Ret = pShm->u32FrameBuf2Size;
            break;
        case E_HVD_GDATA_CMA_ALLOC_DONE:
            u32Ret = pShm->bCMA_AllocDone;
            break;
        case E_HVD_GDATA_CMA_USED:
            u32Ret = pShm->bCMA_Use;
            break;
        case E_HVD_GDATA_DYNMC_DISP_PATH_STATUS:
            u32Ret = pShm->stDynmcDispPath.u8ConnectStatus;//pShm->u8SetDynmcDispPathStatus;
            break;
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
            u32Ret = (MS_VIRT)(&(pHVDHalContext->u64pts_real));
            break;
        }
        case E_HVD_GDATA_U64PTS_PRE_PARSE:
        {
            u64pts_low = (MS_U64)(pShm->u32WRPTR_PTS_LOW);
            pHVDHalContext->u64pts_real = (MS_U64)(pShm->u32WRPTR_PTS_HIGH);
            pHVDHalContext->u64pts_real = (pHVDHalContext->u64pts_real<<32)|u64pts_low;
            u32Ret = (MS_VIRT)(&(pHVDHalContext->u64pts_real));
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
            u32Ret = (MS_VIRT) (&pShm->DispFrmInfo);
            break;
        }
        case E_HVD_GDATA_DEC_FRM_INFO:
        {
            u32Ret = (MS_VIRT) (&pShm->DecoFrmInfo);
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
            u32Ret=  (MS_VIRT) (&(pShm->DispFrmInfo_Sub));
            break;
        }
        case E_HVD_GDATA_DEC_FRM_INFO_SUB:
        {
            u32Ret=  (MS_VIRT) (&(pShm->DecoFrmInfo_Sub));
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
            u32Ret = (MS_VIRT) (pShm->u8UserCCIdx);
            break;
        }
        case E_HVD_GDATA_USERDATA_PACKET_TBL_ADDR:
        {
            u32Ret = (MS_VIRT) (pShm->u32UserCCBase);
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
            u32Ret = (MS_VIRT) _HVD_EX_GetNextDispFrame(u32Id);
            break;
        }
        case E_HVD_GDATA_NEXT_DISP_FRM_INFO_EXT:
        {
            u32Ret = (MS_VIRT) _HVD_EX_GetNextDispFrameExt(u32Id);
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
            u32Ret=((MS_VIRT)(pShm->u32Frm_packing_arr_data_addr));
            break;
        case E_HVD_GDATA_DISPLAY_COLOUR_VOLUME_SEI_DATA:
            u32Ret=((MS_U32)(pShm->u32DisplayColourVolume_addr));
            break;
        case E_HVD_GDATA_CONTENT_LIGHT_LEVEL_INFO:
            u32Ret=((MS_U32)(pShm->u32ContentLightLevel_addr));
            break;
        case E_HVD_GDATA_TYPE_FRAME_MBS_ONLY_FLAG:
            u32Ret=((MS_U32)(pShm->u8FrameMbsOnlyFlag));
            break;
        case E_HVD_GDATA_FW_STATUS_FLAG:
            u32Ret=((MS_U32)(pShm->u32FWStatusFlag));
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
        case E_HVD_GDATA_VIDEO_FULL_RANGE_FLAG:
        {
            if(pShm->u32CodecMiscInfo & E_VIDEO_FULL_RANGE)
            {
                u32Ret = 1;
            }
            else
            {
                u32Ret = 0;
            }
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

            if (pShm->bSpsChange &&
                !(pShm->u8FrmPostProcSupport & E_HVD_POST_PROC_DETILE) &&
                IS_TASK_ALIVE(pHVDHalContext->_stHVDStream[_HVD_EX_GetStreamIdx(u32Id)].s32HvdPpTaskId))
            {
                _HVD_EX_PpTask_Delete(&pHVDHalContext->_stHVDStream[_HVD_EX_GetStreamIdx(u32Id)]);
            }

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
            u32Ret = (MS_VIRT) pShm->u32HVD_DUMMY_WRITE_ADDR;
            break;
        case E_HVD_GDATA_FW_DS_BUF_ADDR:
            u32Ret = (MS_VIRT) pShm->u32HVD_DYNAMIC_SCALING_ADDR;
            break;
        case E_HVD_GDATA_FW_DS_BUF_SIZE:
            u32Ret = pShm->u32DSBuffSize;  //3k or 6k
            break;
        case E_HVD_GDATA_FW_DS_VECTOR_DEPTH:
            u32Ret = pShm->u8DSBufferDepth;  //16 or 24 or 32
            break;
        case E_HVD_GDATA_FW_DS_INFO_ADDR:
            u32Ret = (MS_VIRT) pShm->u32HVD_SCALER_INFO_ADDR;
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
        #if (HVD_ENABLE_IQMEM)
        case E_HVD_GDATA_FW_IQMEM_CTRL:
        {

            u32Ret = (MS_U32)pShm->u8IQmemCtrl;

            break;
        }
        case E_HVD_GDATA_FW_IS_IQMEM_SUPPORT:
        {
            if(pShm->bIsIQMEMSupport){
                u32Ret = TRUE;
            }
            else{

                u32Ret = FALSE;
            }

            break;
        }
        #endif
        case E_HVD_GDATA_TYPE_IS_LEAST_DISPQ_SIZE:
            u32Ret = ((MS_U32)(pShm->bIsLeastDispQSize));
            break;
        case E_HVD_GDATA_FIELD_PIC_FLAG:
            u32Ret = ((MS_U32)(pShm->u8FieldPicFlag));
            break;
        case E_HVD_GDATA_TS_SEAMLESS_STATUS:
            u32Ret = pShm->u32SeamlessTSStatus;
            break;
        case E_HVD_GDATA_HVD_HW_MAX_PIXEL:
            u32Ret = (MS_U32)(_HAL_EX_GetHwMaxPixel(u32Id)/1000);
            break;
#ifdef VDEC3
        case E_HVD_GDATA_FW_VBBU_ADDR:
            u32Ret = (MS_VIRT) pShm->u32HVD_VBBU_DRAM_ST_ADDR;
            break;
#endif
        case E_HVD_GDATA_SEQ_CHANGE_INFO:
            u32Ret = (MS_U32)pShm->u32SeqChangeInfo;
            break;
        default:
            break;
    }
    return u32Ret;
}

MS_U32 HAL_HVD_EX_GetDVSupportProfiles(void)
{
#if 0 // wait avc finish DV dual job
    return E_DV_STREAM_PROFILE_ID_DVAV_PER | E_DV_STREAM_PROFILE_ID_DVHE_DER | E_DV_STREAM_PROFILE_ID_DVHE_DTR | E_DV_STREAM_PROFILE_ID_DVHE_STN | E_DV_STREAM_PROFILE_ID_DVHE_DTH;
#else
    return E_DV_STREAM_PROFILE_ID_DVHE_DER | E_DV_STREAM_PROFILE_ID_DVHE_DTR | E_DV_STREAM_PROFILE_ID_DVHE_STN | E_DV_STREAM_PROFILE_ID_DVHE_DTH;
#endif
}

MS_U32 HAL_HVD_EX_GetDVSupportHighestLevel(MS_U32 u32DV_Stream_Profile)
{
    switch (u32DV_Stream_Profile)
    {
#if 0 // wait avc finish DV dual job
        case E_DV_STREAM_PROFILE_ID_DVAV_PER:
            return E_DV_STREAM_LEVEL_ID_UHD24;// level 6
#endif

#if 0 // unsupported profile
        case E_DV_STREAM_PROFILE_ID_DVAV_PEN:
            return E_DV_STREAM_LEVEL_ID_UNSUPPORTED;
#endif

        case E_DV_STREAM_PROFILE_ID_DVHE_DER:
            return E_DV_STREAM_LEVEL_ID_UHD60;// level 9

#if 0 // unsupported profile
        case E_DV_STREAM_PROFILE_ID_DVHE_DEN:
            return E_DV_STREAM_LEVEL_ID_UNSUPPORTED;
#endif

        case E_DV_STREAM_PROFILE_ID_DVHE_DTR:
            return E_DV_STREAM_LEVEL_ID_UHD60;// level 9

        case E_DV_STREAM_PROFILE_ID_DVHE_STN:
            return E_DV_STREAM_LEVEL_ID_UHD60;// level 9

        case E_DV_STREAM_PROFILE_ID_DVHE_DTH:
            return E_DV_STREAM_LEVEL_ID_UHD60;// level 9

        case E_DV_STREAM_PROFILE_ID_UNSUPPORTED:
        default:
            return E_DV_STREAM_LEVEL_ID_UNSUPPORTED;
    }
}

HVD_Return HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg)
{
    HVD_Return eRet = E_HVD_RETURN_SUCCESS;
    MS_U32 u32Cmd = (MS_U32) eUsrCmd;
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    if (pHVDHalContext->_stHVDStream[u8Idx].u32CodecType == E_HAL_HVD_HEVC_DV)
    {
        // skip mutex
    }
    else
    {
       _HAL_HVD_Entry();
    }
    // check if old SVD cmds
    if (u32Cmd < E_HVD_CMD_SVD_BASE)
    {
        HVD_EX_MSG_ERR("Old SVD FW cmd(%x %x) used in HVD.\n", u32Cmd, u32CmdArg);

        if (pHVDHalContext->_stHVDStream[u8Idx].u32CodecType == E_HAL_HVD_HEVC_DV)
        {
            return E_HVD_RETURN_INVALID_PARAMETER;
        }
        else
        {
            _HAL_HVD_Return(E_HVD_RETURN_INVALID_PARAMETER);
        }
    }

    if (u32Cmd == E_HVD_CMD_ENABLE_DISP_OUTSIDE)
    {
        pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide = (MS_BOOL)u32CmdArg;
    }

    if (pHVDHalContext->_stHVDStream[u8Idx].bDispOutSide)
    {
        if (u32Cmd == E_HVD_CMD_FLUSH)
            pHVDHalContext->_stHVDStream[u8Idx].u32DispQIndex = 0;
    }

    if (u32Cmd == E_HVD_CMD_FLUSH &&
        IS_TASK_ALIVE(pHVDHalContext->_stHVDStream[u8Idx].s32HvdPpTaskId) &&
        pHVDHalContext->_stHVDStream[u8Idx].ePpTaskState == E_HAL_HVD_STATE_RUNNING)
    {
        pHVDHalContext->_stHVDStream[u8Idx].ePpTaskState = E_HAL_HVD_STATE_PAUSING;
        while (pHVDHalContext->_stHVDStream[u8Idx].ePpTaskState != E_HAL_HVD_STATE_PAUSE_DONE)
        {
            if (pHVDHalContext->_stHVDStream[u8Idx].u32CodecType == E_HAL_HVD_HEVC_DV)
            {
                HVD_Delay_ms(1);
            }
            else
            {
                _HAL_HVD_Release();
                HVD_Delay_ms(1);
                _HAL_HVD_Entry();
            }
        }
    }

    HVD_EX_MSG_DBG("cmd=0x%x, arg=0x%x\n", u32Cmd, u32CmdArg);

    eRet = _HVD_EX_SendCmd(u32Id, u32Cmd, u32CmdArg);
    if (pHVDHalContext->_stHVDStream[u8Idx].u32CodecType == E_HAL_HVD_HEVC_DV)
    {
        return eRet;
    }
    else
    {
        _HAL_HVD_Return(eRet);
    }
}

HVD_Return HAL_HVD_EX_DeInit(MS_U32 u32Id)
{
    HVD_Return eRet         = E_HVD_RETURN_FAIL;
    MS_U8 u8Idx             = _HVD_EX_GetStreamIdx(u32Id);
    HVD_EX_Drv_Ctrl *pCtrl  = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U32 u32Timeout       = HVD_GetSysTime_ms() + 3000;
    MS_U8 u8MiuSel;
    MS_U32 u32StartOffset;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

#if HVD_ENABLE_TIME_MEASURE
    MS_U32 ExitTimeCnt = 0;
    ExitTimeCnt = HVD_GetSysTime_ms();
#endif

    pCtrl->MemMap.u32CodeBufVAddr = MS_PA2KSEG1((MS_PHY)pCtrl->MemMap.u32CodeBufAddr);

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
            HVD_EX_MSG_ERR("FW stop timeout, pc = 0x%x\n", HAL_VPU_EX_GetProgCnt());

            //return E_HVD_RETURN_TIMEOUT;
            eRet =  E_HVD_RETURN_TIMEOUT;
            break;
        }
    }

    if (pShm->u32VdecPlusDecCnt+pShm->u32VdecPlusDropCnt)
    {
        HVD_EX_MSG_INF("VDEC PLUS: DropRatio %d, Drop:0.%d (%d), Dec:0.%d (%d), Disp:0.%d\n",
            pShm->u8VdecPlusDropRatio,
            100*pShm->u32VdecPlusDropCnt/(pShm->u32VdecPlusDecCnt+pShm->u32VdecPlusDropCnt),
            pShm->u32VdecPlusDropCnt,
            100*pShm->u32VdecPlusDecCnt/(pShm->u32VdecPlusDecCnt+pShm->u32VdecPlusDropCnt),
            pShm->u32VdecPlusDecCnt,
            100*pShm->u32VdecPlusDispPicCnt/(pShm->u32VdecPlusDecCnt+pShm->u32VdecPlusDropCnt));
    }
    else
    {
        HVD_EX_MSG_INF("VDEC PLUS DISABLE: DropRatio %d, Drop: %d, Dec: %d, Disp: %d\n",
            pShm->u8VdecPlusDropRatio,
            pShm->u32VdecPlusDropCnt,
            pShm->u32VdecPlusDecCnt,
            pShm->u32VdecPlusDispPicCnt);
    }

    VPU_EX_FWCodeCfg       fwCfg;
    VPU_EX_TaskInfo        taskInfo;
    VPU_EX_NDecInitPara    nDecInitPara;

    nDecInitPara.pFWCodeCfg = &fwCfg;
    nDecInitPara.pTaskInfo = &taskInfo;

    fwCfg.u32DstAddr = pCtrl->MemMap.u32CodeBufVAddr;
    fwCfg.u8SrcType  = E_HVD_FW_INPUT_SOURCE_NONE;

    HAL_HVD_EX_GetTaskInfo(u32Id,&taskInfo);//power control
#if 0
    taskInfo.u32Id = u32Id;
    taskInfo.eDecType = E_VPU_EX_DECODER_HVD;
    taskInfo.eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);
#endif

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
#ifndef VDEC3
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
#endif
    _stHVDPreSet[u8Idx].bColocateBBUMode = FALSE;

    if (IS_TASK_ALIVE(pHVDHalContext->_stHVDStream[u8Idx].s32HvdPpTaskId))
    {
        _HVD_EX_PpTask_Delete(&pHVDHalContext->_stHVDStream[u8Idx]);
    }

    if(pHVDHalContext->pHVDPreCtrl_Hal[_HVD_EX_GetStreamIdx(u32Id)]->stIapGnShBWMode.bEnable)
    {

        _phy_to_miu_offset(u8MiuSel, u32StartOffset, pCtrl->MemMap.u32FrameBufAddr);

            _HAL_HVD_Entry();
        HAL_HVD_MIF1_MiuClientSel(u8MiuSel);
            _HAL_HVD_Release();

    }

    pHVDHalContext->_stHVDStream[u8Idx].u32RegBase = 0;
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
    MS_BOOL bCurrentStatus = HAL_HVD_EX_IsEnableISR(u32Id);
    if(bCurrentStatus == bEnable)
        return;

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

void HAL_HVD_EX_SetClearISR(HWDEC_ISR_TYPE eISRType)
{
    MS_U32 u32RB = 0;
    switch(eISRType)
    {
        case E_HWDEC_ISR_HVD:
            u32RB = REG_HVD_BASE;
            break;
        #if SUPPORT_EVD
        case E_HWDEC_ISR_EVD:
            u32RB = REG_EVD_BASE;
            break;
        #endif
        #if SUPPORT_G2VP9
        case E_HWDEC_ISR_G2VP9:
            break;
        #endif
        default:
            break;
    }
    if(u32RB)
    {
        _HVD_WriteWordMask(HVD_REG_RISC_MBOX_CLR(u32RB), HVD_REG_RISC_ISR_CLR, HVD_REG_RISC_ISR_CLR);
    }
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

        HVD_EX_MSG_DBG("PTS table: WptrAddr:%x RptrAddr:%x ByteCnt:%x PreWptr:%lx\n",
            pShm->u32PTStableWptrAddr, pShm->u32PTStableRptrAddr, pHVDHalContext->_stHVDStream[u8Idx].u32PTSByteCnt, (unsigned long)pHVDHalContext->_stHVDStream[u8Idx].u32PTSPreWptr);
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

    if (IS_TASK_ALIVE(pHVDHalContext->_stHVDStream[u8Idx].s32HvdPpTaskId))
        pHVDHalContext->_stHVDStream[u8Idx].ePpTaskState = E_HAL_HVD_STATE_RUNNING;

    return TRUE;
}

void HAL_HVD_EX_UartSwitch2FW(MS_BOOL bEnable)
{
    if (bEnable)
    {
        if (HAL_VPU_EX_IsEVDR2())
            _HVD_WriteWordMask(REG_TOP_UART_SEL0, REG_TOP_UART_SEL_MHEG5, REG_TOP_UART_SEL_0_MASK);
        else
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
    return 216;                 //140;
    //if(  )
}

void HAL_HVD_EX_UpdateESWptr_Fire(MS_U32 u32Id)
{
    //MS_BOOL bBitMIU1 = FALSE;
    //MS_BOOL bCodeMIU1 = FALSE;
    MS_U8 u8BitMiuSel = 0;
    MS_U8 u8CodeMiuSel = 0;
    MS_U32 u32BitStartOffset;
    MS_U32 u32CodeStartOffset;
    //MS_U8 u8MiuSel;
    //MS_U32 u32StartOffset;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);
    MS_VIRT u32BBU_DRAM_ST_ADDR = (MS_VIRT) pShm->u32HVD_BBU_DRAM_ST_ADDR;

#if HVD_ENABLE_MVC
    if(HAL_HVD_EX_CheckMVCID(u32Id))
    {
        // if MVC_BBU_ADDR and HVD_BBU_ADDR are different, we need to add MVC_BBU_DRAM_ST_ADDR and MVC_BBU2_DRAM_ST_ADDR in share memory
        u32BBU_DRAM_ST_ADDR = (MS_VIRT) pShm->u32HVD_BBU_DRAM_ST_ADDR;  //pShm->u32MVC_BBU_DRAM_ST_ADDR;
        if(E_VDEC_EX_SUB_VIEW == HAL_HVD_EX_GetView(u32Id))
        {
            u32BBU_DRAM_ST_ADDR = (MS_VIRT) pShm->u32HVD_BBU2_DRAM_ST_ADDR;  //pShm->u32MVC_BBU2_DRAM_ST_ADDR;
        }
    }
#endif /// HVD_ENABLE_MVC

    _phy_to_miu_offset(u8BitMiuSel, u32BitStartOffset, pCtrl->MemMap.u32BitstreamBufAddr);
    _phy_to_miu_offset(u8CodeMiuSel, u32CodeStartOffset, pCtrl->MemMap.u32CodeBufAddr);




    if (u8BitMiuSel != u8CodeMiuSel)
    {
#if HVD_ENABLE_BDMA_2_BITSTREAMBUF
        BDMA_Result bdmaRlt;
        MS_VIRT u32DstAdd = 0, u32SrcAdd = 0, u32tabsize = 0;

        u32DstAdd = pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->u32BBUTblInBitstreamBufAddr;
        u32SrcAdd = pCtrl->MemMap.u32CodeBufAddr + u32BBU_DRAM_ST_ADDR;
        u32tabsize = pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum << 3;

        bdmaRlt = HVD_dmacpy(u32DstAdd, u32SrcAdd, u32tabsize);

        if (E_BDMA_OK != bdmaRlt)
        {
            HVD_EX_MSG_ERR("MDrv_BDMA_MemCopy fail ret=%x!\n", bdmaRlt);
        }
#else
        MS_VIRT u32DstAdd = 0, u32SrcAdd = 0, u32tabsize = 0;

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
        printf("\tSystime=%u, FWVersionID=0x%x, FwState=0x%x, ErrCode=0x%x, ProgCnt=0x%x\n",
            HVD_GetSysTime_ms(), pShm->u32FWVersionID, pShm->u32FwState, (MS_U32) pShm->u16ErrCode, HAL_VPU_EX_GetProgCnt());

        printf("\tTime: DispSTC=%u, DispT=%u, DecT=%u, CurrentPts=%u, Last Cmd=0x%x, Arg=0x%x, Rdy1=0x%x, Rdy2=0x%x\n",
                pShm->u32DispSTC, pShm->DispFrmInfo.u32TimeStamp,
                pShm->DecoFrmInfo.u32TimeStamp, pShm->u32CurrentPts, tmp1, tmp2,
                (MS_U32) _HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_MBOX), (MS_U32) _HVD_EX_MBoxReady(u32Id, HAL_HVD_CMD_ARG_MBOX));

        printf("\tFlag: InitDone=%d, SpsChange=%d, IsIFrmFound=%d, 1stFrmRdy=%d, SyncStart=%d, SyncReach=%d\n",
                    pShm->bInitDone, pShm->bSpsChange, pShm->bIsIFrmFound,
                pShm->bIs1stFrameRdy, pShm->bIsSyncStart, pShm->bIsSyncReach);

        printf("\tQueue: BBUQNumb=%u, DecQNumb=%d, DispQNumb=%d, ESR=%u, ESRfromFW=%u, ESW=%u, ESLevel=%u\n",
                _HVD_EX_GetBBUQNumb(u32Id), pShm->u16DecQNumb, pShm->u16DispQNumb,
                _HVD_EX_GetESReadPtr(u32Id, TRUE), pShm->u32ESReadPtr, _HVD_EX_GetESWritePtr(u32Id),
                _HVD_EX_GetESLevel(u32Id));

        printf("\tCounter: DecodeCnt=%u, DispCnt=%u, DataErrCnt=%u, DecErrCnt=%u, SkipCnt=%u, DropCnt=%u, idle=%u, MainLoopCnt=%u, VsyncCnt=%u\n",
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

    u32Addr = (MS_U8 *)(MsOS_PA2KSEG1(pDrvCtrl->MemMap.u32CodeBufAddr + (MS_PHY)pShm->u32HVD_BBU_DRAM_ST_ADDR + (u32Idx << 3)));

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
            HVD_EX_MSG_DBG("HVD BBU Entry: Idx:%u Offset:%x Size:%x\n", u32CurIdx, u32NalOffset, u32NalSize);
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

            printf(" %08x", value);

        if (((i % 8) + 1) == 8)
        {
                printf(" |%u\n", i + 1);
        }
    }

        printf("\nHVD Dump HW status End: total number:%u\n", i);
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

void HAL_HVD_EX_PowerSaving(MS_U32 u32Id)    //// turn on power saving mode for STB chips, ex. clippers, kano
{
    HAL_HVD_EX_SetCmd(u32Id, E_HVD_CMD_POWER_SAVING, TRUE);
    return;
}

MS_BOOL HAL_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id, MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate)
{
    MS_U64 _hw_max_pixel = 0;
    _hw_max_pixel = _HAL_EX_GetHwMaxPixel(u32Id);

    HVD_EX_MSG_DBG("%s w:%d, h:%d, fr:%d, MAX:%ld\n", __FUNCTION__,
                    u16HSize, u16VSize, u32FrmRate, (unsigned long)_hw_max_pixel);
    return (((MS_U64)u16HSize*(MS_U64)u16VSize*(MS_U64)u32FrmRate) <= _hw_max_pixel);
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

    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL bDolbyVision = (E_HVD_INIT_HW_HEVC_DV == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK));

    if(bMVC || bDolbyVision)
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
    if ((u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC ||
        (u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_HEVC_DV)
        pHVDHalContext->_stHVDStream[u8Idx].u32RegBase = REG_EVD_BASE;
    else if ((u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_VP9)
        #if SUPPORT_G2VP9 && defined(VDEC3)
        pHVDHalContext->_stHVDStream[u8Idx].u32RegBase = REG_G2VP9_BASE;
        #else // Not using G2 VP9 implies using Mstar EVD VP9
        pHVDHalContext->_stHVDStream[u8Idx].u32RegBase = REG_EVD_BASE;
        #endif
    else
        pHVDHalContext->_stHVDStream[u8Idx].u32RegBase = REG_HVD_BASE;
}

#if SUPPORT_EVD
void HAL_EVD_EX_PowerCtrl(MS_U32 u32Id, MS_BOOL bEnable)
{
#ifdef CONFIG_MSTAR_CLKM
    HAL_VPU_EX_SetClkManagement(E_VPU_EX_CLKPORT_EVD, bEnable);
    HAL_VPU_EX_SetClkManagement(E_VPU_EX_CLKPORT_EVD_PPU, bEnable);
#else
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, ~TOP_CKG_EVD_PPU_DIS, TOP_CKG_EVD_PPU_DIS);
        _HVD_WriteWordMask(REG_TOP_CKG_EVD, ~TOP_CKG_EVD_DIS, TOP_CKG_EVD_DIS);
        _HVD_WriteWordMask(REG_EVDPLL_PD, ~REG_EVDPLL_PD_DIS, REG_EVDPLL_PD_DIS);
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_DIS, TOP_CKG_EVD_PPU_DIS);
        _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_DIS, TOP_CKG_EVD_DIS);
        _HVD_WriteWordMask(REG_EVDPLL_PD, REG_EVDPLL_PD_DIS, REG_EVDPLL_PD_DIS);
    }

    switch (pHVDHalContext->u32EVDClockType)
    {
        case 576:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_PLL_BUF, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_480MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 532:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_MIU128PLL, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_MIU128PLL, TOP_CKG_EVD_MASK);
            break;
        }
        case 456:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_MIU256PLL, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_PLL_BUF, TOP_CKG_EVD_MASK);
            _HVD_WriteWordMask(REG_EVDPLL_LOOP_DIV_SECOND, REG_EVDPLL_LOOP_DIV_SECOND_456MHZ, REG_EVDPLL_LOOP_DIV_SECOND_MASK);
            break;
        }
        case 466:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_MIU256PLL, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_MIU256PLL, TOP_CKG_EVD_MASK);
            break;
        }
        case 480:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_480MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_480MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 384:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_384MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_384MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 320:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_320MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_320MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 240:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_240MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_240MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        case 192:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_192MHZ, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_192MHZ, TOP_CKG_EVD_MASK);
            break;
        }
        default:
        {
            _HVD_WriteWordMask(REG_TOP_CKG_EVD_PPU, TOP_CKG_EVD_PPU_PLL_BUF, TOP_CKG_EVD_PPU_MASK);
            _HVD_WriteWordMask(REG_TOP_CKG_EVD, TOP_CKG_EVD_PLL_BUF, TOP_CKG_EVD_MASK);
            break;
        }
    }
#endif
#ifdef CONFIG_MSTAR_SRAMPD
    if (bEnable)
    {
        _HVD_WriteByteMask(REG_HICODEC_SRAM_SD_EN, HICODEC_SRAM_HICODEC0, HICODEC_SRAM_HICODEC0);
        HVD_Delay_ms(1);
    }
    else
    {
        _HVD_WriteByteMask(REG_HICODEC_SRAM_SD_EN, ~HICODEC_SRAM_HICODEC0, HICODEC_SRAM_HICODEC0);
        HVD_Delay_ms(1);
    }
#endif
    return;
}

void HAL_EVD_EX_ClearTSPInput(MS_U32 u32Id)
{
    #ifndef VDEC3
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    #endif
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;

    #ifdef VDEC3
    if (0 == pCtrl->u32BBUId)
    #else
    if (0 == u8TaskId)
    #endif
    {
        _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) & ~EVD_REG_RESET_HK_TSP2EVD_EN)); //0: tsp2hvd, coz EVD & HVD use the same MVD parser for main-DTV mode
        // disable TSP mode in EVD since EVD maybe effected by MVD parser's write pointer used by previous decoder
        _HVD_Write2Byte(HVD_REG_MIF_BBU(u32RB), _HVD_Read2Byte(HVD_REG_MIF_BBU(u32RB)) & (~HVD_REG_BBU_TSP_INPUT));
        HVD_EX_MSG_INF("id %d disable TSP mode, val 0x%x\n", pCtrl->u32BBUId, _HVD_Read2Byte(HVD_REG_MIF_BBU(u32RB)));
    }
    else
    {
        _HVD_Write2Byte(EVD_REG_RESET, (_HVD_Read2Byte(EVD_REG_RESET) & ~EVD_REG_RESET_USE_HVD_MIU_EN)); //0: tsp2hvd, coz EVD & HVD use the same MVD parser for sub-DTV mode
        // disable TSP mode in EVD since EVD maybe effected by MVD parser's write pointer used by previous decoder
        _HVD_Write2Byte(HVD_REG_MIF_BBU_BS2(u32RB), _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2(u32RB)) & (~HVD_REG_BBU_TSP_INPUT_BS2));
        HVD_EX_MSG_INF("id %d disable TSP mode, val 0x%x\n", pCtrl->u32BBUId, _HVD_Read2Byte(HVD_REG_MIF_BBU_BS2(u32RB)));
    }

    return;
}

MS_BOOL HAL_EVD_EX_DeinitHW(MS_U32 u32Id)
{
    MS_U16 u16Timeout = 1000;
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_BOOL isVP8Used = FALSE;
    MS_BOOL isAECUsed = FALSE;
    MS_BOOL isAVCUsed = FALSE;
    HAL_HVD_EX_VP8AECInUsed(u32Id, &isVP8Used, &isAECUsed, &isAVCUsed);

    if(TRUE == HAL_VPU_EX_EVDInUsed())
    {
         if(!isAECUsed && E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
             HAL_AEC_PowerCtrl(FALSE);
         }
         return FALSE;
    }
    else if(!isAVCUsed)//no AVC/EVD use , close EVD power
    {
         _HVD_EX_SetMIUProtectMask(TRUE);

         _HVD_WriteWordMask(EVD_REG_RESET, EVD_REG_RESET_SWRST, EVD_REG_RESET_SWRST);

         while (u16Timeout)
         {
             if ((_HVD_Read2Byte(EVD_REG_RESET) & (EVD_REG_RESET_SWRST_FIN)) == (EVD_REG_RESET_SWRST_FIN))
             {
                 break;
             }
             u16Timeout--;
         }

         HAL_EVD_EX_PowerCtrl(u32Id, FALSE);

         if(!isAECUsed && E_HVD_INIT_HW_VP9 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
         {
            HAL_AEC_PowerCtrl(FALSE);
         }

         _HVD_EX_SetMIUProtectMask(FALSE);

         return TRUE;
    }

    return FALSE;
}
#endif

#if SUPPORT_G2VP9 && defined(VDEC3)
static void HAL_VP9_EX_PowerCtrl(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HVD_WriteWordMask(REG_TOP_VP9, ~TOP_CKG_VP9_DIS, TOP_CKG_VP9_DIS);
    }
    else
    {
        _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_DIS, TOP_CKG_VP9_DIS);
    }

    switch (pHVDHalContext->u32VP9ClockType)
    {
        case 432:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_432MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 384:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_384MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 345:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_345MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 320:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_320MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 288:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_288MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 240:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_240MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 216:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_216MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        case 172:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_172MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
        default:
        {
            _HVD_WriteWordMask(REG_TOP_VP9, TOP_CKG_VP9_432MHZ, TOP_CKG_VP9_CLK_MASK);
            break;
        }
    }

    return;
}

MS_BOOL HAL_VP9_EX_DeinitHW(void)
{
    MS_U16 u16Timeout = 1000;

    _HVD_WriteWordMask(VP9_REG_RESET, VP9_REG_RESET_SWRST, VP9_REG_RESET_SWRST);

    while (u16Timeout)
    {
        if ((_HVD_Read2Byte(VP9_REG_RESET) & (VP9_REG_RESET_SWRST_FIN)) == (VP9_REG_RESET_SWRST_FIN))
        {
            break;
        }
        u16Timeout--;
    }

    HAL_VP9_EX_PowerCtrl(FALSE);

    return TRUE;
}
#endif

MS_BOOL HAL_HVD_EX_GetSupport2ndMVOPInterface(void)
{
    return TRUE;
}

void HAL_HVD_EX_SetNalTblAddr(MS_U32 u32Id)
{
    MS_VIRT u32StAddr   = 0;
    MS_U8  u8BitMiuSel  = 0;
    MS_U8  u8CodeMiuSel = 0;
    MS_U8  u8TmpMiuSel  = 0;
    MS_U32 u32BitStartOffset;
    MS_U32 u32CodeStartOffset;

#ifndef VDEC3
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
#endif
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);
    MS_U8 u8Idx = _HVD_EX_GetStreamIdx(u32Id);
    MS_U32 u32RB = pHVDHalContext->_stHVDStream[u8Idx].u32RegBase;
    HVD_ShareMem *pShm = (HVD_ShareMem *) HAL_HVD_EX_GetShmAddr(u32Id);

    _HAL_HVD_Entry();

    if (pCtrl == NULL)
    {
        _HAL_HVD_Return();
    }

    MS_BOOL bNalTblAlreadySet = FALSE;
    VPU_EX_TaskInfo taskInfo;
    memset(&taskInfo, 0, sizeof(VPU_EX_TaskInfo));
    HAL_HVD_EX_GetTaskInfo(u32Id, &taskInfo);

    bNalTblAlreadySet = HAL_VPU_EX_CheckBBUSetting(u32Id, pCtrl->u32BBUId, taskInfo.eDecType, VPU_BBU_NAL_TBL);



    _phy_to_miu_offset(u8BitMiuSel, u32BitStartOffset, pCtrl->MemMap.u32BitstreamBufAddr);
    _phy_to_miu_offset(u8CodeMiuSel, u32CodeStartOffset, pCtrl->MemMap.u32CodeBufAddr);

    if (u8BitMiuSel != u8CodeMiuSel)
    {
        _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, (pCtrl->MemMap.u32BitstreamBufAddr + pCtrl->u32BBUTblInBitstreamBufAddr));
    }
    else
    {
        _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, (pCtrl->MemMap.u32CodeBufAddr + pShm->u32HVD_BBU_DRAM_ST_ADDR));
    }

    if (E_HVD_INIT_HW_VP8 == (pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK))
    {
#ifdef VDEC3
        if (!_HAL_EX_BBU_VP8_InUsed())
#endif
        {
            _HVD_Write2Byte(HVD_REG_HK_VP8, HVD_REG_HK_PLAYER_FM);

            _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_L_BS3, (MS_U16)(u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_H_BS3, (MS_U16)(u32StAddr >> 19));
            _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS3, (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));

            u32StAddr += VP8_BBU_TBL_SIZE;

            _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_L_BS4, (MS_U16)(u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_NAL_TAB_ST_H_BS4, (MS_U16)(u32StAddr >> 19));
            _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS4, (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
        }

        _HAL_HVD_Return();
    }

    HVD_EX_MSG_DBG("NAL start addr=%lx\n", (unsigned long)u32StAddr);

#ifdef VDEC3
    if (!bNalTblAlreadySet)
    {
        if (pCtrl->u32BBUId == 0)
        {
            _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L(u32RB), (MS_U16) (u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H(u32RB), (MS_U16) (u32StAddr >> 19));
            // -1 is for NAL_TAB_LEN counts from zero.
            _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN(u32RB), (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
        }
        else
        {
            _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L_BS2(u32RB), (MS_U16) (u32StAddr >> 3));
            _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H_BS2(u32RB), (MS_U16) (u32StAddr >> 19));
            // -1 is for NAL_TAB_LEN counts from zero.
            _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS2(u32RB), (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
        }
    }
#else
    if (0 == u8TaskId)
    {
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L(u32RB), (MS_U16) (u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H(u32RB), (MS_U16) (u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN(u32RB), (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
    }
    else
    {
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L_BS2(u32RB), (MS_U16) (u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H_BS2(u32RB), (MS_U16) (u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS2(u32RB), (MS_U16) (pHVDHalContext->_stHVDStream[u8Idx].u32BBUEntryNum - 1));
    }
#endif


#if (HVD_ENABLE_MVC)
    if ((pCtrl->InitParams.u32ModeFlag & E_HVD_INIT_HW_MASK) == E_HVD_INIT_HW_MVC)
    {
        /// Used sub stream to record sub view data.
        HVD_EX_Drv_Ctrl *pDrvCtrl_Sub = _HVD_EX_GetDrvCtrl((u32Id+0x00011000));
        //printf("**************** Buffer setting for MVC dual-BBU *************\n");

        if (u8BitMiuSel != u8CodeMiuSel)
        {
            _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, (pDrvCtrl_Sub->MemMap.u32BitstreamBufAddr + pDrvCtrl_Sub->u32BBUTblInBitstreamBufAddr));
        }
        else
        {
            _phy_to_miu_offset(u8TmpMiuSel, u32StAddr, (pDrvCtrl_Sub->MemMap.u32CodeBufAddr + pShm->u32HVD_BBU2_DRAM_ST_ADDR));
        }

        HVD_EX_MSG_DBG("[MVC] _HAL_HVD_SetBuffer2Addr: nal StAddr:%lx \n", (unsigned long) u32StAddr);
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_L_BS2(u32RB), (MS_U16)(u32StAddr >> 3));
        _HVD_Write2Byte(HVD_REG_NAL_TBL_ST_ADDR_H_BS2(u32RB), (MS_U16)(u32StAddr >> 19));
        // -1 is for NAL_TAB_LEN counts from zero.
        _HVD_Write2Byte(HVD_REG_NAL_TAB_LEN_BS2(u32RB), (MS_U16)(pHVDHalContext->_stHVDStream[u8Idx+1].u32BBUEntryNum - 1));
    }
#endif

    if (!bNalTblAlreadySet)
    {
        HAL_VPU_EX_SetBBUSetting(u32Id, pCtrl->u32BBUId, taskInfo.eDecType, VPU_BBU_NAL_TBL);
    }

    _HAL_HVD_Return();
}

MS_BOOL HAL_HVD_EX_Is_RM_Supported(MS_U32 u32Id)
{
    HVD_EX_Drv_Ctrl *pCtrl = _HVD_EX_GetDrvCtrl(u32Id);

    if(pCtrl->InitParams.u16ChipECONum == 0)
        return FALSE;
    else
        return TRUE;
}

void HAL_HVD_EX_BBU_Proc(MS_U32 u32streamIdx)
{

}

void HAL_HVD_EX_BBU_StopProc(MS_U32 u32streamIdx)
{

}
#endif
