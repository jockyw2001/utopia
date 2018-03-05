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
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halMultiPVR.c
// @brief  Multi-PVR HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
    #include <linux/string.h>
#else
    #include <string.h>
#endif

#include    "MsCommon.h"
#include    "regMultiPVR.h"
#include    "halMultiPVR.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_MULTI_PVR_ENG_Ctrl   *_RegMultiPvrCtrl   = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U32 REG32_R(REG32_MULTI_PVR *reg)
{
    MS_U32              value = 0;
    value  = (reg)->low;
    value |= (reg)->high << 16;
    return value;
}

static MS_U16 REG16_R(REG16_MULTI_PVR *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

#define REG32_W(reg, value)             {   (reg)->low = ((value) & 0x0000FFFF);    \
                                            (reg)->high = ((value) >> 16); }

#define REG16_W(reg, value)             {   (reg)->data = ((value) & 0x0000FFFF);   }

#define _AND_(flag, bit)                ((flag) & (bit))

#define _SET_(flag, bit)                ((flag) | (bit))
#define _CLR_(flag, bit)                ((flag) & (~(bit)))

#define REG16_SET(reg, value)           REG16_W(reg, _SET_(REG16_R(reg), value))
#define REG32_SET(reg, value)           REG32_W(reg, _SET_(REG32_R(reg), value))
#define REG16_CLR(reg, value)           REG16_W(reg, _CLR_(REG16_R(reg), value))
#define REG32_CLR(reg, value)           REG32_W(reg, _CLR_(REG32_R(reg), value))

#define REG16_MSK_W(reg, mask, value)   REG16_W((reg), _CLR_(REG16_R(reg), (mask)) | _AND_((value), (mask)))
#define REG32_MSK_W(reg, mask, value)   REG32_W((reg), _CLR_(REG32_R(reg), (mask)) | _AND_((value), (mask)))

typedef struct
{
    MS_U16  VCID        : 8;
    MS_U16  RdPos       : 3;
    MS_U16  Clr         : 1;
    MS_U16  RW          : 1;
    MS_U16  Active      : 1;
    MS_U16  RdAddrLsb   : 1;
    MS_U16  PingpongEn  : 1;
} ST_ACPU_CMD;

typedef struct
{
    MS_U16  AlignEn     : 1;
    MS_U16  MiuSel      : 2;
    MS_U16  IntEn       : 1;
    MS_U16  MOBF        : 5;
    MS_U16  SecureFlag  : 1;
    MS_U16  TimestampEn : 1;
    MS_U16  Padding     : 5;
} ST_ACPU_FLAG;

static void _HAL_MultiPVR_IdrW(ST_ACPU_CMD stCmd, ST_ACPU_FLAG stFlag, MS_U32 u32Head, MS_U32 u32Tail)
{
    REG32_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_43_44, u32Head);
    REG32_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_45_46, u32Tail);
    REG16_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_41, *((MS_U16*)&stCmd));
    REG16_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_42, *((MS_U16*)&stFlag));
    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_40, CFG_MULTI_PVR_40_REG_ACPU_ACTIVE);
}

//acpu_rd_position = 0 (used size)
//                 = 1 (acpu_flag)
//                 = 4 (tail)
//                 = 5 (head)
static MS_U32 _HAL_MultiPVR_IdrR(MS_U32 u32ChId, MS_U8 u8RdPos, MS_BOOL bRdAddrLsb)
{
    ST_ACPU_CMD stCmd;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.RdPos = u8RdPos;
    stCmd.RdAddrLsb = bRdAddrLsb;
    REG16_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_41, *((MS_U16*)&stCmd));
    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_40, CFG_MULTI_PVR_40_REG_ACPU_ACTIVE);

    return REG32_R(&_RegMultiPvrCtrl->CFG_MULTI_PVR_47_48);
}

#define MULTI_PVR_GET_USED_SIZE0(ChId)      ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 0, 0); u32RetVal; })
#define MULTI_PVR_GET_USED_SIZE1(ChId)      ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 0, 1); u32RetVal; })
#define MULTI_PVR_GET_ACPU_FLAG(ChId)       ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 1, 0) >> 16; u32RetVal; })
#define MULTI_PVR_GET_HEAD0(ChId)           ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 5, 0); u32RetVal; })
#define MULTI_PVR_GET_TAIL0(ChId)           ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 4, 0); u32RetVal; })
#define MULTI_PVR_GET_HEAD1(ChId)           ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 5, 1); u32RetVal; })
#define MULTI_PVR_GET_TAIL1(ChId)           ({ MS_U32  u32RetVal = _HAL_MultiPVR_IdrR(ChId, 4, 1); u32RetVal; })

#define MULTI_PVR_ENG_CHK(PvrEng, RetVal)   if(PvrEng >= TSP_MULTI_PVR_ENG_NUM)                                                     \
                                            {                                                                                       \
                                                HAL_MULTI_PVR_DBGMSG(E_HAL_MULTI_PVR_DBG_LEVEL_ERR,                                 \
                                                                     E_HAL_MULTI_PVR_DBG_MODEL_ALL,                                 \
                                                                     printf("[MULTI_PVR_ERR][%s][%d] Wrong PVR Engine : 0x%x !!\n", \
                                                                            __FUNCTION__, __LINE__, PvrEng)                         \
                                                                    );                                                              \
                                                return RetVal;                                                                      \
                                            }

#define MULTI_PVR_CHID_CHK(PvrChId, RetVal) if(PvrChId >= TSP_MULTI_PVR_CH_NUM)                                                     \
                                            {                                                                                       \
                                                HAL_MULTI_PVR_DBGMSG(E_HAL_MULTI_PVR_DBG_LEVEL_ERR,                                 \
                                                                     E_HAL_MULTI_PVR_DBG_MODEL_ALL,                                 \
                                                                     printf("[MULTI_PVR_ERR][%s][%d] Wrong PVR ChId : 0x%x !!\n",   \
                                                                            __FUNCTION__, __LINE__, PvrChId)                        \
                                                                    );                                                              \
                                                return RetVal;                                                                      \
                                            }

//-------------------------------------------------------------------------------------------------
//  Debug Message
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_MULTI_PVR_DBG_LEVEL_NONE,     // no debug message shown
    E_HAL_MULTI_PVR_DBG_LEVEL_ERR,      // only shows error message that can't be recover
    E_HAL_MULTI_PVR_DBG_LEVEL_WARN,     // error case can be recover, like retry
    E_HAL_MULTI_PVR_DBG_LEVEL_EVENT,    // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_HAL_MULTI_PVR_DBG_LEVEL_INFO,     // information for internal parameter
    E_HAL_MULTI_PVR_DBG_LEVEL_FUNC,     // Function trace and input parameter trace
    E_HAL_MULTI_PVR_DBG_LEVEL_TRACE,    // debug trace
} EN_HAL_MULTI_PVR_DBGMSG_LEVEL;

typedef enum
{
    E_HAL_MULTI_PVR_DBG_MODEL_NONE,     // @temporarily , need to refine
    E_HAL_MULTI_PVR_DBG_MODEL_ALL,
} EN_HAL_MULTI_PVR_DBGMSG_MODEL;

#define HAL_MULTI_PVR_DBGMSG(_level,_model,_f) do {if(_u32MultiPvrDbgLevel >= (_level)&&((_u32MultiPvrDbgModel&_model)!=0)) (_f);} while(0)
static MS_U32  _u32MultiPvrDbgLevel = E_HAL_MULTI_PVR_DBG_LEVEL_ERR;
static MS_U32  _u32MultiPvrDbgModel = E_HAL_MULTI_PVR_DBG_MODEL_ALL;


void HAL_MultiPVR_SetBank(MS_VIRT u32BankAddr)
{
    _RegMultiPvrCtrl = (REG_MULTI_PVR_ENG_Ctrl*)(u32BankAddr + 0xC1400);    //MultiPVR: 0x160A
}

void HAL_MultiPVR_Init(MS_U32 u32PVREng, MS_U32 pktDmxId)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    // input src
    REG16_MSK_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_71, CFG_MULTI_PVR_71_REG_INPUT_SRC_MASK, (((MS_U16)pktDmxId) << CFG_MULTI_PVR_71_REG_INPUT_SRC_SHIFT));

    // record ts
    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_RECORD_TS);

    // record null
    REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_DIS_NULL_PKT);

    // enbale Multi-PVR
    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_STR2MI_EN);
}

void HAL_MultiPVR_Exit(MS_U32 u32PVREng)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    // disable Multi-PVR
    REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_STR2MI_EN);

    // clear input src
    REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_71, CFG_MULTI_PVR_71_REG_INPUT_SRC_MASK);

    // reset record ts
    REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_RECORD_TS);

    // reset record null
    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_DIS_NULL_PKT);
}

void HAL_MultiPVR_FlushData(MS_U32 u32PVREng)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_DMA_FLUSH_EN);
    REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_DMA_FLUSH_EN);
}

void HAL_MultiPVR_Skip_Scrmb(MS_U32 u32PVREng, MS_BOOL bSkip)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    if(bSkip)
    {
        REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_MASK_SCR_PVR_EN);
    }
    else
    {
        REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_MASK_SCR_PVR_EN);
    }
}

void HAL_MultiPVR_Block_Dis(MS_U32 u32PVREng, MS_BOOL bDisable)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    if(bDisable)
    {
        REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_PVR_BLOCK_DISABLE);
    }
    else
    {
        REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_PVR_BLOCK_DISABLE);
    }
}

void HAL_MultiPVR_BurstLen(MS_U32 u32PVREng, MS_U16 u16BurstMode)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    REG16_MSK_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_BURST_LEN_MASK, (u16BurstMode << CFG_MULTI_PVR_00_REG_PVR_BURST_LEN_SHIFT));
}

void HAL_MultiPVR_Start(MS_U32 u32PVREng, MS_U32 u32ChId)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);
    MULTI_PVR_CHID_CHK(u32ChId,);

    ST_ACPU_CMD stCmd;
    ST_ACPU_FLAG stFlag;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    memset(&stFlag, 0, sizeof(ST_ACPU_FLAG));

    // read head , tail
    MS_U32  u32Head0 = MULTI_PVR_GET_HEAD0(u32ChId);
    MS_U32  u32Head1 = MULTI_PVR_GET_HEAD1(u32ChId);
    MS_U32  u32Tail0 = MULTI_PVR_GET_TAIL0(u32ChId);
    MS_U32  u32Tail1 = MULTI_PVR_GET_TAIL1(u32ChId);

    // read acpu_flag
    *((MS_U16*)&stFlag) = MULTI_PVR_GET_ACPU_FLAG(u32ChId);

    // write pingpong buf #0
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.Clr = 1;
    stCmd.RW = 1;
    stCmd.Active = 1;   // PVR
    stCmd.RdAddrLsb = 0;
    stCmd.PingpongEn = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head0, u32Tail0);

    // write pingpong buf #1
    stCmd.RdAddrLsb = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head1, u32Tail1);
}

void HAL_MultiPVR_Stop(MS_U32 u32PVREng, MS_U32 u32ChId)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);
    MULTI_PVR_CHID_CHK(u32ChId,);

    ST_ACPU_CMD stCmd;
    ST_ACPU_FLAG stFlag;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    memset(&stFlag, 0, sizeof(ST_ACPU_FLAG));

    // read head , tail
    MS_U32  u32Head0 = MULTI_PVR_GET_HEAD0(u32ChId);
    MS_U32  u32Head1 = MULTI_PVR_GET_HEAD1(u32ChId);
    MS_U32  u32Tail0 = MULTI_PVR_GET_TAIL0(u32ChId);
    MS_U32  u32Tail1 = MULTI_PVR_GET_TAIL1(u32ChId);

    // read acpu_flag
    *((MS_U16*)&stFlag) = MULTI_PVR_GET_ACPU_FLAG(u32ChId);

    // write pingpong buf #0
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.Clr = 1;
    stCmd.RW = 1;
    stCmd.Active = 0;   // drop pkt
    stCmd.RdAddrLsb = 0;
    stCmd.PingpongEn = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head0, u32Tail0);

    // write pingpong buf #1
    stCmd.RdAddrLsb = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head1, u32Tail1);
}

void HAL_MultiPVR_Pause(MS_U32 u32PVREng, MS_BOOL bPause)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    if(bPause)
    {
        REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_STR2MI_PAUSE);
    }
    else
    {
        REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_00, CFG_MULTI_PVR_00_REG_PVR_STR2MI_PAUSE);
    }
}

void HAL_MultiPVR_RecPid(MS_U32 u32PVREng, MS_BOOL bSet)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    if(bSet)
    {
        REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_RECORD_ALL);
    }
    else
    {
        REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_RECORD_ALL);
    }
}

void HAL_MultiPVR_RecNull(MS_U32 u32PVREng, MS_BOOL bSet)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);

    if(bSet)
    {
        REG16_CLR(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_DIS_NULL_PKT);
    }
    else
    {
        REG16_SET(&_RegMultiPvrCtrl->CFG_MULTI_PVR_70, CFG_MULTI_PVR_70_REG_DIS_NULL_PKT);
    }
}

void HAL_MultiPVR_SetStr2Miu_StartAddr(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);
    MULTI_PVR_CHID_CHK(u32ChId,);

    ST_ACPU_CMD stCmd;
    ST_ACPU_FLAG stFlag;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    memset(&stFlag, 0, sizeof(ST_ACPU_FLAG));

    // read tail
    MS_U32  u32Tail0 = MULTI_PVR_GET_TAIL0(u32ChId);
    MS_U32  u32Tail1 = MULTI_PVR_GET_TAIL1(u32ChId);

    // read acpu_flag
    *((MS_U16*)&stFlag) = MULTI_PVR_GET_ACPU_FLAG(u32ChId);

    // write pingpong buf #0
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.Clr = 1;
    stCmd.RW = 1;
    stCmd.Active = 0;   // drop pkt
    stCmd.RdAddrLsb = 0;
    stCmd.PingpongEn = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32StartAddr0, u32Tail0);

    // write pingpong buf #1
    stCmd.RdAddrLsb = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32StartAddr1, u32Tail1);
}

void HAL_MultiPVR_SetStr2Miu_EndAddr(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);
    MULTI_PVR_CHID_CHK(u32ChId,);

    ST_ACPU_CMD stCmd;
    ST_ACPU_FLAG stFlag;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    memset(&stFlag, 0, sizeof(ST_ACPU_FLAG));

    // read head
    MS_U32  u32Head0 = MULTI_PVR_GET_HEAD0(u32ChId);
    MS_U32  u32Head1 = MULTI_PVR_GET_HEAD1(u32ChId);

    // read acpu_flag
    *((MS_U16*)&stFlag) = MULTI_PVR_GET_ACPU_FLAG(u32ChId);

    // write pingpong buf #0
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.Clr = 1;
    stCmd.RW = 1;
    stCmd.Active = 0;   // drop pkt
    stCmd.RdAddrLsb = 0;
    stCmd.PingpongEn = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head0, u32EndAddr0);

    // write pingpong buf #1
    stCmd.RdAddrLsb = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head1, u32EndAddr1);
}

MS_U32 HAL_MultiPVR_GetWritePtr(MS_U32 u32PVREng, MS_U32 u32ChId)
{
    MULTI_PVR_ENG_CHK(u32PVREng, 0);
    MULTI_PVR_CHID_CHK(u32ChId, 0);

    MS_U32  u32UsedSize0 = MULTI_PVR_GET_USED_SIZE0(u32ChId);

    if(u32UsedSize0 == 0)
    {
        MS_U32  u32UsedSize1 = MULTI_PVR_GET_USED_SIZE1(u32ChId);

        if(u32UsedSize1 == 0)
        {
            REG16_MSK_W(&_RegMultiPvrCtrl->CFG_MULTI_PVR_50, CFG_MULTI_PVR_50_REG_SGDMA_OUT_VC_STATUS_SEL_MASK, ((MS_U16)u32ChId << CFG_MULTI_PVR_50_REG_SGDMA_OUT_VC_STATUS_SEL_SHIFT));
            MS_BOOL bPingpongPtr = (REG16_R(&_RegMultiPvrCtrl->CFG_MULTI_PVR_51) & CFG_MULTI_PVR_51_REG_SGDMA_OUT_VC_STATUS_PINGPONG_PTR);

            if(bPingpongPtr)
            {
                return MULTI_PVR_GET_HEAD1(u32ChId);
            }
            else
            {
                return MULTI_PVR_GET_HEAD0(u32ChId);
            }
        }
        else
        {
            return (MULTI_PVR_GET_HEAD1(u32ChId) + u32UsedSize1);
        }
    }
    else
    {
        return (MULTI_PVR_GET_HEAD0(u32ChId) + u32UsedSize0);
    }
}

void HAL_MultiPVR_SetStrPacketMode(MS_U32 u32PVREng, MS_U32 u32ChId, MS_BOOL bSet)
{
    MULTI_PVR_ENG_CHK(u32PVREng,);
    MULTI_PVR_CHID_CHK(u32ChId,);

    ST_ACPU_CMD stCmd;
    ST_ACPU_FLAG stFlag;

    memset(&stCmd, 0, sizeof(ST_ACPU_CMD));
    memset(&stFlag, 0, sizeof(ST_ACPU_FLAG));

    // read head , tail
    MS_U32  u32Head0 = MULTI_PVR_GET_HEAD0(u32ChId);
    MS_U32  u32Head1 = MULTI_PVR_GET_HEAD1(u32ChId);
    MS_U32  u32Tail0 = MULTI_PVR_GET_TAIL0(u32ChId);
    MS_U32  u32Tail1 = MULTI_PVR_GET_TAIL1(u32ChId);

    // read acpu_flag
    *((MS_U16*)&stFlag) = MULTI_PVR_GET_ACPU_FLAG(u32ChId);

    stFlag.TimestampEn = !!bSet;    // 188 or 192

    // write pingpong buf #0
    stCmd.VCID = u32ChId & 0xFF;
    stCmd.Clr = 1;
    stCmd.RW = 1;
    stCmd.Active = 0;   // drop pkt
    stCmd.RdAddrLsb = 0;
    stCmd.PingpongEn = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head0, u32Tail0);

    // write pingpong buf #1
    stCmd.RdAddrLsb = 1;
    _HAL_MultiPVR_IdrW(stCmd, stFlag, u32Head1, u32Tail1);
}

void HAL_MultiPVR_SetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32Stamp)
{
    // Not support...
    // (1) live-in: use FIQ timestamp
    // (2) file-in: can't write to PVR
}

MS_U32 HAL_MultiPVR_GetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32ChId)
{
    // Not support...
    // (1) live-in: use FIQ timestamp
    // (2) file-in: can't write to PVR
    return 0;
}
