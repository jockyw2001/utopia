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

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#include <MsCommon.h>
#ifdef CONFIG_USB_DEBUG
#define DEBUG
#endif


#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvIO.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"
#include  "include/drvCompletion.h"
#include  "include/drvOSAPI.h"
#include  "include/drvKernel.h"
#include  "include/drvBitops.h"
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_cache.h>

// USB related header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "drvEHCI.h"
#include "drvMassStor.h"

extern void Chip_Flush_Memory(void);


#if 1


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
LIST_HEAD (usb_bus_list);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_MAXBUS    32
struct usb_busmap {
  U32 busmap [USB_MAXBUS / (8*sizeof (U32))];
};
static struct usb_busmap busmap;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

/*-------------------------------------------------------------------------*/

#define KERNEL_REL  1
#define KERNEL_VER  10

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static const unsigned char usb2_rh_dev_descriptor [18] = {
  0x12,       
  0x01,       
  0x00, 0x02, 

  0x09,     
  0x00,     
  0x01,      
  0x08,      

  0x00, 0x00, 
   0x00, 0x00, 
  KERNEL_VER, KERNEL_REL, 

  0x03,       
  0x02,      
  0x01,      
  0x01        
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static const unsigned char usb11_rh_dev_descriptor [18] = {
  0x12,       
  0x01,       
  0x10, 0x01, 

  0x09,      
  0x00,      
  0x00,       
  0x08,       

  0x00, 0x00, 
   0x00, 0x00, 
  KERNEL_VER, KERNEL_REL, 

  0x03,      
  0x02,       
  0x01,       
  0x01        
};



/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static const unsigned char fs_rh_config_descriptor [] = {

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x09,       
  0x02,       
  0x19, 0x00, 
  0x01,       
  0x01,       
  0x00,       
  0x40,       
         /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x00,       

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x09,       
  0x04,       
  0x00,       
  0x00,       
  0x01,       
  0x09,       
  0x00,       
  0x00,       
  0x00,       

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x07,      
  0x05,      
  0x81,      
   0x03,     
   0x02, 0x00,
  0xff        
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static const unsigned char hs_rh_config_descriptor [] = {

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x09,       
  0x02,       
  0x19, 0x00, 
  0x01,       
  0x01,       
  0x00,       
  0x40,       
       /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
       
       
       
  0x00,         

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x09,       
  0x04,       
  0x00,       
  0x00,       
  0x01,       
  0x09,       
  0x00,       
  0x00,       
  0x00,       

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  0x07,       
  0x05,       
  0x81,       
   0x03,      
   0x02, 0x00,
  0x0c        
};

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int ascii2utf (char *s, unsigned char *utf, int utfmax)
{
  int retval;

  for (retval = 0; *s && utfmax > 1; utfmax -= 2, retval += 2) {
    *utf++ = *s++;
    *utf++ = 0;
  }
  return retval;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int rh_string (
  int    id,
  struct usb_hcd  *hcd,
  unsigned char    *data,
  int    len
) {
  char buf [100];

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (id == 0) {
    *data++ = 4; *data++ = 3;  
    *data++ = 0x09; *data++ = 0x04;  
    return 4;

 /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } else if (id == 1) {
    strncpy(buf, hcd->self.bus_name, sizeof(buf));

 /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } else if (id == 2) {
                strncpy(buf, hcd->product_desc, sizeof(buf));
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } else if (id == 3) {
      USB_sprintf(buf,"%s %s %s", "Non-OS", "1.0",
      hcd->description);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } else
      return 0;
  data [0] = 2 * (strlen(buf) + 1);
  data [1] = 3;  /* type == string */
  return 2 + ascii2utf (buf, data + 2, len - 2);
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
static int rh_call_control (struct usb_hcd *hcd, struct urb *urb)
{
  struct usb_ctrlrequest *cmd = (struct usb_ctrlrequest *) urb->setup_packet;
   U16    typeReq, wValue, wIndex, wLength;
  const unsigned char  *bufp = 0;
  unsigned char    *ubuf = (unsigned char*) urb->transfer_buffer;
  int    len = 0;
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  urb->hcpriv = hcd;  

  typeReq  = (cmd->bRequestType << 8) | cmd->bRequest;
  wValue   = LE16ToCPU (cmd->wValue);
  wIndex   = LE16ToCPU (cmd->wIndex);
  wLength  = LE16ToCPU (cmd->wLength);

  if (wLength > urb->transfer_buffer_length)
    goto error;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb->status = 0;
  urb->actual_length = wLength;
  switch (typeReq) {

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case DeviceRequest | USB_REQ_GET_STATUS:  	
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    ubuf [0] = 1;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    ubuf [1] = 0;      
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  case DeviceOutRequest | USB_REQ_CLEAR_FEATURE:
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/	
  case DeviceOutRequest | USB_REQ_SET_FEATURE:
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/	
    dev_dbg (hcd->controller, "no device features set yet%s","\n");
    break;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  case DeviceRequest | USB_REQ_GET_CONFIGURATION:
    ubuf [0] = 1;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case DeviceOutRequest | USB_REQ_SET_CONFIGURATION:
    break;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  case DeviceRequest | USB_REQ_GET_DESCRIPTOR:
    switch (wValue & 0xff00) {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/	
    case USB_DT_DEVICE << 8:
    	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (hcd->driver->flags & HCD_USB2)
        bufp = usb2_rh_dev_descriptor;
      else if (hcd->driver->flags & HCD_USB11)
        bufp = usb11_rh_dev_descriptor;
      else
        goto error;
      len = 18;
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
    case USB_DT_CONFIG << 8:
      if (hcd->driver->flags & HCD_USB2) {
        bufp = hs_rh_config_descriptor;
        len = sizeof hs_rh_config_descriptor;
      } else {
        bufp = fs_rh_config_descriptor;
        len = sizeof fs_rh_config_descriptor;
      }
      break;
    case USB_DT_STRING << 8:
      urb->actual_length = rh_string (
        wValue & 0xff, hcd,
        ubuf, wLength);
      break;
    default:
      goto error;
    }
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
  case DeviceRequest | USB_REQ_GET_INTERFACE:
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
    ubuf [0] = 0;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case DeviceOutRequest | USB_REQ_SET_INTERFACE:
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
    break;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  case DeviceOutRequest | USB_REQ_SET_ADDRESS:
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    dev_dbg (hcd->controller, "[root hub] device address %d\n",
      wValue);
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
  case EndpointRequest | USB_REQ_GET_STATUS:
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    ubuf [0] = 0;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
    ubuf [1] = 0;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case EndpointOutRequest | USB_REQ_CLEAR_FEATURE:
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  case EndpointOutRequest | USB_REQ_SET_FEATURE:
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
    dev_dbg (hcd->controller, "[no endpoint features yet]%s","\n");
    break;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  default:
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->status = hcd->driver->hub_control (hcd,
      typeReq, wValue, wIndex,
      (char*) ubuf, wLength);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/    
    break;
error:
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->status = -EPIPE;
    dev_dbg (hcd->controller, "[unsupported hub control message] (maxchild %d)\n",
        urb->dev->maxchild);
  }
  if (urb->status) {
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->actual_length = 0;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    dev_dbg (hcd->controller, "CTRL: TypeReq=0x%x val=0x%x idx=0x%x len=%d ==> %d\n",
      typeReq, wValue, wIndex, wLength, urb->status);
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (bufp) {
    if (urb->transfer_buffer_length < len)
      len = urb->transfer_buffer_length;
    urb->actual_length = len;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    memcpy(ubuf, bufp, len);
  }

 /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  local_irq_save (flags);
  usb_hcd_giveback_urb (hcd, urb, NULL);
  local_irq_restore (flags);
  return 0;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#ifndef ROOTHUB_INTERRUPT_MODE
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

static void rh_report_status (U32 ptr);
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
static int rh_status_urb (struct usb_hcd *hcd, struct urb *urb)
{
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int  len = 1 + (urb->dev->maxchild / 8);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (hcd->rh_timer.data
      || urb->status != -EINPROGRESS
      || urb->transfer_buffer_length < len
      || !HCD_IS_RUNNING (hcd->state)) {
    dev_dbg (hcd->controller,
        "not queuing rh status urb, stat %d\n",
        urb->status);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/    
    return -EINVAL;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  init_timer (&hcd->rh_timer);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd->rh_timer.function = rh_report_status;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd->rh_timer.data = (U32) &hcd->rh_timer;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd->rh_timer.data_ex = (U32) urb;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd->rh_timer.tmout_value = ROOTHUB_TIMER_INTERVAL;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  add_timer (&hcd->rh_timer);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb->hcpriv = hcd;  
  return 0;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void rh_report_status (U32 ptr)
{
  struct timer_list *pTimer = (struct timer_list *) ptr;
  struct urb  *urb;
  struct usb_hcd  *hcd;
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int length = 0;
  //U32  flags;
  urb = (struct urb *) pTimer->data_ex;
  local_irq_save (flags);
  spin_lock (&urb->lock);

 /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  if (!urb->dev
      || urb->status != -EINPROGRESS
      || (hcd = (struct usb_hcd*) urb->dev->bus->hcpriv) == 0) {
    spin_unlock (&urb->lock);
    local_irq_restore (flags);
	diag_printf("rh_report_status: it's unlinked. urb->status: 0x%\n", urb->status);
    return;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  #if 1
  length = hcd->driver->hub_status_data (hcd, (char*) urb->transfer_buffer);
  #else
  if (!HCD_IS_SUSPENDED (hcd->state))
    length = hcd->driver->hub_status_data (hcd, urb->transfer_buffer);
  #endif
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  spin_lock (&hcd_data_lock);
  if (length > 0) {
    hcd->rh_timer.data = 0;
    urb->actual_length = length;
    urb->status = 0;
    urb->hcpriv = 0;
  }
  else
    mod_timer (&hcd->rh_timer, ROOTHUB_TIMER_INTERVAL, 0);
  spin_unlock (&hcd_data_lock);
  spin_unlock (&urb->lock);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (length > 0)
    usb_hcd_giveback_urb (hcd, urb, NULL);
  local_irq_restore (flags);
}

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
static int rh_urb_enqueue (struct usb_hcd *hcd, struct urb *urb)
{
  if (usb_pipeint (urb->pipe)) {
    int    retval;
    U32  flags;

    spin_lock_irqsave (&hcd_data_lock, flags);
    retval = rh_status_urb (hcd, urb);
    spin_unlock_irqrestore (&hcd_data_lock, flags);
    return retval;
  }
  if (usb_pipecontrol (urb->pipe))
    return rh_call_control (hcd, urb);
  else
    return -EINVAL;
}

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_rh_status_dequeue (struct usb_hcd *hcd, struct urb *urb)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  del_timer_sync (&hcd->rh_timer);
  hcd->rh_timer.data = 0;

  local_irq_save (flags);
  urb->hcpriv = 0;
  usb_hcd_giveback_urb (hcd, urb, NULL);
  local_irq_restore (flags);
}

#endif
/*-------------------------------------------------------------------------*/
#ifdef   ROOTHUB_INTERRUPT_MODE
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
void usb_hcd_poll_rh_status(struct usb_hcd *hcd)
{
	struct urb	*urb;
	int		length;
	unsigned long	flags;
	char		buffer[4];	

	if (unlikely(!hcd->rh_registered))
		return;
	if (!hcd->uses_new_polling && !hcd->status_urb)
		return;

	length = hcd->driver->hub_status_data(hcd, buffer);
	if (length > 0) {

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		spin_lock_irqsave(&hcd_root_hub_lock, flags);
		urb = hcd->status_urb;
		if (urb) {
			hcd->poll_pending = 0;
			hcd->status_urb = NULL;
			urb->actual_length = length;
                        urb->status = 0; // update status
                        urb->hcpriv = 0;
			memcpy(urb->transfer_buffer, buffer, length);

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			spin_unlock(&hcd_root_hub_lock);
			usb_hcd_giveback_urb(hcd, urb, NULL);
			spin_lock(&hcd_root_hub_lock);
		} else {
			length = 0;
			hcd->poll_pending = 1;
		}
		spin_unlock_irqrestore(&hcd_root_hub_lock, flags);
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (hcd->uses_new_polling ? hcd->poll_rh :
			(length == 0 && hcd->status_urb != NULL))
		mod_timer (&hcd->rh_timer, (jiffies/(HZ/4) + 1) * (HZ/4), 0);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void rh_timer_func (unsigned long _hcd)
{
	usb_hcd_poll_rh_status((struct usb_hcd *) _hcd);
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
/*-------------------------------------------------------------------------*/

static int rh_queue_status (struct usb_hcd *hcd, struct urb *urb)
{
	int		retval;
	unsigned long	flags;
	int		len = 1 + (urb->dev->maxchild / 8);

	spin_lock_irqsave (&hcd_root_hub_lock, flags);
	if (hcd->status_urb || urb->transfer_buffer_length < len) {
		dev_dbg (hcd->self.controller, "not queuing rh status urb\n");
		retval = -EINVAL;
		goto done;
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

	hcd->status_urb = urb;
	urb->hcpriv = hcd;	/* indicate it's queued */
	if (!hcd->uses_new_polling)
		mod_timer(&hcd->rh_timer, (jiffies/(HZ/4) + 1) * (HZ/4), 0);

	/* If a status change has already occurred, report it ASAP */
	else if (hcd->poll_pending)
		mod_timer(&hcd->rh_timer, jiffies, 0);
	retval = 0;
 done:
	spin_unlock_irqrestore (&hcd_root_hub_lock, flags);
	return retval;
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
/*-------------------------------------------------------------------------*/

static int rh_urb_enqueue (struct usb_hcd *hcd, struct urb *urb)
{
  if (usb_pipeint (urb->pipe)) {
    int    retval;
    U32  flags;

//diag_printf("<rh_urb_enqueue>...\n");
    spin_lock_irqsave (&hcd_data_lock, flags);
    retval = rh_queue_status (hcd, urb);
    spin_unlock_irqrestore (&hcd_data_lock, flags);
    return retval;
  }
  else if (usb_pipecontrol (urb->pipe))
    return rh_call_control (hcd, urb);
  else
    return -EINVAL;
}

/*-------------------------------------------------------------------------*/

void usb_rh_status_dequeue (struct usb_hcd *hcd, struct urb *urb)
{
   unsigned long   flags;
   //int     rc;

diag_printf("<usb_rh_status_dequeue>\n");
   spin_lock_irqsave(&hcd_root_hub_lock, flags);
   /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
   if (usb_pipecontrol (urb->pipe)) {    /* Control URB */
       ;   /* Do nothing */

   } else {                /* Status URB */
       if (!hcd->uses_new_polling)
           del_timer (&hcd->rh_timer);
       if (urb == hcd->status_urb) {
           hcd->status_urb = NULL;
           // TODO: FIXME
           //usb_hcd_unlink_urb_from_ep(hcd, urb);

           spin_unlock(&hcd_root_hub_lock);
           usb_hcd_giveback_urb(hcd, urb, NULL);
           spin_lock(&hcd_root_hub_lock);
       }
   }
//done:
   spin_unlock_irqrestore(&hcd_root_hub_lock, flags);
   //return rc;
}
#endif

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

struct usb_bus *usb_bus_get (struct usb_bus *bus)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return bus;
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

void usb_bus_put (struct usb_bus *bus)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
}

/*-------------------------------------------------------------------------*/


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

void usb_host_init(void)
{
  //class_register(&usb_host_class);
}

void usb_host_cleanup(void)
{
  //class_unregister(&usb_host_class);
}



/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_bus_init (struct usb_bus *bus)
{
  memset (&bus->devmap, 0, sizeof(struct usb_devmap));

  bus->devnum_next = 1;

  bus->root_hub = NULL;
  bus->hcpriv = NULL;
  bus->otgpriv = NULL;
  bus->busnum = -1;
  bus->bandwidth_allocated = 0;
  bus->bandwidth_int_reqs  = 0;
  bus->bandwidth_isoc_reqs = 0;

  INIT_LIST_HEAD (&bus->bus_list);
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
struct usb_bus *usb_alloc_bus (struct usb_operations *op)
{
  struct usb_bus *bus;

  bus = (struct usb_bus*) kmalloc (sizeof *bus, GFP_KERNEL);
  if (!bus)
    return NULL;
  memset(bus, 0, sizeof(struct usb_bus));
  usb_bus_init (bus);
  bus->op = op;
  return bus;
}
//EXPORT_SYMBOL (usb_alloc_bus);
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_register_bus(struct usb_bus *bus)
{
  int busnum;
  //int retval;

  down (&usb_bus_list_lock);
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
  busnum = find_next_zero_bit (busmap.busmap, USB_MAXBUS, 1);
  if (busnum < USB_MAXBUS) {
    set_bit (busnum, busmap.busmap, U32);
    bus->busnum = busnum;
    dbg("The busnum is %d",busnum);
  } else
    usb_warn ("too many buses%s","");
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  list_add (&bus->bus_list, &usb_bus_list);
  up (&usb_bus_list_lock);

  usbfs_add_bus (bus);

  dev_info (bus->controller, "new USB bus registered, assigned bus number %d\n", bus->busnum);
  return 0;
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_deregister_bus (struct usb_bus *bus)
{
  dev_info (bus->controller, "USB bus %d deregistered\n", bus->busnum);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  down (&usb_bus_list_lock);
  list_del (&bus->bus_list);
  up (&usb_bus_list_lock);

  usbfs_remove_bus (bus);

  clear_bit (bus->busnum, busmap.busmap, U32);

  //class_device_unregister(&bus->class_dev);
}
//EXPORT_SYMBOL (usb_deregister_bus);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_register_root_hub (struct usb_device *usb_dev, struct device_s *parent_dev)
{
  const int devnum = 1;
  int retval;

  USB_sprintf (&usb_dev->dev.bus_id[0],
     		/*sizeof(usb_dev->dev.bus_id), */ "usb%ld-rh", usb_dev->bus->busnum);
  usb_dev->state = USB_STATE_DEFAULT;

  usb_dev->devnum = devnum;
  usb_dev->bus->devnum_next = devnum + 1;
  set_bit (devnum, usb_dev->bus->devmap.devicemap, U32);

  retval = usb_new_device (usb_dev, parent_dev);
  if (retval)
    dev_err (parent_dev, "can't register root hub for %s, %d\n",
        usb_dev->dev.bus_id, retval);
  return retval;
}
//EXPORT_SYMBOL (usb_register_root_hub);

#ifdef ROOTHUB_INTERRUPT_MODE
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int register_root_hub(struct usb_hcd *hcd)
{
	//struct device *parent_dev = hcd->self.controller;
        struct device_s *parent_dev = hcd->controller;
	struct usb_device *usb_dev = hcd->self.root_hub;
	const int devnum = 1;
	int retval;

        usb_dev->state = USB_STATE_DEFAULT;
        
        usb_dev->devnum = devnum;
        usb_dev->bus->devnum_next = devnum + 1;
        set_bit (devnum, usb_dev->bus->devmap.devicemap, U32);

        retval = usb_new_device (usb_dev, parent_dev);
        if (retval)
          dev_err (parent_dev, "can't register root hub for %s, %d\n",
              usb_dev->dev.bus_id, retval);

	if (retval == 0) {
                diag_printf("<register_root_hub> hcd->rh_registered = 1\n");
		spin_lock_irq (&hcd_root_hub_lock);
		hcd->rh_registered = 1;
		spin_unlock_irq (&hcd_root_hub_lock);

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	}

	return retval;
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
int usb_calc_bus_time (int speed, int is_input, int isoc, int bytecount)
{
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  U32  tmp;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  switch (speed) {
  case USB_SPEED_LOW:   
    if (is_input) {
      tmp = (67667L * (31L + 10L * BitTime (bytecount))) / 1000L;
      return (64060L + (2 * BW_HUB_LS_SETUP) + BW_HOST_DELAY + tmp);
    } else {
      tmp = (66700L * (31L + 10L * BitTime (bytecount))) / 1000L;
      return (64107L + (2 * BW_HUB_LS_SETUP) + BW_HOST_DELAY + tmp);
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
  case USB_SPEED_FULL:  
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (isoc) {
      tmp = (8354L * (31L + 10L * BitTime (bytecount))) / 1000L;
      return (((is_input) ? 7268L : 6265L) + BW_HOST_DELAY + tmp);
    } else {
      tmp = (8354L * (31L + 10L * BitTime (bytecount))) / 1000L;
      return (9107L + BW_HOST_DELAY + tmp);
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
  case USB_SPEED_HIGH:  
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (isoc)
      tmp = HS_USECS (bytecount);
    else
      tmp = HS_USECS_ISO (bytecount);
    return tmp;
  default:
    dbg ("bogus device speed!%s","");
    return -1;
  }
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
void usb_claim_bandwidth (struct usb_device *dev, struct urb *urb, int bustime, int isoc)
{
  dev->bus->bandwidth_allocated += bustime;
  if (isoc)
    dev->bus->bandwidth_isoc_reqs++;
  else
    dev->bus->bandwidth_int_reqs++;
  urb->bandwidth = bustime;

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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifdef USB_BANDWIDTH_MESSAGES
  dev_dbg (&dev->dev, "bandwidth alloc increased by %d (%s) to %d for %d requesters\n",
    bustime,
    isoc ? "ISOC" : "INTR",
    dev->bus->bandwidth_allocated,
    dev->bus->bandwidth_int_reqs + dev->bus->bandwidth_isoc_reqs);
#endif
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
void usb_release_bandwidth (struct usb_device *dev, struct urb *urb, int isoc)
{
  dev->bus->bandwidth_allocated -= urb->bandwidth;
  if (isoc)
    dev->bus->bandwidth_isoc_reqs--;
  else
    dev->bus->bandwidth_int_reqs--;

#ifdef USB_BANDWIDTH_MESSAGES
  dev_dbg (&dev->dev, "bandwidth alloc reduced by %d (%s) to %d for %d requesters\n",
    urb->bandwidth,
    isoc ? "ISOC" : "INTR",
    dev->bus->bandwidth_allocated,
    dev->bus->bandwidth_int_reqs + dev->bus->bandwidth_isoc_reqs);
#endif
  urb->bandwidth = 0;
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

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
static int hcd_alloc_dev (struct usb_device *udev)
{
  struct hcd_dev    *dev;
  struct usb_hcd    *hcd;
  U32    flags;

  if (!udev || udev->hcpriv)
    return -EINVAL;
  if (!udev->bus || !udev->bus->hcpriv)
    return -ENODEV;
  hcd = (struct usb_hcd*) udev->bus->hcpriv;
  if (hcd->state == USB_STATE_QUIESCING)
    return -ENOLINK;

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

  dev = (struct hcd_dev *) kmalloc (sizeof *dev, GFP_KERNEL);
  if (dev == NULL)
    return -ENOMEM;
  memset (dev, 0, sizeof *dev);

  INIT_LIST_HEAD (&dev->dev_list);
  INIT_LIST_HEAD (&dev->urb_list);

  spin_lock_irqsave (&hcd_data_lock, flags);
  list_add (&dev->dev_list, &hcd->dev_list);
  
  udev->hcpriv = dev;
  spin_unlock_irqrestore (&hcd_data_lock, flags);

  return 0;
}

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
static void urb_unlink (struct urb *urb)
{
  U32    flags;
  struct usb_device  *dev;

  /* Release any periodic transfer bandwidth */
  if (urb->bandwidth)
    usb_release_bandwidth (urb->dev, urb,
      usb_pipeisoc (urb->pipe));

  /* clear all state linking urb to this dev (and hcd) */

  spin_lock_irqsave (&hcd_data_lock, flags);
  list_del_init (&urb->urb_list);
  dev = urb->dev;
  spin_unlock_irqrestore (&hcd_data_lock, flags);
  usb_put_dev (dev);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int hcd_submit_urb (struct urb *urb, int mem_flags)
{
  int      status;
  struct usb_hcd    *hcd = (struct usb_hcd*) urb->dev->bus->hcpriv;
  struct hcd_dev    *dev = (struct hcd_dev*) urb->dev->hcpriv;
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  U32    flags;

  if (mem_flags == ASYNC_SUBMIT_QTD)
    return (hcd->driver->urb_enqueue (hcd, urb, mem_flags));

  if (!hcd || !dev)
    return -ENODEV;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  ehci->uDontSendIAA = 0;	//Set IAA interrupt (default)

  spin_lock_irqsave (&hcd_data_lock, flags);
  if (HCD_IS_RUNNING (hcd->state) && hcd->state != USB_STATE_QUIESCING) {
    usb_get_dev (urb->dev);
    list_add_tail (&urb->urb_list, &dev->urb_list);
    status = 0;
  } else {
    INIT_LIST_HEAD (&urb->urb_list);
    diag_printf("hcd_submit_urb return ESHUTDOWN !!! hcd->state = %x\n", hcd->state);
    status = -ESHUTDOWN;
  }
  spin_unlock_irqrestore (&hcd_data_lock, flags);
  if (status)
    return status;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb = usb_get_urb (urb);
  if (urb->dev == hcd->self.root_hub) {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->transfer_flags |= (URB_NO_TRANSFER_DMA_MAP
          | URB_NO_SETUP_DMA_MAP);
    status = rh_urb_enqueue (hcd, urb);
    goto done;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/


  if (hcd->controller->dma_mask) {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  }
  else
  {
    // Non DMA support
    if (usb_pipecontrol (urb->pipe))
    {
        //if ((((U32) urb->setup_packet) & 0xF) != 0 || ((sizeof(struct usb_ctrlrequest)  & 0xF) != 0) )
        if ((((U32) urb->setup_packet) & CPU_L1_CACHE_BOUND) != 0 || ((sizeof(struct usb_ctrlrequest)  & CPU_L1_CACHE_BOUND) != 0) ) // J
        {
            //diag_printf("Setup address %p, length %x\n", urb->setup_packet, sizeof(struct usb_ctrlrequest));
            urb->SetDMALen = sizeof(struct usb_ctrlrequest);
            //urb->SetDMABuf = LIB_MemoryAllocate(urb->SetDMALen | 0xF, MALLOC_UNCACHED_ADDR);
            //urb->SetDMABuf = MsOS_AllocateMemory(((urb->SetDMALen+15) & ~0xF), gs32NonCachedPoolID_MIU0 );
            //urb->SetDMABuf = Usb_AllocateNonCachedMemory(((urb->SetDMALen+15) & ~0xF));
            urb->SetDMABuf = Usb_AllocateNonCachedMemory(((urb->SetDMALen+CPU_L1_CACHE_BOUND) & ~CPU_L1_CACHE_BOUND)); // J

            USB_ASSERT(urb->SetDMABuf != NULL, "Allocate SetDMABuf error\n");

            memcpy(urb->SetDMABuf, urb->setup_packet, urb->SetDMALen);
            //HAL_DCACHE_FLUSH((void*) urb->SetDMABuf, urb->SetDMALen);

            urb->setup_dma = (dma_addr_t) USB_VA2PA((U32)urb->SetDMABuf);
            //diag_printf("SetDMA address %x SetDMA length %x\n", urb->setup_dma, urb->SetDMALen);
            Chip_Flush_Memory();
        }
	    else
	    {
            if ( urb->setup_packet != KSEG02KSEG1(urb->setup_packet) )
			    //HAL_DCACHE_FLUSH((void*) urb->setup_packet, sizeof(struct usb_ctrlrequest));
			    MsOS_Dcache_Flush((U32) urb->setup_packet, sizeof(struct usb_ctrlrequest));

            urb->setup_dma = (dma_addr_t) USB_VA2PA((U32)urb->setup_packet);
	        //diag_printf("Setup DMA address %x DMA length %x\n", urb->setup_dma, sizeof(struct usb_ctrlrequest));
            Chip_Flush_Memory();
	    }
	    USB_ASSERT((urb->setup_dma & 0xF) == 0, "DMA address is not 16 bytes aligned\n");
	//OS_Assert((sizeof(struct usb_ctrlrequest) & 0xF) == 0, "DMA length is not 16 bytes aligned\n");

   }

    if (urb->transfer_buffer_length != 0)
    {
#if 0			//yuwen, mass storage //USB_2 remove
	  if ((Mass_stor_us!=NULL)&& ( urb->transfer_buffer == Mass_stor_us->iobuf ))
        {
		urb->transfer_dma= (dma_addr_t) USB_VA2PA((U32)urb->transfer_buffer);
        }
        else
#endif
        {
            //if ((((U32) urb->transfer_buffer) & 0xF) != 0 || ((urb->transfer_buffer_length  & 0xF) != 0) )
            if ((((U32) urb->transfer_buffer) & CPU_L1_CACHE_BOUND) != 0 || ((urb->transfer_buffer_length  & CPU_L1_CACHE_BOUND) != 0) ) // J
            {
                //diag_printf("<hcd_submit_urb> bouncing A[%x], L[%d]\n", urb->transfer_buffer, urb->transfer_buffer_length);
                #ifdef DEBUG_PERFORMANCE
                if (urb->transfer_buffer_length >= 512)
                    diag_printf("DMA address %x DMA length %x\n", urb->transfer_buffer, urb->transfer_buffer_length);
                #endif
                urb->TxDMALen= urb->transfer_buffer_length;
                //urb->TxDMABuf = LIB_MemoryAllocate(urb->TxDMALen | 0xF, MALLOC_UNCACHED_ADDR);
                //urb->TxDMABuf = MsOS_AllocateMemory(((urb->TxDMALen+15) & ~0xF), gs32NonCachedPoolID_MIU0 );
                //urb->TxDMABuf = Usb_AllocateNonCachedMemory(((urb->TxDMALen+15) & ~0xF));
                urb->TxDMABuf = Usb_AllocateNonCachedMemory(((urb->TxDMALen+CPU_L1_CACHE_BOUND) & ~CPU_L1_CACHE_BOUND)); // J
                USB_ASSERT(urb->TxDMABuf != NULL, "Allocate TxDMABuf error\n");

                if (!usb_pipein(urb->pipe))
                {
                    memcpy(urb->TxDMABuf, urb->transfer_buffer, urb->transfer_buffer_length);
                }

                urb->transfer_dma= (dma_addr_t) USB_VA2PA((U32)urb->TxDMABuf);
                //diag_printf("TxDMA address %x TxDMA length %x\n", urb->transfer_dma, urb->TxDMALen);
            }
            else
            {
                if (  urb->transfer_buffer != KSEG02KSEG1(urb->transfer_buffer) )
                {
                    if (usb_pipein(urb->pipe))
                        //HAL_DCACHE_INVALIDATE((void*) urb->transfer_buffer, urb->transfer_buffer_length);
                        //MsOS_Dcache_Invalidate((U32) urb->transfer_buffer, urb->transfer_buffer_length);
                        MsOS_Dcache_Flush((U32) urb->transfer_buffer, urb->transfer_buffer_length);
                        //MsOS_Dcache_Invalidate seems to cause stack crashed... , change to MsOS_Dcache_Flush temporiarly

                    else
                        //HAL_DCACHE_FLUSH((void*) urb->transfer_buffer, urb->transfer_buffer_length);
                        MsOS_Dcache_Flush((U32) urb->transfer_buffer, urb->transfer_buffer_length);

                }

                urb->transfer_dma= (dma_addr_t) USB_VA2PA((U32)urb->transfer_buffer);
                //diag_printf("DMA address %x DMA length %x\n", urb->transfer_dma, urb->transfer_buffer_length);
            }
        }
        USB_ASSERT((urb->transfer_dma & 0xF) == 0, "DMA address is not 16 bytes aligned\n");
        //OS_Assert((urb->transfer_buffer_length & 0xF) == 0, "DMA length is not 16 bytes aligned\n");
    }
  }


  status = hcd->driver->urb_enqueue (hcd, urb, mem_flags);
done:
  if (status) {
    usb_put_urb (urb);
    urb_unlink (urb);
  }
  return status;
}

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int hcd_get_frame_number (struct usb_device *udev)
{
  struct usb_hcd  *hcd = (struct usb_hcd *)udev->bus->hcpriv;
  if (!HCD_IS_RUNNING (hcd->state))
  {
    diag_printf("hcd_get_frame_number return ESHUTDOWN !!!\n");
    return -ESHUTDOWN;
  }
  return hcd->driver->get_frame_number (hcd);
}

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
static void
unlink1 (struct usb_hcd *hcd, struct urb *urb)
{
  if (urb == (struct urb *) hcd->rh_timer.data)
    usb_rh_status_dequeue (hcd, urb);
  else {
    int    value;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    value = hcd->driver->urb_dequeue (hcd, urb);
    if (value != 0)
      dev_dbg (hcd->controller,
        "dequeue %p --> %d\n",
        urb, value);
  }
}

struct completion_splice {    // modified urb context:
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  struct completion  done;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  usb_complete_t    complete;
  void      *context;
};

static void unlink_complete (struct urb *urb, struct pt_regs *regs)
{
  struct completion_splice  *splice;

  splice = (struct completion_splice *) urb->context;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb->complete = splice->complete;
  urb->context = splice->context;
  urb->complete (urb, regs);  //Callback function

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  complete (&splice->done);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern void USB_HW_Reset(struct usb_hcd* pUsbHcd/*, struct usb_device *pUsbDev*/);
static int hcd_unlink_urb (struct urb *urb)
{
  struct hcd_dev      *dev;
  struct usb_hcd      *hcd = 0;
  struct device_s      *sys = 0;
  U32      flags;
  struct completion_splice  splice;
  int        retval;
  U32    d_cnt = 0;

  if (!urb)
    return -EINVAL;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  spin_lock_irqsave (&urb->lock, flags);
  spin_lock (&hcd_data_lock);

  if (!urb->dev || !urb->dev->bus) {
    retval = -ENODEV;
    goto done;
  }

  dev = (struct hcd_dev*) urb->dev->hcpriv;
  sys = &urb->dev->dev;
  hcd = (struct usb_hcd*) urb->dev->bus->hcpriv;
  if (!dev || !hcd) {
    retval = -ENODEV;
    goto done;
  }

  if (!urb->hcpriv) {
    retval = -EINVAL;
    goto done;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (urb->status != -EINPROGRESS) {
    retval = -EBUSY;
    goto done;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!(urb->transfer_flags & URB_ASYNC_UNLINK)) {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    init_completion (&splice.done);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    splice.complete = urb->complete;
    splice.context = urb->context;
    urb->complete = unlink_complete;
    urb->context = &splice;
    urb->status = -ENOENT;
  } else {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->status = -ECONNRESET;
  }
  spin_unlock (&hcd_data_lock);
  spin_unlock_irqrestore (&urb->lock, flags);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (urb == (struct urb *) hcd->rh_timer.data) {
    usb_rh_status_dequeue (hcd, urb);
    retval = 0;
  } else {
    retval = hcd->driver->urb_dequeue (hcd, urb);

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (retval) {
      dev_dbg (sys, "dequeue %p --> %d\n", urb, retval);
      if (!(urb->transfer_flags & URB_ASYNC_UNLINK)) {
        spin_lock_irqsave (&urb->lock, flags);
        urb->complete = splice.complete;
        urb->context = splice.context;
        spin_unlock_irqrestore (&urb->lock, flags);
      }
      goto bye;
    }
  }

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (urb->transfer_flags & URB_ASYNC_UNLINK)
    return -EINPROGRESS;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  while ( splice.done.done == 0 )
  {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if ( d_cnt > (U32) 3000)
    {
      break;
    }
    else
    {
      HAL_DELAY_US(1000);//tick base is 1 ms
      d_cnt+=1;
    }
  }

  if (d_cnt > (U32) 3000)
  {
     diag_printf("hcd_unlink_urb timeout\n");
     // TODO: Review the flow
     diag_printf("Reset USB HW\n");
     USB_HW_Reset(hcd);
     //USB_HW_Reset((struct usb_hcd*) urb->dev->bus->hcpriv, urb->dev);
        return -ETIMEDOUT;
  }
  return 0;

done:
  spin_unlock (&hcd_data_lock);
  spin_unlock_irqrestore (&urb->lock, flags);
bye:
  if (retval && sys && sys->driver)
    dev_dbg (sys, "hcd_unlink_urb %p fail %d\n", urb, retval);
  return retval;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void hcd_endpoint_disable (struct usb_device *udev, int endpoint)
{
  struct list_head *__mptr;
  struct hcd_dev  *dev;
  struct usb_hcd  *hcd;
  struct urb  *urb;
  char s[10];
  U32  epnum = endpoint & USB_ENDPOINT_NUMBER_MASK;

  dev = (struct hcd_dev*) udev->hcpriv;
  hcd = (struct usb_hcd*) udev->bus->hcpriv;

  local_irq_disable ();

hcd_rescan:
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (endpoint & USB_DIR_IN) {
    usb_endpoint_halt (udev, epnum, 0);
    udev->epmaxpacketin [epnum] = 0;
  } else {
    usb_endpoint_halt (udev, epnum, 1);
    udev->epmaxpacketout [epnum] = 0;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  spin_lock (&hcd_data_lock);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb = list_entry((&dev->urb_list)->next, struct urb, urb_list);
  for( ; &(urb->urb_list) != &(dev->urb_list);
    __mptr = urb->urb_list.next,
    urb = (struct urb *)( (char *)__mptr - (char *)offsetof(struct urb,urb_list) )
  )
  {
    int  tmp = urb->pipe;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if ((U32)usb_pipeendpoint (tmp) != epnum)
      continue;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (epnum != 0 && ((tmp ^ endpoint) & USB_DIR_IN))
      continue;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (urb->status != -EINPROGRESS)
      continue;
    usb_get_urb (urb);
    spin_unlock (&hcd_data_lock);

    spin_lock (&urb->lock);
    tmp = urb->status;
    if (tmp == -EINPROGRESS)
    {
      diag_printf("hcd_endpoint_disable return ESHUTDOWN !!!\n");    
      urb->status = -ESHUTDOWN;
    }
    spin_unlock (&urb->lock);

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (tmp == -EINPROGRESS) {
      tmp = urb->pipe;
      unlink1 (hcd, urb);
      switch (usb_pipetype (tmp)) {
         case PIPE_CONTROL:
          strncpy(s,"", 1);
          break;
         case PIPE_BULK:
          strncpy(s,"-bulk", strlen("-bulk"));
          break;
         case PIPE_INTERRUPT:
          strncpy(s,"-intr", strlen("-intr"));
          break;
         default:
          strncpy(s,"-iso", strlen("-iso"));
          break;
      }

      dev_dbg (hcd->controller,
        "shutdown urb %p pipe %08x ep%d%s%s\n",
        urb, tmp, usb_pipeendpoint (tmp),
        (tmp & USB_DIR_IN) ? "in" : "out",
         s);
    }
    usb_put_urb (urb);

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    goto hcd_rescan;
  }
  spin_unlock (&hcd_data_lock);
  local_irq_enable();

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  //might_sleep ();
  if (hcd->driver->endpoint_disable)
    hcd->driver->endpoint_disable (hcd, dev, endpoint);
}

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
static int hcd_free_dev (struct usb_device *udev)
{
  struct hcd_dev    *dev;
  struct usb_hcd    *hcd;
  U32    flags;

  if (!udev || !udev->hcpriv)
    return -EINVAL;

  if (!udev->bus || !udev->bus->hcpriv)
    return -ENODEV;

  // should udev->devnum == -1 ??

  dev = (struct hcd_dev*) udev->hcpriv;
  hcd = (struct usb_hcd*) udev->bus->hcpriv;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!list_empty (&dev->urb_list)) {
    dev_dbg (hcd->controller, "free busy dev, %s devnum %d (bug!)\n",
      hcd->self.bus_name, udev->devnum);
    return -EINVAL;
  }

  spin_lock_irqsave (&hcd_data_lock, flags);
  list_del (&dev->dev_list);
  udev->hcpriv = NULL;
  spin_unlock_irqrestore (&hcd_data_lock, flags);

  kfree (dev);
  return 0;
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
struct usb_operations usb_hcd_operations = {
  hcd_alloc_dev,
  hcd_free_dev,
  hcd_get_frame_number,
  hcd_submit_urb,
  hcd_unlink_urb,
  hcd_buffer_alloc,
  hcd_buffer_free,
  hcd_endpoint_disable,
};

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_hcd_giveback_urb (struct usb_hcd *hcd, struct urb *urb, struct pt_regs *regs)
{
  urb_unlink (urb);
  urb->complete (urb, regs);
  usb_put_urb (urb);
}

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 #if 0
irqreturn_t usb_hcd_irq (int irq, void *__hcd, struct pt_regs * r)
{
  struct usb_hcd    *hcd = (struct usb_hcd*) __hcd;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd->driver->irq (hcd, r,0);
}
#endif
/*-------------------------------------------------------------------------*/

#if 0
static void hcd_panic (void *_hcd)
{
  struct usb_hcd    *hcd = (struct usb_hcd*) _hcd;
  struct usb_device  *hub = (struct usb_device*) hcd->self.root_hub;
  U32    i;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hub->state = USB_STATE_NOTATTACHED;
  for (i = 0; i < (U32) hub->maxchild; i++) {
    if (hub->children [i])
      usb_disconnect (&hub->children [i]);
  }

}
#endif

#endif    //#ifdef

#ifdef CONFIG_OTG

irqreturn_t usb_otgd_irq (int irq, void *__otgd, struct pt_regs * r)
{
  struct usb_otgd    *otgd = (struct usb_otgd*) __otgd;
  otgd->driver->irq (otgd, r);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

int usb_register_otg_to_bus(struct usb_otgd *otgd, struct device *hc)
{
  struct list_head *buslist;
  struct usb_bus *bus;

  for (buslist = usb_bus_list.next;
       buslist != &usb_bus_list;
       buslist = buslist->next) {
    const struct list_head *mptr = buslist;
    bus = (struct usb_bus *)( (char *)mptr - offsetof( struct usb_bus, bus_list) );
    if ( bus->controller == hc )
    {
      bus->otgpriv = otgd;
      otgd->bus = bus;
      dbg("Found bus (%p), device name is %s",bus,bus->controller->name);
      return 0;
    }
  }
  return -ENODEV;
}


#endif


