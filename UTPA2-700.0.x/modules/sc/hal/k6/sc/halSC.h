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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_SC_H_
#define _HAL_SC_H_


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define SC_DEV_NUM                  2UL                                   // number of device
#define SC_IRQ                      E_INT_IRQ_SMART//E_IRQ_UART1
#define SC_IRQ2                     E_INT_IRQ_SMART1//E_IRQ_UART2

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SC_READ(id,addr)               ((!id)?UART1_READ(addr):UART2_READ(addr))
#define SC_WRITE(id,addr, val)         if((!id)){UART1_WRITE(addr, val);} else {UART2_WRITE(addr, val);}
#define SC_OR(id,addr, val)            if((!id)){UART1_OR(addr, val);}  else {UART2_OR(addr, val);}
#define SC_AND(id,addr, val)           if((!id)){UART1_AND(addr, val);} else {UART2_AND(addr, val);}
#define SC_XOR(id,addr, val)           if((!id)){UART1_XOR(addr, val);} else {UART2_XOR(addr, val);}

#define SC_3M_UARTDIV(d)            (((d*9UL/5UL)>>1)+((d*9UL/5UL)&0x1UL))        // 3M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_4P5M_UARTDIV(d)          (((d*6UL/5UL)>>1)+((d*6UL/5UL)&0x1UL))        // 4.5M/d=43.2M/(16*uart_div) => div=d*3/5
#define SC_6M_UARTDIV(d)            (((d*9UL/10UL)>>1)+((d*9UL/10UL)&0x1UL))      // 6M/d=43.2M/(16*uart_div) => div=d*9/20


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_SC_CGWT_INT_DISABLE,                      //Enable CGT and CWT int
    E_HAL_SC_CGWT_INT_ENABLE,                       //Disable CGT and CWT int
    E_HAL_SC_CWT_ENABLE_CGT_DISABLE,      //Enable CWT int and disable CGT
} HAL_SC_CGWT_INT;

typedef enum
{
    E_HAL_SC_BGWT_INT_DISABLE,                      //Enable BGT and BWT int
    E_HAL_SC_BGWT_INT_ENABLE,                       //Disable BGT and BWT int
    E_HAL_SC_BWT_ENABLE_BGT_DISABLE,      //Enable BWT int and disable BGT
} HAL_SC_BGWT_INT;

typedef enum
{
    E_HAL_SC_VCC_EXT_8024,                                ///< by external 8024 on
    E_HAL_SC_VCC_LOW,                                        ///< by direct Vcc (low active)
    E_HAL_SC_VCC_HIGH,                                       ///< by direct Vcc (high active)
    E_HAL_SC_VCC_OCP_HIGH,
    E_HAL_SC_VCC_INT_8024                              ///< by internal 8024 on
} HAL_SC_VCC_CTRL;

typedef enum
{
    E_HAL_SC_VOL_CTRL_3P3V,        ///<3.3V
    E_HAL_SC_VOL_CTRL_5V,          ///< 5V
    E_HAL_SC_VOL_CTRL_1P8V,        ///<1.8V
    E_HAL_SC_VOL_CTRL_3V,          ///<3V
    E_HAL_SC_VOL_CTRL_INVALID
} HAL_SC_VOLTAGE_CTRL;

/// SmartCard CLK setting
typedef enum
{
    E_HAL_SC_CLK_3M,                ///< 3 MHz
    E_HAL_SC_CLK_4P5M,              ///< 4.5 MHz
    E_HAL_SC_CLK_6M,                ///< 6 MHz
    E_HAL_SC_CLK_13M,               ///< 6 MHz
    E_HAL_SC_CLK_4M,                ///< 4 MHz
    E_HAL_SC_CLK_INVALID
} HAL_SC_CLK_CTRL;

typedef enum
{
    E_HAL_SC_CONV_DIRECT,           ///< Direct convention
    E_HAL_SC_CONV_INVERSE,          ///< Inverse convention
    E_HAL_SC_CONV_INVALID
} HAL_SC_CONV_CTRL;

typedef enum
{
    E_HAL_SC_UART_INT_DISABLE = 0x00,   //Clear all int
    E_HAL_SC_UART_INT_RDI = 0x01,       //Received Data Available Interrupt and Character Timeout Interrupt
    E_HAL_SC_UART_INT_THRI = 0x02,      //Transmitter Holding Register Empty Interrupt
    E_HAL_SC_UART_INT_RLSI = 0x04,      //Receiver Line Status Interrrupt
    E_HAL_SC_UART_INT_MSI = 0x08        //Modem Status interrupt
}HAL_SC_UART_INT_BIT_MAP;

typedef enum
{
    E_HAL_SC_LEVEL_LOW = FALSE,
    E_HAL_SC_LEVEL_HIGH = TRUE
}HAL_SC_LEVEL_CTRL;

typedef enum
{
    E_HAL_SC_IO_FUNC_ENABLE,            //As general I/O pin, idle is logical high
    E_HAL_SC_IO_FORCED_LOW              //I/O pin forced to logical low
}HAL_SC_IO_CTRL;

typedef enum
{
    E_HAL_SC_CD_IN_CLEAR,
    E_HAL_SC_CD_OUT_CLEAR,
    E_HAL_SC_CD_ALL_CLEAR
}HAL_SC_CD_INT_CLEAR;

typedef enum
{
    E_HAL_SC_RX_FIFO_INT_TRI_1,
    E_HAL_SC_RX_FIFO_INT_TRI_4,
    E_HAL_SC_RX_FIFO_INT_TRI_8,
    E_HAL_SC_RX_FIFO_INT_TRI_14
}HAL_SC_RX_FIFO_INT_TRI_LEVEL;

typedef enum
{
    E_HAL_SC_UART_INT_ID_CT,        //Character Timeout
    E_HAL_SC_UART_INT_ID_RLS,       //Receiver Line Status
    E_HAL_SC_UART_INT_ID_RDA,       //Receiver Data Available
    E_HAL_SC_UART_INT_ID_THRE,      //Transmitter Holding Register empty
    E_HAL_SC_UART_INT_ID_MS,        //Modem Status
    E_HAL_SC_UART_INT_ID_NONE       //No interrupt
}HAL_SC_UART_INT_ID;

typedef enum
{
    E_SC_CARD_DET_HIGH_ACTIVE,
    E_SC_CARD_DET_LOW_ACTIVE
}HAL_SC_CARD_DET_TYPE;

typedef struct
{
    MS_BOOL bCardInInt;
    MS_BOOL bCardOutInt;
}HAL_SC_CD_INT_STATUS;

typedef struct
{
    HAL_SC_RX_FIFO_INT_TRI_LEVEL eTriLevel;
    MS_BOOL bEnableFIFO;
    MS_BOOL bClearRxFIFO;
    MS_BOOL bClearTxFIFO;
}HAL_SC_FIFO_CTRL;

typedef struct
{
    MS_BOOL bFlowCtrlEn;
    MS_BOOL bParityChk;
    MS_BOOL bSmartCardMdoe;
    MS_U8 u8ReTryTime;
}HAL_SC_MODE_CTRL;

typedef struct
{
    MS_BOOL bTxLevelInt;
    MS_BOOL bCGT_TxFail;
    MS_BOOL bCGT_RxFail;
    MS_BOOL bCWT_TxFail;
    MS_BOOL bCWT_RxFail;
    MS_BOOL bBGT_Fail;
    MS_BOOL bBWT_Fail;
}HAL_SC_TX_LEVEL_GWT_INT;

typedef struct
{
    HAL_SC_VCC_CTRL eVccCtrlType;
    HAL_SC_VOLTAGE_CTRL eVoltageCtrl;
    HAL_SC_CLK_CTRL eClkCtrl;
    MS_U16 u16ClkDiv;
    MS_U8 u8UartMode;
    HAL_SC_CONV_CTRL eConvCtrl;
    HAL_SC_CARD_DET_TYPE eCardDetType;
}HAL_SC_OPEN_SETTING;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_VIRT   _regSCBase[SC_DEV_NUM]; // for DRV register access

void    HAL_SC_SetClk(MS_U8 u8SCID, HAL_SC_CLK_CTRL eClk);
MS_U32  HAL_SC_GetHwVersion(void);
void    HAL_SC_Init(MS_U8 u8SCID);
void    HAL_SC_Exit(MS_U8 u8SCID);
MS_BOOL HAL_SC_Open(MS_U8 u8SCID, HAL_SC_OPEN_SETTING *pstOpenSetting);
void HAL_SC_Close(MS_U8 u8SCID, HAL_SC_VCC_CTRL eVccCtrlType);
void    HAL_SC_RegMap(MS_VIRT u32RegBase);
void    HAL_SC1_RegMap(MS_VIRT u32RegBase);
void    HAL_SC_HW_RegMap(MS_VIRT u32RegBase);
void    HAL_SC_PowerCtrl(MS_U8 u8SCID, MS_BOOL bEnable);
void HAL_SC_CardInvert(MS_U8 u8SCID, HAL_SC_CARD_DET_TYPE eTpye);
MS_BOOL HAL_SC_CardVoltage_Config(MS_U8 u8SCID, HAL_SC_VOLTAGE_CTRL eVoltage, HAL_SC_VCC_CTRL eVccCtrl);
MS_BOOL HAL_SC_IsParityError(MS_U8 u8SCID);
MS_BOOL HAL_SC_IsRxDataReady(MS_U8 u8SCID);
MS_BOOL HAL_SC_IsTxFIFO_Empty(MS_U8 u8SCID);
void HAL_SC_WriteTxData(MS_U8 u8SCID, MS_U8 u8TxData);
MS_U8 HAL_SC_ReadRxData(MS_U8 u8SCID);
MS_BOOL HAL_SC_IsCardDetected(MS_U8 u8SCID);
void HAL_SC_SetIntCGWT(MS_U8 u8SCID, HAL_SC_CGWT_INT eIntCGWT);
void HAL_SC_SetIntBGWT(MS_U8 u8SCID, HAL_SC_BGWT_INT eIntBGWT);
void HAL_SC_GetIntTxLevelAndGWT(MS_U8 u8SCID, HAL_SC_TX_LEVEL_GWT_INT *pstTxLevelGWT);
void HAL_SC_ClearIntTxLevelAndGWT(MS_U8 u8SCID);
void HAL_SC_SetBWT(MS_U8 u8SCID, MS_U32 u32BWT_etu);
void HAL_SC_SetBGT(MS_U8 u8SCID, MS_U8 u8BGT_etu);
void HAL_SC_SetCWT(MS_U8 u8SCID, MS_U32 u32CWT_etu);
void HAL_SC_SetCGT(MS_U8 u8SCID, MS_U8 u8gCGT_etu);
MS_BOOL HAL_SC_SetUartDiv(MS_U8 u8SCID, HAL_SC_CLK_CTRL eClk, MS_U16 u16ClkDiv);
MS_BOOL HAL_SC_SetUartMode(MS_U8 u8SCID, MS_U8 u8UartMode);
MS_BOOL HAL_SC_SetConvention(MS_U8 u8SCID, HAL_SC_CONV_CTRL eConvCtrl);
MS_BOOL HAL_SC_SetInvConvention(MS_U8 u8SCID);
void HAL_SC_SetUartInt(MS_U8 u8SCID, MS_U8 u8IntBitMask);
HAL_SC_UART_INT_ID HAL_SC_GetUartIntID(MS_U8 u8SCID);
void HAL_SC_SetUartFIFO(MS_U8 u8SCID, HAL_SC_FIFO_CTRL *pstCtrlFIFO);
void HAL_SC_SetSmcModeCtrl(MS_U8 u8SCID, HAL_SC_MODE_CTRL *pstModeCtrl);
void HAL_SC_ClearCardDetectInt(MS_U8 u8SCID, HAL_SC_CD_INT_CLEAR eIntClear);
void HAL_SC_GetCardDetectInt(MS_U8 u8SCID, HAL_SC_CD_INT_STATUS *pstCardDetectInt);
void HAL_SC_ResetPadCtrl(MS_U8 u8SCID, HAL_SC_LEVEL_CTRL eLogicalLevel);
void HAL_SC_SmcVccPadCtrl(MS_U8 u8SCID, HAL_SC_LEVEL_CTRL eLogicalLevel);
void HAL_SC_InputOutputPadCtrl(MS_U8 u8SCID, HAL_SC_IO_CTRL eCtrl);
void HAL_SC_Ext8024ActiveSeq(MS_U8 u8SCID);
void HAL_SC_Ext8024DeactiveSeq(MS_U8 u8SCID);
void HAL_SC_Int8024ActiveSeq(MS_U8 u8SCID);
void HAL_SC_Int8024DeactiveSeq(MS_U8 u8SCID);
void HAL_SC_Int8024PullResetPadLow(MS_U8 u8SCID, MS_U32 u32HoldTimeInMs);
void HAL_SC_SmcClkCtrl(MS_U8 u8SCID, MS_BOOL bEnableClk);
void HAL_SC_PwrCutDeactiveCfg(MS_U8 u8SCID, MS_BOOL bVccOffPolHigh);
void HAL_SC_PwrCutDeactiveCtrl(MS_U8 u8SCID, MS_BOOL bEnable);
void HAL_SC_RxFailAlwaysDetCWT(MS_U8 u8SCID, MS_BOOL bEnable);
MS_BOOL HAL_SC_SetRstToIoTimeout(MS_U8 u8SCID, MS_U32 u32Timeout, MS_U32 u32CardClk);
void HAL_SC_RstToIoEdgeDetCtrl(MS_U8 u8SCID, MS_BOOL bEnable);
MS_BOOL HAL_SC_CheckIntRstToIoEdgeFail(MS_U8 u8SCID);
void HAL_SC_SetIntRstToIoEdgeFail(MS_U8 u8SCID, MS_BOOL bEnable);

#endif // _HAL_SC_H_
