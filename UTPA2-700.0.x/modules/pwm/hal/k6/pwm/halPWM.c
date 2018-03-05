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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////
#define _HAL_PWM_C

////////////////////////////////////////////////////////////////////////////////
/// @file mhal_PWM.c
/// @author MStar Semiconductor Inc.
/// @brief Pulse Width Modulation driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "drvPWM.h"/* this is not good idea, just for temp. */
#include "halPWM.h"
#include "regPWM.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define HAL_SUBBANK0    {do{HAL_PWM_WriteByte(REG_PWM_BASE, 0);}while(0);}
#define HAL_SUBBANK1    {do{HAL_PWM_WriteByte(REG_PWM_BASE, 1);}while(0);}  /* PWM */

////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////
#ifdef CONFIG_NONPM_PWM
static MS_BOOL _gPWM_VDBen  = 0;
#endif
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_PM_MapBase = 0;
////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function    \b Name: HAL_PWM_ReadByte
/// @brief \b Function    \b Description: read 1 Byte data
/// @param <IN>           \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>         \b MS_U8
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////

MS_U8 HAL_PWM_ReadByte(MS_U32 u32RegAddr)
{
    return  ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Read2Byte
/// @brief \b Function   \b Description: read 2 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_PWM_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Read4Byte
/// @brief \b Function   \b Description: read 4 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_PWM_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_PWM_Read2Byte(u32RegAddr) | HAL_PWM_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_WriteByte
/// @brief \b Function   \b Description: write 1 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <IN>          \b u8Val : 1 byte data
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Write2Byte
/// @brief \b Function   \b Description: write 2 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <IN>          \b u16Val : 2 byte data
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Write4Byte
/// @brief \b Function   \b Description: write 4 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <IN>          \b u32Val : 4 byte data
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    HAL_PWM_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_PWM_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_WriteRegBit
/// @brief \b Function   \b Description: write 1 Byte data
/// @param <IN>          \b u32RegAddr: register address
/// @param <IN>          \b u8Val : 1 byte data
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = HAL_PWM_ReadByte(u32RegAddr);
    u8Val = HAL_PWM_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_PWM_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

MS_BOOL HAL_PWM_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Value, MS_U8 u8Mask)
{

	MS_U8 u8TempValue = HAL_PWM_ReadByte(u32RegAddr);

	u8TempValue = (u8TempValue & (~u8Mask)) | (u8Value | u8Mask);
	HAL_PWM_WriteByte(u32RegAddr, u8TempValue);

    return TRUE;
}

#ifdef CONFIG_NONPM_PWM
static MS_BOOL HAL_PWM_WriteNumberByte(MS_U32 u32RegAddr, MS_U32 u32SetValue, MS_U8 u8BitNum)
{
    MS_U32 u32Mask=0x0;
    MS_U32 u32Value=0x0;
    MS_U8 i;

    for(i = 0; i<u8BitNum; i++)
        u32Mask |= (1<<i);

    if (u8BitNum == 0)
    {
        return TRUE;
    }
    else if (u8BitNum <= 8)
    {
        u32Value = u32SetValue & u32Mask;
        HAL_PWM_WriteByte(u32RegAddr, (MS_U8) u32Value) ;
    }
    else if (u8BitNum <= 16)
    {
        u32Value = u32SetValue & u32Mask;
        HAL_PWM_Write2Byte(u32RegAddr, (MS_U16) u32Value) ;
    }
    else if (u8BitNum <= 32)
    {
        u32Value = u32SetValue & u32Mask;
        HAL_PWM_Write4Byte(u32RegAddr, (MS_U32) u32Value) ;
    }
    else
    {
        return FALSE;
    }

    return TRUE;

}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_SetIOMapBase
/// @brief \b Function   \b Description: Set IO Map base
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_SetIOMapBase(MS_VIRT virtBase,MS_VIRT virtBase1)
{
    _gMIO_MapBase = virtBase;
    _gMIO_PM_MapBase = virtBase1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_SetChipTopIOMapBase
/// @brief \b Function   \b Description: Set chip top IO Map base
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_SetChipTopIOMapBase(MS_VIRT virtBase)
{

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_GetIOMapBase
/// @brief \b Function   \b Description: Get IO Map base
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_PWM_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Init
/// @brief \b Function   \b Description: Initial PWM
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Init(void)
{
    MS_BOOL ret = FALSE;
#if 0
    HAL_PWM_WriteRegBit(REG_ALL_PAD_IN, BIT7, 0);
    HAL_PWM_WriteRegBit(REG_PWM_IS_GPIO, BIT0, 0);
    HAL_PWM_WriteRegBit(REG_PWM_IS_GPIO, BIT1, 0);
    HAL_PWM_WriteRegBit(REG_PWM_IS_GPIO, BIT2, 0);
    HAL_PWM_WriteRegBit(REG_PWM_IS_GPIO, BIT3, 0);
#endif
    ret = TRUE;
    return ret;
}
#ifdef CONFIG_NONPM_PWM
static void _HAL_PWM_VDBen_SW(PWM_ChNum index, MS_BOOL bSwitch)
{
	if(_gPWM_VDBen)
	{
		//printf("%s(0x%08X, %x)", __FUNCTION__, (int)index, bSwitch);
		switch(index)
    	        {
        	case E_PWM_CH0:
            	HAL_PWM_WriteByteMask(REG_PWM0_VDBEN_SW,BITS(6:6,bSwitch),BMASK(6:6));
            	break;
        	case E_PWM_CH1:
				HAL_PWM_WriteByteMask(REG_PWM1_VDBEN_SW,BITS(6:6,bSwitch),BMASK(6:6));
            	break;
        	case E_PWM_CH2:
				HAL_PWM_WriteByteMask(REG_PWM2_VDBEN_SW,BITS(6:6,bSwitch),BMASK(6:6));
            	break;
        	case E_PWM_CH3:
				HAL_PWM_WriteByteMask(REG_PWM3_VDBEN_SW,BITS(6:6,bSwitch),BMASK(6:6));
            	break;
        	case E_PWM_CH4:
				HAL_PWM_WriteByteMask(REG_PWM4_VDBEN_SW,BITS(6:6,bSwitch),BMASK(6:6));
            	break;
        	case E_PWM_CH5:
        	case E_PWM_CH6:
        	case E_PWM_CH7:
        	case E_PWM_CH8:
        	case E_PWM_CH9:
				printf("[Utopia] The PWM%d is not support\n", (int)index);
            	UNUSED(bSwitch);
            	break;
			default:
				break;
		}
	}
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_Oen
/// @brief \b Function  \b Description: Enable PWM or not.
/// @param <IN>         \b MS_U16 : index
/// @param <IN>         \b MS_BOOL : letch
/// @param <OUT>      \b None :
/// @param <RET>       \b MS_BOOL :
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Oen(PWM_ChNum index, MS_BOOL letch)
{
    MS_BOOL ret = FALSE;
#ifdef CONFIG_NONPM_PWM
    //Use the PWM oen in ChipTop Reg first, if it provides for.
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            if(HAL_PWM_WriteRegBit(REG_PWM0_OEN+1, BIT7, letch)&&   \
               HAL_PWM_WriteRegBit(REG_PWM_OEN, BIT0, letch))
               ret = HAL_PWM_WriteRegBit(REG_PWM_GRP0_CLK_GATE_EN, BIT0, letch);
            break;
        case E_PWM_CH1:
            if(HAL_PWM_WriteRegBit(REG_PWM1_OEN+1, BIT7, letch)&&   \
               HAL_PWM_WriteRegBit(REG_PWM_OEN, BIT1, letch))
               ret = HAL_PWM_WriteRegBit(REG_PWM_GRP0_CLK_GATE_EN, BIT0, letch);
            break;
        case E_PWM_CH2:
            if(HAL_PWM_WriteRegBit(REG_PWM2_OEN+1, BIT7, letch)&&  \
               HAL_PWM_WriteRegBit(REG_PWM_OEN, BIT2, letch))
               ret = HAL_PWM_WriteRegBit(REG_PWM_GRP0_CLK_GATE_EN, BIT0, letch);
            break;
        case E_PWM_CH3:
            if(HAL_PWM_WriteRegBit(REG_PWM3_OEN+1, BIT7, letch)&&   \
               HAL_PWM_WriteRegBit(REG_PWM_OEN, BIT3, letch))
               ret = HAL_PWM_WriteRegBit(REG_PWM_GRP1_CLK_GATE_EN, BIT1, letch);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_OEN+1, BIT7, letch);
            ret = HAL_PWM_WriteRegBit(REG_PWM_GRP1_CLK_GATE_EN, BIT1, letch);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_OEN+1, BIT7, letch);
            ret = HAL_PWM_WriteRegBit(REG_PWM_GRP1_CLK_GATE_EN, BIT1, letch);
            break;
        case E_PWM_CH6:
            UNUSED(letch);
            break;
        case E_PWM_CH7:
            UNUSED(letch);
            break;
        case E_PWM_CH8:
            UNUSED(letch);
            break;
        case E_PWM_CH9:
            UNUSED(letch);
            break;
    }
    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM OEN Setting\n");
#endif
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_GetOen
/// @brief \b Function  \b Description:Get Enable PWM Oen.
/// @param <IN>         \b MS_U16 : index
/// @param <IN>         \b MS_BOOL : letch
/// @param <OUT>      \b None :
/// @param <RET>       \b MS_BOOL :
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetOen(PWM_ChNum index)
{
#ifdef CONFIG_NONPM_PWM
    MS_BOOL letch;
    //Use the PWM oen in ChipTop Reg first, if it provides for.
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(0:0)) >> 0;
        case E_PWM_CH1:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(1:1)) >> 1;
            break;
        case E_PWM_CH2:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(2:2)) >> 2;
            break;
        case E_PWM_CH3:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(3:3)) >> 3;
            break;
        case E_PWM_CH4:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(4:4)) >> 4;
            break;
        case E_PWM_CH5:
            letch = (HAL_PWM_Read2Byte(REG_PWM5_OEN) & BMASK(15:15)) >> 15;
            break;
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;
    return letch;
#else
    printf("[Utopia] k6 is not support NONPM_PWM OEN Setting\n");
    return 0;
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_UnitDiv
/// @brief \b Function   \b Description : Set the Unit_Div of the pwm
/// @param <IN>          \b MS_U16 : the Unit_Div value
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL : 1 for doen; 0 for not done
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_UnitDiv(MS_U16 u16DivPWM)
{
    MS_BOOL ret = FALSE;
    /* the PWM clock unit divider is NO USE in T3 */
    UNUSED(u16DivPWM);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Period
/// @brief \b Function   \b Description : Set the period of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U32 : the 18-bit Period value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Period(PWM_ChNum index, MS_U32 u32PeriodPWM)
{
#ifdef CONFIG_NONPM_PWM
    MS_U16  Period_L, Period_H;
    MS_BOOL Ex1, Ex2;
    Period_L = (MS_U16)u32PeriodPWM;
    Period_H = (MS_U16)(u32PeriodPWM >> 16);

    Ex1 = (Period_H & BIT0) ? 1 : 0;
    Ex2 = (Period_H & BIT1) ? 1 : 0;

    HAL_SUBBANK1;
    /* the Period capability is restricted to ONLY 18-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM0_PERIOD_EXT, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM0_PERIOD_EXT, BIT1, Ex2);
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM1_PERIOD_EXT, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM1_PERIOD_EXT, BIT3, Ex2);
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM2_PERIOD_EXT, BIT4, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM2_PERIOD_EXT, BIT5, Ex2);
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM3_PERIOD_EXT, BIT6, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM3_PERIOD_EXT, BIT7, Ex2);
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM4_PERIOD_EXT+1, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM4_PERIOD_EXT+1, BIT1, Ex2);
            break;
        case E_PWM_CH5:
            HAL_PWM_Write2Byte(REG_PWM5_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM5_PERIOD_EXT+1, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM5_PERIOD_EXT+1, BIT3, Ex2);
            break;
        case E_PWM_CH6:
            HAL_PWM_Write2Byte(REG_PWM6_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM6_PERIOD_EXT, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM6_PERIOD_EXT, BIT1, Ex2);
            break;
        case E_PWM_CH7:
            HAL_PWM_Write2Byte(REG_PWM7_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM7_PERIOD_EXT, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM7_PERIOD_EXT, BIT3, Ex2);
            break;
        case E_PWM_CH8:
            HAL_PWM_Write2Byte(REG_PWM8_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM8_PERIOD_EXT, BIT4, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM8_PERIOD_EXT, BIT5, Ex2);
            break;
        case E_PWM_CH9:
            UNUSED(Period_L);
            UNUSED(Period_H);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM Period Setting\n");
#endif
}

MS_U32 HAL_PWM_GetPeriod(PWM_ChNum index)
{
    MS_U16  Period_L=0, Period_H=0;
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;
    _HAL_PWM_VDBen_SW(index,0);
    switch(index)
    {
        case E_PWM_CH0:
            Period_L = HAL_PWM_Read2Byte(REG_PWM0_PERIOD);
            Period_H = HAL_PWM_ReadByte(REG_PWM0_PERIOD_EXT) & BMASK(1:0);
            break;
        case E_PWM_CH1:
            Period_L = HAL_PWM_Read2Byte(REG_PWM1_PERIOD);
            Period_H = (HAL_PWM_ReadByte(REG_PWM1_PERIOD_EXT) & BMASK(3:2)) >> 2;
            break;
        case E_PWM_CH2:
            Period_L = HAL_PWM_Read2Byte(REG_PWM2_PERIOD);
            Period_H = (HAL_PWM_ReadByte(REG_PWM2_PERIOD_EXT) & BMASK(5:4)) >> 4;
            break;
        case E_PWM_CH3:
            Period_L = HAL_PWM_Read2Byte(REG_PWM3_PERIOD);
            Period_H = (HAL_PWM_ReadByte(REG_PWM3_PERIOD_EXT) & BMASK(7:6)) >> 6;
            break;
        case E_PWM_CH4:
            Period_L = HAL_PWM_Read2Byte(REG_PWM4_PERIOD);
            Period_H = (HAL_PWM_Read2Byte(REG_PWM4_PERIOD_EXT) & BMASK(9:8)) >> 8;
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
        printf("[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Period_L);
            UNUSED(Period_H);
            break;
        default:
            break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM GetPeriod Setting\n");
#endif
    return ((MS_U32)Period_H << 16 | Period_L);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_DutyCycle
/// @brief \b Function   \b Description : Set the Duty of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U32 : the 18-bit Duty value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_DutyCycle(PWM_ChNum index, MS_U32 u32DutyPWM)
{
#ifdef CONFIG_NONPM_PWM
    MS_U16  Duty_L, Duty_H;
    MS_BOOL Ex1, Ex2;

    Duty_L = (MS_U16)u32DutyPWM;
    Duty_H = (MS_U16)(u32DutyPWM >> 16);

    Ex1 = (Duty_H & BIT0) ? 1 : 0;
    Ex2 = (Duty_H & BIT1) ? 1 : 0;

    HAL_SUBBANK1;
    /* the Duty capability is restricted to ONLY 10-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM0_DUTY_EXT, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM0_DUTY_EXT, BIT1, Ex2);
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM1_DUTY_EXT, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM1_DUTY_EXT, BIT3, Ex2);
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM2_DUTY_EXT, BIT4, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM2_DUTY_EXT, BIT5, Ex2);
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM3_DUTY_EXT, BIT6, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM3_DUTY_EXT, BIT7, Ex2);
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM4_DUTY_EXT+1, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM4_DUTY_EXT+1, BIT1, Ex2);
            break;
        case E_PWM_CH5:
            HAL_PWM_Write2Byte(REG_PWM5_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM5_DUTY_EXT+1, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM5_DUTY_EXT+1, BIT3, Ex2);
            break;
        case E_PWM_CH6:
            HAL_PWM_Write2Byte(REG_PWM6_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM7_DUTY_EXT+1, BIT0, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM7_DUTY_EXT+1, BIT1, Ex2);
            break;
        case E_PWM_CH7:
            HAL_PWM_Write2Byte(REG_PWM7_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM7_DUTY_EXT+1, BIT2, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM7_DUTY_EXT+1, BIT3, Ex2);
            break;
        case E_PWM_CH8:
            HAL_PWM_Write2Byte(REG_PWM8_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM8_DUTY_EXT+1, BIT4, Ex1);
            HAL_PWM_WriteRegBit(REG_PWM8_DUTY_EXT+1, BIT5, Ex2);
            break;
        case E_PWM_CH9:
            UNUSED(Duty_L);
            UNUSED(Duty_H);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM DutyCycle Setting\n");
#endif
}

MS_U32 HAL_PWM_GetDutyCycle(PWM_ChNum index)
{
    MS_U16  Duty_L=0, Duty_H=0;

    //Duty_L = (MS_U16)u32DutyPWM;
    //Duty_H = (MS_U8)(u32DutyPWM >> 16);
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;
    _HAL_PWM_VDBen_SW(index,0);
    /* the Duty capability is restricted to ONLY 10-bit */
    switch(index)
    {
        case E_PWM_CH0:
            Duty_L = HAL_PWM_Read2Byte(REG_PWM0_DUTY);
            Duty_H = HAL_PWM_ReadByte(REG_PWM0_DUTY_EXT) & BMASK(1:0);
            break;
        case E_PWM_CH1:
            Duty_L = HAL_PWM_Read2Byte(REG_PWM1_DUTY);
            Duty_H = (HAL_PWM_ReadByte(REG_PWM1_DUTY_EXT) & BMASK(3:2)) >> 2;
            break;
        case E_PWM_CH2:
            Duty_L = HAL_PWM_Read2Byte(REG_PWM2_DUTY);
            Duty_H = (HAL_PWM_ReadByte(REG_PWM2_DUTY_EXT) & BMASK(5:4)) >> 4;
            break;
        case E_PWM_CH3:
            Duty_L = HAL_PWM_Read2Byte(REG_PWM3_DUTY);
            Duty_H = (HAL_PWM_ReadByte(REG_PWM3_DUTY_EXT) & BMASK(7:6)) >> 6;
            break;
        case E_PWM_CH4:
            Duty_L = HAL_PWM_Read2Byte(REG_PWM4_DUTY);
            Duty_H = (HAL_PWM_Read2Byte(REG_PWM4_DUTY_EXT) & BMASK(9:8)) >> 8;
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Duty_L);
            UNUSED(Duty_H);
            break;
        default:
            break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM GetDutyCycle\n");
#endif
    return ((MS_U32)Duty_H << 16 | Duty_L);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Div
/// @brief \b Function   \b Description : Set the Div of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U16 : the 16-bit Div value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Div(PWM_ChNum index, MS_U16 u16DivPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;
    /* the Div capability is restricted to ONLY 16-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM0_DIV, u16DivPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM1_DIV, u16DivPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM2_DIV, u16DivPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM3_DIV, u16DivPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM4_DIV, u16DivPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_Write2Byte(REG_PWM5_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM5_DIV, u16DivPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_Write2Byte(REG_PWM6_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM6_DIV, u16DivPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_Write2Byte(REG_PWM7_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM7_DIV, u16DivPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_Write2Byte(REG_PWM8_DIV, 0x0000);
            HAL_PWM_Write2Byte(REG_PWM8_DIV, u16DivPWM);
            break;
        case E_PWM_CH9:
            UNUSED(u16DivPWM);
            break;
    }

    HAL_SUBBANK0;
#else
     printf("[Utopia] k6 is not support NONPM_PWM DIV Setting\n");
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetDiv
/// @brief \b Function   \b Description :Get the Div of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_U16 : the 16-bit Div value
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_PWM_GetDiv(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
	MS_U8 u16DivPWM = 0;

    HAL_SUBBANK1;
    /* the Div capability is restricted to ONLY 16-bit */
    switch(index)
    {
        case E_PWM_CH0:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM0_DIV);
            break;
        case E_PWM_CH1:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM1_DIV);
            break;
        case E_PWM_CH2:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM2_DIV);
            break;
        case E_PWM_CH3:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM3_DIV);
            break;
        case E_PWM_CH4:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM4_DIV);
            break;
        case E_PWM_CH5:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM5_DIV);
            break;
        case E_PWM_CH6:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM6_DIV);
            break;
        case E_PWM_CH7:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM7_DIV);
            break;
        case E_PWM_CH8:
            u16DivPWM = HAL_PWM_ReadByte(REG_PWM8_DIV);
            break;
        case E_PWM_CH9:
	        printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }
    HAL_SUBBANK0;

    return u16DivPWM;
	#else
    printf("[Utopia] k6 is not support NONPM_PWM DIV Setting\n");
    return 0;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Polarity
/// @brief \b Function   \b Description : Set the Polarity of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Polarity(PWM_ChNum index, MS_BOOL bPolPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_PORARITY+1, BIT0, bPolPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bPolPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM Polarity Setting\n");
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetPolarity
/// @brief \b Function   \b Description : Get the Polarity of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL : 1 for enable; 0 for disable
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetPolarity(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
    MS_BOOL bPolarity = FALSE;
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM0_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH1:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM1_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH2:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM2_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH3:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM3_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH4:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM4_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH5:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM5_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH6:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM6_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH7:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM7_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH8:
            bPolarity = (HAL_PWM_Read2Byte(REG_PWM8_PORARITY) & BMASK(8:8)) >> 8;
            break;
        case E_PWM_CH9:
	        printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }
    HAL_SUBBANK0;

    return bPolarity;
    #else
    printf("[Utopia] k6 is not support NONPM_PWM Polarity Setting\n");
    return 0;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_VDBen
/// @brief \b Function   \b Description : Set the Vsync Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_VDBen(PWM_ChNum index, MS_BOOL bVdbenPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_VDBEN+1, BIT1, bVdbenPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_VDBEN+1, BIT0, bVdbenPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bVdbenPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM VDBen Setting\n");
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetVDBen
/// @brief \b Function   \b Description :Get the Vsync Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL : 1 for enable; 0 for disable
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetVDBen(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
    MS_BOOL bVdbenPWM = FALSE;

    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            bVdbenPWM = (HAL_PWM_Read2Byte(REG_PWM0_VDBEN) & BMASK(9:9)) >> 9;
            break;
        case E_PWM_CH1:
            bVdbenPWM = (HAL_PWM_Read2Byte(REG_PWM1_VDBEN) & BMASK(9:9)) >> 9;
            break;
        case E_PWM_CH2:
            bVdbenPWM = (HAL_PWM_Read2Byte(REG_PWM2_VDBEN) & BMASK(9:9)) >> 9;
            break;
        case E_PWM_CH3:
            bVdbenPWM = (HAL_PWM_Read2Byte(REG_PWM3_VDBEN) & BMASK(9:9)) >> 9;
            break;
        case E_PWM_CH4:
            bVdbenPWM = (HAL_PWM_Read2Byte(REG_PWM4_VDBEN) & BMASK(9:9)) >> 9;
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	        printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
    return bVdbenPWM;
    #else
    printf("[Utopia] k6 is not support NONPM_PWM VDBen Setting\n");
    return FALSE;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Vrest
/// @brief \b Function   \b Description : Set the Hsync reset of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Vrest(PWM_ChNum index, MS_BOOL bRstPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_RESET_EN+1, BIT2, bRstPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bRstPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM Vsync rest Setting\n");
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetVrest
/// @brief \b Function   \b Description :Get the Hsync reset of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_BOOL : 1 for enable; 0 for disable
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetVrest(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
    MS_BOOL bRstPWM;
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM0_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH1:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM1_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH2:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM2_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH3:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM3_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH4:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM4_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH5:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM5_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH6:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM6_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH7:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM7_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH8:
            bRstPWM = (HAL_PWM_Read2Byte(REG_PWM8_RESET_EN) & BMASK(10:10)) >> 10;
            break;
        case E_PWM_CH9:
	        printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
    return bRstPWM;
    #else
    printf("[Utopia] k6 is not support NONPM_PWM Vsync rest Setting\n");
    return 0;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_DBen
/// @brief \b Function   \b Description : Set the Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_DBen(PWM_ChNum index, MS_BOOL bdbenPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_DBEN+1, BIT3, bdbenPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bdbenPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM DBEN Setting\n");
#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetDBen
/// @brief \b Function   \b Description : Get the Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetDBen(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
    MS_BOOL bdbenPWM;
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM0_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH1:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM1_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH2:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM2_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH3:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM3_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH4:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM4_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH5:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM5_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH6:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM6_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH7:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM7_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH8:
            bdbenPWM = (HAL_PWM_Read2Byte(REG_PWM8_DBEN) & BMASK(11:11)) >> 11;
            break;
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;

    return bdbenPWM;
	#else
    printf("[Utopia] k6 is not support NONPM_PWM DBEN Setting\n");
    return 0;
    #endif
}

void HAL_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM)
{
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            //HAL_PWM_WriteRegBit(REG_PWM0_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH1:
            //HAL_PWM_WriteRegBit(REG_PWM1_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH2:
            //HAL_PWM_WriteRegBit(REG_PWM2_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH3:
            //HAL_PWM_WriteRegBit(REG_PWM3_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH4:
            //HAL_PWM_WriteRegBit(REG_PWM4_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH5:
            //HAL_PWM_WriteRegBit(REG_PWM5_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH6:
            //HAL_PWM_WriteRegBit(REG_PWM6_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH7:
            //HAL_PWM_WriteRegBit(REG_PWM7_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH8:
            //HAL_PWM_WriteRegBit(REG_PWM8_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH9:
            UNUSED(bdbenPWM);
            break;
    }

    HAL_SUBBANK0;
}


void HAL_PWM_ODDEVEN_SYNC(PWM_ChNum index, MS_BOOL bdbenPWM)
{
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            //HAL_PWM_WriteRegBit(REG_PWM0_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH1:
            //HAL_PWM_WriteRegBit(REG_PWM1_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH2:
            //HAL_PWM_WriteRegBit(REG_PWM2_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH3:
            //HAL_PWM_WriteRegBit(REG_PWM3_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH4:
            //HAL_PWM_WriteRegBit(REG_PWM4_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH5:
            //HAL_PWM_WriteRegBit(REG_PWM5_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH6:
            //HAL_PWM_WriteRegBit(REG_PWM6_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH7:
            //HAL_PWM_WriteRegBit(REG_PWM7_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH8:
            //HAL_PWM_WriteRegBit(REG_PWM8_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH9:
            UNUSED(bdbenPWM);
            break;
    }

    HAL_SUBBANK0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_RstMux
/// @brief \b Function   \b Description : Set the Rst Mux of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for Hsync; 0 for Vsync
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_RstMux(PWM_ChNum index, MS_BOOL bMuxPWM)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_RST_MUX0+1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_RST_MUX1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_RST_MUX2+1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_RST_MUX3, BIT7, bMuxPWM);
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_RST_MUX4+1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_RST_MUX5, BIT7, bMuxPWM);
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_RST_MUX6+1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_RST_MUX7, BIT7, bMuxPWM);
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_RST_MUX8+1, BIT7, bMuxPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bMuxPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM RstMux Setting\n");
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_RstCnt
/// @brief \b Function   \b Description : Set the Rst_Cnt of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U8 : u8RstCntPWM
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_RstCnt(PWM_ChNum index, MS_U8 u8RstCntPWM)
{
#ifdef CONFIG_NONPM_PWM
    MS_U8 RstCntMask = 0x00;
    HAL_SUBBANK1;
    /* the Hsync reset counter capability is restricted to ONLY 4-bit */
    switch(index)
    {
        case E_PWM_CH0:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT0+1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT0+1, RstCntMask);
            break;
        case E_PWM_CH1:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT1, RstCntMask);
            break;
        case E_PWM_CH2:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT2+1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT2+1, RstCntMask);
            break;
        case E_PWM_CH3:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT3)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT3, RstCntMask);
            break;
        case E_PWM_CH4:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT4+1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT4+1, RstCntMask);
            break;
        case E_PWM_CH5:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT5)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT5, RstCntMask);
            break;
        case E_PWM_CH6:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT6+1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT6+1, RstCntMask);
            break;
        case E_PWM_CH7:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT7)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT7, RstCntMask);
            break;
        case E_PWM_CH8:
            RstCntMask = ((HAL_PWM_ReadByte(REG_HS_RST_CNT8+1)&0xF0)|u8RstCntPWM);
            HAL_PWM_WriteByte(REG_HS_RST_CNT8+1, RstCntMask);
            break;
        case E_PWM_CH9:
            UNUSED(u8RstCntPWM);
            break;
    }

    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM RstCnt Setting\n");
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_BypassUnit
/// @brief \b Function   \b Description : Set the Bypass Unit of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_BypassUnit(PWM_ChNum index, MS_BOOL bBypassPWM)
{
    HAL_SUBBANK1;

    //T2 ONLY

    switch(index)
    {
        case E_PWM_CH0:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH1:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH2:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH3:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH4:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH5:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH6:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH7:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH8:
            UNUSED(bBypassPWM);
            break;
        case E_PWM_CH9:
            UNUSED(bBypassPWM);
            break;
    }

    HAL_SUBBANK0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM01_CntMode
/// @brief \b Function   \b Description :  Counter mode for PWM0 and PWM1
/// @param <IN>          \b PWM_ChNum : Enum of the Counter mode
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b PWM_Result :
/// @param <GLOBAL>   \b None :
/// @note                                                       \n
///     11: PWM1 donate internal divider to PWM0   \n
///     10: PWM0 donate internal divider to PWM1   \n
///     0x: Normal mode                                      \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM01_CntMode(PWM_CntMode CntMode)
{
    MS_BOOL ret = FALSE;
    //T2 ONLY
    UNUSED(CntMode);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM23_CntMode
/// @brief \b Function   \b Description :  Counter mode for PWM2 and PWM3
/// @param <IN>          \b PWM_ChNum : Enum of the Counter mode
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b PWM_Result :
/// @param <GLOBAL>   \b None :
/// @note                                                       \n
///     11: PWM3 donate internal divider to PWM2   \n
///     10: PWM2 donate internal divider to PWM3   \n
///     0x: Normal mode                                      \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM23_CntMode(PWM_CntMode CntMode)
{
    MS_BOOL ret = FALSE;
    //T2 ONLY
    UNUSED(CntMode);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM67_CntMode
/// @brief \b Function   \b Description :  Counter mode for PWM6 and PWM7
/// @param <IN>          \b PWM_ChNum : Enum of the Counter mode
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b PWM_Result :
/// @param <GLOBAL>   \b None :
/// @note                                                       \n
///     11: PWM7 donate internal divider to PWM6   \n
///     10: PWM6 donate internal divider to PWM7   \n
///     0x: Normal mode                                      \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM67_CntMode(PWM_CntMode CntMode)
{
    MS_BOOL ret = FALSE;
    //T2 ONLY
    UNUSED(CntMode);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Shift
/// @brief \b Function   \b Description : Set the Shift of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U16 : the 18-bit shift value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Shift(PWM_ChNum index, MS_U32 u32ShiftPWM)
{
#ifdef CONFIG_NONPM_PWM
    MS_BOOL ret0 = FALSE, ret1 = FALSE, Ret = FALSE;
    MS_U16 Shift_L, Shift_H;

    Shift_L = (MS_U16)u32ShiftPWM;
    Shift_H = (MS_U16)(u32ShiftPWM >> 16);
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            ret0 = HAL_PWM_Write2Byte(REG_PWM0_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM0_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH1:
            ret0 = HAL_PWM_Write2Byte(REG_PWM1_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM1_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH2:
            ret0 = HAL_PWM_Write2Byte(REG_PWM2_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM2_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH3:
            ret0 = HAL_PWM_Write2Byte(REG_PWM3_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM3_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH4:
            ret0 = HAL_PWM_Write2Byte(REG_PWM4_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM4_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH5:
            ret0 = HAL_PWM_Write2Byte(REG_PWM5_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM5_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH6:
            ret0 = HAL_PWM_Write2Byte(REG_PWM6_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM6_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH7:
            ret0 = HAL_PWM_Write2Byte(REG_PWM7_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM7_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH8:
            ret0 = HAL_PWM_Write2Byte(REG_PWM8_SHIFT_L, Shift_L);
            ret1 = HAL_PWM_Write2Byte(REG_PWM8_SHIFT_H, Shift_H);
            Ret = (ret0 & ret1);
            break;
        case E_PWM_CH9:
            UNUSED(Shift_L);UNUSED(Shift_H);
            break;
    }

    HAL_SUBBANK0;
#else
    MS_BOOL Ret = FALSE;
    printf("[Utopia] k6 is not support NONPM_PWM Shift Setting\n");
#endif
    return Ret;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_GetShift
/// @brief \b Function   \b Description : Set the Shift of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b MS_U16 : the 18-bit shift value
/// @param <OUT>       \b None :
/// @param <RET>        \b MS_U16 : the 18-bit shift value
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_PWM_GetShift(PWM_ChNum index)
{
    #ifdef CONFIG_NONPM_PWM
    MS_U16 Shift_L, Shift_H;
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM0_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM0_SHIFT_H);
            break;
        case E_PWM_CH1:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM1_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM1_SHIFT_H);
            break;
        case E_PWM_CH2:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM2_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM2_SHIFT_H);
            break;
        case E_PWM_CH3:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM3_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM3_SHIFT_H);
            break;
        case E_PWM_CH4:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM4_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM4_SHIFT_H);
            break;
        case E_PWM_CH5:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM5_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM5_SHIFT_H);
            break;
        case E_PWM_CH6:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM6_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM6_SHIFT_H);
            break;
        case E_PWM_CH7:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM7_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM7_SHIFT_H);
            break;
        case E_PWM_CH8:
            Shift_L = HAL_PWM_Read2Byte(REG_PWM8_SHIFT_L);
            Shift_H = HAL_PWM_Read2Byte(REG_PWM8_SHIFT_H);
            break;
        case E_PWM_CH9:
			printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;

    return ((Shift_H<<16) | Shift_L);
    #else
    printf("[Utopia] k6 is not support NONPM_PWM Shift Setting\n");
    return 0;
    #endif
}


void HAL_PWM_Nvsync(PWM_ChNum index, MS_BOOL bNvsPWM)
{
     printf("[Utopia] k6 is not support PWM_Nvsync\n");
}

void HAL_PWM_Align(PWM_ChNum index, MS_BOOL bAliPWM)
{
     printf("[Utopia] k6 is not support PWM_Align\n");
}

PWM_3D_RegisterOffset g_ArrayPWM3D_RegisterOffset[PWM_Num][MAX_3DPWM_NUM] =
{
    { //PWM0
        {//PWM0_Waveform_0
            {0,  0}, //PWM0_Waveform_0_Shift  ==> use: HAL_PWM_Shift()
            {0,  0}, //PWM0_Waveform_0_duty  ==> use: HAL_PWM_DutyCycle()
        },
        {//PWM0_Waveform_1
            {REG_PWM0_HIT_CNT_ST,    16}, //PWM0_Waveform_1_Shift
            {REG_PWM0_HIT_CNT_END,   16}, //PWM0_Waveform_1_duty
        },
        {//PWM0_Waveform_2
            {REG_PWM0_HIT_CNT_ST2,    16}, //PWM0_Waveform_2_Shift
            {REG_PWM0_HIT_CNT_END2,   16}, //PWM0_Waveform_2_duty
        },
        {//PWM0_Waveform_3
            {REG_PWM0_SHIFT4,    16}, //PWM0_Waveform_3_Shift
            {REG_PWM0_DUTY4,   16}, //PWM0_Waveform_3_duty
        },
    },

    { //PWM1
        {//PWM1_Waveform_0
            {0,  0}, //PWM1_Waveform_0_Shift  ==> use: HAL_PWM_Shift()
            {0,  0}, //PWM1_Waveform_0_duty  ==> use: HAL_PWM_DutyCycle()
        },
        {//PWM1_Waveform_1
            {REG_PWM1_HIT_CNT_ST,    16}, //PWM1_Waveform_1_Shift
            {REG_PWM1_HIT_CNT_END,   16}, //PWM1_Waveform_1_duty
        },
        {//PWM1_Waveform_2
            {REG_PWM1_HIT_CNT_ST2,    16}, //PWM1_Waveform_2_Shift
            {REG_PWM1_HIT_CNT_END2,   16}, //PWM1_Waveform_2_duty
        },
        {//PWM1_Waveform_3
            {REG_PWM1_SHIFT4,    16}, //PWM1_Waveform_3_Shift
            {REG_PWM1_DUTY4,   16}, //PWM1_Waveform_3_duty
        },
    },
};

MS_BOOL HAL_PWM_IsSupport3D(PWM_ChNum index)
{
    //Only PWM0 & PWM1 Support 3D Function
    switch (index)
    {
        case E_PWM_CH0:
        case E_PWM_CH1:
            return TRUE;

        default:
            return FALSE;
    }
}

void HAL_PWM_SetMultiDiff(MS_BOOL bEnable)
{
#ifdef CONFIG_NONPM_PWM
    HAL_SUBBANK1;
    HAL_PWM_WriteRegBit(REG_PWM_MULTI_DIFF, PWM_MULTI_DIEF_EN, bEnable);
    HAL_SUBBANK0;
#else
    printf("[Utopia] k6 is not support NONPM_PWM SetMultiDiff\n");
#endif
}

MS_BOOL HAL_PWM_Set3D_DiffWaveform(PWM_ChNum index, MS_U8 u8WaveformIndex, MS_U32 u32Shift, MS_U32 u32Duty)
{
    MS_BOOL bReturn = FALSE;
#ifdef CONFIG_NONPM_PWM
    PWM_3D_RegisterOffset *pReigsterOffset = (PWM_3D_RegisterOffset *)&g_ArrayPWM3D_RegisterOffset[index][u8WaveformIndex];

    if (u8WaveformIndex == 0)
    {
        bReturn &= HAL_PWM_Shift(index, u32Shift);
        HAL_PWM_DutyCycle(index, u32Duty);
    }
    else
    {
        HAL_SUBBANK1;
        bReturn &= HAL_PWM_WriteNumberByte(pReigsterOffset->regShift.u32RegOffset, u32Shift, pReigsterOffset->regShift.u8NumBit);
        bReturn &= HAL_PWM_WriteNumberByte(pReigsterOffset->regDuty.u32RegOffset, u32Duty, pReigsterOffset->regDuty.u8NumBit);
        HAL_SUBBANK0
    }
#else
     printf("[Utopia] k6 is not support NONPM_PWM Set3D\n");
#endif
    return bReturn;
}



//---------------------------PM Base--------------------------------//

void HAL_PM_PWM_Enable(void)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

void HAL_PM_PWM_Period(MS_U16 u16PeriodPWM)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

void HAL_PM_PWM_DutyCycle(MS_U16 u16DutyPWM)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

void HAL_PM_PWM_Div(MS_U8 u8DivPWM)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

void HAL_PM_PWM_Polarity(MS_BOOL bPolPWM)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

void HAL_PM_PWM_DBen(MS_BOOL bdbenPWM)
{
     printf("[Utopia] k1 is not support PM_PWM\n");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_INV_3D_Flag
/// @brief \b Function   \b Description : Set Inverse 3D flag
/// @param <IN>          \b MS_BOOL : 1 for Enable; 0 for Disable
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_INV_3D_Flag(MS_BOOL bInvPWM)
{
	MS_BOOL ret = FALSE;
    HAL_SUBBANK1;
	HAL_PWM_WriteByteMask(REG_INV_3D_FLAG, (bInvPWM?BIT7:0), BIT7);
    ret = true;    
    HAL_SUBBANK0;
    return ret; 
}	
	
	
void HAL_PWM_LR_RST_SEL(PWM_ChNum index, N_LR_SYNC_SEL eLR_Sync)
{

}