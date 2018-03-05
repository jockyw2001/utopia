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
#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "MsTypes.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvMVOP.h"
#include "drvMIU.h"

// Internal Definition
#include "regMVOP.h"
#include "halMVOP.h"

#ifndef ANDROID
#define MVOP_PRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define MVOP_PRINTF ALOGD
#endif

// Common
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#define do_mod(x,y) ((x)%=(y))
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MPLL_CLOCK           (432000000ULL)

#define BIT0    BIT(0)
#define BIT1    BIT(1)
#define BIT2    BIT(2)
#define BIT3    BIT(3)
#define BIT4    BIT(4)
#define BIT5    BIT(5)
#define BIT6    BIT(6)
#define BIT7    BIT(7)
#define BIT15   BIT(15)

#define ALIGN_UPTO_16(x)  ((((x) + 15) >> 4) << 4)
#define ALIGN_UPTO_4(x)   ((((x) + 3) >> 2) << 2)
#define ALIGN_UPTO_2(x)   ((((x) + 1) >> 1) << 1)

#define MVOP_VBlank               45
#define MVOP_HBlank_SD            200
#define MVOP_HBlank_HD            300

#if 0
static MS_U32 u32RiuBaseAdd=0;
static MS_BOOL bMirrorModeVer = 0;
static MS_BOOL bMirrorModeHor = 0;
static MS_BOOL bMirrorMode=0;
static MS_BOOL bSubMirrorModeVer = 0;
static MS_BOOL bSubMirrorModeHor = 0;
static MS_BOOL bSubMirrorMode=0;
static MS_BOOL bEnableFreerunMode = FALSE;
static MS_BOOL bSubEnableFreerunMode = FALSE;
static MS_BOOL b3DLRMode=0;    /// 3D L/R dual buffer mode
static MS_BOOL bSub3DLRMode=0;
static MS_BOOL b3DLRAltOutput = FALSE;    /// 3D L/R line alternative output
static MS_BOOL bNewVSyncMode = FALSE;
static MVOP_RptFldMode eRepeatField = E_MVOP_RPTFLD_NONE;      /// mvop output repeating fields for single field input.
static MVOP_RptFldMode eSubRepeatField = E_MVOP_RPTFLD_NONE;   /// mvop output repeating fields for single field input.
static MVOP_RgbFormat eMainRgbFmt = E_MVOP_RGB_NONE;
static MVOP_RgbFormat eSubRgbFmt = E_MVOP_RGB_NONE;
#endif

typedef struct
{

    MS_BOOL bMirrorModeVer;
    MS_BOOL bMirrorModeHor;
    MS_BOOL bMirrorMode;
    MS_BOOL bEnableFreerunMode;
    MS_BOOL b3DLRMode;    /// 3D L/R dual buffer mode
    MS_BOOL b3DLRAltOutput;    /// 3D L/R line alternative output
    MS_BOOL b3DLRAltSBSOutput; /// 3D L/R side by side output
    MS_BOOL bNewVSyncMode;
    MVOP_RptFldMode eRepeatField;      /// mvop output repeating fields for single field input.
    MVOP_RgbFormat eMainRgbFmt;
    MS_BOOL bIsInit;
    MS_BOOL bRptPreVsync;
    MS_BOOL bIs422;
    MS_BOOL bIsH265;
    MS_BOOL bIsHS;
    MS_U16  u16CropXStart;
    MS_U16  u16CropYStart;
    MS_U16  u16CropXSize;
    MS_U16  u16CropYSize;
    MS_BOOL bIsSetCrop;
    MS_BOOL bIsXcTrig;
    MS_U32 u32MVOPFixClk;
    MS_BOOL bIsEnable;
    MS_BOOL bIs2p;
    MVOP_OutputImodeType eInterlaceType;
#if MVOP_SUPPORT_SUB
    MS_BOOL bSubMirrorModeVer;
    MS_BOOL bSubMirrorModeHor;
    MS_BOOL bSubMirrorMode;
    MS_BOOL bSubEnableFreerunMode;
    MS_BOOL bSub3DLRMode;
    MS_BOOL bSubNewVSyncMode;
    MVOP_RptFldMode eSubRepeatField;   /// mvop output repeating fields for single field input.
    MVOP_RgbFormat eSubRgbFmt;
    MS_BOOL bSubIsInit;
    MS_BOOL bSubRptPreVsync;
    MS_BOOL bSubIs422;
    MS_BOOL bSubIsH265;
    MS_BOOL bSub3DLRAltOutput;    /// 3D L/R line alternative output
    MS_BOOL bSub3DLRAltSBSOutput;
    MS_BOOL bSubIsHS;
    MS_U16  u16SubCropXStart;
    MS_U16  u16SubCropYStart;
    MS_U16  u16SubCropXSize;
    MS_U16  u16SubCropYSize;
    MS_BOOL bSubIsSetCrop;
    MS_BOOL bSubIsXcTrig;
    MS_U32 u32SubMVOPFixClk;
    MS_BOOL bIsSubEnable;
    MS_BOOL bSubIs2p;
    MVOP_OutputImodeType eSubInterlaceType;
#endif
}MVOP_CTX_HAL;

static MVOP_CTX_HAL *g_pHalMVOPCtx = NULL;
static MS_VIRT u32RiuBaseAdd=0;

#ifndef MSOS_TYPE_LINUX
    MVOP_CTX_HAL g_stmvopHalCtx;
#endif
#define RIU_MAP u32RiuBaseAdd  //obtained in init

#define RIU8    ((unsigned char  volatile *) RIU_MAP)
#define RIU16    ((MS_U16  volatile *) RIU_MAP)
#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define HAL_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Val;                                     \
    }                                                                                   \
    MST_MACRO_END

#define HAL_ReadByte( u32Reg )                                                         \
    (__builtin_constant_p( u32Reg ) ?                                                   \
        (((u32Reg) & 0x01) ? RIU8[(u32Reg) * 2 - 1] : RIU8[(u32Reg) * 2]) :             \
        (RIU8[(u32Reg << 1) - (u32Reg & 1)]))

#define HAL_Read2Byte( u32Reg )                                                         \
            (RIU16[u32Reg])

#define HAL_ReadRegBit( u32Reg, u8Mask )                                               \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)] & (u8Mask))

#define HAL_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    MS_U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                   \
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :                           \
                                (RIU8[u32Reg8] & ~(u8Mask));                            \
    MST_MACRO_END

#define HAL_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    MST_MACRO_START                                                                     \
    MS_U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                   \
    RIU8[u32Reg8] = (RIU8[u32Reg8] & ~(u8Msk)) | ((u8Val) & (u8Msk));                   \
    MST_MACRO_END

#define HAL_Write2Byte(u32Reg, u16Val)                                                       \
            RIU16[u32Reg] = u16Val;

#define SUB_REG(x)         (x-MVOP_REG_BASE+MVOP_SUB_REG_BASE)

#define _FUNC_NOT_USED()        do {} while ( 0 )
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define LOWBYTE(u16)            ((MS_U8)(u16))
#define HIGHBYTE(u16)           ((MS_U8)((u16) >> 8))


#define VOP_ON_MIU1                     ((HAL_ReadByte(0x12F9) & BIT2) == BIT2)
#define _MaskMiuReq_MVOP_R( m )         HAL_WriteRegBit(0x12C7, m, BIT2)
#define _MaskMiu1Req_MVOP_R( m )        HAL_WriteRegBit(0x06C7, m, BIT2)

#define HAL_MIU_SetReqMask( miu_clients, mask ) \
   do { \
       if (VOP_ON_MIU1 == 0) \
           _MaskMiuReq_##miu_clients( mask ); \
       else \
           _MaskMiu1Req_##miu_clients( mask ); \
   }while(0)

#define SUBVOP_ON_MIU1                  ((HAL_ReadByte(0x12F9) & BIT2) == BIT2)
#define _MaskMiuReq_SUBMVOP_R( m )         HAL_WriteRegBit(0x12C7, m, BIT2)
#define _MaskMiu1Req_SUBMVOP_R( m )        HAL_WriteRegBit(0x06C7, m, BIT2)

#define HAL_MIU_SubSetReqMask( miu_clients, mask ) \
   do { \
       if (SUBVOP_ON_MIU1 == 0) \
           _MaskMiuReq_##miu_clients( mask ); \
       else \
           _MaskMiu1Req_##miu_clients( mask ); \
   }while(0)

#define MVOP_DBG_ENABLE 0
#if MVOP_DBG_ENABLE
#define MVOP_DBG(fmt, args...)       MVOP_PRINTF(fmt, ##args)
#else
#define MVOP_DBG(fmt, args...)       {}
#endif

typedef enum
{
    E_MVOP_INIT_OK            = 0,
    E_MVOP_INIT_FAIL          = 1,
    E_MVOP_INIT_ALREADY_EXIST = 2
} MVOP_HalInitCtxResults;
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

#define Y_INFO 0
#define UV_INFO 1

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void _HAL_MVOP_InitVarCtx(void)
{
    g_pHalMVOPCtx->bMirrorModeVer = 0;
    g_pHalMVOPCtx->bMirrorModeHor = 0;
    g_pHalMVOPCtx->bMirrorMode=0;
    g_pHalMVOPCtx->bEnableFreerunMode = FALSE;
    g_pHalMVOPCtx->b3DLRMode=0;    /// 3D L/R dual buffer mode
    g_pHalMVOPCtx->b3DLRAltOutput = FALSE;    /// 3D L/R line alternative output
    g_pHalMVOPCtx->b3DLRAltSBSOutput = FALSE;
    g_pHalMVOPCtx->bNewVSyncMode = FALSE;
    g_pHalMVOPCtx->eRepeatField = E_MVOP_RPTFLD_NONE;      /// mvop output repeating fields for single field input.
    g_pHalMVOPCtx->eMainRgbFmt = E_MVOP_RGB_NONE;
    g_pHalMVOPCtx->bIsInit = 0;
    g_pHalMVOPCtx->bRptPreVsync = 0;
    g_pHalMVOPCtx->bIs422 = 0;
    g_pHalMVOPCtx->bIsH265 = 0;
    g_pHalMVOPCtx->bIsHS = FALSE;
    g_pHalMVOPCtx->u16CropXStart = 0;
    g_pHalMVOPCtx->u16CropYStart = 0;
    g_pHalMVOPCtx->u16CropXSize = 0;
    g_pHalMVOPCtx->u16CropYSize = 0;
    g_pHalMVOPCtx->bIsSetCrop = FALSE;
    g_pHalMVOPCtx->bIsXcTrig = TRUE;
    g_pHalMVOPCtx->u32MVOPFixClk = 0;
    g_pHalMVOPCtx->bIsEnable = 0;
    g_pHalMVOPCtx->bIs2p = 0;
    g_pHalMVOPCtx->eInterlaceType = E_MVOP_PRO;
}

void _HAL_MVOP_SubInitVarCtx(void)
{
#if MVOP_SUPPORT_SUB
    g_pHalMVOPCtx->bSubMirrorModeVer = 0;
    g_pHalMVOPCtx->bSubMirrorModeHor = 0;
    g_pHalMVOPCtx->bSubMirrorMode=0;
    g_pHalMVOPCtx->bSubEnableFreerunMode = FALSE;
    g_pHalMVOPCtx->bSub3DLRMode=0;
    g_pHalMVOPCtx->bSubNewVSyncMode = FALSE;
    g_pHalMVOPCtx->eSubRepeatField = E_MVOP_RPTFLD_NONE;   /// mvop output repeating fields for single field input.
    g_pHalMVOPCtx->eSubRgbFmt = E_MVOP_RGB_NONE;
    g_pHalMVOPCtx->bSubIsInit = 0;
    g_pHalMVOPCtx->bSubRptPreVsync = 0;
    g_pHalMVOPCtx->bSubIs422 = 0;
    g_pHalMVOPCtx->bSubIsH265 = 0;
    g_pHalMVOPCtx->bSub3DLRAltOutput = FALSE;    /// 3D L/R line alternative output
    g_pHalMVOPCtx->bSub3DLRAltSBSOutput = FALSE;
    g_pHalMVOPCtx->bSubIsHS = FALSE;
    g_pHalMVOPCtx->u16SubCropXStart = 0;
    g_pHalMVOPCtx->u16SubCropYStart = 0;
    g_pHalMVOPCtx->u16SubCropXSize = 0;
    g_pHalMVOPCtx->u16SubCropYSize = 0;
    g_pHalMVOPCtx->bSubIsSetCrop = FALSE;
    g_pHalMVOPCtx->bSubIsXcTrig = TRUE;
    g_pHalMVOPCtx->u32SubMVOPFixClk = 0;
    g_pHalMVOPCtx->bIsSubEnable = 0;
    g_pHalMVOPCtx->bSubIs2p = 0;
    g_pHalMVOPCtx->eSubInterlaceType = E_MVOP_PRO;
#endif
}

MVOP_HalInitCtxResults _HAL_MVOP_InitContext(MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bNeedInitShared = FALSE;

//check first init by MsOS_SHM_GetId / MSOS_SHM_QUERY
#if 0
    if (g_pHalMVOPCtx)
    {
        // The context instance has been created already
        // before somewhere sometime in the same process.
        *pbFirstDrvInstant = bNeedInitShared;
        //return E_MVOP_INIT_FAIL;
        return E_MVOP_INIT_OK;
    }
#endif

#ifdef MSOS_TYPE_LINUX
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;

    //MsOS_SHM_Init(); init in msos_init
    if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVOP HAL driver", sizeof(MVOP_CTX_HAL), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVOP HAL driver", sizeof(MVOP_CTX_HAL), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            MVOP_DBG(MVOP_PRINTF("MVOP: SHM allocation failed!\n");)
            return E_MVOP_INIT_FAIL;
        }
        MVOP_DBG(MVOP_PRINTF("MVOP: [%s][%d] This is first initial 0x%08lx\n", __FUNCTION__, __LINE__, u32Addr);)
        memset( (MS_U8*)u32Addr, 0, sizeof(MVOP_CTX_HAL));
        bNeedInitShared = TRUE;
        g_pHalMVOPCtx = (MVOP_CTX_HAL*)u32Addr;
        _HAL_MVOP_InitVarCtx();
#if MVOP_SUPPORT_SUB
        _HAL_MVOP_SubInitVarCtx();
#endif
    }
    else
    {
        g_pHalMVOPCtx = (MVOP_CTX_HAL*)u32Addr;
        bNeedInitShared = FALSE;
        *pbFirstDrvInstant = bNeedInitShared;

        return E_MVOP_INIT_ALREADY_EXIST;
    }
#else
      g_pHalMVOPCtx =  &g_stmvopHalCtx;
      bNeedInitShared = TRUE;
#endif
      *pbFirstDrvInstant = bNeedInitShared;

    if (bNeedInitShared)
    {
        memset(g_pHalMVOPCtx, 0, sizeof(*g_pHalMVOPCtx));
    }

      return E_MVOP_INIT_OK;
}

MS_BOOL _HAL_MVOP_IsSupport4k2k2P(void)
{
    return TRUE;
}

void HAL_MVOP_RegSetBase(MS_VIRT u32Base)
{
    u32RiuBaseAdd = u32Base;
}

void HAL_MVOP_InitMirrorMode(MS_BOOL bMir)
{
    //set bit[3:7] to support mirror mode
    HAL_WriteRegBit(VOP_MIRROR_CFG, bMir, VOP_MIRROR_CFG_ENABLE);
}

void HAL_MVOP_SetVerticalMirrorMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

    if (VOP_MIRROR_CFG_ENABLE != HAL_ReadRegBit(VOP_MIRROR_CFG, VOP_MIRROR_CFG_ENABLE))
    {
        //MVOP_PRINTF("Setup mirror mode\n");
        HAL_MVOP_InitMirrorMode(TRUE);
    }

    HAL_WriteRegBit(VOP_MIRROR_CFG, bEnable, VOP_MIRROR_CFG_VEN);
    g_pHalMVOPCtx->bMirrorModeVer = bEnable;
}

void HAL_MVOP_SetHorizontallMirrorMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

    if (VOP_MIRROR_CFG_ENABLE != HAL_ReadRegBit(VOP_MIRROR_CFG, VOP_MIRROR_CFG_ENABLE))
    {
        //MVOP_PRINTF("Setup mirror mode\n");
        HAL_MVOP_InitMirrorMode(TRUE);
    }

    HAL_WriteRegBit(VOP_MIRROR_CFG, bEnable, VOP_MIRROR_CFG_HEN);
    g_pHalMVOPCtx->bMirrorModeHor = bEnable;
}

void HAL_MVOP_Init(void)
{
    MVOP_HalInitCtxResults eRet;
    MS_BOOL pbFirstDrvInstant;

    eRet = _HAL_MVOP_InitContext(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("[%s] MVOP Context Init failed!\n",__FUNCTION__);)
        return;
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
        if(g_pHalMVOPCtx->bIsInit)
        {
            MVOP_DBG(MVOP_PRINTF("[%s] Main MVOP Context has Initialized!\n",__FUNCTION__);)
            return;
        }
    }

    HAL_MVOP_InitMirrorMode(TRUE);
    //Enable dynamic clock gating
    //Note: cannot enable VOP_GCLK_VCLK_ON, or hsync cannot be sent out.
    HAL_WriteByteMask(VOP_GCLK, VOP_GCLK_MIU_ON, VOP_GCLK_MIU_ON|VOP_GCLK_VCLK_ON);
    g_pHalMVOPCtx->bIsInit = 1;
}

void HAL_MVOP_SetFieldInverse(MS_BOOL b2MVD, MS_BOOL b2IP)
{
    // Set fld inv & ofld_inv
    HAL_WriteRegBit(VOP_CTRL0+1, b2MVD, BIT3); //inverse the field to MVD
    HAL_WriteRegBit(VOP_CTRL0+1, b2IP, BIT4);  //inverse the field to IP
}

void HAL_MVOP_SetChromaWeighting(MS_BOOL bEnable)
{
    HAL_WriteRegBit(VOP_REG_WEIGHT_CTRL, bEnable, BIT1);
}

//load new value into active registers 0x20-0x26
void HAL_MVOP_LoadReg(void)
{
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT0);
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT0);

    HAL_WriteRegBit(VOP_REG_WR, 1, BIT4);
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT4);
}


void HAL_MVOP_SetMIUReqMask(MS_BOOL bEnable)
{
#if 0   //[FIXME]
    if (bEnable)
    {   // mask MVOP2MI to protect MIU
        HAL_MIU_SetReqMask(MVOP_R, 1);
        MsOS_DelayTask(1);
    }
    else
    {   // unmask MVOP2MI
        MsOS_DelayTask(1);
        HAL_MIU_SetReqMask(MVOP_R, 0);
    }
#endif
    MS_U8 u8Miu;

    if(HAL_MVOP_GetIsOnlyMiuIPControl() == TRUE)
    {
        // mask msb mvop
        u8Miu = (HAL_ReadByte(VOP_MIU_SEL) & VOP_MSB_BUF0_MIU_SEL) >> 4;
    }
    else
    {
        u8Miu = VOP_ON_MIU1;
    }
    eMIUClientID eClientID = MIU_CLIENT_MVOP_128BIT_R;
    //MVOP_PRINTF("Enter %s bEnable=%x ReqMask=0x%x, 0x%x\n", __FUNCTION__, bEnable,
    //    HAL_ReadByte(0x1266), HAL_ReadByte(0x0666));
    if (bEnable)
    {   // mask MVOP2MI to protect MIU
        MDrv_MIU_MaskReq(u8Miu,eClientID);
        MsOS_DelayTask(1);
    }
    else
    {   // unmask MVOP2MI
        MsOS_DelayTask(1);
        MDrv_MIU_UnMaskReq(u8Miu,eClientID);
    }

    //MVOP_PRINTF(">Exit %s bEnable=%x ReqMask=0x%x, 0x%x\n", __FUNCTION__, bEnable,
    //    HAL_ReadByte(0x1266), HAL_ReadByte(0x0666));
}

void HAL_MVOP_Rst(void)
{
    MS_BOOL bMCU = FALSE;

    bMCU = HAL_ReadRegBit(VOP_MPG_JPG_SWITCH, BIT1);
    HAL_WriteRegBit(VOP_CTRL0, 0, BIT0);
    HAL_WriteRegBit(VOP_CTRL0, 1, BIT0);
    // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
    MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
    HAL_MVOP_SetBlackBG();
    HAL_MVOP_SetPattern(MVOP_PATTERN_FRAMECOLOR);
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT4);
    HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 1, BIT1);
    MsOS_DelayTask(40);
    if(bMCU == FALSE)
    {
        HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 0, BIT1);
    }
    HAL_MVOP_SetPattern(MVOP_PATTERN_NORMAL);
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT4);
}

void HAL_MVOP_Enable(MS_BOOL bEnable, MS_U8 u8Framerate)
{
    MS_U8 regval;
    MS_U8 u8FrmDur = 40;
    MS_BOOL bMCU = FALSE;

    bMCU = HAL_ReadRegBit(VOP_MPG_JPG_SWITCH, BIT1);

    if(u8Framerate != 0)
    {
        u8FrmDur = 1000/u8Framerate; //time of one frame(ms).
    }

    regval = HAL_ReadByte(VOP_CTRL0);

    if ( bEnable )
    {
        regval |= 0x1;
    }
    else
    {
        regval &= ~0x1;
    }


    // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
    if( bEnable && (g_pHalMVOPCtx->bIsEnable == FALSE)) // need patch only from off to on
    {
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
        HAL_MVOP_SetBlackBG();
        HAL_MVOP_SetPattern(MVOP_PATTERN_FRAMECOLOR);
        HAL_WriteRegBit(VOP_REG_WR, 1, BIT4);
        HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 1, BIT1);
        MsOS_DelayTask(u8FrmDur);
        if(bMCU == FALSE)
        {
            HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 0, BIT1);
        }
        HAL_MVOP_SetPattern(MVOP_PATTERN_NORMAL);
        HAL_WriteRegBit(VOP_REG_WR, 0, BIT4);
    }
    g_pHalMVOPCtx->bIsEnable = bEnable;

    HAL_WriteByte(VOP_CTRL0, regval);
}

MS_BOOL HAL_MVOP_GetEnableState(void)
{
    return (HAL_ReadRegBit(VOP_CTRL0, BIT0));
}

HALMVOPFREQUENCY HAL_MVOP_GetMaxFreerunClk(void)
{
    return HALMVOP_160MHZ;
}

HALMVOPFREQUENCY HAL_MVOP_Get4k2kClk(void)
{
    return HALMVOP_320MHZ;
}

void HAL_MVOP_SetFrequency(HALMVOPFREQUENCY enFrequency)
{
    // clear
    HAL_WriteByteMask(REG_CKG_DC0, 0, CKG_DC0_MASK);
    switch(enFrequency)
    {
        case HALMVOP_SYNCMODE:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_SYNCHRONOUS, CKG_DC0_MASK);
            break;
        case HALMVOP_FREERUNMODE:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_FREERUN, CKG_DC0_MASK);
            break;
        case HALMVOP_320MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_320MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_108MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_108MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_123MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_123MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_144MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_144MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_160MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_160MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_192MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_192MHZ, CKG_DC0_MASK);
            break;
        default:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_160MHZ, CKG_DC0_MASK);
            MVOP_PRINTF("Attention! In HAL_MVOP_SetFrequency default 160MHz!\n");
            break;
    }
}

HALMVOPFREQUENCY HAL_MVOP_GetMaximumClk(void)
{
    return HALMVOP_320MHZ;
}

HALMVOPFREQUENCY HAL_MVOP_GetCurrentClk(void)
{
    MS_U8 u8DC0Clk;
    HALMVOPFREQUENCY mvopFreq;

    u8DC0Clk = HAL_ReadByte(REG_CKG_DC0)&CKG_DC0_MASK;

    switch(u8DC0Clk)
    {
        case CKG_DC0_SYNCHRONOUS:
            mvopFreq = HALMVOP_SYNCMODE;
            break;
        case CKG_DC0_FREERUN:
            mvopFreq = HALMVOP_FREERUNMODE;
            break;
        case CKG_DC0_144MHZ:
            mvopFreq = HALMVOP_144MHZ;
            break;
        case CKG_DC0_160MHZ:
            mvopFreq = HALMVOP_160MHZ;
            break;
        case CKG_DC0_320MHZ:
            mvopFreq = HALMVOP_320MHZ;
            break;
        default:
            mvopFreq = 0;
            break;
    }
    return mvopFreq;
}

void HAL_MVOP_SetOutputInterlace(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(VOP_CTRL0);

    if ( bEnable )
    {
        regval |= 0x80;
        HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
    }
    else
    {
        regval &= ~0x80;
    }
    //Kano patch for GOP
    HAL_WriteRegBit(VOP_TST_IMG, bEnable, VOP_SC_VS_INV);
    HAL_WriteByte(VOP_CTRL0, regval);
}

void HAL_MVOP_SetPattern(MVOP_Pattern enMVOPPattern)
{
    HAL_WriteByteMask(VOP_TST_IMG, enMVOPPattern, BIT2 | BIT1 | BIT0);
}

MS_BOOL HAL_MVOP_SetTileFormat(MVOP_TileFormat eTileFmt)
{
    if (eTileFmt == E_MVOP_TILE_8x32)
    {
        HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);
        return TRUE;
    }
    else if (eTileFmt == E_MVOP_TILE_16x32)
    {
        HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MVOP_GetSupportRgbFormat(MVOP_RgbFormat eRgbFmt)
{
    if ((eRgbFmt == E_MVOP_RGB_565) || (eRgbFmt == E_MVOP_RGB_888))
    {
        return TRUE;
    }
    return FALSE;
}

MS_BOOL HAL_MVOP_SetRgbFormat(MVOP_RgbFormat eRgbFmt)
{
    MS_BOOL bRet = FALSE;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }

    if (eRgbFmt == E_MVOP_RGB_NONE)
    {
        HAL_WriteByteMask(VOP_RGB_FMT, 0, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }
    else if (eRgbFmt == E_MVOP_RGB_565)
    {
        HAL_WriteByteMask(VOP_RGB_FMT, VOP_RGB_FMT_565, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }
    else if (eRgbFmt == E_MVOP_RGB_888)
    {
        HAL_WriteByteMask(VOP_RGB_FMT, VOP_RGB_FMT_888, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }

    if (bRet == TRUE)
    {
        g_pHalMVOPCtx->eMainRgbFmt = eRgbFmt;
    }
    return bRet;
}

void HAL_MVOP_SetBlackBG(void)
{
    MS_U8 regval;

    //set MVOP test pattern to black
    HAL_WriteByte((VOP_TST_IMG + 1), 0x10);
    HAL_WriteByte(VOP_U_PAT      , 0x80);
    HAL_WriteByte((VOP_U_PAT   + 1), 0x80);

    regval = HAL_ReadByte(VOP_TST_IMG);
    HAL_WriteByte(VOP_TST_IMG, 0x02);
    HAL_WriteByte(VOP_TST_IMG, 0x00);
    HAL_WriteByte(VOP_TST_IMG, regval);
}

void HAL_MVOP_SetCropWindow(MVOP_InputCfg *pparam)
{
#if 1
    UNUSED(pparam);
#else // enable it when test code is ready
    MS_U32 x, y;
    MS_U32 u32offset;

    if(!pparam)
    {
        return;
    }
    //set MVOP test pattern to black
    HAL_MVOP_SetBlackBG();
#if 0
    if((pparam->enVideoType == MVOP_H264) && (pparam->u16StripSize == 1920))
    {
        pparam->u16StripSize = 1952;
    }
#endif
    if((pparam->enVideoType == MVOP_MPG) || (pparam->enVideoType == MVOP_MPEG4))
    {
        pparam->u16CropX = (pparam->u16CropX >> 3) << 3; // 8 bytes align
        pparam->u16CropY = (pparam->u16CropY >> 6) << 6; // 64 bytes align
    }
    else if((pparam->enVideoType == MVOP_H264) || (pparam->enVideoType == MVOP_RM))
    {
        pparam->u16CropX = (pparam->u16CropX >> 4) << 4; // 16 bytes align
        pparam->u16CropY = (pparam->u16CropY >> 6) << 6; // 64 bytes align
    }
    else
    {
        MS_ASSERT(0);
    }

    x = (MS_U32)pparam->u16CropX;
    y = (MS_U32)pparam->u16CropY;

    // y offset
    u32offset = ((y * pparam->u16StripSize + (x << 5)) >> 3);
    HAL_WriteByte((VOP_JPG_YSTR0_L    ), (MS_U8)(u32offset));
    HAL_WriteByte((VOP_JPG_YSTR0_L + 1), (MS_U8)(u32offset >> 8));
    HAL_WriteByte((VOP_JPG_YSTR0_H    ), (MS_U8)(u32offset >> 16));
    HAL_WriteByte((VOP_JPG_YSTR0_H + 1), (MS_U8)((u32offset >> 24) & VOP_YUV_STR_HIBITS));

    // uv offset
    u32offset = ((y >> 1) * pparam->u16StripSize + (x << 5)) >> 3;
    HAL_WriteByte((VOP_JPG_UVSTR0_L    ), (MS_U8)(u32offset));
    HAL_WriteByte((VOP_JPG_UVSTR0_L + 1), (MS_U8)(u32offset >> 8));
    HAL_WriteByte((VOP_JPG_UVSTR0_H    ), (MS_U8)(u32offset >> 16));
    HAL_WriteByte((VOP_JPG_UVSTR0_H + 1), (MS_U8)((u32offset >> 24) & VOP_YUV_STR_HIBITS));

    pparam->u16CropWidth= (pparam->u16CropWidth >> 3) << 3;
    // HSize, VSize
    HAL_WriteByte((VOP_JPG_HSIZE    ), LOWBYTE(pparam->u16CropWidth ));
    HAL_WriteByte((VOP_JPG_HSIZE + 1), HIGHBYTE(pparam->u16CropWidth ));
    HAL_WriteByte((VOP_JPG_VSIZE    ), LOWBYTE(pparam->u16CropHeight));
    HAL_WriteByte((VOP_JPG_VSIZE + 1), HIGHBYTE(pparam->u16CropHeight ));

    HAL_WriteByteMask(VOP_MPG_JPG_SWITCH, BIT0, BIT1|BIT0);

    // clear extend strip len bit by default
    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);
    if((pparam->enVideoType == MVOP_MPG) || (pparam->enVideoType == MVOP_MPEG4))
    {
        // Disable H264 or RM Input
        HAL_WriteRegBit(VOP_INPUT_SWITCH1, 0, BIT2|BIT3);
        //8*32 tile format
        HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);
    }
    else if((pparam->enVideoType == MVOP_H264) || (pparam->enVideoType == MVOP_RM))
    {
        //16*32 tile format
        HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);
        // SVD mode enable
        HAL_WriteByteMask(VOP_INPUT_SWITCH1, BIT3, BIT2|BIT3);
        // set mvop to 64bit interface
        HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);
    }
    HAL_MVOP_LoadReg();
#endif
}

void HAL_MVOP_SetRepeatField(MVOP_RptFldMode eMode)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->eRepeatField = eMode;
}

void HAL_MVOP_SetFDMaskFromMVD(MS_BOOL bEnable)
{
    HAL_WriteRegBit(VOP_REG_FD_MASK_SEL, bEnable, BIT5); //FD MASK from MVD
}

void HAL_MVOP_SetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion)
{
    MS_U8 regval;
    MS_U16 u16strip, u16strip_lsb;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
#if 0
    /*****************************************************/
    // Reset MVOP setting
    HAL_WriteByte(VOP_TST_IMG, 0x40); //reset test pattern or BG
    HAL_MVOP_Set3DLRAltOutput_VHalfScaling(DISABLE); //reset to default: disable 3D L/R alternative output.
    HAL_MVOP_Set3DLR2ndCfg(DISABLE);    //reset to default: disable 3D L/R 2nd pitch.
    HAL_MVOP_SetRgbFormat(E_MVOP_RGB_NONE); //reset rgb format
    HAL_WriteRegBit(VOP_CTRL1, 0, VOP_MVD_VS_MD); //default use original vsync
    //stb series
    HAL_WriteRegBit(VOP_REG_422_OUT_EN, 1, BIT0);
    HAL_WriteRegBit(VOP_HANDSHAKE, 1, VOP_HANDSHAKE_MODE);
    HAL_MVOP_SetTimingFromXC(E_MVOP_DEV_0, TRUE);

    //set MVOP test pattern to black
    HAL_MVOP_SetBlackBG();

    // clear extend strip len bit by default
    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Disable H264 or RM Input
    HAL_WriteRegBit(VOP_INPUT_SWITCH1, 0, BIT2|BIT3);
    // Clear 422 Flag
    g_pHalMVOPCtx->bIs422 = 0;
    //8*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);
    HAL_WriteRegBit(VOP_MIRROR_CFG_HI, 0, VOP_REF_SELF_FLD);

    // EVD mode disable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 0, EVD_ENABLE);
    // Clear evd Flag for interlace mode setting
    g_pHalMVOPCtx->bIsH265 = 0;
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 1, BIT3);
    HAL_WriteRegBit(VOP_MIRROR_CFG, 1, BIT5);
    // EVD 10 bits disable
    HAL_WriteByteMask(VOP_REG_MASK, BIT1, VOP_LSB_REQ_MASK);
    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_Y_EN);
    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_UV_EN);
    HAL_WriteRegBit(VOP_UV_SHIFT, 1, VOP_GCLK_MIU_ON);
    // Disable 420 BW Saving mode
    HAL_WriteRegBit(VOP_REG_DUMMY, 0, VOP_420_BW_SAVE);
    HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
    // Disable New Vsync Mode
    g_pHalMVOPCtx->bNewVSyncMode = FALSE;
    // Disable 2p mode
    HAL_MVOP_SetEnable4k2k2P(FALSE);
    /* only for STB DC */
    // Handshake mode chip need to set from MVD: Kappa (maybe fix in after project)
    HAL_MVOP_SetFDMaskFromMVD(ENABLE);
    // Use frame sync (CLK_DC1 domain) to reset CLK_DC0 and CLK_MIU domain every frame
    HAL_WriteRegBit(VOP_REG_FRAME_RST+1, 0, BIT7); // reg_frame_rst = 0

    // Kappa do not need to invert DC2SC field; the inverse was setted in SC XC_BK02_33[3]=1
    HAL_MVOP_SetFieldInverse(ENABLE, DISABLE);
    // kappa U02 need to bypass field from mvd to xc for FCR.
    HAL_WriteRegBit(VOP_REG_DUMMY_3D_1, 1, VOP_RGB_FILED_BYPASS);

    // Clippers temp patch: New diu interface, waiting for decoder adding diu setting.
    HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL | VOP_INFO_FROM_CODEC_MIU_BUF1_SEL | VOP_INFO_FROM_CODEC_10BIT);

    //Change MIU BW Default Value to 0x241C (after Einstein)
    //FIFO threshold
    HAL_WriteByteMask(VOP_DMA0, 0x08, VOP_FORCE_HIGH|VOP_HI_TSH|VOP_BURST_EXT);
    //burst and priority
    HAL_WriteByteMask(VOP_DMA1, 0x22, VOP_FORCE_HIGH|VOP_HI_TSH|VOP_BURST_EXT);
    HAL_WriteByte(VOP_BURST_CTRL0, 0x02);
    HAL_WriteByte(VOP_BURST_CTRL1, 0x00);
    //// clippers crop sw patch /////
    HAL_WriteRegBit(VOP_REG_SINGLE_3D_L, 0, VOP_FORCE_SKIP);
    HAL_WriteByteMask(VOP_REG_SINGLE_3D_L, 0, VOP_SKIP_SIZE_LVIEW);
    HAL_WriteByteMask(VOP_REG_SINGLE_3D_H, 0, VOP_SKIP_SIZE_RVIEW);
    HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_LVIEW);
    HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_RVIEW);
    g_pHalMVOPCtx->g_u16SetStartX = 0;
    g_pHalMVOPCtx->g_u16SetStartY = 0;
    g_pHalMVOPCtx->g_u16SetXSize = 0;
    g_pHalMVOPCtx->g_u16SetYSize = 0;
    g_pHalMVOPCtx->g_bIsY4Align = FALSE;
    g_pHalMVOPCtx->g_bIsSetCrop = FALSE;
    g_pHalMVOPCtx->bIsXcTrig = FALSE;
    g_pHalMVOPCtx->bSubIsXcTrig = FALSE;
    /*****************************************************/
#endif

    //regval = HAL_ReadByte(VOP_MPG_JPG_SWITCH);
    regval = 0;
    regval |= ( mode & 0x3 );

    if ( mode == VOPINPUT_HARDWIRE )
    {
        HAL_WriteByte(VOP_MPG_JPG_SWITCH, regval);
    }
    else if ( mode == VOPINPUT_HARDWIRECLIP )
    {
        HAL_WriteByte(VOP_MPG_JPG_SWITCH, regval);

        // HSize, VSize
        HAL_WriteByte(VOP_JPG_HSIZE    , LOWBYTE( pparam->u16HSize ));
        HAL_WriteByte((VOP_JPG_HSIZE + 1), HIGHBYTE( pparam->u16HSize ));
        HAL_WriteByte(VOP_JPG_VSIZE    , LOWBYTE( pparam->u16VSize ));
        HAL_WriteByte((VOP_JPG_VSIZE + 1), HIGHBYTE( pparam->u16VSize ));
    }
    else if (mode == VOPINPUT_MCUCTRL)
    {
        // disable from wb
        HAL_WriteRegBit(VOP_REG_MFDEC_2_L, 0, VOP_MF_FROM_WB);
        HAL_WriteRegBit(VOP_INPUT_SWITCH0, 0, VOP_R2_WISHBONE);
        HAL_WriteRegBit(VOP_REG_DUMMY, 0, VOP_32x32_WB);
        g_pHalMVOPCtx->bMirrorMode = (g_pHalMVOPCtx->bMirrorModeVer||g_pHalMVOPCtx->bMirrorModeHor);
        if ( pparam->bProgressive )
            regval |= 0x4;
        else
        {
            regval &= ~0x4;
            regval |= 0x1;  //reg_dc_md=b'11 for interlace input
            if (E_MVOP_RPTFLD_NONE == g_pHalMVOPCtx->eRepeatField)
            {
                MVOP_DBG("%s normal NOT repeat field %x\n", __FUNCTION__, g_pHalMVOPCtx->eRepeatField);
                //To support mcu mode interlace, need to set h'3B[9]=1,
                //h'11[12]=0, and Y1/UV1 address equal to Y0/UV0 address.
                HAL_WriteRegBit(VOP_MIRROR_CFG_HI, 1, VOP_REF_SELF_FLD);
                //HAL_MVOP_SetFieldInverse(ENABLE, DISABLE); //do not inverse in kano: mvop gen timing + hsk mode
                // mheg5, mcu mode for i mode, do not bypass, 0x11[6] = 0: mvop t/b toggle
                HAL_WriteRegBit(VOP_REG_DUMMY_3D_1, 0, VOP_RGB_FILED_BYPASS);
                HAL_WriteRegBit(VOP_CTRL0, 0, VOP_EXTFLD_EN);
            }
        }

        if ( pparam->bYUV422 )
            regval |= 0x10;
        else
            regval &= ~0x10;

        if ( pparam->b422pack )
            regval |= 0x80;

        if ( pparam->bDramRdContd == 1 )
            regval |= 0x20;
        else
            regval &= ~0x20;

        // for backward compatable to saturn
        // [3] UV-7bit mode don't care
        // [5] dram_rd_md =0
        // [6] Fld don't care
        // [7] 422pack don'care
        HAL_WriteByte(VOP_MPG_JPG_SWITCH, regval);
        g_pHalMVOPCtx->bIs422 = pparam->bYUV422;
        HAL_WriteRegBit(VOP_REG_DUMMY, !(pparam->bYUV422), VOP_420_BW_SAVE);
        HAL_WriteRegBit(VOP_REG_BW_SAVE, !(pparam->bYUV422), VOP_420_BW_SAVE_EX);

        if (pparam->u16StripSize == 0)
        {
            if (pparam->bSD)
            {
                u16strip = 720;
                u16strip_lsb = 720;
            }
            else
            {
                u16strip = 1920;
                u16strip_lsb = 1920;
            }
        }
        else
        {
            u16strip = pparam->u16StripSize;
            u16strip_lsb = pparam->u16StripSize;
        }

        // set dc_strip[7:0]
        if ( pparam->bDramRdContd == 0 )
        {
            u16strip = (u16strip + 31) / 32 * 32; //need align for monaco
            u16strip = u16strip/8;
            u16strip_lsb = (u16strip_lsb+127)/128;
            u16strip_lsb *= 4;
        }
        else
        {
            if ( pparam->b422pack )
            {
                if (E_MVOP_RGB_888 == g_pHalMVOPCtx->eMainRgbFmt)
                {
                    u16strip *= 2; //4bytes/pixel (yuv422:2bytes/pixel)
                }
                //VOP_REG_STRIP_ALIGN and Mirror mode are mutually exclusive, after M10(support mirror), VOP_DC_STRIP_H
                //replace VOP_REG_STRIP_ALIGN, which supported maximun Hsize is 8188
#if 0
                // [071016 Andy] support YUV422 pack mode
                if ((u16strip < 1024) || g_pHalMVOPCtx->bMirrorMode)
                {
                    u16strip = u16strip/4;
                    // dont extend strip len
                    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);
                }
                else
                {
                    u16strip = u16strip/8;
                    // extend strip len to 2048
                    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 1, BIT0);
                }
#endif
                u16strip = u16strip/4;
                HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);
            }
            else
            {
                u16strip = u16strip/8;
            }
        }

        if (u16strip >= 256 )
        {
            HAL_WriteByte(VOP_DC_STRIP_H, (u16strip>>8));
            //reg_dc_strip_h[2:0] = reg_dc_strip[10:8]
        }
        else
        {
            HAL_WriteByteMask(VOP_DC_STRIP_H, 0, BIT0 | BIT1 | BIT2);
        }

        regval = u16strip;
        HAL_WriteByte(VOP_DC_STRIP, regval);
        //LSB strip
        HAL_WriteByte(VOP_DC_STRIP_LSB, u16strip_lsb & 0x3ff);


        HAL_MVOP_SetYUVBaseAdd(pparam->u32YOffset, pparam->u32UVOffset,
                               pparam->bProgressive, pparam->b422pack);

        if (E_MVOP_RPTFLD_NONE != g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%s reset eRepeatField=%x ==>", __FUNCTION__, g_pHalMVOPCtx->eRepeatField);
            //To output the same field for single field input,
            //do NOT set h'3B[9]=1 and h'11[12]=0
            g_pHalMVOPCtx->eRepeatField = E_MVOP_RPTFLD_NONE;    //reset the flag to repeat field
            MVOP_DBG(" %x\n", g_pHalMVOPCtx->eRepeatField);
        }

        // HSize
        MS_U16 u16HSize = ALIGN_UPTO_16(pparam->u16HSize);
        if (u16HSize != pparam->u16HSize)
        {
            MVOP_DBG(MVOP_PRINTF("\n\n Change HSize %d to %d\n", pparam->u16HSize, u16HSize);)
        }
        HAL_WriteByte(VOP_JPG_HSIZE    , LOWBYTE( u16HSize ));
        HAL_WriteByte((VOP_JPG_HSIZE + 1), HIGHBYTE( u16HSize ));

        // VSize
        MS_U16 u16VSize = pparam->u16VSize;
        if (g_pHalMVOPCtx->bMirrorModeVer)
        {
            u16VSize = ALIGN_UPTO_4(pparam->u16VSize);
            MVOP_DBG(MVOP_PRINTF("\n\n Change VSize %d to %d\n", pparam->u16VSize, u16VSize);)
        }
        HAL_WriteByte(VOP_JPG_VSIZE    , LOWBYTE( u16VSize ));
        HAL_WriteByte((VOP_JPG_VSIZE + 1), HIGHBYTE( u16VSize ));
    }

    HAL_MVOP_LoadReg();
}


void HAL_MVOP_EnableUVShift(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(VOP_MPG_JPG_SWITCH);

    if (((regval & BIT4) == BIT4) && ((regval & 0x3)== 0x2))
    {   // 422 with MCU control mode
        if (bEnable)
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
    HAL_WriteByteMask(VOP_UV_SHIFT, (bEnable)?1:0, 0x3);
}

static MS_BOOL _bEnable60P = false;
void HAL_MVOP_SetEnable60P(MS_BOOL bEnable)
{
    _bEnable60P = bEnable;
}

static MS_BOOL _bEnable4k2kClk = false;
void HAL_MVOP_SetEnable4k2kClk(MS_BOOL bEnable)
{
    _bEnable4k2kClk = bEnable;
}

void HAL_MVOP_SetEnable4k2k2P(MS_BOOL bEnable)
{
    g_pHalMVOPCtx->bIs2p = bEnable;
    HAL_WriteRegBit(VOP_REG_4K2K_2P, bEnable, VOP_4K2K_2P);
    HAL_MVOP_Set420BWSaveMode(bEnable);
}

MS_BOOL HAL_MVOP_Get4k2k2PMode(MVOP_DevID eID)
{
    MS_BOOL bEnable4k2k2P = FALSE;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            bEnable4k2k2P = g_pHalMVOPCtx->bIs2p;
            break;

        case E_MVOP_DEV_1:
            bEnable4k2k2P = g_pHalMVOPCtx->bSubIs2p;
            break;

        default:
            MVOP_PRINTF("Attention! %s Not support eID=%u yet\n", __FUNCTION__, eID);
            break;
    }
    return bEnable4k2k2P;
}

static MS_BOOL _bEnableFixClk = false;
void HAL_MVOP_SetEnableFixClk(MS_BOOL bEnable)
{
    _bEnableFixClk = bEnable;
}

void HAL_MVOP_EnableFreerunMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->bEnableFreerunMode = bEnable;
}

void HAL_MVOP_SetVSyncMode(MS_U8 u8Mode)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if (1==u8Mode)
    {
        g_pHalMVOPCtx->bNewVSyncMode = TRUE;
    }
    else
    {
        g_pHalMVOPCtx->bNewVSyncMode = FALSE;
    }
}

void HAL_MVOP_SetOutputTiming( MVOP_Timing *ptiming )
{
    MS_U8 regval;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

    HAL_WriteByte(VOP_FRAME_VCOUNT    , LOWBYTE( ptiming->u16V_TotalCount ));
    HAL_WriteByte((VOP_FRAME_VCOUNT + 1), HIGHBYTE( ptiming->u16V_TotalCount ));
    HAL_WriteByte(VOP_FRAME_HCOUNT    , LOWBYTE( ptiming->u16H_TotalCount ));
    HAL_WriteByte((VOP_FRAME_HCOUNT + 1), HIGHBYTE( ptiming->u16H_TotalCount ));

    HAL_WriteByte((VOP_VB0_STR     ), LOWBYTE( ptiming->u16VBlank0_Start ));
    HAL_WriteByte((VOP_VB0_STR  + 1), HIGHBYTE( ptiming->u16VBlank0_Start ));
    HAL_WriteByte((VOP_VB0_END     ), LOWBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte((VOP_VB0_END  + 1), HIGHBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte((VOP_VB1_STR     ), LOWBYTE( ptiming->u16VBlank1_Start ));
    HAL_WriteByte((VOP_VB1_STR  + 1), HIGHBYTE( ptiming->u16VBlank1_Start ));
    HAL_WriteByte((VOP_VB1_END     ), LOWBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte((VOP_VB1_END  + 1), HIGHBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte((VOP_TF_STR      ), LOWBYTE( ptiming->u16TopField_Start ));
    HAL_WriteByte((VOP_TF_STR   + 1), HIGHBYTE( ptiming->u16TopField_Start ));
    HAL_WriteByte((VOP_BF_STR      ), LOWBYTE( ptiming->u16BottomField_Start ));
    HAL_WriteByte((VOP_BF_STR   + 1), HIGHBYTE( ptiming->u16BottomField_Start ));
    HAL_WriteByte((VOP_HACT_STR    ), LOWBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte((VOP_HACT_STR + 1), HIGHBYTE( ptiming->u16HActive_Start ));

    HAL_WriteByte((VOP_TF_VS      ), LOWBYTE( ptiming->u16TopField_VS ));
    HAL_WriteByte((VOP_TF_VS   + 1), HIGHBYTE( ptiming->u16TopField_VS ));
    HAL_WriteByte((VOP_BF_VS      ), LOWBYTE( ptiming->u16BottomField_VS ));
    HAL_WriteByte((VOP_BF_VS   + 1), HIGHBYTE( ptiming->u16BottomField_VS ));

    //if(((ptiming->u16V_TotalCount > 2160) || (ptiming->u16H_TotalCount > 3840)) && (ptiming->u8Framerate >= 30))
    if((!g_pHalMVOPCtx->bIsXcTrig) && (g_pHalMVOPCtx->bIsHS))
    {
        g_pHalMVOPCtx->bNewVSyncMode = TRUE;
    }

    if (g_pHalMVOPCtx->bNewVSyncMode)
    {
        #define NEW_VSYNC_MODE_ADVANCE_LINECNT 30
        MVOP_DBG(MVOP_PRINTF("MVOP use new vync mode, forwarding %d lines\n",NEW_VSYNC_MODE_ADVANCE_LINECNT);)

        MS_U16 u16BottomField_VS2MVD = ptiming->u16BottomField_VS - NEW_VSYNC_MODE_ADVANCE_LINECNT;
        MVOP_DBG(MVOP_PRINTF("BottomField VS ori=0x%x, new=0x%x\n", ptiming->u16BottomField_VS, u16BottomField_VS2MVD);)
        HAL_WriteByte((VOP_BF_VS_MVD    ), LOWBYTE( u16BottomField_VS2MVD ));
        HAL_WriteByte((VOP_BF_VS_MVD + 1), HIGHBYTE( u16BottomField_VS2MVD ));

        MS_U16 u16TopField_VS2MVD = ptiming->u16V_TotalCount - NEW_VSYNC_MODE_ADVANCE_LINECNT;
        MVOP_DBG(MVOP_PRINTF("TopField VS Vtt=0x%x, new=0x%x\n", ptiming->u16V_TotalCount, u16TopField_VS2MVD);)
        HAL_WriteByte((VOP_TF_VS_MVD    ), LOWBYTE( u16TopField_VS2MVD ));
        HAL_WriteByte((VOP_TF_VS_MVD + 1), HIGHBYTE( u16TopField_VS2MVD ));

        HAL_WriteRegBit(VOP_GCLK, 0, VOP_GCLK_MIU_ON);

        HAL_WriteRegBit(VOP_CTRL1, 1, VOP_MVD_VS_MD); //Use new vsync

        g_pHalMVOPCtx->bNewVSyncMode = FALSE; //restore to original mode
    }
    else
    {
        MS_U16 u16BottomField_VS2MVD = 0x200;
        HAL_WriteByte((VOP_BF_VS_MVD    ), LOWBYTE( u16BottomField_VS2MVD ));
        HAL_WriteByte((VOP_BF_VS_MVD + 1), HIGHBYTE( u16BottomField_VS2MVD ));

        MS_U16 u16TopField_VS2MVD = 0x200;
        HAL_WriteByte((VOP_TF_VS_MVD    ), LOWBYTE( u16TopField_VS2MVD ));
        HAL_WriteByte((VOP_TF_VS_MVD + 1), HIGHBYTE( u16TopField_VS2MVD ));
        HAL_WriteRegBit(VOP_CTRL1, 0, VOP_MVD_VS_MD);
    }


    /* Clippers sw patch: sbs + handshake mode video not stable*/
    if(g_pHalMVOPCtx->b3DLRAltSBSOutput)
    {
        ptiming->u16VImg_Start0 = 0xfff;
        ptiming->u16VImg_Start1 = 0xfff;
    }

    // + S3, set default IMG_HSTR, IMG_VSTR0, IMG_VSTR1
#ifdef _SUPPORT_IMG_OFFSET_
    HAL_WriteByte((VOP_IMG_HSTR    ), LOWBYTE( ptiming->u16HImg_Start));
    HAL_WriteByte((VOP_IMG_HSTR + 1), HIGHBYTE( ptiming->u16HImg_Start ));
    HAL_WriteByte((VOP_IMG_VSTR0   ), LOWBYTE( ptiming->u16VImg_Start0));
    HAL_WriteByte((VOP_IMG_VSTR0+ 1), HIGHBYTE( ptiming->u16VImg_Start0 ));
    HAL_WriteByte((VOP_IMG_VSTR1   ), LOWBYTE( ptiming->u16VImg_Start1 ));
    HAL_WriteByte((VOP_IMG_VSTR1+ 1), HIGHBYTE( ptiming->u16VImg_Start1 ));
#else
    HAL_WriteByte((VOP_IMG_HSTR    ), LOWBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte((VOP_IMG_HSTR + 1), HIGHBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte((VOP_IMG_VSTR0   ), LOWBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte((VOP_IMG_VSTR0+ 1), HIGHBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte((VOP_IMG_VSTR1   ), LOWBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte((VOP_IMG_VSTR1+ 1), HIGHBYTE( ptiming->u16VBlank1_End ));
#endif
    // select mvop output from frame color(black)
    HAL_WriteByte((VOP_TST_IMG + 1), 0x10);
    HAL_WriteByte((VOP_U_PAT      ), 0x80);
    HAL_WriteByte((VOP_U_PAT   + 1), 0x80);
    // set mvop src to test pattern
    regval = HAL_ReadByte(VOP_TST_IMG);
    HAL_WriteByte(VOP_TST_IMG, 0x02);
    // make changed registers take effect
    HAL_MVOP_LoadReg();

    HAL_MVOP_SetMIUReqMask(TRUE);
    // reset mvop to avoid timing change cause mvop hang-up
    HAL_MVOP_Rst();
    HAL_MVOP_SetMIUReqMask(FALSE);

    // select mvop output from mvd
    HAL_WriteByte(VOP_TST_IMG, 0x00);
    HAL_WriteByte(VOP_TST_IMG, regval);
    HAL_WriteRegBit(VOP_CTRL0, ptiming->bHDuplicate, BIT2);// H pixel duplicate

#if 0
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
    MVOP_DBG(MVOP_PRINTF(" u16ExpFRate=%u #\n\n", ptiming->u16ExpFrameRate);)
#endif
}

void HAL_MVOP_SetDCClk(MS_U8 clkNum, MS_BOOL bEnable)
{
    MS_ASSERT( (clkNum==0) || (clkNum==1) );
    if (clkNum==0)
    {
        HAL_WriteRegBit(REG_CKG_DC0, !bEnable, CKG_DC0_GATED);
    }
}

void HAL_MVOP_SetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable)
{
    MS_ASSERT( (clkNum==0) || (clkNum==1) );
    if (clkNum==0)
    {
        HAL_WriteRegBit(REG_CKG_DC_SRAM, !bEnable, CKG_DC0_SRAM);
    }
}


void HAL_MVOP_SetSynClk(MVOP_Timing *ptiming)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

    if(g_pHalMVOPCtx->u32MVOPFixClk != 0)
    {
        HAL_MVOP_SetFrequency(g_pHalMVOPCtx->u32MVOPFixClk);
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] Set Fix clock\n");)
    }
    else if(g_pHalMVOPCtx->bEnableFreerunMode)
    {
        MS_U64 u64mpll_clock = MPLL_CLOCK << 27 ;
        do_div(u64mpll_clock, ((MS_U32)ptiming->u16H_TotalCount * (MS_U32)ptiming->u16V_TotalCount * (MS_U32)ptiming->u8Framerate));
        MS_U32 u32FreerunClk = (MS_U32)u64mpll_clock;
        HAL_MVOP_SetFrequency(HALMVOP_FREERUNMODE);
        HAL_WriteByte((REG_DC0_FREERUN_CW_L  ), LOWBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC0_FREERUN_CW_L+1), HIGHBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC0_FREERUN_CW_H  ), LOWBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteByte((REG_DC0_FREERUN_CW_H+1), HIGHBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 1, UPDATE_DC0_FREERUN_CW);
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 0, UPDATE_DC0_FREERUN_CW);
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] Set Freerun clock\n");)
    }
    else if (_bEnable60P)
    {
        HAL_MVOP_SetFrequency(HAL_MVOP_GetMaxFreerunClk());

        HAL_WriteRegBit(VOP_REG_FRAME_RST, 0, BIT15); // reg_frame_rst = 0

        HAL_WriteRegBit(VOP_CTRL0, DISABLE, VOP_FSYNC_EN); // frame sync disable

        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] Set 160MHz clock\n");)
    }
    else if (_bEnable4k2kClk)
    {
        HAL_MVOP_SetFrequency(HAL_MVOP_Get4k2kClk());

        HAL_WriteRegBit(VOP_REG_FRAME_RST, 0, BIT15); // reg_frame_rst = 0

        HAL_WriteRegBit(VOP_CTRL0, DISABLE, VOP_FSYNC_EN); // frame sync disable
        // Clippers sync wz xc: 4k2k default 2p mode enable(xc output 50 or 60)
        HAL_MVOP_SetEnable4k2k2P(TRUE);
        //Change MIU BW Default Value to 0x241C (after Einstein)
        //FIFO threshold
        HAL_WriteByte(VOP_DMA0, 0x10);
        //burst and priority
        HAL_WriteByte(VOP_DMA1, 0x26);
        HAL_WriteByte(VOP_BURST_CTRL0, 0x05);
        HAL_WriteByte(VOP_BURST_CTRL1, 0x00);
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] Set 4k2k clock\n");)
    }
    else
    {
        HAL_MVOP_SetFrequency(HALMVOP_SYNCMODE);
        HAL_WriteByte((REG_DC0_NUM  ), LOWBYTE( ptiming->u16Num));
        HAL_WriteByte((REG_DC0_NUM+1), HIGHBYTE(ptiming->u16Num));
        HAL_WriteByte((REG_DC0_DEN  ), LOWBYTE( ptiming->u16Den));
        HAL_WriteByte((REG_DC0_DEN+1), HIGHBYTE(ptiming->u16Den));
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 1, UPDATE_DC0_SYNC_CW);
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 0, UPDATE_DC0_SYNC_CW);
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] Set Sync clock\n");)
    }
}


void HAL_MVOP_SetMonoMode(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_WriteByte(VOP_U_PAT  , 0x80);
        HAL_WriteByte((VOP_U_PAT+1), 0x80);

        HAL_WriteRegBit(VOP_INPUT_SWITCH1, 1, BIT1);    // Mono mode enable
    }
    else
    {
        HAL_WriteRegBit(VOP_INPUT_SWITCH1, 0, BIT1);    //Mono mode disable
    }
}

/******************************************************************************/
/// Set MVOP for H264  Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetH264HardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, 0x00);

    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);

    // SVD mode enable
    HAL_WriteByteMask(VOP_INPUT_SWITCH1, BIT3, BIT2|BIT3);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Only for Monaco: Disable deciding bot by top address + 2
    HAL_WriteRegBit(VOP_MIRROR_CFG, 0, VOP_FIELD_FROM_ADDR);

    // Enable mfdec setting from wb from Manhattan
    HAL_WriteRegBit(VOP_REG_MFDEC_2_L, 1, VOP_MF_FROM_WB);
    HAL_WriteRegBit(VOP_REG_DUMMY, 1, VOP_32x32_WB); //32x32 from wb

    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for RM  Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetRMHardwireMode(void)
{
    HAL_MVOP_SetH264HardwireMode();
}

/******************************************************************************/
/// Set MVOP for JPEG Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetJpegHardwireMode(void)
{
    MS_U8 regval = 0x00;

    regval |= 0x80; // packmode
    regval |= 0x20; // Dram Rd Contd
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, regval);
    g_pHalMVOPCtx->bIs422 = 1;
    HAL_WriteRegBit(VOP_REG_DUMMY, 0, VOP_420_BW_SAVE);
    HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);

    HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0x00, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);

    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for EVD Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetEVDHardwireMode(MS_U16 u16ECOVersion)
{
    // Hardwire mode
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, 0x00);

    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);

    // SVD mode enable
    // EVD use HVD interface
    //HAL_WriteByteMask(VOP_INPUT_SWITCH1, BIT3, BIT2|BIT3);

    // EVD mode enable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // set evd flag for interlace mode setting
    g_pHalMVOPCtx->bIsH265 = 1;

    // Enable mfdec setting from wb
    HAL_WriteRegBit(VOP_REG_MFDEC_2_L, 1, VOP_MF_FROM_WB);

    // 10 bits from wb
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_H, 1, VOP_INFO_FROM_CODEC_10BIT);

    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for VP9 Hardwire Mode
/// vp9 hw change in Manhathan: tile mode 16x32
/******************************************************************************/
void HAL_MVOP_SetVP9HardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, 0x00);

    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);

    // EVD mode enable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Enable mfdec setting from wb
    HAL_WriteRegBit(VOP_REG_MFDEC_2_L, 1, VOP_MF_FROM_WB);

    // 10 bits from wb
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_H, 1, VOP_INFO_FROM_CODEC_10BIT);

    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for EVD MCU Mode
/******************************************************************************/
void HAL_MVOP_SetEVDFeature(MVOP_DevID eID, MVOP_EVDFeature* stEVDInput)
{
    switch(eID)
    {
        case E_MVOP_DEV_0:

            HAL_WriteRegBit(VOP_INPUT_SWITCH0, stEVDInput->bEnableEVD, EVD_ENABLE);// 32x32 enable

            switch(stEVDInput->eEVDBit[Y_INFO])
            {
                case E_MVOP_EVD_8BIT:
                    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_Y_EN);
                    break;
                case E_MVOP_EVD_10BIT:
                    HAL_WriteRegBit(VOP_EVD_10B_EN, 1, VOP_EVD_10B_Y_EN);
                    break;
                default:
                    break;
            }

            switch(stEVDInput->eEVDBit[UV_INFO])
            {
                case E_MVOP_EVD_8BIT:
                    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_UV_EN);
                    break;
                case E_MVOP_EVD_10BIT:
                    HAL_WriteRegBit(VOP_EVD_10B_EN, 1, VOP_EVD_10B_UV_EN);
                    break;
                default:
                    break;
            }

            //LSB BW Discard MASK
            if(stEVDInput->eEVDBit[Y_INFO] || stEVDInput->eEVDBit[UV_INFO])
            {
                HAL_WriteByteMask(VOP_REG_MASK, 0, VOP_LSB_REQ_MASK);
                HAL_WriteRegBit(VOP_UV_SHIFT, 0, VOP_GCLK_MIU_ON);
                MVOP_DBG(MVOP_PRINTF("[Debug] 10 bits display\n"););
            }
            else
            {
                HAL_WriteByteMask(VOP_REG_MASK, BIT1, VOP_LSB_REQ_MASK);
            }

            // Write trigger
            HAL_MVOP_LoadReg();
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB

            HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), stEVDInput->bEnableEVD, EVD_ENABLE);// 32x32 enable

            switch(stEVDInput->eEVDBit[Y_INFO])
            {
                case E_MVOP_EVD_8BIT:
                    HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 0, VOP_EVD_10B_Y_EN);
                    break;
                case E_MVOP_EVD_10BIT:
                    HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 1, VOP_EVD_10B_Y_EN);
                    break;
                default:
                    break;
            }

            switch(stEVDInput->eEVDBit[UV_INFO])
            {
                case E_MVOP_EVD_8BIT:
                    HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 0, VOP_EVD_10B_UV_EN);
                    break;
                case E_MVOP_EVD_10BIT:
                    HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 1, VOP_EVD_10B_UV_EN);
                    break;
                default:
                    break;
            }

            //LSB BW Discard MASK
            if(stEVDInput->eEVDBit[Y_INFO] || stEVDInput->eEVDBit[UV_INFO])
            {
                HAL_WriteByteMask(SUB_REG(VOP_REG_MASK), 0, VOP_LSB_REQ_MASK);
                HAL_WriteRegBit(SUB_REG(VOP_UV_SHIFT), 0, VOP_GCLK_MIU_ON);
                MVOP_DBG(MVOP_PRINTF("[Debug] 10 bits display\n"););
            }
            else
            {
                HAL_WriteByteMask(SUB_REG(VOP_REG_MASK), BIT1, VOP_LSB_REQ_MASK);
            }
            // Write trigger
            HAL_MVOP_SubLoadReg();
#endif
            break;
            default:
                break;
    }

}


///Enable 3D L/R dual buffer mode
MS_BOOL HAL_MVOP_Enable3DLR(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    HAL_WriteRegBit(VOP_MULTI_WIN_CFG0, bEnable, VOP_LR_BUF_MODE);
    g_pHalMVOPCtx->b3DLRMode = bEnable;
    if(bEnable)
    {
        //only for monaco: do not wait for data ready.
        HAL_WriteByteMask(VOP_NOT_WAIT_READ_DATA, 2, VOP_NOT_WAIT_RDLAT);
        HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
    }
    else
    {
        HAL_WriteByteMask(VOP_NOT_WAIT_READ_DATA, 0, VOP_NOT_WAIT_RDLAT);
    }
    return TRUE;
}

///Get if 3D L/R mode is enabled
MS_BOOL HAL_MVOP_Get3DLRMode(void)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    return g_pHalMVOPCtx->b3DLRMode;
}

MS_BOOL HAL_MVOP_GetTimingInfoFromRegisters(MVOP_TimingInfo_FromRegisters *pMvopTimingInfo)
{
    if(NULL == pMvopTimingInfo)
    {
        MVOP_PRINTF("HAL_MVOP_GetTimingInfoFromRegisters():pMvopTimingInfo is NULL\n");
        return FALSE;
    }
    if(HAL_MVOP_GetEnableState() == FALSE)
    {
        MVOP_PRINTF("MVOP is not enabled!\n");
        pMvopTimingInfo->bEnabled = FALSE;
        return FALSE;
    }
    pMvopTimingInfo->bEnabled = TRUE;
    pMvopTimingInfo->u16H_TotalCount = (HAL_ReadByte((VOP_FRAME_HCOUNT + 1))<< 8) | (HAL_ReadByte((VOP_FRAME_HCOUNT)));
    pMvopTimingInfo->u16V_TotalCount = (HAL_ReadByte((VOP_FRAME_VCOUNT + 1))<< 8) | (HAL_ReadByte((VOP_FRAME_VCOUNT)));
    pMvopTimingInfo->u16VBlank0_Start = (HAL_ReadByte((VOP_VB0_STR + 1))<< 8) | (HAL_ReadByte((VOP_VB0_STR)));
    pMvopTimingInfo->u16VBlank0_End = (HAL_ReadByte((VOP_VB0_END + 1))<< 8) | (HAL_ReadByte((VOP_VB0_END)));
    pMvopTimingInfo->u16VBlank1_Start = (HAL_ReadByte((VOP_VB1_STR + 1))<< 8) | (HAL_ReadByte((VOP_VB1_STR)));
    pMvopTimingInfo->u16VBlank1_End = (HAL_ReadByte((VOP_VB1_END + 1))<< 8) | (HAL_ReadByte((VOP_VB1_END)));
    pMvopTimingInfo->u16TopField_Start = (HAL_ReadByte((VOP_TF_STR + 1))<< 8) | (HAL_ReadByte((VOP_TF_STR)));
    pMvopTimingInfo->u16BottomField_Start = (HAL_ReadByte((VOP_BF_STR + 1))<< 8) | (HAL_ReadByte((VOP_BF_STR)));
    pMvopTimingInfo->u16HActive_Start = (HAL_ReadByte((VOP_HACT_STR + 1))<< 8) | (HAL_ReadByte((VOP_HACT_STR)));
    pMvopTimingInfo->u16TopField_VS = (HAL_ReadByte((VOP_TF_VS + 1))<< 8) | (HAL_ReadByte((VOP_TF_VS)));
    pMvopTimingInfo->u16BottomField_VS = (HAL_ReadByte((VOP_BF_VS + 1))<< 8) | (HAL_ReadByte((VOP_BF_VS)));
    pMvopTimingInfo->bInterlace = (HAL_ReadRegBit(VOP_CTRL0, BIT7) == BIT7);
    return TRUE;
}

void HAL_MVOP_SetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
    MS_PHY u64tmp = 0;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    // Y offset
    u64tmp = u32YOffset >> 3;
    if ( !bProgressive )
    {   //Refine Y offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(VOP_JPG_YSTR0_L, u64tmp & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+2), (u64tmp >> 16) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if (!bProgressive )
    {   //Y offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_YSTR1_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+2), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    if (b422pack)
    {
        u32UVOffset = u32YOffset + 16; //add 16 for 128bit; add 8 for 64bit
    }
    // UV offset
    u64tmp = u32UVOffset >> 3;
    if( !bProgressive )
    {  //Refine UV offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(VOP_JPG_UVSTR0_L, u64tmp & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+2), (u64tmp >> 16) & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if( !bProgressive )
    {  //UV offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_UVSTR1_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+2), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    return;
}

MS_BOOL HAL_MVOP_SetYUVBaseAddMultiView(MVOP_BaseAddInput *stBaseAddInfo)
{
    MS_PHY u64tmpY = 0;
    MS_PHY u64tmpUV = 0;

    if (stBaseAddInfo == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s stBaseAddInfo is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    // Y offset
    u64tmpY = (stBaseAddInfo->u32YOffset) >> 3;
    // UV offset
    u64tmpUV = (stBaseAddInfo->u32UVOffset) >> 3;

    switch(stBaseAddInfo->eView)
    {
    case E_MVOP_MAIN_VIEW:
        // Y offset
        HAL_WriteByte(VOP_JPG_YSTR0_L, u64tmpY & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+1), (u64tmpY >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+2), (u64tmpY >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+3), (u64tmpY >> 24) & VOP_YUV_STR_HIBITS);

        // UV offset
        HAL_WriteByte(VOP_JPG_UVSTR0_L, u64tmpUV & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+1), (u64tmpUV >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+2), (u64tmpUV >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+3), (u64tmpUV >> 24) & VOP_YUV_STR_HIBITS);
        break;
    case E_MVOP_2ND_VIEW:
        // Y offset
        HAL_WriteByte(VOP_JPG_YSTR1_L, u64tmpY & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+1), (u64tmpY >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+2), (u64tmpY >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+3), (u64tmpY >> 24) & VOP_YUV_STR_HIBITS);

        //UV offset
        HAL_WriteByte(VOP_JPG_UVSTR1_L, u64tmpUV & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+1), (u64tmpUV >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+2), (u64tmpUV >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+3), (u64tmpUV >> 24) & VOP_YUV_STR_HIBITS);
        break;
    default:
        break;
    }
    return TRUE;
}

MS_PHY HAL_MVOP_GetYBaseAdd(void)
{
    MS_PHY u64YOffset = 0;
    u64YOffset = HAL_ReadByte(VOP_JPG_YSTR0_L)&0xff;
    u64YOffset |=((HAL_ReadByte((VOP_JPG_YSTR0_L+1))<<8)&0xff00);
    u64YOffset |=((HAL_ReadByte((VOP_JPG_YSTR0_L+2))<<16)&0xff0000);
    u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u64YOffset;
}

MS_PHY HAL_MVOP_GetUVBaseAdd(void)
{
    MS_PHY u64UVOffset = 0;
    u64UVOffset = HAL_ReadByte(VOP_JPG_UVSTR0_L)&0xff;
    u64UVOffset |=((HAL_ReadByte((VOP_JPG_UVSTR0_L+1))<<8)&0xff00);
    u64UVOffset |=((HAL_ReadByte((VOP_JPG_UVSTR0_L+2))<<16)&0xff0000);
    u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u64UVOffset;
}

MS_PHY HAL_MVOP_GetYBaseAddMultiView(MVOP_3DView eView)
{
    MS_PHY u64YOffset = 0;
    switch(eView)
    {
        case E_MVOP_MAIN_VIEW:
            u64YOffset = HAL_ReadByte(VOP_JPG_YSTR0_L)&0xff;
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+1))<<8)&0xff00);
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+2))<<16)&0xff0000);
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
            break;
        case E_MVOP_2ND_VIEW:
            u64YOffset = HAL_ReadByte(VOP_JPG_YSTR1_L)&0xff;
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+1))<<8)&0xff00);
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+2))<<16)&0xff0000);
            u64YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+3))<<24)&0x7000000);
            break;
        default:
            u64YOffset = 0;
            break;
    }
    return u64YOffset;
}

MS_PHY HAL_MVOP_GetUVBaseAddMultiView(MVOP_3DView eView)
{
    MS_PHY u64UVOffset = 0;
    switch(eView)
    {
        case E_MVOP_MAIN_VIEW:
            u64UVOffset = HAL_ReadByte(VOP_JPG_UVSTR0_L)&0xff;
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+1))<<8)&0xff00);
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+2))<<16)&0xff0000);
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
            break;
        case E_MVOP_2ND_VIEW:
            u64UVOffset = HAL_ReadByte(VOP_JPG_UVSTR1_L)&0xff;
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+1))<<8)&0xff00);
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+2))<<16)&0xff0000);
            u64UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+3))<<24)&0x7000000);
            break;
        default:
            u64UVOffset = 0;
            break;
    }
    return u64UVOffset;
}

MS_BOOL HAL_MVOP_Set3DLRAltOutput_VHalfScaling(MS_BOOL bEnable)
{
    MS_BOOL bEnDualBuff = bEnable ? ENABLE : DISABLE;     //enable dual buffer
    MS_BOOL bEnSWDualBuff = bEnable ? DISABLE : ENABLE;   //buffer controlled by HK instead of FW
    MS_BOOL bEnMirrMaskBase = bEnable ? DISABLE : ENABLE; //do not mask LSB
    MS_BOOL bEnHwFldBase = bEnable ? DISABLE : ENABLE;    //hardware calculate field jump base address

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    //Set 0x27[2] = 1 (enable SW dual buffer mode)
    HAL_WriteRegBit(VOP_REG_WR, bEnDualBuff, VOP_BUF_DUAL);

    //Set 0x38[8] = 0 (use SW dual buffer mode)
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_H, bEnSWDualBuff, VOP_INFO_FROM_CODEC_DUAL_BUFF);

    //Set 0x3b[7] = 0 (use MVD/HVD firmware send base)
    HAL_WriteRegBit(VOP_MIRROR_CFG, bEnMirrMaskBase, VOP_MASK_BASE_LSB);

    //Set 0x3b[5] = 0 (hardware calculate field jump base address)
    HAL_WriteRegBit(VOP_MIRROR_CFG, bEnHwFldBase, VOP_HW_FLD_BASE);

    g_pHalMVOPCtx->b3DLRAltOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_Set3DLRAltOutput(MS_BOOL bEnable)
{
    //Set 0x3C[2] = 1 (enable 3D L/R dual buffer line alternative output)
    //it works when 0x3C[0] = 1
    HAL_WriteRegBit(VOP_MULTI_WIN_CFG0, bEnable, VOP_LR_LA_OUT);
    // bw saving not support: LA/SBS
    HAL_WriteRegBit(VOP_REG_DUMMY, 0, VOP_420_BW_SAVE);
    HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
    HAL_MVOP_LoadReg();

    g_pHalMVOPCtx->b3DLRAltOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_Set3DLRAltSBSOutput(MS_BOOL bEnable)
{
    //it works when 0x3C[0] = 1 and 0x3C[2] = 1
    //Set 0x3C[3] = 1 (3D L/R line alternative read, side-by-side output)
    HAL_WriteRegBit(VOP_MULTI_WIN_CFG0, bEnable, VOP_LR_LA2SBS_OUT);
    g_pHalMVOPCtx->b3DLRAltSBSOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_Get3DLRAltOutput(void)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    return g_pHalMVOPCtx->b3DLRAltOutput;
}

MS_BOOL HAL_MVOP_Get3DLRAltSBSOutput(void)
{
    return g_pHalMVOPCtx->b3DLRAltSBSOutput;
}

EN_MVOP_Output_3D_TYPE HAL_MVOP_GetOutput3DType(void)
{
    EN_MVOP_Output_3D_TYPE en3DType = E_MVOP_OUTPUT_3D_NONE;
    if(g_pHalMVOPCtx->b3DLRMode)
    {
        if(g_pHalMVOPCtx->b3DLRAltSBSOutput)
        {
            en3DType = E_MVOP_OUTPUT_3D_SBS;
        }
        else
        {
            en3DType = E_MVOP_OUTPUT_3D_TB;
        }
    }
    else if(g_pHalMVOPCtx->b3DLRAltOutput)
    {
        en3DType = E_MVOP_OUTPUT_3D_LA;
    }
    return en3DType;
}

MS_BOOL HAL_MVOP_Set3DLR2ndCfg(MS_BOOL bEnable)
{
    //Set 0x3c[7] as 1 to enable
    HAL_WriteRegBit(VOP_MULTI_WIN_CFG0, bEnable, VOP_LR_DIFF_SIZE);
    return TRUE;
}

MS_BOOL HAL_MVOP_Get3DLR2ndCfg(void)
{
    MS_BOOL bEnable = FALSE;
    if (VOP_LR_DIFF_SIZE == (VOP_LR_DIFF_SIZE & HAL_ReadRegBit(VOP_MULTI_WIN_CFG0, VOP_LR_DIFF_SIZE)))
    {
        bEnable = TRUE;
    }
    return bEnable;
}

MVOP_DrvMirror HAL_MVOP_GetMirrorMode(MVOP_DevID eID)
{
    MVOP_DrvMirror enMirror = E_VOPMIRROR_NONE;
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    switch(eID)
    {
        case E_MVOP_DEV_0:
            if(g_pHalMVOPCtx->bMirrorModeVer && g_pHalMVOPCtx->bMirrorModeHor)
            {
                enMirror = E_VOPMIRROR_HVBOTH;
            }
            else if(g_pHalMVOPCtx->bMirrorModeHor)
            {
                enMirror = E_VOPMIRROR_HORIZONTALL;
            }
            else if(g_pHalMVOPCtx->bMirrorModeVer)
            {
                enMirror = E_VOPMIRROR_VERTICAL;
            }
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            if(g_pHalMVOPCtx->bSubMirrorModeVer &&g_pHalMVOPCtx-> bSubMirrorModeHor)
            {
                enMirror = E_VOPMIRROR_HVBOTH;
            }
            else if(g_pHalMVOPCtx->bSubMirrorModeHor)
            {
                enMirror = E_VOPMIRROR_HORIZONTALL;
            }
            else if(g_pHalMVOPCtx->bSubMirrorModeVer)
            {
                enMirror = E_VOPMIRROR_VERTICAL;
            }
#endif
            break;
        default:
            break;
    }
    return enMirror;
}

MS_BOOL HAL_MVOP_SetVerDup(MS_BOOL bEnable)
{
    HAL_WriteRegBit(VOP_CTRL0, bEnable, BIT3);// V line duplicate
    return TRUE;
}

MS_BOOL HAL_MVOP_GetVerDup(void)
{
    return (HAL_ReadRegBit(VOP_CTRL0, BIT3) == BIT3);
}

MS_BOOL HAL_MVOP_SetVerx4Dup(MS_BOOL bEnable)
{
    HAL_WriteRegBit(VOP_CTRL0, bEnable, BIT3);// x4 duplicate should raise V line duplicate first
    HAL_WriteRegBit(VOP_RGB_FMT, bEnable, BIT3);// V line x4 duplicate
    return TRUE;
}

MS_BOOL HAL_MVOP_SetHorx4Dup(MS_BOOL bEnable)
{
    HAL_WriteRegBit(VOP_CTRL0, bEnable, BIT2);// x4 duplicate should raise H pixel duplicate first
    HAL_WriteRegBit(VOP_RGB_FMT, bEnable, BIT2);// H line x4 duplicate
    return TRUE;
}

MS_BOOL HAL_MVOP_GetVerx4Dup(void)
{
    return ((HAL_ReadRegBit(VOP_RGB_FMT, BIT3) & HAL_ReadRegBit(VOP_CTRL0, BIT3)) == BIT3);
}

MS_BOOL HAL_MVOP_GetHorx4Dup(void)
{
    return ((HAL_ReadRegBit(VOP_RGB_FMT, BIT2) & HAL_ReadRegBit(VOP_CTRL0, BIT2)) == BIT2);
}

MS_U16 HAL_MVOP_GetTopVStart(MVOP_DevID eID)
{
    MS_U16 u16TopVStart = 0;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            u16TopVStart = HAL_Read2Byte(VOP_IMG_VSTR0)&0x1fff;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            u16TopVStart = HAL_Read2Byte(SUB_REG(VOP_IMG_VSTR0))&0x1fff;
#endif
            break;
        default:
            break;
    }
    return u16TopVStart;
}

MS_U16 HAL_MVOP_GetBottomVStart(MVOP_DevID eID)
{
    MS_U16 u16BotVStart = 0;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            u16BotVStart = HAL_Read2Byte(VOP_IMG_VSTR1)&0x1fff;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            u16BotVStart = HAL_Read2Byte(SUB_REG(VOP_IMG_VSTR1))&0x1fff;
#endif
            break;
        default:
            break;
    }
    return u16BotVStart;
}

MS_U16 HAL_MVOP_GetVCount(MVOP_DevID eID)
{
    MS_U16 u16VCount = 0;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            u16VCount = HAL_Read2Byte(VOP_DEBUG_2A)&0x1fff;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            u16VCount = HAL_Read2Byte(SUB_REG(VOP_DEBUG_2A))&0x1fff;
#endif
            break;
        default:
            break;
    }
    return u16VCount;
}

MS_BOOL HAL_MVOP_SetVC1RangeMap(MVOP_DevID eID, MVOP_VC1RangeMapInfo *stVC1RangeMapInfo)
{
    MS_U32 u8Luma = 0;
    MS_U32 u8Chroma = 0;

    if (stVC1RangeMapInfo == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s stBaseAddInfo is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }

    // Luma value
    u8Luma = stVC1RangeMapInfo->u8LumaValue;
    // Chroma value
    u8Chroma = stVC1RangeMapInfo->u8ChromaValue;

    switch(eID)
    {
        case E_MVOP_DEV_0:
            //set VC1 Luma value
            if(stVC1RangeMapInfo->bIsEnableLuma)
            {
                HAL_WriteRegBit(VOP_RAMAP_LUMA, 1, BIT7);
                HAL_WriteByteMask(VOP_RAMAP_LUMA, u8Luma, VOP_RAMAP_LUMA_VAL);
            }
            else //disable
            {
                HAL_WriteRegBit(VOP_RAMAP_LUMA, 0, BIT7);
            }

            //set VC1 Chroma value
            if(stVC1RangeMapInfo->bIsEnableChroma)
            {
                HAL_WriteRegBit(VOP_RAMAP_CHROMA, 1, BIT7);
                HAL_WriteByteMask(VOP_RAMAP_CHROMA, u8Chroma, VOP_RAMAP_CHROMA_VAL);
            }
            else
            {
                HAL_WriteRegBit(VOP_RAMAP_CHROMA, 0, BIT7);
            }
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            //set VC1 Luma value
            if(stVC1RangeMapInfo->bIsEnableLuma)
            {
                HAL_WriteRegBit(SUB_REG(VOP_RAMAP_LUMA), 1, BIT7);
                HAL_WriteByteMask(SUB_REG(VOP_RAMAP_LUMA), u8Luma, VOP_RAMAP_LUMA_VAL);
            }
            else //disable
            {
                HAL_WriteRegBit(SUB_REG(VOP_RAMAP_LUMA), 0, BIT7);
            }

            //set VC1 Chroma value
            if(stVC1RangeMapInfo->bIsEnableChroma)
            {
                HAL_WriteRegBit(SUB_REG(VOP_RAMAP_CHROMA), 1, BIT7);
                HAL_WriteByteMask(SUB_REG(VOP_RAMAP_CHROMA), u8Chroma, VOP_RAMAP_CHROMA_VAL);
            }
            else
            {
                HAL_WriteRegBit(SUB_REG(VOP_RAMAP_CHROMA), 0, BIT7);
            }
#endif
            break;
        default:
                break;
    }
    return TRUE;
}

MS_U16 g_u16SetStartX = 0;
MS_U16 g_u16SetStartY = 0;
MS_BOOL g_bIsY4Align = 0;

void HAL_MVOP_SetStartX(MVOP_DevID eID, MS_U16 u16XPos)
{
    switch(eID)
    {
    case E_MVOP_DEV_0:
#if 0 //tv setting
        u16XPos = ALIGN_UPTO_2(u16XPos);
        HAL_WriteByte(VOP_REG_CROP_HSTART, u16XPos & 0xff);
        HAL_WriteByte((VOP_REG_CROP_HSTART + 1),((u16XPos) >> (8)) & (0x1f));
        if(0 == u16XPos)
        {
            g_pHalMVOPCtx->u16CropXStart = 0;
        }
        else
        {
            g_pHalMVOPCtx->u16CropXStart += u16XPos;
        }
#endif
        if(g_pHalMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
        u16XPos = ALIGN_UPTO_2(u16XPos);
        g_pHalMVOPCtx->u16CropXStart = u16XPos;
        HAL_WriteByte(VOP_REG_CROP_HSTART, u16XPos);
        HAL_WriteByteMask((VOP_REG_CROP_HSTART + 1),((u16XPos) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_LoadReg();
        break;
    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
#if 0 //tv setting
        u16XPos = ALIGN_UPTO_2(u16XPos);
        HAL_WriteByte(SUB_REG(VOP_REG_CROP_HSTART), u16XPos & 0xff);
        HAL_WriteByte(SUB_REG((VOP_REG_CROP_HSTART + 1)),((u16XPos) >> (8)) & (0x1f));
        if(0 == u16XPos)
        {
            g_pHalMVOPCtx->u16SubCropXStart = 0;
        }
        else
        {
            g_pHalMVOPCtx->u16SubCropXStart += u16XPos;
        }
#endif
        u16XPos = ALIGN_UPTO_2(u16XPos);
        g_pHalMVOPCtx->u16SubCropXStart = u16XPos;
        HAL_WriteByte(SUB_REG(VOP_REG_CROP_HSTART), u16XPos);
        HAL_WriteByteMask(SUB_REG(VOP_REG_CROP_HSTART + 1),((u16XPos) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_SubLoadReg();
#endif
        break;
        default:
                break;
    }
}


void HAL_MVOP_SetStartY(MVOP_DevID eID, MS_U16 u16YPos, MS_BOOL bIsInterlace)
{
    switch(eID)
    {
    case E_MVOP_DEV_0:
#if 0
        HAL_WriteByteMask(VOP_REG_CROP_VSTART, u16YPos & 0xff, 0xff);
        HAL_WriteByteMask((VOP_REG_CROP_VSTART + 1), ((u16YPos) >> (8)) & (0x1f), 0x1f);
        if(0 == u16YPos)
        {
            g_pHalMVOPCtx->u16CropYStart = 0;
        }
        else
        {
            g_pHalMVOPCtx->u16CropYStart += u16YPos;
        }
#endif
        if(g_pHalMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
        g_pHalMVOPCtx->u16CropYStart = u16YPos;
        HAL_WriteByte(VOP_REG_CROP_VSTART, u16YPos);
        HAL_WriteByteMask((VOP_REG_CROP_VSTART + 1), ((u16YPos) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_LoadReg();
        break;
    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
#if 0
        HAL_WriteByteMask(SUB_REG(VOP_REG_CROP_VSTART), u16YPos & 0xff, 0xff);
        HAL_WriteByteMask(SUB_REG((VOP_REG_CROP_VSTART + 1)), ((u16YPos) >> (8)) & (0x1f), 0x1f);
        if(0 == u16YPos)
        {
            g_pHalMVOPCtx->u16SubCropYStart = 0;
        }
        else
        {
            g_pHalMVOPCtx->u16SubCropYStart += u16YPos;
        }
#endif
        if(g_pHalMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
        g_pHalMVOPCtx->u16SubCropYStart = u16YPos;
        HAL_WriteByte(SUB_REG(VOP_REG_CROP_VSTART), u16YPos);
        HAL_WriteByteMask(SUB_REG(VOP_REG_CROP_VSTART + 1), ((u16YPos) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_SubLoadReg();
#endif
        break;
        default:
                break;
    }
}

//-----------------------------------------
//STB chip set crop size is from image (0,0) ,
//including startx,y
//Kano hw crop before mirror
//-----------------------------------------
void HAL_MVOP_SetPicWidthMinus(MVOP_DevID eID, MS_U16 u16XSizes, MS_U16 u16Width)
{
    switch(eID)
    {
        if(g_pHalMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    case E_MVOP_DEV_0:
        if(0 == u16XSizes)
        {
            g_pHalMVOPCtx->u16CropXSize = 0;
        }
        else
        {
            u16XSizes = u16XSizes + 1 - g_pHalMVOPCtx->u16CropXStart ;
            u16XSizes = ALIGN_UPTO_2(u16XSizes);
            g_pHalMVOPCtx->u16CropXSize = u16XSizes;
        }
        HAL_WriteByte(VOP_REG_CROP_HSIZE, u16XSizes);
        HAL_WriteByteMask((VOP_REG_CROP_HSIZE + 1), ((u16XSizes) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_LoadReg();
        break;
    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB

        if(0 == u16XSizes)
        {
            g_pHalMVOPCtx->u16SubCropXSize = 0;
        }
        else
        {
            u16XSizes = u16XSizes + 1 - g_pHalMVOPCtx->u16SubCropXStart ;
            u16XSizes = ALIGN_UPTO_2(u16XSizes);
            g_pHalMVOPCtx->u16SubCropXSize = u16XSizes;
        }
        HAL_WriteByte(SUB_REG(VOP_REG_CROP_HSIZE), u16XSizes);
        HAL_WriteByteMask(SUB_REG(VOP_REG_CROP_HSIZE + 1), ((u16XSizes) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_SubLoadReg();
#endif
            break;
        default:
                break;
    }
}

void HAL_MVOP_SetPicHeightMinus(MVOP_DevID eID, MS_U16 u16YSizes, MS_U16 u16Height)
{
    switch(eID)
    {
        if(g_pHalMVOPCtx == NULL)
        {
            MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
            return;
        }
    case E_MVOP_DEV_0:
        if(0 == u16YSizes)
        {
            g_pHalMVOPCtx->u16CropYSize = 0;
        }
        else
        {
            u16YSizes = u16YSizes + 1 - g_pHalMVOPCtx->u16CropYStart ;
            g_pHalMVOPCtx->u16CropYSize = u16YSizes;
        }
        HAL_WriteByte(VOP_REG_CROP_VSIZE, u16YSizes);
        HAL_WriteByteMask((VOP_REG_CROP_VSIZE + 1), ((u16YSizes) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_LoadReg();
        break;
    case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
        if(0 == u16YSizes)
        {
            g_pHalMVOPCtx->u16CropYSize = 0;
        }
        else
        {
            u16YSizes = u16YSizes + 1 - g_pHalMVOPCtx->u16CropYStart ;
            g_pHalMVOPCtx->u16CropYSize = u16YSizes;
        }
        HAL_WriteByte(SUB_REG(VOP_REG_CROP_VSIZE), u16YSizes);
        HAL_WriteByteMask(SUB_REG(VOP_REG_CROP_VSIZE + 1), ((u16YSizes) >> (8)) & (0x1f), 0x1f);
        // Write trigger
        HAL_MVOP_SubLoadReg();

#endif
            break;
        default:
                break;
    }
}


/******************************************************************************/
/// Set MVOP Saving BW Mode
/// @ Napoli this command should be set after MDrv_MVOP_SetOutputCfg
/******************************************************************************/
MS_BOOL HAL_MVOP_Set420BWSaveMode(MS_BOOL bEnable)
{
    MS_BOOL bValue = FALSE;

    //hw limtation: 3DLA/3DSBS/422/p mode in, i mode out/i mode in, p mode out(only need to check in MCU mode)
    bValue = (g_pHalMVOPCtx->b3DLRAltSBSOutput || g_pHalMVOPCtx->b3DLRAltOutput /*|| g_pHalMVOPCtx->b3DLRMode*/ || g_pHalMVOPCtx->bIs422 );

    if(bValue)
    {
        MVOP_DBG(MVOP_PRINTF("%s Hit the limitation of saving bw, disable BW Saving mode\n", __FUNCTION__);)
        HAL_WriteRegBit(VOP_REG_DUMMY, 0, VOP_420_BW_SAVE);
        HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
        HAL_MVOP_LoadReg();
        return FALSE;
    }
    else
    {
        HAL_WriteRegBit(VOP_REG_DUMMY, bEnable, VOP_420_BW_SAVE);
        if( g_pHalMVOPCtx->b3DLRMode == FALSE)
            HAL_WriteRegBit(VOP_REG_BW_SAVE, bEnable, VOP_420_BW_SAVE_EX);
        else
            HAL_WriteRegBit(VOP_REG_BW_SAVE, 0, VOP_420_BW_SAVE_EX);
        HAL_MVOP_LoadReg();
        return TRUE;
    }
}

MS_BOOL HAL_MVOP_SetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo)
{
    //----------------------------------------------------
    // Set MSB YUV Address
    //----------------------------------------------------

    MS_PHY u64tmp = 0;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    // Y offset
    u64tmp = stEVDBaseAddInfo->u32MSBYOffset >> 3;
    if ( !stEVDBaseAddInfo->bProgressive)
    {   //Refine Y offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(VOP_JPG_YSTR0_L, u64tmp & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_H), (u64tmp >> 16) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if (!stEVDBaseAddInfo->bProgressive )
    {   //Y offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_YSTR1_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    if (stEVDBaseAddInfo->b422Pack)
    {
        stEVDBaseAddInfo->u32MSBUVOffset = stEVDBaseAddInfo->u32MSBYOffset + 16; //add 16 for 128bit; add 8 for 64bit
    }
    // UV offset
    u64tmp = stEVDBaseAddInfo->u32MSBUVOffset >> 3;
    if( !stEVDBaseAddInfo->bProgressive )
    {  //Refine UV offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
        HAL_WriteByte(VOP_JPG_UVSTR0_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if( !stEVDBaseAddInfo->bProgressive )
    {  //UV offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_UVSTR1_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    //----------------------------------------------------
    // Set MSB YUV Address
    //----------------------------------------------------
    if(stEVDBaseAddInfo->bEnLSB)
    {
        //Enable LSB
        HAL_WriteRegBit(VOP_EVD_10B_EN, 1, VOP_EVD_10B_Y_EN);
        HAL_WriteRegBit(VOP_EVD_10B_EN, 1, VOP_EVD_10B_UV_EN);
        HAL_WriteRegBit(VOP_REG_MASK, 0, VOP_LSB_REQ_MASK);

        // Y offset
        u64tmp = stEVDBaseAddInfo->u32LSBYOffset >> 3;
        if ( !stEVDBaseAddInfo->bProgressive)
        {   //Refine Y offset for interlace repeat bottom field
            if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
            {
                MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
                u64tmp += 2;
            }
            else
            {
                MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            }
        }
        HAL_WriteByte(VOP_LSB_YSTR0_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_LSB_YSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_LSB_YSTR0_L), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_LSB_YSTR0_L+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

        if (!stEVDBaseAddInfo->bProgressive )
        {   //Y offset of bottom field if interlace
            HAL_WriteByte(VOP_LSB_YSTR1_L, u64tmp & 0xff);
            HAL_WriteByte((VOP_LSB_YSTR1_L+1), (u64tmp >> 8) & 0xff);
            HAL_WriteByte((VOP_LSB_YSTR1_H), (u64tmp >> 16) & 0xff);
            HAL_WriteByte((VOP_LSB_YSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
        }

        if (stEVDBaseAddInfo->b422Pack)
        {
            stEVDBaseAddInfo->u32LSBUVOffset = stEVDBaseAddInfo->u32LSBYOffset + 16; //add 16 for 128bit; add 8 for 64bit
        }
        // UV offset
        u64tmp = stEVDBaseAddInfo->u32LSBUVOffset >> 3;
        if( !stEVDBaseAddInfo->bProgressive )
        {  //Refine UV offset for interlace repeat bottom field
            if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
            {
                MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
                u64tmp += 2;
            }
            else
            {
                MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            }
        }
        HAL_WriteByte(VOP_LSB_UVSTR0_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR0_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

        if( !stEVDBaseAddInfo->bProgressive )
        {  //UV offset of bottom field if interlace
        HAL_WriteByte(VOP_LSB_UVSTR1_L, u64tmp & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_LSB_UVSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
        }
    }

    return TRUE;
}

/******************************************************************************/
/// Set MVOP repeat previous frame IF VDEC can not finish vsync.
/// this command should be set disable as call VDEC Exit.
/******************************************************************************/
void HAL_MVOP_SetRptPreVsyncFrame(MVOP_DevID eID, MS_BOOL bEnable)
{
    switch(eID)
    {
        case E_MVOP_DEV_0:
        {
            g_pHalMVOPCtx->bRptPreVsync = bEnable;
            HAL_WriteRegBit(VOP_MIRROR_CFG_HI, bEnable, BIT3);
            break;
        }
        case E_MVOP_DEV_1:
        {
#if MVOP_SUPPORT_SUB
            g_pHalMVOPCtx->bSubRptPreVsync = bEnable;
            HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG_HI), bEnable, BIT3);
#endif
            break;
        }
        default:
            MVOP_PRINTF("[Warning] MVOP_DevID not support!\n");
            break;
    }
}

void HAL_MVOP_PowerStateSuspend(void)
{
    MS_BOOL bFirstDrvInstant = FALSE;
    MVOP_HalInitCtxResults eRet = _HAL_MVOP_InitContext(&bFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("[%s] MVOP Context Init failed!\n",__FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->bIsInit = 0;
#if MVOP_SUPPORT_SUB
    g_pHalMVOPCtx->bSubIsInit = 0;
#endif
}

MVOP_HSMode HAL_MVOP_GetHandShakeMode(MVOP_DevID eID)
{
    MVOP_HSMode eRet = E_MVOP_HS_NOT_SUPPORT;

    switch(eID)
    {
        case E_MVOP_DEV_0:
            if(HAL_ReadRegBit(VOP_HANDSHAKE, BIT0))
            {
                eRet = E_MVOP_HS_ENABLE;
            }
            else
            {
                eRet = E_MVOP_HS_DISABLE;
            }
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            if(HAL_ReadRegBit(SUB_REG(VOP_HANDSHAKE), BIT0))
            {
                eRet = E_MVOP_HS_ENABLE;
            }
            else
            {
                eRet = E_MVOP_HS_DISABLE;
            }
#endif
            break;
        default:
            eRet = E_MVOP_HS_INVALID_PARAM;
            break;
    }
    return eRet;
}

MS_BOOL HAL_MVOP_CheckSTCCW(void)
{
    MS_U16 u16STC_CW_L = 0;
    MS_U16 u16STC_CW_H = 0;
    MS_BOOL u16STC_CW_SEL = 0;
    MS_BOOL u16TSP_CLK_EN = 0;

    u16STC_CW_L = HAL_Read2Byte(REG_STC0_CW_L)&0xffff;
    u16STC_CW_H = HAL_Read2Byte(REG_STC0_CW_H)&0xffff;

    u16STC_CW_SEL = (HAL_ReadRegBit(REG_STC_CW_SLE_L, BIT1) == BIT1);
    u16TSP_CLK_EN = !(HAL_ReadRegBit(REG_TSP_CLK, BIT0) == BIT0);

    if((((u16STC_CW_L || u16STC_CW_H) == 0) && (u16STC_CW_SEL == 0)) || ((u16STC_CW_SEL == 1) && (u16TSP_CLK_EN == 0)))
        return FALSE;
    else
        return TRUE;

}

#define MIU1_REG_BASE       0x0600
MS_BOOL HAL_MVOP_GetIsMiuIPControl(HALMVOPMIUCLIENTINFO stInfo)
{
    MS_U32 u32RegMiu = 0;
    MS_U16 u16Mask = 0;
    MS_BOOL bRet = FALSE;

    u32RegMiu = MIU1_REG_BASE + (0xF0+(stInfo.u8Gp * 2));
    if(stInfo.u8BitPos < 8)
    {
        u16Mask = 1<<stInfo.u8BitPos;
    }
    else
    {
        u16Mask = 1<<(stInfo.u8BitPos-8);
        u32RegMiu += 1;
    }
    MVOP_DBG(MVOP_PRINTF("[%s] u32RegMiu = 0x%lx, u16Mask = 0x%x\n",__FUNCTION__, u32RegMiu,u16Mask);)
    if(HAL_ReadRegBit(u32RegMiu, u16Mask))
    {
        bRet = TRUE;
    }

    return bRet;
}

void HAL_MVOP_SelMIU(MVOP_DevID eDevID, HALMVOPMIUSEL eMiuMSB0, HALMVOPMIUSEL eMiuMSB1, HALMVOPMIUSEL eMiuLSB0, HALMVOPMIUSEL eMiuLSB1)
{
    MS_U8 u8MSBVlue = 0;
    MS_U8 u8LSBVlue = 0;

    if(eMiuMSB0 != eMiuMSB1)
        HAL_WriteRegBit(VOP_MIU_SEL, TRUE, BIT0);
    else
        HAL_WriteRegBit(VOP_MIU_SEL, FALSE, BIT0);

    if(eMiuLSB0 != eMiuLSB1)
        HAL_WriteRegBit(VOP_MIU_SEL, TRUE, BIT1);
    else
        HAL_WriteRegBit(VOP_MIU_SEL, FALSE, BIT1);

    u8MSBVlue |= (eMiuMSB0 << 4);
    u8MSBVlue |= (eMiuMSB1 << 6);

    u8LSBVlue |= (eMiuLSB0 << 4);
    u8LSBVlue |= (eMiuLSB1 << 6);

    switch(eDevID)
    {
        case E_MVOP_DEV_0:
        {
            // MSB
            HAL_WriteByteMask(VOP_MIU_SEL, u8MSBVlue, VOP_MSB_BUF0_MIU_SEL | VOP_MSB_BUF1_MIU_SEL);
            // LSB
            HAL_WriteByteMask(VOP_MIU_SEL_LSB, u8LSBVlue, VOP_LSB_BUF0_MIU_SEL | VOP_LSB_BUF1_MIU_SEL);
            break;
        }
#if MVOP_SUPPORT_SUB
        case E_MVOP_DEV_1:
        {
            // MSB
            HAL_WriteByteMask(SUB_REG(VOP_MIU_SEL), u8MSBVlue, VOP_MSB_BUF0_MIU_SEL | VOP_MSB_BUF1_MIU_SEL);
            // LSB
            HAL_WriteByteMask(SUB_REG(VOP_MIU_SEL_LSB), u8LSBVlue, VOP_LSB_BUF0_MIU_SEL | VOP_LSB_BUF1_MIU_SEL);
            break;
        }
#endif
        default:
            break;
    }
}

MS_BOOL HAL_MVOP_GetIsOnlyMiuIPControl(void)
{
    return TRUE;
}

HALMVOPFREQUENCY HAL_MVOP_SetDefaultClk(MVOP_DevID eDevID)
{
    HALMVOPFREQUENCY eRet = HALMVOP_160MHZ;
    switch(eDevID)
    {
            case E_MVOP_DEV_0:
            {
                if(g_pHalMVOPCtx->bIsXcTrig == FALSE)
                {
                    MVOP_PRINTF("[%s] HALMVOP_SYNCMODE\n",__FUNCTION__);
                    eRet = HALMVOP_SYNCMODE;
                }
                break;
            }
#if MVOP_SUPPORT_SUB
            case E_MVOP_DEV_1:
            {
                if(g_pHalMVOPCtx->bSubIsXcTrig == FALSE)
                {
                    MVOP_PRINTF("[%s] HALMVOP_SYNCMODE\n",__FUNCTION__);
                    eRet = HALMVOP_SYNCMODE;
                }
                break;
            }
#endif
            default:
            break;
    }

        MVOP_PRINTF("[%s] clock = %d\n",__FUNCTION__,eRet);
        return eRet;
}

void HAL_MVOP_GetMaxFramerate(MVOP_DevID eDevID, MVOP_GetMaxFps* stStreamInfo)
{
    MS_U64 u64MaxClk = 0;
    MS_U16 u16HsizeTiming = 0;
    MS_U16 u16VsizeTiming = 0;

    if(NULL == stStreamInfo)
    {
        MVOP_PRINTF("[%s] Input parameter is NULL!\n",__FUNCTION__);
        return;
    }

    switch(eDevID)
    {
        case E_MVOP_DEV_0:
#if MVOP_SUPPORT_SUB
        case E_MVOP_DEV_1:
#endif
        {
            if(_HAL_MVOP_IsSupport4k2k2P())
            {
                u16HsizeTiming = stStreamInfo->u16HSize >> 1;
            }
            else
            {
                u16HsizeTiming = stStreamInfo->u16HSize;
                u16HsizeTiming = ALIGN_UPTO_2(u16HsizeTiming);
            }
            if(stStreamInfo->b3DSBS)
            {
                u16HsizeTiming *= 2;
            }
            if(stStreamInfo->u16HSize > 720)
            {
                u16HsizeTiming +=  MVOP_HBlank_HD;
            }
            else
            {
                u16HsizeTiming +=  MVOP_HBlank_SD;
            }

            u64MaxClk = HAL_MVOP_GetMaximumClk();
            if(stStreamInfo->b3DTB)
            {
                u16VsizeTiming = stStreamInfo->u16VSize*2 + MVOP_VBlank;
            }
            else
            {
                u16VsizeTiming = stStreamInfo->u16VSize + MVOP_VBlank;
            }

            stStreamInfo->u32Framerate = (u64MaxClk / u16HsizeTiming / u16VsizeTiming)*1000;
        }
            break;
        default:
            break;
    }

}

MS_BOOL HAL_MVOP_GetIsSendingData(MVOP_DevID eDevID)
{
     MS_U8 u8IsSend = 0;

    if(eDevID == E_MVOP_DEV_0)
    {
        u8IsSend = HAL_ReadByte(VOP_DEBUG_2D_H) & VOP_DMA_STATUS;
        if(u8IsSend)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        MVOP_PRINTF("[Warning] not support device %d\n",eDevID);
        return FALSE;
    }
}

/******************************************************************************/
/// Set MVOP Handshake Mode, XC should be synchronous with MVOP.
/// this command should be before mvop enable.(before 1st frame)
/******************************************************************************/
void HAL_MVOP_SetTimingFromXC(MVOP_DevID eID, MS_BOOL bEnable)
{
    switch(eID)
    {
        MVOP_DBG(MVOP_PRINTF("[MVOP] HAL_MVOP_SetTimingFromXC = %d\n",bEnable);)
        case E_MVOP_DEV_0:
        {
            g_pHalMVOPCtx->bIsXcTrig = bEnable;
            if(bEnable)
            {
                HAL_WriteByteMask(VOP_HANDSHAKE, 0x06, VOP_TRIG_FROM_XC|VOP_VSUNC_FROM_XC);
                //HAL_WriteRegBit(VOP_CTRL1, 0, BIT4); // disable inverse the field to IP - not in kano

            }
            else
            {
                HAL_WriteByteMask(VOP_HANDSHAKE, 0x00, VOP_TRIG_FROM_XC|VOP_VSUNC_FROM_XC);
                //HAL_WriteRegBit(VOP_CTRL1, 1, BIT4);  // inverse the field to IP - not in kano
            }
            HAL_MVOP_LoadReg();
            break;
        }
        case E_MVOP_DEV_1:
        {
#if MVOP_SUPPORT_SUB
            g_pHalMVOPCtx->bSubIsXcTrig = bEnable;
            if(bEnable)
            {
                HAL_WriteByteMask(SUB_REG(VOP_HANDSHAKE), 0x06, VOP_TRIG_FROM_XC|VOP_VSUNC_FROM_XC);
            }
            else
            {
                HAL_WriteByteMask(SUB_REG(VOP_HANDSHAKE), 0x00, VOP_TRIG_FROM_XC|VOP_VSUNC_FROM_XC);
            }
            HAL_MVOP_SubLoadReg();
#endif
            break;
        }
        default:
            MVOP_PRINTF("[Warning] MVOP_DevID not support!\n");
            break;
    }
}

void HAL_MVOP_ResetReg(MVOP_DevID eDevID, MS_U16 u16ECONumber)
{

    switch(eDevID)
    {
    case E_MVOP_DEV_0:
    {
#if ENABLE_3D_LR_MODE
        HAL_MVOP_Enable3DLR(DISABLE);
#endif
#if SUPPORT_3DLR_ALT_SBS
        HAL_MVOP_Set3DLRAltOutput(DISABLE);
        HAL_MVOP_Set3DLRAltSBSOutput(DISABLE);
#endif

    /*****************************************************/
    // Reset MVOP setting
    HAL_WriteByte(VOP_TST_IMG, 0x40); //reset test pattern or BG
    HAL_MVOP_Set3DLRAltOutput_VHalfScaling(DISABLE); //reset to default: disable 3D L/R alternative output.
    HAL_MVOP_Set3DLR2ndCfg(DISABLE);    //reset to default: disable 3D L/R 2nd pitch.
    HAL_MVOP_SetRgbFormat(E_MVOP_RGB_NONE); //reset rgb format
    HAL_WriteRegBit(VOP_CTRL1, 0, VOP_MVD_VS_MD); //default use original vsync
    //stb series: default hsk
    //HAL_WriteRegBit(VOP_REG_422_OUT_EN, 1, BIT0); //kano ip2 444
    //apple for debug
    HAL_WriteRegBit(VOP_HANDSHAKE, 1, VOP_HANDSHAKE_MODE);
    g_pHalMVOPCtx->bIsHS = TRUE;
    HAL_MVOP_SetTimingFromXC(E_MVOP_DEV_0, TRUE);

    // Kano hsk default 2p
    HAL_MVOP_SetEnable4k2k2P(TRUE);

    HAL_MVOP_SetBlackBG();//set MVOP test pattern to black
    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);// clear extend strip len bit by default
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);// set mvop to 128bit_i128 interface
    HAL_WriteRegBit(VOP_INPUT_SWITCH1, 0, BIT2|BIT3);// Disable H264 or RM Input

    HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);//8*32 tile format
    HAL_WriteRegBit(VOP_MIRROR_CFG_HI, 0, VOP_REF_SELF_FLD);

    g_pHalMVOPCtx->bIs422 = 0;// Clear 422 Flag
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 0, EVD_ENABLE);// EVD mode disable
    g_pHalMVOPCtx->bIsH265 = 0;
    HAL_WriteRegBit(VOP_INT_TYPE, 0, VOP_EVD_INT_SEP);// Clear evd Flag for interlace mode setting


    HAL_WriteByteMask(VOP_REG_MASK, BIT1, VOP_LSB_REQ_MASK);// EVD 10 bits disable
    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_Y_EN);
    HAL_WriteRegBit(VOP_EVD_10B_EN, 0, VOP_EVD_10B_UV_EN);
    HAL_WriteRegBit(VOP_UV_SHIFT, 1, VOP_GCLK_MIU_ON);

    HAL_WriteRegBit(VOP_REG_DUMMY, 1, VOP_420_BW_SAVE);
    HAL_WriteRegBit(VOP_REG_BW_SAVE, 1, VOP_420_BW_SAVE_EX);
    HAL_MVOP_LoadReg(); //kano patch, mantis: 0997677
    g_pHalMVOPCtx->bNewVSyncMode = FALSE;// Disable New Vsync Mode

    /* only for STB DC */
    // Handshake mode chip need to set from MVD: Kappa (maybe fix in after project)
    HAL_MVOP_SetFDMaskFromMVD(ENABLE);
    // Use frame sync (CLK_DC1 domain) to reset CLK_DC0 and CLK_MIU domain every frame
    HAL_WriteRegBit(VOP_REG_FRAME_RST+1, 0, BIT7); // reg_frame_rst = 0

    // Warning: set from tv, clippers HAL_MVOP_SetFieldInverse(ENABLE, DISABLE);
    HAL_MVOP_SetFieldInverse(ENABLE, ENABLE);
    // Warning: kappa U02 need to bypass field from mvd to xc for FCR.
    HAL_WriteRegBit(VOP_REG_DUMMY_3D_1, 1, VOP_RGB_FILED_BYPASS);

    // Clippers temp patch: New diu interface, waiting for decoder adding diu setting.
    HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL | VOP_INFO_FROM_CODEC_MIU_BUF1_SEL | VOP_INFO_FROM_CODEC_10BIT);

    //Change MIU BW Default Value to 0x241C (after Einstein)
    //FIFO threshold
    HAL_WriteByte(VOP_DMA0, 0x08);
    //burst and priority
    HAL_WriteByte(VOP_DMA1, 0x22);
    HAL_WriteByte(VOP_BURST_CTRL0, 0x02);
    HAL_WriteByte(VOP_BURST_CTRL1, 0x00);
    HAL_WriteByteMask(VOP_REG_MFDEC_0_L, 0x70, VOP_MF1_BURST|VOP_MF0_BURST|VOP_MFDEC_EN);// Setting MF burst len
    // Only for monaco: Disable mfdec setting from wb
    HAL_WriteRegBit(VOP_REG_MFDEC_2_L, 0, VOP_MF_FROM_WB);
    // MIU select from WB
    HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0x00, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);
    // All codec use WISHBONE(R2) interface in muji
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, VOP_R2_WISHBONE);
    // Disable 10 bits from codec
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_H, 0, VOP_INFO_FROM_CODEC_10BIT);
    //// clippers crop sw patch /////
    HAL_WriteRegBit(VOP_REG_SINGLE_3D_L, 0, VOP_FORCE_SKIP);
    HAL_WriteByteMask(VOP_REG_SINGLE_3D_L, 0, VOP_SKIP_SIZE_LVIEW);
    HAL_WriteByteMask(VOP_REG_SINGLE_3D_H, 0, VOP_SKIP_SIZE_RVIEW);
    HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_LVIEW);
    HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_RVIEW);
    g_pHalMVOPCtx->u16CropXStart = 0;
    g_pHalMVOPCtx->u16CropYStart = 0;
    g_pHalMVOPCtx->u16CropXSize = 0;
    g_pHalMVOPCtx->u16CropYSize = 0;
    g_pHalMVOPCtx->bIsSetCrop = FALSE;
    g_pHalMVOPCtx->u32MVOPFixClk = 0;
    // MIU select from WB
    HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0x30, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);
    HAL_WriteRegBit(VOP_UV_SHIFT, 0, VOP_GCLK_MIU_ON);// yc seperate can not enable
    g_pHalMVOPCtx->eInterlaceType = E_MVOP_PRO;
    HAL_WriteByte(VOP_REG_BLK_VCNT_L, 0x00);//mantis: 0986938
    HAL_WriteByteMask(VOP_REG_BLK_VCNT_H, 0x01, BIT0);
    HAL_WriteByteMask(REG_CKG_FBDEC, 0x0, VOP_SEL_CLK_432); //default disable mfdec clock.
    HAL_MVOP_LoadReg();
    /*****************************************************/
        break;
    }
#if MVOP_SUPPORT_SUB
    case E_MVOP_DEV_1:
    {
#if ENABLE_3D_LR_MODE
        HAL_MVOP_SubEnable3DLR(DISABLE);
#endif
#if SUPPORT_3DLR_ALT_SBS
        HAL_MVOP_SubSet3DLRAltOutput(DISABLE);
        HAL_MVOP_SubSet3DLRAltSBSOutput(DISABLE);
#endif
        HAL_MVOP_SubEnableMVDInterface(FALSE);

        /*****************************************************/
        // Reset MVOP setting
        HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x40);
        HAL_MVOP_SubSetRgbFormat(E_MVOP_RGB_NONE);
        HAL_WriteRegBit(SUB_REG(VOP_CTRL1), 0, VOP_MVD_VS_MD); //default use original vsync
        //stb series: default hsk
        //HAL_WriteRegBit(SUB_REG(VOP_REG_422_OUT_EN), 1, BIT0); //kano ip2 444
        HAL_WriteRegBit(SUB_REG(VOP_HANDSHAKE), 1, VOP_HANDSHAKE_MODE);
        g_pHalMVOPCtx->bSubIsHS = TRUE;
        HAL_MVOP_SetTimingFromXC(E_MVOP_DEV_1, TRUE);
        // Kano hsk default 2p
        HAL_MVOP_SubSetEnable4k2k2P(TRUE);

        // Only for Monaco: Enable deciding bot by top address + 2
        HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), 1, VOP_FIELD_FROM_ADDR);
        HAL_MVOP_SubSetMonoMode(FALSE);// Reset Mono mode
        HAL_MVOP_SubSetBlackBG();//set MVOP test pattern to black
        HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);// clear extend strip len bit by default
        HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);// set mvop to 128bit_i128 interface
        /* only for STB DC */
        // Use frame sync (CLK_DC1 domain) to reset CLK_DC0 and CLK_MIU domain every frame
        HAL_WriteRegBit(SUB_REG(VOP_REG_FRAME_RST+1), 0, BIT7); // reg_frame_rst = 0

        // Warning: set from tv, clippers HAL_MVOP_SetFieldInverse(ENABLE, DISABLE);
        HAL_MVOP_SubSetFieldInverse(ENABLE, ENABLE);
        // Warning: kappa U02 need to bypass field from mvd to xc for FCR.
        HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY_3D_1), 1, VOP_RGB_FILED_BYPASS);

        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH1), 0, BIT2|BIT3);// Disable H264 or RM Input
        g_pHalMVOPCtx->bSubIs422 = 0;// Clear 422 Flag
        g_pHalMVOPCtx->bSubIsH265 = 0;// Clear evd Flag for interlace mode setting
        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), 0, EVD_ENABLE);
        HAL_WriteRegBit(SUB_REG(VOP_INT_TYPE), 0, VOP_EVD_INT_SEP);

        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT1);//8*32 tile format
        HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG_HI), 0, VOP_REF_SELF_FLD);

        HAL_WriteByteMask(SUB_REG(VOP_REG_MASK), BIT1, VOP_LSB_REQ_MASK);// EVD 10 bits
        HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 0, VOP_EVD_10B_Y_EN);
        HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 0, VOP_EVD_10B_UV_EN);
        // Disable 420 BW Saving mode
        HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), 1, VOP_420_BW_SAVE);
        HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 1, VOP_420_BW_SAVE_EX);
        HAL_MVOP_SubLoadReg(); //kano patch, mantis: 0997677
        // VP9 MODE disable
        //HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), 0, VOP_R2_WISHBONE);
        HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 0, VOP_DRAM_RD_MODE);

        g_pHalMVOPCtx->bSubNewVSyncMode = FALSE;// Disable New Vsync Mode
        HAL_WriteRegBit(SUB_REG(VOP_INFO_FROM_CODEC_H), 1, VOP_INFO_FROM_CODEC_DS_IDX);// Sub mvop ds idx from DIU
        HAL_WriteByteMask(SUB_REG(VOP_REG_MFDEC_0_L), 0x70, VOP_MF1_BURST|VOP_MF0_BURST|VOP_MFDEC_EN);// Setting MF burst len
        // Only for monaco: Disable mfdec setting from wb
        HAL_WriteRegBit(SUB_REG(VOP_REG_MFDEC_2_L), 0, VOP_MF_FROM_WB);
        // All codec use WISHBONE(R2) interface in muji
        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), 1, VOP_R2_WISHBONE);
        // Disable 10 bits from codec
        HAL_WriteRegBit(SUB_REG(VOP_INFO_FROM_CODEC_H), 0, VOP_INFO_FROM_CODEC_10BIT);
        g_pHalMVOPCtx->u16SubCropXStart = 0;
        g_pHalMVOPCtx->u16CropYStart = 0;
        g_pHalMVOPCtx->u16CropXSize = 0;
        g_pHalMVOPCtx->u16CropYSize = 0;
        g_pHalMVOPCtx->bSubIsSetCrop = FALSE;
        g_pHalMVOPCtx->u32SubMVOPFixClk = 0;
        // MIU select from WB
        HAL_WriteByteMask(SUB_REG(VOP_INFO_FROM_CODEC_H), 0x30, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);
        HAL_WriteRegBit(SUB_REG(VOP_UV_SHIFT), 0, VOP_GCLK_MIU_ON);// yc seperate can not enable
        g_pHalMVOPCtx->eSubInterlaceType = E_MVOP_PRO;
        HAL_WriteByte(SUB_REG(VOP_REG_BLK_VCNT_L), 0x00);//mantis: 0986938
        HAL_WriteByteMask(SUB_REG(VOP_REG_BLK_VCNT_H), 0x01, BIT0);
        HAL_WriteByteMask(SUB_REG(REG_CKG_FBDEC), 0x0, VOP_SEL_CLK_432); //default disable mfdec clock.
        HAL_MVOP_SubLoadReg();
        /*****************************************************/
        break;
    }
#endif
    default:
        MVOP_PRINTF("[%s] Input Device ID is Error!\n",__FUNCTION__);
        break;
    }
}

/******************************************************************************/
/// Set MVOP Handshake Mode, XC should be synchronous with MVOP.
/// this command should be before mvop enable.(before 1st frame)
/// kano hsk disable, xc gen timing need to disable.
/******************************************************************************/
void HAL_MVOP_SetHandShakeMode(MVOP_DevID eID, MS_BOOL bEnable, MS_U8 u8Framerate)
{
    MS_U8 u8FrmDur = 40;
    MS_BOOL bMCU = FALSE;

    if(u8Framerate != 0)
    {
        u8FrmDur = 1000/u8Framerate; //time of one frame(ms).
    }

    switch(eID)
    {
        case E_MVOP_DEV_0:
        {
            if(!((bEnable == FALSE) && (g_pHalMVOPCtx->bIsHS == FALSE))) //prevent switch VOP_CTRL0 (mvop enable), for MHEG5 verify
            {
                bMCU = HAL_ReadRegBit(VOP_MPG_JPG_SWITCH, BIT1);
                MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_SetHandShakeMode = %d\n",bEnable);)
                HAL_WriteRegBit(VOP_CTRL0, 0, BIT0);
                g_pHalMVOPCtx->bIsHS = bEnable;
                HAL_WriteRegBit(VOP_HANDSHAKE, bEnable, VOP_HANDSHAKE_MODE);
                if(bEnable == FALSE)
                {
                    HAL_MVOP_SetTimingFromXC(E_MVOP_DEV_0, FALSE);
                }
                HAL_WriteRegBit(VOP_CTRL0, bEnable, BIT4);
                HAL_WriteRegBit(VOP_MIRROR_CFG_HI, bEnable, VOP_HK_MASK); //bk background
                HAL_WriteRegBit(VOP_CTRL0, 1, BIT0);
                // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
                MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
                HAL_MVOP_SetBlackBG();
                HAL_MVOP_SetPattern(MVOP_PATTERN_FRAMECOLOR);
                HAL_WriteRegBit(VOP_REG_WR, 1, BIT4);
                HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 1, BIT1);
                MsOS_DelayTask(u8FrmDur);
                if(bMCU == FALSE)
                {
                    HAL_WriteRegBit(VOP_MPG_JPG_SWITCH, 0, BIT1);
                }
                HAL_MVOP_SetPattern(MVOP_PATTERN_NORMAL);
                HAL_WriteRegBit(VOP_REG_WR, 0, BIT4);
                HAL_MVOP_LoadReg();
            }
            break;
        }
        case E_MVOP_DEV_1:
        {
#if MVOP_SUPPORT_SUB
            if(!((bEnable == FALSE) && (g_pHalMVOPCtx->bSubIsHS == FALSE))) //prevent switch VOP_CTRL0 (mvop enable), for MHEG5 verify
            {
                bMCU = HAL_ReadRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), BIT1);
                HAL_WriteRegBit(SUB_REG(VOP_CTRL0), 0, BIT0);
                g_pHalMVOPCtx->bSubIsHS = bEnable;
                HAL_WriteRegBit(SUB_REG(VOP_HANDSHAKE), bEnable, VOP_HANDSHAKE_MODE);
                if(bEnable == FALSE)
                {
                    HAL_MVOP_SetTimingFromXC(E_MVOP_DEV_1, FALSE);
                }
                HAL_WriteRegBit(SUB_REG(VOP_CTRL0), bEnable, BIT4);
                HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG_HI), bEnable, VOP_HK_MASK); //bk background
                HAL_WriteRegBit(SUB_REG(VOP_CTRL0), 1, BIT0);
                // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
                MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
                HAL_MVOP_SubSetBlackBG();
                HAL_MVOP_SubSetPattern(MVOP_PATTERN_FRAMECOLOR);
                HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT4);
                HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 1, BIT1);
                MsOS_DelayTask(u8FrmDur);
                if(bMCU == FALSE)
                {
                    HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 0, BIT1);
                }
                HAL_MVOP_SubSetPattern(MVOP_PATTERN_NORMAL);
                HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT4);
            }
#endif
            break;
        }
        default:
            MVOP_PRINTF("[Warning] MVOP_DevID not support!\n");
            break;
    }
}

void HAL_MVOP_SetCropforXC(MVOP_DevID eID, MVOP_XCGetCrop* stXCCrop, MS_U16 u16Width, MS_U16 u16Height)
{
    MVOP_DBG(MVOP_PRINTF("%s [crop info] xst: 0x%x yst: 0x%x xsize: 0x%x ysize: 0x%x Width: 0x%x Height: 0x%x\n", __FUNCTION__,
        stXCCrop->u16XStart, stXCCrop->u16XSize, stXCCrop->u16YStart, stXCCrop->u16YSize, u16Width, u16Height);)

    switch(eID)
    {
        case E_MVOP_DEV_0:
        {
            if(g_pHalMVOPCtx->u16CropXSize == 0)
                g_pHalMVOPCtx->u16CropXSize = u16Width;

            if((stXCCrop->u16XStart + stXCCrop->u16XSize) > g_pHalMVOPCtx->u16CropXSize)
            {
                MVOP_PRINTF("[Warning] crop x size or start out of boundary.");
                return;
            }

            if(g_pHalMVOPCtx->u16CropYSize == 0)
                g_pHalMVOPCtx->u16CropYSize = u16Height;

            if((stXCCrop->u16YStart + stXCCrop->u16YSize) > g_pHalMVOPCtx->u16CropYSize)
            {
                MVOP_PRINTF("[Warning] crop y size or start out of boundary.");
                return;
            }
#if 0 // manhattan mvop hw statue: crop -> mirror
            if(TRUE == g_pHalMVOPCtx->bMirrorModeHor)
            {
                g_pHalMVOPCtx->u16CropXStart = g_pHalMVOPCtx->u16CropXSize - stXCCrop->u16XStart - stXCCrop->u16XSize;
                stXCCrop->u16XStart = g_pHalMVOPCtx->u16CropXStart;
                g_pHalMVOPCtx->u16CropXSize = stXCCrop->u16XSize;
            }
            else
#endif
            {
                stXCCrop->u16XStart += g_pHalMVOPCtx->u16CropXStart;
                g_pHalMVOPCtx->u16CropXStart = stXCCrop->u16XStart;
                g_pHalMVOPCtx->u16CropXSize = stXCCrop->u16XSize;
            }
#if 0 // manhattan mvop hw statue: crop -> mirror
            if(TRUE == g_pHalMVOPCtx->bMirrorModeVer)
            {
                g_pHalMVOPCtx->u16CropYStart = g_pHalMVOPCtx->u16CropYSize - stXCCrop->u16YStart - stXCCrop->u16YSize;
                stXCCrop->u16YStart = g_pHalMVOPCtx->u16CropYStart;
                g_pHalMVOPCtx->u16CropYSize = stXCCrop->u16YSize;

            }
            else
#endif
            {
                stXCCrop->u16YStart += g_pHalMVOPCtx->u16CropYStart;
                g_pHalMVOPCtx->u16CropYStart = stXCCrop->u16YStart;
                stXCCrop->u16YStart |= 0x2000;

                g_pHalMVOPCtx->u16CropYSize = stXCCrop->u16YSize;
            }
            break;
        }
        case E_MVOP_DEV_1:
        {
#if MVOP_SUPPORT_SUB
            if(g_pHalMVOPCtx->u16SubCropXSize == 0)
                g_pHalMVOPCtx->u16SubCropXSize = u16Width;

            if((stXCCrop->u16XStart + stXCCrop->u16XSize) > g_pHalMVOPCtx->u16SubCropXSize)
            {
                MVOP_PRINTF("[Warning] crop x size or start out of boundary.");
                return;
            }

            if(g_pHalMVOPCtx->u16SubCropYSize == 0)
                g_pHalMVOPCtx->u16SubCropYSize = u16Height;

            if((stXCCrop->u16YStart + stXCCrop->u16YSize) > g_pHalMVOPCtx->u16SubCropYSize)
            {
                MVOP_PRINTF("[Warning] crop y size or start out of boundary.");
                return;
            }
#if 0 // monet mvop hw statue: crop -> mirror
            if(TRUE == g_pHalMVOPCtx->bSubMirrorModeHor)
            {
                g_pHalMVOPCtx->u16SubCropXStart = g_pHalMVOPCtx->u16SubCropXSize - stXCCrop->u16XStart - stXCCrop->u16XSize;
                stXCCrop->u16XStart = g_pHalMVOPCtx->u16SubCropXStart;
                g_pHalMVOPCtx->u16SubCropXSize = stXCCrop->u16XSize;
            }
            else
#endif
            {
                stXCCrop->u16XStart += g_pHalMVOPCtx->u16SubCropXStart;
                g_pHalMVOPCtx->u16SubCropXStart = stXCCrop->u16XStart;
                g_pHalMVOPCtx->u16SubCropXSize = stXCCrop->u16XSize;
            }
#if 0 // monet mvop hw statue: crop -> mirror

            if(TRUE == g_pHalMVOPCtx->bSubMirrorModeVer)
            {
                g_pHalMVOPCtx->u16SubCropYStart = g_pHalMVOPCtx->u16SubCropYSize - stXCCrop->u16YStart - stXCCrop->u16YSize;
                stXCCrop->u16YStart = g_pHalMVOPCtx->u16SubCropYStart;
                g_pHalMVOPCtx->u16SubCropYSize = stXCCrop->u16YSize;

            }
            else
#endif
            {
                stXCCrop->u16YStart += g_pHalMVOPCtx->u16SubCropYStart;
                g_pHalMVOPCtx->u16SubCropYStart = stXCCrop->u16YStart;
                stXCCrop->u16YStart |= 0x2000;

                g_pHalMVOPCtx->u16SubCropYSize = stXCCrop->u16YSize;
            }

#endif
            break;
        }
        default:
            MVOP_PRINTF("[Warning] MVOP_DevID not support!\n");
            break;
    }
    MVOP_PRINTF("[Debug][Crop to XC] xsize = %d, ysize = %d, xstart = %d, ystart=%d\n",stXCCrop->u16XSize,stXCCrop->u16YSize,stXCCrop->u16XStart,stXCCrop->u16YStart);
    return;
}

MS_BOOL HAL_MVOP_SupportFRCOutputFPS(MVOP_DevID eID)
{
    MS_BOOL bRet = FALSE;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            bRet = TRUE;
            break;
#if MVOP_SUPPORT_SUB
        case E_MVOP_DEV_1:
            bRet = TRUE;
            break;
#endif
        default:
            break;
    }
    return bRet;
}

MS_BOOL HAL_MVOP_GetIsCurrentHSK(MVOP_DevID eID)
{
    MS_BOOL bRet = FALSE;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            bRet = g_pHalMVOPCtx->bIsHS;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            bRet = g_pHalMVOPCtx->bSubIsHS;
#endif
            break;
        default:
            bRet = E_MVOP_HS_INVALID_PARAM;
            break;
    }
    return bRet;
}


MS_BOOL HAL_MVOP_GetIsCurrentXCGenTiming(MVOP_DevID eID)
{
    MS_BOOL bRet = FALSE;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            bRet = g_pHalMVOPCtx->bIsXcTrig;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            bRet = g_pHalMVOPCtx->bSubIsXcTrig;
#endif
            break;
        default:
            bRet = E_MVOP_HS_INVALID_PARAM;
            break;
    }
    return bRet;
}

HALMVOPFREQUENCY HAL_MVOP_SetFixClk(MVOP_DevID eID, MS_U32 u32MVOPClk)
{
    HALMVOPFREQUENCY eRet = HALMVOP_160MHZ;

    if(u32MVOPClk > HALMVOP_320MHZ*2)
    {
        MVOP_PRINTF("[Warning] MVOP clock out of spec(640MHz), set freerun mode.\n");
        eRet = HALMVOP_FREERUNMODE;
    }
    else if(u32MVOPClk <= HALMVOP_320MHZ*2 && u32MVOPClk > HALMVOP_320MHZ)
    {
        eRet = u32MVOPClk;
    }
    else if(u32MVOPClk <= HALMVOP_320MHZ && u32MVOPClk > HALMVOP_192MHZ)
    {
        eRet = u32MVOPClk;
    }
    else if(u32MVOPClk <= HALMVOP_192MHZ && u32MVOPClk > HALMVOP_160MHZ)
    {
        eRet = HALMVOP_192MHZ;
    }
    else if(u32MVOPClk <= HALMVOP_160MHZ && u32MVOPClk > HALMVOP_144MHZ)
    {
        eRet = HALMVOP_160MHZ;
    }
    else if(u32MVOPClk <= HALMVOP_144MHZ && u32MVOPClk > HALMVOP_123MHZ)
    {
        eRet = HALMVOP_144MHZ;
    }
    else if(u32MVOPClk <= HALMVOP_123MHZ && u32MVOPClk > HALMVOP_108MHZ)
    {
        eRet = HALMVOP_123MHZ;
    }
    else
    {
        eRet = HALMVOP_108MHZ;
    }

    MVOP_DBG(MVOP_PRINTF("[MVOP] MVOP clock set as %ld\n",eRet););
    if(eID == E_MVOP_DEV_0)
    {
        if(eRet > HALMVOP_192MHZ)
        {
            g_pHalMVOPCtx->u32MVOPFixClk = HALMVOP_320MHZ;
        }
        else
        {
            g_pHalMVOPCtx->u32MVOPFixClk = eRet;
        }
    }
    else if(eID == E_MVOP_DEV_1)
    {
        if(eRet > HALMVOP_192MHZ)
        {
            g_pHalMVOPCtx->u32SubMVOPFixClk = HALMVOP_320MHZ;
        }
        else
        {
            g_pHalMVOPCtx->u32SubMVOPFixClk = eRet;
        }
    }
    return eRet;
}


#if 0
void HAL_SetSCFEMIUIPSel(MVOP_SCIPSel *stIPSel)
{
    if(NULL == stIPSel)
    {
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, 1, MVOP_MIU_IP_SEL);  //mvop
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, 1, MFDEC0_MIU_IP_SEL);  //mfdec0
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, 1, MFDEC1_MIU_IP_SEL);  //mfdec1
    }
    else
    {
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, stIPSel->bMVOP0, MVOP_MIU_IP_SEL);  //mvop
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, stIPSel->bMFDEC0, MFDEC0_MIU_IP_SEL);  //mfdec0
        HAL_WriteRegBit(REG_MIU_SEL_FROM_IP, stIPSel->bMFDEC1, MFDEC1_MIU_IP_SEL);  //mfdec1
    }

}
#endif

void HAL_MVOP_SetInterlaceType(MVOP_DevID eDevID, MS_U16 u16ECONumber, MS_U8 u8Interlace)
{
    MVOP_DBG(MVOP_PRINTF("%s u8Interlace = %d\n", __FUNCTION__ ,u8Interlace);)
    switch(u8Interlace)
    {
    case 0:
        g_pHalMVOPCtx->eInterlaceType = E_MVOP_PRO;
        break;
    case 0x1:
        if(g_pHalMVOPCtx->bIsH265)
        {
            g_pHalMVOPCtx->eInterlaceType = E_MVOP_INT_TB_SEP_FRAME;
        }
        else
        {
            g_pHalMVOPCtx->eInterlaceType = E_MVOP_INT_TB_ONE_FRAME;
        }
        break;
    case 0x2:
        if(g_pHalMVOPCtx->bIsH265)
        {
            g_pHalMVOPCtx->eInterlaceType = E_MVOP_INT_TB_ONE_FRAME;
        }
        else
        {
            g_pHalMVOPCtx->eInterlaceType = E_MVOP_INT_TB_SEP_FRAME;
        }
        break;
    default:
        g_pHalMVOPCtx->eInterlaceType = E_MVOP_PRO;
        break;
    }

    switch(eDevID)
    {
    case E_MVOP_DEV_0:
        if((g_pHalMVOPCtx->eInterlaceType == E_MVOP_INT_TB_SEP_FRAME) && (g_pHalMVOPCtx->bIsH265))
        {
            HAL_WriteRegBit(VOP_INT_TYPE, 1, VOP_EVD_INT_SEP);
        }
        else if((g_pHalMVOPCtx->eInterlaceType == E_MVOP_INT_TB_ONE_FRAME) && (g_pHalMVOPCtx->bIsH265))
        {
            HAL_WriteRegBit(VOP_INT_TYPE, 0, VOP_EVD_INT_SEP);
            HAL_WriteRegBit(VOP_MIRROR_CFG, 0, VOP_FIELD_FROM_ADDR);
        }
        HAL_MVOP_LoadReg();
        break;
#if MVOP_SUPPORT_SUB
    case E_MVOP_DEV_1:
        if((g_pHalMVOPCtx->eSubInterlaceType == E_MVOP_INT_TB_SEP_FRAME) && (g_pHalMVOPCtx->bSubIsH265))
        {
            HAL_WriteRegBit(SUB_REG(VOP_INT_TYPE), 1, VOP_EVD_INT_SEP);
        }
        else if((g_pHalMVOPCtx->eSubInterlaceType == E_MVOP_INT_TB_ONE_FRAME) && (g_pHalMVOPCtx->bSubIsH265))
        {
            HAL_WriteRegBit(SUB_REG(VOP_INT_TYPE), 0, VOP_EVD_INT_SEP);
            HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), 0, VOP_FIELD_FROM_ADDR);
        }
        HAL_MVOP_SubLoadReg();
        break;
#endif
    default:
        MVOP_PRINTF("[Warning][%s] MVOP_DevID not support\n",__FUNCTION__);
        break;
    }
}

#ifdef UFO_MVOP_GET_IS_MVOP_AUTO_GEN_BLACK
MS_BOOL HAL_MVOP_GetIsMVOPSupportBLKBackground(MVOP_DevID eID)
{
    MS_BOOL bRet = FALSE;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            bRet = TRUE;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            bRet = TRUE;
#endif
            break;
        default:
            bRet = E_MVOP_HS_INVALID_PARAM;
            break;
    }
    return bRet;
}
#endif

MS_U16 HAL_MVOP_ReadBank(MVOP_DevID eID ,MS_U16 u16Length)
{
    MS_U16 u16Reg = 0;
    if(eID == E_MVOP_DEV_0)
    {
        u16Reg = HAL_Read2Byte(MVOP_REG_BASE + (u16Length << 1));
    }
#ifdef MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        u16Reg = HAL_Read2Byte(SUB_REG(MVOP_REG_BASE + (u16Length << 1)));
    }
#endif
    return u16Reg;
}

void HAL_MVOP_WriteBank(MVOP_DevID eID ,MS_U16 u16Length,MS_U16 u16Data)
{
    if(eID == E_MVOP_DEV_0)
    {
        HAL_Write2Byte(MVOP_REG_BASE + (u16Length << 1),u16Data);
    }
#ifdef MVOP_SUPPORT_SUB
    else if(eID == E_MVOP_DEV_1)
    {
        HAL_Write2Byte(SUB_REG(MVOP_REG_BASE + (u16Length << 1)),u16Data);
    }
#endif
}

MS_U16 HAL_MVOP_ReadClkBank(MS_U16 u16Length)
{
    MS_U16 u16Reg = 0;

    if(u16Length < 3) // stc cw
    {
        u16Reg = HAL_Read2Byte(REG_STC_CW_SLE_L + (u16Length << 1));

    }
    else if(u16Length < 7) // dc clk
    {
        u16Length -= 3;
        u16Reg = HAL_Read2Byte(REG_CKG_DC0 + (u16Length << 1)); // main/sub sram
    }
    else // freerun/sync clk
    {
        u16Length -= 7;
        u16Reg = HAL_Read2Byte(REG_UPDATE_DC0_CW + (u16Length << 1));
    }

    //printk("[read][%x] u16Reg = 0x%x\n",u16Length,u16Reg);
    return u16Reg;
}

void HAL_MVOP_WriteClkBank(MS_U16 u16Length,MS_U16 u16Data)
{

    //printk("[write][%x] u16Reg = 0x%x\n",u16Length,u16Data);

    if(u16Length < 3) // stc cw
    {
        HAL_Write2Byte(REG_STC_CW_SLE_L + (u16Length << 1), u16Data);
    }
    else if(u16Length < 7) // dc clk
    {
        u16Length -= 3;
        HAL_Write2Byte(REG_CKG_DC0 + (u16Length << 1), u16Data);
    }
    else // freerun/sync clk
    {
        u16Length -= 7;
        HAL_Write2Byte(REG_UPDATE_DC0_CW + (u16Length << 1), u16Data);
    }
    HAL_WriteRegBit(REG_UPDATE_DC0_CW, TRUE, UPDATE_DC0_SYNC_CW);
    HAL_WriteRegBit(REG_UPDATE_DC0_CW, FALSE, UPDATE_DC0_SYNC_CW);

}

void HAL_MVOP_SetSramPower(MVOP_DevID eID ,MS_BOOL bEnable)
{
    switch(eID)
    {
    case E_MVOP_DEV_0:
#if MVOP_SUPPORT_SUB
        if((bEnable== FALSE) && (g_pHalMVOPCtx->bIsSubEnable == FALSE)) //check sub disable -> mfdec sram disable
        {
            HAL_WriteRegBit(VOP_DC_STRIP_H, 1, MFDEC_SRAM_SD_MASK);
            HAL_WriteRegBit(VOP_DC_STRIP_H, 1, VOP_SRAM_SD_MASK);
        }
        else
        {
            HAL_WriteRegBit(VOP_DC_STRIP_H, 0, MFDEC_SRAM_SD_MASK);
            HAL_WriteRegBit(VOP_DC_STRIP_H, 0, VOP_SRAM_SD_MASK);
        }
#else
        HAL_WriteRegBit(VOP_DC_STRIP_H, !bEnable, VOP_SRAM_SD_MASK);
#endif
        break;
#if MVOP_SUPPORT_SUB
    case E_MVOP_DEV_1:
        if((bEnable== FALSE) && (g_pHalMVOPCtx->bIsEnable == FALSE))
        {
            HAL_WriteRegBit(VOP_DC_STRIP_H, 1, MFDEC_SRAM_SD_MASK);
            HAL_WriteRegBit(VOP_DC_STRIP_H, 1, VOP_SRAM_SD_MASK);
            HAL_WriteRegBit(SUB_REG(VOP_DC_STRIP_H), 1, VOP_SRAM_SD_MASK);
        }
        else
        {
            HAL_WriteRegBit(VOP_DC_STRIP_H, 0, MFDEC_SRAM_SD_MASK);
            HAL_WriteRegBit(VOP_DC_STRIP_H, 0, VOP_SRAM_SD_MASK); //main need to enable, if sub enable.
            HAL_WriteRegBit(SUB_REG(VOP_DC_STRIP_H), 0, VOP_SRAM_SD_MASK);
        }
        break;
#endif
    default:
        break;
    }
    return;
}


///////////////////////   Sub MVOP   ////////////////////////
#if 1
void HAL_MVOP_SubRegSetBase(MS_VIRT u32Base)
{
    u32RiuBaseAdd = u32Base;
}

void HAL_MVOP_SubInitMirrorMode(MS_BOOL bMir)
{
    //set bit[3:7] to support mirror mode
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), bMir, VOP_MIRROR_CFG_ENABLE);
}

void HAL_MVOP_SubInit(void)
{
    MVOP_HalInitCtxResults eRet;
    MS_BOOL pbFirstDrvInstant;

    eRet = _HAL_MVOP_InitContext(&pbFirstDrvInstant);
    if(eRet == E_MVOP_INIT_FAIL)
    {
        MVOP_DBG(MVOP_PRINTF("[%s] MVOP Context Init failed!\n",__FUNCTION__);)
        return;
    }
    else if(eRet == E_MVOP_INIT_ALREADY_EXIST)
    {
         if(g_pHalMVOPCtx->bSubIsInit)
        {
            MVOP_DBG(MVOP_PRINTF("[%s] Sub MVOP Context has Initialized!\n",__FUNCTION__);)
            return;
        }
    }
    HAL_MVOP_SubInitMirrorMode(TRUE);
    //Enable dynamic clock gating
    //Note: cannot enable VOP_GCLK_VCLK_ON, or hsync cannot be sent out.
    HAL_WriteByteMask(SUB_REG(VOP_GCLK), VOP_GCLK_MIU_ON, VOP_GCLK_MIU_ON|VOP_GCLK_VCLK_ON);
    g_pHalMVOPCtx->bSubIsInit = 1;
}

void HAL_MVOP_SubSetVerticalMirrorMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if (VOP_MIRROR_CFG_ENABLE != HAL_ReadRegBit(SUB_REG(VOP_MIRROR_CFG), VOP_MIRROR_CFG_ENABLE))
    {
        //MVOP_PRINTF("Setup mirror mode\n");
        HAL_MVOP_SubInitMirrorMode(TRUE);
    }

    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), bEnable, VOP_MIRROR_CFG_VEN);
    g_pHalMVOPCtx->bSubMirrorModeVer = bEnable;
}

void HAL_MVOP_SubSetHorizontallMirrorMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if (VOP_MIRROR_CFG_ENABLE != HAL_ReadRegBit(SUB_REG(VOP_MIRROR_CFG), VOP_MIRROR_CFG_ENABLE))
    {
        //MVOP_PRINTF("Setup mirror mode\n");
        HAL_MVOP_SubInitMirrorMode(TRUE);
    }

    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), bEnable, VOP_MIRROR_CFG_HEN);
    g_pHalMVOPCtx->bSubMirrorModeHor = bEnable;
}

void HAL_MVOP_SubSetFieldInverse(MS_BOOL b2MVD, MS_BOOL b2IP)
{
    // Set fld inv & ofld_inv
    HAL_WriteRegBit(SUB_REG(VOP_CTRL0+1), b2MVD, BIT3); //inverse the field to MVD
    HAL_WriteRegBit(SUB_REG(VOP_CTRL0+1), b2IP, BIT4);  //inverse the field to IP
}

void HAL_MVOP_SubSetChromaWeighting(MS_BOOL bEnable)
{
    HAL_WriteRegBit(SUB_REG(VOP_REG_WEIGHT_CTRL), bEnable, BIT1);
}

//load new value into active registers 0x20-0x26
void HAL_MVOP_SubLoadReg(void)
{
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT0);
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT0);

    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT4);
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT4);
}

void HAL_MVOP_SubSetMIUReqMask(MS_BOOL bEnable)
{
#if 0
    if (bEnable)
    {   // mask MVOP2MI to protect MIU
        HAL_MIU_SubSetReqMask(SUBMVOP_R, 1);
        MsOS_DelayTask(1);
    }
    else
    {   // unmask MVOP2MI
        MsOS_DelayTask(1);
        HAL_MIU_SubSetReqMask(SUBMVOP_R, 0);
    }
#endif
    MS_U8 u8Miu;
    if(HAL_MVOP_GetIsOnlyMiuIPControl() == TRUE)
    {
        // mask msb mvop
        u8Miu = (HAL_ReadByte(SUB_REG(VOP_MIU_SEL)) & VOP_MSB_BUF0_MIU_SEL) >> 4;
    }
    else
    {
        u8Miu = SUBVOP_ON_MIU1;
    }
    eMIUClientID eClientID = MIU_CLIENT_MVOP1_R;
    //MVOP_PRINTF("Enter %s bEnable=%x ReqMask=0x%x, 0x%x, u8Miu=%x\n", __FUNCTION__, bEnable,
    //    HAL_ReadByte(0x1266), HAL_ReadByte(0x0666), u8Miu);

    if (bEnable)
    {   // mask MVOP2MI to protect MIU
        MDrv_MIU_MaskReq(u8Miu, eClientID);
        MsOS_DelayTask(1);
    }
    else
    {   // unmask MVOP2MI
        MsOS_DelayTask(1);
        MDrv_MIU_UnMaskReq(u8Miu, eClientID);
    }

    //MVOP_PRINTF(">Exit %s bEnable=%x ReqMask=0x%x, 0x%x, u8Miu=%x\n", __FUNCTION__, bEnable,
    //    HAL_ReadByte(0x1266), HAL_ReadByte(0x0666), u8Miu);
}

void HAL_MVOP_SubRst(void)
{
    MS_BOOL bMCU = FALSE;

    bMCU = HAL_ReadRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), BIT1);
    HAL_WriteRegBit(SUB_REG(VOP_CTRL0), 0, BIT0);
    HAL_WriteRegBit(SUB_REG(VOP_CTRL0), 1, BIT0);
    // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
    MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
    HAL_MVOP_SubSetBlackBG();
    HAL_MVOP_SubSetPattern(MVOP_PATTERN_FRAMECOLOR);
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT4);
    HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 1, BIT1);
    MsOS_DelayTask(40);
    if(bMCU == FALSE)
    {
        HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 0, BIT1);
    }
    HAL_MVOP_SubSetPattern(MVOP_PATTERN_NORMAL);
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT4);
}

void HAL_MVOP_SubEnable(MS_BOOL bEnable, MS_U8 u8Framerate)
{
    MS_U8 regval;
    MS_U8 u8FrmDur = 40;
    MS_BOOL bMCU = FALSE;

    bMCU = HAL_ReadRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), BIT1);

    if(u8Framerate != 0)
    {
        u8FrmDur = 1000/u8Framerate; //time of one frame(ms).
    }
    regval = HAL_ReadByte(SUB_REG(VOP_CTRL0));

    if ( bEnable )
    {
        regval |= 0x1;
    }
    else
    {
        regval &= ~0x1;
    }

    // sw patch: for mvop hsk mode, switching mvop enable/disable may cause video was stuck.
    if ( bEnable && (g_pHalMVOPCtx->bIsSubEnable == FALSE)) // need patch only from off to on
    {
        MVOP_DBG(MVOP_PRINTF("[MVOP][DBG] HAL_MVOP_Enable patch \n");)
        HAL_MVOP_SubSetBlackBG();
        HAL_MVOP_SubSetPattern(MVOP_PATTERN_FRAMECOLOR);
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT4);
        HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 1, BIT1);
        MsOS_DelayTask(u8FrmDur);
        if(bMCU == FALSE)
        {
            HAL_WriteRegBit(SUB_REG(VOP_MPG_JPG_SWITCH), 0, BIT1);
        }
        HAL_MVOP_SubSetPattern(MVOP_PATTERN_NORMAL);
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT4);
    }
    g_pHalMVOPCtx->bIsSubEnable = bEnable;

    HAL_WriteByte(SUB_REG(VOP_CTRL0), regval);
}

MS_BOOL HAL_MVOP_SubGetEnableState(void)
{
    return (HAL_ReadRegBit(SUB_REG(VOP_CTRL0), BIT0));
}

HALMVOPFREQUENCY HAL_MVOP_SubGetMaxFreerunClk()
{
    return HALMVOP_160MHZ;
}

HALMVOPFREQUENCY HAL_MVOP_SubGet4k2kClk()
{
    return HALMVOP_320MHZ;
}

//FIXME
void HAL_MVOP_SubSetFrequency(HALMVOPFREQUENCY enFrequency)
{
    // clear
    HAL_WriteByteMask(REG_CKG_SUB_DC0, 0, CKG_SUB_DC0_MASK);
    switch(enFrequency)
    {
        case HALMVOP_SYNCMODE:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_SYNCHRONOUS, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_FREERUNMODE:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_FREERUN, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_320MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_320MHZ, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_108MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_108MHZ, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_123MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_123MHZ, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_144MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_144MHZ, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_160MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_160MHZ, CKG_SUB_DC0_MASK);
            break;
        case HALMVOP_192MHZ:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_192MHZ, CKG_SUB_DC0_MASK);
            break;
        default:
            HAL_WriteByteMask(REG_CKG_SUB_DC0, CKG_SUB_DC0_160MHZ, CKG_SUB_DC0_MASK);
            MVOP_PRINTF("Attention! In HAL_MVOP_SetFrequency default 160MHz!\n");
            break;
    }
}

void HAL_MVOP_SubSetOutputInterlace(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(SUB_REG(VOP_CTRL0));

    if ( bEnable )
    {
        regval |= 0x80;
        HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
    }
    else
    {
        regval &= ~0x80;
    }

    //Kano patch for GOP
    HAL_WriteRegBit(SUB_REG(VOP_TST_IMG), bEnable, VOP_SC_VS_INV);
    HAL_WriteByte(SUB_REG(VOP_CTRL0), regval);
}

void HAL_MVOP_SubSetPattern(MVOP_Pattern enMVOPPattern)
{
    HAL_WriteByteMask(SUB_REG(VOP_TST_IMG), enMVOPPattern, BIT2 | BIT1 | BIT0);
}

MS_BOOL HAL_MVOP_SubSetTileFormat(MVOP_TileFormat eTileFmt)
{
    if (eTileFmt == E_MVOP_TILE_8x32)
    {
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT1);
        return TRUE;
    }
    else if (eTileFmt == E_MVOP_TILE_16x32)
    {
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT1);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MVOP_SubSetRgbFormat(MVOP_RgbFormat eRgbFmt)
{
    MS_BOOL bRet = FALSE;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    if (eRgbFmt == E_MVOP_RGB_NONE)
    {
        HAL_WriteByteMask(SUB_REG(VOP_RGB_FMT), 0, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }
    else if (eRgbFmt == E_MVOP_RGB_565)
    {
        HAL_WriteByteMask(SUB_REG(VOP_RGB_FMT), VOP_RGB_FMT_565, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }
    else if (eRgbFmt == E_MVOP_RGB_888)
    {
        HAL_WriteByteMask(SUB_REG(VOP_RGB_FMT), VOP_RGB_FMT_888, VOP_RGB_FMT_SEL);
        bRet = TRUE;
    }

    if (bRet == TRUE)
    {
        g_pHalMVOPCtx->eSubRgbFmt = eRgbFmt;
    }
    return bRet;
}

void HAL_MVOP_SubSetBlackBG(void)
{
    MS_U8 regval;

    //set MVOP test pattern to black
    HAL_WriteByte(SUB_REG(VOP_TST_IMG + 1), 0x10);
    HAL_WriteByte(SUB_REG(VOP_U_PAT      ), 0x80);
    HAL_WriteByte(SUB_REG(VOP_U_PAT   + 1), 0x80);

    regval = HAL_ReadByte(SUB_REG(VOP_TST_IMG));
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x02);
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x00);
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), regval);
}

void HAL_MVOP_SubSetCropWindow(MVOP_InputCfg *pparam)
{
#if 1
    UNUSED(pparam);
#else // enable it when test code is ready
    MS_U32 x, y;
    MS_U32 u32offset;

    if(!pparam)
    {
        return;
    }
    //set MVOP test pattern to black
    HAL_MVOP_SubSetBlackBG();
#if 0
    if((pparam->enVideoType == MVOP_H264) && (pparam->u16StripSize == 1920))
    {
        pparam->u16StripSize = 1952;
    }
#endif
    if((pparam->enVideoType == MVOP_MPG) || (pparam->enVideoType == MVOP_MPEG4))
    {
        pparam->u16CropX = (pparam->u16CropX >> 3) << 3; // 8 bytes align
        pparam->u16CropY = (pparam->u16CropY >> 6) << 6; // 64 bytes align
    }
    else if((pparam->enVideoType == MVOP_H264) || (pparam->enVideoType == MVOP_RM))
    {
        pparam->u16CropX = (pparam->u16CropX >> 4) << 4; // 16 bytes align
        pparam->u16CropY = (pparam->u16CropY >> 6) << 6; // 64 bytes align
    }
    else
    {
        MS_ASSERT(0);
    }

    x = (MS_U32)pparam->u16CropX;
    y = (MS_U32)pparam->u16CropY;

    // y offset
    u32offset = ((y * pparam->u16StripSize + (x << 5)) >> 3);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L    ), (MS_U8)(u32offset));
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L + 1), (MS_U8)(u32offset >> 8));
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_H    ), (MS_U8)(u32offset >> 16));
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_H + 1), (MS_U8)((u32offset >> 24) & VOP_YUV_STR_HIBITS));

    // uv offset
    u32offset = ((y >> 1) * pparam->u16StripSize + (x << 5)) >> 3;
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L    ), (MS_U8)(u32offset));
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L + 1), (MS_U8)(u32offset >> 8));
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_H    ), (MS_U8)(u32offset >> 16));
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_H + 1), (MS_U8)((u32offset >> 24) & VOP_YUV_STR_HIBITS));

    pparam->u16CropWidth= (pparam->u16CropWidth >> 3) << 3;
    // HSize, VSize
    HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE    ), LOWBYTE(pparam->u16CropWidth ));
    HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE + 1), HIGHBYTE(pparam->u16CropWidth ));
    HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE    ), LOWBYTE(pparam->u16CropHeight));
    HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE + 1), HIGHBYTE(pparam->u16CropHeight ));

    HAL_WriteByteMask(SUB_REG(VOP_MPG_JPG_SWITCH), BIT0, BIT1|BIT0);

    // clear extend strip len bit by default
    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);
    if((pparam->enVideoType == MVOP_MPG) || (pparam->enVideoType == MVOP_MPEG4))
    {
        // Disable H264 or RM Input
        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH), 0, BIT2|BIT3);
        //8*32 tile format
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT1);
    }
    else if((pparam->enVideoType == MVOP_H264) || (pparam->enVideoType == MVOP_RM))
    {
        //16*32 tile format
        HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT1);
        // SVD mode enable
        HAL_WriteByteMask(SUB_REG(VOP_INPUT_SWITCH), BIT3, BIT2|BIT3);
        // set mvop to 128bit_i128 interface
        HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);
    }
    HAL_MVOP_SubLoadReg();
#endif
}

void HAL_MVOP_SubSetRepeatField(MVOP_RptFldMode eMode)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->eSubRepeatField = eMode;
}

void HAL_MVOP_SubSetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion )
{
    MS_U8 regval;
    MS_U16 u16strip, u16strip_lsb;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

#if 0
    /*****************************************************/
    // Reset MVOP setting
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x40);
    HAL_MVOP_SubSetRgbFormat(E_MVOP_RGB_NONE);
    HAL_WriteRegBit(SUB_REG(VOP_CTRL1), 0, VOP_MVD_VS_MD); //default use original vsync
    // Only for Monaco: Enable deciding bot by top address + 2
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), 1, VOP_FIELD_FROM_ADDR);

    //set MVOP test pattern to black
    HAL_MVOP_SubSetBlackBG();

    //regval = HAL_ReadByte(SUB_REG(VOP_MPG_JPG_SWITCH);
    regval = 0;
    regval |= ( mode & 0x3 );

    // clear extend strip len bit by default
    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Disable H264 or RM Input
    HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH), 0, BIT2|BIT3);
    //8*32 tile format
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 0, BIT1);
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG_HI), 0, VOP_REF_SELF_FLD);
    HAL_MVOP_SubSetFieldInverse(ENABLE, ENABLE);
#endif
    regval = 0;
    regval |= ( mode & 0x3 );

    if ( mode == VOPINPUT_HARDWIRE )
    {
        HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), regval);
    }
    else if ( mode == VOPINPUT_HARDWIRECLIP )
    {
        HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), regval);

        // HSize, VSize
        HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE    ), LOWBYTE( pparam->u16HSize ));
        HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE + 1), HIGHBYTE( pparam->u16HSize ));
        HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE    ), LOWBYTE( pparam->u16VSize ));
        HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE + 1), HIGHBYTE( pparam->u16VSize ));
    }
    else if (mode == VOPINPUT_MCUCTRL)
    {
        g_pHalMVOPCtx->bSubMirrorMode = (g_pHalMVOPCtx->bSubMirrorModeVer||g_pHalMVOPCtx->bSubMirrorModeHor);
        if ( pparam->bProgressive )
            regval |= 0x4;
        else
        {
            regval &= ~0x4;
            regval |= 0x1;  //reg_dc_md=b'11 for interlace input
            if (E_MVOP_RPTFLD_NONE == g_pHalMVOPCtx->eSubRepeatField)
            {
                MVOP_DBG("%s normal NOT repeat field %x\n", __FUNCTION__, g_pHalMVOPCtx->eSubRepeatField);
                //To support mcu mode interlace, need to set h'3B[9]=1,
                //h'11[12]=0, and Y1/UV1 address equal to Y0/UV0 address.
                HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG_HI), 1, VOP_REF_SELF_FLD);
                //HAL_MVOP_SubSetFieldInverse(ENABLE, DISABLE); //do not inverse in kano: mvop gen timing + hsk mode
                // mheg5, mcu mode for i mode, do not bypass, 0x11[6] = 0: mvop t/b toggle
                HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY_3D_1), 0, VOP_RGB_FILED_BYPASS);
                HAL_WriteRegBit(SUB_REG(VOP_CTRL0), 0, VOP_EXTFLD_EN);
            }
        }

        if ( pparam->bYUV422 )
            regval |= 0x10;
        else
            regval &= ~0x10;

        if ( pparam->b422pack )
            regval |= 0x80;

        if ( pparam->bDramRdContd == 1 )
            regval |= 0x20;
        else
            regval &= ~0x20;

        // for backward compatable to saturn
        // [3] UV-7bit mode don't care
        // [5] dram_rd_md =0
        // [6] Fld don't care
        // [7] 422pack don'care
        HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), regval);
        g_pHalMVOPCtx->bSubIs422 = pparam->bYUV422;
        HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), !pparam->bYUV422, VOP_420_BW_SAVE);
        HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), !pparam->bYUV422, VOP_420_BW_SAVE_EX);

        if (pparam->u16StripSize == 0)
        {
            if (pparam->bSD)
            {
                u16strip = 720;
                u16strip_lsb = 720;
            }
            else
            {
                u16strip = 1920;
                u16strip_lsb = 1920;
            }
        }
        else
        {
            u16strip = pparam->u16StripSize;
            u16strip_lsb = pparam->u16StripSize;
        }

        // set dc_strip[7:0]
        if (pparam->bDramRdContd == 0)
        {
            u16strip = (u16strip + 31) / 32 * 32; //need align for monaco
            u16strip = u16strip/8;
            u16strip_lsb = (u16strip_lsb+127)/128;
            u16strip_lsb *= 4;
        }
        else
        {
            if ( pparam->b422pack )
            {
                if (E_MVOP_RGB_888 == g_pHalMVOPCtx->eSubRgbFmt)
                {
                    u16strip *= 2;
                }

                if ((u16strip < 1024) || g_pHalMVOPCtx->bSubMirrorMode)
                {
                    u16strip = u16strip/4;
                    // dont extend strip len
                    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);
                }
                else
                {
                    u16strip = u16strip/8;
                    // extend strip len to 2048
                    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 1, BIT0);
                }
            }
            else
            {
                u16strip = u16strip/8;
            }
        }

        if (u16strip >= 256 )
        {
            HAL_WriteByte(SUB_REG(VOP_DC_STRIP_H), (u16strip>>8));
            //reg_dc_strip_h[2:0] = reg_dc_strip[10:8]
        }
        else
        {
            HAL_WriteByteMask(SUB_REG(VOP_DC_STRIP_H), 0, BIT0 | BIT1 | BIT2);
        }

        regval = u16strip;
        HAL_WriteByte(SUB_REG(VOP_DC_STRIP), regval);
        //LSB strip
        HAL_WriteByte(SUB_REG(VOP_DC_STRIP_LSB), u16strip_lsb & 0x3ff);

        HAL_MVOP_SubSetYUVBaseAdd(pparam->u32YOffset, pparam->u32UVOffset,
                               pparam->bProgressive, pparam->b422pack);

        if (E_MVOP_RPTFLD_NONE != g_pHalMVOPCtx->eSubRepeatField)
        {
            MVOP_DBG("%s reset eRepeatField=%x ==>", __FUNCTION__, g_pHalMVOPCtx->eSubRepeatField);
            //To output the same field for single field input,
            //do NOT set h'3B[9]=1 and h'11[12]=0
            g_pHalMVOPCtx->eSubRepeatField = E_MVOP_RPTFLD_NONE;    //reset the flag to repeat field
            MVOP_DBG(" %x\n", g_pHalMVOPCtx->eSubRepeatField);
        }

        // HSize
        MS_U16 u16HSize = ALIGN_UPTO_16(pparam->u16HSize);
        if (u16HSize != pparam->u16HSize)
        {
            MVOP_DBG(MVOP_PRINTF("\n\n Change HSize %d to %d\n", pparam->u16HSize, u16HSize);)
        }
        HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE    ), LOWBYTE( u16HSize ));
        HAL_WriteByte(SUB_REG(VOP_JPG_HSIZE + 1), HIGHBYTE( u16HSize ));

        // VSize
        MS_U16 u16VSize = pparam->u16VSize;
        if (g_pHalMVOPCtx->bSubMirrorModeVer)
        {
            u16VSize = ALIGN_UPTO_4(pparam->u16VSize);
            MVOP_DBG(MVOP_PRINTF("\n\n Change VSize %d to %d\n", pparam->u16VSize, u16VSize);)
        }
        HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE    ), LOWBYTE( u16VSize ));
        HAL_WriteByte(SUB_REG(VOP_JPG_VSIZE + 1), HIGHBYTE( u16VSize ));
    }

    HAL_MVOP_SubLoadReg();
}


void HAL_MVOP_SubEnableUVShift(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(SUB_REG(VOP_MPG_JPG_SWITCH));

    if (((regval & BIT4) == BIT4) && ((regval & 0x3)== 0x2))
    {   // 422 with MCU control mode
        if (bEnable)
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
    HAL_WriteByteMask(SUB_REG(VOP_UV_SHIFT), (bEnable)?1:0, 0x3);
}

static MS_BOOL _bSubEnable60P = false;
void HAL_MVOP_SubSetEnable60P(MS_BOOL bEnable)
{
    _bSubEnable60P = bEnable;
}

static MS_BOOL _bSubEnable4k2kClk = false;
void HAL_MVOP_SubSetEnable4k2kClk(MS_BOOL bEnable)
{
    _bSubEnable4k2kClk = bEnable;
}


void HAL_MVOP_SubSetEnable4k2k2P(MS_BOOL bEnable)
{
    g_pHalMVOPCtx->bSubIs2p = bEnable;
    HAL_WriteRegBit(SUB_REG(VOP_REG_4K2K_2P), bEnable, VOP_4K2K_2P);

}

void HAL_MVOP_SubEnableFreerunMode(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->bSubEnableFreerunMode = bEnable;
}

void HAL_MVOP_SubSetOutputTiming( MVOP_Timing *ptiming )
{
    MS_U8 regval;

    HAL_WriteByte(SUB_REG(VOP_FRAME_VCOUNT    ), LOWBYTE( ptiming->u16V_TotalCount ));
    HAL_WriteByte(SUB_REG(VOP_FRAME_VCOUNT + 1), HIGHBYTE( ptiming->u16V_TotalCount ));
    HAL_WriteByte(SUB_REG(VOP_FRAME_HCOUNT    ), LOWBYTE( ptiming->u16H_TotalCount ));
    HAL_WriteByte(SUB_REG(VOP_FRAME_HCOUNT + 1), HIGHBYTE( ptiming->u16H_TotalCount ));

    HAL_WriteByte(SUB_REG(VOP_VB0_STR     ), LOWBYTE( ptiming->u16VBlank0_Start ));
    HAL_WriteByte(SUB_REG(VOP_VB0_STR  + 1), HIGHBYTE( ptiming->u16VBlank0_Start ));
    HAL_WriteByte(SUB_REG(VOP_VB0_END     ), LOWBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte(SUB_REG(VOP_VB0_END  + 1), HIGHBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte(SUB_REG(VOP_VB1_STR     ), LOWBYTE( ptiming->u16VBlank1_Start ));
    HAL_WriteByte(SUB_REG(VOP_VB1_STR  + 1), HIGHBYTE( ptiming->u16VBlank1_Start ));
    HAL_WriteByte(SUB_REG(VOP_VB1_END     ), LOWBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte(SUB_REG(VOP_VB1_END  + 1), HIGHBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte(SUB_REG(VOP_TF_STR      ), LOWBYTE( ptiming->u16TopField_Start ));
    HAL_WriteByte(SUB_REG(VOP_TF_STR   + 1), HIGHBYTE( ptiming->u16TopField_Start ));
    HAL_WriteByte(SUB_REG(VOP_BF_STR      ), LOWBYTE( ptiming->u16BottomField_Start ));
    HAL_WriteByte(SUB_REG(VOP_BF_STR   + 1), HIGHBYTE( ptiming->u16BottomField_Start ));
    HAL_WriteByte(SUB_REG(VOP_HACT_STR    ), LOWBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte(SUB_REG(VOP_HACT_STR + 1), HIGHBYTE( ptiming->u16HActive_Start ));

    HAL_WriteByte(SUB_REG(VOP_TF_VS      ), LOWBYTE( ptiming->u16TopField_VS ));
    HAL_WriteByte(SUB_REG(VOP_TF_VS   + 1), HIGHBYTE( ptiming->u16TopField_VS ));
    HAL_WriteByte(SUB_REG(VOP_BF_VS      ), LOWBYTE( ptiming->u16BottomField_VS ));
    HAL_WriteByte(SUB_REG(VOP_BF_VS   + 1), HIGHBYTE( ptiming->u16BottomField_VS ));

    //if(((ptiming->u16V_TotalCount > 2160) || (ptiming->u16H_TotalCount > 3840)) && (ptiming->u8Framerate >= 30))
    if((!g_pHalMVOPCtx->bSubIsXcTrig) && (g_pHalMVOPCtx->bIsHS))
    {
        g_pHalMVOPCtx->bSubNewVSyncMode = TRUE;
    }

    if (g_pHalMVOPCtx->bSubNewVSyncMode)
    {
        #define SUB_NEW_VSYNC_MODE_ADVANCE_LINECNT 30
        MVOP_DBG(MVOP_PRINTF("MVOP use new vync mode, forwarding %d lines\n",SUB_NEW_VSYNC_MODE_ADVANCE_LINECNT);)

        MS_U16 u16BottomField_VS2MVD = ptiming->u16BottomField_VS - SUB_NEW_VSYNC_MODE_ADVANCE_LINECNT;
        MVOP_DBG(MVOP_PRINTF("BottomField VS ori=0x%x, new=0x%x\n", ptiming->u16BottomField_VS, u16BottomField_VS2MVD);)
        HAL_WriteByte(SUB_REG(VOP_BF_VS_MVD), LOWBYTE( u16BottomField_VS2MVD ));
        HAL_WriteByte(SUB_REG((VOP_BF_VS_MVD + 1)), HIGHBYTE( u16BottomField_VS2MVD ));

        MS_U16 u16TopField_VS2MVD = ptiming->u16V_TotalCount - SUB_NEW_VSYNC_MODE_ADVANCE_LINECNT;
        MVOP_DBG(MVOP_PRINTF("TopField VS Vtt=0x%x, new=0x%x\n", ptiming->u16V_TotalCount, u16TopField_VS2MVD);)
        HAL_WriteByte(SUB_REG(VOP_TF_VS_MVD), LOWBYTE( u16TopField_VS2MVD ));
        HAL_WriteByte(SUB_REG((VOP_TF_VS_MVD + 1)), HIGHBYTE( u16TopField_VS2MVD ));

        HAL_WriteRegBit(SUB_REG(VOP_GCLK), 0, VOP_GCLK_MIU_ON);

        HAL_WriteRegBit(SUB_REG(VOP_CTRL1), 1, VOP_MVD_VS_MD); //Use new vsync

        g_pHalMVOPCtx->bSubNewVSyncMode = FALSE; //restore to original mode
    }


    // + S3, set default IMG_HSTR, IMG_VSTR0, IMG_VSTR1
#ifdef _SUPPORT_IMG_OFFSET_
    HAL_WriteByte(SUB_REG(VOP_IMG_HSTR    ), LOWBYTE( ptiming->u16HImg_Start));
    HAL_WriteByte(SUB_REG(VOP_IMG_HSTR + 1), HIGHBYTE( ptiming->u16HImg_Start ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR0   ), LOWBYTE( ptiming->u16VImg_Start0));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR0+ 1), HIGHBYTE( ptiming->u16VImg_Start0 ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR1   ), LOWBYTE( ptiming->u16VImg_Start1 ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR1+ 1), HIGHBYTE( ptiming->u16VImg_Start1 ));
#else
    HAL_WriteByte(SUB_REG(VOP_IMG_HSTR    ), LOWBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte(SUB_REG(VOP_IMG_HSTR + 1), HIGHBYTE( ptiming->u16HActive_Start ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR0   ), LOWBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR0+ 1), HIGHBYTE( ptiming->u16VBlank0_End ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR1   ), LOWBYTE( ptiming->u16VBlank1_End ));
    HAL_WriteByte(SUB_REG(VOP_IMG_VSTR1+ 1), HIGHBYTE( ptiming->u16VBlank1_End ));
#endif
    // select mvop output from frame color(black)
    HAL_WriteByte(SUB_REG(VOP_TST_IMG + 1), 0x10);
    HAL_WriteByte(SUB_REG(VOP_U_PAT      ), 0x80);
    HAL_WriteByte(SUB_REG(VOP_U_PAT   + 1), 0x80);
    // set mvop src to test pattern
    regval = HAL_ReadByte(SUB_REG(VOP_TST_IMG));
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x02);
    // make changed registers take effect
    HAL_MVOP_SubLoadReg();

    HAL_MVOP_SubSetMIUReqMask(TRUE);
    // reset mvop to avoid timing change cause mvop hang-up
    HAL_MVOP_SubRst();
    HAL_MVOP_SubSetMIUReqMask(FALSE);

    // select mvop output from mvd
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), 0x00);
    HAL_WriteByte(SUB_REG(VOP_TST_IMG), regval);
    HAL_WriteRegBit(SUB_REG(VOP_CTRL0), ptiming->bHDuplicate, BIT2);// H pixel duplicate

#if 0
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
    MVOP_DBG(MVOP_PRINTF(" u16ExpFRate=%u #\n\n", ptiming->u16ExpFrameRate);)
#endif
}

void HAL_MVOP_SubSetDCClk(MS_U8 clkNum, MS_BOOL bEnable)
{
    MS_ASSERT( (clkNum==0) || (clkNum==1) );
    if (clkNum==0)
    {
        HAL_WriteRegBit(REG_CKG_SUB_DC0, !bEnable, CKG_SUB_DC0_GATED);
    }
}

void HAL_MVOP_SubSetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable)
{
    MS_ASSERT( (clkNum==0) || (clkNum==1) );
    if (clkNum==0)
    {
        HAL_WriteRegBit(REG_CKG_DC_SRAM, !bEnable, CKG_DC1_SRAM);
    }
}

void HAL_MVOP_SubSetSynClk(MVOP_Timing *ptiming)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if(g_pHalMVOPCtx->u32SubMVOPFixClk != 0)
    {
        HAL_MVOP_SubSetFrequency(g_pHalMVOPCtx->u32SubMVOPFixClk);
    }
    else if(g_pHalMVOPCtx->bSubEnableFreerunMode)
    {
        MS_U64 u64mpll_clock = MPLL_CLOCK << 27 ;
        do_div(u64mpll_clock, ((MS_U32)ptiming->u16H_TotalCount * (MS_U32)ptiming->u16V_TotalCount * (MS_U32)ptiming->u8Framerate));
        MS_U32 u32FreerunClk = (MS_U32)u64mpll_clock;
        HAL_MVOP_SubSetFrequency(HALMVOP_FREERUNMODE);
        HAL_WriteByte((REG_DC1_FREERUN_CW_L  ), LOWBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC1_FREERUN_CW_L+1), HIGHBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC1_FREERUN_CW_H  ), LOWBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteByte((REG_DC1_FREERUN_CW_H+1), HIGHBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteRegBit(REG_UPDATE_DC1_CW, 1, UPDATE_DC1_FREERUN_CW);
        HAL_WriteRegBit(REG_UPDATE_DC1_CW, 0, UPDATE_DC1_FREERUN_CW);
    }
    else if (_bSubEnable60P)
    {
        HAL_MVOP_SubSetFrequency(HAL_MVOP_SubGetMaxFreerunClk());
        HAL_WriteRegBit(SUB_REG(VOP_REG_FRAME_RST), 0, BIT15); // reg_frame_rst = 0

        HAL_WriteRegBit(SUB_REG(VOP_CTRL0), DISABLE, VOP_FSYNC_EN); // frame sync disable
    }
    else if (_bSubEnable4k2kClk)
    {
        HAL_MVOP_SubSetFrequency(HAL_MVOP_SubGet4k2kClk());

        HAL_WriteRegBit(SUB_REG(VOP_REG_FRAME_RST), 0, BIT15); // reg_frame_rst = 0

        HAL_WriteRegBit(SUB_REG(VOP_CTRL0), DISABLE, VOP_FSYNC_EN); // frame sync disable
    }
    else
    {
        HAL_MVOP_SubSetFrequency(HALMVOP_SYNCMODE);
        HAL_WriteByte((REG_DC1_NUM  ), LOWBYTE( ptiming->u16Num));
        HAL_WriteByte((REG_DC1_NUM+1), HIGHBYTE(ptiming->u16Num));
        HAL_WriteByte((REG_DC1_DEN  ), LOWBYTE( ptiming->u16Den));
        HAL_WriteByte((REG_DC1_DEN+1), HIGHBYTE(ptiming->u16Den));
        HAL_WriteRegBit(REG_UPDATE_DC1_CW, 1, UPDATE_DC1_SYNC_CW);
        HAL_WriteRegBit(REG_UPDATE_DC1_CW, 0, UPDATE_DC1_SYNC_CW);
    }
}


void HAL_MVOP_SubSetMonoMode(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_WriteByte(SUB_REG(VOP_U_PAT  ), 0x80);
        HAL_WriteByte(SUB_REG(VOP_U_PAT+1), 0x80);

        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH1), 1, BIT1);    // Mono mode enable
    }
    else
    {
        HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH1), 0, BIT1);    //Mono mode disable
    }
}

/******************************************************************************/
/// Set MVOP for H264  Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SubSetH264HardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), 0x00);

    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT1);

    // SVD mode enable
    HAL_WriteByteMask(SUB_REG(VOP_INPUT_SWITCH1), BIT3, BIT2|BIT3);


    // set mvop to 64bit interface
    HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);
    // Only for Monaco: Disable deciding bot by top address + 2
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), 0, VOP_FIELD_FROM_ADDR);

    // Enable mfdec setting from wb from Manhattan
    HAL_WriteRegBit(SUB_REG(VOP_REG_MFDEC_2_L), 1, VOP_MF_FROM_WB);
    HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), 1, VOP_32x32_WB); //32x32 from wb

    // Write trigger
    HAL_MVOP_SubLoadReg();
}

void HAL_MVOP_SubEnableMVDInterface(MS_BOOL bEnable)
{
    MS_BOOL bMVOPMain2MVD = TRUE;
    bMVOPMain2MVD = (bEnable) ? FALSE : TRUE;

    //This bit is only valid in main mvop bank.
    //Select which mvop interrupt that mvd f/w recieve: 1 for main; 0 for sub.
    HAL_WriteByteMask(VOP_INPUT_SWITCH1, bMVOPMain2MVD, VOP_MVD_EN);

    //No need to "Write trigger" since HAL_MVOP_SubSetInputMode() will do it later.
    //HAL_MVOP_SubLoadReg();
}

/******************************************************************************/
/// Set MVOP for RM  Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SubSetRMHardwireMode(void)
{
    HAL_MVOP_SubSetH264HardwireMode();
}

/******************************************************************************/
/// Set MVOP for JPEG Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SubSetJpegHardwireMode(void)
{
    MS_U8 regval = 0x00;

    //Hardwire mode//
    //Use HVD interface//
    HAL_MVOP_SubSetH264HardwireMode();

    regval |= 0x80; // packmode
    regval |= 0x20; // Dram Rd Contd
    regval |= 0x10; // reg_img422
    HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), regval);
    /* There is no hardwire:mvd2dc_img422/hvd2dc_img422 0x20[4] in sub mvop*/
    HAL_WriteByteMask(SUB_REG(VOP_INFO_FROM_CODEC_L), 0, VOP_INFO_FROM_CODEC_422_FMT);
    g_pHalMVOPCtx->bSubIs422 = 1;
    HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), 0, VOP_420_BW_SAVE);
    HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
    HAL_WriteByteMask(SUB_REG(VOP_INFO_FROM_CODEC_H), 0x00, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);

    // Write trigger
    HAL_MVOP_SubLoadReg();
}
/******************************************************************************/
/// Set MVOP for EVD Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SubSetEVDHardwireMode(MS_U16 u16ECOVersion)
{
    // Hardwire mode
    HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), 0x00);

    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT1);

    // EVD use HVD interface
    //HAL_WriteByteMask(SUB_REG(VOP_INPUT_SWITCH1), BIT3, BIT2|BIT3);

    // EVD mode enable
    HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // set evd flag for interlace mode setting
    g_pHalMVOPCtx->bSubIsH265 = 1;

    // Enable mfdec setting from wb
    HAL_WriteRegBit(SUB_REG(VOP_REG_MFDEC_2_L), 1, VOP_MF_FROM_WB);

    // 10 bits from wb
    HAL_WriteRegBit(SUB_REG(VOP_INFO_FROM_CODEC_H), 1, VOP_INFO_FROM_CODEC_10BIT);

    // Write trigger
    HAL_MVOP_SubLoadReg();
}

/******************************************************************************/
/// Set MVOP for VP9 Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SubSetVP9HardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(SUB_REG(VOP_MPG_JPG_SWITCH), 0x00);

    HAL_WriteRegBit(SUB_REG(VOP_REG_STRIP_ALIGN), 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), 1, BIT1);

    // EVD mode enable
    HAL_WriteRegBit(SUB_REG(VOP_INPUT_SWITCH0), 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(SUB_REG(VOP_MIU_IF), VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Enable mfdec setting from wb
    HAL_WriteRegBit(SUB_REG(VOP_REG_MFDEC_2_L), 1, VOP_MF_FROM_WB);

    // 10 bits from wb
    HAL_WriteRegBit(SUB_REG(VOP_INFO_FROM_CODEC_H), 1, VOP_INFO_FROM_CODEC_10BIT);

    // Write trigger
    HAL_MVOP_SubLoadReg();
}


///Enable 3D L/R dual buffer mode
MS_BOOL HAL_MVOP_SubEnable3DLR(MS_BOOL bEnable)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    HAL_WriteRegBit(SUB_REG(VOP_MULTI_WIN_CFG0), bEnable, VOP_LR_BUF_MODE);
    g_pHalMVOPCtx->bSub3DLRMode = bEnable;
    if(bEnable)
    {
        //only for monaco: do not wait for data ready.
        HAL_WriteByteMask(SUB_REG(VOP_NOT_WAIT_READ_DATA), 2, VOP_NOT_WAIT_RDLAT);
        HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
    }
    else
    {
        HAL_WriteByteMask(SUB_REG(VOP_NOT_WAIT_READ_DATA), 0, VOP_NOT_WAIT_RDLAT);
    }
    return TRUE;
}

///Get if 3D L/R mode is enabled
MS_BOOL HAL_MVOP_SubGet3DLRMode(void)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    return g_pHalMVOPCtx->bSub3DLRMode;
}

MS_BOOL HAL_MVOP_SubGetTimingInfoFromRegisters(MVOP_TimingInfo_FromRegisters *pMvopTimingInfo)
{
    if(NULL == pMvopTimingInfo)
    {
        MVOP_PRINTF("HAL_MVOP_SubGetTimingInfoFromRegisters():pMvopTimingInfo is NULL\n");
        return FALSE;
    }
    if(HAL_MVOP_SubGetEnableState() == FALSE)
    {
        MVOP_PRINTF("MVOP is not enabled!\n");
        pMvopTimingInfo->bEnabled = FALSE;
        return FALSE;
    }
    pMvopTimingInfo->bEnabled = TRUE;
    pMvopTimingInfo->u16H_TotalCount = (HAL_ReadByte(SUB_REG(VOP_FRAME_HCOUNT + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_FRAME_HCOUNT)));
    pMvopTimingInfo->u16V_TotalCount = (HAL_ReadByte(SUB_REG(VOP_FRAME_VCOUNT + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_FRAME_VCOUNT)));
    pMvopTimingInfo->u16VBlank0_Start = (HAL_ReadByte(SUB_REG(VOP_VB0_STR + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_VB0_STR)));
    pMvopTimingInfo->u16VBlank0_End = (HAL_ReadByte(SUB_REG(VOP_VB0_END + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_VB0_END)));
    pMvopTimingInfo->u16VBlank1_Start = (HAL_ReadByte(SUB_REG(VOP_VB1_STR + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_VB1_STR)));
    pMvopTimingInfo->u16VBlank1_End = (HAL_ReadByte(SUB_REG(VOP_VB1_END + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_VB1_END)));
    pMvopTimingInfo->u16TopField_Start = (HAL_ReadByte(SUB_REG(VOP_TF_STR + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_TF_STR)));
    pMvopTimingInfo->u16BottomField_Start = (HAL_ReadByte(SUB_REG(VOP_BF_STR + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_BF_STR)));
    pMvopTimingInfo->u16HActive_Start = (HAL_ReadByte(SUB_REG(VOP_HACT_STR + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_HACT_STR)));
    pMvopTimingInfo->u16TopField_VS = (HAL_ReadByte(SUB_REG(VOP_TF_VS + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_TF_VS)));
    pMvopTimingInfo->u16BottomField_VS = (HAL_ReadByte(SUB_REG(VOP_BF_VS + 1)<< 8)) | (HAL_ReadByte(SUB_REG(VOP_BF_VS)));
    pMvopTimingInfo->bInterlace = (HAL_ReadRegBit(SUB_REG(VOP_CTRL0), BIT7) == BIT7);
    return TRUE;
}

void HAL_MVOP_SubSetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
    MS_PHY u64tmp = 0;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    // Y offset
    u64tmp = u32YOffset >> 3;
    if ( !bProgressive )
    {   //Refine Y offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eSubRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eSubRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eSubRepeatField);
        }
    }
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L), u64tmp & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L+2), (u64tmp >> 16) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if (!bProgressive )
    {  //Y offset of bottom field if interlace
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L+2), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    if (b422pack)
    {
        if (HAL_ReadRegBit(SUB_REG(VOP_MIU_IF), VOP_MIU_128B_I64) != VOP_MIU_128B_I64) //128-bit
        {
            u32UVOffset = u32YOffset + 16; //add 16 for 128bit; add 8 for 64bit
        }
        else    //64-bit
        {
            u32UVOffset = u32YOffset + 8; //add 16 for 128bit; add 8 for 64bit
        }
    }
        // UV offset
    u64tmp = u32UVOffset >> 3;
    if( !bProgressive )
    {  //Refine UV offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eSubRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eSubRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eSubRepeatField);
        }
    }
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L), u64tmp & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L+2), (u64tmp >> 16) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if( !bProgressive )
    {  //UV offset of bottom field if interlace
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L+2), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L+3), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    return;
}

MS_PHY HAL_MVOP_SubGetYBaseAdd(void)
{
    MS_PHY u64YOffset;
    u64YOffset = HAL_ReadByte(SUB_REG(VOP_JPG_YSTR0_L))&0xff;
    u64YOffset |=((HAL_ReadByte(SUB_REG(VOP_JPG_YSTR0_L+1))<<8)&0xff00);
    u64YOffset |=((HAL_ReadByte(SUB_REG(VOP_JPG_YSTR0_L+2))<<16)&0xff0000);
    u64YOffset |= ((HAL_ReadByte(SUB_REG(VOP_JPG_YSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u64YOffset;
}

MS_PHY HAL_MVOP_SubGetUVBaseAdd(void)
{
    MS_PHY u64UVOffset;
    u64UVOffset = HAL_ReadByte(SUB_REG(VOP_JPG_UVSTR0_L))&0xff;
    u64UVOffset |=((HAL_ReadByte(SUB_REG(VOP_JPG_UVSTR0_L+1))<<8)&0xff00);
    u64UVOffset |=((HAL_ReadByte(SUB_REG(VOP_JPG_UVSTR0_L+2))<<16)&0xff0000);
    u64UVOffset |= ((HAL_ReadByte(SUB_REG(VOP_JPG_UVSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u64UVOffset;
}

/******************************************************************************/
/// Set MVOP Saving BW Mode
/// @ Napoli this command should be set after MDrv_MVOP_SubSetOutputCfg
/******************************************************************************/
MS_BOOL HAL_MVOP_SubSet420BWSaveMode(MS_BOOL bEnable, MS_U16 u16ECOVersion)
{
MS_BOOL bValue = FALSE;

    UNUSED(u16ECOVersion);
    //hw limtation: 3DLA/3DSBS/422/p mode in, i mode out/i mode in, p mode out(only need to check in MCU mode)
    bValue = (g_pHalMVOPCtx->bSub3DLRAltSBSOutput || g_pHalMVOPCtx->bSub3DLRAltOutput/* || g_pHalMVOPCtx->bSub3DLRMode */|| g_pHalMVOPCtx->bSubIs422 );

    if(bValue)
    {
        MVOP_DBG(MVOP_PRINTF("%s Hit the limitation of saving bw, disable BW Saving mode\n", __FUNCTION__);)
        HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), 0, VOP_420_BW_SAVE);
        HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
        HAL_MVOP_SubLoadReg();
        return FALSE;
    }
    else
    {
        HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), bEnable, VOP_420_BW_SAVE);
        if( g_pHalMVOPCtx->bSub3DLRMode == FALSE)
            HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), bEnable, VOP_420_BW_SAVE_EX);
        else
            HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
        HAL_MVOP_SubLoadReg();
        return TRUE;
    }
}

MS_BOOL HAL_MVOP_SubSetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo)
{
    //----------------------------------------------------
    // Set MSB YUV Address
    //----------------------------------------------------

    MS_PHY u64tmp = 0;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    // Y offset
    u64tmp = stEVDBaseAddInfo->u32MSBYOffset >> 3;
    if ( !stEVDBaseAddInfo->bProgressive)
    {   //Refine Y offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L), u64tmp & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_L+1), (u64tmp >> 8) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_H), (u64tmp >> 16) & 0xff);
    HAL_WriteByte(SUB_REG(VOP_JPG_YSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if (!stEVDBaseAddInfo->bProgressive )
    {   //Y offset of bottom field if interlace
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_YSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    if (stEVDBaseAddInfo->b422Pack)
    {
        stEVDBaseAddInfo->u32MSBUVOffset = stEVDBaseAddInfo->u32MSBYOffset + 16; //add 16 for 128bit; add 8 for 64bit
    }
    // UV offset
    u64tmp = stEVDBaseAddInfo->u32MSBUVOffset >> 3;
    if( !stEVDBaseAddInfo->bProgressive )
    {  //Refine UV offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u64tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

    if( !stEVDBaseAddInfo->bProgressive )
    {  //UV offset of bottom field if interlace
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_JPG_UVSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    //----------------------------------------------------
    // Set MSB YUV Address
    //----------------------------------------------------
    if(stEVDBaseAddInfo->bEnLSB)
    {
        //Enable LSB
        HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 1, VOP_EVD_10B_Y_EN);
        HAL_WriteRegBit(SUB_REG(VOP_EVD_10B_EN), 1, VOP_EVD_10B_UV_EN);
        HAL_WriteRegBit(SUB_REG(VOP_REG_MASK), 0, VOP_LSB_REQ_MASK);

        // Y offset
        u64tmp = stEVDBaseAddInfo->u32LSBYOffset >> 3;
        if ( !stEVDBaseAddInfo->bProgressive)
        {   //Refine Y offset for interlace repeat bottom field
            if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
            {
                MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
                u64tmp += 2;
            }
            else
            {
                MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            }
        }
        HAL_WriteByte(SUB_REG(VOP_LSB_YSTR0_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_YSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_YSTR0_L), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_YSTR0_L+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

        if (!stEVDBaseAddInfo->bProgressive )
        {   //Y offset of bottom field if interlace
            HAL_WriteByte(SUB_REG(VOP_LSB_YSTR1_L), u64tmp & 0xff);
            HAL_WriteByte(SUB_REG(VOP_LSB_YSTR1_L+1), (u64tmp >> 8) & 0xff);
            HAL_WriteByte(SUB_REG(VOP_LSB_YSTR1_H), (u64tmp >> 16) & 0xff);
            HAL_WriteByte(SUB_REG(VOP_LSB_YSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
        }

        if (stEVDBaseAddInfo->b422Pack)
        {
            stEVDBaseAddInfo->u32LSBUVOffset = stEVDBaseAddInfo->u32LSBYOffset + 16; //add 16 for 128bit; add 8 for 64bit
        }
        // UV offset
        u64tmp = stEVDBaseAddInfo->u32LSBUVOffset >> 3;
        if( !stEVDBaseAddInfo->bProgressive )
        {  //Refine UV offset for interlace repeat bottom field
            if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
            {
                MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
                u64tmp += 2;
            }
            else
            {
                MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            }
        }
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR0_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR0_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR0_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR0_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);

        if( !stEVDBaseAddInfo->bProgressive )
        {  //UV offset of bottom field if interlace
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR1_L), u64tmp & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR1_L+1), (u64tmp >> 8) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR1_H), (u64tmp >> 16) & 0xff);
        HAL_WriteByte(SUB_REG(VOP_LSB_UVSTR1_H+1), (u64tmp >> 24) & VOP_YUV_STR_HIBITS);
        }
    }

    return TRUE;
}

MS_BOOL HAL_MVOP_SubCheckSTCCW(void)
{
    MS_U16 u16STC_CW_L = 0;
    MS_U16 u16STC_CW_H = 0;

    MS_BOOL u16STC_CW_SEL = 0;
    MS_BOOL u16TSP_CLK_EN = 0;

    u16STC_CW_L = HAL_Read2Byte(REG_STC1_CW_L)&0xffff;
    u16STC_CW_H = HAL_Read2Byte(REG_STC1_CW_H)&0xffff;

    u16STC_CW_SEL = (HAL_ReadRegBit(REG_STC_CW_SLE_H, BIT1) == BIT1);
    u16TSP_CLK_EN = !(HAL_ReadRegBit(REG_TSP_CLK, BIT0) == BIT0);

    if((((u16STC_CW_L || u16STC_CW_H) == 0) && (u16STC_CW_SEL == 0)) || ((u16STC_CW_SEL == 1) && (u16TSP_CLK_EN == 0)))
        return FALSE;
    else
        return TRUE;

}

void HAL_MVOP_SubSetVSyncMode(MS_U8 u8Mode)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if (1==u8Mode)
    {
        g_pHalMVOPCtx->bSubNewVSyncMode = TRUE;
    }
    else
    {
        g_pHalMVOPCtx->bSubNewVSyncMode = FALSE;
    }
}

MS_BOOL HAL_MVOP_SubSet3DLRAltOutput_VHalfScaling(MS_BOOL bEnable)
{
    MS_BOOL bEnDualBuff = bEnable ? ENABLE : DISABLE;     //enable dual buffer
    MS_BOOL bEnSWDualBuff = bEnable ? DISABLE : ENABLE;   //buffer controlled by HK instead of FW
    MS_BOOL bEnMirrMaskBase = bEnable ? DISABLE : ENABLE; //do not mask LSB
    MS_BOOL bEnHwFldBase = bEnable ? DISABLE : ENABLE;    //hardware calculate field jump base address
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    //Set 0x27[2] = 1 (enable SW dual buffer mode)
    HAL_WriteRegBit(SUB_REG(VOP_REG_WR), bEnDualBuff, VOP_BUF_DUAL);

    //Set 0x38[8] = 0 (use SW dual buffer mode)
    HAL_WriteRegBit(SUB_REG(VOP_INFO_FROM_CODEC_H), bEnSWDualBuff, VOP_INFO_FROM_CODEC_DUAL_BUFF);

    //Set 0x3b[7] = 0 (use MVD/HVD firmware send base)
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), bEnMirrMaskBase, VOP_MASK_BASE_LSB);

    //Set 0x3b[5] = 0 (hardware calculate field jump base address)
    HAL_WriteRegBit(SUB_REG(VOP_MIRROR_CFG), bEnHwFldBase, VOP_HW_FLD_BASE);
    g_pHalMVOPCtx->bSub3DLRAltOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_SubSet3DLRAltOutput(MS_BOOL bEnable)
{
    //Set 0x3C[2] = 1 (enable 3D L/R dual buffer line alternative output)
    //it works when 0x3C[0] = 1
    HAL_WriteRegBit(SUB_REG(VOP_MULTI_WIN_CFG0), bEnable, VOP_LR_LA_OUT);
    // bw saving not support: LA/SBS
    HAL_WriteRegBit(SUB_REG(VOP_REG_DUMMY), 0, VOP_420_BW_SAVE);
    HAL_WriteRegBit(SUB_REG(VOP_REG_BW_SAVE), 0, VOP_420_BW_SAVE_EX);
    HAL_MVOP_LoadReg();

    g_pHalMVOPCtx->bSub3DLRAltOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_SubSet3DLRAltSBSOutput(MS_BOOL bEnable)
{
    //it works when 0x3C[0] = 1 and 0x3C[2] = 1
    //Set 0x3C[3] = 1 (3D L/R line alternative read, side-by-side output)
    HAL_WriteRegBit(SUB_REG(VOP_MULTI_WIN_CFG0), bEnable, VOP_LR_LA2SBS_OUT);
    g_pHalMVOPCtx->bSub3DLRAltSBSOutput = bEnable;
    return TRUE;
}

MS_BOOL HAL_MVOP_SubGet3DLRAltOutput(void)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    return g_pHalMVOPCtx->bSub3DLRAltOutput;
}

MS_BOOL HAL_MVOP_SubGet3DLRAltSBSOutput(void)
{
    return g_pHalMVOPCtx->bSub3DLRAltSBSOutput;
}

EN_MVOP_Output_3D_TYPE HAL_MVOP_SubGetOutput3DType(void)
{
    EN_MVOP_Output_3D_TYPE en3DType = E_MVOP_OUTPUT_3D_NONE;
    if(g_pHalMVOPCtx->bSub3DLRMode)
    {
        if(g_pHalMVOPCtx->bSub3DLRAltSBSOutput)
        {
            en3DType = E_MVOP_OUTPUT_3D_SBS;
        }
        else
        {
            en3DType = E_MVOP_OUTPUT_3D_TB;
        }
    }
    else if(g_pHalMVOPCtx->bSub3DLRAltOutput)
    {
        en3DType = E_MVOP_OUTPUT_3D_LA;
    }
    return en3DType;
}


#endif


#define MVOP_INT_UF BIT0
#define MVOP_INT_OF BIT1
#define MVOP_INT_VSYNC BIT2
#define MVOP_INT_HSYNC BIT3
#define MVOP_INT_RDY   BIT4
#define MVOP_INT_FLD   BIT5
#define MVOP_INT_ALL (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

const MS_U16 u16MvopRegBase[2] = { MVOP_REG_BASE, MVOP_SUB_REG_BASE};
#define MAP_REG(_id, _reg)        ((_reg) - MVOP_REG_BASE + u16MvopRegBase[(_id)])

MS_BOOL HAL_MVOP_IntEnableMask(MVOP_DevID eID, MS_U8 eIntType)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16Reg = MAP_REG( eID, VOP_INT_MASK);
    MS_U8 u8Mask = 0;

    u8Mask = HAL_ReadByte(u16Reg);

    if (E_MVOP_INT_NONE != eIntType)
    {
        u8Mask = (E_MVOP_INT_VSYNC == (eIntType&E_MVOP_INT_VSYNC)) ?
                 (u8Mask & ~MVOP_INT_VSYNC) : (u8Mask);
        u8Mask = (E_MVOP_INT_HSYNC == (eIntType&E_MVOP_INT_HSYNC)) ?
                 (u8Mask & ~MVOP_INT_HSYNC) : (u8Mask);
        u8Mask = (E_MVOP_INT_FDCHNG == (eIntType&E_MVOP_INT_FDCHNG)) ?
                 (u8Mask & ~MVOP_INT_FLD) : (u8Mask);
        u8Mask = (E_MVOP_INT_RDY == (eIntType&E_MVOP_INT_RDY)) ?
                 (u8Mask & ~MVOP_INT_RDY) : (u8Mask);
        u8Mask = (E_MVOP_INT_BUFF_UF == (eIntType&E_MVOP_INT_BUFF_UF)) ?
                 (u8Mask & ~MVOP_INT_UF) : (u8Mask);
        u8Mask = (E_MVOP_INT_BUFF_OF == (eIntType&E_MVOP_INT_BUFF_OF)) ?
                 (u8Mask & ~MVOP_INT_OF) : (u8Mask);
    }
    else    //mask all
    {
        u8Mask |= MVOP_INT_ALL;
    }

    //MVOP_PRINTF("u8Mask %x ", u8Mask);
    HAL_WriteByteMask(u16Reg, u8Mask, MVOP_INT_ALL);
    //u8Mask = HAL_ReadByte(u16Reg);
    //MVOP_PRINTF("==> %x \n", u8Mask);
    return bRet;
}

MS_U8 HAL_MVOP_IntGetStatus(MVOP_DevID eID)
{
    MS_U8 u8IntVal = 0;
    MS_U8 u8IntType = E_MVOP_INT_NONE;
    MS_U16 u16Reg = MAP_REG(eID, (VOP_INT_MASK+1));

    u8IntVal = HAL_ReadByte(u16Reg) & MVOP_INT_ALL;
    //MVOP_PRINTF("u8IntVal %x\n", u8IntVal);
    if ((u8IntVal & MVOP_INT_VSYNC) == MVOP_INT_VSYNC)
    {
        u8IntType |= E_MVOP_INT_VSYNC;
    }
    if ((u8IntVal & MVOP_INT_HSYNC) == MVOP_INT_HSYNC)
    {
        u8IntType |= E_MVOP_INT_HSYNC;
    }
    if ((u8IntVal & MVOP_INT_FLD) == MVOP_INT_FLD)
    {
        u8IntType |= E_MVOP_INT_FDCHNG;
    }
    if ((u8IntVal & MVOP_INT_RDY) == MVOP_INT_RDY)
    {
        u8IntType |= E_MVOP_INT_RDY;
    }
    if ((u8IntVal & MVOP_INT_UF) == MVOP_INT_UF)
    {
        u8IntType |= E_MVOP_INT_BUFF_UF;
    }
    if ((u8IntVal & MVOP_INT_OF) == MVOP_INT_OF)
    {
        u8IntType |= E_MVOP_INT_BUFF_OF;
    }
    return u8IntType;
}

#if 0
void HAL_MVOP_SetStartX(MS_U16 u16Length)
{
    if(g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->g_u16SetStartX = u16Length;
    HAL_WriteByte(VOP_REG_CROP_HSTART, u16Length & 0xff);
    HAL_WriteByte((VOP_REG_CROP_HSTART + 1),((u16Length) >> (8)) & (0x1f));
    // Write trigger
    HAL_MVOP_LoadReg();
}


void HAL_MVOP_SetStartY(MS_U16 u16Length, MS_BOOL bIs4Align)
{
    if(g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    g_pHalMVOPCtx->g_u16SetStartY = u16Length;
    g_pHalMVOPCtx->g_bIsY4Align = bIs4Align;
    HAL_WriteByte(VOP_REG_CROP_VSTART, u16Length & 0xff);
    HAL_WriteByte((VOP_REG_CROP_VSTART + 1), ((u16Length) >> (8)) & (0x1f));
    // Write trigger
    HAL_MVOP_LoadReg();
}


void HAL_MVOP_SetPicWidthMinus(MS_U16 u16Sizes)
{
    if(g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if(u16Sizes == 0)
    {
        HAL_WriteByte(VOP_REG_CROP_HSIZE, u16Sizes);
        HAL_WriteByte((VOP_REG_CROP_HSIZE + 1), u16Sizes);
    }
    else
    {
        u16Sizes = u16Sizes + 1 - g_pHalMVOPCtx->g_u16SetStartX ;
        HAL_WriteByte(VOP_REG_CROP_HSIZE, u16Sizes & 0xff);
        HAL_WriteByte((VOP_REG_CROP_HSIZE + 1), ((u16Sizes) >> (8)) & (0x1f));
    }
    g_pHalMVOPCtx->g_u16SetXSize= u16Sizes;
    // Write trigger
    HAL_MVOP_LoadReg();
}

void HAL_MVOP_SetPicHeightMinus(MS_U16 u16Sizes)
{
    MS_U32 u32YOffset=0;
    MS_U32 u32UVOffset=0;
    MS_BOOL bIsInterlace = 0;

    if(g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }

    if(u16Sizes == 0)
    {
        HAL_WriteByte(VOP_REG_CROP_VSIZE, u16Sizes);
        HAL_WriteByte((VOP_REG_CROP_VSIZE + 1), u16Sizes);
    }
    else
    {
        u16Sizes = u16Sizes + 1 - g_pHalMVOPCtx->g_u16SetStartY ;
        if(g_pHalMVOPCtx->g_bIsY4Align)
            u16Sizes -= 2;

        HAL_WriteByte(VOP_REG_CROP_VSIZE, (u16Sizes) & 0xff);
        HAL_WriteByte((VOP_REG_CROP_VSIZE + 1), ((u16Sizes) >> (8)) & (0x1f));
    }
    g_pHalMVOPCtx->g_u16SetYSize= u16Sizes;
    //***********************************************************************************//
    //@clippers crop sw patch
    // Synchronization: video broken, looks like pitch incorrect.
    // Hit condition: (g_pHalMVOPCtx->g_u16SetStartX%16)+(g_pHalMVOPCtx->g_u16SetXSize%16) > 16
    // Fix: after monet
    // Limitation: have to set MDrv_MVOP_SetImageWidthHight after:
    // MDrv_MVOP_SetInputCfg / MDrv_MVOP_SetOutputCfg / MDrv_MVOP_SetBaseAdd / MDrv_MVOP_SetStartPos
    //***********************************************************************************//
    if((g_pHalMVOPCtx->g_u16SetXSize == 0) && (g_pHalMVOPCtx->g_u16SetYSize == 0))
    {
        HAL_WriteRegBit(VOP_REG_SINGLE_3D_L, 0, VOP_FORCE_SKIP);
        HAL_WriteByteMask(VOP_REG_SINGLE_3D_L, 0, VOP_SKIP_SIZE_LVIEW);
        HAL_WriteByteMask(VOP_REG_SINGLE_3D_H, 0, VOP_SKIP_SIZE_RVIEW);
        HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_LVIEW);
        HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 0, VOP_SKIP_RVIEW);
        if(g_pHalMVOPCtx->g_bIsSetCrop == TRUE)
        {
            u32YOffset = HAL_MVOP_GetYBaseAdd() +4;
            u32UVOffset = HAL_MVOP_GetUVBaseAdd() +4;
            bIsInterlace = HAL_ReadRegBit(VOP_CTRL0, BIT7);
            HAL_MVOP_SetYUVBaseAdd(u32YOffset<<3, u32UVOffset<<3, !bIsInterlace,g_pHalMVOPCtx->bIs422);
            g_pHalMVOPCtx->g_bIsSetCrop = FALSE;
        }
        HAL_MVOP_LoadReg();
        return;
    }
    MVOP_DBG("[Warning] clippers crop may hit hw bug: g_u16SetStartX = %d, g_u16SetXSize = %d\n",g_pHalMVOPCtx->g_u16SetStartX,g_pHalMVOPCtx->g_u16SetXSize);
    if(((g_pHalMVOPCtx->g_u16SetStartX%16)+(g_pHalMVOPCtx->g_u16SetXSize%16) > 16) && (g_pHalMVOPCtx->bIs422))
    {
        HAL_WriteRegBit(VOP_REG_SINGLE_3D_L, 1, VOP_FORCE_SKIP);
        HAL_WriteByteMask(VOP_REG_SINGLE_3D_L, (g_pHalMVOPCtx->g_u16SetStartX%16), VOP_SKIP_SIZE_LVIEW);
        HAL_WriteByteMask(VOP_REG_SINGLE_3D_H, (g_pHalMVOPCtx->g_u16SetStartX%16), VOP_SKIP_SIZE_RVIEW);
        HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 1, VOP_SKIP_LVIEW);
        HAL_WriteRegBit(VOP_REG_ENABLE_SKIP, 1, VOP_SKIP_RVIEW);
        if(g_pHalMVOPCtx->g_bIsSetCrop == FALSE)
        {
            u32YOffset = HAL_MVOP_GetYBaseAdd();
            u32UVOffset = HAL_MVOP_GetUVBaseAdd();
            if((u32YOffset < 4) || (u32UVOffset < 4))
                return;

            u32YOffset -= 4;
            u32UVOffset -= 4;
            bIsInterlace = HAL_ReadRegBit(VOP_CTRL0, BIT7);
            HAL_MVOP_SetYUVBaseAdd(u32YOffset<<3, u32UVOffset<<3, !bIsInterlace,g_pHalMVOPCtx->bIs422);
            g_pHalMVOPCtx->g_bIsSetCrop = TRUE;
        }
    }
    // Write trigger
    HAL_MVOP_LoadReg();
}
#endif
void HAL_MVOP_MaskDBRegCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPMSAKDBREGCTRL eModeCtrl)
{
    MS_U8 regval;
    MS_U8 u8Arg;

    if (eModeCtrl> E_MVOP_INV_OP_VS)
    {
        return;
    }
    #if 0
    else if (eModeCtrl >= E_MVOP_CHKSUM_422_SWAP)
    {
        regval = HAL_ReadByte(MVOP_MASK_DB_REG1);
    }
    else
    {
        regval = HAL_ReadByte(MVOP_MASK_DB_REG0);
    }
    #endif

    switch(eID)
    {
        case E_MVOP_DEV_0:
        {
            switch(eModeCtrl)
            {
                case E_MVOP_MASK_DB_REG:
                    //u8Arg = MVOP_MASK_DB_REG;
                    break;
                case E_MVOP_EN_DB_REG:
                    //u8Arg = MVOP_EN_DB_REG;
                    break;
                case E_MVOP_DEBUG_SEL:
                    //u8Arg = MVOP_DEBUG_SEL;
                    break;
                case E_MVOP_UPDATE_SEL:
                    //u8Arg = MVOP_UPDATE_SEL;
                    break;
                case E_MVOP_RST_START_SEL:
                    //u8Arg = MVOP_RST_START_SEL;
                    break;
                case E_MVOP_SC_VSYNC_SEL:
                    //u8Arg = MVOP_SC_VSYNC_SEL;
                    break;
                case E_MVOP_READ_IP_BASE_SEL:
                    //u8Arg = MVOP_READ_IP_BASE_SEL;
                    break;
                case E_MVOP_CHKSUM_422_SWAP:
                    //u8Arg = MVOP_CHKSUM_422_SWAP;
                    break;
                case E_MVOP_CHKSUM_FULL_C:
                    //u8Arg = MVOP_CHKSUM_FULL_C;
                    break;
                case E_MVOP_OFF_LATCH_CRC:
                    //u8Arg = MVOP_OFF_LATCH_CRC;
                    break;
                case E_MVOP_SEL_OP_FIELD:
                {
                    u8Arg = VOP_MVD_VS_MD;
                    regval = HAL_ReadByte(VOP_CTRL0);
                    if ( bEnable )
                    {
                        regval &= ~u8Arg;
                    }
                    else
                    {
                        regval |= u8Arg;
                    }
                    HAL_WriteByte(VOP_CTRL0, regval);

                    u8Arg = VOP_RGB_FILED_BYPASS;
                    regval = HAL_ReadByte(VOP_REG_DUMMY_3D_1);
                    if ( bEnable )
                    {
                        regval &= ~u8Arg;
                    }
                    else
                    {
                        regval |= u8Arg;
                    }
                    HAL_WriteByte(VOP_REG_DUMMY_3D_1, regval);
                    //xc use bob mode(only use one field de-interlace)
                    // xc receive top/bot toggle signal, but only one field
                    #if 0
                    // xc receive top/bot toggle signal, but only one field
                    u8Arg = VOP_REF_SELF_FLD;
                    regval = HAL_ReadByte(VOP_MIRROR_CFG_HI);
                    if ( bEnable )
                    {
                        regval |= u8Arg;
                    }
                    else
                    {
                        regval &= ~u8Arg;
                    }
                    HAL_WriteByte(VOP_MIRROR_CFG_HI, regval);
                    //if hardwired mode
                    if(BIT2 != HAL_ReadRegBit(VOP_MPG_JPG_SWITCH, BIT2))
                    {
                        if ( bEnable )
                        {
                            HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 0, VOP_INFO_FROM_CODEC_FIELD);
                        }
                        else
                        {
                            HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 1, VOP_INFO_FROM_CODEC_FIELD);
                        }
                    }
                    #endif
                    HAL_MVOP_LoadReg();
                    break;
                }
                case E_MVOP_INV_OP_FIELD:
                    //u8Arg = MVOP_INV_OP_FIELD;
                    break;
                case E_MVOP_INV_OP_VS:
                    //u8Arg = MVOP_INV_OP_VS;
                    break;
                default:
                    //u8Arg = 0x0;
                    break;

            }
            break;
        }
        case E_MVOP_DEV_1:
        {
            switch(eModeCtrl)
            {
                case E_MVOP_MASK_DB_REG:
                    //u8Arg = MVOP_MASK_DB_REG;
                    break;
                case E_MVOP_EN_DB_REG:
                    //u8Arg = MVOP_EN_DB_REG;
                    break;
                case E_MVOP_DEBUG_SEL:
                    //u8Arg = MVOP_DEBUG_SEL;
                    break;
                case E_MVOP_UPDATE_SEL:
                    //u8Arg = MVOP_UPDATE_SEL;
                    break;
                case E_MVOP_RST_START_SEL:
                    //u8Arg = MVOP_RST_START_SEL;
                    break;
                case E_MVOP_SC_VSYNC_SEL:
                    //u8Arg = MVOP_SC_VSYNC_SEL;
                    break;
                case E_MVOP_READ_IP_BASE_SEL:
                    //u8Arg = MVOP_READ_IP_BASE_SEL;
                    break;
                case E_MVOP_CHKSUM_422_SWAP:
                    //u8Arg = MVOP_CHKSUM_422_SWAP;
                    break;
                case E_MVOP_CHKSUM_FULL_C:
                    //u8Arg = MVOP_CHKSUM_FULL_C;
                    break;
                case E_MVOP_OFF_LATCH_CRC:
                    //u8Arg = MVOP_OFF_LATCH_CRC;
                    break;
                case E_MVOP_SEL_OP_FIELD:
                {
                    u8Arg = VOP_MVD_VS_MD;
                    regval = HAL_ReadByte(SUB_REG(VOP_CTRL0));
                    if ( bEnable )
                    {
                        regval &= ~u8Arg;
                    }
                    else
                    {
                        regval |= u8Arg;
                    }
                    HAL_WriteByte(SUB_REG(VOP_CTRL0), regval);

                    u8Arg = VOP_RGB_FILED_BYPASS;
                    regval = HAL_ReadByte(SUB_REG(VOP_REG_DUMMY_3D_1));
                    if ( bEnable )
                    {
                        regval &= ~u8Arg;
                    }
                    else
                    {
                        regval |= u8Arg;
                    }
                    HAL_WriteByte(SUB_REG(VOP_REG_DUMMY_3D_1), regval);
                    //xc use bob mode(only use one field de-interlace)
                    // xc receive top/bot toggle signal, but only one field
                    #if 0
                    // xc receive top/bot toggle signal, but only one field
                    u8Arg = VOP_REF_SELF_FLD;
                    regval = HAL_ReadByte(VOP_MIRROR_CFG_HI);
                    if ( bEnable )
                    {
                        regval |= u8Arg;
                    }
                    else
                    {
                        regval &= ~u8Arg;
                    }
                    HAL_WriteByte(VOP_MIRROR_CFG_HI, regval);
                    //if hardwired mode
                    if(BIT2 != HAL_ReadRegBit(VOP_MPG_JPG_SWITCH, BIT2))
                    {
                        if ( bEnable )
                        {
                            HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 0, VOP_INFO_FROM_CODEC_FIELD);
                        }
                        else
                        {
                            HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 1, VOP_INFO_FROM_CODEC_FIELD);
                        }
                    }
                    #endif
                    HAL_MVOP_SubLoadReg();
                    break;
                }
                case E_MVOP_INV_OP_FIELD:
                    //u8Arg = MVOP_INV_OP_FIELD;
                    break;
                case E_MVOP_INV_OP_VS:
                    //u8Arg = MVOP_INV_OP_VS;
                    break;
                default:
                    //u8Arg = 0x0;
                    break;

            }
            break;
        }
        default:
            break;
    }
    #if 0
    if ( bEnable )
    {
        regval |= u8Arg;
    }
    else
    {
        regval &= ~u8Arg;
    }
    if (eModeCtrl >= E_MVOP_CHKSUM_422_SWAP)
    {
        HAL_WriteByte(MVOP_MASK_DB_REG1, regval);
    }
    else
    {
        HAL_WriteByte(MVOP_MASK_DB_REG0, regval);
    }
    #endif


}


 //patch for Kappa
void HAL_MVOP_SetDeb2MVDFrameModeCtrl(MVOP_DevID eID, MS_BOOL bEnable, MVOPDEB2MVDFRAMECTRL eModeCtrl)
{
    MS_U8 regval;
    MS_U8 u8Arg;

    switch(eID)
    {
        case E_MVOP_DEV_0:
        {
            regval = HAL_ReadByte(VOP_INFO_FROM_CODEC_L);
            switch (eModeCtrl)
            {
                case E_MVOP_DEB2MVD_FRAME_MODE:
                    u8Arg = 0x0;
                    break;
                case E_MVOP_DEB2MVD_FIELD_INV:
                    u8Arg = 0x0;
                    break;
                case E_MVOP_SIZE_FROM_MVD:
                    u8Arg = VOP_INFO_FROM_CODEC_SIZE;
                    if (bEnable)
                    {
                        //crop = 0, then size could be from vdec.
                        HAL_MVOP_SetPicWidthMinus(E_MVOP_DEV_0, 0, 0);
                        HAL_MVOP_SetPicHeightMinus(E_MVOP_DEV_0, 0, 0);
                    }
                    break;
                case E_MVOP_SEQ_FROM_MVD:
                    u8Arg = 0x0;
                    break;
                default:
                    u8Arg = 0x0;
                    break;
            }

            if (bEnable)
            {
                regval |= u8Arg;
            }
            else
            {
                regval &= ~u8Arg;
            }

            HAL_WriteByte(VOP_INFO_FROM_CODEC_L, regval);

            // Write trigger
            HAL_MVOP_LoadReg();
        }
        break;
        case E_MVOP_DEV_1:
        {
            regval = HAL_ReadByte(SUB_REG(VOP_INFO_FROM_CODEC_L));
            switch (eModeCtrl)
            {
                case E_MVOP_DEB2MVD_FRAME_MODE:
                    u8Arg = 0x0;
                    break;
                case E_MVOP_DEB2MVD_FIELD_INV:
                    u8Arg = 0x0;
                    break;
                case E_MVOP_SIZE_FROM_MVD:
                    u8Arg = VOP_INFO_FROM_CODEC_SIZE;
                    if (bEnable)
                    {
                        //crop = 0, then size could be from vdec.
                        HAL_MVOP_SetPicWidthMinus(SUB_REG(E_MVOP_DEV_0), 0, 0);
                        HAL_MVOP_SetPicHeightMinus(SUB_REG(E_MVOP_DEV_0), 0, 0);
                    }
                    break;
                case E_MVOP_SEQ_FROM_MVD:
                    u8Arg = 0x0;
                    break;
                default:
                    u8Arg = 0x0;
                    break;
            }

            if (bEnable)
            {
                regval |= u8Arg;
            }
            else
            {
                regval &= ~u8Arg;
            }

            HAL_WriteByte(SUB_REG(VOP_INFO_FROM_CODEC_L), regval);

            // Write trigger
            HAL_MVOP_SubLoadReg();
        }
        break;
        default:
            break;
    }

}

// for kappa
void HAL_MVOP_BaseFromIPCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPBASEFROMIPCTRL eModeCtrl )
{
    MS_U8 regval;
    MS_U8 u8Arg;

    if ( eModeCtrl>
#if NO_MVOP_PATCH
        E_MVOP_FD_MASK_INV)
#else
        E_MVOP_SIZE_FROM_MVD_PATCH)
#endif
    {
        return;
    }
    else if (eModeCtrl >= E_MVOP_FMT_FROM_MVD)
    {
        regval = HAL_ReadByte(VOP_FD_MASK); // ox30
    }
    else
    {
        regval = HAL_ReadByte(VOP_INFO_FROM_CODEC_L); //0x38
    }

    switch(eModeCtrl)
    {
        case E_MVOP_BASE_FROM_IP:
            u8Arg = 0x0;
            break;
        case E_MVOP_SRC_FROM_MVD:
            u8Arg = 0x0;
            break;
        case E_MVOP_FIELD_FROM_MVD:
            u8Arg = 0x0;
            break;
        case E_MVOP_FIELD_FROM_MVD_INV:
            u8Arg = 0x0;
            break;
        case E_MVOP_PITCH_FROM_IP:
            u8Arg = 0x0;
            break;
        case E_MVOP_FMT_FROM_MVD:
            u8Arg = 0x0;
            break;
        case E_MVOP_FD_MASK_CLR:
            u8Arg = VOP_FD_MASK_CLR;
            break;
        case E_MVOP_FD_MASK_INV:
            u8Arg = VOP_FD_MASK_INV;
            break;
#if (NO_MVOP_PATCH == 0)
        case E_MVOP_SIZE_FROM_MVD_PATCH:
            u8Arg = 0x0;
            break;
        case E_MVOP_SEQ_FROM_MVD_PATCH:
            u8Arg = 0x0;
            break;
#endif
        default:
            u8Arg = 0x0;
            break;

    }
    if ( bEnable )
    {
        regval |= u8Arg;
    }
    else
    {
        regval &= ~u8Arg;
    }
    if (eModeCtrl >= E_MVOP_FMT_FROM_MVD)
    {
        HAL_WriteByte(VOP_FD_MASK, regval);
    }
    else
    {
        HAL_WriteByte(VOP_INFO_FROM_CODEC_L, regval);
    }
}
