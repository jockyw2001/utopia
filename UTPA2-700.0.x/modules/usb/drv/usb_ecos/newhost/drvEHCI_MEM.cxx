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

#include "include/drvPorts.h"
#include "drvEHCI.h"

static __inline__ void ms_ehci_qtd_init (struct ehci_qtd *pQtd, dma_addr_t tDmaAddr);

/*-------------------------------------------------------------------------*/
static struct ehci_qtd *ms_ehci_qtd_alloc (struct ehci_hcd *pEhci, int iFlags)
{
	struct ehci_qtd    *pQtd;
	dma_addr_t    tDmaAddr;

	pQtd = (struct ehci_qtd*) ms_mem_pool_alloc (pEhci->pQtdPool, iFlags, &tDmaAddr);
	if (pQtd != 0)
	{
		ms_ehci_qtd_init (pQtd, tDmaAddr);
	}

	return pQtd;
}

static __inline__ void ms_ehci_qtd_init (struct ehci_qtd *pQtd, dma_addr_t tDmaAddr)
{
	memset (pQtd, 0, sizeof *pQtd);
	pQtd->qtd_dma_addr = tDmaAddr;
	pQtd->hw_token = QTD_STS_HALT;
	pQtd->hw_next_qtd = EHCI_LIST_END;
	pQtd->hw_alt_next_qtd = EHCI_LIST_END;
	ms_list_init (&pQtd->qtd_list);
}

static __inline__ void ms_ehci_qtd_free (struct ehci_hcd *pEhci, struct ehci_qtd *pQtd)
{
	ms_mem_pool_free (pEhci->pQtdPool, pQtd, pQtd->qtd_dma_addr);
}


static struct ehci_qh *ms_ehci_qh_alloc (struct ehci_hcd *pEhci, int iFlags)
{
	struct ehci_qh    *pQh;
	dma_addr_t    tDmaAddr;

	pQh = (struct ehci_qh *)ms_mem_pool_alloc (pEhci->pQhPool, iFlags, &tDmaAddr);
	if (!pQh)
		return pQh;

	memset (pQh, 0, sizeof *pQh);
	osapi_atomic_set (&pQh->tRefCnt, 1);
	pQh->qh_dma_addr = tDmaAddr;
	// INIT_LIST_HEAD (&pQh->qh_list);
	diag_printf("pQh: %p, pQh->qh_dma_addr: %p\n", (void *)pQh, (void *)pQh->qh_dma_addr);
	ms_list_init (&pQh->qtd_list);

	pQh->pDummyQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
	if (pQh->pDummyQtd == 0)
	{
		diag_printf ("no dummy td\n");
		ms_mem_pool_free (pEhci->pQhPool, pQh, pQh->qh_dma_addr);
        // TODO: free qH?
		pQh = 0;
	}

	return pQh;
}

static __inline__ struct ehci_qh *ms_qh_get (struct ehci_qh *pQh)
{
	osapi_atomic_inc (&pQh->tRefCnt);
	return pQh;
}

static void ms_qh_put (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
	if (!osapi_atomic_dec_and_test (&pQh->tRefCnt))
		return;

	if (!ms_is_empty_list (&pQh->qtd_list) || pQh->qh_next.ptr)
	{
		USB_ASSERT (0, "unused qh not empty!\n");
	}

	if (pQh->pDummyQtd)
		ms_ehci_qtd_free (pEhci, pQh->pDummyQtd);
	ms_mem_pool_free (pEhci->pQhPool, pQh, pQh->qh_dma_addr);
}

/*-------------------------------------------------------------------------*/

static void ms_ehci_mem_cleanup (struct ehci_hcd *pEhci)
{
	if (pEhci->stAsync)
		ms_qh_put (pEhci, pEhci->stAsync);
	pEhci->stAsync = 0;

	/* PCI consistent memory and pools */
	if (pEhci->pQtdPool)
		ms_mem_pool_destroy (pEhci->pQtdPool);
	pEhci->pQtdPool = 0;

	if (pEhci->pQhPool)
	{
		ms_mem_pool_destroy (pEhci->pQhPool);
		pEhci->pQhPool = 0;
	}

	if (pEhci->pPeriodic)
	{
		pEhci->u32PeriodicSize = DEFAULT_I_TDPS; // to match with allocated size
		ms_debug_debug("Free periodic frame list array %p, size %d\n",pEhci->pPeriodic, pEhci->u32PeriodicSize * sizeof (void *));
		ms_hcd_buffer_free(&pEhci->hcd.self, pEhci->u32PeriodicSize * sizeof (void *), pEhci->pPeriodic, pEhci->tPeriodicDma);
	}
	pEhci->pPeriodic = 0;
	pEhci->tPeriodicDma = 0;

	/* shadow periodic table */
	if (pEhci->pshadow)
		kfree (pEhci->pshadow);
	pEhci->pshadow = 0;
}

static int ms_ehci_mem_init (struct ehci_hcd *pEhci, int iFlags)
{
	U32 i;

	ms_debug_func("Creat Page memory pool for qtd\n");
	pEhci->pQtdPool = ms_mem_pool_create ("ehci_qtd", sizeof (struct ehci_qtd),
#if (_USB_128_ALIGMENT)
		128 /* byte alignment (for hw parts) */,
#else
		32 /* byte alignment (for hw parts) */,
#endif
		4096 /* can't cross 4K */);
	if (!pEhci->pQtdPool)
	{
		goto fail;
	}

	ms_debug_func("Creat Page memory pool for qh\n");
	pEhci->pQhPool = ms_mem_pool_create ("ehci_qh", sizeof (struct ehci_qh),
#if (_USB_128_ALIGMENT)
		128 /* byte alignment (for hw parts) */,
#else
		32 /* byte alignment (for hw parts) */,
#endif
		4096 /* can't cross 4K */);
	if (!pEhci->pQhPool)
	{
		goto fail;
	}
	pEhci->stAsync = ms_ehci_qh_alloc (pEhci, iFlags);
	if (!pEhci->stAsync)
	{
		goto fail;
	}

	/* Hardware periodic table */
	pEhci->pPeriodic = (U32*) ms_hcd_buffer_alloc(&pEhci->hcd.self,pEhci->u32PeriodicSize * sizeof (void *),0,&pEhci->tPeriodicDma);
	//ehci->periodic_dma = (dma_addr_t) ehci->periodic;	//Note_HC_Test
	ms_debug_msg("Allocate a non-cacheable memory pool for periodic frame list array %p, size %d\n",pEhci->pPeriodic, pEhci->u32PeriodicSize * sizeof (void *));
	if (pEhci->pPeriodic == 0)
	{
		goto fail;
	}
	for (i = 0; i < pEhci->u32PeriodicSize; i++)
		pEhci->pPeriodic [i] = EHCI_LIST_END;

	pEhci->pshadow = (union ehci_qh_shadow  *) kmalloc (pEhci->u32PeriodicSize * sizeof (void *), iFlags);
	if (pEhci->pshadow == 0)
	{
		goto fail;
	}
	memset (pEhci->pshadow, 0, pEhci->u32PeriodicSize * sizeof (void *));
	return 0;

fail:
	ms_debug_err("couldn't init memory\n");
	ms_ehci_mem_cleanup (pEhci);
	return -ENOMEM;
}

