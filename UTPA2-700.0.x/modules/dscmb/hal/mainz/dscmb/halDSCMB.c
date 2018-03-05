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
#include "drvDSCMB.h"
#include "halDSCMB.h"
#include "regDSCMB.h"

#define REG_BASE_BANK           _virtRegBase
// #define REG_BASE_BANK           0xbf200000

#define REG16_R(reg)            *((volatile MS_U32*)(REG_BASE_BANK+ ((reg)<< 2)))

typedef struct
{
    HAL_DSCMB_Type              DscmbType;
    MS_U32                      u32SlotIdEven;
    MS_U32                      u32SlotIdOdd;
} halVirtualSlotMap;

static halVirtualSlotMap        _Map[REG_DSCMB_MAX_PIDFLT];

static MS_VIRT _virtRegBase = 0x0UL;
// static MS_U32 _u32KLDst = 0x0;

// static MS_BOOL _HAL_DSCMB_PidSlotReadAll(MS_U32 u32SlotPidFltId, MS_U32* pu32PidSlot);
static MS_BOOL _HAL_DSCMB_PidSlotRead(MS_U32 u32SlotPidFltId, DSCMB_Key_Type KeyType, MS_U32* pu32Slot);
static MS_BOOL _HAL_DSCMB_PidSlotWrite(MS_U32 u32SlotPidFltId, MS_U32 u32SlotId, DSCMB_Key_Type KeyType);
static MS_BOOL _HAL_DSCMB_SlotKeyWrite(MS_U32 u32SlotPidFltId,
                                       DSCMB_Key_Type eKeyType,
                                       MS_U32 u32Pos,
                                       MS_U32 u32EngFlag,
                                       MS_BOOL bValid,
                                       MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK
                                       MS_U32 u32Key);
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


static void REG16_W(MS_U32 u32Reg, MS_U32 u32Value)
{
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg)<< 2UL))) = u32Value;
}

MS_BOOL HAL_DSCMB_SetBank(MS_VIRT virtBank)
{
    REG_BASE_BANK = virtBank;
    printf("[%s][%d] Set bank 0x%08x\n", __FUNCTION__, __LINE__, (unsigned int)REG_BASE_BANK);
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
            REG16_W(CKG_TSP_STC0, REG16_R(CKG_TSP_STC0) & ~(CLK_TSP_CLK_MASK |
                                                            CLK_TSP_INVERT   |
                                                            CLK_TSP_DISABLE));
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
    MS_U32 i;

    _HAL_DSCMB_ClkCtrl(TRUE);
    // Invalidate first tier index
    for (i = 0; i< REG_DSCMB_MAX_PIDFLT; i++)
    {
        _Map[i].DscmbType = E_HAL_DSCMB_TYPE_INVALID;
        // _HAL_DSCMB_PidSlotWrite(i, 0xFFFFFFFF, E_DSCMB_KEY_CLEAR);
        _HAL_DSCMB_PidSlotWrite(i, 0xFFFFFFFF, E_DSCMB_KEY_EVEN);
        _HAL_DSCMB_PidSlotWrite(i, 0xFFFFFFFF, E_DSCMB_KEY_ODD);
    }
    // T2 has CIP only
    REG16_R(REG_DSCMB_CTRL)&= ~(REG_DSCMB_CTRL_CSA_ENABLE);
    REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);

    REG16_W(REG_DSCMB2_CIPHERENG_CTRL, REG16_R(REG_DSCMB2_CIPHERENG_CTRL) | REG_BLK_AF);
    return TRUE;
}

MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData)
{
    MS_BOOL bRet = TRUE;
    MS_U16  u16data;

    *((MS_U32*)pOutData) = 0;

    switch(eQueryType)
    {
    case E_HAL_DSCMB_CAP_ENGINE_NUM:
        *((MS_U32*)pOutData) = ENG_NUM;
        break;
    case E_HAL_DSCMB_CAP_FLT_NUM:
        *((MS_U32*)pOutData) = REG_DSCMB_MAX_SLOT/2;
        break;
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_START:
        u16data = REG16_R(REG_TSP1_CACTRL);
        if(u16data & REG_TSP1_CACTRL_INPUT_TS1)
        {
            *((MS_U32*)pOutData) = TSP_PID_FLT_NUM;
        }
        else
        {
            *((MS_U32*)pOutData) = ENG0_MAP_PID_START;
        }
        break;
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_END:
        u16data = REG16_R(REG_TSP1_CACTRL);
        if(u16data & REG_TSP1_CACTRL_INPUT_TS1)
        {
            *((MS_U32*)pOutData) = (ENG0_MAP_PID_LENGTH + TSP_PID_FLT_NUM - 1);
        }
        else
        {
            *((MS_U32*)pOutData) = (ENG0_MAP_PID_START + ENG0_MAP_PID_LENGTH - 1);
        }
        break;
    case E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM:
    case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM:
        *((MS_U32*)pOutData) = 0;
        break;
    case E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM:
        switch (*((HAL_DSCMB_Type*)pInData))
        {
            case E_HAL_DSCMB_TYPE_CSA:
            case E_HAL_DSCMB_TYPE_CSA_CONF:
            case E_HAL_DSCMB_TYPE_DES:
            case E_HAL_DSCMB_TYPE_AES:
            case E_HAL_DSCMB_TYPE_AES_SCTE52:
            case E_HAL_DSCMB_TYPE_AES_CTR:
                *((MS_U32*)pOutData) = (MS_U32)TRUE;
                break;
            case E_HAL_DSCMB_TYPE_MULTI2:
            default:
                *((MS_U32*)pOutData) = (MS_U32)FALSE;
                break;
        }
        break;
    case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START:
    case E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END:
    default:
        *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        bRet = FALSE;
        break;
    }
    return bRet;
}

MS_U32 HAL_DSCMB_TransFltId2FltGroup0Id(MS_U32 u32FltId)
{
    MS_U32 u32data = u32FltId;

    if(u32data >= TSP_PID_FLT_NUM)
    {
        u32data = u32data - TSP_PID_FLT_NUM + ENG0_MAP_PID_START;
    }

    return u32data;
}

MS_U32 HAL_DSCMB_TransFltGroup0Id2Fltd(MS_U32 u32FltId)
{
    MS_U32 u32data = u32FltId;
    MS_U16 u16data = REG16_R(REG_TSP1_CACTRL);

    if(u16data & REG_TSP1_CACTRL_INPUT_TS1)
    {
        if((u32FltId >= ENG0_MAP_PID_START) && (u32FltId < (ENG0_MAP_PID_START + ENG0_MAP_PID_LENGTH)))
        {
            u32data = u32data - ENG0_MAP_PID_START + TSP_PID_FLT_NUM;
        }
    }

    return u32data;

}

/*
MS_BOOL HAL_DSCMB_SlotAlloc(MS_U32 u32DscmbId, HAL_DscmbKey_Type KeyType, MS_U32* pu32SlotId)
{
    MS_U32 u32Slot;

    // Oberon/Uranus should judge if u32DscmbId has been visited
    u32Slot = MAsm_CPU_GetTrailOne(_u32FreeSlot);
    if (16 <= u32Slot)
    {
        MS_ASSERT(0);
        return FALSE;
    }
    _u32FreeSlot &= ~(1<< u32Slot);
    *pu32SlotId = u32Slot;
    return TRUE;
}

MS_BOOL HAL_DSCMB_SlotFree(MS_U32 u32DscmbId, HAL_DscmbKey_Type KeyType, MS_U32 u32SlotId)
{
    MS_ASSERT(u32SlotId< REG_DSCMB_MAX_SLOT);
    MS_ASSERT(0 == (_u32FreeSlot & (1<< u32SlotId)));
    _u32FreeSlot |= (1<< u32SlotId);
    return TRUE;
}
*/

MS_BOOL HAL_DSCMB_SlotConnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId)
{
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;
    _HAL_DSCMB_PidSlotWrite(u32PidFltId, u32SlotId, KeyType);
    return TRUE;
}

MS_BOOL HAL_DSCMB_SlotDisconnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId)
{
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;
    _HAL_DSCMB_PidSlotWrite(u32PidFltId, 0xFFFFFFFF, KeyType);
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltTypeSet(MS_U32 u32EngId, MS_U32 u32PidFltId, HAL_DSCMB_Type DscmbType, MS_BOOL bDscmb)
{
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;

    switch (DscmbType)
    {
    default:
        MS_ASSERT(0);
        return FALSE;
    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA
        return FALSE;
    case E_HAL_DSCMB_TYPE_CSA: // CSA
    case E_HAL_DSCMB_TYPE_CSA_CONF:        
    case E_HAL_DSCMB_TYPE_DES: // CI+ DES
    case E_HAL_DSCMB_TYPE_AES:
    case E_HAL_DSCMB_TYPE_AES_CTR:
    case E_HAL_DSCMB_TYPE_AES_SCTE52:
    case E_HAL_DSCMB_TYPE_MULTI2:
        _Map[u32PidFltId].DscmbType = DscmbType;
        break;
    }
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltKeySet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8Key)
{
    MS_U32 u32SlotTmp;
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;

    if (FALSE == _HAL_DSCMB_PidSlotRead(u32PidFltId, eKeyType, &u32SlotTmp))
    {
        return FALSE;
    }
    MS_ASSERT(u32SlotTmp == u32Slot);
    switch (DscmbType)
    {
        case E_HAL_DSCMB_TYPE_MULTI2: // CI+ MULTI2 --> 4/5 0 0
        case E_HAL_DSCMB_TYPE_DES: // CI+ DES  --> 4/5 0 0
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 4, 0, FALSE, 0,
                            ((pu8Key[0]) | (pu8Key[1] << 8) | (pu8Key[2] << 16) | (pu8Key[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 5, 0, FALSE, 0,
                            ((pu8Key[4]) | (pu8Key[5] << 8) | (pu8Key[6] << 16) | (pu8Key[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 6, 0, FALSE, 0, 0);
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 7, 0, TRUE, 0, 0);
        break;
    case E_HAL_DSCMB_TYPE_CSA: // CSA   -> 4/5/6/7
    case E_HAL_DSCMB_TYPE_CSA_CONF:        
    case E_HAL_DSCMB_TYPE_AES: // CI+ AES --> 4/5/6/7 IV 0/1/2/3
    case E_HAL_DSCMB_TYPE_AES_SCTE52: //CI+CBC , DBook
    case E_HAL_DSCMB_TYPE_AES_CTR: // AES CTR, for HDCP2

            _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 4, 0, FALSE, 0,
                                    ((pu8Key[0]) | (pu8Key[1] << 8) | (pu8Key[2] << 16) | (pu8Key[3] << 24)));
            _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 5, 0, FALSE, 0,
                                    ((pu8Key[4]) | (pu8Key[5] << 8) | (pu8Key[6] << 16) | (pu8Key[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 6, 0, FALSE, 0,
                                ((pu8Key[8]) | (pu8Key[9] << 8) | (pu8Key[10] << 16) | (pu8Key[11] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 7, 0, TRUE, 0,
                                ((pu8Key[12]) | (pu8Key[13] << 8) | (pu8Key[14] << 16) | (pu8Key[15] << 24)));
        break;

    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA -> 4/5/6/7
    default:
        MS_ASSERT(0);
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltKeyReset(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType)
{
    MS_U32 u32SlotTmp;
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;

    if (FALSE == _HAL_DSCMB_PidSlotRead(u32PidFltId, eKeyType, &u32SlotTmp))
    {
        return FALSE;
    }
    MS_ASSERT(u32SlotTmp == u32Slot);


    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 0, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 1, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 2, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 3, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 4, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 5, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 6, 0, FALSE, 0, 0);
    _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 7, 0, FALSE, 0, 0);
        
    return TRUE;
}

MS_BOOL HAL_DSCMB_FltIVSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV)
{
    MS_U32 u32SlotTmp;
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;

    if (FALSE == _HAL_DSCMB_PidSlotRead(u32PidFltId, eKeyType, &u32SlotTmp))
    {
        return FALSE;
    }
    MS_ASSERT(u32SlotTmp == u32Slot);

    MS_ASSERT(pu8IV);
    switch (DscmbType)
    {
    case E_HAL_DSCMB_TYPE_CSA: // CSA   -> 4/5 0 0
    case E_HAL_DSCMB_TYPE_CSA_CONF:        
    case E_HAL_DSCMB_TYPE_DES: // CI+ DES  --> 4/5 0 0
    case E_HAL_DSCMB_TYPE_NSA_AS_ESA: // NSA as ESA -> 4/5/6/7
    default:
        MS_ASSERT(0);
        return FALSE;
    case E_HAL_DSCMB_TYPE_AES: // CI+ AES --> 4/5/6/7 IV 0/1/2/3
    case E_HAL_DSCMB_TYPE_AES_SCTE52: //CI+ DBook, T8 new
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 0, 0, FALSE, 0,
                                ((pu8IV[0]) | (pu8IV[1] << 8) | (pu8IV[2] << 16) | (pu8IV[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 1, 0, FALSE, 0,
                                ((pu8IV[4]) | (pu8IV[5] << 8) | (pu8IV[6] << 16) | (pu8IV[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 2, 0, FALSE, 0,
                                ((pu8IV[8]) | (pu8IV[9] << 8) | (pu8IV[10] << 16) | (pu8IV[11] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 3, 0, TRUE, 0,
                                ((pu8IV[12]) | (pu8IV[13] << 8) | (pu8IV[14] << 16) | (pu8IV[15] << 24)));
        break;
    case E_HAL_DSCMB_TYPE_MULTI2: // CI+ MULTI2 64bit IV
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 0, 0, FALSE, 0,
                                ((pu8IV[0]) | (pu8IV[1] << 8) | (pu8IV[2] << 16) | (pu8IV[3] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 1, 0, FALSE, 0,
                                ((pu8IV[4]) | (pu8IV[5] << 8) | (pu8IV[6] << 16) | (pu8IV[7] << 24)));
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 2, 0, FALSE, 0, 0);
        _HAL_DSCMB_SlotKeyWrite(u32PidFltId, eKeyType, 3, 0, TRUE, 0, 0);

        break;
    }
    return TRUE;
}

MS_BOOL HAL_DSCMB_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round)
{
    return FALSE;
}

MS_BOOL HAL_DSCMB_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey)
{
    return FALSE;
}

MS_BOOL HAL_DSCMB_HDCP2_SetRiv(MS_U32 u32EngId, MS_U8* pu8RIV)
{
    REG16_W(REG_DSCMB_CIPHER0_HDCP2_RIV0, ((pu8RIV[6]<<8) | pu8RIV[7]));
    REG16_W(REG_DSCMB_CIPHER0_HDCP2_RIV1, ((pu8RIV[4]<<8) | pu8RIV[5]));
    REG16_W(REG_DSCMB_CIPHER0_HDCP2_RIV2, ((pu8RIV[2]<<8) | pu8RIV[3]));
    REG16_W(REG_DSCMB_CIPHER0_HDCP2_RIV3, ((pu8RIV[0]<<8) | pu8RIV[1]));
    return TRUE;
}

MS_BOOL HAL_DSCMB_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, HAL_Scmb_Level* pScmbLevel)
{
    if ((16UL > u32PidFltId) || (32UL<= u32PidFltId))
    {
        return FALSE;
    }
    u32PidFltId -= 16UL;

    if (REG16_R(REG_DSCMB_SCMB_TS) & (1UL<< u32PidFltId))
    {
        *pScmbLevel |= E_HAL_DSCMB_TS;
    }
/*
    if (REG16_R(REG_DSCMB_SCMB_PES) & (1UL<< u32PidFltId))
    {
        *pScmbLevel |= E_HAL_DSCMB_PES;
    }
*/
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
    MS_ASSERT(0 == u32Mode); // MUST be TDES
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
    MS_ASSERT(u32SlotTmp == u32Slot);

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
        MS_ASSERT(0);
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

static MS_BOOL _HAL_DSCMB_PidSlotRead(MS_U32 u32SlotPidFltId, DSCMB_Key_Type KeyType, MS_U32* pu32Slot)
{
    switch (KeyType)
    {
    default:
        MS_ASSERT(0);
            return FALSE;
    case E_DSCMB_KEY_CLEAR:
        return FALSE;
    case E_DSCMB_KEY_EVEN:
        *pu32Slot = _Map[u32SlotPidFltId].u32SlotIdEven;
        break;
    case E_DSCMB_KEY_ODD:
        *pu32Slot = _Map[u32SlotPidFltId].u32SlotIdOdd;
        break;
    }
    return (0xFFFFFFFFUL == (*pu32Slot))? FALSE: TRUE;
}

static MS_BOOL _HAL_DSCMB_PidSlotWrite(MS_U32 u32SlotPidFltId, MS_U32 u32SlotId, DSCMB_Key_Type KeyType)
{
    switch (KeyType)
    {
    default:
        MS_ASSERT(0);
            return FALSE;
    case E_DSCMB_KEY_CLEAR:
        return FALSE;
    case E_DSCMB_KEY_EVEN:
        _Map[u32SlotPidFltId].u32SlotIdEven = u32SlotId;
        break;
    case E_DSCMB_KEY_ODD:
        _Map[u32SlotPidFltId].u32SlotIdOdd = u32SlotId;
        break;
    }
    return TRUE;
}

static MS_BOOL _HAL_DSCMB_SlotKeyWrite(MS_U32 u32SlotPidFltId,
                                       DSCMB_Key_Type eKeyType,
                                       MS_U32 u32Pos,
                                       MS_U32 u32EngFlag, // useless at T2
                                       MS_BOOL bValid,
                                       MS_U32 u32From, // 0 --> ACPU, 1--> KLadder, 2 --> NSK
                                       MS_U32 u32Key)
{
    MS_U32 u32KeyType;
    MS_U32 u32Valid;
    HAL_DSCMB_Type              DscmbType;

    switch (eKeyType)
    {
    default:
        MS_ASSERT(0);
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
    DscmbType = _Map[u32SlotPidFltId].DscmbType;

    if (E_HAL_DSCMB_TYPE_AES == DscmbType)
    {
        REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_CBC_CLR_MODE |REG_DSCMB_CIP_ESA_DECRYPT);
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
    }
    else if (E_HAL_DSCMB_TYPE_DES == DscmbType)
    {
        REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_DES_MODE |REG_DSCMB_CIP_ESA_ECB_MODE | REG_DSCMB_CIP_ESA_DECRYPT);
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
    }
    else if (E_HAL_DSCMB_TYPE_AES_SCTE52 == DscmbType)
    {
        REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_AES_MODE |REG_DSCMB_CIP_ESA_CBC_MODE |REG_DSCMB_CIP_ESA_DECRYPT);
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L)&= ~(REG_DSCMB_CSA_CIP);
    }
    else if (E_HAL_DSCMB_TYPE_CSA == DscmbType)
    {
        REG16_W(REG_DSCMB_KT_CTRL, 0x0);
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST);
        REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
    }
    else if (E_HAL_DSCMB_TYPE_CSA_CONF == DscmbType)
    {
        REG16_W(REG_DSCMB_KT_CTRL, 0x0);
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_OLD_TSC | REG_DSCMB_CTRL_CORRECT_SCRMBFLAG | REG_DSCMB_CTRL_CSA_ENABLE | REG_DSCMB_CTRL_SW_RST | REG_DSCMB_CTRL_CONFORMANCE_MECH);
        REG16_W(REG_DSCMB_CIPHER_CONNECT_L, REG_DSCMB_CSA_CIP);
    }    
    else if (E_HAL_DSCMB_TYPE_AES_CTR == DscmbType) // HDCP2
    {
        REG16_W(REG_DSCMB_CTRL, REG_DSCMB_CTRL_SW_RST);
        REG16_R(REG_DSCMB_CIPHER_CONNECT_L) = REG_DSCMB_CSA_CIP_ENCRYPT;

        REG16_W(REG_DSCMB_KT_CTRL, REG_DSCMB_CIP_ESA_HDCP2_MODE |
                                   REG_DSCMB_CIP_ESA_CTR_MODE |
                                   REG_DSCMB_CIP_ESA_PES_MODE |
                                   REG_DSCMB_CIP_ESA_AES_MODE );
        // Since no scrmb info in HDCP2 stream, HDCP2 mode use esa mode tsc
        if(eKeyType == E_DSCMB_KEY_EVEN)
            REG16_W(REG_DSCMB_CTRL1, (REG16_R(REG_DSCMB_CTRL1) & ~REG_DSCMB_CTRL1_NEW_TSC_MASK) | REG_DSCMB_CTRL1_NEW_TSC_EVEN);
        else
            REG16_W(REG_DSCMB_CTRL1, (REG16_R(REG_DSCMB_CTRL1) & ~REG_DSCMB_CTRL1_NEW_TSC_MASK) | REG_DSCMB_CTRL1_NEW_TSC_ODD);
    }
    else
    {
        return FALSE;
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
        REG16_W(REG_DSCMB_ACPU_START, REG_ACPU_CMD_START);
        REG16_W(REG_DSCMB_ACPU_START+ 1, 0);
        break;
    case 1: // KLadder
        // not support at this stage
    case 2: // NSK
    default:
       MS_ASSERT(0);
       break;
    }
    REG16_W(REG_DSCMB_ACPU_START, 0);
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
        MS_ASSERT(0);
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
        MS_ASSERT(0);
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
