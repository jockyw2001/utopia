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
/// file    Mdrv_sc_isr.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _MDRV_XC_ISR_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/sched.h>  // for task_normal
#endif
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"

// Internal Definition
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "xc_Analog_Reg.h"
#include "mhal_sc.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SC_ISR_DBG(x)       //x

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


/******************************************************************************/
/*                           Local Variables                                    */
/******************************************************************************/

/******************************************************************************/
/*               P r i v a t e    F u n c t i o n s                             */
/******************************************************************************/
//-------------------------------------------------------------------------------------------------
/// Convert SC_INT_SRC bit shift case
/// @param eIntNum      \b IN: the interrupt number of API layer
/// @return @ref SC_INT_SRC the interrupt number of driver layer
//-------------------------------------------------------------------------------------------------
static MS_U8 _MApi_XC_GetIntSrc(SC_INT_SRC eIntNum)
{
    MS_U8 u8Int_bit;

    switch(eIntNum)
    {
        // 0x04
        default    				  :
        case SC_INT_DIPW          : u8Int_bit = IRQ_INT_DIPW          ;    break;
#ifdef IRQ_INT_MEMSYNC_MAIN
        case SC_INT_MEMSYNC_MAIN  : u8Int_bit = IRQ_INT_MEMSYNC_MAIN  ;    break;
#endif
        case SC_INT_RESERVED1     : u8Int_bit = IRQ_INT_RESERVED1     ;    break;

        case SC_INT_VSINT         : u8Int_bit = IRQ_INT_VSINT         ;    break;
        case SC_INT_F2_VTT_CHG    : u8Int_bit = IRQ_INT_F2_VTT_CHG    ;    break;
        case SC_INT_F1_VTT_CHG    : u8Int_bit = IRQ_INT_F1_VTT_CHG    ;    break;
        case SC_INT_F2_VS_LOSE    : u8Int_bit = IRQ_INT_F2_VS_LOSE    ;    break;
        case SC_INT_F1_VS_LOSE    : u8Int_bit = IRQ_INT_F1_VS_LOSE    ;    break;
        case SC_INT_F2_JITTER     : u8Int_bit = IRQ_INT_F2_JITTER     ;    break;
        case SC_INT_F1_JITTER     : u8Int_bit = IRQ_INT_F1_JITTER     ;    break;
        case SC_INT_F2_IPVS_SB    : u8Int_bit = IRQ_INT_F2_IPVS_SB    ;    break;
        case SC_INT_F1_IPVS_SB    : u8Int_bit = IRQ_INT_F1_IPVS_SB    ;    break;
        case SC_INT_F2_IPHCS_DET  : u8Int_bit = IRQ_INT_F2_IPHCS_DET  ;    break;
        case SC_INT_F1_IPHCS_DET  : u8Int_bit = IRQ_INT_F1_IPHCS_DET  ;    break;

        // 0x10
        case SC_INT_PWM_RP_L_INT  : u8Int_bit = IRQ_INT_PWM_RP_L_INT  ;    break;
        case SC_INT_PWM_FP_L_INT  : u8Int_bit = IRQ_INT_PWM_FP_L_INT  ;    break;
        case SC_INT_F2_HTT_CHG    : u8Int_bit = IRQ_INT_F2_HTT_CHG    ;    break;
        case SC_INT_F1_HTT_CHG    : u8Int_bit = IRQ_INT_F1_HTT_CHG    ;    break;
        case SC_INT_F2_HS_LOSE    : u8Int_bit = IRQ_INT_F2_HS_LOSE    ;    break;
        case SC_INT_F1_HS_LOSE    : u8Int_bit = IRQ_INT_F1_HS_LOSE    ;    break;
        case SC_INT_PWM_RP_R_INT  : u8Int_bit = IRQ_INT_PWM_RP_R_INT  ;    break;
        case SC_INT_PWM_FP_R_INT  : u8Int_bit = IRQ_INT_PWM_FP_R_INT  ;    break;

        case SC_INT_F2_CSOG       : u8Int_bit = IRQ_INT_F2_CSOG       ;    break;
        case SC_INT_F1_CSOG       : u8Int_bit = IRQ_INT_F1_CSOG       ;    break;
        case SC_INT_F2_RESERVED2  : u8Int_bit = IRQ_INT_F2_RESERVED2  ;    break;
        case SC_INT_F1_RESERVED2  : u8Int_bit = IRQ_INT_F1_RESERVED2  ;    break;
        case SC_INT_F2_ATP_READY  : u8Int_bit = IRQ_INT_F2_ATP_READY  ;    break;
        case SC_INT_F1_ATP_READY  : u8Int_bit = IRQ_INT_F1_ATP_READY  ;    break;
        case SC_INT_F2_RESERVED3  : u8Int_bit = IRQ_INT_F2_RESERVED3  ;    break;
        case SC_INT_F1_RESERVED3  : u8Int_bit = IRQ_INT_F1_RESERVED3  ;    break;
    }

    return u8Int_bit;
}

MS_U32 _MDrv_SC_get_interrupt(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32Reg = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK00_10_L);
#if IRQ_CLEAN_INKERNEL
    MS_U32 u32DummyReg = SC_R4BYTE(psXCInstPri->u32DeviceID, INTERRUPT_DUMMY_REGISTER);

    return u32Reg|(u32DummyReg&(SUPPORTED_KERNEL_INT>> (SC_INT_START - IRQ_INT_START)));
#endif
    return u32Reg;
}

static void _MDrv_SC_clear_interrupt(void *pInstance, SC_INT_SRC u8IntSrc)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U8 u8IntRemap = _MApi_XC_GetIntSrc(u8IntSrc);

    if(u8IntRemap < 16)
    {
#if IRQ_CLEAN_INKERNEL
        MS_U32 u32DummyReg = SC_R4BYTE(psXCInstPri->u32DeviceID, INTERRUPT_DUMMY_REGISTER);
        MS_U32 u32DummyRegSrc = u32DummyReg << (SC_INT_START - IRQ_INT_START);

        if(SUPPORTED_KERNEL_INT&(1 << u8IntSrc))
        {
            if( u32DummyRegSrc&(1 << u8IntSrc))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID,INTERRUPT_DUMMY_REGISTER,0, (1 << (u8IntSrc-(SC_INT_START - IRQ_INT_START))));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_12_L , BIT(u8IntRemap),BIT(u8IntRemap));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID,INTERRUPT_DUMMY_REGISTER, (1 << (u8IntSrc-(SC_INT_START - IRQ_INT_START))), (1 << (u8IntSrc-(SC_INT_START - IRQ_INT_START))));
            }
        }
        else
#endif
        {
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION >= 2)
            if((u8IntRemap != IRQ_INT_F2_IPVS_SB))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_12_L , BIT(u8IntRemap),BIT(u8IntRemap));
            }
#else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_12_L , BIT(u8IntRemap),BIT(u8IntRemap));
#endif
        }
    }
    else
    {
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION >= 2)
        //IRQ_INT_PWM_RP_R_INT and IRQ_INT_PWM_FP_R_INT is dummy will be cleaned in kernel
#if IRQ_CLEAN_INKERNEL
        MS_U32 u32DummyReg = SC_R4BYTE(psXCInstPri->u32DeviceID, INTERRUPT_DUMMY_REGISTER);
        MS_U32 u32DummyRegSrc = u32DummyReg << (SC_INT_START - IRQ_INT_START);

        if(SUPPORTED_KERNEL_INT&(1 << u8IntSrc))
        {
            if( u32DummyRegSrc&(1 << u8IntSrc))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID,INTERRUPT_DUMMY_REGISTER_H,0, (1 << ((u8IntSrc-(SC_INT_START - IRQ_INT_START))-16)));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_13_L , BIT(u8IntRemap-16) ,BIT(u8IntRemap-16));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID,INTERRUPT_DUMMY_REGISTER_H, (1 << ((u8IntSrc-(SC_INT_START - IRQ_INT_START))-16)), (1 << ((u8IntSrc-(SC_INT_START - IRQ_INT_START))-16)));
            }
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_13_L , BIT(u8IntRemap-16) ,BIT(u8IntRemap-16));
        }
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_13_L , BIT(u8IntRemap-16) ,BIT(u8IntRemap-16));
#endif
    }
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

void MDrv_SC_set_interrupt(void *pInstance, SC_INT_SRC u8IntSrc, MS_BOOL bEnable)
{
#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8Bank;
#endif
    MS_U8 u8IntRemap = _MApi_XC_GetIntSrc(u8IntSrc);

#if (!ENABLE_REGISTER_SPREAD)
    u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(bEnable)
    {
        if(u8IntRemap < 16)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_12_L ,BIT(u8IntRemap),BIT(u8IntRemap));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_14_L , 0 ,BIT(u8IntRemap));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_13_L ,BIT(u8IntRemap-16),BIT(u8IntRemap-16));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_15_L , 0 ,BIT(u8IntRemap-16));
        }
    }
    else
    {
        if(u8IntRemap < 16)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_14_L , BIT(u8IntRemap),BIT(u8IntRemap));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_12_L , 0,BIT(u8IntRemap));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_15_L ,BIT(u8IntRemap-16),BIT(u8IntRemap-16));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_13_L , 0 ,BIT(u8IntRemap-16));
        }

    }
#if (!ENABLE_REGISTER_SPREAD)
    MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
}

#ifdef MSOS_TYPE_LINUX_KERNEL
void MDrv_SC_vsync_isr(void)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stXC_Utility._XC_VSyncRun)
    {
        pXCResourcePrivate->stXC_Utility._XC_VSyncCount++;
        if (pXCResourcePrivate->stXC_Utility._XC_VSyncCount >= pXCResourcePrivate->stXC_Utility._XC_VSyncMax)
        {
            pXCResourcePrivate->stXC_Utility._XC_EventFlag = 1;
            wake_up(&pXCResourcePrivate->stXC_Utility._XC_EventQueue);
        }
    }
}
#endif // #ifdef MSOS_TYPE_LINUX_KERNEL


#ifdef MSOS_TYPE_LINUX_KERNEL
irqreturn_t MDrv_SC_isr(InterruptNum eIntNum, void* dev_id)
#else
void MDrv_SC_isr( InterruptNum eIntNum )
#endif
{
    void *pInstance = pu32XCInst_private;
    MS_U32 u32IntSource;
    SC_INT_SRC eSCIntNum;
    MS_U8 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    #if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8OldISRBank;
    #endif
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    UNUSED(eIntNum);

    // we can't quarantee ISR thread not being context switched out, so if we change bank here, it's very dangerous
    // need to obtain mutex here
#ifdef MSOS_TYPE_LINUX_KERNEL
    #define _XC_MUTEX_WAIT_TIMEOUT  10
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    if(!MsOS_ObtainMutex(_XC_Mutex, _XC_MUTEX_WAIT_TIMEOUT))
    {
        SC_ISR_DBG(printf("ISR return\n"));

        // re-enable IRQ_DISP
        MsOS_EnableInterrupt(E_INT_IRQ_DISP);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return IRQ_HANDLED;
     }
#else
#ifndef MSOS_TYPE_ECOS
    if(!MsOS_ObtainMutex(_XC_Mutex, MSOS_WAIT_FOREVER))
    {
        SC_ISR_DBG(printf("ISR return\n"));

        // re-enable IRQ_DISP
        MsOS_EnableInterrupt(E_INT_IRQ_DISP);
        return;
    }
#endif
#endif
    // read XC ISR status

    #if (!ENABLE_REGISTER_SPREAD)
    u8OldISRBank = MDrv_ReadByte(BK_SELECT_00);
    #endif

    u32IntSource = _MDrv_SC_get_interrupt(pInstance);

    u32IntSource = u32IntSource << (SC_INT_START - IRQ_INT_START);
    SC_ISR_DBG(printf("ISR %8lx\n", u32IntSource));

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(u32IntSource & (1 << SC_INT_VSINT))
    {
        MDrv_SC_vsync_isr();
    }
#endif
    for(eSCIntNum=SC_INT_DIPW; eSCIntNum<MAX_SC_INT; eSCIntNum++)
    {
        if(u32IntSource & (1 << eSCIntNum))
        {
            // this Interrupt happened, clear it
            _MDrv_SC_clear_interrupt(pInstance, eSCIntNum);

            // call ISR
            for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
            {
                if(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR != NULL)
                {
                    // execute ISR
                    (void)(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR) ((SC_INT_SRC)eSCIntNum, pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR_Param);
                }
            }
        }
    }

#if (!ENABLE_REGISTER_SPREAD)
    MDrv_WriteByte(BK_SELECT_00, u8OldISRBank);
#endif

#ifndef MSOS_TYPE_ECOS
    MsOS_ReleaseMutex(_XC_Mutex);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    MsOS_EnableInterrupt(E_INT_IRQ_DISP);
    return IRQ_HANDLED;
#else
    // re-enable IRQ_DISP
    MsOS_EnableInterrupt(E_INT_IRQ_DISP);
#endif
}

#ifdef MSOS_TYPE_LINUX_KERNEL
irqreturn_t MDrv_SC1_isr(InterruptNum eIntNum, void* dev_id)
#else
void MDrv_SC1_isr( InterruptNum eIntNum )
#endif
{
    void *pInstance = pu32XCInst_1_private;
    MS_U32 u32IntSource;
    SC_INT_SRC eSCIntNum;
    MS_U8 i;

#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8OldISRBank;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    UNUSED(eIntNum);

    // we can't quarantee ISR thread not being context switched out, so if we change bank here, it's very dangerous
    // need to obtain mutex here
#ifdef MSOS_TYPE_LINUX_KERNEL
#define _XC_MUTEX_WAIT_TIMEOUT  10
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    if(!MsOS_ObtainMutex(_XC_Mutex, _XC_MUTEX_WAIT_TIMEOUT))
    {
        SC_ISR_DBG(printf("ISR return\n"));

        // re-enable IRQ_DISP
        MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return IRQ_HANDLED;
     }
#else
#ifndef MSOS_TYPE_ECOS
    if(!MsOS_ObtainMutex(_XC_Mutex, MSOS_WAIT_FOREVER))
    {
        SC_ISR_DBG(printf("ISR return\n"));

        // re-enable IRQ_DISP
        MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
        return;
    }
#endif
#endif
    // read XC ISR status

#if (!ENABLE_REGISTER_SPREAD)
    u8OldISRBank = MDrv_ReadByte(BK_SELECT_00);
#endif

    u32IntSource = _MDrv_SC_get_interrupt(pInstance);

    u32IntSource = u32IntSource << (SC_INT_START - IRQ_INT_START);
    SC_ISR_DBG(printf("ISR %8lx\n", u32IntSource));

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(u32IntSource & (1 << SC_INT_VSINT))
    {
        MDrv_SC_vsync_isr();
    }
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(eSCIntNum=SC_INT_DIPW; eSCIntNum<MAX_SC_INT; eSCIntNum++)
    {
        if(u32IntSource & (1 << eSCIntNum))
        {
            // this Interrupt happened, clear it
            _MDrv_SC_clear_interrupt(pInstance, eSCIntNum);

            // call ISR
            for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
            {
                if(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR != NULL)
                {
                    // execute ISR
                    (void)(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR) ((SC_INT_SRC)eSCIntNum, pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eSCIntNum][i].aryXC_ISR_Param);
                }
            }
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#ifdef MSOS_TYPE_LINUX_KERNEL
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#endif

#if (!ENABLE_REGISTER_SPREAD)
    MDrv_WriteByte(BK_SELECT_00, u8OldISRBank);
#endif

#ifndef MSOS_TYPE_ECOS
    MsOS_ReleaseMutex(_XC_Mutex);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    MsOS_EnableInterrupt(E_INT_IRQ_DISP);
    return IRQ_HANDLED;
#else
    // re-enable IRQ_DISP
    MsOS_EnableInterrupt(E_INT_IRQ_DISP1);
#endif
}


//////////////////////////////////
// Interrupt Function
//////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Get maximum number of interrupts that scaler supports
/// @return @ref MS_U8 maximum number of interrupts that scaler supports
//-------------------------------------------------------------------------------------------------
MS_U8   MDrv_XC_InterruptGetMaxIntNum(void *pInstance)
{
    UNUSED(pInstance);
    return MAX_SC_INT;
}

//-------------------------------------------------------------------------------------------------
/// After get maximum number of interrupts by @ref MDrv_XC_InterruptGetMaxIntNum, you can
/// check each interrupt supported btuy scaler or not.
/// @param eIntNum      \b IN: the interrupt number to query
/// @return @ref MS_BOOL the interrupt number supported or not
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_InterruptAvaliable(void *pInstance, SC_INT_SRC eIntNum)
{
    MS_U8 i = 0;
    MS_BOOL bReturn = FALSE;

    // check if out of range or not supported
    if((eIntNum >= MDrv_XC_InterruptGetMaxIntNum(pInstance)) || (((1UL << eIntNum) & SUPPORTED_XC_INT) == 0))
    {
        return bReturn;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // search avaliable CB to attach
    for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
    {
        if(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR == NULL)
        {
            bReturn = TRUE;
            break;
        }
    }

    return bReturn;
}

//-------------------------------------------------------------------------------------------------
/// Check if ISR already attached or not because we allowed to attach same ISR for many times
/// @param eIntNum @ref SC_INT_SRC          \b IN: the interrupt number that you want to attach
/// @param pIntCb  @ref SC_InterruptCb      \b IN: the ISR that you want to attach to the interrupt number
/// @param pParam                           \b IN: the parameter that you want to pass to the ISR when called
/// @return @ref MS_BOOL Is ISR already attached or not.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_InterruptIsAttached(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    MS_U8 i = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
    {
        if((pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR == pIntCb) && (pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR_Param == pParam))
        {
            return TRUE;
        }
    }

    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Attach interrupt to specified interrupt number
/// @param eIntNum @ref SC_INT_SRC          \b IN: the interrupt number that you want to attach
/// @param pIntCb  @ref SC_InterruptCb      \b IN: the ISR that you want to attach to the interrupt number
/// @param pParam                           \b IN: the parameter that you want to pass to the ISR when called
/// @return @ref MS_BOOL attach successed or not, FALSE means can't attach more ISR
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_InterruptAttach(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    MS_BOOL bReturn = FALSE;
    _XC_ENTRY(pInstance);
    bReturn = MDrv_XC_InterruptAttachWithoutMutex(pInstance, eIntNum, pIntCb, pParam);
    _XC_RETURN(pInstance);
    return bReturn;
}

//-------------------------------------------------------------------------------------------------
/// DeAttach interrupt to specified interrupt number
/// @param eIntNum @ref SC_INT_SRC          \b IN: the interrupt number that you want to de-attach
/// @param pIntCb  @ref SC_InterruptCb      \b IN: the ISR that you want to de-attach to the interrupt number
/// @param pParam                           \b IN: the parameter that you want to pass to the ISR when called
/// @return @ref MS_BOOL attach successed or not, FALSE means can't de-attach this ISR because it not exist
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_InterruptDeAttach(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    MS_BOOL bReturn = FALSE;

    _XC_ENTRY(pInstance);
    bReturn = MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, eIntNum, pIntCb, pParam);
    _XC_RETURN(pInstance);
    return bReturn;
}

MS_BOOL MDrv_XC_InterruptAttachWithoutMutex(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    MS_U8 i = 0;
    MS_BOOL bReturn = FALSE;

    // do SetWindow in AN to create DS script,so this isn't need attach interrupt function
    if (g_bSWDSGenScenario == TRUE)
    {
        printf("ISRAttach failed %d ! do SetWindow in AN to create DS script,so this isn't need attach interrupt function!\n", eIntNum);
        return bReturn;
    }

    // check if out of range or not supported
    if((eIntNum >= MDrv_XC_InterruptGetMaxIntNum(pInstance)) || (((1UL << eIntNum) & SUPPORTED_XC_INT) == 0))
    {
        SC_ISR_DBG(printf("ISRAttach failed %d (max %d, supported %lx)\n", eIntNum, MDrv_XC_InterruptGetMaxIntNum(pInstance), (MS_U32)SUPPORTED_XC_INT));
        return bReturn;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // attach ISR
    for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
    {
        if(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR == NULL)
        {
            // disable interrupt first to avoid racing condition, for example, interrupt come in after set pIntCb but before set pParam, will call ISR and passing parameter NULL
            MDrv_SC_set_interrupt(pInstance, eIntNum, DISABLE);

            pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR_Param = pParam;
            pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR = pIntCb;

            // enable interrupt
            MDrv_SC_set_interrupt(pInstance, eIntNum, ENABLE);

            SC_ISR_DBG(printf("ISRAttach %d successed to [%d][%d]\n", eIntNum, eIntNum, i));

            bReturn = TRUE;
            break;
        }
    }

    return bReturn;
}


MS_BOOL MDrv_XC_InterruptDeAttachWithoutMutex(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    MS_U8 i = 0;
    MS_BOOL bReturn = FALSE;

    // check if out of range or not supported
    if((eIntNum >= MDrv_XC_InterruptGetMaxIntNum(pInstance)) || (((1UL << eIntNum) & SUPPORTED_XC_INT) == 0))
    {
        SC_ISR_DBG(printf("ISRDeAttach failed %d (max %d, supported %lx)\n", eIntNum, MDrv_XC_InterruptGetMaxIntNum(pInstance), SUPPORTED_XC_INT));
        return bReturn;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // de-attach the ISR
    for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
    {
        if((pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR == pIntCb) &&
            (pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR_Param == pParam))
        {
            // disable interrupt first to avoid racing condition
            MDrv_SC_set_interrupt(pInstance, eIntNum, DISABLE);

            pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR = NULL;
            pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR_Param = NULL;
            bReturn = TRUE;

            SC_ISR_DBG(printf("ISRDeAttach %d successed to [%d][%d]\n", eIntNum, eIntNum, i));
            break;
        }
    }

    // re-enable interrupt
    if(bReturn)
    {
        // removed ISR, check if there is other ISR left
        for(i=0; i<MAX_ISR_NUM_OF_EACH_INT; i++)
        {
            if(pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[eIntNum][i].aryXC_ISR != NULL)
            {
                // re-enable interrupt
                MDrv_SC_set_interrupt(pInstance, eIntNum, ENABLE);
                SC_ISR_DBG(printf("ISRAttach %d re-enable ISR\n", eIntNum));
            }
        }
    }

    return bReturn;
}
#undef _MDRV_XC_ISR_C_

