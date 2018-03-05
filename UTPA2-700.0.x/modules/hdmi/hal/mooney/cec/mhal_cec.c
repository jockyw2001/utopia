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
// File Name: mhal_CEC.c
// Description: For CEC functions.
////////////////////////////////////////////////////////////////////////////////


#define _MHAL_CEC_C_

#include <string.h>
#include "MsCommon.h"
#include "cec_hwreg_utility2.h"
#include "cec_Analog_Reg.h"
#include "MsOS.h"
#include "apiCEC.h"
#include "MsIRQ.h"
#include "mhal_CEC.h"
#include "asmCPU.h"

extern MS_U32 CEC_RIU_BASE;
extern InterruptNum gCECIRQ;
extern MS_U8 g_u8CecVendorID[3];
extern MS_U8 g_u8RetryCnt;

#define CEC_DPUTSTR(str)        //printf(str)
#define CEC_DPRINTF(str, x)     //printf(str, x)

#define PM_REG_WRITE    MDrv_WriteByte
#define PM_REG_READ     MDrv_ReadByte


#define MST_XTAL_CLOCK_HZ   (12000000UL)    /* Temp define */

#define _NOP_                       MAsm_CPU_Nop();

void mhal_CEC_PortSelect(MsCEC_INPUT_PORT InputPort)
{

}

void mhal_CEC_init_riu_base(MS_U32 u32riu_base, MS_U32 u32PMriu_base)
{
    CEC_RIU_BASE = u32PMriu_base;
}


MS_U8 mhal_CEC_HeaderSwap(MS_U8 value)
{
    return(((value&0x0f)<<4)+((value&0xf0)>>4));
}

MS_U8 mhal_CEC_SendFrame(MS_U8 header, MS_U8 opcode, MS_U8* operand, MS_U8 len)
{
    MS_U8 i, cnt, *ptr, res;
    MS_U8 u8waitcnt;

     // clear CEC TX INT status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x0E);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);
    PM_REG_WRITE( L_BK_CEC(0x18), header );
    PM_REG_WRITE( H_BK_CEC(0x18), opcode );

    CEC_DPUTSTR("\r\n/********  CEC Tx **********/\r\n");
    CEC_DPRINTF("CEC Tx FIFO= 0x%x", (MS_U8)header);
    CEC_DPRINTF(" 0x%x", (MS_U8)opcode);

    if(len > 0)
    {
        ptr=operand;
        for(i=0;i<len;i++)
        {
            PM_REG_WRITE( L_BK_CEC(0x19)+i , *(ptr+i) );
            CEC_DPRINTF(" 0x%x", *(operand+i));
        }
        CEC_DPUTSTR("\r\n/**************************/\r\n");
    }


    // CEC transmit length
    //if((opcode==0x00)&&(operand==NULL)&&(len==0))
    if((opcode==0x00)&&(len==0))
    {
        PM_REG_WRITE(L_BK_CEC(0x00), 0);                   //polling message
        u8waitcnt = 5;
    }
    else
    {
        PM_REG_WRITE(L_BK_CEC(0x00), (len+1));
        u8waitcnt = 4 * (len+2);
    }

//The total time,
//(1). successful, 4.5 ms + 10 * 2.4 ms * N = 4.5 ms + 24 * N
//              = 28.5 ms (1), or 52.5 ms (2), ....
//(2). NAK,        (4.5 ms + 10 * 2.4 ms) * 1 + (4.5 ms + 10 * 2.4 ms +7.2 ms(3 bit time)) * retry (3)
//              = 28.5 + 35.2 * 3 = 133.6 ms

    cnt=0;
    MsOS_DelayTask(20);

    do
    {
        MsOS_DelayTask(10);
        if(cnt++>=u8waitcnt)
            break;
    } while((PM_REG_READ(H_BK_CEC(0x11))&0x0E)==0);
    res = (PM_REG_READ(H_BK_CEC(0x11))&0x0E);

    if(cnt>=u8waitcnt)
        res |= E_CEC_SYSTEM_BUSY;

     // clear CEC TX INT status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x0E);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);

    return res;
}

MS_U8 mhal_CEC_SendFramex(MS_U8 header, MS_U8 opcode, MS_U8* operand, MS_U8 len)
{
    MS_U8 i, cnt, *ptr, res;
    MS_U8 u8waitcnt;
    volatile MS_U16 k, m;
     // clear CEC TX INT status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x0E);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);
    PM_REG_WRITE( L_BK_CEC(0x18), header );
    PM_REG_WRITE( H_BK_CEC(0x18), opcode );

    CEC_DPUTSTR("\r\n/********  CEC Tx **********/\r\n");
    CEC_DPRINTF("CEC Tx FIFO= 0x%x", (MS_U8)header);
    CEC_DPRINTF(" 0x%x", (MS_U8)opcode);

    if(len > 0)
    {
        ptr=operand;
        for(i=0;i<len;i++)
        {
            PM_REG_WRITE( L_BK_CEC(0x19)+i , *(ptr+i) );
            CEC_DPRINTF(" 0x%x", *(operand+i));
        }
        CEC_DPUTSTR("\r\n/**************************/\r\n");
    }


    // CEC transmit length
    //if((opcode==0x00)&&(operand==NULL)&&(len==0))
    if((opcode==0x00)&&(len==0))
    {
        PM_REG_WRITE(L_BK_CEC(0x00), 0);                   //polling message
        u8waitcnt = 5;
    }
    else
    {
        PM_REG_WRITE(L_BK_CEC(0x00), (len+1));
        u8waitcnt = 30;
    }

//The total time,
//(1). successful, 4.5 ms + 10 * 2.4 ms * N = 4.5 ms + 24 * N
//              = 28.5 ms (1), or 52.5 ms (2), ....
//(2). NAK,        (4.5 ms + 10 * 2.4 ms) * 1 + (4.5 ms + 10 * 2.4 ms +7.2 ms(3 bit time)) * retry (3)
//              = 28.5 + 35.2 * 3 = 133.6 ms


    cnt=0;
    //MsOS_DelayTask(20);
    for(k = 0; k< 20000; k++)
    {
        _NOP_
		_NOP_
		_NOP_
	}

	
    do
    {
        //MsOS_DelayTask(10);
        for(k = 0; k< 20000; k++)
        {
            for(m=0;m<50;m++)
            {
                _NOP_
                _NOP_
                _NOP_
                _NOP_
                _NOP_
            }
        }
        if(cnt++>=u8waitcnt)
            break;
    } while((PM_REG_READ(H_BK_CEC(0x11))&0x0E)==0);

    res = (PM_REG_READ(H_BK_CEC(0x11))&0x0E);

    if(cnt>=u8waitcnt)
        res |= E_CEC_SYSTEM_BUSY;

     // clear CEC TX INT status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x0E);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);

    return res;
}


void mhal_CEC_SetMyAddress(MS_U8 mylogicaladdress)
{
    PM_REG_WRITE(L_BK_CEC(0x02), (PM_REG_READ(L_BK_CEC(0x02)) & 0x0F) |(mylogicaladdress<<4));
}

void mhal_CEC_INTEn(MS_BOOL bflag)
{
    if(bflag) // unmask
        PM_REG_WRITE(L_BK_CEC(0x13), 0x1E);  //REG_HDMI_INT_MASK
        
    else // Mask CEC interrupt
#if ENABLE_CEC_MULTIPLE
        PM_REG_WRITE(L_BK_CEC(0x13), 0x7F);  //REG_HDMI_INT_MASK
#else
        PM_REG_WRITE(L_BK_CEC(0x13), 0x1F);  //REG_HDMI_INT_MASK
#endif
        
}


void mhal_CEC_Init(MS_U32 u32XTAL_CLK_Hz)
{
    MS_U16 reg_val0, reg_val1;

#if ENABLE_CEC_INT

    // CEC irq clear
    PM_REG_WRITE(H_BK_CEC(0x12), 0x1F);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);
    // CEC irq mask control -only enable CEC rx irq
    PM_REG_WRITE(L_BK_CEC(0x13), 0x1E);
    // CEC interrupt mask for PM/normal function
    PM_REG_WRITE(L_BK_CEC(0x30), 0x08); // 11_30[3] = 1 Interrupt clear type select (Level), clear by itself
    
    gCECIRQ = E_INT_IRQ_CEC; // IRQ52
#endif

    PM_REG_WRITE(L_BK_CEC(0x14),0x01); // [1]: clock source from Xtal;[0]: Power down CEC controller select
    PM_REG_WRITE(H_BK_CEC(0x03),PM_REG_READ(H_BK_CEC(0x03))&(~ BIT(4))); // [4]: Standby mode;
    PM_REG_WRITE(H_BK_CEC(0x00),0x10|RETRY_CNT); // retry times
    PM_REG_WRITE(L_BK_CEC(0x01),0x80); // [5]:CEC clock no gate; [7]: Enable CEC controller
    PM_REG_WRITE(H_BK_CEC(0x01),(BusFreeTime<<4)|(ReTxInterval)); // CNT1=ReTxInterval; CNT2=BusFreeTime;
    PM_REG_WRITE(L_BK_CEC(0x02),(E_LA_TV<<4)|(FrameInterval)); // CNT3=FrameInterval; [7:4]=logical address: TV
#if ENABLE_CEC_MULTIPLE
    PM_REG_WRITE(L_BK_CEC(0x30),PM_REG_READ(L_BK_CEC(0x30))|BIT(0)); // enable CEC multiple function
#endif
    reg_val0=(MST_XTAL_CLOCK_HZ%100000l)*0.00016+0.5;
    PM_REG_WRITE(H_BK_CEC(0x02),(MST_XTAL_CLOCK_HZ/100000l)); // CEC time unit by Xtal(integer)
    reg_val1 = PM_REG_READ(L_BK_CEC(0x03));
    PM_REG_WRITE(L_BK_CEC(0x03), ((reg_val1 & 0xF0) | reg_val0)); // CEC time unit by Xtal(fractional)

    PM_REG_WRITE(L_BK_CEC(0x11), 0xFF); // clear CEC status

    
}

MS_BOOL mhal_CEC_IsMessageReceived(void)
{
    return (PM_REG_READ(H_BK_CEC(0x11))& 0x01 ? TRUE : FALSE);
}

MS_U8 mhal_CEC_ReceivedMessageLen(void)
{
    return ((PM_REG_READ(L_BK_CEC(0x04)) & 0x1F) + 1);
}

MS_U8 mhal_CEC_GetMessageByte(MS_U8 idx)
{
    return (PM_REG_READ(L_BK_CEC(0x20) + idx));
}

void mhal_CEC_ClearRxStatus(void)
{
    // clear RX INT status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x11);
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);
    // clear RX NACK status
    PM_REG_WRITE(L_BK_CEC(0x11), 0xFF);
}

/***************************************************************************************/
/// config cec wake up
/***************************************************************************************/

void mhal_CEC_ConfigWakeUp(void)
{
    MS_U16 reg_val0, reg_val1;
    CEC_DPUTSTR("\r\n Here do the PM config cec wakeup \r\n");
    
    //XBYTE[0x250C] &= ~BIT0;
    MDrv_WriteByte( REG_COMBO_PHY0_P0_0C_H, MDrv_ReadByte(REG_COMBO_PHY0_P0_0C_H) &(~ BIT(4)));
    

    //(1) enable chiptop clk_mcu & clk_pram
  #if 0
    XBYTE[0x0E00] = 0x03;
    //enable PM_Sleep's clk_mcu and _pram
    Drv_WriteByte(L_BK_PMSLP(0x00), 0x03);
    XBYTE[0x0E01] = 0x0F;
    //0x0C; Ken 20080916 for calibration to 1Mz
    MDrv_WriteByte(H_BK_PMSLP(0x00), 0x0F);
  #endif


    //(2) HDMI CEC settings
    PM_REG_WRITE(H_BK_CEC(0x00),0x10|g_u8RetryCnt); // retry times
    PM_REG_WRITE(L_BK_CEC(0x01),0x80); // [5]:CEC clock no gate; [7]: Enable CEC controller
    PM_REG_WRITE(H_BK_CEC(0x01),0x63); // CNT1=3; CNT2 = 6;
    PM_REG_WRITE(L_BK_CEC(0x02),0x07); // CNT3=7; logical address: TV

    reg_val0=(MST_XTAL_CLOCK_HZ%100000l)*0.00016+0.5;
    PM_REG_WRITE(H_BK_CEC(0x02),(MST_XTAL_CLOCK_HZ/100000l)); // CEC time unit by Xtal(integer)

    reg_val1 = PM_REG_READ(L_BK_CEC(0x03));
    PM_REG_WRITE(L_BK_CEC(0x03), ((reg_val1 & 0xF0) | reg_val0)); // CEC time unit by Xtal(fractional)


    //(3) PM Sleep: wakeup enable sources
    //PM_REG_WRITE(L_BK_PMMCU(0x00),0x01); // reg_cec_enw


    //(4) PM CEC power down controller settings
    // Mask CEC interrupt in standby mode
    PM_REG_WRITE(L_BK_CEC(0x13),0xFF);
    // select power down SW CEC controller
    PM_REG_WRITE(L_BK_CEC(0x14),0x01); // [1]: clock source from Xtal;[0]: Power down CEC controller select
    PM_REG_WRITE(L_BK_CEC(0x01),0x00); // [5]:CEC clock no gate; [7]: Disable CEC controller
    PM_REG_WRITE(L_BK_CEC(0x01),0x80); // [5]:CEC clock no gate; [7]: Enable CEC controller
#if 0//ENABLE_SW_CEC_WAKEUP
    PM_REG_WRITE(H_BK_CEC(0x03),PM_REG_READ(H_BK_CEC(0x03))&(~ BIT(4))); // [4]: Standby mode;
#else
    PM_REG_WRITE(H_BK_CEC(0x03),PM_REG_READ(H_BK_CEC(0x03))|(BIT(4))); // [4]: sleep mode;
#endif


    //(5) PM CEC wakeup opcode settings
    // OPCODE0: 0x04(Image view on)
    // OPCODE1: 0x0D(Text view on)
    // OPCODE2: 0x44 0x40(Power)
    //          0x44 0x6D(Power ON Function)
    // OPCODE3: N/A
    // OPCODE4: 0x82(Active source) length = 2
    PM_REG_WRITE(L_BK_CEC(0x07), 0x37); // Enable OP0~2 and OP4
    PM_REG_WRITE(H_BK_CEC(0x07), 0x24); // Eanble OPCODE2's operand

    PM_REG_WRITE(L_BK_CEC(0x08), E_MSG_OTP_IMAGE_VIEW_ON);  // OPCODE0: Image View On
    PM_REG_WRITE(H_BK_CEC(0x08), E_MSG_OTP_TEXT_VIEW_ON);   // OPCODE1: Text View ON

    PM_REG_WRITE(L_BK_CEC(0x09), E_MSG_UI_PRESS);           // OPCODE2: E_MSG_UI_PRESS
    PM_REG_WRITE(H_BK_CEC(0x0B), E_MSG_UI_POWER);           // OPCODE2 operand: Power
    PM_REG_WRITE(L_BK_CEC(0x0C), E_MSG_UI_POWER_ON_FUN);    // OPCODE2 operand: Power ON

    PM_REG_WRITE(L_BK_CEC(0x0A), E_MSG_ACTIVE_SOURCE);      // OPCODE4: Active source

    // [2:0]: CEC version 1.4; [7]: OP4 is broadcast message
    PM_REG_WRITE(H_BK_CEC(0x0D), 0x80 | HDMI_CEC_VERSION);


    //(6) Device(TV) Vendor ID for customer (Big Endian)
    // It depends end-customer's vendor ID
    MS_DEBUG_MSG(printf("!!!!!!!!!!!!!!!!!!!Change this Vendor ID according to customer!!!!!!!!!!!!!!!!\n"));
    PM_REG_WRITE(L_BK_CEC(0x0F), g_u8CecVendorID[0]); // Device Vendor ID
    PM_REG_WRITE(H_BK_CEC(0x0F), g_u8CecVendorID[1]); // Device Vendor ID
    PM_REG_WRITE(L_BK_CEC(0x10), g_u8CecVendorID[2]); // Device Vendor ID

    // [2:0]: Feature abort reason - "Not in correct mode to respond"
    PM_REG_WRITE(H_BK_CEC(0x10), E_MSG_AR_CANNOTRESPOND );


    //(7) Device Physical address: default is 0x00 0x00 0x00
    PM_REG_WRITE(L_BK_CEC(0x0E), 0x00); // Physical address 0.0
    PM_REG_WRITE(H_BK_CEC(0x0E), 0x00); // Physical address 0.0
    PM_REG_WRITE(H_BK_CEC(0x14), 0x00); // Device type: TV


    //(8) Clear CEC status
    PM_REG_WRITE(L_BK_CEC(0x11), 0x7F); // Clear CEC wakeup status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x1F); // Clear RX/TX/RF/LA/NACK status status
    PM_REG_WRITE(H_BK_CEC(0x12), 0x00);
}

void mhal_CEC_Enabled(MS_BOOL bEnableFlag)
{
    if(bEnableFlag)
        PM_REG_WRITE(L_BK_CEC(0x01),0x80); //Enable PM CEC controller
    else
        PM_REG_WRITE(L_BK_CEC(0x01),0x00); //Disable PM CEC controller
}

MS_U8 mhal_CEC_TxStatus(void)
{
    return (PM_REG_READ(H_BK_CEC(0x11))&0x0E);
}

MS_BOOL mhal_CEC_Device_Is_Tx(void)
{
    return CEC_DEVICE_IS_SOURCE;
}
