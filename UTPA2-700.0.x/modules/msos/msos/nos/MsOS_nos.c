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


// #if defined (MSOS_TYPE_NOS)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "drvIRQ.h"
#include "string.h"
#include "halCHIP.h"
#include "halMMIO.h"
#include "MsVersion.h"
#include "halMPool.h"

#ifdef CONFIG_UTOPIA_FRAMEWORK
#include "utopia.h"
#endif

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

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


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

//
// Semaphore
//

//
// Event Group
//

//
// Queue
//

//
// Timer
//

//
// Interrupt
//
#if 0 //move to mscommon.h
//cached/unchched segment
#define CACHED_BASE         ((void *)0x00000000)
#define UNCACHED_BASE       ((void *)0x80000000)

#define CACHED_SIZE         ((void *)0x20000000)
#define UNCACHED_SIZE       ((void *)0x20000000)
//  0xA0000000~0xA000FFFF belongs to RIU
//  0xA1000000~           belongs to SPI

//cached/unchched segment
#define KSEG0_BASE          CACHED_BASE
#define KSEG1_BASE          UNCACHED_BASE
#define KSEG0_SIZE          CACHED_SIZE
#define KSEG1_SIZE          UNCACHED_SIZE

//cached <-> uncached
#define KSEG02KSEG1(addr)   ((void *)((U32)(addr)|0x80000000))
#define KSEG12KSEG0(addr)   ((void *)((U32)(addr)&~0x80000000))

//virtual <-> physical
#define VA2PA(addr)         ((void *)(((U32)addr) & 0x1fffffff))
#define PA2KSEG0(addr)      ((void *)(((U32)addr) | 0x00000000))
#define PA2KSEG1(addr)      ((void *)(((U32)addr) | 0x80000000))
#endif
//MS_U32<->MS_U16
#define LOU16(u32Val)               ( (MS_U16)(u32Val) )
#define HIU16(u32Val)               ( (MS_U16)((u32Val) >> 16) )
#define CATU32(u16HiVal, u16LoVal)  ( (MS_U32)(u16HiVal)<<16 | (MS_U32)(u16LoVal) )

//MS_U16<->MS_U8
#define LOU8(u16Val)            ( (MS_U8)(u16Val) )
#define HIU8(u16Val)            ( (MS_U8)((u16Val) >> 8) )

extern void mhal_dcache_flush(MS_U32 u32Base, MS_U32 u32Size);

#if defined(CHIP_A2) || \
    defined(CHIP_T12) || \
    defined(CHIP_T8) || \
    defined(CHIP_J2) || \
    defined(CHIP_T11) || \
    defined(CHIP_A5) || \
    defined(CHIP_A5P) || \
    defined(CHIP_A3) || \
    defined(CHIP_K2) || \
    defined(CHIP_EDISON)
extern void mhal_L2Cache_Flush(void);
extern void mhal_L2Cache_Read(void);
#endif

extern int  mhal_interrupt_disable(void);
extern void mhal_interrupt_enable(void);
extern void mhal_interrupt_restore(int old);
extern void mhal_interrupt_mask_all(void);
extern void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num);
extern void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);
extern void mhal_interrupt_lock(void);
extern void mhal_interrupt_unlock(void);
extern void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, MS_U32 dat);
extern void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num);
extern void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, MS_U32 data);
extern void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num);
extern void mhal_init_mmu(void);

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
#if (!defined(CHIP_T2) && !defined(CHIP_JANUS) && !defined(CHIP_U3) && !defined(CHIP_MARIA10) && !defined(CHIP_K1) && !defined(CHIP_KELTIC) && !defined(CHIP_KAPPA) && !defined(CHIP_KENYA) && !defined(CHIP_MACAW12) && !defined(CHIP_EDEN) && !defined(CHIP_EULER))

#if(defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K))

#if defined(CHIP_T9) || \
    defined(CHIP_T13) || \
    defined(CHIP_A1) || \
    defined(CHIP_A6) || \
    defined(CHIP_A7) || \
    defined(CHIP_EMERALD) || \
    defined(CHIP_EAGLE) || \
    defined(CHIP_EIFFEL) || \
    defined(CHIP_NUGGET) || \
	defined(CHIP_NIKON) || \
	defined(CHIP_MILAN)

    unsigned long entrylo0_miu1 = ((0x60000000 >> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
    unsigned long entrylo1_miu1 = ((0x60000000 >> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1, entrylo1_miu1, MIU1_CACHEABLE_START , 0x1fffe000);

#elif defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS)
    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

#elif defined(CHIP_A2) || \
      defined(CHIP_T12) || \
      defined(CHIP_T8) || \
      defined(CHIP_J2) || \
      defined(CHIP_T11) || \
      defined(CHIP_A5) || \
      defined(CHIP_A5P) || \
      defined(CHIP_A3) || \
      defined(CHIP_K2) || \
      defined(CHIP_AMETHYST) || \
      defined(CHIP_AGATE) || \
      defined(CHIP_EDISON) || \
      defined(CHIP_EINSTEIN) || \
      defined(CHIP_NAPOLI) || \
      defined(CHIP_EINSTEIN3) || \
      defined(CHIP_MONACO) || \
      defined(CHIP_CLIPPERS) || \
      defined(CHIP_MUNICH)

    //MIU0
    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU1
    unsigned long entrylo0_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    unsigned long entrylo0_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1_lo256mb, entrylo1_miu1_lo256mb, 0x00000000, 0x1fffe000);

    //MIU1 (256MB - 512MB)
    add_wired_entry(entrylo0_miu1_hi256mb, entrylo1_miu1_hi256mb, 0x20000000, 0x1fffe000);
#else
    unsigned long entrylo0 = ((0x40000000>> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // cacheable
    unsigned long entrylo1 = ((0x40000000>> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // un-cacheable or 0x7
    add_wired_entry(entrylo0, entrylo1, MIU1_CACHEABLE_START , 0x1fffe000);
#endif
#endif

#if (defined(__arm__) || defined (__aarch64__))
    mhal_init_mmu();
#endif

#endif

#if (defined(__arm__) || defined (__aarch64__))
    extern void ProcessTimer0Isr(void);
#else
    extern void ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
#endif

    MsOS_CPU_AttachInterrupt( E_INTERRUPT_TICK_TIMER, ProcessTimer0Isr, 0 );
    MAsm_CPU_TimerInit();
    MDrv_IRQ_Init();

#ifdef CONFIG_UTOPIA_FRAMEWORK
#if defined(CONFIG_MBOOT) || defined(CONFIG_CHAKRA2) || defined(CONFIG_PUMABOOT) 
    UtopiaInit();
#endif
#endif

    return true;
}
#ifdef CONFIG_MBOOT
MS_BOOL MsOS_Init_str (void)
{
#if (!defined(CHIP_T2) && !defined(CHIP_JANUS) && !defined(CHIP_U3) && !defined(CHIP_MARIA10) && !defined(CHIP_K1) && !defined(CHIP_KELTIC) && !defined(CHIP_KAPPA) && !defined(CHIP_KENYA) && !defined(CHIP_MACAW12) && !defined(CHIP_EDEN) && !defined(CHIP_EULER))

#if(defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K))

#if defined(CHIP_T9) || \
    defined(CHIP_T13) || \
    defined(CHIP_A1) || \
    defined(CHIP_A6) || \
    defined(CHIP_A7) || \
    defined(CHIP_EMERALD) || \
    defined(CHIP_EAGLE) || \
    defined(CHIP_EIFFEL) || \
    defined(CHIP_NUGGET) || \
	defined(CHIP_NIKON) || \
	defined(CHIP_MILAN)

    unsigned long entrylo0_miu1 = ((0x60000000 >> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
    unsigned long entrylo1_miu1 = ((0x60000000 >> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1, entrylo1_miu1, MIU1_CACHEABLE_START , 0x1fffe000);

#elif defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS)
    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

#elif defined(CHIP_A2) || \
      defined(CHIP_T12) || \
      defined(CHIP_T8) || \
      defined(CHIP_J2) || \
      defined(CHIP_T11) || \
      defined(CHIP_A5) || \
      defined(CHIP_A5P) || \
      defined(CHIP_A3) || \
      defined(CHIP_K2) || \
      defined(CHIP_AMETHYST) || \
      defined(CHIP_AGATE) || \
      defined(CHIP_EDISON) || \
      defined(CHIP_EINSTEIN) || \
      defined(CHIP_NAPOLI) || \
      defined(CHIP_EINSTEIN3) || \
      defined(CHIP_MONACO) || \
      defined(CHIP_CLIPPERS) || \
      defined(CHIP_MUNICH)
    //MIU0
    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU1
    unsigned long entrylo0_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    unsigned long entrylo0_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1_lo256mb, entrylo1_miu1_lo256mb, 0x00000000, 0x1fffe000);

    //MIU1 (256MB - 512MB)
    add_wired_entry(entrylo0_miu1_hi256mb, entrylo1_miu1_hi256mb, 0x20000000, 0x1fffe000);
#else
    unsigned long entrylo0 = ((0x40000000>> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // cacheable
    unsigned long entrylo1 = ((0x40000000>> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // un-cacheable or 0x7
    add_wired_entry(entrylo0, entrylo1, MIU1_CACHEABLE_START , 0x1fffe000);
#endif
#endif

#if (defined(__arm__) || defined (__aarch64__))
    mhal_init_mmu();
#endif

#endif

#if (defined(__arm__) || defined (__aarch64__))
    extern void ProcessTimer0Isr(void);
#else
    extern void ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
#endif

    MsOS_CPU_AttachInterrupt( E_INTERRUPT_TICK_TIMER, ProcessTimer0Isr, 0 );
    MAsm_CPU_TimerInit();
    MDrv_IRQ_Init();

#ifdef CONFIG_UTOPIA_FRAMEWORK
#ifdef CONFIG_MBOOT
    UtopiaInit_str();
#endif
#endif

    return true;
}
#endif

#ifdef CONFIG_MBOOT
MS_BOOL MsOS_Init_Fastboot (void)
{
#if (!defined(CHIP_T2) && !defined(CHIP_JANUS) && !defined(CHIP_U3) && !defined(CHIP_MARIA10) && !defined(CHIP_K1) && !defined(CHIP_KELTIC) && !defined(CHIP_KAPPA) && !defined(CHIP_KENYA) && !defined(CHIP_MACAW12) && !defined(CHIP_EDEN) && !defined(CHIP_EULER))

#if(defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K))

#if defined(CHIP_T9) || \
    defined(CHIP_T13) || \
    defined(CHIP_A1) || \
    defined(CHIP_A6) || \
    defined(CHIP_A7) || \
    defined(CHIP_EMERALD) || \
    defined(CHIP_EAGLE) || \
    defined(CHIP_EIFFEL) || \
    defined(CHIP_NUGGET) || \
	defined(CHIP_NIKON) || \
	defined(CHIP_MILAN)

    unsigned long entrylo0_miu1 = ((0x60000000 >> 12) << 6) | (0<<3) | (1<< 2) | (1<< 1) | (1<< 0); // cacheable
    unsigned long entrylo1_miu1 = ((0x60000000 >> 12) << 6) | (2<<3) | (1<< 2) | (1<< 1) | (1<< 0); // un-cacheable or 0x7

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1, entrylo1_miu1, MIU1_CACHEABLE_START , 0x1fffe000);

#elif defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS)

    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

#elif defined(CHIP_A2) || \
      defined(CHIP_T12) || \
      defined(CHIP_T8) || \
      defined(CHIP_J2) || \
      defined(CHIP_T11) || \
      defined(CHIP_A5) || \
      defined(CHIP_A5P) || \
      defined(CHIP_A3) || \
      defined(CHIP_K2) || \
      defined(CHIP_AMETHYST) || \
      defined(CHIP_AGATE) || \
      defined(CHIP_EDISON) || \
      defined(CHIP_EINSTEIN) || \
      defined(CHIP_NAPOLI) || \
      defined(CHIP_EINSTEIN3) || \
      defined(CHIP_MONACO) || \
      defined(CHIP_CLIPPERS) || \
      defined(CHIP_MUNICH)
    //MIU0
    unsigned long entrylo0 = ((0x50000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1 = ((0x50000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU1
    unsigned long entrylo0_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_lo256mb = ((0x60000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7
    unsigned long entrylo0_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // cacheable
    unsigned long entrylo1_miu1_hi256mb = ((0x70000000 >> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // un-cacheable or 0x7

    //MIU0 (256MB - 512MB)
    add_wired_entry(entrylo0, entrylo1, 0xC0000000, 0x1fffe000);  //  PageMask[28:13]=0x1fffe000(256MB max size)

    //MIU1 (0MB - 256MB)
    add_wired_entry(entrylo0_miu1_lo256mb, entrylo1_miu1_lo256mb, 0x00000000, 0x1fffe000);

    //MIU1 (256MB - 512MB)
    add_wired_entry(entrylo0_miu1_hi256mb, entrylo1_miu1_hi256mb, 0x20000000, 0x1fffe000);
#else
    unsigned long entrylo0 = ((0x40000000>> 12) << 6) | (0 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // cacheable
    unsigned long entrylo1 = ((0x40000000>> 12) << 6) | (2 << 3) | (1 << 2) | (1 << 1) | (1<< 0); // un-cacheable or 0x7
    add_wired_entry(entrylo0, entrylo1, MIU1_CACHEABLE_START , 0x1fffe000);
#endif
#endif

#if (defined(__arm__))
    mhal_init_mmu();
#endif

#endif

#if (defined(__arm__))
    extern void ProcessTimer0Isr(void);
#else
    extern void ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
#endif

    MsOS_CPU_AttachInterrupt( E_INTERRUPT_TICK_TIMER, ProcessTimer0Isr, 0 );
    MAsm_CPU_TimerInit();
    MDrv_IRQ_Init();

#ifdef CONFIG_UTOPIA_FRAMEWORK
#ifdef CONFIG_MBOOT
    // In fastboot, utopia init is not necessary
    //UtopiaInit();
#endif
#endif

    return true;
}
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
MS_S32 MsOS_CreateMemoryPool (MS_U32   u32PoolSize,
                           MS_U32   u32MinAllocation,
                           void     *pPoolAddr,
                           MsOSAttribute eAttribute,
                           char     *pPoolName)
{
   //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
   //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

extern unsigned char __heap[];
extern unsigned char __heap_end[];
//-------------------------------------------------------------------------------------------------
/// Allocate a memory block with 16-Byte aligned starting address from the variable-size memory pool
/// @param  u32Size     \b IN: request size
/// @param  s32PoolId   \b IN: memory pool ID
/// @return NULL : not enough available memory
/// @return Otherwise : pointer to the allocated memory block
//-------------------------------------------------------------------------------------------------
void * MsOS_AllocateMemory (MS_U32 u32Size, MS_S32 s32PoolId)
{
#if 1
    // UNUSED(s32PoolId);
    void *ptr = malloc(u32Size);
    // printf("\nMsOS_AllocateMemory: ptr = 0x%x\n",ptr);
    if(ptr == NULL)
    {
        printf("\nMsOS_AllocateMemory will return NULL --> u32Size: 0x%x (heap, heap_end) = (0x%08x, 0x%08x)\n", (int)u32Size, (int)__heap, (int)__heap_end);
        //show_mem_info();
    }

    //clear Dcache data
    MsOS_Dcache_Flush((MS_U32)ptr,u32Size);

    //clear OCP data
    MsOS_FlushMemory();

#endif
#if 0
    extern void *sbrk(unsigned int size);
    void *ptr = sbrk(u32Size);
    printf("ptr=%x\n", ptr);
    if(ptr == NULL)
    {
    printf("\nMsOS_AllocateMemory will return NULL --> u32Size: 0x%x\n", u32Size);
    }
#endif
    return ptr;
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
    //do nothing:
    // printf("MsOS Free=%lx\n",pAddress);
    // UNUSED(s32PoolId);
    free(pAddress);
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
    //printf("\n**************[%s] --> u32Size: 0x%x , addr: 0x%x\n",__FUNCTION__, (unsigned int)u32NewSize,(unsigned int)pOrgAddress);
    return realloc(pOrgAddress, u32NewSize);
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
MS_S32 MsOS_CreateTask (TaskEntry  pTaskEntry,
                     MS_VIRT     u32TaskEntryData,
                     TaskPriority eTaskPriority,
                     MS_BOOL    bAutoStart,
                     void       *pStackBase,
                     MS_U32     u32StackSize,
                     char *pTaskName)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
}

//-------------------------------------------------------------------------------------------------
/// Delete a previously created task
/// @param  s32TaskId   \b IN: task ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTask (MS_S32 s32TaskId)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Yield the execution right to ready tasks with "the same" priority
/// @return None
//-------------------------------------------------------------------------------------------------
void MsOS_YieldTask (void)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
/// @note   Not allowed in interrupt context; otherwise, exception will occur.
//-------------------------------------------------------------------------------------------------
void MsOS_DelayTask (MS_U32 u32Ms)
{
#if 1
    MS_U32 delay_start_time;
    if ( u32Ms < 10 )
        u32Ms++;
    delay_start_time = MsOS_GetSystemTime();
    while ( (MsOS_GetSystemTime() - delay_start_time) < u32Ms );
#else
	MsOS_DelayTaskUs(u32Ms*1000);
#endif
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
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get all task ID
/// @param  ps32TaskIdList   \b IN/OUT: the memory for the all task ID
/// @return number of task created
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_InfoTaskList (MS_S32* ps32TaskIdList)
{
    return 0;
}


//-------------------------------------------------------------------------------------------------
/// Get current task ID
/// @return >=0 : current task ID
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_InfoTaskID (void)
{
    return -1;
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
/// @return >=0 : assigned mutex Id
/// @return <0 : fail
/// @note   A mutex has the concept of an owner, whereas a semaphore does not.
///         A mutex provides priority inheritance protocol against proiorty inversion, whereas a binary semaphore does not.
//-------------------------------------------------------------------------------------------------
MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag)
{
    return 1;
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
    return TRUE;
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
    return TRUE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return -1;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
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
    MS_S32 s32TimerId;

    extern MS_S32 Timer_IRQ_Register(void *ptCb);
    extern void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set);
    extern void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
    extern void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set);

    s32TimerId=Timer_IRQ_Register(pTimerCb);
    Timer_Period_Register(u32PeriodTimeMs,s32TimerId);
    Timer_Start_set(bStartTimer,s32TimerId);
    Timer_firsttime_interval(u32FirstTimeMs, s32TimerId);

    return s32TimerId;
}

//-------------------------------------------------------------------------------------------------
/// Delete the Timer
/// @param  s32TimerId  \b IN: Timer ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_DeleteTimer (MS_S32 s32TimerId)
{
    extern void Timer_IRQ_Delete(MS_S32 timer_ld);
    extern void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set);
    extern void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
    extern void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set);

    Timer_IRQ_Delete(s32TimerId);
    Timer_Period_Register(1,s32TimerId);
    Timer_Start_set(FALSE,s32TimerId);
    Timer_firsttime_interval(0, s32TimerId);
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
    extern void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
    Timer_Start_set(TRUE,s32TimerId);
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
    extern void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
    Timer_Start_set(FALSE,s32TimerId);
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
    extern void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set);
    extern void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
    extern void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set);

    Timer_Period_Register(u32PeriodTimeMs,s32TimerId);
    Timer_Start_set(bStartTimer,s32TimerId);
    Timer_firsttime_interval(u32FirstTimeMs, s32TimerId);

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
    MS_U32 systime;
    systime = MAsm_GetSystemTime();
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return systime;
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    // mdrv_irq_attach(eIntNum, (mhal_isr_t)pIntCb, 0);
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
    // mdrv_irq_detech(eIntNum);
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
    // mdrv_irq_unmask(eIntNum);
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
    // mdrv_irq_mask(eIntNum);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Disable all interrupts (including timer interrupt), the scheduler is disabled.
/// @return Interrupt register value before all interrupts disable
//-------------------------------------------------------------------------------------------------
MS_U32 MsOS_DisableAllInterrupts(void)
{
    MDrv_IRQ_MaskAll();
    // mdrv_irq_maskall();
    return 0x12345678;
}

//-------------------------------------------------------------------------------------------------
/// Restore the interrupts from last MsOS_DisableAllInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_DisableAllInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_RestoreAllInterrupts(MS_U32 u32OldInterrupts)
{
    MDrv_IRQ_Restore();
    // mdrv_irq_restore();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable all CPU interrupts.
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_EnableAllInterrupts(void)
{
    MDrv_IRQ_UnMaskAll();
    // mdrv_irq_unmask_all();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// In Interuupt Context or not
/// @return TRUE : Yes
/// @return FALSE : No
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_In_Interrupt (void)
{
    return MDrv_IRQ_InISR();
    // return mdrv_irq_inisr();
    // return FALSE;
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
    mhal_dcache_flush(u32Start, u32Size);
    return TRUE;
}

#if defined(CHIP_A2) || \
    defined(CHIP_T12) || \
    defined(CHIP_T8) || \
    defined(CHIP_J2) || \
    defined(CHIP_T11) || \
    defined(CHIP_A5) || \
    defined(CHIP_A5P) || \
    defined(CHIP_A3) || \
    defined(CHIP_K2) || \
    defined(CHIP_EDISON)
MS_BOOL MsOS_L2Cache_Flush(void)
{
    mhal_L2Cache_Flush();
    return TRUE;
}

MS_BOOL MsOS_L2Cache_Read(void)
{
    mhal_L2Cache_Read();
    return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dcache_Invalidate( MS_VIRT u32Start, MS_SIZE u32Size )
{

    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
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
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

MS_PHY MsOS_VA2PA(MS_VIRT addr)
{
    return (MS_PHY)HAL_MsOS_MPool_VA2PA(addr);
}

extern MS_VIRT _MPool_PA2VA(MS_PHY pAddrPhys, MS_BOOL bNonCache);
MS_VIRT MsOS_PA2KSEG0(MS_PHY addr)
{
    MS_VIRT pVirtAddr =  _MPool_PA2VA(addr, FALSE);	
    if(!pVirtAddr)	
	 pVirtAddr = (MS_VIRT)HAL_MsOS_MPool_PA2KSEG0(addr);
    return pVirtAddr;	
}

MS_VIRT MsOS_PA2KSEG1(MS_PHY addr)
{
    MS_VIRT pVirtAddr =  _MPool_PA2VA(addr, TRUE);	
    if(!pVirtAddr)	
	 pVirtAddr = (MS_VIRT)HAL_MsOS_MPool_PA2KSEG1(addr);
    return pVirtAddr;	
}

/* FIXME: patch for monaco bring up */
#define SHM_SIZE                (2 * 1024 * 1024)
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
    return mhal_interrupt_disable();
}

//-------------------------------------------------------------------------------------------------
/// Enable the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_EnableInterrupt (void)
{
    mhal_interrupt_enable();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore the CPU interrupt from last MsOS_CPU_DisableInterrupts.
/// @param  u32OldInterrupts \b IN: Interrupt register value from @ref MsOS_CPU_DisableInterrupts
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_RestoreInterrupt (MS_U32 u32OldInterrupts)
{
    mhal_interrupt_restore(u32OldInterrupts);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask all the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskAllInterrupt (void)
{
    mhal_interrupt_mask_all();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Mask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_MaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    mhal_interrupt_mask(intr_num);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// UnMask the CPU interrupt
/// @param  intr_num \b IN: Interrupt number in enumerator MHAL_INTERRUPT_TYPE
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnMaskInterrupt (MHAL_INTERRUPT_TYPE intr_num)
{
    mhal_interrupt_unmask(intr_num);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Lock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_LockInterrupt (void)
{
    mhal_interrupt_lock();
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// UnLock the CPU interrupt
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_UnLockInterrupt (void)
{
    mhal_interrupt_unlock();
    return TRUE;
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
    mhal_interrupt_attach(intr_num, isr, dat);
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
    mhal_interrupt_detach(intr_num);
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
    mhal_exception_attach(expt_num, isr, dat);
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
    mhal_exception_detach(expt_num);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32Addr \b IN: MIPS Code Start Address
/// @return TRUE : succeed
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_CPU_SetEBASE (MS_U32 u32Addr)
{
    MAsm_CPU_SetEBASE(u32Addr);
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
    //This is for linux feature only, do nothing here;
    return 0;
}
// #endif
