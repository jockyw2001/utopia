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

#include "mem_priv.h"
#include "mem.h"

#ifdef CYGDBG_USE_ASSERTS
cyg_bool
Cyg_Mempool__check_this(Cyg_Mempool* pThis, cyg_assert_class_zeal zeal) const
{
    CYG_REPORT_FUNCTION();
    if( this == NULL ) return false;
#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
    return pThis->mypool.check_this( &pThis->mypool, zeal );
#else
    return true;
#endif
}
#endif

#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
cyg_uint8 *
Cyg_Mempool__alloc(Cyg_Mempool* pThis, cyg_int32 size)
{
    return pThis->mypool.alloc( &pThis->mypool, size );
}
    
# ifdef CYGFUN_KERNEL_THREADS_TIMER
cyg_uint8 *
Cyg_Mempool__alloc(Cyg_Mempool* pThis, cyg_int32 size, cyg_tick_count delay_timeout)
{
    return pThis->mypool.alloc( &pThis->mypool, size , delay_timeout );
}
# endif
#endif

cyg_uint8 *
Cyg_Mempool__try_alloc(Cyg_Mempool* pThis, cyg_int32 size)
{
    return pThis->mypool.try_alloc( &pThis->mypool, size );
}

cyg_uint8 *
Cyg_Mempool__resize_alloc( Cyg_Mempool* pThis, cyg_uint8 *alloc_ptr, cyg_int32 newsize,
                                    cyg_int32 *oldsize )
{
    return pThis->mypool.resize_alloc( &pThis->mypool, alloc_ptr, newsize, oldsize );
}


cyg_bool 
Cyg_Mempool__free( Cyg_Mempool* pThis, cyg_uint8 *p, cyg_int32 size )
{
    return pThis->mypool.free( &pThis->mypool, p, size );
}

cyg_int32
Cyg_Mempool__get_allocation_size( Cyg_Mempool* pThis, cyg_uint8* p )
{
    return pThis->mypool.get_allocation_size(&pThis->mypool, p);
}


void
Cyg_Mempool__get_status( Cyg_Mempool* pThis, cyg_mempool_status_flag_t flags,
                                  Cyg_Mempool_Status* pstatus )
{
    
    
    pstatus->waiting = 0;

    pThis->mypool.get_status( &pThis->mypool, flags, pstatus );
}




void cyg_mempool_var_create(
    void            *base,              
    cyg_int32       size,               
    cyg_handle_t    *handle,            
    cyg_mempool_var *var                
    )
{
    

    var->pThis = malloc(sizeof(Cyg_Mempool));
    if(var->pThis != NULL)
        Cyg_Mempool__Create((Cyg_Mempool*)var->pThis, (cyg_uint8*)base, size, 16);
    *handle = (cyg_handle_t)var;
}


void cyg_mempool_var_delete(cyg_handle_t varpool)
{
    
    cyg_mempool_var *var = (cyg_mempool_var*)varpool;
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool__Delete(pThis);
    free(pThis);
    var->pThis = NULL;
}

#ifdef CYGSEM_MEMALLOC_ALLOCATOR_VARIABLE_THREADAWARE
void *cyg_mempool_var_alloc(cyg_handle_t varpool, cyg_int32 size)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->alloc(size);
}

# ifdef CYGFUN_KERNEL_THREADS_TIMER

void *cyg_mempool_var_timed_alloc(
    cyg_handle_t     varpool,
    cyg_int32        size,
    cyg_tick_count_t abstime)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->(size, abstime);
}

# endif
#endif

void *cyg_mempool_var_try_alloc(
    cyg_handle_t varpool,
    cyg_int32    size)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    return pThis->try_alloc(pThis, size);
}


void cyg_mempool_var_free(cyg_handle_t varpool, void *p)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    cyg_bool b;

    b = pThis->free(pThis, (cyg_uint8 *)p, 0);
    CYG_ASSERT( b, "Bad free");
}



cyg_bool_t cyg_mempool_var_waiting(cyg_handle_t varpool)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool_Status stat;

    pThis->get_status(pThis, CYG_MEMPOOL_STAT_WAITING, &stat);
    return (stat.waiting != 0);
}


void cyg_mempool_var_get_info(
    cyg_handle_t varpool,
    cyg_mempool_info *info)
{
    Cyg_Mempool* pThis = ((Cyg_Mempool*)((cyg_mempool_var*)varpool)->pThis);
    Cyg_Mempool_Status stat;

    pThis->get_status(pThis,
                   CYG_MEMPOOL_STAT_AREASIZE|
                   CYG_MEMPOOL_STAT_TOTALFREE|
                   CYG_MEMPOOL_STAT_ARENABASE|
                   CYG_MEMPOOL_STAT_ORIGSIZE|
                   CYG_MEMPOOL_STAT_MAXFREE, &stat );

    info->totalmem = stat.areasize;
    info->freemem  = stat.totalfree;
    info->size = stat.origsize;
    
    info->base = (const void*)stat.arenabase;
    info->blocksize  = -1;
    info->maxfree = stat.maxfree;
}





void
Cyg_Mempool__Create(
    Cyg_Mempool* pThis,
    cyg_uint8 *base,
    cyg_int32 size,
    cyg_int32 alignment)
{
    memset(pThis, 0, sizeof(Cyg_Mempool));
    
    pThis->try_alloc= Cyg_Mempool__try_alloc;
    pThis->resize_alloc= Cyg_Mempool__resize_alloc;
    pThis->free= Cyg_Mempool__free;
    pThis->get_allocation_size= Cyg_Mempool__get_allocation_size;
    pThis->get_status= Cyg_Mempool__get_status;
    Cyg_Mempool_Implementation__Create(&pThis->mypool,
        base,
        size,
        (CYG_ADDRWORD) alignment );
}


void
Cyg_Mempool__Delete( Cyg_Mempool* pThis)
{
    memset(pThis, 0, sizeof(Cyg_Mempool));
}

