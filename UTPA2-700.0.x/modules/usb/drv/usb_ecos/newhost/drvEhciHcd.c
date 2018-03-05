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

#include "include/drvConfig.h"

#ifdef CONFIG_USB_DEBUG
  #define DEBUG
#else
  #undef DEBUG
#endif

//#include <MsCommon.h> // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvPorts.h" // NUSED
#include "include/drvErrno.h"
//#include "include/drvPCIMEM.h" // NUSED
//#include "include/drvList.h" // NUSED
//#include "include/drvTimer.h" // NUSED
//#include "include/drvKernel.h" // NUSED
// USB related implemented header files
#include "include/drvUSBHost.h"
#include "drvUsbd.h"
//#include "include/drvCPE_EHCI.h" // NUSED
#include "drvEHCI.h"
#include "drvUSBHwCtl.h"
//#include "include/drvCPE_AMBA.h" // NUSED
//#include "drvUSB.h" // NUSED
/* should applying drvUsbHostConfig.h (inside drvUSBHwctl.h) */

/*-------------------------------------------------------------------------*/


#define DRIVER_VERSION "eCos Newhost"
#define DRIVER_AUTHOR "Mstar USB Team"
#define DRIVER_DESC "USB 2.0 'Enhanced' Host Controller (EHCI) Driver"

// #define EHCI_VERBOSE_DEBUG
//#define EHCI_TD_DEBUG
#define ENABLE_QH_REFRESH

#ifdef DEBUG
#define EHCI_STATS
#endif

#define  EHCI_RL_NAKCNT     4  /* enable nak throttle mode, spec 4.9 */
#define  EHCI_RL_NAKCNT_TT  0  /* disable nak throttle mode when TT */
#define  EHCI_HI_BW_MULT_HS 1  /* high bandwidth file, transactions number/uframe, spec 4.10.3 */
#define  EHCI_HI_BW_MULT_TT 1

#ifdef CONFIG_HC_TEST
#define  EHCI_PERIODIC_FLS      0  /* (max) 1024 frame for schedule */
#else
#define  EHCI_PERIODIC_FLS      2  /* (minl) 256 frame schedule */
#endif
/* Initial IRQ latency:  lower than default */
static int log2_irq_thresh = 0;    // 0 to 6
#define    INTR_MASK (USBSTS_IAA | USBSTS_FATAL | USBSTS_PCD | USBSTS_ERR | USBSTS_INT)

#if defined(ENABLE_16US_EOF1)
static U32 park_eco = 3;
#else
static U32 park_eco = 0;
#endif

/* Debug print definition */
#define DBG_MSG
#define DBG_WARN
//#define DBG_FUNC
#undef  ms_debug_msg
#undef  ms_debug_warn
#undef  ms_debug_func

#ifdef DBG_MSG
#define ms_debug_msg(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_msg(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_WARN
#define ms_debug_warn(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_warn(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_FUNC
#define ms_debug_func(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_func(fmt, arg...) do {} while (0)
#endif

static void ms_unlink_async(struct ehci_hcd *, struct ehci_qh *);
static void ms_intr_deschedule(struct ehci_hcd *, struct ehci_qh *);
static int ms_qh_schedule(struct ehci_hcd *, struct ehci_qh *);

void ms_ehci_stoprun_setting(MS_U8 bOption, struct ehci_hcd *ehci);
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);

#ifdef EHCI_TD_DEBUG
void Dump_TD(unsigned int addr, int is_qh)
{
	int i;

	diag_printf("[USB] %s:%x -> \n", is_qh?"QH":"QTD", addr);
	for (i = 0; i < 0x20 ; i = i+4)
	{
		diag_printf("%02x ", *(unsigned char volatile *)(addr + i+ 3));
		diag_printf("%02x ", *(unsigned char volatile *)(addr + i+ 2));
		diag_printf("%02x ", *(unsigned char volatile *)(addr + i+ 1));
		diag_printf("%02x ", *(unsigned char volatile *)(addr + i));
		diag_printf("\n");
	}
}
#endif

static int ms_check_status (U32 *pPtr, U32 u32Mask, U32 u32Done, int us)
{
    U32  u32regv;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    do {
        u32regv = hcd_reg_readl ((U32)pPtr);
        if (u32regv == ~(U32)0)    /* card removed */
        {
            ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
            return -ENODEV;
        }

        u32regv &= u32Mask;
        if (u32regv == u32Done)
            return 0;

        udelay (1);
        us--;
    } while (us > 0);

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return -ETIMEDOUT;
}

static int ms_ehci_halt (struct ehci_hcd *pEhci)
{
    U32  u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if ((u32regv & USBSTS_HALT) != 0)
    {
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return 0;
    }

    u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    u32regv &= ~USBCMD_RUN;
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return ms_check_status (&pEhci->op_regs->usbsts, USBSTS_HALT, USBSTS_HALT, 16 * 125);
}

static int ms_ehci_reset (struct ehci_hcd *pEhci)
{
    int retv;
    U32  u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    u32regv |= USBCMD_RESET;
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);
    pEhci->hcd.state = HCD_STATE_HALT;
    //Wait for HC reset complete
    retv = ms_check_status (&pEhci->op_regs->usbcmd, USBCMD_RESET, 0, 250 * 1000);

#if defined(ENABLE_UHC_RUN_BIT_ALWAYS_ON_ECO)
    /* Don't close RUN bit when device disconnect */
    {
        U32 u32hcmisc = hcd_reg_readl ((U32)&pEhci->op_regs->hcmisc);
        hcd_reg_writel(u32hcmisc | BIT7, (U32)&pEhci->op_regs->hcmisc);
    }
#endif

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return retv;
}

static void ms_ehci_quiesce (struct ehci_hcd *pEhci)
{
    U32  u32regv;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

    /* check if there is any schedule bit set (patch from Linux 2.6.28  */
    u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd) << 10;
    u32regv &= USBSTS_ASS | USBSTS_PSS;
    if (ms_check_status (&pEhci->op_regs->usbsts, USBSTS_ASS | USBSTS_PSS,
        0, 16 * 125) != 0)
    {
        ms_debug_err("Cannot stop scheduler, force halt - 1\n");
        ms_ehci_halt(pEhci);
        pEhci->hcd.state = HCD_STATE_HALT;
        return;
    }

    u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    u32regv &= ~(USBCMD_ASE | USBCMD_IAAD | USBCMD_PSE);
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);

    /* wait 16 microframes */
    if (ms_check_status (&pEhci->op_regs->usbsts, USBSTS_ASS | USBSTS_PSS,
        0, 16 * 125) != 0)
    {
        ms_debug_err("Cannot stop scheduler, force halt - 2\n");
        /* patch from Linux 2.6.28 */
        ms_ehci_halt(pEhci);
        pEhci->hcd.state = HCD_STATE_HALT;
        return;
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*
  * @brief             clear reset timer
  *
  * @param          struct ehci_hcd  *pEhci
  * @param          int  iIndex
  * @param          int  port_status
  *
  * @return           port_status
  */

static int ms_reset_complete (
  struct ehci_hcd  *pEhci,
  int    iIndex,
  int    port_status)
{
    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

    if (!(port_status & PORTSC_CONNECT))
    {
        //pEhci->u32ResetEnd [iIndex] = 0; // un-patch from Linux 2.6.28
        return port_status;
    }

    /* Mstar EHCI is integrated TT */
    if (!(port_status & PORTSC_PE))
    {
        ms_debug_err("TT roothub not enable port\n");
    }

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return port_status;
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          check hub ports status change
  *                 our echi ip (root hub) supports only one port
  *
  * @param          struct usb_hcd  *pHcd
  * @param          char *pBuf
  *
  * @return         success/false
  */
int ms_hub_status_data (struct usb_hcd *pHcd, char *pBuf)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);
    U32  u32Temp, u32Status = 0;
    U32  u32Flags;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

	if (!HCD_IS_RUNNING(pHcd->state)) // patch from 2.6.28
		return 0;

    /* init u32Status to no-changes */
    pBuf [0] = 0;

    /* port 1 changes ? */
    osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);

    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc[0]);

    /* PCD interrupt mode, not utilize such scheme */
    //if ( ((u32Temp & PORTSC_CONNECT) == 0) && ((pHcd->pre_sts & PORTSC_CONNECT) != 0) )
    //{
    //    ms_debug_msg("<ms_hub_status_data> Add CSC @ root hub %d, DISCONNECT! (psc = 0x%x)\n", (int)pHcd->host_id, (unsigned int)u32Temp);
    //    u32Temp |= PORTSC_CSC;
    //}
    //pHcd->pre_sts = u32Temp;

    //if (!(u32Temp & PORTSC_CONNECT))  // un-patch from Linux 2.6.28
    //  pEhci->u32ResetEnd [0] = 0;

    /* no need to consider over current for this IP */
    if ((u32Temp & (PORTSC_CSC) || PORTSC_PEC) != 0 ||
            (pEhci->u32ResetEnd[0] && ms_time_after_eq(
					jiffies, pEhci->u32ResetEnd[0])))
    {
      /* bit 0 is for hub up port u32Status change  */
      pBuf [0] |= 1 << 1;
      u32Status = USBSTS_PCD;
    }

    osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return u32Status ? 1 : 0;
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          fill hub descriptor
  *
  * @param          struct ehci_hcd  *pEhci
  * @param          struct usb_hub_descriptor  *pDesc
  *
  * @return         none
  */

static void ms_get_roothub_desc (
  struct ehci_hcd      *pEhci,
  struct usb_hub_descriptor  *pDesc
)
{
  int    iPorts = HCS_N_PORTS (pEhci->hcs_params);
  U16    u16Temp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pDesc->bDescriptorType = 0x29;
  pDesc->bPwrOn2PwrGood = 0;
  pDesc->bHubContrCurrent = 0;
  pDesc->bNbrPorts = iPorts;
  u16Temp = 1 + (iPorts / 8);
  pDesc->bDescLength = 7 + 2 * u16Temp;

  /* two bitmaps:  iPorts removable, and usb 1.0 legacy PortPwrCtrlMask */
  memset (&pDesc->DeviceRemovable [0], 0, u16Temp);
  memset (&pDesc->DeviceRemovable [u16Temp], 0xff, u16Temp);

  u16Temp = 0x0008;       /* Individual Port Over-current Protection */
  if (HCS_PPC (pEhci->hcs_params))
    u16Temp |= 0x0001;    /* Ganged power switching (all iPorts' power at once) */
  if (HCS_P_INDICATOR (pEhci->hcs_params))
    u16Temp |= 0x0080;    /* Individual port indicators (LEDs) */
  pDesc->wHubCharacteristics = u16Temp;
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/
void ms_ehci_softrst(struct ehci_hcd  *pEhci)
{
    U32    regTmp[2];

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

    /* ensure HC already stopped before HC Reset
     * otherwise, there will be a fatal error (DMA Error)
     */
    ms_ehci_halt(pEhci);
    /* make sure USB device going to suspend */
    //mdelay(10); // TBD

    /* iff asynchronous QH head is not pointed to itself, restore it too */
    if ((pEhci->hcd.rh_disconn == 1) &&
        (pEhci->stAsync->hw_next_qh != QH_NEXT(pEhci->stAsync->qh_dma_addr)))
    {
        ms_debug_err("[EHCI] Head QH not point to itself (%x)!\n\n\n",
            pEhci->stAsync->hw_next_qh);
        pEhci->stAsync->hw_next_qh = QH_NEXT(pEhci->stAsync->qh_dma_addr);
    }
    if (!hcd_reg_readl((U32)&pEhci->op_regs->asynclistaddr))
        ms_debug_err("[EHCI] async reg 0!!!\n");

    /* sometimes, replug device quickly crashs aync_base, just restore it */
    hcd_reg_writel ((U32)pEhci->stAsync->qh_dma_addr, (U32)&pEhci->op_regs->asynclistaddr);

    regTmp[0] = hcd_reg_readl((U32)&pEhci->op_regs->usbcmd);
    regTmp[1] = hcd_reg_readl((U32)&pEhci->op_regs->usbintr);
    // HC soft reset
    ms_ehci_reset(pEhci);
    // restore regisger setting
    hcd_reg_writel(regTmp[1],(U32)&pEhci->op_regs->usbintr);
    hcd_reg_writel(regTmp[0],(U32)&pEhci->op_regs->usbcmd);
    pEhci->hcd.state = HCD_STATE_RUNNING;
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

static const char *const speed_name_list[] = {
	"full-speed",
	"low-speed",
	"high-speed",
	"UNKNOWN",
};
/*-------------------------------------------------------------------------*/
/*
  * @brief          process hub request
  *
  * @param          struct usb_hcd  *pHcd
  * @param          U16    typeReq
  * @param          U16    u16Value
  * @param          U16    u16Index
  * @param          char    *pBuf
  *
  * @return         error code
  *
  */
 int ms_hub_control (
  struct usb_hcd  *pHcd,
  U16    typeReq,
  U16    u16Value,
  U16    u16Index,
  char    *pBuf
)
{
  struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);
  int    iPorts = HCS_N_PORTS (pEhci->hcs_params);
  U32    u32Temp, u32Status, u32Bus_monitor;
  U32  u32Flags;
  int    iRetval = 0;
  int    time_out;
  struct cpe_dev *dev;
  const struct device_s *__mptr = pHcd->controller;
  dev = (struct cpe_dev *)( (char *)__mptr - offsetof(struct cpe_dev,dev) );
  U32 u32RegUTMI = dev->utmibase;
  U32 u32Temp2, u32Speed;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  switch (typeReq)
  {
  case Req_ClearHubFeature:
    if ((u16Value != C_HUB_LOCAL_POWER)&&(u16Value != C_HUB_OVER_CURRENT))
      goto error;
    break;
  case Req_ClearPortFeature:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    u32Temp &= ~PORTSC_RWC_BITS;

    if (u32Temp & PORTSC_OWNER)
      break;

    switch (u16Value)
    {
    case USB_PORT_FEAT_ENABLE:
      ms_debug_debug("Disable port\n");
      hcd_reg_writel (u32Temp & ~PORTSC_PE, (U32)&pEhci->op_regs->portsc [u16Index]);
#if 0
      // not in linux
      //裝置被移除的時候 停止scheduler
      hcd_reg_writel(hcd_reg_readl((U32)&pEhci->op_regs->usbcmd)&(~(USBCMD_ASE|USBCMD_PSE|USBCMD_IAAD)),(U32)&pEhci->op_regs->usbcmd);
#endif

      #if 0 // not to reset EHCI at this time
      //Halt HCD
      ms_host_halt(pEhci);
      ms_ehci_softrst(pEhci);
      #endif
      break;
    case USB_PORT_FEAT_C_ENABLE:
      hcd_reg_writel ((u32Temp /*& ~PORTSC_RWC_BITS*/) | PORTSC_PEC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_SUSPEND:
    case USB_PORT_FEAT_C_SUSPEND:
      break;
    case USB_PORT_FEAT_POWER:
      if (HCS_PPC (pEhci->hcs_params))
        hcd_reg_writel (u32Temp & ~PORTSC_POWER,
          (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_C_CONNECTION:
      /* for force HC Reset */
      if(pEhci->u32MoreCSC)
      {
         diag_printf("[EHCI] clear more CSC\n");
         pEhci->u32MoreCSC = false;
      }
      hcd_reg_writel ((u32Temp /*& ~PORTSC_RWC_BITS*/) | PORTSC_CSC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_C_OVER_CURRENT:
      hcd_reg_writel ((u32Temp /*& ~PORTSC_RWC_BITS*/) | PORTSC_OCC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_C_RESET:
      /* GetPortStatus clears reset */
      break;
    default:
      goto error;
    }
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);  /* unblock posted write */
    break;
  case Req_GetHubDescriptor:
    ms_get_roothub_desc(pEhci, (struct usb_hub_descriptor *)
      pBuf);
    break;
  case Req_GetHubStatus:
    /* no hub-wide feature/u32Status u32Flags */
    memset (pBuf, 0, 4);
    //cpu_to_le32s ((U32 *) pBuf);
    break;
  case Req_GetPortStatus:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Status = 0;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    // old flow to patch hub polling
    //if ( ((u32Temp & PORTSC_CONNECT) == 0) && ((u32Temp & PORTSC_CSC) == 0) )
    //{
    //    if ((pHcd->pre_temp & PORTSC_CONNECT) != 0)
    //    {
    //        ms_debug_msg("<ms_hub_control> Req_GetPortStatus:Add CSC here, pStatus = %x\n", (unsigned int)u32Temp);
    //        u32Temp |= PORTSC_CSC;
    //    }
    //}
    //pHcd->pre_temp = u32Temp;

    /* deal with wPortChange bits */
    if (u32Temp & PORTSC_CSC)
    {
      u32Status |= 1 << USB_PORT_FEAT_C_CONNECTION;
    }
    else if(pEhci->u32MoreCSC)
    {
      diag_printf("[EHCI] retrun more CSC\n");
      u32Status |= 1 << USB_PORT_FEAT_C_CONNECTION;
    }
    if (u32Temp & PORTSC_PEC)
      u32Status |= 1 << USB_PORT_FEAT_C_ENABLE;
    // USB_PORT_FEAT_C_SUSPEND
    if (u32Temp & PORTSC_OCC)
      u32Status |= 1 << USB_PORT_FEAT_C_OVER_CURRENT;

    if ((u32Temp & PORTSC_RESET)
        && ms_time_after (jiffies,
          pEhci->u32ResetEnd [u16Index]))
    {
      u32Status |= 1 << USB_PORT_FEAT_C_RESET;
      pEhci->u32ResetEnd [u16Index] = 0; // patch from Linux 2.6.28
      time_out=0;

      writeb(UTMI_DISCON_LEVEL_2A ,(void*)(u32RegUTMI+0x2A*2)); // disconnect level
      mdelay(20); // chirp patch

      /* Do port reset until finish */
      //hcd_reg_writel (u32Temp & ~PORTSC_RESET,
      hcd_reg_writel (u32Temp & ~(PORTSC_RWC_BITS | PORTSC_RESET),
          (U32)&pEhci->op_regs->portsc [u16Index]);
      do {
        u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
        udelay (10);
      } while ((u32Temp&(PORTSC_RESET|PORTSC_CONNECT)) == (PORTSC_RESET|PORTSC_CONNECT)&&(time_out++<10000));

      writeb( UTMI_EYE_SETTING_2C, (void*) (u32RegUTMI+0x2c*2)); // chirp patch
      writeb( UTMI_EYE_SETTING_2D, (void*) (u32RegUTMI+0x2d*2-1)); // chirp patch
      writeb( UTMI_EYE_SETTING_2E, (void*) (u32RegUTMI+0x2e*2)); // chirp patch
      writeb( UTMI_EYE_SETTING_2F, (void*) (u32RegUTMI+0x2f*2-1)); // chirp patch
#ifdef ENABLE_TX_RX_RESET_CLK_GATING_ECO
      // DO UTMI TX/RX RESET
      writeb( readb((void*)(u32RegUTMI+0x06*2)) | 0x03, (void*) (u32RegUTMI+0x06*2));
      writeb( readb((void*)(u32RegUTMI+0x06*2)) & ~0x03, (void*) (u32RegUTMI+0x06*2));
#endif

      //Add time-out mechanism to cover HW BUG
      //HW ISSUE?
      if( time_out >= 10000 )
      {
          //Maybe HW bug ==> check it out
          ms_debug_err("ERR: Port reset cannot force complete!!\n");//HW ISSUE?
          ms_ehci_softrst(pEhci);
      }
      u32Speed = (hcd_reg_readl((U32)&pEhci->op_regs->bus_control) >> 9) & 0x3;
      //ms_debug_msg("SPEED: %x\n", (unsigned int)u32Speed);
      ms_debug_msg("RootHub SPEED: %s\n", speed_name_list[u32Speed]);

      u32Temp2 = hcd_reg_readl((U32)&pEhci->op_regs->hcmisc) & 0xfffffff3;

      /* unset UTMI force full speed hub driving timing */
      ms_UTMI_ANDXBYTE_EX(0x03, (~BIT2), u32RegUTMI);

      if (u32Speed == 2) // high u32Speed
      {
        ms_UTMI_ORXBYTE_EX(0x09, 0x80, u32RegUTMI); //HS rx robust enable
#if defined(ENABLE_16US_EOF1)
        /* set EOF1 to 16us for babble prevention under hub case */
        //diag_printf("[EHCI] set 16us EOF1\n ");
        u32Temp2 |= (1 << 2);
#else
        u32Temp2 |= (3 << 2);
#endif
      }
      else if (u32Speed == 0 ) //full u32Speed
      {
        /* set UTMI force full speed hub driving timing */
        ms_UTMI_ORXBYTE_EX(0x03, BIT2, u32RegUTMI);
        ms_UTMI_ANDXBYTE_EX(0x09, 0x7F, u32RegUTMI);
        u32Temp2 |= (2 << 2);
      }
      else if (u32Speed == 1 ) //low u32Speed
      {
        ms_UTMI_ANDXBYTE_EX(0x09, 0x7F, u32RegUTMI);
        u32Temp2 |= (2 << 2);
      }
      hcd_reg_writel (u32Temp2, (U32)&pEhci->op_regs->hcmisc); // misc, EOF1

      /* patch from Linux 2.6.28 */
      //u32Temp = ms_reset_complete (pEhci, u16Index, u32Temp);
      u32Temp = ms_reset_complete (pEhci, u16Index,
            hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]));
      pHcd->isRootHubPortReset = FALSE;
    }

    if (!(u32Temp & (PORTSC_RESUME|PORTSC_RESET))) // patch from Linux 2.6.28
        pEhci->u32ResetEnd[u16Index] = 0;

    // don't show wPortStatus if it's owned by a companion hc
    /* patch from Linux 2.6.28, TT root hub */
    //if (!(u32Temp & PORTSC_OWNER))
    {
      if (u32Temp & PORTSC_CONNECT)
      {
        u32Status |= USB_PORT_STAT_CONNECTION;
        u32Bus_monitor=hcd_reg_readl ((U32)&pEhci->op_regs->bus_control);
        u32Bus_monitor>>=9;
        if (u32Bus_monitor==2)
            u32Status |= USB_PORT_STAT_HIGH_SPEED;
        else if (u32Bus_monitor==1)
            u32Status |= USB_PORT_STAT_LOW_SPEED;
        //else ==> full u32Speed don't set any bits

      }
      if (u32Temp & PORTSC_PE)
        u32Status |= USB_PORT_STAT_ENABLE;
      if (u32Temp & (PORTSC_SUSPEND | PORTSC_RESUME))
        u32Status |= USB_PORT_STAT_SUSPEND;
      if (u32Temp & PORTSC_OC)
        u32Status |= USB_PORT_STAT_OVERCURRENT;
      if (u32Temp & PORTSC_RESET)
        u32Status |= USB_PORT_STAT_RESET;
      if (u32Temp & PORTSC_POWER)
        u32Status |= USB_PORT_STAT_POWER;
    }

    *pBuf = u32Status;
    *(pBuf+1) = u32Status >> 8;
    *(pBuf+2) = u32Status >> 16;
    *(pBuf+3) = u32Status >> 24;
    break;
  case Req_SetHubFeature:
    if ((u16Value != C_HUB_LOCAL_POWER)&&(u16Value != C_HUB_OVER_CURRENT))
      goto error;
    break;
  case Req_SetPortFeature:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    #if 0 // MStar patch for Faraday IP
    if (u32Temp & PORTSC_OWNER)
      break;
    #endif
    // patch from Linux
    u32Temp &= ~PORTSC_RWC_BITS;

    switch (u16Value)
    {
    case USB_PORT_FEAT_SUSPEND:
      ms_debug_msg("SetPortFeature suspend\n");
      hcd_reg_writel (u32Temp | PORTSC_SUSPEND,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_POWER:
      if (HCS_PPC (pEhci->hcs_params))
        hcd_reg_writel (u32Temp | PORTSC_POWER,
          (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_RESET:
      //Do port reset only if device attaches to this port
      if( u32Temp&PORTSC_CONNECT )
      {
        pHcd->isRootHubPortReset = TRUE;
        ms_ehci_stoprun_setting(HOST20_Enable, pEhci);

        writeb(0x10, (void*)(u32RegUTMI+0x2C*2)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2D*2-1)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2E*2)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2F*2-1)); // chirp patch
        // Chirp K detection level: 0x80 => 400mv, 0x20 => 575mv
        writeb(0x80 ,(void*)(u32RegUTMI+0x2A*2)); // chirp patch

        u32Temp |= PORTSC_RESET;
        u32Temp &= ~PORTSC_PE;
        /*
        * 50 ms resets on root for usb spec.
        */
        pEhci->u32ResetEnd [u16Index] = jiffies
              + ((50 /* msec */ * HZ) / 1000);
        hcd_reg_writel (u32Temp, (U32)&pEhci->op_regs->portsc [u16Index]); // port reset
      }
      else
      {
        /* fail reset process, if device has gone */
        diag_printf("[USB] device has gone before bus reset\n");
        iRetval = -ENODEV;
      }
      break;
    default:
      goto error;
    }
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);  /* unblock posted writes */
    break;

  default:
error:
    /* issue stall */
    iRetval = -EPIPE;
  }
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return iRetval;
}

/*------------------------------------------------------------------------------*/
#include "drvEHCI_MEM.cxx"

/*
  * @brief          fill qtd buffer from urb
  *
  * @param          struct ehci_qtd *pQtd
  * @param          dma_addr_t tBufAddr
  * @param          size_t total
  * @param          int iToken
  * @param          int iMaxpkt
  *
  * @return         u32Cnt
  */
static int
ms_qtd_pack (struct ehci_qtd *pQtd, dma_addr_t tBufAddr, size_t total,
    int iToken, int iMaxpkt)
{
  U32 u32Cnt;
  int  i/*, lastnums = 0*/;
  //unsigned char	u8PageShift;

  /* buffer must continuous, first page address may not on 4K boundary  */

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQtd->hw_buffer [0] = tBufAddr;
  u32Cnt = 0x1000 - (tBufAddr & 0x0fff);  /* rest of that page */
  if (total < u32Cnt)    /* ... iff needed */
    u32Cnt = total;
  else
  {
    tBufAddr +=  0x1000;
    tBufAddr &= ~0x0fff;

    /* pQtd buffer range : min 16K, max 20K */
    for (i = 1; u32Cnt < total && i < 5; i++)
    {
      pQtd->hw_buffer [i] = tBufAddr;
      //lastnums++;
      tBufAddr += 0x1000;
      u32Cnt = ((u32Cnt+0x1000) < total) ? (u32Cnt+0x1000) : total;
    }

    /* short packets on transfer end */
    if (u32Cnt != total)
      u32Cnt -= (u32Cnt % iMaxpkt);
  }
  pQtd->hw_token = (u32Cnt << 16) | iToken;
  pQtd->length = u32Cnt;
  /* un-patched base on linux code */
  //if ((u32Cnt > 0) && (lastnums < 4))
  //{
  //    for (u8PageShift = lastnums + 1;u8PageShift < 5; u8PageShift++)
  //    {
  //        pQtd->hw_buffer[u8PageShift] = pQtd->hw_buffer [lastnums];
  //    }
  //}
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return u32Cnt;
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          refresh QH overlay, based on Linux 3.10.x
  *                 merge qh_refresh and qh_update
  *
  * @param          struct ehci_hcd *pEhci
  * @param          struct ehci_qh *pQh
  *
  * @return         none
  */

static void
ms_qh_refresh (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
	struct ehci_qtd *pQtd;

	ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
	pQtd = entry_to_container(pQh->qtd_list.next, struct ehci_qtd, qtd_list);

	if (pQh->hw_token & QTD_STS_ACT)
	{
		ms_debug_debug("[USB] QTD_STS_ACT, (%x,%x)\n", pQh->hw_next_qtd, pQtd->hw_next_qtd);
		pQh->hw_next_qtd = pQtd->hw_next_qtd;
	}
	else {
		if (pQh->qh_status != QH_STS_IDLE)
			ms_debug_err("[USB] Not in IDLE state!\n");
		pQh->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
		pQh->hw_alt_next_qtd = EHCI_LIST_END;

		if (!(pQh->hw_ep_state1 & QH_DTC_BIT)) {
			unsigned	is_out, epnum;

			is_out = usb_pipeout(pQtd->urb->u32Pipe);
			epnum = (pQh->hw_ep_state1 >> 8) & 0x0f;
			if (!usb_gettoggle (pQh->dev, epnum, is_out)) {
				pQh->hw_token &= ~QTD_TOGGLE;
				usb_settoggle (pQh->dev, epnum, is_out, 1);
			}
		}

		pQh->hw_token &= (QTD_TOGGLE | QTD_STS_PERR);
	}

	ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

#ifndef ENABLE_QH_REFRESH // replaced by ms_qh_refresh()
/*-------------------------------------------------------------------------*/
/*
  * @brief          link qtd list to qh
  *
  * @param          struct ehci_qh *pQh
  * @param          struct ehci_qtd *pQtd
  *
  * @return         none
  */

static __inline__ void
ms_qh_update_link (struct ehci_qh *pQh, struct ehci_qtd *pQtd)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQh->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
  pQh->hw_alt_next_qtd = EHCI_LIST_END;

  /* HC must see latest pQtd and pQh data before we clear ACTIVE+HALT */
  wmb ();
  pQh->hw_token &= (QTD_TOGGLE | QTD_STS_PERR);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}
#endif

/*-------------------------------------------------------------------------*/
/*
  * @brief             check qtd status register
  *
  * @param          struct urb *pUrb
  * @param          size_t length
  * @param          U32 u32Token
  *
  * @return           none
  */

static void ms_qtd_copy_status (
  struct urb *pUrb,
  size_t length,
  U32 u32Token
)
{
#ifdef USB_EHCI_TT
  struct usb_hcd *hcd = pUrb->dev->bus->hcpriv;
#endif

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  if (QTD_PID (u32Token) != 2)
    pUrb->u32ActualLength += length - QTD_BYTES (u32Token);

  /* error codes 不要去改它 */
  if (pUrb->s32Status != -EINPROGRESS)
    return;

  /* force cleanup , may not a wrong */
  if (IS_SHORT_READ (u32Token))
    pUrb->s32Status = -MS_ERR_REMOTEIO;

  if (u32Token & QTD_STS_HALT)
  {
    if (u32Token & QTD_STS_BABBLE)
    {
      diag_printf(" [ !!!!  USB   QTD_STS_BABBLE  !!!!] \n "); // SHOULD BE PRINT
      pUrb->s32Status = -MS_ERR_OVERFLOW;
    }
    else if (u32Token & QTD_STS_MISSMF)
    {
        diag_printf(" [ !!!!  USB	QTD_STS_MISSMF	!!!!] \n "); // SHOULD BE PRINT
      /* FS/LS transfer lost complete-split */
      pUrb->s32Status = -MS_ERR_PROTOCAL;
    }
    else if (u32Token & QTD_STS_DATERR)
    {
        diag_printf(" [ !!!!  USB	QTD_STS_DATERR	!!!!] \n "); // SHOULD BE PRINT
      pUrb->s32Status = (QTD_PID (u32Token) == 1) /* Is IN token? */
        ? -MS_ERR_RECEIVE   /* HCD can't read data */
        : -MS_ERR_SEND;     /* HCD can't write data */
    }
    else if (u32Token & QTD_STS_XACTERR)
    {
        ms_debug_msg(" [ USB	QTD_STS_XACTERR ] \n ");

      if (QTD_CERR (u32Token))    /* 錯誤型態timeout, bad crc, wrong PID */
        pUrb->s32Status = -EPIPE;
      else
      {
          ms_debug_warn ("devpath %s ep%d%s 3strikes\n",
                pUrb->dev->devpath,
                (int)usb_pipeendpoint (pUrb->u32Pipe),
                usb_pipein (pUrb->u32Pipe) ? "in" : "out");
          pUrb->s32Status = -MS_ERR_PROTOCAL;
      }
    /* 沒有error bit 但是有halt, will stall */
    }
    else if (QTD_CERR (u32Token))
      pUrb->s32Status = -EPIPE;
    else  /* 未知的錯誤 */
      pUrb->s32Status = -MS_ERR_PROTOCAL;

    if( pUrb->s32Status < 0 )
    {
      ms_debug_warn("dev%d ep%d%s qtd token %08x --> status %d, len=%d\n",
            (int)usb_pipedevice (pUrb->u32Pipe),
            (int)usb_pipeendpoint (pUrb->u32Pipe),
            usb_pipein (pUrb->u32Pipe) ? "in" : "out",
            (unsigned int)u32Token, (int)pUrb->s32Status, (int)pUrb->u32TransferBufferLength);
    }


    /* Stall 表示需要做error recovery */
    if (pUrb->s32Status == -EPIPE)
    {
      /* halt bit obsolete */
      //int  pipe = pUrb->u32Pipe;

      //if (!usb_pipecontrol (pipe))
      //    usb_endpoint_halt (pUrb->dev, usb_pipeendpoint (pipe), usb_pipeout (pipe));

    }
    /* if async CSPLIT failed, try cleaning out the TT buffer */
    /* patch from linux 2.6.28 */
#ifdef USB_EHCI_TT
    else if (pUrb->dev->tt && !usb_pipeint (pUrb->u32Pipe)
        && ((u32Token & QTD_STS_MISSMF) != 0 || QTD_CERR(u32Token) == 0)
        && (pUrb->dev->tt->hub != hcd->self.root_hub))
    {
//#ifdef DEBUG
      ms_debug_msg ("clear tt buffer port %d, a%d ep%d t%08x\n",
        pUrb->dev->u32TTPort, pUrb->dev->u32DevNum,
        usb_pipeendpoint (pUrb->u32Pipe), u32Token);
//#endif
      ms_usb_hub_tt_clear_buffer (pUrb->dev, pUrb->u32Pipe);
    }
#endif
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}


static void
ms_urb_done (struct ehci_hcd *pEhci, struct urb *pUrb, struct stPtRegs *pRegs)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (pUrb->hcpriv != 0)
  {
    struct ehci_qh  *qh = (struct ehci_qh *) pUrb->hcpriv;
    ms_qh_put (pEhci, qh);
  }

  osapi_spin_lock (&pUrb->lock);
  pUrb->hcpriv = 0;
  switch (pUrb->s32Status)
  {
  case -EINPROGRESS:    /* success */
    pUrb->s32Status = 0;
  default:      /* fault */
    INCREASE (pEhci->stats.u32Complete);
    break;
  case -MS_ERR_REMOTEIO:    /* fault or normal */
    if (!(pUrb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK))
      pUrb->s32Status = 0;
    INCREASE (pEhci->stats.u32Complete);
    break;
  case -ECONNRESET:    /* canceled */
  case -ENOENT:
    INCREASE(pEhci->stats.u32Unlink);
    break;
  }
  osapi_spin_unlock (&pUrb->lock);

  /* HCD run complete() funciton */
  osapi_spin_unlock (&pEhci->tHcdLock);
  ms_usb_hcd_giveback_urb (pUrb, pRegs);
  osapi_spin_lock (&pEhci->tHcdLock);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*
  * @brief          free qtd list and run urb's complet function
  *
  * @param          struct ehci_hcd *pEhci
  * @param          struct ehci_qh *pQh
  * @param          struct stPtRegs *pRegs
  *
  * @return         u8Count
  */

#define HALT_BIT QTD_STS_HALT
static unsigned
ms_qh_completions (struct ehci_hcd *pEhci, struct ehci_qh *pQh, struct stPtRegs *pRegs)
{
  struct ehci_qtd    *pLast = 0, *pEnd = pQh->pDummyQtd;
  struct list_head  *entry, *tmp;
  int      iStopped;
  unsigned    u8Count = 0;
  int      iDoStatus = 0;
  unsigned char      u8State;
  //BOOL      bIsPageOver = FALSE; // obsolete

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (ms_is_empty_list (&pQh->qtd_list))
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return u8Count;
  }

  u8State = pQh->qh_status;
  pQh->qh_status = QH_STS_COMPLETING;
  iStopped = (u8State == QH_STS_IDLE);

  list_for_loop_ex (entry, tmp, &pQh->qtd_list)
  {
    struct ehci_qtd  *pQtd;
    struct urb  *pUrb;
    U32    u32Token = 0;

    pQtd = entry_to_container (entry, struct ehci_qtd, qtd_list);
    //list_entry (entry, struct ehci_qtd,qtd_list, struct list_head,qtd);
    pUrb = pQtd->urb;

    /* obsolete, no such hardware bug */
    //if ( ((pQtd->hw_token >> 12) & 0x7) > 5)	//For patching H/W bug
    //{
    //    bIsPageOver = TRUE;
    //}

    /* free previous QTD ...*/
    if (pLast)
    {
      if (pLast->urb != pUrb)
      {
        ms_urb_done (pEhci, pLast->urb, pRegs);
        u8Count++;
      }
      ms_ehci_qtd_free (pEhci, pLast);
      pLast = 0;
    }

    if (pQtd == pEnd)
      break;

    /* HW implement qtds content copy */
    rmb ();

    u32Token = pQtd->hw_token;

 xacterr_retry:
    if ((u32Token & QTD_STS_ACT) == 0)
    {
      if ((u32Token & QTD_STS_HALT) != 0)
      {
        if ((u32Token & QTD_STS_XACTERR) &&
                QTD_CERR(u32Token) == 0 &&
                --pQh->xacterrs > 0 /*&&                !urb->unlinked*/) {
                u32Token &= ~QTD_STS_HALT;
                u32Token |= QTD_STS_ACT | (MAX_CERR_CNT << 10);
                pQtd->hw_token = u32Token;
                wmb();
                pQh->hw_token = u32Token;
                goto xacterr_retry;
        }
        iStopped = 1;
      }
      else if (IS_SHORT_READ (u32Token)
		/* patch from linux */
		&& !(pQtd->hw_alt_next_qtd & EHCI_LIST_END))
//	  && (pQh->hw_alt_next_qtd & QTD_MASK)
//	    == pEhci->stAsync->hw_alt_next_qtd)
      {
        iStopped = 1;
        goto halt;
      }
    }
    else if(!iStopped && HCD_IS_RUNNING (pEhci->hcd.state))
    {
      break;
    }
    else
    {
      iStopped = 1;

      if (pUrb->s32Status == -EINPROGRESS)
        continue;

      /* issue status for control transfer */
      if ((iDoStatus != 0)
          && QTD_PID (u32Token) == 0)   //Check is OUT token?
      {
        iDoStatus = 0;
        continue;
      }

      if (u8State == QH_STS_IDLE
          && pQtd->qtd_dma_addr
            == pQh->hw_current_qtd)
        u32Token = pQh->hw_token;

      if ((HALT_BIT & pQh->hw_token) == 0)
      {
halt:
        pQh->hw_token |= HALT_BIT;
        wmb ();
      }
    }


    /* remove it from the queue */
    osapi_spin_lock (&pUrb->lock);
    ms_qtd_copy_status (pUrb, pQtd->length, u32Token);
    /* obsolete, no such hardware bug */
    //if (bIsPageOver)
    //{
    //    ms_debug_warn("urb error because C_Page > 5\n");
    //    pUrb->s32Status = (QTD_PID (u32Token) == 1) /* Is IN token? */
    //    ? -MS_ERR_RECEIVE   /* HCD can't read data */
    //    : -MS_ERR_SEND;     /* HCD can't write data */
    //}
    iDoStatus = (pUrb->s32Status == -MS_ERR_REMOTEIO)
        && usb_pipecontrol (pUrb->u32Pipe);
    osapi_spin_unlock (&pUrb->lock);

    if (iStopped && pQtd->qtd_list.prev != &pQh->qtd_list) {
      pLast = entry_to_container (pQtd->qtd_list.prev, struct ehci_qtd, qtd_list);
      //list_entry (qtd->qtd_list.prev,struct ehci_qtd,qtd_list, struct list_head,last);
      pLast->hw_next_qtd = pQtd->hw_next_qtd;

    }
    ms_list_remove (&pQtd->qtd_list);
    pLast = pQtd;

    pQh->xacterrs = QH_MAX_XACTRTT_RETRY;
  }

  if (pLast != 0)
  {
    ms_urb_done (pEhci, pLast->urb, pRegs);
    u8Count++;
    ms_ehci_qtd_free (pEhci, pLast);
    pLast = 0;
  }

  pQh->qh_status = u8State;

  if ((iStopped != 0)
      /* avoid some old EHCI vendor will overlay dummy qtds */
      || pQh->hw_next_qtd == EHCI_LIST_END)
  {
#ifdef ENABLE_QH_REFRESH
	/* patch from linux 2.6.x ~ 3.8.x */
	if (u8State == QH_STS_IDLE)
		ms_qh_refresh(pEhci, pQh);
	else if (u8State == QH_STS_LINKED) {
		/* patch base on linux 2.6.x */
		if ((pQh->hw_ep_state2 & QH_SMASK) != 0) {
			ms_intr_deschedule(pEhci, pQh);
			ms_qh_schedule(pEhci, pQh);
		}
		else
			ms_unlink_async(pEhci, pQh);
	}
#else // replaced by linux code
    if (ms_is_empty_list (&pQh->qtd_list))
      pEnd = pQh->pDummyQtd;
    else
    {
      pEnd = entry_to_container (pQh->qtd_list.next,struct ehci_qtd, qtd_list);
      if (pEnd->qtd_dma_addr == pQh->hw_current_qtd)
        pEnd = 0;
    }
    if (pEnd)
      ms_qh_update_link (pQh, pEnd);
#endif
  }

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return u8Count;
}

/*-------------------------------------------------------------------------*/
// high bandwidth multiplier, as encoded in highspeed endpoint descriptors
#define hb_mult(wMaxPacketSize) (1 + (((wMaxPacketSize) >> 11) & 0x03))
// ... and packet size, for any kind of endpoint descriptor
#define max_packet(wMaxPacketSize) ((wMaxPacketSize) & 0x07ff)

static void ms_qtd_register_free (
  struct ehci_hcd    *pEhci,
  struct list_head  *qtd_list
)
{
  struct list_head  *pEntry, *pTemp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  list_for_loop_ex (pEntry, pTemp, qtd_list)
  {
    struct ehci_qtd  *pQtd;

    pQtd = entry_to_container (pEntry, struct ehci_qtd, qtd_list);
    ms_list_remove (&pQtd->qtd_list);
    ms_ehci_qtd_free (pEhci, pQtd);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*
  * @brief             prepare and allocate qtd_list
  *                       fill buffer to qtd from urb
  *
  * @param          struct ehci_hcd    *pEhci
  * @param          struct urb    *pUrb
  * @param          struct list_head  *pHead
  * @param          int      iFlags
  *
  * @return           struct list_head *
  */

static struct list_head *
ms_qh_urb_process (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *pHead,
  int      iFlags
)
{
  struct ehci_qtd    *pQtd, *pQtdPrev;
  dma_addr_t    tBuf;
  int      iLen, iMaxpkt;
  int      iIsInput;
  U32      u32Token;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
  if (!pQtd)
  {
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return 0;
  }
  ms_insert_list_before (&pQtd->qtd_list, pHead);
  pQtd->urb = pUrb;

  u32Token = QTD_STS_ACT;
  u32Token |= (MAX_CERR_CNT << 10);
  /* for split transactions, SplitXState initialized to zero */

  iLen = pUrb->u32TransferBufferLength;
  iIsInput = usb_pipein (pUrb->u32Pipe);
  if (usb_pipecontrol (pUrb->u32Pipe))
  {
    ms_qtd_pack (pQtd, pUrb->tSetupDma, sizeof (struct usb_ctrlrequest),
      u32Token | PID_SETUP, 8);

    /* ... and always at least one more pid */
    u32Token ^= QTD_TOGGLE;
    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = pUrb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, pHead);
  }

  /*
      * data stage : prepare buffer
      */

  if (iLen > 0)
    tBuf = pUrb->tTransferDma;
  else
    tBuf = 0;

  if (!tBuf || iIsInput)
    u32Token |= PID_IN;

  /* patch code from linux */
  iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
  //if (usb_pipebulk(pUrb->u32Pipe))
  //{
  //  switch (pUrb->dev->eSpeed)
  //  {
  //    case USB_FULL_SPEED:
  //      iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
  //      break;
  //    case USB_HIGH_SPEED:
  //    default:
  //      iMaxpkt = 512;
  //      break;
  //  }
  //}
  //else
  //{
  //  iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
  //}

  for (;;)
  {
    int this_qtd_len;

    this_qtd_len = ms_qtd_pack (pQtd, tBuf, iLen, u32Token, iMaxpkt);
    iLen -= this_qtd_len;
    tBuf += this_qtd_len;
    if (iIsInput)
      pQtd->hw_alt_next_qtd = pEhci->stAsync->hw_alt_next_qtd;

    /* for control transaction, maybe needs switch it */
    if ((iMaxpkt & (this_qtd_len + (iMaxpkt - 1))) == 0)
      u32Token ^= QTD_TOGGLE;

    if (iLen <= 0)
      break;

    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = pUrb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, pHead);
  }

  if ((pUrb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK) == 0
        || usb_pipecontrol (pUrb->u32Pipe))
    pQtd->hw_alt_next_qtd = EHCI_LIST_END;

  //if (tBuf != 0)
  if (pUrb->u32TransferBufferLength != 0)
  {
    int  one_more = 0;

    if (usb_pipecontrol (pUrb->u32Pipe))
    {
      one_more = 1;
      u32Token ^= 0x0100;      /* PID IN/OUT toggled  */
      u32Token |= QTD_TOGGLE;  /* set DATA1 */
    }
    //else if (usb_pipebulk (pUrb->u32Pipe)
    else if (usb_pipeout (pUrb->u32Pipe)
        && (pUrb->u32TransferFlags & MS_FLAG_URB_ZERO_PACKET)
        && !(pUrb->u32TransferBufferLength % iMaxpkt))
    {
      one_more = 1;
    }

    if (one_more)
    {
      pQtdPrev = pQtd;
      pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
      if (!pQtd)
        goto cleanup;
      pQtd->urb = pUrb;
      pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
      ms_insert_list_before (&pQtd->qtd_list, pHead);

      /* make NULL packet */
      ms_qtd_pack (pQtd, 0, 0, u32Token, 0);
    }
  }

  /* set interrupt enable when qtd completed */
  if (!(pUrb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
    pQtd->hw_token |= QTD_IOC;

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pHead;

cleanup:

  ms_qtd_register_free (pEhci, pHead);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/
/* Obsolete code */
//static __inline__ void
//ms_clear_toggle (struct usb_device *pUdev, int iEp, int is_out, struct ehci_qh *pQh)
//{
//  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
//  ms_debug_msg ("clear toggle dev%d iEp 0x%x-%s\n",
//        (int)pUdev->u32DevNum, iEp, is_out ? "out" : "in");
//  pQh->hw_token &= ~(QTD_TOGGLE);
//
//  usb_settoggle (pUdev, iEp, is_out, 1);
//  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
//}
/*
  * @brief             allocate and make qh
  *
  * @param          struct ehci_hcd    *pEhci
  * @param          struct urb    *pUrb
  * @param          int      iFlags
  *
  * @return           struct ehci_qh *
  */

static struct ehci_qh *
ms_qh_make (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  int      iFlags
)
{
  struct ehci_qh    *pQh = ms_ehci_qh_alloc (pEhci, iFlags);
  U32      u32Info1 = 0, u32Info2 = 0;
  int      iIsInput, iType;
  int      iMaxpkt = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (!pQh)
  {
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return pQh;
  }

  u32Info1 |= usb_pipeendpoint (pUrb->u32Pipe) << 8;
  u32Info1 |= usb_pipedevice (pUrb->u32Pipe) << 0;

  iIsInput = usb_pipein (pUrb->u32Pipe);
  iType = usb_pipetype (pUrb->u32Pipe);
  iMaxpkt = usb_maxpacket (pUrb->dev, pUrb->u32Pipe, !iIsInput);

  /* check if maximum packet size > 1024 bytes */
  if (max_packet(iMaxpkt) > 1024) {
    ms_debug_err("error maxp %d\n", max_packet(iMaxpkt));
    goto done;
  }

  if (iType == EP_INTERRUPT)
  {
    pQh->u8Usecs = NS_2_US(ms_usb_calc_bus_time (USB_HIGH_SPEED, iIsInput,
        hb_mult (iMaxpkt) * max_packet (iMaxpkt)));

    pQh->u16Start = NO_FRAME;

    if (pUrb->dev->eSpeed == USB_HIGH_SPEED)
    {
      pQh->c_usecs = 0;
      pQh->u8Gap_uf = 0;

      /* Warning, highspeed periods < 1 frame. */
      pQh->u16Period = pUrb->u32Interval >> 3;
      /* patch from linux, 2.6.28 and 3.10.86 different */
      //if (pQh->u16Period < 1)
      if (pQh->u16Period == 1 && pUrb->u32Interval != 1)
      {
        ms_debug_warn ("intr period %d uframes, NYET!", (int)pUrb->u32Interval);
        goto done;
      }
    }
    else
    {

      /* FS/LS transfer times */
      pQh->u8Gap_uf = 1 + ms_usb_calc_bus_time (pUrb->dev->eSpeed,
          iIsInput, iMaxpkt) / (125 * 1000);

      /* Warning, let SPLIT/CSPLIT times to close */
      if (iIsInput)
      {    // SPLIT, gap, CSPLIT+DATA
        pQh->c_usecs = pQh->u8Usecs + HIGHSPEED_US (0);
        pQh->u8Usecs = (U8)HIGHSPEED_US (1);
      }
      else
      {    // SPLIT+DATA, gap, CSPLIT
        pQh->u8Usecs += HIGHSPEED_US (1);
        pQh->c_usecs = (U8)HIGHSPEED_US (0);
      }
      pQh->u16Period = pUrb->u32Interval;

    }
  }

  /* patch from linux */
  pQh->dev = pUrb->dev;

/* merge code into the following switch case description */
#if 0
  if (pUrb->dev->eSpeed == USB_HIGH_SPEED)
  {
    u32Info1 |= (2 << 12);
  }
  else if (pUrb->dev->eSpeed == USB_LOW_SPEED)
  {
    u32Info1 |= (1 << 12);
  }
  else
  {
    if (pUrb->dev->eSpeed != USB_FULL_SPEED)
       ms_debug_warn ("unknow speed %d", pUrb->dev->eSpeed);
  }
#endif

  /* Is TT? */
  switch (pUrb->dev->eSpeed)
  {

  case USB_LOW_SPEED:
    //if (iType == EP_CONTROL)
    //{
    //  u32Info1 |= 8 << 16;  /* hard code maxpacket item */
    //}
    u32Info1 |= (1 << 12);  /* EPS is 1.5 Mb/s */

  case USB_FULL_SPEED:
    /* patch from linux, maxp applied */
    //if (iType == EP_CONTROL && (pUrb->dev->eSpeed == USB_FULL_SPEED) )
    //{
    //  //info1 |= 64 << 16;  // fixed maxpacket
    //  u32Info1 |= max_packet (iMaxpkt) << 16; // PIPE_INTERRUPT,PIPE_BULK,PIPE_ISOCHRONOUS
    //}
    //if (iType != EP_CONTROL)
    //{
    //  u32Info1 |= max_packet (iMaxpkt) << 16; // PIPE_INTERRUPT,PIPE_BULK,PIPE_ISOCHRONOUS
    //}
    u32Info1 |= iMaxpkt << 16;
    /* EPS 0 means "full" */
  //  info1 |= (3 << 12);  /* EPS "reserve" */
    if (iType != EP_INTERRUPT)
      u32Info1 |= (EHCI_RL_NAKCNT_TT << 28);
    if (iType == EP_CONTROL)
    {
      u32Info1 |= QH_C_BIT;  /* for TT */
      u32Info1 |= QH_DTC_BIT;    /* toggle from qtd */
    }
    u32Info2 |= (EHCI_HI_BW_MULT_TT << 30);

    /* linux code always sets ttport, but not to set hubaddr iff roothub */
    u32Info2 |= pUrb->dev->u32TTPort << 23; /* Port Number */
    if (pUrb->dev->tt && pUrb->dev->tt->hub->u32DevNum != 1) // Skip the Root Hub devnum == 1
    {
        u32Info2 |= pUrb->dev->tt->hub->u32DevNum << 16; /* Hub Addr */
        ms_debug_debug("[USB] set TT and not root hub, (ttport, hubaddr) = (%d,%d)\n", pUrb->dev->u32TTPort, pUrb->dev->tt->hub->u32DevNum);
    }
    //if (pUrb->dev->tt->hub->u32DevNum != 1)     // Skip the Root Hub devnum == 1
    //{
    //    u32Info2 |= pUrb->dev->u32TTPort << 23; /* Port Number */
    //    u32Info2 |= pUrb->dev->tt->hub->u32DevNum << 16; /* Hub Addr */
    //}
    /* NOTE:  if (EP_INTERRUPT) { scheduler sets c-mask } */
    break;
  case USB_HIGH_SPEED:      /* 與 TT 無關 */
    u32Info1 |= (2 << 12);  /* EPS is 480Mb/s */

    if (iType == EP_CONTROL)
    {
      u32Info1 |= (EHCI_RL_NAKCNT << 28);
      u32Info1 |= 64 << 16;  /* Endpoint (wMaxPacketSize) */
      u32Info1 |= QH_DTC_BIT;   /* Data toggle control */
      u32Info2 |= (EHCI_HI_BW_MULT_HS << 30);
    }
    else if (iType == EP_BULK)
    {
      u32Info1 |= (EHCI_RL_NAKCNT << 28);
      u32Info1 |= max_packet (iMaxpkt) << 16;  // Philips mp3 player endpoint descriptor bug
      u32Info2 |= (EHCI_HI_BW_MULT_HS << 30);
    }
    else
    {    /* Other transation type */
      u32Info1 |= max_packet (iMaxpkt) << 16;
      u32Info2 |= hb_mult (iMaxpkt) << 30;
    }
    break;
  default:
     ms_debug_msg ("usb dev %p speed %d", pUrb->dev, pUrb->dev->eSpeed);
done:
    ms_qh_put (pEhci, pQh);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return 0;
  }

  pQh->qh_status = QH_STS_IDLE;
  pQh->hw_ep_state1 = u32Info1;
  pQh->hw_ep_state2 = u32Info2;
  usb_settoggle (pUrb->dev, usb_pipeendpoint (pUrb->u32Pipe), !iIsInput, 1);
#ifdef ENABLE_QH_REFRESH
  /* patch from linux 2.6.x ~ 3.8.x */
  ms_qh_refresh(pEhci, pQh);
#else
  ms_qh_update_link (pQh, pQh->pDummyQtd); // replaced by ms_qh_refresh()
#endif
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pQh;
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          start async schedule
  *
  * @param          struct ehci_hcd    *pEhci
  * @param          struct ehci_qh  *pQh
  *
  * @return         none
  */

static void ms_qh_link_async (struct ehci_hcd *pEhci, struct ehci_qh  *pQh)
{
  U32    u32DmaAddr = QH_NEXT (pQh->qh_dma_addr);
  struct ehci_qh  *pHead;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pHead = pEhci->stAsync;
  ms_timer_action_done (pEhci, TIMER_ASYNC_OFF);

  if (!pHead->qh_next.qh)
  {
    U32  cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    //If disable => restart it
    if (!(cmd & USBCMD_ASE))
    {
      (void) ms_check_status (&pEhci->op_regs->usbsts, USBSTS_ASS, 0, 150);
      cmd |= USBCMD_ASE | USBCMD_RUN;         //asynchronous scheduler enable
      hcd_reg_writel (cmd, (U32)&pEhci->op_regs->usbcmd);
      pEhci->hcd.state = HCD_STATE_RUNNING;
    }
  }

  /* patch from linux code */
#ifdef ENABLE_QH_REFRESH
  /* clear toggle */
  if (pQh->qh_status == QH_STS_IDLE)
      ms_qh_refresh(pEhci, pQh);
#else
  /* Don't write QH memory if not necessary */
  if(pQh->hw_token & HALT_BIT)
    pQh->hw_token &= ~HALT_BIT;
#endif

  pQh->qh_next = pHead->qh_next;
  pQh->hw_next_qh = pHead->hw_next_qh;
  wmb ();

  pHead->qh_next.qh = pQh;
  pHead->hw_next_qh = u32DmaAddr;

  /* note: qh descriptor needs to flush into the memory */

  pQh->xacterrs = QH_MAX_XACTRTT_RETRY; // maximum xacterr retry count
  pQh->qh_status = QH_STS_LINKED;
  /* IRQ will report qtd completions event */
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/

#define  QH_ADDR_MASK  0x7f

static struct ehci_qh *ms_qh_append_tds (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *qtd_list,
  int      iEpNum,
  void      **ptr
)
{
  struct ehci_qh    *pQh = 0;
  //int maxp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQh = (struct ehci_qh *) *ptr;
  if (pQh == 0)
  {
    pQh = ms_qh_make (pEhci, pUrb, GFP_ATOMIC);
    *ptr = pQh;
  }

  if (pQh != 0)
  {
    struct ehci_qtd  *pQtd;

    if (ms_is_empty_list (qtd_list))
      pQtd = 0;
    else
    {
      pQtd = entry_to_container (qtd_list->next, struct ehci_qtd, qtd_list);
      //list_entry (qtd_list->next, struct ehci_qtd, qtd_list, struct list_head, qtd);
    }

    /* control qh may need patching after enumeration */
    if (iEpNum == 0)
    {
      /* ep0 maxpacket will be updated here after HCD issue get_descriptor. */
      /* the purpose like usb_ep0_reinit */
      if (!(pQh->hw_ep_state1 & (0x3 << 12)))
      {
        int maxp, hw_maxp;

        maxp = usb_maxpacket (pUrb->dev, pUrb->u32Pipe, !(usb_pipein (pUrb->u32Pipe)));
        hw_maxp = max_packet(pQh->hw_ep_state1 >> 16);
        if (maxp != hw_maxp)
            ms_debug_debug("[USB] hw_map(%x) <-> maxp(%x)\n", hw_maxp, maxp);

        if (maxp > hw_maxp)
        {
          U32  u32Info = pQh->hw_ep_state1;

          u32Info &= ~(0x07ff << 16);
          u32Info |= maxp << 16;
          pQh->hw_ep_state1 = u32Info;
        }
      }

      /* usb_reset_device() back the device address to 0 */
      if (usb_pipedevice (pUrb->u32Pipe) == 0)
          pQh->hw_ep_state1 &= ~QH_ADDR_MASK;
      /* modify device address, the purpose like usb_ep0_reinit */
      else if ((pQh->hw_ep_state1 & QH_ADDR_MASK) == 0) {
           ms_debug_debug("[USB] qh device addr 0, append urb device addr %d\n", usb_pipedevice (pUrb->u32Pipe));
           pQh->hw_ep_state1 |= usb_pipedevice (pUrb->u32Pipe);
      }
    }

    /* Obsolete code */
    //if ((!usb_gettoggle (pUrb->dev,
    //      (iEpNum & 0x0f), !(iEpNum & 0x10)))
    //    && !usb_pipecontrol (pUrb->u32Pipe))
    //{
    //  /* "never happens": drivers do stall cleanup right */
    //  if (pQh->qh_status != QH_STS_IDLE
    //        && !ms_is_empty_list (&pQh->qtd_list)
    //        && pQh->qh_status != QH_STS_COMPLETING)
    //      ms_debug_msg ("clear toggle dev%d ep%d%s: not idle\n", (int)usb_pipedevice (pUrb->u32Pipe), iEpNum & 0x0f,  usb_pipein (pUrb->u32Pipe) ? "in" : "out");
    //  ms_clear_toggle (pUrb->dev, iEpNum & 0x0f, !(iEpNum & 0x10), pQh);
    //  diag_printf("[USB] clear toggle called!!!\n");
    //}

    if (pQtd != 0)
    {
      struct ehci_qtd    *dummy;
      dma_addr_t    tDmaAddr;
      U32      pToken;

      pToken = pQtd->hw_token;
      pQtd->hw_token = HALT_BIT;
      wmb ();
      dummy = pQh->pDummyQtd;
      /* Copy qtd's data to dummy except dummy->qtd_dma */
      tDmaAddr = dummy->qtd_dma_addr;
      *dummy = *pQtd;
      dummy->qtd_dma_addr = tDmaAddr;

      ms_list_remove (&pQtd->qtd_list);
      ms_insert_list_after (&dummy->qtd_list, qtd_list);
      ms_list_join (qtd_list, pQh->qtd_list.prev);

      ms_ehci_qtd_init (pQtd, pQtd->qtd_dma_addr);
      pQh->pDummyQtd = pQtd;

      /* hc must see the new dummy at list end */
      tDmaAddr = pQtd->qtd_dma_addr;
      pQtd = entry_to_container (pQh->qtd_list.prev, struct ehci_qtd, qtd_list);
      //list_entry (qh->qtd_list.prev, struct ehci_qtd, qtd_list,struct list_head, qtd);
      //the last qtd of qh before spliceing qtd_list
      pQtd->hw_next_qtd = (U32)(tDmaAddr);

      /* let the hc process these next qtds */
      wmb ();
      dummy->hw_token = pToken;

      pUrb->hcpriv = ms_qh_get (pQh);
    }
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pQh;
}

/*-------------------------------------------------------------------------*/
static int
ms_submit_async (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *qtd_list
)
{
  //struct ehci_qtd    *pQtd;
  struct s_hcd_dev    *pDev;
  int      iEpNum;
  U32    u32Flags;
  struct ehci_qh    *pQh = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  /* only urb trace code needs qtd information */
  //pQtd = entry_to_container (qtd_list->next, struct ehci_qtd, qtd_list);
  //list_entry (qtd_list->next, struct ehci_qtd, qtd_list, struct list_head, qtd);
  pDev = (struct s_hcd_dev *)pUrb->dev->hcpriv;
  iEpNum = usb_pipeendpoint (pUrb->u32Pipe);
  if (usb_pipein (pUrb->u32Pipe) && !usb_pipecontrol (pUrb->u32Pipe))
    iEpNum |= 0x10;

  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  pQh = ms_qh_append_tds (pEhci, pUrb, qtd_list, iEpNum, &pDev->ep [iEpNum]);
  /* EP Control/bulk 經過 TT 不需要 scheduling */
  if (pQh != 0)
  {
    if (pQh->qh_status == QH_STS_IDLE)
      ms_qh_link_async (pEhci, ms_qh_get (pQh));
  }

  Chip_Flush_Memory();

  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  if (pQh == 0)
  {
      ms_qtd_register_free (pEhci, qtd_list);
      return -ENOMEM;
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/

static void ms_start_unlink_async (struct ehci_hcd *pEhci, struct ehci_qh *pQh);
static void ms_end_unlink_async (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
  struct ehci_qh    *pQh = pEhci->stReclaim;
  struct ehci_qh    *pNext;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if(pQh == NULL)
  {
     ms_debug_err("--[%s] reclaim NULL\n", __FUNCTION__);
     return;
  }

  //ms_timer_action_done (pEhci, TIMER_IAA_WATCHDOG);
  ms_iaa_watchdog_done(pEhci);

  Chip_Read_Memory(); // 20121218, 20121225

  pQh->qh_status = QH_STS_IDLE;
  pQh->qh_next.qh = 0;
  ms_qh_put (pEhci, pQh);      // pQh is directed to reclaim

  pNext = pQh->pReclaimQh;
  pEhci->stReclaim = pNext;
  //pEhci->iReclaimReady = 0;
  pQh->pReclaimQh = 0;

  ms_qh_completions (pEhci, pQh, pRegs);

  if (!ms_is_empty_list (&pQh->qtd_list)
      && HCD_IS_RUNNING (pEhci->hcd.state))
    ms_qh_link_async (pEhci, pQh);
  else
  {
    ms_qh_put (pEhci, pQh);    // pQh is directed from async list

    if (HCD_IS_RUNNING (pEhci->hcd.state)
        && pEhci->stAsync->qh_next.qh == 0)
      ms_timer_action (pEhci, TIMER_ASYNC_OFF);
  }

  if (pNext)
  {
    pEhci->stReclaim = 0;
    ms_start_unlink_async (pEhci, pNext);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

static void ms_start_unlink_async (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
  int    iUsbCmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
  struct ehci_qh  *pPrev;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
#ifdef DEBUG
  if (pEhci->stReclaim
      || (pQh->qh_status != QH_STS_LINKED
        && pQh->qh_status != QH_STS_UNLINK_WAIT)
// If support SMP, needs to check locked
//      || !spin_is_locked (&pEhci->tHcdLock)
      )
      USB_ASSERT(0, "BUG()!\n");
#endif

  if (pQh == pEhci->stAsync)
  {
    if (pEhci->hcd.state != HCD_STATE_HALT && !pEhci->stReclaim)
    {
      /* stop async schedule */
      hcd_reg_writel (iUsbCmd & ~USBCMD_ASE, (U32)&pEhci->op_regs->usbcmd);
      wmb ();
      // handshake later, if we need to
      ms_timer_action_done (pEhci, TIMER_ASYNC_OFF);
    }
    return;
  }

  pQh->qh_status = QH_STS_UNLINK;
  pEhci->stReclaim = pQh = ms_qh_get (pQh);

  pPrev = pEhci->stAsync;
  while (pPrev->qh_next.qh != pQh)
    pPrev = pPrev->qh_next.qh;

#ifdef _USB_ENABLE_BDMA_PATCH // BDMA SW patch
	if (get_64bit_OBF_cipher()) {
		m_BDMA_write(pQh->qh_dma_addr, pPrev->qh_dma_addr);
		USB_ASSERT(pPrev->hw_next_qh == pQh->hw_next_qh,
			"[BDMA] hw_next not matched!\n");
	}
	else
		pPrev->hw_next_qh = pQh->hw_next_qh;
#else
  pPrev->hw_next_qh = pQh->hw_next_qh;
#endif
  pPrev->qh_next = pQh->qh_next;
  wmb ();

  Chip_Flush_Memory(); // (20140827 to push the pipe)

  //if (pEhci->hcd.state == HCD_STATE_HALT)
  if (!HCD_IS_RUNNING(pEhci->hcd.state))
  {
    ms_end_unlink_async (pEhci, NULL);
    ms_debug_err("HCD not running, end unlink directly\n");
    return;
  }
  //inform HC thar something has been removed from asyn. schedule
  //pEhci->iReclaimReady = 0;
#if 1   // For bug 125MHz copy error (HALT)
  //if (!pEhci->uDontSendIAA) // 20130110 no need flag control
  {
    iUsbCmd |= USBCMD_IAAD;
    hcd_reg_writel (iUsbCmd, (U32)&pEhci->op_regs->usbcmd);
  }
#endif
  //ms_timer_action (pEhci, TIMER_IAA_WATCHDOG);
  ms_iaa_watchdog_begin(pEhci);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

static void ms_unlink_async (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
            if (!HCD_IS_RUNNING (pEhci->hcd.state) && pEhci->stReclaim)
                ms_end_unlink_async (pEhci, NULL);

            if (pQh->qh_status != QH_STS_LINKED)
                ;
            else if (pEhci->stReclaim)
            {
                struct ehci_qh    *pLast;

                for (pLast = pEhci->stReclaim;
                    pLast->pReclaimQh;
                    pLast = pLast->pReclaimQh)
                  continue;
                pQh->qh_status = QH_STS_UNLINK_WAIT;
                pLast->pReclaimQh = pQh;
                /* bypass IAA if the hc can't care */
                diag_printf("[EHCI] set QH_STS_UNLINK_WAIT\n");
            }
            else
                ms_start_unlink_async (pEhci, pQh);
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          free qtd list but kept qh for plug test shall cause some problems
  *
  * @param          struct ehci_hcd *pEhci
  * @param          struct stPtRegs *pRegs
  *
  * @return         none
  */

static void
ms_scan_async (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
  struct ehci_qh    *pQh;
  enum ehci_timer_event  eAction = TIMER_IO_WATCHDOG;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (!++(pEhci->u32Stamp))
      pEhci->u32Stamp++;
  ms_timer_action_done (pEhci, TIMER_ASYNC_SHRINK);
rescan:
  pQh = pEhci->stAsync->qh_next.qh;
  if (pQh != 0)
  {
    do
    {
      /* free qtd for this qh */
      if (!ms_is_empty_list (&pQh->qtd_list)
          && pQh->u32Stamp != pEhci->u32Stamp)
      {
        int temp;

        pQh = ms_qh_get (pQh);
        pQh->u32Stamp = pEhci->u32Stamp;
        temp = ms_qh_completions (pEhci, pQh, pRegs);
        ms_qh_put (pEhci, pQh);
        if (temp != 0)
        {
            goto rescan;
        }
      }

      //FIXME: #if 1 is correct procedure but it will cause some problems when do plug testing
      //Watchdog timer will be unlink for unknow reason ==> Therefore, qHD can not be
      //unlinked from async list. (qh->state == USB_STATE_LINKED but HCD_STATE_HALT)
      //So when disconnect attached device ep1in (Bulk in) cannot be released.
#if 1
      #if 0
      if (ms_is_empty_list (&pQh->qtd_list))
      {
        if (pQh->u32Stamp == pEhci->u32Stamp)
          eAction = TIMER_ASYNC_SHRINK;
        else if (!pEhci->stReclaim
              && pQh->qh_status == QH_STS_LINKED)
          ms_start_unlink_async (pEhci, pQh);
      }
      #else
      #define MS_QH_SHRINK_FRAMES 5
      if (ms_is_empty_list (&pQh->qtd_list) && (pQh->qh_status == QH_STS_LINKED))
      {
        if (!pEhci->stReclaim
              && ((pEhci->u32Stamp - pQh->u32Stamp) & 0x1fff) >= MS_QH_SHRINK_FRAMES * 8)
        {
          //diag_printf("[USB][%d] %s start_unlink sts1:%x\n",
          //                    jiffies, __func__, pQh->hw_ep_state1);
          ms_start_unlink_async (pEhci, pQh);
        }
        else
        {
          //diag_printf("[USB] TIMER_ASYNC_SHRINK qh_sts:0x%x\n", pQh->hw_ep_state1);
          eAction = TIMER_ASYNC_SHRINK;
        }
      }
      #endif
#else
      if (ms_is_empty_list (&pQh->qtd_list))
      {
        if (!pEhci->stReclaim && pQh->qh_status == QH_STS_LINKED)
          ms_start_unlink_async (pEhci, pQh);
      }
#endif
      pQh = pQh->qh_next.qh;
    } while (pQh);
  }
  if (eAction == TIMER_ASYNC_SHRINK)
    ms_timer_action (pEhci, TIMER_ASYNC_SHRINK);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

#ifdef EHCI_TD_DEBUG
void dbg_timeout_async(struct usb_hcd *hcd)
{
#if defined(CPU_TYPE_MIPS)
#define PA2VA(a) (0xA0000000+(a))
#elif defined(CPU_TYPE_ARM)
#define PA2VA(a) (0x20000000+(a))
#endif
#define HW_NEXT(a) ((a) & 0xffffffe0)
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	struct ehci_qh *qh;
	unsigned int *hw_p;
	unsigned int hw_next;

	/* print out async register */
	hw_next = hcd_reg_readl ((U32)&ehci->op_regs->asynclistaddr);
	diag_printf("[EHCI] async register %x\n", hw_next);
	/* print out hardware asynchronous qh link */
	hw_next = (U32)ehci->stAsync;
	diag_printf("--> %8x", hw_next);
	diag_printf("--> %8x", hw_next+0x80);
	diag_printf("--> %8x", hw_next+0x100);
	diag_printf("--> %8x\n", hw_next+0x180);

	hw_p = (unsigned int *)hw_next;
	diag_printf("--> %8x", *hw_p);
	hw_next += 0x80;
	hw_p = (unsigned int *)hw_next;
	diag_printf("--> %8x", *hw_p);
	hw_next += 0x80;
	hw_p = (unsigned int *)hw_next;
	diag_printf("--> %8x", *hw_p);
	hw_next += 0x80;
	hw_p = (unsigned int *)hw_next;
	diag_printf("--> %8x", *hw_p);
	diag_printf("\n");

	/* print out asynchronous qh list */
	qh = ehci->stAsync->qh_next.qh;
	if (qh == 0) {
		diag_printf("[EHCI] no QH list\n");
		return;
	}
	do {
		unsigned int c_qtd;
		int cqtd_n_list = 1;

		diag_printf("qh status %d, dummy_qtd %x\n",
			qh->qh_status, (unsigned int)qh->pDummyQtd);
		Dump_TD((unsigned int)qh, 1);
		c_qtd = (unsigned int)PA2VA(qh->hw_current_qtd);
		if (!ms_is_empty_list (&qh->qtd_list)) {
			struct list_head  *entry, *tmp;

			list_for_loop_ex (entry, tmp, &qh->qtd_list) {
				struct ehci_qtd *qtd;

				qtd = entry_to_container (entry, struct ehci_qtd, qtd_list);
				Dump_TD((unsigned int)qtd, 0);
				if (c_qtd == (unsigned int)qtd)
					cqtd_n_list = 0;
			}
		}
		if (cqtd_n_list) {
			diag_printf("    CURRENT QTD\n");
			Dump_TD((unsigned int)PA2VA(qh->hw_current_qtd), 0);
		}
		qh = qh->qh_next.qh;
	} while (qh);
}
#endif

#include "drvEHCI_SCHD.cxx"

/*-------------------------------------------------------------------------*/

static void ms_ehci_work(struct ehci_hcd *ehci, struct stPtRegs *regs);
static void ms_ehci_iaa_watchdog(U32 u32Param)
{
	struct ehci_hcd		*pEhci = (struct ehci_hcd *) u32Param;
	unsigned long		u32Flags;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
	osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);

	/* see Linux 2.6.28.9 ehci_iaa_watchdog description
	 */
	if (pEhci->stReclaim
			&& !ms_timer_pending(&pEhci->stiaa_Watchdog)
			&& HCD_IS_RUNNING(pEhci->hcd.state)) {
		U32 u32Cmd, u32Status;

		u32Cmd = hcd_reg_readl((U32)&pEhci->op_regs->usbcmd);
		if (u32Cmd & USBCMD_IAAD)
			hcd_reg_writel(u32Cmd & ~USBCMD_IAAD,
					(U32)&pEhci->op_regs->usbcmd);

		u32Status = hcd_reg_readl((U32)&pEhci->op_regs->usbsts);
		if ((u32Status & USBSTS_IAA) || !(u32Cmd & USBCMD_IAAD)) {
			INCREASE (pEhci->stats.u32LostIAA);
			ms_debug_err("IAA watchdog (lost IAA): status %x cmd %x\n",
				u32Status, u32Cmd);
			hcd_reg_writel(USBSTS_IAA, (U32)&pEhci->op_regs->usbsts);
		}

		ms_debug_func("IAA watchdog: status %x cmd %x\n",
				u32Status, u32Cmd);
		ms_end_unlink_async(pEhci, NULL);
	}

	osapi_spin_unlock_irqrestore(&pEhci->tHcdLock, u32Flags);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

static void ms_ehci_watchdog (U32 u32Param)
{
    struct ehci_hcd    *pEhci = (struct ehci_hcd *) u32Param;
    U32    u32Flags;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    ms_debug_func("[%s] action: 0x%08X\n", __FUNCTION__, (U32)pEhci->u32Actions);
    osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);

#if 0 // old reclaim flow
    /* avoid vt8235 lost IAA irqs */
    if (pEhci->stReclaim)
    {
        U32    status = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);

        if (status & USBSTS_IAA)
        {
            INCREASE(pEhci->stats.u32LostIAA);
            hcd_reg_writel (USBSTS_IAA, (U32)&pEhci->op_regs->usbsts);
            pEhci->iReclaimReady = 1;
        }
        else if ( (hcd_reg_readl((U32)&pEhci->op_regs->usbcmd)&USBCMD_IAAD) && !(status & USBSTS_IAA))
        {
            ms_debug_warn ("Something wrong ==>lost IAA\n");//HW ISSUE?
            pEhci->iReclaimReady = 1;
        }
    }
#endif

    /* stop async processing after it's idled a bit */
    //Periodic schedule issue was fixed, so reopen it
    if (ms_test_bit (TIMER_ASYNC_OFF, &pEhci->u32Actions))
        ms_start_unlink_async (pEhci, pEhci->stAsync);

    /* pEhci could run by timer, without IRQs ... */
    //if (ms_test_bit (TIMER_IO_WATCHDOG, &pEhci->u32Actions))
        ms_ehci_work(pEhci, NULL);

    osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);

}

int ms_ehci_init (struct usb_hcd *pHcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (pHcd);

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  osapi_spin_lock_init (&ehci->tHcdLock);

  ehci->u32MoreCSC = false;
  ehci->ehci_port_not_change_cnt = 0;

  ehci->cap_regs = (struct ehci_cap_regs *) pHcd->uhc_regs;
  ehci->op_regs = (struct ehci_op_regs *) ( (U32)pHcd->uhc_regs +
        (U32)hcd_reg_readb ((U32)&ehci->cap_regs->caplength));

  ehci->hcs_params = hcd_reg_readl ((U32)&ehci->cap_regs->hcsparams);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return ms_ehci_halt (ehci);
}

/*
  * @brief          start ehci controller
  *
  * @param          struct usb_hcd *hcd
  *
  * @return         error code
  */

extern void ms_hcd_poll_rh_status(struct usb_hcd *pHcd);
extern MS_U8 MDrv_SYS_GetChipRev(void);
int ms_ehci_begin (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  U32      u32Temp;
  struct usb_device  *pUdev;
  struct usb_bus    *pBus;
  int      iRetval;
  U32      u32HccParams;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  ehci->u32PeriodicSize = DEFAULT_I_TDPS;
  if ((iRetval = ms_ehci_mem_init (ehci, SLAB_KERNEL)) < 0)
    return iRetval;

  u32HccParams = hcd_reg_readl ((U32)&ehci->cap_regs->hcc_params);
  if (HCC_ISO_CACHE (u32HccParams))   // full frame cache
    ehci->u32IThresh = 8;
  else          // N microframes cached
    ehci->u32IThresh = 2 + HCC_ISO_THRES (u32HccParams);

  ehci->stReclaim = 0;
  ehci->iNextUframe = -1;

  if ((iRetval = ms_ehci_reset (ehci)) != 0)
  {
    ms_ehci_mem_cleanup (ehci);
    return iRetval;
  }

  hcd_reg_writel (INTR_MASK, (U32)&ehci->op_regs->usbintr);
  u32Temp=hcd_reg_readl((U32)&ehci->op_regs->bus_control);
 // temp|=INT_POLAR+HALF_SPEED;
  u32Temp|=INT_POLAR;
  u32Temp&=~VBUS_OFF;
  hcd_reg_writel(u32Temp,(U32)&ehci->op_regs->bus_control);       //set intr high active
  hcd_reg_writel (ehci->tPeriodicDma, (U32)&ehci->op_regs->periodiclistbase);

  ehci->stAsync->qh_next.qh = 0;
  ehci->stAsync->hw_next_qh = QH_NEXT (ehci->stAsync->qh_dma_addr);
  ehci->stAsync->hw_ep_state1 = QH_H_BIT;
  ehci->stAsync->hw_token = QTD_STS_HALT;
  ehci->stAsync->hw_next_qtd = EHCI_LIST_END;
  ehci->stAsync->qh_status = QH_STS_LINKED;
  ehci->stAsync->hw_alt_next_qtd = (U32)(ehci->stAsync->pDummyQtd->qtd_dma_addr);
  ms_debug_msg("qh dma addr: %p\n", (void *)ehci->stAsync->qh_dma_addr);
  hcd_reg_writel ((U32)ehci->stAsync->qh_dma_addr, (U32)&ehci->op_regs->asynclistaddr);

  if (HCC_64BIT_ADDR_CAP (u32HccParams))
  {
    hcd_reg_writel (0, (U32)&ehci->op_regs->ctrldssegment);
  }

  u32Temp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd) & 0x0fff;
  if (log2_irq_thresh < 0 || log2_irq_thresh > 6)
    log2_irq_thresh = 0;
  u32Temp |= 1 << (16 + log2_irq_thresh);

#if defined(CHIP_K2)
  // K2 U02 & Newer IC
  if (MDrv_SYS_GetChipRev() >= 0x01)
  {
    park_eco = 3;
  }
#endif

  if (park_eco && HCC_ASPC(u32HccParams))
  {
    ms_debug_msg("[EHCI] Enable Park Mode: %d\n", (int)park_eco);
    u32Temp |= USBCMD_PARK;
    u32Temp |= park_eco << 8;
  }

  if (HCC_PFLF (u32HccParams))
  {
    u32Temp &= ~(3 << 2);
    u32Temp |= (EHCI_PERIODIC_FLS << 2);
    switch (EHCI_PERIODIC_FLS)
    {
    case 0: ehci->u32PeriodicSize = 1024; break;
    case 1: ehci->u32PeriodicSize = 512; break;
    case 2: ehci->u32PeriodicSize = 256; break;
    default:  USB_ASSERT(0, "Not alllowed periodic frame list size\n");
    }
  }
  u32Temp &= ~(USBCMD_IAAD | USBCMD_ASE | USBCMD_PSE),

  // Only make HC run when device connects to bus
  u32Temp |= USBCMD_RUN;
  hcd_reg_writel (u32Temp, (U32)&ehci->op_regs->usbcmd);

  /* initialize watchdog timer function */

  ms_init_timer (&ehci->stWatchdog);
  ehci->stWatchdog.function = ms_ehci_watchdog;
  ehci->stWatchdog.data = (U32) ehci;

  ms_init_timer (&ehci->stiaa_Watchdog);
  ehci->stiaa_Watchdog.function = ms_ehci_iaa_watchdog;
  ehci->stiaa_Watchdog.data = (U32) ehci;

  pBus = (struct usb_bus *)(&hcd->self);
  pBus->root_hub = pUdev = ms_usb_alloc_dev (NULL, pBus);
  if (!pUdev)
  {
done2:
    ms_ehci_mem_cleanup (ehci);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return -ENOMEM;
  }

  ehci->hcd.state = HCD_STATE_RUNNING;
  u32Temp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted write */

  u32Temp = hcd_reg_readw ((U32)&ehci->cap_regs->hciversion);
  ms_debug_msg ("EHCI %x.%02x, driver %s\n",
    (unsigned int)u32Temp >> 8, (unsigned int)u32Temp & 0xff, DRIVER_VERSION);

  pUdev->eSpeed = USB_HIGH_SPEED;
  if (ms_register_root_hub(hcd) != 0)
  {
    if (hcd->state == HCD_STATE_RUNNING)
      ms_ehci_quiesce(ehci);
    ms_ehci_reset (ehci);
    pBus->root_hub = 0;
    ms_usb_put_dev (pUdev);
    iRetval = -ENODEV;
    goto done2;
  }

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

 /*
    * @brief             stop ehci controller
    *
    * @param          struct usb_hcd *hcd
    *
    * @return           none
    */

 void ms_ehci_end (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);

  ms_debug_func("[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  ms_del_timer_sync (&ehci->stWatchdog);
  ms_del_timer_sync (&ehci->stiaa_Watchdog);

  osapi_spin_lock_irq (&ehci->tHcdLock);
  if (hcd->state == HCD_STATE_RUNNING)
    ms_ehci_quiesce(ehci);
  // need to turn off all ports?
  ms_ehci_reset (ehci);
  //osapi_spin_unlock_irq (&ehci->tHcdLock);

  //osapi_spin_lock_irq (&ehci->tHcdLock);
  if (ehci->stAsync) // patch from Linux 3.1.10
    ms_ehci_work(ehci, NULL);
  osapi_spin_unlock_irq (&ehci->tHcdLock);
  ms_ehci_mem_cleanup (ehci);

#ifdef  EHCI_STATS
  ms_debug_msg ("[EHCI DEBUG] irq normal %ld err %ld reclaim %ld (lost %ld) ",
    ehci->stats.u32Normal, ehci->stats.u32Error, ehci->stats.u32Reclaim,
    ehci->stats.u32LostIAA);
  ms_debug_msg ("complete %ld unlink %ld\n",
    ehci->stats.u32Complete, ehci->stats.u32Unlink);
#endif

}

 int ms_ehci_get_frame_idx (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  ms_debug_func("[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return (hcd_reg_readl ((U32)&ehci->op_regs->frindex) >> 3) % ehci->u32PeriodicSize;
}

/*-------------------------------------------------------------------------*/

 int ms_ehci_suspend (struct usb_hcd *hcd, U32 state)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  int      iPorts;
  int      i;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  iPorts = HCS_N_PORTS (ehci->hcs_params);

  if (hcd->state == HCD_STATE_RUNNING)
    ms_ehci_quiesce(ehci);
  ms_debug_msg("ms_ehci_suspend\n");
  hcd_reg_writel(hcd_reg_readl ((U32)&ehci->op_regs->usbcmd) & ~USBCMD_RUN, (U32)&ehci->op_regs->usbcmd);
  while((hcd_reg_readl((U32)&ehci->op_regs->usbsts)&USBSTS_HALT) == 0);

  /* suspend each port, then stop the hc */
  for (i = 0; i < iPorts; i++) {
    int  temp = hcd_reg_readl ((U32)&ehci->op_regs->portsc [i]);
    if ((temp & PORTSC_PE) == 0
        /* || (temp & PORTSC_OWNER) != 0 */)
      continue;
    ms_debug_msg ("suspend port %d", i);
    temp |= PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

 int ms_ehci_resume (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  int  iPorts;
  int  i;
  //U32  U32Tmp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  iPorts = HCS_N_PORTS (ehci->hcs_params);
  hcd->state = HCD_STATE_RUNNING;
  for (i = 0; i < iPorts; i++)
  {
    int  temp = hcd_reg_readl ((U32)&ehci->op_regs->portsc [i]);
    if ((temp & PORTSC_PE) == 0
        || (temp & PORTSC_SUSPEND) != 0)
      continue;
    ms_debug_msg ("resume port %d", i);
    temp |= PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
    /*U32Tmp = */hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted writes */
    wait_ms (20);
    temp &= ~PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
  }
  /*U32Tmp = */hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted writes */
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          scan asynchronous qh chain and periodic frame list
  *
  * @param          struct ehci_hcd *pEhci
  * @param          struct stPtRegs *regs
  *
  * @return         none
  */
static void ms_ehci_work(struct ehci_hcd *pEhci, struct stPtRegs *regs)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  ms_timer_action_done (pEhci, TIMER_IO_WATCHDOG);
  Chip_Read_Memory(); // 20121218, 20121225

#if 0 // old reclaim flow
  // This sequence will release Host task befor USB DSR complete.
  if (pEhci->iReclaimReady)
  {
    ms_end_unlink_async (pEhci, regs);
    ms_scan_async (pEhci, regs);
    //ms_end_unlink_async (pEhci, regs); // move 2 lines ahead for IAA issue. 20140408
  }
#endif

  if (pEhci->iScanning)
    return;
  pEhci->iScanning = 1;
  ms_scan_async (pEhci, regs);
  if (pEhci->iNextUframe != -1)
    ms_scan_periodic (pEhci, regs);
  pEhci->iScanning = 0;

  if (HCD_IS_RUNNING(pEhci->hcd.state) &&
    ((pEhci->stAsync->qh_next.ptr != 0) || (pEhci->u32PeriodicSched != 0)))
    ms_timer_action (pEhci, TIMER_IO_WATCHDOG);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/
/*
  * @brief          process ehci controller interrupt events
  *
  * @param          struct usb_hcd *pHcd
  * @param          struct stPtRegs *pRegs
  *
  * @return         none
  */
void ms_ehci_irq (struct usb_hcd *pHcd, struct stPtRegs *pRegs)
{
  struct ehci_hcd    *pEhci= hcd_to_ehci (pHcd);
  U32  u32Status, int_status;
  int  bh;
  U32 u32Cmd;

  osapi_spin_lock (&pEhci->tHcdLock);

  int_status = u32Status = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);

  if (u32Status == ~(U32) 0)
  {
    ms_debug_msg ("reg usbsts is 0xffff, device removed\n");
    goto dead;
  }

  u32Status &= INTR_MASK;
  if (!u32Status)      /* No any our interrupt event */
  {
    diag_printf("ms_ehci_irq status: %x, intr_enable: %x\n", (unsigned int)int_status, hcd_reg_readw((U32)&pEhci->op_regs->usbintr));
    goto done;
  }

  hcd_reg_writel (u32Status, (U32)&pEhci->op_regs->usbsts);
  u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
  bh = 0;
  //pEhci->uDontSendIAA = 0;

  /* Notify controller for advance async schedule */
  /* USBSTS_INT for normal completion, see 4.15.1.2
         USBSTS_ERR for error completion, see 4.15.1.1 */
  if ((u32Status & (USBSTS_INT|USBSTS_ERR)) != 0)
  {
#if 0 // old reclaim flow
    // Enable the interrupt for Async Advance Enable
    pEhci->iReclaimReady = 0;
    int    cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    cmd |= USBCMD_IAAD;
    hcd_reg_writel (cmd, (U32)&pEhci->op_regs->usbcmd);
    (void) hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    //pEhci->uDontSendIAA = 1;
#endif
    if ((u32Status & USBSTS_ERR) == 0)
    {
      INCREASE(pEhci->stats.u32Normal);
    }
    else
    {
      INCREASE(pEhci->stats.u32Error);
      //bh = 1;	//Note, remove for command stall
    }
    bh = 1;
  }

  /* IAA is used for complete the unlinking of qh */
  if (u32Status & USBSTS_IAA)
  {
#if 0 // old reclaim flow
    INCREASE(pEhci->stats.u32Reclaim);
    pEhci->iReclaimReady = 1;
    //pEhci->uDontSendIAA = 1;
    bh = 1;
#endif
    if (u32Cmd & USBCMD_IAAD)
    {
        hcd_reg_writel (u32Cmd & ~USBCMD_IAAD, (U32)&pEhci->op_regs->usbcmd);
        ms_debug_err("[EHCI] IAAD not clear?\n");
    }
    if (pEhci->stReclaim)
    {
        INCREASE(pEhci->stats.u32Reclaim);
        ms_end_unlink_async(pEhci, pRegs);
    }
    else
        ms_debug_err("reclaim but with nothing there!\n");
  }

  /* See [4.3.1] */
  if (u32Status & USBSTS_PCD)
  {
      // Only support one port
      int iPsc/*, masked_psc*/;
      int isHalt = 0;

      iPsc = hcd_reg_readl((U32)&pEhci->op_regs->portsc [0]);

      /* new patch, root hub not run */
      if (iPsc == pHcd->saved_ehci_state.iPsc &&
            pEhci->u32ResetEnd[0] == pHcd->saved_ehci_state.reset_done &&
            u32Cmd == pHcd->saved_ehci_state.usbcmd &&
            (u32Cmd & USBCMD_RUN) == 0)
        {
                ++pEhci->ehci_port_not_change_cnt;
                if (pEhci->ehci_port_not_change_cnt > 7)
                    isHalt = 1;
        }
      else
        pEhci->ehci_port_not_change_cnt = 0;
      pHcd->saved_ehci_state.iPsc = iPsc;
      pHcd->saved_ehci_state.reset_done = pEhci->u32ResetEnd[0];
      pHcd->saved_ehci_state.usbcmd = u32Cmd;
      if (isHalt)
      {
        diag_printf("[EHCI] PCD not clear, reset UHC to get new CURRENT usbsts\n");
        if(iPsc & PORTSC_CSC)
        {
          diag_printf("[EHCI] set more CSC\n");
          pEhci->u32MoreCSC = true;
        }
        ms_ehci_softrst(pEhci);
        goto done;
      }

      // root hub not run
#if 0 // NUSED
      if ((u32Cmd & USBCMD_RUN) == 0)
      {
          //u32Cmd |= USBCMD_RUN;
          //hcd_reg_writel(u32Cmd, (U32)&pEhci->op_regs->usbcmd);
          //while(hcd_reg_readw ((U32)&pEhci->op_regs->usbsts) & USBSTS_HALT);
          isHalt = 1;
          //hcd_reg_writel (u32Status, (U32)&pEhci->op_regs->usbsts);
          //diag_printf("clear USBSTS_PCD (0x%x) again\n", hcd_reg_readw ((U32)&pEhci->op_regs->usbsts));
      }

      masked_psc = iPsc & 0xf;
      //if ((masked_psc == 0xa /*|| masked_psc == 0xf */) && pEhci->u32ResetEnd[0] == 0)
      if ((masked_psc == 0xb))
      {
          if (isHalt)
          {
            //ms_ehci_softrst(pEhci);
            //diag_printf("PCD not clear, reset UHC to get new CURRENT usbsts = 0x%x\n", hcd_reg_readl ((U32)&pEhci->op_regs->usbsts));
          }
      }
#endif

      /* Clear port enable bit when root device disconnect */
      /* Patch for hub+device hot plug frequency then lost disconnect event */
      if ((iPsc & PORTSC_CSC) && (iPsc & PORTSC_CONNECT)==0)
      {
            iPsc &= ~(PORTSC_PEC | PORTSC_PE | PORTSC_CSC);
            hcd_reg_writel(iPsc, (U32)&pEhci->op_regs->portsc[0]);
            //diag_printf("<irq> clear PORTSC_PE\n");
      }
      #if 0 // un-patch since not to support suspend
      if ((((iPsc & PORTSC_RESUME) ||
           !(iPsc & PORTSC_SUSPEND)) &&
            (iPsc & PORTSC_PE) &&
             pEhci->u32ResetEnd[0] == 0))
      {
          /* The root hub port active 20 msec resume signal,
                    * PORTSC_SUSPEND to stop resume signal.
                    */
          pEhci->u32ResetEnd [0] = jiffies + ((20 /* msec */ * HZ) / 1000);
          ms_debug_msg ("port 1 remote wakeup\n");
          ms_update_timer(&pHcd->roothub_timer, pEhci->u32ResetEnd[0], 0);
      }
      #endif
  }


  /* See [4.15.2.4] */

  if ((u32Status & USBSTS_FATAL) != 0)
  {
     ms_debug_err ("[EHCI] fatal error\n");
dead:
     //ms_ehci_reset (pEhci);
     ms_ehci_softrst(pEhci);
     /* uses ms_ehci_end to clean up the rest */
     bh = 1;
  }

  if (bh ||
    (pEhci->u32PeriodicSched != 0) //If we has periodic transactions in Schedule, we must scan periodic when USBSTS_INT
    )
    ms_ehci_work(pEhci, pRegs);
done:
  osapi_spin_unlock (&pEhci->tHcdLock);
  if (u32Status & USBSTS_PCD)
      ms_hcd_poll_rh_status(pHcd);

}

/*-------------------------------------------------------------------------*/
/*
  * @brief          transforms urb into qh+qtd list and submit to HCD hardware
  *
  * @param          struct usb_hcd  *pHcd
  * @param          struct urb  *pUrb
  * @param          int  iMem_flags
  *
  * @return         error code
  */
 int ms_urb_enqueue (
  struct usb_hcd  *pHcd,
  struct urb  *pUrb,
  int    iMem_flags
)
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  struct list_head  qtd_list;
  int iRetVal;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
  if( (hcd_reg_readl ((U32)&pEhci->op_regs->portsc[0])&PORTSC_CONNECT) == 0 )
  {
    iRetVal = -ENODEV;
    goto urb_enqueue_fail;
  }
#endif
  ms_list_init (&qtd_list);

  switch (usb_pipetype (pUrb->u32Pipe))
  {
    case EP_CONTROL:
    case EP_BULK:
        if (!ms_qh_urb_process (pEhci, pUrb, &qtd_list, iMem_flags))
        {
            iRetVal = -ENOMEM;
            goto urb_enqueue_fail;
        }
        iRetVal = ms_submit_async (pEhci, pUrb, &qtd_list);
        break;

    case EP_INTERRUPT:
        if (!ms_qh_urb_process (pEhci, pUrb, &qtd_list, iMem_flags))
        {
            iRetVal = -ENOMEM;
            goto urb_enqueue_fail;
        }
        iRetVal = ms_intr_submit (pEhci, pUrb, &qtd_list);
        break;

    default:
        ms_debug_msg ("no support the transaction type!!\n");
        iRetVal = -ENOSYS;
  }

urb_enqueue_fail:
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return iRetVal;
}

/*
  * @brief          remove qh from HW content
  *                 will happen a  completion event
  *
  * @param          struct usb_hcd *pHcd
  * @param          struct urb *pUrb
  *
  * @return         error code
  */
 int ms_urb_dequeue (struct usb_hcd *pHcd, struct urb *pUrb)
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  struct ehci_qh    *pQh = (struct ehci_qh *) pUrb->hcpriv;
  U32    u32Flags;
  int iStatus = 0;

  if (!pQh)
      return 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  switch (usb_pipetype (pUrb->u32Pipe))
  {
  /*
      // case EP_CONTROL:
      // case EP_BULK:
        */
  default:
    /* patch the old flow */
    switch (pQh->qh_status) {
        case QH_STS_LINKED:
        case QH_STS_COMPLETING:
#ifdef ENABLE_DISCONNECT_FAST_RESPONSE
            /* remove (20150422) */
            /* do fast unlink if HCD halt */
            //if ( (hcd_reg_readw ((U32)&pEhci->op_regs->usbcmd)&USBCMD_RUN) == 0 )
            //    pEhci->hcd.state = HCD_STATE_HALT;
#endif
            /* base on linux 2.6.28, 3.1.10 */
            /*** unlink_async ***/
            ms_unlink_async(pEhci, pQh);
            break;
        case QH_STS_UNLINK:
        case QH_STS_UNLINK_WAIT:
            /* had started */
            break;
        case QH_STS_IDLE:
            diag_printf("[USB] dequeue idle qh\n");
            break;
    }
    #if 0 // renewed old flow
    /* if we need to use IAA and it's busy, defer */
    if (pQh->qh_status == QH_STS_LINKED
        && pEhci->stReclaim
        && HCD_IS_RUNNING (pEhci->hcd.state)
        )
    {
      struct ehci_qh    *pLast;

      for (pLast = pEhci->stReclaim;
          pLast->pReclaimQh;
          pLast = pLast->pReclaimQh)
        continue;
      pQh->qh_status = QH_STS_UNLINK_WAIT;
      pLast->pReclaimQh = pQh;
      /* bypass IAA if the hc can't care */
    }
    else if (!HCD_IS_RUNNING (pEhci->hcd.state) && pEhci->stReclaim)
      ms_end_unlink_async (pEhci, NULL);

    /* something else might have unlinked the qh by now */
    if (pQh->qh_status == QH_STS_LINKED)
      ms_start_unlink_async (pEhci, pQh);
    #endif
    //FIXME: cover FOTG200 bug
    // NUSED, not proper hardware patch
    //if( (hcd_reg_readw ((U32)&pEhci->op_regs->portsc[0])&PORTSC_CONNECT) == 0 )
    //{
    //  diag_printf("[urb_dequeue] connect bit = 0 \n");
    //  //Finish ullinking procedure
    //  ms_end_unlink_async (pEhci, NULL);
    //  //Reset HC to ensure FOTG200 work correctly at the next time
    //  ms_hub_port_disable(pEhci->hcd.self.root_hub,0);
    //}
    //else if ( (hcd_reg_readw ((U32)&pEhci->op_regs->usbcmd)&USBCMD_RUN) == 0 )
    //{
    //  diag_printf("Something wrong (script auto running?) ==> Check it out\n");
    //  //Finish ullinking procedure
    //  ms_end_unlink_async (pEhci, NULL);
    //  //Reset HC to ensure FOTG200 work correctly at the next time
    //  ms_hub_port_disable(pEhci->hcd.self.root_hub,0);
    //}
    break;
  case EP_INTERRUPT:
    /* patch from Linux 2.6.28 */
    switch (pQh->qh_status)
    {
        case QH_STS_LINKED:
        //case QH_STS_COMPLETING:
            ms_intr_deschedule (pEhci, pQh);
            /* revise to fall through */
            //break;
        case QH_STS_IDLE:
            ms_qh_completions (pEhci, pQh, NULL);
            break;
        default:
            ms_debug_err("bogus qh %p state %d\n",
                    pQh, pQh->qh_status);
            goto done;
    }
    break;
    #if 0 // renewed old one
    if (pQh->qh_status == QH_STS_LINKED)
    {
      /* messy, can spin or block a microframe ... */
      ms_intr_deschedule (pEhci, pQh, 1);
      /* qh_state == IDLE */
    }
    ms_qh_completions (pEhci, pQh, NULL);
    #endif

    /* reschedule QH iff another request is queued */
    if (!ms_is_empty_list (&pQh->qtd_list)
        && HCD_IS_RUNNING (pEhci->hcd.state))
    {
      //int iStatus;

      iStatus = ms_qh_schedule (pEhci, pQh);
      //osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
      if (iStatus != 0)
      {
        // Warning happened
        ms_debug_err ("can't reschedule pQh %p, err %d", pQh, iStatus);
      }
      //ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      //return iStatus;
    }
    break;
  }
done:
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return iStatus;
}

/*-------------------------------------------------------------------------*/

/*
  * @brief          wait qh unlink complete then free qtd list.
  *
  * @param          struct usb_hcd *pHcd
  * @param          struct s_hcd_dev *pDev
  * @param          int iEp
  *
  * @return         none
  */

void
ms_ehci_disable_ep (struct usb_hcd *pHcd, struct s_hcd_dev *pDev, int iEp)
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  int    iEpnum;
  U32    u32Flags;
  struct ehci_qh    *pQh, *tmp;
  int numRescan = 100;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  iEpnum = iEp & USB_ENDPOINT_NUMBER_MASK;
  if (iEpnum != 0 && (iEp & USB_DIR_IN))
    iEpnum |= 0x10;

rescan:
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  pQh = (struct ehci_qh *) pDev->ep[iEpnum];
  if (!pQh)
    goto done;

  if (!HCD_IS_RUNNING (pEhci->hcd.state))
    pQh->qh_status = QH_STS_IDLE;
  switch (pQh->qh_status)
  {
      case QH_STS_LINKED:
        for (tmp = pEhci->stAsync->qh_next.qh;
          tmp && tmp != pQh; tmp = tmp->qh_next.qh)
            continue;
        if (!tmp)
            goto bad;
        /* base on linux 2.6.28, 3.1.10 */
        /*** unlink_async ***/
        ms_unlink_async(pEhci, pQh);
        //break;
        /* to disable endpoint, unlink continued */
      case QH_STS_UNLINK:
      case QH_STS_UNLINK_WAIT:
        osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
        osapi_schedule_timeout (1);
        if (--numRescan)
            goto rescan;
        diag_printf("[UM-DEP] rescan 100 times, qh state %d, HCD state %x\n", pQh->qh_status, pEhci->hcd.state);
        osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
      case QH_STS_IDLE:
        if (ms_is_empty_list (&pQh->qtd_list))
        {
          ms_qh_put (pEhci, pQh);
          break;
        }
      default:
bad:
        //if ( !ms_is_empty_list (&pQh->qtd_list))
        {
            ms_debug_err ("<%s> pQh %p (ep#%d) state %d%s\n",
                __FUNCTION__, pQh, iEpnum, pQh->qh_status,
                ms_is_empty_list (&pQh->qtd_list) ? "" : "(has tds)");
        }
        break;
  }
  if (numRescan)
        pDev->ep[iEpnum] = 0;
done:
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return;
}

/*-------------------------------------------------------------------------*/

void ms_ehci_stoprun_setting(MS_U8 bOption, struct ehci_hcd *ehci)
{
    U32 temp;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (bOption==HOST20_Enable)
    {
        temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        if (temp & USBCMD_RUN)
        {
            ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
            return;
        }
        temp |= USBCMD_RUN;
        hcd_reg_writel(temp, (U32)&ehci->op_regs->usbcmd);
        do
        {
            temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        } while ((temp & USBCMD_RUN) == 0);
        ehci->hcd.state = HCD_STATE_RUNNING;
    }
    else if (bOption==HOST20_Disable)
    {
        temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        if ((temp & USBCMD_RUN) == 0)
            return;

      temp &= ~USBCMD_RUN;
      hcd_reg_writel(temp, (U32)&ehci->op_regs->usbcmd);
      do {
          temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
      } while ((temp & USBCMD_RUN) > 0);
      ehci->hcd.state = HCD_STATE_HALT;
    }
    else
      USB_ASSERT(0, "??? Input Error 'ms_ehci_stoprun_setting'...");

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

void ms_qh_ehci_reinit(struct usb_hcd *pHcd, int not_fix)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);
    U32    u32Temp;
    int is_empty_qtd = 0;

    if( pEhci->stAsync->qh_next.qh != 0 )
    {
        ms_debug_err("[reinit] MAY Not only cantain one qHD in asyn. scheduler (%p)\n", pEhci->stAsync);
        ms_debug_err("[reinit] async hw_next_qh %x\n", pEhci->stAsync->hw_next_qh);
#if 0
        struct ehci_qh *pQh = pEhci->stAsync->qh_next.qh;
        while(pQh) {
          ms_debug_err("qh %p\n=============\n", pQh);
          ms_debug_err("[UM] status %d\n", pQh->qh_status);
          ms_debug_err("[UM] hw ep %d\n", (pQh->hw_ep_state1 >> 8) & 0xf);
          ms_debug_err("[UM] qtd_list %s\n", !ms_is_empty_list (&pQh->qtd_list) ? "there": "none");
          ms_debug_err("[UM] refcnt %d\n", pQh->tRefCnt);
          if (ms_is_empty_list (&pQh->qtd_list))
            is_empty_qtd = 1;
          pQh = pQh->qh_next.qh;
        }
        ms_debug_err("=============\n");
#endif
    }

    if (not_fix || is_empty_qtd)
        return;

    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [0]);
    u32Temp = u32Temp & ~PORTSC_RWC_BITS;
    hcd_reg_writel (u32Temp & ~PORTSC_PE, (U32)&pEhci->op_regs->portsc [0]);
    //裝置被移除的時候 停止scheduler
    hcd_reg_writel(hcd_reg_readl((U32)&pEhci->op_regs->usbcmd)&(~(USBCMD_ASE|USBCMD_PSE|USBCMD_IAAD)),(U32)&pEhci->op_regs->usbcmd);
    /* recover periodic size setting at STR off/on  */
    u32Temp = hcd_reg_readl((U32)&pEhci->op_regs->usbcmd);
    u32Temp &= ~(3 << 2);
    u32Temp |= (EHCI_PERIODIC_FLS << 2);
    hcd_reg_writel(u32Temp,(U32)&pEhci->op_regs->usbcmd);

    if( pEhci->stAsync->qh_next.qh != 0 )
    {
        struct ehci_qh *pQh = pEhci->stAsync->qh_next.qh;

        do {
          struct ehci_qh *qh_rm;

          qh_rm = pQh;
          pQh = pQh->qh_next.qh;
          qh_rm->qh_next.qh = NULL;
          qh_rm->tRefCnt = 1; // forced to be 1
          ms_qh_put(pEhci, qh_rm);
        } while(pQh);

        /* re-init head QH */
        pEhci->stAsync->qh_next.qh = NULL;
        pEhci->stAsync->hw_next_qh = QH_NEXT (pEhci->stAsync->qh_dma_addr);
        pEhci->stAsync->hw_ep_state1 = QH_H_BIT;
        pEhci->stAsync->hw_token = QTD_STS_HALT;
        pEhci->stAsync->hw_next_qtd = EHCI_LIST_END;
        pEhci->stAsync->qh_status = QH_STS_LINKED;
        pEhci->stAsync->hw_alt_next_qtd = (U32)(pEhci->stAsync->pDummyQtd->qtd_dma_addr);
    }
}

#ifdef USB_SYSTEM_STR_SUPPORT
void ms_ehci_periodic_size_init (struct usb_hcd *hcd)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (hcd);
    U32    u32Temp;

    /* recover periodic size setting at STR off/on  */
    u32Temp = hcd_reg_readl((U32)&pEhci->op_regs->usbcmd);
    u32Temp &= ~(3 << 2);
    u32Temp |= (EHCI_PERIODIC_FLS << 2);
    hcd_reg_writel(u32Temp,(U32)&pEhci->op_regs->usbcmd);
}
#endif

#if USB_IF_EHSET_SUPPORT // Embedded host electrical test procedure
struct list_head *
ms_qh_urb_transaction_EHSET (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  struct list_head  *head,
  int      iFlags,
  int      iStage
)
{
    struct ehci_qtd    *pQtd, *pQtdPrev;
    dma_addr_t    tBufAddr;
    int      iLen, iMaxpkt;
    int      iIsInput;
    U32      u32Token;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
    if (!pQtd)
    {
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return 0;
    }
    ms_insert_list_before (&pQtd->qtd_list, head);
    pQtd->urb = urb;

    u32Token = QTD_STS_ACT;
    u32Token |= (MAX_CERR_CNT << 10);

    iLen = urb->u32TransferBufferLength;
    iIsInput = usb_pipein (urb->u32Pipe);
    if (!iStage)
    {
        if (usb_pipecontrol (urb->u32Pipe))
        {
          ms_qtd_pack (pQtd, urb->tSetupDma, sizeof (struct usb_ctrlrequest),
          u32Token | (2 /* "setup" */ << 8), 8);
          pQtd->hw_alt_next_qtd = EHCI_LIST_END; // EHSET
          u32Token ^= QTD_TOGGLE;
        }

        if (!(urb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
          pQtd->hw_token |= QTD_IOC; // EHSET
        return head;
    }
    else
        u32Token ^= QTD_TOGGLE;

  tBufAddr = (iLen > 0) ? urb->tTransferDma : 0;

  if (!tBufAddr || iIsInput)
    u32Token |= (1 /* "in" */ << 8);
  if (usb_pipebulk(urb->u32Pipe))
  {
    switch (urb->dev->eSpeed)
    {
      case USB_FULL_SPEED:
        iMaxpkt = max_packet(usb_maxpacket(urb->dev, urb->u32Pipe, !iIsInput));
        break;
      case USB_HIGH_SPEED:
      default:
        iMaxpkt = 512;
        break;
    }
  }
  else
  {
    iMaxpkt = max_packet(usb_maxpacket(urb->dev, urb->u32Pipe, !iIsInput));
  }

  for (;;)
  {
    int iThisQtdLen;

    iThisQtdLen = ms_qtd_pack (pQtd, tBufAddr, iLen, u32Token, iMaxpkt);
    iLen -= iThisQtdLen;
    tBufAddr += iThisQtdLen;
    if (iIsInput)
      pQtd->hw_alt_next_qtd = ehci->stAsync->hw_alt_next_qtd;

    if ((iMaxpkt & (iThisQtdLen + (iMaxpkt - 1))) == 0)
      u32Token ^= QTD_TOGGLE;

    if (iLen <= 0)
      break;

    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = urb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, head);
  }

  if ((urb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK) == 0
        || usb_pipecontrol (urb->u32Pipe))
    pQtd->hw_alt_next_qtd = EHCI_LIST_END;

  if (tBufAddr != 0)
  {
    int  iOneMore = 0;

    if (usb_pipecontrol (urb->u32Pipe))
    {
      iOneMore = 1;
      u32Token ^= 0x0100;  /* "in" <--> "out"  */
      u32Token |= QTD_TOGGLE;  /* force DATA1 */
    }
    else if (usb_pipebulk (urb->u32Pipe)
        && (urb->u32TransferFlags & MS_FLAG_URB_ZERO_PACKET)
        && !(urb->u32TransferBufferLength % iMaxpkt)) {
      iOneMore = 1;
    }
    if (iOneMore)
    {
      pQtdPrev = pQtd;
      pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
      if (!pQtd)
        goto cleanup;
      pQtd->urb = urb;
      pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
      ms_insert_list_before (&pQtd->qtd_list, head);

      ms_qtd_pack (pQtd, 0, 0, u32Token, 0);
    }
  }

  if (!(urb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
    pQtd->hw_token |= QTD_IOC;

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return head;

cleanup:

  ms_qtd_register_free (ehci, head);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

inline int ms_submit_async_EHSET (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  struct list_head  *qtd_list,
  int      mem_flags
) {
    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return ms_submit_async(ehci, urb, qtd_list);
}
#endif
