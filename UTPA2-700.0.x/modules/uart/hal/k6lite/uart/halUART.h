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

#ifndef _HAL_TEMP_H_
#define _HAL_TEMP_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UART_DEVICE(_dev_)  ((struct uart_dev_struct *)(_dev_))

#define UNUSED( var )       (void)((var) = (var))

#define UART_DEV_MAX        20

#define TX_FLAG_DISABLED    0x0001
#define TX_FLAG_DIRECT      0x0002

#define RX_FLAG_DISABLED    0x0001
#define RX_FLAG_OVERFLOW    0x0002

#define IOCTL_SET_BAUDRATE          0
#define IOCTL_SET_RXCALLBACK        1
#define IOCTL_SET_RXCALLBACK_HALREG 2

#define UART_BASE       (u->regbase)

#define AEON_REG8(_x_)  ((MS_U8 volatile *)(UART_BASE))[(_x_)]
#define UART_REG8(_x_)  ((MS_U8 volatile *)(_gMIO_MapBase+UART_BASE))[((_x_) * 4) - ((_x_) & 1)]



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
struct uart_dev_struct;

typedef struct uart_dev_struct * ms_uart_dev_t;

struct uart_dev_struct
{
    struct device          device;
    void                   (*rx_callback)(int c);
    void                   (*rx_callback_halreg)(int c, MS_U32 pHalReg);
    MS_U32                 regbase;
    MS_U8                  *tx_buf;
    MS_U16                 tx_buf_in;
    volatile MS_U16        tx_buf_out;     // updated by ISR
    MS_U16                 tx_buf_len;
    MS_U16                 tx_flags;
    MS_U8                  *rx_buf;
    volatile MS_U16        rx_buf_in;      // updated by ISR
    MS_U16                 rx_buf_out;
    MS_U16                 rx_buf_len;
    MS_U16                 rx_flags;
    MS_U8                  uarttype ;
};

extern struct uart_dev_struct _uart_dev[UART_DEV_MAX] ;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_UART_SetIOMapBase(MS_VIRT u32Base, MS_VIRT u32PMBase);
MS_VIRT HAL_UART_GetIOMapBase(void);
MS_BOOL HAL_UART_SetPMRxEnable(MS_BOOL bEnable);
UART_DEVICE_TYPE HAL_UART_Get_Connection(UART_PORT_TYPE uart_port);
MS_U16 HAL_UART_Connect(UART_PORT_TYPE uart_port, UART_DEVICE_TYPE uart_dev);
MS_BOOL HAL_UART_SetPadMux(UART_PAD_TYPE uart_pad);
MS_U16 HAL_UART_Invert(UART_PORT_TYPE uart_port, MS_BOOL bInv);
int HAL_UART_Aeon_Close(device_t dev);
int HAL_UART_Aeon_Write(device_t dev, const char *buf, MS_U32 len, int off);
int HAL_UART_Aeon_Read(device_t dev, char *buf, MS_U32 len, int off);
int HAL_UART_Aeon_Poll(device_t dev, int types);
void HAL_UART_Aeon_Isr(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data);
int HAL_UART_Aeon_Ioctl(device_t dev, int request, unsigned long arg);
int HAL_UART_Aeon_Open(device_t dev, int mode);


int HAL_UART_PIU_Close(device_t dev);
int HAL_UART_PIU_Write(device_t dev, const char *buf, MS_U32 len, int off);
int HAL_UART_PIU_Read(device_t dev, char *buf, MS_U32 len, int off);
int HAL_UART_PIU_Poll(device_t dev, int types);
void HAL_UART0_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data);
void HAL_UART1_PIU_Isr(MS_U32 *pHalReg, MS_U32 u32Data);
int HAL_UART_PIU_Ioctl(device_t dev, int request, unsigned long arg);
int HAL_UART_PIU_Open(device_t dev, int mode);
void HAL_UART_SetUARTSecurityBank(UART_DEVICE_TYPE uart_dev);





#endif // _HAL_TEMP_H_
