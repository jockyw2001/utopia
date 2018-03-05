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

//#include <MsCommon.h> // NUSED
//#include <cyg/hal/hal_if.h> // NUSED
#include "include/drvKernel.h"
//#include "include/drvTimer.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED

extern int ms_usb_core_init(void);
extern int ms_usb_core_uninit(void);
extern struct ms_bus_type usb_bus_type;
extern S32 usb_bus_list_lock;
extern void init_cache_memory(void);

// The previous code has serious bug.  Removed. Jonas_20110519
// The following code has been rewitten by  Jonas_20110519
#define BITOP_WORD(nr)  ((nr) / BITS_PER_LONG)
/**
     * @brief               find out the position of the next zero
     *
     * @param           U32 *addr
     * @param           U32 size
     * @param           U32 offset
     *
     * @return          zero bit position
     */
U32 find_next_zero_bit(const U32 *addr, U32 size, U32 offset)
{
    const U32 *p = addr + BITOP_WORD(offset);
    U32 ms_result = offset & ~(BITS_PER_LONG-1);
    U32 ms_tmp;

    if (offset >= size)
        return size;
    size -= ms_result;
    offset %= BITS_PER_LONG;
    if (offset)
    {
        ms_tmp = *(p++);
        ms_tmp |= ~0UL >> (BITS_PER_LONG - offset);
        if (size < BITS_PER_LONG)
            goto ms_found_first;
        if (~ms_tmp)
            goto ms_found_middle;
        size -= BITS_PER_LONG;
        ms_result += BITS_PER_LONG;
    }
    while (size & ~(BITS_PER_LONG-1))
    {
        if (~(ms_tmp = *(p++)))
            goto ms_found_middle;
        ms_result += BITS_PER_LONG;
        size -= BITS_PER_LONG;
    }
    if (!size)
        return ms_result;
    ms_tmp = *p;

ms_found_first:
    ms_tmp |= ~0UL << size;
    if (ms_tmp == ~0UL)
        return ms_result + size;
ms_found_middle:
    return ms_result + ms_find_1st_zero(ms_tmp);
}

/**
     * @brief               find out the position of the next zero
     *
     * @param           U32 *addr
     * @param           U32 size
     * @param           U32 offset
     *
     * @return          zero bit position
     */
U32 find_next_zero_bit_le(U32 *addr, U32 size, U32 offset)
{
    return find_next_zero_bit(addr, size, offset);
}

#ifndef HAL_DELAY_US
    externC void hal_delay_us(int us);
    #define HAL_DELAY_US(n)    hal_delay_us(n)
#endif
/**
     * @brief               wait for N us
     *
     * @param           U32 us
     *
     * @return          none
     */
inline void udelay(U32 us)
{
   HAL_DELAY_US(us);
}

/**
     * @brief               initial USB system
     *
     * @param           none
     *
     * @return          none
     */
void ms_init_sys(void)
{
    init_cache_memory();
    osapi_mutex_init(usb_bus_list_lock, "USB_BUS_MUTEX");
    ms_list_init(&usb_bus_type.devices_list);
    ms_list_init(&usb_bus_type.drivers_list);

    //sti();

    // Init timer and init software timer link list
    ms_init_timers();

    // Init USB core (including hub)
    ms_usb_core_init();
}

void ms_exit_sys(void)
{
    ms_usb_core_uninit();
    ms_remove_timers();

    //cli();
    osapi_mutex_delete(usb_bus_list_lock);
    destroy_NC_mem();
}

/**
     * @brief               wait for N ms
     *
     * @param           int x
     *
     * @return          none
     */
void wait_ms(int x)
{
    if (x==0)
        return;
    MsOS_DelayTask(x);
}

/**
     * @brief               create USB event flag
     *
     * @param           S32 *USBWaitFlg
     *
     * @return          none
     */
void ms_init_OS_Resource_EX(S32 *USBWaitFlg)
{
    *USBWaitFlg = MsOS_CreateEventGroup("USB_Event");
}

void ms_release_OS_Resource_EX(S32 *USBWaitFlg)
{
    MsOS_DeleteEventGroup(*USBWaitFlg);
}

