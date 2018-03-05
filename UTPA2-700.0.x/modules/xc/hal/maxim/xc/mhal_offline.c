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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define  MHAL_OFFLINE_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "mhal_xc_chip_config.h"

// Registers
#include "drvXC_IOPort.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"
#include "hwreg_adc_atop.h"

#include "xc_Analog_Reg.h"
#include "mhal_mux.h"
#include "mhal_offline.h"
#include "mhal_mux.h"
#include "hwreg_sc.h"
#include "drvXC_IOPort.h"
#include "hwreg_dvi_atop.h"
#include "hwreg_hdmi.h"
#include "hwreg_pm_sleep.h"
#include "hwreg_ipmux.h"

#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "mhal_adc.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define    AIS_HalDebug(x)    //x

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


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_WaitForHVCleared_AV(void *pInstance, MS_U8 Channel)
{
    MS_U16 u16Hcnt, u16Vcnt;
    MS_U16 u16Count=0;

    u16Hcnt = Hal_XC_GetOffLineOfH(pInstance);
    u16Vcnt = Hal_XC_GetOffLineOfV(pInstance);
    while((u16Hcnt != 0x3fff)&&(u16Vcnt !=0x7ff)
            &&(u16Hcnt != 0)&&(u16Vcnt !=0))
    {
        AIS_HalDebug(printf("u16Count=%u, Channel =%u, MApi_XC_GetOffLineDetection: vcnt=%x hcnt=%x\n", u16Count,Channel, u16Vcnt,u16Hcnt );)
        u16Count++;
        MsOS_DelayTask(10);
        u16Hcnt = Hal_XC_GetOffLineOfH(pInstance);
        u16Vcnt = Hal_XC_GetOffLineOfV(pInstance);
        AIS_HalDebug(printf("u16Count=%u, Channel =%u, MApi_XC_GetOffLineDetection: vcnt=%x hcnt=%x\n", u16Count++,Channel, u16Vcnt,u16Hcnt );)
        if(u16Count>=0x100)
        {
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");)
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!Hal_XC_WaitForHVCleared(): reset fail!!!!!!!!!!!!!!!!!\n");)
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");)
            return;
        }
    }
}

void Hal_XC_WaitForHVCleared(void *pInstance, MS_U8 Channel)
{
    MS_U16 u16Hcnt, u16Vcnt;
    MS_U16 u16Count=0;

    u16Hcnt = Hal_XC_GetOffLineOfH(pInstance);
    u16Vcnt = Hal_XC_GetOffLineOfV(pInstance);
    while((u16Hcnt != 0x3fff)&&(u16Vcnt !=0x7ff)
           &&(u16Hcnt != 0)&&(u16Vcnt !=0))
    {
        AIS_HalDebug(printf("u16Count=%u, Channel =%u, MApi_XC_GetOffLineDetection: vcnt=%x hcnt=%x\n", u16Count,Channel, u16Vcnt,u16Hcnt );)
        u16Count++;
        //reset offline ip
        SC_W2BYTEMSK(0, REG_SC_BK00_02_L, BIT(5), 0xFF );
        MsOS_DelayTask(1);
        SC_W2BYTEMSK(0, REG_SC_BK00_02_L, 0x00, 0xFF );

        MsOS_DelayTask(10);
        u16Hcnt = Hal_XC_GetOffLineOfH(pInstance);
        u16Vcnt = Hal_XC_GetOffLineOfV(pInstance);
        AIS_HalDebug(printf("u16Count=%u, Channel =%u, MApi_XC_GetOffLineDetection: vcnt=%x hcnt=%x\n", u16Count++,Channel, u16Vcnt,u16Hcnt );)
        if(u16Count>=0x100)
        {
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");)
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!Hal_XC_WaitForHVCleared(): reset fail!!!!!!!!!!!!!!!!!\n");)
            AIS_HalDebug(printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");)
            return;
        }
    }
}

void Hal_XC_SetOffLineToSog_AV( void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(REG_ADC_ATOP_3C_L, 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(REG_ADC_ATOP_3C_L, BIT(13), BIT(13) );
    SC_W2BYTE(0, REG_SC_BK13_02_L, 0x0071);
    Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_MLINK);

    MDrv_Write2Byte(REG_ADC_ATOP_04_L, (MDrv_Read2Byte(REG_ADC_ATOP_04_L)&(0xFDE2)));
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, (MDrv_Read2Byte(REG_ADC_ATOP_05_L)&(0xF7CF)));
    MDrv_Write2Byte(REG_ADC_ATOP_06_L, 0);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5E_L, 0, 0x3fc);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5A_L, 0, 0xC0);//PD_LDO25_ADCA,PD_LDO25_ADCB
    MDrv_WriteByte(REG_ADC_ATOP_3D_L, (MDrv_ReadByte(REG_ADC_ATOP_3D_L) & 0xf0) | (Channel + 3));
    AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel + 3)));
}
//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToSog_YUV( void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(REG_ADC_ATOP_3C_L, 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(REG_ADC_ATOP_3C_L, BIT(13), BIT(13) );
    SC_W2BYTE(0, REG_SC_BK13_02_L, 0x0071);
    Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_MLINK);

    MDrv_Write2Byte(REG_ADC_ATOP_04_L, (MDrv_Read2Byte(REG_ADC_ATOP_04_L)&(0xFDE2)));
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, (MDrv_Read2Byte(REG_ADC_ATOP_05_L)&(0xF7CF)));
    MDrv_Write2Byte(REG_ADC_ATOP_06_L, 0);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5E_L, 0, 0x3fc);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5A_L, 0, 0xC0);//PD_LDO25_ADCA,PD_LDO25_ADCB
    if(Hal_ADC_ISOG_CheckEnabled(pInstance))
    {
        // Internal SOG & re-route SOG signal to channel 12~14 (Original is 0~2)
        MDrv_WriteByte(REG_ADC_ATOP_3D_L, (MDrv_ReadByte(REG_ADC_ATOP_3D_L) & 0xf0) | (Channel+12) );
        AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel + 12)));
    }
    else
    {
        MDrv_WriteByte(REG_ADC_ATOP_3D_L, (MDrv_ReadByte(REG_ADC_ATOP_3D_L) & 0xf0) | Channel);
        AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, Channel));
    }
}
//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToHv( void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(REG_ADC_ATOP_3C_L, 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(REG_ADC_ATOP_3C_L, BIT(13), BIT(13) );
    SC_W2BYTE(0, REG_SC_BK13_02_L,0x0001);
    Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_MLINK);
    MDrv_Write2Byte(REG_ADC_ATOP_04_L, (MDrv_Read2Byte(REG_ADC_ATOP_04_L)&(0xFDE2)));
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, (MDrv_Read2Byte(REG_ADC_ATOP_05_L)&(0xF7CF)));
    MDrv_Write2Byte(REG_ADC_ATOP_06_L, 0);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5E_L, 0, 0x3fc);
    MDrv_Write2ByteMask(REG_ADC_ATOP_5A_L, 0, 0x40);//PD_LDO25_ADCA
    MDrv_WriteByte(REG_ADC_ATOP_01_L, (MDrv_ReadByte(REG_ADC_ATOP_01_L) & 0xf3) | (Channel << 2));
    AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel << 2)));
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToHDMI( void *pInstance, MS_U8 Channel )
{
#if SUPPORT_IMMESWITCH
    MS_U8 u8Tmp = Channel - INPUT_PORT_DVI0;
    W2BYTEMSK(REG_PM_SLP_BASE+0x94,0,BIT(12 + u8Tmp));
    switch(u8Tmp)
    {
        case 0:
            W2BYTEMSK(REG_PM_SLP_BASE+0x96,0,(BIT(0)|BIT(4)|BIT(8)|BIT(12)));
            break;
        case 1:
            W2BYTEMSK(REG_PM_SLP_BASE+0x96,0,(BIT(1)|BIT(5)|BIT(9)|BIT(13)));
            break;
        case 2:
            W2BYTEMSK(REG_PM_SLP_BASE+0x96,0,(BIT(2)|BIT(6)|BIT(10)|BIT(14)));
            break;
        case 3:
            W2BYTEMSK(REG_PM_SLP_BASE+0x96,0,(BIT(3)|BIT(7)|BIT(11)|BIT(15)));
            break;
        default:
            break;
    }
#endif
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToUSB( void *pInstance, MS_U8 Channel )
{
    //Need to do nothing
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfH(void *pInstance)
{
    MS_U16 u16Hcnt;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    /*MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_OFFLINE);
    u16Hcnt = MDrv_Read2Byte(L_BK_OFFLINE(0x20)) & 0x3fff;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    u16Hcnt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK13_20_L) & 0x3fff;
    return u16Hcnt;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfV(void *pInstance)
{
    MS_U16 u16Vcnt;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    /*MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_OFFLINE);
    u16Vcnt = MDrv_Read2Byte(L_BK_OFFLINE(0x1f)) & 0x07ff;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    u16Vcnt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK13_1F_L) & 0x07ff;
    return u16Vcnt;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI0(void)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;

    if((R2BYTE(REG_PM_SLEEP_3F_L) &BMASK(15:14)) != BMASK(15:14))
    {
        u16Ret = 1;
    }
    
    return u16Ret;
    
#else
    return 0;
#endif
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI1(void)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;

    if((R2BYTE(REG_PM_SLEEP_42_L) &BMASK(15:14)) != BMASK(15:14))
    {
        u16Ret = 1;
    }
    
    return u16Ret;
    
#else
    return 0;
#endif
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI2(void)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;

    if((R2BYTE(REG_PM_SLEEP_45_L) &BMASK(15:14)) != BMASK(15:14))
    {
        u16Ret = 1;
    }
    
    return u16Ret;
    
#else
    return 0;
#endif
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI3(void)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;

    if((R2BYTE(REG_PM_SLEEP_48_L) &BMASK(15:14)) != BMASK(15:14))
    {
        u16Ret = 1;
    }
    
    return u16Ret;
    
#else
    return 0;
#endif
}

//------------------------------------------------------------------------------------------------
// Return HW connections status of UHCx
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfStorage(void *pInstance)
{
    return ((MDrv_ReadByte(REG_UHC0_BASE + 0x30) & 0x01) | (MDrv_ReadByte(REG_UHC1_BASE + 0x30) & 0x01));
}

//------------------------------------------------------------------------------------------------
// Function: Switch Clk path
//----
// Input Source Select.
//   0000: ADC A.
//   0001: DVI
//   0010: VD
//   0011: Capture
//   0100: Reserved
//   0101: Ext VD
//   0110: ADC B
//
//------------------------------------------------------------------------------------------------
void   Hal_XC_TurnOnDetectClkPath(void *pInstance, MS_U8 u8DetectClkPath)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) || 
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt < OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_01_H;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = (u8DetectClkPath << 8);
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0xF000;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    MDrv_WriteByte(REG_IPMUX_01_H, (MDrv_ReadByte(REG_IPMUX_01_H) & 0xf0) | u8DetectClkPath);
}

void Hal_XC_SetOfflineDetectClk(void *pInstance)
{
    MDrv_WriteByteMask(REG_CKG_IDCLK0, CKG_IDCLK0_XTAL, CKG_IDCLK0_MASK); // initial to XTAL first, will be changed when switch input source
}

MS_BOOL Hal_XC_SetOffLineSogThreshold(void *pInstance, MS_U8 u8Threshold)
{
    UNUSED(pInstance);
    MDrv_WriteByte(REG_ADC_ATOP_3E_L, u8Threshold); //8 bit
    return TRUE;
}

MS_BOOL Hal_XC_SetOffLineSogBW(void *pInstance, MS_U8 u8BW)
{
    MS_BOOL bRet = TRUE;

    UNUSED(pInstance);

    if(u8BW < 32) // only 5 bit and always >= 0
    {
        bRet = TRUE;
    }
    else
    {
        printf("[Offline Detect]invalid sog filter bandwidth value, use default value:0x0!\n");
        u8BW = 0;
        bRet = FALSE;
    }
    MDrv_WriteByte(REG_ADC_ATOP_3C_L, (MDrv_ReadByte(REG_ADC_ATOP_3C_L) & 0xe0) | u8BW);
    return bRet;
}

void Hal_XC_OffLineInit(void *pInstance)
{
    MDrv_WriteByte(REG_ADC_ATOP_3F_L, MDrv_ReadByte(REG_ADC_ATOP_3F_L) & 0xfd);
}

void Hal_XC_OffLineExit(void *pInstance)
{
    MDrv_WriteByteMask(REG_ADC_ATOP_3F_L, BIT(1), BIT(1));
}

MS_U8 Hal_XC_GetOffLineDetection(void *pInstance, MS_U8 u8CheckSrc)
{
    if(((u8CheckSrc >= (MS_U8)INPUT_PORT_YMUX_CVBS0) &&
        (u8CheckSrc <= (MS_U8)INPUT_PORT_YMUX_CVBS7))
         ||
        ((u8CheckSrc >= (MS_U8)INPUT_PORT_ANALOG0_SYNC)  &&
        (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4_SYNC))
         ||
        ((u8CheckSrc >= (MS_U8)INPUT_PORT_ANALOG0)  &&
        (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4)))
    {
        MS_U16 u16Hcnt, u16Vcnt;

        u16Hcnt = Hal_XC_GetOffLineOfH(pInstance);
        u16Vcnt = Hal_XC_GetOffLineOfV(pInstance);
        AIS_HalDebug(printf("MApi_XC_GetOffLineDetection: vcnt=%x hcnt=%x\n", u16Vcnt,u16Hcnt );)

        if((u16Hcnt == 0x3fff)
            ||(u16Vcnt ==0x7ff)
            ||(u16Hcnt == 0)
            ||(u16Vcnt ==0)
            )
        {
            return 0;        // Nosignal
        }
        else
        {
            return 1;        //Signal in
        }
    }
    else if(u8CheckSrc == INPUT_PORT_DVI0)
    {
        return Hal_XC_GetOffLineOfDVI0();
    }
    else if(u8CheckSrc == INPUT_PORT_DVI1)
    {
        return Hal_XC_GetOffLineOfDVI1();
    }
    else if(u8CheckSrc == INPUT_PORT_DVI2)
    {
        return Hal_XC_GetOffLineOfDVI2();
    }
    else if(u8CheckSrc == INPUT_PORT_DVI3)
    {
        return Hal_XC_GetOffLineOfDVI3();
    }
    else if(u8CheckSrc == INPUT_PORT_MVOP)
    {
        //UHC0_REG_BASE and UHC1_REG_BASE
        return (Hal_XC_GetOffLineOfStorage(pInstance))?1:0;
    }
    else
    {
        return 0;
    }
}

#undef MHAL_OFFLINE_C
