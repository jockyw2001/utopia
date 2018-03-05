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


#include "MsCommon.h"
#include "drvSCSI.h"
#include "drvProtocol.h"
#include "include/drvKernel.h"
#include "drvTransport.h"
#include "drvHCD.h"
#include "drvEHCI.h"

#ifdef SCSI_DEBUG
#define SCSI_DbgPrint(x,...) printk(x,__VA_ARGS__)
#else
#define SCSI_DbgPrint(x,...)
#endif

#define ScsiCmd_Fail_Retry		3
#define ScsiCmd_Fail_TUR_Retry		7
#define Scsi_Max_Transfer_Len	120*1024 //65536

struct us_data *Mass_stor_us[NUM_OF_ROOT_HUB*MAX_USTOR] = {NULL};
//extern S32		g_SCSISem;
//extern S32 _s32MutexUSB;

extern void USBCriticalSectionIn(MS_U8 Port);
extern void USBCriticalSectionOut(MS_U8 Port);
extern struct usb_hcd *msc_get_hcd(U8 host_id);

extern MS_BOOL MDrv_UsbDeviceConnect_EX(struct usb_hcd *hcd);

static void vScsi_SendCmd_Done(struct scsi_cmnd *srb);
BOOL bSCSI_Initial(U8 uPort);
BOOL bInit_USB_Disk(U8 uPort);
BOOL bSCSI_ERASE(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U32 u32SectorSize);
BOOL bSCSI_Read_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer);
BOOL bSCSI_Write_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer);
BOOL bSCSI_Read_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer);
BOOL bSCSI_Write_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer);
VOID RemoveUSBDiskPort(U8 uPort, U8 uLunNum);
BOOL ConnectUSBDisk(U8 uPort, U8 uLunNum);

static U32
get_value_log2(U32 val)
{
    U32 i, log2;

    if(val == 0)
        return 0;

    i    = val;
    log2 = 0;


    while (0 == (i & 1))
    {
        i >>= 1;
        log2++;
    }

    if (i != 1) return 0;
    else        return log2;
}

void vSCSICmd_READ_10(struct us_data *us, U8 u8LunNum,
							  U32 u32BlockAddr, U16 u16BlockNum, U8 *u8Buffer)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	//memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = READ_10;
	srb->cmnd[2] = (U8)(u32BlockAddr >> 24);
	srb->cmnd[3] = (U8)(u32BlockAddr >> 16);
	srb->cmnd[4] = (U8)(u32BlockAddr >> 8);
	srb->cmnd[5] = (U8)(u32BlockAddr);
	srb->cmnd[7] = (U8)(u16BlockNum >> 8);
	srb->cmnd[8] = (U8)(u16BlockNum );

	srb->cmd_len = CB_LENGTH_READ_10;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
	srb->request_buffer = u8Buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = u16BlockNum << (us->Mass_stor_device[u8LunNum].u32BlockSize_log2);

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}

void vSCSICmd_WRITE_10(struct us_data *us, U8 u8LunNum,
							  U32 u32BlockAddr, U16 u16BlockNum, U8 *u8Buffer)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	//memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = WRITE_10;
	srb->cmnd[2] = (U8)(u32BlockAddr >> 24);
	srb->cmnd[3] = (U8)(u32BlockAddr >> 16);
	srb->cmnd[4] = (U8)(u32BlockAddr >> 8);
	srb->cmnd[5] = (U8)(u32BlockAddr);
	srb->cmnd[7] = (U8)(u16BlockNum >> 8);
	srb->cmnd[8] = (U8)(u16BlockNum );

	srb->cmd_len = CB_LENGTH_WRITE_10;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_WRITE;

	/* use the new buffer we have */
	srb->request_buffer = u8Buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = u16BlockNum << us->Mass_stor_device[u8LunNum].u32BlockSize_log2;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}


void vSCSICmd_INQUIRY(struct us_data *us, U8 *u8Buffer,  U8 u8LunNum)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = INQUIRY;
	srb->cmnd[4] = DATA_LENGTH_INQUIRY;

	srb->cmd_len = CB_LENGTH_INQUIRY;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
	srb->request_buffer = u8Buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = DATA_LENGTH_INQUIRY;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}

void vSCSICmd_READ_CAPACITY(struct us_data *us, U8 *u8Buffer,  U8 u8LunNum)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = READ_CAPACITY;

	srb->cmd_len = CB_LENGTH_READ_CAPACITY;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
	srb->request_buffer = u8Buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = DATA_LENGTH_READ_CAPACITY;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}

void vSCSICmd_MODE_SENSE(
	struct us_data *us,
	U8 u8PageCode,
	U8 *u8Buffer,
	U8 u8LunNum,
	U16 uBufLen)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = MODE_SENSE6;
	srb->cmnd[2] = u8PageCode;
	//srb->cmnd[7] = (U8)((uBufLen >> 8) & 0x00FF);
	//srb->cmnd[8] = (U8)uBufLen;
        srb->cmnd[4] = (U8)uBufLen;

	srb->cmd_len = CB_LENGTH_MODE_SENSE;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
	srb->request_buffer = u8Buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = uBufLen;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}

void vSCSICmd_REQUEST_SENSE(struct us_data *us, U8 u8LunNum)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = REQUEST_SENSE;
	srb->cmnd[4] = DATA_LENGTH_REQUEST_SENSE;

	srb->cmd_len = CB_LENGTH_REQUEST_SENSE;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
	srb->request_buffer = srb->sense_buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = DATA_LENGTH_REQUEST_SENSE;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}

void vSCSICmd_TEST_UNIT_READY(struct us_data *us, U8 u8LunNum)
{
	Scsi_Cmnd *srb = &us->srb;

	/* set the command and the LUN */
	memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
	srb->cmnd[0] = TEST_UNIT_READY;

	srb->cmd_len = CB_LENGTH_TEST_UNIT_READY;

	/* set the transfer direction */
	srb->sc_data_direction = SCSI_DATA_READ;

	/* use the new buffer we have */
//	srb->request_buffer = srb->sense_buffer;

	/* set the buffer length for transfer */
	srb->request_bufflen = 0;

	/* set up for no scatter-gather use */
	srb->use_sg = 0;

	/* change the serial number -- toggle the high bit*/
	srb->serial_number ^= 0x80000000;

	/* set Lun number*/
	srb->lun= u8LunNum;

	srb->scsi_done = vScsi_SendCmd_Done;
}


BOOL bSCSI_REQ_SENSE(U8 uPort, U8 uLunNum, U8 *pSenseBuf)
{
    //BOOL	err;
    BOOL      bRet=FALSE;
    struct us_data *pMass_stor;
    U8      host_id;


    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    //err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
    //USB_ASSERT(err, "Wait SCSI sem fail\n");
    //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
    // if (err==FALSE) return FALSE;
    //U32 u32OldIntr;

    // MsOS_DisableAllInterrupts(u32OldIntr);

    //lock_usb_core();
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);

    memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

    vSCSICmd_REQUEST_SENSE(pMass_stor, uLunNum);

    usb_stor_control_thread(pMass_stor);

    if (pMass_stor->srb.result == SAM_STAT_GOOD)
    {
        memcpy(pSenseBuf, pMass_stor->srb.sense_buffer, DATA_LENGTH_REQUEST_SENSE);
        bRet=TRUE;
    }
    else
        bRet=FALSE;

    //unlock_usb_core();
    USBCriticalSectionOut(host_id);

   //MsOS_ReleaseMutex(_s32MutexUSB);
  // MsOS_RestoreAllInterrupts(u32OldIntr);
   return bRet;
}


BOOL bSCSI_INQUIRY(U8 uPort, U8 uLunNum, U8 *pIngBuf)
{
    BOOL	bRet = FALSE;
    int		i;
    struct us_data *pMass_stor;
    U8      host_id;
	//BOOL	err;

	//err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
	//USB_ASSERT(err, "Wait SCSI sem fail\n");
      //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
       //if (err==FALSE) return FALSE;
 //   U32 u32OldIntr;

  //  MsOS_DisableAllInterrupts(u32OldIntr);

	//lock_usb_core();
    //USBCriticalSectionIn(uPort);
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ;i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));
        vSCSICmd_INQUIRY(pMass_stor, pIngBuf, uLunNum);

        usb_stor_control_thread(pMass_stor);

        if(pMass_stor->srb.result == SAM_STAT_GOOD)
        {
            bRet = TRUE;
            break;
        }
    }
    //unlock_usb_core();
    USBCriticalSectionOut(host_id);

	//err = MsOS_ReleaseSemaphore(g_SCSISem);
	//USB_ASSERT(err, "Signal SCSI sem fail\n");
     //MsOS_ReleaseMutex(_s32MutexUSB);
 //  MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

// save all Mode Sense(page code=0x3F) data
// Now we only use u8ModeSenseData[2], this byte save device
// write protection information
BOOL bSCSI_MODE_SENSE(U8 uPort, U8 uLunNum, U8 *pModSenBuf, U16 uBufLen)
{
    int		i;
    BOOL	bRet = FALSE;
    struct us_data *pMass_stor;
    U8      host_id;
	//BOOL			err;

	//err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
	//USB_ASSERT(err, "Wait SCSI sem fail\n");
	//err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
       //if (err==FALSE) return FALSE;
    //U32 u32OldIntr;

   // MsOS_DisableAllInterrupts(u32OldIntr);

	//lock_usb_core();
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ; i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

        vSCSICmd_MODE_SENSE(pMass_stor, 0x3F, pModSenBuf, uLunNum, uBufLen);

        usb_stor_control_thread(pMass_stor);

        if(pMass_stor->srb.result == SAM_STAT_GOOD)
        {
            bRet = TRUE;
            break;
        }

    }
    //unlock_usb_core();
    USBCriticalSectionOut(host_id);

//	err = MsOS_ReleaseSemaphore(g_SCSISem);
//	USB_ASSERT(err, "Signal SCSI sem fail\n");
   //MsOS_ReleaseMutex(_s32MutexUSB);
  // MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

BOOL bSCSI_READ_CAPACITY(U8 uPort, U8 uLunNum, U32 *pTotalBlks, U32 *pBlkSize)
{
    int		i;
    BOOL	bRet = FALSE;
    U8	CapBuf[DATA_LENGTH_READ_CAPACITY];
    struct us_data *pMass_stor;
    U8      host_id;
	//BOOL			err;

	//err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
	//USB_ASSERT(err, "Wait SCSI sem fail\n");
       //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
      //if (err==FALSE) return FALSE;
 //   U32 u32OldIntr;

  //  MsOS_DisableAllInterrupts(u32OldIntr);


	//lock_usb_core();
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return bRet;
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ; i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

        vSCSICmd_READ_CAPACITY(pMass_stor, CapBuf, uLunNum);

        usb_stor_control_thread(pMass_stor);

        if(pMass_stor->srb.result == SAM_STAT_GOOD)
        {
            *pTotalBlks = (((U32)CapBuf[0] << 24) |
            ((U32)CapBuf[1] << 16) |
            ((U32)CapBuf[2] << 8) |
            ((U32)CapBuf[3] )) + 1 ;

            *pBlkSize = ((U32)CapBuf[4] << 24) |
            ((U32)CapBuf[5] << 16) |
            ((U32)CapBuf[6] << 8) |
            ((U32)CapBuf[7] ) ;

            SCSI_DbgPrint("SCSI CAPACITY : SCSI Device total block <0x%x%x>\n",
            (U16)(*pTotalBlks >> 16),
            (U16)*pTotalBlks);
            SCSI_DbgPrint("SCSI CAPACITY : SCSI Product block size <0x%x bytes>\n",
            (U16)*pBlkSize);

            bRet = TRUE;
            break;
        }
    }
    //unlock_usb_core();
    USBCriticalSectionOut(host_id);
#if 0                     //Disable it, for testing new feature which to support different block size
    if(*pBlkSize != 512)  //File system only support BlkSize=512
		bRet = FALSE;
#endif
	//err = MsOS_ReleaseSemaphore(g_SCSISem);
	//USB_ASSERT(err, "Signal SCSI sem fail\n");
    //MsOS_ReleaseMutex(_s32MutexUSB);
   //MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

// GGYY
// Some cameras with CBI subclass can't accept the TUR, but others need.
// Our solution is don't send real TUR to camera. If the command Get Capacity is failed,
// we send the real TUR.
BOOL bSCSI_TEST_UNIT_READY(U8 uPort, U8 uLunNum, BOOL CheckCBI)
{
    int		i;
    BOOL	bRet = FALSE;
    struct us_data *pMass_stor;    U8      host_id;
	//BOOL			err;

	//err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
	//USB_ASSERT(err, "Wait SCSI sem fail\n");
       //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
       //if (err==FALSE) return FALSE;
  //  U32 u32OldIntr;

   // MsOS_DisableAllInterrupts(u32OldIntr);
      // diag_printf("SCSI TEST UNIT READY--");
	//lock_usb_core();
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);

    if (CheckCBI)
    {
        if ( ( Mass_stor_us[uPort] != NULL) &&
            (Mass_stor_us[uPort]->subclass == US_SC_8070) &&
            (Mass_stor_us[uPort]->protocol == US_PR_CBI) )
        {
            bRet = TRUE;
            goto Fun_Exit;
        }
    }

    for(i =0; i< ScsiCmd_Fail_TUR_Retry ;i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        break;

        memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

        vSCSICmd_TEST_UNIT_READY(pMass_stor, uLunNum);

        usb_stor_control_thread(pMass_stor);

        if(pMass_stor->srb.result == SAM_STAT_GOOD)
        {
            //diag_printf(" Succeed for lun %d\n",uLunNum);

            bRet = TRUE;
            break;
        }
        else
        {
            if (i > ScsiCmd_Fail_Retry)
                diag_printf("TUR failed retry %d\n",i);
        }
    }

Fun_Exit: //GGYY
	//unlock_usb_core();
    USBCriticalSectionOut(host_id);

	//err = MsOS_ReleaseSemaphore(g_SCSISem);
	//USB_ASSERT(err, "Signal SCSI sem fail\n");
   //MsOS_ReleaseMutex(_s32MutexUSB);
//   MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

BOOL bInit_USB_LUN(U8 uPort, U8 uLunIdx)
{
    struct us_data *pMass_stor;
    struct LUN_Device* LunDevice;
    U8		pIngBuf[DATA_LENGTH_INQUIRY];
    U8		pModSenBuf[8];
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    LunDevice = pMass_stor->Mass_stor_device;

    if(bSCSI_TEST_UNIT_READY(uPort, uLunIdx, TRUE))	// GGYY
    {
        SCSI_DbgPrint("Scsi Device is ready (Lun=%d).\n", uLunIdx);
    }
    else
        return FALSE;

    if (bSCSI_INQUIRY(uPort, uLunIdx, pIngBuf))
    {
        LunDevice[uLunIdx].u8DevType = (pIngBuf[1] & 0x80) | (pIngBuf[0] & 0x1F);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Device ID <%s>\n",&pMass_stor->device.u8VendorID);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ID <%s>\n",&pMass_stor->device.u8ProductID);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ver <%s>\n",&pMass_stor->device.u8ProductVer);
    }
    else
        return FALSE;

    if( (LunDevice[uLunIdx].u8DevType & 0x1F) == TYPE_ROM) // FS does't support CD-ROM well
    {
        return FALSE;
    }

    if(bSCSI_READ_CAPACITY(uPort, uLunIdx, &LunDevice[uLunIdx].u32BlockTotalNum, &LunDevice[uLunIdx].u32BlockSize))
    {
        SCSI_DbgPrint("Read CAPACITY: TotalBlks %d, BlkSize %d",
                                LunDevice[uLunIdx].u32BlockTotalNum, LunDevice[uLunIdx].u32BlockSize);
        LunDevice[uLunIdx].u32BlockSize_log2 = get_value_log2(LunDevice[uLunIdx].u32BlockSize);
    }
    else
    {
        // GGYY
        // Some cameras need real TUR command
        // to start work. We send real TUR here for these special devices.
        bSCSI_TEST_UNIT_READY(uPort, uLunIdx, FALSE);
        return FALSE;
    }

    LunDevice[uLunIdx].bWriteProtect = FALSE;
    if (bSCSI_MODE_SENSE(uPort, uLunIdx, pModSenBuf, 8))		// Just read the header of mode sense
    {
        LunDevice[uLunIdx].bWriteProtect = (pModSenBuf[2]==0x80) ? TRUE:FALSE;
        if (LunDevice[uLunIdx].bWriteProtect )
        {
            diag_printf("Usb Device WriteProtected! \n");
        }
    }

    return TRUE;
}
extern MS_BOOL UsbReady;
extern MS_BOOL UsbReady_Port2;
BOOL bSCSI_Initial(U8 uPort)
{
    BOOL		bFoundValidDev = FALSE;
    struct us_data *pMass_stor;
    struct LUN_Device* LunDevice;
    U8		pIngBuf[DATA_LENGTH_INQUIRY];
    int			LunIdx;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    LunDevice = pMass_stor->Mass_stor_device;
    // Firstly, we send a Inquiry to every LUN
    // It is simulated from PC
    for (	LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {
        LunDevice[LunIdx].bDeviceReady = FALSE;
        if (LunIdx == 0)
        {
            memset(pIngBuf, 0, DATA_LENGTH_INQUIRY);
            if (bSCSI_INQUIRY(uPort, 0, pIngBuf))
            {
                // save device type
                pMass_stor->device.u8DeviceType = (pIngBuf[0] & 0x1F);
                // save vendor string, total 8 bytes
                memcpy(pMass_stor->device.u8VendorID, &pIngBuf[8], 8);
                // save product string, total 16 bytes
                memcpy(pMass_stor->device.u8ProductID, &pIngBuf[16], 16);
                // save revision information, total 4 bytes
                memcpy(pMass_stor->device.u8ProductVer, &pIngBuf[32], 4);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Device ID <%s>\n",&pMass_stor->device.u8VendorID);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ID <%s>\n",&pMass_stor->device.u8ProductID);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ver <%s>\n",&pMass_stor->device.u8ProductVer);
            }
            else
                goto Init_Done;
        }
        else
        {
            if (!bSCSI_INQUIRY(uPort, LunIdx, pIngBuf))
                goto Init_Done;
        }
    }

    // Start to init every LUN
    for (	LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {
        if (!bInit_USB_LUN(uPort, LunIdx))
            continue;

        LunDevice[LunIdx].bDeviceReady = TRUE;

        if (ConnectUSBDisk(uPort, LunIdx))
            LunDevice[LunIdx].bFSInit = TRUE;

        bFoundValidDev = TRUE;
    }
Init_Done:
    SCSI_DbgPrint("Exit bSCSI_Initial %d\n", bFoundValidDev);

    if (bFoundValidDev)
    {
        if (uPort < MAX_USTOR)
            UsbReady=TRUE;
        else if (uPort < MAX_USTOR*2)
            UsbReady_Port2=TRUE;
    }
    return bFoundValidDev;
}

BOOL ChkUsbReady(void)
{
    return UsbReady;
}

MS_BOOL ChkUsbReadyEx(MS_U8 uHostPort)
{
    if (uHostPort == 0)
            return UsbReady;
    else if (uHostPort == 1)
            return UsbReady_Port2;
    else
    {
        diag_printf("Invalid port number for ChkUsbReadyEx\n");
        return FALSE;
    }
}

BOOL bInit_USB_Disk(U8 uPort)
{
    struct us_data *pMass_stor;
    int		i;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    for (i=0; i<=pMass_stor->max_lun; i++)
    {
        RemoveUSBDiskPort(uPort, i);
        pMass_stor->Mass_stor_device[i].u8LunNum = i;
    }
    return bSCSI_Initial(uPort);
}
#if 1

void vChk_USB_LUNs(U8 uPort)
{
    int			LunIdx;
    struct us_data *pMass_stor;
    struct LUN_Device* LunDevice;
    BOOL		bLUNSts;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return;

    LunDevice = pMass_stor->Mass_stor_device;

    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {	// GGYY
        if ((bLUNSts = bSCSI_TEST_UNIT_READY(uPort, LunIdx, TRUE)) != LunDevice[LunIdx].bDeviceReady) // Status of LUN changed
        {
            if (bLUNSts)	// No media card -> has media card
            {
                if (bInit_USB_LUN(uPort, LunIdx))
                {
                    LunDevice[LunIdx].bDeviceReady = TRUE;
                    RemoveUSBDiskPort(uPort, LunIdx);
                    if (ConnectUSBDisk(uPort, LunIdx))
                        LunDevice[LunIdx].bFSInit = TRUE;
                }
            }
            else	// has media card -> no media card
            {
                LunDevice[LunIdx].bDeviceReady = FALSE;
                LunDevice[LunIdx].bFSInit = FALSE;
                diag_printf("F->R\n");
                RemoveUSBDiskPort(uPort, LunIdx);

            }
        }
    }
}#endif


void vRemove_DISK_LUNs(U8 uPort)
{
    int			LunIdx;
    struct us_data *pMass_stor;
    struct LUN_Device* LunDevice;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return;

    LunDevice = pMass_stor->Mass_stor_device;

    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {	
        LunDevice[LunIdx].bDeviceReady = FALSE;
        LunDevice[LunIdx].bFSInit = FALSE;
        RemoveUSBDiskPort(uPort, LunIdx);
    }
}

BOOL bSCSI_ERASE(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U32 u32SectorSize)
{
	if (uLunNum > Mass_stor_us[uPort]->max_lun)
		return FALSE;
	else
		return TRUE;
}

BOOL bSCSI_Read_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer)
{
	struct us_data *pMass_stor; //USB_2
	//U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
	BOOL	bRet = TRUE;;

	if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
	{
		bRet = FALSE;
		goto Write10_done;
	}

	struct LUN_Device* LunDevice = pMass_stor->Mass_stor_device;
	U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
	U32 u32DataLeft = u32BlockNum << 9;
	U32 u32BlockSize = LunDevice[uLunNum].u32BlockSize;
	U8  *lpu8TempBuf = NULL;

	U32 u32RealBlkAddr;
	U32 u32RealBlkOffset;
	U32 u32RealBlkLast;
	U32 u32RealBlkLeft;
	U32 u32CopyBytes;
	U32 u32BlockNumAdj=0;

	if(u32BlockSize != 512)
	{
	    u32RealBlkAddr = (u32BlockAddr * 512) >> u32BlkSize_log2;
		u32RealBlkOffset = u32BlockAddr * 512 - u32RealBlkAddr * u32BlockSize;

		u32RealBlkLast = ((u32BlockAddr + u32BlockNum) * 512) >> u32BlkSize_log2;
		u32RealBlkLeft = ((u32BlockAddr + u32BlockNum) * 512) - u32RealBlkLast * u32BlockSize;

		u32BlockNumAdj = u32RealBlkLast - u32RealBlkAddr + 1;


		if(u32RealBlkOffset > 0)
		{
		    lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
		    bRet = bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
			if(!bRet)
				goto Write10_done;
			if(u32DataLeft > (u32BlockSize - u32RealBlkOffset))
			{
				u32CopyBytes = u32BlockSize - u32RealBlkOffset;
			    u32DataLeft -= u32CopyBytes;
				//u32ByteOffset = u32RealBlkOffset;
			}
			else
			{
			    u32DataLeft = 0;
				u32CopyBytes = u32DataLeft;
			}

			memcpy(u8Buffer,
				   lpu8TempBuf + u32RealBlkOffset ,
				   (size_t)u32CopyBytes);
			u32BlockNumAdj--;
			u32RealBlkAddr++;
			u8Buffer += u32CopyBytes;
		}//Deal with beginning block

		if(u32BlockNumAdj > 1)  //Transfer mid blocks
		{
      	    if(u32RealBlkLeft != 0)
				u32BlockNumAdj--;

			bRet = bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, u32BlockNumAdj, u8Buffer);
			if(!bRet)
				goto Write10_done;
			u8Buffer += u32BlockNumAdj * u32BlockSize;
			u32DataLeft -= u32BlockNumAdj * u32BlockSize;
		    if(u32DataLeft > 0)
				u32BlockNumAdj = 1;
			else
				u32BlockNumAdj = 0;
		}

		if(u32BlockNumAdj > 0) // Have last block
		{
			if(u32RealBlkLeft > 0)
			{
			    if(lpu8TempBuf == NULL)
			        lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
			    bRet = bSCSI_Read_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
				if(!bRet)
					goto Write10_done;

				memcpy(u8Buffer,
					   lpu8TempBuf,
					   (size_t)u32DataLeft);
			}//Deal with ending block
		}
	}
	else
	{
	    bRet = bSCSI_Read_10(uPort, uLunNum, u32BlockAddr, u32BlockNum, u8Buffer);
	}

Write10_done:
	if(lpu8TempBuf != NULL)
		kfree(lpu8TempBuf);
	return bRet;
}



BOOL bSCSI_Read_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer)
{
	//BOOL			err;

       //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
       //if (err==FALSE) return FALSE;

	struct us_data *pMass_stor; //USB_2
	U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
	BOOL	bRet = TRUE, bConnSts;
	int		i;
    U8      host_id;

//    U32 u32OldIntr;

    //MsOS_DisableAllInterrupts(u32OldIntr);

	//lock_usb_core();

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    host_id = pMass_stor->host_id;
    USBCriticalSectionIn(host_id);
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
    {
        bRet=FALSE;
        goto Func_Done;
    }
    
	struct LUN_Device* LunDevice = pMass_stor->Mass_stor_device;
	U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
	U32 u32DataLeft = u32BlockNum << u32BlkSize_log2;
	if (uLunNum > pMass_stor->max_lun)
	{
		SCSI_DbgPrint("Invalid LUN Index%s","");
		bRet = FALSE;
		goto Func_Done;
	}
	if (LunDevice[uLunNum].u32BlockTotalNum < u32BlockNum)
	{
	       bRet=FALSE;
		goto Func_Done;
	}
	if (LunDevice[uLunNum].bDeviceReady == FALSE)
	{
		bRet = FALSE;
		goto Func_Done;
	}

	while(u32DataLeft > 0)
	{
		if(u32DataLeft > Scsi_Max_Transfer_Len)
		{
			u32TransBlockTmp = Scsi_Max_Transfer_Len >> u32BlkSize_log2;
			u32TransSizeTmp = u32TransBlockTmp << u32BlkSize_log2;
			u32DataLeft -= u32TransSizeTmp;
		}
		else
		{
			u32TransBlockTmp = u32DataLeft >> u32BlkSize_log2;
			u32TransSizeTmp = u32DataLeft;
			u32DataLeft = 0;
		}
	//	err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
		//USB_ASSERT(err, "Wait SCSI sem fail\n");

		for(i =0; i< ScsiCmd_Fail_Retry ;i++)
		{
                        //struct usb_hcd *hcd = pMass_stor->pusb_dev->bus->hcpriv;
                        struct usb_hcd *hcd;
                        
                        hcd = msc_get_hcd(host_id);
                        bConnSts = MDrv_UsbDeviceConnect_EX(hcd) & !(hcd->isRootHubPortReset);

                        if (!bConnSts)
                        {
                            struct ehci_hcd *ehci = hcd_to_ehci(hcd);
                            diag_printf("Device is disconnect @ read 10\n");
                            if (hcd->isRootHubPortReset) // debug purpose
                                diag_printf("bSCSI_Read10:: ehci reset done = %x, isRootHubPortReset = %x\n", ehci->reset_done[0], hcd->isRootHubPortReset);            
                            
                            bRet = FALSE;
                            goto Func_Done;
                        }

                        //if (pMass_stor==NULL)
                        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
                        {
                            diag_printf("Mass_stor_us==NULL!\n");
                            bRet = FALSE;
                            goto Func_Done;
                        }

            			memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

            			// Build SCSI command.
            			vSCSICmd_READ_10(pMass_stor, uLunNum,
            								u32BlockAddr + u32BlockOfSet, (U16)u32TransBlockTmp, u8Buffer + u32TransOfSet);
            			// call mass storage function to send scsi command
            			usb_stor_control_thread(pMass_stor);
            			if (pMass_stor->srb.result != SAM_STAT_GOOD)
            			{
            				if (pMass_stor->srb.result == SAM_STAT_CHECK_CONDITION)
            				{
            					if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x02) &&
            						(pMass_stor->srb.sense_buffer[12]  == 0x3A))
            					{
            						LunDevice[uLunNum].bDeviceReady = FALSE;
            					}
            					else if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x06) &&
            						(pMass_stor->srb.sense_buffer[12]  == 0x28))
            					{
            						LunDevice[uLunNum].bDeviceReady = FALSE;
            					}
            				}

            				SCSI_DbgPrint("Scsi READ_10 command failed.%s","\n");
            				bRet = FALSE;
            			}
            			else
            			{
            				bRet = TRUE;
            				break;
            			}

		}

		//err = MsOS_ReleaseSemaphore(g_SCSISem);
		//USB_ASSERT(err, "Signal SCSI sem fail\n");

		u32BlockOfSet += u32TransBlockTmp;
		u32TransOfSet += u32TransSizeTmp;

		if (bRet == FALSE)
			break;
	}

Func_Done:
    //unlock_usb_core();
    //MsOS_ReleaseMutex(_s32MutexUSB);
    USBCriticalSectionOut(host_id);
    //   MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

BOOL bSCSI_Write_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer)
{
	struct us_data *pMass_stor; //USB_2
	//U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
	BOOL	bRet = TRUE;;

	if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
	{
		bRet = FALSE;
		goto Write10_done;
	}

	struct LUN_Device* LunDevice = pMass_stor->Mass_stor_device;
	U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
	U32 u32DataLeft = u32BlockNum << 9;
	U32 u32BlockSize = LunDevice[uLunNum].u32BlockSize;
	U8  *lpu8TempBuf = NULL;

	U32 u32RealBlkAddr;
	U32 u32RealBlkOffset;
	U32 u32RealBlkLast;
	U32 u32RealBlkLeft;
	U32 u32CopyBytes;
	U32 u32BlockNumAdj=0;

	if(u32BlockSize != 512)
	{
	    u32RealBlkAddr = (u32BlockAddr * 512) >> u32BlkSize_log2;
		u32RealBlkOffset = u32BlockAddr * 512 - u32RealBlkAddr * u32BlockSize;

		u32RealBlkLast = ((u32BlockAddr + u32BlockNum) * 512) >> u32BlkSize_log2;
		u32RealBlkLeft = ((u32BlockAddr + u32BlockNum) * 512) - u32RealBlkLast * u32BlockSize;

		u32BlockNumAdj = u32RealBlkLast - u32RealBlkAddr + 1;


		if(u32RealBlkOffset > 0)
		{
		    lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
		    bRet = bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
			if(!bRet)
				goto Write10_done;
			if(u32DataLeft > (u32BlockSize - u32RealBlkOffset))
			{
				u32CopyBytes = u32BlockSize - u32RealBlkOffset;
			    u32DataLeft -= u32CopyBytes;
				//u32ByteOffset = u32RealBlkOffset;
			}
			else
			{
			    u32DataLeft = 0;
				u32CopyBytes = u32DataLeft;
			}

			memcpy(lpu8TempBuf + u32RealBlkOffset ,
				   u8Buffer,
				   (size_t)u32CopyBytes);
			bRet = bSCSI_Write_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
			if(!bRet)
				goto Write10_done;
			u32BlockNumAdj--;
			u32RealBlkAddr++;
			u8Buffer += u32CopyBytes;
		}//Deal with beginning block

		if(u32BlockNumAdj > 0)  //Transfer mid blocks
		{
		    if(u32RealBlkLeft != 0)
				u32BlockNumAdj--;

			bRet = bSCSI_Write_10(uPort, uLunNum, u32RealBlkAddr, u32BlockNumAdj, u8Buffer);
			if(!bRet)
				goto Write10_done;
			u8Buffer += u32BlockNumAdj * u32BlockSize;
			u32DataLeft -= u32BlockNumAdj * u32BlockSize;
		    if(u32DataLeft > 0)
				u32BlockNumAdj = 1;
			else
				u32BlockNumAdj = 0;
		}

		if(u32BlockNumAdj > 0) // Have last block
		{
			if(u32RealBlkLeft > 0)
			{
			    if(lpu8TempBuf == NULL)
			        lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
			    bRet = bSCSI_Read_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
				if(!bRet)
					goto Write10_done;

				memcpy(lpu8TempBuf,
					   u8Buffer,
					   (size_t)u32DataLeft);
				bRet = bSCSI_Write_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
				if(!bRet)
					goto Write10_done;
			}//Deal with ending block
		}


	}
	else
	{
	    bRet = bSCSI_Write_10(uPort, uLunNum, u32BlockAddr, u32BlockNum, u8Buffer);
	}

Write10_done:
	if(lpu8TempBuf != NULL)
		kfree(lpu8TempBuf);
	return bRet;
}



BOOL bSCSI_Write_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
							U8 *u8Buffer)
{
	//BOOL			err;

       //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
       //if (err==FALSE) return FALSE;

	struct us_data *pMass_stor; //USB_2
	U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
	BOOL	bRet = TRUE, bConnSts;
	int		i;
	int     retrycount=0;
    U8      host_id;
//    U32 u32OldIntr;

    //MsOS_DisableAllInterrupts(u32OldIntr);

	//lock_usb_core();
	if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    
    host_id = pMass_stor->host_id;
	USBCriticalSectionIn(host_id);
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        {
            bRet=FALSE;
            goto Func_Done;
        }

	struct LUN_Device* LunDevice = pMass_stor->Mass_stor_device;
	U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
	U32 u32DataLeft = u32BlockNum << u32BlkSize_log2;

	if (uLunNum > pMass_stor->max_lun)
	{
		SCSI_DbgPrint("Invalid LUN Index%s","");
		bRet = FALSE;
		goto Func_Done;
	}
	if (LunDevice[uLunNum].u32BlockTotalNum < u32BlockNum)
	{
	       bRet=FALSE;
		goto Func_Done;
	}

	if (LunDevice[uLunNum].bDeviceReady == FALSE)
	{
		bRet = FALSE;
		goto Func_Done;
	}

	if(pMass_stor->Mass_stor_device[uLunNum].bWriteProtect)
	{
		SCSI_DbgPrint("Fail to write because write protection%s","");
		bRet = FALSE;
		goto Func_Done;
	}

	while(u32DataLeft > 0)
	{
		if(u32DataLeft > Scsi_Max_Transfer_Len)
		{
			u32TransBlockTmp = Scsi_Max_Transfer_Len >> u32BlkSize_log2;
			u32TransSizeTmp = u32TransBlockTmp << u32BlkSize_log2;
			u32DataLeft -= u32TransSizeTmp;
		}
		else
		{
			u32TransBlockTmp = u32DataLeft >> u32BlkSize_log2;
			u32TransSizeTmp = u32DataLeft;
			u32DataLeft = 0;
		}


		//err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
		//USB_ASSERT(err, "Wait SCSI sem fail\n");

		for(i =0; i< ScsiCmd_Fail_Retry ;i++)
		{
		    retrycount = 0;
			//When USB disk is disconnect or under reseting, FileSystem still writing data
			//and this may caused file system crush. So we block the writing operation for
			//20 secs, waiting for USB disk reseting or reconnect. 
retry:

                    {
                        //struct usb_hcd *hcd = pMass_stor->current_urb->dev->bus->hcpriv;
                        struct usb_hcd *hcd;
                        hcd = msc_get_hcd(host_id);
                        bConnSts = MDrv_UsbDeviceConnect_EX(hcd) & !(hcd->isRootHubPortReset);
                    }

            if (!bConnSts)
            {
                diag_printf("Device is disconnect @ write 10\n");
				retrycount++;
				//if(retrycount <= 10)
				if (0)  //Jonas
				{
					USBCriticalSectionOut(host_id);
	                MsOS_DelayTask(2000);
					USBCriticalSectionIn(host_id);
                    goto retry;
				}
				else
				{
				    bRet = FALSE;
					goto Func_Done;
            	}
			}

            //if (pMass_stor==NULL)
            if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            {
                diag_printf("Mass_stor_us==NULL!\n");				
				retrycount++;
				//if(retrycount <= 10)
				if(0)  //Jonas
				{
					USBCriticalSectionOut(host_id);
	                MsOS_DelayTask(2000);
					USBCriticalSectionIn(host_id);
                    goto retry;
				}
				else
				{
				    bRet = FALSE;
					goto Func_Done;
            	}
            }

			memset(&pMass_stor->srb, 0, sizeof(struct scsi_cmnd ));

			// Build SCSI command.
			vSCSICmd_WRITE_10(pMass_stor, uLunNum,
								u32BlockAddr + u32BlockOfSet, (U16)u32TransBlockTmp, u8Buffer + u32TransOfSet);

			// call mass storage function to send scsi command
			usb_stor_control_thread(pMass_stor);

			if(pMass_stor->srb.result != SAM_STAT_GOOD)
			{
				if (pMass_stor->srb.result == SAM_STAT_CHECK_CONDITION)
				{
					if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x02) &&
						(pMass_stor->srb.sense_buffer[12]  == 0x3A))
					{
						LunDevice[uLunNum].bDeviceReady = FALSE;
					}
					else if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x06) &&
						(pMass_stor->srb.sense_buffer[12]  == 0x28))
					{
						LunDevice[uLunNum].bDeviceReady = FALSE;
					}
				}

				SCSI_DbgPrint("Scsi WRITE_10 command failed.%s","\n");
				bRet = FALSE;
			}
			else
			{
				bRet = TRUE;
				break;
			}
		}

		//err = MsOS_ReleaseSemaphore(g_SCSISem);
		//USB_ASSERT(err, "Signal SCSI sem fail\n");

		u32BlockOfSet += u32TransBlockTmp;
		u32TransOfSet += u32TransSizeTmp;

		if (bRet == FALSE)
			break;
	}

Func_Done:
	//unlock_usb_core();
   //MsOS_ReleaseMutex(_s32MutexUSB);
   USBCriticalSectionOut(host_id);
   //MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

static void vScsi_SendCmd_Done(struct scsi_cmnd *srb)
{

	SCSI_DbgPrint("SCSI command (0x%x) Done, result = 0x%x\n", srb->cmnd[0], srb->result);
}

BOOL bIsDevValid(U8 uPort, U8 LunNum)
{
//	U8	i;
	struct LUN_Device* LunDevice = Mass_stor_us[uPort]->Mass_stor_device;

	if (LunNum <= Mass_stor_us[uPort]->max_lun)
		return (LunDevice[LunNum].bFSInit);
	else
		return FALSE;

//	for (i=0; i<=Mass_stor_us->max_lun; i++)
//		pLunSts[i] = LunDevice[i].bFSInit;
}

U8 u8GetDevType(U8 uPort, U8 LunNum)
{
	struct LUN_Device* LunDevice = Mass_stor_us[uPort]->Mass_stor_device;

	if (LunNum <= Mass_stor_us[uPort]->max_lun)
		return LunDevice[LunNum].u8DevType;
	else
		return  0;
}

