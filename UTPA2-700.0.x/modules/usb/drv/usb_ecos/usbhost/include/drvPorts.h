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


#ifndef _PORTS_H_
#define _PORTS_H_

#include <cyg/hal/hal_arch.h>
#include "MsCommon.h"
#include "../drvUsbcommon.h"

#undef  CONFIG_FARADAY_USB
#define CONFIG_FARADAY_USB


#define USB_ASSERT( _bool_, _msg_ ) \
        do { \
            if ( ! ( _bool_ ) ) \
             {   while (1){} } \
        } while (0);



/* Cache instruction opcodes */
#define HAL_CACHE_OP(which, op)             (which | (op << 2))
#define HAL_DCACHE_HIT_INVALIDATE           0x5             // write back and invalidate
#define HAL_HIT_INVALIDATE                  0x4             // address invalidate

#define HAL_WHICH_ICACHE                    0x0
#define HAL_WHICH_DCACHE                    0x1

///#define HAL_DCACHE_LINE_SIZE            16      // Size of a data cache line

#define HAL_MIPS32_DCACHE_START_ADDRESS(_addr_) \
(((U32)(_addr_)) & ~(HAL_DCACHE_LINE_SIZE-1))

#define HAL_MIPS32_DCACHE_END_ADDRESS(_addr_, _asize_) \
(((U32)((_addr_) + (_asize_) + (HAL_DCACHE_LINE_SIZE-1) )) & ~(HAL_DCACHE_LINE_SIZE-1))


#if 0

#define USBHAL_DCACHE_FLUSH( _base_ , _asize_ )                           \
  do {                                                   \
    register U32 _addr_  = HAL_MIPS32_DCACHE_START_ADDRESS((U32)(_base_));        \
    register U32 _eaddr_ = HAL_MIPS32_DCACHE_END_ADDRESS((U32)(_base_), _asize_); \
    for( ; _addr_ < _eaddr_; _addr_ += HAL_DCACHE_LINE_SIZE )               \
      asm volatile (" cache %0, 0(%1)"                                                  \
                    :                                                                   \
                    : "I" (HAL_CACHE_OP(HAL_WHICH_DCACHE,  HAL_DCACHE_HIT_INVALIDATE)),         \
                      "r"(_addr_));                                                       \
     } while (0)

#define USBHAL_DCACHE_INVALIDATE( _base_ , _asize_ )                           \
    do {                                                        \
    register U32 _addr_  = HAL_MIPS32_DCACHE_START_ADDRESS( (U32)(_base_) );        \
    register U32 _eaddr_ = HAL_MIPS32_DCACHE_END_ADDRESS( (U32)(_base_) , (_asize_) ); \
    for( ; _addr_ < _eaddr_; _addr_ += HAL_DCACHE_LINE_SIZE )               \
      asm volatile (" cache %0, 0(%1)"                                                  \
                    :                                                                   \
                    : "I" (HAL_CACHE_OP(HAL_WHICH_DCACHE, HAL_HIT_INVALIDATE)),         \
                      "r"(_addr_));                                                       \
     } while (0)
#endif
#define wmb() HAL_REORDER_BARRIER()
#define rmb() HAL_REORDER_BARRIER()
#define barrier() HAL_REORDER_BARRIER()

// Debug Function
#define KERN_DEBUG          "KERN_DEBUG "
#define KERN_ERR            "KERN_ERR "
#define KERN_INFO           "KERN_INFO "
#define KERN_WARNING        "KERN_WARNING "

#define printk(x,...)

#define	USB_sprintf	   sprintf //UTL_sprintf

#define CPUToLE32(_x_) (_x_)
#define	LE16ToCPU(x)	(x)
#define	CPUToLE16(x)	(x)
#define	LE32ToCPU(x)	(x)
// Delay routines
#define DelayTime(x)	MsOS_DelayTask(x)

//#define wait_ms(x) DelayTime(x)
#define mdelay(x)  DelayTime(x)
// Linux Device Driver related ports
extern void wait_ms(int x);

#define __init
#define __exit

// Prefetch for List (ARM v4 doesn't supports this feature
static __inline__ void prefetch(void *x) {;}  // Do nothing

/* ========================================  Function Declaration  ======================================== */
extern void udelay(MS_U32 us);
extern void busyloop_delay(MS_U32 count);

/* ========================================  Data structures  ======================================== */

// Used for ISR to stored the processor's context befor the processor entered interrupt code
struct pt_regs {
  MS_U32 uregs[18];
};
#define ARM_cpsr  uregs[16]
#define ARM_pc    uregs[15]
#define ARM_lr    uregs[14]
#define ARM_sp    uregs[13]
#define ARM_ip    uregs[12]
#define ARM_fp    uregs[11]
#define ARM_r10   uregs[10]
#define ARM_r9    uregs[9]
#define ARM_r8    uregs[8]
#define ARM_r7    uregs[7]
#define ARM_r6    uregs[6]
#define ARM_r5    uregs[5]
#define ARM_r4    uregs[4]
#define ARM_r3    uregs[3]
#define ARM_r2    uregs[2]
#define ARM_r1    uregs[1]
#define ARM_r0    uregs[0]
#define ARM_ORIG_r0  uregs[17]

struct pci_dev{
  void *dev;
};

#define CPE_FAKE_PCIDEV ((struct pci_dev *) 1111)
#define pcidev_is_CPE(dev) (dev == CPE_FAKE_PCIDEV)

// Timer data structure for Root hub

#define SLAB_KERNEL   0     //unmeaning
#define SLAB_ATOMIC   1     //unmeaning
#define GFP_KERNEL    2     //unmeaning
#define GFP_ATOMIC    3     //unmeaning
#define GFP_NOIO      4     //unmeaning
#define ASYNC_MAKE_QTD		5  //Asynchronous IO mode for making qtd list
#define ASYNC_SUBMIT_QTD	6  //Asynchronous IO mode for sumbitting qtd list

#define min_t(type,x,y) \
  ( (type)x < (type)y ? (type) x : (type) y)
#define max_t(type,x,y) \
  ( (type)x > (type)y ? (type) x : (type) y)

#ifdef min
#undef min
#define min(x,y) \
  ( x < y ?  x :  y)
#endif
#ifdef max
#undef max
#define max(x,y) \
  ( x > y ? x : y)
#endif

#define BUG()
extern void init_sys(void);
extern void exit_sys(void);
#endif


