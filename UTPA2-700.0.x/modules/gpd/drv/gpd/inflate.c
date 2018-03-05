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
/* inflate.c -- zlib decompression
 * Copyright (C) 1995-2005 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/*
 * Change history:
 *
 * 1.2.beta0    24 Nov 2002
 * - First version -- complete rewrite of inflate to simplify code, avoid
 *   creation of window when not needed, minimize use of window when it is
 *   needed, make inffast.c even faster, implement gzip decoding, and to
 *   improve code readability and style over the previous zlib inflate code
 *
 * 1.2.beta1    25 Nov 2002
 * - Use pointers for available input and output checking in inffast.c
 * - Remove input and output counters in inffast.c
 * - Change inffast.c entry and loop from avail_in >= 7 to >= 6
 * - Remove unnecessary second byte pull from length extra in inffast.c
 * - Unroll direct copy to three copies per loop in inffast.c
 *
 * 1.2.beta2    4 Dec 2002
 * - Change external routine names to reduce potential conflicts
 * - Correct filename to inffixed.h for fixed tables in inflate.c
 * - Make hbuf[] MS_U8 to match parameter type in inflate.c
 * - Change strm->next_out[-state->offset] to *(strm->next_out - state->offset)
 *   to avoid negation problem on Alphas (64 bit) in inflate.c
 *
 * 1.2.beta3    22 Dec 2002
 * - Add comments on state->bits assertion in inffast.c
 * - Add comments on op field in inftrees.h
 * - Fix bug in reuse of allocated window after inflateReset()
 * - Remove bit fields--back to byte structure for speed
 * - Remove distance extra == 0 check in inflate_fast()--only helps for lengths
 * - Change post-increments to pre-increments in inflate_fast(), PPC biased?
 * - Add compile time option, POSTINC, to use post-increments instead (Intel?)
 * - Make MATCH copy in inflate() much faster for when inflate_fast() not used
 * - Use local copies of stream next and avail values, as well as local bit
 *   buffer and bit count in inflate()--for speed when inflate_fast() not used
 *
 * 1.2.beta4    1 Jan 2003
 * - Split ptr - 257 statements in inflate_table() to avoid compiler warnings
 * - Move a comment on output buffer sizes from inffast.c to inflate.c
 * - Add comments in inffast.c to introduce the inflate_fast() routine
 * - Rearrange window copies in inflate_fast() for speed and simplification
 * - Unroll last copy for window match in inflate_fast()
 * - Use local copies of window variables in inflate_fast() for speed
 * - Pull out common write == 0 case for speed in inflate_fast()
 * - Make op and len in inflate_fast() unsigned for consistency
 * - Add FAR to lcode and dcode declarations in inflate_fast()
 * - Simplified bad distance check in inflate_fast()
 * - Added inflateBackInit(), inflateBack(), and inflateBackEnd() in new
 *   source file infback.c to provide a call-back interface to inflate for
 *   programs like gzip and unzip -- uses window as output buffer to avoid
 *   window copying
 *
 * 1.2.beta5    1 Jan 2003
 * - Improved inflateBack() interface to allow the caller to provide initial
 *   input in strm.
 * - Fixed stored blocks bug in inflateBack()
 *
 * 1.2.beta6    4 Jan 2003
 * - Added comments in inffast.c on effectiveness of POSTINC
 * - Typecasting all around to reduce compiler warnings
 * - Changed loops from while (1) or do {} while (1) to for (;;), again to
 *   make compilers happy
 * - Changed type of window in inflateBackInit() to MS_U8 *
 *
 * 1.2.beta7    27 Jan 2003
 * - Changed many types to unsigned or MS_U16 to avoid warnings
 * - Added inflateCopy() function
 *
 * 1.2.0        9 Mar 2003
 * - Changed inflateBack() interface to provide separate opaque descriptors
 *   for the in() and out() functions
 * - Changed inflateBack() argument and in_func typedef to swap the length
 *   and buffer address return values for the input function
 * - Check next_in and next_out for Z_NULL on entry to inflate()
 *
 * The history for versions after 1.2.0 are in ChangeLog in zlib distribution.
 */

#include "MsCommon.h"
#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"
#include "gpd_reg.h"
#include "drvgpd.h"


#ifdef MAKEFIXED
#  ifndef BUILDFIXED
#    define BUILDFIXED
#  endif
#endif


extern MS_U16 read_fifo(MS_U32 n);
extern MS_U16 show_fifo(MS_U32 n);

struct inflate_state inflatestate;



/* function prototypes */
void gpdfixedtables OF((struct inflate_state FAR *state));

#ifdef BUILDFIXED
   void makefixed OF((void));
#endif

#if 0
MS_U32 ZEXPORT gpdinflateReset(strm)
z_streamp strm;
{
    struct inflate_state FAR *state;

    state = (struct inflate_state FAR *)strm->state;
    state->mode = HEAD;
    state->last = 0;
    state->lencode = state->distcode = state->next = state->codes;

    return Z_OK;
}
#endif

MS_U32 ZEXPORT gpdinflateReset(z_streamp strm)
{
    struct inflate_state FAR *state;

    state = (struct inflate_state FAR *)strm->state;
    state->mode = HEAD;
    state->last = 0;
    state->lencode = state->distcode = state->next = state->codes;

    return Z_OK;
}



MS_U32 ZEXPORT gpdinflateInit2_(strm, windowBits, version, stream_size)
z_streamp strm;
MS_U32 windowBits;
const char *version;
MS_U32 stream_size;
{
    struct inflate_state FAR *state;

    state = (struct inflate_state FAR *)&inflatestate;
    strm->state = (struct internal_state FAR *)state;

    return gpdinflateReset(strm);
}

MS_U32 ZEXPORT gpdinflateInit_(strm, version, stream_size)
z_streamp strm;
const char *version;
MS_U32 stream_size;
{
    return gpdinflateInit2_(strm, DEF_WBITS, version, stream_size);
}

/*
   Return state with length and distance decoding tables and index sizes set to
   fixed code decoding.  Normally this returns fixed tables from inffixed.h.
   If BUILDFIXED is defined, then instead this routine builds the tables the
   first time it's called, and returns those tables the first time and
   thereafter.  This reduces the size of the code by about 2K bytes, in
   exchange for a little execution time.  However, BUILDFIXED should not be
   used for threaded applications, since the rewriting of the tables and virgin
   may not be thread-safe.
 */
void gpdfixedtables(state)
struct inflate_state FAR *state;
{
    MS_U32 data;
    MS_U16 tmp_data[16];

    /*
    drv_gpd_set_lbase_g1(0);

    //set lbase7

    drv_gpd_set_lbase_g2(0);


    //set lbase8  lbase9  lbase10

    data=0x18+(0xb0<<PNG_LBASE9_SHF)+(0x120<<PNG_LBASE10_SHF);


    drv_gpd_set_lbase_g3(data);

    //console_printf("set lbase");
    */

    memset(tmp_data, 0, sizeof(tmp_data));
    tmp_data[8]  = 0x18;
    tmp_data[9]  = 0xb0;
    tmp_data[10] = 0x120;
    drv_gpd_set_lbase(tmp_data);


    //set lit_mincode_valid or dist_mincode_valid


    data=0x1c0;
    drv_gpd_set_lmincode_valid(data);



    //console_printf("set lit_mincode_valid or dist_mincode_valid");


    drv_gpd_set_fixed_ldata();

    //console_printf("set ltbl_data");


    //set lit_mincode

    /*
    //set mincode7


    drv_gpd_set_lmincode_g1(0);

    //set mincode8  mincode9

    data=0x30+(0x190<<PNG_MINCODE9_SHF);

    drv_gpd_set_lmincode_g2(data);
    */

    memset(tmp_data, 0, sizeof(tmp_data));
    tmp_data[8]  = 0x30;
    tmp_data[9]  = 0x190;
    drv_gpd_set_lmincode(tmp_data);

    //console_printf("set lit_mincode ");

    set_nop_cmd(10);

    /*
    drv_gpd_set_dmincode_g5(0);
    */

    memset(tmp_data, 0, sizeof(tmp_data));
    drv_gpd_set_dmincode(tmp_data);

}

#ifdef MAKEFIXED
#include <stdio.h>

/*
   Write out the inffixed.h that is #include'd above.  Defining MAKEFIXED also
   defines BUILDFIXED, so the tables are built on the fly.  makefixed() writes
   those tables to stdout, which would be piped to inffixed.h.  A small program
   can simply call makefixed to do this:

    void makefixed(void);

    MS_U32 main(void)
    {
        makefixed();
        return 0;
    }

   Then that can be linked with zlib built with MAKEFIXED defined and run:

    a.out > inffixed.h
 */
void makefixed()
{
    MS_U32 low, size;
    struct inflate_state state;

    fixedtables(&state);
    puts("    /* inffixed.h -- table for decoding fixed codes");
    puts("     * Generated automatically by makefixed().");
    puts("     */");
    puts("");
    puts("    /* WARNING: this file should *not* be used by applications.");
    puts("       It is part of the implementation of this library and is");
    puts("       subject to change. Applications should only use zlib.h.");
    puts("     */");
    puts("");
    size = 1U << 9;
    printf("    static const code lenfix[%u] = {", size);
    low = 0;
    for (;;) {
        if ((low % 7) == 0) printf("\n        ");
        printf("{%u,%u,%d}", state.lencode[low].op, state.lencode[low].bits,
               state.lencode[low].val);
        if (++low == size) break;
        putchar(',');
    }
    puts("\n    };");
    size = 1U << 5;
    printf("\n    static const code distfix[%u] = {", size);
    low = 0;
    for (;;) {
        if ((low % 6) == 0) printf("\n        ");
        printf("{%u,%u,%d}", state.distcode[low].op, state.distcode[low].bits,
               state.distcode[low].val);
        if (++low == size) break;
        putchar(',');
    }
    puts("\n    };");
}
#endif /* MAKEFIXED */

/*
   Update the window with the last wsize (normally 32K) bytes written before
   returning.  If window does not exist yet, create it.  This is only called
   when a window is already in use, or when output has been written during this
   inflate call, but the end of the deflate stream has not been reached yet.
   It is also called to create a window for dictionary data when a dictionary
   is loaded.

   Providing output buffers larger than 32K to inflate() should provide a speed
   advantage, since only the last 32K of output is copied to the sliding window
   upon return from inflate(), and since all distances after the first 32K of
   output will fall in the output data, making match copies simpler and faster.
   The advantage may be dependent on the size of the processor's data caches.
 */




/* Reverse the bytes in a 32-bit value */
MS_U32 num = 0, blk_typ;
static MS_U8 prev,curr;
extern MS_U32 nocompress, fix, dynamic;

MS_U32 ZEXPORT gpdinflate(strm, flag)
z_streamp strm;
MS_U32 flag;
{
    MS_U32 i;
    //extern MS_U32 huff_reg_time;
    MS_U32 ncodebits;
    MS_U32 ncodetmp;
    //MS_U32 test1, test2;
    MS_U32 dbport, bitoffset = 0, tmpxx;
    //MS_U32 hold;
    //MS_U32 bits;
    MS_U8 readdata1/*, readdata2*/;
    MS_U16 readtmp;
    MS_U16 readdata3;
    struct inflate_state FAR *state;
    MS_U32 copy;
    code this;                  /* current decoding table entry */
    //code last;                  /* parent table entry */
    MS_U32 len;               /* length to copy for repeats, bits to drop */
    MS_U32 ret;                    /* return code */
#ifdef GUNZIP
    //MS_U8 hbuf[4];      /* buffer for gzip header crc calculation */
#endif
    static const MS_U16 order[20] = /* permutation of code lengths */
        {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15, 0};

    state = (struct inflate_state FAR *)strm->state;
    if (flag == 0)
    	state->mode = TYPEDO;
    ret = Z_OK;

    for (;;)
    {
        switch (state->mode)
        {
        case HEAD:
            read_fifo(16);
            state->mode = TYPEDO;
            break;

        case TYPEDO:
            readtmp = read_fifo(3);
            state->last = readtmp & 0x01;
            readdata1 = (readtmp & 0x06) >> 1;
            num++;
            blk_typ = readdata1;

            switch(readdata1)
            {
            case 0:
                dbport = drv_gpd_bitpos();
                dbport &= (0x07);
                if (dbport)
                {
                    bitoffset = 8 - dbport;
                    dbport = read_fifo(bitoffset);
                }
                else
                {
                    bitoffset = 0;
                }

                curr = 0;
                state->mode = DONE;
                //set compression_type
                drv_gpd_set_cmp_type(curr);
                prev = curr;
                break;

            case 1:
                curr = 2;
                if(curr != prev)
                    gpdfixedtables(state);
                state->mode = DONE;
                //set compression_type
                drv_gpd_set_cmp_type(curr);
                prev = curr;
                break;

            case 2:
                curr = 3;
                state->mode = TABLE;
                drv_gpd_set_cmp_type(curr);
                prev = curr;
                break;

            case 3:
                //console_printf("invalid block type\n");
                state->mode = BAD;
            }
            break;

        case TABLE:
            readtmp = read_fifo(14);
            state->nlen = (readtmp&(0x001f))+257;
            state->ndist = ((readtmp&(0x03e0))>>5)+1;
            state->ncode = ((readtmp&(0x3c00))>>10)+4;
#ifndef PKZIP_BUG_WORKAROUND
            if (state->nlen > 286 || state->ndist > 30)
            {
                state->mode = BAD;
                break;
            }
#endif
            state->have = 0;
            state->mode = LENLENS;

        case LENLENS:
            ncodebits = 3 * state->ncode;
            while(ncodebits>=15)
            {
                readtmp=read_fifo(15);

                for(i = 0; i < 5; i++)
                {
                    if(state->have < 19)
                    {
                        state->lens[order[state->have++]] = ((readtmp&((0x0007)<<(i*3)))>>(i*3));
                    }
                }
                ncodebits-=15;
            }
            if(ncodebits)
            {
                readtmp=read_fifo(ncodebits);
                ncodetmp = state->ncode%5;
                for(i=0;i<ncodetmp;i++)
                {
                    if(state->have < 19)
                    {
                        state->lens[order[state->have++]]=((readtmp&((0x0007)<<(i*3)))>>(i*3));
                    }
                }
            }

            while (state->have < 19)
                state->lens[order[state->have++]] = 0;

            state->next = state->codes;
            state->lencode = (code const FAR *)(state->next);
            state->lenbits = 7;
            ret = gpdinflate_table(CODES, state->lens, 19, &(state->next),
            &(state->lenbits), state->work);

            if (ret)
            {
                //console_printf("invalid code length set\n");
                state->mode = BAD;
                break;
            }

            state->have = 0;
            state->mode = CODELENS;

        case CODELENS:
            while (state->have < state->nlen + state->ndist)
            {
                readdata3 = show_fifo(state->lenbits);
                this = state->lencode[readdata3];

                if (this.val < 16)
                {
                    read_fifo(this.bits);
                    state->lens[state->have++] = this.val;
                }
                else
                {
                    if (this.val == 16)
                    {
                        if((this.bits+2)>16)
                        {
                            read_fifo(this.bits);
                            copy=3+read_fifo(2);
                        }
                        else
                        {
                            tmpxx = this.bits+2;
                            readtmp=read_fifo(tmpxx);
                            copy=3+((readtmp>>this.bits)&0x0003);
                        }

                        if (state->have == 0)
                        {
                            //console_printf("invalid bit length repeat\n");
                            state->mode = BAD;
                            break;
                        }
                        len = state->lens[state->have - 1];
                    }

                    else if (this.val == 17)
                    {
                        if((this.bits+3)>16)
                        {
                            read_fifo(this.bits);
                            copy=3+read_fifo(3);
                        }
                        else
                        {
                            tmpxx= this.bits+3;
                            readtmp=read_fifo(tmpxx);
                            copy=3+((readtmp>>this.bits)&0x0007);
                        }
                        len=0;
                    }

                    else
                    {
                        if((this.bits+7)>16)
                        {
                            read_fifo(this.bits);
                            copy=11+read_fifo(7);
                        }
                        else
                        {
                            tmpxx = this.bits+7;
                            readtmp=read_fifo(tmpxx);
                            copy=11+((readtmp>>this.bits)&0x007f);
                        }
                        len=0;
                    }

                    if (state->have + copy > state->nlen + state->ndist)
                    {

                        //console_printf("invalid bit length repeat\n");
                        state->mode = BAD;
                        break;
                    }
                    while (copy--)
                    {
                        state->lens[state->have++] = (MS_U16)len;

                    }
                }
            }


            /* handle error breaks in while */
            if (state->mode == BAD) break;

            /* build code tables */
            state->next = state->codes;
            state->lencode = (code const FAR *)(state->next);
            state->lenbits = 9;
            ret = gpdinflate_table(LENS, state->lens, state->nlen, &(state->next),
            &(state->lenbits), state->work);
            if (ret)
            {
                //console_printf("invalid literal/length set\n");
                state->mode = BAD;
                break;
            }
            state->distcode = (code const FAR *)(state->next);
            state->distbits = 6;
            ret = gpdinflate_table(DISTS, state->lens + state->nlen, state->ndist,
            &(state->next), &(state->distbits), state->work);
            if (ret)
            {

                //console_printf("invalid distance set\n");
                state->mode = BAD;
                break;
            }

            state->mode = DONE;


        case DONE:
            if (state->last) {

                prev=255;
                return Z_STREAM_END;
            }
            else
                return Z_OK;

        case BAD:
            return Z_STREAM_ERROR;

        default:
            return Z_STREAM_ERROR;
        }
    }
    /*
       Return from inflate(), updating the total counts and the check value.
       If there was no progress during the inflate() call, return a buffer
       error.  Call updatewindow() to create and/or update the window state.
       Note: a memory error from inflate() is non-recoverable.
     */
}

