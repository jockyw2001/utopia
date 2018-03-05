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
// file   halOTV.c
// @brief  OTV HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "regOTV.h"
#include "halOTV.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT       _u32RegBase                        = 0;

static REG_OTV*      _OTVReg[OTV_ENG_NUM]                 = {NULL , NULL, NULL, NULL};
static MS_VIRT       _u32OTV_PidfltBase[OTV_ENG_NUM]      = {NULL , NULL, NULL, NULL};
static MS_VIRT       _u32OTV_EventMaskBase[OTV_ENG_NUM]   = {NULL , NULL, NULL, NULL};
//static MS_U32      _u32OTV_PayloadMaskBase[OTV_ENG_NUM] = {NULL , NULL, NULL, NULL};

// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_OTV_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                    (reg)->H = ((value) >> 16); } while(0)

#define _HAL_OTV_REG16_W(reg, value)    (reg)->data = ((value) & 0x0000FFFF);


#define OTV_NUM_CHECK(idx) if( (MS_U32)idx >= (MS_U32)OTV_ENG_NUM ) \
                            { printf("[OTV][ERR] OTV Engine [%d] not exist ! \n",(unsigned int)idx);   \
                             return ; }

#define OTV_NUM_CHECK_RET(idx) if( (MS_U32)idx >= (MS_U32)OTV_ENG_NUM ) \
                            { printf("[OTV][ERR] OTV Engine [%d] not exist ! \n",(unsigned int)idx);   \
                             return 0; }


//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
#if 0
static void _delay(void)
{
    volatile MS_U32 i;
    for (i = 0; i< 0xFFFF; i++);
}
#endif
static MS_U32 _HAL_OTV_REG32_R(OTV_REG32 *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_OTV_REG16_R(OTV_REG16 *reg)
{
    MS_U16     value;
    value = (reg)->data;
    return value;
}

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
void HAL_OTV_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase                 = u32BankAddr;

    _OTVReg[0]                = (REG_OTV*)(_u32RegBase + OTV0_REG_CTRL_BASE);
    _u32OTV_PidfltBase[0]        = _u32RegBase + OTV0_PIDFLT_BASE;
    _u32OTV_EventMaskBase[0]     = _u32RegBase + OTV0_EVENT_MASK_BASE;
    //_u32OTV_PayloadMaskBase[0]   = _u32RegBase + OTV0_BANK1_PAYLOAD_MASK_BASE;

    _OTVReg[1]                = (REG_OTV*)(_u32RegBase + OTV1_REG_CTRL_BASE);
    _u32OTV_PidfltBase[1]        = _u32RegBase + OTV1_PIDFLT_BASE;
    _u32OTV_EventMaskBase[1]     = _u32RegBase + OTV1_EVENT_MASK_BASE;
    //_u32OTV_PayloadMaskBase[1]   = _u32RegBase + OTV1_BANK1_PAYLOAD_MASK_BASE;

    _OTVReg[2]                = (REG_OTV*)(_u32RegBase + OTV2_REG_CTRL_BASE);
    _u32OTV_PidfltBase[2]        = _u32RegBase + OTV2_PIDFLT_BASE;
    _u32OTV_EventMaskBase[2]     = _u32RegBase + OTV2_EVENT_MASK_BASE;

    _OTVReg[3]                = (REG_OTV*)(_u32RegBase + OTV3_REG_CTRL_BASE);
    _u32OTV_PidfltBase[3]        = _u32RegBase + OTV3_PIDFLT_BASE;
    _u32OTV_EventMaskBase[3]     = _u32RegBase + OTV3_EVENT_MASK_BASE;

}

// OTV Init

void HAL_OTV_Init(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK(u32OTVEng);

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0, _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0), OTV_SW_RESET));
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0, _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0), OTV_SW_RESET));

    // filter event reset
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3, _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_EVENT_FLT_RST));
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3, _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_EVENT_FLT_RST));
}

#if 0
void HAL_OTV_Exit(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK(u32OTVEng);

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0, _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0), OTV_SW_RESET));
}
#endif

// OTV Config Setting

#if 0

void HAL_OTV_FirstPktTimerBase_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable) // packet timer will be (orginal timer - first pkt timer)
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_FIRST_PKT_TIMER_BASE_EN));
    }
    else        // OTV packet timestamp will match OTV local timstamp
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_FIRST_PKT_TIMER_BASE_EN));
    }
}

#endif



void HAL_OTV_PktTimerSrcFromPVR_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable) // OTV packet timer will match PVR timstamp
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_TIMESTAMP_PVR_TO_OTV_EN));
    }
    else        // OTV packet timer will match OTV local timstamp
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_TIMESTAMP_PVR_TO_OTV_EN));
    }
}


void HAL_OTV_PktNumSrcFromPktDemux_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable) // OTV packet number will count by pkt dmx hit, pkt_num index start from 1,2,3,...etc
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_SRC_SEL));
    }
    else        // OTV packet number will count by OTV pid hit, pkt_num index start from 0,1,2,...etc
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_SRC_SEL));
    }
}



// OTV Interrupt

void HAL_OTV_INT_Timer_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable)
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_INT_TIMER_EN));
    }
    else
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_INT_TIMER_EN));
    }
}

void HAL_OTV_INT_Event_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable)
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_INT_EVENT_EN));
    }
    else
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2), OTV_INT_EVENT_EN));
    }
}

void HAL_OTV_INT_SetEventWaterLevel(MS_U32 u32OTVEng, MS_U32 u32EventWaterLevel)
{
    OTV_NUM_CHECK(u32OTVEng);

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,
        _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2),(u32EventWaterLevel<<OTV_INT_EVENT_WATER_SHIFT)&OTV_INT_EVENT_WATER_MASK));
}

void HAL_OTV_INT_SetTimerWaterLevel(MS_U32 u32OTVEng, MS_U32 u32TimerWaterLevel)
{
    OTV_NUM_CHECK(u32OTVEng);

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2,
        _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL2),(u32TimerWaterLevel<<OTV_INT_TIME_WATER_SHIFT)&OTV_INT_TIME_WATER_MASK));
}

MS_BOOL HAL_OTV_INT_GetEventHW(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK_RET(u32OTVEng);

    MS_U16 u16IntFlag;  // OTV Interrupt Flag

    u16IntFlag = _HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_INT_Flag) & OTV_INT_FLAG_MASK;

    switch(u32OTVEng)
    {
        case 0:
            if( u16IntFlag & OTV_INT_FROM_OTV0_EVENT )
                return TRUE;
            else
                return FALSE;
        case 1:
            if( u16IntFlag & OTV_INT_FROM_OTV1_EVENT )
                return TRUE;
            else
                return FALSE;
        case 2:
            if( u16IntFlag & OTV_INT_FROM_OTV2_EVENT )
                return TRUE;
            else
                return FALSE;
        case 3:
            if( u16IntFlag & OTV_INT_FROM_OTV3_EVENT )
                return TRUE;
            else
                return FALSE;
        default:
            return FALSE;
    }
}

MS_BOOL HAL_OTV_INT_GetTimerHW(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK_RET(u32OTVEng);

    MS_U16 u16IntFlag;  // OTV Interrupt Flag

    u16IntFlag = _HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_INT_Flag) & OTV_INT_FLAG_MASK;

    switch(u32OTVEng)
    {
        case 0:
            if( u16IntFlag & OTV_INT_FROM_OTV0_TIMER )
                return TRUE;
            else
                return FALSE;
        case 1:
            if( u16IntFlag & OTV_INT_FROM_OTV1_TIMER )
                return TRUE;
            else
                return FALSE;
        case 2:
            if( u16IntFlag & OTV_INT_FROM_OTV2_TIMER )
                return TRUE;
            else
                return FALSE;
        case 3:
            if( u16IntFlag & OTV_INT_FROM_OTV3_TIMER )
                return TRUE;
            else
                return FALSE;
        default:
            return FALSE;
    }
}


void HAL_OTV_INT_ClrEventHW(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK(u32OTVEng);

    switch(u32OTVEng)
    {
        case 0:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV0_EVENT));
            break;
        case 1:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV1_EVENT));
            break;
        case 2:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV2_EVENT));
            break;
        case 3:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV3_EVENT));
            break;
        default:
            break;
    }
}

void HAL_OTV_INT_ClrTimerHW(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK(u32OTVEng);

    switch(u32OTVEng)
    {
        case 0:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV0_TIMER));
            break;
        case 1:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV1_TIMER));
            break;
        case 2:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV2_TIMER));
            break;
        case 3:
            _HAL_OTV_REG16_W(&_OTVReg[0][0].OTV_INT_Flag,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[0][0].OTV_INT_Flag), OTV_INT_FROM_OTV3_TIMER));
            break;
        default:
            break;
    }
}



// OTV Event Queue

void HAL_OTV_EventQ_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable)
{
    OTV_NUM_CHECK(u32OTVEng);

    if(bEnable)
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0), OTV_REC_EFRAME_EN));
    }
    else
    {
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL0), OTV_REC_EFRAME_EN));
    }
}


void HAL_OTV_EventQ_GetEventNum(MS_U32 u32OTVEng, MS_U32 *pu32EventNum)
{
    OTV_NUM_CHECK(u32OTVEng);

    *pu32EventNum = _HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_EventLogCtrlStatus) & OTV_EVENT_FIFO_NUM_MASK;
}


MS_BOOL HAL_OTV_EventQ_PopEvent(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK_RET(u32OTVEng);

    MS_U32 u32EventNum;

    HAL_OTV_EventQ_GetEventNum(u32OTVEng, &u32EventNum);

    if (u32EventNum != 0)
    {
        // Event Queue Read
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1), OTV_EFRAME_RD));
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1), OTV_EFRAME_RD));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void HAL_OTV_EventQ_GetEventDesciptor(MS_U32 u32OTVEng, MS_U32 *pu32EventDesciptor)
{
    OTV_NUM_CHECK(u32OTVEng);
    *pu32EventDesciptor = _HAL_OTV_REG32_R(&_OTVReg[u32OTVEng][0].OTV_EventDescriptor);
}

void HAL_OTV_EventQ_GetEventPktNum(MS_U32 u32OTVEng, MS_U32 *pu32EventPktNum)
{
    OTV_NUM_CHECK(u32OTVEng);
    *pu32EventPktNum = _HAL_OTV_REG32_R(&_OTVReg[u32OTVEng][0].OTV_EventPktNum);
}


void HAL_OTV_EventQ_GetEventPktTimer(MS_U32 u32OTVEng, MS_U32 *pu32EventPktTimer)
{
    OTV_NUM_CHECK(u32OTVEng);
    *pu32EventPktTimer = _HAL_OTV_REG32_R(&_OTVReg[u32OTVEng][0].OTV_EventPktTimer);
}


void HAL_OTV_EventQ_GetEventPktPCR(MS_U32 u32OTVEng, MS_U32 *pu32EventPktPCR)
{
    OTV_NUM_CHECK(u32OTVEng);
    *pu32EventPktPCR = _HAL_OTV_REG32_R(&_OTVReg[u32OTVEng][0].OTV_EventPktPCR);
}


void HAL_OTV_EventQ_GetEventPktPID(MS_U32 u32OTVEng, MS_U16 *pu16EventPktPID)
{
    OTV_NUM_CHECK(u32OTVEng);
    *pu16EventPktPID = _HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_EventPktPID);
}





#if 0
MS_BOOL HAL_OTV_EventQ_Pop(MS_U32 u32OTVEng,MS_U32 *DataArray, MS_U32 ArrSize) // [NOTE] NDS structure for 20 bytes..
{
    OTV_NUM_CHECK_RET(u32OTVEng);

    MS_U32 u32EventNum;

    HAL_OTV_EventQ_GetNum(u32OTVEng, &u32EventNum);

    if (u32EventNum != 0)
    {
        // Event Queue Read
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1), OTV_EFRAME_RD));
        _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL1), OTV_EFRAME_RD));

        if (ArrSize >= 5 )
        {
            DataArray[0] = _HAL_OTV_REG32_R(&(_OTVReg[u32OTVEng][0].OTV_EventDescriptor));
            DataArray[1] = _HAL_OTV_REG32_R(&(_OTVReg[u32OTVEng][0].OTV_EventPktNum));
            DataArray[2] = _HAL_OTV_REG32_R(&(_OTVReg[u32OTVEng][0].OTV_EventPktTimer));
            DataArray[3] = _HAL_OTV_REG32_R(&(_OTVReg[u32OTVEng][0].OTV_EventPktPCR));
            DataArray[4] = (MS_U32)_HAL_OTV_REG16_R(&(_OTVReg[u32OTVEng][0].OTV_EventPktPID));
            return TRUE;
        }
        else
        {
             // array size not enough
             return FALSE;
        }
    }
    else
    {
         return FALSE;
    }
}
#endif


// OTV Get Current Packet Number/TimeStamp

void HAL_OTV_GetCurPktTimer(MS_U32 u32OTVEng, MS_U32 *pu32PktTimer)
{
    OTV_NUM_CHECK(u32OTVEng);

    // latch bit:pkt num/timer will be locked for reading if this bit is set to 1
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_TIMER_LOCK));

    *pu32PktTimer =  _HAL_OTV_REG32_R(&_OTVReg[u32OTVEng][0].OTV_PktTimer);

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_TIMER_LOCK));

}

void HAL_OTV_GetCurPktNum(MS_U32 u32OTVEng, MS_U32 *pu32PktNum)
{
    OTV_NUM_CHECK(u32OTVEng);

    // latch bit:pkt num/timer will be locked for reading if this bit is set to 1
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_TIMER_LOCK));

    *pu32PktNum =  _HAL_OTV_REG32_R(&(_OTVReg[u32OTVEng][0].OTV_PktNum));

    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_PKT_NUM_TIMER_LOCK));
}

// OTV Pid Filter

void HAL_OTV_SetFltPid(MS_U32 u32OTVEng , MS_U32 u32FltId , MS_U16 u16Pid)
{
    OTV_NUM_CHECK(u32OTVEng);

    MS_VIRT _u32PidFltReg = _u32OTV_PidfltBase[u32OTVEng] + (u32FltId*0x04);
    _HAL_OTV_REG16_W((OTV_REG16 *)_u32PidFltReg, u16Pid);
}

void HAL_OTV_GetFltPid(MS_U32 u32OTVEng , MS_U32 u32FltId , MS_U16 *pu16Pid)
{
    OTV_NUM_CHECK(u32OTVEng);

    MS_VIRT _u32PidfltReg = _u32OTV_PidfltBase[u32OTVEng] + (u32FltId*0x04);
    *pu16Pid = _HAL_OTV_REG16_R((OTV_REG16 *)_u32PidfltReg);
}


void HAL_OTV_SetFltEvent(MS_U32 u32OTVEng , MS_U32 u32FltId, MS_U32 u32FltEvent)
{
    OTV_NUM_CHECK(u32OTVEng);
    MS_VIRT _u32EventMaskReg = _u32OTV_EventMaskBase[u32OTVEng] + (u32FltId*0x08);
    _HAL_OTV_REG32_W((OTV_REG32 *)_u32EventMaskReg, u32FltEvent);

}

void HAL_OTV_GetFltEvent(MS_U32 u32OTVEng , MS_U32 u32FltId, MS_U32 *pu32FltEvent)
{
    OTV_NUM_CHECK(u32OTVEng);
    MS_VIRT _u32EventMaskReg = _u32OTV_EventMaskBase[u32OTVEng] + (u32FltId*0x08);
    *pu32FltEvent =  _HAL_OTV_REG32_R((OTV_REG32 *)_u32EventMaskReg) & OTV_EVENT_MASK ;

}

void HAL_OTV_RstFltEvent(MS_U32 u32OTVEng)
{
    OTV_NUM_CHECK(u32OTVEng);
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _SET_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_EVENT_FLT_RST));
    _HAL_OTV_REG16_W(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3,  _CLR_(_HAL_OTV_REG16_R(&_OTVReg[u32OTVEng][0].OTV_HW_CTRL3), OTV_EVENT_FLT_RST));
}
