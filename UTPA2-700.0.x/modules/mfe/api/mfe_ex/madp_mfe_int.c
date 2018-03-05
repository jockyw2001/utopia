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

#ifndef __UBOOT__
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#endif

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "madp_ms_dprintf.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef __MFE_G2__
#include <fcntl.h>
#include <sys/ioctl.h>
#endif


#ifdef _MFE_UTOPIA_
static MS_S32 madp_sem_input = -1;


//#define NO_INTERRUPT

void MDrv_MFE_ISR(InterruptNum eIntNum)
{
    ms_dprintf(ADP_L2, "get in MDrv_MFE_ISR\n");
    MsOS_ReleaseSemaphore(madp_sem_input);
}
#else
static MS_S32 mfe_irq_fd = -1;
#endif

void mfe_wait_int(MS_S32 isIsrMode)
{

    if(!isIsrMode)
        return;
#ifdef _MFE_UTOPIA_

    ms_dprintf(ADP_L2, "Wait MFE ISR...\n");
    if(MsOS_ObtainSemaphore(madp_sem_input, MSOS_WAIT_FOREVER) == FALSE) {
//    if(MsOS_ObtainSemaphore(madp_sem_input, 300) == FALSE) {
        ms_dprintk(ADP_L0, "Wait MFE ISR...fail!!\n");
        MsOS_DisableInterrupt((InterruptNum)E_INT_IRQ_MFE);
    }
#else
    if (mfe_irq_fd >= 0)
    {
        MS_U32 icount;
        read(mfe_irq_fd, &icount, 4);
    }
#endif
}

void mfe_open_int(void)
{
#ifdef _MFE_UTOPIA_
    if(madp_sem_input != -1)
        return;
    madp_sem_input = MsOS_CreateSemaphore(0,E_MSOS_FIFO ,"ADP_MFE_SEM") ;
    MsOS_AttachInterrupt((InterruptNum)E_INT_IRQ_MFE,MDrv_MFE_ISR);
#elif defined(WIN32)
#else
    if (mfe_irq_fd < 0)
    {
        mfe_irq_fd = open("/dev/int_mfe", O_RDWR | O_SYNC);
        if (mfe_irq_fd < 0)
        {
            ms_dprintf(ADP_L0,"ERR: MFE IRQ Open Failed\n");
            return;
        }

        ms_dprintf(ADP_L1,"NOTE: MFE IRQ Open OK\n");
    }
#endif
}

void mfe_close_int(void)
{
#ifdef _MFE_UTOPIA_
    if(madp_sem_input != -1) {
        MsOS_ReleaseSemaphore(madp_sem_input);
        MsOS_DeleteSemaphore(madp_sem_input);
        MsOS_DisableInterrupt((InterruptNum)E_INT_IRQ_MFE);
        MsOS_DetachInterrupt(E_INT_IRQ_MFE);
        madp_sem_input = -1;
    }
#elif defined(WIN32)
#else
    if (mfe_irq_fd >= 0)
    {
        close(mfe_irq_fd);
        mfe_irq_fd = -1;

        ms_dprintf(ADP_L1,"NOTE: MFE IRQ Close OK\n");
    }
#endif
}
