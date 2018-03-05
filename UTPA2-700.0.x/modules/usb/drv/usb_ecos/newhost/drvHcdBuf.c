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
//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvErrno.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvPCIMEM.h" // NUSED
//#include "include/drvTimer.h" // NUSED
#include "include/drvKernel.h"

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUsbd.h"

void ms_hcd_buffer_destroy (struct usb_hcd *hcd);

static const size_t  pool_max [HCD_MEM_POOLS] = {
#if (_USB_128_ALIGMENT)
	0,
	0,
#else
    32,
    64,
#endif
    128,
    512,
    PAGE_SIZE / 2
};

/**
     * @brief               create hcd buffer
     *
     * @param           struct usb_hcd *hcd
     *
     * @return          buffer pointer
     */
int ms_hcd_buffer_create (struct usb_hcd *hcd)
{
    char    name [16];
    int     i, pool_size;

    for (i = 0; i < HCD_MEM_POOLS; i++)
    {
        if (!(pool_size = pool_max [i]))
            continue;
        USB_sprintf(name, /*sizeof(name),*/ "buffer-%d", pool_size);
        hcd->pool [i] = ms_mem_pool_create (name, pool_size, pool_size, 0);
        if (!hcd->pool [i])
        {
            ms_hcd_buffer_destroy (hcd);
            return -ENOMEM;
        }
    }
    return 0;
}

/**
     * @brief               hcd buffer destroy
     *
     * @param           struct usb_hcd *hcd
     *
     * @return          none
     */
void ms_hcd_buffer_destroy (struct usb_hcd *hcd)
{
    int    i;

    for (i = 0; i < HCD_MEM_POOLS; i++)
    {
        struct ms_mem_pool    *pool = hcd->pool [i];

        if (pool)
        {
            ms_mem_pool_destroy (pool);
            hcd->pool [i] = 0;
        }
    }
}

/**
     * @brief               hcd buffer memory allocate
     *
     * @param           struct usb_bus     *bus
     * @param           size_t      size
     * @param           void       *addr
     * @param           dma_addr_t    dma
     *
     * @return          buffer pointer
     */
void *ms_hcd_buffer_alloc (
    struct usb_bus     *bus,
    size_t      size,
    int      mem_flags,
    dma_addr_t    *dma
)
{
    struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
    int       i;

    for (i = 0; i < HCD_MEM_POOLS; i++)
    {
        if (size <= pool_max [i])
            return ms_mem_pool_alloc (hcd->pool [i], mem_flags, dma);
    }
    return ncmem_alloc_page(dma);
}

/**
     * @brief               hcd buffer memory free
     *
     * @param           struct usb_bus     *bus
     * @param           size_t      size
     * @param           void       *addr
     * @param           dma_addr_t    dma
     *
     * @return          none
     */
void ms_hcd_buffer_free (
    struct usb_bus     *bus,
    size_t      size,
    void       *addr,
    dma_addr_t    dma
)
{
    struct usb_hcd    *hcd = (struct usb_hcd*) bus->hcpriv;
    int       i;

    for (i = 0; i < HCD_MEM_POOLS; i++)
    {
        if (size <= pool_max [i])
        {
            ms_mem_pool_free (hcd->pool [i], addr, dma);
            return;
        }
    }
    ncmem_free_page((U32)addr);
}

