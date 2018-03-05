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
//#define MSTAR_USB_DEBUG_MSG

//=============include file=================//
//#include <MsCommon.h> // NUSED
//#include <cyg/hal/hal_if.h> // NUSED

//#include "include/drvKernel.h" // NUSED
#include "include/drvConfig.h"
#include "drvBulktran.h"
//#include "drvMassStor.h" // NUSED
//#include "drvUsbHostConfig.h" // NUSED

#include "include/drvErrno.h"
//#include "include/drvTimer.h" // NUSED
#include "include/drvBitops.h"
//#include "drvUsbd.h" // NUSED
//#include "drvEHCI.h" // NUSED
#include "drvUSBHwCtl.h"
/* applying drvUsbHostConfig.h (inside drvUSBHwCtl.h) */

//========================================//
//=============Definition=================//
#define BLK_COMPLETE    1
#define PTP_COMPLETE    2
#define BLK_ASYNC       4
#define ILLEGAL_REQUEST         0x05

unsigned char invalid_CDB[18] =
{ 0x70, 0, ILLEGAL_REQUEST,0,0,0,0,0x0a,0, 0, 0, 0, 0x24, 0, 0, 0, 0, 0};
//========================================//


static void *ms_usb_find_data_location(stCmd_scsi *request_block)
{
    return (void *) request_block->req_buf;
}

static void ms_usb_invoke_and_inquiry(stCmd_scsi *request_block, struct ms_usdata *ms_us)
{
    unsigned char *data_ptr;

    ms_usb_invoke_transport(request_block, ms_us);
    if (request_block->result == MS_STAT_GOOD)
    {
        if (request_block->cmnd[0] != INQUIRY)
            return;

        if (request_block->req_buf_len < 3)
            return;

        data_ptr = (unsigned char*) ms_usb_find_data_location(request_block);

        if ((data_ptr[2] & 7) == 2)
            return;

        ms_usbhost_debug("Fixing INQUIRY data (SCSI rev--2) = %d\n", data_ptr[2] & 7);

        data_ptr[2] = (data_ptr[2] & ~7) | 2;
    }
}

#if SupportOtherDevice
void ms_usb_qic157_command(stCmd_scsi *request_block, struct ms_usdata *ms_us)
{
    for (; request_block->cmd_len<12; request_block->cmd_len++)
        request_block->cmnd[request_block->cmd_len] = 0;

    request_block->cmd_len = 12;
    ms_usb_invoke_and_inquiry(request_block, ms_us);
}

void ms_usb_ufi_command(stCmd_scsi *request_block, struct ms_usdata *ms_us)
{
    for (; request_block->cmd_len<12; request_block->cmd_len++)
        request_block->cmnd[request_block->cmd_len] = 0;

    request_block->cmd_len = 12;

    switch (request_block->cmnd[0])
    {
        case INQUIRY:
            request_block->cmnd[4] = 36;
            break;

        //case MODE_SENSE_10:
        case MODE_SENSE:
            request_block->cmnd[7] = 0;
            request_block->cmnd[8] = 8;
            break;

        case REQUEST_SENSE:
            request_block->cmnd[4] = 18;
            break;
    } /* end switch on cmnd[0] */

    ms_usb_invoke_and_inquiry(request_block, ms_us);
}
#endif

void ms_usb_transparent_scsi_command(stCmd_scsi *request_block, struct ms_usdata *ms_us)
{
    unsigned char *dp;
    U32 capacity;

    ms_usb_invoke_and_inquiry(request_block, ms_us);
    if (request_block->result == MS_STAT_GOOD)
    {
        if (ms_us->flags & MS_FIX_CAPACITY)
        {
            if (request_block->cmnd[0] != READ_CAPACITY)
                return;

            dp = (unsigned char*) ms_usb_find_data_location(request_block);

            capacity = (dp[0]<<24) + (dp[1]<<16) + (dp[2]<<8) + (dp[3]);
            ms_usbhost_msg("US: Fixing capacity (from %ld to %ld)\n", capacity+1, capacity);
            capacity--;
            dp[0] = (capacity >> 24);
            dp[1] = (capacity >> 16);
            dp[2] = (capacity >> 8);
            dp[3] = (capacity);
        }
    }
}

void ms_usb_set_dma_buf(struct urb *request_block)
{
    if (request_block->SetDMABuf != NULL)
    {
        Usb_FreeNonCachedMemory(request_block->SetDMABuf);
        request_block->SetDMABuf = NULL;
    }
}

void ms_usb_set_tx_dma_buf(struct urb *request_block)
{
#ifdef DEBUG_PERFORMANCE
    int max_packet_size=512;
#endif
    if (request_block->TxDMABuf != NULL)
    {
        if (usb_pipein(request_block->u32Pipe))
        {
#ifdef DEBUG_PERFORMANCE
            if (request_block->u32TransferBufferLength >= max_packet_size)
                ms_usbhost_msg("Blocking Completion (len: 0x%lx)\n", request_block->u32TransferBufferLength);
#endif
#ifdef MIPS_TOOLCHAIN_482_ISSUE
            MsOS_Dcache_Invalidate((U32) request_block->TxDMABuf, request_block->u32TransferBufferLength);
#endif
            memcpy(request_block->pTransferBuffer, request_block->TxDMABuf, request_block->u32TransferBufferLength);
        }
        Usb_FreeNonCachedMemory(request_block->TxDMABuf);
        request_block->TxDMABuf = NULL;
    }
}

static void ms_usb_blocking_completion(struct urb *request_block, struct stPtRegs *registers)
{
    struct stCompletion *request_block_done = (struct stCompletion *)request_block->pContext;
    //BOOL    err;

    /* move to ms_usb_hcd_giveback_urb() to support 3rd party driver */
    //ms_usb_set_dma_buf(request_block);
    //ms_usb_set_tx_dma_buf(request_block);
    complete(request_block_done);
    /*err = */MsOS_SetEvent(request_block->WaitFlag_storage, BLK_COMPLETE);
}

#ifndef URB_TIMEOUT_BY_WAIT_EVENT
static void ms_usb_timeout_handler(U32 tmp)
{
    struct timer_list *ptr_timer = (struct timer_list *) tmp;
    struct ms_usdata *ptr = (struct ms_usdata *) ptr_timer->data_ex;

    struct usb_hcd    *host_controller_device = (struct usb_hcd*) ptr->current_urb->dev->bus->hcpriv;

    if ( ptr_timer->tmout_step > 0 )
        ptr_timer->tmout_value -= ptr_timer->tmout_step;
    else
        ptr_timer->tmout_value = 0;

    if ( ms_RoothubPortConnected(host_controller_device) &&
            (ptr_timer->tmout_value > 0) )
        ms_update_timer(ptr_timer, ptr_timer->tmout_value, ptr_timer->tmout_step);
    else
    {
        int retv;
        /* remove (20150422) */
        //struct stCompletion *request_block_done = (struct stCompletion *)ptr->current_urb->pContext;

        ms_usbhost_err("Timeout handler .. Warning!! Cancel urb @ qh %p\n",ptr->current_urb->hcpriv);
        ms_usbhost_err("LOG: dev%d ep%d%s[%d] qtd --> status %d, len=%d\n",
            usb_pipedevice (ptr->current_urb->u32Pipe),
            usb_pipeendpoint (ptr->current_urb->u32Pipe),
            usb_pipein (ptr->current_urb->u32Pipe) ? "in" : "out",usb_pipetype (ptr->current_urb->u32Pipe), ptr->current_urb->s32Status, ptr->current_urb->u32TransferBufferLength);
        ms_usbhost_msg("unlink USB request block ongoing...\n");
        //enhanced_host_controller_interface->uDontSendIAA = 0; // force IAA command
        retv = ms_usb_unlink_urb(ptr->current_urb);
        ms_usbhost_err("unlink URB return(%d)\n", retv);

        ptr->current_urb->s32Status = -ETIMEDOUT;

        /* remove (20150422) */
        /*  to relieve wait event if necessary (20140702) */
        //if (request_block_done->done == 0);
        //    MsOS_SetEvent(ptr->current_urb->WaitFlag_storage, BLK_COMPLETE);
    }
}

static void ms_usb_wait_for_completion(struct stCompletion *cpt, S32 flag)
{
    BOOL    error;
    U32 u32Pattern;

    error = MsOS_WaitEvent(flag, BLK_COMPLETE, &u32Pattern, E_OR_CLEAR, MSOS_WAIT_FOREVER);

    if (cpt->done != 1)
        ms_usbhost_err("Completion fail %08lX %ld, error %d\n", u32Pattern, cpt->done, error);
}
#else
#define TIMEOUT_STEP 500 //ms
static void ms_usb_wait_for_completion_timeout(struct stCompletion *cpt, S32 flag, struct ms_usdata *ptr, U32 wait_ms)
{
    BOOL    error = 0;
    BOOL    disconn = 0;
    U32 u32Pattern;
    U32 wait_remain = wait_ms;
    struct urb *urb = ptr->current_urb;
    struct usb_hcd *hcd = urb->dev->bus->hcpriv;

    while(wait_remain > 0)
    {
        if((urb->dev->parent != NULL) && (urb->dev->parent->parent == NULL) // device on roothub
            && (ms_RoothubPortConnectChg(hcd) || !ms_RoothubPortConnected(hcd))) // CSC or disconnect happen on ehci port
        {
            error = 0;
            disconn = 1;
            diag_printf("[USB][BULK] stop wait CMP, wait_remain %d csc [%d] connect [%d]\n",
                wait_remain, ms_RoothubPortConnectChg(hcd), ms_RoothubPortConnected(hcd));
            break;
        }

        if(wait_remain >= TIMEOUT_STEP)
        {
             //diag_printf("[USB] wait_remain %d csc [%d] connect [%d]\n",
             //    wait_remain, ms_RoothubPortConnectChg(hcd), ms_RoothubPortConnected(hcd));
             error = MsOS_WaitEvent(flag, BLK_COMPLETE, &u32Pattern, E_OR_CLEAR, TIMEOUT_STEP);
             wait_remain -= TIMEOUT_STEP;
        }
        else
        {
             error = MsOS_WaitEvent(flag, BLK_COMPLETE, &u32Pattern, E_OR_CLEAR, wait_remain);
             wait_remain = 0;
        }

        if(error)
            break;
    }

    if (!error) // time out
    {
        int retv;

	diag_printf("[timeout] kill urb (qh:%p)!\n",
		ptr->current_urb->hcpriv);
        retv = ms_hcd_unlink_urb(ptr->current_urb, -ENOENT);
	//diag_printf("[timeout] unlink current urb (qh:%p)!\n",
	//	ptr->current_urb->hcpriv);
        //retv = ms_usb_unlink_urb(ptr->current_urb);
	/* if there is done bit completed, it is really done although timeout event */
	if (cpt->done == 1) {
		ptr->current_urb->s32Status = 0;
		return;
	}
        ms_usbhost_err("<%s> unlink USB request block done (%d)...\n", __FUNCTION__, retv);

        ptr->current_urb->s32Status = disconn ? -ENODEV : -ETIMEDOUT;
    }
    if (cpt->done != 1)
        ms_usbhost_err("Completion fail %08lX %ld, error %d\n", u32Pattern, cpt->done, error);
}
#endif

static int ms_usb_msg_common(struct ms_usdata *ptr, int time_out)
{
    struct stCompletion request_block_done;
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
    struct timer_list to_timer;
#endif
    int status;
    S32 flag;

    if (ptr->flags & NO_SUBMIT)
        return -EIO;

    init_completion(&request_block_done);

    /*Get urb status*/
    ptr->current_urb->pContext = &request_block_done;
    ptr->current_urb->u32ActualLength = 0;
    //ptr->current_urb->s32Status = 0; // dummy
    //ptr->current_urb->u32TransferFlags = MS_FLAG_URB_ASYNC_UNLINK | MS_FLAG_URB_NO_SETUP_DMA_MAP;
    ptr->current_urb->u32TransferFlags = MS_FLAG_URB_NO_SETUP_DMA_MAP;

    if (ptr->current_urb->pTransferBuffer == ptr->iobuf)
        ptr->current_urb->u32TransferFlags |= MS_FLAG_URB_NO_TRANSFER_DMA_MAP;
    ptr->current_urb->tTransferDma = ptr->iobuf_dma;
    ptr->current_urb->tSetupDma = ptr->cr_dma;

    //Add patch by Jonas
    {
    struct usb_hcd *hcd = ptr->pusb_dev->bus->hcpriv;
    flag = hcd->USBWaitFlg;
    }

    ptr->current_urb->WaitFlag_storage = flag;
    MsOS_ClearEvent(flag, BLK_COMPLETE); // 20160328 clear flag before waiting

    status = ms_usb_submit_urb(ptr->current_urb, GFP_NOIO);
    if (status)
        return status;

    ms_set_bit(MS_IDX_URB_ACTIVE, &ptr->flags, U32);
    if (ptr->flags & NO_SUBMIT)
    {
        if (ms_test_and_clear_bit(MS_IDX_URB_ACTIVE, &ptr->flags))
        {
            ms_usbhost_err("NO_SUBMIT --> Cancel USB request block\n");
            ms_usb_unlink_urb(ptr->current_urb);
        }
    }
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
    if (time_out > 0)
    {
        ms_init_timer(&to_timer);
        to_timer.tmout_value = time_out;
        to_timer.tmout_step = 2000;
        to_timer.function = ms_usb_timeout_handler;
        to_timer.data = (U32) &to_timer;
        to_timer.data_ex = (U32) ptr;
        ms_add_timer(&to_timer);
    }
    ms_usb_wait_for_completion(&request_block_done, flag);
#else
    ms_usb_wait_for_completion_timeout(&request_block_done, flag, ptr, ms_jiffies_to_msecs(time_out));
#endif
    ms_clear_bit(MS_IDX_URB_ACTIVE, &ptr->flags, U32);
#ifndef URB_TIMEOUT_BY_WAIT_EVENT
    if (time_out > 0)
        ms_del_timer_sync(&to_timer);
#endif

    /* ETIMEOUT process already done, else just return */
    //if (ptr->current_urb->s32Status < 0 && ptr->current_urb->s32Status != -ETIMEDOUT)
    //{
    //    struct usb_hcd    *hcd = (struct usb_hcd*) ptr->current_urb->dev->bus->hcpriv;
    //
    //    /* kill error return urb */
    //    ms_unlink1(hcd, ptr->current_urb);
    //    diag_printf("[UM] msg_common urb status %d\n", ptr->current_urb->s32Status);
    //}
    return ptr->current_urb->s32Status;
}

int ms_usb_control_msg(struct ms_usdata *ptr, U32 pipe,
            unsigned char req, unsigned char rt, U16 val, U16 idx, void *data, U16 len, int time_out)
{
    int state;

    ms_usbhost_debug("<ms_usb_control_msg>: req=%02x rt=%02x val=%04x idx=%02x len=%u\n",
            req, rt, val, idx, len);

    ptr->cr->bRequestType = rt;
    ptr->cr->bRequest = req;
    ptr->cr->wValue = val;
    ptr->cr->wIndex = idx;
    ptr->cr->wLength = len;

    ms_usb_stuff_control_urb(ptr->current_urb, ptr->pusb_dev, pipe,
            (unsigned char*) ptr->cr, data, len, ms_usb_blocking_completion, NULL);

    state = ms_usb_msg_common(ptr, time_out);
    if (!state)
        state = ptr->current_urb->u32ActualLength;

    return state;
}

int ms_usb_msc_clear_halt(struct ms_usdata *ptr, U32 pipe)
{
    int regval,ep;

    ep = usb_pipeendpoint(pipe);
    if (usb_pipein (pipe))
        ep |= USB_DIR_IN;

    regval = ms_usb_control_msg(ptr, ptr->ctrl_send_pipe,
        USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0, ep, NULL, 0, 3*HZ);
    //usb_endpoint_running(ptr->pusb_dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));
    usb_settoggle(ptr->pusb_dev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);
    ms_usbhost_msg("<ms_usb_msc_clear_halt> regval = %d\n", regval);
    return regval;
}

static int ms_usb_interpret_uRequest_Block_result(struct ms_usdata *ptr, U32 pipe,
        U32 len, int regval, U32 partial)
{
    ms_usbhost_debug("regval=%d, transferred=%u/%u\n", regval, partial, len);

    switch (regval)
    {
        case 0:
            if (partial != len)
            {
                ms_usbhost_debug("--> short transfer\n");
                return XFER_SHORT;
            }

            ms_usbhost_debug("--> transfer complete\n");
            return XFER_GOOD;
        case -EPIPE:
            if (usb_pipecontrol(pipe))
            {
                ms_usbhost_msg("--> stall on control pipe\n");
                return XFER_STALLED;
            }
            ms_usbhost_debug("clearing endpoint halt for pipe 0x%x\n", pipe);
            if (ms_usb_msc_clear_halt(ptr, pipe) < 0)
                return XFER_ERROR;
            return XFER_STALLED;
        case -ETIMEDOUT:
            ms_usbhost_err("--> timeout or NAK\n");
            return XFER_ERROR;
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
        case -ENODEV:
            ms_usbhost_err("<urb_result> no dev\n");
            return XFER_NO_DEV;
#endif
        case -MS_ERR_OVERFLOW:
            ms_usbhost_msg("--> babble\n");
            return XFER_LONG;
        case -ECONNRESET:
            ms_usbhost_msg("--> transfer cancelled\n");
            return XFER_ERROR;
        case -MS_ERR_REMOTEIO:
            ms_usbhost_msg("--> short read transfer\n");
            return XFER_SHORT;
        case -EIO:
            ms_usbhost_msg("--> abort or disconnect in progress\n");
            return XFER_ERROR;
        default:
            ms_usbhost_err("--> unknown error (%d)\n", regval);
            return XFER_ERROR;
    }
}

int ms_usb_bulk_transfer_buf(struct ms_usdata *ptr, U32 pipe, void *buf, U32 len, U32 *act_len)
{
    int regval,time_out=10000;

    ms_usbhost_debug("<ms_usb_bulk_transfer_buf>: transfer %u bytes, buf=%p\n", len, buf);
    ms_usb_stuff_bulk_urb(ptr->current_urb, ptr->pusb_dev, pipe, buf, len,
            ms_usb_blocking_completion, NULL);
    regval = ms_usb_msg_common(ptr, time_out);
    if (act_len)
        *act_len = ptr->current_urb->u32ActualLength;

    return ms_usb_interpret_uRequest_Block_result(ptr, pipe, len, regval,
            ptr->current_urb->u32ActualLength);
}

static int ms_usb_bulk_transfer(struct ms_usdata* ptr, U32 pipe,
        void *buf, U32 trans_left, int *residual)
{
    int regval = -EIO;
    U32 partial;

    regval = ms_usb_bulk_transfer_buf(ptr, pipe, buf, trans_left, &partial);
    trans_left -= partial;

    if (residual)
        *residual = trans_left;
    return regval;
}

static int ms_us_port_reset(struct ms_usdata *pUsdata)
{
	int rval = 1;

	if (pUsdata->pusb_dev->eState == USB_STATE_NOTATTACHED)
		rval = -ENODEV;
	if (pUsdata->pusb_dev->eState == USB_STATE_SUSPENDED)
		rval = -EHOSTUNREACH;

	if (rval < 0)
	{
		ms_usbhost_err("unable to lock device for reset: %d\n", rval);
	}
	else {
		if (ms_test_bit(MS_IDX_DISCONNECTING, &pUsdata->flags)) {
			rval = -EIO;
			ms_usbhost_err("No reset during disconnect\n");
		} else {
			rval = ms_usb_reset_device(pUsdata->pusb_dev);
			if (rval)
			    ms_usbhost_err("usb_reset_device returns %d\n",	rval);
		}
	}
	return rval;
}

void ms_usb_invoke_transport(stCmd_scsi *scsi_request_block, struct ms_usdata *pUsdata)
{
    int auto_sense_activate,regval,temp_result;

    scsi_request_block->resid = 0;
    regval = pUsdata->transport(scsi_request_block, pUsdata);

    //if (pUsdata->usb_state == ABORTING_STATE) // get abort command from higer layer
    //{
    //    ms_usbhost_msg("--> command aborted\n");
    //    scsi_request_block->result = DID_ABORT << 16;
    //    goto Erros_Process;
    //}

#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
    if (regval == TRANSPORT_NO_DEVICE) // break if no device
    {
        ms_usbhost_msg("[USB][BULK] device removed\n");
        scsi_request_block->result = DID_NO_CONNECT << 16;
        return;
    }
#endif

    if (regval == TRANSPORT_ERROR)
    {
        ms_usbhost_msg("--> transport error, resetting\n");
        scsi_request_block->result = DID_ERROR << 16;
        goto Erros_Process;
    }

    if (regval == TRANSPORT_NO_SENSE)
    {
        ms_usbhost_err("--> transport no sense\n");
        scsi_request_block->result = MS_STAT_CHECK_CONDITION;
        return;
    }

    auto_sense_activate = 0;
    if (pUsdata->protocol == MS_CB_PR)
    {
        ms_usbhost_msg("--> CB transport device requiring auto-sense\n");
        auto_sense_activate = 1;
        if (pUsdata->subclass == MS_UFI_SC &&
            ((scsi_request_block->cmnd[0] == REQUEST_SENSE) || (scsi_request_block->cmnd[0] == INQUIRY)))
        {
            ms_usbhost_msg("** no auto-sense for a special command\n");
            auto_sense_activate = 0;
        }
    }

    if (regval == TRANSPORT_FAILED)
    {
        ms_usbhost_debug("--> transport command fail\n");
        auto_sense_activate = 1;
    }

    if ((scsi_request_block->resid > 0) && !((scsi_request_block->cmnd[0] == REQUEST_SENSE) ||
        (scsi_request_block->cmnd[0] == INQUIRY) || (scsi_request_block->cmnd[0] == MODE_SENSE)))
    {
        ms_usbhost_debug("--> unexpectedly short transfer\n");
        // to see kernel 2.6.28.9, not need to activate auto sense
        //auto_sense_activate = 1;
    }

    if (auto_sense_activate)
    {
        struct scMsc_cmd srb_need_auto_sense;

        //backup scsi request block and issue request sense
        memcpy(&srb_need_auto_sense, &pUsdata->srb, sizeof(srb_need_auto_sense));
        scsi_request_block->cmnd[0] = REQUEST_SENSE;
        scsi_request_block->cmnd[1] = srb_need_auto_sense.cmnd[1] & 0xE0;
        scsi_request_block->cmnd[4] = 18;
        scsi_request_block->cmd_len = 12;
        scsi_request_block->sc_data_direction = SCSI_DATA_READ;
        scsi_request_block->req_buf = scsi_request_block->sense_buffer;
        scsi_request_block->req_buf_len = 18;
        scsi_request_block->serial_number ^= 0x80000000;

        temp_result = pUsdata->transport(&pUsdata->srb, pUsdata);

        // restore previous scsi request block
        memcpy(&pUsdata->srb, &srb_need_auto_sense, sizeof(srb_need_auto_sense));

        //if (pUsdata->usb_state == ABORTING_STATE)
        //{
        //    ms_usbhost_msg("--> auto-sense aborted\n");
        //    scsi_request_block->result = DID_ABORT << 16;
        //    goto Erros_Process;
        //}
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
        if (regval == TRANSPORT_NO_DEVICE) // break if no device
        {
            ms_usbhost_msg("[USB][BULK] auto-sense device removed\n");
            scsi_request_block->result = DID_NO_CONNECT << 16;
            return;
        }
#endif
        if (temp_result != TRANSPORT_GOOD)
        {
            ms_usbhost_msg("--> auto-sense fail\n");
            scsi_request_block->result = DID_ERROR << 16;
            if (!(pUsdata->flags & MS_SCM_MULT_TARG))
            {
                goto Erros_Process;
            }
            return;
        }
        ms_usbhost_debug("--> Result from auto-sense is %d\n", temp_result);
        ms_usbhost_debug("--> code: 0x%x, key: 0x%x, ASC: 0x%x, ASCQ: 0x%x\n",
                scsi_request_block->sense_buffer[0], scsi_request_block->sense_buffer[2] & 0xf,
                scsi_request_block->sense_buffer[12], scsi_request_block->sense_buffer[13]);

        scsi_request_block->result = MS_STAT_CHECK_CONDITION;
        if ((scsi_request_block->sense_buffer[2] & 0xf) == 0x0)
            scsi_request_block->result = MS_STAT_GOOD;
    }
    else
        scsi_request_block->result = MS_STAT_GOOD;

    if (regval == TRANSPORT_FAILED)
        scsi_request_block->result = MS_STAT_CHECK_CONDITION;
    if ((pUsdata->protocol == MS_CB_PR) && (regval == TRANSPORT_GOOD) && ((scsi_request_block->sense_buffer[2] & 0xf) == 0x0))
        scsi_request_block->sense_buffer[0] = 0x0;
    return;

Erros_Process:

    ms_set_bit(MS_IDX_RESETTING, &pUsdata->flags, U32);
    ms_clear_bit(MS_IDX_ABORTING, &pUsdata->flags, U32);
    osapi_mutex_unlock(pUsdata->dev_mutex);
    regval = ms_us_port_reset(pUsdata);
    osapi_mutex_lock(pUsdata->dev_mutex);
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
    if (regval == -ENODEV)
        scsi_request_block->result = DID_NO_CONNECT << 16;
    else
#endif
    if (regval < 0)
        pUsdata->transport_reset(pUsdata);
    ms_clear_bit(MS_IDX_RESETTING, &pUsdata->flags, U32);
}

static int ms_usb_stor_data_transfer(stCmd_scsi *request_block, struct ms_usdata *ptr, U32 trans_len)
{
    U32 pipe;

    if(request_block->sc_data_direction == SCSI_DATA_READ)
        pipe = ptr->bulk_recv_pipe;
    else
        pipe = ptr->bulk_send_pipe;

    return ms_usb_bulk_transfer(ptr, pipe, request_block->req_buf, trans_len,
                &request_block->resid);
}

int ms_usb_CB_transport(stCmd_scsi *request_block, struct ms_usdata *ptr)
{
    U32 trans_len = request_block->req_buf_len;
    int regval,result,timeout=10000;

    ptr->cr->bRequestType = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    ptr->cr->bRequest = MS_CBI_ADSC;
    ptr->cr->wValue = 0;
    ptr->cr->wIndex = ptr->ifnum;
    ptr->cr->wLength = request_block->cmd_len;

    ms_usb_stuff_control_urb(ptr->current_urb, ptr->pusb_dev, ptr->ctrl_send_pipe,
            (unsigned char*) ptr->cr, request_block->cmnd, request_block->cmd_len,
            ms_usb_blocking_completion, NULL);
    result = ms_usb_msg_common(ptr, timeout);

    regval = ms_usb_interpret_uRequest_Block_result(ptr, ptr->ctrl_send_pipe,
                request_block->cmd_len, result, ptr->current_urb->u32ActualLength);

    if (regval == XFER_STALLED)
    {
        ms_usbhost_msg("CB xfer stalled\n");
        return TRANSPORT_FAILED;
    }
    if (regval != XFER_GOOD)
    {
        ms_usbhost_msg("CB xfer error\n");
        return TRANSPORT_ERROR;
    }
    if (trans_len)
    {
        regval = ms_usb_stor_data_transfer(request_block,ptr,trans_len);
        ms_usbhost_msg("CB data stage regval is 0x%x\n", regval);
        if (regval > XFER_STALLED)
            return TRANSPORT_ERROR;
    }

    return TRANSPORT_GOOD;
}

static int ms_usb_tranCB(struct stBulk_cb *bulk_cb, stCmd_scsi *request_block, struct ms_usdata *ptr, U32 trans_len)
{
    bulk_cb->Inscription = BULK_CB_SIGN;
    bulk_cb->DataLength = trans_len;
    if(request_block->sc_data_direction == SCSI_DATA_READ)
        bulk_cb->Flag = 1 << 7;
    else
        bulk_cb->Flag = 0;
    bulk_cb->Label = request_block->serial_number;
    bulk_cb->Lun = request_block->lun;
    bulk_cb->Len = request_block->cmd_len;

    memset(bulk_cb->CDB, 0, sizeof(bulk_cb->CDB));
    memcpy(bulk_cb->CDB, request_block->cmnd, bulk_cb->Len);

    return ms_usb_bulk_transfer_buf(ptr, ptr->bulk_send_pipe, bulk_cb, BULK_CB_WRAP_LEN, NULL);
}

static int ms_usb_tranCS(struct stBulk_cs *bulk_cs, struct ms_usdata *ptr)
{
    int regval;
    U32 len_csw;

    ms_usbhost_debug("Trying to get CSW...\n");
    regval = ms_usb_bulk_transfer_buf(ptr, ptr->bulk_recv_pipe, bulk_cs, BULK_CS_WRAP_LEN, &len_csw);
    if (regval == XFER_SHORT && len_csw == 0) // patch from Linux 3.1.10
    {
        ms_usbhost_warn("Since 0 packet received, retrying to get CSW again...\n");
        regval = ms_usb_bulk_transfer_buf(ptr, ptr->bulk_recv_pipe, bulk_cs, BULK_CS_WRAP_LEN, NULL);
    }
    if (regval == XFER_STALLED)
    {
        ms_usbhost_warn("Trying to get CSW again...\n");
        regval = ms_usb_bulk_transfer_buf(ptr, ptr->bulk_recv_pipe, bulk_cs, BULK_CS_WRAP_LEN, NULL);
    }
    return regval;
}

int ms_usb_bulk_transport(stCmd_scsi *request_block, struct ms_usdata *ptr)
{
    unsigned char *tmp_iobuf=0;
    struct stBulk_cb *bulk_cb = (struct stBulk_cb *)(ptr->iobuf - tmp_iobuf);
    struct stBulk_cs *bulk_cs = (struct stBulk_cs *)(ptr->iobuf - tmp_iobuf);
    U32 trans_len = request_block->req_buf_len;
    int regval,fake_sense = 0;

    regval = ms_usb_tranCB(bulk_cb,request_block,ptr,trans_len);
    if (regval != XFER_GOOD)
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
        return (regval == XFER_NO_DEV) ? TRANSPORT_NO_DEVICE : TRANSPORT_ERROR;
#else
        return TRANSPORT_ERROR;
#endif

    if (trans_len)
    {
        regval = ms_usb_stor_data_transfer(request_block,ptr,trans_len);
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
        if (regval == XFER_NO_DEV)
            return TRANSPORT_NO_DEVICE;
#endif
        if (regval == XFER_ERROR)
            return TRANSPORT_ERROR;
        if (regval == XFER_LONG)
            fake_sense = 1;
    }

    regval = ms_usb_tranCS(bulk_cs,ptr);

    /* check CSW stage result */
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
    if (regval == XFER_NO_DEV)
    {
        ms_usbhost_msg("device not found..\n");
        return TRANSPORT_NO_DEVICE;
    }
#endif
    if (regval != XFER_GOOD)
    {
        ms_usbhost_msg("transport error..\n");
        return TRANSPORT_ERROR;
    }

    ms_usbhost_debug("Bulk status Sig 0x%x T 0x%x R %d Stat 0x%x\n",
            bulk_cs->Inscription, bulk_cs->Label, bulk_cs->Remains, bulk_cs->Class);
    if ((bulk_cs->Inscription != BULK_CS_SIGN && bulk_cs->Inscription != BULK_CS_OLYMPUS_SIGN) ||
            bulk_cs->Label != request_block->serial_number || bulk_cs->Class > BULK_STAT_PHASE)
    {
        ms_usbhost_msg("Bulk logical error\n");
        return TRANSPORT_ERROR;
    }
    if(bulk_cs->Class == BULK_STAT_OK)
    {
        if (fake_sense)
        {
            memcpy(request_block->sense_buffer, invalid_CDB, sizeof(invalid_CDB));
            return TRANSPORT_NO_SENSE;
        }
        return TRANSPORT_GOOD;
    }
    else if(bulk_cs->Class == BULK_STAT_FAIL)
    {
        if (request_block->cmnd[0])
            ms_usbhost_err("CSW Bulk transfer fail, command %x\n", request_block->cmnd[0]);
        return TRANSPORT_FAILED;
    }
    else if(bulk_cs->Class == BULK_STAT_PHASE)
    {
        ms_usbhost_msg("Bulk transfer phase\n");
        return TRANSPORT_ERROR;
    }
    ms_usbhost_msg("Bulk transfer error\n");
    return TRANSPORT_ERROR;
}

static int ms_usb_reset_common(struct ms_usdata *ptr, unsigned char req, unsigned char rt,
        U16 val, U16 idx, void *data, U16 len)
{
    int regval,regval1,regval2;

    if (ms_test_bit(MS_IDX_DISCONNECTING, &ptr->flags)) // patch from Linux 2.6.28
    {
        ms_usbhost_err("US disconnected no reset\n");
        return FAILED;
    }

    regval = ms_usb_control_msg(ptr, ptr->ctrl_send_pipe, req, rt, val, idx, data, len, HZ*20);
    if (regval < 0)
    {
        ms_usbhost_msg("ms_usb_reset_common failed: %d\n", regval);
        return FAILED;
    }

    //osapi_up(&ptr->dev_semaphore); // patch from Linux 2.6.28
    set_current_state(TASK_UNINTERRUPTIBLE);
    osapi_schedule_timeout(HZ*6);
    set_current_state(TASK_RUNNING);
    //osapi_down(&ptr->dev_semaphore);

    if (ms_test_bit(MS_IDX_DISCONNECTING, &ptr->flags))
    {
        ms_usbhost_err("US DISCONNECTING\n");
        return FAILED;
    }

    regval1 = ms_usb_msc_clear_halt(ptr, ptr->bulk_recv_pipe);
    regval2 = ms_usb_msc_clear_halt(ptr, ptr->bulk_send_pipe);
    if (regval1 < 0 || regval2 < 0)
    {
        ms_usbhost_msg("clear reset failed\n");
        return FAILED;
    }
    ms_usbhost_msg("reset command done\n");
    return SUCCESS;
}

int ms_usb_CB_reset(struct ms_usdata *ptr)
{
    int CMD_SIZE=12;

    ms_usbhost_msg("Call..<ms_usb_CB_reset>\n");
    memset(ptr->iobuf, 0xFF, CMD_SIZE);

    ptr->iobuf[0] = 0x1d;//SEND_DIAGNOSTIC;
    ptr->iobuf[1] = 4;

    return ms_usb_reset_common(ptr, MS_CBI_ADSC, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                0, ptr->ifnum, ptr->iobuf, CMD_SIZE);
}

int ms_usb_bulk_reset(struct ms_usdata *ptr)
{
    ms_usbhost_msg("Call..<ms_usb_bulk_reset>\n");

    return ms_usb_reset_common(ptr, BULK_RESET_REQUEST, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                0, ptr->ifnum, NULL, 0);
}
