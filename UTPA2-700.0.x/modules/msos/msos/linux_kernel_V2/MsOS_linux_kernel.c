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


#if defined(MSOS_TYPE_LINUX_KERNEL)

#define _GNU_SOURCE

#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/semaphore.h>
#include <linux/hrtimer.h>
#include <chip_setup.h>
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"
#include "mem.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------

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

#define PTH_RET_CHK(_pf_) _pf_

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
//
// Variable-size Memory pool
//
#if defined(MSOS_TYPE_LINUX_KERNEL)

#include "MsTypes.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvIRQ.h"
#include "halIRQ.h"
#include "utopia.h"

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
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
     defined(CHIP_MIAMI)) || \
	 defined(CHIP_NAPOLI)
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
//
// Memory management
//
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
static                          DEFINE_MUTEX(_MsOS_MemoryPool_Mutex);
#define MEMORY_MUTEX_LOCK()     mutex_lock(&_MsOS_MemoryPool_Mutex)
#define MEMORY_MUTEX_UNLOCK()   mutex_unlock(&_MsOS_MemoryPool_Mutex)
#endif

//
// Task Management
//
typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bTerminal;
    TaskEntry           pTaskEntry;
    MS_U32              u32TaskEntryData;
    struct task_struct* pstThreadInfo;
} MsOS_Task_Info;

static MsOS_Task_Info   _MsOS_Task_Info[MSOS_TASK_MAX];

static                          DEFINE_MUTEX(_MsOS_Task_Mutex);
#define TASK_MUTEX_LOCK()       mutex_lock(&_MsOS_Task_Mutex)
#define TASK_MUTEX_UNLOCK()     mutex_unlock(&_MsOS_Task_Mutex)

//
// Mutex
//
typedef struct
{
    MS_BOOL             bUsed;
    struct mutex        stMutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;

static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static                          DEFINE_MUTEX(_MsOS_Mutex_Mutex);
#define MUTEX_MUTEX_LOCK()      mutex_lock(&_MsOS_Mutex_Mutex)
#define MUTEX_MUTEX_UNLOCK()    mutex_unlock(&_MsOS_Mutex_Mutex)


//
// Semaphore
//
typedef struct
{
    MS_BOOL                     bUsed;
    MS_U32                      u32SemaCount;
    struct semaphore            stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info      _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];
static                          DEFINE_SPINLOCK(_MsOS_Semaphore_Mutex);
#define SEMA_MUTEX_LOCK(flags)       spin_lock_irqsave(&_MsOS_Semaphore_Mutex, flags)
#define SEMA_MUTEX_UNLOCK(flags)     spin_unlock_irqrestore(&_MsOS_Semaphore_Mutex, flags)

//
// Event Group
//
typedef struct
{
    MS_BOOL                     bUsed;
    MS_U32                      u32Waiting;
    MS_U32                      u32EventGroup;
    spinlock_t                  stMutexEvent;
    wait_queue_head_t           stEventWaitQueue;
    // pthread_cond_t              stSemaphore; // ?????????????
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info     _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static                          DEFINE_SPINLOCK(_MsOS_EventGroup_Mutex);
#define EVENT_MUTEX_LOCK(flags)      spin_lock_irqsave(&_MsOS_EventGroup_Mutex,flags)
#define EVENT_MUTEX_UNLOCK(flags)    spin_unlock_irqrestore(&_MsOS_EventGroup_Mutex, flags)

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
    MS_BOOL             bCircle;
    TimerCb             pTimerCb;
    struct timer_list   timer;
    int                 period;
    int                 first;
} MsOS_Timer_Info;
static MsOS_Timer_Info  _MsOS_Timer_Info[MSOS_TIMER_MAX];
static                          DEFINE_SPINLOCK(_MsOS_Timer_Mutex);
#define TIMER_MUTEX_LOCK(flags)      spin_lock_irqsave(&_MsOS_Timer_Mutex, flags)
#define TIMER_MUTEX_UNLOCK(flags)    spin_unlock_irqrestore(&_MsOS_Timer_Mutex, flags)
#endif

//
// compatible shm function.
//
#define MAX_SHM_CLIENT_NUM      320

struct SHM_ENTRY {
    char    name[MAX_CLIENT_NAME_LENGTH+ 1];
    void*   addr;
    unsigned int size;
};

struct SHM_ARRAY {
	unsigned int count;
	struct SHM_ENTRY shm_entry[MAX_SHM_CLIENT_NUM+1];
};

struct SHM_ARRAY g_shm_array;

static                          DEFINE_MUTEX(_MsOS_SHM_Mutex);
#define SHM_MUTEX_LOCK()        mutex_lock(&_MsOS_SHM_Mutex)
#define SHM_MUTEX_UNLOCK()      mutex_unlock(&_MsOS_SHM_Mutex)

// Intitialization
static DEFINE_MUTEX(_MsOS_Init_Mutex);
static MS_BOOL g_bMsOSInit = FALSE;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

static int _MsOS_LinuxKernelTaskWrapper(void *data);

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
    MS_BOOL bRet = FALSE;

    mutex_lock(&_MsOS_Init_Mutex);

    if (g_bMsOSInit)
    {
        bRet = TRUE;
        goto ret;
    }

    if (FALSE == MsOS_SHM_Init())
    {
        printk("[MsOS_Init] MsOS_SHM_Init failed!\n");
        bRet = FALSE;
        goto ret;
    }

    // Empty all the MsOS structures

#if 1
    //
    // Memory management
    //
    for( u32I=0; u32I<MSOS_MEMPOOL_MAX; u32I++)
    {
        _MsOS_MemoryPool_Info[u32I].bUsed = FALSE;
    }
#endif

    //
    // Task Management
    //
    for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
    {
        _MsOS_Task_Info[u32I].bUsed = FALSE;
    }

    //
    // Mutex
    //
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }

    //
    // Semaphore
    //
    for( u32I=0; u32I<MSOS_SEMAPHORE_MAX; u32I++)
    {
        _MsOS_Semaphore_Info[u32I].bUsed = FALSE;
    }


    //
    // Event Group
    //
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
    for( u32I=0; u32I<MSOS_TIMER_MAX; u32I++)
    {
        _MsOS_Timer_Info[u32I].bUsed = FALSE;
        _MsOS_Timer_Info[u32I].bCircle = FALSE;
        _MsOS_Timer_Info[u32I].pTimerCb = NULL;
        _MsOS_Timer_Info[u32I].period = 0;
        _MsOS_Timer_Info[u32I].first = 0;
        memset(&_MsOS_Timer_Info[u32I].timer, 0, sizeof(struct timer_list));
        //init_timer(&(_MsOS_Timer_Info[u32I].timer));
    }

    //
    // Interrupt
    //
    printk("CHIP_InitISR\n");
    CHIP_InitISR();

    UtopiaInit();

    bRet = TRUE;
    g_bMsOSInit = TRUE;

ret:
    mutex_unlock(&_MsOS_Init_Mutex);
    return bRet;
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

    MEMORY_MUTEX_LOCK();
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
    MEMORY_MUTEX_UNLOCK();

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

    if ( (s32PoolId >= MSOS_MEMPOOL_MAX) )
    {
        if ( s32PoolId == MSOS_MALLOC_ID )
        {
            // here ??
        }

        printk("Invalid memory pool ID: %td, you must use the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
        return TRUE;

    }

    #if 1
    if(_MsOS_MemoryPool_Info[s32PoolId].bUsed == FALSE)
    {
        printk("ERR: MEMORY POOL WITH MEMORYPOOL_ID: 0x%tX NOT EXIST\n", (ptrdiff_t)s32PoolId);
        return FALSE;
    }
    #endif
    if (_MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        cyg_mempool_var_delete(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
    }

    MEMORY_MUTEX_LOCK();
    _MsOS_MemoryPool_Info[s32PoolId].bUsed = FALSE;
    MEMORY_MUTEX_UNLOCK();
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


    if ( s32PoolId >= MSOS_MEMPOOL_MAX)
    {

        if ( s32PoolId == MSOS_MALLOC_ID )
        {
            return malloc(u32Size);
        }

        else
        {
            printk("ERR: Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
            return NULL;
        }

    }

    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        return malloc(u32Size);
    }

    pAddr = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);

    //Current eCosPro kernel always allocates 16-B aligned block
    if (( (MS_VIRT)pAddr & 0xF) || ( (MS_VIRT)pAddr == 0x0))
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



    if ( s32PoolId >= MSOS_MEMPOOL_MAX )
    {
        if ( s32PoolId == MSOS_MALLOC_ID )
        {
            free(pAddress);
            return TRUE;
        }

        else
        {
            printk("ERR: Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
            return FALSE;
        }

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

#if 0 // Need to reimplement
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
                        MS_VIRT u32TaskEntryData,
                        TaskPriority eTaskPriority,
                        MS_BOOL bAutoStart,
                        void * pStackEntry,
                        MS_U32 u32StackSize,
                        char *pTaskName)
{
    // @FIXME:
    //     (1) eTaskPriority: Task priority is ignored here
    //     (2) pTaskName: is ignored here
    //     (3) Need mutex to protect critical section

    MS_S32 s32Id;

    TASK_MUTEX_LOCK();

    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if( s32Id >= MSOS_TASK_MAX)
    {
        TASK_MUTEX_UNLOCK();
        return -1;
    }
    _MsOS_Task_Info[s32Id].bUsed = TRUE;
    _MsOS_Task_Info[s32Id].bTerminal = FALSE;
    _MsOS_Task_Info[s32Id].pTaskEntry = pTaskEntry;
    _MsOS_Task_Info[s32Id].u32TaskEntryData = u32TaskEntryData;
    _MsOS_Task_Info[s32Id].pstThreadInfo = kthread_create(  _MsOS_LinuxKernelTaskWrapper,
                                                            (void *)&_MsOS_Task_Info[s32Id],
                                                            pTaskName);
    if (IS_ERR(_MsOS_Task_Info[s32Id].pstThreadInfo))
    {
        _MsOS_Task_Info[s32Id].bUsed = FALSE;
        _MsOS_Task_Info[s32Id].bTerminal = FALSE;
        _MsOS_Task_Info[s32Id].pTaskEntry = NULL;
        _MsOS_Task_Info[s32Id].u32TaskEntryData = 0;
        _MsOS_Task_Info[s32Id].pstThreadInfo = NULL;
        TASK_MUTEX_UNLOCK();
        printk("Utopia2K Task %s Create Fail\n", pTaskName);
        return -1;
    }
    TASK_MUTEX_UNLOCK();

    if (bAutoStart)
    {
        wake_up_process(_MsOS_Task_Info[s32Id].pstThreadInfo);
    }

    s32Id |= MSOS_ID_PREFIX;

    return s32Id;
}

static int _MsOS_LinuxKernelTaskWrapper(void *data)
{
    MsOS_Task_Info  *pMsOS_Task_Info = (MsOS_Task_Info *)data;
    TaskEntry       pTaskEntry = pMsOS_Task_Info->pTaskEntry;
    MS_U32          TaskEntryData = pMsOS_Task_Info->u32TaskEntryData;

    pTaskEntry(TaskEntryData, (void *)pMsOS_Task_Info);

    TASK_MUTEX_LOCK();
    pMsOS_Task_Info->bTerminal = TRUE;
    pMsOS_Task_Info->bUsed = FALSE;
    pMsOS_Task_Info->pTaskEntry = NULL;
    pMsOS_Task_Info->u32TaskEntryData = 0;
    TASK_MUTEX_UNLOCK();

    return 0;
}

//-------------------------------------------------------------------------------------------------
/// Delete a previously created task
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId)
{
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    TASK_MUTEX_LOCK();
    if (!_MsOS_Task_Info[s32TaskId].bTerminal)
    {
        if (_MsOS_Task_Info[s32TaskId].bUsed)
            printk("%s : Utopia2K does not support to force destroy Task!\n", __func__);
        TASK_MUTEX_UNLOCK();
        return FALSE;
    }
    //It dose not support force destroy thread function, because it need used kthread_should_stop() in thread loop.
    //kthread_stop(_MsOS_Task_Info[s32TaskId].pstThreadInfo);
    TASK_MUTEX_UNLOCK();

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
    // Refer to Documentation/timers/timers-howto.txt
    //
    // SLEEPING FOR ~USECS OR SMALL MSECS ( 10us - 20ms):
    //        * Use usleep_range
    // SLEEPING FOR LARGER MSECS ( 10ms+ )
    //        * Use msleep or possibly msleep_interruptible
    if(u32Ms <= 20)
    {
        usleep_range(u32Ms * 1000, u32Ms * 1000);
    }
    else
    {
        msleep_interruptible((unsigned int)u32Ms);
    }
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
}

//-------------------------------------------------------------------------------------------------
/// Delay Poll for u32Us microseconds
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs_Poll(MS_U32 u32Us)
{
    MsOS_DelayTaskUs(u32Us);
}

//-------------------------------------------------------------------------------------------------
/// Get task status
/// @param  pTaskInfo       \b IN: task information structure pointer
/// @param  peTaskState      \b OUT: ptr to task istate
/// @return TRUE : succeed
/// @return FALSE : invalid process result
/// @note not support
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_GetTaskStatus (Task_Info* pTaskInfo, TaskStatus *peTaskState)
{

    if ( (pTaskInfo == NULL) || (peTaskState == NULL))
    {
        return FALSE;
    }

    *peTaskState = E_TASK_INVALID_STATE;
    /* not support */
    return TRUE;
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

    TASK_MUTEX_LOCK();
    if (!_MsOS_Task_Info[s32TaskId].bUsed || _MsOS_Task_Info[s32TaskId].bTerminal)
    {
        TASK_MUTEX_UNLOCK();
        return FALSE;
    }
    TASK_MUTEX_UNLOCK();
    wake_up_process(_MsOS_Task_Info[s32TaskId].pstThreadInfo);

    return TRUE;
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

    TASK_MUTEX_LOCK();
    for (s32Id=0; s32Id < MSOS_TASK_MAX; s32Id++)
    {
        if (self == _MsOS_Task_Info[s32Id].pstThreadInfo)
        {
            break;
        }
    }
    TASK_MUTEX_UNLOCK();

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

    MUTEX_MUTEX_LOCK();
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
        if (FALSE==_MsOS_Mutex_Info[s32Id].bUsed && MSOS_MUTEX_MAX==s32LstUnused)
        {
            s32LstUnused = s32Id;
        }
    }
    if ((MSOS_MUTEX_MAX==s32Id) && (MSOS_MUTEX_MAX>s32LstUnused))
    {
        _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
        strcpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName);
        mutex_init(&_MsOS_Mutex_Info[s32LstUnused].stMutex);
        s32Id = s32LstUnused;
    }
    MUTEX_MUTEX_UNLOCK();

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

    //MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);
    if (!_MsOS_Mutex_Info[s32MutexId].bUsed)
    {
        MUTEX_MUTEX_UNLOCK();
        return FALSE;
    }

    /**
     * mutex_is_locked - is the mutex locked
     * @lock: the mutex to be queried
     *
     * Returns 1 if the mutex is locked, 0 if unlocked.
     */
    if (TRUE == mutex_is_locked(&_MsOS_Mutex_Info[s32MutexId].stMutex))
    {
        MUTEX_MUTEX_UNLOCK();
        printk("[Utopia2K] %s : Still locked mutex can not be deleted.\n", __func__);
        return FALSE;
    }

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

    MUTEX_MUTEX_LOCK();
    if (!_MsOS_Mutex_Info[s32MutexId].bUsed)
    {
        MUTEX_MUTEX_UNLOCK();
        return FALSE;
    }
    MUTEX_MUTEX_UNLOCK();

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        mutex_lock(&_MsOS_Mutex_Info[s32MutexId].stMutex);
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if (!mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex))
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
        //printk("[%s][%d] %s doesn't support mutex timeout at this stage\n", __FUNCTION__, __LINE__, __FUNCTION__);
        mutex_lock(&_MsOS_Mutex_Info[s32MutexId].stMutex);
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

    MUTEX_MUTEX_LOCK();
    if (!_MsOS_Mutex_Info[s32MutexId].bUsed)
    {
        MUTEX_MUTEX_UNLOCK();
        return FALSE;
    }
    MUTEX_MUTEX_UNLOCK();

    mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].stMutex);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attempt to unlock a mutex by another thread
/// @param  s32MutexId  \b IN: mutex ID
/// @param  bEnable     \b IN: True(Enable) or False(Disable)
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   Let another thread can unlock it.
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableCrossThreadReleaseMutex (MS_S32 s32MutexId, MS_BOOL bEnable)
{
    // NOT SUPPORT IN UTPA KDRV
    return FALSE;
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
    unsigned long flags;

    //PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Semaphore_Mutex));
    SEMA_MUTEX_LOCK(flags);
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

    if(s32Id >= MSOS_SEMAPHORE_MAX)
    {
        SEMA_MUTEX_UNLOCK(flags);
        return -1;
    }

    sema_init(&_MsOS_Semaphore_Info[s32Id].stSemaphore, u32InitCnt);
    _MsOS_Semaphore_Info[s32Id].u32SemaCount = u32InitCnt;
    s32Id |= MSOS_ID_PREFIX;

    SEMA_MUTEX_UNLOCK(flags);

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
    MS_S32  s32Count=0;
    MS_BOOL bResult, bLockAll=TRUE;
    unsigned long flags;

    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    SEMA_MUTEX_LOCK(flags);

    if (!_MsOS_Semaphore_Info[s32SemaphoreId].bUsed)
    {
        SEMA_MUTEX_UNLOCK(flags);
        return FALSE;
    }

    for(s32Count=0;s32Count<_MsOS_Semaphore_Info[s32SemaphoreId].u32SemaCount;s32Count++)
    {
        /**
         * down_trylock - try to acquire the semaphore, without waiting
         * @sem: the semaphore to be acquired
         *
         * Try to acquire the semaphore atomically.  Returns 0 if the mutex has
         * been acquired successfully or 1 if it it cannot be acquired.
         */
        bResult = down_trylock(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);
        if (bResult!=0)
        {
            bLockAll = FALSE;
            break;
        }
    }

    for(;s32Count>0;s32Count--)
        up(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);

    if (!bLockAll)
    {
        printk("[%s][%d] Semaphore Delete FAIL!\n", __FUNCTION__, __LINE__);
        SEMA_MUTEX_UNLOCK(flags);
        return FALSE;
    }

    _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
    _MsOS_Semaphore_Info[s32SemaphoreId].u32SemaCount = 0;

    SEMA_MUTEX_UNLOCK(flags);
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
    unsigned long flags;

    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    SEMA_MUTEX_LOCK(flags);
    if (!_MsOS_Semaphore_Info[s32SemaphoreId].bUsed)
    {
        SEMA_MUTEX_UNLOCK(flags);
        return FALSE;
    }
    SEMA_MUTEX_UNLOCK(flags);

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
    #if 0
        /**
        * down_interruptible - acquire the semaphore unless interrupted
        * @sem: the semaphore to be acquired
        *
        * Attempts to acquire the semaphore.  If no more tasks are allowed to
        * acquire the semaphore, calling this function will put the task to sleep.
        * If the sleep is interrupted by a signal, this function will return -EINTR.
        * If the semaphore is successfully acquired, this function returns 0.
        */
        bRet = down_interruptible(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore)  >=0 ? TRUE : FALSE;
    #else
        /**
        * down - acquire the semaphore
        * @sem: the semaphore to be acquired
        *
        * Acquires the semaphore.  If no more tasks are allowed to acquire the
        * semaphore, calling this function will put the task to sleep until the
        * semaphore is released.
        *
        * Use of this function is deprecated, please use down_interruptible() or
        * down_killable() instead.
        */
        down(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);
        bRet = TRUE;
    #endif
    }
    else //blocking wait with timeout
    {
        long    timeout;

        timeout = msecs_to_jiffies(u32WaitMs);

    #if 0
        /**
        * down_timeout_interruptible - acquire the semaphore within a specified time unless interrupted
        * @sem: the semaphore to be acquired
        * @jiffies: how long to wait before failing
        */
        bRet = (down_timeout_interruptible(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, timeout) >=0 ? TRUE : FALSE);
    #else
        /**
        * down_timeout - acquire the semaphore within a specified time
        * @sem: the semaphore to be acquired
        * @jiffies: how long to wait before failing
        *
        * Attempts to acquire the semaphore.  If no more tasks are allowed to
        * acquire the semaphore, calling this function will put the task to sleep.
        * If the semaphore is not released within the specified number of jiffies,
        * this function returns -ETIME.  It returns 0 if the semaphore was acquired.
        */
        bRet = (down_timeout(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, timeout) >=0 ? TRUE : FALSE);
    #endif
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
    unsigned long flags;
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    SEMA_MUTEX_LOCK(flags);
    if (!_MsOS_Semaphore_Info[s32SemaphoreId].bUsed)
    {
        SEMA_MUTEX_UNLOCK(flags);
        return FALSE;
    }
    SEMA_MUTEX_UNLOCK(flags);

    /**
    * up - release the semaphore
    * @sem: the semaphore to release
    *
    * Release the semaphore.  Unlike mutexes, up() may be called from any
    * context and even by tasks which have never called down().
    */
    up(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);
    return TRUE;
}

#if 0
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

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return -1;
    }

    spin_lock_init(&_MsOS_EventGroup_Info[s32Id].stMutexEvent);
    _MsOS_EventGroup_Info[s32Id].bUsed = TRUE;
    _MsOS_EventGroup_Info[s32Id].u32Waiting = 0;
    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32Id].stMutexEvent, flag);
    _MsOS_EventGroup_Info[s32Id].u32EventGroup= 0;
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32Id].stMutexEvent, flag);

    EVENT_MUTEX_UNLOCK(flag);

    init_waitqueue_head(&_MsOS_EventGroup_Info[s32Id].stEventWaitQueue);
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
    unsigned long flags;
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    EVENT_MUTEX_LOCK(flags);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flags);
        return FALSE;
    }

    if (0 < _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting)
    {
        EVENT_MUTEX_UNLOCK(flags);
        printk("\033[35m[%s][%d] EventGroup Delete FAIL : Event was still waiting.\033[m\n", __func__, __LINE__);
        return FALSE;
    }

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flags);
    _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup= 0;
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flags);
    _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
    _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting = 0;
    EVENT_MUTEX_UNLOCK(flags);

    init_waitqueue_head(&_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue);
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

    EVENT_MUTEX_LOCK(flag);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return FALSE;
    }
    EVENT_MUTEX_UNLOCK(flag);

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
    SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    wake_up_all(&_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue);
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);

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

    EVENT_MUTEX_LOCK(flag);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return FALSE;
    }
    EVENT_MUTEX_UNLOCK(flag);

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
    RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);

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

    EVENT_MUTEX_LOCK(flag);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return FALSE;
    }
    _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting++;
    EVENT_MUTEX_UNLOCK(flag);

    bClear= ((E_AND_CLEAR== eWaitMode) || (E_OR_CLEAR== eWaitMode))? TRUE: FALSE;
    bAnd= ((E_AND== eWaitMode)|| (E_AND_CLEAR== eWaitMode))? TRUE: FALSE;

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);

    do{
        if (u32WaitMs== MSOS_WAIT_FOREVER) //blocking wait
        {
            if (bAnd)
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
                wait_event(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                            ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag));
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
            }
            else
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
                wait_event(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                            ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0));
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
            }
        }
        else if (u32WaitMs == 0)
        {
            *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
            break;
        }
        else
        {
            u32WaitMs = msecs_to_jiffies(u32WaitMs);

            if (bAnd)
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                                    ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag),
                                    u32WaitMs);
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
            }
            else
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                                    ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0),
                                    u32WaitMs);
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
            }
        }
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
    } while (0);

    //spin_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    bRet= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (bRet && bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);

    EVENT_MUTEX_LOCK(flag);
    _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting--;
    EVENT_MUTEX_UNLOCK(flag);

    return bRet;
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
/// @return -ERESTARTSYS : interrupt by  ERESTARTSYS
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_WaitEvent_Interrupt(MS_S32 s32EventGroupId,
                     MS_U32 u32WaitEventFlag,
                     MS_U32 *pu32RetrievedEventFlag,
                     EventWaitMode eWaitMode,
                     MS_U32 u32WaitMs)
{
    MS_S32 s32Ret= FALSE;
    MS_BOOL bAnd;
    MS_BOOL bClear;
    unsigned long flag;

    *pu32RetrievedEventFlag = 0;

    if (!u32WaitEventFlag)
    {
        return s32Ret;
    }

    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return s32Ret;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    EVENT_MUTEX_LOCK(flag);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return (MS_S32)FALSE;
    }
    _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting++;
    EVENT_MUTEX_UNLOCK(flag);

    bClear= ((E_AND_CLEAR== eWaitMode) || (E_OR_CLEAR== eWaitMode))? TRUE: FALSE;
    bAnd= ((E_AND== eWaitMode)|| (E_AND_CLEAR== eWaitMode))? TRUE: FALSE;

    spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);

    do{
        if (u32WaitMs== MSOS_WAIT_FOREVER) //blocking wait
        {
            if (bAnd)
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
                do
                {
                    s32Ret = wait_event_interruptible(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                            ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag));
                    if(s32Ret == -ERESTARTSYS)
                    {
					_MsOS_EventGroup_Info[s32EventGroupId].u32Waiting--;
                    return -ERESTARTSYS;
                    }
				
                }while(!((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag)== u32WaitEventFlag));
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
            }
            else
            {
                do
                {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
                s32Ret = wait_event_interruptible(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                            ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0));
                    if(s32Ret == -ERESTARTSYS)
                    {
					_MsOS_EventGroup_Info[s32EventGroupId].u32Waiting--;
                    return -ERESTARTSYS;
                    }
                }while(!((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag)!= 0));
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
            }
        }
        else
        {
            if(u32WaitMs != 0)
            {
            u32WaitMs = msecs_to_jiffies(u32WaitMs);
            if (bAnd)
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                                    ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag),
                                    u32WaitMs);
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
            }
            else
            {
                spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);
                wait_event_timeout(_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue,
                                    ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0),
                                    u32WaitMs);
                spin_lock_irqsave(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, flag);
            }
            }
        }
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
    } while (0);

    //spin_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    s32Ret= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (s32Ret && (MS_S32)bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    spin_unlock_irqrestore(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,flag);

    EVENT_MUTEX_LOCK(flag);
    _MsOS_EventGroup_Info[s32EventGroupId].u32Waiting--;
    EVENT_MUTEX_UNLOCK(flag);

    return s32Ret;
}

wait_queue_head_t* MsOS_GetEventQueue (MS_S32 s32EventGroupId)
{
    unsigned long flag;
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    EVENT_MUTEX_LOCK(flag);
    if (!_MsOS_EventGroup_Info[s32EventGroupId].bUsed)
    {
        EVENT_MUTEX_UNLOCK(flag);
        return NULL;
    }
    EVENT_MUTEX_UNLOCK(flag);

    return (&_MsOS_EventGroup_Info[s32EventGroupId].stEventWaitQueue);
}

/// @FIXME: Timer API are empty
//
// Timer management
//
static void _MsOS_TimerNotify(unsigned long data)
{
    MS_S32 s32Id = (MS_S32) data;
    TimerCb pfnTimerCb;
    unsigned long flags;

    TIMER_MUTEX_LOCK(flags);

    if( !_MsOS_Timer_Info[s32Id].bUsed )
    {
        TIMER_MUTEX_UNLOCK(flags);
        return;
    }

    pfnTimerCb = _MsOS_Timer_Info[s32Id].pTimerCb;
    _MsOS_Timer_Info[s32Id].timer.expires = jiffies + HZ*_MsOS_Timer_Info[s32Id].period/1000;

    if ( _MsOS_Timer_Info[s32Id].bCircle )
        add_timer(&(_MsOS_Timer_Info[s32Id].timer));

    TIMER_MUTEX_UNLOCK(flags);

    if (pfnTimerCb)
    {
        pfnTimerCb(0, s32Id | MSOS_ID_PREFIX);
    }
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
    unsigned long flags;

    TIMER_MUTEX_LOCK(flags);
    for(s32Id=0;s32Id<MSOS_TIMER_MAX;s32Id++)
    {
        if(_MsOS_Timer_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }

    if(s32Id >= MSOS_TIMER_MAX)
    {
        TIMER_MUTEX_UNLOCK(flags);
        return -1;
    }

    init_timer(&(_MsOS_Timer_Info[s32Id].timer));
    _MsOS_Timer_Info[s32Id].bUsed = TRUE;
    _MsOS_Timer_Info[s32Id].bCircle = TRUE;
    _MsOS_Timer_Info[s32Id].pTimerCb = pTimerCb;
    _MsOS_Timer_Info[s32Id].first = u32FirstTimeMs;
    _MsOS_Timer_Info[s32Id].period = u32PeriodTimeMs;
    _MsOS_Timer_Info[s32Id].timer.data = (unsigned long)s32Id;
    _MsOS_Timer_Info[s32Id].timer.expires = jiffies + HZ*u32FirstTimeMs/1000;
    _MsOS_Timer_Info[s32Id].timer.function = _MsOS_TimerNotify;

    if (bStartTimer)
    {
        add_timer(&(_MsOS_Timer_Info[s32Id].timer));
    }
    TIMER_MUTEX_UNLOCK(flags);
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
    unsigned long flags;
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    TIMER_MUTEX_LOCK(flags);
    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        del_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
        _MsOS_Timer_Info[s32TimerId].bCircle = FALSE;
        _MsOS_Timer_Info[s32TimerId].pTimerCb = NULL;
        _MsOS_Timer_Info[s32TimerId].period = 0;
        _MsOS_Timer_Info[s32TimerId].first = 0;
        memset(&_MsOS_Timer_Info[s32TimerId].timer, 0, sizeof(struct timer_list));
        TIMER_MUTEX_UNLOCK(flags);
        return TRUE;
    }
    else
    {
        TIMER_MUTEX_UNLOCK(flags);
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
    unsigned long flags;
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    TIMER_MUTEX_LOCK(flags);
    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        _MsOS_Timer_Info[s32TimerId].bCircle = TRUE;
        _MsOS_Timer_Info[s32TimerId].timer.expires = jiffies + _MsOS_Timer_Info[s32TimerId].period/1000;
        add_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        TIMER_MUTEX_UNLOCK(flags);
        return TRUE;
    }
    else
    {
        TIMER_MUTEX_UNLOCK(flags);
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
    unsigned long flags;
    //return FALSE;

    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    TIMER_MUTEX_LOCK(flags);
    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        _MsOS_Timer_Info[s32TimerId].bCircle = FALSE;
        del_timer(&(_MsOS_Timer_Info[s32TimerId].timer));
        TIMER_MUTEX_UNLOCK(flags);
        return TRUE;
    }
    else
    {
        TIMER_MUTEX_UNLOCK(flags);
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

    getrawmonotonic(&ts);
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
    HAL_IRQ_Attach(eIntNum, pIntCb,IRQF_ONESHOT);
    return TRUE;
}


MS_BOOL MsOS_AttachInterrupt_Shared (InterruptNum eIntNum, InterruptCb pIntCb)
{
    HAL_IRQ_Attach(eIntNum, pIntCb,IRQF_SHARED);
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
    HAL_IRQ_Detech((int)eIntNum);
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
    return CHIP_EnableIRQ((int)eIntNum);
}

//-------------------------------------------------------------------------------------------------
/// Disable (mask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DisableInterrupt (InterruptNum eIntNum)
{
    return CHIP_DisableIRQ(eIntNum);
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
MS_BOOL MsOS_Dcache_Flush( MS_VIRT ptrStart, MS_SIZE tSize )
{
    return MsOS_MPool_Dcache_Flush(ptrStart, tSize);
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines for all.
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Flush_All(void)
{
    return MsOS_MPool_Dcache_Flush_All();
}

//-------------------------------------------------------------------------------------------------
/// Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Invalidate( MS_VIRT ptrStart , MS_SIZE tSize )
{
#if 0
    Chip_Inv_Cache_Range(ptrStart, tSize);
    return TRUE;
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Writeback( MS_VIRT ptrStart , MS_SIZE tSize )
{
#if 0
    Chip_Clean_Cache_Range(u32Start, u32Size);
    return TRUE;
#else
    return FALSE;
#endif
}

MS_PHY MsOS_VA2PA(MS_VIRT addr)
{
    return MsOS_MPool_VA2PA(addr);
}

MS_VIRT MsOS_PA2KSEG0(MS_PHY addr)
{
    return MsOS_MPool_PA2KSEG0(addr);
}

MS_VIRT MsOS_PA2KSEG1(MS_PHY addr)
{
    return MsOS_MPool_PA2KSEG1(addr);
}

// Share memory operation
static int _shm_init = -1;

MS_BOOL MsOS_SHM_Init(void)
{
    SHM_MUTEX_LOCK();

    if (-1 != _shm_init)
    {
        return TRUE;
    }

	memset(&g_shm_array, 0, sizeof(g_shm_array));
    //Init First data as default.
    strcpy(g_shm_array.shm_entry[0].name, "DEFAULT_NULL");
    g_shm_array.shm_entry[0].addr = NULL;
    g_shm_array.shm_entry[0].size = 0;
    g_shm_array.count = 1;

    _shm_init = 1;

    SHM_MUTEX_UNLOCK();

    return TRUE;
}

char tmp[2][850*1024];
int tmp_cnt=0;
// Share memory operation
MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag)
{
	int i, match = FALSE, result = FALSE;

    SHM_MUTEX_LOCK();

    if (-1 == _shm_init)
    {
        printk("[%s][%d] SHM Array was not initial!\n", __FUNCTION__, __LINE__);
        SHM_MUTEX_UNLOCK();
        return result;
    }

	// search matching entry
	for (i = 0; i < g_shm_array.count; i++)
	{
		if (0 == strcmp(g_shm_array.shm_entry[i].name, (char*)pu8ClientName))
		{
			match = TRUE;
			break;
		}
	}

	if (match) {
		// if match, fill info
		*pu32ShmId = i;
		*pu32Addr = (MS_VIRT)(g_shm_array.shm_entry[i].addr);
		*pu32BufSize = g_shm_array.shm_entry[i].size;
		result = TRUE;
	} else if (u32Flag == MSOS_SHM_CREATE) {
		// if not match & specify to create one, register to g_shm_array
		*pu32ShmId = g_shm_array.count;
        if(u32BufSize > 700*1024)
        {
            *pu32Addr = tmp[tmp_cnt];
            tmp_cnt++;
            if(tmp_cnt>=3)printk("ERROR: patch size not enough, please find me %s %d\n",__FUNCTION__,__LINE__);
            else printk("Please ask jway to fix me\n");
            memset((void*)*pu32Addr, 0,850*1024);
        }else
        {        		
            *pu32Addr = (MS_VIRT)kmalloc(u32BufSize, GFP_ATOMIC);
            if(NULL == *pu32Addr)
            {
                SHM_MUTEX_UNLOCK();
                return result;
            }
            memset((void*)*pu32Addr, 0, u32BufSize);
        }
        
		*pu32BufSize = u32BufSize;
		// this would cause bug, because string source may be "stack variable"
		//g_shm_array.shm_entry[g_shm_array.count].name = (char*)pu8ClientName;
		strcpy(g_shm_array.shm_entry[g_shm_array.count].name, (char*)pu8ClientName);
		g_shm_array.shm_entry[g_shm_array.count].addr = (void*)*pu32Addr;
		g_shm_array.shm_entry[g_shm_array.count].size = *pu32BufSize;
		g_shm_array.count++;
		result = TRUE;
	}
    SHM_MUTEX_UNLOCK();

	return result;
}

MS_BOOL MsOS_SHM_FreeId(MS_U8* pu8ClientName, MS_U32 u32ShmId)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

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

//-------------------------------------------------------------------------------------------------
/// Sync data in EC-Brigde
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------

void MsOS_Sync(void)
{//not support in current stage.
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
