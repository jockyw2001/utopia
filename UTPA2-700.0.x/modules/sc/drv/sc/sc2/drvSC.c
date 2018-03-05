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
/// file    drvSC.c
/// @brief  SmartCard Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "string.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvSC.h"
#include "MsOS.h"
#ifdef SC_USE_IO_51
#include "drvMBX.h"
#endif
// Internal Definition
#include "sc_msos.h"
#include "regCHIP.h"
#include "regSC.h"
#include "halSC.h"
#include "utopia.h"

#define SC_KERNEL_ISR
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
#include "mdrv_sc_io.h"
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#endif

#include "ULog.h"

extern MS_VIRT _regSCHWBase;

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_SC "SC"

#define SMART_OCP_EN    0

#define SC_DBG_FUNC()             if (_gu8DbgLevel >= E_SC_DBGLV_ALL) \
                                        { ULOGD(TAG_SC, "\t====   %s   ====\n", __FUNCTION__); }
#define SC_DBG_INFO(x, args...)   if (_gu8DbgLevel >= E_SC_DBGLV_INFO ) \
                                        { ULOGI(TAG_SC, x, ##args); }
#define SC_DBG_ERR(x, args...)    if (_gu8DbgLevel >= E_SC_DBGLV_ERR_ONLY) \
                                        { ULOGE(TAG_SC, x, ##args); }

#define SC_WAIT_DELAY               20UL//20//50// TFCA //[TODO] tunable
#define SC_NULL_DELAY               10UL//500
#define SC_WRITE_TIMEOUT            500UL
#define SC_RST_HOLD_TIME            14UL//10                                  // Smart card reset should pull-low for 400 cycle
#define SC_RST_SWITCH_TIME          60UL//100
#define SC_CARD_STABLE_TIME         10UL//100

#define SC_T0_ATR_TIMEOUT           100UL
#define SC_T0_SEND_TIMEOUT          400UL//50

// add this  for TFCA some command will timeout
#define SC_T0_RECV_TIMEOUT          400UL//80
//add this for T14
#define SC_T14_RECV_TIMEOUT         400UL

#define SC_T0_RECV_RETRY_COUNT      (2000UL / SC_T0_RECV_TIMEOUT)//(2000 / SC_T0_RECV_TIMEOUT)
#define SC_T14_ATR_TIMEOUT          100UL
#define SC_T14_SEND_TIMEOUT         100UL

#define SC_T1_SEND_TIMEOUT          70UL// 1.5 //50
#define SC_T1_RECV_TIMEOUT          70UL// 1.5 //80

// local test debug message
#ifdef MS_DEBUG
#define SC_DBG(_d, _f, _a...)       { if (_dbgmsg >= _d) ULOGD(TAG_SC, _f, ##_a); }
#else
#define SC_DBG(_d, _f, _a...)       { }
#endif

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
#define SM0_DEV_NAME                "/dev/smart"
#define SM1_DEV_NAME                "/dev/smart1"
#endif

#define SC_INT_FAIL -1

#ifdef SC_USE_IO_51

#define SC_MBX_TIMEOUT              5000UL

#if defined(__mips__) || defined(__arm__)
#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))
#else
#define _PA2VA(x) x
#define _VA2PA(x) x
#endif

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32                          u32FifoDataBuf;
    MS_U16                          u16DataLen;
    MS_U32                          u8SCID;
    MS_U32                          u32TimeMs;
    SC_Result                       eResult;
} SC_SendData;

typedef struct
{
    MS_U8                           u8NAD;
    MS_U8                           u8NS;
    MS_U8                           u8NR;
    MS_BOOL                         ubMore;
    MS_U8                           u8IFSC;
    MS_U8                           u8RCType;
} SC_T1_State;

typedef enum
{
    SC_T1_R_OK,
    SC_T1_R_EDC_ERROR,
    SC_T1_R_OTHER_ERROR,
} SC_T1_RBlock_State;

typedef enum
{
    SC_T1_S_IFS,
    SC_T1_S_ABORT,
    SC_T1_S_WTX,
    SC_T1_S_RESYNCH,
    SC_T1_S_REQUEST,
    SC_T1_S_RESPONSE,
} SC_T1_SBlock_State;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version                 _drv_sc_version = {
                                        .DDI        = { SC_DRV_VERSION, },
                                    };

// 7816-3
static const MS_U16                 _atr_Fi[]       = { 372, 372, 558, 744,1116,1488,1860, 372, 372, 512, 768,1024,1536,2048, 372, 372 };
static const MS_U16                 _atr_Di[]       = {   1,   1,   2,   4,    8, 16,  32,   1,  12,  20,   1,   1,   1,   1,   1,   1 };

static const SC_Caps                _scCaps = {
                                        .u8DevNum   = SC_DEV_NUM,
                                    };

static SC_Status                    _scStatus = {
                                        .bCardIn = FALSE,
                                    };

//static MS_U8                        _gu8DbgLevel        = E_SC_DBGLV_ERR_ONLY;

MS_S8                               _dbgmsg = E_SC_DBGLV_ERR_ONLY; // E_SC_DBGLV_NONE, E_SC_DBGLV_ERR_ONLY, E_SC_DBGLV_REG_DUMP, E_SC_DBGLV_INFO, E_SC_DBGLV_ALL


static SC_T1_State                  _sc_T1State[SC_DEV_NUM] = {
                                        {
                                        .u8NAD      = 0,
                                        .u8NS       = 0,
                                        .u8NR       = 0,
                                        .ubMore     = FALSE,
                                        .u8IFSC     = 254,
                                        .u8RCType   = 0x00,
                                        }
                                    };

SC_Info                      _scInfo[SC_DEV_NUM] = {
                                        {
                                        .bInited    = 0,
                                        .bOpened    = 0,
                                        .eCardClk   = E_SC_CLK_4P5M,
                                        .eVccCtrl   = E_SC_VCC_CTRL_LOW,
                                        .u16ClkDiv  = 372,
                                        .bCardIn    = FALSE,
                                        .pfNotify   = NULL,
                                        .u16AtrLen  = 0,
                                        .u16HistLen = 0,
                                        .bLastCardIn = FALSE,
                                        .s32DevFd   = -1,
                                        },
#if (SC_DEV_NUM > 1) // no more than 2
                                        {
                                        .bInited    = 0,
                                        .bOpened    = 0,
                                        .eCardClk   = E_SC_CLK_4P5M,
                                        .eVccCtrl   = E_SC_VCC_CTRL_LOW,
                                        .u16ClkDiv  = 372,
                                        .bCardIn    = FALSE,
                                        .pfNotify   = NULL,
                                        .u16AtrLen  = 0,
                                        .u16HistLen = 0,
                                        .bLastCardIn = FALSE,
                                        .s32DevFd   = -1,
                                        }
#endif
                                    };


static MS_U8                        _pu8PPS[SC_PPS_LEN_MAX];
static MS_U16                       _u16PPSLen;

#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
static SC_SendData                  _scSendData;
#endif

#ifdef SC_USE_IO_51
static SC_AckFlags                  gScAckFlags;
static SC_DataCfg                  *gpScDataBuffPA = NULL;
static SC_DataCfg                  *gpScDataBuffVA = NULL;
static MS_PHY                     gScFwDataBuff = 0;
static MS_BOOL                      gbTimeout = FALSE;
#endif
static SC_Param  pParam;
static P_SC_Callback    pfNotify;
MS_BOOL     gbInitCheckDelay = FALSE;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Function prototype
//-------------------------------------------------------------------------------------------------
void _SC_OnChip8024ActiveSeq(void);
void _SC_OnChip8024DeactiveSeq(void);

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _SC_Callback_Notify(MS_U8 u8SCID, SC_Event eEvent)
{
    if(u8SCID >= SC_DEV_NUM)
        return;

    if (_scInfo[u8SCID].pfNotify)
    {
        _scInfo[u8SCID].pfNotify(u8SCID, eEvent);
    }
}



static void _SC_Callback_Enable5V(MS_U8 u8SCID, MS_BOOL bEnable)
{
    if (_scInfo[u8SCID].pfEn5V)
    {
        (_scInfo[u8SCID].pfEn5V)(bEnable);
    }
}


static MS_BOOL _SC_SetOCP(MS_U8 u8SCID, MS_BOOL bCard_In)
{
#if SMART_OCP_EN
    SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE));
    if (bCard_In == TRUE)
    {
        SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH);

        //EN_5V control
        if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5020) // BD_MST060C
        {
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP0);
            TOP_OR(REG_TOP_GPIO_TSI_OEN, TOP_SMART_CARD_CONTROL_ENABLE_OCP0);
            _SC_Callback_Enable5V(u8SCID, TRUE);
        }
        else if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5023) // BD_MST060B
        {
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP0);
            TOP_OR(REG_TOP_GPIO_TSI_OEN, TOP_SMART_CARD_CONTROL_ENABLE_OCP0);
            _SC_Callback_Enable5V(u8SCID, TRUE);
        }
        else if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5028) // BD_MST060A
        {
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP2);
            TOP_OR(REG_TOP_GPIO_TSI_OEN, TOP_SMART_CARD_CONTROL_ENABLE_OCP2);
            _SC_Callback_Enable5V(u8SCID, TRUE);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        //EN_5V control
        if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5020) // BD_MST060C
        {
            _SC_Callback_Enable5V(u8SCID, TRUE); //TODO why always TRUE???
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP0);
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP0));
        }
        else if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5023) // BD_MST060B
        {
            _SC_Callback_Enable5V(u8SCID, TRUE);
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP0);
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP0));
        }
        else if (_scInfo[u8SCID].u16Bonding == BONDING_MSD5028) // BD_MST060A
        {
            _SC_Callback_Enable5V(u8SCID, TRUE);
            TOP_OR(REG_TOP_DUMMY_15_16, TOP_PAD_CI_D4_OEN_OCP2);
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP2));
        }
        else
        {
            return FALSE;
        }
        SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_HIGH));
    }
#endif
    return TRUE;
}


static void _SC_ResetFIFO(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return;

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_RESET_FIFO))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_EXIT failed\n", __FUNCTION__, __LINE__);
    }
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    _scInfo[u8SCID].u16FifoRxRead = 0;
    _scInfo[u8SCID].u16FifoRxWrite = 0;
    _scInfo[u8SCID].u16FifoTxRead = 0;
    _scInfo[u8SCID].u16FifoTxWrite = 0;
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
}

#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
// Input single byte from Rx FIFO
static SC_Result _SC_ReadFIFO(MS_U8 u8SCID, MS_U8 *pu8Char)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    //MsOS_DelayTask(100);

    // Check fifo empty
    if (_scInfo[u8SCID].u16FifoRxWrite == _scInfo[u8SCID].u16FifoRxRead)
    {
        //ULOGE(TAG_SC, "_Smart_GetFifo -- DEVSC_EMPTY r[%d] w[%d]\n", _scInfo[u8SCID].u16FifoRxRead, _scInfo[u8SCID].u16FifoRxWrite);
        return E_SC_NODATA;
    }

    if (!_scInfo[u8SCID].bCardIn)
    {
        return E_SC_CARDOUT;
    }

    *pu8Char = _scInfo[u8SCID].u8FifoRx[_scInfo[u8SCID].u16FifoRxRead++];
    if (_scInfo[u8SCID].u16FifoRxRead == SC_FIFO_SIZE)
    {
        _scInfo[u8SCID].u16FifoRxRead = 0;
    }

    return E_SC_OK;
}
#endif //!(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))

// Input data byte stream
static MS_S16 _SC_Read(MS_U8 u8SCID, MS_U8 *buf, MS_U16 count)
{
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_S16 s16Size;

    if(u8SCID >= SC_DEV_NUM)
        return SC_INT_FAIL;

    if (0 > (s16Size = read(_scInfo[u8SCID].s32DevFd, (MS_VIRT *) buf, count)))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] read error\n", __FUNCTION__, __LINE__);
        return SC_INT_FAIL;
    }
    return s16Size;
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U16 i;
    MS_U8 ch;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if(u8SCID >= SC_DEV_NUM)
        return SC_INT_FAIL;

    if (count > 259)
    {
        return SC_INT_FAIL;
    }

    if (!_scInfo[u8SCID].bCardIn)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    card out 2\n");
        return SC_INT_FAIL;
    }

    for (i = 0; i < count; i++)
    {
        if (_SC_ReadFIFO(u8SCID, &ch) != E_SC_OK)
        {
            break;
        }
        *(MS_U8*)(buf+i) = ch;
    }

    return (i);
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
}


#ifndef SC_USE_IO_51


#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
static SC_Result _SC_WriteFIFO(MS_U8 u8SCID, MS_U8 u8Char)
{
  MS_U32 tmp;

    _scInfo[u8SCID].u8FifoTx[_scInfo[u8SCID].u16FifoTxWrite] = u8Char;
    tmp = _scInfo[u8SCID].u16FifoTxWrite + 1;
    if ((tmp == SC_FIFO_SIZE) && (_scInfo[u8SCID].u16FifoTxRead != 0))
    {
        // Not overflow but wrap
        _scInfo[u8SCID].u16FifoTxWrite = 0;
    }
    else if (tmp != _scInfo[u8SCID].u16FifoTxRead)
    {
        // Not overflow
        _scInfo[u8SCID].u16FifoTxWrite = tmp;
    }
    else
    {
        ULOGE(TAG_SC, "_Smart_PutChar Overflow\n");
        return E_SC_OVERFLOW;
    }
    return E_SC_OK;
}
#endif // !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))

static MS_BOOL _SC_WaitTXFree(MS_U8 u8SCID,MS_U32 u32TimeOutMs)
{
    MS_U32 curTime = OS_SC_TIME();

    if (u8SCID >= SC_DEV_NUM)
        return FALSE;

    while ((SC_READ(u8SCID,UART_LSR) & (UART_LSR_THRE)) == 0)
    {
        if (OS_SC_TIME() - curTime > u32TimeOutMs)
            return FALSE;
        OS_SC_DELAY(1);
    }
    return TRUE;
}

#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
static MS_BOOL _SC_WaitTXFIFOEmpty(MS_U8 u8SCID, MS_U32 u32TimeOutMs)
{
    MS_U32 curTime = OS_SC_TIME();

    while (_scInfo[u8SCID].u16FifoTxWrite != _scInfo[u8SCID].u16FifoTxRead)
    {
        if (OS_SC_TIME() - curTime > u32TimeOutMs)
            return FALSE;
        OS_SC_DELAY(1);
    }
    return TRUE;
}
#endif // !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))

// Output data byte stream
static MS_S16 _SC_Write(MS_U8 u8SCID, MS_U8 *buf, MS_U16 count)
{
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_S32 s32Size;

    if(u8SCID >= SC_DEV_NUM)
        return 0;

    if (0 > (s32Size = write(_scInfo[u8SCID].s32DevFd, (MS_VIRT *) buf, count)))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] read error\n", __FUNCTION__, __LINE__);
    }
    return s32Size;
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U16              i;
    MS_U8               ch;
    MS_U32              curTime;

    if(u8SCID >= SC_DEV_NUM)
        return 0;

    //SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    //ASSERT(buf != NULL);

    if (count > 259)
    {
        return -1;
    }
    if (!_scInfo[u8SCID].bCardIn)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    card out 2\n");
        return -1;
    }

    for (i = 1; i < count; i++)
    {
        ch = buf[i];//*(MS_U8*)(buf+i);
        if (_SC_WriteFIFO(u8SCID, ch) != E_SC_OK)
        {
            return (i);
        }
    }
    curTime = OS_SC_TIME();
    while (!(SC_READ(u8SCID, UART_LSR) & UART_LSR_THRE))
    {
        if ((OS_SC_TIME() - curTime) > SC_WRITE_TIMEOUT)
        {
            return -1;
        }
        OS_SC_DELAY(1);
    }

    SC_WRITE(u8SCID,UART_TX, buf[0]);

    if(!_SC_WaitTXFIFOEmpty(u8SCID, SC_WRITE_TIMEOUT))
    {
        return -1;
    }

    return (i);
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
}

#endif //if SC_USE_IO_51 not defined


static SC_Result _SC_Reset(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (!_scInfo[u8SCID].bCardIn)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    card out 1\n");
        return E_SC_CARDOUT;
    }

    OS_SC_DELAY(SC_RST_SWITCH_TIME);

    if (!_scInfo[u8SCID].bCardIn)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    card out 2\n");
        return E_SC_CARDOUT;
    }

    // Disable all interrupts
    SC_WRITE(u8SCID, UART_IER, 0x00);

    // Reset receiver and transmiter                                // FIFO Control Register
    SC_WRITE(u8SCID, UART_FCR, UART_FCR_ENABLE_FIFO | UART_FCR_TRIGGER_1);

    // Disable all interrupts (Dummy write action. To make Rx FIFO reset really comes into effect)
    SC_WRITE(u8SCID, UART_IER, 0x00);

    // Check Rx FIFO empty
    while ((SC_READ(u8SCID,UART_LSR) & UART_LSR_DR))
    {
        SC_READ(u8SCID,UART_RX);
        OS_SC_DELAY(1);
    }

#ifndef SC_USE_IO_51
    // enable interrupt
    SC_WRITE(u8SCID, UART_IER, (UART_IER_RDI | UART_IER_THRI));
#endif
    // Clear Rx buffer
    _SC_ResetFIFO(u8SCID);

    // Smart card reset
    // need to disable reset for 5v mode when on chip 8024 mode
    // disable these UART_SCCR_RST flow when on chip 8024 mode, move these code to active sequence code when on chip 8024 mode
    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
        SC_WRITE(0, UART_DEACTIVE_RST, 0x0);
        SC_WRITE(0, UART_DEACTIVE_CLK, 0xff);
        SC_WRITE(0, UART_DEACTIVE_IO, 0xff);
        SC_WRITE(0, UART_DEACTIVE_VCC, 0xff);
        SC_WRITE(0, UART_CTRL3, SC_READ(0, UART_CTRL3)|0x1); // ori is 1
        MsOS_DelayTask(1);
        SC_WRITE(0, UART_CTRL3, SC_READ(0, UART_CTRL3)&~0x1); // ori is 1
        SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)|0x20); // all interface release, let RST pin to high
        SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)&0xDF); // all interface release, let RST pin to high
        //ULOGD(TAG_SC, "warm reset by deactive seq\n");
    }
    else
    {
        SC_AND(u8SCID,UART_SCCR, ~UART_SCCR_RST);//active low
        OS_SC_DELAY(SC_RST_HOLD_TIME);
        SC_OR(u8SCID,UART_SCCR, UART_SCCR_RST);//active high
    }

    return E_SC_OK;
}


// Set uart divider
static SC_Result _SC_SetUartDiv(MS_U8 u8SCID)
{
    MS_U16              u16clk, u16div;
    MS_U32  clk = 144000000;

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    SC_WRITE(u8SCID,UART_MCR, 0);
    //SC_REG8(UART_MCR) = UART_MCR_FAST;

    u16clk = (MS_U16)_scInfo[u8SCID].eCardClk;
    switch (_scInfo[u8SCID].eCardClk)
    {
    case E_SC_CLK_3M:
        //u16clk = TOP_CKG_CAM_SRC_27M_D8;/* 3.375M */
        u16div = ((clk/3375000)*_scInfo[u8SCID].u16ClkDiv)/16;
        break;
    case E_SC_CLK_4P5M:
        //u16clk = TOP_CKG_CAM_SRC_27M_D6;/* 4.5M */
        u16div = ((clk/4500000)*_scInfo[u8SCID].u16ClkDiv)/16;//(108Mhz * 372) / (4.5Mhz * 16) <<--TOP_UART1_CKG_CAM_SRC_123M
        break;
    case E_SC_CLK_6M:
        //u16clk = TOP_CKG_CAM_SRC_27M_D4;/* 6.75M */
        u16div = ((clk/6750000)*_scInfo[u8SCID].u16ClkDiv)/16;
        break;
    case E_SC_CLK_13M:
        //u16clk = TOP_CKG_CAM_SRC_27M_D2;/* 13.5M */
        u16div = ((clk/16)*_scInfo[u8SCID].u16ClkDiv)/(3375000);
        // u16div = ((clk/16)*_scInfo[u8SCID].u16ClkDiv)/(13500000);
        break;
    case E_SC_CLK_4M:
        u16div = ((clk/4000000)*_scInfo[u8SCID].u16ClkDiv)/16;//(108Mhz * 372) / (4.5Mhz * 16) <<--TOP_UART1_CKG_CAM_SRC_123M
        break;
    default:
        return E_SC_FAIL;
    }

    // Switch to specified clk
    HAL_SC_SetClk(u8SCID,u16clk);

    // Set divider
    SC_OR(u8SCID,UART_LCR, UART_LCR_DLAB);                                     // Line Control Register
    SC_WRITE(u8SCID,UART_DLL, u16div & 0x00FF);                                // Divisor Latch Low
    SC_WRITE(u8SCID,UART_DLM, u16div >> 8);                                  // Divisor Latch High
    SC_AND(u8SCID,UART_LCR, ~(UART_LCR_DLAB));                              // Line Control Register

    return E_SC_OK;
}


//TODO SM2 support
static SC_Result _SC_SetUartMode(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    SC_WRITE(u8SCID,UART_LCR, (SC_READ(u8SCID,UART_LCR)&0xE0)|_scInfo[u8SCID].u8UartMode);
    return E_SC_OK;
}

static SC_Result _SC_SetConvention(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (_scInfo[u8SCID].u8Convention == 0)
    {
        // direct convention
        SC_WRITE(u8SCID,UART_SCCR, SC_READ(u8SCID,UART_SCCR)&~( UART_SCCR_RX_BSWAP | UART_SCCR_RX_BINV |
                                    UART_SCCR_TX_BSWAP | UART_SCCR_TX_BINV ));
        //SMART_REG8(UART_SCCR) &= ~( UART_SCCR_RX_BSWAP | UART_SCCR_RX_BINV |
        //                            UART_SCCR_TX_BSWAP | UART_SCCR_TX_BINV );
    }
    else
    {
        SC_WRITE(u8SCID,UART_SCCR, SC_READ(u8SCID,UART_SCCR) | (UART_SCCR_RX_BSWAP | UART_SCCR_RX_BINV |
                                 UART_SCCR_TX_BSWAP  | UART_SCCR_TX_BINV));
        // inverse convention
        // SMART_REG8(UART_SCCR) |= UART_SCCR_RX_BSWAP | UART_SCCR_RX_BINV |
        //                          UART_SCCR_TX_BSWAP | UART_SCCR_TX_BINV;
    }
    return E_SC_OK;
}


//TODO SM2 support
static SC_Result _SC_SetInvConv(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    SC_XOR(u8SCID,UART_SCCR, UART_SCCR_RX_BSWAP | UART_SCCR_RX_BINV | UART_SCCR_TX_BSWAP | UART_SCCR_TX_BINV);
    return E_SC_OK;
}

#ifdef SC_USE_IO_51
MBX_Result _MDrv_SC_MailBoxHandler(void *pData)
{
    MBX_Msg MB_Command;

    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;

    mbxResult = MDrv_MBX_RecvMsg(E_MBX_CLASS_PM_WAIT, &MB_Command, SC_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (E_MBX_ERR_TIME_OUT == mbxResult)
    {
        return mbxResult;
    }

    if (E_MBX_SUCCESS == mbxResult)
    {
        if ((MB_Command.u8Ctrl == 0) || (MB_Command.u8Ctrl == 1))
        {
            SC_DBG(E_SC_DBGLV_INFO, "Get SC command: 0x%02x.\n", MB_Command.u8Index);
            switch (MB_Command.u8Index)
            {
                case SC_CMDIDX_ACK_51ToMIPS:
                    switch(MB_Command.u8Parameters[0])
                    {
                        case SC_CMDIDX_RAW_EXCHANGE:
                            *((SC_Result*)pData) = (SC_Result)MB_Command.u8Parameters[1];
                            gScAckFlags &= (SC_AckFlags)(~E_SC_ACKFLG_WAIT_RAW_EXCHANGE);
                            break;
                        case SC_CMDIDX_GET_ATR:
                            *((SC_Result*)pData) = (SC_Result)MB_Command.u8Parameters[1];
                            gScAckFlags &= (SC_AckFlags)(~E_SC_ACKFLG_WAIT_GET_ATR);
                            break;
                        case SC_CMDIDX_SEND:
                            *((SC_Result*)pData) = (SC_Result)MB_Command.u8Parameters[1];
                            gScAckFlags &= (SC_AckFlags)(~E_SC_ACKFLG_WAIT_SEND);
                            break;
                        case SC_CMDIDX_RECV:
                            *((SC_Result*)pData) = (SC_Result)MB_Command.u8Parameters[1];
                            gScAckFlags &= (SC_AckFlags)(~E_SC_ACKFLG_WAIT_RECV);
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    return mbxResult;
}

static SC_Result _SC_Send(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 u16DataLen, MS_U32 u32TimeoutMs)
{
#if 1
    SC_Result ScResult = E_SC_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT u32SrcAddr;
    MS_U32 MBX_Resendcnt = 0;

    u32TimeoutMs = u32TimeoutMs;

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        ULOGE(TAG_SC, "Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
        return E_SC_FAIL;
    }

    gpScDataBuffVA->u8SCID = u8SCID;
    gpScDataBuffVA->u16DataLen = u16DataLen;
    gpScDataBuffVA->u16ReplyMaxLen = 0;
    memcpy(gpScDataBuffVA->u8Data, pu8Data, u16DataLen);

    MsOS_FlushMemory();
    MsOS_Sync();

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }


    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gScAckFlags |= E_SC_ACKFLG_WAIT_SEND;

    u32SrcAddr = (MS_VIRT) gpScDataBuffPA;
    // send to 8051
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = SC_CMDIDX_SEND;
        MB_Command.u8ParameterCount = 4;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for 8051 Getting ATR
    do
    {
        if(MBX_Resendcnt<10)
        {
            mbxResult = _MDrv_SC_MailBoxHandler(&ScResult);
            MBX_Resendcnt++;
        }
        else
        {
            mbxResult = E_MBX_ERR_TIME_OUT;
            ScResult = E_SC_TIMEOUT;
            //TODO: return data length = 0?
            break;
        }
    }
    while((gScAckFlags & E_SC_ACKFLG_WAIT_SEND) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

#if 0 // debug
    MS_S32 i;
    printf("Data Len = %d\n", gpScDataBuffVA->u16DataLen);
    for (i = 0; i < gpScDataBuffVA->u16DataLen; i++)
    {
        printf("%x,", gpScDataBuffVA->u8Data[i]);
    }
    printf("\n");
#endif

    if( E_MBX_SUCCESS != mbxResult)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Send Fail\n");
    }
    else
    {
        if (gpScDataBuffVA->u16DataLen == 0)
        {
            ScResult = E_SC_NODATA;
        }
        else
        {
            SC_DBG(E_SC_DBGLV_INFO, "_SC_Send OK\n");
            ScResult = E_SC_OK;
        }
    }

    return ScResult;

#endif
}

static SC_Result _SC_Recv(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 *u16DataLen, MS_U32 u32TimeoutMs)
{
#if 1
    SC_Result ScResult = E_SC_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT u32SrcAddr;

    MS_U32 MBX_Resendcnt = 0;

    u32TimeoutMs = u32TimeoutMs;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        ULOGE(TAG_SC, "Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
        return E_SC_FAIL;
    }

    gpScDataBuffVA->u8SCID = u8SCID;
    gpScDataBuffVA->u16DataLen = *u16DataLen;
    gpScDataBuffVA->u16ReplyMaxLen = *u16DataLen;

    MsOS_FlushMemory();
    MsOS_Sync();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gScAckFlags |= E_SC_ACKFLG_WAIT_RECV;

    u32SrcAddr = (MS_VIRT) gpScDataBuffPA;
    // send to 8051
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = SC_CMDIDX_RECV;
        MB_Command.u8ParameterCount = 8;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);
        if (gbTimeout)
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(u32TimeoutMs>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(u32TimeoutMs>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(u32TimeoutMs>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(u32TimeoutMs&0x000000FF);
        }
        else
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(0>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(0>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(0>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(0&0x000000FF);
        }

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for 8051 Raw Data Exchange
    do
    {
        if(MBX_Resendcnt<10)
        {
            mbxResult = _MDrv_SC_MailBoxHandler(&ScResult);
            MBX_Resendcnt++;
        }
        else
        {
            mbxResult = E_MBX_ERR_TIME_OUT;
            ScResult = E_SC_TIMEOUT;
            //TODO: return data length = 0?
            break;
        }
    }
    while((gScAckFlags & E_SC_ACKFLG_WAIT_RECV) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if( E_MBX_SUCCESS != mbxResult)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_SC_RawExchange Fail\n");
    }
    else
    {
        SC_DBG(E_SC_DBGLV_INFO, "MDrv_SC_RawExchange OK\n");
    }

    //Get returned data
    memcpy(pu8Data, gpScDataBuffVA->u8Data, gpScDataBuffVA->u16DataLen);
    *u16DataLen = gpScDataBuffVA->u16DataLen;

    return ScResult;

#endif
}

SC_Result _SC_RawExchange(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_Result ScResult = E_SC_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_VIRT u32SrcAddr;

    MS_U32 MBX_Resendcnt = 0;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        ULOGE(TAG_SC, "Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
        return E_SC_FAIL;
    }

    if (*u16SendDataLen > 259)
    {
        return E_SC_PARMERROR;
    }

    gpScDataBuffVA->u8SCID = u8SCID;
    gpScDataBuffVA->u16DataLen = *u16SendDataLen;
    gpScDataBuffVA->u16ReplyMaxLen = *u16ReadDataLen;
    memcpy(gpScDataBuffVA->u8Data, pu8SendData, *u16SendDataLen);

    MsOS_FlushMemory();
    MsOS_Sync();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gScAckFlags |= E_SC_ACKFLG_WAIT_RAW_EXCHANGE;

    u32SrcAddr = (MS_VIRT) gpScDataBuffPA;
    //printf("== MDrv_SC_RawExchange ==\n");
    // send to 8051
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = SC_CMDIDX_RAW_EXCHANGE;
        MB_Command.u8ParameterCount = 8;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);
        if (gbTimeout)
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(u32TimeoutMs>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(u32TimeoutMs>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(u32TimeoutMs>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(u32TimeoutMs&0x000000FF);
        }
        else
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(0>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(0>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(0>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(0&0x000000FF);
        }

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for 8051 Raw Data Exchange
    do
    {
        if (MBX_Resendcnt<10)
        {
            mbxResult = _MDrv_SC_MailBoxHandler(&ScResult);
            MBX_Resendcnt++;
        }
        else
        {
            mbxResult = E_MBX_ERR_TIME_OUT;
            ScResult = E_SC_TIMEOUT;
            //TODO: return data length = 0?
            break;
        }
    }
    while((gScAckFlags & E_SC_ACKFLG_WAIT_RAW_EXCHANGE) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

    if (E_MBX_SUCCESS != mbxResult)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_SC_RawExchange Fail\n");
    }
    else
    {
        SC_DBG(E_SC_DBGLV_INFO, "MDrv_SC_RawExchange OK\n");
    }

    //Get returned data
    memcpy(pu8ReadData, gpScDataBuffVA->u8Data, gpScDataBuffVA->u16DataLen);
    *u16ReadDataLen = gpScDataBuffVA->u16DataLen;

    return ScResult;
}

#else
static SC_Result _SC_Send(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 u16DataLen, MS_U32 u32TimeoutMs)
{
    MS_U16  i = 0;
    MS_S16  len = 0;
    MS_U32  u32StartTime=OS_SYSTEM_TIME();

    if (u8SCID >= SC_DEV_NUM)
        return E_SC_PARMERROR;

    len = _SC_Write(u8SCID, pu8Data, u16DataLen);

    if(len < 0)
    {
        return E_SC_FAIL;
    }

    while (1)
    {
        i = i + len;
        if (i == u16DataLen)
        {
            break;
        }
        //printf("\n\n SC Send again to delay \n\n");
        if ((len == 0) && (OS_SYSTEM_TIME() - u32StartTime) > u32TimeoutMs)
        {
            break;
        }

        OS_SC_DELAY(10);

        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        {
            SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
            return E_SC_CARDOUT;
        }

        len = _SC_Write(u8SCID, pu8Data+i, u16DataLen-i);
        if (len < 0)
        {
            return E_SC_FAIL;
        }
    }

    if(!_SC_WaitTXFree(u8SCID,SC_WRITE_TIMEOUT))
    {
        return E_SC_TIMEOUT;
    }

    return E_SC_OK;
}

static SC_Result _SC_Recv(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 *u16DataLen, MS_U32 u32TimeoutMs)
{
    MS_S16 i = 0;
    MS_S16 len = 0;

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    // first read
    //MS_U32 u32StartTime=OS_SYSTEM_TIME();
    len = _SC_Read(u8SCID, pu8Data, *u16DataLen);
    //u32TimeoutMs =8000;
    //printf("%s,%d:rev %d data:0x%x\n",__FUNCTION__,__LINE__,len,u16DataLen[0]);
    MS_U32  countreadtime = u32TimeoutMs/10;

    while (i < *u16DataLen)
    {
        i = i + len;
        if (i == *u16DataLen)
        {
            break;
        }
        if ((i < *u16DataLen) && (countreadtime == 0)/*(OS_SYSTEM_TIME()-u32StartTime)>500*/)
        {
            *u16DataLen = i;
            return E_SC_TIMEOUT;
        }
        countreadtime -- ;
        //printf("%s,%d:rev %d\n",__FUNCTION__,__LINE__,len);
        OS_SC_DELAY(10);
        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        // if (_scInfo[u8SCID].bCardIn == FALSE)
        {
            SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
            *u16DataLen = i;
            return E_SC_CARDOUT;
        }

        len = _SC_Read(u8SCID, pu8Data+i, (*u16DataLen)-i);
        if (len < 0)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Read get fail\n");
            *u16DataLen = i;
            return E_SC_FAIL;
        }
    }
    *u16DataLen = i;
    return E_SC_OK;
}

#endif //SC_USE_IO_51

//Copy from _SC_Recv for COVERITY CHECK
static SC_Result _SC_Recv_1_Byte(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 *u16DataLen, MS_U32 u32TimeoutMs)
{
    MS_S16 i = 0;
    MS_S16 len = 0;

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    len = _SC_Read(u8SCID, pu8Data, *u16DataLen);
    MS_U32 countreadtime = u32TimeoutMs/10;

    for (i = 0; i < *u16DataLen; )
    {
        i = i + len;
        if (i == *u16DataLen)
        {
            break;
        }
        if ((len==0)&& (countreadtime ==0))
        {
            *u16DataLen = i;
            return E_SC_TIMEOUT;
        }
        countreadtime -- ;
        OS_SC_DELAY(10);
        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        {
            SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
            *u16DataLen = i;
            return E_SC_CARDOUT;
        }

        len = _SC_Read(u8SCID, pu8Data, (*u16DataLen)-i);
        if (len < 0)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Read get fail\n");
            *u16DataLen = i;
            return E_SC_FAIL;
        }
    }
    *u16DataLen = i;
    return E_SC_OK;
}


#ifndef SC_USE_IO_51
// add this will reduce the time when the command is less than 7
//because the length of recv can be calculated by the seven  byte
static SC_Result _T14_SC_Recv(MS_U8 u8SCID, MS_U8 *pu8Data, MS_U16 *u16DataLen, MS_U32 u32TimeoutMs)
{
    MS_S16 i = 0;
    MS_S16 len = 0;

    // first read
    MS_U32 u32StartTime=OS_SYSTEM_TIME();
    MS_U16 u16Datelenbegin = 8;
    MS_S16 lenbegin = 0;

    if (u8SCID >= SC_DEV_NUM)
        return E_SC_PARMERROR;

    lenbegin = _SC_Read(u8SCID, pu8Data, u16Datelenbegin);

    for (i = 0; i < u16Datelenbegin;)
    {
        i = i + lenbegin;
        if ((lenbegin==0)&&(OS_SYSTEM_TIME()-u32StartTime)>u32TimeoutMs)
        {
            *u16DataLen = i;
            return E_SC_TIMEOUT;
        }
        OS_SC_DELAY(10);
        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        {
            SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
            *u16DataLen = i;
            return E_SC_CARDOUT;
        }
        lenbegin = _SC_Read(u8SCID,pu8Data+i, u16Datelenbegin-i);
        if (lenbegin < 0)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Read get fail\n");
            *u16DataLen = i;
            return E_SC_FAIL;
        }
    }

    {
        *u16DataLen = pu8Data[7]+1;
        for (i = 0; i < *u16DataLen; )
        {
            i = i + len;
            if ((len==0)&&(OS_SYSTEM_TIME()-u32StartTime)>u32TimeoutMs)
            {
                *u16DataLen = i;
                return E_SC_TIMEOUT;
            }
            OS_SC_DELAY(10);
            if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
            {
                SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
                *u16DataLen = i;
                return E_SC_CARDOUT;
            }
            len = _SC_Read(u8SCID,&pu8Data[8+i], (*u16DataLen)-i);
            if (len < 0)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Read get fail\n");
                *u16DataLen = i;
                return E_SC_FAIL;
            }
        }
        *u16DataLen = i+8;

        return E_SC_OK;
    }
    return E_SC_OK;
}
#endif


static SC_Result _SC_ClearState(MS_U8 u8SCID)
{
    memset(_scInfo[u8SCID].pu8Atr, 0x0, SC_ATR_LEN_MAX);
    memset(_scInfo[u8SCID].pu8Hist, 0x0, SC_HIST_LEN_MAX);
    _scInfo[u8SCID].u8Protocol = 0xFF;
    _scInfo[u8SCID].bSpecMode = FALSE;
    _scInfo[u8SCID].u8Fi = 0;
    _scInfo[u8SCID].u8Di = 0; // unspecified.

    return E_SC_OK;
}


static SC_Result _SC_Activate(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    //
    // initialize SMART regsiters
    //
    // Smartcard IC Vcc Control -------------------------------------------------------------------
    if (SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT)
    {
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
            if (u8SCID == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (u8SCID == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
            }
#endif
        }

        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME * 10);// Wait Card stable
            SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            SC_AND(u8SCID,UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
            // move "~UART_LCR_SBC" and "UART_SCCR_RST" to here when on chip 8024
            // need to set "~UART_LCR_SBC" here when on chip 8024, otherwise active sequence trigger can't work in IO pin
            SC_AND(u8SCID,UART_LCR, ~UART_LCR_SBC); //I/O
            // need to set "UART_SCCR_RST" here when on chip 8024, otherwise active sequence trigger can't work in RST pin
            SC_OR(u8SCID,UART_SCCR, UART_SCCR_RST);//active high
            _SC_OnChip8024ActiveSeq();
        }
        else
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
            {
                SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
            {
                SC_AND(u8SCID,UART_SCFR, ~UART_SCFR_V_HIGH);
            }
            else//OCP
            {
                //if (_scInfo[u8SCID].bCardIn == FALSE)
                _SC_SetOCP(u8SCID, TRUE);
            }
            OS_SC_DELAY(1);// Wait vcc stable
        }

        // don't need these code when when on chip 8024
        if (_scInfo[u8SCID].eVccCtrl != E_SC_VCC_VCC_ONCHIP_8024)
        {
            if (u8SCID == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (u8SCID == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
            }
#endif
            SC_AND(u8SCID,UART_LCR, ~UART_LCR_SBC); //I/O
        }
    }
    return E_SC_OK;
}

static SC_Result _SC_Deactivate(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
    {
        SC_OR(u8SCID,UART_SCFR, (UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
    }
    else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
        _SC_OnChip8024DeactiveSeq();
    }
    else
    {
        SC_AND(u8SCID, UART_SCCR, ~UART_SCCR_RST);//active low
        OS_SC_DELAY(3);// Wait Card stable
        //SC_WRITE(u8SCID, UART_SCSR, 0x00);
        if (u8SCID == 0)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); // clock disable
        }
#if (SC_DEV_NUM > 1) // no more than 2
        if (u8SCID == 1)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA1_DIS)); // clock disable
        }
#endif
        OS_SC_DELAY(3);// Wait Card stable
        SC_OR(u8SCID,UART_LCR, UART_LCR_SBC); //I/O
        OS_SC_DELAY(3);// Wait Card stable

        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
        {
            SC_AND(u8SCID, UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
        }
        else  if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
        {
            SC_OR(u8SCID, UART_SCFR, UART_SCFR_V_HIGH); // 'b1
        }
        else //OCP
        {
            //if (_scInfo[u8SCID].bCardIn == TRUE)
            _SC_SetOCP(u8SCID, FALSE);
        }
    }
    return E_SC_OK;
}

static SC_Result _SC_Close(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s:\n", __FUNCTION__);

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        return E_SC_FAIL;
    }

    // Disable all interrupts
    SC_WRITE(u8SCID,UART_IER, 0x00);
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_DETACH_INTERRUPT))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_EXIT failed\n", __FUNCTION__, __LINE__);
    }
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    OS_SC_ISR_Disable(u8SCID);
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)

    _scInfo[u8SCID].bOpened = FALSE;

    // Disable Smartcard Clk ----------------------------------------------------------------------

    SC_WRITE(u8SCID,UART_SCSR, 0x00);                                       // disable clock
    // Disable Smartcard IC Vcc Control -----------------------------------------------------------

    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
    {
        SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE));
    }
    else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
    }
    else
    {
        if ((_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH) ||
             (_scInfo[u8SCID].eVccCtrl == E_SC_OCP_VCC_HIGH))
        {
#if SMART_OCP_EN
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP0));
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP1));
            TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP2));
#endif
            SC_AND(u8SCID,UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
        }
        else
            SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
    }

    // reset buffer control pointer
    _SC_ResetFIFO(u8SCID);

    return E_SC_OK;
}




#ifdef SC_USE_IO_51
static SC_Result _SC_ParseATR(MS_U8 u8SCID, MS_U8 *pu8ATR, MS_U16 u16ATRLen)
{
    MS_U8            u8TS, u8T0, u8TAx, u8TBx, u8TCx, u8TDx, u8Len, u8TA2;
    MS_U8            u8Ch, u8Crc = 0;

    MS_S32           i, x;

    _scInfo[u8SCID].u16AtrLen = u16ATRLen;

    memcpy(_scInfo[u8SCID].pu8Atr, pu8ATR, _scInfo[u8SCID].u16AtrLen);

    //[DEBUG]
    /*
    printf("ATR::: ");
    for (i=0;i<_scInfo[u8SCID].u16AtrLen;i++)
    {
        printf("0x%x, ", _scInfo[u8SCID].pu8Atr[i]);
    }
    printf("\n");
    */
    //[DEBUG]


    if (_scInfo[u8SCID].u16AtrLen < SC_ATR_LEN_MIN)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Length error\n");
        return E_SC_FAIL;
    }
    if ((_scInfo[u8SCID].pu8Atr[0] == 0x23) || (_scInfo[u8SCID].pu8Atr[0] == 0x03))
    {
        SC_DBG(E_SC_DBGLV_INFO, "     Inverse convention\n");

        //try to reverse the inverse data
        if (_SC_SetInvConv(u8SCID) != E_SC_OK)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "    set inverse error\n");
            return E_SC_FAIL;
        }

        // set all of the ATR data to be inverse convention
        SC_DBG(E_SC_DBGLV_INFO,"ATR::: ");
        for (i=0;i<_scInfo[u8SCID].u16AtrLen;i++)
        {
            MS_U8 data0 = ~(_scInfo[u8SCID].pu8Atr[i]);

            data0 = ((data0 >> 1) & 0x55) | ((data0 << 1) & 0xAA);
            data0 = ((data0 >> 2) & 0x33) | ((data0 << 2) & 0xCC);
            data0 = ((data0 >> 4) & 0x0F) | ((data0 << 4) & 0xF0);
            _scInfo[u8SCID].pu8Atr[i] = data0;

            SC_DBG(E_SC_DBGLV_INFO,"0x%x, ", _scInfo[u8SCID].pu8Atr[i]);
        }
        SC_DBG(E_SC_DBGLV_INFO," \n");
    }
    if ((_scInfo[u8SCID].pu8Atr[0] != 0x3B) && (_scInfo[u8SCID].pu8Atr[0] != 0x3F))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    first byte error 0x%02X\n", _scInfo[u8SCID].pu8Atr[0]);
        return E_SC_FAIL;
    }

    i = 0;
    u8TS = _scInfo[u8SCID].pu8Atr[i++];
    u8T0 = _scInfo[u8SCID].pu8Atr[i++];

    u8Crc = u8Crc ^ u8T0;
    u8Len = u8T0 & 0x0F;
    u8TDx = u8T0 & 0xF0;

    x = 1;
    _scInfo[u8SCID].bSpecMode = FALSE;
    _scInfo[u8SCID].u8Di = _scInfo[u8SCID].u8Fi = 0;
    _scInfo[u8SCID].u8Protocol = 0;
    while (u8TDx & 0xF0)
    {
        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }

        if (u8TDx & 0x10) // TA
        {
            u8TAx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TAx;
            if (x == 1)
            {
                _scInfo[u8SCID].u8Fi = u8TAx >> 4;
                _scInfo[u8SCID].u8Di = u8TAx & 0xF;
            }
            if (x == 2)
            {
                _scInfo[u8SCID].bSpecMode = TRUE; // specific mode
                u8TA2 = u8TAx;
            }
            if (x == 3)
            {
                _sc_T1State[u8SCID].u8IFSC = u8TAx;
            }
        }
        if (u8TDx & 0x20) // TB
        {
            u8TBx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TBx;
            if (x == 3)
            {
//                _sc_T1State[u8SCID].u8BWI = u8TBx >> 4;
//                _sc_T1State[u8SCID].u8CWI = u8TBx & 0xF;
//                printf("BWI = %x \n", _sc_T1State[u8SCID].u8BWI);
//                printf("CWI = %x \n", _sc_T1State[u8SCID].u8CWI);
            }
        }
        if (u8TDx & 0x40) // TC
        {
            u8TCx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TCx;
            if (x == 3)
            {
                _sc_T1State[u8SCID].u8RCType = u8TCx & 0x1;
            }
        }
        if ((u8TDx & 0x80))
        {
            u8TDx = _scInfo[u8SCID].pu8Atr[i++];
            _scInfo[u8SCID].u8Protocol = u8TDx & 0xF;
            u8Crc = u8Crc ^ u8TDx;

            SC_DBG(E_SC_DBGLV_INFO, "    T=%d\n", _scInfo[u8SCID].u8Protocol);
            x++;
        }
        else
        {
            break;
        }
    }

    // Get ATR history
    _scInfo[u8SCID].u16HistLen = u8Len;

    _scInfo[u8SCID].u16AtrLen = i + _scInfo[u8SCID].u16HistLen;

    for (x = 0; x < u8Len; x++)
    {
        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }
        u8Ch = _scInfo[u8SCID].pu8Atr[i++];
        u8Crc = u8Crc ^ u8Ch;

        _scInfo[u8SCID].pu8Hist[x] = u8Ch;
    }

    // Check ATR checksum
    if ((_scInfo[u8SCID].u8Protocol != 0x00) && (_scInfo[u8SCID].u8Protocol != 0xFF)) // If not T=0
    {
        _scInfo[u8SCID].u16AtrLen += 1;

        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }
        if (u8Crc != _scInfo[u8SCID].pu8Atr[i++])
        {
            return E_SC_CRCERROR;
        }
    }

    // Check ATR length
    if (i != _scInfo[u8SCID].u16AtrLen)
    {
        return E_SC_FAIL; // len error
    }

    return E_SC_OK;

}
static SC_Result _SC_GetATR(MS_U8 u8SCID, MS_U32 u32TimeoutMs)
{
    SC_Result ScResult = E_SC_FAIL;
    MBX_Result mbxResult = E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MS_U32 u32SrcAddr;
    MS_U32 MBX_Resendcnt = 0;

    u32TimeoutMs = u32TimeoutMs;

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        ULOGE(TAG_SC, "Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
        return E_SC_FAIL;
    }

    gpScDataBuffVA->u8SCID = u8SCID;
    gpScDataBuffVA->u16DataLen = SC_ATR_LEN_MAX;
    gpScDataBuffVA->u16ReplyMaxLen = SC_ATR_LEN_MAX;

    MsOS_FlushMemory();
    MsOS_Sync();

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gScAckFlags |= E_SC_ACKFLG_WAIT_GET_ATR;

    u32SrcAddr = (MS_VIRT) gpScDataBuffPA;
    // send to 8051
    {
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;  //0x01;
        MB_Command.u8MsgClass = E_MBX_CLASS_PM_NOWAIT;
        MB_Command.u8Index = SC_CMDIDX_GET_ATR;
        MB_Command.u8ParameterCount = 8;
        MB_Command.u8Parameters[0] =  (MS_U8)(u32SrcAddr>>24);
        MB_Command.u8Parameters[1] =  (MS_U8)(u32SrcAddr>>16);
        MB_Command.u8Parameters[2] =  (MS_U8)(u32SrcAddr>>8);
        MB_Command.u8Parameters[3] =  (MS_U8)(u32SrcAddr&0x000000FF);
        if (gbTimeout)
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(u32TimeoutMs>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(u32TimeoutMs>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(u32TimeoutMs>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(u32TimeoutMs&0x000000FF);
        }
        else
        {
            MB_Command.u8Parameters[4] =  (MS_U8)(0>>24);
            MB_Command.u8Parameters[5] =  (MS_U8)(0>>16);
            MB_Command.u8Parameters[6] =  (MS_U8)(0>>8);
            MB_Command.u8Parameters[7] =  (MS_U8)(0&0x000000FF);
        }

        while(E_MBX_SUCCESS != (mbxResult = MDrv_MBX_SendMsg(&MB_Command)))
        {
            //Error Handling here:
        }
    }

    // Waiting for 8051 Getting ATR
    do
    {
        if(MBX_Resendcnt<10)
        {
            mbxResult = _MDrv_SC_MailBoxHandler(&ScResult);
            MBX_Resendcnt++;
        }
        else
        {
            mbxResult = E_MBX_ERR_TIME_OUT;
            ScResult = E_SC_TIMEOUT;
            //TODO: return data length = 0?
            break;
        }
    }
    while((gScAckFlags & E_SC_ACKFLG_WAIT_GET_ATR) &&
                (mbxResult != E_MBX_ERR_TIME_OUT));

#if 0 // debug
    MS_S32 i;
    printf("Data Len = %d\n", gpScDataBuffVA->u16DataLen);
    for (i = 0; i < gpScDataBuffVA->u16DataLen; i++)
    {
        printf("%x,", gpScDataBuffVA->u8Data[i]);
    }
    printf("\n");
#endif

    if( E_MBX_SUCCESS != mbxResult)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_GetATR Fail\n");
    }
    else
    {
        if (gpScDataBuffVA->u16DataLen == 0)
        {
            ScResult = E_SC_NODATA;
        }
        else
        {
            _SC_ParseATR(gpScDataBuffVA->u8SCID, gpScDataBuffVA->u8Data, gpScDataBuffVA->u16DataLen);

            //*pu16AtrLen = _scInfo[u8SCID].u16AtrLen;
            //*pu16HisLen = _scInfo[u8SCID].u16HistLen;
            //memcpy(pu8Atr, _scInfo[u8SCID].pu8Atr, _scInfo[u8SCID].u16AtrLen);
            //memcpy(pu8His, _scInfo[u8SCID].pu8Hist, _scInfo[u8SCID].u16HistLen);

            SC_DBG(E_SC_DBGLV_INFO, "_SC_GetATR OK\n");
            ScResult = E_SC_OK;
        }
    }

    return ScResult;


}

#else
static SC_Result _SC_GetATR(MS_U8 u8SCID, MS_U32 u32TimeoutMs)
{
    MS_U8               u8T0, u8TAx, u8TBx, u8TCx, u8TDx, u8Len;
    MS_U8               u8Ch, u8Crc = 0;

    MS_S32              i, x;
    SC_Result           res;

    SC_DBG(E_SC_DBGLV_INFO, "%s:\n", __FUNCTION__);
    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }
    _scInfo[u8SCID].u16AtrLen = SC_ATR_LEN_MAX;
    res = _SC_Recv(u8SCID, _scInfo[u8SCID].pu8Atr, &(_scInfo[u8SCID].u16AtrLen), u32TimeoutMs);

//[DEBUG]
/*
    SC_DBG(E_SC_DBGLV_REG_DUMP, "ATR::: ");
    for (i=0;i<_scInfo[u8SCID].u16AtrLen;i++)
    {
        SC_DBG(E_SC_DBGLV_REG_DUMP, "0x%x, ", _scInfo[u8SCID].pu8Atr[i]);
    }
    SC_DBG(E_SC_DBGLV_REG_DUMP, "\n");
*/
//[DEBUG]

    if (_scInfo[u8SCID].u16AtrLen < SC_ATR_LEN_MIN)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Length error\n");
        return E_SC_FAIL;
    }
    if ((_scInfo[u8SCID].pu8Atr[0] == 0x23) || (_scInfo[u8SCID].pu8Atr[0] == 0x03))
    {
        SC_DBG(E_SC_DBGLV_INFO, "     Inverse convention\n");

        //try to reverse the inverse data
        if (_SC_SetInvConv(u8SCID) != E_SC_OK)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "    set inverse error\n");
            return E_SC_FAIL;
        }

        // set all of the ATR data to be inverse convention
        SC_DBG(E_SC_DBGLV_INFO,"ATR::: ");
        for (i=0;i<_scInfo[u8SCID].u16AtrLen;i++)
        {
            MS_U8 data0 = ~(_scInfo[u8SCID].pu8Atr[i]);

            data0 = ((data0 >> 1) & 0x55) | ((data0 << 1) & 0xAA);
            data0 = ((data0 >> 2) & 0x33) | ((data0 << 2) & 0xCC);
            data0 = ((data0 >> 4) & 0x0F) | ((data0 << 4) & 0xF0);
            _scInfo[u8SCID].pu8Atr[i] = data0;

            SC_DBG(E_SC_DBGLV_INFO,"0x%x, ", _scInfo[u8SCID].pu8Atr[i]);
        }
        SC_DBG(E_SC_DBGLV_INFO," \n");
    }
    if ((_scInfo[u8SCID].pu8Atr[0] != 0x3B) && (_scInfo[u8SCID].pu8Atr[0] != 0x3F))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    first byte error 0x%02X\n", _scInfo[u8SCID].pu8Atr[0]);
        return E_SC_FAIL;
    }

    i = 0;
    //u8TS = _scInfo[u8SCID].pu8Atr[i++]; fix compile error
    i++;
    u8T0 = _scInfo[u8SCID].pu8Atr[i++];

    u8Crc = u8Crc ^ u8T0;
    u8Len = u8T0 & 0x0F;
    u8TDx = u8T0 & 0xF0;

    x = 1;
    _scInfo[u8SCID].bSpecMode = FALSE;
    _scInfo[u8SCID].u8Di = _scInfo[u8SCID].u8Fi = 0;
    _scInfo[u8SCID].u8Protocol = 0;
    while (u8TDx & 0xF0)
    {
        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }

        if (u8TDx & 0x10) // TA
        {
            u8TAx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TAx;
            if (x == 1)
            {
                _scInfo[u8SCID].u8Fi = u8TAx >> 4;
                _scInfo[u8SCID].u8Di = u8TAx & 0xF;
            }
            if (x == 2)
            {
                _scInfo[u8SCID].bSpecMode = TRUE; // specific mode
                //u8TA2 = u8TAx; fix compile error
            }
            if (x == 3)
            {
                _sc_T1State[u8SCID].u8IFSC = u8TAx;
            }
        }
        if (u8TDx & 0x20) // TB
        {
            u8TBx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TBx;
        }
        if (u8TDx & 0x40) // TC
        {
            u8TCx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TCx;
            if (x == 3)
            {
                _sc_T1State[u8SCID].u8RCType = u8TCx & 0x1;
            }
        }
        if ((u8TDx & 0x80))
        {
            u8TDx = _scInfo[u8SCID].pu8Atr[i++];
            _scInfo[u8SCID].u8Protocol = u8TDx & 0xF;
            u8Crc = u8Crc ^ u8TDx;

            SC_DBG(E_SC_DBGLV_INFO, "    T=%d\n", _scInfo[u8SCID].u8Protocol);
            x++;
        }
        else
        {
            break;
        }
    }

    // Get ATR history
    _scInfo[u8SCID].u16HistLen = u8Len;

    _scInfo[u8SCID].u16AtrLen = i + _scInfo[u8SCID].u16HistLen;

    for (x = 0; x < u8Len; x++)
    {
        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }
        u8Ch = _scInfo[u8SCID].pu8Atr[i++];
        u8Crc = u8Crc ^ u8Ch;

        _scInfo[u8SCID].pu8Hist[x] = u8Ch;
    }

    // Check ATR checksum
    if ((_scInfo[u8SCID].u8Protocol != 0x00) && (_scInfo[u8SCID].u8Protocol != 0xFF)) // If not T=0
    {
        _scInfo[u8SCID].u16AtrLen += 1;

        if (i >= _scInfo[u8SCID].u16AtrLen)
        {
            return E_SC_FAIL;
        }
        if (u8Crc != _scInfo[u8SCID].pu8Atr[i++])
        {
            return E_SC_CRCERROR;
        }
    }

    // Check ATR length
    if (i != _scInfo[u8SCID].u16AtrLen)
    {
        return E_SC_FAIL; // len error
    }
    res = E_SC_OK;

    return res;
}

#endif
static SC_Result _SC_PPS(MS_U8 u8SCID, MS_U8 u8T, MS_U8 u8DI, MS_U8 u8FI)
{
    SC_Result           eResult = E_SC_OK;
    MS_U8               u8SendData[SC_PPS_LEN_MAX];
    MS_U16              u16SendLen;
    MS_U32              u32Idx;

    //NOTE: Just consider D/F for "work etc" here
    _pu8PPS[0] = u8SendData[0] = 0xFF;                                  // PPSS
    _pu8PPS[1] = u8SendData[1] = 0x10 | (0x0F & u8T);                   // T=x
    _pu8PPS[2] = u8SendData[2] = u8DI | (u8FI<<4);                      // PPS1
    _pu8PPS[3] = u8SendData[3] = _pu8PPS[0] ^ _pu8PPS[1] ^ _pu8PPS[2];  // PCK
    _u16PPSLen = u16SendLen = 4;

#ifdef SC_USE_IO_51
    eResult= _SC_RawExchange(u8SCID, u8SendData, &u16SendLen, _pu8PPS, &_u16PPSLen, 0);
#else
    if (((eResult = _SC_Send(u8SCID, u8SendData, u16SendLen, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    PPS Send Failed!!\n");
        return eResult;
    }

    eResult = _SC_Recv(u8SCID, _pu8PPS, &_u16PPSLen, SC_T0_RECV_TIMEOUT);
#endif
    if (!_u16PPSLen)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    PPS RECV Failed!!\n");
        return eResult;
    }

//[DEBUG]
/*
    SC_DBG(E_SC_DBGLV_INFO, "PPS::: ");
    for (u32Idx = 0; u32Idx < _u16PPSLen; u32Idx++)
    {
        SC_DBG(E_SC_DBGLV_INFO, "0x%x, ", _pu8PPS[u32Idx]);
    }
    SC_DBG(E_SC_DBGLV_INFO, "\n");
*/
//[DEBUG]

    if (_u16PPSLen != u16SendLen)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    PPS Wrong ACK Len!!\n");
        return E_SC_FAIL;
    }

    for (u32Idx = 0; u32Idx < _u16PPSLen; ++u32Idx)
    {
        if (_pu8PPS[u32Idx] != u8SendData[u32Idx])
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "    PPS ACK FAILED!!\n");
            return E_SC_FAIL;
        }
    }

    return E_SC_OK;
}

SC_Result _SC_Task_Proc(MS_U8 u8SCID, MS_U32 u32Events)
{
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U32 u32Evt;

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (gbInitCheckDelay == TRUE)
    {
        MsOS_DelayTask(300);
        ULOGD(TAG_SC, "delay for get events\n");
        gbInitCheckDelay = FALSE;
    }

    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_GET_EVENTS, (MS_S32 *) &u32Evt))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_GET_EVENTS failed\n", __FUNCTION__, __LINE__);
    }

    if (u32Evt & UART_SCSR_INT_CARDOUT) // card out
    {
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
        {
            SC_OR(u8SCID,UART_SCFR, (UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
        }
        else
        {
            // Clear interrupt for 1 ms to avoid the slow drop of signals (IO, Vcc) caused by the indication of interrupt
            MS_U32 u32CurrTime = MsOS_GetSystemTime();
            while ((u32CurrTime + 1 ) - MsOS_GetSystemTime() != 0)
            {
                SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID,UART_SCSR));
                /* Avoid time interval overflow */
                if ((u32CurrTime + 1 ) - MsOS_GetSystemTime() > 1)
                    break;
            }

            SC_AND(u8SCID, UART_SCCR, ~UART_SCCR_RST);//reset active low
            OS_SC_DELAY(3);// Wait Card stable
            if (u8SCID == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); // clock disable
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (u8SCID == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA1_DIS)); // clock disable
            }
#endif
            OS_SC_DELAY(3);// Wait Card stable
            SC_WRITE(u8SCID,UART_LCR, UART_LCR_SBC); //I/O low
            OS_SC_DELAY(3);// Wait Card stable
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
            {
                SC_AND(u8SCID, UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
            }
            else  if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
            {
                SC_OR(u8SCID, UART_SCFR, UART_SCFR_V_HIGH); // 'b1
            }
            else //OCP
            {
                _SC_SetOCP(u8SCID, FALSE);
            }
        }

        _scInfo[u8SCID].bCardIn = FALSE;
    }
    else if (SC_READ(u8SCID, UART_SCSR) & UART_SCSR_DETECT) // card in
    {
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME * 10);// Wait Card stable
            SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            SC_AND(u8SCID,UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
        }
        else
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
            {
                SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
            {
                SC_AND(u8SCID,UART_SCFR, ~UART_SCFR_V_HIGH);
            }
            else //OCP
            {
                _SC_SetOCP(u8SCID, TRUE);
            }
            OS_SC_DELAY(1);// Wait VCC stable

            SC_OR(u8SCID, UART_SCSR, UART_SCSR_CLK);
        }
        if (u8SCID == 0)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
        }
#if (SC_DEV_NUM > 1) // no more than 2
        if (u8SCID == 1)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
        }
#endif
        SC_AND(u8SCID,UART_LCR, ~UART_LCR_SBC); //I/O

        _scInfo[u8SCID].bCardIn = TRUE;
    }

    if (_scInfo[u8SCID].bLastCardIn != _scInfo[u8SCID].bCardIn)
    {
        _scInfo[u8SCID].bLastCardIn = _scInfo[u8SCID].bCardIn;
        if (_scInfo[u8SCID].bCardIn)
        {
            _SC_Callback_Notify(u8SCID, E_SC_EVENT_IN);
        }
        else
        {
            _SC_Callback_Notify(u8SCID, E_SC_EVENT_OUT);
        }
    }
    return E_SC_OK;
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    SC_Result           ret = E_SC_OK;

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    // Send Event //TODO why????? Unless we do send fifo control
    if ((u32Events & OS_SC_EVENT_MASK) == OS_SC_EVENT_SEND)
    {
        //TODO SendData.eResult???????
        _scSendData.eResult = _SC_Send(u8SCID, (MS_U8 *)_scSendData.u32FifoDataBuf, _scSendData.u16DataLen, _scSendData.u32TimeMs);
    }
    else // Card Event
    {
        //Avoid card-detect signal bouce when inserting card
        if ((SC_READ(u8SCID, UART_SCSR) & (UART_SCSR_INT_CARDIN|UART_SCSR_INT_CARDOUT)) && (_scInfo[u8SCID].bCardIn == FALSE))
        {
            MsOS_DelayTask(1);
            SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID,UART_SCSR)& UART_SCSR_INT_CARDOUT); // clear car out interrupt
        }
// cynthia : temporarily patch for fast INSERT/REMOVE
// @TODO: refine it
ReDetect:
        if (!(SC_READ(u8SCID, UART_SCSR) & UART_SCSR_DETECT) || (SC_READ(u8SCID, UART_SCSR) & UART_SCSR_INT_CARDOUT)) // card out
        {
            SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID,UART_SCSR)); // clear interrupt
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                SC_OR(u8SCID,UART_SCFR, (UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                SC_AND(u8SCID, UART_SCCR, ~UART_SCCR_RST);//active low
                OS_SC_DELAY(3);// Wait Card stable
                //SC_WRITE(u8SCID, UART_SCSR, 0x00);
                if (u8SCID == 0)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); // clock disable
                }
#if (SC_DEV_NUM > 1) // no more than 2
                if (u8SCID == 1)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA1_DIS)); // clock disable
                }
#endif
                OS_SC_DELAY(3);// Wait Card stable
                SC_OR(u8SCID,UART_LCR, UART_LCR_SBC); //I/O
                OS_SC_DELAY(3);// Wait Card stable

                if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_AND(u8SCID, UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
                }
                else  if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_OR(u8SCID, UART_SCFR, UART_SCFR_V_HIGH); // 'b1
                }
                else //OCP
                {
                    //if (_scInfo[u8SCID].bCardIn == TRUE)
                    _SC_SetOCP(u8SCID, FALSE);
                }
            }
            _scInfo[u8SCID].bCardIn = FALSE;
        }

        if ((SC_READ(u8SCID, UART_SCSR) & UART_SCSR_DETECT) && !(SC_READ(u8SCID, UART_SCSR) & UART_SCSR_INT_CARDOUT)) // card in
        {
            SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID, UART_SCSR)); // clear interrupt
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                OS_SC_DELAY(SC_CARD_STABLE_TIME * 10);// Wait Card stable
                SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
                SC_AND(u8SCID,UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
                if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH); // 'b11
                }
                else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_AND(u8SCID,UART_SCFR, ~UART_SCFR_V_HIGH);
                }
                else//OCP
                {
                    //if (_scInfo[u8SCID].bCardIn == FALSE)
                    _SC_SetOCP(u8SCID, TRUE);
                }
                OS_SC_DELAY(1);// Wait vcc stable
            }

            if (u8SCID == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (u8SCID == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
            }
#endif
            SC_AND(u8SCID,UART_LCR, ~UART_LCR_SBC); //I/O

            _scInfo[u8SCID].bCardIn = TRUE;
        }

        // Wait Card stable
        OS_SC_DELAY(SC_CARD_STABLE_TIME);

        // cynthia : temporarily patch for fast INSERT/REMOVE
        // @TODO: refine it
        if ((SC_READ(u8SCID, UART_SCSR) & UART_SCSR_INT_CARDOUT))
        {
            //_scInfo[u8SCID].bLastCardIn = _scInfo[u8SCID].bCardIn;
            goto ReDetect;
        }

        if (_scInfo[u8SCID].bLastCardIn != _scInfo[u8SCID].bCardIn)
        {
            _scInfo[u8SCID].bLastCardIn = _scInfo[u8SCID].bCardIn;

            //SC_EVENT();
            if (_scInfo[u8SCID].bCardIn)
            {
                _SC_Callback_Notify(u8SCID, E_SC_EVENT_IN);
                ret = E_SC_CARDIN; // for polling mode (OS_NOS)
            }
            else
            {
                _SC_Callback_Notify(u8SCID, E_SC_EVENT_OUT);
                ret = E_SC_CARDOUT; // for polling mode (OS_NOS)
            }
        }
    }

    return ret;
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
}

static void _SC_T1AppendRC( SC_T1_State *pT1, MS_U8 *pu8Block, MS_U16 *pu16BlockLen  )
{
    //RC-CRC
    if (pT1->u8RCType & 0x1)
    {
        MS_U32 i;
        MS_U16 u16CRC = 0xFFFF;
        MS_U16 u16BlockLenLocal = *pu16BlockLen;

        while (u16BlockLenLocal--)
        {
            u16CRC ^= *pu8Block++;
            for (i=0;i<8;i++)
            {
                if (u16CRC & 0x01)
                {
                    // LSB = 1
                    u16CRC = (u16CRC>>1) ^ 0xA001;
                }
                else
                {
                    //LSB = 2
                    u16CRC = u16CRC >>1;
                }
            }
        }
        pu8Block[*pu16BlockLen]=(u16CRC >> 8) & 0xFF;
        pu8Block[*pu16BlockLen+1]=u16CRC & 0xFF;
        *pu16BlockLen +=2;
    }
    //RC-LRC
    else
    {
        MS_U8 u8LRC=0x00;
        MS_U8 i;
        for (i=0; i<(*pu16BlockLen); i++)
        {
            u8LRC ^= pu8Block[i];
        }
        pu8Block[*pu16BlockLen] = u8LRC;
        *pu16BlockLen +=1;
    }

}


static SC_Result _SC_T1IBlock( SC_T1_State *pT1, MS_BOOL ubMore, MS_U8 *pu8Data, MS_U16 u16DataLen, MS_U8 *pu8Block, MS_U16 *pu16BlockLen )
{
    //_SC_T1IBlock only can handle the data size from 0~255
    if (u16DataLen > 255)
    {
        return E_SC_PARMERROR;
    }

    //[1] NAD
    pu8Block[0] = pT1->u8NAD;

    //[2] PCB - N(S),More bit
    pu8Block[1] = 0x00;
    if (pT1->u8NS)
        pu8Block[1] |= 0x40;

    if (ubMore)
        pu8Block[1] |= 0x20;

    if (u16DataLen > pT1->u8IFSC)
    {
        return E_SC_FAIL;
    }

    //[3] LEN
    pu8Block[2]=(MS_U8) u16DataLen;

    //[4] DATA
    memcpy( pu8Block+3, pu8Data, u16DataLen );

    *pu16BlockLen=u16DataLen+3;

    //[5] EDC
    _SC_T1AppendRC(pT1,pu8Block,pu16BlockLen);

    return E_SC_OK;
}


// build R block
static void _SC_T1RBlock( SC_T1_State *pT1, MS_U32 u32Type, MS_U8 *pu8Block, MS_U16 *pu16BlockLen )
{
    pu8Block[0] = pT1->u8NAD;
    pu8Block[2] = 0x00;

    if (u32Type == SC_T1_R_OK)
    {
        if(pT1->u8NR)
        {
            pu8Block[1]=0x90;
        }
        else
        {
            pu8Block[1]=0x80;
        }
    }
    else if (u32Type == SC_T1_R_EDC_ERROR)
    {
        if (pT1->u8NR)
        {
            pu8Block[1]=0x91;
        }
        else
        {
            pu8Block[1]=0x81;
        }
    }
    else if (u32Type == SC_T1_R_OTHER_ERROR)
    {
        if (pT1->u8NR)
        {
            pu8Block[1]=0x92;
        }
        else
        {
            pu8Block[1]=0x82;
        }
    }
    *pu16BlockLen=3;

    _SC_T1AppendRC(pT1,pu8Block,pu16BlockLen);
}


static SC_Result _SC_T1SBlock( SC_T1_State *pT1, MS_U32 u32Type, MS_U32 u8Dir, MS_U8 u8Param, MS_U8 *pu8Block,
    MS_U16 *pu16len )
{
    pu8Block[0]=pT1->u8NAD;

    switch (u32Type)
    {
    case SC_T1_S_RESYNCH:
        if (u8Dir==SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC0;
        }
        else
        {
            pu8Block[1]=0xE0;
        }
        pu8Block[2]=0x00;
        *pu16len=3;
        break;
    case SC_T1_S_IFS:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC1;
        }
        else
        {
            pu8Block[1]=0xE1;
        }
        pu8Block[2]=0x01;
        pu8Block[3]= u8Param;
        *pu16len=4;
        break;
    case SC_T1_S_ABORT:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC2;
        }
        else
        {
            pu8Block[1]=0xE2;
        }
        pu8Block[2]=0x00;
        *pu16len=3;
        break;
    case SC_T1_S_WTX:
        if (u8Dir == SC_T1_S_REQUEST)
        {
            pu8Block[1]=0xC3;
        }
        else
        {
            pu8Block[1]=0xE3;
        }
        pu8Block[2]=0x01;
        pu8Block[3]= u8Param;
        *pu16len=4;
        break;
    default:
        return E_SC_FAIL;
    }
    _SC_T1AppendRC(pT1, pu8Block, pu16len);

    return E_SC_OK;
}

//get block S(n),R(n)
static MS_U8  _SC_T1GetBlockN( MS_U8 *pu8Block )
{
    // I-Block
    if ((pu8Block[1] & 0x80) == 0x00)
    {
        return((pu8Block[1]>>6) & 0x01);
    }

    // R-Block
    if ((pu8Block[1]&0xC0) == 0x80)
    {
        return((pu8Block[1]>>4) & 0x01);
    }
    return 0;
}

static SC_Result _SC_T1SendRecvBlock( MS_U8 u8SCID,MS_U8 *pu8Block, MS_U16 *pu16BlockLen, MS_U8 *pu8RspBlock, MS_U16 *pu16RspBlockLen)
{
    SC_Result ret_Result = E_SC_FAIL;

    MsOS_DelayTask(300);
    //send data
    ret_Result = _SC_Send(u8SCID, pu8Block, *pu16BlockLen, SC_T1_SEND_TIMEOUT);
    if (ret_Result != E_SC_OK)
    {
        return ret_Result;
    }
    //block waiting time
    MsOS_DelayTask(300);

    //receive data
    ret_Result = _SC_Recv(u8SCID, pu8RspBlock, pu16RspBlockLen, SC_T1_RECV_TIMEOUT);

    return ret_Result;
}



//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

SC_Result MDrv_SC_Task_Proc(void)
{
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U32              i;
    struct pollfd       sPoll;
#if (SC_DEV_NUM>1)
    static MS_S32 err[2] = {-1,-1};
    static MS_S32 err_in[2] = {-1,-1};
#else
    static MS_S32 err[1] = {-1};
    static MS_S32 err_in[1] = {-1};
#endif

    for (i = 0; i < SC_DEV_NUM; i++)
    {
        sPoll.fd = _scInfo[i].s32DevFd;
        sPoll.events = POLLPRI;
        if (0 < poll(&sPoll, 1, 50))
        {
            err[i] = -1;
            err_in[i] = -1;
            _SC_Task_Proc(i, OS_SC_EVENT_CARD);
        }
    }
    OS_SC_DELAY(50);
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (_scInfo[i].bCardIn && !(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
        {
            OS_SC_DELAY(10);
            if (_scInfo[i].bCardIn && !(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
                ;
            else if ((SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
            {
                err[i] = -1;
                return E_SC_OK;
            }
            err[i] ++;
        }

        if (err[i] > 0) // confirm jump into the hw bug state
        {
            _scInfo[i].bCardIn = FALSE;
            _scInfo[i].bLastCardIn = _scInfo[i].bCardIn;
            if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                SC_OR(i,UART_SCFR, (UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            }
            else if (_scInfo[i].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                SC_AND(i,UART_SCCR, ~UART_SCCR_RST);//reset active low
                OS_SC_DELAY(3);// Wait Card stable
                if (i == 0)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); // clock disable
                }
#if (SC_DEV_NUM > 1) // no more than 2
                if (i == 1)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA1_DIS)); // clock disable
                }
#endif
                OS_SC_DELAY(3);// Wait Card stable
                SC_OR(i,UART_LCR, UART_LCR_SBC); //I/O
                OS_SC_DELAY(3);// Wait Card stable
                if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_AND(i,UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
                }
                else  if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_OR(i,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
                }
                else //OCP
                {
                    //if (_scInfo[u8SCID].bCardIn == TRUE)
                    _SC_SetOCP(i, FALSE);
                }
            }

            err[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_OUT); // Polling CARD_OUT
        }
    }
#if 1
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (!_scInfo[i].bCardIn && (SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
        {
            OS_SC_DELAY(10);
            if (!_scInfo[i].bCardIn && (SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
                ;
            else if (!(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
            {
                err[i] = -1;
                return E_SC_OK;
            }
            err_in[i] ++;
        }
        if (err_in[i] > 0) // confirm jump into the hw bug state
        {
            _scInfo[i].bCardIn = TRUE;
            _scInfo[i].bLastCardIn = _scInfo[i].bCardIn;
            if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                SC_AND(i, UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
                SC_AND(i, UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
            }
            else if (_scInfo[i].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
                if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_OR(i,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
                }
                else if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_AND(i,UART_SCFR, ~UART_SCFR_V_HIGH);
                }
                else //OCP
                {
                    _SC_SetOCP(i, TRUE);
                }
                OS_SC_DELAY(1);// Wait VCC stable

                SC_OR(i, UART_SCSR, UART_SCSR_CLK);
            }
            if (i == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (i == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
            }
#endif

            err_in[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_IN); // Polling CARD_IN
        }
    }
#endif

#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U32              u32Events;
    MS_U32              i;

#if (SC_DEV_NUM>1)
        static MS_S32 err[2] = {-1,-1};
        static MS_S32 err_in[2] = {-1,-1};
#else
        static MS_S32 err[1] = {-1};
        static MS_S32 err_in[1] = {-1};
#endif


    u32Events = OS_SC_WaitEvent();
    if (u32Events & OS_SC_EVENT_MASK)
    {
        OS_SC_ClearEvent(OS_SC_EVENT_MASK);
        _SC_Task_Proc(0, u32Events);
        err[0] = -1;
        err_in[0] = -1;
        // @TODO: Enabel IRQ at ISR or TASK?
        MsOS_EnableInterrupt(SC_IRQ);
    }
#if (SC_DEV_NUM > 1) // no more than 2
    if (u32Events & OS_SC_EVENT_MASK2)
    {
        OS_SC_ClearEvent(OS_SC_EVENT_MASK2);
        _SC_Task_Proc(1, u32Events);
        err[1] = -1;
        err_in[1] = -1;
        MsOS_EnableInterrupt(SC_IRQ2);
    }
#endif
    OS_SC_DELAY(50);
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (_scInfo[i].bCardIn && !(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
        {
            OS_SC_DELAY(10);
            if (_scInfo[i].bCardIn && !(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
                ;
            else if ((SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
            {
                err[i] = -1;
                return E_SC_OK;
            }
            err[i] ++;
        }

        if (err[i] > 0) // confirm jump into the hw bug state
        {
            _scInfo[i].bCardIn = FALSE;
            _scInfo[i].bLastCardIn = _scInfo[i].bCardIn;
            if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                SC_OR(i,UART_SCFR, (UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            }
            else if (_scInfo[i].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                SC_AND(i,UART_SCCR, ~UART_SCCR_RST);//reset active low
                OS_SC_DELAY(3);// Wait Card stable
                if (i == 0)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA0_DIS)); // clock disable
                }
#if (SC_DEV_NUM > 1) // no more than 2
                if (i == 1)
                {
                    HW_WRITE(REG_TOP_CKG_SM_CA, (HW_READ(REG_TOP_CKG_SM_CA) | TOP_CKG_SM_CA1_DIS)); // clock disable
                }
#endif
                OS_SC_DELAY(3);// Wait Card stable
                SC_OR(i,UART_LCR, UART_LCR_SBC); //I/O
                OS_SC_DELAY(3);// Wait Card stable
                if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_AND(i,UART_SCFR, ~UART_SCFR_V_HIGH);// 'b0
                }
                else if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_OR(i,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
                }
                else //OCP
                {
                    //if (_scInfo[u8SCID].bCardIn == TRUE)
                    _SC_SetOCP(i, FALSE);
                }
            }

            err[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_OUT); // Polling CARD_OUT
        }
    }
#if 1
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (!_scInfo[i].bCardIn && (SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
        {
            OS_SC_DELAY(10);
            if (!_scInfo[i].bCardIn && (SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
                ;
            else if (!(SC_READ(i, UART_SCSR) & UART_SCSR_DETECT))
            {
                err[i] = -1;
                return E_SC_OK;
            }
            err_in[i] ++;
        }
        if (err_in[i] > 0) // confirm jump into the hw bug state
        {
            _scInfo[i].bCardIn = TRUE;
            _scInfo[i].bLastCardIn = _scInfo[i].bCardIn;
            if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
            {
                SC_AND(i, UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
                SC_AND(i ,UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
            }
            else if (_scInfo[i].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
            {
            }
            else
            {
                OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
                if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_HIGH)
                {
                    SC_OR(i,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
                }
                else if (_scInfo[i].eVccCtrl == E_SC_VCC_CTRL_LOW)
                {
                    SC_AND(i,UART_SCFR, ~UART_SCFR_V_HIGH);
                }
                else //OCP
                {
                    _SC_SetOCP(i, TRUE);
                }
                OS_SC_DELAY(1);// Wait VCC stable

                SC_OR(i, UART_SCSR, UART_SCSR_CLK);
            }
            if (i == 0)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
            }
#if (SC_DEV_NUM > 1) // no more than 2
            if (i == 1)
            {
                HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
            }
#endif

            err_in[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_IN); // Polling CARD_IN
        }
    }
#endif

#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)

    return E_SC_OK;
}

#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
MS_BOOL MDrv_SC_ISR_Proc(MS_U8 u8SCID)
{
    MS_U8               u8Reg;
#ifndef SC_USE_IO_51
    MS_U32  i=0;
#endif
    u8Reg = SC_READ(u8SCID,UART_IIR);

    // to clear interrupt here is to avoid the situation of vcc and io low.
    if (SC_READ(u8SCID, UART_SCSR)&(UART_SCSR_INT_CARDIN | UART_SCSR_INT_CARDOUT))
    {
        //SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID,UART_SCSR)); // clear interrupt

        goto NOTIFY;
    }
#ifndef SC_USE_IO_51
    if (u8Reg & UART_IIR_THRI)
    {
        while(1)
        {
            if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
                break;
            SC_WRITE(u8SCID,UART_TX, _scInfo[u8SCID].u8FifoTx[_scInfo[u8SCID].u16FifoTxRead++]);
            i++;
            if (_scInfo[u8SCID].u16FifoTxRead == SC_FIFO_SIZE)
            {
                _scInfo[u8SCID].u16FifoTxRead = 0;
            }
            else if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
            {
                break;
            }
            else
            {
                if (i >= 8)
                    break;
            }
        }
    }
    else
    {
        // Receive all data in fifo
        while (1)
        {
            if (SC_READ(u8SCID,UART_LSR) & UART_LSR_DR)
            {
                MS_S32      tmp;

                _scInfo[u8SCID].u8FifoRx[_scInfo[u8SCID].u16FifoRxWrite] = SC_READ(u8SCID,UART_RX);
                tmp = _scInfo[u8SCID].u16FifoRxWrite + 1;
                if ((tmp == SC_FIFO_SIZE) && (_scInfo[u8SCID].u16FifoRxRead != 0))
                {
                    // Not overflow but wrap
                    _scInfo[u8SCID].u16FifoRxWrite = 0;
                }
                else if (tmp != _scInfo[u8SCID].u16FifoRxRead)
                {
                    // Not overflow
                    _scInfo[u8SCID].u16FifoRxWrite = tmp;
                }
                else
                {
                    // overflow
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
#endif
    // Check special event from SMART
    if (SC_READ(u8SCID, UART_SCSR) & (UART_SCSR_INT_CARDIN | UART_SCSR_INT_CARDOUT))
    {
NOTIFY:
        // Notify ISR event
        if (u8SCID == 0)
        {
            OS_SC_SetEvent(OS_SC_EVENT_CARD); //support sm1
            return FALSE; // not handled
        }
#if (SC_DEV_NUM > 1) // no more than 2
        else if (u8SCID == 1)
        {
            OS_SC_SetEvent(OS_SC_EVENT_CARD2); //support sm2
            return FALSE; // not handled
        }
#endif
    }

    return TRUE; // handled
}
#endif
//-------------------------------------------------------------------------------------------------
/// Smartcard driver initialization
/// @param u8SCID                   \b IN: Uart device pad
/// @return @ref SC_Result
/// @note
/// Smartcard driver is in @ref E_TASK_PRI_SYS level
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Init(MS_U8 u8SCID)
{
    MS_VIRT u32BaseAddr, u32Addr;
    MS_PHY  u32BaseSize;
	MS_U32  u32BufSize, u32ShmId;
    MS_U32  bInitialized;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (u8SCID >= SC_DEV_NUM)
    {
        return E_SC_PARMERROR;
    }

    _scInfo[u8SCID].bInited = FALSE;

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_SC))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_MMIO_GetBASE fail\n");
        return E_SC_FAIL;
    }
    HAL_SC_RegMap(u32BaseAddr);
#if (SC_DEV_NUM > 1) // no more than 2
    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_SC1))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_MMIO_GetBASE fail\n");
        return E_SC_FAIL;
    }
    HAL_SC1_RegMap(u32BaseAddr);
#endif

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_MMIO_GetBASE fail\n");
        return E_SC_FAIL;
    }
    HAL_SC_HW_RegMap(u32BaseAddr);
    gbInitCheckDelay = TRUE;

    // Initialize OS SC device --------------------------------------------------------------------
    if (OS_SC_Init(u8SCID) == FALSE)
    {
        gbInitCheckDelay = FALSE;
        return E_SC_FAIL;
    }
    OS_SC_SHM_LOCK();
    // return 0 for first initiaization, 1 for repeated initialization (existence shm), so bInitialized==0 is first initial, bInitialized==1 means this shm created before
    /*
        *  u32QueryRet: 0 for not found
        *                       1 for existence
        * u32ShmId:      0 for not found
        *                       n for created share mem id, start from 1
    */
    bInitialized = MsOS_SHM_GetId((MS_U8*)"smc_proc_sync", sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);

    if (!bInitialized)
    {
        if(FALSE == MsOS_SHM_GetId((MS_U8*)"smc_proc_sync", sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            ULOGE(TAG_SC, "smc create shm failed\n");
            gbInitCheckDelay = FALSE;
            OS_SC_SHM_UNLOCK();
            return E_SC_FAIL;
        }
        *(MS_VIRT *) u32Addr = SC_PROC_LOCK;
    }
    else
    {
        if (*(MS_VIRT *) u32Addr == SC_PROC_LOCK)
        {
            ULOGE(TAG_SC, "Another process occupy the smc resource, so exit here\n");
            gbInitCheckDelay = FALSE;
            OS_SC_SHM_UNLOCK();
            return E_SC_FAIL;
        }
        else
        {
            *(MS_VIRT *) u32Addr = SC_PROC_LOCK;
        }
    }

    OS_SC_SHM_UNLOCK();

    // Open related device
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (u8SCID == 0)
    {
        if (0 > (_scInfo[u8SCID].s32DevFd = open(SM0_DEV_NAME, O_RDWR)))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] %s open fail\n", __FUNCTION__, __LINE__, "/dev/smart");
            gbInitCheckDelay = FALSE;
            return E_SC_FAIL;
        }
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        if (0 > (_scInfo[u8SCID].s32DevFd = open(SM1_DEV_NAME, O_RDWR)))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] %s open fail\n", __FUNCTION__, __LINE__, "/dev/smart1");
            gbInitCheckDelay = FALSE;
            return E_SC_FAIL;
        }
    }
#endif
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)

    // Driver local variable initialization
    _scInfo[u8SCID].bOpened = FALSE;

    HAL_SC_Init(u8SCID);

    _scInfo[u8SCID].bInited = TRUE;

    return E_SC_OK;
}

//-------------------------------------------------------------------------------------------------
/// Smartcard driver exit
/// @param u8SCID                   \b IN: Uart device pad
/// @return @ref SC_Result
/// @note
/// Smartcard driver is in @ref E_TASK_PRI_SYS level
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Exit(MS_U8 u8SCID)
{
    MS_VIRT u32Addr;
    MS_U32 u32BufSize, u32ShmId;
    MS_U32 bInitialized;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (_scInfo[u8SCID].bInited == FALSE)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "Not initial\n");
        return E_SC_FAIL;
    }

    bInitialized = MsOS_SHM_GetId((MS_U8*)"smc_proc_sync", sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);
    if (bInitialized != 0)
    {
        *(MS_VIRT *) u32Addr = SC_PROC_UNLOCK;
    }

    HAL_SC_Exit(u8SCID);

    OS_SC_Exit(u8SCID);

    _scInfo[u8SCID].bInited = FALSE;

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    close(_scInfo[u8SCID].s32DevFd);
#endif

    return E_SC_OK;
}


//-------------------------------------------------------------------------------------------------
/// Open a smartcard device
/// @param u8SCID                       \b IN: Uart device pad.
/// @param pParam                          \b IN: Open params structure.
/// @param pfSmartNotify                   \b IN: Register call back function
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Open(MS_U8 u8SCID, MS_U8 u8Protocol, SC_Param* pParam, P_SC_Callback pfSmartNotify)
{
    MS_U8   u8RegTmp;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();

    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    if (_scInfo[u8SCID].bOpened == TRUE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (pParam == NULL)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    // Open SC device
    _scInfo[u8SCID].eCardClk = pParam->eCardClk;//DEVSC_CLK_4P5M;//DEVSC_CLK_3M;
    _scInfo[u8SCID].u16ClkDiv = pParam->u16ClkDiv;//372;
    _scInfo[u8SCID].u8UartMode = pParam->u8UartMode;
    _scInfo[u8SCID].eVccCtrl = pParam->eVccCtrl;
    _scInfo[u8SCID].u16Bonding = pParam->u16Bonding;
    _scInfo[u8SCID].pfEn5V = pParam->pfOCPControl;
    _scInfo[u8SCID].u8Convention = pParam->u8Convention;
    _scInfo[u8SCID].u8Protocol = u8Protocol;

    _scInfo[u8SCID].pfNotify = pfSmartNotify;
    _scInfo[u8SCID].u8Fi = 0; // assigned by ATR
    _scInfo[u8SCID].u8Di = 0;

    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
        // Set bit2 to 0 to power on LDO. Set bit4 to 0 to enable smart card 3v/5v mode (not 3v gpio input mode)
        SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)|0x8); // auto seq control
        SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)|0x10); // all interface off
        SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)&0xEF); // all interface off


        SC_WRITE(0, UART_SCFR, SC_READ(0, UART_SCFR)|0x4); // vcc active high

        SC_WRITE(0, UART_CTRL6, 0); // power on LDO
        // need to call it, otherwise tx/rx will be failed
        SC_WRITE(0, UART_CTRL7, 0x80); // active pull up en
        //SC_WRITE(0, UART_CTRL7, SC_READ(0, UART_CTRL7)|0x80);

        // wthis code set to 5V
        SC_WRITE(0, UART_CTRL6, SC_READ(0, UART_CTRL6)|0x8); // enable 5V mode

        // disable reset for 5v mode
        //SC_OR(u8SCID,UART_SCCR, UART_SCCR_RST);//active high
    }
    HAL_SC_CardInvert(u8SCID, (SC_CardDetType) (pParam->u8CardDetInvert));

    if (_SC_SetUartDiv(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }
    if(_SC_SetUartMode(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }
    if (_SC_SetConvention(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
    {
        SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH);
        SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_ENABLE);

        OS_SC_DELAY(1); // For falling edge after CardIn detect.
    }
    else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
    }
#if SMART_OCP_EN
    else if (_scInfo[u8SCID].eVccCtrl == E_SC_OCP_VCC_HIGH)//OCP init
    {
        SC_OR(u8SCID,UART_SCMR, UART_SCMR_SMARTCARD);
    }
#endif
    else
    {
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
        {
            SC_AND(u8SCID, UART_SCFR, ~(UART_SCFR_V_ENABLE));// 'b0
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
        {
            SC_OR(u8SCID, UART_SCFR, (UART_SCFR_V_ENABLE)); // 'b1
        }

    }
    // Clear Interrupt
    u8RegTmp = (SC_READ(u8SCID,UART_SCSR) & UART_SCSR_CLK);
    SC_WRITE(u8SCID,UART_SCSR, (u8RegTmp | UART_SCSR_INT_CARDIN | UART_SCSR_INT_CARDOUT));

    // UART init ----------------------------------------------------------------------------------

    // Disable all interrupts                                           // Interrupt Enable Register
    SC_WRITE(u8SCID,UART_IER, 0x00);

    // Reset receiver and transmiter                                    // FIFO Control Register
    SC_WRITE(u8SCID,UART_FCR, UART_FCR_ENABLE_FIFO | UART_FCR_TRIGGER_1);

    SC_WRITE(u8SCID,UART_SCMR, UART_SCMR_SMARTCARD | UART_SCMR_PARITYCHK | 0x4 | UART_SCMR_FLOWCTRL); // 0x64;

    // Check Rx FIFO empty
    while ((SC_READ(u8SCID,UART_LSR) & UART_LSR_DR))
    {
        SC_READ(u8SCID,UART_RX);
        OS_SC_DELAY(1);
    }

    //SC_REG8(UART_SCMR) = UART_SCMR_SMARTCARD | 0x4;//stop bit 1.5

    //SC_WRITE(u8SCID,UART_SCCGT,10); // 10 cycles

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_ATTACH_INTERRUPT))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_INIT failed\n", __FUNCTION__, __LINE__);
    }
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    // register interrupt handler
    OS_SC_ISR_Enable(u8SCID, MDrv_SC_ISR_Proc);
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)

#ifndef SC_USE_IO_51
    // enable interrupt
    SC_WRITE(u8SCID,UART_IER, (UART_IER_RDI | UART_IER_THRI));
#endif
    _scInfo[u8SCID].bOpened = TRUE;

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR) // to do the first activation flow, without reset
    if (SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT)
    {
        _scInfo[u8SCID].bLastCardIn = TRUE;
        _scInfo[u8SCID].bCardIn = TRUE;
        //printf("card in\n");
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME * 10);// Wait Card stable
            SC_AND(u8SCID,UART_SCFR, ~(UART_SCFR_V_ENABLE|UART_SCFR_V_HIGH));
            SC_AND(u8SCID,UART_SCCR, ~UART_SCCR_RST);//Pull RSTIN low
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
            _SC_OnChip8024ActiveSeq();
        }
        else
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
            {
                SC_OR(u8SCID,UART_SCFR, UART_SCFR_V_HIGH); // 'b1
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
            {
                SC_AND(u8SCID,UART_SCFR, ~UART_SCFR_V_HIGH);
            }
            else //OCP
            {
                _SC_SetOCP(u8SCID, TRUE);
            }

            OS_SC_DELAY(1);// Wait vcc stable
        }

        if (u8SCID == 0)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA0_DIS);// enable clock
        }
#if (SC_DEV_NUM > 1) // no more than 2
        if (u8SCID == 1)
        {
            HW_WRITE(REG_TOP_CKG_SM_CA, HW_READ(REG_TOP_CKG_SM_CA) & ~TOP_CKG_SM_CA1_DIS);// enable clock
        }
#endif
         SC_AND(u8SCID,UART_LCR, ~UART_LCR_SBC); //I/O

        _SC_Callback_Notify(u8SCID, E_SC_EVENT_IN); // At the beginning CARD_IN
    }
    else
    {
        _scInfo[u8SCID].bLastCardIn = FALSE;
    }
#else
    if (SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT)
    {
        _scInfo[u8SCID].bLastCardIn = FALSE;
        _scInfo[u8SCID].bCardIn = TRUE;

        if (u8SCID == 0)
        {
            OS_SC_SetEvent(OS_SC_EVENT_CARD); //support sm1
            //return FALSE; // not handled
        }
        else if (u8SCID == 1)
        {
            OS_SC_SetEvent(OS_SC_EVENT_CARD2); //support sm2
            //return FALSE; // not handled
        }
    }
    else
    {
        _scInfo[u8SCID].bLastCardIn = FALSE;
    }
#endif
    OS_SC_RETURN(E_SC_OK)

}
//-------------------------------------------------------------------------------------------------
/// Activate a smartcard device
/// @param u8SCID                       \b IN: Uart device pad.
/// @param pParam                          \b IN: Open params structure.
/// @param pfSmartNotify                   \b IN: Register call back function
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Activate(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (_SC_Activate(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    OS_SC_RETURN(E_SC_OK);
}
//-------------------------------------------------------------------------------------------------
/// Deactivate a smartcard device
/// @param u8SCID                       \b IN: Uart device pad.
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Deactivate(MS_U8 u8SCID)
{
    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (_SC_Deactivate(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    OS_SC_RETURN(E_SC_OK);

}

//-------------------------------------------------------------------------------------------------
/// Close a smartcard device
/// @param u8SCID                   \b IN: Uart device pad.
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Close(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    // Close SC device and free system resource
    if (_SC_Close(u8SCID) != E_SC_OK)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    _scInfo[u8SCID].bOpened = FALSE;
    _scInfo[u8SCID].pfNotify = NULL;

    OS_SC_RETURN(E_SC_OK);
}

//cynthia
//-------------------------------------------------------------------------------------------------
/// Reset smartcard
/// @param u8SCID                   \b IN: Uart device pad.
/// @param pParam                          \b IN: Change params structure.
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Reset(MS_U8 u8SCID, SC_Param* pParam)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (pParam != NULL)
    {
        _scInfo[u8SCID].eCardClk = pParam->eCardClk;
        _scInfo[u8SCID].u16ClkDiv = pParam->u16ClkDiv;
        _SC_SetUartDiv(u8SCID);

        _scInfo[u8SCID].u8UartMode = pParam->u8UartMode;
        _SC_SetUartMode(u8SCID);
    }

    if (_SC_Reset(u8SCID) != E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Reset fail\n");
        OS_SC_RETURN(E_SC_FAIL);
    }
    _SC_ClearState(u8SCID);

    OS_SC_RETURN(E_SC_OK);


}

SC_Result MDrv_SC_ClearState(MS_U8 u8SCID)
{
    if (u8SCID >= SC_DEV_NUM)
    {
        return E_SC_PARMERROR;
    }

    return _SC_ClearState(u8SCID);
}

SC_Result MDrv_SC_GetATR(MS_U8 u8SCID, MS_U32 u32TimeOut,MS_U8 *pu8Atr, MS_U16 *pu16AtrLen, MS_U8 *pu8His, MS_U16 *pu16HisLen)
{
    SC_Result        eResult = E_SC_FAIL;

    OS_SC_ENTRY();

    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    eResult = _SC_GetATR(u8SCID, u32TimeOut);
    if (eResult == E_SC_CARDOUT)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    Cardout\n");
        OS_SC_RETURN(E_SC_CARDOUT);
    }
    else if (eResult == E_SC_FAIL)
    {
        _SC_Callback_Enable5V(u8SCID, TRUE);
        OS_SC_DELAY(SC_CARD_STABLE_TIME);
        OS_SC_RETURN(E_SC_FAIL);
    }

    if (eResult == E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    ATR T=%d\n", _scInfo[u8SCID].u8Protocol);

        memcpy(pu8Atr, _scInfo[u8SCID].pu8Atr, _scInfo[u8SCID].u16AtrLen);
        *pu16AtrLen = _scInfo[u8SCID].u16AtrLen;

        if (pu8His != NULL)
        {
            memcpy(pu8His, _scInfo[u8SCID].pu8Hist, _scInfo[u8SCID].u16HistLen);
        }
        if (pu16HisLen != NULL)
        {
            *pu16HisLen = _scInfo[u8SCID].u16HistLen;
        }
    }

    OS_SC_RETURN(eResult);
}

//cynthia
#if 0
//-------------------------------------------------------------------------------------------------
/// get ATR
/// @param u8SCID                   \b IN: Uart device pad.
/// @param pParam                          \b IN: Change params structure.
/// @param  pu8Atr                  \b OUT: pointer of ATR data
/// @param  pu16AtrLen              \b OUT: length of ATR data
/// @param  pu8His                \b OUT: pointer of ATR history data
/// @param  pu16HisLen          \b OUT: length of ATR history data
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------

SC_Result MDrv_SC_GetATR(MS_U8 u8SCID, SC_Param* pParam, MS_U8 *pu8Atr, MS_U16 *pu16AtrLen, MS_U8 *pu8His, MS_U16 *pu16HisLen)
{
    SC_Result        eResult = E_SC_FAIL;
    //cynthia
    _SC_ClearState(u8SCID);

    eResult = _SC_GetATR(u8SCID, SC_T0_ATR_TIMEOUT * 5 );

    if (eResult == E_SC_CARDOUT)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    Cardout\n");
        OS_SC_RETURN(E_SC_CARDOUT);
    }

    if (eResult == E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    ATR T=%d\n", _scInfo[u8SCID].u8Protocol);

        memcpy(pu8Atr, _scInfo[u8SCID].pu8Atr, _scInfo[u8SCID].u16AtrLen);
        *pu16AtrLen = _scInfo[u8SCID].u16AtrLen;

        if (pu8His != NULL)
        {
            memcpy(pu8His, _scInfo[u8SCID].pu8Hist, _scInfo[u8SCID].u16HistLen);
        }
        if (pu16HisLen != NULL)
        {
            *pu16HisLen = _scInfo[u8SCID].u16HistLen;
        }

        OS_SC_RETURN(E_SC_OK);
    }

    OS_SC_RETURN(E_SC_FAIL);
}
#endif //0


//-------------------------------------------------------------------------------------------------
/// Reset smartcard and get ATR
/// @param u8SCID                   \b IN: Uart device pad.
/// @param pParam                          \b IN: Change params structure.
/// @param  pu8Atr                  \b OUT: pointer of ATR data
/// @param  pu16AtrLen              \b OUT: length of ATR data
/// @param  pu8His                \b OUT: pointer of ATR history data
/// @param  pu16HisLen          \b OUT: length of ATR history data
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Reset_ATR(MS_U8 u8SCID, SC_Param* pParam, MS_U8 *pu8Atr, MS_U16 *pu16AtrLen, MS_U8 *pu8His, MS_U16 *pu16HisLen)
{
    SC_Result        eResult = E_SC_FAIL;
    MS_U16 u16InputAtrLen = 0;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }
    if ((pu8Atr == NULL) || (pu16AtrLen == NULL))
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    u16InputAtrLen = *pu16AtrLen;
    *pu16AtrLen = 0;

    if (pParam != NULL)
    {
        _scInfo[u8SCID].eCardClk = pParam->eCardClk;
        _scInfo[u8SCID].u16ClkDiv = pParam->u16ClkDiv;
        _SC_SetUartDiv(u8SCID);

        _scInfo[u8SCID].u8UartMode = pParam->u8UartMode;
        _SC_SetUartMode(u8SCID);
    }
    else
    {
        _scInfo[u8SCID].eCardClk = E_SC_CLK_4P5M;
        _scInfo[u8SCID].u16ClkDiv = 372;
       _SC_SetUartDiv(u8SCID);

        _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
        _SC_SetUartMode(u8SCID);

    }

    _sc_T1State[u8SCID].u8NS = 0;
    _sc_T1State[u8SCID].u8NR = 0;
    _sc_T1State[u8SCID].ubMore = 0;

    if (_SC_Reset(u8SCID) != E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Reset fail\n");
        OS_SC_RETURN(E_SC_FAIL);
    }

    _SC_ClearState(u8SCID);
    eResult = _SC_GetATR(u8SCID, SC_T0_ATR_TIMEOUT * 5);

    if (eResult == E_SC_CARDOUT)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    Cardout\n");
        OS_SC_RETURN(E_SC_CARDOUT);
    }
    else if (eResult == E_SC_FAIL)
    {
        _SC_Callback_Enable5V(u8SCID, TRUE);
        OS_SC_DELAY(SC_CARD_STABLE_TIME);
        OS_SC_RETURN(E_SC_FAIL);

    }

    if (eResult == E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    ATR T=%d\n", _scInfo[u8SCID].u8Protocol);

        // suppose all mode need to go here, check later
        // if don't call it, healthy card abnormal when on chip 8024 mode
        {
            if (_scInfo[u8SCID].pu8Atr[0] == 0x3b)
            {
                _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN;
                _SC_SetUartMode(u8SCID);
            }
            else if (_scInfo[u8SCID].pu8Atr[0] == 0x3f)
            {
                _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
                _SC_SetUartMode(u8SCID);
            }
        }

        if (_scInfo[u8SCID].bSpecMode)
        {
            _scInfo[u8SCID].u16ClkDiv = (_atr_Fi[_scInfo[u8SCID].u8Fi]/(MS_U16)_atr_Di[_scInfo[u8SCID].u8Di]);
            _SC_SetUartDiv(u8SCID);
        }
        else
        {
            eResult = _SC_PPS(u8SCID, _scInfo[u8SCID].u8Protocol, _scInfo[u8SCID].u8Di, _scInfo[u8SCID].u8Fi);
            if (eResult == E_SC_OK)
            {
                SC_DBG(E_SC_DBGLV_INFO, "    PPS OK!!\n");
                //printf("Smart Card PPS AtrFi[%d] AtrDi[%d]\n", _atr_Fi[_scInfo[u8SCID].u8Fi], _atr_Di[_scInfo[u8SCID].u8Di]);
                _scInfo[u8SCID].u16ClkDiv = (_atr_Fi[_scInfo[u8SCID].u8Fi]/(MS_U16)_atr_Di[_scInfo[u8SCID].u8Di]);
                _SC_SetUartDiv(u8SCID);
            }
            else
            {
                SC_DBG(E_SC_DBGLV_INFO, "    PPS FAIL!!\n");
                eResult = _SC_Reset(u8SCID);
                if (eResult != E_SC_OK)
                {
                    OS_SC_RETURN(E_SC_FAIL);
                }
                eResult = _SC_GetATR(u8SCID, SC_T0_ATR_TIMEOUT * 5);
                if (eResult != E_SC_OK)
                {
                    OS_SC_RETURN(E_SC_FAIL);
                }
            }
        }

        // @TODO: check maximum output buffer size.

        *pu16AtrLen = MIN(u16InputAtrLen, _scInfo[u8SCID].u16AtrLen);
        memcpy(pu8Atr, _scInfo[u8SCID].pu8Atr, *pu16AtrLen);

        if ((pu8His != NULL) && (pu16HisLen != NULL))
        {
            *pu16HisLen = MIN(*pu16HisLen, _scInfo[u8SCID].u16HistLen);
            memcpy(pu8His, _scInfo[u8SCID].pu8Hist, *pu16HisLen);
        }

        OS_SC_RETURN(E_SC_OK);
    }
    *pu16AtrLen = 0;

    OS_SC_RETURN(E_SC_FAIL);

}


SC_Result MDrv_SC_Config(MS_U8 u8SCID, SC_Param* pParam)
{
    OS_SC_ENTRY();

    if (pParam != NULL)
    {
        SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);


        _scInfo[u8SCID].eCardClk = pParam->eCardClk;
        _scInfo[u8SCID].u16ClkDiv = pParam->u16ClkDiv;
        if (_SC_SetUartDiv(u8SCID) != E_SC_OK)
        {
            OS_SC_RETURN(E_SC_FAIL);
        }
        _scInfo[u8SCID].u8UartMode = pParam->u8UartMode;
        if (_SC_SetUartMode(u8SCID) != E_SC_OK)
        {
            OS_SC_RETURN(E_SC_FAIL);
        }
   }
   else
   {
       OS_SC_RETURN(E_SC_FAIL);
   }

   OS_SC_RETURN(E_SC_OK);
}

//-------------------------------------------------------------------------------------------------
/// Send smartcard command and data
/// @param u8SCID                   \b IN: Uart device pad.
/// @param  pu8SendData             \b IN:  pointer of send data
/// @param  u16SendLen              \b IN:  length of send data
/// @param  u32TimeoutMs            \b IN:  time out
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Send(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendDataLen, MS_U32 u32TimeoutMs)
{
    SC_Result        eResult;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if ((u8SCID >= SC_DEV_NUM)||(pu8SendData == NULL))
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    eResult = _SC_Send(u8SCID, pu8SendData, u16SendDataLen, u32TimeoutMs);
    if (eResult != E_SC_OK)
    {
        OS_SC_RETURN(eResult);
    }
    OS_SC_RETURN(E_SC_OK);
}

//-------------------------------------------------------------------------------------------------
/// Recv smartcard command and data
/// @param u8SCID                   \b IN: Uart device pad.
/// @param  pu8ReadData             \b OUT: pointer of received data
/// @param  u16ReadDataLen             \b OUT: length of received data
/// @param  u32TimeoutMs             \b IN: time out
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_Recv(MS_U8 u8SCID, MS_U8 *pu8ReadData, MS_U16 *pu16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_Result eResult;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if ((u8SCID >= SC_DEV_NUM)||(pu8ReadData == NULL)||(pu8ReadData == NULL))
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    eResult = _SC_Recv(u8SCID, pu8ReadData, pu16ReadDataLen, u32TimeoutMs);
    OS_SC_RETURN(eResult);
}


//-------------------------------------------------------------------------------------------------
/// Send/Recv T=0 smartcard command and data
/// @param u8SCID                   \b IN: Uart device pad.
/// @param  pu8SendData             \b IN:  pointer of send data
/// @param  u16SendLen              \b IN:  length of send data
/// @param  pu8RecvData             \b OUT: pointer of received data
/// @param  pu16RecvLen             \b OUT: length of received data
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_T0_SendRecvold(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendLen, MS_U8 *pu8RecvData, MS_U16 *pu16RecvLen)
{
    SC_Result           eResult;
    MS_U16              u16Len;
    MS_U32              u32PollingCount = 0;
    MS_S32              pos = 0, LcLe = 0;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    _SC_ResetFIFO(u8SCID);

//[DEBUG]
/*
    {
        MS_S32          i;
        SC_DBG_(8, "    Send: ");
        for (i = 0; i < u16SendLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8SendData[i]);
            if (((i+1) % 16)==0) {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
*/
//[DEBUG]

    *pu16RecvLen = 0;

    // Send HEADER --------------------------------------------------------------------------------
    if (u16SendLen < 4)
    {
        eResult = E_SC_DATAERROR;
        goto clear_fifo; // FAIL
        //SC_RETURN(E_SC_DATAERROR);
    }
    if (((eResult = _SC_Send(u8SCID, pu8SendData+pos, 4, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        goto clear_fifo; // FAIL
        //SC_RETURN(eResult);
    }
    u16SendLen -= 4;
    pos += 4;

    // Send Lc/Le field ---------------------------------------------------------------------------
    //[TODO] support 3 bytes Lc/Le length
    if (u16SendLen)
    {
        if (((eResult = _SC_Send(u8SCID, &pu8SendData[pos], 1, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult);
        }

        LcLe = pu8SendData[pos];
        u16SendLen -= 1;
        pos += 1;
    }
    OS_SC_DELAY(SC_WAIT_DELAY);

    // Get ACK ------------------------------------------------------------------------------------
    u32PollingCount = 0;
    do  // send Lc and recv Le may cost a lot of working time.
    {
        u16Len = 1;
        if (((eResult = _SC_Recv(u8SCID, pu8RecvData, &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult); // FAIL
        }
        if (u16Len > 0 && u16Len <= 255)
        {
            break;
        }
        else
        {
            //if ((_u32PollingCount%10)==0)
                //printf("_Smart_Recv -2- [%d][%d]\n", u16Len, _u32PollingCount);
            if (u32PollingCount >= SC_T0_RECV_RETRY_COUNT)
            {
                eResult = E_SC_DATAERROR;
                goto clear_fifo; // FAIL
            }
        }
        //[TODO] NO more than smartcard "work waiting time" timeout check!!
    } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);

    if (*pu8RecvData == 0x60) // Waiting time
    {
        OS_SC_DELAY(SC_NULL_DELAY);
        u32PollingCount = 0;
        do
        {
            u16Len = 1;
            if (((eResult = _SC_Recv(u8SCID, pu8RecvData, &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
            {
                goto clear_fifo; // FAIL
                //SC_RETURN(eResult); // FAIL
            }
            if (*pu8RecvData == 0x60)
            {
                continue;
            }
            if (u16Len > 0 && u16Len <= 255)
            {
                break;
            }
            else
            {
                if (u32PollingCount >= SC_T0_RECV_RETRY_COUNT)
                {
                    eResult = E_SC_DATAERROR;
                    goto clear_fifo; // FAIL
                }
            }
            //[TODO] NO more than smartcard "work waiting time" timeout check!!
        } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
    }
    if (*pu8RecvData != pu8SendData[1]) // Check ACK
    {
        if (((*pu8RecvData & 0xF0) == 0x60) || ((*pu8RecvData & 0xF0) == 0x90))
        {
            *pu16RecvLen += 1;
            u32PollingCount = 0;
            do
            {
                u16Len = 1; // ACK
                if (((eResult = _SC_Recv(u8SCID, pu8RecvData+*pu16RecvLen, &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
                {
                    goto clear_fifo; // FAIL
                    //SC_RETURN(eResult); // FAIL
                }
                if (u16Len > 0 && u16Len <= 255)
                {
                    break;
                }
                else
                {
                    if (u32PollingCount >= SC_T0_RECV_RETRY_COUNT)
                    {
                        eResult = E_SC_DATAERROR;
                        goto clear_fifo; // FAIL
                    }
                }
                //[TODO] NO more than smartcard "work waiting time" timeout check!!
            } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
            *pu16RecvLen += 1;

            //[TODO] should I keep polling SW1/SW2?

            goto clear_fifo; // SW1, SW2, return immediately
            //SC_RETURN(eResult); // FAIL
        }
        else
        {
            SC_DBG(E_SC_DBGLV_INFO, "ACK error\n");
        }
    }

    // Send Lc Data -------------------------------------------------------------------------------
    if (u16SendLen)
    {
        if (LcLe > u16SendLen)
        {
            //SC_RETURN(E_SC_DATAERROR);
            eResult = E_SC_DATAERROR;
            goto clear_fifo;
        }
        u16Len = LcLe;
        if (((eResult = _SC_Send(u8SCID, &pu8SendData[pos], u16Len, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult); // FAIL
        }
        LcLe = 0;

        u16SendLen -= u16Len;
        pos += u16Len;

        OS_SC_DELAY(SC_WAIT_DELAY);
    }

    //[TODO] is it there any ACK or SW1SW2 between Lc and Le?

    // Send Le after Lc ---------------------------------------------------------------------------
    if (u16SendLen > 1)
    {
        SC_DBG(E_SC_DBGLV_INFO, "T=0 data exceed\n");
    }
    if (u16SendLen) // 1
    {
        if (((eResult = _SC_Send(u8SCID, &pu8SendData[pos], 1, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult);
        }
        LcLe = pu8SendData[pos];
        u16SendLen -= 1;
        pos += 1;

        OS_SC_DELAY(SC_WAIT_DELAY);
    }

    // Recv Le Data -------------------------------------------------------------------------------

    if (LcLe)
    {
        SC_DBG(E_SC_DBGLV_INFO, "Le data length %d\n", LcLe);
        u32PollingCount = 0;
        do
        {
            u16Len = 1;
            if (((eResult = _SC_Recv(u8SCID, pu8RecvData+*pu16RecvLen, &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
            {
                goto clear_fifo; // FAIL
                //SC_RETURN(eResult); // FAIL
            }
            if (u16Len > 0 && u16Len <= 255)
            {
                break;
            }
            else
            {
                if (u32PollingCount >= SC_T0_RECV_RETRY_COUNT)
                {
                    eResult = E_SC_DATAERROR;
                    goto clear_fifo; // FAIL
                }
            }
            //[TODO] NO more than smartcard "work waiting time" timeout check!!
        } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
        if (*pu8RecvData == 0x60) // Waiting time
        {
            OS_SC_DELAY(SC_NULL_DELAY);
            u16Len = LcLe;
        }
        else
        {
            *pu16RecvLen += 1;
            u16Len = LcLe - 1;
        }
        if (((eResult = _SC_Recv(u8SCID, pu8RecvData+*pu16RecvLen, &u16Len, SC_T0_RECV_TIMEOUT * 2)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult); // FAIL
        }
        *pu16RecvLen += u16Len;
        SC_DBG(E_SC_DBGLV_INFO, "reply data length %d\n", *pu16RecvLen);
    }

    u32PollingCount = 0;
    do  // send Lc and recv Le may cost a lot of working time.
    {
        u16Len = 1;
        if (((eResult = _SC_Recv(u8SCID, pu8RecvData+(*pu16RecvLen), &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult); // FAIL
        }
        if (u16Len > 0 && u16Len <= 255)
        {
            break;
        }
        else
        {
            if (u32PollingCount > SC_T0_RECV_RETRY_COUNT)
            {
                eResult = E_SC_DATAERROR;
                goto clear_fifo; // FAIL
            }
        }
        //[TODO] NO more than smartcard "work waiting time" timeout check!!
    } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);

    if (*(pu8RecvData+*pu16RecvLen) == 0x60) // Waiting time
    {
        OS_SC_DELAY(SC_NULL_DELAY);

        u16Len = 2; // SW1, SW2
    }
    else
    {
        u16Len = 1; // ACK
        *pu16RecvLen += 1; // SW2
    }
    u32PollingCount = 0;
    do  // send Lc and recv Le may cost a lot of working time.
    {
        if (((eResult = _SC_Recv(u8SCID, pu8RecvData+*pu16RecvLen, &u16Len, SC_T0_RECV_TIMEOUT)) != E_SC_OK))
        {
            goto clear_fifo; // FAIL
            //SC_RETURN(eResult); // FAIL
        }
        if (u16Len > 0 && u16Len <= 255)
        {
            break;
        }
        else
        {
            if (u32PollingCount > SC_T0_RECV_RETRY_COUNT)
            {
                eResult = E_SC_DATAERROR;
                goto clear_fifo; // FAIL
            }
        }
    } while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
    *pu16RecvLen += u16Len;

    //[TODO] Should I keep polling SW1/SW2?

    //[DEBUG]
clear_fifo:
    u32PollingCount = 0;
    {
        MS_S32 i = 0;
        do
        {
            u16Len = 1;
            if (_SC_Recv(u8SCID, pu8RecvData+*pu16RecvLen+i, &u16Len, SC_T0_RECV_TIMEOUT) != E_SC_OK)
            {
                break;
            }
            if (u16Len == 0)
            {
                break;
            }
            i = i + u16Len;
        }
        while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
        if (i)
        {
            SC_DBG(E_SC_DBGLV_INFO, "CAUTION: fifo not empty %d\n", i);
        }
    }
//[DEBUG]
/*
    if (eResult == E_SC_OK)
    {
        MS_S32          i;
        SC_DBG_(E_SC_DBGLV_REG_DUMP, "    Recv: ");
        for (i = 0; i < *pu16RecvLen; i++)
        {
            SC_DBG_(E_SC_DBGLV_REG_DUMP, "0x%02x ", pu8RecvData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG_(E_SC_DBGLV_REG_DUMP, "\n          ");
            }
        }
        SC_DBG_(E_SC_DBGLV_REG_DUMP, "\n");
    }
    else
    {
        SC_DBG_(E_SC_DBGLV_REG_DUMP, "MDrv_SC_T0_SendRecv Failed!\n");
    }
*/
//[DEBUG]
    OS_SC_RETURN(eResult);
}


#define IsTimeOut(StartTime,TimeOut)    ((MsOS_GetSystemTime()-StartTime)>TimeOut)
MS_BOOL _GetBytes(MS_U8 u8SCID, MS_U8 *pChar,MS_U16 *pu16Size,MS_U32 StartTime,MS_U32 TimeOut)
{
    MS_U16 u16Len=*pu16Size;
    MS_U16 nIndex=0;

    if (u8SCID >= SC_DEV_NUM)
        return FALSE;

    while (1)
    {
        SC_Result eResult = E_SC_FAIL;

        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        // if (!_scInfo[u8SCID].bCardIn)
        {
            ULOGE(TAG_SC, "[%s][%d] card out\n", __FUNCTION__, __LINE__);
            (*pu16Size)= nIndex;
            return FALSE;
        }

        if((*pu16Size) > 1)
        {
            eResult = _SC_Recv(u8SCID, (pChar+nIndex), pu16Size, SC_T0_RECV_TIMEOUT);
        }
        if((eResult == E_SC_OK)||(eResult == E_SC_TIMEOUT))
        {
            nIndex+=(*pu16Size);
            (*pu16Size)=(u16Len-nIndex);
        }
        else
        {
            return FALSE;
        }

        if (nIndex>=u16Len)
            break;
        if (IsTimeOut(StartTime,TimeOut))
        {
            (*pu16Size)=nIndex;
            return FALSE;
        }
    }
    (*pu16Size)=nIndex;
    return TRUE;
}

MS_BOOL _Get1Byte(MS_U8 u8SCID, MS_U8 *pChar,MS_U16 *pu16Size,MS_U32 StartTime,MS_U32 TimeOut)
{
    MS_U16 u16Len=*pu16Size;
    MS_U16 nIndex=0;

    if (u8SCID >= SC_DEV_NUM)
        return FALSE;

    while (1)
    {
        SC_Result eResult = E_SC_FAIL;

        if (!(SC_READ(u8SCID,UART_SCSR) & UART_SCSR_DETECT))
        // if (!_scInfo[u8SCID].bCardIn)
        {
            ULOGE(TAG_SC, "[%s][%d] card out\n", __FUNCTION__, __LINE__);
            (*pu16Size)= nIndex;
            return FALSE;
        }

        if((*pu16Size) == 1)
        {
            //For COVERITY CHECK.
            eResult = _SC_Recv_1_Byte(u8SCID, pChar, pu16Size, SC_T0_RECV_TIMEOUT);
        }

        if((eResult == E_SC_OK)||(eResult == E_SC_TIMEOUT))
        {
            nIndex+=(*pu16Size);
            (*pu16Size)=(u16Len-nIndex);
        }
        else
        {
            return FALSE;
        }

        if (nIndex>=u16Len)
            break;
        if (IsTimeOut(StartTime,TimeOut))
        {
            (*pu16Size)=nIndex;
            return FALSE;
        }
    }
    (*pu16Size)=nIndex;
    return TRUE;
}

typedef enum _T0_PROCEDURE_BYTE_TYPE
{
    T0_PROCEDURE_BYTE_FAIL=0,
    //T0_PROCEDURE_BYTE_NULL,//null handled by functionself
    //T0_PROCEDURE_BYTE_ACK,
    //T0_PROCEDURE_BYTE_SW1,
    //T0_PROCEDURE_BYTE_DATA,
    T0_PROCEDURE_BYTE_SUC,
}T0_PROCEDURE_BYTE_TYPE;
#define T0_WAIT_PROBYTE_TIMEOUT             4000 //if more data send, card need time to handle
#define T0_IS_SW1(ProByte)                  ((((ProByte)&0xF0)==0x60)||(((ProByte)&0xF0)==0x90))
#define T0_IS_ONEBYTE(INS,ACK)              ((((INS)^(ACK))&0xFE)==0xFE)
#define T0_IS_ALLDATA(INS,ACK)              ((((INS)^(ACK))&0xFE)==0x00)
#define T0_ACT_VPP(INS,ACK)                 ((((INS)^(ACK))==0x01)||(((INS)^(ACK))==0xFE))
#define T0_RLS_VPP(INS,ACK)                 ((((INS)^(ACK))==0x00)||(((INS)^(ACK))==0xFF))
T0_PROCEDURE_BYTE_TYPE _WaitProcedureByte(MS_U8 u8SCID,MS_U8 *pU8,MS_U8 u8Ins,MS_U32 TimeOut)
{
    MS_U16 DataSize=1;
    MS_U32 StarTime=MsOS_GetSystemTime();
    do
    {
        DataSize=1;
        if (IsTimeOut(StarTime, TimeOut) || !_Get1Byte(u8SCID, pU8, &DataSize,StarTime,TimeOut))
        {
            return T0_PROCEDURE_BYTE_FAIL;
        }
    } while(*pU8==0x60);

    if (T0_ACT_VPP(u8Ins,*pU8))
    {

    }
    else if (T0_RLS_VPP(u8Ins,*pU8))
    {


    }

    return T0_PROCEDURE_BYTE_SUC;
}

SC_Result _T0SendCAPDU(MS_U8 u8SCID, MS_U8 u8Ins,MS_U16 u16Lc, MS_U8 *pData, MS_U8 u8AckSw[2],MS_U32 *pu32RetLen)
{
    SC_Result           eResult;
    MS_U8               u8Temp1 = 0;
    MS_U16              u16Len = 0;
    MS_U16              pos = 0;

    //SC_DBG_(8, "%s,%d: u16Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
    (*pu32RetLen)=0;
    pos=0;
    while (u16Lc)
    {
        // Get ACK ------------------------------------------------------------------------------------
        if (_WaitProcedureByte(u8SCID, &u8Temp1,u8Ins,T0_WAIT_PROBYTE_TIMEOUT)==T0_PROCEDURE_BYTE_FAIL)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
            eResult = E_SC_FAIL;
            return eResult;
        }
        if (T0_IS_SW1(u8Temp1))
        {
            u8AckSw[(*pu32RetLen)++]=u8Temp1;
            u16Len=1;
            if (!_Get1Byte(u8SCID, &u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
            {
                eResult = E_SC_FAIL;
                SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
                return eResult;
            }
            u8AckSw[(*pu32RetLen)++]=u8Temp1;
            //SC_DBG_(8, "%s,%d: get pro byte2=%x\n",__FUNCTION__,__LINE__,u8Temp1);
            eResult = E_SC_OK;
            return eResult;
        }
        else if (T0_IS_ONEBYTE(u8Ins,u8Temp1))
        {
            u16Len=1;
            if (((eResult = _SC_Send(u8SCID, &pData[pos], u16Len, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Send Data error, Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
                return eResult;
            }
            u16Lc -= 1;
            pos+=1;
        }
        else if (T0_IS_ALLDATA(u8Ins,u8Temp1))
        {
            while (u16Lc)
            {
                u16Len=u16Lc;
                if (((eResult = _SC_Send(u8SCID, pData+pos, u16Len, (u16Len>120)?(SC_T0_SEND_TIMEOUT*2):(SC_T0_SEND_TIMEOUT))) != E_SC_OK))
                {
                    // eResult = E_SC_FAIL;
                    SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Send data fail, Lc=%d\n",__FUNCTION__,__LINE__,u16Lc);
                    return eResult;
                }
                u16Lc-=u16Len;
                pos+=u16Len;
            }
        }
        else
        {
            eResult = E_SC_FAIL;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Error Ack=%x pos=%d, ins=%x,\n",__FUNCTION__,__LINE__,u8Temp1,pos,u8Ins);
            return eResult;
        }
    }
    eResult = E_SC_OK;

    return eResult;
}

SC_Result _T0RcvRAPDU(MS_U8 u8SCID, MS_U8 u8Ins,MS_U16 u16Le, MS_U8 *pData,MS_U32 *pu32RetLen)
{
    SC_Result           eResult;
    MS_U8               u8Temp1 = 0;
    MS_U16              u16Len = 0;

    (*pu32RetLen)=0;
    while(1)
    {
        // Get ACK ------------------------------------------------------------------------------------
        if (_WaitProcedureByte(u8SCID, &u8Temp1,u8Ins,T0_WAIT_PROBYTE_TIMEOUT)==T0_PROCEDURE_BYTE_FAIL)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wait Ack fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
            eResult = E_SC_FAIL;
             return eResult;
        }
        if (T0_IS_SW1(u8Temp1))
        {
            pData[(*pu32RetLen)++]=u8Temp1;
            u16Len=1;
            if (!_Get1Byte(u8SCID, &u8Temp1, &u16Len, MsOS_GetSystemTime(), SC_T0_RECV_TIMEOUT))
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
                eResult = E_SC_FAIL;
                return eResult;
            }
            //SC_DBG_(8, "%s,%d: get pro byte2=%x\n",__FUNCTION__,__LINE__,u8Temp1);
            pData[(*pu32RetLen)++]=u8Temp1;
            eResult = E_SC_OK;
            return eResult;
        }
        else if (T0_IS_ONEBYTE(u8Ins,u8Temp1))
        {
            if (u16Le==0)
            {
                pData[(*pu32RetLen)++]=u8Temp1;
                eResult = E_SC_FAIL;
                return eResult;
            }
            u16Len=1;
            if (!_Get1Byte(u8SCID, &u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Get data fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
                eResult = E_SC_FAIL;
                return eResult;
            }
            pData[(*pu32RetLen)++]=u8Temp1;
            u16Le-=1;
        }
        else if (T0_IS_ALLDATA(u8Ins,u8Temp1))
        {
            if (u16Le==0)
            {
                pData[(*pu32RetLen)++]=u8Temp1;
                eResult = E_SC_FAIL;
                return eResult;
            }
            while (u16Le)
            {
                u16Len=u16Le;
                if(u16Len == 1)
                {
                    if (!_Get1Byte(u8SCID,&pData[(*pu32RetLen)], &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
                    {
                        SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wait Ack fail\n",__FUNCTION__,__LINE__);
                        eResult = E_SC_FAIL;
                        return eResult;
                    }
                }
                else
                {
                    if (!_GetBytes(u8SCID,&pData[(*pu32RetLen)], &u16Len,MsOS_GetSystemTime(),(u16Len>120)?(u16Len*5):(SC_T0_RECV_TIMEOUT)))
                    {
                        SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Get Data fail,Le=%d\n",__FUNCTION__,__LINE__,u16Le);
                        (*pu32RetLen)+=u16Len;
                        u16Le-=u16Len;
                        eResult = E_SC_FAIL;
                        return eResult;
                    }
                }
                (*pu32RetLen)+=u16Len;
                u16Le-=u16Len;
            }
        }
        else
        {
            eResult = E_SC_FAIL;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Error Ack\n",__FUNCTION__,__LINE__);
            return eResult;
        }
    }
    eResult = E_SC_FAIL;
    SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Should never here\n",__FUNCTION__,__LINE__);
    return eResult;
}

void _ClearFifo(MS_U8 u8SCID)
{
    MS_U32              u32PollingCount;
    MS_U16              u16Len;
    MS_U8               u8Temp1;
    u32PollingCount = 0;
    {
        MS_S32 i = 0;
        do
        {
            u16Len = 1;
            if (_SC_Recv(u8SCID, &u8Temp1, &u16Len, 5) != E_SC_OK)
            {
                break;
            }
            if (u16Len == 0)
            {
                break;
            }
            i = i + u16Len;
        }
        while (u32PollingCount++ < SC_T0_RECV_RETRY_COUNT);
        if (i)
        {
            SC_DBG(E_SC_DBGLV_INFO, "CAUTION: fifo not empty %d\n", i);
        }
    }
}
SC_Result MDrv_SC_T0_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendLen, MS_U8 *pu8RecvData, MS_U16 *pu16RecvLen)
{
    SC_Result           eResult;
    MS_U32              u32RetLen;
    MS_U16               u16Lc,u16Le;
    MS_U8 u8SendTmpBuf[5]={0};

    SC_DBG(E_SC_DBGLV_INFO, "enter %s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        eResult = E_SC_FAIL;
        goto clear_fifo; // FAIL
    }

//[DEBUG]
/*
    {
        MS_S32          i;
        SC_DBG_(8, "    Send: ");
        for (i = 0; i < u16SendLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8SendData[i]);
            if (((i+1) % 16)==0) {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
*/
//[DEBUG]
    _ClearFifo(u8SCID);
    *pu16RecvLen = 0;

    if (u16SendLen==4)
    {
        u16Lc=0;
        u16Le=0;
    }
    else if (u16SendLen==5)
    {
        u16Lc=0;
        u16Le=pu8SendData[4];
        if(u16Le==0)
            u16Le=256;
    }
    else if ((u16SendLen>5)&&(u16SendLen==(5+pu8SendData[4])))
    {
        u16Lc=pu8SendData[4];
        u16Le=0;
    }
    else if ((u16SendLen>5)&&(u16SendLen==(5+pu8SendData[4]+1)))
    {
        u16Lc=pu8SendData[4];
        u16Le=pu8SendData[5+u16Lc];
        if (u16Le==0)
            u16Le=256;
    }
    else if ((u16SendLen>5)&&(u16SendLen !=(5+pu8SendData[4]+1)))
    {
        // this is a patch  ,tongfang CA  has one command not compare with the t0 protocol
        SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wrong Data Len please attention the tongfang CA will come in\n",__FUNCTION__,__LINE__);
        u16Lc=pu8SendData[4];
      //  u16Le=pu8SendData[5+u16Lc];
       // if(u16Le==0)
        u16Le=256;
    }
    else
    {
        eResult = E_SC_DATAERROR;
        SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Wrong Data Len\n",__FUNCTION__,__LINE__);
        goto clear_fifo;
    }
    //send Lc
    memcpy(u8SendTmpBuf,pu8SendData,4);
    if (u16Lc)
    {
        u8SendTmpBuf[4]=(MS_U8)u16Lc;
    }
    else if (u16Le)
    {
        u8SendTmpBuf[4]=(MS_U8)u16Le;
    }
    if (((eResult = _SC_Send(u8SCID, u8SendTmpBuf, 5, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        goto clear_fifo; // FAIL
    }
    OS_SC_DELAY(SC_WAIT_DELAY);
    if (u16Lc)
    {
        if (E_SC_OK!= (eResult = _T0SendCAPDU(u8SCID, pu8SendData[1],u16Lc, &pu8SendData[5], pu8RecvData,&u32RetLen)))
        {
            *pu16RecvLen=(MS_U16)u32RetLen;
            // eResult = E_SC_FAIL;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Send CAPDU fail\n",__FUNCTION__,__LINE__);
            goto clear_fifo;
        }
        *pu16RecvLen=(MS_U16)u32RetLen;
        //SC_DBG_(8, "%s,%d: acksw len=%lu\n",__FUNCTION__,__LINE__,u32RetLen);
        if ((*pu16RecvLen)==2)
        {
            eResult = E_SC_OK;
            goto clear_fifo;
        }
        else if ((*pu16RecvLen))
        {
            eResult = E_SC_FAIL;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Send CAPDU fail\n",__FUNCTION__,__LINE__);
            goto clear_fifo;
        }
    }
    if (E_SC_OK!= (eResult = _T0RcvRAPDU(u8SCID, pu8SendData[1],u16Le,pu8RecvData,&u32RetLen)))
    {
        *pu16RecvLen=(MS_U16)u32RetLen;
#if 0
        if(((*pu16RecvLen)>=2)&&(T0_IS_SW1(pu8RecvData[u32RetLen-2])))//we thought Le is the max data to rcv
        {
            eResult = E_SC_OK;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Rcv CRPDU not enough data suc\n",__FUNCTION__,__LINE__);
        }
        else if(u32RetLen==u16Le)
        {
            pu8RecvData[u32RetLen++]=0x90;
            pu8RecvData[u32RetLen++]=0x00;
            *pu16RecvLen=(MS_U16)u32RetLen;
            eResult = E_SC_OK;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Rcv CRPDU sw fail, add\n",__FUNCTION__,__LINE__);
        }
        else
#endif
        {
            //  eResult = E_SC_FAIL;
            SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Rcv CRPDU fail\n",__FUNCTION__,__LINE__);
        }
        goto clear_fifo;
    }
    else
    {
        *pu16RecvLen=(MS_U16)u32RetLen;
        eResult = E_SC_OK;
    }
    //[DEBUG]
clear_fifo:
    _ClearFifo(u8SCID);
//[DEBUG]
/*
    if(eResult == E_SC_OK)
    {
        SC_DBG_(8, "Leo-MDrv_SC_T0_SendRecv Suc!\n");
    }
    else
    {
        SC_DBG_(8, "Leo-MDrv_SC_T0_SendRecv Failed!\n");
    }
    {
        MS_S32          i;
        SC_DBG_(8, "    Recv: ");
        for (i = 0; i < *pu16RecvLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8RecvData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
#if 0
    if (eResult == E_SC_OK)
    {
        MS_S32          i;
        SC_DBG_(8, "    Recv: ");
        for (i = 0; i < *pu16RecvLen; i++)
        {
            SC_DBG_(8, "0x%02x ", pu8RecvData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG_(8, "\n          ");
            }
        }
        SC_DBG_(8, "\n");
    }
    else
    {
        SC_DBG_(8, "MDrv_SC_T0_SendRecv Failed!\n");
    }
#endif
*/
//[DEBUG]



   OS_SC_RETURN(eResult);
}

SC_Result MDrv_SC_T1_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen)
{
    //SC_T1_INFO  eSendInfo;               //SC_T1_INFO eRecvInfo;
    SC_Result ret_Result = E_SC_FAIL;
    MS_U8   _u8HandleErrcount = 0;
    MS_U16  u16SentIdx = 3;
    MS_U16  u16BlockAPDULen = 0;
    MS_U8   u8Block[255];
    MS_U8   u8RspBlock[255];
    MS_U16  u8BlockLen = 0;
    MS_U16  u8RspRBlkLen = *u16ReadDataLen;
    MS_U8   u8EDCByteNum = 0;

    _sc_T1State[u8SCID].u8NAD = pu8SendData[0];
    memset(u8Block,0,255);
    memset(u8RspBlock,0,255);

    *u16ReadDataLen = 0;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (!_scInfo[u8SCID].bCardIn)
    {
        OS_SC_RETURN(E_SC_CARDOUT);
    }

    //info to get the first block(I-Block,S-Block)
    if (*u16SendDataLen <=  _sc_T1State[u8SCID].u8IFSC)
    {
        _sc_T1State[u8SCID].ubMore = FALSE;
        u16BlockAPDULen = *u16SendDataLen - 4;
    }
    else
    {
        _sc_T1State[u8SCID].ubMore = TRUE;
        u16BlockAPDULen = _sc_T1State[u8SCID].u8IFSC;
    }
    u16SentIdx = 3;

    //I-Block
    if ((pu8SendData[1]&0x80) == 0x00 )
    {
        _sc_T1State[u8SCID].u8NS = pu8SendData[1]&0x40;
        if (_SC_T1IBlock( &_sc_T1State[u8SCID], _sc_T1State[u8SCID].ubMore,pu8SendData+u16SentIdx, u16BlockAPDULen,u8Block, &u8BlockLen ) == E_SC_PARMERROR)
            OS_SC_RETURN(E_SC_PARMERROR);
    }
    //S-Block
    else if (pu8SendData[1]>>6 == 0x3 )
    {
        u8BlockLen = *u16SendDataLen;
        memcpy(u8Block,pu8SendData,u8BlockLen);
    }

    //set temp EDC byte number
    if (_sc_T1State[u8SCID].u8RCType & 0x1)
    {
        u8EDCByteNum = 2;   //CRC
    }
    else
    {
        u8EDCByteNum = 1;       //LRC
    }

    while (u16SentIdx <  *u16SendDataLen-u8EDCByteNum)
    {
        if (_scInfo[u8SCID].bCardIn == FALSE)
        {
            break;
        }

        //send Block
        ret_Result = _SC_T1SendRecvBlock(u8SCID,u8Block, &u8BlockLen,u8RspBlock, &u8RspRBlkLen);
        if (ret_Result != E_SC_OK)
        {
            if (_u8HandleErrcount < 3)
            {
                _SC_T1RBlock( &_sc_T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                _u8HandleErrcount++;
                continue;
            }
            else
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s Send block fail", __FUNCTION__);
                OS_SC_RETURN(E_SC_FAIL);
            }
        }

        //receive R-block
        if ((u8RspBlock[1]&0xC0) == 0x80)
        {
            if (_u8HandleErrcount > 3)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s Receive block fail", __FUNCTION__);
                OS_SC_RETURN(E_SC_FAIL);
            }
            //resend I-block
            if (_SC_T1GetBlockN(u8RspBlock) == _sc_T1State[u8SCID].u8NS)
            {
                u16SentIdx -= u16BlockAPDULen;
            }
            else
            {
                 _sc_T1State[u8SCID].u8NS ^= 1;
                 u16SentIdx += u16BlockAPDULen;
            }
            //build the next I-Block
            u16BlockAPDULen = MIN(_sc_T1State[u8SCID].u8IFSC,*u16SendDataLen-u16SentIdx-u8EDCByteNum);

            // send the remain data
            if (u16BlockAPDULen  != 0)
            {
                //last apdu data
                if (*u16SendDataLen-u16SentIdx-u8EDCByteNum == u16BlockAPDULen)
                {
                    _sc_T1State[u8SCID].ubMore = FALSE;
                }
                if (_SC_T1IBlock( &_sc_T1State[u8SCID],_sc_T1State[u8SCID].ubMore,pu8SendData+u16SentIdx,u16BlockAPDULen,u8Block, &u8BlockLen ) == E_SC_PARMERROR)
                    OS_SC_RETURN(E_SC_PARMERROR);
            }
        }
         // receive I-Block   todo : receive I-Block(complete?) when send I-Block
        else if ((u8RspBlock[1]&0x80) == 0x00)
        {
            //receive invalid block
            if (_SC_T1GetBlockN(u8RspBlock) != _sc_T1State[u8SCID].u8NR)
            {
                _SC_T1RBlock( &_sc_T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                continue;
            }
            //copy receive data
            memcpy( pu8ReadData+*u16ReadDataLen+3, u8RspBlock+3, u8RspBlock[2] );
            *u16ReadDataLen += (MS_U16)(u8RspBlock[2]);
            _sc_T1State[u8SCID].u8NR ^= 1;

            //send R-Block
            if ((u8RspBlock[1]>>5) & 1)
            {
                _SC_T1RBlock( &_sc_T1State[u8SCID], SC_T1_R_OK, u8Block, &u8BlockLen);
                continue;
            }
            u16SentIdx += u16BlockAPDULen;              //to check

        }
         // receive S-Block
        else if (u8RspBlock[1]>>6 == 0x3)
        {
            //IFS request
            if (u8RspBlock[1]==0xC1)
            {
                _sc_T1State[u8SCID].u8IFSC = u8RspBlock[3];
                if (_SC_T1SBlock( &_sc_T1State[u8SCID], SC_T1_S_IFS, SC_T1_S_RESPONSE,u8RspBlock[3], u8Block, &u8BlockLen ) != E_SC_OK)
                {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s build IFS s-block fail", __FUNCTION__);
                    OS_SC_RETURN(E_SC_FAIL);
                }
                else
                {
                    continue;
                }
            }
            //ABORT request
            else if (u8RspBlock[1]==0xC2)
            {
               if (_SC_T1SBlock( &_sc_T1State[u8SCID], SC_T1_S_ABORT, SC_T1_S_RESPONSE,0x00, u8Block, &u8BlockLen ) != E_SC_OK)
               {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s build ABORT s-block fail", __FUNCTION__);
                    OS_SC_RETURN(E_SC_FAIL);
               }
               else
               {
                    continue;
               }
            }
            //S-Block WTX Reques
            else if (u8RspBlock[1]==0xC3)
            {
                MsOS_DelayTask(u8RspBlock[3]);          // todo check
                if (_SC_T1SBlock( &_sc_T1State[u8SCID], SC_T1_S_WTX, SC_T1_S_RESPONSE, u8RspBlock[3],u8Block, &u8BlockLen )!=E_SC_OK)
                {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "%s build WTX S-Block fail", __FUNCTION__);
                    OS_SC_RETURN(E_SC_FAIL);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                memcpy( pu8ReadData+*u16ReadDataLen+3, u8RspBlock+3, u8RspBlock[2] );
                *u16ReadDataLen += (MS_U16)(u8RspBlock[2]);
                u16SentIdx += u16BlockAPDULen;
            }

        }
        else
        {
            // error handling
            if (_u8HandleErrcount < 3)
            {
                _SC_T1RBlock( &_sc_T1State[u8SCID], SC_T1_R_OTHER_ERROR, u8Block, &u8BlockLen);
                _u8HandleErrcount++;
                continue;
            }
            else
            {
                OS_SC_RETURN(E_SC_FAIL);
            }
        }
    }

    //NAD , PCB , LEN , EDC of TPDU
    pu8ReadData[0] = u8RspBlock[0]  ;   //NAD information
    pu8ReadData[1] = u8RspBlock[1];                 //PCB
    pu8ReadData[2] = (MS_U8)*u16ReadDataLen;
    *u16ReadDataLen += 3;
    _SC_T1AppendRC(&_sc_T1State[u8SCID],pu8ReadData,u16ReadDataLen);          //EDC

    OS_SC_RETURN(E_SC_OK);
}


//-------------------------------------------------------------------------------------------------
/// Send/Recv T=14 smartcard command and data
/// @param u8SCID                   \b IN: Uart device pad.
/// @param  pu8SendData             \b IN:  pointer of send data
/// @param  u16SendLen              \b IN:  length of send data
/// @param  pu8RecvData             \b OUT: pointer of received data
/// @param  pu16RecvLen             \b OUT: length of received data
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_T14_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendLen, MS_U8 *pu8RecvData, MS_U16 *pu16RecvLen)
{
    SC_Result        eResult;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }


//[DEBUG]
/*
    {
        MS_S32          i;
        SC_DBG(E_SC_DBGLV_REG_DUMP, "    Send: ");
        for (i = 0; i < u16SendLen; i++)
        {
            SC_DBG(E_SC_DBGLV_REG_DUMP, "0x%02x ", pu8SendData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG(E_SC_DBGLV_REG_DUMP, "\n          ");
            }
        }
        SC_DBG(E_SC_DBGLV_REG_DUMP, "\n");
    }
*/
//[DEBUG]

    if (((eResult = _SC_Send(u8SCID, pu8SendData, u16SendLen, SC_T14_SEND_TIMEOUT)) != E_SC_OK))
    {
        OS_SC_RETURN(eResult);
    }

#ifdef SC_USE_IO_51
    eResult = _SC_Recv(u8SCID, pu8RecvData, pu16RecvLen, SC_T14_RECV_TIMEOUT);
#else
    // reduce the time for T14 translate
    eResult = _T14_SC_Recv(u8SCID, pu8RecvData, pu16RecvLen, SC_T14_RECV_TIMEOUT);
#endif
    //[DEBUG]
    if (*pu16RecvLen < 8)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }
    if (eResult == E_SC_OK)
    {
        MS_S32          i;
        SC_DBG(E_SC_DBGLV_REG_DUMP, "    Recv: ");
        for (i = 0; i < *pu16RecvLen; i++)
        {
            SC_DBG(E_SC_DBGLV_REG_DUMP, "0x%02x ", pu8RecvData[i]);
            if (((i+1) % 16)==0)
            {
                SC_DBG(E_SC_DBGLV_REG_DUMP, "\n          ");
            }
        }
        SC_DBG(E_SC_DBGLV_REG_DUMP, "\n");
    }
    //[DEBUG]

    OS_SC_RETURN(eResult);
}



//-------------------------------------------------------------------------------------------------
/// Get smartcard information
/// @param pInfo                        \b IN:  pointer of return smartcard device information structure
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
const SC_Info* MDrv_SC_GetInfo(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    /*if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bDevOpen == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }*/

    //memcpy((void*)pInfo, (void*)&_scInfo[u8SCID], sizeof(SC_Info));

    return (const SC_Info*)&_scInfo[u8SCID];
    //OS_SC_RETURN(E_SC_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get device number of smartcard information
/// @param pCaps                        \b IN:  pointer of return smartcard device information structure
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_GetCaps(SC_Caps *pCaps)
{
    memcpy(pCaps, &_scCaps, sizeof(SC_Caps));

    return E_SC_OK;
}


//-------------------------------------------------------------------------------------------------
/// Perform PPS procedure
/// @param u8SCID                   \b IN: Uart device pad.
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_SetPPS(MS_U8 u8SCID, MS_U8 u8SCProtocol, MS_U8 u8Di,MS_U8 u8Fi)
{
    OS_SC_ENTRY();

    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }
    if (_scInfo[u8SCID].bOpened == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    _scInfo[u8SCID].u8Protocol = u8SCProtocol;
    _scInfo[u8SCID].u8Fi = u8Fi;
    _scInfo[u8SCID].u8Di = u8Di;

    OS_SC_RETURN(E_SC_OK);
}

SC_Result MDrv_SC_PPS(MS_U8 u8SCID)
{
    SC_Result           eResult = E_SC_FAIL;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if (u8SCID >= SC_DEV_NUM)
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    if ((_scInfo[u8SCID].u8Di == 0 || _scInfo[u8SCID].u8Di == 1) && (_scInfo[u8SCID].u8Fi == 0 || _scInfo[u8SCID].u8Fi == 1))
    {
        //identical to default value
        OS_SC_RETURN(E_SC_OK);
    }

    if (_scInfo[u8SCID].bSpecMode)
    {
        _scInfo[u8SCID].u16ClkDiv = (_atr_Fi[_scInfo[u8SCID].u8Fi]/(MS_U16)_atr_Di[_scInfo[u8SCID].u8Di]);
        _SC_SetUartDiv(u8SCID);
        OS_SC_RETURN(E_SC_OK);
    }

    eResult = _SC_PPS(u8SCID, _scInfo[u8SCID].u8Protocol, _scInfo[u8SCID].u8Di, _scInfo[u8SCID].u8Fi);
    if (eResult == E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_INFO, "    PPS ACK OK!!\n");
        //printf("Smart Card PPS AtrFi[%d] AtrDi[%d]\n", _atr_Fi[_scInfo[u8SCID].u8Fi], _atr_Di[_scInfo[u8SCID].u8Di]);
        _scInfo[u8SCID].u16ClkDiv = (_atr_Fi[_scInfo[u8SCID].u8Fi]/(MS_U16)_atr_Di[_scInfo[u8SCID].u8Di]);
        _SC_SetUartDiv(u8SCID);
    }
    OS_SC_RETURN(eResult);
}

//-------------------------------------------------------------------------------------------------
/// Get SC driver version
/// @param <IN>        \b const MS_U8 **pVerString :
/// @param <RET>       \b   : when get ok, return the pointer to the driver version
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_SC_FAIL;
    }

    *ppVersion = &_drv_sc_version;

    return E_SC_OK;
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SC_GetStatus
/// @brief \b Function  \b Description: Get SC status info
/// @param u8SCID                   \b IN:  Uart device pad.
/// @param  pStatus                 \b OUT: pointer of status data
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_GetStatus(MS_U8 u8SCID, SC_Status *pStatus)
{
    if (u8SCID >= SC_DEV_NUM)
    {
        return E_SC_PARMERROR;
    }

    _scStatus.bCardIn = _scInfo[u8SCID].bCardIn;
    memcpy(pStatus, &_scStatus, sizeof(SC_Status));
    return E_SC_OK;
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SC_SetDbgLevel
/// @brief \b Function  \b Description: Set debug level for debug message
/// @param eLevel       \b IN: E_SYS_DBGLV_NONE/E_SYS_DBGLV_ERR_ONLY/E_SYS_DBGLV_INFO/E_SYS_DBGLV_ALL
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetDbgLevel(SC_DbgLv eLevel)
{
    //_gu8DbgLevel = eLevel;
    _dbgmsg = eLevel;
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SC_PowerOff
/// @brief \b Function  \b Description: Turn off clock of smart card IP.
/// @param void
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_PowerOff(void)
{
    MS_VIRT  u32BaseAddr;
    MS_PHY   u32BaseSize;
    MS_S32   i;

    SC_DBG(E_SC_DBGLV_INFO, "%s : Force Power OFF [BEGIN]\n", __FUNCTION__);
    SC_DBG(E_SC_DBGLV_INFO, "%s : All MDrv_SC_ is not accessible now\n", __FUNCTION__);

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_MMIO_GetBASE fail\n");
        return E_SC_FAIL;
    }
    HAL_SC_HW_RegMap(u32BaseAddr);

    for (i = 0; i < SC_DEV_NUM; i++)
    {
        SC_DBG(E_SC_DBGLV_INFO, "SM%d power off!!!\n", i);
        HAL_SC_PowerCtrl(i, FALSE);
    }

    SC_DBG(E_SC_DBGLV_INFO, "%s : Force Power OFF [COMPLETED]\n", __FUNCTION__);

    return E_SC_OK;
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SC_GuardTime
/// @brief \b Function  \b Description: Set smart card guard time
/// @param u8SCID                   \b IN: Uart device pad.
/// @param u8GuardTime           \b IN: Guard time (unit: cycle)
/// @return @ref SC_Result
//-------------------------------------------------------------------------------------------------
SC_Result MDrv_SC_SetGuardTime(MS_U8 u8SCID, MS_U8 u8GuardTime)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    SC_WRITE(u8SCID,UART_SCCGT, u8GuardTime);

    return E_SC_OK;
}

#ifdef SC_USE_IO_51
SC_Result MDrv_SC_SetBuffAddr(SC_BuffAddr *pScBuffAddr)
{
    gpScDataBuffPA = (SC_DataCfg*) pScBuffAddr->u32DataBuffAddr;
    gpScDataBuffVA = (SC_DataCfg*) _PA2VA(pScBuffAddr->u32DataBuffAddr);
    gScFwDataBuff =  (MS_PHY) pScBuffAddr->u32FwBuffAddr;
    //Todo: Check SC firmware valid
    SC_DBG(E_SC_DBGLV_INFO, " gpScDataBuffPA = 0x%lx\n", (MS_PHY) gpScDataBuffPA);
    SC_DBG(E_SC_DBGLV_INFO, " gpScDataBuffVA = 0x%lx\n", (MS_VIRT) gpScDataBuffVA);
    SC_DBG(E_SC_DBGLV_INFO, " gScFwDataBuff  = 0x%lx\n", gScFwDataBuff);
    return E_SC_OK;
}

SC_Result MDrv_SC_RawExchange(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen)
{
    SC_Result        eResult;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if ((u8SCID >= SC_DEV_NUM)||(pu8SendData == NULL))
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

     eResult = _SC_RawExchange(u8SCID, pu8SendData, u16SendDataLen, pu8ReadData, u16ReadDataLen, 0);

    OS_SC_RETURN(eResult);
}

SC_Result MDrv_SC_RawExchangeTimeout(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_Result        eResult;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    OS_SC_ENTRY();
    if ((u8SCID >= SC_DEV_NUM)||(pu8SendData == NULL))
    {
        OS_SC_RETURN(E_SC_PARMERROR);
    }

    eResult = _SC_RawExchange(u8SCID, pu8SendData, u16SendDataLen, pu8ReadData, u16ReadDataLen, u32TimeoutMs);

    OS_SC_RETURN(eResult);
}
#else
SC_Result MDrv_SC_SetBuffAddr(SC_BuffAddr *pScBuffAddr)
{
    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] not support\n", __FUNCTION__, __LINE__);
    return E_SC_FAIL;
}

SC_Result MDrv_SC_RawExchange(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen)
{
    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] not support\n", __FUNCTION__, __LINE__);
    return E_SC_FAIL;
}

SC_Result MDrv_SC_RawExchangeTimeout(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] not support\n", __FUNCTION__, __LINE__);
    return E_SC_FAIL;
}
#endif

MS_BOOL MDrv_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage)
{
    return HAL_SC_CardVoltage_Config(u8SCID, eVoltage);
}

void MDrv_SC_EnableTimeout(MS_BOOL bTimeout)
{
#ifdef SC_USE_IO_51
    gbTimeout = bTimeout;
#endif
}

MS_U32 MDrv_SC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE u16PreSCPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        if(_scInfo[0].bInited == TRUE)
        {
            ULOGD(TAG_SC, "%s %d,EN_POWER_MODE:%d \n",__FUNCTION__,__LINE__,(MS_S32)u16PowerState);
            pParam.eCardClk  = _scInfo[0].eCardClk ;//DEVSC_CLK_4P5M;//DEVSC_CLK_3M;
            pParam.eVccCtrl = _scInfo[0].u16ClkDiv;//372;
            pParam.u8UartMode = _scInfo[0].u8UartMode;
            pParam.eVccCtrl  = _scInfo[0].eVccCtrl ;
            pParam.u16Bonding = _scInfo[0].u16Bonding;
            pParam.u8Convention = _scInfo[0].u8Convention;
            pParam.u8Protocal = _scInfo[0].u8Protocol;
            pParam.pfOCPControl = NULL;
            pfNotify = _scInfo[0].pfNotify;
            MDrv_SC_Close(0);
            MDrv_SC_Exit(0);
            u16PreSCPowerState = u16PowerState;
        }
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreSCPowerState == E_POWER_SUSPEND)
        {
            ULOGD(TAG_SC, "%s %d,EN_POWER_MODE:%d \n",__FUNCTION__,__LINE__,(MS_S32)u16PowerState);
            MDrv_SC_Init(0);
            MDrv_SC_Open(0, pParam.u8Protocal, &pParam,pfNotify);
            u16PreSCPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_SC, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_SC, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

void _SC_OnChip8024DeactiveSeq(void)
{
    SC_WRITE(0, UART_DEACTIVE_RST, 0x3);
    SC_WRITE(0, UART_DEACTIVE_CLK, 0x6);
    SC_WRITE(0, UART_DEACTIVE_IO, 0x9);
    SC_WRITE(0, UART_DEACTIVE_VCC, 0xc);
    SC_WRITE(0, UART_CTRL3, SC_READ(0, UART_CTRL3)|0x1); // ori is 1
    MsOS_DelayTask(3);
    SC_WRITE(0, UART_CTRL3, SC_READ(0, UART_CTRL3)&~0x1); // ori is 1
}

void _SC_OnChip8024ActiveSeq(void)
{
    SC_WRITE(0, UART_ACTIVE_VCC, 0x3);
    SC_WRITE(0, UART_ACTIVE_IO, 0x6);
    SC_WRITE(0, UART_ACTIVE_CLK, 0x9);
    SC_WRITE(0, UART_ACTIVE_RST, 0xc);
    SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)|0x1); // ori is 1
    MsOS_DelayTask(3);
    SC_WRITE(0, UART_CTRL5, SC_READ(0, UART_CTRL5)&~0x1); // ori is 1
}
