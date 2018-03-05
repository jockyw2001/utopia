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
// (¨MStar Confidential Information〃) by the recipient.
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
#include "Board.h"
#include "common.h"
#include "uart.h"
#endif
#include "../drvUsbMain_4.h"
#include "../../include/_drvUSB.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

extern void msAPI_Timer_ResetWDT(void);

#define DRV_SCSI_DBG(x) // x;

//struct us_data xdata *Mass_stor_us;
#define MAX_LUN_SUPPORT    5

struct us_data Mass_stor_us1_port4;
#if 1
struct LUN_Device  Mass_stor_device_port4[MAX_LUN_SUPPORT];
#else
struct LUN_Device Mass_stor_device_1_port4; //support 4 LUN
struct LUN_Device Mass_stor_device_2_port4;
struct LUN_Device Mass_stor_device_3_port4;
struct LUN_Device Mass_stor_device_4_port4;
struct LUN_Device Mass_stor_device_5_port4;
#endif

extern UINT8 pUsbData_Port4[1024];
Scsi_Cmnd  srb_data_port4;

UINT8 u8Drive_port4 = 0;
extern U8 gUsbStatus_Port4;
extern U8 gSpeed_Port4;

struct stForceModeSense1A
{
    U16     VID;
    U16     PID;
} stForceModeSense1A;

struct stForceModeSense1A gForceModeSense1A_Port4[] =
{
        {0x0DD8, 0x1448},
        {0x1AA6, 0x0201},
        {0x0DD8, 0x1414},
        {0x0DD8, 0x1400},
        {0x0DD8, 0xE203},
        {0x8644, 0x800a},           //alan.yu add on 2010.08.13 (紫光 创见)
        {0x0DD8, 0xD202},           //alan.yu add on 2010.08.16 (朗科Netac)
        {0x18A5,0x302},             //alan.yu add on 2011.08.01
        {0, 0}
};

extern void MDrv_USBGetVIDPID_Port4(U16 *pVID, U16 *pPID);

//BOOLEAN gDeviceFatalError=FALSE;
void Init_Mass_Storage_Structure_Port4(void)
{
  U16   uVID, uPID;

  //Mass_stor_us=&Mass_stor_us1;
  Mass_stor_us1_port4.Mass_stor_device=&Mass_stor_device_port4[0];
#ifdef   ENABLE_CBI_HOST
  if ( (psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x00)||
       (psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x01) )
    Mass_stor_us1_port4.max_lun=0;
  else
#endif
  {
    MDrv_USBGetVIDPID_Port4(&uVID, &uPID);
    //printf("MAXLUN VID: %x, PID: %x\n", uVID, uPID);
    if ( ((uVID == 0x8564) && (uPID == 0x1000)) ||
         ((uVID == 0xFCA) && (uPID == 0x8004)) ) //20130301, for blackberry phone
          Mass_stor_us1_port4.max_lun = 0;
    else
          Mass_stor_us1_port4.max_lun=   Host20_MSCD_GetMaxLUN_Port4();
  }
  //printf("Max Lun:%02bx\n",Mass_stor_us1.max_lun);
    if (Mass_stor_us1_port4.max_lun > (MAX_LUN_SUPPORT-1) )
        Mass_stor_us1_port4.max_lun = MAX_LUN_SUPPORT-1;
}
//***************************************************************************************
// Function Name: vSCSICmd_READ_10
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
void vSCSICmd_READ_WRITE_10_Port4(BOOLEAN DoWrite,struct us_data *us, Scsi_Cmnd *srb, INT8U u8LunNum,
							  INT32U u32BlockAddr, INT16U u16BlockNum, U32 u8Buffer)
{


  /* set the command and the LUN */
  #if 0
  memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  srb->cmnd[0] = READ_10;

  srb->cmnd[2] = (INT8U)(u32BlockAddr >> 24);
  srb->cmnd[3] = (INT8U)(u32BlockAddr >> 16);
  srb->cmnd[4] = (INT8U)(u32BlockAddr >> 8);
  srb->cmnd[5] = (INT8U)(u32BlockAddr);

  srb->cmnd[7] = (INT8U)(u16BlockNum >> 8);
  srb->cmnd[8] = (INT8U)(u16BlockNum );
  #endif
  if (DoWrite)
    pOTGH_PT_BLK_Port4->sCBW.u8CB[0]=WRITE_10;
  else
    pOTGH_PT_BLK_Port4->sCBW.u8CB[0]=READ_10;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[1]=0;

  pOTGH_PT_BLK_Port4->sCBW.u8CB[2]= (INT8U)(u32BlockAddr >> 24);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[3]= (INT8U)(u32BlockAddr >> 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= (INT8U)(u32BlockAddr >> 8);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[5]= (INT8U)(u32BlockAddr);

  pOTGH_PT_BLK_Port4->sCBW.u8CB[7] = (INT8U)(u16BlockNum >> 8);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[8] = (INT8U)(u16BlockNum );


  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = 10;

  /* set the transfer direction */
  if (DoWrite)
  	srb->sc_data_direction = SCSI_DATA_WRITE;
  else
	srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
  srb->request_buffer= u8Buffer;

  /* set the buffer length for transfer */
  srb->request_bufflen = (us->Mass_stor_device[u8LunNum].u32BlockSize)*u16BlockNum;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  srb->lun= u8LunNum;

  //srb->scsi_done = vScsi_SendCmd_Done;



}
#if 0
//***************************************************************************************
// Function Name:
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
void vSCSICmd_WRITE_10(struct us_data *us, Scsi_Cmnd *srb,  INT8U u8LunNum,
							  INT32U u32BlockAddr, INT16U u16BlockNum, U32 u8Buffer)
{


  /* set the command and the LUN */
  #if 0
  memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  srb->cmnd[0] = WRITE_10;

  srb->cmnd[2] = (INT8U)(u32BlockAddr >> 24);
  srb->cmnd[3] = (INT8U)(u32BlockAddr >> 16);
  srb->cmnd[4] = (INT8U)(u32BlockAddr >> 8);
  srb->cmnd[5] = (INT8U)(u32BlockAddr);

  srb->cmnd[7] = (INT8U)(u16BlockNum >> 8);
  srb->cmnd[8] = (INT8U)(u16BlockNum );
  #endif
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]=WRITE_10;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[1]=0;

  pOTGH_PT_BLK_Port4->sCBW.u8CB[2]= (INT8U)(u32BlockAddr >> 24);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[3]= (INT8U)(u32BlockAddr >> 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= (INT8U)(u32BlockAddr >> 8);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[5]= (INT8U)(u32BlockAddr);

  pOTGH_PT_BLK_Port4->sCBW.u8CB[7] = (INT8U)(u16BlockNum >> 8);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[8] = (INT8U)(u16BlockNum );

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_WRITE_10;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_WRITE;

  /* use the new buffer we have */
  srb->phy_buf_adr=u8Buffer;
  srb->buffer_type=BUFFER_PHY;

  /* set the buffer length for transfer */
  srb->request_bufflen = us->Mass_stor_device[u8LunNum].u32BlockSize*u16BlockNum;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  srb->lun= u8LunNum;

  //srb->scsi_done = vScsi_SendCmd_Done;


}
#endif
//***************************************************************************************
// Function Name:
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
void vSCSICmd_INQUIRY_Port4( Scsi_Cmnd *srb, INT8U *u8Buffer)
{


  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = INQUIRY;
  //srb->cmnd[4] = DATA_LENGTH_INQUIRY;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= INQUIRY;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= DATA_LENGTH_INQUIRY;

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_INQUIRY;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
    srb->request_buffer = (U32)u8Buffer;

  /* set the buffer length for transfer */
  srb->request_bufflen = DATA_LENGTH_INQUIRY;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;//0;

  //srb->scsi_done = vScsi_SendCmd_Done;


}
//***************************************************************************************
// Function Name:
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
void vSCSICmd_READ_CAPACITY_Port4( Scsi_Cmnd *srb, INT8U *u8Buffer)
{

  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = READ_CAPACITY;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= READ_CAPACITY;

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_READ_CAPACITY;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
    srb->request_buffer = (U32)u8Buffer;

  /* set the buffer length for transfer */
  srb->request_bufflen = DATA_LENGTH_READ_CAPACITY;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;

  //srb->scsi_done = vScsi_SendCmd_Done;


}

void vSCSICmd_READ_FORMAT_CAPACITY_Port4( Scsi_Cmnd *srb, INT8U *u8Buffer)
{

  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = READ_CAPACITY;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= READ_FORMAT_CAPACITY;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[8]= 12; //Length

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_READ_CAPACITY;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
  srb->request_buffer = (U32)u8Buffer;

  /* set the buffer length for transfer */
  srb->request_bufflen = 12;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;

  //srb->scsi_done = vScsi_SendCmd_Done;


}

//***************************************************************************************
// Function Name:vSCSICmd_MODE_SENSE_Port4
// Description:
// Input:
// Output:
// Status:
//***************************************************************************************
void vSCSICmd_MODE_SENSE_Port4( Scsi_Cmnd *srb,INT8U u8PageCode,INT8U *u8Buffer)
{

  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = MODE_SENSE;
  //srb->cmnd[2] = u8PageCode;
  //srb->cmnd[4] = DATA_LENGTH_MODE_SENSE;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= MODE_SENSE;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[2]= u8PageCode;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= DATA_LENGTH_MODE_SENSE;

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
    srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_MODE_SENSE;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
    srb->request_buffer = (U32)u8Buffer;
  /* set the buffer length for transfer */
  srb->request_bufflen = DATA_LENGTH_MODE_SENSE;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;

  //srb->scsi_done = vScsi_SendCmd_Done;


}

//***************************************************************************************
// Function Name:vSCSICmd_MODE_SENSE_OperCode5A_Port4
// Description:
// Input:
// Output:
// Status:
//***************************************************************************************
void vSCSICmd_MODE_SENSE_OperCode5A_Port4( Scsi_Cmnd *srb,INT8U u8PageCode,INT8U *u8Buffer)
{

  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = MODE_SENSE;
  //srb->cmnd[2] = u8PageCode;
  //srb->cmnd[4] = DATA_LENGTH_MODE_SENSE;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= MODE_SENSE_5A;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[2]= u8PageCode;

#if 0
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= DATA_LENGTH_MODE_SENSE_OperCode5A;		//RCC?
#else
  pOTGH_PT_BLK_Port4->sCBW.u8CB[7] = (INT8U)(DATA_LENGTH_MODE_SENSE_OperCode5A >> 8);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[8] = (INT8U)(DATA_LENGTH_MODE_SENSE_OperCode5A );
#endif

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
    srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_MODE_SENSE_5A;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
    srb->request_buffer = (U32)u8Buffer;
  /* set the buffer length for transfer */
  srb->request_bufflen = DATA_LENGTH_MODE_SENSE_OperCode5A;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;

  //srb->scsi_done = vScsi_SendCmd_Done;


}

//***************************************************************************************
// Function Name:vSCSICmd_REQUEST_SENSE_Port4
// Description:
// Input:
// Output:
// Status:
//***************************************************************************************
void vSCSICmd_REQUEST_SENSE_Port4(  Scsi_Cmnd *srb, INT8U *u8Buffer)
{


  /* set the command and the LUN */
  //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
  //srb->cmnd[0] = REQUEST_SENSE;
  //srb->cmnd[4] = 18;
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= REQUEST_SENSE;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= 18;

  /* FIXME: we must do the protocol translation here */
  /*
  if (us->subclass == US_SC_RBC || us->subclass == US_SC_SCSI)
    srb->cmd_len = 6;
  else
	srb->cmd_len = 12;
  */
  srb->cmd_len = CB_LENGTH_REQUEST_SENSE;

  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* use the new buffer we have */
    srb->request_buffer = (U32)u8Buffer;
  /* set the buffer length for transfer */
  srb->request_bufflen = 18;

  /* set up for no scatter-gather use */
  //srb->use_sg = 0;

  /* change the serial number -- toggle the high bit*/
  //srb->serial_number ^= 0x80000000;

  /* set Lun number*/
  //srb->lun= u8Drive;

  //srb->scsi_done = vScsi_SendCmd_Done;


}
#if 0
//***************************************************************************************
// Function Name:vSCSICmd_TEST_UNIT_READY
// Description:
// Input:
// Output:
// Status:S
//***************************************************************************************
void vSCSICmd_TEST_UNIT_READY( Scsi_Cmnd *srb)
{

  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= TEST_UNIT_READY;

  srb->cmd_len = CB_LENGTH_TEST_UNIT_READY;


  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;

  /* set the buffer length for transfer */
  srb->request_bufflen = 0;

}
#endif

void vSCSICmd_Prepare_Port4(Scsi_Cmnd *srb, U8 cmd)
{
  memset(&pOTGH_PT_BLK_Port4->sCBW.u8CB[0], 0, 16);
  pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= cmd;

  srb->cmd_len = 6;
  /* set the transfer direction */
  srb->sc_data_direction = SCSI_DATA_READ;
  /* set the buffer length for transfer */
  srb->request_bufflen = 0;
}




BOOLEAN  vSCSI_EJECT_DEVICE_Port4(U8 lun)
{
  BOOLEAN result;

  Mass_stor_us1_port4.srb =&srb_data_port4;
  memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

  vSCSICmd_Prepare_Port4(Mass_stor_us1_port4.srb,START_STOP);
  Mass_stor_us1_port4.srb->lun=lun;
  pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= 2;                         //stop the device

  //result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1);
  //if (result==FALSE) return result;
  //pOTGH_PT_BLK_Port4->sCBW.u8CB[0]= START_STOP;
  //pOTGH_PT_BLK_Port4->sCBW.u8CB[4]= 2;                         //stop the device
  //printf("lun:%02bx\n",lun);
  result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
  return result;
}





//=======================================================================================
//=======================================================================================
//=======================================================================================
//=======================================================================================
//***************************************************************************************
// Function Name:vSCSI_REQUEST_SENSE_Port4
// Description: Reserved.
//              If Device do not support the command-A
//              ,Host should issue command "vSCSI_REQUEST_SENSE" to ask "why not support".
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
BOOLEAN vSCSI_REQUEST_SENSE_Port4(U8 lun)
{
  INT8U *u8RequestSenseData;
  //Scsi_Cmnd  srb_data;
  BOOLEAN result;

  u8RequestSenseData=pUsbData_Port4;
  Mass_stor_us1_port4.srb = &srb_data_port4;
  memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

  // Build SCSI command.
  vSCSICmd_REQUEST_SENSE_Port4( Mass_stor_us1_port4.srb,u8RequestSenseData);

  Mass_stor_us1_port4.srb->lun=lun;

  //call mass storage function to send CBW
  //and get REQUEST_SENSE Data. Return CSW to check status.
  result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
  return result;



}

//***************************************************************************************
// Function Name:bSCSI_INQUIRY_Port4
// Description:To ask the device u8DeviceType/u8ProductID/u8ProductVer... information
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
///////////////////////////////////////////////////////////////////////////////
//		bSCSI_INQUIRY_Port4()
//		Description:
//			1. scsi inquiry command.
//		input: none
//		output: TRUE or FALSE (BOOLEAN)
///////////////////////////////////////////////////////////////////////////////
//INT8U xdata u8InquiryData[DATA_LENGTH_INQUIRY] _at_ Host20_TEMP_DATA;
BOOLEAN bSCSI_INQUIRY_Port4(U8 lun)
{
  INT8U u8i, u8j;
  //Scsi_Cmnd  srb_data;
  U8 *u8InquiryData;
  BOOLEAN result;

  u8InquiryData=pUsbData_Port4;
  //printf("INQUIRY\n");
  for(u8j =0; u8j< ScsiCmd_Fail_Retry ;u8j++)
  {

    Mass_stor_us1_port4.srb =&srb_data_port4;
	memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

	//Build SCSI command.
	vSCSICmd_INQUIRY_Port4( Mass_stor_us1_port4.srb, u8InquiryData);
    Mass_stor_us1_port4.srb->lun=lun;

	//call mass storage function to send CBW
	//and get INQUIRY Data. Return CSW to check status.
	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
	if (result==FALSE) continue;

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
      //save all INQUIRY data
	  Mass_stor_us1_port4.device.u8DeviceType = (u8InquiryData[0] & 0x1F);

        Mass_stor_us1_port4.Mass_stor_device[lun].bRMB = (u8InquiryData[1] & 0x80) >> 7;
        //printf("Removable: %d\n", Mass_stor_us1_port4.Mass_stor_device[lun].bRMB);

	  for(u8i =0; u8i <8; u8i++)
	    Mass_stor_us1_port4.Mass_stor_device[lun].u8VendorID[u8i] = u8InquiryData[u8i+8];
	  for(u8i =0; u8i <16; u8i++)
		Mass_stor_us1_port4.Mass_stor_device[lun].u8ProductID[u8i] = u8InquiryData[u8i+16];
	  for(u8i =0; u8i <4; u8i++)
	    Mass_stor_us1_port4.Mass_stor_device[lun].u8ProductVer[u8i] = u8InquiryData[u8i+32];

	  if(Mass_stor_us1_port4.device.u8DeviceType != TYPE_DISK)
	  {
	    DRV_SCSI_DBG(printf("Device type unsuport, it's not a scsi disk%s","\n"));

		return FALSE;
	  }
	  else
	  {

	    DRV_SCSI_DBG(SCSI_DbgPrint("SCSI INQUIRY : SCSI Device ID <%s>\n",&Mass_stor_us1_port4.Mass_stor_device[lun].u8VendorID));
		//SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ID <%s>\n",&Mass_stor_us1.device.u8ProductID);
		//SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ver <%s>\n",&Mass_stor_us1.device.u8ProductVer);
        return TRUE;
	  }


	}

  }
	return FALSE;
}
//***************************************************************************************
// Function Name:bSCSI_MODE_SENSE_Port4
// Description:To know the mode of device <1>.Write Protect => Y/N
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
///////////////////////////////////////////////////////////////////////////////
//		bSCSI_MODE_SENSE()
//		Description:
//			1. scsi mode sense command.
//		input: none
//		output: TRUE or FALSE (BOOLEAN)
///////////////////////////////////////////////////////////////////////////////
BOOLEAN bSCSI_MODE_SENSE_Port4(U8 lun)
{
  INT8U  u8j;
  INT8U *u8ModeSenseData;
  BOOLEAN result;
  //Scsi_Cmnd  srb_data;

  u8ModeSenseData=pUsbData_Port4;

  Mass_stor_us1_port4.srb = &srb_data_port4;

  for(u8j =0; u8j< ScsiCmd_Fail_Retry ;u8j++)
  {
    memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));
    //Build SCSI command.
	vSCSICmd_MODE_SENSE_Port4( Mass_stor_us1_port4.srb,0x3F, u8ModeSenseData);
    Mass_stor_us1_port4.srb->lun=lun;

	//call mass storage function to send CBW
	//and get Mode Sense Data. Return CSW to check status.
	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
	if (result==FALSE) return FALSE;

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
      //save all Mode Sense(page code=0x3F) data
	  //Now we only use u8ModeSenseData[2], this byte save device
	  //write protection information
	  Mass_stor_us1_port4.Mass_stor_device[lun].bWriteProtect = (u8ModeSenseData[2]==0x80)?
												TRUE:FALSE;


	  return TRUE;
	}
  }
  Mass_stor_us1_port4.Mass_stor_device[lun].bWriteProtect=FALSE;      //not support this command
  return FALSE;
}


//***************************************************************************************
// Function Name:bSCSI_MODE_SENSE_OperCode5A_Port4
// Description:To know the mode of device <1>.Write Protect => Y/N
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
///////////////////////////////////////////////////////////////////////////////
//		bSCSI_MODE_SENSE_OperCode5A_Port4()
//		Description:
//			1. scsi mode sense command.
//		input: none
//		output: TRUE or FALSE (BOOLEAN)
///////////////////////////////////////////////////////////////////////////////
BOOLEAN bSCSI_MODE_SENSE_OperCode5A_Port4(U8 lun)
{
  INT8U  u8j;
  INT8U *u8ModeSenseData;
  BOOLEAN result;
  //Scsi_Cmnd  srb_data;

  u8ModeSenseData=pUsbData_Port4;

  Mass_stor_us1_port4.srb = &srb_data_port4;

  for(u8j =0; u8j< ScsiCmd_Fail_Retry ;u8j++)
  {
    memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));
    //Build SCSI command.
		vSCSICmd_MODE_SENSE_OperCode5A_Port4( Mass_stor_us1_port4.srb,0x3F, u8ModeSenseData);
    Mass_stor_us1_port4.srb->lun=lun;

	//call mass storage function to send CBW
	//and get Mode Sense Data. Return CSW to check status.
	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
	if (result==FALSE) return FALSE;

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
      //save all Mode Sense(page code=0x3F) data
	  //Now we only use u8ModeSenseData[2], this byte save device
	  //write protection information
	  Mass_stor_us1_port4.Mass_stor_device[lun].bWriteProtect = (u8ModeSenseData[3]==0x80)?
												TRUE:FALSE;
	  return TRUE;
	}
  }
  Mass_stor_us1_port4.Mass_stor_device[lun].bWriteProtect=FALSE;      //not support this command
  return FALSE;
}


//***************************************************************************************
// Function Name:bSCSI_READ_CAPACITY_Port4
// Description:To get the CAPACITY
// Input:
// Output:
// Status:S
//***************************************************************************************
///////////////////////////////////////////////////////////////////////////////
//		bSCSI_READ_CAPACITY_Port4()
//		Description:
//			1. scsi READ_CAPACITY command.
//		input: none
//		output: TRUE or FALSE (BOOLEAN)
///////////////////////////////////////////////////////////////////////////////
BOOLEAN bSCSI_READ_CAPACITY_Port4(U8 lun)
{
  //INT8U u8j;
  INT8U *u8ReadCapacityData;
  BOOLEAN result;
  //Scsi_Cmnd  srb_data;

  u8ReadCapacityData=pUsbData_Port4;
  //for(u8j =0; u8j< ScsiCmd_Fail_Retry ;u8j++)
  {
    Mass_stor_us1_port4.srb = &srb_data_port4;
	memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

	//Build SCSI command.
	vSCSICmd_READ_CAPACITY_Port4(Mass_stor_us1_port4.srb, u8ReadCapacityData);
    Mass_stor_us1_port4.srb->lun=lun;

    //call mass storage function to send CBW
	//and get CAPACITY Data. Return CSW to check status.
	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
	if (result==FALSE) return result;

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
	  //save all CAPACITY data
	  Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum = ((INT32U)u8ReadCapacityData[0] << 24) |
										   ((INT32U)u8ReadCapacityData[1] << 16) |
										   ((INT32U)u8ReadCapacityData[2] << 8) |
										   ((INT32U)u8ReadCapacityData[3] ) ;

	  Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockSize = ((INT32U)u8ReadCapacityData[4] << 24) |
									   ((INT32U)u8ReadCapacityData[5] << 16) |
									   ((INT32U)u8ReadCapacityData[6] << 8) |
									   ((INT32U)u8ReadCapacityData[7] ) ;
     //if ( Mass_stor_us1.Mass_stor_device[lun].u32BlockSize > 1024)
     //{
     //  printf("Block size over 1024, is %X\n",Mass_stor_us1.Mass_stor_device[lun].u32BlockSize);
     //  while(1);
     //}

	 DRV_SCSI_DBG(SCSI_DbgPrint("SCSI CAPACITY : SCSI Device total block <0x%x%x>\n",
	                           (INT16U)(Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum >> 16),
					           (INT16U)Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum));
	 DRV_SCSI_DBG(SCSI_DbgPrint("SCSI CAPACITY : SCSI Product block size <0x%x bytes>\n",(INT16U)Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockSize));

	 return TRUE;
   }
   else
   {
        vSCSI_REQUEST_SENSE_Port4(lun);
        return FALSE;
   }
 }
 return FALSE;

}

BOOLEAN bSCSI_READ_FORMAT_CAPACITY_Port4(U8 lun)
{
  INT8U u8j;
  INT8U *u8ReadCapacityData;
  BOOLEAN result;
  //Scsi_Cmnd  srb_data;

  u8ReadCapacityData=pUsbData_Port4;
  for(u8j =0; u8j< ScsiCmd_Fail_Retry ;u8j++)
  {
      Mass_stor_us1_port4.srb = &srb_data_port4;
	memset(Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

	//Build SCSI command.
	vSCSICmd_READ_FORMAT_CAPACITY_Port4(Mass_stor_us1_port4.srb, u8ReadCapacityData);
       Mass_stor_us1_port4.srb->lun=lun;

    //call mass storage function to send CBW
	//and get CAPACITY Data. Return CSW to check status.
	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
	if (result==FALSE) return result;

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
	  //save all CAPACITY data
	  Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum = ((INT32U)u8ReadCapacityData[4] << 24) |
										   ((INT32U)u8ReadCapacityData[5] << 16) |
										   ((INT32U)u8ReadCapacityData[6] << 8) |
										   ((INT32U)u8ReadCapacityData[7] ) ;

	  Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockSize = ((INT32U)u8ReadCapacityData[9] << 16) |
									   ((INT32U)u8ReadCapacityData[10] << 8) |
									   ((INT32U)u8ReadCapacityData[11] ) ;
     //if ( Mass_stor_us1.Mass_stor_device[lun].u32BlockSize > 1024)
     //{
     //  printf("Block size over 1024, is %X\n",Mass_stor_us1.Mass_stor_device[lun].u32BlockSize);
     //  while(1);
     //}

	 DRV_SCSI_DBG(SCSI_DbgPrint("SCSI CAPACITY : SCSI Device total block <0x%x%x>\n",
	                           (INT16U)(Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum >> 16),
					           (INT16U)Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockTotalNum));
	 DRV_SCSI_DBG(SCSI_DbgPrint("SCSI CAPACITY : SCSI Product block size <0x%x bytes>\n",(INT16U)Mass_stor_us1_port4.Mass_stor_device[lun].u32BlockSize));

	 return TRUE;
   }
 }
 return FALSE;

}


extern void OTGH_PT_Bulk_Init_Port4(void);
extern BOOLEAN MDrv_UsbHost_Init_Port4(void);
//***************************************************************************************
// Function Name:bSCSI_TEST_UNIT_READY_Port4
// Description: To know that "device is ready ?"
// Input:
// Output:
// Status:
//***************************************************************************************
///////////////////////////////////////////////////////////////////////////////
//		bSCSI_TEST_UNIT_READY_Port4()
//		Description:
//			1. scsi TEST_UNIT_READY command.
//		input: none
//		output: TRUE or FALSE (BOOLEAN)
///////////////////////////////////////////////////////////////////////////////
BOOLEAN bSCSI_TEST_UNIT_READY_Port4(U8 lun)
{
  INT8U u8j;
  BOOLEAN result;
  #ifdef ATV_SERISE_USE
  U32 i;
  #endif
  //Scsi_Cmnd  srb_data;
  //printf("TUR\n");

  for(u8j =0; u8j< TestUnitReady_Fail_Retry ;u8j++)
  {
    //printf("connect status:%02bx\n",mwHost20_PORTSC_ConnectStatus_Rd());
	if (mwHost20_PORTSC_ConnectStatus_Rd()==0)
      return FALSE;               //device is not connected

	Mass_stor_us1_port4.srb = &srb_data_port4;
	memset((U8 xdata *)Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

	// Build SCSI command.
	vSCSICmd_Prepare_Port4(Mass_stor_us1_port4.srb,TEST_UNIT_READY);
	//vSCSICmd_TEST_UNIT_READY(Mass_stor_us1.srb);
    Mass_stor_us1_port4.srb->lun=lun;
    //printf("Mass_stor_us1.srb:%02bx\n",Mass_stor_us1.srb->request_bufflen);
	//call mass storage function to send CBW
	//and get CAPACITY Data. Return CSW to check status.

	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
    //printf("result:%02bx\n",result);
#if 1
        if (result == FALSE)
        {
            if (gUsbStatus_Port4== USB_TIMEOUT)
            {
                DRV_SCSI_DBG(printf("resend TUR\n"));
                if ( MDrv_UsbHost_Init_Port4() && (flib_Host20_Enumerate_Port4(1,3)!=0) )
                {
                    OTGH_PT_Bulk_Init_Port4();
                    continue;
                }
            }
            else
                return result;
        }
#else
        if (result==FALSE)
        {
            return result;
        }
#endif

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
	  Mass_stor_us1_port4.Mass_stor_device[lun].bDeviceReady = TRUE;
      //SCSI_DbgPrint("SCSI TEST UNIT READY : Succeed\n",u8Drive);

	  return TRUE;
	}
	else
	{
	  #ifndef ATV_SERISE_USE
         MsOS_DelayTask(10);
         #else
         for(i=0;i<(0x100);i++);
         #endif


	  result=vSCSI_REQUEST_SENSE_Port4(lun);
      if ((pUsbData_Port4[12]==0x3a)&&(pUsbData_Port4[13]==0)) return FALSE;  //no media
	  if (result==FALSE) return result;
	}
    #ifndef ATV_SERISE_USE
	MsOS_DelayTask(10);
	#else
	for(i=0;i<0x100;i++);
	#endif
  }
	//SCSI_DbgPrint("SCSI TEST UNIT READY : Failed%s","\n");
  return FALSE;
}
extern BOOLEAN  MDrv_Usb_Device_Enum_Port4(void);
#ifdef ATV_SERISE_USE
extern void EnableINT(void);
#endif

BOOLEAN bSCSI_TEST_UNIT_READY2_Port4(U8 lun)
{
  INT8U u8j;
  BOOLEAN result;
  U8  RetryCnt=0;

  #ifdef ATV_SERISE_USE
  U32 i;
  #endif
  //Scsi_Cmnd  srb_data;
  //printf("TUR\n");

  for(u8j =0; u8j< TestUnitReady_Fail_Retry ;u8j++)
  {
    //printf("connect status:%02bx\n",mwHost20_PORTSC_ConnectStatus_Rd());
	if (mwHost20_PORTSC_ConnectStatus_Rd()==0)
      return FALSE;               //device is not connected

	Mass_stor_us1_port4.srb = &srb_data_port4;
	memset((U8 xdata *)Mass_stor_us1_port4.srb, 0, sizeof(struct scsi_cmnd ));

	// Build SCSI command.
	vSCSICmd_Prepare_Port4(Mass_stor_us1_port4.srb,TEST_UNIT_READY);
	//vSCSICmd_TEST_UNIT_READY(Mass_stor_us1.srb);
    Mass_stor_us1_port4.srb->lun=lun;
    //printf("Mass_stor_us1.srb:%02bx\n",Mass_stor_us1.srb->request_bufflen);
	//call mass storage function to send CBW
	//and get CAPACITY Data. Return CSW to check status.

	result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
    //printf("result:%02bx\n",result);

    if (result==FALSE)
    {
      RetryCnt++;
      if (gUsbStatus_Port4==USB_TIMEOUT)
      {
        if (RetryCnt > 3) return FALSE;
        if ((RetryCnt==2)&&(mwOTG20_Control_HOST_SPD_TYP_Rd()==2) )           //make sure it is hi speed
        {
          DRV_SCSI_DBG( printf("Force FS\n"));
          UHC4_ORXBYTE(0x40,0x80);//force enter FSmode
        }
        #ifndef ATV_SERISE_USE
        msAPI_Timer_ResetWDT();
        #else
          #ifdef Enable_Close_INT
          //XBYTE[0x2B00]&=0xFD;
          EnableINT();
          #endif
        #endif
        #ifdef Enable_Low_Temperature_Patch
	    if(gSpeed_Port4==0)
	    {
	      //printf("\r\n CDR toggle!!",0);
              UTMI4_SETXBYTE(0x0a,UTMI4_READXBYTE(0x0a)^0x10);    //invert CDR_CLOCK
              UTMI4_ORXBYTE(0x06,0x03);  //reset UTMI
              UTMI4_ANDXBYTE(0x06,0xfc);
	    }
		#endif
       // MDrv_Usb_Device_Enum();
        if (MDrv_Usb_Device_Enum_Port4()==FALSE) return FALSE;

        continue;
      }
      else
        return FALSE;
    }

	if(Mass_stor_us1_port4.srb->result == SAM_STAT_GOOD)
	{
	  Mass_stor_us1_port4.Mass_stor_device[lun].bDeviceReady = TRUE;
      //SCSI_DbgPrint("SCSI TEST UNIT READY : Succeed\n",u8Drive);

	  return TRUE;
	}
	else
	{
	  #ifndef ATV_SERISE_USE
         MsOS_DelayTask(10);
         #else
         for(i=0;i<(0x100);i++);
         #endif


	  result=vSCSI_REQUEST_SENSE_Port4(lun);
      if ((pUsbData_Port4[12]==0x3a)&&(pUsbData_Port4[13]==0)) return FALSE;  //no media
      if ((pUsbData_Port4[12]==0x28)&&(pUsbData_Port4[13]==0)) return FALSE;  //media change
	  if (result==FALSE) return result;
	}
    #ifndef ATV_SERISE_USE
	MsOS_DelayTask(10);
	#else
	for(i=0;i<0x100;i++);
	#endif
  }
	//SCSI_DbgPrint("SCSI TEST UNIT READY : Failed%s","\n");
  return FALSE;
}
//***************************************************************************************
// Function Name:bSCSI_Initial_Port4
// Description:
// Input:
// Output:
// Status:P-OK
//***************************************************************************************
U8 ValidLunNum_Port4;
BOOLEAN bSCSI_Initial_Port4(void)
{
  //INT8U i=0;
  //iCardSuddenlyRemove = FALSE;
  //iCardReplace = FALSE;
  BOOLEAN		bFoundValidDev = FALSE;
  struct LUN_Device* LunDevice = Mass_stor_us1_port4.Mass_stor_device;
  U8		LunIdx;

 ValidLunNum_Port4=0;
 for (	LunIdx=0; LunIdx<=Mass_stor_us1_port4.max_lun; LunIdx++)
  {
    LunDevice[LunIdx].bDeviceValid=FALSE;
	if (bSCSI_INQUIRY_Port4(LunIdx))
       {
            LunDevice[LunIdx].bDeviceValid=TRUE;
            ValidLunNum_Port4++;
       }
	//break;
  }
 DRV_SCSI_DBG(printf("ValidLunNum_Port4:%02bx\n",ValidLunNum_Port4));
    // For some usb harddrive without enough power, may meet following case.
    if (!ValidLunNum_Port4)
    {
    	  if (mwHost20_PORTSC_ConnectChange_Rd()==0)
            MDrv_SetUsbDeviceStatus_Port4(BAD_DEVICE);
        return bFoundValidDev;
    }

    // Sometime device is failed to initial at first time, but success at last.
    // It set the device status as BAD_DEVICE at first time when it failed.
    // We need to set correct value to device status when successed.
    if (gSpeed_Port4==2)
    {
        MDrv_SetUsbDeviceStatus_Port4(USB20_DEVICE);
    }
    else
    {
        MDrv_SetUsbDeviceStatus_Port4(USB11_DEVICE);
    }
  #ifdef ATV_SERISE_USE
    #ifdef Enable_Close_INT
  EA=0;
  #endif
  #endif
  for (	LunIdx=0; LunIdx<=Mass_stor_us1_port4.max_lun; LunIdx++)
  {
    LunDevice[LunIdx].bDeviceReady = FALSE;
	if (!bInit_USB_LUN_Port4(LunIdx))
	  continue;
	LunDevice[LunIdx].bDeviceReady = TRUE;
    bFoundValidDev = TRUE;

  }
  #ifdef ATV_SERISE_USE
    #ifdef Enable_Close_INT
  EA=1;
  #endif
  #endif
  return bFoundValidDev;

}
void GetValidLun_Port4(void)
{
  U8 LunIdx;
  struct LUN_Device* LunDevice = Mass_stor_us1_port4.Mass_stor_device;

  if (LunDevice == NULL)
  {
      DRV_SCSI_DBG(printf(" LUN structure not initialized!!\n"));
      return;
  }

  if ( (Mass_stor_us1_port4.max_lun == 0) && (!LunDevice[0].bRMB) && LunDevice[0].bDeviceReady)
  {
        //20120817, Don't do TUR on this special USB HDD.
        //It returns NOT READY after 30 minutes idle.
        return;
  }

  for (LunIdx=0; LunIdx<=Mass_stor_us1_port4.max_lun; LunIdx++)
  {
    if (LunDevice[LunIdx].bDeviceValid)
    {
      if(!bSCSI_TEST_UNIT_READY2_Port4(LunIdx))
      {
        LunDevice[LunIdx].bDeviceReady=FALSE;
        LunDevice[LunIdx].u32BlockSize = 0; // 20111110: resolve lun capacity zero if not the first lun
        LunDevice[LunIdx].u32BlockTotalNum = 0;
      }
      else
      {
        LunDevice[LunIdx].bDeviceReady = TRUE;
        if (LunDevice[LunIdx].u32BlockSize == 0) // 20111110: resolve lun capacity zero if not the first lun
        {
            if(!bSCSI_READ_CAPACITY_Port4(LunIdx))
            {
              if (!bSCSI_READ_FORMAT_CAPACITY_Port4(LunIdx))
              {
                  DRV_SCSI_DBG(SCSI_DbgPrint("Read CAPACITY failed.%s","\n"));
              }
            }
        }
      }
    }
    else
      LunDevice[LunIdx].bDeviceReady=FALSE;                   //not a valid device, so always not ready
  }
}
BOOLEAN bInit_USB_LUN_Port4(U8 Lun)
{
  U16   uVID, uPID;
  U8     ii;
  BOOLEAN bIsForceMS1A = FALSE;

  MDrv_USBGetVIDPID_Port4(&uVID, &uPID);
  if ( (uVID == 0xFCA) && (uPID == 0x8004) ) //20130301, for blackberry phone
  {
      if (!bSCSI_INQUIRY_Port4(Lun))
      {
        DRV_SCSI_DBG(SCSI_DbgPrint("Scsi inquiry failed (Lun=%d).\n",u8Drive_port4));
    	return FALSE;
      }
  }

  if(!bSCSI_TEST_UNIT_READY_Port4(Lun))
  {
    DRV_SCSI_DBG(SCSI_DbgPrint("Scsi Device not ready (Lun=%d).\n",Lun));
	return FALSE;
  }
  //bSCSI_INQUIRY();
  if (!bSCSI_INQUIRY_Port4(Lun))
  {
    DRV_SCSI_DBG(SCSI_DbgPrint("Scsi inquiry failed (Lun=%d).\n",u8Drive_port4));
	return FALSE;
  }
  if(!bSCSI_READ_CAPACITY_Port4(Lun))
  {
    if (!bSCSI_READ_FORMAT_CAPACITY_Port4(Lun))
    {
        DRV_SCSI_DBG(SCSI_DbgPrint("Read CAPACITY failed.%s","\n"));
        return FALSE;
    }
  }

#ifdef ENABLE_CBI_HOST
    if ((psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x00)||(psAttachDevice_Port4->saCD[0].sInterface[0].bInterfaceProtocol==0x01))
    {
        //do nothing
    }
    else
#endif
    {
    MDrv_USBGetVIDPID_Port4(&uVID, &uPID);
    DRV_SCSI_DBG(printf("VID: %x, PID: %x\n", uVID, uPID));
    ii = 0;
    while ( (gForceModeSense1A_Port4[ii].VID != 0) && (gForceModeSense1A_Port4[ii].PID != 0) )
    {
        if ( (uVID == gForceModeSense1A_Port4[ii].VID) && (uPID == gForceModeSense1A_Port4[ii].PID) )
        {
            DRV_SCSI_DBG(printf("Find a Force Mode Sense 1A device\n"));
            bIsForceMS1A = TRUE;
        }

        ii++;
        if (ii >= (sizeof(gForceModeSense1A_Port4) / sizeof(struct stForceModeSense1A)))
          break;
    }

    if (bIsForceMS1A)
        bSCSI_MODE_SENSE_Port4(Lun);
    else
        bSCSI_MODE_SENSE_OperCode5A_Port4(Lun);

    }
  //if (!bSCSI_MODE_SENSE(Lun))
  //{
  //  SCSI_DbgPrint("Mode sense failed!\n");
  //  return FALSE;
  //}

  if(!bSCSI_TEST_UNIT_READY_Port4(Lun))
  {
    DRV_SCSI_DBG(SCSI_DbgPrint("Scsi Device not ready (Lun=%d).\n",u8Drive_port4));
	return FALSE;
  }
  return TRUE;

}


//***************************************************************************************
// Function Name:
// Description:
// Input:
// Output:
// Status:
//***************************************************************************************



BOOLEAN bSCSI_Read_Write10_Port4(BOOLEAN DoWrite, U8 lun,  INT32U u32BlockAddr, INT32U u32BlockNum,U32 u8Buffer)
{

  //Scsi_Cmnd  srb_data;
  BOOLEAN result;
  Mass_stor_us1_port4.srb = &srb_data_port4;
  //memset(Mass_stor_us1.srb, 0, sizeof(struct scsi_cmnd ));
  //printf("read sector:%lx, sz:%lx\n",u32BlockAddr,u32BlockNum);

  if (DoWrite)
  {
    if(Mass_stor_us1_port4.Mass_stor_device[lun].bWriteProtect)
    {
	  DRV_SCSI_DBG(printf("This device is write protect now\n"));
	  return FALSE;
	}
  }

  //Build SCSI command.
  vSCSICmd_READ_WRITE_10_Port4(DoWrite,&Mass_stor_us1_port4, Mass_stor_us1_port4.srb, lun,
								u32BlockAddr, (INT16U)u32BlockNum, u8Buffer);


  //call mass storage function to send scsi command
  result=Host20_MSCD_usb_stor_control_thread_Port4(&Mass_stor_us1_port4);
  if (result==FALSE) return result;

  if(Mass_stor_us1_port4.srb->result != SAM_STAT_GOOD)
  {
    DRV_SCSI_DBG(SCSI_DbgPrint("Scsi ReadWrite_10 command failed.%s","\n"));

	return FALSE;
  }


  //printf("#");
  return TRUE;



}
#endif //#if defined(MSOS_TYPE_NOS)


