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
/// file    drvIPAPool.c
/// @brief  IPA Pool Driver
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_MSTAR_IPAPOOL)

//TEST_USE_SELF_KO_MAKEFILE is
//only test code use,p4 code not use!!!!
//never set this macro in p4 with 1.any question ,contact with samson.huang
//though test code,please do not delete this test code.
#define TEST_USE_SELF_KO_MAKEFILE 0

#if TEST_USE_SELF_KO_MAKEFILE
#include <linux/module.h>
#else
#include "MsCommon.h"
#include "halCHIP.h"
#include "halMPool.h"
#endif
#include <linux/slab.h>

#include <linux/kthread.h>

#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include "../../include/drvIPAPool.h"
#include "../mstar2/drv/ipa_pool/mdrv_ipa_pool_uapi.h"

#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/uaccess.h>//for get_fs() /set_fs() use.

#include <linux/version.h>
#if (LINUX_VERSION_CODE  == KERNEL_VERSION(3,10,40))
#include <linux/hardirq.h>
#endif


//debug macro,can not enable in p4.
#define DEBUG_IPA_KERNEL_MODE 1

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAX_IPAPOOLSIZE 16UL

//
//
//for example:
//area 1 map 2 times,area 2 map 2times,area 3 map 1 times,total map 2+2+1=5
//
//   map 2t  map 2t map 1t
//   ------   ----
//   ------   ----   ---
//     1        2    3
//   ------   ---- ------
//-----------------------------same client
#define MAX_CLIENT_MAP_NUM 8UL
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

struct VIRT_MAP_INFO
{
    MS_U64 virt_addr;
    MS_U64 length;
    MS_BOOL bNonCache;
    MS_U64 Physaddr;
};
struct IPA_Pool_Init_Param_No_P
{
    MS_U32 heap_id;     //in: heap id the pool will be created in
    MS_U64 pool_name;//in: global identify name for pool to shared between multiple process
	
    MS_U64 offset_in_heap;    //in: pool location in heap
    MS_U64 len;       //in: pool length in  heap 

    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap_id
    MS_U32 miu;  //out: miu id this heap belongs, index from 0.
    enum IPA_SPACE_TYPE heap_type;//out: return heap type to application
    MS_S32 error_code; // error code when pool init failed

    MS_U64 heap_length; //out: heap leagth
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
};

typedef struct
{
    struct IPA_Pool_Init_Param_No_P Init_Param;
    MS_BOOL bIsUsed;
    struct VIRT_MAP_INFO map_info[MAX_CLIENT_MAP_NUM];
    volatile MS_BOOL polling_thread_delete_task_flag;
    pid_t pthIPAPollingId;
    void (*polling_callback)(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length);
} IPAPOOL_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static struct file * _fileIPAPool = NULL;
static struct mutex  _IPA_POOL_Mutex = __MUTEX_INITIALIZER(_IPA_POOL_Mutex);

#if TEST_USE_SELF_KO_MAKEFILE_TOTAL
IPAPOOL_INFO IPAPool_Info[MAX_IPAPOOLSIZE];
#else
static IPAPOOL_INFO IPAPool_Info[MAX_IPAPOOLSIZE];
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _findEmpty_IPA_Pool_Entry(MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_IPAPOOLSIZE; i++)
    {
        if(IPAPool_Info[i].bIsUsed == FALSE)
        {
            find = TRUE;
            *index = i;
            break;
        }
    }

    if(find == FALSE)
        printk("%s:%d  Not enough IPAPool, must increase MAX_IPAPOOLSIZE!!\n",__FUNCTION__,__LINE__);
    
    return find;
}

static MS_BOOL _findPoolHandleId_InIPA_Pool_Table(MS_U32 pool_handle_id, MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_IPAPOOLSIZE; i++)
    {
        if((IPAPool_Info[i].bIsUsed == TRUE) && (IPAPool_Info[i].Init_Param.pool_handle_id == pool_handle_id))
        {
            find = TRUE;
            *index = i;
            break;
        }
    }
    
    return find;
}

static MS_BOOL _findHeapId_InIPA_Pool_Table(struct IPA_Pool_Init_Param * Init_Param,MS_U32 *index)
{
    MS_BOOL find = FALSE;
    MS_U32 i;

    *index = 0;
    for (i = 0; i < MAX_IPAPOOLSIZE; i++)
    {
        if((IPAPool_Info[i].bIsUsed == TRUE) 
			&& (IPAPool_Info[i].Init_Param.heap_id == Init_Param->space_id)
			&&(!strncmp((char *)IPAPool_Info[i].Init_Param.pool_name, Init_Param->pool_name,strlen(Init_Param->pool_name)))
			&& (IPAPool_Info[i].Init_Param.offset_in_heap == Init_Param->offset_in_heap)
			&& (IPAPool_Info[i].Init_Param.len == Init_Param->len))
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
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Init(struct IPA_Pool_Init_Param * Init_Param)
{
    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

    struct IPA_Pool_Init_Args ipa_init_args;

    MS_BOOL ret = TRUE;
    MS_U32 idx = 0;
    int res = 0;
    mm_segment_t old_fs;
#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d heap_id %u  Init_Param->pool_name=%s\n",__FUNCTION__,__LINE__, Init_Param->space_id,Init_Param->pool_name);
#endif

#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d ,Init_Param=%p  heap_id %u\n",__FUNCTION__,__LINE__,Init_Param, Init_Param->space_id);
    printk("%s:%d Init_Param->pool_name=%s\n",__FUNCTION__,__LINE__,Init_Param->pool_name);
#endif


#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  heap_id %u  Init_Param->pool_name=%s\n",__FUNCTION__,__LINE__, Init_Param->space_id,Init_Param->pool_name);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (!_fileIPAPool)
    {
        int flags=O_RDWR;
#ifdef DEBUG_IPA_KERNEL_MODE
        printk("%s:%d  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif
        _fileIPAPool = filp_open( "/dev/ipapool", flags, 0);    
        if (IS_ERR(_fileIPAPool) )
        {
            printk("%s:%d open /dev/ipapool fail\n",__FUNCTION__,__LINE__);
            ret =  FALSE;
            goto IPA_POOL_INIT_DONE;
        }

        memset(IPAPool_Info, 0, sizeof(IPAPOOL_INFO)*MAX_IPAPOOLSIZE);
    }

#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif

    //avoid mmap more than one time
    ret = _findHeapId_InIPA_Pool_Table(Init_Param, &idx);
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif

    if(ret == TRUE)
    {
         //memcpy(Init_Param,IPAPool_Info[idx].Init_Param,sizeof(struct IPA_Pool_Init_Param));
         Init_Param->space_id = IPAPool_Info[idx].Init_Param.heap_id;
         Init_Param->pool_name = (char *)IPAPool_Info[idx].Init_Param.pool_name;
         Init_Param->offset_in_heap = IPAPool_Info[idx].Init_Param.offset_in_heap;
         Init_Param->len = IPAPool_Info[idx].Init_Param.len;
         Init_Param->pool_handle_id = IPAPool_Info[idx].Init_Param.pool_handle_id;
         Init_Param->miu = IPAPool_Info[idx].Init_Param.miu;
         Init_Param->space_type = IPAPool_Info[idx].Init_Param.heap_type;
         Init_Param->error_code = IPAPool_Info[idx].Init_Param.error_code;
         Init_Param->space_length = IPAPool_Info[idx].Init_Param.heap_length;
         Init_Param->space_miu_start_offset = IPAPool_Info[idx].Init_Param.heap_miu_start_offset;
         
         printk("%s:%d pool_handle_id %u already init!\n",__FUNCTION__,__LINE__, IPAPool_Info[idx].Init_Param.pool_handle_id);
         goto IPA_POOL_INIT_DONE;
    }
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d ,  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif

    ipa_init_args.heap_id = Init_Param->space_id;
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d ,  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif

    if(sizeof(Init_Param->pool_name) > IPAPOOL_NAME_MAX_LONG+1)
    {
        printk("%s:%d pool name %s is too long size is %lu\n",__FUNCTION__,__LINE__,Init_Param->pool_name,sizeof(Init_Param->pool_name));
        BUG();
    }

    strcpy(ipa_init_args.pool_name,Init_Param->pool_name);
    
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d ,  heap_id %u\n",__FUNCTION__,__LINE__, Init_Param->space_id);
#endif

    ipa_init_args.offset_in_heap = Init_Param->offset_in_heap;
    ipa_init_args.len = Init_Param->len;	
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d Init_Param  heapid %u  ,offset=0x%llx,len=0x%llx\n",__FUNCTION__,__LINE__, Init_Param->space_id,(long long unsigned int)Init_Param->offset_in_heap,(long long unsigned int)Init_Param->len);
    printk("%s:%d Init_Param->pool_name=%s,ipa_init_args->pool_name=%s\n",__FUNCTION__,__LINE__,Init_Param->pool_name,ipa_init_args.pool_name);
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_INIT, (unsigned long )(&ipa_init_args));
    set_fs(old_fs);
    if (res < 0 || ipa_init_args.error_code != IPAERROR_OK) 
    {
        printk("%s:%d error: ipa init failed, heapid %u error_code %d\n",__FUNCTION__,__LINE__, Init_Param->space_id, ipa_init_args.error_code);
        ret =  FALSE;
        goto IPA_POOL_INIT_CLOSE;
    }
#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d   heap_id %u, pool_handle_id %u heap_length %llu\n",__FUNCTION__,__LINE__, 
		Init_Param->space_id, ipa_init_args.pool_handle_id, (long long unsigned int)ipa_init_args.heap_length);
#endif	
    Init_Param->pool_handle_id = ipa_init_args.pool_handle_id;
    Init_Param->miu = ipa_init_args.miu;
    Init_Param->space_type = ipa_init_args.heap_type;
    Init_Param->error_code = ipa_init_args.error_code;
    Init_Param->space_length = ipa_init_args.heap_length;
    Init_Param->space_miu_start_offset = ipa_init_args.heap_miu_start_offset;
    ret = _findEmpty_IPA_Pool_Entry(&idx);
    if(ret == FALSE)
    {
        printk("%s:%d   error: pool_handle_id %u init failed!\n",__FUNCTION__,__LINE__, ipa_init_args.pool_handle_id);
        goto IPA_POOL_INIT_DONE;
    }
    /* IPA Pool setting*/
    IPAPool_Info[idx].Init_Param.heap_id = Init_Param->space_id;
    
#if defined (__aarch64__)
    IPAPool_Info[idx].Init_Param.pool_name = (MS_U64)Init_Param->pool_name;
#else
    IPAPool_Info[idx].Init_Param.pool_name = (MS_U32)Init_Param->pool_name;
#endif
    IPAPool_Info[idx].Init_Param.offset_in_heap =Init_Param->offset_in_heap;
    IPAPool_Info[idx].Init_Param.len =Init_Param->len;
    IPAPool_Info[idx].Init_Param.pool_handle_id =Init_Param->pool_handle_id;
    IPAPool_Info[idx].Init_Param.miu =Init_Param->miu;
    IPAPool_Info[idx].Init_Param.heap_type =Init_Param->space_type;
    IPAPool_Info[idx].Init_Param.error_code =Init_Param->error_code;
    IPAPool_Info[idx].Init_Param.heap_length =Init_Param->space_length;
    IPAPool_Info[idx].Init_Param.heap_miu_start_offset =Init_Param->space_miu_start_offset;
    IPAPool_Info[idx].bIsUsed = TRUE; 
    IPAPool_Info[idx].pthIPAPollingId = -1;//in init,no polling id yet.
    IPAPool_Info[idx].polling_thread_delete_task_flag = FALSE;

#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d    heap_id %u pool_handle_id %u miu %u offset 0x%llx len 0x%llx\n",__FUNCTION__,__LINE__, 
		Init_Param->space_id, Init_Param->pool_handle_id, Init_Param->miu, (long long unsigned int)Init_Param->offset_in_heap, (long long unsigned int)Init_Param->len);
#endif
    mutex_unlock(&_IPA_POOL_Mutex);
#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d   return %d\n",__FUNCTION__,__LINE__,ret);	
#endif

    return ret;

IPA_POOL_INIT_CLOSE:
    filp_close(_fileIPAPool,NULL);
IPA_POOL_INIT_DONE:
    printk("%s:%d  ret=%d\n",__FUNCTION__,__LINE__,ret);	
    mutex_unlock(&_IPA_POOL_Mutex);

    return ret;
}

//MS_U32 u32flag. Special Flag for customer mem allocation
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem(struct IPA_Pool_GetMem_Param * get_param)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_Alloc_Args  ipa_alloc_args;

    int res = 0;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d handle_id 0x%x length 0x%llx\n",__FUNCTION__,__LINE__, get_param->pool_handle_id, (long long unsigned int)get_param->length);
#endif

    mutex_lock(&_IPA_POOL_Mutex);  
    if (IS_ERR(_fileIPAPool))
    {
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    ipa_alloc_args.pool_handle_id = get_param->pool_handle_id;	
    ipa_alloc_args.offset_in_pool = get_param->offset_in_pool;
    ipa_alloc_args.length = get_param->length;
    ipa_alloc_args.timeout = 0;// 0 means if fail ,will not try again
    old_fs = get_fs();
    set_fs(KERNEL_DS);    
    res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_ALLOC, (unsigned long)(&ipa_alloc_args));
    set_fs(old_fs);
    if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
    {  
        printk("%s:%d fail: pool_handle_id %u, offset 0x%llx, len 0x%llx  ipa_alloc_args.error_code=0x%x  res=%d\n",__FUNCTION__,__LINE__, get_param->pool_handle_id, 
			(long long unsigned int)get_param->offset_in_pool, (long long unsigned int)get_param->length,ipa_alloc_args.error_code,res);
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    get_param->error_code = ipa_alloc_args.error_code;
    
IPA_POOL_GETMEM_DONE:
    mutex_unlock(&_IPA_POOL_Mutex);
    
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_PutMem(struct IPA_Pool_PutMem_Param * put_param)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_free_Args ipa_free_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d handle_id 0x%x length 0x%llx offset 0x%llx\n",__FUNCTION__,__LINE__, 
		put_param->pool_handle_id, (long long unsigned int)put_param->length, (long long unsigned int)put_param->offset_in_pool);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if(IS_ERR(_fileIPAPool) )
    {
        ret = FALSE;
        goto IPA_POOL_PUTMEM_DONE;
    }

    ipa_free_args.pool_handle_id = put_param->pool_handle_id;
    ipa_free_args.offset_in_pool = put_param->offset_in_pool;
    ipa_free_args.length = put_param->length;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if(file_ioctl(_fileIPAPool, IPA_POOL_IOC_FREE, (unsigned long)(&ipa_free_args)))
    {  
        set_fs(old_fs);
        printk("%s:%d fail: pool_handle_id 0x%x, offset 0x%llx, len 0x%llx\n",__FUNCTION__,__LINE__,
			put_param->pool_handle_id, (long long unsigned int)put_param->offset_in_pool, (long long unsigned int)put_param->length);
        ret = FALSE;
        goto IPA_POOL_PUTMEM_DONE;
    }
    set_fs(old_fs);

IPA_POOL_PUTMEM_DONE:
    mutex_unlock(&_IPA_POOL_Mutex);

    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Release(MS_U32 pool_handle_id)
{
    MS_BOOL ret = TRUE;

    struct IPA_Pool_Deinit_Args  deinit_args;
    struct IPA_Pool_Unmap_Args  unmap_args;
    
    MS_U32 idx = 0;
    int res = 0,i=0;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d handle_id %u\n",__FUNCTION__,__LINE__, pool_handle_id);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool) )
    {
        ret = FALSE;
        goto IPA_POOL_RELEASE_DONE;
    }

    ret = _findPoolHandleId_InIPA_Pool_Table(pool_handle_id, &idx);
    if(ret == TRUE)
    {
        for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
        {
            // tmp_map_info
            if((0 !=IPAPool_Info[idx].map_info[i].virt_addr)
			    && (0 != IPAPool_Info[idx].map_info[i].length))
            {
                unmap_args.virt_addr =	IPAPool_Info[idx].map_info[i].virt_addr;
                unmap_args.length = IPAPool_Info[idx].map_info[i].length;
#if TEST_USE_SELF_KO_MAKEFILE
#else
                MsOS_MPool_Remove_PA2VARange(IPAPool_Info[idx].map_info[i].Physaddr, IPAPool_Info[idx].map_info[i].virt_addr, IPAPool_Info[idx].map_info[i].length, IPAPool_Info[idx].map_info[i].bNonCache);
#endif
                old_fs = get_fs();
                set_fs(KERNEL_DS);
                res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_UNMAP, (unsigned long)(&unmap_args));
                set_fs(old_fs);
                if (res < 0)
                {
                    printk("%s:%d error: pool_handle_id %u unmap failed\n",__FUNCTION__,__LINE__, pool_handle_id);
                    ret = FALSE;
                    goto IPA_POOL_RELEASE_DONE;
                }
                IPAPool_Info[idx].map_info[i].virt_addr = 0;
                IPAPool_Info[idx].map_info[i].length = 0;
            }
        }

        if(MAX_CLIENT_MAP_NUM == i)
        {
            // if same client all maps have been unmapped ,set  IPAPool_Info[idx].bIsUsed be FALSE
            IPAPool_Info[idx].bIsUsed = FALSE;
            IPAPool_Info[idx].Init_Param.pool_handle_id = 0;
        }
    }

    deinit_args.pool_handle_id = pool_handle_id;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    if (file_ioctl(_fileIPAPool, IPA_POOL_IOC_DEINIT, (unsigned long)(&deinit_args)))
    {   
        set_fs(old_fs);
        printk("%s:%d pool_handle_id %u deinit fail\n",__FUNCTION__,__LINE__, pool_handle_id);            
        ret = FALSE;
        goto IPA_POOL_RELEASE_DONE;
    }
    set_fs(old_fs);
    ret = TRUE; //not find the pool hand id

    //after deinit, if have polling thread,should delete task
    if(IPAPool_Info[idx].pthIPAPollingId != -1)
    {
        #if 0//no need this while,for later pthread_join + pthread_cancel will wait for set delete_task_flag.
        while(1)
        {
            if(TRUE == IPAPool_Info[idx].polling_thread_delete_task_flag)
                break;
        }
        #endif

        //should add some code about waitting polling thread to finish???
        //free_task(find_task_by_vpid(IPAPool_Info[idx].pthIPAPollingId));
        //mb();
        //__put_task_struct(find_task_by_vpid(IPAPool_Info[idx].pthIPAPollingId));

        if(TRUE != IPAPool_Info[idx].polling_thread_delete_task_flag)
        {
            printk("%s:%d error !!!!!!   after free_task  polling_thread_delete_task_flag is not TRUE!!!\n",__FUNCTION__,__LINE__);
            ret = FALSE;
        }

    }

IPA_POOL_RELEASE_DONE:
    mutex_unlock(&_IPA_POOL_Mutex);   
    return ret;
}

#if 0//in user mode,use MapUserVA and UnmapUserVA,but in kernel mode use MapKerVA and UnmapKerVA.
//allow a same input [pool_handle_id, offset_in_pool, length, cache_type] map for more than once,
//and get different result output [virt_addr]
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_MapUserVA(struct IPA_Pool_Map_User_Param * map_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_Map_Args map_args;

    MS_U32 idx = 0;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d pool_handle_id %u, offset_in_pool 0x%llx, len 0x%llx,  cache_type %u\n",__FUNCTION__,__LINE__, map_param->pool_handle_id,
		(long long unsigned int)map_param->offset_in_pool, (long long unsigned int)map_param->length,map_param->cache_type);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool) )
    {
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }

    if(0 == map_param->length)
    {
        printk("%s:%d  error, len 0x%llx is invalid\n",__FUNCTION__,__LINE__,  (long long unsigned int)map_param->length);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;       
    }
	
    map_args.pool_handle_id = map_param->pool_handle_id;
    map_args.offset_in_pool = map_param->offset_in_pool;
    map_args.length = map_param->length;
    map_args.map_va_type = map_param->cache_type;

    old_fs = get_fs();
    set_fs(KERNEL_DS);	
    res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_MAP, (unsigned long)(&map_args));
    set_fs(old_fs);
    if (res < 0 || map_args.error_code != IPAERROR_OK)
    {  
        printk("%s:%d  fail: pool_handle_id %u, offset_in_pool 0x%llx, len 0x%llx,  cache_type %u\n",__FUNCTION__,__LINE__, map_param->pool_handle_id, 
			(long long unsigned int)map_param->offset_in_pool, (long long unsigned int)map_param->length,map_param->cache_type);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }
    map_param->virt_addr = map_args.virt_addr;
    map_param->error_code = map_args.error_code;

    ret = _findPoolHandleId_InIPA_Pool_Table(map_param->pool_handle_id, &idx);
    if(ret == TRUE)
    {
        int i=0;
        for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
        {
            if((0 == IPAPool_Info[idx].map_info[i].virt_addr)
			  || (0 == IPAPool_Info[idx].map_info[i].length))
            {
                IPAPool_Info[idx].map_info[i].virt_addr  = map_args.virt_addr;
                IPAPool_Info[idx].map_info[i].length = map_args.length;
#if TEST_USE_SELF_KO_MAKEFILE
#else
                _miu_offset_to_phy(IPAPool_Info[idx].Init_Param.miu, IPAPool_Info[idx].Init_Param.offset_in_heap + map_param->offset_in_pool, IPAPool_Info[idx].map_info[i].Physaddr); // get miu base addr
                if(IPA_VA_CACHE_NONE_CACHE_Param == map_param->cache_type)
                    IPAPool_Info[idx].map_info[i].bNonCache = TRUE;
                else
                    IPAPool_Info[idx].map_info[i].bNonCache = FALSE;

                MsOS_MPool_Add_PA2VARange(IPAPool_Info[idx].map_info[i].Physaddr, IPAPool_Info[idx].map_info[i].virt_addr, IPAPool_Info[idx].map_info[i].length, IPAPool_Info[idx].map_info[i].bNonCache);
#endif
                break;
            }
        }
        
        if(MAX_CLIENT_MAP_NUM == i)
        {
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            //here later can add code for dump map info
            ret = FALSE;
            goto IPA_POOL_MapUserVA_DONE;		   
        }
    }

IPA_POOL_MapUserVA_DONE:
#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d ret=%d\n",__FUNCTION__,__LINE__,ret);
#endif
    mutex_unlock(&_IPA_POOL_Mutex);

    return ret;  
}

void  __attribute__((weak)) MApi_IPA_Pool_UnmapUserVA(struct IPA_Pool_Unmap_User_Param * unmap_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    int i=0,idx=0,find_virt_info_i=-1,find_virt_info_idx=-1;
    MS_BOOL find_virt_info = FALSE;

    struct IPA_Pool_Unmap_Args  unmap_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return;
    }
#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d   virt_addr 0x%llx, len 0x%llx \n",__FUNCTION__,__LINE__,(long long unsigned int)unmap_param->virt_addr, (long long unsigned int)unmap_param->length);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool))
    {
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    }
    for(idx=0;idx < MAX_IPAPOOLSIZE;idx++)
    {
        if(TRUE == IPAPool_Info[idx].bIsUsed)
        {
            for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
            {
                if((unmap_param->virt_addr == IPAPool_Info[idx].map_info[i].virt_addr)
			  	 && (unmap_param->length == IPAPool_Info[idx].map_info[i].length))
                {
                    find_virt_info = TRUE;
                    find_virt_info_idx = idx;
                    find_virt_info_i = i;
                }
            }
        }
    }
    if(FALSE == find_virt_info)
    {
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx,length=0x%llx\n", (long long unsigned int)unmap_param->virt_addr,(long long unsigned int)unmap_param->length);
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx,length=0x%llx\n", (long long unsigned int)unmap_param->virt_addr,(long long unsigned int)unmap_param->length);
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx,length=0x%llx\n", (long long unsigned int)unmap_param->virt_addr,(long long unsigned int)unmap_param->length);
        //later will add some debug code
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    }

    unmap_args.virt_addr = unmap_param->virt_addr;
    unmap_args.length = unmap_param->length;
#if TEST_USE_SELF_KO_MAKEFILE
#else
    MsOS_MPool_Remove_PA2VARange(IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].Physaddr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].bNonCache); 	   
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_UNMAP,(unsigned long)(&unmap_args));
    set_fs(old_fs);
    if (res < 0)
    {
        printk("%s:%d  fail:  virt_addr 0x%llx, len 0x%llx \n",__FUNCTION__,__LINE__, 
			(long long unsigned int)unmap_param->virt_addr, (long long unsigned int)unmap_param->length);
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    } 
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr = 0;
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length = 0;
	
IPA_POOL_UnmapUserVA_DONE:
    mutex_unlock(&_IPA_POOL_Mutex); 

    return;
}
#endif
static MS_BOOL MApi_IPA_Pool_MapKerVA(struct IPA_Pool_Map_Param * map_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_Map_Args map_args;

    MS_U32 idx = 0;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d pool_handle_id %u, offset_in_pool 0x%llx, len 0x%llx,  cache_type %u\n",__FUNCTION__,__LINE__, map_param->pool_handle_id,
        (long long unsigned int)map_param->offset_in_pool, (long long unsigned int)map_param->length,map_param->cache_type);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool) )
    {
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }

    if(0 == map_param->length)
    {
        printk("%s:%d  error, len 0x%llx is invalid\n",__FUNCTION__,__LINE__,  (long long unsigned int)map_param->length);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;       
    }
    
    map_args.pool_handle_id = map_param->pool_handle_id;
    map_args.offset_in_pool = map_param->offset_in_pool;
    map_args.length = map_param->length;
    map_args.map_va_type = map_param->cache_type;

    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_KERNEL_MAP, (unsigned long)(&map_args));
    set_fs(old_fs);
    
    if (res < 0 || map_args.error_code != IPAERROR_OK)
    {  
        printk("%s:%d  fail: pool_handle_id %u, offset_in_pool 0x%llx, len 0x%llx,  cache_type %u\n",__FUNCTION__,__LINE__, map_param->pool_handle_id, 
            (long long unsigned int)map_param->offset_in_pool, (long long unsigned int)map_param->length,map_param->cache_type);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }
    map_param->virt_addr = map_args.virt_addr;
    map_param->error_code = map_args.error_code;

    ret = _findPoolHandleId_InIPA_Pool_Table(map_param->pool_handle_id, &idx);
    if(ret == TRUE)
    {
        int i=0;
        for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
        {
            if((0 == IPAPool_Info[idx].map_info[i].virt_addr)
              || (0 == IPAPool_Info[idx].map_info[i].length))
            {
                IPAPool_Info[idx].map_info[i].virt_addr  = map_args.virt_addr;
                IPAPool_Info[idx].map_info[i].length = map_args.length;
#if TEST_USE_SELF_KO_MAKEFILE
#else
                _miu_offset_to_phy(IPAPool_Info[idx].Init_Param.miu, IPAPool_Info[idx].Init_Param.offset_in_heap + map_param->offset_in_pool, IPAPool_Info[idx].map_info[i].Physaddr); // get miu base addr
                if(IPA_VA_CACHE_NONE_CACHE_Param == map_param->cache_type)
                    IPAPool_Info[idx].map_info[i].bNonCache = TRUE;
                else
                    IPAPool_Info[idx].map_info[i].bNonCache = FALSE;
                MsOS_MPool_Add_PA2VARange(IPAPool_Info[idx].map_info[i].Physaddr, IPAPool_Info[idx].map_info[i].virt_addr, IPAPool_Info[idx].map_info[i].length, IPAPool_Info[idx].map_info[i].bNonCache);
#endif
                break;
            }
        }
        if(MAX_CLIENT_MAP_NUM == i)
        {
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            printk("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            //here later can add code for dump map info
            ret = FALSE;
            goto IPA_POOL_MapUserVA_DONE;          
        }
    }

IPA_POOL_MapUserVA_DONE:
    printk("%s ret=%d\n",__FUNCTION__,ret); 
    mutex_unlock(&_IPA_POOL_Mutex);  

    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_MapVA(struct IPA_Pool_Map_Param * map_param)
{
    return MApi_IPA_Pool_MapKerVA(map_param);
}

static void MApi_IPA_Pool_UnmapKerVA(struct IPA_Pool_Unmap_Param * unmap_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    int i=0,idx=0,find_virt_info_i=-1,find_virt_info_idx=-1;
    MS_BOOL find_virt_info = FALSE;

    struct IPA_Pool_Unmap_Args unmap_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return;
    }
    
#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d   virt_addr 0x%llx \n",__FUNCTION__,__LINE__, (long long unsigned int)unmap_param->virt_addr);
#endif    

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool))
    {
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    }
    for(idx=0;idx < MAX_IPAPOOLSIZE;idx++)
    {
        if(TRUE == IPAPool_Info[idx].bIsUsed)
        {
            for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
            {
                if(unmap_param->virt_addr == IPAPool_Info[idx].map_info[i].virt_addr)
                {
                    find_virt_info = TRUE;
                    find_virt_info_idx = idx;
                    find_virt_info_i = i;
                }
            }
        }
    }
    if(FALSE == find_virt_info)
    {
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx\n", (long long unsigned int)unmap_param->virt_addr);
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx\n", (long long unsigned int)unmap_param->virt_addr);
        printk("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%llx\n", (long long unsigned int)unmap_param->virt_addr);
        //later will add some debug code
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    }

    unmap_args.virt_addr = unmap_param->virt_addr;
#if TEST_USE_SELF_KO_MAKEFILE
#else
    MsOS_MPool_Remove_PA2VARange(IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].Physaddr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].bNonCache);      
#endif
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_KERNEL_UNMAP,(unsigned long)(&unmap_args));
    set_fs(old_fs);
    if (res < 0)
    {
        printk("%s:%d  fail:  virt_addr 0x%llx\n",__FUNCTION__,__LINE__, (long long unsigned int)unmap_param->virt_addr);
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    } 
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr = 0;
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length = 0;
    
IPA_POOL_UnmapUserVA_DONE:
    mutex_unlock(&_IPA_POOL_Mutex); 

    return;
}

void __attribute__((weak)) MApi_IPA_Pool_UnmapVA(struct IPA_Pool_Unmap_Param * unmap_param)
{
    return MApi_IPA_Pool_UnmapKerVA(unmap_param);
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_DCacheFlush(struct IPA_Pool_DCacheFlush_Param* dcache_flush_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_DCacheFlush_Args dcache_flush_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d  virt_addr 0x%llx, length 0x%llx flush_type 0x%x\n", 
		__FUNCTION__,__LINE__,(long long unsigned int)dcache_flush_param->virt_addr, (long long unsigned int)dcache_flush_param->length, dcache_flush_param->flush_type);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool))
    {
        ret = FALSE;
        goto IPA_POOL_DCacheFlush_DONE;
    }

    dcache_flush_args.virt_addr = dcache_flush_param->virt_addr;
    dcache_flush_args.length = dcache_flush_param->length;
    dcache_flush_args.flush_type = dcache_flush_param->flush_type;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_FLUSH,(unsigned long)(&dcache_flush_args));
    set_fs(old_fs);
    if (res < 0)
    {
        printk("%s:%d  fail:  virt_addr 0x%llx, length 0x%llx , flush_type 0x%x\n",
            __FUNCTION__,__LINE__,(long long unsigned int)dcache_flush_param->virt_addr, (long long unsigned int)dcache_flush_param->length, dcache_flush_param->flush_type);
        ret = FALSE;
        goto IPA_POOL_DCacheFlush_DONE;
    }

IPA_POOL_DCacheFlush_DONE:
    mutex_unlock(&_IPA_POOL_Mutex);

    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_HEAP_ATTR(struct IPA_Pool_Heap_Attr_Param* heap_attr_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_Heap_Attr  heap_attr_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  heap_id 0x%x\n",__FUNCTION__,__LINE__, heap_attr_param->heap_id);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool ))
    {
        ret = FALSE;
        goto IPA_POOL_HEAP_ATTR_DONE;
    }

    heap_attr_args.heap_id = heap_attr_param->heap_id;
    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_HEAP_ATTR,(unsigned long)(&heap_attr_args));
    set_fs(old_fs);
    if (res < 0 || heap_attr_args.error_code != IPAERROR_OK)
    {
        printk("%s:%d  fail:  heap_id 0x%x\n",__FUNCTION__,__LINE__,heap_attr_param->heap_id);
        ret = FALSE;
        goto IPA_POOL_HEAP_ATTR_DONE;
    }
    memcpy(heap_attr_param->name,heap_attr_args.name,IPAPOOL_HEAP_NAME_MAX_LEN);
    heap_attr_param->heap_miu_start_offset = heap_attr_args.heap_miu_start_offset;
    heap_attr_param->heap_length = heap_attr_args.heap_length;
    heap_attr_param->miu = heap_attr_args.miu;
    heap_attr_param->heap_type = heap_attr_args.heap_type;
    heap_attr_param->error_code = heap_attr_args.error_code;

IPA_POOL_HEAP_ATTR_DONE:
    mutex_unlock(&_IPA_POOL_Mutex); 

    return ret;	
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GETIPCHANDLE(struct IPA_Pool_GetIpcHandle_Param* getipchandle_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_GetIpcHandle_Args  getipchandle_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d    pool_handle_id 0x%x\n",__FUNCTION__,__LINE__, getipchandle_param->pool_handle_id);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool))
    {
        printk("%s:%d  fail  pool_handle_id=0x%x,   _fileIPAPool=%p \n",__FUNCTION__,__LINE__, getipchandle_param->pool_handle_id,_fileIPAPool);      	
        ret = FALSE;
        goto IPA_POOL_GETIPCHANDLE_DONE;
    }

    getipchandle_args.pool_handle_id = getipchandle_param->pool_handle_id;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_GETIPCHANDLE,(unsigned long)(&getipchandle_args));
    set_fs(old_fs);
    if (res < 0 || getipchandle_args.error_code != IPAERROR_OK)
    {
        printk("%s:%d  fail:  pool_handle_id 0x%x\n",__FUNCTION__,__LINE__,getipchandle_param->pool_handle_id);
        ret = FALSE;
        goto IPA_POOL_GETIPCHANDLE_DONE;
    } 
    getipchandle_param->ipc_handle_id = getipchandle_args.ipc_handle_id;
    getipchandle_param->error_code = getipchandle_args.error_code;

IPA_POOL_GETIPCHANDLE_DONE:
    mutex_unlock(&_IPA_POOL_Mutex); 

    return ret;	
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_InstallIpcHandle(struct IPA_Pool_InstallIpcHandle_Param* installipchandle_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;

    struct IPA_Pool_InstallIpcHandle_Args installipchandle_args;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d  ipc_handle_id 0x%x\n",__FUNCTION__,__LINE__, installipchandle_param->ipc_handle_id);
#endif

    mutex_lock(&_IPA_POOL_Mutex);
    if ( !_fileIPAPool || IS_ERR(_fileIPAPool))
    {
#if DEBUG_IPA_KERNEL_MODE     
        printk("%s:%d ipc_handle_id=0x%x,   _fileIPAPool=%p \n",__FUNCTION__,__LINE__, installipchandle_param->ipc_handle_id,_fileIPAPool); 
#endif
        //this process may not open ipapool yet,need we open for this process.
        int flags=O_RDWR;
        _fileIPAPool = filp_open( "/dev/ipapool", flags, 0);
        if (IS_ERR(_fileIPAPool) )
        {
            printk("%s:%d open /dev/ipapool fail   ipc_handle_id=0x%x,   _fileIPAPool=%p\n",__FUNCTION__,__LINE__, installipchandle_param->ipc_handle_id,_fileIPAPool);
            ret =  FALSE;
            goto IPA_POOL_InstallIpcHandle_DONE;
        }

        memset(IPAPool_Info, 0, sizeof(IPAPOOL_INFO)*MAX_IPAPOOLSIZE);
        
    }

    installipchandle_args.ipc_handle_id = installipchandle_param->ipc_handle_id;
    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_INSTALLIPCHANDLE,(unsigned long)(&installipchandle_args));
    set_fs(old_fs);
    
    if (res < 0 || installipchandle_args.error_code != IPAERROR_OK)
    {
        printk("%s:%d  fail:  heap_id 0x%x\n",__FUNCTION__,__LINE__,installipchandle_param->ipc_handle_id);
        ret = FALSE;
        goto IPA_POOL_InstallIpcHandle_DONE;
    } 
    installipchandle_param->pool_handle_id = installipchandle_args.pool_handle_id;
    installipchandle_param->error_code = installipchandle_args.error_code;

IPA_POOL_InstallIpcHandle_DONE:
    mutex_unlock(&_IPA_POOL_Mutex); 

    return ret;	
}

//unit of timeout is millisecond.
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem_Timeout(struct IPA_Pool_GetMem_Param* get_param,MS_U32 timeout_ms)
{
    MS_BOOL ret = TRUE;

    struct IPA_Pool_Alloc_Args  ipa_alloc_args;

    int res = 0;
    mm_segment_t old_fs;

    if (in_interrupt() || (current->flags & PF_KTHREAD))
    {
        printk("should only use kernel mode  utopia API %s in user process\n",__FUNCTION__);	
        return false;
    }

#if DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d handle_id 0x%x length 0x%llx timeout 0x%x\n",__FUNCTION__,__LINE__, 
		get_param->pool_handle_id, (long long unsigned int)get_param->length,timeout_ms);
#endif

    mutex_lock(&_IPA_POOL_Mutex);  
    if (IS_ERR(_fileIPAPool ))
    {
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    ipa_alloc_args.pool_handle_id = get_param->pool_handle_id;	
    ipa_alloc_args.offset_in_pool = get_param->offset_in_pool;
    ipa_alloc_args.length = get_param->length;
    
    ipa_alloc_args.timeout = timeout_ms;
    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_ALLOC, (unsigned long)(&ipa_alloc_args));
    set_fs(old_fs);
    if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
    {
        MS_U32 each_delay_ms = 50;//empirical value in our test.
        MS_U32 now_delay_ms=0;
        while(1)
        {    
            // delay and try again
            mutex_unlock(&_IPA_POOL_Mutex);

#if DEBUG_IPA_KERNEL_MODE        
            printk("%s:%d each_delay_ms=%u, delay and try alloc again now_delay_ms=%u\n",__FUNCTION__,__LINE__,each_delay_ms,now_delay_ms);
#endif

            ipa_alloc_args.timeout = 0;//into kernel will not consider timeout again
#if TEST_USE_SELF_KO_MAKEFILE
            schedule_timeout_interruptible(msecs_to_jiffies(each_delay_ms));
#else
            MsOS_DelayTask(each_delay_ms);
#endif
            mutex_lock(&_IPA_POOL_Mutex);
            
            old_fs = get_fs();
            set_fs(KERNEL_DS);
            res = file_ioctl(_fileIPAPool, IPA_POOL_IOC_ALLOC, (unsigned long)(&ipa_alloc_args));
            set_fs(old_fs);
            if ((res == 0) && (ipa_alloc_args.error_code == IPAERROR_OK))
                break;
            now_delay_ms += each_delay_ms;
            if(now_delay_ms >= timeout_ms)
                break;           
        }
        if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
        {
            printk("%s:%d fail: pool_handle_id %u, offset 0x%llx, len 0x%llx  ipa_alloc_args.error_code=0x%x ipa_alloc_args.timeout=%u res=%d\n",__FUNCTION__,__LINE__, get_param->pool_handle_id, 
			(long long unsigned int)get_param->offset_in_pool, (long long unsigned int)get_param->length,ipa_alloc_args.error_code,ipa_alloc_args.timeout,res);
            ret = FALSE;
            goto IPA_POOL_GETMEM_DONE;
        }
    }

    get_param->error_code = ipa_alloc_args.error_code;
    
IPA_POOL_GETMEM_DONE:
    mutex_unlock(&_IPA_POOL_Mutex);

    return ret;

}

int __attribute__((weak)) IPA_Pool_Polling_Task(void *argc)
{
#if 1
    printk("%s:%d not realize yet!\n",__FUNCTION__,__LINE__);
	return -1;
#else
    MS_BOOL ret = TRUE;
    int res=0;
    MS_U32 idx = 0;
    mm_segment_t old_fs;

    struct IPA_Pool_Event_Args  *pool_event_args =kmalloc(sizeof(struct IPA_Pool_Event_Args),GFP_KERNEL);
    if(!pool_event_args)
    {
        printk("%s:%d  alloc fail  \n",__FUNCTION__,__LINE__);
        return -1;
    }

    memset(pool_event_args, 0, sizeof(struct IPA_Pool_Event_Args));

    struct IPA_Pool_InstallIpcHandle_Param installipchandle_param;
    installipchandle_param.ipc_handle_id = (*(MS_U32 *)argc);
    
    ret = MApi_IPA_Pool_InstallIpcHandle(&installipchandle_param);
    if(ret == FALSE || installipchandle_param.error_code != 0)
    {
        printk("%s:%d  fail\n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    MS_U32 pool_handle_id  = installipchandle_param.pool_handle_id;


    mutex_lock(&_IPA_POOL_Mutex);
    if (IS_ERR(_fileIPAPool))
    {
        printk("%s:%d  fail  \n",__FUNCTION__,__LINE__);      	
        ret = FALSE;
        mutex_unlock(&_IPA_POOL_Mutex); 
        return -1;
    } 
    pool_event_args->pool_handle_id = pool_handle_id;

    ret = _findPoolHandleId_InIPA_Pool_Table(pool_event_args->pool_handle_id, &idx);
    mutex_unlock(&_IPA_POOL_Mutex);  
    if(ret == FALSE)
    {
        printk("%s:%d  fail not find in pool table\n",__FUNCTION__,__LINE__);
        return -1;
    }
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  before while\n",__FUNCTION__,__LINE__);
#endif
    old_fs = get_fs();      
    set_fs(KERNEL_DS);

    while(1)
    {  
        //ioctl for event and after callback,shoud ioctl again.
        //before each ioctl,should clear invalid event value.
        memset(pool_event_args,0,sizeof(struct IPA_Pool_Event_Args));
        pool_event_args->pool_handle_id = pool_handle_id;
        res = file_ioctl(_fileIPAPool,IPA_POOL_IOC_POLL,(unsigned long)pool_event_args);
        if(res < 0)
        {
            printk("%s:%d  fail\n",__FUNCTION__,__LINE__);
            ret = FALSE;
            goto IPA_POOL_POLLING_TASK_DONE;   
        }

        if(IPA_EVENT_NO_WAIT== pool_event_args->event )
        {
#ifdef DEBUG_IPA_KERNEL_MODE
        printk("%s:%d  break\n",__FUNCTION__,__LINE__);
#endif        
            break; 
        }
        else if(IPA_EVENT_CONFLICT == pool_event_args->event )
        {
            //invoke callback corresponding to that pool event
            IPAPool_Info[idx].polling_callback(pool_event_args->pool_handle_id,pool_event_args->start,pool_event_args->length);//call callback
        }
        else
        {
            printk("%s error ,event %d is invalid !!! but continue while\n",__FUNCTION__,pool_event_args->event);
        }
    }
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  after while\n",__FUNCTION__,__LINE__);
#endif

IPA_POOL_POLLING_TASK_DONE:

    //before return ,set this flag.And this flag will be checked in MApi_IPA_Pool_Release.
    IPAPool_Info[idx].polling_thread_delete_task_flag = TRUE;
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  have set polling_thread_delete_task_flag\n",__FUNCTION__,__LINE__);
#endif
    kfree(pool_event_args);

    set_fs(old_fs);

#ifdef DEBUG_IPA_KERNEL_MODE    
    printk("%s:%d  \n",__FUNCTION__,__LINE__);
#endif
#endif
    return 0;
}

//suggest this API be invoked after MApi_IPA_Pool_Init.
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Notify(struct IPA_Pool_Polling_Param *polling_param)
{
#if 1

    printk("[%s:%d]error message no need and can not  realize %s for kernel mode utopia ,please use %s in user mode!!!!  \n",__FUNCTION__,__LINE__,__FUNCTION__,__FUNCTION__);    
    BUG();
    return false;    

#else

    MS_BOOL ret = TRUE;
    struct task_struct  *IPAPolling_struct = NULL;
    MS_U32 idx = 0;
    
    printk("%s\n",__FUNCTION__);
    mutex_lock(&_IPA_POOL_Mutex);
    if(!polling_param->polling_callback)
    {
        printk("%u have no polling_callback function , %s return directly\n",polling_param->pool_handle_id,__FUNCTION__);
        mutex_unlock(&_IPA_POOL_Mutex);
        return TRUE;
    }

    ret = _findPoolHandleId_InIPA_Pool_Table(polling_param->pool_handle_id, &idx);
    
    if(ret == FALSE)//not find idx
    {
         printk(" %s ipa pool fail not find pool_handle_id %u in pool table\n",__FUNCTION__,polling_param->pool_handle_id);
         mutex_unlock(&_IPA_POOL_Mutex);
         return FALSE;
    }

    if(-1 == IPAPool_Info[idx].pthIPAPollingId) //no polling id refer to pool handle id
    {
    
        struct IPA_Pool_GetIpcHandle_Param getipchandle_param;
        getipchandle_param.pool_handle_id = polling_param->pool_handle_id;

        ret = MApi_IPA_Pool_GETIPCHANDLE(&getipchandle_param);
        if(ret == FALSE)
        {
                 mutex_unlock(&_IPA_POOL_Mutex);
            return ret;
        }

        //N.B. use kthread_create to create a thread,do not use kernel_thread. 
        //For kernel_thread use in ko cause many issues in our test.samson.huang 20151105
        IPAPolling_struct = kthread_create(IPA_Pool_Polling_Task,(void *)(&(getipchandle_param.ipc_handle_id)),"iap_polling");
        if (!IS_ERR(IPAPolling_struct))
        {
            wake_up_process(IPAPolling_struct);
            printk("%s:%d tmp_struct=%p\n",__FUNCTION__,__LINE__,IPAPolling_struct);
        }
        else
        {
            printk("%s:%d fail\n",__FUNCTION__,__LINE__);
        }
        printk("%s:%d  debug\n",__FUNCTION__,__LINE__);
    
        if(IS_ERR(IPAPolling_struct))
        {
            printk("%s   can't create thread  \n",__FUNCTION__);
            ret= FALSE;
            mutex_unlock(&_IPA_POOL_Mutex);
            return ret;
        }
        else
        {
            //corresponding pthIPAPollingId to pool_handle_id  
            IPAPool_Info[idx].pthIPAPollingId = get_task_pid(IPAPolling_struct, PIDTYPE_PID);
            IPAPool_Info[idx].polling_callback = polling_param->polling_callback;
#ifdef DEBUG_IPA_KERNEL_MODE
            printk("%s:%d  kernel_thread success  pthIPAPollingId=%d\n",__FUNCTION__,__LINE__,IPAPool_Info[idx].pthIPAPollingId);
#endif
        }
    }
    else//already have polling id refer to pool handle id.
    {
        ret=FALSE;
        printk("pool handle id %u has its polling id %lu ,error,will not set callback again\n",polling_param->pool_handle_id,IPAPool_Info[idx].pthIPAPollingId);
    }

    mutex_unlock(&_IPA_POOL_Mutex);
#ifdef DEBUG_IPA_KERNEL_MODE
    printk("%s:%d  success ret=%d\n",__FUNCTION__,__LINE__,ret);
#endif    
    return ret;

#endif
}

#if (TEST_USE_SELF_KO_MAKEFILE || TEST_USE_SELF_KO_MAKEFILE_TOTAL)//these just test code!!!
#define DFB_TEMP_HID 27

#define PAGE 4096
#define MILLION (1<<20)

static char * Pool_Name1 = "DFB_TEMP";
static char * Pool_Name1_2 = "DFB_TEMP_2";
static char * Pool_Name1_3 = "DFB_TEMP_3";

void *DFB_callback_id_1(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length)
{
    printk("inside %s ,pool_handle_id=%u\n",__FUNCTION__,pool_handle_id);
}

int test_case_2()
{
    bool err;
    MS_U32 timeout_ms = 2;
    printk("debug in %s \n",__FUNCTION__);
  
#if 1
    printk("[%s:%d] will MApi_IPA_Pool_Init\n",__FUNCTION__,__LINE__);

    struct IPA_Pool_Init_Param Init_Param;
    Init_Param.heap_id= DFB_TEMP_HID;
    Init_Param.pool_name = Pool_Name1;
    printk("%s:%d Pool_Name1=%p,Init_Param.pool_name=%s\n",__FUNCTION__,__LINE__,Pool_Name1,Init_Param.pool_name);
    Init_Param.offset_in_heap = 6.5*MILLION;
    Init_Param.len = 8*MILLION;

#if DEBUG_IPA_KERNEL_MODE
    printk("[%s:%d]Init_Param->heap_id=%u,Init_Param->offset=0x%llx,Init_Param->len=0x%llx\n",__FUNCTION__,__LINE__,Init_Param.heap_id
                             ,Init_Param.offset_in_heap,Init_Param.len);
    printk("%s:%d Init_Param.pool_name=%s\n",__FUNCTION__,__LINE__,Init_Param.pool_name);
#endif

    err = MApi_IPA_Pool_Init(&Init_Param);
    if(err ==FALSE)
    {
        printk("[%s:%d] ipa_init fail  Init_Param->heap_id=%u,Init_Param->offset=0x%llx,Init_Param->len=0x%llx\n",__FUNCTION__,__LINE__,Init_Param.heap_id
                          ,Init_Param.offset_in_heap,Init_Param.len);
        return -1;
    }

    printk("[%s:%d] will ipa_alloc\n",__FUNCTION__,__LINE__);
    struct IPA_Pool_GetMem_Param get_param;
    get_param.pool_handle_id=Init_Param.pool_handle_id;
    get_param.offset_in_pool=0;
    get_param.length= PAGE;
    err = MApi_IPA_Pool_GetMem_Timeout(&get_param,timeout_ms);
    printk("[%s:%d] after ipa_alloc   get_param.pool_handle_id=%u\n",__FUNCTION__,__LINE__,get_param.pool_handle_id);
    if(err ==FALSE)
    {
        printk("[%s:%d] err return\n",__FUNCTION__,__LINE__);
        return -1;
    }
#endif

    struct IPA_Pool_Polling_Param polling_param;
    polling_param.pool_handle_id = Init_Param.pool_handle_id;
    printk("[%s:%d] \n",__FUNCTION__,__LINE__);
    polling_param.polling_callback=DFB_callback_id_1;
    printk("[%s:%d]   polling_param.pool_handle_id=%u\n",__FUNCTION__,__LINE__,polling_param.pool_handle_id);
    MApi_IPA_Pool_Notify(&polling_param);
    printk("[%s:%d] \n",__FUNCTION__,__LINE__);

     //must delay before free to ensure polling having sleep¡£
     printk("before sleep\n");

#if 0
    while(1);
#else
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    /*schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));
    schedule_timeout_interruptible(msecs_to_jiffies(1000));   */
     
    printk("after sleep\n");
    printk("[%s:%d] before MApi_IPA_Pool_PutMem\n",__FUNCTION__,__LINE__);  
    struct IPA_Pool_PutMem_Param put_param;
    put_param.length=PAGE;
    put_param.offset_in_pool=0;
    put_param.pool_handle_id =Init_Param.pool_handle_id;
    err = MApi_IPA_Pool_PutMem(&put_param);
    if(err ==FALSE)
    {
        printk("[%s:%d] err return\n",__FUNCTION__,__LINE__);
        return -1;
    }

#if 1	
    printk(" %s  before MApi_IPA_Pool_Release \n",__FUNCTION__);
    err = MApi_IPA_Pool_Release(Init_Param.pool_handle_id);
    printk(" %s  after MApi_IPA_Pool_Release  err=%d\n",__FUNCTION__,err);
    if(err == FALSE)
    {
        printk("[%s:%d] err return\n",__FUNCTION__,__LINE__);
        return -1;
    }
#endif

#endif

    return 0;
}

#endif
#if TEST_USE_SELF_KO_MAKEFILE

static int drvIPAPool_init(void)
{    
    printk(KERN_ERR"%s !!! \n",__FUNCTION__);	

    //test_case_2();

    return 0;
}

static void drvIPAPool_exit(void)
{    
    printk(KERN_ERR"%s !!! \n",__FUNCTION__);
}

module_init(drvIPAPool_init);
module_exit(drvIPAPool_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mstar");
MODULE_DESCRIPTION("A module used for drvIPAPool!");
#endif


#endif
