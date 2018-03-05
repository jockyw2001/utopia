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
/*********************************************************************
*   HEADER NAME:
*       drvUSBHost_PTP_2.h
*
*   Written by Derek.Horng@MSTAR Inc.
*---------------------------------------------------------------------
*
* Initial revision.
*********************************************************************/
#ifndef DRVUSBHOST_PTP_2_H
#define DRVUSBHOST_PTP_2_H

#include "../../include/drvUsbPTPLib.h"

/*--------------------------------------------------------------------
                            GENERAL INCLUDES
--------------------------------------------------------------------*/


/*--------------------------------------------------------------------
                            LITERAL CONSTANTS
--------------------------------------------------------------------*/
// USB interface class
#define USB_CLASS_IMAGE_INTERFACE       0x06
// interface subclass
#define USB_STILL_IMAGE_CAPTURE_DEVICE  0x01
// interface protocol
#define USB_PIMA15740                 0x01

#define INVALID_HANDLE_NO       0xFFFFFFFF

/*--------------------------------------------------------------------
                                 TYPES
--------------------------------------------------------------------*/
//-------------------------------------------
// The stucture define var
// should be assigned by us
//------------------------------------------
typedef struct _ASSIGNVAR {

    U16     u16OpCode;
    U32     u32TransId;
    U32     u32Buf;
    U32     u32Param1;
    U32     u32Param2;
    U32     u32Param3;
} ASSIGNVAR, *PASSIGNVAR;

//-----------------------------------------------------
// The stucture define variable
// will dynamic change from the data fetting from  DSC
//-----------------------------------------------------
typedef struct _DYMNVAR {

//U8          u8FileName[8];
//U8          u8ExtName [4];
U16         u16FileName[13];
U16         u16FileType;

U32         u32DataLen;
U32         u32StorageId;
U32         u32FileLength;
U32         u32FileOffset;
U32         u32DramAddr;
U32         ImagePixWidth;
U32         ImagePixHeight;
//-------------------------------------------------------
// In current status we do not get every Handle from DSC
// We get the  HandlNum and First Handle ID the seq add
// the Handle ID ---> for saving memory
//-------------------------------------------------------
//U32         u32HandleNum;
U32         u32HandleId;
struct _DATETIME sDateTime;
U16         u16ThumbFormat;
U32         u32ThumbSize;
U32         u32ThumbPixWidth;
U32         u32ThumbPixHeight;
U32         u32ParentObject;
} DYMNVAR, *PDYMNVAR;



typedef struct PTP_CONTAINER{
    U32 u32ConLen;
    U16 u16ConType;
    U16 u16OpCode;
    U32 u32TransActionID;
    U32 u32Param1;
    U32 u32Param2;
    U32 u32Param3;
}OTGH_PTP_CONTAINER;


typedef struct _PTP_DRIVER {
    U32 u32CapacityH;
    U32 u32CapacityL;
    U32 u32FreeSpaceH;
    U32 u32FreeSpaceL;
    U32 u32FirstFileHandleNo;
    U32 u32TotalHandleNo;
    U32 u32FirstHandleNo;
    U32 u32CurHandleIdx;
    U32 u32ObjectHandleNumBuf;
}HOST_PTP_DRIVER;

/*--------------------------------------------------------------------
                            PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                            MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                               VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                 MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                               PROCEDURES
--------------------------------------------------------------------*/
U8 drvUSBHost_PTP_SendOpCode_Port4(ASSIGNVAR xdata *pAssignVar,DYMNVAR xdata *pDymnVar);
void drvUSBHost_PTP_BulkInData_Port4  (ASSIGNVAR xdata *pAssignVar,DYMNVAR xdata *pDymnVar);
void drvUSBHost_PTP_Read1stBulkIn_Port4(ASSIGNVAR xdata *pAssignVar,DYMNVAR xdata *pDymnVar);
void drvUSBHost_PTP_ReadRemBulkIn_Port4(ASSIGNVAR xdata *pAssignVar,DYMNVAR xdata *pDymnVar );
U8 drvUSBHost_PTP_BulkInResponse_Port4(void);
U8 drvUSBHost_PTP_Initial_Port4(void);
//DYMNVAR *drvUSBHost_PTP_GetCurObjectInfo(void);
DYMNVAR *drvUSBHost_PTP_GetObjectInfo_Port4(U32 u32HandleNo );
//DYMNVAR *drvUSBHost_PTP_GetFirstFileObjectInfo(void);
//DYMNVAR *drvUSBHost_PTP_GetNextFileObjectInfo(void);
//DYMNVAR *drvUSBHost_PTP_GetPrevFileObjectInfo(void);
//U32 drvUSBHost_PTP_GetPartialObject(U32 u32TotalSize,U32 u32HandleIdx,U32 u32FileOffset,U32 u32Length,U32 u32DRAMAddr);
void drvUSBHost_PTP_GetBulkIn_Port4(DYMNVAR xdata *pDymnVar);
//U32 drvUSBHost_PTP_GetFreeSpace(void);
U32 drvUSBHost_PTP_GetObjectHandleNo_Port4(U32 u32HandleIdx);
void drvUSBHost_PTP_SaveObjectHandleNo_Port4(U32 u32HandleIdx,U32 u32HandleNo);
U32 drvUSBHost_PTP_GetObjectHandles_Port4(U16 u16ObjFormat, U32 u32AssociObjHandle);
//U32 drvUSBHost_PTP_GetThumbnail(U32 u32TotalSize,U32 u32HandleIdx,U32 u32FileOffset,U32 u32Length,U32 u32DRAMAddr);
#endif  //DRVUSBHOST_PTP_2_H
