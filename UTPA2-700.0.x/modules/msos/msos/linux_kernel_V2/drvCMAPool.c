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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvCMAPool.c
/// @brief  CMA Pool Driver
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------




//N.B. If have CMA_PATCH_POOL_UTOPIA_TO_KERNEL MP config,
//Code here can not recognize MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL
//but can recognize CONFIG_MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL
//#if (1 == MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL)
#ifdef CONFIG_MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL

//#include<sys/types.h>
//#include <sys/ioctl.h>
//#include <sys/mman.h>
//#include <pthread.h>
//#include<fcntl.h>
//#include <unistd.h>
//#include <string.h>

#include "MsCommon.h"
#include "halCHIP.h"
#include "halMPool.h"
#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include "../mstar2/include/mdrv_cma_pool_st.h"
#include "../mstar2/include/mdrv_cma_pool_io.h"

//#include "drvCMAPool.h"
#include "drvCMAPool_v2.h"
//#include "../mstar2/drv/cma_pool/mdrv_cma_pool.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define drvCMAPool_DEBUG_CODE_INDISE 1      //for debug use,  do not delete this code,[samson.huang]
//#define drvCMAPool_DEBUG_CODE_OUTDISE 1    //for debug use,  do not delete this code,[samson.huang]

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAX_CMAPOOLSIZE 10UL
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 pool_handle_id;
    MS_VIRT VirtStart;
    MS_U64 Physaddr;
    MS_SIZE CMAPoolSize;
    MS_BOOL bNonCache;
    MS_BOOL bIsUsed;
} CMAPOOL_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern struct file *             _fileCMAPool;

static struct mutex _CMA_POOL_Mutex = __MUTEX_INITIALIZER(_CMA_POOL_Mutex);

static CMAPOOL_INFO cpool_info[MAX_CMAPOOLSIZE];

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _findEmpty_CMA_Pool_Entry(MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_CMAPOOLSIZE; i++)
    {
        if(cpool_info[i].bIsUsed == FALSE)
        {
            find = TRUE;
            *index = i;
            break;
        }
    }

    if(find == FALSE)
        printk("Not enough CMAPool, must increase MAX_CMAPOOLSIZE!!\n");
    
    return find;
}


static MS_BOOL _findPoolHandleId_InCMA_Pool_Table(MS_U32 pool_handle_id, MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_CMAPOOLSIZE; i++)
    {
        if((cpool_info[i].bIsUsed == TRUE) && (cpool_info[i].pool_handle_id == pool_handle_id))
        {
            find = TRUE;
            *index = i;
            break;
        }
    }
    
    return find;
}

//#ifdef CONFIG_MP_NEW_UTOPIA_32BIT/* this is for 32-bit kernel and new_utopia(utopia2) */
#if defined(CONFIG_ARM64)

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// System initialzation
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Init(struct CMA_Pool_Init_Param *Init_Param)
{
    struct cma_heap_info heap_info;
    MS_U64 u64PhyAddr = 0, u64AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_BOOL bNonCache, ret = TRUE;
    MS_U32 idx = 0;
    char * ptr = NULL;	
    mm_segment_t old_fs;


    mutex_lock(&_CMA_POOL_Mutex);
    if ( !_fileCMAPool )
    {
       int flags=O_RDWR;
    	//if (force_o_largefile())
		//flags |= O_LARGEFILE;

        _fileCMAPool = filp_open( "/dev/cmapool", flags, 0);

        if (IS_ERR(_fileCMAPool) )
        {
            printk("open /dev/cmapool fail\n");
            ret =  FALSE;
            goto CMA_POOL_INIT_DONE;
        }
        memset(cpool_info, 0, sizeof(CMAPOOL_INFO)*MAX_CMAPOOLSIZE);
    }
    heap_info.heap_id = Init_Param->heap_id;
    heap_info.flags = Init_Param->flags;
    
    //for kernel utopia use.must set this flag
    heap_info.flags |= CMA_FLAG_MAP_KERNEL;
#ifdef drvCMAPool_DEBUG_CODE_INDISE
    printk("[%s:%d]heap_info heap_id:%x, flags:%llx \n",__FUNCTION__,__LINE__,Init_Param->heap_id,Init_Param->flags);
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if (file_ioctl(_fileCMAPool, CMA_POOL_IOC_INIT, (unsigned long)(&heap_info)))//
    {
        set_fs(old_fs);
        printk("init heap_id %tu fail\n", (ptrdiff_t)heap_info.heap_id);
        ret =  FALSE;
        goto CMA_POOL_INIT_DONE;
    }
    set_fs(old_fs);
    Init_Param->pool_handle_id = heap_info.heap_id;
    Init_Param->miu = heap_info.miu;											// the miu this heap located at
    Init_Param->heap_miu_start_offset = heap_info.heap_miu_start_offset;		// heap_start_offset(pa), with respect to the miu
    Init_Param->heap_length = heap_info.heap_length;
    Init_Param->virt_addr = heap_info.virt_addr;
#ifdef drvCMAPool_DEBUG_CODE_INDISE
    printk("[%s:%d]heap_info pool_handle_id:%u, miu:%u heap_miu_start_offset:%llu heap_length:0x%llx\n",__FUNCTION__,__LINE__,Init_Param->pool_handle_id,Init_Param->miu,Init_Param->heap_miu_start_offset,Init_Param->heap_length);
#endif
    //not need virtual address for this pool
    if(!(Init_Param->flags & CMA_FLAG_MAP_VMA))
    {
         ret = TRUE;
         goto CMA_POOL_INIT_DONE;
    }

    //avoid mmap more than one time
    ret = _findPoolHandleId_InCMA_Pool_Table(Init_Param->pool_handle_id, &idx);
    if(ret == TRUE)
    {
         printk("heap_id %tu already init!\n", (ptrdiff_t)Init_Param->pool_handle_id);
         goto CMA_POOL_INIT_DONE;
    }

    ret = _findEmpty_CMA_Pool_Entry(&idx);
    if(ret == FALSE)
    {
        printk("heap_id %tu init failed!\n", (ptrdiff_t)Init_Param->pool_handle_id);
        goto CMA_POOL_INIT_DONE;
    }

    ptr = Init_Param->virt_addr;
	if(ptr == (char*)(-1))
    {
		printf("pool id %tu, size %lu mmap failed\n", (ptrdiff_t)Init_Param->pool_handle_id, (unsigned long)Init_Param->heap_length);
		ret =  FALSE;
		goto CMA_POOL_INIT_DONE;
    }

    u64PhyAddr = HAL_MsOS_MPool_BA2PA(heap_info.bus_addr);   	// get pa
    _phy_to_miu_offset(u8MiuSel, u64AddrOffset, u64PhyAddr);   	// get miu

    if(Init_Param->flags & CMA_FLAG_CACHED)
        bNonCache = FALSE;
    else
        bNonCache = TRUE;
    ret = MsOS_MPool_Add_PA2VARange(u64PhyAddr, (MS_VIRT)ptr, Init_Param->heap_length, bNonCache);
    if(ret)
    {
        /* CMA Pool setting*/
        cpool_info[idx].pool_handle_id = Init_Param->pool_handle_id;
        cpool_info[idx].VirtStart = (MS_VIRT)ptr;
        cpool_info[idx].Physaddr = u64PhyAddr;
        cpool_info[idx].CMAPoolSize = Init_Param->heap_length;
        cpool_info[idx].bNonCache = bNonCache;
        cpool_info[idx].bIsUsed = TRUE;        
    }
CMA_POOL_INIT_DONE:
    mutex_unlock(&_CMA_POOL_Mutex); 
#ifdef drvCMAPool_DEBUG_CODE_INDISE
    if(true == ret)
        printk("[%s:%d]ret == true=%d\n",__FUNCTION__,__LINE__,ret);
    else if(false == ret)
        printk("[%s:%d]ret == false=%d\n",__FUNCTION__,__LINE__,ret);
    else
        printk("[%s:%d]ret =others=%d\n",__FUNCTION__,__LINE__,ret);
#endif
    return ret;
}



MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param *alloc_param)
{
    struct cma_alloc_args alloc_args;
    MS_BOOL ret = TRUE;
    mm_segment_t old_fs;

    mutex_lock(&_CMA_POOL_Mutex);  
    if (IS_ERR(_fileCMAPool) )
    {
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }

    alloc_args.heap_id = alloc_param->pool_handle_id;
    alloc_args.align = 0x1000ULL;		// 4096
    alloc_args.offset_in_heap = alloc_param->offset_in_pool;
    alloc_args.length = alloc_param->length;
    alloc_args.flags = alloc_param->flags;
    
    //for kernel utopia use,must set this flag,
    //in later kernel real file_ioctl,will check this flag.
    alloc_args.flags |= CMA_FLAG_MAP_KERNEL;
    
#ifdef drvCMAPool_DEBUG_CODE_INDISE
    printk("[%s:%d]alloc_param  pool_handle_id:%x  offset_in_pool:%llx length:%llx flags:%x\n",__FUNCTION__,__LINE__,alloc_param->pool_handle_id,alloc_param->offset_in_pool,alloc_param->length,alloc_param->flags);
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if (file_ioctl(_fileCMAPool, CMA_POOL_IOC_ALLOC, (unsigned long)(&alloc_args)))
    {  
        set_fs(old_fs);
        printk("cma pool get memory fail: heap id %tu, offset %llu, size %lu, flags %tX\n", (ptrdiff_t)alloc_args.heap_id,  
            (unsigned long long)alloc_args.offset_in_heap, (unsigned long)alloc_args.length, (ptrdiff_t)alloc_args.flags);
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }
    set_fs(old_fs);

    if(alloc_args.flags & CMA_FLAG_VIRT_ADDR)
    {
        alloc_param->virt_addr = alloc_args.cpu_addr;
		printk("\033[35mFunction = %s, Line = %d, alloc_param->virt_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)alloc_param->virt_addr);
	}

CMA_POOL_GETMEM_DONE:
	mutex_unlock(&_CMA_POOL_Mutex);
   
    return ret;  
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param)
{
    struct cma_free_args free_args;
    MS_BOOL ret = TRUE;
    mm_segment_t old_fs;

    mutex_lock(&_CMA_POOL_Mutex);
    if(IS_ERR(_fileCMAPool) )
    {
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }
         
    free_args.heap_id = free_param->pool_handle_id;
    free_args.offset_in_heap = free_param->offset_in_pool;
    free_args.length = free_param->length;
#ifdef drvCMAPool_DEBUG_CODE_INDISE
        printk("[%s:%d]free_param  pool_handle_id:%x  offset_in_pool:%llx length:%llx \n",__FUNCTION__,__LINE__,free_param->pool_handle_id,free_param->offset_in_pool,free_param->length);
#endif    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if(file_ioctl(_fileCMAPool, CMA_POOL_IOC_FREE, (unsigned long)(&free_args)))
    {  
        set_fs(old_fs);
        printk("cma pool put memory fail: heap id %tu, offset 0x%lX, size 0x%lX\n", (ptrdiff_t)free_args.heap_id,  
            (unsigned long)free_args.offset_in_heap, (unsigned long)free_args.length);
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }
    set_fs(old_fs);

CMA_POOL_PUTMEM_DONE:
    mutex_unlock(&_CMA_POOL_Mutex);
    return ret;    
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id)
{
    MS_U32  heap_id; 
    MS_BOOL ret = TRUE;
    MS_U32 idx = 0;
    mm_segment_t old_fs;

    heap_id = pool_handle_id;

    mutex_lock(&_CMA_POOL_Mutex);
    if (IS_ERR(_fileCMAPool) )
    {
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }
#ifdef drvCMAPool_DEBUG_CODE_INDISE
    printk("[%s:%d]free_param  pool_handle_id:%x  \n",__FUNCTION__,__LINE__,pool_handle_id);
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if (file_ioctl(_fileCMAPool, CMA_POOL_IOC_RELEASE, (unsigned long)(&heap_id)))
    {  
        set_fs(old_fs);
        printk("cma pool release fail: pool id %tu\n", (ptrdiff_t)pool_handle_id);            
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }
    set_fs(old_fs);

    ret = _findPoolHandleId_InCMA_Pool_Table(pool_handle_id, &idx);
    if(ret == TRUE)
    {
        MsOS_MPool_Remove_PA2VARange(cpool_info[idx].Physaddr, cpool_info[idx].VirtStart, 
                cpool_info[idx].CMAPoolSize, cpool_info[idx].bNonCache);              
        cpool_info[idx].bIsUsed = FALSE;
        cpool_info[idx].pool_handle_id = 0;         
    }
    ret = TRUE; //not find the pool hand id

CMA_POOL_RELEASE_DONE:
    mutex_unlock(&_CMA_POOL_Mutex);   
    return ret;  
}

#if 0//only for test use

#define MILLION (1*1024*1024)
#define PAGE (4096)

static int drvCMAPool_init(void)
{
#ifdef drvCMAPool_DEBUG_CODE_OUTDISE//test code
    struct CMA_Pool_Init_Param Init_Param;
    int ret;
    struct CMA_Pool_Alloc_Param alloc_param;  
    struct CMA_Pool_Alloc_Param alloc_param2; 
    MS_VIRT temp;
    MS_VIRT temp_bk;    
    int i=0;    
    unsigned int read_data ;
    struct CMA_Pool_Free_Param free_param;
    struct CMA_Pool_Free_Param free_param2;
    MS_U32 pool_handle_id;
#endif
    printk(KERN_ERR"drvCMAPool init !!!\n");
#ifdef drvCMAPool_DEBUG_CODE_OUTDISE//test code
    Init_Param.heap_id=30;
    Init_Param.flags = CMA_FLAG_MAP_VMA;
    printk("[%s:%d] will MApi_CMA_Pool_Init fheap_id=%u\n",__FUNCTION__,__LINE__,Init_Param.heap_id);
    ret =MApi_CMA_Pool_Init(&Init_Param);
    if(ret == false)
    {
        printk("[%s:%d] MApi_CMA_Pool_Init fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("MApi_CMA_Pool_Init OK ret=%d\n",ret);
    alloc_param.pool_handle_id = Init_Param.pool_handle_id;
    alloc_param.offset_in_pool = Init_Param.heap_length/4;
    alloc_param.length = Init_Param.heap_length-Init_Param.heap_length/4;
    alloc_param.flags = CMA_FLAG_VIRT_ADDR;    
    ret=MApi_CMA_Pool_GetMem(&alloc_param);
    if(ret == false)
    {
        printk("[%s:%d] MApi_CMA_Pool_GetMem fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("MApi_CMA_Pool_GetMem OK ret=%d\n",ret);  
    temp=alloc_param.virt_addr;
    temp_bk=alloc_param.virt_addr;
    printk("will write data to pool sizeof(MS_VIRT)=%d\n",sizeof(MS_VIRT));

    for(i = 0; i < 1*MILLION/PAGE && i< alloc_param.length/PAGE; ++i)
    {
        *((MS_VIRT *)temp) = 0x12345678;
        *((MS_VIRT *)(temp+sizeof(MS_VIRT))) = 0x11222222;
        *((MS_VIRT *)(temp+PAGE-sizeof(MS_VIRT))) = 0x11777777;
        temp += PAGE; 
    }

    printk("after write data to pool\n");

    printk("will read data to pool\n");
    
    for(i = 0; i < 1*MILLION/PAGE && i< alloc_param.length/PAGE&& i<32; ++i)
    {
        read_data = *((MS_VIRT *)temp_bk);//read from this virt addr
        printk("read data from pool 0x%x  ", read_data);
        read_data = *((MS_VIRT *)(temp_bk+sizeof(MS_VIRT)));//read from this virt addr
        printk("   0x%x    ", read_data);
         read_data = *((MS_VIRT *)(temp_bk+PAGE-sizeof(MS_VIRT)));//read from this virt addr
        printk("    0x%x   \n", read_data);  
    }











#if 0//alloc 2 and write ,read
    alloc_param2.pool_handle_id = Init_Param.pool_handle_id;
    alloc_param2.offset_in_pool = 0;
    alloc_param2.length = Init_Param.heap_length/4;
    alloc_param2.flags = CMA_FLAG_VIRT_ADDR;    
    ret=MApi_CMA_Pool_GetMem(&alloc_param2);
    if(ret == false)
    {
        printk("[%s:%d] 2 MApi_CMA_Pool_GetMem fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("2 MApi_CMA_Pool_GetMem OK ret=%d\n",ret);  
    temp=alloc_param2.virt_addr;
    temp_bk=alloc_param2.virt_addr;
    printk("will 2 write data to pool sizeof(MS_VIRT)=%d\n",sizeof(MS_VIRT));

    for(i = 0; i < 1*MILLION/PAGE && i< alloc_param2.length/PAGE; ++i)
    {
        *((MS_VIRT *)temp) = 0x12300078;
        *((MS_VIRT *)(temp+sizeof(MS_VIRT))) = 0x11333333;
        *((MS_VIRT *)(temp+PAGE-sizeof(MS_VIRT))) = 0x11666666;
        temp += PAGE; 
    }

    printk("after 2 write data to pool\n");

    printk("will 2 read data to pool\n");
    
    for(i = 0; i < 1*MILLION/PAGE && i< alloc_param2.length/PAGE&& i<32; ++i)
    {
        read_data = *((MS_VIRT *)temp_bk);//read from this virt addr
        printk("read data from pool 0x%x  ", read_data);
        read_data = *((MS_VIRT *)(temp_bk+sizeof(MS_VIRT)));//read from this virt addr
        printk("   0x%x    ", read_data);
         read_data = *((MS_VIRT *)(temp_bk+PAGE-sizeof(MS_VIRT)));//read from this virt addr
        printk("    0x%x   \n", read_data);  
    }

#endif
    

    free_param.pool_handle_id = alloc_param.pool_handle_id;
    free_param.offset_in_pool = alloc_param.offset_in_pool;
    free_param.length = alloc_param.length;
    printk("before MApi_CMA_Pool_PutMem\n");
    ret = MApi_CMA_Pool_PutMem(&free_param);
    printk("after MApi_CMA_Pool_PutMem\n");
    if(ret == false)
    {
        printk("[%s:%d] MApi_CMA_Pool_PutMem fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("MApi_CMA_Pool_PutMem OK ret=%d\n",ret);



//////////////////
#if 0
temp_bk=alloc_param.virt_addr;
printk("after free will read again will write data to pool sizeof(MS_VIRT)=%d\n",sizeof(MS_VIRT));



printk("after free will read again  will read data to pool\n");

for(i = 0; i < 1*MILLION/PAGE && i< alloc_param.length/PAGE&& i<32; ++i)
{
    read_data = *((MS_VIRT *)temp_bk);//read from this virt addr
    printk("read data from pool 0x%x  ", read_data);
    read_data = *((MS_VIRT *)(temp_bk+sizeof(MS_VIRT)));//read from this virt addr
    printk("   0x%x    ", read_data);
     read_data = *((MS_VIRT *)(temp_bk+PAGE-sizeof(MS_VIRT)));//read from this virt addr
    printk("    0x%x   \n", read_data);  
}
printk("after free will read again  end\n");
#endif
////////////////////


#if 0// free 2
    free_param2.pool_handle_id = alloc_param2.pool_handle_id;
    free_param2.offset_in_pool = alloc_param2.offset_in_pool;
    free_param2.length = alloc_param2.length;
    printk("before 2 MApi_CMA_Pool_PutMem\n");
    ret = MApi_CMA_Pool_PutMem(&free_param2);
    printk("after 2 MApi_CMA_Pool_PutMem\n");
    if(ret == false)
    {
        printk("[%s:%d] 2 MApi_CMA_Pool_PutMem fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("2 MApi_CMA_Pool_PutMem OK ret=%d\n",ret);
#endif

    
    pool_handle_id = free_param.pool_handle_id;
    ret = MApi_CMA_Pool_Release(pool_handle_id);
    if(ret == false)
    {
        printk("[%s:%d] MApi_CMA_Pool_Release fail ret=%d\n",__FUNCTION__,__LINE__,ret);
        return -1;//if test code fail ,return
    }
    else
        printk("MApi_CMA_Pool_Release OK ret=%d\n",ret);

#endif
    return 0;
}

static void drvCMAPool_exit(void)
{
    printk(KERN_ERR"drvCMAPool exit !!! \n");
}

module_init(drvCMAPool_init);
module_exit(drvCMAPool_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mstar");
MODULE_DESCRIPTION("A module used for drvCMAPool!");
#endif



#else


MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Init(struct CMA_Pool_Init_Param *Init_Param)
{
    printk("warring message not realize %s for 32-bit kernel \n",__FUNCTION__);
    return false;
}
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param *alloc_param)
{
    printk("warring message not realize %s for 32-bit kernel \n",__FUNCTION__);
    return false;    
}
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id)
{
    printk("warring message not realize %s for 32-bit kernel \n",__FUNCTION__);
    return false;    
}
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param)
{
    printk("warring message not realize %s for 32-bit kernel \n",__FUNCTION__);
    return false;    
}

#endif//end of #ifdef CONFIG_MP_NEW_UTOPIA_32BIT


#else
#ifdef CONFIG_MSTAR_CMAPOOL
#error "should not got to here ,please sync and enable kernel code about CONFIG_MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL"
#else
//though in kernel mode,but not use cma pool,so do not have CONFIG_MP_CMA_PATCH_POOL_UTOPIA_TO_KERNEL is OK.
#endif
#endif
