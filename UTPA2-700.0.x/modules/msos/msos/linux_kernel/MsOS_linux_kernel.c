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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    MsOS.c
/// @brief  MStar OS Wrapper
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#if defined (MSOS_TYPE_LINUX_KERNEL)

#define _GNU_SOURCE

#if 0
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <memory.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#endif
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/slab.h>

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#if 0
#include "mem_eCos.h"
#include "MsOS.h"
#include "asmCPU.h"
//#include "devCHIP.h"
#endif
#include "halCHIP.h"
#include "halMMIO.h"
#if 0
#include "regCHIP.h"
#endif
#include "MsVersion.h"
#include "mem.h"

#define free kfree
#define malloc(size) kmalloc((size), GFP_KERNEL)

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
// Switch tasks every 1 ms.
#if 0
#define TASK_TIME_SLICE             (TICK_PER_ONE_MS)

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK

#define CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE (10)

/// Linux priority
typedef enum
{
    E_LINUX_PRI_SYS      = 99,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_LINUX_PRI_HIGHEST  = 80,    ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_LINUX_PRI_HIGH     = 60,    ///< High priority task     ( service task )
    E_LINUX_PRI_MEDIUM   = 40,   ///< Medium priority task   ( application task )
    E_LINUX_PRI_LOW      = 20,   ///< Low priority task      ( nonbusy application task )
    E_LINUX_PRI_LOWEST   = 1,   ///< Lowest priority task   ( idle application task )
} _LinuxPriority;
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))

#if 0
#define _TimeAbs(stTime, u32IntMs)                                                              \
{                                                                                               \
    clock_gettime(CLOCK_REALTIME, &(stTime));                                                   \
    if(u32IntMs>0)                                                                              \
    {                                                                                           \
        MS_U32                         u32Sec=u32Sec=(u32IntMs)/1000;                           \
        (stTime).tv_sec+=           u32Sec;                                                     \
        (stTime).tv_nsec+=          ((u32IntMs)- (1000* u32Sec))* 1000000;                      \
        if ((stTime).tv_nsec>= 1000000000)                                                      \
        {                                                                                       \
            (stTime).tv_sec++;                                                                  \
            (stTime).tv_nsec-=      1000000000;                                                 \
        }                                                                                       \
    }                                                                                           \
}
#endif

#if 0
#include "string.h"
#define PTH_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if (r != 0 && r != ETIMEDOUT) \
            // fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_RET_CHK(_pf_) _pf_
#endif

#define PTH_RET_CHK(_pf_) _pf_

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

#if 0
// Use 'S' as magic number //Needs to sync with header in Linux kernel!!!!
#define SYS_IOCTL_MAGIC             'S'
#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

static MS_S32 _s32FdSYS = -1;
#endif


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
//
// Variable-size Memory pool
//
#if defined (MSOS_TYPE_LINUX_KERNEL)

// #include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/types.h>
#include "MsTypes.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvIRQ.h"
#include "halIRQ.h"

#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK

#if (defined(CHIP_T3) || \
     defined(CHIP_T4) || \
     defined(CHIP_T7) || \
     defined(CHIP_T12) || \
     defined(CHIP_A2) || \
     defined(CHIP_T8) || \
     defined(CHIP_T9) || \
     defined(CHIP_U4) || \
     defined(CHIP_J2) || \
     defined(CHIP_T13) || \
     defined(CHIP_A1) || \
     defined(CHIP_A6) || \
     defined(CHIP_A7) || \
     defined(CHIP_A5) || \
     defined(CHIP_A3) || \
     defined(CHIP_AMETHYST) || \
     defined(CHIP_K1) || \
	 defined(CHIP_KAISER) || \
     defined(CHIP_EDISON) || \
     defined(CHIP_E8) || \
     defined(CHIP_EAGLE) || \
     defined(CHIP_EIFFEL) || \
     defined(CHIP_NIKE) || \
     defined(CHIP_NADAL) || \
     defined(CHIP_MADISON) || \
     defined(CHIP_CLIPPERS) || \
     defined(CHIP_MIAMI) || \
     defined(CHIP_MUNICH))
#define MIU1_CACHEABLE_START        0xC0000000
#define MIU1_CACHEABLE_END          0xD0000000
#define MIU1_UNCACHEABLE_START      0xD0000000
#define MIU1_UNCACHEABLE_END        0xE0000000
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif // #ifndef MIN

#ifndef MS_ASSERT
#define MS_ASSERT(_bool_)                                                                                   \
        {                                                                                                   \
            if (!(_bool_))                                                                                  \
            {                                                                                               \
                printk("ASSERT FAIL: %s, %s %s %d\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
                panic("mstar panic\n");                                                                     \
            }                                                                                               \
        }
#endif

#if 1

#ifdef MSOS_MEMPOOL_MAX
#undef MSOS_MEMPOOL_MAX
#define MSOS_MEMPOOL_MAX 2
#endif // #ifdef MSOS_MEMPOOL_MAX

typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stMemoryPool;
    cyg_mempool_var                 stMemoryPoolInfo;
    MS_BOOL bMPool;
} MsOS_MemoryPool_Info;

static MsOS_MemoryPool_Info     _MsOS_MemoryPool_Info[MSOS_MEMPOOL_MAX];
static                          DEFINE_SPINLOCK(_MsOS_MemoryPool_Mutex);
#define MEMORY_MUTEX_LOCK(flag)     spin_lock_irqsave(&_MsOS_MemoryPool_Mutex, flag)
#define MEMORY_MUTEX_UNLOCK(flag)   spin_unlock_irqrestore(&_MsOS_MemoryPool_Mutex, flag)

#endif
//
// Task Management
//
typedef struct
{
    MS_BOOL             bUsed;
    struct task_struct* pstThreadInfo;
} MsOS_Task_Info;

static MsOS_Task_Info   _MsOS_Task_Info[MSOS_TASK_MAX];
static                  DEFINE_SPINLOCK(_MsOS_Task_Mutex);

//
// Mutex
//
typedef struct
{
    MS_BOOL             bUsed;
    spinlock_t          stMutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;

static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static                          DEFINE_SPINLOCK(_MsOS_Mutex_Mutex);
#define MUTEX_MUTEX_LOCK(flag)      spin_lock_irqsave(&_MsOS_Mutex_Mutex, flag)
#define MUTEX_MUTEX_UNLOCK(flag)    spin_unlock_irqrestore(&_MsOS_Mutex_Mutex, flag)

#if 0
//
// Semaphore
//
typedef struct
{
    MS_BOOL                     bUsed;
    sem_t                       stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info      _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];
static pthread_mutex_t          _MsOS_Semaphore_Mutex;
#endif

//
// Event Group
//
typedef struct
{
    MS_BOOL                     bUsed;
    MS_U32                      u32EventGroup;
    spinlock_t                  stMutexEvent;
    wait_queue_head_t           stSemaphore;
    // pthread_cond_t              stSemaphore; // ?????????????
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info     _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static                          DEFINE_SPINLOCK(_MsOS_EventGroup_Mutex);
#define EVENT_MUTEX_LOCK(flag)      spin_lock_irqsave(&_MsOS_EventGroup_Mutex, flag)
#define EVENT_MUTEX_UNLOCK(flag)    spin_unlock_irqrestore(&_MsOS_EventGroup_Mutex, flag)


#if 0
//
// Queue
//
typedef struct
{
    MS_BOOL            bUsed;
    MS_U8*             pu8Head;
    MS_U8*             pu8Tail;
    MS_U8*             pu8Write;
    MS_U8*             pu8Read;
    pthread_cond_t  SendSem;
    pthread_mutex_t SendMutex;
    pthread_cond_t  RecvSem;
    pthread_mutex_t RecvMutex;
    MessageType     eMsgType;
    MS_U32             u32AlignedMsgSize;
} MsOS_Queue_Info;

static MsOS_Queue_Info  _MsOS_Queue_Info[MSOS_QUEUE_MAX];
static pthread_mutex_t  _MsOS_Queue_Mutex;
#endif

// @FIXME: Leave Timer later
//
// Timer
//
typedef struct
{
    MS_BOOL             bUsed;
    TimerCb             pTimerCb;
    struct timer_list   timer;
    int                 period;
    int                 first;
} MsOS_Timer_Info;
static MsOS_Timer_Info  _MsOS_Timer_Info[MSOS_TIMER_MAX];
static                          DEFINE_SPINLOCK(_MsOS_Timer_Mutex);
#define TIMER_MUTEX_LOCK()      spin_lock(&_MsOS_Timer_Mutex)
#define TIMER_MUTEX_UNLOCK()    spin_unlock(&_MsOS_Timer_Mutex)
#endif


// static pthread_mutex_t  _ISR_Mutex;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

static MSIF_Version _drv_msos_version = {
    .DDI = { MSOS_DRV_VERSION },
};
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MSOS_GetLibVer
/// @brief \b Function  \b Description: Show the MSOS driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b Result
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MSOS_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_msos_version;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize MsOS
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Init (void)
{
    MS_U32 u32I;
/*
    pthread_mutexattr_t _MsOS_Mutex_Attr;

    PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
    //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
#ifdef MS_DEBUG
    PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
#endif
*/

    // Empty all the MsOS structures
    for( u32I=0; u32I<MSOS_MEMPOOL_MAX; u32I++)
    {
        _MsOS_MemoryPool_Info[u32I].bUsed = FALSE;
    }

    //
    // Task Management
    //
    spin_lock_init(&_MsOS_Task_Mutex);
    for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
    {
        _MsOS_Task_Info[u32I].bUsed = FALSE;
    }

    //
    // Mutex
    //
    // PTH_RET_CHK(pthread_mutex_init(&_MsOS_Mutex_Mutex, &_MsOS_Mutex_Attr));
    spin_lock_init(&_MsOS_Mutex_Mutex);
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }

    //
    // Semaphore
    //
/*
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Semaphore_Mutex, &_MsOS_Mutex_Attr));
    for( u32I=0; u32I<MSOS_SEMAPHORE_MAX; u32I++)
    {
        _MsOS_Semaphore_Info[u32I].bUsed = FALSE;
    }
*/

    //
    // Event Group
    //
    // PTH_RET_CHK(pthread_mutex_init(&_MsOS_EventGroup_Mutex, &_MsOS_Mutex_Attr));
    spin_lock_init(&_MsOS_EventGroup_Mutex);
    for( u32I=0; u32I<MSOS_EVENTGROUP_MAX; u32I++)
    {
        _MsOS_EventGroup_Info[u32I].bUsed = FALSE;
    }

    //
    // Queue
    //
/*
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Mutex, &_MsOS_Mutex_Attr));
    for( u32I=0; u32I<MSOS_QUEUE_MAX; u32I++)
    {
        memset(&_MsOS_Queue_Info[u32I].bUsed, 0, sizeof(MsOS_Queue_Info));
    }
*/

    //
    // Timer
    //
    spin_lock_init(&_MsOS_Timer_Mutex);
    for( u32I=0; u32I<MSOS_TIMER_MAX; u32I++)
    {
        _MsOS_Timer_Info[u32I].bUsed = FALSE;
        _MsOS_Timer_Info[u32I].pTimerCb = NULL;
        _MsOS_Timer_Info[u32I].period = 0;
        _MsOS_Timer_Info[u32I].first = 0;
        init_timer(&(_MsOS_Timer_Info[u32I].timer));
    }

    //
    // Interrupt
    //
/*
    printk("pthread_mutex_init\n");
    PTH_RET_CHK(pthread_mutex_init(&_ISR_Mutex, &_MsOS_Mutex_Attr));
*/
    printk("CHIP_InitISR\n");
    CHIP_InitISR();

/*
    if (_s32FdSYS < 0)   //First time open
    {
        _s32FdSYS  = open("/dev/system", O_RDWR);

        if (_s32FdSYS <= 0) //open device success
        {
            printk("[MDrv_SYS_Init]open device node failed!\n");
            return FALSE;
        }
    }
*/
    return TRUE;
}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MsOS_Init);
#endif


#if 1
//
// Memory management
//
//-------------------------------------------------------------------------------------------------
/// Create a variable-size memory pool dynamically
/// @param  u32PoolSize         \b IN: pool size in bytes
/// @param  u32MinAllocation    \b IN: not used
/// @param  pPoolAddr           \b IN: starting address for the memory pool
/// @param  eAttribute          \b IN: only E_MSOS_FIFO - suspended in FIFO order
/// @param  pPoolName           \b IN: not used
/// @return >=0 : assigned memory pool ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMemoryPool (MS_U32 u32PoolSize,
                           MS_U32 u32MinAllocation,
                           void * pPoolAddr,
                           MsOSAttribute eAttribute,
                           char *pPoolName)
{
    MS_S32 s32Id;
    unsigned flag;

    MEMORY_MUTEX_LOCK(flag);
    for(s32Id=0;s32Id<MSOS_MEMPOOL_MAX;s32Id++)
    {
        if(_MsOS_MemoryPool_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_MEMPOOL_MAX)
    {
        _MsOS_MemoryPool_Info[s32Id].bUsed = TRUE;
    }
    MEMORY_MUTEX_UNLOCK(flag);

    if(s32Id >= MSOS_MEMPOOL_MAX)
    {
        return -1;
    }

    if (pPoolAddr)
    {
        _MsOS_MemoryPool_Info[s32Id].bMPool= TRUE;
        cyg_mempool_var_create( pPoolAddr,
                                u32PoolSize,
                                &_MsOS_MemoryPool_Info[s32Id].stMemoryPool,
                                &_MsOS_MemoryPool_Info[s32Id].stMemoryPoolInfo );
    }
    else
    {
        _MsOS_MemoryPool_Info[s32Id].bMPool= FALSE;
    }
    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete a variable-size memory pool
/// @param  s32PoolId   \b IN: pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteMemoryPool (MS_S32 s32PoolId)
{
    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    if (_MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        cyg_mempool_var_delete(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
    }

    MEMORY_MUTEX_LOCK(flag);
    _MsOS_MemoryPool_Info[s32PoolId].bUsed = FALSE;
    MEMORY_MUTEX_UNLOCK(flag);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get the information of a variable-size memory pool
/// @param  s32PoolId   \b IN: memory pool ID
/// @param  pPoolAddr   \b OUT: holding the starting address for the memory pool
/// @param  pu32PoolSize \b OUT: holding the total size of the memory pool
/// @param  pu32FreeSize \b OUT: holding the available free size of the memory pool
/// @param  pu32LargestFreeBlockSize  \b OUT: holding the size of the largest free block
/// @return TRUE : succeed
/// @return FALSE : the pool has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoMemoryPool (MS_S32 s32PoolId,
                          void **pPoolAddr,
                          MS_U32 *pu32PoolSize,
                          MS_U32 *pu32FreeSize,
                          MS_U32 *pu32LargestFreeBlockSize)
{

    return FALSE;

}

//-------------------------------------------------------------------------------------------------
/// Allocate a memory block with 16-Byte aligned starting address from the variable-size memory pool
/// @param  u32Size     \b IN: request size
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory
/// @return Otherwise : pointer to the allocated memory block
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateMemory (MS_U32 u32Size, MS_S32 s32PoolId)
{
    void  *pAddr;

    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    // Jerry.Tsao: cyg_mempool_var_try_alloc allocates the maximum pool it has when size is zero.
    if (u32Size == 0)
    {
        return NULL;
    }

    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        return malloc(u32Size);
    }

    pAddr = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);

    //Current eCosPro kernel always allocates 16-B aligned block
    if ( (MS_U32)pAddr & 0xF)
    {
        return NULL;
    }

    return ( pAddr );
}

//-------------------------------------------------------------------------------------------------
/// Free a memory block from the variable-size memory pool
/// @param  pAddress    \b IN: pointer to previously allocated memory block
/// @param  s32PoolId   \b IN: memory pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_FreeMemory (void *pAddress, MS_S32 s32PoolId)
{
    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    if (pAddress == NULL)
    {
        return FALSE;
    }

    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        free(pAddress);
    }
    else
    {
        cyg_mempool_var_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pAddress);
    }
    return TRUE;
}

#if 0
//-------------------------------------------------------------------------------------------------
/// Reallocate a block of memory with 4-byte aligned start address from the variable-size memory pool
/// @param  pOrgAddress \b IN: points to the beginning of the original memory block
/// @param  u32NewSize  \b IN: size of new memory block
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory to expand the block or u32NewSize == 0 && pOrgAddress != NULL
/// @return Otherwise : pointer to the reallocated (and possibly moved) memory block
//  @note   reference realloc in malloc.cxx
//-------------------------------------------------------------------------------------------------
void * MsOS_ReallocateMemory (void *pOrgAddress, MS_U32 u32NewSize, MS_S32 s32PoolId)
{
    void *pNewAddress = NULL;
    MS_U32 u32OrgSize;

    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        return realloc(pOrgAddress, u32NewSize);
    }

    if ( pOrgAddress == NULL)
    {
        //malloc
        pNewAddress = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32NewSize);
        return pNewAddress;
    }

    if ( u32NewSize == 0)
    {
        //free
        cyg_mempool_var_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pOrgAddress);
        return NULL;
    }

    //mvarimpl.inl
    struct memdq {
        struct memdq *prev, *next;
        MS_S32 size;
    };

    struct memdq *dq = (struct memdq *) ((MS_U32 )pOrgAddress  - sizeof(struct memdq));
    u32OrgSize = dq->size - sizeof(struct memdq);   //dq->size was rounded up to 16B-aligned when malloc, so u32OrgSize may be larger than requested

    //No resize function is implemented, so malloc a new block directly
    pNewAddress = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32NewSize);

    if ( pNewAddress != NULL) //move to a new block
    {
        if ( u32NewSize < u32OrgSize)
        {
            memcpy( pNewAddress, pOrgAddress, u32NewSize );
        }
        else
        {
            memcpy( pNewAddress, pOrgAddress, u32OrgSize );
        }
        cyg_mempool_var_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pOrgAddress);

    }
    else //not enough available memory to expand the block to the given size
    {
        //the original block is unchanged
    }

    return pNewAddress;
}
#endif
#endif

//
// Task
//
//-------------------------------------------------------------------------------------------------
/// Create a task
/// @param  pTaskEntry       \b IN: task entry point
/// @param  u32TaskEntryData \b IN: task entry data: a pointer to some static data, or a
///          small integer, or NULL if the task does not require any additional data.
/// @param  eTaskPriority    \b IN: task priority
/// @param  bAutoStart       \b IN: start immediately or later
/// @param  pStackBase       \b IN: task stack
/// @param  u32StackSize     \b IN: stack size
/// @param  pTaskName        \b IN: task name
/// @return >=0 : assigned Task ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateTask (TaskEntry pTaskEntry,
                     MS_U32 u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL bAutoStart,
                     void * pStackBase,
                     MS_U32 u32StackSize,
                     char *pTaskName)
{
#if 0
    MS_S32 s32Id;
    pthread_attr_t thread_attr;
    struct sched_param thrsched;

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Task_Mutex));
    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if( s32Id < MSOS_TASK_MAX)
    {
        _MsOS_Task_Info[s32Id].bUsed = TRUE;
    }
    //printk("pthread_mutex_unlock\n");
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Task_Mutex));

    if( s32Id >= MSOS_TASK_MAX)
    {
        return -1;
    }
    //printk("pthread_attr_init\n");
    if (PTH_RET_CHK(pthread_attr_init(&thread_attr)))
    {
        return -1;
    }

    /* - MaxCC20080205
    if (pthread_attr_setstack(&thread_attr, (void*)(((MS_U32)pStackBase)+ u32StackSize), u32StackSize))
    {
        return -1;
    }
    // . MaxCC20080205 */
    //set thread priority
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_getschedparam(&thread_attr, &thrsched);
    switch(eTaskPriority)
    {
        case E_TASK_PRI_SYS:
                thrsched.sched_priority = E_LINUX_PRI_SYS;
                break;
        case E_TASK_PRI_HIGHEST:
                thrsched.sched_priority = E_LINUX_PRI_HIGHEST;
                break;
        case E_TASK_PRI_HIGH:
                thrsched.sched_priority = E_LINUX_PRI_HIGH;
                break;
        case E_TASK_PRI_MEDIUM:
                thrsched.sched_priority = E_LINUX_PRI_MEDIUM;
                break;
        case E_TASK_PRI_LOW:
                thrsched.sched_priority = E_LINUX_PRI_LOW;
                break;
        case E_TASK_PRI_LOWEST:
                thrsched.sched_priority = E_LINUX_PRI_LOWEST;
                break;
    }
    pthread_attr_setschedparam(&thread_attr, &thrsched);
    //pthread_attr_getschedparam(&thread_attr, &thrsched);printk("%d\n",thrsched.sched_priority);
    //printk("max=%d,min=%d\n",sched_get_priority_max(SCHED_RR),sched_get_priority_min(SCHED_RR));
#else
    // @FIXME:
    //     (1) eTaskPriority: Task priority is ignored here
    //     (2) pTaskName: is ignored here
    //     (3) Need mutex to protect critical section

    MS_S32 s32Id;

    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if( s32Id >= MSOS_TASK_MAX)
    {
        return -1;
    }
    _MsOS_Task_Info[s32Id].bUsed = TRUE;
    _MsOS_Task_Info[s32Id].pstThreadInfo = kthread_create(((int)(void*)pTaskEntry), (void*)u32TaskEntryData, pTaskName);

    if (bAutoStart)
    {
        wake_up_process(_MsOS_Task_Info[s32Id].pstThreadInfo);
    }
    s32Id |= MSOS_ID_PREFIX;
#endif
    return s32Id;
}


//-------------------------------------------------------------------------------------------------
/// Delete a previously created task
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId)
{
#if 1
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    kthread_stop(_MsOS_Task_Info[s32TaskId].pstThreadInfo);
    _MsOS_Task_Info[s32TaskId].bUsed = FALSE;
#else
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
#endif
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void)
{
    yield();
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
    msleep_interruptible((unsigned int)u32Ms);
}

//-------------------------------------------------------------------------------------------------
/// Delay for u32Us microseconds
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs (MS_U32 u32Us)
{
    struct timespec TS1, TS2;
    getnstimeofday(&TS1);
    getnstimeofday(&TS2);

    while((TS2.tv_nsec - TS1.tv_nsec)< (u32Us * 1000UL))
    {
        getnstimeofday(&TS2);
    }
#if 0
    struct timespec req, rem;

    req.tv_sec = 0;
    req.tv_nsec = (long) (u32Us*1000UL);

    while(1)
    {
        int err;

        err = nanosleep(&req, &rem);
        if(err==-1)
        {
            #if 0
            switch(errno)
            {
                case EINTR:
                    req.tv_sec = rem.tv_sec;
                    req.tv_nsec = rem.tv_nsec;
                    continue;
                default:
                    printk("nanosleep is interrupted: %d\n", errno);
            }
            #endif
        }

        break;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Resume the specified suspended task
/// @param  s32TaskId   \b IN: Task ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   This API is not supported in Linux
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ResumeTask (MS_S32 s32TaskId)
{
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }
    wake_up_process(_MsOS_Task_Info[s32TaskId].pstThreadInfo);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Suspend the specified task
/// @param  s32TaskId   \b IN: Task ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   This API is not supported in Linux
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SuspendTask (MS_S32 s32TaskId)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Get a task information
// @param  s32TaskId       \b IN: task ID
// @param  peTaskPriority  \b OUT: ptr to task priority
// @param  pu32StackSize   \b OUT: ptr to stack size
// @param  pTaskName       \b OUT: ptr to task name
// @return TRUE : succeed
// @return FALSE : the task has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoTask (MS_S32 s32TaskId, TaskPriority *peTaskPriority, MS_U32 *pu32StackSize, char *pTaskName)
{
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    //ToDO
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void)
{
    MS_S32 s32Id;
    struct task_struct *self = current;

    for (s32Id=0; s32Id < MSOS_TASK_MAX; s32Id++)
    {
        if (self == _MsOS_Task_Info[s32Id].pstThreadInfo)
        {
            break;
        }
    }

    if (s32Id < MSOS_TASK_MAX)
    {
        s32Id |= MSOS_ID_PREFIX;
        return s32Id;
    }
    else
    {
        return -1;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get thread ID of current thread/process in Linux(Kernel)
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    return MsOS_InfoTaskID();
}

//
// Mutex
//
//-------------------------------------------------------------------------------------------------
/// Create a mutex in the unlocked state
/// @param  eAttribute  \b IN: E_MSOS_FIFO: suspended in FIFO order
/// @param  pMutexName  \b IN: mutex name
/// @param  u32Flag  \b IN: process data shared flag
/// @return >=0 : assigned mutex Id
/// @return <0 : fail
/// @note   A mutex has the concept of an owner, whereas a semaphore does not.
///         A mutex provides priority inheritance protocol against proiorty inversion, whereas a binary semaphore does not.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName1, MS_U32 u32Flag)
{
    MS_S32 s32Id, s32LstUnused = MSOS_MUTEX_MAX;
    MS_U8 pMutexName[MAX_MUTEX_NAME_LENGTH];
    MS_U32 u32MaxLen;
    unsigned long flag;

    if (NULL == pMutexName1)
    {
        return -1;
    }
    if (strlen(pMutexName1) >= (MAX_MUTEX_NAME_LENGTH-1))
    {
        printk("%s: Warning strlen(%s) is longer than MAX_MUTEX_NAME_LENGTH(%d). Oversize char will be discard.\n",
        __FUNCTION__,pMutexName1,MAX_MUTEX_NAME_LENGTH);
    }
    if (0 == (u32MaxLen = MIN(strlen(pMutexName1), (MAX_MUTEX_NAME_LENGTH-1))))
    {
        return -1;
    }
    strncpy((char*)pMutexName, (const char*)pMutexName1, u32MaxLen);
    pMutexName[u32MaxLen] = '\0';

    MUTEX_MUTEX_LOCK(flag);
    for(s32Id=0;s32Id<MSOS_MUTEX_MAX;s32Id++)
    {
        // if (PTHREAD_PROCESS_SHARED == s32Prop) // @FIXME: Richard: is the mutex name always used as an id, regardless of process shared/private property?
        {
            if(TRUE == _MsOS_Mutex_Info[s32Id].bUsed)
            {
                if (0== strcmp((const char*)_MsOS_Mutex_Info[s32Id].u8Name, (const char*)pMutexName))
                {
                    break;
                }
            }
        }
        if (FALSE==_MsOS_Mutex_Info[s32Id].bUsed  && MSOS_MUTEX_MAX==s32LstUnused)
        {
            s32LstUnused = s32Id;
        }
    }
    if ((MSOS_MUTEX_MAX==s32Id) && (MSOS_MUTEX_MAX>s32LstUnused))
    {
        _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
        strcpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName);
        spin_lock_init(&_MsOS_Mutex_Info[s32LstUnused].stMutex);
        s32Id = s32LstUnused;
    }
    MUTEX_MUTEX_UNLOCK(flag);

    if(MSOS_MUTEX_MAX <= s32Id)
    {
        return -1;
    }

    s32Id |= MSOS_ID_PREFIX;

    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the specified mutex
/// @param  s32MutexId  \b IN: mutex ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It is important that the mutex be in the unlocked state when it is
///            destroyed, or else the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteMutex (MS_S32 s32MutexId)
{
    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }
    MUTEX_MUTEX_LOCK();

    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);
    _MsOS_Mutex_Info[s32MutexId].bUsed = FALSE;
    _MsOS_Mutex_Info[s32MutexId].u8Name[0] = '\0';

    MUTEX_MUTEX_UNLOCK();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to lock a mutex
/// @param  s32MutexId  \b IN: mutex ID
/// @param  u32WaitMs   \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the mutex is locked
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// @FIXME: don't support time-out at this stage
MS_BOOL MsOS_ObtainMutex (MS_S32 s32MutexId, MS_U32 u32WaitMs)
{
    MS_BOOL bRet = FALSE;

    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        spin_lock(&(_MsOS_Mutex_Info[s32MutexId].stMutex));
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if (!spin_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex))
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
        //printk("[%s][%d] %s doesn't support mutex timeout at this stage\n", __FUNCTION__, __LINE__, __FUNCTION__);
        spin_lock(&(_MsOS_Mutex_Info[s32MutexId].stMutex));
        //printf("--------------------------------------\n");
        bRet = TRUE;
    }
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a mutex
/// @param  s32MutexId  \b IN: mutex ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Only the owner thread of the mutex can unlock it.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseMutex (MS_S32 s32MutexId)
{
    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }
    spin_unlock(&(_MsOS_Mutex_Info[s32MutexId].stMutex));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Get a mutex informaton
// @param  s32MutexId  \b IN: mutex ID
// @param  peAttribute \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
// @param  pMutexName  \b OUT: ptr to mutex name
// @return TRUE : succeed
// @return FALSE : the mutex has not been created.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoMutex (MS_S32 s32MutexId, MsOSAttribute *peAttribute, char *pMutexName)
{
    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if(_MsOS_Mutex_Info[s32MutexId].bUsed == TRUE)
    {
        //ToDo: extend _MsOS_Mutex_Info structure ?
        *peAttribute = E_MSOS_FIFO; //only FIFO for eCos
        // @FIXME: linux porting
        // UTL_strcpy(pMutexName, "ABC");
        strcpy(pMutexName, (const char*)_MsOS_Mutex_Info[s32MutexId].u8Name);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if 0
//
// Semaphore
//
//-------------------------------------------------------------------------------------------------
/// Create a semaphore
/// @param  u32InitCnt \b IN: initial semaphore value
/// @param  eAttribute \b IN: E_MSOS_FIFO suspended in FIFO order
/// @param  pSemaphoreName \b IN: semaphore name
/// @return >=0 : assigned Semaphore Id
/// @return <0 : fail
/// @note   A semaphore does not have the concept of an owner; it is possible for one thread to lock a
///           binary semaphore and another thread to unlock it.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateSemaphore (MS_U32 u32InitCnt,
                          MsOSAttribute eAttribute,
                          char *pSemaphoreName)
{
    MS_S32 s32Id;

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Semaphore_Mutex));
    for(s32Id=0;s32Id<MSOS_SEMAPHORE_MAX;s32Id++)
    {
        if(_MsOS_Semaphore_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_SEMAPHORE_MAX)
    {
        _MsOS_Semaphore_Info[s32Id].bUsed = TRUE;
    }
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Semaphore_Mutex));

    if(s32Id >= MSOS_SEMAPHORE_MAX)
    {
        return -1;
    }

    sem_init(&_MsOS_Semaphore_Info[s32Id].stSemaphore, 0, u32InitCnt);
    s32Id |= MSOS_ID_PREFIX;

    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the specified semaphore
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It is important that there are not any threads waiting on the semaphore
///             when this function is called or the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteSemaphore (MS_S32 s32SemaphoreId)
{
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    sem_destroy(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Semaphore_Mutex));
    _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Semaphore_Mutex));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to decrement a semaphore count
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @param  u32WaitMs       \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the semaphore count = 0
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ObtainSemaphore (MS_S32 s32SemaphoreId, MS_U32 u32WaitMs)
{
    MS_BOOL bRet = FALSE;

    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        bRet = sem_wait(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore)  >=0 ? TRUE : FALSE;
    }
    else //blocking wait with timeout
    {
        struct timespec         StopTime;
//        if (0== u32WaitMs)
//        {
//            u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//        }
        _TimeAbs(StopTime, u32WaitMs);
        bRet = sem_timedwait(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, &StopTime) >=0 ? TRUE : FALSE;
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Increase a semaphore count
/// @param  s32SemaphoreId  \b IN: semaphore ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   It's possible for any thread to increase the semaphore count
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ReleaseSemaphore (MS_S32 s32SemaphoreId)
{
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    return sem_post(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore) >= 0 ? TRUE : FALSE;
}

//-------------------------------------------------------------------------------------------------
// Get a semaphore informaton
// @param  s32SemaphoreId  \b IN: semaphore ID
// @param  pu32InitCnt     \b OUT: ptr to initial semaphore value
// @param  peAttribute     \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
// @param  pSemaphoreName  \b OUT: ptr to semaphore name
// @return TRUE : succeed
// @return FALSE : the semaphore has not been created.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoSemaphore (MS_S32 s32SemaphoreId, MS_U32 *pu32InitCnt, MsOSAttribute *peAttribute, char *pSemaphoreName)
{
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    return sem_getvalue(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore,
        (int *)pu32InitCnt) >= 0 ? TRUE : FALSE;
}
#endif

//
// Event management
//
//-------------------------------------------------------------------------------------------------
/// Create an event group
/// @param  pEventName  \b IN: event group name
/// @return >=0 : assigned Event Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateEventGroup (char *pEventName)
{
    MS_S32 s32Id;
    unsigned long flag;

    EVENT_MUTEX_LOCK(flag);
    for(s32Id=0; s32Id<MSOS_EVENTGROUP_MAX; s32Id++)
    {
        if(_MsOS_EventGroup_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_EVENTGROUP_MAX)
    {
        _MsOS_EventGroup_Info[s32Id].bUsed = TRUE;
        _MsOS_EventGroup_Info[s32Id].u32EventGroup= 0;
    }
    EVENT_MUTEX_UNLOCK(flag);

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }
    spin_lock_init(&_MsOS_EventGroup_Info[s32Id].stMutexEvent);
    init_waitqueue_head(&_MsOS_EventGroup_Info[s32Id].stSemaphore);
    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the event group
/// @param  s32EventGroupId \b IN: event group ID
/// @return TRUE : succeed
/// @return FALSE : fail, sb is waiting for the event flag
/// @note event group that are being waited on must not be deleted
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteEventGroup (MS_S32 s32EventGroupId)
{
    unsigned long flag;
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    init_waitqueue_head(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
    // PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stMutex));
    EVENT_MUTEX_LOCK(flag);
    _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup= 0;
    _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
    EVENT_MUTEX_UNLOCK(flag);
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set the event flag (bitwise OR w/ current value) in the specified event group
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SetEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    unsigned long flag;
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    wake_up(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Clear the specified event flag (bitwise XOR operation) in the specified event group
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ClearEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    unsigned long flag;
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Wait for the specified event flag combination from the event group
/// @param  s32EventGroupId     \b IN: event group ID
/// @param  u32WaitEventFlag    \b IN: wait event flag value
/// @param  pu32RetrievedEventFlag \b OUT: retrieved event flag value
/// @param  eWaitMode           \b IN: E_AND/E_OR/E_AND_CLEAR/E_OR_CLEAR
/// @param  u32WaitMs           \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the event is not ready
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_WaitEvent (MS_S32 s32EventGroupId,
                     MS_U32 u32WaitEventFlag,
                     MS_U32 *pu32RetrievedEventFlag,
                     EventWaitMode eWaitMode,
                     MS_U32 u32WaitMs)
{
    MS_BOOL bRet= FALSE;
    MS_BOOL bAnd;
    MS_BOOL bClear;
    unsigned long flag;

    *pu32RetrievedEventFlag = 0;

    if (!u32WaitEventFlag)
    {
        return FALSE;
    }

    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    bClear= ((E_AND_CLEAR== eWaitMode) || (E_OR_CLEAR== eWaitMode))? TRUE: FALSE;
    bAnd= ((E_AND== eWaitMode)|| (E_AND_CLEAR== eWaitMode))? TRUE: FALSE;

    do{
/*
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
        if ((bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag))
        {
            break;
        }
*/
        if (u32WaitMs== MSOS_WAIT_FOREVER) //blocking wait
        {
            if (bAnd)
            {
                wait_event(_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                           ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag));
            }
            else
            {
                wait_event(_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                           ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0));
            }
        }
        else
        {
            u32WaitMs = msecs_to_jiffies(u32WaitMs);
            if (bAnd)
            {
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                   ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag),
                                   u32WaitMs);
            }
            else
            {
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                   ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0),
                                   u32WaitMs);
            }
        }
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
    } while (0);

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    bRet= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (bRet && bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    return bRet;
}

wait_queue_head_t* MsOS_GetEventQueue (MS_S32 s32EventGroupId)
{
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }
    return (&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
}

/// @FIXME: Timer API are empty
//
// Timer management
//
static void _MsOS_TimerNotify(unsigned long data)
{
    MS_S32 s32Id = (MS_S32) data;

    if (_MsOS_Timer_Info[s32Id].pTimerCb)
    {
        _MsOS_Timer_Info[s32Id].pTimerCb(0, s32Id| MSOS_ID_PREFIX);
    }
    _MsOS_Timer_Info[s32Id].timer.expires = jiffies + HZ*_MsOS_Timer_Info[s32Id].period/1000;
    add_timer(&(_MsOS_Timer_Info[s32Id].timer));
}


//-------------------------------------------------------------------------------------------------
/// Create a Timer
/// @param  pTimerCb        \b IN: timer callback function
/// @param  u32FirstTimeMs  \b IN: first ms for timer expiration
/// @param  u32PeriodTimeMs \b IN: periodic ms for timer expiration after first expiration
///                                0: one shot timer
/// @param  bStartTimer     \b IN: TRUE: activates the timer after it is created
///                                FALSE: leaves the timer disabled after it is created
/// @param  pTimerName      \b IN: Timer name (not used by eCos)
/// @return >=0 : assigned Timer ID
///         <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateTimer (TimerCb pTimerCb,
                      MS_U32 u32FirstTimeMs,
                      MS_U32 u32PeriodTimeMs,
                      MS_BOOL bStartTimer,
                      char *pTimerName)
{
    MS_S32 s32Id;

    TIMER_MUTEX_LOCK();
    for(s32Id=0;s32Id<MSOS_TIMER_MAX;s32Id++)
    {
        if(_MsOS_Timer_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_TIMER_MAX)
    {
        _MsOS_Timer_Info[s32Id].bUsed = TRUE;
    }
    TIMER_MUTEX_UNLOCK();

    if(s32Id >= MSOS_TIMER_MAX)
    {
        return -1;
    }

    _MsOS_Timer_Info[s32Id].pTimerCb=   pTimerCb;
    _MsOS_Timer_Info[s32Id].first = u32FirstTimeMs;
    _MsOS_Timer_Info[s32Id].period = u32PeriodTimeMs;
    _MsOS_Timer_Info[s32Id].timer.data = (unsigned long)s32Id;
    _MsOS_Timer_Info[s32Id].timer.expires = jiffies + HZ*u32FirstTimeMs/1000;
    _MsOS_Timer_Info[s32Id].timer.function = _MsOS_TimerNotify;
    if (bStartTimer)
    {
        add_timer(&(_MsOS_Timer_Info[s32Id].timer));
    }
    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTimer (MS_S32 s32TimerId)
{
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        TIMER_MUTEX_LOCK();
        del_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;

        _MsOS_Timer_Info[s32TimerId].pTimerCb = NULL;
        _MsOS_Timer_Info[s32TimerId].period = 0;
        _MsOS_Timer_Info[s32TimerId].first = 0;
        TIMER_MUTEX_UNLOCK();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Start the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_StartTimer (MS_S32 s32TimerId)
{
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        _MsOS_Timer_Info[s32TimerId].timer.expires = jiffies + _MsOS_Timer_Info[s32TimerId].period/1000;
        add_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Stop the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   MsOS_StopTimer then MsOS_StartTimer => The timer will trigger at the same relative
///             intervals that it would have if it had not been disabled.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_StopTimer (MS_S32 s32TimerId)
{
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        del_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Reset a Timer & reset the expiration periods
/// @param  s32TimerId      \b IN: Timer ID
/// @param  u32FirstTimeMs  \b IN: first ms for timer expiration
/// @param  u32PeriodTimeMs \b IN: periodic ms for timer expiration after first expiration
///                                0: one shot timer
/// @param  bStartTimer     \b IN: TRUE: activates the timer after it is created
///                                FALSE: leaves the timer disabled after it is created
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ResetTimer (MS_S32 s32TimerId,
                      MS_U32 u32FirstTimeMs,
                      MS_U32 u32PeriodTimeMs,
                      MS_BOOL bStartTimer)
{
#if 0
    struct itimerspec StopTimer;
    struct itimerspec dummy;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    _MsOS_Timer_Info[s32TimerId].TimeInfo.it_value.tv_sec=           u32FirstTimeMs/1000;
    _MsOS_Timer_Info[s32TimerId].TimeInfo.it_value.tv_nsec=          (u32FirstTimeMs- (_MsOS_Timer_Info[s32TimerId].TimeInfo.it_value.tv_sec)*1000)* 1000000;
    _MsOS_Timer_Info[s32TimerId].TimeInfo.it_interval.tv_sec=        u32PeriodTimeMs/1000;
    _MsOS_Timer_Info[s32TimerId].TimeInfo.it_interval.tv_sec=        (u32PeriodTimeMs- (_MsOS_Timer_Info[s32TimerId].TimeInfo.it_interval.tv_sec)*1000)* 1000000;


    if(bStartTimer)
    {
        if (0> timer_settime(_MsOS_Timer_Info[s32TimerId].TimerId, 0, &_MsOS_Timer_Info[s32TimerId].TimeInfo, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }
    else
    {
        memset(&StopTimer, 0, sizeof(StopTimer));
        if (0> timer_settime(_MsOS_Timer_Info[s32TimerId].TimerId, 0, &StopTimer, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }

    return TRUE;
#else
    return FALSE;
#endif
}

//
// System time
//
//-------------------------------------------------------------------------------------------------
/// Get current system time in ms
/// @return system time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTime (void)
{
    struct timespec         ts;

    getnstimeofday(&ts);
    return ts.tv_sec* 1000+ ts.tv_nsec/1000000;
}

//-------------------------------------------------------------------------------------------------
///[OBSOLETE]
/// Time difference between current time and task time
/// @return system time diff in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer) //unit = ms
{
    return (MsOS_GetSystemTime() - u32TaskTimer);
}

//-------------------------------------------------------------------------------------------------
///[OBSOLETE]
/// Time difference between setting time and task time
/// @return system time diff in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_Timer_DiffTime(MS_U32 u32Timer, MS_U32 u32TaskTimer) //unit = ms
{
    // simple minus is enough, because max(timer_counter) == max(u32)
    return (u32Timer - u32TaskTimer);
}

/*
//COUNT was reset at system initialization
//-------------------------------------------------------------------------------------------------
/// Set current system time
/// @param  u32SystemTime  \b IN: system time in ms
/// @return TRUE - succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SetSystemTime (MS_U32 u32SystemTime)
{
    MS_U32 u32COUNT = u32SystemTime*TICK_PER_ONE_MS*CYGNUM_HAL_RTC_PERIOD;

    asm volatile (
        "move $8,%0;"
        "mtc0 $8,$9;"
        "nop; nop; nop;"
        :
        : "r"(u32COUNT)
        : "$8"
        );

    return TRUE;
}
*/

#if 0
//
// Queue
//
static inline MS_U32 _MsOS_QueueUsedSize(MsOS_Queue_Info* pQueueInfo)
{
    return (pQueueInfo->pu8Write>= pQueueInfo->pu8Read)?
               (pQueueInfo->pu8Write- pQueueInfo->pu8Read):
               (pQueueInfo->pu8Tail- pQueueInfo->pu8Head+ pQueueInfo->pu8Write- pQueueInfo->pu8Read);
}

static inline MS_U32 _MsOS_QueueFreeSize(MsOS_Queue_Info* pQueueInfo)
{
    return pQueueInfo->pu8Tail- pQueueInfo->pu8Head- _MsOS_QueueUsedSize(pQueueInfo);
}

//-------------------------------------------------------------------------------------------------
/// Create a Queue
/// @param  pStartAddr      \b IN: It is useless now, can pass NULL.
/// @param  u32QueueSize    \b IN: queue size (byte unit) : now fixed as
///                                CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE * u32MessageSize
/// @param  eMessageType    \b IN: E_MSG_FIXED_SIZE / E_MSG_VAR_SIZE
/// @param  u32MessageSize  \b IN: message size (byte unit) for E_MSG_FIXED_SIZE
///                                max message size (byte unit) for E_MSG_VAR_SIZE
/// @param  eAttribute      \b IN: E_MSOS_FIFO suspended in FIFO order
/// @param  pQueueName      \b IN: queue name
/// @return assigned message queue ID
/// @return < 0 - fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateQueue (void         *pStartAddr,
                      MS_U32           u32QueueSize,
                      MessageType   eMessageType,
                      MS_U32           u32MessageSize,
                      MsOSAttribute eAttribute,
                      char         *pQueueName)
{
    MS_S32 s32Id;
    MS_U32 u32AlignSize;
    pthread_mutexattr_t _MsOS_Mutex_Attr;

    PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
    //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
#ifdef MS_DEBUG
    PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
#endif

    if (E_MSG_VAR_SIZE== eMessageType)
    {
        printk("E_MSG_VAR_SIZE has not been supported yet\n");
        return -1;
    }

    PTH_RET_CHK(pthread_mutex_lock( &_MsOS_Queue_Mutex));
    for( s32Id=0; s32Id<MSOS_QUEUE_MAX; s32Id++)
    {
        if(_MsOS_Queue_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_QUEUE_MAX)
    {
        _MsOS_Queue_Info[s32Id].u32AlignedMsgSize = u32AlignSize= ALIGN_4(u32MessageSize);
        u32QueueSize= u32AlignSize* (CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE+1);
        _MsOS_Queue_Info[s32Id].pu8Head= (MS_U8*)malloc(u32AlignSize* (CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE+1));
        if(!_MsOS_Queue_Info[s32Id].pu8Head)
        {
            s32Id = MSOS_QUEUE_MAX; // Invalid queue index
        }
        else
        {
            _MsOS_Queue_Info[s32Id].pu8Read= _MsOS_Queue_Info[s32Id].pu8Write= _MsOS_Queue_Info[s32Id].pu8Head;
            _MsOS_Queue_Info[s32Id].pu8Tail= _MsOS_Queue_Info[s32Id].pu8Head+ u32QueueSize;
            _MsOS_Queue_Info[s32Id].bUsed = TRUE;
            _MsOS_Queue_Info[s32Id].eMsgType = eMessageType;
        }
    }
    PTH_RET_CHK(pthread_mutex_unlock( &_MsOS_Queue_Mutex));

    if(s32Id >= MSOS_QUEUE_MAX)
    {
        return -1;
    }
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].SendSem, NULL));
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].RecvSem, NULL));
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].SendMutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].RecvMutex, &_MsOS_Mutex_Attr));

    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the Queue
/// @param  s32QueueId  \b IN: Queue ID
/// @return TRUE : succeed
/// @return FALSE :  fail
/// @note   It is important that there are not any threads blocked on the queue
///             when this function is called or the behavior is undefined.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteQueue (MS_S32 s32QueueId)
{
    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    PTH_RET_CHK(pthread_mutex_lock( &_MsOS_Queue_Mutex));
    if(_MsOS_Queue_Info[s32QueueId].pu8Head)
    {
        free(_MsOS_Queue_Info[s32QueueId].pu8Head);
    }
    _MsOS_Queue_Info[s32QueueId].pu8Head= NULL;
    _MsOS_Queue_Info[s32QueueId].bUsed = FALSE;
    PTH_RET_CHK(pthread_mutex_unlock( &_MsOS_Queue_Mutex));

    PTH_RET_CHK(pthread_cond_destroy(&_MsOS_Queue_Info[s32QueueId].SendSem));
    PTH_RET_CHK(pthread_cond_destroy(&_MsOS_Queue_Info[s32QueueId].RecvSem));
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Queue_Info[s32QueueId].SendMutex));
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Queue_Info[s32QueueId].RecvMutex));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Send a message to the end of the specified queue
/// @param  s32QueueId  \b IN: Queue ID
/// @param  pu8Message  \b IN: ptr to msg to send. NULL ptr is not allowed
/// @param  u32Size     \b IN: msg size (byte)
/// @param  u32WaitMs   \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the queue is full
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SendToQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32Size, MS_U32 u32WaitMs)
{
    MsOS_Queue_Info* pQueueInfo= NULL;

    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    //coz cyg_mbox_get will return NULL for error
    if ( pu8Message==NULL)
    {
        return FALSE;
    }

    pQueueInfo= &_MsOS_Queue_Info[s32QueueId];
    if(u32Size > pQueueInfo->u32AlignedMsgSize)
    {
        return FALSE;
    }

    if (u32Size>= _MsOS_QueueFreeSize(pQueueInfo))
    {
        if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
        {
            PTH_RET_CHK(pthread_cond_wait(&pQueueInfo->RecvSem, &pQueueInfo->RecvMutex));
        }
        else
        {
            struct timespec     StopTime;
//            if (0== u32WaitMs)
//            {
//                u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//            }
            _TimeAbs(StopTime, u32WaitMs);
            if (PTH_RET_CHK(pthread_cond_timedwait(&pQueueInfo->RecvSem, &pQueueInfo->RecvMutex, &StopTime)))
            {
                return FALSE;
            }
        }
    }

    memcpy(pQueueInfo->pu8Write, pu8Message, u32Size);
    pQueueInfo->pu8Write+= pQueueInfo->u32AlignedMsgSize;
    if (pQueueInfo->pu8Write>= pQueueInfo->pu8Tail)
    {
        pQueueInfo->pu8Write= pQueueInfo->pu8Head;
    }
    PTH_RET_CHK(pthread_cond_signal(&pQueueInfo->SendSem));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Receive a message from the specified queue
/// @param  s32QueueId      \b IN: Queue ID
/// @param  pu8Message      \b OUT: msg destination
/// @param  u32IntendedSize \b IN: intended msg size (byte unit) to receive:
/// @param  pu32ActualSize  \b OUT: actual msg size (byte unit) received
/// @param  u32WaitMs       \b IN: 0 ~ MSOS_WAIT_FOREVER: suspend time (ms) if the queue is empty
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RecvFromQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32IntendedSize, MS_U32 *pu32ActualSize, MS_U32 u32WaitMs)
{
    MsOS_Queue_Info* pQueueInfo= NULL;

    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    pQueueInfo= &_MsOS_Queue_Info[s32QueueId];

    MS_ASSERT(ALIGN_4(u32IntendedSize)== pQueueInfo->u32AlignedMsgSize);

    if (0== _MsOS_QueueUsedSize(pQueueInfo))
    {
        if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
        {
            PTH_RET_CHK(pthread_cond_wait(&pQueueInfo->SendSem, &pQueueInfo->SendMutex));
        }
        else
        {
            struct timespec     StopTime;
//            if (0== u32WaitMs)
//            {
//                u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//            }
            _TimeAbs(StopTime, u32WaitMs);
            if (PTH_RET_CHK(pthread_cond_timedwait(&pQueueInfo->SendSem, &pQueueInfo->SendMutex, &StopTime)))
            {
                return FALSE;
            }
        }
    }

    memcpy(pu8Message, pQueueInfo->pu8Read, u32IntendedSize);
    pQueueInfo->pu8Read+= pQueueInfo->u32AlignedMsgSize;
    if (pQueueInfo->pu8Read>= pQueueInfo->pu8Tail)
    {
        pQueueInfo->pu8Read= pQueueInfo->pu8Head;
    }
    PTH_RET_CHK(pthread_cond_signal(&pQueueInfo->RecvSem));
    *pu32ActualSize = u32IntendedSize;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Receive a message from the specified queue
/// @param  s32QueueId      \b IN: Queue ID
/// @param  pu8Message      \b OUT: msg destination
/// @param  u32IntendedSize \b IN: intended msg size (byte unit) to receive:
/// @param  pu32ActualSize  \b OUT: actual msg size (byte unit) received
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_PeekFromQueue (MS_S32 s32QueueId, MS_U8 *pu8Message, MS_U32 u32IntendedSize, MS_U32 *pu32ActualSize)
{
    MsOS_Queue_Info* pQueueInfo= NULL;

    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    pQueueInfo= &_MsOS_Queue_Info[s32QueueId];

    MS_ASSERT(ALIGN_4(u32IntendedSize)== pQueueInfo->u32AlignedMsgSize);

    if (0== _MsOS_QueueUsedSize(pQueueInfo))
    {
        return FALSE;
    }
    memcpy(pu8Message, pQueueInfo->pu8Read, u32IntendedSize);
    *pu32ActualSize = u32IntendedSize;
    return TRUE;
}
#endif


//
// Interrupt management
//
//-------------------------------------------------------------------------------------------------
/// Attach the interrupt callback function to interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_AttachInterrupt (InterruptNum eIntNum, InterruptCb pIntCb)
{
    // char u8IsrName[32];

    // PTH_RET_CHK(pthread_mutex_lock(&_ISR_Mutex));
    // CHIP_AttachISR(eIntNum, pIntCb);
    // request_irq((int)eIntNum, pIntCb, SA_INTERRUPT, u8IsrName, NULL);
    // PTH_RET_CHK(pthread_mutex_unlock(&_ISR_Mutex));
    HAL_IRQ_Attach(eIntNum, pIntCb);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Detach the interrupt callback function from interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DetachInterrupt (InterruptNum eIntNum)
{
    // PTH_RET_CHK(pthread_mutex_lock(&_ISR_Mutex));
    HAL_IRQ_Detech((int)eIntNum);
    // PTH_RET_CHK(pthread_mutex_unlock(&_ISR_Mutex));
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable (unmask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableInterrupt (InterruptNum eIntNum)
{
    enable_irq((int)eIntNum + 8);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable (mask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DisableInterrupt (InterruptNum eIntNum)
{
    disable_irq((int)eIntNum + 8);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_DisableAllInterrupts(void)
{
#if 0
    return CHIP_DisableAllInterrupt() ;
#else
    return 0;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
#if 0
    return CHIP_EnableAllInterrupt() ;
#else
    return TRUE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
#if 0
    return CHIP_InISRContext();
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#define DCACHE_LINE_SIZE 16
MS_BOOL MsOS_Dcache_Flush( MS_U32 u32Start, MS_U32 u32Size )
{
#if defined(CONFIG_MSTAR_TITANIA3) || \
    defined(CONFIG_MSTAR_TITANIA8) || \
    defined(CONFIG_MSTAR_TITANIA4) || \
    defined(CONFIG_MSTAR_TITANIA7) || \
    defined(CONFIG_MSTAR_URANUS4)  || \
	defined(CONFIG_MSTAR_KAISER)
    // @FIXME
    // how to do flush for MIU1
#else
    _dma_cache_wback_inv((unsigned long)__va(u32Start), u32Size);
#endif
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Invalidate( MS_U32 u32Start, MS_U32 u32Size )
{
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Writeback( MS_U32 u32Start, MS_U32 u32Size )
{
    return FALSE;
}

MS_U32 MsOS_VA2PA(MS_U32 addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3) || defined(CHIP_JANUS))
        return ((MS_U32)(addr) & ~(0xC0000000));
    #else
        return ((MS_U32)(addr) & ~(0x80000000));
    #endif
#else
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        // miu0
        if ((0x80000000 <= addr) && (0x90000000 > addr))
        {
            return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
        }
        if ((0xA0000000 <= addr) && (0xB0000000 > addr))
        {
            return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
        }
        // miu1
        if ((MIU1_CACHEABLE_START <= addr) && (MIU1_CACHEABLE_END > addr))
        {
            return ((addr & ~MIU1_CACHEABLE_START) | (HAL_MIU1_BASE));
        }
        if ((MIU1_UNCACHEABLE_START <= addr) && (MIU1_UNCACHEABLE_END > addr))
        {
            return ((MS_U32)(((MS_U32)(addr & ~MIU1_UNCACHEABLE_START)) | (HAL_MIU1_BASE)));
        }
        return 0;
    #else
        return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
    #endif
#endif
}

MS_U32 MsOS_PA2KSEG0(MS_U32 addr)
{
#if (defined(MCU_AEON))
    return (addr);
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        // miu0
        if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
        {
            return ((MS_U32)(((MS_U32)addr) | 0x80000000));
        }
        // miu1
        if ((HAL_MIU1_BASE <= addr) && ((HAL_MIU1_BASE * 2) > addr))
        {
            return ((addr & ~(HAL_MIU1_BASE)) | 0xC0000000);
        }
        return 0;
    #else
        return ((MS_U32)(((MS_U32)addr) | 0x80000000));
    #endif
#endif
}

MS_U32 MsOS_PA2KSEG1(MS_U32 addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3) || defined(CHIP_U4) )
        return ((MS_U32)(addr) | (0xC0000000));
    #else
        return ((MS_U32)(addr) | (0x80000000));
    #endif
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        // miu0
        if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
        {
            return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
        }
        // miu1
        if ((HAL_MIU1_BASE <= addr) && ((HAL_MIU1_BASE * 2) > addr))
        {
            return ((addr & ~(HAL_MIU1_BASE)) | 0xD0000000);
        }
        return 0;
    #else
        return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
    #endif
#endif
}

#if 1
//-------------------------------------------------------------------------------------------------
/// Disable the CPU interrupt
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_CPU_DisableInterrupt (void)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Attach the CPU interrupt callback function to interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_AttachInterrupt (MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, MS_U32 dat)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Detach the CPU interrupt callback function to interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_DetachInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Attach the CPU exception callback function to interrupt #
/// @param  eIntNum \b IN: Exception number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Exception callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_AttachException (MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, MS_U32 dat)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Detach the CPU exception callback function to interrupt #
/// @param  eIntNum \b IN: Exception number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Exception callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_DetachExceptiont (MHAL_EXCEPTION_TYPE expt_num)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Sync data in EC-Brigde
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);

void MsOS_Sync(void)
{//not support in current stage.
    // printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

void MsOS_FlushMemory(void)
{
    Chip_Flush_Memory(); // Refine for CDI Report issues
}

void MsOS_ReadMemory(void)
{
    Chip_Read_Memory();  // Refine for CDI Report issues
}

#endif
