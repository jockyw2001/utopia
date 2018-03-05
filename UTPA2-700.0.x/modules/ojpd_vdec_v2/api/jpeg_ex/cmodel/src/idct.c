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
//
// 2D IDCT
// Derived from an older version of the IJG's JPEG software.
// Downloadable from: www.ijg.org
// This module is going to be replaced with a faster (and
// uncopyrighted) version.
// I am unable to find the original file from which this code was derived.
// I have included the copyright notice included with latest IJG version of this
// module.
//

/*
 * jidctint.c
 *
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a slow-but-accurate integer implementation of the
 * inverse DCT (Discrete Cosine Transform).  In the IJG code, this routine
 * must also perform dequantization of the input coefficients.
 *
 * A 2-D IDCT can be done by 1-D IDCT on each column followed by 1-D IDCT
 * on each row (or vice versa, but it's more convenient to emit a row at
 * a time).  Direct algorithms are also available, but they are much more
 * complex and seem not to be any faster when reduced to code.
 *
 * This implementation is based on an algorithm described in
 *   C. Loeffler, A. Ligtenberg and G. Moschytz, "Practical Fast 1-D DCT
 *   Algorithms with 11 Multiplications", Proc. Int'l. Conf. on Acoustics,
 *   Speech, and Signal Processing 1989 (ICASSP '89), pp. 988-991.
 * The primary algorithm described there uses 11 multiplies and 29 adds.
 * We use their alternate method with 12 multiplies and 32 adds.
 * The advantage of this method is that no data path contains more than one
 * multiplication; this allows a very simple and accurate implementation in
 * scaled fixed-point arithmetic, with a minimal number of shifts.
 */

/*----------------------------------------------------------------------------*/
#include "jpegmain.h"
#include "apiJPEG.h"
/*----------------------------------------------------------------------------*/
#define CONST_BITS  13
#define PASS1_BITS  2
#define SCALEDONE ((S32) 1)
#define CONST_SCALE (SCALEDONE << CONST_BITS)
#define FIX(x)  ((S32) ((x) * CONST_SCALE + 0.5))
/*----------------------------------------------------------------------------*/
#define FIX_0_298631336  ((S32)  2446)        /* FIX(0.298631336) */
#define FIX_0_390180644  ((S32)  3196)        /* FIX(0.390180644) */
#define FIX_0_541196100  ((S32)  4433)        /* FIX(0.541196100) */
#define FIX_0_765366865  ((S32)  6270)        /* FIX(0.765366865) */
#define FIX_0_899976223  ((S32)  7373)        /* FIX(0.899976223) */
#define FIX_1_175875602  ((S32)  9633)        /* FIX(1.175875602) */
#define FIX_1_501321110  ((S32)  12299)       /* FIX(1.501321110) */
#define FIX_1_847759065  ((S32)  15137)       /* FIX(1.847759065) */
#define FIX_1_961570560  ((S32)  16069)       /* FIX(1.961570560) */
#define FIX_2_053119869  ((S32)  16819)       /* FIX(2.053119869) */
#define FIX_2_562915447  ((S32)  20995)       /* FIX(2.562915447) */
#define FIX_3_072711026  ((S32)  25172)       /* FIX(3.072711026) */
/*----------------------------------------------------------------------------*/
#define DESCALE(x,n)  (((x) + (SCALEDONE << ((n)-1))) >> (n))
/*----------------------------------------------------------------------------*/
#define MULTIPLY(var,cnst)  ((var) * (cnst))
#define clamp(i) if (i & 0xFF00) i = (((~i) >> 15) & 0xFF);

/*----------------------------------------------------------------------------*/
void idct( JPEG_BLOCK_TYPE *data, U8 *Pdst_ptr )
{
    S32 tmp0, tmp1, tmp2, tmp3;
    S32 tmp10, tmp11, tmp12, tmp13;
    S32 z1, z2, z3, z4, z5;
    register JPEG_BLOCK_TYPE *dataptr;
    int rowctr;

    //kevinhuang, use an internal array in idct to avoid memcpy to save time
    JPEG_BLOCK_TYPE workspace[64];
    JPEG_BLOCK_TYPE *wsptr;
//printf("idct::idct\n");
    dataptr = data;
    wsptr = workspace;
    for ( rowctr = 8 - 1; rowctr >= 0; rowctr-- )
    {
        if ( ( dataptr[1] | dataptr[2] | dataptr[3] | dataptr[4] | dataptr[5] | dataptr[6] | dataptr[7] ) == 0 )
        {
            S16 dcval = ( S16 )( dataptr[0] << PASS1_BITS );

            wsptr[0] = dcval;
            wsptr[1] = dcval;
            wsptr[2] = dcval;
            wsptr[3] = dcval;
            wsptr[4] = dcval;
            wsptr[5] = dcval;
            wsptr[6] = dcval;
            wsptr[7] = dcval;

            dataptr += 8;       /* advance pointer to next row */
            wsptr += 8;
            continue;
        }

        z2 = ( S32 )dataptr[2];
        z3 = ( S32 )dataptr[6];

        z1 = MULTIPLY( z2 + z3, FIX_0_541196100 );
        tmp2 = z1 + MULTIPLY( z3, -FIX_1_847759065 );
        tmp3 = z1 + MULTIPLY( z2, FIX_0_765366865 );

        tmp0 = ( ( S32 )dataptr[0] + ( S32 )dataptr[4] ) << CONST_BITS;
        tmp1 = ( ( S32 )dataptr[0] - ( S32 )dataptr[4] ) << CONST_BITS;

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        tmp0 = ( S32 )dataptr[7];
        tmp1 = ( S32 )dataptr[5];
        tmp2 = ( S32 )dataptr[3];
        tmp3 = ( S32 )dataptr[1];

        z1 = tmp0 + tmp3;
        z2 = tmp1 + tmp2;
        z3 = tmp0 + tmp2;
        z4 = tmp1 + tmp3;
        z5 = MULTIPLY( z3 + z4, FIX_1_175875602 );

        tmp0 = MULTIPLY( tmp0, FIX_0_298631336 );
        tmp1 = MULTIPLY( tmp1, FIX_2_053119869 );
        tmp2 = MULTIPLY( tmp2, FIX_3_072711026 );
        tmp3 = MULTIPLY( tmp3, FIX_1_501321110 );
        z1 = MULTIPLY( z1, -FIX_0_899976223 );
        z2 = MULTIPLY( z2, -FIX_2_562915447 );
        z3 = MULTIPLY( z3, -FIX_1_961570560 );
        z4 = MULTIPLY( z4, -FIX_0_390180644 );

        z3 += z5;
        z4 += z5;

        tmp0 += z1 + z3;
        tmp1 += z2 + z4;
        tmp2 += z2 + z3;
        tmp3 += z1 + z4;

        wsptr[0] = ( S16 )DESCALE( tmp10 + tmp3, CONST_BITS - PASS1_BITS );
        wsptr[7] = ( S16 )DESCALE( tmp10 - tmp3, CONST_BITS - PASS1_BITS );
        wsptr[1] = ( S16 )DESCALE( tmp11 + tmp2, CONST_BITS - PASS1_BITS );
        wsptr[6] = ( S16 )DESCALE( tmp11 - tmp2, CONST_BITS - PASS1_BITS );
        wsptr[2] = ( S16 )DESCALE( tmp12 + tmp1, CONST_BITS - PASS1_BITS );
        wsptr[5] = ( S16 )DESCALE( tmp12 - tmp1, CONST_BITS - PASS1_BITS );
        wsptr[3] = ( S16 )DESCALE( tmp13 + tmp0, CONST_BITS - PASS1_BITS );
        wsptr[4] = ( S16 )DESCALE( tmp13 - tmp0, CONST_BITS - PASS1_BITS );

        dataptr += 8;
        wsptr += 8;
    }

    dataptr = workspace;
    for ( rowctr = 8 - 1; rowctr >= 0; rowctr-- )
    {
        S16 i;

        if ( ( dataptr[8 * 1] | dataptr[8 * 2] | dataptr[8 * 3] | dataptr[8 * 4] | dataptr[8 * 5] | dataptr[8 * 6] | dataptr[8 * 7] ) == 0 )
        {
            S16 dcval = ( S16 )DESCALE( ( S32 )dataptr[0], PASS1_BITS + 3 ) + 128;
            /*
            if ((dcval) < 0)
              dcval = 0;
            else if (dcval > 255)
              dcval = 255;
            */
            clamp( dcval );

            Pdst_ptr[8 * 0] = ( U8 )dcval;
            Pdst_ptr[8 * 1] = ( U8 )dcval;
            Pdst_ptr[8 * 2] = ( U8 )dcval;
            Pdst_ptr[8 * 3] = ( U8 )dcval;
            Pdst_ptr[8 * 4] = ( U8 )dcval;
            Pdst_ptr[8 * 5] = ( U8 )dcval;
            Pdst_ptr[8 * 6] = ( U8 )dcval;
            Pdst_ptr[8 * 7] = ( U8 )dcval;

            dataptr++;
            Pdst_ptr++;
            continue;
        }

        z2 = ( S32 )dataptr[8 * 2];
        z3 = ( S32 )dataptr[8 * 6];

        z1 = MULTIPLY( z2 + z3, FIX_0_541196100 );
        tmp2 = z1 + MULTIPLY( z3, -FIX_1_847759065 );
        tmp3 = z1 + MULTIPLY( z2, FIX_0_765366865 );

        tmp0 = ( ( S32 )dataptr[8 * 0] + ( S32 )dataptr[8 * 4] ) << CONST_BITS;
        tmp1 = ( ( S32 )dataptr[8 * 0] - ( S32 )dataptr[8 * 4] ) << CONST_BITS;

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        tmp0 = ( S32 )dataptr[8 * 7];
        tmp1 = ( S32 )dataptr[8 * 5];
        tmp2 = ( S32 )dataptr[8 * 3];
        tmp3 = ( S32 )dataptr[8 * 1];

        z1 = tmp0 + tmp3;
        z2 = tmp1 + tmp2;
        z3 = tmp0 + tmp2;
        z4 = tmp1 + tmp3;
        z5 = MULTIPLY( z3 + z4, FIX_1_175875602 );

        tmp0 = MULTIPLY( tmp0, FIX_0_298631336 );
        tmp1 = MULTIPLY( tmp1, FIX_2_053119869 );
        tmp2 = MULTIPLY( tmp2, FIX_3_072711026 );
        tmp3 = MULTIPLY( tmp3, FIX_1_501321110 );
        z1 = MULTIPLY( z1, -FIX_0_899976223 );
        z2 = MULTIPLY( z2, -FIX_2_562915447 );
        z3 = MULTIPLY( z3, -FIX_1_961570560 );
        z4 = MULTIPLY( z4, -FIX_0_390180644 );

        z3 += z5;
        z4 += z5;

        tmp0 += z1 + z3;
        tmp1 += z2 + z4;
        tmp2 += z2 + z3;
        tmp3 += z1 + z4;

        i = ( S16 )DESCALE( tmp10 + tmp3, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 0] = ( U8 )i;

        i = ( S16 )DESCALE( tmp10 - tmp3, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 7] = ( U8 )i;

        i = ( S16 )DESCALE( tmp11 + tmp2, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 1] = ( U8 )i;

        i = ( S16 )DESCALE( tmp11 - tmp2, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 6] = ( U8 )i;

        i = ( S16 )DESCALE( tmp12 + tmp1, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 2] = ( U8 )i;

        i = ( S16 )DESCALE( tmp12 - tmp1, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 5] = ( U8 )i;

        i = ( S16 )DESCALE( tmp13 + tmp0, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 3] = ( U8 )i;

        i = ( S16 )DESCALE( tmp13 - tmp0, CONST_BITS + PASS1_BITS + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 4] = ( U8 )i;

        dataptr++;
        Pdst_ptr++;
    }
}
/*----------------------------------------------------------------------------*/

