#include "utopia_adp.h"
#include "utopia.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/spinlock.h>

#include <linux/list.h>


#define DEBUG  1
#define USE_MUTEX 0
#define USE_SPINLOCK 1

typedef struct KQueue {
    char*   pointer;
    struct  list_head list;
} KQueueNode ;


static struct list_head KQueue_head_list[MODULE_NUM];
#if USE_MUTEX
static struct mutex KQueue_mutex[MODULE_NUM];
#endif
static wait_queue_head_t wq[MODULE_NUM];
static spinlock_t KQueue_spinlock[MODULE_NUM];

char our_thread[8]="thread1";
int j =4;
int *k;

char * UADPKQueuePop(MS_U32 module_ID)
{

    KQueueNode * pKQueueNode = NULL;
    unsigned long   dwLockFlag = 0;

    if(UADPKQueueIsempty(module_ID))
    {
        wait_event_interruptible(wq[module_ID],!UADPKQueueIsempty(module_ID));
    }
    #if USE_MUTEX
        mutex_lock(&KQueue_mutex[module_ID]);
    #endif

    #if USE_SPINLOCK
        spin_lock_irqsave(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif
        pKQueueNode = (KQueueNode *)list_entry((KQueue_head_list[module_ID].next),KQueueNode,list);
        list_del(KQueue_head_list[module_ID].next);

    #if USE_MUTEX
        mutex_unlock(&KQueue_mutex[module_ID]);
    #endif

    #if USE_SPINLOCK
        spin_unlock_irqrestore(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    #if DEBUG
    {
        printk("get value%d\n",* (pKQueueNode -> pointer) );
    }
    #endif

    return pKQueueNode -> pointer;
}
EXPORT_SYMBOL(UADPKQueuePop);


void UADPKQueuePush(MS_U8 module_ID,char *pointer)
{
    //struct list_head * plist;
    unsigned long   dwLockFlag = 0;
    KQueueNode * pKQueueNode=NULL;
    pKQueueNode= kmalloc(sizeof(*pKQueueNode), GFP_KERNEL);
    pKQueueNode->pointer = pointer;

    #if USE_SPINLOCK
    spin_lock_irqsave(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    #if USE_MUTEX
        mutex_lock(&KQueue_mutex[module_ID]);
    #endif
    list_add_tail(&pKQueueNode->list,&KQueue_head_list[module_ID]);

    #if DEBUG
        pKQueueNode = (KQueueNode *)list_entry((KQueue_head_list[module_ID].next),KQueueNode,list);
        printk("[%s,%d]:%d\n",__func__,__LINE__,*pKQueueNode -> pointer );
    #endif

    #if USE_SPINLOCK
        spin_unlock_irqrestore(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    #if USE_MUTEX
        mutex_unlock(&KQueue_mutex[module_ID]);
    #endif

    wake_up(&wq[module_ID]);

}
EXPORT_SYMBOL(UADPKQueuePush);


int UADPKQueueWaitThreadNum(MS_U8 module_ID)
{
    unsigned long   dwLockFlag = 0;
    struct list_head * plist;
    int count = 0;

    #if USE_MUTEX
        mutex_lock(&KQueue_mutex[module_ID]);
    #endif

    #if USE_SPINLOCK
        spin_lock_irqsave(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    list_for_each(plist,&(wq[module_ID].task_list))
    {
        count++;
    }
    #if USE_SPINLOCK
        spin_unlock_irqrestore(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    #if USE_MUTEX
        mutex_unlock(&KQueue_mutex[module_ID]);
    #endif

    return count;

}

int UADPKQueueDataNum(u8 module_ID)
{
    unsigned long   dwLockFlag = 0;
    struct list_head * plist;
    int count = 0;

    #if USE_MUTEX
        mutex_lock(&KQueue_mutex[module_ID]);
    #endif

    #if USE_SPINLOCK
        spin_lock_irqsave(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    list_for_each(plist,&KQueue_head_list[module_ID])
    {
        count++;
    }
    #if USE_SPINLOCK
        spin_unlock_irqrestore(&KQueue_spinlock[module_ID], dwLockFlag);
    #endif

    #if USE_MUTEX
        mutex_unlock(&KQueue_mutex[module_ID]);
    #endif

    return count;
}
EXPORT_SYMBOL(UADPKQueueDataNum);

void UADPCreateKQueue(MS_U8 module_ID)
{
   ;
}
//EXPORT_SYMBOL(UADPCreateKQueue);

void UADPDeleteQueue(MS_U8 module_ID)
{
}
//EXPORT_SYMBOL(UADPDeleteQueue);

static void UADPDeleteKQueue(void)
{
    unsigned long dwLockFlag = 0;
    struct list_head * plist, *temp;
    int i;
    KQueueNode * pKQueueNode = NULL;

    for (i=0;i<MODULE_NUM;i++)
    {
        spin_lock_irqsave(&KQueue_spinlock[i], dwLockFlag);
        list_for_each_safe(plist,temp,&KQueue_head_list[i])
        {
            pKQueueNode=(KQueueNode *)list_entry(plist,KQueueNode, list);
            list_del(plist);
            kfree((const void*) pKQueueNode);
        }
        spin_unlock_irqrestore(&KQueue_spinlock[i], dwLockFlag);
    }
    printk("driver removed successfully\n");

}

static int test(void * p)
{
    //int j =4;
    int  *k=NULL;
    //i = &j;
    int count=0;
    //UADPKQueuePush(0,(char *)i);

    k = (int *)UADPKQueuePop(0);

    if ( k != NULL)
    {
        printk("%s:%d 0x%p\n",__func__,__LINE__,k);
        printk("%s:%d val:%d\n",__func__,__LINE__,*k);
    }

    printk("data count = %d\n",count);
    return 0;
}

static int UADPKQueueInit(void)
{
    int i,count;

    struct task_struct *thread1;

    k = &j;

    for ( i=0;i< MODULE_NUM; i++)
    {
        INIT_LIST_HEAD(&KQueue_head_list[i]);
        init_waitqueue_head(&wq[i]);
    #if USE_MUTEX
        mutex_init(&KQueue_mutex[i]);
    #endif

    #if USE_SPINLOCK
        KQueue_spinlock[i] = __SPIN_LOCK_UNLOCKED(KQueue_spinlock[i]);
    #endif

    }

    return 0;
}

int UADPKQueueIsempty(MS_U8 module_ID)
{
    unsigned long   dwLockFlag = 0;
    int result;
    //int count;

    spin_lock_irqsave(&KQueue_spinlock[module_ID], dwLockFlag);
    result=list_empty(&KQueue_head_list[module_ID]);
    //count = UADPKQueueDataNum(0);
    spin_unlock_irqrestore(&KQueue_spinlock[module_ID], dwLockFlag);

    printk("result=%d\n",result);
    //printk("thread count=%d\n",count);
    return result;
}
EXPORT_SYMBOL(UADPKQueueIsempty);

