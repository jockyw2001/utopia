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
#include "include/drvPorts.h"
#include "drvEHCI.h"

#if 1

 void ehci_hcd_free (struct usb_hcd *hcd)
{
  kfree (hcd_to_ehci (hcd));
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 struct usb_hcd *ehci_hcd_alloc (void)
{

  struct ehci_hcd *ehci;

  ehci = (struct ehci_hcd *)
    kmalloc (sizeof (struct ehci_hcd), GFP_KERNEL);
  
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (ehci != 0) 
  {
    diag_printf("ehci: %lx\n", (U32) ehci);
    memset (ehci, 0, sizeof (struct ehci_hcd));
    //ehci->hcd.product_desc = "EHCI Host Controller";
    memcpy(ehci->hcd.product_desc, "EHCI Host Controller", sizeof(ehci->hcd.product_desc));
    return &ehci->hcd;
  }

  return 0;
}
static __inline__ void ehci_qtd_init (struct ehci_qtd *qtd, dma_addr_t dma);

/*-------------------------------------------------------------------------*/
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static struct ehci_qtd *ehci_qtd_alloc (struct ehci_hcd *ehci, int flags)
{
  struct ehci_qtd    *qtd;
  dma_addr_t    dma;

  qtd = (struct ehci_qtd*) pci_pool_alloc (ehci->qtd_pool, flags, &dma);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  if (qtd != 0) 
  {
    ehci_qtd_init (qtd, dma);
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return qtd;
}


static __inline__ void ehci_qtd_init (struct ehci_qtd *qtd, dma_addr_t dma)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  memset (qtd, 0, sizeof *qtd);
  qtd->qtd_dma = dma;
  qtd->hw_token = CPUToLE32 (QTD_STS_HALT);
  qtd->hw_next = EHCI_LIST_END;
  qtd->hw_alt_next = EHCI_LIST_END;
  INIT_LIST_HEAD (&qtd->qtd_list);
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static __inline__ void ehci_qtd_free (struct ehci_hcd *ehci, struct ehci_qtd *qtd)
{
  pci_pool_free (ehci->qtd_pool, qtd, qtd->qtd_dma);
}


static struct ehci_qh *ehci_qh_alloc (struct ehci_hcd *ehci, int flags)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  struct ehci_qh    *qh;
  dma_addr_t    dma;

  qh = (struct ehci_qh *)
    pci_pool_alloc (ehci->qh_pool, flags, &dma);
  if (!qh)
    return qh;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  memset (qh, 0, sizeof *qh);
  atomic_set (&qh->refcount, 1);
  qh->qh_dma = dma;
  // INIT_LIST_HEAD (&qh->qh_list);
  diag_printf("qh: %p, qh->qh_dma: %p\n", (MS_U32)qh, qh->qh_dma);
  INIT_LIST_HEAD (&qh->qtd_list);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  qh->dummy = ehci_qtd_alloc (ehci, flags);
  if (qh->dummy == 0) 
  {
    ehci_dbg (ehci, "no dummy td%s%s","\n","");
    pci_pool_free (ehci->qh_pool, qh, qh->qh_dma);
    qh = 0;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return qh;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static __inline__ struct ehci_qh *qh_get (/* ehci, */ struct ehci_qh *qh)
{
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  atomic_inc (&qh->refcount);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  return qh;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void qh_put (struct ehci_hcd *ehci, struct ehci_qh *qh)
{
  if (!atomic_dec_and_test (&qh->refcount))
    return;
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!list_empty (&qh->qtd_list) || qh->qh_next.ptr) 
  {
    ehci_dbg (ehci, "unused qh not empty%s%s","!","\n");
    BUG ();
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
  if (qh->dummy)
    ehci_qtd_free (ehci, qh->dummy);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  pci_pool_free (ehci->qh_pool, qh, qh->qh_dma);
}

/*-------------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static void ehci_mem_cleanup (struct ehci_hcd *ehci)
{
  if (ehci->async)
    qh_put (ehci, ehci->async);
  ehci->async = 0;

  /* PCI consistent memory and pools */
  if (ehci->qtd_pool)
    pci_pool_destroy (ehci->qtd_pool);
  ehci->qtd_pool = 0;

  if (ehci->qh_pool) {
    pci_pool_destroy (ehci->qh_pool);
    ehci->qh_pool = 0;
  }

  if (ehci->itd_pool)
    pci_pool_destroy (ehci->itd_pool);
  ehci->itd_pool = 0;

  if (ehci->sitd_pool)
    pci_pool_destroy (ehci->sitd_pool);
  ehci->sitd_pool = 0;

  if (ehci->periodic)
  {
    /*
    pci_free_consistent (ehci->hcd.pdev,
      ehci->periodic_size * sizeof (u32),
      ehci->periodic, ehci->periodic_dma);
      ehci->periodic = 0;
    */
    //free(ehci->periodic_rptr);
    hcd_buffer_free(&ehci->hcd.self, ehci->periodic_size * sizeof (void *), ehci->periodic, ehci->periodic_dma);
  }
  ehci->periodic = 0;
  ehci->periodic_dma = 0;

  /* shadow periodic table */
  if (ehci->pshadow)
    kfree (ehci->pshadow);
  ehci->pshadow = 0;
}

/*******************************************************************************
 * Function name: alloc_ehci_periodic
 *    returns    :
 * Created by    : Peter Liao
 * Date created : 2003/12/10
 * Description  : It will allocate memory for ohci hcca (32-byte boundary) from
 *                global memory.(32-byte boundary)
 * Notes        :
 ******************************************************************************/
/*
static struct u32 *alloc_ehci_periodic(struct ohci_hcca **rptr,unsigned long size)
{
  *rptr = (struct u32 *)malloc(size+32);
  if ( *rptr != NULL ) {
    return (struct u32 *)( (u32)((char *)(*rptr) + 31) & ~31 );
  }
  else {
    return NULL;
  }
}
*/


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
static int ehci_mem_init (struct ehci_hcd *ehci, int flags)
{
  
  U32 i;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  vdbg("Creat Page memory poof for qtd%s","\n");
  ehci->qtd_pool = pci_pool_create ("ehci_qtd", ehci->hcd.pdev,
      sizeof (struct ehci_qtd),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!ehci->qtd_pool) 
  {
    goto fail;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  vdbg("Creat Page memory poof for qh%s","\n");
  ehci->qh_pool = pci_pool_create ("ehci_qh", ehci->hcd.pdev,
      sizeof (struct ehci_qh),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!ehci->qh_pool) 
  {
    goto fail;
  }
  ehci->async = ehci_qh_alloc (ehci, flags);
  if (!ehci->async) 
  {
    goto fail;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  vdbg("Creat Page memory poof for itd%s","\n");
  ehci->itd_pool = pci_pool_create ("ehci_itd", ehci->hcd.pdev,
      sizeof (struct ehci_itd),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!ehci->itd_pool) 
  {
    goto fail;
  }
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  vdbg("Creat Page memory poof for sitd%s","\n");
  ehci->sitd_pool = pci_pool_create ("ehci_sitd", ehci->hcd.pdev,
      sizeof (struct ehci_sitd),
      32 /* byte alignment (for hw parts) */,
      4096 /* can't cross 4K */);
  if (!ehci->sitd_pool)
  {
    goto fail;
  }

  /* Hardware periodic table */
  /*
  ehci->periodic = (u32 *)
    pci_alloc_consistent (ehci->hcd.pdev,
      ehci->periodic_size * sizeof (u32),
      &ehci->periodic_dma);
  */
  /*
  ehci->periodic = alloc_ehci_periodic ( ehci->periodic_rptr, ehci->periodic_size * sizeof (u32));
  ehci->periodic_dma = ehci->periodic;
  */
  ehci->periodic = (U32*) hcd_buffer_alloc(&ehci->hcd.self,ehci->periodic_size * sizeof (void *),0,&ehci->periodic_dma);
  //ehci->periodic_dma = (dma_addr_t) ehci->periodic;	//Note_HC_Test
  vdbg("Allocate a non-cacheable memory poof for periodic frame list array %p\n",ehci->periodic);
  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (ehci->periodic == 0) 
  {
    goto fail;
  }
  for (i = 0; i < ehci->periodic_size; i++)
    ehci->periodic [i] = EHCI_LIST_END;

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  ehci->pshadow = (union ehci_shadow  *) kmalloc (ehci->periodic_size * sizeof (void *), flags);
  if (ehci->pshadow == 0) 
  {
    goto fail;
  }
  memset (ehci->pshadow, 0, ehci->periodic_size * sizeof (void *));
  return 0;

fail:
  ehci_dbg (ehci, "couldn't init memory%s","");
  ehci_mem_cleanup (ehci);
  return -ENOMEM;
}
#endif	//#ifdef

