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
#ifndef DRV_USBHOST_CONFIG_H
#define DRV_USBHOST_CONFIG_H

#include <MsIRQ.h>
#include <drvUSB_eCos.h> // for root hub support only, applying the configuration

#define ECOS_USB_HOST_LOCAL_VER "20170310"

/*** eCos USB Host driver exclusive default setting ***/
#define URB_TIMEOUT_BY_WAIT_EVENT

//#define USB_EHCI_TT

#define ECOS_XHC_COMPANION_SUPPORT

//#define ENABLE_LEGACY_CACHE_SETTING

#define ENABLE_DISCONNECT_FAST_RESPONSE

/* 20140630 quick response for file read/write root hub disconnected */
#define ENABLE_RW_DISCONNECTING

#define ENABLE_ROOTHUB_DISCONN_REINIT

/* 20140715 maximum hub top level to restric hub_probe */
#ifdef USB_NOT_SUPPORT_EX_HUB
#define MAX_HUB_TOPO_LEVEL 1
#else
#define MAX_HUB_TOPO_LEVEL 6
#endif

/*** Definition by chip attribute ***/
//#define CPU_TYPE_AEON
#if defined(CHIP_KAISER) || \
    defined(CHIP_CLIPPERS) || \
    defined(CHIP_KANO) || \
    defined(CHIP_K6) || \
    defined(CHIP_CURRY) || \
    defined(CHIP_K6LITE)
#define CPU_TYPE_ARM
#else
#define CPU_TYPE_MIPS
#endif

#if defined(CPU_TYPE_AEON)
    #define OS_BASE_ADDR        0xa0000000
#elif defined(CPU_TYPE_MIPS) // U4, K1, K2
    #if defined(TITANIA2_SERIAL_USE)
    #define OS_BASE_ADDR    0xbf800000
    #else
    #define OS_BASE_ADDR    0xbf200000
    #endif
    #define _MSTAR_PM_BASE  0xbf000000
    #define MIPS_TOOLCHAIN_482_ISSUE
    #define MIPS_L1_CACHE_SIZE 32
#elif defined(CPU_TYPE_ARM) // K3
    #define OS_BASE_ADDR        0xfd200000
    #define _MSTAR_PM_BASE  	0xfd000000
    #define MIPS_L1_CACHE_SIZE 32
#else
#No CPU type for USB
#endif

#ifdef ENABLE_LEGACY_CACHE_SETTING
#define CPU_L1_CACHE_BOUND (15)
#else
#define CPU_L1_CACHE_BOUND (MIPS_L1_CACHE_SIZE-1)
#endif

#define KAISERIN_CHIP_TOP_BASE (OS_BASE_ADDR+(0x1E00*2))

#define BASE_USBBC_NULL     (0)

/****** USB port RIU base address ******/
/* port 0 */
#define BASE_UTMI0          (OS_BASE_ADDR+(0x3A80*2))
#define BASE_UHC0           (OS_BASE_ADDR+(0x2400*2))
#define BASE_USBC0          (OS_BASE_ADDR+(0x0700*2))
#define BASE_USBBC0_KAPPA   (OS_BASE_ADDR+(0x20500*2))
#define BASE_USBBC0_KIWI    (OS_BASE_ADDR+(0x20500*2))
#define BASE_USBBC0_KELTIC  (OS_BASE_ADDR+(0x11700*2))
#define BASE_USBBC0_KRATOS  (OS_BASE_ADDR+(0x20500*2))
#define BASE_USBBC0_KENYA   (OS_BASE_ADDR+(0x205E0*2))
#define BASE_USBBC0_KAISER  (OS_BASE_ADDR+(0x22F00*2))
#define BASE_USBBC0_KERES   (OS_BASE_ADDR+(0x13700*2))
#define BASE_USBBC0_CLIPPERS (OS_BASE_ADDR+(0x23600*2))
#define BASE_USBBC0_KANO    BASE_USBBC0_KERES
#define BASE_USBBC0_K6      BASE_USBBC0_KERES
#define BASE_USBBC0_CURRY   BASE_USBBC0_KERES
#define BASE_USBBC0_K6LITE  BASE_USBBC0_CLIPPERS
#define E_IRQ_UHC           (E_INT_IRQ_UHC)
#define E_IRQ_USBC          (E_INT_IRQ_USB)

/* port 1 */
#define BASE_UTMI1          (OS_BASE_ADDR+(0x3A00*2))
#define BASE_UHC1           (OS_BASE_ADDR+(0x0D00*2))
#define BASE_USBC1          (OS_BASE_ADDR+(0x0780*2))
#define BASE_USBBC1_KENYA   (OS_BASE_ADDR+(0x205C0*2))
#define BASE_USBBC1_KAISER  (OS_BASE_ADDR+(0x22F40*2))
#define BASE_USBBC1_KERES   (OS_BASE_ADDR+(0x13740*2))
#define BASE_USBBC1_KRATOS  (OS_BASE_ADDR+(0x20540*2))
#define BASE_USBBC1_CLIPPERS (OS_BASE_ADDR+(0x23620*2))
#define BASE_USBBC1_KIWI    (OS_BASE_ADDR+(0x20540*2))
#define BASE_USBBC1_KANO    (OS_BASE_ADDR+(0x13720*2))
#define BASE_USBBC1_K6      BASE_USBBC1_KERES
#define BASE_USBBC1_CURRY   BASE_USBBC1_KANO
#define BASE_USBBC1_K6LITE  BASE_USBBC1_CLIPPERS
#define E_IRQ_UHC1          (E_INT_IRQ_UHC1)
#define E_IRQ_USBC1         (E_INT_IRQ_USB1)

/* port 2 */
#define BASE_UTMI2          (OS_BASE_ADDR+(0x2A00*2))
#define BASE_UTMI2_CLIPPERS (OS_BASE_ADDR+(0x3900*2))
#define BASE_UTMI2_KANO     BASE_UTMI2_CLIPPERS
#define BASE_UTMI2_CURRY    BASE_UTMI2_CLIPPERS
#define BASE_UTMI2_K6       (OS_BASE_ADDR+(0x3800*2))
#define BASE_UTMI2_K6LITE   BASE_UTMI2_K6
#define BASE_UHC2           (OS_BASE_ADDR+(0x10300*2))
#define BASE_UHC2_CLIPPERS  (OS_BASE_ADDR+(0x13900*2))
#define BASE_UHC2_K6        (OS_BASE_ADDR+(0x40400*2))
#define BASE_UHC2_K6LITE    BASE_UHC2_CLIPPERS
#define BASE_USBC2          (OS_BASE_ADDR+(0x10200*2))
#define BASE_USBC2_CLIPPERS (OS_BASE_ADDR+(0x13800*2))
#define BASE_USBC2_K6       (OS_BASE_ADDR+(0x40600*2))
#define BASE_USBC2_K6LITE   BASE_USBC2_CLIPPERS
#define BASE_USBBC2_KAISER  (OS_BASE_ADDR+(0x22F80*2))
#define BASE_USBBC2_CLIPPERS (OS_BASE_ADDR+(0x23640*2))
#define BASE_USBBC2_KANO    (OS_BASE_ADDR+(0x13740*2))
#define BASE_USBBC2_K6      (OS_BASE_ADDR+(0x13780*2))
#define BASE_USBBC2_CURRY   BASE_USBBC2_KANO
#define BASE_USBBC2_K6LITE  BASE_USBBC2_CLIPPERS
#define E_IRQ_UHC2          (E_INT_IRQ_UHC2)
#define E_IRQ_USBC2         (E_INT_IRQ_USB2)
#define E_IRQ_UHC2_K6       (E_INT_IRQ_USB30_HS_UHC_INT)
#define E_IRQ_USBC2_K6      (E_INT_IRQ_USB30_HS_USB_INT)

/* port 3 */
#define BASE_UTMI3          (OS_BASE_ADDR+(0x20A00*2))
#define BASE_UTMI3_KANO     (OS_BASE_ADDR+(0x3800*2))
#define BASE_UHC3           (OS_BASE_ADDR+(0x20900*2))
#define BASE_UHC3_KANO      (OS_BASE_ADDR+(0x40400*2))
#define BASE_USBC3          (OS_BASE_ADDR+(0x20800*2))
#define BASE_USBC3_KANO     (OS_BASE_ADDR+(0x40600*2))
#define BASE_USBBC3_KANO    (OS_BASE_ADDR+(0x13780*2))
#define E_IRQ_UHC3          (E_INT_IRQ_UHC3)
#define E_IRQ_USBC3         (E_INT_IRQ_USB3)
#define E_IRQ_UHC3_KANO     (E_INT_IRQ_USB30_HS_UHC_INT)
#define E_IRQ_USBC3_KANO    (E_INT_IRQ_USB30_HS_USB_INT)

/* port 4 */
#define BASE_UTMI4_KANO     (OS_BASE_ADDR+(0x03880*2))
#define BASE_UHC4_KANO      (OS_BASE_ADDR+(0x40500*2))
#define BASE_USBC4_KANO     (OS_BASE_ADDR+(0x40680*2))
#define BASE_USBBC4_KANO    (OS_BASE_ADDR+(0x137a0*2))
#define E_IRQ_UHC4          (E_INT_IRQ_UHC4)
#define E_IRQ_USBC4         (E_INT_IRQ_USB4)
#define E_IRQ_UHC4_KANO     (E_INT_IRQ_USB30_HS1_UHC_INT)
#define E_IRQ_USBC4_KANO    (E_INT_IRQ_USB30_HS1_USB_INT)

/* struct xhc_comp */
#ifdef ECOS_XHC_COMPANION_SUPPORT
#if defined(CHIP_KANO) || \
	defined(CHIP_K6)
#define ENABLE_XHC_COMPANION
#define XHC_COMP_NONE       {0, 0, 0, 0, 0}
#define XHCI_SINGLE_PORT_ENABLE_MAC
#define XHCI_ENABLE_PD_OVERRIDE
#endif
#endif

#if defined(CHIP_KANO)
#define XHCI_PORT0_ADDR         (OS_BASE_ADDR+(0xc0000<<1)+0x0440)
#define XHCI_PORT1_ADDR         (OS_BASE_ADDR+(0xc0000<<1)+0x0450)
#define MSTAR_U3TOP_BASE        (OS_BASE_ADDR+(0x40200*2))
#define MSTAR_U3PHY_P0_A_BASE	(OS_BASE_ADDR+(0x02100 << 1))
#define MSTAR_U3PHY_P0_D_BASE	(OS_BASE_ADDR+(0x02000 << 1))
#define MSTAR_U3PHY_P1_A_BASE	(OS_BASE_ADDR+(0x02300 << 1))
#define MSTAR_U3PHY_P1_D_BASE	(OS_BASE_ADDR+(0x02200 << 1))
#define XHC_COMP_PORT0          {0, XHCI_PORT0_ADDR, MSTAR_U3TOP_BASE, MSTAR_U3PHY_P0_A_BASE, MSTAR_U3PHY_P0_D_BASE}
#define XHC_COMP_PORT1          {1, XHCI_PORT1_ADDR, MSTAR_U3TOP_BASE, MSTAR_U3PHY_P1_A_BASE, MSTAR_U3PHY_P1_D_BASE}
#elif defined(CHIP_K6)
#define XHCI_PORT0_ADDR         (OS_BASE_ADDR+(0xf0000<<1)+0x0430)
#define MSTAR_U3TOP_BASE        (OS_BASE_ADDR+(0x40200*2))
#define MSTAR_U3PHY_P0_A_BASE	(OS_BASE_ADDR+(0x02100 << 1))
#define MSTAR_U3PHY_P0_D_BASE	(OS_BASE_ADDR+(0x02000 << 1))
#define XHC_COMP_PORT0          {0, XHCI_PORT0_ADDR, MSTAR_U3TOP_BASE, MSTAR_U3PHY_P0_A_BASE, MSTAR_U3PHY_P0_D_BASE}
#endif

/* define UHC port enable */
/* two ports is default, else extra needs to be defined */
#if defined(CHIP_U4) || \
	defined(CHIP_K2) || \
	defined(CHIP_KAISER) || \
	defined(CHIP_CLIPPERS) || \
	defined(CHIP_KANO) || \
	defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE)
#define ENABLE_THIRD_EHC
#endif

#if defined(CHIP_K2) || \
	(defined(CHIP_KANO) && defined(ENABLE_XHC_COMPANION))
#define ENABLE_FOURTH_EHC
#endif

#if (defined(CHIP_KANO) && defined(ENABLE_XHC_COMPANION))
#define ENABLE_FIFTH_EHC
#endif

#if (defined(CHIP_KANO) && defined(ENABLE_XHC_COMPANION))
	#define NUM_OF_ROOT_HUB 5
#elif defined(CHIP_K2)
	#define NUM_OF_ROOT_HUB 4
#elif defined(CHIP_U4) || \
	defined(CHIP_KAISER) || \
	defined(CHIP_CLIPPERS) || \
	defined(CHIP_KANO) || \
	(defined(CHIP_K6) && defined(ENABLE_XHC_COMPANION)) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE)
	#define NUM_OF_ROOT_HUB 3
#else
	#define NUM_OF_ROOT_HUB 2
#endif

//------ Hardware ECO enable switch ----------------------------------
//---- 1. fix pv2mi bridge mis-behavior, list chip before Kris
#if defined(CHIP_KAPPA) || \
	defined(CHIP_KELTIC) || \
	defined(CHIP_KENYA) || \
	defined(CHIP_KRITI) || \
	defined(CHIP_CLIPPERS) || \
	defined(CHIP_KERES) || \
	defined(CHIP_KIRIN) || \
	defined(CHIP_KRIS)
        #define ENABLE_PV2MI_BRIDGE_ECO
#endif

//---- 2. Reduce EOF1 to 16us for performance imporvement
#if !defined(CHIP_U4) && !defined(CHIP_K1) && !defined(CHIP_K2)
/* Enlarge EOF1 from 12us to 16us for babble prvention under hub case.
 * However, we keep the "old config name". 20130121
 */
        #define ENABLE_16US_EOF1
#endif

//---- 3. Enable UTMI 240 as 120 phase
#if !defined(CHIP_U4) && !defined(CHIP_K1) && !defined(CHIP_K2)
        #define ENABLE_UTMI_240_AS_120_PHASE_ECO
#endif

//---- 4. Change to 55 interface
#if !defined(CHIP_U4) && !defined(CHIP_K1) && !defined(CHIP_K2)
        #define ENABLE_UTMI_55_INTERFACE
#endif

//---- 5. tx/rx reset clock gating cause XIU timeout
#if !defined(CHIP_U4) && \
    !defined(CHIP_K1) && \
    !defined(CHIP_K2)
        #define ENABLE_TX_RX_RESET_CLK_GATING_ECO
#endif

//---- 6. Setting PV2MI bridge read/write burst size to minimum
#if 0 // every chip must apply it, so far
        #define _USB_MINI_PV2MI_BURST_SIZE 0
#else
        #define _USB_MINI_PV2MI_BURST_SIZE 1
#endif

//---- 7. HS connection fail problem (Gate into VFALL state)
#if defined(CHIP_KELTIC) || \
	defined(CHIP_KERES) || \
	defined(CHIP_KIRIN) || \
	defined(CHIP_KRIS) || \
	defined(CHIP_KAYLA) || \
	defined(CHIP_CLIPPERS) || \
	defined(CHIP_KRATOS) || \
	defined(CHIP_KIWI) || \
	defined(CHIP_KANO) || \
	defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_HS_CONNECTION_FAIL_INTO_VFALL_ECO
#endif

//---- 8. Enable UHC Preamble ECO function
#if defined(CHIP_KIWI) || \
	defined(CHIP_KRATOS) || \
	defined(CHIP_KAYLA) || \
	defined(CHIP_KANO) || \
	defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_UHC_PREAMBLE_ECO
#endif

//---- 9. Enable HS ISO IN Camera Cornor case ECO function
#if defined(CHIP_KRATOS) || \
	defined(CHIP_KIWI) || \
	defined(CHIP_KAYLA) || \
	defined(CHIP_KANO) || \
	defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_HS_ISO_IN_CORNER_ECO
#endif

//---- 10. Don't close RUN bit when device disconnect, default enable after KIWI
#if defined(CHIP_KERES) || \
	defined(CHIP_CLIPPERS) || \
	defined(CHIP_KIRIN) || \
	defined(CHIP_KRIS) || \
	defined(CHIP_KRATOS) || \
	defined(CHIP_KANO)
	#define ENABLE_UHC_RUN_BIT_ALWAYS_ON_ECO
#endif

//---- 11. Port is disabled when device is dosconnected
#if 0 // no chip must apply it, it can't work with RUN_BIT_ALWAYS_ON_ECO
	#define ENABLE_DISCONNECT_PE_CLR_ECO
#endif

//---- 12.
#if defined(CHIP_KRITI)
	#define ENABLE_HS_DISCONNECTION_DEBOUNCE_ECO
#endif

//---- 13. UHC speed type report should be reset by device disconnection
#if defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_DISCONNECT_SPEED_REPORT_RESET_ECO
#endif

//---- 14. Port Change Detect (PCD) is triggered by babble. Pulse trigger will not hang this condition.
/* 1'b0: level trigger
 * 1'b1: one-pulse trigger
 */
#if defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_BABBLE_PCD_ONE_PULSE_TRIGGER_ECO
#endif

//---- 15. generation of hhc_reset_u
/* 1'b0: hhc_reset is_u double sync of hhc_reset
 * 1'b1: hhc_reset_u is one-pulse of hhc_reset
 */
#if defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_HC_RESET_FAIL_ECO
#endif

//---- 16. Do SRAM clock gating automatically to save power */
#if defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
	#define ENABLE_SRAM_CLK_GATING_ECO
#endif

//------ Software patch enable switch ----------------------------------
//---- 1. Monkey Test software Patch
#if defined(CHIP_U4) || defined(CHIP_K1) || defined(CHIP_K2)
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    1
#else
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    0
#endif

//---- 2. Clock phase adjustment software Patch
#if defined(CHIP_KERES)
        #define _USB_CLOCK_PHASE_ADJ_PATCH    1
#else
        #define _USB_CLOCK_PHASE_ADJ_PATCH    0
#endif

//---- 3. enabe PVCI i_miwcplt wait for mi2uh_last_done_z
#if defined(CHIP_KERES) || \
    defined(CHIP_KIRIN) || \
    defined(CHIP_KRIS) || \
    defined(CHIP_KAYLA) || \
    defined(CHIP_CLIPPERS) || \
    defined(CHIP_KRATOS) || \
    defined(CHIP_KIWI) || \
    defined(CHIP_KANO) || \
    defined(CHIP_K6) || \
    defined(CHIP_CURRY) || \
    defined(CHIP_K6LITE) || \
    defined(CHIP_K5TN)
        #define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH    1
#else
        #define _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH    0
#endif

//---- 4. data structure (qtd ,...) must be 128-byte aligment
#define _USB_128_ALIGMENT 1

//---- 5. OBF application should enable BDMA function to move QH head
#if defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_KANO)
#define _USB_ENABLE_BDMA_PATCH
#define EHCI_CHECK_MIU1 1
	#if defined(CHIP_CURRY) || \
		defined(CHIP_KANO)
	#define EHCI_CHECK_ECO_VER 1
	#endif
	#ifdef CHIP_CURRY
	#define CHIP_BDMA_ECO_VER 0x2
	#endif
	#ifdef CHIP_KANO
	#define CHIP_BDMA_ECO_VER 0x3
	#endif
#endif

#ifdef _USB_ENABLE_BDMA_PATCH
#define MIU0_RIU_BASE (_MSTAR_PM_BASE+0x101200*2)
#define MIU1_RIU_BASE (_MSTAR_PM_BASE+0x100600*2)
#define MIU_DRAMOBF_READY_OFFSET (0x2a*2)
#define MIU_DRAMOBF_READY_BIT (1 << 15)
#define MIU_64BIT_CIPHER_OFFSET (0xd8*2)
#define MIU_64BIT_CIPHER_BIT (1 << 10)

#ifdef EHCI_CHECK_ECO_VER
#define CHIP_VER_TOP (_MSTAR_PM_BASE+0x1E00*2)
#define CHIP_VER_OFFSET (0xCE*2)
#define CHIP_VER_SHIFT 8
#define CHIP_VER_MASK 0xff
#endif
#endif
//-----------------------------------------
// U4_series_usb_init flag:
// Use low word as flag
#define EHCFLAG_NONE             0x0
#define EHCFLAG_DPDM_SWAP        0x1
#define EHCFLAG_TESTPKG          0x2
#define EHCFLAG_DOUBLE_DATARATE  0x4
#define EHCFLAG_USBBC_OFF        0x8
#define EHCFLAF_XHC_COMP         0x10
// Use high word as data
#define EHCFLAG_DDR_MASK     0xF0000000
#define EHCFLAG_DDR_x15      0x10000000 //480MHz x1.5
#define EHCFLAG_DDR_x18      0x20000000 //480MHz x1.8
//-----------------------------------------

#define HUB_STACK_SIZE  0x2000

#ifndef MSTAR_USB_DEBUG
#define MSTAR_USB_DEBUG                  0
#endif

/* Debug print definition */
#define DBG_MSG
//#define DBG_WARN
//#define DBG_FUNC
//#define DBG_DEBUG
#define DBG_ERR

#undef  ms_debug_msg
#undef  ms_debug_warn
#undef  ms_debug_err
#undef  ms_debug_func
#undef  ms_debug_debug

#ifdef DBG_MSG
#define ms_debug_msg(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_msg(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_DEBUG
#define ms_debug_debug(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_debug(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_WARN
#define ms_debug_warn(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_warn(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_ERR
#define ms_debug_err(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_err(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_FUNC
#define ms_debug_func(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_func(fmt, arg...) do {} while (0)
#endif

#define ms_usbhost_msg ms_debug_msg
#define ms_usbhost_warn ms_debug_warn
#define ms_usbhost_debug ms_debug_debug
#define ms_usbhost_err ms_debug_err

//------ UTMI disconnect level parameters ---------------------------------
// 0x00: 550mv, 0x20: 575, 0x40: 600, 0x60: 625
#if defined(CHIP_KIWI) || \
	defined(CHIP_KAYLA) || \
	defined(CHIP_KRATOS) || \
	defined(CHIP_KANO) || \
	defined(CHIP_K6) || \
	defined(CHIP_CURRY) || \
	defined(CHIP_K6LITE) || \
	defined(CHIP_K5TN)
        #define UTMI_DISCON_LEVEL_2A    (0x60)
#else
        #define UTMI_DISCON_LEVEL_2A    (0x0)
#endif

//------ UTMI eye diagram parameters ---------------------------------
#if 0
        // for 40nm
        #define UTMI_EYE_SETTING_2C     (0x98)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x10)
        #define UTMI_EYE_SETTING_2F     (0x01)
#elif 0
        // for 40nm after Agate, use 55nm setting7
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting6
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting5
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting4
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting3
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif defined(CHIP_KENYA) || \
    defined(CHIP_KAISER) || \
    defined(CHIP_KELTIC)
        #if defined(CHIP_KENYA)
                // for 40nm after Agate, use 55nm special setting2
                // STB chips only to deal with bad Dev
                // - Manufacturer:"HP" Product:"v220w"
                // - VID:0x3f0 PID:0x5a07
                #define UTMI_EYE_SETTING_2C     (0x90)
                #define UTMI_EYE_SETTING_2D     (0x02)
                #define UTMI_EYE_SETTING_2E     (0x00)
                #define UTMI_EYE_SETTING_2F     (0x01)
        #else
                // for 40nm after Agate, use 55nm setting2
                #define UTMI_EYE_SETTING_2C     (0x90)
                #define UTMI_EYE_SETTING_2D     (0x02)
                #define UTMI_EYE_SETTING_2E     (0x00)
                #define UTMI_EYE_SETTING_2F     (0x81)
        #endif
#else
        #if defined(CHIP_KERES) || \
                defined(CHIP_KIRIN) || \
                defined(CHIP_KRIS) || \
                defined(CHIP_KAYLA) || \
                defined(CHIP_KIWI) || \
                defined(CHIP_K5TN)
                // for 40nm after Agate, use 55nm special setting1
                // STB chips only to deal with bad Dev
                // - Manufacturer:"HP" Product:"v220w"
                // - VID:0x3f0 PID:0x5a07
                #define UTMI_EYE_SETTING_2C     (0x10)
                #define UTMI_EYE_SETTING_2D     (0x02)
                #define UTMI_EYE_SETTING_2E     (0x00)
                #define UTMI_EYE_SETTING_2F     (0x01)
        #else
                // for 40nm after Agate, use 55nm setting1, the default
                #define UTMI_EYE_SETTING_2C     (0x10)
                #define UTMI_EYE_SETTING_2D     (0x02)
                #define UTMI_EYE_SETTING_2E     (0x00)
                #define UTMI_EYE_SETTING_2F     (0x81)
        #endif
#endif

#endif

