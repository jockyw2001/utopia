////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   cma_mpool_manager.h
/// @brief  CMA mpool Manager interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CMA_POOL_ST_H__
#define __DRV_CMA_POOL_ST_H__
#define KERN_CHUNK_NUM 3

struct cma_alloc_args {
    MS_U64 offset_in_heap;
    MS_VIRT cpu_addr;
    MS_SIZE length;        
    MS_U64 align;
    MS_U32 heap_id;
    MS_U32 flags;
};

struct cma_free_args {
    MS_U32 heap_id;
    MS_U64 offset_in_heap;
    MS_SIZE length;
};

struct cma_heap_info {
    MS_U32 heap_id;
    MS_VIRT flags;

    MS_U32 miu;
    MS_U64 bus_addr;
    MS_U64 heap_miu_start_offset;
    MS_SIZE heap_length;
    MS_VIRT virt_addr;//out:if need,shoud give this out put
};

struct pa_in_cma_pool_args
{
    MS_U64 PA;//in:   PA value
	MS_S32 miu;//out:  in which miu

    MS_BOOL in_heap;//out:  whether in heap.If not, relative value can not get.
    MS_S32 heap_id;     //out: heap id the pa in
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
    MS_U64 heap_length; //out: heap leagth
    MS_U64 pa_offset_in_heap;    //out: pa location in heap
    MS_S32 error_code; // error code when failed,not used yet
};

//only kernel mode will use this
struct cma_get_info_from_pa {
    MS_U64 PA;//in:phy addr that input

    MS_U32 heap_id;//out:in which heap
    MS_U32 miu;//out:in which miu
    MS_U64 heap_miu_start_offset;//out:heap start offset in miu
    MS_U64 heap_length;//out:heap length
    MS_U64 pa_offset_in_heap;//out :pa offset in heap
};

enum USER_VA_VALID_FLAG {
    USER_VA_VALID = (1<<0),
    NO_HEAP = (1<<1),
    NO_USER_VA = (1<<2),
    DIFFERENT_CACHE_TYPE = (1<<3),
};

struct cma_heap_get_user_va {
    MS_U32 heap_id;//in:in which heap
    MS_U32 flags;//in:flags,such as cache or uncache

    enum USER_VA_VALID_FLAG user_va_valid_flag;//out:whether vma is valid
    MS_VIRT heap_start_user_space_virt_addr;//out:if vma is valid and alloced,get this user space virt addr.
};

struct cma_mmap_user_va_page {
    MS_U32 heap_id;//in
    MS_U32 flags;//in
};

#endif

