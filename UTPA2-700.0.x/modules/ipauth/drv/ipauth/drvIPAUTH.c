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
//******************************************************************************
//  Copyright (c) 2008-2009 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      drvIPAUTH.c
//  [Abstract]:
//      This module contains code for Data Security .
//  [Author(s)]:
//      Hank Kuo
//  [Reversion History]:
//      Initial release:    15 Aprial, 2009
//
//  [Doxygen]
/// @file drvIPAUTH.h
/// @brief Subroutine for sound effect
/// @author MStarSemi Inc.
//*******************************************************************************
#define _DRVIPAUTH_C

//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "ULog.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvIPAUTH.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifndef _DRV_SYS_H_
#include "drvSYS.h"
#endif

#define S_11 7
#define S_12 12
#define S_13 17
#define S_14 22
#define S_21 5
#define S_22 9
#define S_23 14
#define S_24 20
#define S_31 4
#define S_32 11
#define S_33 16
#define S_34 23
#define S_41 6
#define S_42 10
#define S_43 15
#define S_44 21

#define IPAUTH_LOGD(fmt,...)  //printf(fmt, ##__VA_ARGS__)
typedef MS_U8 *POINTER;

// MD5 context.
typedef struct
{
    MS_U32 state[4];        //state (ABCD)
    MS_U32 count[2];        //number of bits, modulo 2^64 (lsb first)
    MS_U8 buffer[64];       //input buffer
} MD5_CTX;


static MS_U8 IPAuthState = 0;
static MS_U8 DolbyVersionState = 0;
static MS_U8 IPControl[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static MS_U8 gCusID[] = {0x00,0x00};
static MS_U8 gCusHash[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static void MDrv_AUTH_MD5Transform (MS_U32 [4], MS_U8 [64]);
static void MDrv_AUTH_Encode (MS_U8 *, MS_U32 *, MS_U16);
static void MDrv_AUTH_Decode (MS_U32 *, MS_U8 *, MS_U16);
static void MDrv_AUTH_MD5_memcpy (POINTER, POINTER, MS_U16);
static void MDrv_AUTH_MD5_memset (POINTER, MS_S32, MS_U16);
static void MDrv_AUTH_MDString (MS_U8 *, MS_U8 *, MS_U8 *);
static MS_U8 MDrv_AUTH_Char2Int(MS_U8 c);

static void MDrv_AUTH_MD5Init (MD5_CTX *);
static void MDrv_AUTH_MD5Update (MD5_CTX *context, MS_U8 *input, MS_U16 inputLen);
static void MDrv_AUTH_MD5Final (MS_U8 [16], MD5_CTX *);

static MS_U8 CONTEXT_PADDING[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static MSIF_Version _drv_ipauth_version = {
    .MW = { IPAUTH_VER },
};

#define ROTATION_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define P(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define Q(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define R(x, y, z) ((x) ^ (y) ^ (z))
#define S(x, y, z) ((y) ^ ((x) | (~z)))

#define PP(a, b, c, d, x, s, ac) { \
    (a) += P ((b), (c), (d)) + (x) + (MS_U32)(ac); \
    (a) = ROTATION_LEFT ((a), (s)); (a) += (b); \
}
#define QQ(a, b, c, d, x, s, ac) { \
    (a) += Q ((b), (c), (d)) + (x) + (MS_U32)(ac); \
    (a) = ROTATION_LEFT ((a), (s)); (a) += (b); \
}
#define RR(a, b, c, d, x, s, ac) { \
    (a) += R ((b), (c), (d)) + (x) + (MS_U32)(ac); \
    (a) = ROTATION_LEFT ((a), (s)); (a) += (b); \
}
#define SS(a, b, c, d, x, s, ac) { \
    (a) += S ((b), (c), (d)) + (x) + (MS_U32)(ac); \
    (a) = ROTATION_LEFT ((a), (s)); (a) += (b); \
}

extern MS_U16 MDrv_SYS_GetChipID(void);
extern MS_BOOL MDrv_SYS_Init(void);
static MS_BOOL  g_bInitShmFlag = FALSE;
ST_IPAUTH_SHARED_VARS * g_IpAuthVars = NULL;
#ifndef MSOS_TYPE_LINUX
ST_IPAUTH_SHARED_VARS   g_ipauthShared;
#endif

extern MS_U32 UtopiaSetIPAUTH(ST_IPAUTH_SHARED_VARS *IpControl, MS_U8 *gCusID, MS_U8 *gCusHush);
extern void MDrv_SYS_GetDolbyKeyCustomer (MS_U8 *key);
//static MS_U8 Dolbybits[] = {11,12,13,14,17,56,57,80,81,82,83,84};
static MS_U8 DolbyInvalid[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFF,0xFD,0x87,0xFF};
                               


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5Init()
/// @brief \b Function \b Description: MD5 initialization. Begins an MD5 operation, writing a new context.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUTH_MD5Init(MD5_CTX *context)
{
    context->count[0] = context->count[1] = 0;
    /* Load magic initialization constants.*/
    //context->state[0] = 0x67452301;
    //context->state[1] = 0xefcdab89;
    //context->state[2] = 0x98badcfe;
    //context->state[3] = 0x10325476;
    context->state[0] = 0xc6f75b28;
    context->state[1] = 0x6b2ccf94;
    context->state[2] = 0xdaff678b;
    context->state[3] = 0x4518c0e8;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5Update()
/// @brief \b Function \b Description:  MD5 block update operation. Continues an MD5 message-digest
///                                                   operation, processing another message block, and updating the
///                                                   context.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUTH_MD5Update (MD5_CTX *context, MS_U8 *input, MS_U16 inputLen)
{
    MS_U16 i, index, partLen;

    // Compute number of bytes mod 64
    index = (MS_U16)((context->count[0] >> 3) & 0x3F);

    // Update number of bits
    if ((context->count[0] += ((MS_U32)inputLen << 3))
     < ((MS_U32)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((MS_U32)inputLen >> 29);

    partLen = 64 - index;

    // Transform as many times as possible.
    if (inputLen >= partLen)
    {
        MDrv_AUTH_MD5_memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
        MDrv_AUTH_MD5Transform (context->state, context->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
            MDrv_AUTH_MD5Transform (context->state, &input[i]);

        index = 0;
    }
    else
        i = 0;

    // Buffer remaining input
    MDrv_AUTH_MD5_memcpy((POINTER)&context->buffer[index], (POINTER)&input[i], inputLen-i);
}

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5Final()
/// @brief \b Function \b Description: This routine is the initialization for KTV sound effect module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUTH_MD5Final(MS_U8 digest[16], MD5_CTX *context)
{
    MS_U8 bits[8];
    MS_U16 index, padLen;

    // Save number of bits
    MDrv_AUTH_Encode (bits, context->count, 8);

    // Pad out to 56 mod 64.
    index = (MS_U16)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MDrv_AUTH_MD5Update (context, CONTEXT_PADDING, padLen);

    // Append length (before padding)
    MDrv_AUTH_MD5Update (context, bits, 8);

    // Store state in digest
    MDrv_AUTH_Encode (digest, context->state, 16);

    // Zeroize sensitive information.
    MDrv_AUTH_MD5_memset ((POINTER)context, 0, sizeof (*context));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5Transform()
/// @brief \b Function \b Description: MD5 basic transformation. Transforms state based on block.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_AUTH_MD5Transform(MS_U32 state[4], MS_U8 block[64])
{
    MS_U32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    MS_U8 i;

    for(i=0;i<=15;i++)
    {
        x[i] = 0;
    }

    MDrv_AUTH_Decode (x, block, 64);

    // 1st Round
    PP (a, b, c, d, x[ 0], S_11, 0xd76aa478);
    PP (d, a, b, c, x[ 1], S_12, 0xe8c7b756);
    PP (c, d, a, b, x[ 2], S_13, 0x242070db);
    PP (b, c, d, a, x[ 3], S_14, 0xc1bdceee);
    PP (a, b, c, d, x[ 4], S_11, 0xf57c0faf);
    PP (d, a, b, c, x[ 5], S_12, 0x4787c62a);
    PP (c, d, a, b, x[ 6], S_13, 0xa8304613);
    PP (b, c, d, a, x[ 7], S_14, 0xfd469501);
    PP (a, b, c, d, x[ 8], S_11, 0x698098d8);
    PP (d, a, b, c, x[ 9], S_12, 0x8b44f7af);
    PP (c, d, a, b, x[10], S_13, 0xffff5bb1);
    PP (b, c, d, a, x[11], S_14, 0x895cd7be);
    PP (a, b, c, d, x[12], S_11, 0x6b901122);
    PP (d, a, b, c, x[13], S_12, 0xfd987193);
    PP (c, d, a, b, x[14], S_13, 0xa679438e);
    PP (b, c, d, a, x[15], S_14, 0x49b40821);

    // 2nd Round
    QQ (a, b, c, d, x[ 1], S_21, 0xf61e2562);
    QQ (d, a, b, c, x[ 6], S_22, 0xc040b340);
    QQ (c, d, a, b, x[11], S_23, 0x265e5a51);
    QQ (b, c, d, a, x[ 0], S_24, 0xe9b6c7aa);
    QQ (a, b, c, d, x[ 5], S_21, 0xd62f105d);
    QQ (d, a, b, c, x[10], S_22,  0x2441453);
    QQ (c, d, a, b, x[15], S_23, 0xd8a1e681);
    QQ (b, c, d, a, x[ 4], S_24, 0xe7d3fbc8);
    QQ (a, b, c, d, x[ 9], S_21, 0x21e1cde6);
    QQ (d, a, b, c, x[14], S_22, 0xc33707d6);
    QQ (c, d, a, b, x[ 3], S_23, 0xf4d50d87);
    QQ (b, c, d, a, x[ 8], S_24, 0x455a14ed);
    QQ (a, b, c, d, x[13], S_21, 0xa9e3e905);
    QQ (d, a, b, c, x[ 2], S_22, 0xfcefa3f8);
    QQ (c, d, a, b, x[ 7], S_23, 0x676f02d9);
    QQ (b, c, d, a, x[12], S_24, 0x8d2a4c8a);

    // 3rd Round
    RR (a, b, c, d, x[ 5], S_31, 0xfffa3942);
    RR (d, a, b, c, x[ 8], S_32, 0x8771f681);
    RR (c, d, a, b, x[11], S_33, 0x6d9d6122);
    RR (b, c, d, a, x[14], S_34, 0xfde5380c);
    RR (a, b, c, d, x[ 1], S_31, 0xa4beea44);
    RR (d, a, b, c, x[ 4], S_32, 0x4bdecfa9);
    RR (c, d, a, b, x[ 7], S_33, 0xf6bb4b60);
    RR (b, c, d, a, x[10], S_34, 0xbebfbc70);
    RR (a, b, c, d, x[13], S_31, 0x289b7ec6);
    RR (d, a, b, c, x[ 0], S_32, 0xeaa127fa);
    RR (c, d, a, b, x[ 3], S_33, 0xd4ef3085);
    RR (b, c, d, a, x[ 6], S_34,  0x4881d05);
    RR (a, b, c, d, x[ 9], S_31, 0xd9d4d039);
    RR (d, a, b, c, x[12], S_32, 0xe6db99e5);
    RR (c, d, a, b, x[15], S_33, 0x1fa27cf8);
    RR (b, c, d, a, x[ 2], S_34, 0xc4ac5665);

    // 4th Round
    SS (a, b, c, d, x[ 0], S_41, 0xf4292244);
    SS (d, a, b, c, x[ 7], S_42, 0x432aff97);
    SS (c, d, a, b, x[14], S_43, 0xab9423a7);
    SS (b, c, d, a, x[ 5], S_44, 0xfc93a039);
    SS (a, b, c, d, x[12], S_41, 0x655b59c3);
    SS (d, a, b, c, x[ 3], S_42, 0x8f0ccc92);
    SS (c, d, a, b, x[10], S_43, 0xffeff47d);
    SS (b, c, d, a, x[ 1], S_44, 0x85845dd1);
    SS (a, b, c, d, x[ 8], S_41, 0x6fa87e4f);
    SS (d, a, b, c, x[15], S_42, 0xfe2ce6e0);
    SS (c, d, a, b, x[ 6], S_43, 0xa3014314);
    SS (b, c, d, a, x[13], S_44, 0x4e0811a1);
    SS (a, b, c, d, x[ 4], S_41, 0xf7537e82);
    SS (d, a, b, c, x[11], S_42, 0xbd3af235);
    SS (c, d, a, b, x[ 2], S_43, 0x2ad7d2bb);
    SS (b, c, d, a, x[ 9], S_44, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    // Zeroize sensitive information.
    MDrv_AUTH_MD5_memset ((POINTER)x, 0, sizeof (x));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_Encode()
/// @brief \b Function \b Description: Encodes input (MS_U32) into output (MS_U8).
///                                    Assumes len is a multiple of 4.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_AUTH_Encode(MS_U8 *output, MS_U32 *input, MS_U16 len)
{
    MS_U16 i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
    {
        output[j] = (MS_U8)(input[i] & 0xff);
        output[j+1] = (MS_U8)((input[i] >> 8) & 0xff);
        output[j+2] = (MS_U8)((input[i] >> 16) & 0xff);
        output[j+3] = (MS_U8)((input[i] >> 24) & 0xff);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_Decode()
/// @brief \b Function \b Description: Decodes input (MS_U8) into output (MS_U32).
/// Assumes len is a multiple of 4.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_AUTH_Decode(MS_U32 *output, MS_U8 *input, MS_U16 len)
{
    MS_U16 i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((MS_U32)input[j]) | (((MS_U32)input[j+1]) << 8) |
            (((MS_U32)input[j+2]) << 16) | (((MS_U32)input[j+3]) << 24);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5_memcpy()
/// @brief \b Function \b Description: Replace "for loop" with standard memcpy if possible.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_AUTH_MD5_memcpy(POINTER output, POINTER input, MS_U16 len)
{
    MS_U16 i;

    for (i = 0; i < len; i++)
        output[i] = input[i];
}

// Note: Replace "for loop" with standard memset if possible.

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MD5_memset()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void MDrv_AUTH_MD5_memset(POINTER output, MS_S32 value, MS_U16 len)
{
    MS_U16 i;

    for (i = 0; i < len; i++)
        ((char *)output)[i] = (char)value;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_MDString()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUTH_MDString(MS_U8 *string, MS_U8 *digest, MS_U8 *key)
{
    MD5_CTX context;
    MS_U16 len = strlen ((char*)string);
    MDrv_AUTH_MD5Init (&context);

    context.state[0] = key[0]+key[1]*256+key[2]*256*256+key[3]*256*256*256;
    context.state[1] = key[4]+key[5]*256+key[6]*256*256+key[7]*256*256*256;
    context.state[2] = key[8]+key[9]*256+key[10]*256*256+key[11]*256*256*256;
    context.state[3] = key[12]+key[13]*256+key[14]*256*256+key[15]*256*256*256;

    MDrv_AUTH_MD5Update (&context, string, len);
    MDrv_AUTH_MD5Final (digest, &context);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUTH_Process()
/// @brief \b Function \b Description: This routine is the initialization for KTV sound effect module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MApi_AUTH_Process(MS_U8 *string, MS_U8 *hash)
{
    MS_U8 digest[16];
    MS_U16 customerid;
    MS_U16 modelid;
    MS_U16 chipid;
    MS_U32 dolbyversion;
    MS_U32 dolbycontrol;
    MS_U32 reverse;
    MS_U8 chiptype;
    MS_U8 KeyCustomer[17] = {0x28,0x5b,0xf7,0xc6,0x94,0xcf,0x2c,0x6b,0x8b,0x67,0xff,0xda,0xe8,0xc0,0x18,0x45,0x00};
    MS_S32 n, i;
    ST_AES_CONTEXT ctx;
    //MS_U8 buf[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    MS_U8 buf[17] = {0x65,0x85,0x68,0xba,0xcb,0xee,0x16,0xc7,0x3c,0x30,0xe8,0xb5,0x44,0x0b,0x95,0x3a};
    MS_U8 enkey[17] = {0x4d,0x53,0x54,0x41,0x52,0x53,0x45,0x4d,0x49,0x24,0x44,0x4f,0x4c,0x42,0x59,0x23,0x00};//MSTARSEMI$DOLBY#
    MS_U8 authmode = 0;
    char t[49];

    MDrv_AUTH_InitialVars();
    MDrv_SYS_Init();

    IPAUTH_LOGD("string len:%d\n",(MS_S32)strlen((char*)string));

    if(strlen((char*)string) == 80)
    {
       strncpy(t, (char*)string + 16, 32);
       t[32] = 0;
       IPAUTH_LOGD("%s\n", t);
       if(strcmp(t,"00000000000000000000000000000000")==0)
       {
          IPAUTH_LOGD("128bit\n");
          strncpy(t, (char*)string, 16);
          strncpy(t+16, (char*)string+48, 32);
          t[48] = 0;
          strncpy((char*)string, t, 49);
       }
    }

    IPAUTH_LOGD("\n");
    dolbycontrol = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_CONTROL_BIT);
    IPAUTH_LOGD("E_SYS_DOLBY_CONTROL_BIT:%4x\n",dolbycontrol);
    chiptype = MDrv_SYS_GetChipType();
    IPAUTH_LOGD("GetChipType:%x\n",chiptype);
    reverse = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_REVERSE_BIT);
    IPAUTH_LOGD("E_SYS_DOLBY_REVERSE_BIT:%4x\n",reverse);

    if(dolbycontrol == 1 && chiptype == 1 && reverse == 0)
    {
        MDrv_SYS_GetDolbyKeyCustomer(buf);
        IPAUTH_LOGD("enKeyCustomer: ");
        for(i=0; i<16; i++)
        {
           IPAUTH_LOGD("%x ",buf[i]);
        }
        IPAUTH_LOGD("\n");

        n = 0;
        MDrv_AUTH_AES_Set_Key( &ctx, enkey, 128 + n * 64 );
        MDrv_AUTH_AES_Decrypt( &ctx, buf, buf );
        IPAUTH_LOGD("decrypted keycustome:");
        for(i=0; i<16; i++)
        {
           IPAUTH_LOGD("%x ",buf[i]);
           KeyCustomer[i] = buf[i];
        }
        IPAUTH_LOGD("\n");
        authmode = 1;
    }
    else
    {
        IPAUTH_LOGD("dkeycustome:");
        for(i=0; i<16; i++)
        {
           IPAUTH_LOGD("%x ",KeyCustomer[i]);
        }
        IPAUTH_LOGD("\n");
    }

    IPAUTH_LOGD("string:%s\n",string);

    IPAUTH_LOGD("hash:");
    for(i=0; i<16; i++)
    {
       IPAUTH_LOGD("%02x ",hash[i]);
    }
    IPAUTH_LOGD("\n");

    for(i=0;i<16;i++)
    {
        digest[i] = 0;   // fix the coverity issue
    }

    MDrv_AUTH_MDString (string, digest, KeyCustomer);

    IPAUTH_LOGD("digest:");
    for(i=0; i<16; i++)
    {
       IPAUTH_LOGD("%02x ",digest[i]);
    }
    IPAUTH_LOGD("\n");

    IPAuthState = 1;

    for(i=0;i<16;i++)
    {
        gCusHash[i] = hash[i];
    } 

    for(i=0;i<16;i++)
    {
        if(digest[i]!=hash[i])
        {
            IPAuthState = 0;
            ULOGI("IPAUTH","Wrong hash key\n");
            break;
        }
    }

    customerid = (MDrv_AUTH_Char2Int(string[0])<<12) + (MDrv_AUTH_Char2Int(string[1])<<8) + (MDrv_AUTH_Char2Int(string[2])<<4) + MDrv_AUTH_Char2Int(string[3]);
    modelid = (MDrv_AUTH_Char2Int(string[4])<<12) + (MDrv_AUTH_Char2Int(string[5])<<8) + (MDrv_AUTH_Char2Int(string[6])<<4) + MDrv_AUTH_Char2Int(string[7]);
    chipid = (MDrv_AUTH_Char2Int(string[8])<<12) + (MDrv_AUTH_Char2Int(string[9])<<8) + (MDrv_AUTH_Char2Int(string[10])<<4) + MDrv_AUTH_Char2Int(string[11]);
    dolbyversion = (MDrv_AUTH_Char2Int(string[14])<<4) + MDrv_AUTH_Char2Int(string[15]);

    gCusID[0] = (MDrv_AUTH_Char2Int(string[0])<<4) + MDrv_AUTH_Char2Int(string[1]);
    gCusID[1] = (MDrv_AUTH_Char2Int(string[2])<<4) + MDrv_AUTH_Char2Int(string[3]);

    ULOGI("IPAUTH","\n%04x%04x%04x\n",customerid,modelid,chipid);

    if((chipid != 0x6666) && (chipid != 0x6667))
    {
        if(chipid != MDrv_SYS_GetChipID())
        {
            IPAuthState = 0;
            ULOGI("IPAUTH","MDrv_SYS_GetChipID:%x\n",MDrv_SYS_GetChipID());
            ULOGI("IPAUTH","Wrong Chip ID\n");
        }
        if(authmode == 1)
        {
            if(dolbyversion != MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_VERSION))
            {
                DolbyVersionState = 1;
                ULOGI("IPAUTH","Wrong Dolby Version\n");
            }
        }        
    }

    if(IPAuthState != 0)
    {
        ULOGI("IPAUTH","Auth OK\n");
        if(strlen((char*)string) == 48)
        {
            for(i=0 ;i<16; i++)
            {
                g_IpAuthVars->g_IpControl[i] = 0;
            }

            for(i=16 ;i<32; i++)
            {
                IPControl[i] = (MDrv_AUTH_Char2Int(string[16 + (i-16)*2])<<4) + MDrv_AUTH_Char2Int(string[16 + (i-16)*2 + 1]);
                g_IpAuthVars->g_IpControl[i] = (MDrv_AUTH_Char2Int(string[16 + (i-16)*2])<<4) + MDrv_AUTH_Char2Int(string[16 + (i-16)*2 + 1]);
            }
            if(DolbyVersionState == 1)
            {            	 
                for(i=0 ;i<32; i++)
                {
                    IPControl[i] = IPControl[i]&DolbyInvalid[i]; 
                    g_IpAuthVars->g_IpControl[i] = IPControl[i];  
                }	
            }            		
        }
        else {
            for(i=0 ;i<32; i++)
            {
                IPControl[i] = (MDrv_AUTH_Char2Int(string[16 + i*2])<<4) + MDrv_AUTH_Char2Int(string[16 + i*2 + 1]);
                g_IpAuthVars->g_IpControl[i] = (MDrv_AUTH_Char2Int(string[16 + i*2])<<4) + MDrv_AUTH_Char2Int(string[16 + i*2 + 1]);
            }
            if(DolbyVersionState == 1)
            {            	 
                for(i=0 ;i<32; i++)
                {
                    IPControl[i] = IPControl[i]&DolbyInvalid[i]; 
                    g_IpAuthVars->g_IpControl[i] = IPControl[i];  
                }	
            }
        }
    }
    else
    {
        ULOGI("IPAUTH","Auth NG\n");
        for(i=0 ;i<32; i++)
        {
            IPControl[i] = 0;
            g_IpAuthVars->g_IpControl[i] = 0;
        }
    }
        
    IPAUTH_LOGD("\n");

    IPAUTH_LOGD("IPCheck:");
    for(i=0;i<256;i++)
    {
       if(MDrv_AUTH_IPCheck(i)==0) IPAUTH_LOGD("%d ",0);
    	 else IPAUTH_LOGD("%d ",1);
    }

    IPAUTH_LOGD("\n");
#ifdef CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER
    UtopiaSetIPAUTH(g_IpAuthVars, gCusID, gCusHash);
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUTH_State()
/// @brief \b Function \b Description: This routine is for returning AUTH state.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MApi_AUTH_State(void)
{
    return IPAuthState;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_Char2Int()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUTH_Char2Int(MS_U8 c)
{
    if(c>=0x61) c = c-0x57;
    else if( c > 0x39) c = c - 0x37;
    else c = c-0x30;

    return (MS_U8)c;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_IPCheck()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit)
{
    MS_U8 rval;

    MDrv_AUTH_InitialVars();

    if (g_IpAuthVars == NULL)
    {
        ULOGI("IPAUTH","%s() : invalid g_IpAuthVars!\n", __FUNCTION__);
        return 0;
    }

    //rval = IPControl[15 - u8Bit/8] & (1<< (u8Bit % 8));
    //rval = g_IpAuthVars->g_IpControl[15 - u8Bit/8] & (1<< (u8Bit % 8));
    rval = g_IpAuthVars->g_IpControl[31 - u8Bit/8] & (1<< (u8Bit % 8));
    if (rval) rval = 1;

    if(u8Bit == IPAUTH_CONTROL_VIDEO_H264)
    {
        //rval = 1;
        if(MDrv_SYS_Query(E_SYS_QUERY_H264_SUPPORTED) == FALSE)
        {
            rval = 0;
        }
    }
    else if(u8Bit == IPAUTH_CONTROL_XC_DVBC)
    {
        rval = 1;
        if(MDrv_SYS_Query(E_SYS_QUERY_DVBC_SUPPORTED) == FALSE)
        {
            rval = 0;
        }
    }
    return rval;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApp_IPAUTH_GetLibVer()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApp_IPAUTH_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return false;
    *ppVersion = &_drv_ipauth_version;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_InitialVars()
/// @brief \b Function \b Description: This routine set ip control share memory.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUTH_InitialVars(void)
{
    if (g_IpAuthVars == NULL)
    {
        g_IpAuthVars = MDrv_AUTH_AllocateVars();
        if (g_IpAuthVars == NULL)
        {
            return FALSE;
        }
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_AllocateVars()
/// @brief \b Function \b Description: This routine allocate ip control share memory.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
ST_IPAUTH_SHARED_VARS * MDrv_AUTH_AllocateVars(void)
{
    ST_IPAUTH_SHARED_VARS * pIPAUTHShared = NULL;

#ifdef MSOS_TYPE_LINUX
    MS_U32 u32ShmId = 0;
    MS_VIRT u32Addr = 0;
    MS_U32 u32BufSize = 0;

//    MsOS_SHM_Init();
    if (MsOS_SHM_GetId((MS_U8 *)IPAUTH_SHM_NAME, sizeof(ST_IPAUTH_SHARED_VARS), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY) == TRUE)
    {
        pIPAUTHShared = (ST_IPAUTH_SHARED_VARS *) u32Addr;

        if (pIPAUTHShared->g_u32ClientCounter == 0)
        {
            IPAUTH_LOGD("reset IP AUTH SHM data ...\n");
            MDrv_AUTH_ResetDefaultVars(pIPAUTHShared);
        }
    }
    else
    {
        IPAUTH_LOGD("create IP AUTH SHM data ...\n");

        if (MsOS_SHM_GetId((MS_U8 *)IPAUTH_SHM_NAME, sizeof(ST_IPAUTH_SHARED_VARS), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE) == TRUE)
        {
            IPAUTH_LOGD("Ip auth SHM data is created\n");
            pIPAUTHShared = (ST_IPAUTH_SHARED_VARS *) u32Addr;

            IPAUTH_LOGD("reset Ip auth SHM data ...\n");
            MDrv_AUTH_ResetDefaultVars(pIPAUTHShared);
        }
        else
        {
            IPAUTH_LOGD("IPAUTH SHM allocation failed!\n");
            return NULL;
        }
    }

    if (g_bInitShmFlag == FALSE)
    {
        g_bInitShmFlag = TRUE;
        pIPAUTHShared->g_u32ClientCounter++;
    }
#else
    pIPAUTHShared = &g_ipauthShared;

    if (g_bInitShmFlag == FALSE)
    {
        IPAUTH_LOGD("create Ipauth SHM data ...\n");
        g_bInitShmFlag = TRUE;

        MDrv_AUTH_ResetDefaultVars(pIPAUTHShared);

        pIPAUTHShared->g_u32ClientCounter++;
    }
#endif

    return pIPAUTHShared;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_ResetDefaultVars()
/// @brief \b Function \b Description: This routine reset ip control share memory.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUTH_ResetDefaultVars(ST_IPAUTH_SHARED_VARS * pIPAUTHShared)
{
    if (pIPAUTHShared == NULL)
    {
        return FALSE;
    }
    memset(pIPAUTHShared, 0x00, sizeof(ST_IPAUTH_SHARED_VARS));
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUTH_GetHashInfo()
/// @brief \b Function \b Description: This routine reports hashinfo for audit.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUTH_GetHashInfo (MS_U8 *hashinfo)
{
    int i;
    hashinfo[0] = gCusID[0];
    hashinfo[1] = gCusID[1];

    for(i=0;i<16;i++)
    {
        hashinfo[i+2] = gCusHash[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
/// AES function
////////////////////////////////////////////////////////////////////////////////
static MS_U32 MS_AUTH_FSb[256];
static MS_U32 MS_AUTH_FT0[256];
static MS_U32 MS_AUTH_FT1[256];
static MS_U32 MS_AUTH_FT2[256];
static MS_U32 MS_AUTH_FT3[256];

/* reverse S-box & tables */

static MS_U32 MS_AUTH_RSb[256];
static MS_U32 MS_AUTH_RT0[256];
static MS_U32 MS_AUTH_RT1[256];
static MS_U32 MS_AUTH_RT2[256];
static MS_U32 MS_AUTH_RT3[256];

/* round constants */

static MS_U32 MS_AUTH_RCON[10];

/* tables generation flag */

static MS_S32 do_init = 1;

/* tables generation routine */

#define MS_AUTH_XTIME(a) ( ( ( 0x80 & a ) ? 0x1B : 0x00 ) ^ ( a << 1 ) )
#define MS_AUTH_MUL(a,b) ( !( a && b ) ? 0 : pow[(log[a] + log[b]) % 255])
#define MS_AUTH_ROTR8(a) ( ( ( 0xFFFFFFFF & a ) >>  8 ) | ( 0xFFFFFFFF & ( a << 24 ) ) )

void MDrv_AUTH_AES_Gen_Tables( void )
{
    MS_S32 i;
    MS_U8 x, y;
    MS_U8 pow[256];
    MS_U8 log[256];

    /* compute pow and log tables over GF(2^8) */

    for( i = 0, x = 1; i < 256; i++, x ^= MS_AUTH_XTIME( x ) )
    {
        pow[i] = x;
        log[x] = i;
    }

    /* calculate the round constants */

    for( i = 0, x = 1; i < 10; i++, x = MS_AUTH_XTIME( x ) )
    {
        MS_AUTH_RCON[i] = (MS_U32) x << 24;
    }

    /* generate the forward and reverse S-boxes */

    MS_AUTH_FSb[0x00] = 0x63;
    MS_AUTH_RSb[0x63] = 0x00;

    for( i = 1; i < 256; i++ )
    {
        x = pow[255 - log[i]];

        y = x;  y = ( y << 1 ) | ( y >> 7 );
        x ^= y; y = ( y << 1 ) | ( y >> 7 );
        x ^= y; y = ( y << 1 ) | ( y >> 7 );
        x ^= y; y = ( y << 1 ) | ( y >> 7 );
        x ^= y ^ 0x63;

        MS_AUTH_FSb[i] = x;
        MS_AUTH_RSb[x] = i;
    }

    /* generate the forward and reverse tables */

    for( i = 0; i < 256; i++ )
    {
        x = (MS_U8) MS_AUTH_FSb[i]; y = MS_AUTH_XTIME( x );

        MS_AUTH_FT0[i] =   (MS_U32) ( x ^ y ) ^
                 ( (MS_U32) x <<  8 ) ^
                 ( (MS_U32) x << 16 ) ^
                 ( (MS_U32) y << 24 );

        MS_AUTH_FT0[i] &= 0xFFFFFFFF;

        MS_AUTH_FT1[i] = MS_AUTH_ROTR8( MS_AUTH_FT0[i] );
        MS_AUTH_FT2[i] = MS_AUTH_ROTR8( MS_AUTH_FT1[i] );
        MS_AUTH_FT3[i] = MS_AUTH_ROTR8( MS_AUTH_FT2[i] );

        y = (MS_U8) MS_AUTH_RSb[i];

        MS_AUTH_RT0[i] = ( (MS_U32) MS_AUTH_MUL( 0x0B, y )       ) ^
                 ( (MS_U32) MS_AUTH_MUL( 0x0D, y ) <<  8 ) ^
                 ( (MS_U32) MS_AUTH_MUL( 0x09, y ) << 16 ) ^
                 ( (MS_U32) MS_AUTH_MUL( 0x0E, y ) << 24 );

        MS_AUTH_RT0[i] &= 0xFFFFFFFF;

        MS_AUTH_RT1[i] = MS_AUTH_ROTR8( MS_AUTH_RT0[i] );
        MS_AUTH_RT2[i] = MS_AUTH_ROTR8( MS_AUTH_RT1[i] );
        MS_AUTH_RT3[i] = MS_AUTH_ROTR8( MS_AUTH_RT2[i] );
    }
}

/* platform-independant 32-bit integer manipulation macros */

#define GET_UINT32(n,b,i)                       \
{                                               \
    (n) = ( (MS_U32) (b)[(i)    ] << 24 )       \
        | ( (MS_U32) (b)[(i) + 1] << 16 )       \
        | ( (MS_U32) (b)[(i) + 2] <<  8 )       \
        | ( (MS_U32) (b)[(i) + 3]       );      \
}

#define PUT_UINT32(n,b,i)                       \
{                                               \
    (b)[(i)    ] = (MS_U8) ( (n) >> 24 );       \
    (b)[(i) + 1] = (MS_U8) ( (n) >> 16 );       \
    (b)[(i) + 2] = (MS_U8) ( (n) >>  8 );       \
    (b)[(i) + 3] = (MS_U8) ( (n)       );       \
}

/* decryption key schedule tables */

static MS_S32 KT_init = 1;

static MS_U32 MS_AUTH_KT0[256];
static MS_U32 MS_AUTH_KT1[256];
static MS_U32 MS_AUTH_KT2[256];
static MS_U32 MS_AUTH_KT3[256];

/* AES key scheduling routine */

MS_S32 MDrv_AUTH_AES_Set_Key( ST_AES_CONTEXT *ctx, MS_U8 *key, MS_S32 nbits )
{
    MS_S32 i;
    MS_U32 *RK, *SK;

    if( do_init )
    {
        MDrv_AUTH_AES_Gen_Tables();

        do_init = 0;
    }

    switch( nbits )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return( 1 );
    }

    RK = ctx->erk;

    for( i = 0; i < (nbits >> 5); i++ )
    {
        GET_UINT32( RK[i], key, i * 4 );
    }

    /* setup encryption round keys */

    switch( nbits )
    {
    case 128:

        for( i = 0; i < 10; i++, RK += 4 )
        {
            RK[4]  = RK[0] ^ MS_AUTH_RCON[i] ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[3] >> 16 ) ] << 24 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[3] >>  8 ) ] << 16 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[3]       ) ] <<  8 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[3] >> 24 ) ]       );

            RK[5]  = RK[1] ^ RK[4];
            RK[6]  = RK[2] ^ RK[5];
            RK[7]  = RK[3] ^ RK[6];
        }
        break;

    case 192:

        for( i = 0; i < 8; i++, RK += 6 )
        {
            RK[6]  = RK[0] ^ MS_AUTH_RCON[i] ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[5] >> 16 ) ] << 24 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[5] >>  8 ) ] << 16 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[5]       ) ] <<  8 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[5] >> 24 ) ]       );

            RK[7]  = RK[1] ^ RK[6];
            RK[8]  = RK[2] ^ RK[7];
            RK[9]  = RK[3] ^ RK[8];
            RK[10] = RK[4] ^ RK[9];
            RK[11] = RK[5] ^ RK[10];
        }
        break;

    case 256:

        for( i = 0; i < 7; i++, RK += 8 )
        {
            RK[8]  = RK[0] ^ MS_AUTH_RCON[i] ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[7] >> 16 ) ] << 24 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[7] >>  8 ) ] << 16 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[7]       ) ] <<  8 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[7] >> 24 ) ]       );

            RK[9]  = RK[1] ^ RK[8];
            RK[10] = RK[2] ^ RK[9];
            RK[11] = RK[3] ^ RK[10];

            RK[12] = RK[4] ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[11] >> 24 ) ] << 24 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[11] >> 16 ) ] << 16 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[11] >>  8 ) ] <<  8 ) ^
                        ( MS_AUTH_FSb[ (MS_U8) ( RK[11]       ) ]       );

            RK[13] = RK[5] ^ RK[12];
            RK[14] = RK[6] ^ RK[13];
            RK[15] = RK[7] ^ RK[14];
        }
        break;
    }

    /* setup decryption round keys */

    if( KT_init )
    {
        for( i = 0; i < 256; i++ )
        {
            MS_AUTH_KT0[i] = MS_AUTH_RT0[ MS_AUTH_FSb[i] ];
            MS_AUTH_KT1[i] = MS_AUTH_RT1[ MS_AUTH_FSb[i] ];
            MS_AUTH_KT2[i] = MS_AUTH_RT2[ MS_AUTH_FSb[i] ];
            MS_AUTH_KT3[i] = MS_AUTH_RT3[ MS_AUTH_FSb[i] ];
        }

        KT_init = 0;
    }

    SK = ctx->drk;

    *SK++ = *RK++;
    *SK++ = *RK++;
    *SK++ = *RK++;
    *SK++ = *RK++;

    for( i = 1; i < ctx->nr; i++ )
    {
        RK -= 8;

        *SK++ = MS_AUTH_KT0[ (MS_U8) ( *RK >> 24 ) ] ^
                MS_AUTH_KT1[ (MS_U8) ( *RK >> 16 ) ] ^
                MS_AUTH_KT2[ (MS_U8) ( *RK >>  8 ) ] ^
                MS_AUTH_KT3[ (MS_U8) ( *RK       ) ]; RK++;

        *SK++ = MS_AUTH_KT0[ (MS_U8) ( *RK >> 24 ) ] ^
                MS_AUTH_KT1[ (MS_U8) ( *RK >> 16 ) ] ^
                MS_AUTH_KT2[ (MS_U8) ( *RK >>  8 ) ] ^
                MS_AUTH_KT3[ (MS_U8) ( *RK       ) ]; RK++;

        *SK++ = MS_AUTH_KT0[ (MS_U8) ( *RK >> 24 ) ] ^
                MS_AUTH_KT1[ (MS_U8) ( *RK >> 16 ) ] ^
                MS_AUTH_KT2[ (MS_U8) ( *RK >>  8 ) ] ^
                MS_AUTH_KT3[ (MS_U8) ( *RK       ) ]; RK++;

        *SK++ = MS_AUTH_KT0[ (MS_U8) ( *RK >> 24 ) ] ^
                MS_AUTH_KT1[ (MS_U8) ( *RK >> 16 ) ] ^
                MS_AUTH_KT2[ (MS_U8) ( *RK >>  8 ) ] ^
                MS_AUTH_KT3[ (MS_U8) ( *RK       ) ]; RK++;
    }

    RK -= 8;

    *SK++ = *RK++;
    *SK++ = *RK++;
    *SK++ = *RK++;
    *SK++ = *RK++;

    return( 0 );
}

/* AES 128-bit block encryption routine */

void MDrv_AUTH_AES_Encrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] )
{
    MS_U32 *RK, A0, A1, A2, A3, B0, B1, B2, B3;

    RK = ctx->erk;

    GET_UINT32( A0, input,  0 ); A0 ^= RK[0];
    GET_UINT32( A1, input,  4 ); A1 ^= RK[1];
    GET_UINT32( A2, input,  8 ); A2 ^= RK[2];
    GET_UINT32( A3, input, 12 ); A3 ^= RK[3];

#define MS_AUTH_AES_FRND(A0,A1,A2,A3,B0,B1,B2,B3)     \
{                                               \
    RK += 4;                                    \
                                                \
    A0 = RK[0] ^ MS_AUTH_FT0[ (MS_U8) ( B0 >> 24 ) ] ^  \
                 MS_AUTH_FT1[ (MS_U8) ( B1 >> 16 ) ] ^  \
                 MS_AUTH_FT2[ (MS_U8) ( B2 >>  8 ) ] ^  \
                 MS_AUTH_FT3[ (MS_U8) ( B3       ) ];   \
                                                \
    A1 = RK[1] ^ MS_AUTH_FT0[ (MS_U8) ( B1 >> 24 ) ] ^  \
                 MS_AUTH_FT1[ (MS_U8) ( B2 >> 16 ) ] ^  \
                 MS_AUTH_FT2[ (MS_U8) ( B3 >>  8 ) ] ^  \
                 MS_AUTH_FT3[ (MS_U8) ( B0       ) ];   \
                                                \
    A2 = RK[2] ^ MS_AUTH_FT0[ (MS_U8) ( B2 >> 24 ) ] ^  \
                 MS_AUTH_FT1[ (MS_U8) ( B3 >> 16 ) ] ^  \
                 MS_AUTH_FT2[ (MS_U8) ( B0 >>  8 ) ] ^  \
                 MS_AUTH_FT3[ (MS_U8) ( B1       ) ];   \
                                                \
    A3 = RK[3] ^ MS_AUTH_FT0[ (MS_U8) ( B3 >> 24 ) ] ^  \
                 MS_AUTH_FT1[ (MS_U8) ( B0 >> 16 ) ] ^  \
                 MS_AUTH_FT2[ (MS_U8) ( B1 >>  8 ) ] ^  \
                 MS_AUTH_FT3[ (MS_U8) ( B2       ) ];   \
}

    MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 1 */
    MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 2 */
    MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 3 */
    MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 4 */
    MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 5 */
    MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 6 */
    MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 7 */
    MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 8 */
    MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 9 */

    if( ctx->nr > 10 )
    {
        MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );   /* round 10 */
        MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );   /* round 11 */
    }

    if( ctx->nr > 12 )
    {
        MS_AUTH_AES_FRND( A0, A1, A2, A3, B0, B1, B2, B3 );   /* round 12 */
        MS_AUTH_AES_FRND( B0, B1, B2, B3, A0, A1, A2, A3 );   /* round 13 */
    }

    /* last round */

    RK += 4;

    A0 = RK[0] ^ ( MS_AUTH_FSb[ (MS_U8) ( B0 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B1 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B2 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B3       ) ]       );

    A1 = RK[1] ^ ( MS_AUTH_FSb[ (MS_U8) ( B1 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B2 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B3 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B0       ) ]       );

    A2 = RK[2] ^ ( MS_AUTH_FSb[ (MS_U8) ( B2 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B3 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B0 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B1       ) ]       );

    A3 = RK[3] ^ ( MS_AUTH_FSb[ (MS_U8) ( B3 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B0 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B1 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_FSb[ (MS_U8) ( B2       ) ]       );

    PUT_UINT32( A0, output,  0 );
    PUT_UINT32( A1, output,  4 );
    PUT_UINT32( A2, output,  8 );
    PUT_UINT32( A3, output, 12 );
}

/* AES 128-bit block decryption routine */

void MDrv_AUTH_AES_Decrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] )
{
    MS_U32 *RK, A0, A1, A2, A3, B0, B1, B2, B3;

    RK = ctx->drk;

    GET_UINT32( A0, input,  0 ); A0 ^= RK[0];
    GET_UINT32( A1, input,  4 ); A1 ^= RK[1];
    GET_UINT32( A2, input,  8 ); A2 ^= RK[2];
    GET_UINT32( A3, input, 12 ); A3 ^= RK[3];

#define MS_AUTH_AES_RRND(A0,A1,A2,A3,B0,B1,B2,B3)     \
{                                               \
    RK += 4;                                    \
                                                \
    A0 = RK[0] ^ MS_AUTH_RT0[ (MS_U8) ( B0 >> 24 ) ] ^  \
                 MS_AUTH_RT1[ (MS_U8) ( B3 >> 16 ) ] ^  \
                 MS_AUTH_RT2[ (MS_U8) ( B2 >>  8 ) ] ^  \
                 MS_AUTH_RT3[ (MS_U8) ( B1       ) ];   \
                                                \
    A1 = RK[1] ^ MS_AUTH_RT0[ (MS_U8) ( B1 >> 24 ) ] ^  \
                 MS_AUTH_RT1[ (MS_U8) ( B0 >> 16 ) ] ^  \
                 MS_AUTH_RT2[ (MS_U8) ( B3 >>  8 ) ] ^  \
                 MS_AUTH_RT3[ (MS_U8) ( B2       ) ];   \
                                                \
    A2 = RK[2] ^ MS_AUTH_RT0[ (MS_U8) ( B2 >> 24 ) ] ^  \
                 MS_AUTH_RT1[ (MS_U8) ( B1 >> 16 ) ] ^  \
                 MS_AUTH_RT2[ (MS_U8) ( B0 >>  8 ) ] ^  \
                 MS_AUTH_RT3[ (MS_U8) ( B3       ) ];   \
                                                \
    A3 = RK[3] ^ MS_AUTH_RT0[ (MS_U8) ( B3 >> 24 ) ] ^  \
                 MS_AUTH_RT1[ (MS_U8) ( B2 >> 16 ) ] ^  \
                 MS_AUTH_RT2[ (MS_U8) ( B1 >>  8 ) ] ^  \
                 MS_AUTH_RT3[ (MS_U8) ( B0       ) ];   \
}

    MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 1 */
    MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 2 */
    MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 3 */
    MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 4 */
    MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 5 */
    MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 6 */
    MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 7 */
    MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );       /* round 8 */
    MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );       /* round 9 */

    if( ctx->nr > 10 )
    {
        MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );   /* round 10 */
        MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );   /* round 11 */
    }

    if( ctx->nr > 12 )
    {
        MS_AUTH_AES_RRND( A0, A1, A2, A3, B0, B1, B2, B3 );   /* round 12 */
        MS_AUTH_AES_RRND( B0, B1, B2, B3, A0, A1, A2, A3 );   /* round 13 */
    }

    /* last round */

    RK += 4;

    A0 = RK[0] ^ ( MS_AUTH_RSb[ (MS_U8) ( B0 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B3 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B2 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B1       ) ]       );

    A1 = RK[1] ^ ( MS_AUTH_RSb[ (MS_U8) ( B1 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B0 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B3 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B2       ) ]       );

    A2 = RK[2] ^ ( MS_AUTH_RSb[ (MS_U8) ( B2 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B1 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B0 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B3       ) ]       );

    A3 = RK[3] ^ ( MS_AUTH_RSb[ (MS_U8) ( B3 >> 24 ) ] << 24 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B2 >> 16 ) ] << 16 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B1 >>  8 ) ] <<  8 ) ^
                 ( MS_AUTH_RSb[ (MS_U8) ( B0       ) ]       );

    PUT_UINT32( A0, output,  0 );
    PUT_UINT32( A1, output,  4 );
    PUT_UINT32( A2, output,  8 );
    PUT_UINT32( A3, output, 12 );
}


