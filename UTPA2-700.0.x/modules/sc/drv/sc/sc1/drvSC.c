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

#include <errno.h>

#define SC_KERNEL_ISR
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
#include "mdrv_sc_io.h"
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#endif
extern MS_U32 _regSCHWBase;

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
/////////////////////
// PATCH_TO_DISABLE_CGT => In 7816 test item 3412/3416, it indicates GT shall be 13 etu but we found the rcved data from Card is still 12 etu.
//                                               So we add this definition to set CGT intterupt mask avoiding RX CGT fail interrupt triggered
//                                               And we also add switch for nagra paired CGT test
/////////////////////
#define PATCH_TO_DISABLE_CGT        1
#if PATCH_TO_DISABLE_CGT
#define DISABLE_RX_CGT_CHECK        _gbPatchToDisableRxCGTFlag = TRUE
#define ENABLE_RX_CGT_CHECK         _gbPatchToDisableRxCGTFlag = FALSE
#define IS_RX_CGT_CHECK_DISABLE     _gbPatchToDisableRxCGTFlag

/////////////////////
// PATCH_TO_DISABLE_CGT_IRDETO_T14 => The CGT is fail for irdeto T=14 in ATR. It sems ETU time has problem to cause incorrect CGT flag.
//                                                    Temp to disable CGT check for Irdeto T=14
/////////////////////
#define PATCH_TO_DISABLE_CGT_IRDETO_T14     1
#endif

/////////////////////
// PATCH_TO_DISABLE_BGT => In 7816 test item 3311/3321, we found the RX BGT is smaller than 22 etu that violates T=1 need minimum interval 22 etu requirement.
//                                               So we add this definition to set BGT intterupt mask avoiding RX BGT fail interrupt triggered
/////////////////////
#define PATCH_TO_DISABLE_BGT        1


#define SMART_OCP_EN    0

#define SC_DBG_FUNC()             if (_gu8DbgLevel >= E_SC_DBGLV_ALL) \
                                        { printf("\t====   %s   ====\n", __FUNCTION__); }
#define SC_DBG_INFO(x, args...)   if (_gu8DbgLevel >= E_SC_DBGLV_INFO ) \
                                        { printf(x, ##args); }
#define SC_DBG_ERR(x, args...)    if (_gu8DbgLevel >= E_SC_DBGLV_ERR_ONLY) \
                                        { printf(x, ##args); }

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
#define MS_DEBUG 1
#ifdef MS_DEBUG
#define SC_DBG(_d, _f, _a...)       { if (_dbgmsg >= _d) printf(_f, ##_a); }
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

#define SC_MAX_CONT_SEND_LEN        (24)
MS_U16 u16ISRTXLevel=0;
MS_U16 u16ISRTXEmpty=0;

#define E_TIME_ST_NOT_INITIALIZED  0xFFFFFFFFUL

#define E_INVALID_TIMEOUT_VAL 0x00000000UL

#define RFU 0x00UL   //Reserved for future of Di and Fi

////////////////////////////
// The RST_TO_ATR_TIMEOUT is an experimental value for SW patch
////////////////////////////
#define RST_TO_ATR_TIMEOUT  44464UL //40000 closks + 12etu

#define SC_SHMSMC1 "smc1_proc_sync"
#define SC_SHMSMC2 "smc2_proc_sync"

#define SC_STAR3150_COMPENSATION 1.00002

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
    MS_U8                           u8BWI;
    MS_U8                           u8CWI;
    MS_U32                         u32CWT; //usec
    MS_U32                         u32CGT; //usec
    MS_U32                         u32BWT; //usec
    MS_U32                         u32BGT; //usec
    MS_U8                           u8BGT_IntCnt;
    MS_U8                           u8BWT_IntCnt;
    MS_U8                           u8CWT_TxIntCnt;
    MS_U8                           u8CWT_RxIntCnt;
    MS_U8                           u8CGT_RxIntCnt;
    MS_U8                           u8CGT_TxIntCnt;
    MS_BOOL                         bParityError;
} SC_T1_State;

typedef struct
{
    MS_U8                           u8WI;                 //Waiting Integer, 0x00 is RFU
    MS_U32                          u32WT;  //usec
    MS_U32                          u32GT;  //usec
    MS_U8                     u8WT_TxIntCnt;
    MS_U8                     u8WT_RxIntCnt;
    MS_U8                     u8GT_RxIntCnt;
    MS_U8                     u8GT_TxIntCnt;
    MS_BOOL                         bParityError;
    MS_BOOL                         bUseBwtInsteadExtCwt; //Use BWT instead of CWT
} SC_T0_State;

typedef struct
{
    MS_BOOL                   bRstToAtrTimeout;
} SC_RST_TO_ATR_CTRL;

typedef struct
{
    MS_U8                     u8N; //N
    SC_VoltageCtrl            eVoltage;
} SC_Info_Ext;

typedef struct
{
    MS_BOOL                   bRstToIoEdgeDet; //Support HW RST_to_IO_edge_detect
    MS_BOOL                   bExtCwtFixed; //Support ext-CWT and comparison issue is fixed
}SC_Hw_Feature;

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

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
typedef enum
{
    E_SC_INT_TX_LEVEL = 0x00000001UL,
    E_SC_INT_CARD_IN = 0x00000002UL, //UART_SCSR_INT_CARDIN
    E_SC_INT_CARD_OUT = 0x00000004UL, //UART_SCSR_INT_CARDOUT
    E_SC_INT_CGT_TX_FAIL = 0x00000008UL,
    E_SC_INT_CGT_RX_FAIL = 0x00000010UL,
    E_SC_INT_CWT_TX_FAIL = 0x00000020UL,
    E_SC_INT_CWT_RX_FAIL = 0x00000040UL,
    E_SC_INT_BGT_FAIL = 0x00000080UL,
    E_SC_INT_BWT_FAIL = 0x00000100UL,
    E_SC_INT_PE_FAIL = 0x00000200UL,
    E_SC_INT_RST_TO_ATR_FAIL = 0x00000400UL,
    E_SC_INT_INVALID = 0xFFFFFFFFUL
}SC_INT_BIT_MAP;

typedef enum
{
    E_SC_ATTR_INVALID = 0x00000000UL,
    E_SC_ATTR_TX_LEVEL = 0x00000001UL
}SC_ATTR_TYPE;
#endif

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
static const MS_U16                 _atr_Fi[]       = { 372, 372, 558, 744,1116,1488,1860, RFU, RFU, 512, 768,1024,1536,2048, RFU, RFU };
static const MS_U16                 _atr_Di[]       = { RFU,   1,   2,   4,    8, 16,  32,   64,  12,  20, RFU, RFU, RFU, RFU, RFU, RFU };

static const SC_Caps                _scCaps = {
                                        .u8DevNum   = SC_DEV_NUM,
                                    };

static SC_Status                    _scStatus = {
                                        .bCardIn = FALSE,
                                    };

//static MS_U8                        _gu8DbgLevel        = E_SC_DBGLV_ERR_ONLY;

MS_S8                               _dbgmsg = E_SC_DBGLV_ERR_ONLY; // E_SC_DBGLV_NONE, E_SC_DBGLV_ERR_ONLY, E_SC_DBGLV_REG_DUMP, E_SC_DBGLV_INFO, E_SC_DBGLV_ALL

static SC_T0_State  _sc_T0State[SC_DEV_NUM] =
                                        {
                                            {0, E_TIME_ST_NOT_INITIALIZED, 0},
#if (SC_DEV_NUM > 1) // no more than 2
                                            {0, E_TIME_ST_NOT_INITIALIZED, 0}
#endif
                                        };


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

static SC_Info_Ext                  _scInfoExt[SC_DEV_NUM];

static MS_U8                        _pu8PPS[SC_PPS_LEN_MAX];
static MS_U16                       _u16PPSLen;

#if !(defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR))
static SC_SendData                  _scSendData;
#endif

#ifdef SC_USE_IO_51
static SC_AckFlags                  gScAckFlags;
static SC_DataCfg                  *gpScDataBuffPA = NULL;
static SC_DataCfg                  *gpScDataBuffVA = NULL;
static MS_U32                       gScFwDataBuff = 0;
static MS_BOOL                      gbTimeout = FALSE;
#endif
static SC_Param  pParam;
static P_SC_Callback    pfNotify;
static SC_RST_TO_ATR_CTRL _gastRstToAtrCtrl[SC_DEV_NUM] = {
                                                            {FALSE},
#if (SC_DEV_NUM > 1) // no more than 2
                                                            {FALSE}
#endif
                                                          };

#if PATCH_TO_DISABLE_CGT
static MS_BOOL _gbPatchToDisableRxCGTFlag = FALSE;
#endif

static SC_Hw_Feature _gstHwFeature;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Function prototype
//-------------------------------------------------------------------------------------------------
static HAL_SC_VOLTAGE_CTRL _SC_ConvHalVoltageCtrl(SC_VoltageCtrl eVoltage);
static HAL_SC_VCC_CTRL _SC_ConvHalVccCtrl(SC_VccCtrl eVccCtrl);
static HAL_SC_CLK_CTRL _SC_ConvHalClkCtrl(SC_ClkCtrl eClkCtrl);
static HAL_SC_CARD_DET_TYPE _SC_ConvHalCardDetType(SC_CardDetType eCardDetType);


#ifndef SC_USE_IO_51
static SC_Result  _SC_Calculate_BGWT(MS_U8 u8SCID, MS_U8 u8BWI);
static SC_Result  _SC_Calculate_CGWT(MS_U8 u8SCID, MS_U8 u8CWI);
static SC_Result  _SC_Calculate_GWT(MS_U8 u8SCID);
static void _SC_EnableIntCGWT(MS_U8 u8SCID);
static void _SC_DisableIntCGWT(MS_U8 u8SCID);
static void _SC_EnableIntBGWT(MS_U8 u8SCID);
static void _SC_DisableIntBGWT(MS_U8 u8SCID);
static MS_U32 _SC_ConvClkEnumToNum(SC_ClkCtrl  eCardClk);
static SC_Result _SC_SetupUartInt(MS_U8 u8SCID);
static MS_U32 _SC_GetRcvWaitingTime(MS_U8 u8SCID);
static MS_BOOL _SC_CheckRstToAtrTimeout(MS_U8 u8SCID);
static void _SC_RstToAtrTimeoutProc(MS_U8 u8SCID);
static SC_Result _SC_SetupHwFeature(MS_U32 u32HwVer);
static void _SC_RstToIoEdgeDetCtrl(MS_U8 u8SCID, MS_BOOL bEnable);
#endif
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
        //printf("_Smart_GetFifo -- DEVSC_EMPTY r[%d] w[%d]\n", _scInfo[u8SCID].u16FifoRxRead, _scInfo[u8SCID].u16FifoRxWrite);
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

    if (0 > (s16Size = read(_scInfo[u8SCID].s32DevFd, buf, count)))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] read error\n", __FUNCTION__, __LINE__);
        return SC_INT_FAIL;
    }
    return s16Size;
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U16 i;
    MS_U8 ch;

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
        printf("_Smart_PutChar Overflow\n");
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

    //To get TX FIFO status
    while (HAL_SC_IsTxFIFO_Empty(u8SCID) == FALSE)
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

    //Check uart tx fifo is empty
    if(!_SC_WaitTXFree(u8SCID,SC_WRITE_TIMEOUT))
            return -1;

    if (0 > (s32Size = write(_scInfo[u8SCID].s32DevFd, buf, count)))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] read error\n", __FUNCTION__, __LINE__);
    }
    return s32Size;
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U16              i;
    MS_U8               ch;
    MS_U16              j;
    MS_U8               sendLen=0;
    MS_U8               txdata;

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

    //Fill up SW TX FIFO
    for (i = 0; i < count; i++)
    {
        ch = buf[i];//*(MS_U8*)(buf+i);
        if (_SC_WriteFIFO(u8SCID, ch) != E_SC_OK)
        {
            return (i);
        }
    }

    //Check uart tx fifo is empty
    if(!_SC_WaitTXFree(u8SCID,SC_WRITE_TIMEOUT))
            return -1;

    // replace to use tx level int in tx pkts send
    if (count > SC_MAX_CONT_SEND_LEN)
    {
        sendLen = SC_MAX_CONT_SEND_LEN;
        }
    else
    {
        sendLen = count;
    }
    for (j=0;j<sendLen;j++)
    {
        if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
            break;
        txdata =  _scInfo[u8SCID].u8FifoTx[_scInfo[u8SCID].u16FifoTxRead++];

        HAL_SC_WriteTxData(u8SCID, txdata);
        if (_scInfo[u8SCID].u16FifoTxRead == SC_FIFO_SIZE)
        {
            _scInfo[u8SCID].u16FifoTxRead = 0;
        }
        else if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
        {
            break;
        }
    }

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
    HAL_SC_FIFO_CTRL stCtrlFIFO;

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
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_DISABLE);

    // Reset receiver and transmiter                                // FIFO Control Register
    memset(&stCtrlFIFO, 0x00, sizeof(stCtrlFIFO));
    stCtrlFIFO.bEnableFIFO = TRUE;
    stCtrlFIFO.eTriLevel = E_HAL_SC_RX_FIFO_INT_TRI_1;
    HAL_SC_SetUartFIFO(u8SCID, &stCtrlFIFO);

    // Disable all interrupts (Dummy write action. To make Rx FIFO reset really comes into effect)
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_DISABLE);

    // Check Rx FIFO empty
    while (HAL_SC_IsRxDataReady(u8SCID))
    {
        HAL_SC_ReadRxData(u8SCID);
        OS_SC_DELAY(1);
    }

    // Clear interrupt status
    HAL_SC_ClearIntTxLevelAndGWT(u8SCID);

#ifndef SC_USE_IO_51
    // enable interrupt
    _SC_SetupUartInt(u8SCID);
#endif
    // Clear Rx buffer
    _SC_ResetFIFO(u8SCID);

#ifndef SC_USE_IO_51
    //Calculate GT/WT and enable interrupt to detect GT/WT of ATR
    _SC_Calculate_GWT(u8SCID);
    _SC_EnableIntCGWT(u8SCID);
    if (_gstHwFeature.bRstToIoEdgeDet == TRUE)
        _SC_RstToIoEdgeDetCtrl(u8SCID, TRUE);
#endif

    // Smart card reset
    // need to disable reset for 5v mode when on chip 8024 mode
    // disable these UART_SCCR_RST flow when on chip 8024 mode, move these code to active sequence code when on chip 8024 mode
    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
        HAL_SC_Int8024PullResetPadLow(u8SCID, 1);
        HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_HIGH);
    }
    else
    {
        HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);
        OS_SC_DELAY(SC_RST_HOLD_TIME);
        HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_HIGH);
    }

#ifndef SC_USE_IO_51
    //////////////////////////
    // This is the SW patch
    // To process RST to I/O timeout check just after card rest
    //////////////////////////
    if (_gstHwFeature.bRstToIoEdgeDet == FALSE)
        _SC_RstToAtrTimeoutProc(u8SCID);
#endif

    return E_SC_OK;
}


// Set uart divider
static SC_Result _SC_SetUartDiv(MS_U8 u8SCID)
{
    HAL_SC_CLK_CTRL eHalClkCtrl = _SC_ConvHalClkCtrl(_scInfo[u8SCID].eCardClk);
    SC_Result eRetVal;

    if (HAL_SC_SetUartDiv(u8SCID, eHalClkCtrl, _scInfo[u8SCID].u16ClkDiv))
        eRetVal = E_SC_OK;
    else
        eRetVal = E_SC_FAIL;

    return eRetVal;
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
    MS_U32 u32SrcAddr;
    MS_U32 MBX_Resendcnt = 0;

    u32TimeoutMs = u32TimeoutMs;

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        printf("Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
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

    u32SrcAddr = (MS_U32)gpScDataBuffPA;
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
    int i;
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
    MS_U32 u32SrcAddr;

    MS_U32 MBX_Resendcnt = 0;

    u32TimeoutMs = u32TimeoutMs;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        printf("Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
        return E_SC_FAIL;
    }

    gpScDataBuffVA->u8SCID = u8SCID;
    gpScDataBuffVA->u16DataLen = *u16DataLen;
    gpScDataBuffVA->u16ReplyMaxLen = *u16DataLen;

    MsOS_FlushMemory();
    MsOS_Sync();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));

    gScAckFlags |= E_SC_ACKFLG_WAIT_RECV;

    u32SrcAddr = (MS_U32)gpScDataBuffPA;
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
    MS_U32 u32SrcAddr;

    MS_U32 MBX_Resendcnt = 0;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    if (gpScDataBuffPA == NULL || gpScDataBuffVA == NULL)
    {
        printf("Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
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

    u32SrcAddr = (MS_U32)gpScDataBuffPA;
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

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (u8SCID >= SC_DEV_NUM)
        return E_SC_PARMERROR;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    //
    // NOTE: When BWT works as ext-CWT, the RST_TO_IO_EDGE_DET_EN (0x1029_0x0A[7]) must be set
    //
    if (_sc_T0State[u8SCID].bUseBwtInsteadExtCwt)
    {
        _SC_DisableIntBGWT(u8SCID);
        HAL_SC_RstToIoEdgeDetCtrl(u8SCID, TRUE);
        _SC_EnableIntBGWT(u8SCID);
    }

    // Before data sent to card, clear character WT of previous rcv
    _sc_T0State[u8SCID].u8WT_RxIntCnt = 0;
    _sc_T1State[u8SCID].u8CWT_RxIntCnt = 0;

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

        if (u32TimeoutMs != E_INVALID_TIMEOUT_VAL)
        {
            if ((len == 0) && (OS_SYSTEM_TIME() - u32StartTime) > u32TimeoutMs)
            {
                break;
            }
        }

        OS_SC_DELAY(10);

        if (!HAL_SC_IsCardDetected(u8SCID))
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
    SC_Result eResult=E_SC_OK;
    MS_S16 i = 0;
    MS_S16 len = 0;
    MS_U32 u32SetupTimeoutCountMs = 0;
    MS_U32 u32TimeoutCountMs = 0;
    MS_U32 u32TimeDelayStepMs;
    MS_BOOL bUseInternalWT = FALSE;
    MS_U32 u32_f;
    MS_U32 u32WorkETU;
    MS_U32 u32SwWt = 0;
    MS_U32 u32SwWtStartTime = 0, u32DiffTime;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    //
    // About timeout value of SC receive, it will be applied and controled as following
    // 1) u32TimeoutMs != E_INVALID_TIMEOUT_VAL:
    //     The timeout value is defined by user, and it indicates the timeout of whole SC receiving procedure of _SC_Recv
    //
    // 2) u32TimeoutMs == E_INVALID_TIMEOUT_VAL:
    //     The timeout is NOT applied by user, and enable bit of CWT_Rx_fail_always_detect which allows HW starts to count
    //      CWT from Rx start bit and no need comparion with next rcv data to check if SC receiving procedure is timeout
    //

    // Set bUseInternalWT flag
    if (u32TimeoutMs == E_INVALID_TIMEOUT_VAL)
        bUseInternalWT = TRUE;
    else
        bUseInternalWT = FALSE;

    // Apply delay step and steup timeout value
    if (bUseInternalWT)
    {
        u32SetupTimeoutCountMs = E_TIME_ST_NOT_INITIALIZED;
        u32TimeDelayStepMs = 1;
        u32SwWt = _SC_GetRcvWaitingTime(u8SCID);

        // As Nagra stress test and irdeto T=14, the HW CWT maybe not be triggered and cause rcv hang-up,
        // so add a SW WT to work around this case of CWT flag not raised
        u32SwWt = 2*u32SwWt; //2times CWT
    }
    else
    {
        u32SetupTimeoutCountMs = u32TimeoutMs;
        u32TimeDelayStepMs = 1;
    }

    // Display info
    SC_DBG(E_SC_DBGLV_INFO, "%s: bUseInternalWT = %d\n", __FUNCTION__, bUseInternalWT);
    SC_DBG(E_SC_DBGLV_INFO, "%s: u32SetupTimeoutCountMs = %u ms\n", __FUNCTION__, u32SetupTimeoutCountMs);
    SC_DBG(E_SC_DBGLV_INFO, "%s: u32TimeDelayStepMs = %u ms\n", __FUNCTION__, u32TimeDelayStepMs);
    SC_DBG(E_SC_DBGLV_INFO, "%s: u32SwWt = %u ms\n", __FUNCTION__, u32SwWt);

    if (bUseInternalWT)
    {
        //ATR and T=0 need to check data rcv WT timeout always
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
        {
            //If using BWT instead of CWT, do not enable CWT always detect flag
            if (_sc_T0State[u8SCID].bUseBwtInsteadExtCwt == FALSE)
                HAL_SC_RxFailAlwaysDetCWT(u8SCID, TRUE);

            u32SwWtStartTime = MsOS_GetSystemTime();
        }
    }

    // Apply timeout value
    u32TimeoutCountMs = u32SetupTimeoutCountMs;

    len = _SC_Read(u8SCID, pu8Data, *u16DataLen);

    while (i < *u16DataLen)
    {
        if (bUseInternalWT)
        {
            // T=1 need to check data rcv WT timeout always
            if ((_scInfo[u8SCID].u8Protocol == 1) && (i == 0) && (len > 0))
            {
                HAL_SC_RxFailAlwaysDetCWT(u8SCID, TRUE);
                u32SwWtStartTime = MsOS_GetSystemTime();
            }
        }

        i = i + len;
        if (i == *u16DataLen)
        {
            ////////////////////////////////////////
            // SW patch for Irdeto stress test
            // Add one etu delay time to make sure the last one byte is transmitted complete IO bus is free
            ////////////////////////////////////////
            if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
            {
                u32_f = _SC_ConvClkEnumToNum( _scInfo[u8SCID].eCardClk);

                //Calculate ETU
                u32WorkETU = (_atr_Fi[_scInfo[u8SCID].u8Fi] / _atr_Di[_scInfo[u8SCID].u8Di])*1000000UL / (u32_f/1000UL); // in nsec
                u32WorkETU = (u32WorkETU/1000UL)+1; // in usec
                MsOS_DelayTaskUs(u32WorkETU); // delay 1 etu
            }
            ////////////////////////////////////////
            break;
        }

        OS_SC_DELAY(u32TimeDelayStepMs);

        if (!bUseInternalWT)
        {
            // To check CWT/WT timeout
            if ((i < *u16DataLen) && (u32TimeoutCountMs == 0))
            {
                *u16DataLen = i;
                SC_DBG(E_SC_DBGLV_INFO, "[%s][%d] ATR Timeout\n", __FUNCTION__, __LINE__);
                eResult =  E_SC_TIMEOUT;
                break;
            }

            // To decrease CWT/WT timeout value
            if ((bUseInternalWT == TRUE) && (_scInfo[u8SCID].u8Protocol == 1) && (i == 0))
            {
                //If we are at T=1 mode, then first data waiting time is handled by BWT of HW. Do nothing here...
            }
            else
            {
                if (u32TimeoutCountMs >= u32TimeDelayStepMs)
                    u32TimeoutCountMs = u32TimeoutCountMs - u32TimeDelayStepMs;
                else
                    u32TimeoutCountMs = 0;
            }
        }

        // Check if card is removed during recv
        if (!HAL_SC_IsCardDetected(u8SCID))
        {
            SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT 2\n");
            *u16DataLen = i;
            eResult =  E_SC_CARDOUT;
            break;
        }

        // T= 0
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
        {
            if (_SC_CheckRstToAtrTimeout(u8SCID))
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] The duration between RST and ATR is timeout\n", __FUNCTION__, __LINE__);
                *u16DataLen = 0;
                eResult =  E_SC_FAIL;
                break;
            }
            if (_sc_T0State[u8SCID].bParityError)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] Parity error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_DATAERROR;
                break;
            }
            //To check BWT if bUseBwtInsteadExtCwt is enabled
            if (_sc_T1State[u8SCID].u8BWT_IntCnt > 0 && _sc_T0State[u8SCID].bUseBwtInsteadExtCwt == TRUE)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] BWT error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_FAIL;
                break;
            }
            //To check CWT if bUseBwtInsteadExtCwt is disabled
            if (_sc_T0State[u8SCID].u8WT_RxIntCnt > 0 && _sc_T0State[u8SCID].bUseBwtInsteadExtCwt == FALSE)
            {
                if (bUseInternalWT)
                {
                    len = _SC_Read(u8SCID, pu8Data+i, (*u16DataLen)-i);
                    if (len > 0)
                        continue;
                }
                SC_DBG(E_SC_DBGLV_INFO, "[%s][%d] CWT RX error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_TIMEOUT;
                break;
            }
#if PATCH_TO_DISABLE_CGT //Iverlin: Patch to ignore CGT sent from card
            if (IS_RX_CGT_CHECK_DISABLE == FALSE)
            {
                if (_sc_T0State[u8SCID].u8GT_RxIntCnt > 0)
                {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] CGT RX error\n", __FUNCTION__, __LINE__);
                    *u16DataLen = i;
                    eResult =  E_SC_FAIL;
                    break;
                }
            }
#endif
        }

        // T= 1
        if (_scInfo[u8SCID].u8Protocol == 1)
        {
            if (_sc_T1State[u8SCID].bParityError)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] Parity error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_DATAERROR;
                break;
            }
            if (_sc_T1State[u8SCID].u8CWT_RxIntCnt > 0)
            {
                if (bUseInternalWT)
                {
                    len = _SC_Read(u8SCID, pu8Data+i, (*u16DataLen)-i);
                    if (len > 0)
                        continue;
                }
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] CWT RX error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_TIMEOUT;
                break;
            }
#if PATCH_TO_DISABLE_CGT //Iverlin: Patch to ignore CGT sent from card
            if (IS_RX_CGT_CHECK_DISABLE == FALSE)
            {
                if (_sc_T1State[u8SCID].u8CGT_RxIntCnt > 0)
                {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] CGT RX error\n", __FUNCTION__, __LINE__);
                    *u16DataLen = i;
                    eResult =  E_SC_FAIL;
                    break;
                }
            }
#endif
#if !PATCH_TO_DISABLE_BGT //Iverlin: Patch to ignore BGT sent from card
            if (_sc_T1State[u8SCID].u8BGT_IntCnt > 0)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] BGT error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_FAIL;
                break;
            }
#endif
            if (_sc_T1State[u8SCID].u8BWT_IntCnt > 0)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] BWT error\n", __FUNCTION__, __LINE__);
                *u16DataLen = i;
                eResult =  E_SC_FAIL;
                break;
            }
        }

        len = _SC_Read(u8SCID, pu8Data+i, (*u16DataLen)-i);
        if (len < 0)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "_SC_Read get fail\n");
            *u16DataLen = i;
            eResult = E_SC_FAIL;
        }
        if (len > 0)
        {
            if (bUseInternalWT == TRUE)
            {
                u32TimeoutCountMs = u32SetupTimeoutCountMs; //If data is available, reset timeoutcount
                u32SwWtStartTime = MsOS_GetSystemTime();
            }
        }

        if (bUseInternalWT)
        {
          if (u32SwWtStartTime!=0){
            u32DiffTime = MsOS_GetSystemTime();
            if (u32DiffTime < u32SwWtStartTime)
            {
                u32DiffTime = (0xFFFFFFFF - u32SwWtStartTime) + u32DiffTime;
            }
            else
            {
                u32DiffTime = u32DiffTime - u32SwWtStartTime;
            }

            if ((i < *u16DataLen) && (u32DiffTime > u32SwWt))
            {
                *u16DataLen = i;
                SC_DBG(E_SC_DBGLV_INFO, "[%s][%d]SW WT Timeout\n", __FUNCTION__, __LINE__);
                eResult =  E_SC_TIMEOUT;
                break;
            }
          }
        }
    }
    *u16DataLen = i;

    if (bUseInternalWT)
    {
        if (_sc_T0State[u8SCID].bUseBwtInsteadExtCwt)
        {
            HAL_SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
        }

        HAL_SC_RxFailAlwaysDetCWT(u8SCID, FALSE);
        _sc_T0State[u8SCID].u8WT_RxIntCnt = 0;
        _sc_T1State[u8SCID].u8CWT_RxIntCnt = 0;
    }

    return eResult;
}

SC_Result _SC_RawExchange(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_Result ret_Result = E_SC_OK;

    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    //Clear SW fifo to reset any remaining data bytes of previous comminication
    _SC_ResetFIFO(u8SCID);

    do{
        ret_Result = _SC_Send(u8SCID, pu8SendData, *u16SendDataLen, u32TimeoutMs);
        if (ret_Result != E_SC_OK)
            break;

        ret_Result = _SC_Recv(u8SCID, pu8ReadData, u16ReadDataLen, u32TimeoutMs);
    }while(0);

    //Add extra delay to ensure interval between RCV and Trans is larger than BGT for T=1
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        if (_sc_T1State[u8SCID].u32BGT < 1000)
            OS_DELAY_TASK(1);
        else
            OS_DELAY_TASK(_sc_T1State[u8SCID].u32BGT/1000);
    }

    return ret_Result;
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
        if (!HAL_SC_IsCardDetected(u8SCID))
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
        if (!HAL_SC_IsCardDetected(u8SCID))
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
            if (!HAL_SC_IsCardDetected(u8SCID))
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
    _scInfo[u8SCID].u8Fi = 1;
    _scInfo[u8SCID].u8Di = 1; // unspecified.
    _scInfoExt[u8SCID].u8N = 0;

    return E_SC_OK;
}

static void _SC_ClearProtocolState(MS_U8 u8SCID)
{
    _sc_T0State[u8SCID].u8WI = 10;//Default is 10 as 7816-3
    _sc_T0State[u8SCID].u32WT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T0State[u8SCID].u32GT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T0State[u8SCID].u8WT_RxIntCnt = 0;
    _sc_T0State[u8SCID].u8WT_TxIntCnt = 0;
    _sc_T0State[u8SCID].u8GT_RxIntCnt = 0;
    _sc_T0State[u8SCID].u8GT_TxIntCnt = 0;
    _sc_T0State[u8SCID].bParityError = FALSE;
    _sc_T1State[u8SCID].u8CWI = 13; //Default is 13 as 7816-3
    _sc_T1State[u8SCID].u8BWI = 4; //Default is 4 as 7816-3
    _sc_T1State[u8SCID].u32CWT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T1State[u8SCID].u32CGT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T1State[u8SCID].u32BWT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T1State[u8SCID].u32BGT = E_TIME_ST_NOT_INITIALIZED;
    _sc_T1State[u8SCID].u8BWT_IntCnt = 0;
    _sc_T1State[u8SCID].u8BGT_IntCnt = 0;
    _sc_T1State[u8SCID].u8CWT_RxIntCnt = 0;
    _sc_T1State[u8SCID].u8CWT_TxIntCnt = 0;
    _sc_T1State[u8SCID].u8CGT_RxIntCnt = 0;
    _sc_T1State[u8SCID].u8CGT_TxIntCnt = 0;
    _sc_T1State[u8SCID].bParityError = FALSE;
    _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = FALSE;
#if PATCH_TO_DISABLE_CGT
     ENABLE_RX_CGT_CHECK;

    #if PATCH_TO_DISABLE_CGT_IRDETO_T14
    if ((_scInfo[u8SCID].eCardClk == E_SC_CLK_6M) && (_scInfo[u8SCID].u16ClkDiv == 620))
    {
        DISABLE_RX_CGT_CHECK;
    }
    #endif
#endif
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
    if (HAL_SC_IsCardDetected(u8SCID))
    {
        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME * 10);// Wait Card stable
            HAL_SC_Ext8024ActiveSeq(u8SCID);
            HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);//Pull RSTIN low
        }
        else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME * 20);// Wait Card stable
            // move "~UART_LCR_SBC" and "UART_SCCR_RST" to here when on chip 8024
            // need to set "~UART_LCR_SBC" here when on chip 8024, otherwise active sequence trigger can't work in IO pin
            HAL_SC_InputOutputPadCtrl(u8SCID, E_HAL_SC_IO_FUNC_ENABLE);
            // need to set "UART_SCCR_RST" here when on chip 8024, otherwise active sequence trigger can't work in RST pin
            HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);//Pull RSTIN low
            HAL_SC_Int8024ActiveSeq(u8SCID);
        }
        else
        {
            OS_SC_DELAY(SC_CARD_STABLE_TIME);// Wait Card stable
            if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
            {
                HAL_SC_SmcVccPadCtrl(u8SCID, E_HAL_SC_LEVEL_HIGH);
            }
            else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
            {
                HAL_SC_SmcVccPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);
            }
            else//OCP
            {
                _SC_SetOCP(u8SCID, TRUE);
            }
            OS_SC_DELAY(1);// Wait vcc stable
        }

        // don't need these code when when on chip 8024
        if (_scInfo[u8SCID].eVccCtrl != E_SC_VCC_VCC_ONCHIP_8024)
        {
            HAL_SC_SmcClkCtrl(u8SCID, TRUE);// enable clock
            HAL_SC_InputOutputPadCtrl(u8SCID, E_HAL_SC_IO_FUNC_ENABLE);
        }
    }
    return E_SC_OK;
}

static SC_Result _SC_Deactivate(MS_U8 u8SCID)
{
    if(u8SCID >= SC_DEV_NUM)
        return E_SC_FAIL;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

#ifndef SC_USE_IO_51
    // Disable BGWT/CGWT interrupt firstly
    _SC_DisableIntBGWT(u8SCID);
    _SC_DisableIntCGWT(u8SCID);
    if (_gstHwFeature.bRstToIoEdgeDet == TRUE)
        _SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
#endif

    if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_8024_ON)
    {
        HAL_SC_Ext8024DeactiveSeq(u8SCID);
    }
    else if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_VCC_ONCHIP_8024)
    {
        HAL_SC_Int8024DeactiveSeq(u8SCID);
    }
    else
    {
        HAL_SC_ResetPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);//Pull RSTIN low
        OS_SC_DELAY(3);// Wait Card stable
        HAL_SC_SmcClkCtrl(u8SCID, FALSE); // clock disable

        OS_SC_DELAY(3);// Wait Card stable
        HAL_SC_InputOutputPadCtrl(u8SCID, E_HAL_SC_IO_FORCED_LOW);
        OS_SC_DELAY(3);// Wait Card stable

        if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH)
        {
            HAL_SC_SmcVccPadCtrl(u8SCID, E_HAL_SC_LEVEL_LOW);
        }
        else  if (_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_LOW)
        {
            HAL_SC_SmcVccPadCtrl(u8SCID, E_HAL_SC_LEVEL_HIGH);
        }
        else //OCP
        {
            _SC_SetOCP(u8SCID, FALSE);
        }
    }

    // Reset to default setting
    HAL_SC_SetUartMode(u8SCID, (SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO));

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

#ifndef SC_USE_IO_51
    // Disable BGWT/CGWT interrupt firstly
    _SC_DisableIntBGWT(u8SCID);
    _SC_DisableIntCGWT(u8SCID);
    if (_gstHwFeature.bRstToIoEdgeDet == TRUE)
        _SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
#endif

    // Disable all interrupts
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_DISABLE);
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_DETACH_INTERRUPT))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_EXIT failed\n", __FUNCTION__, __LINE__);
    }
#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    OS_SC_ISR_Disable(u8SCID);
#endif // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)

    _scInfo[u8SCID].bOpened = FALSE;

#if SMART_OCP_EN
    if ((_scInfo[u8SCID].eVccCtrl == E_SC_VCC_CTRL_HIGH) ||
             (_scInfo[u8SCID].eVccCtrl == E_SC_OCP_VCC_HIGH))
    {
        TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP0));
        TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP1));
        TOP_AND(REG_TOP_GPIO_TSI_OEN, ~(TOP_SMART_CARD_CONTROL_ENABLE_OCP2));
    }
#endif

    HAL_SC_Close(u8SCID, _SC_ConvHalVccCtrl(_scInfo[u8SCID].eVccCtrl));

    // reset buffer control pointer
    _SC_ResetFIFO(u8SCID);

    return E_SC_OK;
}

static SC_Result _SC_ParseATR(MS_U8 u8SCID)
{
    MS_U8            u8T0, u8TAx, u8TBx, u8TCx, u8TDx, u8Len;
    MS_U8            u8Ch, u8Crc = 0;

    int              i, x;

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
        if (HAL_SC_SetInvConvention(u8SCID) == FALSE)
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
    i++;
    //u8TS = _scInfo[u8SCID].pu8Atr[i++];
    u8T0 = _scInfo[u8SCID].pu8Atr[i++];

    u8Crc = u8Crc ^ u8T0;
    u8Len = u8T0 & 0x0F;
    u8TDx = u8T0 & 0xF0;

    x = 1;
    _scInfo[u8SCID].bSpecMode = FALSE;
    _scInfo[u8SCID].u8Di = _scInfo[u8SCID].u8Fi = 1;
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
                if ((_atr_Fi[_scInfo[u8SCID].u8Fi] == RFU)||(_atr_Di[_scInfo[u8SCID].u8Di] == RFU))
                {
                    SC_DBG(E_SC_DBGLV_ERR_ONLY, "Di or Fi parsing error \n");
                    return E_SC_FAIL;
                }
            }
            if (x == 2)
            {
                _scInfo[u8SCID].bSpecMode = TRUE; // specific mode
                _scInfo[u8SCID].u8Protocol = u8TAx & 0xF;
                if ((u8TAx & 0x10))
                {
                    //As 7816-3 page 19 defined, if bit[5] is '1', then implicit values (not defined by the interface bytes) shall apply
                    // TO DO
                }
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
            if (x >= 3 && _scInfo[u8SCID].u8Protocol == 1)
            {
                _sc_T1State[u8SCID].u8BWI = u8TBx >> 4;
                _sc_T1State[u8SCID].u8CWI = u8TBx & 0xF;
            }

        }
        if (u8TDx & 0x40) // TC
        {
            u8TCx = _scInfo[u8SCID].pu8Atr[i++];
            u8Crc = u8Crc ^ u8TCx;
            if (x == 1)
            {
                _scInfoExt[u8SCID].u8N = u8TCx;
            }
            if (x == 2)
            {
                _sc_T0State[u8SCID].u8WI = u8TCx;
            }
            if (x == 3)
            {
                _sc_T1State[u8SCID].u8RCType = u8TCx & 0x1;
            }
        }
        if ((u8TDx & 0x80))// TD
        {
            u8TDx = _scInfo[u8SCID].pu8Atr[i++];
            _scInfo[u8SCID].u8Protocol = u8TDx & 0xF;
            u8Crc = u8Crc ^ u8TDx;
            x++;
        }
        else
        {
            break;
        }
    }

    // Get ATR history
    _scInfo[u8SCID].u16HistLen = u8Len;

    //
    // If protocol is T=0, there is no checksum byte.
    // We need to check received bytes number and expect ATR number (from T0~Historical Bytes)
    //
    if (_scInfo[u8SCID].u8Protocol == 0x00)
    {
        if (_scInfo[u8SCID].u16AtrLen < (i + _scInfo[u8SCID].u16HistLen))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "ATR length error!! \n");
            return E_SC_FAIL;
        }
    }

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

#ifndef SC_USE_IO_51
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        _SC_Calculate_CGWT(u8SCID, _sc_T1State[u8SCID].u8CWI);
        _SC_Calculate_BGWT(u8SCID, _sc_T1State[u8SCID].u8BWI);
    }
    else
    {
        _SC_Calculate_GWT(u8SCID);
    }

    //Enable interrupt
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        _SC_EnableIntBGWT(u8SCID);
    }
    _SC_EnableIntCGWT(u8SCID);
#endif

    return E_SC_OK;

}


#ifdef SC_USE_IO_51
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
        printf("Data buffer not given, call MDrv_SC_SetDataBuffAddr() first\n");
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

    u32SrcAddr = (MS_U32)gpScDataBuffPA;
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
    int i;
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
            //Get ATR data
            _scInfo[u8SCID].u16AtrLen = gpScDataBuffVA->u16DataLen;
            memcpy(_scInfo[u8SCID].pu8Atr, gpScDataBuffVA->u8Data, _scInfo[u8SCID].u16AtrLen);

            //Parse ATR data
            _SC_ParseATR(gpScDataBuffVA->u8SCID);

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
static MS_BOOL _SC_CheckRstToAtrTimeout(MS_U8 u8SCID)
{
    return _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout;
}

static void _SC_RstToAtrTimeoutProc(MS_U8 u8SCID)
{
    MS_U32 u32RstToAtrPeriod;

    /////////////////////
    // SW_PATCH_RST_IO_TIMEOUT => HW provides conax_RST_to_IO_edge_detect_en bit to detect RST-to-IO timeout, but it will causes CGT/CWT malfunction since the
    //                                                     HW timer is occupied by RST-to-IO. Now we is temporary to use SW to check timeout
    /////////////////////

    _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = FALSE;
    u32RstToAtrPeriod = ((1000 * RST_TO_ATR_TIMEOUT)/((_SC_ConvClkEnumToNum(_scInfo[u8SCID].eCardClk))/1000UL));

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_CHECK_RST_TO_ATR, (int *)&u32RstToAtrPeriod))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_CHECK_RST_TO_ATR failed\n", __FUNCTION__, __LINE__);
    }
#else
    MS_U32 u32Step = 100;

    if (u32RstToAtrPeriod < u32Step)
        u32RstToAtrPeriod = u32Step;

    while (u32RstToAtrPeriod > 0)
    {
        MsOS_DelayTaskUs(u32Step);

        if (_scInfo[u8SCID].u16FifoRxRead != _scInfo[u8SCID].u16FifoRxWrite)
            break;

        if (u32RstToAtrPeriod < u32Step)
            u32RstToAtrPeriod = u32Step;

        u32RstToAtrPeriod -= u32Step;
    }

    if (u32RstToAtrPeriod > 0)
        _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = FALSE;
    else
        _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = TRUE;
#endif
}

static SC_Result _SC_SetupHwFeature(MS_U32 u32HwVer)
{
    SC_Result eResult = E_SC_OK;
    switch(u32HwVer)
    {
        case 0x00010110: //K6U02
            _gstHwFeature.bRstToIoEdgeDet = TRUE;
            _gstHwFeature.bExtCwtFixed = TRUE;
            break;

        case 0x00010101: //K6lite, K5TN
            _gstHwFeature.bRstToIoEdgeDet = TRUE;
            _gstHwFeature.bExtCwtFixed = FALSE;
            break;

        case 0x00010100: //K6U01
            _gstHwFeature.bRstToIoEdgeDet = FALSE;
            _gstHwFeature.bExtCwtFixed = FALSE;
            break;

        case 0x00010099: //Kayla
            _gstHwFeature.bRstToIoEdgeDet = FALSE;
            _gstHwFeature.bExtCwtFixed = FALSE;
            break;

        case 0x00010098: //keres, kirin, kiwi, kris
            _gstHwFeature.bRstToIoEdgeDet = FALSE;
            _gstHwFeature.bExtCwtFixed = FALSE;
            break;

        default:
            _gstHwFeature.bRstToIoEdgeDet = FALSE;
            _gstHwFeature.bExtCwtFixed = FALSE;
            eResult = E_SC_FAIL;
            break;
    }

    return eResult;
}

static void _SC_RstToIoEdgeDetCtrl(MS_U8 u8SCID, MS_BOOL bEnable)
{
    //
    // NOTICE: RstToIoEdgeDet interrupt will be masked in ISR to prevent interrupt burst
    //
    if (bEnable)
    {
        HAL_SC_SetRstToIoTimeout(u8SCID, 40000UL, _SC_ConvClkEnumToNum( _scInfo[u8SCID].eCardClk));
        HAL_SC_RstToIoEdgeDetCtrl(u8SCID, TRUE);
        HAL_SC_SetIntRstToIoEdgeFail(u8SCID, TRUE);
    }
    else
    {
        HAL_SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
        HAL_SC_SetIntRstToIoEdgeFail(u8SCID, FALSE);
    }
}

static MS_U32 _SC_GetRcvWaitingTime(MS_U8 u8SCID)
{
    MS_U32 u32CWT_TimeoutCountMs = 0;
    //
    // Setup CWT/WT timeout value:
    // Since HW CWT/WT timeout flag cannot be set if no any data bytes received.
    // So we add SW CWT/WT monitor to workaround above use case...
    //
    if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
    {
        u32CWT_TimeoutCountMs = _sc_T0State[u8SCID].u32WT / 1000;
        if (_sc_T0State[u8SCID].u32WT % 1000)
            u32CWT_TimeoutCountMs += 1;
    }
    else
    {
        u32CWT_TimeoutCountMs = _sc_T1State[u8SCID].u32CWT / 1000;
        if (_sc_T1State[u8SCID].u32CWT % 1000)
            u32CWT_TimeoutCountMs += 1;
    }
    if (u32CWT_TimeoutCountMs <= 0)
    {
        u32CWT_TimeoutCountMs = 1;
    }

    return u32CWT_TimeoutCountMs;
}

static SC_Result _SC_SetupUartInt(MS_U8 u8SCID)
{
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U32 u32Attr = 0x00;
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_GET_ATTRIBUTE, (int *)&u32Attr))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_GET_ATTRIBUTE failed\n", __FUNCTION__, __LINE__);
        return E_SC_FAIL;
    }
    u32Attr |= E_SC_ATTR_TX_LEVEL; // Enable Tx level driven mechanism for data trasmiting
    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_SET_ATTRIBUTE, (int *)&u32Attr))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_SET_ATTRIBUTE failed\n", __FUNCTION__, __LINE__);
        return E_SC_FAIL;
    }
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_RDI);
#else
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_RDI);
#endif

    return E_SC_OK;
}

static SC_Result _SC_ParsePPS(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 u16ReadDataLen)
{
    SC_Result eResult = E_SC_OK;
    MS_BOOL bUseDefaultPPS = FALSE;

    do{
        //ISO 7816 9.3 Successful PPS exchange (Page 21)
        if (u16ReadDataLen < 3)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "the length of PPS0_Response is smaller than 3, length = %d\n", u16ReadDataLen);
            eResult = E_SC_PPSFAIL;
            break;
        }

        // Bits1 to 4 of PPS0_Response shall be identical to bits 1 to 4 of PPS0_Request
        if((pu8ReadData[1]&0x0f) != (pu8SendData[1]&0x0f))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "Bits1 to 4 of PPS0_Response is different from PPS0, PPS0=0x%x, PPS0_RESPONSE=0x%x\n", pu8SendData[1], pu8ReadData[1]);
            eResult = E_SC_PPSFAIL;
            break;
        }

        //if bit 5 of PPS0 is set to 1, PPS1_Response shall be identical to PPS1_REQUEST
        if((pu8ReadData[1] & 0x10) == 0x10)
        {
            if(pu8ReadData[2] != pu8SendData[2])
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "PPS1_RESPONSE is different from PPS1, PPS1=0x%x, PPS1_REQUEST=0x%x\n", pu8SendData[2], pu8ReadData[2]);
                eResult = E_SC_PPSFAIL;
                break;
            }
        }
        else //if bit 5 of PPS0 is set to 0, PPS1_Response shall be absent, meaning that Fd and Dd shall be used
        {
            if (u16ReadDataLen != u16SendDataLen - 1)
            {
                SC_DBG(E_SC_DBGLV_ERR_ONLY, "bit 5 of PPS0 is set to 0, but PPS1_Response exists,\n");
                eResult = E_SC_PPSFAIL;
                break;
            }
            else
            {
                eResult = E_SC_OK;
            }

            bUseDefaultPPS = TRUE;
        }
    }while(0);

    // If PPS OK, update Fi and Di
    if ((eResult == E_SC_OK) && (!bUseDefaultPPS))
    {
        _scInfo[u8SCID].u8Fi = (pu8SendData[2] & 0xF0) >> 4;
        _scInfo[u8SCID].u8Di = pu8SendData[2] & 0x0F;
        SC_DBG(E_SC_DBGLV_INFO, "Fi update: %u\n", _scInfo[u8SCID].u8Fi);
        SC_DBG(E_SC_DBGLV_INFO, "Di update: %u\n", _scInfo[u8SCID].u8Di);
    }

    return eResult;
}

static void _SC_EnableIntCGWT(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
    {
        _sc_T0State[u8SCID].u8WT_TxIntCnt = 0;  _sc_T0State[u8SCID].u8WT_RxIntCnt = 0;
        _sc_T0State[u8SCID].u8GT_TxIntCnt = 0;  _sc_T0State[u8SCID].u8GT_RxIntCnt = 0;
    }
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        _sc_T1State[u8SCID].u8CWT_TxIntCnt = 0;  _sc_T1State[u8SCID].u8CWT_RxIntCnt = 0;
        _sc_T1State[u8SCID].u8CGT_TxIntCnt = 0;  _sc_T1State[u8SCID].u8CGT_RxIntCnt = 0;
    }

    HAL_SC_SetIntCGWT(u8SCID, E_HAL_SC_CGWT_INT_ENABLE);
}

static void _SC_DisableIntCGWT(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    HAL_SC_SetIntCGWT(u8SCID, E_HAL_SC_CGWT_INT_DISABLE);

    if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)
    {
        _sc_T0State[u8SCID].u8WT_TxIntCnt = 0;  _sc_T0State[u8SCID].u8WT_RxIntCnt = 0;
        _sc_T0State[u8SCID].u8GT_TxIntCnt = 0;  _sc_T0State[u8SCID].u8GT_RxIntCnt = 0;
    }
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        _sc_T1State[u8SCID].u8CWT_TxIntCnt = 0;  _sc_T1State[u8SCID].u8CWT_RxIntCnt = 0;
        _sc_T1State[u8SCID].u8CGT_TxIntCnt = 0;  _sc_T1State[u8SCID].u8CGT_RxIntCnt = 0;
    }
}

static void _SC_EnableIntBGWT(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    _sc_T1State[u8SCID].u8BWT_IntCnt = 0;
    _sc_T1State[u8SCID].u8BGT_IntCnt = 0;
    HAL_SC_SetIntBGWT(u8SCID, E_HAL_SC_BGWT_INT_ENABLE);
}

static void _SC_DisableIntBGWT(MS_U8 u8SCID)
{
    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    HAL_SC_SetIntBGWT(u8SCID, E_HAL_SC_BGWT_INT_DISABLE);
    _sc_T1State[u8SCID].u8BWT_IntCnt = 0;
    _sc_T1State[u8SCID].u8BGT_IntCnt = 0;
}

static MS_U32 _SC_ConvClkEnumToNum(SC_ClkCtrl  eCardClk)
{
    MS_U32 u32Clk;

    switch(eCardClk)
    {
        case E_SC_CLK_3M:
            u32Clk = 3375000UL;
            break;

        case E_SC_CLK_4P5M:
        default:
            u32Clk = 4500000UL;
            break;

        case E_SC_CLK_6M:
            u32Clk = 6750000UL;
            break;

        case E_SC_CLK_13M:
            u32Clk = 13500000UL;
            break;
    }

    return u32Clk;
}

static SC_Result  _SC_Calculate_BGWT(MS_U8 u8SCID, MS_U8 u8BWI)
{
    MS_U32 u32_f;
    MS_U32 u32WorkETU;
    MS_U32 u32RegBWT;
    MS_FLOAT fTempBWT = 0.0;
    MS_FLOAT fETU = 0.0;

     SC_DBG(E_SC_DBGLV_INFO, "***** Calculate_BGWT *****\n");

    u32_f = _SC_ConvClkEnumToNum( _scInfo[u8SCID].eCardClk);

    //Calculate ETU = (Fi/Di) * (1/f)  (seconds)
    u32WorkETU = (_atr_Fi[_scInfo[u8SCID].u8Fi] / _atr_Di[_scInfo[u8SCID].u8Di])*1000000UL / (u32_f/1000UL); // in nsec
    SC_DBG(E_SC_DBGLV_INFO, "***** Work etu = %u nsec *****\n", u32WorkETU);

    //BWT = 11 etu + 2 ^ ( BWI ) * 960 * ( Fd / f )  (seconds)
    fTempBWT = ((1<<u8BWI) * 960.0 * 372.0 / (float)u32_f) * 1000000.0; //usec
    fETU = ((float)u32WorkETU) / 1000.0; //usec
    fTempBWT = fTempBWT / fETU; //etu
    fTempBWT = fTempBWT + 1.0 + 11.0;
    fTempBWT = ((float)fTempBWT)*SC_STAR3150_COMPENSATION;
    u32RegBWT = (MS_U32)fTempBWT;
    _sc_T1State[u8SCID].u32BWT = u32RegBWT * u32WorkETU/1000UL;// in usec
    HAL_SC_SetBWT(u8SCID, u32RegBWT);


    // BGT = 22 etu
     _sc_T1State[u8SCID].u32BGT = 22 * u32WorkETU/1000UL;// in usec
     HAL_SC_SetBGT(u8SCID, 22);

    SC_DBG(E_SC_DBGLV_INFO, "***** u32RegBWT = 0x%x *****\n", u32RegBWT);
     SC_DBG(E_SC_DBGLV_INFO, "***** BWT = %u usec *****\n", _sc_T1State[u8SCID].u32BWT);
     SC_DBG(E_SC_DBGLV_INFO, "***** BGT = %u usec *****\n\n", _sc_T1State[u8SCID].u32BGT);

    return E_SC_OK;
}

static SC_Result  _SC_Calculate_CGWT(MS_U8 u8SCID, MS_U8 u8CWI)
{
    MS_U32 u32_f;
    MS_U32 u32WorkETU;
    MS_U32 u32RegCWT;
    MS_U8 u8ExtraGT = 0;
    MS_U8 u8CGT = 0;
    MS_FLOAT fETU = 0.0;

    SC_DBG(E_SC_DBGLV_INFO, "***** Calculate_CGWT *****\n");

    u32_f = _SC_ConvClkEnumToNum( _scInfo[u8SCID].eCardClk);

    //Calculate ETU = (Fi/Di) * (1/f)  (seconds)
    u32WorkETU = (_atr_Fi[_scInfo[u8SCID].u8Fi] / _atr_Di[_scInfo[u8SCID].u8Di])*1000000UL / (u32_f/1000UL); // in nsec
    SC_DBG(E_SC_DBGLV_INFO, "***** Work etu = %u nsec *****\n", u32WorkETU);

    if (u8CWI == 0xFF) //For PPS case
    {
        //CWT
        u32RegCWT = 9600UL;
        fETU = ((float)u32RegCWT)*SC_STAR3150_COMPENSATION;
        u32RegCWT = (MS_U32)fETU;
        _sc_T1State[u8SCID].u32CWT = u32RegCWT * u32WorkETU / 1000UL; // in usec
        SC_DBG(E_SC_DBGLV_INFO, "***** PPS CWT = %u usec *****\n", _sc_T1State[u8SCID].u32CWT);

        //CGT
        //Calculate extra guard time
        if ( _scInfoExt[u8SCID].u8N == 0xFF)
        {
            u8ExtraGT = 0;
            u8CGT = 12; //12etu
        }
        else
        {
            u8ExtraGT = _scInfoExt[u8SCID].u8N;
            u8CGT = 12 + u8ExtraGT; //12etu + R*(N/f). R = Fi/Di
        }

        // GT = 12 etu + R * N / f, R = F/D
        _sc_T0State[u8SCID].u32GT = ((MS_U32)u8CGT * u32WorkETU) / 1000UL; // in usec
        SC_DBG(E_SC_DBGLV_INFO, "***** PPS extra GT integer = %u  *****\n", u8ExtraGT);
        SC_DBG(E_SC_DBGLV_INFO, "***** PPS GT = %u usec *****\n", _sc_T0State[u8SCID].u32GT);
    }
    else
    {
        //CWT = (11+2^CWI) * etu
        _sc_T1State[u8SCID].u32CWT = (11+(1<<u8CWI))*u32WorkETU / 1000UL; // in usec
         u32RegCWT = (11+(1<<u8CWI));
        fETU = ((float)u32RegCWT)*SC_STAR3150_COMPENSATION;
        u32RegCWT = (MS_U32)fETU;
        SC_DBG(E_SC_DBGLV_INFO, "***** CWT = %u usec *****\n", _sc_T1State[u8SCID].u32CWT);

        //CGT
        if (_scInfoExt[u8SCID].u8N == 0xFF)
        {
            //As 7816-3 page 24 (ch 11.2), the CGT = 11etu if N = 255
            u8CGT = 11;
            _sc_T1State[u8SCID].u32CGT = ((MS_U32)u8CGT) * u32WorkETU / 1000UL; // in usec
        }
        else
        {
            //As 7816-3 page 24 (ch 11.2), the CGT = GT if N = 0~254
            u8ExtraGT = _scInfoExt[u8SCID].u8N;
            u8CGT = 12 + u8ExtraGT;

            // GT = 12 etu + R * N / f, R = F/D
            _sc_T1State[u8SCID].u32CGT = (u8CGT*u32WorkETU)/1000UL; // in usec
        }
        SC_DBG(E_SC_DBGLV_INFO, "***** CGT = %u usec *****\n\n", _sc_T1State[u8SCID].u32CGT);
    }

    HAL_SC_SetCWT(u8SCID, u32RegCWT);
    HAL_SC_SetCGT(u8SCID, u8CGT);

    return E_SC_OK;
}


static SC_Result  _SC_Calculate_GWT(MS_U8 u8SCID)
{
    MS_U32 u32_f;
    MS_U32 u32WorkETU;
    MS_U32 u32RegCWT = 0;
    MS_U8 u8ExtraGT = 0;
    MS_U8 u8CGT = 0;
    MS_FLOAT fETU = 0.0;

    SC_DBG(E_SC_DBGLV_INFO, "***** Calculate_GWT *****\n");

    u32_f = _SC_ConvClkEnumToNum( _scInfo[u8SCID].eCardClk);

    //Calculate ETU
    u32WorkETU = (_atr_Fi[_scInfo[u8SCID].u8Fi] / _atr_Di[_scInfo[u8SCID].u8Di])*1000000UL / (u32_f/1000UL); // in nsec
    SC_DBG(E_SC_DBGLV_INFO, "***** Work etu = %u nsec *****\n", u32WorkETU);

    //===== Calculate WT =====
    _sc_T0State[u8SCID].u32WT = (960UL * u32WorkETU/1000UL) * _sc_T0State[u8SCID].u8WI * _atr_Di[_scInfo[u8SCID].u8Di]; // in usec
    u32RegCWT = _sc_T0State[u8SCID].u8WI * 960UL * _atr_Di[_scInfo[u8SCID].u8Di];
    fETU = ((float)u32RegCWT)*SC_STAR3150_COMPENSATION;
    u32RegCWT = (MS_U32)fETU;
    if (u32RegCWT & 0xFFFF0000)
    {
        //////////////////////////////////////////////
        // This is a HW patch for extent CWT issue.
        // We can use BWT function as ext-CWT function if the following conditions are conformed
        // 1. The HW BWT counter is independent of Reset-to-I/O counter (bRstToIoEdgeDet)
        // 2. The HW extent CWT issue is not fixed (bExtCwtFixed)
        //
        // NOTE: When BWT works as ext-CWT, the RST_TO_IO_EDGE_DET_EN (0x1029_0x0A[7]) must be set
        // ///////////////////////////////////////////
        if (_gstHwFeature.bExtCwtFixed == FALSE && _gstHwFeature.bRstToIoEdgeDet == TRUE)
        {
            _sc_T0State[u8SCID].bUseBwtInsteadExtCwt = TRUE;
            HAL_SC_SetBWT(u8SCID, u32RegCWT);
            HAL_SC_SetCWT(u8SCID, 0x10000UL);
        }
        else
        {
            _sc_T0State[u8SCID].bUseBwtInsteadExtCwt = FALSE;
            HAL_SC_SetCWT(u8SCID, u32RegCWT);
        }
    }
    else
    {
        _sc_T0State[u8SCID].bUseBwtInsteadExtCwt = FALSE;
        HAL_SC_SetCWT(u8SCID, u32RegCWT);
    }
    SC_DBG(E_SC_DBGLV_INFO, "***** WT reg = 0x%x *****\n", u32RegCWT);
   SC_DBG(E_SC_DBGLV_INFO, "***** WT = %u usec *****\n", _sc_T0State[u8SCID].u32WT);

    //===== Calculate GT =====
    //Calculate extra guard time
    if ( _scInfoExt[u8SCID].u8N == 0xFF)
    {
        u8ExtraGT = 0;
    }
    else
    {
        u8ExtraGT = _scInfoExt[u8SCID].u8N;
    }
    //Add 1 etu to pass "delay at least 2 etu after detection of error signal"
    u8CGT = u8ExtraGT+12;
    HAL_SC_SetCGT(u8SCID, u8CGT);

    // GT = 12 etu + R * N / f, R = F/D
    _sc_T0State[u8SCID].u32GT = (u8CGT*u32WorkETU)/1000UL; // in usec

    SC_DBG(E_SC_DBGLV_INFO, "***** extra GT integer = 0x%x  *****\n", u8ExtraGT);
    SC_DBG(E_SC_DBGLV_INFO, "***** GT = %u usec *****\n\n", _sc_T0State[u8SCID].u32GT);

    return E_SC_OK;
}

static SC_Result _SC_GetATR(MS_U8 u8SCID, MS_U32 u32TimeoutMs)
{
    SC_Result ScResult = E_SC_OK;

    SC_DBG(E_SC_DBGLV_INFO, "%s:\n", __FUNCTION__);
    if (_scInfo[u8SCID].bCardIn == FALSE)
    {
        SC_DBG(E_SC_DBGLV_INFO, "   DRV Card OUT\n");
        return E_SC_CARDOUT;
    }

    //Initialize
    _sc_T0State[u8SCID].u8WI = 10;//Default is 10 as 7816-3
    _sc_T1State[u8SCID].u8CWI = 13; //Default is 13 as 7816-3
    _sc_T1State[u8SCID].u8BWI = 4; //Default is 13 as 7816-3
    _scInfo[u8SCID].u8Fi = 1;
    _scInfo[u8SCID].u8Di = 1;
    _scInfoExt[u8SCID].u8N = 0;

    //Get ATR data
    (void)u32TimeoutMs;
    _scInfo[u8SCID].u16AtrLen = SC_ATR_LEN_MAX;
    ScResult = _SC_Recv(u8SCID, _scInfo[u8SCID].pu8Atr, &(_scInfo[u8SCID].u16AtrLen), E_INVALID_TIMEOUT_VAL);
    _SC_DisableIntCGWT(u8SCID); //This is paired to GWT control and int enable loacted at _sc_reset()

    //Check HW RstToIoEdgeDet
    if (_gstHwFeature.bRstToIoEdgeDet == TRUE)
        _SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);

    //Clear bRstToAtrTimeout
    _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = FALSE;

    if (ScResult == E_SC_OK || ScResult == E_SC_TIMEOUT)
    {
        //Parse ATR data
        if (_scInfo[u8SCID].u16AtrLen == 0)
        {
            ScResult = E_SC_NODATA;
            printf("_SC_GetATR get E_SC_NODATA\n");
        }
        else
        {
            ScResult =  _SC_ParseATR(u8SCID);
        }
    }

    return ScResult;
}

#endif

static SC_Result _SC_PPS(MS_U8 u8SCID, MS_U8 u8T, MS_U8 u8DI, MS_U8 u8FI)
{
    SC_Result           eResult = E_SC_OK;
    MS_U8               u8SendData[SC_PPS_LEN_MAX];
    MS_U16              u16SendLen;
    MS_U32              u32Idx;

    if ((_atr_Fi[u8FI] == RFU)||(_atr_Di[u8DI] == RFU))
    {
        return E_SC_FAIL;
    }

    //NOTE: Just consider D/F for "work etc" here
    _pu8PPS[0] = u8SendData[0] = 0xFF;                                  // PPSS
    _pu8PPS[1] = u8SendData[1] = 0x10 | (0x0F & u8T);                   // T=x
    _pu8PPS[2] = u8SendData[2] = u8DI | (u8FI<<4);                      // PPS1
    _pu8PPS[3] = u8SendData[3] = _pu8PPS[0] ^ _pu8PPS[1] ^ _pu8PPS[2];  // PCK
    _u16PPSLen = u16SendLen = 4;

#ifdef SC_USE_IO_51
    eResult= _SC_RawExchange(u8SCID, u8SendData, &u16SendLen, _pu8PPS, &_u16PPSLen, 0);
#else
    MS_U8 u8TempWI = 0;

    // Disable BGWT/CGWT interrupt firstly
    _SC_DisableIntBGWT(u8SCID);
    _SC_DisableIntCGWT(u8SCID);

    //Setup WT and GT for PPS
    if (_scInfo[u8SCID].u8Protocol != 1)
    {
        u8TempWI = _sc_T0State[u8SCID].u8WI;
        _sc_T0State[u8SCID].u8WI = 10;
        _SC_Calculate_GWT(u8SCID);
    }
    else
    {
        _SC_Calculate_CGWT(u8SCID, 0xFF);
    }

    _SC_EnableIntCGWT(u8SCID);
    do{
    if (((eResult = _SC_Send(u8SCID, u8SendData, u16SendLen, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    PPS Send Failed!!\n");
            break;
    }
    eResult = _SC_Recv(u8SCID, _pu8PPS, &_u16PPSLen, SC_T0_RECV_TIMEOUT);
    }while(0);
    _SC_DisableIntCGWT(u8SCID);

    //Parse PPS
    if (eResult == E_SC_OK || eResult == E_SC_TIMEOUT)
        eResult = _SC_ParsePPS(u8SCID, u8SendData, u16SendLen, _pu8PPS, _u16PPSLen);

    //Restore WT and GT for PPS
    if (_scInfo[u8SCID].u8Protocol != 1)
    {
        _sc_T0State[u8SCID].u8WI = u8TempWI;
        _SC_Calculate_GWT(u8SCID);
    }
    else
    {
        _SC_Calculate_CGWT(u8SCID, _sc_T1State[u8SCID].u8CWI);
    }

    //Enable interrupt
    if (_scInfo[u8SCID].u8Protocol == 1)
    {
        _SC_EnableIntBGWT(u8SCID);
    }
    _SC_EnableIntCGWT(u8SCID);

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

    if (_scInfo[u8SCID].s32DevFd < 0)
        return E_SC_FAIL;

    if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_GET_EVENTS, (int *)&u32Evt))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_GET_EVENTS failed\n", __FUNCTION__, __LINE__);
    }

    if (u32Evt & UART_SCSR_INT_CARDOUT) // card out
    {
        if ((_scInfo[u8SCID].eVccCtrl != E_SC_VCC_CTRL_8024_ON) && (_scInfo[u8SCID].eVccCtrl != E_SC_VCC_VCC_ONCHIP_8024))
        {
            // Clear interrupt for 1 ms to avoid the slow drop of signals (IO, Vcc) caused by the indication of interrupt
            MS_U32 u32CurrTime = MsOS_GetSystemTime();
            while ((u32CurrTime + 1 ) - MsOS_GetSystemTime() != 0)
            {
                HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_ALL_CLEAR);
                /* Avoid time interval overflow */
                if ((u32CurrTime + 1 ) - MsOS_GetSystemTime() > 1)
                    break;
            }
        }

        _SC_Deactivate(u8SCID);
        _scInfo[u8SCID].bCardIn = FALSE;
    }
    if (HAL_SC_IsCardDetected(u8SCID) && (u32Evt & UART_SCSR_INT_CARDIN)) // card in
    {
        _SC_Activate(u8SCID);
        _scInfo[u8SCID].bCardIn = TRUE;
    }

    ///////////////////////////
    // To check if any timing fail found
    ///////////////////////////
    if (u32Evt & E_SC_INT_TX_LEVEL)
    {
        u16ISRTXLevel++;
    }
    if (u32Evt & E_SC_INT_RST_TO_ATR_FAIL)
    {
        _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = TRUE;
    }
    if (u32Evt & E_SC_INT_PE_FAIL)/////////////////
    {
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].bParityError = TRUE;
        if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].bParityError = TRUE;
    }
    if (u32Evt & E_SC_INT_CWT_RX_FAIL)  //CWT RX INT
    {
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8WT_RxIntCnt++;
        if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CWT_RxIntCnt++;
    }
    if (u32Evt & E_SC_INT_CWT_TX_FAIL)  //CWT TX INT
    {
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8WT_TxIntCnt++;
        if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CWT_TxIntCnt++;
    }
#if PATCH_TO_DISABLE_CGT //Iverlin: Patch to ignore CGT sent from card
    if (IS_RX_CGT_CHECK_DISABLE == FALSE)
    {
    if (u32Evt & E_SC_INT_CGT_RX_FAIL)  //CGT RX INT
    {
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8GT_RxIntCnt++;
        if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CGT_RxIntCnt++;
    }
    }
#endif
    if (u32Evt & E_SC_INT_CGT_TX_FAIL)  //CGT TX INT
    {
        if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8GT_TxIntCnt++;
        if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CGT_TxIntCnt++;
    }
    if (u32Evt & E_SC_INT_BGT_FAIL)  //BGT INT
    {
        _sc_T1State[u8SCID].u8BGT_IntCnt++;
    }
    if (u32Evt & E_SC_INT_BWT_FAIL)  //BWT INT
    {
        //If BWT works as ext-CWT, to clear 0x1029_0x0A[7] to prevnet BWT fail int is always triggered
        if (_sc_T0State[u8SCID].bUseBwtInsteadExtCwt)
        {
            HAL_SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
        }
        _sc_T1State[u8SCID].u8BWT_IntCnt++;
    }
    //////////////////////////


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
    HAL_SC_CD_INT_STATUS stCardDetectInt;

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
        HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
        if ((stCardDetectInt.bCardInInt || stCardDetectInt.bCardOutInt) && (_scInfo[u8SCID].bCardIn == FALSE))
        {
            MsOS_DelayTask(1);
            HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_OUT_CLEAR); // clear car out interrupt
        }
// cynthia : temporarily patch for fast INSERT/REMOVE
// @TODO: refine it
ReDetect:
        HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
        if (!HAL_SC_IsCardDetected(u8SCID) || stCardDetectInt.bCardOutInt) // card out
        {
            if (stCardDetectInt.bCardOutInt)
                HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_OUT_CLEAR); // clear card out interrupt
            if (stCardDetectInt.bCardInInt)
                HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_IN_CLEAR); // clear card in interrupt

            _SC_Deactivate(u8SCID);
            _scInfo[u8SCID].bCardIn = FALSE;
        }

        HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
        if (HAL_SC_IsCardDetected(u8SCID) && !stCardDetectInt.bCardOutInt) // card in
        {
            if (stCardDetectInt.bCardOutInt)
                HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_OUT_CLEAR); // clear card out interrupt
            if (stCardDetectInt.bCardInInt)
                HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_IN_CLEAR); // clear card in interrupt

            _SC_Activate(u8SCID);
            _scInfo[u8SCID].bCardIn = TRUE;
        }

        // Wait Card stable
        OS_SC_DELAY(SC_CARD_STABLE_TIME);

        // cynthia : temporarily patch for fast INSERT/REMOVE
        // @TODO: refine it
        HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
        if (stCardDetectInt.bCardOutInt)
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

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

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
    static int err[2] = {-1,-1};
    static int err_in[2] = {-1,-1};
#else
    static int err[1] = {-1};
    static int err_in[1] = {-1};
#endif

    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (_scInfo[i].s32DevFd >= 0)
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
    }

    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (_scInfo[i].bCardIn && !HAL_SC_IsCardDetected(i))
        {
            OS_SC_DELAY(10);
            if (_scInfo[i].bCardIn && !HAL_SC_IsCardDetected(i))
                ;
            else if (HAL_SC_IsCardDetected(i))
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
            _SC_Deactivate(i);
            err[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_OUT); // Polling CARD_OUT
        }
    }
#if 1
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (!_scInfo[i].bCardIn && HAL_SC_IsCardDetected(i))
        {
            OS_SC_DELAY(10);
            if (!_scInfo[i].bCardIn && HAL_SC_IsCardDetected(i))
                ;
            else if (!HAL_SC_IsCardDetected(i))
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
            _SC_Activate(i);
            err_in[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_IN); // Polling CARD_IN
        }
    }
#endif

#else // defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    MS_U32              u32Events;
    MS_U32              i;

#if (SC_DEV_NUM>1)
        static int err[2] = {-1,-1};
        static int err_in[2] = {-1,-1};
#else
        static int err[1] = {-1};
        static int err_in[1] = {-1};
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

    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (_scInfo[i].bCardIn && !HAL_SC_IsCardDetected(i))
        {
            OS_SC_DELAY(10);
            if (_scInfo[i].bCardIn && !HAL_SC_IsCardDetected(i))
                ;
            else if (HAL_SC_IsCardDetected(i))
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
            _SC_Deactivate(i);
            err[i] = -1;
            _SC_Callback_Notify(i, E_SC_EVENT_OUT); // Polling CARD_OUT
        }
    }
#if 1
    for (i = 0; i < SC_DEV_NUM; i++)
    {
        if (!_scInfo[i].bCardIn && HAL_SC_IsCardDetected(i))
        {
            OS_SC_DELAY(10);
            if (!_scInfo[i].bCardIn && HAL_SC_IsCardDetected(i))
                ;
            else if (!HAL_SC_IsCardDetected(i))
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
            _SC_Activate(i);
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
    HAL_SC_CD_INT_STATUS stCardDetectInt;
    HAL_SC_TX_LEVEL_GWT_INT stTxLevelGWT_Int;
    HAL_SC_UART_INT_ID eIntID;

#ifndef SC_USE_IO_51
    MS_U32  i=0;
#endif

    // to clear interrupt here is to avoid the situation of vcc and io low.
    HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
    if (stCardDetectInt.bCardInInt || stCardDetectInt.bCardOutInt)
    {
        //SC_WRITE(u8SCID, UART_SCSR, SC_READ(u8SCID,UART_SCSR)); // clear interrupt

        goto NOTIFY;
    }
#ifndef SC_USE_IO_51
    eIntID = HAL_SC_GetUartIntID(u8SCID);
    HAL_SC_GetIntTxLevelAndGWT(u8SCID, &stTxLevelGWT_Int);

    if (eIntID == E_HAL_SC_UART_INT_ID_THRE)
    {
        u16ISRTXEmpty++;

        while(1)
        {
            if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
                break;
            HAL_SC_WriteTxData(u8SCID, _scInfo[u8SCID].u8FifoTx[_scInfo[u8SCID].u16FifoTxRead++]);
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
                if (i >= 16)
                    break;
            }
        }
    }

    if (stTxLevelGWT_Int.bTxLevelInt || stTxLevelGWT_Int.bCGT_TxFail ||
        stTxLevelGWT_Int.bCGT_RxFail || stTxLevelGWT_Int.bCWT_TxFail ||
        stTxLevelGWT_Int.bCWT_RxFail || stTxLevelGWT_Int.bBGT_Fail || stTxLevelGWT_Int.bBWT_Fail)
    {
        if (stTxLevelGWT_Int.bTxLevelInt)
        {
            u16ISRTXLevel++;
        }
        if (stTxLevelGWT_Int.bCWT_RxFail)  //CWT RX INT
        {
            if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8WT_RxIntCnt++;
            if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CWT_RxIntCnt++;
        }
        if (stTxLevelGWT_Int.bCWT_TxFail)  //CWT TX INT
        {
            if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8WT_TxIntCnt++;
            if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CWT_TxIntCnt++;
        }
#if PATCH_TO_DISABLE_CGT //Iverlin: Patch to ignore CGT sent from card
        if (IS_RX_CGT_CHECK_DISABLE == FALSE)
        {
            if (stTxLevelGWT_Int.bCGT_RxFail)  //CGT RX INT
            {
                if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8GT_RxIntCnt++;
                if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CGT_RxIntCnt++;
            }
        }
#endif
        if (stTxLevelGWT_Int.bCGT_TxFail)  //CGT TX INT
        {
            if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].u8GT_TxIntCnt++;
            if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].u8CGT_TxIntCnt++;
        }
        if (stTxLevelGWT_Int.bBGT_Fail)  //BGT INT
        {
            _sc_T1State[u8SCID].u8BGT_IntCnt++;
        }
        if (stTxLevelGWT_Int.bBWT_Fail)  //BWT INT
        {
            //If BWT works as ext-CWT, to clear 0x1029_0x0A[7] to prevnet BWT fail int is always triggered
            if (_sc_T0State[u8SCID].bUseBwtInsteadExtCwt)
            {
                HAL_SC_RstToIoEdgeDetCtrl(u8SCID, FALSE);
            }
            _sc_T1State[u8SCID].u8BWT_IntCnt++;
        }

        // Clear int flag
        HAL_SC_ClearIntTxLevelAndGWT(u8SCID);
        if (stTxLevelGWT_Int.bTxLevelInt)
        {
            while(1)
            {
                if (_scInfo[u8SCID].u16FifoTxRead == _scInfo[u8SCID].u16FifoTxWrite)
                    break;
                HAL_SC_WriteTxData(u8SCID, _scInfo[u8SCID].u8FifoTx[_scInfo[u8SCID].u16FifoTxRead++]);
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
                    if (i >= 16)
                        break;
                }
            }
        }
    }

    //Check HW Rst to IO fail
    if (_gstHwFeature.bRstToIoEdgeDet == TRUE)
    {
        if (HAL_SC_CheckIntRstToIoEdgeFail(u8SCID))
        {
            HAL_SC_SetIntRstToIoEdgeFail(u8SCID, FALSE); //Mask int
            _gastRstToAtrCtrl[u8SCID].bRstToAtrTimeout = TRUE;
        }
    }

    // Receive all data in fifo
    while (1)
    {
        if (HAL_SC_IsRxDataReady(u8SCID))
        {
            int         tmp;

            _scInfo[u8SCID].u8FifoRx[_scInfo[u8SCID].u16FifoRxWrite] = HAL_SC_ReadRxData(u8SCID);
            if (HAL_SC_IsParityError(u8SCID))
            {
                if (_scInfo[u8SCID].u8Protocol == 0 || _scInfo[u8SCID].u8Protocol == 0xFF)    _sc_T0State[u8SCID].bParityError = TRUE;
                if (_scInfo[u8SCID].u8Protocol == 1)    _sc_T1State[u8SCID].bParityError = TRUE;
            }

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
#endif
    // Check special event from SMART
    HAL_SC_GetCardDetectInt(u8SCID, &stCardDetectInt);
    if (stCardDetectInt.bCardInInt || stCardDetectInt.bCardOutInt)
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
    MS_VIRT              u32BaseAddr;
    MS_PHY               u32BaseSize;
    MS_BOOL              bIsInitialized = FALSE;
#if defined(MSOS_TYPE_LINUX)
    MS_U32 u32ShmId, u32Addr, u32BufSize;
    MS_U32 bInitialized;
    MS_U8 au8ShmSmc1[] = SC_SHMSMC1;
#if (SC_DEV_NUM > 1)
    MS_U8 au8ShmSmc2[] = SC_SHMSMC2;
#endif
    MS_U8 *pu8ShmSmc = NULL;
#endif

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (u8SCID >= SC_DEV_NUM)
    {
        return E_SC_PARMERROR;
    }

    if (_scInfo[u8SCID].bInited)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "SMC%d has been initialized\n", u8SCID);
        return E_SC_OK;
    }

    bIsInitialized = _scInfo[0].bInited;
#if (SC_DEV_NUM > 1)
    bIsInitialized |= _scInfo[1].bInited;
#endif

    if (!bIsInitialized)
    {
        if (_SC_SetupHwFeature(HAL_SC_GetHwVersion()) != E_SC_OK)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "Invalid HW version: 0x%X!!\n", HAL_SC_GetHwVersion());
            return E_SC_FAIL;
        }

        if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_SC))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "MDrv_MMIO_GetBASE fail\n");
            return E_SC_FAIL;
        }
        HAL_SC_RegMap(u32BaseAddr);

#if (SC_DEV_NUM > 1)
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
    }

    _scInfo[u8SCID].bInited = FALSE;

    // Initialize OS SC device --------------------------------------------------------------------
    if (OS_SC_Init(u8SCID) == FALSE)
    {
        return E_SC_FAIL;
    }
#if defined(MSOS_TYPE_LINUX)
    OS_SC_SHM_LOCK();
    // return 0 for first initiaization, 1 for repeated initialization (existence shm), so bInitialized==0 is first initial, bInitialized==1 means this shm created before
    /*
        *  u32QueryRet: 0 for not found
        *                       1 for existence
        * u32ShmId:      0 for not found
        *                       n for created share mem id, start from 1
    */
    if (u8SCID == 0)
    {
        pu8ShmSmc = au8ShmSmc1;
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        pu8ShmSmc = au8ShmSmc2;
    }
#endif

    bInitialized = MsOS_SHM_GetId((MS_U8*)pu8ShmSmc, sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);

    if (!bInitialized)
    {
        if(FALSE == MsOS_SHM_GetId((MS_U8*)pu8ShmSmc, sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
        {
            printf("smc create shm failed\n");
            OS_SC_SHM_UNLOCK();
            return E_SC_FAIL;
        }
        *(MS_U32*)u32Addr = SC_PROC_LOCK;
    }
    else
    {
        if (*(MS_U32*)u32Addr == SC_PROC_LOCK)
        {
            printf("Another process occupy the smc resource, so exit here\n");
            OS_SC_SHM_UNLOCK();
            return E_SC_FAIL;
        }
        else
        {
            *(MS_U32*)u32Addr = SC_PROC_LOCK;
        }
    }

    OS_SC_SHM_UNLOCK();
#endif

    // Open related device
#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    if (u8SCID == 0)
    {
        if (0 > (_scInfo[u8SCID].s32DevFd = open(SM0_DEV_NAME, O_RDWR)))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] %s open fail\n", __FUNCTION__, __LINE__, "/dev/smart");
            return E_SC_FAIL;
        }
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        if (0 > (_scInfo[u8SCID].s32DevFd = open(SM1_DEV_NAME, O_RDWR)))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] %s open fail\n", __FUNCTION__, __LINE__, "/dev/smart1");
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
    MS_U32 u32ShmId, u32Addr, u32BufSize, bInitialized;
    MS_U8 au8ShmSmc1[] = SC_SHMSMC1;
#if (SC_DEV_NUM > 1)
    MS_U8 au8ShmSmc2[] = SC_SHMSMC2;
#endif
    MS_U8 *pu8ShmSmc = NULL;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    if (_scInfo[u8SCID].bInited == FALSE)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "Not initial\n");
        return E_SC_FAIL;
    }

    if (u8SCID == 0)
    {
        pu8ShmSmc = au8ShmSmc1;
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        pu8ShmSmc = au8ShmSmc2;
    }
#endif

    bInitialized = MsOS_SHM_GetId((MS_U8*)pu8ShmSmc, sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);
    if (bInitialized != 0)
    {
        *(MS_U32*)u32Addr = SC_PROC_UNLOCK;
    }

    HAL_SC_Exit(u8SCID);

#if (SC_DEV_NUM > 1)
    if (_scInfo[0].bInited != _scInfo[1].bInited)
#endif
    {
        if (OS_SC_Exit(u8SCID) == FALSE)
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] Failed to close _OS_SC_Task!!\n", __FUNCTION__, __LINE__);
            return E_SC_FAIL;
        }
    }

    _scInfo[u8SCID].bInited = FALSE;

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR)
    close(_scInfo[u8SCID].s32DevFd);
    _scInfo[u8SCID].s32DevFd = -1;
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
    HAL_SC_OPEN_SETTING stOpenSetting;
    HAL_SC_FIFO_CTRL stCtrlFIFO;
    HAL_SC_MODE_CTRL stModeCtrl;

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
    _scInfoExt[u8SCID].eVoltage = pParam->eVoltage;
    _scInfo[u8SCID].pfNotify = pfSmartNotify;
    _scInfo[u8SCID].u8Fi = 1; // assigned by ATR
    _scInfo[u8SCID].u8Di = 1;
    _scInfoExt[u8SCID].u8N = 0;
    _SC_ClearProtocolState(u8SCID);

    if (_scInfo[u8SCID].u8Convention == 0)
    {
        stOpenSetting.eConvCtrl = E_HAL_SC_CONV_DIRECT;
    }
    else
    {
        stOpenSetting.eConvCtrl = E_HAL_SC_CONV_INVERSE;
    }
    stOpenSetting.eVccCtrlType = _SC_ConvHalVccCtrl(_scInfo[u8SCID].eVccCtrl);
    stOpenSetting.eVoltageCtrl = _SC_ConvHalVoltageCtrl(_scInfoExt[u8SCID].eVoltage);
    stOpenSetting.eClkCtrl = _SC_ConvHalClkCtrl(_scInfo[u8SCID].eCardClk);
    stOpenSetting.u8UartMode = _scInfo[u8SCID].u8UartMode;
    stOpenSetting.u16ClkDiv = _scInfo[u8SCID].u16ClkDiv;
    stOpenSetting.eCardDetType = _SC_ConvHalCardDetType(pParam->u8CardDetInvert);
    if (HAL_SC_Open(u8SCID, &stOpenSetting) == FALSE)
    {
        OS_SC_RETURN(E_SC_FAIL);
    }

    // Clear Interrupt
    HAL_SC_ClearCardDetectInt(u8SCID, E_HAL_SC_CD_ALL_CLEAR);

    // UART init ----------------------------------------------------------------------------------

    // Disable all interrupts                                           // Interrupt Enable Register
    HAL_SC_SetUartInt(u8SCID, E_HAL_SC_UART_INT_DISABLE);

    // Reset receiver and transmiter                                    // FIFO Control Register
    memset(&stCtrlFIFO, 0x00, sizeof(stCtrlFIFO));
    stCtrlFIFO.bEnableFIFO = TRUE;
    stCtrlFIFO.eTriLevel = E_HAL_SC_RX_FIFO_INT_TRI_1;
    HAL_SC_SetUartFIFO(u8SCID, &stCtrlFIFO);

    stModeCtrl.bFlowCtrlEn = TRUE;
    stModeCtrl.bParityChk = TRUE;
    stModeCtrl.bSmartCardMdoe = TRUE;
    stModeCtrl.u8ReTryTime = 0x4;
    HAL_SC_SetSmcModeCtrl(u8SCID, &stModeCtrl); // 0x64;

    // Check Rx FIFO empty
    while (HAL_SC_IsRxDataReady(u8SCID))
    {
        HAL_SC_ReadRxData(u8SCID);
        OS_SC_DELAY(1);
    }

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
    if (_SC_SetupUartInt(u8SCID) != E_SC_OK)
    {
        HAL_SC_Close(u8SCID, _SC_ConvHalVccCtrl(_scInfo[u8SCID].eVccCtrl));
        OS_SC_RETURN(E_SC_FAIL);
    }
#endif
    _scInfo[u8SCID].bOpened = TRUE;

#if defined(MSOS_TYPE_LINUX) && defined(SC_KERNEL_ISR) // to do the first activation flow, without reset
    if (HAL_SC_IsCardDetected(u8SCID))
    {
        _scInfo[u8SCID].bLastCardIn = FALSE;
        _scInfo[u8SCID].bCardIn = TRUE;

        //Set event to SC Linux driver and make MDrv_SC_Task_Proc can get card_in event from driver
        MS_U32 u32Evt = E_SC_INT_CARD_IN;
        if (0 != ioctl(_scInfo[u8SCID].s32DevFd, MDRV_SC_SET_EVENTS, (int *)&u32Evt))
        {
            SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] ioctl: MDRV_SC_SET_EVENTS failed\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        _scInfo[u8SCID].bLastCardIn = FALSE;
    }
#else
    if (HAL_SC_IsCardDetected(u8SCID))
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
        HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode);
    }

    // Clear LSR register
    _SC_ClearState(u8SCID);
    _SC_ClearProtocolState(u8SCID);

    if (_SC_Reset(u8SCID) != E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Reset fail\n");
        OS_SC_RETURN(E_SC_FAIL);
    }

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

#if PATCH_TO_DISABLE_CGT
        DISABLE_RX_CGT_CHECK;
#endif
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
        HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode);
    }
    else
    {
        _scInfo[u8SCID].eCardClk = E_SC_CLK_4P5M;
        _scInfo[u8SCID].u16ClkDiv = 372;
       _SC_SetUartDiv(u8SCID);

        _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
        HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode);

    }

    _sc_T1State[u8SCID].u8NS = 0;
    _sc_T1State[u8SCID].u8NR = 0;
    _sc_T1State[u8SCID].ubMore = 0;

    _SC_ClearState(u8SCID);
    _SC_ClearProtocolState(u8SCID);

    if (_SC_Reset(u8SCID) != E_SC_OK)
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "    Reset fail\n");
        OS_SC_RETURN(E_SC_FAIL);
    }

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

#if PATCH_TO_DISABLE_CGT
        DISABLE_RX_CGT_CHECK;
#endif
        // suppose all mode need to go here, check later
        // if don't call it, healthy card abnormal when on chip 8024 mode
        {
            if (_scInfo[u8SCID].pu8Atr[0] == 0x3b)
            {
                _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN;
                HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode);
            }
            else if (_scInfo[u8SCID].pu8Atr[0] == 0x3f)
            {
                _scInfo[u8SCID].u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
                HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode);
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
                _SC_Reset(u8SCID);
                _SC_GetATR(u8SCID, SC_T0_ATR_TIMEOUT * 5);
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
        if (!HAL_SC_SetUartMode(u8SCID, _scInfo[u8SCID].u8UartMode))
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
    int                 pos = 0, LcLe = 0;

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
        int             i;
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
        int i = 0;
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
        int             i;
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

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    while (1)
    {
        SC_Result eResult = E_SC_FAIL;

        if (!HAL_SC_IsCardDetected(u8SCID))
        // if (!_scInfo[u8SCID].bCardIn)
        {
            printf("[%s][%d] card out\n", __FUNCTION__, __LINE__);
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

        if (!HAL_SC_IsCardDetected(u8SCID))
        // if (!_scInfo[u8SCID].bCardIn)
        {
            printf("[%s][%d] card out\n", __FUNCTION__, __LINE__);
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
            if (!_Get1Byte(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
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
            if (!_Get1Byte(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
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
            if (!_Get1Byte(u8SCID,&u8Temp1, &u16Len,MsOS_GetSystemTime(),SC_T0_RECV_TIMEOUT))
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
        int i = 0;
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
        int             i;
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
    if (((eResult = _SC_Send(u8SCID, pu8SendData, 4, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        goto clear_fifo; // FAIL
    }

    //send Lc
    MS_U8 u8Temp1;
    if (u16Lc)
    {
        u8Temp1=(MS_U8)u16Lc;
    }
    else
    {
        u8Temp1=(MS_U8)u16Le;
    }
    if (((eResult = _SC_Send(u8SCID,&u8Temp1, 1, SC_T0_SEND_TIMEOUT)) != E_SC_OK))
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY,"%s, %d: Send LcLe Fail\n",__FUNCTION__,__LINE__);
        OS_SC_RETURN(eResult);
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
        int             i;
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
        int             i;
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
        _SC_T1IBlock( &_sc_T1State[u8SCID], _sc_T1State[u8SCID].ubMore,pu8SendData+u16SentIdx, u16BlockAPDULen,u8Block, &u8BlockLen );
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
                _SC_T1IBlock( &_sc_T1State[u8SCID],_sc_T1State[u8SCID].ubMore,pu8SendData+u16SentIdx,u16BlockAPDULen,u8Block, &u8BlockLen );
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
        int             i;
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
        int             i;
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

    if ((_atr_Fi[u8Fi] == RFU)||(_atr_Di[u8Di] == RFU))
    {
        return E_SC_FAIL;
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

    if ((_scInfo[u8SCID].u8Di == 1) && (_scInfo[u8SCID].u8Fi == 0 || _scInfo[u8SCID].u8Fi == 1))
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
/// @param eLevel       \b IN: E_SC_DBGLV_NONE/E_SC_DBGLV_ERR_ONLY/E_SC_DBGLV_REG_DUMP/E_SC_DBGLV_INFO/E_SC_DBGLV_ALL
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
    MS_VIRT u32BaseAddr;
    MS_PHY  u32BaseSize;
    int     i;

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

    HAL_SC_SetCGT(u8SCID, u8GuardTime);

    return E_SC_OK;
}

#ifdef SC_USE_IO_51
SC_Result MDrv_SC_SetBuffAddr(SC_BuffAddr *pScBuffAddr)
{
    gpScDataBuffPA = (SC_DataCfg*)pScBuffAddr->u32DataBuffAddr;
    gpScDataBuffVA = (SC_DataCfg*)_PA2VA(pScBuffAddr->u32DataBuffAddr);
    gScFwDataBuff =  pScBuffAddr->u32FwBuffAddr;
    //Todo: Check SC firmware valid
    SC_DBG(E_SC_DBGLV_INFO, " gpScDataBuffPA = 0x%lx\n", (MS_U32)gpScDataBuffPA);
    SC_DBG(E_SC_DBGLV_INFO, " gpScDataBuffVA = 0x%lx\n", (MS_U32)gpScDataBuffVA);
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
    SC_Result ret_Result = E_SC_OK;
    MS_U32 u32Timeout = E_INVALID_TIMEOUT_VAL;

    SC_DBG(E_SC_DBGLV_INFO, "%s\n", __FUNCTION__);

    ret_Result = _SC_RawExchange(u8SCID, pu8SendData, u16SendDataLen, pu8ReadData, u16ReadDataLen, u32Timeout);

    return ret_Result;
}

SC_Result MDrv_SC_RawExchangeTimeout(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs)
{
    SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] not support\n", __FUNCTION__, __LINE__);
    return E_SC_FAIL;
}
#endif

MS_BOOL MDrv_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage)
{
    HAL_SC_VCC_CTRL eHalVccCtrl;
    HAL_SC_VOLTAGE_CTRL eHalVoltageCtrl;
    MS_BOOL bRetVal;

    eHalVccCtrl = _SC_ConvHalVccCtrl(_scInfo[u8SCID].eVccCtrl);
    eHalVoltageCtrl = _SC_ConvHalVoltageCtrl(eVoltage);

    bRetVal = HAL_SC_CardVoltage_Config(u8SCID, eHalVoltageCtrl, eHalVccCtrl);
    if (bRetVal)
    {
        _scInfoExt[u8SCID].eVoltage = eVoltage;
    }
    else
    {
        SC_DBG(E_SC_DBGLV_ERR_ONLY, "[%s][%d] not support\n", __FUNCTION__, __LINE__);
    }

    return bRetVal;
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
           printf("%s %d,EN_POWER_MODE:%d \n",__FUNCTION__,__LINE__,(int)u16PowerState);
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
            printf("%s %d,EN_POWER_MODE:%d \n",__FUNCTION__,__LINE__,(int)u16PowerState);
            MDrv_SC_Init(0);
            MDrv_SC_Open(0, pParam.u8Protocal, &pParam,pfNotify);
            u16PreSCPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

static HAL_SC_VOLTAGE_CTRL _SC_ConvHalVoltageCtrl(SC_VoltageCtrl eVoltage)
{
    HAL_SC_VOLTAGE_CTRL eHalVoltageCtrl;

    switch(eVoltage)
    {
        /*case E_SC_VOLTAGE_3V:
            eHalVoltageCtrl = E_HAL_SC_VOL_CTRL_3V;
            break;

        case E_SC_VOLTAGE_1P8V:
            eHalVoltageCtrl = E_HAL_SC_VOL_CTRL_1P8V;
            break;*/

        case E_SC_VOLTAGE_3_POINT_3V:
            eHalVoltageCtrl = E_HAL_SC_VOL_CTRL_3P3V;
            break;

        case E_SC_VOLTAGE_5V:
            eHalVoltageCtrl = E_HAL_SC_VOL_CTRL_5V;
            break;

        default:
            eHalVoltageCtrl = E_HAL_SC_VOL_CTRL_INVALID;
            break;
    }

    return eHalVoltageCtrl;
}

static HAL_SC_VCC_CTRL _SC_ConvHalVccCtrl(SC_VccCtrl eVccCtrl)
{
    HAL_SC_VCC_CTRL eHalVccCtrl;

    switch(eVccCtrl)
    {
        case E_SC_VCC_CTRL_8024_ON:
        default:
            eHalVccCtrl = E_HAL_SC_VCC_EXT_8024;
            break;

        case E_SC_VCC_CTRL_LOW:
            eHalVccCtrl = E_HAL_SC_VCC_LOW;
            break;

        case E_SC_VCC_CTRL_HIGH:
            eHalVccCtrl = E_HAL_SC_VCC_HIGH;
            break;

        case E_SC_OCP_VCC_HIGH:
            eHalVccCtrl = E_HAL_SC_VCC_OCP_HIGH;
            break;

        case E_SC_VCC_VCC_ONCHIP_8024:
            eHalVccCtrl = E_HAL_SC_VCC_INT_8024;
            break;
    }

    return eHalVccCtrl;
}

static HAL_SC_CLK_CTRL _SC_ConvHalClkCtrl(SC_ClkCtrl eClkCtrl)
{
    HAL_SC_CLK_CTRL eHalClkCtrl;

    switch(eClkCtrl)
    {
        case E_SC_CLK_3M:
            eHalClkCtrl = E_HAL_SC_CLK_3M;
            break;

        case E_SC_CLK_4P5M:
            eHalClkCtrl = E_HAL_SC_CLK_4P5M;
            break;

        case E_SC_CLK_4M:
            eHalClkCtrl = E_HAL_SC_CLK_4M;
            break;

        case E_SC_CLK_6M:
            eHalClkCtrl = E_HAL_SC_CLK_6M;
            break;

        case E_SC_CLK_13M:
            eHalClkCtrl = E_HAL_SC_CLK_13M;
            break;

        default:
            eHalClkCtrl = E_HAL_SC_CLK_INVALID;
            break;
    }

    return eHalClkCtrl;
}

static HAL_SC_CARD_DET_TYPE _SC_ConvHalCardDetType(SC_CardDetType eCardDetType)
{
    HAL_SC_CARD_DET_TYPE eHalCardDetType;

    if (eCardDetType == E_SC_LOW_ACTIVE)
    {
        eHalCardDetType = E_SC_CARD_DET_LOW_ACTIVE;
    }
    else
    {
        eHalCardDetType = E_SC_CARD_DET_HIGH_ACTIVE;
    }

    return eHalCardDetType;
}
