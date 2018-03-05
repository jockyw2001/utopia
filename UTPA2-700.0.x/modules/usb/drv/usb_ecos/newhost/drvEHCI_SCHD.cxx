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
int ms_ehci_get_frame_idx (struct usb_hcd *hcd);

/*-------------------------------------------------------------------------*/
static void ms_periodic_unlink (struct ehci_hcd *pEhci, U32 u32Frame, void *ptr)
{
	union ehci_qh_shadow  *prev_p = &pEhci->pshadow [u32Frame];
	U32   *hw_p = &pEhci->pPeriodic [u32Frame];
	union ehci_qh_shadow  here = *prev_p;
	union ehci_qh_shadow  *next_p1;

	while (here.ptr && here.ptr != ptr)
	{
		prev_p = (Q_NEXT_TYPE (*hw_p) == QH_TYPE)
			? (union ehci_qh_shadow *)(&prev_p->qh->qh_next)
			: NULL;
		//hw_p = &here.qh->hw_next_qh;
		hw_p = here.hw_next;
		here = *prev_p;
	}
	if (!here.ptr)
	{
		ms_debug_msg ("[periodic_unlink] entry %p no longer on u32Frame [%d]\n", ptr, (int)u32Frame);
		return;
	}

	next_p1 = (Q_NEXT_TYPE (*hw_p) == QH_TYPE)
		? (union ehci_qh_shadow *)(&here.qh->qh_next)
		: NULL;
	//*hw_p = here.qh->hw_next_qh;
	*hw_p = *here.hw_next;
	*prev_p = *next_p1;
	//next_p1->ptr = 0; // no meaning code
}

static U16 ms_periodic_usecs (struct ehci_hcd *pEhci, U32 u32Frame, U32 u32uFrame)
{
	U32      *phw_p = &pEhci->pPeriodic [u32Frame];
	union ehci_qh_shadow  *pQh = &pEhci->pshadow [u32Frame];
	U32    u32uSecs = 0;

	while (pQh->ptr)
	{
		if (Q_NEXT_TYPE (*phw_p) == QH_TYPE)
		{
			if (pQh->qh->hw_ep_state2 & (1 << u32uFrame))
				u32uSecs += pQh->qh->u8Usecs;
			if (pQh->qh->hw_ep_state2 & (1 << (8 + u32uFrame)))
				u32uSecs += pQh->qh->c_usecs;
			phw_p = &pQh->qh->hw_next_qh; // patch from Linux, move hardware pointer too
			pQh = &pQh->qh->qh_next;
		}
		else
			USB_ASSERT (0, "Next list type not qH type!\n");
	}
#ifdef  DEBUG
	/*
	 * set standard 80% (== 100 usec/uframe) periodic
	 * bandwidth as required by USB 2.0 by default, (100/125)
	 */
	if (u32uSecs > 100)
	ms_debug_err("overallocated u32uFrame %d, periodic is %d u32uSecs",
		u32Frame * 8 + u32uFrame, u32uSecs);
#endif
	return u32uSecs;
}

/*-------------------------------------------------------------------------*/
static int ms_enable_periodic (struct ehci_hcd *pEhci)
{
	U32  u32Cmd;
	int  iStatus;

	if (pEhci->u32PeriodicSched++) // patch from Linux
		return 0;

	ms_debug_func("<%s> ++\n", __FUNCTION__);
	iStatus = ms_check_status (&pEhci->op_regs->usbsts, USBSTS_PSS, 0, 9 * 125);
	if (iStatus != 0)
	{
		pEhci->hcd.state = HCD_STATE_HALT;
		ms_debug_err("enable periodic schedule time out!!!!\n");
		return iStatus;
	}

	u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd) | USBCMD_PSE;
	hcd_reg_writel (u32Cmd, (U32)&pEhci->op_regs->usbcmd);
	pEhci->hcd.state = HCD_STATE_RUNNING;
	pEhci->iNextUframe = hcd_reg_readl ((U32)&pEhci->op_regs->frindex)
		% (pEhci->u32PeriodicSize << 3);
	return 0;
}

static int ms_disable_periodic (struct ehci_hcd *pEhci)
{
	U32  u32Cmd;
	int  iStatus;

	if (--pEhci->u32PeriodicSched) // patch from Linux
		return 0;

	ms_debug_func("<%s> ++\n", __FUNCTION__);
	iStatus = ms_check_status (&pEhci->op_regs->usbsts, USBSTS_PSS, USBSTS_PSS, 9 * 125);
	if (iStatus != 0)
	{
		ms_debug_err("disable periodic schedule time out!!!!\n");
		return iStatus;
	}

	u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd) & ~USBCMD_PSE;
	hcd_reg_writel (u32Cmd, (U32)&pEhci->op_regs->usbcmd);
	pEhci->iNextUframe = -1;
	return 0;
}

/*-------------------------------------------------------------------------*/
static int ms_qh_schedule (struct ehci_hcd *pEhci, struct ehci_qh *pQh);
static void ms_intr_deschedule (
	struct ehci_hcd  *pEhci,
	struct ehci_qh  *pQh1
	)
{
	U32  u32Frame = pQh1->u16Start;
	U32  iWait;
	U32  period;
	//int			rc;

	ms_debug_func("<%s> ++\n", __FUNCTION__);

	/*** qh_unlink_periodic statement ***/
	if ((period = pQh1->u16Period) == 0)
		period = 1;

	/*** qh_unlink_periodic statement ***/
	do
	{
		ms_periodic_unlink (pEhci, u32Frame, pQh1);
		//ms_qh_put (pEhci, pQh1); // moved
		u32Frame += period;
	} while (u32Frame < pEhci->u32PeriodicSize);

	// TODO: update bandwidth per ep if required
	pQh1->qh_status = QH_STS_UNLINK;
	pQh1->qh_next.ptr = 0;
	ms_qh_put (pEhci, pQh1); // patch from Linux here

	#if 1
	ms_disable_periodic(pEhci);
	#else
	pEhci->u32PeriodicSched--;
	if (!pEhci->u32PeriodicSched)
		ms_disable_periodic (pEhci);
	else
	{
		ms_debug_msg ("periodic schedule still enabled\n");
	}
	#endif
	/*** end of qh_unlink_periodic statement ***/
	wmb(); // make sure hardware visible (hint from linux 3.10.40)

	/* patch from Linux 2.6.28 */
	if (list_empty (&pQh1->qtd_list) || (QH_CMASK & pQh1->hw_ep_state2) != 0)
		iWait = 2;
	else
		iWait = 55;

	udelay (iWait);
	#if 0
	if (((ms_ehci_get_frame_idx (&pEhci->hcd) - u32Frame) % pQh1->u16Period) == 0)
	{
		if (iWait)
		{
			udelay (125);
			pQh1->hw_next_qh = EHCI_LIST_END;
		}
		else
		{
			ms_debug_msg ("ms_intr_deschedule...\n");
		}
	}
	else
		pQh1->hw_next_qh = EHCI_LIST_END;
	#endif

	pQh1->qh_status = QH_STS_IDLE;

	ms_debug_debug ("descheduled qh %p, period = %d u32Frame = %d count = %d, u32PeriodicSched = %d\n",
		pQh1, pQh1->u16Period, (int)u32Frame,
		osapi_atomic_read(&pQh1->tRefCnt), (int)pEhci->u32PeriodicSched);

	#if 1 // patch from Linux
	pQh1->hw_next_qh = EHCI_LIST_END;
	wmb ();
	#endif

	#if 0
	ms_qh_completions(pEhci, pQh1, NULL);

	if (!list_empty(&pQh1->qtd_list) && HCD_IS_RUNNING(pEhci->hcd.state))
	{
		rc = ms_qh_schedule(pEhci, pQh1); // reschedule

		if (rc != 0)
			ms_debug_err("can't reschedule qh %p, err %d\n", pQh1, rc);
	}
	#endif
}

static int ms_check_period (
	struct ehci_hcd *pEhci,
	U32  u32Frame,
	U32  u32uFrame,
	U32  u32Period,
	U32  u32Usecs
	)
{
	int  claimed;

	if (u32uFrame >= 8)
		return 0;

	u32Usecs = 100 - u32Usecs; // high speed case
#if 0
	if ( pEhci->hcd.eSpeed == USB_HIGH_SPEED )
		u32Usecs = 100 - u32Usecs;
	else
		u32Usecs = 900 - u32Usecs;
#endif

	/* period=0, check every micro-frame, uframe==7??? */
	if (u32Period == 0) {
		do {
			for (u32uFrame = 0; u32uFrame < 7; u32uFrame++) {
				claimed = ms_periodic_usecs (pEhci, u32Frame, u32uFrame);
				if (claimed > u32Usecs)
					return 0;
			}
		} while ((u32Frame += 1) < pEhci->u32PeriodicSize);

	/* just check the specified uframe, at that period */
	} else {
		do
		{
			/* un-patch base on linux code */
			//if (pEhci->pshadow [u32Frame].ptr)
			//	return 0;

			claimed = ms_periodic_usecs (pEhci, u32Frame, u32uFrame);
			if ((U32)claimed > u32Usecs)
				return 0;

		} while ((u32Frame += u32Period) < pEhci->u32PeriodicSize);
	}

	/* pass all check! */
	return 1;
}

static int ms_same_tt (struct usb_device *dev1, struct usb_device *dev2)
{
	if (!dev1->tt || !dev2->tt)
		return 0;
	if (dev1->tt != dev2->tt)
		return 0;
	if (dev1->tt->multi)
		return dev1->u32TTPort == dev2->u32TTPort;
	else
		return 1;
}

static int ms_tt_no_collision (
	struct ehci_hcd		*pEhci,
	U32     		u32Period,
	struct usb_device	*dev,
	U32     		frame,
	U32			uf_mask
)
{
	if (u32Period == 0)	/* error */
		return 0;

	for (; frame < pEhci->u32PeriodicSize; frame += u32Period) {
		union ehci_qh_shadow	here;
		U32			type;

		here = pEhci->pshadow [frame];
		type = Q_NEXT_TYPE(pEhci->pPeriodic [frame]);
		while (here.ptr) {
			switch (type) {
			case QH_TYPE:
				if (ms_same_tt (dev, here.qh->dev)) {
					U32		mask;

					mask = here.qh->hw_ep_state2;
					/* "knows" no gap is needed */
					mask |= mask >> 8;
					if (mask & uf_mask)
						break;
				}
				type = Q_NEXT_TYPE(here.qh->hw_next_qh);
				here = here.qh->qh_next;
				continue;
			default:
				ms_debug_err ("periodic frame %d bogus type %d\n",
					frame, type);
			}

			/* collision or error */
			return 0;
		}
	}

	/* no collision */
	return 1;
}

static int ms_check_intr_schedule (
	struct ehci_hcd    *pEhci,
	U32    u32Frame,
	U32    u32uFrame,
	const struct ehci_qh  *pQh,
	U32      *c_maskp
	)
{
	int    iRetVal = -ENOSPC;
	U8		mask = 0;

	/* patch from linux */
	if (pQh->c_usecs && u32uFrame >= 6)		/* may be FSTN */
		goto done;

	if (!ms_check_period (pEhci, u32Frame, u32uFrame, pQh->u16Period, pQh->u8Usecs))
		goto done;
	if (!pQh->c_usecs)
	{
		iRetVal = 0;
		*c_maskp = 0;
		goto done;
	}

#if 1 // adding tt collision check
	mask = 0x03 << (u32uFrame + pQh->u8Gap_uf);
	//*c_maskp = cpu_to_hc32(ehci, mask << 8);
	*c_maskp = (0x03 << (8 + u32uFrame + pQh->u8Gap_uf));

	mask |= 1 << u32uFrame;
	if (ms_tt_no_collision (pEhci, pQh->u16Period, pQh->dev, u32Frame, mask)) {
		if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf + 1,
					pQh->u16Period, pQh->c_usecs))
			goto done;
		if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf,
					pQh->u16Period, pQh->c_usecs))
			goto done;
		iRetVal = 0; // pass all periodic check
	}

#else
	if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf + 1,
		pQh->u16Period, pQh->c_usecs))
		goto done;
	if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf,
		pQh->u16Period, pQh->c_usecs))
	    goto done;

	*c_maskp = (0x03 << (8 + u32uFrame + pQh->u8Gap_uf));
	iRetVal = 0;
#endif
done:
	return iRetVal;
}

static int ms_qh_link_periodic (struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	unsigned	i;
	unsigned	period = qh->u16Period;

	ms_debug_debug ("link qh%d-%04x/%p start %d [%d/%d us]\n",
		period, qh->hw_ep_state2 & (QH_CMASK | QH_SMASK),
		qh, qh->u16Start, qh->u8Usecs, qh->c_usecs);

	/* high bandwidth, or otherwise every microframe */
	if (period == 0)
		period = 1;

	for (i = qh->u16Start; i < ehci->u32PeriodicSize; i += period) {
		union ehci_qh_shadow	*prev = &ehci->pshadow[i];
		U32			*hw_p = &ehci->pPeriodic[i];
		union ehci_qh_shadow	here = *prev;
		//U32			type = 0;

		/* skip the iso nodes at list head */
		#if 0
		while (here.ptr) {
			type = Q_NEXT_TYPE(*hw_p);
			if (type == QH_TYPE)
				break;
			prev = periodic_next_shadow(ehci, prev, type);
			hw_p = shadow_next_periodic(ehci, &here, type);
			here = *prev;
		}
		#endif

		/* sorting each branch by period (slow-->fast)
		 * enables sharing interior tree nodes
		 */
		while (here.ptr && qh != here.qh) {
			if (qh->u16Period > here.qh->u16Period)
				break;
			prev = &here.qh->qh_next;
			hw_p = &here.qh->hw_next_qh;
			here = *prev;
		}
		/* link in this qh, unless some earlier pass did that */
		if (qh != here.qh) {
			qh->qh_next = here;
			if (here.qh)
				qh->hw_next_qh = *hw_p;
			wmb ();
			prev->qh = qh;
			*hw_p = QH_NEXT (qh->qh_dma_addr);
		}
	}
	qh->qh_status = QH_STS_LINKED;
	qh->xacterrs = 0;
	ms_qh_get (qh);

	// TODO: update bandwidth per ep if required

	/* maybe enable periodic schedule processing */
	return ms_enable_periodic(ehci);
}

static int ms_qh_schedule (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
	int  iStatus;
	U32  u32uFrame1;
	U32  u32_C_mask1;
	U32  u32Frame1;

	pQh->hw_next_qh = EHCI_LIST_END;
	u32Frame1 = pQh->u16Start;

	if (u32Frame1 < pQh->u16Period)
	{
		u32uFrame1 = ms_find_1st_set (pQh->hw_ep_state2 & 0x00ff);
		iStatus = ms_check_intr_schedule (pEhci, u32Frame1, --u32uFrame1,
			pQh, &u32_C_mask1);
	}
	else
	{
		u32uFrame1 = 0;
		u32_C_mask1 = 0;
		iStatus = -ENOSPC;
		/* the 1st path, pQh->u16Start is initialed as NOFRAME(65535) */
	}

	if (iStatus)
	{
		if (pQh->u16Period) {
			//u32Frame1 = pQh->u16Period - 1;
			//do
			//{
			int i;

			for (i = pQh->u16Period; iStatus && i > 0; --i) {
				/* patch from linux, to generate relative random start frame */
				u32Frame1 = ++pEhci->u32random_frm % pQh->u16Period;
				for (u32uFrame1 = 0; u32uFrame1 < 8; u32uFrame1++)
				{
					iStatus = ms_check_intr_schedule (pEhci, u32Frame1, u32uFrame1, pQh, &u32_C_mask1);
					if (iStatus == 0) // first fit @ uframe
					{
						ms_debug_msg("[USB] first fit (frame,uframe)=(%d,%d)\n", u32Frame1, u32uFrame1);
						break;
					}
				}
			//} while (iStatus && u32Frame1--);
			}
		/* pQh->u16Period == 0 means every uframe */
		} else {
			u32Frame1 = 0;
			iStatus = ms_check_intr_schedule (pEhci, 0, 0, pQh, &u32_C_mask1);
		}
		if (iStatus) // no time slot fit
		{
			ms_debug_msg("[USB] no time slot fit\n");
			goto done;
		}
		pQh->u16Start = u32Frame1;

		pQh->hw_ep_state2 &= ~0xffff; // clear QH_CMASK and QH_SMASK
		pQh->hw_ep_state2 |= (pQh->u16Period ? (1 << u32uFrame1) : QH_SMASK) | u32_C_mask1;
	}
	else
		ms_debug_msg ("reused previous pQh %p schedule", pQh);

	/* ms_qh_link_periodic() moved, patch as linux 3.10.x */
	//iStatus = ms_qh_link_periodic (pEhci, pQh);

	ms_debug_msg ("[ms_qh_schedule] qh->hw_info2:%x ,usecs %d/%d, period %d.0 starting %d.%d (gap %d)\n",
		pQh->hw_ep_state2 & (QH_CMASK | QH_SMASK), pQh->u8Usecs, pQh->c_usecs, pQh->u16Period, (int)u32Frame1, (int)u32uFrame1, pQh->u8Gap_uf);
	#if 0
	pQh->qh_status = QH_STS_LINKED;
	ms_debug_msg ("[ms_qh_schedule] scheduled pQh %p usecs %d/%d period %d.0 starting %d.%d (gap %d)\n",
		pQh, pQh->u8Usecs, pQh->c_usecs, pQh->u16Period, (int)u32Frame1, (int)u32uFrame1, pQh->u8Gap_uf);
	do
	{
		if (pEhci->pshadow [u32Frame1].ptr != 0)
		{
			USB_ASSERT (0, "pEhci->pshadow [u32Frame1].ptr != 0\n");
		}
		else
		{
			pEhci->pshadow [u32Frame1].qh = ms_qh_get (pQh);
			pEhci->pPeriodic [u32Frame1] = QH_NEXT (pQh->qh_dma_addr);
		}
		wmb ();
		u32Frame1 += pQh->u16Period;
	} while (u32Frame1 < pEhci->u32PeriodicSize);

	if (!pEhci->u32PeriodicSched++)
		iStatus = ms_enable_periodic (pEhci);
	#endif
done:
	return iStatus;
}

static int ms_intr_submit (
	struct ehci_hcd    *pEhci,
	struct urb    *pUrb,
	struct list_head  *qtd_list
	)
{
	U32    epnum1;
	U32    u32Flags;
	struct ehci_qh    *pQh1;
	struct s_hcd_dev    *dev;
	int      is_input;
	int      status = 0;
	struct list_head  stEmpty;

	epnum1 = usb_pipeendpoint (pUrb->u32Pipe);
	is_input = usb_pipein (pUrb->u32Pipe);
	if (is_input && !usb_pipecontrol(pUrb->u32Pipe))
		epnum1 |= 0x10;

	osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
	dev = (struct s_hcd_dev *)pUrb->dev->hcpriv;

	// TODO: HC accessible decision, if not jump to done

	ms_list_init (&stEmpty);
	pQh1 = ms_qh_append_tds (pEhci, pUrb, &stEmpty, epnum1, &dev->ep [epnum1]);
	if (pQh1 == 0)
	{
		status = -ENOMEM;
		goto done;
	}
	if (pQh1->qh_status == QH_STS_IDLE)
	{
		if ((status = ms_qh_schedule (pEhci, pQh1)) != 0)
			goto done;
	}

	pQh1 = ms_qh_append_tds (pEhci, pUrb, qtd_list, epnum1, &dev->ep [epnum1]);
	USB_ASSERT(pQh1!=0, "BUG_ON()!!!! pQh1==NULL\n");

	/* patch from linux code from 3.10.x */
#if 1
	/* stuff into the periodic schedule */
	if (pQh1->qh_status == QH_STS_IDLE) {
		ms_qh_refresh(pEhci, pQh1);
		ms_qh_link_periodic(pEhci, pQh1);
	}
#endif
	Chip_Flush_Memory();

done:
	osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
	if (status)
		ms_qtd_register_free (pEhci, qtd_list);

	return status;
}

static U32 ms_intr_complete (
	struct ehci_hcd  *pEhci,
	struct ehci_qh  *pQh,
	struct stPtRegs  *pRegs
	)
{
	U32  u32Count;
	if ((pQh->hw_token & QTD_STS_ACT) != 0)
		return 0;

	if (ms_is_empty_list (&pQh->qtd_list))
	{
		ms_debug_msg ("[intr qh] %p no TDs?", pQh);
		return 0;
	}

	u32Count = ms_qh_completions (pEhci, pQh, pRegs);
	if (ms_is_empty_list (&pQh->qtd_list))
		ms_intr_deschedule (pEhci, pQh);
	return u32Count;
}

/*-------------------------------------------------------------------------*/

static void ms_scan_periodic (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
	U32  u32Frame, u32Clock, u32clock_uframe, u32Mod;
	U32  u32Count = 0;

	u32Mod = pEhci->u32PeriodicSize << 3;
	u32Frame = pEhci->iNextUframe >> 3;
	if (HCD_IS_RUNNING (pEhci->hcd.state))
		u32clock_uframe = hcd_reg_readl ((U32)&pEhci->op_regs->frindex);
	else
		//u32clock_uframe = (u32Frame << 3) - 1;
		u32clock_uframe = pEhci->iNextUframe + u32Mod - 1;
	u32clock_uframe %= u32Mod;
	u32Clock = u32clock_uframe >> 3;

	for (;;)
	{
		union ehci_qh_shadow  q, *q_p;
		U32      u32Type, *hw_p;

restart:
		q_p = &pEhci->pshadow [u32Frame];
		hw_p = &pEhci->pPeriodic [u32Frame];
		q.ptr = q_p->ptr;
		u32Type = Q_NEXT_TYPE (*hw_p);
		u32Count = 0;

		while (q.ptr != 0)
		{
			int      last;
			union ehci_qh_shadow  temp;

			if (u32Type == QH_TYPE)
			{
				last = (q.qh->hw_next_qh == EHCI_LIST_END);
				temp = q.qh->qh_next;
				u32Type = Q_NEXT_TYPE (q.qh->hw_next_qh);
				u32Count = ms_intr_complete (pEhci, ms_qh_get (q.qh), pRegs);
				ms_qh_put (pEhci, q.qh);
				q = temp;
			}
			else
			{
				ms_debug_msg ("corrupt u32Type %d u32Frame %d shadow %p",
					(int)u32Type, (int)u32Frame, q.ptr);
				// BUG ();
				last = 1;
				q.ptr = 0;
			}

			if (q.ptr == 0 && !last)
				goto restart;
			if (u32Count) {
				if (pEhci->u32PeriodicSched > 0)
					goto restart;
				u32Frame = u32Clock;
				break;
			}
		}

		if (u32Frame == u32Clock)
		{
			U32  now;

			if (!HCD_IS_RUNNING (pEhci->hcd.state) || pEhci->u32PeriodicSched == 0)
				break;

			pEhci->iNextUframe = u32clock_uframe;
			now = hcd_reg_readl ((U32)&pEhci->op_regs->frindex) % u32Mod;
			if (u32clock_uframe == now)
				break;

			u32clock_uframe = now;
			u32Clock = u32clock_uframe >> 3;
		}
		else
			u32Frame = (u32Frame + 1) % pEhci->u32PeriodicSize;
	}
}

