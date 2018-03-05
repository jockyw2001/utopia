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

int ehci_get_frame (struct usb_hcd *hcd);

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

static union ehci_shadow *
periodic_next_shadow (union ehci_shadow *periodic, int tag)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  switch (tag) {
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case Q_TYPE_QH:
    return &periodic->qh->qh_next;
#ifdef have_fstn
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case Q_TYPE_FSTN:
    return &periodic->fstn->fstn_next;
#endif
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case Q_TYPE_ITD:
    return &periodic->itd->itd_next;
#ifdef have_split_iso
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  case Q_TYPE_SITD:
    return &periodic->sitd->sitd_next;
#endif 
  }
  dbg ("[periodic_next_shadow] BAD shadow %p tag %d", periodic->ptr, tag);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return 0;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int periodic_unlink (struct ehci_hcd *ehci, U32 frame, void *ptr)
{
  union ehci_shadow  *prev_p = &ehci->pshadow [frame];
  U32      *hw_p = &ehci->periodic [frame];
  union ehci_shadow  here = *prev_p;
  union ehci_shadow  *next_p1;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  while (here.ptr && here.ptr != ptr) {
  	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    prev_p = periodic_next_shadow (prev_p, Q_NEXT_TYPE (*hw_p));
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    hw_p = &here.qh->hw_next;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    here = *prev_p;
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!here.ptr) {
    dbg ("[periodic_unlink] entry %p no longer on frame [%d]", ptr, frame);
    return 0;
  }
  // vdbg ("periodic unlink %p from frame %d", ptr, frame);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  next_p1 = periodic_next_shadow (&here, Q_NEXT_TYPE (*hw_p));
  *hw_p = here.qh->hw_next;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  *prev_p = *next_p1;
  next_p1->ptr = 0;

  return 1;
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

static U16
periodic_usecs (struct ehci_hcd *ehci, U32 frame, U32 uframe)
{
  U32      *hw_p = &ehci->periodic [frame];
  union ehci_shadow  *q = &ehci->pshadow [frame];
  U32    usecs = 0;

  while (q->ptr) {
    switch (Q_NEXT_TYPE (*hw_p)) {
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    case Q_TYPE_QH:   
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->qh->hw_info2 & CPUToLE32 (1 << uframe))
        usecs += q->qh->usecs;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->qh->hw_info2 & CPUToLE32 (1 << (8 + uframe)))
        usecs += q->qh->c_usecs;
      q = &q->qh->qh_next;
      break;
#ifdef have_fstn
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    case Q_TYPE_FSTN:
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->fstn->hw_prev != EHCI_LIST_END) {
        dbg ("not counting FSTN bandwidth yet ...%s","");
      }
      q = &q->fstn->fstn_next;
      break;
#endif
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    case Q_TYPE_ITD:
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->itd->hw_transaction [uframe] != 0)
        usecs += q->itd->usecs;
      q = &q->itd->itd_next;
      break;
#ifdef have_split_iso
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    case Q_TYPE_SITD:
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      temp = q->sitd->hw_fullspeed_ep &
        CPUToLE32 (1 << 31);
     

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->sitd->hw_uframe & CPUToLE32 (1 << uframe)) {
        if (temp)
          usecs += HS_USECS (188);
        else
          usecs += HS_USECS (1);
      }

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (q->sitd->hw_uframe &
          CPUToLE32 (1 << (8 + uframe))) {
        if (temp)
          usecs += HS_USECS (0);
        else
          usecs += HS_USECS (188);
      }
      q = &q->sitd->sitd_next;
      break;
#endif /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    default:
      BUG ();
    }
  }
#ifdef  DEBUG
  #ifndef CONFIG_FARADAY_FOTG200
  if (usecs > 100)
    err ("overallocated uframe %d, periodic is %d usecs",
      frame * 8 + uframe, usecs);
  #endif
#endif
  return usecs;
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

static int enable_periodic (struct ehci_hcd *ehci)
{
  U32  cmd;
  int  status;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  status = handshake (&ehci->regs->status, STS_PSS, 0, 9 * 125);
  if (status != 0) {
    ehci->hcd.state = USB_STATE_HALT;
    return status;
  }

  cmd = ehci_readl ((U32)&ehci->regs->command) | CMD_PSE;
  ehci_writel (cmd, (U32)&ehci->regs->command);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  ehci->hcd.state = USB_STATE_RUNNING;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  ehci->next_uframe = ehci_readl ((U32)&ehci->regs->frame_index)
        % (ehci->periodic_size << 3);
  return 0;
}

static int disable_periodic (struct ehci_hcd *ehci)
{
  U32  cmd;
  int  status;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  status = handshake (&ehci->regs->status, STS_PSS, STS_PSS, 9 * 125);
  if (status != 0) {
    //ehci->hcd.state = USB_STATE_HALT; //Note, Fix me
    return status;
  }

  cmd = ehci_readl ((U32)&ehci->regs->command) & ~CMD_PSE;
  ehci_writel (cmd, (U32)&ehci->regs->command);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  ehci->next_uframe = -1;
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

static void intr_deschedule (
  struct ehci_hcd  *ehci,
  struct ehci_qh  *qh1,
  int    wait
) {
  int    status;
  U32  frame = qh1->start;

  do {
    periodic_unlink (ehci, frame, qh1);
    qh_put (ehci, qh1);
    frame += qh1->period;
  } while (frame < ehci->periodic_size);

  qh1->qh_state = QH_STATE_UNLINK;
  qh1->qh_next.ptr = 0;
  ehci->periodic_sched--;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!ehci->periodic_sched)
    status = disable_periodic (ehci);
  else {
    status = 0;
    vdbg ("periodic schedule still enabled%s%s","","");
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (((ehci_get_frame (&ehci->hcd) - frame) % qh1->period) == 0) {
    if (wait) {
      udelay (125);
      qh1->hw_next = EHCI_LIST_END;
    } else {
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      vdbg ("intr_deschedule...%s%s","","");
    }
  } else
    qh1->hw_next = EHCI_LIST_END;

  qh1->qh_state = QH_STATE_IDLE;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd_to_bus (&ehci->hcd)->bandwidth_allocated -=
    (qh1->usecs + qh1->c_usecs) / qh1->period;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dbg ("descheduled qh %p, period = %d frame = %d count = %d, urbs = %d",
    qh1, qh1->period, frame,
    atomic_read(&qh1->refcount), ehci->periodic_sched);
}

static int check_period (
  struct ehci_hcd *ehci,
  U32  frame,
  U32  uframe,
  U32  period,
  U32  usecs
) {
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (uframe >= 8)
    return 0;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  #ifndef CONFIG_FARADAY_FOTG200
  usecs = 100 - usecs;
  #else
  if ( eCurURBSpeed == USB_SPEED_HIGH )
    usecs = 100 - usecs;
  else
    usecs = 900 - usecs;
  #endif

  do {
    int  claimed;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (ehci->pshadow [frame].ptr)
      return 0;

    claimed = periodic_usecs (ehci, frame, uframe);
    if ((U32)claimed > usecs)
      return 0;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } while ((frame += period) < ehci->periodic_size);

  // success!
  return 1;
}

static int check_intr_schedule (
  struct ehci_hcd    *ehci,
  U32    frame,
  U32    uframe,
  const struct ehci_qh  *qh,
  U32      *c_maskp
)
{
      int    retval = -ENOSPC;

  if (!check_period (ehci, frame, uframe, qh->period, qh->usecs))
    goto done;
  #ifndef CONFIG_FARADAY_FOTG200
  if (!qh->c_usecs) {
    retval = 0;
    *c_maskp = CPUToLE32 (0);
    goto done;
  }
  #endif

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  #ifndef CONFIG_FARADAY_FOTG200
  if (!check_period (ehci, frame, uframe + qh->gap_uf + 1,
        qh->period, qh->c_usecs))
    goto done;
  if (!check_period (ehci, frame, uframe + qh->gap_uf,
        qh->period, qh->c_usecs))
    goto done;

  *c_maskp = CPUToLE32 (0x03 << (8 + uframe + qh->gap_uf));
  #endif
  retval = 0;
done:
  return retval;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int qh_schedule (struct ehci_hcd *ehci, struct ehci_qh *qh)
{
  int     status;
  U32  uframe1;
  U32    c_mask1;
  U32  frame1;    

  qh->hw_next = EHCI_LIST_END;
  frame1 = qh->start;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (frame1 < qh->period) {
    uframe1 = ffs (LE32ToCPU(qh->hw_info2) & 0x00ff);
    status = check_intr_schedule (ehci, frame1, --uframe1,
        qh, &c_mask1);
  } else {
    uframe1 = 0;
    c_mask1 = 0;
    status = -ENOSPC;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (status) {
    frame1 = qh->period - 1;
    do {
      for (uframe1 = 0; uframe1 < 8; uframe1++) {
        status = check_intr_schedule (ehci,
            frame1, uframe1, qh,
            &c_mask1);
        if (status == 0)
          break;
      }
    } while (status && frame1--);
    if (status)
      goto done;
    qh->start = frame1;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    qh->hw_info2 &= ~0xffff;
    qh->hw_info2 |= CPUToLE32 (1 << uframe1) | c_mask1;
  } else
    dbg ("reused previous qh %p schedule", qh);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  qh->qh_state = QH_STATE_LINKED;
  dbg ("[qh_schedule] scheduled qh %p usecs %d/%d period %d.0 starting %d.%d (gap %d)",
    qh, qh->usecs, qh->c_usecs,
    qh->period, frame1, uframe1, qh->gap_uf);
  do {
    if (unlikely (ehci->pshadow [frame1].ptr != 0)) {

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

      BUG ();
    } else {
      ehci->pshadow [frame1].qh = qh_get (qh);
      ehci->periodic [frame1] =
        QH_NEXT (qh->qh_dma);
    }
    wmb ();
    frame1 += qh->period;
  } while (frame1 < ehci->periodic_size);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd_to_bus (&ehci->hcd)->bandwidth_allocated +=
    (qh->usecs + qh->c_usecs) / qh->period;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!ehci->periodic_sched++)
    status = enable_periodic (ehci);
done:
  return status;
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

static int intr_submit (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  struct list_head  *qtd_list,
  int      mem_flags
) {
  U32    epnum1;
  U32    flags;
  struct ehci_qh    *qh1;
  struct hcd_dev    *dev;
  int      is_input;
  int      status = 0;
  struct list_head  empty;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  epnum1 = usb_pipeendpoint (urb->pipe);
  is_input = usb_pipein (urb->pipe);
  if (is_input)
    epnum1 |= 0x10;

  spin_lock_irqsave (&ehci->lock, flags);
  dev = (struct hcd_dev *)urb->dev->hcpriv;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  INIT_LIST_HEAD (&empty);
  qh1 = qh_append_tds (ehci, urb, &empty, epnum1, &dev->ep [epnum1]);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (qh1 == 0) {
    status = -ENOMEM;
    goto done;
  }
  if (qh1->qh_state == QH_STATE_IDLE) {
    if ((status = qh_schedule (ehci, qh1)) != 0)
      goto done;
  }
    
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  qh1 = qh_append_tds (ehci, urb, qtd_list, epnum1, &dev->ep [epnum1]);
  

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  hcd_to_bus (&ehci->hcd)->bandwidth_int_reqs++;

  //#ifdef ENABLE_PIPE_FLUSH  
  Chip_Flush_Memory();
  //#endif

done:
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  spin_unlock_irqrestore (&ehci->lock, flags);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (status)
    qtd_list_free (ehci, urb, qtd_list);

  return status;
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

static U32
intr_complete (
  struct ehci_hcd  *ehci,
  U32  frame,
  struct ehci_qh  *qh,
  struct pt_regs  *regs
) {
  U32  count;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (likely ((qh->hw_token & CPUToLE32 (QTD_STS_ACTIVE))
      != 0))
    return 0;
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  if (unlikely (list_empty (&qh->qtd_list))) {
    dbg ("[intr qh] %p no TDs?", qh);
    return 0;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  count = qh_completions (ehci, qh, regs);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (unlikely (list_empty (&qh->qtd_list)))
    intr_deschedule (ehci, qh, 0);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  return count;
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

#ifdef have_iso_itd
/*-------------------------------------------------------------------------*/
static void
itd_free_list (struct ehci_hcd *ehci, struct urb *urb)
{
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  struct ehci_itd *first_itd = (struct ehci_itd *) urb->hcpriv;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  while (!list_empty (&first_itd->itd_list)) {
    struct ehci_itd  *itd;

    itd = list_entry (first_itd->itd_list.next,  struct ehci_itd, itd_list);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    list_del (&itd->itd_list);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    pci_pool_free (ehci->itd_pool, itd, itd->itd_dma);
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  pci_pool_free (ehci->itd_pool, first_itd, first_itd->itd_dma);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb->hcpriv = 0;
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

static int
itd_fill (
  struct ehci_hcd  *ehci,
  struct ehci_itd  *itd,
  struct urb  *urb,
  U32  index,    
  dma_addr_t  dma    
) {
  cyg_uint64    temp;
  U32    buf1;
  U32  i, epnum, maxp, multi;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int   length;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int    is_input;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->hw_next = EHCI_LIST_END;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->urb = urb;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->index = index;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  length = urb->iso_frame_desc [index].length;
  dma += urb->iso_frame_desc [index].offset;
  temp = dma & ~0x0fff;
  for (i = 0; i < 2; i++) {
    itd->hw_bufp [i] = CPUToLE32 ((U32) temp);
    itd->hw_bufp_hi [i] = CPUToLE32 ((U32)(temp >> 32));
    temp += 0x1000;
  }
  itd->buf_dma = dma;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  epnum = usb_pipeendpoint (urb->pipe);
  is_input = usb_pipein (urb->pipe);
  if (is_input) {
    maxp = urb->dev->epmaxpacketin [epnum];
    buf1 = (1 << 11);
  } else {
    maxp = urb->dev->epmaxpacketout [epnum];
    buf1 = 0;
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  buf1 |= (maxp & 0x03ff);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  multi = 1;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  multi += (maxp >> 11) & 0x03;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  maxp &= 0x03ff;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  maxp *= multi;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if ( (length < 0) || (maxp < (U32) length) ) {
    dbg ("BAD iso packet: %d bytes, max %d, urb %p [%d] (of %d)",
      length, maxp, urb, index,
      urb->iso_frame_desc [index].length);
    return -ENOSPC;
  }
  itd->usecs = usb_calc_bus_time (USB_SPEED_HIGH, is_input, 1, length);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->hw_bufp [0] |= CPUToLE32 ((epnum << 8) | urb->dev->devnum);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->hw_bufp [1] |= CPUToLE32 (buf1);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->hw_bufp [2] |= CPUToLE32 (multi);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->transaction = EHCI_ISOC_ACTIVE;
  itd->transaction |= dma & 0x0fff;    
  if ((index + 1) == (U32) urb->number_of_packets)
    itd->transaction |= EHCI_ITD_IOC;   
  itd->transaction |= length << 16;
  itd->transaction = CPUToLE32(itd->transaction);

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

static int
itd_urb_transaction (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  int      mem_flags
) {
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int      frame_index;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  struct ehci_itd    *first_itd, *itd;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  int      status;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dma_addr_t    itd_dma;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  for (frame_index = 0, first_itd = 0;
      frame_index < urb->number_of_packets;
      frame_index++) {
    itd = (struct ehci_itd*) pci_pool_alloc (ehci->itd_pool, mem_flags, &itd_dma);
    if (!itd) {
      status = -ENOMEM;
      goto fail;
    }
    memset (itd, 0, sizeof *itd);
    itd->itd_dma = itd_dma;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    status = itd_fill (ehci, itd, urb, frame_index,
        urb->transfer_dma);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/    
    if (status != 0)
      goto fail;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (first_itd)
      list_add_tail (&itd->itd_list,
          &first_itd->itd_list);
    else {
    	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      INIT_LIST_HEAD (&itd->itd_list);
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      urb->hcpriv = first_itd = itd;
    }
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  urb->error_count = 0;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return 0;

fail:
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (urb->hcpriv)
    itd_free_list (ehci, urb);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  return status;
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

static inline void
itd_link (struct ehci_hcd *ehci, U32 frame, struct ehci_itd *itd)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  itd->itd_next = ehci->pshadow [frame];
  itd->hw_next = ehci->periodic [frame];
  ehci->pshadow [frame].itd = itd;
  ehci->periodic [frame] = CPUToLE32 (itd->itd_dma) | Q_TYPE_ITD;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int get_iso_range (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  U32    *start,
  U32    *max,
  U32    mod
) {
  struct list_head  *lh;
  struct hcd_dev    *dev = (struct hcd_dev*) urb->dev->hcpriv;
  int      last = -1;
  U32    now, span, end;

  span = urb->interval * urb->number_of_packets;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  list_for_each (lh, &dev->urb_list) {
    struct urb  *u;
    struct ehci_itd  *itd;
    U32  s;

    u = list_entry (lh, struct urb, urb_list);
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (u == urb || u->pipe != urb->pipe)
      continue;
    if (u->interval != urb->interval) {  
      dbg ("urb %p interval %d ... != %p interval %d",
        u, u->interval, urb, urb->interval);
      return -EINVAL;
    }

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    itd = (struct ehci_itd*) urb->hcpriv;
    s = itd->uframe + u->interval * u->number_of_packets;
    if (last < 0)
      last = s;
    else {

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      dbg ("NYET: queue >2 URBs per ISO endpoint%s%s","","");
      return -EDOM;
    }
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  now = ehci_readl ((U32)&ehci->regs->frame_index) + 1;  
  if (!ehci->periodic_sched)
    now += 8;        
  now %= mod;
  end = now + mod;
  if (last < 0) {
    *start = now + ehci->i_thresh +  1;
    *max = end - span;
    if (*max < *start + 1)
      *max = *start + 1;
  } else {
    *start = last % mod;
    *max = (last + 1) % mod;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!(urb->transfer_flags & URB_ISO_ASAP)) {
    U32  temp;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->start_frame %= ehci->periodic_size;
    temp = urb->start_frame << 3;
    if (temp < *start)
      temp += mod;
    if (temp > *max)
      return -EDOM;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    *start = urb->start_frame << 3;
    temp += 8;
    if (temp < *max)
      *max = temp;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

  *max %= ehci->periodic_size;
  if ((*start + span) < end)
    return 0;
  return -EFBIG;
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

static int
itd_schedule (struct ehci_hcd *ehci, struct urb *urb)
{
  U32  start, max, i;
  int    status;
  U32  mod = ehci->periodic_size << 3;

  for (i = 0; i < (U32) urb->number_of_packets; i++) {
    urb->iso_frame_desc [i].status = (U32) -EINPROGRESS;
    urb->iso_frame_desc [i].actual_length = 0;
  }

  if ((status = get_iso_range (ehci, urb, &start, &max, mod)) != 0)
    return status;

  do {
    U32  uframe;
    U32  usecs;
    struct ehci_itd  *itd;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    itd = (struct ehci_itd*) urb->hcpriv;
    uframe = start;
    for (i = 0, uframe = start;
        i < (U32) urb->number_of_packets;
        i++, uframe += urb->interval) {
      uframe %= mod;

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (periodic_usecs (ehci, uframe >> 3, uframe & 0x7)
          > (100 - itd->usecs)) {
        itd = 0;
        break;
      }
      itd = list_entry (itd->itd_list.next,  struct ehci_itd, itd_list);
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    }
    if (!itd)
      continue;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    itd = (struct ehci_itd*) urb->hcpriv;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    urb->start_frame = start >> 3;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    vdbg ("[ISO] urb %p (%d packets period %d) starting %d.%d",
      urb, urb->number_of_packets, urb->interval,
      urb->start_frame, start & 0x7);
  
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    for (i = 0, uframe = start, usecs = 0;
        i < (U32) urb->number_of_packets;
        i++, uframe += urb->interval) {
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  	
      uframe %= mod;

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      itd->uframe = uframe;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      itd->hw_transaction [uframe & 0x07] = itd->transaction;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      itd_link (ehci, (uframe >> 3) % ehci->periodic_size,
        itd);
      wmb ();
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      usecs += itd->usecs;

      itd = list_entry (itd->itd_list.next, struct ehci_itd, itd_list);
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    }

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    usecs /= urb->number_of_packets;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    usecs /= urb->interval;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    usecs >>= 3;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (usecs < 1)
      usecs = 1;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
    usb_claim_bandwidth (urb->dev, urb, usecs, 1);

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (!ehci->periodic_sched++) {
      if ((status =  enable_periodic (ehci)) != 0) {        
        err ("[itd_schedule], enable = %d", status);
      }
    }
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    return 0;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    ++start;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    start  %=  mod;
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  } while ((start) != max);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dbg ("[urb] %p, CAN'T SCHEDULE", urb);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return -ENOSPC;
}

/*-------------------------------------------------------------------------*/

#define  ISO_ERRS (EHCI_ISOC_BUF_ERR | EHCI_ISOC_BABBLE | EHCI_ISOC_XACTERR)

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

static int itd_submit (struct ehci_hcd *ehci, struct urb *urb, int mem_flags)
{
  int    status;
  U32  flags;

  dbg ("itd_submit urb %p", urb);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  status = itd_urb_transaction (ehci, urb, mem_flags);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (status < 0)
    return status;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  spin_lock_irqsave (&ehci->lock, flags);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  status = itd_schedule (ehci, urb);
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  Chip_Flush_Memory();
    
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
  spin_unlock_irqrestore (&ehci->lock, flags);
  if (status < 0)
    itd_free_list (ehci, urb);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return status;
}
#endif

#ifdef have_split_iso

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

#endif

/*-------------------------------------------------------------------------*/

static void
scan_periodic (struct ehci_hcd *ehci, struct pt_regs *regs)
{
  U32  frame, clock, now_uframe, mod;
  U32  count = 0;

  mod = ehci->periodic_size << 3;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  frame = ehci->next_uframe >> 3;
  if (HCD_IS_RUNNING (ehci->hcd.state))
    now_uframe = ehci_readl ((U32)&ehci->regs->frame_index);
  else
    now_uframe = (frame << 3) - 1;
  now_uframe %= mod;
  clock = now_uframe >> 3;

  for (;;) {
    union ehci_shadow  q, *q_p;
    U32      type, *hw_p;
    U32    uframes;

restart:
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (frame == clock)
      uframes = now_uframe & 0x07;
    else
      uframes = 8;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
    q_p = &ehci->pshadow [frame];
    hw_p = &ehci->periodic [frame];
    q.ptr = q_p->ptr;
    type = Q_NEXT_TYPE (*hw_p);

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    while (q.ptr != 0) {
      int      last;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      union ehci_shadow  temp;

      switch (type) {
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/	
      case Q_TYPE_QH:
      	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        last = (q.qh->hw_next == EHCI_LIST_END);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        temp = q.qh->qh_next;
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        type = Q_NEXT_TYPE (q.qh->hw_next);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        count += intr_complete (ehci, frame,
            qh_get (q.qh), regs);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/     
        qh_put (ehci, q.qh);
        q = temp;
        break;
#ifdef have_fstn
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      case Q_TYPE_FSTN:
      	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        last = (q.fstn->hw_next == EHCI_LIST_END);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        if (q.fstn->hw_prev != EHCI_LIST_END) {
          dbg ("ignoring completions from FSTNs%s%s","","");
        }
        type = Q_NEXT_TYPE (q.fstn->hw_next);
        q = q.fstn->fstn_next;
        break;
#endif
#ifdef have_iso
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      case Q_TYPE_ITD:
      	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        last = (q.itd->hw_next == EHCI_LIST_END);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        for (uf = 0; uf < uframes; uf++) {
        	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
          if (q.itd->hw_transaction [uf] != 0) {
          	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
            temp = q;
            *q_p = q.itd->itd_next;
            *hw_p = q.itd->hw_next;
            type = Q_NEXT_TYPE (*hw_p);

            /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
            count += itd_complete (ehci,
              temp.itd, uf, regs);
            break;
          }
        }
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        if (uf == uframes) {
          q_p = &q.itd->itd_next;
          hw_p = &q.itd->hw_next;
          type = Q_NEXT_TYPE (q.itd->hw_next);
        }

        q = *q_p;
        break;
#endif
#ifdef have_split_iso
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      case Q_TYPE_SITD:
      	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        last = (q.sitd->hw_next == EHCI_LIST_END);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        sitd_complete (ehci, q.sitd);
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
        type = Q_NEXT_TYPE (q.sitd->hw_next);

        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
        q = q.sitd->sitd_next;
        break;
#endif 
      default:
      	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
        dbg ("corrupt type %d frame %d shadow %p",
          type, frame, q.ptr);
        // BUG ();
        last = 1;
        q.ptr = 0;
      }

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      if (unlikely (q.ptr == 0 && !last))
        goto restart;
    }

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (frame == clock) {
      U32  now;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/  
      if (!HCD_IS_RUNNING (ehci->hcd.state))
        break;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/   
      ehci->next_uframe = now_uframe;
      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
      now = ehci_readl ((U32)&ehci->regs->frame_index) % mod;
      if (now_uframe == now)
        break;

      /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
      now_uframe = now;
      clock = now_uframe >> 3;
    } else
      frame = (frame + 1) % ehci->periodic_size;
  }
}

