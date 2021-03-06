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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SC_H_
#define _REG_SC_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_LINUX)
#define REG_SC_BASE1                 0x00102900UL
#define REG_SC_BASE2                 0x00102A00UL                  //not support
#define RIU_BUS_BASE                 0xFD000000UL
#else
#define REG_SC_BASE1                        (_regSCBase[0])
#define REG_SC_BASE2                        (_regSCBase[1])
#define RIU_BUS_BASE                        _regSCHWBase
#endif

#define REG_CHIP_TOP_BASE            0x00101E00UL
#define REG_CLK_GEN_BASE             0x00100B00UL
#define REG_CLK_GEN1_BASE            0x00103300UL
#define REG_CLK_GEN2_BASE            0x00100A00UL
#define REG_CHIP_GPIO_BASE           0x00102B00UL
// ------------------------------------------------------------------------------------------------
// UART / SC

#if !defined(MSOS_TYPE_LINUX)
#define UART1_READ(addr)                    READ_BYTE(RIU_BUS_BASE + ((REG_SC_BASE1 + ((addr) << 1)) << 1))
#define UART2_READ(addr)                    READ_BYTE(RIU_BUS_BASE + ((REG_SC_BASE2 + ((addr) << 1)) << 1))

#define UART1_WRITE(addr, val)              WRITE_BYTE(RIU_BUS_BASE + ((REG_SC_BASE1 + ((addr) << 1)) << 1), (val))
#define UART2_WRITE(addr, val)              WRITE_BYTE(RIU_BUS_BASE + ((REG_SC_BASE2 + ((addr) << 1)) << 1), (val))
#else
#define UART1_READ(addr)                    READ_BYTE(REG_SC_BASE1 + ((addr) << 2))
#define UART2_READ(addr)                    READ_BYTE(REG_SC_BASE2 + ((addr) << 2))

#define UART1_WRITE(addr, val)              WRITE_BYTE((REG_SC_BASE1 + ((addr) << 2)), (val))
#define UART2_WRITE(addr, val)              WRITE_BYTE((REG_SC_BASE2 + ((addr) << 2)), (val))
#endif


#define UART1_OR(addr, val)                 UART1_WRITE(addr, UART1_READ(addr) | (val))
#define UART1_AND(addr, val)                UART1_WRITE(addr, UART1_READ(addr) & (val))
#define UART1_XOR(addr, val)                UART1_WRITE(addr, UART1_READ(addr) ^ (val))
#define UART2_OR(addr, val)                 UART2_WRITE(addr, UART2_READ(addr) | (val))
#define UART2_AND(addr, val)                UART2_WRITE(addr, UART2_READ(addr) & (val))
#define UART2_XOR(addr, val)                UART2_WRITE(addr, UART2_READ(addr) ^ (val))
//
// UART Register List
//
#define UART_RX                             (0x00000000UL)                  // In:  Receive buffer (DLAB=0), 16-byte FIFO
#define UART_TX                             (0x00000000UL)                  // Out: Transmit buffer (DLAB=0), 16-byte FIFO
#define UART_DLL                            (0x00000000UL)                  // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM                            (0x00000001UL)                  // Out: Divisor Latch High (DLAB=1)
#define UART_IER                            (0x00000001UL)                  // Out: Interrupt Enable Register
#define UART_IIR                            (0x00000002UL)                  // In:  Interrupt ID Register
#define UART_FCR                            (0x00000002UL)                  // Out: FIFO Control Register
#define UART_EFR                            (0x00000002UL)                  // I/O: Extended Features Register
                                                                    // (DLAB=1, 16C660 only)
#define UART_LCR                            (0x00000003UL)                  // Out: Line Control Register
#define UART_MCR                            (0x00000004UL)                  // Out: Modem Control Register
#define UART_LSR                            (0x00000005UL)                  // In:  Line Status Register
#define UART_MSR                            (0x00000006UL)                  // In:  Modem Status Register
#define UART_SCR                            (0x00000007UL)                  // I/O: Scratch Register

#define UART_SCCR                           (0x00000008UL)                  // Smartcard Control Register
#define UART_SCSR                           (0x00000009UL)                  // Smartcard Status Register

#define UART_SCFC                           (0x0000000aUL)                  // Smartcard Fifo Count Register
#define UART_SCFI                           (0x0000000bUL)                  // Smartcard Fifo Index Register
#define UART_SCFR                           (0x0000000cUL)                  // Smartcard Fifo Read Delay Register

#define UART_SCMR                           (0x0000000dUL)                  // Smartcard Mode Register
#define UART_SCCGT                          (0x0000000eUL)                  // Smartcard char guard time - only after Kronus U02
#define UART_SCDBT                          (0x0000000fUL)                  // Smartcard debounce time
#define UART_SCCWT_L                        (0x00000010UL)                  // Smartcard char waiting time - LoByte
#define UART_SCCWT_H                        (0x00000011UL)                  // Smartcard char waiting time - HiByte
#define UART_SCBGT                          (0x00000012UL)                  // Smartcard block guard time
#define UART_SCBWT_0                        (0x00000014UL)                  // Smartcard block waiting time bit 07:00
#define UART_SCBWT_1                        (0x00000015UL)                  // Smartcard block waiting time bit 15:08
#define UART_SCBWT_2                        (0x00000016UL)                  // Smartcard block waiting time bit 23:16
#define UART_SCBWT_3                        (0x00000017UL)                  // Smartcard block waiting time bit 31:24
#define UART_CTRL2                          (0x00000018UL)                  // CGT/CWT/BGT/BWT int mask
#define UART_GWT_INT                        (0x00000019UL)                  // CGT/CWT/BGT/BWT int status
#define UART_TX_FIFO_COUNT                  (0x0000001aUL)                  // TX FIFO count
#define UART_DEACTIVE_RST                   (0x0000001bUL)                  // Set the active time for RST pin
#define UART_DEACTIVE_CLK                   (0x0000001cUL)                  // Set the active time for CLK pin
#define UART_DEACTIVE_IO                    (0x0000001dUL)                  // Set the active time for IO pin
#define UART_DEACTIVE_VCC                   (0x0000001eUL)                  // Set the active time for VCC pin
#define UART_CTRL3                          (0x0000001fUL)                  // ctrl3, bit0 use to enable the de-active sequence func
#define UART_ACTIVE_RST                     (0x00000020UL)                  // Set the active time for RST pin
#define UART_ACTIVE_CLK                     (0x00000021UL)                  // Set the active time for CLK pin
#define UART_ACTIVE_IO                      (0x00000022UL)                  // Set the active time for IO pin
#define UART_ACTIVE_VCC                     (0x00000023UL)                  // Set the active time for VCC pin
#define UART_CTRL4                          (0x00000024UL)                  // control4 reg
#define UART_CTRL5                          (0x00000025UL)                  // control5 reg
#define UART_DEBUG4                         (0x00000026UL)                  // debug4 reg
#define UART_CTRL6                          (0x00000027UL)                  // control6 reg
#define UART_SHORT_CIRCUT_DB_TIME_L         (0x00000028UL)                  // short_circuit_debounce_time_low
#define UART_SHORT_CIRCUT_DB_TIME_H         (0x00000029UL)                  // short_circuit_debounce_time_high
#define UART_SMC_ATOP_LOW                   (0x0000002aUL)                  // reg_test_smc_atop_low
#define UART_SMC_ATOP_HIGH                  (0x0000002bUL)                  // reg_test_smc_atop_high
#define UART_CTRL7                          (0x0000002cUL)                  // control7 reg
#define UART_ANALOG_STATUS                  (0x0000002dUL)                  // analog_status reg
#define UART_CTRL8                          (0x0000002eUL)                  // control8 reg
#define UART_CTRL9                          (0x0000002fUL)                  // control9 reg
#define UART_CTRL10                         (0x00000030UL)                  // control10 reg
#define UART_CTRL11                         (0x00000031UL)                  // control11 reg
#define UART_CTRL12                         (0x00000032UL)                  // control12 reg
#define UART_CTRL13                         (0x00000033UL)                  // control13 reg
#define UART_CTRL14                         (0x00000034UL)                  // control14 reg
#define UART_ANALOG_STATUS2                 (0x00000035UL)                  // analog_status2 reg


//
// UART_FCR(2)
// FIFO Control Register (16650 only)
//
#define UART_FCR_ENABLE_FIFO                (0x00000001UL)                  // Enable the FIFO
#define UART_FCR_CLEAR_RCVR                 (0x00000002UL)                  // Clear the RCVR FIFO
#define UART_FCR_CLEAR_XMIT                 (0x00000004UL)                  // Clear the XMIT FIFO
#define UART_FCR_DMA_SELECT                 (0x00000008UL)                  // For DMA applications
#define UART_FCR_TRIGGER_MASK               (0x000000C0UL)                  // Mask for the FIFO trigger range
#define UART_FCR_TRIGGER_1                  (0x00000000UL)                  // Mask for trigger set at 1
#define UART_FCR_TRIGGER_4                  (0x00000040UL)                  // Mask for trigger set at 4
#define UART_FCR_TRIGGER_8                  (0x00000080UL)                  // Mask for trigger set at 8
#define UART_FCR_TRIGGER_14                 (0x000000C0UL)                  // Mask for trigger set at 14

//
// UART_LCR(3)
// Line Control Register
// Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
// UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
//
#define UART_LCR_WLEN5                      (0x00000000UL)              // Wordlength: 5 bits
#define UART_LCR_WLEN6                      (0x00000001UL)              // Wordlength: 6 bits
#define UART_LCR_WLEN7                      (0x00000002UL)              // Wordlength: 7 bits
#define UART_LCR_WLEN8                      (0x00000003UL)              // Wordlength: 8 bits
#define UART_LCR_STOP1                      (0x00000000UL)              //
#define UART_LCR_STOP2                      (0x00000004UL)              // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_PARITY                     (0x00000008UL)              // Parity Enable
#define UART_LCR_EPAR                       (0x00000010UL)              // Even parity select
#define UART_LCR_SPAR                       (0x00000020UL)              // Stick parity (?)
#define UART_LCR_SBC                        (0x00000040UL)              // Set break control
#define UART_LCR_DLAB                       (0x00000080UL)              // Divisor latch access bit

//
// UART_LSR(5)
// Line Status Register
//
#define UART_LSR_DR                         (0x00000001UL)              // Receiver data ready
#define UART_LSR_OE                         (0x00000002UL)              // Overrun error indicator
#define UART_LSR_PE                         (0x00000004UL)              // Parity error indicator
#define UART_LSR_FE                         (0x00000008UL)              // Frame error indicator
#define UART_LSR_BI                         (0x00000010UL)              // Break interrupt indicator
#define UART_LSR_THRE                       (0x00000020UL)              // Transmit-hold-register empty
#define UART_LSR_TEMT                       (0x00000040UL)              // Transmitter empty

//
// UART_IIR(2)
// Interrupt Identification Register
//
#define UART_IIR_NO_INT                     (0x00000001UL)              // No interrupts pending
#define UART_IIR_ID                         (0x0000000EUL)              // Mask for the interrupt ID

#define UART_IIR_MSI                        (0x00000000UL)              // Modem status interrupt
#define UART_IIR_THRI                       (0x00000002UL)              // Transmitter holding register empty
#define UART_IIR_TOI                        (0x0000000cUL)              // Receive time out interrupt
#define UART_IIR_RDI                        (0x00000004UL)              // Receiver data interrupt
#define UART_IIR_RLSI                       (0x00000006UL)              // Receiver line status interrupt

//
// UART_IER(1)
// Interrupt Enable Register
//
#define UART_IER_RDI                        (0x00000001UL)              // Enable receiver data available interrupt
#define UART_IER_THRI                       (0x00000002UL)              // Enable Transmitter holding reg empty int
#define UART_IER_RLSI                       (0x00000004UL)              // Enable receiver line status interrupt
#define UART_IER_MSI                        (0x00000008UL)              // Enable Modem status interrupt

//
// UART_MCR(4)
// Modem Control Register
//
#define UART_MCR_DTR                        (0x00000001UL)              // DTR complement
#define UART_MCR_RTS                        (0x00000002UL)              // RTS complement
#define UART_MCR_OUT1                       (0x00000004UL)              // Out1 complement
#define UART_MCR_OUT2                       (0x00000008UL)              // Out2 complement
#define UART_MCR_LOOP                       (0x00000010UL)              // Enable loopback test mode

#define UART_MCR_FAST                       (0x00000020UL)              // Slow / Fast baud rate mode

//
// UART_MSR(6)
// Modem Status Register
//
#define UART_MSR_ANY_DELTA                  (0x0000000FUL)             // Any of the delta bits!
#define UART_MSR_DCTS                       (0x00000001UL)             // Delta CTS
#define UART_MSR_DDSR                       (0x00000002UL)             // Delta DSR
#define UART_MSR_TERI                       (0x00000004UL)             // Trailing edge ring indicator
#define UART_MSR_DDCD                       (0x00000008UL)             // Delta DCD
#define UART_MSR_CTS                        (0x00000010UL)             // Clear to Send
#define UART_MSR_DSR                        (0x00000020UL)             // Data Set Ready
#define UART_MSR_RI                         (0x00000040UL)             // Ring Indicator
#define UART_MSR_DCD                        (0x00000080UL)             // Data Carrier Detect

//
// UART_EFR(2, UART_LCR_DLAB)
// These are the definitions for the Extended Features Register
// (StarTech 16C660 only, when DLAB=1)
//
#define UART_EFR_ENI                        (0x00000010UL)             // Enhanced Interrupt
#define UART_EFR_SCD                        (0x00000020UL)             // Special character detect
#define UART_EFR_RTS                        (0x00000040UL)             // RTS flow control
#define UART_EFR_CTS                        (0x00000080UL)             // CTS flow control

//
// UART_SCCR(8)
// SmartCard Control Register
//
#define UART_SCCR_MASK_CARDIN               (0x00000001UL)             // Smartcard card in interrupt mask
#define UART_SCCR_MASK_CARDOUT              (0x00000002UL)             // Smartcard card out interrupt mask
#define UART_SCCR_TX_BINV                   (0x00000004UL)             // Smartcard Tx bit invert
#define UART_SCCR_TX_BSWAP                  (0x00000008UL)             // Smartcard Tx bit swap
#define UART_SCCR_RST                       (0x00000010UL)             // Smartcard reset 0->1, UART Rx enable 1
#define UART_SCCR_RX_BINV                   (0x00000020UL)             // Smartcard Rx bit inverse
#define UART_SCCR_RX_BSWAP                  (0x00000040UL)             // Smartcard Rx bit swap

//
// UART_SCSR(9)
// Smartcard Status Register
//
#define UART_SCSR_CLK                       (0x00000001UL)             // Smartcard clock out
#define UART_SCSR_INT_CARDIN                (0x00000002UL)             // Smartcard card in interrupt
#define UART_SCSR_INT_CARDOUT               (0x00000004UL)             // Smartcard card out interrupt
#define UART_SCSR_DETECT                    (0x00000008UL)             // Smartcard detection status
#define UART_SCSR_BGT_MASK                  (0x00000020UL)                                // Smartcard BGT interrupt mask
#define UART_SCSR_CWT_MASK                  (0x00000040UL)                              // Smartcard CWT interrupt mask
#define UART_SCSR_CGT_MASK                  (0x00000080UL)                               // Smartcard CGT interrupt mask

//
// UART_SCFC(10), UART_SCFI(11), UART_SCFR(12)
// Smartcard Fifo Register
//
#define UART_SCFC_MASK                      (0x00000007UL)
#define UART_SCFI_MASK                      (0x0000000FUL)
#define UART_SCFR_MASK                      (0x00000007UL)
#define UART_SCFC_RST_TO_IO_EDGE_DET_EN     (0x00000080UL)

//
// UART_SCFR(12)
// Smartcard Fifo Read Delay Register
//
#define UART_SCFR_DELAY_MASK                (0x00000003UL)
#define UART_SCFR_V_HIGH                    (0x00000004UL)
#define UART_SCFR_V_ENABLE                  (0x00000008UL)             // Vcc = (Vcc_high ^ (Vcc_en & UART_SCSR_INT_CARDOUT))
#define UART_SCFR_STOP_BIT_CNT_REACH_NDS_FLC_EN (0x00000080UL)

//
// UART_SCMR(13)
// SMart Mode Register
//
#define UART_SCMR_RETRY_MASK                (0x0000001FUL)
#define UART_SCMR_SMARTCARD                 (0x00000020UL)
#define UART_SCMR_PARITYCHK                 (0x00000040UL)
#define UART_SCMR_FLOWCTRL                  (0x00000080UL)             // control the parity check error and prevent from data overflow

//
// UART_CTRL2(0x18)
//
#define UART_CTRL2_FLAG_CLEAR           (0x00000001UL)
#define UART_CTRL2_CGWT_MASK            (0x00000002UL)                        // Smartcard CGWT interrupt mask
#define UART_CTRL2_BGWT_MASK            (0x00000004UL)                       // Smartcard BGWT interrupt mask
#define UART_CTRL2_TX_LEVEL_MASK        (0x00000008UL)                        // Smartcard TX level interrupt mask
#define UART_CTRL2_TX_LEVEL             (0x00000030UL)                        // Smartcard TX level interrupt
#define UART_CTRL2_NDS_FLC_EN           (0x00000080UL)                        // Smartcard NDS flow control enable
#define UART_CTRL2_TX_LEVEL_5_TO_4      (0x00000000UL)                        // TX level to 5->4 trigger int mode
#define UART_CTRL2_TX_LEVEL_9_TO_8      (0x00000010UL)                        // TX level to 9->8 trigger int mode
#define UART_CTRL2_TX_LEVEL_17_TO_16    (0x00000020UL)                        // TX level to 17->16 trigger int mode
#define UART_CTRL2_TX_LEVEL_31_TO_30    (0x00000030UL)                        // TX level to 31->30 trigger int mode
#define UART_CTRL2_REC_PE_EN            (0x00000040UL)                        // Enable parity error record

//
// UART_GWT_INT(0x19)
//
#define UART_GWT_BWT_FAIL               (0x00000001UL)
#define UART_GWT_BGT_FAIL               (0x00000002UL)
#define UART_GWT_CWT_RX_FAIL            (0x00000004UL)
#define UART_GWT_CWT_TX_FAIL            (0x00000008UL)
#define UART_GWT_CGT_RX_FAIL            (0x00000010UL)
#define UART_GWT_CGT_TX_FAIL            (0x00000020UL)
#define UART_GWT_TX_LEVEL_INT           (0x00000040UL)

//
// UART_TX_FIFO_COUNT(0x1A)
//
#define UART_TX_FIFO_CWT_RX_FAIL_DET_EN (0x00000040UL)


//
// UART_CTRL3(0x1F)
//
#define UART_NDS_FLC_BLK_DATA_CTRL      (0x00000080UL)                      // Tell rx module DO NOT received any data during NDS flow control
#define UART_AC_PWR_OFF_CTL             (0x00000040UL)                      // Power cut deactive function control
#define UART_TRAN_ETU_CTL               (0x00000020UL)                      // Delay 1 etu while parity error
#define UART_INVERT_CD                  (0x00000010UL)                      // Invert card detection
#define UART_AC_PWR_OFF_MASK            (0x00000008UL)                      // AC power off mask
#define UART_VCC_OFF_POL                (0x00000004UL)
#define UART_DEACTIVE_SEQ_EN            (0x00000001UL)                      // Smartcard deactive sequence enable


//
// UART_CTRL4(0x24)
//
#define UART_CTRL4_EXT_CWT_ECO_EN_BIT   (0x00000080UL)                        // Extend_CWT eco option bit for k6u02


//
// UART_CTRL5(0x25)
//
#define UART_CTRL5_PAD_MASK             (0x00000010UL)                        // Block all pads output to Smart Card
#define UART_CTRL5_PAD_RELEASE          (0x00000020UL)                        // Make all pads output to Smart Card without block
#define UART_CTRL5_ACTIVE_SEQ_EN        (0x00000001UL)                        // Smartcard active sequence enable
#define UART_CTRL5_AUTO_SEQ_CTRL        (0x00000008UL)                        // Smartcard auto sequence enable


//
// UART_CTRL6(0x27)
//
#define UART_CTRL6_SHORT_CIRCUIT_AUTO_DEACTIVE          (0x00000080UL)        // Automatical to deactivate while short circuit
#define UART_CTRL6_SHORT_CIRCUIT_MASK                   (0x00000040UL)
#define UART_CTRL6_SHORT_CIRCUIT_DISABLE                (0x00000020UL)        // Disable short circuit automatical deactivation
#define UART_CTRL6_GCR_SMC_3P3_GPIO_EN                  (0x00000010UL)        // Config as general GPIO or Smart Card mode
#define UART_CTRL6_LDO_3V_5V_EN                         (0x00000008UL)        // Enable 5v/3v mode
#define UART_CTRL6_PD_SMC_LDO                           (0x00000004UL)        // Power down Smart Card internal analog circuit


//
// UART_CTRL7(0x2c)
//
#define UART_CTRL7_ACT_PULLUP_EN            (0x00000080UL)          // Increasing I/O pad raising time
#define UART_CTRL7_PD_SMC_PAD               (0x00000040UL)          // Power down Smart Card PAD function
#define UART_CTRL7_VL_SEL_DEL_CNT_SEL       (0x00000030UL)          // Control vsel of analog circuit for 5v/3v control
#define UART_CTRL7_GCR_SMC_VL_SEL_HW_MASK   (0x00000008UL)          // vsel HW mask
#define UART_CTRL7_GCR_SMC_VL_SEL_SW        (0x00000004UL)          // vsel SW
#define UART_CTRL7_LDO_5V_EN_SW             (0x00000002UL)          // LDO 5V enable
#define UART_CTRL7_LDO_5V_DEBUG_MODE        (0x00000001UL)          // LDO debug mode


//
// UART_ANALOG_STATUS (0x2d)
//
#define UART_ANALOG_STATUS_SMC_LDO_5V_EN                (0x00000020UL)        // 5v enable status
#define UART_ANALOG_STATUS_PD_SMC_LDO                   (0x00000010UL)        // Power down Smart Card status
#define UART_ANALOG_STATUS_PD_SMC_PAD                   (0x00000008UL)        // Power down Smart Card pad func
#define UART_ANALOG_STATUS_GCR_SMC_VL_SEL               (0x00000004UL)        //
#define UART_ANALOG_STATUS_LDO_3V_TO_5V_DONE            (0x00000002UL)        // 3v to 5v is done
#define UART_ANALOG_STATUS_LDO_5V_TO_3V_DONE            (0x00000001UL)        // 5v to 3v is done

//
// UART_CTRL8 (0x2e)
//
#define UART_CTRL8_ANALOG_CNT_DONT_CARE_CARD_RESET      (0x00000008UL)        // analog counter donot care card reset
#define UART_CTRL8_ANALOG_DC_CARD_RESET                 (0x00000004UL)        // analog circuit donot care card reset
#define UART_CTRL8_CWT_EXT_EN                           (0x00000002UL)        // Extend CWT enable
#define UART_CTRL8_OCP_INTRF_MASK                       (0x00000001UL)        //

//
// UART_CTRL9 (0x2f)
//
#define UART_CTRL9_CWT_EXT                              (0x000000FFUL)        // Extend CWT

//
// UART_CTRL11 (0x31)
//
#define UART_CTRL11_SMC_SW_1P8V_EN                      (0x00000080UL)        //
#define UART_CTRL11_TEST_SMC_LDO_SELFB                  (0x0000000FUL)        //


//
// UART_CTRL14 (0x34)
//
#define UART_CTRL14_PD_SD_LDO                           (0x00000002UL)        //
#define UART_CTRL14_GCR_SMC_LDO_1P8V_EN                 (0x00000001UL)        // 1.8v ldo enable

//
// UART_ANALOG_STATUS2 (0x35)
//
#define UART_ANALOG_STATUS2_SMC_IO_1P8V_EN              (0x00000020UL)        //
#define UART_ANALOG_STATUS2_PD_SD_LDO                   (0x00000010UL)        //
#define UART_ANALOG_STATUS2_GCR_SMC_LDO_1P8V_EN         (0x00000008UL)        //
#define UART_ANALOG_STATUS2_GPIO_SMC_1P8V_SW            (0x00000004UL)        //
#define UART_ANALOG_STATUS2_LDO_3V_TO_1P8V_D            (0x00000002UL)        //
#define UART_ANALOG_STATUS2_LDO_1P8V_TO_3V_D            (0x00000001UL)        //

// ------------------------------------------------------------------------------------------------
// TOP

#if !defined(MSOS_TYPE_LINUX)
#if defined(READ_WORD) && defined(WRITE_WORD)
#define HW_READ(addr)                       READ_WORD((RIU_BUS_BASE + ((addr) << 1)))
#define HW_WRITE(addr, val)                 WRITE_WORD((RIU_BUS_BASE + ((addr) << 1)), (val))
#else // HAL / HWTEST
#define HW_READ(addr)                       (*(volatile MS_U16*)(RIU_BUS_BASE + ((addr) << 1)))
#define HW_WRITE(addr, val)                 { (*((volatile MS_U16*)(RIU_BUS_BASE + ((addr) << 1)))) = (MS_U16)(val); }
#endif
#else
#if defined(READ_WORD) && defined(WRITE_WORD)
#define HW_READ(addr)                       READ_WORD((RIU_BUS_BASE + ((addr-0x100000UL) << 1)))
#define HW_WRITE(addr, val)                 WRITE_WORD((RIU_BUS_BASE + ((addr-0x100000UL) << 1)), (val))
#else // HAL / HWTEST
#define HW_READ(addr)                       (*(volatile MS_U16*)(RIU_BUS_BASE + ((addr-0x100000UL) << 1)))
#define HW_WRITE(addr, val)                 { (*((volatile MS_U16*)(RIU_BUS_BASE + ((addr-0x100000UL) << 1)))) = (MS_U16)(val); }
#endif
#endif

#define REG_TOP_SM0                         (REG_CHIP_TOP_BASE + (0x0000000cUL << 1))
    #define TOP_SM0_OPEN                    BIT(0)
    #define TOP_SM0_C48                     BIT(4)
    #define TOP_SM0_EN                      BIT(8)

#define REG_TOP_SM1                         (REG_CHIP_TOP_BASE + (0x0000000cUL << 1))
    #define TOP_SM1_OPEN                    BIT(1)
    #define TOP_SM1_C48                     BIT(6)
    #define TOP_SM1_EN                      BIT(9)

#define REG_TOP_OCP                         (REG_CHIP_TOP_BASE + (0x0000000eUL << 1))
    #define TOP_OCP0_EN                     BIT(0)
    #define TOP_OCP1_EN                     BIT(1)

#define REG_TOP_ECO_SC_DEBOUNCE             (REG_CHIP_TOP_BASE + (0x00000020UL << 1))
    #define REG_TOP_ECO_SC_DEBOUNCE_EN      BIT(14)              // Smartcard ECO debounce switch. 0: off 1: on

#define REG_TOP_ALLPAD_IN                   (REG_CHIP_TOP_BASE + (0x00000050UL << 1))
    #define TOP_ALLPAD_IN_EN                BIT(15)

#define REG_TOP_SM0_DRV0                    (REG_CHIP_TOP_BASE + (0x0000001aUL << 1))
    #define TOP_SM0_DRV0_MASK               BMASK(4:0)
    #define TOP_SM0_DRV0_CLK                BIT(0)
    #define TOP_SM0_DRV0_RST                BIT(1)
    #define TOP_SM0_DRV0_IO                 BIT(2)
    #define TOP_SM0_DRV0_C4                 BIT(3)
    #define TOP_SM0_DRV0_C8                 BIT(4)

#define REG_TOP_SM0_DRV1                    (REG_CHIP_TOP_BASE + (0x0000001aUL << 1))
    #define TOP_SM0_DRV1_MASK               BMASK(9:5)
    #define TOP_SM0_DRV1_CLK                BIT(5)
    #define TOP_SM0_DRV1_RST                BIT(6)
    #define TOP_SM0_DRV1_IO                 BIT(7)
    #define TOP_SM0_DRV1_C4                 BIT(8)
    #define TOP_SM0_DRV1_C8                 BIT(9)

#define REG_TOP_SM0_PU                      (REG_CHIP_TOP_BASE + (0x0000001aUL << 1))
    #define TOP_SM0_PU_11K_MASK             BMASK(14:10)
    #define TOP_SM0_PU_11K_CLK              BIT(10)
    #define TOP_SM0_PU_11K_RST              BIT(11)
    #define TOP_SM0_PU_11K_IO               BIT(12)
    #define TOP_SM0_PU_11K_C4               BIT(13)
    #define TOP_SM0_PU_11K_C8               BIT(14)

#define REG_TOP_CKG_UART_SRC                (REG_CLK_GEN_BASE + (0x00000013UL << 1))
    #define TOP_CKG_UART_SRC_DFT            0
    #define TOP_CKG_UART0_SRC_CLK           BIT(0)              // DFT_LIVE / CLK_UART
    #define TOP_CKG_UART1_SRC_CLK           BIT(1)
    #define TOP_CKG_UART2_SRC_CLK           BIT(2)

#define REG_TOP_CKG_UART_CLK                (REG_CLK_GEN2_BASE + (0x00000012UL << 1))
    #define TOP_CKG_UART1_MASK              BMASK(4:0)
    #define TOP_CKG_UART1_CLK_DIS           BIT(0)
    #define TOP_CKG_UART1_CLK_INV           BIT(1)
    #define TOP_CKG_UART1_CLK_MASK          BMASK(4:2)
    #define TOP_CKG_UART1_CLK_170M          BITS(4:2, 0)
    #define TOP_CKG_UART1_CLK_160M          BITS(4:2, 1)
    #define TOP_CKG_UART1_CLK_144M          BITS(4:2, 2)
    #define TOP_CKG_UART1_CLK_123M          BITS(4:2, 3)
    #define TOP_CKG_UART1_CLK_108M          BITS(4:2, 4)
    #define TOP_CKG_UART1_CLK_PLL           BITS(4:2, 5)
    #define TOP_CKG_UART1_CLK_PLL_D2        BITS(4:2, 6)

    #define TOP_CKG_UART2_MASK              BMASK(12:8)
    #define TOP_CKG_UART2_CLK_DIS           BIT(8)
    #define TOP_CKG_UART2_CLK_INV           BIT(9)
    #define TOP_CKG_UART2_CLK_MASK          BMASK(12:10)
    #define TOP_CKG_UART2_CLK_170M          BITS(12:10, 0)
    #define TOP_CKG_UART2_CLK_160M          BITS(12:10, 1)
    #define TOP_CKG_UART2_CLK_144M          BITS(12:10, 2)
    #define TOP_CKG_UART2_CLK_123M          BITS(12:10, 3)
    #define TOP_CKG_UART2_CLK_108M          BITS(12:10, 4)
    #define TOP_CKG_UART2_CLK_PLL           BITS(12:10, 5)
    #define TOP_CKG_UART2_CLK_PLL_D2        BITS(12:10, 6)

#define REG_TOP_CKG_SM_CA                   (REG_CLK_GEN1_BASE + (0x25 << 1))
    #define TOP_CKG_SM_CA0_MASK             BMASK(3:0)
    #define TOP_CKG_SM_CA0_DIS              BIT(0)
    #define TOP_CKG_SM_CA0_INV              BIT(1)
    #define TOP_CKG_SM_CA0_CLK_MASK         BMASK(3:2)
    #define TOP_CKG_SM_CA0_CLK_27M_D6       BITS(3:2, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D2       BITS(3:2, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D4       BITS(3:2, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D8       BITS(3:2, 3)        // 3.375MHz
    #define TOP_CKG_SM_CA1_MASK             BMASK(7:4)
    #define TOP_CKG_SM_CA1_DIS              BIT(4)
    #define TOP_CKG_SM_CA1_INV              BIT1(5)
    #define TOP_CKG_SM_CA1_CLK_MASK         BMASK(7:6)
    #define TOP_CKG_SM_CA1_CLK_27M_D6       BITS(7:6, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D2       BITS(7:6, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D4       BITS(7:6, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D8       BITS(7:6, 3)        // 3.375MHz

#define REG_TOP_CKG_SM_CACLK_EXT            (REG_CLK_GEN1_BASE + (0x00000020UL << 1))
#define REG_TOP_CKG_SM_CACLK_M              (REG_CLK_GEN1_BASE + (0x00000020UL << 1))
#define REG_TOP_CKG_SM_CACLK_N              (REG_CLK_GEN1_BASE + (0x00000021UL << 1))

#define REG_TOP_CKG_SM1_CACLK_EXT           (REG_CLK_GEN1_BASE + (0x00000022UL << 1))
#define REG_TOP_CKG_SM1_CACLK_M             (REG_CLK_GEN1_BASE + (0x00000022UL << 1))
#define REG_TOP_CKG_SM1_CACLK_N             (REG_CLK_GEN1_BASE + (0x00000023UL << 1))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------



#endif // _REG_SC_H_
