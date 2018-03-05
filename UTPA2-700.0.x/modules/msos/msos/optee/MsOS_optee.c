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
#include <stdio.h>
#include <stdlib.h>
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
//#include "hal_dcache.h"
#include "halMPool.h"
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
typedef signed char         int8_t;
typedef unsigned char       uint8_t;

typedef signed short        int16_t;
typedef unsigned short      uint16_t;

typedef signed int          int32_t;
typedef unsigned int        uint32_t;

typedef signed long long    int64_t;
typedef unsigned long long  uint64_t;

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

#define TIMER_ABSTIME      1

#define CLOCK_REALTIME     0

#ifdef CONFIG_RTC
#  define CLOCK_ACTIVETIME 1
#else
#  define CLOCK_ACTIVETIME CLOCK_REALTIME
#endif




//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

typedef enum {
	DCACHE_CLEAN = 0x1,
	DCACHE_AREA_CLEAN = 0x2,
	DCACHE_INVALIDATE = 0x3,
	DCACHE_AREA_INVALIDATE = 0x4,
	ICACHE_INVALIDATE = 0x5,
	ICACHE_AREA_INVALIDATE = 0x6,
	WRITE_BUFFER_DRAIN = 0x7,
	DCACHE_CLEAN_INV = 0x8,
	DCACHE_AREA_CLEAN_INV = 0x9,
	L2CACHE_INVALIDATE = 0xA,
	L2CACHE_AREA_INVALIDATE = 0xB,
	L2CACHE_CLEAN = 0xC,
	L2CACHE_AREA_CLEAN = 0xD,
	L2CACHE_CLEAN_INV = 0xE,
	L2CACHE_AREA_CLEAN_INV = 0xF
} t_cache_operation_id;

#define paddr_t MS_PHY
extern unsigned int cache_maintenance_l1(int op, void *va, size_t len);
extern unsigned int cache_maintenance_l2(int op, paddr_t pa, size_t len);
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
// Mutex
//
enum mutex_value {
	MUTEX_VALUE_UNLOCKED,
	MUTEX_VALUE_LOCKED,
};

struct mutex {
	enum mutex_value value;
	size_t num_waiters;	/* > 0 means that some thread(s) are waiting */
	unsigned spin_lock;	/* used when operating on this struct */
	int handle;		/* identifier of mutex passed to linux driver */
	uint32_t tick;		/* how recent the operation passed to */
				/* linux driver is */
};
typedef struct mutex Optee_mutex;


typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bShared;
    Optee_mutex         stMutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;

static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static Optee_mutex			    _MsOS_Mutex_Mutex;
#define OPTEE_MUTEX_LOCK()		mutex_lock(&_MsOS_Mutex_Mutex)
#define OPTEE_MUTEX_UNLOCK()	        mutex_unlock(&_MsOS_Mutex_Mutex)


//
// Semaphore
//
//typedef int pthread_condattr_t;
/**
struct Semaphore
{
  uint32_t total_semcount;            //   >0 -> Num counts available
                              //   <0 -> Num tasks waiting for semaphore
  Optee_mutex Sem_mutex;

  uint32_t current_count;

};
typedef struct Semaphore Optee_semaphore;

Optee_semaphore Semaphoreformutex[MSOS_SEMAPHORE_MAX];


typedef struct
{
    MS_BOOL                     bUsed;
    Optee_semaphore             stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info              _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];
static Optee_mutex			_MsOS_Semaphore_Mutex;

//#define OPTEE_Semaphore_LOCK()		Semaphore_LOCK(&_MsOS_Semaphore_Mutex)
//#define OPTEE_Semaphore_UNLOCK()	        Semaphore_UNLOCK(&_MsOS_Semaphore_Mutex)
*/
//
// Queue
//

//
// Timer
//
typedef struct {
	uint32_t seconds;
	uint32_t millis;
} TEE_Time;

#define TEE_Result MS_U32
extern TEE_Result tee_time_get_sys_time(TEE_Time *time);
extern void tee_time_wait(uint32_t milliseconds_delay);
extern void tee_time_wait_mstar(uint32_t usseconds_delay) __attribute__((weak));


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


/* The following routines create, delete, lock and unlock mutexes. */
extern void mutex_init(Optee_mutex *mutex);
extern void mutex_lock(Optee_mutex *mutex);
extern void mutex_unlock(Optee_mutex *mutex);
extern MS_BOOL mutex_trylock(Optee_mutex *mutex);
extern void mutex_destroy(Optee_mutex *mutex);


/* Operations on condition variables */


static MSIF_Version _drv_msos_version = {
    .DDI = { MSOS_DRV_VERSION },
};

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//#define MSOS_PRINT(fmt, args...)    PRINTF("[MSOS_PRINT][%06d]     " fmt, __LINE__, ## args)
#define MSOS_PRINT(fmt, args...)

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

// Macros optee
#define TEE_SUCCESS                       0x00000000
extern int core_va2pa_helper(void *va, paddr_t *pa);
/* Special macro to avoid breaking strict aliasing rules */
#ifdef __GNUC__
#define core_va2pa(va, pa) (__extension__ ({ \
	paddr_t _p; \
	int _res = core_va2pa_helper((va), &_p); \
	if (!_res) \
		*(pa) = _p; \
	_res; \
	}))
#else
#define core_va2pa(pa, va) \
		core_va2pa_helper((pa), (va))
#endif

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

void msos_strcpy(char* strDest,const char* strSrc)
{
	if((strDest == NULL) || (strSrc == NULL))
	{
		printf("\033[35mFunction = %s, Line = %d, msos_strcpy ERROR, strDest is NULL or strSrc is NULL\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return;
	}
	while((*strDest++ = *strSrc++) != '\0');
}
void msos_strncpy (char* dest, const char* source, int size_d)
{
    if((dest == NULL) || (source == NULL))
        return ;

    int len = 0 ;
    while (*source != '\0')
    {
	dest[len] = *source ;
	len ++ ;
	if (len >= size_d-1)
		break ;
            source ++ ;
    }
	dest[len] = 0 ;
}

//-------------------------------------------------------------------------------------------------
/// Initialize MsOS
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Init (void)
{
   MS_U32 u32I;
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }
	mutex_init(&_MsOS_Mutex_Mutex);

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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
   // PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
#if 0
	/* optee does not support this */
	task_create(pTaskName, eTaskPriority, CONFIG_USERMAIN_STACKSIZE
			, (main_t)pTaskEntry, (const char*)u32TaskEntryData);
#endif

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Delete a previously created task
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
	if(tee_time_wait_mstar) tee_time_wait_mstar(u32Ms * 1000);
	else tee_time_wait(u32Ms);
}

//-------------------------------------------------------------------------------------------------
/// Delay for u32Us microseconds
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs (MS_U32 u32Us)
{
	if(tee_time_wait_mstar) tee_time_wait_mstar(u32Us);
	else tee_time_wait(u32Us/1000);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get all task ID
/// @param  ps32TaskIdList   \b IN/OUT: the memory for the all task ID
/// @return number of task created
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_InfoTaskList (MS_S32* ps32TaskIdList)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0;
}


//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
}
//-------------------------------------------------------------------------------------------------
/// Get thread ID of current thread/process in Linux(Kernel)
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MS_S32 s32Prop = (MSOS_PROCESS_PRIVATE == u32Flag)? PTHREAD_PROCESS_PRIVATE: PTHREAD_PROCESS_SHARED;
    MS_U8 pMutexName[MAX_MUTEX_NAME_LENGTH];
    MS_U32 u32MaxLen;

    if (NULL == pMutexName1)
    {
        return -1L;
    }
    if (strlen(pMutexName1) >= (MAX_MUTEX_NAME_LENGTH-1))
    {
        printf("%s: Warning strlen(%s) is longer than MAX_MUTEX_NAME_LENGTH(%d). Oversize char will be discard.\n",
        __FUNCTION__,pMutexName1,MAX_MUTEX_NAME_LENGTH);
    }
    if (0 == (u32MaxLen = MIN(strlen(pMutexName1), (MAX_MUTEX_NAME_LENGTH-1))))
    {
        return -1L;
    }
    msos_strncpy((char*)pMutexName, (const char*)pMutexName1, u32MaxLen);
    pMutexName[u32MaxLen] = '\0';

    OPTEE_MUTEX_LOCK();
    for (s32Id=0;s32Id<MSOS_MUTEX_MAX;s32Id++)
    {

        if(TRUE == _MsOS_Mutex_Info[s32Id].bUsed)
        {
            if (0== strcmp((const char*)_MsOS_Mutex_Info[s32Id].u8Name, (const char*)pMutexName))
            {
                break;
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
        msos_strncpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName,MIN(strlen((const char*)pMutexName),(MAX_MUTEX_NAME_LENGTH-1)));
        mutex_init(&_MsOS_Mutex_Info[s32LstUnused].stMutex);
        s32Id = s32LstUnused;
    }
    OPTEE_MUTEX_UNLOCK();

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

    if (s32MutexId >= MSOS_MUTEX_MAX)
	return FALSE;

    OPTEE_MUTEX_LOCK();
    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);
    mutex_destroy(&_MsOS_Mutex_Info[s32MutexId].stMutex);
    _MsOS_Mutex_Info[s32MutexId].bUsed = FALSE;
    memset(_MsOS_Mutex_Info[s32MutexId].u8Name,0x00,sizeof(_MsOS_Mutex_Info[s32MutexId].u8Name));
    OPTEE_MUTEX_UNLOCK();

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

    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if (s32MutexId >= MSOS_MUTEX_MAX)
	return FALSE;

    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        mutex_lock(&(_MsOS_Mutex_Info[s32MutexId].stMutex));
        bRet = TRUE;

    }
    else if (u32WaitMs==0) //non-blocking
    {

        mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex);
        bRet = TRUE;
    }
    else //blocking wait with timeout
    {
       printf( "not support\n");
       bRet = FALSE;
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
     MS_BOOL bRet = TRUE;
    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if (s32MutexId >= MSOS_MUTEX_MAX)
	return FALSE;

    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);
    mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].stMutex);

    return bRet;
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
        msos_strncpy(pMutexName, (const char*)_MsOS_Mutex_Info[s32MutexId].u8Name,MIN(strlen((const char*)_MsOS_Mutex_Info[s32MutexId].u8Name),(MAX_MUTEX_NAME_LENGTH-1)));
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

    return MsOS_CreateMutex(E_MSOS_FIFO, pSemaphoreName, MSOS_PROCESS_SHARED);
/*
    MS_S32 s32Id;
    MS_BOOL result=0;

    mutex_lock(&_MsOS_Semaphore_Mutex);
    for(s32Id=0;s32Id<MSOS_SEMAPHORE_MAX;s32Id++)
    {
        if(_MsOS_Semaphore_Info[s32Id].bUsed == FALSE)
        {
            _MsOS_Semaphore_Info[s32Id].bUsed = TRUE;
            break;
        }
    }

    if(s32Id >= MSOS_SEMAPHORE_MAX)
    {
    	mutex_unlock(&_MsOS_Semaphore_Mutex);
        return -1;
    }
    mutex_init(&Semaphoreformutex[s32Id].Sem_mutex);
    Semaphoreformutex[s32Id].current_count=u32InitCnt;
    Semaphoreformutex[s32Id].total_semcount=u32InitCnt;
    s32Id |= MSOS_ID_PREFIX;
    mutex_unlock(&_MsOS_Semaphore_Mutex);
    return s32Id;
*/
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
    return MsOS_DeleteSemaphore(s32SemaphoreId);
/*
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }
    while (Semaphoreformutex[s32SemaphoreId].current_count != Semaphoreformutex[s32SemaphoreId].total_semcount)
    {
	if (Semaphoreformutex[s32SemaphoreId].current_count < Semaphoreformutex[s32SemaphoreId].total_semcount)
	{
	    MsOS_ReleaseSemaphore( (s32SemaphoreId|s32SemaphoreId));
	}
    }
    mutex_lock(&_MsOS_Semaphore_Mutex);
    _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
    mutex_unlock(&_MsOS_Semaphore_Mutex);
    return TRUE;
*/
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
    return MsOS_ObtainMutex(s32SemaphoreId,u32WaitMs);
/*
    u32WaitMs=MSOS_WAIT_FOREVER;

    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    mutex_lock(&_MsOS_Semaphore_Mutex);

    if (_MsOS_Semaphore_Info[s32SemaphoreId].bUsed == FALSE)
    {
	mutex_unlock(&_MsOS_Semaphore_Mutex);
	return FALSE;
    }

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
    	Semaphoreformutex[s32SemaphoreId].current_count--;
	if(Semaphoreformutex[s32SemaphoreId].current_count<=0)
	{
	     mutex_lock(&Semaphoreformutex[s32SemaphoreId].Sem_mutex);
	}
    }
    mutex_unlock(&_MsOS_Semaphore_Mutex);
	return TRUE;
	*/
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
    return MsOS_ReleaseMutex(s32SemaphoreId);
/*
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }
	mutex_lock(&_MsOS_Semaphore_Mutex);

   if (_MsOS_Semaphore_Info[s32SemaphoreId].bUsed == FALSE)
    {
	mutex_unlock(&_MsOS_Semaphore_Mutex);
	return FALSE;
    }

    if((Semaphoreformutex[s32SemaphoreId].current_count)<=0)
    {
	mutex_unlock(&Semaphoreformutex[s32SemaphoreId].Sem_mutex);
    }
	Semaphoreformutex[s32SemaphoreId].current_count++;
	mutex_unlock(&_MsOS_Semaphore_Mutex);

    return TRUE;
    */
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
  //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
{    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    TEE_Time current;
    MS_U32 system_timer=0;
    system_timer=tee_time_get_sys_time(&current);
    return system_timer;
}
//-------------------------------------------------------------------------------------------------
///[OBSOLETE]
/// Time difference between current time and task time
/// @return system time diff in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer) //unit = ms
{
    //return (MsOS_GetSystemTime() - u32TaskTimer);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0x12345678;
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MS_U32 ret=0;
    ret = cache_maintenance_l1(DCACHE_AREA_CLEAN, u32Start, u32Size);
    if (ret != TEE_SUCCESS)
    {
        printf( "Optee Clean L1 error, OPTEE Result nummber=%d\n",ret);
    }
#ifdef CFG_PL310
    ret = cache_maintenance_l2(L2CACHE_AREA_CLEAN_INV, core_va2pa(u32Start), u32Size);
    if (ret != TEE_SUCCESS)
    {
        printf( "Optee Flush L2 error, OPTEE Result nummber=%d\n",ret);
    }
#endif /*CFG_PL310*/
    ret = cache_maintenance_l1(DCACHE_AREA_CLEAN_INV,u32Start, u32Size);
    if (ret != TEE_SUCCESS)
    {
        printf( "Optee Flush L1 error, OPTEE Result nummber=%d\n",ret);
    }
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
#if 0
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
#else
    MS_U32 ret=0;
#ifdef CFG_PL310
    ret = cache_maintenance_l2(L2CACHE_AREA_INVALIDATE, core_va2pa(u32Start), u32Size);
    if (ret != TEE_SUCCESS)
    {
        printf( "Optee Invalidate L2 error, OPTEE Result nummber=%d\n",ret);
    }
#endif /*CFG_PL310*/
    ret = cache_maintenance_l1(DCACHE_AREA_INVALIDATE,u32Start, u32Size);
    if (ret != TEE_SUCCESS)
    {
        printf( "Optee Invalidate L1 error, OPTEE Result nummber=%d\n",ret);
    }
    return TRUE;
#endif
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
		msos_strcpy(shm_array.shm_entry[shm_array.count].name, (char*)pu8ClientName);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
	//up_enable_irq((int) intr_num);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
   	// irq_attach(intr_num, (xcpt_t) isr);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    //PRINTF("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    HAL_MMIO_FlushMemory();
}

//-------------------------------------------------------------------------------------------------
/// Flush non-cached data into dram
//-------------------------------------------------------------------------------------------------
void MsOS_ReadMemory(void)
{
    HAL_MMIO_ReadMemory();
}

MS_U32 MsOS_GetKattribute(char *pAttr)
{
    // This is for linux only, do nothing here
    return 0;
}
