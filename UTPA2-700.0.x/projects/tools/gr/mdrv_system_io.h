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

#define IO_SYS_GET_RAW_UART	// not used for LGE (dreamer@lge.com)
//#define IO_SYS_REG_OP      	// not used for LGE (dreamer@lge.com)

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
#define IOCTL_SYS_IS_AEON_ENABLE	   _IOR (SYS_IOCTL_MAGIC, 0x0C, int)
#define IOCTL_SYS_CHANGE_UART          _IOWR(SYS_IOCTL_MAGIC, 0x0D, int)

#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

#ifdef IO_SYS_REG_OP
#   define IOCTL_SYS_REG_OP            _IOWR(SYS_IOCTL_MAGIC, 0x54, int)
#endif

#ifdef IO_SYS_GET_RAW_UART
#define IOCTL_SYS_GET_RAW_UART         _IOWR(SYS_IOCTL_MAGIC, 0x55, int) //terry, change ID 56 -> 55
#define IOCTL_SYS_SEND_RAW_UART        _IOWR(SYS_IOCTL_MAGIC, 0x56, int) 
#endif

#define IOCTL_SYS_TIMER                _IOWR(SYS_IOCTL_MAGIC, 0x57, int)
#define IOCTL_SYS_RELOAD_AEON          _IOWR(SYS_IOCTL_MAGIC, 0x58, int)

#if 0
//#define IOCTL_SYS_SWITCH_PAD        _IOWR(SYS_IOCTL_MAGIC, 0x01, DevSys_Switch_Pad)
#define IOCTL_SYS_WDT_ENABLE        _IOW (SYS_IOCTL_MAGIC, 0x02, MS_U32)
#define IOCTL_SYS_WDT_CLEAR         _IO  (SYS_IOCTL_MAGIC, 0x03)
#define IOCTL_SYS_WDT_LASTSTATUS    _IOWR(SYS_IOCTL_MAGIC, 0x04, MS_U32)
#define IOCTL_SYS_WDT_SETTIME       _IOW (SYS_IOCTL_MAGIC, 0x05, MS_U32)
#define IOCTL_SYS_RESET_CHIP        _IO  (SYS_IOCTL_MAGIC, 0x06)
#define IOCTL_SYS_RESET_CPU         _IO  (SYS_IOCTL_MAGIC, 0x07)
#endif

#define IOCTL_SYS_HOTEL_MODE           _IOWR(SYS_IOCTL_MAGIC, 0x59, int)
#define IOCTL_SYS_HOTEL_MODE_PRINTF    _IOWR(SYS_IOCTL_MAGIC, 0x5A, int)

#define IOCTL_SYS_SETSEQFILE           _IOW (SYS_IOCTL_MAGIC, 0x60, int)

// Samuel, bad HDD timeout, 090115
#define IOCTL_SYS_SETVBUS              _IOW (SYS_IOCTL_MAGIC, 0x61, int)

//20090724 Terry, URSA ISP Load Code
#define IOCTL_SYS_GETSPI               _IOWR (SYS_IOCTL_MAGIC, 0x62, int)

#define IOCTL_SYS_GETREV               _IOR (SYS_IOCTL_MAGIC, 0x63, int)

//20091028, Terry, MIU protect
#define IOCTL_SYS_MIU_PROTECT          _IOWR (SYS_IOCTL_MAGIC, 0x64, int)

#define IOCTL_SYS_SETUART_MODE         _IOWR (SYS_IOCTL_MAGIC, 0x65, int)

#define IOCTL_SYS_SPI_LOAD             _IOWR (SYS_IOCTL_MAGIC, 0x66, int)

#define IOCTL_SYS_MAXNR    0xFF

#endif // __DRV_SYSTEM_IO_H__

