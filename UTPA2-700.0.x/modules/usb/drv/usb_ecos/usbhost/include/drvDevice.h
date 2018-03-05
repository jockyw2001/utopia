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

#ifndef _Device_H_
#define _Device_H_

#include "MsCommon.h"
#include "drvOSAPI.h"
#include "drvTypes.h"
#include "drvList.h"
#include "drvPorts.h"

#define DEVICE_NAME_SIZE  24
#define BUS_ID_SIZE       24

#ifndef U16
#define U16 MS_U16
#endif


struct device_s;

struct device_driver {
  char      name[DEVICE_NAME_SIZE];
  struct bus_type    * bus;

  struct list_head  bus_list; // driver's bus's drivers list, added by Pliauo
  struct list_head  devices;  // driver's devices

  int  (*probe)  (struct device_s * dev);
  int  (*remove)  (struct device_s * dev);
  void (*shutdown)  (struct device_s * dev);
  int  (*suspend)  (struct device_s * dev, U32 state, U32 level);
  int  (*resume)  (struct device_s * dev, U32 level);
}; 

// FIXME:
struct device_s {

  struct list_head node;        // node in device's parent's children list
  struct list_head bus_list;    // node in device's bus's list
  struct list_head driver_list; // Node in device's driver's list
  struct list_head children;    // List of child devices

  
  struct device_s   * parent;

  //only reserve refcount in kobj
  int   refcount;
  char  name[DEVICE_NAME_SIZE];  /* descriptive ascii string */
  char  bus_id[BUS_ID_SIZE];  /* position on parent bus */

  struct bus_type  * bus;    /* type of bus device is on */
  struct device_driver *driver;  /* which driver has allocated this device */
  void    *driver_data;  /* data private to the driver */

  unsigned char *saved_state;  /* saved device state */
  cyg_uint64    *dma_mask;  /* dma mask (if dma'able device) */

  void  (*release)(struct device_s * dev);
}; 

static __inline__ void
dev_set_drvdata (struct device_s *dev, void *data)
{
  dev->driver_data = data;
}

static __inline__ void *
dev_get_drvdata (struct device_s *dev)
{
  return dev->driver_data;
}

extern void device_initialize(struct device_s *dev);

/* debugging and troubleshooting/diagnostic helpers. */
#define dev_printk(level, dev, format, ...)  \
  printk(format, __VA_ARGS__)

#ifdef DEV_DBG
#define dev_dbg(dev, format, ...)    \
  dev_printk("KERN_DBG", dev , format , __VA_ARGS__)
#else
#define dev_dbg(dev, format, ...) do {} while (0)
#endif

#ifdef DEV_ERR
#define dev_err(dev, format, ...)    \
  dev_printk("KERN_ERR" , dev , format , __VA_ARGS__)
#else
#define	dev_err(dev, format, ...) do {} while (0)
#endif  

#ifdef DEV_INFO
#define dev_info(dev, format, ...)    \
  dev_printk("KERN_INFO" , dev , format , __VA_ARGS__)
#else
#define dev_info(dev, format, ...) do {} while (0)
#endif

#ifdef DEV_WARM
#define dev_warn(dev, format, ...)    \
  dev_printk("KERN_WARNING" , dev , format , __VA_ARGS__)
#else
#define dev_warn(dev, format, ...) do {} while (0)
#endif

//#define get_device(dev_ptr)         dev_ptr->refcount++;dev_ptr   // Just return the passed pointer back
static __inline__ struct device_s *get_device(struct device_s *dev_ptr)
{
  atomic_inc(&dev_ptr->refcount);
  return dev_ptr;
}
#define put_device(dev_ptr)


struct usb_device_id {
  /* which fields to match against? */
  U16    match_flags;

  /* Used for product specific matches; range is inclusive */
  U16    idVendor;
  U16    idProduct;
  U16    bcdDevice_lo;
  U16    bcdDevice_hi;

  /* Used for device class matches */
  unsigned char    bDeviceClass;
  unsigned char    bDeviceSubClass;
  unsigned char    bDeviceProtocol;

  /* Used for interface class matches */
  unsigned char    bInterfaceClass;
  unsigned char    bInterfaceSubClass;
  unsigned char    bInterfaceProtocol;

  /* not matched against */
  U32  driver_info;
};

struct bus_type {
  char  name[24];
  
  struct list_head  drivers_list;
  struct list_head  devices_list;

   int    (*match)(struct device_s * dev, struct device_driver * drv);
 
   int    (*hotplug) (struct device_s *dev, char **envp,
            int num_envp, char *buffer, int buffer_size);
};


/* Some useful macros to use to create struct usb_device_id */
#define USB_DEVICE_ID_MATCH_VENDOR        0x0001
#define USB_DEVICE_ID_MATCH_PRODUCT       0x0002
#define USB_DEVICE_ID_MATCH_DEV_LO        0x0004
#define USB_DEVICE_ID_MATCH_DEV_HI        0x0008
#define USB_DEVICE_ID_MATCH_DEV_CLASS     0x0010
#define USB_DEVICE_ID_MATCH_DEV_SUBCLASS  0x0020
#define USB_DEVICE_ID_MATCH_DEV_PROTOCOL  0x0040
#define USB_DEVICE_ID_MATCH_INT_CLASS     0x0080
#define USB_DEVICE_ID_MATCH_INT_SUBCLASS  0x0100
#define USB_DEVICE_ID_MATCH_INT_PROTOCOL  0x0200

extern int  driver_register(struct device_driver * drv);
extern void driver_unregister(struct device_driver * drv);
extern void device_unregister(struct device_s * dev);
extern int  device_add(struct device_s *dev);
extern int  bus_register(struct bus_type * bus);
extern int  bus_add_device(struct device_s * dev);
extern int  bus_add_driver(struct device_driver * drv);
extern void bus_remove_driver(struct device_driver * drv);
extern void bus_unregister(struct bus_type * bus);
extern void bus_remove_device(struct device_s * dev);
#endif

