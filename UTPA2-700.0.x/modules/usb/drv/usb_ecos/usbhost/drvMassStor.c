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
#include <MsCommon.h>
#include  "include/drvConfig.h"
#include  "include/drvErrno.h"
#include "include/drvUSB.h"
//#include "scsiglue.h"
#include "drvTransport.h"
#include "drvProtocol.h"
//#include "initializers.h"
#include "include/drvBitops.h"
#include "include/drvCPE_AMBA.h"
#include "drvUsbHostConfig.h"
#include "drvHCD.h"

#if 1

#ifdef CONFIG_USB_STORAGE_HP8200e
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "shuttle_usbat.h"
#endif

#ifdef CONFIG_USB_STORAGE_SDDR09
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "sddr09.h"
#endif

#ifdef CONFIG_USB_STORAGE_SDDR55
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "sddr55.h"
#endif

#ifdef CONFIG_USB_STORAGE_DPCM
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "dpcm.h"
#endif

#ifdef CONFIG_USB_STORAGE_FREECOM
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "freecom.h"
#endif

#ifdef CONFIG_USB_STORAGE_ISD200
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "isd200.h"
#endif

#ifdef CONFIG_USB_STORAGE_DATAFAB
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "datafab.h"
#endif

#ifdef CONFIG_USB_STORAGE_JUMPSHOT
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#include "jumpshot.h"
#endif

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifdef MASS_DEBUG
#define US_DEBUGP(x,...) diag_printf(x,__VA_ARGS__)
#else
#define US_DEBUGP(x,...)
#endif

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern struct cpe_dev cpe_ehci_dev;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int storage_probe(struct usb_interface *iface, const struct usb_device_id *id);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void storage_disconnect(struct usb_interface *iface);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern void RemoveUSBDiskPort(U8 uPort, U8 uLunNum);


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define USB_STOR_DEVICE_VER(vend,prod,lo,hi) \
	 USB_DEVICE_ID_MATCH_DEVICE_AND_VERSION, (vend), (prod), (lo), (hi),0,0,0,0,0,0,0

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define UNUSUAL_DEV(id_vendor, id_product, bcdDeviceMin, bcdDeviceMax, \
		    vendorName, productName,useProtocol, useTransport, \
		    initFunction, flags) \
{ USB_STOR_DEVICE_VER(id_vendor, id_product, bcdDeviceMin,bcdDeviceMax) }

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define GET_INTERFACE_INFO(cl,sc,pr) \
	 USB_DEVICE_ID_MATCH_INT_INFO,0,0,0,0,0,0,0, (cl), (sc), (pr),0

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#undef UNUSUAL_DEV
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define UNUSUAL_DEV(idVendor, idProduct, bcdDeviceMin, bcdDeviceMax, \
		    vendor_name, product_name, use_protocol, use_transport, \
		    init_function, Flags) \
{ \
	 vendor_name,	\
	 product_name,	\
	 use_protocol,	\
	 use_transport,	\
	 init_function,	\
	 Flags \
}
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static struct usb_device_id storage_usb_ids [] =
{

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//#include "unusual_devs.h"
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//#undef UNUSUAL_DEV

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 	
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_RBC, US_PR_CB) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE,US_SC_8020,US_PR_CB) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE,US_SC_QIC,US_PR_CB) },
        
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE,US_SC_UFI,US_PR_CB) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE,US_SC_8070,US_PR_CB) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE,US_SC_SCSI,US_PR_CB) },
	
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_RBC, US_PR_CBI) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_8020, US_PR_CBI) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_QIC, US_PR_CBI) },
   
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_UFI, US_PR_CBI) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_8070, US_PR_CBI) },        
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_SCSI, US_PR_CBI) },

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_RBC, US_PR_BULK) },
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_8020, US_PR_BULK) },
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_QIC, US_PR_BULK) },

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_UFI, US_PR_BULK) },
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_8070, US_PR_BULK) },
	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, US_SC_SCSI, US_PR_BULK) },

	{GET_INTERFACE_INFO (USB_CLASS_STORAGE, 0xFF, US_PR_CB) },
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{ 0,0,0,0,0,0,0,0,0,0,0,0}        
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static struct us_unusual_dev us_unusual_dev_list[] =
{
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//#include "unusual_devs.h"
//#undef UNUSUAL_DEV
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{ 0,0, US_SC_RBC,
	   US_PR_CB,0,0},
	{ 0,0, US_SC_8020,
	   US_PR_CB,0,0},
	{ 0,0, US_SC_QIC,
	   US_PR_CB,0,0},
	{ 0,0,  US_SC_UFI,
	   US_PR_CB,0,0},
	{ 0,0,  US_SC_8070,
	   US_PR_CB,0,0},
	{ 0,0,  US_SC_SCSI,
	   US_PR_CB,0,0},

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{ 0,0,  US_SC_RBC,
	   US_PR_CBI,0,0},
	{  0,0,  US_SC_8020,
	   US_PR_CBI,0,0},
	{ 0,0,  US_SC_QIC,
	   US_PR_CBI,0,0},
	{ 0,0,  US_SC_UFI,
	   US_PR_CBI,0,0},
	{  0,0, US_SC_8070,
	   US_PR_CBI,0,0},
	{  0,0,  US_SC_SCSI,
	   US_PR_CBI,0,0},

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{  0,0,  US_SC_RBC,
	   US_PR_BULK,0,0},
	{  0,0,  US_SC_8020,
	   US_PR_BULK,0,0},
	{  0,0,  US_SC_QIC,
	   US_PR_BULK,0,0},
	{  0,0,  US_SC_UFI,
	   US_PR_BULK,0,0},
	{  0,0,  US_SC_8070,
	   US_PR_BULK,0,0},
	{  0,0,  US_SC_SCSI,
	   US_PR_BULK,0,0},

	{  0,0,  0xFF,
	   US_PR_CB,0,0},

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	{ 0,0,0,0,0,0 }
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct usb_driver usb_storage_driver =
{
	(void*) 0,
	"usb-storage",
	storage_probe,
	storage_disconnect,
	0,
	storage_usb_ids
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#if 1
void fill_inquiry_response(struct us_data *us, unsigned char *data,
							U32 data_len)
{
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	//int i;
//	struct scatterlist *sg;		//YPING??
	int len =
		us->srb.request_bufflen > data_len ? data_len :
		us->srb.request_bufflen;
	//int transferred;
	//int amt;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	if (data_len<36) // You lose.
		return;

	if(data[0]&0x20)
	{    	
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		memset(data+8,0,28);
	}
	else
	{
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		memcpy(data+8, us->unusual_dev->vendorName,
			strlen(us->unusual_dev->vendorName) > 8 ? 8 :
			strlen(us->unusual_dev->vendorName));

                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
		memcpy(data+16, us->unusual_dev->productName,
			strlen(us->unusual_dev->productName) > 16 ? 16 :
			strlen(us->unusual_dev->productName));

                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		data[32] = 0x30 + ((us->pusb_dev->descriptor.bcdDevice>>12) & 0x0F);
		data[33] = 0x30 + ((us->pusb_dev->descriptor.bcdDevice>>8) & 0x0F);
		data[34] = 0x30 + ((us->pusb_dev->descriptor.bcdDevice>>4) & 0x0F);
		data[35] = 0x30 + ((us->pusb_dev->descriptor.bcdDevice) & 0x0F);
	}


        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
	if (us->srb.use_sg)
	{ 
		printk("fill_inquiry_response: Not support scatter function.");
	}
	else
	{
		memset(us->srb.request_buffer, 0, us->srb.request_bufflen);
		memcpy(us->srb.request_buffer, data, len);
	}
}
#endif

int usb_stor_control_thread(void * __us)
{
	struct us_data *us = (struct us_data *)__us;

//	lock_kernel();

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*	daemonize("usb-storage");

	current->flags |= PF_IOTHREAD;

	unlock_kernel();
*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	complete(&(us->notify));

//	for(;;)
	{
//		struct Scsi_Host *host;			//YPING01??
		US_DEBUGP("*** thread sleeping.%s","\n");
//		if(down_interruptible(&us->sema))
//			break;

		US_DEBUGP("*** thread awakened.%s","\n");

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//		if (us->srb == NULL)
//		{
//			US_DEBUGP("-- exit command received%s","\n");
//			goto SkipForAbort;
//			break;
//		}
//		host = us->srb.device->host;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//		scsi_lock(host);

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (us->sm_state == US_STATE_ABORTING)
		{
			us->srb.result = DID_ABORT << 16;
			goto SkipForAbort;
		}

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->sm_state = US_STATE_RUNNING;
//		scsi_unlock(host);

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		down(&(us->dev_semaphore));

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

		if (test_bit(US_FLIDX_DISCONNECTING, &us->flags))
		{
			US_DEBUGP("No command during disconnect%s","\n");
			us->srb.result = DID_BAD_TARGET << 16;
		}
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		else if (us->srb.sc_data_direction == SCSI_DATA_UNKNOWN)
		{
			US_DEBUGP("UNKNOWN data direction%s","\n");
			us->srb.result = DID_ERROR << 16;
		}
		
#if 0                
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		else if (us->srb.device->id &&
				!(us->flags & US_FL_SCM_MULT_TARG))
		{
			US_DEBUGP("Bad target number (%d:%d)\n",
				  us->srb.device->id, us->srb.device->lun);
			us->srb.result = DID_BAD_TARGET << 16;
		}

		else if (us->srb.device->lun > us->max_lun)
		{
			US_DEBUGP("Bad LUN (%d:%d)\n",
				  us->srb.device->id, us->srb.device->lun);
			us->srb.result = DID_BAD_TARGET << 16;
		}
#endif
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		else if ((us->srb.cmnd[0] == INQUIRY) &&
			    (us->flags & US_FL_FIX_INQUIRY)) {
			unsigned char data_ptr[36] = {
			    0x00, 0x80, 0x02, 0x02,
			    0x1F, 0x00, 0x00, 0x00};

			US_DEBUGP("Faking INQUIRY command%s","\n");
			fill_inquiry_response(us, data_ptr, 36);
			us->srb.result = SAM_STAT_GOOD;
		}

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		else
		{
//			US_DEBUGP(usb_stor_show_command(us->srb)"%s","\n");		//xyz
			us->proto_handler(&us->srb, us);
		}

		/* unlock the device pointers */
		up(&(us->dev_semaphore));

		/* lock access to the state */
//		scsi_lock(host);

		/* indicate that the command is done */
		if (us->srb.result != DID_ABORT << 16)
		{
			US_DEBUGP("scsi cmd done, result=0x%x\n",
				   us->srb.result);

			if(us->srb.scsi_done != NULL)
				us->srb.scsi_done(&us->srb);
		}
		else
		{
			SkipForAbort:
			US_DEBUGP("scsi command aborted%s","\n");
		}

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (us->sm_state == US_STATE_ABORTING)
			complete(&(us->notify));

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//		us->srb = NULL;			//YPING01??
		us->sm_state = US_STATE_IDLE;
//		scsi_unlock(host);
	} /* for (;;) */

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	complete(&(us->notify));

	return 0;
}

static int associate_dev(struct us_data *us, struct usb_interface *intf)
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
{  

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->pusb_dev = interface_to_usbdev(intf);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	us->pusb_intf = intf;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	us->ifnum = intf->altsetting->desc.bInterfaceNumber;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_set_intfdata(intf, us);
	usb_get_dev(us->pusb_dev);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->cr = (struct usb_ctrlrequest*) usb_buffer_alloc(us->pusb_dev, sizeof(*us->cr), GFP_KERNEL, &us->cr_dma);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/          


        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (!us->cr) 
        {
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		US_DEBUGP("[associate_dev] usb_ctrlrequest allocation failed%s","\n");
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		return -ENOMEM;
	}

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	us->iobuf = (unsigned char*) usb_buffer_alloc(us->pusb_dev, US_IOBUF_SIZE, GFP_KERNEL, &us->iobuf_dma);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (!us->iobuf) {
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
		US_DEBUGP("[associate_dev] I/O buffer allocation failed%s","\n");
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
		return -ENOMEM;
	}

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	return 0;
}

static void get_device_info(struct us_data *us, int id_index)
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
{

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_interface_descriptor *idesc =
		&us->pusb_intf->altsetting[us->pusb_intf->act_altsetting].desc;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
	struct us_unusual_dev *unusual_dev = &us_unusual_dev_list[id_index];

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_device_id *id = &storage_usb_ids[id_index];

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (unusual_dev->vendorName)
		US_DEBUGP("[get_device_info] Vendor: %s\n", unusual_dev->vendorName);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (unusual_dev->productName)
		US_DEBUGP("[get_device_info] Product: %s\n", unusual_dev->productName);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->unusual_dev = unusual_dev;
	us->subclass = (unusual_dev->useProtocol == US_SC_DEVICE) ?
			idesc->bInterfaceSubClass :
			unusual_dev->useProtocol;
	us->protocol = (unusual_dev->useTransport == US_PR_DEVICE) ?
			idesc->bInterfaceProtocol :
			unusual_dev->useTransport;
	us->flags = unusual_dev->flags;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (id->idVendor || id->idProduct) {

		//struct usb_device_descriptor *ddesc = &dev->descriptor;
		int msg = -1;

		if (unusual_dev->useProtocol != US_SC_DEVICE &&
			us->subclass == idesc->bInterfaceSubClass)
			msg += 1;
		if (unusual_dev->useTransport != US_PR_DEVICE &&
			us->protocol == idesc->bInterfaceProtocol)
			msg += 2;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (strlen(us->vendor) == 0) {
		if (unusual_dev->vendorName)
			strncpy(us->vendor, unusual_dev->vendorName,
				sizeof(us->vendor));
		else
			strncpy(us->vendor, "Unknown", sizeof(us->vendor));
	}
	if (strlen(us->product) == 0) {
		if (unusual_dev->productName)
			strncpy(us->product, unusual_dev->productName,
				sizeof(us->product));
		else
			strncpy(us->product, "Unknown", sizeof(us->product));
	}
	if (strlen(us->serial) == 0)
		strncpy(us->serial, "None", sizeof(us->serial));
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int get_transport(struct us_data *us)
{
	switch (us->protocol) {
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case US_PR_CB:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->transport_name = "Control/Bulk";
		us->transport = usb_stor_CB_transport;
		us->transport_reset = usb_stor_CB_reset;
		//us->max_lun = 7;
		us->max_lun = 0;	//FIXME, Find some way to get real Max. LUN
		break;

	case US_PR_CBI:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		us->transport_name = "Control/Bulk/Interrupt";
		us->transport = usb_stor_CBI_transport;
		us->transport_reset = usb_stor_CB_reset;
		us->max_lun = 0;
		break;        
        
	case US_PR_BULK:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->transport_name = "Bulk";
		us->transport = usb_stor_Bulk_transport;
		us->transport_reset = usb_stor_Bulk_reset;
		break;

#ifdef CONFIG_USB_STORAGE_HP8200e  
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/       
	case US_PR_SCM_ATAPI:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->transport_name = "SCM/ATAPI";
		us->transport = hp8200e_transport;
		us->transport_reset = usb_stor_CB_reset;
		us->max_lun = 1;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_SDDR09 
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/         
	case US_PR_EUSB_SDDR09:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		us->transport_name = "EUSB/SDDR09";
		us->transport = sddr09_transport;
		us->transport_reset = usb_stor_CB_reset;
		us->max_lun = 0;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_SDDR55
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/        
	case US_PR_SDDR55:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->transport_name = "SDDR55";
		us->transport = sddr55_transport;
		us->transport_reset = sddr55_reset;
		us->max_lun = 0;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_DPCM  
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/      
	case US_PR_DPCM_USB:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->transport_name = "Control/Bulk-EUSB/SDDR09";
		us->transport = dpcm_transport;
		us->transport_reset = usb_stor_CB_reset;
		us->max_lun = 1;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_FREECOM
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case US_PR_FREECOM:
		us->transport_name = "Freecom";
		us->transport = freecom_transport;
		us->transport_reset = usb_stor_freecom_reset;
		us->max_lun = 0;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_DATAFAB  
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/      
	case US_PR_DATAFAB:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		us->transport_name  = "Datafab Bulk-Only";
		us->transport = datafab_transport;
		us->transport_reset = usb_stor_Bulk_reset;
		us->max_lun = 1;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_JUMPSHOT  
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/      
	case US_PR_JUMPSHOT:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
		us->transport_name  = "Lexar Jumpshot Control/Bulk";
		us->transport = jumpshot_transport;
		us->transport_reset = usb_stor_Bulk_reset;
		us->max_lun = 1;
		break;
#endif

	default:
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		return -EIO;
	}
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Transport: %s\n", us->transport_name);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->flags & US_FL_SINGLE_LUN)
		us->max_lun = 0;
	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int get_protocol(struct us_data *us)
{
	switch (us->subclass) {
	case US_SC_RBC:
		us->protocol_name = "Reduced Block Commands (RBC)";
//		USB_sprintf(us->protocol_name,"Reduced Block Commands (RBC)");
		us->proto_handler = usb_stor_transparent_scsi_command;
		break;

	case US_SC_8070:
	case US_SC_SCSI:
	case 0xFF:	// For Sony DC P8 (P series) and Sony DV
		us->protocol_name = "Transparent SCSI";
//		USB_sprintf(us->protocol_name,"Transparent SCSI");
		us->proto_handler = usb_stor_transparent_scsi_command;
		break;

	case US_SC_8020:
		us->protocol_name = "8020i";
		us->proto_handler = usb_stor_transparent_scsi_command;
		us->max_lun = 0;
		break;

#if SupportOtherDevice
#if 0
	case US_SC_8020:
		us->protocol_name = "8020i";
		us->proto_handler = usb_stor_ATAPI_command;
		us->max_lun = 0;
		break;
#endif

	case US_SC_QIC:
		us->protocol_name = "QIC-157";
		us->proto_handler = usb_stor_qic157_command;
		us->max_lun = 0;
		break;
#if 0
	case US_SC_8070:
		us->protocol_name = "8070i";
		us->proto_handler = usb_stor_ATAPI_command;
		us->max_lun = 0;
		break;
#endif

	case US_SC_UFI:
		us->protocol_name = "Uniform Floppy Interface (UFI)";
		us->proto_handler = usb_stor_ufi_command;
		break;
#endif

#ifdef CONFIG_USB_STORAGE_ISD200
	case US_SC_ISD200:
		us->protocol_name = "ISD200 ATA/ATAPI";
		us->proto_handler = isd200_ata_command;
		break;
#endif

	default:
		return -EIO;
	}
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Protocol: %s\n", us->protocol_name);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int get_pipes(struct us_data *us)
{
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_host_interface *altsetting =	&us->pusb_intf->altsetting[us->pusb_intf->act_altsetting];

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	int ii;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_endpoint_descriptor *ep_tmp;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_endpoint_descriptor *ep_in = NULL; 
       
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_endpoint_descriptor *ep_out = NULL;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct usb_endpoint_descriptor *ep_int = NULL;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	for (ii = 0; ii < altsetting->desc.bNumEndpoints; ii++) 
        {
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		ep_tmp=&altsetting->endpoint[ii].desc;                 
		
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if ((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)                 
                {                        
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			if (ep_tmp->bEndpointAddress & USB_DIR_IN)
                        {                                  
                                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
				ep_in = ep_tmp;                                  
                        }
			else
                        {                                
                                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
				ep_out = ep_tmp;
                        }
		}
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/		
		else 
                if ((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)                 
                             == USB_ENDPOINT_XFER_INT)                 
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
                {                        
                        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
                       	ep_int = ep_tmp;                        

		}
	}    
             

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (!ep_in || !ep_out || (us->protocol == US_PR_CBI && !ep_int)) 
        {
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		US_DEBUGP("Endpoint sanity check failed! Rejecting dev.%s","\n");
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		return -EIO;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->send_ctrl_pipe = usb_sndctrlpipe(us->pusb_dev, 0);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->recv_ctrl_pipe = usb_rcvctrlpipe(us->pusb_dev, 0);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	us->send_bulk_pipe = usb_sndbulkpipe(us->pusb_dev,
		ep_out->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->recv_bulk_pipe = usb_rcvbulkpipe(us->pusb_dev,
		ep_in->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
        
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (ep_int) {
#if 1    //
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		us->recv_intr_pipe = usb_rcvbulkpipe(us->pusb_dev,
			ep_int->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
#else
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->recv_intr_pipe = usb_rcvintpipe(us->pusb_dev,
			ep_int->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
#endif
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		us->ep_bInterval = ep_int->bInterval;
	}
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int usb_stor_acquire_resources(struct us_data *us)
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
{
	//int p;

	us->current_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!us->current_urb) {
		US_DEBUGP("URB allocation failed%s","\n");
		return -ENOMEM;
	}
	us->data_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!us->data_urb) {
		US_DEBUGP("URB allocation failed%s","\n");
		return -ENOMEM;
	}
	us->status_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!us->status_urb) {
		US_DEBUGP("URB allocation failed%s","\n");
		return -ENOMEM;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	down(&us->dev_semaphore);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->protocol == US_PR_BULK)
		us->max_lun = usb_stor_Bulk_max_lun(us);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->Mass_stor_device=(struct LUN_Device*)kmalloc(sizeof(struct LUN_Device)*(us->max_lun+1), GFP_KERNEL);
	memset(us->Mass_stor_device,0 , sizeof(struct LUN_Device)*(us->max_lun+1));

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->unusual_dev->initFunction)
		us->unusual_dev->initFunction(us);

	up(&us->dev_semaphore);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->sm_state = US_STATE_IDLE;
/*			//YPING??
	p = kernel_thread(usb_stor_control_thread, us, CLONE_VM);
	if (p < 0) {
		printk(KERN_WARNING USB_STORAGE
		       "Unable to start control thread\n");
		return p;
	}
	us->pid = p;
*/

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	wait_for_completion(&(us->notify));

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*	    	//YPING??
	us->host = scsi_host_alloc(&usb_stor_host_template, sizeof(us));
	if (!us->host) {
		printk(KERN_WARNING USB_STORAGE
			"Unable to register the scsi host\n");
		return -EBUSY;
	}
*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	us->host->hostdata[0] = (unsigned long) us;		//YPING??

	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void dissociate_dev(struct us_data *us)
{

	US_DEBUGP("-- %s\n", __FUNCTION__);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	down(&us->dev_semaphore);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->cr) {
		usb_buffer_free(us->pusb_dev, sizeof(*us->cr), us->cr,
				us->cr_dma);
		us->cr = NULL;
	}
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	if (us->iobuf) {
		usb_buffer_free(us->pusb_dev, US_IOBUF_SIZE, us->iobuf,
				us->iobuf_dma);
		us->iobuf = NULL;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_set_intfdata(us->pusb_intf, NULL);
	usb_put_dev(us->pusb_dev);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->pusb_dev = NULL;
	us->pusb_intf = NULL;
	up(&us->dev_semaphore);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_stor_release_resources(struct us_data *us)
{
diag_printf("usb_stor_release_resources\n");
    MS_U8   ii, iLun;
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*					//YPING??
	if (us->host) {
		(struct us_data *) us->host->hostdata[0] = NULL;
		scsi_host_put(us->host);
	}
*/

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*	    			//YPING??
	if (us->pid) {
		US_DEBUGP("-- sending exit command to thread\n");
		BUG_ON(us->sm_state != US_STATE_IDLE);
		us->srb = NULL;
		up(&(us->sema));
		wait_for_completion(&(us->notify));
	}
*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->extra_destructor) {
		US_DEBUGP("-- calling extra_destructor()%s","\n");
		us->extra_destructor(us->extra);
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->extra)
		kfree(us->extra);
	if (us->current_urb)
		usb_free_urb(us->current_urb);
	if (us->data_urb)
		usb_free_urb(us->data_urb);
	if (us->status_urb)
		usb_free_urb(us->status_urb);

	/* Free the structure itself */
	//vUSBDisk_FileSystem_Remove();

    for (ii=0; ii<NUM_OF_ROOT_HUB*MAX_USTOR; ii++)
    {
        if (Mass_stor_us[ii] == us)
        {
            for (iLun=0; iLun<=Mass_stor_us[ii]->max_lun; iLun++)
                RemoveUSBDiskPort(Mass_stor_us[ii]->ustor_id, iLun);
        
            Mass_stor_us[ii] = NULL;
        }    
    }
    if (us->Mass_stor_device)
        kfree(us->Mass_stor_device);

  kfree(us);
 // US_DEBUGP("-- %s finished\n", __FUNCTION__);
//  US_DEBUGP("USB Mass Storage device disconnected\n");
}

/* Get free ID from MAX_USTOR during one host controller support range */
int GetUStorID(unsigned char startId)
{
    int i;
    
    if (startId >= NUM_OF_ROOT_HUB*MAX_USTOR)
        ASSERT(0);

    for (i=startId; i<startId+MAX_USTOR; i++)
    {
        if (Mass_stor_us[i] == NULL)
            return i;
    }

    diag_printf("!! ERROR: Exceed the max support number of usb storage device !!\n");
    return (-1);
}


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int storage_probe(struct usb_interface *intf,
			 const struct usb_device_id *id)
{

	struct us_data *us;
	const int id_index = id - storage_usb_ids;
	int result,i;
	struct usb_device *pudev;

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        diag_printf("storage_probe\n");
	US_DEBUGP("USB Mass Storage device detected%s","\n");
	US_DEBUGP("act_altsetting is %d, id_index is %d\n",
			intf->act_altsetting, id_index);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us = (struct us_data *) kmalloc(sizeof(*us), GFP_KERNEL);
	if (!us) 
        {
		US_DEBUGP("Out of memory %d\n", us);
		return -ENOMEM;
	}

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	memset(us, 0, sizeof(struct us_data)); 
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        init_MUTEX(&(us->dev_semaphore));      
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	init_MUTEX_LOCKED(&(us->sema));        
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	init_completion(&(us->notify));         

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = associate_dev(us, intf); 
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result)
	      goto BadDevice;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	get_device_info(us, id_index);

#ifdef CONFIG_USB_STORAGE_SDDR09
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->protocol == US_PR_EUSB_SDDR09 || us->protocol == US_PR_DPCM_USB) 
        {
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		result = usb_set_configuration(us->pusb_dev, 1);
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
		US_DEBUGP("Result from usb_set_configuration is %d\n", result);
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (result == -EPIPE) 
                {
                        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			US_DEBUGP("-- stall on control interface\n");
		} 
                /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
                else if (result != 0) 
                {
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			US_DEBUGP("-- Unknown error.  Rejecting device\n");
                        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			goto BadDevice;
		}
	}
#endif

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = get_transport(us);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result)
		goto BadDevice;
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = get_protocol(us);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	if (result)
		goto BadDevice;
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = get_pipes(us);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
	if (result)
		goto BadDevice;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = usb_stor_acquire_resources(us);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result)
		goto BadDevice;
	
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	pudev = interface_to_usbdev(intf);
        i = GetUStorID(MAX_USTOR* (((struct usb_hcd *)pudev->bus->hcpriv)->host_id));
    
    if (i != (-1))
    {
        Mass_stor_us[i] = us;
        us->ustor_id = (unsigned char)i;
        //diag_printf("storage_probe: get ustor_id: %d\n", i);
    }
    else
        goto BadDevice;

        Mass_stor_us[i]->host_id = ((struct usb_hcd *)pudev->bus->hcpriv)->host_id;


	for(i = 0; i<=us->max_lun; i++)
		us->Mass_stor_device[i].bDeviceReady = FALSE;
	//Mass_stor_us->bFileSystemInit = FALSE;

	kdbg("WARNING: USB Mass Storage data integrity not assured%s\n","");

	US_DEBUGP("USB Mass Storage device found at %d\n", us->pusb_dev->devnum);

	return 0;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
BadDevice:
	US_DEBUGP("storage_probe() failed%s","\n");
diag_printf("storage_probe() failed%s","\n");    
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	dissociate_dev(us);

	usb_stor_release_resources(us);

	return result;

}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void storage_disconnect(struct usb_interface *intf)
{
	struct us_data *us = (struct us_data*) usb_get_intfdata(intf);
//	struct scsi_device *sdev;

	diag_printf("storage_disconnect() : ustor_id: %d \n", us->ustor_id);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*
	scsi_lock(us->host);
	list_for_each_entry(sdev, &us->host->my_devices, siblings)
		sdev->online = 0;
	scsi_unlock(us->host);
*/

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	set_bit(US_FLIDX_DISCONNECTING, &us->flags, unsigned long);
	usb_stor_stop_transport(us);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	dissociate_dev(us);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*
	if (scsi_remove_host(us->host))
	{
		US_DEBUGP("-- SCSI refused to remove the host\n");
		BUG();
		return;
	}
*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_stor_release_resources(us);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

int  usb_stor_init(void)
{
	printk(KERN_INFO "Initializing USB Mass Storage driver...\n");

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (usb_register(&usb_storage_driver) < 0)
		return -1;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	printk(KERN_INFO "USB Mass Storage support registered.\n");
	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void  usb_stor_exit(void)
{

	US_DEBUGP("usb_stor_exit() called%s","\n");

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("-- calling usb_deregister()%s","\n");
	usb_deregister(&usb_storage_driver) ;

#if 0
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	for (next = us_list; next; next = next->next) {
		US_DEBUGP("-- calling scsi_unregister_host()\n");
		scsi_unregister_host(&usb_stor_host_template);
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	while (us_list) {
		/* keep track of where the next one is */
		next = us_list->next;

		/* If there's extra data in the us_data structure then
		 * free that first */
		if (us_list->extra) {
			/* call the destructor routine, if it exists */
			if (us_list->extra_destructor) {
				US_DEBUGP("-- calling extra_destructor()\n");
				us_list->extra_destructor(us_list->extra);
			}

			/* destroy the extra data */
			US_DEBUGP("-- freeing the data structure\n");
			kfree(us_list->extra);
		}

		/* free the structure itself */
		kfree (us_list);

		/* advance the list pointer */
		us_list = next;
	}
#endif
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 /*
int Test_and_Clear_Bit(int nr, void *addr)
{
	U32 mask, retval;
	U32 *adr = (U32 *)addr;

	adr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	retval = (mask & *adr) != 0;
	*adr &= ~mask;
	return retval;
}

 int Test_Bit(int nr, long * addr)
{
	int	mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}
*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#endif	//#ifdef

