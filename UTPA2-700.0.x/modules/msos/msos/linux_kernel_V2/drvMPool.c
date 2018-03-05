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

#include <linux/mman.h>
#include <asm/mach/map.h>
#include <chip_setup.h>

#include "MsCommon.h"
#include "drvMMIO.h"
#include "halCHIP.h"
#include "halMPool.h"

#include <linux/vmalloc.h>
#include <linux/io.h>
#include <linux/module.h>

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#ifdef _Debug
#define MPOOL_DEBUG
#endif

//--------------------------------------------------------------------------------------------------
//  New Internal Variable of MPool Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32		MPOOL_MAPPING;
#define MAX_MAPPINGSIZE 200

#define MMAP_NONCACHE true
#define MMAP_CACHE false

//static MS_U32 MPOOL_MAPPING;

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
//static MS_S32              _s32MPoolFd = -1;
//static void*               _pAddrVirtStart[2] = { NULL, NULL};
//static void*               _pAddrVirtEnd[2] = { NULL, NULL};
//static MS_U32              _u32Va2PaOff[2] = { 0, 0};
static unsigned long u32MpoolRegBase;
static MS_VIRT map_kdriver_mem(MS_PHY u32BusStart, MS_SIZE u32MapSize, MS_BOOL bNonCache);



typedef struct
{
    MS_VIRT pVirtStart;
    MS_VIRT pVirtEnd;
    MS_U64 pPhyaddr;
    MS_U64 u32MpoolSize;
    MS_BOOL bIsUsed;
    MS_SIZE s32V2Poff;
    MS_BOOL bNonCache;
    MS_U8 u8MiuSel;
    MS_BOOL bIsDyn;
} MPOOL_INFO;




static MsOSMPool_DbgLevel _u32MPoolDBGLevel = E_MsOSMPool_DBG_L1;
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];
static struct mutex _MsOS_MPool_Mutex = __MUTEX_INITIALIZER(_MsOS_MPool_Mutex);
static MS_BOOL g_bMpoolInit = FALSE;
static DEFINE_MUTEX(_Mpool_Init_Mutex);

//#ifdef CONFIG_MP_PLATFORM_UTOPIA2K_EXPORT_SYMBOL
#if 1
static unsigned long VMALLOC_SIZE;
static MS_VIRT driver_mem_va_start;
//static unsigned long driver_mem_va_end;
#else
#error "Please allocate kernel VA for driver mapping."
#endif

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
#ifdef MPOOL_DEBUG
    #define MPOOL_ERROR(fmt, args...)           printk("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            printk("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           printk("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)

    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); MS_ASSERT(0); }
#else
    #define MPOOL_ERROR(fmt, args...)           do{} while (0)
    #define MPOOL_WARN(fmt, args...)            do{} while (0)
    #define MPOOL_PRINT(fmt, args...)           do{} while (0)
    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); }
#endif

//#define MPOOL_IS_CACHE_ADDR(addr)              ( ( ((MS_U32)_pAddrVirtStart[0] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[0] > (MS_U32)addr) ) ? TRUE : FALSE )
//#define MPOOL_IS_UNCACHE_ADDR(addr)            ( ( ((MS_U32)_pAddrVirtStart[1] <= (MS_U32)(addr)) && ((MS_U32)_pAddrVirtEnd[1] > (MS_U32)addr) ) ? TRUE : FALSE )

#define MPOOL_DBG_MSG(debug_level, x)           do { if (_u32MPoolDBGLevel >= (debug_level)) (x); } while(0)

//--------------------------------------------------------------------------------------------------
//  Private Function Prototype
//--------------------------------------------------------------------------------------------------
static MS_VIRT _MPool_PA2VA(MS_PHY pAddrPhys, MS_BOOL bNonCache);
static void _MPool_Check_aligned(MS_U32 u32Offset, MS_U32 u32MapSize);
static MS_BOOL _MPOOL_DELAY_BINDING(int idx);

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------


MS_BOOL MsOS_MPool_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	return FALSE;
}

MS_BOOL MsOS_MPool_Init(void)
{
    MS_PHY phySize;
    MS_BOOL bRet = FALSE;

	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);
    VMALLOC_SIZE = VMALLOC_END - VMALLOC_START;

    mutex_lock(&_Mpool_Init_Mutex);

    if (g_bMpoolInit)
    {
        bRet = TRUE;
        goto MPool_Init_End;
    }
    memset(mpool_info,0,sizeof(MPOOL_INFO)*MAX_MAPPINGSIZE);

    if(!MDrv_MMIO_GetBASE(&u32MpoolRegBase, &phySize, MS_MODULE_PM))
    {
        MPOOL_ERROR("[Utopia2K] %s Get IOMAP Base faill!\n", __FUNCTION__);
        bRet = FALSE;
        goto MPool_Init_End;
    }

    bRet = TRUE;
    g_bMpoolInit = TRUE;
	//printk("UTPA2K VA Start:0x%x, end:0x%x\n", (unsigned int)driver_mem_va_start, (unsigned int)driver_mem_va_end);
	//printk("UTPA2K VA Size:0x%x\n", (unsigned int)(driver_mem_va_end-driver_mem_va_start));

MPool_Init_End:
    mutex_unlock(&_Mpool_Init_Mutex);
    return bRet;
}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MsOS_MPool_Init);
#endif

MS_BOOL MsOS_MPool_Close(void)
{
    //printk("MsOS_MPool_Close\n");

    return TRUE;
}



MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
    MS_U32 phyAddr=0xFFFFFFFF;
    int i, idx;

    mutex_lock(&_Mpool_Init_Mutex);

    if (!g_bMpoolInit)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] ERROR!! MPool was not initialized.!\033[m\n");
        goto VA2PA_END;
    }

    for(i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        // if not used, convert searching is over...
        if(!mpool_info[i].bIsUsed)
        {
            break;
        }

		// check if cache_policy is correct
        //if(mpool_info[i].bNonCache != bNonCache)
        //{
		//	//printk("\033[35mcache policy is not correct, check next\033[m\n");
        //    continue;
        //}

        // we do PA_REGION check here, to check if pa_addr in located in mpool_info[i]
        if(!((mpool_info[i].pVirtStart <= pAddrVirt) && (pAddrVirt < mpool_info[i].pVirtEnd)))
        {
            continue;
        }

        //return HAL_MsOS_MPool_VA2PA(pAddrVirt, mpool_info[i].s32V2Poff);
        phyAddr = pAddrVirt - mpool_info[i].s32V2Poff;

        mutex_unlock(&_Mpool_Init_Mutex);
        return phyAddr;
    }

    // Debug
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("=== MPool Table Print! ===\n"));
    for(idx=0;idx<MAX_MAPPINGSIZE;idx++)
    {
        if(!mpool_info[idx].bIsUsed)
            continue;
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].u8MiuSel = %d\n", idx,mpool_info[idx].u8MiuSel));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].bNonCache = %d\n", idx,mpool_info[idx].bNonCache));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtStart =%lx\n",idx, mpool_info[idx].pVirtStart));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtEnd = %lx\n", idx,mpool_info[idx].pVirtEnd));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pPhyaddr =%lx\n", idx,mpool_info[idx].pPhyaddr));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].u32MpoolSize =%lx\n", idx,mpool_info[idx].u32MpoolSize));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("###\n"));
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("==========================\n"));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2K] ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%lx) TO PA\n", pAddrVirt));

VA2PA_END:
    mutex_unlock(&_Mpool_Init_Mutex);
    return phyAddr;
}

void MsOS_MPool_InfoMsg(void)
{
	int i;
	printk("=========================Mapping Info=================================\n");
        printk("MPool Total VA 0x%lx bytes (0x%lx ~ 0x%lx)\n", VMALLOC_SIZE, VMALLOC_START, VMALLOC_END);
	for (i=0;i<MAX_MAPPINGSIZE; i++)
    {
        	if(mpool_info[i].bIsUsed == true)
			printk("map miu:%d-pa:0x%x with size:0x%x to VA(0x%x ~ 0x%x)\n",mpool_info[i].u8MiuSel,mpool_info[i].pPhyaddr,mpool_info[i].u32MpoolSize,mpool_info[i].pVirtStart,mpool_info[i].pVirtEnd);
    }
	printk("======================================================================\n");
	return;
}

MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys)   // non-cache
{
    return _MPool_PA2VA(pAddrPhys, TRUE);
}
MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY pAddrPhys)   //cache
{
    return _MPool_PA2VA(pAddrPhys, FALSE);
}

static MS_VIRT _MPool_PA2VA(MS_PHY pAddrPhys, MS_BOOL bNonCache)
{
    MS_VIRT pAddrVirt=0;
    int i, idx,maxID = -1;
    MS_U64 u64Maxsize = 0;

    mutex_lock(&_Mpool_Init_Mutex);

    if (!g_bMpoolInit)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] ERROR!! MPool was not initialized.!\033[m\n");
        goto PA2VA_End;
    }

    for(i=0;i<MAX_MAPPINGSIZE;i++)
    {
        // search all ...
        if(!mpool_info[i].bIsUsed)
        {
            continue;
        }

        // check if cache_policy is correct
        if(mpool_info[i].bNonCache != bNonCache)
        {
            continue;
        }

        // we do PA_REGION check here, to check if pAddrPhys in located in mpool_info[i]
        if(!((mpool_info[i].pPhyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize))))
        {
            continue;
        }

        if( (mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize - pAddrPhys) > u64Maxsize)
        {
            u64Maxsize = (mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize - pAddrPhys);
            maxID = i;
        }
    }

    if(maxID == -1)
        goto PA2VA_Error;

    if(mpool_info[maxID].bIsDyn)
    {
        if((mpool_info[maxID].s32V2Poff == 0) && (mpool_info[maxID].pVirtStart == 0))
        {
            if(!_MPOOL_DELAY_BINDING(maxID))
            {
                printk("[Utopia2K] PA2KSEG delay binding fail!, pa = %x", (int)mpool_info[maxID].pPhyaddr);

                goto PA2VA_Error;
                //return (MS_U32)NULL;
            }
        }
        //mpool_info[i].bIsDyn = false    // joe, need to be discussed
    }

    //pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].pPhyaddr, mpool_info[i].u32MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);
    pAddrVirt = pAddrPhys + mpool_info[maxID].s32V2Poff;

    if(pAddrVirt != 0)
    {
        mutex_unlock(&_Mpool_Init_Mutex);
        return pAddrVirt;
    }

PA2VA_Error:
    // Debug
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("=== MPool Table Print! ===\n"));
    for(idx=0;idx<MAX_MAPPINGSIZE;idx++)
    {
        if(!mpool_info[idx].bIsUsed)
            continue;
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].u8MiuSel = %d\n", idx,mpool_info[idx].u8MiuSel));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].bNonCache = %d\n", idx,mpool_info[idx].bNonCache));
	   	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtStart =%lx\n",idx, mpool_info[idx].pVirtStart));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtEnd = %lx\n", idx,mpool_info[idx].pVirtEnd));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pPhyaddr =%lx\n", idx,mpool_info[idx].pPhyaddr));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].u32MpoolSize =%lx\n", idx,mpool_info[idx].u32MpoolSize));
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("###\n"));
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("==========================\n"));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2K] ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM PA(%lx) TO VA.\n", pAddrPhys));

PA2VA_End:
    mutex_unlock(&_Mpool_Init_Mutex);
    return 0x0;
}

MS_BOOL MsOS_MPool_Dcache_Flush(MS_VIRT pAddrVirt, MS_SIZE tSize)
{
    mutex_lock(&_Mpool_Init_Mutex);
    if (!g_bMpoolInit)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] ERROR!! MPool was not initialized.!\033[m\n");
        mutex_unlock(&_Mpool_Init_Mutex);
        return FALSE;
    }
    mutex_unlock(&_Mpool_Init_Mutex);

    Chip_Flush_Cache_Range(pAddrVirt, tSize);

    return TRUE;
}

MS_BOOL MsOS_MPool_Dcache_Flush_All()
{
    mutex_lock(&_Mpool_Init_Mutex);
    if (!g_bMpoolInit)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] ERROR!! MPool was not initialized.!\033[m\n");
        mutex_unlock(&_Mpool_Init_Mutex);
        return FALSE;
    }
    mutex_unlock(&_Mpool_Init_Mutex);

    Chip_Flush_Cache_All();

    return TRUE;
}

static void _MPool_Check_aligned(MS_U32 u32Offset, MS_U32 u32MapSize)
{
    if(u32Offset&0xFFF)
    {
        printk("\033[31m!!! Important issue !!!\033[m\n");
        printk("\033[31mThe memory mapping address is not aligned, please make sure of that\033[m\n");
        MS_ASSERT(0);
    }

    if(u32MapSize&0xFFF)
    {
        printk("\033[31m!!! Important issue !!!\033[m\n");
        printk("\033[31mThe memory mapping size is not aligned, please make sure of that\033[m\n");
        MS_ASSERT(0);
    }

    return;
}

static MS_BOOL _MPOOL_DELAY_BINDING(int idx)
{
    //Mutex lock was added on caller function.
    MS_U32 ba_mapping_start;
    driver_mem_va_start = 0;
    MS_BOOL bRet = FALSE;

    // if this mapping will over driver_mem_va_end, return failed, need to check your total mapping size
    if(VMALLOC_SIZE < mpool_info[idx].u32MpoolSize)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] Kernel driver VA size is not enough !!\033[m\n");
        return FALSE;
    }

	//if(mpool_info[idx].u8MiuSel == 0)
    //    ba_mapping_start = mpool_info[idx].pPhyaddr + HAL_MIU0_BUS_BASE;	// miu_0
	//else
    //    ba_mapping_start = (mpool_info[idx].pPhyaddr - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE;	// miu_1
    ba_mapping_start = HAL_MsOS_MPool_PA2BA(mpool_info[idx].pPhyaddr);
    driver_mem_va_start = (unsigned long)map_kdriver_mem(ba_mapping_start, mpool_info[idx].u32MpoolSize, mpool_info[idx].bNonCache);
    if(driver_mem_va_start)
	bRet = TRUE;

    // record PA_VA mapping
    mpool_info[idx].pVirtStart = driver_mem_va_start;
    mpool_info[idx].pVirtEnd   = mpool_info[idx].pVirtStart + mpool_info[idx].u32MpoolSize;

    mpool_info[idx].s32V2Poff  = mpool_info[idx].pVirtStart - mpool_info[idx].pPhyaddr;

    //driver_mem_va_start += mpool_info[idx].u32MpoolSize;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtStart =%lx\n", idx,mpool_info[idx].pVirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pVirtEnd = %lx\n", idx,mpool_info[idx].pVirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].pPhyaddr =%lx\n", idx,mpool_info[idx].pPhyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].u32MpoolSize =%lx\n", idx,mpool_info[idx].u32MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("mpool_info[%d].s32V2Poff = %lx\n", idx,mpool_info[idx].s32V2Poff));

    return bRet;
}

MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_BOOL bNonCache)
{
    int i, idx = 0;
    MS_U32 ba_mapping_start;
    MS_U32 Phyaddr, PhyaddrEnd;
    MS_BOOL bRet = FALSE;
    driver_mem_va_start = 0;

    _MPool_Check_aligned(u32Offset, u32MapSize);

    //if(u8MiuSel == 0)
    //    Phyaddr = u32Offset;
    //else
    //    Phyaddr = u32Offset + HAL_MIU1_BASE;
    _miu_offset_to_phy(u8MiuSel, u32Offset, Phyaddr);

    PhyaddrEnd = Phyaddr + u32MapSize - 1;

    mutex_lock(&_Mpool_Init_Mutex);

    if (!g_bMpoolInit)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] ERROR!! MPool was not initialized.!\033[m\n");
        goto Mapping_End;
    }

    // if all mpool_info is used, return failed, need to enlarge MAX_MAPPINGSIZE
    if(mpool_info[MAX_MAPPINGSIZE-1].bIsUsed)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] Not enough MPool, must increase MAX_MAPPINGSIZE!!\033[m\n");
        goto Mapping_End;
    }

    // if this mapping will over driver_mem_va_end, return failed, need to check your total mapping size
    /*if(driver_mem_va_end - driver_mem_va_start < u32MapSize)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] Kernel driver VA size is not enough !!\033[m\n");
        goto Mapping_End;
    }*/
    if(VMALLOC_SIZE < u32MapSize)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] Kernel driver VA size is not enough !!\033[m\n");
        goto Mapping_End;
    }

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if (mpool_info[i].bIsUsed == FALSE)
        {
            mpool_info[i].bIsUsed = TRUE;

            if (bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            break;
        }
        else
        {
            if (bNonCache != mpool_info[i].bNonCache)
                continue;

            if ( (mpool_info[i].pPhyaddr <= Phyaddr && Phyaddr < mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize) ||
                 (mpool_info[i].pPhyaddr <= PhyaddrEnd && PhyaddrEnd < mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize) )
            {
                bRet = TRUE;
                printk("[Utopia2K] Duplicated PA mapping.\n");
                //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2K] Duplicated PA mapping.\n"));
                //Duplicated mapping or Overlap mapping.
                goto Mapping_End;
            }
        }
    }

    ba_mapping_start = HAL_MsOS_MPool_PA2BA(Phyaddr);

    driver_mem_va_start = (MS_VIRT)map_kdriver_mem(ba_mapping_start, u32MapSize, bNonCache);

    // record PA_VA mapping
    mpool_info[idx].pVirtStart      = driver_mem_va_start;
    mpool_info[idx].pVirtEnd        = mpool_info[idx].pVirtStart + u32MapSize;

    mpool_info[idx].pPhyaddr        = Phyaddr;				// pa
    mpool_info[idx].u8MiuSel        = u8MiuSel;

    mpool_info[idx].u32MpoolSize    = u32MapSize;
    mpool_info[idx].s32V2Poff       = mpool_info[idx].pVirtStart - mpool_info[idx].pPhyaddr;
    mpool_info[idx].bIsDyn          = false;

    //driver_mem_va_start += u32MapSize;
    bRet = TRUE;

Mapping_End:
    mutex_unlock(&_Mpool_Init_Mutex);
    return bRet;
}


MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_BOOL bNonCache)
{
    MS_U32 Phyaddr, PhyaddrEnd;
    MS_BOOL ret=TRUE;
    int i,idx=0;

    _MPool_Check_aligned(u32Offset, u32MapSize);

    //if(u8MiuSel == 0)
    //    Phyaddr = u32Offset;
    //else
    //    Phyaddr = u32Offset + HAL_MIU1_BASE;
    _miu_offset_to_phy(u8MiuSel, u32Offset, Phyaddr);

    PhyaddrEnd = Phyaddr + u32MapSize - 1;

    mutex_lock(&_Mpool_Init_Mutex);

    if(mpool_info[MAX_MAPPINGSIZE-1].bIsUsed)
    {
        printk(KERN_EMERG "\033[35m[Utopia2K] Not enough MPool, must increase MAX_MAPPINGSIZE!!\033[m\n");
        ret = FALSE;
        goto Mapping_Dynamic_End;
    }

    for (i=0;i<MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed==false)
        {
            mpool_info[i].bIsUsed = true;

            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            break;
        }
        else
        {
            if (bNonCache != mpool_info[i].bNonCache)
                continue;

            if ( (mpool_info[i].pPhyaddr == Phyaddr) && ((mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize) == PhyaddrEnd) )
            {
                ret = TRUE;
                printk("[Utopia2K] Duplicated PA mapping.\n");
                MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2K] Duplicated PA(0x%lx ~ 0x%lx) mapping with Table[%d](0x%lx ~0x%lx).\n",
                    Phyaddr, PhyaddrEnd, i, mpool_info[i].pPhyaddr, (mpool_info[i].pPhyaddr + mpool_info[i].u32MpoolSize -1)));
                //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printk("[Utopia2K] Duplicated PA mapping.\n"));
                //Duplicated mapping or Overlap mapping.
                goto Mapping_Dynamic_End;
            }
        }
    }

    mpool_info[idx].pVirtStart = 0;
    mpool_info[idx].pVirtEnd = 0;
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = true;
    mpool_info[idx].pPhyaddr = Phyaddr;
    mpool_info[idx].u32MpoolSize = u32MapSize;
    mpool_info[idx].s32V2Poff = 0;

	printk("mpool_info[%d].u8MiuSel = %d\n", idx,mpool_info[idx].u8MiuSel);
    printk("mpool_info[%d].bNonCache = %d\n", idx,mpool_info[idx].bNonCache);
    printk("mpool_info[%d].pVirtStart =%lx\n",idx, mpool_info[idx].pVirtStart);
    printk("mpool_info[%d].pVirtEnd = %lx\n", idx,mpool_info[idx].pVirtEnd);
    printk("mpool_info[%d].pPhyaddr =%lx\n", idx,mpool_info[idx].pPhyaddr);
    printk("mpool_info[%d].u32MpoolSize =%lx\n", idx,mpool_info[idx].u32MpoolSize);
    printk("###\n");

Mapping_Dynamic_End:
    mutex_unlock(&_Mpool_Init_Mutex);
    return ret;
}

static MS_VIRT map_kdriver_mem(MS_PHY u32BusStart, MS_SIZE u32MapSize, MS_BOOL bNonCache){
	MS_VIRT VirtAddr = 0;

    if(pfn_valid(__phys_to_pfn(u32BusStart & PAGE_MASK)))
	{
		/* the __va will return the cached kernel_memory mapping !! This is due to the kernel memory is mapped as cached, and never be remapped @3.10.86(but 3.10.40 will be remaped to non-cached after allocated by cma)
		 * So, we need to remap the non-cached request !! The UnMapping will also need a corresponding change.
		 *
		 *
		 * If 3.10.86 having remap for cma, then here and UnMapping may need to change again
		 */
	    if(bNonCache)
		{
			int err;
			unsigned long addr;
			struct vm_struct *area;

			area = get_vm_area_caller(u32MapSize, VM_IOREMAP, "TVOS");
			if (!area)
			{
				dump_stack();
				return NULL;
			}
			addr = (unsigned long)area->addr;
#if defined (__aarch64__)
			err = ioremap_page_range(addr, addr + u32MapSize, u32BusStart, __pgprot(PROT_DEVICE_nGnRnE));
#else
			err = ioremap_page_range(addr, addr + u32MapSize, u32BusStart, MT_DEVICE);
#endif
			if(err)
			{
				dump_stack();
				return NULL;
			}
			VirtAddr = addr;
		}
		else
        	VirtAddr = __va(u32BusStart);
	}
    else
    {
	    if(bNonCache)
			VirtAddr = (MS_VIRT)ioremap(u32BusStart, u32MapSize);
	    else
			VirtAddr = (MS_VIRT)ioremap_cached(u32BusStart, u32MapSize);
    }

	return VirtAddr;
}

MS_BOOL MsOS_MPool_UnMapping(MS_VIRT u32VirtStart, MS_SIZE u32MapSize)
{
    MS_BOOL ret = FALSE;
    int i;
	MS_PHY u32BusStart;

    mutex_lock(&_Mpool_Init_Mutex);
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == true)
		{
        	if(mpool_info[i].pVirtStart == u32VirtStart)
			{
				u32BusStart = HAL_MsOS_MPool_PA2BA(mpool_info[i].pPhyaddr);
				/* if Linux Memory and also cached, the mapping is using Linux Memory Mapping. We don't need to unmapping it */
				if( (!mpool_info[i].bNonCache) && (pfn_valid(__phys_to_pfn(u32BusStart & PAGE_MASK))) )
				{
					printk("\033[35mFunction = %s, Line = %d, Linux Memory and also cached, do not unmapping\033[m\n", __PRETTY_FUNCTION__, __LINE__);
				}
				else
					iounmap((volatile void __iomem *)u32VirtStart);

				mpool_info[i].pVirtStart = 0;
				mpool_info[i].pVirtEnd = 0;
				mpool_info[i].u8MiuSel = 0;
				mpool_info[i].bIsDyn = false;
				mpool_info[i].pPhyaddr = 0;
				mpool_info[i].u32MpoolSize = 0;
				mpool_info[i].s32V2Poff = 0;
				mpool_info[i].bNonCache = 0;
				mpool_info[i].bIsUsed = false;
				ret = TRUE;
        	    break;
	    	}
		}
    }
    mutex_unlock(&_Mpool_Init_Mutex);
    return ret;
}

MS_BOOL MsOS_MPool_Kernel_Detect(MS_PHY *lx_addr, MS_U64 *lx_size, MS_PHY *lx2_addr, MS_U64 *lx2_size)
{
    printk("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return FALSE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
    _u32MPoolDBGLevel = DbgLevel;
}

EXPORT_SYMBOL(MsOS_MPool_Mapping);
EXPORT_SYMBOL(MsOS_MPool_Mapping_Dynamic);
EXPORT_SYMBOL(MsOS_MPool_PA2KSEG1);
EXPORT_SYMBOL(MsOS_MPool_PA2KSEG0);
EXPORT_SYMBOL(MsOS_MPool_VA2PA);

MS_BOOL MsOS_MPool_Add_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL find = FALSE;
    MS_U64 u64AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_U32 i, idx = 0;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    //for multi-thread access
    mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;
            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            find = TRUE;

            break;
        }
    }

    if(!find)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    _phy_to_miu_offset(u8MiuSel, u64AddrOffset, u64PhysAddr);   // get miu & offset

    mpool_info[idx].pVirtStart = u64VirtAddr;
    mpool_info[idx].pVirtEnd = (u64VirtAddr + u64MapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;
    mpool_info[idx].pPhyaddr = u64PhysAddr;
    mpool_info[idx].u32MpoolSize = u64MapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].pVirtStart - mpool_info[idx].pPhyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pVirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].pPhyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u32MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    MPOOL_MAPPING = 1;
    mutex_unlock(&_MsOS_MPool_Mutex);

    return TRUE;
}

EXPORT_SYMBOL(MsOS_MPool_Add_PA2VARange);

MS_BOOL MsOS_MPool_Remove_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL ret = FALSE, mNonCache = MMAP_NONCACHE;
    MS_U32 i;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    if(bNonCache)
        mNonCache= MMAP_NONCACHE;
    else
        mNonCache= MMAP_CACHE;

    //multi-thread access
    mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if((mpool_info[i].bIsUsed == false) || (mpool_info[i].bNonCache != mNonCache))
            continue;

        if((mpool_info[i].pVirtStart == u64VirtAddr)
            && (mpool_info[i].pPhyaddr == u64PhysAddr)
            && (mpool_info[i].u32MpoolSize == u64MapSize))
        {
             ret = TRUE;
             memset(&mpool_info[i], 0, sizeof(mpool_info[i]));
             break;
        }
    }

    mutex_unlock(&_MsOS_MPool_Mutex);
    return ret;
}
EXPORT_SYMBOL(MsOS_MPool_Remove_PA2VARange);
MODULE_LICENSE("GPL");
