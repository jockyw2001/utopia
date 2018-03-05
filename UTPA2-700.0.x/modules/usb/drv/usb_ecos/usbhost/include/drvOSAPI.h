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

 
#ifndef _OS_API_H_
#define _OS_API_H_

#include <MsCommon.h>
#include <cyg/hal/hal_if.h>		
// Note, include for kernel service functions of eCos
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_intr.h>
#include "drvPorts.h"

#define IRQ_NONE              0
#define IRQ_HANDLED           1
#define TASK_UNINTERRUPTIBLE  0
/* ========================================  Data structures  ======================================== */

typedef int spinlock_t;
typedef int atomic_t;

#define SPIN_LOCK_UNLOCKED  (spinlock_t)0
#define SPIN_LOCK_LOCKED    (spinlock_t)1

/* ========================================  Function Declaration  ======================================== */
#define DECLARE_WAITQUEUE(x,y)
#define init_waitqueue_head(x)
#define add_wait_queue(x,y)
#define remove_wait_queue(x,y)
#define wake_up(x)
#define signal_pending(x)     0

// Spin lock functions
#define spin_lock_init(lock)
#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_irqsave(lock,flag)      save_and_disable_firqs(&flag)
#define spin_unlock_irqrestore(lock,flag) restore_firqs(&flag)
#define spin_lock_irq(lock)
#define spin_unlock_irq(lock)
// Semaphore functions
#define up(x)
#define down(x)
// Mutex functions
#define init_MUTEX(x)
#define init_MUTEX_LOCKED(x)
#define atomic_read(v)        *(v)
#define ATOMIC_INIT(value)    value
// Kernel function porting (Unsupport now, so just skip)
#define set_current_state(x)
#define local_irq_restore(x)
#define local_irq_save(x)
#define local_irq_disable()
#define local_irq_enable()
#define lock_kernel()
#define unlock_kernel()
#define schedule()

static __inline__ void atomic_inc(int *cnt_p)
{
  (*cnt_p)++;
}

static __inline__ void atomic_dec(int *cnt_p)
{
  (*cnt_p)--;
}

static __inline__ int atomic_dec_and_test(int *count)
{
  (*count)--;
  return (*count == 0);
}

static __inline__ void atomic_set(int *count, int value)
{
  (*count) = value;
}

static __inline__ void schedule_timeout(int ms)
{
	mdelay(ms);
}
//extern int schedule_timeout(int ms);
#endif

