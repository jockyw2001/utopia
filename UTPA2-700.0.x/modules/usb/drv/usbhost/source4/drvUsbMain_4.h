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
//	File name: drvUsbMain.H
//	Version: 1.0
//	Date: 2004/12/08
//
//	Company: Faraday Tech. Corp.
//
//	Description:
///////////////////////////////////////////////////////////////////////////////
#ifndef Main_Host_Only__H
#define Main_Host_Only__H

#include "../drvUsbHostConfig.h"

//=================== 1.Condition Definition  ============================================================
//========================================================================================================

	#define mbFUSBH200_HALFSPEEDEnable_Set()		  (mwHost20Bit_Set_Port4(0x40,BIT2))
	#define mbFUSBH200_Interrupt_OutPut_High_Set()	  (mwHost20Bit_Set_Port4(0x40,BIT3))
       #define mbFUSBH200_Interrupt_OutPut_Low_Set()     (mwHost20Bit_Clr_Port4(0x40,BIT3))
	#define mbFUSBH200_VBUS_OFF_Set()		          (mwHost20Bit_Set_Port4(0x40,BIT4))
	#define mbFUSBH200_VBUS_ON_Set()   		          (mwHost20Bit_Clr_Port4(0x40,BIT4))
	#define mbFUSBH200_VBUS_VLD_Rd()   		          (mwHost20Port_Port4(0x40)&BIT8)


extern void FUSBH200_Driver_VBUS(void);
//extern void FUSBH200_Drop_VBUS(void);
extern U8 MDrv_UsbGetMaxLUNCount_Port4(void);
//#define Enable_Issue_TestPacket
//#define Issue_TestPacket
//#define Enable_Burning_Test
//#define Enable_SOF_Only
#define ENABLE_CBI_HOST
//#define Enable_Issue_TestPacketByHW
//#define ENABLE_HOST_TEST
#define DEVICE_ENUM_SEGMENT

void UTMI4_ORXBYTE(U8 offset,U8 val);
void UTMI4_ANDXBYTE(U8 offset,U8 val);
void UTMI4_SETXBYTE(U8 offset,U8 val);
U8 UTMI4_READXBYTE(U8 offset);
void UHC4_ORXBYTE(U8 offset,U8 val);
void UHC4_ANDXBYTE(U8 offset,U8 val);
void UHC4_SETXBYTE(U8 offset,U8 val);
U8 UHC4_READXBYTE(U8 offset);
void USBC4_ORXBYTE(U8 offset,U8 val);
void USBC4_ANDXBYTE(U8 offset,U8 val);
void USBC4_SETXBYTE(U8 offset,U8 val);
U8 USBC4_READXBYTE(U8 offset);

extern U32  gUHC4_BASE;
extern U32  gUTMI_BASE;
extern U32  gUTMI4_BASE;
extern U32  gUSBC4_BASE;

#if defined(CPU_TYPE_AEON)
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr)|0x80000000))
#define KSEG12KSEG0(addr)  ((void *)((U32)(addr)&~0x80000000))
#elif defined(CPU_TYPE_MIPS)

#undef KSEG02KSEG1
#if 0
#define KSEG02KSEG1(addr)  MsOS_PA2KSEG1(MsOS_VA2PA((U32)addr))
#else
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr) | 0xA0000000))
#endif

#undef KSEG12KSEG0
#if 0
#define KSEG12KSEG0(addr)  MsOS_PA2KSEG0(MsOS_VA2PA((U32)addr))
#else
#define KSEG12KSEG0(addr)  ((void *)((U32)(((addr) & ~0x20000000) | 0x80000000)))
#endif

#elif defined(CPU_TYPE_ARM)
#undef KSEG02KSEG1
#define KSEG02KSEG1(addr)  MsOS_PA2KSEG1(MsOS_VA2PA((U32)addr))

#undef KSEG12KSEG0
#define KSEG12KSEG0(addr)  MsOS_PA2KSEG0(MsOS_VA2PA((U32)addr))

#else
##No_CPU_type_for_USB2_KSEG02KSEG1
#endif

#endif
