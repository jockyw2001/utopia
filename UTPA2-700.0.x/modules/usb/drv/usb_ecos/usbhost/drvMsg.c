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

#include <MsCommon.h>
#include <cyg/hal/hal_if.h>

#include  "include/drvConfig.h"

#ifdef CONFIG_DEBUG
#define DEBUG
#else
#undef DEBUG
#endif

#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"

#include "include/drvUSB.h"
#include "drvHCD.h"
#include "include/drvCPE_AMBA.h"
#include "drvEHCI.h"

#if 1

char usb_string_buf[64];

struct usb_api_data {
  //wait_queue_head_t wqh;
  int done;
};

static void usb_api_blocking_completion(struct urb *urb, struct pt_regs *regs)
{
	struct usb_api_data *awd = (struct usb_api_data *)urb->context;


	if (urb->SetDMABuf != NULL)
	{
		//MsOS_FreeMemory(urb->SetDMABuf, gs32NonCachedPoolID_MIU0 );
		Usb_FreeNonCachedMemory(urb->SetDMABuf);

		urb->SetDMABuf = NULL;
	}

	if (urb->TxDMABuf != NULL)
	{
		if (usb_pipein(urb->pipe))
        {
            #ifdef DEBUG_PERFORMANCE
            if (urb->transfer_buffer_length >= 512)
                diag_printf("usb_api_blocking_completion: copy length: 0x%lx \n", urb->transfer_buffer_length); 
            #endif
			memcpy(urb->transfer_buffer, urb->TxDMABuf, urb->transfer_buffer_length);
        }
		//MsOS_FreeMemory(urb->TxDMABuf,gs32NonCachedPoolID_MIU0 );
		Usb_FreeNonCachedMemory(urb->TxDMABuf);

		urb->TxDMABuf = NULL;
	}

	awd->done = 1;
	wmb();

	wake_up(&awd->wqh);
}

static int usb_start_wait_urb(struct urb *urb, int timeout, int* actual_length)
{
  struct usb_api_data awd;
  int status;
  U32 d_cnt=0;

  DECLARE_WAITQUEUE(wait, current);
  init_waitqueue_head(&awd.wqh);

  awd.done = 0;

  set_current_state(TASK_UNINTERRUPTIBLE);
  add_wait_queue(&awd.wqh, &wait);

  urb->context = &awd;
  status = usb_submit_urb(urb, GFP_ATOMIC);
  if (status) {

    usb_free_urb(urb);

    set_current_state(TASK_RUNNING);
    remove_wait_queue(&awd.wqh, &wait);
    return status;
  }

  while ( timeout && (awd.done == 0) )
  {
    if ( d_cnt > (U32) timeout)
    {
      timeout =0;
      break;
    }
    else
    {
      //mdelay(1);//tick base is 1 ms
      HAL_DELAY_US(1000);
      d_cnt+=1;
    }
    set_current_state(TASK_UNINTERRUPTIBLE);
    rmb();
  }
  //dbg("Done(%d) or time-out,tick is %ld\n",awd.done,jiffies);

  set_current_state(TASK_RUNNING);
  remove_wait_queue(&awd.wqh, &wait);

  if (!timeout && !awd.done) {
    if (urb->status != -EINPROGRESS) {  /* No callback?!! */
      printk(KERN_ERR "usb: raced timeout, "
          "pipe 0x%x status %d time left %d\n",
          urb->pipe, urb->status, timeout);
      status = urb->status;
    } else {
        struct usb_hcd * hcd =  urb->dev->bus->hcpriv;
        diag_printf("usb_control/bulk_msg: timeout... %d \n", urb->status);

        diag_printf("<usb_start_wait_urb> set Bad Device...............................................\n");        
        hcd->isBadDevice = TRUE;
        hcd->badDeviceCnt++;

      if (usb_unlink_urb(urb) == -ETIMEDOUT)  
      {
        diag_printf("timeout retry\n");
        usb_unlink_urb(urb);
      }
      diag_printf("unllnk urb ok\n");
      status = -ETIMEDOUT;
    }
  } else
  status = urb->status;

  if (actual_length)
    *actual_length = urb->actual_length;

  usb_free_urb(urb);
  return status;
}

int usb_internal_control_msg(struct usb_device *usb_dev, unsigned int pipe,
          struct usb_ctrlrequest *cmd,  void *data, int len, int timeout)
{
  struct urb *urb;
  int retv;
  int length;

  urb = usb_alloc_urb(0, GFP_NOIO);
  if (!urb)
    return -ENOMEM;

  usb_fill_control_urb(urb, usb_dev, pipe, (unsigned char*)cmd, data, len,
       usb_api_blocking_completion, 0);
  retv = usb_start_wait_urb(urb, timeout, &length);
  if (retv < 0)
     {
     	diag_printf("usb_start_wait_urb retv:%d \n",-retv);
       	return retv;
     }
  else
    return length;
}

int usb_control_msg(struct usb_device *dev, unsigned int pipe, unsigned char request, unsigned char requesttype,
       unsigned short value, unsigned short index, void *data, unsigned short size, int timeout)
{
  struct usb_ctrlrequest *dr = (struct usb_ctrlrequest*) kmalloc(sizeof(struct usb_ctrlrequest), GFP_NOIO);
  int ret;
  U32 addr_t;

  if (!dr)
    return -ENOMEM;

  addr_t = (U32)dr;
  dr->bRequestType= requesttype;
  dr->bRequest = request;
  dr->wValue = CPUToLE16(value);
  dr->wIndex = CPUToLE16(index);
  dr->wLength = CPUToLE16(size);

  ret = usb_internal_control_msg(dev, pipe, dr, data, size, timeout);

  kfree((void *)addr_t);

  return ret;
}


int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe,
      void *data, int len, int *actual_length, int timeout)
{
  struct urb *urb;

  if (len < 0)
    return -EINVAL;

  urb=usb_alloc_urb(0, GFP_KERNEL);
  if (!urb)
    return -ENOMEM;

  usb_fill_bulk_urb(urb, usb_dev, pipe, data, len,
        usb_api_blocking_completion, 0);

  return usb_start_wait_urb(urb,timeout,actual_length);
}

int usb_get_descriptor(struct usb_device *dev, unsigned char type, unsigned char index, void *buf, int size)
{
  int ii = 5;
  int relt;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  memset(buf,0,size); 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  while (ii--) {
    if ((relt = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
            (type << 8) + index, 0, buf, size,
            HZ * USB_CTRL_GET_TIMEOUT)) > 0
        || relt == -EPIPE)
      break;
  }
  return relt;
}

int usb_get_string(struct usb_device *dev, unsigned short langid, unsigned char index, void *buf, int size)
{
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
    USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
    (USB_DT_STRING << 8) + index, langid, buf, size,
    HZ * USB_CTRL_GET_TIMEOUT);
}

int usb_get_device_descriptor(struct usb_device *dev)
{
  int ret = usb_get_descriptor(dev, USB_DT_DEVICE, 0, &dev->descriptor,
             sizeof(dev->descriptor));
  if (ret >= 0) {
    dev->descriptor.bcdUSB = LE16ToCPU(dev->descriptor.bcdUSB);
    dev->descriptor.idVendor = LE16ToCPU(dev->descriptor.idVendor);
    dev->descriptor.idProduct = LE16ToCPU(dev->descriptor.idProduct);
    dev->descriptor.bcdDevice = LE16ToCPU(dev->descriptor.bcdDevice);
  }
  return ret;
}

int usb_get_status(struct usb_device *dev, int type, int target, void *data)
{
  return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
    USB_REQ_GET_STATUS, USB_DIR_IN | type, 0, target, data, 2,
    HZ * USB_CTRL_GET_TIMEOUT);
}

int usb_clear_halt(struct usb_device *dev, int pipe)
{
  int result;
  int endp = usb_pipeendpoint(pipe);

  if (usb_pipein (pipe))
    endp |= USB_DIR_IN;

  result = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
    USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0, endp, NULL, 0,
    HZ * USB_CTRL_SET_TIMEOUT);

  if (result < 0)
    return result;

  usb_settoggle(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);
  usb_endpoint_running(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

  return 0;
}

void usb_disable_endpoint(struct usb_device *dev, U32 epaddr)
{
  if (dev && dev->bus && dev->bus->op && dev->bus->op->disable)
    dev->bus->op->disable(dev, epaddr);
  else {
    U32 epnum = epaddr & USB_ENDPOINT_NUMBER_MASK;

    if (usb_endpoint_out(epaddr)) {
      usb_endpoint_halt(dev, epnum, 1);
      dev->epmaxpacketout[epnum] = 0;
    } else {
      usb_endpoint_halt(dev, epnum, 0);
      dev->epmaxpacketin[epnum] = 0;
    }
  }
}

void usb_disable_interface(struct usb_device *dev, struct usb_interface *intf)
{
  struct usb_host_interface *hintf =
      &intf->altsetting[intf->act_altsetting];
  int i;

  for (i = 0; i < hintf->desc.bNumEndpoints; ++i) {
    usb_disable_endpoint(dev,
        hintf->endpoint[i].desc.bEndpointAddress);
  }
}

void usb_disable_device(struct usb_device *dev, int skip_ep0)
{
  int i;

  dbg("nuking URBs for device %s", dev->dev.bus_id);
  for (i = skip_ep0; i < 16; ++i) {
    usb_disable_endpoint(dev, i);
    usb_disable_endpoint(dev, i + USB_DIR_IN);
  }
}


void usb_enable_endpoint(struct usb_device *dev,
    struct usb_endpoint_descriptor *epd)
{
  int maxsize = epd->wMaxPacketSize;
  unsigned int eptaddr = epd->bEndpointAddress;
  unsigned int eptnum = eptaddr & USB_ENDPOINT_NUMBER_MASK;
  int is_control = ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_CONTROL);

  if (usb_endpoint_out(eptaddr) || is_control) {
    usb_endpoint_running(dev, eptnum, 1);
    usb_settoggle(dev, eptnum, 1, 0);
    dev->epmaxpacketout[eptnum] = maxsize;
  }
  if (!usb_endpoint_out(eptaddr) || is_control) {
    usb_endpoint_running(dev, eptnum, 0);
    usb_settoggle(dev, eptnum, 0, 0);
    dev->epmaxpacketin[eptnum] = maxsize;
  }
}

void usb_enable_interface(struct usb_device *dev,
    struct usb_interface *intf)
{
  struct usb_host_interface *hintf =
      &intf->altsetting[intf->act_altsetting];
  int i;

  for (i = 0; i < hintf->desc.bNumEndpoints; ++i)
    usb_enable_endpoint(dev, &hintf->endpoint[i].desc);
}

int usb_set_interface(struct usb_device *dev, int interface, int alternate)
{
  struct usb_interface *iface;
  int ret;
  int manual = 0;

  iface = usb_ifnum_to_if(dev, interface);
  if (!iface) {
    usb_warn("Invalid interface %d", interface);
    return -EINVAL;
  }

  if (alternate < 0 || alternate >= iface->num_altsetting)
    return -EINVAL;

  ret = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
           USB_REQ_SET_INTERFACE, USB_RECIP_INTERFACE,
           iface->altsetting[alternate]
             .desc.bAlternateSetting,
           interface, NULL, 0, HZ * 5);

  if (ret == -EPIPE && iface->num_altsetting == 1) {
    dbg("manual set interface for iface %d, alt %d",
      interface, alternate);
    manual = 1;
  } else if (ret < 0)
    return ret;

  usb_disable_interface(dev, iface);

  iface->act_altsetting = alternate;

  if (manual) {
    struct usb_host_interface *iface_as =
        &iface->altsetting[alternate];
    int ii;

    for (ii = 0; ii < iface_as->desc.bNumEndpoints; ii++) {
      unsigned int epaddr =
        iface_as->endpoint[ii].desc.bEndpointAddress;
      unsigned int pipe =
  __create_pipe(dev, USB_ENDPOINT_NUMBER_MASK & epaddr)
  | (usb_endpoint_out(epaddr) ? USB_DIR_OUT : USB_DIR_IN);

      usb_clear_halt(dev, pipe);
    }
  }

  usb_enable_interface(dev, iface);

  return 0;
}

int usb_set_configuration(struct usb_device *dev, int configuration)
{
  int i, ret;
  struct usb_host_config *cp = NULL;

  for (i=0; i<dev->descriptor.bNumConfigurations; i++) {
    if (dev->config[i].desc.bConfigurationValue == configuration) {
      cp = &dev->config[i];
      break;
    }
  }
  if ((!cp && configuration != 0) || (cp && configuration == 0)) {
    usb_warn("selecting invalid configuration %d", configuration);
    return -EINVAL;
  }

  if (dev->state != USB_STATE_ADDRESS)
    usb_disable_device (dev, 1);  // Skip ep0
  dev->toggle[0] = dev->toggle[1] = 0;
  dev->halted[0] = dev->halted[1] = 0;
  dev->state = USB_STATE_ADDRESS;

  if ((ret = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
      USB_REQ_SET_CONFIGURATION, 0, configuration, 0,
      NULL, 0, HZ * USB_CTRL_SET_TIMEOUT)) < 0)
    return ret;
  if (configuration)
    dev->state = USB_STATE_CONFIGURED;
  dev->actconfig = cp;

  for (i = 0; i < cp->desc.bNumInterfaces; ++i) {
    struct usb_interface *intf = cp->interface[i];

    intf->act_altsetting = 0;
    usb_enable_interface(dev, intf);
  }

    // Add string
    if (dev->descriptor.iProduct)
    {
        usb_string(dev, dev->descriptor.iProduct, usb_string_buf, 64);
        dev->product = usb_string_buf;
    }
    else
        dev->product = NULL;
    diag_printf("USB device product string (index 2) = %s\n", dev->product);

  return 0;
}


int usb_string(struct usb_device *dev, int index, char *buf, size_t size)
{
#if 1

  unsigned char *tbuf;
  int err, len;
  U32 u, idx;
  if ( (int)size <= 0 || !buf || !index)
    return -EINVAL;
  buf[0] = 0;
  tbuf = (unsigned char*) kmalloc(256, GFP_KERNEL);
  if (!tbuf)
    return -ENOMEM;

  /* get langid for strings if it's not yet known */
  if (!dev->have_langid) {
    err = usb_get_string(dev, 0, 0, tbuf, 4);
    if (err < 0) {
      dbg("error getting string descriptor 0 (error=%d)", err);
      goto errout;
    } else if (err < 4 || tbuf[0] < 4) {
      dbg("string descriptor 0 too short%s","");
      err = -EINVAL;
      goto errout;
    } else {
      dev->have_langid = -1;
      dev->string_langid = tbuf[2] | (tbuf[3]<< 8);
        /* always use the first langid listed */
      dbg("USB device number %d default language ID 0x%x",
        dev->devnum, dev->string_langid);
    }
  }

  err = usb_get_string(dev, dev->string_langid, index, tbuf, 2);
  if(err<2)
    goto errout;
  len=tbuf[0];

  err = usb_get_string(dev, dev->string_langid, index, tbuf, len);
  if (err < 0)
    goto errout;

  size--;    
  for (idx = 0, u = 2; u < (U32) err; u += 2) {
    if (idx >= size)
      break;
    if (tbuf[u+1])      
      buf[idx++] = '?';  
    else
      buf[idx++] = tbuf[u];
  }
  buf[idx] = 0;
  err = idx;

 errout:
  kfree(tbuf);
  return err;
  #else
  return 0;
  #endif
}


int usb_reset_configuration(struct usb_device *dev)
{
   int      i, retval;
   struct usb_host_config  *config;

   for (i = 1; i < 16; ++i) {
     usb_disable_endpoint(dev, i);
     usb_disable_endpoint(dev, i + USB_DIR_IN);
  }

  config = dev->actconfig;
  retval = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
      USB_REQ_SET_CONFIGURATION, 0,
      config->desc.bConfigurationValue, 0,
      NULL, 0, HZ * USB_CTRL_SET_TIMEOUT);
  if (retval < 0)
    return retval;

  dev->toggle[0] = dev->toggle[1] = 0;
  dev->halted[0] = dev->halted[1] = 0;

  for (i = 0; i < config->desc.bNumInterfaces; i++) {
    struct usb_interface *intf = config->interface[i];
    intf->act_altsetting = 0;
    usb_enable_interface(dev, intf);
  }
  return 0;
}
#endif	//#ifdef

