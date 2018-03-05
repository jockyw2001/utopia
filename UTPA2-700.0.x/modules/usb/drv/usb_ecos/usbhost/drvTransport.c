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
#include <cyg/hal/hal_if.h>

#include "include/drvKernel.h"
#include  "include/drvConfig.h"
#include "drvTransport.h"
#include "drvProtocol.h"
#include "drvMassStor.h"
#include "drvUsbHostConfig.h"

#include  "include/drvErrno.h"
#include  "include/drvTimer.h"
#include  "include/drvBitops.h"
#include "drvHCD.h"
#include "drvEHCI.h"
#include "include/drvIO.h"

//#define US_DEBUG
#ifdef US_DEBUG
#define US_DEBUGP diag_printf
#else
#define US_DEBUGP(x,...)
#endif


extern S32      g_USBWaitFlg;
extern S32      g_USBWaitP2Flg;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void usb_stor_blocking_completion(struct urb *urb, struct pt_regs *regs)
{
	BOOL    err;
	struct completion *urb_done_ptr = (struct completion *)urb->context;

	if (urb->SetDMABuf != NULL)
	{
		//B_MemoryFree(urb->SetDMABuf);
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
                  diag_printf("usb_stor_blocking_completion: copy length: 0x%lx \n", urb->transfer_buffer_length); 
            #endif

			memcpy(urb->transfer_buffer, urb->TxDMABuf, urb->transfer_buffer_length);
        }
		//MsOS_FreeMemory(urb->TxDMABuf,gs32NonCachedPoolID_MIU0 );
		Usb_FreeNonCachedMemory(urb->TxDMABuf);
		urb->TxDMABuf = NULL;
	}

	complete(urb_done_ptr);
    
      err = MsOS_SetEvent(urb->WaitFlag_storage, USB_STOR_BLK_COMPLETE);
  //printk("%X\n",((Scsi_Cmnd *)(*((u32 *)((u32)urb - 0x80))))->cmnd[0]);
}

 static void usb_stor_async_completion(struct urb *urb, struct pt_regs *regs)
{
	BOOL err;
	struct completion *urb_done_ptr = (struct completion *)urb->context;

	//diag_printf("async_completion\n");
	if (urb->SetDMABuf != NULL)
	{
		//MsOS_FreeMemory(urb->SetDMABuf,gs32NonCachedPoolID_MIU0 );
		Usb_FreeNonCachedMemory(urb->SetDMABuf);
		urb->SetDMABuf = NULL;
	}

	if (urb->TxDMABuf != NULL)
	{
		if (usb_pipein(urb->pipe))
			memcpy(urb->transfer_buffer, urb->TxDMABuf, urb->transfer_buffer_length);
		//MsOS_FreeMemory(urb->TxDMABuf,gs32NonCachedPoolID_MIU0 );
		Usb_FreeNonCachedMemory(urb->TxDMABuf);
		urb->TxDMABuf = NULL;
	}

	complete(urb_done_ptr);
      err = MsOS_SetEvent(g_USBWaitFlg, USB_STOR_BLK_ASYNC);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//extern void USB_HW_Reset(struct usb_hcd* pUsbHcd, struct usb_device *pUsbDev);
static void timeout_handler(U32 us_)
{
    //struct us_data *us = (struct us_data *) us_;

    struct timer_list *pTimer = (struct timer_list *) us_;
    struct us_data *us = (struct us_data *) pTimer->data_ex;

    struct usb_hcd    *hcd = (struct usb_hcd*) us->current_urb->dev->bus->hcpriv;
    struct ehci_hcd    *ehci = hcd_to_ehci (hcd);

    // GGYY
    //if (test_and_clear_bit(US_FLIDX_URB_ACTIVE, &us->flags))
    {
        if ( pTimer->tmout_step > 0 )
            pTimer->tmout_value -= pTimer->tmout_step;
        else
            pTimer->tmout_value = 0;

        if ( ( (ehci_readl ((U32)&ehci->regs->port_status[0])&PORT_CONNECT) != 0 ) &&
                (pTimer->tmout_value > 0) )
        {
            mod_timer(pTimer, pTimer->tmout_value, pTimer->tmout_step);
        }
        else
        {
            diag_printf("Timeout -- cancelling URB%s","\n");
            US_DEBUGP("Timeout -- cancelling URB %p\n",us->current_urb);
            US_DEBUGP("msg: dev%d ep%d%s qtd --> status %d, len=%d",
            usb_pipedevice (us->current_urb->pipe),
            usb_pipeendpoint (us->current_urb->pipe),
            usb_pipein (us->current_urb->pipe) ? "in" : "out", us->current_urb->status, us->current_urb->transfer_buffer_length);
            // not do USB_HW_Reset here, upper layer will handle this
            //diag_printf("USB_HW_Reset\n");
            //USB_HW_Reset(hcd, us->current_urb->dev);
            diag_printf("try to usb_unlink_urb\n");
            usb_unlink_urb(us->current_urb);
            diag_printf("unlink urb done\n");
        }
    }
}

#ifndef URB_TIMEOUT_BY_WAIT_EVENT
void usb_Stor_wait_for_completion(struct completion *x, S32 waitFlag)
{
	BOOL	err;
	U32 RtnPattern;
	//U32 idx;
	//diag_printf("wait complete->");
#if 0
	idx=0;
	while (1)
      {
          idx++;
	 if (idx>6)
	    {
	    	diag_printf("wait timeout, game_status:%x \n",game_status);
			diag_printf("qtd:%x  buf:%x qtd_status:%x\n",my_qtd,my_buf,qtd_status);
			while (1){}
	    }
	  err=   MsOS_WaitEvent(g_USBWaitFlg, USB_STOR_BLK_COMPLETE, &RtnPattern, E_AND_CLEAR, 10000);
         if (err==TRUE) break;
       //  diag_printf("0xbf820030:%x\n",USBRead_REG32(0xbf820030));
          if ((USBRead_REG32(0xbf820030)&0x1)==0) break;     //device is removed
          if (USBRead_REG32(0xbf820030)&0x100) break;      //port is reset
       }
	//diag_printf("ok\n");
#endif
#if 1
	  err=   MsOS_WaitEvent(waitFlag, USB_STOR_BLK_COMPLETE, &RtnPattern, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#endif
	if (x->done != 1)
	{
		diag_printf("Wait for completion fail %08lX %ld\n", RtnPattern, x->done);
		//breakpoint();
		//while(1){}
	}
}

#else
void usb_Stor_wait_for_completion(struct completion *x, S32 waitFlag, struct us_data *us, U32 u32WaitMs)
{
	BOOL	err;
	U32 RtnPattern;
    
        err=   MsOS_WaitEvent(waitFlag, USB_STOR_BLK_COMPLETE, &RtnPattern, E_OR_CLEAR, u32WaitMs);
        if (!err) // time out
        {
            diag_printf("<usb_Stor_wait_for_completion> Timeout -- cancelling URB%s","\n");
            usb_unlink_urb(us->current_urb);
            diag_printf("<usb_Stor_wait_for_completion> unlink urb done\n");
        }
	if (x->done != 1)
	{
		diag_printf("Wait for completion fail %08lX %ld\n", RtnPattern, x->done);
	}
}
#endif

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int usb_stor_msg_common(struct us_data *us, int timeout)
{
	struct completion urb_done;
#ifndef URB_TIMEOUT_BY_WAIT_EVENT    
	struct timer_list to_timer;
#endif
	int status;
    S32 WaitFlag;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->flags & DONT_SUBMIT)
		return -EIO;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	init_completion(&urb_done);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->current_urb->context = &urb_done;
	us->current_urb->actual_length = 0;
	us->current_urb->error_count = 0;
	us->current_urb->status = 0;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->current_urb->transfer_flags =
			URB_ASYNC_UNLINK | URB_NO_SETUP_DMA_MAP;
	if (us->current_urb->transfer_buffer == us->iobuf)
		us->current_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	us->current_urb->transfer_dma = us->iobuf_dma;
	us->current_urb->setup_dma = us->cr_dma;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

    //Add patch by Jonas
    {
    struct usb_hcd *hcd = us->pusb_dev->bus->hcpriv;
    WaitFlag = hcd->USBWaitFlg;
    }
    
    us->current_urb->WaitFlag_storage = WaitFlag;
    
	status = usb_submit_urb(us->current_urb, GFP_NOIO);
	if (status) {
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		return status;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	set_bit(US_FLIDX_URB_ACTIVE, &us->flags, U32);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->flags & DONT_SUBMIT) {

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (test_and_clear_bit(US_FLIDX_URB_ACTIVE, &us->flags)) {
			US_DEBUGP("-- cancelling URB%s","\n");
			usb_unlink_urb(us->current_urb);
		}
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
	if (timeout > 0) {
		init_timer(&to_timer);
		//to_timer.expires =  timeout;
		to_timer.tmout_value = timeout;
		to_timer.tmout_step = 2000;
		to_timer.function = timeout_handler;
		//to_timer.data = (U32) us;
		to_timer.data = (U32) &to_timer;
		to_timer.data_ex = (U32) us;
		add_timer(&to_timer);
	}
#endif

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
	usb_Stor_wait_for_completion(&urb_done, WaitFlag);
#else
	usb_Stor_wait_for_completion(&urb_done, WaitFlag, us, timeout);
#endif
	clear_bit(US_FLIDX_URB_ACTIVE, &us->flags, U32);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
	if (timeout > 0)
		del_timer_sync(&to_timer);
#endif    

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
       if (urb_done.done!=1)
        {
        diag_printf("<usb_stor_msg_common> timeout err [[[ENODEV\n");
        return -ENODEV;                
        }
	return us->current_urb->status;
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
int usb_stor_control_msg(struct us_data *us, U32 pipe,
		 unsigned char request, unsigned char requesttype, U16 value, U16 index,
		 void *data, U16 size, int timeout)
{
	int status;

	US_DEBUGP("%s: rq=%02x rqtype=%02x value=%04x index=%02x len=%u\n",
			__FUNCTION__, request, requesttype,
			value, index, size);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->cr->bRequestType = requesttype;
	us->cr->bRequest = request;
	us->cr->wValue = CPUToLE16(value);
	us->cr->wIndex = CPUToLE16(index);
	us->cr->wLength = CPUToLE16(size);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_fill_control_urb(us->current_urb, us->pusb_dev, pipe,
			 (unsigned char*) us->cr, data, size,
			 usb_stor_blocking_completion, NULL);
	status = usb_stor_msg_common(us, timeout);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (status == 0)
		status = us->current_urb->actual_length;
	return status;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_clear_halt(struct us_data *us, U32 pipe)
{
	int result;
	int endp = usb_pipeendpoint(pipe);

	if (usb_pipein (pipe))
		endp |= USB_DIR_IN;

	result = usb_stor_control_msg(us, us->send_ctrl_pipe,
		USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0,
		endp, NULL, 0, 3*HZ);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_endpoint_running(us->pusb_dev, usb_pipeendpoint(pipe),
		usb_pipeout(pipe));
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/	
	usb_settoggle(us->pusb_dev, usb_pipeendpoint(pipe),
		usb_pipeout(pipe), 0);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("%s: result = %d\n", __FUNCTION__, result);
	return result;
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
static int interpret_urb_result(struct us_data *us, U32 pipe,
		U32 length, int result, U32 partial)
{
	US_DEBUGP("Status code %d; transferred %u/%u\n",
			result, partial, length);
	switch (result) {

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case 0:
		if (partial != length) {
			US_DEBUGP("-- short transfer%s","\n");
			return USB_STOR_XFER_SHORT;
		}

		US_DEBUGP("-- transfer complete%s","\n");
		return USB_STOR_XFER_GOOD;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -EPIPE:
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (usb_pipecontrol(pipe)) {
			US_DEBUGP("-- stall on control pipe%s","\n");
			return USB_STOR_XFER_STALLED;
		}

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		US_DEBUGP("clearing endpoint halt for pipe 0x%x\n", pipe);
		if (usb_stor_clear_halt(us, pipe) < 0)
			return USB_STOR_XFER_ERROR;
		return USB_STOR_XFER_STALLED;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -ETIMEDOUT:
		US_DEBUGP("-- timeout or NAK%s","\n");
		return USB_STOR_XFER_ERROR;
       case -ENODEV:
              diag_printf("<interpret_urb_result> no dev\n");
              return USB_STOR_XFER_NO_DEV;
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -EOVERFLOW:
		US_DEBUGP("-- babble%s","\n");
		return USB_STOR_XFER_LONG;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -ECONNRESET:
		US_DEBUGP("-- transfer cancelled%s","\n");
		return USB_STOR_XFER_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -EREMOTEIO:
		US_DEBUGP("-- short read transfer%s","\n");
		return USB_STOR_XFER_SHORT;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	case -EIO:
		US_DEBUGP("-- abort or disconnect in progress%s","\n");
		return USB_STOR_XFER_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	default:
		US_DEBUGP("-- unknown error%s","\n");
		return USB_STOR_XFER_ERROR;
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_ctrl_transfer(struct us_data *us, U32 pipe,
		unsigned char request, unsigned char requesttype, U16 value, U16 index,
		void *data, U16 size)
{
	int result;

	US_DEBUGP("%s: rq=%02x rqtype=%02x value=%04x index=%02x len=%u\n",
			__FUNCTION__, request, requesttype,
			value, index, size);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	us->cr->bRequestType = requesttype;
	us->cr->bRequest = request;
	us->cr->wValue = CPUToLE16(value);
	us->cr->wIndex = CPUToLE16(index);
	us->cr->wLength = CPUToLE16(size);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_fill_control_urb(us->current_urb, us->pusb_dev, pipe,
			 (unsigned char*) us->cr, data, size,
			 usb_stor_blocking_completion, NULL);
	result = usb_stor_msg_common(us, 10000);

	return interpret_urb_result(us, pipe, size, result,
			us->current_urb->actual_length);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_intr_transfer(struct us_data *us, void *buf, U32 length)
{
	int result;
	U32 pipe = us->recv_intr_pipe;
	U32 maxp;

	US_DEBUGP("%s: xfer %u bytes\n", __FUNCTION__, length);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	maxp = usb_maxpacket(us->pusb_dev, pipe, usb_pipeout(pipe));
	if (maxp > length)
		maxp = length;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#if 1
	usb_fill_bulk_urb(us->current_urb, us->pusb_dev, pipe, buf,
			length, usb_stor_blocking_completion, NULL);
#else
	usb_fill_int_urb(us->current_urb, us->pusb_dev, pipe, buf,
			maxp, usb_stor_blocking_completion, NULL,
			us->ep_bInterval);

#endif

	result = usb_stor_msg_common(us, 10000);

	return interpret_urb_result(us, pipe, length, result,
			us->current_urb->actual_length);
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

int usb_stor_bulk_transfer_buf(struct us_data *us, U32 pipe,
	void *buf, U32 length, U32 *act_len)
{
	int result;
	US_DEBUGP("%s: xfer %u bytes, buf=%p\n", __FUNCTION__, length, buf);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_fill_bulk_urb(us->current_urb, us->pusb_dev, pipe, buf, length,
		      usb_stor_blocking_completion, NULL);
	result = usb_stor_msg_common(us, 10000);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (act_len)
		*act_len = us->current_urb->actual_length;
	return interpret_urb_result(us, pipe, length, result,
			us->current_urb->actual_length);
}

void usb_stor_async_wait_completion(struct completion *x)
{
	U32 RtnPattern;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	MsOS_WaitEvent(g_USBWaitFlg, USB_STOR_BLK_ASYNC, &RtnPattern, E_AND_CLEAR, MSOS_WAIT_FOREVER);
}

int usb_stor_bulk_transfer_async_prepare(struct us_data *us, struct urb *purb, U32 pipe,
	void *buf, U32 length)
{
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

	usb_fill_bulk_urb(purb, us->pusb_dev, pipe, buf, length,
		      usb_stor_async_completion, &purb->urb_done);

	init_completion(&purb->urb_done);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	purb->actual_length = 0;
	purb->error_count = 0;
	purb->status = 0;

	purb->transfer_flags = URB_ASYNC_UNLINK | URB_NO_SETUP_DMA_MAP;
	if (purb->transfer_buffer == us->iobuf)
		purb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	return (usb_submit_urb(purb, ASYNC_MAKE_QTD));
}

int usb_stor_bulk_transfer_async_wait(struct us_data *us, struct urb *purb,
	U32 pipe, U32 length)
{
	int	status;
	//diag_printf("Bulk_Wait:\n");

	usb_stor_async_wait_completion(&purb->urb_done);
	status =  interpret_urb_result(us, pipe, length, purb->status,
			purb->actual_length);
	clear_bit(US_FLIDX_URB_ACTIVE, &us->flags, U32);

	del_timer_sync(&purb->to_timer);

	return status;
}

int usb_stor_bulk_transfer_async_send(struct us_data *us, struct urb *purb)
{
	int	status;

	//diag_printf("Bulk_Send:\n");

	status = usb_submit_urb(purb, ASYNC_SUBMIT_QTD);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	set_bit(US_FLIDX_URB_ACTIVE, &us->flags, U32);

	if (status == ENOERR)
	{
		init_timer(&purb->to_timer);
		purb->to_timer.tmout_value = 10000;
		purb->to_timer.tmout_step = 2000;
		purb->to_timer.function = timeout_handler;
		purb->to_timer.data = (U32) &purb->to_timer;
		purb->to_timer.data_ex = (U32) us;
		add_timer(&purb->to_timer);
	}

	return status;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 #if 0
int usb_stor_bulk_transfer_sglist(struct us_data *us, U32 pipe,
		struct scatterlist *sg, int num_sg, U32 length,
		U32 *act_len)
{
	int result;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->flags & DONT_SUBMIT)
		return USB_STOR_XFER_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("%s: xfer %u bytes, %d entries\n", __FUNCTION__,
			length, num_sg);
	result = usb_sg_init(&us->current_sg, us->pusb_dev, pipe, 0,
			sg, num_sg, length, SLAB_NOIO);
	if (result) {
		US_DEBUGP("usb_sg_init returned %d\n", result);
		return USB_STOR_XFER_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	set_bit(US_FLIDX_SG_ACTIVE, &us->flags);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->flags & DONT_SUBMIT) {

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (test_and_clear_bit(US_FLIDX_SG_ACTIVE, &us->flags)) {
			US_DEBUGP("-- cancelling sg request\n");
			usb_sg_cancel(&us->current_sg);
		}
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	usb_sg_wait(&us->current_sg);
	clear_bit(US_FLIDX_SG_ACTIVE, &us->flags);

	result = us->current_sg.status;
	if (act_len)
		*act_len = us->current_sg.bytes;
	return interpret_urb_result(us, pipe, length, result,
			us->current_sg.bytes);
}
#endif

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_bulk_transfer_sg(struct us_data* us, U32 pipe,
		void *buf, U32 length_left, int use_sg, int *residual)
{
	int result = -EIO;
	U32 partial;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (use_sg)
	{
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		US_DEBUGP("%s : not support scatter list.\n", __FUNCTION__);

	}
	else
	{
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		result = usb_stor_bulk_transfer_buf(us, pipe, buf,
				length_left, &partial);
		length_left -= partial;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (residual)
		*residual = length_left;
	return result;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_stor_invoke_transport(Scsi_Cmnd *srb, struct us_data *us)
{
	int need_auto_sense;
	int result;

	int temp_result;
	void* old_request_buffer;
	U16 old_sg;
	U32 old_request_bufflen;
	unsigned char old_sc_data_direction;
	unsigned char old_cmd_len;
	unsigned char old_cmnd[MAX_COMMAND_SIZE];
	U32 old_serial_number;


	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	srb->resid = 0;
	result = us->transport(srb, us);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->sm_state == US_STATE_ABORTING)
	{
		US_DEBUGP("-- command was aborted%s","\n");
		goto Handle_Abort;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_TRANSPORT_ERROR)
	{
            /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	
		US_DEBUGP("-- transport indicates error, resetting%s","\n");
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->result = DID_ERROR << 16;
		//return;
                goto Handle_Errors;
	}
      if (result ==USB_STOR_TRANSPORT_NO_DEVICE)
        {
        	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
          US_DEBUGP("[device removed]\n");
          /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
          srb->result = DID_ERROR << 16;
          /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
          return;
        }
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_TRANSPORT_NO_SENSE)
	{
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->result = SAM_STAT_CHECK_CONDITION;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		return;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	need_auto_sense = 0;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->protocol == US_PR_CB || us->protocol == US_PR_DPCM_USB)
	{
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		US_DEBUGP("-- CB transport device requiring auto-sense%s","\n");
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		need_auto_sense = 1;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (us->subclass == US_SC_UFI &&
		    ((srb->cmnd[0] == REQUEST_SENSE) ||
		     (srb->cmnd[0] == INQUIRY)))
		{
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			US_DEBUGP("** no auto-sense for a special command%s","\n");
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			need_auto_sense = 0;
		}
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_TRANSPORT_FAILED)
	{
		US_DEBUGP("-- transport indicates command failure%s","\n");
		need_auto_sense = 1;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if ((srb->resid > 0) &&
	    !((srb->cmnd[0] == REQUEST_SENSE) ||
	      (srb->cmnd[0] == INQUIRY) ||
	      (srb->cmnd[0] == MODE_SENSE) //||
//	      (srb->cmnd[0] == LOG_SENSE) ||			//(unsupport)
//	      (srb->cmnd[0] == MODE_SENSE_10)
	      ))
	{
		US_DEBUGP("-- unexpectedly short transfer%s","\n");
		need_auto_sense = 1;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (need_auto_sense)
	{
		US_DEBUGP("Issuing auto-REQUEST_SENSE%s","\n");

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		memcpy(old_cmnd, srb->cmnd, MAX_COMMAND_SIZE);
		old_cmd_len = srb->cmd_len;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
		srb->cmnd[0] = REQUEST_SENSE;
		srb->cmnd[1] = old_cmnd[1] & 0xE0;
		srb->cmnd[4] = 18;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
			srb->cmd_len = 12;
		else
			srb->cmd_len = 12;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		old_sc_data_direction = srb->sc_data_direction;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->sc_data_direction = SCSI_DATA_READ;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		old_request_buffer = srb->request_buffer;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->request_buffer = srb->sense_buffer;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		old_request_bufflen = srb->request_bufflen;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->request_bufflen = 18;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		old_sg = srb->use_sg;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->use_sg = 0;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		old_serial_number = srb->serial_number;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->serial_number ^= 0x80000000;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		temp_result = us->transport(&us->srb, us);

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->request_buffer = old_request_buffer;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->request_bufflen = old_request_bufflen;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->use_sg = old_sg;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->serial_number = old_serial_number;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->sc_data_direction = old_sc_data_direction;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->cmd_len = old_cmd_len;
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		memcpy(srb->cmnd, old_cmnd, MAX_COMMAND_SIZE);
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

		if (us->sm_state == US_STATE_ABORTING)
		{
			US_DEBUGP("-- auto-sense aborted%s","\n");
			goto Handle_Abort;
		}
		if (temp_result != USB_STOR_TRANSPORT_GOOD)
		{
			US_DEBUGP("-- auto-sense failure%s","\n");

			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
                        srb->result = DID_ERROR << 16;
			if (!(us->flags & US_FL_SCM_MULT_TARG))
			{
				/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
				goto Handle_Errors;
			}
			return;
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
		US_DEBUGP("-- Result from auto-sense is %d\n", temp_result);
		US_DEBUGP("-- code: 0x%x, key: 0x%x, ASC: 0x%x, ASCQ: 0x%x\n",
			  srb->sense_buffer[0],
			  srb->sense_buffer[2] & 0xf,
			  srb->sense_buffer[12],
			  srb->sense_buffer[13]);
#ifdef CONFIG_USB_STORAGE_DEBUG
		usb_stor_show_sense(
			  srb->sense_buffer[2] & 0xf,
			  srb->sense_buffer[12],
			  srb->sense_buffer[13]);
#endif
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		srb->result = SAM_STAT_CHECK_CONDITION;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if ((srb->sense_buffer[2] & 0xf) == 0x0)
			srb->result = SAM_STAT_GOOD;
	}
	else 
		srb->result = SAM_STAT_GOOD;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_TRANSPORT_FAILED)
		srb->result = SAM_STAT_CHECK_CONDITION;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if ((us->protocol == US_PR_CB || us->protocol == US_PR_DPCM_USB) &&
	    (result == USB_STOR_TRANSPORT_GOOD) &&
	    ((srb->sense_buffer[2] & 0xf) == 0x0))
		srb->sense_buffer[0] = 0x0;
	return;

Handle_Errors:
        //us->transport_reset(us);
        usb_reset_device(us->pusb_dev); 
        return;
    
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
Handle_Abort:
	srb->result = DID_ABORT << 16;
	if (us->protocol == US_PR_BULK)
	{
		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		clear_bit(US_FLIDX_ABORTING, &us->flags, U32);
		//us->transport_reset(us);
		usb_reset_device(us->pusb_dev);	//Note_Timeout
	}
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
void usb_stor_stop_transport(struct us_data *us)
{
	US_DEBUGP("%s called\n", __FUNCTION__);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (test_and_clear_bit(US_FLIDX_URB_ACTIVE, &us->flags)) {
		US_DEBUGP("-- cancelling URB%s","\n");
		usb_unlink_urb(us->current_urb);
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

int usb_stor_CBI_transport(Scsi_Cmnd *srb, struct us_data *us)
{
	U32 transfer_length = srb->request_bufflen;
	int result;
	U32 pipe;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = usb_stor_ctrl_transfer(us, us->send_ctrl_pipe,
				      US_CBI_ADSC,
				      USB_TYPE_CLASS | USB_RECIP_INTERFACE, 0,
				      us->ifnum, srb->cmnd, srb->cmd_len);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Call to usb_stor_ctrl_transfer() returned %d\n", result);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_XFER_STALLED)
	{
		return USB_STOR_TRANSPORT_FAILED;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result != USB_STOR_XFER_GOOD)
	{
		return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (transfer_length)
	{
		pipe = srb->sc_data_direction == SCSI_DATA_READ ?
				us->recv_bulk_pipe : us->send_bulk_pipe;
		result = usb_stor_bulk_transfer_sg(us, pipe,
					srb->request_buffer, transfer_length,
					srb->use_sg, &srb->resid);
		US_DEBUGP("CBI data stage result is 0x%x\n", result);
		if (result > USB_STOR_XFER_STALLED)
			return USB_STOR_TRANSPORT_ERROR;
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
	result = usb_stor_intr_transfer(us, us->iobuf, 2);
	US_DEBUGP("Got interrupt data (0x%x, 0x%x)\n",
			us->iobuf[0], us->iobuf[1]);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->subclass == US_SC_UFI)
	{
		if (srb->cmnd[0] == REQUEST_SENSE ||
		    srb->cmnd[0] == INQUIRY)
			return USB_STOR_TRANSPORT_GOOD;
		else
		{
			if (us->iobuf[0])
				return USB_STOR_TRANSPORT_FAILED;
			else
				return USB_STOR_TRANSPORT_GOOD;
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
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (us->iobuf[0])
	{
		US_DEBUGP("CBI IRQ data showed reserved bType %d\n",
				us->iobuf[0]);
		return USB_STOR_TRANSPORT_ERROR;
	}

	switch (us->iobuf[1] & 0x0F)
	{
		case 0x00:
			return USB_STOR_TRANSPORT_GOOD;
		case 0x01:
			return USB_STOR_TRANSPORT_FAILED;
		default:
			return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	return USB_STOR_TRANSPORT_ERROR;
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
int usb_stor_CB_transport(Scsi_Cmnd *srb, struct us_data *us)
{
	U32 transfer_length = srb->request_bufflen;
	int result;
	U32 pipe;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = usb_stor_ctrl_transfer(us, us->send_ctrl_pipe,
				      US_CBI_ADSC,
				      USB_TYPE_CLASS | USB_RECIP_INTERFACE, 0,
				      us->ifnum, srb->cmnd, srb->cmd_len);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Call to usb_stor_ctrl_transfer() returned %d\n", result);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_XFER_STALLED)
	{
		return USB_STOR_TRANSPORT_FAILED;
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
	if (result != USB_STOR_XFER_GOOD)
	{
		return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (transfer_length)
	{
		pipe = srb->sc_data_direction == SCSI_DATA_READ ?
				us->recv_bulk_pipe : us->send_bulk_pipe;
		result = usb_stor_bulk_transfer_sg(us, pipe,
					srb->request_buffer, transfer_length,
					srb->use_sg, &srb->resid);
		US_DEBUGP("CB data stage result is 0x%x\n", result);
		if (result > USB_STOR_XFER_STALLED)
			return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	return USB_STOR_TRANSPORT_GOOD;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_Bulk_max_lun(struct us_data *us)
{
	int result;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	result = usb_stor_control_msg(us, us->recv_ctrl_pipe,
				 US_BULK_GET_MAX_LUN,
				 USB_DIR_IN | USB_TYPE_CLASS |
				 USB_RECIP_INTERFACE,
				 0, us->ifnum, us->iobuf, 1, HZ);

	US_DEBUGP("GetMaxLUN command result is %d, data is %d\n",
		  result, us->iobuf[0]);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == 1)
		return us->iobuf[0];

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
unsigned char usb_stor_sense_invalidCDB[18] =
{ 0x70, 0, ILLEGAL_REQUEST,0,0,0,0,0x0a,0, 0, 0, 0, 0x24, 0, 0, 0, 0, 0};
#if 0
//	[0]	= 0x70,			    /* current error */
//	[2]	= ILLEGAL_REQUEST,	    /* Illegal Request = 0x05 */
//	[7]	= 0x0a,			    /* additional length */
//	[12]	= 0x24			    /* Invalid Field in CDB */
//};
#endif

int usb_stor_Bulk_transport(Scsi_Cmnd *srb, struct us_data *us)
{
    unsigned char *tmp_iobuf=0;
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *)(us->iobuf - tmp_iobuf);
	struct bulk_cs_wrap *bcs = (struct bulk_cs_wrap *)(us->iobuf - tmp_iobuf);
	U32 transfer_length = srb->request_bufflen;
	int result;
	int fake_sense = 0;
	U32 pipe;
	//int CSW_relt; //GGYY

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	bcb->Signature = CPUToLE32(US_BULK_CB_SIGN);
	bcb->DataTransferLength = CPUToLE32(transfer_length);
	bcb->Flags = srb->sc_data_direction == SCSI_DATA_READ ? 1 << 7 : 0;
	bcb->Tag = srb->serial_number;
	bcb->Lun = srb->lun;			//YPING02??
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	bcb->Length = srb->cmd_len;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	memset(bcb->CDB, 0, sizeof(bcb->CDB));
	memcpy(bcb->CDB, srb->cmnd, bcb->Length);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Bulk command S 0x%x T 0x%x Trg %d LUN %d L %d F %d CL %d\n",
			LE32ToCPU(bcb->Signature), bcb->Tag,
			(bcb->Lun >> 4), (bcb->Lun & 0x0F),
			LE32ToCPU(bcb->DataTransferLength),
			bcb->Flags, bcb->Length);

#if 0	//GGYY, disable speed-up procedure for compatibility
	result = usb_stor_bulk_transfer_async_prepare(us, us->current_urb, us->send_bulk_pipe,
				bcb, US_BULK_CB_WRAP_LEN);
	//diag_printf("Bulk cmd prepare result=%d\n", result);
	if (result != ENOERR)
		return USB_STOR_TRANSPORT_ERROR;

	result = usb_stor_bulk_transfer_async_send(us, us->current_urb);
	//diag_printf("Bulk cmd send result=%d\n", result);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	pipe = srb->sc_data_direction == SCSI_DATA_READ ?
			us->recv_bulk_pipe : us->send_bulk_pipe;
	if (transfer_length)
	{
		result = usb_stor_bulk_transfer_async_prepare(us, us->data_urb, pipe,
					srb->request_buffer, transfer_length);
		//diag_printf("Bulk data prepare result 0x%x\n", result);
		if (result == USB_STOR_XFER_ERROR)
			return USB_STOR_TRANSPORT_ERROR;

		result = usb_stor_bulk_transfer_async_wait(us, us->current_urb,
					us->send_bulk_pipe, US_BULK_CB_WRAP_LEN);
		//diag_printf("Bulk cmd wait result 0x%x\n", result);
		if (result != USB_STOR_XFER_GOOD)
			return USB_STOR_TRANSPORT_ERROR;

		result = usb_stor_bulk_transfer_async_send(us, us->data_urb);
		//diag_printf("Bulk data send result 0x%x\n", result);
		if (result != USB_STOR_XFER_GOOD)
			return USB_STOR_TRANSPORT_ERROR;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (result == USB_STOR_XFER_LONG)
			fake_sense = 1;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#if 0
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Attempting to get CSW...%s","\n");
	CSW_relt = usb_stor_bulk_transfer_async_prepare(us, us->status_urb, us->recv_bulk_pipe,
				bcs, US_BULK_CS_WRAP_LEN);
	//diag_printf("Bulk status prepare result 0x%x\n", CSW_relt);
	if (CSW_relt == USB_STOR_XFER_ERROR)
		return USB_STOR_TRANSPORT_ERROR;
#endif

	if (transfer_length)
	{
		result = usb_stor_bulk_transfer_async_wait(us, us->data_urb, pipe, transfer_length);
		//diag_printf("Bulk data wait result 0x%x\n", result);
		if (result == USB_STOR_XFER_ERROR)
			return USB_STOR_TRANSPORT_ERROR;
	}
	else
	{
		result = usb_stor_bulk_transfer_async_wait(us, us->current_urb,
					us->send_bulk_pipe, US_BULK_CB_WRAP_LEN);
	//	diag_printf("Bulk cmd wait result 0x%x\n", result);
		if (result != USB_STOR_XFER_GOOD)
			return USB_STOR_TRANSPORT_ERROR;
	}

#if 0
	if (CSW_relt != USB_STOR_XFER_GOOD)
	{
		result = usb_stor_bulk_transfer_async_prepare(us, us->status_urb, us->recv_bulk_pipe,
					bcs, US_BULK_CS_WRAP_LEN);
	//	diag_printf("2nd time Bulk status prepare result 0x%x\n", result);
		if (result == USB_STOR_XFER_ERROR)
			return USB_STOR_TRANSPORT_ERROR;
	}
	result = usb_stor_bulk_transfer_async_send(us, us->status_urb);
//	diag_printf("Bulk status send result 0x%x\n", result);
	if (result == USB_STOR_XFER_ERROR)
		return USB_STOR_TRANSPORT_ERROR;
//	if ( (result != USB_STOR_XFER_GOOD) && (result != USB_STOR_XFER_STALLED) )
//		return USB_STOR_TRANSPORT_ERROR;

	//diag_printf("Before Bulk status wait result 0x%x\n", result);

	result = usb_stor_bulk_transfer_async_wait(us, us->status_urb, us->recv_bulk_pipe,
			US_BULK_CS_WRAP_LEN);
	//diag_printf("Bulk status wait result 0x%x\n", result);
#endif
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Attempting to get CSW...%s","\n");
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
				bcs, US_BULK_CS_WRAP_LEN, NULL);

#else
	result = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe,
				bcb, US_BULK_CB_WRAP_LEN, NULL);
	US_DEBUGP("Bulk command transfer result=%d\n", result);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (transfer_length)
	{
		pipe = srb->sc_data_direction == SCSI_DATA_READ ?
				us->recv_bulk_pipe : us->send_bulk_pipe;
		result = usb_stor_bulk_transfer_sg(us, pipe,
					srb->request_buffer, transfer_length,
					srb->use_sg, &srb->resid);
		US_DEBUGP("Bulk data transfer result 0x%x\n", result);
		if (result == USB_STOR_XFER_ERROR)
			return USB_STOR_TRANSPORT_ERROR;

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		if (result == USB_STOR_XFER_LONG)
			fake_sense = 1;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Attempting to get CSW...%s","\n");
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
				bcs, US_BULK_CS_WRAP_LEN, NULL);
#endif
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result == USB_STOR_XFER_STALLED) {

		/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
		US_DEBUGP("Attempting to get CSW (2nd try)...%s","\n");
		result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
				bcs, US_BULK_CS_WRAP_LEN, NULL);
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Bulk status result = %d\n", result);
       if (result==USB_STOR_XFER_NO_DEV)
             return USB_STOR_TRANSPORT_NO_DEVICE;
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	US_DEBUGP("Bulk status Sig 0x%x T 0x%x R %d Stat 0x%x\n",
			LE32ToCPU(bcs->Signature), bcs->Tag,
			bcs->Residue, bcs->Status);
	if ((bcs->Signature != CPUToLE32(US_BULK_CS_SIGN) &&
		    bcs->Signature != CPUToLE32(US_BULK_CS_OLYMPUS_SIGN)) ||
			bcs->Tag != srb->serial_number ||
			bcs->Status > US_BULK_STAT_PHASE)
	{
		US_DEBUGP("Bulk logical error%s","\n");
		return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	switch (bcs->Status)
	{
		case US_BULK_STAT_OK:
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			if (fake_sense)
			{
				memcpy(srb->sense_buffer,
				       usb_stor_sense_invalidCDB,
				       sizeof(usb_stor_sense_invalidCDB));
				return USB_STOR_TRANSPORT_NO_SENSE;
			}

			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			return USB_STOR_TRANSPORT_GOOD;

		case US_BULK_STAT_FAIL:
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			return USB_STOR_TRANSPORT_FAILED;

		case US_BULK_STAT_PHASE:
			/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
			return USB_STOR_TRANSPORT_ERROR;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	return USB_STOR_TRANSPORT_ERROR;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int usb_stor_reset_common(struct us_data *us,
		unsigned char request, unsigned char requesttype,
		U16 value, U16 index, void *data, U16 size)
{
	int result;
	int result2;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

	result = usb_stor_control_msg(us, us->send_ctrl_pipe,
			request, requesttype, value, index, data, size,
			20*HZ);
	if (result < 0)
	{
		US_DEBUGP("Soft reset failed: %d\n", result);
		return FAILED;
	}

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	up(&us->dev_semaphore);
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout(HZ*6);
	set_current_state(TASK_RUNNING);
	down(&us->dev_semaphore);

	if (test_bit(US_FLIDX_DISCONNECTING, &us->flags))
	{
		US_DEBUGP("Reset interrupted by disconnect%s","\n");
		return FAILED;
	}

	US_DEBUGP("Soft reset: clearing bulk-in endpoint halt%s","\n");
	result = usb_stor_clear_halt(us, us->recv_bulk_pipe);

	US_DEBUGP("Soft reset: clearing bulk-out endpoint halt%s","\n");
	result2 = usb_stor_clear_halt(us, us->send_bulk_pipe);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	if (result < 0 || result2 < 0)
	{
		US_DEBUGP("Soft reset failed%s","\n");
		return FAILED;
	}
	US_DEBUGP("Soft reset done%s","\n");
	return SUCCESS;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define CB_RESET_CMD_SIZE	12
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_CB_reset(struct us_data *us)
{
	US_DEBUGP("%s called\n", __FUNCTION__);

	memset(us->iobuf, 0xFF, CB_RESET_CMD_SIZE);
	us->iobuf[0] = SEND_DIAGNOSTIC;
	us->iobuf[1] = 4;
	return usb_stor_reset_common(us, US_CBI_ADSC,
				 USB_TYPE_CLASS | USB_RECIP_INTERFACE,
				 0, us->ifnum, us->iobuf, CB_RESET_CMD_SIZE);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
int usb_stor_Bulk_reset(struct us_data *us)
{
	US_DEBUGP("%s called\n", __FUNCTION__);

	return usb_stor_reset_common(us, US_BULK_RESET_REQUEST,
				 USB_TYPE_CLASS | USB_RECIP_INTERFACE,
				 0, us->ifnum, NULL, 0);
}

void usb_default_timeout_handler(void *pTm, void *pUrb)
{
	struct timer_list *pTimer = (struct timer_list *) pTm;
	struct urb		*current_urb = (struct urb*) pUrb;

	struct usb_hcd    *hcd = (struct usb_hcd*) current_urb->dev->bus->hcpriv;
	struct ehci_hcd    *ehci = hcd_to_ehci (hcd);

	if ( pTimer->tmout_step > 0 )
		pTimer->tmout_value -= pTimer->tmout_step;
	else
		pTimer->tmout_value = 0;

	if ( ( (ehci_readl ((U32)&ehci->regs->port_status[0])&PORT_CONNECT) != 0 ) &&
		(pTimer->tmout_value > 0) )
	{
		mod_timer(pTimer, pTimer->tmout_value, pTimer->tmout_step);
	}
	else
	{
		diag_printf("Timeout -- cancelling URB %p\n",current_urb);
		US_DEBUGP("msg: dev%d ep%d%s qtd --> status %d, len=%d",
		        usb_pipedevice (current_urb->pipe),
		        usb_pipeendpoint (current_urb->pipe),
		        usb_pipein (current_urb->pipe) ? "in" : "out", current_urb->status, current_urb->transfer_buffer_length);
		diag_printf("unlink urb now\n");
		usb_unlink_urb(current_urb);
	}
}

