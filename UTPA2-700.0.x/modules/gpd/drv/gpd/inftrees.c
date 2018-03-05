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
/* inftrees.c -- generate Huffman trees for efficient decoding
 * Copyright (C) 1995-2005 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */


#include "MsCommon.h"
#include "zutil.h"
#include "inftrees.h"
#include "drvgpd.h"
//#include <semihost_io.h>
//#include "board.h"

#define MAXBITS 15

static MS_U16 mincode_tmp[MAXBITS+1];
static MS_U16 offset_tmp[MAXBITS+2];
static MS_U16 count[MAXBITS+1];    /* number of codes of each length */
static MS_U16 offs[MAXBITS+2];     /* offsets in table for each length */

static const MS_U16 lbase[31] = { /* Length codes 257..285 base */
3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
static const MS_U16 lext[31] = { /* Length codes 257..285 extra */
16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18,
19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 16, 201, 196};
static const MS_U16 dbase[32] = { /* Distance codes 0..29 base */
1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
8193, 12289, 16385, 24577, 0, 0};
static const MS_U16 dext[32] = { /* Distance codes 0..29 extra */
16, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22,
23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
28, 28, 29, 29, 64, 64};

//const char inflate_copyright[] =
//   " inflate 1.2.3 Copyright 1995-2005 Mark Adler ";

MS_U32 gpdinflate_table(type, lens, codes, table, bits, work)
codetype type;
MS_U16 FAR *lens;
MS_U32 codes;
code FAR * FAR *table;
MS_U32 FAR *bits;
MS_U16 FAR *work;
{
    MS_U32 i=0;
    //extern MS_U32 huff_reg_time;
    MS_U16 mincode=0;
    //MS_U16 mincode_tmp[288];
    //MS_U16 offset_tmp[MAXBITS+1];
    MS_U16  mincode_valid=0;
    MS_U32 len;               /* a code's length in bits */
    MS_U32 sym;               /* index of code symbols */
    MS_U32 min, max;          /* minimum and maximum code lengths */
    MS_U32 root;              /* number of index bits for root table */
    MS_U32 curr;              /* number of index bits for current table */
    MS_U32 drop;              /* code bits to drop for sub-table */
    MS_U32 left;                   /* number of prefix codes available */
    MS_U32 used;              /* code entries in table used */
    MS_U32 huff;              /* Huffman code */
    MS_U32 incr;              /* for incrementing code, index */
    MS_U32 fill;              /* index for replicating entries */
    MS_U32 low;               /* low bits for current root entry */
    MS_U32 mask;              /* mask for low root bits */
    code this;                  /* table entry for duplication */
    code FAR *next;             /* next available space in table */
    const MS_U16 FAR *base;     /* base value table to use */
    const MS_U16 FAR *extra;    /* extra bits table to use */
    MS_U32 end;                    /* use base and extra for symbol > end */
   //MS_U16 count[MAXBITS+1];    /* number of codes of each length */
   //MS_U16 offs[MAXBITS+1];     /* offsets in table for each length */

 #if 0
    static const MS_U16 lbase[31] = { /* Length codes 257..285 base */
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
    static const MS_U16 lext[31] = { /* Length codes 257..285 extra */
        16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18,
        19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 16, 201, 196};
    static const MS_U16 dbase[32] = { /* Distance codes 0..29 base */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577, 0, 0};
    static const MS_U16 dext[32] = { /* Distance codes 0..29 extra */
        16, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22,
        23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
        28, 28, 29, 29, 64, 64};
#endif

    //console_printf("creating inflate table\n");


    for(len=0;len<=MAXBITS;len++)
        count[len]=0;

    //console_printf("clear length\n");


    for (sym = 0; sym < codes; sym++)
        count[lens[sym]]++;

    //console_printf("accumulate length for codes\n");

    /* bound code lengths, force root to be within code lengths */
    root = *bits;
    for (max = MAXBITS; max >= 1; max--)
        if (count[max] != 0) break;
    if (root > max) root = max;
    if (max == 0) {                     /* no symbols to code at all */
        this.op = (MS_U8)64;    /* invalid code marker */
        this.bits = (MS_U8)1;
        this.val = (MS_U16)0;
        *(*table)++ = this;             /* make a table to force an error */
        *(*table)++ = this;
        *bits = 1;
        return 0;     /* no symbols, but wait for decoding to report error */
    }
    for (min = 1; min <= MAXBITS; min++)
        if (count[min] != 0) break;

    if (root < min) root = min;

    /* check for an over-subscribed or incomplete set of lengths */
    left = 1;
    for (len = 1; len <= MAXBITS; len++) {
        left <<= 1;
        left -= count[len];
        if (left < 0) return -1;        /* over-subscribed */
    }
    if (left > 0 && (type == CODES || max != 1))
        return -1;                      /* incomplete set */

    /* generate offsets into symbol table for each length for sorting */
    offs[1] = 0;
    offset_tmp[1]=0;
    for (len = 1; len < MAXBITS+1; len++)
        offset_tmp[len+1]=offs[len + 1] = offs[len] + count[len];

    //console_printf("generate offset\n");


    /* sort symbols by length, by symbol order within each length */
    for (sym = 0; sym < codes; sym++)
        if (lens[sym] != 0) work[offs[lens[sym]]++] = (MS_U16)sym;

    //console_printf("sort symbols\n");


   if(type==CODES)
   {
        base = extra = work;    /* dummy value--not used */
        end = 19;

        /* initialize state for loop */
        huff = 0;                   /* starting code */
        sym = 0;                    /* starting code symbol */
        len = min;                  /* starting code length */
        next = *table;              /* current table to fill in */
        curr = root;                /* current table index bits */
        drop = 0;                   /* current bits to drop from code for index */
        low = (MS_U32)(-1);       /* trigger new sub-table when len > root */
        used = 1U << root;          /* use root table entries */
        mask = used - 1;            /* mask for comparing low */

        //console_printf("initialize state for loop\n");

        /* check available table space */
         if (used >= ENOUGH - MAXD)
        return 1;

        /* process all codes and make table entries */
        for (;;) {
            /* create table entry */
            this.bits = (MS_U8)(len - drop);
            if ((int)(work[sym]) < end) {
                this.op = (MS_U8)0;
                this.val = work[sym];
            }
            else if ((int)(work[sym]) > end) {
                this.op = (MS_U8)(extra[work[sym]]);
                this.val = base[work[sym]];
            }
            else {
                this.op = (MS_U8)(32 + 64);         /* end of block */
                this.val = 0;
            }

            /* replicate for those indices with low len bits equal to huff */
            incr = 1U << (len - drop);
            fill = 1U << curr;
            min = fill;                 /* save offset to next table */
            do {
                fill -= incr;
                next[(huff >> drop) + fill] = this;
            } while (fill != 0);



            /* backwards increment the len-bit code huff */
            incr = 1U << (len - 1);
            while (huff & incr)
                incr >>= 1;
            if (incr != 0) {
                huff &= incr - 1;
                huff += incr;
            }
            else
                huff = 0;

            /* go to next symbol, update count, len */
            sym++;
            if (len <= MAXBITS)
            {
                if (--(count[len]) == 0) {
                    if (len == max) break;
                        len = lens[work[sym]];
                }
            }

            /* create new sub-table if needed */
            if (len > root && (huff & mask) != low) {

                /* if first time, transition to sub-tables */
                if (drop == 0)
                    drop = root;

                /* increment past last table */
                next += min;            /* here min is 1 << curr */

                /* determine length of next table */
                curr = len - drop;
                left = (int)(1 << curr);

                while ((curr + drop) < max)
                {
                    MS_U32 index = (curr + drop)%(MAXBITS+1);
                    if(index <= MAXBITS)   // (curr + drop) < MAXBITS is for coverity check
                    {
                        left -= count[index];
                    }
                    if (left <= 0) break;
                    curr++;
                    left <<= 1;
                }

                /* check for enough space */
                used += 1U << curr;
                if (used >= ENOUGH - MAXD)
                    return 1;

                /* point entry in root table to sub-table */
                low = huff & mask;
                (*table)[low].op = (MS_U8)curr;
                (*table)[low].bits = (MS_U8)root;
                (*table)[low].val = (MS_U16)(next - *table);
            }
        }

        this.op = (MS_U8)64;                /* invalid code marker */
        this.bits = (MS_U8)(len - drop);
        this.val = (MS_U16)0;
        while (huff != 0) {
            /* when done with sub-table, drop back to root table */
            if (drop != 0 && (huff & mask) != low) {
                drop = 0;
                len = root;
                next = *table;
                this.bits = (MS_U8)len;
            }

            /* put invalid code marker in table */
            next[huff >> drop] = this;

            /* backwards increment the len-bit code huff */
            incr = 1U << (len - 1);
            while (huff & incr)
                incr >>= 1;
            if (incr != 0) {
                huff &= incr - 1;
                huff += incr;
            }
            else
                huff = 0;
        }

        /* set return parameters */
        *table += used;
        *bits = root;
   }
   else
   {
        for (i = (int)min; i <= (int)max; i++)
        {
            mincode_tmp[i]=mincode;

            //console_printf("mincode%d:%x\n",i,mincode_tmp[i]);
            mincode=(mincode+count[i])<<1;
        }

        if (type == LENS)
        {
            /*
            //set lbase2 lbase3 lbase4
            data=offset_tmp[2]+(offset_tmp[3]<<PNG_LBASE3_SHF)+(offset_tmp[4]<<PNG_LBASE4_SHF);

            drv_gpd_set_lbase_g1(data);


            //set lbase5 lbase6 lbase7
            data=offset_tmp[5]+(offset_tmp[6]<<PNG_LBASE6_SHF)+(offset_tmp[7]<<PNG_LBASE7_SHF);
            drv_gpd_set_lbase_g2(data);


            //set lbase8 lbase9 lbase10
            data=offset_tmp[8]+(offset_tmp[9]<<PNG_LBASE9_SHF)+(offset_tmp[10]<<PNG_LBASE10_SHF);
            drv_gpd_set_lbase_g3(data);


            //set lbase11 lbase12 lbase13
            data=offset_tmp[11]+(offset_tmp[12]<<PNG_LBASE12_SHF)+(offset_tmp[13]<<PNG_LBASE13_SHF);
            drv_gpd_set_lbase_g4(data);


            //set lbase14 lbase15

            data=offset_tmp[14]+(offset_tmp[15]<<PNG_LBASE15_SHF);
            drv_gpd_set_lbase_g5(data);

            //console_printf("set lit base\n");
            */
            drv_gpd_set_lbase(offset_tmp);


            for (i=1;i<16;i++)
            {
                if (count[i]!=0)
            	    mincode_valid |= (1<<(i-1));
            }


            drv_gpd_set_lmincode_valid(mincode_valid);


            //console_printf("set lit mincode_valid\n");

            drv_gpd_set_dynamic_ldata(offset_tmp[max+1], work);


            //console_printf("set lit data\n");

            /*
            //set mincode1_lit~mincode7
            data=((mincode_tmp[1]&0x1)<<PNG_MINCODE1_SHF)+((mincode_tmp[2]&0x3)<<PNG_MINCODE2_SHF)+
              ((mincode_tmp[3]&0x7)<<PNG_MINCODE3_SHF)+((mincode_tmp[4]&0xf)<<PNG_MINCODE4_SHF)+
              ((mincode_tmp[5]&0x1f)<<PNG_MINCODE5_SHF)+((mincode_tmp[6]&0x3f)<<PNG_MINCODE6_SHF)+
              ((mincode_tmp[7]&0x7f)<<PNG_MINCODE7_SHF);

            drv_gpd_set_lmincode_g1(data);


            //set mincode8~mincode10
            data=(mincode_tmp[8]&0xff)+((mincode_tmp[9]&0x1ff)<<PNG_MINCODE9_SHF)+
            ((mincode_tmp[10]&0x3ff)<<PNG_MINCODE10_SHF);
            drv_gpd_set_lmincode_g2(data);



            //set mincode11 mincode12
            data=(mincode_tmp[11]&0x7ff)+((mincode_tmp[12]&0xfff)<<PNG_MINCODE12_SHF);
            drv_gpd_set_lmincode_g3(data);



            //set mincode13 mincode14
            data=(mincode_tmp[13]&0x1fff)+((mincode_tmp[14]&0x3fff)<<PNG_MINCODE14_SHF);
            drv_gpd_set_lmincode_g4(data);

            //set mincode15

            data=mincode_tmp[15]&0x7fff;
            drv_gpd_set_lmincode_g5(data);

            //console_printf("set lit mincode\n");
            */
            drv_gpd_set_lmincode(mincode_tmp);

        }
        else if(type==DISTS)
        {
            /*
            //set dbase2 dbase3
            //console_printf("dbase2:%x\n",offset_tmp[2]);
            //console_printf("dbase3:%x\n",offset_tmp[3]);

            data=(offset_tmp[2]<<PNG_DBASE2_SHF)+(offset_tmp[3]<<PNG_DBASE3_SHF);
            drv_gpd_set_dbase_g1(data);



            //set dbase4 ~ dbase9
            data=offset_tmp[4]+(offset_tmp[5]<<PNG_DBASE5_SHF)+(offset_tmp[6]<<PNG_DBASE6_SHF)+
            (offset_tmp[7]<<PNG_DBASE7_SHF)+(offset_tmp[8]<<PNG_DBASE8_SHF)+(offset_tmp[9]<<PNG_DBASE9_SHF);

            drv_gpd_set_dbase_g2(data);



            //set dbase10~dbase15
            data=offset_tmp[10]+(offset_tmp[11]<<PNG_DBASE11_SHF)+(offset_tmp[12]<<PNG_DBASE12_SHF)+
            (offset_tmp[13]<<PNG_DBASE13_SHF)+(offset_tmp[14]<<PNG_DBASE14_SHF)+(offset_tmp[15]<<PNG_DBASE15_SHF);

            drv_gpd_set_dbase_g3(data);

            //console_printf("set dist base\n");
            */

            drv_gpd_set_dbase(offset_tmp);

            for (i=1;i<16;i++)
            {
                if (count[i]!=0)
                    mincode_valid |= (1<<(i-1));
            }


            drv_gpd_set_dmincode_valid(mincode_valid);

            //console_printf("set dist valid\n");


            drv_gpd_set_dynamic_ddata(offset_tmp[max+1],work);



            //console_printf("set dist data\n");

            /*
            //set mincode1_dist ~ mincode5
            data=((mincode_tmp[1]&0x1)<<PNG2_MINCODE1_SHF)+
            ((mincode_tmp[2]&0x3)<<PNG2_MINCODE2_SHF)+((mincode_tmp[3]&0x7)<<PNG2_MINCODE3_SHF)+
            ((mincode_tmp[4]&0xf)<<PNG2_MINCODE4_SHF)+((mincode_tmp[5]&0x1f)<<PNG2_MINCODE5_SHF);

            drv_gpd_set_dmincode_g1(data);



            //set mincode6~mincode9

            data=(mincode_tmp[6]&0x3f)+((mincode_tmp[7]&0x7f)<<PNG2_MINCODE7_SHF)+
            ((mincode_tmp[8]&0xff)<<PNG2_MINCODE8_SHF)+((mincode_tmp[9]&0x1ff)<<PNG2_MINCODE9_SHF);

            drv_gpd_set_dmincode_g2(data);

            //set mincode10~mincode11
            data=(mincode_tmp[10]&0x3ff)+((mincode_tmp[11]&0x7ff)<<PNG2_MINCODE11_SHF);
            drv_gpd_set_dmincode_g3(data);



            //set mincode12~13
            data=(mincode_tmp[12]&0xfff)+((mincode_tmp[13]&0x1fff)<<PNG2_MINCODE13_SHF);
            drv_gpd_set_dmincode_g4(data);



            //set mincode14~15
            data=(mincode_tmp[14]&0x3fff)+((mincode_tmp[15]&0x7fff)<<PNG2_MINCODE15_SHF);
            //set_nop_cmd(30);
            drv_gpd_set_dmincode_g5(data);

            //console_printf("set dist mincode\n");
            */
            drv_gpd_set_dmincode(mincode_tmp);
        }
    }

    return 0;
}
