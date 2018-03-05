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

//#include <MsCommon.h> // NUSED
//#include "include/drvPCIMEM.h" // NUSED
#include "include/drvKernel.h"
#include "include/drvBitops.h"
#include "drvUsbHostConfig.h"

/**
     * @brief               Return a memory pool to be used to allocate memory by the requested characteristics.
     *
     * @param pName          name of memory pool
     *
     * @param u32Size         block size in the pool.
     *
     * @param u32AlignLen   block alignment size.
     *
     * @param U32CrossLimit  returned blocks can't cross the boundray.
     *
     * @return  ms_mem_pool     return memory pool with the requested characteristics.
     */
struct ms_mem_pool *ms_mem_pool_create (const char *pName, MS_U32 u32Size, MS_U32 u32AlignLen, MS_U32 U32CrossLimit)
{
    struct ms_mem_pool    *pPool;

    if (u32AlignLen == 0)
        u32AlignLen = 1;
    if (u32Size == 0)
        return 0;
    else if (u32Size < u32AlignLen)
    u32Size = u32AlignLen;
    else if ((u32Size % u32AlignLen) != 0)
    {
        u32Size += u32AlignLen + 1;
        u32Size &= ~(u32AlignLen - 1);
    }

    if (U32CrossLimit == 0)
    {
        if (PAGE_SIZE < u32Size)
            U32CrossLimit = u32Size;
        else
            U32CrossLimit = PAGE_SIZE;
    }
    else if (U32CrossLimit < u32Size)
        return 0;

    if (!(pPool = (struct ms_mem_pool*) kmalloc (sizeof (*pPool), SLAB_KERNEL)))
        return pPool;

    strncpy (pPool->name, pName, sizeof (pPool->name)-1 );
    pPool->name[sizeof(pPool->name)-1] = '\0';
    ms_list_init (&pPool->page_list);
    pPool->size = u32Size;
    pPool->allocation = U32CrossLimit;
    pPool->blocks_per_page = U32CrossLimit / u32Size;
    ms_usbhost_debug("ms_mem_pool_create: size=%d,allocation=%d bytes, %d blocks per page",
        pPool->size, pPool->allocation, pPool->blocks_per_page);
    return pPool;
}

static struct ms_mem_page *
pool_alloc_page (struct ms_mem_pool *pMem_pool, int iFlags)
{
    struct ms_mem_page  *pPage;
    int    mapsize;

    mapsize = pMem_pool->blocks_per_page;
    mapsize = (mapsize + BITS_PER_LONG - 1) / BITS_PER_LONG;
    mapsize *= sizeof (U32);

    pPage = (struct ms_mem_page *) kmalloc (mapsize + sizeof(struct ms_mem_page), iFlags);
    ms_list_init(&pPage->page_list);
    if (!pPage)
        return 0;

    pPage->vaddr = ncmem_alloc_page(&pPage->dma);

    if (pPage->vaddr)
    {
        memset (pPage->bitmap, ((U32)-1), mapsize);  // bit set == free
        ms_insert_list_after (&pPage->page_list, &pMem_pool->page_list);
    }
    else
    {
        kfree (pPage);
        pPage = 0;
    }
    return pPage;
}

/**
     * @brief                       Allocate a block of memory
     *
     * @param pMem_pool    The pool to allocate memory.
     *
     * @param iFlags            memory flag.
     *
     * @param pDma_addr     pointer to physical address of block
     *
     * @return                      pointer to virtual address of block
     */
void *ms_mem_pool_alloc (struct ms_mem_pool *pMem_pool, int iFlags, dma_addr_t *pDma_addr)
{
    U32    u32flag;
    struct list_head  *entry;
    struct ms_mem_page    *ms_page;
    int      iMap, iBlock;
    U32      u32Offset;
    void     *ms_retval;

    osapi_spin_lock_irqsave (&pMem_pool->lock, u32flag);
    list_for_loop (entry, &pMem_pool->page_list)
    {
        U32    i;

        const struct list_head *__mptr = entry;
        ms_page = (struct ms_mem_page *)( (char *)__mptr - (char *)offsetof(struct ms_mem_page,page_list));

        /* only cachable accesses here ... */
        for (iMap = 0, i = 0; i < pMem_pool->blocks_per_page; i += BITS_PER_LONG, iMap++)
        {
            if (ms_page->bitmap [iMap] == 0)
                continue;
            iBlock = ms_find_1st_zero (~ ms_page->bitmap [iMap]);
            if ((i + iBlock) < pMem_pool->blocks_per_page)
            {
                ms_clear_bit ( iBlock, &ms_page->bitmap [iMap],U32);
                u32Offset = (BITS_PER_LONG * iMap) + iBlock;
                u32Offset *= pMem_pool->size;
                goto ready;
            }
        }
    }

    if (!(ms_page = pool_alloc_page (pMem_pool, iFlags)))
    {
        diag_printf("pool allocate page fail\n");
        ms_retval = 0;
        goto done;
    }

    ms_clear_bit (0, &ms_page->bitmap [0],U32);
    u32Offset = 0;
ready:
    ms_retval = (void*) (u32Offset + (size_t) ( ms_page->vaddr ));
    *pDma_addr = u32Offset + ms_page->dma;
done:
    osapi_spin_unlock_irqrestore (&pMem_pool->lock, u32flag);
    //ms_usbhost_debug("The allocated addr is %p, block size is %d ,bit_map[%d] is 0x%08X",
    //    ms_retval, pMem_pool->size ,iMap,(U32)ms_page->bitmap [iMap]);
    return ms_retval;
}

static __inline__ int __is_page_busy (int ms_blocks, U32 *ms_bitmap)
{
    while (ms_blocks > 0)
    {
        if (*ms_bitmap++ != ~0UL)
            return 1;
        ms_blocks -= BITS_PER_LONG;
    }
    return 0;
}

/**
     * @brief                       Destory memory pool
     *
     * @param pMem_pool    The pool to be destoryed.
     *
     * @return                     None
     */
void ms_mem_pool_destroy (struct ms_mem_pool *pMem_pool)
{
    U32    u32flag;

    osapi_spin_lock_irqsave (&pMem_pool->lock, u32flag);
    while (!ms_is_empty_list (&pMem_pool->page_list))
    {
        struct ms_mem_page    *ms_page;

        const struct list_head *__mptr = pMem_pool->page_list.next;
        ms_page = (struct ms_mem_page *)( (char *)__mptr - (char *)offsetof(struct ms_mem_page,page_list) );

        if (__is_page_busy (pMem_pool->blocks_per_page, ms_page->bitmap))
        {
            diag_printf ("ms_mem_pool_destroy %s, %p busy\n",
                pMem_pool->name, ms_page->vaddr);
            /* leak the still-in-use consistent memory */
            ms_list_remove (&ms_page->page_list);
            kfree (ms_page);
        }
        else
        {
			ncmem_free_page((U32)ms_page->vaddr);
			ms_list_remove (&ms_page->page_list);
			kfree (ms_page);
        }

    }
    osapi_spin_unlock_irqrestore (&pMem_pool->lock, u32flag);
    kfree (pMem_pool);
}

/**
     * @brief               Free memory block and put back into pool
     *
     * @param pMem_pool    The pool contains the block.
     *
     * @param pVirt_addr     Virtual address of the blcok.
     *
     * @param dma_addr      Physical address of the block.
     *
     * @return  BOOL           success or not
     */
MS_BOOL ms_mem_pool_free (struct ms_mem_pool *pMem_pool, void *pVirt_addr, dma_addr_t dma_addr)
{
    struct ms_mem_page   *ms_page;
    struct list_head  *pEntry;
    U32      u32flag;
    int      iMap, iBlock;

    // find page in memory pool
	osapi_spin_lock_irqsave (&pMem_pool->lock, u32flag);
	list_for_loop (pEntry, &pMem_pool->page_list)
	{
		//page = list_entry (entry, struct ms_mem_page, page_list);
		const struct list_head *__mptr = pEntry;
		ms_page = (struct ms_mem_page *)( (char *)__mptr - (char *)offsetof(struct ms_mem_page,page_list) );

		if (dma_addr < ms_page->dma)
			continue;
		if (dma_addr < (ms_page->dma + pMem_pool->allocation))
			goto page_found;
	}

	//Not found !!
	osapi_spin_unlock_irqrestore (&pMem_pool->lock, u32flag);
    ms_usbhost_debug ("ms_mem_pool_free %s, %p/%lx (bad dma)\n", pMem_pool->name, pVirt_addr, (U32) dma_addr);
    return FALSE;


page_found:
	osapi_spin_unlock_irqrestore (&pMem_pool->lock, u32flag);

    iBlock = dma_addr - ms_page->dma;
    iBlock /= pMem_pool->size;
    iMap = iBlock / BITS_PER_LONG;
    iBlock %= BITS_PER_LONG;

    osapi_spin_lock_irqsave (&pMem_pool->lock, u32flag);
    ms_set_bit (iBlock, &ms_page->bitmap [iMap], U32);
    ms_usbhost_debug("Free Block: addr=0x%08X bitmap is 0x%08X",dma_addr,(U32)ms_page->bitmap [iMap]);

    ////   if (!is_page_busy(bpp, page->bitmap)) pool_free_page(pool, page);
    ////   it is not interrupt safe. Better have empty pages hang around.

    osapi_spin_unlock_irqrestore (&pMem_pool->lock, u32flag);
    return TRUE;
}

