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
/**************************************************************************
 * Copyright (c) 2008 MStar Semiconductor, Inc. All Rights Reserved.
 **************************************************************************/
/** \file
 * \brief This is the header file for the FrHsl ring buffer module.
 *
 */

#ifndef FRHSL_RINGBUFFER_H
#define FRHSL_RINGBUFFER_H

#include <pthread.h>

#include "apiHSL.h"
#if HSL_TCPIP == true
#include "frhsl_autotest.h"
#include "frhsl_cfg.h"

#endif

#ifdef __cpluplus
extern "C"
{
#endif

#if defined(MSOS_TYPE_LINUX)

#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

enum HSLOutputType
{
    HSL_UART = 'U',
    HSL_FILE = 'F',
    HSL_SOCKET = 'S',
};

#define FR_HSL_FILE_DESCRIPTOR             (HSL_UART)

#define HSL_DEVICE_PATH "/dev/hsl"
#define CANNOT_OPEN_MSG "Cannot open /dev/hsl\n"

#define HSL_IOC_MAGIC               'h'

#define MDRV_HSL_SET_OUTPUT_PATH                _IOW(HSL_IOC_MAGIC, 1, MS_U8)
#define MDRV_HSL_CLEAR_OUTPUT_FILE                _IO(HSL_IOC_MAGIC, 2)

#if HSL_TCPIP
#define PUTCS(dataptr, length, port, hsl) CmdSend(hsl, dataptr, length)
#define FFLUSH(port)
#else
#define PUTCS(dataptr, length, port, hsl) \
      {\
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      if (0==ioctl(fd, MDRV_HSL_SET_OUTPUT_PATH, (port)) ) \
      { \
        write(fd, (dataptr), (length)); \
      } \
      close(fd); \
    } \
      }
  
#define FFLUSH(port) \
      { \
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      if (0==ioctl(fd, MDRV_HSL_SET_OUTPUT_PATH, (port)) ) \
      { \
        fsync(fd); \
      } \
      close(fd); \
    } \
      }
#endif
  
#define HSL_CLEAR_OUTPUT_FILE() \
      { \
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      ioctl(fd, MDRV_HSL_CLEAR_OUTPUT_FILE); \
      close(fd); \
    } \
      }
  
#else
  
#define FR_HSL_FILE_DESCRIPTOR             (STDOUT_FILENO)
  
#define PUTCS(dataptr, length, port) \
      { \
    MS_U32 i; \
    for( i=0 ; i<length ; i++) \
    { \
      write(port, dataptr, length); \
    } \
      }

#define FFLUSH(port)        fsync(port)

#endif

#define BUFFER_FLAG_NONE            0x0000
#define BUFFER_FLAG_DUMP            0x0001
#define BUFFER_FLAG_EMERGENCY_DUMP  0x0002
#define BUFFER_FLAG_TX              0x0004
#define BUFFER_FLAG_FLUSH            0x0008

struct stListNode;

MS_BOOL bufferInitialize(stHslController* const _hsl, const MS_PHYADDR physAddr, const MS_U32 size);
MS_BOOL bufferFinalize(stHslController* const _hsl);
MS_BOOL bufferAllocateWrite(stHslController* const _hsl, const MS_U32 dataSize, stListNode** const s_dataInfo);
MS_BOOL bufferDumpOldestDataSegment(stHslController* const _hsl);
void bufferAcknowledgeWrite(stHslController* const _hsl, stListNode* const s_dataInfo);
MS_BOOL bufferProcessCommand(stHslController* const _hsl, CmdInfo* const pCmd);

// Function prototypes
void *bufferWriteThread(void*);        // output thread function
MS_BOOL initThread( hslThread* aThread);

#ifdef __cpluplus
}
#endif

#endif // FRHSL_RINGBUFFER_H
