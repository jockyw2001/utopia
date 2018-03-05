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

#define _MHAL_MBX_INTERRUPT_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "drvMBX.h"
#include "regMBXINT.h"
#include "halMBXINT.h"

//=============================================================================
// Compile options
//=============================================================================

//=============================================================================
// Local Defines
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================
//#define MBXINT_DEBUG
#ifdef MBXINT_DEBUG
    #define MBXINT_ERROR(fmt, args...)           printf("[MBX INT Driver USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MBXINT_WARN(fmt, args...)            printf("[MBX INT Driver WARN][%06d]    " fmt, __LINE__, ## args)
    #define MBXINT_PRINT(fmt, args...)           printf("[MBX INT Driver][%06d]     " fmt, __LINE__, ## args)
    #define MBXINT_ASSERT(_cnd, _fmt, _args...)    \
                                    if (!(_cnd)) {              \
                                        MBXINT_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define MBXINT_ERROR(fmt, args...)
    #define MBXINT_WARN(fmt, args...)
    #define MBXINT_PRINT(fmt, args...)
    #define MBXINT_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================
static MBX_MSGRECV_CB_FUNC _pMBXMsgRecvCbFunc = NULL;
static MS_U32 _u32RIUBaseAddrMBXINT = 0;

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MHAL_MBXINT_INTHandler(InterruptNum vector);
static MBX_Result _MHAL_MBXINT_SetHostCPU(MBX_CPU_ID eHostCPUID);

//=============================================================================
// Local Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt, schedule tasklet
/// @param  irq                  \b IN: interrupt number
/// @param  dev_id                  \b IN: dev id
/// @return irqreturn_t: IRQ_HANDLED
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MHAL_MBXINT_INTHandler(InterruptNum eIntNum)
{
     if(NULL == _pMBXMsgRecvCbFunc)
    {
        return;
    }

    _pMBXMsgRecvCbFunc(eIntNum);

    MsOS_EnableInterrupt(eIntNum);
}

//-------------------------------------------------------------------------------------------------
/// Set Interrupt to Host CPU ID: Enable related interrupt and attached related callback.
/// @param  eHostCPUID                  \b IN: The Host CPU ID
/// @return E_MBX_SUCCESS
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result _MHAL_MBXINT_SetHostCPU(MBX_CPU_ID eHostCPUID)
{
    switch(eHostCPUID)
    {
        case E_MBX_CPU_PM:
        {
            MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_8051, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_MIPS_VPE0_TO_8051, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_EnableInterrupt(E_INT_FIQ_MIPS_VPE0_TO_8051);
        }
        break;

        case E_MBX_CPU_AEON:
        {
            MsOS_AttachInterrupt(E_INT_FIQ_8051_TO_AEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_BEON_TO_AEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_EnableInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_EnableInterrupt(E_INT_FIQ_BEON_TO_AEON);
        }
        break;

        case E_MBX_CPU_MIPS:
        {
            MsOS_AttachInterrupt(E_INT_FIQ_8051_TO_MIPS_VPE0, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_BEON , (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_EnableInterrupt(E_INT_FIQ_8051_TO_MIPS_VPE0);
            MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_BEON);
        }
        break;

        default:
            return E_MBX_ERR_INVALID_CPU_ID;
    }

    return E_MBX_SUCCESS;
}

//=============================================================================
// Mailbox HAL Interrupt Driver Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt INIT
/// @param  eHostCPU                  \b IN: interrupt owner
/// @param  pMBXRecvMsgCBFunc                  \b IN: callback func by driver
/// @param  u32RIUBaseAddrMBXINT                  \b IN: RIU Base Addr with platform
/// @return E_MBX_ERR_INVALID_CPU_ID: the cpu id is wrong
/// @return E_MBX_UNKNOW_ERROR: request_irq failed;
/// @return E_MBX_SUCCESS: success;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_Init (MBX_CPU_ID eHostCPU, MBX_MSGRECV_CB_FUNC pMBXRecvMsgCBFunc, MS_U32 u32RIUBaseAddrMBXINT)
{
    _pMBXMsgRecvCbFunc = pMBXRecvMsgCBFunc;
    _u32RIUBaseAddrMBXINT = u32RIUBaseAddrMBXINT;

    return _MHAL_MBXINT_SetHostCPU(eHostCPU);
}

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt DeINIT
/// @param  eHostCPU                  \b IN: interrupt owner
/// @return void;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void MHAL_MBXINT_DeInit (MBX_CPU_ID eHostCPU)
{
    switch(eHostCPU)
    {
        case E_MBX_CPU_PM:
        {
            MsOS_DisableInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_DisableInterrupt(E_INT_FIQ_MIPS_VPE0_TO_8051);
            MsOS_DetachInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_DetachInterrupt(E_INT_FIQ_MIPS_VPE0_TO_8051);
        }
        break;

        case E_MBX_CPU_AEON:
        {
            MsOS_DisableInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_DisableInterrupt(E_INT_FIQ_MIPS_VPE0_TO_AEON);
            MsOS_DetachInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_DetachInterrupt(E_INT_FIQ_MIPS_VPE0_TO_AEON);
        }
        break;

        case E_MBX_CPU_MIPS:
        {
            MsOS_DisableInterrupt(E_INT_FIQ_8051_TO_MIPS_VPE0);
            MsOS_DisableInterrupt(E_INT_FIQ_AEON_TO_MIPS_VPE0);
            MsOS_DetachInterrupt(E_INT_FIQ_8051_TO_MIPS_VPE0);
            MsOS_DetachInterrupt(E_INT_FIQ_AEON_TO_MIPS_VPE0);
        }
        break;

        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
/// Reset Host CPU for MBX Interrupt
/// @param  ePrevCPU                  \b IN: previous host cpu id
/// @param  eConfigCpu                  \b IN: new configed cpu id
/// @return E_MBX_SUCCESS: success;
/// @return E_MBX_INVALID_CPU_ID
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_ResetHostCPU (MBX_CPU_ID ePrevCPU, MBX_CPU_ID eConfigCpu)
{
    MHAL_MBXINT_DeInit(ePrevCPU);

    return _MHAL_MBXINT_SetHostCPU(eConfigCpu);
}

//-------------------------------------------------------------------------------------------------
/// Fire Interrupt
/// @param  dstCPUID                  \b IN: dst cpu of interrupt
/// @param  srcCPUID                  \b IN: src cpu of interrupt
/// @return E_MBX_SUCCESS: success;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_Fire (MBX_CPU_ID dstCPUID, MBX_CPU_ID srcCPUID)
{
    MBXINT_ASSERT((dstCPUID!=srcCPUID),"dst cpu is the same as src cpu!\n");

#if defined(MSOS_TYPE_NUTTX)
	srcCPUID = E_MBX_CPU_AEON;
	dstCPUID = E_MBX_CPU_MIPS; 

	CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_H2;
	CPU_INT_REG(REG_INT_AEONFIRE) &= ~INT_AEON_H2;

	return  E_MBX_SUCCESS; 

#endif

    switch(srcCPUID)
    {
        case E_MBX_CPU_PM:
        {
            if(dstCPUID==E_MBX_CPU_AEON)
            {
                //PM51-to-R2
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_PM_AEON);
                CPU_INT_REG(REG_INT_PMFIRE) |= INT_PM_AEON;
                CPU_INT_REG(REG_INT_PMFIRE) &= ~(INT_PM_AEON);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
            else
            {
                //PM51-to-ARM Core 0
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_PM_H2);
                CPU_INT_REG(REG_INT_PMFIRE) |= INT_PM_H2;
                CPU_INT_REG(REG_INT_PMFIRE) &= ~(INT_PM_H2);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
        }
        break;

        case E_MBX_CPU_AEON:
        {
            if(dstCPUID==E_MBX_CPU_PM)
            {
                //R2-to-PM51
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_AEON_PM);
                CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_PM;
                CPU_INT_REG(REG_INT_AEONFIRE) &= ~(INT_AEON_PM);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
            else
            {
                //R2-to-ARM Core 0
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_AEON_H2);
                CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_H2;
                CPU_INT_REG(REG_INT_AEONFIRE) &= ~(INT_AEON_H2);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
        }
        break;

        case E_MBX_CPU_MIPS:
        {
            if(dstCPUID==E_MBX_CPU_PM)
            {
                //ARM Core 0-to-PM51
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_H2_PM);
                CPU_INT_REG(REG_INT_H2FIRE) |= INT_H2_PM;
                CPU_INT_REG(REG_INT_H2FIRE) &= ~(INT_H2_PM);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
            else
            {
                //ARM Core 0-to-R2
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_H2_AEON);
                CPU_INT_REG(REG_INT_H2FIRE) |= INT_H2_AEON;
                CPU_INT_REG(REG_INT_H2FIRE) &= ~(INT_H2_AEON);
                MBXINT_PRINT("FIQ [32-47] Mask = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47] Status = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
        }
        break;

        default:
        {
            MBXINT_ASSERT(FALSE, "wrong src cpu!\n");
        }
        break;
    }

    return E_MBX_SUCCESS;
}

MBX_Result MHAL_MBXINT_CpuInit(MBX_CPU_ID eHKCPU)
{
    if((eHKCPU == E_MBX_CPU_AEON) || \
       (eHKCPU == E_MBX_CPU_MIPS) || \
       (eHKCPU == E_MBX_CPU_MIPS_VPE1))
    {
        return E_MBX_SUCCESS;
    }

    MBXINT_ASSERT(FALSE, "[MHAL_MBXINT_CpuInit] Invalid HK CPU ID \n");

    return E_MBX_ERR_INVALID_CPU_ID;
}
