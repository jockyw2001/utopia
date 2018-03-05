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
#include "ULog.h"

////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////

static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_PM_MapBase = 0;
static MS_U16 _gPWM_Status  = 0;

//static MS_BOOL _gPWM_DBen   = 0;
static MS_BOOL _gPWM_VDBen  = 0;

static void _HAL_PWM_VDBen_SW(PWM_ChNum index, MS_BOOL bSwitch);
////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }

#define HAL_PWM_ReadByte(addr)		     READ_BYTE((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2))
#define HAL_PWM_Read2Byte(addr)              READ_WORD((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2))

#define HAL_PWM_WriteByte(addr, val) 	     WRITE_BYTE((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_Write2Byte(addr, val)        WRITE_WORD((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val), (mask))

#define HAL_TOP_ReadByte(addr)		     READ_BYTE((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2))
#define HAL_TOP_Read2Byte(addr)              READ_WORD((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2))
#define HAL_TOP_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2), (val), (mask))

#define HAL_PM_WriteByte(addr, val) 	     WRITE_BYTE((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val))
#define HAL_PM_Write2Byte(addr, val)        WRITE_WORD((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val))
#define HAL_PM_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val), (mask))

#define HAL_SUBBANK0    //HAL_PWM_WriteByte(0,0)
#define HAL_SUBBANK1    //HAL_PWM_WriteByte(0,1) /* PWM sub-bank */

#define TAG_PWM "PWM"

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    HAL_PWM_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_PWM_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

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

static void _HAL_PWM_VDBen_SW(PWM_ChNum index, MS_BOOL bSwitch)
{
	if(_gPWM_VDBen)
	{
		//ULOGE(TAG_PWM, "%s(0x%08X, %x)", __FUNCTION__, (int)index, bSwitch);
		switch(index)
    	        {
        	case E_PWM_CH0:
            	HAL_PWM_WriteRegBit(REG_PWM0_VDBEN_SW,BITS(14:14,bSwitch),BMASK(14:14));
            	break;
        	case E_PWM_CH1:
				HAL_PWM_WriteRegBit(REG_PWM1_VDBEN_SW,BITS(14:14,bSwitch),BMASK(14:14));
            	break;
        	case E_PWM_CH2:
				HAL_PWM_WriteRegBit(REG_PWM2_VDBEN_SW,BITS(14:14,bSwitch),BMASK(14:14));
            	break;
        	case E_PWM_CH3:
				HAL_PWM_WriteRegBit(REG_PWM3_VDBEN_SW,BITS(14:14,bSwitch),BMASK(14:14));
            	break;
        	case E_PWM_CH4:
				HAL_PWM_WriteRegBit(REG_PWM4_VDBEN_SW,BITS(14:14,bSwitch),BMASK(14:14));
            	break;
        	case E_PWM_CH5:
        	case E_PWM_CH6:
        	case E_PWM_CH7:
        	case E_PWM_CH8:
        	case E_PWM_CH9:
				ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            	UNUSED(bSwitch);
            	break;
			default:
				break;
		}
	}
}

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
	_gPWM_Status = HAL_TOP_Read2Byte(REG_PWM_MODE);

	if(_gPWM_Status&PAD_PWM0_OUT)
	{
		ULOGD(TAG_PWM, "Init PWM0\n");
	}
	if(_gPWM_Status&PAD_PWM1_OUT)
	{
		ULOGD(TAG_PWM, "Init PWM1\n");
	}
	if(_gPWM_Status&PAD_PWM2_OUT)
	{
		ULOGD(TAG_PWM, "Init PWM2\n");
	}
	if(_gPWM_Status&PAD_PWM3_OUT)
	{
		ULOGD(TAG_PWM, "Init PWM3\n");
	}
	if(_gPWM_Status&PAD_PWM4_OUT)
	{
		ULOGD(TAG_PWM, "Init PWM4\n");
	}
    ret = TRUE;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_Oen
/// @brief \b Function  \b Description: Switch PWM PAD as Output or Input
/// @param <IN>         \b MS_U16 : index
/// @param <IN>         \b MS_BOOL : letch, 1 for Input; 0 for Output
/// @param <OUT>      \b None :
/// @param <RET>       \b MS_BOOL :
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_Oen(PWM_ChNum index, MS_BOOL letch)
{

    //Use the PWM oen in ChipTop Reg first, if it provides for.
    
    switch(index)
    {
        case E_PWM_CH0:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(0:0,letch), BMASK(0:0));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(2:2,1), BMASK(2:2));	
            break;
        case E_PWM_CH1:
			HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(1:1,letch), BMASK(1:1));
			HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(6:6,1), BMASK(6:6));
            break;
        case E_PWM_CH2:
			HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(2:2,letch), BMASK(2:2));
			HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(7:7,1), BMASK(7:7));
            break;
        case E_PWM_CH3:
			HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(3:3,letch), BMASK(3:3));
			HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(12:12,1), BMASK(12:12));
            break;
        case E_PWM_CH4:
			HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(4:4,letch), BMASK(4:4));
			HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(13:13,1), BMASK(13:13));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
			ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(letch);
            break;
		default:
			break;
    }
   
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_GetOen
/// @brief \b Function  \b Description: Switch PWM PAD as Output or Input
/// @param <IN>         \b MS_U16 : index
/// @param <OUT>      \b None :
/// @param <RET>       \b MS_BOOL : letch, 1 for Input; 0 for Output
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PWM_GetOen(PWM_ChNum index)
{
    MS_BOOL letch = FALSE;
    //Use the PWM oen in ChipTop Reg first, if it provides for.
    switch(index)
    {
        case E_PWM_CH0:
            letch = (HAL_PWM_Read2Byte(REG_PWM_OEN) & BMASK(0:0)) >> 0;
            break;
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
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM,"[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    return letch;
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
    ULOGE(TAG_PWM, "[Utopia] T8 is not support\n");
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
    MS_U16  Period_L, Period_H;
    Period_L = (MS_U16)u32PeriodPWM;
    Period_H = (MS_U16)(u32PeriodPWM >> 16);

    HAL_SUBBANK1;
    _HAL_PWM_VDBen_SW(index,0);
    /* the Period capability is restricted to ONLY 18-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM0_PERIOD_EXT,BITS(1:0,Period_H),BMASK(1:0));
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM1_PERIOD_EXT,BITS(3:2,Period_H),BMASK(3:2));
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM2_PERIOD_EXT,BITS(5:4,Period_H),BMASK(5:4));
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM3_PERIOD_EXT,BITS(7:6,Period_H),BMASK(7:6));
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM4_PERIOD_EXT,BITS(9:8,Period_H),BMASK(9:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Period_L);
            UNUSED(Period_H);
            break;
		default:
			break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
}

MS_U32 HAL_PWM_GetPeriod(PWM_ChNum index)
{
    MS_U16  Period_L=0, Period_H=0;
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
        ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Period_L);
            UNUSED(Period_H);
            break;
        default:
            break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
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
    MS_U16  Duty_L, Duty_H;

    Duty_L = (MS_U16)u32DutyPWM;
    Duty_H = (MS_U8)(u32DutyPWM >> 16);

    HAL_SUBBANK1;
    _HAL_PWM_VDBen_SW(index,0);
    /* the Duty capability is restricted to ONLY 10-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM0_DUTY_EXT,BITS(1:0,Duty_H),BMASK(1:0));
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM1_DUTY_EXT,BITS(3:2,Duty_H),BMASK(3:2));
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_DUTY, Duty_L);
	    HAL_PWM_WriteRegBit(REG_PWM2_DUTY_EXT,BITS(5:4,Duty_H),BMASK(5:4));
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM3_DUTY_EXT,BITS(7:6,Duty_H),BMASK(7:6));
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM4_DUTY_EXT,BITS(9:8,Duty_H),BMASK(9:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
			ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Duty_L);
            UNUSED(Duty_H);
            break;
		default:
			break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
}

MS_U32 HAL_PWM_GetDutyCycle(PWM_ChNum index)
{
    MS_U16  Duty_L=0, Duty_H=0;

    //Duty_L = (MS_U16)u32DutyPWM;
    //Duty_H = (MS_U8)(u32DutyPWM >> 16);

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
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Duty_L);
            UNUSED(Duty_H);
            break;
        default:
            break;
    }
    _HAL_PWM_VDBen_SW(index,1);
    HAL_SUBBANK0;
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
	MS_U8 u8DivPWM[2] = {0};

	u8DivPWM[0] = (MS_U8)(u16DivPWM&0xFF);
	u8DivPWM[1] = (MS_U8)(u16DivPWM >> 8);

    HAL_SUBBANK1;
    /* the Div capability is restricted to ONLY 16-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteByte(REG_PWM0_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM0_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteByte(REG_PWM1_DIV,     u8DivPWM[0]);
            //HAL_PWM_WriteByte(REG_PWM1_DIV_EXT+1, u8DivPWM[1]);
			HAL_PWM_WriteRegBit(REG_PWM1_DIV_EXT,u16DivPWM,BMASK(15:8));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteByte(REG_PWM2_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM2_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteByte(REG_PWM3_DIV,     u8DivPWM[0]);
            //HAL_PWM_WriteByte(REG_PWM3_DIV_EXT+1, u8DivPWM[1]);
            HAL_PWM_WriteRegBit(REG_PWM3_DIV_EXT,u16DivPWM,BMASK(15:8));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteByte(REG_PWM4_DIV, 	u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM4_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(u16DivPWM);
            break;
		default:
			break;
    }
    HAL_SUBBANK0;
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
	MS_U8 u8DivPWM[2] = {0};

    HAL_SUBBANK1;
    /* the Div capability is restricted to ONLY 16-bit */
    switch(index)
    {
        case E_PWM_CH0:
            u8DivPWM[0] = HAL_PWM_ReadByte(REG_PWM0_DIV);
            u8DivPWM[1] = HAL_PWM_ReadByte(REG_PWM0_DIV_EXT);
            break;
        case E_PWM_CH1:
            u8DivPWM[0] = HAL_PWM_ReadByte(REG_PWM1_DIV);
            u8DivPWM[1] = ((HAL_PWM_Read2Byte(REG_PWM1_DIV_EXT) & BMASK(15:8)) >> 8)&0xFF;
            break;
        case E_PWM_CH2:
            u8DivPWM[0] = HAL_PWM_ReadByte(REG_PWM2_DIV);
            u8DivPWM[1] = HAL_PWM_ReadByte(REG_PWM2_DIV_EXT);
            break;
        case E_PWM_CH3:
            u8DivPWM[0] = HAL_PWM_ReadByte(REG_PWM3_DIV);
            u8DivPWM[1] = ((HAL_PWM_Read2Byte(REG_PWM3_DIV_EXT) & BMASK(15:8)) >> 8)&0xFF;
            break;
        case E_PWM_CH4:
            u8DivPWM[0] = HAL_PWM_ReadByte(REG_PWM4_DIV);
            u8DivPWM[1] = HAL_PWM_ReadByte(REG_PWM4_DIV_EXT);
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;

    return ((u8DivPWM[1]<<8) | u8DivPWM[0]);
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
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bPolPWM);
            break;
		default:
			break;
    }
    HAL_SUBBANK0;
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
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;

    return bPolarity;
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
    _gPWM_VDBen = bVdbenPWM;

    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bVdbenPWM);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
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
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }

    HAL_SUBBANK0;
    return bVdbenPWM;
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
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_RESET_EN,BITS(10:10,bRstPWM),BMASK(10:10));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_RESET_EN,BITS(10:10,bRstPWM),BMASK(10:10));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_RESET_EN,BITS(10:10,bRstPWM),BMASK(10:10));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_RESET_EN,BITS(10:10,bRstPWM),BMASK(10:10));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_RESET_EN,BITS(10:10,bRstPWM),BMASK(10:10));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bRstPWM);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
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
    MS_BOOL bRstPWM = FALSE;

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
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }

    HAL_SUBBANK0;
    return bRstPWM;
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
	//_gPWM_DBen = bdbenPWM;

    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bdbenPWM);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
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
    MS_BOOL bdbenPWM = FALSE;

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
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }

    HAL_SUBBANK0;

    return bdbenPWM;
}

void HAL_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM)
{
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_IMPULSE_EN,BITS(12:12,bdbenPWM),BMASK(12:12));
            break;
        case E_PWM_CH9:
            UNUSED(bdbenPWM);
            break;
		default:
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
            HAL_PWM_WriteRegBit(REG_PWM0_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH5:
            HAL_PWM_WriteRegBit(REG_PWM5_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH6:
            HAL_PWM_WriteRegBit(REG_PWM6_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH7:
            HAL_PWM_WriteRegBit(REG_PWM7_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH8:
            HAL_PWM_WriteRegBit(REG_PWM8_ODDEVEN_SYNC,BITS(13:13,bdbenPWM),BMASK(13:13));
            break;
        case E_PWM_CH9:
            UNUSED(bdbenPWM);
            break;
		default:
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
    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_RST_MUX0,BITS(15:15,bMuxPWM),BMASK(15:15));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_RST_MUX1,BITS(7:7,bMuxPWM),BMASK(7:7));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_RST_MUX2,BITS(15:15,bMuxPWM),BMASK(15:15));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_RST_MUX3,BITS(7:7,bMuxPWM),BMASK(7:7));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_RST_MUX4,BITS(15:15,bMuxPWM),BMASK(15:15));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bMuxPWM);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
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
	if( u8RstCntPWM & 0x10 )
	{
		ULOGE(TAG_PWM, "PWM%d Reset Count is too large\n", index);
	}

    HAL_SUBBANK1;
    /* the Hsync reset counter capability is restricted to ONLY 4-bit */
    switch(index)
    {
        case E_PWM_CH0:
	    HAL_PWM_WriteRegBit(REG_HS_RST_CNT0,BITS(11:8,u8RstCntPWM),BMASK(11:8));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_HS_RST_CNT1,BITS(3:0,u8RstCntPWM),BMASK(3:0));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_HS_RST_CNT2,BITS(11:8,u8RstCntPWM),BMASK(11:8));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_HS_RST_CNT3,BITS(3:0,u8RstCntPWM),BMASK(3:0));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_HS_RST_CNT4,BITS(11:8,u8RstCntPWM),BMASK(11:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
	    ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(u8RstCntPWM);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;
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
        case E_PWM_CH1:
        case E_PWM_CH2:
        case E_PWM_CH3:
        case E_PWM_CH4:
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
			ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(bBypassPWM);
            break;
		default:
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
    MS_U16 Shift_L, Shift_H;

    Shift_L = (MS_U16)(u32ShiftPWM & 0xFFFF);
    Shift_H = (MS_U16)(u32ShiftPWM >> 16);

    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_SHIFT_L, Shift_L);
            HAL_PWM_Write2Byte(REG_PWM0_SHIFT_H, Shift_H);
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_SHIFT_L, Shift_L);
            HAL_PWM_Write2Byte(REG_PWM1_SHIFT_H, Shift_H);
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_SHIFT_L, Shift_L);
            HAL_PWM_Write2Byte(REG_PWM2_SHIFT_H, Shift_H);
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_SHIFT_L, Shift_L);
            HAL_PWM_Write2Byte(REG_PWM3_SHIFT_H, Shift_H);
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_SHIFT_L, Shift_L);
            HAL_PWM_Write2Byte(REG_PWM4_SHIFT_H, Shift_H);
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
			ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            UNUSED(Shift_L);UNUSED(Shift_H);
            break;
		default:
			break;
    }

    HAL_SUBBANK0;

    return TRUE;
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
    MS_U16 Shift_L = 0, Shift_H = 0;

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
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }

    HAL_SUBBANK0;

    return ((Shift_H<<16) | Shift_L);
}


void HAL_PWM_Nvsync(PWM_ChNum index, MS_BOOL bNvsPWM)
    {

        HAL_SUBBANK1;

        switch(index)
        {
            case E_PWM_CH0:
                HAL_PWM_WriteRegBit(REG_PWM0_NVS,BITS(0:0,bNvsPWM),BMASK(0:0));
                break;
            case E_PWM_CH1:
                HAL_PWM_WriteRegBit(REG_PWM1_NVS,BITS(1:1,bNvsPWM),BMASK(1:1));
                break;
            case E_PWM_CH2:
                HAL_PWM_WriteRegBit(REG_PWM2_NVS,BITS(2:2,bNvsPWM),BMASK(2:2));
                break;
            case E_PWM_CH3:
                HAL_PWM_WriteRegBit(REG_PWM3_NVS,BITS(3:3,bNvsPWM),BMASK(3:3));
                break;
            case E_PWM_CH4:
                HAL_PWM_WriteRegBit(REG_PWM4_NVS,BITS(4:4,bNvsPWM),BMASK(4:4));
                break;
            case E_PWM_CH5:
            case E_PWM_CH6:
            case E_PWM_CH7:
            case E_PWM_CH8:
            case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
                UNUSED(bNvsPWM);
                break;
            default:
                break;
        }

        HAL_SUBBANK0;
    }

void HAL_PWM_Align(PWM_ChNum index, MS_BOOL bAliPWM)
    {

        HAL_SUBBANK1;

        switch(index)
        {
            case E_PWM_CH0:
                HAL_PWM_WriteRegBit(REG_PWM0_Align,BITS(0:0,bAliPWM),BMASK(0:0));
                break;
            case E_PWM_CH1:
                HAL_PWM_WriteRegBit(REG_PWM1_Align,BITS(1:1,bAliPWM),BMASK(1:1));
                break;
            case E_PWM_CH2:
                HAL_PWM_WriteRegBit(REG_PWM2_Align,BITS(2:2,bAliPWM),BMASK(2:2));
                break;
            case E_PWM_CH3:
                HAL_PWM_WriteRegBit(REG_PWM3_Align,BITS(3:3,bAliPWM),BMASK(3:3));
                break;
            case E_PWM_CH4:
                HAL_PWM_WriteRegBit(REG_PWM4_Align,BITS(4:4,bAliPWM),BMASK(4:4));
                break;
            case E_PWM_CH5:
            case E_PWM_CH6:
            case E_PWM_CH7:
            case E_PWM_CH8:
            case E_PWM_CH9:
            ULOGE(TAG_PWM, "[Utopia] The PWM%d is not support\n", (int)index);
                UNUSED(bAliPWM);
                break;
            default:
                break;
        }

        HAL_SUBBANK0;
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
    HAL_SUBBANK1;
    HAL_PWM_WriteRegBit(REG_PWM_MULTI_DIFF, PWM_MULTI_DIEF_EN, bEnable);
    HAL_SUBBANK0;
}

MS_BOOL HAL_PWM_Set3D_DiffWaveform(PWM_ChNum index, MS_U8 u8WaveformIndex, MS_U32 u32Shift, MS_U32 u32Duty)
{
    MS_BOOL bReturn = TRUE;
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
	
    return bReturn;
}

//---------------------------PM Base--------------------------------//

void HAL_PM_PWM_Enable(void)
{
    HAL_PM_WriteRegBit(REG_PM_PWM0_IS_GPIO,BITS(5:5,0),BMASK(5:5));
    HAL_PM_WriteRegBit(reg_pwm_as_chip_config,BITS(0:0,0),BMASK(0:0));//reg_pwm_pm_is_PWM
}

void HAL_PM_PWM_Period(MS_U16 u16PeriodPWM)
{
    MS_U16  Period;
    Period = u16PeriodPWM;
    HAL_PM_Write2Byte(REG_PM_PWM0_PERIOD, Period);
}

void HAL_PM_PWM_DutyCycle(MS_U16 u16DutyPWM)
{
    MS_U16  Duty;
    Duty =u16DutyPWM;
    HAL_PM_Write2Byte(REG_PM_PWM0_DUTY, Duty);
}

void HAL_PM_PWM_Div(MS_U8 u8DivPWM)
{
    MS_U8 Div;
    Div = u8DivPWM;
    HAL_PM_Write2Byte(REG_PM_PWM0_DIV, Div);
}

void HAL_PM_PWM_Polarity(MS_BOOL bPolPWM)
{
    HAL_PM_WriteRegBit(REG_PM_PWM0_PORARITY,BITS(0:0,bPolPWM),BMASK(0:0));
}

void HAL_PM_PWM_DBen(MS_BOOL bdbenPWM)
{
    HAL_PM_WriteRegBit(REG_PM_PWM0_DBEN,BITS(1:1,bdbenPWM),BMASK(1:1));
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
    HAL_PM_WriteRegBit(REG_INV_3D_FLAG,BITS(15:15,bInvPWM),BMASK(15:15));
    ret = true;    
    HAL_SUBBANK0;
    return ret; 
}

static void HAL_PWM_LR_RST_RISING(PWM_ChNum index, MS_BOOL bMask)
{
    switch (index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(0:0, bMask), BMASK(0:0));
            break;

        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(1:1, bMask), BMASK(1:1));
            break;

        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(2:2, bMask), BMASK(2:2));
            break;

        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(3:3, bMask), BMASK(3:3));
            break;

        default:
            break;
    }
}

static void HAL_PWM_LR_RST_FALLING(PWM_ChNum index, MS_BOOL bMask)
{
    switch (index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(8:8, bMask), BMASK(8:8));
            break;

        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(9:9, bMask), BMASK(9:9));
            break;

        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(10:10, bMask), BMASK(10:10));
            break;

        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_INV_3D_FLAG, BITS(11:11, bMask), BMASK(11:11));
            break;

        default:
            break;
    }
}

void HAL_PWM_LR_RST_SEL(PWM_ChNum index, N_LR_SYNC_SEL eLR_Sync)
{
    switch (eLR_Sync)
    {
        case E_VSYNC:
            HAL_PWM_LR_RST_RISING(index, FALSE);
            HAL_PWM_LR_RST_FALLING(index, FALSE);
            break;

        case E_LR_RISING:
            HAL_PWM_LR_RST_RISING(index, TRUE);
            HAL_PWM_LR_RST_FALLING(index, FALSE);
            break;

        case E_LR_FALLING:
            HAL_PWM_LR_RST_RISING(index, FALSE);
            HAL_PWM_LR_RST_FALLING(index, TRUE);
            break;

        case E_LR_RISING_FALLING:
            HAL_PWM_LR_RST_RISING(index, TRUE);
            HAL_PWM_LR_RST_FALLING(index, TRUE);
            break;

        default:
            break;
    }
}
