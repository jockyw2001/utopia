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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#if defined(MSOS_TYPE_NOS)
#include "../USBHostConfig_4.h"
#include <stdlib.h>
#ifndef ATV_SERISE_USE
#include <stdio.h>
#endif
#include <string.h>
#ifndef ATV_SERISE_USE
#include "../../include/datatype.h"
#else
#include "UsbHostDataDef.h"
#endif
#include "../drvHostLib_4.h"
#include "../drvHost200_4.h"
#include "../drvscsi_4.h"
#include "../drvMassStorage_4.h"
#ifdef ATV_SERISE_USE
#include "uart.h"
#else
//#include "drvmiu.h"
#endif
#include "../drvUsbMain_4.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#define DRV_MASS_STORAGE_DBG(x)  //x;
#define DRV_MASS_STORAGE_MSG(x)  MS_CRITICAL_MSG(x)

#define ErrorRetryCount1 3

UINT32 gwTagPort4=0x88888888;
extern U8 gSpeed_Port4;

extern void Dump_Data_Port4(UINT16 addr,UINT16 sz);
#define Enable_UHCI_Recovery
U8 RecoveryFlag_Port4=0;


//===============================================================================================
//===============================================================================================
//===============================================================================================
//============================= Group-1: Mass Storage Class Implement ===========================
//===============================================================================================
//===============================================================================================
//===============================================================================================
//***************************************************************************************
// Function Name: Host20_MSCD_ClearFeature_Port4
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
code  UINT8 HOST20_CLEAR_FEATURE_PORT4[8]     = {0x02,0x01,0x00,0x00,0x81,0x00,0x00,0x00}; //

UINT8 Host20_MSCD_ClearFeature_Port4(struct us_data *psMassStorage)
{
  UINT8 bTemp;
  UINT8 bCMD[8];


  memcpy(bCMD,HOST20_CLEAR_FEATURE_PORT4,8);
  //bCMD[4]=bEdNum;
  if (psMassStorage->srb->sc_data_direction==SCSI_DATA_WRITE)
    bCMD[4]=psAttachDevice_Port4->saCD[0].sInterface[0].sED[ pOTGH_PT_BLK_Port4->bOutQHDArrayNum].bED_EndpointAddress;
  else
    bCMD[4]=psAttachDevice_Port4->saCD[0].sInterface[0].sED[ pOTGH_PT_BLK_Port4->bInQHDArrayNum].bED_EndpointAddress;

  //if (pOTGH_PT_BLK_Port4->bInQHDArrayNum==0) bCMD[4]=0x81;
  //else bCMD[4]=0x82;
  //printf("clear stall:%02bx\n",bCMD[4]);

  if (flib_Host20_Issue_Control_Port4 (1,bCMD,0x08,&bTemp)>0)
  {

    //printf(">>> Host20_MSCD_ClearFeature_Port4 Fail ...\r\n");
    return (0);
  }
  else
  {
    //printf("Clear stall succeed\n");
    return (1);
  }
}

#if 1
code   UINT8 HOST20_MSCD_GET_MAX_LUN_PORT4[8]     = {0xA1,0xFE,0x00,0x00,0x00,0x00,0x01,0x00}; //Get MAX LUN

//***************************************************************************************
// Function Name: Host20_MSCD_GetMaxLUN_Port4
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
UINT8 Host20_MSCD_GetMaxLUN_Port4(void)
{
  UINT8 bMaxLun;
  UINT8 bCMD[8];
  //UINT8 HOST20_MSCD_GET_MAX_LUN[8]     = {0xA1,0xFE,0x00,0x00,0x00,0x00,0x01,0x00}; //Get MAX LUN

  //Setup=0xA1,0xFE,0x00,0x00,0x00,0x00,0x01,0x00
  //In =1 bytes
  //Ack
  //printf("data buffer:%x  %lx\n",psAttachDevice_Port4->bDataBuffer,USB_BUFFER_START_ADR_4K_ALIGN);
  memcpy(bCMD,HOST20_MSCD_GET_MAX_LUN_PORT4,8);

  if (flib_Host20_Issue_Control_Port4 (1,bCMD,0x01,&bMaxLun)>0)
  {

    DRV_MASS_STORAGE_DBG(printf(">>>Get Max Lun Fail ...\n"));
    return (0);
  }
  else
  {
    //printf("Get Max Lun succeed\n");
    return (bMaxLun);

  }



}

#endif

extern void Dump_Data_Port4(UINT16 addr,UINT16 sz);

UINT32 SwapWord_Port4(UINT32 dat1)
{
  U8  t1,t2,t3,t4;
  U32 temp;

  t1=(U8)(dat1 >> 24) & 0xff;
  t2=(U8)(dat1 >> 16) & 0xff;
  t3=(U8)(dat1 >> 8) & 0xff;
  t4=(U8)(dat1) & 0xff;

  temp=( (U32) t4 <<24) | ((U32) t3 << 16) |  ((U32) t2 << 8) | t1  ;
  return temp;
}
//***************************************************************************************
// Function Name:Host20_MSCD_issue_CBW_Port4
// Description:
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
UINT8 Host20_MSCD_issue_CBW_Port4(struct us_data *psMassStorage)
{
  //Host20_BufferPointerArray_Structure aTemp;
  U8 result;
  U32 buf;

  //   printf("issue CBW...\n");
  //<1>.Prepare the CBW data
  pOTGH_PT_BLK_Port4->sCBW.u32Signature = OTGH_PT_CBW_SIGNATE;
  pOTGH_PT_BLK_Port4->sCBW.u32DataTransferLength=psMassStorage->srb->request_bufflen;
  pOTGH_PT_BLK_Port4->sCBW.u32Tag=gwTagPort4;
  pOTGH_PT_BLK_Port4->sCBW.u8LUN=psMassStorage->srb->lun;

  switch (psMassStorage->srb->sc_data_direction)
  {
    case SCSI_DATA_WRITE:
         //printf("data wr\n");
         pOTGH_PT_BLK_Port4->sCBW.u8Flags=0x00;
         break;
    case SCSI_DATA_READ:
         //printf("data rd\n");

         pOTGH_PT_BLK_Port4->sCBW.u8Flags=0x80;  //john 0x01
         break;
    //case SCSI_DATA_UNKNOWN:
    //case SCSI_DATA_NONE:
    //DRV_MASS_STORAGE_DBG(printf(">>> Unknow 'psMassStorage->srb->sc_data_direction'... \n"));
    //while(1);
    //break;
  }

  pOTGH_PT_BLK_Port4->sCBW.u8LUN=psMassStorage->srb->lun;//Only support 1 LUN
  pOTGH_PT_BLK_Port4->sCBW.u8CBLength=psMassStorage->srb->cmd_len;//Command Length
  //memcpy((U8 xdata *)&(pOTGH_PT_BLK_Port4->sCBW.u8CB[0]),(U8 xdata *)&(psMassStorage->srb->cmnd[0]),16);

  //printf("signature:%lx\n",(pOTGH_PT_BLK_Port4->sCBW.u32Signature));
  //printf("scbw:%x\n",(U16)&(pOTGH_PT_BLK_Port4->sCBW.u32Signature));
  //Dump_Data((U16)&(pOTGH_PT_BLK_Port4->sCBW.u32Signature),31);
  //buf=(U8 xdata *)psAttachDevice_Port4->bDataBuffer;
  //memcpy(buf,(U8 xdata *)&(pOTGH_PT_BLK_Port4->sCBW.u32Signature),31);
  buf=(U32)&(pOTGH_PT_BLK_Port4->sCBW.u32Signature);
  result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bOutQHDArrayNum,31,buf,OTGH_Dir_Out);
  return(result);
}

//***************************************************************************************
// Function Name:Host20_MSCD_issue_CSW_Port4
// Description:
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
UINT8 Host20_MSCD_issue_CSW_Port4(void)
{
  UINT32 wCBWTag,wCSWTag;
  //Host20_BufferPointerArray_Structure aTemp;
  U32 buf;
  U8 result,retry=0;

  //printf("Get CSW");
  // buf=(U8 xdata *)psAttachDevice_Port4->bDataBuffer;
    buf=(U32)&(pOTGH_PT_BLK_Port4->sCSW.u32Signature);

  redo2:
       result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bInQHDArrayNum,13,buf,OTGH_Dir_IN);
       if (result==HOST20_TRANSACTION_ERROR)
       {
	     retry++;
		 if(mwHost20_PORTSC_ConnectStatus_Rd()==0 || (retry>ErrorRetryCount1))
		   return HOST20_FATAL;

	     #ifdef Enable_Low_Temperature_Patch
	     if(retry<=ErrorRetryCount1)
         {
	       if(gSpeed_Port4==0)
	       {
	         //printf("\r\n CSW toggle!!",0);
                UTMI4_SETXBYTE(0x0a, UTMI4_READXBYTE(0x0a)^0x10); //invert CDR_CLOCK
                UTMI4_ORXBYTE(0x06,0x03);    //reset UTMI
                UTMI4_ANDXBYTE(0x06,0xfc);
	       }
         }
	     #endif
         //printf("csw trans err retry\n");
         goto redo2;
       }

   // printf("result:%02bx\n",result);
   if (result==HOST20_FATAL) return HOST20_MSCD_DEVICE_FATAL_ERROR;
   else if (result!=0)
   {  //john    if (bTemp==HOST20_FAIL) {
     return(HOST20_MSCD_CSW_Status_STALL);
   }
   //memcpy((U8 xdata *)&(pOTGH_PT_BLK_Port4->sCSW.u32Signature),(U8 xdata *)psAttachDevice_Port4->bDataBuffer,13);

   //<2>.Check the data-Tag
   wCBWTag=(UINT32)(pOTGH_PT_BLK_Port4->sCBW.u32Tag);
   wCSWTag=(UINT32)(pOTGH_PT_BLK_Port4->sCSW.u32Tag);
   if (wCBWTag!=wCSWTag)
   {
     DRV_MASS_STORAGE_DBG(printf(">>> Error: Received CSW->Tag fail (Expected data:0x%x / Received Data:0x%x)\n",(UINT32)(wCBWTag),(UINT32)(wCSWTag)));
     return(HOST20_MSCD_CSW_Status_NOT_VALID);
   }

   //<3>.Check the data-Signature
   if ((pOTGH_PT_BLK_Port4->sCSW.u32Signature)!=OTGH_PT_CSW_SIGNATE)
   {
     DRV_MASS_STORAGE_DBG(printf(">>> Error: Received CSW->Signature fail (Expected data:%d / Received Data:%d)\n",OTGH_PT_CSW_SIGNATE,(UINT32)(pOTGH_PT_BLK_Port4->sCSW.u32Tag)));
     return(HOST20_MSCD_CSW_Status_NOT_VALID);
   }

   //<4>.Checking Status OTGH_PT_CSW_PASS
   if ((pOTGH_PT_BLK_Port4->sCSW.u8Status)!=HOST20_MSCD_CSW_Status_PASS)
   {//printf(">>> Error: Received CSW->Status fail 'HOST20_MSCD_CSW_Status_FAIL'\n");
     return(pOTGH_PT_BLK_Port4->sCSW.u8Status);
   }
   return(HOST20_MSCD_CSW_Status_PASS);
}
extern U16 TotalBytes_Port4;

//***************************************************************************************
// Function Name: Host20_MSCD_issue_Data_Port4
// Description:
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
UINT8 Host20_MSCD_issue_Data_Port4(struct us_data *psMassStorage)
{

  // UINT8 *pbDataPage[5];
  UINT32 wTotalLengthRemain,Offset;
  U32 buf;
  U16 TransferLengh;
  U8 result=HOST20_FAIL,retry;

  //printf("data phase\n");


  //<1>.To fill the data buffer

  wTotalLengthRemain=psMassStorage->srb->request_bufflen;

    buf=psMassStorage->srb->request_buffer;

  //<2>.Issue Transfer

    switch (psMassStorage->srb->sc_data_direction)
    {
      case SCSI_DATA_WRITE://Out
	  Offset=0;
	  while(wTotalLengthRemain>0)
	  {
	    if(wTotalLengthRemain > Scsi_Max_Transfer_Len)
	    {
	      TransferLengh = Scsi_Max_Transfer_Len;
	    }
	    else
	    {
		  TransferLengh=wTotalLengthRemain;
	    }

		result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bOutQHDArrayNum,TransferLengh
	                         ,buf+Offset,OTGH_Dir_Out);
		retry=0;
        while (result==HOST20_TRANSACTION_ERROR)
        {
		  retry++;
	      RecoveryFlag_Port4|=0x01;
	      if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
	        pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=1;
		  else
		    pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=1;

		  DRV_MASS_STORAGE_DBG(printf("\r\n retry OUT\n",0));

		  if(mwHost20_PORTSC_ConnectStatus_Rd()==0 || (retry>ErrorRetryCount1))
	      {
	        if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
		      pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=0;
	        else
		      pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=0;
	        RecoveryFlag_Port4&=0xFE;
	        return HOST20_FATAL;
	      }

		  result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bOutQHDArrayNum, TotalBytes_Port4,
                     buf+Offset+(TransferLengh-TotalBytes_Port4),OTGH_Dir_Out);

	      if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
	        pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=0;
		  else
		    pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=0;
	      RecoveryFlag_Port4&=0xFE;

        }
		Offset+=TransferLengh;
        wTotalLengthRemain-=TransferLengh;
	  }
      return result;

      break;

      case SCSI_DATA_READ://In
      //<2>.Issue the Loop2-InData & Compare
      //	if ((psMassStorage->srb->phy_buf_adr & 0xfff)==0)
      //      buf=(UINT8*)(psMassStorage->srb->phy_buf_adr);  //use original buffer

	  Offset=0;
      while(wTotalLengthRemain>0)
	  {
	    if(wTotalLengthRemain > Scsi_Max_Transfer_Len)
	    {
	      TransferLengh = Scsi_Max_Transfer_Len;
	    }
	    else
	    {

		  TransferLengh=wTotalLengthRemain;
	    }

		result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bInQHDArrayNum, TransferLengh
	                         ,buf+Offset,OTGH_Dir_IN);

        if (result==HOST20_TRANSACTION_ERROR)
        {
		  retry=0;
          XREDO:
		       retry++;
               //printf("data in transerror retry\n");
			   if(mwHost20_PORTSC_ConnectStatus_Rd()==0 || (retry>ErrorRetryCount1))
			     return HOST20_FATAL;
			   #ifdef Enable_Low_Temperature_Patch
	           if(retry<=ErrorRetryCount1)
               {
	             if(gSpeed_Port4==0)
	             {
	               //printf("\r\n IN toggle!!",0);
                        UTMI4_SETXBYTE(0x0a,UTMI4_READXBYTE(0x0a)^0x10);  //invert CDR_CLOCK
                        UTMI4_ORXBYTE(0x06,0x03);    //reset UTMI
                        UTMI4_ANDXBYTE(0x06,0xfc);
	             }
               }
	           #endif
               result=flib_Host20_Issue_Bulk_Port4 (pOTGH_PT_BLK_Port4->bInQHDArrayNum, TotalBytes_Port4,
                   buf+Offset+(TransferLengh-TotalBytes_Port4),OTGH_Dir_IN);
               if (result==HOST20_TRANSACTION_ERROR)
                 goto XREDO;
        }
        else if (result==HOST20_FATAL)   return result;               //timeout
        Offset+=TransferLengh;
        wTotalLengthRemain-=TransferLengh;
      }
	  //for (i=0 ; i < 16 ; i++)
	  //	printf("%02bx ", buf[i]);
	  //printf("\n");
      //printf("\ndump data\n");
      //Dump_Data((U16)psMassStorage->srb->request_buffer,psMassStorage->srb->request_bufflen);
      //printf("\ndump buf\n");
      //Dump_Data((U16)buf,36);

      return result;

      break;



    }

    return(result);

}

#ifdef  ENABLE_CBI_HOST
code UINT8 CBI_SET_COMMAND_PORT4[]    = {0x21,0x00,0x00,0x00,0x00,0x00,0x0c,0x00};

BOOLEAN Host20_MSDCD_usb_stor_CBI_protocol_Port4(struct us_data *psMassStorage)
{
    UINT8 bCMD[8];
    UINT8 bValue,result;
    //printf("CBI protocol\n");
    memcpy(bCMD,CBI_SET_COMMAND_PORT4,8);

    result=flib_Host20_Issue_Control_CBI_Port4(bCMD,12,(UINT8 *)&(pOTGH_PT_BLK_Port4->sCBW.u8CB[0]));
    if (result== HOST20_DEVICE_STALL)
    {
        //  printf("stall result\n");
        psMassStorage->srb->result   = SAM_STAT_CHECK_CONDITION;
        return TRUE;           //that is a kind of success, CB doesn't have status phase,STALL mean CSW=1
    }

    if (psMassStorage->srb->request_bufflen>0)
    {
        bValue=Host20_MSCD_issue_Data_Port4(psMassStorage);
        if (bValue==HOST20_DEVICE_STALL)
        {
            DRV_MASS_STORAGE_MSG(printf("data phase stall\n"));
            Host20_MSCD_ClearFeature_Port4(psMassStorage);
        }
        else if (bValue==HOST20_FATAL)
        {
            DRV_MASS_STORAGE_MSG(printf("data phase fatal\n"));
            return FALSE;
        }
    }

    return TRUE;
}
#endif

//***************************************************************************************
// Function Name: Host20_MSCD_usb_stor_control_thread_Port4
// Description:
//             <1>.Issue CBW
//             <2>.Issue Data Stage
//             <3>.Issue CSW
//             <4>.Fill the result & return
// Input:
// Output:
// Status:S
//***************************************************************************************
BOOLEAN Host20_MSCD_usb_stor_control_thread_Port4(struct us_data *psMassStorage)
{
  UINT8 bValue,retry;
  UINT8 result;
  //<1>.CBW issue
#ifdef ENABLE_CBI_HOST
  if ((psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x00)||(psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x01))
  {
      return Host20_MSDCD_usb_stor_CBI_protocol_Port4(psMassStorage);
  }
#endif
  result=Host20_MSCD_issue_CBW_Port4(psMassStorage);
  if (result==HOST20_FATAL) return FALSE;
  else if (result==HOST20_TRANSACTION_ERROR)
  {
    DRV_MASS_STORAGE_DBG( printf("\r\n ??? 'Host20_MSCD_issue_CBW_Port4' Fail...(%bx)",pOTGH_PT_BLK_Port4->sCBW.u8Flags));
    #ifdef Enable_UHCI_Recovery
    DRV_MASS_STORAGE_DBG(printf("\r\n retry cbw",0));
    RecoveryFlag_Port4|=0x01;
    if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
	  pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=1;
	else
	  pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=1;
	retry=0;
    while(1)
    {
	  retry++;

      if(mwHost20_PORTSC_ConnectStatus_Rd()==0 || (retry>ErrorRetryCount1))
	  {
	    if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
		  pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=0;
	    else
		  pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=0;
	    RecoveryFlag_Port4&=0xFE;
	    return FALSE;
	  }

	  result=Host20_MSCD_issue_CBW_Port4(psMassStorage);

      if (result==HOST20_OK) break;
      else if (result==HOST20_FATAL) return FALSE;
    }
    if(pOTGH_PT_BLK_Port4->bOutQHDArrayNum)
	  pHost20_qHD_List_Bulk1_Port4->bDataToggleControl=0;
	else
	  pHost20_qHD_List_Bulk0_Port4->bDataToggleControl=0;
	RecoveryFlag_Port4&=0xFE;
    #else
    return FALSE;
    #endif
  }

  //<2>.Data issue
  retry=0;
  if (psMassStorage->srb->request_bufflen>0)
  {
    bValue=Host20_MSCD_issue_Data_Port4(psMassStorage);
    if (bValue==HOST20_DEVICE_STALL)
      Host20_MSCD_ClearFeature_Port4(psMassStorage);
    else if (bValue==HOST20_FATAL)   return FALSE;

  }
  //<3>.CSW issue
  //printf("Get CSW");
  //printf("bULK 0:\n");
  // Dump_Data(pHost20_qHD_List_Bulk0_Port4,0x30);

  bValue=Host20_MSCD_issue_CSW_Port4();
  if (bValue==HOST20_MSCD_DEVICE_FATAL_ERROR) return FALSE;
  if(bValue>0)
  {
    //printf("??? 'Host20_MSCD_issue_CSW_Port4' Fail...\n");
    //For Stall=>try again
    psMassStorage->srb->result = SAM_STAT_CHECK_CONDITION;        //problem happened
    if (bValue==HOST20_MSCD_CSW_Status_STALL)
    {
      //printf(">>> Device Stall the Command, CSW issue again.\n");
      //<3.1>.Clear Feature
      MsOS_DelayTask(1);
      Host20_MSCD_ClearFeature_Port4(psMassStorage);
      //<3.2>.Read CSW again
      MsOS_DelayTask(1);
      bValue=Host20_MSCD_issue_CSW_Port4();
      if(bValue>0)
        psMassStorage->srb->result = SAM_STAT_CHECK_CONDITION;        //problem happened
      else
        psMassStorage->srb->result = SAM_STAT_GOOD;        //problem happened


    }


  }
  else
    psMassStorage->srb->result = SAM_STAT_GOOD;

  gwTagPort4++;

  return TRUE;
}
#endif //#if defined(MSOS_TYPE_NOS)

