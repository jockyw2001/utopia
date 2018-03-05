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

#include "MsCommon.h"
#include "drvUSBHwCtl.h"
#include "drvEHCI.h"
#include "drvUSB.h"

void XBYTE_OR(MS_U32 Addr, MS_U8 offset, MS_U8 val)
{
    MS_U16 temp;

    if (offset & 1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=(((MS_U16)val)<<8) | (temp );
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=(temp )|val;
     }
}

void XBYTE_AND(MS_U32 Addr, MS_U8 offset,MS_U8 val)
{
    MS_U16 temp;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=((((MS_U16)val)<<8)|0xff) & (temp );
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=(temp & (0xff00|val) );

     }
}

void XBYTE_SET(MS_U32 Addr, MS_U8 offset,MS_U8 val)
{
    MS_U16 temp;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=((temp & 0x00ff) | (((MS_U16)val)<<8));
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=((temp & 0xff00) |val );
     }
}

MS_U8 XBYTE_READ(MS_U32 Addr, MS_U8 offset)
{
    MS_U16 temp;
    MS_U8  uRetVal = 0;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        uRetVal = (MS_U8) (temp >> 8);
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        uRetVal = (MS_U8) temp;
    }

    //diag_printf("XBYTE_READ: Addr: %X, offset: %X, uRetVal: %X\n", Addr, offset, uRetVal);
    return uRetVal;
}
// ------------------------------------------------------------------------
#include "drvEHCI.h"
void ResetMstarUsb(struct ehci_hcd *ehci)
{
    U32 reg_cmd, reg_inten;

    reg_inten = ehci_readl((U32)&ehci->regs->intr_enable);
    
    // 20110324: only reset UHC
    // TODO: without UTMI TX/RX reset after K1
    reg_cmd = ehci_readl((U32)&ehci->regs->command);
    ehci_writel(reg_cmd | CMD_RESET, (U32)&ehci->regs->command);
    //MsOS_DelayTask(2);    
    while(ehci_readb((U32)&ehci->regs->command) & CMD_RESET); // wait for reset done
    
    // restore UHC register
    ehci_writel (reg_inten, (U32)&ehci->regs->intr_enable);     
    ehci_writel (reg_cmd, (U32)&ehci->regs->command);
}
void UTMI_ORXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_OR(base, offset, val);
}

void UTMI_ANDXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_AND(base, offset, val);
}

void UTMI_SETXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_SET(base, offset, val);
}

MS_U8 UTMI_READXBYTE_EX(MS_U8 offset, MS_U32 base)
{
    return XBYTE_READ(base, offset);
}

#if USBC_IP_SUPPORT // USBC control
void usbc_irq(MS_U32 regUTMI, MS_U32 regUSBC, struct s_UsbcInfo *pUsbc)
{
    U16 status, vbus_t;
    
    pUsbc->intSts = status = usb_readw((void*)(regUSBC+0x6*2));    
    vbus_t = usb_readw((void*)(regUTMI+0x30*2)) & 0x20;  // bit[5]
    pUsbc->eventType = vbus_t ? 1 : 0; 
    diag_printf("<usbc_irq> status change(%x) vbus(%x)\n", status, vbus_t);
    status &= pUsbc->intEn;
    usb_writew(status, (void*)(regUSBC+0x6*2)); // write 1 clear status
    if (status)
        pUsbc->eventFlag = 1;
}

extern struct s_ChipUsbHostDef *pCurrentChip;
void _usbc_on_intr(InterruptNum eIntNum)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    MS_U8 p;

    if (pChip == NULL)
        return;
    MsOS_DisableInterrupt(eIntNum);
    for (p = 0; p < pChip->nRootHub; p++)
    {
        if (eIntNum == pChip->reg[p].usbcIRQ)
            break;
    }
    usbc_irq(pChip->reg[p].baseUTMI, pChip->reg[p].baseUSBC, &pChip->usbc_ip[p]);
    MsOS_EnableInterrupt(eIntNum);

}
void init_usbc_intr(MS_U8 p)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct s_UsbcInfo *pUsbc = &pChip->usbc_ip[p];
    MS_U8 intNum = pChip->reg[p].usbcIRQ;
    MS_U32 regUSBC = pChip->reg[p].baseUSBC;

    pUsbc->portNum = p;
    pUsbc->eventFlag = 0;
    pUsbc->intEn = USBCINTR_VBusValidChange;
    //pUsbc->intEn = USBCINTR_AValidChange; // for testing
    pUsbc->int_pol = 1;
    
    diag_printf("<init_usbc_intr> port: %d, enable %x\n", p, pUsbc->intEn);
    //usb_writeb((pUsbc->int_pol << 2) || usb_readb((void*)(regUSBC+0x2*2)), (void*)(regUSBC+0x2*2)); // set interrupt polarity
    usb_writew(pUsbc->intEn, (void*)(regUSBC+0x6*2)); // clear interrupt status
    usb_writew(pUsbc->intEn, (void*)(regUSBC+0x4*2)); // set interrupt enable
#ifndef  MS_NOSAPI
    MsOS_AttachInterrupt(intNum, _usbc_on_intr);
    MsOS_EnableInterrupt(intNum);
#endif
}
#endif
