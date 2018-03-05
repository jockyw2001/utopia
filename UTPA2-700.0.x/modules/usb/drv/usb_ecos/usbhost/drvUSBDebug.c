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

#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"


#include "include/drvUSB.h"
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_endpoint(struct usb_host_endpoint *endpoint)
{
  usb_show_endpoint_descriptor(&endpoint->desc);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_interface(struct usb_host_interface *altsetting)
{
  int i;

  usb_show_interface_descriptor(&altsetting->desc);

  for (i = 0; i < altsetting->desc.bNumEndpoints; i++)
    usb_show_endpoint(altsetting->endpoint + i);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_show_config(struct usb_host_config *config)
{
  int ii, jj;
  struct usb_interface *ifp;

  usb_show_config_descriptor(&config->desc);
  for (ii = 0; ii < config->desc.bNumInterfaces; ii++) {
    ifp = config->interface[ii];

    if (!ifp)
      break;

    printk("\nInterface: %d\n", ii);
    for (jj = 0; jj < ifp->num_altsetting; jj++)
      usb_show_interface(ifp->altsetting + jj);
  }
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_device(struct usb_device *dev)
{
  int i;

  usb_show_device_descriptor(&dev->descriptor);
  if (dev->descriptor.bNumConfigurations > 0) {
    for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
      usb_show_config(dev->config + i);
  }
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_device_descriptor(struct usb_device_descriptor *desc)
{
  if (!desc)
  {
    printk("desc is NULL\n");
    return;
  }
  printk("  Len = %d\n", desc->bLength);
  printk("  DescType = %x\n", desc->bDescriptorType);

  printk("  Ver = %x.%02x\n", desc->bcdUSB >> 8, desc->bcdUSB & 0xff);
  printk("  VID:PID      = %04x:%04x\n", desc->idVendor, desc->idProduct);
  printk("  MaxPktSize0      = %d\n", desc->bMaxPacketSize0);
  printk("  NumConfig   = %d\n", desc->bNumConfigurations);
  printk("  Dev Ver      = %x.%02x\n", desc->bcdDevice >> 8, desc->bcdDevice & 0xff);

  printk("  Dev Class:SubClass:Protocol = %02x:%02x:%02x\n",
    desc->bDeviceClass, desc->bDeviceSubClass, desc->bDeviceProtocol);
  switch (desc->bDeviceClass) {
  case 0:
    printk("    Per-interface classes\n");
    break;
  case USB_CLASS_AUDIO:
    printk("    Audio class\n");
    break;
  case USB_CLASS_COMM:
    printk("    Communications class\n");
    break;
  case USB_CLASS_HID:
    printk("    HID class\n");
    break;
  case USB_CLASS_PRINTER:
    printk("    Printer class\n");
    break;
  case USB_CLASS_STORAGE:
    printk("    MSD class\n");
    break;
  case USB_CLASS_HUB:
    printk("    Hub class\n");
    break;
  case USB_CLASS_VENDOR_SPEC:
    printk("    Vendor device Class\n");
    break;
  default:
    printk("    Unknown device Class\n");
  }
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_config_descriptor(struct usb_config_descriptor *desc)
{
  printk("Config Desc:\n");
  printk("Len = %d\n",desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("TotalLen = %x\n", desc->wTotalLength);
  printk("Interfaces = %x\n", desc->bNumInterfaces);
  printk("ConfigValue = %x\n", desc->bConfigurationValue);
  printk("Config = %x\n", desc->iConfiguration);
  printk("Attr = %x\n", desc->bmAttributes);
  printk("MaxPower = %d\n", desc->bMaxPower * 2);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_interface_descriptor(struct usb_interface_descriptor *desc)
{
  printk("Alt Setting: %d\n", desc->bAlternateSetting);
  printk("Len = %d\n", desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("Interface = %x\n", desc->bInterfaceNumber);
  printk("AltSetting = %x\n", desc->bAlternateSetting);
  printk("NumEpts = %x\n", desc->bNumEndpoints);
  printk("Interface Class:SubClass:Protocol =   %x:%x:%x\n",
    desc->bInterfaceClass, desc->bInterfaceSubClass, desc->bInterfaceProtocol);
  printk("Interface = %x\n", desc->iInterface);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_endpoint_descriptor(struct usb_endpoint_descriptor *desc)
{
  printk("Endpoint:\n");
  printk("Len = %d\n", desc->bLength);
  printk("DescType = %x\n", desc->bDescriptorType);
  printk("EptAddress = %x\n", desc->bEndpointAddress);
  printk("Attributes = %x\n", desc->bmAttributes);
  printk("MaxPktSize = %x\n", desc->wMaxPacketSize);
  printk("Interval = %x\n", desc->bInterval);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_show_string(struct usb_device *dev, char *id, int index)
{
#if 0
  char *buf;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!index)
    return;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!(buf = (char*) kmalloc(256, GFP_KERNEL)))
    return;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (usb_string(dev, index, buf, 256) > 0)
    {
       dev_printk(KERN_INFO, &dev->dev, "%s: %s\n", id, buf);
    }
  kfree(buf);
#endif
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_dump_urb (struct urb *urb)
{
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("urb                   :%p\n", urb);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("dev                   :%p\n", urb->dev);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("pipe                  :%08X\n", urb->pipe);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("status                :%d\n", urb->status);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_flags        :%08X\n", urb->transfer_flags);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_buffer       :%p\n", urb->transfer_buffer);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("transfer_buffer_length:%d\n", urb->transfer_buffer_length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("actual_length         :%d\n", urb->actual_length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("setup_packet          :%p\n", urb->setup_packet);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("start_frame           :%d\n", urb->start_frame);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("number_of_packets     :%d\n", urb->number_of_packets);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("interval              :%d\n", urb->interval);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("error_count           :%d\n", urb->error_count);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("context               :%p\n", urb->context);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  printk ("complete              :%p\n", urb->complete);
}

