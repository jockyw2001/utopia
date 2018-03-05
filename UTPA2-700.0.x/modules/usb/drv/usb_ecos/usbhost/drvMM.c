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
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_arch.h>
#include "include/drvConfig.h"
#include "include/drvPorts.h"
#include "include/drvKernel.h"
#include "include/drvBitops.h"

#if 1
#define NOCACHE_MEMORY_SIZE   SZ_128K //SZ_64K, Kaiserin
//#define NOCACHE_MEMORY_REGION MEM_REGION_8M
//#define NOCACHE_REGION_NUM    3
//#define NOCACHE_MIN_ALIGN     32
//#define NCMEM_BUFFER_POOLS    4

extern int BaseOfFreeMemory;
extern int TopOfFreeMemory;
extern int BaseOfHeap;
extern int TopOfHeap;

#define NCM_DEBUG

#ifdef NCM_DEBUG
#define NCM_Dbg(x,...)		diag_printf(x,__VA_ARGS__)
#else
#define NCM_Dbg(x,...)
#endif

struct ncmem_pool_tag {  /* the pool */
  U32      base_addr;
  size_t      pages_per_pool;
  U32    *bitmap;
};
static struct ncmem_pool_tag  ncmem_pool;

U32 ncmem_addr;
U32 ncmem_base;

/*******************************************************************************
 * Routine name : ncmem_free_page
 *    returns    : none
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : Create a non-cacheable and non-bufferable page memory pool via
 *                CPU memoryt region declaration API. Page momery pool is splited
 *                by the size of page memory. Each page memory is aligned to
 *                page_size bytes.
 * Notes        :
 *******************************************************************************/
void init_cache_memory(void)
{
	int mapsize;


	 ncmem_addr=(U32)KSEG02KSEG1(Usb_AllocateNonCachedMemory(NOCACHE_MEMORY_SIZE + PAGE_SIZE));

	USB_ASSERT(ncmem_addr, "Allocate ncmem buffer fail\n");
	ncmem_base = (ncmem_addr + PAGE_SIZE)&(~(PAGE_SIZE-1));
	NCM_Dbg("The base address is 0x%8lX\n", ncmem_base);

	// Initialize non-cached memory pool information
	ncmem_pool.pages_per_pool = (NOCACHE_MEMORY_SIZE/PAGE_SIZE);
	mapsize = ncmem_pool.pages_per_pool;
	mapsize = (mapsize + BITS_PER_LONG - 1) / BITS_PER_LONG;
	mapsize *= sizeof (U32);
	ncmem_pool.base_addr =(U32) KSEG02KSEG1(ncmem_base);	// convert cached address to uncached
//	ncmem_pool.bitmap = (U32 *)LIB_MemoryAllocate(mapsize, MALLOC_CACHED_ADDR);
       ncmem_pool.bitmap = (U32*)Usb_AllocateNonCachedMemory(mapsize);
	memset(ncmem_pool.bitmap, ((U32)-1), mapsize);//Bitmap: 0-used 1-unused

	NCM_Dbg("Base addr is 0x%08lX\n",ncmem_pool.base_addr);
	NCM_Dbg("Top addr is 0x%08lX\n",ncmem_pool.base_addr+NOCACHE_MEMORY_SIZE-1);
	NCM_Dbg("Pages per pool is %d\n",ncmem_pool.pages_per_pool);
	NCM_Dbg("Bit map base addr is %p\n",ncmem_pool.bitmap);
}

/*******************************************************************************
 * Routine name : ncmem_alloc_page
 *    returns    : 0 - pointer to allocated virtual address
 *    dma_addr  : pointer to allocated physical address
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : It will allocate a new non-cacheable and non-bufferable memory
 *                (size = page_size) from non-cacheable page memory pool for callee
 *                PAGE_SIZE is defined in pci_mem.h file
 * Notes        :
 *******************************************************************************/
void *ncmem_alloc_page(dma_addr_t *dma_addr)
{
	U32    i;
	int     map, page;
	size_t  offset;
	void    *retval;
	U32 flags;

	spin_lock_irqsave (&pool->lock, flags);
	/* only cachable accesses here ... */
	for (map = 0, i = 0;
		i < ncmem_pool.pages_per_pool;
		i += BITS_PER_LONG, map++)
	{
		if (ncmem_pool.bitmap[map] == 0)
			continue;
		page = ffz (~ ncmem_pool.bitmap [map]);
		if ((i + page) < ncmem_pool.pages_per_pool) {
			clear_bit ( page, &ncmem_pool.bitmap [map],U32);
			offset = (BITS_PER_LONG * map) + page;
			offset *= PAGE_SIZE;
			goto ready;
		}
	}

	NCM_Dbg("ERROR: No enough non-cached memory space !!%s\n","");
	retval = 0;
	goto done;

ready:
	retval = (void*) (offset + (size_t) (ncmem_pool.base_addr));
	*dma_addr = (dma_addr_t)USB_VA2PA((U32)retval);	// Convert to physical address for DMA
done:
	spin_unlock_irqrestore (&pool->lock, flags);
	NCM_Dbg("The allocated addr is %p, bit_map[%d] is 0x%08lX\n",retval,map,(U32)(ncmem_pool.bitmap[map]));
	return retval;
}

/*******************************************************************************
 * Routine name : ncmem_free_page
 *    returns    : none
 *    addr      : allocated virtual base address
 * Created by    : Peter Liao
 * Date created : 2004/01/25
 * Description  : It will de-allocate/release the allocated non-cacheable memory
 *                page to non-cacheable page memory pool.
 * Notes        :
 *******************************************************************************/
void ncmem_free_page(U32 addr)
{
	U32 flags;
	int map, page;

	page = addr - ncmem_pool.base_addr;
	page /= PAGE_SIZE;
	map = page / BITS_PER_LONG;
	page %= BITS_PER_LONG;
	spin_lock_irqsave (NULL, flags);
	set_bit (page, &(ncmem_pool.bitmap[map]), U32);
	NCM_Dbg("Free Block: bitmap[%d] is 0x%08lX",map,(U32)ncmem_pool.bitmap[map]);
	spin_unlock_irqrestore (&pool->lock, flags);
}
/*******************************************************************************
 * Routine name : ncmem_alloc
 *    returns    : 0 - pointer to allocated virtual address
 *    dma_addr  : pointer to allocated physical address
 * Created by    : Peter Liao
 * Date created : 2004/02/10
 * Description  : It will allocate a new non-cacheable and non-bufferable memory
 *                (size = user specific) from non-cacheable page memory pool for callee
 *                Therefore, it may waste up memory space if the size is not mutiply
 *                of page size. Callee should call this function before calling
 *                ncmem_alloc_page because of continuous issue.
 * Notes        :
 *******************************************************************************/
void *ncmem_alloc(dma_addr_t *dma_addr, U32 size)
{
	U32 flags;
	U32 i;
	U32 map, page;
	U32 num_of_pages;
	size_t  offset;
	void    *retval;

	//If the size is not multiply of PAGE_SIZE, just allocate a PAGE for remaining space
	if ( size%PAGE_SIZE != 0)
		size = size/PAGE_SIZE +1;
	else
		size = size/PAGE_SIZE;
	num_of_pages = size/PAGE_SIZE;
	spin_lock_irqsave (&pool->lock, flags);
	/* only cachable accesses here ... */
	for (map = 0, i = 0;
		i < ncmem_pool.pages_per_pool;
		i += BITS_PER_LONG, map++)
	{
		if (ncmem_pool.bitmap[map] == 0)
			continue;
		page = ffz (~ ncmem_pool.bitmap [map]);
		if ((i + page) < ncmem_pool.pages_per_pool) {
			offset = (BITS_PER_LONG * map) + page;
			offset *= PAGE_SIZE;
			goto ready;
		}
	}
	NCM_Dbg("ERROR: No enough non-cached memory space !!%s\n","");
	retval = 0;
	goto done;

ready:
	//Allocate enough pages for required memory space
	for(i=0;(i < num_of_pages);i++)
	{
		if ( page >= BITS_PER_LONG )
		{
		page = 0;
		map++;
		}
		clear_bit ( page++, &ncmem_pool.bitmap [map],U32);
	}
	retval = (void*) (offset + (size_t) (ncmem_pool.base_addr));
	*dma_addr = (dma_addr_t)USB_VA2PA((U32)retval);
done:
	spin_unlock_irqrestore (&pool->lock, flags);
	NCM_Dbg("The allocated addr = %p, size = %ld pages = %ld, end of bitmap[%ld]=0x%08lX",retval ,size, num_of_pages, map,(U32)ncmem_pool.bitmap[map]);
	return retval;
}

/*******************************************************************************
 * Routine name : ncmem_free
 *    returns    : none
 *    addr      : allocated virtual base address
 *    size      : allocated size
 * Created by    : Peter Liao
 * Date created : 2004/02/10
 * Description  : It will de-allocate/release the allocated non-cacheable memory
 *                space groupged by pages.
 * Notes        :
 *******************************************************************************/
void ncmem_free(U32 addr,U32 size)
{
	U32 flags;
	int i;
	int map, page, num_of_pages;

	if ( size%PAGE_SIZE != 0)
		size = size/PAGE_SIZE +1;
	else
		size = size/PAGE_SIZE;
	num_of_pages = size/PAGE_SIZE;

	page = ncmem_pool.base_addr - addr;
	page /= PAGE_SIZE;
	map = page / BITS_PER_LONG;
	page %= BITS_PER_LONG;
	spin_lock_irqsave (&pool->lock, flags);
	for(i=0;(i < num_of_pages);i++)
	{
		if ( page >= BITS_PER_LONG )
		{
			page = 0;
			map++;
		}
		set_bit (page, &(ncmem_pool.bitmap[map]), U32);
	}
	NCM_Dbg("Free non-cacheable memory : size = %ld = %d pages, end of bitmap[%d]=0x%08lX",size, num_of_pages, map,(U32)ncmem_pool.bitmap[map]);
	spin_unlock_irqrestore (&pool->lock, flags);
}

void Destory_NC_mem(void)
{
#if 0
	LIB_MemoryFree((void*) ncmem_base);
#else
	//MsOS_FreeMemory((void*)CYGARC_CACHED_ADDRESS( ncmem_addr),gs32NonCachedPoolID_MIU0 );
	Usb_FreeNonCachedMemory((void*)CYGARC_CACHED_ADDRESS( ncmem_addr));
#endif
}

extern void MsOS_FlushMemory(void);
void Chip_Flush_Memory(void)
{
    MsOS_FlushMemory();
}

#endif	//#ifdef

