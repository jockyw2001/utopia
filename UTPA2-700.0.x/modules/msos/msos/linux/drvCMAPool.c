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
#if defined (MSOS_TYPE_LINUX)
#include<sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>
#include<fcntl.h>
#include <unistd.h>
#include <string.h>

#include "MsCommon.h"
#include "halCHIP.h"
#include "halMPool.h"
#include "mdrv_cma_pool_io.h"
#include "mdrv_cma_pool_st.h"
#include "mdrv_cma_pool_private.h"

#include "drvCMAPool.h"

#ifndef ANDROID
#define VPRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>
#define VPRINTF ALOGD
#endif
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
static MS_S32              _s32FdCMAPool = -1;
static MS_S32 _s32FdCMAPool_pid = -1;
static pthread_mutex_t  _CMA_POOL_Mutex = PTHREAD_MUTEX_INITIALIZER;
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
        VPRINTF("Not enough CMAPool, must increase MAX_CMAPOOLSIZE!!\n");
    
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
    VPRINTF("[user mode %s:%d]debug    Init_Param->heap_id=%u\n",__FUNCTION__,__LINE__,Init_Param->heap_id);

	memset(&heap_info, 0, sizeof(struct cma_heap_info));

    pthread_mutex_lock(&_CMA_POOL_Mutex); 
    if(_s32FdCMAPool > 0 && _s32FdCMAPool_pid != getpid())
    {
        close(_s32FdCMAPool);//need close and later open again.
        _s32FdCMAPool = -1;
        _s32FdCMAPool_pid = -1;
    }
    if (_s32FdCMAPool <= 0)
    {
        if ((_s32FdCMAPool = open("/dev/cmapool", O_RDWR)) < 0)
        {
            VPRINTF("Open /dev/cmapool fail\n");
            ret =  FALSE;
            goto CMA_POOL_INIT_DONE;
        }

        _s32FdCMAPool_pid = getpid();
        memset(cpool_info, 0, sizeof(CMAPOOL_INFO)*MAX_CMAPOOLSIZE);
    }
    
    heap_info.heap_id = Init_Param->heap_id;
    heap_info.flags = Init_Param->flags;
    
    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_INIT, &heap_info))
    {
        VPRINTF("init heap_id %tu fail\n", (ptrdiff_t)heap_info.heap_id);
        ret =  FALSE;
        goto CMA_POOL_INIT_DONE;
    }

    Init_Param->pool_handle_id = heap_info.heap_id;
    Init_Param->miu = heap_info.miu;											// the miu this heap located at
    Init_Param->heap_miu_start_offset = heap_info.heap_miu_start_offset;		// heap_start_offset(pa), with respect to the miu
    Init_Param->heap_length = heap_info.heap_length;

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
         VPRINTF("heap_id %tu already init!\n", (ptrdiff_t)Init_Param->pool_handle_id);
         goto CMA_POOL_INIT_DONE;
    }

    ret = _findEmpty_CMA_Pool_Entry(&idx);
    if(ret == FALSE)
    {
        VPRINTF("heap_id %tu init failed!\n", (ptrdiff_t)Init_Param->pool_handle_id);
        goto CMA_POOL_INIT_DONE;
    }

	/* we reserve all heap_length in va at beginning(keep vma, if CMA_FLAG_MAP_VMA)!!
	 * really mapping will be done while MApi_CMA_Pool_GetMem
	 * also, we will defined this reserved va as cached/noncached here
	 * and then, you can see the reserved vma @ /proc/pid/maps
	 */
    ptr = mmap(NULL, Init_Param->heap_length, PROT_READ|PROT_WRITE, MAP_SHARED, _s32FdCMAPool, 0);
    
	if(ptr == (char*)(-1))
    {
		VPRINTF("pool id %tu, size %lu mmap failed\n", (ptrdiff_t)Init_Param->pool_handle_id, (unsigned long)Init_Param->heap_length);
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
    else
    {
        VPRINTF("[%s:%d] ret==FALSE   ret=%d\n",__FUNCTION__,__LINE__,ret);
    }

CMA_POOL_INIT_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);  
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param *alloc_param)
{
    struct cma_alloc_args alloc_args;
    MS_BOOL ret = TRUE;

	memset(&alloc_args, 0, sizeof(struct cma_alloc_args));

    pthread_mutex_lock(&_CMA_POOL_Mutex);  
    if (_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }

    alloc_args.heap_id = alloc_param->pool_handle_id;
    alloc_args.align = 0x1000ULL;		// 4096
    alloc_args.offset_in_heap = alloc_param->offset_in_pool;
    alloc_args.length = alloc_param->length;
    alloc_args.flags = alloc_param->flags;

    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_ALLOC, &alloc_args))
    {  
        VPRINTF("cma pool get memory fail: heap id %tu, offset %llu, size %lu, flags %tX\n", (ptrdiff_t)alloc_args.heap_id,  
            (unsigned long long)alloc_args.offset_in_heap, (unsigned long)alloc_args.length, (ptrdiff_t)alloc_args.flags);
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }

    if(alloc_args.flags & CMA_FLAG_VIRT_ADDR)
    {
        alloc_param->virt_addr = alloc_args.cpu_addr;
		VPRINTF("\033[35mFunction = %s, Line = %d, alloc_param->virt_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)alloc_param->virt_addr);
	}

CMA_POOL_GETMEM_DONE:
	pthread_mutex_unlock(&_CMA_POOL_Mutex);
   
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param)
{
    struct cma_free_args free_args;
    MS_BOOL ret = TRUE;

	memset(&free_args, 0, sizeof(struct cma_free_args));

    pthread_mutex_lock(&_CMA_POOL_Mutex);
    if(_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }
         
    free_args.heap_id = free_param->pool_handle_id;
    free_args.offset_in_heap = free_param->offset_in_pool;
    free_args.length = free_param->length;
    if(ioctl(_s32FdCMAPool, CMA_POOL_IOC_FREE, &free_args))
    {        
        VPRINTF("cma pool put memory fail: heap id %tu, offset 0x%lX, size 0x%lX\n", (ptrdiff_t)free_args.heap_id,  
            (unsigned long)free_args.offset_in_heap, (unsigned long)free_args.length);
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }

CMA_POOL_PUTMEM_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id)
{
    MS_U32  heap_id = pool_handle_id;
    MS_BOOL ret = TRUE;
    MS_U32 idx = 0;

    pthread_mutex_lock(&_CMA_POOL_Mutex);
    if (_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }

    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_RELEASE, &heap_id))
    {        
        VPRINTF("cma pool release fail: pool id %tu\n", (ptrdiff_t)pool_handle_id);            
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }

    ret = _findPoolHandleId_InCMA_Pool_Table(pool_handle_id, &idx);
    if(ret == TRUE)
    {
        MsOS_MPool_Remove_PA2VARange(cpool_info[idx].Physaddr, cpool_info[idx].VirtStart, 
                cpool_info[idx].CMAPoolSize, cpool_info[idx].bNonCache);              
        munmap((void *)(cpool_info[idx].VirtStart), cpool_info[idx].CMAPoolSize);
        cpool_info[idx].bIsUsed = FALSE;
        cpool_info[idx].pool_handle_id = 0;         
    }
    ret = TRUE; //not find the pool hand id

CMA_POOL_RELEASE_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);   
    return ret;    
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PA_in_pool_info(struct PA_In_CMA_Pool_Param * pa_in_cma_pool_param)
{
    struct pa_in_cma_pool_args pa_in_args;
    pa_in_args.PA = pa_in_cma_pool_param->PA;//input
    VPRINTF("%s:%d to do...\n",__FUNCTION__,__LINE__);
    return false;
}

//only kernel mode will use this function
MS_BOOL CMA_Pool_Get_Info_From_PA(MS_S32 fd,struct CMA_Pool_Get_Info_From_Pa_Param *get_info_param)
{
    MS_BOOL ret = TRUE;
    struct cma_get_info_from_pa get_info_args;
    memset(&get_info_args,0,sizeof(struct cma_get_info_from_pa));
    //VPRINTF("%s:%d  \n",__FUNCTION__,__LINE__);
    if (fd <= 0)
    {
       VPRINTF("%s:%d  fd=%d is false\n",__FUNCTION__,__LINE__,fd);
       return false;
    }

    get_info_args.PA=get_info_param->PA;
    //VPRINTF("%s:%d get_info_args.PA=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)get_info_args.PA);
    if(ioctl(fd,CMA_POOL_IOC_GET_HEAP_INFO_FROM_PA,&get_info_args))
    {
        //VPRINTF("cma pool get heap info from pa fail: PA=0x%lx\n", (unsigned long)get_info_args.PA);
        ret = FALSE;
    }
    get_info_param->heap_id = get_info_args.heap_id;
    get_info_param->miu = get_info_args.miu;
    get_info_param->heap_miu_start_offset = get_info_args.heap_miu_start_offset;
    get_info_param->heap_length = get_info_args.heap_length;
    get_info_param->pa_offset_in_heap = get_info_args.pa_offset_in_heap;
    //VPRINTF("%s:%d  ret=%d\n",__FUNCTION__,__LINE__,ret);
    return ret;
}
MS_BOOL CMA_Pool_CMA_Heap_Get_User_VA(MS_S32 fd,struct CMA_Pool_Heap_Get_User_Va_Param *vma_valid_Param)
{
    struct cma_heap_get_user_va vma_valid_args;
    MS_BOOL ret = TRUE;
    memset(&vma_valid_args,0,sizeof(struct cma_heap_get_user_va));
    if (fd <= 0)
    {
        VPRINTF("%s:%d fd=%d is false \n",__FUNCTION__,__LINE__,fd);
        return FALSE;
    }

    vma_valid_args.heap_id = vma_valid_Param->heap_id;
    vma_valid_args.flags = vma_valid_Param->flags;

    if(ioctl(fd,CMA_POOL_IOC_KERNEL_MODE_GET_USER_VA,&vma_valid_args))
    {
        VPRINTF("cma pool kernel mode get user va fail: heap_id %u,flags %u\n", vma_valid_args.heap_id,vma_valid_args.flags);
		ret = FALSE;
    }
    vma_valid_Param->user_va_valid_flag = vma_valid_args.user_va_valid_flag;
    vma_valid_Param->heap_start_user_space_virt_addr = vma_valid_args.heap_start_user_space_virt_addr;
    VPRINTF("%s:%d  ret=%d  heap_start_user_space_virt_addr=0x%lx\n",__FUNCTION__,__LINE__,ret,(unsigned long)vma_valid_Param->heap_start_user_space_virt_addr);
    return ret;
}

MS_BOOL CMA_Pool_CMA_Mmap_User_VA_Page(MS_S32 fd,struct CMA_Pool_Mmap_User_Va_Page_Param *mmap_user_va_page)
{
    MS_BOOL ret = TRUE;
    struct cma_mmap_user_va_page mmap_user_va_page_args;
    memset(&mmap_user_va_page_args,0,sizeof(struct cma_mmap_user_va_page));
    VPRINTF("%s:%d  \n",__FUNCTION__,__LINE__);
    if (fd <= 0)
    {
        VPRINTF("%s:%d  fd=%d is false\n",__FUNCTION__,__LINE__,fd);
		return false;
    }

    mmap_user_va_page_args.heap_id = mmap_user_va_page->heap_id;
    mmap_user_va_page_args.flags = mmap_user_va_page->flags;
    if(ioctl(fd,CMA_POOL_IOC_KERNEL_MODE_MAPPING_USER_VA_PAGE,&mmap_user_va_page_args))
    {
        VPRINTF("cma pool kernel mode mapping user va page fail: heap_id %u,flags %u\n", mmap_user_va_page_args.heap_id,mmap_user_va_page_args.flags);
		ret = FALSE;
    }
    VPRINTF("%s:%d  ret=%d\n",__FUNCTION__,__LINE__,ret);
    return ret;
}

MS_VIRT MApi_CMA_Pool_GetKernelCMAPooLUserVA(MS_PHY pAddrPhys,MS_BOOL un_cache_flags)
{
    MS_VIRT pAddrVirt=NULL;
    struct CMA_Pool_Get_Info_From_Pa_Param get_info_param;
    struct CMA_Pool_Heap_Get_User_Va_Param vma_valid_Param;
    struct CMA_Pool_Mmap_User_Va_Page_Param mmap_user_va_page;
    MS_BOOL ret = TRUE;

    memset(&get_info_param,0,sizeof(struct CMA_Pool_Get_Info_From_Pa_Param));
    memset(&vma_valid_Param,0,sizeof(struct CMA_Pool_Heap_Get_User_Va_Param));
    memset(&mmap_user_va_page,0,sizeof(struct CMA_Pool_Mmap_User_Va_Page_Param));

    //VPRINTF("%s:%d un_cache_flags=%d  sizeof(MS_VIRT)=%d,sizeof(MS_PHY)=%d,sizeof(unsigned long)=%d ",__FUNCTION__,__LINE__,un_cache_flags,sizeof(MS_VIRT),sizeof(MS_PHY),sizeof(unsigned long));
/*
#if defined (__aarch64__)
    {
        VPRINTF("lx pAddrPhys=0x%lx  \n",pAddrPhys);
    }
#else
    {
        VPRINTF("llx pAddrPhys=0x%llx  \n",pAddrPhys);
    }
#endif
*/
    pthread_mutex_lock(&_CMA_POOL_Mutex);
    if (_s32FdCMAPool <= 0)
    {
        if ((_s32FdCMAPool = open("/dev/cmapool", O_RDWR)) < 0)
        {
            VPRINTF("open /dev/cmapool fail\n");
            pAddrVirt=NULL;
            goto CMA_POOL_USER_VA_DONE;
        }

        memset(cpool_info, 0, sizeof(CMAPOOL_INFO)*MAX_CMAPOOLSIZE);
    }
    get_info_param.PA = pAddrPhys;
    //VPRINTF("%s:%d get_info_param.PA=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)get_info_param.PA);
    ret = CMA_Pool_Get_Info_From_PA(_s32FdCMAPool,&get_info_param);
    if(ret == FALSE)
    {
        //VPRINTF("%s:%d  GET_INFO_FROM_PA fail  PA=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)get_info_param.PA);
        pAddrVirt=NULL;
        goto CMA_POOL_USER_VA_DONE;
    }
    VPRINTF("%s:%d get_info_param.heap_id=%u\n",__FUNCTION__,__LINE__,get_info_param.heap_id);
    vma_valid_Param.heap_id = get_info_param.heap_id;
    if(un_cache_flags)
        vma_valid_Param.flags &= ~CMA_FLAG_KERNEL_MODE_USER_SPACE_MAP_CACHED;
    else
        vma_valid_Param.flags |= CMA_FLAG_KERNEL_MODE_USER_SPACE_MAP_CACHED;
    VPRINTF("%s:%d vma_valid_Param.heap_id=%u   vma_valid_Param.flags=%u\n",__FUNCTION__,__LINE__,vma_valid_Param.heap_id ,vma_valid_Param.flags);
    ret = CMA_Pool_CMA_Heap_Get_User_VA(_s32FdCMAPool,&vma_valid_Param);
    if(ret == FALSE)
    {
        VPRINTF("%s:%d  CMA_Pool_CMA_Heap_Get_User_VA fail  heap_id=%u,flags=%u\n",__FUNCTION__,__LINE__,vma_valid_Param.heap_id,vma_valid_Param.flags);
        pAddrVirt=NULL;
        goto CMA_POOL_USER_VA_DONE;
    }
    VPRINTF("%s:%d vma_valid_Param.user_va_valid_flag=%d\n",__FUNCTION__,__LINE__,vma_valid_Param.user_va_valid_flag);
    if(USER_VA_VALID == (int32_t)vma_valid_Param.user_va_valid_flag)
    {
        pAddrVirt = vma_valid_Param.heap_start_user_space_virt_addr + get_info_param.pa_offset_in_heap;
        VPRINTF("%s:%d  USER_VA_VALID case   pAddrPhys=0x%lx pAddrVirt=0x%lx   get_info_param.pa_offset_in_heap=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)pAddrPhys,(unsigned long)pAddrVirt,(unsigned long)get_info_param.pa_offset_in_heap);
    }
    else if(NO_USER_VA == (int32_t)vma_valid_Param.user_va_valid_flag)
    {
        char * ptr = NULL;
        VPRINTF("%s:%d  NO_USER_VA case\n",__FUNCTION__,__LINE__);
        ptr = mmap(NULL, get_info_param.heap_length, PROT_READ|PROT_WRITE, MAP_SHARED, _s32FdCMAPool, 0);
        VPRINTF("%s:%d  \n",__FUNCTION__,__LINE__);
        if(ptr == (char*)(-1))
        {
            VPRINTF("%s:%d  mmap fail heap_length=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)get_info_param.heap_length);
            pAddrVirt = NULL;
            goto CMA_POOL_USER_VA_DONE;
        }
        VPRINTF("%s:%d get_info_param.heap_id=%u \n",__FUNCTION__,__LINE__,get_info_param.heap_id);
        mmap_user_va_page.heap_id = get_info_param.heap_id;
        if(un_cache_flags)
			mmap_user_va_page.flags &= ~CMA_FLAG_KERNEL_MODE_USER_SPACE_MAP_CACHED;
        else
			mmap_user_va_page.flags |= CMA_FLAG_KERNEL_MODE_USER_SPACE_MAP_CACHED;
        VPRINTF("%s:%d mmap_user_va_page.heap_id=%u \n",__FUNCTION__,__LINE__,mmap_user_va_page.heap_id);
        ret = CMA_Pool_CMA_Mmap_User_VA_Page(_s32FdCMAPool,&mmap_user_va_page);
        if(ret == FALSE)
        {
            VPRINTF("%s:%d  CMA_Pool_CMA_Mmap_User_VA_Page fail heap_id=%u,flags=%u\n",__FUNCTION__,__LINE__,
                                             mmap_user_va_page.heap_id,mmap_user_va_page.flags);
            pAddrVirt=NULL;
			munmap((MS_VIRT *)ptr, get_info_param.heap_length);
            goto CMA_POOL_USER_VA_DONE;
        }
        VPRINTF("%s:%d  \n",__FUNCTION__,__LINE__);
        pAddrVirt  = (intptr_t)ptr  + get_info_param.pa_offset_in_heap;
        VPRINTF("%s:%d pAddrPhys=0x%lx pAddrVirt=0x%lx  get_info_param.pa_offset_in_heap=0x%lx\n",__FUNCTION__,__LINE__,(unsigned long)pAddrPhys,(unsigned long)pAddrVirt,(unsigned long)get_info_param.pa_offset_in_heap);
    }
    else
    {
        VPRINTF("%s:%d  fail  vma_valid_Param.user_va_valid_flag=%d  \n",__FUNCTION__,__LINE__,vma_valid_Param.user_va_valid_flag);
        pAddrVirt = NULL;
        goto CMA_POOL_USER_VA_DONE;
    }
CMA_POOL_USER_VA_DONE:

    pthread_mutex_unlock(&_CMA_POOL_Mutex);
    return pAddrVirt;
}

#endif
