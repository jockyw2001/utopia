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

#include <unistd.h>     //close
#include <fcntl.h>      // open
#include <sys/ioctl.h>  // ioctl
#include <sys/mman.h>
#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include <string.h>
#include <errno.h>
#include <pthread.h>

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
#include "MsOS.h"
#include "utopia.h"
#endif

#if defined (ANDROID)
#include <cutils/log.h>
#endif

#if defined (TV_OS)
#include "mdrv_semutex_io.h"
#include <pthread.h>
#endif

#include "mdrv_cma_pool_private.h"

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#ifdef _Debug
#define MPOOL_DEBUG
#endif

//--------------------------------------------------------------------------------------------------
//  New Internal Variable of MPool Implementation
//--------------------------------------------------------------------------------------------------
#define SUPPORT_PARTIAL_MAPPING 1
static MS_U32		MPOOL_MAPPING;
#define MAX_MAPPINGSIZE 200

#define MMAP_NONCACHE true
#define MMAP_CACHE false

#define ION_ALIGN 0x1000    // align to PAGE_SIZE, remember this ALIGN must <= PAGE_SIZE

#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif
#endif
/*
static MS_BOOL             _Mpool_use[MAX_MAPPINGSIZE];//={-1}
static MS_U8               _Mpool_NonCache[MAX_MAPPINGSIZE];

static void*               _pMpoolAddrVStart[MAX_MAPPINGSIZE];// = { NULL, NULL};
static void*               _pMpoolAddrVEnd[MAX_MAPPINGSIZE];// = { NULL, NULL};
static MS_U32              _u32MpoolAddrPhys[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u64MpoolSize[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u32MPoolVa2PaOff[MAX_MAPPINGSIZE];// = { 0, 0};
*/

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_S32              _s32MPoolFd = -1;
static MS_S32              _s32FdION = -1;
static void*               _pAddrVirtStart[2] = { NULL, NULL};
static void*               _pAddrVirtEnd[2] = { NULL, NULL};
#if (!SUPPORT_PARTIAL_MAPPING)
static MS_SIZE              _tAddrPhys[2] = { 0, 0};
static MS_U32              _u32Size[2] = { 0, 0};
#endif
static MS_SIZE              _tVa2PaOff[2] = { 0, 0};

static MS_VIRT u32MpoolRegBase;

typedef struct
{
    MS_VIRT u64VirtStart;
    MS_VIRT u64VirtEnd;
    MS_U64 u64Phyaddr;
    MS_U64 u64MpoolSize;
    MS_BOOL bIsUsed;
    MS_SIZE s32V2Poff;
    MS_BOOL bNonCache;
    MS_U8 u8MiuSel;
    MS_BOOL bIsDyn;
} MPOOL_INFO;

static MsOSMPool_DbgLevel    _u32MPoolDBGLevel;
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];
static pthread_mutex_t _MsOS_MPool_Mutex = PTHREAD_MUTEX_INITIALIZER;

#define DLMALLOC_DBG 0
#if DLMALLOC_DBG
#ifdef ENABLE_KERNEL_DLMALLOC
#define DLMALLOC_INFO_CNT 10
#define DETAIL_DLMALLOC_INFO_CNT 200

typedef struct {
	void *allocated_start_va;
	size_t allocated_size;
} DETAIL_DLMALLOC_INFO;

typedef struct {
	mspace msp;
	DETAIL_DLMALLOC_INFO detail_dlmalloc_info[DETAIL_DLMALLOC_INFO_CNT];
} DLMALLOC_INFO;

static DLMALLOC_INFO mstar_dlmalloc_info[DLMALLOC_INFO_CNT];
#endif
#endif

#if defined (TV_OS)
static  MS_S32  _s32SemutexFD = -1;
static  void*     _psAddr;
static pthread_mutex_t          _MsOS_Openfd_Mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
#ifdef MPOOL_DEBUG
    #if defined (ANDROID)
    #ifndef LOGI // android 4.1 rename LOGx to ALOGx
    #define LOGI ALOGI
    #endif
    #define MPOOL_ERROR(fmt, args...)           LOGI("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            LOGI("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           LOGI("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #else
    #define MPOOL_ERROR(fmt, args...)           printf("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            printf("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           printf("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #endif

    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); MS_ASSERT(0); }
#else
    #define MPOOL_ERROR(fmt, args...)           do{} while (0)
    #define MPOOL_WARN(fmt, args...)            do{} while (0)
    #define MPOOL_PRINT(fmt, args...)           do{} while (0)
    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); }
#endif

#define MPOOL_IS_CACHE_ADDR(addr)              ( ( ((MS_VIRT)_pAddrVirtStart[0] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[0] > (MS_VIRT)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)            ( ( ((MS_VIRT)_pAddrVirtStart[1] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[1] > (MS_VIRT)addr) ) ? TRUE : FALSE )

#define MPOOL_DBG_MSG(debug_level, x)     do { if (_u32MPoolDBGLevel >= (debug_level)) (x); } while(0)

static MS_S32 _s32MapMutexId = -1;
static MS_U8  _u8MapMutex[] = {"MPool_Mutex"};

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32MPoolFd)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] MPOOL is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}

	return FALSE;
}

MS_BOOL MsOS_MPool_Init(void)
{
    MS_PHY u32size;

	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

    if (0 <= _s32MPoolFd)
    {
        MPOOL_WARN("%s is initiated more than once\n", __FUNCTION__);
        return FALSE;
    }

    memset(mpool_info, 0, sizeof(MPOOL_INFO)*MAX_MAPPINGSIZE);
#if DLMALLOC_DBG
    memset(mstar_dlmalloc_info, 0, sizeof(DLMALLOC_INFO)*DLMALLOC_INFO_CNT);
#endif

    if (0 > (_s32MPoolFd = open("/dev/malloc", O_RDWR)))
    {
        MPOOL_ERROR("Open /dev/malloc fail\n");
        MS_ASSERT(0);
        return FALSE;
    }
	else
	{
	}

    //Create Mutex
    if(_s32MapMutexId < 0)
    {
		MsOS_Init();  // Before MsOS_CreateMutex, need to do MsOS_Init()
        _s32MapMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8MapMutex, MSOS_PROCESS_SHARED);
    }
    if(_s32MapMutexId < 0)
    {
        printf("%s MsOS_CreateMutex failed!!\n", __FUNCTION__);
        return FALSE;
    }

#if defined (TV_OS)
	pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
		_s32SemutexFD = open("/dev/semutex", O_RDWR);
        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
#endif

    if(!MDrv_MMIO_GetBASE(&u32MpoolRegBase, &u32size, MS_MODULE_PM))
    {
        MPOOL_ERROR("%s Get IOMAP Base faill!\n",__FUNCTION__);
        return FALSE;
    }

    return TRUE;
}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MsOS_MPool_Init);
#endif

static MS_BOOL _MPOOL_MutexLock(void)
    {
    if (_s32MapMutexId != -1)
    {
        return MsOS_ObtainMutex(_s32MapMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_ION_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32FdION)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] ION is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}
	else
		printf("\033[35mFunction = %s, Line = %d, ION is not Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);

	return FALSE;
}

MS_BOOL MsOS_ION_Init(void)
{
	printf("\033[35mOpen /dev/ion ...\033[m\n");
    if (0 <= _s32FdION)
    {
        printf("\033[35m ION Fd is opened\033[m\n");
        return FALSE;
    }

    if (0 > (_s32FdION = open("/dev/ion", O_RDWR)))
    {
        printf("\033[35mOpen /dev/ion fail\033[m\n");
        return FALSE;
    }

	printf("\033[35mOpen /dev/ion success\033[m\n");
    return TRUE;
}

static MS_BOOL _MPOOL_MutexUnlock(void)
{
    if (_s32MapMutexId != -1)
    {
        return MsOS_ReleaseMutex(_s32MapMutexId);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_MPool_Close(void)
{
    if (0 > _s32MPoolFd)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    if(_s32MapMutexId != -1)
    {
        MsOS_DeleteMutex(_s32MapMutexId);
        _s32MapMutexId = -1;
    }

    close(_s32MPoolFd);
    _s32MPoolFd = -1;

#if defined (TV_OS)
    pthread_mutex_lock(&_MsOS_Openfd_Mutex);//coverity Data race condition
    if (0 > _s32SemutexFD)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        pthread_mutex_unlock(&_MsOS_Openfd_Mutex);//coverity Data race condition
        return FALSE;
    }
    close(_s32SemutexFD);
    _s32SemutexFD = -1;
    _psAddr = NULL;
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);//coverity Data race condition
#endif

    return TRUE;
}

MS_BOOL MsOS_ION_Close(void)
{
    if(0 > _s32FdION)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    close(_s32FdION);
    _s32MPoolFd = -1;

    return TRUE;
}

#if defined (TV_OS)
//~!~get sharememory size
MS_U32 MsOS_GetSHMSize(void)
{
    MS_U32  u32SHMSize = 0;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);


    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_GET_SHMSIZE, &u32SHMSize))
    {
        MPOOL_ERROR("get SHM size failed!\n");
        MS_ASSERT(0);
        return 0;
    }

    return u32SHMSize;
}

//~!~ expand sharemem
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
MS_BOOL MsOS_SHM_Expand(MS_U32 u32ShmSize, MS_U32 u32ExpandSize)
{
    printf("[msos] expanding shared memory %d\n", u32ExpandSize);
    if(_s32SemutexFD < 0)
    {
        MPOOL_ERROR("%s: mapping sharemem with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }
    u32ExpandSize += u32ShmSize;

    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_EXPAND_SHAREMEMORY,&u32ExpandSize))
    {
        MPOOL_ERROR("create an anonymous sharememory failed! \n");
        close(_s32SemutexFD);
        return FALSE;
    }

    return TRUE;
}
#endif

//~!~ map sharemem
MS_VIRT MsOS_Mapping_SharedMem(MS_U32 u32ShmSize,MS_BOOL *bInit)
{
    //void * pAddr; unused variable - fixed compile warning
    *bInit = FALSE;

    if(_s32SemutexFD < 0)
    {
        MPOOL_ERROR("%s: mapping sharemem with invalid dev fd \n", __FUNCTION__);
        //return NULL; fixed compile warning
        return 0;
    }

//!!!
//there is a limitation, you must map sharemem first with the most largest len!
//that is , if you map with len A, then map with B(B > A), problem happens!!!

    if(_psAddr)
    {
        MPOOL_ERROR("~!~ 2 MsOS_Mapping_SharedMem = %p \n",_psAddr);
        return (MS_VIRT)_psAddr;
    }

    MPOOL_ERROR("~!~!~%s: pid = %d \n", __FUNCTION__,getpid());

    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_SHAREMEMORY,&u32ShmSize))
    {
        MPOOL_ERROR("create an anonymous sharememory failed! \n");
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    *bInit = ioctl(_s32SemutexFD,MDRV_SEMUTEX_QUERY_ISFIRST_SHAREMEMORY,NULL);
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
    _psAddr = mmap(NULL,MsOS_GetSHMSize(),PROT_READ | PROT_WRITE,MAP_SHARED,_s32SemutexFD,0);
#else
    _psAddr = mmap(NULL,u32ShmSize,PROT_READ | PROT_WRITE,MAP_SHARED,_s32SemutexFD,0);
#endif

    //LOGE("u32ShmSize= %d, _s32SemutexFD = %d, pAddr= %p \n",u32ShmSize,_s32SemutexFD,_psAddr);

    if (_psAddr == MAP_FAILED)
    {
        MPOOL_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    //LOGE("~!~ 1 MsOS_Mapping_SharedMem = %p \n",(MS_U32)_psAddr);

    //*bInit = TRUE;
    return (MS_VIRT)_psAddr;

}

//~!~create namedmutex
MS_S32 MsOS_CreateNamedMutex(MS_S8 *ps8MutexName)
{
    MS_S32  s32Index;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);

#if 0
    CREATE_MUTEX_ARG createmutex;
    createmutex.pnameaddr = (unsigned long)ps8MutexName;
    createmutex.length = strlen(ps8MutexName);
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,&createmutex);
#else
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,ps8MutexName);
#endif

    if(s32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("the returned mutex index invalid!!! \n");
        return -1;
    }

    //s32Index -= MUTEX_INDEX_BEGIN;

    return s32Index;
}

//~!~lock mutex
MS_BOOL MsOS_LockMutex(MS_S32 u32Index,MS_S32 flag)
{
    MS_S32 res = 0;
    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    if(flag == 0)
    {
        res = -1;
        while(res == -1)
        {
            res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK,&u32Index);
            if(res == -1 && errno == EINTR)
                usleep(1000);
        }
    }
    else if(flag == -1)
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_TRY_LOCK,&u32Index);
    else
    {
        LOCKARG lockarg;
        lockarg.index = u32Index;
        lockarg.time = flag;
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK_WITHTIMEOUT,&lockarg);
    }
    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


//~!~lock mutex
MS_BOOL MsOS_UnlockMutex(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index  : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_UNLOCK,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_CrossThreadUnlockMutex(MS_S32 u32Index, MS_BOOL bEnable)
{
    MS_S32 res = 0;
    CROSS_THREAD_UNLOCK_INFO unlock_info = {0};

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }
    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index  : u32Index = %d \n", __FUNCTION__,u32Index);
        return FALSE;
    }
    unlock_info.index = u32Index;
    unlock_info.flag = (TRUE == bEnable)?E_CROSS_THREAD_UNLOCK_ENABLE:E_CROSS_THREAD_UNLOCK_DISABLE;
    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_SET_CROSS_THREAD_UNLOCK,&unlock_info);
    if(res == 0)
        return TRUE;
    else
    {
        perror("ioctl MDRV_SEMUTEX_SET_CROSS_THREAD_UNLOCK");
        return FALSE;
    }
}

//~!~lock mutex
MS_BOOL MsOS_DeleteNamedMutexbyIndex(MS_S32 u32Index)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index \n", __FUNCTION__);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_DEL_MUTEX,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}
#endif

//~!~create named semaphore
MS_S32 MsOS_CreateNamedSemaphore(char *ps8SemaName, MS_U32 u32SemaNum)
{
    CREATE_SEMA_ARG semaarg;
    MS_S32  s32Index;
    MS_U32  u32MaxLen;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);

    if (NULL == ps8SemaName)
        return -1;
    if (strlen((const char *)ps8SemaName) >= (SEMA_NAME_LEN-1))
    {
        MPOOL_WARN("%s: Warning strlen(%s) is longer than SEMA_NAME_LEN(%d). Oversize char will be discard.\n",
            __FUNCTION__, ps8SemaName, SEMA_NAME_LEN);
    }
    if (0 == (u32MaxLen = MIN(strlen((const char *)ps8SemaName), (SEMA_NAME_LEN-1))))
    {
        MPOOL_ERROR("%s: Input string is empty.\n", __FUNCTION__);
        return -1;
    }
    strncpy((char*)semaarg.semaname, (const char*)ps8SemaName, u32MaxLen);
    semaarg.semaname[u32MaxLen] = '\0';
    semaarg.semanum = u32SemaNum;

    s32Index = ioctl(_s32SemutexFD, MDRV_SEMUTEX_CREATE_SEMAPHORE, &semaarg);

    if(s32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s the returned mutex index %d invalid!!!\n", __FUNCTION__, s32Index);
        return -1;
    }

    return s32Index;
}

MS_BOOL MsOS_ResetNamedSemaphore(MS_S32 u32Index)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Reset semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_RESET, &u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_ObtainNamedSemaphore(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Obtain semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    if(flag == -1)
        res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_TRY_LOCK, &u32Index);
    else
    {
        res = -1;
        while(res == -1)
        {
            res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_LOCK, &u32Index);
            if(res == -1 && errno == EINTR)
                usleep(1000);
            else
            {
                MPOOL_ERROR("%s lock semaphore error: %d.\n", __FUNCTION__, errno);
                break;
            }
        }
    }

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_ReleaseNamedSemaphore(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Obtain semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_UNLOCK, &u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
#if(!SUPPORT_PARTIAL_MAPPING)
    if(MPOOL_MAPPING) return TRUE; //we had already mapped by function MsOS_MPool_Mapping()

    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32            bCache = (bNonCache) ? 0: 1;
    MS_U32                  u32Idx = (bCache) ? 0: 1;

    if (NULL == _pAddrVirtStart[u32Idx])
    {
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_INFO, &stPoolInfo))
        {
            return FALSE;
        }
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, stPoolInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }

        _pAddrVirtStart[u32Idx] = (void*)ptrAddrVirt;
        _pAddrVirtEnd[u32Idx]   = (void*)(ptrAddrVirt + stPoolInfo.u32Size);
        _tAddrPhys[u32Idx]    = stPoolInfo.u32Addr;
        _u32Size[u32Idx]        = stPoolInfo.u32Size;
        _tVa2PaOff[u32Idx]    = (MS_SIZE)_pAddrVirtStart[u32Idx] - (MS_SIZE)_tAddrPhys[u32Idx];
    }

    if (ppAddrVirt)
    {
        *ppAddrVirt   = _pAddrVirtStart[u32Idx];
    }
    if (pu32AddrPhys)
    {
        *pu32AddrPhys = _tAddrPhys[u32Idx];
    }
    if (pu32Size)
    {
        *pu32Size     = _u32Size[u32Idx];
    }
#endif
    return TRUE;
}

static void _MPool_Check_aligned(MS_U64 u64Offset, MS_U64 u64MapSize)
{
    if(u64Offset&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping address is not aligned, please make sure of that\n");
        MS_ASSERT(0);
		//assert(0);
    }

    if(u64MapSize&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping size is not aligned, please make sure of that\n");
		MS_ASSERT(0);
        //assert(0);
    }

    return;
}

static MS_U8 _MPOOL_DELAY_BINDING(MS_S32 idx)
{
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_U64                  u64AddrVirt;
    MS_U32            bCache = (mpool_info[idx].bNonCache) ? 0: 1;

	MS_U8 	u8Check_MiuSel = 0;
	MS_U64 	u64Check_Offset = 0;

    stPoolInfo.u64Addr = mpool_info[idx].u64Phyaddr;
    stPoolInfo.u64Size = mpool_info[idx].u64MpoolSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = mpool_info[idx].u8MiuSel;

	/* use u64Phyaddr to get MIU, offset */
	_phy_to_miu_offset(u8Check_MiuSel, u64Check_Offset, stPoolInfo.u64Addr);

	if(u8Check_MiuSel != stPoolInfo.u8MiuSel)
	{
		printf("\033[35mFunction = %s, Line = %d, [Error] miu_setting is wrong\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return FALSE;
	}
	stPoolInfo.u64Addr = u64Check_Offset;

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (u64AddrVirt = (MS_VIRT)mmap(0, mpool_info[idx].u64MpoolSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = u64AddrVirt;
    mpool_info[idx].u64VirtEnd = (u64AddrVirt + mpool_info[idx].u64MpoolSize);

	mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
    if (MPOOL_IS_CACHE_ADDR(pAddrVirt))
    {
        return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[0]);
    }

    if (MPOOL_IS_UNCACHE_ADDR(pAddrVirt))
    {
       return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[1]);
    }

    // we currently will go to here
    MS_S32 i;
    for(i = 0; i <MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed)
        {
            if(!((mpool_info[i].u64VirtStart <= pAddrVirt) && (pAddrVirt < mpool_info[i].u64VirtEnd)))
                continue;


            return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt, mpool_info[i].s32V2Poff);
        }
    }
	#if defined (__aarch64__)
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%lx) TO PA(%x)\n", pAddrVirt,0);
	#else
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%tx) TO PA(%x)\n", (ptrdiff_t)pAddrVirt,0);
	#endif
    return (MS_PHY) 0xFFFFFFFFUL;
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys) // un-cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[1])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE;i ++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(!mpool_info[i].bNonCache)
            {
                continue;
            }

	        // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
	        if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
	        {
	            continue;
	        }

            if(mpool_info[i].bIsDyn)
            {
                _MPOOL_MutexLock();
	            if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
                {
                    if(!_MPOOL_DELAY_BINDING(i))
                    {
						#if defined (__aarch64__)
                        printf("PA2KSEG1 delay binding fail!, pa = %lx", mpool_info[i].u64Phyaddr);
						#else
                        printf("PA2KSEG1 delay binding fail!, pa = %llx", mpool_info[i].u64Phyaddr);
						#endif
						_MPOOL_MutexUnlock();
                        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
						pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,true);
						if(pAddrVirt )
							return pAddrVirt;

						//printf("%s:%d  \n",__FUNCTION__,__LINE__);						
			            return (MS_VIRT)NULL;
                    }
		        }
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
            else
            {
                //printf("%s:%d  \n",__FUNCTION__,__LINE__);
				pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,true);
				if(pAddrVirt )
					return pAddrVirt;
            }			
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG1 return NULL pAddrPhys =%llX\n", (unsigned long long)pAddrPhys));

        //assert(0);
        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
        pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,true);
        if(pAddrVirt )
            return pAddrVirt;

        //printf("%s:%d  \n",__FUNCTION__,__LINE__);        
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[1], printf("MsOS_MPool_Init/MsOS_MPool_Get for un-cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    if(pAddrVirt == 0)
    {
        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
		pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,true);
		if(pAddrVirt )
			return pAddrVirt;
    }

    return pAddrVirt;
}

inline MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY pAddrPhys) // cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[0])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE; i++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(mpool_info[i].bNonCache)
            {
                continue;
            }

    	    // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
    	    if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
    	    {
    		    continue;
    	    }

            if(mpool_info[i].bIsDyn)
            {
    	        _MPOOL_MutexLock();
    	        if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
        	    {
        		    if(!_MPOOL_DELAY_BINDING(i))
    	            {
    	                printf("PA2KSEG0 delay binding fail!, pa = %tX", (ptrdiff_t)mpool_info[i].u64Phyaddr);

            			_MPOOL_MutexUnlock();
                        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
						pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,false);
						if(pAddrVirt )
							return pAddrVirt;
						//printf("%s:%d  \n",__FUNCTION__,__LINE__);						
                        return (MS_VIRT)NULL;
    	            }
    	        }
                //mpool_info[i].bIsDyn = false    // joe, need to be discussed
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
            else
			{
			    //printf("%s:%d  \n",__FUNCTION__,__LINE__);
				pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,false);
				if(pAddrVirt )
					return pAddrVirt;
            }			
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG0 return NULL\n"));
        //assert(0);
        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
		pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,false);
		if(pAddrVirt )
			return pAddrVirt;

        //printf("%s:%d  \n",__FUNCTION__,__LINE__);        
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[0], printf("MsOS_MPool_Init/MsOS_MPool_Get for cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    if(pAddrVirt == 0)
    {
        //printf("%s:%d  \n",__FUNCTION__,__LINE__);
		pAddrVirt = MApi_CMA_Pool_GetKernelCMAPooLUserVA(pAddrPhys,false);
        if(pAddrVirt )
			return pAddrVirt;
    }

    return pAddrVirt;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush_All()
{
    if (HAL_MsOS_MPool_Dcache_Flush_All(_s32MPoolFd) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_VIRT pAddrVirt, MS_SIZE tSize)
{
    MS_PHY pAddrPhys;

    pAddrPhys = MsOS_MPool_VA2PA(pAddrVirt);

    /* to prevent the va is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
    if(pAddrPhys == 0xFFFFFFFF)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    /* to prevent the va_end is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
	if( MsOS_MPool_VA2PA(pAddrVirt + tSize) == 0xFFFFFFFF )
	{
		MsOS_MPool_Dcache_Flush_All();
		return TRUE;
	}

    if (HAL_MsOS_MPool_Dcache_Flush(_s32MPoolFd, pAddrVirt, tSize, pAddrPhys) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}


MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_U8 u8MapMode) // 0:cache 1: noncache
{
    MS_S32 i,idx = 0;
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32      bCache = 0; //(bNonCache) ? 0: 1;
    MS_U64 Phyaddr, PhyaddrEnd;
    MS_BOOL     bNonCache =0;

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_CACHE_KERNEL )
    {
        bNonCache = 0;
        bCache = 1;
    }else
    {
        bNonCache =1;
        bCache = 0;
    }

    _miu_offset_to_phy(u8MiuSel, tOffset, Phyaddr);
    PhyaddrEnd = Phyaddr + tMapSize;

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
    {
    stPoolInfo.u64Addr = tOffset;
    stPoolInfo.u64Size = tMapSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = u8MiuSel;

    _MPool_Check_aligned(tOffset, tMapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %lX, u32MapSize = %lX, u8MapMode = %02x\n", (unsigned long)tOffset, (unsigned long)tMapSize, u8MapMode));

    pthread_mutex_lock(&_MsOS_MPool_Mutex);
    // check mmap table to avoid duplicated mmap for same start_addr and end_addr
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if (mpool_info[i].bIsUsed == false)
        {
            continue;
        }
        else
        {
            if (bNonCache != mpool_info[i].bNonCache || u8MiuSel != mpool_info[i].u8MiuSel)
                continue;

			if ( (mpool_info[i].u64Phyaddr == Phyaddr) && ((mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize) == PhyaddrEnd) )
            {
                //printf("[User space] Duplicated PA(0x%llx ~ 0x%llx) mapping with Table[%ld](0x%llx ~0x%llx).\n",
                  //       Phyaddr, PhyaddrEnd, i, mpool_info[i].u64Phyaddr, (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize));
				pthread_mutex_unlock(&_MsOS_MPool_Mutex);
                return TRUE;
            }
        }
    }

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            idx = i;
            break;
        }
    }
    if(i >= MAX_MAPPINGSIZE)
    {
        MPOOL_ERROR("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    //prevent race condition cause mpool mapping size modified in the kernel layer

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, tMapSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }
    mpool_info[idx].bIsUsed = true;
    if(bNonCache)
    {
        mpool_info[idx].bNonCache = MMAP_NONCACHE;
    }
    else
    {
        mpool_info[idx].bNonCache = MMAP_CACHE;
    }
    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = ptrAddrVirt;
    mpool_info[idx].u64VirtEnd = (ptrAddrVirt + tMapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;

    _miu_offset_to_phy(u8MiuSel, tOffset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = tMapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    MPOOL_MAPPING = 1;
    }

    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    if(u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
        return TRUE;


    MS_S32              _s32UtopiaFd = -1;
    KDrvMPool_Info_t    stPoolInfoKernel;

    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");

    }else
    {
        stPoolInfoKernel.u32Addr = tOffset;
        stPoolInfoKernel.u32Size = tMapSize;
        stPoolInfoKernel.u32Interval = MIU_INTERVAL;
        stPoolInfoKernel.u8MiuSel = u8MiuSel;
        stPoolInfoKernel.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfoKernel))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");

        }
        close(_s32UtopiaFd);//coverity resource leak
    }
    #endif

    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_U8 u8MapMode)
{
    MS_S32 i,idx = 0;
    MS_U64 Phyaddr, PhyaddrEnd;

    MS_BOOL     bNonCache =0;


    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_CACHE_KERNEL )
    {
        bNonCache = 0;
    }else
    {
        bNonCache =1;
    }
    _miu_offset_to_phy(u8MiuSel, u32Offset, Phyaddr);
    PhyaddrEnd = Phyaddr + u32MapSize;

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    KDrvMPool_Info_t  stPoolInfo;

    MS_S32              _s32UtopiaFd = -1;
    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_KERNEL || u8MapMode == MSOS_NON_CACHE_KERNEL)
    {


    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");
    }
	else
    {
        stPoolInfo.u32Addr = u32Offset;
        stPoolInfo.u32Size = u32MapSize;
        stPoolInfo.u32Interval = MIU_INTERVAL;
        stPoolInfo.u8MiuSel = u8MiuSel;
        stPoolInfo.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfo))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");
        }
    }
    }
    #endif

    _MPool_Check_aligned(u32Offset, u32MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %tX, u32MapSize = %tX, u8MapMode = %02x\n", (ptrdiff_t)u32Offset, (ptrdiff_t)u32MapSize, u8MapMode));

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
    {

    // check mmap table to avoid duplicated mmap for same start_addr and end_addr
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if (mpool_info[i].bIsUsed == false)
        {
            continue;
        }
        else
        {
            if (bNonCache != mpool_info[i].bNonCache || u8MiuSel != mpool_info[i].u8MiuSel)
                continue;

			if ( (mpool_info[i].u64Phyaddr == Phyaddr) && ((mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize) == PhyaddrEnd) )
            {
                //printf("[User space] Duplicated PA(0x%llx ~ 0x%llx) mapping with Table[%ld](0x%llx ~0x%llx).\n",
                  //       Phyaddr, PhyaddrEnd, i, mpool_info[i].u64Phyaddr, (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize));
                return TRUE;
            }
        }
    }

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            idx = i;
            break;
        }
    }
    if(i >= MAX_MAPPINGSIZE)
    {
        MPOOL_ERROR("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }

    mpool_info[idx].bIsUsed = true;
    if(bNonCache)
    {
        mpool_info[idx].bNonCache = MMAP_NONCACHE;
    }
    else
    {
        mpool_info[idx].bNonCache = MMAP_CACHE;
    }
    mpool_info[idx].u64VirtStart = 0;
    mpool_info[idx].u64VirtEnd = 0;
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = true;

    _miu_offset_to_phy(u8MiuSel, u32Offset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = u32MapSize;
    mpool_info[idx].s32V2Poff = 0;

    MPOOL_MAPPING = 1;
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("MPOOL_MAPPING =%d\n",MPOOL_MAPPING));//coverity MPOOL_MAPPING unused
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    if(0 <= _s32UtopiaFd)
        close(_s32UtopiaFd);//coverity resource leak
#endif
    return TRUE;
}

MS_BOOL MsOS_MPool_UnMapping(MS_VIRT ptrVirtStart, MS_SIZE tMapSize)
{
    MS_S32 i;

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == true)
        {
            if( (mpool_info[i].u64VirtStart == ptrVirtStart) && (mpool_info[i].u64MpoolSize == tMapSize) )
            {
                if( !munmap((MS_VIRT *)mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize) )
                {
                	printf("mpool[%td] Unmapping PA:[%tX], VA:[%tX], Size:[%tX] done.\n", (ptrdiff_t)i, (ptrdiff_t)mpool_info[i].u64Phyaddr, (ptrdiff_t)mpool_info[i].u64VirtStart, (ptrdiff_t)mpool_info[i].u64MpoolSize);
					mpool_info[i].bIsUsed = false;
                    mpool_info[i].u64Phyaddr = 0;
                    mpool_info[i].u64VirtStart = 0;
                    mpool_info[i].u64VirtEnd = 0;
                    mpool_info[i].u64MpoolSize = 0;
                    mpool_info[i].s32V2Poff = 0;

                    /* cause default value is 0 */
                    mpool_info[i].bNonCache = 0;
                    mpool_info[i].u8MiuSel = 0;
                    mpool_info[i].bIsDyn = false;
                }
                else
                {
                    printf("Unmapping PA:[%tX], VA:[%tX], Size:[%tX] fail.\n", (ptrdiff_t)mpool_info[i].u64Phyaddr, (ptrdiff_t)mpool_info[i].u64VirtStart, (ptrdiff_t)mpool_info[i].u64MpoolSize);

                    break;
                }

                return TRUE;
            }
        }
    }
    printf("Unmapping Fail !! VA:[%tX], Size:[%tX].\n", (ptrdiff_t)ptrVirtStart, (ptrdiff_t)tMapSize);

    return FALSE;
}

MS_BOOL MsOS_MPool_Kernel_Detect(MS_PHY *lx_addr, MS_U64 *lx_size, MS_PHY *lx2_addr, MS_U64 *lx2_size)
{
    DevMalloc_MPool_Kernel_Info_t kernel_info;
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_KERNELDETECT, &kernel_info))
    {
        return FALSE;
    }

   *lx_addr = kernel_info.u64lxAddr;
   *lx_size = kernel_info.u64lxSize;
   *lx2_addr = kernel_info.u64lx2Addr-0x40000000;
   *lx2_size = kernel_info.u64lx2Size;

   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx_addr = %lx, lx_size = %lx\n", (MS_U64)lx_addr, lx_size));
   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx2_addr = %lx, lx2_size = %lx\n", (MS_U64)lx2_addr, lx2_size));

  return TRUE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
    _u32MPoolDBGLevel = DbgLevel;
}

MS_BOOL MsOS_MPool_SetWatchPT(MS_VIRT ptrAddrVirt, MS_U32 u32ASID, MS_U8 u8Global, MS_U8 u8WType, MS_U32 u32Mask)
{
	DevMalloc_MPool_Watchpt_Info_t stWPTInfo;
	stWPTInfo.mask = u32Mask;
	stWPTInfo.u64AddrVirt = ptrAddrVirt;
	stWPTInfo.ASID = u32ASID;
	stWPTInfo.global = u8Global;
	stWPTInfo.rwx = u8WType;
	if(ioctl(_s32MPoolFd, MALLOC_IOC_SETWATCHPT, &stWPTInfo)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("this is api test after ioctl by york\n");
	return TRUE;
}

MS_BOOL MsOS_MPool_GetWatchPT(char *str)
{
	printf("Read watchpoint register\n");
	if(ioctl(_s32MPoolFd, MALLOC_IOC_GETWATCHPT, str)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("%s\n",str);
	return TRUE;
}

MS_BOOL MsOS_MPool_Add_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL find = FALSE;
    MS_U64 u64AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_U32 i, idx = 0;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    //for multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;
            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            find = TRUE;

            break;
        }
    }

    if(!find)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    _phy_to_miu_offset(u8MiuSel, u64AddrOffset, u64PhysAddr);   // get miu & offset
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64AddrOffset =0x%x\n",(unsigned int)u64AddrOffset));//coverity u64AddrOffset unused

    mpool_info[idx].u64VirtStart = u64VirtAddr;
    mpool_info[idx].u64VirtEnd = (u64VirtAddr + u64MapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;
    mpool_info[idx].u64Phyaddr = u64PhysAddr;
    mpool_info[idx].u64MpoolSize = u64MapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    MPOOL_MAPPING = 1;
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("MPOOL_MAPPING =%d\n",MPOOL_MAPPING));//coverity MPOOL_MAPPING unused
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    return TRUE;
}

MS_BOOL MsOS_MPool_Remove_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL ret = FALSE, mNonCache = MMAP_NONCACHE;
    MS_U32 i;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    if(bNonCache)
        mNonCache= MMAP_NONCACHE;
    else
        mNonCache= MMAP_CACHE;

    //multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if((mpool_info[i].bIsUsed == false) || (mpool_info[i].bNonCache != mNonCache))
            continue;

        if((mpool_info[i].u64VirtStart == u64VirtAddr)
            && (mpool_info[i].u64Phyaddr == u64PhysAddr)
            && (mpool_info[i].u64MpoolSize == u64MapSize))
        {
             ret = TRUE;
             memset(&mpool_info[i], 0, sizeof(mpool_info[i]));
             break;
        }
    }

    pthread_mutex_unlock(&_MsOS_MPool_Mutex);
    return ret;
}

#ifdef ENABLE_KERNEL_DLMALLOC
mspace MsOS_MPool_CreateMemoryPool(void *base, size_t capacity, int locked)
{
#if DLMALLOC_DBG
	int first_free_dlmalloc_info_index = DLMALLOC_INFO_CNT;
	int i = 0;
#endif
	MS_PHY pa_start = MsOS_MPool_VA2PA((MS_VIRT)base);
	MS_PHY ba_start = HAL_MsOS_MPool_PA2BA(pa_start);
	printf("\033[31mFunction = %s, Line = %d, VA: from 0x%lX to 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)base, ((unsigned long)base+capacity));
	printf("\033[31mFunction = %s, Line = %d, PA: from 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, pa_start);
	printf("\033[31mFunction = %s, Line = %d, BA: from 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, ba_start);
	DevMalloc_MPool_Dlmalloc_Info_t dlmalloc_info;
	dlmalloc_info.user_va = base;
	dlmalloc_info.bus_addr = ba_start;
	dlmalloc_info.capacity = capacity;
	dlmalloc_info.locked = locked;

	//MsOS_DelayTask(100);
	if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_DLMALLOC_CREATE_POOL, &dlmalloc_info))
	{
		sleep(1);
        printf("create a mpool memory pool failed!\n");
        return 0;
	}
	//MsOS_DelayTask(100);

	// we can not use pa2va, because we donot have cache/non-cache infomation, so we use the offset to calculate the va
	dlmalloc_info.MemoryPool = (mspace)((unsigned long)(dlmalloc_info.pool_bus_addr - ba_start) + (unsigned long)base);
	printf("\033[31mFunction = %s, Line = %d, get MemoryPool @ ba: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_info.pool_bus_addr);
	printf("\033[31mFunction = %s, Line = %d, get MemoryPool @ va: 0x%lX\033[m\n\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)dlmalloc_info.MemoryPool);

#if DLMALLOC_DBG
	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == dlmalloc_info.MemoryPool)
			break;

		if( (mstar_dlmalloc_info[i].msp == 0) && (first_free_dlmalloc_info_index == DLMALLOC_INFO_CNT) )
			first_free_dlmalloc_info_index = i;
	}

	if(i == DLMALLOC_INFO_CNT)	// we can not find dlmalloc_info.MemoryPool @ mstar_dlmalloc_info, insert it
	{
		if(first_free_dlmalloc_info_index == DLMALLOC_INFO_CNT)	// the mstar_dlmalloc_info is full, die
		{
			printf("\033[35mFunction = %s, Line = %d, no space to insert mstar_dlmalloc_info, die\033[m\n", __PRETTY_FUNCTION__, __LINE__);
			*(volatile int *)0x1 = 0x5566;
		}
		printf("\033[31mFunction = %s, Line = %d, insert dlmalloc_info.MemoryPool: 0x%lX to mstar_dlmalloc_info[%d]\033[m\n\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_info.MemoryPool, first_free_dlmalloc_info_index);
		mstar_dlmalloc_info[first_free_dlmalloc_info_index].msp = dlmalloc_info.MemoryPool;
	}
#endif

	return dlmalloc_info.MemoryPool;
}

size_t MsOS_MPool_DeleteMemoryPool(mspace msp)
{
#if DLMALLOC_DBG
	int i = 0;
#endif
	DevMalloc_MPool_Dlmalloc_Delete_Info_t dlmalloc_delete_info;
	MS_PHY pa_pool = MsOS_MPool_VA2PA((MS_VIRT)msp);
	MS_PHY ba_pool = HAL_MsOS_MPool_PA2BA(pa_pool);

	dlmalloc_delete_info.pool_bus_addr = ba_pool;
	printf("\033[35mFunction = %s, Line = %d, release VA: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)msp);
	printf("\033[35mFunction = %s, Line = %d, relase PA: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, pa_pool);
	printf("\033[35mFunction = %s, Line = %d, release BA: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, ba_pool);

	//MsOS_DelayTask(100);
	if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_DLMALLOC_DELETE_POOL, &dlmalloc_delete_info))
	{
		sleep(1);
        printf("delete a mpool memory pool failed!\n");
        return 0;
	}
	//MsOS_DelayTask(100);

#if DLMALLOC_DBG
	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == msp)
			break;
	}

	if(i == DLMALLOC_INFO_CNT)	// we can not find msp @ mstar_dlmalloc_info, die
	{
		printf("\033[35mFunction = %s, Line = %d, no mstar_dlmalloc_info match for msp: 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, msp);
		*(volatile int *)0x2 = 0x5566;
	}
	else
		memset(&mstar_dlmalloc_info[i], 0, sizeof(mstar_dlmalloc_info[i]));
#endif

	return 0;
}

void * MsOS_MPool_AllocateMemory(mspace msp, size_t bytes)
{
#if DLMALLOC_DBG
	int first_free_detail_dlmalloc_info_index = DETAIL_DLMALLOC_INFO_CNT;
	int i,j = 0;
#endif
	DevMalloc_MPool_Dlmalloc_Alloc_Free_Info_t dlmalloc_alloc_free_info;
	void *alloc_return_va = 0;
	MS_PHY pa_pool = MsOS_MPool_VA2PA((MS_VIRT)msp);
	MS_PHY ba_pool = HAL_MsOS_MPool_PA2BA(pa_pool);

	dlmalloc_alloc_free_info.pool_bus_addr = ba_pool;
	dlmalloc_alloc_free_info.alloc_size = bytes;
	//printf("\033[35mFunction = %s, Line = %d, alloc from pool ba: 0x%llX, size: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.pool_bus_addr, dlmalloc_alloc_free_info.alloc_size);

	//MsOS_DelayTask(100);
	if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_DLMALLOC_ALLOC_POOL_MEMORY, &dlmalloc_alloc_free_info))
	{
		sleep(1);
        printf("alloc memory from mpool failed!\n");
        return 0;
	}
	//MsOS_DelayTask(100);

	// we can not use pa2va, because we donot have cache/non-cache infomation, so we use the offset to calculate the va
	if(dlmalloc_alloc_free_info.alloc_addr)
		alloc_return_va = (void *)((unsigned long)(dlmalloc_alloc_free_info.alloc_addr - ba_pool) + (unsigned long)msp);
	else
		alloc_return_va = 0;
	//printf("\033[35mFunction = %s, Line = %d, get memory @ ba: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.alloc_addr);
	//printf("\033[35mFunction = %s, Line = %d, alloc_return_va is 0x%lX, size: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, alloc_return_va, bytes);

#if DLMALLOC_DBG
	if(alloc_return_va == 0)
		return alloc_return_va;

	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == msp)
			break;
	}

	if(i == DLMALLOC_INFO_CNT)
	{
		printf("\033[35mFunction = %s, Line = %d, no mstar_dlmalloc_info match for msp: 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, msp);
		for(i = 0; i < DLMALLOC_INFO_CNT; i++)
			printf("\033[35mFunction = %s, Line = %d, mstar_dlmalloc_info[%d].msp is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, i, mstar_dlmalloc_info[i].msp);
		sleep(3);
		*(volatile int *)0x3 = 0x5566;
	}
	else
	{
		for(j = 0; j < DETAIL_DLMALLOC_INFO_CNT; j++)
		{
			if(mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va == alloc_return_va)
			{
				printf("\033[35mFunction = %s, Line = %d, the alloc_return_va: 0x%lX is already allocated, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, alloc_return_va);
				*(volatile int *)0x4 = 0x5566;
			}

			if( (mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va == 0) && (first_free_detail_dlmalloc_info_index == DETAIL_DLMALLOC_INFO_CNT) )
				first_free_detail_dlmalloc_info_index = j;
		}

		if(first_free_detail_dlmalloc_info_index == DETAIL_DLMALLOC_INFO_CNT)
		{
			printf("\033[35mFunction = %s, Line = %d, no space to insert detail_dlmalloc_info, die (having %d data)\033[m\n", __PRETTY_FUNCTION__, __LINE__, DETAIL_DLMALLOC_INFO_CNT);
			*(volatile int *)0x5 = 0x5566;
		}
		else
		{
			mstar_dlmalloc_info[i].detail_dlmalloc_info[first_free_detail_dlmalloc_info_index].allocated_start_va = alloc_return_va;
			mstar_dlmalloc_info[i].detail_dlmalloc_info[first_free_detail_dlmalloc_info_index].allocated_size = bytes;
		}
	}
#endif

	return alloc_return_va;
}

void MsOS_MPool_FreeMemory(mspace msp, void *free_start_addr)
{
#if DLMALLOC_DBG
	int i,j = 0;
#endif
	DevMalloc_MPool_Dlmalloc_Alloc_Free_Info_t dlmalloc_alloc_free_info;
	MS_PHY pa_pool = MsOS_MPool_VA2PA((MS_VIRT)msp);
	MS_PHY ba_pool = HAL_MsOS_MPool_PA2BA(pa_pool);

	MS_PHY pa_free_start_addr = MsOS_MPool_VA2PA((MS_VIRT)free_start_addr);
	MS_PHY ba_free_start_addr = HAL_MsOS_MPool_PA2BA(pa_free_start_addr);

	dlmalloc_alloc_free_info.alloc_addr = ba_free_start_addr;
	dlmalloc_alloc_free_info.pool_bus_addr = ba_pool;
	//printf("\033[31mFunction = %s, Line = %d, free from pool ba: 0x%llX, free_addr: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.pool_bus_addr, dlmalloc_alloc_free_info.alloc_addr);

#if DLMALLOC_DBG
	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == msp)
		{
			for(j = 0; j < DETAIL_DLMALLOC_INFO_CNT; j++)
			{
				if(mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va == free_start_addr)
				{
					printf("\033[31mFunction = %s, Line = %d, free size is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_size);
				}
			}
			break;
		}

		printf("\033[31mFunction = %s, Line = %d, i is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, i);
		*(volatile int *)0x10 = 0x5566;
	}
#endif

	//MsOS_DelayTask(100);
	if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_DLMALLOC_FREE_POOL_MEMORY, &dlmalloc_alloc_free_info))
	{
		sleep(1);
        printf("alloc memory from mpool failed!\n");
        return;
	}
	//MsOS_DelayTask(100);

#if DLMALLOC_DBG
	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == msp)
			break;
	}

	if(i == DLMALLOC_INFO_CNT)
	{
		printf("\033[31mFunction = %s, Line = %d, no mstar_dlmalloc_info match for msp: 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, msp);
		*(volatile int *)0x6 = 0x5566;
	}
	else
	{
		for(j = 0; j < DETAIL_DLMALLOC_INFO_CNT; j++)
		{
			if(mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va == free_start_addr)
			{
				mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va = 0;
				mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_size = 0;
				break;
			}
		}

		if(j == DETAIL_DLMALLOC_INFO_CNT)
		{
			printf("\033[31mFunction = %s, Line = %d, no detail_dlmalloc_info can be delete for free_start_addr 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, free_start_addr);
			*(volatile int *)0x7 = 0x5566;
		}
	}
#endif

	return;
}

void * MsOS_MPool_ReallocateMemory(mspace msp, void *old_alloc_start_addr, size_t bytes)
{
#if DLMALLOC_DBG
	int i,j = 0;
#endif
	DevMalloc_MPool_Dlmalloc_Alloc_Free_Info_t dlmalloc_alloc_free_info;
	void *alloc_return_va = 0;
	MS_PHY pa_pool = MsOS_MPool_VA2PA((MS_VIRT)msp);
	MS_PHY ba_pool = HAL_MsOS_MPool_PA2BA(pa_pool);

	MS_PHY pa_old_alloc_start_addr = MsOS_MPool_VA2PA((MS_VIRT)old_alloc_start_addr);
	MS_PHY ba_old_alloc_start_addr = HAL_MsOS_MPool_PA2BA(pa_old_alloc_start_addr);

	dlmalloc_alloc_free_info.alloc_addr = ba_old_alloc_start_addr;
	dlmalloc_alloc_free_info.pool_bus_addr = ba_pool;
	dlmalloc_alloc_free_info.alloc_size = bytes;
	//printf("\033[35mFunction = %s, Line = %d, re-alloc from ba: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.alloc_addr);
	//printf("\033[35mFunction = %s, Line = %d, re-alloc from pool ba: 0x%llX, size: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.pool_bus_addr, dlmalloc_alloc_free_info.alloc_size);

	//MsOS_DelayTask(100);
	if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_DLMALLOC_REALLOC_POOL_MEMORY, &dlmalloc_alloc_free_info))
	{
		sleep(1);
        printf("alloc memory from mpool failed!\n");
        return 0;
	}
	//MsOS_DelayTask(100);

	// we can not use pa2va, because we donot have cache/non-cache infomation, so we use the offset to calculate the va
	if(dlmalloc_alloc_free_info.new_alloc_addr)
		alloc_return_va = (void *)((unsigned long)(dlmalloc_alloc_free_info.new_alloc_addr - ba_pool) + (unsigned long)msp);
	else
		alloc_return_va = 0;
	//printf("\033[35mFunction = %s, Line = %d, get memory @ ba: 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, dlmalloc_alloc_free_info.new_alloc_addr);
	//printf("\033[35mFunction = %s, Line = %d, re-alloc_return_va is 0x%lX, size: 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, alloc_return_va, bytes);

#if DLMALLOC_DBG
	if(alloc_return_va == 0)
		return alloc_return_va;

	for(i = 0; i < DLMALLOC_INFO_CNT; i++)
	{
		if(mstar_dlmalloc_info[i].msp == msp)
			break;
	}

	if(i == DLMALLOC_INFO_CNT)
	{
		printf("\033[35mFunction = %s, Line = %d, no mstar_dlmalloc_info match for msp: 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, msp);
		*(volatile int *)0x8 = 0x5566;
	}
	else
	{
		// reset info
		for(j = 0; j < DETAIL_DLMALLOC_INFO_CNT; j++)
		{
			if(mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va == old_alloc_start_addr)
			{
				mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_start_va = alloc_return_va;
				mstar_dlmalloc_info[i].detail_dlmalloc_info[j].allocated_size = bytes;
				break;
			}

		}

		if(j == DETAIL_DLMALLOC_INFO_CNT)
		{
			printf("\033[35mFunction = %s, Line = %d, no detail_dlmalloc_info can be delete for old_alloc_start_addr 0x%lX, die\033[m\n", __PRETTY_FUNCTION__, __LINE__, old_alloc_start_addr);
			*(volatile int *)0x9 = 0x5566;
		}
	}
#endif

	return alloc_return_va;
}
#endif

