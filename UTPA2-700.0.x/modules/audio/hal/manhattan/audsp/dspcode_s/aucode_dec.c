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
#define MST_CODEC_DEC_PM1_ADDR     0x1000  
#define MST_CODEC_DEC_PM1_SIZE     0x0012  

BYTE code mst_codec_dec_pm1[]=
{   //PM 1 Addr: 0x1000-0x10FF[0x0100] Len=0x0006(words)/18(bytes)
    0x01, 0x01, 0x00, 0x10, 0x12, 0x00,

    //start:0x001000, len:6(words), 18(bytes)
    0x00, 0x80, 0x07, 0x4A, 0x00, 0x41, 0xCA, 0xC4, 0x93, 0x0F, 0x00, 0x0A, 
    0xB5, 0xC4, 0x33, 0x4F, 0x00, 0x0B, 

};

#define MST_CODEC_DEC_PM2_ADDR     0x23FF  
#define MST_CODEC_DEC_PM2_SIZE     0x0021  

BYTE code mst_codec_dec_pm2[]=
{   //PM 2 Addr: 0x23FF-0x47FF[0x2401] Len=0x000B(words)/33(bytes)
    0x01, 0x01, 0xFF, 0x23, 0x21, 0x00,

    //start:0x0023FF, len:1(words), 3(bytes)
    0x78, 0x88, 0x77, 

    //start:0x002400, len:10(words), 30(bytes)
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 
    0x04, 0x00, 0x00, 0x05, 0x00, 0x00, 0x06, 0x00, 0x00, 0x07, 0x00, 0x00, 
    0x08, 0x00, 0x00, 0x09, 0x00, 0x00, 

};

#define MST_CODEC_DEC_PM3_ADDR     0xA000  
#define MST_CODEC_DEC_PM3_SIZE     0x0003  

BYTE code mst_codec_dec_pm3[]=
{   //PM 3 Addr: 0xA000-0xDFFF[0x4000] Len=0x0001(words)/3(bytes)
    0x01, 0x01, 0x00, 0xA0, 0x03, 0x00,

    //start:0x00A000, len:1(words), 3(bytes)
    0x0F, 0x00, 0x0A, 

};

#define MST_CODEC_DEC_PM4_ADDR     0x10000  
#define MST_CODEC_DEC_PM4_SIZE     0x0012  

BYTE code mst_codec_dec_pm4[]=
{   //PM 4 Addr: 0x10000-0x11FFF[0x2000] Len=0x0006(words)/18(bytes)
    0x01, 0x01, 0x00, 0x00, 0x12, 0x00,

    //start:0x010000, len:6(words), 18(bytes)
    0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xCC, 0xCC, 0xCC, 0xDD, 0xDD, 0xDD, 
    0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 

};

//Total: [PM1 0x0006/0100 words] [PM2 0x000B/2401 words] [PM3 0x0001/4000 words] [PM4 0x0006/2000 words] 
//Version:[77.88.78](codec_dec)
//Time: 2015/09/15 22:52:26
//Generated by MStar Audio DSP Pack Utility Ver. 4.46.00 <ellis.chen>
