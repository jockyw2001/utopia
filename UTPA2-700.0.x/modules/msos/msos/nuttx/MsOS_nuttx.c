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
/// file    MsOS_nuttx.c
/// @brief  MStar OS Wrapper
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <unistd.h>
#include "MsCommon.h"
#include "MsOS.h"
#include "debug.h"
#include "asmCPU.h"
#include "drvIRQ.h"
#include "string.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"
#ifdef CONFIG_UTOPIA_FRAMEWORK
#include "utopia.h"
#endif
#include "hal_dcache.h"
#include "halMPool.h"
//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
typedef signed char        _int8_t;
typedef unsigned char      _uint8_t;

typedef signed short       _int16_t;
typedef unsigned short     _uint16_t;

typedef signed int         _int32_t;
typedef unsigned int       _uint32_t;

typedef signed long long   _int64_t;
typedef unsigned long long _uint64_t;

struct shm_entry {
	char name[50]; // FIXME: potential bug
	void* addr;
	unsigned int size;
};

#define SHM_ARRAY_LENGTH 200
struct shm_array {
	unsigned int count;
	struct shm_entry shm_entry[SHM_ARRAY_LENGTH];
};

struct shm_array shm_array;

/* Values for the process shared (pshared) attribute */

#define PTHREAD_PROCESS_PRIVATE       0
#define PTHREAD_PROCESS_SHARED        1

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK

/* CLOCK_REALTIME refers to the standard time source.  For most implementations,
 * the standard time source is the system timer interrupt.  However, if the
 * platform supports an RTC, then the standard time source will be the RTC
 * for the clock_gettime() and clock_settime() interfaces (the system timer
 * is still the time source for all of the interfaces).
 */

#define CLOCK_REALTIME     0

/* If an RTC is supported, then the non-standard CLOCK_ACTIVETIME is also
 * supported to manage time based on the system timer interrupt separately from
 * the RTC.  This may be necessary, for example, in certain cases where the
 * system timer interrupt has been stopped in low power modes.
 *
 * CLOCK_ACTIVETIME is only recognized by clock_gettime() and clock_settime().
 */

#ifdef CONFIG_RTC
#  define CLOCK_ACTIVETIME 1
#else
#  define CLOCK_ACTIVETIME CLOCK_REALTIME
#endif

/* This is a flag that may be passed to the timer_settime() function */

#define TIMER_ABSTIME      1


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_U32 g_system_timer;
extern void Hal_Dcache_Flush(unsigned int base, unsigned int size);
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//
// Variable-size Memory pool
//
//

//
// Task Management
//

//
// Semaphore
//
typedef int pthread_condattr_t;

struct sem_s
{
  _int16_t semcount;              /* >0 -> Num counts available */
                                /* <0 -> Num tasks waiting for semaphore */
#ifdef CONFIG_PRIORITY_INHERITANCE
  struct semholder_s hlist;     /* List of holders of semaphore counts */
#endif
};
typedef struct sem_s sem_t;

typedef struct
{
    MS_BOOL                     bUsed;
    sem_t                       stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info      _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];


//
// Mutex
//
struct pthread_mutexattr_s
{
  _uint8_t pshared;  /* PTHREAD_PROCESS_PRIVATE or PTHREAD_PROCESS_SHARED */
#ifdef CONFIG_MUTEX_TYPES
  _uint8_t type;	  /* Type of the mutex.  See PTHREAD_MUTEX_* definitions */
#endif
};

typedef struct pthread_mutexattr_s pthread_mutexattr_t;

struct pthread_mutex_s
{
  int   pid;      /* ID of the holder of the mutex */
  sem_t sem;      /* Semaphore underlying the implementation of the mutex */
#ifdef CONFIG_MUTEX_TYPES
  _uint8_t type;   /* Type of the mutex.  See PTHREAD_MUTEX_* definitions */
  int   nlocks;   /* The number of recursive locks held */
#endif
};

typedef struct pthread_mutex_s pthread_mutex_t;

typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bShared;
    pthread_mutex_t     stMutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;

static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static pthread_mutex_t			_MsOS_Mutex_Mutex;
#define MUTEX_MUTEX_LOCK()		pthread_mutex_lock(&_MsOS_Mutex_Mutex)
#define MUTEX_MUTEX_UNLOCK()	pthread_mutex_unlock(&_MsOS_Mutex_Mutex)

static pthread_mutex_t              _MsOS_Semaphore_Mutex;
#define MUTEX_SEMEPHORE_LOCK()		pthread_mutex_lock(&_MsOS_Semaphore_Mutex)
#define MUTEX_SEMEPHORE_UNLOCK()	pthread_mutex_unlock(&_MsOS_Semaphore_Mutex)


//
// Event Group
//
struct pthread_cond_s
{
  sem_t sem;
};

typedef struct pthread_cond_s pthread_cond_t;

typedef struct
{
    MS_BOOL                     bUsed;
    MS_U32                      u32EventGroup;
    pthread_mutex_t             stMutexEvent;
    pthread_cond_t              stSemaphore;
    pthread_mutex_t             stMutex;
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info     _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static pthread_mutex_t          _MsOS_EventGroup_Mutex;

//
// Queue
//

//
// Timer
//

extern int clock_gettime(clockid_t clockid, struct timespec *tp);

//
// Interrupt
//
extern MS_BOOL up_interrupt_context(void);

//-------------------------------------------------------------------------------------------------
// Externel Function Prototypes
//-------------------------------------------------------------------------------------------------
typedef int (*xcpt_t)(int irq, void *context);
extern int hal_irq_attach(int irq, xcpt_t isr);
extern int hal_irq_detach(int irq);
extern void chip_irq_mask(unsigned irq);
extern void chip_irq_unmask(unsigned irq);


/* Create, operate on, and destroy mutex attributes. */
extern int pthread_mutexattr_init(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
extern int pthread_mutexattr_getpshared(pthread_mutexattr_t *attr, int *pshared);
extern int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);


/* The following routines create, delete, lock and unlock mutexes. */
extern int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr);
extern int pthread_mutex_destroy(pthread_mutex_t *mutex);
extern int pthread_mutex_lock(pthread_mutex_t *mutex);
extern int pthread_mutex_trylock(pthread_mutex_t *mutex);
extern int pthread_mutex_unlock(pthread_mutex_t *mutex);

/* Operations on condition variables */

extern int pthread_condattr_init(pthread_condattr_t *attr);
extern int pthread_condattr_destroy(pthread_condattr_t *attr);

/* A thread can create and delete condition variables. */

extern int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
extern int pthread_cond_destroy(pthread_cond_t *cond);

/* A thread can signal to and broadcast on a condition variable. */

extern int pthread_cond_broadcast(pthread_cond_t *cond);
extern int pthread_cond_signal(pthread_cond_t *cond);

/* A thread can wait for a condition variable to be signalled or broadcast. */

extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/* A thread can perform a timed wait on a condition variable. */

extern int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                                  const struct timespec *abstime);

/* Counting Semaphore Interfaces (based on POSIX APIs) */
extern int        sem_init(sem_t *sem, int pshared, unsigned int value);
extern int        sem_destroy(sem_t *sem);
extern sem_t *    sem_open(const char *name, int oflag, ...);
extern int        sem_close(sem_t *sem);
extern int        sem_unlink(const char *name);
extern int        sem_wait(sem_t *sem);
extern int        sem_trywait(sem_t *sem);
extern int        sem_post(sem_t *sem);
extern int        sem_getvalue(sem_t *sem, int *sval);

/* To avoid nuttx crash in interrupt mode, replace printf with lib_lowprintf. */
extern int lib_lowprintf(const char *fmt, ...);
#define PRINTF lib_lowprintf

static MSIF_Version _drv_msos_version = {
    .DDI = { MSOS_DRV_VERSION },
};

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MSOS_PRINT(fmt, args...)    PRINTF("[MSOS_PRINT][%06d]     " fmt, __LINE__, ## args)

#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))

#define _TimeAbs(stTime, u32IntMs)                                                              \
{                                                                                               \
    clock_gettime(CLOCK_REALTIME, &(stTime));                                                   \
    if(u32IntMs>0)                                                                              \
    {                                                                                           \
        MS_U32                         u32Sec=(u32IntMs)/1000;                                  \
        (stTime).tv_sec+=           u32Sec;                                                     \
        (stTime).tv_nsec+=          ((u32IntMs)- (1000* u32Sec))* 1000000;                      \
        if ((stTime).tv_nsec>= 1000000000)                                                      \
        {                                                                                       \
            (stTime).tv_sec++;                                                                  \
            (stTime).tv_nsec-=      1000000000;                                                 \
        }                                                                                       \
    }                                                                                           \
}

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
	pthread_mutexattr_t _MsOS_Mutex_Attr;

    //
    //mutex
    //
	pthread_mutexattr_init(&_MsOS_Mutex_Attr);
	pthread_mutex_init(&_MsOS_Mutex_Mutex, &_MsOS_Mutex_Attr);
	for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }

    //
    // Semaphore
    //
    pthread_mutex_init(&_MsOS_Semaphore_Mutex, &_MsOS_Mutex_Attr);
    for( u32I=0; u32I<MSOS_SEMAPHORE_MAX; u32I++)
    {
        _MsOS_Semaphore_Info[u32I].bUsed = FALSE;
    }

	MDrv_IRQ_Init();

    //
    // Event Group
    //
    pthread_mutex_init(&_MsOS_EventGroup_Mutex, &_MsOS_Mutex_Attr);

    for( u32I=0; u32I<MSOS_EVENTGROUP_MAX; u32I++)
    {
        _MsOS_EventGroup_Info[u32I].bUsed = FALSE;
    }

#ifdef CONFIG_UTOPIA_FRAMEWORK
	UtopiaInit();
#endif

    return true;
}

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
MS_S32 MsOS_CreateMemoryPool (MS_U32   u32PoolSize,
                           MS_U32   u32MinAllocation,
                           void     *pPoolAddr,
                           MsOSAttribute eAttribute,
                           char     *pPoolName)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
}

//-------------------------------------------------------------------------------------------------
/// Delete a variable-size memory pool
/// @param  s32PoolId   \b IN: pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteMemoryPool (MS_S32 s32PoolId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
MS_BOOL MsOS_InfoMemoryPool (MS_S32    s32PoolId,
                          void      **pPoolAddr,
                          MS_U32    *pu32PoolSize,
                          MS_U32    *pu32FreeSize,
                          MS_U32    *pu32LargestFreeBlockSize)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return NULL;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}


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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return NULL;
}

//-------------------------------------------------------------------------------------------------
/// Create a fixed-size memory pool dynamically
/// @param  u32PoolSize         \b IN: pool size in bytes
/// @param  u32BlockSize        \b IN: fixed block size for each allocated block in the pool
/// @param  pPoolAddr           \b IN: starting address for the memory pool
/// @param  eAttribute          \b IN: only E_MSOS_FIFO - suspended in FIFO order
/// @param  pPoolName           \b IN: not used
/// @return >=0 : assigned memory pool ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateFixSizeMemoryPool (MS_U32    u32PoolSize,
                                  MS_U32    u32BlockSize,
                                  void      *pPoolAddr,
                                  MsOSAttribute eAttribute,
                                  char      *pPoolName)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
}

//-------------------------------------------------------------------------------------------------
/// Delete a fixed-size memory pool
/// @param  s32PoolId   \b IN: pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteFixSizeMemoryPool (MS_S32 s32PoolId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get the information of a fixed-size memory pool
/// @param  s32PoolId   \b IN: memory pool ID
/// @param  pPoolAddr   \b OUT: holding the starting address for the memory pool
/// @param  pu32PoolSize \b OUT: holding the total size of the memory pool
/// @param  pu32FreeSize \b OUT: holding the available free size of the memory pool
/// @param  pu32LargestFreeBlockSize  \b OUT: holding the size of the largest free block
/// @return TRUE : succeed
/// @return FALSE : the pool has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoFixSizeMemoryPool (MS_S32 s32PoolId,
                                 void   **pPoolAddr,
                                 MS_U32 *pu32PoolSize,
                                 MS_U32 *pu32FreeSize,
                                 MS_U32 *pu32LargestFreeBlockSize)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Allocate a memory block from the fixed-size memory pool
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory
/// @return Otherwise : pointer to the allocated memory block
/// @note   watch out for alignment if u32BlockSize is not a multiple of 4
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateFixSizeMemory (MS_S32 s32PoolId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return NULL;
}

//-------------------------------------------------------------------------------------------------
/// Free a memory block from the fixed-size memory pool
/// @param  pAddress    \b IN: pointer to previously allocated memory block
/// @param  s32PoolId   \b IN: memory pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_FreeFixSizeMemory (void *pAddress, MS_S32 s32PoolId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}


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
#define CONFIG_USERMAIN_STACKSIZE (0x40000)
typedef int (*main_t)(int argc, char *argv[]);
extern int task_create(const char *name, int priority,
		int stack_size, main_t entry, const char *argv);

MS_S32 MsOS_CreateTask (TaskEntry  pTaskEntry,
                     MS_VIRT     u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL    bAutoStart,
                     void       *pStackBase,
                     MS_U32     u32StackSize,
                     char *pTaskName)
{
	return task_create(pTaskName, eTaskPriority, CONFIG_USERMAIN_STACKSIZE
			, (main_t)pTaskEntry, (const char*)u32TaskEntryData);
}

//-------------------------------------------------------------------------------------------------
/// Delete a previously created task
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
	usleep(u32Ms*1000);
}

//-------------------------------------------------------------------------------------------------
/// Delay for u32Us microseconds
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs (MS_U32 u32Us)
{
    MAsm_CPU_DelayUs(u32Us);
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
/// Resume the specified suspended task
/// @param  s32TaskId   \b IN: Task ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note   This API is not supported in Linux
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ResumeTask (MS_S32 s32TaskId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get a task information
/// @param  s32TaskId       \b IN: task ID
/// @param  peTaskPriority  \b OUT: ptr to task priority
/// @param  pStackBase      \b OUT: ptr to stack base
/// @param  pu32StackSize   \b OUT: ptr to stack size
/// @param  pTaskName       \b OUT: ptr to task name
/// @return TRUE : succeed
/// @return FALSE : the task has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoTask (MS_S32 s32TaskId,
                       TaskPriority *peTaskPriority,
                       void* pStackBase,
                       MS_U32* pu32StackSize,
                       MS_U32* pu32StackUsed,
                       char *pTaskName)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get all task ID
/// @param  ps32TaskIdList   \b IN/OUT: the memory for the all task ID
/// @return number of task created
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_InfoTaskList (MS_S32* ps32TaskIdList)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0;
}


//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
}
//-------------------------------------------------------------------------------------------------
/// Get thread ID of current thread/process in Linux(Kernel)
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return MsOS_InfoTaskID();
}


//
// Mutex
//
//-------------------------------------------------------------------------------------------------
/// Create a mutex in the unlocked state
/// @param  eAttribute  \b IN: E_MSOS_FIFO: suspended in FIFO order
/// @param  pMutexName  \b IN: mutex name
/// @return >=0 : assigned mutex Id
/// @return <0 : fail
/// @note   A mutex has the concept of an owner, whereas a semaphore does not.
///         A mutex provides priority inheritance protocol against proiorty inversion, whereas a binary semaphore does not.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName1, MS_U32 u32Flag)
{
    MS_S32 s32Id, s32LstUnused = MSOS_MUTEX_MAX;
    pthread_mutexattr_t _MsOS_Mutex_Attr;
    int s32Prop = (MSOS_PROCESS_PRIVATE == u32Flag)? PTHREAD_PROCESS_PRIVATE: PTHREAD_PROCESS_SHARED;
    MS_U8 pMutexName[MAX_MUTEX_NAME_LENGTH];
    MS_U32 u32MaxLen;

    if (NULL == pMutexName1)
    {
        return -1L;
    }
    if (strlen(pMutexName1) >= (MAX_MUTEX_NAME_LENGTH-1))
    {
        MSOS_PRINT("%s: Warning strlen(%s) is longer than MAX_MUTEX_NAME_LENGTH(%d). Oversize char will be discard.\n",
        __FUNCTION__,pMutexName1,MAX_MUTEX_NAME_LENGTH);
    }
    if (0 == (u32MaxLen = MIN(strlen(pMutexName1), (MAX_MUTEX_NAME_LENGTH-1))))
    {
        return -1L;
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
        if (FALSE==_MsOS_Mutex_Info[s32Id].bUsed  && MSOS_MUTEX_MAX==s32LstUnused)
        {
            s32LstUnused = s32Id;
        }
    }
	
    if ((MSOS_MUTEX_MAX==s32Id) && (MSOS_MUTEX_MAX>s32LstUnused))
    {

        pthread_mutexattr_init(&_MsOS_Mutex_Attr);
        pthread_mutexattr_setpshared(&_MsOS_Mutex_Attr, s32Prop);
       //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
     #ifdef MS_DEBUG
        pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK);
     #endif
        _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
        strncpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName,MIN(strlen((const char*)pMutexName),(MAX_MUTEX_NAME_LENGTH-1)));
        pthread_mutex_init(&_MsOS_Mutex_Info[s32LstUnused].stMutex, &_MsOS_Mutex_Attr);
        s32Id = s32LstUnused;

    }
	
    MUTEX_MUTEX_UNLOCK();
	
    if(MSOS_MUTEX_MAX <= s32Id)
    {
        return -1L;
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

   
    if( (_MsOS_Mutex_Info[s32MutexId].bUsed == FALSE))
    {
        MSOS_PRINT("MUTEX WITH MUTEX_ID:0x%lx NOT EXIST\n",s32MutexId );
        return FALSE;
    }
   

    MUTEX_MUTEX_LOCK();

    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);
    pthread_mutex_destroy(&_MsOS_Mutex_Info[s32MutexId].stMutex);
    _MsOS_Mutex_Info[s32MutexId].bUsed = FALSE;
    memset(_MsOS_Mutex_Info[s32MutexId].u8Name,0x00,sizeof(_MsOS_Mutex_Info[s32MutexId].u8Name));

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
MS_BOOL MsOS_ObtainMutex (MS_S32 s32MutexId, MS_U32 u32WaitMs)
{
    MS_BOOL bRet = FALSE;

	u32WaitMs=MSOS_WAIT_FOREVER;
	
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
        if(pthread_mutex_lock(&(_MsOS_Mutex_Info[s32MutexId].stMutex)))
        {
            MSOS_PRINT("Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
			bRet = FALSE;
        }
		else
        {
            bRet = TRUE;
        }
    }
    else
    {
        if (pthread_mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex))
        {
            MSOS_PRINT("Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
            bRet = FALSE;
        }
		else
        {
            bRet = TRUE;
        }
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

    if(pthread_mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].stMutex))
    {
        MSOS_PRINT("Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
		return FALSE;
    }
	
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
        strncpy(pMutexName, (const char*)_MsOS_Mutex_Info[s32MutexId].u8Name,MIN(strlen((const char*)_MsOS_Mutex_Info[s32MutexId].u8Name),(MAX_MUTEX_NAME_LENGTH-1)));
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

    MUTEX_SEMEPHORE_LOCK();
	
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
	
    MUTEX_SEMEPHORE_UNLOCK();

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

    if(_MsOS_Semaphore_Info[s32SemaphoreId].bUsed == FALSE )
    {
        MSOS_PRINT("SEMAPHORE WITH SEMAPHORE_ID:0x%lx NOT EXIST\n",s32SemaphoreId );
        return FALSE;
    }

    sem_destroy(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);

    MUTEX_SEMEPHORE_LOCK();
    _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
    MUTEX_SEMEPHORE_UNLOCK();
    return TRUE;

    return FALSE;
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
	
    u32WaitMs=MSOS_WAIT_FOREVER;
		
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

    return sem_getvalue(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore,(int *)pu32InitCnt) >= 0 ? TRUE : FALSE;
}


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
    pthread_mutexattr_t _MsOS_Mutex_Attr;

    pthread_mutexattr_init(&_MsOS_Mutex_Attr);
#ifdef MS_DEBUG
    pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK);
#endif

    pthread_mutex_lock(&_MsOS_EventGroup_Mutex);
    for(s32Id=0; s32Id<MSOS_EVENTGROUP_MAX; s32Id++)
    {
        if(_MsOS_EventGroup_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }

    if(s32Id < MSOS_EVENTGROUP_MAX)
    {
        pthread_mutex_init(&_MsOS_EventGroup_Info[s32Id].stMutexEvent, &_MsOS_Mutex_Attr);
        pthread_mutex_lock(&_MsOS_EventGroup_Info[s32Id].stMutexEvent);
        _MsOS_EventGroup_Info[s32Id].bUsed = TRUE;
        _MsOS_EventGroup_Info[s32Id].u32EventGroup= 0;
        pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32Id].stMutexEvent);
    }
    pthread_mutex_unlock(&_MsOS_EventGroup_Mutex);

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }

    pthread_mutex_init(&_MsOS_EventGroup_Info[s32Id].stMutex, &_MsOS_Mutex_Attr);
    pthread_cond_init(&_MsOS_EventGroup_Info[s32Id].stSemaphore, NULL);

    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
    return -1;
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
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    #if 1
    if( _MsOS_EventGroup_Info[s32EventGroupId].bUsed == FALSE)
    {
        MSOS_PRINT("EVENTGROUP WITH EVENTGROUP:0x%lx NOT EXIST\n",s32EventGroupId);
        return FALSE;
    }
    #endif

    pthread_cond_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
    pthread_mutex_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stMutex);
    pthread_mutex_lock(&_MsOS_EventGroup_Mutex);
    pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup= 0;
    pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
    pthread_mutex_unlock(&_MsOS_EventGroup_Mutex);
    pthread_mutex_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
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
        
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }
    
    if(up_interrupt_context())
    {
        SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
        pthread_cond_broadcast(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
    }
    else
    {
        MDrv_IRQ_MaskAll();
        pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
        SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
        pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
        pthread_cond_broadcast(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore);
        MDrv_IRQ_UnMaskAll();
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Clear the specified event flag (bitwise XOR operation) in the specified event group
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @returnTRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ClearEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    if(up_interrupt_context())
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    }
    else
    {
        MDrv_IRQ_MaskAll();
        pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
        pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
        MDrv_IRQ_UnMaskAll();
    }
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
MS_BOOL MsOS_WaitEvent (MS_S32     s32EventGroupId,
                     MS_U32     u32WaitEventFlag,
                     MS_U32     *pu32RetrievedEventFlag,
                     EventWaitMode eWaitMode,
                     MS_U32     u32WaitMs)
{
    MS_BOOL bRet= FALSE;
    struct timespec StopTime;
    MS_BOOL bAnd;
    MS_BOOL bClear;

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

    if (u32WaitMs!= MSOS_WAIT_FOREVER) //blocking wait
    {
        _TimeAbs(StopTime, u32WaitMs);
    }

    pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutex);
    do{
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
        if ((bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag))
        {
            break;
        }
        if (u32WaitMs== MSOS_WAIT_FOREVER) //blocking wait
        {
            pthread_cond_wait(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                          &_MsOS_EventGroup_Info[s32EventGroupId].stMutex);
        }
        else
        {
            if (pthread_cond_timedwait(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                                   &_MsOS_EventGroup_Info[s32EventGroupId].stMutex,
                                                   &StopTime))
            {
                break;
            }
        }
    } while (1);
   
    pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutex);
    pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    bRet= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (bRet && bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    return bRet;
}

//
// Timer management
//
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
MS_S32 MsOS_CreateTimer (TimerCb   pTimerCb,
                         MS_U32    u32FirstTimeMs,
                         MS_U32    u32PeriodTimeMs,
                         MS_BOOL   bStartTimer,
                         char      *pTimerName)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Delete the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTimer (MS_S32 s32TimerId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Start the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_StartTimer (MS_S32 s32TimerId)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
MS_BOOL MsOS_ResetTimer (MS_S32     s32TimerId,
                         MS_U32     u32FirstTimeMs,
                         MS_U32     u32PeriodTimeMs,
                         MS_BOOL    bStartTimer)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}


//
// System time
//
//-------------------------------------------------------------------------------------------------
/// Get current system time in ms
/// @return system time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTime (void) //   1ms/systime
{
    return g_system_timer;
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
//
// Queue
//
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Get a Queue information
// @param  s32QueueId      \b IN: Queue ID
// @param  pu32QueueSize   \b OUT: ptr to queue size (DW)
// @param  pu32MessageSize \b OUT: ptr to message size (DW)
// @param  peAttribute     \b OUT: ptr to suspended mode: E_MSOS_FIFO / E_MSOS_PRIORITY
// @param  pQueueName      \b OUT: ptr to Queue name
// @return TRUE : succeed
// @return FALSE : the Queue has not been created
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_InfoQueue ( MS_S32 s32QueueId, MS_U32 *pu32QueueSize, MS_U32 *pu32MessageSize,
                      MsOSAttribute *peAttribute, char *pQueueName)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

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
    MDrv_IRQ_Attach(eIntNum, pIntCb);

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
    MDrv_IRQ_Detech(eIntNum);

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
    MDrv_IRQ_UnMask(eIntNum);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable (mask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DisableInterrupt (InterruptNum eIntNum)
{
    MDrv_IRQ_Mask(eIntNum);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_DisableAllInterrupts(void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0x12345678;
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Flush( MS_VIRT u32Start, MS_SIZE u32Size )
{
    Hal_Dcache_Flush(u32Start, u32Size);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Invalidate( MS_VIRT u32Start, MS_SIZE u32Size )
{

    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Writeback( MS_VIRT u32Start, MS_SIZE u32Size )
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

MS_PHY MsOS_VA2PA(MS_VIRT addr)
{
    return HAL_MsOS_MPool_VA2PA(addr);
}

MS_VIRT MsOS_PA2KSEG0(MS_PHY addr)
{
    return HAL_MsOS_MPool_PA2KSEG0(addr);
}

MS_VIRT MsOS_PA2KSEG1(MS_PHY addr)
{
    return HAL_MsOS_MPool_PA2KSEG1(addr);
}


// Share memory operation
MS_BOOL MsOS_SHM_Init(void)
{
	memset(&shm_array, 0, sizeof(shm_array));
    return TRUE;
}

// Share memory operation
// linux behavior simulation for nuttx

MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag)
{
	int i, match = FALSE, result = FALSE;
	
	// search matching entry
	for (i = 0; i < shm_array.count; i++)
	{
		if (0 == strcmp(shm_array.shm_entry[i].name, (char*)pu8ClientName))
		{
			match = TRUE;
			break;
		}
	}

	if (match) {
		// if match, fill info
		*pu32ShmId = i;
		*pu32Addr = (MS_U32)(shm_array.shm_entry[i].addr);
		*pu32BufSize = shm_array.shm_entry[i].size;
		result = TRUE;
	} else if (u32Flag == MSOS_SHM_CREATE) {
		// if not match & specify to create one, register to shm_array
		*pu32ShmId = shm_array.count;
		*pu32Addr = (MS_U32)malloc(u32BufSize);
		memset((void*)*pu32Addr, 0, u32BufSize);
		*pu32BufSize = u32BufSize;
		// this would cause bug, because string source may be "stack variable"
		//shm_array.shm_entry[shm_array.count].name = (char*)pu8ClientName;
		strcpy(shm_array.shm_entry[shm_array.count].name, (char*)pu8ClientName);
		shm_array.shm_entry[shm_array.count].addr = (void*)*pu32Addr;
		shm_array.shm_entry[shm_array.count].size = *pu32BufSize;
		shm_array.count++;
		result = TRUE;
	} 

	return result;
}

MS_BOOL MsOS_SHM_FreeId(MS_U8* pu8ClientName, MS_U32 u32ShmId)
{
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable the CPU interrupt
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_CPU_DisableInterrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
extern void up_enable_irq(int irq);

MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
	up_enable_irq((int) intr_num);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Attach the CPU interrupt callback function to interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Interrupt callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
extern int irq_attach(int irq, xcpt_t isr);

MS_BOOL MsOS_CPU_AttachInterrupt (MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, MS_U32 dat)
{
   	 irq_attach(intr_num, (xcpt_t) isr);
	 return TRUE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Sync data in EC-Brigde
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void MsOS_Sync(void)
{
    MAsm_CPU_Sync();
}

//-------------------------------------------------------------------------------------------------
/// Flush non-cached data into dram
//-------------------------------------------------------------------------------------------------
void MsOS_FlushMemory(void)
{
}

//-------------------------------------------------------------------------------------------------
/// Flush non-cached data into dram
//-------------------------------------------------------------------------------------------------
void MsOS_ReadMemory(void)
{
}

MS_U32 MsOS_GetKattribute(char *pAttr)
{
    // This is for linux only, do nothing here
    return 0;
}
