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



#define UCOS_V1 1

#include "sysdefs.h"
#include "dlmalloc.h"

#if UCOS_V1

// Tsaihua has to move
//     OSCPUSaveSR --> MSCPU_OSCPUSaveSR
//     OSCPURestoreSR --> MSCPU_OSCPURestoreSR
//     OSCPUEnableSR --> MSCPU_OSCPUEnableSR
//      from utopia/os/ucos2/mips_ucos/common/src/os_cpu_a.S
extern unsigned int MSCPU_OSCPUSaveSR(void);
extern void MSCPU_OSCPURestoreSR(unsigned int cpu_sr);
extern void MSCPU_OSCPUEnableSR(void);

// Tsaihua : it is a global variable in uCos library, no matter version 1.x or 2.x
extern unsigned char OSIntNesting;             /* Interrupt nesting level                         */
#endif

#if defined (MSOS_TYPE_UCOS)
#include "ucos.h"

#undef OS_ENTER_CRITICAL
#undef OS_EXIT_CRITICAL


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "string.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"

#include "mem.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
// Switch tasks every 1 ms.
#define TASK_TIME_SLICE             (TICK_PER_ONE_MS)

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK

#if (defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_T9) || defined(CHIP_U4) || defined(CHIP_T13) || defined(CHIP_A1) || defined(CHIP_A6) || defined(CHIP_A7) || defined(CHIP_AMETHYST)) || defined(CHIP_E8)
#define MIU1_CACHEABLE_START        0xC0000000
#define MIU1_CACHEABLE_END          0xD0000000
#define MIU1_UNCACHEABLE_START      0xD0000000
#define MIU1_UNCACHEABLE_END        0xE0000000
#endif

#define UCOS_FOREVER                0

#define CPU_INTTERUPT_ENABLE()      OS_EXIT_CRITICAL()
#define CPU_INTTERUPT_DISABLE()     OS_ENTER_CRITICAL()

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL bInitTaskTable = FALSE;
MS_BOOL* pUserDefineTask = NULL;


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
#ifdef UCOS
//
// Variable-size Memory pool
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stMemoryPool;
    cyg_mempool_var                 stMemoryPoolInfo;
} MsOS_MemoryPool_Info;

static MsOS_MemoryPool_Info         _MsOS_MemoryPool_Info[MSOS_MEMPOOL_MAX];
static cyg_mutex_t                  _MsOS_MemoryPool_Mutex;

//
// Fixed-size Memory pool
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stMemoryPool;
    cyg_mempool_fix                 stMemoryPoolInfo;
} MsOS_FixSizeMemoryPool_Info;

static MsOS_FixSizeMemoryPool_Info  _MsOS_FixSizeMemoryPool_Info[MSOS_FIXSIZE_MEMPOOL_MAX];
static cyg_mutex_t                  _MsOS_FixSizeMemoryPool_Mutex;
#else
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stMemoryPool;
    cyg_mempool_var                 stMemoryPoolInfo;
    MS_BOOL                         bMPool;
} MsOS_MemoryPool_Info;

static MsOS_MemoryPool_Info         _MsOS_MemoryPool_Info[MSOS_MEMPOOL_MAX];
static OS_EVENT*                    _MsOS_MemoryPool_Mutex;

#endif //UCOS

//
// Task Management
//
typedef struct
{
    MS_BOOL                         bUsed;
    MS_U32                          u32TaskId;
} MsOS_Task_Info;

static MsOS_Task_Info               _MsOS_Task_Info[MSOS_TASK_MAX];
static OS_EVENT*                    _MsOS_Task_Mutex;

//
// Mutex
//
typedef struct
{
    MS_BOOL                         bUsed;
    OS_EVENT*                       stMutex;
} MsOS_Mutex_Info;

static MsOS_Mutex_Info              _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static OS_EVENT*                    _MsOS_Mutex_Mutex;

//
// Semaphore
//
typedef struct
{
    MS_BOOL                         bUsed;
    OS_EVENT*                       stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info          _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];
static OS_EVENT*                    _MsOS_Semaphore_Mutex;

#if UCOS_V1
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#endif

//
// Event Group
//
typedef struct
{
    MS_BOOL                         bUsed;
#if UCOS_V1
    MS_U32                          u32EventGroup;
    OS_EVENT*                       stMutexEvent;
    // OS_EVENT*                       stSemEvent;
#else
    OS_FLAG_GRP*                    stEventGroup;
#endif
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info         _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static OS_EVENT*                    _MsOS_EventGroup_Mutex;

//
// Queue
//
typedef struct
{
    MS_BOOL                         bUsed;
    OS_EVENT*                       stQueue;
    OS_EVENT*                       stMutex;
    void                            *pMsgTbl;
    MS_U8                           *pu8MsgQueue;
    MS_U32                          u32AlignedMsgSize;
    MS_U32                          u32QueueSize;
    MS_U32                          u32MsgWriteIdx;
    MessageType                     eMsgType;
} MsOS_Queue_Info;

static MsOS_Queue_Info              _MsOS_Queue_Info[MSOS_QUEUE_MAX];
static OS_EVENT*                    _MsOS_Queue_Mutex;

//
// Timer
//
#if UCOS_V1
typedef struct
{
    MS_BOOL                         bUsed;
    MS_BOOL                         bEnable;
    TimerCb                         pTimerCb;
    MS_U32                          u32FirstTimeMs;
    MS_U32                          u32PeriodTimeMs;
    MS_U32                          u32TimeTrig;
} MsOS_Timer_Info;
static MsOS_Timer_Info              _MsOS_Timer_Info[MSOS_TIMER_MAX];
static OS_EVENT*                    _MsOS_Timer_Mutex = NULL;
static MS_BOOL                      bTimerTask = FALSE;

#else
typedef struct
{
    MS_BOOL                         bUsed;
    OS_TMR*                         stTimer;
} MsOS_Timer_Info;
static MsOS_Timer_Info              _MsOS_Timer_Info[MSOS_TIMER_MAX];
static OS_EVENT*                    _MsOS_Timer_Mutex;
#endif

//
// Interrupt
//
static OS_EVENT*                    _MsOS_HISR_Mutex = NULL;

#if 1 //move to mscommon.h
//cached/unchched segment
#define KSEG0_BASE              ((void *)0x80000000)
#define KSEG1_BASE              ((void *)0xa0000000)
#define KSEG0_SIZE              0x20000000
#define KSEG1_SIZE              0x20000000

//cached addr <-> unchched addr
#define KSEG02KSEG1(addr)       ((void *)((MS_U32)(addr)|0x20000000))  //cached -> unchched
#define KSEG12KSEG0(addr)       ((void *)((MS_U32)(addr)&~0x20000000)) //unchched -> cached

//virtual addr <-> physical addr
#define VA2PA(addr)             ((void *)(((MS_U32)addr) & 0x1fffffff)) //virtual -> physical
#define PA2KSEG0(addr)          ((void *)(((MS_U32)addr) | 0x80000000)) //physical -> cached
#define PA2KSEG1(addr)          ((void *)(((MS_U32)addr) | 0xa0000000)) //physical -> unchched
#endif
//MS_U32<->MS_U16
#define LOU16(u32Val)           ( (MS_U16)(u32Val) )
#define HIU16(u32Val)           ( (MS_U16)((u32Val) >> 16) )
#define CATU32(u16HiVal, u16LoVal)  ( (MS_U32)(u16HiVal)<<16 | (MS_U32)(u16LoVal) )

//MS_U16<->MS_U8
#define LOU8(u16Val)            ( (MS_U8)(u16Val) )
#define HIU8(u16Val)            ( (MS_U8)((u16Val) >> 8) )


//FIXME: It's better to be internal or have internal system pool for usage.
extern MS_S32 gs32SystemPoolID;
//extern MS_S32 gs32CachedPoolID;
//extern MS_S32 gs32NonCachedPoolID;
//extern MS_S32 gs32DisplayPoolID;
//extern MS_S32 gs32UserPoolID;                                              //[OPTIONAL]
//extern MS_S32 gs32RAMFSPoolID;                                             //[OPTIONAL]

char *UTL_strcpy(char *strDst, const char *strSrc)
{
    int i;

    for(i=0;strSrc[i];i++)
    {
        strDst[i] = strSrc[i];
    }
    strDst[i] = '\0';
    return strDst;
}

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

extern void add_wired_entry(unsigned long entrylo0, unsigned long entrylo1, unsigned long entryhi, unsigned long pagemask);

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

    // Empty all the MsOS structures
#ifdef UCOS
    for( u32I=0; u32I<MSOS_MEMPOOL_MAX; u32I++)
    {
        _MsOS_MemoryPool_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_FIXSIZE_MEMPOOL_MAX; u32I++)
    {
        _MsOS_FixSizeMemoryPool_Info[u32I].bUsed = FALSE;
    }
#endif //UCOS
    if ((bInitTaskTable == FALSE) || (pUserDefineTask == NULL))
    {
        for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
        {
            _MsOS_Task_Info[u32I].bUsed = FALSE;
        }
    }
    else
    {
        for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
        {
            _MsOS_Task_Info[u32I].bUsed = pUserDefineTask[u32I];

        }
    }
    for( u32I=0; u32I<MSOS_MUTEX_MAX; u32I++)
    {
        _MsOS_Mutex_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_SEMAPHORE_MAX; u32I++)
    {
        _MsOS_Semaphore_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_EVENTGROUP_MAX; u32I++)
    {
        _MsOS_EventGroup_Info[u32I].bUsed = FALSE;
    }
#if UCOS_V1
    for( u32I=0; u32I<MSOS_TIMER_MAX; u32I++)
    {
        _MsOS_Timer_Info[u32I].bUsed = FALSE;
        _MsOS_Timer_Info[u32I].bEnable = FALSE;
        _MsOS_Timer_Info[u32I].pTimerCb = NULL;
        _MsOS_Timer_Info[u32I].u32FirstTimeMs = 0xFFFFFFFF;
        _MsOS_Timer_Info[u32I].u32PeriodTimeMs = 0xFFFFFFFF;
        _MsOS_Timer_Info[u32I].u32TimeTrig =  0xFFFFFFFF;
    }
#else
#endif
    for( u32I=0; u32I<MSOS_QUEUE_MAX; u32I++)
    {
        _MsOS_Queue_Info[u32I].bUsed = FALSE;
    }

    // Please update the HISR priority here if it is necessary.


    // Create Semaphore for all resources.
#ifdef UCOS
    _MsOS_MemoryPool_Mutex = OSSemCreate(1);
    _MsOS_FixSizeMemoryPool_Mutex = OSSemCreate(1);
#endif //UCOS
    _MsOS_Task_Mutex = OSSemCreate(1);
    _MsOS_Mutex_Mutex = OSSemCreate(1);
    _MsOS_Semaphore_Mutex = OSSemCreate(1);
    _MsOS_EventGroup_Mutex = OSSemCreate(1);
    _MsOS_Timer_Mutex = OSSemCreate(1);
    _MsOS_Queue_Mutex = OSSemCreate(1);
    _MsOS_HISR_Mutex = OSSemCreate(1);

#if(defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K))
    #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2))
        // MIU0
        unsigned long entrylo0 = ((0x10000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1 = ((0x10000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        // MIU1
        unsigned long entrylo0_miu1_lo256mb = ((0x20000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1_miu1_lo256mb = ((0x20000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        unsigned long entrylo0_miu1_hi256mb = ((0x30000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1_miu1_hi256mb = ((0x30000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        // MIU0
        add_wired_entry(entrylo0, entrylo1, 0xC0000000 , 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)
        // MIU1
        add_wired_entry(entrylo0_miu1_lo256mb, entrylo1_miu1_lo256mb, 0x00000000 , 0x1fffe000);
        add_wired_entry(entrylo0_miu1_hi256mb, entrylo1_miu1_hi256mb, 0x20000000 , 0x1fffe000);
    #elif (defined(CHIP_U4))
        unsigned long entrylo0 = ((0x40000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1 = ((0x40000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        add_wired_entry(entrylo0, entrylo1, MIU1_CACHEABLE_START , 0x1fffe000);
    #endif
#endif

    CHIP_InitISR();

    //can't enable INT till LISR/HISR has been registered/attached
    MsOS_DisableInterrupt(E_INT_IRQ_FIQ_ALL);

    return TRUE;
}


#ifdef UCOS
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
MS_S32 MsOS_CreateMemoryPool (MS_U32        u32PoolSize,
                              MS_U32        u32MinAllocation,
                              void          *pPoolAddr,
                              MsOSAttribute eAttribute,
                              char          *pPoolName)
{
    MS_S32 s32Id;

    cyg_mutex_lock(&_MsOS_MemoryPool_Mutex);

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
    cyg_mutex_unlock(&_MsOS_MemoryPool_Mutex);

    if(s32Id >= MSOS_MEMPOOL_MAX)
    {
        return -1;
    }

    //no Priority order for eCos
    //pPoolName is not used for eCos

    // Call OS kernel to create a memory pool.
    cyg_mempool_var_create( pPoolAddr,
                            u32PoolSize,
                            &_MsOS_MemoryPool_Info[s32Id].stMemoryPool,
                            &_MsOS_MemoryPool_Info[s32Id].stMemoryPoolInfo );

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

    cyg_mempool_var_delete(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool);
    cyg_mutex_lock(&_MsOS_MemoryPool_Mutex);
    _MsOS_MemoryPool_Info[s32PoolId].bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_MemoryPool_Mutex);
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
MS_BOOL MsOS_InfoMemoryPool (MS_S32    s32PoolId,
                          void      **pPoolAddr,
                          MS_U32    *pu32PoolSize,
                          MS_U32    *pu32FreeSize,
                          MS_U32    *pu32LargestFreeBlockSize)
{
    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    cyg_mempool_info mempool_info;
    cyg_mempool_var_get_info(_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, &mempool_info);

    *pPoolAddr = mempool_info.base;
    *pu32PoolSize = mempool_info.size;
    *pu32FreeSize = mempool_info.freemem;
    *pu32LargestFreeBlockSize = mempool_info.maxfree;

    return TRUE;
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

    pAddr = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, u32Size);

    //Current eCosPro kernel always allocates 16-B aligned block
    if ( (MS_U32)pAddr % 16 )
    {
        return NULL;
    }

    return ( pAddr );
}

/*
//-------------------------------------------------------------------------------------------------
// Allocate a block of memory with aligned starting address from the variable-size memory pool
// @param  u32Size         \b IN: requested size
// @param  s32PoolId       \b IN: memory pool ID
// @param  u32AlignedByte  \b IN: #-byte aligned start address (must be a multiple of 4)
// @return NULL : fail
// @return Otherwise : pointer to the allocated memory block
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateAlignedMemory (MS_U32 u32Size, MS_U32 u32AlignedByte, MS_S32 s32PoolId)
{
#if 0
    void *pAddr;

    if ( (u32AlignedByte % 4) != 0)
    {
        return NULL;
    }
    if( NU_Allocate_Aligned_Memory( &_MsOS_MemoryPool_Info[s32PoolId].stMemoryPool,
                                    &pAddr,
                                    u32Size,
                                    u32AlignedByte,
                                    0) != NU_SUCCESS)
    {
        return NULL;
    }
    else
    {
        return pAddr;
    }
#endif

    //eCos does not support aligned allocate
    return NULL;
}
*/

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

    cyg_mempool_var_free( _MsOS_MemoryPool_Info[s32PoolId].stMemoryPool, pAddress);

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
    MS_U32 u32OrgSize;

    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
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
        cyg_int32 size;
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
    MS_S32 s32Id;

    cyg_mutex_lock(&_MsOS_FixSizeMemoryPool_Mutex);

    for( s32Id=0; s32Id<MSOS_FIXSIZE_MEMPOOL_MAX; s32Id++)
    {
        if(_MsOS_FixSizeMemoryPool_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_FIXSIZE_MEMPOOL_MAX)
    {
        _MsOS_FixSizeMemoryPool_Info[s32Id].bUsed = TRUE;
    }
    cyg_mutex_unlock(&_MsOS_FixSizeMemoryPool_Mutex);

    if(s32Id >= MSOS_FIXSIZE_MEMPOOL_MAX)
    {
        return -1;
    }

    //no Priority order for eCos
    //pPoolName is not used for eCos

    // Call OS kernel to create a memory pool.
    cyg_mempool_fix_create( pPoolAddr,
                            u32PoolSize,
                            u32BlockSize,
                            &_MsOS_FixSizeMemoryPool_Info[s32Id].stMemoryPool,
                            &_MsOS_FixSizeMemoryPool_Info[s32Id].stMemoryPoolInfo );

    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete a fixed-size memory pool
/// @param  s32PoolId   \b IN: pool ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteFixSizeMemoryPool (MS_S32 s32PoolId)
{
    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    cyg_mempool_fix_delete(_MsOS_FixSizeMemoryPool_Info[s32PoolId].stMemoryPool);
    cyg_mutex_lock(&_MsOS_FixSizeMemoryPool_Mutex);
    _MsOS_FixSizeMemoryPool_Info[s32PoolId].bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_FixSizeMemoryPool_Mutex);

    return TRUE;
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
    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    cyg_mempool_info mempool_info;
    cyg_mempool_fix_get_info(_MsOS_FixSizeMemoryPool_Info[s32PoolId].stMemoryPool, &mempool_info);

    *pPoolAddr = mempool_info.base;
    *pu32PoolSize = mempool_info.size;
    *pu32FreeSize = mempool_info.freemem;
    *pu32LargestFreeBlockSize = mempool_info.maxfree;

    return TRUE;
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
    void  *pAddr;

    if ( (s32PoolId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return NULL;
    }
    else
    {
        s32PoolId &= MSOS_ID_MASK;
    }

    pAddr = cyg_mempool_fix_try_alloc( _MsOS_FixSizeMemoryPool_Info[s32PoolId].stMemoryPool);

    return ( pAddr );
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

    cyg_mempool_fix_free( _MsOS_FixSizeMemoryPool_Info[s32PoolId].stMemoryPool, pAddress);

    return TRUE;
}
#endif //UCOS


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
MS_S32 MsOS_CreateTask (TaskEntry  pTaskEntry,
                     MS_VIRT     u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL    bAutoStart,
                     void       *pStackBase,
                     MS_U32     u32StackSize,
                     char       *pTaskName)
{
    MS_S32 s32Id = 0;
#if UCOS_V1
    void *pTop, *pBottom;
#else
    OS_TCB stTaskData;
    OS_STK *pTop, *pBottom;
    MS_U32 u32OSStkSize;
#endif
    MS_U8 u8Err;

    // Check parameters
    if ((MS_U32)pStackBase % 4)
    {
        return -1;
    }

    OSSemPend(_MsOS_Task_Mutex, 0, &u8Err);
    for (s32Id = eTaskPriority; s32Id < MSOS_TASK_MAX; s32Id++)
    {
        if (_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
#if (!UCOS_V1)
            if (OSTaskQuery(s32Id, &stTaskData) != OS_TASK_NOT_EXIST)
            {
                MS_ASSERT(0);
            }
#endif
            break;
        }
    }
    if (s32Id < MSOS_TASK_MAX)
    {
        _MsOS_Task_Info[s32Id].bUsed = TRUE;
    }
    OSSemPost(_MsOS_Task_Mutex);

    if (s32Id >= MSOS_TASK_MAX)
    {
        return -1;
    }

    if (pStackBase == NULL)
    {
        MS_ASSERT(0);
    }
    else
    {

#if UCOS_V1
        pBottom = (void*)pStackBase;
        pTop = (void*)(((MS_U32)pStackBase) + u32StackSize - 1);

        u8Err = OSTaskCreate(
                    // pTaskEntry,
                    (void(*)(void*))pTaskEntry,
                    (void *)u32TaskEntryData,
                    pTop,
                    /// UCOS_V1 --> correct the priority later
                    (UBYTE)s32Id);
#else
        u32OSStkSize = u32StackSize / sizeof(OS_STK);
        pTop = ((OS_STK *)pStackBase) + u32OSStkSize - 1;
        pBottom = (OS_STK *)pStackBase;
        u8Err = OSTaskCreateExt((OSTaskEntry)pTaskEntry, //TaskEntry has two arguments
                                (void *)u32TaskEntryData,
                                pTop,
                                s32Id,
                                s32Id,
                                pBottom,
                                u32OSStkSize,
                                NULL,
                                0);
#endif
        if (OS_NO_ERR != u8Err)
        {
            OSSemPend(_MsOS_Task_Mutex, 0, &u8Err);
            _MsOS_Task_Info[s32Id].bUsed = FALSE;
            OSSemPost(_MsOS_Task_Mutex);
            return -1;
        }
#if UCOS_V1
#else
        OSTaskNameSet((MS_U8)s32Id, (MS_U8 *)pTaskName, &u8Err);
#endif
    }

    if (bAutoStart == FALSE)
    {
#if UCOS_V1
#else
        OSTaskSuspend(s32Id);
#endif
    }

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
    MS_U8 u8Err;

    if ((s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    if (OSTaskDel(s32TaskId) == OS_NO_ERR)
    {
        OSSemPend(_MsOS_Task_Mutex, 0, &u8Err);
        _MsOS_Task_Info[s32TaskId].bUsed = FALSE;
        OSSemPost(_MsOS_Task_Mutex);
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
void MsOS_YieldTask(void)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    //MS_ASSERT(0);
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
	MS_U32 u32Ticks = 0;

    //u32Ms=0 => The task will enter sleep state and wake up immediately with relative trigger time = 0
    // => Other ready tasks with the same or lower priorities will have no chance to run
    MS_ASSERT( u32Ms != 0);
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    //OSTimeDly(u32Ms * TICK_PER_ONE_MS);
    u32Ticks = u32Ms / 10;
    if (u32Ticks < 1)
    	OSTimeDly(1);
    else
        OSTimeDly(u32Ticks);

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
    MAsm_CPU_DelayUs(u32Us);
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
#if UCOS_V1
    return FALSE;
#else
    if ((s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    if (OSTaskResume(s32TaskId) == OS_NO_ERR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
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
#if UCOS_V1
    return FALSE;
#else
    if ((s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    if (OSTaskSuspend(s32TaskId) == OS_NO_ERR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
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
#if UCOS_V1
    return TRUE;
#else
    OS_TCB stTaskData;

    if ((s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    if (FALSE == _MsOS_Task_Info[s32TaskId].bUsed)
    {
        return FALSE;
    }

    if (OSTaskQuery(s32TaskId, &stTaskData) != OS_NO_ERR)
    {
        return FALSE;
    }

    *peTaskPriority = s32TaskId;
    *((MS_U32*)pStackBase) = (MS_U32)stTaskData.OSTCBStkBottom;
    *((MS_U32*)pu32StackSize) = stTaskData.OSTCBStkSize * sizeof(OS_STK);
    *((MS_U32*)pu32StackUsed) = stTaskData.OSTCBStkUsed;
    strcpy(pTaskName, (void *)stTaskData.OSTCBTaskName);
    return TRUE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get all task ID
/// @param  ps32TaskIdList   \b IN/OUT: the memory for the all task ID
/// @return number of task created
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_InfoTaskList (MS_S32* ps32TaskIdList)
{
    MS_S32 s32Id;
    MS_U32 u32TaskNum = 0;

    for (s32Id = 0; s32Id < MSOS_TASK_MAX; s32Id++)
    {
        if (_MsOS_Task_Info[s32Id].bUsed == TRUE)
        {
            ps32TaskIdList[u32TaskNum] = s32Id;
            u32TaskNum++;
        }
    }
    return u32TaskNum;
}

//-------------------------------------------------------------------------------------------------
/// Get thread ID of current thread/process in Linux(Kernel)
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    //return MsOS_InfoTaskID();
    return OSGetTaskID();
}

//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
#if UCOS_V1
#else
MS_S32 MsOS_InfoTaskID (void)
{
    return OSPrioCur;
}
#endif


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
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag)
{
    MS_S32 s32Id;
    MS_U8 u8Err;
    //OPTION suspend_type;

    OSSemPend(_MsOS_Mutex_Mutex, 0, &u8Err);
    for (s32Id = 0; s32Id < MSOS_MUTEX_MAX; s32Id++)
    {
        if(_MsOS_Mutex_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_MUTEX_MAX)
    {
        _MsOS_Mutex_Info[s32Id].bUsed = TRUE;
    }
    OSSemPost(_MsOS_Mutex_Mutex);

    if(s32Id >= MSOS_MUTEX_MAX)
    {
        return -1L;
    }

    // no fifo order for ucos
    // pMutexName is not used for ucos

    // Call OS kernel to create a semaphore.
    _MsOS_Mutex_Info[s32Id].stMutex = OSSemCreate(1);
    if (!_MsOS_Mutex_Info[s32Id].stMutex)
    {
        OSSemPend(_MsOS_Mutex_Mutex, 0, &u8Err);
        _MsOS_Mutex_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_Mutex_Mutex);
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
#if UCOS_V1
    return FALSE;
#else
    MS_U8 u8Err;

    if ((s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    OSSemDel(_MsOS_Mutex_Info[s32MutexId].stMutex, OS_DEL_ALWAYS, &u8Err);
    if (OS_NO_ERR == u8Err)
    {
        OSSemPend(_MsOS_Mutex_Mutex, 0, &u8Err);
        _MsOS_Mutex_Info[s32MutexId].bUsed = FALSE;
        OSSemPost(_MsOS_Mutex_Mutex);
        return TRUE;
    }

    return FALSE;
#endif
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
    MS_U8 u8Err;

    if ((s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs == MSOS_WAIT_FOREVER) //blocking wait
    {
        OSSemPend(_MsOS_Mutex_Info[s32MutexId].stMutex, 0, &u8Err);
        if (u8Err == OS_NO_ERR)
        {
            bRet = TRUE;
        }
    }
    else if (u32WaitMs == 0) //non-blocking
    {
#if UCOS_V1
        printf("[%s][%d] obtain mutex for time out 0 is not accepted\n", __FUNCTION__, __LINE__);
        MAsm_CPU_SwDbgBp();
#else
        if (OSSemAccept(_MsOS_Mutex_Info[s32MutexId].stMutex))
        {
            bRet = TRUE;
        }
#endif
    }
    else //blocking wait with timeout
    {
        OSSemPend(_MsOS_Mutex_Info[s32MutexId].stMutex, u32WaitMs, &u8Err);
        if (u8Err == OS_NO_ERR)
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
    if ((s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if (OS_NO_ERR != OSSemPost(_MsOS_Mutex_Info[s32MutexId].stMutex))
    {
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
    if ((s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    if (_MsOS_Mutex_Info[s32MutexId].bUsed == TRUE)
    {
        //ToDo: extend _MsOS_Mutex_Info structure ?
        *peAttribute = E_MSOS_PRIORITY; //only FIFO for ucos
        UTL_strcpy(pMutexName, "ABC");

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
    MS_U8 u8Err;

    OSSemPend(_MsOS_Semaphore_Mutex, 0, &u8Err);
    for (s32Id = 0; s32Id < MSOS_SEMAPHORE_MAX; s32Id++)
    {
        if(_MsOS_Semaphore_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if (s32Id < MSOS_SEMAPHORE_MAX)
    {
        _MsOS_Semaphore_Info[s32Id].bUsed = TRUE;
    }
    OSSemPost(_MsOS_Semaphore_Mutex);

    if (s32Id >= MSOS_SEMAPHORE_MAX)
    {
        return -1;
    }

    // no fifo order for ucos
    // pSemaphoreName is not used for ucos

    // Call OS kernel to create a semaphore.
    _MsOS_Semaphore_Info[s32Id].stSemaphore = OSSemCreate(u32InitCnt);
    if (!_MsOS_Semaphore_Info[s32Id].stSemaphore)
    {
        OSSemPend(_MsOS_Semaphore_Mutex, 0, &u8Err);
        _MsOS_Semaphore_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_Semaphore_Mutex);
        return -1;
    }

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
#if UCOS_V1
    printf("[%s][%d] This function is un-supported since OSSemDel is not unavailable from NDS uCos\n", __FUNCTION__, __LINE__);
    return FALSE;
#else
    MS_U8 u8Err;

    if ((s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    OSSemDel(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, OS_DEL_ALWAYS, &u8Err);
    if (OS_NO_ERR == u8Err)
    {
        OSSemPend(_MsOS_Semaphore_Mutex, 0, &u8Err);
        _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
        OSSemPost(_MsOS_Semaphore_Mutex);
        return TRUE;
    }

    return FALSE;
#endif
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
    MS_U8 u8Err;

    if ((s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs == MSOS_WAIT_FOREVER) //blocking wait
    {
        OSSemPend(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, 0, &u8Err);
        if (u8Err == OS_NO_ERR)
        {
            bRet = TRUE;
        }
    }
    else if (u32WaitMs == 0) //non-blocking
    {
#if UCOS_V1
        printf("[%s][%d] (u32WaitMs == 0) is un-supported since OSSemDel is not unavailable from NDS uCos\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
#else
        if (OSSemAccept(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore))
        {
            bRet = TRUE;
        }
#endif
    }
    else //blocking wait with timeout
    {
        OSSemPend(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, u32WaitMs, &u8Err);
        if (u8Err == OS_NO_ERR)
        {
            bRet = TRUE;
        }
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
    if ((s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    if (OS_NO_ERR != OSSemPost(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore))
    {
        return FALSE;
    }

    return TRUE;
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
#if UCOS_V1
    printf("[%s][%d] This function is un-supported since OSSemQuery is not unavailable from NDS uCos\n", __FUNCTION__, __LINE__);
    return FALSE;
#else
    OS_SEM_DATA sem_data;

    if ((s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    if(_MsOS_Semaphore_Info[s32SemaphoreId].bUsed == TRUE)
    {
        //ToDo: extend _MsOS_Semaphore_Info structure ?
        OSSemQuery(_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore, &sem_data);
        *pu32InitCnt = sem_data.OSCnt;
        *peAttribute = E_MSOS_PRIORITY;
        UTL_strcpy(pSemaphoreName, "XXX");

        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}


//
// Event management
//
#define EVENT_MUTEX_LOCK()                                              \
{                                                                       \
    MS_U8 u8Err;                                                        \
    OSSemPend(_MsOS_EventGroup_Mutex, 0, &u8Err);                       \
}

#define EVENT_MUTEX_UNLOCK()            OSSemPost(_MsOS_EventGroup_Mutex)

//-------------------------------------------------------------------------------------------------
/// Create an event group
/// @param  pEventName  \b IN: event group name
/// @return >=0 : assigned Event Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateEventGroup (char *pEventName)
{
#if UCOS_V1
    MS_S32 s32Id;

    EVENT_MUTEX_LOCK();
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
    EVENT_MUTEX_UNLOCK();

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }

    // _MsOS_EventGroup_Info[s32Id].stSemEvent = OSSemCreate(0); //// need semaphore????
    _MsOS_EventGroup_Info[s32Id].stMutexEvent = OSSemCreate(1);
    _MsOS_EventGroup_Info[s32Id].u32EventGroup = 0;
    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
#else
    MS_S32 s32Id;
    MS_U8 u8Err;

    OSSemPend(_MsOS_EventGroup_Mutex, 0, &u8Err);
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
    }
    OSSemPost(_MsOS_EventGroup_Mutex);

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }

    //pEventName is not used for ucos

    _MsOS_EventGroup_Info[s32Id].stEventGroup = OSFlagCreate(0, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        OSSemPend(_MsOS_EventGroup_Mutex, 0, &u8Err);
        _MsOS_EventGroup_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_EventGroup_Mutex);
        return -1;
    }

    s32Id |= MSOS_ID_PREFIX;
    return s32Id;
#endif
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
#if UCOS_V1

/*
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
    EVENT_MUTEX_LOCK();
    _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup= 0;
    _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
    EVENT_MUTEX_UNLOCK();
*/
    printf("[%s][%d] This function is un-supported at this stage. Might be supported later\n", __FUNCTION__, __LINE__);
    return FALSE;
#else
    MS_U8 u8Err;

    if ((s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    OSFlagDel(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, OS_DEL_NO_PEND, &u8Err);
    if (u8Err == OS_NO_ERR)
    {
        OSSemPend(_MsOS_EventGroup_Mutex, 0, &u8Err);
        _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
        OSSemPost(_MsOS_EventGroup_Mutex);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

#define EVENT_EACH_MUTEX_LOCK(mutex, err)    OSSemPend((mutex), 0, &(err))
#define EVENT_EACH_MUTEX_UNLOCK(mutex)       OSSemPost((mutex))

//-------------------------------------------------------------------------------------------------
/// Set the event flag (bitwise OR w/ current value) in the specified event group
/// @param  s32EventGroupId \b IN: event group ID
/// @param  u32EventFlag    \b IN: event flag value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SetEvent (MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
#if UCOS_V1
    MS_U8 u8Err;

    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    EVENT_EACH_MUTEX_LOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, u8Err);
    SET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    EVENT_EACH_MUTEX_UNLOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    // OSSemPost(_MsOS_EventGroup_Info[s32EventGroupId].stSemEvent);
    return TRUE;
#else
    MS_U8 u8Err;

    if ((s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    OSFlagPost(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, u32EventFlag, OS_FLAG_SET, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }
    return TRUE;
#endif
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
#if UCOS_V1
    MS_U8 u8Err;

    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    EVENT_EACH_MUTEX_LOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, u8Err);
    RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, u32EventFlag);
    EVENT_EACH_MUTEX_UNLOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    return TRUE;
#else
    MS_U8 u8Err;

    if ((s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    OSFlagPost(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, u32EventFlag, OS_FLAG_CLR, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }
    return TRUE;
#endif
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
#if UCOS_V1
    MS_BOOL bRet= FALSE;
    MS_BOOL bAnd;
    MS_BOOL bClear;
    MS_U8 u8Err;
    MS_U32 u32Timeout;
    MS_U32 u32Delay;

    *pu32RetrievedEventFlag = 0;
    if (!u32WaitEventFlag)
    {
        return FALSE;
    }

    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    bClear= ((E_AND_CLEAR== eWaitMode) || (E_OR_CLEAR== eWaitMode))? TRUE: FALSE;
    bAnd= ((E_AND== eWaitMode)|| (E_AND_CLEAR== eWaitMode))? TRUE: FALSE;

    u32Timeout = (u32WaitMs== MSOS_WAIT_FOREVER)? 0xFFFFFFFF : u32WaitMs;
    do
    {
        if (bAnd)
        {
            if ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) == u32WaitEventFlag)
            {
                *pu32RetrievedEventFlag = _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup;
                break;
            }
        }
        else
        {
            if ((_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup & u32WaitEventFlag) != 0)
            {
                *pu32RetrievedEventFlag = _MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup;
                break;
            }
        }
        u32Delay = (u32Timeout >= 10)? 10 : u32Timeout;
        if (u32WaitMs != MSOS_WAIT_FOREVER)
        {
            u32Timeout -= u32Delay;
        }
        if (u32Delay)
        {
            OSTimeDly(u32Delay * TICK_PER_ONE_MS); // delay 10ms
        }
    } while (u32Timeout> 0);

    EVENT_EACH_MUTEX_LOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent, u8Err);
    // spin_lock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    bRet= (bAnd)? (*pu32RetrievedEventFlag== u32WaitEventFlag): (0!= *pu32RetrievedEventFlag);
    if (bRet && bClear)
    {
        RESET_FLAG(_MsOS_EventGroup_Info[s32EventGroupId].u32EventGroup, *pu32RetrievedEventFlag);
    }
    EVENT_EACH_MUTEX_UNLOCK(_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    // spin_unlock(&_MsOS_EventGroup_Info[s32EventGroupId].stMutexEvent);
    return bRet;
#else
    MS_U8 u8WaitType;
    MS_BOOL bClear;
    MS_U8 u8Err;

    if ((s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32EventGroupId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    bClear = FALSE;
    switch(eWaitMode)
    {
    case E_AND:
        u8WaitType = OS_FLAG_WAIT_SET_ALL;
        break;
    case E_OR:
        u8WaitType = OS_FLAG_WAIT_SET_ANY;
        break;
    case E_AND_CLEAR:
        u8WaitType = OS_FLAG_WAIT_SET_ALL;
        bClear = TRUE;
        break;
    case E_OR_CLEAR:
        u8WaitType = OS_FLAG_WAIT_SET_ANY;
        bClear = TRUE;
        break;
    default:
        return FALSE;
    }


    if (u32WaitMs == MSOS_WAIT_FOREVER) //blocking wait
    {
        *pu32RetrievedEventFlag = OSFlagPend(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                u32WaitEventFlag, u8WaitType, 0, &u8Err);
    }
    else if (u32WaitMs == 0) //non-blocking
    {
        *pu32RetrievedEventFlag = OSFlagAccept(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                u32WaitEventFlag, u8WaitType, &u8Err);
    }
    else //blocking wait with timeout
    {
        *pu32RetrievedEventFlag = OSFlagPend(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                u32WaitEventFlag, u8WaitType, u32WaitMs, &u8Err);
    }

    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }

    if (bClear && *pu32RetrievedEventFlag)
    {
        OSFlagPost(_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, u32WaitEventFlag, OS_FLAG_CLR, &u8Err);
    }

    if (*pu32RetrievedEventFlag)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}


#if UCOS_V1
//
// Timer management
//
static MS_U8 u8TimeTaskStack[4096];

static MS_BOOL _IsTimerTrigger(MS_U32 u32TimeNow, MS_U32 u32TimeTrig)
{
    if (u32TimeNow >= u32TimeTrig)//Timer expired.
    {
        return TRUE;
    }

    if ((u32TimeTrig - u32TimeNow) < 3)//Timer will be expired soon.
    {
        return TRUE;
    }

    return FALSE;
}

void TimerTask(void* pArg)
{
    int i;
    void* pTempArg = pArg;
    MS_U8 u8Err;

    pArg = pTempArg;
    while (1)
    {
        OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
        for (i = 0; i < MSOS_TIMER_MAX; i++)
        {
            if (FALSE == _MsOS_Timer_Info[i].bUsed)
            {
                continue;
            }
            if (FALSE == _MsOS_Timer_Info[i].bEnable)
            {
                continue;
            }
            if (NULL == _MsOS_Timer_Info[i].pTimerCb)
            {
                continue;
            }
            if (0xFFFFFFFF == _MsOS_Timer_Info[i].u32TimeTrig)
            {
                continue;
            }
            if (TRUE == _IsTimerTrigger(MsOS_GetSystemTime(), _MsOS_Timer_Info[i].u32TimeTrig))
            {
                if (0 != _MsOS_Timer_Info[i].u32PeriodTimeMs) //Periodic Timer
                {
                    _MsOS_Timer_Info[i].bEnable = TRUE;
                    _MsOS_Timer_Info[i].u32TimeTrig += _MsOS_Timer_Info[i].u32PeriodTimeMs;
                }
                else
                {
                    _MsOS_Timer_Info[i].bEnable = FALSE;
                }

                _MsOS_Timer_Info[i].pTimerCb(0, (i | MSOS_ID_PREFIX));
            }
        }
        OSSemPost(_MsOS_Timer_Mutex);
        MsOS_DelayTask(10);
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
MS_S32 MsOS_CreateTimer (TimerCb   pTimerCb,
                         MS_U32    u32FirstTimeMs,
                         MS_U32    u32PeriodTimeMs,
                         MS_BOOL   bStartTimer,
                         char      *pTimerName)
{
    MS_S32 s32Id = -1;
    MS_U8 u8Err;

    printf("[%s][%d]\n", __FUNCTION__, __LINE__);

    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);

    for (s32Id = 0; s32Id < MSOS_TIMER_MAX; s32Id++)
    {
        if (_MsOS_Timer_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if (s32Id < MSOS_TIMER_MAX)
    {
        _MsOS_Timer_Info[s32Id].bUsed = TRUE;
        _MsOS_Timer_Info[s32Id].bEnable = FALSE;
        _MsOS_Timer_Info[s32Id].pTimerCb = pTimerCb;
        _MsOS_Timer_Info[s32Id].u32FirstTimeMs = u32FirstTimeMs;
        _MsOS_Timer_Info[s32Id].u32PeriodTimeMs = u32PeriodTimeMs;
    }
    else
    {
        goto TimerCreate_fail;
    }
    if (FALSE == bTimerTask)
    {
        MS_U8* pu8TimeTaskStack = u8TimeTaskStack + sizeof(u8TimeTaskStack) - 1;
        MS_S32 i;

        for (i= 63; i>= 0; i--)
        {
            u8Err = OSTaskCreate(
                        (void(*)(void*))TimerTask,
                        (void *)NULL,
                        pu8TimeTaskStack,
                        (UBYTE)i);
/*
            if (OS_NO_ERR != u8Err)
            {
                printf("[%s][%d] create timer task fail\n", __FUNCTION__, __LINE__);
                goto TimerCreate_fail;
            }
*/
            if (OS_NO_ERR == u8Err)
            {
            	break;
            }
        }
        if (OS_NO_ERR != u8Err)
        {
            printf("[%s][%d] create timer task fail\n", __FUNCTION__, __LINE__);
            goto TimerCreate_fail;
        }
        bTimerTask = TRUE;
    }
    if (bStartTimer)
    {
        _MsOS_Timer_Info[s32Id].u32TimeTrig = MsOS_GetSystemTime() + _MsOS_Timer_Info[s32Id].u32FirstTimeMs;
        _MsOS_Timer_Info[s32Id].bEnable = TRUE;
    }
    else
    {
        _MsOS_Timer_Info[s32Id].u32TimeTrig =  0xFFFFFFFF;
        _MsOS_Timer_Info[s32Id].bEnable = FALSE;
    }
    s32Id |= MSOS_ID_PREFIX;
TimerCreate_fail:
    OSSemPost(_MsOS_Timer_Mutex);
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }
    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
    _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
    _MsOS_Timer_Info[s32TimerId].bEnable = FALSE;
    _MsOS_Timer_Info[s32TimerId].pTimerCb = NULL;
    _MsOS_Timer_Info[s32TimerId].u32FirstTimeMs = 0xFFFFFFFF;
    _MsOS_Timer_Info[s32TimerId].u32PeriodTimeMs = 0xFFFFFFFF;
    _MsOS_Timer_Info[s32TimerId].u32TimeTrig =  0xFFFFFFFF;
    OSSemPost(_MsOS_Timer_Mutex);
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (TRUE == _MsOS_Timer_Info[s32TimerId].bEnable)
    {
        return TRUE;
    }

    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
    _MsOS_Timer_Info[s32TimerId].u32TimeTrig =  _MsOS_Timer_Info[s32TimerId].u32FirstTimeMs + MsOS_GetSystemTime();
    _MsOS_Timer_Info[s32TimerId].bEnable = TRUE;
    OSSemPost(_MsOS_Timer_Mutex);
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
    MS_U8 u8Err;


    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (FALSE == _MsOS_Timer_Info[s32TimerId].bEnable)
    {
        return TRUE;
    }
    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
    _MsOS_Timer_Info[s32TimerId].bEnable = FALSE;
    OSSemPost(_MsOS_Timer_Mutex);
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }
    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
    _MsOS_Timer_Info[s32TimerId].bEnable = FALSE;
    _MsOS_Timer_Info[s32TimerId].u32FirstTimeMs = u32FirstTimeMs;
    if (bStartTimer)
    {
        _MsOS_Timer_Info[s32TimerId].u32TimeTrig +=  MsOS_GetSystemTime();
        _MsOS_Timer_Info[s32TimerId].bEnable = TRUE;
    }
    else
    {
        _MsOS_Timer_Info[s32TimerId].u32TimeTrig =  0xFFFFFFFF;
        _MsOS_Timer_Info[s32TimerId].bEnable = FALSE;
    }
    OSSemPost(_MsOS_Timer_Mutex);
    return TRUE;
}
#else
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
    MS_S32 s32Id;
    MS_U8 u8Err;

    OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
    for (s32Id = 0; s32Id < MSOS_TIMER_MAX; s32Id++)
    {
        if (_MsOS_Timer_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if (s32Id < MSOS_TIMER_MAX)
    {
        _MsOS_Timer_Info[s32Id].bUsed = TRUE;
    }
    OSSemPost(_MsOS_Timer_Mutex);

    if (s32Id >= MSOS_TIMER_MAX)
    {
        return -1;
    }

    // Call OS kernel to create a Timer
    if (u32PeriodTimeMs)
    {
        _MsOS_Timer_Info[s32Id].stTimer = OSTmrCreate(u32FirstTimeMs, u32PeriodTimeMs, OS_TMR_OPT_PERIODIC,
                (OS_TMR_CALLBACK)pTimerCb, (void *)s32Id, (UBYTE *)pTimerName, &u8Err);
    }
    else
    {
        _MsOS_Timer_Info[s32Id].stTimer = OSTmrCreate(u32FirstTimeMs, u32PeriodTimeMs, OS_TMR_OPT_ONE_SHOT,
                (OS_TMR_CALLBACK)pTimerCb, (void *)s32Id, (UBYTE *)pTimerName, &u8Err);
    }

    if (OS_NO_ERR != u8Err)
    {
        OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
        _MsOS_Timer_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_Timer_Mutex);
        return -1;
    }

    if (bStartTimer)
    {
        if (!OSTmrStart(_MsOS_Timer_Info[s32Id].stTimer, &u8Err))
        {
            OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
            _MsOS_Timer_Info[s32Id].bUsed = FALSE;
            OSSemPost(_MsOS_Timer_Mutex);
            return -1;
        }
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (_MsOS_Timer_Info[s32TimerId].bUsed)
    {
        if (OSTmrDel(_MsOS_Timer_Info[s32TimerId].stTimer, &u8Err))
        {
            OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
            _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
            OSSemPost(_MsOS_Timer_Mutex);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (_MsOS_Timer_Info[s32TimerId].bUsed)
    {
        if (OSTmrStart(_MsOS_Timer_Info[s32TimerId].stTimer, &u8Err))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
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
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (_MsOS_Timer_Info[s32TimerId].bUsed)
    {
        if (OSTmrStop(_MsOS_Timer_Info[s32TimerId].stTimer, OS_TMR_OPT_NONE, 0, &u8Err))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
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
MS_BOOL MsOS_ResetTimer (MS_S32     s32TimerId,
                         MS_U32     u32FirstTimeMs,
                         MS_U32     u32PeriodTimeMs,
                         MS_BOOL    bStartTimer)
{
    MS_U8 u8Err;

    if ((s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    if (!OSTmrStop(_MsOS_Timer_Info[s32TimerId].stTimer, OS_TMR_OPT_NONE, 0, &u8Err))
    {
        return FALSE;
    }
    _MsOS_Timer_Info[s32TimerId].stTimer->OSTmrDly = u32FirstTimeMs;
    _MsOS_Timer_Info[s32TimerId].stTimer->OSTmrPeriod = u32PeriodTimeMs;

    if (bStartTimer)
    {
        if (!OSTmrStart(_MsOS_Timer_Info[s32TimerId].stTimer, &u8Err))
        {
            OSSemPend(_MsOS_Timer_Mutex, 0, &u8Err);
            _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
            OSSemPost(_MsOS_Timer_Mutex);
            return FALSE;
        }
    }

    return TRUE;
}
#endif


//
// System time
//
//-------------------------------------------------------------------------------------------------
/// Get current system time in ms
/// @return system time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTime (void)
{
    //return (OSTimeGet() / TICK_PER_ONE_MS);
    return (OSTimeGet() * 10);
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


#if UCOS_V1
/// utopia driver might not use Queue
#else
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
MS_S32 MsOS_CreateQueue(void            *pStartAddr,
                        MS_U32          u32QueueSize,
                        MessageType     eMessageType,
                        MS_U32          u32MessageSize,
                        MsOSAttribute   eAttribute,
                        char            *pQueueName)
{
    MS_S32 s32Id;
    MS_U8 u8Err;

    OSSemPend(_MsOS_Queue_Mutex, 0, &u8Err);
    for (s32Id = 0; s32Id < MSOS_QUEUE_MAX; s32Id++)
    {
        if (_MsOS_Queue_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if (s32Id < MSOS_QUEUE_MAX)
    {
        _MsOS_Queue_Info[s32Id].bUsed = TRUE;
        _MsOS_Queue_Info[s32Id].u32AlignedMsgSize = sizeof(MS_U32) + ALIGN_4(u32MessageSize); // header: msg_len(MS_U32)

        // Allocate memory for queue.
        _MsOS_Queue_Info[s32Id].pMsgTbl = dlmalloc((sizeof(void *) * u32QueueSize));
        _MsOS_Queue_Info[s32Id].pu8MsgQueue = dlmalloc((_MsOS_Queue_Info[s32Id].u32AlignedMsgSize * u32QueueSize));
        if (!_MsOS_Queue_Info[s32Id].pMsgTbl || !_MsOS_Queue_Info[s32Id].pu8MsgQueue)
        {
            _MsOS_Queue_Info[s32Id].bUsed = FALSE;
            return -1;
        }
        _MsOS_Queue_Info[s32Id].u32QueueSize = u32QueueSize;
        _MsOS_Queue_Info[s32Id].u32MsgWriteIdx = 0;
        _MsOS_Queue_Info[s32Id].eMsgType = eMessageType;
    }
    OSSemPost(_MsOS_Queue_Mutex);

    if (s32Id >= MSOS_QUEUE_MAX)
    {
        return -1;
    }

    _MsOS_Queue_Info[s32Id].stMutex = OSSemCreate(1);
    if (!_MsOS_Queue_Info[s32Id].stMutex)
    {
        OSSemPend(_MsOS_Queue_Mutex, 0, &u8Err);
        _MsOS_Queue_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_Queue_Mutex);
        s32Id = -1;
    }

    // no fifo order for ucos
    // pQueueName is not used for ucos

    _MsOS_Queue_Info[s32Id].stQueue = OSQCreate(&(_MsOS_Queue_Info[s32Id].pMsgTbl), u32QueueSize);
    if (!_MsOS_Queue_Info[s32Id].stQueue )
    {
        OSSemPend(_MsOS_Queue_Mutex, 0, &u8Err);
        _MsOS_Queue_Info[s32Id].bUsed = FALSE;
        OSSemPost(_MsOS_Queue_Mutex);
        s32Id = -1;
    }

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
    MS_U8 u8Err;

    if ((s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    OSQDel(_MsOS_Queue_Info[s32QueueId].stQueue, OS_DEL_ALWAYS, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }

    if (_MsOS_Queue_Info[s32QueueId].pMsgTbl)
    {
        dlfree(_MsOS_Queue_Info[s32QueueId].pMsgTbl);
    }
    if (_MsOS_Queue_Info[s32QueueId].pu8MsgQueue)
    {
        dlfree(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue);
    }

    OSSemDel(_MsOS_Queue_Info[s32QueueId].stMutex, OS_DEL_ALWAYS, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }

    OSSemPend(_MsOS_Queue_Mutex, 0, &u8Err);
    _MsOS_Queue_Info[s32QueueId].pMsgTbl = NULL;
    _MsOS_Queue_Info[s32QueueId].pu8MsgQueue = NULL;
    _MsOS_Queue_Info[s32QueueId].bUsed = FALSE;
    OSSemPost(_MsOS_Queue_Mutex);
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
    MS_BOOL bRet = FALSE;
    MS_U32 *pu32MsgPos;
    MS_U8 u8Err;

    if ((s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    if (pu8Message == NULL)
    {
        return FALSE;
    }

    if (u32Size > _MsOS_Queue_Info[s32QueueId].u32AlignedMsgSize)
    {
        return FALSE;
    }

    if (!_MsOS_Queue_Info[s32QueueId].pu8MsgQueue)
    {
        return FALSE;
    }

    if (u32WaitMs != 0)
    {
        MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    }

    if (u32WaitMs == 0)
    {
#if UCOS_V1
        printf("[%s][%d] MsOS_SendToQueue for time out 0 is not accepted\n", __FUNCTION__, __LINE__);
        MAsm_CPU_SwDbgBp();
#else
        if (!OSSemAccept(_MsOS_Queue_Info[s32QueueId].stMutex))
        {
            return FALSE;
        }
#endif
    }
    else if (u32WaitMs == MSOS_WAIT_FOREVER)
    {
        OSSemPend(_MsOS_Queue_Info[s32QueueId].stMutex, 0, &u8Err);
        if (OS_NO_ERR != u8Err)
        {
            return FALSE;
        }
    }
    else
    {
        OSSemPend(_MsOS_Queue_Info[s32QueueId].stMutex, u32WaitMs, &u8Err);
        if (OS_NO_ERR != u8Err)
        {
            return FALSE;
        }
    }

    // Copy message.
    pu32MsgPos = (MS_U32*)(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue +
            _MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx * _MsOS_Queue_Info[s32QueueId].u32AlignedMsgSize);

    pu32MsgPos[0] = u32Size;
    memcpy(pu32MsgPos + 1, pu8Message, u32Size);

    if (OSQPost(_MsOS_Queue_Info[s32QueueId].stQueue, pu32MsgPos) == OS_NO_ERR)
    {
        _MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx =
            (_MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx + 1) % _MsOS_Queue_Info[s32QueueId].u32QueueSize;
        bRet = TRUE;
    }
    else
    {
        if (u32WaitMs != 0) // No support blocking mode
        {
            printf("[%s][%d] Blocking is not supported\n", __FUNCTION__, __LINE__);
        }
    }

    OSSemPost(_MsOS_Queue_Info[s32QueueId].stMutex);
    return bRet;
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
    MS_BOOL bRet = FALSE;
    void *pItem;
    MS_U8 u8Err;
    MS_U32 u32MsgLen;

    if ((s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs == 0) //non-blocking
    {
        pItem = OSQAccept(_MsOS_Queue_Info[s32QueueId].stQueue, &u8Err);
    }
    else if (u32WaitMs == MSOS_WAIT_FOREVER) //blocking wait
    {
        pItem = OSQPend(_MsOS_Queue_Info[s32QueueId].stQueue, 0, &u8Err);
    }
    else //blocking wait with timeout
    {
        pItem = OSQPend(_MsOS_Queue_Info[s32QueueId].stQueue, u32WaitMs, &u8Err);
    }

    if (pItem == NULL)
    {
        *pu32ActualSize = 0;
        bRet = FALSE;
    }
    else
    {
        u32MsgLen = *(MS_U32*)pItem;
        if (u32IntendedSize < u32MsgLen)
        {
            *pu32ActualSize = u32IntendedSize;
        }
        else
        {
            *pu32ActualSize = u32MsgLen;
        }
        memcpy(pu8Message, ((MS_U32*)pItem)+1, *pu32ActualSize);
        bRet = TRUE;
    }

    return bRet;
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
    MS_BOOL bRet = FALSE;
    void *pItem;
    MS_U8 u8Err;
    MS_U32 u32MsgLen;

    if ((s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    pItem = OSQAccept(_MsOS_Queue_Info[s32QueueId].stQueue, &u8Err);

    if (pItem == NULL)
    {
        *pu32ActualSize = 0;
        bRet = FALSE;
    }
    else
    {
        u32MsgLen = *(MS_U32*)pItem;
        if (u32IntendedSize < u32MsgLen)
        {
            *pu32ActualSize = u32IntendedSize;
        }
        else
        {
            *pu32ActualSize = u32MsgLen;
        }
        memcpy(pu8Message, ((MS_U32*)pItem)+1, *pu32ActualSize);
        bRet = TRUE;
    }

    return bRet;
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
    if ((s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    //ToDo
    return FALSE;
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
    UBYTE u8Err;

    OSSemPend (_MsOS_HISR_Mutex, UCOS_FOREVER, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }
    CHIP_AttachISR(eIntNum, pIntCb);
    OSSemPost(_MsOS_HISR_Mutex);
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
    UBYTE u8Err;

    OSSemPend (_MsOS_HISR_Mutex, UCOS_FOREVER, &u8Err);
    if (OS_NO_ERR != u8Err)
    {
        return FALSE;
    }
    CHIP_DetachISR(eIntNum);
    OSSemPost(_MsOS_HISR_Mutex);
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
    return CHIP_EnableIRQ(eIntNum);
}

//-------------------------------------------------------------------------------------------------
/// Disable (mask) the interrupt #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DisableInterrupt (InterruptNum eIntNum)
{
    CHIP_DisableIRQ(eIntNum);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_DisableAllInterrupts(void)
{
#if UCOS_V1
    return (MS_U32) MSCPU_OSCPUSaveSR();
#else
    OS_CPU_SR  cpu_sr = 0;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    cpu_sr = OSCPUSaveSR();
#endif
    return cpu_sr;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
#if UCOS_V1
    MSCPU_OSCPURestoreSR(u32OldInterrupts);
#else
    OS_CPU_SR  cpu_sr = (OS_CPU_SR)u32OldInterrupts;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OSCPURestoreSR(cpu_sr);
#endif
#endif
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
#if UCOS_V1
    MSCPU_OSCPUEnableSR();
#else
    OSCPUEnableSR();
#endif
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
    // return CHIP_InISRContext();
    // CHIP_InISRContext();
    return (OSIntNesting > 0) ? TRUE : FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
#define DCACHE_LINE_SIZE 16
extern void DCACHE_vFlushAddr(U32 wStartAddr, U32 wEndAddr);
extern void DCACHE_vInvalidateAddr(U32 wStartAddr, U32 wEndAddr);
MS_BOOL MsOS_Dcache_Flush( MS_VIRT ptrStart, MS_SIZE tSize  )
{
    MS_U32 u32OldIntr;
    MS_U32 u32StartAddr;
    MS_U32 u32EndAddr;

    u32StartAddr = ptrStart & 0xFFFFFFF0;
    tSize+= (ptrStart - u32StartAddr);
    tSize = ALIGN_16(tSize);

    if ((u32StartAddr % DCACHE_LINE_SIZE) || ((u32StartAddr & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE))
    {
        return FALSE;
    }
    u32EndAddr = u32StartAddr+tSize;
    u32OldIntr = MsOS_DisableAllInterrupts();
    DCACHE_vFlushAddr(u32StartAddr, u32EndAddr);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
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
    MS_U32 u32OldIntr;
    MS_U32 u32StartAddr;
    MS_U32 u32EndAddr;

    u32StartAddr = ptrStart & 0xFFFFFFF0;
    tSize+= (ptrStart - u32StartAddr);
    tSize = ALIGN_16(tSize);
    if ( (ptrStart % DCACHE_LINE_SIZE) || ( (ptrStart & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE) )
    {
        return FALSE;
    }
    u32EndAddr = u32StartAddr + tSize;
    u32OldIntr = MsOS_DisableAllInterrupts();
    DCACHE_vInvalidateAddr(u32StartAddr, u32EndAddr);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
}

#ifdef UCOS
//-------------------------------------------------------------------------------------------------
/// Write back if dirty the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Writeback( MS_U32 u32Start, MS_U32 u32Size )
{
    MS_U32 u32OldIntr;
    if ( (u32Start % DCACHE_LINE_SIZE) || ( (u32Start & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE) )
    {
        return FALSE;
    }
    u32OldIntr = MsOS_DisableAllInterrupts();
    printf("WARNING: this ecos doesn't enable the dcache_store\b");
//TODO:    HAL_DCACHE_STORE( u32Start, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
}
#endif //UCOS

MS_PHY MsOS_VA2PA(MS_VIRT addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3) || defined(CHIP_JANUS))
        return ((MS_U32)(addr) & ~(0xC0000000));
    #else
        return ((MS_U32)(addr) & ~(0x80000000));
    #endif
#else
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_EIFFEL) || defined(CHIP_NIKE) || defined(CHIP_MADISON) || defined(CHIP_CLIPPERS) || defined(CHIP_MIAMI) || defined(CHIP_NADAL) || defined(CHIP_MUNICH))
            // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000
            // miu0 (low 256MB) (VA)0xA000:0000~0xB000:0000 --> (PA)0x0000:0000~0x1000:0000
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr & 0x1fffffff);
            }
            if ((0xA0000000 <= addr) && (0xB0000000 > addr))
            {
                return (addr & 0x1fffffff);
            }
            // miu0 (high 256MB) (VA)0xC000:0000~0xD000:0000 -> (PA)0x1000:0000~0x2000:0000
            // miu0 (high 256MB) (VA)0xD000:0000~0xE000:0000 -> (PA)0x1000:0000~0x2000:0000
            if ((0xC0000000 <= addr) && (0xD0000000 > addr))
            {
                return ((addr - 0xC0000000) + 0x10000000);
            }
            if ((0xD0000000 <= addr) && (0xE0000000 > addr))
            {
                return ((addr - 0xD0000000) + 0x10000000);
            }
            // miu1 (low 256MB) (VA)0x0000:0000~0x1000:0000 -> (PA)0x2000:0000~0x3000:0000
            // miu1 (low 256MB) (VA)0x1000:0000~0x2000:0000 -> (PA)0x2000:0000~0x3000:0000
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr + 0x20000000);
            }
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return (addr + 0x10000000);
            }
            // miu1 (high 256MB) (VA)0x2000:0000~0x3000:0000 -> (PA)0x3000:0000~0x4000:0000
            // miu1 (high 256MB) (VA)0x3000:0000~0x4000:0000 -> (PA)0x3000:0000~0x4000:0000
            if ((0x20000000 <= addr) && (0x30000000 > addr))
            {
                return (addr + 0x10000000);
            }
            if ((0x30000000 <= addr) && (0x40000000 > addr))
            {
                return addr;
            }
            return 0;
        #elif (defined(CHIP_K1)||defined(CHIP_KELTIC))
            // K1 has only 1 MIU
            // miu0
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
            }
            if ((0xA0000000 <= addr) && (0xB0000000 > addr))
            {
                return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
            }

            return 0;
         #elif(defined(CHIP_K6) || defined(CHIP_K6LITE)|| (defined(CHIP_K7U)))
            // miu0 (low 256MB) (VA)0x0000:0000~0x1000:0000 --> (PA)0x0000:0000~0x1000:0000
            // miu0 (low 256MB) (VA)0x2000:0000~0x3000:0000 --> (PA)0x0000:0000~0x1000:0000
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr);
            }
            if ((0x20000000 <= addr) && (0x30000000 > addr))
            {
                return (addr & 0x0FFFFFFF);
            }
            // miu0 (high 256MB) (VA)0x1000:0000~0x2000:0000 -> (PA)0x1000:0000~0x2000:0000
            // miu0 (high 256MB) (VA)0x3000:0000~0x4000:0000 -> (PA)0x1000:0000~0x2000:0000
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return (addr);
            }
            if ((0x30000000 <= addr) && (0x40000000 > addr))
            {
                return (addr - 0x20000000);
            }
            // miu1 (low 256MB) (VA)0x4000:0000~0x5000:0000 -> (PA)0x8000:0000~09000:0000
            // miu1 (low 256MB) (VA)0x6000:0000~0x7000:0000 -> (PA)0x8000:0000~0x9000:0000
            if ((0x40000000 <= addr) && (0x50000000 > addr))
            {
                return (addr + 0x40000000);
            }
            if ((0x60000000 <= addr) && (0x70000000 > addr))
            {
                return (addr + 0x20000000);
            }
            // miu1 (high 256MB) (VA)0x5000:0000~0x6000:0000 -> (PA)0x9000:0000~0xA000:0000
            // miu1 (high 256MB) (VA)0x7000:0000~0x8000:0000 -> (PA)0x9000:0000~0xA000:0000
            if ((0x50000000 <= addr) && (0x60000000 > addr))
            {
                return (addr + 0x40000000);
            }
            if ((0x70000000 <= addr) && (0x80000000 > addr))
            {
                return  (addr + 0x20000000);
            }
            return 0;
        #elif(defined(CHIP_KANO)) // Kano : 1G *512 solution
            // miu0 (0~256MB) (VA)0x0000:0000~0x1000:0000 --> (PA)0x0000:0000~0x1000:0000
            // miu0 (0~256MB) (VA)0x4000:0000~0x5000:0000 --> (PA)0x0000:0000~0x1000:0000
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr);
            }
            if ((0x40000000 <= addr) && (0x50000000 > addr))
            {
                return (addr - 0x40000000);
            }
            // miu0 (256MB~512MB) (VA)0x1000:0000~0x2000:0000 -> (PA)0x1000:0000~0x2000:0000
            // miu0 (256MB~512MB) (VA)0x5000:0000~0x6000:0000 -> (PA)0x1000:0000~0x2000:0000
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return (addr);
            }
            if ((0x50000000 <= addr) && (0x60000000 > addr))
            {
                return (addr - 0x40000000);
            }
            // miu0 (512~768MB) (VA)0x2000:0000~0x3000:0000 -> (PA)0x2000:0000~0x3000:0000
            // miu0 (512~768MB) (VA)0x6000:0000~0x7000:0000 -> (PA)0x2000:0000~0x3000:0000
            if ((0x20000000 <= addr) && (0x30000000 > addr))
            {
                return (addr);
            }
            if ((0x60000000 <= addr) && (0x70000000 > addr))
            {
                return (addr - 0x40000000);
            }
            // miu0 (768MB~1024MB) (VA)0x3000:0000~0x4000:0000 -> (PA)0x3000:0000~0x4000:0000
            // miu0 (768MB~1024MB) (VA)0x7000:0000~0x8000:0000 -> (PA)0x3000:0000~0x4000:0000
            if ((0x30000000 <= addr) && (0x40000000 > addr))
            {
                return (addr);
            }
            if ((0x70000000 <= addr) && (0x80000000 > addr))
            {
                return (addr - 0x40000000);
            }
            // miu1 (low 256MB) (VA)0x8000:0000~0x9000:0000 -> (PA)0x8000:0000~09000:0000
            // miu1 (low 256MB) (VA)0xA000:0000~0xB000:0000 -> (PA)0x8000:0000~0x9000:0000
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr);
            }
            if ((0xA0000000 <= addr) && (0xB0000000 > addr))
            {
                return (addr - 0x20000000);
            }
            // miu1 (high 256MB) (VA)0x9000:0000~0xA000:0000 -> (PA)0x9000:0000~0xA000:0000
            // miu1 (high 256MB) (VA)0xB000:0000~0xC000:0000 -> (PA)0x9000:0000~0xA000:0000
            if ((0x90000000 <= addr) && (0xA0000000 > addr))
            {
                return (addr);
            }
            if ((0xB0000000 <= addr) && (0xC0000000 > addr))
            {
                return  (addr - 0x20000000);
            }
        #else
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
        #endif
    #else
        return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
    #endif
#endif
}

MS_VIRT MsOS_PA2KSEG0(MS_PHY addr)
{
#if (defined(MCU_AEON))
    return (addr);
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_EIFFEL) || defined(CHIP_NIKE) || defined(CHIP_MADISON) || defined(CHIP_CLIPPERS) || defined(CHIP_MIAMI) || defined(CHIP_NADAL) || defined(CHIP_MUNICH))
            // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> (VA)0x8000:0000~0x9000:0000 cached
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr + 0x80000000);
            }
            // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0xC000:0000~0xD000:0000 cached
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return ((addr - 0x10000000) + 0xC0000000);
            }
            // miu1 (low 256MB) - (PA)0x2000:0000~0x3000:0000 -> (VA)0x0000:0000~0x1000:0000 cached
            if ((0x20000000 <= addr) && (0x30000000 > addr))
            {
                return (addr - 0x20000000);
            }
            // miu1 (high 256MB) - (PA)0x3000:0000~0x4000:0000 -> (VA)0x2000:0000~0x3000:0000 cached
            if ((0x30000000 <= addr) && (0x40000000 > addr))
            {
                return (addr - 0x10000000);
            }
            return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC))
            // miu0
            if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
            {
                return ((MS_U32)(((MS_U32)addr) | 0x80000000));
            }
            return 0;
        #elif(defined(CHIP_K6) ||defined(CHIP_K6LITE)||defined(CHIP_K7U))
            // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> (VA)0x0000:0000~0x1000:0000 cached
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr);
            }
            // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0x1000:0000~0x2000:0000 cached
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return (addr);
            }
            // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> (VA)0x4000:0000~0x5000:0000 cached
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr - 0x40000000);
            }
            // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> (VA)0x5000:0000~0x6000:0000 cached
            if ((0x90000000 <= addr) && (0xA0000000 > addr))
            {
                return (addr - 0x40000000);
            }
            return 0;
        #elif defined(CHIP_KANO) // Kano : 1G * 512 solution
            // miu0 (0~1024MB) - (PA)0x0000:0000~0x4000:0000 -> (VA)0x0000:0000~0x4000:0000 cached
            if ((0x00000000 <= addr) && (0x40000000 > addr))
            {
                return (addr);
            }
            // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> (VA)0x8000:0000~0x9000:0000 cached
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr);
            }
            // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> (VA)0x9000:0000~0xA000:0000 cached
            if ((0x90000000 <= addr) && (0xA0000000 > addr))
            {
                return (addr);
            }
            return 0;
        #else
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
        #endif
    #else
        return ((MS_U32)(((MS_U32)addr) | 0x80000000));
    #endif
#endif
}

MS_VIRT MsOS_PA2KSEG1(MS_PHY addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3))
        return ((MS_U32)(addr) | (0xC0000000));
    #else
        return ((MS_U32)(addr) | (0x80000000));
    #endif
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_EIFFEL) || defined(CHIP_NIKE) || defined(CHIP_MADISON) || defined(CHIP_CLIPPERS) || defined(CHIP_MIAMI) || defined(CHIP_NADAL) || defined(CHIP_MUNICH))
            // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> 0xA000:0000~0xB000:0000 uncached
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr + 0xA0000000);
            }
            // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> 0xD000:0000~0xE000:0000 uncached
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return ((addr - 0x10000000) + 0xD0000000);
            }
            // miu1 (low 256MB) - (PA)0x2000:0000~0x3000:0000 -> 0x1000:0000~0x2000:0000 uncached
            if ((0x20000000 <= addr) && (0x30000000 > addr))
            {
                return (addr - 0x10000000);
            }
            // miu1 (high 256MB) - (PA)0x3000:0000~0x4000:0000 -> 0x3000:0000~0x4000:0000 uncached
            if ((0x30000000 <= addr) && (0x40000000 > addr))
            {
                return addr;
            }
            return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC))
            // miu0
            if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
            {
                return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
            }
            return 0;
        #elif (defined(CHIP_K6) ||defined(CHIP_K6LITE)||defined(CHIP_K7U))
            // miu0 (low 256MB) - (PA)0x0000:0000~0x1000:0000 -> 0x2000:0000~0x3000:0000 uncached
            if ((0x00000000 <= addr) && (0x10000000 > addr))
            {
                return (addr + 0x20000000);
            }
            // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> 0x3000:0000~0x4000:0000 uncached
            if ((0x10000000 <= addr) && (0x20000000 > addr))
            {
                return (addr + 0x20000000);
            }
            // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> 0x6000:0000~0x7000:0000 uncached
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr - 0x20000000);
            }
            // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> 0x7000:0000~0x8000:0000 uncached
            if ((0x90000000 <= addr) && (0xA0000000 > addr))
            {
                return (addr - 0x20000000);
            }
            return 0;
        #elif defined(CHIP_KANO) // Kano : 1G * 512 solution
            // miu0 (0~1024MB) - (PA)0x0000:0000~0x4000:0000 -> 0x4000:0000~0x8000:0000 uncached
            if ((0x00000000 <= addr) && (0x40000000 > addr))
            {
                return (addr + 0x40000000);
            }
            // miu1 (low 256MB) - (PA)0x8000:0000~0x9000:0000 -> 0xA000:0000~0xB000:0000 uncached
            if ((0x80000000 <= addr) && (0x90000000 > addr))
            {
                return (addr + 0x20000000);
            }
            // miu1 (high 256MB) - (PA)0x9000:0000~0xA000:0000 -> 0xB000:0000~0xC000:0000 uncached
            if ((0x90000000 <= addr) && (0xA0000000 > addr))
            {
                return (addr + 0x20000000);
            }
            return 0;
        #else
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
        #endif
    #else
        return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
    #endif
#endif
}

#define SHM_SIZE                (200*1024)
#define MAX_CLIENT_NUM          200
typedef struct
{
    MS_U8       u8ClientName[MAX_CLIENT_NAME_LENGTH+ 1];
    MS_U32      u32Offset;
    MS_U32      u32Size;
    MS_U8       u8ClientId;    // 0 means "never used"
    MS_U8       u8RefCnt;
    MS_U8       u8Dummy[2];
} MsOS_SHM_Context;

typedef struct
{
    MS_U32              u32MaxClientNum;
    MS_U32              u32ClientNum;
    MS_U32              u32ShmSize;
    MS_U32              u32Offset;
    MsOS_SHM_Context    context[MAX_CLIENT_NUM];
} MsOS_SHM_Hdr;

static int _shm_id = -1;
static MS_U8 *_pu8ShareMem = NULL;
static MsOS_SHM_Hdr _ShmHdr; // dummy storage

// Share memory operation
// MS_BOOL MsOS_SHM_Init(MS_U32 u32ShmSize)
MS_BOOL MsOS_SHM_Init(void)
{
    MS_BOOL bInit = FALSE;
    MS_U32 u32ShmSize;

    if (-1 != _shm_id)
    {
        return FALSE;
    }

    u32ShmSize = SHM_SIZE;
    u32ShmSize += sizeof(MsOS_SHM_Hdr);
    u32ShmSize += ((1<< 12)- 1);
    u32ShmSize = (u32ShmSize>> 12)<< 12; // make it 4KBytes alignment

    if (NULL == (_pu8ShareMem = (MS_U8*)malloc(u32ShmSize)))
    {
        printf("[%s][%d] fail, ask for 0x%X mem_size\n", __FUNCTION__, __LINE__, (unsigned int)u32ShmSize);
        return FALSE;
    }
    bInit = TRUE;
    if (bInit)
    {
#if 0
        MsOS_SHM_Hdr* pHdr;

        // printf("[%s][%d] Clear share memory header\n", __FUNCTION__, __LINE__);
        memset(_pu8ShareMem, 0, sizeof(MsOS_SHM_Hdr));
        pHdr = (MsOS_SHM_Hdr*) _pu8ShareMem;

        pHdr->u32MaxClientNum = MAX_CLIENT_NUM;
        pHdr->u32ClientNum = 0;
        pHdr->u32ShmSize = u32ShmSize;
        pHdr->u32Offset = (sizeof(MsOS_SHM_Hdr)+3)&~3;
#else
        memset(&_ShmHdr, 0, sizeof(_ShmHdr)); // dummy storage
        _ShmHdr.u32MaxClientNum = MAX_CLIENT_NUM;
        _ShmHdr.u32ClientNum = 0;
        _ShmHdr.u32ShmSize = u32ShmSize;
        _ShmHdr.u32Offset = (sizeof(MsOS_SHM_Hdr)+7)&~7;
        memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
#endif
    }
    return TRUE;
}

// Share memory operation
MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_VIRT* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag)
{
    MsOS_SHM_Context* pContext = NULL;
    MsOS_SHM_Context* pClient = NULL;
    MS_U32 i;
    MS_U32 u32CopyLen;

    if (!_pu8ShareMem)
    {
        printf("[%s][%d] MsOS_SHM_Init should be invoked first\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    memcpy(&_ShmHdr, _pu8ShareMem, sizeof(_ShmHdr));
    pContext = (MsOS_SHM_Context*)_ShmHdr.context;
    for (i= 0; i< MAX_CLIENT_NUM; i++, pContext++)
    {
        if (0 == pContext->u8ClientId)
        {
            continue;
        }
        if (0== strcmp((const char*)pContext->u8ClientName, (const char*)pu8ClientName))
        {
            pClient = pContext;
            if (u32BufSize != pClient->u32Size)
            {
                printf("[%s][%d] MsOS_SHM_GetId: inconsistent buffer size with other process\n", __FUNCTION__, __LINE__);
            }
            break;
        }
    }
    if ((NULL == pClient) && (MSOS_SHM_CREATE == u32Flag))
    {
        pContext = (MsOS_SHM_Context*)_ShmHdr.context;
        for (i= 0; i< MAX_CLIENT_NUM; i++, pContext++)
        {
            if (pContext->u8ClientId)
            {
                continue;
            }
            if ((_ShmHdr.u32Offset + u32BufSize)> _ShmHdr.u32ShmSize)
            {
                printf("[%s][%d] MsOS_SHM_GetId: shared memory buffer overflow\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            _ShmHdr.u32ClientNum++;
            pClient = pContext;
            u32CopyLen = MIN((int)strlen((const char*)pu8ClientName), MAX_CLIENT_NAME_LENGTH);
            strncpy((char*)pClient->u8ClientName, (const char*)pu8ClientName, u32CopyLen);
            pClient->u32Size = u32BufSize;
            pClient->u8ClientId = i + 1;
            pClient->u8RefCnt = 0;
            pClient->u32Offset = _ShmHdr.u32Offset;
            _ShmHdr.u32Offset += (u32BufSize+7)&~7;
            memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
            break;
        }
    }
    if (NULL == pClient)
    {
        if(MSOS_SHM_QUERY != u32Flag)
        {
            printf("[%s][%d] MsOS_SHM_Init: Unable to get available share memeory\n", __FUNCTION__, __LINE__);
        }
        else
        {
            //printf("[%s][%d] MsOS_SHM_Init: Unable to query share memeory[%s]\n", __FUNCTION__, __LINE__, (char *)pu8ClientName);
        }
        return FALSE;
    }

    *pu32ShmId = pClient->u8ClientId;
    *pu32BufSize = pClient->u32Size;
    *pu32Addr = (MS_VIRT)_pu8ShareMem + pClient->u32Offset;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable the CPU interrupt
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_CPU_DisableInterrupt (void)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Detach the CPU exception callback function to interrupt #
/// @param  eIntNum \b IN: Exception number in enumerator InterruptNum
/// @param  pIntCb  \b IN: Exception callback function
/// @param  dat  \b IN: Data
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_DetachException (MHAL_EXCEPTION_TYPE expt_num)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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

void MsOS_FlushMemory(void)
{
    HAL_MMIO_FlushMemory();
}

void MsOS_ReadMemory(void)
{
    HAL_MMIO_ReadMemory();
}

MS_U32 MsOS_GetKattribute(char *pAttr)
{
    // This is for linux only, do nothing here
    return 0;
}
#endif
