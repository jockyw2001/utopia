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
///
/// file   halDSCMB.c
/// @brief  Descrambler (DSCMB) HAL Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "halDSCMB.h"
#include "regDSCMB.h"

#ifdef  CONFIG_MSTAR_CLKM
#include "drvCLKM.h"
#endif //CONFIG_MSTAR_CLKM


#define REG_BASE_BANK           _u32RegBase
// #define REG_BASE_BANK           0xbf200000

#define REG16_R(reg)            *((volatile MS_U32*)(REG_BASE_BANK+ ((reg)<< 2)))
#define DSCMB_ASSERT(x)             if (!(x)){printf ("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);}

typedef struct
{
    HAL_DSCMB_Type              DscmbType;
    MS_U32                      u32SlotIdEven;
    MS_U32                      u32SlotIdOdd;
} halVirtualSlotMap;

static halVirtualSlotMap        _Map[ENG_NUM][REG_DSCMB_MAX_PIDFLT];

static MS_VIRT _u32RegBase = 0x0;
// static MS_U32 _u32KLDst = 0x0;

// static MS_BOOL _HAL_DSCMB_PidSlotReadAll(MS_U32 u32SlotPidFltId, MS_U32* pu32PidSlot);
//static MS_BOOL _HAL_DSCMB_PidSlotRead(MS_U32 u32EngId, MS_U32 u32SlotPidFltId, DSCMB_Key_Type KeyType, MS_U32* pu32Slot);
static MS_BOOL _HAL_DSCMB_PidSlotWrite(MS_U32 u32EngId, MS_U32 u32SlotPidFltId, MS_U32 u32SlotId, DSCMB_Key_Type KeyType);
static MS_BOOL _HAL_DSCMB_SlotKeyWrite(MS_U32 u32EngId,
                                       MS_U32 u32SlotPidFltId,
                                       DSCMB_Key_Type eKeyType,
                                       MS_U32 u32Pos,
                                       MS_U32 u32EngFlag,
                                       MS_BOOL bValid,
                                       MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK, 3--> key reset
                                       MS_U32 u32Key);
static MS_BOOL _HAL_DSCMB_KTE_ParaTableSet(MS_U32 u32EngId, MS_U32 u32Slot, DSCMB_Key_Type eKeyType, HAL_DSCMB_Type DscmbType);

/*
static MS_BOOL _HAL_DSCMB_SlotKeyRead(MS_U32 u32SlotPidFltId,
                                      HAL_DscmbKey_Type eKeyType,
                                      MS_U32 u32Pos,
                                      MS_U32 u32EngFlag,
                                      MS_BOOL bValid,
                                      MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK
                                      MS_U32* pu32Key);
*/
// static MS_BOOL _HAL_DSCMB_CIP_Enable(MS_BOOL bEnable);
// static MS_BOOL _HAL_DSCMB_SlotSwitchWrite(MS_U32 u32SlotPidFltId, HAL_DscmbKey_Type KeyType, MS_U32 u32Switch);
// static MS_BOOL _HAL_DSCMB_SlotSwitchRead(MS_U32 u32SlotPidFltId, HAL_DscmbKey_Type KeyType, MS_U32* pu32Switch);

static MS_U32 _HAL_DmxFlt2Dscmb(MS_U32 u32EngId, MS_U32 u32Idx)
{
    return u32Idx;
}

static void REG16_W(MS_VIRT u32Reg, MS_U32 u32Value)
{
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg)<< 2))) = u32Value;
}

static void REG32_W(MS_VIRT u32Reg, MS_U32 u32Value)
{
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg)<< 2))) = u32Value & 0x0000FFFF;
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg+1)<< 2))) = u32Value >> 16;
}

void inline _pid_key_table_w(MS_U32 eng, MS_U32 addr, MS_U32 key_idx)
{
    MS_U32 engbit = (eng == 0) ? REG_DSCMB2_ARRAY_ENG0_VLD :
                    (eng == 1) ? REG_DSCMB2_ARRAY_ENG1_VLD :
                    (eng == 2) ? REG_DSCMB2_ARRAY_ENG2_VLD :
                    (eng == 3) ? REG_DSCMB2_ARRAY_ENG3_VLD : 0;

    REG16_W(REG_DSCMB2_ARRAY_WDATA, engbit | (key_idx & REG_DSCMB2_KEY_IDX_MASK));
    REG16_W(REG_DSCMB2_ARRAY_ADR, addr & REG_DSCMB2_ARRAY_ADR_MASK);
    REG16_W(REG_DSCMB2_ARRAY_RW_TRIG, REG_DSCMB2_TRIG_W) ;
}

MS_U32 inline _pid_key_table_r(MS_U32 eng, MS_U32 addr, MS_U32 key_idx)
{
    REG16_W(REG_DSCMB2_ARRAY_ADR, addr & REG_DSCMB2_ARRAY_ADR_MASK);
    REG16_W(REG_DSCMB2_ARRAY_RW_TRIG, REG_DSCMB2_TRIG_R);
    return REG16_R(REG_DSCMB2_ARRAY_RDATA);
}

static void _HAL_PidHitTableInit(void)
{
    MS_U32 eng = 0;
    MS_U32 addr = 0;
    for(;eng < ENG_NUM; eng++)
    {
        for(;addr < MAX_KEY; addr++)
        {
            _pid_key_table_w(0xFFFFFFFFUL, addr, 0);
        }
    }
}

MS_BOOL _HAL_DSCMB_Mode(MS_U32 u32EngId, HAL_DSCMB_Type type)
{

    if (E_HAL_DSCMB_TYPE_AES == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_CBC_CLR_MODE |REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_AES_MODE |REG_DSCMB1_CIP_ESA_CBC_CLR_MODE |REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_AES_MODE |REG_DSCMB2_CIP_ESA_CBC_CLR_MODE |REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;
    }
    else if (E_HAL_DSCMB_TYPE_AES_ECB == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_ECB_MODE |REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_ECB_MODE |REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_ECB_MODE |REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;
    }    
    else if (E_HAL_DSCMB_TYPE_DES == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_DES_MODE |REG_DSCMB_CIP_ESA_ECB_MODE | REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_DES_MODE |REG_DSCMB1_CIP_ESA_ECB_MODE | REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_DES_MODE |REG_DSCMB2_CIP_ESA_ECB_MODE | REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;

    }
    else if (E_HAL_DSCMB_TYPE_TDES_ECB == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_TDES_MODE | REG_DSCMB_CIP_ESA_ECB_MODE | REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_TDES_MODE | REG_DSCMB1_CIP_ESA_ECB_MODE | REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_TDES_MODE | REG_DSCMB2_CIP_ESA_ECB_MODE | REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;
    }    
    else if (E_HAL_DSCMB_TYPE_TDES_SCTE52 == type) // TDS-CBC
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_TDES_MODE | REG_DSCMB_CIP_ESA_CBC_MODE | REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_TDES_MODE | REG_DSCMB1_CIP_ESA_CBC_MODE | REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_TDES_MODE | REG_DSCMB2_CIP_ESA_CBC_MODE | REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;
    }    
    else if (E_HAL_DSCMB_TYPE_AES_SCTE52 == type) //DBook
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_CBC_MODE |REG_DSCMB_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_AES_MODE |REG_DSCMB1_CIP_ESA_CBC_MODE |REG_DSCMB1_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_AES_MODE |REG_DSCMB2_CIP_ESA_CBC_MODE |REG_DSCMB2_CIP_ESA_DECRYPT);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
            REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;
    }
    else if (E_HAL_DSCMB_TYPE_CSA == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST);
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_DSCMB1_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST); //80C1
            REG16_W(REG_ESA1_CTRL,  REG_ESA1_CTRL_CORRECT_SCRMBFLAG | REG_ESA1_CTRL_OLD_TSC | REG_ESA1_CTRL_USE_CSA_PARSER | REG_ESA1_CTRL_SWITCH_KEY_VALID); // 4006
            REG16_W(REG_CSA1_CTRL, REG_DSCMB1_CTRL_CSA_MODE); //0001
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_DSCMB2_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST); //80C1
            REG16_W(REG_ESA2_CTRL,  REG_ESA2_CTRL_CORRECT_SCRMBFLAG | REG_ESA2_CTRL_OLD_TSC | REG_ESA2_CTRL_USE_CSA_PARSER | REG_ESA2_CTRL_SWITCH_KEY_VALID); // 4006
            REG16_W(REG_CSA2_CTRL, REG_DSCMB2_CTRL_CSA_MODE); //0001
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;

    }
    else if (E_HAL_DSCMB_TYPE_CSA_CONF == type)
    {
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST | REG_DSCMB_CTRL_CONFORMANCE_MECH);
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_DSCMB1_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST); //80C1
            REG16_W(REG_ESA1_CTRL,  REG_ESA1_CTRL_CORRECT_SCRMBFLAG | REG_ESA1_CTRL_OLD_TSC | REG_ESA1_CTRL_USE_CSA_PARSER | REG_ESA1_CTRL_SWITCH_KEY_VALID); // 4006
            REG16_W(REG_CSA1_CTRL, REG_DSCMB1_CTRL_CSA_MODE | REG_DSCMB1_CTRL_CONFORMANCE_MECH); //0001
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_DSCMB2_KT_CTRL, 0x0);
            REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST); //80C1
            REG16_W(REG_ESA2_CTRL,  REG_ESA2_CTRL_CORRECT_SCRMBFLAG | REG_ESA2_CTRL_OLD_TSC | REG_ESA2_CTRL_SWITCH_KEY_VALID); // 4006
            REG16_W(REG_CSA2_CTRL, REG_DSCMB2_CTRL_CSA_MODE | REG_DSCMB2_CTRL_CONFORMANCE_MECH); //0001
            REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
        }
        else
            return FALSE;

    }
    else if (E_HAL_DSCMB_TYPE_MULTI2 == type)
    {
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_MULTI2_MODE | REG_DSCMB_CIP_ESA_CBC_MODE | REG_DSCMB_CIP_ESA_DECRYPT);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_MULTI2_MODE | REG_DSCMB1_CIP_ESA_CBC_MODE | REG_DSCMB1_CIP_ESA_DECRYPT);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_MULTI2_MODE | REG_DSCMB2_CIP_ESA_CBC_MODE | REG_DSCMB2_CIP_ESA_DECRYPT);
        }
        else if(u32EngId == 3)
        {
            REG16_W(REG_CSA3_CTRL, 0x0);
            REG16_W(REG_DSCMB3_KT_CTRL, REG_DSCMB3_CIP_ESA_MULTI2_MODE | REG_DSCMB3_CIP_ESA_CBC_MODE | REG_DSCMB3_CIP_ESA_DECRYPT);
        }
        else
            return FALSE;
    }
    else if (E_HAL_DSCMB_TYPE_AES_CTR == type) // HDCP2
    {
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP_ENCRYPT);
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_HDCP2_MODE |
                                       REG_DSCMB_CIP_ESA_CTR_MODE |
                                       REG_DSCMB_CIP_ESA_PES_MODE |
                                       REG_DSCMB_CIP_ESA_AES_MODE );
            // Since no scrmb info in HDCP2 stream, HDCP2 mode use esa mode tsc
            //if(eKeyType == E_DSCMB_KEY_EVEN)
            //    REG16_W(REG_DSCMB_CTRL1, (REG16_R(REG_DSCMB_CTRL1) & ~REG_DSCMB_CTRL1_NEW_TSC_MASK) | REG_DSCMB_CTRL1_NEW_TSC_EVEN);
            //else
                REG16_W(REG_DSCMB_CTRL1, (REG16_R(REG_DSCMB_CTRL1) & ~REG_DSCMB_CTRL1_NEW_TSC_MASK) | REG_DSCMB_CTRL1_NEW_TSC_ODD);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);

            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_HDCP2_MODE |
                                       REG_DSCMB1_CIP_ESA_CTR_MODE |
                                       REG_DSCMB1_CIP_ESA_PES_MODE |
                                       REG_DSCMB1_CIP_ESA_AES_MODE );

            // HDCP2 mode use esa mode tsc
            //if(eKeyType == E_DSCMB_KEY_EVEN)
            //    REG16_W(REG_ESA1_CTRL, (REG16_R(REG_ESA1_CTRL) & ~REG_ESA1_CTRL_NEW_TSC_MASK) | REG_ESA1_CTRL_NEW_TSC_EVEN);
            //else
                REG16_W(REG_ESA1_CTRL, (REG16_R(REG_ESA1_CTRL) & ~(REG_ESA1_CTRL_NEW_TSC_MASK|REG_ESA1_CTRL_USE_CSA_PARSER)) | REG_ESA1_CTRL_NEW_TSC_ODD);
        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);

            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_HDCP2_MODE |
                                       REG_DSCMB2_CIP_ESA_CTR_MODE |
                                       REG_DSCMB2_CIP_ESA_PES_MODE |
                                       REG_DSCMB2_CIP_ESA_AES_MODE );

            // HDCP2 mode use esa mode tsc
            //if(eKeyType == E_DSCMB_KEY_EVEN)
            //    REG16_W(REG_ESA2_CTRL, (REG16_R(REG_ESA2_CTRL) & ~REG_ESA2_CTRL_NEW_TSC_MASK) | REG_ESA2_CTRL_NEW_TSC_EVEN);
            //else
                REG16_W(REG_ESA2_CTRL, (REG16_R(REG_ESA2_CTRL) & ~(REG_ESA2_CTRL_NEW_TSC_MASK|REG_ESA2_CTRL_USE_CSA_PARSER)) | REG_ESA2_CTRL_NEW_TSC_ODD);
        }
        else
            return FALSE;
    }
    else if(E_HAL_DSCMB_TYPE_OC == type) // open cable
    {
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L) = 0x0;
        if(u32EngId == 0)
        {
            REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_DECRYPT |
                                       REG_DSCMB_CIP_ESA_ECB_MODE |
                                       REG_DSCMB_CIP_ESA_TDES_MODE);

            REG16_W(REG_DSCMB_KT_CTRL_EXT, REG_DSCMB_ESA_OC_MODE);
        }
        else if(u32EngId == 1)
        {
            REG16_W(REG_CSA1_CTRL, 0x0);
            REG16_W(REG_DSCMB1_KT_CTRL, REG_DSCMB1_CIP_ESA_DECRYPT |
                                       REG_DSCMB1_CIP_ESA_ECB_MODE |
                                       REG_DSCMB1_CIP_ESA_TDES_MODE);

            REG16_W(REG_DSCMB1_KT_CTRL_EXT, REG_DSCMB1_ESA_OC_MODE);

        }
        else if(u32EngId == 2)
        {
            REG16_W(REG_CSA2_CTRL, 0x0);
            REG16_W(REG_DSCMB2_KT_CTRL, REG_DSCMB2_CIP_ESA_DECRYPT |
                                       REG_DSCMB2_CIP_ESA_ECB_MODE |
                                       REG_DSCMB2_CIP_ESA_TDES_MODE);

            REG16_W(REG_DSCMB2_KT_CTRL_EXT, REG_DSCMB2_ESA_OC_MODE);

        }
        else
            return FALSE;
    }
    else
    {
        return FALSE;
    }


    return TRUE;
}

MS_BOOL HAL_DSCMB_SetBank(MS_VIRT u32Bank)
{
    REG_BASE_BANK = u32Bank;
    printf("[%s][%d] Set bank 0x%08x\n", __FUNCTION__, __LINE__, (int)REG_BASE_BANK);
    return TRUE;
}

#define CKG_TSP_STC0       0x5AAUL
    #define CLK_TSP_DISABLE    0x0001UL
    #define CLK_TSP_INVERT     0x0002UL
    #define CLK_TSP_CLK_MASK   0x000CUL

void _HAL_DSCMB_ClkCtrl(MS_BOOL bEnable)
{
    if(bEnable)
    {
        #ifdef  CONFIG_MSTAR_CLKM
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tsp");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSP_FAST");
        #else
        REG16_W(CKG_TSP_STC0, REG16_R(CKG_TSP_STC0) & ~(CLK_TSP_CLK_MASK |
                                                            CLK_TSP_INVERT   |
                                                            CLK_TSP_DISABLE));
        #endif
    }
    else
    {
        // leave to TSP
    }
}

#undef CKG_TS2_TSGP
    #undef CKG_TSP_GPONLY_MASK
#undef CKG_TSP_STC0
    #undef CLK_TSP_DISABLE
    #undef CLK_TSP_INVERT
    #undef CLK_TSP_CLK_MASK


MS_BOOL HAL_DSCMB_Init(void)
{
    MS_U32 i, j;

    _HAL_DSCMB_ClkCtrl(TRUE);
    // Invalidate first tier index
    for (j = 0; j < ENG_NUM; j++)
    {
        for (i = 0; i< REG_DSCMB_MAX_PIDFLT; i++)
        {
            _Map[j][i].DscmbType = E_HAL_DSCMB_TYPE_INVALID;
            // _HAL_DSCMB_PidSlotWrite(i, 0xFFFFFFFFUL, E_DSCMB_KEY_CLEAR);
            _HAL_DSCMB_PidSlotWrite(j, i, 0xFFFFFFFFUL, E_DSCMB_KEY_EVEN);
            _HAL_DSCMB_PidSlotWrite(j, i, 0xFFFFFFFFUL, E_DSCMB_KEY_ODD);
        }

        // key reset
        for (i = 0; i < MAX_KEY; i++)
        {
            HAL_DSCMB_FltKeyReset(j, 0, i << 1, 0, E_DSCMB_KEY_ODD);
            HAL_DSCMB_FltKeyReset(j, 0, i << 1, 0, E_DSCMB_KEY_EVEN);
        }
    }

    // T2 has CIP only
    REG16_R(REG_DSCMB_CTRL)&= ~(REG_DSCMB_CTRL_CSA_ENABLE);
    REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);

    // dscmb bank 2 KTE valid, visible from register
    REG16_W(REG_ESA1_CTRL, REG16_R(REG_ESA1_CTRL) | REG_ESA1_CTRL_SWITCH_KEY_VALID);

    // enable HDCP2 mode ECO bit for Agate
    REG16_W(REG_DSCMB_CTRL1, REG16_R(REG_DSCMB_CTRL1) | REG_DSCMB_CTRL1_HDCP2_ECO);

    // scmb status reg don't care pure-AF packet
    REG16_W(REG_DSCMB2_CIPHERENG_CTRL, REG16_R(REG_DSCMB2_CIPHERENG_CTRL) | ( REG_BLK_AF_ENG0 | REG_BLK_AF_ENG1 | REG_BLK_AF_ENG2 | REG_BLK_AF_ENG3 ));

#if (DSCMB_LAST_ENG_MULTI2_ONLY == 1)
    //for Engine #3, only supports multi2, need to set ESA mode for scramble status, not CSA mode
    REG16_W(REG_DSCMB3_KT_CTRL_EXT, REG16_R(REG_DSCMB3_KT_CTRL_EXT)|REG_DSCMB3_ESA_UPDATE_TSC_ESA);
#endif

    // pid - key mapping table initialize
    _HAL_PidHitTableInit();
    return TRUE;
}

MS_BOOL HAL_DSCMB_CheckShareConnect(MS_U32 u32EngId, MS_U32 u32FltId, DSCMB_SLOT_Type SlotType)
{
    return TRUE;
}

// --------------------------------------------------------
//  Filter allocate staatus.
// --------------------------------------------------------
//  #========#           #=========#          #=========#
//  #   Engine 0   #           #    Engine 1    #          #    Engine 2    #
//  #========#           #=========#          #=========#
//  ------------  0          ------------  0         ------------  16
//  |   Share0     |             |   Share1     |            |   Share1     |
//  ------------ 15          ------------ 15        ------------ 31
//  ------------ 16          ------------ 32        ------------ 48
//  |   DSCM0     |             |   DSCM0     |            |   DSCM0     |
//  ------------ 31          ------------ 47        ------------ 63
//  ------------ 64          ------------ 64        ------------ 80
//  |    Share1    |             |    Share0    |            |    Share0    |
//  ------------ 79          ------------ 79        ------------ 95
//  ------------ 80          ------------ 96        ------------ 112
//  |    Share2    |             |    Share2    |            |    Share2    |
//  ------------ 95          ------------ 111      ------------ 127

MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData)
{
    MS_BOOL bRet = TRUE;
    *((MS_U32*)pOutData) = 0;

    switch(eQueryType)
    {
    case E_HAL_DSCMB_CAP_ENGINE_NUM:
        *((MS_U32*)pOutData) = ENG_NUM;
        break;
    case E_HAL_DSCMB_CAP_FLT_NUM:
        *((MS_U32*)pOutData) = REG_DSCMB_MAX_SLOT/2;
        break;
    //------------------------------------------------------------
    case E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM:
        *((MS_U32*)pOutData) = 1;
        break;
   case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM:
        *((MS_U32*)pOutData) = 1;  // not use 3 share region now.
        break;
    case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START: // this region is fix on TSP.
        if ((u32EngId) == 0)
        {
            if(*((MS_U32*)pInData) == 0)
                *((MS_U32*)pOutData) = DSCMB0_SHARE2_FLT_START_ID;
            else
                *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        }
        else if ((u32EngId) == 1)
        {
            if(*((MS_U32*)pInData) == 0)
                *((MS_U32*)pOutData) = DSCMB1_SHARE2_FLT_START_ID;
            else
                *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        }
        else
            *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        break;
    case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END:
        if ((u32EngId) == 0)
        {
            if(*((MS_U32*)pInData) == 0)
                *((MS_U32*)pOutData) = DSCMB0_SHARE2_FLT_END_ID + 1;
            else
                *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        }
        else if ((u32EngId) == 1)
        {
            if(*((MS_U32*)pInData) == 0)
                *((MS_U32*)pOutData) = DSCMB1_SHARE2_FLT_END_ID + 1;
            else
                *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        }
        else
            *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        break;
    //------------------------------------------------------------
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_START:
        if(u32EngId == 0)
            *((MS_U32*)pOutData) = GROUP0_MAP_PID_START;
        else if(u32EngId == 1)
            *((MS_U32*)pOutData) = GROUP1_MAP_PID_START;
            break;
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_END:
        if(u32EngId == 0)
            *((MS_U32*)pOutData) = (GROUP0_MAP_PID_START + GROUP0_MAP_PID_LENGTH - 1);
        else if(u32EngId == 1)
            *((MS_U32*)pOutData) = (GROUP1_MAP_PID_START + GROUP1_MAP_PID_LENGTH - 1);
            break;
    case E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM:
        switch (*((HAL_DSCMB_Type*)pInData)) // engine 3 support MULTI2 only
        {
            case E_HAL_DSCMB_TYPE_MULTI2:
                *((MS_U32*)pOutData) = (MS_U32)TRUE;
                break;
            case E_HAL_DSCMB_TYPE_CSA:
            case E_HAL_DSCMB_TYPE_CSA_CONF:
            case E_HAL_DSCMB_TYPE_DES:
            case E_HAL_DSCMB_TYPE_TDES_ECB:
            case E_HAL_DSCMB_TYPE_TDES_SCTE52:
            case E_HAL_DSCMB_TYPE_AES:
            case E_HAL_DSCMB_TYPE_AES_ECB:
            case E_HAL_DSCMB_TYPE_AES_SCTE52:
            case E_HAL_DSCMB_TYPE_AES_CTR:
            case E_HAL_DSCMB_TYPE_OC:
                if((u32EngId == 0) || (u32EngId == 1) || (u32EngId == 2))
                    *((MS_U32*)pOutData) = (MS_U32)TRUE;
                else // engine 3
                    *((MS_U32*)pOutData) = (MS_U32)FALSE;
                break;
            default:
                *((MS_U32*)pOutData) = (MS_U32)FALSE;
                break;
        }
        break;
    default:
        *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        bRet = FALSE;
        break;
    }
    return bRet;
}

MS_BOOL HAL_DSCMB_OptConfig(MS_U32 u32EngId, HAL_DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet)
{
    MS_U16 u16reg = 0;
    MS_U16 u16data = 0;
    MS_U16 u16bit = 0;
    MS_BOOL bRet = FALSE;
    
    if(pInput == NULL) 
        return FALSE;
        
    if(bSet)
    {
        switch(eOpt)
        {
            case E_DSCMB_CFG_HDCP2_TX_CONTENT_CATEGORY_SUPPORT:
            {
                MS_BOOL bEnable = *(MS_BOOL*)pInput;
                
                u16reg = (u32EngId == 0) ? REG_DSCMB_KT_CTRL_EXT  :
                         (u32EngId == 1) ? REG_DSCMB1_KT_CTRL_EXT :
                         (u32EngId == 2) ? REG_DSCMB2_KT_CTRL_EXT : 
                         (u32EngId == 3) ? REG_DSCMB3_KT_CTRL_EXT : 0 ;
                         
                u16bit = (u32EngId == 0) ? REG_DSCMB_ESA_HDCP_ERRATA_EN  :
                         (u32EngId == 1) ? REG_DSCMB1_ESA_HDCP_ERRATA_EN :
                         (u32EngId == 2) ? REG_DSCMB2_ESA_HDCP_ERRATA_EN : 
                         (u32EngId == 3) ? REG_DSCMB3_ESA_HDCP_ERRATA_EN : 0;
                         
                u16data = bEnable ? (REG16_R(u16reg) | u16bit) :
                                    (REG16_R(u16reg) & ~u16bit);
                                   
                REG16_W(u16reg, u16data);
                bRet = TRUE;
                break;                
            }
            
            case E_HAL_DSCMB_CFG_SCMB_STATUS_PUSI_ONLY:
            {
                MS_BOOL bEnable = *(MS_BOOL*)pInput;
                u16reg = (u32EngId == 0) ? REG_DSCMB_KT_CTRL_EXT  :
                         (u32EngId == 1) ? REG_DSCMB1_KT_CTRL_EXT :
                         (u32EngId == 2) ? REG_DSCMB2_KT_CTRL_EXT : 
                         (u32EngId == 3) ? REG_DSCMB3_KT_CTRL_EXT : 0 ;
                            
                u16bit = (u32EngId == 0) ? REG_DSCMB_ESA_TSC_PUSI_MODE  :
                         (u32EngId == 1) ? REG_DSCMB1_ESA_TSC_PUSI_MODE : 
                         (u32EngId == 2) ? REG_DSCMB2_ESA_TSC_PUSI_MODE :
                         (u32EngId == 3) ? REG_DSCMB3_ESA_TSC_PUSI_MODE : 0;

                u16data = bEnable ? (REG16_R(u16reg) | u16bit) :
                                    (REG16_R(u16reg) & ~u16bit);
                                                        
                REG16_W(u16reg, u16data);
                bRet = TRUE;
                break;
            }
                        
            default:
                break;
        }
    }
    else
    {
    }
    
    return bRet;
}

/*
MS_BOOL HAL_DSCMB_SlotAlloc(MS_U32 u32DscmbId, HAL_DscmbKey_Type KeyType, MS_U32* pu32SlotId)
{
    MS_U32 u32Slot;

    // Oberon/Uranus should judge if u32DscmbId has been visited
    u32Slot = MAsm_CPU_GetTrailOne(_u32FreeSlot);
    if (16 <= u32Slot)
    {
        DSCMB_ASSERT(0);
        return FALSE;
    }
    _u32FreeSlot &= ~(1<< u32Slot);
    *pu32SlotId = u32Slot;
    return TRUE;
}

MS_BOOL HAL_DSCMB_SlotFree(MS_U32 u32DscmbId, HAL_DscmbKey_Type KeyType, MS_U32 u32SlotId)
{
    DSCMB_ASSERT(u32SlotId< REG_DSCMB_MAX_SLOT);
    DSCMB_ASSERT(0 == (_u32FreeSlot & (1<< u32SlotId)));
    _u32FreeSlot |= (1<< u32SlotId);
    return TRUE;
}
*/

MS_BOOL HAL_DSCMB_SlotConnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId)
{
    u32PidFltId = _HAL_DmxFlt2Dscmb(u32EngId, u32PidFltId);
    if (u32PidFltId == 0xFFFFFFFFUL)
    {
        return FALSE;
    }

    _HAL_DSCMB_PidSlotWrite(u32EngId, u32PidFltId, u32SlotId, KeyType);

    // there is an assumption that even/odd slot id allocated continuously and alignment to 2
    u32SlotId = u32SlotId >> 1;
    _pid_key_table_w(u32EngId, u32PidFltId, u32SlotId);

    return TRUE;
}

MS_BOOL HAL_DSCMB_SlotDisconnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId)
{
    u32PidFltId = _HAL_DmxFlt2Dscmb(u32EngId, u32PidFltId);
    if (u32PidFltId == 0xFFFFFFFFUL)
    {
        return FALSE;
    }

    _HAL_DSCMB_PidSlotWrite(u32EngId, u32PidFltId, 0xFFFFFFFFUL, KeyType);

    u32SlotId = u32SlotId >> 1;
    _pid_key_table_w(0xFFFFFFFFUL, u32PidFltId, 0);
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltTypeSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, MS_BOOL bDscmb)
{
    u32Slot = u32Slot >> 1;

    switch (DscmbType)
    {
        default:
            DSCMB_ASSERT(0);
            return FALSE;
        case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA
            return FALSE;
        case E_HAL_DSCMB_TYPE_CSA: // CSA
        case E_HAL_DSCMB_TYPE_CSA_CONF: // CSA
        case E_HAL_DSCMB_TYPE_DES: // CI+ DES
        case E_HAL_DSCMB_TYPE_TDES_ECB:
        case E_HAL_DSCMB_TYPE_TDES_SCTE52: // TDES_CBC            
        case E_HAL_DSCMB_TYPE_AES:
        case E_HAL_DSCMB_TYPE_AES_ECB:
        case E_HAL_DSCMB_TYPE_AES_CTR:
        case E_HAL_DSCMB_TYPE_AES_SCTE52:
        case E_HAL_DSCMB_TYPE_MULTI2:
        case E_HAL_DSCMB_TYPE_OC:
            _Map[u32EngId][u32PidFltId].DscmbType = DscmbType;
            if(_HAL_DSCMB_KTE_ParaTableSet(u32EngId, u32Slot, E_DSCMB_KEY_EVEN, DscmbType) == FALSE ||
               _HAL_DSCMB_KTE_ParaTableSet(u32EngId, u32Slot, E_DSCMB_KEY_ODD,  DscmbType) == FALSE)
            {
                printf("[%s]ERROR, set parameter fail\n", __FUNCTION__);
                return FALSE;
            }
            else
            {
                return _HAL_DSCMB_Mode(u32EngId, DscmbType);
            }
            break;
    }
}

MS_BOOL HAL_DSCMB_FltKeySet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    u32Slot = u32Slot >> 1;

    switch (DscmbType)
    {
        case E_HAL_DSCMB_TYPE_MULTI2: // CI+ MULTI2 --> 4/5 0 0
        case E_HAL_DSCMB_TYPE_DES: // CI+ DES  --> 4/5 0 0
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 4, 0, FALSE, 0,
                               ((pu8Key[0]) | (pu8Key[1] << 8) | (pu8Key[2] << 16) | (pu8Key[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 5, 0, FALSE, 0,
                               ((pu8Key[4]) | (pu8Key[5] << 8) | (pu8Key[6] << 16) | (pu8Key[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 6, 0, FALSE, 0, 0);
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 7, 0, TRUE, 0, 0);
        break;

    case E_HAL_DSCMB_TYPE_TDES_ECB:
    case E_HAL_DSCMB_TYPE_TDES_SCTE52:
    case E_HAL_DSCMB_TYPE_OC: // open cable
    case E_HAL_DSCMB_TYPE_CSA: // CSA   -> 4/5/6/7
    case E_HAL_DSCMB_TYPE_CSA_CONF:
    case E_HAL_DSCMB_TYPE_AES: // CI+ AES --> 4/5/6/7 IV 0/1/2/3
    case E_HAL_DSCMB_TYPE_AES_ECB:
    case E_HAL_DSCMB_TYPE_AES_SCTE52: //CI+CBC , DBook
    case E_HAL_DSCMB_TYPE_AES_CTR: // AES CTR, for HDCP2
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 4, 0, FALSE, 0,
                               ((pu8Key[0]) | (pu8Key[1] << 8) | (pu8Key[2] << 16) | (pu8Key[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 5, 0, FALSE, 0,
                               ((pu8Key[4]) | (pu8Key[5] << 8) | (pu8Key[6] << 16) | (pu8Key[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 6, 0, FALSE, 0,
                               ((pu8Key[8]) | (pu8Key[9] << 8) | (pu8Key[10] << 16) | (pu8Key[11] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 7, 0, TRUE, 0,
                               ((pu8Key[12]) | (pu8Key[13] << 8) | (pu8Key[14] << 16) | (pu8Key[15] << 24)));
        break;
    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA -> 4/5/6/7
    default:
        DSCMB_ASSERT(0);
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltKeyReset(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType)
{
    u32Slot = u32Slot >> 1;

    _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 0x0, 0, FALSE, 3, 0);
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltIVSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    u32Slot = u32Slot >> 1;

    DSCMB_ASSERT(pu8IV);

    if(pu8IV == NULL)
        return FALSE;

    switch (DscmbType)
    {
    case E_HAL_DSCMB_TYPE_CSA: // CSA   -> 4/5 0 0
    case E_HAL_DSCMB_TYPE_CSA_CONF:
    case E_HAL_DSCMB_TYPE_DES: // CI+ DES  --> 4/5 0 0
    case E_HAL_DSCMB_TYPE_TDES_ECB:
    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA -> 4/5/6/7
    case E_HAL_DSCMB_TYPE_AES_ECB:
    default:
        DSCMB_ASSERT(0);
        return FALSE;
    case E_HAL_DSCMB_TYPE_OC: // open cable
    case E_HAL_DSCMB_TYPE_AES: // CI+ AES --> 4/5/6/7 IV 0/1/2/3
    case E_HAL_DSCMB_TYPE_AES_SCTE52: //CI+ DBook, T8 new
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 0, 0, FALSE, 0,
                                ((pu8IV[0]) | (pu8IV[1] << 8) | (pu8IV[2] << 16) | (pu8IV[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 1, 0, FALSE, 0,
                                ((pu8IV[4]) | (pu8IV[5] << 8) | (pu8IV[6] << 16) | (pu8IV[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 2, 0, FALSE, 0,
                                ((pu8IV[8]) | (pu8IV[9] << 8) | (pu8IV[10] << 16) | (pu8IV[11] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 3, 0, TRUE, 0,
                                ((pu8IV[12]) | (pu8IV[13] << 8) | (pu8IV[14] << 16) | (pu8IV[15] << 24)));
        break;
    case E_HAL_DSCMB_TYPE_TDES_SCTE52:
    case E_HAL_DSCMB_TYPE_MULTI2: // CI+ MULTI2 64bit IV
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 0, 0, FALSE, 0,
                                ((pu8IV[0]) | (pu8IV[1] << 8) | (pu8IV[2] << 16) | (pu8IV[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 1, 0, FALSE, 0,
                                ((pu8IV[4]) | (pu8IV[5] << 8) | (pu8IV[6] << 16) | (pu8IV[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 2, 0, FALSE, 0, 0);
        _HAL_DSCMB_SlotKeyWrite(u32EngId, u32Slot, eKeyType, 3, 0, TRUE, 0, 0);

        break;
    }
    return TRUE;
}

MS_BOOL HAL_DSCMB_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
    MS_U32 u32Reg;
    if(0 != (u32Round % 4) || (0 != (u32Round & ~(REG_DSCMB_MULTI2_ROUNDS_MASK))))
        return FALSE;

    if(u32EngId == 0)
        u32Reg = REG_DSCMB_MULTI2_ROUND;
    else if(u32EngId == 1)
        u32Reg = REG_DSCMB1_MULTI2_ROUND;
    else if(u32EngId == 2)
        u32Reg = REG_DSCMB2_MULTI2_ROUND;
    else if(u32EngId == 3)
        u32Reg = REG_DSCMB3_MULTI2_ROUND;
    else
        return FALSE;

    REG16_W(REG_DSCMB_ENG2_CTRL, 0x0); // old for A5, must clear here
    REG16_W(u32Reg, u32Round);
    return TRUE;
}

MS_BOOL HAL_DSCMB_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey)
{
    if(u32EngId == 0)
    {
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_L0, ((u8SysKey[3])  | (u8SysKey[2]<<8)  | (u8SysKey[1]<<16)  | (u8SysKey[0]<<24))); // [0-63]
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_H0, ((u8SysKey[7])  | (u8SysKey[6]<<8)  | (u8SysKey[5]<<16)  | (u8SysKey[4]<<24)));
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_L1, ((u8SysKey[11]) | (u8SysKey[10]<<8) | (u8SysKey[9]<<16)  | (u8SysKey[8]<<24))); // [64-127]
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_H1, ((u8SysKey[15]) | (u8SysKey[14]<<8) | (u8SysKey[13]<<16) | (u8SysKey[12]<<24)));
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_L2, ((u8SysKey[19]) | (u8SysKey[18]<<8) | (u8SysKey[17]<<16) | (u8SysKey[16]<<24))); // [128-191]
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_H2, ((u8SysKey[23]) | (u8SysKey[22]<<8) | (u8SysKey[21]<<16) | (u8SysKey[20]<<24)));
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_L3, ((u8SysKey[27]) | (u8SysKey[26]<<8) | (u8SysKey[25]<<16) | (u8SysKey[24]<<24))); // [192-255]
        REG32_W(REG_DSCMB0_MULTI2_SYSKEY_H3, ((u8SysKey[31]) | (u8SysKey[30]<<8) | (u8SysKey[29]<<16) | (u8SysKey[28]<<24)));
    }
    else if(u32EngId == 1)
    {
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_L0, ((u8SysKey[3])  | (u8SysKey[2]<<8)  | (u8SysKey[1]<<16)  | (u8SysKey[0]<<24))); // [0-63]
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_H0, ((u8SysKey[7])  | (u8SysKey[6]<<8)  | (u8SysKey[5]<<16)  | (u8SysKey[4]<<24)));
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_L1, ((u8SysKey[11]) | (u8SysKey[10]<<8) | (u8SysKey[9]<<16)  | (u8SysKey[8]<<24))); // [64-127]
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_H1, ((u8SysKey[15]) | (u8SysKey[14]<<8) | (u8SysKey[13]<<16) | (u8SysKey[12]<<24)));
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_L2, ((u8SysKey[19]) | (u8SysKey[18]<<8) | (u8SysKey[17]<<16) | (u8SysKey[16]<<24))); // [128-191]
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_H2, ((u8SysKey[23]) | (u8SysKey[22]<<8) | (u8SysKey[21]<<16) | (u8SysKey[20]<<24)));
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_L3, ((u8SysKey[27]) | (u8SysKey[26]<<8) | (u8SysKey[25]<<16) | (u8SysKey[24]<<24))); // [192-255]
        REG32_W(REG_DSCMB1_MULTI2_SYSKEY_H3, ((u8SysKey[31]) | (u8SysKey[30]<<8) | (u8SysKey[29]<<16) | (u8SysKey[28]<<24)));
    }
    else if(u32EngId == 2)
    {
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_L0, ((u8SysKey[3])  | (u8SysKey[2]<<8)  | (u8SysKey[1]<<16)  | (u8SysKey[0]<<24))); // [0-63]
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_H0, ((u8SysKey[7])  | (u8SysKey[6]<<8)  | (u8SysKey[5]<<16)  | (u8SysKey[4]<<24)));
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_L1, ((u8SysKey[11]) | (u8SysKey[10]<<8) | (u8SysKey[9]<<16)  | (u8SysKey[8]<<24))); // [64-127]
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_H1, ((u8SysKey[15]) | (u8SysKey[14]<<8) | (u8SysKey[13]<<16) | (u8SysKey[12]<<24)));
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_L2, ((u8SysKey[19]) | (u8SysKey[18]<<8) | (u8SysKey[17]<<16) | (u8SysKey[16]<<24))); // [128-191]
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_H2, ((u8SysKey[23]) | (u8SysKey[22]<<8) | (u8SysKey[21]<<16) | (u8SysKey[20]<<24)));
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_L3, ((u8SysKey[27]) | (u8SysKey[26]<<8) | (u8SysKey[25]<<16) | (u8SysKey[24]<<24))); // [192-255]
        REG32_W(REG_DSCMB2_MULTI2_SYSKEY_H3, ((u8SysKey[31]) | (u8SysKey[30]<<8) | (u8SysKey[29]<<16) | (u8SysKey[28]<<24)));
    }
    else if(u32EngId == 3)
    {
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_L0, ((u8SysKey[3])  | (u8SysKey[2]<<8)  | (u8SysKey[1]<<16)  | (u8SysKey[0]<<24))); // [0-63]
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_H0, ((u8SysKey[7])  | (u8SysKey[6]<<8)  | (u8SysKey[5]<<16)  | (u8SysKey[4]<<24)));
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_L1, ((u8SysKey[11]) | (u8SysKey[10]<<8) | (u8SysKey[9]<<16)  | (u8SysKey[8]<<24))); // [64-127]
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_H1, ((u8SysKey[15]) | (u8SysKey[14]<<8) | (u8SysKey[13]<<16) | (u8SysKey[12]<<24)));
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_L2, ((u8SysKey[19]) | (u8SysKey[18]<<8) | (u8SysKey[17]<<16) | (u8SysKey[16]<<24))); // [128-191]
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_H2, ((u8SysKey[23]) | (u8SysKey[22]<<8) | (u8SysKey[21]<<16) | (u8SysKey[20]<<24)));
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_L3, ((u8SysKey[27]) | (u8SysKey[26]<<8) | (u8SysKey[25]<<16) | (u8SysKey[24]<<24))); // [192-255]
        REG32_W(REG_DSCMB3_MULTI2_SYSKEY_H3, ((u8SysKey[31]) | (u8SysKey[30]<<8) | (u8SysKey[29]<<16) | (u8SysKey[28]<<24)));
    }
    else
        return FALSE;

    return TRUE;
}


MS_BOOL HAL_DSCMB_HDCP2_SetRiv(MS_U32 u32EngId, MS_U8* pu8RIV)
{
    if(u32EngId == 0)
    {
        REG16_W(REG_DSCMB0_HDCP2_RIV0, ((pu8RIV[6]<<8) | pu8RIV[7]));
        REG16_W(REG_DSCMB0_HDCP2_RIV1, ((pu8RIV[4]<<8) | pu8RIV[5]));
        REG16_W(REG_DSCMB0_HDCP2_RIV2, ((pu8RIV[2]<<8) | pu8RIV[3]));
        REG16_W(REG_DSCMB0_HDCP2_RIV3, ((pu8RIV[0]<<8) | pu8RIV[1]));
    }
    else if(u32EngId == 1)
    {
        REG16_W(REG_DSCMB1_HDCP2_RIV0, ((pu8RIV[6]<<8) | pu8RIV[7]));
        REG16_W(REG_DSCMB1_HDCP2_RIV1, ((pu8RIV[4]<<8) | pu8RIV[5]));
        REG16_W(REG_DSCMB1_HDCP2_RIV2, ((pu8RIV[2]<<8) | pu8RIV[3]));
        REG16_W(REG_DSCMB1_HDCP2_RIV3, ((pu8RIV[0]<<8) | pu8RIV[1]));
    }
    else if(u32EngId == 2)
    {
        REG16_W(REG_DSCMB2_HDCP2_RIV0, ((pu8RIV[6]<<8) | pu8RIV[7]));
        REG16_W(REG_DSCMB2_HDCP2_RIV1, ((pu8RIV[4]<<8) | pu8RIV[5]));
        REG16_W(REG_DSCMB2_HDCP2_RIV2, ((pu8RIV[2]<<8) | pu8RIV[3]));
        REG16_W(REG_DSCMB2_HDCP2_RIV3, ((pu8RIV[0]<<8) | pu8RIV[1]));
    }
    else
        return FALSE;

    return TRUE;
}

MS_BOOL HAL_DSCMB_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, HAL_Scmb_Level* pScmbLevel)
{
    MS_U16 u16ScmbStatus = 0;
    u32PidFltId = _HAL_DmxFlt2Dscmb(u32EngId, u32PidFltId);

    if (u32PidFltId == 0xFFFFFFFFUL)
    {
        return FALSE;
    }

    REG16_W(REG_DSCMB2_ARRAY_ADR, (u32PidFltId & 0x000000FFUL)); //pid idx
    REG16_W(REG_DSCMB2_ARRAY_RW_TRIG, REG_DSCMB2_TRIG_R);      //fire read

    if(u32EngId == 0)
    {
        u16ScmbStatus = (REG16_R(REG_DSCMB2_ARRAY_RDATA1) >> REG_DSCMB2_ENG0_SHFT);
    }
    else if (u32EngId == 1)
    {
        u16ScmbStatus = (REG16_R(REG_DSCMB2_ARRAY_RDATA1) >> REG_DSCMB2_ENG1_SHFT);
    }
    else if (u32EngId == 2)
    {
        u16ScmbStatus = (REG16_R(REG_DSCMB2_ARRAY_RDATA1) >> REG_DSCMB2_ENG2_SHFT);
    }
    else if (u32EngId == 3)
    {
        u16ScmbStatus = (REG16_R(REG_DSCMB2_ARRAY_RDATA1) >> REG_DSCMB2_ENG3_SHFT);
    }

    if(u16ScmbStatus & REG_DSCMB2_ENG_TS)
        *pScmbLevel |= E_HAL_DSCMB_TS;

    if(u16ScmbStatus & REG_DSCMB2_ENG_PES)
        *pScmbLevel |= E_HAL_DSCMB_PES;

    return TRUE;
}

MS_BOOL HAL_DSCMB_KLadder_Acpu_Ack(MS_BOOL bEn)
{
    /*if (bEn)
        REG16_W(REG_DSCMB_KL_CTRL3, REG16_R(REG_DSCMB_KL_CTRL3)| REG_KL_ACPU_ACK);
    else
        REG16_W(REG_DSCMB_KL_CTRL3, REG16_R(REG_DSCMB_KL_CTRL3) & ~(REG_KL_ACPU_ACK));*/

    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Acpu_Data(MS_U8* pu8Out)
{
    /*MS_U16  u16RegData=0;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_L);
    pu8Out[15] = u16RegData & 0xFF;
    pu8Out[14] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_L + 1);
    pu8Out[13] = u16RegData & 0xFF;
    pu8Out[12] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_L + 2);
    pu8Out[11] = u16RegData & 0xFF;
    pu8Out[10] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_L + 3);
    pu8Out[9] = u16RegData & 0xFF;
    pu8Out[8] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_H);
    pu8Out[7] = u16RegData & 0xFF;
    pu8Out[6] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_H + 1);
    pu8Out[5] = u16RegData & 0xFF;
    pu8Out[4] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_H + 2);
    pu8Out[3] = u16RegData & 0xFF;
    pu8Out[2] = (u16RegData >> 8) & 0xFF;
    u16RegData = REG16_R(REG_DSCMB_KL_CW_ACPU_H + 3);
    pu8Out[1] = u16RegData & 0xFF;
    pu8Out[0] = (u16RegData >> 8) & 0xFF;*/

    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Reset(void)
{
/*
    REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) | REG_KL_SWRST);
    REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) & ~(REG_KL_SWRST));
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_SetInput(MS_U32 u32Level, MS_U8* pu8In, MS_U32 u32InLen)
{
/*
    MS_U8 u8CW[16];
    MS_U32 u32Reg;

    switch (u32Level)
    {
    case 0:
        u32Reg = REG_DSCMB_CW_LEVEL0;
        break;
    case 1:
        u32Reg = REG_DSCMB_CW_LEVEL1;
        break;
    case 2:
        u32Reg = REG_DSCMB_CW_LEVEL2;
        break;
    default:
        return FALSE;
    }
    memset(u8CW, 0, sizeof(u8CW));
    memcpy(u8CW, pu8In, u32InLen);

    REG16_W(u32Reg + 0, (u8CW[14]<< 8) | u8CW[15]);
    REG16_W(u32Reg + 1, (u8CW[12]<< 8) | u8CW[13]);
    REG16_W(u32Reg + 2, (u8CW[10]<< 8) | u8CW[11]);
    REG16_W(u32Reg + 3, (u8CW[8]<< 8) | u8CW[9]);
    REG16_W(u32Reg + 4, (u8CW[6]<< 8) | u8CW[7]);
    REG16_W(u32Reg + 5, (u8CW[4]<< 8) | u8CW[5]);
    REG16_W(u32Reg + 6, (u8CW[2]<< 8) | u8CW[3]);
    REG16_W(u32Reg + 7, (u8CW[0]<< 8) | u8CW[1]);
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_SetKey(HAL_DscmbKLadderSrc KLSrc, MS_U8* pu8Key, MS_U32 u32KeyLen)
{
/*
    MS_U32 u32Src;

    switch (KLSrc)
    {
    case E_HAL_DSCMB_KLAD_SRC_ACPU:
        u32Src = REG_KL_KEY_SRC_ACPU;
        break;
    case E_HAL_DSCMB_KLAD_SRC_SECRET_KEY1:
        u32Src = REG_KL_KEY_SRC_SECRET1;
        break;
    case E_HAL_DSCMB_KLAD_SRC_SECRET_KEY2:
        u32Src = REG_KL_KEY_SRC_SECRET2;
        break;
    case E_HAL_DSCMB_KLAD_SRC_SECRET_KEY3:
        u32Src = REG_KL_KEY_SRC_SECRET3;
        break;
    case E_HAL_DSCMB_KLAD_SRC_SECRET_KEY4:
        u32Src = REG_KL_KEY_SRC_SECRET4;
        break;
    case E_HAL_DSCMB_KLAD_SRC_VGK:
        u32Src = REG_KL_KEY_SRC_VGK;
        break;
    default:
        return FALSE;
    }
    REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) & ~(REG_KL_KEY_SRC_MASK));
    REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) | (u32Src<< REG_KL_KEY_SRC_SHFT));

    if (KLSrc == E_HAL_DSCMB_KLAD_SRC_ACPU)
    {
        REG16_W(REG_DSCMB_KL_KEY + 0x1, (pu8Key[6] << 8) | pu8Key[7]);
        REG16_W(REG_DSCMB_KL_KEY + 0x2, (pu8Key[4] << 8) | pu8Key[5]);
        REG16_W(REG_DSCMB_KL_KEY + 0x3, (pu8Key[2] << 8) | pu8Key[3]);
        REG16_W(REG_DSCMB_KL_KEY + 0x4, (pu8Key[0] << 8) | pu8Key[1]);
        REG16_W(REG_DSCMB_KL_KEY + 0x5, (pu8Key[14] << 8) | pu8Key[15]);
        REG16_W(REG_DSCMB_KL_KEY + 0x6, (pu8Key[12] << 8) | pu8Key[13]);
        REG16_W(REG_DSCMB_KL_KEY + 0x7, (pu8Key[10] << 8) | pu8Key[11]);
        REG16_W(REG_DSCMB_KL_KEY + 0x8, (pu8Key[8] << 8) | pu8Key[9]);

        REG16_W(REG_DSCMB_KL_CTRL3, REG16_R(REG_DSCMB_KL_CTRL3)| REG_KL_ACPU_ACK);
    }
    else
    {
        REG16_W(REG_DSCMB_KL_CTRL3, REG16_R(REG_DSCMB_KL_CTRL3) & ~(REG_KL_ACPU_ACK));
    }
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_SetDst(HAL_DscmbKLadderDst KLDst)
{
/*
    switch (KLDst)
    {
    case E_HAL_DSCMB_KLAD_DST_KTAB_ESA:
        _u32KLDst = REG_KL_KEY_DST_KTAB_ESA;
        break;
    case E_HAL_DSCMB_KLAD_DST_KTAB_NSA:
        _u32KLDst = REG_KL_KEY_DST_KTAB_NSK;
        break;
    case E_HAL_DSCMB_KLAD_DST_AESDMA_AES:
        _u32KLDst = REG_KL_KEY_DST_AESDMA_AES;
        break;
    case E_HAL_DSCMB_KLAD_DST_AESDMA_TDES:
        _u32KLDst = REG_KL_KEY_DST_AESDMA_TDES;
        break;
    case E_HAL_DSCMB_KLAD_DST_ACPU:
        _u32KLDst = REG_KL_KEY_DST_ACPU;
        break;
    default:
        _u32KLDst = 0;
        return FALSE;
    }
    REG16_W(REG_DSCMB_KL_CTRL2, REG16_R(REG_DSCMB_KL_CTRL2) & ~(REG_KL_KEY_DST_MASK));
    REG16_W(REG_DSCMB_KL_CTRL2, REG16_R(REG_DSCMB_KL_CTRL2) | (_u32KLDst<< REG_KL_KEY_DST_SHFT));
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Start(void)
{
/*
    REG16_W(REG_DSCMB_KL_CTRL2, REG16_R(REG_DSCMB_KL_CTRL2) & ~(REG_KL_FORCE_ACK));
    REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1)| REG_KL_START);
    return TRUE;
*/
    return FALSE;
}

void HAL_DSCMB_KLadder_Decrypt(MS_BOOL bDecrypt)
{
/*
    if (bDecrypt)
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1)| REG_KL_DECRYPT);
    }
    else
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1)& ~(REG_KL_DECRYPT));
    }
*/
}

void HAL_DSCMB_KLadder_ByteInv(MS_BOOL bInverse)
{
/*
    if (bInverse)
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) | REG_KL_BYTE_INV);
    }
    else
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1) & ~(REG_KL_BYTE_INV));
    }
*/
}

void HAL_DSCMB_KLadder_KeepRound(MS_BOOL bKeepRound)
{
/*
    if (bKeepRound)
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1)| REG_KL_KEEP_ROUNDS);
    }
    else
    {
        REG16_W(REG_DSCMB_KL_CTRL1, REG16_R(REG_DSCMB_KL_CTRL1)& ~(REG_KL_KEEP_ROUNDS));
    }
*/
}

void HAL_DSCMB_KLadder_Round(MS_U32 u32Round)
{
/*
    REG16_W(REG_DSCMB_KL_CTRL1, (REG16_R(REG_DSCMB_KL_CTRL1)& ~(REG_KL_ROUNDS_MASK)) | ((u32Round<< REG_KL_ROUNDS_SHFT) & REG_KL_ROUNDS_MASK));
*/
}

void HAL_DSCMB_KLadder_EngMode(MS_U32 u32Mode)
{
/*
    DSCMB_ASSERT(0 == u32Mode); // MUST be TDES
    REG16_W(REG_DSCMB_KL_CTRL2, (REG16_R(REG_DSCMB_KL_CTRL2)& ~(REG_KL_ENG_MODE_MASK)) | ((0 << REG_KL_ENG_MODE_SHFT) & REG_KL_ENG_MODE_MASK));
*/
}


MS_BOOL HAL_DSCMB_KLadder_IsComplete(void)
{
/*
    return (((REG16_R(REG_DSCMB_KL_STATUS) & REG_KL_STATUS_CW_RDY_MASK)>> REG_KL_STATUS_CW_RDY_SHFT) == _u32KLDst)? TRUE: FALSE;
*/
    return TRUE;
}

MS_BOOL HAL_DSCMB_KLadder_Stop(void)
{
/*
    _u32KLDst = 0;
    REG16_W(REG_DSCMB_KL_CTRL1, 0);
    REG16_W(REG_DSCMB_KL_CTRL2, 0);
    REG16_W(REG_DSCMB_KL_CTRL3, 0);
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Output_Start(MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType)
{
/*
    MS_U32 u32SlotTmp;
    MS_U32 u32EngFlag = 0x0002;
    MS_U32 u32Pos;

    if ((16 > u32PidFltId) || (32<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16;

    if (FALSE == _HAL_DSCMB_PidSlotRead(u32PidFltId, eKeyType, &u32SlotTmp))
    {
        return FALSE;
    }
    DSCMB_ASSERT(u32SlotTmp == u32Slot);

    switch (DscmbType)
    {
    case E_HAL_DSCMB_TYPE_CSA: // CSA
    case E_HAL_DSCMB_TYPE_DES: // CI+ DES
    case E_HAL_DSCMB_TYPE_AES: // CI+ AES
    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA
        u32EngFlag = 0x0002;
        u32Pos = 4;
        break;
    // case E_HAL_DSCMB_TYPE_NSAS: // scramble ??
    //     u32Pos = 8;
    //     break;
    default:
        DSCMB_ASSERT(0);
        return FALSE;
    }
    return _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, u32Pos, u32EngFlag, TRUE, 1, 0);
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Output_IsComplete(void)
{
/*
    return (REG16_R(REG_DSCMB_KL_STATUS) & REG_KL_KTE_STATUS_DONE)? TRUE: FALSE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_KLadder_Output_Stop(void)
{
/*
    return TRUE;
*/
    return FALSE;
}

MS_BOOL HAL_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary)
{
    return FALSE;
}

MS_BOOL HAL_DSCMB_2ndaryReset(MS_U32 u32PidPrim)
{
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// First tier
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
static MS_BOOL _HAL_DSCMB_PidSlotReadAll(MS_U32 u32SlotPidFltId, MS_U32* pu32PidSlot)
{
    MS_U32 u32Reg;
    u32Reg = REG_DSCMB2_PIDSLOT0 + (u32SlotPidFltId<< 1);
    *pu32PidSlot = (REG16_R(u32Reg) | (REG16_R(u32Reg+ 1)<< 16));
    return TRUE;
}
*/

/*
static MS_BOOL _HAL_DSCMB_PidSlotRead(MS_U32 u32EngId, MS_U32 u32SlotPidFltId, DSCMB_Key_Type KeyType, MS_U32* pu32Slot)
{
    switch (KeyType)
    {
    default:
        DSCMB_ASSERT(0);
            return FALSE;
    case E_DSCMB_KEY_CLEAR:
        return FALSE;
    case E_DSCMB_KEY_EVEN:
        *pu32Slot = _Map[u32EngId][u32SlotPidFltId].u32SlotIdEven;
        break;
    case E_DSCMB_KEY_ODD:
        *pu32Slot = _Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd;
        break;
    }
    return (0xFFFFFFFFUL == (*pu32Slot))? FALSE: TRUE;
}
*/

static MS_BOOL _HAL_DSCMB_PidSlotWrite(MS_U32 u32EngId, MS_U32 u32SlotPidFltId, MS_U32 u32SlotId, DSCMB_Key_Type KeyType)
{
    MS_U32 id_min = 0xFFFFFFFFUL;

    switch (KeyType)
    {
    default:
        DSCMB_ASSERT(0);
            return FALSE;
    case E_DSCMB_KEY_CLEAR:
        return FALSE;
    case E_DSCMB_KEY_EVEN:
        _Map[u32EngId][u32SlotPidFltId].u32SlotIdEven = u32SlotId;
        if((_Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd != 0xFFFFFFFFUL) && (u32SlotId != 0xFFFFFFFFUL))
        {
            id_min = (_Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd > u32SlotId) ?
            u32SlotId : _Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd;
        }
        break;
    case E_DSCMB_KEY_ODD:
        _Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd = u32SlotId;
        if((_Map[u32EngId][u32SlotPidFltId].u32SlotIdEven != 0xFFFFFFFFUL) && (u32SlotId != 0xFFFFFFFFUL))
        {
            id_min = (_Map[u32EngId][u32SlotPidFltId].u32SlotIdEven > u32SlotId) ?
                        u32SlotId : _Map[u32EngId][u32SlotPidFltId].u32SlotIdEven;
        }
        break;
    }

    if((id_min != 0xFFFFFFFFUL) && (id_min % 0x2))
    {
        printf("[%s]WARN, dscmb slot not 2-alignment[%d, %d]\n",
            __FUNCTION__,
            (int)_Map[u32EngId][u32SlotPidFltId].u32SlotIdOdd,
            (int)_Map[u32EngId][u32SlotPidFltId].u32SlotIdEven);
    }
    return TRUE;
}

static MS_BOOL _HAL_DSCMB_SlotKeyWrite(MS_U32 u32EngId,
                                       MS_U32 u32SlotPidFltId,
                                       DSCMB_Key_Type eKeyType,
                                       MS_U32 u32Pos,
                                       MS_U32 u32EngFlag, // useless at T2
                                       MS_BOOL bValid,
                                       MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK
                                       MS_U32 u32Key)
{
    MS_U32 u32KeyType;
    MS_U32 u32Valid;
    MS_U32 u32eng;

    if(u32EngId < ENG_NUM)
    {
        u32eng = u32EngId;
    }
    else
    {
        printf("unsupported DSCMB engine %d\n", (int)u32EngId);
        return FALSE;
    }

    switch (eKeyType)
    {
    default:
        DSCMB_ASSERT(0);
            return FALSE;
    case E_DSCMB_KEY_CLEAR:
        return FALSE;
    case E_DSCMB_KEY_ODD:
        u32KeyType = REG_DSCMB_KEY_TYPE_ODD;
        break;
    case E_DSCMB_KEY_EVEN:
        u32KeyType = REG_DSCMB_KEY_TYPE_EVEN;
        break;
    }

    u32Valid = 1;
    switch (u32From)
    {
    case 0: // ACPU
        REG16_W(REG_DSCMB_WRITE, ((u32Key & 0xFF0000UL)>> 8) | ((u32Key & 0xFF000000UL)>> 24));
        REG16_W(REG_DSCMB_WRITE + 1, (((u32Key & 0xFFUL)<< 8) | ((u32Key & 0xFF00UL)>> 8)));
        REG16_W(REG_DSCMB_ACPU_CMD, (u32Valid<< REG_ACPU_DSCMB_VALID) |
                                    (u32KeyType<< REG_ACPU_DSCMB_TYPE_SHFT) |
                                    (u32SlotPidFltId << REG_ACPU_PIDFLTID_SHFT)|
                                    ((u32Pos << REG_ACPU_CMD_POS_SHFT) & REG_ACPU_CMD_POS_MASK) |
                                    REG_ACPU_CMD_WRITE);
        REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);
        // REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_WEN_ACPU | REG_ACPU_CMD_START);
        REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START |
                                      (u32eng << REG_ACPU_CMD_ENG_SHIFT));
        REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
        break;
    case 1: // KLadder
        // not support at this stage
    case 2: // NSK
        break;
    case 3: // key reset
        REG16_W(REG_DSCMB_ACPU_CMD, (u32Valid<< REG_ACPU_DSCMB_VALID) |
                                    (u32KeyType<< REG_ACPU_DSCMB_TYPE_SHFT) |
                                    (u32SlotPidFltId << REG_ACPU_PIDFLTID_SHFT)|
                                    ((u32Pos << REG_ACPU_CMD_POS_SHFT) & REG_ACPU_CMD_POS_MASK) |
                                    REG_ACPU_CMD_WRITE | REG_ACPU_CMD_CLEAR);
        REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);
        // REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_WEN_ACPU | REG_ACPU_CMD_START);
        REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START |
                                      (u32eng << REG_ACPU_CMD_ENG_SHIFT));
        REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
        break;
    default:
       DSCMB_ASSERT(0);
       break;
    }
    REG16_W(REG_DSCMB_ACPU_START, u32eng << REG_ACPU_CMD_ENG_SHIFT);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

/*
    if (FALSE == bValid)
    {
        REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_ACPU_KEY_STRENGTH);
        REG16_W(REG_DSCMB_KT_CTRL + 1, 0);
    }
*/
    return TRUE;
}

static MS_BOOL _HAL_DSCMB_KTE_ParaTableSet(MS_U32 u32EngId, MS_U32 u32Slot, DSCMB_Key_Type eKeyType, HAL_DSCMB_Type DscmbType)
{
    MS_U32 u32eng       = 0;
    MS_U32 u32KeyType   = REG_DSCMB_KEY_TYPE_ODD;

    MS_U16 u16KTEEng    = 0;
    MS_U16 u16DecType   = 0;
    MS_U16 u16DscmbType = 0;
    MS_U16 u16KeyLen    = 0;

    if(u32EngId < ENG_NUM)
    {
        u32eng = u32EngId;
    }
    else
    {
        printf("unsupported DSCMB engine %d\n", (int)u32EngId);
        return FALSE;
    }

    switch (u32EngId)
    {
        case 0:
            u16KTEEng = REG_KTE_PARA_TABLE_KTE0;
            break;
        case 1:
            u16KTEEng = REG_KTE_PARA_TABLE_KTE1;
            break;
        case 2:
            u16KTEEng = REG_KTE_PARA_TABLE_KTE2;
            break;
        case 3:
            u16KTEEng = REG_KTE_PARA_TABLE_KTE3;
            break;
        default:
            printf("Unsupported DSCMB eng %d\n", (int)u32EngId);
            return FALSE;
    }

    switch (eKeyType)
    {
        case E_DSCMB_KEY_CLEAR:
            return FALSE;
        case E_DSCMB_KEY_ODD:
            u32KeyType = REG_DSCMB_KEY_TYPE_ODD;
            break;
        case E_DSCMB_KEY_EVEN:
            u32KeyType = REG_DSCMB_KEY_TYPE_EVEN;
            break;
        default:
            printf("Unsupported DSCMB key type %d\n", (int)eKeyType);
            return FALSE;
    }

    switch (DscmbType)
    {
        case E_HAL_DSCMB_TYPE_CSA:
        case E_HAL_DSCMB_TYPE_CSA_CONF:
            u16DscmbType = REG_KTE_PARA_TABLE_CSAV2;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_64BIT;
            break;
        case E_HAL_DSCMB_TYPE_DES:
            u16DscmbType = REG_KTE_PARA_TABLE_DES;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_64BIT;
            break;
        case E_HAL_DSCMB_TYPE_TDES_ECB:
        case E_HAL_DSCMB_TYPE_TDES_SCTE52:
            u16DscmbType = REG_KTE_PARA_TABLE_TDES;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_128BIT;
            break;
        case E_HAL_DSCMB_TYPE_AES:
        case E_HAL_DSCMB_TYPE_AES_ECB:            
            u16DscmbType = REG_KTE_PARA_TABLE_AES;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_128BIT;
            break;
        case E_HAL_DSCMB_TYPE_AES_CTR: // HDCP 2.0
            u16DscmbType = REG_KTE_PARA_TABLE_AES;
            u16DecType   = REG_KTE_PARA_TABLE_ENC;
            u16KeyLen    = REG_KTE_PARA_TABLE_128BIT;
            break;
        case E_HAL_DSCMB_TYPE_AES_SCTE52: // DBook
            u16DscmbType = REG_KTE_PARA_TABLE_AES;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_128BIT;
            break;
        case E_HAL_DSCMB_TYPE_MULTI2:
            u16DscmbType = REG_KTE_PARA_TABLE_MULTI2;
            u16DecType   = REG_KTE_PARA_TABLE_DEC;
            u16KeyLen    = REG_KTE_PARA_TABLE_128BIT;
            break;
        default:
            printf("Unsupported DSCMB type %d\n", (int)DscmbType);
            return FALSE;
    }

    // ACPU [63:32]
    REG16_W(REG_DSCMB_WRITE, u16KeyLen);
    REG16_W(REG_DSCMB_WRITE + 1, 0);
    REG16_W(REG_DSCMB_ACPU_CMD, (u32KeyType << REG_ACPU_DSCMB_TYPE_SHFT) |
                                (1 << REG_ACPU_CMD_POS_SHFT) |
                                (u32Slot << REG_ACPU_PIDFLTID_SHFT)|
                                (REG_ACPU_CMD_POS2 | REG_ACPU_CMD_WRITE));
    REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START | (u32eng << REG_ACPU_CMD_ENG_SHIFT));
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, u32eng << REG_ACPU_CMD_ENG_SHIFT);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);


    // ACPU [31:0]
    REG16_W(REG_DSCMB_WRITE, (u16DecType | u16KTEEng | REG_KTE_PARA_TABLE_PARA_VLD));
    REG16_W(REG_DSCMB_WRITE + 1, u16DscmbType);
    REG16_W(REG_DSCMB_ACPU_CMD, (1 << REG_ACPU_DSCMB_VALID) |
                                (u32KeyType << REG_ACPU_DSCMB_TYPE_SHFT) |
                                (u32Slot << REG_ACPU_PIDFLTID_SHFT)|
                                (REG_ACPU_CMD_POS2 | REG_ACPU_CMD_WRITE));
    REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START | (u32eng << REG_ACPU_CMD_ENG_SHIFT));
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, u32eng << REG_ACPU_CMD_ENG_SHIFT);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

    return TRUE;
}

#if 0 // only for debug
static MS_BOOL _HAL_DSCMB_SlotKeyRead(MS_U32 u32SlotPidFltId,
                                      DSCMB_Key_Type eKeyType,
                                      MS_U32 u32Pos,
                                      MS_U32 u32EngFlag,
                                      MS_BOOL bValid,
                                      MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK
                                      MS_U32* pu32Key)
{
    MS_U32 u32KeyType;

    switch (eKeyType)
    {
    case E_DSCMB_KEY_EVEN:
        u32KeyType = REG_DSCMB_KEY_TYPE_EVEN;
        break;
    case E_DSCMB_KEY_ODD:
        u32KeyType = REG_DSCMB_KEY_TYPE_ODD;
        break;
    case E_DSCMB_KEY_CLEAR:
        u32KeyType = REG_DSCMB_KEY_TYPE_CLEAR;
        break;
    default:
        DSCMB_ASSERT(0);
        return FALSE;
    }

    REG16_W(REG_DSCMB_ACPU_CMD, (u32KeyType << REG_ACPU_DSCMB_TYPE_SHFT)|
                                (u32SlotPidFltId << REG_ACPU_PIDFLTID_SHFT)|
                                ((u32Pos << REG_ACPU_CMD_POS_SHFT) & REG_ACPU_CMD_POS_MASK) |
                                REG_ACPU_CMD_READ);
    REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
    REG16_W(REG_DSCMB_ACPU_START, 0);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

    *pu32Key = (REG16_R(REG_DSCMB_READ+ 1)<< 16)| REG16_R(REG_DSCMB_READ);
    return TRUE;
}
#endif


/*
static MS_BOOL _HAL_DSCMB_CIP_Enable(MS_BOOL bEnable)
{
    return TRUE;
}
*/

/*
static MS_BOOL _HAL_DSCMB_SlotSwitchWrite(MS_U32 u32SlotPidFltId, HAL_DscmbKey_Type eKeyType, MS_U32 u32Switch)
{
    MS_U32 u32Slot;
    MS_U32 u32KeyType;

    switch (eKeyType)
    {
    case E_DSCMB_KEY_EVEN:
        u32KeyType = REG_DSCMB_KEY_TYPE_EVEN;
        break;
    case E_DSCMB_KEY_ODD:
        u32KeyType = REG_DSCMB_KEY_TYPE_ODD;
        break;
    case E_DSCMB_KEY_CLEAR:
        u32KeyType = REG_DSCMB_KEY_TYPE_CLEAR;
        break;
    default:
        DSCMB_ASSERT(0);
        return FALSE;
    }

    if (FALSE == _HAL_DSCMB_PidSlotRead(u32SlotPidFltId, eKeyType, &u32Slot))
    {
        printf("[%s][%d] Get Slot fail (SlotPidId, eKeyType)= (%d, %d)\n", __FUNCTION__, __LINE__, (int)u32SlotPidFltId, (int)eKeyType);
        return FALSE;
    }
    printf("[%s][%d] Get Slot (SlotPidId, eKeyType)= (%d, %d)\n", __FUNCTION__, __LINE__, (int)u32SlotPidFltId, (int)eKeyType);

    REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_SWITCH2KT_VALID | REG_DSCMB_SWITCH2KT_WEN | REG_DSCMB_SWITCH2WNASK | REG_DSCMB_ACPU_KEY_STRENGTH);
    REG16_W(REG_DSCMB_KT_CTRL + 1, 0);

    REG16_W(REG_DSCMB_ACPU_CMD, (u32KeyType << REG_ACPU_DSCMB_TYPE_SHFT)|
                                (u32SlotPidFltId<< REG_ACPU_PIDFLTID_SHFT)|
                                ((REG_DSCMB_POS_SWITCH2 << REG_ACPU_CMD_POS_SHFT) & REG_ACPU_CMD_POS_MASK) |
                                REG_ACPU_CMD_WRITE);
    REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);

    REG16_W(REG_DSCMB_WRITE, u32Switch & 0xFFFF);
    REG16_W(REG_DSCMB_WRITE+ 1, (u32Switch>> 16) & 0xFFFF);

    REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_WEN_SWITCH2 | REG_ACPU_CMD_START);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
    REG16_W(REG_DSCMB_ACPU_START, 0);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);

    REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_ACPU_KEY_STRENGTH);
    REG16_W(REG_DSCMB_KT_CTRL + 1, 0);
    return TRUE;
}
*/

#if 0 //  only for debug
static MS_BOOL _HAL_DSCMB_SlotSwitchRead(MS_U32 u32SlotPidFltId, DSCMB_Key_Type KeyType, MS_U32* pu32Switch)
{
    REG16_W(REG_DSCMB_ACPU_CMD, (E_DSCMB_KEY_CLEAR<< REG_ACPU_DSCMB_TYPE_SHFT)|
                                (u32SlotPidFltId<< REG_ACPU_PIDFLTID_SHFT)|
                                REG_ACPU_CMD_READ);
    REG16_W(REG_DSCMB_ACPU_CMD+ 1, 0);

    REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
    REG16_W(REG_DSCMB_ACPU_START, 0);
    REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
    *pu32Switch = (REG16_R(REG_DSCMB_READ) | REG16_R(REG_DSCMB_READ+ 1) << 16);
    return TRUE;
}
#endif
