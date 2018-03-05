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

#ifndef _REG_SC_H_
#define _REG_SC_H_

#include "MsCommon.h"
#include "halSC.h"
//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_LINUX)
#define REG_SC_BASE1                        0x00102900UL
#define REG_SC_BASE2                        0x00000000UL                  //not support
#define RIU_BUS_BASE                        0xFD000000UL
#else
#define REG_SC_BASE1                        (_regSCBase[0])
#define REG_SC_BASE2                        (_regSCBase[1])
#define RIU_BUS_BASE                        _regSCHWBase
#endif

#define REG_CHIP_TOP_BASE                   0x00101E00UL
#define REG_CLK_GEN_BASE                    0x00100B00UL
#define REG_CHIP_GPIO_BASE                  0x00102B00UL
// ------------------------------------------------------------------------------------------------
// UART / SC

#if !defined(MSOS_TYPE_LINUX)
#define UART1_READ(addr)            READ_BYTE(RIU_BUS_BASE + ((REG_SC_BASE1 + ((addr) << 1)) << 1))
#define UART2_READ(addr)            READ_BYTE(RIU_BUS_BASE + ((REG_SC_BASE2 + ((addr) << 1)) << 1))

#define UART1_WRITE(addr, val)              WRITE_BYTE(RIU_BUS_BASE + ((REG_SC_BASE1 + ((addr) << 1)) << 1), (val))
#define UART2_WRITE(addr, val)      WRITE_BYTE(RIU_BUS_BASE + ((REG_SC_BASE2 + ((addr) << 1)) << 1), (val))
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
#define UART_RX                             (0x00UL)                                 // In:  Receive buffer (DLAB=0), 16-byte FIFO
#define UART_TX                             (0x00UL)                                 // Out: Transmit buffer (DLAB=0), 16-byte FIFO
#define UART_DLL                            (0x00UL)                                 // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM                            (0x01UL)                                 // Out: Divisor Latch High (DLAB=1)
#define UART_IER                            (0x01UL)                                 // Out: Interrupt Enable Register
#define UART_IIR                            (0x02UL)                                 // In:  Interrupt ID Register
#define UART_FCR                            (0x02UL)                                 // Out: FIFO Control Register
#define UART_EFR                            (0x02UL)                                 // I/O: Extended Features Register
                                                                                     // (DLAB=1, 16C660 only)
#define UART_LCR                            (0x03UL)                                 // Out: Line Control Register
#define UART_MCR                            (0x04UL)                                 // Out: Modem Control Register
#define UART_LSR                            (0x05UL)                                 // In:  Line Status Register
#define UART_MSR                            (0x06UL)                                 // In:  Modem Status Register
#define UART_SCR                            (0x07UL)                                 // I/O: Scratch Register

#define UART_SCCR                           (0x08UL)                                 // Smartcard Control Register
#define UART_SCSR                           (0x09UL)                                 // Smartcard Status Register

#define UART_SCFC                           (0x0aUL)                                 // Smartcard Fifo Count Register
#define UART_SCFI                           (0x0bUL)                                 // Smartcard Fifo Index Register
#define UART_SCFR                           (0x0cUL)                                 // Smartcard Fifo Read Delay Register

#define UART_SCMR                           (0x0dUL)                                 // Smartcard Mode Register
#define UART_SCCGT                          (0x0eUL)                                 // Smartcard char guard time - only after Kronus U02
#define UART_SCDBT                          (0x0fUL)                                 // Smartcard debounce time
#define UART_SCCWT_L                        (0x10UL)                                 // Smartcard char waiting time - LoByte
#define UART_SCCWT_H                        (0x11UL)                                  // Smartcard char waiting time - HiByte
#define UART_SCBGT                          (0x12UL)                                  // Smartcard block guard time
#define UART_SCBWT_0                        (0x14UL)                                  // Smartcard block waiting time bit 07:00
#define UART_SCBWT_1                        (0x15UL)                                  // Smartcard block waiting time bit 15:08
#define UART_SCBWT_2                        (0x16UL)                                  // Smartcard block waiting time bit 23:16
#define UART_SCBWT_3                        (0x17UL)                                  // Smartcard block waiting time bit 31:24
#define UART_CTRL2                          (0x18UL)                                  // CGT/CWT/BGT/BWT int mask
#define UART_GWT_INT                        (0x19UL)                                  // CGT/CWT/BGT/BWT int status
#define UART_DEACTIVE_RST                   (0x1bUL)        //0x1b:reg_deactive_rst   // Set the active time for RST pin
#define UART_DEACTIVE_CLK                   (0x1cUL)        //0x1c:reg_deactive_clk   // Set the active time for CLK pin
#define UART_DEACTIVE_IO                    (0x1dUL)        //0x1d:reg_deactive_io    // Set the active time for IO pin
#define UART_DEACTIVE_VCC                   (0x1eUL)        //0x1e:reg_deactive_vcc   // Set the active time for VCC pin
#define UART_DEACTIVE_ENABLE                (0x1fUL)        //0x1f:reg_ctrl3          // Enable the de-active sequence func
#define UART_CTRL3                          (0x1fUL)                                  // NDS/ACpower-off/transmitter ctrl
#define UART_ACTIVE_RST                     (0x20UL)                  // Set the active time for RST pin
#define UART_ACTIVE_CLK                     (0x21UL)                  // Set the active time for CLK pin
#define UART_ACTIVE_IO                      (0x22UL)                  // Set the active time for IO pin
#define UART_ACTIVE_VCC                     (0x23UL)                  // Set the active time for VCC pin
#define UART_CTRL4                          (0x24UL)                  // control4 reg
#define UART_CTRL5                          (0x25UL)                  // control5 reg
#define UART_DEBUG4                         (0x26UL)                  // debug4 reg
#define UART_CTRL6                          (0x27UL)                  // control6 reg
#define UART_SHORT_CIRCUT_DB_TIME_L         (0x28UL)                  // short_circuit_debounce_time_low
#define UART_SHORT_CIRCUT_DB_TIME_H         (0x29UL)                  // short_circuit_debounce_time_high
#define UART_SMC_ATOP_LOW                   (0x2aUL)                  // reg_test_smc_atop_low
#define UART_SMC_ATOP_HIGH                  (0x2bUL)                  // reg_test_smc_atop_high
#define UART_CTRL7                          (0x2cUL)                  // control6 reg

#define UART_ANALOG_STATUS                  (0x2dUL)                  // analog_status
#define UART_CTRL8                          (0x2eUL)                  // reg_ctrl8
#define UART_CTRL9                          (0x2eUL)                  // reg_ctrl9

//
// UART_FCR(2)
// FIFO Control Register (16650 only)
//
#define UART_FCR_ENABLE_FIFO                (0x01UL)                  // Enable the FIFO
#define UART_FCR_CLEAR_RCVR                 (0x02UL)                  // Clear the RCVR FIFO
#define UART_FCR_CLEAR_XMIT                 (0x04UL)                  // Clear the XMIT FIFO
#define UART_FCR_DMA_SELECT                 (0x08UL)                  // For DMA applications
#define UART_FCR_TRIGGER_MASK               (0xC0UL)                  // Mask for the FIFO trigger range
#define UART_FCR_TRIGGER_1                  (0x00UL)                  // Mask for trigger set at 1
#define UART_FCR_TRIGGER_4                  (0x40UL)                  // Mask for trigger set at 4
#define UART_FCR_TRIGGER_8                  (0x80UL)                  // Mask for trigger set at 8
#define UART_FCR_TRIGGER_14                 (0xC0UL)                  // Mask for trigger set at 14

//
// UART_LCR(3)
// Line Control Register
// Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
// UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
//
#define UART_LCR_WLEN5                      (0x00UL)              // Wordlength: 5 bits
#define UART_LCR_WLEN6                      (0x01UL)              // Wordlength: 6 bits
#define UART_LCR_WLEN7                      (0x02UL)              // Wordlength: 7 bits
#define UART_LCR_WLEN8                      (0x03UL)              // Wordlength: 8 bits
#define UART_LCR_STOP1                      (0x00UL)              //
#define UART_LCR_STOP2                      (0x04UL)              // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_PARITY                     (0x08UL)              // Parity Enable
#define UART_LCR_EPAR                       (0x10UL)              // Even parity select
#define UART_LCR_SPAR                       (0x20UL)              // Stick parity (?)
#define UART_LCR_SBC                        (0x40UL)              // Set break control
#define UART_LCR_DLAB                       (0x80UL)              // Divisor latch access bit

//
// UART_LSR(5)
// Line Status Register
//
#define UART_LSR_DR                         (0x01UL)              // Receiver data ready
#define UART_LSR_OE                         (0x02UL)              // Overrun error indicator
#define UART_LSR_PE                         (0x04UL)              // Parity error indicator
#define UART_LSR_FE                         (0x08UL)              // Frame error indicator
#define UART_LSR_BI                         (0x10UL)              // Break interrupt indicator
#define UART_LSR_THRE                       (0x20UL)              // Transmit-hold-register empty
#define UART_LSR_TEMT                       (0x40UL)              // Transmitter empty

//
// UART_IIR(2)
// Interrupt Identification Register
//
#define UART_IIR_NO_INT                     (0x01UL)              // No interrupts pending
#define UART_IIR_ID                         (0x06UL)              // Mask for the interrupt ID
#define UART_IIR_MSI                        (0x00UL)              // Modem status interrupt
#define UART_IIR_THRI                       (0x02UL)              // Transmitter holding register empty
#define UART_IIR_TOI                        (0x0cUL)              // Receive time out interrupt
#define UART_IIR_RDI                        (0x04UL)              // Receiver data interrupt
#define UART_IIR_RLSI                       (0x06UL)              // Receiver line status interrupt

//
// UART_IER(1)
// Interrupt Enable Register
//
#define UART_IER_RDI                        (0x01UL)              // Enable receiver data available interrupt
#define UART_IER_THRI                       (0x02UL)              // Enable Transmitter holding reg empty int
#define UART_IER_RLSI                       (0x04UL)              // Enable receiver line status interrupt
#define UART_IER_MSI                        (0x08UL)              // Enable Modem status interrupt

//
// UART_MCR(4)
// Modem Control Register
//
#define UART_MCR_DTR                        (0x01UL)             // DTR complement
#define UART_MCR_RTS                        (0x02UL)             // RTS complement
#define UART_MCR_OUT1                       (0x04UL)             // Out1 complement
#define UART_MCR_OUT2                       (0x08UL)             // Out2 complement
#define UART_MCR_LOOP                       (0x10UL)             // Enable loopback test mode
#define UART_MCR_FAST                       (0x20UL)             // Slow / Fast baud rate mode

//
// UART_MSR(6)
// Modem Status Register
//
#define UART_MSR_ANY_DELTA                  (0x0FUL)             // Any of the delta bits!
#define UART_MSR_DCTS                       (0x01UL)             // Delta CTS
#define UART_MSR_DDSR                       (0x02UL)             // Delta DSR
#define UART_MSR_TERI                       (0x04UL)             // Trailing edge ring indicator
#define UART_MSR_DDCD                       (0x08UL)             // Delta DCD
#define UART_MSR_CTS                        (0x10UL)             // Clear to Send
#define UART_MSR_DSR                        (0x20UL)             // Data Set Ready
#define UART_MSR_RI                         (0x40UL)             // Ring Indicator
#define UART_MSR_DCD                        (0x80UL)             // Data Carrier Detect

//
// UART_EFR(2, UART_LCR_DLAB)
// These are the definitions for the Extended Features Register
// (StarTech 16C660 only, when DLAB=1)
//
#define UART_EFR_ENI                        (0x10UL)             // Enhanced Interrupt
#define UART_EFR_SCD                        (0x20UL)             // Special character detect
#define UART_EFR_RTS                        (0x40UL)             // RTS flow control
#define UART_EFR_CTS                        (0x80UL)             // CTS flow control

//
// UART_SCCR(8)
// SmartCard Control Register
//
#define UART_SCCR_MASK_CARDIN               (0x01UL)             // Smartcard card in interrupt mask
#define UART_SCCR_MASK_CARDOUT              (0x02UL)             // Smartcard card out interrupt mask
#define UART_SCCR_TX_BINV                   (0x04UL)             // Smartcard Tx bit invert
#define UART_SCCR_TX_BSWAP                  (0x08UL)             // Smartcard Tx bit swap
#define UART_SCCR_RST                       (0x10UL)             // Smartcard reset 0->1, UART Rx enable 1
#define UART_SCCR_RX_BINV                   (0x20UL)             // Smartcard Rx bit inverse
#define UART_SCCR_RX_BSWAP                  (0x40UL)             // Smartcard Rx bit swap

//
// UART_SCSR(9)
// Smartcard Status Register
//
#define UART_SCSR_CLK                       (0x01UL)             // Smartcard clock out
#define UART_SCSR_INT_CARDIN                (0x02UL)             // Smartcard card in interrupt
#define UART_SCSR_INT_CARDOUT               (0x04UL)             // Smartcard card out interrupt
#define UART_SCSR_DETECT                    (0x08UL)             // Smartcard detection status

//
// UART_SCFC(10), UART_SCFI(11), UART_SCFR(12)
// Smartcard Fifo Register
//
#define UART_SCFC_MASK                      (0x07UL)
#define UART_SCFI_MASK                      (0x0FUL)
#define UART_SCFR_MASK                      (0x07UL)


//
// UART_SCFR(12)
// Smartcard Fifo Read Delay Register
//
#define UART_SCFR_DELAY_MASK                (0x03UL)
#define UART_SCFR_V_HIGH                    (0x04UL)
#define UART_SCFR_V_ENABLE                  (0x08UL)             // Vcc = (Vcc_high ^ (Vcc_en & UART_SCSR_INT_CARDOUT))

//
// UART_SCMR(13)
// SMart Mode Register
//
#define UART_SCMR_RETRY_MASK                (0x1FUL)
#define UART_SCMR_SMARTCARD                 (0x20UL)
#define UART_SCMR_PARITYCHK                 (0x40UL)
#define UART_SCMR_FLOWCTRL                  (0x80UL)             // control the parity check error and prevent from data overflow

//
// REG_CTRL3(0x1f)
//
#define UART_DEACTIVE_SEQUENCE_EN           (0x01UL)
#define UART_C4_C8_OFF_EN                   (0x02UL)
#define UART_VCC_OFF_POL                    (0x04UL)
#define UART_ACPOWER_MASK                   (0x08UL)
#define UART_INVERT_CD                      (0x10UL)
#define UART_TRANSMITTER_ETU_CTRL           (0x20UL)
#define UART_ACPOWER_OFF_CTRL               (0x40UL)
#define UART_NDS_FLC_BLOCK_DATA_CTRL        (0x80UL)

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

#define REG_TOP_TS1_PE                      (REG_CHIP_TOP_BASE + (0x6UL << 1))
#define REG_TOP_TEST_MODE                   (REG_CHIP_TOP_BASE + (0x12UL << 1))
#define REG_TOP_ALLPAD_IN                   (REG_CHIP_TOP_BASE + (0x50UL << 1))
#define TOP_ALLPAD_IN_EN                    BIT(15)
#define REG_TOP_CONFIG                      (REG_CHIP_TOP_BASE + (0x57UL << 1))

#define REG_DSPEJTAG_MODE                   (REG_CHIP_TOP_BASE + (0x3UL << 1))
#define REG_TS1CONFIG                       (REG_CHIP_TOP_BASE + (0x51UL << 1))

#define REG_TOP_SM0                         (REG_CHIP_TOP_BASE + (0x6eUL << 1))

#define REG_TOP_CKG_UART_CLK                (REG_CLK_GEN_BASE + (0x6cUL << 1))
    #define TOP_CKG_UART1_MASK              BMASK(4:0)
    #define TOP_CKG_UART1_CLK_DIS           BIT(0)
    #define TOP_CKG_UART1_CLK_INV           BIT(1)
    #define TOP_CKG_UART1_CLK_MASK          BMASK(4:2)
    #define TOP_CKG_UART1_CLK_172M          BITS(4:2, 0)
    #define TOP_CKG_UART1_CLK_160M          BITS(4:2, 1)
    #define TOP_CKG_UART1_CLK_144M          BITS(4:2, 2)
    #define TOP_CKG_UART1_CLK_123M          BITS(4:2, 3)
    #define TOP_CKG_UART1_CLK_108M          BITS(4:2, 4)
    //#define TOP_CKG_UART1_CLK_PLL                   BITS(4:2, 5)
    //#define TOP_CKG_UART1_CLK_PLL_D2                BITS(4:2, 6)

#define REG_TOP_CKG_OUT_SEL                 (REG_CLK_GEN_BASE + (0x68UL << 1)) //?

#define REG_TOP_CKG_SM_CA                   (REG_CLK_GEN_BASE + (0x6cUL << 1))
    #define TOP_CKG_SM_CA0_MASK             BMASK(11:8)
    #define TOP_CKG_SM_CA0_DIS              BIT(8)
    #define TOP_CKG_SM_CA0_INV              BIT(9)
    #define TOP_CKG_SM_CA0_CLK_MASK         BMASK(11:10)
    #define TOP_CKG_SM_CA0_CLK_27M_D6       BITS(11:10, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D2       BITS(11:10, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D4       BITS(11:10, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D8       BITS(11:10, 3)        // 3.375MHz

#define REG_TOP_3V_5V_SELECT                (REG_CHIP_GPIO_BASE + (0x5bUL << 1))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------



#endif // _REG_SC_H_
