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


#ifndef __LINUX_USB_H
#define __LINUX_USB_H

#define USB_MAJOR      180

#include "drvDevice.h"
#include "drvList.h"

#include  "drvConfig.h"
#include  "drvCompiler.h"
//#include  "drvErrno.h"
#include  "drvPorts.h"
#include  "drvPCIMEM.h"

#include "drvUSB_CH9.h"
#include "drvTimer.h"
#include "drvCompletion.h"
#include "MsCommon.h"
#include "../drvUsbcommon.h"

struct usb_device;


struct usb_host_endpoint {
  struct usb_endpoint_descriptor  desc;

  unsigned char *extra;
  S32 extralen;
};

struct usb_host_interface {
  struct usb_interface_descriptor  desc;

  struct usb_host_endpoint *endpoint;

  unsigned char *extra;
  S32 extralen;
};

struct usb_interface {
  struct usb_host_interface *altsetting;

  S32 act_altsetting;
  S32 num_altsetting;
  S32 max_altsetting;

  struct usb_driver *driver;
  S32 minor;
  struct device_s dev;
};

static __inline__ void *usb_get_intfdata (struct usb_interface *intf)
{
  return dev_get_drvdata (&intf->dev);
}

static __inline__ void usb_set_intfdata (struct usb_interface *intf, void *data)
{
  dev_set_drvdata(&intf->dev, data);
}

#define USB_MAXINTERFACES  32

struct usb_host_config {
  struct usb_config_descriptor  desc;

  struct usb_interface *interface[USB_MAXINTERFACES];

  unsigned char *extra;
  S32 extralen;
};

extern struct usb_endpoint_descriptor *
  usb_epnum_to_ep_desc(struct usb_device *dev, U32 epnum);

int __usb_get_extra_descriptor(char *buffer, U32 size,
  U8 type, void **ptr);

#define usb_get_extra_descriptor(ifpoint,type,ptr)\
  __usb_get_extra_descriptor((ifpoint)->extra,(ifpoint)->extralen,\
    type,(void**)ptr)

/* -------------------------------------------------------------------------- */

struct usb_operations;

struct usb_devmap {
  U32 devicemap[128 / (8*sizeof(U32))];
};

struct usb_bus {
  struct device_s *controller;
  MS_S32 busnum;
  char *bus_name;

  S32 devnum_next;

  struct usb_devmap devmap;
  struct usb_operations *op;
  struct usb_device *root_hub;
  struct list_head bus_list;
  void *hcpriv;
  void *otgpriv;

  S32 bandwidth_allocated;
  S32 bandwidth_int_reqs;
  S32 bandwidth_isoc_reqs;

  void (*release)(struct usb_bus *bus);
};

//#define  to_usb_bus(d) container_of(d, struct usb_bus, class_dev)

/*
static __inline struct usb_bus *to_usb_bus(struct class_device *d)
{
  const struct usb_bus *__mptr = (d);
  return (char *)__mptr - offsetof(struct usb_bus,dev);
}
*/


#define USB_MAXCHILDREN    (16)

struct usb_tt;

struct usb_device {
  S32    devnum;
  char    devpath [16];
  enum usb_device_state  state;
  enum usb_device_speed  speed;

  struct usb_tt  *tt;
  S32    ttport;

  U32 toggle[2];
  U32 halted[2];

  S32 epmaxpacketin[16];
  S32 epmaxpacketout[16];

  struct usb_device *parent;
  struct usb_bus *bus;

  struct device_s dev;

  struct usb_device_descriptor descriptor;
  struct usb_host_config *config;
  struct usb_host_config *actconfig;

  char **rawdescriptors;

  S32 have_langid;
  S32 string_langid;

  void *hcpriv;

  struct list_head filelist;

  char *product;
  char *manufacturer;
  char *serial;


  S32 maxchild;
  struct usb_device *children[USB_MAXCHILDREN];
};

//#define  to_usb_device(d) container_of(d, struct usb_device, dev)
static __inline__ struct usb_device *to_usb_device(struct device_s *d)
{
  const struct usb_interface *__mptr = (struct usb_interface *)(d);
  return (struct usb_device *)( (char *)__mptr - (char *)offsetof(struct usb_device,dev));

}
//#define  to_usb_interface(d) container_of(d, struct usb_interface, dev)
static __inline__ struct usb_interface *to_usb_interface(struct device_s *d)
{
  const struct usb_interface *__mptr = (struct usb_interface *)(d);
  return (struct usb_interface *)((char *)__mptr - (char *)offsetof(struct usb_interface,dev));
}

static __inline__ struct usb_device *interface_to_usbdev(struct usb_interface *ptr)
{
  const struct usb_device *__mptr = (struct usb_device *)(ptr->dev.parent);
  return (struct usb_device *)((char *)__mptr - (char *)offsetof( struct usb_device, dev));
}

extern struct usb_device *usb_alloc_dev(struct usb_device *parent, struct usb_bus *bus);
extern struct usb_device *usb_get_dev(struct usb_device *dev);
extern void usb_put_dev(struct usb_device *dev);

extern int usb_reset_device(struct usb_device *dev);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern struct usb_device *usb_find_device(U16 vendor_id, U16 product_id);

extern int usb_get_current_frame_number (struct usb_device *usb_dev);

extern S32 usb_driver_claim_interface(struct usb_driver *driver,
      struct usb_interface *iface, void* priv);
extern S32 usb_interface_claimed(struct usb_interface *iface);
extern void usb_driver_release_interface(struct usb_driver *driver,
      struct usb_interface *iface);
const struct usb_device_id *usb_match_id(struct usb_interface *interface,
           const struct usb_device_id *id);

extern struct usb_interface *usb_find_interface(struct usb_driver *drv, int minor);
extern struct usb_interface *usb_ifnum_to_if(struct usb_device *dev, int ifnum);
 #if 0
static __inline__ S32 usb_make_path (struct usb_device *dev,char *buf, size_t size)
{
  S32 actual;
  actual = LIB_StringPrintFormatN(buf, size, "usb-%s-%s", dev->bus->bus_name, dev->devpath);
  return ((size_t) actual >= size) ? -1 : actual;
}
 #endif

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_DEVICE_ID_MATCH_DEVICE    (USB_DEVICE_ID_MATCH_VENDOR | USB_DEVICE_ID_MATCH_PRODUCT)
#define USB_DEVICE_ID_MATCH_DEV_RANGE    (USB_DEVICE_ID_MATCH_DEV_LO | USB_DEVICE_ID_MATCH_DEV_HI)
#define USB_DEVICE_ID_MATCH_DEVICE_AND_VERSION  (USB_DEVICE_ID_MATCH_DEVICE | USB_DEVICE_ID_MATCH_DEV_RANGE)
#define USB_DEVICE_ID_MATCH_DEV_INFO \
  (USB_DEVICE_ID_MATCH_DEV_CLASS | USB_DEVICE_ID_MATCH_DEV_SUBCLASS | USB_DEVICE_ID_MATCH_DEV_PROTOCOL)
#define USB_DEVICE_ID_MATCH_INT_INFO \
  (USB_DEVICE_ID_MATCH_INT_CLASS | USB_DEVICE_ID_MATCH_INT_SUBCLASS | USB_DEVICE_ID_MATCH_INT_PROTOCOL)

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define USB_DEVICE(vend,prod) \
  USB_DEVICE_ID_MATCH_DEVICE, vend, prod

#define USB_DEVICE_VER(vend,prod,lo,hi) \
  .match_flags = USB_DEVICE_ID_MATCH_DEVICE_AND_VERSION, .idVendor = (vend), .idProduct = (prod), .bcdDevice_lo = (lo), .bcdDevice_hi = (hi)

#define USB_DEVICE_INFO(cl,sc,pr) \
  USB_DEVICE_ID_MATCH_DEV_INFO,0,0,0,0,cl,sc,pr,0,0,0,0

#define USB_INTERFACE_INFO(cl,sc,pr) \
  USB_DEVICE_ID_MATCH_INT_INFO,0,0,0,0,0,0,0,cl,sc,pr,0

struct usb_driver {
  //struct module *owner;
  void *module;

  //const char *name;
  char name[16];

  int (*probe) (struct usb_interface *intf,
          const struct usb_device_id *id);

  void (*disconnect) (struct usb_interface *intf);

  int (*ioctl) (struct usb_interface *intf, U32 code, void *buf);

  const struct usb_device_id *id_table;

  struct device_driver driver;

  //struct file_operations *fops;
  unsigned int minor_base;

  //struct semaphore serialize;
};

//#define  to_usb_driver(d) container_of(d, struct usb_driver, driver)

static __inline__ struct usb_driver *to_usb_driver(struct device_driver *dev_ptr)
{
  const struct device_driver *__mptr = (dev_ptr);
  return (struct usb_driver *)( (char *)__mptr - (char *)offsetof(struct usb_driver,driver) );
}

extern struct bus_type usb_bus_type;

struct usb_class_driver {
  char name[BUS_ID_SIZE];
  struct file_operations *fops;
  //mode_t mode;
  S32 minor_base;
};

extern int usb_register(struct usb_driver *);
extern void usb_deregister(struct usb_driver *);

extern S32 usb_register_dev(struct usb_interface *intf,
          struct usb_class_driver *class_driver);
extern void usb_deregister_dev(struct usb_interface *intf,
             struct usb_class_driver *class_driver);


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/


extern MS_S32 usb_disabled(void);

#define URB_SHORT_NOT_OK  0x0001  /* report short reads as errors */
#define URB_ISO_ASAP    0x0002  /* iso-only, urb->start_frame ignored */
#define URB_NO_TRANSFER_DMA_MAP  0x0004  /* urb->transfer_dma valid on submit */
#define URB_NO_SETUP_DMA_MAP  0x0008  /* urb->setup_dma valid on submit */
#define URB_ASYNC_UNLINK  0x0010  /* usb_unlink_urb() returns asap */
#define URB_NO_FSBR    0x0020  /* UHCI-specific */
#define URB_ZERO_PACKET    0x0040  /* Finish bulk OUTs with short packet */
#define URB_NO_INTERRUPT  0x0080  /* HINT: no non-error interrupt needed */

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct usb_iso_packet_descriptor {
  U32 offset;
  U32 length;    /* expected length */
  U32 actual_length;
  U32 status;
};

struct urb;
struct pt_regs;

typedef void (*usb_complete_t)(struct urb *, struct pt_regs *);

struct urb
{
  spinlock_t lock;
  atomic_t count;
  void *hcpriv;
  struct list_head urb_list;
  S32 bandwidth;

  struct usb_device *dev;
  U32 pipe;
  S32 status;
  U32 transfer_flags;
  void *transfer_buffer;
  dma_addr_t transfer_dma;
  S32 transfer_buffer_length;
  S32 actual_length;
  unsigned char *setup_packet;
  dma_addr_t setup_dma;
  S32 start_frame;
  S32 number_of_packets;
  S32 interval;
  S32 error_count;
  S32 timeout;
  void *context;
  usb_complete_t complete;
  //__Test
  void    *SetDMABuf;	//Note, for DMA
  S32		SetDMALen;
  void	*TxDMABuf;
  S32		TxDMALen;
  struct list_head  qtd_list;       // Store qtds belong to this urb
  struct completion urb_done;	// completion for urb
  struct timer_list to_timer;	// timeout value for urb
  //__Test
  struct usb_iso_packet_descriptor iso_frame_desc[0];  /* (in) ISO ONLY */

  S32  WaitFlag_storage;  //added by jonas

};

static __inline__ void usb_fill_control_urb (struct urb *urb,
           struct usb_device *dev,
           U32 pipe,
           unsigned char *setup_packet,
           void *transfer_buffer,
           S32 buffer_length,
           usb_complete_t complete,
           void *context)
{
  spin_lock_init(&urb->lock);
  urb->dev = dev;
  urb->pipe = pipe;
  urb->setup_packet = setup_packet;
  urb->transfer_buffer = transfer_buffer;
  urb->transfer_buffer_length = buffer_length;
  urb->complete = complete;
  urb->context = context;
  urb->SetDMABuf = NULL;
  urb->SetDMALen = 0;
  urb->TxDMABuf = NULL;
  urb->TxDMALen = 0;
}

static __inline__ void usb_fill_bulk_urb (struct urb *urb,
              struct usb_device *dev,
              U32 pipe,
              void *transfer_buffer,
              S32 buffer_length,
              usb_complete_t complete,
              void *context)
{
  spin_lock_init(&urb->lock);
  urb->dev = dev;
  urb->pipe = pipe;
  urb->transfer_buffer = transfer_buffer;
  urb->transfer_buffer_length = buffer_length;
  urb->complete = complete;
  urb->context = context;
  urb->SetDMABuf = NULL;
  urb->SetDMALen = 0;
  urb->TxDMABuf = NULL;
  urb->TxDMALen = 0;
}

static __inline__ void usb_fill_int_urb (struct urb *urb,
             struct usb_device *dev,
             U32 pipe,
             void *transfer_buffer,
             S32 buffer_length,
             usb_complete_t complete,
             void *context,
             S32 interval)
{
  spin_lock_init(&urb->lock);
  urb->dev = dev;
  urb->pipe = pipe;
  urb->transfer_buffer = transfer_buffer;
  urb->transfer_buffer_length = buffer_length;
  urb->complete = complete;
  urb->context = context;
  if (dev->speed == USB_SPEED_HIGH)
    urb->interval = 1 << (interval - 1);
  else
    urb->interval = interval;
  urb->start_frame = -1;
  urb->SetDMABuf = NULL;
  urb->SetDMALen = 0;
  urb->TxDMABuf = NULL;
  urb->TxDMALen = 0;
}

extern void usb_init_urb(struct urb *urb);
extern struct urb *usb_alloc_urb(int iso_packets, int mem_flags);
extern void usb_free_urb(struct urb *urb);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define usb_put_urb usb_free_urb
extern struct urb *usb_get_urb(struct urb *urb);
extern int usb_submit_urb(struct urb *urb, int mem_flags);
extern int usb_unlink_urb(struct urb *urb);

#define HAVE_USB_BUFFERS
void *usb_buffer_alloc (struct usb_device *dev, size_t size,
  int mem_flags, dma_addr_t *dma);
void usb_buffer_free (struct usb_device *dev, size_t size,
  void *addr, dma_addr_t dma);

struct urb *usb_buffer_map (struct urb *urb);
void usb_buffer_dmasync (struct urb *urb);
void usb_buffer_unmap (struct urb *urb);

struct scatterlist;
S32 usb_buffer_map_sg (struct usb_device *dev, U32 pipe,
    struct scatterlist *sg, S32 nents);
void usb_buffer_dmasync_sg (struct usb_device *dev, U32 pipe,
    struct scatterlist *sg, S32 n_hw_ents);
void usb_buffer_unmap_sg (struct usb_device *dev, U32 pipe,
    struct scatterlist *sg, S32 n_hw_ents);


extern int usb_control_msg(struct usb_device *dev, unsigned int pipe,
  unsigned char request, unsigned char requesttype, unsigned short value, unsigned short index,
  void *data, unsigned short size, int timeout);
extern int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe,
  void *data, int len, int *actual_length,
  int timeout);


#define USB_CTRL_GET_TIMEOUT  5
#define USB_CTRL_SET_TIMEOUT  5

#define PIPE_ISOCHRONOUS    0
#define PIPE_INTERRUPT      1
#define PIPE_CONTROL      2
#define PIPE_BULK      3

#define usb_maxpacket(dev, pipe, out)  (out \
        ? (dev)->epmaxpacketout[usb_pipeendpoint(pipe)] \
        : (dev)->epmaxpacketin [usb_pipeendpoint(pipe)] )

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define usb_pipein(pipe)  ((pipe) & USB_DIR_IN)
#define usb_pipeout(pipe)  (!usb_pipein(pipe))
#define usb_pipedevice(pipe)  (((pipe) >> 8) & 0x7f)
#define usb_pipeendpoint(pipe)  (((pipe) >> 15) & 0xf)
#define usb_pipetype(pipe)  (((pipe) >> 30) & 3)
#define usb_pipeisoc(pipe)  (usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipeint(pipe)  (usb_pipetype((pipe)) == PIPE_INTERRUPT)
#define usb_pipecontrol(pipe)  (usb_pipetype((pipe)) == PIPE_CONTROL)
#define usb_pipebulk(pipe)  (usb_pipetype((pipe)) == PIPE_BULK)

#define usb_endpoint_running(dev, ep, out) ((dev)->halted[out] &= ~(1 << (ep)))
#define usb_endpoint_halted(dev, ep, out) ((dev)->halted[out] & (1 << (ep)))


static __inline__ U32 __create_pipe(struct usb_device *dev, U32 endpoint)
{
  return (dev->devnum << 8) | (endpoint << 15);
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
#define usb_gettoggle(dev, ep, out) (((dev)->toggle[out] >> (ep)) & 1)
#define  usb_dotoggle(dev, ep, out)  ((dev)->toggle[out] ^= (1 << (ep)))
#define usb_settoggle(dev, ep, out, bit) ((dev)->toggle[out] = ((dev)->toggle[out] & ~(1 << (ep))) | ((bit) << (ep)))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define usb_sndctrlpipe(dev,endpoint)  (((U32)PIPE_CONTROL << 30) | __create_pipe(dev,endpoint))
#define usb_rcvctrlpipe(dev,endpoint)  (((U32)PIPE_CONTROL << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndisocpipe(dev,endpoint)  (((U32)PIPE_ISOCHRONOUS << 30) | __create_pipe(dev,endpoint))
#define usb_rcvisocpipe(dev,endpoint)  (((U32)PIPE_ISOCHRONOUS << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndbulkpipe(dev,endpoint)  (((U32)PIPE_BULK << 30) | __create_pipe(dev,endpoint))
#define usb_rcvbulkpipe(dev,endpoint)  (((U32)PIPE_BULK << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndintpipe(dev,endpoint)  (((U32)PIPE_INTERRUPT << 30) | __create_pipe(dev,endpoint))
#define usb_rcvintpipe(dev,endpoint)  (((U32)PIPE_INTERRUPT << 30) | __create_pipe(dev,endpoint) | USB_DIR_IN)

extern int usb_get_descriptor(struct usb_device *dev, unsigned char desctype,
  unsigned char descindex, void *buf, int size);
extern int usb_get_device_descriptor(struct usb_device *dev);
extern int usb_get_status(struct usb_device *dev,
  int type, int target, void *data);
extern int usb_get_string(struct usb_device *dev,
  unsigned short langid, unsigned char index, void *buf, int size);
extern int usb_string(struct usb_device *dev, int index,
  char *buf, size_t size);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_clear_halt(struct usb_device *dev, int pipe);
extern int usb_reset_configuration(struct usb_device *dev);
extern int usb_set_configuration(struct usb_device *dev, int configuration);
extern int usb_set_interface(struct usb_device *dev, int ifnum, int alternate);

void usb_show_device_descriptor(struct usb_device_descriptor *);
void usb_show_config_descriptor(struct usb_config_descriptor *);
void usb_show_interface_descriptor(struct usb_interface_descriptor *);
void usb_show_endpoint_descriptor(struct usb_endpoint_descriptor *);
void usb_show_device(struct usb_device *);
void usb_show_string(struct usb_device *dev, char *id, int index);

#ifdef DBG
#define dbg    printk
#else
#define dbg(format, ...) do {} while (0)
#endif

#ifdef SHOW_MSG
#define usb_msg	    printk
#else
#define usb_msg(format,...) do {} while (0)
#endif


#ifdef VERBOSE_DEBUG
#define vdbg dbg
#else
#define vdbg(format, ...) do {} while (0)
#endif

#ifdef KERNEL_DEBUG
#define kdbg dbg
#else
#define kdbg(format, ...) do {} while (0)
#endif



#define usb_err(format, ...) printk(format, __VA_ARGS__)
#define usb_info(format, ...) printk(format, __VA_ARGS__)
#define usb_warn(format, ...) printk(format, __VA_ARGS__)


struct usb_otg_ap_operations {
  void (*gadget_init)(void *);
  S32 (*gadget_bh_init)(void);
  S32 (*host_init)(void *,U32);
  S32 (*host_bh_init)(void *);
  S32 (*gadget_main) (void *,U8 ,U8 );
  void (*host_main) (void);
  S32 (*gadget_exit) (void *);
  S32 (*gadget_bh_exit) (void);
  S32 (*host_exit) (void *);
  S32 (*host_bh_exit) (void *);
};

// Added by Peter Liao, 2003/09/08
extern int USB_core_lock;
extern int Timer_task_lock;

#ifdef CONFIG_OTG
extern BOOL gOPT_Device_attached;
static __inline__ BOOL Is_OPT_Dev_Attach()
{
	return gOPT_Device_attached;
}

static __inline__ void Set_OPT_Dev_Attach()
{
	gOPT_Device_attached = TRUE;
}

static __inline__ void Set_OPT_Dev_Unattach()
{
	gOPT_Device_attached = FALSE;
}

#endif

#define USB_HOST_ROLE     0
#define USB_DEVICE_ROLE   1
//Used by power management related funciton (ex. suspend, wake up, etc.)
#define USB_PWM_SUSPEND   0
#define USB_OTG_SUSPEND   1

#define USB_STOR_STRING_LEN 32

extern S32 usb_role_change(void);
extern S32 usb_get_cur_role(void);
extern S32 usb_get_role_ID(void);
extern void usb_drop_Vbus(void);
extern void usb_drive_Vbus(void);
extern S32 usb_srp_section(void);
extern S32 usb_hnp_section(void);
#endif

