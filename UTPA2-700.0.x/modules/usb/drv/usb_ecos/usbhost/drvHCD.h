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

#ifndef _CORE_HCD_H_
#define _CORE_HCD_H_

#include "MsCommon.h"
#include "include/drvUSB.h"

#define USB_PID_UNDEF_0      0xf0
#define USB_PID_OUT      0xe1
#define USB_PID_ACK      0xd2
#define USB_PID_DATA0      0xc3
#define USB_PID_PING      0xb4
#define USB_PID_SOF      0xa5
#define USB_PID_NYET      0x96
#define USB_PID_DATA2      0x87
#define USB_PID_SPLIT      0x78
#define USB_PID_IN      0x69
#define USB_PID_NAK      0x5a
#define USB_PID_DATA1      0x4b
#define USB_PID_PREAMBLE    0x3c
#define USB_PID_ERR      0x3c
#define USB_PID_SETUP      0x2d
#define USB_PID_STALL      0x1e
#define USB_PID_MDATA      0x0f

struct usb_hcd {

  struct usb_bus    self;

  //const char    *product_desc;  /* product/vendor string */
  char    product_desc[24];  /* product/vendor string */
  //const char    *description;  /* "ehci-hcd" etc */
  char    description[24];  /* "ehci-hcd" etc */

  struct timer_list  rh_timer;
  struct urb		*status_urb;
  struct list_head  dev_list;

  struct hc_driver  *driver;

#if 1 //def ROOTHUB_INTERRUPT_MODE
  unsigned        rh_registered:1;/* is root hub registered? */

  /* The next flag is a stopgap, to be removed when all the HCDs
   * support the new root-hub polling mechanism. */
  unsigned        uses_new_polling:1;
  unsigned        poll_rh:1;  /* poll for rh status? */
  unsigned        poll_pending:1; /* status has changed? */
  unsigned        wireless:1; /* Wireless USB HCD */
  unsigned        authorized_default:1;
  unsigned        has_tt:1;   /* Integrated TT in root hub */
#endif

  int      irq;
  U32      *regs;
  struct device_s    *controller;

  struct pci_dev    *pdev;

#ifdef  CONFIG_PCI
  int      region;
  U32      pci_state [16];
  atomic_t    resume_count;
#endif

#define HCD_BUFFER_POOLS  5
  struct pci_pool    *pool [HCD_BUFFER_POOLS];

  int      state;
#define  __ACTIVE    0x01
#define  __SLEEPY    0x02
#define  __SUSPEND    0x04
#define  __TRANSIENT    0x80

#define  USB_STATE_HALT    0
#define  USB_STATE_RUNNING  (__ACTIVE)
#define  USB_STATE_READY    (__ACTIVE|__SLEEPY)
#define  USB_STATE_QUIESCING  (__SUSPEND|__TRANSIENT|__ACTIVE)
#define  USB_STATE_RESUMING  (__SUSPEND|__TRANSIENT)
#define  USB_STATE_SUSPENDED  (__SUSPEND)

#define  HCD_IS_RUNNING(state) ((state) & __ACTIVE)
#define  HCD_IS_SUSPENDED(state) ((state) & __SUSPEND)

#if 1 // new to collect hcd related variables
    U32 host_id;
    U32 pre_sts; // preserve the previous state (ehci_hub_status_data)
    U32 pre_temp; // preserve the previous state (ehci_hub_control_EX)
    S32 USBWaitFlg; // OS wait flag
    U32 badDeviceCnt; // for Philips mp3 device
    BOOL isBadDevice; // backward compatible
    BOOL isRootHubPortReset; // backward compatible, may be omitted
    struct list_head *phub_event;
   struct s_gVar4UsbPort *pgVar; // link to global variable struct, JF
#endif
};

struct usb_otgd {

  struct usb_bus    *bus;

  //const char    *product_desc;  /* product/vendor string */
  char    product_desc[24];  /* product/vendor string */
  //const char    *description;    /* "FOTG100_DRIVER" etc */
  char    description[24];    /* "FOTG100_DRIVER" etc */

  struct otg_driver  *driver;
  int      irq;
  U32     *regs;
  struct device    *controller;
  struct usb_otg_ap_operations ap_op;
  struct usb_otg_descriptor desc;
};

static __inline__ struct usb_bus *hcd_to_bus (struct usb_hcd *hcd)
{
  return &hcd->self;
}


struct hcd_dev {
  struct list_head  dev_list;
  struct list_head  urb_list;

  void      *ep[32];
};


struct hcd_timeout {
  struct list_head  timeout_list;
  struct timer_list  timer;
};

struct usb_operations {
  int (*allocate)(struct usb_device *);
  int (*deallocate)(struct usb_device *);
  int (*get_frame_number) (struct usb_device *usb_dev);
  int (*submit_urb) (struct urb *urb, int mem_flags);
  int (*unlink_urb) (struct urb *urb);

  void *(*buffer_alloc)(struct usb_bus *bus, size_t size,
      int mem_flags,
      dma_addr_t *dma);
  void (*buffer_free)(struct usb_bus *bus, size_t size,
      void *addr, dma_addr_t dma);

  void (*disable)(struct usb_device *udev, int bEndpointAddress);
};


struct pt_regs;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct hc_driver {
  char  description[24];

  void  (*irq) (struct usb_hcd *hcd, struct pt_regs *regs);
  int  flags;
#define  HCD_MEMORY  0x0001
#define  HCD_USB11  0x0010
#define  HCD_USB2  0x0020

  int  (*reset) (struct usb_hcd *hcd);
  int  (*start) (struct usb_hcd *hcd);
  int  (*restart) (struct usb_hcd *hcd);

  int  (*suspend) (struct usb_hcd *hcd, U32 state);

  int  (*resume) (struct usb_hcd *hcd);

  void  (*stop) (struct usb_hcd *hcd);

  int  (*get_frame_number) (struct usb_hcd *hcd);

  struct usb_hcd  *(*hcd_alloc) (void);

  void  (*hcd_free) (struct usb_hcd *hcd);

  int  (*urb_enqueue) (struct usb_hcd *hcd, struct urb *urb,
          int mem_flags);
  int  (*urb_dequeue) (struct usb_hcd *hcd, struct urb *urb);

  void   (*endpoint_disable)(struct usb_hcd *hcd,
      struct hcd_dev *dev, int bEndpointAddress);

  int    (*hub_status_data) (struct usb_hcd *hcd, char *buf);
  int    (*hub_control) (struct usb_hcd *hcd,
        U16 typeReq, U16 wValue, U16 wIndex,
        char *buf, U16 wLength);
};

struct otg_driver {
  char  description[24];  /* "FOTG100_DRIVER" etc */

  void  (*irq) (struct usb_otgd *otgd, struct pt_regs *regs);

  int  flags;

  int  (*get_role_ID) (struct usb_otgd *otgd);
  int  (*get_cur_role) (struct usb_otgd *otgd);
  void   (*drop_Vbus) (struct usb_otgd *otgd);
  void   (*drive_Vbus) (struct usb_otgd *otgd);
  int  (*role_change) (struct usb_otgd *otgd);
  int  (*srp_section) (struct usb_otgd *otgd);
  int  (*hnp_section) (struct usb_otgd *otgd);

  void  (*start) (struct usb_otgd *otgd);

  void  (*stop) (struct usb_otgd *otgd);

  struct usb_otgd  *(*otgd_alloc) (void);
  void  (*otgd_free) (struct usb_otgd *otgd);
};

extern void usb_hcd_giveback_urb (struct usb_hcd *hcd, struct urb *urb, struct pt_regs *regs);
extern void usb_bus_init (struct usb_bus *bus);
extern void usb_rh_status_dequeue (struct usb_hcd *hcd, struct urb *urb);

#ifdef CONFIG_PCI
struct pci_dev;
struct pci_device_id;
extern int usb_hcd_pci_probe (struct pci_dev *dev,
        const struct pci_device_id *id);
extern void usb_hcd_pci_remove (struct pci_dev *dev);

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifdef CONFIG_PM
extern int usb_hcd_pci_suspend (struct pci_dev *dev, U32 state);
extern int usb_hcd_pci_resume (struct pci_dev *dev);
#endif /* CONFIG_PM */

#endif /* CONFIG_PCI */

int hcd_buffer_create (struct usb_hcd *hcd);
void hcd_buffer_destroy (struct usb_hcd *hcd);

void *hcd_buffer_alloc (struct usb_bus *bus, size_t size,
  int mem_flags, dma_addr_t *dma);
void hcd_buffer_free (struct usb_bus *bus, size_t size,
  void *addr, dma_addr_t dma);

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
extern struct usb_operations usb_hcd_operations;
extern irqreturn_t usb_hcd_irq (int irq, void *__hcd, struct pt_regs *r);
extern void usb_hc_died (struct usb_hcd *hcd);

extern int usb_new_device(struct usb_device *dev, struct device_s *parent);
extern void usb_choose_address(struct usb_device *dev);
extern void usb_disconnect(struct usb_device **);

extern int usb_get_configuration(struct usb_device *dev);
extern void usb_destroy_configuration(struct usb_device *dev);
extern int usb_set_address(struct usb_device *dev);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define usb_snddefctrl(dev)    (((U32)PIPE_CONTROL << 30))
#define usb_rcvdefctrl(dev)    (((U32)PIPE_CONTROL << 30) | USB_DIR_IN)

#include "drvHUB.h"

#define DeviceRequest \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_DEVICE)<<8)
#define DeviceOutRequest \
  ((USB_DIR_OUT|USB_TYPE_STANDARD|USB_RECIP_DEVICE)<<8)

#define InterfaceRequest \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define EndpointRequest \
  ((USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)
#define EndpointOutRequest \
  ((USB_DIR_OUT|USB_TYPE_STANDARD|USB_RECIP_INTERFACE)<<8)

#define DEVICE_REMOTE_WAKEUP  1
#define ENDPOINT_HALT    0

#define ClearHubFeature    (0x2000 | USB_REQ_CLEAR_FEATURE)
#define ClearPortFeature  (0x2300 | USB_REQ_CLEAR_FEATURE)
#define GetHubDescriptor  (0xa000 | USB_REQ_GET_DESCRIPTOR)
#define GetHubStatus    (0xa000 | USB_REQ_GET_STATUS)
#define GetPortStatus    (0xa300 | USB_REQ_GET_STATUS)
#define SetHubFeature    (0x2000 | USB_REQ_SET_FEATURE)
#define SetPortFeature    (0x2300 | USB_REQ_SET_FEATURE)
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


#define FRAME_TIME_USECS  1000L
#define BitTime(bytecount)  (7 * 8 * bytecount / 6)  /* with integer truncation */

#define NS_TO_US(ns)  ((ns + 500L) / 1000L)

extern void usb_claim_bandwidth (struct usb_device *dev, struct urb *urb,
    int bustime, int isoc);
extern void usb_release_bandwidth (struct usb_device *dev, struct urb *urb,
    int isoc);

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
#define BW_HOST_DELAY  1000L    /* nanoseconds */
#define BW_HUB_LS_SETUP  333L    /* nanoseconds */

#define FRAME_TIME_BITS         12000L    /* frame = 1 millisecond */
#define FRAME_TIME_MAX_BITS_ALLOC  (90L * FRAME_TIME_BITS / 100L)
#define FRAME_TIME_MAX_USECS_ALLOC  (90L * FRAME_TIME_USECS / 100L)

extern int usb_check_bandwidth (struct usb_device *dev, struct urb *urb);

#define USB2_HOST_DELAY  5  /* nsec, guess */
#define HS_USECS(bytes) NS_TO_US ( ((55 * 8 * 2083)/1000) \
  + ((2083UL * (3167 + BitTime (bytes)))/1000) \
  + USB2_HOST_DELAY)
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
#define HS_USECS_ISO(bytes) NS_TO_US ( ((long)(38 * 8 * 2.083)) \
  + ((2083UL * (3167 + BitTime (bytes)))/1000) \
  + USB2_HOST_DELAY)

extern int usb_calc_bus_time (int speed, int is_input,
      int isoc, int bytecount);

/*-------------------------------------------------------------------------*/

extern struct usb_bus *usb_alloc_bus (struct usb_operations *);

extern int usb_register_bus (struct usb_bus *);
extern void usb_deregister_bus (struct usb_bus *);

extern int usb_register_root_hub (struct usb_device *usb_dev,
    struct device_s *parent_dev);
extern int register_root_hub (struct usb_hcd *hcd);

extern int usb_register_otg_to_bus(struct usb_otgd *otgd, struct device *hc);
extern int usb_unregister_otg_from_bus(struct usb_otgd *otgd);

static __inline__ int hcd_register_root (struct usb_hcd *hcd)
{
#ifdef ROOTHUB_INTERRUPT_MODE
    return register_root_hub(hcd);
#else
  return usb_register_root_hub (
    hcd_to_bus (hcd)->root_hub, hcd->controller);
#endif
}

extern struct list_head usb_bus_list;

extern struct usb_bus *usb_bus_get (struct usb_bus *bus);
extern void usb_bus_put (struct usb_bus *bus);

extern int usb_find_interface_driver (struct usb_device *dev,
  struct usb_interface *interface);

#define usb_endpoint_halt(dev, ep, out) ((dev)->halted[out] |= (1 << (ep)))

#define usb_endpoint_out(ep_dir)  (!((ep_dir) & USB_DIR_IN))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifdef CONFIG_USB_DEVICEFS

extern void usbfs_add_bus(struct usb_bus *bus);
extern void usbfs_remove_bus(struct usb_bus *bus);
extern void usbfs_add_device(struct usb_device *dev);
extern void usbfs_remove_device(struct usb_device *dev);
extern void usbfs_update_special (void);

extern int usbfs_init(void);
extern void usbfs_cleanup(void);

#else /* CONFIG_USB_DEVICEFS */

static __inline__ void usbfs_add_bus(struct usb_bus *bus) {}
static __inline__ void usbfs_remove_bus(struct usb_bus *bus) {}
static __inline__ void usbfs_add_device(struct usb_device *dev) {}
static __inline__ void usbfs_remove_device(struct usb_device *dev) {}
static __inline__ void usbfs_update_special (void) {}

static __inline__ int usbfs_init(void) { return 0; }
static __inline__ void usbfs_cleanup(void) { }

#endif /* CONFIG_USB_DEVICEFS */

#define bitmap   DeviceRemovable


#define  RUN_CONTEXT (in_irq () ? "in_irq" \
    : (in_interrupt () ? "in_interrupt" : "can sleep"))


#endif
