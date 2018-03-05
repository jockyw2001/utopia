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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMPool.c
/// @brief  Memory pool manager
/// @author MStar Semiconductor,Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

// Common Definition

#include "cyg/kernel/kapi.h"
#include "cyg/hal/hal_arch.h"
#include "cyg/hal/hal_diag.h"
#include "cyg/hal/hal_cache.h"


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

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define MPOOL_DEBUG
#endif

#ifdef MPOOL_DEBUG
    #define MPOOL_WARN(fmt, args...)         (printf("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ##args))
#else
    #define MPOOL_WARN(fmt, args...)         while (0)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
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
#else
//cached/unchched segment
#define KSEG0_BASE              ((void *)0x80000000)
#define KSEG1_BASE              ((void *)0xa0000000)

#endif

#define MPOOL_ERROR(fmt, args...)       while (0)
#define MPOOL_PRINT(fmt, args...)       while (0)
#define MPOOL_ASSERT(_bool, _f)         if (!(_bool)) { (_f); }
#define MPOOL_IS_CACHE_ADDR(addr)       ( ( ((MS_U32)_pAddrVirtStart[0] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[0] > (MS_U32)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)     ( ( ((MS_U32)_pAddrVirtStart[1] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[1] > (MS_U32)addr) ) ? TRUE : FALSE )
#define DCACHE_LINE_SIZE 16

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MsOS_MPool_Init(void)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}

MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT addr)
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3) || defined(CHIP_JANUS))
            return ((MS_U32)(addr) & ~(0xC0000000));
    #else
            return ((MS_U32)(addr) & ~(0x80000000));
    #endif
#else
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
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
        #elif (defined(CHIP_KENYA))// kenya has only 1 MIU
                // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000
                // miu0 (low 256MB) (VA)0xA000:0000~0xB000:0000 --> (PA)0x0000:0000~0x1000:0000
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
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
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC) || defined(CHIP_KAPPA))
                // miu0
                // K1 has only 1 MIU
                if ((0x80000000 <= addr) && (0x90000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                if ((0xA0000000 <= addr) && (0xB0000000 > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) & 0x1fffffff));
                }
                return 0;
        #elif (defined(CHIP_K6)||defined(CHIP_K6LITE)||defined(CHIP_K7U)||defined(CHIP_KANO))
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

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY addr) // un-cache
{
#if (defined(MCU_AEON))
    #if (defined(CHIP_T3) || defined(CHIP_U3))
            return ((MS_U32)(addr) | (0xC0000000));
    #else
            return ((MS_U32)(addr) | (0x80000000));
    #endif
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
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
        #elif (defined(CHIP_KENYA))
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
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC) || defined(CHIP_KAPPA))
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0xa0000000));
                }
                return 0;
        #elif (defined(CHIP_K6)||defined(CHIP_K6LITE)||defined(CHIP_K7U)||defined(CHIP_KANO))
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

inline MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY addr) // cache
{
#if (defined(MCU_AEON))
        return (addr);
#else // for mips and refine it later
    #if (!defined(CHIP_T2) && !defined(CHIP_U3))
        #if (defined(CHIP_T12) || defined(CHIP_A2) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A3) || defined(CHIP_K2) || defined(CHIP_Edison) || defined(CHIP_EIFFEL) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRIS))
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
        #elif (defined(CHIP_KENYA))
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
                return 0;
        #elif (defined(CHIP_K1) || defined(CHIP_KELTIC)|| defined(CHIP_KAPPA))
                // miu0
                if ((0x00000000 <= addr) && (HAL_MIU1_BASE > addr))
                {
                    return ((MS_U32)(((MS_U32)addr) | 0x80000000));
                }
                return 0;
        #elif (defined(CHIP_K6)||defined(CHIP_K6LITE)||defined(CHIP_K7U)||defined(CHIP_KANO))
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


inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_U32 u32Start, MS_U32 u32Size)
{
    MS_U32 u32OldIntr;
    MS_U32 u32Addr;

    u32Addr = u32Start & 0xFFFFFFF0;
    u32Size+= (u32Start - u32Addr);
    u32Size = ALIGN_16(u32Size);

    if ( (u32Addr % DCACHE_LINE_SIZE) || ( (u32Addr & (MS_U32)KSEG1_BASE) != (MS_U32)KSEG0_BASE) )
    {
        return FALSE;
    }
    u32OldIntr = MsOS_DisableAllInterrupts();
    HAL_DCACHE_FLUSH( u32Addr, u32Size);
    //flush EC's write FIFO
    MAsm_CPU_Sync();
    MsOS_RestoreAllInterrupts(u32OldIntr);
    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_U32 u32Offset, MS_U32 u32MapSize, MS_BOOL bNonCache)
{
    MPOOL_WARN("No Implement \n");
    return TRUE;
}
