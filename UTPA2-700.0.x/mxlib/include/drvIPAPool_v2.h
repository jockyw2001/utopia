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
/// @file   drvIPAPool.h
/// @brief  drv IPA Pool Control Interface
/// @author MStar Semiconductor,Inc.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVCMAPOOL_H_
#define _DRVCMAPOOL_H_

#if defined (CONFIG_MSTAR_IPAPOOL)
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#define TEST_USE_SELF_KO_MAKEFILE_TOTAL 0//do not enable this in p4.this only test code.

#define IPAPOOL_HEAP_NAME_MAX_LEN_Param 32

enum IPA_SPACE_TYPE
{
    IPA_SPACE_TYPE_PA = 0,//directly PA space
    IPA_SPACE_TYPE_CMA = 1, //CMA space 
    IPA_IPA_SPACE_TYPE_INVALID = 0xFFFFFFFF//force enum size to be 4 byte alignment
};

struct IPA_Pool_InstallIpcHandle_Param
{
    MS_U32 ipc_handle_id;  //in: returned by kernel, when get IPC handle
    MS_U32 pool_handle_id; //out: pool handle id
    MS_S32 error_code; // out: error code when failed
};

struct IPA_Pool_GetIpcHandle_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U32 ipc_handle_id;  //out: returned by kernel
    MS_S32 error_code; // out: error code when failed
};

struct IPA_Pool_Heap_Attr_Param
{
    MS_U32 heap_id;     //in: maybe shared with more than one pools which based on this heap

    char   name[IPAPOOL_HEAP_NAME_MAX_LEN_Param]; //out: heap name    
    MS_U64 heap_miu_start_offset; //out: heap start offset in miu
    MS_U64 heap_length; //out: heap length
    MS_U32 miu;  //out: miu id this heap belongs, index from 0.
    enum IPA_SPACE_TYPE heap_type;//out: return heap type to application (enum IPA_SPACE_TYPE )
    MS_S32 error_code; // error code when failed 
};
enum IPA_DCACHE_FLUSH_TYPE_Param
{
    IPA_DCACHE_FLUSH_Param,//flush dcache into DRAM
    IPA_DCACHE_INVALID_Param,// invalid dcache lines
    IPA_DCACHE_FLUSH_INVALID_Param// flush and invalid dcache lines
};

struct IPA_Pool_DCacheFlush_Param
{
    MS_U64 virt_addr; //in: the VA need to flush
    MS_U64 length;               //in: flush length
    enum IPA_DCACHE_FLUSH_TYPE_Param flush_type;// in: flush type (enum IPA_DCACHE_FLUSH_TYPE)
};

struct IPA_Pool_Unmap_Param
{
    MS_U64 virt_addr; //in: the VA need to unmap
    MS_U64 length;               //in: unmap length
};

//enum for mapping virtual address type
enum IPA_MAP_VA_TYPE_Param
{
    IPA_MAP_VA_NO_NEEDED_Param = 0,  //we don't need to map VA
    IPA_VA_CACHE_WRITE_BACK_Param = 1, //we need to map VA with cache type "write back", which is normally used
    IPA_VA_CACHE_WRITE_THROUGH_Param = 2, //we need to map VA with cache type "write through", which is //mostly used by graphic system
    IPA_VA_CACHE_NONE_CACHE_Param = 3,  //we don't need data cache
    IPA_VA_TYPE_MAX_Param = 0xFFFFFFFF//force enum size to be 4 byte alignment
};

struct IPA_Pool_Map_Param 
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel 
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in  get memory length
    enum IPA_MAP_VA_TYPE_Param  cache_type;    //in cache type of mapping

    MS_U64 virt_addr;          //out: virtual address for user mode access
    MS_S32 error_code; // out£º reason about why map failed
};


struct IPA_Pool_PutMem_Param
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in
};

struct IPA_Pool_GetMem_Param 
{
    MS_U32 pool_handle_id; //in: pool handle id, when pool init, returned by kernel 
    MS_U64 offset_in_pool;  //in: offset in pool
    MS_U64 length;               //in  get memory length
    MS_S32 error_code; // out£º reason about why MApi_IPA_Pool_GetMem failed
};

struct IPA_Pool_Init_Param
{
    MS_U32 space_id;     //in: space id the pool will be created in
    char *pool_name;//in: global identify name for pool to shared between multiple process
	
    MS_U64 offset_in_heap;    //in: pool location in space
    MS_U64 len;       //in: pool length in  space 

    MS_U32 pool_handle_id; //out: generate pool id based on space specified by space_id
    MS_U32 miu;  //out: miu id this space belongs, index from 0.
    enum IPA_SPACE_TYPE space_type;//out: return space type to application
    MS_S32 error_code; // error code when pool init failed

    MS_U64 space_length; //out: space leagth
    MS_U64 space_miu_start_offset; //out: space start offset in miu
};


enum IPA_event_Param
{
  IPA_EVENT_FREE_Param = 0,
  IPA_EVENT_NO_WAIT_Param,
  IPA_EVNET_NUM_Param
};

struct IPA_Pool_Event_Param
{
    enum IPA_event_Param event;
    MS_U32 pool_handle_id;
};

struct IPA_Pool_Polling_Param
{
    MS_U32 pool_handle_id;//pool handle
    void (*polling_callback)(MS_U32 pool_handle_id,MS_U64 start,MS_U64 length);
};


//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Init(struct IPA_Pool_Init_Param * Init_Param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Release(MS_U32 pool_handle_id);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem(struct IPA_Pool_GetMem_Param * get_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_PutMem(struct IPA_Pool_PutMem_Param * put_param);

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_MapVA(struct IPA_Pool_Map_Param * map_param);
void __attribute__((weak)) MApi_IPA_Pool_UnmapVA(struct IPA_Pool_Unmap_Param * unmap_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_MapKerVA(struct IPA_Pool_Map_Param * map_param);
void __attribute__((weak)) MApi_IPA_Pool_UnmapKerVA(struct IPA_Pool_Unmap_Param * unmap_param);

MS_BOOL __attribute__((weak)) MApi_IPA_Pool_DCacheFlush(struct IPA_Pool_DCacheFlush_Param* dcache_flush_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_HEAP_ATTR(struct IPA_Pool_Heap_Attr_Param* heap_attr_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GETIPCHANDLE(struct IPA_Pool_GetIpcHandle_Param* getipchandle_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_InstallIpcHandle(struct IPA_Pool_InstallIpcHandle_Param* installipchandle_param);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_GetMem_Timeout(struct IPA_Pool_GetMem_Param* get_param,MS_U32 timeout);
MS_BOOL __attribute__((weak)) MApi_IPA_Pool_Notify(struct IPA_Pool_Polling_Param *polling_param);
    #ifdef __cplusplus
}
#endif
#endif
#endif
