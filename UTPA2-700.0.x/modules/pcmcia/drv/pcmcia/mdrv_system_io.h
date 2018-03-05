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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_SYSTEM_IO_H__
#define __DRV_SYSTEM_IO_H__

#define IO_SYS_GET_RAW_UART
#define IO_SYS_REG_OP

//-------------------------------------------------------------------------------------------------
//  ioctl method
//-------------------------------------------------------------------------------------------------

// Use 'S' as magic number
#define SYS_IOCTL_MAGIC             'S'

#define IOCTL_SYS_INIT                 _IOWR(SYS_IOCTL_MAGIC, 0x00, int)
#define IOCTL_SYS_SET_PANEL_INFO       _IOW (SYS_IOCTL_MAGIC, 0x01, int)
#define IOCTL_SYS_SET_BOARD_INFO       _IOW (SYS_IOCTL_MAGIC, 0x02, int)
#define IOCTL_SYS_GET_PANEL_RES        _IOR (SYS_IOCTL_MAGIC, 0x03, int)
#define IOCTL_SYS_READ_GEN_REGISTER    _IOR (SYS_IOCTL_MAGIC, 0x04, int)
#define IOCTL_SYS_WRITE_GEN_REGISTER   _IOWR(SYS_IOCTL_MAGIC, 0x05, int)
#define IOCTL_SYS_LOAD_AEON            _IOWR(SYS_IOCTL_MAGIC, 0x06, int)
#define IOCTL_SYS_RESET_AEON           _IOWR(SYS_IOCTL_MAGIC, 0x07, int)
#define IOCTL_SYS_ENABLE_AEON          _IO(SYS_IOCTL_MAGIC, 0x08)
#define IOCTL_SYS_DISABLE_AEON         _IO(SYS_IOCTL_MAGIC, 0x09)
#define IOCTL_SYS_SWITCH_UART          _IOR (SYS_IOCTL_MAGIC, 0x0A, int)
#define IOCTL_SYS_DUMP_AEON_MSG        _IOR (SYS_IOCTL_MAGIC, 0x0B, int)
#define IOCTL_SYS_IS_AEON_ENABLE       _IOR (SYS_IOCTL_MAGIC, 0x0C, int)
#define IOCTL_SYS_CHANGE_UART          _IOWR(SYS_IOCTL_MAGIC, 0x0D, int)
#define IOCTL_SYS_DDC2BI               _IOWR(SYS_IOCTL_MAGIC, 0x0E, int) //2008/10/23 Nick

#define IOCTL_SYS_SET_GFX_GOP_INDEX     _IOW (SYS_IOCTL_MAGIC, 0x11,int)
#define IOCTL_SYS_GET_GFX_GOP_INDEX     _IOR (SYS_IOCTL_MAGIC, 0x12,int)

#define IOCTL_SYS_SET_DISPLAY_CTLR_SEPT_INDEX       _IOW (SYS_IOCTL_MAGIC, 0x15,int)
#define IOCTL_SYS_IS_DISPLAY_CTLR_SEPT_INDEX        _IOR (SYS_IOCTL_MAGIC, 0x16,int)

#define IOCTL_SYS_SET_NEXUS     _IOW (SYS_IOCTL_MAGIC, 0x17,int)
#define IOCTL_SYS_HAS_NEXUS     _IOR (SYS_IOCTL_MAGIC, 0x18,int)

#define IOCTL_SYS_PRING_MSG             _IOW (SYS_IOCTL_MAGIC, 0x19,int)
#define IOCTL_SYS_GET_GFX_GOP_PIPELINE_DELAY        _IOWR (SYS_IOCTL_MAGIC, 0x1A,int)
#define IOCTL_SYS_GET_PANEL_H_START     _IOR (SYS_IOCTL_MAGIC, 0x1B,int)

#define IOCTL_SYS_SET_NEXUS_PID     _IOW (SYS_IOCTL_MAGIC, 0x1C,int)
#define IOCTL_SYS_GET_NEXUS_PID     _IOR (SYS_IOCTL_MAGIC, 0x1D,int)

#define IOCTL_SYS_PCMCIA_WRITE      _IOW(SYS_IOCTL_MAGIC, 0x40,int)
#define IOCTL_SYS_PCMCIA_READ       _IOR(SYS_IOCTL_MAGIC, 0x41,int)
#define IOCTL_SYS_PCMCIA_WRITE_DATA       _IOW(SYS_IOCTL_MAGIC, 0x42,int)
#define IOCTL_SYS_PCMCIA_READ_DATA       _IOR(SYS_IOCTL_MAGIC, 0x43,int)

#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

#ifdef IO_SYS_REG_OP
#   define IOCTL_SYS_REG_OP            _IOWR(SYS_IOCTL_MAGIC, 0x54, int)
#endif

#ifdef IO_SYS_GET_RAW_UART
#   define IOCTL_SYS_GET_RAW_UART      _IOWR(SYS_IOCTL_MAGIC, 0x55, int)
#endif

#define IOCTL_SYS_TIMER                _IOWR(SYS_IOCTL_MAGIC, 0x56, int)
#define IOCTL_SYS_RELOAD_AEON          _IOWR(SYS_IOCTL_MAGIC, 0x57, int)

#if 0
//#define IOCTL_SYS_SWITCH_PAD        _IOWR(SYS_IOCTL_MAGIC, 0x01, DevSys_Switch_Pad)
#define IOCTL_SYS_WDT_ENABLE        _IOW (SYS_IOCTL_MAGIC, 0x02, MS_U32)
#define IOCTL_SYS_WDT_CLEAR         _IO  (SYS_IOCTL_MAGIC, 0x03)
#define IOCTL_SYS_WDT_LASTSTATUS    _IOWR(SYS_IOCTL_MAGIC, 0x04, MS_U32)
#define IOCTL_SYS_WDT_SETTIME       _IOW (SYS_IOCTL_MAGIC, 0x05, MS_U32)
#define IOCTL_SYS_RESET_CHIP        _IO  (SYS_IOCTL_MAGIC, 0x06)
#define IOCTL_SYS_RESET_CPU         _IO  (SYS_IOCTL_MAGIC, 0x07)
#endif

#define IOCTL_SYS_HOTEL_MODE           _IOWR(SYS_IOCTL_MAGIC, 0x58, int)
#define IOCTL_SYS_HOTEL_MODE_PRINTF    _IOWR(SYS_IOCTL_MAGIC, 0x59, int)
#define IOCTL_SYS_POWER_DOWN           _IOWR(SYS_IOCTL_MAGIC, 0x60, int)
#define IOCTL_SYS_GET_MBOX_SHM          _IOWR(SYS_IOCTL_MAGIC, 0x61, int)
#define IOCTL_SYS_GET_MSBIN_INFO          _IOWR(SYS_IOCTL_MAGIC, 0x62, int)

#define IOCTL_SYS_MAXNR    0xFF

#endif // __DRV_SYSTEM_IO_H__

