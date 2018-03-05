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
#define MST_CODEC_DEC_PM1_ADDR     0x08C0  
#define MST_CODEC_DEC_PM1_SIZE     0x0057  

BYTE code mst_codec_dec_pm1[]=
{   //PM 1 Addr: 0x08C0-0x09BF[0x0100] Len=0x001D(words)/87(bytes)
    0x01, 0x01, 0xC0, 0x08, 0x57, 0x00,

    //start:0x0008C0, len:29(words), 87(bytes)
    0x00, 0x80, 0x07, 0x9A, 0x8D, 0x40, 0x1A, 0xF5, 0x93, 0x00, 0x80, 0x07, 
    0x1A, 0x08, 0x40, 0x3A, 0xAF, 0x93, 0x00, 0x80, 0x07, 0xBA, 0x8D, 0x40, 
    0x2A, 0x79, 0x93, 0x0A, 0x00, 0x40, 0x7A, 0x71, 0x93, 0x0A, 0x00, 0x40, 
    0x4A, 0x76, 0x93, 0x5A, 0xAA, 0x93, 0x0A, 0x00, 0x40, 0x5F, 0xA2, 0x23, 
    0x5A, 0x76, 0x93, 0x6A, 0xAA, 0x93, 0xDA, 0x0B, 0x40, 0x6A, 0x76, 0x93, 
    0x0A, 0xFC, 0x43, 0x7A, 0x76, 0x93, 0xFA, 0x0F, 0x40, 0xEA, 0x73, 0x93, 
    0x0F, 0x00, 0x0A, 0x05, 0xF5, 0x33, 0x4F, 0x00, 0x0B, 0x00, 0x00, 0x00, 
    0x0F, 0x00, 0x0A, 

};

#define MST_CODEC_DEC_PM2_ADDR     0x17FF  
#define MST_CODEC_DEC_PM2_SIZE     0x0021  

BYTE code mst_codec_dec_pm2[]=
{   //PM 2 Addr: 0x17FF-0x4FFF[0x3801] Len=0x000B(words)/33(bytes)
    0x01, 0x01, 0xFF, 0x17, 0x21, 0x00,

    //start:0x0017FF, len:1(words), 3(bytes)
    0x99, 0x88, 0x77, 

    //start:0x001800, len:10(words), 30(bytes)
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 
    0x04, 0x00, 0x00, 0x05, 0x00, 0x00, 0x06, 0x00, 0x00, 0x07, 0x00, 0x00, 
    0x08, 0x00, 0x00, 0x09, 0x00, 0x00, 

};

#define MST_CODEC_DEC_PM3_ADDR     0xB000  
#define MST_CODEC_DEC_PM3_SIZE     0x0003  

BYTE code mst_codec_dec_pm3[]=
{   //PM 3 Addr: 0xB000-0x13BFF[0x8C00] Len=0x0001(words)/3(bytes)
    0x01, 0x01, 0x00, 0xB0, 0x03, 0x00,

    //start:0x00B000, len:1(words), 3(bytes)
    0x0F, 0x00, 0x0A, 

};

#define MST_CODEC_DEC_PM4_ADDR     0x15800  
#define MST_CODEC_DEC_PM4_SIZE     0x0012  

BYTE code mst_codec_dec_pm4[]=
{   //PM 4 Addr: 0x15800-0x1DFFF[0x8800] Len=0x0006(words)/18(bytes)
    0x01, 0x01, 0x00, 0x58, 0x12, 0x00,

    //start:0x015800, len:6(words), 18(bytes)
    0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xCC, 0xCC, 0xCC, 0xDD, 0xDD, 0xDD, 
    0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 

};

//Total: [PM1 0x001D/0100 words] [PM2 0x000B/3801 words] [PM3 0x0001/8C00 words] [PM4 0x0006/8800 words] 
//Version:[77.88.99](codec_dec)
//Time: 2016/10/25 15:29:25
//Generated by MStar Audio DSP Pack Utility Ver. 4.46.00 <richard.ni>
