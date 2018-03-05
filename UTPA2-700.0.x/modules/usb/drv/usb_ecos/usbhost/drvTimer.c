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

//=============================================================================
#include <MsCommon.h>

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
#include "include/drvList.h"

U32 volatile jiffies;

//void SW_Timer_Tick(U32 u32Value);
void SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID);

static struct list_head sw_timer_vec_head;

S32 g_Timer_ID;

#ifdef TIMER_NO_ALL_INT_DISABLE
#include "drvHUB.h"
extern struct s_ChipUsbHostDef *pCurrentChip;
void disable_USB_interrupts(void)
{
    U8 i;

    for (i=0; i<pCurrentChip->nRootHub; i++)
        MsOS_DisableInterrupt(pCurrentChip->reg[i].uhcIRQ);
}

void enable_USB_interrupts(void)
{
    U8 i;
    
    for (i=0; i<pCurrentChip->nRootHub; i++)
        MsOS_EnableInterrupt(pCurrentChip->reg[i].uhcIRQ);
}
#endif

S32 _s32MutexUTimer;
void init_timers(void)
{
	INIT_LIST_HEAD(&sw_timer_vec_head);
	// Register software timer dispatch program
	// Base 1 ms, interruptted every 1 ms

	BOOL err;

	g_Timer_ID = MsOS_CreateTimer((TimerCb) SW_Timer_Tick,100, 100, FALSE, "USB_TIMER");
	//err = OS_DefineCyclicHandler(&g_Timer_ID, SW_Timer_Tick, (void*) 0, 100);
	//USB_ASSERT((err == TRUE), "Define USB timer fail");

	//err = OS_ActivateCyclicHandler(g_Timer_ID);
	//
	err=MsOS_StartTimer(g_Timer_ID);
	USB_ASSERT((err == TRUE), "Activate USB timer fail");
	//kdbg("Init Software Timer ok %s","\n");
	printk("Init Software Timer ok \n");
        _s32MutexUTimer = MsOS_CreateMutex(E_MSOS_FIFO, "UTimer_MUTEX", MSOS_PROCESS_SHARED);
}

void remove_timers(void)
{
	MsOS_DeleteTimer(g_Timer_ID);
	printk("Remove Software Timer ok \n");
        MsOS_DeleteMutex(_s32MutexUTimer);
}

/*
 * Timer ISR: It's responsible for dispatching tasks registered to software-timer
 * task queue. It need to be implemented as Bottom-Half mechanism of Linux
 * Timer and USB IRQ have the same priority and cannot be interrupted by each other
 */
//void SW_Timer_Tick(U32 pData)
void SW_Timer_Tick(U32 u32StTimer, U32 u32TimerID)
{
	struct timer_list *timer;
	struct list_head  *tmp;

	jiffies+=100;

	if ( Timer_task_lock == 1 )
		goto sw_done;

	// Dispath task from task queue of software-timer here
	if (!list_empty(&sw_timer_vec_head))
	{
		tmp = sw_timer_vec_head.next;
		while (tmp != &sw_timer_vec_head)
		{
			timer = list_entry (tmp, struct timer_list, entry);
			tmp = tmp->next;
			//list_entry (tmp, struct timer_list, entry, struct list_head, timer);
			// Check if time expired ?
			timer->expires -= 100;
			if ( timer->expires <= 0 )
			{
				void (*fn)(U32);
				U32 data;
				// If lock ==> don't dispatch task
				/*
				if ( Timer_task_lock == 1 )
				{
				  timer->expires++;
				  continue;
				}
				*/
				fn = timer->function;
				data = timer->data;
				del_timer(timer);
				// Call registered function
				if ( (U32)fn )
				{
					lock_usb_core();
					fn(data);
					unlock_usb_core();
				}
				else
				{
					printk("ERROR: fun is NULL\n");
					while(1);
				}
				//Current version only support dispatch one task each time
				//break;
			}
			/*
			if ( (U32)tmp->next == (U32)0x1 )
			while(1);
			if ( tmp->next == NULL )
			while(1);*/
		}
	}
sw_done:
	return;
	// End of dispatching
}

#ifdef TIMER_NO_ALL_INT_DISABLE
void timerSectionIn(void)
{
    MsOS_StopTimer(g_Timer_ID);
    disable_USB_interrupts();
    MsOS_ObtainMutex(_s32MutexUTimer, MSOS_WAIT_FOREVER);
}
void timerSectionOut(void)
{
    enable_USB_interrupts();
    MsOS_StartTimer(g_Timer_ID);
    MsOS_ReleaseMutex(_s32MutexUTimer);
}
int del_timer_body(struct timer_list *timer)
{
     //MsOS_StopTimer(g_Timer_ID);
     //disable_USB_interrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
	return 1;
}
int del_timer(struct timer_list *timer)
{
    //timerSectionIn();
    del_timer_body(timer);
    //timerSectionOut();
    return 1;
}
int del_timer_sync_body(struct timer_list *timer)
{
    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
	return 1;
}
int del_timer_sync(struct timer_list *timer)
{
    timerSectionIn();
    del_timer_sync_body(timer);
    timerSectionOut();
    return 1;
}
void add_timer_body(struct timer_list *timer)
{
    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;

	list_add_tail(&timer->entry, &sw_timer_vec_head);

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	//TIMER_TASK_LOCK = flags;
}
void add_timer(struct timer_list *timer)
{
    timerSectionIn();
    add_timer_body(timer);
    timerSectionOut();
}
int mod_timer_body(struct timer_list *timer, U32 expires, U32 steps)
{
	//U32 flags;
	int ret = 0;

    //MsOS_StopTimer(g_Timer_ID);
    //disable_USB_interrupts();

	timer->tmout_value = expires;
	timer->tmout_step = steps;

	//ret = del_timer(timer);
	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//add_timer(timer);
	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;
	list_add_tail(&timer->entry, &sw_timer_vec_head);

     //enable_USB_interrupts();
     //MsOS_StartTimer(g_Timer_ID);

	return ret;
}
int mod_timer(struct timer_list *timer, U32 expires, U32 steps)
{    
    int ret = 0;
    //timerSectionIn();
    ret = mod_timer_body(timer, expires, steps);
    //timerSectionOut();
    return ret;
}
#else // Disable All Interrupts
/***
 * del_timer - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * del_timer() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function returns whether it has deactivated a pending timer or not.
 * (ie. del_timer() of an inactive timer returns 0, del_timer() of an
 * active timer returns 1.)
 */
int del_timer(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();yuwen
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
	 U32 u32OldIntr;
     u32OldIntr = MsOS_DisableAllInterrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();yuwen
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
	return 1;
}

/***
 * del_timer_sync - deactive a timer.
 * @timer: the timer to be deactivated
 *
 * del_timer_sync() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function will protect synchronization problem before deactivating timer
 */
int del_timer_sync(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
     U32 u32OldIntr;
     u32OldIntr = MsOS_DisableAllInterrupts();

	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
	return 1;
}

/***
 * add_timer - start a timer
 * @timer: the timer to be added
 *
 * The kernel will do a ->function(->data) callback from the
 * timer interrupt at the ->expired point in the future. The
 * current time is 'jiffies'.
 *
 * The timer's ->expired, ->function (and if the handler uses it, ->data)
 * fields must be set prior calling this function.
 *
 * Timers with an ->expired field in the past will be executed in the next
 * timer tick. It's illegal to add an already pending timer.
 */
void add_timer(struct timer_list *timer)
{
	//U32 flags;

	//flags = TIMER_TASK_LOCK;
	//lock_TmrTaskDispatch();
	//save_and_disable_firqs(&flags);
	//HAL_DISABLE_INTERRUPTS(flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;

	list_add_tail(&timer->entry, &sw_timer_vec_head);

	//restore_firqs(&flags);
	//HAL_RESTORE_INTERRUPTS(flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	//TIMER_TASK_LOCK = flags;
}

int mod_timer(struct timer_list *timer, U32 expires, U32 steps)
{
	//U32 flags;
	int ret = 0;

	//save_and_disable_firqs(&flags);
	//OS_DisableDispatch();
	//MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
    U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

	timer->tmout_value = expires;
	timer->tmout_step = steps;

	//ret = del_timer(timer);
	if( timer->entry.next != NULL && timer->entry.prev != NULL )
	{
		USB_ASSERT(timer->entry.prev->next == &timer->entry, "timer->entry.prev->next error!!!!\n");
		USB_ASSERT(timer->entry.next != 0, "timer->entry.next!!!!\n");
		USB_ASSERT(timer->entry.next->prev == &timer->entry, "timer->entry.next->prev error!!!!\n");
		USB_ASSERT(timer->entry.prev != 0, "timer->entry.prev error!!!!\n");
		list_del_init(&timer->entry);
	}

	//add_timer(timer);
	if ( (timer->tmout_step > 0) && (timer->tmout_value > timer->tmout_step) )
		timer->expires = timer->tmout_step;
	else
		timer->expires = timer->tmout_value;
	list_add_tail(&timer->entry, &sw_timer_vec_head);

	//restore_firqs(&flags);
	//OS_EnableDispatch();
	//MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
     MsOS_RestoreAllInterrupts(u32OldIntr);

	return ret;
}
#endif	//#ifdef

