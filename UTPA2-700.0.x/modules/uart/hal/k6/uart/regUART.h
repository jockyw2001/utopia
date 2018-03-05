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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
//
//  File name: regUART.h
//  Description: UART Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_UART_H_
#define _REG_UART_H_

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#if defined(MCU_AEON)
#define PIU_UART_VER                1
#endif

#if defined(__mips__)
#define PIU_UART_VER                2
#endif

#if defined(__arm__) || defined(__aarch64__)
#define PIU_UART_VER                2
#endif

#define UART_AEON_NUM               3
#define UART_PIU_NUM                3

#define UART_AEON_ADDR              0x90000000
#define UART_AEON_IRQ               E_INT_IRQ_UART0

#define UART0_PIU_ADDR              0x1300
#define UART0_PIU_IRQ               E_INT_IRQ_UART0

#define UART1_PIU_ADDR              0x20C00
#define UART1_PIU_IRQ               E_INT_IRQ_UART1

#define UART2_PIU_ADDR              0x20C80
#define UART2_PIU_IRQ               E_INT_IRQ_UART2

#define FUART_PIU_ADDR              0x20D00
#define FUART_PIU_IRQ               E_INT_IRQ_UART2MCU

#if defined(MCU_AEON)
#define UART_CLK_FREQ               172800000
#endif

#if defined(__mips__)
#define UART_CLK_FREQ               123000000
#endif

#if defined(__arm__) || defined(__aarch64__)
#ifdef CONFIG_FPGA
#define UART_CLK_FREQ               12000000
#else
#define UART_CLK_FREQ               123000000
#endif
#endif


#ifndef UART_CLK_FREQ
#define UART_CLK_FREQ               123000000//123000000
#endif

#ifndef UART_CLK_FREQ_172M
#define UART_CLK_FREQ_172M          172000000//172000000
#endif

#ifndef UART_CLK_FREQ_160M
#define UART_CLK_FREQ_160M          160000000//160000000
#endif

#ifndef UART_CLK_FREQ_144M
#define UART_CLK_FREQ_144M          144000000//144000000
#endif

#ifndef UART_CLK_FREQ_108M
#define UART_CLK_FREQ_108M          108000000//108000000
#endif
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//
// UART Register List
//

#if (PIU_UART_VER == 1)
#define UART_RX         0       // In:  Receive buffer (DLAB=0)
#define UART_TX         0       // Out: Transmit buffer (DLAB=0)
#define UART_DLL        0       // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM        1       // Out: Divisor Latch High (DLAB=1)
#define UART_IER        1       // Out: Interrupt Enable Register
#define UART_IIR        2       // In:  Interrupt ID Register
#define UART_FCR        2       // Out: FIFO Control Register
#define UART_LCR        3       // Out: Line Control Register
#define UART_MCR        4       // Out: Modem Control Register
#define UART_LSR        5       // In:  Line Status Register
#define UART_USR        7
#endif

#if (PIU_UART_VER == 2)
#define UART_RX        (0 * 2)  // In:  Receive buffer (DLAB=0)
#define UART_TX        (0 * 2)  // Out: Transmit buffer (DLAB=0)
#define UART_DLL       (0 * 2)  // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM       (1 * 2)  // Out: Divisor Latch High (DLAB=1)
#define UART_IER       (1 * 2)  // Out: Interrupt Enable Register
#define UART_IIR       (2 * 2)  // In:  Interrupt ID Register
#define UART_FCR       (2 * 2)  // Out: FIFO Control Register
#define UART_LCR       (3 * 2)  // Out: Line Control Register
#define UART_MCR       (4 * 2)	// Out: Modem Control Register
#define UART_LSR       (5 * 2)  // In:  Line Status Register
#define UART_MSR       (6 * 2)  // In:  Modem Status Register
#define UART_USR       (7 * 2)  // Out: USER Status Register
#endif

#define UART_FIFO                   1           // Divisor Latch Low
#define UART_EFR                    2              // I/O: Extended Features Register
#define UART_RB                     3           // optional: set rf_pop delay for memack ; [3:0] rf_pop_delay; [6] rf_pop_mode_sel ; [7] reg_rb_read_ack
                                                // (DLAB=1, 16C660 only)
#define UART_SCR                    7           // I/O: Scratch Register
#define UART_SCCR                   8           // Smartcard Control Register
#define UART_SCSR                   9           // Smartcard Status Register
#define UART_SCFC                   10          // Smartcard Fifo Count Register
#define UART_SCFI                   11          // Smartcard Fifo Index Register
#define UART_SCFR                   12          // Smartcard Fifo Read Delay Register
#define UART_SCMR                   13          // Smartcard Mode Register
#define UART_DL                     0           // Divisor Latch
#define UART_DL1_LSB                0           // Divisor Latch Low
#define UART_DL2_MSB                0           // Divisor Latch High

//
// UART_FCR(2)
// FIFO Control Register (16650 only)
//
#define UART_FCR_ENABLE_FIFO        0x01        // Enable the FIFO
#define UART_FCR_CLEAR_RCVR         0x02        // Clear the RCVR FIFO
#define UART_FCR_CLEAR_XMIT         0x04        // Clear the XMIT FIFO
#define UART_FCR_DMA_SELECT         0x08        // For DMA applications
#define UART_FCR_TRIGGER_MASK       0xC0        // Mask for the FIFO trigger range
//#define UART_FCR_TRIGGER_1          0x00        // Mask for trigger set at 1
//#define UART_FCR_TRIGGER_4          0x40        // Mask for trigger set at 4
#define UART_FCR_TRIGGER_8          0x80        // Mask for trigger set at 8
#define UART_FCR_TRIGGER_14         0xC0        // Mask for trigger set at 14

//
// UART_LCR(3)
// Line Control Register
// Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
// UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
//
#define UART_LCR_WLEN5              0x00        // Wordlength: 5 bits
#define UART_LCR_WLEN6              0x01        // Wordlength: 6 bits
#define UART_LCR_WLEN7              0x02        // Wordlength: 7 bits
#define UART_LCR_WLEN8              0x03        // Wordlength: 8 bits
#define UART_LCR_STOP1              0x00        // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_STOP2              0x04        // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_PARITY             0x08        // Parity Enable
#define UART_LCR_EPAR               0x10        // Even parity select
#define UART_LCR_SPAR               0x20        // Stick parity (?)
#define UART_LCR_SBC                0x40        // Set break control
#define UART_LCR_DLAB               0x80        // Divisor latch access bit

// UART_FCR(4)
// FIFO Control Register (16650 only)
#define UART_FCR_MASK               0x00FF
#define UART_FCR_RXFIFO_CLR         0x0001
#define UART_FCR_TXFIFO_CLR         0x0002
#define UART_FCR_TRIGGER_0          0x0000
#define UART_FCR_TRIGGER_1          0x0010
#define UART_FCR_TRIGGER_2          0x0020
#define UART_FCR_TRIGGER_3          0x0030
#define UART_FCR_TRIGGER_4          0x0040
#define UART_FCR_TRIGGER_5          0x0050
#define UART_FCR_TRIGGER_6          0x0060
#define UART_FCR_TRIGGER_7          0x0070

//
// UART_LSR(5)
// Line Status Register
//
#define UART_LSR_DR                 0x01          // Receiver data ready
#define UART_LSR_OE                 0x02          // Overrun error indicator
#define UART_LSR_PE                 0x04          // Parity error indicator
#define UART_LSR_FE                 0x08          // Frame error indicator
#define UART_LSR_BI                 0x10          // Break interrupt indicator
#define UART_LSR_THRE               0x20          // Transmit-hold-register empty
#define UART_LSR_TEMT               0x40          // Transmitter empty

// UART_LSR(6)
// Line Status Register
#define UART_LSR_TXFIFO_FULL        0x0080      //

//
// UART_IIR(2)
// Interrupt Identification Register
//

#define UART_IIR_MSI                0x00          // Modem status interrupt
#define UART_IIR_NO_INT             0x01          // No interrupts pending
#define UART_IIR_THRI               0x02          // Transmitter holding register empty
#define UART_IIR_TOI                0x0c          // Receive time out interrupt
#define UART_IIR_RDI                0x04          // Receiver data interrupt
#define UART_IIR_RLSI               0x06          // Receiver line status interrupt
#define UART_IIR_ID                 0x06          // Mask for the interrupt ID

//
// UART_IER(1)
// Interrupt Enable Register
//
#define UART_IER_RDI                0x01        // Enable receiver data available interrupt
#define UART_IER_THRI               0x02        // Enable Transmitter holding reg empty int
#define UART_IER_RLSI               0x04        // Enable receiver line status interrupt
#define UART_IER_MSI                0x08        // Enable Modem status interrupt

// UART_IER(3)
// Interrupt Enable Register
#define UART_IER_MASK               0xFF00
#define UART_IER_RDA                0x0100      // Enable receiver data available interrupt
#define UART_IER_THRE               0x0200      // Enable Transmitter holding reg empty int

//
// UART_MCR(4)
// Modem Control Register
//
#define UART_MCR_DTR                0x01        // DTR complement
#define UART_MCR_RTS                0x02        // RTS complement
#define UART_MCR_OUT1               0x04        // Out1 complement
#define UART_MCR_OUT2               0x08        // Out2 complement
#define UART_MCR_LOOP               0x10        // Enable loopback test mode

#define UART_MCR_FAST               0x20        // Slow / Fast baud rate mode

// UART_LCR(5)  //2008/07/09 Nick
#define UART_LCR_MASK               0xFF00
#define UART_LCR_CHAR_BITS_5        0x0000      // Wordlength: 5 bits
#define UART_LCR_CHAR_BITS_6        0x0100      // Wordlength: 6 bits
#define UART_LCR_CHAR_BITS_7        0x0200      // Wordlength: 7 bits
#define UART_LCR_CHAR_BITS_8        0x0300      // Wordlength: 8 bits
#define UART_LCR_STOP_BITS_1        0x0000      // 1 bit
#define UART_LCR_STOP_BITS_2        0x0400      // 1.5, 2 bit
#define UART_LCR_PARITY_EN          0x0800      // Parity Enable
#define UART_LCR_EVEN_PARITY_SEL    0x1000      // Even parity select
#define UART_LCR_DIVISOR_EN         0x8000      // Divisor latch access bit

//
// UART_MSR(6)
// Modem Status Register
//
#define UART_MSR_ANY_DELTA          0x0F        // Any of the delta bits!
#define UART_MSR_DCTS               0x01        // Delta CTS
#define UART_MSR_DDSR               0x02        // Delta DSR
#define UART_MSR_TERI               0x04        // Trailing edge ring indicator
#define UART_MSR_DDCD               0x08        // Delta DCD
#define UART_MSR_CTS                0x10        // Clear to Send
#define UART_MSR_DSR                0x20        // Data Set Ready
#define UART_MSR_RI                 0x40        // Ring Indicator
#define UART_MSR_DCD                0x80        // Data Carrier Detect

//
// UART_EFR(2, UART_LCR_DLAB)
// These are the definitions for the Extended Features Register
// (StarTech 16C660 only, when DLAB=1)
//
#define UART_EFR_ENI                0x10        // Enhanced Interrupt
#define UART_EFR_SCD                0x20        // Special character detect
#define UART_EFR_RTS                0x40        // RTS flow control
#define UART_EFR_CTS                0x80        // CTS flow control

//
// UART_SCCR(8)
// SmartCard Control Register
//
#define UART_SCCR_MASK_CARDIN       0x01        // Smartcard card in interrupt mask
#define UART_SCCR_MASK_CARDOUT      0x02        // Smartcard card out interrupt mask
#define UART_SCCR_TX_BINV           0x04        // Smartcard Tx bit invert
#define UART_SCCR_TX_BSWAP          0x08        // Smartcard Tx bit swap
#define UART_SCCR_RST               0x10        // Smartcard reset 0->1, UART Rx enable 1
#define UART_SCCR_RX_BINV           0x20        // Smartcard Rx bit inverse
#define UART_SCCR_RX_BSWAP          0x40        // Smartcard Rx bit swap

//
// UART_SCSR(9)
// Smartcard Status Register
//
#define UART_SCSR_CLK               0x01        // Smartcard clock out
#define UART_SCSR_INT_CARDIN        0x02        // Smartcard card in interrupt
#define UART_SCSR_INT_CARDOUT       0x04        // Smartcard card out interrupt
#define UART_SCSR_DETECT            0x08        // Smartcard detection status

//
// UART_SCFC(10), UART_SCFI(11), UART_SCFR(12)
// Smartcard Fifo Register
//
#define UART_SCFC_MASK              0x07
#define UART_SCFI_MASK              0x0F
#define UART_SCFR_MASK              0x07


//
// UART_SCFR(12)
// Smartcard Fifo Read Delay Register
#define UART_SCFR_DELAY_MASK        0x03
#define UART_SCFR_V_HIGH            0x04
#define UART_SCFR_V_ENABLE          0x08        // Vcc = (Vcc_high ^ (Vcc_en & UART_SCSR_INT_CARDOUT))

//
// UART_SCMR(13)
// SMart Mode Register
//
#define UART_SCMR_RETRY_MASK        0x1F
#define UART_SCMR_SMARTCARD         0x20
#define UART_SCMR_2STOP_BIT         0x40

//both Transmitter empty / Transmit-hold-register empty
#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

// UART_SEL_TYPE
#define UART_SEL_HK_R2          0x00
#define UART_SEL_VD_MHEG5       0x02
#define UART_SEL_TSP            0x03
#define UART_SEL_PIU_UART0      0x04
#define UART_SEL_PIU_UART1      0x05
#define UART_SEL_PIU_UART2      0x06
#define UART_SEL_PIU_FAST_UART  0x07
#define UART_SEL_DMD_MCU51_TXD0 0x08
#define UART_SEL_DMD_MCU51_TXD1 0x09
#define UART_SEL_VD_MCU51_TXD0  0x0A
#define UART_SEL_VD_MCU51_TXD1  0x0B
#define UART_SEL_AUDIO_R2       0x0C
#define UART_SEL_SECURE_R2      0x0D
#define UART_SEL_OFF            0x0F

#define BIT0    (0x0001)
#define BIT1    (0x0002)
#define BIT2    (0x0004)
#define BIT3    (0x0008)
#define BIT4    (0x0010)
#define BIT5    (0x0020)
#define BIT6    (0x0040)
#define BIT7    (0x0080)
#define BIT8    (0x0100)
#define BIT9    (0x0200)
#define BIT10   (0x0400)
#define BIT11   (0x0800)
#define BIT12   (0x1000)
#define BIT13   (0x2000)
#define BIT14   (0x4000)
#define BIT15   (0x8000)

//
// PM_SLEEP
// for UART RX enable and switch MCU & PM51
//
#define REG_PM_SLEEP 0x0E00
#define REG_PM_UART_CTRL (REG_PM_SLEEP + (0x09*2) + 1)
	#define _uart_rx_enable     BIT3
	#define _reg_hk51_uart0_en  BIT4

//
// CHIP_TOP
// for UART PAD Select
//
#define CHIP_REG_BASE 0x1E00
#define CHIP_REG_SecondUART                 (CHIP_REG_BASE+ (0x03*2))
    #define CHIP_SecondUART_PAD_1           (BIT0)
	#define CHIP_SecondUART_PAD_2           (BIT1)
	#define CHIP_SecondUART_PAD_3           (BIT0|BIT1)
	#define CHIP_SecondUART_PAD_MASK        (BIT0|BIT1)
#define CHIP_REG_ThirdUART                  (CHIP_REG_BASE+ (0x03*2))
    #define CHIP_ThirdUART_PAD_1            (BIT4)
	#define CHIP_ThirdUART_PAD_2            (BIT5)
	#define CHIP_ThirdUART_PAD_3            (BIT4|BIT5)
	#define CHIP_ThirdUART_PAD_MASK         (BIT4|BIT5)
#define CHIP_REG_FourthUART                 (CHIP_REG_BASE+ (0x03*2) +1 )
    #define CHIP_FourthUART_PAD_1           (BIT0)
	#define CHIP_FourthUART_PAD_2           (BIT1)
	#define CHIP_FourthUART_PAD_3           (BIT0|BIT1)
	#define CHIP_FourthUART_PAD_MASK        (BIT0|BIT1)
#define CHIP_REG_FastUART                   (CHIP_REG_BASE+ (0x09*2)+ 1)
    #define CHIP_FastUART_PAD_1             (BIT2)
	#define CHIP_FastUART_PAD_2             (BIT3)
	#define CHIP_FastUART_PAD_3             (BIT2|BIT3)
	#define CHIP_FastUART_PAD_MASK          (BIT2|BIT3)

#define UART_CLOCK_REG_BASE (0xB00)  
    #define UART0_CLK_REG     (UART_CLOCK_REG_BASE + 0x27)
    #define UART1_CLK_REG     (UART_CLOCK_REG_BASE + 0x28)
    #define UART2_CLK_REG     (UART_CLOCK_REG_BASE + 0x29)  
    #define FUART_CLK_REG     (UART_CLOCK_REG_BASE + 0x2E)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#endif // _REG_UART_H_
