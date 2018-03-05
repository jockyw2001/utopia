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
//#include "include/drvTimer.h" // NUSED
//#include "include/drvList.h" // NUSED
//#include "include/drvKernel.h" // NUSED

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUsbd.h"

/**
     * @brief           create new urb
     *
     * @param           int iso_packets
     * @param           int mem_flags
     *
     * @return          allocated urb
     */
struct urb *ms_usb_alloc_urb(int mem_flags)
{
    struct urb *ms_urb;

    ms_urb = (struct urb *)kmalloc(sizeof(struct urb), mem_flags);
    if (!ms_urb)
    {
        ms_usbhost_err("alloc_urb: kmalloc failed%s","\n");
        return NULL;
    }
    memset(ms_urb, 0, sizeof(*ms_urb));
    ms_urb->count = (atomic_t)osapi_ATOMIC_INIT(1);
    osapi_spin_lock_init(&ms_urb->lock);
    return ms_urb;
}

/**
     * @brief           test urb reference count and free the memory
     *
     * @param           struct urb *ms_urb
     *
     * @return          none
     */
void ms_usb_free_urb(struct urb *ms_urb)
{
    if (ms_urb)
        if (osapi_atomic_dec_and_test(&ms_urb->count))
            kfree(ms_urb);
}

/**
     * @brief           increase urb reference count
     *
     * @param           struct urb *urb
     *
     * @return          current urb
     */
struct urb * ms_usb_get_urb(struct urb *ms_urb)
{
    if (ms_urb)
    {
        osapi_atomic_inc(&ms_urb->count);
        return ms_urb;
    }
    else
        return NULL;
}

/**
     * @brief           submit urb to the queue
     *
     * @param           struct urb *ms_urb
     * @param           int mem_flags
     *
     * @return          error code
     */
int ms_usb_submit_urb(struct urb *ms_urb, int mem_flags)
{
    int ms_pipe, temp_p, max_p;
    struct usb_device  *pUdev;
    //struct ms_usb_bus_ops  *usb_op;
    int      is_out;

    if (!ms_urb || ms_urb->hcpriv || !ms_urb->complete_func)
        return -EINVAL;
    if (!(pUdev = ms_urb->dev) || (pUdev->eState < USB_STATE_DEFAULT) ||
        (!pUdev->bus) || (pUdev->u32DevNum <= 0))
        return -ENODEV;
    //if (!(usb_op = pUdev->bus->bus_ops) || !usb_op->ms_hcd_submit_urb)
    //    return -ENODEV;

    ms_urb->s32Status = -EINPROGRESS;
    ms_urb->u32ActualLength = 0;

    ms_pipe = ms_urb->u32Pipe;
    temp_p = usb_pipetype (ms_pipe);
    is_out = usb_pipeout (ms_pipe);

    if (!usb_pipecontrol (ms_pipe) && pUdev->eState < USB_STATE_CONFIGURED)
        return -ENODEV;

    /* patch from linux, obsolete code */
    //if (usb_endpoint_halted (pUdev, usb_pipeendpoint (ms_pipe), is_out))
    //    return -EPIPE;

    max_p = usb_maxpacket (pUdev, ms_pipe, is_out);
    if (max_p <= 0)
    {
        ms_usbhost_err ("<%s> wrong endpoint %d-%s on usb-%s-%s (bad maxpacket %d)",
            __FUNCTION__,
            usb_pipeendpoint (ms_pipe), is_out ? "OUT" : "IN",
            pUdev->bus->bus_name, pUdev->devpath,
            max_p);
        return -EMSGSIZE;
    }

    if (ms_urb->u32TransferBufferLength < 0)
        return -EMSGSIZE;

    //only support interrupt pipe
    if (temp_p == EP_INTERRUPT)
    {
            if (ms_urb->u32Interval <= 0)
                return -EINVAL;
            switch (pUdev->eSpeed)
            {
                case USB_HIGH_SPEED:  /* units are microframes */
                    // NOTE usb handles 2^15
                    if (ms_urb->u32Interval > (1024 * 8))
                        ms_urb->u32Interval = 1024 * 8;
                    temp_p = 1024 * 8;
                    break;
                case USB_FULL_SPEED:  /* units are frames/msec */
                case USB_LOW_SPEED:
                    {
                        if (ms_urb->u32Interval > 255)
                            return -EINVAL;
                        // NOTE ohci only handles up to 32
                        temp_p = 128;
                    }
                    break;
                default:
                    return -EINVAL;
            }
            while (temp_p > ms_urb->u32Interval)
                temp_p >>= 1;
            ms_urb->u32Interval = temp_p;
    }

    //return usb_op->ms_hcd_submit_urb (ms_urb, mem_flags);
    return ms_hcd_submit_urb (ms_urb, mem_flags);
}

/**
     * @brief           unlink urb from the queue
     *
     * @param           struct urb *ms_urb
     *
     * @return          error code
     */
int ms_usb_unlink_urb(struct urb *ms_urb)
{
    //if (ms_urb && ms_urb->dev && ms_urb->dev->bus && ms_urb->dev->bus->bus_ops)
    //    return ms_urb->dev->bus->bus_ops->ms_hcd_unlink_urb(ms_urb);
    //else
    //    return -ENODEV;
    if (!ms_urb)
        return -EINVAL;
    if (!ms_urb->dev)
        return -ENODEV;
    return ms_hcd_unlink_urb(ms_urb, -ECONNRESET);
}
