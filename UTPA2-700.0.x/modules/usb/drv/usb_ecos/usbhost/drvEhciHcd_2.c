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

#include "include/drvConfig.h"

#include <MsCommon.h>
#include "include/drvCompiler.h"
#include "include/drvPorts.h"
#include "include/drvErrno.h"
#include "include/drvIO.h"
#include "include/drvPCIMEM.h"
#include "include/drvList.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
// USB related implemented header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "include/drvCPE_EHCI.h"
#include "drvEHCI.h"
#include "drvUSBHwCtl.h"
#include "drvUsbHostConfig.h"
#include "include/drvCPE_AMBA.h"
#include "drvHUB.h"

#define EHCI_DEBUG
#ifdef EHCI_DEBUG
#define EHCI_Dbg		diag_printf
#else
#define EHCI_Dbg(x,...)
#endif

#define EHCI_ERR		diag_printf

struct cpe_dev cpe_ehci_dev_Port2;
struct cpe_driver ehci_hcd_cpe_driver_Port2;
struct hc_driver ehci_cpe_hc_driver_Port2;
struct usb_hcd *g_pUsbHcd_Port2 = NULL;

extern void cpe_ehci_start_hc(struct cpe_dev *dev);
extern void usb_hcd_cpe_ehci_remove (struct usb_hcd *hcd, struct cpe_dev *dev);
extern void cpe_ehci_stop_hc(struct cpe_dev *dev);
extern void ehci_irq (struct usb_hcd *hcd, struct pt_regs *regs);
extern int ehci_hc_reset (struct usb_hcd *hcd);
extern  int ehci_start (struct usb_hcd *hcd);
extern int ehci_suspend (struct usb_hcd *hcd, U32 state);
extern int ehci_resume (struct usb_hcd *hcd);
extern void ehci_stop (struct usb_hcd *hcd);
extern  struct usb_hcd *ehci_hcd_alloc (void);
extern  void ehci_hcd_free (struct usb_hcd *hcd);
extern  int ehci_urb_enqueue (struct usb_hcd  *hcd, struct urb  *urb, int    mem_flags);
extern  int ehci_urb_dequeue (struct usb_hcd *hcd, struct urb *urb);
extern  void ehci_endpoint_disable (struct usb_hcd *hcd, struct hcd_dev *dev, int ep);
extern int ehci_get_frame (struct usb_hcd *hcd);
extern int ehci_hub_status_data (struct usb_hcd *hcd, char *buf);
extern  int ehci_hub_control (struct usb_hcd  *hcd, U16 typeReq, U16 wValue, U16 wIndex, char *buf, U16 wLength);

extern struct list_head hub_event_list_Port2;
extern void init_OS_Resource_EX(S32 *USBWaitFlg);
extern int ehci_hcd_cpe_ehci_drv_remove(struct device_s *_dev);
extern int ehci_hcd_cpe_ehci_drv_suspend(struct device_s *_dev, U32 state, U32 level);
extern int ehci_hcd_cpe_ehci_drv_resume(struct device_s *_dev, U32 level);
extern struct bus_type CPE_AMBA_bus_type_Port2;


int usb_hcd_cpe_ehci_probe_Port2(struct hc_driver *driver,
        struct usb_hcd **hcd_out,
        struct cpe_dev *dev)
{
	int retval;
	struct usb_hcd *hcd = 0;
	//Basic initialization for FPFA version IP
//	mwPeri20_Control_ChipEnable_Set();
//    #ifdef CYGPKG_USB_HALF_SPEED_MODE
//	mwPeri20_Control_HALFSPEEDEnable_Set();
 //  #endif

//#ifdef FORCE_FULL_SPEED
	// Force HC to full speed
	//mwOTG20_Control_ForceFullSpeed_Set();
	//mwOTG20_Control_ForceHighSpeed_Clr();
//#endif
//#ifdef FORCE_HIGH_SPEED
	// Force HC to high speed
	//mwOTG20_Control_ForceFullSpeed_Clr();
	//mwOTG20_Control_ForceHighSpeed_Set();
//#endif


       diag_printf("usb_hcd_cpe_ehci_probe port2\n");
	cpe_ehci_start_hc(dev);

	/*
	* driver->hcd_alloc() will allocate a memory space of (struct ehci_hcd) and then return the reference of ehci_hcd.hcd back
	* Therefore, driver can access the member of ehci_hcd later.
	* driver->hcd_alloc is a function point of ehci_hcd_alloc()
	*/

	hcd = driver->hcd_alloc();
       diag_printf("hcd: %lx\n", (U32)hcd);
	if (hcd == NULL){
		dbg ("hcd_alloc failed%s"," ");
		retval = -ENOMEM;
		goto err1;
	}

	hcd->driver = (struct hc_driver *) driver;
	//hcd->description = driver->description;

	memcpy((char*)hcd,(char*)driver,24);

	hcd->irq  = gIrqUHC2; //CPE_USB_EHCI1_IRQ;
	hcd->regs = (U32*) gBaseUHC2;
        hcd->host_id = 2; // new
        hcd->phub_event = &hub_event_list_Port2;
	hcd->pdev = (struct pci_dev  *)(1111); //Fake PCI device
	hcd->self.controller = &dev->dev;
	hcd->controller = hcd->self.controller;

	retval = hcd_buffer_create (hcd);
	if (retval != 0) {
		dbg ("pool alloc fail%s","\n");
		goto err1;
	}

	/* till now HC has been in an indeterminate state ... */
	if (driver->reset && (retval = driver->reset (hcd)) < 0) {
		dev_err (hcd->controller, "can't reset%s","\n");
		goto err2;
	}
	// Initialize IRQ and ISR Trap, enable interrupt later (at the end)
#if 0
	fLib_CloseInt(hcd->irq);
	fLib_SetIntTrig(hcd->irq,LEVEL,L_ACTIVE);
	retval = fLib_ReqInt (hcd->irq, usb_hcd_cpe_ehci_hcim_irq, hcd);
	if ( retval == FALSE )
	{
		err("Register ISR fail%s","");
		retval = -EINVAL;
		goto err1;
	}
#ifdef CONFIG_OTG
#ifndef CONFIG_FARADAY_FOTG200
	// Enable CPE EHCI HC's Interrupt
	fLib_EnableInt(hcd->irq);
#else
	// Enable EHCI interrupt after intilizing OTG controller driver
#endif
#else
	// Do OTG support ==> Enable CPE EHCI HC's Interrupt now
	fLib_EnableInt(hcd->irq);
#endif
#endif
	//mwOTG20_Interrupt_Mask_HOST_Clr();//yuwen mark

#if 0
	if (retval != TRUE) {
		dbg("request_irq failed%s"," ");
		retval = -EBUSY;
		goto err2;
	}
#endif

	usb_info ("%s (CPE_AMBA) at 0x%p, irq %d\n",
		hcd->description, hcd->regs, hcd->irq);
	usb_bus_init (&hcd->self);
	hcd->self.op = &usb_hcd_operations;
	hcd->self.hcpriv = (void *) hcd;
	hcd->self.bus_name = "cpe_ehci_2";
	//hcd->product_desc = "CPE_AMBA EHCI";
	memcpy(hcd->product_desc, "CPE_AMBA EHCI 2", sizeof(hcd->product_desc));

        hcd->pre_sts = 0; // new
        hcd->pre_temp = 0; // new
        init_OS_Resource_EX(&hcd->USBWaitFlg);
	INIT_LIST_HEAD (&hcd->dev_list);
	usb_register_bus (&hcd->self);    //For USB2, marked???

	if ((retval = driver->start (hcd)) < 0)
	{
		usb_hcd_cpe_ehci_remove(hcd, dev);
		return retval;
	}

	*hcd_out = hcd;

	return retval;

err2:
	hcd_buffer_destroy (hcd);
	if (hcd)
	driver->hcd_free(hcd);
err1:
	cpe_ehci_stop_hc(dev);
	//release_mem_region(dev->res.start, dev->res.end - dev->res.start + 1);
	return retval;
}
static void cpe_ehci_hcd_init_Port2(void)
{
	memcpy( ehci_cpe_hc_driver_Port2.description ,  "ehci_hcd_2", sizeof("ehci_hcd_2")+1);

	/*
	* generic hardware linkage
	*/
	ehci_cpe_hc_driver_Port2.irq =      ehci_irq;
	ehci_cpe_hc_driver_Port2.flags =    HCD_MEMORY | HCD_USB2;

	/*
	* basic lifecycle operations
	*/
	ehci_cpe_hc_driver_Port2.reset =    ehci_hc_reset;
	ehci_cpe_hc_driver_Port2.start =    ehci_start;
	ehci_cpe_hc_driver_Port2.suspend =  ehci_suspend;
	ehci_cpe_hc_driver_Port2.resume  =  ehci_resume;
	ehci_cpe_hc_driver_Port2.stop    =  ehci_stop;

	/*
	* memory lifecycle (except per-request)
	*/
	ehci_cpe_hc_driver_Port2.hcd_alloc =  ehci_hcd_alloc;
	ehci_cpe_hc_driver_Port2.hcd_free  =  ehci_hcd_free;

	/*
	* managing i/o requests and associated device resources
	*/
	ehci_cpe_hc_driver_Port2.urb_enqueue      = ehci_urb_enqueue;
	ehci_cpe_hc_driver_Port2.urb_dequeue      = ehci_urb_dequeue;
	ehci_cpe_hc_driver_Port2.endpoint_disable =  ehci_endpoint_disable;

	/*
	* scheduling support
	*/
	ehci_cpe_hc_driver_Port2.get_frame_number =  ehci_get_frame;

	/*
	* root hub support
	*/
	ehci_cpe_hc_driver_Port2.hub_status_data =  ehci_hub_status_data;
	ehci_cpe_hc_driver_Port2.hub_control     =  ehci_hub_control;
}

extern void InitUSBIntr_Port2(struct usb_hcd * hcd);
static int ehci_hcd_cpe_ehci_drv_probe_Port2(struct device_s *_dev)
{
    struct cpe_dev *dev;
    struct usb_hcd *hcd = NULL;
    int ret;

    const struct device_s *__mptr = _dev;
    dev = (struct cpe_dev *)( (char *)__mptr - (char *)offsetof(struct cpe_dev,dev) );

    EHCI_Dbg("ehci_hcd_cpe_ehci_drv_probe_Port2 enter\n");
    if (usb_disabled())
        return -ENODEV;

    cpe_ehci_hcd_init_Port2();

    ret = usb_hcd_cpe_ehci_probe_Port2(&ehci_cpe_hc_driver_Port2, &hcd, dev);

    if (ret == 0)
    {
        dev->dev.driver_data = hcd;
        g_pUsbHcd_Port2 = hcd;
        InitUSBIntr_Port2(hcd);					//add USB interrupt entry . yuwen
    }

    return ret;
}
static int __init ehci_hcd_cpe_ehci_init_Port2(void)
{
	pr_debug ("ehci_hcd: block sizes: qh %d qtd %d itd %d sitd %d\n",
				sizeof (struct ehci_qh), sizeof (struct ehci_qtd),
				sizeof (struct ehci_itd), sizeof (struct ehci_sitd));
	strncpy(ehci_hcd_cpe_driver_Port2.drv.name, "cpe_ehci_2", sizeof("cpe_ehci_2"));
	ehci_hcd_cpe_driver_Port2.drv.bus     = &CPE_AMBA_bus_type_Port2;
	ehci_hcd_cpe_driver_Port2.drv.probe   = ehci_hcd_cpe_ehci_drv_probe_Port2;
	ehci_hcd_cpe_driver_Port2.drv.remove  = ehci_hcd_cpe_ehci_drv_remove;
	ehci_hcd_cpe_driver_Port2.drv.suspend = ehci_hcd_cpe_ehci_drv_suspend;
	ehci_hcd_cpe_driver_Port2.drv.resume  = ehci_hcd_cpe_ehci_drv_resume;
	ehci_hcd_cpe_driver_Port2.devid       = CPE_DEVID_USB_PORT2;
	return driver_register(&ehci_hcd_cpe_driver_Port2.drv);
}


int __init ehci_cpe_dev_init_Port2(void)
{
	// Cannot remove: Init list heads of device and driver lists of CPE EHCI device
	EHCI_Dbg("ehci_cpe_dev_init_Port2 enter\n");
	device_initialize(&cpe_ehci_dev_Port2.dev);
#ifdef DEBUG
	//dbg (DRIVER_INFO " Init CPE EHCI Host Controller%s"," ");
#endif
	// Init CPE EHCI device data structure
	cpe_ehci_dev_Port2.dev.parent =  NULL; //parent should be CPE AMBA
	strncpy(cpe_ehci_dev_Port2.dev.name,"CPE_EHCI HC 2", sizeof("CPE_EHCI HC 2"));
	strncpy(cpe_ehci_dev_Port2.dev.bus_id,"AHB0_EHCI_2", sizeof("AHB0_EHCI_2"));
	cpe_ehci_dev_Port2.dev.bus  = &CPE_AMBA_bus_type_Port2;
	cpe_ehci_dev_Port2.devid    = CPE_DEVID_USB_PORT2;
	cpe_ehci_dev_Port2.mapbase  = gBaseUHC2; 
	cpe_ehci_dev_Port2.utmibase  = gBaseUTMI2;  
        cpe_ehci_dev_Port2.usbcbase  = gBaseUSBC2;
	cpe_ehci_dev_Port2.dma_mask = 0x00; //Unsupport DMA
	//register cpe_ehci devie to CPE AMBA bus
	device_add(&cpe_ehci_dev_Port2.dev);

	return ehci_hcd_cpe_ehci_init_Port2();
}

