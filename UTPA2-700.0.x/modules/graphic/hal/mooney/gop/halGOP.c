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
// (!��MStar Confidential Information!�L) by the recipient.
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
extern MS_U8 bMIUSelect[MAX_GOP_SUPPORT];
#endif
GOP_CHIP_PROPERTY g_GopChipPro =
{
    .bSetHSyncInverse =         FALSE,
    .bGop1GPalette =            FALSE,
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
    .bGOPWithVscale =           {TRUE, TRUE, FALSE, FALSE, FALSE}, //setting GOP with/without Vscale
    .bGOPWithScaleDown      =   {FALSE, FALSE, FALSE, FALSE, FALSE},

    .bDWINSupport   =           FALSE,
    .DwinVer =             		0x1,
    .bTstPatternAlpha =         TRUE,
    .bXCDirrectBankSupport =    TRUE,   /*XC Dirrect Bank R/W*/
    .bFRCSupport =              FALSE,  /*OC path*/
    .bGOPMixerToVE=             FALSE,  /*Mixer to VE path*/
    .bBnkForceWrite =           TRUE,   /*Direct Bank Force Write*/
    .bPixelModeSupport =        TRUE,   /*Pixel Mode Support*/
    .bScalingDownSupport=       TRUE,
    .b2Pto1PSupport=            TRUE,
    .bTLBSupport=               {TRUE, FALSE, TRUE, FALSE, FALSE, FALSE},
    .GOP_TestPattern_Vaild=     E_GOP0,
    .bInternalMIUSelect=        {TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE}, //{E_GOP0, E_GOP1, E_GOP2, E_GOP3, E_GOP4, E_GOP_Dwin, E_GOP_MIXER, E_GOP5}

#ifdef ENABLE_GOP_T3DPATCH
    .GOP_PD =                   0x1C,
#else
    .GOP_PD =                   0x1C8,
#endif
    .GOP_UHD_PD_Offset =        0x0,
    .GOP_IP_PD =                0x00,
    .GOP_MVOP_PD =              0x69,
    .GOP_VE_PD =                0x89,
    .GOP_MIXER_PD =             0x0,
    .GOP_NonVS_PD_Offset =      0x5, //GOP without Vsacle might need add offset on pipedelay
    .GOP_VE_V_Offset =          0x0,

    .GOP_MUX_Delta  =           0x1,
    .GOP_Mux_Offset =           {0x0, 0x12, 0x6, 0xC, 0x18},
    .GOP_MapLayer2Mux =         {E_GOP_MUX0, E_GOP_MUX2, E_GOP_MUX3, E_GOP_MUX1, E_GOP_MUX4},
    .GOP_Mux_FRC_offset=        0x13,

    .WordUnit =                 GOP_WordUnit,
    .TotalGwinNum =             GOP_TotalGwinNum,
    .Default_ConsAlpha_bits =   DRV_VALID_8BITS,
    .enGOP3DType =              E_DRV_3D_DUP_FULL,
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
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(Gop23_GwinCtl_Ofet), colortype, 0x00f0);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(u8GwinNum), colortype, 0x00f0);
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
        default:
            break;
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
        default:
            break;
    }
    return u16GopAck;
}

GOP_Result HAL_GOP_EnableTwoLineBufferMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEnable)
{
    MS_U32 u32BankOffSet;

    if (u8GOP < 2) /* only GOP0/GOP1 can support two linebuufer mode */
    {
        _GetBnkOfstByGop(u8GOP, &u32BankOffSet);
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MULTI_ALPHA, bEnable?GOP_BIT12:0, GOP_BIT12);
    }

    return GOP_SUCCESS;
}

void HAL_GOP_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum)
{
    MS_U32 u32bankoff = 0;
    MS_U16 mask_shift=0xFF;
    MS_U16 u16RegVal=0xFF;
    MS_U8 u8MIUSel=0xF;

    if(u8GOPNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,u8GOPNum);
        return;
    }

    _GetBnkOfstByGop(u8GOPNum, &u32bankoff);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_CTRL0, 0x000, GOP_BIT9);     // Genshot fast=0 for t3, for T4 and after no need to set this bit.


    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT14, GOP_BIT14);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT12, GOP_BIT13|GOP_BIT12);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_HW_USAGE, 0, GOP_BIT0);

    if((u8GOPNum == E_GOP0)||(u8GOPNum == E_GOP1))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MULTI_ALPHA, GOP_BIT12, GOP_BIT12);

        HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_SCALING, GOP_BIT2|GOP_BIT3 , GOP_REG_WORD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_MG, 0, GOP_BIT2);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, GOP_BIT8|GOP_BIT10|GOP_BIT12, (GOP_BIT8|GOP_BIT9)|(GOP_BIT10|GOP_BIT11)|(GOP_BIT12|GOP_BIT13));
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
        default:
            mask_shift = 0xFF;
            return;
    }


    if(pGOPHalLocal->pGopChipPro->bInternalMIUSelect[u8GOPNum] ==TRUE)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, &u16RegVal);
        u8MIUSel = (u16RegVal>>mask_shift)&0x0000001UL;
        HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MIU_SEL, u8MIUSel, GOP_BIT0|GOP_BIT1 );//GWIN MIU Select
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP, 1<<mask_shift, 1<<mask_shift );//Get Control MIU select by GOP itself
    }

    // set VIP/VOP timing select, always select VOP should be OK.
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MULTI_ALPHA, GOP_BIT10, GOP_BIT10);
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
        case E_GOP4:
            u8GWinId = GOP4_GwinIdBase + u8Idx;
            break;
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
    case E_GOP4: // GOP1GS
            pGOPHalLocal->bank_offset = GOP_1GS0_OFST<<16;
            return GOP_SUCCESS;
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
    MS_U16 u16GetBank;
    u16GetBank = GOP_READ2BYTE(GOP_BAK_SEL)&0xF;
    return u16GetBank;
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


GOP_Result HAL_GOP_GWIN_SetDstPlane(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType)
{
    /* GOP dst type:
        1: IP (Main)
        0: IP (Sub)
        2: OP
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
            u16RegVal = 0x0;
            bEnable = FALSE;
            u16HSyncMask=0;
            break;

        case E_DRV_GOP_DST_IP0_SUB:
            u16RegVal = 0x1;
            bEnable = TRUE;
            u16HSyncMask=0;
            break;

        case E_DRV_GOP_DST_OP0:
            u16RegVal = 0x2;
            bEnable = FALSE;
            u16HSyncMask=GOP_BIT14;
            break;

        case E_DRV_GOP_DST_VOP:
            u16RegVal = 0x3;
            bEnable = FALSE;
            u16HSyncMask=GOP_BIT14;
            break;

        case E_DRV_GOP_DST_VOP_SUB:
            u16RegVal = 0x4;
            bEnable = FALSE;
            u16HSyncMask=GOP_BIT14;
            break;

        default:
            return GOP_FUN_NOT_SUPPORTED;
    }

    _GetBnkOfstByGop(GopNum, &u32BankOffSet);
    HAL_GOP_SetGOP2Pto1P(pGOPHalLocal, GopNum, bEnable);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL1, u16RegVal, BMASK(GOP_BIT3:0));

    //for Monaco/Muji/Monet, IP need set Vsync inverse in IP
    if (eDstType == E_DRV_GOP_DST_IP0)
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL0, (MS_U16)GOP_BIT1, GOP_BIT1);
    else
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL0, (MS_U16)~GOP_BIT1, GOP_BIT1);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL0, u16HSyncMask, GOP_BIT14);               // Set mask Hsync when VFDE is low

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType eDstType)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_GetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType *pGopDst)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_InitMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    /*T8
    OP path: support 3 mux (mux0, mux 2, mux3) to blend with SC simultaneously
    IP  path: support mux0 and mux1 to IPMain/IPSub. Only one mux of mux0 and mux1 can be blended to IPMain/IPSub
    SW default setting=> mux0:gop1g, mux1:gop1gx, mux2:gop2g, mux3:gop4g
    */
    MS_U8 gop4g=0, gop2g=1, gop1g=2, gop1gx=3, gop1gs=4;
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, ((gop1gs<<(GOP_MUX_SHIFT*4))|(gop1gx<<(GOP_MUX_SHIFT*3))|(gop1g<<(GOP_MUX_SHIFT*2))|(gop2g<<(GOP_MUX_SHIFT*1))|gop4g), GOP_REG_WORD_MASK);
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
          break;
        case E_GOP_IP0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_MAIN_MUX_SHIFT, GOP_IP_MAIN_MUX_MASK);
            break;
        case E_GOP_IP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_MAIN_MUX_SHIFT, GOP_IP_MAIN_MUX_MASK);
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
    MS_U16 muxValue=0, regval=0;

    DRV_GOPDstType pGopDst = E_DRV_GOP_DST_INVALID;
    MS_U16 u164K2KMuxValue=0;

    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum);
        return GOP_FAIL;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPEN, &regval);

    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT15):(regval & ~GOP_BIT15), GOP_BIT15);
    }

    if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval | GOP_BIT12):(regval & ~GOP_BIT12), GOP_BIT12);
    }

    if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT14):(regval & ~GOP_BIT14), GOP_BIT14);
    }

    if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT13):(regval & ~GOP_BIT13), GOP_BIT13);
    }

    if (gopNum== ((muxValue & GOP_MUX4_MASK)>> (GOP_MUX_SHIFT*4))) //enable mux4
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |GOP_BIT11):(regval & ~GOP_BIT11), GOP_BIT11);
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
        HAL_GOP_Write16Reg(pGOPHalLocal, u32pBankOffSet + GOP_4G_SRAM_BORROW, bEnable?(u16Regval |0x0800):(u16Regval & ~0x0800), 0x0800);
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
        *pbEnable = (regval & GOP_BIT4) == GOP_BIT4;
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        *pbEnable = (regval & GOP_BIT12) == GOP_BIT12;
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        *pbEnable = (regval & GOP_BIT6) == GOP_BIT6;
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        *pbEnable = (regval & GOP_BIT8) == GOP_BIT8;
    }
    else if (gopNum== ((muxValue & GOP_MUX4_MASK)>> (GOP_MUX_SHIFT*4))) //enable mux4
    {
        *pbEnable = (regval & GOP_BIT14) == GOP_BIT14;
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
    if(gopNum >= MAX_GOP_SUPPORT)
    {
        printf("[%s][%d] Out of GOP support!!! GOP=%d\n",__FUNCTION__,__LINE__ ,gopNum);
        MS_ASSERT(0);
        return GOP_INVALID_PARAMETERS;
    }

    switch(eDstType)
    {
        case E_DRV_GOP_DST_IP0:
            if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK2, CKG_GOPG1_MASK);
            }
            else if (gopNum ==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_IDCLK2, CKG_GOPG2_MASK);
            }
            else if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG0_IDCLK2, CKG_GOPG0_MASK);
            }
            else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
            break;

        case E_DRV_GOP_DST_IP0_SUB:
            if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK1, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_IDCLK1, CKG_GOPG2_MASK);
            }
            else if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG0_IDCLK1, CKG_GOPG0_MASK);
            }
            else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
            break;

        case E_DRV_GOP_DST_OP0:
            if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_ODCLK, CKG_GOPG1_MASK);
            }
            else if (gopNum==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_ODCLK, CKG_GOPG2_MASK);
            }
            else if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG0_ODCLK, CKG_GOPG0_MASK);
            }
            else
            {
                MS_ASSERT(0);
                return GOP_INVALID_PARAMETERS;
            }
            break;

        case E_DRV_GOP_DST_VOP:
            if (gopNum==E_GOP1)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK2, CKG_GOPG1_MASK);
            }
            else if (gopNum ==E_GOP2)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG2_IDCLK2, CKG_GOPG2_MASK);
            }
            else if (gopNum==E_GOP0)
            {
                HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG0_IDCLK2, CKG_GOPG0_MASK);
            }
            else
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
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPD_CLK_ODCLK, CKG_GOPD_MASK);
    else if (enSrcSel==GOP_DRV_DWIN_SRC_MVOP)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPD_CLK_DC0CLK, CKG_GOPD_MASK);
    else if (enSrcSel==GOP_DRV_DWIN_SRC_IP)
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPD_CLK_IDCLK2, CKG_GOPD_MASK);
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
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPD_DISABLE_CLK, CKG_GOPD_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, CKG_SRAM0_DISABLE_CLK, CKG_SRAM0_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SRAMCLK, CKG_SRAM1_DISABLE_CLK, CKG_SRAM1_MASK);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_MIXER_SetGOPEnable2Mixer(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_MIXER_SetMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_U8 muxNum, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_MIXER_EnableOldBlendMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEn)
{
    return GOP_FUN_NOT_SUPPORTED;
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

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, &u16RegVal_L);
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
        u8Miu= u16RegVal_L& (GOP_BIT0|GOP_BIT1);
    }
    else
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, &u16RegVal_L);

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
        break;

    case MS_DRV_IP0_SEL_GOP1:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        if(E_GOP1 == (muxValue &GOP_MUX0_MASK))
            u16RegVal |= GOP_BIT12;//mux0
        else
            u16RegVal |= GOP_BIT13;//mux1
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        break;
    case MS_DRV_IP0_SEL_GOP2:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        if(E_GOP2 == (muxValue &GOP_MUX0_MASK))
            u16RegVal |= GOP_BIT12;//mux0
        else
            u16RegVal |= GOP_BIT13;//mux1
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
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

        break;
    case MS_DRV_MVOP_SEL:
        u16RegVal = GOP_BIT7 | GOP_BIT5 ;
        u16RegVal |= GOP_BIT12; //mux0
        u16RegMsk = 0xFFE7;//Skip bit3/4 for GOP_SUB_IP control
        break;
    default:
        printf("[%s] ERROR invalid source select\n",__FUNCTION__);
        break;
    }
    if(0 != u16RegMsk)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, u16RegVal, u16RegMsk);
    }
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
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_MIXER_SetOutputTiming(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32mode, GOP_DRV_MixerTiming *pTM)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_MIXER_EnableVfilter(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEn)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_GWIN_EnableTileMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable, E_GOP_TILE_DATA_TYPE tilemode)
{
     return GOP_FUN_NOT_SUPPORTED;
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
GOP_Result HAL_GOP_VE_SetOSDEnable(MS_BOOL bEnable, EN_VE_OSD_ENABLE eOSD, MS_U8 gopNum)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_SetGOPToVE(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEn )
{
    return GOP_FUN_NOT_SUPPORTED;
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
    u16Mux[4] =( u16Val & GOP_MUX4_MASK)    >> (GOP_MUX_SHIFT*4);

    switch(u8GOP)
    {
        case E_GOP0:
        case E_GOP1:
        case E_GOP2:
                for(i=0; i<MAX_GOP_MUX; i++)
                {
                    if(u8GOP == u16Mux[i])
                    {
                        eGopMux = i;
						if(eGopMux == 4)
						{
							HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_4K2K, bOCEn<<15, GOP_BIT15);
						}
						else
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
    return GOP_FUN_NOT_SUPPORTED;
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

            //SRAM
            HAL_GOP_Read16Reg(pGOPHalLocal, CKG_GOPG0_SCALING, &(pGOP_STRPrivate->GS_GopReg[0]));
            HAL_GOP_Read16Reg(pGOPHalLocal, CKG_GOPG0_MG, &(pGOP_STRPrivate->GS_GopReg[1]));
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, &(pGOP_STRPrivate->GS_GopReg[2]));

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

            //SRAM
            HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_SCALING, pGOP_STRPrivate->GS_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_MG, pGOP_STRPrivate->GS_GopReg[1], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, pGOP_STRPrivate->GS_GopReg[2], GOP_REG_WORD_MASK);

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
    MS_U16 regval=0;

    switch(tile_mode)
    {
        case E_DRV_GOP_GPU_TILE_LINEAR:
            regval=0;
            break;
        case E_DRV_GOP_GPU_TILE_DIP8X:
            regval=1<<12;
            break;
        case E_DRV_GOP_GPU_TILE_DIP16X:
            regval=2<<12;
            break;
        case E_DRV_GOP_GPU_TILE_GPU:
            regval=3<<12;
            break;
        default:
            return GOP_FAIL;
            break;
    }
    regval |= GPU_TILE_FORMAT_ARGB8888<<4;

    if (gwinid<(MAX_GOP0_GWIN+MAX_GOP1_GWIN))
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, (gwinid < MAX_GOP0_GWIN)? GOP_4G_GWIN0_CTRL(gwinid):GOP_2G_GWIN_CTRL(gwinid-MAX_GOP0_GWIN), regval, 0x31f0);
    }
    else
    {
        if (gwinid==(MAX_GOP0_GWIN+MAX_GOP1_GWIN))
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN0_CTRL, regval, 0x31f0);
        }
        else if ((gwinid==(MAX_GOP0_GWIN+MAX_GOP1_GWIN+MAX_GOP2_GWIN)))
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_GWIN0_CTRL, regval, 0x31f0);
        }
        else
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GS0_GWIN0_CTRL, regval, 0x31f0);
        }
    }

    return GOP_SUCCESS;

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

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MIU_SEL, miusel, GOP_BIT0|GOP_BIT1 );//GWIN MIU Select
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MIU_SEL, miusel<<2, GOP_BIT2|GOP_BIT3 );//GWIN_3D MIU Select
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_MIU_SEL, miusel<<4, GOP_BIT4|GOP_BIT5 );//GWIN Palette MIU Select

    return GOP_SUCCESS;
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
        case E_GOP4:
            mask_shift = GOP_MIU_CLIENT_GOP4;
            break;
        case E_GOP5:
            mask_shift = GOP_MIU_CLIENT_GOP5;
            break;
        case E_GOP_Dwin:
            mask_shift = GOP_MIU_CLIENT_DWIN;
            break;
        default:
            mask_shift = 0xFF;
            MS_CRITICAL_MSG(printf("ERROR gop miu client\n"));
            break;
    }

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, miusel<<mask_shift, 1<<mask_shift );
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP2, (miusel>>1)<<mask_shift, 1<<mask_shift );
#endif

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
GOP_Result HAL_GOP_CMDQ_BegineDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 u32GopIdx)
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
    _GetBnkOfstByGop(u8CurrentCmdGop, &u32BankOffSet);
    HAL_GOP_Read16Reg(pGOPHalLocal,u32BankOffSet+GOP_4G_MULTI_ALPHA,&u16RegVal1);
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,target,number,u32BankOffSet+GOP_4G_MULTI_ALPHA,(u16RegVal1|GOP_BIT4),0xFFFF);//reset mask
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,target,number,u32BankOffSet+GOP_4G_MULTI_ALPHA,(u16RegVal1&(~GOP_BIT4)) ,0xFFFF);    //reset nable detect
    MDrv_CMDQ_Gen_WaitTrigger_Bus_Command(&(target[(*number)]),GOPG0_GOP_CMDQ_INT_0 + u8CurrentCmdGop,FALSE);
    (*number)++;
    return GOP_SUCCESS;
}
GOP_Result HAL_GOP_CMDQ_EndDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 u32GopIdx)
{
    CH_Struct ch_fire;
    MS_U32 Receive_Return_Value = 0,u32BankOffSet = 0,timer1 = 0x0,timer2 = 0x0;
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
        printf("\33[0;36m   %s:%d  timeout = %ld org = %d target = %d\33[m \n",__FUNCTION__,__LINE__,(timer2 - timer1),u16ret1,(u16ret+1));
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_CMDQ_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    CAF_Struct fire_struct[24];
    MS_U32 u32BankOffSet=0xFFFF;
    MS_U32 number = 0;
    MS_U16 u16RegVal1 = 0,u16RegVal2 = 0,u16RegVal3 = 0,u16MiuClient = 0;
    _GetBnkOfstByGop(gop, &u32BankOffSet);

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, &u16RegVal1);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal3);
#endif
    HAL_GOP_Read16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_BANK_FWR,&u16RegVal2);
    HAL_GOP_CMDQ_BegineDraw(pGOPHalLocal,fire_struct,&number,(MS_U32)0);

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
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2|(GOP_BIT0)) ,0xFFFF);//current GOP force write en
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,u32BankOffSet+GOP_4G_BANK_FWR,(u16RegVal2&(~GOP_BIT0)) ,0xFFFF);//current GOP force write en
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_SC_MIU_SEL,u16RegVal1,0xFFFF);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP2,u16RegVal3,0xFFFF);
#endif
    HAL_GOP_CMDQ_EndDraw(pGOPHalLocal,fire_struct,&number,(MS_U32)0);
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_CMDQ_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask)
{
    CAF_Struct fire_struct[24];
    MS_U32 u32BankOffSet;
    MS_U32 number = 0;
    MS_U8 gop;
    MS_U16 u16RegVal1=0,u16RegVal2=0,u16RegVal3 = 0,u16MiuClient = 0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_MIU_SEL, &u16RegVal1);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP2, &u16RegVal3);
#endif
    HAL_GOP_CMDQ_BegineDraw(pGOPHalLocal,fire_struct,&number,(MS_U32)0);

    for(gop = 0; gop<MAX_GOP_SUPPORT; gop++)
    {
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
    }
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_SC_MIU_SEL,u16RegVal1,0xFFFF);
#ifdef GOP_MIU_GROUP2
    HAL_GOP_CMDQ_WriteCommand(pGOPHalLocal,fire_struct,&number,GOP_MIU_GROUP2,u16RegVal3,0xFFFF);
#endif
    HAL_GOP_CMDQ_EndDraw(pGOPHalLocal,fire_struct,&number,(MS_U32)0);
    return GOP_SUCCESS;
}
#endif
