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
#ifndef _AUDIO_MBOX_H_
#define _AUDIO_MBOX_H_
/************************************************
*  這個檔案不需要和 Utopia 裡的這個檔案一致
*
*  1. 註解不要用 !
*
************************************************/
#include "sys_main2.h"

#define _COMPILE_DSP_

/********************************************************************
* MIU unit type
********************************************************************/
/* MIU Line Size */
    #define MIU_128                     1

    #if (MIU_128 == 1)
        #define BYTES_IN_MIU_LINE       16
        #define BYTES_IN_MIU_LINE_LOG2  4
    #else
        #define BYTES_IN_MIU_LINE       8
        #define BYTES_IN_MIU_LINE_LOG2  3
    #endif

/************************************************
*   mailbox type
************************************************/
    .const MBOX_BITS_OFFSET  =  0;
    .const MBOX_BITS_SHIFT   =  0;               //The bits shift needed to Align to LSB
    .const MBOX_STEP_ONE     =  0x1;
    .const MBOX_MASK        =  0xFFFF;
    .const MBOX_HIBYTE_MASK = 0x00FF00;
    .const MBOX_LOBYTE_MASK = 0x0000FF;

    .const MBOX_BIT0    =   MBOX_BITS_OFFSET+0;
    .const MBOX_BIT1    =   MBOX_BITS_OFFSET+1;
    .const MBOX_BIT2    =   MBOX_BITS_OFFSET+2;
    .const MBOX_BIT3    =   MBOX_BITS_OFFSET+3;
    .const MBOX_BIT4    =   MBOX_BITS_OFFSET+4;
    .const MBOX_BIT5    =   MBOX_BITS_OFFSET+5;
    .const MBOX_BIT6    =   MBOX_BITS_OFFSET+6;
    .const MBOX_BIT7    =   MBOX_BITS_OFFSET+7;
    .const MBOX_BIT8    =   MBOX_BITS_OFFSET+8;
    .const MBOX_BIT9    =   MBOX_BITS_OFFSET+9;
    .const MBOX_BIT10   =   MBOX_BITS_OFFSET+10;
    .const MBOX_BIT11   =   MBOX_BITS_OFFSET+11;
    .const MBOX_BIT12   =   MBOX_BITS_OFFSET+12;
    .const MBOX_BIT13   =   MBOX_BITS_OFFSET+13;
    .const MBOX_BIT14   =   MBOX_BITS_OFFSET+14;
    .const MBOX_BIT15   =   MBOX_BITS_OFFSET+15;

/************************************************
*   mailbox define
************************************************/
    .const MAIL_IO_BASE                 = 0xB000;

    .const MB_2D00      = MAIL_IO_BASE + 0x00;  ! R
    .const MB_2D02      = MAIL_IO_BASE + 0x01;  ! R
    .const MB_2D04      = MAIL_IO_BASE + 0x02;  ! R
    .const MB_2D06      = MAIL_IO_BASE + 0x03;  ! R
    .const MB_2D08      = MAIL_IO_BASE + 0x04;  ! R
    .const MB_2D0A      = MAIL_IO_BASE + 0x05;  ! R
    .const MB_2D0C      = MAIL_IO_BASE + 0x06;  ! R
    .const MB_2D0E      = MAIL_IO_BASE + 0x07;  ! R
    .const MB_2D10      = MAIL_IO_BASE + 0x08;  ! R
    .const MB_2D12      = MAIL_IO_BASE + 0x09;  ! R
    .const MB_2D14      = MAIL_IO_BASE + 0x0A;  ! R
    .const MB_2D16      = MAIL_IO_BASE + 0x0B;  ! R
    .const MB_2D18      = MAIL_IO_BASE + 0x0C;  ! R
    .const MB_2D1A      = MAIL_IO_BASE + 0x0D;  ! R
    .const MB_2D1C      = MAIL_IO_BASE + 0x0E;  ! R
    .const MB_2D1E      = MAIL_IO_BASE + 0x0F;  ! R
    .const MB_2D20      = MAIL_IO_BASE + 0x10;  ! R
    .const MB_2D22      = MAIL_IO_BASE + 0x11;  ! R
    .const MB_2D24      = MAIL_IO_BASE + 0x12;  ! R
    .const MB_2D26      = MAIL_IO_BASE + 0x13;  ! R
    .const MB_2D28      = MAIL_IO_BASE + 0x14;  ! R
    .const MB_2D2A      = MAIL_IO_BASE + 0x15;  ! R
    .const MB_2D2C      = MAIL_IO_BASE + 0x16;  ! R
    .const MB_2D2E      = MAIL_IO_BASE + 0x17;  ! R
    .const MB_2D30      = MAIL_IO_BASE + 0x18;  ! R
    .const MB_2D32      = MAIL_IO_BASE + 0x19;  ! R
    .const MB_2D34      = MAIL_IO_BASE + 0x1A;  ! R
    .const MB_2D36      = MAIL_IO_BASE + 0x1B;  ! R
    .const MB_2D38      = MAIL_IO_BASE + 0x1C;  ! R
    .const MB_2D3A      = MAIL_IO_BASE + 0x1D;  ! R
    .const MB_2D3C      = MAIL_IO_BASE + 0x1E;  ! R
    .const MB_2D3E      = MAIL_IO_BASE + 0x1F;  ! R
    .const MB_2D40      = MAIL_IO_BASE + 0x20;  ! R
    .const MB_2D42      = MAIL_IO_BASE + 0x21;  ! R
    .const MB_2D44      = MAIL_IO_BASE + 0x22;  ! R
    .const MB_2D46      = MAIL_IO_BASE + 0x23;  ! R
    .const MB_2D48      = MAIL_IO_BASE + 0x24;  ! R
    .const MB_2D4A      = MAIL_IO_BASE + 0x25;  ! R
    .const MB_2D4C      = MAIL_IO_BASE + 0x26;  ! R
    .const MB_2D4E      = MAIL_IO_BASE + 0x27;  ! R
    .const MB_2D50      = MAIL_IO_BASE + 0x28;  ! R
    .const MB_2D52      = MAIL_IO_BASE + 0x29;  ! R
    .const MB_2D54      = MAIL_IO_BASE + 0x2A;  ! R
    .const MB_2D56      = MAIL_IO_BASE + 0x2B;  ! R
    .const MB_2D58      = MAIL_IO_BASE + 0x2C;  ! R
    .const MB_2D5A      = MAIL_IO_BASE + 0x2D;  ! R
    .const MB_2D5C      = MAIL_IO_BASE + 0x2E;  ! R
    .const MB_2D5E      = MAIL_IO_BASE + 0x2F;  ! R
    .const MB_2D60      = MAIL_IO_BASE + 0x30;  ! R/W,
    .const MB_2D62      = MAIL_IO_BASE + 0x31;  ! R/W,
    .const MB_2D64      = MAIL_IO_BASE + 0x32;  ! R/W,
    .const MB_2D66      = MAIL_IO_BASE + 0x33;  ! R/W,
    .const MB_2D68      = MAIL_IO_BASE + 0x34;  ! R/W,
    .const MB_2D6A      = MAIL_IO_BASE + 0x35;  ! R/W,
    .const MB_2D6C      = MAIL_IO_BASE + 0x36;  ! R/W,
    .const MB_2D6E      = MAIL_IO_BASE + 0x37;  ! R/W,
    .const MB_2D70      = MAIL_IO_BASE + 0x38;  ! reserved for SE-DSP
    .const MB_2D72      = MAIL_IO_BASE + 0x39;  ! reserved for SE-DSP
    .const MB_2D74      = MAIL_IO_BASE + 0x3A;  ! reserved for SE-DSP
    .const MB_2D76      = MAIL_IO_BASE + 0x3B;  ! reserved for SE-DSP
    .const MB_2D78      = MAIL_IO_BASE + 0x3C;  ! reserved for SE-DSP
    .const MB_2D7A      = MAIL_IO_BASE + 0x3D;  ! reserved for SE-DSP
    .const MB_2D7C      = MAIL_IO_BASE + 0x3E;  ! reserved for SE-DSP
    .const MB_2D7E      = MAIL_IO_BASE + 0x3F;  ! reserved for SE-DSP
    .const MB_2D80      = MAIL_IO_BASE + 0x40;  ! R
    .const MB_2D82      = MAIL_IO_BASE + 0x41;  ! R
    .const MB_2D84      = MAIL_IO_BASE + 0x42;  ! R
    .const MB_2D86      = MAIL_IO_BASE + 0x43;  ! R
    .const MB_2D88      = MAIL_IO_BASE + 0x44;  ! R
    .const MB_2D8A      = MAIL_IO_BASE + 0x45;  ! R
    .const MB_2D8C      = MAIL_IO_BASE + 0x46;  ! R
    .const MB_2D8E      = MAIL_IO_BASE + 0x47;  ! R
    .const MB_2D90      = MAIL_IO_BASE + 0x48;  ! R
    .const MB_2D92      = MAIL_IO_BASE + 0x49;  ! R
    .const MB_2D94      = MAIL_IO_BASE + 0x4A;  ! R
    .const MB_2D96      = MAIL_IO_BASE + 0x4B;  ! R
    .const MB_2D98      = MAIL_IO_BASE + 0x4C;  ! R
    .const MB_2D9A      = MAIL_IO_BASE + 0x4D;  ! R  , DE-DSP Play Command
    .const MB_2D9C      = MAIL_IO_BASE + 0x4E;  ! R  , System Command + Parameter1 to DE-DSP
    .const MB_2D9E      = MAIL_IO_BASE + 0x4F;  ! R  , System Parameter 2/3 to DE-DSP
    .const MB_2DA0      = MAIL_IO_BASE + 0x50;  ! R/W,
    .const MB_2DA2      = MAIL_IO_BASE + 0x51;  ! R/W,
    .const MB_2DA4      = MAIL_IO_BASE + 0x52;  ! R/W,
    .const MB_2DA6      = MAIL_IO_BASE + 0x53;  ! R/W,
    .const MB_2DA8      = MAIL_IO_BASE + 0x54;  ! R/W,
    .const MB_2DAA      = MAIL_IO_BASE + 0x55;  ! R/W,
    .const MB_2DAC      = MAIL_IO_BASE + 0x56;  ! R/W,
    .const MB_2DAE      = MAIL_IO_BASE + 0x57;  ! R/W,
    .const MB_2DB0      = MAIL_IO_BASE + 0x58;  ! R/W,
    .const MB_2DB2      = MAIL_IO_BASE + 0x59;  ! R/W,
    .const MB_2DB4      = MAIL_IO_BASE + 0x5A;  ! R/W,
    .const MB_2DB6      = MAIL_IO_BASE + 0x5B;  ! R/W,
    .const MB_2DB8      = MAIL_IO_BASE + 0x5C;  ! R/W, MSB: DE-DSP while 1 counter, LSB: Timer counter
    .const MB_2DBA      = MAIL_IO_BASE + 0x5D;  ! R/W,
    .const MB_2DBC      = MAIL_IO_BASE + 0x5E;  ! R/W, DE-DSP Cmd + Parameter 1 ack to System
    .const MB_2DBE      = MAIL_IO_BASE + 0x5F;  ! R/W, DE-DSP Parameter 2/3 ack to System
    .const MB_2DC0      = MAIL_IO_BASE + 0x60;  ! reserved for SE-DSP
    .const MB_2DC2      = MAIL_IO_BASE + 0x61;  ! reserved for SE-DSP
    .const MB_2DC4      = MAIL_IO_BASE + 0x62;  ! reserved for SE-DSP
    .const MB_2DC6      = MAIL_IO_BASE + 0x63;  ! reserved for SE-DSP
    .const MB_2DC8      = MAIL_IO_BASE + 0x64;  ! reserved for SE-DSP
    .const MB_2DCA      = MAIL_IO_BASE + 0x65;  ! reserved for SE-DSP
    .const MB_2DCC      = MAIL_IO_BASE + 0x66;  ! reserved for SE-DSP
    .const MB_2DCE      = MAIL_IO_BASE + 0x67;  ! reserved for SE-DSP
    .const MB_2DD0      = MAIL_IO_BASE + 0x68;  ! reserved for SE-DSP
    .const MB_2DD2      = MAIL_IO_BASE + 0x69;  ! reserved for SE-DSP
    .const MB_2DD4      = MAIL_IO_BASE + 0x6A;  ! reserved for SE-DSP
    .const MB_2DD6      = MAIL_IO_BASE + 0x6B;  ! reserved for SE-DSP
    .const MB_2DD8      = MAIL_IO_BASE + 0x6C;  ! reserved for SE-DSP
    .const MB_2DDA      = MAIL_IO_BASE + 0x6D;  ! reserved for SE-DSP
    .const MB_2DDC      = MAIL_IO_BASE + 0x6E;  ! reserved for SE-DSP
    .const MB_2DDE      = MAIL_IO_BASE + 0x6F;  ! reserved for SE-DSP
    .const MB_2DE0      = MAIL_IO_BASE + 0x70;  ! reserved for SE-DSP
    .const MB_2DE2      = MAIL_IO_BASE + 0x71;  ! reserved for SE-DSP
    .const MB_2DE4      = MAIL_IO_BASE + 0x72;  ! reserved for SE-DSP
    .const MB_2DE6      = MAIL_IO_BASE + 0x73;  ! reserved for SE-DSP
    .const MB_2DE8      = MAIL_IO_BASE + 0x74;  ! reserved for SE-DSP
    .const MB_2DEA      = MAIL_IO_BASE + 0x75;  ! reserved for SE-DSP
    .const MB_2DEC      = MAIL_IO_BASE + 0x76;  ! reserved for SE-DSP
    .const MB_2DEE      = MAIL_IO_BASE + 0x77;  ! reserved for SE-DSP
    .const MB_2DF0      = MAIL_IO_BASE + 0x78;  ! reserved for SE-DSP
    .const MB_2DF2      = MAIL_IO_BASE + 0x79;  ! reserved for SE-DSP
    .const MB_2DF4      = MAIL_IO_BASE + 0x7A;  ! reserved for SE-DSP
    .const MB_2DF6      = MAIL_IO_BASE + 0x7B;  ! reserved for SE-DSP
    .const MB_2DF8      = MAIL_IO_BASE + 0x7C;  ! reserved for SE-DSP
    .const MB_2DFA      = MAIL_IO_BASE + 0x7D;  ! reserved for SE-DSP
    .const MB_2DFC      = MAIL_IO_BASE + 0x7E;  ! reserved for SE-DSP
    .const MB_2DFE      = MAIL_IO_BASE + 0x7F;  ! reserved for SE-DSP

{=== 24-bit MailBox DM IO : SE_DSP to DEC R2  ======================}
    .const DSP_2_DECR2M_MAILBOX0  = MAIL_IO_BASE + 0x0400;
    .const DSP_2_DECR2M_MAILBOX1  = MAIL_IO_BASE + 0x0401;
    .const DSP_2_DECR2M_MAILBOX2  = MAIL_IO_BASE + 0x0402;
    .const DSP_2_DECR2M_MAILBOX3  = MAIL_IO_BASE + 0x0403;
    .const DSP_2_DECR2M_MAILBOX4  = MAIL_IO_BASE + 0x0404;
    .const DSP_2_DECR2M_MAILBOX5  = MAIL_IO_BASE + 0x0405;
    .const DSP_2_DECR2M_MAILBOX6  = MAIL_IO_BASE + 0x0406;
    .const DSP_2_DECR2M_MAILBOX7  = MAIL_IO_BASE + 0x0407;
    .const DSP_2_DECR2M_MAILBOX8  = MAIL_IO_BASE + 0x0408;
    .const DSP_2_DECR2M_MAILBOX9  = MAIL_IO_BASE + 0x0409;
    .const DSP_2_DECR2M_MAILBOXA  = MAIL_IO_BASE + 0x040A;
    .const DSP_2_DECR2M_MAILBOXB  = MAIL_IO_BASE + 0x040B;
    .const DSP_2_DECR2M_MAILBOXC  = MAIL_IO_BASE + 0x040C;
    .const DSP_2_DECR2M_MAILBOXD  = MAIL_IO_BASE + 0x040D;
    .const DSP_2_DECR2M_MAILBOXE  = MAIL_IO_BASE + 0x040E;
    .const DSP_2_DECR2M_MAILBOXF  = MAIL_IO_BASE + 0x040F;

    .const DECR2M_2_DSP_MAILBOX0  = MAIL_IO_BASE + 0x0410;
    .const DECR2M_2_DSP_MAILBOX1  = MAIL_IO_BASE + 0x0411;
    .const DECR2M_2_DSP_MAILBOX2  = MAIL_IO_BASE + 0x0412;
    .const DECR2M_2_DSP_MAILBOX3  = MAIL_IO_BASE + 0x0413;
    .const DECR2M_2_DSP_MAILBOX4  = MAIL_IO_BASE + 0x0414;
    .const DECR2M_2_DSP_MAILBOX5  = MAIL_IO_BASE + 0x0415;
    .const DECR2M_2_DSP_MAILBOX6  = MAIL_IO_BASE + 0x0416;
    .const DECR2M_2_DSP_MAILBOX7  = MAIL_IO_BASE + 0x0417;
    .const DECR2M_2_DSP_MAILBOX8  = MAIL_IO_BASE + 0x0418;
    .const DECR2M_2_DSP_MAILBOX9  = MAIL_IO_BASE + 0x0419;
    .const DECR2M_2_DSP_MAILBOXA  = MAIL_IO_BASE + 0x041A;
    .const DECR2M_2_DSP_MAILBOXB  = MAIL_IO_BASE + 0x041B;
    .const DECR2M_2_DSP_MAILBOXC  = MAIL_IO_BASE + 0x041C;
    .const DECR2M_2_DSP_MAILBOXD  = MAIL_IO_BASE + 0x041D;
    .const DECR2M_2_DSP_MAILBOXE  = MAIL_IO_BASE + 0x041E;
    .const DECR2M_2_DSP_MAILBOXF  = MAIL_IO_BASE + 0x041F;    

#endif //_AUDIO_MBOX_H_
