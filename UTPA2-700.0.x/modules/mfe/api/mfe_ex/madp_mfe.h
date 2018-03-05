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
#ifndef _MADP_MFE_H_
#define _MADP_MFE_H_



#ifdef __cplusplus
extern "C"
{
#endif
#include "MFE_chip.h"
#include "MsTypes.h"
#include "drv_mfe_st.h"
#include "apiVOIPMFE.h"
//input method: _SCALER_IN_, _FILE_IN_, _YUVGEN_, and _MUXTS_

// #define _FILE_IN_
//#define _AUDIO_FILE_IN_
//#define _YUVGEN_

//#define _MUXTS_
//#define _MUX_WITH_AUDIO_


//#define AUDIO_MUX_DUMP

//#define _EN_ADP_CRC_ //for testing
//#define CRC_Accumulate	//acc CRC value for overnight testing
//#define DEBUG_GET
//#define TIME_MEASURE

#define MFE_MIU 0UL //kernel mode

#if defined(_YUVGEN_)||defined(_FILE_IN_)
	#define TARGET_FRAME 10UL
#endif

#define FRAM_BUF_SIZE 512UL*1024UL //output bsp buffer

typedef struct _MFE_INPUT_BUF_INFO {
    MS_U32 mfe_miu_offset;
    MS_U32 mfe_buf_pa_withoffset;
    MS_U32 mfe_buf_pa_withoutoffset;
    MS_U32 mfe_buf_size;
    MS_U32 mfe_buf_va;
    MS_U32 mfe_buf_va_end;
} MFE_INPUT_BUF_INFO;


#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_UTOPIA_)
		#include "MsTypes.h"
		#define E_SYS_MMAP_MFE 0x02010000UL //0
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG0(a) MS_PA2KSEG0((MS_PHY)(a)) // for cached memory
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_PHY)(a)) // for non-cached memory
		#define open_dev(a, b) 1UL
             #define MIU_OFFSET 0x18000000UL
#elif defined(__MOBILE_CASE__)
		#include "msutil/MsTypes.h"
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_PHY)(a))
		#define open_dev(a, b) 1UL
#else
    #define open_dev(a, b) 1UL
	#define E_SYS_MMAP_MFE 0UL
       #define MIU_OFFSET 0x0000000000UL
	#ifdef WIN32
        #include "MsTypes.h"
        #include "pmem_mm.h"
		#include <windows.h>
		#define usleep(time) Sleep(time)
		#define MS_VA2PA(a) pmem_mm_va2pa((MS_VIRT)(a))
		#define MS_PA2KSEG1(a) pmem_mm_pa2va((MS_PHY)(a))
		#define MAdp_MPool_VA2PA(a) pmem_mm_va2pa((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG1(a) pmem_mm_pa2va((MS_PHY)(a))
    #else
		#ifndef BOOLEAN
			#define BOOLEAN bool
		#endif
		#define MAdp_MPool_VA2PA(a) (((MS_VIRT)(a))-MEM_LOGMAPPHY)
		#define MAdp_MPool_PA2KSEG1(a) (((MS_PHY)(a))+MEM_LOGMAPPHY)
	#endif
#endif //defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)


#ifndef MUX
#define MUX(a, b, c) ((a) ? (b) : (c))
#endif


MS_BOOL MAdp_MFE_Encode(void/*U8 frc*/);
MS_S32 MAdp_MFE_Finish(void);


#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
#define MAdp_MPool_Init()
MS_S32 MAdp_MPool_GetBlockVa(MS_U32 opt, MS_U32 *buf_start, MS_U32 *buf_size);
#endif

//set memory addr and size, miu0 or miu1.
#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
MS_S32 MAdp_MFE_SetBuffInfo(MS_S32 miu_type,MS_U32 mfe_pa_without_offset_addr,MS_U32 miu_offset,MS_U32 mem_size);
#endif


//this function can write data to DRAM. We can use trace32:"data.save" to get data from FPGA to PC.
                                              //data pointer, offset of 0x82C00000(physical DRAM addr,size(bytes) )
void MADP_MFE_WRITE(MS_U8* data,MS_U32 offset,MS_U32 size);


#ifdef __cplusplus
}
#endif

#endif

