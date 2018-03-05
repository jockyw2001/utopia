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
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"

#include "xc_Analog_Reg.h"
#include "mhal_mux.h"
#include "mhal_offline.h"
#include "mhal_mux.h"
#include "hwreg_sc.h"
#include "drvXC_IOPort.h"
#include "hwreg_dvi_atop.h"
#include "hwreg_hdmi.h"

#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
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
void Hal_XC_WaitForHVCleared_AV(void *pInstance,MS_U8 Channel)
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

void Hal_XC_WaitForHVCleared(void *pInstance,MS_U8 Channel)
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

void Hal_XC_SetOffLineToSog_AV(void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), BIT(13), BIT(13) );
    SC_W2BYTE(0,REG_SC_BK13_02_L, 0x0071);
    Hal_XC_TurnOnDetectClkPath(pInstance,SC_SUBWIN_IPMUX_MLINK);

    MDrv_Write2Byte(L_BK_ADC_ATOP(0x04), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x05), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x06), 0);
    MDrv_Write2ByteMask(L_BK_ADC_ATOP(0x5e), 0, 0x3fc);
    MDrv_WriteByte(L_BK_ADC_ATOP(0x3d), (MDrv_ReadByte(L_BK_ADC_ATOP(0x3d)) & 0xf0) | (Channel + 3));
    AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel + 3)));
}
//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToSog_YUV(void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), BIT(13), BIT(13) );
    SC_W2BYTE(0,REG_SC_BK13_02_L, 0x0071);
    Hal_XC_TurnOnDetectClkPath(pInstance,SC_SUBWIN_IPMUX_MLINK);

    MDrv_Write2Byte(L_BK_ADC_ATOP(0x04), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x05), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x06), 0);
    MDrv_Write2ByteMask(L_BK_ADC_ATOP(0x5e), 0, 0x3fc);
    if(Hal_ADC_ISOG_CheckEnabled(pInstance))
    {
        // Internal SOG & re-route SOG signal to channel 12~14 (Original is 0~2)
        MDrv_WriteByte(L_BK_ADC_ATOP(0x3d), (MDrv_ReadByte(L_BK_ADC_ATOP(0x3d)) & 0xf0) | (Channel + 12) );
        AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel + 12)));
    }
    else
    {
        MDrv_WriteByte(L_BK_ADC_ATOP(0x3d), (MDrv_ReadByte(L_BK_ADC_ATOP(0x3d)) & 0xf0) | Channel);
        AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, Channel));
    }
}
//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToHv(void *pInstance, MS_U8 Channel )
{
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), 0, BIT(13) );
    OS_DELAY_TASK(1);
    W2BYTEMSK(L_BK_ADC_ATOP(0x3C), BIT(13), BIT(13) );
    SC_W2BYTE(0,REG_SC_BK13_02_L,0x0001);
    Hal_XC_TurnOnDetectClkPath(pInstance,SC_SUBWIN_IPMUX_MLINK);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x04), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x05), 0);
    MDrv_Write2Byte(L_BK_ADC_ATOP(0x06), 0);
    MDrv_Write2ByteMask(L_BK_ADC_ATOP(0x5e), 0, 0x3fc);
    MDrv_WriteByte(L_BK_ADC_ATOP(0x01), (MDrv_ReadByte(L_BK_ADC_ATOP(0x01)) & 0xf3) | (Channel << 2));
    AIS_HalDebug(printf("%s: SOG offline mux -> %d\n", __FUNCTION__, (Channel << 2)));
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void Hal_XC_SetOffLineToHDMI(void *pInstance, MS_U8 Channel )
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
void Hal_XC_SetOffLineToUSB(void *pInstance, MS_U8 Channel )
{
    //Need to do nothing
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfH(void *pInstance)
{
    MS_U16 u16Hcnt;
    /*MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_OFFLINE);
    u16Hcnt = MDrv_Read2Byte(L_BK_OFFLINE(0x20)) & 0x3fff;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    u16Hcnt = SC_R2BYTE(0,REG_SC_BK13_20_L) & 0x3fff;
    return u16Hcnt;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfV(void *pInstance)
{
    MS_U16 u16Vcnt;
    /*MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_OFFLINE);
    u16Vcnt = MDrv_Read2Byte(L_BK_OFFLINE(0x1f)) & 0x07ff;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    u16Vcnt = SC_R2BYTE(0,REG_SC_BK13_1F_L) & 0x07ff;
    return u16Vcnt;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI0(void *pInstance)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;
    MS_U16 u16DVIATOP_06 = R2BYTE(REG_DVI_ATOP_06_L);
    MS_U16 u16DVIATOP_60 = R2BYTE(REG_DVI_ATOP_60_L);
    
    // DVI Clock power control
    W2BYTE(REG_DVI_ATOP_06_L, 0); // enable DVI0 clock power
    // DVI PLL power control
    W2BYTE(REG_DVI_ATOP_60_L, 0); // enable DVI0 PLL power

    MsOS_DelayTask(20);

    if(((MDrv_ReadByte(REG_DVI_DTOP_16_H) & 0x03) == 0x02)
        &&((MDrv_ReadByte(REG_DVI_DTOP_17_H)&0x80)== 0))
    {
        u16Ret = 1;
    }
    else
    {
        u16Ret = 0;
    }

    // restore
    W2BYTE(REG_DVI_ATOP_06_L, u16DVIATOP_06); 
    W2BYTE(REG_DVI_ATOP_60_L, u16DVIATOP_60); 
     
    return u16Ret;
#else
    return 0;
#endif
}

MS_U16 Hal_XC_GetOffLineOfDVI1(void *pInstance)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;
    MS_U16 u16DVIATOP_06 = R2BYTE(REG_DVI_ATOP1_06_L);
    MS_U16 u16DVIATOP_60 = R2BYTE(REG_DVI_ATOP1_60_L);
    
    // DVI Clock power control
    W2BYTE(REG_DVI_ATOP1_06_L, 0); // enable DVI1 clock power
    // DVI PLL power control
    W2BYTE(REG_DVI_ATOP1_60_L, 0); // enable DVI1 PLL power

    MsOS_DelayTask(20);

    if(((MDrv_ReadByte(REG_DVI_DTOP1_16_H) & 0x03) == 0x02)
       &&((MDrv_ReadByte(REG_DVI_DTOP1_17_H )&0x80) == 0))
    {
        u16Ret = 1;
    }
    else
    {
        u16Ret = 0;
    }

    // restore
    W2BYTE(REG_DVI_ATOP1_06_L, u16DVIATOP_06); 
    W2BYTE(REG_DVI_ATOP1_60_L, u16DVIATOP_60); 
     
    return u16Ret;
#else
    return 0;
#endif
}

MS_U16 Hal_XC_GetOffLineOfDVI2(void *pInstance)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;
    MS_U16 u16DVIATOP_06 = R2BYTE(REG_DVI_ATOP2_06_L);
    MS_U16 u16DVIATOP_60 = R2BYTE(REG_DVI_ATOP2_60_L);
    
    // DVI Clock power control
    W2BYTE(REG_DVI_ATOP2_06_L, 0); // enable DVI2 clock power
    // DVI PLL power control
    W2BYTE(REG_DVI_ATOP2_60_L, 0); // enable DVI2 PLL power

    MsOS_DelayTask(20);

    if(((MDrv_ReadByte(REG_DVI_DTOP2_16_H) & 0x03) == 0x02)
        &&((MDrv_ReadByte(REG_DVI_DTOP2_17_H )&0x80) == 0))
    {
        u16Ret = 1;
    }
    else
    {
        u16Ret = 0;
    }

    // restore
    W2BYTE(REG_DVI_ATOP2_06_L, u16DVIATOP_06); 
    W2BYTE(REG_DVI_ATOP2_60_L, u16DVIATOP_60); 
     
    return u16Ret;
#else
    return 0;
#endif
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
MS_U16 Hal_XC_GetOffLineOfDVI3(void *pInstance)
{
#if SUPPORT_IMMESWITCH
    MS_U16 u16Ret = 0;
    MS_U16 u16DVIATOP_06 = R2BYTE(REG_DVI_ATOP3_06_L);
    MS_U16 u16DVIATOP_60 = R2BYTE(REG_DVI_ATOP3_60_L);
    
    // DVI Clock power control
    W2BYTE(REG_DVI_ATOP3_06_L, 0); // enable DVI3 clock power
    // DVI PLL power control
    W2BYTE(REG_DVI_ATOP3_60_L, 0); // enable DVI3 PLL power

    MsOS_DelayTask(20);

    if(((MDrv_ReadByte(REG_DVI_DTOP3_16_H) & 0x03) == 0x02)
        &&((MDrv_ReadByte(REG_DVI_DTOP3_17_H )&0x80) == 0))
    {
        u16Ret = 1;
    }
    else
    {
        u16Ret = 0;
    }

    // restore
    W2BYTE(REG_DVI_ATOP3_06_L, u16DVIATOP_06); 
    W2BYTE(REG_DVI_ATOP3_60_L, u16DVIATOP_60); 
     
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
void   Hal_XC_TurnOnDetectClkPath(void *pInstance,MS_U8 u8DetectClkPath)
{
    MDrv_WriteByte(H_BK_IPMUX(0x01), (MDrv_ReadByte(H_BK_IPMUX(0x01)) & 0xf0) | u8DetectClkPath);
}

void Hal_XC_SetOfflineDetectClk(void *pInstance)
{
    MDrv_WriteByteMask(REG_CKG_IDCLK0, CKG_IDCLK0_XTAL, CKG_IDCLK0_MASK); // initial to XTAL first, will be changed when switch input source
}

MS_BOOL Hal_XC_SetOffLineSogThreshold(void *pInstance,MS_U8 u8Threshold)
{
    MDrv_WriteByte(L_BK_ADC_ATOP(0x3e), u8Threshold); //8 bit
    return TRUE;
}

MS_BOOL Hal_XC_SetOffLineSogBW(void *pInstance,MS_U8 u8BW)
{
    MS_BOOL bRet = TRUE;
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
    MDrv_WriteByte(L_BK_ADC_ATOP(0x3c), (MDrv_ReadByte(L_BK_ADC_ATOP(0x3c)) & 0xe0) | u8BW);
    return bRet;
}

void Hal_XC_OffLineInit(void *pInstance)
{
    MDrv_WriteByte(L_BK_ADC_ATOP(0x3f), MDrv_ReadByte(L_BK_ADC_ATOP(0x3f)) & 0xfd);
}

void Hal_XC_OffLineExit(void *pInstance)
{
    MDrv_WriteByteMask(L_BK_ADC_ATOP(0x3f), BIT(1), BIT(1));
}

MS_U8 Hal_XC_GetOffLineDetection(void *pInstance,MS_U8 u8CheckSrc)
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
        return Hal_XC_GetOffLineOfDVI0(pInstance);
    }
    else if(u8CheckSrc == INPUT_PORT_DVI1)
    {
        return Hal_XC_GetOffLineOfDVI1(pInstance);
    }
    else if(u8CheckSrc == INPUT_PORT_DVI2)
    {
        return Hal_XC_GetOffLineOfDVI3(pInstance); //the mapping relation is  in Hal_DVI_SwitchSrc()
    }
    else if(u8CheckSrc == INPUT_PORT_DVI3)
    {
        return Hal_XC_GetOffLineOfDVI2(pInstance);
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
