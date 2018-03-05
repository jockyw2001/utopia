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
////////////////////////////////////////////////////////////////////////////////
#define _HAL_PWS_C

////////////////////////////////////////////////////////////////////////////////
/// @file halPWS.c
/// @author MStar Semiconductor Inc.
/// @brief power saving driver
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "MsCommon.h"
#include "MsTypes.h"
#include "halPWS.h"
#include "MsOS.h"

#include "mhal_pws_table.h"
#include "mhal_pws_setting_info_table.h"
//------------------------------------------------------------------------------
// Define & data type
//------------------------------------------------------------------------------
#define DEBUG_INFO(y)    //y

#define RunBefore   0UL
#define RunAfter    1UL
//------------------------------------------------------------------------------
// Local variable
//------------------------------------------------------------------------------
static MS_VIRT _gPWS_MapBase = 0xA0000000UL;
MS_U8 PWS_flag[OutOfRange_PWS_Items];
MS_U16 u16PWS_First_Items=0;
MS_U16 u16OutOfRange_PWS_Items=0;
static E_PWS_SouceInfo SourceIndex = _NO_SOURCE_;
PWS_RegisterCallback PWS_RegCBinfo[PWS_Register_CB_size];
E_PWS_VIF_type SelectVifTable = E_PWS_VIF_DUAL_SAW;

//------------------------------------------------------------------------------
// Local Function
//------------------------------------------------------------------------------
MS_U8 HAL_PWS_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gPWS_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_U16 HAL_PWS_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gPWS_MapBase))[u32RegAddr];
}

MS_U32 HAL_PWS_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_PWS_Read2Byte(u32RegAddr) | HAL_PWS_Read2Byte(u32RegAddr+2) << 16);
}

MS_BOOL HAL_PWS_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gPWS_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL HAL_PWS_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gPWS_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

MS_BOOL HAL_PWS_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    HAL_PWS_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_PWS_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

MS_BOOL HAL_PWS_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8BitPos)
{
    MS_U8 u8Val = HAL_PWS_ReadByte(u32RegAddr);
    if (!u32RegAddr)
    {
        return FALSE;
    }

    u8Val = HAL_PWS_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8BitPos) : (u8Val & ~u8BitPos);
    HAL_PWS_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

E_PWS_Result HAL_PWS_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    return (E_PWS_Result)HAL_PWS_WriteByte(u32RegAddr, (HAL_PWS_ReadByte(u32RegAddr) & (~u8Mask)) | (u8Val & u8Mask));
}

void HAL_PWS_RunCallback(MS_U8 Position,MS_U8 OnOff_flag, MS_U16 index)
{
    MS_U8 i;
    MS_U8 temp = 0;

    for(i=0;i<PWS_Register_CB_size;i++)
    {
        temp = (MS_U8)PWS_RegCBinfo[i].ctrl;
        if(Position == RunAfter)
            temp = temp - 2;
        else
            temp = temp;

        if(temp == OnOff_flag)  // ctrl
        {
            if(strcmp((char *)pws_setting_info_table[index].regName,(char *)PWS_RegCBinfo[i].regName) == 0) // regName
            {
                if(PWS_RegCBinfo[i]._PWSCallback != NULL) // callback
                {
                    DEBUG_INFO(printf("%s, %s \r\n",__FUNCTION__,PWS_RegCBinfo[i].regName));
                    PWS_RegCBinfo[i]._PWSCallback();
                }
            }
        }
    }
}

E_PWS_Result HAL_PWS_SetXXX(MS_U8 OnOff_flag, MS_U16 index)
{
    E_PWS_Result ret = PWS_FAIL;

    DEBUG_INFO(printf("%s, %s, index %d\n",__FUNCTION__,pws_setting_info_table[index].regName, index));

    if(pws_setting_info_table[index].bInvert == TRUE)
    {
        if(OnOff_flag)
        {
            // INVERT = Y , OFF = 0x00
            HAL_PWS_RunCallback(RunBefore,OnOff_flag,index);
            DEBUG_INFO(printf("[PWS OFF]0x00 %s, index %d\n",pws_setting_info_table[index].regName, index));
            ret = HAL_PWS_WriteByteMask(pws_setting_info_table[index].u32RegAddr, 0x00, pws_setting_info_table[index].u8RegMask);
            HAL_PWS_RunCallback(RunAfter,OnOff_flag,index);
        }
        else
        {
            // INVERT = Y , ON = 0x01
            HAL_PWS_RunCallback(RunBefore,OnOff_flag,index);
            DEBUG_INFO(printf("[PWS ON]0xFF %s, index %d\n",pws_setting_info_table[index].regName, index));
            ret = HAL_PWS_WriteByteMask(pws_setting_info_table[index].u32RegAddr, 0xFF, pws_setting_info_table[index].u8RegMask);
            HAL_PWS_RunCallback(RunAfter,OnOff_flag,index);
        }
    }
    else
    {
        if(OnOff_flag)
        {
            // INVERT = N , OFF = 0x01
            HAL_PWS_RunCallback(RunBefore,OnOff_flag,index);
            DEBUG_INFO(printf("[PWS OFF]0xFF %s, index %d\n",pws_setting_info_table[index].regName, index));
            ret = HAL_PWS_WriteByteMask(pws_setting_info_table[index].u32RegAddr, 0xFF, pws_setting_info_table[index].u8RegMask);
            HAL_PWS_RunCallback(RunAfter,OnOff_flag,index);
        }
        else
        {
            // INVERT = N , ON = 0x00
            HAL_PWS_RunCallback(RunBefore,OnOff_flag,index);
            DEBUG_INFO(printf("[PWS ON]0x00 %s, index %d\n",pws_setting_info_table[index].regName, index));
            ret = HAL_PWS_WriteByteMask(pws_setting_info_table[index].u32RegAddr, 0x00, pws_setting_info_table[index].u8RegMask);
            HAL_PWS_RunCallback(RunAfter,OnOff_flag,index);
        }
    }

    return ret;
}

//------------------------------------------------------------------------------
// Global Function
//------------------------------------------------------------------------------
void HAL_PWS_SetIOMapBase(MS_U32 u32Base)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(u32Base == 0)
    {
        printf("Get PWS RIU base address failed\r\n");
    }
    else
    {
        _gPWS_MapBase = u32Base;
        DEBUG_INFO(printf("PWS_MAP_BASE=%lx\n",_gPWS_MapBase));
    }
}

MS_U32 HAL_PWS_GetIOMapBase(void)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));
    return _gPWS_MapBase;
}

E_PWS_SouceInfo  HAL_PWS_GetSourceInfo(void)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));
    return SourceIndex;
}

void HAL_PWS_Init(E_PWS_VIF_type eVifType)
{
    MS_U16 pws_init_items_counter = 0;
    MS_U16 PWS_flag_index=0;
    MS_U8 PWS_CB_items;

    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(eVifType > E_PWS_VIF_UNKNOWN)
    {
        SelectVifTable = E_PWS_VIF_DUAL_SAW;
    }
    else
    {
        SelectVifTable = eVifType;
    }

    for(PWS_CB_items = 0; PWS_CB_items < PWS_Register_CB_size; PWS_CB_items++)
    {
        PWS_RegCBinfo[PWS_CB_items].ctrl = E_PWS_IP_CALLBACK_UNKNOWN;
        PWS_RegCBinfo[PWS_CB_items].regName[0] = '\n';
        PWS_RegCBinfo[PWS_CB_items]._PWSCallback = NULL;
    }

    // default power save register setting (keep ON or OFF)
    if(OutOfRange_PWS_Init_Items > 0)
    {
        u16PWS_First_Items= PWS_First_Items;
        u16OutOfRange_PWS_Items= OutOfRange_PWS_Items;

        // initial PWS_flag table
        for(PWS_flag_index = u16PWS_First_Items ; PWS_flag_index < u16OutOfRange_PWS_Items ; PWS_flag_index++)
        {
            PWS_flag[PWS_flag_index] = 1;
        }

        DEBUG_INFO(printf("PWS init value setting\r\n"));
        while(pws_init_items_counter < OutOfRange_PWS_Init_Items)
        {
            if(pws_init_setting_info_table[pws_init_items_counter].bInvert == TRUE)
            {
                if((pws_init_table[pws_init_items_counter] && 0xFFFF) == TRUE)
                {
                    // INVERT = Y , OFF = 0x00
                    DEBUG_INFO(printf("[PWS OFF]0x00 %s, index %d, mask %x\r\n",pws_init_setting_info_table[pws_init_items_counter].regName, pws_init_items_counter, pws_init_setting_info_table[pws_init_items_counter].u8RegMask));
                    HAL_PWS_WriteByteMask(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr, 0x00, pws_init_setting_info_table[pws_init_items_counter].u8RegMask);
                }
                else
                {
                    // INVERT = Y , ON = 0x01
                    DEBUG_INFO(printf("[PWS ON]0xFF %s, index %d, mask %x\r\n",pws_init_setting_info_table[pws_init_items_counter].regName, pws_init_items_counter, pws_init_setting_info_table[pws_init_items_counter].u8RegMask));
                    HAL_PWS_WriteByteMask(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr, 0xFF, pws_init_setting_info_table[pws_init_items_counter].u8RegMask);
                }
            }
            else
            {
                if((pws_init_table[pws_init_items_counter] && 0xFFFF) == TRUE)
                {
                    // INVERT = N , OFF = 0x01
                    DEBUG_INFO(printf("[PWS OFF]0xFF %s, index %d, mask %x\r\n",pws_init_setting_info_table[pws_init_items_counter].regName, pws_init_items_counter, pws_init_setting_info_table[pws_init_items_counter].u8RegMask));
                    HAL_PWS_WriteByteMask(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr, 0xFF, pws_init_setting_info_table[pws_init_items_counter].u8RegMask);
                }
                else
                {
                    // INVERT = N , ON = 0x00
                    DEBUG_INFO(printf("[PWS ON]0x00 %s, index %d, mask %x\r\n",pws_init_setting_info_table[pws_init_items_counter].regName, pws_init_items_counter, pws_init_setting_info_table[pws_init_items_counter].u8RegMask));
                    //HAL_PWS_WriteByte(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr, (HAL_PWS_ReadByte(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr) & ~pws_init_setting_info_table[pws_init_items_counter].u8RegMask));
                    HAL_PWS_WriteByteMask(pws_init_setting_info_table[pws_init_items_counter].u32RegAddr, 0x00, pws_init_setting_info_table[pws_init_items_counter].u8RegMask);
                }
            }
            pws_init_items_counter++;
        }
    }
    else
    {
        DEBUG_INFO(printf("PWS No init value setting\r\n"));
    }
}

E_PWS_Result HAL_PWS_HandleSource(E_PWS_Operation operation,E_PWS_SouceInfo source)
{
    E_PWS_Result ret = PWS_SUCCESS;
    E_PWS_SouceInfo find_source;
    MS_U16 PWS_flag_index=0;

    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(operation >=  PWS_INVAILD_OP)
    {
        ret = PWS_FAIL;
        return ret;
    }

    if((source <= _NO_SOURCE_) || (source >= _UNKNOWN_))
    {
        ret = PWS_FAIL;
        return ret;
    }

    u16PWS_First_Items= PWS_First_Items;
    u16OutOfRange_PWS_Items= OutOfRange_PWS_Items;

    // initial PWS_flag table
    for(PWS_flag_index = u16PWS_First_Items ; PWS_flag_index < u16OutOfRange_PWS_Items ; PWS_flag_index++)
    {
        PWS_flag[PWS_flag_index] = 1;
    }

    // Get ON/OFF flag
    // add or del PWS input/output data path
    if(operation == PWS_ADD_SOURCE)
    {
        SourceIndex |= source;
    }
    else
    {
        SourceIndex &= (E_PWS_SouceInfo)(~source);
    }
    DEBUG_INFO(printf("SourceIndex = %d\n",SourceIndex));

    // keep last source PWS register setting - avoid source change issue without PIP case
    if(SourceIndex == _NO_SOURCE_)
    {
        return ret;
    }

    // first input/output data path
    find_source = _USB_;
    do
    {
        if((SourceIndex & find_source) > 0)  // find out source
        {
            for(PWS_flag_index = u16PWS_First_Items ; PWS_flag_index < u16OutOfRange_PWS_Items ; PWS_flag_index++)
            {
                //  ? 1 : 0; --> 1: OFF , 0:ON
                //  PWS_flag[PWS_flag_index] &= --> Src_A(OFF) PIP Src_B(ON) --> PWS = ON
                PWS_flag[PWS_flag_index] &= ((pws_table[PWS_flag_index] & find_source) > 0) ? 1 : 0;
            }
        }
        find_source = (E_PWS_SouceInfo)((MS_U16)(find_source) << 1); // USB ->SV->...
    }while(find_source < _UNKNOWN_);  // last input/output source

    DEBUG_INFO(printf("PWS_FULL\n"));
    for(PWS_flag_index = u16PWS_First_Items; PWS_flag_index < u16OutOfRange_PWS_Items ; PWS_flag_index++)
    {
        ret &= HAL_PWS_SetXXX(PWS_flag[PWS_flag_index], PWS_flag_index);
        DEBUG_INFO(printf("func=%d,arg=%d, ret=%d\n",PWS_flag_index,PWS_flag[PWS_flag_index],ret));
    }

    return ret;
}

E_PWS_Result HAL_PWS_HandleIP(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName)
{
    MS_U16 PWS_flag_index = 0;
    MS_U8  PWS_findString = 0;
    E_PWS_Result ret = PWS_SUCCESS;

    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(IpPowerCtrl > E_PWS_IP_OFF)
    {
        ret = PWS_FAIL;
        return ret;
    }

    for(PWS_flag_index = u16PWS_First_Items ; PWS_flag_index < u16OutOfRange_PWS_Items ; PWS_flag_index++)
    {
       if(strcmp((char *)pws_setting_info_table[PWS_flag_index].regName,(char *)RegName) == 0)
       {
           DEBUG_INFO(printf("Regname is the same\r\n"));
           PWS_findString = 1;  // find out string
           ret &= HAL_PWS_SetXXX((MS_U8)IpPowerCtrl, PWS_flag_index);
       }
    }

    if(PWS_findString == 0)
    {
        ret = PWS_FAIL;
    }

    return ret;
}

E_PWS_Result HAL_PWS_RegisterCallback(MS_U8 index, E_PWS_CallbackCtrl cb_ctrl, unsigned char *RegName, P_PWS_Callback pfCallback)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(index >= PWS_Register_CB_size)
    {
        return PWS_FAIL;
    }

    if(cb_ctrl >= E_PWS_IP_CALLBACK_UNKNOWN)
    {
        return PWS_FAIL;
    }

    if(strlen((char *)RegName) > STR_LEN)
    {
        return PWS_FAIL;
    }

    if(pfCallback == NULL)
    {
        return PWS_FAIL;
    }

    PWS_RegCBinfo[index].ctrl = cb_ctrl;
    strncpy((char *)PWS_RegCBinfo[index].regName,(char *)RegName,strlen((char *)RegName));
    DEBUG_INFO(printf("Register IP name = %s\r\n",PWS_RegCBinfo[index].regName));
    PWS_RegCBinfo[index]._PWSCallback = pfCallback;
    return PWS_SUCCESS;
}

E_PWS_Result HAL_PWS_CancelCallback(MS_U8 index)
{
    DEBUG_INFO(printf("%s\n",__FUNCTION__));

    if(index >= PWS_Register_CB_size)
    {
        return PWS_FAIL;
    }

    PWS_RegCBinfo[index].ctrl = E_PWS_IP_CALLBACK_UNKNOWN;
    PWS_RegCBinfo[index]._PWSCallback = NULL;
    return PWS_SUCCESS;
}

void HAL_PWS_Stop_VDMCU(void)
{

}

E_PWS_Result HAL_PWS_IpPowerControl(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type)
{
    MS_U32 i;
    MS_BOOL bWriteRet;

    bWriteRet = TRUE;

    for (i = 0; pws_ip_info_table[i].eIpName != E_PWS_IP_MAX; ++i)
    {
        if ( pws_ip_info_table[i].eIpName == eIpName)
        {
            bWriteRet = HAL_PWS_Write2Byte (pws_ip_info_table[i].u32RegAddr, pws_ip_info_table[i].u16RegVal);

            if (bWriteRet != TRUE)
            {
                return PWS_FAIL;
            }
        }
    }

    for (i = 0; pws_ip_mask_info_table[i].eIpName != E_PWS_IP_MAX; ++i)
    {
        if ( pws_ip_mask_info_table[i].eIpName == eIpName)
        {
            bWriteRet = HAL_PWS_WriteRegBit (pws_ip_mask_info_table[i].u32RegAddr, pws_ip_mask_info_table[i].bEnable, pws_ip_mask_info_table[i].u8BitPos);

            if (bWriteRet != TRUE)
            {
                return PWS_FAIL;
            }
        }
    }



    return PWS_SUCCESS;
}
