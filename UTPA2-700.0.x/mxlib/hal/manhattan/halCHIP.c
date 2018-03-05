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


//use flag to seperate later,for PPI int base
#ifdef CHIP_INT_SPI_MODE
#define INT_BASE 32
#else  
#define INT_BASE  128
#endif
// for SPI int base
//#define INT_BASE  32 

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_ECOS)

#include <cyg/kernel/kapi.h>

#include "MsCommon.h"
#include "MsOS.h"
#include "halIRQTBL.h"
#include "halCHIP.h"
#include "regCHIP.h"
#include "asmCPU.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// support 8 vector inerrupts on 4KEc
#define CHIP_LISR_MAX               2 //vector0: IRQ, vector1: FRQ, vector5: Timer INT


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//typedef void ( *LISR_Entry ) ( MS_S32 );
typedef struct
{
    cyg_handle_t                    stIntr;
    cyg_interrupt                   stIntrInfo;
} CHIP_LISR_Info;

typedef struct
{
    MS_BOOL                         bUsed;
    MS_BOOL                         bPending;
    //MS_BOOL                         priority;
    InterruptCb                     pIntCb;
} CHIP_HISR_Info;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static CHIP_LISR_Info               _LISR_Info[CHIP_LISR_MAX];
static MS_BOOL                      _bInLISR = FALSE;

static CHIP_HISR_Info               _HISR_Info[MS_IRQ_MAX];
static MS_BOOL                      _bInHISR = FALSE;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL CHIP_DisableIRQ(InterruptNum eIntNum);

// -- Jerry --
// Leave these to be chip independent. Different chip can have the opportunities to
// revise the priority policy for different interrupts.

//-------------------------------------------------------------------------------------------------
// ISR of IRQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return ISR result
//-------------------------------------------------------------------------------------------------
static MS_U32 _CHIP_LISR0(MS_U32 u32VectorNum, MS_U32 u32Data)
{
    MS_U32              u32Reg;
    MS_U32              u32Bit;
    IRQFIQNum           eVector;
    InterruptNum        eIntNum;

    u32Reg = 0;

    //in interrupt context
    _bInLISR = TRUE;

    u32Reg = IRQ_REG(REG_IRQ_PENDING_H);
    u32Reg <<=16;
    u32Reg |= IRQ_REG(REG_IRQ_PENDING_L);

    while (32!= (u32Bit = MAsm_CPU_GetTrailOne(u32Reg)))
    {
        if(u32Bit<16)
            eVector = (IRQFIQNum)(u32Bit+ E_IRQL_START);
        else
            eVector = (IRQFIQNum)(u32Bit+ E_IRQH_START);
        eIntNum = (InterruptNum)HWIdx2IntEnum[eVector];
        CHIP_DisableIRQ(eIntNum);
        if(_HISR_Info[eVector].bUsed)
        {
            _HISR_Info[eVector].bPending = TRUE;
        }
        u32Reg &= ~(0x1<< u32Bit);
    }

    u32Reg=0;
    u32Reg = IRQ_REG(REG_IRQEXP_PENDING_H);
    u32Reg <<=16;
    u32Reg |= IRQ_REG(REG_IRQEXP_PENDING_L);

    while (32!= (u32Bit = MAsm_CPU_GetTrailOne(u32Reg)))
    {
        if(u32Bit<16)
            eVector = (IRQFIQNum)(u32Bit+ E_IRQEXPL_START);
        else
            eVector = (IRQFIQNum)(u32Bit+ E_IRQEXPH_START);
        eIntNum = (InterruptNum)HWIdx2IntEnum[eVector];
        CHIP_DisableIRQ(eIntNum);
        if(_HISR_Info[eVector].bUsed)
        {
            _HISR_Info[eVector].bPending = TRUE;
        }
        u32Reg &= ~(0x1<< u32Bit);
    }

    // Mask this interrupt until the DSR completes.
    cyg_interrupt_mask( E_INTERRUPT_IRQ );  //why mask INT0 -> cause can still be 1 ???

    // Tell the processor that we have received the interrupt.
    cyg_interrupt_acknowledge( E_INTERRUPT_IRQ );

    _bInLISR = FALSE;

    // Tell the kernel that the ISR processing is done and the DSR needs to be executed next.
    return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
}


//-------------------------------------------------------------------------------------------------
// ISR of FIQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return ISR result
// @note    FIQ - handle interrupt service routine in ISR
//-------------------------------------------------------------------------------------------------
static MS_U32 _CHIP_LISR1(MS_U32 u32VectorNum, MS_U32 u32Data)
{
    MS_U32              u32Reg;
    MS_U32              u32Bit;
    IRQFIQNum           eVector;
    InterruptNum        eIntNum;

    //in interrupt context
    _bInLISR = TRUE;

    u32Reg=0;
    u32Reg = IRQ_REG(REG_FIQ_PENDING_H);
    u32Reg <<=16;
    u32Reg |= IRQ_REG(REG_FIQ_PENDING_L);

    while (32!= (u32Bit = MAsm_CPU_GetTrailOne(u32Reg)))
    {
        if(u32Bit<16)
            eVector = (IRQFIQNum)(u32Bit+ E_FIQL_START);
        else
            eVector = (IRQFIQNum)(u32Bit+ E_FIQH_START);
        eIntNum = (InterruptNum)HWIdx2IntEnum[eVector];
        CHIP_DisableIRQ(eIntNum);
        if(_HISR_Info[eVector].bUsed)
        {
            _HISR_Info[eVector].bPending = TRUE;
        }
        u32Reg &= ~(0x1<< u32Bit);
    }

    u32Reg=0;
    u32Reg = IRQ_REG(REG_FIQEXP_PENDING_H);
    u32Reg <<=16;
    u32Reg |= IRQ_REG(REG_FIQEXP_PENDING_L);

    while (32!= (u32Bit = MAsm_CPU_GetTrailOne(u32Reg)))
    {
        if(u32Bit<16)
            eVector = (IRQFIQNum)(u32Bit+ E_FIQEXPL_START);
        else
            eVector = (IRQFIQNum)(u32Bit+ E_FIQEXPH_START);
        eIntNum = (InterruptNum)HWIdx2IntEnum[eVector];
        CHIP_DisableIRQ(eIntNum);
        if(_HISR_Info[eVector].bUsed)
        {
            _HISR_Info[eVector].bPending = TRUE;
        }
        u32Reg &= ~(0x1<< u32Bit);
    }

    // Mask this interrupt until the ISR completes.
    cyg_interrupt_mask( E_INTERRUPT_FIQ );

    // Tell the processor that we have received the interrupt.
    cyg_interrupt_acknowledge( E_INTERRUPT_FIQ );

    _bInLISR = FALSE;
    return( CYG_ISR_HANDLED | CYG_ISR_CALL_DSR );
}


//-------------------------------------------------------------------------------------------------
// DSR of IRQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Count        \b IN: # of occurrences
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return None
//-------------------------------------------------------------------------------------------------
static void _CHIP_HISR0( MS_U32 u32VectorNum, MS_U32 u32Count, MS_U32 u32Data )
{
    InterruptNum        i;

    _bInHISR = TRUE; //in interrupt context

    // Process all pending DSRs, then enable relative IRQ again
    // The following SW processing flow decides the priorities from high to low
    //for loop later
    // IRQ H
    for (i= (InterruptNum)E_IRQL_START; i<= (InterruptNum)E_IRQL_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }
    for (i= (InterruptNum)E_IRQH_START; i<= (InterruptNum)E_IRQH_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }

    for (i= (InterruptNum)E_IRQEXPL_START; i<= (InterruptNum)E_IRQEXPL_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }

    for (i= (InterruptNum)E_IRQEXPH_START; i<= (InterruptNum)E_IRQEXPH_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }
    _bInHISR = FALSE;
    // Allow this interrupt to occur again.
    cyg_interrupt_unmask(E_INTERRUPT_IRQ);
}


static void _CHIP_HISR1( MS_U32 u32VectorNum, MS_U32 u32Count, MS_U32 u32Data )
{
    InterruptNum        i;

    _bInHISR = TRUE; //in interrupt context


    for (i= (InterruptNum)E_FIQL_START; i<=(InterruptNum) E_IRQL_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }
    for (i= (InterruptNum)E_FIQH_START; i<= (InterruptNum)E_FIQH_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }

    for (i= (InterruptNum)E_FIQEXPL_START; i<= (InterruptNum)E_FIQEXPL_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }

    for (i= (InterruptNum)E_FIQEXPH_START; i<= (InterruptNum)E_FIQEXPH_END; i++)
    {
        if ( _HISR_Info[i].bPending )
        {
            _HISR_Info[i].bPending = FALSE;
            _HISR_Info[i].pIntCb((InterruptNum)HWIdx2IntEnum[i]);
        }
    }

    //exit interrupt context
    _bInHISR = FALSE;

    // Allow this interrupt to occur again.
    cyg_interrupt_unmask(E_INTERRUPT_FIQ);
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL CHIP_EnableIRQ(InterruptNum eIntNum)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];

    if(_HISR_Info[u8VectorIndex].bUsed)
    {
        if (u8VectorIndex == E_IRQ_FIQ_ALL)
        {
            IRQ_REG(REG_IRQ_MASK_L) &= ~0xFFFF;
            IRQ_REG(REG_IRQ_MASK_H) &= ~0xFFFF;
            IRQ_REG(REG_IRQEXP_MASK_L) &= ~0xFFFF;
            IRQ_REG(REG_IRQEXP_MASK_H) &= ~0xFFFF;
            IRQ_REG(REG_FIQ_MASK_L) &= ~0xFFFF;
            IRQ_REG(REG_FIQ_MASK_H) &= ~0xFFFF;
            IRQ_REG(REG_FIQEXP_MASK_L) &= ~0xFFFF;
            IRQ_REG(REG_FIQEXP_MASK_H) &= ~0xFFFF;
        }
        else if ( /*(u8VectorIndex >= E_IRQL_START) &&*/ (u8VectorIndex <= (MS_U8)E_IRQL_END) )
        {
            IRQ_REG(REG_IRQ_MASK_L) &= ~(0x1 << (u8VectorIndex-E_IRQL_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_IRQH_START) && (u8VectorIndex <= (MS_U8)E_IRQH_END) )
        {
            IRQ_REG(REG_IRQ_MASK_H) &= ~(0x1 << (u8VectorIndex-E_IRQH_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_IRQEXPL_START) && (u8VectorIndex <= (MS_U8)E_IRQEXPL_END) )
        {
            IRQ_REG(REG_IRQEXP_MASK_L) &= ~(0x1 << (u8VectorIndex-E_IRQEXPL_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_IRQEXPH_START) && (u8VectorIndex <= (MS_U8)E_IRQEXPH_END) )
        {
            IRQ_REG(REG_IRQEXP_MASK_H) &= ~(0x1 << (u8VectorIndex-E_IRQEXPH_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_FIQL_START) && (u8VectorIndex <= (MS_U8)E_FIQL_END) )
        {
            IRQ_REG(REG_FIQ_MASK_L) &= ~(0x1 << (u8VectorIndex-E_FIQL_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_FIQH_START) && (u8VectorIndex <= (MS_U8)E_FIQH_END) )
        {
            IRQ_REG(REG_FIQ_MASK_H) &= ~(0x1 << (u8VectorIndex-E_FIQH_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_FIQEXPL_START) && (u8VectorIndex <= (MS_U8)E_FIQEXPL_END) )
        {
            IRQ_REG(REG_FIQEXP_MASK_L) &= ~(0x1 << (u8VectorIndex-E_FIQEXPL_START) );
        }
        else if ( (u8VectorIndex >= (MS_U8)E_FIQEXPH_START) && (u8VectorIndex <= (MS_U8)E_FIQEXPH_END) )
        {
            IRQ_REG(REG_FIQEXP_MASK_H) &= ~(0x1 << (u8VectorIndex-E_FIQEXPH_START) );
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}


MS_BOOL CHIP_DisableIRQ(InterruptNum eIntNum)
{
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];

    if (u8VectorIndex == E_IRQ_FIQ_ALL)
    {
        IRQ_REG(REG_IRQ_MASK_L) |= 0xFFFF;
        IRQ_REG(REG_IRQ_MASK_H) |= 0xFFFF;
        IRQ_REG(REG_IRQEXP_MASK_L) |= 0xFFFF;
        IRQ_REG(REG_IRQEXP_MASK_H) |= 0xFFFF;
        IRQ_REG(REG_FIQ_MASK_L) |= 0xFFFF;
        IRQ_REG(REG_FIQ_MASK_H) |= 0xFFFF;
        IRQ_REG(REG_FIQEXP_MASK_L) |= 0xFFFF;
        IRQ_REG(REG_FIQEXP_MASK_H) |= 0xFFFF;
    }
    else if ( /*(u8VectorIndex >= E_IRQL_START) && */(u8VectorIndex <= (MS_U8)E_IRQL_END) )
    {
        IRQ_REG(REG_IRQ_MASK_L) |= (0x1 << (u8VectorIndex-E_IRQL_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_IRQH_START) && (u8VectorIndex <= (MS_U8)E_IRQH_END) )
    {
        IRQ_REG(REG_IRQ_MASK_H) |= (0x1 << (u8VectorIndex-E_IRQH_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_IRQEXPL_START) && (u8VectorIndex <= (MS_U8)E_IRQEXPL_END) )
    {
        IRQ_REG(REG_IRQEXP_MASK_L) |= (0x1 << (u8VectorIndex-E_IRQEXPL_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_IRQEXPH_START) && (u8VectorIndex <= (MS_U8)E_IRQEXPH_END) )
    {
        IRQ_REG(REG_IRQEXP_MASK_H) |= (0x1 << (u8VectorIndex-E_IRQEXPH_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_FIQL_START) && (u8VectorIndex <= (MS_U8)E_FIQL_END) )
    {
        IRQ_REG(REG_FIQ_MASK_L) |= (0x1 << (u8VectorIndex-E_FIQL_START) );
        IRQ_REG(REG_FIQ_CLEAR_L) = (0x1 << (u8VectorIndex-E_FIQL_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_FIQH_START) && (u8VectorIndex <= (MS_U8)E_FIQH_END) )
    {
        IRQ_REG(REG_FIQ_MASK_H) |= (0x1 << (u8VectorIndex-E_FIQH_START) );
        IRQ_REG(REG_FIQ_CLEAR_H) = (0x1 << (u8VectorIndex-E_FIQH_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_FIQEXPL_START) && (u8VectorIndex <= (MS_U8)E_FIQEXPL_END) )
    {
        IRQ_REG(REG_FIQEXP_MASK_L) |= (0x1 << (u8VectorIndex-E_FIQEXPL_START) );
        IRQ_REG(REG_FIQEXP_CLEAR_L) = (0x1 << (u8VectorIndex-E_FIQEXPL_START) );
    }
    else if ( (u8VectorIndex >= (MS_U8)E_FIQEXPH_START) && (u8VectorIndex <= (MS_U8)E_FIQEXPH_END) )
    {
        IRQ_REG(REG_FIQEXP_MASK_H) |= (0x1 << (u8VectorIndex-E_FIQEXPH_START) );
        IRQ_REG(REG_FIQEXP_CLEAR_H) = (0x1 << (u8VectorIndex-E_FIQEXPH_START) );
    }
    return TRUE;
}


MS_BOOL CHIP_AttachISR(InterruptNum eIntNum, InterruptCb pIntCb)
{
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];
    _HISR_Info[u8VectorIndex].pIntCb = pIntCb;
    _HISR_Info[u8VectorIndex].bUsed = TRUE;

    return TRUE;
}


MS_BOOL CHIP_DetachISR(InterruptNum eIntNum)
{
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];
    _HISR_Info[u8VectorIndex].bUsed = FALSE;

    return TRUE;
}


MS_BOOL CHIP_InISRContext(void)
{
    if (_bInLISR || _bInHISR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void CHIP_InitISR(void)
{
    MS_U32              i;

	HAL_InitIrqTable();

    for( i = 0; i < MS_IRQ_MAX; i++)
    {
        _HISR_Info[i].bUsed = FALSE;
        _HISR_Info[i].bPending = FALSE;
    }

    // Create the interrupt (0: IRQ register ISR/DSR;  1:FIQ register ISR)
    cyg_interrupt_create( E_INTERRUPT_IRQ, 0, 0, (cyg_ISR_t *)_CHIP_LISR0, (cyg_DSR_t *)_CHIP_HISR0, &_LISR_Info[0].stIntr, &_LISR_Info[0].stIntrInfo);
    cyg_interrupt_create( E_INTERRUPT_FIQ, 0, 1, (cyg_ISR_t*)_CHIP_LISR1, (cyg_DSR_t *)_CHIP_HISR1, &_LISR_Info[1].stIntr, &_LISR_Info[1].stIntrInfo);

    // Attach the interrupt created to the vector.
    cyg_interrupt_attach( _LISR_Info[0].stIntr );
    cyg_interrupt_attach( _LISR_Info[1].stIntr );

    // Unmask the interrupt we just configured.
    cyg_interrupt_unmask( E_INTERRUPT_IRQ );
    cyg_interrupt_unmask( E_INTERRUPT_FIQ );
}
#endif

#if defined (MSOS_TYPE_LINUX)


#include <fcntl.h>
#include <errno.h>

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>

#include "MsCommon.h"
#include "MsOS.h"
#include "halIRQTBL.h"
#include "regCHIP.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// support 8 vector inerrupts on 4KEc
#define CHIP_LISR_MAX               2 //vector0: IRQ, vector1: FRQ, vector5: Timer INT
#define MAX_NAME                   30 //max thread_name_length
#define SEND_ACK                    0 //send ack to kernel before executing registered ISR



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
struct irq_desc {
    void    *driverp ;
    void  (*handler)(InterruptNum eIntNum) ;
    int     irqfd ;
    MS_U16  u16irq ;
};

struct pollfd
{
    int fd;         /* File descriptor to poll.  */
    short int events;       /* Types of events poller cares about.  */
    short int revents;      /* Types of events that actually occurred.  */
};

typedef struct
{
    MS_BOOL bUsed;
    MS_BOOL bPending;
    MS_BOOL bEnable;
    pthread_t ithr ;
    InterruptCb pIntCb;
    void   *pThreadParam;
} CHIP_HISR_Info ;
typedef unsigned long int nfds_t;
extern int poll (struct pollfd *__fds, nfds_t __nfds, int __timeout);

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static CHIP_HISR_Info               _HISR_Info[MS_IRQ_MAX];
static MS_BOOL                      _bInHISR = FALSE ;
static MS_BOOL                      _bInLISR = FALSE ;
//static MS_BOOL                      _bEnableAll = FALSE ;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

// -- Jerry --
// Leave these to be chip independent. Different chip can have the opportunities to
// revise the priority policy for different interrupts.

//-------------------------------------------------------------------------------------------------
// ISR of IRQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return ISR result
//-------------------------------------------------------------------------------------------------
/*
static MS_U32 _CHIP_LISR0(MS_U32 u32VectorNum, MS_U32 u32Data)
{
    return FALSE ;
}
*/


//-------------------------------------------------------------------------------------------------
// ISR of FIQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return ISR result
// @note    FIQ - handle interrupt service routine in ISR
//-------------------------------------------------------------------------------------------------
/*
static MS_U32 _CHIP_LISR1(MS_U32 u32VectorNum, MS_U32 u32Data)
{
    return FALSE ;
}
*/


//-------------------------------------------------------------------------------------------------
// DSR of IRQ
// @param  u32VectorNum    \b IN: 0: IRQ  1: FIQ
// @param  u32Count        \b IN: # of occurrences
// @param  u32Data         \b IN: argument 3 of cyg_interrupt_create
// @return None
//-------------------------------------------------------------------------------------------------
/*
static void _CHIP_HISR0( MS_U32 u32VectorNum, MS_U32 u32Count, MS_U32 u32Data )
{
}


static void _CHIP_HISR1( MS_U32 u32VectorNum, MS_U32 u32Count, MS_U32 u32Data )
{
}
*/


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

static void *interrupt_thread(void *arg)
{
    struct irq_desc *ip = (struct irq_desc *)arg;
    int fd = ip->irqfd;
    int err;
    struct pollfd       PollFd;
    int irq;
    char irq_thd_name[MAX_NAME];

    //naming the irq thread
    irq=ip->u16irq+INT_BASE;
    memset(irq_thd_name,'\0',sizeof(irq_thd_name));
    snprintf(irq_thd_name,MAX_NAME-1,"IRQThread_%d",irq);
    prctl(PR_SET_NAME, (unsigned long)irq_thd_name, NULL, NULL, NULL);
    memset(irq_thd_name,'\0',sizeof(irq_thd_name));
    prctl(PR_GET_NAME, (unsigned long)irq_thd_name, NULL, NULL, NULL);
    //printf("%s\n",irq_thd_name);

    PollFd.fd= fd;
    PollFd.events= POLLIN;
    PollFd.revents= 0;

    for (;;)
    {
        if(!_HISR_Info[ip->u16irq].bUsed)
        {
            //normal exit
                break;
        }


        err=poll(&PollFd, 1, 100);
        if (err == -1)
        {
            if(errno==EINTR)
                {
                    continue;
                }
            else
            {
                printf("IRQ %d ",(ip->u16irq+INT_BASE));
                perror("polling error!!");
                break;
                }
            }
        else if(PollFd.revents &(0x08|0x10|0x20)) //<= we can not include poll.h so use 0x08=POLLERR 0x10=POLLHUP 0x20=POLLNVAL
        {
            printf("IRQ %d ",(ip->u16irq+INT_BASE));
            perror("polling error!!");
            break;
        }

        if(PollFd.revents & POLLIN)
        {
            //after successful polling, interrupt had been disable by Kernel
            _HISR_Info[(IRQFIQNum)ip->u16irq].bEnable = FALSE;
#if SEND_ACK == 1
            int enable = E_IRQ_ACK;
            write(fd, &enable, sizeof(enable));
#endif
           (void)(ip->handler)((InterruptNum)HWIdx2IntEnum[ip->u16irq]);
        }


    }

    return NULL;
}

MS_BOOL CHIP_EnableAllInterrupt(void)
{
    //_bEnableAll = TRUE ;
    return TRUE ;
}

MS_BOOL CHIP_DisableAllInterrupt(void)
{
    //_bEnableAll = FALSE ;
    return TRUE ;
}

static MS_BOOL CHIP_ProcessIRQ(InterruptNum eIntNum, IrqDebugOpt eIrqDebugOpt)
{
    int opt = eIrqDebugOpt;
    int fd;
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];

    if (_HISR_Info[u8VectorIndex].pThreadParam)
    {
        fd = ((struct irq_desc *)_HISR_Info[u8VectorIndex].pThreadParam)->irqfd;
        write(fd, &opt, sizeof(opt));
    }

	if (eIrqDebugOpt == E_IRQ_ENABLE)
	{
		_HISR_Info[u8VectorIndex].bEnable = TRUE ;
	}
	else if (eIrqDebugOpt == E_IRQ_DISABLE) 
	{
		_HISR_Info[u8VectorIndex].bEnable = FALSE ;
	}

    return TRUE ;
}

MS_BOOL CHIP_DebugIRQ(InterruptNum eIntNum, IrqDebugOpt eIrqDebugOpt)
{
    return CHIP_ProcessIRQ(eIntNum, eIrqDebugOpt);
}

MS_BOOL CHIP_EnableIRQ(InterruptNum eIntNum)
{
    return CHIP_ProcessIRQ(eIntNum, E_IRQ_ENABLE);
}

MS_BOOL CHIP_DisableIRQ(InterruptNum eIntNum)
{
    return CHIP_ProcessIRQ(eIntNum, E_IRQ_DISABLE);
}

MS_BOOL CHIP_CompleteIRQ(InterruptNum eIntNum)
{
    return CHIP_ProcessIRQ(eIntNum, E_IRQ_COMPLETE);
}

void *UTL_memset( void *d, int c, size_t n )
{
    MS_U8 *pu8Dst = d;
    register MS_U32 u32Cnt = n;
    register MS_U32 u32Val;
    register MS_U32 *pu32Dst;

    c &= 0xff;

    while ((MS_VIRT)pu8Dst & 3 && u32Cnt)
    {
        *pu8Dst++ = (MS_U8)c;
        u32Cnt--;
    }

    pu32Dst = (MS_U32 *)pu8Dst;
    u32Val = (c << 8) | c;
    u32Val = (u32Val << 16) | u32Val;
    while (u32Cnt >= 32)
    {
        pu32Dst[0]= u32Val;
        pu32Dst[1]= u32Val;
        pu32Dst[2]= u32Val;
        pu32Dst[3]= u32Val;
        pu32Dst[4]= u32Val;
        pu32Dst[5]= u32Val;
        pu32Dst[6]= u32Val;
        pu32Dst[7]= u32Val;
        pu32Dst += 8;
        u32Cnt -= 32;
    }

    while (u32Cnt >= 4)
    {
        *pu32Dst++ = u32Val;
        u32Cnt -= 4;
    }

    pu8Dst = (MS_U8 *)pu32Dst;
    while (u32Cnt)
    {
        *pu8Dst++ = (MS_U8)c;
        u32Cnt--;
    }

    return d;
}


MS_BOOL CHIP_AttachISR(InterruptNum eIntNum, InterruptCb pIntCb)
{
    int fd = 0;
    char name[48];
    struct irq_desc *idp ;
    pthread_attr_t attr;
    struct sched_param schp;
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];

    idp = ( struct irq_desc*)malloc(sizeof(*idp));
    MS_ASSERT(idp != NULL);
    snprintf(name, sizeof(name)-1,  "/proc/irq/%d/irq", (u8VectorIndex+INT_BASE));
    //printf("name=%s\n", name);

    fd = open(name, O_RDWR|O_EXCL);
    if (fd < 0)
    {
        printf("Cannot open interrupt descriptor for irq=%d ", (MS_U16)(u8VectorIndex+INT_BASE));
        perror("");
        free(idp);
        return FALSE ;
    }

    idp->irqfd = fd;
    idp->u16irq = (MS_U16)u8VectorIndex ;
    idp->driverp = &(idp->u16irq) ;
    idp->handler = (pIntCb);

    UTL_memset(&schp, 0, sizeof(schp));
    schp.sched_priority = sched_get_priority_max(SCHED_FIFO);

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedparam(&attr, &schp);

    _HISR_Info[u8VectorIndex].pIntCb = pIntCb;
    _HISR_Info[u8VectorIndex].pThreadParam = idp;
    _HISR_Info[u8VectorIndex].bUsed = TRUE ;
    _HISR_Info[u8VectorIndex].bEnable = FALSE;

    pthread_create(&_HISR_Info[u8VectorIndex].ithr, &attr, interrupt_thread, idp);


    return TRUE;
}


MS_BOOL CHIP_DetachISR(InterruptNum eIntNum)
{
    MS_U8 u8VectorIndex = 0;

    u8VectorIndex = (MS_U8)IntEnum2HWIdx[eIntNum];

    if(TRUE == _HISR_Info[u8VectorIndex].bEnable)
    {
        CHIP_DisableIRQ(eIntNum);
    }

    _HISR_Info[u8VectorIndex].bUsed = FALSE ;

    if( _HISR_Info[u8VectorIndex].ithr ) {
        int ret;
        if((ret=pthread_join( _HISR_Info[u8VectorIndex].ithr,NULL ))!=0)
        {
            printf("IRQ %d ", (MS_U16)(u8VectorIndex+INT_BASE));
            perror("polling thread destroy failed");
        }
        else
        {
            printf("IRQ %d polling thread destroyed\n", (MS_U16)(u8VectorIndex+INT_BASE));

        }
        ret=ret;
       _HISR_Info[u8VectorIndex].ithr = 0 ;
    }



    if(_HISR_Info[u8VectorIndex].pThreadParam)
    {
        int ret;
        if(-1==ioctl(((struct irq_desc *)_HISR_Info[u8VectorIndex].pThreadParam) ->irqfd, 137))
        {
            printf("%s.%d ioctl fail\n",__FUNCTION__,__LINE__);
        }
        if((ret=close(((struct irq_desc *)_HISR_Info[u8VectorIndex].pThreadParam) ->irqfd))==-1)
        {
            printf("IRQ %d ", (MS_U16)(u8VectorIndex+INT_BASE));
            perror("polling fd close failed");
        }
        else
        {
            printf("IRQ %d polling fd closed!!\n", (MS_U16)(u8VectorIndex+INT_BASE));
        }
        ret=ret;
        free(_HISR_Info[u8VectorIndex].pThreadParam);
      _HISR_Info[u8VectorIndex].pThreadParam = NULL;
    }

    return TRUE;
}


MS_BOOL CHIP_InISRContext(void)
{
    if (_bInLISR || _bInHISR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void CHIP_InitISR(void)
{
    MS_U16 i ;

	HAL_InitIrqTable();

    //printf("Init\n");
    for( i = 0 ; i < MS_IRQ_MAX ; i++)
    {
        _HISR_Info[i].bUsed = 0 ;
        _HISR_Info[i].bPending = 0 ;
        _HISR_Info[i].bEnable = 0 ;
        _HISR_Info[i].ithr = 0 ;
        _HISR_Info[i].pIntCb = 0 ;
        _HISR_Info[i].pThreadParam = NULL;
    }
    // printf("+pthread_mutex_init\n");
    // pthread_mutex_init(&_HISR_Info,NULL);
    //printf("-CHIP_InitISR\n");
}

#endif
