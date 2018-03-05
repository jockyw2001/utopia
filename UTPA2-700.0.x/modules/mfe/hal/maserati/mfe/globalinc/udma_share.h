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
#ifndef _UDMA_SHARE_H_
#define _UDMA_SHARE_H_

//#define _FPGA_
//#define _MSVC6_ //open when using VC6.0 platform
#include "MFE_chip.h"
#ifdef _AEON_PLATFORM_
#ifdef _MFE_T8_
#else
#include "../../../../MsTypes.h"
#endif
#endif

#ifdef _MIPS_PLATFORM_
#if defined(_MFE_T8_)&&(!defined(_KERNEL_MODE_))
#include "../../../../MsTypes.h"
#include "../../../../drv/mfe/mdrv_mfe_st.h"
#elif defined(_MFE_T8_)&&(defined(_KERNEL_MODE_))
#include "mdrv_mfe_st.h"
#include "mdrv_types.h"
#else
#include "mdrv_types.h"
#endif
#endif //_MIPS_PLATFORM_

#include "mfe_type.h"

#ifdef _WIN32
#include "../../mdrv_mfe_st.h"
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&defined(_KERNEL_MODE_)
	//#define addr_log2phy(log_addr) MS_VA2PA(log_addr) //no log to phy in drv level
	#define addr_phy2log(phy_addr) MDrv_SYS_PA2NonCacheSeg( (void*) phy_addr )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&(!defined(_KERNEL_MODE_))
	#define addr_log2phy(log_addr) MS_VA2PA(log_addr)
	#define addr_phy2log(phy_addr) MS_PA2KSEG1(phy_addr)
#elif defined(_M4VE_BIG2_)&&defined(_MIPS_PLATFORM_)
	#include "utility.h"
	#define addr_log2phy(log_addr) MApi_UTL_HwMemAccessOffset((void *)log_addr)
	//#define addr_phy2log(phy_addr) ((phy_addr)+MEM_LOGMAPPHY) //MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W)
	#define addr_phy2log(phy_addr) (MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W) + CYGARC_KSEG_UNCACHED_BASE )
	//#define addr_phy2log(phy_addr) MApi_UTL_PhyMemRestore((CYG_ADDRESS)phy_addr, INTF_M4VE_MC_W)
#elif defined(WIN32)
	extern MFE_U32 MAdp_MPool_VA2PA_Win32(MFE_U32 a);
	extern MFE_U32 MAdp_MPool_PA2VA_Win32(MFE_U32 a);
	#define addr_log2phy(log_addr) MAdp_MPool_VA2PA_Win32((MFE_U32)log_addr)
	#define addr_phy2log(phy_addr) MAdp_MPool_PA2VA_Win32((MFE_U32)phy_addr)
#else
	#define addr_log2phy(log_addr) ((log_addr)-MEM_LOGMAPPHY)
	#define addr_phy2log(phy_addr) ((phy_addr)+MEM_LOGMAPPHY)
#endif

typedef void (*proto_write)(MFE_U32 u32Address, MFE_U16 val);
typedef void (*proto_read) (MFE_U32 u32Address, MFE_U16 *val);

void MMAPInit(MFE_U32 buf_base, MFE_U32 buf_size, MFE_U32* ppBufStart);
long MMAPMalloc(MFE_U32* ppBufStart, MFE_U32 pBufEnd, MFE_U32 size, MEMMAP_t * memmap, int nAlignBytes,const char* msg);
int reg_scan(unsigned short *reg_mask, MFE_U32 num_reg, proto_write write_func, proto_read read_func);

#endif
