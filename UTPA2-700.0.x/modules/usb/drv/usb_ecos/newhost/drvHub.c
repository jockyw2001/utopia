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

#include  "include/drvConfig.h"
//#include  "include/drvCompiler.h" // NUSED
//#include  "include/drvTypes.h" // NUSED
#include  "include/drvErrno.h"
//#include  "include/drvPorts.h" // NUSED
//#include  "include/drvPCIMEM.h" // NUSED
//#include  "include/drvTimer.h" // NUSED
//#include  "include/drvList.h" // NUSED
#include  "include/drvKernel.h"
//#include  "include/drvBitops.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUSBCore.h"
//#include "drvUsbd.h" // NUSED
//#include "drvHub.h" // NUSED
#include "drvEHCI.h"
#include "drvUSBHwCtl.h"
/* applying drvUsbHostConfig.h (inside drvUSBHwCtl.h) */

//#define DBG_DBG
#ifdef DBG_DBG
#undef ms_usbhost_debug
#define ms_usbhost_debug(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#endif

//struct list_head hub_list =            { &(hub_list), &(hub_list) };

/**
     * @brief               Interpreting post speed by text
     *
     * @param           int portstatus
     *
     * @return          Speed description
     */
static __inline__ char *ms_portspeed (int portstatus)
{
    if (portstatus & USB_PORT_STAT_HIGH_SPEED)
        return "480 Mb/s";
    else if (portstatus & USB_PORT_STAT_LOW_SPEED)
        return "1.5 Mb/s";
    else
        return "12 Mb/s";
}

/**
     * @brief               retrieving  generic device pointer from usb device
     *
     * @param           struct usb_device *ms_dev
     *
     * @return          generic device pointer
     */
static __inline__ struct device_s *ms_hubdev (struct usb_device *ms_dev)
{
    return &ms_dev->actconfig->interface[0]->dev;
}

/**
     * @brief               hub driver complete function
     *
     * @param           struct urb *urb
     * @param           struct stPtRegs *regs
     *
     * @return          none
     */
static void ms_hub_irq(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_hub *pHub = (struct usb_hub *)urb->pContext;
    U32 flags;
    int status;
    int ms_rtval = ENOENT;
    struct usb_hcd    *hcd = (struct usb_hcd*) urb->dev->bus->hcpriv;

    switch (urb->s32Status)
    {
        case -ENOENT:
        case -ECONNRESET:
        case -ESHUTDOWN:
            ms_usbhost_debug("<%s> urb->status : %d \n", __FUNCTION__, (int)urb->s32Status);
            return;

        default:
            ms_usbhost_debug ("transfer --> %d\n", urb->s32Status);
            ms_rtval = urb->s32Status;
            if ((++pHub->nerrors < 10) || pHub->error)
                goto resubmit;
            pHub->error = urb->s32Status;

        case 0:
            break;
    }

    pHub->nerrors = 0;

    osapi_spin_lock_irqsave(&hub_event_lock, flags);
    if (!pHub->disconnected && ms_is_empty_list(&pHub->event_list))
    {
        //list_add(&pHub->event_list, &hub_event_list);
        ms_insert_list_after(&pHub->event_list, hcd->phub_event); // new, link to the right hcd

        ms_usbhost_debug("%s:%s >>> Add HUB EVENT!\n", hcd->product_desc, interface_to_usbdev(pHub->intf)->devpath);
    }
    osapi_spin_unlock_irqrestore(&hub_event_lock, flags);

resubmit:
    if ((status = ms_usb_submit_urb (pHub->urb, GFP_ATOMIC)) != 0
        /* ENODEV means we raced disconnect() */
        && status != -ENODEV)
    {
        diag_printf ("previous urb status is %d, resubmit --> %d\n", ms_rtval, status);
        diag_printf ("hcd->state=%d\n",hcd->state);
    }
}

/**
     * @brief               hub power on function
     *
     * @param           struct usb_hub *hub
     *
     * @return          none
     */
void ms_hub_power_on(struct usb_hub *hub)
{
    struct usb_device *ms_dev;
    int i;

    ms_usbhost_debug("enabling power on all ports, delay %d\n", hub->descriptor.bPwrOn2PwrGood * 2);
    ms_dev = interface_to_usbdev(hub->intf);
    for (i = 0; i < hub->descriptor.bNbrPorts; i++)
        // (USB_PORT_FEAT_POWER)
        ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
            USB_REQ_SET_FEATURE, USB_RT_PORT, USB_PORT_FEAT_POWER, i + 1, NULL, 0, HZ);

    wait_ms(hub->descriptor.bPwrOn2PwrGood * 2);
}

/**
     * @brief               retrieving hub current condition
     *
     * @param           struct usb_hub *hub
     * @param           U16 *status_r
     * @param           U16 *change_r
     *
     * @return          error code
     */
int ms_hub_hub_status(struct usb_hub *hub, U16 *status_r, U16 *change_r)
{
    struct usb_device *ms_dev = interface_to_usbdev (hub->intf);
    S32 s32Ret;

    // ms_get_hub_status
    s32Ret = ms_usb_control_cmd(ms_dev, usb_rcvctrlpipe(ms_dev, 0),
        USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_HUB, 0, 0,
        &hub->status.hub, sizeof(hub->status.hub), HZ * USB_CTRL_GET_TIMEOUT);
    if (s32Ret < 0)
    {
        ms_usbhost_err ("<%s> failed (err = %d)\n", __FUNCTION__, (int)s32Ret);
    }
    else
    {
        *status_r = hub->status.hub.wHubStatus;
        *change_r = hub->status.hub.wHubChange;
        s32Ret = 0;
    }
    return s32Ret;
}

/**
     * @brief               hub configure by hub descriptor
     *
     * @param           struct usb_hub *hub
     * @param           struct usb_endpoint_descriptor *endpoint
     *
     * @return          error code
     */
static int ms_hub_configure(struct usb_hub *hub,
  struct usb_endpoint_descriptor *endpoint)
{
    struct usb_device *ms_dev = interface_to_usbdev (hub->intf);
    //struct device_s *hub_dev;
    U16 hubstatus, hubchange;
    U32 pipe;
    S32 maxp, s32Ret;
    char *message;

    hub->buffer = (char (*)[3]) ms_usb_buffer_alloc(ms_dev, sizeof(*hub->buffer), GFP_KERNEL,
        &hub->buffer_dma);
    if (!hub->buffer)
    {
        message = "can't allocate hub irq buffer";
        s32Ret = -ENOMEM;
        goto fail;
    }

    // ms_get_hub_descriptor
    s32Ret = ms_usb_control_cmd(ms_dev, usb_rcvctrlpipe(ms_dev, 0),
        USB_REQ_GET_DESCRIPTOR, USB_DIR_IN | USB_RT_HUB,
        USB_DT_HUB << 8, 0, &hub->descriptor, sizeof(hub->descriptor), HZ * USB_CTRL_GET_TIMEOUT);
    if (s32Ret < 0)
    {
        message = "can't read hub descriptor";
        goto fail;
    }
    else if (hub->descriptor.bNbrPorts > USB_MAXCHILDREN)
    {
        message = "hub has too many ports!";
        s32Ret = -ENODEV;
        goto fail;
    }

    //hub_dev = ms_hubdev(ms_dev);
    ms_dev->u32MaxChild = hub->descriptor.bNbrPorts;
    ms_usbhost_msg ("%d port%s detected\n", ms_dev->u32MaxChild, (ms_dev->u32MaxChild == 1) ? "" : "s");

    hub->descriptor.wHubCharacteristics = hub->descriptor.wHubCharacteristics;

    if (hub->descriptor.wHubCharacteristics & HUB_CHAR_COMPOUND)
    {
        int  i;
        char  portstr [USB_MAXCHILDREN + 1];

        for (i = 0; i < ms_dev->u32MaxChild; i++)
          portstr[i] = hub->descriptor.DeviceRemovable[((i + 1) / 8)] & (1 << ((i + 1) % 8))
            ? 'F' : 'R';
        portstr[ms_dev->u32MaxChild] = 0;
        ms_usbhost_msg("compound device; port removable status: %s\n", portstr);
    }
    else
        ms_usbhost_debug("Standalone hub\n");

    switch (hub->descriptor.wHubCharacteristics & HUB_CHAR_LPSM)
    {
        case 0x00:
            ms_usbhost_debug("Ganged power switching\n");
            break;
        case 0x01:
            ms_usbhost_debug("Individual port power switching\n");
            break;
        case 0x02:
        case 0x03:
            ms_usbhost_debug("Unknown reserved power switching mode\n");
            break;
    }

    switch (hub->descriptor.wHubCharacteristics & HUB_CHAR_OCPM)
    {
        case 0x00:
          ms_usbhost_debug("Global over-current protection\n");
          break;
        case 0x08:
          ms_usbhost_debug("Individual port over-current protection\n");
          break;
        case 0x10:
        case 0x18:
          ms_usbhost_debug("No over-current protection\n");
          break;
    }

    osapi_spin_lock_init (&hub->tt.lock);
    ms_list_init (&hub->tt.clear_list);
    //INIT_WORK (&hub->tt.kevent, hub_tt_kevent, hub);
    switch (ms_dev->descriptor.bDeviceProtocol)
    {
        case 0:
          break;
        case 1:
          ms_usbhost_debug("Single TT\n");
          hub->tt.hub = ms_dev;
          break;
        case 2:
          ms_usbhost_debug("TT per port\n");
          hub->tt.hub = ms_dev;
          hub->tt.multi = 1;
          break;
        default:
          ms_usbhost_err("Unrecognized hub protocol %d\n", ms_dev->descriptor.bDeviceProtocol);
          break;
    }

    switch (hub->descriptor.wHubCharacteristics & HUB_CHAR_TTTT)
    {
        case 0x00:
            if (ms_dev->descriptor.bDeviceProtocol != 0)
            {
                hub->tt.think_time = 666;
                ms_usbhost_debug("TT requires at most 8 FS bit times\n");
            }
            break;
        case 0x20:
            hub->tt.think_time = 666 * 2;
            ms_usbhost_debug("TT requires at most 16 FS bit times\n");
            break;
        case 0x40:
            hub->tt.think_time = 666 * 3;
            ms_usbhost_debug("TT requires at most 24 FS bit times\n");
            break;
        case 0x60:
            hub->tt.think_time = 666 * 4;
            ms_usbhost_debug("TT requires at most 32 FS bit times\n");
            break;
    }

    ms_usbhost_debug("Port LEDs are %s supported\n",(hub->descriptor.wHubCharacteristics & HUB_CHAR_PORTIND)? "" : "not");
    ms_usbhost_debug("Power on to power good last time: %dms\n", hub->descriptor.bPwrOn2PwrGood * 2);
    ms_usbhost_debug("Hub current requirement: %dmA\n", hub->descriptor.bHubContrCurrent);

    s32Ret = ms_hub_hub_status(hub, &hubstatus, &hubchange);
    if (s32Ret < 0)
    {
        message = "can't get hub status";
        goto fail;
    }

    ms_usbhost_debug("Local power source is %s\n",(hubstatus & HUB_STATUS_LOCAL_POWER)? "lost (inactive)" : "good");

    ms_usbhost_debug("%s over-current exists\n",(hubstatus & HUB_STATUS_OVERCURRENT) ? "" : "No");

    pipe = usb_rcvintpipe(ms_dev, endpoint->bEndpointAddress);
    maxp = usb_maxpacket(ms_dev, pipe, usb_pipeout(pipe));

    if ((U32) maxp > sizeof(*hub->buffer))
        maxp = sizeof(*hub->buffer);

    hub->urb = ms_usb_alloc_urb(GFP_KERNEL);
    if (!hub->urb)
    {
        message = "couldn't allocate interrupt urb";
        s32Ret = -ENOMEM;
        goto fail;
    }

    ms_usb_stuff_intr_urb(hub->urb, ms_dev, pipe, *hub->buffer, maxp, ms_hub_irq,
        hub, endpoint->bInterval);
    hub->urb->tTransferDma = hub->buffer_dma;
    hub->urb->u32TransferFlags |= MS_FLAG_URB_NO_TRANSFER_DMA_MAP;

    /* power on hub before submit urb */
    ms_hub_power_on(hub);

    s32Ret = ms_usb_submit_urb(hub->urb, GFP_KERNEL);
    if (s32Ret)
    {
        message = "couldn't submit status urb";
        goto fail;
    }

    return 0;

fail:
    ms_usbhost_err("Hub config failed, %s (err %d)\n", message, s32Ret);
    return s32Ret;
}

/**
     * @brief           hub disconnect service
     *
     * @param           struct usb_interface *intf
     *
     * @return          none
     */
static void ms_hub_disconnect(struct usb_interface *intf)
{
    struct usb_hub *pHub = (struct usb_hub*) ms_usb_get_intfdata (intf);
    U32 flags;

    if (!pHub)
        return;

    osapi_spin_lock_irqsave(&hub_event_lock, flags);
    /* Delete it and then reset it */
    if (!ms_is_empty_list(&pHub->event_list))
    {
        ms_list_remove(&pHub->event_list);
        ms_list_init(&pHub->event_list);
    }
    //ms_list_remove(&pHub->hub_list);
    //ms_list_init(&pHub->hub_list);
    pHub->disconnected = 1;
    osapi_spin_unlock_irqrestore(&hub_event_lock, flags);

    /* All children disconnect  and cease the hub */
    pHub->error = 0;
    //ms_hub_quiesce(pHub, HUB_DISCONNECT);

    ms_usb_set_intfdata (intf, NULL);

    if (pHub->urb)
    {
        ms_usbhost_debug("hub_disconnect: unlink urb %p\n", pHub->urb);
        //ms_usb_unlink_urb(pHub->urb); // patch from Linux 2.6.28
        ms_usb_free_urb(pHub->urb);
        pHub->urb = NULL;
    }

    if (pHub->buffer)
    {
        ms_usb_buffer_free(interface_to_usbdev(intf),
            sizeof(*pHub->buffer), pHub->buffer, pHub->buffer_dma);
        pHub->buffer = NULL;
    }

    kfree(pHub);
}

/**
     * @brief               hub driver probe function
     *
     * @param           struct usb_interface *intf
     * @param           const struct usb_device_id *id
     *
     * @return          error code
     */
static int ms_hub_probe(struct usb_interface *pIntf, const struct usb_device_id *id)
{
    struct usb_host_interface *pHIdesc;
    struct usb_endpoint_descriptor *pEndpoint;
    struct usb_device *pUdev;
    struct usb_hub *pHub;
    //U32 flags;

    pHIdesc = pIntf->altsetting + pIntf->act_altsetting;
    pUdev = interface_to_usbdev(pIntf);

    if (pUdev->level == MAX_HUB_TOPO_LEVEL)
    {
        ms_usbhost_msg("hub nested too deep, level = %d\n", MAX_HUB_TOPO_LEVEL);
        return -MS_ERR_2BIG;
    }

    if ((pHIdesc->desc.bInterfaceSubClass != 0) && (pHIdesc->desc.bInterfaceSubClass != 1))
    {
descriptor_error:
        ms_usbhost_err("bad descriptor, ignoring hub\n");
        return -EIO;
    }

    if (pHIdesc->desc.bNumEndpoints != 1)
    {
        goto descriptor_error;
    }

    pEndpoint = &pHIdesc->endpoint[0].desc;

    // Determine hub endpoint attribute as interrupt IN
    if (!(pEndpoint->bEndpointAddress & USB_DIR_IN))
    {
        goto descriptor_error;
    }
    if ((pEndpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
        != USB_ENDPOINT_XFER_INT)
    {
        goto descriptor_error;
    }

    ms_usbhost_msg ("USB hub found\n");

    pHub = (struct usb_hub*) kmalloc(sizeof(*pHub), GFP_KERNEL);
    if (!pHub)
    {
        ms_usbhost_err("couldn't kmalloc hub struct\n");
        return -ENOMEM;
    }

    memset(pHub, 0, sizeof(*pHub));

    ms_list_init(&pHub->event_list);
    pHub->intf = pIntf;

    //osapi_spin_lock_irqsave(&hub_event_lock, flags);
    //ms_list_init(&pHub->hub_list);
    //ms_insert_list_after(&pHub->hub_list, &hub_list);
    //osapi_spin_unlock_irqrestore(&hub_event_lock, flags);

    ms_usb_set_intfdata (pIntf, pHub);

    if (ms_hub_configure(pHub, pEndpoint) >= 0)
        return 0;

    ms_hub_disconnect (pIntf);
    return -ENODEV;
}

/**
     * @brief               hub reset according to hub tree
     *
     * @param           struct usb_hub *hub
     *
     * @return          error code
     */
int ms_hub_reset(struct usb_hub *hub)
{
    struct usb_device *ms_dev = interface_to_usbdev(hub->intf);
    int i;

    ms_usbhost_msg("\nhub reset\n");
    for (i = 0; i < hub->descriptor.bNbrPorts; i++)
    {
        if (ms_dev->children[i])
            ms_usb_disconnect(&ms_dev->children[i]);
    }

    if (hub->urb)
    {
        ms_usb_unlink_urb(hub->urb);
        ms_usbhost_msg("hub_reset: unlink urb\n");
    }
    else
        return -1;

    if (ms_usb_reset_device(ms_dev))
        return -1;

    hub->urb->dev = ms_dev;
    if (ms_usb_submit_urb(hub->urb, GFP_KERNEL))
        return -1;

    ms_hub_power_on(hub);

    return 0;
}

/**
     * @brief               hub disconnect by hub tree
     *
     * @param           struct usb_device *ms_dev
     *
     * @return          none
     */
void ms_hub_start_disconnect(struct usb_device *ms_dev)
{
    struct usb_device *ms_parent = ms_dev->parent;
    int i;

    if (ms_parent)
    {
        for (i = 0; i < ms_parent->u32MaxChild; i++)
        {
            if (ms_parent->children[i] == ms_dev)
            {
                ms_usb_disconnect(&ms_parent->children[i]);
                return;
            }
        }
    }

    ms_usbhost_err("cannot disconnect hub %s\n", ms_dev->devpath);
}

/**
     * @brief               retrieving hub port current condition
     *
     * @param           struct usb_device *dev
     * @param           int port
     * @param           U16 *status_r
     * @param           U16 *change_r
     *
     * @return          error code
     */
int ms_hub_port_status(struct usb_device *ms_dev, int port,
             U16 *status_r, U16 *change_r)
{
    struct usb_hub *hub = (struct usb_hub*) ms_usb_get_intfdata(ms_dev->actconfig->interface[0]);
    S32 s32Err;

    // ms_get_port_status
    s32Err = ms_usb_control_cmd(ms_dev, usb_rcvctrlpipe(ms_dev, 0),
        USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_PORT, 0, port+1,
        &hub->status.port, sizeof(hub->status.port), HZ * USB_CTRL_GET_TIMEOUT);
    if (s32Err < 0)
    {
        ms_usbhost_err("%s failed (err = %d)\n", __FUNCTION__, (int)s32Err);
    }
    else
    {
        *status_r = hub->status.port.wPortStatus;
        *change_r = hub->status.port.wPortChange;
        s32Err = 0;
    }
    return s32Err;
}

/**
     * @brief               hub port reset waiting function
     *
     * @param           struct usb_device *hub
     * @param           int port
     * @param           struct usb_device *ms_dev
     * @param           U32 delay
     *
     * @return          error code, -1 on error, 0 on success, 1 on disconnect.
     */
static int ms_hub_port_wait_reset(struct usb_device *hub, int port,
        struct usb_device *ms_dev, U32 delay)
{
    S32 s32Dtime, s32Ret;
    U16 u16Portstatus;
    U16 u16Portchange;

    for (s32Dtime = 0; s32Dtime < HUB_RESET_TIMEOUT; s32Dtime += delay)
    {
        wait_ms(delay);

        s32Ret = ms_hub_port_status(hub, port, &u16Portstatus, &u16Portchange);
        if (s32Ret < 0) {
            if (s32Ret == -ENODEV)
                return 1;
            else
                return -1;
        }

        if (!(u16Portstatus & USB_PORT_STAT_CONNECTION))
            return 1;

        if ((u16Portchange & USB_PORT_STAT_C_CONNECTION))
            //return -1;
            return 1; // disconnect

        if (!(u16Portstatus & USB_PORT_STAT_RESET) &&
            (u16Portstatus & USB_PORT_STAT_ENABLE))
        {
            if (u16Portstatus & USB_PORT_STAT_HIGH_SPEED)
                ms_dev->eSpeed = USB_HIGH_SPEED;
            else if (u16Portstatus & USB_PORT_STAT_LOW_SPEED)
                ms_dev->eSpeed = USB_LOW_SPEED;
            else
                ms_dev->eSpeed = USB_FULL_SPEED;
            ms_usbhost_msg("::Device Speed is %s\n", ms_portspeed(u16Portstatus));
            return 0;
        }

        if (s32Dtime >= 2 * HUB_SHORT_RESET_TIME)
            delay = HUB_LONG_RESET_TIME;

        ms_usbhost_debug ("port %d not reset yet, waiting %dms\n", port + 1, delay);
    }

    return -1;
}

/**
     * @brief               hub port reset command
     *
     * @param           struct usb_device *hub
     * @param           int port
     * @param           struct usb_device *ms_dev
     * @param           U32 delay
     *
     * @return          error code
     */
static int ms_hub_port_reset(struct usb_device *hub, int port,
        struct usb_device *ms_dev, U32 delay)
{
    int i, status;

    /* patch for DM always keep high issue */
    #if (_USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH)
    /* turn off overwrite mode */
    if (hub->parent == NULL)
    {
      struct usb_hcd *hcd = (struct usb_hcd*) hub->bus->hcpriv;
      struct cpe_dev *dev;
      const struct device_s *__mptr = hcd->controller;
      dev = (struct cpe_dev *)( (char *)__mptr - offsetof(struct cpe_dev,dev) );
      U32 regUTMI = dev->utmibase;

      usb_writeb(usb_readb((void*)(regUTMI+0x0*2)) & ~BIT1, (void*) (regUTMI+0x0*2)); //tern_ov = 0
    }
    #endif

    for (i = 0; i < HUB_RESET_TRIES; i++)
    {
        // (USB_PORT_FEAT_RESET)
        ms_usb_control_cmd(hub, usb_sndctrlpipe(hub, 0),
            USB_REQ_SET_FEATURE, USB_RT_PORT, USB_PORT_FEAT_RESET, port+1, NULL, 0, HZ);

        status = ms_hub_port_wait_reset(hub, port, ms_dev, delay);
        ms_usbhost_debug("<hub_port_reset> loop %d, status %d\n", i, status);
        if (status != -1)
        {
            // (USB_PORT_FEAT_C_RESET)
            ms_usb_control_cmd(hub, usb_sndctrlpipe(hub, 0),
                USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_RESET, port+1, NULL, 0, HZ);
            // status: 0 on success, 1 on disconnect
            //ms_dev->eState = status ? USB_STATE_NOTATTACHED : USB_STATE_DEFAULT;
            ms_usb_set_device_state(ms_dev, status ? USB_STATE_NOTATTACHED : USB_STATE_DEFAULT);
            return status;
        }

        ms_usbhost_debug("port %d not enabled, trying reset again...\n", port + 1);
        delay = HUB_LONG_RESET_TIME; // increase reset waiting time
    }

    ms_usbhost_err("Cannot enable port %d.  Maybe the USB cable is bad?\n", port + 1);

    return -1;
}

/**
     * @brief               hub port disable command
     *
     * @param           struct usb_device *hub
     * @param           int port
     *
     * @return          error code
     */
int ms_hub_port_disable(struct usb_device *hub, int port)
{
    S32 s32Err;

    // (USB_PORT_FEAT_ENABLE)
    s32Err = ms_usb_control_cmd(hub, usb_sndctrlpipe(hub, 0),
        USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_ENABLE, port+1, NULL, 0, HZ);
    if (s32Err)
    {
        ms_usbhost_err("cannot disable port %d (err = %d)\n", port + 1, (int)s32Err);
    }

    return s32Err;
}

static void ms_recursively_NOTATTACHED(struct usb_device *udev)
{
	int i;

	for (i = 0; i < udev->u32MaxChild; ++i) {
		if (udev->children[i])
			ms_recursively_NOTATTACHED(udev->children[i]);
	}
	udev->eState = USB_STATE_NOTATTACHED;
}

void ms_usb_set_device_state(struct usb_device *udev,
    enum usb_device_state new_state)
{
	unsigned long flags;

	osapi_spin_lock_irqsave(&device_state_lock, flags);
	if (udev->eState == USB_STATE_NOTATTACHED)
		;	/* no action */
	else if (new_state != USB_STATE_NOTATTACHED) {
		udev->eState = new_state;
	} else
		ms_recursively_NOTATTACHED(udev);
	osapi_spin_unlock_irqrestore(&device_state_lock, flags);
}

#define HUB_DEBOUNCE_TIMEOUT  1500 //400
//#define HUB_DEBOUNCE_STEP      25
//#define HUB_DEBOUNCE_STABLE   4 // # of counts
/**
     * @brief               hub port deboucing
     *
     * @param           struct usb_device *hub
     * @param           int port
     *
     * @return          error code
     */
static int ms_hub_port_debounce(struct usb_device *hub, int port)
{
    S32 s32Err;
    S32 s32Dtime, s32Stable_count;
    U16 u16Portchange, u16Portstatus;
    U32 u32Connection;

    u32Connection = 0;
    s32Stable_count = 0;
    for (s32Dtime = 0; s32Dtime < HUB_DEBOUNCE_TIMEOUT; s32Dtime += HUB_DEBOUNCE_STEP)
    {
        wait_ms(HUB_DEBOUNCE_STEP);

        s32Err = ms_hub_port_status(hub, port, &u16Portstatus, &u16Portchange);
        if (s32Err < 0)
            return -1;

        if ((U16) (u16Portstatus & USB_PORT_STAT_CONNECTION) == u32Connection)
        {
            if (u32Connection)
            {
                if (++s32Stable_count == HUB_DEBOUNCE_STABLE)
                    break;
            }
        }
        else
        {
            s32Stable_count = 0;
        }
        u32Connection = u16Portstatus & USB_PORT_STAT_CONNECTION;

        if ((u16Portchange & USB_PORT_STAT_C_CONNECTION))
        {
            // (USB_PORT_FEAT_C_CONNECTION)
            ms_usb_control_cmd(hub, usb_sndctrlpipe(hub, 0),
                USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_CONNECTION, port+1, NULL, 0, HZ);
        }
    }

    ms_usbhost_debug("debounce: port %d: delay %dms stable %d status 0x%x\n",
        port + 1, s32Dtime, s32Stable_count, u16Portstatus);

    return ((u16Portstatus & USB_PORT_STAT_CONNECTION)) ? 0 : 1;
}

/**
     * @brief           hub port connect change process
     *
     * @param           struct usb_hub *hubstate
     * @param           int port
     * @param           U16 portstatus
     * @param           U16 portchange
     *
     * @return          none
     */
static struct usb_device* ms_hub_port_connect_change(
	struct usb_hub *hubstate, int port, U16 portstatus, U16 portchange)
{
    struct usb_device *pHub = interface_to_usbdev(hubstate->intf);
    struct usb_device *pUdev;
    //struct s_hcd_dev    *phcd_dev;
    struct usb_hcd    *pHcd = (struct usb_hcd*) pHub->bus->hcpriv;
    U32 u32Delay = HUB_SHORT_RESET_TIME;
    int i, sErr = 0;

    ms_usbhost_msg("hub_port_connect_change ===%s===>\t",
        ECOS_USB_HOST_LOCAL_VER);
    ms_usbhost_msg("port %d, status %x, change %x\n",
        port+1, portstatus, portchange);

    /* patch for DM always keep high issue */
    #if (_USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH)
    /* turn on overwrite mode */
    if (pHub->parent == NULL)
    {
        struct cpe_dev *dev;
        const struct device_s *__mptr = pHcd->controller;
        dev = (struct cpe_dev *)( (char *)__mptr - offsetof(struct cpe_dev,dev) );
        U32 regUTMI = dev->utmibase;

        usb_writeb(usb_readb((void*)(regUTMI+0x0*2)) | BIT1, (void*) (regUTMI+0x0*2)); //tern_ov = 1
    }
    #endif

    /* move back to hub_events() */
    // (USB_PORT_FEAT_C_CONNECTION)
    //ms_usb_control_cmd(pHub, usb_sndctrlpipe(pHub, 0),
    //    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_CONNECTION, port+1, NULL, 0, HZ);

    if (pHub->children[port])
        ms_usb_disconnect(&pHub->children[port]);

    if (!(portstatus & USB_PORT_STAT_CONNECTION))
    {
        if (portstatus & USB_PORT_STAT_ENABLE)
            ms_hub_port_disable(pHub, port);
        /* deal with HC abnormal state in fast plug in/out */
        else if (pHub->parent == NULL) // limited at roothub
            ms_roothub_disconn_reinit(pHcd);
        return NULL;
    }

    if (ms_hub_port_debounce(pHub, port))
    {
        ms_usbhost_err("connect-debounce failed, port %d disabled\n",
            port+1);
        ms_hub_port_disable(pHub, port);
        return NULL;
    }

    if (!pHub->parent)
        u32Delay = HUB_ROOT_RESET_TIME;

    if (portstatus & USB_PORT_STAT_LOW_SPEED)
        u32Delay = HUB_LONG_RESET_TIME;

  for (i = 0; i < HUB_PROBE_TRIES; i++) {
    struct usb_device *pdev;
    int  len;

    pUdev = ms_usb_alloc_dev(pHub, pHub->bus);
    if (!pUdev) {
      ms_usbhost_err("couldn't allocate usb_device\n");
      break;
    }

    pHub->children[port] = pUdev;
    //pUdev->eState = USB_STATE_POWERED;
    ms_usb_set_device_state(pUdev, USB_STATE_POWERED); // patch from Linux 2.6.28

    ms_usbhost_debug("pHcd->product_desc: (%s)\n", pHcd->product_desc);
    if ( (pHub->parent == NULL) && (pHcd->isBadDevice) )
    {
        if (pHcd->badDeviceCnt == 3)
            ms_RH_force_FSmode(pHcd, 1); //force enter FSmode
    }
    pHcd->isBadDevice = FALSE;

    if (pHub->parent == NULL && (pHcd->isBadDeviceRH == TRUE))
    {
        diag_printf("[USB] clear Bad Device on RH\n");
        pHcd->isBadDeviceRH = FALSE;
    }

    if (ms_hub_port_reset(pHub, port, pUdev, u32Delay)) {
        ms_usb_put_dev(pUdev);
        continue;
    }

    // device unexpected disconnected checking
    if (pHub->parent == NULL)
    {
        //struct ehci_hcd *ehci = hcd_to_ehci (pHcd);
        //U32 temp = hcd_reg_readl((U32)&ehci->op_regs->portsc[0]);

        //if (!(temp & PORTSC_CONNECT))
        if (!ms_RoothubPortConnected(pHcd))
        {
            diag_printf("[USB] no connect after bus reset\n");
            ms_usb_put_dev(pUdev);
            break;
        }
    }

    pUdev->level = pHub->level + 1;

    ms_usb_choose_address(pUdev);

    if (pHub->tt) {
      pUdev->tt = pHub->tt;
      pUdev->u32TTPort = pHub->u32TTPort;
    }
    else if (pUdev->eSpeed != USB_HIGH_SPEED
        /* MStar patch to setup roothub tt record */
        && (pHub->parent == NULL || pHub->eSpeed == USB_HIGH_SPEED)) {
        //&& pHub->eSpeed == USB_HIGH_SPEED) {
      pUdev->tt = &hubstate->tt;
      pUdev->u32TTPort = port + 1;
    }

    pdev = pUdev->parent;
    if (pdev->devpath [0] != '0')
      len = USB_sprintf (pUdev->devpath,"%s.%d", pdev->devpath, port + 1);
    else
      len = USB_sprintf (pUdev->devpath,"%d", port + 1);
    if (len == sizeof pUdev->devpath) {
      ms_usbhost_err("devpath size! usb/%03d/%03d path %s\n",
        (int)pUdev->bus->busnum, (int)pUdev->u32DevNum, pUdev->devpath);
    }

    ms_usbhost_msg("new USB device on path usb:%s, assigned address %d\n",
      pUdev->devpath, (int)pUdev->u32DevNum);

    pUdev->dev.parent = pUdev->parent->dev.parent->parent;

    if (!(sErr = ms_usb_new_device(pUdev, &pHub->dev)))
      goto done;

    ms_usbhost_msg("[HPCC] usb new device failed tier(%d)\n", i);
    ms_hub_port_disable(pHub, port); // patch from Linux code

    /* Clean qh buffer through urb list of udev */
    //ms_usb_disable_endpoint(pUdev, 0);
    ms_usb_disable_endpoint(pUdev, 0+USB_DIR_OUT);
    ms_usb_disable_endpoint(pUdev, 0+USB_DIR_IN);
    //phcd_dev = (struct s_hcd_dev *)pUdev->hcpriv;
    //ms_ehci_disable_ep(pHcd, phcd_dev, 0);

    ms_usb_put_dev(pUdev);

    if (sErr == -ESHUTDOWN || sErr == -ENODEV)
    {
        ms_usbhost_err("[HPCC] sErr %s!\n",
		(sErr == -ENODEV) ? "ENODEV" : "ESHUTDOWN");
        break;
    }
    u32Delay = HUB_LONG_RESET_TIME; // increase hub reset period
  }


  diag_printf("HUB_PROBE_TRIES fail, #%d\n", i);
  if (i >= HUB_PROBE_TRIES && pHub->parent == NULL) // 3 reset retries
  {
    ms_usbhost_err("HUB PROBE fail, set bad device on root hub!\n");
    pHcd->isBadDeviceRH = TRUE;
  }
  pHub->children[port] = NULL;
  ms_hub_port_disable(pHub, port);
  pUdev = NULL;
done:
  osapi_up(&usb_address0_sem);
  return pUdev;
}

static struct usb_device_id hub_id_table [] = {
    { USBDEV_MATCH_ID_DEV_CLASS,0,0,0,0,USB_CLASS_HUB,0,0,0,0,0,0},
    { USBDEV_MATCH_ID_INT_CLASS,0,0,0,0,0,0,0,USB_CLASS_HUB,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0,0,0}

};

static struct usb_driver hub_driver = {
    "hub",
    ms_hub_probe,
    ms_hub_disconnect,
    hub_id_table
};

/**
     * @brief               hub driver installation
     *
     * @param           none
     *
     * @return          error code
     */
int ms_usb_hub_init(void)
{
    if (ms_usb_register(&hub_driver) < 0)
    {
        ms_usbhost_err("Unable to register USB hub driver\n");
        return -1;
    }
    // Start up hub thread here (if platform has OS support)

    return 0;
}

int ms_usb_hub_uninit(void)
{
    ms_usb_deregister(&hub_driver);
    return 0;
}
/**
     * @brief               usb reset function
     *
     * @param           struct usb_device *ms_dev
     *
     * @return          error code
     */
int ms_usb_physical_reset_device(struct usb_device *ms_dev)
{
    struct usb_device *parent = ms_dev->parent;
    struct usb_device_descriptor *d_desc;
    S32 i, s32Err, port = -1;

    if (!parent)
    {
        ms_usbhost_err("attempting to reset root hub!\n");
        return -EINVAL;
    }

    for (i = 0; i < parent->u32MaxChild; i++)
        if (parent->children[i] == ms_dev)
        {
            port = i;
            break;
        }

    if (port < 0)
        return -ENOENT;

    d_desc = (struct usb_device_descriptor*) kmalloc(sizeof *d_desc, GFP_NOIO);
    if (!d_desc)
    {
        ms_usbhost_err("[physical reset device] No available memory!\n");
        return -ENOMEM;
    }

    s32Err = ms_hub_port_reset(parent, port, ms_dev, HUB_SHORT_RESET_TIME);
    if (s32Err)
    {
        ms_usbhost_err("hub port[%d] reset fail, to disable the port (error=%d)\n", (int)port, (int)s32Err);
        ms_hub_port_disable(parent, port);
        kfree(d_desc);
        return -ENODEV;
    }

    diag_printf("Re-enumerating...\n");
    s32Err = ms_usb_set_address(ms_dev);
    if (s32Err < 0)
    {
        ms_usbhost_err("USB device not accepting new address (error=%d)\n", (int)s32Err);
        ms_hub_port_disable(parent, port);
        kfree(d_desc);
        /* prevent usb_port_reset to be submitted */
        if (s32Err == -ETIMEDOUT)
            s32Err = -ENODEV;
        return s32Err;
    }

    wait_ms(10);

    s32Err = ms_usb_get_descriptor(ms_dev, USB_DT_DEVICE, 0, d_desc, sizeof(*d_desc));
    if (s32Err < 0)
    {
        ms_usbhost_err("USB device not getting device descriptor (error=%d)\n", (int)s32Err);
        kfree(d_desc);
        return s32Err;
    }

    /* no need to do endien transform */
    //d_desc->bcdUSB = d_desc->bcdUSB;
    //d_desc->idVendor = d_desc->idVendor;
    //d_desc->idProduct = d_desc->idProduct;
    //d_desc->bcdDevice = d_desc->bcdDevice;

    /* this part is for re-enumeration, descriptor changed */
    if (memcmp(&ms_dev->descriptor, d_desc, sizeof(*d_desc)))
    {
        kfree(d_desc);
        ms_usb_destroy_config(ms_dev);

        s32Err = ms_usb_get_dev_descriptor(ms_dev);
        if ((U32) s32Err < sizeof(ms_dev->descriptor))
        {
            if (s32Err < 0)
            {
                ms_usbhost_err("unable to get device %s descriptor (error=%d)\n",
                    ms_dev->devpath, (int)s32Err);
            }
            else
                ms_usbhost_msg("USB device %s descriptor short read "
                    "(expected %i, got %i)\n",
                    ms_dev->devpath, sizeof(ms_dev->descriptor), (int)s32Err);

            //ms_clear_bit(ms_dev->u32DevNum, ms_dev->bus->devmap.usb_devicemap,U32);
            ms_devmap_clear_bit(ms_dev->u32DevNum, ms_dev->bus->devmap.usb_devicemap);
            ms_dev->u32DevNum = -1;
            return -EIO;
        }

        s32Err = ms_usb_get_config(ms_dev);
        if (s32Err < 0)
        {
            ms_usbhost_err("unable to get configuration (error=%d)\n", (int)s32Err);
            ms_usb_destroy_config(ms_dev);
            //ms_clear_bit(ms_dev->u32DevNum, ms_dev->bus->devmap.usb_devicemap,U32);
            ms_devmap_clear_bit(ms_dev->u32DevNum, ms_dev->bus->devmap.usb_devicemap);
            ms_dev->u32DevNum = -1;
            return 1;
        }

        //if(ms_dev->descriptor.bDeviceClass == USB_CLASS_COMM)
        //    ms_usb_set_config(ms_dev, ms_dev->config[1].desc.bConfigurationValue);
        //else
            ms_usb_set_config(ms_dev, ms_dev->config[0].desc.bConfigurationValue);
        return 1;
    }

    kfree(d_desc);

    s32Err = ms_usb_set_config(ms_dev, ms_dev->actconfig->desc.bConfigurationValue);
    diag_printf("usb_physical_reset_device done\n");
    return 0;
}

/**
     * @brief               usb reset function
     *
     * @param           struct usb_device *ms_dev
     *
     * @return          error code
     */
int ms_usb_reset_device(struct usb_device *ms_dev)
{
    int retval;

    ms_usbhost_debug("ms_usb_reset_device\n");

	if (ms_dev->eState == USB_STATE_NOTATTACHED ||
			ms_dev->eState == USB_STATE_SUSPENDED) {
		ms_usbhost_err("device reset not allowed in state %d\n",
				ms_dev->eState);
		return -EINVAL;
	}

    /* unbind interface */

    retval = ms_usb_physical_reset_device(ms_dev);

    /* rebind interface */

    return retval;
}

/**
     * @brief               hub events process
     *
     * @param           struct s_gVar4UsbPort *pRootHub
     *
     * @return          none
     */
static int ms_hub_events(struct s_gVar4UsbPort *pRootHub)
{
    U32 flags;
    struct list_head *tmp_list = &pRootHub->hub_event;
    struct list_head *list_t;
    struct usb_device *ms_dev;
    struct usb_hub *hub;
    U16 hubstatus;
    U16 hubchange;
    U16 u16Portstatus;
    U16 u16Portchange;
    S32 i, s32Err;
    S32 isHubConnEvent = 0;

    while (1)
    {
        osapi_spin_lock_irqsave(&hub_event_lock, flags);
        if (ms_is_empty_list(tmp_list))
        {
            osapi_spin_unlock_irqrestore(&hub_event_lock, flags);
            break;
        }

        list_t = tmp_list->next;
        hub = entry_to_container(list_t, struct usb_hub, event_list);
        ms_dev = interface_to_usbdev(hub->intf);
        ms_list_remove_and_init(list_t);
        osapi_spin_unlock_irqrestore(&hub_event_lock, flags);

        if (hub->error)
        {
            ms_usbhost_debug ("resetting for error %d\n", hub->error);

            if (ms_hub_reset(hub))
            {
                ms_usbhost_err("can't reset; disconnecting\n");
                ms_hub_start_disconnect(ms_dev);
                continue;
            }

            hub->nerrors = 0;
            hub->error = 0;
        }

        ms_usbhost_debug("**EX** get hub event:%s at port %d\n",
                ms_dev->devpath, pRootHub->hostid);

        if (ms_dev->parent == NULL)
            ms_usbhost_debug("<saved irq> portsc[0x%4x], usbcmd[%x]\n",
                pRootHub->p_UsbHcd->saved_ehci_state.iPsc,
                pRootHub->p_UsbHcd->saved_ehci_state.usbcmd);

        for (i = 0; i < hub->descriptor.bNbrPorts; i++)
        {
            int connect_change = 0;

            s32Err = ms_hub_port_status(ms_dev, i, &u16Portstatus, &u16Portchange);
            if ((ms_dev->parent == NULL) &&
                (pRootHub->p_UsbHcd->saved_ehci_state.iPsc & 0x2) &&
                ((u16Portchange & USB_PORT_STAT_C_CONNECTION)==0) &&
                ((u16Portstatus & USB_PORT_STAT_CONNECTION)==0))
            {
                diag_printf("current u16Portchange [0x%4x], u16Portstatus [0x%4x] s32Err %d\n", u16Portchange, u16Portstatus, s32Err);
                u16Portchange |= USB_PORT_STAT_C_CONNECTION;
            }
            if (s32Err < 0)
            {
                continue;
            }
            if (u16Portchange & USB_PORT_STAT_C_CONNECTION)
            {
                // (USB_PORT_FEAT_C_CONNECTION)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_CONNECTION, i + 1, NULL, 0, HZ);
                connect_change = 1;
            }
            if (u16Portchange & USB_PORT_STAT_C_ENABLE)
            {
                ms_usbhost_debug ("port %d enable change, status %x\n", i + 1, u16Portstatus);
                // (USB_PORT_FEAT_C_ENABLE)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_ENABLE, i + 1, NULL, 0, HZ);

                if (!(u16Portstatus & USB_PORT_STAT_ENABLE)
                    && (u16Portstatus & USB_PORT_STAT_CONNECTION)
                    && (ms_dev->children[i]))
                {
                    ms_usbhost_err("port %d disabled by hub (EMI?), re-enabling...\n", (int)i + 1);
                    connect_change = 1;
                }
            }

            if (u16Portchange & USB_PORT_STAT_C_SUSPEND)
            {
                ms_usbhost_debug ("suspend change on port %d\n", i + 1);
                // (USB_PORT_FEAT_C_SUSPEND)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_SUSPEND, i + 1, NULL, 0, HZ);
            }

            if (u16Portchange & USB_PORT_STAT_C_OVERCURRENT)
            {
                ms_usbhost_err("over-current change on port %d\n", (int)i + 1);
                // (USB_PORT_FEAT_C_OVER_CURRENT)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_OVER_CURRENT, i + 1, NULL, 0, HZ);
                ms_hub_power_on(hub);
            }

            if (u16Portchange & USB_PORT_STAT_C_RESET)
            {
                ms_usbhost_debug ("reset change on port %d\n", i + 1);
                // (USB_PORT_FEAT_C_RESET)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_PORT, USB_PORT_FEAT_C_RESET, i + 1, NULL, 0, HZ);
            }
            if (connect_change) {
                struct usb_device *pUdev;

                pUdev = ms_hub_port_connect_change(hub, i, u16Portstatus, u16Portchange);
                if ((pUdev != NULL) && (u16Portstatus & USB_PORT_STAT_CONNECTION))
                {
                    pRootHub->arConnDev[isHubConnEvent].connDev = pUdev;
                    isHubConnEvent++;
                    USB_ASSERT(isHubConnEvent<USB_MAXCHILDREN, "exceed the size of arConnDev!!!!\n");
                }
            }
        } /* end for i */

        if (ms_hub_hub_status(hub, &hubstatus, &hubchange) < 0)
        {
            ms_usbhost_err("get_hub_status failed\n");
        }
        else
        {
            if (hubchange & HUB_CHANGE_LOCAL_POWER)
            {
                ms_usbhost_debug ("power change\n");
                // (C_HUB_LOCAL_POWER)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_HUB, C_HUB_LOCAL_POWER, 0, NULL, 0, HZ);
            }

            if (hubchange & HUB_CHANGE_OVERCURRENT)
            {
                ms_usbhost_debug ("overcurrent change\n");
                // (C_HUB_OVER_CURRENT)
                ms_usb_control_cmd(ms_dev, usb_sndctrlpipe(ms_dev, 0),
                    USB_REQ_CLEAR_FEATURE, USB_RT_HUB, C_HUB_OVER_CURRENT, 0, NULL, 0, HZ);
                wait_ms(500); // to be cool down
                ms_hub_power_on(hub);
            }
        }
    } /* end while (1) */

    return (isHubConnEvent);
}

#ifdef USB_EHCI_TT
static void ms_hub_tt_kevent(struct s_gVar4UsbPort *);
#endif
/**
     * @brief               hub event polling from UsbTask
     *
     * @param           struct s_gVar4UsbPort *pRootHub
     *
     * @return          # of device connected
     */
int ms_hub_poll(struct s_gVar4UsbPort *pRootHub)
{
    S32 isHubConnected = 0;
    struct list_head *tmp_list = &pRootHub->hub_event;

    if (ms_is_empty_list(tmp_list))
        return 0;

    ms_USBCriticalSectionIn(pRootHub->hostid);
    isHubConnected = ms_hub_events(pRootHub);
#ifdef USB_EHCI_TT
    ms_hub_tt_kevent(pRootHub);
#endif
    ms_USBCriticalSectionOut(pRootHub->hostid);
    return (isHubConnected);
}

#ifdef USB_EHCI_TT
/**
     * @brief           high speed hub tt buffer clear
     *
     * @param           struct usb_device *ms_dev
     * @param           int ms_pipe
     *
     * @return          none
     */
void ms_usb_hub_tt_clear_buffer (struct usb_device *ms_dev, int ms_pipe)
{
    struct usb_tt    *utt = ms_dev->tt;
    U32    flags;
    struct usb_tt_clear  *ms_clear;
    struct usb_hcd *hcd = (struct usb_hcd*) ms_dev->bus->hcpriv;

    diag_printf("%s++\n",__FUNCTION__);
    if ((ms_clear = kmalloc (sizeof *ms_clear, GFP_ATOMIC)) == 0)
    {
        ms_usbhost_err("can't allocate CLEAR_TT_BUFFER state for hub at usb-%s-%s\n",
            ms_dev->bus->bus_name, utt->hub->devpath);
        return;
    }

    ms_clear->tt = utt->multi ? ms_dev->u32TTPort : 1;
    ms_clear->devinfo = usb_pipeendpoint (ms_pipe);
    ms_clear->devinfo |= ms_dev->u32DevNum << 4;
    ms_clear->devinfo |= usb_pipecontrol (ms_pipe)
        ? (USB_ENDPOINT_XFER_CONTROL << 11)
        : (USB_ENDPOINT_XFER_BULK << 11);
    if (usb_pipein (ms_pipe))
        ms_clear->devinfo |= 1 << 15;

    osapi_spin_lock_irqsave (&utt->lock, flags);
    ms_insert_list_before (&ms_clear->clear_list, &utt->clear_list);
    //schedule_work (&utt->kevent);
    ms_insert_list_after(&utt->clear_list, &hcd->tt_clear_list); // new, link to the right hcd
    osapi_spin_unlock_irqrestore (&utt->lock, flags);
}

static void ms_hub_tt_kevent(struct s_gVar4UsbPort *pRootHub)
{
	struct list_head	*tmp_list = &pRootHub->p_UsbHcd->tt_clear_list;
	struct list_head	*list_t;
	struct usb_hub		*hub;
	unsigned long		flags;
	int			limit = 100;

	osapi_spin_lock_irqsave (&hub->tt.lock, flags);
	if (ms_is_empty_list(tmp_list)) {
		osapi_spin_unlock_irqrestore (&hub->tt.lock, flags);
		return;
	}
	list_t = tmp_list->next;
	hub = entry_to_container(list_t, struct usb_hub, event_list);
	while (--limit && !list_empty (&hub->tt.clear_list)) {
		struct list_head	*temp;
		struct usb_tt_clear	*clear;
		struct usb_device	*hdev = interface_to_usbdev(hub->intf);
		int			status;

		temp = hub->tt.clear_list.next;
		clear = list_entry (temp, struct usb_tt_clear, clear_list);
		list_del (&clear->clear_list);

		/* drop lock so HCD can concurrently report other TT errors */
		osapi_spin_unlock_irqrestore (&hub->tt.lock, flags);
		/* hub_clear_tt_buffer */
		status = ms_usb_control_cmd(hdev, usb_rcvctrlpipe(hdev, 0),
			       HUB_CLEAR_TT_BUFFER, USB_RT_PORT, clear->devinfo,
			       clear->tt, NULL, 0, 1000);
		osapi_spin_lock_irqsave (&hub->tt.lock, flags);

		if (status)
			ms_usbhost_err ("clear tt %d (%04x) error %d\n",
				clear->tt, clear->devinfo, status);
		kfree(clear);
	}
	osapi_spin_unlock_irqrestore (&hub->tt.lock, flags);
}

#endif

