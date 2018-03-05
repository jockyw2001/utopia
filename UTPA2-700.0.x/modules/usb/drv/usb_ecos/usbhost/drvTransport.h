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


#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <MsTypes.h>
#include  "include/drvConfig.h"
//#include <kernel/blkdev.h>
#include "drvMassStor.h"
#include "drvSCSI.h"
/* Protocols */

#define US_PR_CBI    0x00		/* CBI */
#define US_PR_CB	0x01		/* CB */
#define US_PR_BULK	0x50		/* BOT */
#ifdef CONFIG_USB_STORAGE_HP8200e
#define US_PR_SCM_ATAPI	0x80		
#endif
#ifdef CONFIG_USB_STORAGE_SDDR09
#define US_PR_EUSB_SDDR09	0x81	
#endif
#ifdef CONFIG_USB_STORAGE_SDDR55
#define US_PR_SDDR55	0x82		
#endif
#define US_PR_DPCM_USB  0xf0		

#ifdef CONFIG_USB_STORAGE_FREECOM
#define US_PR_FREECOM   0xf1		
#endif

#ifdef CONFIG_USB_STORAGE_DATAFAB
#define US_PR_DATAFAB   0xf2		
#endif

#ifdef CONFIG_USB_STORAGE_JUMPSHOT
#define US_PR_JUMPSHOT  0xf3		
#endif

#define US_PR_DEVICE	0xff		

struct bulk_cb_wrap {
	U32	Signature;		
	U32	Tag;			
	U32	DataTransferLength;	
	unsigned char	Flags;			
	unsigned char	Lun;			
	unsigned char	Length;			
	unsigned char	CDB[16];		
};

#define US_BULK_CB_WRAP_LEN	31
#define US_BULK_CB_SIGN		0x43425355	
#define US_BULK_FLAG_IN		1
#define US_BULK_FLAG_OUT	0

struct bulk_cs_wrap {
	U32	Signature;		
	U32	Tag;			
	U32	Residue;		
	unsigned char	Status;			
	unsigned char	Filler[18];
};

#define US_BULK_CS_WRAP_LEN	13
#define US_BULK_CS_SIGN		0x53425355	

#define US_BULK_CS_OLYMPUS_SIGN		0x55425355	
#define US_BULK_STAT_OK		0
#define US_BULK_STAT_FAIL	1
#define US_BULK_STAT_PHASE	2

#define US_BULK_RESET_REQUEST	0xff
#define US_BULK_GET_MAX_LUN	0xfe


#define USB_STOR_XFER_GOOD	0	
#define USB_STOR_XFER_SHORT	1	
#define USB_STOR_XFER_STALLED	2	
#define USB_STOR_XFER_LONG	3	
#define USB_STOR_XFER_ERROR	4	
#define USB_STOR_XFER_NO_DEV    5

#define USB_STOR_TRANSPORT_GOOD	   0   
#define USB_STOR_TRANSPORT_FAILED  1   
#define USB_STOR_TRANSPORT_NO_SENSE 2  
#define USB_STOR_TRANSPORT_ERROR   3   
#define USB_STOR_TRANSPORT_NO_DEVICE  4

#define US_CBI_ADSC		0

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_CBI_transport(Scsi_Cmnd*, struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_CB_transport(Scsi_Cmnd*, struct us_data*);
extern int usb_stor_CB_reset(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_Bulk_transport(Scsi_Cmnd*, struct us_data*);
extern int usb_stor_Bulk_max_lun(struct us_data*);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_Bulk_reset(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern void usb_stor_invoke_transport(Scsi_Cmnd*, struct us_data*);
extern void usb_stor_stop_transport(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_control_msg(struct us_data *us, U32 pipe,
		unsigned char request, unsigned char requesttype, U16 value, U16 index,
		void *data, U16 size, int timeout);
extern int usb_stor_clear_halt(struct us_data *us, U32 pipe);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

extern int usb_stor_ctrl_transfer(struct us_data *us, U32 pipe,
		unsigned char request, unsigned char requesttype, U16 value, U16 index,
		void *data, U16 size);
extern int usb_stor_intr_transfer(struct us_data *us, void *buf,
		U32 length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_bulk_transfer_buf(struct us_data *us, U32 pipe,
		void *buf, U32 length, U32 *act_len);




#endif
