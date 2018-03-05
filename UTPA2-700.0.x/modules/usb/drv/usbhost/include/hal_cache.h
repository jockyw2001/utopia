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
#ifndef CYGONCE_HAL_CACHE_H
#define CYGONCE_HAL_CACHE_H

//=============================================================================
//
//      hal_cache.h
//
//      HAL cache control API
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Scott Furman
// Contributors:
// Date:        2003-02-08
// Purpose:     Cache control API
// Description: The macros defined here provide the HAL APIs for handling
//              cache control operations.
// Usage:
//              #include <cyg/hal/hal_cache.h>
//              ...
//
//
//####DESCRIPTIONEND####
//
//=============================================================================

//-----------------------------------------------------------------------------
// Cache dimensions.
// These really should be defined in var_cache.h. If they are not, then provide
// a set of numbers that are typical of many variants.

#ifndef HAL_DCACHE_SIZE

// Data cache
#ifdef __AEONR2__
#define HAL_DCACHE_SIZE                 32768    // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            16      // Bytes in a data cache line
#define HAL_DCACHE_WAYS                 4 // 1       // Associativity of the cache
// Instruction cache
#define HAL_ICACHE_SIZE                 16384 // 4096    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Bytes in a cache line
#define HAL_ICACHE_WAYS                 2 //  1       // Associativity of the cache
#else
#define HAL_DCACHE_SIZE                 4096    // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            16      // Bytes in a data cache line
#define HAL_DCACHE_WAYS                 2 // 1       // Associativity of the cache
// Instruction cache
#define HAL_ICACHE_SIZE                 8192 // 4096    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            16      // Bytes in a cache line
#define HAL_ICACHE_WAYS                 2 //  1       // Associativity of the cache
#endif



#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#endif

#ifndef __ASSEMBLER__

#include "hal_arch.h"
#ifdef NOS_MIPS
void mhal_dcache_flush(unsigned long u32Base, unsigned long u32Size );
#else //NOS_MIPS
//-----------------------------------------------------------------------------
// Global control of data cache
#ifndef __AEONR2__
// Enable the data cache
#define HAL_DCACHE_ENABLE() MTSPR(SPR_SR, MFSPR(SPR_SR) | SPR_SR_DCE)

// Disable the data cache
#define HAL_DCACHE_DISABLE() MTSPR(SPR_SR, MFSPR(SPR_SR) & ~SPR_SR_DCE)

// Enable or disable the data cache, depending on argument, which is required
// to be 0 or 1.
#define HAL_SET_DCACHE_ENABLED(enable)                          \
    MTSPR(SPR_SR, MFSPR(SPR_SR) | (SPR_SR_DCE & -(enable)))

// Invalidate the entire data cache
#define HAL_DCACHE_INVALIDATE_ALL()                             \
    CYG_MACRO_START                                             \
    int cache_enabled, addr;                                    \
                                                                \
    /* Save current cache mode (disabled/enabled) */            \
    HAL_DCACHE_IS_ENABLED(cache_enabled);                       \
                                                                \
    /* Disable cache, so that invalidation ignores cache tags */\
    HAL_DCACHE_DISABLE();                                       \
    addr = HAL_DCACHE_SIZE;                                     \
    do {                                                        \
        MTSPR(SPR_DCBIR, addr);                                 \
        addr -= HAL_DCACHE_LINE_SIZE;                           \
    } while (addr > 0);                                         \
                                                                \
    /* Re-enable cache if it was enabled on entry */            \
    HAL_SET_DCACHE_ENABLED(cache_enabled);                      \
    CYG_MACRO_END

// Synchronize the contents of the cache with memory.
// (Unnecessary on OR12K, since cache is write-through.)
#define HAL_DCACHE_SYNC()                       \
    CYG_MACRO_START                             \
    CYG_MACRO_END

// Query the state (enabled/disabled) of the data cache
#define HAL_DCACHE_IS_ENABLED(_state_)                          \
    CYG_MACRO_START                                             \
    (_state_) = (1 - !(MFSPR(SPR_SR) & SPR_SR_DCE));            \
    CYG_MACRO_END

// Load the contents of the given address range into the data cache
// and then lock the cache so that it stays there.

// The OpenRISC architecture defines these operations, but no
// implementation supports them yet.

//#define HAL_DCACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_DCACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_DCACHE_UNLOCK_ALL()


//-----------------------------------------------------------------------------
// Data cache line control

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
// OR12k has write-through cache, so no flushing of writes to memory
// are necessary.
#define HAL_DCACHE_FLUSH( _base_ , _size_ )                          \
    HAL_DCACHE_INVALIDATE(_base_, _size_)

// Invalidate cache lines in the given range without writing to memory.
#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )                     \
    CYG_MACRO_START                                                  \
    U32 addr;                                                        \
    U32 end = (U32)_base_ + (U32)_size_;                                       \
    for (addr = end; addr >= (U32)_base_; addr -= HAL_DCACHE_LINE_SIZE) { \
        MTSPR(SPR_DCBIR, addr);                                      \
    }                                                                \
    CYG_MACRO_END

// Write dirty cache lines to memory for the given address range.
// OR12k has write-through cache, so this is a NOP
#define HAL_DCACHE_STORE( _base_ , _size_ )

// Preread the given range into the cache with the intention of reading
// from it later.
//#define HAL_DCACHE_READ_HINT( _base_ , _size_ )

// Preread the given range into the cache with the intention of writing
// to it later.
//#define HAL_DCACHE_WRITE_HINT( _base_ , _size_ )

// Allocate and zero the cache lines associated with the given range.
//#define HAL_DCACHE_ZERO( _base_ , _size_ )

//-----------------------------------------------------------------------------
// Global control of Instruction cache

// Enable the instruction cache
#define HAL_ICACHE_ENABLE() MTSPR(SPR_SR, MFSPR(SPR_SR) | SPR_SR_ICE)

// Disable the instruction cache
#define HAL_ICACHE_DISABLE() MTSPR(SPR_SR, MFSPR(SPR_SR) & ~SPR_SR_ICE)

// Enable or disable the data cache, depending on argument, which must
// be 0 or 1.
#define HAL_SET_ICACHE_ENABLED(enable)                          \
    MTSPR(SPR_SR, MFSPR(SPR_SR) | (SPR_SR_ICE & -(enable)))

// Invalidate the entire instruction cache
#define HAL_ICACHE_INVALIDATE_ALL()                             \
    CYG_MACRO_START                                             \
    int cache_enabled, addr;                                    \
                                                                \
    /* Save current cache mode (disabled/enabled) */            \
    HAL_ICACHE_IS_ENABLED(cache_enabled);                       \
                                                                \
    /* Disable cache, so that invalidation ignores cache tags */\
    HAL_ICACHE_DISABLE();                                       \
    addr = HAL_ICACHE_SIZE;                                     \
    do {                                                        \
        MTSPR(SPR_ICBIR, addr);                                 \
        addr -= HAL_ICACHE_LINE_SIZE;                           \
    } while (addr > 0);                                         \
                                                                \
    /* Re-enable cache if it was enabled on entry */            \
    HAL_SET_ICACHE_ENABLED(cache_enabled);                      \
    CYG_MACRO_END

// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC() HAL_ICACHE_INVALIDATE_ALL()

// Query the state of the instruction cache
#define HAL_ICACHE_IS_ENABLED(_state_)                          \
    CYG_MACRO_START                                             \
    (_state_) = (1 - !(MFSPR(SPR_SR) & SPR_SR_ICE));            \
    CYG_MACRO_END

#endif
// Load the contents of the given address range into the instruction cache
// and then lock the cache so that it stays there.

// The OpenRISC architecture defines these operations, but no
// implementation supports them yet.

//#define HAL_ICACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_ICACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_ICACHE_UNLOCK_ALL()

#endif /* __ASSEMBLER__ */
#endif//NOS_MIPS
#endif // ifndef CYGONCE_HAL_CACHE_H
// End of hal_cache.h
