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
///////////////////////////////////////////////////////////////////////////////
//
//	File name: drvScsi.h
//	Version: 1.0
//	Date: 2003/7/13
//  Description:
//
//
//	Author:
//	Email:
//	Phone: (03) 578-7888
//	Company: Faraday Tech. Corp.
///////////////////////////////////////////////////////////////////////////////
#ifndef SCSI_H
#define SCSI_H



/*
 * This header file contains public constants and structures used by
 * the scsi code for linux.
 */
//=================== 1.Condition Definition  ============================================================
//========================================================================================================
#define ScsiCmd_Fail_Retry		3
#define TestUnitReady_Fail_Retry  0x3
#define Scsi_Max_Transfer_Len    16384 //65536;;yuwen
#define SCSI_DbgPrint         printf
//#define printk                  printf
//=================== 2.Define Data Type  ================================================================
//========================================================================================================

#ifndef BOOLEAN
#define BOOLEAN    UINT8
#endif
#define INT32U     UINT32
#define INT16U     UINT16
#define INT8U      UINT8
/*
 *      SCSI opcodes
 */

#define TEST_UNIT_READY			0x00
#define REQUEST_SENSE			0x03
#define FORMAT_UNIT				0x04		//Option (unsupport)
#define INQUIRY               	0x12
#define MODE_SELECT				0x15		//(unsupport)
#define RESERVE					0x16		//Option (unsupport)
#define RELEASE					0x17		//Option (unsupport)
#define MODE_SENSE				0x1a
#define MODE_SENSE_5A				0x5a
#define START_STOP				0x1b		// (unsupport)
#define ALLOW_MEDIUM_REMOVAL	0x1e
#define READ_CAPACITY			0x25
#define READ_FORMAT_CAPACITY	0x23
#define READ_10					0x28
#define WRITE_10				0x2a
#define VERIFY					0x2f		//(unsupport)
#define SYNCHRONIZE_CACHE		0x35		//Option (unsupport)
#define WRITE_BUFFER			0x3b		//Option (unsupport)
#define PERSISTENT_RESERVE_IN	0x5e		//Option (unsupport)
#define PERSISTENT_RESERVE_OUT	0x5f		//Option (unsupport)

#define SEND_DIAGNOSTIC		0x1d		//(unsupport)

#define TYPE_DISK           0x00

/*
 * FIXME(eric) - one of the great regrets that I have is that I failed to define
 * these structure elements as something like sc_foo instead of foo.  This would
 * make it so much easier to grep through sources and so forth.  I propose that
 * all new elements that get added to these structures follow this convention.
 * As time goes on and as people have the stomach for it, it should be possible to
 * go back and retrofit at least some of the elements here with with the prefix.
 */
 #define MAX_COMMAND_SIZE	16
 #define SCSI_SENSE_BUFFERSIZE 	64

/*
 * These are the values that the SCpnt->sc_data_direction and
 * SRpnt->sr_data_direction can take.  These need to be set
 * The SCSI_DATA_UNKNOWN value is essentially the default.
 * In the event that the command creator didn't bother to
 * set a value, you will see SCSI_DATA_UNKNOWN.
 */
#define SCSI_DATA_UNKNOWN       0
#define SCSI_DATA_WRITE         1
#define SCSI_DATA_READ          2
#define SCSI_DATA_NONE          3

/*
 *  SCSI Architecture Model (SAM) Status codes. Taken from SAM-3 draft
 *  T10/1561-D Revision 4 Draft dated 7th November 2002.
 */
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


/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-indent-level: 4
 * c-brace-imaginary-offset: 0
 * c-brace-offset: -4
 * c-argdecl-indent: 4
 * c-label-offset: -4
 * c-continued-statement-offset: 4
 * c-continued-brace-offset: 0
 * indent-tabs-mode: nil
 * tab-width: 8
 * End:
 */
#define CBW_TAG							0x01020304		// temp

#define CBW_SIGNATE					0x43425355
#define CSW_SIGNATE						0x53425355
#define CSW_STATUS_CMD_PASS			0x00
#define CSW_STATUS_CMD_FAIL			0x01
#define CSW_STATUS_PHASE_ERROR		0x02

#define CBW_FLAG_IN					0x80
#define CBW_FLAG_OUT					0x00

#define CB_LENGTH_TEST_UNIT_READY			0x06
#define CB_LENGTH_REQUEST_SENSE			0x0C
#define CB_LENGTH_INQUIRY               			0x06
#define CB_LENGTH_MODE_SENSE				0x06
#define CB_LENGTH_MODE_SENSE_OperCode5A				0x0B
//#define CB_LENGTH_ALLOW_MEDIUM_REMOVAL	0x1e
#define CB_LENGTH_READ_CAPACITY			0x0A
#define CB_LENGTH_READ_10					0x0A
#define CB_LENGTH_WRITE_10					0x0A



#define DATA_LENGTH_INQUIRY			36
#define DATA_LENGTH_MODE_SENSE		32 //192 , yuwen
#define DATA_LENGTH_MODE_SENSE_OperCode5A		32
#define DATA_LENGTH_REQUEST_SENSE		18
#define DATA_LENGTH_READ_CAPACITY		8






//=================== 3.Structure Definition =============================================================
//========================================================================================================


	//typedef struct CommandBlockWrapper
	//{
	//	UINT32 u32Signature;
	//	INT32U u32Tag;
	//	INT32U u32DataTransferLength;
	//	INT8U u8Flags;
	//	INT8U u8LUN;
	//	INT8U u8CBLength;
	//	INT8U u8CB[16];
	//} CBW;

	//typedef struct CommandStatusWrapper
	//{
	//	INT32U u32Signature;
	//	INT32U u32Tag;
	//	INT32U u32DataResidue;
	//	INT8U u8Status;
	//} CSW;

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
		UINT8 u8DeviceType;
		//UINT8 u8VendorID[8];      //Colin, moved to LUN device
		//UINT8 u8ProductID[16];
		//UINT8 u8ProductVer[4];
	} Scsi_Device;

//	extern Scsi_Device tScsi_Device;
#define BUFFER_LOGI  0
#define BUFFER_PHY	1
struct scsi_cmnd
{

	unsigned char lun;				//no use??
//	unsigned int channel;				//no use
	unsigned char cmd_len;
//	unsigned char old_cmd_len;				//no use
	unsigned char sc_data_direction;
//	unsigned char sc_old_data_direction;				//no use

	/* These elements define the operation we are about to perform */
//    unsigned char cmnd[MAX_COMMAND_SIZE];
	U32 request_bufflen;				/* Actual request size */

	U32 request_buffer;					/* Actual requested buffer */

	U8 result;						/* Status code from lower level driver */

	unsigned char tag;					/* SCSI-II queued command tag */
//	unsigned long pid;					/* Process ID, starts at 0 */
};

typedef struct scsi_cmnd Scsi_Cmnd;




struct LUN_Device
{
	UINT8 u8LunNum;
	/* file system : YPING add */
//	FAT_FileSysStruct FileSys;

	// Get by READ CAPACITY command
	UINT32 u32BlockSize;
	UINT32 u32BlockTotalNum;

	// Get by TEST UNIT READY command
	UINT8 bDeviceReady;
       UINT8 bDeviceValid;
	// Save Write Protection information
	UINT8 bWriteProtect;
	UINT8 bRMB;

	UINT8 u8VendorID[8];
	UINT8 u8ProductID[16];
	UINT8 u8ProductVer[4];
};


struct us_data {

	Scsi_Cmnd		*srb;		 /* current srb		*/
	Scsi_Device 	device;
	UINT8			max_lun;
	// For one Device but have many LUN
	BOOLEAN bFileSystemInit;
	struct LUN_Device* Mass_stor_device;



};



//=================== 4.Extern Function Definition =======================================================
//========================================================================================================

//extern void vSCSI_REQUEST_SENSE(void);
extern BOOLEAN vSCSI_REQUEST_SENSE(U8);
extern BOOLEAN bSCSI_INQUIRY(U8);
extern BOOLEAN bSCSI_READ_CAPACITY(U8);
extern BOOLEAN bSCSI_TEST_UNIT_READY(U8);

extern BOOLEAN bInit_USB_LUN(U8);


extern BOOLEAN bSCSI_Initial(void);

extern BOOLEAN bSCSI_ERASE( struct LUN_Device* LunDevice, UINT32 u32BlockAddr, UINT32 u32BlockNum,
							UINT32 u32SectorSize);
//extern BOOLEAN bSCSI_Read_10(U8 lun,  UINT32 u32BlockAddr, UINT32 u32BlockNum,UINT32 u8Buffer);
//extern BOOLEAN bSCSI_Write_10(U8 lun, UINT32 u32BlockAddr, UINT32 u32BlockNum, UINT32 u8Buffer);
extern BOOLEAN bSCSI_Read_Write10(BOOLEAN DoWrite, U8 lun,  INT32U u32BlockAddr, INT32U u32BlockNum,U32 u8Buffer);
extern BOOLEAN  vSCSI_EJECT_DEVICE(U8 lun);

extern BOOLEAN bSCSI_MODE_SENSE(U8);
extern BOOLEAN bSCSI_MODE_SENSE_OperCode5A(U8);
//=================== 5.Extern Variable Definition =======================================================
//========================================================================================================
extern struct us_data Mass_stor_us1;

//extern struct us_data *Mass_stor_us;
extern struct us_data us_data_Signal;
extern struct LUN_Device Mass_stor_device_1;
//extern INT8U u8Drive;






#endif

