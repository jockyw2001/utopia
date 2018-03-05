//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, GE_WaitCmdQAvail, or republication of all
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
#include "MsCommon.h"
#include <string.h>
#include "regGE.h"
#include "drvGE.h"
#include "halGE.h"
#include "halCHIP.h"
#ifdef MSOS_TYPE_LINUX
#include "halMPool.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define GE_DITHER_RAND_ENABLE       0                                   //[TBD] Add new option for SetDither if rand is used in the future.
#define GE_PATCH_ENABLE             0

#define GE_LOG_ENABLE               0
#define MS_DEBUG 1

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GE_MIU_ADDR_MASK            0x7FFFFFFF

#define GE_CMDQ_FREECNT()           ((GE_REG(REG_GE_STAT)&GE_STAT_CMDQ_MASK)>>GE_STAT_CMDQ_SHFT)
#define GE_VCMDQ_FREECNT()          (GE_REG(REG_GE_VCMDQ_STAT) + ((GE_REG(REG_GE_BIST_STAT)&GE_VCMDQ_STAT_H_MASK) << 16))

#define GE_BUSY()                   (GE_REG(REG_GE_STAT) & GE_STAT_BUSY)

#define GE_CMDQ_ENABLE              1 // Always Enable
#define GE_CMD_SIZE_MAX             GE_STAT_CMDQ_MAX
#define GE_VCMD_SIZE_MAX            GE_STAT_VCMDQ_MAX
#define GE_CMD_SIZE                 1 // 1 queue entry available for 2 commands, but we just check entry for convenience

#define GE_MAP_VCMD_SIZE_TO_HWDEF(x)  ((x))

#define GE_YIELD()                  MsOS_DelayTask(2)

#define GE_RESET_COUNT  0x800

#ifdef MS_DEBUG
#define GE_DBG(_fmt, _args...)      printf(_fmt, ##_args)
#else
#define GE_DBG(_fmt, _args...)      { }
#endif
#define GE_BURST_LEN                 128

#define GE_TAG_INTERRUPT_WAITING_TIME 10 // ms
#define GE_TAG_INTERRUPT_DEBUG_PRINT_THRESHOLD (500/GE_TAG_INTERRUPT_WAITING_TIME)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
typedef enum
{
    E_GE_CLEAR_INT = 0x0001,
    E_GE_MASK_INT = 0x0002,
    E_GE_UNMASK_INT = 0x0004,
    E_GE_INT_TAG_MODE = 0x0008,
    E_GE_INT_NORMAL_MODE = 0x0010
} E_GE_INT_OP;
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------                 // line pattern reset


const MS_U8  _GE_Reg_Backup[] = {
    REG_GE_EN, REG_GE_CFG, REG_GE_TH, REG_GE_ROP2, REG_GE_BLEND, REG_GE_ALPHA, REG_GE_ALPHA_CONST,
    REG_GE_SCK_HTH_L, REG_GE_SCK_HTH_H, REG_GE_SCK_LTH_L, REG_GE_SCK_LTH_H, REG_GE_DCK_HTH_L,
    REG_GE_DCK_HTH_H, REG_GE_DCK_LTH_L, REG_GE_DCK_LTH_H, REG_GE_OP_MODE, REG_GE_ATEST_TH,
    REG_GE_YUV_MODE, REG_GE_SRC_BASE_L, REG_GE_SRC_BASE_H, REG_GE_DST_BASE_L, REG_GE_DST_BASE_H,
    REG_GE_SRC_PITCH, REG_GE_DST_PITCH, REG_GE_FMT,
    0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e,  // I0~I4
    0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048,  // I5-I9
    0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f, 0x0050, 0x0051, 0x0052,  // I10-I14
    0x0053, 0x0054,                                                                  // I15
    REG_GE_CLIP_L, REG_GE_CLIP_R, REG_GE_CLIP_T, REG_GE_CLIP_B, REG_GE_ROT_MODE, REG_GE_BLT_SCK_MODE,
    REG_GE_BLT_SCK_CONST_L, REG_GE_BLT_SCK_CONST_H, REG_GE_BLT_DST_X_OFST, REG_GE_BLT_DST_Y_OFST,
    REG_GE_LINE_DELTA, REG_GE_LINE_STYLE, REG_GE_LINE_LENGTH, REG_GE_BLT_SRC_DX, REG_GE_BLT_SRC_DY,
    REG_GE_ITALIC_OFFSET, REG_GE_ITALIC_DELTA, REG_GE_PRIM_V0_X, REG_GE_PRIM_V0_Y, REG_GE_PRIM_V1_X,
    REG_GE_PRIM_V1_Y, REG_GE_PRIM_V2_X, REG_GE_PRIM_V2_Y, REG_GE_BLT_SRC_W, REG_GE_BLT_SRC_H,
    REG_GE_PRIM_C_L, REG_GE_PRIM_C_H, REG_GE_PRIM_RDX_L, REG_GE_PRIM_RDX_H, REG_GE_PRIM_RDY_L,
    REG_GE_PRIM_RDY_H, REG_GE_PRIM_GDX_L, REG_GE_PRIM_GDX_H, REG_GE_PRIM_GDY_L, REG_GE_PRIM_GDY_H,
    REG_GE_PRIM_BDX_L, REG_GE_PRIM_BDX_H, REG_GE_PRIM_BDY_L, REG_GE_PRIM_BDY_H, REG_GE_PRIM_ADX,
    REG_GE_PRIM_ADY, 0xFF
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Local Var
//------------------------------------------------------------------------------
GE_CHIP_PROPERTY g_GeChipPro =
{
    .WordUnit =                             GE_WORD_UNIT,

    .bSupportFourePixelMode =               TRUE,
    .bFourPixelModeStable =                 TRUE,

    .bSupportMultiPixel =                   FALSE,
    .bSupportSpiltMode =                    TRUE,
    .bSupportTwoSourceBitbltMode =          FALSE,
    .bSupportTLBMode =                      TRUE,
    .MIUSupportMaxNUM =                     GE_MAX_MIU,
    .BltDownScaleCaps =
    {
        .u8RangeMax =                       1,
        .u8RangeMin =                       32,
        .u8ContinuousRangeMin =             1,
        .bFullRangeSupport =                TRUE,

        .u8ShiftRangeMax =                  0,              /// 1   = 2^0   = 1<<0
        .u8ShiftRangeMin =                  5,              /// 32  = 2^5   = 1<<5
        .u8ShiftContinuousRangeMin =        0,              /// 1   = 2^0   = 1<<0
    }
};


#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
static MS_U32 s32HalIomapBaseAddr = 0;
static MS_BOOL bGeIrqInited = FALSE;
static MS_S32 s32GeWaitTagEventHandle = -1;
static MS_S32 s32WaitingTagPid = -1;

void            _GE_WaitTag_InterruptCbk(InterruptNum eIntNum);
static void     _GE_InitGE1Regs( GE_CTX_HAL_LOCAL *pGEHalLocal );
static void     _GE_SetGeThreshold(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8GeIdx, MS_U8 u8Threshold);
static GE_Result _GE_Ctrl_IntMode(GE_CTX_HAL_LOCAL *pGEHalLocal, E_GE_INT_OP int_op);
static void     _GE_Print_GeWaitTagTimeout_Msg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tagID);
static MS_U16   _GE1_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr);
#endif


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void GE_Chip_Proprity_Init(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    pGEHalLocal->pGeChipPro = &g_GeChipPro;
}

GE_Result _GE_SetBltScaleRatio2HW(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_ScaleInfo *pScaleinfo)
{
    MS_U16 u16RegVal;

    GE_WriteReg(pGEHalLocal, REG_GE_BLT_SRC_DX, (MS_U16)(pScaleinfo->x&0xFFFF));
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_SRC_DY, (MS_U16)(pScaleinfo->y&0xFFFF));
    //Set Initial DeltaX, DeltaY:
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_DST_X_OFST, (MS_U16)(pScaleinfo->init_x&0xFFFF));
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_DST_Y_OFST, (MS_U16)(pScaleinfo->init_y&0xFFFF));

    //set MSBs of REG_GE_BLT_SRC_DY, REG_GE_BLT_SRC_DY:
    u16RegVal = GE_ReadReg(pGEHalLocal, REG_GE_BLT_DST_X_OFST) & ~(GE_STBB_DX_MSB);
    u16RegVal |= (((pScaleinfo->x>>16)<<GE_STBB_DX_MSB_SHFT) & GE_STBB_DX_MSB);
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_DST_X_OFST, u16RegVal);

    u16RegVal = GE_ReadReg(pGEHalLocal, REG_GE_BLT_DST_Y_OFST) & ~(GE_STBB_DY_MSB);
    u16RegVal |= (((pScaleinfo->y>>16)<<GE_STBB_DY_MSB_SHFT) & GE_STBB_DY_MSB);
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_DST_Y_OFST, u16RegVal);

    return E_GE_OK;
}

void GE_SetActiveCtrlMiu1(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MIU1_REG(MIU1_GEGROUP) = MIU1_REG(MIU1_GEGROUP)|MIU1_GE_CLIENT;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
static void GE_DumpReg(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U32 i;

    printf("Dump GE register:\n");
    for (i = 0; i < 0x80; i++)
    {
        if(i % 0x08 == 0) {
            printf("\n");
            printf("h%02x    ", (MS_U8)i );
        }
        printf("%04x ", GE_REG(i) );
    }

    printf("\n");
}
static MS_U32 _GET_MIU_MASK_SHIFT(void)
{
    if (HAL_MIU1_BASE==0x20000000)
        return (29UL);
    else if (HAL_MIU1_BASE==0x10000000)
        return (28UL);
    else if (HAL_MIU1_BASE==0x8000000)
        return (27UL);
    else if (HAL_MIU1_BASE==0x4000000)
        return (26UL);
    else if (HAL_MIU1_BASE==0x60000000)
        return (29UL);
    else
    {
        printf("\n[%s] !!!!!! get miu1 base error!!!!!!", __FUNCTION__);
        return (27UL);  //default return case
    }
}

MS_U8  _GFXAPI_MIU_ID(MS_U32 ge_fbaddr)
{
#if 1
    if(ge_fbaddr>=HAL_MIU1_BASE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return ((MS_U8) (((ge_fbaddr)>>_GET_MIU_MASK_SHIFT())&((1UL<<GE_FB_ADDR_MIU_MASK_BIT)-1)));

#endif
}

MS_U32 _GFXAPI_PHYS_ADDR_IN_MIU(MS_U32 ge_fbaddr)
{
#if 1
            if(ge_fbaddr>=HAL_MIU1_BASE)
            {
                return (ge_fbaddr -= HAL_MIU1_BASE);
            }
            else
            {
                return (ge_fbaddr);
            }
#else
    return ((ge_fbaddr)&((1UL<<_GET_MIU_MASK_SHIFT())-1));
#endif
}

MS_U32 _GFXAPI_PHYS_ADDR_2_API(MS_U8 u8MIUId, MS_U32 ge_addrInMIU)
{
#if 1
    if(u8MIUId == 1)
    {

      return  (HAL_MIU1_BASE| (ge_addrInMIU&((1UL<<_GET_MIU_MASK_SHIFT())-1)));
    }
   else
    {

      return  (ge_addrInMIU&((1UL<<_GET_MIU_MASK_SHIFT())-1));
    }
#else

    return (((((MS_U32)(u8MIUId))&((1UL<<GE_FB_ADDR_MIU_MASK_BIT)-1))<<_GET_MIU_MASK_SHIFT()) |  \
        (((MS_U32)(ge_addrInMIU))&((1UL<<_GET_MIU_MASK_SHIFT())-1)));
#endif
}

#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
static void _GE_InitGE1Regs( GE_CTX_HAL_LOCAL *pGEHalLocal )
{
    // to support the ge tag interrupt mode
    // we use ge1 to make a fake fire to manufacture a tag interrupt

    // Set default FMT for avoiding 1st set buffinfo error.
    GE1_WriteReg(pGEHalLocal, REG_GE_FMT, (GE_FMT_ARGB1555<<GE_SRC_FMT_SHFT)+(GE_FMT_ARGB1555<<GE_DST_FMT_SHFT));

    _GE_SetGeThreshold(pGEHalLocal, 1, GE_THRESHOLD_SETTING);

    // useless dst addr
    GE1_WriteReg(pGEHalLocal, REG_GE_DST_BASE_L, 0x1234);
    GE1_WriteReg(pGEHalLocal, REG_GE_DST_BASE_H, 0x5678);

    // for designer suggestion
    // clip window should on left side of the destination rect for the speed factor
    GE1_WriteReg(pGEHalLocal, REG_GE_CLIP_L, 0x0);
    GE1_WriteReg(pGEHalLocal, REG_GE_CLIP_R, 0x1);
    GE1_WriteReg(pGEHalLocal, REG_GE_CLIP_T, 0x0);
    GE1_WriteReg(pGEHalLocal, REG_GE_CLIP_B, 0x1);
    GE1_WriteReg(pGEHalLocal, REG_GE_PRIM_V0_X, 0x10);
    GE1_WriteReg(pGEHalLocal, REG_GE_PRIM_V0_Y, 0x10);
    GE1_WriteReg(pGEHalLocal, REG_GE_PRIM_V1_X, 0x11);
    GE1_WriteReg(pGEHalLocal, REG_GE_PRIM_V1_Y, 0x11);
}

void _GE_WaitTag_InterruptCbk(InterruptNum eIntNum)
{
#if defined(MSOS_TYPE_LINUX)
    MS_S32 s32CurPid = (MS_S32)getpid();
#endif

#if defined(MSOS_TYPE_LINUX)
    if (s32WaitingTagPid == s32CurPid)
#endif
    {
        (*((volatile MS_U16 *)(s32HalIomapBaseAddr + GE1_BANK_NUM*2 + ((REG_GE_SRCMASK_GB)<<2) ))) = 0xE0;

        if (s32GeWaitTagEventHandle > 0)
        {
            if (FALSE == MsOS_SetEvent(s32GeWaitTagEventHandle, 0x1))
            {
                GE_DBG("[%s, %d]:  MsOS_ReleaseSemaphore failed\r\n", __FUNCTION__, __LINE__);
            }
        }
    }

    MsOS_EnableInterrupt(E_INT_IRQ_GE);
#ifdef MSOS_TYPE_LINUX
    MsOS_CompleteInterrupt(E_INT_IRQ_GE);
#endif
}

static void _GE_SetGeThreshold(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8GeIdx, MS_U8 u8Threshold)
{
    MS_U16 u16tmp;

    u16tmp = GE_ReadReg(pGEHalLocal, REG_GE_TH);

    u16tmp &= ~(GE_TH_STBB_MASK);
    u16tmp |= (u8Threshold & GE_TH_STBB_MASK);

    if (0 == u8GeIdx)
    {
        GE_WriteReg(pGEHalLocal, REG_GE_TH, u16tmp);
    }
    else
    {
        GE1_WriteReg(pGEHalLocal, REG_GE_TH, u16tmp);
    }
}

static GE_Result _GE_Ctrl_IntMode(GE_CTX_HAL_LOCAL *pGEHalLocal, E_GE_INT_OP int_op)
{
    MS_U16  u16IntReg=0;

    u16IntReg = _GE1_ReadReg(pGEHalLocal, REG_GE_SRCMASK_GB);

    if (E_GE_CLEAR_INT & int_op)
    {
        u16IntReg |= GE_INT_MODE_CLEAR;
        GE1_WriteReg(pGEHalLocal, REG_GE_SRCMASK_GB, u16IntReg);
        u16IntReg &= (~GE_INT_MODE_CLEAR);
    }

    if (E_GE_MASK_INT & int_op)
    {
        u16IntReg |= GE_INT_TAG_MASK;
    }

    if (E_GE_UNMASK_INT & int_op)
    {
        u16IntReg &= (~GE_INT_TAG_MASK);
    }

    if (E_GE_INT_TAG_MODE & int_op)
    {
        u16IntReg |= GE_INT_TAG_MODE;
    }

    if (E_GE_INT_NORMAL_MODE & int_op)
    {
        u16IntReg &= (~GE_INT_TAG_MODE);
    }

    GE1_WriteReg(pGEHalLocal, REG_GE_SRCMASK_GB, u16IntReg);

    return E_GE_OK;
}

static MS_BOOL _GE_IsTagInterruptEnabled(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_BOOL bret;
    MS_U16  u16IntReg=0;

    u16IntReg = _GE1_ReadReg(pGEHalLocal, REG_GE_SRCMASK_GB);

    bret = FALSE;
    if ((GE_INT_TAG_MODE & u16IntReg) > 0)
    {
        bret = TRUE;
    }

    return bret;
}

static void _GE_Print_GeWaitTagTimeout_Msg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tagID)
{
    MS_U16 tmp_reg;

    GE_DBG("[%s, %d]: >>>>>>>>>>>>>>>>>> ge wait event timeout <<<<<<<<<<<<<<<<<<<\r\n", __FUNCTION__, __LINE__);
    GE_DBG("[%s, %d]: current proc id = %ld \r\n", __FUNCTION__, __LINE__, s32WaitingTagPid);

    tmp_reg = _GE1_ReadReg(pGEHalLocal, REG_GE_SRCMASK_GB);
    GE_DBG("[%s, %d]: ge int status = 0x%x \r\n", __FUNCTION__, __LINE__, tmp_reg);

    tmp_reg = INTR_CTNL_BK(0x56);
    GE_DBG("[%s, %d]: cpu int mask = 0x%x \r\n", __FUNCTION__, __LINE__, tmp_reg);
    tmp_reg = INTR_CTNL_BK(0x5E);
    GE_DBG("[%s, %d]: cpu int status = 0x%x \r\n", __FUNCTION__, __LINE__, tmp_reg);

    tmp_reg = _GE1_ReadReg(pGEHalLocal, REG_GE_INT_TAG_COND_L);
    GE_DBG("[%s, %d]: int_tag = 0x%x \r\n", __FUNCTION__, __LINE__, tmp_reg);

    tmp_reg = _GE1_ReadReg(pGEHalLocal, REG_GE_TAG_L);
    GE_DBG("[%s, %d]: tag = 0x%x \r\n", __FUNCTION__, __LINE__, tmp_reg);

    GE_DBG("[%s, %d]: tagID = 0x%x \r\n", __FUNCTION__, __LINE__, tagID);
}

#endif

static void GE_Reset(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U16 reg0, reg1;

    reg0 = GE_REG(REG_GE_EN);
    reg1 = GE_REG(REG_GE_CFG);

    GE_REG(REG_GE_EN) = 0;
    GE_REG(REG_GE_CFG) = 0;

    GE_REG(REG_GE_EN) = reg0;
    GE_REG(REG_GE_CFG) = reg1;

}

static MS_U8 GE_MapVQ2Reg(GE_VcmqBufSize enBufSize)
{
    switch(enBufSize)
    {
        case E_GE_VCMD_4K:
            return GE_VQ_4K;
        case E_GE_VCMD_8K:
            return GE_VQ_8K;
        case E_GE_VCMD_16K:
            return GE_VQ_16K;
        case E_GE_VCMD_32K:
            return GE_VQ_32K;
        case E_GE_VCMD_64K:
            return GE_VQ_64K;
        case E_GE_VCMD_128K:
            return GE_VQ_128K;
        case E_GE_VCMD_256K:
            return GE_VQ_256K;
        case E_GE_VCMD_512K:
            return GE_VQ_512K;
        case E_GE_VCMD_1024K:
            return GE_VQ_1024K;
        default:
            return 0;
    }
}

void GE_WaitCmdQAvail(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U32 u32Count)
{
#if GE_CMDQ_ENABLE

    #ifdef MS_DEBUG
    MS_U32 waitcount = 0;
    #endif
    MS_U16 tmp1 = 0;
    MS_U32 u32CmdMax;

    /// VCMQ enabled
    if((GE_REG(REG_GE_CFG) & GE_CFG_VCMDQ) != 0)
    {
        // 16 Bytes one command in VCMDQ.
        u32CmdMax = (512 << (GE_REG(REG_GE_VCMDQ_SIZE) & 0x7));
        u32Count = MIN(u32CmdMax, u32Count);

        while (GE_CMDQ_FREECNT() < u32Count)
        {
            #ifdef MS_DEBUG
            if (waitcount >= GE_RESET_COUNT)
            {
                printf("[GE] V0 Wait command queue: %d : %x, %lx\n", tmp1, GE_CMDQ_FREECNT(), u32Count);
                waitcount = 0;
                tmp1++;
                if(tmp1 > 10)
                {
                    GE_DumpReg(pGEHalLocal);
                    GE_Reset(pGEHalLocal);
                }
            }
            waitcount++;
            #endif
            GE_YIELD();
        }
        tmp1 = 0;
        waitcount = 0;


        //If u32Count >= u32CmdMax, It will be dead loop. But since it won't happen, and if match
        //Full VCMDQ, hw will hang, so keep the logic.
        while ( (MS_U32)GE_VCMDQ_FREECNT() >= (MS_U32)(u32CmdMax- u32Count))
        {
            #ifdef MS_DEBUG
            if (waitcount >= GE_RESET_COUNT)
            {
                printf("[GE] Wait VCMQ : %d : %lx, %lx\n", tmp1, (MS_U32)GE_VCMDQ_FREECNT(), u32Count);
                waitcount = 0;
                tmp1++;
                if(tmp1 > 10)
                {
                    GE_DumpReg(pGEHalLocal);
                    GE_Reset(pGEHalLocal);
                }
            }
            waitcount++;
            #endif
            GE_YIELD();
        }
    }
    else
    {
        u32Count = MIN(GE_CMD_SIZE_MAX, u32Count);

        while (GE_CMDQ_FREECNT() < u32Count)
        {
            #ifdef MS_DEBUG
            if (waitcount >= GE_RESET_COUNT)
            {
                printf("[GE] Wait command queue: %d : %x, %lx\n", tmp1, GE_CMDQ_FREECNT(), u32Count);
                waitcount = 0;
                tmp1++;
                if(tmp1 > 10)
                {
                    GE_DumpReg(pGEHalLocal);
                    GE_Reset(pGEHalLocal);
                }
            }
            waitcount++;
            #endif
            GE_YIELD();
        }

    }

#endif
}

MS_U32 GE_ConvertAPIAddr2HAL(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8MIUId, MS_U32 u32GE_APIAddrInMIU)
{
     u32GE_APIAddrInMIU &= (1UL<<MIU_SELETE_OFFSET)-1UL;
     if(u8MIUId)
        u32GE_APIAddrInMIU |= 1UL<<MIU_SELETE_OFFSET;
     return u32GE_APIAddrInMIU;
}
MS_U32 GE_ConvertHALAddr2API(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8MIUId, MS_U32 u32GE_HALAddr)
{
     return _GFXAPI_PHYS_ADDR_2_API(u8MIUId, u32GE_HALAddr&((1UL<<MIU_SELETE_OFFSET)-1));
}

void GE_WaitIdle(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    #ifdef MS_DEBUG
    MS_U32 waitcount = 0;
    #endif
    MS_U16 tmp1 = 0;

    GE_WriteReg(pGEHalLocal, REG_GE_TAG_H, 0);
    GE_WriteReg(pGEHalLocal, REG_GE_TAG_L, GE_GetNextTAGID(pGEHalLocal, FALSE)); // write dummy
                                // GE will pack 2 register commands before CMDQ
                                // We need to push fifo if there is one command in the fifo before
                                // CMDQ. Then the GE status register will be consistant after idle.
    GE_WaitCmdQAvail(pGEHalLocal, GE_STAT_CMDQ_MAX); // Wait CMDQ empty

    // Wait level-2 command queue flush
    while (((GE_REG(REG_GE_STAT)&GE_STAT_CMDQ2_MASK)>>GE_STAT_CMDQ2_SHFT) != GE_STAT_CMDQ2_MAX)
    {
        #ifdef MS_DEBUG
        if (waitcount >= GE_RESET_COUNT)
        {
            printf("[GE] Wait Idle: %u : %x\n", tmp1, GE_CMDQ_FREECNT());
            waitcount = 0;
            tmp1++;
            if(tmp1 > 10)
            {
                GE_DumpReg(pGEHalLocal);
                GE_Reset(pGEHalLocal);
            }
        }
        waitcount++;
        #endif

        GE_YIELD();
    }

#ifdef MS_DEBUG
    waitcount = 0;
    tmp1 = 0;
#endif
    // Wait GE idle
    while (GE_REG(REG_GE_STAT) & GE_STAT_BUSY)
    {
        #ifdef MS_DEBUG
        if (waitcount >= GE_RESET_COUNT)
        {
            printf("[GE] Wait Busy: %d : %x\n", tmp1, GE_CMDQ_FREECNT());
            waitcount = 0;
            tmp1++;
            if(tmp1 > 10)
            {
                GE_DumpReg(pGEHalLocal);
                GE_Reset(pGEHalLocal);
            }
        }
        waitcount++;
        #endif

        GE_YIELD();
    }

}

GE_Result GE_Map_Share_Reg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr)
{
        if(addr == REG_GE_CFG)
            return E_GE_OK;
        else
            return E_GE_FAIL;
#if 0
        switch(addr)
        {
            case REG_GE_CFG:
                 return E_GE_OK;
            default:
                return E_GE_FAIL;
        }
#endif

}

GE_Result GE_Map_Share_RegEX(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr)
{
    return E_GE_FAIL;
}

MS_U16 GE_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr)
{
    MS_U16 u16NoFIFOMask;

    if(0x80 <= addr)
    {
        GE_WaitIdle(pGEHalLocal);
        return GE_REG(addr-GE_TABLE_REGNUM);
    }
    switch (addr)
    {//for registers which do not go through command queue
        case REG_GE_EN:
            u16NoFIFOMask = GE_EN_GE;
            break;
        /*
        case REG_GE_CFG:
            //u16NoFIFOMask = ~(GE_CFG_BLT_STRETCH|GE_CFG_EN_CLIPCHK|GE_CFG_BLT_ITALIC|GE_CFG_SRC_TILE|GE_CFG_DST_TILE);
            return pGEHalLocal->u16RegImage[addr];
            break;
        */
        case REG_GE_DBG:
        case REG_GE_TH:
        case REG_GE_BIST_STAT:
        case REG_GE_STAT:
        case REG_GE_VCMDQ_STAT:
        case REG_GE_MIU_PROT_LTH_L(0):
        case REG_GE_MIU_PROT_LTH_H(0):
        case REG_GE_MIU_PROT_HTH_L(0):
        case REG_GE_MIU_PROT_HTH_H(0):
        case REG_GE_MIU_PROT_LTH_L(1):
        case REG_GE_MIU_PROT_LTH_H(1):
        case REG_GE_MIU_PROT_HTH_L(1):
        case REG_GE_MIU_PROT_HTH_H(1):
        case REG_GE_TAG_L:
        case REG_GE_TAG_H:
        case REG_GE_VCMDQ_BASE_L:
        case REG_GE_VCMDQ_BASE_H:
            u16NoFIFOMask = 0xffff;
            break;
        case REG_GE_VCMDQ_SIZE:
            u16NoFIFOMask = GE_VCMDQ_SIZE_MASK;
            break;
        default:
              u16NoFIFOMask = 0;
              break;
    }

    if(0 == u16NoFIFOMask)
    {
        if(GE_Map_Share_Reg(pGEHalLocal,addr)== E_GE_OK)
            return pGEHalLocal->pHALShared->u16ShareRegImage[addr];
        else
        {
            return pGEHalLocal->u16RegGETable[addr];
        }
    }
    return (GE_REG(addr)&u16NoFIFOMask)|(pGEHalLocal->u16RegGETable[addr]&~u16NoFIFOMask);
}


void GE_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value)
{
    // CMDQ special command
    if(addr < GE_TABLE_REGNUM)
    {
         if(GE_Map_Share_Reg(pGEHalLocal,addr)== E_GE_OK)
         {
            pGEHalLocal->pHALShared->u16ShareRegImage[addr]= value;
    }

        pGEHalLocal->u16RegGETable[addr] = value;
    }
    else
    {
        printf("[%s][%d] Reg Index [%d]is out of GE_TABLE_REGNUM [%d]range!!!!\n",__FUNCTION__,__LINE__, addr, GE_TABLE_REGNUM);
    }
    GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);

if(pGEHalLocal->pHALShared->bGE_DirectToReg ==TRUE)
{
    GE_REG(addr)= value;
    return;
}
else
{
    MS_U16 i=0;

    switch (addr)
    {
        case REG_GE_EN:
        case REG_GE_CFG:
        case REG_GE_DBG:
        case REG_GE_TH:
        case REG_GE_BIST_STAT:
        case REG_GE_STAT:
        case REG_GE_VCMDQ_STAT:
        case REG_GE_MIU_PROT_LTH_L(0):
        case REG_GE_MIU_PROT_LTH_H(0):
        case REG_GE_MIU_PROT_HTH_L(0):
        case REG_GE_MIU_PROT_HTH_H(0):
        case REG_GE_MIU_PROT_LTH_L(1):
        case REG_GE_MIU_PROT_LTH_H(1):
        case REG_GE_MIU_PROT_HTH_L(1):
        case REG_GE_MIU_PROT_HTH_H(1):
        case REG_GE_TAG_L:
        case REG_GE_TAG_H:
        case REG_GE_VCMDQ_BASE_L:
        case REG_GE_VCMDQ_BASE_H:
        case REG_GE_VCMDQ_SIZE:

        //Palette
        case REG_GE_CLUT_L:
        case REG_GE_CLUT_H:
        case REG_GE_CLUT_CTRL:
    GE_REG(addr) = value;
            break;

        case REG_GE_CMD:
            for(i=0; i<GE_TABLE_REGNUM; i++)
            {
               //CMQ/Palette
               if(i==REG_GE_EN || i==REG_GE_CFG || i==REG_GE_DBG || i==REG_GE_TH || i==REG_GE_VCMDQ_STAT || i==REG_GE_BIST_STAT \
                || i==REG_GE_MIU_PROT_LTH_L(0) || i==REG_GE_MIU_PROT_LTH_H(0) || i==REG_GE_MIU_PROT_HTH_L(0) || i==REG_GE_MIU_PROT_HTH_H(0)\
                || i==REG_GE_MIU_PROT_LTH_L(1) || i==REG_GE_MIU_PROT_LTH_H(1) || i==REG_GE_MIU_PROT_HTH_L(1) || i==REG_GE_MIU_PROT_HTH_H(1)\
                || i==REG_GE_VCMDQ_BASE_L || i==REG_GE_VCMDQ_BASE_H || i==REG_GE_VCMDQ_SIZE \
                || i==REG_GE_CLUT_L || i==REG_GE_CLUT_H || i==REG_GE_CLUT_CTRL || i==REG_GE_TAG_L || i==REG_GE_TAG_H)
                {
                    continue;
                }

               if(i == REG_GE_CMD )
               {
                    continue;
               }

               if(i == (GE_TABLE_REGNUM-1))
                {
                    GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);
                    GE_REG(i)= pGEHalLocal->u16RegGETable[i];
    				GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);
                    GE_REG(REG_GE_CMD)= pGEHalLocal->u16RegGETable[REG_GE_CMD];
                }
                else
                {
                    GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);
                    GE_REG(i)= pGEHalLocal->u16RegGETable[i];
                }

            }
        break;
    default:
#if GE_LOG_ENABLE
        GE_LOG(addr, value);
#endif
        break;
    }

}

}

MS_U16 GE2_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr)
{
    MS_U16 u16NoFIFOMask=0;

    if(0x80 <= addr)
    {
        GE_WaitIdle(pGEHalLocal);
        return GE2_REG(addr-GE_TABLE_REGNUM);
    }

    if(0 == u16NoFIFOMask)
    {
        if(GE_Map_Share_RegEX(pGEHalLocal,addr)== E_GE_OK)
            return pGEHalLocal->pHALShared->u16ShareRegImageEx[addr];
        else
        {
            return pGEHalLocal->u16RegGETableEX[addr];
        }
    }
    return (GE2_REG(addr)&u16NoFIFOMask)|(pGEHalLocal->u16RegGETableEX[addr]&~u16NoFIFOMask);

}

void GE2_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value)
{
    // CMDQ special command
    if(addr < GE_TABLE_REGNUM)
    {
        if(GE_Map_Share_RegEX(pGEHalLocal,addr)== E_GE_OK)
        {
            pGEHalLocal->pHALShared->u16ShareRegImageEx[addr]= value;
        }
        pGEHalLocal->u16RegGETableEX[addr] = value;
    }
    else
    {
        printf("[%s][%d] Reg Index [%d]is out of GE_TABLE_REGNUM [%d]range!!!!\n",__FUNCTION__,__LINE__, addr, GE_TABLE_REGNUM);
    }
    GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);
    GE2_REG(addr)= value;
  return;
}


MS_U16 _GE1_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr)
{
    return GE1_REG(addr);
}


void GE1_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value)
{
    GE_WaitCmdQAvail(pGEHalLocal, GE_CMD_SIZE);
    GE1_REG(addr)= value;
    return;
}

void GE_RestoreReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value)
{
    // CMDQ special command
    switch (addr)
    {
    case REG_GE_CMD:
        break;
    //[OBSOLETE]
    default:
        GE_WriteReg(pGEHalLocal, addr, value);
        break;
    }
}


void GE_ResetState(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    GE_WaitIdle(pGEHalLocal);

    GE_WriteReg(pGEHalLocal, REG_GE_EN, GE_EN_GE);
#if GE_DITHER_RAND_ENABLE
    GE_WriteReg(pGEHalLocal, REG_GE_EN, GE_EN_GE | GE_EN_DITHER_RAND); //fixed random dither by default
#endif
    GE_WriteReg(pGEHalLocal, REG_GE_TH, 0x0000); //0(<half) will be default to be half

    GE_WriteReg(pGEHalLocal, REG_GE_LINE_STYLE, GE_LINEPAT_RST);
    GE_WriteReg(pGEHalLocal, REG_GE_BLT_SCK_MODE, GE_BLT_SCK_NEAREST);
    GE_WriteReg(pGEHalLocal, REG_GE_BLEND, GE_ALPHA_ARGB1555); //force alpha constant of ARGB"1"555 to be 1.0 by default
}


void GE_Init_RegImage(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U8 addr;

    for(addr = 0; addr<GE_TABLE_REGNUM; addr++)
    {
        if(GE_Map_Share_Reg(pGEHalLocal,addr)== E_GE_OK)
            pGEHalLocal->pHALShared->u16ShareRegImage[addr]= GE_REG(addr);
        pGEHalLocal->u16RegGETable[addr] = GE_REG(addr);
    }

}

void GE_Init(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_Config *cfg)
{
    MS_U16 u16temp =0;

    GE_WaitIdle(pGEHalLocal);

    u16temp = GE_ReadReg(pGEHalLocal, REG_GE_CFG);

    if ((u16temp & BIT(1)) != BIT(1)) //if VQ is Not Enabled
    {
#if GE_CMDQ_ENABLE
        u16temp = GE_ReadReg(pGEHalLocal, REG_GE_CFG);
        GE_WriteReg(pGEHalLocal, REG_GE_CFG, u16temp | GE_CFG_CMDQ); // enable command queue
#endif

        // Set default FMT for avoiding 1st set buffinfo error.
        GE_WriteReg(pGEHalLocal, REG_GE_FMT, (GE_FMT_ARGB1555<<GE_SRC_FMT_SHFT)+(GE_FMT_ARGB1555<<GE_DST_FMT_SHFT));

        if (cfg->u32VCmdQSize >= GE_VCMDQ_SIZE_MIN)
        {
            MS_PHYADDR u32VQAddr = cfg->u32VCmdQAddr;
            MS_U32 u32VcmdqBufSz = cfg->u32VCmdQSize;

            GE_SetVCmdBuffer(pGEHalLocal, u32VQAddr, u32VcmdqBufSz);

            u16temp = GE_ReadReg(pGEHalLocal, REG_GE_CFG);
            GE_WriteReg(pGEHalLocal, REG_GE_CFG, u16temp | GE_CFG_VCMDQ);
        }

        GE_ResetState(pGEHalLocal);
    }
    else
    {
        //No need to set command queue
        printf(" warning!!! Virtual Command queue has been activated!! \n");
    }
    //GE_Init_RegImage(pGEHalLocal);

    GE_WriteReg(pGEHalLocal, REG_GE_TH, GE_THRESHOLD_SETTING);

    u16temp = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TYPE_EN)&(~GE_TLB_MODE_MASK);
    GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TYPE_EN, u16temp);

    u16temp = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_EN);
    u16temp |= GE_TLB_EN;
    GE2_WriteReg(pGEHalLocal, REG_GE_TLB_EN, u16temp);

    u16temp = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TAG)&(~GE_TLB_TAG);
    GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TAG, u16temp);

#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)

    if (FALSE == bGeIrqInited)
    {
        _GE_InitGE1Regs(pGEHalLocal);

        if (FALSE == _GE_IsTagInterruptEnabled(pGEHalLocal))
        {
            _GE_Ctrl_IntMode(pGEHalLocal, E_GE_INT_TAG_MODE | E_GE_MASK_INT);
        }

        if (FALSE == MsOS_AttachInterrupt(E_INT_IRQ_GE, _GE_WaitTag_InterruptCbk))
        {
            GE_DBG("[%s, %d]: MsOS_AttachInterrupt failed \r\n", __FUNCTION__, __LINE__);
            return ;
        }
        MsOS_EnableInterrupt(E_INT_IRQ_GE);

        bGeIrqInited = TRUE;
    }

    if (s32GeWaitTagEventHandle < 0)
    {
        s32GeWaitTagEventHandle = MsOS_CreateEventGroup("GE_Wait_Event_Handle");
        if (s32GeWaitTagEventHandle < 0)
        {
            GE_DBG("[%s, %d]: ge_semid_waitTag = %ld", __FUNCTION__, __LINE__, s32GeWaitTagEventHandle);
            return;
        }
    }
#endif

}

GE_Result GE_SetRotate(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RotateAngle geRotAngle)
{
    MS_U16 u16RegVal;

    u16RegVal = (GE_ReadReg(pGEHalLocal, REG_GE_ROT_MODE) & ~REG_GE_ROT_MODE_MASK) | (geRotAngle<<REG_GE_ROT_MODE_SHFT);
    GE_WriteReg(pGEHalLocal, REG_GE_ROT_MODE, u16RegVal);

    return E_GE_OK;
}

GE_Result GE_SetOnePixelMode(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable)
{

    MS_U16    u16en;
    //GE_DBG("%s\n", __FUNCTION__);

    u16en = GE_ReadReg(pGEHalLocal, REG_GE_EN);
    if (enable)
    {
        u16en |= GE_EN_ONE_PIXEL_MODE;
    }
    else
    {
        u16en &= (~GE_EN_ONE_PIXEL_MODE);
    }
    u16en |= GE_EN_BURST;
    GE_WriteReg(pGEHalLocal, REG_GE_EN, u16en);

    return E_GE_OK;
}

GE_Result GE_SetBlend(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_BlendOp eBlendOp)
{
    MS_U16              u16op;

    switch (eBlendOp)
    {
    case E_GE_BLEND_ONE:
    case E_GE_BLEND_CONST:
    case E_GE_BLEND_ASRC:
    case E_GE_BLEND_ADST:
    case E_GE_BLEND_ROP8_ALPHA:
    case E_GE_BLEND_ROP8_SRCOVER:
    case E_GE_BLEND_ROP8_DSTOVER:
    case E_GE_BLEND_ZERO:
    case E_GE_BLEND_CONST_INV:
    case E_GE_BLEND_ASRC_INV:
    case E_GE_BLEND_ADST_INV:
    case E_GE_BLEND_ALPHA_ADST:
    case E_GE_BLEND_SRC_ATOP_DST:
    case E_GE_BLEND_DST_ATOP_SRC:
    case E_GE_BLEND_SRC_XOR_DST:
    case E_GE_BLEND_INV_CONST:

        u16op = eBlendOp;
        break;
    default:
        return E_GE_FAIL_PARAM;
        break;
    }

    u16op = (GE_ReadReg(pGEHalLocal, REG_GE_BLEND) & ~GE_BLEND_MASK) | u16op;
    GE_WriteReg(pGEHalLocal, REG_GE_BLEND, u16op);

    return E_GE_OK;
}


GE_Result GE_SetAlpha(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_AlphaSrc eAlphaSrc)
{
    MS_U16              u16src;

    switch (eAlphaSrc)
    {
    case E_GE_ALPHA_CONST:
    case E_GE_ALPHA_ASRC:
    case E_GE_ALPHA_ADST:
    case E_GE_ALPHA_ROP8_SRC:
    case E_GE_ALPHA_ROP8_IN:
    case E_GE_ALPHA_ROP8_DSTOUT:
    case E_GE_ALPHA_ROP8_SRCOUT:
    case E_GE_ALPHA_ROP8_OVER:
    case E_GE_ALPHA_ROP8_INV_CONST:
    case E_GE_ALPHA_ROP8_INV_ASRC:
    case E_GE_ALPHA_ROP8_INV_ADST:
    case E_GE_ALPHA_ROP8_SRC_ATOP_DST:
    case E_GE_ALPHA_ROP8_DST_ATOP_SRC:
    case E_GE_ALPHA_ROP8_SRC_XOR_DST:
    case E_GE_ALPHA_ROP8_INV_SRC_ATOP_DST:
    case E_GE_ALPHA_ROP8_INV_DST_ATOP_SRC:

        u16src = eAlphaSrc;
        break;
    default:
        return E_GE_FAIL_PARAM;
        break;
    }

    u16src = (GE_ReadReg(pGEHalLocal, REG_GE_ALPHA) & ~GE_ALPHA_MASK) | (u16src<<GE_ALPHA_SHFT);
    GE_WriteReg(pGEHalLocal, REG_GE_ALPHA, u16src);

    return E_GE_OK;
}

GE_Result   GE_QueryDFBBldCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 *pU16SupportedBldFlags)
{
    if(NULL == pU16SupportedBldFlags)
    {
        return E_GE_FAIL_PARAM;
    }

    (*pU16SupportedBldFlags) = E_GE_DFB_BLD_FLAG_ALL;

    return E_GE_OK;
}

GE_Result   GE_EnableDFBBld(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable)
{
    MS_U16 u16RegVal;

    u16RegVal = GE_ReadReg(pGEHalLocal, REG_GE_EN);

    if (enable)
    {
        u16RegVal |= GE_EN_DFB_BLD;
    }
    else
    {
        u16RegVal &= ~GE_EN_DFB_BLD;
    }

    GE_WriteReg(pGEHalLocal, REG_GE_EN, u16RegVal);

    return E_GE_OK;
}

GE_Result   GE_SetDFBBldFlags(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 u16DFBBldFlags)
{
    MS_U16 u16RegVal;

    u16RegVal = (GE_ReadReg(pGEHalLocal, REG_GE_DFB_BLD_FLAGS) & ~GE_DFB_BLD_FLAGS_MASK);

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_COLORALPHA)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_COLORALPHA;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_ALPHACHANNEL)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_ALPHACHANNEL;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_COLORIZE)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_COLORIZE;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_SRCPREMUL)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_SRCPREMUL;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_SRCPREMULCOL)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_SRCPREMULCOL;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_DSTPREMUL)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_DSTPREMUL;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_XOR)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_XOR;
    }

    if(u16DFBBldFlags & E_GE_DFB_BLD_FLAG_DEMULTIPLY)
    {
        u16RegVal |= GE_DFB_BLD_FLAG_DEMULTIPLY;
    }

    GE_WriteReg(pGEHalLocal, REG_GE_DFB_BLD_FLAGS, u16RegVal);


    u16RegVal = (GE_ReadReg(pGEHalLocal, REG_GE_DFB_BLD_OP) & ~GE_DFB_SRC_COLORMASK);

    if(u16DFBBldFlags & (E_GE_DFB_BLD_FLAG_SRCCOLORMASK | E_GE_DFB_BLD_FLAG_SRCALPHAMASK))
    {
        u16RegVal |= (1 << GE_DFB_SRC_COLORMASK_SHIFT);
    }



    return E_GE_OK;
}

GE_Result   GE_SetDFBBldOP(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_DFBBldOP geSrcBldOP, GE_DFBBldOP geDstBldOP)
{
    MS_U16 u16RegVal;

    u16RegVal = (GE_ReadReg(pGEHalLocal, REG_GE_DFB_BLD_OP) & ~(GE_DFB_SRCBLD_OP_MASK|GE_DFB_DSTBLD_OP_MASK));
    u16RegVal |= ((geSrcBldOP<<GE_DFB_SRCBLD_OP_SHFT) | (geDstBldOP<<GE_DFB_DSTBLD_OP_SHFT));

    GE_WriteReg(pGEHalLocal, REG_GE_DFB_BLD_OP, u16RegVal);

    return E_GE_OK;
}

GE_Result   GE_SetDFBBldConstColor(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RgbColor geRgbColor)
{
    MS_U16 u16RegVal;

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_ALPHA_CONST) & ~GE_ALPHA_CONST_MASK) | (geRgbColor.a & 0xFF));
    GE_WriteReg(pGEHalLocal, REG_GE_ALPHA_CONST, u16RegVal);

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_R_CONST) & ~GE_R_CONST_MASK) | ((geRgbColor.r<<GE_R_CONST_SHIFT) & GE_R_CONST_MASK));
    GE_WriteReg(pGEHalLocal, REG_GE_R_CONST, u16RegVal);

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_G_CONST) & ~GE_G_CONST_MASK) | ((geRgbColor.g<<GE_G_CONST_SHIFT) & GE_G_CONST_MASK));
    GE_WriteReg(pGEHalLocal, REG_GE_G_CONST, u16RegVal);

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_B_CONST) & ~GE_B_CONST_MASK) | ((geRgbColor.b<<GE_B_CONST_SHIFT) & GE_B_CONST_MASK));
    GE_WriteReg(pGEHalLocal, REG_GE_B_CONST, u16RegVal);

    return E_GE_OK;
}

GE_Result   GE_SetDFBBldSrcColorMask(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RgbColor geRgbColor)
{
//no more hw function
/*
    MS_U16 u16RegVal;

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_ALPHA_CONST) & ~GE_ALPHA_SRCMASK_MASK) | (geRgbColor.a & 0xFF));
    GE_WriteReg(pGEHalLocal, REG_GE_ALPHA_CONST, u16RegVal);

    u16RegVal = ((GE_ReadReg(pGEHalLocal, REG_GE_OP_MODE) & ~GE_SRCCOLOR_MASK_R) | ((geRgbColor.r<<GE_SRCCOLOR_MASK_R_SHIFT) & GE_SRCCOLOR_MASK_R));
    GE_WriteReg(pGEHalLocal, REG_GE_OP_MODE, u16RegVal);

    u16RegVal = (geRgbColor.g<<GE_SRCCOLOR_MASK_G_SHIFT) | (geRgbColor.b<<GE_SRCCOLOR_MASK_B_SHIFT);
    GE_WriteReg(pGEHalLocal, REG_GE_SRCMASK_GB, u16RegVal);
*/
    return E_GE_OK;
}


GE_Result GE_WriteProtect(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 miu, MS_PHYADDR addr_low, MS_PHYADDR addr_high, GE_WPType eWPType)
{
    MS_U16              u16cfg;

    if (miu > 1)
    {
        return E_GE_FAIL;
    }

    if ( (eWPType == E_GE_WP_IN_RANGE) || (eWPType == E_GE_WP_OUT_RANGE) )
    {
        // range setting
        GE_WriteReg(pGEHalLocal, REG_GE_MIU_PROT_LTH_L(miu),  addr_low & (GE_MIU_ADDR_MASK&0xFFFF));
        GE_WriteReg(pGEHalLocal, REG_GE_MIU_PROT_LTH_H(miu), ((addr_low>>16) & (GE_MIU_ADDR_MASK>>16)) | (eWPType<<GE_MIU_PROT_MODE_SHFT));
        GE_WriteReg(pGEHalLocal, REG_GE_MIU_PROT_HTH_L(miu), addr_high & (GE_MIU_ADDR_MASK&0xFFFF));
        GE_WriteReg(pGEHalLocal, REG_GE_MIU_PROT_HTH_H(miu), (addr_high>>16) & (GE_MIU_ADDR_MASK>>16));
        // enable setting
        u16cfg = GE_ReadReg(pGEHalLocal, REG_GE_CFG) | (GE_CFG_MIU0_PROT << miu);
        GE_WriteReg(pGEHalLocal, REG_GE_CFG, u16cfg);
    }
    else if (eWPType == E_GE_WP_DISABLE)
    {
        u16cfg = GE_ReadReg(pGEHalLocal, REG_GE_CFG) & ~(GE_CFG_MIU0_PROT<<miu);
        GE_WriteReg(pGEHalLocal, REG_GE_CFG, u16cfg);
    }
    else
    {
        return E_GE_FAIL;
    }

    return E_GE_OK;
}


GE_Result GE_SetSrcTile(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL tile)
{
    //GE_DBG("%s\n", __FUNCTION__);

    return E_GE_NOT_SUPPORT;

}


GE_Result GE_SetDstTile(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL tile)
{
    //GE_DBG("%s\n", __FUNCTION__);

    return E_GE_NOT_SUPPORT;

}
GE_Result GE_SetASCK(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable)
{
    MS_U16              u16cfg;

    u16cfg = GE_ReadReg(pGEHalLocal, REG_GE_EN);
    if (enable)
    {
        u16cfg |= GE_EN_ASCK;
    }
    else
    {
        u16cfg &= ~GE_EN_ASCK;
    }
    GE_WriteReg(pGEHalLocal, REG_GE_EN, u16cfg);

    return E_GE_OK;
}
GE_Result GE_SetADCK(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable)
{
    MS_U16              u16cfg;

    u16cfg = GE_ReadReg(pGEHalLocal, REG_GE_EN);
    if (enable)
    {
        u16cfg |= GE_EN_DSCK;
    }
    else
    {
        u16cfg &= ~GE_EN_DSCK;
    }
    GE_WriteReg(pGEHalLocal, REG_GE_EN, u16cfg);

    return E_GE_OK;
}


GE_Result GE_GetFmtCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_BufFmt fmt, GE_BufType type, GE_FmtCaps *caps)
{
    static const MS_U8 _u8GETileWidth[] = {8, 4, 2, 0, 1};

    caps->fmt = fmt;
    if (type == E_GE_BUF_SRC)
    {
        switch (fmt)
        {
        case E_GE_FMT_I1:
        case E_GE_FMT_I2:
        case E_GE_FMT_I4:
        case E_GE_FMT_I8:
            caps->u8BaseAlign = 1;
            caps->u8PitchAlign = 1;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 1;
            caps->u8TileBaseAlign = 0x80;//[HWBUG] 8;
            caps->u8TileWidthAlign = _u8GETileWidth[fmt];
            caps->u8TileHeightAlign = 16;
            break;
        case E_GE_FMT_RGB565:
        case E_GE_FMT_RGBA5551:
        case E_GE_FMT_RGBA4444:
        case E_GE_FMT_ARGB1555:
        case E_GE_FMT_1ABFgBg12355:
        case E_GE_FMT_ARGB4444:
        case E_GE_FMT_YUV422:
        case E_GE_FMT_FaBaFgBg2266:
            caps->u8BaseAlign = 2;
            caps->u8PitchAlign = 2;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 2;
            caps->u8TileBaseAlign = 0x80;//[HWBUG] 8;
            caps->u8TileWidthAlign = 16;
            caps->u8TileHeightAlign = 16;
            break;
        case E_GE_FMT_ABGR8888:
        case E_GE_FMT_ARGB8888:
            caps->u8BaseAlign = 4;
            caps->u8PitchAlign = 4;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 4;
            caps->u8TileBaseAlign = 0x80;//[HWBUG] 8;
            caps->u8TileWidthAlign = 8;
            caps->u8TileHeightAlign = 16;
            break;
        // Not Support
        default:
            caps->fmt = E_GE_FMT_GENERIC;
            caps->u8BaseAlign = 4;
            caps->u8PitchAlign = 4;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 4;
            caps->u8TileBaseAlign = 0;
            caps->u8TileWidthAlign = 0;
            caps->u8TileHeightAlign = 0;
            return E_GE_FAIL_FORMAT;
        }
    }
    else
    {
        switch (fmt)
        {
        case E_GE_FMT_I8:
            caps->u8BaseAlign = 1;
            caps->u8PitchAlign = 1;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 1;
            caps->u8TileBaseAlign = 8;
            caps->u8TileWidthAlign = _u8GETileWidth[fmt];
            caps->u8TileHeightAlign = 16;
            break;
        case E_GE_FMT_RGB565:
        case E_GE_FMT_ARGB1555:
        case E_GE_FMT_RGBA5551:
        case E_GE_FMT_RGBA4444:
        case E_GE_FMT_1ABFgBg12355:
        case E_GE_FMT_ARGB4444:
        case E_GE_FMT_YUV422:
        case E_GE_FMT_FaBaFgBg2266:
        case E_GE_FMT_ARGB1555_DST:
            caps->u8BaseAlign = 2;
            caps->u8PitchAlign = 2;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 2;
            caps->u8TileBaseAlign = 8;
            caps->u8TileWidthAlign = 16;
            caps->u8TileHeightAlign = 16;
            break;
        case E_GE_FMT_ABGR8888:
        case E_GE_FMT_ARGB8888:
            caps->u8BaseAlign = 4;
            caps->u8PitchAlign = 4;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 4;
            caps->u8TileBaseAlign = 8;
            caps->u8TileWidthAlign = 8;
            caps->u8TileHeightAlign = 16;
            break;
        // Not Support
        case E_GE_FMT_I1:
        case E_GE_FMT_I2:
        case E_GE_FMT_I4:
        default:
            caps->fmt = E_GE_FMT_GENERIC;
            caps->u8BaseAlign = 4;
            caps->u8PitchAlign = 4;
            caps->u8Non1pAlign = 0;
            caps->u8HeightAlign = 1;
            caps->u8StretchAlign = 4;
            caps->u8TileBaseAlign = 0;
            caps->u8TileWidthAlign = 0;
            caps->u8TileHeightAlign = 0;
            return E_GE_FAIL_FORMAT;
        }
    }

    return E_GE_OK;
}


GE_Result GE_Set_IOMap_Base(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U32 addr)
{
    pGEHalLocal->u32_mmio_base = addr;

#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
    s32HalIomapBaseAddr = addr;
#endif

    return E_GE_OK;
}


static  int direct_serial_diff( MS_U16 tagID1,  MS_U16 tagID2)
{
    if(tagID1 < tagID2)
    {
        if((tagID2-tagID1)>0x7FFF)
         {
             return (int)(0xFFFFUL-tagID2+tagID1+1);
         }
        else
            return -(int)(tagID2-tagID1);
    }
    else
    {
        if((tagID1-tagID2)>0x7FFF)
         {
             return -(int)(0xFFFFUL-tagID1+tagID2+1);
         }
        else
            return (int)(tagID1-tagID2);
    }
}

//-------------------------------------------------------------------------------------------------
/// Wait GE TagID back
/// @param  tagID                     \b IN: tag id number for wating
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_WaitTAGID(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tagID)
{
     MS_U16 tagID_HW;
     MS_U32 u32Temp;
#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
    MS_U8  timeout_count = 0;
    MS_U32 event = 0x0;
    MS_BOOL bHadPrintOutDbgMsg = FALSE;
#endif

#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
    tagID_HW = GE_ReadReg(pGEHalLocal, REG_GE_TAG_L);
    if (tagID_HW >= tagID)
    {
        return E_GE_OK;
    }

#if defined(MSOS_TYPE_LINUX)
    s32WaitingTagPid = (MS_S32)getpid();
#endif

    MsOS_ClearEvent(s32GeWaitTagEventHandle, 0x1);

    // unmask ge interrupt
    _GE_Ctrl_IntMode(pGEHalLocal, E_GE_CLEAR_INT | E_GE_UNMASK_INT);

    GE1_WriteReg(pGEHalLocal, REG_GE_INT_TAG_COND_H, 0);
    GE1_WriteReg(pGEHalLocal, REG_GE_INT_TAG_COND_L, tagID);

    // make a fake busy
    GE1_WriteReg(pGEHalLocal, REG_GE_CMD, 0x30);

    while (MsOS_WaitEvent(s32GeWaitTagEventHandle, 0x1, &event, E_OR_CLEAR, GE_TAG_INTERRUPT_WAITING_TIME) == FALSE)
    {
        tagID_HW = GE_ReadReg(pGEHalLocal, REG_GE_TAG_L);
        if (direct_serial_diff(tagID_HW, tagID) >= 0)
        {
            break;
        }

        timeout_count++;

        if ((bHadPrintOutDbgMsg == FALSE) && (timeout_count > GE_TAG_INTERRUPT_DEBUG_PRINT_THRESHOLD))
        {
            _GE_Print_GeWaitTagTimeout_Msg(pGEHalLocal, tagID);
            bHadPrintOutDbgMsg = TRUE;
        }

        if(GE_ReadReg(pGEHalLocal, REG_GE_STAT) & GE_STAT_BUSY)
            continue;

        break;
    }

    // mask ge interrupt
    s32WaitingTagPid = 0;
    _GE_Ctrl_IntMode(pGEHalLocal, E_GE_MASK_INT);

#endif

    while(1)
    {
        tagID_HW = GE_ReadReg(pGEHalLocal, REG_GE_TAG_L);
        if(direct_serial_diff(tagID_HW, tagID) >= 0)
        {
            //printf("tagIDHW = %04x %04x\n", tagID_HW, tagID);
            break;
        }

        u32Temp = GE_ReadReg(pGEHalLocal, REG_GE_STAT);
        if((u32Temp&GE_STAT_CMDQ_MASK) < (16UL<<11))
        {
            GE_YIELD();
            continue;
        }
        if((u32Temp&GE_STAT_CMDQ2_MASK) < (16UL<<3))
        {
            GE_YIELD();
            continue;
        }
        if(GE_ReadReg(pGEHalLocal, REG_GE_CFG) & GE_CFG_VCMDQ)
        {
            u32Temp = GE_ReadReg(pGEHalLocal, REG_GE_VCMDQ_STAT);
            u32Temp |= (GE_ReadReg(pGEHalLocal, REG_GE_BIST_STAT)&1)<<16;
            if(u32Temp)
                continue;
        }

        if(GE_ReadReg(pGEHalLocal, REG_GE_STAT) & GE_STAT_BUSY)
            continue;

        break;
    }

    return E_GE_OK;
}
//-------------------------------------------------------------------------------------------------
/// MDrv_GE_SAVE_CHIP_IMAGE
//-------------------------------------------------------------------------------------------------
GE_Result GE_Restore_HAL_Context(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U16 i = 0;
    MS_U16 u16RegVal;

    //GE_WaitIdle(pGEHalLocal);

    while( (_GE_Reg_Backup[i] != 0xFF) )
    {
        if(_GE_Reg_Backup[i]>= 0x80)
        {
            break;
        }

        u16RegVal = GE_ReadReg(pGEHalLocal, _GE_Reg_Backup[i]);
        GE_RestoreReg(pGEHalLocal, _GE_Reg_Backup[i], u16RegVal);
        i++;
    }

    //GE_DBG(printf("GE_Restore_HAL_Context finished \n\n"));

    return E_GE_OK;
}

//-------------------------------------------------------------------------------------------------
/// Calculate Blit Scale Ratio:
//-------------------------------------------------------------------------------------------------
GE_Result GE_CalcBltScaleRatio(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 u16SrcWidth, MS_U16 u16SrcHeight, MS_U16 u16DstWidth, MS_U16 u16DstHeight, GE_ScaleInfo *pScaleinfo)
{
    if(NULL == pScaleinfo)
    {
        return E_GE_FAIL_PARAM;
    }

    if(u16SrcWidth >= (u16DstWidth<< g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin))
    {
        pScaleinfo->x = 0xFFFFFFFF;
    }
    else
    {
        pScaleinfo->x = GE_Divide2Fixed(u16SrcWidth, u16DstWidth, g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin, 12);
    }

    if(u16SrcHeight >= (u16DstHeight<< g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin))
    {
        pScaleinfo->y = 0xFFFFFFFF;
    }
    else
    {
        pScaleinfo->y = GE_Divide2Fixed(u16SrcHeight, u16DstHeight, g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin, 12);
    }

    /* HW use format S0.12 which means Bit(12) should be Sign bit
    // If overflow, S bit maybe wrong, handle it as actually value we hoped*/
    pScaleinfo->init_x = GE_Divide2Fixed(u16SrcWidth-u16DstWidth, 2 * u16DstWidth, 0, 12);
    if((u16SrcWidth/4) > u16DstWidth)
    {
        pScaleinfo->init_x = 0x1000;
    }
    else if(u16SrcWidth >= u16DstWidth)
    {
        pScaleinfo->init_x &= (~(1<<12));
    }
    else
    {
        pScaleinfo->init_x |= (1<<12);
    }

    pScaleinfo->init_y = GE_Divide2Fixed(u16SrcHeight-u16DstHeight, 2 * u16DstHeight, 0, 12);
    if((u16SrcHeight/4) > u16DstHeight)
    {
        pScaleinfo->init_y = 0x1000;
    }
    else if(u16SrcHeight >= u16DstHeight)
    {
        pScaleinfo->init_y &= (~(1<<12));
    }
    else
    {
        pScaleinfo->init_y |= (1<<12);
    }

    if (pGEHalLocal->bYScalingPatch)
    {
        if (u16SrcHeight<=5)
            pScaleinfo->init_y = (1<<12);
    }
    return E_GE_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set GE scale register
/// @param  GE_Rect *src                    \b IN: src coordinate setting
/// @param  GE_DstBitBltType *dst           \b IN: dst coordinate setting
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_SetBltScaleRatio(GE_CTX_HAL_LOCAL *pGEHalLocal,GE_Rect *src, GE_DstBitBltType *dst, GE_Flag flags, GE_ScaleInfo* scaleinfo)
{
    GE_ScaleInfo geScaleinfo, *pGeScaleInfo = scaleinfo;

    if(flags & E_GE_FLAG_BYPASS_STBCOEF)
    {
        _GE_SetBltScaleRatio2HW(pGEHalLocal, pGeScaleInfo);
    }
    else if (flags & E_GE_FLAG_BLT_STRETCH)
    {
        /* Safe Guard. Prevent set scaling ratio < 1/32. Also prevent 0 h/w */
        if ((src->width-1) >= (dst->dstblk.width << g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin))
        {
            if(pGEHalLocal->bIsComp == FALSE)
            {
                return E_GE_FAIL_PARAM;
            }

            dst->dstblk.width = ((src->width-1) >> g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin) + 1;
        }
        if ((src->height-1) >= (dst->dstblk.height << g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin))
        {
            if(pGEHalLocal->bIsComp == FALSE)
            {
                return E_GE_FAIL_PARAM;
            }

            dst->dstblk.height = ((src->height-1) >> g_GeChipPro.BltDownScaleCaps.u8ShiftRangeMin) + 1;
        }

        pGeScaleInfo = &geScaleinfo;
        GE_CalcBltScaleRatio(pGEHalLocal, src->width, src->height, dst->dstblk.width, dst->dstblk.height, pGeScaleInfo);
        _GE_SetBltScaleRatio2HW(pGEHalLocal, pGeScaleInfo);
    }
    else
    {
        pGeScaleInfo = &geScaleinfo;

        pGeScaleInfo->x = (1<<12);
        pGeScaleInfo->y = (1<<12);
        pGeScaleInfo->init_x = 0;
        pGeScaleInfo->init_y = 0;

        _GE_SetBltScaleRatio2HW(pGEHalLocal, pGeScaleInfo);
    }

    return E_GE_OK;
}

GE_Result GE_BitBltEX_Trape(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_Rect *pSrcRect, GE_Normalized_Trapezoid *pGENormTrapezoid, MS_U32 u32Flags, GE_ScaleInfo* pScaleinfo)
{
    return E_GE_NOT_SUPPORT;
}

//-------------------------------------------------------------------------------------------------
/// GE Primitive Drawing - TRAPEZOID
/// @param  pGENormTrapezoid                    \b IN: pointer to position of TRAPEZOID
/// @param  u32ColorS                   \b IN: start color of TRAPEZOID when gradient
/// @param  u32ColorE                   \b IN: end color of TRAPEZOID when gradient
/// @param  pColorDeltaX                  \b IN: x gradient color
/// @param  pColorDeltaY                   \b IN:  y gradient color
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_FillTrapezoid(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bYTrapezoid, GE_Normalized_Trapezoid *pGENormTrapezoid, MS_U32 u32Color, GE_ColorDelta *pColorDeltaX, GE_ColorDelta *pColorDeltaY)
{
    return E_GE_NOT_SUPPORT;
}

//-------------------------------------------------------------------------------------------------
/// Set GE DISABLE MIU ACCESS
/// @param  enable                  \b IN: enable and update setting
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_SetDisaMIUAccess(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable)
{
    MS_U16              u16en;

    GE_DBG("%s\n", __FUNCTION__);

    u16en = GE_ReadReg(pGEHalLocal,REG_GE_CFG);
    if (enable)
    {
        u16en |= GE_CFG_DISABLE_MIU_ACS;
    }
    else
    {
        u16en &= ~GE_CFG_DISABLE_MIU_ACS;
    }
    GE_WriteReg(pGEHalLocal,REG_GE_CFG, u16en);

    return E_GE_OK;
}
//-------------------------------------------------------------------------------------------------
/// Set GE Clear Invalid MIU Flag
/// @param  enable                  \b IN: enable and update setting
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_ClrInvalMIUFlg(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable)
{
    MS_U16              u16en;

    GE_DBG("%s\n", __FUNCTION__);

    u16en = GE_ReadReg(pGEHalLocal,REG_GE_CFG);
    if (enable)
    {
        u16en |= GE_CFG_CLR_MIU_FLG;
    }
    else
    {
        u16en &= ~GE_CFG_CLR_MIU_FLG;
    }
    GE_WriteReg(pGEHalLocal,REG_GE_CFG, u16en);

    return E_GE_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set Enable Dynamic Clock Gating
/// @param  enable                  \b IN: enable and update setting
/// @return @ref GE_Result
//-------------------------------------------------------------------------------------------------
GE_Result GE_EnableDynaClkGate(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable)
{
    MS_U16              u16en;

    GE_DBG("%s\n", __FUNCTION__);

    u16en = GE_ReadReg(pGEHalLocal,REG_GE_CFG);
    if (enable)
    {
        u16en |= GE_CFG_EN_DNY_CLK_GATE;
    }
    else
    {
        u16en &= ~GE_CFG_EN_DNY_CLK_GATE;
    }
    GE_WriteReg(pGEHalLocal,REG_GE_CFG, u16en);

    return E_GE_OK;
}

GE_Result GE_EnableTrapezoidAA(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable)
{
    //GE_DBG("%s\n", __FUNCTION__);

    return E_GE_NOT_SUPPORT;

}

GE_Result GE_EnableTrapSubPixCorr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable)
{
    //GE_DBG("%s\n", __FUNCTION__);

    return E_GE_NOT_SUPPORT;

}

MS_U16  GE_GetNextTAGID(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bStepTagBefore)
{
    MS_U16 tagID;
    if(bStepTagBefore)
    {
        if(0 == ++pGEHalLocal->pHALShared->global_tagID)
             ++pGEHalLocal->pHALShared->global_tagID;
    }
    tagID =pGEHalLocal->pHALShared->global_tagID;

    return tagID;
}

GE_Result GE_SetVCmdBuffer(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHYADDR u32Addr, GE_VcmqBufSize enBufSize)
{
    MS_U16 u16RegVal;
    MS_U16 i=0;

    if(enBufSize >= E_GE_VCMD_1024K)
    {
        return E_GE_NOT_SUPPORT;
    }

    GE_SetVQBufMIUId(pGEHalLocal, _GFXAPI_MIU_ID(u32Addr));
    u32Addr = GE_ConvertAPIAddr2HAL(pGEHalLocal, _GFXAPI_MIU_ID(u32Addr), _GFXAPI_PHYS_ADDR_IN_MIU(u32Addr));

#ifdef GE_VQ_MIU_HANG_PATCH
    pGEHalLocal->pHALShared->u8VCmdQMiu = _GFXAPI_MIU_ID(u32Addr);
#endif

#if GE_VQADDR_LOCK_PATCH
    for (i=0; i<GE_VQADDR_ENHANCE_LOCK_TIMES; i++)
    {
        GE_WriteReg(pGEHalLocal, REG_GE_VCMDQ_BASE_H, u32Addr >> 16);        // Address
        GE_WriteReg(pGEHalLocal, REG_GE_VCMDQ_BASE_L, u32Addr & 0xffff);     // Address
    }
#else
    GE_WriteReg(pGEHalLocal, REG_GE_VCMDQ_BASE_L, u32Addr & 0xffff);     // Address
    GE_WriteReg(pGEHalLocal, REG_GE_VCMDQ_BASE_H, u32Addr >> 16);        // Address
#endif

    u16RegVal = (GE_ReadReg(pGEHalLocal, REG_GE_VCMDQ_SIZE) & ~GE_VCMDQ_SIZE_MASK) | ((GE_MapVQ2Reg(enBufSize) & GE_VCMDQ_SIZE_MASK));
    GE_WriteReg(pGEHalLocal, REG_GE_VCMDQ_SIZE, u16RegVal);

    return E_GE_OK;
}

GE_Result GE_InitCtxHalPalette(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U32 u32Idx;

    for(u32Idx=0; u32Idx<GE_PALETTE_NUM; u32Idx++)
    {
        GE_WriteReg(pGEHalLocal, REG_GE_CLUT_CTRL, ((u32Idx) & GE_CLUT_CTRL_IDX_MASK) | GE_CLUT_CTRL_RD);
        GE_WaitIdle(pGEHalLocal);
        pGEHalLocal->u32Palette[u32Idx] = ByteSwap32(((GE_ReadReg(pGEHalLocal, REG_GE_CLUT_H)<<16) | GE_ReadReg(pGEHalLocal, REG_GE_CLUT_L)));
    }

    pGEHalLocal->bPaletteDirty = FALSE;

    return (E_GE_OK);
}

void GE_Init_HAL_Context(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_CTX_HAL_SHARED *pHALShared, MS_BOOL bNeedInitShared)
{
     memset(pGEHalLocal, 0, sizeof(*pGEHalLocal));

     if(bNeedInitShared)
     {
         memset(pHALShared, 0, sizeof(*pHALShared));
         pHALShared->global_tagID = 1;
     }
     pGEHalLocal->pHALShared = pHALShared;
     pGEHalLocal->bYScalingPatch = FALSE;
}

GE_Result GE_Set_IOMap_Base2(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U32 addr)
{
    pGEHalLocal->u32_mmio_base2 = addr;
    return E_GE_OK;
}

GE_Result GE_SetClock(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bOnOff)
{
    MS_U8 u8tmp = 0;

    u8tmp = READ_BYTE( pGEHalLocal->u32_mmio_base + (CHIP_CLK_BASE) + CHIP_GE_CLK);

    if (bOnOff)
    {
        u8tmp &= ~ BIT(0);
    }
    else
    {
        u8tmp |= BIT(0);
    }
    WRITE_BYTE(pGEHalLocal->u32_mmio_base + (CHIP_CLK_BASE) + CHIP_GE_CLK, u8tmp);
    return E_GE_OK;

}

MS_BOOL GE_NonOnePixelModeCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, PatchBitBltInfo* patchInfo)
{
    GE_ScaleInfo geScaleinfo;
    GE_Result ret;

    patchInfo->scaleinfo =&geScaleinfo;
    ret = GE_CalcBltScaleRatio(pGEHalLocal, patchInfo->src.width , patchInfo->src.height ,patchInfo->dst.dstblk.width , patchInfo->dst.dstblk.height, patchInfo->scaleinfo);

    if(ret == E_GE_FAIL_PARAM)
    {
        return pGEHalLocal->pGeChipPro->bFourPixelModeStable;
    }
    else if ((patchInfo->scaleinfo->init_x>0xFFF)||(patchInfo->scaleinfo->init_y>0xFFF))
    {
         return FALSE;
    }
    else
    {
   return pGEHalLocal->pGeChipPro->bFourPixelModeStable;
    }
}

GE_Result HAL_GE_EnableCalcSrc_WidthHeight(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable)
{
    MS_U16 u16en;

    u16en = GE_ReadReg(pGEHalLocal, REG_GE_EN);

    if(bEnable)
    {
        if(u16en & GE_EN_BURST)
        {
            GE_WriteReg(pGEHalLocal, REG_GE_EN, u16en | GE_EN_CALC_SRC_WH);
        }
    }
    else
    {
        GE_WriteReg(pGEHalLocal, REG_GE_EN, u16en & (~GE_EN_CALC_SRC_WH));
    }

    return E_GE_OK;
}

GE_Result GEWD_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16* value)
{
    //For two source buffer read register
    return E_GE_NOT_SUPPORT;
}

GE_Result GEWD_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value)
{
    //For two source buffer write register
    return E_GE_NOT_SUPPORT;
}

GE_Result GE_SetTLBMode(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_TLB_Mode tlb_type)
{
    MS_U16 u16cmd = 0;

    u16cmd = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TYPE_EN)&(~GE_TLB_MODE_MASK);

    if(tlb_type == (u16cmd>>1))
    {
        return E_GE_OK;
    }
    else
    {
        switch(tlb_type)
        {
            case E_GE_TLB_NONE:
                break;
            case E_GE_TLB_SRC:
                u16cmd |= GE_TLB_SRC;
                break;
            case E_GE_TLB_DST:
                u16cmd |= GE_TLB_DST;
                break;
            case E_GE_TLB_SRC_DST:
                u16cmd |= (GE_TLB_SRC|GE_TLB_DST);
                break;
            default:
                printf("[%s,%5d]Do Nothing: Invalid TLB Format\n",__FUNCTION__,__LINE__);
                return E_GE_FAIL;
        }
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TYPE_EN, u16cmd);
    }
    return E_GE_OK;
}

GE_Result GE_GetTLBSRCADDR(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHYADDR* addr)
{
    MS_U16 miu=0;

    miu= (GE2_ReadReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_H)&0x8000)>>15;
    *addr = ((GE2_ReadReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_H)&0x7fff)<<16)|GE2_ReadReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_L);

    if(miu==1)
    {
        *addr += HAL_MIU1_BASE;
    }
    return E_GE_OK;
}

GE_Result GE_GetTLBDSTADDR(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHYADDR* addr)
{
    MS_U16 miu=0;

    miu= (GE2_ReadReg(pGEHalLocal, REG_GE_DST_TLB_BASE_H)&0x8000)>>15;
    *addr = ((GE2_ReadReg(pGEHalLocal, REG_GE_DST_TLB_BASE_H)&0x7fff)<<16)|GE2_ReadReg(pGEHalLocal, REG_GE_DST_TLB_BASE_L);

    if(miu==1)
    {
        *addr += HAL_MIU1_BASE;
    }

    return E_GE_OK;
}

GE_Result GE_SetTLBSrcBaseAddr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHYADDR addr)
{
    MS_U16 regval=0;
    MS_U8 u8miu=0;
    MS_U32 u32offset=0;

    u8miu= _GFXAPI_MIU_ID(addr);
    u32offset=_GFXAPI_PHYS_ADDR_IN_MIU(addr);

    if(u8miu==0)
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_L, u32offset);
        GE2_WriteReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_H, (u32offset&0x7fff0000)>>16);
        regval= GE2_ReadReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H)&(~GE_SB_TLB_SRC_MIU_SEL_H);
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H, regval);
    }
    else if(u8miu==1)
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_L, u32offset);
        GE2_WriteReg(pGEHalLocal, REG_GE_SRC_TLB_BASE_H, ((u32offset&0x7fff0000)>>16)|(u8miu<<15));
        regval= GE2_ReadReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H)&(~GE_SB_TLB_SRC_MIU_SEL_H);
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H, regval);
    }
    else
    {
        return E_GE_FAIL;
    }

    return E_GE_OK;
}

GE_Result GE_SetTLBDstBaseAddr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHYADDR addr)
{
    MS_U16 regval=0;
    MS_U8 u8miu=0;
    MS_U32 u32offset=0;

    u8miu= _GFXAPI_MIU_ID(addr);
    u32offset=_GFXAPI_PHYS_ADDR_IN_MIU(addr);

    if(u8miu==0)
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_DST_TLB_BASE_L, u32offset);
        GE2_WriteReg(pGEHalLocal, REG_GE_DST_TLB_BASE_H, ((u32offset&0x7fff0000)>>16));
        regval= GE2_ReadReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H)&(~GE_SB_TLB_DST_MIU_SEL_H);
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H, regval);
    }
    else if(u8miu==1)
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_DST_TLB_BASE_L, u32offset);
        GE2_WriteReg(pGEHalLocal, REG_GE_DST_TLB_BASE_H, ((u32offset&0x7fff0000)>>16)|(u8miu<<15));
        regval= GE2_ReadReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H)&(~GE_SB_TLB_DST_MIU_SEL_H);
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_BASE_MIU_H, regval);
    }
    else
    {
        return E_GE_FAIL;
    }

    return E_GE_OK;
}

GE_Result GE_FlushTLBTable(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable)
{
    MS_U16 u16en;

    u16en = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TYPE_EN);

    if(bEnable)
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TYPE_EN, u16en|GE_TLB_FLUSH);
    }
    else
    {
        GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TYPE_EN, u16en&(~GE_TLB_FLUSH));
    }

    return E_GE_OK;
}

GE_Result GE_SetTLBTag(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tag)
{
    MS_U16 reg_val;

    reg_val = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TAG)&(~GE_TLB_TAG);
    reg_val |= tag;

    GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TAG, reg_val);

    return E_GE_OK;
}

GE_Result GE_StopFlushTLB(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
    MS_U16 u16en;

    u16en = GE2_ReadReg(pGEHalLocal, REG_GE_TLB_TYPE_EN);

    GE2_WriteReg(pGEHalLocal, REG_GE_TLB_TYPE_EN, u16en & (~GE_TLB_FLUSH));

    return E_GE_OK;
}

GE_Result GE_Get_MIU_INTERVAL(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 miu, MS_U32* value)
{
#ifdef MSOS_TYPE_LINUX
    if(miu)
    {
        *value = MIU_INTERVAL;
    }
    else
    {
        *value = 0;
    }
#else
    *value = 0;
#endif
    return E_GE_OK;
}

GE_Result HAL_GE_AdjustDstWin( GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bDstXInv )
{
    MS_U16  u16ClipL=0,u16ClipR=0;
    MS_U16  u16DstX=0;

    u16DstX  = GE_ReadReg(pGEHalLocal, REG_GE_PRIM_V1_X);
    if( bDstXInv==FALSE )
    {
        u16ClipR = GE_ReadReg(pGEHalLocal, REG_GE_CLIP_R);
        if( u16ClipR < u16DstX )
        {
            GE_WriteReg(pGEHalLocal, REG_GE_PRIM_V1_X, u16ClipR);
        }
    }
    else
    {
        u16ClipL = GE_ReadReg(pGEHalLocal, REG_GE_CLIP_L);
        if( u16ClipL > u16DstX )
        {
            GE_WriteReg(pGEHalLocal, REG_GE_PRIM_V1_X, u16ClipL);
        }
    }

    return E_GE_OK;
}

GE_Result HAL_GE_AdjustRotateDstWin( GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8Rotate )
{
    return E_GE_NOT_SUPPORT;
}


GE_Result HAL_GE_exit(GE_CTX_HAL_LOCAL *pGEHalLocal)
{
#if (__GE_WAIT_TAG_MODE == __USE_GE_INT_MODE)
    if (TRUE == MsOS_DetachInterrupt(E_INT_IRQ_GE))
    {
        bGeIrqInited = FALSE;
    }

    if (s32GeWaitTagEventHandle > 0)
    {
        if (TRUE == MsOS_DeleteEventGroup(s32GeWaitTagEventHandle))
        {
            s32GeWaitTagEventHandle = -1;
        }
    }
#endif

    return E_GE_OK;
}

GE_Result HAL_GE_SetBurstMiuLen(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL bEnable,MS_U32 u32BurstLen)
{
    MS_U16 u16Reg = 0;

    u16Reg = GE_ReadReg(pGEHalLocal, REG_GE_DBG);
    u16Reg &= ( ~GE_DBG_MIU_MAX_LEG );
    u16Reg |= ( ((u32BurstLen - 1)<<8) & GE_DBG_MIU_MAX_LEG );
    GE_WriteReg(pGEHalLocal, REG_GE_DBG, u16Reg);

    u16Reg = GE_ReadReg(pGEHalLocal, REG_GE_CFG);
    if(bEnable)
        u16Reg |= GE_CFG_LENGTH_LIMIT;
    else
        u16Reg &= (~GE_CFG_LENGTH_LIMIT);
    GE_WriteReg(pGEHalLocal, REG_GE_CFG, u16Reg);

    return E_GE_OK;
}

