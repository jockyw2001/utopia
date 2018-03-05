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

/*
 * Note! Functions related to  
 * pthread_mutex_timedlock/sem_timedwait/pthread_cond_timedwait
 * may be disrupted by settimeofday() or other alike APIs.
 *
 * root cause:
 *              your-process                            other-process
 *  1. get "now" in realtime(ex: 5/7)
 *  2. add duration(ex: 1 day -> 5/8)
 *                                          a. modify "now" in realtime(ex: 5/5)
 *  3. set expire time in realtime(ex: 5/8)
 *
 * Well, "now" you have to wait 3 days instead of 1 day.
 * There's nothing we can do unless pthread_mutex_timedlock/sem_timedwait
 * provide monotonic version.
 */

#if defined (MSOS_TYPE_LINUX)

#if defined (ANDROID) && !defined (TV_OS)
#error "Define ANDROID, must define TV_OS"
#endif
#if defined (TV_OS) && defined (MSOS_PROCESS_SAFT_MUTEX)
#error "Can't both define TV_OS and MSOS_PROCESS_SAFT_MUTEX"
#endif

#define _GNU_SOURCE

#if 0
#undef malloc
#undef realloc
#undef free
#endif

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
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
#include <sys/prctl.h>

// Android (bionic libc) do not support key based ipc,
// we must use ashmem (anonymous shared memory) instead

#if defined (ANDROID)
#include <cutils/log.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_DLMALLOC
#define USE_DLMALLOC 1
#else
#define USE_DLMALLOC 0
#endif


#include "MsCommon.h"

#include "mem.h"

#include "MsOS.h"
#include "MsOS_private.h"
#include "asmCPU.h"
//#include "devCHIP.h"
#include "halCHIP.h"
#include "regCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"
#include "halMPool.h"
#if USE_DLMALLOC
#include "dlmalloc.h"
#endif
#include "utopia.h"
#include "ULog.h"

#define MSOS_ERROR(fmt, args...)    ULOGE("MSOS","[%06d]     " fmt, __LINE__, ## args)
#define MSOS_WARN(fmt, args...)    ULOGW("MSOS","[%06d]     " fmt, __LINE__, ## args)
#define MSOS_PRINT(fmt, args...)    ULOGI("MSOS","[%06d]     " fmt, __LINE__, ## args)
#define MSOS_ASSERT(_bool, _f)    if (!(_bool)) { ULOGF("MSOS",_f); MS_ASSERT(0);) }

#if 1 //Enable real assert from linux
#include <assert.h>
#include <unistd.h>
#define LINUX_ASSERT(x)  if (!(x)) {MSOS_WARN("\nAssert in %s,%d\n", __FUNCTION__, __LINE__); MsOS_DelayTask(100); assert(0);}
#endif


//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
// Switch tasks every 1 ms.
//#define UCLIBC 1
#define TASK_TIME_SLICE             (TICK_PER_ONE_MS)

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK



#define CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE (10)
#if !defined (ANDROID)
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

#ifdef UCLIBC
void Timer_Thread(void *data);
typedef struct {
    TimerCb pTimerCb;
    MS_U32 u32FirstTimeMs;
    MS_U32 u32PeriodTimeMs;
    MS_BOOL bStartTimer;
    char *pTimerName;
    MS_BOOL create_ready;
    MS_S32 s32Id;
}TimerSET;
#endif
//Thread name length
#define MAX_NAME  30

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))

/*
 * clock_gettime() base should not be monotonic, 
 * though reatlime isnt excatly suitable.
 * Only pthread_cond_timedwait can benefit from mononic.
 * But to pthread_mutex_timedlock/sem_timedwait,
 * it would be disaster if there's gap between monotonic/realtime
 */

#define _TimeAbsM(stTime, u32IntMs)                                      \
{                                                                       \
    clock_gettime(CLOCK_MONOTONIC, &(stTime));                           \
    if(u32IntMs > 0)                                                    \
    {                                                                   \
        MS_U32 u32Sec = (u32IntMs) / 1000;                              \
        (stTime).tv_sec += u32Sec;                                      \
        (stTime).tv_nsec += ((u32IntMs) - (1000 * u32Sec)) * 1000000;   \
        if ((stTime).tv_nsec >= 1000000000)                             \
        {                                                               \
            (stTime).tv_sec++;                                          \
            (stTime).tv_nsec -= 1000000000;                             \
        }                                                               \
    }                                                                   \
}

#define _TimeAbs(stTime, u32IntMs)                                      \
{                                                                       \
    clock_gettime(CLOCK_REALTIME, &(stTime));                           \
    if(u32IntMs > 0)                                                    \
    {                                                                   \
        MS_U32 u32Sec = (u32IntMs) / 1000;                              \
        (stTime).tv_sec += u32Sec;                                      \
        (stTime).tv_nsec += ((u32IntMs) - (1000 * u32Sec)) * 1000000;   \
        if ((stTime).tv_nsec >= 1000000000)                             \
        {                                                               \
            (stTime).tv_sec++;                                          \
            (stTime).tv_nsec -= 1000000000;                             \
        }                                                               \
    }                                                                   \
}

#ifdef MS_DEBUG
#include "string.h"

#define PTH_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if ((r != 0) && (r != ETIMEDOUT)) \
            fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_RET_CHK(_pf_) _pf_
#endif

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

// Use 'S' as magic number //Needs to sync with header in Linux kernel!!!!
#define SYS_IOCTL_MAGIC             'S'
#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

static MS_S32 _s32FdSYS = -1;


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
//
// Variable-size Memory pool
//
#if defined (MSOS_TYPE_LINUX)

typedef struct
{
    MS_BOOL                         bUsed;
#if USE_DLMALLOC
    mspace stMemoryPool;                /* dlmalloc handler */
#else /* if USE_DLMALLOC */
    cyg_handle_t                    stMemoryPool;
    cyg_mempool_var                 stMemoryPoolInfo;
#endif /* if USE_DLMALLOC */
    MS_BOOL bMPool;
} MsOS_MemoryPool_Info;

static MsOS_MemoryPool_Info     _MsOS_MemoryPool_Info[MSOS_MEMPOOL_MAX];
static pthread_mutex_t          _MsOS_MemoryPool_Mutex;

//
// Task Management
//
typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bWaitJoin;
    pthread_t           stThreadInfo;
    void*               pStackBase;
    TaskEntry           pTaskEntry;
    MS_VIRT              u32TaskEntryData;
    char                pTaskName[MAX_NAME];
	MS_BOOL             bStackMalloc;

} MsOS_Task_Info;

static MsOS_Task_Info   _MsOS_Task_Info[MSOS_TASK_MAX];
static pthread_mutex_t  _MsOS_Task_Mutex;

#if defined (TV_OS)
typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bShared;
    union{
        MS_S32          u32Index;
        pthread_mutex_t     stMutex;
    }msos_mutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;
#else
typedef struct
{
    MS_BOOL             bUsed;
    pthread_mutex_t     stMutex;
    MS_U8               u8Name[MAX_MUTEX_NAME_LENGTH];
} MsOS_Mutex_Info;
#endif


#define MUTEX_MUTEX_NAME        "msosMutexMutex"

#if defined (TV_OS)
    static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
    static MS_S32 u32MSos_Mutex_Index = -10;
    #define MUTEX_MUTEX_LOCK()      MsOS_LockMutex(u32MSos_Mutex_Index,0)
    #define MUTEX_MUTEX_UNLOCK()    MsOS_UnlockMutex(u32MSos_Mutex_Index,2)
#elif defined (MSOS_PROCESS_SAFT_MUTEX)
    static MsOS_Mutex_Info*         _MsOS_Mutex_Info = NULL;
    static sem_t* pMutexMutex =     NULL;
    #define MUTEX_MUTEX_LOCK()      sem_wait(pMutexMutex)
    #define MUTEX_MUTEX_UNLOCK()    sem_post(pMutexMutex)
#else
    static MsOS_Mutex_Info          _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
    static pthread_mutex_t          _MsOS_Mutex_Mutex;
    #define MUTEX_MUTEX_LOCK()      PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Mutex_Mutex));
    #define MUTEX_MUTEX_UNLOCK()    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Mutex_Mutex));
#endif

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

//
// Event Group
//
typedef struct
{
    MS_BOOL bUsed;
    MS_U32                         u32EventGroup;
    pthread_mutex_t             stMutexEvent;
    pthread_cond_t              stSemaphore;
    pthread_mutex_t             stMutex;
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info     _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static pthread_mutex_t          _MsOS_EventGroup_Mutex;

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
    pthread_mutex_t     WaitQueueSpaceMutex;
    pthread_mutex_t     WaitQueueDataMutex;
} MsOS_Queue_Info;

static MsOS_Queue_Info  _MsOS_Queue_Info[MSOS_QUEUE_MAX];
static pthread_mutex_t  _MsOS_Queue_Mutex;

// @FIXME: Leave Timer later
//
// Timer
//
typedef struct
{
    MS_BOOL                bUsed;
    TimerCb             pTimerCb;
    struct itimerspec   TimeInfo;
    timer_t             TimerId;
	pthread_mutex_t     TimerMutex;
} MsOS_Timer_Info;
static MsOS_Timer_Info  _MsOS_Timer_Info[MSOS_TIMER_MAX];
#ifdef UCLIBC
static pid_t  _MsOS_Timer_Pid[MSOS_TIMER_MAX];
#endif
static pthread_mutex_t  _MsOS_Timer_Mutex;

#endif


static pthread_mutex_t  _ISR_Mutex;

static pthread_mutex_t  _MsOS_Init_Mutex = PTHREAD_MUTEX_INITIALIZER;

//
// Share memory
//
/* FIXME: patch for monaco bring up */
#define SHM_SIZE_RESEVED      (124*1024)
#define SHM_SIZE              (2<<20)
#define MAX_SHM_CLIENT_NUM      320
#define SHM_MUTEX_NAME          "msos_shm_mutex"

typedef struct
{
    MS_U8       u8ClientName[MAX_CLIENT_NAME_LENGTH+ 1];
    MS_U32      u32Offset;
    MS_U32      u32Size;
    MS_U32       u32ClientId;    // 0 means "never used"
    MS_U8       u8RefCnt;
    MS_U8       u8Dummy[2];
} MsOS_SHM_Context;

typedef struct
{
    MS_U32              u32MaxClientNum;
    MS_U32              u32ClientNum;
    MS_U32              u32ShmSize;
    MS_U32              u32Offset;
    MsOS_SHM_Context    context[MAX_SHM_CLIENT_NUM];
} MsOS_SHM_Hdr;

static MS_VIRT _shm_id = -1;
static MS_U8 *_pu8ShareMem = NULL;
static MsOS_SHM_Hdr _ShmHdr; // dummy storage

#if defined (TV_OS)
static MS_S32  pMutexShmIdx = -1;
#define SHAREMEM_MUTEX_NAME     "ShareMemoryMutex"
#define _MSOS_SHM_LOCK(pSemIndex)    MsOS_LockMutex(pSemIndex,0)
#define _MSOS_SHM_UNLOCK(pSemIndex)  MsOS_UnlockMutex(pSemIndex,4)
#elif defined (MSOS_PROCESS_SAFT_MUTEX)
static sem_t* pMutexShm = NULL;
#define _MSOS_SHM_LOCK(pSem)    sem_wait(pSem)
#define _MSOS_SHM_UNLOCK(pSem)  sem_post(pSem)
#else
#define _MSOS_SHM_LOCK(pSem)
#define _MSOS_SHM_UNLOCK(pSem)
#endif

#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
#define SHM_EXPAND_SIZE (256*1024)
#endif

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

void* _MsOS_LinuxTaskWrapper(void *);

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
    MS_BOOL bRet = TRUE;
    pthread_mutexattr_t _MsOS_Mutex_Attr;
#if defined (MSOS_PROCESS_SAFT_MUTEX)
    MS_U32 u32ShmId;
    MS_U32 u32Addr, u32Size;
#endif
    static MS_S32 bInit = 0;

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Init_Mutex));

    if (bInit == 1)
    {
        MSOS_PRINT("[MsOS_Init] already inited!\n");
        bRet = TRUE;
        goto ret;
    }

    if (FALSE == MsOS_SHM_Init())
    {
        MSOS_ERROR("[MsOS_Init] MsOS_SHM_Init failed!\n");
        bRet = FALSE;
        goto ret;
    }

    PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
    //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
    #ifdef MS_DEBUG
    PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
    #endif
    //
    // Task Management
    //
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Task_Mutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Task_Mutex));
    for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
    {
        _MsOS_Task_Info[u32I].bUsed = FALSE;
        _MsOS_Task_Info[u32I].bWaitJoin = FALSE;
		_MsOS_Task_Info[u32I].pStackBase= NULL;
		_MsOS_Task_Info[u32I].bStackMalloc= FALSE;
    }
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Task_Mutex));

    //
    // Mutex
    //
#if defined (TV_OS)
    if(u32MSos_Mutex_Index < 0)
    {
        u32MSos_Mutex_Index = MsOS_CreateNamedMutex((MS_S8*)MUTEX_MUTEX_NAME);
        if(u32MSos_Mutex_Index < 0)
        {
            MSOS_ERROR("%s : %d,crate named mutex %s \n",__FUNCTION__,__LINE__,MUTEX_MUTEX_NAME);
            bRet = FALSE;
            goto ret;
        }
    }
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }

#elif defined (MSOS_PROCESS_SAFT_MUTEX)
    if (SEM_FAILED == (pMutexMutex = sem_open(MUTEX_MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1)))
    {
        if (SEM_FAILED == (pMutexMutex = sem_open(MUTEX_MUTEX_NAME, 0)))
        {
            MSOS_ERROR("[MsOS_Init] sem_open failed!\n");
            bRet = FALSE;
            goto ret;
        }
    }
    if (FALSE == MsOS_SHM_GetId((MS_U8*)"_proc_mutex", MSOS_MUTEX_MAX* sizeof(MsOS_Mutex_Info), &u32ShmId, &u32Addr, &u32Size, MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"_proc_mutex", MSOS_MUTEX_MAX* sizeof(MsOS_Mutex_Info), &u32ShmId, &u32Addr, &u32Size, MSOS_SHM_CREATE))
        {
            MSOS_ERROR("[MsOS_Init] MsOS_SHM_GetId _proc_mutex failed!\n");
            bRet = FALSE;
            goto ret;
        }
    }
    _MsOS_Mutex_Info = (MsOS_Mutex_Info*)u32Addr;

#else
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Mutex_Mutex, &_MsOS_Mutex_Attr));
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }
#endif

    //
    // Semaphore
    //
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Semaphore_Mutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Semaphore_Mutex));
    for( u32I=0; u32I<MSOS_SEMAPHORE_MAX; u32I++)
    {
        _MsOS_Semaphore_Info[u32I].bUsed = FALSE;
    }
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Semaphore_Mutex));

    //
    // Event Group
    //
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_EventGroup_Mutex, &_MsOS_Mutex_Attr));

    for( u32I=0; u32I<MSOS_EVENTGROUP_MAX; u32I++)
    {
        _MsOS_EventGroup_Info[u32I].bUsed = FALSE;
    }

    //
    // Queue
    //
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Mutex, &_MsOS_Mutex_Attr));
    for( u32I=0; u32I<MSOS_QUEUE_MAX; u32I++)
    {
        memset(&_MsOS_Queue_Info[u32I].bUsed, 0, sizeof(MsOS_Queue_Info));
    }

    //
    // Timer
    //
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Timer_Mutex, &_MsOS_Mutex_Attr));
    for( u32I=0; u32I<MSOS_TIMER_MAX; u32I++)
    {
        _MsOS_Timer_Info[u32I].bUsed = FALSE;
        PTH_RET_CHK(pthread_mutex_init(&_MsOS_Timer_Info[u32I].TimerMutex, &_MsOS_Mutex_Attr));
    }
    //
    // Interrupt
    //
    MSOS_PRINT("pthread_mutex_init\n");
    PTH_RET_CHK(pthread_mutex_init(&_ISR_Mutex, &_MsOS_Mutex_Attr));
    MSOS_PRINT("CHIP_InitISR\n");
    CHIP_InitISR();

    PTH_RET_CHK(pthread_mutex_init(&_MsOS_MemoryPool_Mutex, &_MsOS_Mutex_Attr));


    if (_s32FdSYS < 0)   //First time open
    {
        _s32FdSYS  = open("/dev/system", O_RDWR);

        if (_s32FdSYS < 0) // return 0 ~ 19 if open device success; return -1 if failed
        {
            MSOS_ERROR("[MsOS_Init]open device node failed!\n");
            bRet = FALSE;
            goto ret;
        }
    }

    UtopiaInit();

    bInit = 1;

ret:

    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Init_Mutex));
    return bRet;
}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MsOS_Init);
#endif


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

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));

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
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));

    if(s32Id >= MSOS_MEMPOOL_MAX)
    {
        return -1;
    }

    if (pPoolAddr)
    {
        _MsOS_MemoryPool_Info[s32Id].bMPool= TRUE;
#if USE_DLMALLOC
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
#ifdef ENABLE_KERNEL_DLMALLOC
		_MsOS_MemoryPool_Info[s32Id].stMemoryPool = MsOS_MPool_CreateMemoryPool(pPoolAddr, u32PoolSize, 1);
#else
		_MsOS_MemoryPool_Info[s32Id].stMemoryPool = mstar_create_mspace_with_base(pPoolAddr, u32PoolSize, 1);
#endif
        if (NULL == _MsOS_MemoryPool_Info[s32Id].stMemoryPool)
        {
            PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
            return -1;
        }
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
        // my_info = mspace_mallinfo(_MsOS_MemoryPool_Info[s32Id].stMemoryPool);
        // printf("Total size %ld, used size %ld, free size %ld\n", (unsigned long) my_info.usmblks, (unsigned long) my_info.uordblks, (unsigned long) my_info.fordblks);
#else /* if USE_DLMALLOC */
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
        cyg_mempool_var_create( pPoolAddr,
                                u32PoolSize,
                                &_MsOS_MemoryPool_Info[s32Id].stMemoryPool,
                                &_MsOS_MemoryPool_Info[s32Id].stMemoryPoolInfo );
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
#endif /* if USE_DLMALLOC */
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
            // herer ??
        }

        MSOS_PRINT("Invalid memory pool ID: %td, you must use the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
        return TRUE;

    }

    #if 1
    if(_MsOS_MemoryPool_Info[s32PoolId].bUsed == FALSE)
    {
        MSOS_ERROR("MEMORY POOL WITH MEMORYPOOL_ID: 0x%tX NOT EXIST\n", (ptrdiff_t)s32PoolId);
        return FALSE;
    }
    #endif
    if (_MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
#if USE_DLMALLOC
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
#ifdef ENABLE_KERNEL_DLMALLOC
		MsOS_MPool_DeleteMemoryPool(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
#else
		mstar_destroy_mspace(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
#endif
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
#else /* if USE_DLMALLOC */
        cyg_mempool_var_delete(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
#endif /* if USE_DLMALLOC */
    }

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));

    _MsOS_MemoryPool_Info[s32PoolId].bUsed = FALSE;

    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
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
            MSOS_ERROR("Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
            return NULL;
        }

    }

    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        MSOS_WARN("Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
        MSOS_WARN("System will use default mpool to allocate memory here\n");
        return malloc(u32Size);
    }

#if USE_DLMALLOC
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
#ifdef ENABLE_KERNEL_DLMALLOC
    pAddr = MsOS_MPool_AllocateMemory(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);
#else
    pAddr = mstar_mspace_malloc(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);
#endif

    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
    // MSOS_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %s %d: %d %p\n", __FUNCTION__, __LINE__, s32PoolId, pAddr);
#else /* if USE_DLMALLOC */
    pAddr = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);
#endif /* if USE_DLMALLOC */

    //Current eCosPro kernel always allocates 16-B aligned block
    if (( (MS_VIRT)pAddr & 0xF) || ( (MS_VIRT)pAddr == 0x0))
    {
        return NULL;
    }

    //flush Dcache data
    MsOS_Dcache_Flush((MS_VIRT)pAddr,u32Size);

    //flush OCP data
    MsOS_FlushMemory();

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
#if USE_DLMALLOC
    // struct mallinfo my_info;
#endif


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
            MSOS_ERROR("Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
            return FALSE;
        }

    }


    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        MSOS_WARN("Invalid mpool ID:%td, you must use the default ID macro: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
        MSOS_WARN("System will free default mpool memory here\n");
        free(pAddress);

    }
    else
    {
#if USE_DLMALLOC
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
#ifdef ENABLE_KERNEL_DLMALLOC
		MsOS_MPool_FreeMemory(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pAddress);
#else
		mstar_mspace_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pAddress);
#endif
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
#else /* if USE_DLMALLOC */
        cyg_mempool_var_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pAddress);
#endif /* if USE_DLMALLOC */
    }
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
    void *pNewAddress = NULL;
#if !USE_DLMALLOC
    MS_U32 u32OrgSize;
#endif /* if !USE_DLMALLOC */

    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }


    if ( s32PoolId >= MSOS_MEMPOOL_MAX )
    {
        if ( s32PoolId == MSOS_MALLOC_ID )
        {
            return realloc(pOrgAddress, u32NewSize);
        }

        else
        {
            MSOS_WARN("Invalid memory pool ID: %td, you must use the default ID: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
            return NULL;
        }

    }
    if (FALSE== _MsOS_MemoryPool_Info[s32PoolId].bMPool)
    {
        MSOS_WARN("Invalid mpool ID: %td, you must use the default ID macro: MSOS_MALLOC_ID ,or the ID of the mpool you created\n", (ptrdiff_t)s32PoolId);
        MSOS_WARN("System will reallocate memory from default pookl\n");
        return realloc(pOrgAddress, u32NewSize);
    }

#if USE_DLMALLOC
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_MemoryPool_Mutex));
#ifdef ENABLE_KERNEL_DLMALLOC
	pNewAddress = MsOS_MPool_ReallocateMemory(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pOrgAddress, u32NewSize);
#else
	pNewAddress = mstar_mspace_realloc(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pOrgAddress, u32NewSize);
#endif
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_MemoryPool_Mutex));
#else /* if USE_DLMALLOC */
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

    struct memdq *dq = (struct memdq *) ((MS_VIRT )pOrgAddress  - sizeof(struct memdq));
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
#endif /* if USE_DLMALLOC */

    return pNewAddress;
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
MS_S32 MsOS_CreateTask (TaskEntry pTaskEntry,
                     MS_VIRT u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL bAutoStart,
                     void * pStackBase,
                     MS_U32 u32StackSize,
                     char *pTaskName)
{
    MS_S32 s32Id;
    pthread_attr_t thread_attr;
#if !defined (ANDROID)
    struct sched_param thrsched;
#endif

    // Check parameters
    /* - MaxCC20080205
    if ( ((MS_U32)pStackBase & 0xFFF ) || (u32StackSize < PTHREAD_STACK_MIN)|| (u32StackSize & 0xFFF))
    {
        return -1;
    }
    // . MaxCC20080205 */
    //printf("pthread_mutex_lock\n");
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Task_Mutex));
    for(s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
            if(_MsOS_Task_Info[s32Id].bWaitJoin == TRUE)
            {
                if (0 == PTH_RET_CHK(pthread_join(_MsOS_Task_Info[s32Id].stThreadInfo, NULL)))
                {
                    _MsOS_Task_Info[s32Id].bWaitJoin = FALSE;
                }
                else
                {
                    MSOS_ERROR("thread %td join fail\n", (ptrdiff_t)s32Id);
                }
            }
			
			if((_MsOS_Task_Info[s32Id].pStackBase != NULL) && (_MsOS_Task_Info[s32Id].bStackMalloc == TRUE))
            {
			    free(_MsOS_Task_Info[s32Id].pStackBase);
				_MsOS_Task_Info[s32Id].pStackBase = NULL;
				_MsOS_Task_Info[s32Id].bStackMalloc = FALSE;
			}
			
            break;
        }
    }
    if( s32Id < MSOS_TASK_MAX)
    {
        _MsOS_Task_Info[s32Id].bUsed = TRUE;
        _MsOS_Task_Info[s32Id].bWaitJoin = TRUE;
    }
    //printf("pthread_mutex_unlock\n");
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Task_Mutex));

    if( s32Id >= MSOS_TASK_MAX)
    {
        return -1;
    }
    //printf("pthread_attr_init\n");
    if (PTH_RET_CHK(pthread_attr_init(&thread_attr)))
    {
        return -1;
    }

    if (NULL != pStackBase)
    {
         //printf("posix_memalign\n");
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600)
        if (posix_memalign(&pStackBase, 0x1000, u32StackSize))
        {
            return -1;
        }
#else
        pStackBase = memalign(0x1000, u32StackSize);
        if (0 == pStackBase)
        {
            return -1;
        }
#endif
        MS_ASSERT(pStackBase);
        MS_ASSERT(0 == ((MS_VIRT)pStackBase & 0xFFF ));
        _MsOS_Task_Info[s32Id].pStackBase= pStackBase;
		_MsOS_Task_Info[s32Id].bStackMalloc = TRUE;
    }
    else
    {
        _MsOS_Task_Info[s32Id].pStackBase= NULL;
    }

    _MsOS_Task_Info[s32Id].pTaskEntry = pTaskEntry;
    _MsOS_Task_Info[s32Id].u32TaskEntryData = u32TaskEntryData;
    memset(_MsOS_Task_Info[s32Id].pTaskName,'\0',sizeof(MAX_NAME));
    strncpy(_MsOS_Task_Info[s32Id].pTaskName,pTaskName,(MAX_NAME - 1));

    /* - MaxCC20080205
    if (pthread_attr_setstack(&thread_attr, (void*)(((MS_U32)pStackBase)+ u32StackSize), u32StackSize))
    {
        return -1;
    }
    // . MaxCC20080205 */
#if !defined (ANDROID)
    //set thread priority
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);//thread will not exit after return until join
    //pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);//thread will auto exit after return.
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    switch(eTaskPriority)
    {
        case E_TASK_PRI_SYS:
        case E_TASK_PRI_HIGHEST:
    		pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
            break;
        default:
            pthread_attr_setschedpolicy(&thread_attr, SCHED_OTHER);
            break;
    }
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
        case E_TASK_PRI_MEDIUM:
        case E_TASK_PRI_LOW:
        case E_TASK_PRI_LOWEST:
        default:
            thrsched.sched_priority = sched_get_priority_min(SCHED_OTHER);
                break;
    }
    pthread_attr_setschedparam(&thread_attr, &thrsched);
    //pthread_attr_getschedparam(&thread_attr, &thrsched);printf("%d\n",thrsched.sched_priority);
    //printf("max=%d,min=%d\n",sched_get_priority_max(SCHED_RR),sched_get_priority_min(SCHED_RR));
#endif

    // @FIXME:
    //     (1) eTaskPriority: Task priority is ignored here
    //     (2) bAutoStart: Auto start is ignored here
    //     (3) pTaskName: is ignored here
    //printf("pthread_create\n");
    PTH_RET_CHK(pthread_create(&_MsOS_Task_Info[s32Id].stThreadInfo,
                               &thread_attr,
                               _MsOS_LinuxTaskWrapper,
                               (void *)&_MsOS_Task_Info[s32Id]));





    s32Id |= MSOS_ID_PREFIX;
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
    if ((s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }

    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    #if 0
    if( _MsOS_Task_Info[s32TaskId].bUsed == FALSE )
    {
        printf("TASK WITH TASK_ID:0x%lx NOT EXIST\n",s32TaskId);
        return FALSE ;
    }
    #endif


#if defined (ANDROID)
    MSOS_WARN("ERROR!!! pthread_cancel() is not supported by BIONIC libc\n");
#else
    if (_MsOS_Task_Info[s32TaskId].stThreadInfo != (int)NULL)
    {
        PTH_RET_CHK(pthread_cancel(_MsOS_Task_Info[s32TaskId].stThreadInfo));
    }
    else
    {
        MSOS_ERROR("TASK WITH TASK_ID: %td NOT EXIST\n", (ptrdiff_t)s32TaskId);
        return FALSE ;
    }
#endif

    if (0== PTH_RET_CHK(pthread_join(_MsOS_Task_Info[s32TaskId].stThreadInfo, NULL)))
    {
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Task_Mutex));
        if (_MsOS_Task_Info[s32TaskId].pStackBase)
        {
            free(_MsOS_Task_Info[s32TaskId].pStackBase);
            _MsOS_Task_Info[s32TaskId].pStackBase= NULL;
        }
        _MsOS_Task_Info[s32TaskId].bUsed = FALSE;
        _MsOS_Task_Info[s32TaskId].bWaitJoin = FALSE;

        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Task_Mutex));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void)
{
    sched_yield();
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
    struct timespec req, rem;
    if(u32Ms >= 1000)
    {
        req.tv_sec = u32Ms/1000;
        req.tv_nsec = (long) ((u32Ms%1000)*1000000UL);
    }
    else
    {
        req.tv_sec = 0;
        req.tv_nsec = (long) (u32Ms*1000000UL);
    }

    //u32Ms=0 => The task will enter sleep state and wake up immediately with relative trigger time = 0
    // => Other ready tasks with the same or lower priorities will have no chance to run
    MS_ASSERT( u32Ms != 0);

    while(1)
    {
        MS_S32 err;

        err = nanosleep(&req, &rem);

        if(err==-1)
        {
            switch(errno)
            {
                case EINTR:
                    req.tv_sec = rem.tv_sec;
                    req.tv_nsec = rem.tv_nsec;
                   continue;
                default:
                    if(errno !=(int)NULL)
                        MSOS_ERROR("nanosleep is interrupted: %d\n", errno);
                    break;
            }

        }

        break;
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
    struct timespec req, rem;
    req.tv_sec = 0;
    req.tv_nsec = (long) (u32Us*1000UL);

    while(1)
    {
        MS_S32 err;

        err = nanosleep(&req, &rem);
        if(err==-1)
        {
           switch(errno)
           {
              case EINTR:
                  req.tv_sec = rem.tv_sec;
                  req.tv_nsec = rem.tv_nsec;
                  continue;
              default:
                  if(errno !=(int)NULL)
                      MSOS_ERROR("nanosleep is interrupted: %d\n", errno);
                  break;
           }

      }

        break;
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
    struct timespec prev, cur;
	MS_U32 u32Ns = (long) (u32Us*1000UL);

	clock_gettime(CLOCK_REALTIME, &prev);

	do
	{
		 clock_gettime(CLOCK_REALTIME, &cur);
		 if (((cur.tv_nsec-prev.tv_nsec) + ((cur.tv_sec-prev.tv_sec)*1000000000UL)) >= u32Ns)
			break;
	} while (1);
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
    pthread_t self;

    self= pthread_self();

    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if( self== _MsOS_Task_Info[s32Id].stThreadInfo)
        {
            break;
        }
    }
    if( s32Id < MSOS_TASK_MAX)
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
/// Get thread ID of current thread/process in Linux
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    return (MS_S32)pthread_self();
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
    pthread_mutexattr_t _MsOS_Mutex_Attr;
    MS_S32 s32Prop = (MSOS_PROCESS_PRIVATE == u32Flag)? PTHREAD_PROCESS_PRIVATE: PTHREAD_PROCESS_SHARED;
    MS_U8 pMutexName[MAX_MUTEX_NAME_LENGTH];
    MS_U32 u32MaxLen;

    if (NULL == pMutexName1)
    {
        return -1L;
    }
    if (strlen(pMutexName1) >= (MAX_MUTEX_NAME_LENGTH-1))
    {
        MSOS_WARN("%s: Warning strlen(%s) is longer than MAX_MUTEX_NAME_LENGTH(%d). Oversize char will be discard.\n",
        __FUNCTION__,pMutexName1,MAX_MUTEX_NAME_LENGTH);
    }
    if (0 == (u32MaxLen = MIN(strlen(pMutexName1), (MAX_MUTEX_NAME_LENGTH-1))))
    {
        return -1L;
    }
    strncpy((char*)pMutexName, (const char*)pMutexName1, u32MaxLen);
    pMutexName[u32MaxLen] = '\0';

#if defined (TV_OS)
    if (u32MSos_Mutex_Index < 0)
    {
        u32MSos_Mutex_Index = MsOS_CreateNamedMutex((MS_S8*)MUTEX_MUTEX_NAME);
        if(u32MSos_Mutex_Index < 0)
        {
            MSOS_ERROR("%s : %d,crate named mutex %s \n",__FUNCTION__,__LINE__,MUTEX_MUTEX_NAME);
            return FALSE;
        }
    }
#endif

    MUTEX_MUTEX_LOCK();
    for (s32Id=0;s32Id<MSOS_MUTEX_MAX;s32Id++)
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
#if defined (TV_OS)
        if (PTHREAD_PROCESS_SHARED == s32Prop)       //want shared mutex
        {
            _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
            strncpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName,MIN(strlen((const char*)pMutexName),(MAX_MUTEX_NAME_LENGTH-1)));
            _MsOS_Mutex_Info[s32LstUnused].msos_mutex.u32Index = MsOS_CreateNamedMutex((MS_S8*)_MsOS_Mutex_Info[s32LstUnused].u8Name);
            if(_MsOS_Mutex_Info[s32LstUnused].msos_mutex.u32Index < 0)
            {
                MSOS_ERROR("~!~%s create %s fail \n",__FUNCTION__,_MsOS_Mutex_Info[s32LstUnused].u8Name);
                return -1L;
            }
            _MsOS_Mutex_Info[s32LstUnused].bShared = TRUE;
        }
        else        //want local mutex
        {
            PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
            PTH_RET_CHK(pthread_mutexattr_setpshared(&_MsOS_Mutex_Attr, s32Prop));
           //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
         #ifdef MS_DEBUG
            PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
         #endif
            _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
            strncpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName,MIN(strlen((const char*)pMutexName),(MAX_MUTEX_NAME_LENGTH-1)));
            PTH_RET_CHK(pthread_mutex_init(&_MsOS_Mutex_Info[s32LstUnused].msos_mutex.stMutex, &_MsOS_Mutex_Attr));
            _MsOS_Mutex_Info[s32LstUnused].bShared = FALSE;
        }
        s32Id = s32LstUnused;
#else
        PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
        PTH_RET_CHK(pthread_mutexattr_setpshared(&_MsOS_Mutex_Attr, s32Prop));
       //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
     #ifdef MS_DEBUG
        PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
     #endif
        _MsOS_Mutex_Info[s32LstUnused].bUsed = TRUE;
        strncpy((char*)_MsOS_Mutex_Info[s32LstUnused].u8Name, (const char*)pMutexName,MIN(strlen((const char*)pMutexName),(MAX_MUTEX_NAME_LENGTH-1)));
        PTH_RET_CHK(pthread_mutex_init(&_MsOS_Mutex_Info[s32LstUnused].stMutex, &_MsOS_Mutex_Attr));
        s32Id = s32LstUnused;
#endif
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

    #if 1
    if( (_MsOS_Mutex_Info[s32MutexId].bUsed == FALSE))
    {
        MSOS_ERROR("MUTEX WITH MUTEX_ID: %td NOT EXIST\n", (ptrdiff_t)s32MutexId);
        return FALSE;
    }
    #endif

    MUTEX_MUTEX_LOCK();

    MS_ASSERT(_MsOS_Mutex_Info[s32MutexId].bUsed);

#if defined (TV_OS)
    if(_MsOS_Mutex_Info[s32MutexId].bShared)
        MsOS_DeleteNamedMutexbyIndex((_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index));
    else
        PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Mutex_Info[s32MutexId].msos_mutex.stMutex));
#else
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Mutex_Info[s32MutexId].stMutex));
#endif

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
#if defined (TV_OS)
        if (_MsOS_Mutex_Info[s32MutexId].bShared)
        {
            if(!MsOS_LockMutex(_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index,0))
            {
                fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
            }
            else
            {
                bRet = TRUE;
            }
        }
        else
        {
            if(PTH_RET_CHK(pthread_mutex_lock(&(_MsOS_Mutex_Info[s32MutexId].msos_mutex.stMutex))))
            {
                fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
            }
            else
            {
                bRet = TRUE;
            }
        }
#else
        if (PTH_RET_CHK(pthread_mutex_lock(&(_MsOS_Mutex_Info[s32MutexId].stMutex))))
        {
            fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
        }
        else
        {
            bRet = TRUE;
        }
#endif
    }
    else if (u32WaitMs==0) //non-blocking
    {
#if defined (TV_OS)
        if (_MsOS_Mutex_Info[s32MutexId].bShared)
        {
            if(!MsOS_LockMutex(_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index,-1))      //!!!!!!  imply later
                bRet = TRUE;
        }
        else
        {
            if (!PTH_RET_CHK(pthread_mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].msos_mutex.stMutex)))
                bRet = TRUE;
        }
#else
        if (!PTH_RET_CHK(pthread_mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex)))
        {
            bRet = TRUE;
        }
#endif
    }
    else //blocking wait with timeout
    {
#if defined (TV_OS)
        if (_MsOS_Mutex_Info[s32MutexId].bShared)
            bRet = MsOS_LockMutex(_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index,u32WaitMs);
        else
        {
            PTH_RET_CHK(pthread_mutex_lock(&(_MsOS_Mutex_Info[s32MutexId].msos_mutex.stMutex)));
            bRet = TRUE;
        }
#else
        struct timespec         StopTime;
        _TimeAbs(StopTime, u32WaitMs);
        bRet= (PTH_RET_CHK(pthread_mutex_timedlock(&_MsOS_Mutex_Info[s32MutexId].stMutex, &StopTime)))? FALSE: TRUE;
#endif
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
#if defined (TV_OS)
    if(_MsOS_Mutex_Info[s32MutexId].bShared)
    {
        if(!MsOS_UnlockMutex(_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index,5))
        {
            bRet = FALSE;
            fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
        }
    }
    else
    {
        if(PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].msos_mutex.stMutex)))
        {
            bRet = FALSE;
            fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
        }
    }
#else
    if(PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].stMutex)))
    {
        bRet = FALSE;
        fprintf(stderr, "Mutex Name: %s\n", _MsOS_Mutex_Info[s32MutexId].u8Name);
    }
#endif

    return bRet;
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
#if defined (TV_OS)
    if ( (s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }
    return MsOS_CrossThreadUnlockMutex(_MsOS_Mutex_Info[s32MutexId].msos_mutex.u32Index, bEnable);
#else
    return TRUE;
#endif
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

    #if 1
    if(_MsOS_Semaphore_Info[s32SemaphoreId].bUsed == FALSE )
    {
        MSOS_ERROR("SEMAPHORE WITH SEMAPHORE_ID: %td NOT EXIST\n", (ptrdiff_t)s32SemaphoreId);
        return FALSE;
    }
    #endif

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
#if !defined (ANDROID) || defined (__LP64__)
    pthread_condattr_t attr;
#endif

    PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
    //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
#ifdef MS_DEBUG
    PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
#endif

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Mutex));
    for(s32Id=0; s32Id<MSOS_EVENTGROUP_MAX; s32Id++)
    {
        if(_MsOS_EventGroup_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }

    if(s32Id < MSOS_EVENTGROUP_MAX)
    {
        PTH_RET_CHK(pthread_mutex_init(&_MsOS_EventGroup_Info[s32Id].stMutexEvent, &_MsOS_Mutex_Attr));
        PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Info[s32Id].stMutexEvent));
        _MsOS_EventGroup_Info[s32Id].bUsed = TRUE;
        _MsOS_EventGroup_Info[s32Id].u32EventGroup= 0;
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32Id].stMutexEvent));
    }
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Mutex));

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }

    PTH_RET_CHK(pthread_mutex_init(&_MsOS_EventGroup_Info[s32Id].stMutex, &_MsOS_Mutex_Attr));
#if !defined (ANDROID) || defined (__LP64__)
    // Set the clock to be CLOCK_MONOTONIC
    pthread_condattr_init(&attr);
    PTH_RET_CHK(pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
    PTH_RET_CHK(pthread_cond_init(&_MsOS_EventGroup_Info[s32Id].stSemaphore, &attr));
#else
    PTH_RET_CHK(pthread_cond_init(&_MsOS_EventGroup_Info[s32Id].stSemaphore, NULL));
#endif
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
        MSOS_ERROR("EVENTGROUP WITH EVENTGROUP: %td NOT EXIST\n", (ptrdiff_t)s32EventGroupId);
        return FALSE;
    }
    #endif

    PTH_RET_CHK(pthread_cond_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore));
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stMutex));
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Mutex));
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
    _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup= 0;
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
    _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Mutex));
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
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

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
    SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    PTH_RET_CHK(pthread_cond_broadcast(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore));
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
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
    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
    RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
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
//        if (0== u32WaitMs)
//        {
//            u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//        }
        _TimeAbsM(StopTime, u32WaitMs);
    }

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
    do{
        *pu32RetrievedEventFlag= HAS_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32WaitEventFlag);
        if ((bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag))
        {
            break;
        }
        if (u32WaitMs== MSOS_WAIT_FOREVER) //blocking wait
        {
            PTH_RET_CHK(pthread_cond_wait(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                          &_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
        }
        else if(u32WaitMs == 0)
        {
            break;
        }
        else
        {
#if !defined (ANDROID) || defined (__LP64__)
            if (PTH_RET_CHK(pthread_cond_timedwait(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                                   &_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,
                                                   &StopTime)))
#else
            if (PTH_RET_CHK(pthread_cond_timedwait_monotonic(&_MsOS_EventGroup_Info[s32EventGroupId].stSemaphore,
                                                   &_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent,
                                                   &StopTime)))
#endif
            {
                break;
            }
        }
    } while (1);

    bRet= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (bRet && bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent));
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
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_WaitEvent_Interrupt(MS_S32 s32EventGroupId,
                     MS_U32 u32WaitEventFlag,
                     MS_U32 *pu32RetrievedEventFlag,
                     EventWaitMode eWaitMode,
                     MS_U32 u32WaitMs)
{
    return  (MS_S32)MsOS_WaitEvent(s32EventGroupId,u32WaitEventFlag,pu32RetrievedEventFlag,eWaitMode,u32WaitMs);
}

//
// Timer management
//
#ifdef UCLIBC
static void _MsOS_TimerNotify(MS_S32 sig, siginfo_t *si, void *uc)
{
    //printf("_MsOS_TimerNotify\n");
    timer_t *tidp;
    MS_S32 s32Id;

    tidp = si->si_value.sival_ptr;
    s32Id = (MS_S32)*tidp;

    MS_ASSERT(_MsOS_Timer_Info[s32Id].bUsed);
    _MsOS_Timer_Info[s32Id].pTimerCb(0, s32Id| MSOS_ID_PREFIX);
}
#else
static void _MsOS_TimerNotify(sigval_t v)
{
    MS_S32 s32Id= v.sival_int;
    int err = errno;
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Timer_Info[s32Id].TimerMutex));
    MS_ASSERT(_MsOS_Timer_Info[s32Id].bUsed);
    if(_MsOS_Timer_Info[s32Id].bUsed)
        _MsOS_Timer_Info[s32Id].pTimerCb(0, s32Id| MSOS_ID_PREFIX);
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Timer_Info[s32Id].TimerMutex));
    errno = err;
}
#endif

#ifdef UCLIBC
void Timer_Thread(void *data)
{
    TimerSET curr_set;
    curr_set.create_ready= ((TimerSET*)data)->create_ready;
    curr_set.pTimerCb= ((TimerSET*)data)->pTimerCb;
    curr_set.u32FirstTimeMs= ((TimerSET*)data)->u32FirstTimeMs;
    curr_set.u32PeriodTimeMs= ((TimerSET*)data)->u32PeriodTimeMs;
    curr_set.bStartTimer= ((TimerSET*)data)->bStartTimer;
    curr_set.pTimerName= ((TimerSET*)data)->pTimerName;
    MS_S32 s32Id = ((TimerSET*)data)->s32Id;

    struct itimerspec StopTimer;
    struct itimerspec dummy;
    struct sigevent   SignalEvent;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = _MsOS_TimerNotify;
    sigaddset(&sa.sa_mask, SIGRTMIN);
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        // @FIXME: should not fail here. Do error handling later
        MS_ASSERT(0);
    }
    /* Block timer signal temporarily */
    MSOS_PRINT("Blocking signal %d\n", SIGRTMIN);

    if (sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL) == -1)
    {
        // @FIXME: should not fail here. Do error handling later
        MS_ASSERT(0);
    }
    /* Create the timer */
    SignalEvent.sigev_notify = SIGEV_SIGNAL;
    SignalEvent.sigev_signo = SIGRTMIN;
    SignalEvent.sigev_value.sival_ptr = &s32Id;

    if (0 > timer_create(CLOCK_REALTIME, &SignalEvent, &_MsOS_Timer_Info[s32Id].TimerId))
    {
        MSOS_ERROR("Failed to Create the timer.\n");
        // @FIXME: should not fail here. Do error handling later
        MS_ASSERT(0);
    }
    /* Start the timer */
    _MsOS_Timer_Info[s32Id].pTimerCb=   curr_set.pTimerCb;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_sec=           curr_set.u32FirstTimeMs/1000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_nsec=          (curr_set.u32FirstTimeMs- (_MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_sec)*1000)* 1000000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_sec=        curr_set.u32PeriodTimeMs/1000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_nsec=       (curr_set.u32PeriodTimeMs- (_MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_sec)*1000)* 1000000;
    if(curr_set.bStartTimer)
    {
        if (0> timer_settime(_MsOS_Timer_Info[s32Id].TimerId, 0, &_MsOS_Timer_Info[s32Id].TimeInfo, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }
    else
    {
        memset(&StopTimer, 0, sizeof(StopTimer));
        if (0> timer_settime(_MsOS_Timer_Info[s32Id].TimerId, 0, &StopTimer, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }
    //Unblock signal
    if (sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL) == -1)
    {
        // @FIXME: should not fail here. Do error handling later
        MS_ASSERT(0);
    }

    ((TimerSET*)data)->create_ready=1;
    while(1)
    {
        sigwaitinfo(&sa.sa_mask, NULL);
    }
}
#endif

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
    struct itimerspec StopTimer;
    struct itimerspec dummy;
    struct sigevent   SignalEvent;

    PTH_RET_CHK(pthread_mutex_lock( &_MsOS_Timer_Mutex));
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
    PTH_RET_CHK(pthread_mutex_unlock( &_MsOS_Timer_Mutex));

    if(s32Id >= MSOS_TIMER_MAX)
    {
        return -1;
    }
#ifdef UCLIBC //hardes
    TimerSET* curr_set = malloc(sizeof(TimerSET));
    curr_set->create_ready = 0;
    curr_set->pTimerCb=pTimerCb;
    curr_set->u32FirstTimeMs=u32FirstTimeMs;
    curr_set->u32PeriodTimeMs=u32PeriodTimeMs;
    curr_set->bStartTimer=bStartTimer;
    curr_set->pTimerName=pTimerName;
    curr_set->s32Id = s32Id;
    pthread_t id;
    MS_S32 ret;
    //ret=pthread_create(&id,NULL,(void *) _thread,NULL);
    ret=pthread_create(&id,NULL,(void *) Timer_Thread,(void *) curr_set);
    if(ret!=0){
        MSOS_ERROR("Create pthread error!\n");
        MS_ASSERT(0);
    }
    else
    {
        MSOS_PRINT ("Create pthread success!\n");
    }
    while(!curr_set->create_ready)
    {
        sleep(0);
    }
    free(curr_set);
    s32Id |= MSOS_ID_PREFIX;
    return s32Id;

#else
    memset(&SignalEvent, 0, sizeof(SignalEvent));
    SignalEvent.sigev_notify=           SIGEV_THREAD;
    SignalEvent.sigev_notify_function=  _MsOS_TimerNotify;
    SignalEvent.sigev_value.sival_int=  s32Id;

    if(0> timer_create(CLOCK_REALTIME, &SignalEvent, &_MsOS_Timer_Info[s32Id].TimerId))
    {
        return -1;
    }

    _MsOS_Timer_Info[s32Id].pTimerCb=   pTimerCb;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_sec=           u32FirstTimeMs/1000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_nsec=          (u32FirstTimeMs- (_MsOS_Timer_Info[s32Id].TimeInfo.it_value.tv_sec)*1000)* 1000000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_sec=        u32PeriodTimeMs/1000;
    _MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_nsec=       (u32PeriodTimeMs- (_MsOS_Timer_Info[s32Id].TimeInfo.it_interval.tv_sec)*1000)* 1000000;

    if(bStartTimer)
    {
        if (0> timer_settime(_MsOS_Timer_Info[s32Id].TimerId, 0, &_MsOS_Timer_Info[s32Id].TimeInfo, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }
    else
    {
        memset(&StopTimer, 0, sizeof(StopTimer));
        if (0> timer_settime(_MsOS_Timer_Info[s32Id].TimerId, 0, &StopTimer, &dummy))
        {
            // @FIXME: should not fail here. Do error handling later
            MS_ASSERT(0);
        }
    }

    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Delete the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTimer (MS_S32 s32TimerId)
{
    MsOS_StopTimer(s32TimerId);
    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Timer_Info[s32TimerId].TimerMutex));
    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        #ifdef UCLIBC
        kill(_MsOS_Timer_Pid[s32TimerId], SIGTERM);
        #endif
        timer_delete(_MsOS_Timer_Info[s32TimerId].TimerId);
        _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
        _MsOS_Timer_Info[s32TimerId].pTimerCb = NULL;
        memset(&_MsOS_Timer_Info[s32TimerId].TimeInfo, 0, sizeof(struct itimerspec));
        _MsOS_Timer_Info[s32TimerId].TimerId = 0;
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Timer_Info[s32TimerId].TimerMutex));
        return TRUE;
    }
    else
    {
        MSOS_ERROR("TIMER WITH TIMER_ID: %td NOT EXIST\n", (ptrdiff_t)s32TimerId);
        PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Timer_Info[s32TimerId].TimerMutex));
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
    struct itimerspec dummy;

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
        if(0> timer_settime(_MsOS_Timer_Info[s32TimerId].TimerId, 0, &_MsOS_Timer_Info[s32TimerId].TimeInfo, &dummy))
        {
            MS_ASSERT(0);
        }
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

    if( _MsOS_Timer_Info[s32TimerId].bUsed )
    {
        #ifdef UCLIBC
        // Need to kill previous thread and create a new thread for new timer?
        #endif
        memset(&StopTimer, 0, sizeof(StopTimer));
        if(0> timer_settime(_MsOS_Timer_Info[s32TimerId].TimerId, 0, &StopTimer, &dummy))
        {
           MS_ASSERT(0);
        }
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


    #ifdef UCLIBC
    // Need to kill previous thread and create a new thread for new timer
    #endif

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

#ifdef UCLIBC
    clock_gettime(CLOCK_REALTIME, &ts);
#else
    clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
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

static void _MsOS_QueueMutexUnlock(pthread_mutex_t* pMutex)
{
    PTH_RET_CHK(pthread_mutex_unlock(pMutex));
}
static MS_BOOL _MsOS_QueueMutexLock(pthread_mutex_t* pMutex, MS_U32 u32WaitMs)
{
    MS_BOOL bRet = TRUE;

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        PTH_RET_CHK(pthread_mutex_lock(pMutex));
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if (!PTH_RET_CHK(pthread_mutex_trylock(pMutex)))
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
#if defined (TV_OS)
        PTH_RET_CHK(pthread_mutex_lock(pMutex));
        bRet = TRUE;
#else
        struct timespec         StopTime;
        _TimeAbs(StopTime, u32WaitMs);
        bRet= (PTH_RET_CHK(pthread_mutex_timedlock(pMutex, &StopTime)))? FALSE: TRUE;
#endif
    }
    return bRet;
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
#if !defined (ANDROID) || defined (__LP64__)
    pthread_condattr_t attr;
#endif

    PTH_RET_CHK(pthread_mutexattr_init(&_MsOS_Mutex_Attr));
    //PTH_RET_CHK(pthread_mutexattr_setprotocol(&_MsOS_Mutex_Attr, PTHREAD_PRIO_INHERIT));
#ifdef MS_DEBUG
    PTH_RET_CHK(pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK));
#endif

    if (E_MSG_VAR_SIZE== eMessageType)
    {
        MSOS_ERROR("E_MSG_VAR_SIZE has not been supported yet\n");
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

#if !defined (ANDROID) || defined (__LP64__)
    pthread_condattr_init(&attr);
    PTH_RET_CHK(pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].SendSem, &attr));
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].RecvSem, &attr));
#else
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].SendSem, NULL));
    PTH_RET_CHK(pthread_cond_init(&_MsOS_Queue_Info[s32Id].RecvSem, NULL));
#endif
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].SendMutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].RecvMutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].WaitQueueSpaceMutex, &_MsOS_Mutex_Attr));
    PTH_RET_CHK(pthread_mutex_init(&_MsOS_Queue_Info[s32Id].WaitQueueDataMutex, &_MsOS_Mutex_Attr));

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

    #if 1
    if(_MsOS_Queue_Info[s32QueueId].bUsed == FALSE)
    {
        MSOS_ERROR("QUEUE WITH QUEUEID: %td NOT EXIST\n", (ptrdiff_t)s32QueueId);
        return FALSE;
    }
    #endif


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
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Queue_Info[s32QueueId].WaitQueueSpaceMutex));
    PTH_RET_CHK(pthread_mutex_destroy(&_MsOS_Queue_Info[s32QueueId].WaitQueueDataMutex));
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

    _MsOS_QueueMutexLock(&_MsOS_Queue_Info[s32QueueId].SendMutex, u32WaitMs);

    pQueueInfo= &_MsOS_Queue_Info[s32QueueId];
    if(u32Size > pQueueInfo->u32AlignedMsgSize)
    {
        _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].SendMutex);
        return FALSE;
    }

    if (u32Size>= _MsOS_QueueFreeSize(pQueueInfo))
    {
        _MsOS_QueueMutexLock(&_MsOS_Queue_Info[s32QueueId].WaitQueueSpaceMutex, u32WaitMs);
        if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
        {
            PTH_RET_CHK(pthread_cond_wait(&pQueueInfo->RecvSem, &pQueueInfo->WaitQueueSpaceMutex));
        }
        else
        {
            struct timespec     StopTime;
//            if (0== u32WaitMs)
//            {
//                u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//            }
            _TimeAbsM(StopTime, u32WaitMs);
#if !defined (ANDROID) || defined (__LP64__)
            if (PTH_RET_CHK(pthread_cond_timedwait(&pQueueInfo->RecvSem, &pQueueInfo->WaitQueueSpaceMutex, &StopTime)))
#else
            if (PTH_RET_CHK(pthread_cond_timedwait_monotonic(&pQueueInfo->RecvSem, &pQueueInfo->WaitQueueSpaceMutex, &StopTime)))
#endif
            {
                _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].WaitQueueSpaceMutex);
                _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].SendMutex);
                return FALSE;
            }
        }
        _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].WaitQueueSpaceMutex);
    }

    memcpy(pQueueInfo->pu8Write, pu8Message, u32Size);
    pQueueInfo->pu8Write+= pQueueInfo->u32AlignedMsgSize;
    if (pQueueInfo->pu8Write>= pQueueInfo->pu8Tail)
    {
        pQueueInfo->pu8Write= pQueueInfo->pu8Head;
    }
    PTH_RET_CHK(pthread_cond_signal(&pQueueInfo->SendSem));
    _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].SendMutex);
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

    _MsOS_QueueMutexLock(&_MsOS_Queue_Info[s32QueueId].RecvMutex, u32WaitMs);

    pQueueInfo= &_MsOS_Queue_Info[s32QueueId];

    MS_ASSERT(ALIGN_4(u32IntendedSize)== pQueueInfo->u32AlignedMsgSize);

    if (0== _MsOS_QueueUsedSize(pQueueInfo))
    {
        _MsOS_QueueMutexLock(&_MsOS_Queue_Info[s32QueueId].WaitQueueDataMutex, u32WaitMs);
        if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
        {
            PTH_RET_CHK(pthread_cond_wait(&pQueueInfo->SendSem, &pQueueInfo->WaitQueueDataMutex));
        }
        else
        {
            struct timespec     StopTime;
//            if (0== u32WaitMs)
//            {
//                u32WaitMs=          MSOS_TIME_MIN_WAIT_TV;
//            }
            _TimeAbsM(StopTime, u32WaitMs);

#if !defined (ANDROID) || defined (__LP64__)
            if (PTH_RET_CHK(pthread_cond_timedwait(&pQueueInfo->SendSem, &pQueueInfo->WaitQueueDataMutex, &StopTime)))
#else
            if (PTH_RET_CHK(pthread_cond_timedwait_monotonic(&pQueueInfo->SendSem, &pQueueInfo->WaitQueueDataMutex, &StopTime)))
#endif
            {
                _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].WaitQueueDataMutex);
                _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].RecvMutex);
                return FALSE;
            }
        }
        _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].WaitQueueDataMutex);
    }

    memcpy(pu8Message, pQueueInfo->pu8Read, u32IntendedSize);
    pQueueInfo->pu8Read+= pQueueInfo->u32AlignedMsgSize;
    if (pQueueInfo->pu8Read>= pQueueInfo->pu8Tail)
    {
        pQueueInfo->pu8Read= pQueueInfo->pu8Head;
    }
    PTH_RET_CHK(pthread_cond_signal(&pQueueInfo->RecvSem));
    *pu32ActualSize = u32IntendedSize;
    _MsOS_QueueMutexUnlock(&_MsOS_Queue_Info[s32QueueId].RecvMutex);
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
    PTH_RET_CHK(pthread_mutex_lock(&_ISR_Mutex));

    CHIP_AttachISR(eIntNum, pIntCb);
#if 0 // @fixme: move to oberon or euclid
    CHIP_AttachISR( (MS_U16)(eIntNum+8), (MS_U16)(eIntNum), pIntCb) ;
    _ISR_Info[eIntNum].pIntCb = pIntCb;
    _ISR_Info[eIntNum].bUsed = TRUE;
#endif

    PTH_RET_CHK(pthread_mutex_unlock(&_ISR_Mutex));

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
    PTH_RET_CHK(pthread_mutex_lock(&_ISR_Mutex));

    CHIP_DetachISR(eIntNum);

    PTH_RET_CHK(pthread_mutex_unlock(&_ISR_Mutex));

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Debug the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @param IrqDebugOpt \n IN: Debug option
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DebugInterrupt (InterruptNum eIntNum, IrqDebugOpt eIrqDebugOpt)
{
    return CHIP_DebugIRQ(eIntNum, eIrqDebugOpt);
}

//-------------------------------------------------------------------------------------------------
/// Enable (unmask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableInterrupt (InterruptNum eIntNum)
{
    return CHIP_EnableIRQ(eIntNum);
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
/// Notify the interrupt complete #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CompleteInterrupt (InterruptNum eIntNum)
{
    return CHIP_CompleteIRQ(eIntNum);
}

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_DisableAllInterrupts(void)
{
    return CHIP_DisableAllInterrupt() ;
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

    return CHIP_EnableAllInterrupt() ;
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
    return CHIP_InISRContext();
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#define DCACHE_LINE_SIZE 16
MS_BOOL MsOS_Dcache_Flush( MS_VIRT ptrStart, MS_SIZE tSize )
{
    return MsOS_MPool_Dcache_Flush(ptrStart, tSize);
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
    return FALSE;
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
    return FALSE;
}

MS_PHY MsOS_VA2PA(MS_VIRT addr)
{
    return (MS_PHY)MsOS_MPool_VA2PA(addr);
}

MS_VIRT MsOS_PA2KSEG0(MS_PHY addr)
{
    return MsOS_MPool_PA2KSEG0(addr);
}

MS_VIRT MsOS_PA2KSEG1(MS_PHY addr)
{
    return MsOS_MPool_PA2KSEG1(addr);
}

MS_PHY MsOS_PA2BA(MS_PHY PhyAddr)
{
    return HAL_MsOS_MPool_PA2BA(PhyAddr);
}

MS_PHY MsOS_BA2PA(MS_PHY BusAddr)
{
    return HAL_MsOS_MPool_BA2PA(BusAddr);
}

// Share memory operation
// MS_BOOL MsOS_SHM_Init(MS_U32 u32ShmSize)
MS_BOOL MsOS_SHM_Init(void)
{
#if !defined (TV_OS)
    key_t key = NULL;
#endif

    MS_BOOL bInit = FALSE;
    MS_U32 u32ShmSize;

#if defined (TV_OS)
    if(pMutexShmIdx < 0)
    {
        pMutexShmIdx = MsOS_CreateNamedMutex((MS_S8*)SHAREMEM_MUTEX_NAME);
        if(pMutexShmIdx < 0)
        {
            MSOS_ERROR("create namedmutex %td fail ", (ptrdiff_t)pMutexShmIdx);
            return FALSE;
        }
    }
    _MSOS_SHM_LOCK(pMutexShmIdx);
#elif defined (MSOS_PROCESS_SAFT_MUTEX)
    if (SEM_FAILED == (pMutexShm = sem_open(SHM_MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1)))
    {
        if (SEM_FAILED == (pMutexShm = sem_open(SHM_MUTEX_NAME, 0)))
        {
            return FALSE;
        }
    }
#else
    _MSOS_SHM_LOCK(pMutexShm);
#endif

    if (-1 != _shm_id)
    {
          goto SHM_INIT_AGAIN;
    }

#if defined (TV_OS)
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
    u32ShmSize = SHM_SIZE;
#else
    u32ShmSize = MsOS_GetSHMSize();
    if(u32ShmSize < SHM_SIZE)
    {
       MSOS_ERROR("[%s][%d] shm size too small\n", __FUNCTION__, __LINE__);
	   _MSOS_SHM_UNLOCK(pMutexShmIdx);
	   LINUX_ASSERT(0);
	   return FALSE;
    }    
#endif
#else
    u32ShmSize = SHM_SIZE;
#endif
    
    u32ShmSize -= SHM_SIZE_RESEVED;
    u32ShmSize += sizeof(MsOS_SHM_Hdr);
    u32ShmSize += ((1<< 12)- 1);
    u32ShmSize = (u32ShmSize>> 12)<< 12; // make it 4KBytes alignment

#if defined (TV_OS)
    _pu8ShareMem = (MS_U8 *)MsOS_Mapping_SharedMem(u32ShmSize,&bInit);
    MSOS_PRINT("~!~mappd sharemem  %s \n", _pu8ShareMem);
    if (_pu8ShareMem == NULL)
    {
        MSOS_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        _MSOS_SHM_UNLOCK(pMutexShmIdx);
        return FALSE;
    }
    _shm_id = (MS_VIRT)_pu8ShareMem;
#else
    key = (key_t)7890;
    if (-1 == (_shm_id = shmget(key, u32ShmSize, IPC_CREAT| IPC_EXCL | 0777)))
    {
        if (-1 == (_shm_id = shmget(key, u32ShmSize, IPC_CREAT)))
        {
            MSOS_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
            _MSOS_SHM_UNLOCK(pMutexShm);
            return FALSE;
        }
    }
    else
    {
        bInit = TRUE;
    }
    if (-1 == (int)(_pu8ShareMem = (MS_U8*)shmat(_shm_id, NULL, 0)))
    {
        MSOS_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        _MSOS_SHM_UNLOCK(pMutexShm);
        return FALSE;
    }
#endif

    if (bInit)
    {
        memset(&_ShmHdr, 0, sizeof(_ShmHdr)); // dummy storage
        _ShmHdr.u32MaxClientNum = MAX_SHM_CLIENT_NUM;
        _ShmHdr.u32ClientNum = 0;
        _ShmHdr.u32ShmSize = u32ShmSize;
        _ShmHdr.u32Offset = (sizeof(MsOS_SHM_Hdr)+7)&~7;
        memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
    }
#if defined (TV_OS)
    _MSOS_SHM_UNLOCK(pMutexShmIdx);
#else
    _MSOS_SHM_UNLOCK(pMutexShm);
#endif
    return TRUE;

SHM_INIT_AGAIN:
#if defined (TV_OS)
    _MSOS_SHM_UNLOCK(pMutexShmIdx);
#else
    _MSOS_SHM_UNLOCK(pMutexShm);
#endif
    return FALSE;
}

#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MsOS_SHM_Init);
#endif

// Share memory operation
MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag)
{
    MsOS_SHM_Context* pContext = NULL;
    MsOS_SHM_Context* pClient = NULL;
    MS_U32 i;
    MS_U32 u32CopyLen;
    MS_U8 pClientName[MAX_CLIENT_NAME_LENGTH];
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
    MS_U32 u32ExpandSize = SHM_EXPAND_SIZE;
    MS_U32 u32ShmMaxSize;
#endif

    if (!_pu8ShareMem)
    {
        MSOS_ERROR("[%s][%d] MsOS_SHM_Init should be invoked first\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (NULL == pu8ClientName)
    {
        return FALSE;
    }
    if (0 == (u32CopyLen = MIN(strlen((const char*)pu8ClientName), (MAX_CLIENT_NAME_LENGTH-1))))
    {
        return FALSE;
    }
    strncpy((char*)pClientName, (const char*)pu8ClientName, u32CopyLen);
    pClientName[u32CopyLen] = '\0';

#if defined (TV_OS)
    _MSOS_SHM_LOCK(pMutexShmIdx);
#else
    _MSOS_SHM_LOCK(pMutexShm);
#endif
    memcpy(&_ShmHdr, _pu8ShareMem, sizeof(_ShmHdr));
    pContext = (MsOS_SHM_Context*)_ShmHdr.context;
    for (i= 0; i< MAX_SHM_CLIENT_NUM; i++, pContext++)
    {
        if (0 == pContext->u32ClientId)
        {
            continue;
        }
        if (0== strcmp((const char*)pContext->u8ClientName, (const char*)pClientName))
        {
            pClient = pContext;
            if (u32BufSize != pClient->u32Size)
            {
                MSOS_ERROR("\n[%s][%d][ClientName %s] Error: inconsistent buffer size with other process\n", __FUNCTION__, __LINE__, (const char*)pClientName);
                LINUX_ASSERT(0); //assert for incompatible utopia lib with shm implement
                return FALSE;
            }
            break;
        }
    }
    if ((NULL == pClient) && (MSOS_SHM_CREATE == u32Flag))
    {
        pContext = (MsOS_SHM_Context*)_ShmHdr.context;
        for (i= 0; i< MAX_SHM_CLIENT_NUM; i++, pContext++)
        {
            if (pContext->u32ClientId)
            {
                continue;
            }
            if ((_ShmHdr.u32Offset + u32BufSize)> _ShmHdr.u32ShmSize)
            {
                MSOS_ERROR("[%s][%d] MsOS_SHM_GetId: shared memory buffer overflow\n", __FUNCTION__, __LINE__);
#if defined (TV_OS)
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
                u32ShmMaxSize = MsOS_GetSHMSize();
                if ((_ShmHdr.u32Offset + u32BufSize) > u32ShmMaxSize)
                {
                    MSOS_ERROR("[%s][%d] exceed the avaiable shared memory size\n", __FUNCTION__, __LINE__);
                    _MSOS_SHM_UNLOCK(pMutexShmIdx);
                    return FALSE;
                }
                // expand shared mem, default add 256k
                if ((_ShmHdr.u32ShmSize + u32ExpandSize) > u32ShmMaxSize)
                {
                    u32ExpandSize = u32ShmMaxSize - (_ShmHdr.u32ShmSize);
                }
                if (FALSE == MsOS_SHM_Expand(_ShmHdr.u32ShmSize, u32ExpandSize))
                {
                    MSOS_ERROR("[%s][%d] expand shared memory fail\n", __FUNCTION__, __LINE__);
                    _MSOS_SHM_UNLOCK(pMutexShmIdx);
                    return FALSE;
                }
                _ShmHdr.u32ShmSize += u32ExpandSize;
                memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
#else
                _MSOS_SHM_UNLOCK(pMutexShmIdx);
                return FALSE;
#endif
#else
                _MSOS_SHM_UNLOCK(pMutexShm);
                return FALSE;
#endif
            }
            _ShmHdr.u32ClientNum++;
            pClient = pContext;
            strncpy((char*)pClient->u8ClientName, (const char*)pClientName,MIN(strlen((const char*)pClientName),(MAX_CLIENT_NAME_LENGTH-1)));
            pClient->u32Size = u32BufSize;
            pClient->u32ClientId = i+ 1;
            pClient->u8RefCnt = 0;
            pClient->u32Offset = _ShmHdr.u32Offset;
            _ShmHdr.u32Offset += (u32BufSize+7)&~7;
            memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
            break;
        }
    }
    if (NULL == pClient)
    {
        // printf("[%s][%d] MsOS_SHM_Init: Unable to get available share memeory\n", __FUNCTION__, __LINE__);
#if defined (TV_OS)
        _MSOS_SHM_UNLOCK(pMutexShmIdx);
#else
        _MSOS_SHM_UNLOCK(pMutexShm);
#endif
        return FALSE;
    }
    *pu32ShmId = pClient->u32ClientId;
    *pu32BufSize = pClient->u32Size;
    *pu32Addr = (MS_VIRT)_pu8ShareMem + pClient->u32Offset;
#if defined (TV_OS)
    _MSOS_SHM_UNLOCK(pMutexShmIdx);
#else
    _MSOS_SHM_UNLOCK(pMutexShm);
#endif

    return TRUE;
}

void* _MsOS_LinuxTaskWrapper (void *argv)
{

    char thd_name[MAX_NAME];
    MsOS_Task_Info * pMsOS_Task_Info= (MsOS_Task_Info *) argv;

    TaskEntry  pTaskEntry;

    MS_VIRT  TaskEntryData= pMsOS_Task_Info -> u32TaskEntryData;
    pTaskEntry = pMsOS_Task_Info -> pTaskEntry;


    //set thread name
    if(strlen(pMsOS_Task_Info->pTaskName) > 15 )
    {
        puts("the length of the thread name exceeds 15, it will be truncated to 15 chars");
    }

    //puts("set thread name");
    memset(thd_name,'\0',sizeof(thd_name));
    snprintf(thd_name, (MAX_NAME - 1),"%s", pMsOS_Task_Info->pTaskName);
    prctl(PR_SET_NAME, (unsigned long)thd_name, NULL, NULL, NULL);

    //get thread name
    memset(thd_name,'\0',sizeof(thd_name));
    prctl(PR_GET_NAME, (unsigned long)thd_name, NULL, NULL, NULL);
    //printf("%s\n",thd_name);

    //execute function
    //printf("%s.%d\n",__FUNCTION__,__LINE__);
    //puts("execute function ");
    pTaskEntry( TaskEntryData , (void *)pMsOS_Task_Info);
    //printf("%s.%d\n",__FUNCTION__,__LINE__);

    //release resource
    //puts("release resource");
    PTH_RET_CHK(pthread_mutex_lock(&_MsOS_Task_Mutex));
    //pStackBase is not used now
    //if (pMsOS_Task_Info->pStackBase)
    //{
          //free(pMsOS_Task_Info->pStackBase);
          //pMsOS_Task_Info->pStackBase= NULL;
    //}
    pMsOS_Task_Info->bUsed = FALSE;
    PTH_RET_CHK(pthread_mutex_unlock(&_MsOS_Task_Mutex));

    return NULL;
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
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    MSOS_WARN("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}
//-------------------------------------------------------------------------------------------------
/// Sync data in EC-Brigde
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void MsOS_Sync(void)
{//not support in current stage.
}

void MsOS_FlushMemory(void)
{
    if(-1==ioctl(_s32FdSYS, IOCTL_SYS_FLUSH_MEMORY, NULL))
    {
        MSOS_ERROR("%s.%d ioctl fail\n",__FUNCTION__,__LINE__);
    }
}

void MsOS_ReadMemory(void)
{
    if(-1==ioctl(_s32FdSYS, IOCTL_SYS_READ_MEMORY, NULL))
    {
        MSOS_ERROR("%s.%d ioctl fail\n",__FUNCTION__,__LINE__);
    }
}

MS_U32 MsOS_GetKattribute(char *pAttr)
{
    FILE *pFile;
    char path[256];
    char str[256];
    MS_U32 u32val, u32Len;

    sprintf(path, "/proc/kattr/%s/value", pAttr);

    if((pFile=fopen(path, "w+"))==NULL)
    {
        MSOS_ERROR("Can not open file at: %s", path);
        return -1;
    }

    fscanf(pFile, "%s", str);
    u32Len = strlen(str);
    u32val = (int)strtol(str, (char **)str+u32Len, 16);

    fclose(pFile);

    return u32val;
}

#endif
