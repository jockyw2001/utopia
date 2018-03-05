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
//==============================================================================
// [mhal_mux.c]
// Date: 20090220
// Descriptions: Add a new mux layer for HW setting
//==============================================================================

#define  MHAL_MUX_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia_dapi.h"
// Registers
#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
//#include "hwreg_chiptop.h"
#include "hwreg_hdmi.h"
#include "hwreg_pm_sleep.h"//alex_tung
#include "hwreg_ipmux.h"

#include "apiXC.h"

#include "xc_Analog_Reg.h"

#include "mhal_mux.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_ADC_Internal.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mhal_adc.h"
#include "drvXC_HDMI_if.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_sc.h"
#include "mhal_hdmi.h"
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


//-------------------------------------------------------------------------------------------------
//  Local Structures
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


void Hal_SC_mux_set_dvi_mux(void *pInstance, MS_U8 PortId, E_OUTPUT_PORT_TYPE enDstPort)
{
    MS_U8 u8RxConnectionCount = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    //_XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE0))
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE0)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW]))
    {
        u8RxConnectionCount++;
    }

    if(IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[SUB_WINDOW]))
    {
        u8RxConnectionCount++;
    }
    //_XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE0));

    //_XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE1))
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE1)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW]))
    {
        u8RxConnectionCount++;
    }
    //_XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(E_XC_DEVICE1));

    // If deleting a HDMI Rx path and there are still other paths connect to Rx
    if(enDstPort == HAL_OUTPUTPORT_NONE_PORT)
    {
        if(u8RxConnectionCount > 1)
        {
            return;
        }
    }
    // If connecting to HDMI Rx and there are some other paths already connected to Rx
    else
    {
        if(u8RxConnectionCount > 0)
        {
            return;
        }
    }
    UNUSED(enDstPort);

    switch (PortId)
    {
        case 0: // DVI0
            W2BYTEMSK(REG_ADC_ATOP_61_L, 0x05, 0x0F); //data/clk a switch
            W2BYTEMSK(REG_ADC_ATOP_6A_L, 0x00, BIT(0));
            break;
        case 1: // DVI1
            W2BYTEMSK(REG_ADC_ATOP_61_L, 0x0A, 0x0F); //data/clk a switch
            W2BYTEMSK(REG_ADC_ATOP_6A_L, 0x00, BIT(0));
            break;
        case 2: // DVI2
            W2BYTEMSK(REG_ADC_ATOP_61_L, 0x00, 0x0F); //data/clk a switch
            W2BYTEMSK(REG_ADC_ATOP_6A_L, 1 , BIT(0));
            break;
        default:
            break;
    }
}

void Hal_SC_mux_set_analog_mux(MS_U8 PortId)
{
    // 0x2501
    W2BYTEMSK(REG_ADC_ATOP_01_L, PortId & 0x03, 0x03);
}

void Hal_SC_mux_set_adc_y_mux(void *pInstance, MS_U8 PortId)
{
    // ADC VD Ymux
    W2BYTEMSK(REG_ADC_ATOP_02_L, PortId , BITMASK(3:0) );
}

void Hal_SC_mux_set_adc_c_mux(void *pInstance, MS_U8 PortId)
{
    // ADC VD Cmux
    W2BYTEMSK(REG_ADC_ATOP_02_L, (PortId<<4), BITMASK(7:4) ); // ADC_VD_CMUX_MASK
}
#if 0
static void _Hal_SC_mux_set_cvbsout1_y_mux(E_INPUT_PORT_TYPE u8Ymux)
{
    MS_U16 u16mux;

    if( u8Ymux >= HAL_INPUTPORT_YMUX_CVBS0 && u8Ymux < (HAL_INPUTPORT_YMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        u16mux = u8Ymux - HAL_INPUTPORT_YMUX_CVBS0;
        W2BYTEMSK(REG_ADC_ATOP_39_L, ((u16mux<<4)|u16mux) , LBMASK);
    }
    // else part
    // ATV has different source which is coming VIF or CVBS, but mux api does not know this information.
    // For this reason, we move CVBS out control to Hal_ADC_set_cvbs_out in Euclid.
}
#endif
static void _Hal_SC_mux_set_cvbsout1_c_mux(E_INPUT_PORT_TYPE u8Cmux)
{
    MS_U16 u16mux;
    if( u8Cmux >= HAL_INPUTPORT_CMUX_CVBS0 && u8Cmux < (HAL_INPUTPORT_CMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        u16mux = u8Cmux - HAL_INPUTPORT_CMUX_CVBS0;
        W2BYTEMSK(REG_ADC_ATOP_39_L, ((u16mux<<4)|u16mux)<<8 , HBMASK);
    }
    // else part
    // ATV has different source which is coming VIF or CVBS, but mux api does not know this information.
    // For this reason, we move CVBS out control to Hal_ADC_set_cvbs_out in Euclid.
}
#if 0
static void _Hal_SC_mux_set_cvbsout2_y_mux(E_INPUT_PORT_TYPE u8Ymux)
{
    MS_U16 u16mux;

    if( u8Ymux >= HAL_INPUTPORT_YMUX_CVBS0 && u8Ymux < (HAL_INPUTPORT_YMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        u16mux = u8Ymux - HAL_INPUTPORT_YMUX_CVBS0;
        W2BYTEMSK(REG_ADC_ATOP_3D_L, ((u16mux<<4)|u16mux) , LBMASK);
    }
    // else part
    // ATV has different source which is coming VIF or CVBS, but mux api does not know this information.
    // For this reason, we move CVBS out control to Hal_ADC_set_cvbs_out in Euclid.
}
#endif
static void _Hal_SC_mux_set_cvbsout2_c_mux(E_INPUT_PORT_TYPE u8Cmux)
{
    MS_U16 u16mux;
    if( u8Cmux >= HAL_INPUTPORT_CMUX_CVBS0 && u8Cmux < (HAL_INPUTPORT_CMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        u16mux = u8Cmux - HAL_INPUTPORT_CMUX_CVBS0;
        W2BYTEMSK(REG_ADC_ATOP_3D_L, ((u16mux<<4)|u16mux)<<8 , HBMASK);
    }
    // else part
    // ATV has different source which is coming VIF or CVBS, but mux api does not know this information.
    // For this reason, we move CVBS out control to Hal_ADC_set_cvbs_out in Euclid.
}

void Hal_SC_mux_set_mainwin_ip_mux( void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL, *pXCResourcePrivate_GetAnotherTEEStatus = NULL;
    MS_U32 DeviceID = 0;
    MS_U32* u32XCInst = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(psXCInstPri->u32DeviceID == 0)
    {
        DeviceID = 1;
        u32XCInst = pu32XCInst_1;
    }
    else if(psXCInstPri->u32DeviceID == 1)
    {
        DeviceID = 0;
        u32XCInst = pu32XCInst;
    }

    if(u32XCInst != NULL)
    {
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(DeviceID)],(void**)(&pXCResourcePrivate_GetAnotherTEEStatus));
    }

    ///pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt = 0;
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt <=OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_01_L;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = (u8Data_Mux << 4);
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0xF0;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    else if((u32XCInst == NULL) ||((pXCResourcePrivate_GetAnotherTEEStatus->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == FALSE) &&
        (pXCResourcePrivate_GetAnotherTEEStatus->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == FALSE)))
    {

        if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        {
            MDrv_WriteByteMask(REG_IPMUX_01_L, u8Data_Mux << 4, 0xF0);
        }
        else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        {
        MDrv_WriteByteMask(REG_IPMUX_03_L, u8Data_Mux << 4, 0xF0);
        }
        else
        {
            // Invalid XC device ID
        }
    }

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
        MDrv_WriteByteMask(REG_CKG_IDCLK2, u8Clk_Mux << 2, CKG_IDCLK2_MASK);
    }
    else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
    {
        MDrv_WriteByteMask(REG_CKG_SC1_IDCLK2, u8Clk_Mux << 2, CKG_SC1_IDCLK2_MASK);
    }
    else
    {
        // Invalid XC device ID
    }
}

void Hal_SC_set_subwin_ip_mux( void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL, *pXCResourcePrivate_GetAnotherTEEStatus = NULL;
    MS_U32 DeviceID = 0;
    MS_U32* u32XCInst = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(psXCInstPri->u32DeviceID == 0)
    {
        DeviceID = 1;
        u32XCInst = pu32XCInst_1;
    }
    else if(psXCInstPri->u32DeviceID == 1)
    {
        DeviceID = 0;
        u32XCInst = pu32XCInst;
    }

    if(u32XCInst != NULL)
    {
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(DeviceID)],(void**)(&pXCResourcePrivate_GetAnotherTEEStatus));
    }
    ///pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt = 0;
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt <=OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_01_L;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = u8Data_Mux;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0x0F;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    else if((u32XCInst == NULL) ||((pXCResourcePrivate_GetAnotherTEEStatus->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == FALSE) &&
        (pXCResourcePrivate_GetAnotherTEEStatus->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == FALSE)))
    {
        MDrv_WriteByteMask(REG_IPMUX_01_L, u8Data_Mux, 0x0F);
    }
    MDrv_WriteByteMask(REG_CKG_IDCLK1, u8Clk_Mux << 2, CKG_IDCLK1_MASK);
}

void Hal_SC_mux_set_dipwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    MDrv_WriteByteMask(REG_IPMUX_02_L, u8Data_Mux << 4, 0xF0);
    MDrv_WriteByteMask(REG_CKG_IDCLK3, u8Clk_Mux << 2, CKG_IDCLK3_MASK);
}

MS_BOOL Hal_SC_mux_get_mainwin_ip_mux( void *pInstance, MS_U8 *pU8Data_Mux, MS_U8 *pU8Clk_Mux)
{
    MS_BOOL bRet = FALSE;
    if(pU8Data_Mux && pU8Clk_Mux)
    {
        *pU8Data_Mux = (MDrv_ReadRegBit(REG_IPMUX_01_L, 0xF0)) >> 4;
        *pU8Clk_Mux = (MDrv_ReadRegBit(REG_CKG_IDCLK2, CKG_IDCLK2_MASK)) >> 2;
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    return bRet;
}

/******************************************************************************/
///Dispatch Port
///@param src \b IN: source type
///@param dest \b IN: pixel clock
/******************************************************************************/
void Hal_SC_mux_dispatch(void *pInstance, E_MUX_INPUTPORT src_port , E_MUX_OUTPUTPORT dest_port)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 _src = (MS_U8) src_port;
    MS_U8 _dest = (MS_U8) dest_port;

    // Hal level in T2 is following MsCommon.h directly.
    // It is unnecessary to add a mapping function.
    E_INPUT_PORT_TYPE src = (E_INPUT_PORT_TYPE) _src;
    E_OUTPUT_PORT_TYPE dest = (E_OUTPUT_PORT_TYPE) _dest;

    // Src = Analog 0ort 0 ~ Max number of analog port
    if ( src >= HAL_INPUTPORT_ANALOG0  && src < (HAL_INPUTPORT_ANALOG0 + NUMBER_OF_ANALOG_PORT ))
    {
        // Do nothing, not support in Kano
    }

    // Src = Y mux port 0 ~ Max number of Y mux port
    else if (  src >= HAL_INPUTPORT_YMUX_CVBS0 && src < (HAL_INPUTPORT_YMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        // Do nothing, not support in Kano
    }

    // Src = C mux port 0 ~ Max number of C mux port
    else if ( src >= HAL_INPUTPORT_CMUX_CVBS0 && src < (HAL_INPUTPORT_CMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        // Src -> ( IPmux )
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW || dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1)
            Hal_SC_mux_set_adc_c_mux(pInstance, src - HAL_INPUTPORT_CMUX_CVBS0);

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            ; // Controlled by Ymux
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            ; // Controlled by Ymux
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS1 )
        {
            _Hal_SC_mux_set_cvbsout1_c_mux(src);
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS2 )
        {
            _Hal_SC_mux_set_cvbsout2_c_mux(src);
        }
        else
        {
            // TODO:
        }

    }

    // Src = DVI port 0 ~ Max number of DVI port
    else if ( src >= HAL_INPUTPORT_DVI0 && src < (HAL_INPUTPORT_DVI0 + NUMBER_OF_DVI_PORT) )
    {
        if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        {
            // Src -> ( IPmux )
            Hal_SC_mux_set_dvi_mux(pInstance, src - HAL_INPUTPORT_DVI0, dest);

            // IPmux -> Dest
            if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
            {
                Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_HDR, SC_MAINWIN_IPMUX_HDMI_DVI);
                //Set HDR IPMUX and Clock
                if (src == HAL_INPUTPORT_DVI0)
                {
                    MDrv_WriteByteMask(REG_IPMUX_HDR, SC_HDR_IPMUX_HDMI_DVI, 0x0F);
                    MDrv_WriteByteMask(REG_CLK_HDR, SC_CLK_HDMI_DVI << 2, CKG_IDCLK2_MASK);
                }
                else
                {
                }
            }
            else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
            {
                Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_HDMI_DVI , SC_SUBWIN_IPMUX_HDMI_DVI);
            }
            else
            {
                // TODO:
            }
        }
        else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        {
            // Src -> ( IPmux )
            Hal_SC_mux_set_dvi_mux(pInstance, src - HAL_INPUTPORT_DVI0, dest);

            // IPmux -> Dest
            if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
            {
                Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_HDMI_DVI, SC_MAINWIN_IPMUX_HDMI_DVI);
            }
            else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
            {
                Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_HDMI_DVI , SC_SUBWIN_IPMUX_HDMI_DVI);
            }
            else
            {
                // TODO:
            }
        }
    }
    else if ( src >= HAL_INPUTPORT_MVOP && src < (HAL_INPUTPORT_MVOP + NUMBER_OF_MVOP_PORT) )
    {
        // Src -> ( IPmux )
        //remove below flow and wait HW ECO because of FD_MASK signal can NOT pass HDR module
#if 0
        if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        {
            // IPmux -> Dest
            if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_HDR, SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_HDR, SC_CLK_DC1);
                //Set HDR IPMUX and Clock
                if (src == HAL_INPUTPORT_MVOP)
                {
                    MDrv_WriteByteMask(REG_IPMUX_HDR, SC_HDR_IPMUX_MVOP, 0x0F);
                    MDrv_WriteByteMask(REG_CLK_HDR, SC_CLK_DC0 << 2, CKG_IDCLK2_MASK);
                }
                else if (src == HAL_INPUTPORT_MVOP2)
                {
                    MDrv_WriteByteMask(REG_IPMUX_HDR, SC_HDR_IPMUX_MVOP2, 0x0F);
                    MDrv_WriteByteMask(REG_CLK_HDR, SC_CLK_DC1 << 2, CKG_IDCLK2_MASK);
                }
                else
                {
                }
            }
            else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP , SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP2 , SC_CLK_DC1);
            }
            else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP , SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP2 , SC_CLK_DC1);
            }
            else
            {
            }
        }
        else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
#endif
        {
            // IPmux -> Dest
            if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_MVOP, SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_MVOP2, SC_CLK_DC1);

            }
            else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP , SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP2 , SC_CLK_DC1);

            }
            else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
            {
                if (src == HAL_INPUTPORT_MVOP)
                    Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP , SC_CLK_DC0);
                else if (src == HAL_INPUTPORT_MVOP2)
                    Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP2 , SC_CLK_DC1);
            }
            else
            {
            }
        }
    }
    else if ( src >= HAL_INPUTPORT_SCALER_OP && src < (HAL_INPUTPORT_SCALER_OP + NUMBER_OF_SCALER_OP_PORT) )
    {
        // Src -> ( IPmux )

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
            {
                Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC1_MAINWIN_IPMUX_H2V2, SC1_CLK_PRE_H2V2);
            }
            else
            {
                //Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_CAPTURE, SC_CLK_ODCLK);
            }
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            //Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_CAPTURE, SC_CLK_ODCLK);
        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
        {
            Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC1_MAINWIN_IPMUX_OP1 , SC_CLK_ODCLK);
        }
        else
        {
            // Undefeined
        }
    }
    else
    {
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            Hal_SC_mux_set_mainwin_ip_mux(pInstance, 0x0F, 0x0F);
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            Hal_SC_set_subwin_ip_mux(pInstance, 0x0F , 0x0F);
        }
    }
}

void Hal_SC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable)
{
    // Enable DIP window clock
     if(bEnable)
     {
         // Enable IDClk3
         MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_INVERT);                   // Not Invert
         MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_GATED);                    // Enable clock
     }
     else
     {
         // Disable IDClk3
         MDrv_WriteRegBit(REG_CKG_IDCLK3, ENABLE, CKG_IDCLK3_INVERT);                   // Not Invert
         MDrv_WriteRegBit(REG_CKG_IDCLK3, ENABLE, CKG_IDCLK3_GATED);                    // Enable clock
     }
}

#undef MHAL_MUX_C
