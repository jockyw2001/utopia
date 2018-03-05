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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    jpeg_cmodel.c
/// @brief  JPEG CMODEL
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "jpegmain.h"
#include "jpeg_cmodel.h"
#include "apiJPEG.h"
#include <setjmp.h>

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SCALEBITS 16
#define ONE_HALF ((long) 1 << (SCALEBITS-1))
#define FIX(x) ((long) ((x) * (1L<<SCALEBITS) + 0.5))

#define HUFF_EXTEND(x,s)    ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
// Coefficients are stored in this sequence in the data stream.
const MS_U8 _u8ZAG[64] =
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};
// This table is only used in JPD
const MS_U8 _u8Jpeg_zigzag_order[64] =
{
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

extern const MS_S32 extend_test[16];
extern const MS_S32 extend_offset[16];

extern MS_U16 _u16Image_x_size;
extern JPEG_HuffInfo _Huff_info[JPEG_MAXHUFFTABLES];

extern JPEG_QuantTbl _QuantTables[JPEG_MAXQUANTTABLES];

extern MS_U8 _u8Comps_in_frame;
extern MS_U8 _u8Comp_quant[JPEG_MAXCOMPONENTS];
extern MS_U8 _u8Comp_ident[JPEG_MAXCOMPONENTS];
extern MS_U8 _u8LumaCi;
extern MS_U8 _u8ChromaCi;
extern MS_U8 _u8Comp_dc_tab[JPEG_MAXCOMPONENTS];
extern MS_U8 _u8Comp_ac_tab[JPEG_MAXCOMPONENTS];
extern MS_U8 _u8Blocks_per_mcu;
extern MS_U32 _u32Max_blocks_per_row;
extern MS_U16 _u16Mcus_per_row;

extern MS_U8 _u8Mcu_org[JPEG_MAXBLOCKSPERMCU];
extern MS_U8 gu8Max_mcu_y_size;         /* MCU's max. Y size in pixels */
extern MS_U16 gu16Max_mcus_per_row;

extern MS_U16 _u16Total_lines_left;               /* total # lines left in image */
extern JPEG_HuffTbl _Huff_tbls[JPEG_MAXHUFFTABLES];
extern MS_U32 _u32Last_dc_val[JPEG_MAXCOMPONENTS];
extern MS_U16 gu16Mcu_lines_left;       /* total # lines left in this MCU */

extern MS_U8 gu8Scan_type;

extern MS_U32 _u32In_buf_left;

extern MS_S16 _s16Bits_left;

extern MS_U16 _u16Restart_interval;

extern MS_U16 _u16Restarts_left;

extern JPEG_ErrCode _Error_code;
extern MS_BOOL _bReady_flag;

extern jmp_buf _jmp_state;

extern MS_U32 _u32Total_bytes_read;


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
extern void JPEG_load_next_row( void );
extern void JPEG_process_restart( void );
extern MS_U32 JPEG_get_bits_2( MS_U8 numbits );
extern MS_S32 JPEG_huff_decode(JPEG_HuffTbl *Ph);
extern void JPEG_terminate( MS_U16 status );
extern MS_U32 JPEG_process_markers( void );
extern void idct( JPEG_BLOCK_TYPE *data, MS_U8 *Pdst_ptr );
extern void msAPI_JPEG_H2V2Convert( void );

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
void JPEG_CMODEL_DumpTables(void)
{
    // Symbol tables
    {
        MS_U16 i, tbl_num_luma, tbl_num_chroma;
        MS_U8 ci, luma_ci = 0, chroma_ci = 0;

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];

        JPEG_CMODEL_DEBUG("Symidx\n");
        JPEG_CMODEL_DEBUG("%d %d\n", tbl_num_luma, tbl_num_chroma);
        JPEG_CMODEL_DEBUG("Write symidx : AC\n");
        JPEG_CMODEL_DEBUG("====\n");
        JPEG_CMODEL_DEBUG("   // Symidx Addr\n");
        JPEG_CMODEL_DEBUG("wriu 0x101780 0x00\n");
        JPEG_CMODEL_DEBUG("wriu 0x101781 0x02\n");
#if 0
        for ( i = 0; i < 256; i++ )
        {
            JPEG_DEBUG("%04x\n", ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ));
        }
#else
        for ( i = 0; i < 256; i++ )
        {
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%x\n", (_Huff_info[tbl_num_luma].u8Huff_val[i]));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%x\n", (_Huff_info[tbl_num_chroma].u8Huff_val[i]));
        }
#endif

        tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];

        JPEG_CMODEL_DEBUG("%d %d\n", tbl_num_luma, tbl_num_chroma);
        JPEG_CMODEL_DEBUG("Write symidx : DC\n");
#if 0
        for ( i = 0; i < 16; i++ )
        {
            JPEG_CMODEL_DEBUG("%04x\n", ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ));
        }
#else
        for ( i = 0; i < 16; i++ )
        {
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%x\n", (_Huff_info[tbl_num_luma].u8Huff_val[i]));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%x\n", (_Huff_info[tbl_num_chroma].u8Huff_val[i]));
        }
#endif
    }

    // Q tables
    {
        MS_U8 i, j;
        MS_U8 com_num = 0;
        MS_U8 comp[JPEG_MAXCOMPONENTS];

        // Calculate how many valid quantization tables
        memset(comp, 0, JPEG_MAXCOMPONENTS);
        for(i = 0; i<_u8Comps_in_frame; i++)
        {
            comp[_u8Comp_quant[i]] = 1;
        }

        for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
        {
            if(comp[i]==1)
                com_num++;
        }

        JPEG_CMODEL_DEBUG("Q Table\n");
        JPEG_CMODEL_DEBUG("Component: %d\n", com_num);

        JPEG_CMODEL_DEBUG("   // IQtbl\n");
        JPEG_CMODEL_DEBUG("====\n");

        JPEG_CMODEL_DEBUG("wriu 0x101780 0x00\n");
        JPEG_CMODEL_DEBUG("wriu 0x101781 0x04\n");

        for ( i = 0; i < com_num; i++ )
        {
            JPEG_CMODEL_DEBUG("====\n");
            JPEG_CMODEL_DEBUG("\n");
#if 0
            for(j = 0; j<64; j++)
            {
                JPEG_CMODEL_DEBUG("%04x\n", _QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
            }
#else
            for(j = 0; j<64; j++)
            {
                JPEG_CMODEL_DEBUG("wriu 0x101782 0x%x\n", (_QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]] & 0x00FF));
                JPEG_CMODEL_DEBUG("wriu 0x101783 0x%x\n", ((_QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]] >> 8) & 0x00FF));
            }
#endif
        }
    }

    // SC tables
    {
        MS_U32 reg_value;
        MS_U16 i, ci, valid, tbl_num_luma, tbl_num_chroma;
        MS_U8 luma_ci = 0, chroma_ci = 0;

        JPEG_CMODEL_DEBUG("group table\n");
        JPEG_CMODEL_DEBUG("   // Grpinf Addr\n");
        JPEG_CMODEL_DEBUG("====\n");

        JPEG_CMODEL_DEBUG("wriu 0x101780 0x00\n");
        JPEG_CMODEL_DEBUG("wriu 0x101781 0x00\n");

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }
#if 0
            JPEG_CMODEL_DEBUG("%08lx\n", reg_value);
#else
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", (reg_value & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 8) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", ((reg_value >> 16) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 24) & 0x000000FF));
#endif
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }
#if 0
            JPEG_CMODEL_DEBUG("%08lx\n", reg_value);
#else
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", (reg_value & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 8) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", ((reg_value >> 16) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 24) & 0x000000FF));
#endif
        }

        tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }
#if 0
            JPEG_CMODEL_DEBUG("%08lx\n", reg_value);
#else
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", (reg_value & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 8) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", ((reg_value >> 16) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 24) & 0x000000FF));
#endif
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }
#if 0
            JPEG_CMODEL_DEBUG("%08lx\n", reg_value);
#else
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", (reg_value & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 8) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101782 0x%lx\n", ((reg_value >> 16) & 0x000000FF));
            JPEG_CMODEL_DEBUG("wriu 0x101783 0x%lx\n", ((reg_value >> 24) & 0x000000FF));
#endif
        }
    }
}


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Performs a 2D IDCT over the entire row's coefficient buffer.
static void JPEG_transform_row( void )
{
    {
        JPEG_BLOCK_TYPE *Psrc_ptr = gps16Block_seg[0];
        MS_U8 *Pdst_ptr = gpu8Sample_buf;
        MS_U32 i;

        for ( i = _u32Max_blocks_per_row; i > 0; i-- )
        {
            // Copy the block to a temp. buffer to prevent the IDCT
            // from modifying the entire block.
            //kevinhuang, use an internal array in idct to avoid memcpy to save time
            //memcpy(temp_block, Psrc_ptr, 64 * sizeof(JPEG_BLOCK_TYPE));
            //idct(temp_block, Pdst_ptr);
            idct( Psrc_ptr, Pdst_ptr );
            Psrc_ptr += 64;
            Pdst_ptr += 64;
        }
    }
}

//------------------------------------------------------------------------------
// Decodes and dequantizes the next row of coefficients.
static void JPEG_decode_next_row( void )
{
    MS_U16 row_block = 0;
    MS_U16 mcu_row, mcu_block, k;

    // Clearing the entire row block buffer can take a lot of time!
    // Instead of clearing the entire buffer each row, keep track
    // of the number of nonzero entries written to each block and do
    // selective clears.
    //memset(gps16Block_seg[0], 0, _u16Mcus_per_row * _u8Blocks_per_mcu * 64 * sizeof(JPEG_BLOCK_TYPE));

    for ( mcu_row = 0; mcu_row < _u16Mcus_per_row; mcu_row++ )
    {
        if ( ( _u16Restart_interval ) && ( _u16Restarts_left == 0 ) )
        {
            JPEG_process_restart();
        }

        for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu; mcu_block++ )
        {
            MS_U8 component_id = _u8Mcu_org[mcu_block];
            MS_U8 prev_num_set;
            JPEG_HuffTbl *Ph;

            JPEG_BLOCK_TYPE *p = gps16Block_seg[row_block];
            JPEG_QUANT_TYPE *q = _QuantTables[_u8Comp_quant[component_id]].s16Value;
            MS_U32 r, s;

            if ( ( s = JPEG_huff_decode( &_Huff_tbls[_u8Comp_dc_tab[component_id]] ) ) != 0 )
            {
                r = JPEG_get_bits_2( s );
                s = HUFF_EXTEND( r, s );
            }

            _u32Last_dc_val[component_id] = ( s += _u32Last_dc_val[component_id] );

            p[0] = s * q[0];

            prev_num_set = gu8Block_max_zag_set[row_block];

            Ph = &_Huff_tbls[_u8Comp_ac_tab[component_id]];

            for ( k = 1; k < 64; k++ )
            {
                s = JPEG_huff_decode( Ph );

                r = s >> 4;
                s &= 15;

                if ( s )
                {
                    if ( r )
                    {
                        if ( ( k + r ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U32 n = min( r, prev_num_set - k );
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += r;
                    }

                    r = JPEG_get_bits_2( s );
                    s = HUFF_EXTEND( r, s );

                    //assert(k < 64);

                    p[_u8ZAG[k]] = s * q[k];
                }
                else
                {
                    if ( r == 15 )
                    {
                        if ( ( k + 15 ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U16 n = min( 16, prev_num_set - k );        //bugfix Dec. 19, 2001 - was 15!
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += 15;
                    }
                    else
                    {
                        //while (k < 64)
                        //  p[ZAG[k++]] = 0;

                        break;
                    }
                }
            }

            if ( k < prev_num_set )
            {
                MS_U16 kt = k;
                while ( kt < prev_num_set )
                {
                    p[_u8ZAG[kt++]] = 0;
                }
            }

            gu8Block_max_zag_set[row_block] = k;

            //block_num[row_block++] = k;
            row_block++;
        }

        _u16Restarts_left--;
    }
}


//------------------------------------------------------------------------------
// Find end of image (EOI) marker, so we can return to the user the
// exact size of the input stream.
static void JPEG_find_eoi( void )
{
    if ( !MApi_JPEG_IsProgressive() )
    {
        // Prime the bit buffer
        _s16Bits_left = 0;

        // The next marker _should_ be EOI
        JPEG_process_markers();
    }

    _u32Total_bytes_read -= _u32In_buf_left;
}

//------------------------------------------------------------------------------
// YCbCr H1V1 (1x1:1:1, 3 blocks per MCU) to 24-bit RGB
static void JPEG_CMODEL_H1V1Convert( void )
{
    MS_U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    MS_U8 *d = pgu8Scan_line_0;
    MS_U8 *s = gpu8Sample_buf + row * 8;
    MS_U16 i, j;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( j = 0; j < 8; j++ )
        {
            MS_U8 y = s[j];
            MS_U8 cb = s[64 + j];
            MS_U8 cr = s[128 + j];

            d[0] = JPEG_CMODEL_clamp( y + gs32Crr[cr] );
            d[1] = JPEG_CMODEL_clamp( y + ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 ) );
            d[2] = JPEG_CMODEL_clamp( y + gs32Cbb[cb] );
            d += 4;
        }

        s += 64 * 3;
    }
}

//------------------------------------------------------------------------------
// YCbCr H4V1 (4x1:1:1, 4 blocks per MCU) to 24-bit RGB
// Wrong... but it can be used to see the decoding effect... =_=""
static void JPEG_CMODEL_H4V1Convert( void )
{
    MS_U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    MS_U8 *d0 = pgu8Scan_line_0;
    MS_U8 *y = gpu8Sample_buf + row * 8;
    MS_U8 *c = gpu8Sample_buf + 4 * 64 + row * 8;
    MS_U16 i, l, j;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( l = 0; l < 4; l++ )
        {
            for ( j = 0; j < 4; j++ )
            {
                MS_U8 cb = c[0];
                MS_U8 cr = c[64];

                MS_S32 rc = gs32Crr[cr];
                MS_S32 gc = ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 );
                MS_S32 bc = gs32Cbb[cb];

                MS_U16 yy = y[j << 1];
                d0[0] = JPEG_CMODEL_clamp( yy + rc );
                d0[1] = JPEG_CMODEL_clamp( yy + gc );
                d0[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[( j << 1 ) + 1];
                d0[4] = JPEG_CMODEL_clamp( yy + rc );
                d0[5] = JPEG_CMODEL_clamp( yy + gc );
                d0[6] = JPEG_CMODEL_clamp( yy + bc );

                d0 += 8;

                if(j%2==0)
                    c++;
            }
            y += 64;
        }

        y += 64 * 4 - 64 * 2;
        c += 64 * 6 - 8;
    }
}

//------------------------------------------------------------------------------
// YCbCr H2V1 (2x1:1:1, 4 blocks per MCU) to 24-bit RGB
static void JPEG_CMODEL_H2V1Convert( void )
{
    MS_U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    MS_U8 *d0 = pgu8Scan_line_0;
    MS_U8 *y = gpu8Sample_buf + row * 8;
    MS_U8 *c = gpu8Sample_buf + 2 * 64 + row * 8;
    MS_U16 i, l, j;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( l = 0; l < 2; l++ )
        {
            for ( j = 0; j < 4; j++ )
            {
                MS_U8 cb = c[0];
                MS_U8 cr = c[64];

                MS_S32 rc = gs32Crr[cr];
                MS_S32 gc = ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 );
                MS_S32 bc = gs32Cbb[cb];

                MS_U16 yy = y[j << 1];
                d0[0] = JPEG_CMODEL_clamp( yy + rc );
                d0[1] = JPEG_CMODEL_clamp( yy + gc );
                d0[2] = JPEG_CMODEL_clamp( yy + bc );

                yy = y[( j << 1 ) + 1];
                d0[4] = JPEG_CMODEL_clamp( yy + rc );
                d0[5] = JPEG_CMODEL_clamp( yy + gc );
                d0[6] = JPEG_CMODEL_clamp( yy + bc );

                d0 += 8;

                c++;
            }
            y += 64;
        }

        y += 64 * 4 - 64 * 2;
        c += 64 * 4 - 8;
    }
}

//------------------------------------------------------------------------------
// YCbCr H2V1 (1x2:1:1, 4 blocks per MCU) to 24-bit RGB
static void JPEG_CMODEL_H1V2Convert( void )
{
    MS_U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    MS_U8 *d0 = pgu8Scan_line_0;
    MS_U8 *d1 = pgu8scan_line_1;
    MS_U8 *y;
    MS_U8 *c;
    MS_U16 i, j;

    if ( row < 8 )
    {
        y = gpu8Sample_buf + row * 8;
    }
    else
    {
        y = gpu8Sample_buf + 64 * 1 + ( row & 7 ) * 8;
    }

    c = gpu8Sample_buf + 64 * 2 + ( row >> 1 ) * 8;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        for ( j = 0; j < 8; j++ )
        {
            MS_U8 cb = c[0 + j];
            MS_U8 cr = c[64 + j];

            MS_S32 rc = gs32Crr[cr];
            MS_S32 gc = ( ( gs32Crg[cr] + gs32Cbg[cb] ) >> 16 );
            MS_S32 bc = gs32Cbb[cb];

            MS_U16 yy = y[j];
            d0[0] = JPEG_CMODEL_clamp( yy + rc );
            d0[1] = JPEG_CMODEL_clamp( yy + gc );
            d0[2] = JPEG_CMODEL_clamp( yy + bc );

            yy = y[8 + j];
            d1[0] = JPEG_CMODEL_clamp( yy + rc );
            d1[1] = JPEG_CMODEL_clamp( yy + gc );
            d1[2] = JPEG_CMODEL_clamp( yy + bc );

            d0 += 4;
            d1 += 4;
        }

        y += 64 * 4;
        c += 64 * 4;
    }
}

//------------------------------------------------------------------------------
static void JPEG_CMODEL_GrayConvert( void )
{
    MS_U8 row = gu8Max_mcu_y_size - gu16Mcu_lines_left;
    MS_U8 *d = pgu8Scan_line_0;
    MS_U8 *s = gpu8Sample_buf + row * 8;
    MS_U16 i;

    for ( i = gu16Max_mcus_per_row; i > 0; i-- )
    {
        *( MS_U32 * )d = *( MS_U32 * )s;
        *( MS_U32 * )( &d[4] ) = *( MS_U32 * )( &s[4] );

        s += 64;
        d += 8;
    }
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Create a few tables that allow us to quickly convert YCbCr to RGB.
void JPEG_CMODEL_create_look_ups( void )
{
    MS_S16 i, k;
    //kevinhuang, modify
    /*
    for (i = 0; i <= 255; i++)
    {
      //k = (i * 2) - 255;
        k = (i * 2) - 256; // Dec. 28 2001- change so table[128] == 0
      gs32Crr[i] = ( FIX(1.40200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Cbb[i] = ( FIX(1.77200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Crg[i] = (-FIX(0.71414/2)) * k;
      gs32Cbg[i] = (-FIX(0.34414/2)) * k + ONE_HALF;
    }
    */
    for ( i = 0; i <= 255; i++ )
    {
        k = i - 128;

        gs32Crr[i] = ( FIX( 1.40200 ) * k + ONE_HALF ) >> SCALEBITS;
        gs32Cbb[i] = ( FIX( 1.77200 ) * k + ONE_HALF ) >> SCALEBITS;

        gs32Crg[i] = ( -FIX( 0.71414 ) ) * k ; //+ ONE_HALF) >> SCALEBITS;???
        gs32Cbg[i] = ( -FIX( 0.34414 ) ) * k + ONE_HALF; //>> SCALEBITS;???
    }
}

//------------------------------------------------------------------------------
// Returns the next scan line.
// Returns E_JPEG_OKAY if all scan lines have been returned.
// Returns E_JPEG_DONE if a scan line has been returned.
// Returns E_JPEG_FAILED if an error occured.
MS_S16 JPEG_CMODEL_decode( void **Pscan_line_ofs, MS_U32 *Pscan_line_len )
{
    if ( ( _Error_code ) || ( !_bReady_flag ) )
    {
        return ( E_JPEG_FAILED );
    }

    if ( _u16Total_lines_left == 0 )
    {
        return ( E_JPEG_OKAY );
    }

    if ( gu16Mcu_lines_left == 0 ) //decode every gu8Max_mcu_y_size (8 or 16) lines
    {
        if ( setjmp( _jmp_state ) )
        {
            return ( E_JPEG_DECODE_ERROR );
        }

        if ( MApi_JPEG_IsProgressive() )
        {
            JPEG_load_next_row();
        }
        else
        {
            JPEG_decode_next_row(); //Dequantization
        }

        // Find the EOI marker if that was the last row.
        if ( _u16Total_lines_left <= gu8Max_mcu_y_size )
        {
            JPEG_find_eoi();
        }

        JPEG_transform_row(); //IDCT

        gu16Mcu_lines_left = gu8Max_mcu_y_size;
    }

    //YUV->RGB
    switch ( gu8Scan_type )
    {
        case E_JPEG_YH2V2:
            {
                if ( ( gu16Mcu_lines_left & 1 ) == 0 )
                {
                    msAPI_JPEG_H2V2Convert();
                    *Pscan_line_ofs = pgu8Scan_line_0;
                }
                else
                {
                    *Pscan_line_ofs = pgu8scan_line_1;
                }

                break;
            }
        case E_JPEG_YH4V1:
            {
                JPEG_CMODEL_H4V1Convert();
                *Pscan_line_ofs = pgu8Scan_line_0;
                break;
            }
        case E_JPEG_YH2V1:
            {
                JPEG_CMODEL_H2V1Convert();
                *Pscan_line_ofs = pgu8Scan_line_0;
                break;
            }
        case E_JPEG_YH1V2:
            {
                if ( ( gu16Mcu_lines_left & 1 ) == 0 )
                {
                    JPEG_CMODEL_H1V2Convert();
                    *Pscan_line_ofs = pgu8Scan_line_0;
                }
                else
                {
                    *Pscan_line_ofs = pgu8scan_line_1;
                }

                break;
            }
        case E_JPEG_YH1V1:
            {
                JPEG_CMODEL_H1V1Convert();
                *Pscan_line_ofs = pgu8Scan_line_0;
                break;
            }
        case E_JPEG_GRAYSCALE:
            {
                JPEG_CMODEL_GrayConvert();
                *Pscan_line_ofs = pgu8Scan_line_0;

                break;
            }
    }

    *Pscan_line_len = gu16Real_dest_bytes_per_scan_line;

    gu16Mcu_lines_left--;
    _u16Total_lines_left--;

    return ( E_JPEG_DONE );
}

//------------------------------------------------------------------------------
MS_U8 JPEG_CMODEL_get_bytes_per_pixel( void )
{
    return ( gu8Dest_bytes_per_pixel );
}

//------------------------------------------------------------------------------
MS_U8 JPEG_CMODEL_get_num_components( void ) //cmodel
{
    return ( _u8Comps_in_frame );
}
//------------------------------------------------------------------------------
MS_U32 JPEG_CMODEL_get_total_bytes_read( void ) //cmodel
{
    return ( _u32Total_bytes_read );
}

//------------------------------------------------------------------------------
// Clamps a value between 0-255.
MS_U8 JPEG_CMODEL_clamp( MS_S32 i ) //???
{
    if ( i & 0xFFFFFF00 )
    {
        i = ( ( ( ~i ) >> 31 ) & 0xFF );
    }

    return ( i );
}

#if 0 //useless funciton
//------------------------------------------------------------------------------
MS_U32 JPEG_CMODEL_get_bytes_per_scan_line( void )
{
    return ( _u16Image_x_size * JPEG_CMODEL_get_bytes_per_pixel() );
}
#endif


