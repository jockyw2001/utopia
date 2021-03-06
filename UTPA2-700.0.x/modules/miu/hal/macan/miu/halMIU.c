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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
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
#include "MsTypes.h"
#include "drvMIU.h"
#include "regMIU.h"
#include "halMIU.h"
#include "halCHIP.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MIU_HAL_ERR(x, args...)        {printf(x, ##args);}
#define HAL_MIU_SSC_DBG(x)             // x

#define MIU_CLIENT_GP0  \
    MIU_CLIENT_NONE, \
    MIU_CLIENT_VIVALDI9_DECODER_R, \
    MIU_CLIENT_SECAU_R2_RW, \
    MIU_CLIENT_USB_UHC4_RW,\
    MIU_CLIENT_SECURE_R2_RW,\
    MIU_CLIENT_AU_R2_RW, \
    MIU_CLIENT_VD_R2D_RW,\
    MIU_CLIENT_PM51_RW, \
    MIU_CLIENT_VD_R2I_R, \
    MIU_CLIENT_USB_UHC0_RW, \
    MIU_CLIENT_USB_UHC1_RW, \
    MIU_CLIENT_USB_UHC2_RW, \
    MIU_CLIENT_MVD_BBU_RW, \
    MIU_CLIENT_EMAC_RW, \
    MIU_CLIENT_BDMA_RW, \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP1  \
    MIU_CLIENT_VIVALDI9_MAD_RW, \
    MIU_CLIENT_DEMOD_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_JPD720P_RW, \
    MIU_CLIENT_VE_RW, \
    MIU_CLIENT_SC_DIPW_RW,\
    MIU_CLIENT_SC_LOCALDIMING_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_TSP_ORZ_W, \
    MIU_CLIENT_TSP_ORZ_R, \
    MIU_CLIENT_USB_UHC3_RW, \
    MIU_CLIENT_VD_TTXSL_W, \
    MIU_CLIENT_VD_COMB_W, \
    MIU_CLIENT_VD_COMB_R,  \
    MIU_CLIENT_ZDEC_RW, \
    MIU_CLIENT_ZDEC_ACP_RW

#define MIU_CLIENT_GP2  \
    MIU_CLIENT_CMD_QUEUE_RW, \
    MIU_CLIENT_GE_RW, \
    MIU_CLIENT_MIIC0_RW, \
    MIU_CLIENT_UART_DMA_RW, \
    MIU_CLIENT_MVD_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_GPD_RW, \
    MIU_CLIENT_MFE0_W, \
    MIU_CLIENT_MFE1_R, \
    MIU_CLIENT_NAND_RW, \
    MIU_CLIENT_SDIO_RW, \
    MIU_CLIENT_DSCRMB_RW, \
    MIU_CLIENT_TSP_FIQ_RW, \
    MIU_CLIENT_TSP_ORZ_W, \
    MIU_CLIENT_TSP_ORZ_R,\
    MIU_CLIENT_TSO_RW

#define MIU_CLIENT_GP3  \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP4  \
    MIU_CLIENT_HVD_BBU_R, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_HVD_RW, \
    MIU_CLIENT_SECHVD_RW, \
    MIU_CLIENT_EVD_RW,\
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_MVD_RTO_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP5  \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU_CLIENT_GP6  \
    MIU_CLIENT_SC_IPMAIN_RW, \
    MIU_CLIENT_SC_OPMAIN_RW, \
    MIU_CLIENT_MVOP_128BIT_R, \
    MIU_CLIENT_MFDEC_R, \
    MIU_CLIENT_SECMFDEC_R, \
    MIU_CLIENT_GOP0_R, \
    MIU_CLIENT_GOP1_R, \
    MIU_CLIENT_GOP2_R, \
    MIU_CLIENT_GOP3_PDW0_RW, \
    MIU_CLIENT_SC_PDW_W, \
    MIU_CLIENT_SC_IPSUB_RW, \
    MIU_CLIENT_SC_DIPW_RW, \
    MIU_CLIENT_SC2_OPMAIN_RW, \
    MIU_CLIENT_SC_OD_RW, \
    MIU_CLIENT_SC2_IPMAIN_RW, \
    MIU_CLIENT_SC_IPMAIN2_RW 

#define MIU_CLIENT_GP7  \
    MIU_CLIENT_MIPS_RW, \
    MIU_CLIENT_G3D_RW

#define MIU1_CLIENT_GP0  \
    MIU_CLIENT_NONE, \
    MIU_CLIENT_VIVALDI9_DECODER_R, \
    MIU_CLIENT_SECAU_R2_RW, \
    MIU_CLIENT_USB_UHC4_RW,\
    MIU_CLIENT_SECURE_R2_RW,\
    MIU_CLIENT_AU_R2_RW, \
    MIU_CLIENT_VD_R2D_RW,\
    MIU_CLIENT_PM51_RW, \
    MIU_CLIENT_VD_R2I_R, \
    MIU_CLIENT_USB_UHC0_RW, \
    MIU_CLIENT_USB_UHC1_RW, \
    MIU_CLIENT_USB_UHC2_RW, \
    MIU_CLIENT_MVD_BBU_RW, \
    MIU_CLIENT_EMAC_RW, \
    MIU_CLIENT_BDMA_RW, \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP1  \
    MIU_CLIENT_VIVALDI9_MAD_RW, \
    MIU_CLIENT_DEMOD_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_JPD720P_RW, \
    MIU_CLIENT_VE_RW, \
    MIU_CLIENT_SC_DIPW_RW,\
    MIU_CLIENT_SC_LOCALDIMING_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_TSP_ORZ_W, \
    MIU_CLIENT_TSP_ORZ_R, \
    MIU_CLIENT_USB_UHC3_RW, \
    MIU_CLIENT_VD_TTXSL_W, \
    MIU_CLIENT_VD_COMB_W, \
    MIU_CLIENT_VD_COMB_R,  \
    MIU_CLIENT_ZDEC_RW, \
    MIU_CLIENT_ZDEC_ACP_RW

#define MIU1_CLIENT_GP2  \
    MIU_CLIENT_CMD_QUEUE_RW, \
    MIU_CLIENT_GE_RW, \
    MIU_CLIENT_MIIC0_RW, \
    MIU_CLIENT_UART_DMA_RW, \
    MIU_CLIENT_MVD_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_GPD_RW, \
    MIU_CLIENT_MFE0_W, \
    MIU_CLIENT_MFE1_R, \
    MIU_CLIENT_NAND_RW, \
    MIU_CLIENT_SDIO_RW, \
    MIU_CLIENT_DSCRMB_RW, \
    MIU_CLIENT_TSP_FIQ_RW, \
    MIU_CLIENT_TSP_ORZ_W, \
    MIU_CLIENT_TSP_ORZ_R,\
    MIU_CLIENT_TSO_RW

#define MIU1_CLIENT_GP3  \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP4  \
    MIU_CLIENT_HVD_BBU_R, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_HVD_RW, \
    MIU_CLIENT_SECHVD_RW, \
    MIU_CLIENT_EVD_RW,\
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_MVD_RTO_RW, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP5  \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY, \
    MIU_CLIENT_DUMMY

#define MIU1_CLIENT_GP6  \
    MIU_CLIENT_SC_IPMAIN_RW, \
    MIU_CLIENT_SC_OPMAIN_RW, \
    MIU_CLIENT_MVOP_128BIT_R, \
    MIU_CLIENT_MFDEC_R, \
    MIU_CLIENT_SECMFDEC_R, \
    MIU_CLIENT_GOP0_R, \
    MIU_CLIENT_GOP1_R, \
    MIU_CLIENT_GOP2_R, \
    MIU_CLIENT_GOP3_PDW0_RW, \
    MIU_CLIENT_SC_PDW_W, \
    MIU_CLIENT_SC_IPSUB_RW, \
    MIU_CLIENT_SC_DIPW_RW, \
    MIU_CLIENT_SC2_OPMAIN_RW, \
    MIU_CLIENT_SC_OD_RW, \
    MIU_CLIENT_SC2_IPMAIN_RW, \
    MIU_CLIENT_SC_IPMAIN2_RW 

#define MIU1_CLIENT_GP7  \
    MIU_CLIENT_MIPS_RW, \
    MIU_CLIENT_G3D_RW

#define KHz                 (1000UL)
#define MHz                 (1000000UL)
#define MPPL                (432)
#define DDR_FACTOR          (524288)
#define DDFSPAN_FACTOR      (131072)
#define IDNUM_KERNELPROTECT (8)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
const eMIUClientID clientTbl[MIU_MAX_DEVICE][MIU_MAX_TBL_CLIENT] =
{
    {MIU_CLIENT_GP0, MIU_CLIENT_GP1, MIU_CLIENT_GP2, MIU_CLIENT_GP3, MIU_CLIENT_GP4, MIU_CLIENT_GP5, MIU_CLIENT_GP6, MIU_CLIENT_GP7}
    ,{MIU1_CLIENT_GP0, MIU1_CLIENT_GP1, MIU1_CLIENT_GP2, MIU1_CLIENT_GP3, MIU1_CLIENT_GP4, MIU1_CLIENT_GP5, MIU1_CLIENT_GP6, MIU1_CLIENT_GP7}
};

MS_U8 clientId_KernelProtect[IDNUM_KERNELPROTECT] =
{
    MIU_CLIENT_MIPS_RW, 
    MIU_CLIENT_NAND_RW, 
    MIU_CLIENT_USB_UHC0_RW, 
    MIU_CLIENT_USB_UHC1_RW,
    MIU_CLIENT_USB_UHC2_RW, 
    MIU_CLIENT_NONE, 
    MIU_CLIENT_NONE, 
    MIU_CLIENT_NONE
};
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIU_MapBase = 0xBF200000;      //default set to MIPS platfrom
static MS_VIRT _gPM_MapBase = 0xBF000000;      //default set to MIPS platfrom

MS_BOOL IDEnables[MIU_MAX_DEVICE][MIU_MAX_PROTECT_BLOCK][MIU_MAX_PROTECT_ID] = {{{0},{0},{0},{0}}, {{0},{0},{0},{0}}}; //ID enable for protect block 0~3
MS_U8 IDs[MIU_MAX_DEVICE][MIU_MAX_PROTECT_ID] = {{0}, {0}}; //IDs for protection
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_MIU_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIU_MapBase = virtBase;
    HAL_MIU_SSC_DBG(printf("MIU _gMIU_MapBase= %lx\n", _gMIU_MapBase));
}

void HAL_MIU_SetPMIOMapBase(MS_VIRT virtBase)
{
    _gPM_MapBase = virtBase;
    HAL_MIU_SSC_DBG(printf("MIU _gPM_MapBase= %lx\n", _gPM_MapBase));
}

MS_S16 HAL_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID)
{
    MS_U8 idx;

    if (MIU_MAX_DEVICE <= u8MiuDev)
    {
        printf("Wrong MIU device:%u\n", u8MiuDev);
        return (-1);
    }

    for (idx = 0; idx < MIU_MAX_TBL_CLIENT; idx++)
        if (eClientID == clientTbl[u8MiuDev][idx])
            return idx;
    return (-1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MIU_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIU_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_PM_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MIU_PM_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gPM_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MIU_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIU_MapBase))[u32RegAddr];
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MIU_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_MIU_Read2Byte(u32RegAddr) | HAL_MIU_Read2Byte(u32RegAddr+2) << 16);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIU_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIU_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_MIU_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_MIU_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = HAL_MIU_ReadByte(u32RegAddr);
    if (!u32RegAddr)
    {
        MIU_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_MIU_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_MIU_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

void HAL_MIU_Write2BytesBit(MS_U32 u32RegOffset, MS_BOOL bEnable, MS_U16 u16Mask)
{
    MS_U16 val = HAL_MIU_Read2Byte(u32RegOffset);
    val = (bEnable) ? (val | u16Mask) : (val & ~u16Mask);
    HAL_MIU_Write2Byte(u32RegOffset, val);
}

void HAL_MIU_SetProtectID(MS_U32 u32Reg, MS_U8 u8MiuDev, MS_U8 u8ClientID)
{
    MS_S16 sVal = HAL_MIU_GetClientInfo(u8MiuDev, (eMIUClientID)u8ClientID);
    MS_S16 sIDVal;

    if (0 > sVal)
        sVal = 0;

    sIDVal = HAL_MIU_ReadByte(u32Reg);
    sIDVal &= 0x80;
    sIDVal |= sVal;
    HAL_MIU_WriteByte(u32Reg, sIDVal);

}

MS_BOOL HAL_MIU_SetGroupID(MS_U8 u8MiuSel, MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32RegAddrID, MS_U32 u32RegAddrIDenable)
{
    MS_U32 u32index0, u32index1;
    MS_U8 u8ID;
    MS_U8 u8isfound0, u8isfound1;
    MS_U16 u16idenable;
   
    //reset IDenables for protect u8Blockx
    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        IDEnables[u8MiuSel][u8Blockx][u32index0] = 0;
    }

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        u8ID = pu8ProtectId[u32index0];

        //Unused ID
        if(u8ID == 0)  
           continue;

        u8isfound0 = FALSE;

        for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_ID; u32index1++)
        {
            if(IDs[u8MiuSel][u32index1] == u8ID)
            {
                //ID reused former setting
                IDEnables[u8MiuSel][u8Blockx][u32index1] = 1;
                u8isfound0 = TRUE;
                break;
            }
        }
        

        //Need to create new ID in IDs 
        if(u8isfound0 != TRUE)
        {
            u8isfound1 = FALSE;

            for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_ID; u32index1++)
            {
                if(IDs[u8MiuSel][u32index1] == 0)
                {
                    IDs[u8MiuSel][u32index1] = u8ID;
                    IDEnables[u8MiuSel][u8Blockx][u32index1] = 1;
                    u8isfound1 = TRUE;
                    break;
                }
            }
             
            //ID overflow
            if(u8isfound1 == FALSE)
                return FALSE;
        }
    }

    u16idenable = 0;

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        if(IDEnables[u8MiuSel][u8Blockx][u32index0] == 1)
            u16idenable |= (1<<u32index0);
    }

    HAL_MIU_Write2Byte(u32RegAddrIDenable, u16idenable);

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
         HAL_MIU_SetProtectID(u32RegAddrID + u32index0, u8MiuSel, IDs[u8MiuSel][u32index0]);
    } 

    return TRUE;
}

MS_BOOL HAL_MIU_ResetGroupID(MS_U8 u8MiuSel, MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32RegAddrID, MS_U32 u32RegAddrIDenable)
{
    MS_U32 u32index0, u32index1;
    MS_U8 u8isIDNoUse;
    MS_U16 u16idenable;
   
    //reset IDenables for protect u8Blockx
    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        IDEnables[u8MiuSel][u8Blockx][u32index0] = 0;
    }

    u16idenable = 0x0;

    HAL_MIU_Write2Byte(u32RegAddrIDenable, u16idenable);

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
        u8isIDNoUse  = FALSE;

        for(u32index1 = 0; u32index1 < MIU_MAX_PROTECT_BLOCK; u32index1++)
        {
            if(IDEnables[u8MiuSel][u32index1][u32index0] == 1)
            {
                //protect ID is still be used
                u8isIDNoUse  = FALSE;
                break;
            }
            u8isIDNoUse  = TRUE;
        }

        if(u8isIDNoUse == TRUE)
            IDs[u8MiuSel][u32index0] = 0;
    }

    for(u32index0 = 0; u32index0 < MIU_MAX_PROTECT_ID; u32index0++)
    {
         HAL_MIU_SetProtectID(u32RegAddrID + u32index0, u8MiuSel, IDs[u8MiuSel][u32index0]);
    } 

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_GetDefaultClientID_KernelProtect()
/// @brief \b Function \b Description:  Get default client id array pointer for protect kernel
/// @param <RET>           \b     : The pointer of Array of client IDs
////////////////////////////////////////////////////////////////////////////////
MS_U8* HAL_MIU_GetDefaultClientID_KernelProtect()
{
     if(IDNUM_KERNELPROTECT > 0)
         return  (MS_U8 *)&clientId_KernelProtect[0];   

     return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function    \b Name: HAL_MIU_ProtectAlign()
/// @brief \b Function    \b Description:  Get the page shift for MIU protect
/// @param <*u32PageShift>\b IN: Page shift 
/// @param <RET>          \b OUT: None 
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MIU_ProtectAlign(void)
{    
    MS_U32 u32PageShift;

    u32PageShift = MIU_PAGE_SHIFT;
    return u32PageShift;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Dram_Size()
/// @brief \b Function \b Description: Set up Dram size for MIU protect 
/// @param MiuID        \b IN     : MIU ID
/// @param DramSize     \b IN     : Specified Dram size for MIU protect
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize)
{
    MS_U32 u32RegAddr;
    

    if(E_MIU_2 == (MIU_ID)MiuID)
    {
        u32RegAddr = MIU2_PROTECT_DDR_SIZE;
        switch (DramSize)
        {
            case E_MIU_DDR_32MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_32MB);
                break;
            case E_MIU_DDR_64MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_64MB);
                break;
            case E_MIU_DDR_128MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_128MB);
                break;
            case E_MIU_DDR_256MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_256MB);
                break;
            case E_MIU_DDR_512MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_512MB);
                break;
            case E_MIU_DDR_1024MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_1024MB);
                break;
            default:
                return false;
        }
    }
    else if(E_MIU_1 == (MIU_ID)MiuID)
    {
        u32RegAddr = MIU1_PROTECT_DDR_SIZE;
        switch (DramSize)
        {
            case E_MIU_DDR_32MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_32MB);
                break;
            case E_MIU_DDR_64MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_64MB);
                break;
            case E_MIU_DDR_128MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_128MB);
                break;
            case E_MIU_DDR_256MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_256MB);
                break;
            case E_MIU_DDR_512MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_512MB);
                break;
            case E_MIU_DDR_1024MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_1024MB);
                break;
            default:
                return false;
        }
    }
    else if(E_MIU_0 == (MIU_ID)MiuID)
    {
        u32RegAddr = MIU_PROTECT_DDR_SIZE;
        switch (DramSize)
        {
            case E_MIU_DDR_32MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_32MB);
                break;
            case E_MIU_DDR_64MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_64MB);
                break;
            case E_MIU_DDR_128MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_128MB);
                break;
            case E_MIU_DDR_256MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_256MB);
                break;
            case E_MIU_DDR_512MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_512MB);
                break;
            case E_MIU_DDR_1024MB:
			    HAL_MIU_WriteByte(u32RegAddr, MIU_PROTECT_DDR_1024MB);
                break;
            default:
                return false;
        }
     }
     else
     {
 	     printf("%s not support MIU%u!\n", __FUNCTION__, MiuID );
 	      return FALSE;
     }

     return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Dram_ReadSize()
/// @brief \b Function \b Description: Set up Dram size for MIU protect 
/// @param MiuID        \b IN     : MIU ID
/// @param DramSize     \b IN     : Specified Dram size for MIU protect
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Dram_ReadSize(MS_U8 MiuID, MIU_DDR_SIZE *pDramSize)
{
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetClinetNumber()
/// @brief \b Function \b Description:  Get the number of clients for specific MIU block 
/// @param DramSize     \b IN     : MIU Block to protect (0 ~ 3)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MIU_ClinetNumber(MS_U8 u8Blockx)
{
    MS_U8 u8ClientNumber;
    
    switch (u8Blockx)
    {
        case E_MIU_BLOCK_0:
            u8ClientNumber = MIU_BLOCK0_CLIENT_NUMBER;
            break;
        case E_MIU_BLOCK_1:
            u8ClientNumber = MIU_BLOCK1_CLIENT_NUMBER;
            break;
        case E_MIU_BLOCK_2:
            u8ClientNumber = MIU_BLOCK2_CLIENT_NUMBER;
            break;
        case E_MIU_BLOCK_3:
            u8ClientNumber = MIU_BLOCK3_CLIENT_NUMBER;
            break;
        default:
            u8ClientNumber = 0;
    }

    return u8ClientNumber;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Protect()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 4)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param u32Start        \b IN     : Starting address
/// @param u32End          \b IN     : End address
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_Protect(
                          MS_U8    u8Blockx,
                          MS_U8    *pu8ProtectId,
                          MS_PHY phy64Start,
                          MS_PHY phy64End,
                          MS_BOOL  bSetFlag
                         )
{
    MS_U32 u32RegAddr;
    MS_U32 u32Reg;
    MS_U32 u32RegAddrStar;
    MS_U32 u32RegAddrMSB;
    MS_U32 u32RegAddrIDenable;
    MS_U32 u32MiuProtectEn;
    MS_PHY phy64StartOffset;
    MS_PHY phy64EndOffset;
    MS_U16 u16Data;
    MS_U16 u16Data1;
    MS_U16 u16Data2;    
    MS_U8  u8Data;
    MS_U8  u8MiuSel;

    // Get MIU selection and offset
    _phy_to_miu_offset(u8MiuSel, phy64EndOffset, phy64End)
    _phy_to_miu_offset(u8MiuSel, phy64StartOffset, phy64Start)
    
    phy64Start = phy64StartOffset;
    phy64End = phy64EndOffset;
    
    // Incorrect Block ID
    if(u8Blockx >= E_MIU_BLOCK_NUM)
    {
        MIU_HAL_ERR("Err: Out of the number of protect device\n")
        return false;
    }
    else if(((phy64Start & ((1 << MIU_PAGE_SHIFT) -1)) != 0) || ((phy64End & ((1 << MIU_PAGE_SHIFT) -1)) != 0))
    {
        MIU_HAL_ERR("Err: Protected address should be aligned to 8KB\n")
        return false;
    } 
    else if(phy64Start >= phy64End)
    {
        MIU_HAL_ERR("Err: Start address is equal to or more than end address\n")
        return false;
    }

    //
    // Check MIU SWAP, Efuse: 6F[8]
    //
    u16Data = HAL_MIU_PM_ReadByte(0x2015);

    if (u16Data & 0x01)
    {
        if(u8MiuSel == E_CHIP_MIU_0)
        {
            u8MiuSel = E_CHIP_MIU_1;
        }
        else if(u8MiuSel == E_CHIP_MIU_1)
        {
            u8MiuSel = E_CHIP_MIU_0;
        }
        else
        {
            MIU_HAL_ERR("Err: swap function only support on 2 MIUs model\n")
            return false;
        }
    }

    //write_enable
    u8Data = 1 << u8Blockx;
    if(u8MiuSel == E_CHIP_MIU_0)
    {
        u32RegAddrMSB = MIU_PROTECT0_MSB;
        u16Data1 = HAL_MIU_Read2Byte(u32RegAddrMSB);

        u32RegAddr = MIU_PROTECT0_ID0;
	    u32MiuProtectEn=MIU_PROTECT_EN;
        u32Reg = MIU_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddrStar = MIU_PROTECT0_START;
                u32RegAddrIDenable = MIU_PROTECT0_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFFF0);
                break;
            case E_MIU_BLOCK_1:
                u32RegAddrStar = MIU_PROTECT1_START;
                u32RegAddrIDenable = MIU_PROTECT1_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFF0F);
                break;
            case E_MIU_BLOCK_2:
                u32RegAddrStar = MIU_PROTECT2_START;
                u32RegAddrIDenable = MIU_PROTECT2_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xF0FF);
                break;
            case E_MIU_BLOCK_3:
                u32RegAddrStar = MIU_PROTECT3_START;
                u32RegAddrIDenable = MIU_PROTECT3_ID_ENABLE;
                u16Data2 = (u16Data1 & 0x0FFF);
                break;
            default:
		        return false;
        }
    }
    else if(u8MiuSel == E_CHIP_MIU_1)
    {
        u32RegAddrMSB = MIU1_PROTECT0_MSB;
        u16Data1 = HAL_MIU_Read2Byte(u32RegAddrMSB);

        u32RegAddr = MIU1_PROTECT0_ID0;
	    u32MiuProtectEn=MIU1_PROTECT_EN;
        u32Reg = MIU1_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddrStar = MIU1_PROTECT0_START;
                u32RegAddrIDenable = MIU1_PROTECT0_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFFF0);
                break;
            case E_MIU_BLOCK_1:
                u32RegAddrStar = MIU1_PROTECT1_START;
                u32RegAddrIDenable = MIU1_PROTECT1_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFF0F);
                break;
            case E_MIU_BLOCK_2:
                u32RegAddrStar = MIU1_PROTECT2_START;
                u32RegAddrIDenable = MIU1_PROTECT2_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xF0FF);
                break;
            case E_MIU_BLOCK_3:
                u32RegAddrStar = MIU1_PROTECT3_START;
                u32RegAddrIDenable = MIU1_PROTECT3_ID_ENABLE;
                u16Data2 = (u16Data1 & 0x0FFF);
                break;
            default:
		        return false;
        }
     }
    else if(u8MiuSel == E_CHIP_MIU_2)
    {
        u32RegAddrMSB = MIU2_PROTECT0_MSB;
        u16Data1 = HAL_MIU_Read2Byte(u32RegAddrMSB);

        u32RegAddr = MIU2_PROTECT0_ID0;
	    u32MiuProtectEn=MIU2_PROTECT_EN;
        u32Reg = MIU2_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddrStar = MIU2_PROTECT0_START;
                u32RegAddrIDenable = MIU2_PROTECT0_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFFF0);
                break;
            case E_MIU_BLOCK_1:
                u32RegAddrStar = MIU2_PROTECT1_START;
                u32RegAddrIDenable = MIU2_PROTECT1_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFF0F);
                break;
            case E_MIU_BLOCK_2:
                u32RegAddrStar = MIU2_PROTECT2_START;
                u32RegAddrIDenable = MIU2_PROTECT2_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xF0FF);
                break;
            case E_MIU_BLOCK_3:
                u32RegAddrStar = MIU2_PROTECT3_START;
                u32RegAddrIDenable = MIU2_PROTECT3_ID_ENABLE;
                u16Data2 = (u16Data1 & 0x0FFF);
                break;
            default:
		        return false;
        }
    }
    else if(u8MiuSel == E_CHIP_MIU_3)
    {
        u32RegAddrMSB = MIU3_PROTECT0_MSB;
        u16Data1 = HAL_MIU_Read2Byte(u32RegAddrMSB);

        u32RegAddr = MIU3_PROTECT0_ID0;
	    u32MiuProtectEn=MIU3_PROTECT_EN;
        u32Reg = MIU3_REG_BASE;

        switch (u8Blockx)
        {
            case E_MIU_BLOCK_0:
                u32RegAddrStar = MIU3_PROTECT0_START;
                u32RegAddrIDenable = MIU3_PROTECT0_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFFF0);
                break;
            case E_MIU_BLOCK_1:
                u32RegAddrStar = MIU3_PROTECT1_START;
                u32RegAddrIDenable = MIU3_PROTECT1_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xFF0F);
                break;
            case E_MIU_BLOCK_2:
                u32RegAddrStar = MIU3_PROTECT2_START;
                u32RegAddrIDenable = MIU3_PROTECT2_ID_ENABLE;
                u16Data2 = (u16Data1 & 0xF0FF);
                break;
            case E_MIU_BLOCK_3:
                u32RegAddrStar = MIU3_PROTECT3_START;
                u32RegAddrIDenable = MIU3_PROTECT3_ID_ENABLE;
                u16Data2 = (u16Data1 & 0x0FFF);
                break;
            default:
		        return false;
        }
	}
    else
    {
 	   printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuSel );
 	   return FALSE;
    }

    // Disable MIU protect
    HAL_MIU_WriteRegBit(u32MiuProtectEn,u8Data,DISABLE);

    if ( bSetFlag )
    {
        // Set Protect IDs
        if(HAL_MIU_SetGroupID(u8MiuSel, u8Blockx, pu8ProtectId, u32RegAddr, u32RegAddrIDenable) == FALSE)
        {
            return FALSE;
        }

        // Set BIT29,30 of start/end address
        u16Data2 = u16Data2 | (MS_U16)((phy64Start >> 29) << (u8Blockx*4));
        u16Data1 = u16Data2 | (MS_U16)(((phy64End - 1) >> 29) << (u8Blockx*4+2));
        HAL_MIU_Write2Byte(u32RegAddrMSB, u16Data1);

        // Start Address
        u16Data = (MS_U16)(phy64Start >> MIU_PAGE_SHIFT);   //8k/unit
        HAL_MIU_Write2Byte(u32RegAddrStar , u16Data);

        // End Address
        u16Data = (MS_U16)((phy64End >> MIU_PAGE_SHIFT)-1);   //8k/unit;
        HAL_MIU_Write2Byte(u32RegAddrStar + 2, u16Data);

        // Enable MIU protect
        HAL_MIU_WriteRegBit(u32MiuProtectEn, u8Data, ENABLE);
    }
    else
    {
        // Reset Protect IDs
        HAL_MIU_ResetGroupID(u8MiuSel, u8Blockx, pu8ProtectId, u32RegAddr, u32RegAddrIDenable);
    }

    // clear log
    HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, TRUE, REG_MIU_PROTECT_LOG_CLR);
    HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, FALSE, REG_MIU_PROTECT_LOG_CLR);

    return TRUE;
}

#define GET_HIT_BLOCK(regval)       BITS_RANGE_VAL(regval, REG_MIU_PROTECT_HIT_NO)
#define GET_HIT_CLIENT(regval)      BITS_RANGE_VAL(regval, REG_MIU_PROTECT_HIT_ID)

MS_BOOL HAL_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo)
{
    MS_U16 ret = 0;
    MS_U16 loaddr = 0;
    MS_U16 hiaddr = 0;
    MS_U32 u32Address = 0;
    MS_U32 u32Reg ;

    if(u8MiuDev == E_MIU_0)
    {
        u32Reg = MIU_REG_BASE;
    }
    else if(u8MiuDev == E_MIU_1)
    {
        u32Reg = MIU1_REG_BASE;
    }
    else if(u8MiuDev == E_MIU_2)
    {
        u32Reg = MIU2_REG_BASE;
    }
    else
    {
 	   printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuDev );
 	   return FALSE;
    }

    if (!pInfo)
        return FALSE;

    ret = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_STATUS);
    loaddr = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_LOADDR);
    hiaddr = HAL_MIU_Read2Byte(u32Reg+REG_MIU_PROTECT_HIADDR);

    pInfo->bHit = false;
    
    if (REG_MIU_PROTECT_HIT_FALG & ret)
    {
        pInfo->bHit = TRUE;

        pInfo->u8Block = (MS_U8)GET_HIT_BLOCK(ret);
        pInfo->u8Group = (MS_U8)(GET_HIT_CLIENT(ret) >> 4);
        pInfo->u8ClientID = (MS_U8)(GET_HIT_CLIENT(ret) & 0x0F);
        u32Address = (MS_U32)((hiaddr << 16) | loaddr) ;
        u32Address = u32Address * MIU_PROTECT_ADDRESS_UNIT;
        printf("MIU%u Block:%u Group:%u ClientID:%u Hitted_Address:0x%tX<->0x%tX\n", u8MiuDev,
        pInfo->u8Block, pInfo->u8Group, pInfo->u8ClientID, (ptrdiff_t)u32Address, (ptrdiff_t)(u32Address + MIU_PROTECT_ADDRESS_UNIT - 1));

        //clear log
        HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, TRUE, REG_MIU_PROTECT_LOG_CLR);
        HAL_MIU_Write2BytesBit(u32Reg+REG_MIU_PROTECT_STATUS, FALSE, REG_MIU_PROTECT_LOG_CLR);
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_SetSsc()
/// @brief \b Function \b Description: MDrv_MIU_SetSsc, @Step & Span
/// @param u16Fmodulation   \b IN : 20KHz ~ 40KHz (Input Value = 20 ~ 40)
/// @param u16FDeviation    \b IN  : under 0.1% ~ 2% (Input Value = 1 ~ 20)
/// @param bEnable          \b IN    :
/// @param None             \b OUT  :
/// @param None             \b RET  :
/// @param None             \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_SetSsc(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable)
{
    MS_U32 uDDFSET, uDDR_MHz, uDDFStep, uRegBase = MIU_ATOP_BASE;
    MS_U16 u16DDFSpan;
    MS_U16 u16Input_DIV_First,u16Input_DIV_Second,u16Loop_DIV_First,u16Loop_DIV_Second;
    MS_U8  u8Temp,i;

    //Pre check the input
    if(u8MiuDev == E_MIU_0)
    {
        uRegBase = MIU_ATOP_BASE;
    }
    else if(u8MiuDev == E_MIU_1)
    {
        uRegBase = MIU1_ATOP_BASE;
    }
    else if(u8MiuDev == E_MIU_2)
    {
        uRegBase = MIU2_ATOP_BASE;
    }
    else
    {
 	   printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuDev );
 	   return FALSE;
    }

    HAL_MIU_SSC_DBG(printf("MMIO base:%lx uRegBase:%lx\n", _gMIU_MapBase, uRegBase));

    if ((u16Fmodulation<20)||(u16Fmodulation>40))
    {
        MIU_HAL_ERR("SSC u16Fmodulation Error...(20KHz - 40KHz)\n");
        return 0;
    }

    if ((u16FDeviation<1)||(u16FDeviation>20))
    {
        MIU_HAL_ERR("SSC u16FDeviation Error...(0.1%% - 2%% ==> 1 ~20)\n");
        return 0;
    }

    HAL_MIU_SSC_DBG(printf("---> u16Fmodulation = %d u16FDeviation = %d \n",(int)u16Fmodulation,(int)u16FDeviation));
    //<1>.Caculate DDFM = (Loop_DIV_First * Loop_DIV_Second)/(Input_DIV_First * Input_DIV_Second);
    //Prepare Input_DIV_First
    u8Temp = ((MS_U16)(HAL_MIU_Read2Byte(uRegBase+MIU_DDRPLL_DIV_FIRST)&0x3));       //Bit 9,8 (0x110D36)
    u16Input_DIV_First = 0x01;
    for (i=0;i<u8Temp;i++)
        u16Input_DIV_First = u16Input_DIV_First << 1;
    //Prepare Input_DIV_Second
    u16Input_DIV_Second = 0;// no mapping in Einstein(HAL_MIU_ReadByte(uRegBase+MIU_PLL_INPUT_DIV_2ND));     //Bit 0~7 (0x101222)
    if (u16Input_DIV_Second == 0)
        u16Input_DIV_Second = 1;
    //Prepare Loop_DIV_First
    u8Temp = ((HAL_MIU_ReadByte(uRegBase+MIU_DDRPLL_DIV_FIRST)&0xC)>>2);         //Bit 11,10 (0x110D36)
    u16Loop_DIV_First = 0x01;
     for (i=0;i<u8Temp;i++)
        u16Loop_DIV_First = u16Loop_DIV_First << 1;

    //Prepare Loop_DIV_Second
    u16Loop_DIV_Second = (HAL_MIU_ReadByte(uRegBase+MIU_PLL_LOOP_DIV_2ND))&0x1F;      //Bit 0~4 (0x101223)
    if (u16Loop_DIV_Second == 0)
        u16Loop_DIV_Second = 1;

    //<2>.From DDFSET register to get DDRPLL
    uDDFSET = HAL_MIU_Read4Byte(uRegBase+MIU_DDFSET) & 0x00ffffff;
    //DDRPLL = MPPL * DDR_FACTOR * Loop_First * Loop_Second / DDFSET * Input_First * Input_Second
    HAL_MIU_SSC_DBG(printf("---> Loop_First:%u Loop_Second:%u\n", u16Loop_DIV_First, u16Loop_DIV_Second));
    HAL_MIU_SSC_DBG(printf("---> Input_first:%u Input_second:%u\n", u16Input_DIV_First, u16Input_DIV_Second));
    uDDR_MHz = (MPPL * DDR_FACTOR * u16Loop_DIV_First * u16Loop_DIV_Second)/ (uDDFSET*u16Input_DIV_First*u16Input_DIV_Second);
    HAL_MIU_SSC_DBG(printf("---> uDDFSET = 0x%lx\n",uDDFSET));
    HAL_MIU_SSC_DBG(printf("---> DDR_MHZ = 0x%lx (%d MHz)\n",uDDR_MHz,(int)uDDR_MHz));

    //<3>.Caculate DDFSPAN = (MPLL * DDFSPAN_FACTOR * MHz) / (DDFSET * Fmodulation * KHz)
    u16DDFSpan = (MS_U32)((DDFSPAN_FACTOR * MPPL/u16Fmodulation)* 1000/uDDFSET);
    HAL_MIU_SSC_DBG(printf("---> DDFSPAN = 0x%x (%d)\n",u16DDFSpan,(int)u16DDFSpan));
    if (u16DDFSpan > 0x3FFF)
    {
        u16DDFSpan = 0x3FFF;
        HAL_MIU_SSC_DBG(printf("??? DDFSPAN overflow > 0x3FFF, Fource set to 0x03FF\n"));
    }

    //Write to Register
    HAL_MIU_Write2Byte(uRegBase+MIU_DDFSPAN,u16DDFSpan);
    //<4>.Caculate DDFSTEP = (FDeviation*DDFSET/10)/(DDFSPAN*100)
    uDDFStep = (MS_U32)((u16FDeviation * (uDDFSET/10))/(u16DDFSpan*100));
    HAL_MIU_SSC_DBG(printf("---> DDFSTEP = 0x%lx (%lu)\n",uDDFStep,uDDFStep));
    //Write to Register
    uDDFStep &= (0x03FF);
    HAL_MIU_Write2Byte(uRegBase+MIU_DDFSTEP,(HAL_MIU_Read2Byte(uRegBase+MIU_DDFSTEP) & (~0x03FF))|uDDFStep);

    //<5>.Set ENABLE
    if(bEnable == ENABLE)
        HAL_MIU_WriteByte(uRegBase+MIU_SSC_EN,(HAL_MIU_ReadByte(uRegBase+MIU_SSC_EN)|0xC0));
    else
        HAL_MIU_WriteByte(uRegBase+MIU_SSC_EN,(HAL_MIU_ReadByte(uRegBase+MIU_SSC_EN)&(~0xC0))|0x80);
    
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_MaskReq()
/// @brief \b Function \b Description: Mask MIU request
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param eClientID IN     \b  : client ID
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    MS_S16 sVal;

    sVal = HAL_MIU_GetClientInfo(u8Miu, eClientID);
    if (sVal < 0)
        printf("%s not support client ID:%u!\n", __FUNCTION__, eClientID);
    else
    {
        MS_U32 u32Reg = REG_MIU_RQX_MASK(MIU_GET_CLIENT_GROUP(sVal));

        if(u8Miu == E_MIU_0)
        {
            u32Reg += MIU_REG_BASE;
        }
        else if(u8Miu == E_MIU_1)
        {
            u32Reg += MIU1_REG_BASE;
        }
        else if(u8Miu == E_MIU_2)
        {
            u32Reg += MIU2_REG_BASE;
        }
        else
        {
 	       printf("%s not support MIU%u!\n", __FUNCTION__, u8Miu );
 	       return;
        }

        HAL_MIU_Write2BytesBit(u32Reg, TRUE, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_UnMaskReq()
/// @brief \b Function \b Description: Mask MIU request
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param eClientID IN      \b  : client ID
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    MS_S16 sVal;

    sVal = HAL_MIU_GetClientInfo(u8Miu, eClientID);
    if (sVal < 0)
        printf("%s not support client ID:%u!\n", __FUNCTION__, eClientID);
    else
    {
        MS_U32 u32Reg = REG_MIU_RQX_MASK(MIU_GET_CLIENT_GROUP(sVal));

        if(u8Miu == E_MIU_0)
        {
            u32Reg += MIU_REG_BASE;
        }
        else if(u8Miu == E_MIU_1)
        {
            u32Reg += MIU1_REG_BASE;
        }
        else if(u8Miu == E_MIU_2)
        {
            u32Reg += MIU2_REG_BASE;
        }
        else
        {
            printf("%s not support MIU%u!\n", __FUNCTION__, u8Miu );
            return;
        }

        HAL_MIU_Write2BytesBit(u32Reg, FALSE, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_SelMIU()
/// @brief \b Function \b Description: MIU selection
/// @param u8MiuDev	   IN   \b  : miu device
/// @param u16ClientID IN   \b  : client ID
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_SelMIU(eMIU_SelType eType, eMIUClientID eClientID)
{
    MS_S16 sVal;
    MS_U32 u32Reg0 = 0;
    MS_U32 u32Reg1 = 0;
    MS_U32 u32Reg2 = 0;

    if(eType == MIU_SELTYPE_MIU0)
    {
        sVal = HAL_MIU_GetClientInfo(0, eClientID);
        if (sVal < 0)
            goto fail;
        
        u32Reg0 = MIU_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg1 = MIU1_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg2 = MIU2_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        
        HAL_MIU_Write2BytesBit(u32Reg0, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg1, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg2, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    else if(eType == MIU_SELTYPE_MIU1)
    {
        sVal = HAL_MIU_GetClientInfo(1, eClientID);
        if (sVal < 0)
            goto fail;
        
        u32Reg0 = MIU_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg1 = MIU1_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg2 = MIU2_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        
        HAL_MIU_Write2BytesBit(u32Reg0, 1, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg1, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg2, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    else if(eType == MIU_SELTYPE_MIU2)
    {
        sVal = HAL_MIU_GetClientInfo(2, eClientID);
        if (sVal < 0)
            goto fail;
        
        u32Reg0 = MIU_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg1 = MIU1_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg2 = MIU2_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        
        HAL_MIU_Write2BytesBit(u32Reg0, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg1, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg2, 1, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    else if(eType == MIU_SELTYPE_MIU3)
    {
        sVal = HAL_MIU_GetClientInfo(3, eClientID);
        if (sVal < 0)
            goto fail;
        
        u32Reg0 = MIU_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg1 = MIU1_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg2 = MIU2_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        
        HAL_MIU_Write2BytesBit(u32Reg0, 1, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg1, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg2, 1, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    else if(eType == MIU_SELTYPE_MIU_ALL)
    {
        sVal = HAL_MIU_GetClientInfo(0, eClientID);
        if (sVal < 0)
            goto fail;
        
        u32Reg0 = MIU_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg1 = MIU1_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        u32Reg2 = MIU2_REG_BASE + REG_MIU_SELX(MIU_GET_CLIENT_GROUP(sVal));
        
        HAL_MIU_Write2BytesBit(u32Reg0, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg1, 1, BIT(MIU_GET_CLIENT_POS(sVal)));
        HAL_MIU_Write2BytesBit(u32Reg2, 0, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    else
    {
        printf("%s not support MIU selection:%u!\n", __FUNCTION__, eType );
        return FALSE;
    }

    return TRUE;

fail:
	printf("%s not support client ID:%u!\n", __FUNCTION__, eClientID);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Mask_Req_OPM_R()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_DNRB_R()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_DNRB_W()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_DNRB_RW()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_SC_RW()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_MVOP_R()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_MVD_R()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_MVD_W()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_MVD_RW()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_Mask_Req_AUDIO_RW()
/// @brief \b Function \b Description: Set OPM MIU mask
/// @param u8Mask IN        \b  : miu mask
/// @param u8Miu  IN        \b  : miu0 or miu1
/// @param None   OUT       \b  :
/// @param None   RET       \b  :
/// @param None   GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _FUNC_NOT_USED();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_GET_MUX()
/// @brief \b Function \b Description:
/// @param None IN        \b  :
/// @param None IN        \b  :
/// @param None OUT       \b  :
/// @param None RET       \b  :
/// @param None GLOBAL    \b  :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MIU_GET_MUX(void)
{
    return 0x0000;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_SwitchMIU()
/// @brief \b Function \b Description:
/// @param u8MiuID        \b IN     : select MIU0 or MIU1
/// @param None \b RET:
////////////////////////////////////////////////////////////////////////////////
void HAL_MIU_VOP_SwitchMIU(MS_U8 u8MiuID)
{
    _FUNC_NOT_USED();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_IsI64Mode()
/// @brief \b Function \b Description:
/// @param None \b RET: 0: not support, 64 or 128 bits
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MIU_IsI64Mode(void)
{
    _FUNC_NOT_USED();
    return 0x0000;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MIU_IsInitMiu1()
/// @brief \b Function \b Description:
/// @param None \b RET:
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_IsInitMiu1(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_SetGroupPriority()
/// @brief \b Function  \b Description:  This function for set each group priority
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param sPriority    \b IN   : gropu priority
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority)
{
    MS_U8 u8Val = 0;
    MS_U32 u32RegAddr = REG_MIU_GROUP_PRIORITY;

    u8Val = (sPriority.u84th << 6 | sPriority.u83rd << 4 | sPriority.u82nd << 2 | sPriority.u81st);
    printf("Change miu%u group priority:%x\n", u8MiuDev, u8Val);

    if(u8MiuDev == E_MIU_0)
    {
        u32RegAddr += MIU_REG_BASE;
    }
    else if(u8MiuDev == E_MIU_1)
    {
        u32RegAddr += MIU1_REG_BASE;
    }
    else if(u8MiuDev == E_MIU_2)
    {
        u32RegAddr += MIU2_REG_BASE;
    }
    else
    {
	    printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuDev );
    }

    HAL_MIU_Write2BytesBit(u32RegAddr,DISABLE, BIT8);
    HAL_MIU_WriteByte(u32RegAddr, u8Val);
    HAL_MIU_Write2BytesBit(u32RegAddr,ENABLE, BIT8);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_SetGroupPriority()
/// @brief \b Function  \b Description:  This function for set each group priority
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : client ID
/// @param bMask        \b IN   : TRUE: Mask high priority FALSE: Unmask hih priority
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask)
{
    MS_S16 sVal;

    sVal = HAL_MIU_GetClientInfo(u8MiuDev, eClientID);
    if (sVal < 0)
    {
        printf("%s not support client ID:%u!\n", __FUNCTION__, eClientID);
        return FALSE;
    }
    else
    {
        MS_U32 u32Reg = REG_MIU_RQX_HPMASK(MIU_GET_CLIENT_GROUP(sVal));

        if(u8MiuDev == E_MIU_0)
        {
            u32Reg += MIU_REG_BASE;
        }
        else if(u8MiuDev == E_MIU_1)
        {
            u32Reg += MIU1_REG_BASE;
        }
        else if(u8MiuDev == E_MIU_2)
        {
            u32Reg += MIU2_REG_BASE;
        }        
        else
        {
            printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuDev );
            return FALSE;
        }

        HAL_MIU_Write2BytesBit(u32Reg, bMask, BIT(MIU_GET_CLIENT_POS(sVal)));
    }
    return TRUE;
}

MS_BOOL HAL_MIU_GetAutoPhaseResult(MS_U32 *miu0, MS_U32 *miu1)
{
    static MS_U32 u32Miu0 = 0, u32Miu1 = 0;

    *miu0 = u32Miu0;
    *miu1 = u32Miu1;

    return FALSE;
}

MS_BOOL HAL_MIU_EnableScramble(MS_BOOL bEnable)
{
    return FALSE; // not implemented yet
}

MS_BOOL HAL_MIU_IsScrambleEnabled(void)
{
    return FALSE; // not implemented yet
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_SetLoadingRequest
/// @brief \b Function  \b Description: Set loading request
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_SetLoadingRequest(MS_U8 u8MiuDev)
{
    MS_U32 u32RegAddr;

    if (u8MiuDev == 1)
    {
        u32RegAddr = MIU1_BW_REQUEST;
    }
    else
    {
        u32RegAddr = MIU_BW_REQUEST;
    }

    HAL_MIU_Write2Byte(u32RegAddr, 0x0050);
    HAL_MIU_Write2Byte(u32RegAddr, 0x0051);

    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_GetLoadingRequest
/// @brief \b Function  \b Description: Get loading request
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param *u32Loading  \b IN   : percentage of MIU loading
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_GetLoadingRequest(MS_U8 u8MiuDev, MS_U32 *u32Loading)
{
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_ParseOccupiedResource
/// @brief \b Function  \b Description: Parse occupied resource to software structure
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_ParseOccupiedResource(void)
{
    MS_U8  u8MiuSel;
    MS_U8  u8Blockx;
    MS_U8  u8ClientID;
    MS_U16 u16idenable;
    MS_U32 u32index;
    MS_U32 u32RegAddr;
    MS_U32 u32RegAddrIDenable;

    for(u8MiuSel = E_MIU_0; u8MiuSel < MIU_MAX_DEVICE; u8MiuSel++)
    {
        for(u8Blockx = E_MIU_BLOCK_0; u8Blockx < E_MIU_BLOCK_NUM; u8Blockx++)
        {
            if(u8MiuSel == E_MIU_0)
            {
                u32RegAddr = MIU_PROTECT0_ID0;

                switch (u8Blockx)
                {
                    case E_MIU_BLOCK_0:
                        u32RegAddrIDenable = MIU_PROTECT0_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_1:
                        u32RegAddrIDenable = MIU_PROTECT1_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_2:
                        u32RegAddrIDenable = MIU_PROTECT2_ID_ENABLE;
                        break;
                    case E_MIU_BLOCK_3:
                        u32RegAddrIDenable = MIU_PROTECT3_ID_ENABLE;
                        break;
                    default:
                        return false;
                }
            }
            else if(u8MiuSel == E_MIU_1)
            {
                u32RegAddr = MIU1_PROTECT0_ID0;

                switch (u8Blockx)
                {
                case E_MIU_BLOCK_0:
                     u32RegAddrIDenable = MIU1_PROTECT0_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_1:
                     u32RegAddrIDenable = MIU1_PROTECT1_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_2:
                     u32RegAddrIDenable = MIU1_PROTECT2_ID_ENABLE;
                     break;
                 case E_MIU_BLOCK_3:
                     u32RegAddrIDenable = MIU1_PROTECT3_ID_ENABLE;
                     break;
                 default:
                     return false;
                }
            }
            else
            {
                printf("%s not support MIU%u!\n", __FUNCTION__, u8MiuSel );
                return FALSE;
            }

            u16idenable = HAL_MIU_Read2Byte(u32RegAddrIDenable);
            for(u32index = 0; u32index < MIU_MAX_PROTECT_ID; u32index++)
            {
                IDEnables[u8MiuSel][u8Blockx][u32index] = ((u16idenable >> u32index) & 0x1)? 1: 0;
            }
        }//for(u8Blockx = E_MIU_BLOCK_0; u8Blockx < E_MIU_BLOCK_NUM; u8Blockx++)

        for(u32index = 0; u32index < MIU_MAX_PROTECT_ID; u32index++)
        {
            u8ClientID = HAL_MIU_ReadByte(u32RegAddr + u32index);
            IDs[u8MiuSel][u32index] = clientTbl[u8MiuSel][u8ClientID];
        }
    }//for(u8MiuSel = E_MIU_0; u8MiuSel < E_MIU_NUM; u8MiuSel++)

    return TRUE;
}


void HAL_MIU_PrintMIUProtectArea(MS_U8 u8Blockx,MS_U8 miu_dev)
{

    MS_U16 val_16,val1_16,val2_16;
    MS_U32 val_32;
    MS_U32 u32RegAddrMSB;
    MS_U32 u32RegAddrStar;
      

    if( miu_dev == E_MIU_0 )
    {
        u32RegAddrMSB =  MIU_PROTECT0_MSB;
        switch (u8Blockx)
        {
            case 0:
                u32RegAddrStar = MIU_PROTECT0_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1 );
                val2_16 = ( val1_16 & 0xC );
                break;

            case 1:
                u32RegAddrStar = MIU_PROTECT1_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x10 );
                val2_16 = ( val1_16 & 0xC0 );
                break;

            case 2:
                u32RegAddrStar = MIU_PROTECT2_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x100 );
                val2_16 = ( val1_16 & 0xC00 );
                break;

            case 3:
                u32RegAddrStar = MIU_PROTECT3_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1000 );
                val2_16 = ( val1_16 & 0xC000 );
                break;

            default:
                printf("error miu protect block number\n");
                return ;
        }
    }
    else if( miu_dev == E_MIU_1 )
    {
        u32RegAddrMSB =  MIU1_PROTECT0_MSB;
        switch (u8Blockx)
        {
            case 0:
                u32RegAddrStar = MIU1_PROTECT0_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1 );
                val2_16 = ( val1_16 & 0xC );
                break;

            case 1:
                u32RegAddrStar = MIU1_PROTECT1_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x10 );
                val2_16 = ( val1_16 & 0xC0 );
                break;

            case 2:
                u32RegAddrStar = MIU1_PROTECT2_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x100 );
                val2_16 = ( val1_16 & 0xC00 );
                break;

            case 3:
                u32RegAddrStar = MIU1_PROTECT3_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1000 );
                val2_16 = ( val1_16 & 0xC000 );
                break;

            default:
                printf("error miu protect block number\n");
                return ;
        }

    }
    else if( miu_dev == E_MIU_2 )
    {
        u32RegAddrMSB =  MIU2_PROTECT0_MSB;
        switch (u8Blockx)
        {
            case 0:
                u32RegAddrStar = MIU2_PROTECT0_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1 );
                val2_16 = ( val1_16 & 0xC );
                break;

            case 1:
                u32RegAddrStar = MIU2_PROTECT1_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x10 );
                val2_16 = ( val1_16 & 0xC0 );
                break;

            case 2:
                u32RegAddrStar = MIU2_PROTECT2_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x100 );
                val2_16 = ( val1_16 & 0xC00 );
                break;

            case 3:
                u32RegAddrStar = MIU2_PROTECT3_START;
                val1_16 = HAL_MIU_Read2Byte(u32RegAddrMSB);
                val1_16 = ( val1_16 & 0x1000 );
                val2_16 = ( val1_16 & 0xC000 );
                break;

            default:
                printf("error miu protect block number\n");
                return ;
        }

    }
    else
    {
        printf("%s not support MIU%u!\n", __FUNCTION__, miu_dev );
        return;
    }

    //protect start address
    val_16 = HAL_MIU_Read2Byte(u32RegAddrStar);
    val_32 = (( val_16 ) | (( val1_16  ) << 4 )) << MIU_PAGE_SHIFT;
    printf("miu%d protect%d startaddr is 0x%tX\n", miu_dev, u8Blockx, (ptrdiff_t)val_32);

    //protect end address
    val_16 = HAL_MIU_Read2Byte(u32RegAddrStar+0x2);
    val_32 = ((( val_16 + 1 ) | (( val1_16  ) << 4 ) ) << MIU_PAGE_SHIFT ) - 1;

    printf("miu%d protect%d endaddr   is 0x%tX\n", miu_dev, u8Blockx, (ptrdiff_t)val_32);
}

void HAL_MIU_PrintMIUProtectInfo(void)
{
    MS_U32 u32MiuProtectEn;
    MS_U32 u32MiuProtectIdEn;
    
    MS_U8 val_8;      
    MS_U16 val_16;      

    u32MiuProtectEn = MIU_PROTECT_EN;

    u32MiuProtectIdEn= MIU_PROTECT0_ID_ENABLE;
    val_8 = HAL_MIU_ReadByte(u32MiuProtectEn);
   
    //printf("val=%d\n",val);
    
    if ( (val_8 & 0xf) != 0 )
    {
        printf("miu0 protect is enabled\n");

        //protect_ID_enable information   
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu0 protect0_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(0,0);
        }
        
        u32MiuProtectIdEn= MIU_PROTECT1_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu0 protect1_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(1,0);
        }

        u32MiuProtectIdEn= MIU_PROTECT2_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu0 protect2_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(2,0);
        }

        u32MiuProtectIdEn= MIU_PROTECT3_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu0 protect3_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(3,0);
        }

    }
    else
    {
        printf("miu0 protect is not enabled\n");
    }

    u32MiuProtectEn=MIU1_PROTECT_EN;
    val_8 = HAL_MIU_ReadByte(u32MiuProtectEn);

    //printf("val=%d\n",val);

    if ( (val_8 & 0xf) != 0x0 )
    {
        printf("miu1 protect is enabled\n");

        //protect_ID_enable information   
        u32MiuProtectIdEn= MIU1_PROTECT0_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu1 protect0_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(0,1);
        }

        u32MiuProtectIdEn= MIU1_PROTECT1_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu1 protect1_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(1,1);
        }

        u32MiuProtectIdEn= MIU1_PROTECT2_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu1 protect2_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(2,1);
        }

        u32MiuProtectIdEn= MIU1_PROTECT3_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu1 protect3_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(3,1);
        }


    }  
    else
    {
        printf("miu1 protect is not enabled\n");

    } 
    
    u32MiuProtectEn=MIU2_PROTECT_EN;
    val_8 = HAL_MIU_ReadByte(u32MiuProtectEn);

    //printf("val=%d\n",val);

    if ( (val_8 & 0xf) != 0x0 )
    {
        printf("miu2 protect is enabled\n");

        //protect_ID_enable information   
        u32MiuProtectIdEn= MIU2_PROTECT0_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu2 protect0_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(0,2);
        }

        u32MiuProtectIdEn= MIU2_PROTECT1_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu2 protect1_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(1,2);
        }

        u32MiuProtectIdEn= MIU2_PROTECT2_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu2 protect2_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(2,2);
        }

        u32MiuProtectIdEn= MIU2_PROTECT3_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu2 protect3_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(3,2);
        }


    }  
    else
    {
        printf("miu2 protect is not enabled\n");

    } 
    
    u32MiuProtectEn=MIU3_PROTECT_EN;
    val_8 = HAL_MIU_ReadByte(u32MiuProtectEn);

    //printf("val=%d\n",val);

    if ( (val_8 & 0xf) != 0x0 )
    {
        printf("miu3 protect is enabled\n");

        //protect_ID_enable information   
        u32MiuProtectIdEn= MIU3_PROTECT0_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu3 protect0_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(0,3);
        }

        u32MiuProtectIdEn= MIU3_PROTECT1_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu3 protect1_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(1,3);
        }

        u32MiuProtectIdEn= MIU3_PROTECT2_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu3 protect2_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(2,3);
        }

        u32MiuProtectIdEn= MIU3_PROTECT3_ID_ENABLE;
        val_16 =HAL_MIU_Read2Byte(u32MiuProtectIdEn);
        if ( val_16 != 0 )
        {
            printf("miu3 protect3_ID_enable is 0x%x\n",val_16);
            HAL_MIU_PrintMIUProtectArea(3,3);
        }


    }  
    else
    {
        printf("miu3 protect is not enabled\n");

    } 
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_MIU_GetClientWidth
/// @brief \b Function  \b Description: Get MIU client bus width
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : client ID
/// @param pClientWidth \b IN   : client bus width
/// @return             \b 0 : Fail  1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MIU_GetClientWidth(MS_U8 u8MiuDevi, eMIUClientID eClientID, eMIU_ClientWidth *pClientWidth)
{
    MS_U32 u32Reg;
    MS_S16 sVal;
    MS_U16 u16Group;
    MS_U16 u16Client;
    MS_U16 u16RegVal;

    sVal = HAL_MIU_GetClientInfo(u8MiuDevi, eClientID);

    if (sVal < 0)
    {
        printf("%s not support client ID:%u!\n", __FUNCTION__, eClientID);
        return FALSE;
    }
    else
    {
        u16Group = MIU_GET_CLIENT_GROUP(sVal);
        u16Client = MIU_GET_CLIENT_POS(sVal);

        if(u16Group >= 4)
        {
            *pClientWidth = E_MIU_CLIENT_256BIT;
            return TRUE;
        }

        u32Reg = REG_MI64_FORCE + (MIU_GET_CLIENT_GROUP(sVal) << 1);
        u16RegVal = HAL_MIU_Read2Byte(u32Reg);
        
        if(u16RegVal & (1 << u16Client))
            *pClientWidth = E_MIU_CLIENT_64BIT;
        else
            *pClientWidth = E_MIU_CLIENT_128BIT;

        return TRUE;
    }
}

MS_BOOL HAL_MIU_GetMIUSelectSettings(MS_U16 pSelectData[][MIU_SELECT_GROUP_MAX])
{

    //MIU0
    pSelectData[0][0] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP0_REG);
    pSelectData[0][1] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP1_REG);
    pSelectData[0][2] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP2_REG);
    pSelectData[0][3] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP3_REG);
    pSelectData[0][4] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP4_REG);
    pSelectData[0][5] = HAL_MIU_Read2Byte (MIU_SELECT_GROUP5_REG);

    //MIU1
    pSelectData[1][0] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP0_REG);
    pSelectData[1][1] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP1_REG);
    pSelectData[1][2] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP2_REG);
    pSelectData[1][3] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP3_REG);
    pSelectData[1][4] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP4_REG);
    pSelectData[1][5] = HAL_MIU_Read2Byte (MIU1_SELECT_GROUP5_REG);

    return TRUE;
}

MS_BOOL HAL_MIU_SetMIUSelectSettings(MS_U16 pSelectData[][MIU_SELECT_GROUP_MAX])
{

    HAL_MIU_Write2Byte (MIU_SELECT_GROUP0_REG, pSelectData[0][0]);
    HAL_MIU_Write2Byte (MIU_SELECT_GROUP1_REG, pSelectData[0][1]);
    HAL_MIU_Write2Byte (MIU_SELECT_GROUP2_REG, pSelectData[0][2]);
    HAL_MIU_Write2Byte (MIU_SELECT_GROUP3_REG, pSelectData[0][3]);
    HAL_MIU_Write2Byte (MIU_SELECT_GROUP4_REG, pSelectData[0][4]);
    HAL_MIU_Write2Byte (MIU_SELECT_GROUP5_REG, pSelectData[0][5]);

    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP0_REG, pSelectData[1][0]);
    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP1_REG, pSelectData[1][1]);
    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP2_REG, pSelectData[1][2]);
    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP3_REG, pSelectData[1][3]);
    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP4_REG, pSelectData[1][4]);
    HAL_MIU_Write2Byte (MIU1_SELECT_GROUP5_REG, pSelectData[1][5]);

    return TRUE;
}
