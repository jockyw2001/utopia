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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define HAL_SAR_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsTypes.h"

// Internal Definition
#include "regSAR.h"
#include "halSAR.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define BIT0    BIT(0)
#define BIT1    BIT(1)
#define BIT2    BIT(2)
#define BIT3    BIT(3)
#define BIT4    BIT(4)
#define BIT5    BIT(5)
#define BIT6    BIT(6)
#define BIT7    BIT(7)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_MapBase_NPM = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_ERR(x, args...)        //{printf(x, ##args);}
#define HAL_SAR_INFO(x, args...)        //{printf(x, ##args);}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_SAR_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[(u32RegAddr)];
}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    return (u8Val & u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte_NPM
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte_NPM(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte_NPM(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask_NPM(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte_NPM(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte_NPM(u32RegAddr, u8Val);
    return TRUE;
}

#if 0//Temporarily marked out for compiler warning free
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_SAR_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U32 HAL_SAR_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_SAR_Read2Byte(u32RegAddr) | HAL_SAR_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_SAR_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_SAR_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
    HAL_SAR_INFO("SAR IOMap base:%8lx Reg offset:%4x\n", virtBase, SAR_REG_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_SAR_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgChannelBound
/// @brief \b Function  \b Description: Configure sar channel upper/lower bound
/// @param <IN>         \b u8Channel : sar channel (0~7), psarBndCfg: sar bound info
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_CfgChannelBound(MS_U8 u8Channel ,HAL_SAR_BndCfg *psarBndCfg)
{
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return FALSE;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH2:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH3:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH4:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH5:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH6:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH7:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

        case HAL_SAR_CH8:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
            break;

    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetChannelADC
/// @brief \b Function  \b Description: Get sar channel ADC value
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8: sar ADC value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SAR_GetChannelADC(MS_U8 u8Channel)
{
    MS_U32 u32Reg=REG_SAR_ADC_CH1_DATA;
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return HAL_SAR_ADC_DATA_MAX;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u32Reg = REG_SAR_ADC_CH1_DATA;
            break;
        case HAL_SAR_CH2:
            u32Reg = REG_SAR_ADC_CH2_DATA;
            break;
        case HAL_SAR_CH3:
            u32Reg = REG_SAR_ADC_CH3_DATA;
            break;
        case HAL_SAR_CH4:
            u32Reg = REG_SAR_ADC_CH4_DATA;
            break;
        case HAL_SAR_CH5:
            u32Reg = REG_SAR_ADC_CH5_DATA;
            break;
        case HAL_SAR_CH6:
            u32Reg = REG_SAR_ADC_CH6_DATA;
            break;
        case HAL_SAR_CH7:
            u32Reg = REG_SAR_ADC_CH7_DATA;
            break;
        case HAL_SAR_CH8:
            u32Reg = REG_SAR_ADC_CH8_DATA;
            break;
    }

    HAL_SAR_WriteByte(REG_SAR_CTRL1 ,HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);
    wChannelAdcValue = (HAL_SAR_Read2Byte(u32Reg) & _SAR_ADC_OUT_10BITMSK);
    wChannelAdcValue = ((wChannelAdcValue >> 2) & _SAR_ADC_OUT_8BITMSK);
    /* because of interface can't be change , we return [9:2] to drv layer */

    return (MS_U8) wChannelAdcValue;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannel
/// @brief \b Function  \b Description: Configure channel for single channel mode
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannel(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, u8Channel, _SAR_SINGLE_CH_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgTriggerMode
/// @brief \b Function  \b Description: Configure keypad level(trigger) mode
/// @param <IN>         \b bMode : 0: edge trigger mode, 1: level trigger mode
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgTriggerMode(MS_U8 bMode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bMode)? _SAR_LEVEL_TRIGGER : 0), _SAR_LEVEL_TRIGGER);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannelEn
/// @brief \b Function  \b Description: enable single channel mode
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannelEn(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_SINGLE_CH_EN : 0), _SAR_SINGLE_CH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitOperMode
/// @brief \b Function  \b Description: Configure sar digital operation mode
/// @param <IN>         \b u8Mode : 0: one-shot, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitOperMode(MS_U8 u8Mode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((u8Mode)? _SAR_MODE : 0), _SAR_MODE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitPowerdown
/// @brief \b Function  \b Description: Configure sar digital power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_PD : 0), _SAR_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgStart
/// @brief \b Function  \b Description: Configure sar to trigger start signal
/// @param <IN>         \b bEnable : 0: stop, 1: trigger to start
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgStart(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_START : 0), _SAR_START);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopPowerdown
/// @brief \b Function  \b Description: Configure sar atop power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_ADC_PD : 0), _SAR_ADC_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopFreeRun
/// @brief \b Function  \b Description: Configure sar atop freerun mode
/// @param <IN>         \b bEnable : 0: controlled by digital, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopFreeRun(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_FREERUN : 0), _SAR_FREERUN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSelection
/// @brief \b Function  \b Description: Configure sar selection
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSelection(MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask=_SAR_SEL;

    u8CtrlData = (bEnable)? u8Mask : 0;
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgHighChannel
/// @brief \b Function  \b Description: Configure to use high sar channel ( 4~7, or 4~5)
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgHighChannel(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_NCH_EN : 0), _SAR_NCH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgClockSamplePeriod
/// @brief \b Function  \b Description: Configure sample period
/// @param <IN>         \b u8ClkSmpPrd :
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgClockSamplePeriod(MS_U8 u8ClkSmpPrd)
{
    HAL_SAR_WriteByteMask(REG_SAR_CKSAMP_PRD, u8ClkSmpPrd, _SAR_CKSAMP_PRD_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAanlogInputSelect
/// @brief \b Function  \b Description: Configure Analog Input/GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable : 0: GPIO, 1: Analog Input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAanlogInputSelect(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH1 : (~_SAR_AISEL_CH1);
            u8Mask=_SAR_AISEL_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH2 : (~_SAR_AISEL_CH2);
            u8Mask=_SAR_AISEL_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH3 : (~_SAR_AISEL_CH3);
            u8Mask=_SAR_AISEL_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH4 : (~_SAR_AISEL_CH4);
            u8Mask=_SAR_AISEL_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH5 : (~_SAR_AISEL_CH5);
            u8Mask=_SAR_AISEL_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_AISEL, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgOutputEnable
/// @brief \b Function  \b Description: Configure output enable for sar channel set as GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable: 0: output, 1: input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgOutputEnable(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH1 : _SAR_OEN_GPIO_CH1;
            u8Mask = _SAR_OEN_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH2 : _SAR_OEN_GPIO_CH2;
            u8Mask = _SAR_OEN_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH3 : _SAR_OEN_GPIO_CH3;
            u8Mask = _SAR_OEN_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH4 : _SAR_OEN_GPIO_CH4;
            u8Mask = _SAR_OEN_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH5 : _SAR_OEN_GPIO_CH5;
            u8Mask = _SAR_OEN_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_OEN_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetOutput
/// @brief \b Function  \b Description: Set GPIO output value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetOutput(MS_U8 u8Channel, MS_BOOL bHighLow)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH1 : ~_SAR_I_GPIO_CH1;
            u8Mask = _SAR_I_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH2 : ~_SAR_I_GPIO_CH2;
            u8Mask = _SAR_I_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH3 : ~_SAR_I_GPIO_CH3;
            u8Mask = _SAR_I_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH4 : ~_SAR_I_GPIO_CH4;
            u8Mask = _SAR_I_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH5 : ~_SAR_I_GPIO_CH5;
            u8Mask = _SAR_I_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_I_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetInput
/// @brief \b Function  \b Description: Get GPIO input value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b TRUE: High FALSE: Low
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetInput(MS_U8 u8Channel)
{
    MS_U8 u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8Mask = _SAR_C_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8Mask = _SAR_C_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8Mask = _SAR_C_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8Mask = _SAR_C_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8Mask = _SAR_C_GPIO_CH5;
            break;
        default: return FALSE;
    }
    return HAL_SAR_ReadRegBit(REG_SAR_C_GPIO, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntMask
/// @brief \b Function  \b Description: Interrupt mask for sar int
/// @param <IN>         \b bEnable : 0: enable int, 1: disable int
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntMask(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_MASK : 0), _SAR_INT_MASK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntClear
/// @brief \b Function  \b Description: Interrupt clear for sar int
/// @param <IN>         \b bEnable : 0: not clear, 1: clear
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntClear(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_CLR : 0), _SAR_INT_CLR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntForce
/// @brief \b Function  \b Description: Force interrupt for sar int
/// @param <IN>         \b bEnable : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntForce(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_FORCE : 0), _SAR_INT_FORCE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIntStatus
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b None : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetIntStatus(void)
{
    MS_U8 u8IntStatus;

    u8IntStatus = HAL_SAR_ReadByte(REG_SAR_STATUS0) & _SAR_INT_STATUS;
    return (u8IntStatus)? ENABLE : DISABLE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase_NPM
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase_NPM(MS_VIRT virtBase)
{
    _gMIO_MapBase_NPM = virtBase;
    HAL_SAR_INFO("SAR NPM IOMap base:%8lx Reg offset:%4x\n", u32Base);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase_NPM
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_SAR_GetIOMapBase_NPM(void)
{
    return _gMIO_MapBase_NPM;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetAdcHSyncChannel
/// @brief \b Function  \b Description: Switch HSync signal to SAR High channel
/// @param <IN>         \b u8Channel : ADC HSync channel (0~2)
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_SetAdcHSyncChannel(MS_U8 u8Channel)
{
    MS_U8 u8HSyncCh;

    switch(u8Channel)
    {
        case HAL_SAR_ADC_HSYNC_CH0:
            u8HSyncCh = _SAR2_HSYNC_CH0;
            break;
        case HAL_SAR_ADC_HSYNC_CH1:
            u8HSyncCh = _SAR2_HSYNC_CH1;
            break;
        case HAL_SAR_ADC_HSYNC_CH2:
            u8HSyncCh = _SAR2_HSYNC_CH2;
            break;
        case HAL_SAR_ADC_HSYNC_CH3:
            u8HSyncCh = _SAR2_HSYNC_CH3;
            break;
        default: return FALSE;
    }
    return HAL_SAR_WriteByteMask_NPM(REG_ADC_ATOP_SAR2, u8HSyncCh, _SAR2_HSYNC_MSK);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Interrupt_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt Enable/Disable
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <IN>         \b bEnablel : True: enable interrupt; False: disable interrupt
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Interrupt_EN(MS_U8 u8Channel, MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_SAR_INT, ((bEnable)? 1<<u8Channel : 0), 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Wakeup_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt  Wakeup Enable/Disable
/// @param <IN>         \b bEnablel : True: enable interrrupt wakeup; False: disable interrupt wakeup
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Wakeup_EN(MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_WK_IRQ, ((bEnable)? _W_SAR : 0), _W_SAR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CLR_INT
/// @brief \b Function  \b Description: Switch SAR Clear Interrupt Status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b  None
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CLR_INT(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 1<<u8Channel, 1<<u8Channel);
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 0, 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_INT_Status
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_INT_Status(MS_U8 u8Channel)
{
    return (HAL_SAR_ReadByte(REG_SAR_INT_STATUS) & (1<<u8Channel)) > 0? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SET_LEVEL
/// @brief \b Function  \b Description: set sar level is 2.0V or 3.3V
/// @param <IN>         \b bLevel : 1:3.3V , 0:2.0V
/// @param <OUT>        \b None :
/// @param <RET>        \b Nonw:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SET_LEVEL(MS_BOOL bLevel)
{
    //HAL_SAR_WriteByteMask(REG_SAR_TEST0, (bLevel? 1<<6:0) , 1<<6);
    HAL_SAR_WriteByteMask(REG_SAR__REF_V_SEL, (bLevel?0xFF:0x00), 0xFF);
}

#ifdef CONFIG_MSTAR_T_SENSOR_ENABLE

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

//T-Sensor
//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
#define TSENSOR_DEBUG 0

#if TSENSOR_DEBUG
#define t_print(fmt, args...)  printf(fmt, ## args)
#else
#define t_print(fmt, args...)  while(0);
#endif

static MS_U16  _u16_25SARCode = 0;
#if TSENSOR_DEBUG
MS_U32 _u32SARBase   = 0;
#endif
static MS_BOOL _bSARInit  = FALSE;

static MS_S16 _SarToCelsius(MS_U16 u16SarCode);

static MS_S16 _CelsiusToSar(MS_S16 Celsius);
//-------------------------------------------------------------------------------------------------
/// Enable/Init T-Sensor module
/// @param None:
/// @return TRUE : Enable OK
/// @return FALSE : Enable Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorEnable(void)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16VbeCodeFt = 0 ,u16VbeCode= 0;

    if ( _bSARInit == TRUE)
    {
        return TRUE;
    }

    //Tsensor Initialization
    bRet &= HAL_SAR_WriteByte(REG_SAR_ADC_REFV_SEL, HAL_SAR_ReadByte(REG_SAR_ADC_REFV_SEL)&~_SAR_ADC_CH8_REFV_SEL); // 0:2.0V,  1:3.3V
    bRet &= HAL_SAR_WriteByte(REG_SAR_PM_DMY, HAL_SAR_ReadByte(REG_SAR_PM_DMY)&~BIT0); //select external t-sensor from CPU side

    bRet &= HAL_SAR_WriteByte(REG_PM_TR, HAL_SAR_ReadByte(REG_PM_TR)|BIT7); //select external t-sensor from CPU side
    bRet &= HAL_SAR_WriteByte(REG_PM_TSENSOR_H, HAL_SAR_ReadByte(REG_PM_TSENSOR_H)|_EN_TSEN); //enable t-sensor

    //Freerun mode
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL0, HAL_SAR_ReadByte(REG_SAR_CTRL0)&~_SAR_SINGLE_CH_EN);
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL0, HAL_SAR_ReadByte(REG_SAR_CTRL0)|_SAR_MODE);
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_FREERUN);
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_NCH_EN);

    //Power ON
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)&~_SAR_ADC_PD);// power on SAR_ATOP
    MsOS_DelayTaskUs(10);
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL0, HAL_SAR_ReadByte(REG_SAR_CTRL0)&~_SAR_PD);// power on SAR_DTOP

    //Read SAR DATA of T-sensor
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);//Set this value to 1 before getting the value of SAR
    u16VbeCode = HAL_SAR_Read2Byte(REG_SAR_ADC_CH8_DATA);
    u16VbeCode = u16VbeCode & _SAR_ADC_OUT_10BITMSK;

    //Read VBE_Code trimmed in FT
    u16VbeCodeFt = HAL_SAR_Read2Byte(REG_VBE_CODE_FT);
    u16VbeCodeFt = (u16VbeCodeFt>>6) & _SAR_ADC_OUT_10BITMSK;
    t_print("u16VbeCodeFt %lx\n",u16VbeCodeFt);
    //---

    _u16_25SARCode = u16VbeCodeFt;
    t_print("[TSNSR] Read constant 25C SARCode:0x%x(%d C) \n",_u16_25SARCode,_SarToCelsius(_u16_25SARCode));
    _bSARInit = TRUE;

    MsOS_DelayTask(20);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Enable T-Sensor interrupt in SAR bank
/// @param None:
/// @return TRUE : Enable OK
/// @return FALSE : Enable Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorIRQEnable(void)
{
    MS_BOOL bRet = TRUE;
    bRet &= HAL_SAR_WriteByteMask(REG_SAR_INT, 0, (MS_U8)BIT_PM_SAR_CHANNEL8);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Disable T-Sensor interrupt in SAR bank
/// @param None:
/// @return TRUE : Disable OK
/// @return FALSE : Disable Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorIRQDisable(void)
{
    MS_BOOL bRet = TRUE;
    bRet &= HAL_SAR_WriteByteMask(REG_SAR_INT, (MS_U8)BIT_PM_SAR_CHANNEL8, (MS_U8)BIT_PM_SAR_CHANNEL8);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Clear T-Sensor INT in SAR BANK
/// @param None:
/// @return TRUE : Clear OK
/// @return FALSE : Clear Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorIRQClear(void)
{
    MS_BOOL bRet = TRUE;
    bRet &= HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, (MS_U8)BIT_PM_SAR_CHANNEL8, (MS_U8)BIT_PM_SAR_CHANNEL8);
    bRet &= HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 0, (MS_U8)BIT_PM_SAR_CHANNEL8);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Set temperature up /low bound.
/// Please note that higher temperature => lower SAR code , lower temperature => higher SAR code
/// @param s16UpBound_C : up bound temperature (unit:Celsius)
/// @param s16LowBound_C : low bound temperature (unit:Celsius)
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorSetBound(MS_S16 s16UpBound_C , MS_S16 s16LowBound_C)
{
    MS_BOOL bRet = TRUE;
    MS_U16 u16LowBound_SAR,u16UpBound_SAR;
    bRet &= HAL_TSensorIRQDisable();
    bRet &= HAL_SAR_WriteByte(REG_PM_TSENSOR_H, HAL_SAR_ReadByte(REG_PM_TSENSOR_H)&~_EN_TSEN); //disable t-sensor
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);//Set this value to 1 before getting the value of SAR
    bRet &= HAL_SAR_Write2Byte(REG_SAR_CH8_LOB,0);

    MsOS_DelayTaskUs(10);
    u16LowBound_SAR = _CelsiusToSar(s16LowBound_C);
    bRet &= HAL_SAR_Write2Byte(REG_SAR_CH8_UPD,u16LowBound_SAR);

    HAL_TSensorIRQClear();
    bRet &= HAL_SAR_WriteByte(REG_PM_TSENSOR_H, HAL_SAR_ReadByte(REG_PM_TSENSOR_H)|_EN_TSEN); //enable t-sensor
    bRet &= HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);//Set this value to 1 before getting the value of SAR
    HAL_TSensorIRQEnable();

    MsOS_DelayTaskUs(10);
    u16UpBound_SAR = _CelsiusToSar(s16UpBound_C);
    bRet &= HAL_SAR_Write2Byte(REG_SAR_CH8_LOB,u16UpBound_SAR);

    HAL_TSensorINTCheck();

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Return the temperature up bound
/// @param None:
/// @return u16UpBound_C : Temperature up bound (unit:Celsius)
//-------------------------------------------------------------------------------------------------
MS_S16 HAL_TSensorGetUpBound(void)
{
    MS_U16 u16UpBound_SAR = 0;
    MS_S16 u16UpBound_C = 0;
    u16UpBound_SAR = HAL_SAR_Read2Byte(REG_SAR_CH8_LOB)&_SAR_ADC_OUT_10BITMSK;
    u16UpBound_C   = _SarToCelsius(u16UpBound_SAR);
    t_print("[TSNSR] UPB = %d C = SAR:0x%x\n",u16UpBound_C,u16UpBound_SAR);

    return u16UpBound_C;
}

//-------------------------------------------------------------------------------------------------
/// Return the temperature low bound
/// @param None:
/// @return u8LowBound_C : Temperature low bound (unit:Celsius)
//-------------------------------------------------------------------------------------------------
MS_S16 HAL_TSensorGetLowBound(void)
{
    MS_U16 u16LowBound_SAR = 0;
    MS_S16 u16LowBound_C   = 0;
    u16LowBound_SAR = HAL_SAR_Read2Byte(REG_SAR_CH8_UPD)&_SAR_ADC_OUT_10BITMSK;
    u16LowBound_C   = _SarToCelsius(u16LowBound_SAR);
    t_print("[TSNSR] LOB = %d C = SAR:0x%x\n",u16LowBound_C,u16LowBound_SAR);

    return u16LowBound_C;
}

//-------------------------------------------------------------------------------------------------
/// Return the chip's current temperature (unit:Celsius)
/// @param None:
/// @return s16CurTemperature_C : the chip's current temperature (unit:Celsius)
//-------------------------------------------------------------------------------------------------
MS_S16 HAL_TSensorGetCurTemp_C(void)
{
    MS_U16 u16VbeCode = 0;
    MS_S16 s16CurTemperature_C = 0;
    //Read SAR DATA of T-sensor
    HAL_SAR_WriteByte(REG_SAR_CTRL1, HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);//Set this value to 1 before getting the value of SAR 
    u16VbeCode = HAL_SAR_Read2Byte(REG_SAR_ADC_CH8_DATA);
    s16CurTemperature_C = _SarToCelsius(u16VbeCode);
    t_print("[TSNSR] Read current SAR code = 0x%x = %d C\n",u16VbeCode,s16CurTemperature_C);

    return s16CurTemperature_C;
}

//-------------------------------------------------------------------------------------------------
/// Check that the interrupt is triggered by t-sensor
/// @param None:
/// @return true : the SAR bank interrupt is triggered by t-sensor
/// @return flase: the SAR bank interrupt is not triggered by t-sensor
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSensorINTCheck(void)
{
    MS_U16 u16RegStatus = 0;

    u16RegStatus = HAL_SAR_ReadByte(REG_SAR_INT_STATUS);
    t_print("[TSNSR] PM_SAR_INT_STATUS :0x%x\n",u16RegStatus);

    //check IRQ is caused by TSensor
    if ( !(u16RegStatus & BIT_PM_SAR_CHANNEL8) )
    {
        t_print("[TSNSR] IRQ is not caused by T-Sensor\n");
        return FALSE;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// @param None: u8SarCode: temperature (unit:SAR code)
/// @return : temperature (unit:Celsius )
//-------------------------------------------------------------------------------------------------
static MS_S16 _SarToCelsius(MS_U16 u16SarCode)
{
    return (MS_S16)(5 * ((MS_S16) _u16_25SARCode - (MS_S16) u16SarCode) + 2) / 4 + 30;
}

//-------------------------------------------------------------------------------------------------
/// @param None: Celsius: temperature (unit:Celsius)
/// @return : temperature (unit:SAR code )
//-------------------------------------------------------------------------------------------------
static MS_S16 _CelsiusToSar(MS_S16 Celsius)
{
    return (MS_S16)((MS_S16) _u16_25SARCode - ((4 * (Celsius - 30) - 2) / 5));
}

#endif

#undef HAL_SAR_C

