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
//#include "types.h"
//#include "mhal_arch.h"

//#define R2_ICACHE_SIZE              (0x2000)
//#define R2_DCACHE_SIZE              (0x1000)

//#define R2_ICACHE_LINE_SIZE         32
//#define R2_ICACHE_WAY               4

//#define R2_DCACHE_LINE_SIZE         16
//#define R2_DCACHE_WAY               2

//extern int _RISC32_DCACHE_SIZE;
//extern int _RISC32_DCACHE_WAYS;
//extern int _RISC32_DCACHE_LINE_SIZE;

#define _RISC32_DCACHE_SIZE             (0x1000)
#define _RISC32_DCACHE_LINE_SIZE        16
#define _RISC32_DCACHE_WAYS             2

/*This patch is for A3-R2 D-cache bug*/
#define PATCH_0001                  1
/*In R2, support flush_invalidate_line instruction. it is neccessary to use r2-elf tool-chain*/
#define R2_SUPPROT_FLUSH_INVALID    0

#include "risc32_spr.h"

__attribute__((always_inline)) static inline void local_irq_restore(unsigned long flags)
{
    asm volatile("l.mtspr   r0,%0,%1" :: "r"(flags), "i"(SPR_SR));
}

#define local_irq_save(flags)       \
    do { \
    unsigned long sr; \
    asm volatile("l.mfspr   %0,r0,%1" : "=r"(flags) : "i"(SPR_SR)); \
    sr= flags & ~(SPR_SR_TEE | SPR_SR_IEE); \
    asm volatile("l.mtspr   r0,%0,%1" :: "r"(sr), "i"(SPR_SR)); \
} while(0)

#define local_sr_save(sr) {}
#define local_sr_write(sr,flag) {}

#undef  _BUILT_IN_

/**
* @brief Synchronize write buffer
* @return None
*/
#ifdef _BUILT_IN_
#define HAL_DRAIN_WRITE_BUFFER()        \
    do {                                    \
    __builtin_syncwb();                 \
} while(0);
#else
#define HAL_DRAIN_WRITE_BUFFER()        \
    do {                                    \
    asm volatile("b.syncwritebuffer");  \
} while(0);
#endif

/**
* @brief Flush and invalidate all D-Cache with 1 instruction by way, and it does not need exact address
* @return None
*/
static inline void hal_dcache_flush_invalidate_all(void)
{
    unsigned int addr;
    unsigned int loops;

    addr = 0;
    loops = (_RISC32_DCACHE_SIZE)/(_RISC32_DCACHE_WAYS)/(_RISC32_DCACHE_LINE_SIZE);
    while (loops) {
    #if R2_SUPPROT_FLUSH_INVALID
    #ifdef _BUILT_IN_
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 3);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 3);
        #if (_RISC32_DCACHE_WAYS>1)
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 2);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 2);
        #endif
        #if (_RISC32_DCACHE_WAYS>2)
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 1);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 1);
        #endif
        #if (_RISC32_DCACHE_WAYS>3)
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 0);
        __builtin_flush_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 0);
        #endif
    #else
        asm volatile(
            "l.flush_invalidate_line         0(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line      (%1)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*2)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*3)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*4)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*5)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*6)(%0), 3\n" /* way 3 */
            "l.flush_invalidate_line    (%1*7)(%0), 3\n" /* way 3 */
            #if (_RISC32_DCACHE_WAYS>1)
            "l.flush_invalidate_line         0(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line      (%1)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*2)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*3)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*4)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*5)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*6)(%0), 2\n" /* way 2 */
            "l.flush_invalidate_line    (%1*7)(%0), 2\n" /* way 2 */
            #endif
            #if (_RISC32_DCACHE_WAYS>2)
            "l.flush_invalidate_line         0(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line      (%1)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*2)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*3)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*4)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*5)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*6)(%0), 1\n" /* way 1 */
            "l.flush_invalidate_line    (%1*7)(%0), 1\n" /* way 1 */
            #endif
            #if (_RISC32_DCACHE_WAYS>3)
            "l.flush_invalidate_line         0(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line      (%1)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*2)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*3)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*4)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*5)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*6)(%0), 0\n" /* way 0 */
            "l.flush_invalidate_line    (%1*7)(%0), 0\n" /* way 0 */
            #endif

            :
            : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
        );
    #endif
    #else
    #ifdef _BUILT_IN_
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 3);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 3);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 3);

        #if (_RISC32_DCACHE_WAYS>1)
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 2);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 2);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 2);
        #endif
        #if (_RISC32_DCACHE_WAYS>2)
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 1);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 1);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 1);
        #endif
        #if (_RISC32_DCACHE_WAYS>3)
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 0);
        __builtin_flush_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6), 0);
        __builtin_invalidate_line( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7), 0);
        #endif
    #else
        asm volatile(
            "l.flush_line        0(%0), 3\n" /* way 3 */
            "l.flush_line     (%1)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*2)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*3)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*4)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*5)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*6)(%0), 3\n" /* way 3 */
            "l.flush_line   (%1*7)(%0), 3\n" /* way 3 */
            "l.invalidate_line       0(%0), 3\n" /* way 3 */
            "l.invalidate_line    (%1)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*2)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*3)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*4)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*5)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*6)(%0), 3\n" /* way 3 */
            "l.invalidate_line  (%1*7)(%0), 3\n" /* way 3 */
            #if (_RISC32_DCACHE_WAYS>1)
            "l.flush_line        0(%0), 2\n" /* way 2 */
            "l.flush_line     (%1)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*2)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*3)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*4)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*5)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*6)(%0), 2\n" /* way 2 */
            "l.flush_line   (%1*7)(%0), 2\n" /* way 2 */
            "l.invalidate_line       0(%0), 2\n" /* way 2 */
            "l.invalidate_line    (%1)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*2)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*3)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*4)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*5)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*6)(%0), 2\n" /* way 2 */
            "l.invalidate_line  (%1*7)(%0), 2\n" /* way 2 */
            #endif
            #if (_RISC32_DCACHE_WAYS>2)
            "l.flush_line        0(%0), 1\n" /* way 1 */
            "l.flush_line     (%1)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*2)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*3)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*4)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*5)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*6)(%0), 1\n" /* way 1 */
            "l.flush_line   (%1*7)(%0), 1\n" /* way 1 */
            "l.invalidate_line       0(%0), 1\n" /* way 1 */
            "l.invalidate_line    (%1)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*2)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*3)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*4)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*5)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*6)(%0), 1\n" /* way 1 */
            "l.invalidate_line  (%1*7)(%0), 1\n" /* way 1 */
            #endif
            #if (_RISC32_DCACHE_WAYS>3)
            "l.flush_line        0(%0), 0\n" /* way 0 */
            "l.flush_line     (%1)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*2)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*3)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*4)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*5)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*6)(%0), 0\n" /* way 0 */
            "l.flush_line   (%1*7)(%0), 0\n" /* way 0 */
            "l.invalidate_line       0(%0), 0\n" /* way 0 */
            "l.invalidate_line    (%1)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*2)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*3)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*4)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*5)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*6)(%0), 0\n" /* way 0 */
            "l.invalidate_line  (%1*7)(%0), 0\n" /* way 0 */
            #endif

            :
            : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
        );

    #endif
    #endif
        addr += _RISC32_DCACHE_LINE_SIZE * 8;
        loops -= 8;
    }
    HAL_DRAIN_WRITE_BUFFER();
}

/**
* @brief Flush and invalidate D-Cache with address with 1 instruction
* @param base Exact physical Address to be flushed and invalidated
* @param size Size to be flushed and invalidated
* @return None
*/
static inline void hal_dcache_flush_invalidate(unsigned int base, unsigned int size)
{
    unsigned int addr;
    unsigned int addr_end;
    unsigned int loops;

    addr = base & -(_RISC32_DCACHE_LINE_SIZE);
    addr_end = base + size + (_RISC32_DCACHE_LINE_SIZE - 1);
    addr_end &= -(_RISC32_DCACHE_LINE_SIZE);

    if (size < _RISC32_DCACHE_SIZE) {
        loops = (addr_end - addr) / _RISC32_DCACHE_LINE_SIZE;
        while (loops) {
            /*This patch is for A3-R2 D-cache bug*/
            #if (PATCH_0001 == 1)
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)(addr));
            #else
                asm volatile(
                    "l.flush_invalidate 0(%0)\n\t"
                    :: "r"(addr)
            );
            #endif
            asm volatile(
                "l.movhi r31, 0x8000\n\t"
                "l.lwz r31, 0(r31)\n\t"
                :::"r31"
            );
            addr += _RISC32_DCACHE_LINE_SIZE;
            loops -= 1;
            #else
            if (loops >= 16) {
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 8));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 9));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 10));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 11));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 12));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 13));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 14));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 15));
            #else
                asm volatile(
                    "l.flush_invalidate 0(%0)\n"
                    "l.flush_invalidate (%1)(%0)\n"
                    "l.flush_invalidate (%1*2)(%0)\n"
                    "l.flush_invalidate (%1*3)(%0)\n"
                    "l.flush_invalidate (%1*4)(%0)\n"
                    "l.flush_invalidate (%1*5)(%0)\n"
                    "l.flush_invalidate (%1*6)(%0)\n"
                    "l.flush_invalidate (%1*7)(%0)\n"
                    "l.flush_invalidate (%1*8)(%0)\n"
                    "l.flush_invalidate (%1*9)(%0)\n"
                    "l.flush_invalidate (%1*10)(%0)\n"
                    "l.flush_invalidate (%1*11)(%0)\n"
                    "l.flush_invalidate (%1*12)(%0)\n"
                    "l.flush_invalidate (%1*13)(%0)\n"
                    "l.flush_invalidate (%1*14)(%0)\n"
                    "l.flush_invalidate (%1*15)(%0)\n"
                    :
                    : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
                );
            #endif
                addr += _RISC32_DCACHE_LINE_SIZE << 4;
                loops -= 16;
            } else if (loops >= 8) {
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 4));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 5));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 6));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 7));
            #else
                asm volatile(
                    "l.flush_invalidate 0(%0)\n"
                    "l.flush_invalidate (%1)(%0)\n"
                    "l.flush_invalidate (%1*2)(%0)\n"
                    "l.flush_invalidate (%1*3)(%0)\n"
                    "l.flush_invalidate (%1*4)(%0)\n"
                    "l.flush_invalidate (%1*5)(%0)\n"
                    "l.flush_invalidate (%1*6)(%0)\n"
                    "l.flush_invalidate (%1*7)(%0)\n"
                    :
                    : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
                );
            #endif
                addr += _RISC32_DCACHE_LINE_SIZE << 3;
                loops -= 8;
            } else if (loops >= 4) {
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 2));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 3));
            #else
                asm volatile (
                    "l.flush_invalidate 0(%0)\n"
                    "l.flush_invalidate (%1)(%0)\n"
                    "l.flush_invalidate (%1*2)(%0)\n"
                    "l.flush_invalidate (%1*3)(%0)\n"
                    :
                    : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
                );
            #endif
                addr += _RISC32_DCACHE_LINE_SIZE << 2;
                loops -= 4;
            } else if (loops >= 2) {
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 0));
                __builtin_flush_invalidate( (void *)(addr) + (_RISC32_DCACHE_LINE_SIZE * 1));
            #else
                asm volatile(
                    "l.flush_invalidate 0(%0)\n"
                    "l.flush_invalidate (%1)(%0)\n"
                    :
                    : "r"(addr), "K"(_RISC32_DCACHE_LINE_SIZE)
                );
            #endif
                addr += _RISC32_DCACHE_LINE_SIZE << 1;
                loops -= 2;
            } else {
            #ifdef _BUILT_IN_
                __builtin_flush_invalidate( (void *)addr );
            #else
                asm volatile(
                    "l.flush_invalidate 0(%0)\n"
                    :
                    : "r"(addr)
                );
            #endif
                addr += _RISC32_DCACHE_LINE_SIZE;
                loops -= 1;
            }
            #endif
        }
        HAL_DRAIN_WRITE_BUFFER();
    } else {
        hal_dcache_flush_invalidate_all();
    }
}

/**
* @brief Flush and invalidate D-Cache
* @param base Physical Address to be flushed
* @param size Size to be invalidated
* @return None
*/
//#define  r2_dcache_flush_invalidate(base,size)
void Hal_Dcache_Flush(unsigned int base, unsigned int size)
{
    int flags;

    local_irq_save(flags);
    hal_dcache_flush_invalidate(base,size);
    local_irq_restore(flags);
}

