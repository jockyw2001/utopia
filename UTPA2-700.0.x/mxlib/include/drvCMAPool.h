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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSYS_priv.h
/// @brief  System Control Interface
/// @author MStar Semiconductor,Inc.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVCMAPOOL_H_
#define _DRVCMAPOOL_H_

#if defined (MSOS_TYPE_LINUX) || defined (MSOS_TYPE_OPTEE)
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "UFO.h"

#define CMA_FLAG_MAP_VMA    (1UL<<0)
#define CMA_FLAG_CACHED      (1UL<<1)
#define CMA_FLAG_VIRT_ADDR   (1UL<<2)
#define CMA_FLAG_MTLB_POOL  (1UL<<3)

//Be notice 1UL<<4  CMA_FLAG_MAP_KERNEL only kernel utopia use,user utopia not use!


#define CMA_FLAG_KERNEL_MODE_USER_SPACE_MAP_CACHED (1UL<<5) //for cma kernel mode map use va use:whether cache or un-cache

/**
 * These are the only ids that should be used for Ion heap ids.
 * The ids listed are the order in which allocation will be attempted
 * if specified. Don't swap the order of heap ids unless you know what
 * you are doing!
 * Id's are spaced by purpose to allow new Id's to be inserted in-between (for
 * possible fallbacks)
 */
enum ion_heap_ids {
    INVALID_HEAP_ID = -1,

    //system heap
    ION_SYSTEM_HEAP_ID = 0,

    //system contig heap
    ION_SYSTEM_CONTIG_HEAP_ID = 1,

    //carveout heap
    ION_CARVEOUT_HEAP_ID = 2,

    //CHUNK heap
    ION_CHUNK_HEAP_ID = 3,

    //dma heap
    ION_DMA_HEAP_ID = 4,

    //CMA heap
    //mstar id start with 15, and it is created with bootarg in module init dynamically
    //don't add manually!!!!!!!
    ION_CMA_HEAP_ID_START = 15,

    //mstar cma heap: mali type
    ION_MALI_MIUO_HEAP_ID = 16,
    ION_MALI_MIU1_HEAP_ID = 17,
    ION_MALI_MIU2_HEAP_ID = 18,

    //mstar cma heap: vdec type
    ION_VDEC_HEAP_ID = 19,

    //mstar cma heap: xc type
#if defined(CHIP_MONET) || defined(CHIP_MANHATTAN)
        ION_XC_HEAP_ID = 20,
#else
        ION_XC_HEAP_ID = 22,
#endif

    ION_HEAP_ID_RESERVED = 31 /** Bit reserved for ION_SECURE flag */
};

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
#ifdef UFO_PUBLIC_HEADER_300
struct CMA_Pool_Alloc_Param 
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel 
    MS_U32 offset_in_pool;  //in: offset in pool
    MS_U32 length;               //in 
    MS_U32 flags;                 //in: if need virtual address, set VIRT_ADDR
    MS_U32 virt_addr;          //out: if flags: VIRT_ADDR is setted
};
#else
struct CMA_Pool_Alloc_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_SIZE length;               //in
    MS_U32 flags;                 //in: if need virtual address, set VIRT_ADDR
    MS_VIRT virt_addr;          //out: if flags: VIRT_ADDR is setted
};
#endif

#ifdef UFO_PUBLIC_HEADER_300
struct CMA_Pool_Free_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U32 offset_in_pool;  //in: offset in pool
    MS_U32 length;               //in
};
#else
struct CMA_Pool_Free_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_SIZE length;               //in
};
#endif

#ifdef UFO_PUBLIC_HEADER_300
struct CMA_Pool_Init_Param
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap
	
    MS_U32 mtlb_addrspace_offset;    //in: pool location in mtlb address space
    MS_U32 mtlb_addrspace_len;       //in: pool length in mtlb address space
    //if app need virtual address, set MAP_VMA flag
    //if need mapping cached, set MAP_CACHE(when MAP_VMA is set), else noncache
    //if access MTLB address space, set MTLB_POOL 	
    MS_U32 flags; 
    	

    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap id
    MS_U32 miu;  //out: heap base on
    MS_U32 heap_miu_start_offset; //out: heap start offset in miu
    MS_U32 heap_length; //out: heap leagth
};
#else
struct CMA_Pool_Init_Param
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap

    MS_U64 mtlb_addrspace_offset;    //in: pool location in mtlb address space
    MS_U64 mtlb_addrspace_len;       //in: pool length in mtlb address space
    //if app need virtual address, set MAP_VMA flag
    //if need mapping cached, set MAP_CACHE(when MAP_VMA is set), else noncache
    //if access MTLB address space, set MTLB_POOL
    MS_VIRT flags;


    MS_U32 pool_handle_id; //out: generate pool id based on heap specified by heap id
    MS_U32 miu;  //out: heap base on
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
    MS_SIZE heap_length; //out: heap leagth
};

struct PA_In_CMA_Pool_Param
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
#endif
//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Init(struct CMA_Pool_Init_Param * Init_Param);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param * alloc_param);
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param);

#ifdef __cplusplus
}
#endif
#endif
#endif
