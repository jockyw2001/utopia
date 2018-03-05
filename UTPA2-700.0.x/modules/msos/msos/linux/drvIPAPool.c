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
#if defined (MSOS_TYPE_LINUX)
#include<sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <poll.h>
#include <pthread.h>
#include<fcntl.h>
#include <unistd.h>
#include <string.h>
#include "MsCommon.h"
#include "halCHIP.h"
#include "halMPool.h"
#include "drvIPAPool.h"
#include "mdrv_ipa_pool_uapi.h"

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
    volatile MS_U32 polling_thread_delete_task_flag;
    pthread_t pthIPAPollingId;
    void (*polling_callback)(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length);
} IPAPOOL_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_S32 _s32FdIPAPool = -1;
static pthread_mutex_t  _IPA_POOL_Mutex = PTHREAD_MUTEX_INITIALIZER;
static IPAPOOL_INFO IPAPool_Info[MAX_IPAPOOLSIZE];

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
        VPRINTF("Not enough IPAPool, must increase MAX_IPAPOOLSIZE!!\n");
    
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
			&&(!strncmp((char *)(intptr_t)IPAPool_Info[i].Init_Param.pool_name, Init_Param->pool_name,strlen(Init_Param->pool_name)))
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

//N.B.  This API only for each module debug code use,in each module release code,please do not call it !!!
//in:pa value
//out:whether in miu/heap/pool,and info about miu/heap/pool,and if in pool whether allocated.
//return value:only allocated in pool will return TRUE,otherwise return FALSE.
MS_BOOL __attribute__((weak)) PA_in_IPA_POOL_info(struct PA_In_IPA_Pool_Param * in_ipa_pool_info)
{
    MS_BOOL ret = FALSE;
    int res = 0;
    struct PA_In_IPA_Pool_Args in_ipa_pool_info_args;
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool <= 0)//may never open device
    {
        if ((_s32FdIPAPool = open("/dev/ipapool", O_RDWR)) < 0)
        {
            VPRINTF("open /dev/ipapool fail\n");
            ret =  FALSE;
            goto PA_in_IPA_POOL_info;
        }

        memset(IPAPool_Info, 0, sizeof(IPAPOOL_INFO)*MAX_IPAPOOLSIZE);
    }
    in_ipa_pool_info_args.PA = in_ipa_pool_info->PA;
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_PA_INFO, &in_ipa_pool_info_args);
    if (res < 0 ||  in_ipa_pool_info_args.error_code < 0) 
    {
        ret =  FALSE;//ioctl fail,return false.
        VPRINTF("ipa error: %s , PA 0x%lx,res=%d ,error_code=0x%x\n",__FUNCTION__, (unsigned long)in_ipa_pool_info_args.PA,res,in_ipa_pool_info_args.error_code);
        goto PA_in_IPA_POOL_info;     

    }
    VPRINTF(" %s  PA:0x%lx   pa_state=%d\n",__FUNCTION__, (unsigned long)in_ipa_pool_info_args.PA,in_ipa_pool_info_args.pa_state);

     in_ipa_pool_info->pa_state= in_ipa_pool_info_args.pa_state;
     in_ipa_pool_info->miu= in_ipa_pool_info_args.miu;
     if(in_ipa_pool_info->miu < 0)
     {
        VPRINTF("ipa error: %s failed, PA 0x%lx,miu=%d\n",__FUNCTION__, (unsigned long)in_ipa_pool_info_args.PA,in_ipa_pool_info_args.miu);
        in_ipa_pool_info->in_heap = FALSE;
        in_ipa_pool_info->allocated = FALSE;
        ret =  TRUE;//ioctl success,but not in miu,return TRUE!!!
        goto PA_in_IPA_POOL_info;     
     }
     in_ipa_pool_info->in_heap= in_ipa_pool_info_args.in_heap;
     if(FALSE == in_ipa_pool_info->in_heap)
     {
        VPRINTF("ipa error: %s failed, PA 0x%lx,miu=%d,in_heap is FALSE\n",__FUNCTION__, (unsigned long)in_ipa_pool_info_args.PA,in_ipa_pool_info_args.miu);
        in_ipa_pool_info->allocated = FALSE;
        ret =  TRUE;//ioctl success,but not in heap,return TRUE!!!
        goto PA_in_IPA_POOL_info;       
     }
     else
     {
         in_ipa_pool_info->space_id= in_ipa_pool_info_args.heap_id;
         in_ipa_pool_info->space_type= in_ipa_pool_info_args.heap_type;
         in_ipa_pool_info->space_miu_start_offset= in_ipa_pool_info_args.heap_miu_start_offset;
         in_ipa_pool_info->space_length= in_ipa_pool_info_args.heap_length;
         in_ipa_pool_info->pa_offset_in_heap= in_ipa_pool_info_args.pa_offset_in_heap;
        
         in_ipa_pool_info->allocated = in_ipa_pool_info_args.allocated;
         if(FALSE == in_ipa_pool_info->allocated)
         {
             VPRINTF("ipa error: %s failed, PA 0x%lx,miu=%d,in_ipa_pool_info_args.heap_id=%d,allocated is FALSE\n",__FUNCTION__, (unsigned long)in_ipa_pool_info_args.PA,in_ipa_pool_info_args.miu,in_ipa_pool_info_args.heap_id);
             ret =  TRUE;//ioctl success,but not in pool(not allocated),return TRUE!!!
             goto PA_in_IPA_POOL_info;                    
         }
         else
         {
             //in_ipa_pool_info->pool_handle_id= in_ipa_pool_info_args.pool_handle_id;
#if defined (__aarch64__)
            strcpy((char *)in_ipa_pool_info->pool_name , (char *)in_ipa_pool_info_args.pool_name);
#else
            strcpy((char *)in_ipa_pool_info->pool_name ,(char *)in_ipa_pool_info_args.pool_name);
#endif
            in_ipa_pool_info->pool_len = in_ipa_pool_info_args.pool_len;
            in_ipa_pool_info->pool_offset_in_heap = in_ipa_pool_info_args.pool_offset_in_heap;
            in_ipa_pool_info->pa_offset_in_pool = in_ipa_pool_info_args.pa_offset_in_pool;
        }
    }
    ret =  TRUE;

PA_in_IPA_POOL_info:
    
   pthread_mutex_unlock(&_IPA_POOL_Mutex);
   return ret;
}

MS_BOOL __attribute__((weak)) IN_IPA_POOL_To_PA(struct Pool_To_PA_Param * pool_to_pa_param)
{
    MS_BOOL ret = TRUE;
    struct Pool_To_PA_Args pool_to_pa_args;
    int res = 0;

    VPRINTF("%s handle_id 0x%x offset_in_pool 0x%lx\n", 
        __FUNCTION__,pool_to_pa_param->pool_handle_id, (unsigned long)pool_to_pa_param->offset_in_pool);
    
    pthread_mutex_lock(&_IPA_POOL_Mutex);  
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto POOL_To_PA_DONE;
    }

    //input
    pool_to_pa_args.pool_handle_id = pool_to_pa_param->pool_handle_id;  
    pool_to_pa_args.offset_in_pool = pool_to_pa_param->offset_in_pool;
    
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_POOL_TO_PA, &pool_to_pa_args);
    if (res < 0 || pool_to_pa_args.error_code != IPAERROR_OK)
    {  
        VPRINTF("%s fail: pool_handle_id %u, offset 0x%lx, error_code=0x%x  res=%d\n",__FUNCTION__, pool_to_pa_param->pool_handle_id, 
            (unsigned long)pool_to_pa_param->offset_in_pool,pool_to_pa_args.error_code,res);
        ret = FALSE;
        goto POOL_To_PA_DONE;
    }

    //output
    pool_to_pa_param->PA = pool_to_pa_args.PA;
    pool_to_pa_param->error_code = pool_to_pa_args.error_code;
    pool_to_pa_param->miu = pool_to_pa_args.miu;
    pool_to_pa_param->heap_id = pool_to_pa_args.heap_id;
    
POOL_To_PA_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    
    return ret;
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
    struct IPA_Pool_Init_Args ipa_init_args;
    MS_U64 u64PhyAddr = 0;
    MS_BOOL ret = TRUE;
    MS_U32 idx = 0;
    int res = 0;
    //VPRINTF("%s start\n",__FUNCTION__);
    VPRINTF("%s heap_id %u\n",__FUNCTION__, Init_Param->space_id);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool <= 0)
    {
        if ((_s32FdIPAPool = open("/dev/ipapool", O_RDWR)) < 0)
        {
            VPRINTF("open /dev/ipapool fail\n");
            ret =  FALSE;
            goto IPA_POOL_INIT_DONE;
        }

        memset(IPAPool_Info, 0, sizeof(IPAPOOL_INFO)*MAX_IPAPOOLSIZE);
    }


    //avoid mmap more than one time
    ret = _findHeapId_InIPA_Pool_Table(Init_Param, &idx);
    if(ret == TRUE)
    {
         //memcpy(Init_Param,IPAPool_Info[idx].Init_Param,sizeof(struct IPA_Pool_Init_Param));
         Init_Param->space_id = IPAPool_Info[idx].Init_Param.heap_id;
         Init_Param->pool_name = (char *)(intptr_t)IPAPool_Info[idx].Init_Param.pool_name;
         Init_Param->offset_in_heap = IPAPool_Info[idx].Init_Param.offset_in_heap;
         Init_Param->len = IPAPool_Info[idx].Init_Param.len;
         Init_Param->pool_handle_id = IPAPool_Info[idx].Init_Param.pool_handle_id;
         Init_Param->miu = IPAPool_Info[idx].Init_Param.miu;
         Init_Param->space_type = IPAPool_Info[idx].Init_Param.heap_type;
         Init_Param->error_code = IPAPool_Info[idx].Init_Param.error_code;
         Init_Param->space_length = IPAPool_Info[idx].Init_Param.heap_length;
         Init_Param->space_miu_start_offset = IPAPool_Info[idx].Init_Param.heap_miu_start_offset;

         VPRINTF("pool_handle_id %u already init!\n", IPAPool_Info[idx].Init_Param.pool_handle_id);
         goto IPA_POOL_INIT_DONE;
    }

    ipa_init_args.heap_id = Init_Param->space_id;
#if defined (__aarch64__)
    strcpy((char *)ipa_init_args.pool_name , Init_Param->pool_name);
#else
    strcpy((char *)ipa_init_args.pool_name ,Init_Param->pool_name);
#endif
    ipa_init_args.offset_in_heap = Init_Param->offset_in_heap;
    ipa_init_args.len = Init_Param->len;	
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_INIT, &ipa_init_args);
    if (res < 0 || ipa_init_args.error_code != IPAERROR_OK) 
    {
        VPRINTF("ipa error: ipa init failed, heapid %u error_code %d\n", Init_Param->space_id, ipa_init_args.error_code);
        ret =  FALSE;
        goto IPA_POOL_INIT_CLOSE;
    }

    VPRINTF("%s heap_id %u, pool_handle_id %u heap_length %llu\n", 
		__FUNCTION__,Init_Param->space_id, ipa_init_args.pool_handle_id, (long long unsigned int)ipa_init_args.heap_length);
	
    Init_Param->pool_handle_id = ipa_init_args.pool_handle_id;
    Init_Param->miu = ipa_init_args.miu;
    Init_Param->space_type = ipa_init_args.heap_type;
    Init_Param->error_code = ipa_init_args.error_code;
    Init_Param->space_length = ipa_init_args.heap_length;
    Init_Param->space_miu_start_offset = ipa_init_args.heap_miu_start_offset;
    //VPRINTF("%s before _findEmpty_IPA_Pool_Entry\n",__FUNCTION__);
    ret = _findEmpty_IPA_Pool_Entry(&idx);
    //VPRINTF("%s after _findEmpty_IPA_Pool_Entry\n",__FUNCTION__);
    if(ret == FALSE)
    {
        VPRINTF("ipa error: pool_handle_id %u init failed!\n", ipa_init_args.pool_handle_id);
        goto IPA_POOL_INIT_DONE;
    }
    //VPRINTF("%s before _miu_offset_to_phy\n",__FUNCTION__);
    _miu_offset_to_phy(ipa_init_args.miu, ipa_init_args.heap_miu_start_offset, u64PhyAddr); // get miu base addr
    //VPRINTF("%s after _miu_offset_to_phy   idx=%d  ",__FUNCTION__,idx);
    
    
    #if defined (__aarch64__)//make sure when build no warning.
    VPRINTF("sizeof(struct IPA_Pool_Init_Param)=%lu\n",sizeof(struct IPA_Pool_Init_Param));
    #else
    VPRINTF("sizeof(struct IPA_Pool_Init_Param)=%u\n",sizeof(struct IPA_Pool_Init_Param));
    #endif
    	
    /* IPA Pool setting*/
    //VPRINTF("%s before memcpy\n",__FUNCTION__);
    //memcpy(IPAPool_Info[idx].Init_Param,Init_Param,sizeof(struct IPA_Pool_Init_Param));
#if 1
    //VPRINTF("%s before heap_id\n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.heap_id = Init_Param->space_id;

    //VPRINTF("%s before pool_name \n",__FUNCTION__);
#if defined (__aarch64__)
    IPAPool_Info[idx].Init_Param.pool_name = (MS_U64)Init_Param->pool_name;
#else
    IPAPool_Info[idx].Init_Param.pool_name = (MS_U32)Init_Param->pool_name;
#endif
    //VPRINTF("%s before offset \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.offset_in_heap =Init_Param->offset_in_heap;
    //VPRINTF("%s before len \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.len =Init_Param->len;
    //VPRINTF("%s before pool_handle_id \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.pool_handle_id =Init_Param->pool_handle_id;
    //VPRINTF("%s before miu \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.miu =Init_Param->miu;
    //VPRINTF("%s before heap_type \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.heap_type =Init_Param->space_type;
    //VPRINTF("%s before error_code \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.error_code =Init_Param->error_code;
    //VPRINTF("%s before heap_length \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.heap_length =Init_Param->space_length;
    //VPRINTF("%s before heap_miu_start_offset \n",__FUNCTION__);
    IPAPool_Info[idx].Init_Param.heap_miu_start_offset =Init_Param->space_miu_start_offset;
#endif	

    //VPRINTF("%s after memcpy\n",__FUNCTION__);
    IPAPool_Info[idx].bIsUsed = TRUE; 
    IPAPool_Info[idx].pthIPAPollingId = -1;//in init,no polling id yet.
    IPAPool_Info[idx].polling_thread_delete_task_flag = 0;

    VPRINTF("%s heap_id %u pool_handle_id %u miu %u offset 0x%lx len 0x%lx  idx=%u\n",__FUNCTION__,
		Init_Param->space_id, Init_Param->pool_handle_id, Init_Param->miu, (unsigned long)Init_Param->offset_in_heap, (unsigned long)Init_Param->len,idx);
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    //VPRINTF("%s  before return %d\n",__FUNCTION__,ret);
    return ret;

IPA_POOL_INIT_CLOSE:
    //VPRINTF("%s  before close\n",__FUNCTION__);
    close(_s32FdIPAPool);
    //VPRINTF("%s  after close\n",__FUNCTION__);
IPA_POOL_INIT_DONE:
    //VPRINTF("%s  after IPA_POOL_INIT_DONE  ret=%d\n",__FUNCTION__,ret);
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
	
    return ret;
}

//MS_U32 u32flag. Special Flag for customer mem allocation
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem(struct IPA_Pool_GetMem_Param * get_param)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_Alloc_Args ipa_alloc_args;
    int res = 0;

    //VPRINTF("%s handle_id 0x%x length 0x%lx\n",__FUNCTION__,
	//	get_param->pool_handle_id, (unsigned long)get_param->length);

    pthread_mutex_lock(&_IPA_POOL_Mutex);  
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    ipa_alloc_args.pool_handle_id = get_param->pool_handle_id;	
    ipa_alloc_args.offset_in_pool = get_param->offset_in_pool;
    ipa_alloc_args.length = get_param->length;
    ipa_alloc_args.timeout = 0;// 0 means if fail ,will not try again
    VPRINTF("%s %d pool_handle_id 0x%x [offset_in_pool ,length]=[0x%lx,0x%lx]\n",__FUNCTION__,__LINE__,
		ipa_alloc_args.pool_handle_id,(unsigned long)ipa_alloc_args.offset_in_pool, (unsigned long)ipa_alloc_args.length);
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_ALLOC, &ipa_alloc_args);
    if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
    {  
        VPRINTF("%s fail: pool_handle_id %u, offset 0x%lx, len 0x%lx  ipa_alloc_args.error_code=0x%x  res=%d\n",__FUNCTION__, get_param->pool_handle_id,
			(unsigned long)get_param->offset_in_pool, (unsigned long)get_param->length,ipa_alloc_args.error_code,res);
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    get_param->error_code = ipa_alloc_args.error_code;
    
IPA_POOL_GETMEM_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_PutMem(struct IPA_Pool_PutMem_Param * put_param)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_free_Args ipa_free_args;
    
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if(_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_PUTMEM_DONE;
    }
	
    ipa_free_args.pool_handle_id = put_param->pool_handle_id;
    ipa_free_args.offset_in_pool = put_param->offset_in_pool;
    ipa_free_args.length = put_param->length;
    VPRINTF("%s %d pool_handle_id 0x%x [offset_in_pool,length]=[0x%lx,0x%lx]\n", __FUNCTION__,__LINE__,
		ipa_free_args.pool_handle_id, (unsigned long)ipa_free_args.offset_in_pool, (unsigned long)ipa_free_args.length);

    if(ioctl(_s32FdIPAPool, IPA_POOL_IOC_FREE, &ipa_free_args))
    {        
        VPRINTF("%s fail: pool_handle_id 0x%x, offset 0x%lx, len 0x%lx\n",__FUNCTION__,
			put_param->pool_handle_id, (unsigned long)put_param->offset_in_pool,(unsigned long)put_param->length);
        ret = FALSE;
        goto IPA_POOL_PUTMEM_DONE;
    }

IPA_POOL_PUTMEM_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Release(MS_U32 pool_handle_id)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_Deinit_Args deinit_args;
    struct IPA_Pool_Unmap_Args unmap_args;
    
    MS_U32 idx = 0;
    int res = 0,i=0;

    VPRINTF("%s handle_id %u\n",__FUNCTION__, pool_handle_id);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_RELEASE_DONE;
    }

    ret = _findPoolHandleId_InIPA_Pool_Table(pool_handle_id, &idx);
    if(ret == FALSE)
    {
        VPRINTF("pool_handle_id %u _findPoolHandleId_InIPA_Pool_Table fail\n", pool_handle_id);
        goto IPA_POOL_RELEASE_DONE;
    }

    for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
    {
        // tmp_map_info
        if((0 !=IPAPool_Info[idx].map_info[i].virt_addr)
		    && (0 != IPAPool_Info[idx].map_info[i].length))
        {
            unmap_args.virt_addr =	IPAPool_Info[idx].map_info[i].virt_addr;
            unmap_args.length = IPAPool_Info[idx].map_info[i].length;

            MsOS_MPool_Remove_PA2VARange(IPAPool_Info[idx].map_info[i].Physaddr, IPAPool_Info[idx].map_info[i].virt_addr, IPAPool_Info[idx].map_info[i].length, IPAPool_Info[idx].map_info[i].bNonCache);
            res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_UNMAP, &unmap_args);
            if (res < 0)
            {
                //give printf,but not end.
                VPRINTF("error: pool_handle_id %u unmap failed\n", pool_handle_id);
            }

            IPAPool_Info[idx].map_info[i].virt_addr = 0;
            IPAPool_Info[idx].map_info[i].length = 0;
        }
    }

    IPAPool_Info[idx].bIsUsed = FALSE;
    IPAPool_Info[idx].Init_Param.pool_handle_id = 0;

    IPAPool_Info[idx].polling_thread_delete_task_flag = 2; //request thread to exit
    deinit_args.pool_handle_id = pool_handle_id;
    
    if (ioctl(_s32FdIPAPool, IPA_POOL_IOC_DEINIT, &deinit_args))
    {        
        VPRINTF("pool_handle_id %u deinit fail\n", pool_handle_id);            
        ret = FALSE;
        goto IPA_POOL_RELEASE_DONE;
    }
    ret = TRUE; //not find the pool hand id

    //after deinit, if have polling thread,should delete task
    if(IPAPool_Info[idx].pthIPAPollingId != -1)
    {
        #if 0//no need this while,for later pthread_join + pthread_cancel will wait for set delete_task_flag.
        while(1)
        {
            if(1 == IPAPool_Info[idx].polling_thread_delete_task_flag)
                break;
        }
        #endif

        //N.B. here we do not directly use MsOS_DeleteTask but use pthread_join + pthread_cancel,
        //For in MsOS_DeleteTask,first use pthread_cancel and then use pthread_join and that will 
        //cause hang.
        //MsOS_DeleteTask (IPAPool_Info[idx].pthIPAPollingId);//delete polling task
        pthread_join(IPAPool_Info[idx].pthIPAPollingId, NULL);

        #if 0
        //build SN lib can find pthread_cancel,
        //but build AN lib,can not find pthread_cancel,
        //in fact only need pthread_join,no need pthread_cancel.
        pthread_cancel(IPAPool_Info[idx].pthIPAPollingId);//delete polling task 
        #endif

        if(1 != IPAPool_Info[idx].polling_thread_delete_task_flag)
        {
            VPRINTF("error !!!!!!   after pthread_join  polling_thread_delete_task_flag is not 1!!!  idx=%u  IPAPool_Info[%d].polling_thread_delete_task_flag=%u\n",idx,idx,IPAPool_Info[idx].polling_thread_delete_task_flag);
            ret = FALSE;
        }

    }


IPA_POOL_RELEASE_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    return ret;
}

//allow a same input [pool_handle_id, offset_in_pool, length, cache_type] map for more than once,
//and get different result output [virt_addr]
static MS_BOOL MApi_IPA_Pool_MapUserVA(struct IPA_Pool_Map_Param * map_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    struct IPA_Pool_Map_Args map_args;
    MS_U32 idx = 0;
    VPRINTF("%s pool_handle_id %u, offset_in_pool 0x%lx, len 0x%lx,  cache_type %u\n",__FUNCTION__, map_param->pool_handle_id,
		(unsigned long)map_param->offset_in_pool, (unsigned long)map_param->length,map_param->cache_type);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }

    if(0 == map_param->length)
    {
        VPRINTF("%s  error, len 0x%lx is invalid\n",__FUNCTION__,  (unsigned long)map_param->length);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;       
    }
	
    map_args.pool_handle_id = map_param->pool_handle_id;
    map_args.offset_in_pool = map_param->offset_in_pool;
    map_args.length = map_param->length;
    map_args.map_va_type = map_param->cache_type;
	
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_MAP, &map_args);
    //VPRINTF("MApi_IPA_Pool_MapUserVA after ioctl\n");
    if (res < 0 || map_args.error_code != IPAERROR_OK)
    {  
        VPRINTF("%s  fail: pool_handle_id %u, offset_in_pool 0x%lx, len 0x%lx,  cache_type %u\n",__FUNCTION__, map_param->pool_handle_id,
			(unsigned long)map_param->offset_in_pool, (unsigned long)map_param->length,map_param->cache_type);
        ret = FALSE;
        goto IPA_POOL_MapUserVA_DONE;
    }
    //VPRINTF("MApi_IPA_Pool_MapUserVA before virt_addr\n");
    map_param->virt_addr = map_args.virt_addr;
    map_param->error_code = map_args.error_code;
    //VPRINTF("MApi_IPA_Pool_MapUserVA before _findPoolHandleId_InIPA_Pool_Table\n");

    ret = _findPoolHandleId_InIPA_Pool_Table(map_param->pool_handle_id, &idx);
    //VPRINTF("MApi_IPA_Pool_MapUserVA after _findPoolHandleId_InIPA_Pool_Table\n");
    if(ret == TRUE)
    {
        int i=0;
        //VPRINTF("MApi_IPA_Pool_MapUserVA before for\n");
        for(i=0;i<MAX_CLIENT_MAP_NUM;i++)
        {
            //VPRINTF("MApi_IPA_Pool_MapUserVA before if\n");
            if((0 == IPAPool_Info[idx].map_info[i].virt_addr)
                    || (0 == IPAPool_Info[idx].map_info[i].length))
            {
                //VPRINTF("MApi_IPA_Pool_MapUserVA inside if\n");
                IPAPool_Info[idx].map_info[i].virt_addr  = map_args.virt_addr;
                IPAPool_Info[idx].map_info[i].length = map_args.length;
                //VPRINTF("MApi_IPA_Pool_MapUserVA before _miu_offset_to_phy\n");
                _miu_offset_to_phy(IPAPool_Info[idx].Init_Param.miu, IPAPool_Info[idx].Init_Param.offset_in_heap + map_param->offset_in_pool, IPAPool_Info[idx].map_info[i].Physaddr); // get miu base addr
                if(IPA_VA_CACHE_NONE_CACHE_Param == map_param->cache_type)
                    IPAPool_Info[idx].map_info[i].bNonCache = TRUE;
                else
                    IPAPool_Info[idx].map_info[i].bNonCache = FALSE;
                //VPRINTF("MApi_IPA_Pool_MapUserVA before MsOS_MPool_Add_PA2VARange\n");
                MsOS_MPool_Add_PA2VARange(IPAPool_Info[idx].map_info[i].Physaddr, IPAPool_Info[idx].map_info[i].virt_addr, IPAPool_Info[idx].map_info[i].length, IPAPool_Info[idx].map_info[i].bNonCache);
                break;
            }
        }
        //VPRINTF("MApi_IPA_Pool_MapUserVA after for\n");
        if(MAX_CLIENT_MAP_NUM == i)
        {
            VPRINTF("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            VPRINTF("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            VPRINTF("print important log for 3 times :a same client mapped %lu times,unable  map once more!!!!\n", MAX_CLIENT_MAP_NUM);
            //here later can add code for dump map info
            ret = FALSE;
            goto IPA_POOL_MapUserVA_DONE;		   
        }
    }
    //VPRINTF("MApi_IPA_Pool_MapUserVA before IPA_POOL_MapUserVA_DONE\n");

IPA_POOL_MapUserVA_DONE:
    //VPRINTF("MApi_IPA_Pool_MapUserVA after IPA_POOL_MapUserVA_DONE\n");
    pthread_mutex_unlock(&_IPA_POOL_Mutex);  	
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_MapVA(struct IPA_Pool_Map_Param * map_param)
{
    return MApi_IPA_Pool_MapUserVA(map_param);
}

static void MApi_IPA_Pool_UnmapUserVA(struct IPA_Pool_Unmap_Param * unmap_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    int i=0,idx=0,find_virt_info_i=-1,find_virt_info_idx=-1;
    MS_BOOL find_virt_info = FALSE;
    struct IPA_Pool_Unmap_Args unmap_args;
    VPRINTF("%s   virt_addr 0x%lx, len 0x%lx \n",__FUNCTION__,
		(unsigned long)unmap_param->virt_addr, (unsigned long)unmap_param->length);	
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
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
        VPRINTF("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%lx,length=0x%lx\n", (unsigned long)unmap_param->virt_addr,(unsigned long)unmap_param->length);
        VPRINTF("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%lx,length=0x%lx\n", (unsigned long)unmap_param->virt_addr,(unsigned long)unmap_param->length);
        VPRINTF("print important log for 3 times :map and unmap area should be strict equal !!!!  virt_addr=0x%lx,length=0x%lx\n", (unsigned long)unmap_param->virt_addr,(unsigned long)unmap_param->length);
        //later will add some debug code
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    }
    unmap_args.virt_addr = unmap_param->virt_addr;
    unmap_args.length = unmap_param->length;
    MsOS_MPool_Remove_PA2VARange(IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].Physaddr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length, IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].bNonCache); 	   
    res = ioctl(_s32FdIPAPool,IPA_POOL_IOC_UNMAP,&unmap_args);
    if (res < 0)
    {
        VPRINTF("%s  fail:  virt_addr 0x%lx, len 0x%lx \n",__FUNCTION__,
			(unsigned long)unmap_param->virt_addr, (unsigned long)unmap_param->length);
        ret = FALSE;
        goto IPA_POOL_UnmapUserVA_DONE;
    } 
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].virt_addr = 0;
    IPAPool_Info[find_virt_info_idx].map_info[find_virt_info_i].length = 0;
	
IPA_POOL_UnmapUserVA_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex); 

    return;
}

void  __attribute__((weak)) MApi_IPA_Pool_UnmapVA(struct IPA_Pool_Unmap_Param * unmap_param)
{
    return MApi_IPA_Pool_UnmapUserVA(unmap_param);
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_DCacheFlush(struct IPA_Pool_DCacheFlush_Param* dcache_flush_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    struct IPA_Pool_DCacheFlush_Args dcache_flush_args;
    VPRINTF("%s  virt_addr 0x%lx, length 0x%lx flush_type 0x%x\n",__FUNCTION__,
		(unsigned long)dcache_flush_param->virt_addr, (unsigned long)dcache_flush_param->length, dcache_flush_param->flush_type);	
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_DCacheFlush_DONE;
    }

    dcache_flush_args.virt_addr = dcache_flush_param->virt_addr;
    dcache_flush_args.length = dcache_flush_param->length;
    dcache_flush_args.flush_type = dcache_flush_param->flush_type;
    res = ioctl(_s32FdIPAPool,IPA_POOL_IOC_FLUSH,&dcache_flush_args);
    if (res < 0)
    {
        VPRINTF("%s  fail:  virt_addr 0x%lx, length 0x%lx flush_type 0x%x\n",__FUNCTION__,
		(unsigned long)dcache_flush_param->virt_addr, (unsigned long)dcache_flush_param->length, dcache_flush_param->flush_type);
        ret = FALSE;
        goto IPA_POOL_DCacheFlush_DONE;
    }

IPA_POOL_DCacheFlush_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex); 		
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_HEAP_ATTR(struct IPA_Pool_Heap_Attr_Param* heap_attr_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    struct IPA_Pool_Heap_Attr heap_attr_args;
    VPRINTF("%s  heap_id 0x%x\n",__FUNCTION__, heap_attr_param->heap_id);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_HEAP_ATTR_DONE;
    }

    heap_attr_args.heap_id = heap_attr_param->heap_id;
    res = ioctl(_s32FdIPAPool,IPA_POOL_IOC_HEAP_ATTR,&heap_attr_args);
    if (res < 0 || heap_attr_args.error_code != IPAERROR_OK)
    {  
        VPRINTF("%s  fail:  heap_id 0x%x\n",__FUNCTION__,heap_attr_param->heap_id);
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
    pthread_mutex_unlock(&_IPA_POOL_Mutex); 		
    return ret;	
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GETIPCHANDLE(struct IPA_Pool_GetIpcHandle_Param* getipchandle_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    struct IPA_Pool_GetIpcHandle_Args getipchandle_args;
    VPRINTF("%s  pool_handle_id 0x%x\n",__FUNCTION__, getipchandle_param->pool_handle_id);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        VPRINTF("MApi_IPA_Pool_GETIPCHANDLE  fail  pool_handle_id=0x%x,   _s32FdIPAPool=%d \n", getipchandle_param->pool_handle_id,_s32FdIPAPool);      	
        ret = FALSE;
        goto IPA_POOL_GETIPCHANDLE_DONE;
    }

    getipchandle_args.pool_handle_id = getipchandle_param->pool_handle_id;
    res = ioctl(_s32FdIPAPool,IPA_POOL_IOC_GETIPCHANDLE,&getipchandle_args);
    if (res < 0 || getipchandle_args.error_code != IPAERROR_OK)
    {
        VPRINTF("%s  fail:  pool_handle_id 0x%x\n",__FUNCTION__,getipchandle_param->pool_handle_id);
        ret = FALSE;
        goto IPA_POOL_GETIPCHANDLE_DONE;
    } 
    getipchandle_param->ipc_handle_id = getipchandle_args.ipc_handle_id;
    getipchandle_param->error_code = getipchandle_args.error_code;

IPA_POOL_GETIPCHANDLE_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex); 		
    return ret;	
}

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_InstallIpcHandle(struct IPA_Pool_InstallIpcHandle_Param* installipchandle_param)
{
    MS_BOOL ret = TRUE;
    int res = 0;
    struct IPA_Pool_InstallIpcHandle_Args installipchandle_args;
    VPRINTF("%s  ipc_handle_id 0x%x\n",__FUNCTION__, installipchandle_param->ipc_handle_id);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        VPRINTF("%s ipc_handle_id=0x%x,   _s32FdIPAPool=%d \n",__FUNCTION__, installipchandle_param->ipc_handle_id,_s32FdIPAPool);
        //this process may not open ipapool yet,need we open for this process.
        if ((_s32FdIPAPool = open("/dev/ipapool", O_RDWR)) < 0)
        {
            VPRINTF("open /dev/ipapool fail   ipc_handle_id=0x%x,   _s32FdIPAPool=%d\n", installipchandle_param->ipc_handle_id,_s32FdIPAPool);
            ret =  FALSE;
            goto IPA_POOL_InstallIpcHandle_DONE;
        }

        memset(IPAPool_Info, 0, sizeof(IPAPOOL_INFO)*MAX_IPAPOOLSIZE);
        
    }

    installipchandle_args.ipc_handle_id = installipchandle_param->ipc_handle_id;
    // VPRINTF("MApi_IPA_Pool_InstallIpcHandle  ipc_handle_id=0x%x  debug will ioctl\n", installipchandle_param->ipc_handle_id);          
    res = ioctl(_s32FdIPAPool,IPA_POOL_IOC_INSTALLIPCHANDLE,&installipchandle_args);
    // VPRINTF("MApi_IPA_Pool_InstallIpcHandle  ipc_handle_id=0x%x  debug after ioctl  res=%d\n", installipchandle_param->ipc_handle_id,res);          
    
    if (res < 0 || installipchandle_args.error_code != IPAERROR_OK)
    {
        VPRINTF("%s  fail:  heap_id 0x%x\n",__FUNCTION__,installipchandle_param->ipc_handle_id);
        ret = FALSE;
        goto IPA_POOL_InstallIpcHandle_DONE;
    } 
    installipchandle_param->pool_handle_id = installipchandle_args.pool_handle_id;
    installipchandle_param->error_code = installipchandle_args.error_code;

IPA_POOL_InstallIpcHandle_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex); 		
    return ret;	
}

//unit of timeout is millisecond.
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem_Timeout(struct IPA_Pool_GetMem_Param* get_param,MS_U32 timeout_ms)
{
    MS_BOOL ret = TRUE;
    struct IPA_Pool_Alloc_Args ipa_alloc_args;
    int res = 0;
	
    pthread_mutex_lock(&_IPA_POOL_Mutex);  
    if (_s32FdIPAPool < 0)
    {
        ret = FALSE;
        goto IPA_POOL_GETMEM_DONE;
    }

    ipa_alloc_args.pool_handle_id = get_param->pool_handle_id;	
    ipa_alloc_args.offset_in_pool = get_param->offset_in_pool;
    ipa_alloc_args.length = get_param->length;
    
    ipa_alloc_args.timeout = timeout_ms;

    VPRINTF("%s %d pool_handle_id 0x%x [offset_in_pool,length]= [0x%lx,0x%lx] timeout 0x%x\n", __FUNCTION__,__LINE__,
		ipa_alloc_args.pool_handle_id, (unsigned long)ipa_alloc_args.offset_in_pool,(unsigned long)ipa_alloc_args.length,timeout_ms);
    
    res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_ALLOC, &ipa_alloc_args);
    if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
    {
        MS_U32 each_delay_ms = 50;//empirical value in our test.
        MS_U32 now_delay_ms=0;
        while(1)
        {    
            // delay and try again
            pthread_mutex_unlock(&_IPA_POOL_Mutex);
        
            VPRINTF("%s:%d each_delay_ms=%u, delay and try alloc again now_delay_ms=%u\n",__FUNCTION__,__LINE__,each_delay_ms,now_delay_ms);

            ipa_alloc_args.timeout = 0;//into kernel will not consider timeout again
            MsOS_DelayTask(each_delay_ms);
            pthread_mutex_lock(&_IPA_POOL_Mutex);
            res = ioctl(_s32FdIPAPool, IPA_POOL_IOC_ALLOC, &ipa_alloc_args);
            if ((res == 0) && (ipa_alloc_args.error_code == IPAERROR_OK))
                break;
            now_delay_ms += each_delay_ms;
            if(now_delay_ms >= timeout_ms)
                break;           
        }
        if (res < 0 || ipa_alloc_args.error_code != IPAERROR_OK)
        {
            VPRINTF("ipa pool get memory fail: pool_handle_id %u, offset 0x%lx, len 0x%lx  ipa_alloc_args.error_code=0x%x ipa_alloc_args.timeout=%u res=%d\n", get_param->pool_handle_id, 
			(unsigned long)get_param->offset_in_pool, (unsigned long)get_param->length,ipa_alloc_args.error_code,ipa_alloc_args.timeout,res);
            ret = FALSE;
            goto IPA_POOL_GETMEM_DONE;
        }
    }

    get_param->error_code = ipa_alloc_args.error_code;
    
IPA_POOL_GETMEM_DONE:
    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    
    return ret;

}

void *__attribute__((weak)) IPA_Pool_Polling_Task(void *argc)
{
    MS_BOOL ret = TRUE;
    MS_U32 idx = 0;
    struct IPA_Pool_Event_Args pool_event_args;
    MS_U32 pool_handle_id  = (MS_U32)(intptr_t)(argc);
    VPRINTF("%s %d pool_handle_id=%u,argc=%p\n",__FUNCTION__,__LINE__,pool_handle_id,argc);

    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if (_s32FdIPAPool < 0)
    {
        VPRINTF("%s  fail  \n",__FUNCTION__);      	
        ret = FALSE;
        pthread_mutex_unlock(&_IPA_POOL_Mutex); 
        return NULL;
    } 
    pool_event_args.pool_handle_id = pool_handle_id;
    VPRINTF("%s %d pool_event_args.pool_handle_id=%u\n",__FUNCTION__,__LINE__,pool_event_args.pool_handle_id);

    ret = _findPoolHandleId_InIPA_Pool_Table(pool_event_args.pool_handle_id, &idx);

    pthread_mutex_unlock(&_IPA_POOL_Mutex);  
    
    if(ret == FALSE)
    {
        VPRINTF("%s  fail not find in pool table\n",__FUNCTION__);
        return NULL;
    }
    IPAPool_Info[idx].polling_thread_delete_task_flag = 0;
    VPRINTF("%s %d set for idx=%u\n",__FUNCTION__,__LINE__,idx);

    while(1)
    {
        struct pollfd fds;
        VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
        if(IPAPool_Info[idx].polling_thread_delete_task_flag)
        {
            VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            break; //exit this thread
        }
        fds.fd = pool_handle_id;
        fds.events = POLLIN;
        VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
        if ((ret=poll(&fds, 1, -1)) < 0)
        {
            VPRINTF("%s %d fail\n",__FUNCTION__,__LINE__);
            break;
        }

        if(ret==0)
        {
            VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            continue;
        }

        if (fds.revents&(POLLHUP | POLLERR |POLLNVAL))
        {
            VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            break;
        }
        if (fds.revents&POLLIN)
        {
            VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            memset(&pool_event_args,0,sizeof(struct IPA_Pool_Event_Args));
			pool_event_args.pool_handle_id = pool_handle_id;
            ret=read(fds.fd, &pool_event_args, sizeof(struct IPA_Pool_Event_Args));
			VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            if (sizeof(struct IPA_Pool_Event_Args)!=ret)
            {
                VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
                continue;
            }
            if(IPA_EVENT_CONFLICT == pool_event_args.event)
            {
                //invoke callback corresponding to that pool event
                VPRINTF("%s %d  pool_handle_id=%u  start=0x%lx,length=0x%lx\n",__FUNCTION__,__LINE__,pool_event_args.pool_handle_id,(unsigned long)pool_event_args.start,(unsigned long)pool_event_args.length);
                //after read,may change pool_handle_id,so set pool_handle_id again.
				pool_event_args.pool_handle_id = pool_handle_id;
                VPRINTF("%s %d  pool_handle_id=%u  start=0x%lx,length=0x%lx\n",__FUNCTION__,__LINE__,pool_event_args.pool_handle_id,(unsigned long)pool_event_args.start,(unsigned long)pool_event_args.length);
                IPAPool_Info[idx].polling_callback(pool_event_args.pool_handle_id,
                                                                  pool_event_args.start,pool_event_args.length);
                VPRINTF("%s %d\n",__FUNCTION__,__LINE__);
            }
        }
    }

    //before return ,set this flag.And this flag will be checked in MApi_IPA_Pool_Release.
    IPAPool_Info[idx].polling_thread_delete_task_flag = 1;
    VPRINTF("%s %d idx=%u\n",__FUNCTION__,__LINE__,idx);
    return NULL;

}

//suggest this API be invoked after MApi_IPA_Pool_Init.
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Notify(struct IPA_Pool_Polling_Param *polling_param)
{
    MS_BOOL ret = TRUE;
    pthread_t pthIPAPollingId = -1;
    MS_U32 idx = 0;
    int pthread_res = -1;
    
    VPRINTF("%s\n",__FUNCTION__);
    pthread_mutex_lock(&_IPA_POOL_Mutex);
    if(!polling_param->polling_callback)
    {
        VPRINTF("%u have no polling_callback function , %s return directly\n",polling_param->pool_handle_id,__FUNCTION__);
        pthread_mutex_unlock(&_IPA_POOL_Mutex);
        return TRUE;
    }

    ret = _findPoolHandleId_InIPA_Pool_Table(polling_param->pool_handle_id, &idx);
    
    if(ret == FALSE)//not find idx
    {
         VPRINTF(" %s ipa pool fail not find pool_handle_id %u in pool table\n",__FUNCTION__,polling_param->pool_handle_id);
         pthread_mutex_unlock(&_IPA_POOL_Mutex);
         return FALSE;
    }
    VPRINTF("%s  polling_param->pool_handle_id=0x%x\n",__FUNCTION__,polling_param->pool_handle_id);

    if(-1 == IPAPool_Info[idx].pthIPAPollingId) //no polling id refer to pool handle id
    {
    
        //create polling thread
        pthread_res = pthread_create(&pthIPAPollingId,
                                                NULL,
                                                (void *)IPA_Pool_Polling_Task,
                                                (void*)(intptr_t)polling_param->pool_handle_id);
        //N.B. notice that first parameter of pthread_create is output parameter,which
        //equal with pthread_self() in new pthread,and not equal with getpid().
        //to judge whether pthread_create success,only need check  pthread_create return value.
        if(pthread_res != 0)
        {
            printf("%s   can't create thread  pthread_res=%d 1111\n",__FUNCTION__,pthread_res);
            ret= FALSE;
            pthread_mutex_unlock(&_IPA_POOL_Mutex);
            return ret;
        }

        //corresponding pthIPAPollingId to pool_handle_id
        IPAPool_Info[idx].pthIPAPollingId = pthIPAPollingId;
        IPAPool_Info[idx].polling_callback = polling_param->polling_callback;
    }
    else//already have polling id refer to pool handle id.
    {
        ret=FALSE;
        VPRINTF("pool handle id %u has its polling id %lu ,error,will not set callback again\n",polling_param->pool_handle_id,IPAPool_Info[idx].pthIPAPollingId);
    }

    pthread_mutex_unlock(&_IPA_POOL_Mutex);
    return ret;
}
#endif
