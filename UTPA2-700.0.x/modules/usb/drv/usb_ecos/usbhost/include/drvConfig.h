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


#ifndef _CONFIG_H_
#define _CONFIG_H_

// Define Hardware configuration constant macro

#define __LINUX_ARM_ARCH__ 4    //  Architecture 4

/* Mass Storage Class Driver related CONFIG macro definition*/
//#define CONFIG_USB_STORAGE_DEBUG
//#define CONFIG_USB_SCSI_DEBUG
/* =======================================================*/
/* Test application related CONFIG macro definitions */
#define CONFIG_MASS_STORAGE_TEST
//#define CONFIG_USB_PRINTER_TEST
//#define CONFIG_USB_POLLING_ONLY_TEST

/* =======================================================*/
/* Macros for Speed limitation */
//#define FORCE_FULL_SPEED
//#ifdef FORCE_FULL_SPEED
//#undef FORCE_HIGH_SPEED
//#else
//#define FORCE_HIGH_SPEED
//#endif
/* =======================================================*/
/* OTG application related macro definitions */
//#define CONFIG_OTG
//#define CONFIG_OPT_TESTING
//#define CONFIG_HOST_AP_BURNIN  //Do specific procedures then leave
#define OTG_SRP_DATA_PULSING
//#define OTG_SRP_VBUS_PULSING

// for FPGA host test mode
//#define CONFIG_HC_TEST

#ifdef CONFIG_OPT_TESTING
#undef  CONFIG_OTG
//#define CONFIG_OTG
#endif
/* =======================================================*/
//#define CONFIG_FARADAY_USBH100
//#define CONFIG_FARADAY_FOTG100
//#define CONFIG_FARADAY_FOTG200

//#ifdef CONFIG_FARADAY_USBH100
//#undef CONFIG_OTG
//#undef CONFIG_OPT_TESTING
//#undef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_FOTG200
//#endif

//#ifdef CONFIG_FARADAY_FOTG200
//#undef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_USBH100
//#endif

//#ifdef CONFIG_FARADAY_FOTG100
//#undef CONFIG_FARADAY_FOTG200
//#undef  CONFIG_FARADAY_USBH100
//#define CONFIG_FARADAY_USBH100
//#endif
/* ======================================================*/
/* Transfer types support macro definitions for USB 2.0 */

#ifdef CONFIG_FARADAY_FOTG200
//#define have_split_iso
#define have_iso_itd
#define have_fstn
#endif
// Frequecy of kernel timer interrupt
#define HZ                 1000
#undef  __ARMEB__                // For Little Endian (CPU is little endian)
#define __ARMEL__                // For Little Endian (CPU is little endian)
/* ======================================================*/
/* DEBUG fucniton macro definitions */
//#define SHOW_MSG
//#define DEBUG
//#define CONFIG_DEBUG
//#define CONFIG_USB_DEBUG
//#define VERBOSE_DEBUG
//#define KERNEL_DEBUG
#ifndef CONFIG_FARADAY_FOTG200
#ifdef VERBOSE_DEBUG
#define OHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#else
#ifdef VERBOSE_DEBUG
#define EHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#endif

#define __LITTLE_ENDIAN 1234

#endif

