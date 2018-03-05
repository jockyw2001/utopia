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


#ifndef _LINUX_TIMER_H_
#define _LINUX_TIMER_H_

#include <MsTypes.h>
#include "drvPorts.h"
#include "drvOSAPI.h"
#include "drvList.h"

struct timer_list {
  struct list_head entry;
  int expires;
  int base_jiffies;

  spinlock_t lock;
  U32 magic;

  void (*function)(U32);
  U32 data;
  int tmout_value;
  int tmout_step;
  U32 data_ex;
};

#define TIMER_MAGIC  0x4b87ad6e

/*
#define TIMER_INITIALIZER(_function, _expires, _data) {    \
    .function = (_function),      \
    .expires = (_expires),        \
    .data = (_data),        \
    .base = NULL,          \
    .magic = TIMER_MAGIC,        \
    .lock = SPIN_LOCK_UNLOCKED,      \
  }
*/

#define TIMER_INITIALIZER(var, _function, _expires, _data) {    \
    var.function = (_function),      \
    var.expires = (_expires),        \
    var.data = (_data),        \
    var.base = NULL,          \
    var.magic = TIMER_MAGIC,        \
    var.lock = SPIN_LOCK_UNLOCKED,      \
  }

/***
 * init_timer - initialize a timer.
 * @timer: the timer to be initialized
 *
 * init_timer() must be done to a timer prior calling *any* of the
 * other timer functions.
 */

static __inline__ void init_timer(struct timer_list * timer)
{
  timer->magic = TIMER_MAGIC;
  timer->tmout_step = 0;
  timer->tmout_value = 0;
  INIT_LIST_HEAD(&timer->entry);
  spin_lock_init(&timer->lock);
}

/***
 * timer_pending - is a timer pending?
 * @timer: the timer in question
 *
 * timer_pending will tell whether a given timer is currently pending,
 * or not. Callers must ensure serialization wrt. other operations done
 * to this timer, eg. interrupt contexts, or other CPUs on SMP.
 *
 * return value: 1 if the timer is pending, 0 if not.
 */
 
#define timer_pending(x)  ((x)->expires>0)
/*
 *  These inlines deal with timer wrapping correctly. You are 
 *  strongly encouraged to use them
 *  1. Because people otherwise forget
 *  2. Because if the timer wrap changes in future you won't have to
 *     alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a,b)    ( ((int)(b) - (int)(a)) < 0 )
#define time_before(a,b)  time_after(b,a)

#define time_after_eq(a,b)  ( ((int)(a) - (int)(b)) >= 0 )
#define time_before_eq(a,b)  time_after_eq(b,a)


extern U32 volatile jiffies;

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);
extern int del_timer_sync(struct timer_list * timer);
extern int mod_timer(struct timer_list *timer, U32 expires, U32 steps);
extern void init_timers(void);
extern void remove_timers(void);

#endif

