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
#include <stdlib.h>
//#include <stdarg.h>
#ifndef ATV_SERISE_USE
#include <stdio.h>
#endif
#include <string.h>

//#include "chipset.h"
//#include "flib.h"
#ifndef ATV_SERISE_USE
#include "../../include/datatype.h"
#else
#include "UsbHostDataDef.h"
#endif
#include "../drvHostLib_4.h"
#include "../drvHost200_4.h"
//#include "Host20_AP_HID.h"
//#include "Host20_AP_Camera.h"
 #include "../drvUsbMain_4.h"
#include "../drvscsi_4.h"
#ifdef ATV_SERISE_USE
#include "uart.h"
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#define DRV_HOST_DBG(x)  //x;


//OTGH_PT_BLK_Struct  xdata    OTGH_PT_BLK _at_ OTGH_PT_BLK_DATA;
OTGH_PT_BLK_Struct  OTGH_PT_BLK_Port4 __attribute__ ((aligned (16)));
OTGH_PT_BLK_Struct  *pOTGH_PT_BLK_Port4 = (OTGH_PT_BLK_Struct*) &OTGH_PT_BLK_Port4;

void OTGH_PT_Bulk_Init_Port4(void);


extern void Init_Mass_Storage_Structure_Port4(void);

//====================================================================
// * Function Name: OTGH_AP_Main
// * Description: Main menu of path test
// * Input: none
// * OutPut: none
//====================================================================
BOOLEAN MassStorage_Init_Port4(void)
{

  BOOLEAN result;

  if (psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceClass!=0x08)
  {
    DRV_HOST_DBG(printf("not mass storage class!\n"));
    return FALSE;
  }
  DRV_HOST_DBG(printf("Mass storage device..\n"));
  Init_Mass_Storage_Structure_Port4();

  OTGH_PT_Bulk_Init_Port4();

  result=bSCSI_Initial_Port4();
  if(!result)
  {
    MsOS_DelayTask(500); //081205 at SEC, disk needs more time for TUR OK
    result=bSCSI_Initial_Port4();
  }

  return result;
}




#if 0
//====================================================================
// * Function Name: OTGH_PT_Bulk_Close
// * Description:
// * Input: none
// * OutPut: none
//====================================================================
void OTGH_PT_Bulk_Close(void)
{

  //<5>.Hang the qHD
  if (psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_bmAttributes==OTGH_ED_BULK)
  { //<5.1>.stop Asynchronous Schedule
    flib_Host20_Asynchronous_Setting(HOST20_Disable);
    if (pHost20_qHD_List_Bulk0_Port4->bMaxPacketSize1> 2)           //larger than 512
    {//Support only 2 QHD
      mwHost20_CurrentAsynchronousAddr_Set(Host20_STRUCTURE_qHD_BASE_ADDRESS);
    }
    else
    {
      //<5.2>.Hang the qHD for ED0~ED3
      SetPointer_Port4(&(pHost20_qHD_List_Control1_Port4->bNextQHDPointer3),VirtoPhyAddr((UINT16)pHost20_qHD_List_Control0_Port4));

    }

    //<5.2>.Enable Asynchronous Schedule
    flib_Host20_Asynchronous_Setting(HOST20_Enable);
  }

}
#endif
extern void Dump_Data_Port4(UINT16 addr,UINT16 sz);
extern BOOLEAN FirstBulkOutPort4,FirstBulkInPort4;

//====================================================================
// * Function Name: OTGH_PT_Bulk_Init_Port4
// * Description:
// * Input: none
// * OutPut: none
//====================================================================
void OTGH_PT_Bulk_Init_Port4(void)
{

  UINT16 wMaxPacketSize;

  // UINT8 *wTemp;

  wMaxPacketSize=psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_wMaxPacketSizeHighByte;
  wMaxPacketSize=(wMaxPacketSize<<8)+(psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_wMaxPacketSizeLowByte);



  //<5>.Hook the qHD
  if (psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_bmAttributes==OTGH_ED_BULK)
  { //<5.1>.stop Asynchronous Schedule
    //flib_Host20_Asynchronous_Setting(HOST20_Disable);
    //mbHost20_USBCMD_AsynchronousEnable_Clr();

    mbHost20_USBCMD_AsynchronousEnable_Clr();
    while(mwHost20_USBSTS_AsynchronousStatus_Rd()>0);


      //<5.2>.Hook the qHD for ED0~ED3
     pHost20_qHD_List_Control1_Port4->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Bulk0_Port4)>>5);
     pHost20_qHD_List_Bulk0_Port4->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Bulk1_Port4)>>5);
     pHost20_qHD_List_Bulk1_Port4->bNextQHDPointer=(VirtoPhyAddr((U32)pHost20_qHD_List_Control0_Port4)>>5);

    //<5.2>.Enable Asynchronous Schedule
    //flib_Host20_Asynchronous_Setting(HOST20_Enable);

  }


  FirstBulkInPort4=TRUE;
  FirstBulkOutPort4=TRUE;

  //Setting Max Packet Size and print message
  wMaxPacketSize=psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_wMaxPacketSizeHighByte;
  wMaxPacketSize=(wMaxPacketSize<<8)+(psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_wMaxPacketSizeLowByte);
  pHost20_qHD_List_Bulk0_Port4->bMaxPacketSize=wMaxPacketSize;
  pHost20_qHD_List_Bulk1_Port4->bMaxPacketSize=wMaxPacketSize; //Suppose In/Out have the same Max packet Size
  pHost20_qHD_List_Bulk0_Port4->bDeviceAddress=psAttachDevice_Port4->bAdd;
  pHost20_qHD_List_Bulk1_Port4->bDeviceAddress=psAttachDevice_Port4->bAdd;
  pHost20_qHD_List_Bulk0_Port4->bEdNumber=psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_EndpointAddress & 0x0f; //set endpoint address
  pHost20_qHD_List_Bulk1_Port4->bEdNumber=psAttachDevice_Port4->saCD[0].sInterface[0].sED[1].bED_EndpointAddress & 0x0f; //set endpoint address

  //printf("add:%02bx\n",psDevice_AP->bAdd);
  //printf("Endpoint 1 addr: %02bx\n", pHost20_qHD_List_Bulk0_Port4->bEdNumber);
  //printf("Endpoint 2 addr: %02bx\n", pHost20_qHD_List_Bulk1_Port4->bEdNumber);


  //printf(">>> Bulk In/Out Max Packet Size = %x\n",wMaxPacketSize);

  //For Bulk Structure
  //sOTGH_PT_BLK_PORT4=( OTGH_PT_BLK_Struct *)flib_Host20_GetStructure_Port4(Host20_MEM_TYPE_4K_BUFFER);//2=>4kBuffer;
  //sOTGH_PT_BLK_PORT4=(OTGH_PT_BLK_Struct xdata *)OTGH_PT_BLK_DATA;            //bulk QH data structure
  //printf("PT_BLK:%x\n",(U16)pOTGH_PT_BLK_Port4);
  //printf("OTGH_PT_BLK_DATA:%x\n",OTGH_PT_BLK_DATA);
  //pOTGH_PT_BLK_Port4->wDataRange=2*1024*1; ////2k


  if (((psAttachDevice_Port4->saCD[0].sInterface[0].sED[0].bED_EndpointAddress)&BIT7)>0)
  { pOTGH_PT_BLK_Port4->bInQHDArrayNum=0;//Array 0           //for in 0, for out,1
    pOTGH_PT_BLK_Port4->bOutQHDArrayNum=1;//Array 0
  }
  else
  {
    pOTGH_PT_BLK_Port4->bInQHDArrayNum=1;//Array 0            //for in 1 for out,0
    pOTGH_PT_BLK_Port4->bOutQHDArrayNum=0;//Array 0
  }

  pOTGH_PT_BLK_Port4->bStatus = HOST20_OK;
}
#endif  //#if defined(MSOS_TYPE_NOS)

