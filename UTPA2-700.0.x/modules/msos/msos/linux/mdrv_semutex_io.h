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

// PUT THIS FILE under INLCUDE path

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_temp_io.h
/// @brief  TEMP Driver Interface.
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SEMUTEX_IO_H_
#define _MDRV_SEMUTEX_IO_H_

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define SEMUTEX_IOC_MAGIC               'S'
#define MUTEX_INDEX_BEGIN               0xFF
#ifdef CONFIG_KERN_3_10_40
#define SEMAPHORE_INDEX_BEGIN           0x3FF
#else
#define SEMAPHORE_INDEX_BEGIN           0x55660000
#endif

/* IOCTL functions. */
/* int ioctl(int fd,
             int request == MDRV_SEMUTEX_LOCK);
*/
#define MDRV_SEMUTEX_LOCK                (int)_IO(SEMUTEX_IOC_MAGIC, 0U)

/* int ioctl(int fd,
             int request == MDRV_SC_DETACH_INTERRUPT);
*/
#define MDRV_SEMUTEX_UNLOCK                (int)_IO(SEMUTEX_IOC_MAGIC, 1U)

/* int ioctl(int fd,
             int request == MDRV_SC_RESET_FIFO);
*/
#define MDRV_SEMUTEX_CREATE_SHAREMEMORY                      (int)_IO(SEMUTEX_IOC_MAGIC, 2U)

/* int ioctl(int fd,
             int request == MDRV_SC_GET_EVENTS,
             int *events);
*/
#define MDRV_SEMUTEX_MAP_SHAREMEMORY                      (int)_IOR(SEMUTEX_IOC_MAGIC, 3U, int)

#define MDRV_SEMUTEX_UNMAP_SHAREMEMORY                      (int)_IOR(SEMUTEX_IOC_MAGIC, 4U, int)

#define MDRV_SEMUTEX_CREATE_MUTEX                      (int)_IOR(SEMUTEX_IOC_MAGIC, 5U, int)

#define MDRV_SEMUTEX_DEL_MUTEX                      (int)_IOR(SEMUTEX_IOC_MAGIC, 6U, int)

#define MDRV_SEMUTEX_TRY_LOCK                       (int)_IOR(SEMUTEX_IOC_MAGIC, 7U, int)

#define MDRV_SEMUTEX_QUERY_ISFIRST_SHAREMEMORY                      (int)_IO(SEMUTEX_IOC_MAGIC, 8U)

#define MDRV_SEMUTEX_LOCK_WITHTIMEOUT                      (int)_IOR(SEMUTEX_IOC_MAGIC, 9U, int)

#define MDRV_SEMUTEX_EXPAND_SHAREMEMORY                      (int)_IOR(SEMUTEX_IOC_MAGIC, 10U, int)

#define MDRV_SEMUTEX_GET_SHMSIZE		            (int)_IOR(SEMUTEX_IOC_MAGIC, 11U, unsigned int)

#define MDRV_SEMUTEX_CREATE_SEMAPHORE               (int)_IOR(SEMUTEX_IOC_MAGIC, 12U, int)

#define MDRV_SEMUTEX_SEMA_LOCK                      (int)_IOR(SEMUTEX_IOC_MAGIC, 13U, int)

#define MDRV_SEMUTEX_SEMA_TRY_LOCK                  (int)_IOR(SEMUTEX_IOC_MAGIC, 14U, int)

#define MDRV_SEMUTEX_SEMA_UNLOCK                    (int)_IOR(SEMUTEX_IOC_MAGIC, 15U, int)

#define MDRV_SEMUTEX_SEMA_RESET                     (int)_IOR(SEMUTEX_IOC_MAGIC, 16U, int)

#define MDRV_SEMUTEX_SET_CROSS_THREAD_UNLOCK        (int)_IOR(SEMUTEX_IOC_MAGIC, 17U, CROSS_THREAD_UNLOCK_INFO)

#define SEMA_NAME_LEN   64

typedef struct{
int index;
int time;
}LOCKARG;

typedef struct{
    unsigned int    semanum;
    char            semaname[SEMA_NAME_LEN];
}CREATE_SEMA_ARG;

typedef enum
{
    E_CROSS_THREAD_UNLOCK_ENABLE = 0,
    E_CROSS_THREAD_UNLOCK_DISABLE,
}CROSS_THREAD_UNLOCK_FLAG;

typedef struct{
    int index;
    CROSS_THREAD_UNLOCK_FLAG flag;
}CROSS_THREAD_UNLOCK_INFO;

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//MS_S32 MsOS_CreateNamedSemaphore(char *ps8SemaName, MS_U32 u32SemaNum);

MS_BOOL MsOS_ResetNamedSemaphore(MS_S32 u32Index);

//MS_BOOL MsOS_ObtainNamedSemaphore(MS_S32 u32Index, MS_S32 flag);

//MS_BOOL MsOS_ReleaseNamedSemaphore(MS_S32 u32Index, MS_S32 flag);

#endif // SEMUTEX

