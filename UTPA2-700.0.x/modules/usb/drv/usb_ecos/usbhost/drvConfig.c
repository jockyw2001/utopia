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
#include  "include/drvList.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include "include/drvUSB.h"
#include "drvHCD.h"
#define USB_MAXALTSETTING    128
#define USB_MAXENDPOINTS    30

#define USB_MAXCONFIG      8
#define USB_ALTSETTINGALLOC    4
#define USB_MAXINTERFACES    32
#define max_packet(wMaxPacketSize) ((wMaxPacketSize) & 0x07ff)

static int usb_parse_endpoint(struct usb_device *dev, struct usb_host_endpoint *endpoint, unsigned char *buffer, int size)
{
  struct usb_descriptor_header *header;
  unsigned char *begin;
  int parsed = 0, len, numskipped;

  header = (struct usb_descriptor_header *)buffer;

  if (header->bLength > size) {
    usb_err("ran out of descriptors parsing%s","");
    return -1;
  }

  if (header->bDescriptorType != USB_DT_ENDPOINT) {
    usb_warn("unexpected descriptor 0x%X, expecting endpoint, 0x%X",
      header->bDescriptorType, USB_DT_ENDPOINT);
    return parsed;
  }

  if (header->bLength == USB_DT_ENDPOINT_AUDIO_SIZE)
    memcpy(&endpoint->desc, buffer, USB_DT_ENDPOINT_AUDIO_SIZE);
  else
    memcpy(&endpoint->desc, buffer, USB_DT_ENDPOINT_SIZE);

  endpoint->desc.wMaxPacketSize = LE16ToCPU(endpoint->desc.wMaxPacketSize);

#if 0
  if (to_usb_device(ddev)->speed == USB_SPEED_HIGH
          && usb_endpoint_xfer_bulk(d)) {
      unsigned maxp;

      maxp = le16_to_cpu(endpoint->desc.wMaxPacketSize) & 0x07ff;
      if (maxp != 512)
          dev_warn(ddev, "config %d interface %d altsetting %d "
              "bulk endpoint 0x%X has invalid maxpacket %d\n",
              cfgno, inum, asnum, d->bEndpointAddress,
              maxp);
  }
#else
    if (dev->speed == USB_SPEED_HIGH
        && (endpoint->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK) {
        unsigned maxp;

        maxp = max_packet(endpoint->desc.wMaxPacketSize);
        if (maxp != 512)
            diag_printf("bulk endpoint 0x%X has invalid maxpacket %d\n",
                endpoint->desc.bEndpointAddress, maxp);
    }
#endif

  buffer += header->bLength;
  size -= header->bLength;
  parsed += header->bLength;

  begin = buffer;
  numskipped = 0;
  while ((U32)size >= sizeof(struct usb_descriptor_header)) {
    header = (struct usb_descriptor_header *)buffer;

    if (header->bLength < 2) {
      usb_err("invalid descriptor length of %d", header->bLength);
      return -1;
    }

    if ((header->bDescriptorType == USB_DT_ENDPOINT) ||
        (header->bDescriptorType == USB_DT_INTERFACE) ||
        (header->bDescriptorType == USB_DT_CONFIG) ||
        (header->bDescriptorType == USB_DT_DEVICE))
      break;
#if 0
    if ( header->bDescriptorType == USB_DT_OTG )
    {
      vdbg("OTG descriptor found, bmAttributes=0x%08X\n",buffer[2]);
      OTG.otgd->desc.bLength = buffer[0];
      OTG.otgd->desc.bDescriptorType = buffer[1];
      OTG.otgd->desc.bmAttributes = buffer[2];
    }
    else
#else
      dbg("skipping descriptor 0x%X,len=%d",
      header->bDescriptorType,header->bLength);
#endif
    numskipped++;
    buffer += header->bLength;
    size -= header->bLength;
    parsed += header->bLength;
  }
  if (numskipped)
    dbg("skipped %d class/vendor specific endpoint descriptors", numskipped);

  len = (int)(buffer - begin);
  if (!len) {
    endpoint->extra = NULL;
    endpoint->extralen = 0;
    return parsed;
  }

  endpoint->extra = (unsigned char *) kmalloc(len, GFP_KERNEL);

  if (!endpoint->extra) {
    usb_err("couldn't allocate memory for endpoint extra descriptors%s","");
    endpoint->extralen = 0;
    return parsed;
  }

  memcpy(endpoint->extra, begin, len);
  endpoint->extralen = len;

  return parsed;
}

static void usb_release_intf(struct device_s *dev)
{
  struct usb_interface *intf;
  int j;
  int k;

  intf = to_usb_interface(dev);

  if (intf->altsetting) {
    for (j = 0; j < intf->num_altsetting; j++) {
      struct usb_host_interface *as = &intf->altsetting[j];
      if (as->extra)
        kfree(as->extra);

      if (as->endpoint) {
        for (k = 0; k < as->desc.bNumEndpoints; k++)
          if (as->endpoint[k].extra)
            kfree(as->endpoint[k].extra);
        kfree(as->endpoint);
      }
    }
    kfree(intf->altsetting);
  }
  kfree(intf);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int usb_parse_interface(struct usb_device *dev, struct usb_interface *interface, unsigned char *buffer, int size)
{
  int i, len, numskipped, retval, parsed = 0;
  struct usb_descriptor_header *header;
  struct usb_host_interface *ifp;
  unsigned char *begin;

  interface->act_altsetting = 0;
  interface->num_altsetting = 0;
  interface->max_altsetting = USB_ALTSETTINGALLOC;
  device_initialize(&interface->dev);
  interface->dev.release = usb_release_intf;

  //get_device(&interface->dev);

  interface->altsetting = (struct usb_host_interface *) kmalloc(sizeof(*interface->altsetting) * interface->max_altsetting,
          GFP_KERNEL);

  if (!interface->altsetting) {
    usb_err("failed to alloc altsetting%s","");
    return -1;
  }

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  while (size > 0) {
    struct usb_interface_descriptor  *d;

    if (interface->num_altsetting >= interface->max_altsetting) {
      struct usb_host_interface *ptr;
      int oldmas;

      oldmas = interface->max_altsetting;
      interface->max_altsetting += USB_ALTSETTINGALLOC;
      if (interface->max_altsetting > USB_MAXALTSETTING) {
        usb_warn("too many alt settings (incr %d max %d)\n",
          USB_ALTSETTINGALLOC, USB_MAXALTSETTING);
        return -1;
      }

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      ptr = (struct usb_host_interface *) kmalloc(sizeof(*ptr) * interface->max_altsetting, GFP_KERNEL);
      if (ptr == NULL) {
        usb_err("failed to kmalloc interface->altsetting%s","");
        return -1;
      }
      memcpy(ptr, interface->altsetting, sizeof(*interface->altsetting) * oldmas);
      kfree(interface->altsetting);
      interface->altsetting = ptr;
    }

    ifp = interface->altsetting + interface->num_altsetting;
    ifp->endpoint = NULL;
    ifp->extra = NULL;
    ifp->extralen = 0;
    interface->num_altsetting++;

    memcpy(ifp, buffer, USB_DT_INTERFACE_SIZE);

    buffer += ifp->desc.bLength;
    parsed += ifp->desc.bLength;
    size -= ifp->desc.bLength;

    begin = buffer;
    numskipped = 0;

    while ((U32)size >= sizeof(struct usb_descriptor_header)) {
      header = (struct usb_descriptor_header *)buffer;

      if (header->bLength < 2) {
        usb_err("invalid descriptor len of %d", header->bLength);
        return -1;
      }

      if ((header->bDescriptorType == USB_DT_INTERFACE) ||
          (header->bDescriptorType == USB_DT_ENDPOINT) ||
          (header->bDescriptorType == USB_DT_CONFIG) ||
          (header->bDescriptorType == USB_DT_DEVICE))
        break;

      numskipped++;

      buffer += header->bLength;
      parsed += header->bLength;
      size -= header->bLength;
    }

    if (numskipped)
      dbg("skipped %d class/vendor specific interface descriptors", numskipped);

    len = (int)(buffer - begin);
    if (len) {
      ifp->extra = (unsigned char*) kmalloc(len, GFP_KERNEL);

      if (!ifp->extra) {
        usb_err("couldn't allocate memory for interface extra descriptors%s","");
        ifp->extralen = 0;
        return -1;
      }
      memcpy(ifp->extra, begin, len);
      ifp->extralen = len;
    }

    header = (struct usb_descriptor_header *)buffer;
    if (((U32)size >= sizeof(struct usb_descriptor_header)) &&
        ((header->bDescriptorType == USB_DT_CONFIG) ||
         (header->bDescriptorType == USB_DT_DEVICE)))
      return parsed;

    if (ifp->desc.bNumEndpoints > USB_MAXENDPOINTS) {
      usb_warn("too many endpoints%s","");
      return -1;
    }
    else if ( ifp->desc.bNumEndpoints == 0 )
    {
      goto skip_ep_pase;
    }

    ifp->endpoint = (struct usb_host_endpoint *)
      kmalloc(ifp->desc.bNumEndpoints *
      sizeof(struct usb_host_endpoint), GFP_KERNEL);
    if (!ifp->endpoint) {
      usb_err("out of memory%s","");
      return -1;
    }

    memset(ifp->endpoint, 0, ifp->desc.bNumEndpoints *
      sizeof(struct usb_host_endpoint));

    for (i = 0; i < ifp->desc.bNumEndpoints; i++) {
      header = (struct usb_descriptor_header *)buffer;

      if (header->bLength > size) {
        usb_err("ran out of descriptors parsing%s","");
        return -1;
      }

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      retval = usb_parse_endpoint(dev, ifp->endpoint + i, buffer, size);
      if (retval < 0)
        return retval;

      buffer += retval;
      parsed += retval;
      size -= retval;
    }
skip_ep_pase:
    d = (struct usb_interface_descriptor *)buffer;
    if (size < USB_DT_INTERFACE_SIZE
        || d->bDescriptorType != USB_DT_INTERFACE
        || !d->bAlternateSetting)
      return parsed;
  }

  return parsed;
}

int usb_parse_configuration(struct usb_device *dev, struct usb_host_config *config, char *buffer)
{
  int i, size;
  int retval = -EINVAL;
  struct usb_descriptor_header *header;

  memcpy(&config->desc, buffer, USB_DT_CONFIG_SIZE);
  config->desc.wTotalLength = LE16ToCPU(config->desc.wTotalLength);
  size = config->desc.wTotalLength;

  //for (i = 0; i < USB_MAXINTERFACES; ++i)
  //  config->interface[i] = (struct usb_interface)NULL;

  if (config->desc.bNumInterfaces > USB_MAXINTERFACES) {
    usb_warn("too many interfaces%s","");
    goto error;
  }

  for (i = 0; i < config->desc.bNumInterfaces; ++i) {
    config->interface[i] = (struct usb_interface *) kmalloc(sizeof(struct usb_interface), GFP_KERNEL);
    dbg("kmalloc IF %p, numif %i", config->interface[i], i);
    if (!config->interface[i]) {
      usb_err("out of memory%s","");
      retval = -ENOMEM;
      goto error;
    }
    memset(config->interface[i], 0x00, sizeof(struct usb_interface));
  }

  buffer += config->desc.bLength;
  size -= config->desc.bLength;

  config->extra = NULL;
  config->extralen = 0;

  for (i = 0; i < config->desc.bNumInterfaces; i++) {
    int numskipped, len;
    char *begin;

    begin = buffer;
    numskipped = 0;
    while ((U32)size >= sizeof(struct usb_descriptor_header)) {
      header = (struct usb_descriptor_header *)buffer;

      if ((header->bLength > size) || (header->bLength < 2)) {
        usb_err("invalid descriptor length of %d", header->bLength);
        return -1;
      }

      if ((header->bDescriptorType == USB_DT_ENDPOINT) ||
          (header->bDescriptorType == USB_DT_INTERFACE) ||
          (header->bDescriptorType == USB_DT_CONFIG) ||
          (header->bDescriptorType == USB_DT_DEVICE))
        break;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
       dbg("skipping descriptor 0x%X", header->bDescriptorType);
      numskipped++;
      buffer += header->bLength;
      size -= header->bLength;
    }
    if (numskipped)
      dbg("skipped %d class/vendor specific endpoint descriptors", numskipped);

    len = (int)(buffer - begin);
    if (len) {
      if (config->extralen) {
        usb_warn("extra config descriptor%s","");
      } else {
        config->extra = (unsigned char*) kmalloc(len, GFP_KERNEL);
        if (!config->extra) {
          usb_err("couldn't allocate memory for config extra descriptors%s","");
          config->extralen = 0;
          return -1;
        }

        memcpy(config->extra, begin, len);
        config->extralen = len;
      }
    }

    retval = usb_parse_interface(dev, config->interface[i], (unsigned char*) buffer, size);
    if (retval < 0)
      return retval;

    buffer += retval;
    size -= retval;
  }

  return size;
error:
  for (i = 0; i < USB_MAXINTERFACES; ++i)
    kfree(config->interface[i]);
  return retval;
}

void usb_destroy_configuration(struct usb_device *dev)
{
  int c, i;

  if (!dev->config)
    return;

  if (dev->rawdescriptors) {
    for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
      kfree(dev->rawdescriptors[i]);

    kfree(dev->rawdescriptors);
  }

  for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {
    struct usb_host_config *cf = &dev->config[c];

    if (!cf->interface)
      break;

    for (i = 0; i < cf->desc.bNumInterfaces; i++) {
      //struct usb_interface *ifp = cf->interface[i];
      put_device(&ifp->dev);
    }
  }
  kfree(dev->config);
}


int usb_get_configuration(struct usb_device *dev)
{
  int result;
  U32 cfgno, length;
  unsigned char *buffer;
  unsigned char *bigbuffer;
   struct usb_config_descriptor *desc;
  int i=0;

  if (dev->descriptor.bNumConfigurations > USB_MAXCONFIG) {
    usb_warn("too many configurations%s","");
    return -EINVAL;
  }

  if (dev->descriptor.bNumConfigurations < 1) {
    usb_warn("not enough configurations%s","");
    return -EINVAL;
  }

  dev->config = (struct usb_host_config *)
    kmalloc(dev->descriptor.bNumConfigurations *
    sizeof(struct usb_host_config), GFP_KERNEL);
  if (!dev->config) {
    usb_err("out of memory%s","");
    return -ENOMEM;
  }
  memset(dev->config, 0, dev->descriptor.bNumConfigurations *
    sizeof(struct usb_host_config));

  dev->rawdescriptors = (char **)kmalloc(sizeof(char *) *
    dev->descriptor.bNumConfigurations, GFP_KERNEL);
  if (!dev->rawdescriptors) {
    usb_err("out of memory%s","");
    return -ENOMEM;
  }

  buffer = (unsigned char*) kmalloc(8, GFP_KERNEL);
  if (!buffer) {
    usb_err("unable to allocate memory for configuration descriptors%s","");
    return -ENOMEM;
  }
  desc = (struct usb_config_descriptor *)buffer;

  for (cfgno = 0; cfgno < dev->descriptor.bNumConfigurations; cfgno++) {
    result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, buffer, 8);
    if (result < 8) {
      if (result < 0)
      {
        usb_err("unable to get descriptor%s","");
      }
      else {
        usb_err("config descriptor too short (expected %i, got %i)", 8, result);
        result = -EINVAL;
      }
      goto err;
    }

    length = LE16ToCPU(desc->wTotalLength);
    bigbuffer = (unsigned char*) kmalloc(length, GFP_KERNEL);
    if (!bigbuffer) {
      do
      {
        length = LE16ToCPU(desc->wTotalLength);
        i++;
      }while(length == 0 );
      usb_err("unable to allocate (len=%d) memory for configuration descriptors, i=%d",length,i);
      result = -ENOMEM;
      goto err;
    }

    result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, bigbuffer, length);
    if (result < 0) {
      usb_err("couldn't get all of config descriptors%s","");
      kfree(bigbuffer);
      goto err;
    }

    if ((U32)result < length) {
      usb_err("config descriptor too short (expected %i, got %i)", length, result);
      result = -EINVAL;
      kfree(bigbuffer);
      goto err;
    }

    dev->rawdescriptors[cfgno] = (char*) bigbuffer;

    result = usb_parse_configuration(dev, &dev->config[cfgno], (char*) bigbuffer);
    if (result > 0) {
      dbg("descriptor data left%s","");
    }
    else if (result < 0) {
      result = -EINVAL;
      goto err;
    }
  }

  kfree(buffer);
  return 0;
err:
  kfree(buffer);
  dev->descriptor.bNumConfigurations = cfgno;
  return result;
}





