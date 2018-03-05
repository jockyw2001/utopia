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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvNDS.c
/// @brief  NDS Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"

#include "asmCPU.h"
#include "regNDS.h"
#include "drvNDS.h"
#include "halNDS.h"

#include "ddiNDS_HDI.h"

#include "nds.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/delay.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define NDS_SC_FIFO_SIZE                (256 + 64)                      // Rx/Tx fifo size
#define NDS_SC_RX_REQ_MAX               32
#define NDS_SC_RX_TIMEOUT               1500//1600// 1000ms
#define NDS_SC_VCC_DELAY                10

#define NDS_SC_CHK_TIMER                500

#define NDS_SC_EVENT_CARD               0x00000001      // 0x00000010-0x00000019
#define NDS_SC_EVENT_RX                 0x00000010
#define NDS_SC_EVENT_RX_OVERFLOW        0x00000020
#define NDS_SC_EVENT_RX_ERROR           0x00000040

#define NDS_SC_OS_AttachInterrupt(isr)  MsOS_AttachInterrupt(NDS_INT_UART_CA, isr)
#define NDS_SC_OS_EnableInterrupt()     MsOS_EnableInterrupt(NDS_INT_UART_CA);
#define NDS_SC_OS_DisableInterrupt()    MsOS_DisableInterrupt(NDS_INT_UART_CA);
#define NDS_SC_OS_DetachInterrupt()     MsOS_DetachInterrupt(NDS_INT_UART_CA);

#define NDS_SC_LOCK()                   { MsOS_ObtainMutex(_nds_sc_dev.s32Mutex, MSOS_WAIT_FOREVER); }
#define NDS_SC_UNLOCK()                 { MsOS_ReleaseMutex(_nds_sc_dev.s32Mutex); }
#define NDS_SC_ENTRY()                  { }
#define NDS_SC_RETURN(_ret)             { return _ret; }
#define NDS_SC_RXQ_LOCK()                { MsOS_ObtainMutex(_nds_sc_dev.s32MutexIdRx, MSOS_WAIT_FOREVER); }
#define NDS_SC_RXQ_UNLOCK()              { MsOS_ReleaseMutex(_nds_sc_dev.s32MutexIdRx); }

#define SC_RX_TIMEOUT_ENABLE            0
#define SC_FLOWCTRL_ENABLE              1

#if 1 //defined(MS_DEBUG)
#define NDS_SC_DBG(_fmt, _args...)      printf(_fmt, ##_args)
#else
#define NDS_SC_DBG(_fmt, _args...)      { }
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct _NDS_SC_Dev
{
    // device info

    // device identification

    // device context
    MS_BOOL                         bOpened;
    MS_BOOL                         bCardRdy;                           // since Vcc is controlled by user
    MS_BOOL                         bAborting;

    MS_U8                           Rxfifo[NDS_SC_FIFO_SIZE];
    MS_U8                           Txfifo[NDS_SC_FIFO_SIZE];
    MS_U16                          Rxfifo_r;
    MS_U16                          Rxfifo_w;
    MS_U16                          Txfifo_r;
    MS_U16                          Txfifo_w;

    // system specified
    MS_S32                          s32DevEventId;
    MS_S32                          s32TaskId;
    MS_S32                          s32Mutex;
    MS_S32                          s32MutexIdRx;

    NDS_SC_Clk                      clk;
    NDS_SC_Param                    param;

} NDS_SC_Dev;


typedef struct _NDS_SC_Rx
{
    // system specified
    MS_BOOL                         bUse;
    MS_S32                          s32Len;
    MS_S32                          s32Num;
    MS_BOOL                         bNullFilter;
    MS_BOOL                         bFlowCtrl;
    MS_U8                           *pAddr;
    MS_U32                          u32Timeout;
} NDS_SC_Rx;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

#if 1 //[VERIFIER]
//TODO: remove it
extern MS_BOOL bEmmOverflow;
extern MS_U32 emm_overflow_time;
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32                       _nds_sc_stack[4096/4];

// Rx Request Queue
static NDS_SC_Rx                    _nds_sc_rxq[NDS_SC_RX_REQ_MAX];
static MS_U8                        _nds_sc_rxq_read        = 0;    // User read queue (HDI)
static MS_U8                        _nds_sc_rxq_cur         = 0;    // Current active queue (ISR)
static MS_U8                        _nds_sc_rxq_next        = 0;    // The next non-used queue for request (HDI)

static NDS_SC_Dev                   _nds_sc_dev = {
                                        .bOpened            = FALSE,
                                        .bCardRdy           = FALSE,
                                        .bAborting          = FALSE,
                                        .clk                = E_NDS_SC_CLK_27M_D6,
                                        .param.cbSetVcc     = NULL,
                                        .param.cbEvent      = NULL,
                                        .param.bCommDump    = FALSE,
                                    };

static MS_BOOL                      _nds_sc_cardin          = FALSE;
static MS_BOOL                      _nds_sc_last_cardin     = 2;    // other than TRUE / FALSE
static MS_U16                       _nds_sc_rx_cnt          = 0;    // for flow control
static MS_U8                        _nds_sc_reg_ctrl        = 0;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

void NDS_SC_DBG_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
{
    int                 i;
    //printf("(%d)>>", u16DataLen);
    for (i = 0; i < u16DataLen; i++)
    {
        if ((i % 16) == 0)
        {
            NDS_SC_DBG("\n    ");
        }
        NDS_SC_DBG(" %02X ", pu8Data[i]);
    }
    NDS_SC_DBG("\n");
}


void _NDS_SC_SET_CTRL(MS_U8 bits)
{
//try
    MS_U8 dbg;
    dbg = _nds_sc_reg_ctrl | bits;
    MS_U32 old_int = MsOS_DisableAllInterrupts();
//try

    NDS_SC_OS_DisableInterrupt();
    NDS_SC_LOCK();
    _nds_sc_reg_ctrl |= bits;
    CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
    NDS_SC_UNLOCK();
    NDS_SC_OS_EnableInterrupt();

//try
    old_int = MsOS_RestoreAllInterrupts(old_int);
    if (dbg != _nds_sc_reg_ctrl)
    {
        while (1);
    }
//try
}

void _NDS_SC_CLR_CTRL(MS_U8 bits)
{
//try
    MS_U8 dbg;
    dbg = _nds_sc_reg_ctrl & ~bits;
    MS_U32 old_int = MsOS_DisableAllInterrupts();
//try

    NDS_SC_OS_DisableInterrupt();
    NDS_SC_LOCK();
    _nds_sc_reg_ctrl &= ~(bits);
    CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
    NDS_SC_UNLOCK();
    NDS_SC_OS_EnableInterrupt();

//tt
    old_int = MsOS_RestoreAllInterrupts(old_int);
    if (dbg != _nds_sc_reg_ctrl)
    {
        while (1);
    }
//tt
}


static void _NDS_SC_ResetRxIdx(void)
{
    MS_U32              i = 0;
    for (i = 0; i < NDS_SC_RX_REQ_MAX; i++)
    {
        _nds_sc_rxq[i].bUse = FALSE;
        //_smart_rx[i].pAddr = NULL;
        _nds_sc_rxq[i].s32Num = 0;
        _nds_sc_rxq[i].u32Timeout = 0;
        _nds_sc_rxq[i].bFlowCtrl = 0;
        _nds_sc_rxq[i].bNullFilter = 0;
        _nds_sc_rxq[i].s32Len = 0;
    }
    _nds_sc_rxq_read     = 0;
    _nds_sc_rxq_cur      = 0;
    _nds_sc_rxq_next     = 0;
    _nds_sc_rx_cnt  = 0;
}


static void _NDS_SC_Isr(InterruptNum eIntNum)
{
    MS_U32              events = 0;
    MS_U16              u16RegRead = 0, u16RegCtrl = 0;
    MS_U8               u8Char;

//    NDS_SC_OS_DisableInterrupt(); // @FIXME: why?

    // Recieve all data in fifo
    u16RegRead = CAM_REG(REG_CAM_UART_INT);
    CAM_REG(REG_CAM_UART_INT) = (CAM_UART_INT_MASK_RESET | u16RegRead); // clear interrupt

    if (u16RegRead & CAM_UART_INT_CD)
    {
        _nds_sc_dev.bCardRdy = FALSE;

        // [NOTE] ICAM2 certification needs the Vcc to be OFF after removal.
        _nds_sc_reg_ctrl &= ~(CAM_UART_CTRL_VCC_ACTIVE);
        CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;

        u16RegRead &= ~(CAM_UART_INT_CD);
        MsOS_SetEvent(_nds_sc_dev.s32DevEventId, NDS_SC_EVENT_CARD);// Notify ISR event
    }

    // recv ALL bytes if avaiable
    if (u16RegRead & (CAM_UART_INT_RX | CAM_UART_INT_RX_OVERFLOW | CAM_UART_INT_PARITY_ERR_RX))
    {
        if (u16RegRead & CAM_UART_INT_RX)
        {
            if (u16RegRead & CAM_UART_INT_RX_OVERFLOW)
            {
                events |= NDS_SC_EVENT_RX_OVERFLOW;
            }
            else if (u16RegRead & CAM_UART_INT_PARITY_ERR_RX)
            {
                events |= NDS_SC_EVENT_RX_ERROR;
            }

            while (1) // read all Rx data received regardless of any software status, e.g. bCardIn
            {
                u16RegCtrl = CAM_REG(REG_CAM_UART_CTRL);
                if ((u16RegCtrl & CAM_UART_STAT_RX_RDY) /* && (_nds_sc_dev.bCardIn)*/)
                {
                    MS_U32  u32tmp;

                    u8Char = CAM_REG(REG_CAM_UART_DATA);
                    _nds_sc_dev.Rxfifo[_nds_sc_dev.Rxfifo_w] = u8Char;
                    u32tmp = _nds_sc_dev.Rxfifo_w + 1;
#if 0
                    if ( (NDS_SC_FIFO_SIZE == u32tmp) && (_nds_sc_dev.Rxfifo_r != 0))
                    {
                        // Not overflow but wrap
                        _nds_sc_dev.Rxfifo_w = 0;
                    }
                    else if ( u32tmp != _nds_sc_dev.Rxfifo_r )
                    {
                        // Not overflow
                        _nds_sc_dev.Rxfifo_w = u32tmp;
                    }
                    else
                    {
                        break; //overflow
                        //_NDS_ASSERT_();
                    }
#else
                    if (u32tmp >= NDS_SC_FIFO_SIZE)
                    {
                        u32tmp = 0;
                    }

                    if (u32tmp != _nds_sc_dev.Rxfifo_r)
                    {
                        // Not overflow
                        _nds_sc_dev.Rxfifo_w = u32tmp;
                    }
                    else
                    {
                        MAsm_CPU_SwDbgBp();
                        break; // overflow
                    }
#endif

                    _nds_sc_rx_cnt--; // _nds_sc_rx_cnt > 0

#if SC_FLOWCTRL_ENABLE
                    // Do flow control by current rxq
                    // _nds_sc_rx_cur could be 2 over than _nds_sc_rx_read
                    if ( (_nds_sc_rx_cnt == 0) && (_nds_sc_rxq_cur != _nds_sc_rxq_next) )
                    {
                        // switch to next rxq
                        _nds_sc_rxq_cur++;
                        if (_nds_sc_rxq_cur >= (NDS_SC_RX_REQ_MAX-2))
                        {
                            _nds_sc_rxq_cur = 0;
                        }
                        if (_nds_sc_rxq_cur != _nds_sc_rxq_next)
                        {
                            _nds_sc_rx_cnt = _nds_sc_rxq[_nds_sc_rxq_cur].s32Len;
                        }
                    }

                    if (_nds_sc_rx_cnt != 0)
                    {
                        _nds_sc_reg_ctrl &= ~(CAM_UART_CTRL_FLOWCTRL); // open (high)
                        CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
                        if (_nds_sc_rxq[_nds_sc_rxq_cur].bFlowCtrl)
                        {
                            _nds_sc_reg_ctrl |= (CAM_UART_CTRL_FLOWCTRL); // close (low)
                            CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
                        }
                    }
                    else
                    {
                        // do nothing, flow control is be defined by the last byte
                    }

#else
                    if ((_nds_sc_rx_cnt != 0) && (_nds_sc_rxq[_nds_sc_rxq_read].bFlowCtrl))
                    {
                        _nds_sc_reg_ctrl &= ~(CAM_UART_CTRL_FLOWCTRL); // open (high)
                        CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
                        //__asm__ __volatile__("nop;");
                        _nds_sc_reg_ctrl |= (CAM_UART_CTRL_FLOWCTRL); // close (low)
                        CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
                    }
                    else
                    {
                        _nds_sc_rx_cnt = 0;
                        break;
                    }
#endif
                }
                else
                {
                    break;
                    //_NDS_ASSERT_();
                }
            }
            events |= NDS_SC_EVENT_RX;
        }

        MsOS_SetEvent(_nds_sc_dev.s32DevEventId, events);
        u16RegRead &= ~(CAM_UART_INT_RX | CAM_UART_INT_RX_OVERFLOW | CAM_UART_INT_PARITY_ERR_RX);
    }

    // send ONE byte if avaiable
    if (u16RegRead & (CAM_UART_INT_TX | CAM_UART_INT_PARITY_ERR_TX))
    {
        if (u16RegRead & CAM_UART_INT_PARITY_ERR_TX)
        {
            NDS_ASSERT(FALSE, , "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
        }
        else
        {
            u16RegCtrl = CAM_REG(REG_CAM_UART_CTRL);
            if (u16RegCtrl & CAM_UART_STAT_TX_RDY)
            {
                if(_nds_sc_dev.Txfifo_r != _nds_sc_dev.Txfifo_w)
                {
                    CAM_REG(REG_CAM_UART_DATA) = _nds_sc_dev.Txfifo[_nds_sc_dev.Txfifo_r];
                    _nds_sc_dev.Txfifo[_nds_sc_dev.Txfifo_r] = 0xff;
                    _nds_sc_dev.Txfifo_r++;
                    if (NDS_SC_FIFO_SIZE == _nds_sc_dev.Txfifo_r)
                    {
                        _nds_sc_dev.Txfifo_r = 0;
                    }
                }
            }
        }
        u16RegRead &= ~(CAM_UART_INT_TX | CAM_UART_INT_PARITY_ERR_TX);
    }

    // for debugging
    if (u16RegRead)
    {
        NDS_ASSERT(FALSE, , "NDS_ASSERT [%s]-[%d]\n", __FUNCTION__, __LINE__);
    }

    // IRQ handled
    NDS_SC_OS_EnableInterrupt();
}


static void _NDS_SC_IsrTask(void)
{
    MS_U8               u8Reg;
    MS_U32              u32Events, u32Timer;

    NDS_DBG("%s\n", __FUNCTION__);

    while(1)
    {
        u32Events = 0;

#if SC_RX_TIMEOUT_ENABLE
        MsOS_WaitEvent(_nds_sc_dev.s32DevEventId, 0xFFFF, &u32Events, E_OR_CLEAR, NDS_SC_CHK_TIMER);
#else
        MsOS_WaitEvent(_nds_sc_dev.s32DevEventId, 0xFFFF, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#endif

        if (u32Events & NDS_SC_EVENT_CARD)
        {
            MsOS_DelayTask(10); // skip glitch

            // event notification
            u8Reg = CAM_REG(REG_CAM_UART_CTRL);
            if (u8Reg & CAM_UART_STAT_DETECT)
            {
                _nds_sc_cardin = TRUE;
            }
            else
            {
                _nds_sc_cardin = FALSE;
            }
            if (_nds_sc_cardin)
            {
                if (_nds_sc_last_cardin == _nds_sc_cardin)
                {   //[NOTE] Since reset is control by upper level, the REMOVE event can never be LOST!!
                    // -- Jerry
//printf("ISR_SC_REMOVED_2\n");
                    CORECA_SmartCardComm(SC_REMOVED);
                    if (_nds_sc_dev.param.cbEvent)
                    {
                        _nds_sc_dev.param.cbEvent(E_NDS_SC_EVENT_CARD_REMOVED);
                    }
//                    MsOS_DelayTask(100);
                }
//printf("ISR_SC_REMOVED\n");
                CORECA_SmartCardComm(SC_INSERTED);
                if (_nds_sc_dev.param.cbEvent)
                {
                    _nds_sc_dev.param.cbEvent(E_NDS_SC_EVENT_CARD_INSERTED);
                }
            }
            else
            {
//printf("ISR_SC_REMOVED_1\n");
                if (_nds_sc_last_cardin != _nds_sc_cardin)
                {
                    CORECA_SmartCardComm(SC_REMOVED);
                    if (_nds_sc_dev.param.cbEvent)
                    {
                        _nds_sc_dev.param.cbEvent(E_NDS_SC_EVENT_CARD_REMOVED);
                    }
                }
            }
            _nds_sc_last_cardin = _nds_sc_cardin;

        } // NDS_SC_EVENT_CARD

        if (u32Events & NDS_SC_EVENT_RX)
        {
            if ( (u32Events == (NDS_SC_EVENT_RX | NDS_SC_EVENT_RX_OVERFLOW))
                 && (_nds_sc_dev.bAborting != TRUE) )
            {
                printf("SC_COMM_OVERFLOW[%d]\n", __LINE__);
                CORECA_SmartCardComm(SC_COMM_OVERFLOW);
            }
            if ( (u32Events == (NDS_SC_EVENT_RX | NDS_SC_EVENT_RX_ERROR))
                 && (_nds_sc_dev.bAborting != TRUE) )
            {
                printf("SC_PARITY[%d]\n", __LINE__);
                CORECA_SmartCardComm(SC_PARITY);
            }

            if ( (_nds_sc_rxq_read != _nds_sc_rxq_next) && (_nds_sc_dev.bCardRdy) ) // incompleted Rx queue
            {
                while(1)
                {
                    if (_nds_sc_dev.Rxfifo_w != _nds_sc_dev.Rxfifo_r) // data in fifo
                    {
                        u32Timer = 0;

                        if (_nds_sc_rxq[_nds_sc_rxq_read].bUse)
                        {
                            if (_nds_sc_dev.bCardRdy != TRUE)
                            {
                                break;
                            }

                            NDS_SC_RXQ_LOCK();
                            if ( (_nds_sc_rxq[_nds_sc_rxq_read].s32Len == 1)
                                 && (_nds_sc_rxq[_nds_sc_rxq_read].bNullFilter == TRUE)
                                 && (_nds_sc_dev.Rxfifo[_nds_sc_dev.Rxfifo_r] == 0x60) )
                            {
                                //NOTE:
                                // Will NULL only comes when the s32Len == 1?
                                // - jerry
                                _nds_sc_dev.Rxfifo_r++;

                                if (_nds_sc_dev.param.bCommDump)
                                {
                                    NDS_SC_DBG("NULL(0x60)\n");
                                }
                            }
                            else
                            {
                                if (_nds_sc_rxq[_nds_sc_rxq_read].s32Num > 0)
                                {
                                    // receive one byte from Rx FIFO
                                    *(MS_U8 *)_nds_sc_rxq[_nds_sc_rxq_read].pAddr = _nds_sc_dev.Rxfifo[_nds_sc_dev.Rxfifo_r++];
                                    _nds_sc_rxq[_nds_sc_rxq_read].s32Num--;
                                    _nds_sc_rxq[_nds_sc_rxq_read].pAddr++;
                                }

                                // check Rx queue completion
                                if (_nds_sc_rxq[_nds_sc_rxq_read].s32Num <= 0)
                                {
                                    // Dump communcation for NDS tests
                                    if (_nds_sc_dev.param.bCommDump)
                                    {
                                        NDS_SC_DBG("RX= ");
                                        NDS_SC_DBG_PrintData( (_nds_sc_rxq[_nds_sc_rxq_read].pAddr - _nds_sc_rxq[_nds_sc_rxq_read].s32Len),
                                                          _nds_sc_rxq[_nds_sc_rxq_read].s32Len);
                                    }

                                    // reset queue
                                    _nds_sc_rxq[_nds_sc_rxq_read].bUse = FALSE;
                                    _nds_sc_rxq[_nds_sc_rxq_read].s32Num = 0;
                                    // next queue
                                    _nds_sc_rxq_read++;
                                    if (_nds_sc_rxq_read >= (NDS_SC_RX_REQ_MAX-2))
                                    {
                                        _nds_sc_rxq_read = 0;
                                    }

                                    if (_nds_sc_dev.bAborting == FALSE)
                                    {
                                        if (_nds_sc_dev.bCardRdy == TRUE)
                                        {
//                                            printf("SC_COMM_OK\n");
                                            CORECA_SmartCardComm(SC_COMM_OK);
                                        }
                                    }
                                }
                            }
                            // wrap FIFO
                            if (_nds_sc_dev.Rxfifo_r >= NDS_SC_FIFO_SIZE)
                            {
                                _nds_sc_dev.Rxfifo_r = 0;
                            }
                            NDS_SC_RXQ_UNLOCK();

                        }
                        else // not bUsed
                        {
                            //_NDS_ASSERT_();
                        }
                    }
                    else
                    {
#if 1 //[VERIFIER] try '50' for verifier overnight instead of '1'
//TODO: more overnight & smartcard single test for verification
                        NDS_DELAY(1);
#endif
                    }

                    if ( (!_nds_sc_dev.bCardRdy) || (!_nds_sc_rxq[_nds_sc_rxq_read].bUse) )
                    {
                        break;
                    }
                }
            }
        }

#if SC_RX_TIMEOUT_ENABLE // WaitEvent should have timer to support timeout
        //---- Time Out ------
        // @TODO reset the new u32Timeout for next queued read request
        if (_nds_sc_rxq[_nds_sc_rxq_read].bUse == TRUE)
        {
            if (u32Timer > _nds_sc_rxq[_nds_sc_rxq_read].u32Timeout)
            {
                u32Timer = 0; // reset timer

                NDS_SC_RXQ_LOCK();

                // Dump communcation for NDS tests
                if (_nds_sc_dev.param.bCommDump)
                {
                    NDS_SC_DBG("RX= ");
                    NDS_SC_DBG_PrintData( (_nds_sc_rxq[_nds_sc_rxq_read].pAddr - (_nds_sc_rxq[_nds_sc_rxq_read].s32Len-_nds_sc_rxq[_nds_sc_rxq_read].s32Num)),
                                       (_nds_sc_rxq[_nds_sc_rxq_read].s32Len-_nds_sc_rxq[_nds_sc_rxq_read].s32Num));
                }

                // reset queue
                _nds_sc_rxq[_nds_sc_rxq_read].bUse = FALSE;
                _nds_sc_rxq[_nds_sc_rxq_read].s32Num = 0;
                // next queue
                _nds_sc_rxq_read++;
                if (_nds_sc_rxq_read >= (NDS_SC_RX_REQ_MAX-2))
                {
                    _nds_sc_rxq_read = 0;
                }
                NDS_SC_RXQ_UNLOCK();

                if ( (_nds_sc_dev.bAborting == FALSE)
                     && (_nds_sc_dev.bCardIn)
                     && (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_DETECT))
                {
                    printf("SC_ERROR line %d\n", __LINE__);
                    CORECA_SmartCardComm(SC_ERROR); // time out
                }

#if 0 // Only send one SC_ERROR and clear all the rest. But it may also resolved by AbortCommSession
                NDS_SC_RXQ_LOCK();
                while (_nds_sc_rxq[_nds_sc_rxq_read].bUse)
                {
                    // reset queue
                    _nds_sc_rxq[_nds_sc_rxq_read].bUse = FALSE;
                    _nds_sc_rxq[_nds_sc_rxq_read].s32Num = 0;
                    // next queue
                    _nds_sc_rxq_read++;
                    if (_nds_sc_rxq_read >= (NDS_SC_RX_REQ_MAX-2))
                    {
                        _nds_sc_rxq_read = 0;
                    }
                }
                NDS_SC_RXQ_UNLOCK();
#endif
            }
            u32Timer += NDS_SC_CHK_TIMER;
        }
#endif
    }
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void _NDS_SC_ResetFifo(void)
{
    //printf("--- SmartNDS_ResetFifo ---\n");
    NDS_SC_OS_DisableInterrupt();

    // @TODO: is it possible to lost card detection interrupt becuase clear it?
    CAM_REG(REG_CAM_UART_INT) = (CAM_UART_INT_MASK_RESET | CAM_UART_INT_ALL);

    NDS_SC_RXQ_LOCK();
    _nds_sc_dev.Rxfifo_r = 0;
    _nds_sc_dev.Rxfifo_w = 0;
    _NDS_SC_ResetRxIdx();
    NDS_SC_RXQ_UNLOCK();

    _nds_sc_dev.Txfifo_r = 0;
    _nds_sc_dev.Txfifo_w = 0;
    NDS_SC_OS_EnableInterrupt();
}


int _NDS_SC_Write(MS_U8 *buf, MS_U32 len, MS_BOOL nfc)
{
    int                 i, retry;

    NDS_SC_ENTRY();

    if ( (len <= 0) || (_nds_sc_dev.bCardRdy != TRUE) )
    {
        NDS_SC_RETURN(0);
    }

    NDS_SC_OS_DisableInterrupt();
    for (i = 1; i < len; i++)
    {
        if (_nds_sc_dev.bCardRdy != TRUE)
        {
            break;
        }
        _nds_sc_dev.Txfifo[_nds_sc_dev.Txfifo_w++] = buf[i];
        if (_nds_sc_dev.Txfifo_w >= NDS_SC_FIFO_SIZE)
        {
            _nds_sc_dev.Txfifo_w = 0;
        }

        if (i >= len)
        {
            break;
        }
    }
    NDS_SC_OS_EnableInterrupt();

    // Flow Control
    retry = 0;
    while (!(CAM_REG(REG_CAM_UART_CTRL)&CAM_UART_STAT_TX_RDY))
    {
        if ( ((retry++) > 10) || (_nds_sc_dev.bCardRdy != TRUE) )
        {
            NDS_SC_OS_DisableInterrupt();
            _nds_sc_dev.Txfifo_w = _nds_sc_dev.Txfifo_r;
            NDS_SC_OS_EnableInterrupt();

            NDS_SC_RETURN(0);
        }
        NDS_DELAY(1);
    }
    //printf("MStar. SC TX ing..\n");
    CAM_REG(REG_CAM_UART_DATA) = buf[0];

    NDS_SC_RETURN(i);

}


int _NDS_SC_Read(MS_U8 *buf, MS_U32 len, MS_BOOL bnf, MS_BOOL bfc, MS_U32 timeout)
{
    NDS_SC_ENTRY();

    if ( (len > 255) || (len == 0) || (_nds_sc_dev.bCardRdy != TRUE) )
    {
        NDS_SC_RETURN(0);
    }

    NDS_SC_RXQ_LOCK();
    if ( (_nds_sc_rxq[_nds_sc_rxq_next].bUse) || (buf == NULL) )
    {
        NDS_SC_RXQ_UNLOCK();
        //_NDS_ASSERT_();
        //GEN_EXCEP;
        NDS_SC_RETURN(0);
    }

    _nds_sc_rxq[_nds_sc_rxq_next].pAddr = buf;
    _nds_sc_rxq[_nds_sc_rxq_next].s32Num = len;
    _nds_sc_rxq[_nds_sc_rxq_next].s32Len = len;
    _nds_sc_rxq[_nds_sc_rxq_next].u32Timeout = timeout;
    _nds_sc_rxq[_nds_sc_rxq_next].bNullFilter = bnf;
    _nds_sc_rxq[_nds_sc_rxq_next].bFlowCtrl = bfc;

    // Flow control
#if SC_FLOWCTRL_ENABLE

    _nds_sc_rxq[_nds_sc_rxq_next].bUse = TRUE;

    NDS_SC_OS_DisableInterrupt();
    _nds_sc_rxq_next++;
    if (_nds_sc_rxq_next >= (NDS_SC_RX_REQ_MAX-2))
    {
        _nds_sc_rxq_next = 0;
    }
    NDS_SC_OS_EnableInterrupt();

    // @TODO: check read request queue overflow
    {
        //NDS_SC_OS_DisableInterrupt();
        // read is protected by RXQ_LOCK, but cur is touched at ISR
        if (_nds_sc_rxq_next == _nds_sc_rxq_read)
        {
            NDS_ERR("[%s]-[%d] -- sc_rxq vverflow\n", __FUNCTION__, __LINE__);
            while(1);
        }
        //NDS_SC_OS_EnableInterrupt();
    }

    if (_nds_sc_rx_cnt == 0) // cur == next
    {
//printf("DEBUG:rx_cnt==0\n");
        //NOTE:
        // rx_cnt == 0 iff cur == next
        // there is no race condition between USER function and ISR
        // -- jerry
        _nds_sc_rx_cnt = len;

        NDS_SC_OS_DisableInterrupt();
        NDS_SC_LOCK();
        _nds_sc_reg_ctrl &= ~(CAM_UART_CTRL_FLOWCTRL); // open (high)
        CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
        if (bfc)
        {
            _nds_sc_reg_ctrl |= (CAM_UART_CTRL_FLOWCTRL); // close (low)
            CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;
        }
        NDS_SC_UNLOCK();
        NDS_SC_OS_EnableInterrupt();
    }

#else
    _nds_sc_rx_cnt = len;
    _nds_sc_rxq_next++;
    if (_nds_sc_rxq_next >= (NDS_SC_RX_REQ_MAX-2))
    {
        _nds_sc_rxq_next = 0;
    }
#endif
    NDS_SC_RXQ_UNLOCK();

    if (_nds_sc_dev.bCardRdy != TRUE)
    {
        NDS_SC_RETURN(0);
    }

    NDS_SC_RETURN(len);
}


NDS_Result NDS_SC_Open(void)
{
    if (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_DETECT)
    {
        _nds_sc_cardin = TRUE;
    }
    else
    {
        _nds_sc_cardin = FALSE;
    }

    // register interrupt handler//enable interrupt for Icam-2
    NDS_SC_OS_AttachInterrupt(_NDS_SC_Isr);
    NDS_SC_OS_EnableInterrupt();
    CAM_REG(REG_CAM_UART_INT) = CAM_UART_INT_ALL;//(CAM_UART_INT_TX|CAM_UART_INT_RX|CAM_UART_INT_CD|CAM_UART_INT_RX_OVERFLOW);//0x3F;

    //MsOS_SetEvent(_smart_dev.s32DevEventId, 0x0001);
    _nds_sc_dev.bOpened = TRUE;
    return E_NDS_OK;
}


NDS_Result NDS_SC_Close(void)
{
    NDS_SC_OS_DisableInterrupt();
    NDS_SC_OS_DetachInterrupt();

    // clear interrupt
    CAM_REG(REG_CAM_UART_INT) = (CAM_UART_INT_MASK_RESET | CAM_UART_INT_ALL);

    _nds_sc_dev.bOpened = FALSE;
    return E_NDS_OK;
}


NDS_Result NDS_SC_Init(NDS_SC_Param *param)
{
    NDS_FUNC("[%s]-[%d] cbSetVcc:0x%08lX, cbEvent:0x%08lX\n", __FUNCTION__, __LINE__, (MS_U32)param->cbSetVcc, (MS_U32)param->cbEvent);

    _nds_sc_reg_ctrl = 0;
    CAM_REG(REG_CAM_UART_CTRL) = _nds_sc_reg_ctrl;

    _nds_sc_dev.param.cbSetVcc  = param->cbSetVcc;
    _nds_sc_dev.param.cbEvent   = param->cbEvent;
    _nds_sc_dev.param.bCommDump = param->bCommDump;
    HAL_NDS_SC_Init(param);

    _NDS_SC_ResetFifo();
    //SmartNDS_ResetRxIdx();
    //NDS_SC_OS_DisableInterrupt();
    //NDS_SC_OS_DetachInterrupt();


    // Initialize inter-process variable --------------------------------------
    _nds_sc_dev.s32DevEventId = MsOS_CreateEventGroup("DevSmart_Event");
    if (_nds_sc_dev.s32DevEventId < 0)
    {
        MsOS_DeleteTask(_nds_sc_dev.s32TaskId);
        return E_NDS_FAIL;
    }

    _nds_sc_dev.s32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "NDS_SC_MutexRx", MSOS_PROCESS_SHARED);
    if (_nds_sc_dev.s32Mutex < 0)
    {
        MsOS_DeleteEventGroup(_nds_sc_dev.s32DevEventId);
        MsOS_DeleteTask(_nds_sc_dev.s32TaskId);
        return E_NDS_FAIL;
    }

    _nds_sc_dev.s32MutexIdRx = MsOS_CreateMutex(E_MSOS_FIFO, "NDS_SC_MutexRx", MSOS_PROCESS_SHARED);
    if (_nds_sc_dev.s32MutexIdRx < 0)
    {
        MsOS_DeleteMutex(_nds_sc_dev.s32Mutex);
        MsOS_DeleteEventGroup(_nds_sc_dev.s32DevEventId);
        MsOS_DeleteTask(_nds_sc_dev.s32TaskId);
        return E_NDS_FAIL;
    }

    _nds_sc_dev.s32TaskId = MsOS_CreateTask( (TaskEntry) _NDS_SC_IsrTask,
                                             NULL,
#if 1 //[VERIFIER]
//TODO: try to be lower-priority for system banlance and smartcard single test for verification
                                             (TaskPriority) (E_TASK_PRI_SYS+2),
#else
                                             (TaskPriority) (E_TASK_PRI_SYS),
#endif
                                             TRUE,
                                             _nds_sc_stack,
                                             4096,
                                             "NDS_SC_IsrTask");

    return E_NDS_OK;
}


NDS_Result NDS_SC_Exit(void)
{
    MsOS_DeleteTask(_nds_sc_dev.s32TaskId);
    MsOS_DeleteMutex(_nds_sc_dev.s32MutexIdRx);
    MsOS_DeleteMutex(_nds_sc_dev.s32Mutex);
    MsOS_DeleteEventGroup(_nds_sc_dev.s32DevEventId);

    HAL_NDS_SC_Exit();

    return E_NDS_FAIL;
}


// 6.3.3
NDS_STATUS HDICA_SetCardClockDivisor(NDS_BYTE clock_divisor)
{
    NDS_FUNC("[%s]-[%d] -- clock_divisor[%d]\n", __FUNCTION__, __LINE__, clock_divisor);

    // ND-T189
    switch (clock_divisor)
    {
    case HIGHEST_CLOCK : // 13.5 MHz
        _nds_sc_dev.clk = E_NDS_SC_CLK_27M_D2;
        break;
    case MEDIUM_CLOCK : // 6.75 MHz
        _nds_sc_dev.clk = E_NDS_SC_CLK_27M_D4;
        break;
    case LOWEST_CLOCK : // 4.5 MHz
        _nds_sc_dev.clk = E_NDS_SC_CLK_27M_D6;
        break;
    default:
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    if (!HAL_NDS_SC_SetClockDivisor(_nds_sc_dev.clk))
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    MsOS_DelayTask(10);

    return CA_OK;
}


// 6.3.4
NDS_STATUS HDICA_SetConvention (NDS_BYTE convention)
{
    NDS_FUNC("[%s]-[%d] -- convention[%d] \n", __FUNCTION__, __LINE__, convention);

    switch (convention)
    {
    case HDICA_CONVENTION_INVERSE:
        _NDS_SC_CLR_CTRL(CAM_UART_CTRL_CONV_DIRECT);
        break;
    case HDICA_CONVENTION_DIRECT:
        _NDS_SC_SET_CTRL(CAM_UART_CTRL_CONV_DIRECT);
        break;
    default:
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    return CA_OK;
}


// 6.3.5
NDS_STATUS HDICA_SetUartBaudRate (NDS_BYTE baud_rate)
{
    MS_U16              n;
    MS_U32              br, div;

    NDS_FUNC("[%s]-[%d] -- baud_rate[%d]\n", __FUNCTION__, __LINE__, baud_rate);

    // Real baudrate (BR) for ICAM2_clk = 50 MHz, SM_clk = 27MHz/6 = 4.5 MHz

    br = 580645; // Nominal BAUD_RATE (153600) for HIGH_CLOCK (13.5 MHz) : 13.5 * 153600 / 3.5712
    div = 1;
    switch (_nds_sc_dev.clk)
    {
    case E_NDS_SC_CLK_27M_D2:
        // do nothing
        break;
    case E_NDS_SC_CLK_27M_D4:
        div = 2;
        break;
    case E_NDS_SC_CLK_27M_D6:
    default:
        div = 3;
        break;
    }

    switch (baud_rate)
    {
    case HDICA_BAUD_RATE_9600:
        div = div << 1;
    case HDICA_BAUD_RATE_19200:
        div = div << 1;
    case HDICA_BAUD_RATE_38400:
        div = div << 1;
    case HDICA_BAUD_RATE_76800:
        div = div << 1;
    case HDICA_BAUD_RATE_153600:
        // do nothing
        break;
    case HDICA_BAUD_RATE_223200:
    default:
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    n = NDS_CAM_CLK / br * div; // clk / (br / div)
/*
    switch(_nds_sc_dev.clk)
    {
    case E_NDS_SC_CLK_27M_D2:
        clkdiv = 72580; // 72580.5
        break;
    case E_NDS_SC_CLK_27M_D4:
        clkdiv = 36290; // 36290.25
        break;
    case E_NDS_SC_CLK_27M_D6:
    default:
        clkdiv = 24193; // 24193.5 (BR) for
        break;
    }

    // N = MCLK / BReal
    switch (baud_rate)
    {
    case HDICA_BAUD_RATE_153600:
        //div = (div>>1) + (div&0x1);
        n = (clk/193548)-2 ;
        break;
    case HDICA_BAUD_RATE_76800:
        //div = (div>>1) + (div&0x1);
        n = (clk/96774)-2 ;
        break;
    case HDICA_BAUD_RATE_38400:
        //div = (div>>1) + (div&0x1);
        n = (clk/48387)-2 ;
        break;
    case HDICA_BAUD_RATE_19200:
        //div = (div>>1) + (div&0x1);
        n = (clk/clkdiv)-2 ;
        break;
    case HDICA_BAUD_RATE_9600:
        n = (clk/12096)-2 ;
        break;
    case HDICA_BAUD_RATE_223200:
    default:
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }
*/

    n = n - 2;
    CAM_REG(REG_CAM_UART_BDRT_L) = n & 0xFF;
    CAM_REG(REG_CAM_UART_BDRT_H) = n >> 8;

    return CA_OK;
}


// 6.3.6
NDS_STATUS HDICA_SetVccLevel(NDS_BYTE vcc_level)
{
    NDS_FUNC("[%s]-[%d] -- vcc_level[%d]\n", __FUNCTION__, __LINE__, vcc_level);

    if (!_nds_sc_dev.param.cbSetVcc)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    switch (vcc_level)
    {
    case HDICA_VCC_3V:
        if (FALSE == _nds_sc_dev.param.cbSetVcc(FALSE))
        {
            return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        }
        break;
    case HDICA_VCC_5V:
        if (FALSE == _nds_sc_dev.param.cbSetVcc(TRUE))
        {
            return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
        }
        break;
    }

    return CA_OK;

    /*
    switch (vcc_level)
    {
    case HDICA_VCC_3V:
        *((volatile unsigned int*)0xBF203CC0) = 0x0B00;//TODO:: GPIO_OEN pad enable
        *((volatile unsigned int*)0xBF203CC0) |= 0xB000;//TODO:: GPIO
        break;
    case HDICA_VCC_5V:

        *((volatile unsigned int*)0xBF203CC0) = 0x4F00;//TODO:: GPIO_OEN pad enable
        *((volatile unsigned int*)0xBF203CC0) &= ~0xB000;//TODO:: GPIO
        break;
    default:
    }
    */

    return CA_OK;
}

// 6.3.7
NDS_STATUS HDICA_SetVcc (NDS_BYTE vcc_switch)
{
    NDS_FUNC("[%s]-[%d] -- vcc_switch[%d]\n", __FUNCTION__, __LINE__, vcc_switch);

    if (_nds_sc_dev.bOpened)
    {
        NDS_SC_RXQ_LOCK(); // for SetVcc, bCardIn
        {
            if (vcc_switch == HDICA_VCC_ON) //(HDICA_VCC_ON)
            {
//                NDS_DBG("HDICA_VCC_ON\n");

                //[NOTE]
                // When ACTIVE, only remove could be detected.
                // When DEACTIVE, both remove & insert could be detected.
                // If the card is removed and Vcc is ON, the DETECT will never work
                // -- Jerry
                _NDS_SC_SET_CTRL(CAM_UART_CTRL_VCC_ACTIVE);
                if ( !(CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_DETECT) )
                {
//                    NDS_DBG("HDICA_VCC_ON[FAIL]\n");
                    _NDS_SC_CLR_CTRL(CAM_UART_CTRL_VCC_ACTIVE);
                    NDS_SC_RXQ_UNLOCK(); // for SetVcc, bCardIn
                    return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
                }
#ifdef MSOS_TYPE_LINUX_KERNEL
                mdelay(NDS_SC_VCC_DELAY);
#else
                MsOS_DelayTask(NDS_SC_VCC_DELAY);
#endif

                _nds_sc_dev.bAborting = FALSE;
                _nds_sc_dev.bCardRdy = TRUE;
            }
            else
            {
//                NDS_DBG("HDICA_VCC_OFF\n");
                _NDS_SC_CLR_CTRL(CAM_UART_CTRL_VCC_ACTIVE);

#ifdef MSOS_TYPE_LINUX_KERNEL
                mdelay(10);
#else
                MsOS_DelayTask(10);
#endif
            }
        }
        NDS_SC_RXQ_UNLOCK(); // for SetVcc, bCardIn

        _NDS_SC_ResetFifo();
    }

    return CA_OK;
}


// 6.3.8
NDS_STATUS HDICA_SetVpp (NDS_BYTE vpp_switch)
{
    NDS_FUNC("[%s][%d]\n", __FUNCTION__, __LINE__);

    if (vpp_switch == HDICA_VCC_ON)
    {
        _NDS_SC_SET_CTRL(CAM_UART_CTRL_VPP_ACTIVE);
    }
    else
    {
        _NDS_SC_CLR_CTRL(CAM_UART_CTRL_VPP_ACTIVE);
    }

    return CA_OK;
}


// 6.3.9
NDS_STATUS HDICA_SetGuardTime (NDS_BYTE guard_time)
{
    NDS_FUNC("[%s]-[%d] -- guard_time[%d]\n", __FUNCTION__, __LINE__, guard_time);

    CAM_REG(REG_CAM_UART_GUARD) = guard_time;

    return CA_OK;
}


// 6.3.10
NDS_STATUS HDICA_GetInputByte (NDS_BYTE *input_byte)
{
    NDS_FUNC("[%s]-[%d]\n", __FUNCTION__, __LINE__);

    return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
}


// 6.3.11
NDS_STATUS HDICA_PutOutputByte (NDS_BYTE output_byte)
{
    NDS_FUNC("[%s]-[%d]\n", __FUNCTION__, __LINE__);

    return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
}


MS_U32 sc_poll_time = 0;

// 6.3.12
NDS_STATUS HDICA_ResetCard (NDS_BYTE reset_switch)
{
    NDS_FUNC("[%s]-[%d] -- reset_switch[%d]\n", __FUNCTION__, __LINE__, reset_switch);

#if 0 //[VERIFIER]
    // Debug Only
    MS_U32 time = MsOS_GetSystemTime();
    printf("HDICA_ResetCard %ld poll:%ld overflow:%ld\n", time, time-sc_poll_time, time-emm_overflow_time);
#else
    printf("HDICA_ResetCard %ld\n", MsOS_GetSystemTime());
#endif

    switch (reset_switch)
    {
    case ACTIVE:
        _NDS_SC_SET_CTRL(CAM_UART_CTRL_RST);
        break;

    case INACTIVE:
        _NDS_SC_CLR_CTRL(CAM_UART_CTRL_RST);
        break;

    case RESET_CYCLE:
        MsOS_DelayTask(10); /* Delay for Vcc to rise */
        _NDS_SC_CLR_CTRL(CAM_UART_CTRL_RST);
        MsOS_DelayTask(5);
        _NDS_SC_SET_CTRL(CAM_UART_CTRL_RST);
        MsOS_DelayTask(5);
        //CAM_REG(REG_CAM_UART_CTRL) = 0;//_NDS_SC_Ctrl_Reg;
        //MsOS_DelayTask(2); /* should be 10 msec - Zvika */

        /* perform card reset cycle */
        /* enable RX interrupts (should already be enabled!) */
        //ICAM_RegisterWrite(SmartCardICAM, ICAM_UART_INT, IC_UI_DETECT_INT + IC_UI_RX_INT + IC_UI_RX_OV_INT);
        CAM_REG(REG_CAM_UART_INT) = CAM_UART_INT_ALL; //enable (CAM_UART_INT_TX|CAM_UART_INT_RX|CAM_UART_INT_CD|CAM_UART_INT_RX_OVERFLOW);

        _NDS_SC_CLR_CTRL(CAM_UART_CTRL_RST);

        //printf("================>   RESET VAR:[%02X]\n", _nds_sc_reg_ctrl);
        break;

    default:
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    return CA_OK;
}


// 6.3.13
NDS_STATUS HDICA_GetUartStatus (NDS_BYTE *uart_status)
{
    *uart_status = 0;

    if (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_TX_RDY)
    {
        *uart_status |= HDICA_UART_STATUS_TX_IS_READY;                  /* Bit 0 */
    }

    if (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_RX_RDY)
    {
        *uart_status |= HDICA_UART_STATUS_RX_IS_READY;                  /* Bit 1 */
    }

    if (CAM_REG(REG_CAM_UART_INT) & CAM_UART_INT_PARITY_ERR_RX)
    {
        *uart_status |= HDICA_UART_STATUS_RCV_PARITY_ERROR;             /* Bit 2 */
    }

    if (CAM_REG(REG_CAM_UART_INT) & CAM_UART_INT_RX_OVERFLOW)
    {
        *uart_status |= HDICA_UART_STATUS_OVERFLOW;                     /* Bit 3 */
    }

    if (CAM_REG(REG_CAM_UART_INT) & CAM_UART_INT_PARITY_ERR_TX)
    {
        *uart_status |= HDICA_UART_STATUS_TX_PARITY_ERROR;              /* Bit 4 */
    }

    if (_nds_sc_reg_ctrl & CAM_UART_CTRL_CONV_DIRECT)
    {
        *uart_status |= HDICA_UART_STATUS_CONVENTION_DIRECT;            /* Bit 5 */
    }

    if (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_INT_PEND)
    {
        *uart_status |= HDICA_UART_STATUS_INTERRUPT_PENDING;            /* Bit 6 */
    }

    if (CAM_REG(REG_CAM_UART_CTRL) & CAM_UART_STAT_DETECT)
    {
        *uart_status |= HDICA_UART_STATUS_CARD_INSERTED;                /* Bit 7 */
    }

    NDS_FUNC("[%s]-[%d] -- status[0x%x]\n", __FUNCTION__, __LINE__, *uart_status);

    return CA_OK;
}


// 6.3.14
NDS_STATUS HDICA_SetUartCommand (NDS_BYTE pin_mask)
{
    NDS_FUNC("[%s]-[%d]\n", __FUNCTION__, __LINE__);

    CAM_REG(REG_CAM_UART_COM) = pin_mask;

    if (pin_mask & (CAM_UART_COM_C4 | CAM_UART_COM_C8))
    {
        HAL_NDS_SC_SetUartC48(TRUE);
    }
    else
    {
        HAL_NDS_SC_SetUartC48(FALSE);
    }

    return CA_OK;
}


// 6.3.15
NDS_STATUS HDICA_SendAndReceiveBytesSession (NDS_BYTE       *xmt_ptr,
                                             NDS_BYTE       xmit_length,
                                             CAHDI_BOOLEAN  null_filter,
                                             CAHDI_BOOLEAN  use_flow_control,
                                             NDS_BYTE       *rcv_ptr,
                                             NDS_BYTE       rcv_length)
{
    MS_U32  len;

    NDS_FUNC("[%s]-[%d] -- xmit[%d], rcv[%d]\n", __FUNCTION__, __LINE__, xmit_length, rcv_length);

    // check busy
    if (_nds_sc_dev.bAborting)
    {
        return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
    }

    // Dump communcation for NDS tests
    if (_nds_sc_dev.param.bCommDump)
    {
        NDS_SC_DBG("TX= ");
        NDS_SC_DBG_PrintData(xmt_ptr, xmit_length);
    }

#if 1 // debug
    if (xmt_ptr[0] == 0x48) // 0x40, 0x54, 0x5c
    {
        switch (xmt_ptr[1])
        {
        case 0x5C:
            //sc_poll_time = MsOS_GetSystemTime();
            printf("NDS_POLL(%02X)\n", xmt_ptr[1]);
            break;
        default:
            break;
        }
    }
#endif // debug

    // Rx
    len = _NDS_SC_Read(rcv_ptr, rcv_length, null_filter, use_flow_control, NDS_SC_RX_TIMEOUT);
    if (len != rcv_length)
    {
        return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
    }

    // Tx
    if ( (_NDS_SC_Write(xmt_ptr, xmit_length, use_flow_control) <= 0) ||
         (_nds_sc_dev.bAborting) )
    {
        return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
    }

#if 1 //[VERIFIER]
//TODO: check if delay necessary and smartcard single test
    MsOS_DelayTask(5); // yield for ISR_Task
#endif

    return CA_OK;
}


NDS_STATUS HDICA_ReceiveBytesSession (CAHDI_BOOLEAN null_filter,
                                      CAHDI_BOOLEAN use_flow_control,
                                      NDS_BYTE      *rcv_ptr,
                                      NDS_BYTE      rcv_length)
{
    MS_U32  len;

    NDS_FUNC("[%s]-[%d] rcv[%d]\n", __FUNCTION__, __LINE__, rcv_length);

    // check busy
    if (_nds_sc_dev.bAborting)
    {
        return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
    }

    // Flow control
#if SC_FLOWCTRL_ENABLE
#else
    _NDS_SC_CLR_CTRL(CAM_UART_CTRL_FLOWCTRL); // open (high)
    if (bfc)
    {
        _NDS_SC_SET_CTRL(CAM_UART_CTRL_FLOWCTRL); // close (low)
    }
#endif

    // Rx
    len = _NDS_SC_Read(rcv_ptr, rcv_length, null_filter, use_flow_control, NDS_SC_RX_TIMEOUT);
    if (len != rcv_length)
    {
        return CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW;
    }

#if 1 //[VERIFIER]
//TODO: check if delay necessary and smartcard single test
    MsOS_DelayTask(5); // yield for ISR_Task
#endif

    return CA_OK;
}


NDS_STATUS HDICA_AbortSmartCardCommSession (void)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    printf("HDICA_AbortSmartCardCommSession\n");

    _nds_sc_dev.bAborting = TRUE;
//tt
    // disable flow control
    _NDS_SC_CLR_CTRL(CAM_UART_CTRL_FLOWCTRL); // open (high)
//tt

    _NDS_SC_ResetFifo();

    _nds_sc_dev.bAborting = FALSE;

    return CA_OK;
}


