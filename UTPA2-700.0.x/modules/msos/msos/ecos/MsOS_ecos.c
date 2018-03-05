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

#if defined (MSOS_TYPE_ECOS)

#include "cyg/kernel/kapi.h"
#include "cyg/hal/hal_arch.h"
#include "cyg/hal/hal_diag.h"
//POSIX API
//#include <fcntl.h>
//#include <mqueue.h>

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"
#include "utopia.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
// Switch tasks every 1 ms.
#define TASK_TIME_SLICE             (TICK_PER_ONE_MS)
#define DCACHE_LINE_SIZE 16

// Combine 3-B prefix with s32ID = MSOS_ID_PREFIX | s32Id
//  to avoid the kernel object being used before initialzed.
#define MSOS_ID_PREFIX              0x76540000
#define MSOS_ID_PREFIX_MASK         0xFFFF0000
#define MSOS_ID_MASK                0x0000FFFF //~MSOS_ID_PREFIX_MASK

#if (defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_T9) || defined(CHIP_U4) || defined(CHIP_T13) || defined(CHIP_A1) || defined(CHIP_A6) || defined(CHIP_A7) || defined(CHIP_AMETHYST) || defined(CHIP_EMERALD) || defined(CHIP_NUGGET) || defined(CHIP_NIKON))
#define MIU1_CACHEABLE_START        0xC0000000
#define MIU1_CACHEABLE_END          0xD0000000
#define MIU1_UNCACHEABLE_START      0xD0000000
#define MIU1_UNCACHEABLE_END        0xE0000000
#elif defined(CHIP_K2)
#define MIU1_CACHEABLE_START        0x00000000
#define MIU1_CACHEABLE_END          0x10000000
#define MIU1_UNCACHEABLE_START      0x10000000
#define MIU1_UNCACHEABLE_END        0x20000000
#elif defined(CHIP_KAISER)
#define MIU1_CACHEABLE_START        0x60000000
#define MIU1_CACHEABLE_END          0x50000000
#define MIU1_UNCACHEABLE_START      0x60000000
#define MIU1_UNCACHEABLE_END        0x80000000
#endif

#if (defined(CHIP_K6)||defined(CHIP_K6LITE)||defined(CHIP_K7U)||defined(CHIP_KANO))
//cached/unchched segment
#define KSEG0_BASE              ((void *)0x00000000)
#define KSEG1_BASE              ((void *)0x20000000)
#define KSEG0_SIZE              0x20000000
#define KSEG1_SIZE              0x20000000

//cached addr <-> unchched addr
#define KSEG02KSEG1(addr)       ((void *)((MS_U32)(addr)|0x20000000))  //cached -> unchched
#define KSEG12KSEG0(addr)       ((void *)((MS_U32)(addr)&~0x20000000)) //unchched -> cached

//virtual addr <-> physical addr
#define VA2PA(addr)             ((void *)(((MS_U32)addr) & 0x1fffffff)) //virtual -> physical
#define PA2KSEG0(addr)          ((void *)(((MS_U32)addr) | 0x00000000)) //physical -> cached
#define PA2KSEG1(addr)          ((void *)(((MS_U32)addr) | 0x20000000)) //physical -> unchched
#else
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

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define PRINT_DEBUG_INFO    0
#if PRINT_DEBUG_INFO
#define PRINT_INFO(fmt, args...)  printf("[Utopia Info]" fmt, ## args)
#else
#define PRINT_INFO(fmt, args...)
#endif


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
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

//
// Task Management
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stTask;
    cyg_thread                      stThreadInfo;
    void *                          pStackBase;
    cyg_addrword_t                  entry_data;
    TaskEntry                       pTaskEntry;
} MsOS_Task_Info;

static MsOS_Task_Info               _MsOS_Task_Info[MSOS_TASK_MAX];
static cyg_mutex_t                  _MsOS_Task_Mutex;

//
// Mutex
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_mutex_t                     stMutex;
} MsOS_Mutex_Info;

static MsOS_Mutex_Info  _MsOS_Mutex_Info[MSOS_MUTEX_MAX];
static cyg_mutex_t  _MsOS_Mutex_Mutex;

//
// Semaphore
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_sem_t                       stSemaphore;
} MsOS_Semaphore_Info;

static MsOS_Semaphore_Info          _MsOS_Semaphore_Info[MSOS_SEMAPHORE_MAX];
static cyg_mutex_t                  _MsOS_Semaphore_Mutex;

//
// Event Group
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_flag_t                      stEventGroup;
} MsOS_EventGroup_Info;

static MsOS_EventGroup_Info         _MsOS_EventGroup_Info[MSOS_EVENTGROUP_MAX];
static cyg_mutex_t                  _MsOS_EventGroup_Mutex;

//
// Queue
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stQueue;
    cyg_mbox                        stQueueInfo;
    cyg_mutex_t                     stMutex;
    MS_U8                           *pu8MsgQueue;
    MS_U32                          u32AlignedMsgSize;
    MS_U32                          u32MsgWriteIdx;
    MessageType                     eMsgType;
} MsOS_Queue_Info;

static MsOS_Queue_Info              _MsOS_Queue_Info[MSOS_QUEUE_MAX];
static cyg_mutex_t                  _MsOS_Queue_Mutex;

//
// Timer
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_handle_t                    stTimer; //alarm handler
    cyg_alarm                       stAlarm; //alarm object
} MsOS_Timer_Info;
static MsOS_Timer_Info              _MsOS_Timer_Info[MSOS_TIMER_MAX];
static cyg_mutex_t                  _MsOS_Timer_Mutex;
static cyg_handle_t                 _stCounter; //counter handle
static cyg_handle_t                 _stSysClk;  //clock handle

//
// Condition Variables
//
typedef struct
{
    MS_BOOL                         bUsed;
    cyg_cond_t                      stCond;
} MsOS_Condition_Info;
static MsOS_Condition_Info         _MsOS_Condition_Info[MSOS_CONDITION_MAX];
static cyg_mutex_t                  _MsOS_Condition_Mutex;

//
// Interrupt
//
static cyg_mutex_t                  _MsOS_HISR_Mutex;

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

void* _MsOS_eCosTaskWrapper (cyg_addrword_t data)
{
    MsOS_Task_Info *taskinfo = NULL;
    taskinfo = (MsOS_Task_Info *)data;
    taskinfo->pTaskEntry((MS_U32)taskinfo->entry_data,(void *)NULL);
    cyg_mutex_lock(&_MsOS_Task_Mutex);
    taskinfo->bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_Task_Mutex);
    cyg_thread_exit();
    return NULL;
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
    static int bInit = 0;

    if (bInit == 0)
	    bInit = 1;
    else
        return TRUE;

    if (FALSE == MsOS_SHM_Init())
    {
        return FALSE;
    }

    // Empty all the MsOS structures
    for( u32I=0; u32I<MSOS_MEMPOOL_MAX; u32I++)
    {
        _MsOS_MemoryPool_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_FIXSIZE_MEMPOOL_MAX; u32I++)
    {
        _MsOS_FixSizeMemoryPool_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_TASK_MAX; u32I++)
    {
        _MsOS_Task_Info[u32I].bUsed = FALSE;
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
    for( u32I=0; u32I<MSOS_TIMER_MAX; u32I++)
    {
        _MsOS_Timer_Info[u32I].bUsed = FALSE;
    }
    for( u32I=0; u32I<MSOS_CONDITION_MAX; u32I++)
    {
        _MsOS_Condition_Info[u32I].bUsed = FALSE;
    }

    // Please update the HISR priority here if it is necessary.

    //For MsOS_CreateTimer
    _stSysClk = cyg_real_time_clock();
    cyg_clock_to_counter( _stSysClk, &_stCounter);

    // Create Semaphore for all resources.
    cyg_mutex_init(&_MsOS_MemoryPool_Mutex);
    cyg_mutex_init(&_MsOS_FixSizeMemoryPool_Mutex);
    cyg_mutex_init(&_MsOS_Task_Mutex);
    cyg_mutex_init(&_MsOS_Mutex_Mutex);
    cyg_mutex_init(&_MsOS_Semaphore_Mutex);
    cyg_mutex_init(&_MsOS_EventGroup_Mutex);
    cyg_mutex_init(&_MsOS_Queue_Mutex);
    cyg_mutex_init(&_MsOS_Timer_Mutex);
    cyg_mutex_init(&_MsOS_Condition_Mutex);
    cyg_mutex_init(&_MsOS_HISR_Mutex);

#if(defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K))
    #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_EDISON) || defined(CHIP_EIFFEL) || defined(CHIP_NIKE) || defined(CHIP_MADISON) || defined(CHIP_NADAL) || defined(CHIP_MIAMI) || defined(CHIP_MUNICH))
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
    #elif (defined(CHIP_KENYA))
           MS_U32 entry = 0;
           extern int read_tlb_entry_number(void);
           // MIU0
           unsigned long entrylo0 = ((0x50000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
           unsigned long entrylo1 = ((0x50000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
           // MIU1
//           unsigned long entrylo0_miu1_lo256mb = ((0x60000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
//           unsigned long entrylo1_miu1_lo256mb = ((0x60000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
//           unsigned long entrylo0_miu1_hi256mb = ((0x70000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
//           unsigned long entrylo1_miu1_hi256mb = ((0x70000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7

           entry = read_tlb_entry_number();

           if(entry == 0)
           {
               // MIU0
               add_wired_entry(entrylo0, entrylo1, 0xC0000000 , 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)
           }
           else
           {
               printf("TLB had ready mapped. Not mapping again !!\n");
           }
     #elif (defined(CHIP_K2))
        MS_U32 entry = 0;
        extern int read_tlb_entry_number(void);
        // MIU0
        unsigned long entrylo0 = ((0x50000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1 = ((0x50000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        // MIU1
        unsigned long entrylo0_miu1_lo256mb = ((0x60000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1_miu1_lo256mb = ((0x60000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        unsigned long entrylo0_miu1_hi256mb = ((0x70000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1_miu1_hi256mb = ((0x70000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7

        entry = read_tlb_entry_number();

        if(entry == 0)
        {
            // MIU0
            add_wired_entry(entrylo0, entrylo1, 0xC0000000 , 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)
            // MIU1
            add_wired_entry(entrylo0_miu1_lo256mb, entrylo1_miu1_lo256mb, 0x00000000 , 0x1fffe000);
            add_wired_entry(entrylo0_miu1_hi256mb, entrylo1_miu1_hi256mb, 0x20000000 , 0x1fffe000);
        }
        else
        {
            printf("TLB had ready mapped. Not mapping again !!\n");
        }
     #elif (defined(CHIP_U4))
        unsigned long entrylo0 = ((0x40000000>> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
        unsigned long entrylo1 = ((0x40000000>> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7
        add_wired_entry(entrylo0, entrylo1, MIU1_CACHEABLE_START , 0x1fffe000);
    #endif
#endif

    CHIP_InitISR();

    //can't enable INT till LISR/HISR has been registered/attached
    MsOS_DisableInterrupt(E_INT_IRQ_FIQ_ALL);

	UtopiaInit();

    return TRUE;
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
                     MS_U32     u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL    bAutoStart,
                     void       *pStackBase,
                     MS_U32     u32StackSize,
                     char       *pTaskName)
{
    MS_S32 s32Id;

    // Check parameters
    if ( ((MS_U32)pStackBase % 4) || (u32StackSize < CYGNUM_HAL_STACK_SIZE_MINIMUM) )
    {
        return -1;
    }

    cyg_mutex_lock(&_MsOS_Task_Mutex);
    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == FALSE)
        {
            if (_MsOS_Task_Info[s32Id].stTask != 0)
            {
                if ( cyg_thread_delete(_MsOS_Task_Info[s32Id].stTask) == TRUE)
                {
                    _MsOS_Task_Info[s32Id].stTask = 0;
                }
                else
                {
                    cyg_mutex_unlock(&_MsOS_Task_Mutex);
                    return -1;
                }
            }
            break;
        }
    }
    if( s32Id < MSOS_TASK_MAX)
    {
        _MsOS_Task_Info[s32Id].bUsed = TRUE;
    }
    cyg_mutex_unlock(&_MsOS_Task_Mutex);

    if( s32Id >= MSOS_TASK_MAX)
    {
        return -1;
    }

    if (pStackBase == NULL)
    {
    /*
        #if 1 // stack autogen
        // No stack base specified create default 4K stack
        _MsOS_Task_Info[s32Id].pStackBase = cyg_mempool_var_try_alloc( _MsOS_MemoryPool_Info[0].stMemoryPool, u32StackSize);
        if (_MsOS_Task_Info[s32Id].pStackBase == NULL)
        {
            return -1;
        }
        cyg_thread_create( eTaskPriority,
                           (cyg_thread_entry_t  *)pTaskEntry, //TaskEntry has two arguments
                           u32TaskEntryData,
                           pTaskName,
                           _MsOS_Task_Info[s32Id].pStackBase,
                           u32StackSize,
                           &_MsOS_Task_Info[s32Id].stTask,
                           &_MsOS_Task_Info[s32Id].stThreadInfo );
        #else
        return -1;
        #endif
        */
        MS_ASSERT(0);
    }
    else
    {
        _MsOS_Task_Info[s32Id].pStackBase = NULL;
        _MsOS_Task_Info[s32Id].pTaskEntry = pTaskEntry;
        _MsOS_Task_Info[s32Id].entry_data = u32TaskEntryData;
        cyg_thread_create( eTaskPriority,
                           //(cyg_thread_entry_t  *)pTaskEntry, //TaskEntry has two arguments
                           (cyg_thread_entry_t  *)_MsOS_eCosTaskWrapper,
                           (cyg_addrword_t )&_MsOS_Task_Info[s32Id],
                           pTaskName,
                           pStackBase,
                           u32StackSize,
                           &_MsOS_Task_Info[s32Id].stTask,
                           &_MsOS_Task_Info[s32Id].stThreadInfo );

    }

    if (bAutoStart == TRUE)
    {
        cyg_thread_resume(_MsOS_Task_Info[s32Id].stTask);
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
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    if (FALSE == _MsOS_Task_Info[s32TaskId].bUsed)
    {
        return TRUE;
    }

    /*20090609 - somethims the delete task fail*/
    cyg_thread_kill(_MsOS_Task_Info[s32TaskId].stTask);
    MsOS_DelayTask(50);

    if( cyg_thread_delete(_MsOS_Task_Info[s32TaskId].stTask) == TRUE)
    {
        cyg_mutex_lock(&_MsOS_Task_Mutex);
        _MsOS_Task_Info[s32TaskId].bUsed = FALSE;
        _MsOS_Task_Info[s32TaskId].stTask = 0;
        cyg_mutex_unlock(&_MsOS_Task_Mutex);
        #if 1 // stack autogen
        if (_MsOS_Task_Info[s32TaskId].pStackBase)
        {
            // No stack base specified create default 4K stack
            cyg_mempool_var_free( _MsOS_MemoryPool_Info[0].stMemoryPool, _MsOS_Task_Info[s32TaskId].pStackBase);
            _MsOS_Task_Info[s32TaskId].pStackBase = NULL;
        }
        #endif
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
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    cyg_thread_yield();
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
    //u32Ms=0 => The task will enter sleep state and wake up immediately with relative trigger time = 0
    // => Other ready tasks with the same or lower priorities will have no chance to run
    MS_ASSERT( u32Ms != 0);
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    cyg_thread_delay(u32Ms*TICK_PER_ONE_MS);
}

//-------------------------------------------------------------------------------------------------
/// Delay for u32Us microseconds
/// @param  u32Us  \b IN: delay 0 ~ 999 us
/// @return None
/// @note   implemented by "busy waiting". Plz call MsOS_DelayTask directly for ms-order delay
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTaskUs (MS_U32 u32Us)
{
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    hal_delay_us(u32Us);
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

    cyg_thread_resume(_MsOS_Task_Info[s32TaskId].stTask);
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
    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TaskId &= MSOS_ID_MASK;
    }

    cyg_thread_suspend(_MsOS_Task_Info[s32TaskId].stTask);
    return TRUE;
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
    cyg_thread_info ThreadInfo;

    if ( (s32TaskId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
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

    if(!cyg_thread_get_info(_MsOS_Task_Info[s32TaskId].stTask,
                            cyg_thread_get_id(_MsOS_Task_Info[s32TaskId].stTask),
                            &ThreadInfo))
    {
        return FALSE;
    }
    *peTaskPriority =           (TaskPriority) ThreadInfo.cur_pri;
    *((MS_U32*)pStackBase) =    (MS_U32)ThreadInfo.stack_base;
    *((MS_U32*)pu32StackSize) = (MS_U32)ThreadInfo.stack_size;
    *((MS_U32*)pu32StackUsed) = (MS_U32)ThreadInfo.stack_used;
    strcpy(pTaskName, ThreadInfo.name);
    return TRUE;
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

    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if(_MsOS_Task_Info[s32Id].bUsed == TRUE)
        {
            ps32TaskIdList[u32TaskNum] = (s32Id | MSOS_ID_PREFIX);
            u32TaskNum++;
        }
    }
    return u32TaskNum;
}


//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void)
{
    MS_S32 s32Id;
    cyg_handle_t stTask;
    stTask = cyg_thread_self();

    for( s32Id=0; s32Id<MSOS_TASK_MAX; s32Id++)
    {
        if( stTask == _MsOS_Task_Info[s32Id].stTask )
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
/// Get thread ID of current thread/process in ecos
/// @return : current thread ID
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_GetOSThreadID (void)
{
    return (MS_S32)cyg_thread_self();
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
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag)
{
    MS_S32 s32Id;
    //OPTION suspend_type;

    cyg_mutex_lock(&_MsOS_Mutex_Mutex);
    for(s32Id=0;s32Id<MSOS_MUTEX_MAX;s32Id++)
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
    cyg_mutex_unlock(&_MsOS_Mutex_Mutex);

    if(s32Id >= MSOS_MUTEX_MAX)
    {
        return -1L;
    }

    //no Priority order for eCos
    //pMutexName is not used for eCos

    // Call OS kernel to create a semaphore.
    cyg_mutex_init( &_MsOS_Mutex_Info[s32Id].stMutex);

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

    cyg_mutex_destroy(&_MsOS_Mutex_Info[s32MutexId].stMutex);

    cyg_mutex_lock(&_MsOS_Mutex_Mutex);
    _MsOS_Mutex_Info[s32MutexId].bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_Mutex_Mutex);

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

    //@NOTE not call this assert because TSP driver must call obtain mutex in TSP ISR fun.
    //@TODO need to call system team for help this
    //MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        cyg_mutex_lock(&_MsOS_Mutex_Info[s32MutexId].stMutex);
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if ( cyg_mutex_trylock(&_MsOS_Mutex_Info[s32MutexId].stMutex) )
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
        if ( cyg_mutex_timed_lock(&_MsOS_Mutex_Info[s32MutexId].stMutex,
                                    cyg_current_time()+u32WaitMs*TICK_PER_ONE_MS) )
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

    cyg_mutex_unlock(&_MsOS_Mutex_Info[s32MutexId].stMutex);

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

    cyg_mutex_lock(&_MsOS_Semaphore_Mutex);
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
    cyg_mutex_unlock(&_MsOS_Semaphore_Mutex);

    if(s32Id >= MSOS_SEMAPHORE_MAX)
    {
        return -1;
    }

    //no Priority order for eCos
    //pSemaphoreName is not used for eCos

    // Call OS kernel to create a semaphore.
    cyg_semaphore_init( &_MsOS_Semaphore_Info[s32Id].stSemaphore, u32InitCnt);

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

    cyg_semaphore_destroy(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);

    cyg_mutex_lock(&_MsOS_Semaphore_Mutex);
    _MsOS_Semaphore_Info[s32SemaphoreId].bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_Semaphore_Mutex);
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

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        cyg_semaphore_wait(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if ( cyg_semaphore_trywait(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore) )
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
        if ( cyg_semaphore_timed_wait(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore,
                                        cyg_current_time()+u32WaitMs*TICK_PER_ONE_MS) )
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
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32SemaphoreId &= MSOS_ID_MASK;
    }

    cyg_semaphore_post(&_MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore);

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
    if ( (s32SemaphoreId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
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
        *pu32InitCnt = _MsOS_Semaphore_Info[s32SemaphoreId].stSemaphore.count;
        *peAttribute = E_MSOS_FIFO;
        UTL_strcpy(pSemaphoreName, "XXX");

        return TRUE;
    }
    else
    {
        return FALSE;
    }
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
    cyg_mutex_lock(&_MsOS_EventGroup_Mutex);

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
    cyg_mutex_unlock(&_MsOS_EventGroup_Mutex);

    if(s32Id >= MSOS_EVENTGROUP_MAX)
    {
        return -1;
    }

    //pEventName is not used for eCos

    cyg_flag_init(&_MsOS_EventGroup_Info[s32Id].stEventGroup);

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

    if ( cyg_flag_waiting(&_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup) )
    {
        return FALSE;
    }
    else
    {
        cyg_flag_destroy(&_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup);
        cyg_mutex_lock(&_MsOS_EventGroup_Mutex);
        _MsOS_EventGroup_Info[s32EventGroupId].bUsed = FALSE;
        cyg_mutex_unlock(&_MsOS_EventGroup_Mutex);
        return TRUE;
    }
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

    cyg_flag_setbits(&_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, u32EventFlag);
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

    cyg_flag_maskbits(&_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, ~u32EventFlag);
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
    cyg_flag_mode_t operation;
    MS_BOOL bClear;

    if ( (s32EventGroupId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
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
        operation = CYG_FLAG_WAITMODE_AND;
        break;
    case E_OR:
        operation = CYG_FLAG_WAITMODE_OR;
        break;
    case E_AND_CLEAR:
        operation = CYG_FLAG_WAITMODE_AND;
        bClear = TRUE;
        break;
    case E_OR_CLEAR:
        operation = CYG_FLAG_WAITMODE_OR;
        bClear = TRUE;
        break;
    default:
        return FALSE;
    }


    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        *pu32RetrievedEventFlag = cyg_flag_wait( &_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                                                 u32WaitEventFlag,
                                                 operation );
    }
    else if (u32WaitMs==0) //non-blocking
    {
        *pu32RetrievedEventFlag = cyg_flag_poll( &_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                                                 u32WaitEventFlag,
                                                 operation );
    }
    else //blocking wait with timeout
    {
        *pu32RetrievedEventFlag = cyg_flag_timed_wait( &_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup,
                                                       u32WaitEventFlag,
                                                       operation,
                                                       cyg_current_time()+u32WaitMs*TICK_PER_ONE_MS );
    }

    if (bClear && *pu32RetrievedEventFlag)
    {
        cyg_flag_maskbits(&_MsOS_EventGroup_Info[s32EventGroupId].stEventGroup, ~((*pu32RetrievedEventFlag)&(u32WaitEventFlag)));
    }

    if( *pu32RetrievedEventFlag )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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


/*
//
// Signal management
//
//-------------------------------------------------------------------------------------------------
// Create a signal and attach the callback function
// @param  pSignalCb   \b IN: signal callback function
// @return TRUE : succeed
// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CreateSignal (SignalCb pSignalCb)
{
    //ToDo: POSIX signal or ...
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Enable / Disable the corresponding signal
// @param  u32SignalMask   \b IN: bit pattern signal mask (Set a bit to enable; Clear a bit to disable)
// @return TRUE : succeed
// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_ControlSignals (MS_U32 u32SignalMask)
{
    //ToDo: POSIX signal or ...
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Send the signal to the specified task
// @param  s32TaskId \b IN: Task ID
// @param  u32Signal \b IN: signal bit pattern
// @return TRUE : succeed
// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SendSignals (MS_S32 s32TaskId, MS_U32 u32Signal)
{
    //ToDo: POSIX signal or ...
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Return the current signal value associated with the calling task
// @return current signal value
// @note   All signals are automatically cleared as a result of this service call
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_ReceiveSignals (void)
{
    //ToDo: POSIX signal or ...
    return FALSE;
}
*/

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

    cyg_mutex_lock( &_MsOS_Timer_Mutex);
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
    cyg_mutex_unlock( &_MsOS_Timer_Mutex);

    if(s32Id >= MSOS_TIMER_MAX)
    {
        return -1;
    }

    // Call OS kernel to create a Timer
    cyg_alarm_create( _stCounter, //_MsOS_Timer_Info[s32Id].stCounter,
                      (cyg_alarm_t*)pTimerCb,
                      s32Id | MSOS_ID_PREFIX,
                      &_MsOS_Timer_Info[s32Id].stTimer,
                      &_MsOS_Timer_Info[s32Id].stAlarm );

    //pTimerName is not used for eCos

    cyg_alarm_initialize( _MsOS_Timer_Info[s32Id].stTimer, cyg_current_time() + u32FirstTimeMs*TICK_PER_ONE_MS,
                            u32PeriodTimeMs*TICK_PER_ONE_MS); //will enable timer

    if(!bStartTimer)
    {
        cyg_alarm_disable(_MsOS_Timer_Info[s32Id].stTimer);
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
        cyg_alarm_delete(_MsOS_Timer_Info[s32TimerId].stTimer);
        cyg_mutex_lock( &_MsOS_Timer_Mutex);
        _MsOS_Timer_Info[s32TimerId].bUsed = FALSE;
        cyg_mutex_unlock(&_MsOS_Timer_Mutex);
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
        cyg_alarm_enable(_MsOS_Timer_Info[s32TimerId].stTimer);
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
        cyg_alarm_disable(_MsOS_Timer_Info[s32TimerId].stTimer);
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
MS_BOOL MsOS_ResetTimer (MS_S32     s32TimerId,
                         MS_U32     u32FirstTimeMs,
                         MS_U32     u32PeriodTimeMs,
                         MS_BOOL    bStartTimer)
{
    if ( (s32TimerId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32TimerId &= MSOS_ID_MASK;
    }

    cyg_alarm_initialize( _MsOS_Timer_Info[s32TimerId].stTimer, cyg_current_time() + u32FirstTimeMs*TICK_PER_ONE_MS,
                            u32PeriodTimeMs*TICK_PER_ONE_MS); //will enable timer

    if(!bStartTimer)
    {
        cyg_alarm_disable(_MsOS_Timer_Info[s32TimerId].stTimer);
    }

    return TRUE;
}


//
// System time
//
/*
//use MsOS_GetSystemTime instead
//-------------------------------------------------------------------------------------------------
/// Get current system time in timer ticks
/// @return system time in timer ticks
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTick (void)
{
#if 1
    //kernel counter value will not be updated if called by eCos DSR
    // timer interrupts -> Cyg_RealTimeClock::dsr()->Cyg_Counter::tick()->counter += increment;
    //  will not be called if another DSR is currently being serived
    return cyg_current_time();

    //This will overtime after COUNT register reaches max value.
    MS_U32 u32COUNT;
#else
    //FixMe: this is a little different from cyg_current_time, coz now kerel ignores the period from
    //       timer interrupt trigger to COUNT/COMPARE update
    asm volatile (
        "mfc0 $8,$9;"
        "nop; nop; nop;"
        "move %0,$8;"
        : "=r"(u32COUNT)
        :
        : "$8"
        );

    return ( u32COUNT/CYGNUM_HAL_RTC_PERIOD );
#endif
}
*/

//-------------------------------------------------------------------------------------------------
/// Get current system time in ms
/// @return system time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_GetSystemTime (void)
{
    return ( cyg_current_time()/TICK_PER_ONE_MS );
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
    // Note: eCos Kernel does not provide message queue API; use POSIX API instead ?

    MS_S32 s32Id;

    cyg_mutex_lock( &_MsOS_Queue_Mutex);
    for( s32Id=0; s32Id<MSOS_QUEUE_MAX; s32Id++)
    {
        if(_MsOS_Queue_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }
    if(s32Id < MSOS_QUEUE_MAX)
    {
        _MsOS_Queue_Info[s32Id].bUsed = TRUE;
        _MsOS_Queue_Info[s32Id].u32AlignedMsgSize = ALIGN_4(u32MessageSize);

        // Allocate memory for queue.
        _MsOS_Queue_Info[s32Id].pu8MsgQueue =
            MsOS_AllocateMemory((ALIGN_4(u32MessageSize)+sizeof(MS_U32))*
                                (CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE+1),
                                gs32SystemPoolID);
        if(!_MsOS_Queue_Info[s32Id].pu8MsgQueue)
        {
            _MsOS_Queue_Info[s32Id].bUsed = FALSE;
            return -1;
        }
        _MsOS_Queue_Info[s32Id].u32MsgWriteIdx = 0;
        _MsOS_Queue_Info[s32Id].eMsgType = eMessageType;
    }
    cyg_mutex_unlock( &_MsOS_Queue_Mutex);

    if(s32Id >= MSOS_QUEUE_MAX)
    {
        return -1;
    }
    cyg_mutex_init(&_MsOS_Queue_Info[s32Id].stMutex);

    /* no E_MSG_VAR_SIZE for POSIX message queue
    switch(u32MessageType)
    {
    case E_MSG_FIXED_SIZE:
        msg_type = NU_FIXED_SIZE;
        break;
    case E_MSG_VAR_SIZE:
        msg_type = NU_VARIABLE_SIZE;
        break;
    default:
        return -1;
        break;
    }
    */
    /* ONLY Priority order for POSIX message queue
    switch(eAttribute)
    {
    case E_MSOS_PRIORITY:
        suspend_type = NU_PRIORITY;
        break;
    case E_MSOS_FIFO:
        suspend_type = NU_FIFO;
        break;
    default:
        return -1;
        break;
    }
    */

    //no Priority order for eCos
    //pQueueName is not used for eCos

    // u32QueueSize: fixed as CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE
    cyg_mbox_create( &_MsOS_Queue_Info[s32Id].stQueue, &_MsOS_Queue_Info[s32Id].stQueueInfo);
    /*
    if ( _MsOS_Queue_Info[s32Id].stQueue < 0 )
    {
        cyg_mutex_lock( &_MsOS_Queue_Mutex);
        _MsOS_Queue_Info[s32Id].bUsed = FALSE;
        cyg_mutex_unlock( &_MsOS_Queue_Mutex);
        s32Id = -1;
    }
    */

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

    cyg_mbox_delete( _MsOS_Queue_Info[s32QueueId].stQueue);
    if(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue)
    {
        MsOS_FreeMemory(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue, gs32SystemPoolID);
    }
    cyg_mutex_destroy(&_MsOS_Queue_Info[s32QueueId].stMutex);

    cyg_mutex_lock( &_MsOS_Queue_Mutex);
    _MsOS_Queue_Info[s32QueueId].pu8MsgQueue = NULL;
    _MsOS_Queue_Info[s32QueueId].bUsed = FALSE;
    cyg_mutex_unlock( &_MsOS_Queue_Mutex);
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

    if(u32Size > _MsOS_Queue_Info[s32QueueId].u32AlignedMsgSize)
    {
        return FALSE;
    }

    if ( u32WaitMs != 0)
    {
        MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    }

    if(u32WaitMs == 0)
    {
        if(cyg_mutex_trylock(&_MsOS_Queue_Info[s32QueueId].stMutex) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        cyg_mutex_lock(&_MsOS_Queue_Info[s32QueueId].stMutex);
    }

    // Copy message.
    if(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue)
    {
        pu32MsgPos = (MS_U32*)(_MsOS_Queue_Info[s32QueueId].pu8MsgQueue +
                            _MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx *
                            (_MsOS_Queue_Info[s32QueueId].u32AlignedMsgSize + sizeof(MS_U32)));
        pu32MsgPos[0] = u32Size;
        memcpy(pu32MsgPos+1, pu8Message, u32Size);
    }
    else
    {
        cyg_mutex_unlock(&_MsOS_Queue_Info[s32QueueId].stMutex);
        return FALSE;
    }

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        cyg_mbox_put( _MsOS_Queue_Info[s32QueueId].stQueue, pu32MsgPos);
        bRet = TRUE;
    }
    else if (u32WaitMs==0) //non-blocking
    {
        if ( cyg_mbox_tryput( _MsOS_Queue_Info[s32QueueId].stQueue, pu32MsgPos) )
        {
            bRet = TRUE;
        }
    }
    else //blocking wait with timeout
    {
        if ( cyg_mbox_timed_put( _MsOS_Queue_Info[s32QueueId].stQueue, pu32MsgPos,
                                 cyg_current_time()+u32WaitMs*TICK_PER_ONE_MS) )
        {
            bRet = TRUE;
        }
    }

    if(bRet)
    {
        _MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx =
            (_MsOS_Queue_Info[s32QueueId].u32MsgWriteIdx + 1) %
            (CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE+1);
    }

    cyg_mutex_unlock(&_MsOS_Queue_Info[s32QueueId].stMutex);
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

    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    if (u32WaitMs==MSOS_WAIT_FOREVER) //blocking wait
    {
        pItem = cyg_mbox_get( _MsOS_Queue_Info[s32QueueId].stQueue);
    }
    else if (u32WaitMs==0) //non-blocking
    {
        pItem = cyg_mbox_tryget( _MsOS_Queue_Info[s32QueueId].stQueue );
    }
    else //blocking wait with timeout
    {
        pItem = cyg_mbox_timed_get( _MsOS_Queue_Info[s32QueueId].stQueue,
                                    cyg_current_time()+u32WaitMs*TICK_PER_ONE_MS);
    }

    if ( pItem == NULL )
    {
        *pu32ActualSize = 0;
        bRet = FALSE;
    }
    else
    {
        if(u32IntendedSize < *(MS_U32*)pItem)
        {
            *pu32ActualSize = u32IntendedSize;
        }
        else
        {
            *pu32ActualSize = *(MS_U32*)pItem;
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

    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
    {
        return FALSE;
    }
    else
    {
        s32QueueId &= MSOS_ID_MASK;
    }

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );

    pItem= cyg_mbox_peek_item( _MsOS_Queue_Info[s32QueueId].stQueue );

    if ( pItem == NULL )
    {
        *pu32ActualSize = 0;
        bRet = FALSE;
    }
    else
    {
        if(u32IntendedSize < *(MS_U32*)pItem)
        {
            *pu32ActualSize = u32IntendedSize;
        }
        else
        {
            *pu32ActualSize = *(MS_U32*)pItem;
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
    if ( (s32QueueId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX )
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

//
// Atomic operation
//
void MsOS_AtomicSet(MsOS_Atomic *pAtomic, MS_S32 s32Value)
{
    pAtomic->s32Value = s32Value;
}

MS_S32 MsOS_AtomicRead(const MsOS_Atomic *pAtomic)
{
    return pAtomic->s32Value;
}

void MsOS_AtomicAdd(MsOS_Atomic *pAtomic, MS_S32 s32Value)
{
    cyg_interrupt_disable();
    pAtomic->s32Value += s32Value;
    cyg_interrupt_enable();
}

void MsOS_AtomicSub(MsOS_Atomic *pAtomic, MS_S32 s32Value)
{
    cyg_interrupt_disable();
    pAtomic->s32Value -= s32Value;
    cyg_interrupt_enable();
}

MS_S32 MsOS_AtomicAddReturn(MsOS_Atomic *pAtomic, MS_S32 s32Value)
{
    MS_S32 s32Ret;

    cyg_interrupt_disable();
    pAtomic->s32Value += s32Value;
    s32Ret = pAtomic->s32Value;
    cyg_interrupt_enable();

    return s32Ret;
}

MS_S32 MsOS_AtomicSubReturn(MsOS_Atomic *pAtomic, MS_S32 s32Value)
{
    MS_S32 s32Ret;

    cyg_interrupt_disable();
    pAtomic->s32Value -= s32Value;
    s32Ret = pAtomic->s32Value;
    cyg_interrupt_enable();

    return s32Ret;
}


//
// Per-thread data
//
//-------------------------------------------------------------------------------------------------
/// Per-thread data index creation
/// @param  pu32Index  \b OUT: the new per-thread data index
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CreateThreadDataIndex(MS_U32 *pu32Index)
{
    if(pu32Index != NULL)
    {
        *pu32Index = cyg_thread_new_data_index();
        return TRUE;
    }

    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Per-thread data index deletion
/// @param  u32Index  \b IN: the per-thread data index
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteThreadDataIndex(MS_U32 u32Index)
{
    cyg_thread_free_data_index(u32Index);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set per-thread data by index
/// @param  u32Index  \b IN: the per-thread data index
/// @param  u32Data  \b IN: the per-thread data
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SetThreadData(MS_U32 u32Index, MS_U32 u32Data)
{
    cyg_thread_set_data(u32Index, u32Data);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get per-thread data by index
/// @param  u32Index  \b IN: the per-thread data index
/// @param  pu32Data  \b OUT: the per-thread data
/// @return TRUE : succeed
/// @return FALSE :  fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_GetThreadData(MS_U32 u32Index, MS_U32 *pu32Data)
{
    if(pu32Data != NULL)
    {
        *pu32Data = cyg_thread_get_data(u32Index);
        return TRUE;
    }

    return FALSE;
}

//
// Condition Variables
//
//-------------------------------------------------------------------------------------------------
/// Create a condition variable
/// @param  s32MutexId  \b IN: associated mutex
/// @param  pCondName  \b IN: condition variable name
/// @return >=0 : assigned condition variable Id
/// @return <0 : fail
/// @note
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateCondition(MS_S32 s32MutexId, const char *pCondName)
{
    MS_S32 s32Id;
    cyg_cond_t stCond;

    if((s32MutexId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return -1;
    }
    else
    {
        s32MutexId &= MSOS_ID_MASK;
    }

    cyg_mutex_lock(&_MsOS_Condition_Mutex);
    for(s32Id = 0; s32Id < MSOS_CONDITION_MAX; s32Id++)
    {
        if(_MsOS_Condition_Info[s32Id].bUsed == FALSE)
        {
            break;
        }
    }

    if(s32Id < MSOS_CONDITION_MAX)
    {
        _MsOS_Condition_Info[s32Id].bUsed = TRUE;
    }
    cyg_mutex_unlock(&_MsOS_Condition_Mutex);

    if(s32Id >= MSOS_CONDITION_MAX)
    {
        return -1;
    }

    //pCondName is not used for eCos

    cyg_cond_init(&stCond, &_MsOS_Mutex_Info[s32MutexId].stMutex);

    cyg_mutex_lock(&_MsOS_Condition_Mutex);
    _MsOS_Condition_Info[s32Id].stCond = stCond;
    cyg_mutex_unlock(&_MsOS_Condition_Mutex);

    s32Id |= MSOS_ID_PREFIX;

    return s32Id;
}

//-------------------------------------------------------------------------------------------------
/// Delete the specified condition variable
/// @param  s32CondId  \b IN: condition variable ID
/// @return TRUE : succeed
/// @return FALSE : fail
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteCondition(MS_S32 s32CondId)
{
    if((s32CondId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32CondId &= MSOS_ID_MASK;
    }

    cyg_cond_destroy(&_MsOS_Condition_Info[s32CondId].stCond);

    cyg_mutex_lock(&_MsOS_Condition_Mutex);
    _MsOS_Condition_Info[s32CondId].bUsed = FALSE;
    cyg_mutex_unlock(&_MsOS_Condition_Mutex);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Wait on a condition
/// @param  s32CondId  \b IN: condition variable ID
/// @param  u32WaitMs   \b IN: 1 ~ MSOS_WAIT_FOREVER: condition variable timeout
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_WaitCondition(MS_S32 s32CondId, MS_U32 u32WaitMs)
{
    if(u32WaitMs == 0)
    {
        return FALSE;
    }

    if((s32CondId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32CondId &= MSOS_ID_MASK;
    }

    MS_ASSERT(MsOS_In_Interrupt() == FALSE);

    if (u32WaitMs == MSOS_WAIT_FOREVER) //blocking wait
    {
        cyg_cond_wait(&_MsOS_Condition_Info[s32CondId].stCond);
        return TRUE;
    }

    cyg_cond_timed_wait(&_MsOS_Condition_Info[s32CondId].stCond,
            cyg_current_time() + u32WaitMs * TICK_PER_ONE_MS);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Signal a condition
/// @param  s32CondId  \b IN: condition variable ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_SignalCondition(MS_S32 s32CondId)
{
    if((s32CondId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32CondId &= MSOS_ID_MASK;
    }

    cyg_cond_signal(&_MsOS_Condition_Info[s32CondId].stCond);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Broadcast a condition
/// @param  s32CondId  \b IN: condition variable ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_BroadcastCondition(MS_S32 s32CondId)
{
    if((s32CondId & MSOS_ID_PREFIX_MASK) != MSOS_ID_PREFIX)
    {
        return FALSE;
    }
    else
    {
        s32CondId &= MSOS_ID_MASK;
    }

    cyg_cond_broadcast(&_MsOS_Condition_Info[s32CondId].stCond);

    return TRUE;
}

//
// Spinlock
// The spinlock of the eCos doesn't work in the preemptive multi-level queue scheduler.
// Use scheduler control and IRQ dis/enable instead
//
MS_S32 MsOS_CreateSpin(MsOSAttribute eAttribute, char *pSpinName)
{
    return 0;
}

MS_BOOL MsOS_DeleteSpin(MS_S32 s32SpinId)
{
    return TRUE;
}

MS_BOOL MsOS_ObtainSpin(MS_S32 s32SpinId)
{
    cyg_scheduler_lock();
    return TRUE;
}

MS_BOOL MsOS_ReleaseSpin(MS_S32 s32SpinId)
{
    cyg_scheduler_unlock();
    return TRUE;
}

MS_BOOL MsOS_ObtainSpinIrqSave(MS_S32 s32SpinId, MS_U32 *pu32Flag)
{
    CYG_INTERRUPT_STATE s;

    MS_ASSERT(pu32Flag != NULL);

    HAL_DISABLE_INTERRUPTS(s);
    *pu32Flag = s;

    return TRUE;
}

MS_BOOL MsOS_ReleaseSpinIrqSave(MS_S32 s32SpinId, MS_U32 u32Flag)
{
    CYG_INTERRUPT_STATE s = u32Flag;
    HAL_RESTORE_INTERRUPTS(s);

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
    cyg_mutex_lock(&_MsOS_HISR_Mutex);

    CHIP_AttachISR(eIntNum, pIntCb);

    cyg_mutex_unlock(&_MsOS_HISR_Mutex);

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
    cyg_mutex_lock(&_MsOS_HISR_Mutex);

    CHIP_DetachISR(eIntNum);

    cyg_mutex_unlock(&_MsOS_HISR_Mutex);

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
    MS_U32                 u32OldInterrupts;
    HAL_DISABLE_INTERRUPTS(u32OldInterrupts);
    return u32OldInterrupts;
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
    HAL_RESTORE_INTERRUPTS(u32OldInterrupts);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
    HAL_ENABLE_INTERRUPTS();
    return TRUE;
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
/// Notify the interrupt complete #
/// @param  eIntNum \b IN: Interrupt number in enumerator InterruptNum
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CompleteInterrupt (InterruptNum eIntNum)
{
    return TRUE;
    //return CHIP_CompleteIRQ(eIntNum);
}

#include <cyg/hal/hal_cache.h>
//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------

MS_BOOL MsOS_Dcache_Flush( MS_U32 u32Start, MS_U32 u32Size )
{
    return MsOS_MPool_Dcache_Flush(u32Start,u32Size);
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
    MS_U32 u32OldIntr;
    if ( (u32Start % DCACHE_LINE_SIZE) || ( (u32Start & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE) )
    {
        return FALSE;
    }
    u32OldIntr = MsOS_DisableAllInterrupts();
    HAL_DCACHE_INVALIDATE( u32Start, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
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

#define SHM_SIZE                (200*1024)
#define MAX_CLIENT_NUM          320
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
        printf("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    _shm_id = (int)_pu8ShareMem;

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
MS_BOOL MsOS_SHM_GetId(MS_U8* pu8ClientName, MS_U32 u32BufSize, MS_U32* pu32ShmId, MS_U32* pu32Addr, MS_U32* pu32BufSize, MS_U32 u32Flag)
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
            pClient->u8ClientId = i+ 1;
            pClient->u8RefCnt = 0;
            pClient->u32Offset = _ShmHdr.u32Offset;
            _ShmHdr.u32Offset += (u32BufSize+7)&~7;
            memcpy(_pu8ShareMem, &_ShmHdr, sizeof(_ShmHdr));
            break;
        }
    }
    if (NULL == pClient)
    {
        PRINT_INFO("[%s][%d] MsOS_SHM_Init: Unable to get available share memeory\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    *pu32ShmId = pClient->u8ClientId;
    *pu32BufSize = pClient->u32Size;
    *pu32Addr = (MS_U32)_pu8ShareMem + pClient->u32Offset;
    return TRUE;
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
MS_BOOL MsOS_CPU_DetachExceptiont (MHAL_EXCEPTION_TYPE expt_num)
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

