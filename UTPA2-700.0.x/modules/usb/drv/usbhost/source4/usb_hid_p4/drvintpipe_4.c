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
#include "../USBHostConfig_4.h"
#include "../../include/datatype.h"
#include "../drvHost200_4.h"
#include "../drvHostLib_4.h"
#include "../drvUsbMain_4.h"

#include "../../include/_drvUSB.h"
#include "../../include/hal_arch.h"
#include "../../include/hal_cache.h"
#include <string.h>
#include "../drvscsi_4.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#define USB_HID_INT_DBG(x) // x
#define USB_HID_INT_MSG(x)		MS_CRITICAL_MSG(x)

extern void msAPI_Timer_ResetWDT(void);

#define   FRAME_LIST_SIZE                           (256*4)

Periodic_Frame_List_Structure  Host20_FramList_Port4 __attribute__ ((aligned (4096)));
Periodic_Frame_List_Structure   *pHost20_FramList_Port4;
qHD_Structure  Host20_qHD_List_Intr_Port4    __attribute__ ((aligned (32)));
qHD_Structure *pHost20_qHD_List_Intr_Port4;
extern  Host20_Attach_Device_Structure sAttachDevice_Port4;
//extern Host20_Attach_Device_Structure  *psAttachDevice_Port4;
extern UINT8 gSpeed_Port4;
extern U8 gUsbStatus_Port4;

extern UINT8 flib_Host20_CheckingForResult_QHD_Port4(qHD_Structure *spTempqHD);
extern void ResetUsbHardwarePort4(void);
extern U8 usb_temp_buf_Port4[Scsi_Max_Transfer_Len];

void SetFrameList_Port4(UINT8 i)
{
    pHost20_FramList_Port4->sCell[i].bTerminal=0;
    pHost20_FramList_Port4->sCell[i].bType=HOST20_HD_Type_QH;
    //SetPointer(&(Host20_FramList.sCell[i].bLinkPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Intr));
    pHost20_FramList_Port4->sCell[i].bLinkPointer = (VirtoPhyAddr((U32)pHost20_qHD_List_Intr_Port4)>>5);
}

//====================================================================
// * Function Name: flib_Host20_Interrupt_Init
// * Description:
//   //Reserve=> <1>.If Periodic Enable => Disable Periodic
//   <2>.Disable all the Frame List (Terminal=1)
//   <3>.Hang the Interrupt-qHD-1 to Frame List
//
// * Input: wInterval=1~16 => Full Speed => 1ms ~ 32 sec
//                            High Speed => 125us ~ 40.96 sec
// * OutPut:
//====================================================================
void  flib_Host20_Interrupt_Init_Port4(UINT8 bAddr)
{
    UINT32 i,j;
    // UINT32 wSize;
    UINT8 *pData;
    //UINT16 wForceInterval=0;
    UINT8 EndPaddr;
    //struct usb_interface_descriptor *ip;

    //  ip=&dev->config.if_desc[0];

    //<1>.Disable the Periodic
    flib_Host20_Periodic_Setting_Port4(HOST20_Disable);

    //<2>.Init qHD for Interrupt(7~9) Scan the ED

    mbHost20_USBCMD_FrameListSize_Set(HOST20_USBCMD_FrameListSize_256);
    pHost20_qHD_List_Intr_Port4=(qHD_Structure*)KSEG02KSEG1(&Host20_qHD_List_Intr_Port4);
    pHost20_FramList_Port4= (Periodic_Frame_List_Structure*) KSEG02KSEG1(&Host20_FramList_Port4);
    //printf("pHost20_qHD_List_Intr: %X\n", pHost20_qHD_List_Intr);
    MY_HAL_DCACHE_FLUSH((U32)&Host20_qHD_List_Intr_Port4, sizeof(qHD_Structure));
    MY_HAL_DCACHE_FLUSH((U32)&Host20_FramList_Port4, sizeof(Periodic_Frame_List_Structure));

    pData=(UINT8*)pHost20_qHD_List_Intr_Port4;
    for (j=0;j<sizeof(qHD_Structure);j++)
        *(pData+j)=0;

    //  printf("battribute:%02bx\n", sAttachDevice.saCD[0].sInterface[0].sED[0].bED_bmAttributes);
    EndPaddr=  sAttachDevice_Port4.saCD[0].sInterface[0].sED[0].bED_EndpointAddress & 0x7f;
    USB_HID_INT_DBG(printf("interrupt addr:%02x\n",EndPaddr));
    flib_Host20_Allocate_QHD_Port4(pHost20_qHD_List_Intr_Port4,HOST20_HD_Type_QH,bAddr,0,EndPaddr,8);//Address=0,Head=1,EndPt=0,Size

    pHost20_qHD_List_Intr_Port4->bHighBandwidth=1;
    pHost20_qHD_List_Intr_Port4->bInterruptScheduleMask=1;
    pHost20_qHD_List_Intr_Port4->bEdSpeed= gSpeed_Port4;
    pHost20_qHD_List_Intr_Port4->bDataToggleControl=0;

    //SetPointer(&(pHost20_qHD_List_Intr->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Intr));
     pHost20_qHD_List_Intr_Port4->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Intr_Port4)>>5);

     //pHost20_qHD_List_Control1->bNextQHDPointer=(((U32)pHost20_qHD_List_Intr)>>5);
     //pHost20_qHD_List_Intr->bNextQHDPointer=(((U32)pHost20_qHD_List_Control0)>>5);

    //       pHost20_qHD_List_Intr->bNextQHDPointer=(((UINT32)pHost20_qHD_List_Intr)>>5);      //point to itself
    pHost20_qHD_List_Intr_Port4->bTerminate=1;          //terminated


    //    psHost20_FramList=&Host20_FramList;
    //<3>.Link qHD to the FameListCell by wInterval
    //    printf("clear frame list data \n");

    pData=(UINT8 *)pHost20_FramList_Port4;
    for (j=0;j<sizeof(Periodic_Frame_List_Structure);j++)
        *(pData+j)=0;

    // printf("OK\n");

    for (i=0;i< Host20_Preiodic_Frame_List_MAX ;i++)
        pHost20_FramList_Port4->sCell[i].bTerminal=1;
    //Find the Interval-X
    //  Host20_FramList.sCell[0].bLinkPointer=((UINT32)(pHost20_qHD_List_Intr))>>5;


    SetFrameList_Port4(0);
    SetFrameList_Port4(50);
    SetFrameList_Port4(100);
    SetFrameList_Port4(150);
    SetFrameList_Port4(200);


    // printf("FrameList:%x\n",FRAME_LIST_DATA);
    // printf("first QH:%lx\n",VirtoPhyAddr((UINT16)pHost20_qHD_List_Intr));

    //while (1){}
 #if 0
        x=0;
        while(waIntervalMap[x]<wForceInterval)
        {
          x++;
          if (x>10)
             {
                 printf("Interval Input Error...\n");
                 return;
             }
          };

        for (i=0;i<Host20_Preiodic_Frame_List_MAX;i=i+waIntervalMap[x])
             {
               Host20_FramList.sCell[i].bLinkPointer=((UINT32)(pHost20_qHD_List_Intr))>>5;
               Host20_FramList.sCell[i].bTerminal=0;
               Host20_FramList.sCell[i].bType=HOST20_HD_Type_QH;
             }
#endif

    //<4>.Set Periodic Base Address
    //  printf("FrameListPhy:%lx\n",VirtoPhyAddr((UINT16)&Host20_FramList));

    USB_HID_INT_DBG(printf("pHost20_FramList: %lX\n", (U32)pHost20_FramList_Port4));
    mwHost20_PeriodicBaseAddr_Set(VirtoPhyAddr((U32)pHost20_FramList_Port4));


    //<5>.Enable the periodic
    flib_Host20_Periodic_Setting_Port4(HOST20_Enable);

}
//====================================================================
// * Function Name: flib_Host20_Periodic_Setting
// * Description:
// * Input:
// * OutPut:
//====================================================================
void flib_Host20_Periodic_Setting_Port4(UINT8 bOption)
{
    if (bOption==HOST20_Enable)
    {
        //<1>.If Already enable => return
        if (mwHost20_USBSTS_PeriodicStatus_Rd()>0)
            return ;

        //<2>.Disable Periodic
        mbHost20_USBCMD_PeriodicEnable_Set();

        //<3>.Polling Status
        // while(mwHost20_USBSTS_PeriodicStatus_Rd()==0);

    }
    else if (bOption==HOST20_Disable)
    {
        //<1>.If Already Disable => return
        if (mwHost20_USBSTS_PeriodicStatus_Rd()==0)
            return ;

        //<2>.Enable Periodic
        mbHost20_USBCMD_PeriodicEnable_Clr();

        //<3>.Polling Status
        //      while(mwHost20_USBSTS_PeriodicStatus_Rd()>0);

    }
              //  else
               // {
                  //  printf("??? Input Error 'flib_Host20_Periodic_Setting'...");
               // }
}

UINT8 flib_Host20_Send_qTD2_Port4(qTD_Structure  *spHeadqTD ,qHD_Structure  *spTempqHD,U16 wTimeOutSec)
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

  //if (wTimeOutSec==0)
  //{
  //  DEBUG_USB(printf("error, timeout sec is zero\n"));
  //}
    spOldDumyqTD=(qTD_Structure*)PhytoVirAddr((( U32)(spTempqHD->bOverlay_NextqTD))<<5);

  //spTempqHD->bOverlay_Status|=HOST20_qTD_STATUS_Halted;
  while (mbHost20_USBCMD_PeriodicEnable_Rd())
  {
    mbHost20_USBCMD_PeriodicEnable_Clr();
  }

  //spHeadqTD->bTerminate=1;			//set to terminate
  memcpy(spOldDumyqTD,spHeadqTD,Host20_qTD_SIZE);
  //spOldDumyqTD->bStatus_Halted=1;

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

  spLastqTD->bNextQTDPointer=VirtoPhyAddr(( U32)spNewDumyqTD)>>5;

  spLastqTD->bTerminate=0;

  //Link Alternate qTD pointer
    spLastqTD->bAlternateQTDPointer=(VirtoPhyAddr(( U32)spNewDumyqTD)>>5);

  spLastqTD->bAlternateTerminate=0;



  //<4>.Set OldDumyqTD->Active=1
  //gwLastqTDSendOK=0;
  //sAttachDevice.psSendLastqTD=spLastqTD;
  sAttachDevice_Port4.bSendStatusError=0;

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

  while (mbHost20_USBCMD_PeriodicEnable_Rd()==0)
  {
    mbHost20_USBCMD_PeriodicEnable_Set();
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
  msAPI_Timer_ResetWDT();
  #endif
  //<5>.Waiting for result
  //EAL=0;
  while (1)
  {
    #ifdef ATV_SERISE_USE
    wTimes++;
    #endif
    if ((spOldDumyqTD->bStatus_Active==0) &&
	    ( mwHost20_USBSTS_Rd()& HOST20_USBINTR_CompletionOfTransaction))              //wait until Status_Active become 0
    {
      bReturnValue=HOST20_OK;

      break;
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
      if (MsOS_Timer_DiffTimeFromNow(LastTime) >(U32)300)     //over 1 mini sec
        {

            if (mwHost20_FrameIndex_Rd()==FrameIdx)
            {
                USB_HID_INT_MSG(printf("1C stop,timeout !!\n"));
                gUsbStatus_Port4=USB_TIMEOUT;         //ESD , USB hang,should be timeout
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
      gUsbStatus_Port4=USB_TIMEOUT;
      USB_HID_INT_MSG(printf("USB_TIMEOUT\n"));
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
  while (mbHost20_USBCMD_PeriodicEnable_Rd())
  {
    mbHost20_USBCMD_PeriodicEnable_Clr();
  }

  //<6>.Checking the Result
  if (bReturnValue!=HOST20_FATAL)
    bReturnValue=flib_Host20_CheckingForResult_QHD_Port4(spTempqHD);
  if (bReturnValue==HOST20_TRANSACTION_ERROR)
  {
    //printf("Transaction Error\n");

    spTempqHD->bOverlay_Status=0;      //clear halt status
    spTempqHD->bOverlay_CurrentqTD=VirtoPhyAddr(( U32)spNewDumyqTD)>>5;
  }

  //<5>.Release the all the qTD (Not include spNewDumyqTD)
  #if 1
  spReleaseqTD=spOldDumyqTD;
  do {
    //spReleaseqTDNext=((UINT32)(spReleaseqTD->bNextQTDPointerL))<<5 + ((UINT32)(spReleaseqTD->bNextQTDPointerH))<<16  ;
     spReleaseqTDNext=(qTD_Structure*)PhytoVirAddr(((U32)(spReleaseqTD->bNextQTDPointer))<<5);

        flib_Host20_ReleaseStructure_Port4(Host20_MEM_TYPE_qTD,(U32)spReleaseqTD);

    spReleaseqTD=spReleaseqTDNext;
  } while(((UINT32)spReleaseqTD)!=((UINT32)spNewDumyqTD));
  #endif

 return (bReturnValue);
}
#if 1
//====================================================================
// * Function Name: flib_Host20_Issue_Interrupt
// * Description:
// * Input:
// * OutPut:
//====================================================================
U8  flib_Host20_Issue_Interrupt_Port4(U32 buf,U32 hwSize)
{
    qTD_Structure *spTempqTD;
    U32      mybuf =0, workbuf;
    U8      bResult;

    if ( buf !=(U32) KSEG02KSEG1(buf) )
    {
        if (buf & 0x7)			//flush should be 8 bytes aligned
        {
            //	printf(" buf ");
            #if 0
            mybuf=(U32)msAPI_Memory_Allocate((hwSize+0xf)&~0xf, BUF_ID_USB_HOST );
            {
                    printf("Memory allocate failed\n");
                    return HOST20_FATAL;
            }
            MY_HAL_DCACHE_FLUSH((U32)mybuf,(U32)((hwSize+0xf)&~0xf));
            mybuf = (U32)KSEG02KSEG1(mybuf);
            #else
            mybuf = (U32)KSEG02KSEG1(usb_temp_buf_Port4);
            #endif
        }
        else
        {
                //printf(" fuh ");
            MY_HAL_DCACHE_FLUSH((U32)buf,(U32)hwSize);		//flush buffer to uncached buffer
            buf=(U32)KSEG02KSEG1(buf);
        }
    }
    else
    {
        if (buf & 0xf)
        {
                //   printf(" buf ");
                #if 0
                mybuf=(U32)msAPI_Memory_Allocate((hwSize+0xf)&~0xf, BUF_ID_USB_HOST );
                if ( mybuf == 0 )
                {
                    printf("Memory allocate failed\n");
                    return HOST20_FATAL;
                }
                MY_HAL_DCACHE_FLUSH((U32)mybuf,(U32)((hwSize+0xf)&~0xf));
                mybuf = (U32)KSEG02KSEG1(mybuf);
                #else
                mybuf = (U32)KSEG02KSEG1(usb_temp_buf_Port4);
                #endif
        }
    }

    if (mybuf==0)
        workbuf=buf;        //use original buf
    else
        workbuf=mybuf;

    //<1>.Fill TD
    spTempqTD =flib_Host20_GetStructure_Port4(Host20_MEM_TYPE_qTD); //The qTD will be release in the function "Send"
    spTempqTD->bTotalBytes=hwSize;
    spTempqTD->ArrayBufferPointer_Word[0] = VirtoPhyAddr(workbuf);

    //<2>.Analysis the Direction
    spTempqTD->bPID=HOST20_qTD_PID_IN;

    //<3>.Send TD
    bResult = flib_Host20_Send_qTD2_Port4(spTempqTD ,pHost20_qHD_List_Intr_Port4,3);

    if (mybuf)
    {
        memcpy((void*)buf,(void*)mybuf,hwSize);	//copy buffer
        #if 0
        mybuf = (U32)KSEG12KSEG0(mybuf);
        msAPI_Memory_Free((void*)mybuf,BUF_ID_USB_HOST);
        #endif
    }

    return bResult;
}
#endif
#endif //#if defined(MSOS_TYPE_NOS)

