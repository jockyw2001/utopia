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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halAESDMA.c
// @brief  AESDMA HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "regRSA.h"
#include "halRSA.h"


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

#define RSA_FLOW_DBG(fmt, args...)  //{printf("\033[33m [%s]",__func__); printf(fmt, ## args); printf("\033[m");}


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_U32                _u32keylen = 0;
static MS_VIRT                _u32RegBase = 0;
static REG_RSACtrl          *_RSACtrl = 0;

static MS_U32                _u32CurrentSecureRangeSet = 0;


//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

void _RSA_REG32_W(REG32 *reg, MS_U32 value)
{
     (*((volatile MS_U32*)(reg))) = value;
}



MS_U32 _RSA_REG32_R(REG32 *reg)
{
    MS_U32     value;
    value = (*(volatile MS_U32*)(reg));
    return value;
}

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void HAL_RSA_SetBank(MS_VIRT u32NonPmBankAddr)  // OK
{
    _u32RegBase = u32NonPmBankAddr;
    _RSACtrl =(REG_RSACtrl        *) (_u32RegBase +REG_RSACTRL_BASE);
}


void HAL_RSA_ClearInt(void)  /// OK
{
    //RSA interrupt clear
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , _RSA_REG32_R(&_RSACtrl[0].Rsa_Ind32_Start)|(RSA_INT_CLR));
}

void HAL_RSA_Reset(void)  //// OK
{
    //RSA Rst
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) , RSA_CTRL_RSA_RST);
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) , 0);


    //add polling RSA status before load data to SRAM
    while ((HAL_RSA_GetStatus()&RSA_STATUS_RSA_BUSY) == RSA_STATUS_RSA_BUSY)
    {
        ;
    }
}

void HAL_RSA_Ind32Ctrl(MS_U8 u8dirction)  ////OK
{
    //[1] reg_ind32_direction 0: Read. 1: Write


    if(u8dirction==1)
    {
        /// Move it in Loadram
        ////_RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) ,(RSA_RAM_DIR)|(RSA_IND32_CTRL_ADDR_AUTO_INC)|(RSA_IND32_CTRL_ACCESS_AUTO_START)|(RSA_RAM_MSB_FIRST));

    }
    else
    {
        _RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) ,(RSA_IND32_CTRL_ADDR_AUTO_INC)|(RSA_IND32_CTRL_ACCESS_AUTO_START)|(RSA_RAM_MSB_FIRST));
    }
}

void HAL_RSA_LoadSram(MS_U32 *u32Buf, RSA_IND32Address eMode) /// Question in K3 demo code
{
    MS_U32 u32Cmd = 0;
    MS_U32 i = 0;
    MS_U8  *pu8Data;
    MS_U32 u32Data = 0;

    switch (eMode)
    {
    case E_RSA_ADDRESS_E:
        u32Cmd |= (RSA_E_BASE_ADDR);
        break;

    case E_RSA_ADDRESS_N:
        u32Cmd |= (RSA_N_BASE_ADDR);
        break;

    case E_RSA_ADDRESS_A:
        u32Cmd |= (RSA_A_BASE_ADDR);
        break;

    default:
        return;
    }

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Addr) , u32Cmd);
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) ,(RSA_RAM_DIR)|(RSA_IND32_CTRL_ADDR_AUTO_INC)|(RSA_IND32_CTRL_ACCESS_AUTO_START)|(RSA_RAM_MSB_FIRST));
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , RSA_INDIRECT_START);
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , 0);

    for( i = 0; i < 64; i++ )
    {
        if(eMode==E_RSA_ADDRESS_E)
        {
            pu8Data = (MS_U8 *)(&u32Buf[(64-1)-i]);
        }
        else{
            pu8Data = (MS_U8 *)(&u32Buf[i]);
        }
        u32Data = ((MS_U32) pu8Data[0])<<24 |((MS_U32) pu8Data[1])<<16 | ((MS_U32) pu8Data[2])<<8 |(MS_U32) pu8Data[3];
        _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Data) , u32Data);
    }
}

void HAL_RSA_SetKeyLength(MS_U32 u32keylen) /// Ken Len  unit:Word
{
    //[13:8] n_len_e: key length
    _u32keylen = u32keylen;
}

void HAL_RSA_SetKeyType(MS_U8 u8hwkey, MS_U8 u8pubkey)
{
    //[1] hw_key_e : 0 : software key, 1: hardware key
    //[2] e_pub_e : 0: pvivate key, 1: public key
    MS_U32 value = (_u32keylen<<8);

    if(u8hwkey==1)
    {
        value |= RSA_CTRL_SEL_HW_KEY;

    }
    else
    {
        value &= ~RSA_CTRL_SEL_HW_KEY;

    }

    if(u8pubkey==1)
    {
        value |= (RSA_CTRL_SEL_PUBLIC_KEY);

    }
    else
    {
        value &= ~RSA_CTRL_SEL_PUBLIC_KEY;

    }

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ctrl) , value);
}

void HAL_RSA_Hwkey_Switch(void)
{
    MS_U32 value = 0;

    value |= RSA_HWKEY_SWITCH;

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Hwkey_Switch) , value);
}

MS_U32 HAL_RSA_Read_Hwkey_Switch(void)
{
    return _RSA_REG32_R((&_RSACtrl[0].Rsa_Hwkey_Switch)) & RSA_READ_HWKEY_SWITCH;
}

void HAL_RSA_Rootkey_Sel(MS_U8 u8KeySrc)
{
    // RSA_ROOTKEY_SEL_PUB_NO              0x00000020    /// if public key, 0: select 1st key; 1 select 2nd key
    // RSA_ROOTKEY_SEL_TYPE                0x00000040    /// 1: OTP RSA key is public; 0: OTP RSA is private
    MS_U32 value = 0;

    switch (u8KeySrc)
    {
    case E_RSA_HW_PRI_KEY:   // 0*
        {
            value &= ~RSA_ROOTKEY_SEL_TYPE;
            break;
        }
    case E_RSA_HW_PUB_KEY1:  // 10
        {
            value |= RSA_ROOTKEY_SEL_TYPE;
            value &= ~RSA_ROOTKEY_SEL_PUB_NO;
            break;
        }
    case E_RSA_HW_PUB_KEY2:  // 11
        {
            value |= RSA_ROOTKEY_SEL_TYPE;
            value |= RSA_ROOTKEY_SEL_PUB_NO;
            break;
        }
    default:
        {
            break;
        }
    }

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , value);
}

void HAL_RSA_ExponetialStart(void)
{
    //RSA exp start

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , _RSA_REG32_R(&_RSACtrl[0].Rsa_Ind32_Start)|(RSA_EXP_START));
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , _RSA_REG32_R(&_RSACtrl[0].Rsa_Ind32_Start)&(~RSA_EXP_START) );
}

MS_U32 HAL_RSA_GetStatus(void)
{
    return _RSA_REG32_R(&_RSACtrl[0].Rsa_Ind32_Addr) & RSA_STATUS_MASK;
}

void HAL_RSA_FileOutStart(void)
{
    //RSA ind32_start

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start) , RSA_INDIRECT_START);
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Start), 0);
}

void HAL_RSA_SetFileOutAddr(MS_U32 u32offset)
{
    MS_U32 u32Addr = 0;
    u32Addr = RSA_Z_BASE_ADDR + u32offset;

    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Addr) ,u32Addr);
}

MS_U32 HAL_RSA_FileOut(void)
{
    MS_U32 u32out = 0;

    u32out = _RSA_REG32_R(&_RSACtrl[0].Rsa_Ind32_Data);
    _RSA_REG32_W((&_RSACtrl[0].Rsa_Ind32_Data) , 0);

    return u32out;
}

MS_U32 HAL_RSA_Get_RSA_IsFinished(void)  ///  done
{
    //// in fact it is used to detect if rsa is busy .... but the function name seems not correct.
    ////  return value :TRUE  means busy....
    ////                FALSE means finished
    ////  But the API is used in utopia, so I did not refine it now.....
    if((HAL_RSA_GetStatus()&RSA_STATUS_RSA_BUSY) == RSA_STATUS_RSA_BUSY)
    {
        return -1;
    }
    else{
        return 0;
    }
}


MS_BOOL HAL_RSA_SetSecureRange(MS_U32 u32addr, MS_U32 u32size)
{
	RSA_FLOW_DBG("u32addr=%x, u32size=%x\n", u32addr, u32size);
	RSA_FLOW_DBG("_RSACtrl = %x\n", _RSACtrl);

	MS_U32 start_addr    = u32addr;


	MS_U32 start_unit = 0;
	MS_U32 end_unit   = 0;


	start_unit =  u32addr >> REG_RSA_SEC_RANGE_SHIFT;
	end_unit   = (u32addr + u32size) >> REG_RSA_SEC_RANGE_SHIFT;


	//alignment to secure range start
	start_addr = u32addr & REG_RSA_SEC_RANGE_MASK;

	if(start_addr < u32addr)
	{
		RSA_FLOW_DBG("Invalid Input Address. Need to alignment...\n");
		return FALSE;
	}


	if((end_unit == start_unit)  || (end_unit < start_unit))
	{
		RSA_FLOW_DBG("Invalid Input Size\n");
		return FALSE;
	}
	else
	{
		if(_u32CurrentSecureRangeSet < RSA_MAX_SECURE_RANGE_SIZE)
		{

		#if 1
			_RSA_REG32_W(&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_Start, start_unit);
			_RSA_REG32_W(&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_End, end_unit);

			RSA_FLOW_DBG("Rsa_Sec_Range_Start=%x, Rsa_Sec_Range_End=%x\n",
				&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_Start,
				&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_End);

			RSA_FLOW_DBG("start_unit = %x, end_unit = %x, _u32CurrentSecureRangeSet  = %x",
				         start_unit, end_unit, _u32CurrentSecureRangeSet);

			_u32CurrentSecureRangeSet ++;

		#endif
			return TRUE;
		}
		else
		{
			RSA_FLOW_DBG("Secure Range Set is not enough\n");
			return FALSE;
		}
	}
}

#if 0
MS_BOOL HAL_RSA_GetSecureRange(MS_U32 u32addr, MS_U32 u32size, MS_U32* u32getaddr, MS_U32* u32getsize)
{
	MS_U32 start_addr    = u32addr;
	MS_U32 avalible_size = u32size;

	MS_U32 start_unit = 0;
	MS_U32 end_unit   = 0;

	*u32getaddr = NULL;
	*u32getsize = 0;

	start_unit =  u32addr >> 16;
	end_unit   = (u32addr + u32size) >> 16;


	//alignment to secure range start
	start_addr = start_addr >> 16;
	start_addr = start_addr << 16;

	if(start_addr < u32addr)
	{
		start_unit += 1;

		avalible_size = u32size - ((start_unit << 16) - u32addr);
		end_unit = start_unit +  (avalible_size >> 16);
	}


	if((end_unit == start_unit)  || (end_unit < start_unit))
	{
		RSA_FLOW_DBG("Invalid Input Size\n");
		return FALSE;
	}
	else
	{
		if(_u32CurrentSecureRangeSet < RSA_MAX_SECURE_RANGE_SIZE)
		{
			_RSA_REG32_W(&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_Start, start_unit);
			_RSA_REG32_W(&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_End, end_unit);

			_u32CurrentSecureRangeSet ++;

			*u32getaddr = start_unit << 16;
			*u32getsize = (end_unit - start_unit) << 16;
			RSA_FLOW_DBG("Rsa_Sec_Range_Start=%x, Rsa_Sec_Range_End=%x\n",
				&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_Start,
				&_RSACtrl[0].Rsa_Sec_Range[_u32CurrentSecureRangeSet].Rsa_Sec_Range_End);

			RSA_FLOW_DBG("start_unit = %x, end_unit = %x, _u32CurrentSecureRangeSet  = %x",
				         start_unit, end_unit, _u32CurrentSecureRangeSet);
			return TRUE;
		}
		else
		{
			RSA_FLOW_DBG("Secure Range Set is not enough\n");
			return FALSE;
		}
	}
}
#endif

