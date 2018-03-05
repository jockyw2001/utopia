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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#if 0
#include <stdlib.h>
#include <assert.h>
#endif
//#include "common.h"
#include "CheckSum.h"

//#define _COUNT4BYTES_
#ifdef _COUNT4BYTES_
#define COUNT_BYTE 4
#else
#define COUNT_BYTE 8
#endif
// Adler-32 has a weakness for short messages with few hundred bytes, because the checksums for these
// messages have a poor coverage of the 32 available bits.
// Problem: The problem is that sum A does not wrap for short messages. The maximum value of A for a 128-byte
// message is 32640, which is below the value 65521 used by the modulo operation.
//unsigned int adler;

void InitCheckSum(unsigned int *p_adler)
{
    *p_adler = 1;
}
//Dn : n-th byte
//s1: 1 + D1 + D2 + D3 + ... + Dn
//s2: (1+D1) + (1+D1+D2) + ... + (1+D1+D2+..+Dn) = n*D1 + (n-1)*D2 + (n-2)*D3 + ... + Dn + n
#if 1
void UpdateCheckSum(unsigned char* ptr, int len, unsigned int *p_adler)
{
    /* RFC 1950 -- Adler-32 checksum */
    typedef unsigned int u32;
    unsigned char * buf = (unsigned char *) ptr;
    const u32 BASE = 65521; //the largest prime number smaller than 2^16
    unsigned int adler = *p_adler;
    u32 s1 = adler & 0xffff;
    u32 s2 = (adler >> 16) & 0xffff;
    /*
    while (len % COUNT_BYTE != 0) {
      s1 += *buf++;
      s2 += s1;
      --len;
    }*/
    do {
        u32 count = (len > 5552 - COUNT_BYTE) ? (5552 - COUNT_BYTE) : len; //5552-8:  the largest number of sums that can be performed without overflowing s2
        len -= count;
        while (count) {
            u32 i;
            if(len==0 && count<COUNT_BYTE) {
                for (i=0; i<count; i++) {
                    s1 += buf[i];
                    s2 += s1;
                }
                for (; i<8; i++) {
                    s1+=/*buf[i]*/0;
                    s2 += s1;
                }
                count = 0;
                break;
            }
          s1 += buf[0]; s2 += s1;
          s1 += buf[1]; s2 += s1;
          s1 += buf[2]; s2 += s1;
          s1 += buf[3]; s2 += s1;
#ifdef _COUNT4BYTES_
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
#else
          s1 += buf[4]; s2 += s1;
          s1 += buf[5]; s2 += s1;
          s1 += buf[6]; s2 += s1;
          s1 += buf[7]; s2 += s1;
#endif
          count -= COUNT_BYTE;
          buf += COUNT_BYTE;
        }
        s1 %= BASE;
        s2 %= BASE;
    } while (len);
    adler = (s2 << 16) + s1;
    *p_adler = adler;
    // Copy from decoder END
}
#else
void UpdateCheckSum(unsigned char* ptr, int len)
{
    /* RFC 1950 -- Adler-32 checksum */
    typedef unsigned int u32;
    unsigned char * buf = (unsigned char *) ptr;
    const u32 BASE = 65521; //the largest prime number smaller than 2^16
    u32 s1 = adler & 0xffff;
    u32 s2 = (adler >> 16) & 0xffff;
    while (len % COUNT_BYTE != 0) {
      s1 += *buf++;
      s2 += s1;
      --len;
    }
    do {
        u32 count = (len > 5552 - COUNT_BYTE) ? (5552 - COUNT_BYTE) : len; //5552-8:  the largest number of sums that can be performed without overflowing s2
        len -= count;
        while (count) {
          s1 += buf[0]; s2 += s1;
          s1 += buf[1]; s2 += s1;
          s1 += buf[2]; s2 += s1;
          s1 += buf[3]; s2 += s1;
#ifdef _COUNT4BYTES_
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
          s1 += 0; s2 += s1;
#else
          s1 += buf[4]; s2 += s1;
          s1 += buf[5]; s2 += s1;
          s1 += buf[6]; s2 += s1;
          s1 += buf[7]; s2 += s1;
#endif
          count -= COUNT_BYTE;
          buf += COUNT_BYTE;
        }
        s1 %= BASE;
        s2 %= BASE;
    } while (len);
    adler = (s2 << 16) + s1;

    // Copy from decoder END
}
#endif
#if 0
void crco_add_frame(uchar* y, uchar* u, uchar* v,
                    int width, int height,
                    int stride_y, int stride_uv,
                    int active_x, int active_y
                    )
{
    int i;
    uchar *src_y;
    uchar *src_u;
    uchar *src_v;

    src_y = (uchar*)y + active_y*stride_y+active_x;
    src_u = (uchar*)u + (active_y>>1)*stride_uv+(active_x>>1);
    src_v = (uchar*)v + (active_y>>1)*stride_uv+(active_x>>1);
    for (i=0; i<height; i++) {
        //fwrite(src_y, 1, width, ofp);
        UpdateCheckSum(src_y, width);
        src_y += stride_y;
    }
    for (i=0; i<height/2; i++) {
        UpdateCheckSum(src_u, width/2);
        src_u += stride_uv;
    }
    for (i=0; i<height/2; i++) {
        UpdateCheckSum(src_v, width/2);
        src_v += stride_uv;
    }
}
#endif
