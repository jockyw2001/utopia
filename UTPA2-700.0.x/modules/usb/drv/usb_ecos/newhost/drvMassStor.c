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

//=============================================================================
//================include file======================//
//#include <MsCommon.h> // NUSED
#include "include/drvConfig.h"
//#include "include/drvErrno.h" // NUSED
//#include "include/drvUSBHost.h" // NUSED
#include "drvBulktran.h"
#include "include/drvBitops.h"
//#include "include/drvCPE_AMBA.h" // NUSED
//#include "drvUsbHostConfig.h" // NUSED
#include "drvUsbd.h"
//==================================================//

//==================Definition======================//
static int ms_usb_msc_probe(struct usb_interface *iface, const struct usb_device_id *id);
static void ms_usb_msc_disconnect(struct usb_interface *iface);
extern void RemoveUSBDiskPort(U8 uPort, U8 uLunNum);

#define GET_INTERFACE_INFO(cl,sc,pr) \
        USB_DEVICE_ID_MATCH_INT_INFO,0,0,0,0,0,0,0, (cl), (sc), (pr),0
//==================================================//

static struct usb_device_id MSC_usb_ids [] =
{
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_RBC_SC, MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_ATAPI_SC,MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_QIC_SC, MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_UFI_SC, MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SFF_SC,MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SCSI_SC,MS_CB_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_RBC_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_ATAPI_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_QIC_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_UFI_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SFF_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SCSI_SC, MS_CBI_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_RBC_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_ATAPI_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_QIC_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_UFI_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SFF_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, MS_SCSI_SC, MS_BULK_PR) },
    {GET_INTERFACE_INFO (USB_CLASS_STORAGE, 0xFF, MS_CB_PR) },
    {0,0,0,0,0,0,0,0,0,0,0,0}
};

static struct stUnusual_dev MS_USB_unusual_dev_list[] =
{
    { 0,0, MS_RBC_SC, MS_CB_PR,0,0 },
    { 0,0, MS_ATAPI_SC, MS_CB_PR,0,0 },
    { 0,0, MS_QIC_SC, MS_CB_PR,0,0 },
    { 0,0, MS_UFI_SC, MS_CB_PR,0,0 },
    { 0,0, MS_SFF_SC, MS_CB_PR,0,0 },
    { 0,0, MS_SCSI_SC, MS_CB_PR,0,0 },
    { 0,0, MS_RBC_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_ATAPI_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_QIC_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_UFI_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_SFF_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_SCSI_SC, MS_CBI_PR,0,0 },
    { 0,0, MS_RBC_SC, MS_BULK_PR,0,0 },
    { 0,0, MS_ATAPI_SC, MS_BULK_PR,0,0 },
    { 0,0, MS_QIC_SC, MS_BULK_PR,0,0 },
    { 0,0, MS_UFI_SC, MS_BULK_PR,0,0 },
    { 0,0, MS_SFF_SC, MS_BULK_PR,0,0 },
    { 0,0, MS_SCSI_SC, MS_BULK_PR,0,0 },
    { 0,0, 0xFF, MS_CB_PR,0,0 },
    { 0,0,0,0,0,0 }
};

struct usb_driver usb_storage_driver =
{
    "usb-storage",
    ms_usb_msc_probe,
    ms_usb_msc_disconnect,
    MSC_usb_ids
};

static void ms_usb_inquiry_data(struct ms_usdata *ms_us)
{
    U32 data_len=36;
    int len,len1,len2;
    unsigned char data_ptr[36] = {0x00, 0x80, 0x02, 0x02, 0x1F, 0x00, 0x00, 0x00};

    if (ms_test_bit(MS_IDX_DISCONNECTING, &ms_us->flags))
    {
        ms_usbhost_msg("No command allowed during disconnection\n");
        ms_us->srb.result = DID_BAD_TARGET << 16;
    }
    else if (ms_us->srb.sc_data_direction == SCSI_DATA_UNKNOWN)
    {
        ms_usbhost_msg("UNKNOWN data direction\n");
        ms_us->srb.result = DID_ERROR << 16;
    }
    else if ((ms_us->srb.cmnd[0] == INQUIRY) && (ms_us->flags & MS_FIX_INQUIRY))
    {
        ms_usbhost_msg("Faking INQUIRY command\n");
        if(ms_us->srb.req_buf_len > data_len)
            len=data_len;
        else
            len=ms_us->srb.req_buf_len;

        if(data_len >= 36)
        {
            if(data_ptr[0]&0x20)
                memset(data_ptr+8,0,28);
            else
            {
                if(strlen(ms_us->unusual_dev->vendorName) > 8)
                    len1 = 8;
                else
                    len1 = strlen(ms_us->unusual_dev->vendorName);
                memcpy(data_ptr+8, ms_us->unusual_dev->vendorName,len1);

                if(strlen(ms_us->unusual_dev->productName) > 16)
                    len2 = 16;
                else
                    len2 = strlen(ms_us->unusual_dev->productName);
                memcpy(data_ptr+16, ms_us->unusual_dev->productName,len2);

                data_ptr[32] = 0x30 + ((ms_us->pusb_dev->descriptor.bcdDevice>>12) & 0x0F);
                data_ptr[33] = 0x30 + ((ms_us->pusb_dev->descriptor.bcdDevice>>8) & 0x0F);
                data_ptr[34] = 0x30 + ((ms_us->pusb_dev->descriptor.bcdDevice>>4) & 0x0F);
                data_ptr[35] = 0x30 + ((ms_us->pusb_dev->descriptor.bcdDevice) & 0x0F);
            }

            memset(ms_us->srb.req_buf, 0, ms_us->srb.req_buf_len);
            memcpy(ms_us->srb.req_buf, data_ptr, len);
        }

        ms_us->srb.result = MS_STAT_GOOD;
    }
    else
        ms_us->proto_handler(&ms_us->srb, ms_us);
}

int ms_usb_msc_control_thread(void * __us)
{
    struct ms_usdata *ms_us = (struct ms_usdata *)__us;

    ms_usbhost_debug("*** thread awakened.\n");
    //if (ms_us->usb_state == ABORTING_STATE)
    //{
    //    ms_us->srb.result = DID_ABORT << 16;
    //    goto Stop_transfer;
    //}
    //ms_us->usb_state = RUNNING_STATE;
    //osapi_down(&(ms_us->dev_semaphore));
    osapi_mutex_lock(ms_us->dev_mutex);
    ms_usb_inquiry_data(ms_us);
    //osapi_up(&(ms_us->dev_semaphore));    //unlock the device pointers
    osapi_mutex_unlock(ms_us->dev_mutex);

    /* lock access to the state */
    /* indicate that the command is done */
    if (ms_us->srb.result != DID_ABORT << 16)
    {
        ms_usbhost_debug("SCSI cmd done, result=0x%x\n", ms_us->srb.result);
        if(ms_us->srb.scsi_done != NULL)
            ms_us->srb.scsi_done(&ms_us->srb);
    }
    else
    {
//Stop_transfer:
        ms_usbhost_msg("SCSI command aborted\n");
    }
    //if (ms_us->usb_state == ABORTING_STATE)
    //    complete(&(ms_us->notify));
    //ms_us->usb_state = IDLE_STATE;
    //complete(&(ms_us->notify));

    return 0;
}

static int ms_usb_associate_dev(struct ms_usdata *ms_us, struct usb_interface *ms_intf)
{
    ms_us->pusb_dev = interface_to_usbdev(ms_intf);
    ms_us->pusb_intf = ms_intf;
    ms_us->ifnum = ms_intf->altsetting->desc.bInterfaceNumber;

    ms_usb_set_intfdata(ms_intf, ms_us);
    ms_usb_get_dev(ms_us->pusb_dev);
    ms_us->cr = (struct usb_ctrlrequest*) ms_usb_buffer_alloc(ms_us->pusb_dev, sizeof(*ms_us->cr), GFP_KERNEL, &ms_us->cr_dma);
    ms_us->iobuf = (unsigned char*) ms_usb_buffer_alloc(ms_us->pusb_dev, MS_IOBUF_SIZE, GFP_KERNEL, &ms_us->iobuf_dma);
    if((!ms_us->cr) || (!ms_us->iobuf))
    {
        ms_usbhost_msg("[ms_usb_associate_dev] Allocating failed\n");
        return -ENOMEM;
    }
    return 0;
}

static void ms_usb_set_subclass(struct ms_usdata *ms_us,struct stUnusual_dev *unusual_dev, struct usb_interface_descriptor *idesc)
{
    if (unusual_dev->vendorName)
        ms_usbhost_msg("[ms_usb_get_device_info] Vendor: %s\n", unusual_dev->vendorName);

    if(unusual_dev->useProtocol == MS_DEVICE_SC)
        ms_us->subclass = idesc->bInterfaceSubClass;
    else
        ms_us->subclass = unusual_dev->useProtocol;

    if (strlen(ms_us->vendor) == 0)
    {
        if (unusual_dev->vendorName)
        {
            strncpy(ms_us->vendor, unusual_dev->vendorName, sizeof(ms_us->vendor)-1);
            ms_us->vendor[sizeof(ms_us->vendor)-1] = '\0';
        }
        else
            strncpy(ms_us->vendor, "Unknown", sizeof(ms_us->vendor));
    }
}

static void ms_usb_set_protocol(struct ms_usdata *ms_us,struct stUnusual_dev *unusual_dev, struct usb_interface_descriptor *idesc)
{
    if (unusual_dev->productName)
        ms_usbhost_msg("[ms_usb_get_device_info] Product: %s\n", unusual_dev->productName);

    if(unusual_dev->useTransport == MS_DEVICE_PR)
        ms_us->protocol = idesc->bInterfaceProtocol;
    else
        ms_us->protocol = unusual_dev->useTransport;

    if (strlen(ms_us->product) == 0)
    {
        if (unusual_dev->productName)
        {
            strncpy(ms_us->product, unusual_dev->productName, sizeof(ms_us->product)-1);
            ms_us->product[sizeof(ms_us->product)-1] = '\0';
        }
        else
            strncpy(ms_us->product, "Unknown", sizeof(ms_us->product));
    }
}

static void ms_usb_get_device_info(struct ms_usdata *ms_us, int idx)
{
    struct usb_interface_descriptor *idesc =
        &ms_us->pusb_intf->altsetting[ms_us->pusb_intf->act_altsetting].desc;
    struct stUnusual_dev *unusual_dev = &MS_USB_unusual_dev_list[idx];

    ms_us->unusual_dev = unusual_dev;
    ms_us->flags = unusual_dev->flags;
    ms_usb_set_subclass(ms_us,unusual_dev,idesc);
    ms_usb_set_protocol(ms_us,unusual_dev,idesc);
}

static int ms_usb_get_transport(struct ms_usdata *ms_us)
{
    switch (ms_us->protocol)
    {
        case MS_CB_PR:
            ms_usbhost_msg("CB Protocol\n");
            ms_us->transport = ms_usb_CB_transport;
            ms_us->transport_reset = ms_usb_CB_reset;
            ms_us->max_lun = 0; //FIXME, Find some way to get real Max. LUN
            break;
        case MS_BULK_PR:
            ms_usbhost_debug("BULK Protocol\n");
            ms_us->transport = ms_usb_bulk_transport;
            ms_us->transport_reset = ms_usb_bulk_reset;
            break;
        case MS_CBI_PR:
            ms_usbhost_msg("Not support CBI Protocol..\n");
        default:
            return -EIO;
    }
    if (ms_us->flags & MS_SINGLE_LUN)
        ms_us->max_lun = 0;
    return 0;
}

static int ms_usb_get_protocol(struct ms_usdata *ms_us)
{
    switch (ms_us->subclass)
    {
        case MS_RBC_SC:
            ms_usbhost_msg("RBC Subclass\n");
            ms_us->proto_handler = ms_usb_transparent_scsi_command;
            break;
        case MS_SFF_SC:
        case MS_SCSI_SC:
        case 0xFF:	// For Sony DC P8 (P series) and Sony DV
            ms_usbhost_debug("Transparent SCSI\n");
            ms_us->proto_handler = ms_usb_transparent_scsi_command;
            break;
        case MS_ATAPI_SC:
            ms_usbhost_msg("ATAPI Subclass\n");
            ms_us->proto_handler = ms_usb_transparent_scsi_command;
            ms_us->max_lun = 0;
            break;
#if SupportOtherDevice
        case MS_QIC_SC:
            ms_usbhost_msg("QIC-157 Subclass\n");
            ms_us->proto_handler = ms_usb_qic157_command;
            ms_us->max_lun = 0;
            break;
        case MS_UFI_SC:
            ms_usbhost_msg("UFI Subclass\n");
            ms_us->proto_handler = ms_usb_ufi_command;
            break;
#endif
        default:
            return -EIO;
    }
    return 0;
}

static void ms_usb_set_pipe(struct ms_usdata *ms_us,
    struct usb_endpoint_descriptor *ep_in,
    struct usb_endpoint_descriptor *ep_out,
    struct usb_endpoint_descriptor *ep_int)
{
    ms_us->ctrl_send_pipe = usb_sndctrlpipe(ms_us->pusb_dev, 0);
    ms_us->ctrl_recv_pipe = usb_rcvctrlpipe(ms_us->pusb_dev, 0);
    ms_us->bulk_send_pipe = usb_sndbulkpipe(ms_us->pusb_dev, ep_out->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
    ms_us->bulk_recv_pipe = usb_rcvbulkpipe(ms_us->pusb_dev, ep_in->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
    if (ep_int)
    {
        ms_us->int_recv_pipe = usb_rcvbulkpipe(ms_us->pusb_dev,
        ep_int->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
        ms_us->bInterval_ep = ep_int->bInterval;
    }
}

static int ms_usb_get_pipes(struct ms_usdata *ms_us)
{
    int tmp;

    struct usb_host_interface *altsetting =&ms_us->pusb_intf->altsetting[ms_us->pusb_intf->act_altsetting];
    struct usb_endpoint_descriptor *ep_tmp;
    struct usb_endpoint_descriptor *ep_in = NULL;
    struct usb_endpoint_descriptor *ep_out = NULL;
    struct usb_endpoint_descriptor *ep_int = NULL;

    for (tmp = 0; tmp < altsetting->desc.bNumEndpoints; tmp++)
    {
        ep_tmp=&altsetting->endpoint[tmp].desc;
        if ((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)
        {
            if (ep_tmp->bEndpointAddress & USB_DIR_IN)
                ep_in = ep_tmp;
            else
                ep_out = ep_tmp;
        }
        else if ((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)
            ep_int = ep_tmp;
    }
    if (!ep_in || !ep_out || (ms_us->protocol == MS_CBI_PR && !ep_int))
    {
        ms_usbhost_msg("EP sanity check failed! Rejecting dev.\n");
        return -EIO;
    }

    ms_usb_set_pipe(ms_us,ep_in,ep_out,ep_tmp);

    return 0;
}

static int ms_usb_acquire_resources(struct ms_usdata *ms_us)
{
    int regval;

    ms_us->current_urb = ms_usb_alloc_urb(GFP_KERNEL);
    //ms_us->data_urb = ms_usb_alloc_urb(GFP_KERNEL);
    //ms_us->status_urb = ms_usb_alloc_urb(GFP_KERNEL);
    if((!ms_us->current_urb)/* || (!ms_us->data_urb) || (!ms_us->status_urb)*/)
    {
        ms_usbhost_err("USB Request Block allocates failed\n");
        return -ENOMEM;
    }
    //osapi_down(&ms_us->dev_semaphore);
    osapi_mutex_lock(ms_us->dev_mutex);
    if (ms_us->protocol == MS_BULK_PR)
    {
        regval = ms_usb_control_msg(ms_us, ms_us->ctrl_recv_pipe, BULK_GET_MAX_LUN,
                    USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                    0, ms_us->ifnum, ms_us->iobuf, 1, HZ*5);

        ms_usbhost_debug("GetMaxLUN command regval is %d, data is %d\n", regval, ms_us->iobuf[0]);

        if (regval == 1)
            ms_us->max_lun = ms_us->iobuf[0];
        else
            ms_us->max_lun = 0;
    }

    ms_us->msc_device=(struct LUN_Device*)kmalloc(sizeof(struct LUN_Device)*(ms_us->max_lun+1), GFP_KERNEL);
    memset(ms_us->msc_device,0 , sizeof(struct LUN_Device)*(ms_us->max_lun+1));
    if (ms_us->unusual_dev->initFunction)
        ms_us->unusual_dev->initFunction(ms_us);

    //osapi_up(&ms_us->dev_semaphore);
    osapi_mutex_unlock(ms_us->dev_mutex);
    //ms_us->usb_state = IDLE_STATE;

    return 0;
}

static void ms_usb_unregister_dev(struct ms_usdata *ms_us)
{
    MS_U8 u8Lun;

    //osapi_down(&ms_us->dev_semaphore);
    osapi_mutex_lock(ms_us->dev_mutex);
    if (ms_us->cr)
    {
        ms_usb_buffer_free(ms_us->pusb_dev, sizeof(*ms_us->cr), ms_us->cr, ms_us->cr_dma);
        ms_us->cr = NULL;
    }
    if (ms_us->iobuf)
    {
        ms_usb_buffer_free(ms_us->pusb_dev, MS_IOBUF_SIZE, ms_us->iobuf, ms_us->iobuf_dma);
        ms_us->iobuf = NULL;
    }
    ms_usb_set_intfdata(ms_us->pusb_intf, NULL);
    ms_usb_put_dev(ms_us->pusb_dev);
    ms_us->pusb_dev = NULL;
    ms_us->pusb_intf = NULL;
    //osapi_up(&ms_us->dev_semaphore); // if ms_us to be protected, unlock later

    if (ms_us->extra_destructor)
    {
        ms_usbhost_msg("--> Call extra_destructor()\n");
        ms_us->extra_destructor(ms_us->extra);
    }
    if (ms_us->extra)
        kfree(ms_us->extra);
    if (ms_us->current_urb)
        ms_usb_free_urb(ms_us->current_urb);
    //if (ms_us->data_urb)
    //    ms_usb_free_urb(ms_us->data_urb);
    //if (ms_us->status_urb)
    //    ms_usb_free_urb(ms_us->status_urb);

    for (u8Lun=0; u8Lun<=ms_us->max_lun; u8Lun++)
        if (ms_us->msc_device[u8Lun].bFSInit)
            RemoveUSBDiskPort(ms_us->ustor_id, u8Lun);
    Mass_stor_us[ms_us->ustor_id] = NULL;

    if (ms_us->msc_device)
        kfree(ms_us->msc_device);

    osapi_mutex_unlock(ms_us->dev_mutex);
    osapi_mutex_delete(ms_us->dev_mutex);
    kfree(ms_us);
}

static int ms_usb_msc_probe(struct usb_interface *ms_intf, const struct usb_device_id *id)
{
    struct ms_usdata *ms_us;
    const int idx = id - MSC_usb_ids;
    int regval,tmp,i;
    struct usb_device *pudev;
    unsigned char startId;

    ms_usbhost_debug("USB MSC device detected\n");
    ms_usbhost_debug("act_altsetting = %d, idx = %d\n", ms_intf->act_altsetting, idx);
    ms_us = (struct ms_usdata *) kmalloc(sizeof(*ms_us), GFP_KERNEL);
    if (!ms_us)
    {
        ms_usbhost_msg("Out of memory %d\n", ms_us);
        return -ENOMEM;
    }

    memset(ms_us, 0, sizeof(struct ms_usdata));
    //osapi_init_MUTEX(&(ms_us->dev_semaphore));
    osapi_mutex_init(ms_us->dev_mutex, "US_MUTEX");
    //osapi_init_MUTEX_LOCKED(&(ms_us->sema));
    //init_completion(&(ms_us->notify));

    regval = ms_usb_associate_dev(ms_us, ms_intf);
    if (regval)
    {
        ms_usb_unregister_dev(ms_us);
        return regval;
    }

    ms_usb_get_device_info(ms_us, idx);
    if (strlen(ms_us->serial) == 0)
        strncpy(ms_us->serial, "None", sizeof(ms_us->serial));

    regval = ms_usb_get_transport(ms_us);
    if (regval)
    {
        ms_usb_unregister_dev(ms_us);
        return regval;
    }
    regval = ms_usb_get_protocol(ms_us);
    if (regval)
    {
        ms_usb_unregister_dev(ms_us);
        return regval;
    }
    regval = ms_usb_get_pipes(ms_us);
    if (regval)
    {
        ms_usb_unregister_dev(ms_us);
        return regval;
    }
    regval = ms_usb_acquire_resources(ms_us);
    if (regval)
    {
        ms_usb_unregister_dev(ms_us);
        return regval;
    }

    pudev = interface_to_usbdev(ms_intf);

    startId = (MAX_USTOR* (((struct usb_hcd *)pudev->bus->hcpriv)->host_id));
    USB_ASSERT(startId < NUM_OF_ROOT_HUB*MAX_USTOR, "startId >= NUM_OF_ROOT_HUB*MAX_USTOR\n");

    tmp = (-1);
    for (i=startId; i<startId+MAX_USTOR; i++)
    {
        if (Mass_stor_us[i] == NULL)
            tmp = i; // the result sequence 7,6,5...
    }

    //ERROR: Exceed the max support number of usb storage device !!
    if (tmp != (-1))
    {
        Mass_stor_us[tmp] = ms_us;
        ms_us->ustor_id = (unsigned char)tmp;
    }
    else
    {
        ms_usbhost_err("MSC: exceed the max number of support devices!!!\n");
        ms_usb_unregister_dev(ms_us);
        return -ENOBUFS;
    }

    ms_us->host_id = ((struct usb_hcd *)pudev->bus->hcpriv)->host_id;
    for(tmp = 0; tmp<=ms_us->max_lun; tmp++)
        ms_us->msc_device[tmp].bDeviceReady = FALSE;

    ms_usbhost_msg("find USB MSC device at device number %d\n", ms_us->pusb_dev->u32DevNum);

    return 0;
}

static void ms_usb_msc_disconnect(struct usb_interface *ms_intf)
{
    struct ms_usdata *ms_us = (struct ms_usdata*) ms_usb_get_intfdata(ms_intf);

    ms_usbhost_msg("MSC disconnect id: %d, us->flag %x \n", ms_us->ustor_id, ms_us->flags);
    ms_set_bit(MS_IDX_DISCONNECTING, &ms_us->flags, unsigned long);
    if (ms_test_and_clear_bit(MS_IDX_URB_ACTIVE, &ms_us->flags))
    {
        ms_usbhost_err("[MSCDIS]--> cancel USB request block\n");
        ms_usb_unlink_urb(ms_us->current_urb);
    }
    ms_usb_unregister_dev(ms_us);
}

int ms_ioctl_issue_ctrl(struct ms_usdata *ms_us, void *pData)
{
    struct usb_ioctl_ctrl_xfer *pCtrlData = (struct usb_ioctl_ctrl_xfer*)pData;
    int retval = -1;

    ms_usbhost_msg("[USB] control urb: bRequestType=%02x "
            "bRequest=%02x wValue=%04x "
            "wIndex=%04x wLength=%04x\n",
            pCtrlData->bRequestType,
            pCtrlData->bRequest,
            pCtrlData->wValue,
            pCtrlData->wIndex,
            pCtrlData->wLength);

    if((pCtrlData->wLength > 0) && ((pCtrlData->bRequestType & USB_DIR_IN) == 0)) /* OUT transfer */
        memcpy(ms_us->iobuf, pCtrlData->pDataBuf, pCtrlData->wLength);

    retval = ms_usb_control_msg(ms_us, ms_us->ctrl_recv_pipe,
              pCtrlData->bRequest,
              pCtrlData->bRequestType,
              pCtrlData->wValue,
              pCtrlData->wIndex,
              ms_us->iobuf,
              pCtrlData->wLength,
              (pCtrlData->timeout*HZ)/1000);

    ms_usbhost_msg("[USB] ctrl ioctl reault: %d\n", retval);

    if(retval < 0)
    {
        goto done;
    }

    if((pCtrlData->wLength > 0) && (pCtrlData->bRequestType & USB_DIR_IN)) /* IN transfer */
        memcpy(pCtrlData->pDataBuf, ms_us->iobuf, pCtrlData->wLength);

done:
    return retval;
}

int ms_ioctl_issue_bulk(struct ms_usdata *ms_us, void *pData)
{
    struct usb_ioctl_bulk_xfer *pBulkData = (struct usb_ioctl_bulk_xfer*)pData;
    int retval = -1;
    U32 act_len = 0;
    U32 pipe;

    ms_usbhost_msg("[USB] bulk urb: dir=%02x "
            "len=%02x timeout=%04x\n",
            pBulkData->dir,
            pBulkData->len);

    pipe = (pBulkData->dir == USB_IOCTL_BULK_OUT) ? ms_us->bulk_send_pipe : ms_us->bulk_recv_pipe;

    retval = ms_usb_bulk_transfer_buf(ms_us, pipe, pBulkData->pDataBuf, pBulkData->len, &act_len);

    if(retval == 0)
    {
        retval = act_len;
    }

    return retval;
}

#define DECLARE_VID_PID(VID, PID) (((U32)VID & 0xFFFF) << 16 | ((U32)PID & 0xFFFF))
#define QUIRK_TABLE_TERMINATOR DECLARE_VID_PID(0x0000, 0x0000)
#define QUIRK_NULL_FLAG	0x00000000UL

struct quirk {
    U32 vid_pid;
    U32 flag;
};

#define QUIRK_NUMMAX  20

struct quirk quirk_list[QUIRK_NUMMAX];
int quirk_num;

void quirk_list_init(void)
{
    int i;

    diag_printf("[USB] quirk list init, support max %d\n", QUIRK_NUMMAX);
    quirk_num = 0;

    for(i = 0;i < QUIRK_NUMMAX; i++)
    {
        quirk_list[i].vid_pid = QUIRK_TABLE_TERMINATOR;
        quirk_list[i].flag = QUIRK_NULL_FLAG;
    }
}

int quirk_list_add(U32 vid_pid, U32 flag)
{
    int retval = -1;

    if(quirk_num < QUIRK_NUMMAX)
    {
        quirk_list[quirk_num].vid_pid = vid_pid;
        quirk_list[quirk_num].flag = flag;

        diag_printf("[USB] quirk(%d) add vid/pid 0x%x flag 0x%x\n", quirk_num, vid_pid, flag);
        quirk_num++;
        retval = 0;
    }
    else
    {
        diag_printf("[USB] quirk num overrun, max(%d)\n", QUIRK_NUMMAX);
        retval = -1;
    }
    return retval;
}

int quirk_list_add_3g(U32 vid_pid)
{
    return quirk_list_add(vid_pid, QUIRK_IGNORE_TUR | QUIRK_IGNORE_CD_ROM | QUIRK_IGNORE_RD_CAP | QUIRK_IGNORE_MOD_SENSE);
}

bool quirk_bypass_stage(struct ms_usdata *ms_us, U32 stage_flag)
{
    U32 vid_pid = DECLARE_VID_PID(ms_us->pusb_dev->descriptor.idVendor,  ms_us->pusb_dev->descriptor.idProduct);
    int iter;

    iter = 0;

    while(iter < QUIRK_NUMMAX && quirk_list[iter].vid_pid != QUIRK_TABLE_TERMINATOR)
    {
        if((vid_pid == quirk_list[iter].vid_pid) && (quirk_list[iter].flag & stage_flag))
        {
            diag_printf("[USB] quirk bypass stage(0x%x) (VID_PID=0x%x)\n", stage_flag, vid_pid);
            return true;
        }
        iter++;
    }
    return false;
}
