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

#include "include/drvConfig.h"
#include "include/drvPorts.h"
#include "include/drvKernel.h"
#include "include/drvTimer.h"
#include "include/drvUSBMsg.h"

#include "drvHCD.h"
#include "drvMassStor.h"
#include "include/drvCPE_EHCI.h"
#include "MsCommon.h"
//#include "drvpower.h"
#include "drvUSB.h"
#include "include/drvIO.h"
#include "drvUSBHwCtl.h"
#include "include/drvCPE_AMBA.h"
#include "drvEHCI.h"
#include "drvUsbcommon.h"
#include "drvSCSI.h"
#if USB_HID_SUPPORT  
#include "drvHIDDev.h"
#endif
#if USB_CDC_SUPPORT  
#include "drvCDCDev.h"
#endif

//#define HSTTASK_DEBUG
#ifdef HSTTASK_DEBUG
#define UDRV_Dbg		diag_printf
#else
#define UDRV_Dbg(x,...)
#endif

#define UDRV_ERR		diag_printf


struct s_ChipUsbHostDef *pCurrentChip;
struct s_ChipUsbHostDef chipURANUS4;
struct s_ChipUsbHostDef chipKRONUS;
struct s_ChipUsbHostDef chipKAISERIN;

mem_Alloc    pfnAllocCachedMem=NULL, pfnAllocNoncachedMem=NULL;
mem_Free     pfnFreeCachedMem=NULL, pfnFreeNoncachedMem=NULL;
mem_VA2PA    pfnVA2PA=NULL;
mem_PA2VA    pfnPA2VA=NULL;
mem_Cached2Noncached     pfnCached2Noncached=NULL;
mem_NonCached2Cached     pfnNoncached2Cached=NULL;


S32 _s32MutexUSB;
S32 _s32MutexUSB_Port2;
S32 _s32MutexUSB_Port1;
S32 _s32MutexUSB_Port3;

MS_S32 _s32UsbEventId = -1;
MS_BOOL UsbReady = FALSE;
//MS_U16  gIntStatus;
MS_BOOL  isRootHubPortReset = TRUE;
BOOL  gIsBadDevice = FALSE;
MS_U8 gUsbChipID = 0xFF;


MS_U32 gBaseUTMI=0, gBaseUSBC=0, gBaseUHC=0;
MS_U32 gBaseUTMI2=0, gBaseUSBC2=0, gBaseUHC2=0;
MS_U32 gIrqUHC=0xFFFF, gIrqUHC2=0xFFFF;

#define MAX_HCD_SUPPORT 2
MS_U8  gInitHcdNum=0, gHcdMap[MAX_HCD_SUPPORT]={0}; 

USBCallback _DrvUSBC_CBFun = NULL;

static void _DrvUSB_OnInterrupt(InterruptNum eIntNum);
void  UsbTask(MS_U32 argc, VOID *argv);
void USB_Start(void);
USBCallback _DrvUSB_CBFun = NULL;

extern BOOL UsbReady;
extern struct usb_hcd *g_pUsbHcd;
extern struct cpe_dev cpe_ehci_dev;

//HID device types
#if USB_HID_SUPPORT            
extern S32 g_Hid_Timer_ID;
extern void usb_hid_parse_report(U8 *start, U16 size, struct button_struct *pButton,struct axis_struct *pAxis, struct key_struct *pKeys);
#endif
extern void init_OS_Port2_Resource(void);
extern void USB_Start_Port2(void);
void USBCriticalSectionIn(MS_U8 Port);
void USBCriticalSectionOut(MS_U8 Port);
extern void init_OS_Resource(void);
extern int __init ehci_cpe_dev_init(void);
extern int __init ehci_cpe_dev_init_Port2(void);


//#define ERROR_CONCEALMENT
#ifdef ERROR_CONCEALMENT
U32 u32UsbTaskTimer = 0;
#endif

#ifndef  MS_NOSAPI
static MS_U8 u8HubStackBuffer[HUB_STACK_SIZE];
#endif

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
    //MsOS_DisableInterrupt(E_IRQ_UHC);
    _DrvUSB_CBFun = pCallbackFn;
    //MsOS_EnableInterrupt(E_IRQ_UHC);
}

MS_U8 MDrv_USBGetChipID(void)
{
    if (gUsbChipID == 0xFF)
        gUsbChipID = usb_readb(OS_BASE_ADDR+0x1ecc*2);

	//diag_printf("GetChipID: 0x%x\n", gUsbChipID);
    return gUsbChipID;
}

void U4_series_usb_init(unsigned int UTMI_base, unsigned int USBC_base, unsigned int UHC_base, unsigned int flag)
{
    MS_U8 chipID;
    MS_U16 chipVER = 0x100; // U01
    
	diag_printf("usb_init start, UTMI base %p, USBC base %p, UHC base %p\n", UTMI_base, USBC_base, UHC_base);

    if (flag & EHCFLAG_TESTPKG)
    {
	    usb_writew(0x2084, (void*) (UTMI_base+0x2*2)); //
	    usb_writew(0x0003, (void*) (UTMI_base+0x20*2)); //
    }

	usb_writeb(0x0a, (void*) (USBC_base)); // Disable MAC initial suspend, Reset UHC
	usb_writeb(0x28, (void*) (USBC_base)); // Release UHC reset, enable UHC and OTG XIU function

	usb_writeb(usb_readb((void*)(UTMI_base+0x22*2)) | 0xE0, (void*) (UTMI_base+0x22*2)); // Set PLL_TEST[23:21] for enable 480MHz clock
	usb_writeb(usb_readb((void*)(UTMI_base+0x20*2)) | 0x02, (void*) (UTMI_base+0x20*2)); // Set PLL_TEST[1] for PLL multiplier 20X

    chipID = MDrv_USBGetChipID();
    if ((chipID == CHIPID_KRONUS) || (chipID == CHIPID_KAISERIN))
    {
            usb_writeb(usb_readb((void*)(UTMI_base+0x0D*2-1)) | 0x01, (void*) (UTMI_base+0x0D*2-1)); // set reg_double_data_rate, To get better jitter performance
	    usb_writeb(0,    (void*) (UTMI_base+0x21*2-1));
	    usb_writeb(0x10, (void*) (UTMI_base+0x23*2-1));
	    usb_writeb(0x01, (void*) (UTMI_base+0x24*2));
    }

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
	mdelay(1);	// delay 1ms

	usb_writeb(0x69, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	usb_writeb(0x01, (void*) (UTMI_base)); // Turn all (including hs_current) use override mode
	usb_writeb(0, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	usb_writeb(0x0A, (void*) (USBC_base)); // Disable MAC initial suspend, Reset UHC
	usb_writeb(0x28, (void*) (USBC_base)); // Release UHC reset, enable UHC XIU function

	usb_writeb(usb_readb((void*)(UTMI_base+0x3C*2)) | 0x01, (void*) (UTMI_base+0x3C*2)); // set CA_START as 1
	mdelay(10);

	usb_writeb(usb_readb((void*)(UTMI_base+0x3C*2)) & ~0x01, (void*) (UTMI_base+0x3C*2)); // release CA_START

	while ((usb_readb((void*)(UTMI_base+0x3C*2)) & 0x02) == 0);	// polling bit <1> (CA_END)

    if (flag & EHCFLAG_DPDM_SWAP)
    	usb_writeb(usb_readb((void*)(UTMI_base+0x0b*2-1)) |0x20, (void*) (UTMI_base+0x0b*2-1)); // dp dm swap

	usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) & ~0x03, (void*) (USBC_base+0x02*2)); //UHC select enable
	usb_writeb(usb_readb((void*)(USBC_base+0x02*2)) | 0x01, (void*) (USBC_base+0x02*2)); //UHC select enable

	usb_writeb(usb_readb((void*)(UHC_base+0x40*2)) & ~0x10, (void*) (UHC_base+0x40*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	usb_writeb(usb_readb((void*)(UHC_base+0x40*2)) | 0x08, (void*) (UHC_base+0x40*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	usb_writeb(usb_readb((void*)(UHC_base+0x81*2-1)) | 0x8F, (void*) (UHC_base+0x81*2-1)); //improve the efficiency of USB access MIU when system is busy
	
    if ((chipID == CHIPID_KRONUS) || (chipID == CHIPID_URANUS4))
	usb_writeb(usb_readb((void*)(UHC_base+0x83*2-1)) | 0x40, (void*) (UHC_base+0x83*2-1)); //set MIU1_sel to 128MB
    // Kaiserin will keep the defaut "00" to be as 512MB support	

	usb_writeb((usb_readb((void*)(UTMI_base+0x06*2)) & 0x9F) | 0x40, (void*) (UTMI_base+0x06*2)); //reg_tx_force_hs_current_enable

	usb_writeb(usb_readb((void*)(UTMI_base+0x03*2-1)) | 0x28, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select
	usb_writeb(usb_readb((void*)(UTMI_base+0x03*2-1)) & 0xef, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select

	usb_writeb(usb_readb((void*)(UTMI_base+0x07*2-1)) & 0xfd, (void*) (UTMI_base+0x07*2-1)); //Disable improved CDR
	usb_writeb(usb_readb((void*)(UTMI_base+0x09*2-1)) |0x81, (void*) (UTMI_base+0x09*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	if ((flag & EHCFLAG_DOUBLE_DATARATE)==0)
	    usb_writeb(usb_readb((void*)(UTMI_base+0x0b*2-1)) |0x80, (void*) (UTMI_base+0x0b*2-1)); // TX timing select latch path
	usb_writeb(usb_readb((void*)(UTMI_base+0x15*2-1)) |0x20, (void*) (UTMI_base+0x15*2-1)); // Chirp signal source select

	usb_writeb(usb_readb((void*)(UTMI_base+0x2c*2)) |0x10, (void*) (UTMI_base+0x2c*2));
	usb_writeb(usb_readb((void*)(UTMI_base+0x2d*2-1)) |0x02, (void*) (UTMI_base+0x2d*2-1));
	usb_writeb(usb_readb((void*)(UTMI_base+0x2f*2-1)) |0x81, (void*) (UTMI_base+0x2f*2-1));

    // Kaiserin U02 patch code
    if (chipID == CHIPID_KAISERIN)
        chipVER = usb_readw((void *)(KAISERIN_CHIP_TOP_BASE+0xCE*2));
    if ((chipID == CHIPID_KAISERIN) && (chipVER == 0x101)) // && U02
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
        writeb(readb((void*)(USBC_base+0x0A*2)) | 0x40, (void*) (USBC_base+0x0A*2)); //enable short packet MDATA in Split transaction clears ACT bit (LS dev under a HS hub)
    }

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
    MS_U8 cid;
    MS_U16 chipVER = 0x100; // U01
    diag_printf("Usb init\n");

    pfnAllocCachedMem = pfn_Cachedmem_Alloc;
    pfnFreeCachedMem = pfn_Cachedmem_Free;
    pfnAllocNoncachedMem = pfn_NonCachedmem_Alloc;
    pfnFreeNoncachedMem = pfn_NonCachedmem_Free;
    pfnVA2PA = pfn_mem_VA2PA;
    pfnPA2VA = pfn_mem_PA2VA;
    pfnCached2Noncached = pfn_mem_Cached2Noncached;
    pfnNoncached2Cached = pfn_mem_NonCached2Cached;

    init_sys();

#ifndef  MS_NOSAPI
    ASSERT(-1 == _s32UsbEventId);
    _s32UsbEventId = MsOS_CreateEventGroup("UHC_Event");
#endif

    usb_stor_init();

#if USB_HID_SUPPORT
    usb_hid_init();
#endif

#if USB_CDC_SUPPORT
    usb_cdc_init();
#endif

    // Decide the current chip
    cid = MDrv_USBGetChipID();
    switch (cid)
    {
        case CHIPID_URANUS4:
            pCurrentChip = &chipURANUS4;
            break;
        case CHIPID_KRONUS:
            pCurrentChip = &chipKRONUS;
            break;
        case CHIPID_KAISERIN:
            pCurrentChip = &chipKAISERIN;
            diag_printf("Chip Kaiserin!\n");
            chipVER = usb_readw((void *)(KAISERIN_CHIP_TOP_BASE+0xCE*2));
            if (chipVER == 0x101) // U02
            {
                // chip top performance tuning [11:9]
                usb_writew(usb_readw((void*)(KAISERIN_CHIP_TOP_BASE+0x46*2)) | 0xe00, (void*) (KAISERIN_CHIP_TOP_BASE+0x46*2));
            }
            break;            
        default:
            pCurrentChip = NULL;
            diag_printf("Not allowed chip ID\n");
    }
    pCurrentChip->chipID = cid;
}


extern MS_BOOL MDrv_USB_Port_Init_EX(MS_U8 u8PortNum);
MS_BOOL MDrv_USB_Port_Init(MS_U8 u8PortNum)
{
        unsigned int dwFlag=0;
	MS_U8 i=0;
        MS_U8 cid;
	MS_U32 *pUTMI, *pUSBC, *pUHC, *pIrq;

        cid = MDrv_USBGetChipID();
	if (cid== CHIPID_KAISERIN)
	{
	        return( MDrv_USB_Port_Init_EX(u8PortNum));
	}

	if (gInitHcdNum >= MAX_HCD_SUPPORT)
	{
		diag_printf(" Assert !! U4_series_usb_init: Exceed max USB number support !!\n");
		while(1);
	}

	if (gInitHcdNum) {
		for (i=0; i<gInitHcdNum; i++) {
			if (u8PortNum == gHcdMap[i])
			{
				diag_printf(" Assert !! U4_series_usb_init: Initialize the same port duplicately !!\n");
				while(1);
			}
		}
	}

	gHcdMap[gInitHcdNum] = u8PortNum;
	gInitHcdNum++;

	switch (gInitHcdNum)
	{
		case 1:
				pUTMI = &gBaseUTMI;
				pUSBC = &gBaseUSBC;
				pUHC  = &gBaseUHC;
				pIrq  = &gIrqUHC;
				break;
		case 2:	
				pUTMI = &gBaseUTMI2;
				pUSBC = &gBaseUSBC2;
				pUHC  = &gBaseUHC2;
				pIrq  = &gIrqUHC2;
				break;
		default:
			diag_printf(" Assert !! U4_series_usb_init: Not handled code !!\n");
			while(1);
			break;
	}

    
    if (cid== CHIPID_KRONUS) //K1 only!!
    {
        dwFlag |= EHCFLAG_DPDM_SWAP;
    }

    switch(u8PortNum)
    {
        case 0:
			*pUTMI = BASE_UTMI0;
			*pUSBC = BASE_USBC0;
			*pUHC  = BASE_UHC0;
			*pIrq  = E_IRQ_UHC;
            break;

        case 1:
            if (cid== CHIPID_KRONUS)
            {
				*pUTMI = BASE_UTMI2;
				*pUSBC = BASE_USBC2;
				*pUHC  = BASE_UHC2;
				*pIrq  = E_IRQ_UHC2;
			}
			else
			{   //U4
				*pUTMI = BASE_UTMI1;
				*pUSBC = BASE_USBC1;
				*pUHC  = BASE_UHC1;
				*pIrq  = E_IRQ_UHC1;
            }
			break;

        case 2:
            if (cid!= CHIPID_URANUS4) //U4 only!!
        	{
				diag_printf(" Assert !! U4_series_usb_init: No this port number in the chip !!\n");
				while(1);
        	}
            if (cid== CHIPID_URANUS4) //U4 only!!
            {
                dwFlag |= EHCFLAG_DPDM_SWAP;
            }

			*pUTMI = BASE_UTMI2;
			*pUSBC = BASE_USBC2;
			*pUHC  = BASE_UHC2;
			*pIrq  = E_IRQ_UHC2;
            break;
			
        default:
			diag_printf(" Assert !! U4_series_usb_init: No this port number in the chip !!\n");
			while(1);
            return FALSE;
            }
        
    
	switch (gInitHcdNum)
	{
		case 1:
            U4_series_usb_init(gBaseUTMI, gBaseUSBC, gBaseUHC, dwFlag);
            init_OS_Resource();
            ehci_cpe_dev_init();
            USB_Start();
			break;
		case 2:	
            U4_series_usb_init(gBaseUTMI2, gBaseUSBC2, gBaseUHC2, dwFlag);
            init_OS_Port2_Resource();
            ehci_cpe_dev_init_Port2();
            USB_Start_Port2();
            break;
        default:
			diag_printf(" Assert !! U4_series_usb_init: Not handled code !!\n");
			while(1);
			break;
    }
    
    return TRUE;
}

void USB_Start(void)
{
#ifndef  MS_NOSAPI
    MS_U8 *HubStack;
    int pid;
#endif

    diag_printf("Usb start..\n");

#ifndef  MS_NOSAPI
    HubStack = u8HubStackBuffer;
    _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX", MSOS_PROCESS_SHARED);

    if (_s32MutexUSB < 0)
    {
        GEN_EXCEP;
        return;
    }
#endif //MS_NOSAPI

#ifndef MS_NOSAPI
    //Create Task
    pid = MsOS_CreateTask((TaskEntry) UsbTask,
                         (MS_U32)NULL,
                         E_TASK_PRI_HIGH,
                         TRUE,
                         HubStack,
                         HUB_STACK_SIZE,
                         "USB Hub Task");
    if (pid < 0)
    {
        GEN_EXCEP;
        //MsOS_FreeMemory(HubStack, gs32CachedPoolID);
        MsOS_DeleteMutex(_s32MutexUSB);       
            
        //MsOS_DeleteMutex(_s32MutexUSB_Port2);

        return;
    }
#else
    //UsbTask();
#endif //MS_NOSAPI

    return;
}

//void ResetUsbHardware(void) moved to drvUSBHwCtl.c
extern void ResetMstarUsb(struct ehci_hcd *ehci);

MS_BOOL MDrv_UsbDeviceConnect_EX(struct usb_hcd *hcd);
MS_BOOL MDrv_UsbDeviceConnect(struct usb_hcd *hcd)
{
#if 1 // _EX
    if (MDrv_UsbDeviceConnect_EX(hcd))
        return TRUE;
    else
        return(UsbReady = FALSE);
#else
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    MS_U8 temp_s;
    MS_U32 temp;
    static MS_U32  UsbStartTime=0;

    temp_s = ehci_readb((U32)&ehci->regs->port_status[0]);
    //temp=mwHost20_PORTSC_ConnectStatus_Rd();
    //diag_printf("ConnectStatus:%02bx\n",temp);
    //MsOS_DelayTask(10);
    if (temp_s & PORT_CONNECT)
    {
        // diag_printf("TEMP:%02bx\n",temp);
        return TRUE;
    }
    else
    {
#ifndef MS_NOSAPI
        if (MsOS_GetSystemTime()-UsbStartTime > 1000 )
        {
            UsbStartTime=MsOS_GetSystemTime();
            
            temp = ehci_readl((U32)&ehci->regs->bus_control);
            temp &= ~0x80;
            ehci_writel(temp, (U32)&ehci->regs->bus_control);
            //UHC_ANDXBYTE(0x40, 0x7F);//disable force enter FSmode
            ResetMstarUsb(ehci);
        }
#endif
        hcd->isRootHubPortReset = TRUE;
        UsbReady=FALSE;
        hcd->isBadDevice = FALSE;
        return FALSE;
    }
#endif    
}

void MDrv_UsbClose(void)
{
#ifndef  MS_NOSAPI
//    MsOS_DisableInterrupt(E_IRQ_UHC);
//    MsOS_DetachInterrupt(E_IRQ_UHC);
//    MsOS_DeleteEventGroup(_s32UsbEventId);
//    _s32UsbEventId = -1;
///    Host20_ReleaseBuffer();
#endif
///	flib_Host20_Close(0);
#ifndef MS_NOSAPI
#if 0 ///
    if(usb_temp_buf)
    {
        MsOS_FreeMemory((void*)usb_temp_buf,gs32NonCachedPoolID_MIU0);
        usb_temp_buf = 0;
    }
#endif
#endif
}

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

int PortReset(unsigned int regUTMI, unsigned int regUHC)
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

    mdelay(5);		//wait some slow device to be ready

    return (0);
}

#if USB_HID_SUPPORT            
extern int usb_hid_get_int_ex (U8 uPort, U8 *Buf, U8 size);
extern int usb_hid_get_int_ex_MultipleIntf (U8 uPort, U8 uIntfNum, U8 *Buf, U8 size);
#endif

//BOOL  gUsbInit;
#if USB_HID_SUPPORT            
extern U8 Hid_Report_Desc[0x100];
extern U8 Hid_Data[10];
extern struct hid_data *HID_us[MAX_HID_DEV_COUNT][MAX_HID_INTF_COUNT];
extern MS_U32  Invalid_HidDevs;
BOOL  gScanHIDDevices;
#endif
extern BOOL usb_get_connected_dev_state(int *pdevstate, unsigned char *pDevClass, struct usb_hcd *hcd);
void  UsbTask(MS_U32 argc, VOID *argv)
{
    MS_U16    PortStatus;
    MS_BOOL   isExtHubConnect, isFirstEvent, isConnect;
    int       DevState;
    MS_U8     DevClass, ii, kk;
    MS_U8     LunSts[MAX_USB_STOR_LUNS];
    MS_U8     LunPreSts[MAX_USB_STOR_LUNS];
    MS_BOOL   isMSCPlugIn;
    MS_U8     us_id;
    //struct LUN_Device* LunDevice;    
    struct ehci_hcd *ehci = hcd_to_ehci(g_pUsbHcd);

    #if (USB_HID_SUPPORT)
    MS_U8    jj;
    #endif
    
    #if USB_HID_SUPPORT    
    MS_BOOL   isHIDPlugIn, isHIDPlugIntoSamePort;
    MS_U8     isHIDPlugIn_IntfCnt;
    #endif
    #if USB_CDC_SUPPORT
	MS_BOOL   isCDCPlugIn;
    #endif	

    diag_printf("UsbTask..\n");
    while (1)
    {
        //UsbReady=FALSE;   //move to MDrv_UsbDeviceConnect
        isExtHubConnect = FALSE;
        isMSCPlugIn = FALSE;
#if USB_HID_SUPPORT            
        isHIDPlugIn = FALSE;
        isHIDPlugIn_IntfCnt = 0;
#endif
#if USB_CDC_SUPPORT
		isCDCPlugIn = FALSE;
#endif
        // Waiting for USB port connection
        while(1)
        {
            USBCriticalSectionIn(0);
            isConnect = MDrv_UsbDeviceConnect(g_pUsbHcd);
            USBCriticalSectionOut(0);
            
            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[0].portNum == 0) && (pCurrentChip->usbc_ip[0].eventFlag))
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[0].eventType)                                    
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, 0, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, 0, NULL);
                            pCurrentChip->usbc_ip[0].eventFlag = 0;
                        }
            #endif
            if(isConnect)
                break;

            MsOS_DelayTask(100);
        }

        // removing any delay before USB bus reset
        //MsOS_DelayTask(1000);
        //diag_printf("USB is connect\n");

        while (1)
        {
            if (g_pUsbHcd->isBadDevice)
            {
                diag_printf("A bad device found, cnt = %d\n", g_pUsbHcd->badDeviceCnt);
                break;
            }

            kk = 0;
            while(kk<HUB_DEBOUNCE_STABLE) // 100, debouncing?
            {
                USBCriticalSectionIn(0);
                isConnect = MDrv_UsbDeviceConnect(g_pUsbHcd);
                USBCriticalSectionOut(0);
                if ( !isConnect )
                {
                    MsOS_DelayTask(600);  //By Jonas! for hub event 
                    diag_printf("USB port disconnect\n");
                    //if (isExtHubConnect)
                        hub_poll(); //for disconnect hub event
                    goto PORT_DISCONNECT;
                }
                kk++;
                MsOS_DelayTask(HUB_DEBOUNCE_STEP); // mimic Linux behavior
            }
#if 0
            if (!MDrv_UsbDeviceConnect())
            {
                hub_poll(); //for disconnect hub event
                break;
            }
#endif
            // Device is connecting to the port
            isFirstEvent = FALSE;
            if (has_hub_events(&PortStatus))
            {
                //diag_printf("hub event\n");
                isFirstEvent = TRUE;
                hub_poll();

#if 0
                if ( PortStatus & USB_PORT_STAT_CONNECTION )
                {
                    usb_get_connected_dev_state(&DevState, &DevClass);

                    if (DevClass == 9)
                    {
                        diag_printf("External Hub is connected\n");
                        isExtHubConnect = TRUE;
                    }
                }
#endif
            }

            if (isExtHubConnect)
                hub_poll();

            isMSCPlugIn = FALSE;
            for (us_id=0; us_id<MAX_USTOR; us_id++)
            {
                if (Is_Stor_Dev_Info_Valid(us_id))   // Check if we found a Mass Stoarge device.
                {
                    isMSCPlugIn = TRUE;
                    if (!Is_Stor_Dev_Init(us_id))    // First time after connected
                    {
                        diag_printf("found a Mass Storage device [%x], try to init it\n", us_id);

                        if (bInit_USB_Disk(us_id))
                        {
                            for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                            {
                                LunSts[ii] = LunPreSts[ii] = bIsDevValid(us_id, ii);
                                if (LunSts[ii])
                                {
                                    diag_printf("LUN %d is init ok\n", ii);
                                }
                            }
                            Set_Stor_Dev_Init(us_id, TRUE);
                            diag_printf("MSC plug in\n");
                        }
                    }
                    else
                    {
                        vChk_USB_LUNs(us_id);

                        for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                        {
                            LunSts[ii] = bIsDevValid(us_id, ii);
                            if ( LunSts[ii] && (LunSts[ii] != LunPreSts[ii]) )
                            {
                                diag_printf("Chk LUN %d is init ok\n", ii);
                            }
                            LunPreSts[ii] = LunSts[ii];
                        }
                    }
                }
                else
                {
                    #if 0
                    if (isMSCPlugIn)                    
                    {
                        diag_printf("MSC plug out\n");
                        isMSCPlugIn = FALSE;
                        if ( _DrvUSB_CBFun != NULL )
                            _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_STOR);
                    }
                    #endif
                }

            }


                
        #if USB_HID_SUPPORT
            if (Is_Any_Hid_Dev_Info_Valid_EX())
            {
                gScanHIDDevices =FALSE;
                for (ii=0; ii<MAX_HID_DEV_COUNT; ii++)	//find the port index
                {
                    isHIDPlugIntoSamePort = FALSE;		//a bool value to indicate that the interface enumeration below is for the same port
		            isHIDPlugIn_IntfCnt = 0;
                    for (jj=0; jj<MAX_HID_INTF_COUNT; jj++)	//find the interface index
                    {
                        if(isHIDPlugIntoSamePort==FALSE)
			    isHIDPlugIntoSamePort = TRUE;	//for multiple interfaces, using same port

                        if ( Is_Hid_Dev_Info_Valid_EX(ii,jj) &&
				  (HID_us[ii][jj]->pusb_dev->bus->hcpriv == cpe_ehci_dev.dev.driver_data) )
                        {
                            if (!Is_Hid_Dev_Init_EX(ii,jj))
                            {
	                        if ((Invalid_HidDevs & ((1 << jj)<<(ii*MAX_HID_INTF_COUNT))) != 0)
                                {
			            continue;;
				}

                               if (usb_get_report_descriptor_EX(ii, jj, Hid_Report_Desc, 0x65) == ENOERR)
                                {
                   	    /* index 3 of id_Report_Desc is for Local Usage type: 1(Unknown), 2(Mouse),4(Joystick),6(Keyboard)
                   	    *  e.g. of Joystick: 5 1 9 "4" a1 1 85 ...
                   	    */
                   	    if(Hid_Report_Desc[3]==Usage_Unkown)
                                    {
       			        Invalid_HidDevs |= ((1 << jj)<<(ii * MAX_HID_INTF_COUNT));
                       		diag_printf("Invalid_HidDevs=%lx\n",Invalid_HidDevs);
                       		continue;
                                   }

                                    //hid_parse_report(Hid_Report_Desc, 0x65);
                                   usb_hid_parse_report(Hid_Report_Desc, 0x65, &HID_us[ii][jj]->buttons, HID_us[ii][jj]->axis, &HID_us[ii][jj]->keys);
                                    diag_printf("HID finished\n");
                                   Set_Hid_Dev_Init_EX(ii, jj, TRUE);

       			    #if 1  //RCC
       			        if ((isHIDPlugIn_IntfCnt < MAX_HID_INTF_COUNT)&& (isHIDPlugIntoSamePort == TRUE))
                                       {
                                           isHIDPlugIn = TRUE;
                                           isHIDPlugIn_IntfCnt++;
                                           diag_printf("HID plug into Port(%d) Intf(%d)\n", ii, jj);
                                           if ( _DrvUSB_CBFun != NULL )
                                               _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_HID, NULL);
                                       }
                   	#else
                                        if (!isHIDPlugIn)
                                        {
                                            isHIDPlugIn = TRUE;
                                            diag_printf("HID plug in\n");
                                            if ( _DrvUSB_CBFun != NULL )
                                                _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_HID, NULL);
                                        }
       			    #endif
                                //usb_hid_get_int_ex(0, HID_us[0]->IntPipeBuf, 8);
                                //MsOS_StartTimer(g_Hid_Timer_ID);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (isHIDPlugIn)
                {
                    diag_printf("HID plug out\n");
                    isHIDPlugIn = FALSE;
                    if ( _DrvUSB_CBFun != NULL )
                        _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_HID, NULL);
                }
            }
        #endif

        #if USB_CDC_SUPPORT/* Start of CDC check*/
    		if (Is_Any_Cdc_Dev_Info_Valid())
    		{
    			for (ii=0; ii<MAX_CDC_DEV_COUNT; ii++)	//find the port index
    			{
    				if (Is_Cdc_Dev_Info_Valid(ii) &&
    					(CDC_us[ii]->pusb_dev->bus->hcpriv == cpe_ehci_dev.dev.driver_data) )
    				{
    					if (!Is_Cdc_Dev_Init(ii))
    					{
    						if( Usb_Cdc_Dev_Init())
    						{						
    							diag_printf("CDC plug into Port(%d)\n", ii);
    							isCDCPlugIn = TRUE;							
    							Set_Cdc_Dev_Init(ii, TRUE);							
    							
    						}
    						else								
    							diag_printf("CDC plug into Port(%d),init faile\n", ii);

    						if ( _DrvUSB_CBFun != NULL )
    							_DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_CDC, NULL);
    					}
                        else                        
                            Usb_Cdc_Handle();

    					if (isFirstEvent && (CDC_us[ii]->pusb_dev->parent->parent != NULL))
    					{
    						diag_printf("External Hub is connected\n");
    						isExtHubConnect = TRUE;
    					}
    	
    				}
    			}
    		}
    		else
    		{
    			if (isCDCPlugIn)
    			{
    				diag_printf("CDC plug out\n");
    				isCDCPlugIn = FALSE;
    				if ( _DrvUSB_CBFun != NULL )
    					_DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_CDC, NULL);
    			}
    		}
        #endif/* END of CDC check*/


            if ( (isMSCPlugIn == FALSE)
                #if USB_HID_SUPPORT
                    && !Is_Any_Hid_Dev_Info_Valid_EX()
                #endif
                #if USB_CDC_SUPPORT				
				    && !Is_Any_Cdc_Dev_Info_Valid() 
                #endif
				&& isFirstEvent )
            {
                if ( PortStatus & USB_PORT_STAT_CONNECTION )
                {
                    //if ( usb_get_connected_dev_state(&DevState, &DevClass) && (DevState < USB_STATE_CONFIGURED) )
                    if (usb_get_connected_dev_state(&DevState, &DevClass, g_pUsbHcd))
                    {
                        if (DevState < USB_STATE_CONFIGURED)
                        {
                            diag_printf("Usb device no responding\n");
                        }
                        else
                        {
                            if (DevClass == 0x09)
                            {
                                diag_printf("External Hub is connected\n");
                                isExtHubConnect = TRUE;
                            }
                            else
                            {
                                diag_printf("Usb device not supported\n");
                                if ( _DrvUSB_CBFun != NULL )
                                    _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_UNKNOW, NULL);
                            }
                        }
                    }
                }

            }

            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[0].portNum == 0) && (pCurrentChip->usbc_ip[0].eventFlag))
                    if ( PortStatus & USB_PORT_STAT_CONNECTION)
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[0].eventType)                                    
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, 0, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, 0, NULL);
                            pCurrentChip->usbc_ip[0].eventFlag = 0;
                        }
            #endif
            #if USB_HID_SUPPORT
	        gScanHIDDevices =TRUE;
            #endif
            //MsOS_DelayTask(1000);
        }

PORT_DISCONNECT:
        diag_printf("No device is connecting\n");
        //MsOS_StopTimer(g_Hid_Timer_ID);

        if (has_hub_events(&PortStatus))
        {
            diag_printf("Has hub event .. @ port disconnect\n");
            hub_poll();
        }

#if USB_HID_SUPPORT        
        for (ii=0; ii<MAX_HID_DEV_COUNT; ii++)
        {
            for (jj=0; jj<MAX_HID_INTF_COUNT; jj++)		//find the interface index
            {
                if (Is_Hid_Dev_Info_Valid_EX(ii,jj))
                {
                    if (HID_us[ii][jj]->pusb_dev->bus->hcpriv == cpe_ehci_dev.dev.driver_data)
                    {
                        diag_printf("Set HID Port(%d) Intf(%d) not init\n", ii, jj);
                        Set_Hid_Dev_Init_EX(ii, jj, FALSE);
                    }
                }
            }
        }
#endif
        
#if USB_CDC_SUPPORT	
        /* set each bDevInit of CDC_us back to FALSE */
        for (ii=0; ii<MAX_CDC_DEV_COUNT; ii++)
        {
            if (Is_Cdc_Dev_Info_Valid(ii))
            {
                if (CDC_us[ii]->pusb_dev->bus->hcpriv == cpe_ehci_dev.dev.driver_data)
                {
                    diag_printf("Set CDC Port(%d) not init\n", ii);
                    Set_Cdc_Dev_Init(ii, FALSE);
                }
            }
        }
#endif

        #if 0
        if (isMSCPlugIn)
        {
            diag_printf("MSC plug out\n");
            if ( _DrvUSB_CBFun != NULL )
                _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_STOR);
        }
        #endif

        #if 0
        for (us_id=0; us_id<MAX_USTOR; us_id++)
        {
            if (Is_Stor_Dev_Info_Valid(us_id))
            {
                diag_printf("!!! Remove device:%d begin..", us_id);
                vRemove_DISK_LUNs(us_id);
                //Free memory
	            dissociate_dev(Mass_stor_us[us_id]);
	            usb_stor_release_resources(Mass_stor_us[us_id]);
                Clr_Stor_Dev_Info(us_id);
                diag_printf("OK\n");
            }
        }
        #endif

        #if USB_HID_SUPPORT
        if (isHIDPlugIn)
        {
            diag_printf("HID plug out\n");
            if ( _DrvUSB_CBFun != NULL )
                _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_HID, NULL);
        }
        #endif
        
        #if USB_CDC_SUPPORT
        if (isCDCPlugIn)
        {
            diag_printf("CDC plug out\n");            
            if ( _DrvUSB_CBFun != NULL )
                _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_CDC, NULL);
        }
        #endif
        //if get any Hub device, enter Hub handler

        //if get any MSC device, enter MSC handler

        //device is disconnected

        USBCriticalSectionIn(0);
        ResetMstarUsb(ehci);
        USBCriticalSectionOut(0);

    }
}
//struct usb_hcd *g_phcd;

void InitUSBIntr(struct usb_hcd * hcd)
{
    struct ehci_hcd  *ehci = hcd_to_ehci (hcd);
    U32 temp;

    diag_printf("InitUSBIntr enter\n");
    #if 0
    mwHost20_USBINTR_Set(HOST20_USBINTR_IntOnAsyncAdvance |
                                            HOST20_USBINTR_SystemError |
                                            HOST20_USBINTR_USBError |
                                            HOST20_USBINTR_CompletionOfTransaction);

    mwHost20_USBSTS_Set(mwHost20_USBSTS_Rd());//clear all pending interrupt
    //  diag_printf("usb int status:%x\n",mwHost20_USBSTS_Rd());
    mwOTG20_Control_Int_Polarity_Hi();
    #else
    temp = (HOST20_USBINTR_IntOnAsyncAdvance |
                                            HOST20_USBINTR_SystemError |
#ifdef ROOTHUB_INTERRUPT_MODE
                                            HOST20_USBINTR_PortChangeDetect |
#endif
                                            HOST20_USBINTR_USBError |
                                            HOST20_USBINTR_CompletionOfTransaction);
    ehci_writel (temp, (U32)&ehci->regs->intr_enable);    

    temp = ehci_readb((U32)&ehci->regs->status);
    ehci_writel (temp, (U32)&ehci->regs->status); //clear all pending interrupt

    temp = ehci_readb((U32)&ehci->regs->bus_control);
    temp|= INT_POLAR;
    ehci_writel (temp, (U32)&ehci->regs->bus_control); // set interrupt polarity high
    #endif

#ifndef  MS_NOSAPI
    //diag_printf("attach usb int\n");
	MsOS_AttachInterrupt(gIrqUHC, _DrvUSB_OnInterrupt);
    //diag_printf("enable usb int\n");
	MsOS_EnableInterrupt(gIrqUHC);
    //diag_printf("ok");
#endif
}

static void _DrvUSB_OnInterrupt(InterruptNum eIntNum)
{
    //MS_U8		OldUSBEchiID;

    //MS_U8 status;
    //gIntStatus=mwHost20_USBSTS_Rd();
    //diag_printf("USB INT:%x\n",gIntStatus);
    //mwHost20_USBSTS_Set(gIntStatus);         //clear usb intr status
    MsOS_DisableInterrupt(gIrqUHC);
    //MsOS_SetEvent(_s32UsbEventId,gIntStatus);

    //OldUSBEchiID = MDrv_USBGetCurntEhciNum();
    //MDrv_USBSelectEhci(1);
    g_pUsbHcd->driver->irq(g_pUsbHcd, NULL);
    //MDrv_USBSelectEhci(OldUSBEchiID);

    MsOS_EnableInterrupt(gIrqUHC);
}
//extern S32 gs32NonCachedPoolID_MIU0;
#if 0
void TestUsbDisk(void)
{
U8   *buf1,*buf2;
U32  ii,k;

        buf1=(U8*)MsOS_AllocateMemory(0x10000, gs32NonCachedPoolID_MIU0 );//make 64K buffer for testing
        buf2=(U8*)MsOS_AllocateMemory(0x10000, gs32NonCachedPoolID_MIU0 );//make 64K buffer for testing



        for (ii=0 ; ii< 0x10000 ; ii++)
            buf1[ii]=(U8)ii & 0xff;             //make test pattern;

        k=0;
       while (1)
        {

           for (ii=0 ; ii<0x10000; ii++)
              buf2[ii]=0;

           bSCSI_Write_10(0, 0x200, 0x80,buf1);
           bSCSI_Read_10(0,0x200,0x80,buf2);

           for (ii=0 ; ii< 0x10000 ; ii++)
              if (buf1[ii]!=buf2[ii])
                {
                    k=99999999;
                    while(1) {}         //miscompare....

                }
           // buf1[ii]=(U8)ii & 0xff;             //make test pattern;
           k++;
        }

        MsOS_FreeMemory((void *)buf1,  gs32NonCachedPoolID_MIU0);
        MsOS_FreeMemory((void *)buf2,  gs32NonCachedPoolID_MIU0);
}
#endif

void USBCriticalSectionIn(MS_U8 Port)
{
    if (Port == 0)
        MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
    else if (Port == 2)
        MsOS_ObtainMutex(_s32MutexUSB_Port2, MSOS_WAIT_FOREVER);
    else if (Port == 1)
        MsOS_ObtainMutex(_s32MutexUSB_Port1, MSOS_WAIT_FOREVER);
    else if (Port == 3)
        MsOS_ObtainMutex(_s32MutexUSB_Port3, MSOS_WAIT_FOREVER);

    lock_usb_core();
}

void USBCriticalSectionOut(MS_U8 Port)
{
    unlock_usb_core();
    
    if (Port == 0)
        MsOS_ReleaseMutex(_s32MutexUSB);
    else if (Port == 2)
        MsOS_ReleaseMutex(_s32MutexUSB_Port2);
    else if (Port == 1)
        MsOS_ReleaseMutex(_s32MutexUSB_Port1);
    else if (Port == 3)
        MsOS_ReleaseMutex(_s32MutexUSB_Port3);    
}

MS_U32 MDrv_GetUsbBlockSize(MS_U8 lun)
{
    MS_U32 uBlkSize = 0;

    if ( (Mass_stor_us[0] != NULL) && (lun <= Mass_stor_us[0]->max_lun) )
    {
        uBlkSize = Mass_stor_us[0]->Mass_stor_device[lun].u32BlockSize;
    }

    return uBlkSize;
}

MS_U32 MDrv_GetUsbBlockSizeEx(MS_U8 uPort, MS_U8 lun)
{
    MS_U32 uBlkSize = 0;

    if ( (Mass_stor_us[uPort] != NULL) && (lun <= Mass_stor_us[uPort]->max_lun) )
    {
        uBlkSize = Mass_stor_us[uPort]->Mass_stor_device[lun].u32BlockSize;
    }

    return uBlkSize;
}

MS_U32 MDrv_GetUsbBlockNum(MS_U8 lun)
{
    MS_U32 uTotalBlks = 0;

    if ( (Mass_stor_us[0] != NULL) && (lun <= Mass_stor_us[0]->max_lun) )
    {
        uTotalBlks = Mass_stor_us[0]->Mass_stor_device[lun].u32BlockTotalNum;
    }

    return uTotalBlks;
}

MS_U32 MDrv_GetUsbBlockNumEx(MS_U8 uPort, MS_U8 lun)
{
    MS_U32 uTotalBlks = 0;

    if ( (Mass_stor_us[uPort] != NULL) && (lun <= Mass_stor_us[uPort]->max_lun) )
    {
        uTotalBlks = Mass_stor_us[uPort]->Mass_stor_device[lun].u32BlockTotalNum;
    }

    return uTotalBlks;
}

MS_U8 MDrv_UsbGetMaxLUNCount(void)
{
    if (Mass_stor_us[0] != NULL)
        return Mass_stor_us[0]->max_lun + 1;
    else
        return 0;
}

MS_U8 MDrv_UsbGetMaxLUNCountEx(MS_U8 uPort)
{
    if (Mass_stor_us[uPort] != NULL)
        return Mass_stor_us[uPort]->max_lun + 1;
    else
        return 0;
}

MS_BOOL MDrv_UsbBlockReadToMIU(
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return bSCSI_Read_10(0, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockReadToMIUEx(
    MS_U8 uPort,
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return bSCSI_Read_10(uPort, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockWriteFromMIU(
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return bSCSI_Write_10(0, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockWriteFromMIUEx(
    MS_U8 uPort,
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return bSCSI_Write_10(uPort, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbIsLunConnected(MS_U8 uPort, MS_U8 lun)
{
    struct LUN_Device* LunDevice;

    if (Mass_stor_us[uPort] == NULL)
        return FALSE;

    LunDevice = Mass_stor_us[uPort]->Mass_stor_device;

    if (lun <= Mass_stor_us[uPort]->max_lun)
        return (LunDevice[lun].bFSInit);
    else
        return FALSE;

}

// ------------------------------------------------------------------------


extern MS_U8 u8HubStackBuffer_Port2[HUB_STACK_SIZE];

struct s_gVar4UsbPort gVar4UsbPort0 =
{
    "USB Hub Task",
    0,
    { 0*MAX_USTOR, 1*MAX_USTOR},
    "cpe_ehci",
    "CPE_AMBA EHCI",
    {CPE_DEVID_USB, "CPE_EHCI HC"},
    u8HubStackBuffer,
};

struct s_gVar4UsbPort gVar4UsbPort2 =
{
    "USB Hub Task 2",
    2,
    { 2*MAX_USTOR, 3*MAX_USTOR}, // TODO: re-arrange    
    "cpe_ehci_2",
    "CPE_AMBA EHCI 2",    
    {CPE_DEVID_USB_PORT2, "CPE_EHCI HC 2"},
    u8HubStackBuffer_Port2,
};

#ifndef  MS_NOSAPI
static MS_U8 u8HubStackBuffer_Port1[HUB_STACK_SIZE];
#endif
struct s_gVar4UsbPort gVar4UsbPort1 =
{
    "USB Hub Task 1",
    1,
    { MAX_USTOR, 2*MAX_USTOR},
    "cpe_ehci_1",
    "CPE_AMBA EHCI 1",
    {CPE_DEVID_USB_PORT1, "CPE_EHCI HC 1"},
    u8HubStackBuffer_Port1,
};

#ifndef  MS_NOSAPI
static MS_U8 u8HubStackBuffer_Port3[HUB_STACK_SIZE];
#endif
struct s_gVar4UsbPort gVar4UsbPort3 =
{
    "USB Hub Task 3",
    3,
    { 3*MAX_USTOR, 4*MAX_USTOR},
    "cpe_ehci_3",
    "CPE_AMBA EHCI 3",    
    {CPE_DEVID_USB_PORT3, "CPE_EHCI HC 3"},
    u8HubStackBuffer_Port3,
};

/* USB host declaration by chip ID */
struct s_ChipUsbHostDef chipURANUS4 =
{
    CHIPID_URANUS4,
    "URANUS4",
    3,
    {
    {0, BASE_UTMI0, BASE_UHC0, BASE_USBC0, E_IRQ_UHC, E_IRQ_USBC},
    {0, BASE_UTMI1, BASE_UHC1, BASE_USBC1, E_IRQ_UHC1, E_IRQ_USBC1},        
    {EHCFLAG_DPDM_SWAP, BASE_UTMI2, BASE_UHC2, BASE_USBC2, E_IRQ_UHC2, E_IRQ_USBC2},    
    },    
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2}
};

struct s_ChipUsbHostDef chipKRONUS =
{
    CHIPID_KRONUS,
    "KRONUS",
    2,
    {
    {EHCFLAG_DPDM_SWAP, BASE_UTMI0, BASE_UHC0, BASE_USBC0, E_IRQ_UHC, E_IRQ_USBC},
    {EHCFLAG_DPDM_SWAP, BASE_UTMI2, BASE_UHC2, BASE_USBC2, E_IRQ_UHC2, E_IRQ_USBC2},    
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, }
};

struct s_ChipUsbHostDef chipKAISERIN  =
{
    CHIPID_KAISERIN,
    "KAISERIN",
    4,
    {
    {0, BASE_UTMI0, BASE_UHC0, BASE_USBC0, E_IRQ_UHC, E_IRQ_USBC},
    {0, BASE_UTMI1, BASE_UHC1, BASE_USBC1, E_IRQ_UHC1, E_IRQ_USBC1},    
    {0, BASE_UTMI2, BASE_UHC2, BASE_USBC2, E_IRQ_UHC2, E_IRQ_USBC2},    
    {0, BASE_UTMI3, BASE_UHC3, BASE_USBC3, E_IRQ_UHC3, E_IRQ_USBC3},        
    },
    {&gVar4UsbPort0, &gVar4UsbPort1, &gVar4UsbPort2, &gVar4UsbPort3}
};

// any new chip added here ^^^
//

static void _DrvUSB_OnInterrupt_EX(InterruptNum eIntNum)
{
    // TODO: check if there is any alternative
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
    hcd->driver->irq(hcd, NULL);
    MsOS_EnableInterrupt(eIntNum);
}

void InitUSBIntr_EX(struct usb_hcd * hcd)
{
    struct ehci_hcd  *ehci = hcd_to_ehci (hcd);
    U32 temp;

    //diag_printf("InitUSBIntr_EX enter\n");
    temp = (HOST20_USBINTR_IntOnAsyncAdvance |
                                            HOST20_USBINTR_SystemError |
#ifdef ROOTHUB_INTERRUPT_MODE
                                            HOST20_USBINTR_PortChangeDetect |
#endif                                            
                                            HOST20_USBINTR_USBError |
                                            HOST20_USBINTR_CompletionOfTransaction);
    ehci_writel (temp, (U32)&ehci->regs->intr_enable);    

    temp = ehci_readb((U32)&ehci->regs->status);
    ehci_writel (temp, (U32)&ehci->regs->status); //clear all pending interrupt
    //  diag_printf("usb int status:%x\n",mwHost20_USBSTS_Rd());

    temp = ehci_readb((U32)&ehci->regs->bus_control);
    temp|= INT_POLAR;
    ehci_writel (temp, (U32)&ehci->regs->bus_control);

#ifndef  MS_NOSAPI
	MsOS_AttachInterrupt(hcd->irq, _DrvUSB_OnInterrupt_EX);
    //diag_printf("enable usb int\n");
	MsOS_EnableInterrupt(hcd->irq);
    //diag_printf("ok");
#endif
}

MS_BOOL Usb_host_Init_EX(struct usb_hcd *hcd)
{
    MS_U8 wValue;
    MS_U16 wTimer_ms;
    struct ehci_hcd  *ehci = hcd_to_ehci (hcd);
    U32 temp, temp2;    
    const struct device_s *__mptr = hcd->controller;
    struct cpe_dev *cdev;

    diag_printf("Usb_host_Init_EX......\thcd %x, port %d\n", hcd, hcd->host_id);

   //<1>.Waiting for the Device connect
    temp = ehci_readl((U32)&ehci->regs->command);
    temp |= CMD_RESET;
    ehci_writel (temp, (U32)&ehci->regs->command); //host controller reset
    while(ehci_readb((U32)&ehci->regs->command) & CMD_RESET);
      //mbHost20_USBCMD_HCReset_Set();            //host controller reset
      //while(mbHost20_USBCMD_HCReset_Rd()>0);

   wValue=0;
   wTimer_ms=0;
   do
   {
        wValue = ehci_readb((U32)&ehci->regs->port_status[0]) & PORT_CONNECT;
      //wValue=mwHost20_PORTSC_ConnectStatus_Rd();
            //diag_printf("wValue:%x\n",wValue);

      if (wValue==0) {
         USB_DELAY(1);//10, wait 1 ms
         wTimer_ms++;
      }
      //if (mwHost20_PORTSC_ConnectStatus_Rd()==0) return 0;
      if (wTimer_ms>9500)          // Case1:Waiting for 10 sec=10000
      {                                           // Case2:Waiting for 100 ms =100
         diag_printf("??? Waiting for Peripheral Connecting Fail...\n");
         return (0);
      }
   }while(wValue==0);

    cdev = (struct cpe_dev *)( (char *)__mptr - (char *)offsetof(struct cpe_dev,dev) );
    
    //if (PortReset(UTMIBaseAddr, UHC_BASE)>0)
    if (PortReset(cdev->utmibase, (U32)hcd->regs)>0)
        return(2);

    temp = (ehci_readl((U32)&ehci->regs->bus_control) >> 9) & 0x3;
    diag_printf("speed: %x\n", temp);
    
    //diag_printf("speed: %x\n", mwOTG20_Control_HOST_SPD_TYP_Rd());    

    temp2 = ehci_readl((U32)&ehci->regs->hcmisc) & 0xfffffff3;
    if (temp == 2) // high speed
    //if (mwOTG20_Control_HOST_SPD_TYP_Rd()==2)                //high speed
    {
      UTMI_ORXBYTE_EX(0x09, 0x80, cdev->utmibase);                            //HS rx robust enable
      temp2 |= (3 << 2);
      //mwHost20_Misc_EOF1Time_Set(3);
    }
    else                 //full speed, low speed
    {
      UTMI_ANDXBYTE_EX(0x09, 0x7F, cdev->utmibase);
      temp2 |= (2 << 2);
      //mwHost20_Misc_EOF1Time_Set(2);
    }
    ehci_writel (temp2, (U32)&ehci->regs->hcmisc); // misc, EOF1

    temp = ehci_readl((U32)&ehci->regs->command) & 0xfffffff3;
    temp |= (HOST20_USBCMD_FrameListSize_256 << 2);
    ehci_writel (temp, (U32)&ehci->regs->command);

    temp = (HOST20_USBINTR_IntOnAsyncAdvance |
                                            HOST20_USBINTR_SystemError |
#ifdef ROOTHUB_INTERRUPT_MODE
                                            HOST20_USBINTR_PortChangeDetect |
#endif                                            
                                            HOST20_USBINTR_USBError |
                                            HOST20_USBINTR_CompletionOfTransaction);
    ehci_writel (temp, (U32)&ehci->regs->intr_enable);        
    //mwHost20_USBINTR_Set(HOST20_USBINTR_IntOnAsyncAdvance |
    //                                        HOST20_USBINTR_SystemError |
    //                                        HOST20_USBINTR_USBError |
    //                                        HOST20_USBINTR_CompletionOfTransaction);

   return (1);
    
}

extern void ResetMstarUsb(struct ehci_hcd *ehci);
MS_BOOL MDrv_UsbDeviceConnect_EX(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    MS_U8 temp_s;
    MS_U32 temp;
    static MS_U32 usbStartTime = 0;

    temp_s = ehci_readb((U32)&ehci->regs->port_status[0]);
    //temp=mwHost20_PORTSC_ConnectStatus_Rd_Port2();
    //diag_printf("ConnectStatus:%02bx\n",temp);
    //MsOS_DelayTask(10);
    if (temp_s & PORT_CONNECT)
    {
        return TRUE;
    }
    else
    {
#ifndef MS_NOSAPI
        if (MsOS_GetSystemTime()-usbStartTime > 1000 )
        {
            usbStartTime=MsOS_GetSystemTime();

            temp = ehci_readl((U32)&ehci->regs->bus_control);
            temp &= ~0x80;
            ehci_writel(temp, (U32)&ehci->regs->bus_control);
            //UHC2_ANDXBYTE(0x40, 0x7F);//disable force enter FSmode
            ResetMstarUsb(ehci);
        }
#endif
        hcd->isRootHubPortReset = TRUE;
        hcd->isBadDevice = FALSE;
        hcd->badDeviceCnt = 0;
        return FALSE;
    }
}

extern BOOL has_hub_events_EX(U16 *pPortStatus, struct s_gVar4UsbPort *gVar);
extern void hub_poll_EX(struct s_gVar4UsbPort *gVar);
extern struct cpe_dev cpe_ehci_dev_Port2; // for HID support
extern struct list_head hub_event_list_Port2;
extern struct list_head hub_event_list_Port1;
extern struct list_head hub_event_list_Port3;
void  UsbTask_EX(MS_U32 argc, VOID *argv)
{
    MS_U16	    PortStatus;
    MS_BOOL     isExtHubConnect, isFirstEvent, isConnect;
    int		    DevState;
    MS_U8       DevClass, ii, kk;
    MS_U8       LunSts[MAX_USB_STOR_LUNS];
    MS_U8       LunPreSts[MAX_USB_STOR_LUNS];
    MS_BOOL     isMSCPlugIn; 
    #if USB_HID_SUPPORT
    MS_U8       jj;
    MS_BOOL     isHIDPlugIn, isHIDPlugIntoSamePort;
    MS_U8       isHIDPlugIn_IntfCnt;
    #endif
    MS_U8       us_id; // for MSC
    struct s_gVar4UsbPort *gVar = (struct s_gVar4UsbPort *) argc;
    struct ehci_hcd *ehci = hcd_to_ehci(gVar->p_UsbHcd);

    diag_printf("UsbTask EX ... port %d\n", gVar->portNum);
    diag_printf("gVar = %x\n", gVar);
    while (1)
    {
        isExtHubConnect = FALSE;
        isMSCPlugIn = FALSE;
        #if USB_HID_SUPPORT
        isHIDPlugIn = FALSE;
        #endif
        
        // Waiting for USB port connection
        while(1)
        {
            USBCriticalSectionIn(gVar->portNum);
            isConnect = MDrv_UsbDeviceConnect_EX(gVar->p_UsbHcd);
            USBCriticalSectionOut(gVar->portNum);
            
            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[gVar->portNum].portNum == gVar->portNum) && (pCurrentChip->usbc_ip[gVar->portNum].eventFlag))
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[gVar->portNum].eventType)                                    
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, gVar->portNum, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, gVar->portNum, NULL);
                            pCurrentChip->usbc_ip[gVar->portNum].eventFlag = 0;
                        }
            #endif
            if(isConnect)
                break;
            MsOS_DelayTask(100);
        }

        // removing any delay before USB bus reset
        //MsOS_DelayTask(1000);
        //diag_printf("USB Port %d is connect\n", gVar->portNum);

        while (1)
        {
            if (gVar->p_UsbHcd->isBadDevice)
            {
                diag_printf("A bad device found on port %d\n", gVar->portNum);
                break;
            }

            kk = 0;
            while(kk<HUB_DEBOUNCE_STABLE)
            {
                USBCriticalSectionIn(gVar->portNum);
                isConnect = MDrv_UsbDeviceConnect_EX(gVar->p_UsbHcd);
                USBCriticalSectionOut(gVar->portNum);
                if ( !isConnect )
                    goto PORT_DISCONNECT_EX;
                kk++;
                MsOS_DelayTask(HUB_DEBOUNCE_STEP);
            }

            // Device is connecting to the port
            isFirstEvent = FALSE;
            if (has_hub_events_EX(&PortStatus, gVar))
            {
                diag_printf("EX**hub event %d\n", gVar->portNum);
                isFirstEvent = TRUE;
                hub_poll_EX(gVar);
            }

            if (isExtHubConnect)
                hub_poll_EX(gVar);

            isMSCPlugIn = FALSE;
            for (us_id=gVar->vPortRange.vPortStart; us_id<gVar->vPortRange.vPortEnd; us_id++)
            {
                if (Is_Stor_Dev_Info_Valid(us_id))   // Check if we found a Mass Stoarge device.
                {
                    isMSCPlugIn = TRUE;
                    if (!Is_Stor_Dev_Init(us_id))    // First time after connected
                    {
                        diag_printf("found a Mass Storage device @ port %d, try to init it\n", gVar->portNum);

                        if (bInit_USB_Disk(us_id))
                        {
                            for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                            {
                                LunSts[ii] = LunPreSts[ii] = bIsDevValid(us_id, ii);
                                if (LunSts[ii])
                                {
                                    diag_printf("LUN %d is init ok\n", ii);
                                }
                            }
                            Set_Stor_Dev_Init(us_id, TRUE);
                            diag_printf("MSC plug in\n");
                        }
                    }
                    else
                    {
                        vChk_USB_LUNs(us_id);

                        for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                        {
                            LunSts[ii] = bIsDevValid(us_id, ii);
                            if ( LunSts[ii] && (LunSts[ii] != LunPreSts[ii]) )
                            {
                                diag_printf("Chk LUN %d is init ok\n", ii);
                            }
                            LunPreSts[ii] = LunSts[ii];
                        }
                    }
                }
                else
                {
                }

            }


#if USB_HID_SUPPORT
            if (Is_Any_Hid_Dev_Info_Valid_EX())
            {
                for (ii=0; ii<MAX_HID_DEV_COUNT; ii++)	//find the port index
                {
                    isHIDPlugIntoSamePort = FALSE;		//a bool value to indicate that the interface enumeration below is for the same port
		            isHIDPlugIn_IntfCnt = 0;
                    for (jj=0; jj<MAX_HID_INTF_COUNT; jj++)	//find the interface index
                    {
                        if(isHIDPlugIntoSamePort==FALSE)
			                isHIDPlugIntoSamePort = TRUE;	//for multiple interfaces, using same port

                        if ( Is_Hid_Dev_Info_Valid_EX(ii,jj) &&
				  (HID_us[ii][jj]->pusb_dev->bus->hcpriv == cpe_ehci_dev_Port2.dev.driver_data) )
                        {
                            if (!Is_Hid_Dev_Init_EX(ii,jj))
                            {
	                        if ((Invalid_HidDevs & ((1 << jj)<<(ii*MAX_HID_INTF_COUNT))) != 0)
                                {
			            continue;;
				}

                               if (usb_get_report_descriptor_EX(ii, jj, Hid_Report_Desc, 0x65) == ENOERR)
                                {
                   	    /* index 3 of id_Report_Desc is for Local Usage type: 1(Unknown), 2(Mouse),4(Joystick),6(Keyboard)
                   	    *  e.g. of Joystick: 5 1 9 "4" a1 1 85 ...
                   	    */
                   	    if(Hid_Report_Desc[3]==Usage_Unkown)
                                    {
       			        Invalid_HidDevs |= ((1 << jj)<<(ii * MAX_HID_INTF_COUNT));
                       		diag_printf("Invalid_HidDevs=%lx\n",Invalid_HidDevs);
                       		continue;
                                   }

                                    //hid_parse_report(Hid_Report_Desc, 0x65);
                                   usb_hid_parse_report(Hid_Report_Desc, 0x65, &HID_us[ii][jj]->buttons, HID_us[ii][jj]->axis, &HID_us[ii][jj]->keys);
                                    diag_printf("HID finished\n");
                                   Set_Hid_Dev_Init_EX(ii, jj, TRUE);

       			    #if 1  //RCC
       			        if ((isHIDPlugIn_IntfCnt < MAX_HID_INTF_COUNT)&& (isHIDPlugIntoSamePort == TRUE))
                                       {
                                           isHIDPlugIn = TRUE;
                                           isHIDPlugIn_IntfCnt++;
                                           diag_printf("HID plug into Port(%d) Intf(%d)\n", ii, jj);
                                           if ( _DrvUSB_CBFun != NULL )
                                               _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_HID, NULL);
                                       }
                   	    #else
                                        if (!isHIDPlugIn)
                                        {
                                            isHIDPlugIn = TRUE;
                                            diag_printf("HID plug in\n");
                                            if ( _DrvUSB_CBFun != NULL )
                                                _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_HID, NULL);
                                        }
       			    #endif
                                //usb_hid_get_int_ex(0, HID_us[0]->IntPipeBuf, 8);
                                //MsOS_StartTimer(g_Hid_Timer_ID);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (isHIDPlugIn)
                {
                    diag_printf("HID plug out\n");
                    isHIDPlugIn = FALSE;
                    if ( _DrvUSB_CBFun != NULL )
                        _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_HID, NULL);
                }
            }
#endif


            if ( (isMSCPlugIn == FALSE)
                #if USB_HID_SUPPORT
                    && !Is_Any_Hid_Dev_Info_Valid_EX() 
                #endif                    
                    && isFirstEvent )
            {
                if ( PortStatus & USB_PORT_STAT_CONNECTION )
                {
                    //if ( usb_get_connected_dev_state(&DevState, &DevClass) && (DevState < USB_STATE_CONFIGURED) )
                    if ( usb_get_connected_dev_state(&DevState, &DevClass, gVar->p_UsbHcd) )
                    {
                        if (DevState < USB_STATE_CONFIGURED)
                        {
                            diag_printf("EX> Usb device no responding\n");
                        }
                        else
                        {
                            if (DevClass == 0x09)
                            {
                                diag_printf("EX> External Hub is connected\n");
                                isExtHubConnect = TRUE;
                            }
                            else
                            {
                                diag_printf("EX> Usb device not supported\n");                            
                                if ( _DrvUSB_CBFun != NULL )
                                    _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_UNKNOW, NULL);
                            }
                        }
                    }
                }

            }

            #if USBC_IP_SUPPORT // USBC IP control
            if ((pCurrentChip->usbc_ip[gVar->portNum].portNum == gVar->portNum) && (pCurrentChip->usbc_ip[gVar->portNum].eventFlag))
                    if ( PortStatus & USB_PORT_STAT_CONNECTION)
                        if (_DrvUSBC_CBFun)
                        {
                            if (pCurrentChip->usbc_ip[gVar->portNum].eventType)                                    
                                _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, gVar->portNum, NULL);
                            else
                                _DrvUSBC_CBFun(USBC_OVER_CURRENT, gVar->portNum, NULL);
                            pCurrentChip->usbc_ip[gVar->portNum].eventFlag = 0;
                        }
            #endif

            //MsOS_DelayTask(1000);
        }

PORT_DISCONNECT_EX:
        diag_printf("USB port %d disconnect\n", gVar->portNum);
        MsOS_DelayTask(600);  //By Jonas! for hub event!                          
        hub_poll_EX(gVar); //for disconnect hub event
        
        #if USB_HID_SUPPORT                
        for (ii=0; ii<MAX_HID_DEV_COUNT; ii++)
        {
            for (jj=0; jj<MAX_HID_INTF_COUNT; jj++)		//find the interface index
            {
                if (Is_Hid_Dev_Info_Valid_EX(ii,jj))
                {
                    if (HID_us[ii][jj]->pusb_dev->bus->hcpriv == cpe_ehci_dev_Port2.dev.driver_data)
                    {
                        diag_printf("Set HID Port(%d) Intf(%d) not init\n", ii, jj);
                        Set_Hid_Dev_Init_EX(ii, jj, FALSE);
                    }
                }
            }
        }
        #endif        

        #if USB_HID_SUPPORT
        if (isHIDPlugIn)
        {
            diag_printf("HID plug out\n");
            if ( _DrvUSB_CBFun != NULL )
                _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_HID, NULL);
        }
        #endif    
        //if get any Hub device, enter Hub handler

        //if get any MSC device, enter MSC handler

        //device is disconnected

        USBCriticalSectionIn(gVar->portNum);
        ResetMstarUsb(ehci);
        USBCriticalSectionOut(gVar->portNum);

    }
}

S32 Create_USB_Mutex(U8 port)
{
    S32 temp = -1;

    if (port == 0)
        temp = _s32MutexUSB = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX", MSOS_PROCESS_SHARED);
    else if (port == 2)
       temp =  _s32MutexUSB_Port2 = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX_PORT2", MSOS_PROCESS_SHARED);
    else if (port == 1)
        temp = _s32MutexUSB_Port1 = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX_PORT1", MSOS_PROCESS_SHARED);
    else if (port == 3)
        temp = _s32MutexUSB_Port3 = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX_PORT3", MSOS_PROCESS_SHARED);
    return temp;
}

void Delete_USB_Mutex(U8 port)
{
    if (port == 0)
        MsOS_DeleteMutex(_s32MutexUSB);
    else if (port == 2)
       MsOS_DeleteMutex(_s32MutexUSB_Port2);
    else if (port == 1)
        MsOS_DeleteMutex(_s32MutexUSB_Port1);
    else if (port == 3)
        MsOS_DeleteMutex(_s32MutexUSB_Port3);
}

void USB_Start_EX(struct s_gVar4UsbPort *gVar)
{
#ifndef  MS_NOSAPI
    MS_U8 *HubStack;
    int pid;
#endif

    diag_printf("\nUsb start EX..., gVar = %x\n\n", gVar);

#ifndef  MS_NOSAPI
    HubStack = gVar->u8pHubStackBuffer;

    if (Create_USB_Mutex(gVar->portNum) < 0)
    {
        GEN_EXCEP;
        return;
    }

    //Create Task
    pid = MsOS_CreateTask((TaskEntry) UsbTask_EX,
                         (MS_U32)gVar,
                         E_TASK_PRI_HIGH,
                         TRUE,
                         HubStack,
                         HUB_STACK_SIZE,
                         gVar->name);
    if (pid < 0)
    {
        GEN_EXCEP;
        Delete_USB_Mutex(gVar->portNum);
        return;
    }
#else
    //UsbTask();
#endif //MS_NOSAPI
}

extern int __init ehci_cpe_dev_init_EX(struct cpe_dev * dev);
extern struct list_head hub_event_list;
extern int __init ehci_hcd_cpe_driver_init_EX(void);
MS_BOOL MDrv_USB_Port_Init_EX(MS_U8 u8PortNum)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    
    if (pChip->nRootHub <= u8PortNum)
    {
        diag_printf("Chip %s does not not support port %d\n", pChip->name, u8PortNum);
        return FALSE;
    }
    else
    {
        diag_printf("Init chip %s, port %d\n", pChip->name, u8PortNum);
        // bind chip reg definition with cpe_dev
        pChip->p_roothub[u8PortNum]->cpe_ehci_dev.mapbase = pChip->reg[u8PortNum].baseUHC;
        pChip->p_roothub[u8PortNum]->cpe_ehci_dev.utmibase = pChip->reg[u8PortNum].baseUTMI;
        pChip->p_roothub[u8PortNum]->cpe_ehci_dev.usbcbase = pChip->reg[u8PortNum].baseUSBC;        
        pChip->p_roothub[u8PortNum]->cpe_ehci_dev.intNum = pChip->reg[u8PortNum].uhcIRQ;
        if (u8PortNum == 0)
            pChip->p_roothub[u8PortNum]->p_hub_event = &hub_event_list;
        else if (u8PortNum == 1)
            pChip->p_roothub[u8PortNum]->p_hub_event = &hub_event_list_Port1;
        else if (u8PortNum == 2)
            pChip->p_roothub[u8PortNum]->p_hub_event = &hub_event_list_Port2;
        else if (u8PortNum == 3)
            pChip->p_roothub[u8PortNum]->p_hub_event = &hub_event_list_Port3;        
    }

    // do Usb Host initial
    U4_series_usb_init(pChip->reg[u8PortNum].baseUTMI, pChip->reg[u8PortNum].baseUSBC, pChip->reg[u8PortNum].baseUHC, pChip->reg[u8PortNum].iFlag);

    //ehci_hcd_cpe_driver_init_EX(); // register cpe_driver

    ehci_cpe_dev_init_EX(&pChip->p_roothub[u8PortNum]->cpe_ehci_dev); // initial cpe_dev
    if (pChip->p_roothub[u8PortNum]->cpe_ehci_dev.dev.driver_data == NULL)
    {
        diag_printf("port %d hcd not allocated!!!\n", u8PortNum);
        return FALSE;
    }                        
    pChip->p_roothub[u8PortNum]->p_UsbHcd = pChip->p_roothub[u8PortNum]->cpe_ehci_dev.dev.driver_data; // for MDrv_UsbDeviceConnect()
    
    USB_Start_EX(pChip->p_roothub[u8PortNum]);
    
    return TRUE;
}

extern struct usb_hcd *g_pUsbHcd_Port2;
struct usb_hcd *msc_get_hcd(U8 host_id)
{
    if ( (MDrv_USBGetChipID()== CHIPID_KRONUS) || (MDrv_USBGetChipID()== CHIPID_URANUS4))
    {
        if (host_id == 0)
            return g_pUsbHcd;
        else
            return g_pUsbHcd_Port2;
    }
    else
    {        
        return pCurrentChip->p_roothub[host_id]->p_UsbHcd;
    }
}

extern void init_usbc_intr(MS_U8 p);
void MDrv_OverCurrentDetect_RegisterCallBack (USBCallback pCallbackFn, MS_U8 port_mask)
{
#if USBC_IP_SUPPORT // USBC IP control
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    MS_U8 p;

    diag_printf("<MDrv_OverCurrentDetect_RegisterCallBack> %p, port_mask(%x)\n", pCallbackFn, port_mask);
    _DrvUSBC_CBFun = pCallbackFn;    
    for (p = 0; p < pChip->nRootHub; p++)
        if (port_mask & (1<<p))
            init_usbc_intr(p);
#else
    diag_printf("<MDrv_OverCurrentDetect_RegisterCallBack> NOT support!!! Please turn on USBC_IP_SUPPORT in drvUSB.h\n");
#endif        
}
