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


#ifndef SCSI_H
#define SCSI_H

#include <MsTypes.h>
#include "include/drvTypes.h"
#include "drvUsbcommon.h"
//#include  "../../include/kernel/Timer.h"
//#include "../mass_stor/drvMassStor.h"

#define MAX_USTOR  8


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define TEST_UNIT_READY			0x00
#define REQUEST_SENSE			0x03
#define FORMAT_UNIT				0x04		
#define INQUIRY                  	0x12
#define MODE_SELECT				0x15		
#define RESERVE					0x16		
#define RELEASE					0x17		
#define MODE_SENSE				0x5a
#define MODE_SENSE6                     0x1a
#define START_STOP				0x1b		
#define ALLOW_MEDIUM_REMOVAL	0x1e
#define READ_CAPACITY			0x25
#define READ_10					0x28
#define WRITE_10				0x2a
#define VERIFY					0x2f		
#define SYNCHRONIZE_CACHE		0x35		
#define WRITE_BUFFER			0x3b		
#define PERSISTENT_RESERVE_IN	0x5e		
#define PERSISTENT_RESERVE_OUT	0x5f		

#define SEND_DIAGNOSTIC		0x1d		


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define GOOD					0x00
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define CHECK_CONDITION		0x01
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define CONDITION_GOOD			0x02
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define BUSY						0x04
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define INTERMEDIATE_GOOD		0x08
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define INTERMEDIATE_C_GOOD	0x0a
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define RESERVATION_CONFLICT	0x0c
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define COMMAND_TERMINATED	0x11
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define QUEUE_FULL				0x14
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define STATUS_MASK				0x3e

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define NO_SENSE				0x00
#define RECOVERED_ERROR		0x01
#define NOT_READY				0x02
#define MEDIUM_ERROR			0x03
#define HARDWARE_ERROR		0x04
#define ILLEGAL_REQUEST			0x05
#define UNIT_ATTENTION			0x06
#define DATA_PROTECT			0x07
#define BLANK_CHECK			0x08
#define COPY_ABORTED			0x0a
#define ABORTED_COMMAND		0x0b
#define VOLUME_OVERFLOW		0x0d
#define MISCOMPARE				0x0e


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_DISK           0x00
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_TAPE           0x01
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_PRINTER        0x02
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_PROCESSOR      0x03    
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_WORM           0x04    
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_ROM            0x05
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_SCANNER        0x06
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_MOD            0x07    
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_MEDIUM_CHANGER 0x08
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_COMM           0x09    
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_ENCLOSURE      0x0d    
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define TYPE_NO_LUN         0x7f

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define COMMAND_COMPLETE    0x00
#define EXTENDED_MESSAGE    0x01
#define EXTENDED_MODIFY_DATA_POINTER    0x00
#define EXTENDED_SDTR                   0x01
#define EXTENDED_EXTENDED_IDENTIFY      0x02    
#define EXTENDED_WDTR                   0x03
#define SAVE_POINTERS       0x02
#define RESTORE_POINTERS    0x03
#define DISCONNECT          0x04
#define INITIATOR_ERROR     0x05
#define ABORT               0x06
#define MESSAGE_REJECT      0x07
#define NOP                 0x08
#define MSG_PARITY_ERROR    0x09
#define LINKED_CMD_COMPLETE 0x0a
#define LINKED_FLG_CMD_COMPLETE 0x0b
#define BUS_DEVICE_RESET    0x0c

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define INITIATE_RECOVERY   0x0f
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/            
#define RELEASE_RECOVERY    0x10            

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SIMPLE_QUEUE_TAG    0x20
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define HEAD_OF_QUEUE_TAG   0x21
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define ORDERED_QUEUE_TAG   0x22

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define SCSI_IOCTL_GET_IDLUN		0x5382	/* conflicts with CDROMAUDIOBUFSIZ */

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

#define SCSI_IOCTL_TAGGED_ENABLE	0x5383
#define SCSI_IOCTL_TAGGED_DISABLE	0x5384

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SCSI_IOCTL_PROBE_HOST		0x5385

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SCSI_IOCTL_GET_BUS_NUMBER	0x5386

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SCSI_IOCTL_GET_PCI			0x5387

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define status_byte(result) (((result) >> 1) & 0x1f)
#define msg_byte(result)    (((result) >> 8) & 0xff)
#define host_byte(result)   (((result) >> 16) & 0xff)
#define driver_byte(result) (((result) >> 24) & 0xff)
#define suggestion(result)  (driver_byte(result) & SUGGEST_MASK)

#define sense_class(sense)  (((sense) >> 4) & 0x7)
#define sense_error(sense)  ((sense) & 0xf)
#define sense_valid(sense)  ((sense) & 0x80);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define NEEDS_RETRY     0x2001
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SUCCESS         0x2002
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define FAILED          0x2003
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define QUEUED          0x2004
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SOFT_ERROR      0x2005
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define ADD_TO_MLQUEUE  0x2006

#define IDENTIFY_BASE       0x80
#define IDENTIFY(can_disconnect, lun)   (IDENTIFY_BASE |\
		     ((can_disconnect) ?  0x40 : 0) |\
		     ((lun) & 0x07))



/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 #define MAX_COMMAND_SIZE	16
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
 #define SCSI_SENSE_BUFFERSIZE    64

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/


#define DID_OK          0x00	
#define DID_NO_CONNECT  0x01	
#define DID_BUS_BUSY    0x02	
#define DID_TIME_OUT    0x03	
#define DID_BAD_TARGET  0x04	
#define DID_ABORT       0x05	
#define DID_PARITY      0x06	
#define DID_ERROR       0x07	
#define DID_RESET       0x08	
#define DID_BAD_INTR    0x09	
#define DID_PASSTHROUGH 0x0a	
#define DID_SOFT_ERROR  0x0b	
#define DRIVER_OK       0x00	

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SCSI_DATA_UNKNOWN       0
#define SCSI_DATA_WRITE         1
#define SCSI_DATA_READ          2
#define SCSI_DATA_NONE          3

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define SAM_STAT_GOOD            0x00
#define SAM_STAT_CHECK_CONDITION 0x02
#define SAM_STAT_CONDITION_MET   0x04
#define SAM_STAT_BUSY            0x08
#define SAM_STAT_INTERMEDIATE    0x10
#define SAM_STAT_INTERMEDIATE_CONDITION_MET 0x14
#define SAM_STAT_RESERVATION_CONFLICT 0x18
#define SAM_STAT_COMMAND_TERMINATED 0x22	/* obsolete in SAM-3 */
#define SAM_STAT_TASK_SET_FULL   0x28
#define SAM_STAT_ACA_ACTIVE      0x30
#define SAM_STAT_TASK_ABORTED    0x40


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define CBW_TAG							0x01020304		// temp

#define CBW_SIGNATE					0x43425355
#define CSW_SIGNATE						0x53425355
#define CSW_STATUS_CMD_PASS			0x00
#define CSW_STATUS_CMD_FAIL			0x01
#define CSW_STATUS_PHASE_ERROR		0x02

#define CBW_FLAG_IN					0x80
#define CBW_FLAG_OUT					0x00

#if 0
#define CB_LENGTH_TEST_UNIT_READY			0x06
#define CB_LENGTH_REQUEST_SENSE			0x0C
#define CB_LENGTH_INQUIRY                  		0x06
#define CB_LENGTH_MODE_SENSE				0x0C
//#define CB_LENGTH_ALLOW_MEDIUM_REMOVAL	0x1e
#define CB_LENGTH_READ_CAPACITY			0x0A
#define CB_LENGTH_READ_10					0x0A
#define CB_LENGTH_WRITE_10					0x0A
#else
#define CB_LENGTH_TEST_UNIT_READY			0x0C
#define CB_LENGTH_REQUEST_SENSE			0x0C
#define CB_LENGTH_INQUIRY                  		0x0C
#define CB_LENGTH_MODE_SENSE				0x0C
//#define CB_LENGTH_ALLOW_MEDIUM_REMOVAL	0x1e
#define CB_LENGTH_READ_CAPACITY			0x0C
#define CB_LENGTH_READ_10					0x0C
#define CB_LENGTH_WRITE_10					0x0C
#endif

#define DATA_LENGTH_INQUIRY			36
#define DATA_LENGTH_MODE_SENSE		192
#define DATA_LENGTH_REQUEST_SENSE		18
#define DATA_LENGTH_READ_CAPACITY		8

#define REMOVABLE_DEVICE_MASK		0x80

	typedef struct CommandBlockWrapper
	{
		U32 u32Signature;
		U32 u32Tag;
		U32 u32DataTransferLength;
		unsigned char u8Flags;
		unsigned char u8LUN;
		unsigned char u8CBLength;
		unsigned char u8CB[16];
	} CBW;

	typedef struct CommandStatusWrapper
	{
		U32 u32Signature;
		U32 u32Tag;
		U32 u32DataResidue;
		unsigned char u8Status;
	} CSW;

	typedef enum
	{
		SCSI_OPSTATE_TEST_UNIT_READY = 0,
		SCSI_OPSTATE_REQUEST_SENSE,
		SCSI_OPSTATE_INQUIRY,
		SCSI_OPSTATE_MODE_SELECT,
		SCSI_OPSTATE_MODE_SENSE,
		SCSI_OPSTATE_ALLOW_MEDIUM_REMOVAL,
		SCSI_OPSTATE_READ_CAPACITY,
		SCSI_OPSTATE_READ_10,
		SCSI_OPSTATE_WRITE_10
	} ScsiOpState;


	typedef struct ScsiDevice
	{
		// Get by INQUIRY command
		unsigned char u8DeviceType;
		unsigned char u8VendorID[8];
		unsigned char u8ProductID[16];
		unsigned char u8ProductVer[4];
	} Scsi_Device;

//	extern Scsi_Device tScsi_Device;
struct scsi_cmnd
{
//	int     sc_magic;				//no use

//	struct Scsi_Host *host;
//	U16 state;				//no use
//	U16 owner;				//no use

//	Scsi_Device device;

//	Scsi_Request *sc_request;
//	struct scsi_cmnd *next;				//no use
//	struct scsi_cmnd *reset_chain;				//no use

//	int eh_state;					//no use
						/* Used for state tracking in error handlr */
//	void (*done) (struct scsi_cmnd *);				//no use
						/* Mid-level done function */
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

	U32 serial_number;					// CBW Tag
//	U32 serial_number_at_timeout;				//no use

//	int retries;				//no use
//	int allowed;				//no use
//	int timeout_per_command;				//no use
//	int timeout_total;				//no use
//	int timeout;			//no use

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	unsigned volatile char internal_timeout;				//no use
//	struct scsi_cmnd *bh_next;					//no use
									/* To enumerate the commands waiting
					      				to be processed. */

//	U32 target;				//no use
	unsigned char lun;				//no use??
//	U32 channel;				//no use
	unsigned char cmd_len;
//	unsigned char old_cmd_len;				//no use
	unsigned char sc_data_direction;
//	unsigned char sc_old_data_direction;				//no use

	/* These elements define the operation we are about to perform */
	unsigned char cmnd[MAX_COMMAND_SIZE];
	U32 request_bufflen;				/* Actual request size */

//	struct timer_list eh_timeout;			/* Used to time out the command. */
	void *request_buffer;					/* Actual requested buffer */

	/* These elements define the operation we ultimately want to perform */
//	unsigned char data_cmnd[MAX_COMMAND_SIZE];				//no use
//	U16 old_use_sg;				//no use
										/* We save  use_sg here when requesting
					    				* sense info */
	U16 use_sg;				/* Number of pieces of scatter-gather */
//	U16 sglist_len;					//no use
										/* size of malloc'd scatter-gather list */
//	U16 abort_reason;				//no use
										/* If the mid-level code requests an
					    				* abort, this is the reason. */
//	U32 bufflen;						//no use
										/* Size of data buffer */
//	void *buffer;							//no use
										/* Data buffer */

//	U32 underflow;					//no use
										/* Return error if less than
				      					this amount is transferred */
//	U32 old_underflow;				//no use
										/* save underflow here when reusing the
				    					* command for error handling */

//	U32 transfersize;					//no use
									/* How much we are guaranteed to
				      					transfer with each SCSI transfer
				      					(ie, between disconnect /
				      					reconnects.   Probably == sector
				      					size */

	int resid;
									/* Number of bytes requested to be
				     					transferred less actual number
				      					transferred (0 if not supported) */

//	struct request *request;				//no use
									/* The command we are
				            				working on */

	unsigned char sense_buffer[SCSI_SENSE_BUFFERSIZE];		/* obtained by REQUEST SENSE
						    							* when CHECK CONDITION is
						    							* received on original command
						    							* (auto-sense) */

	U32 flags;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	U32 done_late:1;				//no use

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	void (*scsi_done) (struct scsi_cmnd *);

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

//	Scsi_Pointer SCp;					/* Scratchpad used by some host adapters */

//	unsigned char *host_scribble;				//no use
									/* The host adapter is allowed to
									* call scsi_malloc and get some memory
									* and hang it here.     The host adapter
									* is also expected to call scsi_free
									* to release this memory.  (The memory
									* obtained by scsi_malloc is guaranteed
									* to be at an address < 16Mb). */

	int result;						/* Status code from lower level driver */

	unsigned char tag;					/* SCSI-II queued command tag */
//	U32 pid;					/* Process ID, starts at 0 */
};

typedef struct scsi_cmnd Scsi_Cmnd;

extern BOOL bSCSI_INQUIRY(U8 uPort, unsigned char uLunNum, unsigned char *pIngBuf);
extern BOOL bSCSI_READ_CAPACITY(U8 uPort, unsigned char uLunNum,
							U32 *pTotalBlks,
							U32 *pBlkSize);
extern BOOL bSCSI_TEST_UNIT_READY(U8 uPort, unsigned char uLunNum, BOOL CheckCBI); //GGYY
extern BOOL bInit_USB_Disk(U8 uPort);
extern BOOL bSCSI_Read_10(U8 uPort,
                                                unsigned char uLunNum,
                                                U32 u32BlockAddr,
                                                U32 u32BlockNum,
                                                unsigned char *u8Buffer);

extern BOOL bSCSI_Write_10(U8 uPort,
                                                unsigned char uLunNum,
                                                U32 u32BlockAddr,
                                                U32 u32BlockNum,
                                                unsigned char *u8Buffer);

extern BOOL bSCSI_Read_10_512(U8 uPort,
                                                unsigned char uLunNum,
                                                U32 u32BlockAddr,
                                                U32 u32BlockNum,
                                                unsigned char *u8Buffer);

extern BOOL bSCSI_Write_10_512(U8 uPort,
                                                unsigned char uLunNum,
                                                U32 u32BlockAddr,
                                                U32 u32BlockNum,
                                                unsigned char *u8Buffer);

extern BOOL bIsDevValid(U8 uPort, unsigned char LunNum);
extern unsigned char u8GetDevType(U8 uPort, unsigned char LunNum);
extern void vRemove_DISK_LUNs(U8 uPort);
#endif

