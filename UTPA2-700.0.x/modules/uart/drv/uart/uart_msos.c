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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSC_msos.c
/// @brief  SmartCard Driver MsOS Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(OS_ECOS) || defined(OS_LINUX))


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvUART.h"

// Internal Definition
#include "uart_msos.h"
#include "regUART.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static P_UART_ISR_Proc                _os_UART_ISR_Proc[UART_PIU_NUM]; // in case there should be multiple ISR


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

static void _OS_UART_ISR0(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[0](0);
    MsOS_EnableInterrupt(UART0_PIU_IRQ);
}

#ifdef UART1_PIU_ADDR
static void _OS_UART_ISR1(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[1](1);
    MsOS_EnableInterrupt(UART1_PIU_IRQ);
}
#endif

#ifdef UART2_PIU_ADDR
static void _OS_UART_ISR2(InterruptNum eIntNum)
{
    _os_UART_ISR_Proc[2](2);
    MsOS_EnableInterrupt(UART2_PIU_IRQ);
}
#endif

void OS_UART_ISR_Enable(MS_U8 u8UARTID, P_UART_ISR_Proc pfISRProc)
{
    if (u8UARTID == 0)
    {
        _os_UART_ISR_Proc[0] = pfISRProc;
        MsOS_AttachInterrupt(UART0_PIU_IRQ, _OS_UART_ISR0);
        MsOS_EnableInterrupt(UART0_PIU_IRQ);
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        _os_UART_ISR_Proc[1] = pfISRProc;
        MsOS_AttachInterrupt(UART1_PIU_IRQ, (InterruptCb)_OS_UART_ISR1);
        MsOS_EnableInterrupt(UART1_PIU_IRQ);
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        _os_UART_ISR_Proc[2] = pfISRProc;
        MsOS_AttachInterrupt(UART2_PIU_IRQ, (InterruptCb)_OS_UART_ISR2);
        MsOS_EnableInterrupt(UART2_PIU_IRQ);
    }
#endif
}

void OS_UART_ISR_Disable(MS_U8 u8UARTID)
{
    if (u8UARTID == 0)
    {
        MsOS_DisableInterrupt(UART0_PIU_IRQ);
        MsOS_DetachInterrupt (UART0_PIU_IRQ);
        _os_UART_ISR_Proc[0] = NULL;
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        MsOS_DisableInterrupt(UART1_PIU_IRQ);
        MsOS_DetachInterrupt (UART1_PIU_IRQ);
        _os_UART_ISR_Proc[1] = NULL;
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        MsOS_DisableInterrupt(UART2_PIU_IRQ);
        MsOS_DetachInterrupt (UART2_PIU_IRQ);
        _os_UART_ISR_Proc[2] = NULL;
    }
#endif
}

MS_BOOL OS_UART_Init(MS_U8 u8UARTID)
{

    if (u8UARTID == 0)
    {
        MsOS_DisableInterrupt(UART0_PIU_IRQ);
        MsOS_DetachInterrupt (UART0_PIU_IRQ);
    }

#ifdef UART1_PIU_ADDR
    if (u8UARTID == 1)
    {
        MsOS_DisableInterrupt(UART1_PIU_IRQ);
        MsOS_DetachInterrupt (UART1_PIU_IRQ);
    }
#endif

#ifdef UART2_PIU_ADDR
    if (u8UARTID == 2)
    {
        MsOS_DisableInterrupt(UART2_PIU_IRQ);
        MsOS_DetachInterrupt (UART2_PIU_IRQ);
    }
#endif

    return TRUE;
}


MS_BOOL OS_UART_Exit(void)
{
    return TRUE;
}

#endif //OS_ECOS
