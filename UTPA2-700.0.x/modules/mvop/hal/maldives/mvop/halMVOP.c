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
static MS_BOOL bEnableFreerunMode = FALSE;
static MS_BOOL b3DLRMode=0;    /// 3D L/R dual buffer mode
static MS_BOOL b3DLRAltOutput = FALSE;    /// 3D L/R line alternative output
static MS_BOOL b3DLRAltSBSOutput = FALSE;
static MS_BOOL bEnableFreerunMode = FALSE;
static MS_BOOL bNewVSyncMode = FALSE;
static MVOP_RptFldMode eRepeatField = E_MVOP_RPTFLD_NONE;      /// mvop output repeating fields for single field input.
static MVOP_RgbFormat eMainRgbFmt = E_MVOP_RGB_NONE;
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
    MS_BOOL bIs422;
    MS_BOOL bIsH265;
}MVOP_CTX_HAL;

static MVOP_CTX_HAL *g_pHalMVOPCtx = NULL;
static MS_U32 u32RiuBaseAdd=0;
static MS_U16  u16InsVBlank=0;

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

#define SUB_REG(x)         (x-MVOP_REG_BASE+MVOP_SUB_REG_BASE)

#define _FUNC_NOT_USED()        do {} while ( 0 )
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define LOWBYTE(u16)            ((MS_U8)(u16))
#define HIGHBYTE(u16)           ((MS_U8)((u16) >> 8))


#define VOP_ON_MIU1                     ((HAL_ReadByte(0x12F2) & BIT3) == BIT3)
#define _MaskMiuReq_MVOP_R( m )         HAL_WriteRegBit(0x1266, m, BIT3)
#define _MaskMiu1Req_MVOP_R( m )        HAL_WriteRegBit(0x0666, m, BIT3)

#define HAL_MIU_SetReqMask( miu_clients, mask ) \
   do { \
       if (VOP_ON_MIU1 == 0) \
           _MaskMiuReq_##miu_clients( mask ); \
       else \
           _MaskMiu1Req_##miu_clients( mask ); \
   }while(0)

#define SUBVOP_ON_MIU1                  ((HAL_ReadByte(0x12F2) & BIT4) == BIT4)
#define _MaskMiuReq_SUBMVOP_R( m )         HAL_WriteRegBit(0x1266, m, BIT4)
#define _MaskMiu1Req_SUBMVOP_R( m )        HAL_WriteRegBit(0x0666, m, BIT4)

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
    g_pHalMVOPCtx->bIs422 = 0;
    g_pHalMVOPCtx->bIsH265 = 0;
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
    MS_U32 u32Addr;
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
    return FALSE;
}

void HAL_MVOP_RegSetBase(MS_U32 u32Base)
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
    MS_U8 u8Miu = VOP_ON_MIU1;
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
    HAL_WriteRegBit(VOP_CTRL0, 0, BIT0);
    HAL_WriteRegBit(VOP_CTRL0, 1, BIT0);
}

void HAL_MVOP_Enable(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(VOP_CTRL0);

    if ( bEnable )
    {
        regval |= 0x1;
    }
    else
    {
        regval &= ~0x1;
    }

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
        case HALMVOP_160MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_160MHZ, CKG_DC0_MASK);
            break;
        case HALMVOP_144MHZ:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_144MHZ, CKG_DC0_MASK);
            break;
        default:
            HAL_WriteByteMask(REG_CKG_DC0, CKG_DC0_SYNCHRONOUS, CKG_DC0_MASK);
            MVOP_PRINTF("Attention! In HAL_MVOP_SetFrequency default path!\n");
            break;
    }
}

HALMVOPFREQUENCY HAL_MVOP_GetMaximumClk(void)
{
    return HALMVOP_160MHZ;
}

HALMVOPFREQUENCY HAL_MVOP_GetCurrentClk(void)
{
    (MVOP_DBG("%s err: HAL_MVOP_GetCurrentClk=NULL\n", __FUNCTION__));
    return 0;
}

void HAL_MVOP_SetOutputInterlace(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = HAL_ReadByte(VOP_CTRL0);

    if ( bEnable )
    {
        regval |= 0x80;
        // set h265 interlace mode
        if(g_pHalMVOPCtx->bIsH265)
        {
            HAL_WriteRegBit(VOP_INT_TYPE, 1, VOP_EVD_INT_SEP);
        }
    }
    else
    {
        regval &= ~0x80;
    }

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

void HAL_MVOP_SetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion )
{
    MS_U8 regval;
    MS_U16 u16strip;

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

    //set MVOP test pattern to black
    HAL_MVOP_SetBlackBG();

    //regval = HAL_ReadByte(VOP_MPG_JPG_SWITCH);
    regval = 0;
    regval |= ( mode & 0x3 );

    // clear extend strip len bit by default
    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Disable H264 or RM Input
    HAL_WriteRegBit(VOP_INPUT_SWITCH1, 0, BIT2|BIT3);
    // Clear 422 Flag
    g_pHalMVOPCtx->bIs422 = 0;
    // Clear evd Flag for interlace mode setting
    g_pHalMVOPCtx->bIsH265 = 0;
    HAL_WriteRegBit(VOP_INFO_FROM_CODEC_L, 1, BIT3);
    HAL_WriteRegBit(VOP_MIRROR_CFG, 1, BIT5);
    //8*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);
    HAL_WriteRegBit(VOP_MIRROR_CFG_HI, 0, VOP_REF_SELF_FLD);
    HAL_MVOP_SetFieldInverse(ENABLE, ENABLE);
    // Disable New Vsync Mode
    g_pHalMVOPCtx->bNewVSyncMode = FALSE;
    // EVD mode disable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 0, EVD_ENABLE);
    /*****************************************************/
#endif
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
                HAL_MVOP_SetFieldInverse(ENABLE, DISABLE);
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

        if (pparam->u16StripSize == 0)
        {
            if (pparam->bSD)
                u16strip = 720;
            else
                u16strip = 1920;
        }
        else
        {
            u16strip = pparam->u16StripSize;
        }

        // set dc_strip[7:0]
        if ( pparam->bDramRdContd == 0 ) {
            u16strip = u16strip/8;
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


void HAL_MVOP_SetSynClk(MVOP_Timing *ptiming)
{
    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    if(g_pHalMVOPCtx->bEnableFreerunMode)
    {
        MS_U32 u32FreerunClk = (MS_U32)((MS_U64)(MPLL_CLOCK << 27) / ((MS_U32)ptiming->u16H_TotalCount * (MS_U32)ptiming->u16V_TotalCount * (MS_U32)ptiming->u8Framerate));
        HAL_MVOP_SetFrequency(HALMVOP_FREERUNMODE);
        HAL_WriteByte((REG_DC0_FREERUN_CW_L  ), LOWBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC0_FREERUN_CW_L+1), HIGHBYTE((MS_U16)u32FreerunClk));
        HAL_WriteByte((REG_DC0_FREERUN_CW_H  ), LOWBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteByte((REG_DC0_FREERUN_CW_H+1), HIGHBYTE((MS_U16)(u32FreerunClk >> 16)));
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 1, UPDATE_DC0_FREERUN_CW);
        HAL_WriteRegBit(REG_UPDATE_DC0_CW, 0, UPDATE_DC0_FREERUN_CW);
    }
    else if (_bEnable60P)
    {
        //Set DC1 Timing
        MS_U32 u32FrameRate = (MS_U32)ptiming->u16ExpFrameRate;
        MS_U32 u32VSize = 1024;
        MS_U32 u32HSize = ((86400000 / u32FrameRate) * 1000) / u32VSize;

        MS_ASSERT(u32HSize <= 4096);

        HAL_MVOP_SetFrequency(HAL_MVOP_GetMaxFreerunClk());

        HAL_WriteRegBit(VOP_REG_FRAME_RST, 0, BIT15); // reg_frame_rst = 0

        HAL_WriteRegBit(VOP_CTRL0, DISABLE, VOP_FSYNC_EN); // frame sync disable
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

    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for EVD Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetEVDHardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, 0x00);

    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    //16*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 1, BIT1);

    // SVD mode enable
    // EVD use HVD interface
    HAL_WriteByteMask(VOP_INPUT_SWITCH1, BIT3, BIT2|BIT3);

    // EVD use WISHBONE(R2) interface
    //HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, VOP_R2_WISHBONE);

    // MIU select from WB
    //HAL_WriteByteMask(VOP_INFO_FROM_CODEC_H, 0x30, VOP_INFO_FROM_CODEC_MIU_BUF0_SEL|VOP_INFO_FROM_CODEC_MIU_BUF1_SEL);

    // EVD mode enable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // set evd flag for interlace mode setting
    g_pHalMVOPCtx->bIsH265 = 1;

#if defined(CONFIG_PURE_SN)
    HAL_WriteByteMask(VOP_REG_MASK, 0, VOP_LSB_REQ_MASK);
#endif
    // Write trigger
    HAL_MVOP_LoadReg();
}

/******************************************************************************/
/// Set MVOP for VP9 Hardwire Mode
/******************************************************************************/
void HAL_MVOP_SetVP9HardwireMode(void)
{
    // Hardwire mode
    HAL_WriteByte(VOP_MPG_JPG_SWITCH, 0x00);

    HAL_WriteRegBit(VOP_REG_STRIP_ALIGN, 0, BIT0);

    // SVD mode enable
    // EVD use HVD interface
    HAL_WriteByteMask(VOP_INPUT_SWITCH1, BIT3, BIT2|BIT3);

    // EVD mode enable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 1, EVD_ENABLE);

    // set mvop to 128bit_i128 interface
    HAL_WriteByteMask(VOP_MIU_IF, VOP_MIU_128BIT, VOP_MIU_128BIT|VOP_MIU_128B_I64);

    // Write trigger
    HAL_MVOP_LoadReg();
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

MS_BOOL HAL_MVOP_Set3DLRInsVBlank(MS_U16 u16InsLines)
{
    u16InsVBlank = u16InsLines;
    return TRUE;
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

void HAL_MVOP_SetYUVBaseAdd(MS_U32 u32YOffset, MS_U32 u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)
{
    MS_U32 u32tmp = 0;

    if (g_pHalMVOPCtx == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s g_pHalMVOPCtx is NULL pointer\n", __FUNCTION__);)
        return;
    }
    // Y offset
    u32tmp = u32YOffset >> 3;
    if ( !bProgressive )
    {   //Refine Y offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u32tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP or NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(VOP_JPG_YSTR0_L, u32tmp & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+1), (u32tmp >> 8) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+2), (u32tmp >> 16) & 0xff);
    HAL_WriteByte((VOP_JPG_YSTR0_L+3), (u32tmp >> 24) & VOP_YUV_STR_HIBITS);

    if (!bProgressive )
    {   //Y offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_YSTR1_L, u32tmp & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+1), (u32tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+2), (u32tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+3), (u32tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    if (b422pack)
    {
        u32UVOffset = u32YOffset + 16; //add 16 for 128bit; add 8 for 64bit
    }
    // UV offset
    u32tmp = u32UVOffset >> 3;
    if( !bProgressive )
    {  //Refine UV offset for interlace repeat bottom field
        if (E_MVOP_RPTFLD_BOT == g_pHalMVOPCtx->eRepeatField)
        {
            MVOP_DBG("%d eRepeatField(%x) is bottom!\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
            u32tmp += 2;
        }
        else
        {
            MVOP_DBG("%d eRepeatField(%x) is TOP/NONE.\n", __LINE__, g_pHalMVOPCtx->eRepeatField);
        }
    }
    HAL_WriteByte(VOP_JPG_UVSTR0_L, u32tmp & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+1), (u32tmp >> 8) & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+2), (u32tmp >> 16) & 0xff);
    HAL_WriteByte((VOP_JPG_UVSTR0_L+3), (u32tmp >> 24) & VOP_YUV_STR_HIBITS);

    if( !bProgressive )
    {  //UV offset of bottom field if interlace
        HAL_WriteByte(VOP_JPG_UVSTR1_L, u32tmp & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+1), (u32tmp >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+2), (u32tmp >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+3), (u32tmp >> 24) & VOP_YUV_STR_HIBITS);
    }

    return;
}

MS_BOOL HAL_MVOP_SetYUVBaseAddMultiView(MVOP_BaseAddInput *stBaseAddInfo)
{
    MS_U32 u32tmpY = 0;
    MS_U32 u32tmpUV = 0;

    if (stBaseAddInfo == NULL)
    {
        MVOP_DBG(MVOP_PRINTF("%s stBaseAddInfo is NULL pointer\n", __FUNCTION__);)
        return FALSE;
    }
    // Y offset
    u32tmpY = (stBaseAddInfo->u32YOffset) >> 3;
    // UV offset
    u32tmpUV = (stBaseAddInfo->u32UVOffset) >> 3;

    switch(stBaseAddInfo->eView)
    {
    case E_MVOP_MAIN_VIEW:
        // Y offset
        HAL_WriteByte(VOP_JPG_YSTR0_L, u32tmpY & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+1), (u32tmpY >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+2), (u32tmpY >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR0_L+3), (u32tmpY >> 24) & VOP_YUV_STR_HIBITS);

        // UV offset
        HAL_WriteByte(VOP_JPG_UVSTR0_L, u32tmpUV & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+1), (u32tmpUV >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+2), (u32tmpUV >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR0_L+3), (u32tmpUV >> 24) & VOP_YUV_STR_HIBITS);
        break;
    case E_MVOP_2ND_VIEW:
        // Y offset
        HAL_WriteByte(VOP_JPG_YSTR1_L, u32tmpY & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+1), (u32tmpY >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+2), (u32tmpY >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_YSTR1_L+3), (u32tmpY >> 24) & VOP_YUV_STR_HIBITS);

        //UV offset
        HAL_WriteByte(VOP_JPG_UVSTR1_L, u32tmpUV & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+1), (u32tmpUV >> 8) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+2), (u32tmpUV >> 16) & 0xff);
        HAL_WriteByte((VOP_JPG_UVSTR1_L+3), (u32tmpUV >> 24) & VOP_YUV_STR_HIBITS);
        break;
    default:
        break;
    }
    return TRUE;
}

MS_U32 HAL_MVOP_GetYBaseAdd(void)
{
    MS_U32 u32YOffset;
    u32YOffset = HAL_ReadByte(VOP_JPG_YSTR0_L)&0xff;
    u32YOffset |=((HAL_ReadByte((VOP_JPG_YSTR0_L+1))<<8)&0xff00);
    u32YOffset |=((HAL_ReadByte((VOP_JPG_YSTR0_L+2))<<16)&0xff0000);
    u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u32YOffset;
}

MS_U32 HAL_MVOP_GetUVBaseAdd(void)
{
    MS_U32 u32UVOffset;
    u32UVOffset = HAL_ReadByte(VOP_JPG_UVSTR0_L)&0xff;
    u32UVOffset |=((HAL_ReadByte((VOP_JPG_UVSTR0_L+1))<<8)&0xff00);
    u32UVOffset |=((HAL_ReadByte((VOP_JPG_UVSTR0_L+2))<<16)&0xff0000);
    u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
    return u32UVOffset;
}

MS_U32 HAL_MVOP_GetYBaseAddMultiView(MVOP_3DView eView)
{
    MS_U32 u32YOffset;
    switch(eView)
    {
        case E_MVOP_MAIN_VIEW:
            u32YOffset = HAL_ReadByte(VOP_JPG_YSTR0_L)&0xff;
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+1))<<8)&0xff00);
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+2))<<16)&0xff0000);
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
            break;
        case E_MVOP_2ND_VIEW:
            u32YOffset = HAL_ReadByte(VOP_JPG_YSTR1_L)&0xff;
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+1))<<8)&0xff00);
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+2))<<16)&0xff0000);
            u32YOffset |= ((HAL_ReadByte((VOP_JPG_YSTR1_L+3))<<24)&0x7000000);
            break;
        default:
            u32YOffset = 0;
            break;
    }
    return u32YOffset;
}

MS_U32 HAL_MVOP_GetUVBaseAddMultiView(MVOP_3DView eView)
{
    MS_U32 u32UVOffset;
    switch(eView)
    {
        case E_MVOP_MAIN_VIEW:
            u32UVOffset = HAL_ReadByte(VOP_JPG_UVSTR0_L)&0xff;
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+1))<<8)&0xff00);
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+2))<<16)&0xff0000);
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR0_L+3)) & VOP_YUV_STR_HIBITS) <<24);
            break;
        case E_MVOP_2ND_VIEW:
            u32UVOffset = HAL_ReadByte(VOP_JPG_UVSTR1_L)&0xff;
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+1))<<8)&0xff00);
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+2))<<16)&0xff0000);
            u32UVOffset |= ((HAL_ReadByte((VOP_JPG_UVSTR1_L+3))<<24)&0x7000000);
            break;
        default:
            u32UVOffset = 0;
            break;
    }
    return u32UVOffset;
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
    return ((HAL_ReadRegBit(VOP_RGB_FMT, BIT2) & HAL_ReadRegBit(VOP_CTRL0, BIT3)) == BIT2);
}

MS_U16 HAL_MVOP_GetTopVStart(MVOP_DevID eID)
{
    MS_U16 u16TopVStart = 0;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            u16TopVStart = HAL_Read2Byte(VOP_IMG_VSTR0)&0x1fff;
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
        default:
                break;
    }
    return TRUE;
}

MS_U16 g_u16SetStartX = 0;
MS_U16 g_u16SetStartY = 0;
MS_BOOL g_bIsY4Align = 0;

void HAL_MVOP_SetStartX(MS_U16 u16Length)
{
	g_u16SetStartX = u16Length;
	HAL_WriteByte(VOP_REG_CROP_HSTART, u16Length & 0xff);
	HAL_WriteByte((VOP_REG_CROP_HSTART + 1),((u16Length) >> (8)) & (0x1f));
	// Write trigger
	HAL_MVOP_LoadReg();
}


void HAL_MVOP_SetStartY(MS_U16 u16Length, MS_BOOL bIs4Align)
{
	g_u16SetStartY = u16Length;
        g_bIsY4Align = bIs4Align;
	HAL_WriteByte(VOP_REG_CROP_VSTART, u16Length & 0xff);
	HAL_WriteByte((VOP_REG_CROP_VSTART + 1), ((u16Length) >> (8)) & (0x1f));
	// Write trigger
	HAL_MVOP_LoadReg();
}


void HAL_MVOP_SetPicWidthMinus(MS_U16 u16Sizes)
{
	u16Sizes = u16Sizes + 1 - g_u16SetStartX ;
    HAL_WriteByte(VOP_REG_CROP_HSIZE, u16Sizes & 0xff);
    HAL_WriteByte((VOP_REG_CROP_HSIZE + 1), ((u16Sizes) >> (8)) & (0x1f));
	// Write trigger
    HAL_MVOP_LoadReg();
}

void HAL_MVOP_SetPicHeightMinus(MS_U16 u16Sizes)
{
	u16Sizes = u16Sizes + 1 - g_u16SetStartY ;
    if(g_bIsY4Align)
        u16Sizes -= 2;

    HAL_WriteByte(VOP_REG_CROP_VSIZE, (u16Sizes) & 0xff);
    HAL_WriteByte((VOP_REG_CROP_VSIZE + 1), ((u16Sizes) >> (8)) & (0x1f));
    // Write trigger
    HAL_MVOP_LoadReg();
}

void HAL_MVOP_PowerStateSuspend(void)
{
    g_pHalMVOPCtx->bIsInit = 0;
}

MVOP_HSMode HAL_MVOP_GetHandShakeMode(MVOP_DevID eID)
{
    MVOP_HSMode eRet = E_MVOP_HS_NOT_SUPPORT;
    switch(eID)
    {
        case E_MVOP_DEV_0:
            eRet = E_MVOP_HS_NOT_SUPPORT;
            break;
        case E_MVOP_DEV_1:
#if MVOP_SUPPORT_SUB
            eRet = E_MVOP_HS_INVALID_PARAM;
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

    u16STC_CW_L = HAL_Read2Byte(REG_STC0_CW_L)&0xffff;
    u16STC_CW_H = HAL_Read2Byte(REG_STC0_CW_H)&0xffff;

    if((u16STC_CW_L || u16STC_CW_H) == 0)
        return FALSE;
    else
        return TRUE;

}

// munich not support miu control by ip
#if 0
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
#endif

/* Y and UV buffer can not be in seperated buffer in Madison*/
void HAL_MVOP_SelMIU(MVOP_DevID eDevID, HALMVOPMIUSEL eMiuMSB0, HALMVOPMIUSEL eMiuMSB1, HALMVOPMIUSEL eMiuLSB0, HALMVOPMIUSEL eMiuLSB1)
{
    switch(eDevID)
    {
        case E_MVOP_DEV_0:
        {
            HAL_WriteRegBit(VOP_MIU_SEL, eMiuMSB0, VOP_BUF_MIU_SEL);
            break;
        }
#if MVOP_SUPPORT_SUB
        case E_MVOP_DEV_1:
        {
            HAL_WriteRegBit(SUB_REG(VOP_MIU_SEL), eMiuMSB0, VOP_BUF_MIU_SEL);
            break;
        }
#endif
        default:
            break;
    }
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

void HAL_MVOP_ResetReg(MVOP_DevID eDevID)
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
    // Reset Mono mode
    HAL_MVOP_SetMonoMode(FALSE);

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
    // Clear evd Flag for interlace mode setting
    g_pHalMVOPCtx->bIsH265 = 0;
    HAL_WriteRegBit(VOP_INT_TYPE, 0, VOP_EVD_INT_SEP);
    //8*32 tile format
    HAL_WriteRegBit(VOP_REG_WR, 0, BIT1);
    HAL_WriteRegBit(VOP_MIRROR_CFG_HI, 0, VOP_REF_SELF_FLD);
    HAL_MVOP_SetFieldInverse(ENABLE, ENABLE);
    // Disable New Vsync Mode
    g_pHalMVOPCtx->bNewVSyncMode = FALSE;
    // EVD mode disable
    HAL_WriteRegBit(VOP_INPUT_SWITCH0, 0, EVD_ENABLE);
	HAL_WriteRegBit(VOP_REG_DUMMY, 1, VOP_420_BW_SAVE);
	//FIFO threshold for netflix
    HAL_WriteByteMask(VOP_DMA0, 0x1A, VOP_FORCE_HIGH|VOP_HI_TSH|VOP_BURST_EXT);
    HAL_WriteByteMask(VOP_DMA1, 0xA2, VOP_FORCE_HIGH|VOP_HI_TSH|VOP_BURST_EXT);
    HAL_WriteByte(VOP_BURST_CTRL0, 0x04);
    HAL_WriteRegBit(VOP_CTRL1, 1, VOP_MVD_VS_SEL);
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
        HAL_MVOP_SubEnableMVDInterface(FALSE);
        MVOP_PRINTF("[%s] Not Support sub MVOP!\n",__FUNCTION__);
        break;
    }
#endif
    default:
        MVOP_PRINTF("[%s] Input Device ID is Error!\n",__FUNCTION__);
        break;
    }
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


