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
#ifndef _MADP_MFC_SPI_H_
#define _MADP_MFC_SPI_H_

#include "mdrv_mfc_platform.h"

#define MFC_MADP_IIC_CHANNEL_ID 0x02
#define _BIT0			(0x0001)
#define _BIT1			(0x0002)
#define _BIT2			(0x0004)
#define _BIT3			(0x0008)
#define _BIT4			(0x0010)
#define _BIT5			(0x0020)
#define _BIT6			(0x0040)
#define _BIT7			(0x0080)
#define _BIT8			(0x0100)
#define _BIT9			(0x0200)
#define _BIT10			(0x0400)
#define _BIT11			(0x0800)
#define _BIT12			(0x1000)
#define _BIT13			(0x2000)
#define _BIT14			(0x4000)
#define _BIT15			(0x8000)

//------------------------------------------------------------------------------
// SPI Definition
//------------------------------------------------------------------------------
#define MFC_SPI_SLAVE_ADDR 	0x94
enum
{
	SpiEnter = 0x23,
	SpiReset = 0x24,
};

// Status register[7:0]
#define RDY    _BIT0
#define WEN    _BIT1
#define BP0    _BIT2
#define BP1    _BIT3
#define AAI    _BIT6
#define WPEN   _BIT7

enum
{
	FV_ATMEL = 0x1F,
	FV_ST    = 0x20,	
	FV_PMC   = 0x9D,	
	FV_SST   = 0xBF,
	FV_MXIC  = 0xC2,
	FV_NUM   = 0xFF
};

typedef struct
{
  U8  ManufacturerID;
  U8  ProductID;

  U16 ProgramCKS; // check sum
  U16 VerifyCKS; // check sum
} _FLASHTYPE;

U8 spiRDSR(void);
void spiWaitReady(U8 u8Flag);
void spiWREN(BOOL bCtrl); // Set write enable latch
void spiEWSR(void);
void spiWRSR(U8 u8ID, U8 u8Cmd);
void spiRDID(_FLASHTYPE* Flash);
void spiBlockErase(U8 u8ID, U16 wPage, U8 ucAddr);
U16 spiAAIWriteByte(U16 wPage, U8 u8Start, U16 u16Len, U8 *pBuf);
U16 spiPageWriteByte(U16 wPage, U8 u8Start, U8 u8End, U8 *pBuf);
void spiPageReadByte(U16 wPage, U8 ucStart, U8 ucEnd, U8 *pBuf);

#endif
