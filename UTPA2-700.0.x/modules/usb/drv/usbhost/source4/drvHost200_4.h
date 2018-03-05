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
//	File name: Host20_AP.H
//	Version: 1.0
//	Date: 2003/7/13
//
//	Author: Bruce
//	Email:
//	Phone: (03) 578-7888
//	Company: Faraday Tech. Corp.
//
//	Description:
///////////////////////////////////////////////////////////////////////////////
 #ifndef HOST200_AP__H

 #define HOST200_AP__H

 #define OTGH_PT_ADVANCE_TEST	                 0

//=================== 1.Condition Definition  ============================================================
//========================================================================================================
 #define OTGH_PT_DataStart	                     0x04000000
 #define OTGH_PT_AUTO_TEST


//=================== 2.Define Data Type  ================================================================
//========================================================================================================
 #define OTGH_PT_CBW_SIGNATE	                 0x43425355
 #define OTGH_PT_CSW_SIGNATE 	                 0x53425355
 #define OTGH_PT_CSW_PASS          			     0x00
 #define OTGH_Dir_IN 	                         0x01
 #define OTGH_Dir_Out 	                         0x00

//=================== 3.Structure Definition =============================================================
//========================================================================================================
 typedef struct
 {
 	UINT8  bDataCounter_IN;	         //0~255
 	UINT8  bDataCounter_OUT;	     //0~255
 	UINT32 wMaxSize;
    //UINT8 *pbDataPage[5];

 }OTGH_PT_CONTROL_Struct;


 typedef struct
 {
 	UINT8 bDataCounterIn;	         //0~255
 	UINT8 bDataCounterOut;	         //0~255
 	UINT8 bMaxSize;              //Assume 13
    UINT8 bINT_Dir;             //0:Init 1:IN 2:Out
 	UINT8 bInQHDArrayNum;
 	UINT8 bOutQHDArrayNum;
 }OTGH_PT_INT_Struct;

 #define OTGH_PT_ISO_DATABUFFER_NUM                600

 typedef struct
 {
 	UINT8       bDataMax;	      //252
 	UINT8       bDataCounter;     //Dynamic index
    UINT32      aDataBufferArray[OTGH_PT_ISO_DATABUFFER_NUM];//Max support 600*4K=2.4M

 }OTGH_PT_ISO_Struct;

 typedef struct CommandBlockWrapper
 {
 	UINT32 u32Signature;
 	UINT32 u32Tag;
 	UINT32 u32DataTransferLength;
 	UINT8 u8Flags;
 	UINT8 u8LUN;
 	UINT8 u8CBLength;
 	UINT8 u8CB[16];
 }OTGH_PT_CBW;

 typedef struct CommandStatusWrapper
 {
 	UINT32 u32Signature;
 	UINT32 u32Tag;
 	UINT32 u32DataResidue;
 	UINT8 u8Status;
 }OTGH_PT_CSW;

 typedef struct
 {
 	OTGH_PT_CBW sCBW;	                 //31 bytes
 	OTGH_PT_CSW sCSW;                    //13bytes
    //UINT32      wDataRange;
 	UINT8        bInQHDArrayNum;
 	UINT8        bOutQHDArrayNum;
    UINT8        bStatus;
 }OTGH_PT_BLK_Struct;

//=================== 4.Extern Function Definition =======================================================
//========================================================================================================
 extern void OTGH_AP_Main(void);
 extern void OTGH_PT_CV(void);
 extern void OTGH_PT_Bulk(void);
 extern void OTGH_PT_Interrupt_In_Out_RandomSize(void);
 extern void OTGH_PT_Interrupt_In_Out_FixSize(void);
 extern void OTGH_PT_GetDescriptor (UINT8 bCounter);

 extern void OTGH_PT_Bulk_SendCBW(UINT32 wLength, UINT16 hwStartAdd,  UINT32 wTagNum , UINT8 bDataDir);
 extern void OTGH_PT_Bulk_ReceiveCSW(void);
 extern void OTGH_PathTest_BULK_INT(void);
 extern void OTGH_PathTest_ISO_IN(void);
 extern void OTGH_PathTest_ISO_OUT(void);
 extern void OTGH_PathTest_Control(void);
 extern void OTGH_Suspend_Test(void);
 extern UINT8 OTGH_SetDescriptor_Test(void);
 extern void OTGH_PT_Control_Path(void);
 extern void OTGH_PT_Control_Command(void);
 extern void OTGH_PT_FrameNumber(void);
 extern void OTGH_PT_CV_Like(void);


extern void OTGC_A_Bus_Drop(void);
extern void OTGC_A_Bus_Drive(void);

extern void OTGH_Hub_Main(void);
extern void OTGH_AP_Faraday(void);
extern void OTGH_PT_Init(void);
extern void Host20_MSCD_MassStorage_Main(void);

extern void OTGH_PT_Bulk_Close(void);

extern void OTGH_AP_RemoteWakeUp_Test(void);
extern void OTGH_AP_Suspend_Resume_Test(void);
extern BOOLEAN MassStorage_Init_Port4(void);

void OTGH_Error_Handle(void);

//=================== 5.Extern Variable Definition =======================================================
//========================================================================================================
 extern  OTGH_PT_BLK_Struct   xdata   *sOTGH_PT_BLK_PORT4;
 extern  OTGH_PT_BLK_Struct   xdata   *pOTGH_PT_BLK_Port4;

#endif //HOST200_AP__H


