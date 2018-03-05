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
#if defined(MSOS_TYPE_NOS)

#include "../USBHostConfig.h"
#include "../../include/hal_arch.h"
#include "../../include/hal_cache.h"
#ifndef ATV_SERISE_USE
#include <stdio.h>
#endif
#include <string.h>
#ifndef ATV_SERISE_USE
#include "../../include/datatype.h"
#else
#include "UsbHostDataDef.h"
#endif
#include "../drvHostLib.h"
#include "../drvUsbMain.h"
#ifndef ATV_SERISE_USE

#endif
#ifdef ATV_SERISE_USE
#include "Board.h"
#include "common.h"
#include "uart.h"
#endif
#include "../../include/_drvUSB.h"
#include "../drvscsi.h"
#include "../../include/drvGlobal.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#define DEBUG_USB(msg)          //msg
#define  MINI_DEBUG2(x)         //x;
#define HOSTLIB_MSG(x)		MS_CRITICAL_MSG(x)

code UINT8 OTGH_GETDESCRIPTOR_DEVICE_8[]  = {0x80,0x06,0x00,0x01,0x00,0x00,0x08,0x00};
code UINT8 OTGH_GETDESCRIPTOR_DEVICE[]    = {0x80,0x06,0x00,0x01,0x00,0x00,0x12,0x00};
code UINT8 OTGH_GETDESCRIPTOR_CONFIG[]    = {0x80,0x06,0x00,0x02,0x00,0x00,0x08,0x00};

code UINT8 OTGH_SETADDRESS[]              = {0x00,0x05,03,0x00,0x00,0x00,0x00,0x00};

code UINT8 OTGH_SETCONFIGURATION[]        = {0x00,0x09,0x01,0x00,0x00,0x00,0x00,0x00};
code UINT8 OTGH_GETDESCRIPTOR_STR[]     = {0x80,0x06,0x00,0x03,0x00,0x00,0x02,0x00}; //Get String Descriptor
//code UINT8 OTGH_SET_FEATURE_OTG[]         = {0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00};
//code UINT8 OTGH_SETDESCRIPTOR_DEVICE[]    = {0x00,0x07,0x00,0x01,0x00,0x00,0x12,0x00}; //Set Device Descriptor
//code UINT8 OTGH_GETDESCRIPTOR_STR70[]     = {0x80,0x06,0x70,0x03,0x00,0x00,0x06,0x00}; //Get String80 Descriptor
//code UINT8 OTGH_SETDESCRIPTOR_STR70[]     = {0x00,0x07,0x70,0x03,0x00,0x00,0x06,0x00}; //Set String80 Descriptor
//code UINT8 OTGH_GETDESCRIPTOR_STR80[]     = {0x80,0x06,0x80,0x03,0x00,0x00,0x12,0x00}; //Get String80 Descriptor
//code UINT8 OTGH_SETDESCRIPTOR_STR80[]     = {0x00,0x07,0x80,0x03,0x00,0x00,0x12,0x00}; //Set String80 Descriptor
//code UINT8 waIntervalMap[11]={1,2,4,8,16,32,64,128,256,512,1024};


qHD_Structure Host20_qHD_List_Control0  __attribute__ ((aligned (32)));
qHD_Structure  Host20_qHD_List_Control1  __attribute__ ((aligned (32)));
qHD_Structure  Host20_qHD_List_Bulk0    __attribute__ ((aligned (32)));
qHD_Structure  Host20_qHD_List_Bulk1   __attribute__ ((aligned (32)));
 qHD_Structure *pHost20_qHD_List_Control0,*pHost20_qHD_List_Control1;
 qHD_Structure *pHost20_qHD_List_Bulk0,*pHost20_qHD_List_Bulk1;

UINT8            Host20_qTD_Manage[Host20_qTD_MAX];        //1=>Free 2=>used
Host20_Attach_Device_Structure    sAttachDevice __attribute__  ((aligned (8)));
BOOLEAN FirstBulkOut,FirstBulkIn;
//Host20_Attach_Device_Structure  xdata *psDevice_AP;
U8 *pUsbCtrlBuf;
U8 UsbCtrlBuf[CONTROL_DMA_BUF_LEN] __attribute__ ((aligned (4096)));
UINT8   pUsbData[CONTROL_BUF_LEN] __attribute__ ((aligned (16))) ;
U8 QtdBuf[Host20_qTD_SIZE*Host20_qTD_MAX+0x20] __attribute__ ((aligned (32)));
U8 *Host20_STRUCTURE_qTD_BASE_ADDRESS,*Host20_STRUCTURE_qHD_BASE_ADDRESS;
U8 *qTD_Base_Buf;

UINT8 gSpeed;
U16  TotalBytes;
//volatile UINT32 gwLastqTDSendOK;
extern U8 gUsbTimeout;
extern U8 gUsbStatus;
U8 DataToggleBackup;
extern U8 RecoveryFlag;
//extern BOOLEAN gDeviceFatalError;
extern U8  gUsbDeviceState;
extern U8  NowIsHub;
extern U16 MDrv_USBGetChipID(void);
extern U8 MDrv_USBGetChipVersion(void);

U8 usb_temp_buf[Scsi_Max_Transfer_Len] __attribute__ ((aligned (128)));

#define         USB_DMA_SIGNATURE1           0x55661234

extern void msAPI_Timer_ResetWDT(void);
#if defined(CPU_TYPE_MIPS) || defined(CPU_TYPE_ARM)
extern void   MAsm_CPU_Sync(void);
#endif

void EnterTestMode(void);

//************************************************************************************************************
//************************************************************************************************************
//                          *** Group-1:Main Function ***
//*************************************************************************************************************
//************************************************************************************************************
unsigned char mwHost20Port(int bOffset)
{
    return UHC_READXBYTE((U8) bOffset);
}
void mwHost20Port_wr(int bOffset,int value)
{
    UHC_SETXBYTE((U8) bOffset, (U8) value);
}

int  mwHost20Bit_Rd(int bByte,int wBitNum)
{
    return (mwHost20Port(bByte)&wBitNum);
}
void  mwHost20Bit_Set(int bByte,int wBitNum)
{
    U8 temp;
    temp=mwHost20Port(bByte);
    temp|=wBitNum;
    mwHost20Port_wr(bByte,temp);

}
void  mwHost20Bit_Clr(int bByte,int wBitNum)
{
    U8 temp;
    temp=mwHost20Port(bByte);
    temp&=~wBitNum;

    mwHost20Port_wr(bByte,temp);
}


//====================================================================
// * Function Name: flib_OTGH_Init
// * Description: Init the Host HW and prepare the ED/TD
//   <1>.Init All the Data Structure
//       <1.1>.Build control list
//       <1.2>.Build Bulk list
//       <1.3>.Build Interrupt list
//       <1.4>.Build ISO list (Reserved)
//   <2>.Reset the chip
//   <3>.Set HW register
//       <3.1>.Enable FIFO-Enable(0x100->Bit5) & FPGA-Mode-Half-Speed (0x100->Bit1)
//       <3.2>.Enable interrupts
//       <3.3>.Periodic List Base Address register
//       <3.4>.USBCMD (Interrupt/Threshod/Frame List/Run-Stop)
//
// * Input: wTimeWaitForConnect_ms:The time of waitting for connecting
// * OutPut: 0:Device do not connect
//           1:Host init ok
//           2:Bus Rest Fail
//====================================================================
#ifdef ATV_SERISE_USE
extern U8 jpCVBS5_Get_Device_Define(void);
#endif

#define   wTimeWaitForConnect_ms 1500

UINT8 flib_OTGH_Init(UINT8 wForDevice_B)
{
  UINT8 wValue;
  UINT16 wTimer_ms;

  MsOS_DelayTask(2);//2ms
  UTMI_ANDXBYTE(0,0xfe);
  MsOS_DelayTask(2);//2ms

  //<1>.Waiting for the Device connect
  #if 1
  if (wForDevice_B==0) {

#if 0 //20121109 replace this temp patch with disable BC mode in init code.
  //20121019, temp patch for Connect Bit missed in USB HW Reset while Run bit = 1
  if (MDrv_USBGetChipID() == CHIPID_EDEN)
  {
    flib_Host20_StopRun_Setting(HOST20_Disable);
    MsOS_DelayTask(10);  //10ms
  }
#endif

    mbHost20_USBCMD_HCReset_Set();            //host controller reset
    while(mbHost20_USBCMD_HCReset_Rd()>0);
  }
  #endif

  if (MDrv_USBGetChipID()==CHIPID_EUCLID) // Euclid
  {
     mwHost20Bit_Set(0x81,(BIT6|BIT7));
  }

  wValue=0;
  wTimer_ms=0;
  do {
       wValue=mwHost20_PORTSC_ConnectStatus_Rd();
       //printf("wValue:%02bx\n",wValue);

       if (wValue==0) {
         MsOS_DelayTask(1);//10, wait 1 ms
         wTimer_ms++;
       }
       //if (mwHost20_PORTSC_ConnectStatus_Rd()==0) return 0;
       if (wTimer_ms>wTimeWaitForConnect_ms)          // Case1:Waiting for 10 sec=10000
       { // Case2:Waiting for 100 ms =100
         DEBUG_USB(printf("??? Waiting for Peripheral Connecting Fail...\n"));
         return (0);
       }
  }while(wValue==0);



  //mwHost20_Misc_EOF1Time_Set(Host20_EOF1Time);
  //printf("34:%02bx\n",XBYTE[0x2434]);
  //<2>.Init All the Data Structure & write Base Address register
  flib_Host20_InitStructure();

  //Write Base Address to Register
  // Host20_STRUCTURE_BASE_ADDRESS
   mwHost20_CurrentAsynchronousAddr_Set(VirtoPhyAddr((U32)Host20_STRUCTURE_qHD_BASE_ADDRESS));

  //mwHost20_CurrentAsynchronousAddr_Set(VirtoPhyAddr((U16)&Host20_qHD_List_Work));
  // printf("qhead:%x\n",VirtoPhyAddr(Host20_STRUCTURE_qHD_BASE_ADDRESS));

  //<4>.Enable interrupts
  mwHost20_USBINTR_Set( HOST20_USBINTR_SystemError |

	                    HOST20_USBINTR_IntOnAsyncAdvance |
						HOST20_USBINTR_USBError |
						HOST20_USBINTR_CompletionOfTransaction);

  #ifdef ATV_SERISE_USE
  if(jpCVBS5_Get_Device_Define())
        UHC_XBYTE(0x40)|=0x80;//force enter FSmode
  #endif

  if (wForDevice_B==0) {
    if (flib_Host20_PortBusReset()>0)
      return(2);
  }

  return (1);  //success
}
void USB_Write_REG32(UINT8 addr,UINT32 val)
{
    UHC_SETXBYTE(addr, (U8) val & 0xFF);
    UHC_SETXBYTE(addr+1, (U8) (val>>8) & 0xFF);
    UHC_SETXBYTE(addr+2, (U8) (val>>16) & 0xFF);
    UHC_SETXBYTE(addr+3, (U8) (val>>24) & 0xFF);

 // mwHost20Port(addr)=(unsigned char)val & 0xff;
 // mwHost20Port(addr+1)=(unsigned char)((val >>8));
//  mwHost20Port(addr+2)=(unsigned char)((val>>16));
 // mwHost20Port(addr+3)=(unsigned char)((val>>24));

}

//====================================================================
// * Function Name: flib_Host20_Close
// * Description:
//   <1>.Suspend Host
//   <2>.Disable the interrupt
//   <3>.Clear Interrupt Status
//   <4>.Issue HW Reset
//   <5>.Free the Allocated Memory
// * Input:
// * OutPut:
//====================================================================
UINT8 flib_Host20_Close(void)
{
#if 0 //We don't need to suspend the bus.
  UINT32 wTemp;

  if (mwHost20_USBINTR_Rd()>0) {
    //<1>.Suspend Host
    flib_Host20_Suspend();

    //<2>.Disable the interrupt
    mwHost20_USBINTR_Set(0);

    //<3>.Clear Interrupt Status
    wTemp=mwHost20_USBSTS_Rd();
    wTemp=wTemp&0x0000003F;
    mwHost20_USBSTS_Set(wTemp);
  }
#endif

  return (1);
}

//====================================================================
// * Function Name: flib_Host20_StopRun_Setting
// * Description:
// * Input:
// * OutPut:
//====================================================================
void flib_Host20_StopRun_Setting(UINT8 bOption)
{
  if (bOption==HOST20_Enable) {
    if (mbHost20_USBCMD_RunStop_Rd()>0)
      return;

    mbHost20_USBCMD_RunStop_Set();

    while(mbHost20_USBCMD_RunStop_Rd()==0);
  }
  else {
    if (mbHost20_USBCMD_RunStop_Rd()==0)
      return;

    mbHost20_USBCMD_RunStop_Clr();

    while(mbHost20_USBCMD_RunStop_Rd()>0);
 }

}
#if 0
//====================================================================
// * Function Name: flib_Host20_Asynchronous_Setting
// * Description:
// * Input:
// * OutPut:
//====================================================================
void flib_Host20_Asynchronous_Setting(UINT8 bOption)
{
  if (bOption==HOST20_Enable) {
    if (mwHost20_USBSTS_AsynchronousStatus_Rd()>0)
      return;

    mbHost20_USBCMD_AsynchronousEnable_Set();

    while(mwHost20_USBSTS_AsynchronousStatus_Rd()==0);
  }
  else{
    if (mwHost20_USBSTS_AsynchronousStatus_Rd()==0)
      return;

    mbHost20_USBCMD_AsynchronousEnable_Clr();

    while(mwHost20_USBSTS_AsynchronousStatus_Rd()>0);
  }

}
#endif
//void _noop_()
//{
   //  int i=0;
//}
//====================================================================
// * Function Name: flib_Host20_PortBusReset
// * Description:
//   <1>.Waiting for HCHalted=0
//   <2>.Write PortEnable=0(Reserved for Faraday-IP)
//   <3>.Write PortReset=0
//   <4>.Wait time
//   <5>.Write PortReset=0
//   <6>.Waiting for IRS->PortChangeDetect
// * Input:
// * OutPut:
//====================================================================
UINT8 flib_Host20_PortBusReset(void)
{
  UINT8 bExitLoop;
  //UINT8 ttt;
  UINT32 wTmp;
  //flib_Host20_ForceSpeed(0);

  //<1>.Disable RunStop
  // bResetDuringRun=0;

  //printf("disable run\n");
  //if (mbHost20_USBCMD_RunStop_Rd()>0)
  flib_Host20_StopRun_Setting(HOST20_Disable);
  //printf("0x10 ->%02bx \n",XBYTE[0x2410]);

  //<2>.Write PortReset=0
  if ( (MDrv_USBGetChipID() == CHIPID_NEPTUNE) ||
       (MDrv_USBGetChipID() == CHIPID_ERIS) ||
       (MDrv_USBGetChipID() == CHIPID_TITANIA) ||
       (MDrv_USBGetChipID() == CHIPID_PLUTO) ||
       (MDrv_USBGetChipID() == CHIPID_TRITON) ||
       (MDrv_USBGetChipID() == CHIPID_TITANIA2) )
  {
    UTMI_ANDXBYTE(0x2c,0x3E);
    UTMI_ANDXBYTE(0x2d,~0x03);
    UTMI_ANDXBYTE(0x2f,~0x4A);
    UTMI_SETXBYTE(0x2a,0xd0);
  }
  else //for T3 later
  {
      UTMI_SETXBYTE(0x2c,0x10);
      UTMI_SETXBYTE(0x2d,0);
      UTMI_SETXBYTE(0x2e,0);
      UTMI_SETXBYTE(0x2f,0);
      UTMI_SETXBYTE(0x2a,0x80);//Chirp K detection level: 0x80 => 400mv, 0x20 => 575mv
  }

  mwHost20_PORTSC_PortReset_Set();


  //<3>.Wait time=>55ms
  //flib_Host20_TimerEnable(55);
  //MsOS_DelayTask(100);
  MsOS_DelayTask(80);
  if ( (MDrv_USBGetChipID() == CHIPID_EINSTEIN) ||
       (MDrv_USBGetChipID() == CHIPID_NAPOLI) )
  	UTMI_SETXBYTE(0x2a,0x60); // disconnect level 625 mV for 28 nm process
  else
  	UTMI_SETXBYTE(0x2a,0);
  MsOS_DelayTask(20);
  //flib_Debug_LED_Off_All();; //GPIO-High

  mwHost20_PORTSC_PortReset_Clr();

  if ( (MDrv_USBGetChipID() == CHIPID_NEPTUNE) ||
       (MDrv_USBGetChipID() == CHIPID_ERIS) ||
       (MDrv_USBGetChipID() == CHIPID_TITANIA) ||
       (MDrv_USBGetChipID() == CHIPID_PLUTO) ||
       (MDrv_USBGetChipID() == CHIPID_TRITON) ||
       (MDrv_USBGetChipID() == CHIPID_TITANIA2) )
  {
    UTMI_ORXBYTE(0x2c,0xC1);
    UTMI_ORXBYTE(0x2d,0x03);
    UTMI_ORXBYTE(0x2f,0x4a);
  }
  else if ((MDrv_USBGetChipID() == CHIPID_TITANIA3) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA4) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA7) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA8) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA9) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA12) ||
            (MDrv_USBGetChipID() == CHIPID_TITANIA13) ||
            (MDrv_USBGetChipID() == CHIPID_JANUS) ||
            (MDrv_USBGetChipID() == CHIPID_MARIA10) ||
            (MDrv_USBGetChipID() == CHIPID_JANUS2)  ||
            (MDrv_USBGetChipID() == CHIPID_AMBER6) )
  {
      UTMI_SETXBYTE(0x2c,0x10);
      UTMI_SETXBYTE(0x2d,0x02);
      UTMI_SETXBYTE(0x2f,0x81);
  }
  else if ( (MDrv_USBGetChipID() == CHIPID_MACAW12) )
  {
      //PreEmphasis +40%, CM +20%
      UTMI_SETXBYTE(0x2c,0x90);
      UTMI_SETXBYTE(0x2d,0x03);
      UTMI_SETXBYTE(0x2f,0x81);
  }
  else if ((MDrv_USBGetChipID()==CHIPID_AMBER1) ||
            (MDrv_USBGetChipID()==CHIPID_AMBER5) ||
            (MDrv_USBGetChipID()==CHIPID_AMBER7) ||
            (MDrv_USBGetChipID()==CHIPID_AMBER3) ||
            (MDrv_USBGetChipID()==CHIPID_AMETHYST)  ||
            (MDrv_USBGetChipID()==CHIPID_EAGLE))
  {
      //for Amber1 later 40nm before Agate
      UTMI_SETXBYTE(0x2c,0x98);
      UTMI_SETXBYTE(0x2d,0x02);
      UTMI_SETXBYTE(0x2e,0x10);
      UTMI_SETXBYTE(0x2f,0x01);
  }
  else if ( (MDrv_USBGetChipID() == CHIPID_EDISON) ||
              (MDrv_USBGetChipID() == CHIPID_EIFFEL) )
  {
      //for Agate later 40nm, same as 55nm setting2
      UTMI_SETXBYTE(0x2c,0x90);
      UTMI_SETXBYTE(0x2d,0x02);
      UTMI_SETXBYTE(0x2f,0x81);

  }
  else  //for Agate later 40nm, same as 55nm setting1
  {
      UTMI_SETXBYTE(0x2c,0x10);
      UTMI_SETXBYTE(0x2d,0x02);
      UTMI_SETXBYTE(0x2f,0x81);
  }
  //printf("Host Speed:%02bx\n",mwOTG20_Control_HOST_SPD_TYP_Rd());

  //<5>.Waiting for IRS->PortChangeDetect
  //printf("wait reset\n");
  #if 1
  bExitLoop=0;
  wTmp=0;

  do {
       if (mwHost20_PORTSC_PortReset_Rd()==0)
       bExitLoop=1;
       //USBDELAY(100);
       //if (mwHost20_PORTSC_ConnectStatus_Rd()==0) return 1;
       wTmp++;
       if (wTmp>20000) {
         DEBUG_USB(printf("??? Error waiting for Bus Reset Fail...==> Reset HW Control\n"));
         mbHost20_USBCMD_HCReset_Set();
         //while(mbHost20_USBCMD_HCReset_Rd()==1);
         return (1);
       }
  }
  while(bExitLoop==0);
  #endif
  //<6>.Enable RunStop Bit
  #if 0
  if (mwHost20_PORTSC_ForceSuspend_Rd())
  {
    printf("port suspend\n");
    mwHost20_PORTSC_ForceResume_Set();          //force resume
    USBDELAY(14);
    mwHost20_PORTSC_ForceResume_Clr();
  }
  #endif
  //mwHost20_PORTSC_ConnectChange_Set();  //clear connection change bit

  UTMI_ORXBYTE(0x06,0x03);                  //reset UTMI
  UTMI_ANDXBYTE(0x06,0xfc);

  flib_Host20_StopRun_Setting(HOST20_Enable);

  //flib_Debug_LED_On_All();   //GPIO-Low
  MsOS_DelayTask(50);

  //<7>.Detect Speed
  gSpeed= mwOTG20_Control_HOST_SPD_TYP_Rd();
  //ttt=mwOTG20_Control_HOST_SPD_TYP_Rd();
  HOSTLIB_MSG(printf("\r\n Host type:%x\n", gSpeed));
#if 0
  printf("UTMI_2F: %x, UTMI_2E: %x, UTMI_2D: %x, UTMI_2C: %x\n",
  			UTMI_READXBYTE(0x2F), UTMI_READXBYTE(0x2E),
  			UTMI_READXBYTE(0x2D), UTMI_READXBYTE(0x2C));
#endif
  //<8>.Delay 20 ms
  //flib_Host20_TimerEnable(5);//After Reset => Host must reserve 20 ms for Device init

  if (gSpeed==2)
  {
    UTMI_ORXBYTE(9,0x80);                                    //HS rx robust enable
    mwHost20_Misc_EOF1Time_Set(3);
    gUsbDeviceState=USB20_DEVICE;
  }
  else
  {
    UTMI_ANDXBYTE(9,0x7f);
    mwHost20_Misc_EOF1Time_Set(2);
    gUsbDeviceState=USB11_DEVICE;
  }

  flib_Host20_QHD_Control_Init();
  //printf("return 1");
#if 0 //Try to support HID class
  if(gSpeed==0x01)
    {
         MINI_DEBUG2(printf("low speed device\n"));
         return (2); //not support Low Speed device
    }
#endif

  return (0);
}

//====================================================================
// * Function Name: flib_Host20_Suspend
// * Description:
//   <1>.Make sure PortEnable=1
//   <2>.Write PORTSC->Suspend=1
//   <3>.Waiting for the ISR->PORTSC->Suspend=1
// * Input:
// * OutPut: 0:OK
//           1:Fail
//====================================================================
void flib_Host20_Suspend(void)
{
  #if 0
  if (mbHost20_USBCMD_RunStop_Rd()==0)
    return(1);

  //<1>.Make sure PortEnable=1
  if (mwHost20_PORTSC_EnableDisable_Rd()==0)
    return(1);
  #endif
  //<2>.Write PORTSC->Suspend=1
  flib_Host20_StopRun_Setting(HOST20_Disable);//For Faraday HW request

  //<3>.Write PORTSC->Suspend=1
  mwHost20_PORTSC_ForceSuspend_Set();

  //<4>.Waiting for the ISR->PORTSC->Suspend=1
  #if 0
  flib_Host20_TimerEnable_UnLock(1);//1sec
  bExitFlag=0;

  do {
       if (mwHost20_PORTSC_ForceSuspend_Rd()>0)
       bExitFlag=1;

       if (gwOTG_Timer_Counter>5) {
	     bExitFlag=1;
         printf(">>> Fail => Time Out for Waiting ForceSuspend...\n");
       }
  }
  while(bExitFlag==0);
  #else
  while(mwHost20_PORTSC_ForceSuspend_Rd()==0);
  #endif

}




//====================================================================
// * Function Name: flib_Host20_Control_Command_Request
// * Description:
// * Input: none
// * OutPut: none
//====================================================================
void flib_Host20_Control_Command_Request(Host20_Control_Command_Structure *pbCMD,UINT8 bmRequestType_Temp,UINT8 bRequest_Temp,UINT16 wValue_Temp,UINT16 wIndex_Temp,UINT16 wLength_Temp)
{
  UINT8 i;
  UINT8 *pbTemp;

  pbTemp=(UINT8*)pbCMD;
  for (i=0;i<8;i++)
    *pbTemp++=0x00;
  pbCMD->bmRequestType=bmRequestType_Temp;  //Byte-0:In/Standard/Device
  pbCMD->bRequest=bRequest_Temp;

  pbCMD->wValueLow=(UINT8)wValue_Temp;
  pbCMD->wValueHigh=(UINT8)(wValue_Temp>>8);

  pbCMD->wIndexLow=(UINT8)wIndex_Temp;
  pbCMD->wIndexHigh=(UINT8)(wIndex_Temp>>8);

  pbCMD->wLengthLow=(UINT8)wLength_Temp;
  pbCMD->wLengthHigh=(UINT8)(wLength_Temp>>8);
}
#if 0
void FillBufferArray(qTD_Structure xdata *spTempqTD, UINT16 bpDataPage)
{

  U32 temp;
  temp=VirtoPhyAddr(bpDataPage);
  spTempqTD->ArrayBufferPointer[0].Byte1=(UINT8)(temp>>8)&0xf0;
  spTempqTD->ArrayBufferPointer[0].Byte2=(UINT8)(temp>>16)&0xff;
  spTempqTD->ArrayBufferPointer[0].Byte3=(UINT8)(temp>>24)&0xff;

}
#endif

//void Dump_QTD(UINT16 addr);
//void Dump_Data(UINT16 addr,UINT16 sz);
#if 0
void Dump_Data(UINT16 addr,UINT16 sz)
{
  UINT16 i;
  printf("addr:%x -> \r\n",addr);
  for (i=0; i < sz ; i++)
    printf("%02bx ",*(unsigned char volatile xdata *)(addr+i));
}
#endif

#ifdef  ENABLE_CBI_HOST
UINT8 flib_Host20_Issue_Control_CBI(UINT8* pbCmd,UINT16 hwDataSize,UINT8* pbData)
{
    qTD_Structure  *spTempqTD;
//   U32       bpDataPage;
     U8        bReturnValue;
 // U8 i;
    qHD_Structure  *qh_ptr;

    qh_ptr=pHost20_qHD_List_Control1;

    //<0>.Allocate qTD & Data Buffer
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    // bpDataPage=sAttachDevice.bDataBuffer;           //2//2k buffer

    //<2.1>.Setup packet
    //<A>.Fill qTD
    spTempqTD->bPID=HOST20_qTD_PID_SETUP;                   //Bit8~9
    spTempqTD->bTotalBytes=8;           //Bit16~30

    spTempqTD->bDataToggle=0;            //Bit31
    //  FillBufferArray(spTempqTD, bpDataPage);
    memcpy(pUsbCtrlBuf,pbCmd,8);

    spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);

    // printf("bpDataPage:%x\n",bpDataPage);
    //   memcpy((U8  *)bpDataPage,pbCmd,8);

    //<B>.Send qTD
    //  Dump_QTD(spTempqTD);

    //  Dump_Data(0x2400,0x50);
    //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        goto exit_issue_control;

    //<4.2>.Out packet
    //<A>.Fill qTD
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
    spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30
    spTempqTD->bDataToggle=1;            //Bit31
    spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);
    // FillBufferArray(spTempqTD, bpDataPage);

    memcpy(pUsbCtrlBuf,pbData,hwDataSize);

    //<B>.Send qTD
    //   bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        goto exit_issue_control;

    //<4.3>.In packet
    //<A>.Fill qTD
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
    spTempqTD->bTotalBytes=0;           //Bit16~30
    spTempqTD->bDataToggle=1;            //Bit31

    //<B>.Send qTD
    //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        goto exit_issue_control;

    return (0);
exit_issue_control:

    return (bReturnValue);
}
#endif

//====================================================================
 //For Control-Single qTD// * Function Name: flib_Host20_Issue_Control
// * Description:
//   <1>.Analysis the Controller Command => 3 type
//   <2>.Case-1:"Setup/In/Out' Format..."
//       (get status/get descriptor/get configuration/get interface)
//   <3>.Case-2:'Setup/In' Format...      => Faraday Driver will not need
//       (clear feature/set feature/set address/set Configuration/set interface  )
//   <4>.Case-3:'Setup/Out/In'
//       (set descriptor)
// * Input:
// * OutPut: 0: OK
//           X:>0 => Fail
//====================================================================
 U8 flib_Host20_Issue_Control ( U8 bEdNum, U8* pbCmd, U16 hwDataSize, U8* pbData)
{
    qTD_Structure  *spTempqTD;
//   U32       bpDataPage;
     U8        bReturnValue;
 // U8 i;
    qHD_Structure  *qh_ptr;
    U32 *dnaptr;
    U32  dwTimes;

    if (bEdNum==0)
        qh_ptr=pHost20_qHD_List_Control0;
    else
        qh_ptr=pHost20_qHD_List_Control1;


    //<0>.Allocate qTD & Data Buffer
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    // bpDataPage=sAttachDevice.bDataBuffer;           //2//2k buffer

    //<2.1>.Setup packet
    //<A>.Fill qTD
    spTempqTD->bPID=HOST20_qTD_PID_SETUP;                   //Bit8~9
    spTempqTD->bTotalBytes=8;           //Bit16~30

    spTempqTD->bDataToggle=0;            //Bit31
    //  FillBufferArray(spTempqTD, bpDataPage);
    memcpy(pUsbCtrlBuf,pbCmd,8);

    spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);

    // printf("bpDataPage:%x\n",bpDataPage);
    //   memcpy((U8  *)bpDataPage,pbCmd,8);

#if 1
    //<B>.Send qTD
    //  Dump_QTD(spTempqTD);

    //  Dump_Data(0x2400,0x50);
    //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        goto exit_issue_control;

 //  printf("c3");

   //<1>.Analysis the Controller Command
    switch (*(pbCmd+1)) { // by Standard Request codes
      // <2>.Case-1:"Setup/In/Out' Format..."
        case 0:		// get status
	    case 6:		// get descriptor
	    case 8:		// get configuration
	    case 10:	// get interface
	    case 0xfe:        //get Max Lun
         //<2.2>.In packet

         //<A>.Fill qTD
            spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
            spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
            spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30

            spTempqTD->bDataToggle=1;            //Bit31

            //  spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr(bpDataPage);
            //FillBufferArray(spTempqTD, bpDataPage);
            spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);

	    //MAKE THE SIGNATURE
	    dnaptr=(U32 *)(U32)pUsbCtrlBuf;
	    if (hwDataSize>3)
	    {
	        dnaptr+=(hwDataSize/4) - 1;
	        *dnaptr=USB_DMA_SIGNATURE1;
	    }

            //<B>.Send qTD
            //bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
              bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);
            if (bReturnValue>0)
                goto exit_issue_control;
            //   printf("c4");
            if (((hwDataSize-TotalBytes)==hwDataSize) && (hwDataSize>3))
            {
	            dwTimes=MsOS_GetSystemTime();
	            while (*dnaptr==USB_DMA_SIGNATURE1)
	            {
	              if ((MsOS_GetSystemTime()-dwTimes) >= 10)
	              {
	                  HOSTLIB_MSG(printf("MIU DMA not finished yet\n"));
	                  break;          //timeout
	              }
	            }
            }

            //<C>.Waiting for result
            memcpy(( U8  *)pbData,pUsbCtrlBuf,hwDataSize);

            //    Dump_Data((U16)pbData,hwDataSize);

            //<2.3>.Out packet

            //<A>.Fill qTD
            spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
            spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
            spTempqTD->bTotalBytes=0;           //Bit16~30

            spTempqTD->bDataToggle=1;            //Bit31

            //<B>.Send qTD
            //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
            bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

            if (bReturnValue>0)
                goto exit_issue_control;


            break;

      //<3>.Case-2:'Setup/In' Format...      => Faraday Driver will not need
	  case 1:		// clear feature
	  case 3:		// set feature
   	  case 5:		// set address
   	  case 9:		// set Configuration
      case 11:	// set interface

        //<3.2>.In packet

        //<A>.Fill qTD
        spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
        spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
        spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30
        spTempqTD->bDataToggle=1;            //Bit31
        spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);
        //FillBufferArray(spTempqTD, bpDataPage);

        //<B>.Send qTD
        // bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
        bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

        if (bReturnValue>0)
            goto exit_issue_control;
         //<C>.Copy Result
        // memcpy(pbData,pUsbCtrlBuf,hwDataSize);
        break;

      //<4>.Case-3:'Setup/Out/In'
      case 7:		// set descriptor
        //<4.2>.Out packet
        //<A>.Fill qTD
        spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
        spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
        spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30
        spTempqTD->bDataToggle=1;            //Bit31
        spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);
        // FillBufferArray(spTempqTD, bpDataPage);

        memcpy(pUsbCtrlBuf,pbData,hwDataSize);

        //<B>.Send qTD
        //   bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
        bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

        if (bReturnValue>0)
            goto exit_issue_control;


        //<4.3>.In packet
        //<A>.Fill qTD
        spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
        spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
        spTempqTD->bTotalBytes=0;           //Bit16~30
        spTempqTD->bDataToggle=1;            //Bit31

        //<B>.Send qTD
        //  bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
        bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

        if (bReturnValue>0)
            goto exit_issue_control;

        break;

    default:
 	    break;
   }
   #endif
   return (0);
exit_issue_control:

    return (bReturnValue);

}


#if 0
void Dump_QTD(UINT16 addr)
{
  UINT8 i;
  // kcj 2007-06-07	printf("QTD:%x -> \n",addr);
  // kcj 2007-06-07	for (i=0; i < 0x20 ; i++)
  // kcj 2007-06-07	printf("%02bx ",*(unsigned char volatile xdata *)(addr+i));


}
void Dump_Data(UINT16 addr,UINT16 sz)
{
  UINT16 i;
  // kcj 2007-06-07	printf("addr:%x -> \n",addr);
  // kcj 2007-06-07	for (i=0; i < sz ; i++)
  // kcj 2007-06-07	printf("%02bx ",*(unsigned char volatile xdata *)(addr+i));


}
#endif
//====================================================================
// * Function Name: flib_Host20_Issue_Bulk
// * Description: Input data must be 4K-Alignment
//               <1>.MaxSize=20 K
//               <2>.Support Only 1-TD
// * Input:
// * OutPut:
//====================================================================
U8  flib_Host20_Issue_Bulk (U8 bArrayListNum,U16 hwSize,U32 pwBuffer,U8 bDirection)
{
    qTD_Structure  *spTempqTD;
    U8 bTemp,i;
    U16 count;
    U32 addr;
    U32 mybuf,workbuf;
    qHD_Structure   *spTempqH;
    U32 *dnaptr=NULL;
    U32 dwTimes;



    mybuf=0;

    if ( pwBuffer !=(U32) KSEG02KSEG1(pwBuffer) )
    {
        if (pwBuffer & 0x7)			//flush should be 8 bytes aligned
        {
            //	printf(" buf ");
            #if 0
            mybuf=(U32)msAPI_Memory_Allocate(hwSize, BUF_ID_USB_HOST );  //temporarily buffer for USB control
            if ( mybuf == 0 )
            {
                    printf("Memory allocate failed\n");
                    return HOST20_FATAL;
            }
            mybuf = (U32)KSEG02KSEG1(mybuf);
            #else
            mybuf = (U32)KSEG02KSEG1(usb_temp_buf);
            pwBuffer=(U32)KSEG02KSEG1(pwBuffer);
            #endif

        }
        else
        {
            //    printf(" fuh ");
            MY_HAL_DCACHE_FLUSH((U32)pwBuffer,(U32)hwSize);		//flush buffer to uncached buffer
            pwBuffer=(U32)KSEG02KSEG1(pwBuffer);
        }
    }
    else
    {
        if (pwBuffer & 0x7)
        {
                //   printf(" buf ");
                #if 0
                mybuf=(U32)msAPI_Memory_Allocate(hwSize, BUF_ID_USB_HOST );  //temporarily buffer for USB control
                if ( mybuf == 0 )
                {
                    printf("Memory allocate failed\n");
                    return HOST20_FATAL;
                }
                mybuf = (U32)KSEG02KSEG1(mybuf);
                #else
                mybuf = (U32)KSEG02KSEG1(usb_temp_buf);
                pwBuffer=(U32)KSEG02KSEG1(pwBuffer);
                #endif

        }
    }

    spTempqTD =flib_Host20_GetStructure(Host20_MEM_TYPE_qTD); //The qTD will be release in the function "Send"
    spTempqTD->bTotalBytes=hwSize ;

      //    FillBufferArray2(spTempqTD,pwBufferArray,hwSize);         //use global buffer , because it is 4k alignment
#if 0
    spTempqTD->ArrayBufferPointer_Word[0]=pwBufferArray;
    if (((pwBufferArray&0xfff)+hwSize) > 0xfff)     //goto page 2
        spTempqTD->ArrayBufferPointer_Word[1]=pwBufferArray+0x1000; 		//+4K
    if (hwSize > (0x1000+0x1000-(pwBufferArray&0xfff))
#endif
//not allow buffer over 16K for my usage
   if (mybuf==0)			//use original buf
        workbuf=pwBuffer;
   else
        workbuf=mybuf;

    dnaptr=(U32 *)(U32)workbuf;
    spTempqTD->ArrayBufferPointer_Word[0] = VirtoPhyAddr(workbuf);

    count = 0x1000 - (workbuf & 0x0fff);  /* rest of that page */
    if ( hwSize < count)    /* ... iff needed */
        count = hwSize;
    else
    {
        workbuf +=  0x1000;
        workbuf &= ~0x0fff;

        /* per-qtd limit: from 16K to 20K (best alignment) */
        for (i = 1; count < hwSize && i < 5; i++)
        {
            addr = workbuf;
            spTempqTD->ArrayBufferPointer_Word[i] = VirtoPhyAddr(addr);
            //      		lastnums++;
            workbuf += 0x1000;
            if ((count + 0x1000) < hwSize)
                count += 0x1000;
            else
                count = hwSize;
        }

    }
    if (bArrayListNum==0)
        spTempqH=pHost20_qHD_List_Bulk0;
  else  // if (bArrayListNum==1)
        spTempqH=pHost20_qHD_List_Bulk1;


  //<2>.Analysis the Direction
    if (bDirection==OTGH_Dir_IN)
    {
        spTempqTD->bPID=HOST20_qTD_PID_IN;
        if (FirstBulkIn)
        {
            spTempqTD->bDataToggle=0;
            spTempqH->bDataToggleControl=1;
        }

        //MAKE THE SIGNATURE
        if (hwSize>3)
        {
            dnaptr+=(hwSize/4) - 1;
            *dnaptr=USB_DMA_SIGNATURE1;
        }

    }
    else
    {
        spTempqTD->bPID=HOST20_qTD_PID_OUT;
        if (FirstBulkOut)
        {
            spTempqTD->bDataToggle=0;
            spTempqH->bDataToggleControl=1;
        }
        if (mybuf)  memcpy((void*)mybuf,(void*)pwBuffer,hwSize);	//copy buffer
    }
    //<3>.Send TD
    //  if (bArrayListNum==0)
    //     bTemp=flib_Host20_Send_qTD(spTempqTD ,pHost20_qHD_List_Bulk0,Host20_Timeout);
    // else if (bArrayListNum==1)
    bTemp=flib_Host20_Send_qTD(spTempqTD ,spTempqH,gUsbTimeout);

    if ((FirstBulkIn)&&(bDirection==OTGH_Dir_IN))
    {
        spTempqH->bDataToggleControl=0;
        FirstBulkIn=FALSE;
    }
    if ((FirstBulkOut)&&(bDirection==OTGH_Dir_Out))
    {
        spTempqH->bDataToggleControl=0;
        FirstBulkOut=FALSE;
    }

    if (bDirection==OTGH_Dir_IN)
    {

        if(((hwSize-TotalBytes)==hwSize) && (hwSize>3))
        {
          dwTimes=MsOS_GetSystemTime();
          while (*dnaptr==USB_DMA_SIGNATURE1)
          {
            if ((MsOS_GetSystemTime()-dwTimes) >= 10)
            {
              //printf("MIU DMA not finished yet\n");
	          break;          //timeout
            }
          }
        }

#if defined(CPU_TYPE_MIPS) || defined(CPU_TYPE_ARM)
     MAsm_CPU_Sync();
#endif
        if (mybuf)
        memcpy((void*)pwBuffer,(void*)mybuf,hwSize);	//copy buffer
    }

#if 0
    if (mybuf)
    {
        mybuf = (U32)KSEG12KSEG0(mybuf);
        msAPI_Memory_Free((void*)mybuf,BUF_ID_USB_HOST);
    }
#endif
    //  printf("Z");
    return (bTemp);
}

//====================================================================
// * Function Name: flib_Host20_AnalysisConfigyration
// * Description:
// * Input:
//
// * OutPut:
//====================================================================
UINT8 flib_Host20_AnalysisConfigyration (UINT8  *pbData)
{
#if 1
    UINT8 i = 0;
//    UINT8 *pBuf, j;
    UINT8 ept_idx = 0;
    int RemainLen;

    //<1>.Copy Configuration 1~2
    if (sAttachDevice.sDD.bCONFIGURATION_NUMBER>HOST20_CONFIGURATION_NUM_MAX)
    {
        //while(1);
    }

    memcpy((U8 *)&(sAttachDevice.saCD[i]),pbData,HOST20_CONFIGURATION_LENGTH);
    pbData=pbData+HOST20_CONFIGURATION_LENGTH;

#if 0
    printf("Got a Config Descriptor: \n");
    pBuf = (U8 *)&(sAttachDevice.saCD[i]);
    for (j=0; j<HOST20_CONFIGURATION_LENGTH; j++)
        printf(" %02X", pBuf[j]);
    printf("\n\n");
#endif

    RemainLen = (int)sAttachDevice.saCD[i].bTotalLengthLowByte +
                    ((int)sAttachDevice.saCD[i].bTotalLengthHighByte << 8) - HOST20_CONFIGURATION_LENGTH;

    //intf_idx = 0;
    while (RemainLen > 0)
    {
#if 0
        printf("RemainLen: 0x%x\n", RemainLen);

        printf("Find a descriptor:\n");
        pBuf = pbData;
        for (j=0; j<pbData[0]; j++)
            printf(" %02X", pBuf[j]);
        printf("\n\n");
#endif

        if (pbData[1] == 4) /*INTERFACE*/
        {
//            printf("Get a interface descriptor\n");
            memcpy((U8 *)&(sAttachDevice.saCD[i].sInterface[0]),pbData,HOST20_INTERFACE_LENGTH);
            ept_idx = 0;
            if (sAttachDevice.saCD[i].sInterface[0].bEP_NUMBER>HOST20_ENDPOINT_NUM_MAX)
            {
//                printf("endpoint number is over  HOST20_ENDPOINT_NUM_MAX\n");
            }
        }
        else if (pbData[1] == 5) /*ENDPOINT*/
        {
//            printf("Get a endpoint descriptor\n");
            memcpy((U8 *)&(sAttachDevice.saCD[i].sInterface[0].sED[ept_idx]),pbData,HOST20_ENDPOINT_LENGTHX);
            ept_idx++;
        }

        RemainLen -= pbData[0];
        pbData = pbData + pbData[0];

        if (ept_idx == sAttachDevice.saCD[i].sInterface[0].bEP_NUMBER) //All endpoints are collected
        {
            //intf_idx++;
            if ( (sAttachDevice.saCD[i].sInterface[0].bInterfaceClass == USB_INTERFACE_CLASS_MSD) ||
                (sAttachDevice.saCD[i].sInterface[0].bInterfaceClass == USB_INTERFACE_CLASS_HUB) ||
                (sAttachDevice.saCD[i].sInterface[0].bInterfaceClass == USB_INTERFACE_CLASS_IMAGE) ||
                (sAttachDevice.saCD[i].sInterface[0].bInterfaceClass == USB_INTERFACE_CLASS_HID) )
                break;
        }
    }

//    printf("End the config analyzing\n");

    return (1);
#else

  UINT8 i,j,k;


  //<1>.Copy Configuration 1~2
  if (sAttachDevice.sDD.bCONFIGURATION_NUMBER>HOST20_CONFIGURATION_NUM_MAX)
  {
    // kcj 2007-06-07	printf("??? Analysis Configuration Fail(bCONFIGURATION_NUMBER>Max)...");
    //while(1);
  }
  //for (i=0;i<sAttachDevice.sDD.bCONFIGURATION_NUMBER;i++)
  i=0;
  {

    memcpy((U8  *)&(sAttachDevice.saCD[i]),pbData,HOST20_CONFIGURATION_LENGTH);
    pbData=pbData+HOST20_CONFIGURATION_LENGTH;

    //<2>.Copy Interface 1~5
    if (sAttachDevice.saCD[i].bINTERFACE_NUMBER>HOST20_INTERFACE_NUM_MAX)
    {
      // kcj 2007-06-07	printf("??? Analysis Interface Fail(bINTERFACE_NUMBER>Max)...");
      //while(1);
    }
    for (j=0;j<sAttachDevice.saCD[i].bINTERFACE_NUMBER;j++)
    {
      memcpy((U8  *)&(sAttachDevice.saCD[i].sInterface[j]),pbData,HOST20_INTERFACE_LENGTH);
      pbData=pbData+HOST20_INTERFACE_LENGTH;

      //<3>.Copy Class HID
      if (sAttachDevice.saCD[i].sInterface[j].bInterfaceClass ==3)
      {//Only support 1 class
        memcpy((U8  *)&(sAttachDevice.saCD[i].sInterface[j].sClass[0]),pbData,*pbData);
        pbData=pbData+(*pbData);
      }

      //<4>.Copy Endpoint 1~5
      if (sAttachDevice.saCD[i].sInterface[j].bEP_NUMBER>HOST20_ENDPOINT_NUM_MAX)
      {
        // kcj 2007-06-07	printf("??? Analysis Endpoint Fail(bEP_NUMBER>Max)...");
        //while(1);
      }
      for (k=0;k<sAttachDevice.saCD[i].sInterface[j].bEP_NUMBER;k++)
      {
        memcpy((U8  *)&(sAttachDevice.saCD[i].sInterface[j].sED[k]),pbData,HOST20_ENDPOINT_LENGTHX);
        pbData=pbData+HOST20_ENDPOINT_LENGTHX;
      }//Endpoint

    }//Interface

  }//Configuration


  //<4>.Checkong the OTG Descriptor
  //if (*pbData==3)
  //{ if (*(pbData+1)==9)
  //  {
  //    sAttachDevice.sOTG.bED_OTG_Length=3;
  //    sAttachDevice.sOTG.bED_OTG_bDescriptorType=9;
  //    sAttachDevice.sOTG.bED_OTG_bAttributes=*(pbData+2);
  //  }
  //}

  return (1);
#endif
}

//====================================================================
// * Function Name: flib_Host20_FirstTransfer        ==>51ok
// * Description:
// * Input:
//
// * OutPut:
//====================================================================
UINT8 flib_Host20_FirstTransfer (void)
{

  UINT8  bLoop;
  UINT8 bCMD[8];
  bLoop=0;
  do{
      bLoop++;
      //<1>.GetDescriptor - Device (8 Bytes)
      //while (1)
      //{

      memcpy(bCMD,OTGH_GETDESCRIPTOR_DEVICE_8,8);

      if (flib_Host20_Issue_Control (0,bCMD,8,(UINT8*)&(sAttachDevice.sDD))==0)
      {

        return (0);//ok
      }
      //}
      //<2>.Suspend the Bus
      //printf("first\n");
      if (gUsbStatus==USB_TIMEOUT) return 1;
      #if 1
      flib_Host20_Close();
      //
      UTMI_ORXBYTE(0x06,0x03);                  //reset UTMI
      UTMI_ANDXBYTE(0x06,0xfc);
      mbHost20_USBCMD_HCReset_Set();
      MsOS_DelayTask(100);

      //<3>.Reset Port
      flib_Host20_PortBusReset();
      #endif
  }while(bLoop<6);
   gUsbStatus=USB_INIT_FAIL;

  return(1);

}
#if 1
code UINT8 SET_FEATURE_PORT_POWER[]        = {0x23,0x03,0x08,0x00,0x01,0x00,0x00,0x00};
code UINT8 CLEAR_FEATURE_C_PORT_CONNECTION[]={0x23,0x01,0x10,0,0x01,0,0,0};
code UINT8 SET_FEATURE_PORT_RESET[]        = {0x23,0x03,0x04,0x00,0x01,0x00,0x00,0x00};
code UINT8 CLEAR_FEATURE_C_PORT_RESET[]={0x23,0x01,0x14,0,0x01,0,0,0};
code UINT8 OTGH_GETDESCRIPTOR_HUBCLASS[]={0xA0,0x06,0x00,0x00,0x00,0x00,0x09,0x00};
code UINT8 GET_PORT_STATUS[]={0xA3,0x00,0x00,0x00,0x01,0x00,0x04,0x00};
U8 Usb_Hub_Port_Num()
{
  UINT8 bCMD[8];
  U8 PortNum,i;

  memcpy(bCMD,OTGH_GETDESCRIPTOR_HUBCLASS,8);

  if (flib_Host20_Issue_Control (1,bCMD,0x09,pUsbData)>0)
  {
    return FALSE;
  }
  PortNum=*(pUsbData+2);
  #if 1
  for(i=1;i<=PortNum;i++)
  {
    //set feature port power
    memcpy(bCMD,SET_FEATURE_PORT_POWER,8);
    bCMD[4]=i;
    if ( flib_Host20_Issue_Control (1,bCMD,0,NULL)>0)
	{
      DEBUG_USB(printf("set port power failed\n"));
	  return PortNum;
    }
  }
  MsOS_DelayTask(125);

  for(i=1;i<=PortNum;i++)
  {
    //clear feature c_port_connection
    memcpy(bCMD,CLEAR_FEATURE_C_PORT_CONNECTION,8);
    bCMD[4]=i;
    if (flib_Host20_Issue_Control (1,bCMD,0,NULL)>0)
	{
      DEBUG_USB(printf("clear feature failed\n"));
	  return PortNum;
    }
  }
  #endif
  return PortNum;
}
BOOLEAN USB_Hub_Handle(U8 port)
{
  UINT8 bCMD[8];

  MsOS_DelayTask(10);//10, wait 1 ms

  //get all prot status


  //get prot status
  memcpy(bCMD,GET_PORT_STATUS,8);
  bCMD[4]=port;
  if (flib_Host20_Issue_Control (1,bCMD,0x04,pUsbData)>0)
  {
    DEBUG_USB(printf("Get port status failed\n"));

    return FALSE;
  }

  DEBUG_USB(printf(" Port:%02bx Status=%02bx",port,(*pUsbData)));
  if(!((*pUsbData)&0x01))
    return FALSE;
  //for (i=0 ; i < 3 ; i++)
  {
    DEBUG_USB(printf("Port %02bx have device.\n",port));
    //set feature port_reset
    memcpy(bCMD,SET_FEATURE_PORT_RESET,8);
    bCMD[4]=port;
    if (flib_Host20_Issue_Control (1,bCMD,0,NULL)>0)
      return FALSE;
    MsOS_DelayTask(10);//10, wait 1 ms

    memcpy(bCMD,GET_PORT_STATUS,8);
    bCMD[4]=port;
    if (flib_Host20_Issue_Control (1,bCMD,0x04,pUsbData)>0)
    {
      DEBUG_USB(printf("Get port status failed\n"));

      return FALSE;
    }

    #if 1
    DEBUG_USB(printf(" Port:%02bx Status=%02bx",port,(*pUsbData)));
    //clear feature c_port_reset
    memcpy(bCMD,CLEAR_FEATURE_C_PORT_RESET,8);
    bCMD[4]=port;
    if (flib_Host20_Issue_Control (1,bCMD,0,NULL)>0)
      return FALSE;

    memcpy(bCMD,GET_PORT_STATUS,8);
    bCMD[4]=port;
    if (flib_Host20_Issue_Control (1,bCMD,0x04,pUsbData)>0)
    {
      DEBUG_USB(printf("Get port status failed\n"));

      return FALSE;
    }
    #endif
    // flib_Host20_Interrupt_Init(1,1,1);
    //if (pUsbData[2]!=0)
    //{
	//  OTGH_PT_Bulk_Init_for_Int();
    //  result=flib_Host20_Issue_Bulk (0,1,pUsbData,OTGH_Dir_IN);
    //  // flib_Host20_Issue_Interrupt(1,HOST20_qTD_PID_IN);

    MsOS_DelayTask(30);//10, wait 1 ms
  }
  return TRUE;
}
#endif

#ifdef Enable_Issue_TestPacket
extern void IssueTestPacket(U8 xdata *TestData);
void USB_DACHE_FLUSH(U32 addr, U32 length)
{
  MY_HAL_DCACHE_FLUSH(addr, length);
}

#endif

//====================================================================
// * Function Name: flib_Host20_Enumerate
// * Description:
// * Input:
//
// * OutPut:
//====================================================================
#ifndef DEVICE_ENUM_SEGMENT
UINT8 flib_Host20_Enumerate (UINT8 bNormalEnumerate,UINT8 bAddress)
{
  UINT8 bCMD[8];
  UINT16 wLength;
  UINT16 ii, jj;
  //UINT32 i;
  //UINT8 bFaradayAP[4]={0x10,0x23,0x78,0x56};
  //UINT8 pbData[128];     //yuwen ,move to USB global SDRAM
  //UINT32 wSizeTemp;
  UINT8 bReturn;

  //printf("\r\n Enumerate",0);

  #if defined(Enable_Issue_TestPacket) && !defined(ENABLE_HOST_TEST)
  MY_HAL_DCACHE_FLUSH((U32)pUsbData, 128);
  IssueTestPacket(pUsbData);
  #endif

  bReturn=1;
  //sAttachDevice.bISOTransferEnable=0;
   gUsbTimeout=3;

  //<1>.GetDescriptor - Device (8 Bytes)
  MsOS_DelayTask(10);
  if (flib_Host20_FirstTransfer()>0)
  {
    return(0);//Fail
  }
   gUsbTimeout=11;           //extend usb timeout , for some really slow response HD

  //Set the ep0 max packet size

  pHost20_qHD_List_Control0->bMaxPacketSize=sAttachDevice.sDD.bEP0MAXPACKETSIZE;
  //psHost20_qHD_List_Control[0]->bMaxPacketSize1=sAttachDevice.sDD.bEP0MAXPACKETSIZE>>8;

  pHost20_qHD_List_Control1->bMaxPacketSize=sAttachDevice.sDD.bEP0MAXPACKETSIZE;
  //psHost20_qHD_List_Control[1]->bMaxPacketSize1=sAttachDevice.sDD.bEP0MAXPACKETSIZE>>8;

  //printf("max packet:%02bx\n",sAttachDevice.sDD.bEP0MAXPACKETSIZE);
  //printf("max packet:%02bx\n",Host20_qHD_List_Control0.bMaxPacketSize0);

  MsOS_DelayTask(10);
  #if 1
  //<2>.bNormalEnumerate =>Issue the Bus Reset
  //Issue Bus Reset
  //flib_Host20_Suspend();//Bruce;;add;;06102005
  //flib_Host20_TimerEnable(55);//Bruce;;add;;06102005
  //printf("bus reset again...\n");
  if(!NowIsHub)
  flib_Host20_PortBusReset();
  #endif
  //<2.2>.Get Descriptor again
  memcpy(bCMD,OTGH_GETDESCRIPTOR_DEVICE,8);

  if (flib_Host20_Issue_Control (0,bCMD,0x12,(UINT8*)&(sAttachDevice.sDD))>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);
  #if defined(Enable_Issue_TestPacket) && !defined(ENABLE_HOST_TEST)
  IssueTestPacket(pUsbData);
  #endif
  #ifdef Enable_SOF_Only
  printf("Repeat FS SOF...\n");
  while (1)
  {
    MsOS_DelayTask(100);
  }
  #endif


  //if (bNormalEnumerate>0)
  //  if (memcmp(bFaradayAP,&(sAttachDevice.sDD.bVIDLowByte),4)>0)
  //  {//This is not the Faraday Test AP
  //    printf(">>>Error:This is not the Faraday Test AP...\n");
  //    flib_DumpDeviceDescriptor(&(sAttachDevice.sDD));
  //    bReturn=2;
  //  }

  //<3>.Set Address to i
  memcpy(bCMD,OTGH_SETADDRESS,8);
  sAttachDevice.bAdd=bAddress;
  pHost20_qHD_List_Control1->bDeviceAddress=sAttachDevice.bAdd;
  bCMD[2]=sAttachDevice.bAdd;
  if (flib_Host20_Issue_Control(0,bCMD,0,OTGH_NULL)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(100);            //leave some time for device to be ready
  //<4>.GetDescriptor - Configuration (8Bytes)
  memcpy(bCMD,OTGH_GETDESCRIPTOR_CONFIG,8);
  //pbData=USBMALLOC(1000 );
  //for (i=0;i<1000;i++)
  //  *(pbData+i)=0;
  if (flib_Host20_Issue_Control (1,bCMD,0x08,pUsbData)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);

  wLength=*(pUsbData+2)+((*(pUsbData+3))<<8);

  //<5>.GetDescriptor - Configuration (Y-Bytes = wLength)
  if (wLength > CONTROL_BUF_LEN)
  {
    HOSTLIB_MSG(printf("Warning too long descriptor\n"));
    return(0);//Fail
  }

  bCMD[6]=*(pUsbData+2);//Low Byte
  bCMD[7]=*(pUsbData+3) ;//High Byte
  if (flib_Host20_Issue_Control (1,bCMD,wLength,pUsbData)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);


  //printf("pUsbData:%x\n",wLength);
  //Dump_Data((UINT16)pUsbData,wLength);
  flib_Host20_AnalysisConfigyration(pUsbData);

  //Give it a try

   //-----------do test mode -------------

#ifdef ENABLE_HOST_TEST
   if ((sAttachDevice.sDD.bVIDHighByte==0x1a)&&(sAttachDevice.sDD.bVIDLowByte==0x0a))
   {
      EnterTestMode();
   }
#endif
  //--- test mode end---------------

  //<6>.If OTG-Descriptor exist => then return to issue the HNP_Enable
  if (bNormalEnumerate==0)
  {//To check the OTG Descriptor
    if (sAttachDevice.sOTG.bED_OTG_bDescriptorType==9)
    {
      return(1);//Fail
    }
  }
  MsOS_DelayTask(10);

  //<7>.Get String
  //sAttachDevice.bStringLanguage[0]=0;
  //sAttachDevice.bStringManufacture[0]=0;
  //sAttachDevice.bStringProduct[0]=0;
  //sAttachDevice.bStringSerialN[0]=0;

  sAttachDevice.bStringLanguage[0]=0;
  sAttachDevice.bStringManufacture[0]=0;
  sAttachDevice.bStringProduct[0]=0;

  if ((sAttachDevice.sDD.bManufacturer == 0) && (sAttachDevice.sDD.bProduct == 0))
    goto Get_Str_Done;

  memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
  bCMD[6] = 4;
  if (flib_Host20_Issue_Control (1,bCMD,4, pUsbData)>0)
  {
    goto Get_Str_Done;
    //return(0);//Fail
  }
  MsOS_DelayTask(1);
  sAttachDevice.bStringLanguage[0] = pUsbData[2];
  sAttachDevice.bStringLanguage[1] = pUsbData[3];

  if (sAttachDevice.sDD.bManufacturer != 0)
  {
      memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
      bCMD[2] = sAttachDevice.sDD.bManufacturer;
      bCMD[4] = sAttachDevice.bStringLanguage[0];
      bCMD[5] = sAttachDevice.bStringLanguage[1];
      bCMD[6] = 2;

      if (flib_Host20_Issue_Control (1,bCMD,2, pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      bCMD[6] = pUsbData[0]; //real string length
      if (flib_Host20_Issue_Control (1,bCMD,bCMD[6], pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      for (ii=2, jj=0; ii<pUsbData[0]; ii+=2, jj++)
            sAttachDevice.bStringManufacture[jj] = pUsbData[ii];

      sAttachDevice.bStringManufacture[jj] = 0;

#if 0
      printf("bStringManufacture: ");

      for (ii=0; ii<64; ii++)
      {
        printf("%c", sAttachDevice.bStringManufacture[ii]);
        if (sAttachDevice.bStringManufacture[ii] == 0)
            break;
      }

      printf("\n");
#endif
  }

  if (sAttachDevice.sDD.bProduct != 0)
  {
      memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
      bCMD[2] = sAttachDevice.sDD.bProduct;
      bCMD[4] = sAttachDevice.bStringLanguage[0];
      bCMD[5] = sAttachDevice.bStringLanguage[1];
      bCMD[6] = 2;

      if (flib_Host20_Issue_Control (1,bCMD,2, pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      bCMD[6] = pUsbData[0]; //real string length
      if(bCMD[6] > 128)
      {//Alan.yu 2010.12.17 : for some bad devices return wrong length larger then 128, which will over-write the memory after pUsbData[]
        bCMD[6] = 128;
      }
      if (flib_Host20_Issue_Control (1,bCMD,bCMD[6], pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      for (ii=2, jj=0; ii<pUsbData[0]; ii+=2, jj++)
            sAttachDevice.bStringProduct[jj] = pUsbData[ii];

      sAttachDevice.bStringProduct[jj] = 0;

#if 0
      printf("bStringProduct: ");

      for (ii=0; ii<pUsbData[0]-2; ii++)
      {
        printf("%c", sAttachDevice.bStringProduct[ii]);
        if (sAttachDevice.bStringProduct[ii] == 0)
            break;
      }

      printf("\n");
#endif
  }
  //#endif //Skip Get String

Get_Str_Done:
  //<8>.Set Configurarion to 0
  memcpy(bCMD,OTGH_SETCONFIGURATION,8);
  bCMD[2]=sAttachDevice.saCD[0].bConfigurationValue;
  if (flib_Host20_Issue_Control (1,bCMD,0x00,OTGH_NULL)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(USB_ENUM_DELAY);            //leave some time for device to be ready

  //<9>.Printf the Device-Descriptor/Configuration-Descriptor

  return (bReturn);
}
#else // support device enumeration dividable
UINT8 _flib_Host20_Enumerate (UINT8 bNormalEnumerate,UINT8 bAddress)
{
  UINT8 bCMD[8];
  UINT16 wLength;
  UINT16 ii, jj;
  //UINT32 i;
  //UINT8 bFaradayAP[4]={0x10,0x23,0x78,0x56};
  //UINT8 pbData[128];     //yuwen ,move to USB global SDRAM
  //UINT32 wSizeTemp;
  UINT8 bReturn;

  //printf("\r\n Enumerate",0);

  #if defined(Enable_Issue_TestPacket) && !defined(ENABLE_HOST_TEST)
  MY_HAL_DCACHE_FLUSH((U32)pUsbData, 128);
  IssueTestPacket(pUsbData);
  #endif

  bReturn=1;
  //sAttachDevice.bISOTransferEnable=0;
   gUsbTimeout=3;

  //<1>.GetDescriptor - Device (8 Bytes)
  MsOS_DelayTask(10);
  if (flib_Host20_FirstTransfer()>0)
  {
    return(0);//Fail
  }
   gUsbTimeout=11;           //extend usb timeout , for some really slow response HD

  //Set the ep0 max packet size

  pHost20_qHD_List_Control0->bMaxPacketSize=sAttachDevice.sDD.bEP0MAXPACKETSIZE;
  //psHost20_qHD_List_Control[0]->bMaxPacketSize1=sAttachDevice.sDD.bEP0MAXPACKETSIZE>>8;

  pHost20_qHD_List_Control1->bMaxPacketSize=sAttachDevice.sDD.bEP0MAXPACKETSIZE;
  //psHost20_qHD_List_Control[1]->bMaxPacketSize1=sAttachDevice.sDD.bEP0MAXPACKETSIZE>>8;

  //printf("max packet:%02bx\n",sAttachDevice.sDD.bEP0MAXPACKETSIZE);
  //printf("max packet:%02bx\n",Host20_qHD_List_Control0.bMaxPacketSize0);

  MsOS_DelayTask(10);
  #if 1
  //<2>.bNormalEnumerate =>Issue the Bus Reset
  //Issue Bus Reset
  //flib_Host20_Suspend();//Bruce;;add;;06102005
  //flib_Host20_TimerEnable(55);//Bruce;;add;;06102005
  //printf("bus reset again...\n");
  if(!NowIsHub)
  flib_Host20_PortBusReset();
  #endif
  //<2.2>.Get Descriptor again
  memcpy(bCMD,OTGH_GETDESCRIPTOR_DEVICE,8);

  if (flib_Host20_Issue_Control (0,bCMD,0x12,(UINT8*)&(sAttachDevice.sDD))>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);
  #if defined(Enable_Issue_TestPacket) && !defined(ENABLE_HOST_TEST)
  IssueTestPacket(pUsbData);
  #endif
  #ifdef Enable_SOF_Only
  printf("Repeat FS SOF...\n");
  while (1)
  {
    MsOS_DelayTask(100);
  }
  #endif


  //if (bNormalEnumerate>0)
  //  if (memcmp(bFaradayAP,&(sAttachDevice.sDD.bVIDLowByte),4)>0)
  //  {//This is not the Faraday Test AP
  //    printf(">>>Error:This is not the Faraday Test AP...\n");
  //    flib_DumpDeviceDescriptor(&(sAttachDevice.sDD));
  //    bReturn=2;
  //  }

  //<3>.Set Address to i
  memcpy(bCMD,OTGH_SETADDRESS,8);
  sAttachDevice.bAdd=bAddress;
  pHost20_qHD_List_Control1->bDeviceAddress=sAttachDevice.bAdd;
  bCMD[2]=sAttachDevice.bAdd;
  if (flib_Host20_Issue_Control(0,bCMD,0,OTGH_NULL)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(100);            //leave some time for device to be ready
  //<4>.GetDescriptor - Configuration (8Bytes)
  memcpy(bCMD,OTGH_GETDESCRIPTOR_CONFIG,8);
  //pbData=USBMALLOC(1000 );
  //for (i=0;i<1000;i++)
  //  *(pbData+i)=0;
  if (flib_Host20_Issue_Control (1,bCMD,0x08,pUsbData)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);

  wLength=*(pUsbData+2)+((*(pUsbData+3))<<8);

  //<5>.GetDescriptor - Configuration (Y-Bytes = wLength)
  if (wLength > CONTROL_BUF_LEN)
  {
    HOSTLIB_MSG(printf("Warning too long descriptor\n"));
    return(0);//Fail
  }

  bCMD[6]=*(pUsbData+2);//Low Byte
  bCMD[7]=*(pUsbData+3) ;//High Byte
  if (flib_Host20_Issue_Control (1,bCMD,wLength,pUsbData)>0)
  {
    return(0);//Fail
  }
  MsOS_DelayTask(10);


  //printf("pUsbData:%x\n",wLength);
  //Dump_Data((UINT16)pUsbData,wLength);
  flib_Host20_AnalysisConfigyration(pUsbData);

  //Give it a try

   //-----------do test mode -------------

#ifdef ENABLE_HOST_TEST
   if ((sAttachDevice.sDD.bVIDHighByte==0x1a)&&(sAttachDevice.sDD.bVIDLowByte==0x0a))
   {
      EnterTestMode();
   }
#endif
  //--- test mode end---------------

  //<6>.If OTG-Descriptor exist => then return to issue the HNP_Enable
  if (bNormalEnumerate==0)
  {//To check the OTG Descriptor
    if (sAttachDevice.sOTG.bED_OTG_bDescriptorType==9)
    {
      return(1);//Fail
    }
  }
  MsOS_DelayTask(10);

  //<7>.Get String
  //sAttachDevice.bStringLanguage[0]=0;
  //sAttachDevice.bStringManufacture[0]=0;
  //sAttachDevice.bStringProduct[0]=0;
  //sAttachDevice.bStringSerialN[0]=0;

  sAttachDevice.bStringLanguage[0]=0;
  sAttachDevice.bStringManufacture[0]=0;
  sAttachDevice.bStringProduct[0]=0;

  if ((sAttachDevice.sDD.bManufacturer == 0) && (sAttachDevice.sDD.bProduct == 0))
    goto Get_Str_Done;

  memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
  bCMD[6] = 4;
  if (flib_Host20_Issue_Control (1,bCMD,4, pUsbData)>0)
  {
    goto Get_Str_Done;
    //return(0);//Fail
  }
  MsOS_DelayTask(1);
  sAttachDevice.bStringLanguage[0] = pUsbData[2];
  sAttachDevice.bStringLanguage[1] = pUsbData[3];

  if (sAttachDevice.sDD.bManufacturer != 0)
  {
      memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
      bCMD[2] = sAttachDevice.sDD.bManufacturer;
      bCMD[4] = sAttachDevice.bStringLanguage[0];
      bCMD[5] = sAttachDevice.bStringLanguage[1];
      bCMD[6] = 2;

      if (flib_Host20_Issue_Control (1,bCMD,2, pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      bCMD[6] = pUsbData[0]; //real string length
      if (flib_Host20_Issue_Control (1,bCMD,bCMD[6], pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      for (ii=2, jj=0; ii<pUsbData[0]; ii+=2, jj++)
            sAttachDevice.bStringManufacture[jj] = pUsbData[ii];

      sAttachDevice.bStringManufacture[jj] = 0;

#if 0
      printf("bStringManufacture: ");

      for (ii=0; ii<64; ii++)
      {
        printf("%c", sAttachDevice.bStringManufacture[ii]);
        if (sAttachDevice.bStringManufacture[ii] == 0)
            break;
      }

      printf("\n");
#endif
  }

  if (sAttachDevice.sDD.bProduct != 0)
  {
      memcpy(bCMD, OTGH_GETDESCRIPTOR_STR, 8);
      bCMD[2] = sAttachDevice.sDD.bProduct;
      bCMD[4] = sAttachDevice.bStringLanguage[0];
      bCMD[5] = sAttachDevice.bStringLanguage[1];
      bCMD[6] = 2;

      if (flib_Host20_Issue_Control (1,bCMD,2, pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      bCMD[6] = pUsbData[0]; //real string length
      if(bCMD[6] > 128)
      {//Alan.yu 2010.12.17 : for some bad devices return wrong length larger then 128, which will over-write the memory after pUsbData[]
        bCMD[6] = 128;
      }
      if (flib_Host20_Issue_Control (1,bCMD,bCMD[6], pUsbData)>0)
      {
        return(0);//Fail
      }
      MsOS_DelayTask(1);

      for (ii=2, jj=0; ii<pUsbData[0]; ii+=2, jj++)
            sAttachDevice.bStringProduct[jj] = pUsbData[ii];

      sAttachDevice.bStringProduct[jj] = 0;

#if 0
      printf("bStringProduct: ");

      for (ii=0; ii<pUsbData[0]-2; ii++)
      {
        printf("%c", sAttachDevice.bStringProduct[ii]);
        if (sAttachDevice.bStringProduct[ii] == 0)
            break;
      }

      printf("\n");
#endif
  }
  //#endif //Skip Get String

Get_Str_Done:
  //<8>.Set Configurarion to 0
  memcpy(bCMD,OTGH_SETCONFIGURATION,8);
  bCMD[2]=sAttachDevice.saCD[0].bConfigurationValue;
  if (flib_Host20_Issue_Control (1,bCMD,0x00,OTGH_NULL)>0)
  {
    return(0);//Fail
  }

  return (bReturn);
}
UINT8 flib_Host20_Enumerate (UINT8 bNormalEnumerate,UINT8 bAddress)
{
    UINT8 bReturn;

    bReturn = _flib_Host20_Enumerate(bNormalEnumerate, bAddress);
    if (!bReturn)
        return(bReturn);

    MsOS_DelayTask(USB_ENUM_DELAY);            //leave some time for device to be ready

    return (1);
}
#endif




//************************************************************************************************************
//************************************************************************************************************
//                          *** Group-4:Structure Function ***
//*************************************************************************************************************
//************************************************************************************************************
//====================================================================
// * Function Name: flib_Host20_InitStructure
// * Description:
//              1.Init qHD for Control
//                qHD_C-->qHD_C-->qHD_C
//              2.Init qHD for Bulk
//                |-------------------------|
//                qHD_C-->qHD_C-->qHD_B-->qHD_B
//
//              3.Init qHD for Interrupt
//              4.Init iTD for ISO (Reserved for feature)
// * Input:Type =0 =>iTD
//              =1 =>qTD
//              =2
// * OutPut: 0:Fail
//           1:ok
//====================================================================

void flib_Host20_InitStructure(void)
{
   U16   i;
   U8 *pData;

   //<1>.Clear memory

      // XBYTE[Host20_STRUCTURE_qHD_BASE_ADDRESS+i]=0x00;

    //for ( i=0 ; i < Host20_qTD_SIZE*Host20_qTD_MAX ; i++)
     //  XBYTE[Host20_STRUCTURE_qTD_BASE_ADDRESS+i]=0x00;
    //printf("UsbCtrlBuf size: %d\n", sizeof(UsbCtrlBuf));
    pUsbCtrlBuf=(U8*) KSEG02KSEG1(UsbCtrlBuf);
    MY_HAL_DCACHE_FLUSH((U32)UsbCtrlBuf, sizeof(UsbCtrlBuf));

  //  ASSERT(pUsbCtrlBuf != NULL)

    //printf("QtdBuf size: %d\n", sizeof(QtdBuf));
    qTD_Base_Buf=(U8*) KSEG02KSEG1(QtdBuf);
    MY_HAL_DCACHE_FLUSH((U32)QtdBuf, Host20_qTD_SIZE*Host20_qTD_MAX+0x20);

  //  ASSERT(qTD_Base_Buf != NULL)
    Host20_STRUCTURE_qTD_BASE_ADDRESS=qTD_Base_Buf;

    pData=qTD_Base_Buf;
    for (i=0 ; i < (Host20_qTD_SIZE*Host20_qTD_MAX+0x20) ; i++)
        pData[i]=0;

    if ((U32)Host20_STRUCTURE_qTD_BASE_ADDRESS& 0x10)
		Host20_STRUCTURE_qTD_BASE_ADDRESS+=0x10;			//make it aligned with 32
    pHost20_qHD_List_Control0=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Control0);
    pHost20_qHD_List_Control1=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Control1);
    pHost20_qHD_List_Bulk0=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Bulk0);
    pHost20_qHD_List_Bulk1=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Bulk1);
    MY_HAL_DCACHE_FLUSH((U32)&Host20_qHD_List_Control0, sizeof(qHD_Structure));
    MY_HAL_DCACHE_FLUSH((U32)&Host20_qHD_List_Control1, sizeof(qHD_Structure));
    MY_HAL_DCACHE_FLUSH((U32)&Host20_qHD_List_Bulk0, sizeof(qHD_Structure));
    MY_HAL_DCACHE_FLUSH((U32)&Host20_qHD_List_Bulk1, sizeof(qHD_Structure));

    pData=(U8*)pHost20_qHD_List_Control0;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(U8*)pHost20_qHD_List_Control1;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(U8*)pHost20_qHD_List_Bulk0;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;
    pData=(U8*)pHost20_qHD_List_Bulk1;
    for ( i=0 ; i < sizeof(qHD_Structure); i++)
        pData[i]=0;

    Host20_STRUCTURE_qHD_BASE_ADDRESS=(U8*)pHost20_qHD_List_Control0;

    DEBUG_USB(printf("qhd:%lx\n", (U32)pHost20_qHD_List_Control0));
   //<2>.For qTD & iTD & 4K-Buffer Manage init
   for (i=0;i<Host20_qTD_MAX;i++)
	  Host20_qTD_Manage[i]=Host20_MEM_FREE;


//   sAttachDevice.bDataBuffer=flib_Host20_GetStructure(Host20_MEM_TYPE_4K_BUFFER);//For Control


  //
 // printf("List_control 0:%x\n",(U16) pHost20_qHD_List_Control0);
#if 1
//psHost20_qHD_List_Control[0]->bType=HOST20_HD_Type_QH;
   flib_Host20_Allocate_QHD(pHost20_qHD_List_Control0,HOST20_HD_Type_QH,0,1,0,8);//Address=0,Head=1,EndPt=0,Size

   flib_Host20_Allocate_QHD(pHost20_qHD_List_Control1,HOST20_HD_Type_QH,1,0,0,64);//Address=1,Head=0,EndPt=0,Size

#endif



#if 1
   flib_Host20_Allocate_QHD(pHost20_qHD_List_Bulk0,HOST20_HD_Type_QH,1,0,1,64);//Address=1,Head=0,EndPt=1,Size
   flib_Host20_Allocate_QHD(pHost20_qHD_List_Bulk1,HOST20_HD_Type_QH,1,0,2,64);//Address=1,Head=0,EndPt=2,Size
#endif


   //<3.3>.Link the qHD (contol)
#if 1

     pHost20_qHD_List_Control0->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Control1)>>5);
     pHost20_qHD_List_Control1->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Control0)>>5);

  // SetPointer(&(pHost20_qHD_List_Control0->bNextQHDPointer3),VirtoPhyAddr((U16)pHost20_qHD_List_Control1));
    //SetPointer(&(pHost20_qHD_List_Control1->bNextQHDPointer3),VirtoPhyAddr((U16)pHost20_qHD_List_Control0));
   // SetPointer(&(Host20_qHD_List_Work.bNextQHDPointer3),VirtoPhyAddr((U16)&Host20_qHD_List_Work));

#endif

}

//====================================================================
// * Function Name: flib_Host20_GetStructure
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2 =>4K Buffer
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
qTD_Structure *flib_Host20_GetStructure( U8 Type)
{
    U32 i;
    U8 bFound;
    // U16 spTempqTD;
    qTD_Structure   *spTempqTD;
    // iTD_Structure  *spTempiTD;
    //  siTD_Structure  *spTempsiTD;
    bFound=0;

    switch(Type)
    {
        case Host20_MEM_TYPE_qTD:

            //For qTD

            for (i=0;i<Host20_qTD_MAX;i++)
                if (Host20_qTD_Manage[i]==Host20_MEM_FREE)
                {
                    bFound=1;
                    Host20_qTD_Manage[i]=Host20_MEM_USED;
                    break;
                }


            if (bFound==1)
            {
                //printf("USB base:%lx  \n",USB_BUFFER_START_ADR);

                spTempqTD=(qTD_Structure    *)((U32)Host20_STRUCTURE_qTD_BASE_ADDRESS+i*Host20_qTD_SIZE);
                memset((unsigned char   *)spTempqTD ,0, Host20_qTD_SIZE);
                spTempqTD->bTerminate=1;         //Bit0
                spTempqTD->bStatus_Active=0;             //Bit7
                spTempqTD->bInterruptOnComplete=1;   //Bit15

                spTempqTD->bAlternateTerminate=1;
                spTempqTD->bErrorCounter=3;
                return (spTempqTD);
            }
            else
            {
			HOSTLIB_MSG(printf("QTD underrun!\n"));
            }
            break;

        default:
            return 0;
            break;
    }

    //Not Found...
  //  while(1)
    //{
     //   printf("i:%d",i);
  //  }
    return (0);
}

//====================================================================
// * Function Name: flib_Host20_ReleaseStructure
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================



void flib_Host20_ReleaseStructure(U8 Type,U32 pwAddress)
{
    //U8 i;
    U16 wReleaseNum;
    U8  *pData;

 //   printf("release QTD:%x\n",pwAddress);
    pData=(U8*)pwAddress;

    switch(Type)
    {
        case Host20_MEM_TYPE_qTD:

            if (pwAddress<(U32)Host20_STRUCTURE_qTD_BASE_ADDRESS)
            {
                HOSTLIB_MSG(printf("??? Memory release area fail...\n"));
              //  while(1);
            }

            if ((pwAddress-(U32)Host20_STRUCTURE_qTD_BASE_ADDRESS)==0)
                wReleaseNum=0;
            else
                wReleaseNum=(pwAddress-(U32)Host20_STRUCTURE_qTD_BASE_ADDRESS)/Host20_qTD_SIZE;

            if (wReleaseNum>=Host20_qTD_MAX)//ERROR FIX Prevent Tool 070522
            {
                HOSTLIB_MSG(printf("Memory release area fail...\n"));
                return;
              //  while(1);
            }

            Host20_qTD_Manage[wReleaseNum]=Host20_MEM_FREE;

            // Removed, for improve the performance,
            // because we will clear qtd structure when we get it, so don¡¦t have to do it when release it.
            //for (i=0;i<Host20_qTD_SIZE ;i++) //qTD size=32 bytes
            //  *(pData+i)=0;
            break;

        default:
           // printf("??? Memory release type fail...\n");
          //  while(1);
            break;

    }


}
//====================================================================
// * Function Name: flib_Host20_QHD_Control_Init
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================
void flib_Host20_QHD_Control_Init(void)
{


  //<1>.Init Control-0/1
  pHost20_qHD_List_Control0->bEdSpeed=gSpeed;
  //printf("bEdSpeed:%bx\n",sAttachDevice.bSpeed);
  pHost20_qHD_List_Control0->bInactiveOnNextTransaction=0;
  pHost20_qHD_List_Control0->bDataToggleControl=1;

  pHost20_qHD_List_Control1->bEdSpeed=gSpeed;
  pHost20_qHD_List_Control1->bInactiveOnNextTransaction=0;
  pHost20_qHD_List_Control1->bDataToggleControl=1;

  //<2>.Init Bulk-0/1
  pHost20_qHD_List_Bulk0->bEdSpeed=gSpeed;
  pHost20_qHD_List_Bulk0->bInactiveOnNextTransaction=0;
  pHost20_qHD_List_Bulk0->bDataToggleControl=0;

  pHost20_qHD_List_Bulk1->bEdSpeed=gSpeed;
  pHost20_qHD_List_Bulk1->bInactiveOnNextTransaction=0;
  pHost20_qHD_List_Bulk1->bDataToggleControl=0;


  //printf("enable aynch \n");
  //<12>.Enable Asynchronous

  //mbHost20_USBCMD_AsynchronousEnable_Set();    //Temp;;Bruce
  //printf("0x10:%02bx",XBYTE[0x2410]);


}

//====================================================================
// * Function Name: flib_Host20_Allocate_QHD
// * Description:
//
// * Input:Type =0 =>qTD
//              =1 =>iTD
//              =2
// * OutPut: 0:Fail
//           ~0:Addrress
//====================================================================

void flib_Host20_Allocate_QHD(qHD_Structure  *psQHTemp,U8 bNextType,U8 bAddress,U8 bHead,U8 bEndPt, U32 wMaxPacketSize)
{
    qTD_Structure  *spTempqTD;
    // U32 sp;

    psQHTemp->bTerminate=0;             //Bit0
    psQHTemp->bType=bNextType;          //Bit2~1

    psQHTemp->bDeviceAddress=bAddress;             //Bit0~6
    psQHTemp->bEdNumber=bEndPt;                  //Bit11~8
    psQHTemp->bHeadOfReclamationListFlag=bHead; //Bit15
    psQHTemp->bMaxPacketSize=wMaxPacketSize;            //Bit16~26
    psQHTemp->bNakCounter=Host20_QHD_Nat_Counter;

    psQHTemp->bOverlay_NextTerminate=1;
    psQHTemp->bOverlay_AlternateNextTerminate=1;

    //<2>.allocate dumy qTD
#if 1
    //<2.1>.Allocate qTD
    spTempqTD=flib_Host20_GetStructure(0);//0=>qTD
    psQHTemp->bOverlay_NextqTD=(VirtoPhyAddr((U32)spTempqTD)>>5);

#endif
    //<2.3>.Active the qTD
    psQHTemp->bOverlay_NextTerminate=0;

}

//====================================================================
// * Function Name: flib_Host20_CheckingForResult_QHD
// * Description:
// * Input:Type
// * OutPut:
//====================================================================
UINT8 flib_Host20_CheckingForResult_QHD(qHD_Structure *spTempqHD)
{
  UINT8 bQHStatus;
  U16 wIntStatus;

  wIntStatus=mwHost20_USBSTS_Rd();
  //printf("USB int:%x\n",wIntStatus);

  //<2>.Checking for the Error type interrupt => Halt the system
  if (wIntStatus&HOST20_USBINTR_SystemError)
  {
    DEBUG_USB(printf("???System Error... Halt the system...\n "));
    sAttachDevice.bSendStatusError=1;

  }
  if (wIntStatus&HOST20_USBINTR_USBError) {
    mwHost20_USBSTS_USBError_Set();
    sAttachDevice.bSendStatusError=1;
  }

  if (sAttachDevice.bSendStatusError==0)
    return HOST20_OK;

  //<1>.Analysis the qHD Status
  sAttachDevice.bSendStatusError=0;
  //printf("??? USB Error Interrupt Event...\n");
  bQHStatus=spTempqHD->bOverlay_Status;

  if (bQHStatus&HOST20_qTD_STATUS_Halted)
  {
    if (bQHStatus&HOST20_qTD_STATUS_TransactionError)
    {
      //printf("\r\n transaction error!!",0);
      gUsbStatus=USB_TRANS_ERROR;              //trans error
      return HOST20_TRANSACTION_ERROR;
    }

    //printf("??? qHD Status => Halted (<1>.Stall/<2>.Babble/<3>.Error Counter=0)...(Device Not Supported...)\n");
    spTempqHD->bOverlay_Status=0;      //clear halt status
    spTempqHD->bOverlay_TotalBytes=0;
    spTempqHD->bOverlay_Direction=0;
    spTempqHD->bOverlay_CurrentOffset=0;   //clear offset
    if (bQHStatus==HOST20_qTD_STATUS_Halted)            //no other error status
    {
      //printf("STALL\n");
      return HOST20_DEVICE_STALL;
    }
  }
  if (bQHStatus&HOST20_qTD_STATUS_BufferError)
  {
    // kcj 2007-06-07	printf("??? qHD Status => HOST20_qTD_STATUS_BufferError...\n");
  }
  if (bQHStatus&HOST20_qTD_STATUS_Babble)
  {
    DEBUG_USB(printf("??? qHD Status => HOST20_qTD_STATUS_Babble...\n"));
    //sAttachDevice.bSendStatusError=0;			//don't fix babble error for Bert
    #ifdef Enable_Low_Temperature_Patch
	if(gSpeed==0)
	{
	  //printf("\r\n CDR toggle!!",0);
          UTMI_SETXBYTE(0x0a,UTMI_READXBYTE(0x0a)^0x10);    //invert CDR_CLOCK
          UTMI_ORXBYTE(0x06,0x03);  //reset UTMI
          UTMI_ANDXBYTE(0x06,0xfc);
	}
	#endif
    spTempqHD->bOverlay_Status=0;
    return HOST20_OK;

  }
  if (bQHStatus&HOST20_qTD_STATUS_MissMicroFrame)
  {
    // kcj 2007-06-07	printf("??? qHD Status => HOST20_qTD_STATUS_MissMicroFrame...\n");
  }
  //<2>.Clear the status
  spTempqHD->bOverlay_Status=0;

  return HOST20_FAIL;


}

#if defined(CPU_TYPE_MIPS) || defined(CPU_TYPE_ARM)
extern void MsOS_FlushMemory(void);
extern void MsOS_ReadMemory(void);
void USB_Chip_Read_Memory(void)
{
#if 0
    volatile unsigned int *pu8;
    volatile unsigned int t ;

	// Transfer the memory to noncache memory
    pu8 = ((volatile unsigned int *)0xA0380000);
    t = pu8[0] ;
    t = pu8[64] ;
#else
    MsOS_ReadMemory();
#endif
}
void USB_Chip_Flush_Memory(void)

{
    MsOS_FlushMemory();
}
#endif

#if 1

//====================================================================
// * Function Name: flib_Host20_Send_qTD
// * Description:
//   Case-1:1qTD
//   Case-2:2qTD
//   Case-3:3qTD above
// * Input:Type
// * OutPut: 0 => OK
//           1 => TimeOut
//====================================================================
UINT8 flib_Host20_Send_qTD(qTD_Structure  *spHeadqTD ,qHD_Structure  *spTempqHD,U16 wTimeOutSec)
{
  UINT8 bExitLoop,bReturnValue;
  qTD_Structure   *spNewDumyqTD;
  qTD_Structure   *spOldDumyqTD;
  qTD_Structure   *spReleaseqTD;
  qTD_Structure   *spReleaseqTDNext;
  qTD_Structure   *spLastqTD;
  //UINT32 wDummyTemp;
  //UINT32 xdata *pwData;
  #ifdef ATV_SERISE_USE
  UINT32 wTimes;
  UINT32 StartTime=0;
  #else
  UINT16 wTimes;
  UINT32 StartTime;
  UINT32 LastTime;
  UINT16 FrameIdx=0;

  #endif

  #ifdef ATV_SERISE_USE
  wTimeOutSec=wTimeOutSec;
  #endif

  gUsbStatus=USB_OK;
  //if (wTimeOutSec==0)
  //{
  //  DEBUG_USB(printf("error, timeout sec is zero\n"));
  //}
  spOldDumyqTD=(qTD_Structure*)PhytoVirAddr((( U32)(spTempqHD->bOverlay_NextqTD))<<5);

  //spTempqHD->bOverlay_Status|=HOST20_qTD_STATUS_Halted;
  while (mbHost20_USBCMD_AsynchronousEnable_Rd())
  {
    mbHost20_USBCMD_AsynchronousEnable_Clr();//pause asynchronous scheduler
  }

  //spHeadqTD->bTerminate=1;			//set to terminate
  memcpy(spOldDumyqTD,spHeadqTD,Host20_qTD_SIZE);
  //spOldDumyqTD->bStatus_Halted=1;
  if(RecoveryFlag&0x1)
    spOldDumyqTD->bDataToggle=DataToggleBackup;

  //spOldDumyqTD->bStatus_Active=0;

  //<2>.Prepare new dumy qTD
  spNewDumyqTD=spHeadqTD;
  memset((void *)spNewDumyqTD ,0, Host20_qTD_SIZE);
  spNewDumyqTD->bTerminate=1;
  //spNewDumyqTD->bAlternateTerminate=1;
  //spNewDumyqTD->bStatus_Halted=1;
  //<3>.Find spLastqTD & link spLastqTD to NewDumyqTD & Set NewDumyqTD->T=1
  spLastqTD=spOldDumyqTD;
  while(spLastqTD->bTerminate==0) {
         spLastqTD=(qTD_Structure*)PhytoVirAddr((( U32)(spLastqTD->bNextQTDPointer))<<5);

  };

  spLastqTD->bNextQTDPointer=(VirtoPhyAddr((U32)spNewDumyqTD)>>5);

  spLastqTD->bTerminate=0;

  //Link Alternate qTD pointer
    spLastqTD->bAlternateQTDPointer=(VirtoPhyAddr(( U32)spNewDumyqTD)>>5);

  spLastqTD->bAlternateTerminate=0;



  //<4>.Set OldDumyqTD->Active=1
  //gwLastqTDSendOK=0;
  //sAttachDevice.psSendLastqTD=spLastqTD;
  sAttachDevice.bSendStatusError=0;

  //Dump_QTD(spOldDumyqTD);
  //Dump_QTD(spNewDumyqTD);

  while (mwHost20_USBSTS_Rd() & 0x3b)
  {
    mwHost20_USBSTS_Set(0x3b);      //clear interrupt, don't clear port change int
  }
  //bExitLoop=0;
  //spOldDumyqTD->bStatus_Halted=0;
  //spOldDumyqTD->bStatus_Halted=0;

#if 0
    printf("spOldDumyqTD: %X\n", spOldDumyqTD);
    printf("bTerminate: %X\n", spOldDumyqTD->bTerminate);
    printf("bNextQTDPointer: %X\n", spOldDumyqTD->bNextQTDPointer);
    printf("bAlternateTerminate: %X\n", spOldDumyqTD->bAlternateTerminate);
    printf("bAlternateQTDPointer: %X\n", spOldDumyqTD->bAlternateQTDPointer);
    printf("bStatus_PingState: %X\n", spOldDumyqTD->bStatus_PingState);
    printf("bStatus_SplitState: %X\n", spOldDumyqTD->bStatus_SplitState);
    printf("bStatus_MissMicroFrame: %X\n", spOldDumyqTD->bStatus_MissMicroFrame);
    printf("bStatus_Transaction_Err: %X\n", spOldDumyqTD->bStatus_Transaction_Err);
    printf("bStatus_Babble: %X\n", spOldDumyqTD->bStatus_Babble);
    printf("bStatus_Buffer_Err: %X\n", spOldDumyqTD->bStatus_Buffer_Err);
    printf("bStatus_Halted: %X\n", spOldDumyqTD->bStatus_Halted);
    printf("bStatus_Active: %X\n", spOldDumyqTD->bStatus_Active);
    printf("bPID: %X\n", spOldDumyqTD->bPID);
    printf("bErrorCounter: %X\n", spOldDumyqTD->bErrorCounter);
    printf("CurrentPage: %X\n", spOldDumyqTD->CurrentPage);
    printf("bInterruptOnComplete: %X\n", spOldDumyqTD->bInterruptOnComplete);
    printf("bTotalBytes: %X\n", spOldDumyqTD->bTotalBytes);
    printf("bDataToggle: %X\n", spOldDumyqTD->bDataToggle);
    printf("ArrayBufferPointer_Word: %X\n", spOldDumyqTD->ArrayBufferPointer_Word[0]);
#endif
    //flib_Host20_StopRun_Setting(HOST20_Enable);
    //printf("RunStop: %X\n", mbHost20_USBCMD_RunStop_Rd());
  spOldDumyqTD->bStatus_Active=1;
#if 1
  //20120912, temp patch for USB HW can't update currentQTD
  if ( (MDrv_USBGetChipID() == CHIPID_EDEN) )
      spTempqHD->bOverlay_CurrentqTD=VirtoPhyAddr((U32)spOldDumyqTD);
#endif
#if defined(CPU_TYPE_MIPS) || defined(CPU_TYPE_ARM)
      USB_Chip_Flush_Memory();


#endif

  while (mbHost20_USBCMD_AsynchronousEnable_Rd()==0)
  {
    mbHost20_USBCMD_AsynchronousEnable_Set();//re start asynchronous scheduler
  }
  //spTempqHD->bOverlay_Status&=~HOST20_qTD_STATUS_Halted;

  //wait until asynchronous scheduler is idle

  //mbHost20_USBCMD_RunStop_Set();

  //spTempqHD->bOverlay_Status&=~0x40;			//clr HALT bit, start this queue head
  //XBYTE[0x2410]|=1;//start run
  //wDummyTemp=0;
  wTimes=0;
  #ifndef ATV_SERISE_USE
  StartTime=MsOS_GetSystemTime();
   LastTime=StartTime;
   FrameIdx=mwHost20_FrameIndex_Rd();
  #endif
  //<5>.Waiting for result
  //EAL=0;
  while (1)
  {
    #ifndef ATV_SERISE_USE
    msAPI_Timer_ResetWDT();
    #endif

    #ifdef ATV_SERISE_USE
    wTimes++;
    #endif
    //if ((spOldDumyqTD->bStatus_Active==0) &&
	//    ( mwHost20_USBSTS_Rd()& HOST20_USBINTR_CompletionOfTransaction))              //wait until Status_Active become 0
	if(mwHost20_USBSTS_Rd()& HOST20_USBINTR_CompletionOfTransaction)             //wait until Status_Active become 0
    {

#if defined(CPU_TYPE_MIPS) || defined(CPU_TYPE_ARM)
        USB_Chip_Read_Memory();
#endif
        if (spOldDumyqTD->bStatus_Active==0)
        {
          bReturnValue=HOST20_OK;
          break;
        }
      //bExitLoop=1;
      //printf("bReturn:%02bx\n",    bReturnValue);
    }
    //if (bExitLoop) break;
    #if 1
    if (mwHost20_USBSTS_Rd()&(HOST20_USBINTR_SystemError+HOST20_USBINTR_USBError))
    {
      bReturnValue=HOST20_OK;
      //if (spOldDumyqTD->bStatus_Active==1)
	  //{
	  //  printf("something wrong..USBINTR:%02bx\n",mwHost20_USBSTS_Rd());
	  //  printf("QH status:%02bx\n",spTempqHD->bOverlay_Status);
	  //}
      break;            //USB interrupt happened
    }
    #endif
    #if 1
    //if (gwOTG_Timer_Counter>wTimeOutSec)
    //  wTimes++;
    //if (wTimes > wTimeOutSec*400)
    if (mwHost20_PORTSC_ConnectStatus_Rd()==0)
    {
        //return HOST20_FATAL;
        bReturnValue = HOST20_FATAL;
        break;
    }
    //if (mwHost20_PORTSC_ConnectChange_Rd())
    //{
    //  gDeviceFatalError=TRUE;
    //  mwHost20_PORTSC_ConnectChange_Set();
    //  return HOST20_FATAL;          //usb has been plug out and in
    //}
    #ifndef ATV_SERISE_USE
#if 1
      if (MsOS_Timer_DiffTimeFromNow(LastTime) >(U32)3)     //over 1 mini sec
        {

            if (mwHost20_FrameIndex_Rd()==FrameIdx)
            {
                HOSTLIB_MSG(printf("1C stop,timeout !!\n"));
                gUsbStatus=USB_TIMEOUT;         //ESD , USB hang,should be timeout
                bReturnValue=HOST20_FATAL;
                break;
            }
            FrameIdx=mwHost20_FrameIndex_Rd();
            LastTime=MsOS_GetSystemTime();

        }
#endif
    if (MsOS_Timer_DiffTimeFromNow(StartTime) > (U32)wTimeOutSec *1000)
    #else
    if(wTimes>0x80000)
    #endif
    {
      #if 0
      printf("\nQH1:\n");
	  Dump_Data(0xf800,0x30);
	  printf("\nQH2:\n");
	  Dump_Data(0xf840,0x30);
	  printf("\nQH3:\n");
	  Dump_Data(0xf880,0x30);
	  printf("\nQH4:\n");
	  Dump_Data(0xf8c0,0x30);
	  printf("\n");
	  Dump_Data(0xfa80,0x20);
	  printf("\n");
	  Dump_Data(0xfb00,0x20);
	  printf("\n");
	  Dump_Data(0xfae0,0x20);
	  printf("\n");
	  Dump_Data(0xfaa0,0x20);
	  printf("\n");
	  Dump_Data(0xfac0,0x20);

      printf("\nQH->%x\n",(U16)spTempqHD);
      //Dump_Data((U16)spTempqHD,0x30);

      printf("\nqtd->\n");
      Dump_Data((U16)spOldDumyqTD,0x20);
	  printf("\nEHCI REG:\n");
      Dump_Data(0x2400,0x50);
      #endif
      bExitLoop=2;
      bReturnValue=HOST20_FATAL;
      gUsbStatus=USB_TIMEOUT;
      //gDeviceFatalError=TRUE;

      //MINI_DEBUG2(printf("Time Out:%02bx\n",XBYTE[gUHC_BASE+30]));
      break;
    }

    #endif
    //}
  }
  #ifndef ATV_SERISE_USE
  msAPI_Timer_ResetWDT();       //in case spend too much time at polling
  #endif
  while (mbHost20_USBCMD_AsynchronousEnable_Rd())
  {
    mbHost20_USBCMD_AsynchronousEnable_Clr();//pause asynchronous scheduler
  }

  //<6>.Checking the Result
  if (bReturnValue!=HOST20_FATAL)
    bReturnValue=flib_Host20_CheckingForResult_QHD(spTempqHD);
  if (bReturnValue==HOST20_TRANSACTION_ERROR)
  {
    //printf("Transaction Error\n");

    spTempqHD->bOverlay_Status=0;      //clear halt status
    //SetPointer(&(spTempqHD->bOverlay_NextqTD3),0);
    //SetPointer(&(spTempqHD->bOverlay_AlternateqTD3),0);
   // SetPointer(&(spTempqHD->bOverlay_CurqTD3),VirtoPhyAddr((UINT16)spNewDumyqTD));
    spTempqHD->bOverlay_CurrentqTD=(VirtoPhyAddr((U32)spNewDumyqTD)>>5);
    //printf("TotalBytes:%x\n",TotalBytes);

    //printf("spNewDumyqTD->%x\n",(U16)spNewDumyqTD);
    //printf("spOldDumyqTD->%x\n",(U16)spOldDumyqTD);
    //Dump_Data((U16)spOldDumyqTD,0x20);
    //printf("\nQH->%x\n",(U16)spTempqHD);
    //Dump_Data((U16)spTempqHD,0x20);

  }
  TotalBytes=  spOldDumyqTD->bTotalBytes;

  //<5>.Release the all the qTD (Not include spNewDumyqTD)
  #if 1
  spReleaseqTD=spOldDumyqTD;
  do {
    //spReleaseqTDNext=((UINT32)(spReleaseqTD->bNextQTDPointerL))<<5 + ((UINT32)(spReleaseqTD->bNextQTDPointerH))<<16  ;
   spReleaseqTDNext=(qTD_Structure*)PhytoVirAddr(((U32)(spReleaseqTD->bNextQTDPointer))<<5);

    DataToggleBackup=spOldDumyqTD->bDataToggle;

        flib_Host20_ReleaseStructure(Host20_MEM_TYPE_qTD,(U32)spReleaseqTD);

 spReleaseqTD=spReleaseqTDNext;
  } while(((UINT32)spReleaseqTD)!=((UINT32)spNewDumyqTD));
  #endif

  // Double Check the QH overlay status. Adjust it if need.
  if( spTempqHD->bOverlay_NextqTD != (VirtoPhyAddr(( U32)spNewDumyqTD)>>5)
    || spTempqHD->bOverlay_Status & BIT7 )
  {
    // If chain qTDs after disconnection/qTD timeout, QH overlay will not be advanced by HW.
    // It muss up qTD chain layout. QH doesn't stop at old dumy and stare at new dumy.
    // SW advance QH overlay manually no matter HW advancing or not.
    // Run bit is cleared by HQ when disconnection, so it is safe to modify the QH.

    HOSTLIB_MSG(printf("[Warning] Adjust bad qTD chain..\r\n"));

    spTempqHD->bOverlay_Status = spTempqHD->bOverlay_Status & ~BIT7;
    spTempqHD->bOverlay_CurrentqTD = (VirtoPhyAddr(( U32)spOldDumyqTD)>>5);
    spTempqHD->bOverlay_NextqTD = (VirtoPhyAddr(( U32)spNewDumyqTD)>>5);
    spTempqHD->bOverlay_AlternateqTD = (VirtoPhyAddr(( U32)spNewDumyqTD)>>5); 
  }

 return (bReturnValue);
}
#endif

#ifdef ENABLE_HOST_TEST
extern void MDrv_UsbSendSE0(void);
extern void MDrv_UsbSendTestJ(void);
extern void MDrv_UsbSendTestK(void);
extern void MDrv_UsbSendTestPacket(void);

UINT8 flib_Host20_Issue_Control_Test_Mode (UINT8 bEdNum,UINT8* pbCmd,UINT16 hwDataSize,UINT8* pbData)
{
    qTD_Structure *spTempqTD;
    //UINT16       bpDataPage;
    UINT8        bReturnValue;
    UINT8 ii;
    //UINT8 i;
    qHD_Structure xdata *qh_ptr;

    if (bEdNum==0)
        qh_ptr=pHost20_qHD_List_Control0;
    else
        qh_ptr=pHost20_qHD_List_Control1;

    //<0>.Allocate qTD & Data Buffer
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    //bpDataPage=sAttachDevice.bDataBuffer;           //2//2k buffer
    //bpDataPage=sAttachDevice.bDataBuffer;
    //<2.1>.Setup packet
    //<A>.Fill qTD
    spTempqTD->bPID=HOST20_qTD_PID_SETUP;                   //Bit8~9
    spTempqTD->bTotalBytes=8;           //Bit16~30

    spTempqTD->bDataToggle=0;            //Bit31
    //FillBufferArray(spTempqTD, bpDataPage);
    memcpy(pUsbCtrlBuf,pbCmd,8);

    spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);

    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        return bReturnValue;

    //MsOS_DelayTask(15000);          //FOR TEST MODE
    for (ii=0; ii<150; ii++)
    {
    	MsOS_DelayTask(100);
	msAPI_Timer_ResetWDT();
    }
             //<A>.Fill qTD
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    spTempqTD->bPID=HOST20_qTD_PID_IN;                   //Bit8~9
    spTempqTD->bTotalBytes=hwDataSize;           //Bit16~30

    spTempqTD->bDataToggle=1;            //Bit31

    spTempqTD->ArrayBufferPointer_Word[0]=VirtoPhyAddr((U32)pUsbCtrlBuf);

    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    if (bReturnValue>0)
        return bReturnValue;
             //printf("c4");


    //<C>.Waiting for result
    memcpy(pbData,pUsbCtrlBuf,hwDataSize);

    //Dump_Data((UINT16)pbData,hwDataSize);

    //<2.3>.Out packet

    //<A>.Fill qTD
    spTempqTD=flib_Host20_GetStructure(Host20_MEM_TYPE_qTD);//0=>qTD
    spTempqTD->bPID=HOST20_qTD_PID_OUT;                   //Bit8~9
    spTempqTD->bTotalBytes=0;           //Bit16~30

    spTempqTD->bDataToggle=1;            //Bit31

    //<B>.Send qTD
    //bReturnValue=flib_Host20_Send_qTD(spTempqTD ,psHost20_qHD_List_Control[bEdNum],5);
    bReturnValue=flib_Host20_Send_qTD(spTempqTD ,qh_ptr,gUsbTimeout);

    return (bReturnValue);
}

void GetDeviceDescriptor(void)
{
    UINT8 bCMD[8];

    memcpy(bCMD,OTGH_GETDESCRIPTOR_DEVICE_8,8);

    flib_Host20_Issue_Control (1,bCMD,8,(UINT8*)&(sAttachDevice.sDD));
}

void EnterTestMode(void)
{
  UINT8 ii;
  UINT8 bCMD[8];
#if 0
    if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x06))
        {
            //--- do suspend resume test
            // first, SOF for 15 seconds
              StartTime=MDrv_Timer_GetTime0();

            printf("\r\n start sending SOF..",0);
            while (1)
                {
                   msAPI_Timer_ResetWDT();       //in case watchdog timer is too short

                  if ((MDrv_Timer_GetTime0()-StartTime) >  1000)     //SOF for 15 sec
                        break;

                }
            printf("\r\n  suspend",0);
            mbHost20_USBCMD_RunStop_Clr();
                mwHost20_PORTSC_ForceSuspend_Set(); //suspend the device
                   StartTime=MDrv_Timer_GetTime0();

                while (1)
                {
                  msAPI_Timer_ResetWDT();       //in case watchdog timer is too short

                  if ((MDrv_Timer_GetTime0()-StartTime) >  1000)     //SOF for 15 sec
                        break;
                }
                printf("\r\n  resume\n",0);
              mwHost20_PORTSC_ForceResume_Set();
               mbHost20_USBCMD_RunStop_Set();

        }
   else
#endif
    if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x01))
        // test_SE0
    {
        MDrv_UsbSendSE0();
     }
    else  if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x02))
        // test_J
    {
        MDrv_UsbSendTestJ();
     }
    else  if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x03))
        // test_J
    {
        MDrv_UsbSendTestK();
     }
    else  if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x04))
        // test_J
    {
        MDrv_UsbSendTestPacket();
     }
    else  if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x07))
     //single step get device descriptor
    {
        printf("\r\n start sending SOF..\n");

	 for (ii=0; ii<150; ii++)
 	{
	       MsOS_DelayTask(100);
		msAPI_Timer_ResetWDT();
 	}

        printf("\r\n Get device descriptor.\n");

        GetDeviceDescriptor();
    }
    else  if ((sAttachDevice.sDD.bPIDHighByte==0x01)&&(sAttachDevice.sDD.bPIDLowByte==0x08))
     //single step get device descriptor
    {
        printf("\r\n Get device descriptor.\n");
        memcpy(bCMD,OTGH_GETDESCRIPTOR_DEVICE,8);

        flib_Host20_Issue_Control_Test_Mode (1,bCMD,0x12,(UINT8*)&(sAttachDevice.sDD));
    }

}

#endif //ENABLE_HOST_TEST

#endif  //#if defined(MSOS_TYPE_NOS)


