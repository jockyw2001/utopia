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
///////////////////////////////////////////////////////////////////////////////////////////////////
/// File name: usbentry.c
/// @brief USB host driver Entry function.
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
//#include <MsCommon.h> // NUSED

#include "include/drvConfig.h"
//#include "include/drvPorts.h" // NUSED
#include "include/drvKernel.h"
//#include "include/drvTimer.h" // NUSED
//#include "include/drvCPE_EHCI.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED

//#include "drvUsbd.h" // NUSED
#include "drvMassStor.h"
//#include "drvUSB.h" // NUSED
#include "drvUSBHwCtl.h"
//#include "drvEHCI.h" // NUSED
#if USB_MASS_STORAGE_SUPPORT
#include "drvMSC.h"
#endif
#if USB_HID_SUPPORT
#include "drvHIDGlue.h"
#endif
#if USB_CDC_SUPPORT
#include "drvCDCDev.h"
#endif
/* applying drvUsbHostConfig.h (inside drvUSBHwCtl.h) */

struct s_ChipUsbHostDef *pCurrentChip;
struct s_ChipUsbHostDef chipDESC;

mem_Alloc    pfnAllocCachedMem=NULL, pfnAllocNoncachedMem=NULL;
mem_Free     pfnFreeCachedMem=NULL, pfnFreeNoncachedMem=NULL;
mem_VA2PA    pfnVA2PA=NULL;
mem_PA2VA    pfnPA2VA=NULL;
mem_Cached2Noncached     pfnCached2Noncached=NULL;
mem_NonCached2Cached     pfnNoncached2Cached=NULL;

MS_S32 _s32UsbEventId = -1;
MS_U8 gUsbChipID = 0xFF;

USBCallback _DrvUSBC_CBFun = NULL;

USBCallback _DrvUSB_CBFun = NULL;

void ms_ehci_irq (struct usb_hcd *pHcd, struct stPtRegs *pRegs);

static MS_U8 u8HubStackBuffer[HUB_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
/// Register a callback function to process the usb plug event
/// @param  pCallbackFn \b IN: callback function used in interrupt context.
/// @return None
/// @note   call after USB has been initialized
///         The last registered callback will overwrite the previous ones.
//-------------------------------------------------------------------------------------------------
void MDrv_USB_RegisterCallBack (USBCallback pCallbackFn)
{
    //Register a callback function for application to process the data received
    _DrvUSB_CBFun = pCallbackFn;
}

MS_U8 ms_USBGetChipID(void)
{
#if defined(USB_LIB_CHIPID)
    return USB_LIB_CHIPID;
#else
    if (gUsbChipID == 0xFF)
        gUsbChipID = usb_readb(OS_BASE_ADDR+0x1ecc*2);

    //diag_printf("GetChipID: 0x%x\n", gUsbChipID);
    return gUsbChipID;
#endif
}

extern MS_U8 MDrv_SYS_GetChipRev(void);
void ms_U4_series_usb_init(struct s_ChipUsbHostDef *pChip, MS_U8 u8Hostid)
{
    unsigned int UTMI_base = pChip->reg[u8Hostid].baseUTMI;
    unsigned int USBC_base = pChip->reg[u8Hostid].baseUSBC;
    unsigned int UHC_base = pChip->reg[u8Hostid].baseUHC;
    unsigned int USBBC_base = pChip->reg[u8Hostid].baseUSBBC;
    unsigned int flag = pChip->reg[u8Hostid].iFlag;

    MS_U8 chipID = pChip->chipID;

    diag_printf("[USB] constant upll NLib, UTMI base %p, USBC base %p, UHC base %p, USBBC base %p\n",
        (void *)UTMI_base, (void *)USBC_base, (void *)UHC_base, (void *)USBBC_base);

    if (flag & EHCFLAG_TESTPKG)
    {
        usb_writew(0x2084, (void*) (UTMI_base+0x2*2));
        usb_writew(0x0003, (void*) (UTMI_base+0x20*2));
    }

#if _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH
    /*
    * patch for DM always keep high issue
    * init overwrite register
    */
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) & (MS_U8)(~BIT3), (void*) (UTMI_base+0x0*2)); //DP_PUEN = 0
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) & (MS_U8)(~BIT4), (void*) (UTMI_base+0x0*2)); //DM_PUEN = 0

    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) & (MS_U8)(~BIT5), (void*) (UTMI_base+0x0*2)); //R_PUMODE = 0

    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT6, (void*) (UTMI_base+0x0*2)); //R_DP_PDEN = 1
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT7, (void*) (UTMI_base+0x0*2)); //R_DM_PDEN = 1

    usb_writeb(usb_readb((void*)(UTMI_base+0x10*2)) | BIT6, (void*) (UTMI_base+0x10*2)); //hs_txser_en_cb = 1
    usb_writeb(usb_readb((void*)(UTMI_base+0x10*2)) & (MS_U8)(~BIT7), (void*) (UTMI_base+0x10*2)); //hs_se0_cb = 0

    /* turn on overwrite mode */
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT1, (void*) (UTMI_base+0x0*2)); //tern_ov = 1
#endif

    // disable battery charger function
    if (flag & EHCFLAG_USBBC_OFF)
    {
        usb_writeb(usb_readb((void*)(USBBC_base+0x03*2-1)) & ~0x40, (void*) (USBBC_base+0x03*2-1));
        usb_writeb(usb_readb((void*)(USBBC_base+0x0c*2)) & ~0x40, (void*) (USBBC_base+0x0c*2));
        usb_writeb(usb_readb((void*)(UTMI_base+0x01*2-1)) & ~0x40, (void*) (UTMI_base+0x01*2-1));
    }

    usb_writeb(0x0a, (void*) (USBC_base)); // Disable MAC initial suspend, Reset UHC
    usb_writeb(0x28, (void*) (USBC_base)); // Release UHC reset, enable UHC and OTG XIU function

#if 0 // don't touch UPLL setting again. set UPLL only in boot code.
    if ((chipID == CHIPID_KRONUS) || (chipID == CHIPID_KAISERIN))
    {
        U16 reg_t;

        usb_writeb(usb_readb((void*)(UTMI_base+0x0D*2-1)) | 0x01, (void*) (UTMI_base+0x0D*2-1)); // set reg_double_data_rate, To get better jitter performance

        reg_t = usb_readw(UTMI_base+0x22*2);
        if ((reg_t & 0x10e0) != 0x10e0)
            usb_writew(0x10e0, (void*) (UTMI_base+0x22*2));
        reg_t = usb_readw(UTMI_base+0x24*2);
        if (reg_t != 0x1)
            usb_writew(0x1, (void*) (UTMI_base+0x24*2));
        //usb_writeb(usb_readb((void*)(UTMI_base+0x22*2)) | 0xE0, (void*) (UTMI_base+0x22*2)); // Set PLL_TEST[23:21] for enable 480MHz clock
        //usb_writeb(usb_readb((void*)(UTMI_base+0x20*2)) | 0x02, (void*) (UTMI_base+0x20*2)); // Set PLL_TEST[1] for PLL multiplier 20X
	    //usb_writeb(0,    (void*) (UTMI_base+0x21*2-1));
	    //usb_writeb(0x10, (void*) (UTMI_base+0x23*2-1));
	    //usb_writeb(0x01, (void*) (UTMI_base+0x24*2));
    }
#endif

    if (flag & EHCFLAG_DOUBLE_DATARATE)
    {
        if ((flag & EHCFLAG_DDR_MASK) == EHCFLAG_DDR_x15)
        {
            usb_writeb(usb_readb((void*)(UTMI_base+0x20*2)) | 0x76, (void*) (UTMI_base+0x20*2)); // Set usb bus = 480MHz x 1.5
        }
        else if ((flag & EHCFLAG_DDR_MASK) == EHCFLAG_DDR_x18)
        {
            usb_writeb(usb_readb((void*)(UTMI_base+0x20*2)) | 0x8e, (void*) (UTMI_base+0x20*2)); // Set usb bus = 480MHz x 1.8
        }
        //else if ((flag & EHCFLAG_DDR_MASK) == EHCFLAG_DDR_x20)
        //{
        //    usb_writeb(usb_readb((void*)(UTMI_base+0xD*2-1)) | 0x01, (void*) (UTMI_base+0xD*2-1)); // Set usb bus = 480MHz x2
        //}

        usb_writeb(usb_readb((void*)(UTMI_base+0x2c*2)) |0x1, (void*) (UTMI_base+0x2c*2));  //Set slew rate control for overspeed (or 960MHz)
    }

    usb_writeb(usb_readb((void*)(UTMI_base+0x09*2-1)) & ~0x08, (void*) (UTMI_base+0x09*2-1)); // Disable force_pll_on
    usb_writeb(usb_readb((void*)(UTMI_base+0x08*2)) & ~0x80, (void*) (UTMI_base+0x08*2)); // Enable band-gap current
    usb_writeb(0xC3, (void*)(UTMI_base)); // reg_pdn: bit<15>, bit <2> ref_pdn
    mdelay(1); // delay 1ms

    usb_writeb(0x69, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>
    mdelay(2); // delay 2ms

    usb_writeb(0x01, (void*) (UTMI_base)); // Turn all (including hs_current) use override mode
    usb_writeb(0, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>

    usb_writeb(usb_readb((void*)(UTMI_base+0x3C*2)) | 0x01, (void*) (UTMI_base+0x3C*2)); // set CA_START as 1
    mdelay(1); // 10 -> 1

    usb_writeb(usb_readb((void*)(UTMI_base+0x3C*2)) & ~0x01, (void*) (UTMI_base+0x3C*2)); // release CA_START

    while ((usb_readb((void*)(UTMI_base+0x3C*2)) & 0x02) == 0); // polling bit <1> (CA_END)

    if (flag & EHCFLAG_DPDM_SWAP)
        usb_writeb(usb_readb((void*)(UTMI_base+0x0b*2-1)) |0x20, (void*) (UTMI_base+0x0b*2-1)); // dp dm swap

    usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) & ~0x03, (void*) (USBC_base+0x02*2)); //UHC select enable
    usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) | 0x01, (void*) (USBC_base+0x02*2)); //UHC select enable

    usb_writeb(usb_readb((void*)(UHC_base+0x40*2)) & ~0x10, (void*) (UHC_base+0x40*2)); //0: VBUS On.
    mdelay(1); // delay 1ms

    usb_writeb(usb_readb((void*)(UHC_base+0x40*2)) | 0x08, (void*) (UHC_base+0x40*2)); // Active HIGH
    //mdelay(1); // NUSED

    usb_writeb(usb_readb((void*)(UHC_base+0x81*2-1)) | 0x8F, (void*) (UHC_base+0x81*2-1)); //improve the efficiency of USB access MIU when system is busy

    if ((chipID == CHIPID_KRONUS) || (chipID == CHIPID_URANUS4))
    usb_writeb(usb_readb((void*)(UHC_base+0x83*2-1)) | 0x40, (void*) (UHC_base+0x83*2-1)); //set MIU1_sel to 128MB
    // Kaiserin will keep the defaut "00" to be as 512MB support

    usb_writeb((usb_readb((void*)(UTMI_base+0x06*2)) & 0x9F) | 0x40, (void*) (UTMI_base+0x06*2)); //reg_tx_force_hs_current_enable

    usb_writeb(usb_readb((void*)(UTMI_base+0x03*2-1)) | 0x28, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select
    usb_writeb(usb_readb((void*)(UTMI_base+0x03*2-1)) & 0xef, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select

    usb_writeb(usb_readb((void*)(UTMI_base+0x07*2-1)) & 0xfd, (void*) (UTMI_base+0x07*2-1)); //Disable improved CDR
#ifdef ENABLE_UTMI_240_AS_120_PHASE_ECO
    usb_writeb(usb_readb((void*)(UTMI_base+0x08*2)) | 0x08, (void*) (UTMI_base+0x08*2));
#endif
#if _USB_CLOCK_PHASE_ADJ_PATCH
    if ((chipID == CHIPID_KERES) && (MDrv_SYS_GetChipRev() == 0x0))
	usb_writeb(usb_readb((void*)(UTMI_base+0x08*2)) & ~0x08, (void*) (UTMI_base+0x08*2));
#endif

    usb_writeb(usb_readb((void*)(UTMI_base+0x09*2-1)) |0x81, (void*) (UTMI_base+0x09*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
#if _USB_CLOCK_PHASE_ADJ_PATCH
    if ((chipID == CHIPID_KERES) && (MDrv_SYS_GetChipRev() == 0x0))
	usb_writeb(usb_readb((void*)(UTMI_base+0x0b*2-1)) & ~0x80, (void*) (UTMI_base+0x0b*2-1));
#else
    if ((flag & EHCFLAG_DOUBLE_DATARATE)==0)
        usb_writeb(usb_readb((void*)(UTMI_base+0x0b*2-1)) |0x80, (void*) (UTMI_base+0x0b*2-1)); // TX timing select latch path
#endif
    usb_writeb(usb_readb((void*)(UTMI_base+0x15*2-1)) |0x20, (void*) (UTMI_base+0x15*2-1)); // Chirp signal source select
#ifdef ENABLE_UTMI_55_INTERFACE
    usb_writeb(usb_readb((void*)(UTMI_base+0x15*2-1)) |0x40, (void*) (UTMI_base+0x15*2-1)); // Change to 55 interface
#endif

    usb_writeb( UTMI_EYE_SETTING_2C, (void*) (UTMI_base+0x2c*2));
    usb_writeb( UTMI_EYE_SETTING_2D, (void*) (UTMI_base+0x2d*2-1));
    usb_writeb( UTMI_EYE_SETTING_2E, (void*) (UTMI_base+0x2e*2));
    usb_writeb( UTMI_EYE_SETTING_2F, (void*) (UTMI_base+0x2f*2-1));

    /* Add hardware ECO items here */
    // Kaiserin U02  patch code (K2S only)
    if ((chipID == CHIPID_KAISERIN) && (MDrv_SYS_GetChipRev() >= 0x01)) // && U02 and Newer IC
    {
        diag_printf("K2S software patch!!!\n");

        // enable LS cross point ECO
        usb_writeb(usb_readb((void*)(UTMI_base+0x39*2-1)) | 0x04, (void*) (UTMI_base+0x39*2-1));  //enable deglitch SE0¡¨(low-speed cross point)

        // enable power noise ECO
        usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) | 0x40, (void*) (USBC_base+0x02*2)); //enable use eof2 to reset state machine¡¨ (power noise)

        // enable TX/RX reset clock gating ECO
        usb_writeb(usb_readb((void*)(UTMI_base+0x39*2-1)) | 0x02, (void*) (UTMI_base+0x39*2-1)); //enable hw auto deassert sw reset(tx/rx reset)

        // enable loss short packet interrupt ECO, default 0 on
        //usb_writeb(usb_readb((void*)(USBC_base+0x04*2)) & 0x7f, (void*) (USBC_base+0x04*2)); //enable patch for the assertion of interrupt(Lose short packet interrupt)

        // enable babble ECO
        usb_writeb(usb_readb((void*)(USBC_base+0x04*2)) | 0x40, (void*) (USBC_base+0x04*2)); //enable add patch to Period_EOF1(babble problem)

        // enable MDATA single TT ECO
        usb_writeb(usb_readb((void*)(USBC_base+0x0A*2)) | 0x40, (void*) (USBC_base+0x0A*2)); //enable short packet MDATA in Split transaction clears ACT bit (LS dev under a HS hub)
    }

    // Kaiser and Newer IC patch code
    if ((chipID == CHIPID_KAISER))
    {
        diag_printf("Applied hardware ECO patch!!!\n");

        // enable LS cross point ECO (manually all)
        usb_writeb(usb_readb((void*)(UTMI_base+0x04*2)) | 0x40, (void*) (UTMI_base+0x04*2));  //enable deglitch SE0¡¨(low-speed cross point)

        // enable power noise ECO (manually all)
        usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) | 0x40, (void*) (USBC_base+0x02*2)); //enable use eof2 to reset state machine¡¨ (power noise)

        // enable TX/RX reset clock gating ECO (manually all)
        usb_writeb(usb_readb((void*)(UTMI_base+0x04*2)) | 0x20, (void*) (UTMI_base+0x04*2)); //enable hw auto deassert sw reset(tx/rx reset)

        // enable loss short packet interrupt ECO, default 0 on
        //usb_writeb(usb_readb((void*)(USBC_base+0x04*2)) & 0x7f, (void*) (USBC_base+0x04*2)); //enable patch for the assertion of interrupt(Lose short packet interrupt)

        // enable babble ECO, default on
        //usb_writeb(usb_readb((void*)(USBC_base+0x04*2)) | 0x40, (void*) (USBC_base+0x04*2)); //enable add patch to Period_EOF1(babble problem)

        // enable MDATA single TT ECO, default on
        //writeb(readb((void*)(USBC_base+0x0A*2)) | 0x40, (void*) (USBC_base+0x0A*2)); //enable short packet MDATA in Split transaction clears ACT bit (LS dev under a HS hub)

        // enable DM always high ECO
        usb_writeb(usb_readb((void*)(UTMI_base+0x10*2)) | 0x40, (void*) (UTMI_base+0x10*2)); // monkey test
    }

    // enable miu new bridge ECO
#if defined(ENABLE_PV2MI_BRIDGE_ECO)
    usb_writeb(usb_readb((void*)(USBC_base+0x0a*2)) | 0x40, (void*) (USBC_base+0xa*2)); //fix pv2mi bridge mis-behavior
#endif

#if _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH
    /*
     * patch for DM always keep high issue
     * init overwrite register
     */
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT6, (void*) (UTMI_base+0x0*2)); //R_DP_PDEN = 1
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT7, (void*) (UTMI_base+0x0*2)); //R_DM_PDEN = 1

    /* turn on overwrite mode */
    usb_writeb(usb_readb((void*)(UTMI_base+0x0*2)) | BIT1, (void*) (UTMI_base+0x0*2)); //tern_ov = 1
#endif

#if _USB_MINI_PV2MI_BURST_SIZE
    usb_writeb(usb_readb((void*)(USBC_base+0x0b*2-1)) & ~(BIT1|BIT2|BIT3|BIT4), (void*)(USBC_base+0x0b*2-1));
#endif

#ifdef ENABLE_HS_CONNECTION_FAIL_INTO_VFALL_ECO
    usb_writeb(usb_readb((void*)(USBC_base+0x11*2-1)) | BIT1, (void*)(USBC_base+0x11*2-1));
#endif

#if _USB_MIU_WRITE_WAIT_LAST_DONE_Z_PATCH
    /* Enabe PVCI i_miwcplt wait for mi2uh_last_done_z */
    usb_writeb(usb_readb((void*)(UHC_base+0x83*2-1)) | BIT4, (void*)(UHC_base+0x83*2-1));
#endif

#ifdef ENABLE_HS_DISCONNECTION_DEBOUNCE_ECO
    usb_writeb(usb_readb((void*)(USBC_base+0x1*2-1)) | BIT4, (void*)(USBC_base+0x1*2-1));
#endif

#if defined(ENABLE_UHC_PREAMBLE_ECO)
    /* [7]: reg_etron_en, to enable utmi Preamble function */
    usb_writeb(usb_readb((void*)(UTMI_base+0x3f*2-1)) | BIT7, (void*)(UTMI_base+0x3f*2-1));

    /* [3:]: reg_preamble_en, to enable Faraday Preamble */
    usb_writeb(usb_readb((void*)(USBC_base+0x0f*2-1)) | BIT3, (void*)(USBC_base+0x0f*2-1));

    /* [0]: reg_preamble_babble_fix, to patch Babble occurs in Preamble */
    usb_writeb(usb_readb((void*)(USBC_base+0x10*2)) | BIT0, (void*)(USBC_base+0x10*2));

    /* [1]: reg_preamble_fs_within_pre_en, to patch FS crash problem */
    usb_writeb(usb_readb((void*)(USBC_base+0x10*2)) | BIT1, (void*)(USBC_base+0x10*2));

    /* [2]: reg_fl_sel_override, to override utmi to have FS drive strength */
    usb_writeb(usb_readb((void*)(UTMI_base+0x03*2-1)) | BIT2, (void*)(UTMI_base+0x03*2-1));
#endif

#if defined(ENABLE_HS_ISO_IN_CORNER_ECO)
    usb_writeb(usb_readb((void*)(USBC_base+0x13*2-1)) | BIT0, (void*)(USBC_base+0x13*2-1));
#endif

#if defined(ENABLE_UHC_RUN_BIT_ALWAYS_ON_ECO)
    /* Don't close RUN bit when device disconnect */
    usb_writeb(usb_readb((void*)(UHC_base+0x34*2)) | BIT7, (void*)(UHC_base+0x34*2));
#endif

#if defined(ENABLE_DISCONNECT_PE_CLR_ECO)
    /*Enable Port is disabled when device is dosconnected ECO*/
    usb_writeb(usb_readb((void*)(USBC_base+0x12*2)) | BIT7, (void*) (USBC_base+0x12*2));
#endif

#if !defined(_EHC_SINGLE_SOF_TO_CHK_DISCONN)
    /* Use 2 SOFs to check disconnection */
    usb_writeb((usb_readb((void*)(USBC_base+0x03*2-1)) & BIT7) | (0x02<<1 | BIT0), (void*)(USBC_base+0x03*2-1));
#endif

#if defined(ENABLE_DISCONNECT_SPEED_REPORT_RESET_ECO)
    /* UHC speed type report should be reset by device disconnection */
    usb_writeb(usb_readb((void*)(USBC_base+0x20*2)) | BIT0, (void*)(USBC_base+0x20*2));
#endif

#if defined(ENABLE_BABBLE_PCD_ONE_PULSE_TRIGGER_ECO)
    /* Port Change Detect (PCD) is triggered by babble.
      * Pulse trigger will not hang this condition.
      */
    usb_writeb(usb_readb((void*)(USBC_base+0x20*2)) | BIT1, (void*)(USBC_base+0x20*2));
#endif

#if defined(ENABLE_HC_RESET_FAIL_ECO)
    /* generation of hhc_reset_u */
    usb_writeb(usb_readb((void*)(USBC_base+0x20*2)) | BIT2, (void*)(USBC_base+0x20*2));
#endif

#if defined(ENABLE_SRAM_CLK_GATING_ECO)
    /* do SRAM clock gating automatically to save power */
    usb_writeb(usb_readb((void*)(USBC_base+0x20*2)) & (U8)(~BIT4), (void*)(USBC_base+0x20*2));
#endif

    if (flag & EHCFLAG_TESTPKG)
    {
        usb_writew(0x0210, (void*) (UTMI_base+0x2C*2)); //
        usb_writew(0x8100, (void*) (UTMI_base+0x2E*2)); //

        usb_writew(0x0600, (void*) (UTMI_base+0x14*2)); //
        usb_writew(0x0038, (void*) (UTMI_base+0x10*2)); //
        usb_writew(0x0BFE, (void*) (UTMI_base+0x32*2)); //
    }
}

void MDrv_Usb_Init(
    mem_Alloc     pfn_Cachedmem_Alloc,
    mem_Free      pfn_Cachedmem_Free,
    mem_Alloc     pfn_NonCachedmem_Alloc,
    mem_Free      pfn_NonCachedmem_Free,
    mem_VA2PA     pfn_mem_VA2PA,
    mem_PA2VA     pfn_mem_PA2VA,
    mem_Cached2Noncached pfn_mem_Cached2Noncached,
    mem_NonCached2Cached pfn_mem_NonCached2Cached
)
{
    diag_printf("USB initial, %d port supported\n", NUM_OF_ROOT_HUB);

    pfnAllocCachedMem = pfn_Cachedmem_Alloc;
    pfnFreeCachedMem = pfn_Cachedmem_Free;
    pfnAllocNoncachedMem = pfn_NonCachedmem_Alloc;
    pfnFreeNoncachedMem = pfn_NonCachedmem_Free;
    pfnVA2PA = pfn_mem_VA2PA;
    pfnPA2VA = pfn_mem_PA2VA;
    pfnCached2Noncached = pfn_mem_Cached2Noncached;
    pfnNoncached2Cached = pfn_mem_NonCached2Cached;

    ms_init_sys();

    USB_ASSERT(-1 == _s32UsbEventId, "USB event ID not valid!\n");
    _s32UsbEventId = MsOS_CreateEventGroup("UHC_Event");

    quirk_list_init();

    diag_printf("Init USB MSC\n");
    if (ms_usb_register(&usb_storage_driver) < 0)
        USB_ASSERT( 0, "Init USB MSC fail..\n");

#if USB_HID_SUPPORT
    usb_hid_init();
#endif

#if USB_CDC_SUPPORT
    usb_cdc_init();
#endif

    // Decide the current chip
    pCurrentChip = &chipDESC;
#if defined(CHIP_K2)
    diag_printf("Chip Kaiserin!\n");
    if (MDrv_SYS_GetChipRev() >= 0x01) // U02 and Newer IC
    {
        // chip top performance tuning [11:9]
        usb_writew(usb_readw((void*)(KAISERIN_CHIP_TOP_BASE+0x46*2)) | 0xe00, (void*) (KAISERIN_CHIP_TOP_BASE+0x46*2));
    }
#endif
    // set the current chipID
    pCurrentChip->chipID = ms_USBGetChipID();

#ifdef _USB_ENABLE_BDMA_PATCH
    /* get the registers and decide to turn on BDMA SW patch */
    set_64bit_OBF_cipher();
#endif
    // show Lib Infomation
    diag_printf("[USB] MS USB Host Lib for %s only\n", pCurrentChip->name);
}

void MDrv_UsbClose(void)
{
    ms_usb_deregister(&usb_storage_driver);
    MsOS_DeleteEventGroup(_s32UsbEventId);
    _s32UsbEventId = -1;

    ms_exit_sys();
    diag_printf("[USB] End of MDrv_UsbClose...\n");
}

#if 0 // NUSED
/*
    <1>.Disable RunStop
    <1.1>. Chirp hardware patch 1
    <2> .Write PortReset=1
    <3>.Wait time=>50ms
    <3.1>. Chirp hardware patch 2
    <3.2>. Wait time=>20ms
    <4>.Write PortReset=0
    <4.1>. Chirp hardware patch 3
    <5>.Waiting for PortReset==0
    <6>.Enable RunStop Bit
    <6.1>.reset UTMI
    <6.2> Write RunStop Bit=1
    <6.3>.Wait time 5ms, wait some slow device to be ready
    <7>.Detect Speed
*/
#define MSTAR_CHIRP_PATCH 1
#define MSTAR_EHC_RTERM_PATCH 1

int ms_PortReset(unsigned int regUTMI, unsigned int regUHC)
{
    MS_U32 wTmp;

    diag_printf("PortReset: UTMI 0x%x, UHC 0x%x\n", regUTMI, regUHC);

    writeb(readb((void*)(regUHC+0x10*2)) | 0x1, (void*)(regUHC+0x10*2)); //Set UHC run

#if MSTAR_CHIRP_PATCH
    writeb(0x10, (void*)(regUTMI+0x2C*2));
    writeb(0x00, (void*)(regUTMI+0x2D*2-1));
    writeb(0x00, (void*)(regUTMI+0x2F*2-1));
    writeb(0x80 ,(void*)(regUTMI+0x2A*2));
    //writeb(0x20 ,(void*)(regUTMI+0x2A*2));
#endif

#if (MSTAR_EHC_RTERM_PATCH)
    writeb(readb((void*)(regUTMI+0x13*2-1))|0x70, (void*)(regUTMI+0x13*2-1));
#endif

    // Write PortReset bit
    writeb(readb((void*)(regUHC+0x31*2-1)) | 0x01,(void*)(regUHC+0x31*2-1));
    mdelay(50);  //shorten the reset period for Transcend USB3.0 HDD
#if MSTAR_CHIRP_PATCH
    writeb(0x0 ,(void*)(regUTMI+0x2A*2));
#endif
    mdelay(20);

    // Clear PortReset bit
    writeb(readb((void*)(regUHC+0x31*2-1)) & 0xfe,(void*)(regUHC+0x31*2-1));

    wTmp=0;

    while (1)
    {
        if ((readb((void*)(regUHC+0x31*2-1)) & 0x1)==0)  break;

        wTmp++;
        //udelay(1);
        if (wTmp>20000)
        {
            diag_printf("??? Error waiting for Bus Reset Fail...==> Reset HW Control\n");
            //mbHost20_USBCMD_HCReset_Set();
            writeb(readb((void*)(regUHC+0x10*2)) | 0x2,(void*)(regUHC+0x10*2));

            //while(mbHost20_USBCMD_HCReset_Rd()==1);
            while(readb((void*)(regUHC+0x10*2)) && 0x2);
            return (1);
         }
    }


#if (MSTAR_CHIRP_PATCH)
    writeb(readb((void*)(regUTMI+0x2c*2)) |0x10, (void*) (regUTMI+0x2c*2));
    writeb(readb((void*)(regUTMI+0x2d*2-1)) |0x02, (void*) (regUTMI+0x2d*2-1));
    writeb(readb((void*)(regUTMI+0x2f*2-1)) |0x81, (void*) (regUTMI+0x2f*2-1));
#endif

#if (MSTAR_EHC_RTERM_PATCH)
    writeb(readb((void*)(regUTMI+0x13*2-1))&0x8F, (void*)(regUTMI+0x13*2-1));
#endif

    writeb(readb((void*)(regUTMI+0x06*2))|0x03, (void*)(regUTMI+0x06*2)); //reset UTMI
    writeb(readb((void*)(regUTMI+0x06*2))&(~0x03), (void*)(regUTMI+0x06*2)); //reset UTMI

    writeb(readb((void*)(regUHC+0x10*2)) | 0x1, (void*)(regUHC+0x10*2)); //Set UHC run

    mdelay(5); //wait some slow device to be ready

    return (0);
}
#endif

extern BOOL ms_usb_get_connected_dev_state(int *pdevstate,
        unsigned char *pDevClass, struct usb_device *pusbdev, BOOL *pIntfDrvMatched);

MS_BOOL __ms_USBCriticalSectionIn(MS_U8 u8Hostid, MS_U32 WaitMs)
{
    MS_BOOL retval = false;
    struct s_gVar4UsbPort *pRootHub = pCurrentChip->p_roothub[u8Hostid];

    retval = MsOS_ObtainMutex(pRootHub->_s32MutexUSB, WaitMs);

    //lock_usb_core(); // NUSED

    return retval;
}

MS_BOOL ms_USBCriticalSectionIn_TimeOut(MS_U8 Port, MS_U32 WaitMs)
{
    return __ms_USBCriticalSectionIn(Port, WaitMs);
}

MS_BOOL ms_USBCriticalSectionIn(MS_U8 Port)
{
    return __ms_USBCriticalSectionIn(Port, MSOS_WAIT_FOREVER);
}

void ms_USBCriticalSectionOut(MS_U8 u8Hostid)
{
    struct s_gVar4UsbPort *pRootHub = pCurrentChip->p_roothub[u8Hostid];

    //unlock_usb_core(); // NUSED
    MsOS_ReleaseMutex(pRootHub->_s32MutexUSB);
}

// ------------------------------------------------------------------------

struct s_gVar4UsbPort gVar4UsbPort0 =
{
    "USB Hub Task",
    0,
    { 0*MAX_USTOR, 1*MAX_USTOR},
    "cpe_ehci",
    "CPE_AMBA EHCI",
    u8HubStackBuffer,
    "USB_MUTEX",
};

#ifdef ENABLE_THIRD_EHC
MS_U8 u8HubStackBuffer_Port2[HUB_STACK_SIZE];
struct s_gVar4UsbPort gVar4UsbPort2 =
{
    "USB Hub Task 2",
    2,
    { 2*MAX_USTOR, 3*MAX_USTOR},
    "cpe_ehci_2",
    "CPE_AMBA EHCI 2",
    u8HubStackBuffer_Port2,
    "USB_MUTEX_Port2",
};
#endif

static MS_U8 u8HubStackBuffer_Port1[HUB_STACK_SIZE];
struct s_gVar4UsbPort gVar4UsbPort1 =
{
    "USB Hub Task 1",
    1,
    { MAX_USTOR, 2*MAX_USTOR},
    "cpe_ehci_1",
    "CPE_AMBA EHCI 1",
    u8HubStackBuffer_Port1,
    "USB_MUTEX_Port1",
};

#ifdef ENABLE_FOURTH_EHC
static MS_U8 u8HubStackBuffer_Port3[HUB_STACK_SIZE];
struct s_gVar4UsbPort gVar4UsbPort3 =
{
    "USB Hub Task 3",
    3,
    { 3*MAX_USTOR, 4*MAX_USTOR},
    "cpe_ehci_3",
    "CPE_AMBA EHCI 3",
    u8HubStackBuffer_Port3,
    "USB_MUTEX_Port3",
};
#endif

#ifdef ENABLE_FIFTH_EHC
static MS_U8 u8HubStackBuffer_Port4[HUB_STACK_SIZE];
struct s_gVar4UsbPort gVar4UsbPort4 =
{
    "USB Hub Task 4",
    4,
    { 4*MAX_USTOR, 5*MAX_USTOR},
    "cpe_ehci_4",
    "CPE_AMBA EHCI 4",
    u8HubStackBuffer_Port4,
    "USB_MUTEX_Port4",
};
#endif

/* USB host declaration by chip ID */
#if defined(CHIP_U4)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_URANUS4,
    "URANUS4",
    3,
    {
    {0, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC_NULL, E_IRQ_UHC, E_IRQ_USBC},
    {0, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC_NULL, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_DPDM_SWAP, BASE_UTMI2, BASE_UHC2, BASE_USBC2, BASE_USBBC_NULL, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};
#elif defined(CHIP_K1)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KRONUS,
    "KRONUS",
    2,
    {
    {EHCFLAG_DPDM_SWAP, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC_NULL, E_IRQ_UHC, E_IRQ_USBC },
    {EHCFLAG_DPDM_SWAP, BASE_UTMI2, BASE_UHC2, BASE_USBC2, BASE_USBBC_NULL, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, }
};
#elif defined(CHIP_K2)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KAISERIN,
    "KAISERIN",
    4,
    {
    {0, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC_NULL, E_IRQ_UHC, E_IRQ_USBC},
    {0, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC_NULL, E_IRQ_UHC1, E_IRQ_USBC1},
    {0, BASE_UTMI2, BASE_UHC2, BASE_USBC2, BASE_USBBC_NULL, E_IRQ_UHC2, E_IRQ_USBC2},
    {0, BASE_UTMI3, BASE_UHC3, BASE_USBC3, BASE_USBBC_NULL, E_IRQ_UHC3, E_IRQ_USBC3},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2, &gVar4UsbPort3}
};
#elif defined(CHIP_KAPPA)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KAPPA,
    "KAPPA",
    1,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KAPPA, E_IRQ_UHC, E_IRQ_USBC},
    },
    {&gVar4UsbPort0}
};
#elif defined(CHIP_KRITI)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KRITI,
    "KRITI",
    1,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KAPPA, E_IRQ_UHC, E_IRQ_USBC},
    },
    {&gVar4UsbPort0}
};
#elif defined(CHIP_KRATOS)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KRATOS,
    "KRATOS",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KRATOS, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KRATOS, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KELTIC)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KELTIC,
    "KELTIC",
    1,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KELTIC, E_IRQ_UHC, E_IRQ_USBC},
    },
    {&gVar4UsbPort0}
};
#elif defined(CHIP_KENYA)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KENYA,
    "KENYA",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KENYA, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KENYA, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KAISER)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KAISER,
    "KAISER",
    3,
    {
    {EHCFLAG_USBBC_OFF | EHCFLAG_DPDM_SWAP, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KAISER, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF | EHCFLAG_DPDM_SWAP, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KAISER, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2, BASE_UHC2, BASE_USBC2, BASE_USBBC2_KAISER, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};
#elif defined(CHIP_KERES)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KERES,
    "KERES",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KERES, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KERES, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KIRIN)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KIRIN,
    "KIRIN",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KERES, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KERES, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KRIS)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KRIS,
    "KRIS",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KERES, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KERES, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KIWI)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KIWI,
    "KIWI",
    2,
    {
    {EHCFLAG_USBBC_OFF | EHCFLAG_DPDM_SWAP, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KIWI, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF | EHCFLAG_DPDM_SWAP, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KIWI, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_CLIPPERS)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_CLIPPERS,
    "CLIPPERS",
    3,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_CLIPPERS, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_CLIPPERS, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2_CLIPPERS, BASE_UHC2_CLIPPERS, BASE_USBC2_CLIPPERS, BASE_USBBC2_CLIPPERS, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};
#elif defined(CHIP_CURRY)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_CURRY,
    "CURRY",
    3,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_CURRY, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_CURRY, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2_CURRY, BASE_UHC2, BASE_USBC2, BASE_USBBC2_CURRY, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};
#elif defined(CHIP_KAYLA)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KAYLA,
    "KAYLA",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KERES, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KERES, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#elif defined(CHIP_KANO)
#if 1 // for compile
#define E_INT_IRQ_UHC4 0xff
#define E_INT_IRQ_USB4 0xff
#endif
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_KANO,
    "KANO",
#ifdef ENABLE_XHC_COMPANION
    5,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KANO, E_IRQ_UHC, E_IRQ_USBC, XHC_COMP_NONE},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KANO, E_IRQ_UHC1, E_IRQ_USBC1, XHC_COMP_NONE},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2_KANO, BASE_UHC2, BASE_USBC2, BASE_USBBC2_KANO, E_IRQ_UHC2, E_IRQ_USBC2, XHC_COMP_NONE},
    {EHCFLAG_USBBC_OFF | EHCFLAF_XHC_COMP, BASE_UTMI3_KANO, BASE_UHC3_KANO, BASE_USBC3_KANO, BASE_USBBC3_KANO, E_IRQ_UHC3, E_IRQ_USBC3, XHC_COMP_PORT0},
    {EHCFLAG_USBBC_OFF | EHCFLAF_XHC_COMP, BASE_UTMI4_KANO, BASE_UHC4_KANO, BASE_USBC4_KANO, BASE_USBBC4_KANO, E_IRQ_UHC4, E_IRQ_USBC4, XHC_COMP_PORT1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2, &gVar4UsbPort3, &gVar4UsbPort4}
#else
    3,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KANO, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KANO, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2_KANO, BASE_UHC2, BASE_USBC2, BASE_USBBC2_KANO, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
#endif
};
#elif defined(CHIP_K6)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_K6,
    "K6",
#ifdef ENABLE_XHC_COMPANION
    3,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_K6, E_IRQ_UHC, E_IRQ_USBC, XHC_COMP_NONE},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_K6, E_IRQ_UHC1, E_IRQ_USBC1, XHC_COMP_NONE},
    {EHCFLAG_USBBC_OFF | EHCFLAF_XHC_COMP, BASE_UTMI2_K6, BASE_UHC2_K6, BASE_USBC2_K6, BASE_USBBC2_K6, E_IRQ_UHC2_K6, E_IRQ_USBC2_K6, XHC_COMP_PORT0},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
#else
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_K6, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_K6, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
#endif
};
#elif defined(CHIP_K6LITE)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_K6LITE,
    "K6LITE",
    3,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_K6LITE, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_K6LITE, E_IRQ_UHC1, E_IRQ_USBC1},
    {EHCFLAG_USBBC_OFF, BASE_UTMI2_K6LITE, BASE_UHC2_K6LITE, BASE_USBC2_K6LITE, BASE_USBBC2_K6LITE, E_IRQ_UHC2, E_IRQ_USBC2},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};
#elif defined(CHIP_K5TN)
struct s_ChipUsbHostDef chipDESC =
{
    CHIPID_K5TN,
    "K5TN",
    2,
    {
    {EHCFLAG_USBBC_OFF, BASE_UTMI0, BASE_UHC0, BASE_USBC0, BASE_USBBC0_KERES, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_USBBC_OFF, BASE_UTMI1, BASE_UHC1, BASE_USBC1, BASE_USBBC1_KERES, E_IRQ_UHC1, E_IRQ_USBC1},
    },
    {&gVar4UsbPort0, &gVar4UsbPort1}
};
#else
#error No USB Chip definition
#endif

// any new chip added here ^^^
//

/**
     * @brief               USB port N interrupt service routine
     *
     * @param           InterruptNum eIntNum
     *
     * @return          none
     */
static void ms_DrvUSB_OnInterrupt_EX(InterruptNum eIntNum)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct usb_hcd *hcd;
    MS_U8 p;

    if (pChip == NULL)
        return;
    MsOS_DisableInterrupt(eIntNum);
    for (p = 0; p < pChip->nRootHub; p++)
    {
        if (eIntNum == pChip->reg[p].uhcIRQ)
            break;
    }
    hcd = pChip->p_roothub[p]->cpe_ehci_dev.dev.driver_data;
    ms_ehci_irq(hcd, NULL);
    MsOS_EnableInterrupt(eIntNum);
}

/**
     * @brief               initial USB port N interrupt service routine
     *
     * @param           InterruptNum eIntNum
     *
     * @return          none
     */
void ms_InitUSBIntr_EX(struct usb_hcd * hcd, int str_on)
{
    ms_ehci_interrupt_enable(hcd, str_on);
    MsOS_AttachInterrupt(hcd->ehci_irq, ms_DrvUSB_OnInterrupt_EX);
    MsOS_EnableInterrupt(hcd->ehci_irq);
}

void ms_UnInitUSBIntr_EX(struct usb_hcd * hcd)
{
    MsOS_DisableInterrupt(hcd->ehci_irq);
    MsOS_DetachInterrupt(hcd->ehci_irq);
    ms_ehci_interrupt_disable(hcd);
}

/**
     * @brief               USB port connect status and safe guard disconnect ehci reset
     *
     * @param           struct usb_hcd *hcd
     *
     * @return          true/false
     */
MS_BOOL ms_UsbDeviceConnect_EX(struct usb_hcd *hcd)
{
    static MS_U32 usbStartTime = 0;

    if (ms_RoothubPortConnected(hcd))
    {
        return TRUE;
    }
    else
    {
        if (MsOS_GetSystemTime()-usbStartTime > 1000 )
        {
            usbStartTime=MsOS_GetSystemTime();
            ms_ResetMstarUsb(hcd);
            hcd->isRootHubPortReset = TRUE;
            hcd->isBadDeviceRH = FALSE;
            hcd->isBadDevice = FALSE;
            hcd->badDeviceCnt = 0;
        }
        return FALSE;
    }
}

extern int ms_hub_poll(struct s_gVar4UsbPort *pRootHub);
/**
     * @brief               USB port N hub event polling task
     *
     * @param           MS_U32 argc
     * @param           VOID *argv
     *
     * @return          none
     */
void  ms_UsbTask_EX(MS_U32 argc, VOID *argv)
{
    MS_BOOL     isConnect;
    int         DevState, numConnDev;
    MS_U8       DevClass = USB_INTERFACE_CLASS_NONE;
    MS_U8       kk;
    #if USB_HID_SUPPORT
    MS_BOOL     isHIDPlugIn;
    #endif
    struct s_gVar4UsbPort *pRootHub = (struct s_gVar4UsbPort *) argc;
    const MS_U8 hostID = pRootHub->hostid;

    diag_printf("UTask EX>> ... port %d starting\n", hostID);
    while (pRootHub->taskRunning)
    {
        #if USB_HID_SUPPORT
        isHIDPlugIn = FALSE;
        #endif

        // Waiting for USB port connection
        while(pRootHub->taskRunning)
        {
            ms_USBCriticalSectionIn(hostID);
            isConnect = ms_UsbDeviceConnect_EX(pRootHub->p_UsbHcd);
            ms_USBCriticalSectionOut(hostID);

            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[hostID].portNum == hostID) && (pCurrentChip->usbc_ip[hostID].eventFlag))
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[hostID].eventType)
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, hostID, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, hostID, NULL);
                            pCurrentChip->usbc_ip[hostID].eventFlag = 0;
                        }
            #endif
            if(isConnect)
                break;
#ifdef USB_SYSTEM_STR_SUPPORT
            if (pCurrentChip->u8Park)
                pRootHub->bPark_ok = TRUE;
#endif
            MsOS_DelayTask(100);
        }

        // removing any delay before USB bus reset
        //MsOS_DelayTask(1000);
        diag_printf("UTask EX>> USB Port %d is connected\n", hostID);

        while (pRootHub->taskRunning)
        {
#ifdef USB_SYSTEM_STR_SUPPORT
            while(pCurrentChip->u8Park)
            {
                pRootHub->bPark_ok = TRUE;
                MsOS_DelayTask(50);
            }
#endif

            if (pRootHub->p_UsbHcd->isBadDeviceRH)
            {
                diag_printf("UTask EX>> A bad device found on root port %d\n", pRootHub->hostid);
                break;
            }

            kk = 0;
            while(kk<HUB_DEBOUNCE_STABLE)
            {
                ms_USBCriticalSectionIn(hostID);
                isConnect = ms_RoothubPortConnected(pRootHub->p_UsbHcd);
                ms_USBCriticalSectionOut(hostID);
                if ( !isConnect )
                {
                    #if USB_MASS_STORAGE_SUPPORT
                    if (pRootHub->taskRunning) // guard for port close
                        ms_MSC_fast_device_disconnect(pRootHub->vPortRange.vPortDevStart, pRootHub->vPortRange.vPortDevEnd);
                    #endif
                    goto PORT_DISCONNECT_EX;
                }
                kk++;
                MsOS_DelayTask(HUB_DEBOUNCE_STEP);
            }

            // Device is connecting to the port
            numConnDev = 0;
            if (pRootHub->taskRunning) // guard for port close
                numConnDev = ms_hub_poll(pRootHub);

announce_new_dev:
            // Mass storage disk mount
#if USB_MASS_STORAGE_SUPPORT
            if (pRootHub->taskRunning) // guard for port close
                ms_MSC_device_inquiry(pRootHub->vPortRange.vPortDevStart, pRootHub->vPortRange.vPortDevEnd);
#endif

#if USB_HID_SUPPORT
            // TODO: correct HIDGlue API pointer type
            //if (pRootHub->p_UsbHcd == pCurrentChip->p_roothub[pRootHub->hostid]->cpe_ehci_dev.dev.driver_data)
            //    diag_printf("Yes, matched\n");
            if (pRootHub->taskRunning) // guard for port close
                isHIDPlugIn = ms_HID_device_in_inquiry((void *)pRootHub->p_UsbHcd, isHIDPlugIn);
#endif

            while (numConnDev-- && pRootHub->taskRunning)
            {
                USB_ASSERT(numConnDev>=0, "BUG()!!!! numConndev<0\n");
                BOOL IntfDrvMatched = FALSE;

                if ( ms_usb_get_connected_dev_state(&DevState, &DevClass, pRootHub->arConnDev[numConnDev].connDev, &IntfDrvMatched) )
                {
                    if (DevState < USB_STATE_CONFIGURED)
                    {
                        diag_printf("UTask EX>> Usb device not configured (dev#%d)\n", numConnDev);
                        if (pRootHub->p_UsbHcd->isBadDevice || pRootHub->p_UsbHcd->isBadDeviceRH)
                        {
                            diag_printf("UTask EX>> Usb device no responding (dev#%d)\n", numConnDev);
                            if ( _DrvUSB_CBFun != NULL )
                                _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_NO_RESPONSE, NULL);
                        }
                    }
                    else
                    {
                        switch (DevClass)
                        {
#if USB_MASS_STORAGE_SUPPORT
                            case USB_INTERFACE_CLASS_MSD: // list of device class supported
#endif
#if USB_HID_SUPPORT
                            case USB_INTERFACE_CLASS_HID:
#endif
                                break;
#ifndef USB_NOT_SUPPORT_EX_HUB
                            case USB_INTERFACE_CLASS_HUB:
                                diag_printf("UTask EX>> Port %d external Hub is connected (dev#%d)\n", hostID, numConnDev);
                                break;
#endif
                            default:
                                diag_printf("UTask EX>> found Non-classical deivce (dev#%d)\n", numConnDev);
                                if ( _DrvUSB_CBFun != NULL )
                                {
                                    if(IntfDrvMatched == TRUE)
                                    {
                                        diag_printf("UTask EX>> Usb device Vendor supported (dev#%d)\n", numConnDev);
                                        _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_VENDOR, NULL);
                                    }
                                    else
                                    {
                                        diag_printf("UTask EX>> Usb device not supported (dev#%d)\n", numConnDev);
                                        _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_UNKNOW, NULL);
                                    }
                                }
                        }
                    }
                }
            }

            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[hostID].portNum == hostID) && (pCurrentChip->usbc_ip[hostID].eventFlag))
                    if (ms_RoothubPortConnected(pRootHub->p_UsbHcd))
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[hostID].eventType)
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, hostID, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, hostID, NULL);
                            pCurrentChip->usbc_ip[hostID].eventFlag = 0;
                        }
            #endif

            //MsOS_DelayTask(1000);
        }

PORT_DISCONNECT_EX:
        diag_printf("UTask EX>> USB root port %d disconnecting...\n", hostID);
        //if (pRootHub->taskRunning)
        //{
        //    MsOS_DelayTask(100);  //By Jonas! for hub event!
        //}
        //else
        //{
        //MsOS_DelayTask(600);  //By Jonas! for hub event!
        //}
        numConnDev = 0;
        if (pRootHub->taskRunning) // guard for port close
            numConnDev = ms_hub_poll(pRootHub); //for disconnect hub event

        #if USB_HID_SUPPORT
        if (pRootHub->taskRunning) // guard for port close
            ms_HID_device_out_inquiry((void *)pRootHub->p_UsbHcd, isHIDPlugIn);
        #endif

        if(numConnDev != 0)
        {
            diag_printf("[USB] get new device after disconnecting...\n");
            goto announce_new_dev;
        }

        //device is disconnected
        kk = 0;
        while(pRootHub->taskRunning)
        {
            ms_USBCriticalSectionIn(hostID);
            isConnect = ms_RoothubPortConnected(pRootHub->p_UsbHcd);
            ms_USBCriticalSectionOut(hostID);
            if(isConnect == FALSE)
            {
                diag_printf("UTask EX>> device plug out!\n");
                break;
            }
            if (pRootHub->p_UsbHcd->isBadDeviceRH && (kk % 10 == 0))
            {
                diag_printf("UTask EX>> a bad device waiting plug-out!\n");
            }
#ifdef USB_SYSTEM_STR_SUPPORT
            if (pCurrentChip->u8Park)
                pRootHub->bPark_ok = TRUE;
#endif
            MsOS_DelayTask(100);
            kk++;
        }
        if (pRootHub->p_UsbHcd) // guard for port close
        {
            ms_USBCriticalSectionIn(hostID);
            diag_printf("UTask EX>> reset ms USB\n");
            /* enable QH Head re-link flow */
            pRootHub->p_UsbHcd->rh_disconn = 1;
            ms_ResetMstarUsb(pRootHub->p_UsbHcd);
            pRootHub->p_UsbHcd->rh_disconn = 0;
            ms_USBCriticalSectionOut(hostID);
        }

    }
    diag_printf("UTask EX>> ... port %d finished\n", hostID);
    pRootHub->taskFinish = TRUE;
}

/**
     * @brief               creating USB mutex for  selected port
     *
     * @param           U8 port
     *
     * @return          USB mutex
     */
MS_S32 ms_Create_USB_Mutex(MS_U8 u8Hostid)
{
    struct s_gVar4UsbPort *pRootHub = pCurrentChip->p_roothub[u8Hostid];

    return pRootHub->_s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, pRootHub->usb_mutex_name, MSOS_PROCESS_SHARED);
}

/**
     * @brief               deleting USB mutex for selected port
     *
     * @param           U8 port
     *
     * @return          none
     */
void ms_Delete_USB_Mutex(MS_U8 u8Hostid)
{
    struct s_gVar4UsbPort *pRootHub = pCurrentChip->p_roothub[u8Hostid];

    MsOS_DeleteMutex(pRootHub->_s32MutexUSB);
}

/**
     * @brief               creating USB port N hub event polling task
     *
     * @param           struct s_gVar4UsbPort *pRootHub
     *
     * @return          none
     */
void ms_USB_Start_EX(struct s_gVar4UsbPort *pRootHub)
{
    MS_U8 *HubStack;

    diag_printf("\nUsb start EX..., pRootHub = %x\n\n", (unsigned int)pRootHub);

    HubStack = pRootHub->u8pHubStackBuffer;

    if (ms_Create_USB_Mutex(pRootHub->hostid) < 0)
    {
	diag_printf("create USB mutex failed\n");
        GEN_EXCEP;
        return;
    }

    //Create Task
    pRootHub->taskRunning = TRUE;
    pRootHub->taskFinish = FALSE;

    pRootHub->pid = MsOS_CreateTask((TaskEntry) ms_UsbTask_EX,
                         (MS_U32)pRootHub,
                         E_TASK_PRI_HIGH,
                         TRUE,
                         HubStack,
                         HUB_STACK_SIZE,
                         pRootHub->name);
    if (pRootHub->pid < 0)
    {
	diag_printf("create USB Task failed\n");
        ms_Delete_USB_Mutex(pRootHub->hostid);
        GEN_EXCEP;
        return;
    }
}

void ms_USB_Stop_EX(struct s_gVar4UsbPort *pRootHub)
{
    while(!pRootHub->taskFinish)
    {
        MsOS_DelayTask(100);
    }
    diag_printf("[ms_USB_Stop_EX] DeleteTask ++\n");
    /* 20150608 remove MsOS_DeleteTask per system's request */
    //MsOS_DeleteTask(pRootHub->pid);
    pRootHub->pid = -1;
    ms_USBCriticalSectionIn(pRootHub->hostid);
    ms_USBCriticalSectionOut(pRootHub->hostid);
    ms_Delete_USB_Mutex(pRootHub->hostid);
    diag_printf("[ms_USB_Stop_EX] delete Mutex --\n");
}


extern int ms_create_cpe_hcd(struct cpe_dev *dev);
extern void ms_usb_hcd_cpe_ehci_remove (struct usb_hcd *pHcd);

/**
     * @brief             USB port N initial
     *
     * @param           MS_U8 u8PortNum
     *
     * @return            true/false
     */
MS_BOOL MDrv_USB_Port_Init(MS_U8 u8Hostid)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;

    if (pChip->nRootHub <= u8Hostid)
    {
        diag_printf("Chip %s does not not support port %d\n", pChip->name, u8Hostid);
        return FALSE;
    }
    else
    {
        diag_printf("Init chip %s, port %d\n", pChip->name, u8Hostid);
        // bind chip reg definition with cpe_dev
        pChip->p_roothub[u8Hostid]->cpe_ehci_dev.uhcbase = pChip->reg[u8Hostid].baseUHC;
        pChip->p_roothub[u8Hostid]->cpe_ehci_dev.utmibase = pChip->reg[u8Hostid].baseUTMI;
        pChip->p_roothub[u8Hostid]->cpe_ehci_dev.usbcbase = pChip->reg[u8Hostid].baseUSBC;
        pChip->p_roothub[u8Hostid]->cpe_ehci_dev.intNum = pChip->reg[u8Hostid].uhcIRQ;
    }
    /* initial hub event list */
    ms_list_init(&pChip->p_roothub[u8Hostid]->hub_event);
    pChip->p_roothub[u8Hostid]->cpe_ehci_dev.pHubEvent = &pChip->p_roothub[u8Hostid]->hub_event;
    pChip->p_roothub[u8Hostid]->cpe_ehci_dev.bus_name = pChip->p_roothub[u8Hostid]->bus_name;
    pChip->p_roothub[u8Hostid]->cpe_ehci_dev.product_desc = pChip->p_roothub[u8Hostid]->product_desc;
    pChip->p_roothub[u8Hostid]->cpe_ehci_dev.devid = pChip->p_roothub[u8Hostid]->hostid;

    // do Usb Host initial
    ms_U4_series_usb_init(pChip, u8Hostid);

#ifdef ENABLE_XHC_COMPANION
    if(pChip->reg[u8Hostid].iFlag & EHCFLAF_XHC_COMP)
    {
        //enable xHCI clock
        xhci_enable_clock();
        //disable port
        xhci_ssport_set_state(&pChip->reg[u8Hostid].xhci, false);
        //turn on power
        xhci_ppc(&pChip->reg[u8Hostid].xhci, true);
    }
#endif

    ms_create_cpe_hcd(&pChip->p_roothub[u8Hostid]->cpe_ehci_dev); // create hcd base on cpe info
    if (pChip->p_roothub[u8Hostid]->cpe_ehci_dev.dev.driver_data == NULL)
    {
        diag_printf("port %d hcd not allocated!!!\n", u8Hostid);
        return FALSE;
    }
    pChip->p_roothub[u8Hostid]->p_UsbHcd = pChip->p_roothub[u8Hostid]->cpe_ehci_dev.dev.driver_data; // for MDrv_UsbDeviceConnect()

    ms_USB_Start_EX(pChip->p_roothub[u8Hostid]);

    return TRUE;
}

/**
     * @brief           Halt USB port N
     *
     * @param           MS_U8 u8Hostid
     *
     * @return          true/false
     */
MS_BOOL MDrv_USB_Port_Close(MS_U8 u8Hostid)
{
    struct s_gVar4UsbPort *pRootHub = pCurrentChip->p_roothub[u8Hostid];
    struct usb_hcd *pHcd = pRootHub->p_UsbHcd;

    diag_printf("[USB] try to shutdown UHC(%d)\n", u8Hostid);

    if(!pHcd)
    {
        diag_printf("[USB] Error UHC%d is not inited ??\n", u8Hostid);
        return FALSE;
    }

    pRootHub->taskRunning= FALSE;
    pHcd->state = HCD_STATE_HALT; // speed up port close flow
    ms_usb_set_device_state(pHcd->self.root_hub, USB_STATE_NOTATTACHED);
    diag_printf("[USB] disable Port %d interrupt\n", u8Hostid);
    ms_UnInitUSBIntr_EX(pHcd);

    diag_printf("[USB] ms_USB_Stop_EX(%d)\n", u8Hostid);
    ms_USB_Stop_EX(pRootHub);

#ifdef ENABLE_XHC_COMPANION
    if(pCurrentChip->reg[u8Hostid].iFlag & EHCFLAF_XHC_COMP)
    {
        //turn off power
        xhci_ppc(&pCurrentChip->reg[u8Hostid].xhci, false);
        //enable port
        xhci_ssport_set_state(&pCurrentChip->reg[u8Hostid].xhci, true);
        mdelay(1000);
    }
#endif

    diag_printf("[USB] ms_usb_hcd_cpe_ehci_remove\n");
    ms_usb_hcd_cpe_ehci_remove(pHcd);

    pRootHub->cpe_ehci_dev.dev.driver_data = NULL;
    pRootHub->p_UsbHcd = NULL;
    diag_printf("[USB] End of MDrv_USB_Port_Close(%d)\n", u8Hostid);
    return TRUE;
}

extern void ms_init_usbc_intr(MS_U8 p);
/**
     * @brief               register call back function of over current detection
     *
     * @param           USBCallback pCallbackFn
     * @param           MS_U8 port_mask
     *
     * @return          none
     */
void MDrv_OverCurrentDetect_RegisterCallBack (USBCallback pCallbackFn, MS_U8 port_mask)
{
#if USBC_IP_SUPPORT // USBC IP control
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    MS_U8 p;

    diag_printf("<MDrv_OverCurrentDetect_RegisterCallBack> %p, port_mask(%x)\n", pCallbackFn, port_mask);
    _DrvUSBC_CBFun = pCallbackFn;
    for (p = 0; p < pChip->nRootHub; p++)
        if (port_mask & (1<<p))
            ms_init_usbc_intr(p);
#else
    diag_printf("<MDrv_OverCurrentDetect_RegisterCallBack> NOT support!!! Please turn on USBC_IP_SUPPORT in drvUSB.h\n");
#endif
}

/**
     * @brief           return connection bit for portN
     *
     * @param           MS_U8 u8Hostid
     *
     * @return          connection bit
     */

int MDrv_UsbDeviceConnectBitEx(MS_U8 u8Hostid)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct usb_hcd *hcd;

    if (pChip == NULL)
    {
        diag_printf("[USB] Error!!! pChip is Null\n");
        return -1;
    }

    hcd = pChip->p_roothub[u8Hostid]->cpe_ehci_dev.dev.driver_data;

    if(hcd == NULL)
    {
        diag_printf("[USB] Error!!! hcd is Null, host_id %d\n", u8Hostid);
        return -1;
    }

    if(ms_RoothubPortConnected(hcd))
        return 1;

    return 0;
}

#ifdef USB_SYSTEM_STR_SUPPORT
void MDrv_Usb_STR_Off(MS_U8 u8Hostid)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct usb_hcd *hcd;

    if (pChip == NULL)
    {
        diag_printf("[USB] Error!!! pChip is Null\n");
        return;
    }
    hcd = pChip->p_roothub[u8Hostid]->cpe_ehci_dev.dev.driver_data;

    if(hcd == NULL)
    {
        diag_printf("[USB] Error!!! hcd is Null, host_id %d\n", u8Hostid);
        return;
    }
    diag_printf("[USB] STR off...port %d\n", u8Hostid);
    pChip->u8Park = 1;
    hcd->state = HCD_STATE_HALT; // spped up urb flow
    diag_printf("[USB] STR waiting for park ");
    while(pChip->p_roothub[u8Hostid]->bPark_ok == FALSE)
    {
        diag_printf(".");
        MsOS_DelayTask(50);
    }
    diag_printf(" ok\n");
    ms_UnInitUSBIntr_EX(hcd);
}

extern void ms_ehci_periodic_size_init (struct usb_hcd *hcd);
void MDrv_Usb_STR_On(MS_U8 u8Hostid)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct usb_hcd *hcd;

    if (pChip == NULL)
    {
        diag_printf("[USB] Error!!! pChip is Null\n");
        return;
    }
    hcd = pChip->p_roothub[u8Hostid]->cpe_ehci_dev.dev.driver_data;

    if(hcd == NULL)
    {
        diag_printf("[USB] Error!!! hcd is Null, host_id %d\n", u8Hostid);
        return;
    }
    diag_printf("[USB] STR on...port %d\n", u8Hostid);
    ms_U4_series_usb_init(pChip, u8Hostid);
    ms_ehci_periodic_size_init(hcd);
    ms_InitUSBIntr_EX(hcd, 1);
    hcd->isBadDeviceRH = FALSE;
    pChip->u8Park = 0;
    pChip->p_roothub[u8Hostid]->bPark_ok = FALSE;
}
#endif

