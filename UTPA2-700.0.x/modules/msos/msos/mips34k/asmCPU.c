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
/// file    asmCPU.c
/// @brief  MIPS Inline Assembly Wrapper
/// @author MStar Semiconductor Inc.
/// @note   Compile only with mips32 or mips32r2, but not mips16 or mips16e
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "asmCPU.h"
#include "halCHIP.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Flush EC's write FIFO
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_Sync(void)
{
    asm volatile (
        "sync;"
        );
}

//-------------------------------------------------------------------------------------------------
/// Nop
/// @return None
//-------------------------------------------------------------------------------------------------
inline void ATTRIBUTE  MAsm_CPU_Nop(void)
{
    asm volatile (
        "nop;"
        );
}

//-------------------------------------------------------------------------------------------------
/// SW Debug Breakpoint
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_SwDbgBp(void)
{
    asm volatile (
        "SDBBP;"
        );
}

//-------------------------------------------------------------------------------------------------
/// Enter CPU power saving mode
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_PowerDown(void)
{
    asm volatile (
        "wait;"
        "nop;"
        "nop;"
    );
}

//-------------------------------------------------------------------------------------------------
/// Set Status BEV
/// @param  bBEV \b IN: TRUE/FALSE: 1 bootstrap / 0 normal
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_StatusBEV(MS_BOOL bBEV)
{
    MS_U32 u32BEV = ((MS_U32)bBEV) << 22;

    asm volatile (
        "mfc0   $8, $12;"
        "nop;"
        "move   $9, %0;"
        "or     $8, $8, $9;"
        "mtc0   $8, $12;"
        "nop; nop; nop;"
        :
        : "r"(u32BEV)
        : "$8", "$9"
        );
}

//-------------------------------------------------------------------------------------------------
/// Jump to the specified PC
/// @param  u32PC \b IN: PC
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_Jump(MS_U32 u32PC)
{
    typedef void ( *DirectJump ) (void);
    DirectJump pFunc = (DirectJump)u32PC;
    (*pFunc)();
}

//-------------------------------------------------------------------------------------------------
/// Get trail one
/// @param  u32Flags \b IN: 32-bit flag
/// @return trail one position
//-------------------------------------------------------------------------------------------------
MS_U8 ATTRIBUTE  MAsm_CPU_GetTrailOne(MS_U32 u32Flags)
{
    u32Flags = (~u32Flags) & (u32Flags-1);
    asm volatile
    (
        "clz %0, %0\n"
        : "=r" (u32Flags)
        : "0" (u32Flags)
    );
    return 32 - u32Flags;
}

//-------------------------------------------------------------------------------------------------
/// Disable system timer interrupt
/// @param  bEnable \b IN: TRUE: Enable timer interrupt, FALSE: Disable timer interrupt.
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_EnableTimerInterrupt(MS_BOOL bEnable)
{
    if(bEnable)
    {
        asm volatile (
            "mfc0   $8, $12;"
            "nop;"
            "or     $8,$8,0x00008000;"      // IM7 = 1, timer interrupt enabled.
            "mtc0   $8,$12;"
            "nop; nop; nop;"
        );
    }
    else
    {
        asm volatile (
            "mfc0   $8, $12;"
            "nop;"
            "and    $8,$8,0XFFFF7FFF;"      // IM7 = 0, timer interrupt disabled.
            "mtc0   $8,$12;"
            "nop; nop; nop;"
        );
    }
}

//-------------------------------------------------------------------------------------------------
/// Pause for specifc duration by CPU dummy instruction
/// @param  msec \b IN: miniseconds ( msec < (12800000/CPU_CLK_MHZ) )
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_DelayMs(MS_U32 msec)
{
    #define CPU_LOOP_MSEC(_msec)        ((_msec)*(MIPS_CLOCK_FREQ/1000/3)) // 3 cycles / loop

    register MS_U32 loop = CPU_LOOP_MSEC(msec);
    while(loop--);
}

//-------------------------------------------------------------------------------------------------
/// Pause for specifc duration by CPU dummy instruction
/// @param  usec \b IN: microseconds ( usec < (12800000/CPU_CLK_MHZ) )
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_DelayUs(MS_U32 usec)
{
#if 0
    #define CPU_LOOP_USEC(_usec)        ((_usec)*(MIPS_CLOCK_FREQ/1000000/3)) // 3 cycles / loop

    register MS_U32 loop = CPU_LOOP_USEC(usec);
    while(loop--);
#endif

    asm __volatile__ (
        "li     $8, %0\n"
        "mul    $9, $4, $8\n"
        "1:\n"
        "addiu  $9, $9, -1\n"
        "bnez   $9, 1b\n"
        :
        :"I" (MIPS_CLOCK_FREQ/1000000/3)
        :"$8", "$9"
    );
}

//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32addr \b IN: MIPS Code Start Address
/// @return None
//-------------------------------------------------------------------------------------------------
void ATTRIBUTE  MAsm_CPU_SetEBASE(MS_U32 u32addr)
{
    asm __volatile__ (
        // Set interrupt mode to compaitible mode.
        // 1. Need not to fill IntCtlVs, the default value is already 0.
        // 2. Set CauseIv = 0. Use general exception vector. page 66, 117.
        "mfc0    $26, $13;"         //mfc0    k0, cause
        "nop; nop;"                 //mfc0_delay
        "li     $27, ~(1<<23);"     //li      k1, ~(1<<23)
        "and    $26, $26, $27;"     //and     k0, k0, k1
        "mtc0   $26, $13;"          //mtc0    k0, cause
        "nop; nop;"                 //mfc0_delay

        // 3. Set StatusBEV  = 1 and adjust EBASE coprocessor
        "mfc0   $26, $12;"          //mfc0    k0, status
        "nop; nop;"                 //mfc0_delay
        "li     $27, (1<<22);"      //li      k1, (1<<22)
        "or     $26, $26, $27;"     //or      k0, k0, k1
        "mtc0   $26, $12;"          //mtc0    k0, status
        "nop; nop;"                 //mfc0_delay

        "mfc0   $26, $15, 1;"       //mfc0    k0, $15, 1
        "nop; nop;"                 //mfc0_delay
        "move   $27, %0;"           //move    k1, u32addr
        "or     $26, $26, $27;"     //or      k0, k0, k1
        "mtc0   $26, $15, 1;"       //mtc0    k0, $15, 1
        "nop; nop;"                 //mfc0_delay

        // 4. Set StatusBEV & StatusEXL to be zero. page 66,
        "mfc0   $26, $12;"          //mfc0    k0, status
        "nop; nop;"                 //mfc0_delay
        "li     $27, ~((1<<22)|(1<<1));" //li      k1, ~((1<<22)|(1<<1))
        "and    $26, $26, $27;"     //and     k0, k0, k1
        "mtc0   $26, $12;"          //mtc0    k0, status
        "nop; nop;"                 //mfc0_delay
        // Now the Refill exception vector is u32addr and
        // general exception vector is u32addr.
        :
        : "r"(u32addr)
        : "$12", "$13", "$15", "$26", "$27"
        );
}

//-------------------------------------------------------------------------------------------------
/// Get current system time in timer ticks
/// @return system time in timer ticks
//-------------------------------------------------------------------------------------------------
volatile MS_U32 gsystem_time_ms=0;
MS_U32 ATTRIBUTE  MAsm_GetSystemTime (void)
{
    return gsystem_time_ms;
}


void ATTRIBUTE  MAsm_CPU_TimerInit(void)
{

    asm __volatile__ (
            "lui     $8, ((%0*1000)>>16);"
            "ori     $8, $8, ((%0*1000)&0xFFFF);"
            "mtc0    $0,$9;"
            "nop; nop;  nop;"
            "mtc0    $8,$11;"
            "nop; nop;  nop;"
            :
        :"I" (MIPS_CLOCK_FREQ/1000000/2)
        :"$8", "$9"
            );

}


#define PAGE_SHIFT              12
#define CKSEG0                  0x80000000
#define UNIQUE_ENTRYHI(idx) (CKSEG0 + ((idx) << (PAGE_SHIFT + 1)))

#include "mipsreg.h"

void ATTRIBUTE  local_flush_tlb_all(void)
{
    unsigned long flags;
    unsigned long old_ctx;
    int entry;

    // ENTER_CRITICAL(flags);
    flags = MsOS_CPU_DisableInterrupt();
    /* Save old context and create impossible VPN2 value */
    old_ctx = read_c0_entryhigh();
    write_c0_entrylow0(0);
    write_c0_entrylow1(0);

    entry = read_c0_with_wired();

    /* Blast 'em all away. */
    while (entry < 16){ // current_cpu_data.tlbsize) {
        /* Make sure all entries differ. */
        write_c0_entryhigh(UNIQUE_ENTRYHI(entry));
        write_c0_by_index(entry);
        // mtc0_tlbw_hazard();
        asm volatile ("sll    $0, $0, 3;");
        tlb_index_write();
        entry++;
    }
    // tlbw_use_hazard();
    asm volatile ("sll    $0, $0, 3;");
    write_c0_entryhigh(old_ctx);
    // FLUSH_ITLB;
    // EXIT_CRITICAL(flags);
    MsOS_CPU_RestoreInterrupt (flags);
}

void ATTRIBUTE  add_wired_entry(unsigned long entrylow0, unsigned long entrylow1,
        unsigned long entryhigh, unsigned long pagemask)
{
    unsigned long wired;
    unsigned long old_pagemask;
    unsigned long old_ctx;
    MS_U32 u32OldInt;

    u32OldInt = MsOS_CPU_DisableInterrupt();

    // ENTER_CRITICAL(flags);
    /* Save old context and create impossible VPN2 value */
    old_ctx = read_c0_entryhigh();
    old_pagemask = read_c0_by_pagemask();
    wired = read_c0_with_wired();
    write_c0_with_wired(wired + 1);
    write_c0_by_index(wired);

    // tlbw_use_hazard();      /* What is the hazard here? */
    asm volatile ("sll    $0, $0, 3;");

    write_c0_by_pagemask(pagemask);
    write_c0_entryhigh(entryhigh);
    write_c0_entrylow0(entrylow0);
    write_c0_entrylow1(entrylow1);
    // mtc0_tlbw_hazard();
    asm volatile ("sll    $0, $0, 3;");
    tlb_index_write();
    // tlbw_use_hazard();
    asm volatile ("sll    $0, $0, 3;");

    write_c0_entryhigh(old_ctx);
    // richard tlbw_use_hazard();      /* What is the hazard here? */
    asm volatile ("sll    $0, $0, 3;");
    write_c0_by_pagemask(old_pagemask);
    local_flush_tlb_all();
    MsOS_CPU_RestoreInterrupt (u32OldInt);
    // EXIT_CRITICAL(flags);
}

