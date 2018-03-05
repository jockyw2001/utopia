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
#include "include/drvPorts.h"
#include "include/drvKernel.h"
#include "include/drvTimer.h"
#include "include/drvUSBMsg.h"

#include "drvHCD.h"
#include "drvMassStor.h"
#include "include/drvCPE_EHCI.h"
#include "MsCommon.h"
#include "drvUSB.h"
#include "include/drvIO.h"
#include "drvUSBHwCtl.h"
#include "include/drvCPE_AMBA.h"
#include "drvSCSI.h"
#include "drvEHCI.h"
#if USB_HID_SUPPORT
#include "drvHIDDev.h"
#endif

#define USBENT_DEBUG
#ifdef USBENT_DEBUG
#define USBENT_Dbg		diag_printf
#else
#define USBENT_Dbg(x,...)
#endif

#define USBENT_ERR		diag_printf

MS_BOOL UsbReady_Port2 = FALSE;

#ifndef  MS_NOSAPI
/*static*/ MS_U8 u8HubStackBuffer_Port2[HUB_STACK_SIZE];
#endif

extern struct usb_hcd *g_pUsbHcd_Port2;
#if USB_HID_SUPPORT
extern MS_U32  Invalid_HidDevs;
extern U8 Hid_Report_Desc[0x100];
extern void usb_hid_parse_report(
    U8 *start,
    U16 size,
    struct   button_struct *pButton,
    struct   axis_struct *pAxis,
    struct   key_struct *pKeys);
#endif
extern struct cpe_dev cpe_ehci_dev_Port2;

extern void USBCriticalSectionIn(MS_U8 Port);
extern void USBCriticalSectionOut(MS_U8 Port);

extern void hub_poll_Port2(void);
extern BOOL has_hub_events_Port2(U16 *pPortStatus);
extern int PortReset(unsigned int regUTMI, unsigned int regUHC);

extern S32 _s32MutexUSB_Port2;

extern MS_BOOL MDrv_UsbDeviceConnect_EX(struct usb_hcd *hcd);
extern void ResetMstarUsb(struct ehci_hcd *ehci);  // replace void ResetUsbHardware_Port2(void) and moved to drvUSBHwCtl.c
MS_BOOL MDrv_UsbDeviceConnect_Port2(struct usb_hcd *hcd)
{
    if (MDrv_UsbDeviceConnect_EX(hcd))
        return TRUE;
    else
        return(UsbReady_Port2 = FALSE);
}

extern struct s_ChipUsbHostDef *pCurrentChip;
extern USBCallback _DrvUSBC_CBFun;
extern BOOL usb_get_connected_dev_state(int *pdevstate, unsigned char *pDevClass, struct usb_hcd *hcd);
void  UsbTask_Port2(MS_U32 argc, VOID *argv)
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
    MS_U8       us_id;
    struct ehci_hcd *ehci = hcd_to_ehci(g_pUsbHcd_Port2);

    diag_printf("UsbTask 2...\n");
    while (1)
    {
        //UsbReady_Port2=FALSE; //move to MDrv_UsbDeviceConnect_Port2
        isExtHubConnect = FALSE;
        isMSCPlugIn = FALSE;
        #if USB_HID_SUPPORT
        isHIDPlugIn = FALSE;
        #endif
        
        // Waiting for USB port connection
        while(1)
        {
            USBCriticalSectionIn(2);
            isConnect = MDrv_UsbDeviceConnect_Port2(g_pUsbHcd_Port2);
            USBCriticalSectionOut(2);
            
            #if USBC_IP_SUPPORT // USBC IP control
            {
                U8 idx = 1;

                if (pCurrentChip->chipID == CHIPID_URANUS4)
                    idx = 2;
                if ((pCurrentChip->usbc_ip[idx].portNum == idx) && (pCurrentChip->usbc_ip[idx].eventFlag))
                            if (_DrvUSBC_CBFun)
                            {
                                if (pCurrentChip->usbc_ip[idx].eventType)                                    
                                    _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, idx, NULL);
                                else
                                    _DrvUSBC_CBFun(USBC_OVER_CURRENT, idx, NULL);
                                pCurrentChip->usbc_ip[idx].eventFlag = 0;
                            }
            }
            #endif
            if(isConnect)
                break;
            MsOS_DelayTask(100);
        }

        //MsOS_DelayTask(1000);
        //diag_printf("USB Port 2 is connect\n");

        while (1)
        {
            if (g_pUsbHcd_Port2->isBadDevice)
            {
                diag_printf("A bad device found on port 2\n");
                break;
            }

            kk = 0;
            while(kk<HUB_DEBOUNCE_STABLE)
            {
                USBCriticalSectionIn(2);
                isConnect = MDrv_UsbDeviceConnect_Port2(g_pUsbHcd_Port2);
                USBCriticalSectionOut(2);
                if ( !isConnect )
                {
                    MsOS_DelayTask(600);  //By Jonas! for hub event!                  
                    diag_printf("USB port 2 disconnect\n");
                    //if (isExtHubConnect)
                        hub_poll_Port2(); //for disconnect hub event
                    goto PORT_DISCONNECT;
                }
                kk++;
                MsOS_DelayTask(HUB_DEBOUNCE_STEP);
            }
#if 0
            if (!MDrv_UsbDeviceConnect_Port2())
            {
                hub_poll_Port2(); //for disconnect hub event
                break;
            }
#endif

            // Device is connecting to the port
            isFirstEvent = FALSE;
            if (has_hub_events_Port2(&PortStatus))
            {
                //diag_printf("hub event 2\n");
                isFirstEvent = TRUE;
                hub_poll_Port2();

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
                hub_poll_Port2();

            isMSCPlugIn = FALSE;
            for (us_id=2*MAX_USTOR; us_id<3*MAX_USTOR; us_id++)
            {
                if (Is_Stor_Dev_Info_Valid(us_id))   // Check if we found a Mass Stoarge device.
                {
                    isMSCPlugIn = TRUE;
                    if (!Is_Stor_Dev_Init(us_id))    // First time after connected
                    {
                        diag_printf("found a Mass Storage device 2, try to init it\n");

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
                    if ( usb_get_connected_dev_state(&DevState, &DevClass, g_pUsbHcd_Port2) )
                    {
                        if (DevState < USB_STATE_CONFIGURED)
                        {
                            diag_printf("P2> Usb device no responding\n");
                        }
                        else
                        {
                            if (DevClass == 0x09)
                            {
                                diag_printf("P2> External Hub is connected\n");
                                isExtHubConnect = TRUE;
                            }
                            else
                            {
                                diag_printf("P2> Usb device not supported\n");                            
                                if ( _DrvUSB_CBFun != NULL )
                                    _DrvUSB_CBFun(USB_PLUG_IN, USB_EVENT_DEV_TYPE_UNKNOW, NULL);                            
                            }
                        }
                    }
                }

            }

            #if USBC_IP_SUPPORT // USBC IP control
            {
                U8 idx = 1;

                if (pCurrentChip->chipID == CHIPID_URANUS4)
                    idx = 2;
                if ((pCurrentChip->usbc_ip[idx].portNum == idx) && (pCurrentChip->usbc_ip[idx].eventFlag))
                        if ( PortStatus & USB_PORT_STAT_CONNECTION)
                            if (_DrvUSBC_CBFun)
                            {
                                if (pCurrentChip->usbc_ip[idx].eventType)                                    
                                    _DrvUSBC_CBFun(USBC_NON_OVER_CURRENT, idx, NULL);
                                else
                                    _DrvUSBC_CBFun(USBC_OVER_CURRENT, idx, NULL);
                                pCurrentChip->usbc_ip[idx].eventFlag = 0;
                            }
            }
            #endif

            //MsOS_DelayTask(1000);
        }

PORT_DISCONNECT:
        diag_printf("No device is connecting @ port 2\n");
        if (has_hub_events_Port2(&PortStatus))
        {
            diag_printf("Has hub event 2.. @ port disconnect\n");
            hub_poll_Port2();
        }
        
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


        #if 0
        if (isMSCPlugIn)
        {
            diag_printf("MSC plug out\n");
            if ( _DrvUSB_CBFun != NULL )
                _DrvUSB_CBFun(USB_PLUG_OUT, USB_EVENT_DEV_TYPE_STOR);
        }
        #endif

        #if 0
        for (us_id=MAX_USTOR; us_id<MAX_USTOR*2; us_id++)
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
        //if get any Hub device, enter Hub handler

        //if get any MSC device, enter MSC handler

        //device is disconnected

        USBCriticalSectionIn(2);
        ResetMstarUsb(ehci);
        USBCriticalSectionOut(2);

    }
}

void USB_Start_Port2(void)
{
#ifndef  MS_NOSAPI
    MS_U8 *HubStack;
    int pid;
#endif

    diag_printf("Usb 2 start..\n");

#ifndef  MS_NOSAPI
    HubStack = u8HubStackBuffer_Port2;
    _s32MutexUSB_Port2 = MsOS_CreateMutex(E_MSOS_FIFO, "USB_MUTEX_PORT2", MSOS_PROCESS_SHARED);

    if (_s32MutexUSB_Port2<0)
    {
        GEN_EXCEP;
        return;
    }
#endif

#ifndef MS_NOSAPI
    //Create Task
    pid = MsOS_CreateTask((TaskEntry) UsbTask_Port2,
                         (MS_U32)NULL,
                         E_TASK_PRI_HIGH,
                         TRUE,
                         HubStack,
                         HUB_STACK_SIZE,
                         "USB Hub Task 2");
    if (pid < 0)
    {
        GEN_EXCEP;

        MsOS_DeleteMutex(_s32MutexUSB_Port2);
        //MsOS_FreeMemory(HubStack, gs32CachedPoolID);
        return;
    }
#else
    //UsbTask();
#endif //MS_NOSAPI
    return;
}

static void _DrvUSB_OnInterrupt_Port2(InterruptNum eIntNum)
{
    //MS_U8		OldUSBEchiID;

    //MS_U8 status;
    //gIntStatus=mwHost20_USBSTS_Rd();
    //diag_printf("USB INT:%x\n",gIntStatus);
    //mwHost20_USBSTS_Set(gIntStatus);         //clear usb intr status
    MsOS_DisableInterrupt(gIrqUHC2);

    //OldUSBEchiID = MDrv_USBGetCurntEhciNum();
    //MDrv_USBSelectEhci(2);
    g_pUsbHcd_Port2->driver->irq(g_pUsbHcd_Port2, NULL);
    //MDrv_USBSelectEhci(OldUSBEchiID);

    MsOS_EnableInterrupt(gIrqUHC2);
}

void InitUSBIntr_Port2(struct usb_hcd * hcd)
{
    struct ehci_hcd  *ehci = hcd_to_ehci (hcd);
    U32 temp;

    diag_printf("InitUSBIntr_Port2 enter\n");
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

#ifndef  MS_NOSAPI
    //diag_printf("attach usb int\n");
	MsOS_AttachInterrupt(gIrqUHC2, _DrvUSB_OnInterrupt_Port2);
    //diag_printf("enable usb int\n");
	MsOS_EnableInterrupt(gIrqUHC2);
    //diag_printf("ok");
#endif
}

