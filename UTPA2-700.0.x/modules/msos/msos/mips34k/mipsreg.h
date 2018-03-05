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
#ifndef _ASM_MIPSREG_H
#define _ASM_MIPSREG_H

/*
 * Coprocessor 0 register names
 */
#define CP0_INDEX $0
#define CP0_RANDOM $1
#define CP0_ENTRYLO0 $2
#define CP0_ENTRYLO1 $3
#define CP0_CONF $3
#define CP0_CONTEXT $4
#define CP0_PAGEMASK $5
#define CP0_WIRED $6
#define CP0_INFO $7
#define CP0_BADVADDR $8
#define CP0_COUNT $9
#define CP0_ENTRYHI $10

/*
 * Functions to access the R10000 performance counters.  These are basically
 * mfc0 and mtc0 instructions from and to coprocessor register with a 5-bit
 * performance counter number encoded into bits 1 ... 5 of the instruction.
 * Only performance counters 0 to 1 actually exist, so for a non-R10000 aware
 * disassembler these will look like an access to selection 0 or 1.
 */

/*
 * Macros to access the system control coprocessor
 */
#define __read_32bit_c0_register(src, selectionection)				\
({ int __res;								\
	if (selectionection == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #src "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #src ", " #selectionection "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})


#define __write_32bit_c0_register(register, selection, val)			\
do {									\
	if (selection == 0)							\
		__asm__ __volatile__(					\
			"mtc0\t%z0, " #register "\n\t"			\
			: : "Jr" ((unsigned int)(val)));		\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mtc0\t%z0, " #register ", " #selection "\n\t"	\
			".set\tmips0"					\
			: : "Jr" ((unsigned int)(val)));		\
} while (0)


#define __read_ulong_c0_register(reg, selection)				\
	(unsigned long) __read_32bit_c0_register(reg, selection) ;		\

#define __write_ulong_c0_register(reg, selection, val)			\
do {									\
		__write_32bit_c0_register(reg, selection, val);		\
} while (0)


#define write_c0_by_index(value)	__write_32bit_c0_register($0, 0, value)
#define write_c0_entrylow0(value)	__write_ulong_c0_register($2, 0, value)
#define write_c0_entrylow1(value)	__write_ulong_c0_register($3, 0, value)
#define read_c0_by_pagemask()	__read_32bit_c0_register($5, 0)
#define write_c0_by_pagemask(value)	__write_32bit_c0_register($5, 0, value)
#define read_c0_with_wired()		__read_32bit_c0_register($6, 0)
#define write_c0_with_wired(value)	__write_32bit_c0_register($6, 0, value)
#define read_c0_entryhigh()	__read_ulong_c0_register($10, 0)
#define write_c0_entryhigh(value)	__write_ulong_c0_register($10, 0, value)


/*
 * TLB operations.
 *
 * It is responsibility of the caller to take care of any TLB hazards.
 */
static inline void tlb_index_write(void)
{
	__asm__ __volatile__(
		".set noreorder\n\t"
		"tlbwi\n\t"
		".set reorder");
}


#endif /* _ASM_MIPSREG_H */
