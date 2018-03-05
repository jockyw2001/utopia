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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "MsTypes.h"
#include "halGOP.h"
#include "regGOP.h"
#include "halCHIP.h"
#include "drvSYS.h"

//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------
#define HAL_GOP_DEBUGINFO(x)   //x

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------
#define RIU     ((unsigned short volatile *) pGOPHalLocal->va_mmio_base)
#define GOP_WRITE2BYTE(addr, val)    { RIU[addr] = val; }
#define GOP_READ2BYTE(addr)            RIU[addr]
#define GOP_BANK_MASK                  0x1FUL
#define GOP_DST_MASK                   0xFUL

//------------------------------------------------------------------------------
//  Local Var
//------------------------------------------------------------------------------
MS_BOOL bIsMuxVaildToGopDst[MAX_GOP_MUX][MAX_DRV_GOP_DST_SUPPORT] =
{
    /*IP0,      IP0_SUB,  MIXER2VE, OP0,         VOP,   IP1,       IP1_SUB, MIXER2OP*/
    {TRUE,    FALSE, FALSE,    TRUE,    TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 0 */
    {TRUE,    FALSE, FALSE,    FALSE,   TRUE, FALSE, FALSE,  FALSE},        /*All Gop Dst case is vaild or FALSE for mux 1 */
    {TRUE,    FALSE, FALSE,    TRUE,    TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 0 */
    {TRUE,    FALSE, FALSE,    TRUE,    TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 0 */
};
#ifdef GOP_CMDQ_ENABLE
extern MS_U16 u16MIUSelect[MAX_GOP_SUPPORT];
extern MS_BOOL bMIUSelect[MAX_GOP_SUPPORT];

extern MS_U16 u16AFBCMIUSelect[MAX_GOP_SUPPORT];
extern MS_BOOL bAFBCMIUSelect[MAX_GOP_SUPPORT];
#endif
GOP_CHIP_PROPERTY g_GopChipPro =
{
    .bSetHSyncInverse =         TRUE,
    .bGop1GPalette =            TRUE,
    .bSetHPipeOfst =            FALSE,
    .bNeedCheckMVOP =           FALSE,
    .bNeedSetMUX1ToIP0 =        FALSE,
    .bNeedSetMUX3ToIP0 =        FALSE,
    .bNewMux   =                TRUE,
    .bNewPalette   =            TRUE,
    .bNewBwReg =                TRUE,
    .bGop2VStretch =            TRUE,
    .bIgnoreIPHPD  =            TRUE,  //Uranus4 has handshack with XC, should not set HPD
    .bIgnoreVEHPD  =            FALSE,  //Uranus4 to VE through Mixer, do not need adjust HPD
    .bhastilemode  =            TRUE,
    .bInitNotEnableGOPToSC =    FALSE,  //For Uranus4 mux init setting, enable GOP to SC in GOP init would cause problem
    .bAutoAdjustMirrorHSize =   TRUE,   //whether hw will auto adjust start addr when H mirror is enable
    .bGOPWithVscale =           {TRUE, TRUE, TRUE, FALSE, FALSE}, //setting GOP with/without Vscale

    .bDWINSupport   =           FALSE,
    .DwinVer =             		0x1,
    .bTstPatternAlpha =         TRUE,
    .bXCDirrectBankSupport =    TRUE,   /*XC Dirrect Bank R/W*/
    .bFRCSupport =              FALSE,  /*OC path*/
    .bGOPMixerToVE=             TRUE,  /*Mixer to VE path*/
    .bBnkForceWrite =           TRUE,   /*Direct Bank Force Write*/
    .bPixelModeSupport =        TRUE,   /*Pixel Mode Support*/
    .bScalingDownSupport=       FALSE,
    .b2Pto1PSupport=            TRUE,
    .bTLBSupport=               {TRUE, FALSE, TRUE, TRUE, FALSE, FALSE},
    .GOP_TestPattern_Vaild=     E_GOP0,
    .bInternalMIUSelect=        {TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE}, //{E_GOP0, E_GOP1, E_GOP2, E_GOP3, E_GOP4, E_GOP_Dwin, E_GOP_MIXER, E_GOP5}
    .bAFBC_Support=             {TRUE, FALSE, TRUE, FALSE, FALSE, FALSE},

#ifdef ENABLE_GOP_T3DPATCH
    .GOP_PD =                   0x30,
#else
    .GOP_PD =                   0x179,
#endif
    .GOP_UHD_PD_Offset =        0x0,
    .GOP_IP_PD =                0x00,
    .GOP_MVOP_PD =              0x69,
    .GOP_VE_PD =                0x89,
    .GOP_MIXER_PD =             0x0,
    .GOP_NonVS_PD_Offset =      0x5, //GOP without Vsacle might need add offset on pipedelay
    .GOP_VE_V_Offset =          0x0,
    .GOP_OP1_PD     =           0x1A0,

    .GOP_MUX_Delta  =           0x4,
    .GOP_Mux_Offset =           {0x0, 0x1, 0x2, 0x3, 0x3},
    .GOP_MapLayer2Mux =         {E_GOP_MUX0, E_GOP_MUX1, E_GOP_MUX2, E_GOP_MUX3, E_GOP_MUX4},
    .GOP_Mux_FRC_offset=        0x13,

    .WordUnit =                 GOP_WordUnit,
    .TotalGwinNum =             GOP_TotalGwinNum,
    .Default_ConsAlpha_bits =   DRV_VALID_8BITS,
    .enGOP3DType =              E_DRV_3D_DUP_FULL,
    .bAFBC_Merge_GOP_Trig =     TRUE,
    .bGOPVscalePipeDelay  =     {FALSE, FALSE, FALSE, FALSE, TRUE, FALSE},
    .bGOPAutoClkGating = TRUE,
};


//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
void HAL_GOP_GetGOPEnum(GOP_CTX_HAL_LOCAL *pGOPHalLocal, GOP_TYPE_DEF* GOP_TYPE)
{
    GOP_TYPE->GOP0 = E_GOP0;
    GOP_TYPE->GOP1 = E_GOP1;
    GOP_TYPE->GOP2 = E_GOP2;
    GOP_TYPE->GOP3 = E_GOP3;
    GOP_TYPE->GOP4 = E_GOP4;
    GOP_TYPE->GOP5 = E_GOP5;
    GOP_TYPE->DWIN = E_GOP_Dwin;
    GOP_TYPE->MIXER = E_GOP_MIXER;
}

GOP_Result HAL_GOP_SetWinFmt(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 regForm, MS_U8 u8GOPNum, MS_U8 u8GwinNum, MS_U16 colortype)
{
    MS_U32 u32BankOffSet = 0;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if (((regForm & E_GOP_REG_FORM_MASK) == E_GOP_REG_FORM_T21G) || ((regForm & E_GOP_REG_FORM_MASK) == E_GOP_REG_FORM_T81G))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(Gop23_GwinCtl_Ofet), colortype, 0x01f0);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(u8GwinNum), colortype, 0x01f0);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_Set_PINPON(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum, MS_BOOL bEn, E_DRV_GOP_PINPON_MODE pinpon_mode)
{
    MS_U32 u32BankOffSet =0;
    MS_U32 u32BitMask,Regval;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    switch(pinpon_mode)
    {
        default:
        case E_DRV_GOP_PINPON_G3D:
            Regval = bEn << 5;
            u32BitMask = GOP_BIT5;
            break;
        case E_DRV_GOP_PINPON_DWIN:
            Regval = bEn << 6;
            u32BitMask = GOP_BIT6;
            break;
        case E_DRV_GOP_PINPON_DIP:
            Regval = bEn << 7;
            u32BitMask = GOP_BIT7;
            HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT9,(GOP_BIT8|GOP_BIT9));
            break;
        case E_DRV_GOP_PINPON_DWIN0:
            Regval = bEn << 7;
            u32BitMask = GOP_BIT7;
            HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, 0x0,(GOP_BIT8|GOP_BIT9));
            break;
        case E_DRV_GOP_PINPON_DWIN1:
            Regval = bEn << 7;
            u32BitMask = GOP_BIT7;
            HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT8,(GOP_BIT8|GOP_BIT9));
            break;
            break;
    }

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL1, Regval, u32BitMask);
    return GOP_SUCCESS;
}


MS_BOOL _GetBnkOfstByGop(MS_U8 gop, MS_U32 *pBnkOfst)
{
    if(gop >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gop);
        return FALSE;
    }

    if (gop==E_GOP0)
        *pBnkOfst = GOP_4G_OFST<<16;
    else if (gop==E_GOP1)
        *pBnkOfst = GOP_2G_OFST<<16;
    else if (gop==E_GOP2)
        *pBnkOfst = GOP_1G_OFST<<16;
    else if (gop==E_GOP3)
        *pBnkOfst = GOP_1GX_OFST<<16;
    else if (gop==E_GOP5)
        *pBnkOfst = GOP_1GS1_OFST<<16;
    else
        return FALSE;

    return TRUE;
}

GOP_Result HAL_GOP_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask)
{
    MS_U16 u16Mask0 = 0;
    MS_U16 u16Mask1 = 0;
    if (u16GopMask&GOP_BIT0)
    {
        u16Mask0 |= GOP_BIT12;
    }
    if (u16GopMask&GOP_BIT1)
    {
        u16Mask0 |= GOP_BIT13;
    }
    if (u16GopMask&GOP_BIT2)
    {
        u16Mask0 |= GOP_BIT14;
    }
    if (u16GopMask&GOP_BIT3)
    {
        u16Mask0 |= GOP_BIT15;
    }
    if (u16GopMask&GOP_BIT4)
    {
        u16Mask1 |= GOP_BIT4;
    }
    if (u16Mask0 != 0)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, 0xFFFF , u16Mask0);
    }
    if (u16Mask1 != 0)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL_EX, 0xFFFF , u16Mask1);
    }

    if(g_GopChipPro.bAFBC_Merge_GOP_Trig ==FALSE)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_TRIGGER, GOP_BIT1, GOP_BIT1);
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    if(gop >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gop);
        return GOP_FAIL;
    }

    switch(gop)
    {
        case E_GOP0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT12 , GOP_BIT12);
            break;
        case E_GOP1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT13 , GOP_BIT13);
            break;
        case E_GOP2:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT14 , GOP_BIT14);
            break;
        case E_GOP3:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT15 , GOP_BIT15);
            break;
#if (MAX_GOP_SUPPORT>4)
        case E_GOP4:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL_EX, GOP_BIT4 , GOP_BIT4);
            break;
#endif
        default:
            break;
    }
    if(g_GopChipPro.bAFBC_Merge_GOP_Trig ==FALSE)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_TRIGGER, GOP_BIT0, GOP_BIT0);
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_TRIGGER, GOP_BIT1, GOP_BIT1);
    }
    return GOP_SUCCESS;
}

MS_U16 HAL_GOP_GetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    MS_U16 u16GopAck = 0,reg_val =0;

    if(gop >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gop);
        return FALSE;
    }

    switch(gop)
    {
        case E_GOP0:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&GOP_BIT12)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP1:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&GOP_BIT13)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP2:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&GOP_BIT14)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP3:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&GOP_BIT15)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
#if (MAX_GOP_SUPPORT>4)
        case E_GOP4:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL_EX,&reg_val);
            if(reg_val&GOP_BIT4)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
#endif
        default:
            break;
    }
    return u16GopAck;
}

GOP_Result HAL_GOP_EnableTwoLineBufferMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

void HAL_GOP_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum)
{
    MS_U32 u32bankoff = 0;
    MS_U16 mask_shift=0xFF;
    MS_U16 u16RegVal=0xFF;
    MS_U8 u8MIUSel=0xF;
    MS_U8 u8Core=0;

    if(u8GOPNum >= MAX_GOP_SUPPORT)
    {
        return;
    }

    _GetBnkOfstByGop(u8GOPNum, &u32bankoff);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_CTRL0, 0x000, GOP_BIT9);     // Genshot fast=0 for t3, for T4 and after no need to set this bit.

    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_BW, 0xDCF0, GOP_REG_WORD_MASK);  //set GOP DMA Burst length to "32"

    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT14, GOP_BIT14);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT12, GOP_BIT13|GOP_BIT12);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT8|GOP_BIT9, GOP_BIT8|GOP_BIT9);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_HW_USAGE, 0, GOP_BIT0);

    // H121 filter
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_H121, 0, GOP_BIT0); // HD alpha Enable
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_H121, 0, GOP_BIT1); // HD data Enable
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_H121, GOP_BIT2, GOP_BIT2); // SD data Enable
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_H121, GOP_BIT3, GOP_BIT3); // SD data Enable
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_H121, 0x2000, 0x3F00); // Ratio between [H121] and [Bypass mode]

    if((u8GOPNum == 0)||(u8GOPNum == 1)||(u8GOPNum == 2))
    {
        if(u8GOPNum == 0)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_SRAM_BORROW, 0, GOP_BIT13|GOP_BIT14);
        }
        else
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_SRAM_BORROW, 0, GOP_BIT14);
        }
        HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_TWO_LINEBUFFER, GOP_BIT12, GOP_BIT12);
    }

    /* enable GOP clock */
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, 0, CKG_SRAM1_DISABLE_CLK|CKG_SRAM0_DISABLE_CLK);  /* GOP palette SRAM0&1 */
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_LB_SRAMCLK, 0, BMASK(7:0));                                /* GOP Line buffer sram clock */

    switch(u8GOPNum)
    {
        case E_GOP0:
            mask_shift = GOP_MIU_CLIENT_GOP0;
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 0, CKG_GOPG0_DISABLE_CLK_MASK);  /* GOP 0 */
            break;
        case E_GOP1:
            mask_shift = GOP_MIU_CLIENT_GOP1;
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 0, CKG_GOPG1_DISABLE_CLK_MASK);  /* GOP 1 */
            break;
        case E_GOP2:
            mask_shift = GOP_MIU_CLIENT_GOP2;
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 0, CKG_GOPG2_DISABLE_CLK_MASK);  /* GOP 2 */
            break;
        case E_GOP3:
            mask_shift = GOP_MIU_CLIENT_GOP3;
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, 0, CKG_GOPG3_DISABLE_CLK_MASK);  /* GOP 3 */
            break;
        case E_GOP5:
            mask_shift = GOP_MIU_CLIENT_GOP5;
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP5CLK, 0, CKG_GOPG5_DISABLE_CLK_MASK);  /* GOP 5 */
            break;
        default:
            mask_shift = 0xFF;
            break;
    }
    if(g_GopChipPro.bAFBC_Support[u8GOPNum] ==TRUE)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_AFBCCLK, 0, CKG_AFBCCLK_DISABLE_CLK_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_FMT(0), GOP_BIT5 , GOP_BIT5);									//Half block
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_FMT(1), GOP_BIT5 , GOP_BIT5);									//Half block
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_AFBCCLK, CKG_AFBCCLK_432, GOP_BIT2|GOP_BIT3);       				//Clk(Without resoluton)
        HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_TRIGGER, GOP_BIT0, GOP_BIT0);                				//Double buffer
    }


    if(pGOPHalLocal->pGopChipPro->bInternalMIUSelect[u8GOPNum] ==TRUE)
    {
        if(u8GOPNum ==E_GOP5)
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, &u16RegVal);
        else
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal);
        u8MIUSel = (u16RegVal>>mask_shift)&0x0000001UL;
#ifdef GOP_MIU_GROUP2
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal);
        u8MIUSel = u8MIUSel|(((u16RegVal>>mask_shift)&0x0000001UL)<<1);
#endif
        HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MIU_SEL, u8MIUSel<<0, GOP_BIT0|GOP_BIT1 );//GWIN MIU Select
        HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MIU_SEL, u8MIUSel<<2, GOP_BIT2|GOP_BIT3 );//GWIN_3D MIU Select
        if(g_GopChipPro.bAFBC_Support[u8GOPNum]==TRUE)
        {
            HAL_GOP_AFBC_GetCore(pGOPHalLocal, u8GOPNum, &u8Core);
            HAL_GOP_Write16Reg(pGOPHalLocal, REG_AFBC_MIU, u8MIUSel<<4 , GOP_BIT4|GOP_BIT5);
        }

        if(u8GOPNum ==E_GOP5)
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP5_MIU_GROUP, 1<<mask_shift, 1<<mask_shift );//Get Control MIU select by GOP itself
        else
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP, 1<<mask_shift, 1<<mask_shift );//Get Control MIU select by GOP itself
    }

    //OC_RM_alpha
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCALPHA, GOP_BIT0, GOP_BIT0);
    //THEALE/utopia_release/UTPA2-205.0.x_Muji/mxlib/hal/muji/gop/
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, 0x7<< GOP_DIP_MUX_SHIFT, GOP_DIP_MUX_MASK);

    // set VIP/VOP timing select, always select VOP should be OK.
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_VIP_VOP_TIMING_SEL, GOP_BIT10, GOP_BIT10);

    // Enable OP Mux double buff
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_OP_MUX_DBF, GOP_BIT15 , GOP_BIT15);

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, GOP_BIT7|GOP_BIT15, GOP_BIT7|GOP_BIT15);//Per Pixel New alpha mode
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, GOP_BIT7|GOP_BIT15, GOP_BIT7|GOP_BIT15);//Per Pixel New alpha mode
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP,GOP_IP_SUB_MUX_MASK, GOP_IP_SUB_MUX_MASK);   //default mask sub IP
}

void HAL_GOP_Chip_Proprity_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    *pGOPHalLocal->pGopChipPro = g_GopChipPro;
}

MS_U8 HAL_GOP_GetMaxGwinNumByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GopNum)
{
    switch(u8GopNum)
    {
        case E_GOP0:
        return (MS_U8)MAX_GOP0_GWIN;
            break;
        case E_GOP1:
        return (MS_U8)MAX_GOP1_GWIN;
            break;
        case E_GOP2:
        return (MS_U8)MAX_GOP2_GWIN;
            break;
        case E_GOP3:
        return (MS_U8)MAX_GOP3_GWIN;
            break;
        case E_GOP4:
            return (MS_U8)MAX_GOP4_GWIN;
            break;
        default:
        MS_ASSERT(0);
        return 0xFF;
            break;
    }
}

MS_U8 HAL_GOP_SelGwinIdByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8Gop, MS_U8 u8Idx)
{
    MS_U8 u8GWinId = 0;

    //Adjust GWIN ID by different Chip
    if(u8Gop >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,u8Gop);
        MS_ASSERT(0);
        return 0xFF;
    }

    switch(u8Gop)
    {
        case E_GOP0:
            u8GWinId = GOP0_GwinIdBase + u8Idx;
            break;
        case E_GOP1:
            u8GWinId = GOP1_GwinIdBase + u8Idx;
            break;
        case E_GOP2:
            u8GWinId = GOP2_GwinIdBase + u8Idx;
            break;
        case E_GOP3:
            u8GWinId = GOP3_GwinIdBase + u8Idx;
            break;
#if (MAX_GOP_SUPPORT>4)
        case E_GOP4:
            u8GWinId = GOP4_GwinIdBase + u8Idx;
            break;
#endif
        default:
            break;
    }
    return u8GWinId;

}

GOP_Result HAL_GOP_GOPSel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum)
{
    if(u8GOPNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,u8GOPNum);
        MS_ASSERT(0);
        return 0xFF;
    }

    switch(u8GOPNum)
    {
    case E_GOP0: // GOP4G
            pGOPHalLocal->bank_offset = GOP_4G_OFST<<16;
            return GOP_SUCCESS;
    case E_GOP1: // GOP2G
            pGOPHalLocal->bank_offset = GOP_2G_OFST<<16;
            return GOP_SUCCESS;
    case E_GOP2: // GOP1G
            pGOPHalLocal->bank_offset = GOP_1G_OFST<<16;
            return GOP_SUCCESS;
    case E_GOP3: // GOP1GX
            pGOPHalLocal->bank_offset = GOP_1GX_OFST<<16;
            return GOP_SUCCESS;
#if (MAX_GOP_SUPPORT>4)
    case E_GOP4: // GOP1GS
            pGOPHalLocal->bank_offset = GOP_1GS0_OFST<<16;
            return GOP_SUCCESS;
#endif
    default:
        MS_ASSERT(0);
        return GOP_FAIL;
    }
}

void HAL_GOP_BANK_SEL(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8bank)
{
    MS_U16 u16Bank;
    u16Bank = GOP_READ2BYTE(GOP_BAK_SEL);
    u16Bank &= ~BMASK(3:0);
    u16Bank |= (u8bank&BMASK(3:0));
    GOP_WRITE2BYTE(GOP_BAK_SEL, u16Bank);
    u16Bank = GOP_READ2BYTE(GOP_BAK_SEL_EX);
    u16Bank &= ~GOP_BIT4;
    u16Bank |= ((u8bank&GOP_BIT4)<<GOP_BIT4);
    GOP_WRITE2BYTE(GOP_BAK_SEL_EX, u16Bank);
}

MS_U8 HAL_GOP_Get_BANK(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    MS_U16 u16GetBank=0;
#if (MAX_GOP_SUPPORT>4)
    MS_U16 u16BankMask=0;

    if(MAX_GOP_SUPPORT ==5)
    {
        u16BankMask = GOP_BANK_MASK | GOP_BIT4;
        u16GetBank = ((GOP_READ2BYTE(GOP_BAK_SEL_EX)& GOP_BIT8)>> GOP_BIT4)| (GOP_READ2BYTE(GOP_BAK_SEL)&0xF);
        return (u16GetBank&u16BankMask);
    }
    else
#endif
    {
        u16GetBank = (GOP_READ2BYTE(GOP_BAK_SEL)&0xF);
        return (u16GetBank&GOP_BANK_MASK);
    }
}

void HAL_GOP_Read16Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32addr, MS_U16* pu16ret)
{
    MS_U16 u16xcSubbank=0, u16BankAddr=0, u16BankTemp=0;
    MS_U32 bank;
    MS_U32 direct_addr;

    HAL_GOP_DEBUGINFO(printf("HAL_GOP_Read16Reg[%x]\n", u32addr));

    //* Gop driver should access another HW IP register
    //* ex: SC's IP and OP setting, GE's det frame buffer setting, ChipTop GOP clk setting
    switch (u32addr & 0xFF00)
    {
        case GOP_REG_BASE:
        {
            bank = (u32addr & 0xFF0000) >> 8;
#if 0 //for GOP4
            if(bank==0xE00)//GOP4:  0x121B00
            {
                    bank=GOP_REG_GOP4_BK_OFFSET;
            }
            else if(bank==0xF00)//GWIN4: 0x121E00
            {
                bank=GOP_REG_GOP4_GW_OFFSET;
            }
            else if(bank==0x1000)//GOP4_ST
            {
                bank=GOP_REG_GOP4_ST_OFFSET;
            }
#endif
            direct_addr = GOP_REG_DIRECT_BASE + bank + (u32addr & 0xFF);  //Direct_Base + bank + addr_offset
            *pu16ret = GOP_READ2BYTE((direct_addr&0xFFFFF));
            break;
        }
        case SC1_REG_BASE:
        {
            if(g_GopChipPro.bXCDirrectBankSupport)
            {
                u16xcSubbank =  (u32addr & 0xFF0000)>>8;
                u32addr = SC1_DIRREG_BASE+ u16xcSubbank + (u32addr & 0xFF);
                *pu16ret = GOP_READ2BYTE((u32addr&0xFFFFF));
            }
            else
            {
                u16xcSubbank =  (u32addr & 0xFF0000)>>16;
                u16BankAddr = GOP_SC_BANKSEL+0;
                u32addr = SC1_REG_BASE + (u32addr & 0xFF);

                u16BankTemp = GOP_READ2BYTE(u16BankAddr&0xFFFF);
                GOP_WRITE2BYTE(u16BankAddr&0xFFFF, u16xcSubbank);
                *pu16ret = GOP_READ2BYTE((u32addr&0xFFFF));
                GOP_WRITE2BYTE(u16BankAddr&0xFFFF, u16BankTemp);
            }
            break;
        }
        case GE_REG_BASE:
        case CKG_REG_BASE:
        case MIU0_REG_BASE:
        case MIU_REG_BASE:
        case MVOP_REG_BASE:
        case VE_REG_BASE:
        {
            *pu16ret = GOP_READ2BYTE((u32addr&0xFFFF));
            break;
        }
#ifdef GOP_MIU_GROUP2
        case (MIU2_REG_BASE & 0xFF00):
        {
            direct_addr = MIU2_REG_BASE + (u32addr & 0xFF);  //Direct_Base + addr_offset
            *pu16ret = GOP_READ2BYTE((direct_addr&0xFFFFF));

            break;
        }
#endif
        default:
        {
            //Gop lib current do not support this HW ip base
            MS_ASSERT(0);
            *pu16ret =0;
            break;
        }
    }
}

void HAL_GOP_Write16Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32addr, MS_U16 u16val, MS_U16 mask)
{
    MS_U16 u16tmp=0;
    MS_U16 u16xcSubbank=0,u16BankAddr=0,pu16ret=0;
    MS_U32 bank;
    MS_U32 direct_addr;

    HAL_GOP_DEBUGINFO(printf("HAL_GOP_Write16Reg[%04x] = %04x\n", u32addr, u16val));

    if(mask!=0xffff)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, u32addr, &u16tmp);
        u16tmp &= ~mask;
        u16val &=  mask;
        u16val |=  u16tmp;
    }

    //* Gop driver should access another HW IP register
    //* ex: SC's IP and OP setting, GE's det frame buffer setting, ChipTop GOP clk setting
    switch (u32addr & 0xFF00)
    {
        case GOP_REG_BASE:
        {
            bank = (u32addr & 0xFF0000) >> 8;
#if 0 //for GOP4
            if(bank==0xE00)//GOP4:  0x121B00
            {
                    bank=GOP_REG_GOP4_BK_OFFSET;
            }
            else if(bank==0xF00)//GWIN4: 0x121E00
            {
                    bank=GOP_REG_GOP4_GW_OFFSET;
            }
            else if(bank==0x1000) //GOP4_ST
            {
                    bank=GOP_REG_GOP4_ST_OFFSET;
            }
#endif
            direct_addr = GOP_REG_DIRECT_BASE + bank + (u32addr & 0xFF);
            GOP_WRITE2BYTE((direct_addr&0xFFFFF), u16val);
            break;
        }
        case SC1_REG_BASE:
            if(g_GopChipPro.bXCDirrectBankSupport)
            {   /*Derrick Bank*/
                u16xcSubbank =  (u32addr & 0xFF0000)>>8 ;
                direct_addr = SC1_DIRREG_BASE + u16xcSubbank+ (u32addr & 0xFF);
                GOP_WRITE2BYTE((direct_addr&0xFFFFF), u16val);
            }
            else
            {   /*Sub Bank*/
                u16xcSubbank =  (u32addr & 0xFF0000)>>16 ;
                u16BankAddr = GOP_SC_BANKSEL+0;
                u32addr = SC1_REG_BASE + (u32addr & 0xFF);

                pu16ret = GOP_READ2BYTE(u16BankAddr&0xFFFF);
                GOP_WRITE2BYTE((u16BankAddr&0xFFFF), u16xcSubbank);
                GOP_WRITE2BYTE((u32addr&0xFFFF), u16val);
                GOP_WRITE2BYTE((u16BankAddr&0xFFFF), pu16ret);
            }
            break;
        case GE_REG_BASE:
        case CKG_REG_BASE:
        case MIU0_REG_BASE:
        case MIU_REG_BASE:
        case VE_REG_BASE:
        {
            GOP_WRITE2BYTE((u32addr&0xFFFF), u16val);
            break;
        }
#ifdef GOP_MIU_GROUP2
        case (MIU2_REG_BASE & 0xFF00):
        {
            direct_addr = MIU2_REG_BASE + (u32addr & 0xFF);  //Direct_Base + addr_offset
            GOP_WRITE2BYTE((direct_addr&0xFFFFF), u16val);
            break;
        }
#endif
        default:
        {
            //Gop lib current do not support this HW ip base
            MS_ASSERT(0);
            break;
        }

    }
}


void HAL_GOP_Write32Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32addr, MS_U32 u32val)
{
    MS_U32 bank;
    MS_U32 direct_addr;

    HAL_GOP_DEBUGINFO(printf("HAL_GOP_Write32Reg[%bx] = %lx\n", u32addr, u32val));

    //* Gop driver should access another HW IP register
    //* ex: SC's IP and OP setting, GE's det frame buffer setting, ChipTop GOP clk setting
    switch (u32addr & 0xFF00)
    {
        case GOP_REG_BASE:
        {
            bank = (u32addr & 0xFF0000) >> 8;
#if 0 //for GOP4
            if(bank==0xE00)//GOP4:  0x121B00
            {
                    bank=GOP_REG_GOP4_BK_OFFSET;
            }
            else if(bank==0xF00)//GWIN4: 0x121E00
            {
                    bank=GOP_REG_GOP4_GW_OFFSET;
            }
            else if(bank==0x1000) //GOP4_ST
            {
                    bank=GOP_REG_GOP4_ST_OFFSET;
            }
#endif
            direct_addr = GOP_REG_DIRECT_BASE + bank + (u32addr & 0xFF);
            GOP_WRITE2BYTE((direct_addr&0xFFFFF), (u32val&0xFFFF));
            GOP_WRITE2BYTE((direct_addr&0xFFFFF)+2, (u32val&0xFFFF0000)>>16);
            break;
        }

        case GE_REG_BASE:
        case SC1_REG_BASE:
        case CKG_REG_BASE:
        {
            GOP_WRITE2BYTE((u32addr&0xFFFF), (u32val&0xFFFF));
            GOP_WRITE2BYTE((u32addr&0xFFFF)+2, (u32val&0xFFFF0000)>>16);
            break;
        }

        default:
        {
            //Gop lib current do not support this HW ip base
            MS_ASSERT(0);
            break;
        }

    }
}

//extern E_BDMA_Ret MDrv_BDMA_Mem_Fill(MS_U32 u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, E_BDMA_DstDev eDev);

void HAL_GOP_Write32Pal(GOP_CTX_HAL_LOCAL *pGOPHalLocal,
                                           MS_U8 *pREGMAP_Base, MS_U16 *pREGMAP_Offset, MS_U32 u32REGMAP_Len,
                                           MS_U8 u8Index, MS_U8 u8A, MS_U8 u8R, MS_U8 u8G, MS_U8 u8B)
{
	MS_U8 i=0;
    HAL_GOP_DEBUGINFO(printf("GOP_Write32Pal : i= %02bx, ARGB = %02bx,%02bx,%02bx,%02bx\n",u8Index, u8A, u8R, u8G, u8B));

    /* Don't care high byte */
    MS_ASSERT((MS_U32)(*pREGMAP_Offset +GOP_WordUnit)<= u32REGMAP_Len);

	for(i =(GOP_WordUnit-1);i>4;i--)
    {
		*(pREGMAP_Base + *pREGMAP_Offset + i) = 0;
    }
    *(pREGMAP_Base + *pREGMAP_Offset + 4) = u8Index;
    *(pREGMAP_Base + *pREGMAP_Offset + 3) = u8A;
    *(pREGMAP_Base + *pREGMAP_Offset + 2) = u8R;
    *(pREGMAP_Base + *pREGMAP_Offset + 1) = u8G;
    *(pREGMAP_Base + *pREGMAP_Offset) = u8B;
    *pREGMAP_Offset += GOP_WordUnit;

    MsOS_FlushMemory(); //make sure cpu write data to dram

}

MS_U16 HAL_GOP_GetBPP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPColorType fbFmt)
{
    MS_U16 bpp=0;

    switch ( fbFmt )
    {
    case E_DRV_GOP_COLOR_RGB555_BLINK :
    case E_DRV_GOP_COLOR_RGB565 :
    case E_DRV_GOP_COLOR_ARGB1555:
    case E_DRV_GOP_COLOR_RGBA5551:
    case E_DRV_GOP_COLOR_ARGB4444 :
    case E_DRV_GOP_COLOR_RGBA4444 :
    case E_DRV_GOP_COLOR_RGB555YUV422:
    case E_DRV_GOP_COLOR_YUV422:
    case E_DRV_GOP_COLOR_2266:
        bpp = 16;
        break;
    case E_DRV_GOP_COLOR_ARGB8888 :
    case E_DRV_GOP_COLOR_ABGR8888 :
        bpp = 32;
        break;

    case E_DRV_GOP_COLOR_I8 :
        bpp = 8;
        break;

    default :
        //print err
        MS_ASSERT(0);
        bpp = 0xFFFF;
        break;
    }
    return bpp;

}

void HAL_GOP_GWIN_SetBlending(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
    MS_U16 regval;
    /*For compatibility
        Old chip only have alpha coeffient 6 bits. Charka2 define UI alpha value base on it.*/
    MS_U8 u8coef_cpt = u8coef;

    //if UI alpha value based on alpha coeffient 6 bits, and our chip is 8bit, please do it.
    if( pGOPHalLocal->User_ConsAlpha_bits != g_GopChipPro.Default_ConsAlpha_bits)
    {
        switch(u8coef)
        {
            case 0x0 :
                u8coef_cpt = u8coef<<2;
                break;
            case 0x3f :
                u8coef_cpt = ((u8coef<<2)|0x3);
                break;
            default:
                u8coef_cpt = ((u8coef<<2)|0x1);
                break;
        }
    }

	/*alpha coeffient 6/8bit chip has GOP0,GOP1*/
    if (u8win<(MAX_GOP0_GWIN+MAX_GOP1_GWIN))
    {
        regval = (MS_U16)(bEnable?(1<<14):0)|(MS_U16)((u8coef&0x3F)<<8);
        HAL_GOP_Write16Reg(pGOPHalLocal, (u8win < MAX_GOP0_GWIN)? GOP_4G_GWIN0_CTRL(u8win):GOP_2G_GWIN_CTRL(u8win-MAX_GOP0_GWIN), regval, 0x4000);
        HAL_GOP_Write16Reg(pGOPHalLocal, (u8win < MAX_GOP0_GWIN)? GOP_4G_GWIN_ALPHA01(u8win):GOP_2G_GWIN_ALPHA01(u8win-MAX_GOP0_GWIN), u8coef_cpt, 0xFF);
    }
    /*Only alpha coeffient 8bit chip has GOP2,GOP3...*/
    else
    {
        if (u8win==(MAX_GOP0_GWIN+MAX_GOP1_GWIN))
        {
            regval = (MS_U16)(bEnable?(1<<14):0);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN0_CTRL, regval, 0x4000);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN_ALPHA01, u8coef, 0xFF);
        }
        else if ((u8win==(MAX_GOP0_GWIN+MAX_GOP1_GWIN+MAX_GOP2_GWIN)))
        {
            regval = (MS_U16)(bEnable?(1<<14):0);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_GWIN0_CTRL, regval, 0x4000);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_GWIN_ALPHA01, u8coef, 0xFF);
        }
        else
        {
            regval = (MS_U16)(bEnable?(1<<14):0);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_GWIN0_CTRL, regval, 0x4000);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_GWIN_ALPHA01, u8coef, 0xFF);
        }
    }
}

void HAL_GOP_SetIOMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr)
{
    pGOPHalLocal->va_mmio_base = addr;
}
void HAL_GOP_SetIOFRCMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr)
{
}
void HAL_GOP_SetIOPMMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr)
{
}


GOP_Result HAL_GOP_GWIN_SetDstPlane(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType,MS_BOOL bOnlyCheck)
{
    /*A5 GOP dst type:
        1: IP (Main)
        0: IP (Sub)
        2: OP
        3: MVOP
        4: MVOP (Sub)
        6: FRC
        11: InsideFRC
    */
    MS_U16 u16RegVal;
    MS_U32 u32BankOffSet;
    MS_BOOL bEnable=FALSE;
    MS_U16 u16HSyncMask=GOP_BIT14;

    if( GopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,GopNum);
        return GOP_INVALID_PARAMETERS;
    }

    switch (eDstType)
    {
        case E_DRV_GOP_DST_IP0:
            if( (GopNum == E_GOP0) || (GopNum == E_GOP1) )
            {
                u16RegVal = 0x0;
                bEnable = FALSE;
                u16HSyncMask=0;
            }else{
                return GOP_FUN_NOT_SUPPORTED;
            }
            break;

        case E_DRV_GOP_DST_IP0_SUB:
            if( (GopNum == E_GOP0) || (GopNum == E_GOP1) )
            {
                u16RegVal = 0x1;
                bEnable = FALSE;
                u16HSyncMask=0;
            }else{
                return GOP_FUN_NOT_SUPPORTED;
            }
            break;

        case E_DRV_GOP_DST_OP0:
            u16RegVal = 0x2;
            bEnable = FALSE;
            u16HSyncMask=GOP_BIT14;
            break;

        case E_DRV_GOP_DST_IP1:
            u16RegVal = 0x5;
            bEnable = TRUE;
            break;

        case E_DRV_GOP_DST_OP1:
            u16RegVal = 0x6;
            bEnable = TRUE;
            u16HSyncMask=GOP_BIT14;
            break;

        case E_DRV_GOP_DST_DIP:
            u16RegVal = 0x8;
            bEnable = TRUE;
            u16HSyncMask=GOP_BIT14;
            break;

        default:
            return GOP_FUN_NOT_SUPPORTED;
    }

    if(bOnlyCheck == FALSE)
    {
        _GetBnkOfstByGop(GopNum, &u32BankOffSet);
        HAL_GOP_SetGOP2Pto1P(pGOPHalLocal, GopNum, bEnable);
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL1, u16RegVal, BMASK(3:0));

        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL0, u16HSyncMask, GOP_BIT14);               // Set mask Hsync when VFDE is low
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType eDstType)
{
    /*U4 Mixer dst type:
        2: OP
        3: VE
      */
    MS_U16 u16Regval = 0;
	DRV_MixerDstType  eType=E_DRV_MIXER_DST_VE;

	switch(eDstType)
	{
		case E_DRV_GOP_DST_MIXER2VE:
			eType = E_DRV_MIXER_DST_VE;
			HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT7, GOP_BIT7);     	//Enable pseudo hsync
			break;
		case E_DRV_GOP_DST_MIXER2OP:
			eType = E_DRV_MIXER_DST_OP;
			HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, 0x0, GOP_BIT7);     		//Disable pseudo hsync
			break;
		default:
			break;
	}

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIXER_CTRL, &u16Regval);

    u16Regval &= (~(GOP_BIT3|GOP_BIT4));
    u16Regval |= (eType<<0x3);

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, u16Regval, (GOP_BIT3|GOP_BIT4));
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType *pGopDst)
{
    MS_U16 u16Regval = 0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIXER_CTRL, &u16Regval);

    if(((u16Regval&0x18)>>3) == 0x2)
        *pGopDst = E_DRV_GOP_DST_MIXER2OP;
    else
        *pGopDst = E_DRV_GOP_DST_MIXER2VE;

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_InitMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    /*T8
    OP path: support 3 mux (mux0, mux 2, mux3) to blend with SC simultaneously
    IP  path: support mux0 and mux1 to IPMain/IPSub. Only one mux of mux0 and mux1 can be blended to IPMain/IPSub
    SW default setting=> mux0:gop1g, mux1:gop1gx, mux2:gop2g, mux3:gop4g
    */
    MS_U8 gop4g=0, gop2g=1, gop1g=2, gop1gx=3, gop1gs=4;
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, ((gop1gs<<(GOP_MUX_SHIFT*4))|(gop4g<<(GOP_MUX_SHIFT*3))|(gop2g<<(GOP_MUX_SHIFT*2))|(gop1gx<<(GOP_MUX_SHIFT*1))|gop1g), GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL_EX, GOP_BIT15 , GOP_BIT15); // trigger

    /*Disable VE OSD enable*/
    HAL_GOP_VE_SetOSDEnable(pGOPHalLocal, FALSE, EN_OSD_0, 0x4);
    HAL_GOP_VE_SetOSDEnable(pGOPHalLocal, FALSE, EN_OSD_1, 0x4);

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_VOPNBL, 1<<7, GOP_BIT7);
    return GOP_SUCCESS;
}

void HAL_GOP_GWIN_GetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8* u8GOPNum, Gop_MuxSel eGopMux)
{
    MS_U16 u16GopMux=0;
    switch(eGopMux)
    {
        case E_GOP_MUX0:
        case E_GOP_MUX1:
        case E_GOP_MUX2:
        case E_GOP_MUX3:
        case E_GOP_MUX4:
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &u16GopMux);
            *u8GOPNum = (u16GopMux >> (eGopMux*GOP_MUX_SHIFT))& GOP_REGMUX_MASK;
            break;
        case E_GOP_IP0_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_IP_MAIN_MUX_MASK)>>GOP_IP_MAIN_MUX_SHIFT;
            break;
        case E_GOP_IP0_SUB_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_IP_SUB_MUX_MASK)>>GOP_IP_SUB_MUX_SHIFT;
            break;
        case E_GOP_VOP0_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_IP_VOP0_MUX_MASK)>>GOP_IP_VOP0_MUX_SHIFT;
            break;
        case E_GOP_VOP0_SUB_MUX:
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_IP_VOP1_MUX_MASK)>>GOP_IP_VOP1_MUX_SHIFT;
            break;
        case E_GOP_Mix_MUX0:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_Mix_MUX0_MASK)>>GOP_Mix_MUX0_SHIFT;
            break;
        case E_GOP_Mix_MUX1:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_Mix_MUX1_MASK)>>GOP_Mix_MUX1_SHIFT;
            break;
        case E_GOP_VE0_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_VE0_MUX_MASK)>>GOP_VE0_MUX_SHIFT;
            break;
        case E_GOP_VE1_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_VE1_MUX_MASK)>>GOP_VE1_MUX_SHIFT;
            break;
        case E_GOP_DIP_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_4K2K, &u16GopMux);
            *u8GOPNum = (u16GopMux&GOP_DIP_MUX_MASK)>>GOP_DIP_MUX_SHIFT;
            break;
        case E_GOP_FRC_MUX0:
        case E_GOP_FRC_MUX1:
        case E_GOP_FRC_MUX2:
        case E_GOP_FRC_MUX3:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_4K2K, &u16GopMux);
            *u8GOPNum = (u16GopMux >> ((eGopMux%E_GOP_FRC_MUX0)*GOP_FRC_MUX_SHIFT))& GOP_FRC_REGMUX_MASK;
            break;
        case E_GOP_BYPASS_MUX0:
            break;
        case E_GOP_OP1_MUX:
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_SC1, &u16GopMux);
            *u8GOPNum = (u16GopMux& ((BMASK((GOP_MUX_SHIFT-1):0))<<12))>>12;
            break;
        case E_GOP_IP1_MUX:
        case E_GOP_VOP1_MUX:
        case E_GOP_GS_MUX:
        case E_GOP_DUALRATE_OP_MUX0:
        case E_GOP_DUALRATE_OP_MUX1:
        case E_GOP_DUALRATE_OP_MUX2:
            // to prevent warning log
            break;
        default:
            printf("[%s]ERROR, not support the mux[%d]\n",__FUNCTION__,eGopMux);
            break;
        }
}

void HAL_GOP_GWIN_SetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8 u8GOPNum, Gop_MuxSel eGopMux)
{
    MS_U16 u16Ret[4]={0};

    switch(eGopMux)
    {
        case E_GOP_MUX0:
        case E_GOP_MUX1:
        case E_GOP_MUX2:
        case E_GOP_MUX3:
        case E_GOP_MUX4:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, u8GOPNum <<(GOP_MUX_SHIFT*eGopMux), GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*eGopMux));
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL_EX, GOP_BIT15 , GOP_BIT15); // trigger
          break;
        case E_GOP_OP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_SC1, u8GOPNum <<12, GOP_REGMUX_MASK<<12);
          break;
        case E_GOP_IP0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_MAIN_MUX_SHIFT, GOP_IP_MAIN_MUX_MASK);
            break;
        case E_GOP_IP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum << 0, BMASK(2:0));
            break;
        case E_GOP_VOP0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_VOP0_MUX_SHIFT, GOP_IP_VOP0_MUX_MASK);
            break;
        case E_GOP_VOP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_VOP1_MUX_SHIFT, GOP_IP_VOP1_MUX_MASK);
            break;
        case E_GOP_Mix_MUX0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, u8GOPNum << GOP_Mix_MUX0_SHIFT, GOP_Mix_MUX0_MASK);
            break;
        case E_GOP_Mix_MUX1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, u8GOPNum << GOP_Mix_MUX1_SHIFT, GOP_Mix_MUX1_MASK);
            break;
        case E_GOP_VE0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, u8GOPNum << GOP_VE0_MUX_SHIFT, GOP_VE0_MUX_MASK);
            break;
        case E_GOP_VE1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, u8GOPNum << GOP_VE1_MUX_SHIFT, GOP_VE1_MUX_MASK);
            break;
        case E_GOP_DIP_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum << GOP_DIP_MUX_SHIFT, GOP_DIP_MUX_MASK);
            break;
        case E_GOP_FRC_MUX0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)));
            break;
        case E_GOP_FRC_MUX1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)));
            break;
        case E_GOP_FRC_MUX2:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)));
            break;
        case E_GOP_FRC_MUX3:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(eGopMux%E_GOP_FRC_MUX0)));
            break;
        case E_GOP_BYPASS_MUX0:
                    //Check MUX is using? Checking from priority high to low
                    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER2_L_EN, &u16Ret[0]);
                    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER2_R_EN, &u16Ret[1]);
                    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER1_L_EN, &u16Ret[2]);
                    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER1_R_EN, &u16Ret[3]);

                    //Checking if un-use
                    if((u16Ret[0] & GOP_BIT0) ==0)
                    {
                        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX2%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX2%E_GOP_FRC_MUX0)));
                    }
                    else if((u16Ret[1] & GOP_BIT0) ==0)
                    {
                        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX3%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX3%E_GOP_FRC_MUX0)));
                    }
                    else if((u16Ret[2] & GOP_BIT0) ==0)
                    {
                        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX0%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX0%E_GOP_FRC_MUX0)));
                    }
                    else if((u16Ret[3] & GOP_BIT0) ==0)
                    {
                        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, u8GOPNum <<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX1%E_GOP_FRC_MUX0)) , GOP_REGMUX_MASK<<(GOP_FRC_MUX_SHIFT*(E_GOP_FRC_MUX1%E_GOP_FRC_MUX0)));
                    }
                    else
                    {
                        printf("[%s][%d]FRC mux is already full.\n",__FUNCTION__,__LINE__);
                    }
                    break;
             break;
        default:
            printf("[%s]ERROR mux setting\n",__FUNCTION__);
            break;
    }
}

GOP_Result HAL_GOP_SetGOPEnable2SC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    /* GOP OP Path enable to SC Setting
        A5: GOP OP Path blending with SC sequence
        mux1-->mux0-->mux2-->mux3
    */
    MS_BOOL bOSDBEnable=TRUE;
    MS_U16 muxValue=0, mux4Value=0, regval=0;

    DRV_GOPDstType pGopDst = E_DRV_GOP_DST_INVALID;
    MS_U16 u164K2KMuxValue=0;

    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum);
        return GOP_FAIL;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    if(bOSDBEnable)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, 0x0, GOP_REG_HW_MASK);

    }else
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPEN, &regval);
    }

    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        if(bOSDBEnable)
        {
            if(bEnable)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 0<<1, GOP_BIT1|GOP_BIT2);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, GOP_BIT3, GOP_BIT3);
            }else{
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 0, GOP_BIT3);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 0<<1, GOP_BIT1|GOP_BIT2);
            }
        }
        else
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT15):(regval & ~GOP_BIT15), GOP_BIT15);
    }

    if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        if(bOSDBEnable)
        {
            if(bEnable)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 1<<9, GOP_BIT9|GOP_BIT10);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, GOP_BIT11, GOP_BIT11);
            }else{
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 0, GOP_BIT11);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, 1<<9, GOP_BIT9|GOP_BIT10);
            }
        }
        else
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval | GOP_BIT12):(regval & ~GOP_BIT12), GOP_BIT12);
    }

    if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        if(bOSDBEnable)
        {
            if(bEnable)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 2<<1, GOP_BIT1|GOP_BIT2);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, GOP_BIT3, GOP_BIT3);
            }else{
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 0, GOP_BIT3);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 2<<1, GOP_BIT1|GOP_BIT2);
            }
        }
        else
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT14):(regval & ~GOP_BIT14), GOP_BIT14);
    }

    if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        if(bOSDBEnable)
        {
            if(bEnable)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 3<<9, GOP_BIT9|GOP_BIT10);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, GOP_BIT11, GOP_BIT11);
            }else{
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 0, GOP_BIT11);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, 3<<9, GOP_BIT9|GOP_BIT10);
            }
        }
        else
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT13):(regval & ~GOP_BIT13), GOP_BIT13);
    }


    if(MAX_GOP_SUPPORT ==5)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX4_MIX_VE, &mux4Value);
        if (gopNum== ((mux4Value & GOP_MUX4_MASK)>> GOP_MUX4_SHIFT)) //enable mux4
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT11):(regval & ~GOP_BIT11), GOP_BIT11);
        }
    }

    /*For FRC mux switch*/
    HAL_GOP_GetGOPDst(pGOPHalLocal, gopNum, &pGopDst);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_4K2K, &u164K2KMuxValue);
    if (gopNum== (u164K2KMuxValue & GOP_MUX0_MASK))
    {
        /* Switch FRC blending */
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER1_L_EN, (pGopDst==E_DRV_GOP_DST_BYPASS)?GOP_BIT0: ~GOP_BIT0, GOP_BIT0);
    }

    if (gopNum== ((u164K2KMuxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1)))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER1_R_EN, (pGopDst==E_DRV_GOP_DST_BYPASS)?GOP_BIT0: ~GOP_BIT0, GOP_BIT0);
    }

    if (gopNum== ((u164K2KMuxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2)))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER2_L_EN, (pGopDst==E_DRV_GOP_DST_BYPASS)?GOP_BIT0: ~GOP_BIT0, GOP_BIT0);
    }

    if (gopNum== ((u164K2KMuxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3)))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_FRC_LAYER2_R_EN, (pGopDst==E_DRV_GOP_DST_BYPASS)?GOP_BIT0: ~GOP_BIT0, GOP_BIT0);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOP2Pto1P(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    MS_U32 u32pBankOffSet=0;
    MS_U16 u16Regval;

    if(g_GopChipPro.b2Pto1PSupport)
    {
        _GetBnkOfstByGop(gopNum, &u32pBankOffSet);
        HAL_GOP_Read16Reg(pGOPHalLocal, u32pBankOffSet + GOP_4G_SRAM_BORROW, &u16Regval);
        HAL_GOP_Write16Reg(pGOPHalLocal, u32pBankOffSet + GOP_4G_SRAM_BORROW, bEnable?(u16Regval |GOP_BIT11):(u16Regval & ~GOP_BIT11), GOP_BIT11);
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPEnable2Mode1(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    /* GOP OP Path enable to SC Setting
        A5: GOP OP Path blending with SC sequence
        mux1-->mux0-->mux2-->mux3
    */
    MS_U16 muxValue=0, regval=0;

    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum );
        return FALSE;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, &regval);
    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x10):(regval & ~0x10), 0x30);
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        //printf("");
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x1000):(regval & ~0x1000), 0x3000);
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x40):(regval & ~0x40), 0xC0);
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x100):(regval & ~0x100), 0x300);
    }
    else if (gopNum== ((muxValue & GOP_MUX4_MASK)>> (GOP_MUX_SHIFT*4))) //enable mux4
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x4000):(regval & ~0x4000), 0xc000);
    }
    else
    {
        return GOP_FAIL;
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GetGOPAlphaMode1(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL *pbEnable)
{
    MS_U16 muxValue=0;
    MS_U16 regval37=0;
    MS_U16 regval38=0;

    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum );
        return FALSE;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_L, &regval37);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_VOP2BLENDING_H, &regval38);
    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        *pbEnable = (regval37& GOP_BIT5) == GOP_BIT5;
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        *pbEnable = (regval37 & GOP_BIT13) == GOP_BIT13;
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        *pbEnable = (regval38 & GOP_BIT5) == GOP_BIT5;
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        *pbEnable = (regval38 & GOP_BIT13) == GOP_BIT13;
    }
    else
    {
        return GOP_FAIL;
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPHighPri(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum)
{
    /*T8
    OP path: support 4 mux (mux0, mux1, mux 2, mux3) to blend with SC simultaneously
    IP  path: support mux0 and mux1 to IPMain/IPSub. Only one mux of mux0 and mux1 can be blended to IPMain/IPSub
    */
    MS_U16 Mux3Gop, muxValue=0, i;
    MS_U16 MuxShift;

    MuxShift = GOP_MUX_SHIFT * E_GOP_MUX3;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    for (i=0; i<MAX_GOP_MUX;i++) //T8 4 mux
    {
        if (gopNum== ((muxValue&(GOP_REGMUX_MASK<<(i*GOP_MUX_SHIFT)))>>(i*GOP_MUX_SHIFT)))
        {
            Mux3Gop = (muxValue&GOP_MUX3_MASK)>> MuxShift; //save mux2 gop

            muxValue &= ~GOP_MUX3_MASK; //clear mux2 setting
            muxValue &= ~(GOP_REGMUX_MASK<<(i*GOP_MUX_SHIFT));  //clear current mux setting
            muxValue |= ((gopNum<< MuxShift)|(Mux3Gop<<(i*GOP_MUX_SHIFT)));
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, muxValue, GOP_REG_WORD_MASK);
             HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL_EX, GOP_BIT15 , GOP_BIT15); // trigger
            break;

        }

    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPClk(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, DRV_GOPDstType eDstType)
{
    /*T8 GOP dst type:
    0: IP (Main)
    1: IP (Sub)
    2: OP
    3: MVOP
    */

    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum);
        MS_ASSERT(0);
        return GOP_INVALID_PARAMETERS;
    }
    /* Monaco clkgen setting */
    /******************
    0: odclk
    1: idclk_f2  (ipm)
    2: idclk_f1 (ips)
    3: ocmixer
    4: ve
    5 : mvop main
    6 : mvop sub
    ******************/

    switch(eDstType)
    {
        case E_DRV_GOP_DST_IP0:
            if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 6<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 6<<8, CKG_GOPG1_MASK);
            }
            else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

        case E_DRV_GOP_DST_IP0_SUB:
            if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 6<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 6<<8, CKG_GOPG1_MASK);
            }
            else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

        case E_DRV_GOP_DST_OP0:
            if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 0<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 0<<8, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 0<<2, CKG_GOPG2_MASK);
            }else if (gopNum==E_GOP3)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, 0<<8, CKG_GOPG3_MASK);
            }else if (gopNum==E_GOP5)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP5CLK, 0<<8, CKG_GOPG5_MASK);
            }else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

	case E_DRV_GOP_DST_IP1:
	     if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 7<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 7<<8, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 7<<2, CKG_GOPG2_MASK);
            }else if (gopNum==E_GOP3)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, 7<<8, CKG_GOPG3_MASK);
            }else if (gopNum==E_GOP5)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP5CLK, 7<<8, CKG_GOPG5_MASK);
            }else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

        case E_DRV_GOP_DST_OP1:
	     if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 2<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 2<<8, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 2<<2, CKG_GOPG2_MASK);
            }else if (gopNum==E_GOP3)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, 2<<8, CKG_GOPG3_MASK);
            }else if (gopNum==E_GOP5)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP5CLK, 2<<8, CKG_GOPG5_MASK);
            }else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

        case E_DRV_GOP_DST_DIP:
	     if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 3<<2, CKG_GOPG0_MASK);
            }
            else if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, 3<<8, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 3<<2, CKG_GOPG2_MASK);
            }else if (gopNum==E_GOP3)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, 3<<8, CKG_GOPG3_MASK);
            }else if (gopNum==E_GOP5)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP5CLK, 3<<8, CKG_GOPG5_MASK);
            }else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
        break;

        default:
            MS_ASSERT(0);
            return GOP_ENUM_NOT_SUPPORTED;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOPCLK, &pGOPHalLocal->u16Clk0Setting); //Backup current GOPG clock settings
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SRAMCLK,&pGOPHalLocal->u16Clk2Setting); //Backup current SRAM clock settings

    return GOP_SUCCESS;
}


GOP_Result HAL_GOP_SetClkForCapture(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_DWIN_SRC_SEL enSrcSel)
{
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, 0, GOP_BIT8);
    if (enSrcSel==GOP_DRV_DWIN_SRC_OP)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPDCLK, CKG_GOPD_CLK_ODCLK, CKG_GOPD_MASK);
    else if (enSrcSel==GOP_DRV_DWIN_SRC_MVOP)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPDCLK, CKG_GOPD_CLK_DC0CLK, CKG_GOPD_MASK);
    else if (enSrcSel==GOP_DRV_DWIN_SRC_IP)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPDCLK, CKG_GOPD_CLK_IDCLK2, CKG_GOPD_MASK);
    else
        return GOP_INVALID_PARAMETERS;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOP2CLK, &pGOPHalLocal->u16Clk1Setting); //Backup current GOPD clock settings
    return GOP_SUCCESS;
}
GOP_Result HAL_GOP_SetClock(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_BOOL bEnable)
{
    if (bEnable)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, pGOPHalLocal->u16Clk0Setting, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, pGOPHalLocal->u16Clk1Setting, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, pGOPHalLocal->u16Clk2Setting, GOP_REG_WORD_MASK);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG0_DISABLE_CLK, CKG_GOPG0_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_DISABLE_CLK, CKG_GOPG1_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_DISABLE_CLK, CKG_GOPG2_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPDCLK, CKG_GOPD_DISABLE_CLK, CKG_GOPD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, CKG_SRAM0_DISABLE_CLK, CKG_SRAM0_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, CKG_SRAM1_DISABLE_CLK, CKG_SRAM1_MASK);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_MIXER_SetGOPEnable2Mixer(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
       DRV_GOPDstType MixerDstType;
        MS_U16 regval = 0;
        MS_U8 u8MUXGOPNum = 0;
        int i = 0;

        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIXER_CTRL, &regval);
#if 0     //TO DO: To disable mixer mux
        MS_U16 u16val;
        if( regval & 0x0001) //MIXER GOP0 enable -> enable GOP1
        {
            u16val = (bEnable? TRUE: FALSE);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (u16val<<1), GOP_BIT1);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<12), (GOP_BIT12|GOP_BIT13));
        }
        else //enable GOP0
        {
            u16val = (bEnable? TRUE: FALSE);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, u16val, GOP_BIT0);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<10), (GOP_BIT10|GOP_BIT11));
        }
#else
        if((regval&(GOP_BIT0|GOP_BIT1)) == (GOP_BIT0|GOP_BIT1))
        {
            if((gopNum != ((regval&(GOP_BIT10|GOP_BIT11))>>10)) && (gopNum != ((regval&(GOP_BIT12|GOP_BIT13))>>12)))
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<10), (GOP_BIT10|GOP_BIT11));
            }
        }
        else if(regval&GOP_BIT0)
        {
            if(gopNum != ((regval&(GOP_BIT10|GOP_BIT11))>>10))
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT1, GOP_BIT1);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<12), (GOP_BIT12|GOP_BIT13));
            }
        }
        else if(regval&GOP_BIT1)  //gop1Enable
        {
            if(gopNum != ((regval&(GOP_BIT12|GOP_BIT13))>>12))
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT1, GOP_BIT1);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<12), (GOP_BIT12|GOP_BIT13));
            }
        }
        else //no one enable
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<10), (GOP_BIT10|GOP_BIT11));
        }
#endif

        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT7, GOP_BIT7);     //disable pseudo hsync

        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, 0x0, GOP_BIT2);     //alpha mode: 0 bypass mode: 1

        HAL_GOP_GetMixerDst(pGOPHalLocal,&MixerDstType);
        if(MixerDstType == E_DRV_GOP_DST_MIXER2OP)
        {
            for (i=0; i<MAX_GOP_MUX - 1; i++) //mux3 not supported to OP
            {
                HAL_GOP_GWIN_GetMUX(pGOPHalLocal, &u8MUXGOPNum, (Gop_MuxSel)i);
                if (0x4==u8MUXGOPNum)
                {
                    HAL_GOP_EnableSCNewAlphaMode(pGOPHalLocal, (Gop_MuxSel)i, TRUE);
                }
                else
                {
                    HAL_GOP_EnableSCNewAlphaMode(pGOPHalLocal, (Gop_MuxSel)i, FALSE);
                }
            }
        }
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_VE, GOP_MIXER_EN_VFIL, GOP_MIXER_EN_VFIL_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_VE, GOP_MIXER_VS_FLD_ON, GOP_MIXER_VS_FLD_ON_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_REG_DUMMY, GOP_BIT0, GOP_BIT0);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT15, GOP_BIT15);   //Enable Mixer

        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHST, GOP_VE_PAL_HSTART_OFST, GOP_REG_WORD_MASK);
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVST, GOP_VE_PAL_VSTART_OFST, GOP_REG_WORD_MASK);


        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_HS_PIPE, pGOPHalLocal->pGopChipPro->GOP_MIXER_PD, GOP_REG_WORD_MASK);

#if 0    /* PAL System*/
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT0, GOP_BIT0);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT2, GOP_BIT2);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHST, GOP_VE_HSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVST, GOP_VE_VSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHEND, 0x330, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVEND, 0x25D, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HTT, GOP_VE_PAL_HTOTAL-1, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HS_DELAY, GOP_VE_HS_DELAY, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_HS_PIPE, GOP_MIXER_PD, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT15, GOP_BIT15);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
#endif
        return GOP_SUCCESS;

}

GOP_Result HAL_GOP_MIXER_SetMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_U8 muxNum, MS_BOOL bEnable)
{
    MS_U16 u16val;

    if (muxNum >= MAX_MIXER_MUX)
    {
        MS_CRITICAL_MSG(printf("\n[%s] not support mux%d in this chip version!!",__FUNCTION__, muxNum));
        return GOP_FAIL;
    }

    if(muxNum==MIXER_MUX0Id) //Set GOP to mux0
    {
        u16val = (bEnable? TRUE: FALSE);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, u16val, GOP_BIT0);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<10), (GOP_BIT10|GOP_BIT11));
    }
    else //Set GOP to mux1
    {
        u16val = (bEnable? TRUE: FALSE);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (u16val<<1), GOP_BIT1);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, (gopNum<<12), (GOP_BIT12|GOP_BIT13));
    }
    return GOP_SUCCESS;

}

GOP_Result HAL_GOP_MIXER_EnableOldBlendMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEn)
{
    MS_U32 u32bankoffset = 0;
    DRV_GOPDstType gopDstType = E_DRV_GOP_DST_IP0;
    MS_U16 regval = 0x0;

    HAL_GOP_GetGOPDst(pGOPHalLocal, u8GOP, &gopDstType);

    if(gopDstType == E_DRV_GOP_DST_MIXER2VE)
    {

        _GetBnkOfstByGop(u8GOP, &u32bankoffset);

        if(bEn)
        {

            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT2, GOP_BIT2);   //alpha mode: 0 bypass mode: 1
            HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoffset + GOP_4G_CTRL0, (MS_U16)~GOP_BIT15, GOP_BIT15);

            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIXER_CTRL, &regval);

            if(u8GOP == ((regval&(GOP_BIT10|GOP_BIT11))>>10))
            {
                MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK_3B, GOP_VE_TVS_OSD_EN, (~BIT(2))|BIT(1), BIT(1)|BIT(2));
            }
            else if(u8GOP == ((regval&(GOP_BIT12|GOP_BIT13))>>12))
            {
                MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK_3B, GOP_VE_TVS_OSD1_EN, (~BIT(2))|BIT(1), BIT(1)|BIT(2));
            }
            else
            {
                printf("[%s][%d]GOP not exisit in VE mux\n",__FUNCTION__,__LINE__);
                return GOP_FAIL;
            }
        }
        else
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, 0x0, GOP_BIT2);   //alpha mode: 0 bypass mode: 1
            HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoffset + GOP_4G_CTRL0, (MS_U16)GOP_BIT15, GOP_BIT15);

            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIXER_CTRL, &regval);

            if(u8GOP == ((regval&(GOP_BIT10|GOP_BIT11))>>10))
            {
                MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK_3B, GOP_VE_TVS_OSD_EN, (~BIT(1))|BIT(2), BIT(1)|BIT(2));
            }
            else if(u8GOP == ((regval&(GOP_BIT12|GOP_BIT13))>>12))
            {
                MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK_3B, GOP_VE_TVS_OSD1_EN, (~BIT(1))|BIT(2), BIT(1)|BIT(2));
            }
            else
            {
                printf("[%s][%d]GOP not exisit in VE mux\n",__FUNCTION__,__LINE__);
                return GOP_FAIL;
            }
        }
    }
    else
    {
        printf("[%s][%d]GOP not on Mixer2VE path\n",__FUNCTION__,__LINE__);
        return GOP_FAIL;
    }

    return GOP_SUCCESS;

}

GOP_Result HAL_GOP_EnableSCNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, Gop_MuxSel muxNum, MS_BOOL bEnable)
{
    MS_U16 bankTemp = 0;
    MS_U16 u16val = 0;

    u16val = (bEnable? TRUE: FALSE);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_BANKSEL, &bankTemp);
    switch (muxNum) //Enable OP new alpha mode
    {
        case 0:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x2F, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BLEND0_GOP_SWITCH, (u16val<<1), GOP_BIT1);
                break;
        case 1:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x10, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_ALPHAMODE, (u16val<<6), GOP_BIT6);
                break;
        case 2:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x10, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_ALPHAMODE, (u16val<<8), GOP_BIT8);
                break;
        default:
                printf("\n [%s],This chip is not support this MUX value!\n",__FUNCTION__);
                break;
    }
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, bankTemp, GOP_REG_WORD_MASK);
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_EnableSCPerPixelNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, Gop_MuxSel muxNum, MS_BOOL bEnable)
{
    MS_U16 bankTemp = 0;
    MS_U16 u16val = 0;

    u16val = (bEnable? TRUE: FALSE);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_BANKSEL, &bankTemp);
    switch (muxNum) //Enable OP per pixel new alpha mode
    {
        case 0:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x2F, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BLEND0_GOP_SWITCH, (u16val<<3), GOP_BIT3);
                break;
        case 1:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x10, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_ALPHAMODE, (u16val<<14), GOP_BIT14);
                break;
        case 2:
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, 0x10, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_ALPHAMODE, (u16val<<15), GOP_BIT15);
                break;
        default:
                printf("\n [%s],This chip is not support this MUX value!\n",__FUNCTION__);
                break;
    }
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, bankTemp, GOP_REG_WORD_MASK);
    return GOP_SUCCESS;
}

void  HAL_GOP_Init_Context(GOP_CTX_HAL_LOCAL *pGOPHalLocal, GOP_CTX_HAL_SHARED *pHALShared, MS_BOOL bNeedInitShared)
{
    MS_U32 u32GopIdx;

    memset(pGOPHalLocal, 0, sizeof(*pGOPHalLocal));
    pGOPHalLocal->pHALShared = pHALShared;

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        pGOPHalLocal->drvGFlipGOPDst[u32GopIdx] = E_DRV_GOP_DST_OP0;
    }
    pGOPHalLocal->pGopChipPro = &pHALShared->gopChipProperty;
    pGOPHalLocal->pbIsMuxVaildToGopDst = (MS_BOOL *)bIsMuxVaildToGopDst;
}
void  HAL_GOP_Restore_Ctx(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
}

GOP_Result HAL_ConvertAPIAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gwinid, MS_PHY* u64Adr)
{
        MS_U8 u8Miu=0xff;
        MS_U16 u16RegVal_L=0;
#ifdef GOP_MIU_GROUP2
        MS_U16 u16RegVal_H=0;
#endif

        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal_L);
#ifdef GOP_MIU_GROUP2
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal_H);
#endif
        if (gwinid<MAX_GOP0_GWIN)   //gop0
        {
            u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP0);
#ifdef GOP_MIU_GROUP2
            u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP0);
            u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP0) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP0)<<1;
#else
            u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP0);
#endif
        }
        else if (gwinid>=MAX_GOP0_GWIN && gwinid<MAX_GOP0_GWIN+MAX_GOP1_GWIN)   //gop1
        {
            u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP1);
#ifdef GOP_MIU_GROUP2
            u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP1);
            u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP1) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP1)<<1;
#else
            u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP1);
#endif
        }
        else if (gwinid==GOP2_GwinIdBase)  //gop2
        {
             u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP2);
#ifdef GOP_MIU_GROUP2
             u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP2);
             u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP2) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP2)<<1;
#else
             u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP2);
#endif
        }
        else if (gwinid==GOP3_GwinIdBase)  //gop3
        {
             u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP3);
#ifdef GOP_MIU_GROUP2
             u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP3);
             u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP3) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP3)<<1;
#else
             u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP3);
#endif
        }
        else if (gwinid==GOP5_GwinIdBase)   //gop5
        {
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, &u16RegVal_L);
            u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP5);
            u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP5);
        }
         else
        {
            return GOP_FAIL;
        }

        if (u8Miu <= 2)
        {
            _miu_offset_to_phy(u8Miu, *u64Adr, *u64Adr);
        }
        else
        {
            printf("[%s] ERROR GOP miu client\n",__FUNCTION__);
            return GOP_FAIL;
        }
        return GOP_SUCCESS;
}

MS_U8 HAL_GOP_GetMIUDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopnum)
{
    MS_U8 u8Miu=0xff;
    MS_U16 u16RegVal_L=0;
    MS_U32 u32BnkOfst=0xFFFF;
#ifdef GOP_MIU_GROUP2
    MS_U16 u16RegVal_H=0;
#endif

    if(pGOPHalLocal->pGopChipPro->bInternalMIUSelect[gopnum]==TRUE)
    {
        _GetBnkOfstByGop(gopnum, &u32BnkOfst);
        HAL_GOP_Read16Reg(pGOPHalLocal, u32BnkOfst+GOP_4G_MIU_SEL, &u16RegVal_L);

        u8Miu= (u16RegVal_L& (GOP_BIT2|GOP_BIT3)) >>2;
    }
    else
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal_L);
#ifdef GOP_MIU_GROUP2
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal_H);
#endif
        switch (gopnum)
        {
            case 0:
                    u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP0);
#ifdef GOP_MIU_GROUP2
                    u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP0);
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP0) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP0)<<1;
#else
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP0);
#endif
                    break;
            case 1:
                    u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP1);
#ifdef GOP_MIU_GROUP2
                    u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP1);
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP1) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP1)<<1;
#else
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP1);
#endif
                    break;

            case 2:
                    u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP2);
#ifdef GOP_MIU_GROUP2
                    u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP2);
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP2) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP2)<<1;
#else
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP2);
#endif
                    break;

            case 3:
                    u16RegVal_L &= BIT(GOP_MIU_CLIENT_GOP3);
#ifdef GOP_MIU_GROUP2
                    u16RegVal_H &= BIT(GOP_MIU_CLIENT_GOP3);
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP3) | (u16RegVal_H>>GOP_MIU_CLIENT_GOP3)<<1;
#else
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP3);
#endif
                    break;
            case 5:
                    HAL_GOP_Read16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, &u16RegVal_L);
                    u8Miu = (u16RegVal_L>>GOP_MIU_CLIENT_GOP5);

            default:
                    return 0xff;
                    break;
        }

        if(u8Miu > MAX_GOP_MIUSEL )
        {
            printf("[%s] ERROR GOP miu client\n",__FUNCTION__);
            return 0xff;
        }

    }
    return u8Miu;
}
GOP_Result HAL_GOP_GetGOPDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8gopNum, DRV_GOPDstType *pGopDst)
{
    MS_U16 u16Regval;
    MS_U32 u32pBankOffSet=0;
    GOP_Result ret;

    if(u8gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,u8gopNum);
        *pGopDst = E_DRV_GOP_DST_INVALID;
        return GOP_FAIL;
    }

    _GetBnkOfstByGop(u8gopNum, &u32pBankOffSet);
    HAL_GOP_Read16Reg(pGOPHalLocal, u32pBankOffSet + GOP_4G_CTRL1, &u16Regval);

    switch (u16Regval&GOP_DST_MASK)
    {
        case 0:
            *pGopDst = E_DRV_GOP_DST_IP0;
            ret = GOP_SUCCESS;
            break;
        case 1:
            *pGopDst = E_DRV_GOP_DST_IP0_SUB;
            ret = GOP_SUCCESS;
            break;
        case 2:
            *pGopDst = E_DRV_GOP_DST_OP0;
            ret = GOP_SUCCESS;
            break;
        case 5:
            *pGopDst = E_DRV_GOP_DST_IP1;
            ret = GOP_SUCCESS;
            break;
        case 6:
            *pGopDst = E_DRV_GOP_DST_OP1;
            ret = GOP_SUCCESS;
            break;
        case 8:
            *pGopDst = E_DRV_GOP_DST_DIP;
            ret = GOP_SUCCESS;
            break;
        default:
            *pGopDst = E_DRV_GOP_DST_INVALID;
            ret = GOP_FAIL;
            break;
    }

    return ret;

}

void HAL_GOP_SetIPSel2SC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_IPSEL_GOP ipSelGop)
{
    MS_U16 muxValue=0;
    MS_U16 u16RegVal= 0, u16RegMsk = 0;
    MS_BOOL bSC1_OP=0,bSC1_IP=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &muxValue);

    switch(ipSelGop)
    {
    case MS_DRV_IP0_SEL_GOP0:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        if(E_GOP0 == (muxValue &GOP_MUX0_MASK))
            u16RegVal |= GOP_BIT12;//mux0
        else
            u16RegVal |= GOP_BIT13;//mux1
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;

    case MS_DRV_IP0_SEL_GOP1:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        if(E_GOP1 == (muxValue &GOP_MUX0_MASK))
            u16RegVal |= GOP_BIT12;//mux0
        else
            u16RegVal |= GOP_BIT13;//mux1
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;
    case MS_DRV_IP0_SEL_GOP2:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        if(E_GOP2 == (muxValue &GOP_MUX0_MASK))
            u16RegVal |= GOP_BIT12;//mux0
        else
            u16RegVal |= GOP_BIT13;//mux1
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;

    case MS_DRV_NIP_SEL_GOP0:
        if(E_GOP0 == (muxValue &GOP_MUX0_MASK))
        {
                u16RegVal = ~GOP_BIT12;//mux0
                u16RegMsk = GOP_BIT12;
        }
            else
        {
                u16RegVal = ~GOP_BIT13;//mux1
                u16RegMsk = GOP_BIT13;
        }
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;

    case MS_DRV_NIP_SEL_GOP1:
        if(E_GOP1 == (muxValue &GOP_MUX0_MASK))
        {
                u16RegVal = ~GOP_BIT12;//mux0
                u16RegMsk = GOP_BIT12;
        }
        else
        {
                u16RegVal = ~GOP_BIT13;//mux1
                u16RegMsk = GOP_BIT13;
        }
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;
    case MS_DRV_NIP_SEL_GOP2:
        if(E_GOP2 == (muxValue &GOP_MUX0_MASK))
        {
                u16RegVal = ~GOP_BIT12;//mux0
                u16RegMsk = GOP_BIT12;
        }
            else
        {
                u16RegVal = ~GOP_BIT13;//mux1
                u16RegMsk = GOP_BIT13;
        }
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;
    case MS_DRV_MVOP_SEL:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        u16RegVal |= GOP_BIT12; //mux0
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        bSC1_OP =FALSE;
        bSC1_IP =FALSE;
        break;
    case MS_DRV_SC1OP_SEL:
        bSC1_OP =TRUE;
        bSC1_IP =FALSE;
        break;
    case MS_DRV_IP1_SEL:
        bSC1_OP =FALSE;
        bSC1_IP =TRUE;
        break;
    default:
        printf("[%s] ERROR invalid source select\n",__FUNCTION__);
        break;
    }
    if(0 != u16RegMsk)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, u16RegVal, u16RegMsk);
    }

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_GOPEN, bSC1_OP<<14, GOP_BIT14);
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_CHANNELSYNC, !bSC1_IP<<11, GOP_BIT11);
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_CHANNELSYNC, bSC1_IP<<12, GOP_BIT12);

}

GOP_Result HAL_GOP_DWIN_SetSourceSel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_DWIN_SRC_SEL enSrcSel)
{
    return GOP_FUN_NOT_SUPPORTED;
}


MS_U8 HAL_GOP_GetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    return GOP_FUN_NOT_SUPPORTED;
}
GOP_Result HAL_GOP_SetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 miu)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_DWIN_EnableR2YCSC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_VE_SetOutputTiming(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32mode)
{
    if(u32mode == E_GOP_PAL)
    {
        /* PAL System*/
        pGOPHalLocal->pGopChipPro->GOP_VE_PD = 0x73;
        pGOPHalLocal->pGopChipPro->GOP_VE_V_Offset = 0x13;
        HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_BOT_HS, ~GOP_BIT2, GOP_BIT2);
    }
    else if(u32mode == E_GOP_NTSC)
    {
        /* NTSC System*/
        pGOPHalLocal->pGopChipPro->GOP_VE_PD = 0x65;
        pGOPHalLocal->pGopChipPro->GOP_VE_V_Offset = 0xF;
        HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_BOT_HS, GOP_BIT2, GOP_BIT2);
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_MIXER_SetOutputTiming(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32mode, GOP_DRV_MixerTiming *pTM)
{

    if(u32mode == E_GOP_PAL)
    {
        /* PAL System*/
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT0, GOP_BIT0);
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT2, GOP_BIT2);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHST, GOP_VE_PAL_HSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVST, GOP_VE_PAL_VSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHEND, GOP_VE_PAL_WIDTH, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVEND, GOP_VE_PAL_HEIGHT, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HTT, GOP_VE_PAL_HTOTAL-1, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HS_DELAY, GOP_VE_PAL_HS_DELAY, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_HS_PIPE, pGOPHalLocal->pGopChipPro->GOP_MIXER_PD, GOP_REG_WORD_MASK);

        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT15, GOP_BIT15);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
    }
    else if(u32mode == E_GOP_NTSC)
    {
        /* NTSC System*/
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT0, GOP_BIT0);
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT2, GOP_BIT2);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHST, GOP_VE_NTSC_HSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVST, GOP_VE_NTSC_VSTART_OFST, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHEND, GOP_VE_NTSC_WIDTH, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVEND, GOP_VE_NTSC_HEIGHT, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HTT, GOP_VE_NTSC_HTOTAL, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HS_DELAY, GOP_VE_NTSC_HS_DELAY, GOP_REG_WORD_MASK);

        HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_HS_PIPE, pGOPHalLocal->pGopChipPro->GOP_MIXER_PD, GOP_REG_WORD_MASK);

        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT15, GOP_BIT15);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
    }
    else if(u32mode == E_GOP_CUSTOM)
    {
        return GOP_FUN_NOT_SUPPORTED;
        /*
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT0, GOP_BIT0);
        //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_VE_ENABLE_OSD, GOP_BIT2, GOP_BIT2);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHST, pTM->hstart, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVST, pTM->vstart, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FHEND, pTM->hend, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_FVEND, pTM->vend, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HTT, pTM->htotal, GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_HS_DELAY, pTM->hsyncdelay, GOP_REG_WORD_MASK);

        HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_HS_PIPE, pGOPHalLocal->pGopChipPro->GOP_MIXER_PD, GOP_REG_WORD_MASK);

        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT7, GOP_BIT7);     //enable pseudo hsync
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT15, GOP_BIT15);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_CTRL, GOP_BIT0, GOP_BIT0);
        */
    }
    else if(u32mode == E_GOP_CUSTOM_OP)
    {
    return GOP_FUN_NOT_SUPPORTED;
}
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_MIXER_EnableVfilter(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEn)
{
	HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIXER_VE, bEn, GOP_MIXER_EN_VFIL_MASK);
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GWIN_EnableTileMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable, E_GOP_TILE_DATA_TYPE tilemode)
{
       //fix me need to modify tile_32bpp mode in 64b_bus
	if(E_DRV_GOP_TILE_DATA_16BPP == tilemode)
	{
	    HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_GWIN0_CTRL(u8win), \
        bEnable?GOP_BIT15:~GOP_BIT15, GOP_BIT15);
           HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_DRAM_FADE(u8win), \
        ~GOP_BIT7, GOP_BIT7);

	}
	else if(E_DRV_GOP_TILE_DATA_32BPP == tilemode)
	{
	    HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_GWIN0_CTRL(u8win), \
        bEnable?GOP_BIT15:~GOP_BIT15, GOP_BIT15);
	    HAL_GOP_Write16Reg(pGOPHalLocal, HAL_GOP_BankOffset(pGOPHalLocal)+GOP_4G_DRAM_FADE(u8win), \
        ~GOP_BIT7, GOP_BIT7);
	}
	else
	{
     return GOP_FUN_NOT_SUPPORTED;
}

	return GOP_SUCCESS;

}

GOP_Result HAL_GOP_SetUVSwap(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum,MS_BOOL bEn)
{
    MS_U32 u32BankOffSet =0;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if (bEn)
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_YUV_SWAP, GOP_BIT14, GOP_BIT14);
    else
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_YUV_SWAP, ~GOP_BIT14, GOP_BIT14);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetYCSwap(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum,MS_BOOL bEn)
{
    MS_U32 u32BankOffSet =0;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    if (bEn)
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_YUV_SWAP, GOP_BIT15, GOP_BIT15);
    else
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_YUV_SWAP, 0x0, GOP_BIT15);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GWIN_GetNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL* pEnable)
{
    MS_U16 u16Val =0x0;
    if (u8win < GOP1_GwinIdBase)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_4G_GWIN_ALPHA01(u8win), &u16Val);
        *pEnable = (MS_BOOL)(u16Val>>15);
    }
    else if (u8win < GOP2_GwinIdBase)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_2G_GWIN_ALPHA01(u8win - MAX_GOP0_GWIN), &u16Val);
        *pEnable = (MS_BOOL)(u16Val>>15);
    }
    else if (u8win < GOP3_GwinIdBase)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_1G_GWIN_ALPHA01, &u16Val);
        *pEnable = (MS_BOOL)(u16Val>>15);
    }
    else if (u8win < GOP4_GwinIdBase)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_1GX_GWIN_ALPHA01, &u16Val);
        *pEnable = (MS_BOOL)(u16Val>>15);
    }
    else if (u8win < GOP5_GwinIdBase)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_1GS0_GWIN_ALPHA01, &u16Val);
        *pEnable = (MS_BOOL)(u16Val>>15);
    }
    else
    {
        printf("%s Not support this GWIN num%d!!!\n",__FUNCTION__, u8win);
        return GOP_INVALID_PARAMETERS;
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GWIN_SetNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable)
{
    MS_U16 u16Val = bEnable << 15;
    if (u8win < GOP1_GwinIdBase)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_GWIN_ALPHA01(u8win), u16Val, GOP_BIT15);
    }
    else if (u8win < GOP2_GwinIdBase)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_GWIN_ALPHA01(u8win - MAX_GOP0_GWIN), u16Val, GOP_BIT15);
    }
    else if (u8win < GOP3_GwinIdBase)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN_ALPHA01, u16Val, GOP_BIT15);
    }
    else if (u8win < GOP4_GwinIdBase)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_GWIN_ALPHA01, u16Val, GOP_BIT15);
    }
    else if (u8win < GOP5_GwinIdBase)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_GWIN_ALPHA01, u16Val, GOP_BIT15);
    }
    else
    {
        printf("%s Not support this GWIN num%d!!!\n",__FUNCTION__, u8win);
        return GOP_INVALID_PARAMETERS;
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_GWiN_Set3DOSD_Sub(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP ,MS_U8 u8Gwin, MS_PHY u32SubAddr)
{
    MS_U16 u16Reg = 0;
    MS_U32 u32WordBase = 0;
    MS_U32 u32BankOffSet=0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Read16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_BANK_FWR, &u16Reg);

    if(u16Reg & GOP_BIT7)
    {
        u32WordBase = 1;
    }
    else
    {
        u32WordBase = GOP_WordUnit;
    }

    u32SubAddr /= u32WordBase;

    if(u8GOP == E_GOP2)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_3DOSD_SUB_RBLK_L, u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_3DOSD_SUB_RBLK_H, u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else if(u8GOP == E_GOP3)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_3DOSD_SUB_RBLK_L, u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_3DOSD_SUB_RBLK_H, u32SubAddr>>16 ,GOP_REG_WORD_MASK );

    }
    else if(u8GOP == E_GOP4)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_3DOSD_SUB_RBLK_L, u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_3DOSD_SUB_RBLK_H, u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else if(u8GOP == E_GOP1)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_3DOSD_SUB_RBLK_L(u8Gwin - MAX_GOP0_GWIN), u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_3DOSD_SUB_RBLK_H(u8Gwin - MAX_GOP0_GWIN), u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else  //gop0
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_3DOSD_SUB_RBLK_L(u8Gwin), u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_3DOSD_SUB_RBLK_H(u8Gwin), u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }

    return GOP_SUCCESS;

}

//------------------------------------------------------------------------------
/// Set VE output with OSD
/// @return none
//------------------------------------------------------------------------------
GOP_Result HAL_GOP_VE_SetOSDEnable(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEnable, EN_VE_OSD_ENABLE eOSD, MS_U8 gopNum)
{
    if(eOSD == EN_OSD_0)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_GOPEN, bEnable<<14, GOP_BIT14);

    if(eOSD == EN_OSD_1)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC1_GOPEN, bEnable<<15, GOP_BIT15);
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPToVE(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEn )
{
/*
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(gopNum, &u32BankOffSet);

    //Direct to SC1 OP
    HAL_GOP_SetIPSel2SC(pGOPHalLocal, MS_DRV_SC1OP_SEL);
    HAL_GOP_SetGOPClk(pGOPHalLocal, gopNum, E_DRV_GOP_DST_OP1);
    HAL_GOP_GWIN_SetMUX(pGOPHalLocal,gopNum,E_GOP_OP1_MUX);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL0, (MS_U16)~GOP_BIT1, GOP_BIT1);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_CTRL0, (MS_U16)GOP_BIT15, GOP_BIT15);
    */
    return GOP_SUCCESS;
}

E_GOP_VIDEOTIMING_MIRRORTYPE HAL_GOP_GetVideoTimingMirrorType(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bHorizontal)
{
    E_GOP_VIDEOTIMING_MIRRORTYPE enMirrorType = E_GOP_VIDEOTIMING_MIRROR_BYSCALER;
    MS_U16 u16MVOPMirrorCfg = 0;
    MS_U16 u16ScalerMirrorCfg = 0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MVOP_MIRRORCFG, &u16MVOPMirrorCfg);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIRRORCFG, &u16ScalerMirrorCfg);
    if(bHorizontal) // Horizontal
    {
        if(u16MVOPMirrorCfg & GOP_BIT1)
        {
            enMirrorType = E_GOP_VIDEOTIMING_MIRROR_BYMVOP;
        }
        else if(u16ScalerMirrorCfg & GOP_BIT12)
        {
            enMirrorType = E_GOP_VIDEOTIMING_MIRROR_BYSCALER;
        }
    }
    else //vertical
    {
        if(u16MVOPMirrorCfg & GOP_BIT0)
        {
            enMirrorType = E_GOP_VIDEOTIMING_MIRROR_BYMVOP;
        }
        else if(u16ScalerMirrorCfg & GOP_BIT13)
        {
            enMirrorType = E_GOP_VIDEOTIMING_MIRROR_BYSCALER;
        }
    }
    return enMirrorType;
}

GOP_Result HAL_GOP_3D_SetMiddle(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP,MS_U16 u16Middle)
{
   MS_U32 u32BankOffSet =0;

   _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
   HAL_GOP_Write16Reg(pGOPHalLocal,u32BankOffSet+GOP_4G_3D_MIDDLE, u16Middle, GOP_REG_WORD_MASK);
   return GOP_SUCCESS;
}

GOP_Result HAL_GOP_OC_SetOCEn(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bOCEn)
{
    MS_U8  i=0, eGopMux=0, FRCMuxOffset=0;
    MS_U16 u16Val=0;
    MS_U16 u16Mux[MAX_GOP_MUX];

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &u16Val);
    u16Mux[0] =( u16Val & GOP_MUX0_MASK);
    u16Mux[1] =( u16Val & GOP_MUX1_MASK)    >> (GOP_MUX_SHIFT*1);
    u16Mux[2] =( u16Val & GOP_MUX2_MASK)    >> (GOP_MUX_SHIFT*2);
    u16Mux[3] =( u16Val & GOP_MUX3_MASK)    >> (GOP_MUX_SHIFT*3);
#if (MAX_GOP_MUX>4)
    u16Mux[4] =( u16Val & GOP_MUX4_MASK)    >> (GOP_MUX_SHIFT*4);
#endif
    switch(u8GOP)
    {
        case E_GOP0:
        case E_GOP1:
        case E_GOP2:
        case E_GOP3:
#if (MAX_GOP_MUX>4)
        case E_GOP4:
#endif
                for(i=0; i<MAX_GOP_MUX; i++)
                {
                    if(u8GOP == u16Mux[i])
                    {
                        eGopMux = i;
#if (MAX_GOP_MUX>4)
                        if(eGopMux == 4)
                        {
                            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, bOCEn<<15, GOP_BIT15);
                        }
                        else
#endif
                        {
                            FRCMuxOffset = 12 + eGopMux;
                            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, bOCEn<<FRCMuxOffset, 1<<FRCMuxOffset);
                        }
                    }
                }
                break;
        default:
                return GOP_FAIL;
                break;
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_OC_SetOCInfo(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_OC_INFO* pOCinfo)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_OC_Get_MIU_Sel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 *MIUId)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_DWIN_SetRingBuffer(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32RingSize,MS_U32 u32BufSize)
{
	return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_AdjustField(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType)
{
    MS_U32 u32BankOffSet = 0;
    MS_BOOL bInverse = 0xFF;
    _GetBnkOfstByGop(GopNum, &u32BankOffSet);

    switch (eDstType)
    {
        case E_DRV_GOP_DST_IP0:
        case E_DRV_GOP_DST_IP1:
        case E_DRV_GOP_DST_OP1:
            bInverse = TRUE;
            break;
        default:
            bInverse = FALSE;
            break;
    }
    if(bInverse == TRUE)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_CTRL0, 1<<4, 0x10);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_CTRL0, 0<<4, 0x10);
    }
    return GOP_SUCCESS;
}

/********************************************************************************/
///Test Pattern
/********************************************************************************/
GOP_Result HAL_GOP_TestPattern_IsVaild(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GopNum)
{
    if(u8GopNum == pGOPHalLocal->pGopChipPro->GOP_TestPattern_Vaild)
    {
            return GOP_SUCCESS;
    }
    else
    {
            return GOP_FAIL;
    }

}


/********************************************************************************/
///GOP Scaling down (internal)
/********************************************************************************/

MS_BOOL HAL_GOP_EnableScalingDownSram(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP,MS_BOOL bEn)
{
   return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_HScalingDown(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable,MS_U16 src, MS_U16 dst)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_VScalingDown(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable,MS_U16 src, MS_U16 dst)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_DeleteWinHVSize(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize)
{
    MS_U32 u32BankOffSet=0;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_BANK_HVAILDSIZE, u16HSize, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_BANK_VVAILDSIZE, u16VSize, GOP_REG_WORD_MASK);
    return GOP_SUCCESS;
}

GOP_Result  HAL_GOP_DumpGOPReg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16* u16Val)
{
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);

    if (u32GopIdx < MAX_GOP_SUPPORT)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, (u32BankOffSet+ (u16BankIdx<<16) + u16Addr +GOP_4G_CTRL0), u16Val);
    }
    else
    {
        printf("[%s][%d] Data is zero!!!\n",__FUNCTION__,__LINE__);
        *u16Val = 0;
    }
    return GOP_SUCCESS;
}

GOP_Result  HAL_GOP_RestoreGOPReg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16 u16Val)
{
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);

    if (u32GopIdx < MAX_GOP_SUPPORT)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, (u32BankOffSet+ (u16BankIdx<<16) + u16Addr +GOP_4G_CTRL0), u16Val, GOP_REG_WORD_MASK);
    }
    else
    {
        printf("[%s][%d] Data is zero!!!\n",__FUNCTION__,__LINE__);
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_PowerState(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32PowerState, GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate)
{
    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
        {
            //CLK
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOPCLK, &(pGOP_STRPrivate->CKG_GopReg[0]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOP2CLK, &(pGOP_STRPrivate->CKG_GopReg[1]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOP3CLK, &(pGOP_STRPrivate->CKG_GopReg[2]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOP4CLK, &(pGOP_STRPrivate->CKG_GopReg[3]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SRAMCLK, &(pGOP_STRPrivate->CKG_GopReg[4]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_LB_SRAMCLK, &(pGOP_STRPrivate->CKG_GopReg[5]));

            //XC
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_BANKSEL, &(pGOP_STRPrivate->XC_GopReg[0]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, &(pGOP_STRPrivate->XC_GopReg[1]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPEN, &(pGOP_STRPrivate->XC_GopReg[2]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_IP_SYNC, &(pGOP_STRPrivate->XC_GopReg[3]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_IP2GOP_SRCSEL, &(pGOP_STRPrivate->XC_GopReg[4]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_OSD_CHECK_ALPHA, &(pGOP_STRPrivate->XC_GopReg[5]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_VOPNBL, &(pGOP_STRPrivate->XC_GopReg[6]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, &(pGOP_STRPrivate->XC_GopReg[7]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIRRORCFG, &(pGOP_STRPrivate->XC_GopReg[8]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_OCMIXER, &(pGOP_STRPrivate->XC_GopReg[9]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_OCMISC, &(pGOP_STRPrivate->XC_GopReg[10]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_OCALPHA, &(pGOP_STRPrivate->XC_GopReg[11]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, &(pGOP_STRPrivate->XC_GopReg[12]));
        }
            break;
        case E_POWER_RESUME:
        {
            //CLK
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, pGOP_STRPrivate->CKG_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, pGOP_STRPrivate->CKG_GopReg[1], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, pGOP_STRPrivate->CKG_GopReg[2], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP4CLK, pGOP_STRPrivate->CKG_GopReg[3], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, pGOP_STRPrivate->CKG_GopReg[4], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_LB_SRAMCLK, pGOP_STRPrivate->CKG_GopReg[5], GOP_REG_WORD_MASK);

            //XC
            //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, pGOP_STRPrivate->XC_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, pGOP_STRPrivate->XC_GopReg[1], GOP_BIT11);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, pGOP_STRPrivate->XC_GopReg[2], GOP_REG_WORD_MASK);
            //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_IP_SYNC, pGOP_STRPrivate->XC_GopReg[3], GOP_REG_WORD_MASK);
            //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_IP2GOP_SRCSEL, pGOP_STRPrivate->XC_GopReg[4], GOP_BIT15);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OSD_CHECK_ALPHA, pGOP_STRPrivate->XC_GopReg[5], GOP_BIT6);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOPNBL, pGOP_STRPrivate->XC_GopReg[6], GOP_BIT5);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, pGOP_STRPrivate->XC_GopReg[7], GOP_REG_WORD_MASK);
            //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_MIRRORCFG, pGOP_STRPrivate->XC_GopReg[8], GOP_REG_WORD_MASK);
            //HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCMIXER, pGOP_STRPrivate->XC_GopReg[9], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCMISC, pGOP_STRPrivate->XC_GopReg[10], GOP_BIT2);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCALPHA, pGOP_STRPrivate->XC_GopReg[11], GOP_BIT2);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, pGOP_STRPrivate->XC_GopReg[12], GOP_REG_WORD_MASK);
        }
            break;
        default:
            break;
    }
    return GOP_SUCCESS;
}
GOP_Result HAL_GOP_GWIN_SetGPUTileMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gwinid, EN_DRV_GOP_GPU_TILE_MODE tile_mode)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_EnableTLB(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet=0xFFFF;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_SRAM_BORROW, bEnable?GOP_BIT10:0, GOP_BIT10);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetTLBAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_PHY u64TLBAddr, MS_U32 u32size)
{
    MS_U32 u32BankOffSet=0xFFFF;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_TAG_ADDR_L, u32size&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_TAG_ADDR_H, u32size>>16, GOP_REG_WORD_MASK);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_L, u64TLBAddr&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_H, u64TLBAddr>>16, GOP_REG_WORD_MASK);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetTLBSubAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_PHY u64TLBAddr)
{
    MS_U32 u32BankOffSet=0xFFFF;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_L, u64TLBAddr&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_H, u64TLBAddr>>16, GOP_REG_WORD_MASK);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_Set_GWIN_INTERNAL_MIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP,MS_U8 miusel)
{
    MS_U32 u32BankOffSet=0xFFFF;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MIU_SEL, miusel<<0, GOP_BIT0|GOP_BIT1 );//GWIN MIU Select
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MIU_SEL, miusel<<2, GOP_BIT2|GOP_BIT3 );//GWIN_3D MIU Select

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGopGwinHVPixel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_U8 u8win, MS_U16 hstart, MS_U16 hend, MS_U16 vstart, MS_U16 vend)
{
    GOP_Result ret = GOP_SUCCESS;

    switch(u8GOP)
    {
        case E_GOP0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_HSTR(u8win), hstart, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_HEND(u8win), hend, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_VSTR(u8win), vstart, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_VEND(u8win), vend, GOP_REG_WORD_MASK);
        break;

        case E_GOP1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_HSTR(u8win), hstart, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_HEND(u8win), hend, GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_VSTR(u8win), vstart, GOP_REG_WORD_MASK);    // 1 pixel
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_VEND(u8win), vend, GOP_REG_WORD_MASK);    // 1 pixel
            break;

        case E_GOP2:
        case E_GOP3:
            HAL_GOP_Write16Reg(pGOPHalLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_HSTR:GOP_1GX_HSTR, hstart, GOP_REG_WORD_MASK);    // word pixels
            HAL_GOP_Write16Reg(pGOPHalLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_HEND:GOP_1GX_HEND, hend, GOP_REG_WORD_MASK);    // word pixels
            HAL_GOP_Write16Reg(pGOPHalLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_VSTR:GOP_1GX_VSTR, vstart, GOP_REG_WORD_MASK);    // 1 pixel
            HAL_GOP_Write16Reg(pGOPHalLocal, (u8win==GOP2_Gwin0Id)?GOP_1G_VEND:GOP_1GX_VEND, vend, GOP_REG_WORD_MASK);    // 1 pixel
            break;

        default:
            printf("invalid Gwin number:%d\n",u8win);
            break;
    }

    return ret;
}

GOP_Result HAL_GOP_Set_MIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP,MS_U8 miusel)
{
    MS_U32 u32BankOffSet=0xFFFF;
    MS_U16 mask_shift=0xFF;

    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
    switch(u8GOP)
    {
        case E_GOP0:
            mask_shift = GOP_MIU_CLIENT_GOP0;
            break;
        case E_GOP1:
            mask_shift = GOP_MIU_CLIENT_GOP1;
            break;
        case E_GOP2:
            mask_shift = GOP_MIU_CLIENT_GOP2;
            break;
        case E_GOP3:
            mask_shift = GOP_MIU_CLIENT_GOP3;
            break;
        case E_GOP5:
            mask_shift = GOP_MIU_CLIENT_GOP5;
            break;
            break;
        default:
            mask_shift = 0xFF;
            MS_CRITICAL_MSG(printf("ERROR gop miu client\n"));
            break;
    }
    if(u8GOP==E_GOP5)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, miusel<<mask_shift, 1<<mask_shift );
    else
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP1, miusel<<mask_shift, 1<<mask_shift );
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP2, (miusel>>1)<<mask_shift, 1<<mask_shift );
#endif

    return GOP_SUCCESS;
}

GOP_Result Hal_SetCropWindow(
    GOP_CTX_HAL_LOCAL *pGOPHalLocal,
    MS_U8 u8GOP,
    EN_GOP_CROP_CTL crop_mode
    )
{
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_AFBC_GetCore(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP,MS_U8* u8Core)
{
    if(u8GOP==0)
    {
        *u8Core=0;
    }
    else
    {
        *u8Core=1;
    }
    return GOP_SUCCESS;
}

#ifdef GOP_CMDQ_ENABLE
GOP_Result HAL_GOP_CMDQ_WriteCommand(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *cmdq_struct,MS_U32 *number,MS_U32 u32addr, MS_U16 u16val, MS_U16 mask)
{
    MS_U16 u16xcSubbank=0;
    MS_U32 bank;
    MS_U32 direct_addr;

    switch (u32addr & 0xFF00)
    {
        case GOP_REG_BASE:
        {
            bank = (u32addr & 0xFF0000) >> 8;
#if 0 //for GOP4
            if(bank==0xE00)//GOP4:  0x121B00
            {
                    bank=GOP_REG_GOP4_BK_OFFSET;
            }
            else if(bank==0xF00)//GWIN4: 0x121E00
            {
                    bank=GOP_REG_GOP4_GW_OFFSET;
            }
            else if(bank==0x1000) //GOP4_ST
            {
                    bank=GOP_REG_GOP4_ST_OFFSET;
            }
#endif
            direct_addr = GOP_REG_DIRECT_BASE + bank + (u32addr & 0xFF);

            cmdq_struct[(*number)].destionation_address = (direct_addr&0xFFFFFF);
            cmdq_struct[(*number)].destionation_value = u16val;
            cmdq_struct[(*number)].mask = ((~mask)&0xFFFF);
            cmdq_struct[(*number)].operation = 0x57;
            (*number)++;
            break;
        }
        case SC1_REG_BASE:
            u16xcSubbank =  (u32addr & 0xFF0000)>>8 ;
            direct_addr = SC1_DIRREG_BASE + u16xcSubbank+ (u32addr & 0xFF);

            cmdq_struct[(*number)].destionation_address = (direct_addr&0xFFFFFF);
            cmdq_struct[(*number)].destionation_value = u16val;
            cmdq_struct[(*number)].mask = ((~mask)&0xFFFF);
            cmdq_struct[(*number)].operation = 0x57;
            (*number)++;
            break;
        case GE_REG_BASE:
        case CKG_REG_BASE:
        case MIU_REG_BASE:
        {
            cmdq_struct[(*number)].destionation_address = (u32addr&0xFFFFF)+0x100000;
            cmdq_struct[(*number)].destionation_value = u16val;
            cmdq_struct[(*number)].mask = ((~mask)&0xFFFF);
            cmdq_struct[(*number)].operation = 0x57;
            (*number)++;
            break;
        }
#ifdef GOP_MIU_GROUP2
        case (MIU2_REG_BASE & 0xFF00):
        {
            direct_addr = MIU2_REG_BASE + (u32addr & 0xFF);  //Direct_Base + addr_offset

            cmdq_struct[(*number)].destionation_address = (direct_addr&0xFFFFFF);
            cmdq_struct[(*number)].destionation_value = u16val;
            cmdq_struct[(*number)].mask = ((~mask)&0xFFFF);
            cmdq_struct[(*number)].operation = 0x57;
            (*number)++;
            break;
        }
#endif
        default:
        {
            //Gop lib current do not support this HW ip base
            MS_ASSERT(0);
            break;
        }

    }
    return GOP_SUCCESS;
}
GOP_Result HAL_GOP_CMDQ_BegineDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 *u32GopIdx)
{
    MS_U32 u32BankOffSet=0xFFFF;
    MS_U16 u16RegVal1 = 0;
    MS_BOOL bCheckValidGop = FALSE;
    MS_U8 u8CheckTimeCnt = 0;
    MS_U8 u8CurrentCmdGop = 0;

    while (!bCheckValidGop && (u8CheckTimeCnt < (GOPG3_GOP_CMDQ_INT_3 - GOPG0_GOP_CMDQ_INT_0 + 2)))
    {
        if (u8CurrentCmdGop > (GOPG3_GOP_CMDQ_INT_3 - GOPG0_GOP_CMDQ_INT_0))
        {
            u8CurrentCmdGop = 0;
        }
        _GetBnkOfstByGop(u8CurrentCmdGop, &u32BankOffSet);
        HAL_GOP_Read16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_CTRL0, &u16RegVal1);
        if ((u16RegVal1 & GOP_BIT0) != 0) // gop not init, cmdq won't work
        {
            bCheckValidGop = FALSE;
        }
        else
        {
            bCheckValidGop = TRUE;
            break;
        }

        // if current gop not init, use next gop instead, check order 0->2->1->3
        switch (u8CurrentCmdGop)
        {
            case 0:
                u8CurrentCmdGop = 2;
                break;
            case 1:
                u8CurrentCmdGop = 0;
                break;
            case 2:
                u8CurrentCmdGop = 3;
                break;
            case 3:
                u8CurrentCmdGop = 1;
                break;
            default:
                u8CurrentCmdGop = 0;
                break;
        }
        u8CheckTimeCnt++;
    }
    if (!bCheckValidGop)
    {
        printf("[%s] Error message no avalible gop can support current cmdq!!\n",__FUNCTION__);
    }

    *u32GopIdx = u8CurrentCmdGop;
    MDrv_CMDQ_Gen_WaitTrigger_Bus_Command(&(target[(*number)]),GOPG0_GOP_CMDQ_INT_0 + u8CurrentCmdGop,FALSE);
    (*number)++;
    return GOP_SUCCESS;
}
GOP_Result HAL_GOP_CMDQ_EndDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 u32GopIdx)
{
    CH_Struct ch_fire;
    MS_U32 Receive_Return_Value = 0,u32BankOffSet = 0,u32FireBankOffSet = 0,timer1 = 0x0,timer2 = 0x0;
    MS_U16 u16ret = 0,u16ret1 = 0;
    int i = 0;


    _GetBnkOfstByGop(0, &u32BankOffSet);
    HAL_GOP_Read16Reg(pGOPHalLocal,u32BankOffSet+GOP_4G_BG_CLR(1),&u16ret);
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,target,number,u32BankOffSet+GOP_4G_BG_CLR(1),u16ret+1,0xFFFF);//current GOP force write dis
#if 0  //for Debug
    for(i=0;i<(*number);i++)
    {
        printf("\33[0;36m [%d]op = %d, addr = %lx,value = %x,mask = %x\33[m \n",i,target[i].operation,target[i].destionation_address,target[i].destionation_value,target[i].mask);
    }
#endif
    ch_fire.Command_Number = *number;
    ch_fire.Pointer_To_CAFArray = target;
    Receive_Return_Value = MDrv_CMDQ_Receive(&ch_fire);
    if(Receive_Return_Value == DRVCMDQ_CMDQ_FULL)
    {
        Receive_Return_Value = 0;
        MDrv_CMDQ_Printf_Crash_Command();
    }

    _GetBnkOfstByGop(u32GopIdx, &u32FireBankOffSet);
    MsOS_DelayTask(1);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32FireBankOffSet+GOP_4G_MULTI_ALPHA, GOP_BIT4, GOP_BIT4);//reset mask
    HAL_GOP_Write16Reg(pGOPHalLocal, u32FireBankOffSet+GOP_4G_MULTI_ALPHA, 0, GOP_BIT4);    //reset nable detect

    HAL_GOP_Read16Reg(pGOPHalLocal,u32BankOffSet+GOP_4G_BG_CLR(1),&u16ret1);
    timer1 = MsOS_GetSystemTime();
    timer2 = MsOS_GetSystemTime();
    while( u16ret1 != (u16ret+1) && ((timer2 - timer1)<100))
    {
        HAL_GOP_Read16Reg(pGOPHalLocal,u32BankOffSet+GOP_4G_BG_CLR(1),&u16ret1);
        MsOS_DelayTask(1);
        timer2 = MsOS_GetSystemTime();
    }
    if(u16ret1 != (u16ret+1))
    {
        printf("\33[0;36m   %s:%d  timeout = %ld org = %d target = %d\33[m \n",__FUNCTION__,__LINE__,(timer2 - timer1),u16ret1,(u16ret+1));
        MDrv_CMDQ_Printf_Crash_Command();
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_CMDQ_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    CAF_Struct fire_struct[24];
    MS_U32 u32BankOffSet;
    MS_U32 number = 0;
    MS_U16 u16RegVal1 = 0,u16RegVal2 = 0,u16RegVal3 = 0,u16MiuClient = 0;
    MS_U16 u16RegMiu=0, u16RegCoreEna=0;
    MS_U32 fireGOP=0;

    _GetBnkOfstByGop(gop, &u32BankOffSet);

    if(gop == E_GOP5)
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, &u16RegVal1);
    else
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal1);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal3);
#endif
    HAL_GOP_Read16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_BANK_FWR,&u16RegVal2);
    HAL_GOP_CMDQ_BegineDraw(pGOPHalLocal,fire_struct,&number,&fireGOP);

    switch(gop)
    {
        case E_GOP0:
        {
            u16MiuClient = GOP_MIU_CLIENT_GOP0;
            break;
        }
        case E_GOP1:
        {
            u16MiuClient = GOP_MIU_CLIENT_GOP1;
            break;
        }
        case E_GOP2:
        {
            u16MiuClient = GOP_MIU_CLIENT_GOP2;
            break;
        }
        case E_GOP3:
            {
            u16MiuClient = GOP_MIU_CLIENT_GOP3;
            break;
        }
        case E_GOP4:
        {
            u16MiuClient = GOP_MIU_CLIENT_GOP4;
            break;
        }
        default:
        {
            MS_ASSERT(0);
            break;
        }
    }
    if(bMIUSelect[gop] == TRUE)
    {
        if(u16MIUSelect[gop] == 0)
        {
            u16RegVal1 &= ~(1<<u16MiuClient);
            u16RegVal3 &= ~(1<<u16MiuClient);
        }
        else if(u16MIUSelect[gop] == 1)
        {
            u16RegVal1 |= (1<<u16MiuClient);
            u16RegVal3 &= ~(1<<u16MiuClient);
        }
        else if(u16MIUSelect[gop] == 2)
        {
            u16RegVal1 &= ~(1<<u16MiuClient);
            u16RegVal3 |= (1<<u16MiuClient);
        }
        else if(u16MIUSelect[gop] == 3)
        {
            u16RegVal1 |= (1<<u16MiuClient);
            u16RegVal3 |= (1<<u16MiuClient);
        }
        bMIUSelect[gop] = FALSE;
    }

    if(bAFBCMIUSelect[gop] == TRUE)
    {
        bAFBCMIUSelect[gop] = FALSE;
    }
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2|(GOP_BIT0)) ,0xFFFF);//current GOP force write en
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2&(~GOP_BIT0)) ,0xFFFF);//current GOP force write en
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP1,u16RegVal1,0xFFFF);

    MS_U8 u8Core=0;
    HAL_GOP_AFBC_GetCore(pGOPHalLocal, gop, &u8Core);
    if(g_GopChipPro.bAFBCCore_Merge_GOP_Trig[gop] ==FALSE)
    {
        if(bAFBCMIUSelect[gop] == TRUE)
        {
            HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_MIU(u8Core),u16AFBCMIUSelect[gop]<<4, 0xFFFF);
            bAFBCMIUSelect[gop] = FALSE;
        }
        HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_MIU(u8Core),u16AFBCMIUSelect[gop]<<4, 0xFFFF);
        HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_TRIGGER(u8Core),GOP_BIT1, 0xFFFF);
        HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_TRIGGER(u8Core),GOP_BIT0, 0xFFFF);
    }
#ifdef GOP_MIU_GROUP2
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP2,u16RegVal3,0xFFFF);
#endif

    HAL_GOP_CMDQ_EndDraw(pGOPHalLocal,fire_struct,&number,fireGOP);
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_CMDQ_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask)
{
    CAF_Struct fire_struct[24];
    MS_U32 u32BankOffSet=0;
    MS_U32 number = 0;
    MS_U8 gop=0;
    MS_U16 u16RegVal1=0,u16RegVal2=0,u16RegVal3 = 0,u16MiuClient = 0;
    MS_U16 u16RegMiu=0,u16RegCoreEna=0;
    MS_U8 u8Core=0;
    MS_U32 fireGOP=0;
    MS_BOOL bCoreTrig[AFBC_CORE_COUNT];
    MS_BOOL bCoreMiu[AFBC_CORE_COUNT];
    MS_U8 i=0;

    for(i=0;i<AFBC_CORE_COUNT;i++)
    {
        bCoreTrig[i]=FALSE;
    }

    if(gop == E_GOP5)
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP5_MIU_GROUP1, &u16RegVal1);
    else
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal1);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal3);
#endif
    HAL_GOP_CMDQ_BegineDraw(pGOPHalLocal,fire_struct,&number,&fireGOP);

    for(gop = 0; gop<MAX_GOP_SUPPORT; gop++)
    {
        switch(gop)
        {
            case E_GOP0:
            {
                u16MiuClient = GOP_MIU_CLIENT_GOP0;
                HAL_GOP_AFBC_GetCore(pGOPHalLocal, (MS_U8)E_GOP0, &u8Core);
                bCoreTrig[u8Core]=TRUE;
                break;
            }
            case E_GOP1:
            {
                u16MiuClient = GOP_MIU_CLIENT_GOP1;
                HAL_GOP_AFBC_GetCore(pGOPHalLocal, (MS_U8)E_GOP1, &u8Core);
                bCoreTrig[u8Core]=TRUE;
                break;
            }
            case E_GOP2:
            {
                u16MiuClient = GOP_MIU_CLIENT_GOP2;
                HAL_GOP_AFBC_GetCore(pGOPHalLocal, (MS_U8)E_GOP2, &u8Core);
                bCoreTrig[u8Core]=TRUE;
                break;
            }
            case E_GOP3:
            {
                u16MiuClient = GOP_MIU_CLIENT_GOP3;
                HAL_GOP_AFBC_GetCore(pGOPHalLocal, (MS_U8)E_GOP3, &u8Core);
                bCoreTrig[u8Core]=TRUE;
                break;
            }
            case E_GOP5:
            {
                u16MiuClient = GOP_MIU_CLIENT_GOP5;
                HAL_GOP_AFBC_GetCore(pGOPHalLocal, (MS_U8)E_GOP5, &u8Core);
                bCoreTrig[u8Core]=TRUE;
                break;
            }
            default:
            {
                continue;
            }
        }
        if( ( u16GopMask & (1<<gop) ) )
        {
            _GetBnkOfstByGop(gop, &u32BankOffSet);
            if(bMIUSelect[gop] == TRUE)
            {
                if(u16MIUSelect[gop] == 0)
                {
                    u16RegVal1 &= ~(1<<u16MiuClient);
                    u16RegVal3 &= ~(1<<u16MiuClient);
                }
                else if(u16MIUSelect[gop] == 1)
                {
                    u16RegVal1 |= (1<<u16MiuClient);
                    u16RegVal3 &= ~(1<<u16MiuClient);
                }
                else if(u16MIUSelect[gop] == 2)
                {
                    u16RegVal1 &= ~(1<<u16MiuClient);
                    u16RegVal3 |= (1<<u16MiuClient);
                }
                else if(u16MIUSelect[gop] == 3)
                {
                    u16RegVal1 |= (1<<u16MiuClient);
                    u16RegVal3 |= (1<<u16MiuClient);
                }
                bMIUSelect[gop] = FALSE;
            }
            HAL_GOP_Read16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_BANK_FWR,&u16RegVal2);
            HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2|(GOP_BIT0)) ,0xFFFF);//current GOP force write en
            HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2&(~GOP_BIT0)) ,0xFFFF);//current GOP force write en
        }

        if(bAFBCMIUSelect[gop] == TRUE)
        {
            HAL_GOP_AFBC_GetCore(pGOPHalLocal, gop, &u8Core);
            bCoreMiu[gop]=TRUE;
            bAFBCMIUSelect[gop] = FALSE;
        }
    }

    for(i=0;i<AFBC_CORE_COUNT;i++)
    {
        if(g_GopChipPro.bAFBCCore_Merge_GOP_Trig[i] == FALSE)
        {
            if(bCoreTrig[i]==TRUE)
            {
                if((g_GopChipPro.bAFBCMIUSelDoubleBuffer[i] == FALSE)&&(bCoreMiu[i]==TRUE))
                {
                    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_MIU(i),u16AFBCMIUSelect[gop]<<4, 0xFFFF);
                }
                HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_TRIGGER(i),GOP_BIT1, 0xFFFF);
                HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,REG_AFBC_TRIGGER(i),GOP_BIT0, 0xFFFF);
            }
        }
    }

    if(gop == E_GOP5)
        HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP5_MIU_GROUP1,u16RegVal1,0xFFFF);
    else
        HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP1,u16RegVal1,0xFFFF);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP2,u16RegVal3,0xFFFF);
#endif
    HAL_GOP_CMDQ_EndDraw(pGOPHalLocal,fire_struct,&number,fireGOP);
    return GOP_SUCCESS;
}
#endif
