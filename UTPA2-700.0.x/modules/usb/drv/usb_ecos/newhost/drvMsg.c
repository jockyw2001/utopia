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

#include  "include/drvConfig.h"

#ifdef CONFIG_DEBUG
#define DEBUG
#else
#undef DEBUG
#endif

//#include  "include/drvCompiler.h" // NUSED
//#include  "include/drvErrno.h" // NUSED
//#include  "include/drvPorts.h" // NUSED
//#include  "include/drvPCIMEM.h" // NUSED
//#include  "include/drvTimer.h" // NUSED
//#include  "include/drvList.h" // NUSED

//#include "include/drvUSBHost.h" // NUSED
//#include "drvUsbd.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED
#include "drvEHCI.h"

/**
     * @brief               complete function of URB
     *
     * @param urb
     * @param regs
     *
     * @return          none
     */
static void ms_usb_api_completion(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_api_data *pAwd = (struct usb_api_data *)urb->pContext;

    /* move to ms_usb_hcd_giveback_urb() to support 3rd driver */
    //if (urb->SetDMABuf != NULL)
    //{
    //    //MsOS_FreeMemory(urb->SetDMABuf, gs32NonCachedPoolID_MIU0 );
    //    Usb_FreeNonCachedMemory(urb->SetDMABuf);
    //
    //    urb->SetDMABuf = NULL;
    //}
    //
    //if (urb->TxDMABuf != NULL)
    //{
    //    if (usb_pipein(urb->u32Pipe))
    //    {
    //    #ifdef DEBUG_PERFORMANCE
    //        if (urb->u32TransferBufferLength >= 512)
    //        diag_printf("ms_usb_api_completion: copy length: 0x%lx \n", urb->u32TransferBufferLength);
    //    #endif
    //        memcpy(urb->pTransferBuffer, urb->TxDMABuf, urb->u32TransferBufferLength);
    //    }
    //    //MsOS_FreeMemory(urb->TxDMABuf,gs32NonCachedPoolID_MIU0 );
    //    Usb_FreeNonCachedMemory(urb->TxDMABuf);
    //
    //    urb->TxDMABuf = NULL;
    //}

    pAwd->done = 1;
    wmb();

    wake_up(&pAwd->wqh);
}
#ifdef DEBUG
extern BOOL ms_isHcdRunning(struct usb_hcd *hcd); // debug
extern void ms_dumpHcdRegister(struct usb_hcd *hcd); // debug
#endif
/**
     * @brief               Start the URB and wait until it complete
     *
     * @param urb
     * @param timeout
     * @param act_len
     *
     * @return          function status
     */
static int ms_usb_start_wait_urb(struct urb *urb, int timeout, int* act_len)
{
    struct usb_api_data stAwd;
    struct usb_hcd *hcd = urb->dev->bus->hcpriv;
    struct ehci_hcd *ehci = hcd_to_ehci (hcd);
    int status;
    int rh_csc_shutdown = 0;
    U32 uDelay=0;

    stAwd.done = 0;

    urb->pContext = &stAwd;
    status = ms_usb_submit_urb(urb, GFP_ATOMIC);
    if (status)
    {
        ms_usb_free_urb(urb);
        return status;
    }

    while ( timeout && (stAwd.done == 0) )
    {
        /* to speed up one WIFI dongle with internal HUB */
        //if((urb->dev->parent != NULL) && (urb->dev->parent->parent == NULL) // device on roothub
        if ((hcd_reg_readb((U32)&ehci->op_regs->portsc[0]) & PORTSC_CSC)  // CSC happen on ehci port
            || !(hcd_reg_readb((U32)&ehci->op_regs->portsc[0]) & PORTSC_PE)) // PE cleared on ehci port
        {
            timeout = 0;
            rh_csc_shutdown = 1;
            diag_printf("[USB] CSC happend/PE clr while wait CMP PSC [0x%x]\n", hcd_reg_readb((U32)&ehci->op_regs->portsc[0]));
            break;
        }

        if ( uDelay > (U32) timeout)
        {
            timeout =0;
            break;
        }
        else
        {
            mdelay(1);//tick base is 1 ms
            //HAL_DELAY_US(1000); // NUSED
            uDelay+=1;
        }
        rmb();
    }

    if (!timeout && !stAwd.done)
    {
        diag_printf("usb_control/bulk_msg: timeout... (status=%d)\n", (int)urb->s32Status);
        if (urb->s32Status != -EINPROGRESS)
        {
            status = urb->s32Status;
        }
        else
        {
            //int retv;

            //diag_printf("<%s> set Bad Device............\n", __FUNCTION__);
#ifdef DEBUG
            ms_dumpHcdRegister(hcd);
            if (ms_isHcdRunning(hcd)) // just for debugging
                diag_printf("HCD RUN!!!\n");
#endif
            hcd->isBadDevice = TRUE;
            hcd->badDeviceCnt++;

            /* Linux kernel put 5 seconds wait time for control commands,  we put 3 seconds here */
            diag_printf("timeout unlink(kill) urb(qh:%p)\n", urb->hcpriv);
            ms_hcd_unlink_urb(urb, -ENOENT);
            //if ((retv = ms_usb_unlink_urb(urb)) == -ETIMEDOUT)
            //{
            //    diag_printf("timeout retry %d\n", retv);
            //    retv = ms_usb_unlink_urb(urb);
            //}
            //diag_printf("timeout unlink urb(%p) error %d\n", urb, retv);

            status = rh_csc_shutdown ? -ESHUTDOWN : -ETIMEDOUT;
        }
    }
    else
        status = urb->s32Status;

    if (act_len)
        *act_len = urb->u32ActualLength;

    ms_usb_free_urb(urb);
    return status;
}

/**
     * @brief               Send a control setup command
     *
     * @param pDev
     * @param pipe
     * @param u8Reg
     * @param urReg_type
     * @param u16Val
     * @param u16Index
     * @param data
     * @param u16Size
     * @param timeout
     *
     * @return          function status or actual length
     */
int ms_usb_control_cmd(
        struct usb_device *pDev,
        unsigned int pipe,
        unsigned char u8Req,
        unsigned char u8Req_type,
        unsigned short u16Val,
        unsigned short u16Index,
        void *data,
        unsigned short u16Size,
        int timeout)
{
    struct usb_ctrlrequest *dr = (struct usb_ctrlrequest*) kmalloc(sizeof(struct usb_ctrlrequest), GFP_NOIO);
    struct urb *pUrb;
    int ret;
    int length = 0;
    void *addr_t;

    if (!dr)
        return -ENOMEM;

    addr_t = (void *)dr;
    dr->bRequestType= u8Req_type;
    dr->bRequest = u8Req;
    dr->wValue = u16Val;
    dr->wIndex = u16Index;
    dr->wLength = u16Size;

    pUrb = ms_usb_alloc_urb(GFP_NOIO);
    if (!pUrb)
    {
        ret = -ENOMEM;
        goto Func_done;
    }

    ms_usb_stuff_control_urb(pUrb, pDev, pipe, (unsigned char*)dr, data, u16Size,
            ms_usb_api_completion, 0);
    ret = ms_usb_start_wait_urb(pUrb, timeout, &length);
    if (ret < 0)
    {
        ms_debug_err("%s retv:%d, RT:%x,R:%x,V:%x,I:%x,L:%x\n", __FUNCTION__,
            ret, dr->bRequestType, dr->bRequest, dr->wValue, dr->wIndex, dr->wLength);
    }
    else
        ret = length;

Func_done:
    kfree(addr_t);

    return ret;
}

/**
     * @brief               Send a bulk stansaction
     *
     * @param pUsb_dev
     * @param pipe
     * @param data
     * @param len
     * @param actual_length
     * @param timeout
     *
     * @return          function status
     */
#if 0 // NUSED
int ms_usb_bulk_msg(
        struct usb_device *pUsb_dev,
        unsigned int pipe,
        void *data,
        int len,
        int *actual_length,
        int timeout)
{
    struct urb *urb;

    if (len < 0)
        return -EINVAL;

    urb=ms_usb_alloc_urb(GFP_KERNEL);
    if (!urb)
        return -ENOMEM;

    ms_usb_stuff_bulk_urb(urb, pUsb_dev, pipe, data, len,
            ms_usb_api_completion, 0);

    return ms_usb_start_wait_urb(urb,timeout,actual_length);
}
#endif

/**
     * @brief               get descriptor from the device
     *
     * @param pDev
     * @param u8Type
     * @param u8Index
     * @param pBuf
     * @param size
     *
     * @return          function status
     */
int ms_usb_get_descriptor(
        struct usb_device *pDev,
        unsigned char u8Type,
        unsigned char u8Index,
        void *pBuf,
        int size)
{
    int retry = 3;
    int relt = 0;

    memset(pBuf,0,size);

    while (retry--)
    {
        relt = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
                USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
                (u8Type << 8) + u8Index, 0, pBuf, size,
                HZ * USB_CTRL_GET_TIMEOUT);

        if ( (relt > 0) || (relt == -EPIPE) )
            break;
    }

    return relt;
}

/**
     * @brief               get the device descriptor from the device
     *
     * @param pDev
     *
     * @return          function status
     */
int ms_usb_get_dev_descriptor(struct usb_device *pDev)
{

    return ms_usb_get_descriptor(pDev, USB_DT_DEVICE,
            0, &pDev->descriptor,
            sizeof(pDev->descriptor));
}

/**
     * @brief               Clear halt status of the endpoint
     *
     * @param pDev
     * @param pipe
     *
     * @return          function status
     */
int ms_usb_clear_halt(struct usb_device *pDev, int pipe)
{
    int retval;
    int endp;

    if (usb_pipein (pipe))
        endp = usb_pipeendpoint(pipe) | USB_DIR_IN;
    else
        endp = usb_pipeendpoint(pipe);

    retval = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
            USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0, endp, NULL, 0,
            HZ * USB_CTRL_SET_TIMEOUT);

    if (retval < 0)
        return retval;
    else
    {
        usb_settoggle(pDev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);
        //usb_endpoint_running(pDev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

        return 0;
    }

}

/**
     * @brief               disable the endpoint
     *
     * @param pDev
     * @param epaddr
     *
     * @return          none
     */
void ms_usb_disable_endpoint(struct usb_device *pDev, U32 epaddr)
{
    U32 u32Ept = epaddr & USB_ENDPOINT_NUMBER_MASK;

    if (!pDev)
        return;
    //if (pDev && pDev->bus && pDev->bus->bus_ops && pDev->bus->bus_ops->ms_disable_ep)
    {
        if (pDev->pEpMaxPacketIn[u32Ept] || pDev->pEpMaxPacketOut[u32Ept])
        {
            ms_usb_hcd_flush_endpoint(pDev, epaddr);
            /* unbind interface considered not to disable endpoint */
            //pDev->bus->bus_ops->ms_disable_ep(pDev, epaddr);
            ms_hcd_endpoint_disable(pDev, epaddr);
            ms_hcd_check_dev_urb(pDev, epaddr); // check urb
        }
    }
}

/**
     * @brief               disable the interface
     *
     * @param pDev
     * @param pIntf
     *
     * @return          none
     */
void ms_usb_disable_interface(struct usb_device *pDev, struct usb_interface *pIntf)
{
    int EptIdx;
    struct usb_host_interface *hintf = &pIntf->altsetting[pIntf->act_altsetting];

    for (EptIdx = 0; EptIdx < hintf->desc.bNumEndpoints; ++EptIdx)
        ms_usb_disable_endpoint(pDev, hintf->endpoint[EptIdx].desc.bEndpointAddress);
}

/**
     * @brief               disable the device
     *
     * @param pDev
     * @param skip_ep0
     *
     * @return          none
     */
void ms_usb_disable_device(struct usb_device *pDev, int skip_ep0)
{
    int EptIdx, i;

    if (pDev->actconfig)
    {
        for (i = 0; i < pDev->actconfig->desc.bNumInterfaces; i++)
        {
            struct usb_interface  *pUiface;

            /* remove this interface */
            pUiface = pDev->actconfig->interface[i];
            ms_usbhost_debug("%s - unregistering interface %s\n",
                            pUdev->dev.dev_name, pUdev->dev.bus_id);
            ms_device_unregister(&pUiface->dev);
        }
        /* patched from Linux */
        pDev->actconfig = NULL;
        if (pDev->eState == USB_STATE_CONFIGURED)
            ms_usb_set_device_state(pDev, USB_STATE_ADDRESS);
    }

    /* if check_bandwidth, that needs two phases of disable endpoint */
    for (EptIdx = skip_ep0; EptIdx < 16; ++EptIdx)
    {
        ms_usb_disable_endpoint(pDev, EptIdx);
        ms_usb_disable_endpoint(pDev, EptIdx + USB_DIR_IN);
    }
}

/**
     * @brief               enable the endpoint
     *
     * @param pDev
     * @param pEpt_desc
     *
     * @return          none
     */
void ms_usb_enable_endpoint(struct usb_device *pDev,
    struct usb_endpoint_descriptor *pEpt_desc)
{
    unsigned int eptaddr = pEpt_desc->bEndpointAddress;
    unsigned int eptnum = eptaddr & USB_ENDPOINT_NUMBER_MASK;
    int is_control = ((pEpt_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_CONTROL);

    if (usb_endpoint_dir(eptaddr) || is_control)
    {
        //usb_endpoint_running(pDev, eptnum, 1);
        usb_settoggle(pDev, eptnum, 1, 0);
        pDev->pEpMaxPacketOut[eptnum] = pEpt_desc->wMaxPacketSize;
    }

    if (!usb_endpoint_dir(eptaddr) || is_control)
    {
        //usb_endpoint_running(pDev, eptnum, 0);
        usb_settoggle(pDev, eptnum, 0, 0);
        pDev->pEpMaxPacketIn[eptnum] = pEpt_desc->wMaxPacketSize;
    }
}

/**
     * @brief               enable the interface
     *
     * @param pDev
     * @param pIntf
     *
     * @return          none
     */
void ms_usb_enable_interface(struct usb_device *pDev,
    struct usb_interface *pIntf)
{
    struct usb_host_interface *pAlt_set =
            &pIntf->altsetting[pIntf->act_altsetting];
    int i;

    for (i = 0; i < pAlt_set->desc.bNumEndpoints; ++i)
        ms_usb_enable_endpoint(pDev, &pAlt_set->endpoint[i].desc);
}

/**
     * @brief               set specific interface to the device
     *
     * @param pDev
     * @param interface
     * @param alternate
     *
     * @return          function status
     */
int ms_usb_set_interface(struct usb_device *pDev, int interface, int alternate)
{
    struct usb_interface *pIntf;
    int ret;
    int manual = 0;

    pIntf = ms_usb_ifnum_to_if(pDev, interface);
    if (!pIntf)
    {
        ms_usbhost_err("Invalid interface %d", interface);
        return -EINVAL;
    }

    if (alternate < 0 || alternate >= pIntf->num_altsetting)
        return -EINVAL;

    ret = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
                USB_REQ_SET_INTERFACE, USB_RECIP_INTERFACE,
                pIntf->altsetting[alternate]
                .desc.bAlternateSetting,
                interface, NULL, 0, HZ * 5);

    if (ret == -EPIPE && pIntf->num_altsetting == 1)
    {
        ms_debug_msg("manual set interface for pIntf %d, alt %d",
        interface, alternate);
        manual = 1;
    }
    else if (ret < 0)
        return ret;

    ms_usb_disable_interface(pDev, pIntf);

    pIntf->act_altsetting = alternate;

    if (manual)
    {
        struct usb_host_interface *iface_as = &pIntf->altsetting[alternate];
        int ii;

        for (ii = 0; ii < iface_as->desc.bNumEndpoints; ii++)
        {
            unsigned int epaddr = iface_as->endpoint[ii].desc.bEndpointAddress;
            unsigned int pipe =
            __pack_pipe(pDev, USB_ENDPOINT_NUMBER_MASK & epaddr)
            | (usb_endpoint_dir(epaddr) ? USB_DIR_OUT : USB_DIR_IN);

            ms_usb_clear_halt(pDev, pipe);
        }
    }

    ms_usb_enable_interface(pDev, pIntf);

    return 0;
}

/**
     * @brief               set specific configuration to the device
     *
     * @param pDev
     * @param config_val
     *
     * @return          function status
     */
int ms_usb_set_config(struct usb_device *pDev, int config_val)
{
    int i, ret;
    struct usb_host_config *cp = NULL;

    for (i=0; i<pDev->descriptor.bNumConfigurations; i++)
    {
        if (pDev->config[i].desc.bConfigurationValue == config_val)
        {
            cp = &pDev->config[i];
            break;
        }
    }
    if ((!cp && config_val != 0) || (cp && config_val == 0))
    {
        ms_usbhost_err("selecting invalid config_val %d", config_val);
        return -EINVAL;
    }

    if (pDev->eState != USB_STATE_ADDRESS)
        ms_usb_disable_device (pDev, 1);  // Skip ep0
    pDev->toggle[0] = pDev->toggle[1] = 0;
    //pDev->halted[0] = pDev->halted[1] = 0; // Obsolete
    //pDev->eState = USB_STATE_ADDRESS;
    ms_usb_set_device_state(pDev, USB_STATE_ADDRESS);

    if ((ret = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
        USB_REQ_SET_CONFIGURATION, 0, config_val, 0,
        NULL, 0, HZ * USB_CTRL_SET_TIMEOUT)) < 0)
        return ret;
    if (config_val)
        //pDev->eState = USB_STATE_CONFIGURED;
        ms_usb_set_device_state(pDev, USB_STATE_CONFIGURED);
    pDev->actconfig = cp;

    for (i = 0; i < cp->desc.bNumInterfaces; ++i)
    {
        struct usb_interface *intf = cp->interface[i];

        intf->act_altsetting = 0;
        ms_usb_enable_interface(pDev, intf);
    }

    return 0;
}


/**
     * @brief               get the string from the device
     *
     * @param pDev
     * @param index
     * @param pBuf
     * @param size
     *
     * @return          function status
     */
int ms_usb_string(struct usb_device *pDev, int index, char *pBuf, size_t size)
{
    unsigned char *pTmp_buf;
    int err, len;
    U32 u, idx;
    void *addr_t;

    if ( (int)size <= 0 || !pBuf || !index)
        return -EINVAL;

    pBuf[0] = 0;
    pTmp_buf = (unsigned char*) kmalloc(256, GFP_KERNEL);

    if (!pTmp_buf)
        return -ENOMEM;

    addr_t = (void *)pTmp_buf;
    /* get langid for strings if it's not yet known */
    if (!pDev->u32HaveLangId)
    {
        err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
	        USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
	        (USB_DT_STRING << 8), 0, pTmp_buf, 4,
	        HZ * USB_CTRL_GET_TIMEOUT);

        if (err < 0)
        {
            ms_debug_err("error getting string descriptor 0 (error=%d)\n", err);
            goto errout;
        }
        else if (err < 4 || pTmp_buf[0] < 4)
        {
            ms_debug_msg("string descriptor 0 too short!\n");
            err = -EINVAL;
            goto errout;
        }
        else
        {
            pDev->u32HaveLangId = -1;
            pDev->u32StringLangId = pTmp_buf[2] | (pTmp_buf[3]<< 8);
            /* always use the first langid listed */
            ms_debug_debug("USB device number %d default language ID 0x%x\n",
            pDev->u32DevNum, pDev->u32StringLangId);
        }
    }

    err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
            (USB_DT_STRING << 8) + index, pDev->u32StringLangId, pTmp_buf, 2,
            HZ * USB_CTRL_GET_TIMEOUT);

    if(err<2)
        goto errout;
    else if (pTmp_buf[1]!=3)
    {
        err = -EINVAL;
        goto errout;
    }
    len=pTmp_buf[0];

    err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
            (USB_DT_STRING << 8) + index, pDev->u32StringLangId, pTmp_buf, len,
            HZ * USB_CTRL_GET_TIMEOUT);

    if (err < 0)
        goto errout;

    size--;
    for (idx = 0, u = 2; u < (U32) err; u += 2)
    {
        if (idx >= size)
            break;
        if (pTmp_buf[u+1])
            pBuf[idx++] = '?';
        else
            pBuf[idx++] = pTmp_buf[u];
    }
    pBuf[idx] = 0;
    err = idx;

errout:
    kfree(addr_t);
    return err;
}
