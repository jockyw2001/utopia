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

//#include <MsCommon.h> // NUSED
#include "drvUSBHwCtl.h"
#include "drvEHCI.h"
//#include "drvUSB.h" // NUSED
/* applying drvUSB.h (inside drvUSBHwCtl.h) */

#ifdef _USB_ENABLE_BDMA_PATCH
#include <drvBDMA.h>
#endif

void ms_XBYTE_OR(MS_U32 Addr, MS_U8 offset, MS_U8 val)
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

void ms_XBYTE_AND(MS_U32 Addr, MS_U8 offset,MS_U8 val)
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

void ms_XBYTE_SET(MS_U32 Addr, MS_U8 offset,MS_U8 val)
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

MS_U8 ms_XBYTE_READ(MS_U32 Addr, MS_U8 offset)
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
void ms_RH_force_FSmode(struct usb_hcd *hcd, int enable)
{
	U32 temp;
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	temp = hcd_reg_readl((U32)&ehci->op_regs->bus_control);
	if (enable) {
		hcd->isFSmode = 1;
		temp |= 0x80; //force enter FSmode
		diag_printf("[USB] force enter FSmode!\n");
	}
	else {
		hcd->isFSmode = 0;
		temp &= ~0x80;
		diag_printf("[USB] leave FSmode!\n");
	}
	hcd_reg_writel(temp, (U32)&ehci->op_regs->bus_control);
}

extern void ms_ehci_softrst(struct ehci_hcd *);
void ms_ResetMstarUsb(struct usb_hcd *hcd)
{
	/* disable force enter FSmode, 20130220 place here */
	if (hcd->isFSmode)
		ms_RH_force_FSmode(hcd, 0);
	ms_ehci_softrst(hcd_to_ehci(hcd));
}

inline BOOL ms_RoothubPortConnected(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->portsc[0]);
    return (regv & PORTSC_CONNECT) ? TRUE : FALSE;
}

inline BOOL ms_RoothubPortConnectChg(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->portsc[0]);
    return (regv & PORTSC_CSC) ? TRUE : FALSE;
}

inline BOOL ms_RoothubPortEnabled(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->portsc[0]);
    return (regv & PORTSC_PE) ? TRUE : FALSE;
}

inline BOOL ms_isHcdRunning(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->usbcmd);
    return (regv & USBCMD_RUN) ? TRUE : FALSE;
}

inline void ms_forceHcdRun(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->usbsts);
    if (regv & USBSTS_HALT)
    {
        //diag_printf("[UM-disconnect] force RUN!!!\n");
        hcd_reg_writel(USBCMD_RUN | hcd_reg_readl((U32)&ehci->op_regs->usbcmd),
            (U32)&ehci->op_regs->usbcmd);
    }
}

#ifdef DEBUG
inline void ms_dumpHcdRegister(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);

    diag_printf("[UM] usbcmd = %x\n", hcd_reg_readl((U32)&ehci->op_regs->usbcmd));
    diag_printf("[UM] status = %x\n", hcd_reg_readl((U32)&ehci->op_regs->usbsts));
    diag_printf("[UM] portsc = %x\n", hcd_reg_readl((U32)&ehci->op_regs->portsc[0]));
}
#endif

// ------------------------------------------------------------------------

void ms_UTMI_ORXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_OR(base, offset, val);
}

void ms_UTMI_ANDXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_AND(base, offset, val);
}

void ms_UTMI_SETXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_SET(base, offset, val);
}

MS_U8 ms_UTMI_READXBYTE_EX(MS_U8 offset, MS_U32 base)
{
    return ms_XBYTE_READ(base, offset);
}

void ms_ehci_interrupt_enable (struct usb_hcd *hcd, int str_on)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (hcd);
    U32 u32Reg_t;

    u32Reg_t = (HOST20_USBINTR_IntOnAsyncAdvance |
            HOST20_USBINTR_SystemError |
            HOST20_USBINTR_PortChangeDetect |
            HOST20_USBINTR_USBError |
            HOST20_USBINTR_CompletionOfTransaction);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbintr);

    if (!str_on)
        return;

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->usbsts);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbsts); //clear all pending interrupt

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->bus_control);
    u32Reg_t|= INT_POLAR;
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->bus_control); // set interrupt polarity high
}

void ms_ehci_interrupt_disable (struct usb_hcd *hcd)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (hcd);
    U32 u32Reg_t;
    hcd_reg_writel (0, (U32)&pEhci->op_regs->usbintr);

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->usbsts);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbsts); //clear all pending interrupt
}

/*
  * @brief          ehci controller reset
  *
  * @param          struct usb_hcd *pHcd
  *
  * @return         none
  */
extern void ms_qh_ehci_reinit(struct usb_hcd *, int);
void ms_roothub_disconn_reinit(struct usb_hcd *pHcd)
{
	struct ehci_hcd *pEhci = hcd_to_ehci (pHcd);
	U32 t_portsc, t_uhcsts;

	//diag_printf("root hub reinitial [usbdis]\n");
	t_portsc = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [0]);
	t_uhcsts = hcd_reg_readl(((U32)&pEhci->op_regs->usbcmd)+0x76);
	//ms_qh_ehci_reinit(pHcd, 0); // deleted 20150505
	ms_qh_ehci_reinit(pHcd, 1); // check again
	/* check if HC falls into abnormal state, if yes, do HC reset */
	if ((t_portsc == 0x8) && ((t_uhcsts & 0xc00) == 0xc00)) {
		/* to simulate MStar linux patch
		* ms_ehci_end(pHcd);
		* ms_ehci_init(pHcd);
		* ms_ehci_begin(pHcd);
		*/
		ms_ResetMstarUsb(pHcd); // reset ehci
		diag_printf("IN-NAK reset UHC\n");
	}
}

#ifdef _USB_ENABLE_BDMA_PATCH
void m_BDMA_write(unsigned int s, unsigned int t)
#if 1 // BDMA API
{
	BDMA_Result ret;

	ret = MDrv_BDMA_MemCopy(s, t, 4);
	if (ret != E_BDMA_OK) {
		diag_printf("[BDMA] write retuen fail (%d)!\n", ret);
	}
}
#else
{
//#define BDMA_RIU_BASE (MSTAR_PM_BASE+0x100900*2) // CH0
#define BDMA_RIU_BASE (_MSTAR_PM_BASE+0x100920*2) // CH1
#define MIU1_PHY_BASE_ADDR (0x40000000)
	int s_miu1, t_miu1, t_off2;

        //diag_printf("[USB] s %x, t %x\n", s, t);
	while (readw((void*)(BDMA_RIU_BASE+0x2*2)) & 0x1) {
		/* do nothing when last BDMA event not done */
	}
	/* decide which miu and calculate physical address */
	s_miu1 = (s >= MIU1_PHY_BASE_ADDR) ? 1 : 0;
	t_miu1 = (t >= MIU1_PHY_BASE_ADDR) ? 1 : 0;
	t_off2 = 0x4040 | s_miu1 | (t_miu1 << 8);
	s = s_miu1 ? (s - MIU1_PHY_BASE_ADDR) : s;
	t = t_miu1 ? (t - MIU1_PHY_BASE_ADDR) : t;
        writew(t_off2, (void*)(BDMA_RIU_BASE+0x4*2));
	/* source address */
        writew(s & 0xffff, (void*)(BDMA_RIU_BASE+0x8*2));
        writew((s>>16) & 0xffff, (void*)(BDMA_RIU_BASE+0xA*2));
	/* destination address */
        writew(t & 0xffff, (void*)(BDMA_RIU_BASE+0xC*2));
        writew((t>>16) & 0xffff, (void*)(BDMA_RIU_BASE+0xE*2));
	/* transfer size by byte */
        writew(0x4, (void*)(BDMA_RIU_BASE+0x10*2));

	/* clear status */
        writeb(readb((void*)(BDMA_RIU_BASE+0x2*2)) | 0x1c, (void*)(BDMA_RIU_BASE+0x2*2));

	/* fire once */
        writew(readw((void*)(BDMA_RIU_BASE)) | 0x1, (void*)(BDMA_RIU_BASE));
	while (1) {
		if ((readw((void*)(BDMA_RIU_BASE+0x2*2)) & 0x8) == 0x8)
			break;
	}
}
#endif

static int en_64bit_OBF_cipher;
void set_64bit_OBF_cipher(void)
{
	int retv = 0;
	unsigned int tmp_t, tmp1_t;

	tmp_t = readl((void*)(MIU0_RIU_BASE+MIU_DRAMOBF_READY_OFFSET));
	tmp1_t = readl((void*)(MIU0_RIU_BASE+MIU_64BIT_CIPHER_OFFSET));
	//diag_printf("[MIU0] offset(2A) = %x\n", tmp_t);
	//diag_printf("[MIU0] offset(D8) = %x\n", tmp1_t);
	if ((tmp_t & MIU_DRAMOBF_READY_BIT) != 0 &&
		(tmp1_t & MIU_64BIT_CIPHER_BIT) != 0)
		retv = 1;
	else
		retv = 0;

	#if defined(EHCI_CHECK_MIU1) && (EHCI_CHECK_MIU1 == 1)
	tmp_t = readl((void*)(MIU1_RIU_BASE+MIU_DRAMOBF_READY_OFFSET));
	tmp1_t = readl((void*)(MIU1_RIU_BASE+MIU_64BIT_CIPHER_OFFSET));
	//diag_printf("[MIU1] offset(2A) = %x\n", tmp_t);
	//diag_printf("[MIU1] offset(D8) = %x\n", tmp1_t);
	if ((tmp_t & MIU_DRAMOBF_READY_BIT) != 0 &&
		(tmp1_t & MIU_64BIT_CIPHER_BIT) != 0)
		retv = retv ? 1 : 0;
	else
		retv = 0;
	#endif

	en_64bit_OBF_cipher = retv;
	diag_printf("[MIU] 64-bit OBF cipher enabled!\n");

	#if defined(EHCI_CHECK_ECO_VER) && (EHCI_CHECK_ECO_VER == 1)
	tmp_t = readl((void*)(CHIP_VER_TOP+CHIP_VER_OFFSET));
	tmp_t = (tmp_t >> CHIP_VER_SHIFT) & CHIP_VER_MASK;
	USB_ASSERT(tmp_t >= CHIP_BDMA_ECO_VER,
		"[BDMA] Chip ECO version %d NOT correct!\n");
	#endif
}

int get_64bit_OBF_cipher(void)
{
	return en_64bit_OBF_cipher;
}
#endif

#if USBC_IP_SUPPORT // USBC control
void ms_usbc_irq(MS_U32 regUTMI, MS_U32 regUSBC, struct s_UsbcInfo *pUsbc)
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
void ms_usbc_on_intr(InterruptNum eIntNum)
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
    ms_usbc_irq(pChip->reg[p].baseUTMI, pChip->reg[p].baseUSBC, &pChip->usbc_ip[p]);
    MsOS_EnableInterrupt(eIntNum);

}
void ms_init_usbc_intr(MS_U8 p)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct s_UsbcInfo *pUsbc = &pChip->usbc_ip[p];
    InterruptNum intNum = pChip->reg[p].usbcIRQ;
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
    MsOS_AttachInterrupt(intNum, ms_usbc_on_intr);
    MsOS_EnableInterrupt(intNum);
}
#endif

#ifdef ENABLE_XHC_COMPANION
//------- xHCI --------
void xhci_ppc(struct xhc_comp *xhci, int bOn)
{
   MS_U16 addr_w, bit_num;
   MS_U32 addr, gpio_addr;
   MS_U8  value, low_active;


   switch (xhci->port_index) {
   case 0:
       addr_w = usb_readw((void*)(xhci->u3top_base+0xFC*2));
       addr = (MS_U32)addr_w << 8;
       addr_w = usb_readw((void*)(xhci->u3top_base+0xFE*2));
       addr |= addr_w & 0xFF;
       bit_num = (addr_w >> 8) & 0x7;
       low_active = (MS_U8)((addr_w >> 11) & 0x1);
       break;
   case 1:
       addr_w = usb_readw((void*)(xhci->u3top_base+0xE6*2));
       addr = (MS_U32)addr_w << 8;
       addr_w = usb_readw((void*)(xhci->u3top_base+0xE8*2));
       addr |= addr_w & 0xFF;
       bit_num = (addr_w >> 8) & 0x7;
       low_active = (MS_U8)((addr_w >> 11) & 0x1);
       break;
   default:        /* "can't happen" */
       return;
   }

   if (addr)
   {
       diag_printf("xhci_ppc: turn %s USB3.0 port %d power \n", (bOn) ? "on" : "off", xhci->port_index);
       diag_printf("Addr: 0x%x bit_num: %d low_active:%d\n", addr, bit_num, low_active);

       value = (MS_U8)(1 << bit_num);
       if (addr & 0x1)
           gpio_addr = _MSTAR_PM_BASE+addr*2-1;
       else
           gpio_addr = _MSTAR_PM_BASE+addr*2;

       if (low_active^bOn)
       {
           usb_writeb(usb_readb((void*)gpio_addr) | value, (void*)gpio_addr);
       }
       else
       {
           usb_writeb(usb_readb((void*)gpio_addr) & (MS_U8)(~value), (void*)gpio_addr);
       }
   }
   else {
       diag_printf("\n\n!!!! ERROR : xhci: no GPIO information for vbus port power control  !!!! \n\n");
       return;
   }
}

void U3phy_MS28_init(struct xhc_comp *xhci)
{
#ifdef XHCI_SINGLE_PORT_ENABLE_MAC
        writeb(readb((void*)(xhci->u3phy_D_base+0x84*2))|0x40, (void*)(xhci->u3phy_D_base+0x84*2)); // open XHCI MAC clock
#endif

	//-- 28 hpm mstar only---
	writew(0x0104, (void*) (xhci->u3phy_A_base+0x6*2));  // for Enable 1G clock pass to UTMI //[2] reg_pd_usb3_purt [7:6] reg_gcr_hpd_vsel

	//U3phy initial sequence
	writew(0x0,    (void*) (xhci->u3phy_A_base)); 		 // power on rx atop
	writew(0x0,    (void*) (xhci->u3phy_A_base+0x2*2));	 // power on tx atop
	//writew(0x0910, (void*) (U3PHY_D_base+0x4*2));        // the same as default
	writew(0x0,    (void*) (xhci->u3phy_A_base+0x3A*2));  // overwrite power on rx/tx atop
	writew(0x0160, (void*) (xhci->u3phy_D_base+0x18*2));
	writew(0x0,    (void*) (xhci->u3phy_D_base+0x20*2));	 // power on u3_phy clockgen
	writew(0x0,    (void*) (xhci->u3phy_D_base+0x22*2));	 // power on u3_phy clockgen

#ifdef XHCI_ENABLE_PD_OVERRIDE
        writew(0x308,    (void*) (xhci->u3phy_A_base+0x3A*2)); // [9,8,3] PD_TXCLK_USB3TXPLL, PD_USB3_IBIAS, PD_USB3TXPLL override enable
        writeb(readb((void*)(xhci->u3phy_A_base+0x3*2-1)) & 0xbb,     (void*)(xhci->u3phy_A_base+0x3*2-1)); // override value 0
#endif
	writeb(0xF4,   (void*) (xhci->u3phy_D_base+0x12*2));	  //TX lock threshold
}

void xhci_enable_clock(void)
{
	static int clock_enable = 0;
#ifdef XHCI_PORT0_ADDR
	struct xhc_comp xc = XHC_COMP_PORT0;
#endif
#ifdef XHCI_PORT1_ADDR
	struct xhc_comp xc1 = XHC_COMP_PORT1;
#endif

	if (clock_enable)
		return;

#ifdef XHCI_PORT0_ADDR
	diag_printf("xhci_enable_clock\n");
	U3phy_MS28_init(&xc);
	clock_enable = 1;
#endif
#ifdef XHCI_PORT1_ADDR
	#ifdef XHCI_2PORTS
	U3phy_MS28_init(&xc1);
	#endif
#endif
}

MS_U32 xhci_port_state_to_neutral(MS_U32 state)
{
    /* Save read-only status and port state */
    return (state & XHCI_PORT_RO) | (state & XHCI_PORT_RWS);
}

void xhci_ssport_set_state(struct xhc_comp *xhci, int bOn)
{
    MS_U32 temp;

    temp = readl((void*)(xhci->xhci_port_addr));
    diag_printf("port status 0x%lx: 0x%lx\n", xhci->xhci_port_addr, temp);
    if (bOn) {
        if ((temp & PORT_PLS_MASK) == USB_SS_PORT_LS_SS_DISABLED) {

            temp = xhci_port_state_to_neutral(temp);
            temp &= ~PORT_PLS_MASK;
            temp |= PORT_LINK_STROBE | USB_SS_PORT_LS_RX_DETECT;

            writel(temp, (void*)(xhci->xhci_port_addr));

            wait_ms(10);
            temp = readl((void*)(xhci->xhci_port_addr));
            diag_printf("port status: 0x%lx\n", temp);
        }
    } else {
        if ((temp & PORT_PLS_MASK) != USB_SS_PORT_LS_SS_DISABLED) {

            temp = xhci_port_state_to_neutral(temp);
            writel(temp | PORT_PE, (void*)(xhci->xhci_port_addr));

            wait_ms(10);
            temp = readl((void*)(xhci->xhci_port_addr));
            diag_printf("port status: 0x%lx\n", temp);
        }
    }
}
//--------------------
#endif
