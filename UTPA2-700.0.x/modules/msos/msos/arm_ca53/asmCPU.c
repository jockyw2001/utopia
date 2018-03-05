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
#ifdef CONFIG_MBOOT
#define RIUBASE   0x1F000000
//#define PERIBASE  0x16000000
#else
#define RIUBASE   0xFD000000
//#define PERIBASE  0xFC000000
#endif

#if defined (__aarch64__)
#else
#define MSOS_HAL_RTC_DENOMINATOR 100
const MS_U32 msos_hal_rtc_period = 12000000 / MSOS_HAL_RTC_DENOMINATOR; //CNTCLK 12MHz -> ARM timer
#endif
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
void MAsm_CPU_Sync(void)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "dsb sy;"
        );
#else
    asm __volatile__ (
        "DSB"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// Nop
/// @return None
//-------------------------------------------------------------------------------------------------
inline void MAsm_CPU_Nop(void)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
        );
#else
    asm __volatile__ (
        "nop;"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// SW Debug Breakpoint
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SwDbgBp(void)
{
    // @FIXME: Richard: which trap should I issue?
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
        );
#else 
    asm __volatile__ (
        "BKPT 0x1234;"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enter CPU power saving mode
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_PowerDown(void)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Set Status BEV
/// @param  bBEV \b IN: TRUE/FALSE: 1 bootstrap / 0 normal
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_StatusBEV(MS_BOOL bBEV)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Jump to the specified PC
/// @param  u32PC \b IN: PC
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_Jump(MS_U32 u32PC)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Get trail one
/// @param  u32Flags \b IN: 32-bit flag
/// @return trail one position
//-------------------------------------------------------------------------------------------------
MS_U8 MAsm_CPU_GetTrailOne(MS_U32 u32Flags)
{
    MS_U32 i;
    for (i = 0; i< 32; i++)
    {
        if (u32Flags & (1<< i))
        {
            return i;
        }
    }
    return 32;
}

//-------------------------------------------------------------------------------------------------
/// Disable system timer interrupt
/// @param  bEnable \b IN: TRUE: Enable timer interrupt, FALSE: Disable timer interrupt.
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_EnableTimerInterrupt(MS_BOOL bEnable)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

/*To do: you should use MsOS_DelayTask instead of this API*/
void MAsm_CPU_DelayMs(MS_U32 msec)
{
    #define CPU_LOOP_MSEC(_msec)        ((_msec) * ((ARM_CLOCK_FREQ / 2 / 1000) / 3)) // 3 cycles / loop

    register MS_U32 loop = CPU_LOOP_MSEC(msec);
    while(loop--);
}

#if defined (__aarch64__)
#else
static void _physical_count_read(MS_U64 *pvalue)
{
    MS_U32 temp;
    MS_U32 temp1;

    asm volatile(
        "mrrc p15, 0, r0, r1, c14\n"
        "str r0, %0\n"
        "str r1, %1\n"
        :"=m"(temp), "=m"(temp1)::"r0","r1"
    );

    *pvalue = (MS_U64)temp1 << 32 | temp;
}

static void _hal_delay_us(MS_U32 usecs)
{
    MS_U64 val1, val2, delaycount;
    MS_U64 usticks;

    // Calculate the number of counter register ticks per microsecond.
    usticks = msos_hal_rtc_period * MSOS_HAL_RTC_DENOMINATOR / 1000000;

    // Make sure that the value is not zero. This will only happen if the
    // CPU is running at < 2MHz.
    if( usticks == 0 ) usticks = 1;

    delaycount = usticks * usecs;

    _physical_count_read(&val1);

    while( usecs > 0 )
    {
        _physical_count_read(&val2);
        if ((val2 - val1) > delaycount)
            break;
    }
}
#endif
//-------------------------------------------------------------------------------------------------
/// Pause for specifc duration by CPU dummy instruction
/// @param  usec \b IN: microseconds ( usec < (12800000/CPU_CLK_MHZ) )
/// @return None
//-------------------------------------------------------------------------------------------------

void MAsm_CPU_DelayUs(MS_U32 usec)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
    );
#else
#if (defined(CHIP_KANO) || defined(CHIP_K6) || defined(CHIP_K7U))
    _hal_delay_us(usec);
#else
    asm __volatile__ (
        "mov  r0, %0\n"
        "wait: subs    r0, r0, #1\n"
        "bne     wait\n"
        :
        :"r" (usec)
        :"memory"
    );
#endif
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32addr \b IN: MIPS Code Start Address
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SetEBASE(MS_U32 u32addr)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//
// System time
//


//-------------------------------------------------------------------------------------------------
/// Get current system time in timer ticks
/// @return system time in timer ticks
//-------------------------------------------------------------------------------------------------
MS_U32 gsystem_time_ms=0;
MS_U32 MAsm_GetSystemTime (void)
{
    return gsystem_time_ms;
}

void MAsm_CPU_TimerInit(void)
{
#if defined (__aarch64__)     
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));
    asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (ARM_CLOCK_FREQ / 2 / 1000));
#endif
}

void MAsm_CPU_TimerStart(void)
{
#if defined (__aarch64__)     
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (1));
#endif    
}

void MAsm_CPU_TimerClean(void)
{
#if defined (__aarch64__)     
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));
#endif   
}
