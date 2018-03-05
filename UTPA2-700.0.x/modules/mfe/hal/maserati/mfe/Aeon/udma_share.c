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
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"


#include "udma_share.h"
#include "memmap.h"

#include "MFE_chip.h"



#if (defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&(!defined(_KERNEL_MODE_)))
	#include "MsIRQ.h"
	#include "MsOS.h"
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	#include "mdrv_system.h" //for addr_phy2log
#endif
#define MMAP_FAILED      0
#define MMAP_OK          0
#define MMAP_ERROR      -1

void MMAPInit(MFE_U32 buf_base, MFE_U32 buf_size, MFE_U32* ppBufStart)
{
	*ppBufStart = buf_base;
    ms_dprintk(DRV_L1, "MMAPInit: start:0x%x size:0x%x\n", (unsigned int)buf_base,(unsigned int) buf_size);
}

long MMAPMalloc(MFE_U32* ppBufStart, MFE_U32 pBufEnd, MFE_U32 size, MEMMAP_t * memmap, int nAlignBytes,const char* msg)
{
    //assert(memmap != NULL);
    ms_dprintk(DRV_L1, "MMAPMalloc[%s] 0x%x %d\n",msg, (unsigned int)*ppBufStart,(unsigned int) size);
//    size = (size + (nAlignBytes - 1)) & (~(nAlignBytes - 1));

    memmap->size = size;
    memmap->miuPointer = MMAP_FAILED;
    memmap->miuAddress = 0;

    //*ppBufStart == 0 is posspile in MIU1 
/*
    if(*ppBufStart == 0) {
        ms_dprintk(DRV_L1, "MMAPMalloc: global_buf_phy == 0 \n");
        return MMAP_ERROR;
    }
*/
	*ppBufStart = (*ppBufStart + (nAlignBytes - 1)) & (~(nAlignBytes - 1));
    memmap->miuAddress = *ppBufStart;
    *ppBufStart += size;
    if (*ppBufStart > pBufEnd) {
        ms_dprintk(DRV_L1, "Warning!! MFE MMAPMalloc over size!!, 0x%x 0x%x 0x%x\n"
            ,(unsigned int) *ppBufStart, (unsigned int)pBufEnd, (unsigned int)size);
    }
    memmap->miuPointer = (MFE_U8*)addr_phy2log(memmap->miuAddress);

    //ms_dprintk(DRV_L1,"MMAPMalloc:\n");
    ms_dprintk(DRV_L1,"memmap:miuPointer: 0x%08x,", (unsigned int)memmap->miuPointer);
    ms_dprintk(DRV_L1,"size  : 0x%08x,", (unsigned int)memmap->size);
    ms_dprintk(DRV_L1,"miuAddress: 0x%08x\n",(unsigned int) memmap->miuAddress);

    return MMAP_OK;
}

int reg_scan(unsigned short *reg_mask, MFE_U32 num_reg, proto_write write_func, proto_read read_func)
{
    MFE_U32 i=0, j=0;
    ms_dprintk(DRV_L1, "get in reg_scan %d\n", (unsigned int)num_reg);
    for (i=0; i<num_reg; i++) {
        unsigned short val, temp;
        for (j=1; j<reg_mask[i]; j<<=1) {
            if (!(i==0 && j==1)) {
                val = ((unsigned short)j)&reg_mask[i];
                write_func(i, val);
                read_func(i, &temp);
                if (val != temp) {
                    ms_dprintk(DRV_L1, "register scan error: reg:%d write:%d read:%d\n", (unsigned int)i, (unsigned int)val&reg_mask[i], (unsigned int)temp);
                }
            } else {
                ms_dprintk(DRV_L1, "passed reg %d\n", (unsigned int)i);
            }
        }
    }
    ms_dprintk(DRV_L1, "passed reg_scan\n");
    return 0;
}
