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
#include <string.h>
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
#define RIU     ((unsigned short volatile *) pGOPHalLocal->u32_mmio_base)
#define GOP_WRITE2BYTE(addr, val)    { RIU[addr] = val; }
#define GOP_READ2BYTE(addr)            RIU[addr]
#define GOP_BANK_MASK   0x0F


//------------------------------------------------------------------------------
//  Local Var
//------------------------------------------------------------------------------
MS_BOOL bIsMuxVaildToGopDst[MAX_GOP_MUX][MAX_DRV_GOP_DST_SUPPORT] =
{
    /*IP0,      IP0_SUB,  MIXER2VE, OP0,         VOP,   IP1,       IP1_SUB, MIXER2OP*/
    {TRUE,    FALSE, FALSE,    TRUE,    TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 0 */
    {TRUE,    FALSE, FALSE,    FALSE,   TRUE, FALSE, FALSE,  FALSE},        /*All Gop Dst case is vaild or FALSE for mux 1 */
    {TRUE,    FALSE, FALSE,    TRUE,    TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 2 */
    {TRUE,    FALSE, FALSE,    FALSE,   TRUE, FALSE, FALSE,  FALSE},         /*All Gop Dst case is vaild or FALSE for mux 3 */

};


GOP_CHIP_PROPERTY g_GopChipPro =
{
    .bSetHSyncInverse =         FALSE,
    .bGop1GPalette =            FALSE,
    .bSetHPipeOfst =            FALSE,
    .bNeedCheckMVOP =           FALSE,
    .bNeedSetMUX1ToIP0 =        FALSE,
    .bNeedSetMUX3ToIP0 =        FALSE,
    .bNewMux   =                TRUE,
    .bNewPalette   =            FALSE,
    .bNewBwReg =                TRUE,
    .bGop2VStretch =            TRUE,
    .bIgnoreIPHPD  =            FALSE,  //Uranus4 has handshack with XC, should not set HPD
    .bIgnoreVEHPD  =            FALSE,  //Uranus4 to VE through Mixer, do not need adjust HPD
    .bhastilemode  =            FALSE,
    .bInitNotEnableGOPToSC =    FALSE,  //For Uranus4 mux init setting, enable GOP to SC in GOP init would cause problem
    .bAutoAdjustMirrorHSize =   FALSE,   //whether hw will auto adjust start addr when H mirror is enable
    .bGOPWithVscale =           {TRUE, TRUE, TRUE, TRUE}, //setting GOP with/without Vscale
    .DwinVer =                  0x1,
    .bTstPatternAlpha =         FALSE,
    .bXCDirrectBankSupport =    TRUE,   /*XC Dirrect Bank R/W*/
    .bFRCSupport =              FALSE,  /*OC path*/
    .bGOPMixerToVE=             FALSE,  /*Mixer to VE path*/
    .bBnkForceWrite =           FALSE,   /*Direct Bank Force Write*/
    .bPixelModeSupport =        FALSE,  /*Pixel Mode Support*/
    .bScalingDownSupport=       FALSE,
    .b2Pto1PSupport=            FALSE,
    .bTLBSupport=               {TRUE, FALSE, TRUE, FALSE, FALSE, FALSE},
    .GOP_TestPattern_Vaild=     E_GOP2,

#ifdef ENABLE_GOP_T3DPATCH
    .GOP_PD =                   GOP_PD_T3D,
#else
    .GOP_PD =                   0x114,
#endif
    .GOP_IP_PD =                (-0x3),
    .GOP_MVOP_PD =              0x6E,
    .GOP_VE_PD =                0x89,
    .GOP_MIXER_PD =             0x0,
    .GOP_NonVS_PD_Offset =      0x0, //GOP without Vsacle might need add offset on pipedelay
    .GOP_VE_V_Offset =          0x0,
    .GOP_UHD_PD_Offset =        0x0,

    .GOP_MUX_Delta  =           0x1,
    .GOP_Mux_Offset =           {0x0, 0x9, 0xC, 0xF},
    .GOP_Mux_FRC_offset=        0x0,
    .GOP_MapLayer2Mux =         {E_GOP_MUX0, E_GOP_MUX1, E_GOP_MUX2, E_GOP_MUX3},
    .WordUnit =                 GOP_WordUnit,
    .TotalGwinNum =             GOP_TotalGwinNum,
    .Default_ConsAlpha_bits =   DRV_VALID_8BITS,
    .enGOP3DType =              E_DRV_3D_DUP_HALF,
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
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(Gop23_GwinCtl_Ofet), colortype, GOP_REG_COLORTYPE_MASK<<GOP_REG_COLORTYPE_SHIFT);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet + GOP_4G_GWIN0_CTRL(u8GwinNum), colortype, GOP_REG_COLORTYPE_MASK<<GOP_REG_COLORTYPE_SHIFT);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_Set_PINPON(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum, MS_BOOL bEn, E_DRV_GOP_PINPON_MODE pinpon_mode)
{
    MS_U32 u32BankOffSet =0;
    MS_U32 u32BitMask=0,Regval=0;

    _GetBnkOfstByGop(u8GOPNum, &u32BankOffSet);

    switch(pinpon_mode)
    {
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
            break;
        default:
            break;
    }

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+GOP_4G_CTRL1, Regval, u32BitMask);
    return GOP_SUCCESS;
}


MS_BOOL _GetBnkOfstByGop(MS_U8 gop, MS_U32 *pBnkOfst)
{
    if (gop==0)
        *pBnkOfst = GOP_4G_OFST<<16;
    else if (gop==1)
        *pBnkOfst = GOP_2G_OFST<<16;
    else if (gop==2)
        *pBnkOfst = GOP_1G_OFST<<16;
    else if (gop==3)
        *pBnkOfst = GOP_1GX_OFST<<16;
    else if (gop==4)
        *pBnkOfst = GOP_DW_OFST<<16;
    else
        return FALSE;

    return TRUE;
}

MS_BOOL HAL_GOP_GWIN_IsNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP)
{
    return FALSE;
}

GOP_Result HAL_GOP_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask)
{
    MS_U16 u16Mask = 0;

    if (u16GopMask&GOP_BIT0)
    {
        u16Mask |= GOP_BIT12;
    }
    if (u16GopMask&GOP_BIT1)
    {
        u16Mask |= GOP_BIT13;
    }
    if (u16GopMask&GOP_BIT2)
    {
        u16Mask |= GOP_BIT15;
    }
    if (u16Mask != 0)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, 0xFFFF , u16Mask);
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    switch(gop)
    {
        case E_GOP0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT12 , GOP_BIT12);
            break;
        case E_GOP1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT13 , GOP_BIT13);
            break;
        case E_GOP2:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_BIT15 , GOP_BIT15);
            break;
        default:
            break;
    }
    return GOP_SUCCESS;
}

MS_U16 HAL_GOP_GetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop)
{
    MS_U16 u16GopAck = 0,reg_val =0;
    switch(gop)
    {
        case E_GOP0:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&0x1000)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP1:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&0x2000)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP2:
        case E_GOP3:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&0x8000)
                u16GopAck = FALSE;
            else
                u16GopAck = TRUE;
            break;
        case E_GOP_Dwin:
            HAL_GOP_Read16Reg(pGOPHalLocal,GOP_BAK_SEL,&reg_val);
            if(reg_val&0x4000)
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
    return GOP_FUN_NOT_SUPPORTED;
}

MS_BOOL bAddOffset = FALSE;
void HAL_GOP_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum)
{
    MS_U32 u32bankoff = 0;
    MS_U16 chipid=0;
    const SYS_Info *sys_info = NULL;

    _GetBnkOfstByGop(u8GOPNum, &u32bankoff);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_NEW_BW, GOP_BIT7|GOP_BIT15, GOP_BIT7|GOP_BIT15 ); //use new BW mode, enable couple LB
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_BW, GOP_FIFO_BURST_ALL, GOP_FIFO_BURST_MASK );  //set GOP DMA Burst length to "32"
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_BW, GOP_FIFO_THRESHOLD, GOP_REG_LW_MASK );  //set DMA FIFO threshold to 3/4 FIFO length
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_CTRL0, 0x000, 0x200);     // Genshot fast=0 for t3, for T4 and after no need to set this bit.
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_DUMMY_22, 0x0, 0x200);     //0: Vsync will use the same pipe delay as Hsync

    // set VIP/VOP timing select, always select VOP should be OK.
    HAL_GOP_Write16Reg(pGOPHalLocal, u32bankoff+GOP_4G_MULTI_ALPHA, GOP_BIT10, GOP_BIT10);

    chipid=MDrv_SYS_GetChipID();
    sys_info=MDrv_SYS_GetInfo();
    if( ( bAddOffset == FALSE ) && ( chipid == CHIPID_MUSTANG_SERIES )){
        pGOPHalLocal->pGopChipPro->GOP_PD += MUSTANG_PD_OFFSET;
        bAddOffset = TRUE;
    }
}

void HAL_GOP_Chip_Proprity_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    *pGOPHalLocal->pGopChipPro = g_GopChipPro;
}

MS_U8 HAL_GOP_GetMaxGwinNumByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GopNum)
{
    if (u8GopNum==0)
    {
        return (MS_U8)MAX_GOP0_GWIN;
    }
    else if (u8GopNum==1)
    {
        return (MS_U8)MAX_GOP1_GWIN;
    }
    else if (u8GopNum==2)
    {
        return (MS_U8)MAX_GOP2_GWIN;
    }
    else if (u8GopNum==3)
    {
        return (MS_U8)MAX_GOP3_GWIN;
    }
    else
    {
        MS_ASSERT(0);
        return 0xFF;
    }
}

MS_U8 HAL_GOP_SelGwinIdByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8Gop, MS_U8 u8Idx)
{
    MS_U8 u8GWinId = 0;

    //Adjust GWIN ID by different Chip

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
        default:
            break;
    }
    return u8GWinId;

}

GOP_Result HAL_GOP_GOPSel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum)
{
    switch(u8GOPNum)
    {
    case 0: // GOP4G
            pGOPHalLocal->bank_offset = GOP_4G_OFST<<16;
            return GOP_SUCCESS;
    case 1: // GOP2G
            pGOPHalLocal->bank_offset = GOP_2G_OFST<<16;
            return GOP_SUCCESS;
    case 2: // GOP1G
            pGOPHalLocal->bank_offset = GOP_1G_OFST<<16;
            return GOP_SUCCESS;
    case 3: // GOP1GX
            pGOPHalLocal->bank_offset = GOP_1GX_OFST<<16;
            return GOP_SUCCESS;
    case 4: // GOPDWX
            pGOPHalLocal->bank_offset = GOP_DW_OFST<<16;
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
    u16Bank &= ~GOP_BANK_MASK;
    u16Bank |= (u8bank&GOP_BANK_MASK);
    GOP_WRITE2BYTE(GOP_BAK_SEL, u16Bank);
}

MS_U8 HAL_GOP_Get_BANK(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    MS_U16 u16GetBank;
    u16GetBank = GOP_READ2BYTE(GOP_BAK_SEL);
    return (u16GetBank&GOP_BANK_MASK);
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
            bank = (u32addr & 0xF0000) >> 8;
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
        case MIU_REG_BASE:
        case MVOP_REG_BASE:
        {
            *pu16ret = GOP_READ2BYTE((u32addr&0xFFFF));
            break;
        }
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

    HAL_GOP_DEBUGINFO(printf("HAL_GOP_Write16Reg[%04x] = %04x\n", u16addr, u16val));

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
            bank = (u32addr & 0xF0000) >> 8;
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
        case MIU_REG_BASE:
        {
            GOP_WRITE2BYTE((u32addr&0xFFFF), u16val);
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
            bank = (u32addr & 0xF0000) >> 8;
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
    HAL_GOP_DEBUGINFO(printf("GOP_Write32Pal : i= %02bx, ARGB = %02bx,%02bx,%02bx,%02bx\n",
                                                                                                                        u8Index, u8A, u8R, u8G, u8B));
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
        regval = (MS_U16)(bEnable?(1<<14):0)|(MS_U16)((u8coef_cpt&0xFC)<<6);
        HAL_GOP_Write16Reg(pGOPHalLocal, (u8win < MAX_GOP0_GWIN)? GOP_4G_GWIN0_CTRL(u8win):GOP_2G_GWIN_CTRL(u8win-MAX_GOP0_GWIN), regval, 0x7f00);
        HAL_GOP_Write16Reg(pGOPHalLocal, (u8win < MAX_GOP0_GWIN)? GOP_4G_GWIN_ALPHA01(u8win):GOP_2G_GWIN_ALPHA01(u8win-MAX_GOP0_GWIN), (u8coef_cpt&0x03), 0x03);
    }
    /*Only alpha coeffient 8bit chip has GOP2,GOP3...*/
    else
    {
        if (u8win==(MAX_GOP0_GWIN+MAX_GOP1_GWIN))
        {
            regval = (MS_U16)(bEnable?(1<<14):0)|(MS_U16)((u8coef_cpt&0xFC)<<6);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN0_CTRL, regval, 0x7f00);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_GWIN_ALPHA01, (u8coef&0x03), 0x03);
        }
        else
        {
            printf("[%s]ERROR Invalid GwinID %d\n",__FUNCTION__,u8win);
        }
    }
}

void HAL_GOP_SetIOMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 addr)
{
    pGOPHalLocal->u32_mmio_base = addr;
}
void HAL_GOP_SetIOFRCMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 addr)
{
}
void HAL_GOP_SetIOPMMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 addr)
{
}


GOP_Result HAL_GOP_GWIN_SetDstPlane(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType,MS_BOOL bOnlyCheck)
{
    /* GOP dst type:
        0: IP (Main)
        1: IP (Sub)
        2: OP
        3: MVOP
    */
    MS_U16 u16RegVal;

    switch (eDstType)
    {
        case E_DRV_GOP_DST_IP0:
            u16RegVal = 0x0;
            break;

        case E_DRV_GOP_DST_IP0_SUB:
            u16RegVal = 0x1;
            break;

        case E_DRV_GOP_DST_OP0:
            u16RegVal = 0x2;
            break;

        case E_DRV_GOP_DST_VOP:
            u16RegVal = 0x3;
            break;

        default:
            return GOP_FUN_NOT_SUPPORTED;
    }

    if(bOnlyCheck == FALSE)
    {
        if (GopNum==0)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_CTRL1, u16RegVal, 0x0007);
            return GOP_SUCCESS;
        }
        else if (GopNum==1)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_CTRL1, u16RegVal, 0x0007);
            return GOP_SUCCESS;
        }
        else if (GopNum==2)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_CTRL1, u16RegVal, 0x0007);
            return GOP_SUCCESS;
        }
        else if (GopNum==3)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1GX_CTRL1, u16RegVal, 0x0007);
            return GOP_SUCCESS;
        }
        else
        {
            return GOP_INVALID_PARAMETERS;
        }
    }
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
    /*
    OP path: support 4 mux (mux0/1/2/3) to blend with SC simultaneously
    IP  path: support mux0 and mux1 to IPMain/IPSub. Only one mux of mux0 and mux1 can be blended to IPMain/IPSub
    SW default setting=> mux0:gop1g, mux1:gop1gx, mux2:gop2g, mux3:gop4g
    */
    MS_U8 gop4g=0, gop2g=1, gop1g=2, gop1gx=3;
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, ((gop1gx<<(GOP_MUX_SHIFT*3))|(gop1g<<(GOP_MUX_SHIFT*2))|(gop2g<<(GOP_MUX_SHIFT*1))|gop4g), GOP_REG_WORD_MASK);
    return GOP_SUCCESS;
}

void HAL_GOP_GWIN_GetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8* u8GOPNum, Gop_MuxSel eGopMux)
{
    MS_U16 u16GopMux=0;
    if(eGopMux <4)
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &u16GopMux);
    else
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX_IPVOP, &u16GopMux);

    *u8GOPNum = (u16GopMux >> ((eGopMux%4)*GOP_MUX_SHIFT))& GOP_REGMUX_MASK;
}

void HAL_GOP_GWIN_SetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8 u8GOPNum, Gop_MuxSel eGopMux)
{
    switch(eGopMux)
    {
        case E_GOP_MUX0:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, u8GOPNum <<(GOP_MUX_SHIFT*eGopMux), GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*eGopMux));
            break;
        case E_GOP_MUX1:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, u8GOPNum <<(GOP_MUX_SHIFT*eGopMux), GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*eGopMux));
            break;
        case E_GOP_MUX2:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, u8GOPNum <<(GOP_MUX_SHIFT*eGopMux), GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*eGopMux));
            break;
        case E_GOP_MUX3:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, u8GOPNum <<(GOP_MUX_SHIFT*eGopMux), GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*eGopMux));
            break;
        case E_GOP_IP0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_MAIN_MUX_SHIFT, GOP_IP_MAIN_MUX_MASK);
            break;
        case E_GOP_IP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_SUB_MUX_SHIFT, GOP_IP_SUB_MUX_MASK);
            break;
        case E_GOP_VOP0_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_VOP0_MUX_SHIFT, GOP_IP_VOP0_MUX_MASK);
            break;
        case E_GOP_VOP1_MUX:
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX_IPVOP, u8GOPNum << GOP_IP_VOP1_MUX_SHIFT, GOP_IP_VOP1_MUX_MASK);
            break;
        default:
            printf("[%s]ERROR mux setting\n",__FUNCTION__);
            break;
    }
}

GOP_Result HAL_GOP_SetGOPEnable2SC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    /* GOP OP Path enable to SC Setting
        : GOP OP Path blending with SC sequence
    */
    MS_U16 muxValue=0, regval=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPEN, &regval);

    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |0x8000):(regval & ~0x8000), 0x8000);
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux2
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |0x4000):(regval & ~0x4000), 0x4000);
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux3
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |0x2000):(regval & ~0x2000), 0x2000);
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux1
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, bEnable?(regval |0x1000):(regval & ~0x1000), 0x1000);
    }
    else
    {
        return GOP_FAIL;
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOP2Pto1P(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_SetGOPEnable2Mode1(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable)
{
    /* GOP OP Path enable to SC Setting
        A5: GOP OP Path blending with SC sequence
        mux1-->mux0-->mux2-->mux3
    */
    MS_U16 muxValue=0, regval=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, &regval);
    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x10):(regval & ~0x10), 0x30);
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        //printf("");
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x40):(regval & ~0x40), 0xC0);
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x100):(regval & ~0x100), 0x300);
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, bEnable?(regval |0x1000):(regval & ~0x1000), 0x3000);
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

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, &regval);
    if (gopNum== (muxValue & GOP_MUX0_MASK))    //enable mux0 to SC
    {
        *pbEnable = (regval & GOP_BIT4) == GOP_BIT4;
    }
    else if (gopNum== ((muxValue & GOP_MUX1_MASK)>>(GOP_MUX_SHIFT*1))) //enable mux1
    {
        *pbEnable = (regval & GOP_BIT6) == GOP_BIT6;
    }
    else if (gopNum== ((muxValue & GOP_MUX2_MASK)>>(GOP_MUX_SHIFT*2))) //enable mux2
    {
        *pbEnable = (regval & GOP_BIT8) == GOP_BIT8;
    }
    else if (gopNum== ((muxValue & GOP_MUX3_MASK)>>(GOP_MUX_SHIFT*3))) //enable mux3
    {
        *pbEnable = (regval & GOP_BIT12) == GOP_BIT12;
    }
    else
    {
        return GOP_FAIL;
    }
    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPHighPri(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum)
{
    MS_U16 MuxGop, muxValue=0, i;
    MS_U16 MuxShift;

    MuxShift = GOP_MUX_SHIFT * E_GOP_MUX1;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MUX, &muxValue);
    for (i=0; i<MAX_GOP_MUX;i++)
    {
        if (gopNum== ((muxValue&(GOP_REGMUX_MASK<<(i*GOP_MUX_SHIFT)))>>(i*GOP_MUX_SHIFT)))
        {
            MuxGop = (muxValue&GOP_MUX1_MASK)>> MuxShift; //save mux1 gop

            muxValue &= ~GOP_MUX1_MASK; //clear mux1 setting
            muxValue &= ~(GOP_REGMUX_MASK<<(i*GOP_MUX_SHIFT));  //clear current mux setting
            muxValue |= ((gopNum<< MuxShift)|(MuxGop<<(i*GOP_MUX_SHIFT)));
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MUX, muxValue, GOP_REG_WORD_MASK);
            break;
        }
    }

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetGOPClk(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, DRV_GOPDstType eDstType)
{
    /* GOP dst type:
    0: IP (Main)
    1: IP (Sub)
    2: OP
    3: MVOP
    */

    switch(eDstType)
    {
    case E_DRV_GOP_DST_IP0:

        if (gopNum==1)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK2, CKG_GOPG1_MASK);
        }
        else if (gopNum ==2)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_IDCLK2, CKG_GOPG2_MASK);
        }
        else if (gopNum ==3)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, CKG_GOPG3_IDCLK2, CKG_GOPG2_MASK);
        }
        else if (gopNum==0)
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
        if (gopNum==1)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK1, CKG_GOPG1_MASK);
        }
        else if (gopNum==2)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_IDCLK1, CKG_GOPG2_MASK);
        }
        else if (gopNum==3)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, CKG_GOPG3_IDCLK1, CKG_GOPG2_MASK);
        }
        else if (gopNum==0)
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
        if (gopNum==1)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_ODCLK, CKG_GOPG1_MASK);
        }
        else if (gopNum==2)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, CKG_GOPG2_ODCLK, CKG_GOPG2_MASK);
        }
        else if (gopNum==3)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, CKG_GOPG3_ODCLK, CKG_GOPG2_MASK);
        }
        else if (gopNum==0)
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
        if (gopNum==1)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG1_IDCLK2, CKG_GOPG1_MASK);
        }
        else if (gopNum ==2)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG2_IDCLK2, CKG_GOPG2_MASK);
        }
        else if (gopNum ==3)
        {
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, CKG_GOPG3_IDCLK2, CKG_GOPG2_MASK);
        }
        else if (gopNum==0)
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

GOP_Result HAL_ConvertAPIAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gwinid, MS_U32* u32Adr)
{
    MS_U16 u16RegVal=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal);
    if (gwinid<MAX_GOP0_GWIN)   //gop0
    {
        if ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP0))==BIT(GOP_MIU_CLIENT_GOP0)) //MIU1
           * u32Adr = (* u32Adr|HAL_MIU1_BASE);
    }
    else if (gwinid>=MAX_GOP0_GWIN && gwinid<MAX_GOP0_GWIN+MAX_GOP1_GWIN)   //gop1
    {
        if ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP1))==BIT(GOP_MIU_CLIENT_GOP1)) //MIU1
            * u32Adr = (* u32Adr|HAL_MIU1_BASE);
    }
    else if (gwinid==(MAX_GOP0_GWIN+MAX_GOP1_GWIN))  //gop2
    {
         if ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP2))==BIT(GOP_MIU_CLIENT_GOP2)) //MIU1
            * u32Adr = (* u32Adr|HAL_MIU1_BASE);
    }
    else if (gwinid==(MAX_GOP0_GWIN+MAX_GOP1_GWIN+MAX_GOP2_GWIN))  //gop3
    {
         if ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP3))==BIT(GOP_MIU_CLIENT_GOP3)) //MIU1
            * u32Adr = (* u32Adr|HAL_MIU1_BASE);
    }
    else
    {
        return GOP_FAIL;
    }
    return GOP_SUCCESS;
}
MS_U8 HAL_GOP_GetMIUDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopnum)
{
    MS_U16 u16RegVal=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal);

    switch (gopnum)
    {
        case 0:
                return ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP0))==BIT(GOP_MIU_CLIENT_GOP0)? MIU_1:MIU_0);
                break;

        case 1:
                return ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP1))==BIT(GOP_MIU_CLIENT_GOP1)? MIU_1:MIU_0);
                break;

        case 2:
                return ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP2))==BIT(GOP_MIU_CLIENT_GOP2)? MIU_1:MIU_0);
                break;

        case 3:
                return ((u16RegVal&BIT(GOP_MIU_CLIENT_GOP3))==BIT(GOP_MIU_CLIENT_GOP3)? MIU_1:MIU_0);
                break;
        default:
                return 0xff;
                break;
    }
}
GOP_Result HAL_GOP_GetGOPDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8gopNum, DRV_GOPDstType *pGopDst)
{
    MS_U16 u16Regval=0;
    MS_U32 u32pBankOffSet=0;
    GOP_Result ret;

    _GetBnkOfstByGop(u8gopNum, &u32pBankOffSet);
    HAL_GOP_Read16Reg(pGOPHalLocal, u32pBankOffSet + GOP_4G_CTRL1, &u16Regval);

    switch (u16Regval&0x3)
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
        case 3:
            *pGopDst = E_DRV_GOP_DST_VOP;
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

/*
   mustang IP blending
   [15:8]         IP Main blending
   [7:0]          IP Sub blending
===========================================
   [10:8]/[2:0]   No use
   [11]/[3]       0: Enable IP blending
                  1: Disable IP blending
   [13:12]/[5:4]  2'b01 : Select GOP ips mux as source
                  2'b10 : Select GOP ipm mux as source
   [15:14]/[7:6]  2'b00, 2'b01 : Select SUB timing for GOP
                  2'b10, 2'b11 : Select Main timing for GOP
*/
    switch(ipSelGop)
    {
        case MS_DRV_IP0_SEL_GOP0:
            u16RegVal = GOP_BIT15 ;
            if(E_GOP0 == (muxValue &GOP_MUX0_MASK))
                u16RegVal |= IPMUX0_BLENDING_ENABLE;//mux0
            else
                u16RegVal |= IPMUX1_BLENDING_ENABLE;//mux1
            u16RegVal |= GOP_BIT3; //Disable Sub IP blending
            u16RegMsk = GOP_REG_WORD_MASK;
            break;
        case MS_DRV_IP0_SEL_GOP1:
            u16RegVal = GOP_BIT15 ;
            if(E_GOP1 == (muxValue &GOP_MUX0_MASK))
                u16RegVal |= IPMUX0_BLENDING_ENABLE;//mux0
            else
                u16RegVal |= IPMUX1_BLENDING_ENABLE;//mux1
            u16RegVal |= GOP_BIT3; //Disable Sub IP blending
            u16RegMsk = GOP_REG_WORD_MASK;
            break;
        case MS_DRV_IP0_SEL_GOP2:
            u16RegVal = GOP_BIT15 ;
            if(E_GOP2 == (muxValue &GOP_MUX0_MASK))
                u16RegVal |= IPMUX0_BLENDING_ENABLE;//mux0
            else
                u16RegVal |= IPMUX1_BLENDING_ENABLE;//mux1
            u16RegVal |= GOP_BIT3; //Disable Sub IP blending
            u16RegMsk = GOP_REG_WORD_MASK;
            break;
        case MS_DRV_NIP_SEL_GOP0:
            if(E_GOP0 == (muxValue &GOP_MUX0_MASK))
            {
                u16RegVal = ~IPMUX0_BLENDING_ENABLE;//mux0
                u16RegMsk = IPMUX0_BLENDING_ENABLE;
            }
            else
            {
                u16RegVal = ~IPMUX1_BLENDING_ENABLE;//mux1
                u16RegMsk = IPMUX1_BLENDING_ENABLE;
            }
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
            break;
        case MS_DRV_NIP_SEL_GOP1:
            if(E_GOP1 == (muxValue &GOP_MUX0_MASK))
            {
                u16RegVal = ~IPMUX0_BLENDING_ENABLE;//mux0
                u16RegMsk = IPMUX0_BLENDING_ENABLE;
            }
            else
            {
                u16RegVal = ~IPMUX1_BLENDING_ENABLE;//mux1
                u16RegMsk = IPMUX1_BLENDING_ENABLE;
            }
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
            break;
        case MS_DRV_NIP_SEL_GOP2:
            if(E_GOP2 == (muxValue &GOP_MUX0_MASK))
            {
                u16RegVal = ~IPMUX0_BLENDING_ENABLE;//mux0
                u16RegMsk = IPMUX0_BLENDING_ENABLE;
            }
            else
            {
                u16RegVal = ~IPMUX1_BLENDING_ENABLE;//mux1
                u16RegMsk = IPMUX1_BLENDING_ENABLE;
            }
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT11, GOP_BIT11);
            break;
        case MS_DRV_MVOP_SEL:
            u16RegVal = GOP_BIT15 ;
            u16RegVal |= IPMUX0_BLENDING_ENABLE; //mux0
            u16RegVal |= GOP_BIT3; //Disable Sub IP blending
            u16RegMsk = GOP_REG_WORD_MASK;
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
    if (enSrcSel==GOP_DRV_DWIN_SRC_OP)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal,  GOP_DW_CTL0_EN, 0, (GOP_BIT8|GOP_BIT9));
    }
    else if (enSrcSel==GOP_DRV_DWIN_SRC_MVOP)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal,  GOP_DW_CTL0_EN, (GOP_BIT8|GOP_BIT9), (GOP_BIT8|GOP_BIT9));
    }
    else if (enSrcSel==GOP_DRV_DWIN_SRC_IP)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal,  GOP_DW_CTL0_EN, (GOP_BIT8), (GOP_BIT8|GOP_BIT9));
         //enable scaler IP.
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, GOP_BIT12, GOP_BIT12|GOP_BIT13);
        // set GOPD to scaler ip OUT
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_IP2GOP_SRCSEL, GOP_BIT15, GOP_BIT15);
    }
    else
    {
        return GOP_INVALID_PARAMETERS;
    }

    return GOP_SUCCESS;
}


MS_U8 HAL_GOP_GetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal)
{
    MS_U16 u16RegVal=0;
    MS_U16 mask_shift=0;


    if (GOP_MIU_CLIENT_DWIN >=0x10)
    {
        MS_CRITICAL_MSG(printf(" %s :DWIN is not support\n",__FUNCTION__));
        return GOP_FAIL;
    }
    else
        mask_shift = GOP_MIU_CLIENT_DWIN;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_MIU_GROUP1, &u16RegVal);
    return ((u16RegVal&(1<<mask_shift))? MIU_1:MIU_0);

}
GOP_Result HAL_GOP_SetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 miu)
{
    MS_U16 mask_shift=0;

    if ( (miu>2) || (GOP_MIU_CLIENT_DWIN >= 0x10))
    {
        MS_CRITICAL_MSG(printf(" %s :DWIN is not support\n",__FUNCTION__));
        return GOP_FAIL;
    }
    else
        mask_shift = GOP_MIU_CLIENT_DWIN;

    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_MIU_GROUP1, miu<<mask_shift, 1<<mask_shift);

    return GOP_SUCCESS;


}

GOP_Result HAL_GOP_DWIN_EnableR2YCSC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEnable)
{
    if (bEnable)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_DW_ALPHA, GOP_BIT6, GOP_BIT6);
    }
    else
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_DW_ALPHA, ~GOP_BIT6, GOP_BIT6);
    }
    return GOP_SUCCESS;
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

GOP_Result HAL_GOP_GWIN_SetNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable)
{
    return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_GWiN_Set3DOSD_Sub(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP ,MS_U8 u8Gwin, MS_U32 u32SubAddr)
{
    u32SubAddr /= GOP_WordUnit;

    if(u8GOP == E_GOP2)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_3DOSD_SUB_RBLK_L, u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_1G_3DOSD_SUB_RBLK_H, u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else if(u8GOP == E_GOP1)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_3DOSD_SUB_RBLK_L(u8Gwin - MAX_GOP0_GWIN), u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_2G_3DOSD_SUB_RBLK_H(u8Gwin - MAX_GOP0_GWIN), u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else if(u8GOP == E_GOP0)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_3DOSD_SUB_RBLK_L(u8Gwin), u32SubAddr&GOP_REG_WORD_MASK ,GOP_REG_WORD_MASK );
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_4G_3DOSD_SUB_RBLK_H(u8Gwin), u32SubAddr>>16 ,GOP_REG_WORD_MASK );
    }
    else
    {
        return GOP_FUN_NOT_SUPPORTED;
    }

    return GOP_SUCCESS;

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
   return GOP_FUN_NOT_SUPPORTED;
}

GOP_Result HAL_GOP_OC_SetOCEn(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bOCEn)
{
   return GOP_FUN_NOT_SUPPORTED;
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
    switch(u8GopNum)
    {
        case E_GOP0:
            return GOP_FUN_NOT_SUPPORTED;
            break;
        case E_GOP1:
            return GOP_SUCCESS;
            break;
        case E_GOP2:
            return GOP_FUN_NOT_SUPPORTED;
            break;
        case E_GOP3:
            return GOP_FUN_NOT_SUPPORTED;
            break;
        default:
            printf("[%s] ERROR GOP Num %d\n",__FUNCTION__,u8GopNum);
            return GOP_FAIL;
    }
    return GOP_FAIL;

}


/********************************************************************************/
///GOP Scaling down (internal)
/********************************************************************************/

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

GOP_Result HAL_GOP_SetTLBAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_U32 u32tlbaddr, MS_U32 u32size)
{
    MS_U32 u32BankOffSet=0xFFFF;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_TAG_ADDR_L, u32size&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_TAG_ADDR_H, u32size>>16, GOP_REG_WORD_MASK);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_L, u32tlbaddr&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_H, u32tlbaddr>>16, GOP_REG_WORD_MASK);

    return GOP_SUCCESS;
}

GOP_Result HAL_GOP_SetTLBSubAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_U32 u32tlbaddr)
{
    MS_U32 u32BankOffSet=0xFFFF;
    _GetBnkOfstByGop(u8GOP, &u32BankOffSet);

    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_L, u32tlbaddr&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
    HAL_GOP_Write16Reg(pGOPHalLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_H, u32tlbaddr>>16, GOP_REG_WORD_MASK);

    return GOP_SUCCESS;
}

GOP_Result  HAL_GOP_DumpGOPReg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16* u16Val)
{
    MS_U32 u32BankOffSet=0;
    _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);

    if (u32GopIdx < MAX_GOP_SUPPORT)
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, (u32BankOffSet+ (u16BankIdx<<16) + (u16Addr<<1) +GOP_4G_CTRL0), u16Val);
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
        HAL_GOP_Write16Reg(pGOPHalLocal, (u32BankOffSet+ (u16BankIdx<<16) + (u16Addr<<1) +GOP_4G_CTRL0), u16Val, GOP_REG_WORD_MASK);
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
            if(g_GopChipPro.TotalGwinNum > 4)
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GOP4CLK, &(pGOP_STRPrivate->CKG_GopReg[3]));


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
            HAL_GOP_Read16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, &(pGOP_STRPrivate->XC_GopReg[11]));
        }
            break;
        case E_POWER_RESUME:
        {
            //CLK
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOPCLK, pGOP_STRPrivate->CKG_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP2CLK, pGOP_STRPrivate->CKG_GopReg[1], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP3CLK, pGOP_STRPrivate->CKG_GopReg[2], GOP_REG_WORD_MASK);
            if(g_GopChipPro.TotalGwinNum > 4)
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_GOP4CLK, pGOP_STRPrivate->CKG_GopReg[3], GOP_REG_WORD_MASK);

            //SRAM
            HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_SCALING, pGOP_STRPrivate->GS_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, CKG_GOPG0_MG, pGOP_STRPrivate->GS_GopReg[1], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, pGOP_STRPrivate->GS_GopReg[2], GOP_REG_WORD_MASK);

            //XC
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_BANKSEL, pGOP_STRPrivate->XC_GopReg[0], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_CHANNELSYNC, pGOP_STRPrivate->XC_GopReg[1], GOP_BIT11);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPEN, pGOP_STRPrivate->XC_GopReg[2], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_IP_SYNC, pGOP_STRPrivate->XC_GopReg[3], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_IP2GOP_SRCSEL, pGOP_STRPrivate->XC_GopReg[4], GOP_BIT15);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OSD_CHECK_ALPHA, pGOP_STRPrivate->XC_GopReg[5], GOP_BIT6);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_VOPNBL, pGOP_STRPrivate->XC_GopReg[6], GOP_BIT5);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPENMODE1, pGOP_STRPrivate->XC_GopReg[7], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_MIRRORCFG, pGOP_STRPrivate->XC_GopReg[8], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCMIXER, pGOP_STRPrivate->XC_GopReg[9], GOP_REG_WORD_MASK);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_OCMISC, pGOP_STRPrivate->XC_GopReg[10], GOP_BIT2);
            HAL_GOP_Write16Reg(pGOPHalLocal, GOP_SC_GOPSC_SRAM_CTRL, pGOP_STRPrivate->XC_GopReg[11], GOP_REG_WORD_MASK);
        }
            break;
        default:
            break;
    }
    return GOP_SUCCESS;
}

