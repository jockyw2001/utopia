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

#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"

/******* for nos Mpool Mapping***********/
//for temporary using, if miu mapping is moved from mhal_init_Mmu function, va_start = 0x20000000, va_end = 0xFFFFFFFF
static unsigned int va_start = 0x60000000;
static unsigned int va_end   = 0xA0000000;
typedef struct
{
     unsigned int va;
     unsigned int pa;
     unsigned int size;
     int         access;
     int         attributes;
}
_cpu_memory_t; 

typedef struct
{
	MS_VIRT pVirtStart;
	MS_VIRT pVirtEnd;
	MS_PHY pPhyaddr;
        MS_PHY u64MpoolSize;
        MS_BOOL bIsUsed;
        MS_SIZE s32V2Poff;
        MS_BOOL bNonCache;
        MS_U8 u8MiuSel;
 } MPOOL_INFO;

static MsOSMPool_DbgLevel	_u64MPoolDBGLevel;
extern void _cpu_mmu_map_memory (_cpu_memory_t *entry);
extern void mhal_tlb_flush(void);

#define MAX_MAPPINGSIZE 100
#define MPOOL_DBG_MSG(debug_level, x)     do { if (_u64MPoolDBGLevel >= (debug_level)) (x); } while(0)

#define MMAP_NONCACHE true
#define MMAP_CACHE false
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];

/*for nos mpool mapping*/
 #define MMU_AP_NONE         0
 #define MMU_AP_PRIVILEGED   1
 #define MMU_AP_READ         2
 #define MMU_AP_FULL         3

 #define MMU_BUFFERED        1
 #define MMU_CACHED          2
/**********************/

/****************************************/
//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Init(void)
{
    return TRUE;
}

MS_BOOL MsOS_MPool_Close(void)
{
    return TRUE;
}

// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    return TRUE;
}
static void _MPool_Check_aligned(MS_PHY u64Offset, MS_PHY u64MapSize)
{
     if(u64Offset&0xFFFFFUL)
     {
         printf("!!! Important issue !!!\n");
         printf("The memory mapping address is not aligned, please make sure of that\n");
         MS_ASSERT(0);
     }

     if(u64MapSize&0xFFFFFUL)
     {
         printf("!!! Important issue !!!\n");
         printf("The memory mapping size is not aligned, please make sure of that\n");
                 MS_ASSERT(0);
     }

     return;
}
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE u64Offset, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    printf("Optee don't support !!!\n");
    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
    return HAL_MsOS_MPool_VA2PA(pAddrVirt);
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys)
{
    return HAL_MsOS_MPool_PA2KSEG1(pAddrPhys);
}
/*PA->BA->VA, through the VA<->BA offset to get the return VA*/
MS_VIRT _MPool_PA2VA(MS_PHY pAddrPhys, MS_BOOL bNonCache)
{
    MS_VIRT pAddrVirt=0;
    int i, idx;

    for(i=0;i<MAX_MAPPINGSIZE;i++)
    {
        // if not used, convert searching is over...
        if(!mpool_info[i].bIsUsed)
        {
            break;
        }

        // check if cache_policy is correct
        if(mpool_info[i].bNonCache != bNonCache)
        {
            continue;
        }

        // we do PA_REGION check here, to check if pAddrPhys in located in mpool_info[i]
        if(!((mpool_info[i].pPhyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].pPhyaddr + mpool_info[i].u64MpoolSize))))
        {
            continue;
        }

        pAddrVirt = HAL_MsOS_MPool_PA2BA(pAddrPhys) + mpool_info[i].s32V2Poff;

        if(pAddrVirt != 0)
            return pAddrVirt;
        else
            break;
    }

    // Debug
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("=== MPool Table Print! ===\n"));
    for(idx=0;idx<MAX_MAPPINGSIZE;idx++)
    {
        if(!mpool_info[idx].bIsUsed)
            continue;

		MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u8MiuSel = %d\n", (ptrdiff_t)idx, mpool_info[idx].u8MiuSel));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].bNonCache = %d\n", (ptrdiff_t)idx, mpool_info[idx].bNonCache));
		MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtStart));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pVirtEnd =  %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtEnd));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].pPhyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pPhyaddr));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
        
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("###\n"));
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("==========================\n"));

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("[Utopia2_nos] ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM PA(%tX) TO VA.\n", (ptrdiff_t)pAddrPhys));


PA2VA_End:
  return 0x0;
}	
