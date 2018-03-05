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
#include  "include/drvTypes.h"
#include  "include/drvIO.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"
#include  "include/drvKernel.h"
#include  "include/drvBitops.h"

// USB related header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "drvHUB.h"

extern void USBCriticalSectionIn(MS_U8 Port);
extern void USBCriticalSectionOut(MS_U8 Port);
extern int hub_port_status(struct usb_device *dev, int port,
             U16 *status, U16 *change);
extern int hub_reset(struct usb_hub *hub);
extern void hub_start_disconnect(struct usb_device *dev);
extern void hub_port_connect_change(struct usb_hub *hubstate, int port,
          U16 portstatus, U16 portchange);
extern int clear_port_feature(struct usb_device *dev, int port, int feature);
extern void hub_power_on(struct usb_hub *hub);
extern int hub_hub_status(struct usb_hub *hub,
            U16 *status, U16 *change);
extern int clear_hub_feature(struct usb_device *dev, int feature);

/*static*/
LIST_HEAD(hub_event_list_Port2);  /* List of hubs needing servicing */

BOOL has_hub_events_Port2(U16 *pPortStatus)
{
    //U16 portstatus;
    BOOL status;
    U16 portchange;
    struct list_head *tmp;
    struct usb_device *dev;
    struct usb_hub *hub;
    //int i, ret;

    USBCriticalSectionIn(2);
    if (list_empty(&hub_event_list_Port2))
    {
        status = FALSE;
    }
    else
    {
        U32 u32OldIntr;
        u32OldIntr = MsOS_DisableAllInterrupts();

        tmp = hub_event_list_Port2.next;
        hub = list_entry(tmp, struct usb_hub, event_list);
        dev = interface_to_usbdev(hub->intf);

        //MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
        MsOS_RestoreAllInterrupts(u32OldIntr);


        hub_port_status(dev, 0, pPortStatus, &portchange);
        //diag_printf("portstatus: %X\n", *pPortStatus);

        if (portchange & USB_PORT_STAT_C_CONNECTION)
            status = TRUE;
        else
            status = FALSE;
    }

    USBCriticalSectionOut(2);
    return status;
}

static void hub_events_Port2(void)
{
  U32 flags;
  struct list_head *tmp;
  struct usb_device *dev;
  struct usb_hub *hub;
  U16 hubstatus;
  U16 hubchange;
  U16 portstatus;
  U16 portchange;
  int i, ret;

  while (1) {
    spin_lock_irqsave(&hub_event_lock_Port2, flags);

    if (list_empty(&hub_event_list_Port2))
      break;

    /* Grab the next entry from the beginning of the list */
 // MsOS_DisableInterrupt(E_IRQ_FIQ_ALL);
     U32 u32OldIntr;
    u32OldIntr = MsOS_DisableAllInterrupts();

    tmp = hub_event_list_Port2.next;

	hub = list_entry(tmp, struct usb_hub, event_list);
    //list_entry(tmp, struct usb_hub, event_list,struct list_head, hub);
    dev = interface_to_usbdev(hub->intf);

    list_del_init(tmp);
 // MsOS_EnableInterrupt(E_IRQ_FIQ_ALL);
      MsOS_RestoreAllInterrupts(u32OldIntr);

    //if (unlikely(down_trylock(&hub->khubd_sem)))
    //  BUG();  /* never blocks, we were on list */

    spin_unlock_irqrestore(&hub_event_lock_Port2, flags);

    if (hub->error) {
      dev_dbg (&hub->intf->dev, "resetting for error %d\n",
        hub->error);

      if (hub_reset(hub)) {
        dev_dbg (&hub->intf->dev,
          "can't reset; disconnecting%s","\n");
        up(&hub->khubd_sem);
        hub_start_disconnect(dev);
        continue;
      }

      hub->nerrors = 0;
      hub->error = 0;
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
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

    diag_printf("get hub event 2\n");
    for (i = 0; i < hub->descriptor.bNbrPorts; i++) {
      ret = hub_port_status(dev, i, &portstatus, &portchange);
      if (ret < 0) {
        continue;
      }
      if (portchange & USB_PORT_STAT_C_CONNECTION) {
        hub_port_connect_change(hub, i, portstatus, portchange);
      } else if (portchange & USB_PORT_STAT_C_ENABLE) {
        dev_dbg (hubdev (dev),
          "port %d enable change, status %x\n",
          i + 1, portstatus);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_ENABLE);

         
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        if (!(portstatus & USB_PORT_STAT_ENABLE)
            && (portstatus & USB_PORT_STAT_CONNECTION)
            && (dev->children[i])) {
          dev_err (&hub->intf->dev,
              "port %i "
              "disabled by hub (EMI?), "
              "re-enabling...",
            i + 1);
          hub_port_connect_change(hub,
            i, portstatus, portchange);
        }
      }

      if (portchange & USB_PORT_STAT_C_SUSPEND) {
        dev_dbg (&hub->intf->dev,
          "suspend change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1,  USB_PORT_FEAT_C_SUSPEND);
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
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

      if (portchange & USB_PORT_STAT_C_OVERCURRENT) {
        dev_err (&hub->intf->dev,
          "over-current change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_OVER_CURRENT);
        hub_power_on(hub);
      }

      if (portchange & USB_PORT_STAT_C_RESET) {
        dev_dbg (&hub->intf->dev,
          "reset change on port %d\n",
          i + 1);
        clear_port_feature(dev,
          i + 1, USB_PORT_FEAT_C_RESET);
      }
    } /* end for i */

    /* deal with hub status changes */
	
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (hub_hub_status(hub, &hubstatus, &hubchange) < 0) {
      dev_err (&hub->intf->dev, "get_hub_status failed%s","\n");
    }
    else {
      if (hubchange & HUB_CHANGE_LOCAL_POWER) {
        dev_dbg (&hub->intf->dev, "power change%s","\n");
        clear_hub_feature(dev, C_HUB_LOCAL_POWER);
      }
      if (hubchange & HUB_CHANGE_OVERCURRENT) {
        dev_dbg (&hub->intf->dev, "overcurrent change%s","\n");
        wait_ms(500);  /* Cool down */
        clear_hub_feature(dev, C_HUB_OVER_CURRENT);
                          hub_power_on(hub);
      }
    }
    up(&hub->khubd_sem);
        } /* end while (1) */

  spin_unlock_irqrestore(&hub_event_lock_Port2, flags);
}

void hub_poll_Port2(void)
{
  // Disable Interrupts
  //cli();
  //vdbg("%s","polling");
  //check_usb_lock();
  USBCriticalSectionIn(2);
  hub_events_Port2();
  USBCriticalSectionOut(2);

  //vdbg("%s","polling ok!!");
  //unlock_usb_core();
  // Re-enable Interrupts
  //sti();
}

void hub_add_event_Port2(struct list_head *newent)
{
	list_add(newent, &hub_event_list_Port2);
}
