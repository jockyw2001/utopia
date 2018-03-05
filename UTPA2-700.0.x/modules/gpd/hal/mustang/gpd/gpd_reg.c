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
#include <stdio.h>
#include "gpd_reg.h"

int GPD_REG_BASE;

const unsigned short gpd_reg_addr[] =
{
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x0,
0x1,
0x1,
0x2,
0x3,
0x4,
0x5,
0x6,
0x8,
0xa,
0xc,
0xe,
0xf,
0x10,
0x12,
0x13,
0x14,
0x15,
0x15,
0x15,
0x16,
0x17,
0x18,
0x18,
0x18,
0x18,
0x18,
0x18,
0x19,
0x19,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1a,
0x1b,
0x1b,
0x1b,
0x1b,
0x1c,
0x1c,
0x1d,
0x1e,
0x1e,
0x20,
0x20,
0x22,
0x22,
0x23,
0x23,
0x23,
0x23,
0x24,
0x24,
0x24,
0x25,
0x26,
0x26,
0x28,
0x28,
0x2a,
0x2a,
0x2c,
0x2c,
0x2d,
0x2e,
0x2e,
0x2f,
0x30,
0x30,
0x31,
0x32,
0x32,
0x33,
0x34,
0x34,
0x35,
0x35,
0x36,
0x36,
0x36,
0x36,
0x37,
0x37,
0x38,
0x38,
0x38,
0x38,
0x39,
0x39,
0x3a,
0x3b,
0x3c,
0x3d,
0x3e,
0x3f,
0x40,
0x41,
0x42,
0x43,
0x44,
0x45,
0x46,
0x47,
0x48,
0x48,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4a,
0x4b,
0x4c,
0x4e,
0x4f,
0x4f,
0x4f,
0x50,
0x51,
0x51,
0x51,
0x52,
0x53,
0x53,
0x53,
0x53,
0x53,
0x54,
0x55,
0x55,
0x55,
0x55,
0x56,
0x58,
0x5a,
0x5c,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5e,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x5f,
0x60,
0x60,
0x60,
0x60,
0x60,
0x60,
0x61,
0x61,
0x61,
0x61,
0x61,
0x62,
0x64,
0x65,
0x66,
0x66,
0x67,
0x69,
0x6b,
0x6b,
0x6b,
0x6b,
0x6c,
0x6e,
0x70,
0x70
};

const unsigned char gpd_reg_msb[] =
{
0,
1,
2,
3,
4,
5,
8,
9,
10,
11,
12,
13,
14,
19,
9,
15,
15,
14,
15,
15,
30,
30,
30,
31,
8,
15,
15,
15,
15,
15,
0,
8,
15,
15,
15,
0,
1,
3,
6,
10,
15,
5,
12,
0,
2,
5,
6,
7,
8,
9,
15,
8,
9,
14,
15,
7,
16,
10,
10,
22,
12,
26,
14,
15,
1,
4,
8,
13,
5,
12,
20,
13,
9,
20,
11,
24,
13,
28,
8,
17,
10,
8,
17,
10,
8,
17,
10,
8,
17,
10,
8,
17,
6,
11,
4,
9,
14,
19,
8,
13,
4,
9,
14,
19,
8,
13,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
14,
29,
0,
1,
4,
5,
8,
10,
13,
14,
15,
15,
31,
23,
13,
14,
15,
25,
10,
14,
15,
25,
10,
11,
12,
14,
15,
25,
10,
12,
13,
14,
31,
31,
27,
27,
0,
1,
2,
3,
4,
5,
6,
7,
8,
9,
10,
11,
12,
13,
14,
15,
0,
1,
2,
3,
4,
5,
6,
4,
9,
12,
13,
14,
15,
4,
9,
11,
13,
15,
27,
27,
13,
5,
15,
31,
17,
9,
12,
13,
14,
23,
31,
7,
15
};

const unsigned char gpd_reg_lsb[] =
{
0,
1,
2,
3,
4,
5,
6,
9,
10,
11,
12,
13,
14,
15,
4,
10,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
1,
9,
0,
0,
0,
1,
2,
4,
7,
11,
0,
6,
0,
1,
3,
6,
7,
8,
9,
10,
0,
9,
10,
15,
0,
8,
1,
0,
11,
0,
13,
0,
15,
0,
2,
5,
9,
0,
6,
13,
5,
0,
10,
0,
12,
0,
14,
0,
9,
2,
0,
9,
2,
0,
9,
2,
0,
9,
2,
0,
9,
2,
7,
0,
5,
10,
15,
4,
9,
0,
5,
10,
15,
4,
9,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
15,
0,
1,
2,
5,
6,
9,
11,
14,
15,
0,
0,
0,
8,
14,
15,
0,
10,
11,
15,
0,
10,
11,
12,
13,
15,
0,
10,
11,
13,
14,
0,
0,
0,
0,
0,
1,
2,
3,
4,
5,
6,
7,
8,
9,
10,
11,
12,
13,
14,
15,
0,
1,
2,
3,
4,
5,
6,
0,
5,
10,
13,
14,
15,
0,
5,
10,
13,
14,
0,
0,
12,
0,
6,
0,
0,
0,
10,
13,
14,
0,
0,
0,
8
};

const char *gpd_reg_name[] = {
"reg_gif_go_le            ",
"reg_act_chk_le           ",
"reg_gpd_bsaddr_go_le     ",
"reg_gpd_rst_le           ",
"reg_png_go_le            ",
"reg_png_blk_go_le        ",
"reg_gif_ltbl_size        ",
"reg_gif_local_tbl        ",
"reg_gif_done             ",
"reg_ofifo_abort          ",
"reg_ififo_full           ",
"reg_ififo_empty          ",
"reg_bsadr_full           ",
"reg_ififo_diff           ",
"reg_bitpos               ",
"reg_png_dtbl_size        ",
"reg_gif_state            ",
"reg_gpd_pitch            ",
"reg_gpd_iwidth           ",
"reg_gpd_iheight          ",
"reg_gpd_bstart           ",
"reg_gpd_bend             ",
"reg_gpd_istart           ",
"reg_spare1               ",
"reg_gpd_boffset          ",
"reg_iofifo_state         ",
"reg_gpd_roi_hstart       ",
"reg_gpd_roi_vstart       ",
"reg_gpd_roi_width        ",
"reg_gpd_roi_height       ",
"reg_gpd_roi_en           ",
"reg_gpd_default_alpha    ",
"reg_spare2_rst0          ",
"reg_png_trans_r          ",
"reg_png_trans_g          ",
"reg_png_blk_done         ",
"reg_png_mincode1         ",
"reg_png_mincode2         ",
"reg_png_mincode3         ",
"reg_png_mincode4         ",
"reg_png_mincode5         ",
"reg_png_mincode6         ",
"reg_png_mincode7         ",
"reg_gpd_ipm_en           ",
"reg_gpd_ipm_size         ",
"reg_gpd_ocolor           ",
"reg_gpd_en               ",
"reg_ofifo_done           ",
"reg_miu_domain_empty     ",
"reg_mreq_always_active   ",
"reg_eng_always_active    ",
"reg_png_ltbl_size        ",
"reg_gpd_act_chk          ",
"reg_png_sca              ",
"reg_png_eob              ",
"reg_png_mincode8         ",
"reg_png_mincode9         ",
"reg_png_mincode10        ",
"reg_png_mincode11        ",
"reg_png_mincode12        ",
"reg_png_mincode13        ",
"reg_png_mincode14        ",
"reg_png_mincode15        ",
"reg_png2_mincode1        ",
"reg_png2_mincode2        ",
"reg_png2_mincode3        ",
"reg_png2_mincode4        ",
"reg_png2_mincode5        ",
"reg_png2_mincode6        ",
"reg_png2_mincode7        ",
"reg_png2_mincode8        ",
"reg_png2_mincode9        ",
"reg_png2_mincode10       ",
"reg_png2_mincode11       ",
"reg_png2_mincode12       ",
"reg_png2_mincode13       ",
"reg_png2_mincode14       ",
"reg_png2_mincode15       ",
"reg_png_lbase2           ",
"reg_png_lbase3           ",
"reg_png_lbase4           ",
"reg_png_lbase5           ",
"reg_png_lbase6           ",
"reg_png_lbase7           ",
"reg_png_lbase8           ",
"reg_png_lbase9           ",
"reg_png_lbase10          ",
"reg_png_lbase11          ",
"reg_png_lbase12          ",
"reg_png_lbase13          ",
"reg_png_lbase14          ",
"reg_png_lbase15          ",
"reg_png_dbase2           ",
"reg_png_dbase3           ",
"reg_png_dbase4           ",
"reg_png_dbase5           ",
"reg_png_dbase6           ",
"reg_png_dbase7           ",
"reg_png_dbase8           ",
"reg_png_dbase9           ",
"reg_png_dbase10          ",
"reg_png_dbase11          ",
"reg_png_dbase12          ",
"reg_png_dbase13          ",
"reg_png_dbase14          ",
"reg_png_dbase15          ",
"reg_png_scline0_width    ",
"reg_png_scline1_width    ",
"reg_png_scline2_width    ",
"reg_png_scline3_width    ",
"reg_png_scline4_width    ",
"reg_png_scline5_width    ",
"reg_png_scline6_width    ",
"reg_png_scline0_height   ",
"reg_png_scline1_height   ",
"reg_png_scline2_height   ",
"reg_png_scline3_height   ",
"reg_png_scline4_height   ",
"reg_png_scline5_height   ",
"reg_png_scline6_height   ",
"reg_png_mincode_valid    ",
"reg_png2_mincode_valid   ",
"reg_gpd_only_decom_en    ",
"reg_png_done             ",
"reg_png_color_type       ",
"reg_gpd_interlace        ",
"reg_spare6               ",
"reg_png_compress_type    ",
"reg_png_color_depth      ",
"reg_miu_act_chk          ",
"reg_png_trans_en         ",
"reg_png_trans_b          ",
"reg_png_state            ",
"reg_ififo_cnt            ",
"reg_hipri                ",
"reg_gpd_premult_alpha_en ",
"reg_gpd_gif_alpha_mask_en",
"reg_frun_cnt             ",
"reg_png_burst_en         ",
"reg_gif_mask             ",
"reg_deflt_fast_on        ",
"reg_ififo_radr           ",
"reg_gpd_time_out         ",
"reg_gif_code_size_err    ",
"reg_gif_err              ",
"reg_gpd_pgend            ",
"reg_miu64                ",
"reg_gpd2mi_adr           ",
"reg_wait_last_done       ",
"reg_miu_wait_cyc         ",
"reg_fixed_pri            ",
"reg_last_done_md         ",
"reg_gpd_read_data        ",
"reg_io_read_gpd          ",
"reg_cbuf_bas             ",
"reg_zbuf_bas             ",
"reg_bist_fail_lz_psram   ",
"reg_bist_fail_bst_psram  ",
"reg_bist_fail_lit_psram  ",
"reg_bist_fail_flt_psram  ",
"reg_bist_fail_lmem       ",
"reg_bist_fail_cmem       ",
"reg_bist_fail_omem       ",
"reg_bist_fail_imem       ",
"reg_bist_fail_dc0_cmem   ",
"reg_bist_fail_dc1_cmem   ",
"reg_bist_fail_stk_psram  ",
"reg_bist_fail_dma1       ",
"reg_bist_fail_dma0       ",
"reg_bist_fail_cmd        ",
"reg_bist_fail_data       ",
"reg_bist_fail_cc_cmem    ",
"reg_bist_fail_cc_dmem    ",
"reg_bist_fail_cc_rdmem   ",
"reg_bist_fail_cc_wdmem   ",
"reg_bist_fail_zc_cmem    ",
"reg_bist_fail_zc_dmem    ",
"reg_bist_fail_zc_rdmem   ",
"reg_bist_fail_zc_wdmem   ",
"reg_int_mask             ",
"reg_int_rst              ",
"reg_debug_mux            ",
"reg_dram_imi             ",
"reg_gpd_istr_8b          ",
"reg_gif_act_clr          ",
"reg_int_status           ",
"reg_int_sw_force         ",
"reg_cache_hit_cmp        ",
"reg_scale_en             ",
"reg_scale_md             ",
"reg_lb_addr              ",
"reg_ub_addr              ",
"reg_gpd_xiu_byte_sel   ",
"reg_gpd_read_bits        ",
"reg_gpd_reserved1        ",
"reg_gpd_cmem_wdata       ",
"reg_png_ltbl_wdata       ",
"reg_png_dtbl_wdata       ",
"reg_debug2_mux           ",
"reg_wbe_bypass_go_chk    ",
"reg_gpd_sram_sd_en       ",
"reg_gpd_debug            ",
"reg_gpd_debug2           ",
"reg_gpd_version          ",
"reg_gpd_tlb              "
};

static volatile U32 *GPD_GET_ADDR(int index)
{
    U32 offset = gpd_reg_addr[index] - (gpd_reg_lsb[index] >> 4);
    return (volatile U32 *)(GPD_REG_BASE + offset * 4);
}

static U32 GPD_GET_U32MASK(U32 index)
{
    if(gpd_reg_msb[index] - gpd_reg_lsb[index] == 31)

      return 0xffffffff;

    else
        return (((U32)1 << (gpd_reg_msb[index] - gpd_reg_lsb[index] + 1)) - 1) << gpd_reg_lsb[index];
}

static U32 GPD_READ_U32(volatile U32 *u32addr)
{
    //volatile U32 *u32addr = (volatile U32 *)u8addr;
    return (u32addr[0] & 0xFFFF) | (u32addr[1] << 16);
}

static void GPD_WRITE_U32(volatile U32 *u32addr, U32 value)
{
    //volatile U32 *u32addr = (volatile U32 *)u8addr;
    u32addr[0] = (value) & 0xFFFF;
    u32addr[1] = (value) >> 16;
}

U32 GPD_GET_U32REG_dbg(int index)
{
    volatile U32 *u32addr = GPD_GET_ADDR(index);


    U32 temp = (GPD_READ_U32(u32addr) & GPD_GET_U32MASK(index)) >> gpd_reg_lsb[index];


    console_printf("Reg read: %s = 0x%08x, offset = 0x%02X, lsb=%d, msb=%d\n", gpd_reg_name[index], (unsigned int)temp,
        (unsigned short)gpd_reg_addr[index], gpd_reg_lsb[index], gpd_reg_msb[index]);
    return temp;
}

U32 GPD_GET_U32REG(int index)
{
    volatile U32 *u32addr = GPD_GET_ADDR(index);


    U32 temp = (GPD_READ_U32(u32addr) & GPD_GET_U32MASK(index)) >> gpd_reg_lsb[index];


    //console_printf("Reg read: %s = 0x%08X, addr = 0x%08X\n", gpd_reg_name[index], temp, u8addr);
    return temp;
}

void GPD_SET_U32REG(int index, U32 value)
{
    volatile U32 *u32addr = GPD_GET_ADDR(index);
    U32 mask = GPD_GET_U32MASK(index);
    U32 temp = GPD_READ_U32(u32addr);

    temp &= ~mask;

    temp |= ((value) << gpd_reg_lsb[index]) & mask;
    GPD_WRITE_U32(u32addr, temp);

//    console_printf("Reg write: %s = 0x%08X, offset = 0x%02X, lsb=%d, msb=%d\n", gpd_reg_name[index], (unsigned int)value,
//        (unsigned short)gpd_reg_addr[index], gpd_reg_lsb[index], gpd_reg_msb[index]);
}

unsigned int hal_gpd_get_clkbase()
{
    //mustang, clkgen0
    return (0x0b00 * 2);
}

unsigned int hal_gpd_get_clkoffset()
{
    //mustang, clkgen0, h0003	h0003	12	8	reg_ckg_gpd
    return ((0x06 << 1) | (0x8<<24));
}

void hal_gpd_reg_base(U32* u32RIUBase, U32* u32XIUBase)
{
    *u32RIUBase = (0x13E00 * 2);
    *u32XIUBase = (0x13F00 * 2);
}

void hal_gpd_miu_client(U8* u8Offset, U16* u16BitMask)
{
    // mustang, h0079	h0079	15	0	reg_miu_sel1
    // group1, bit [5]
    *u8Offset = 0xf2;
    *u16BitMask = 0x20;
}

void hal_gpd_init_outside_reg(U32 BankBaseAddr)
{
    // mustang, Bank 0x101e, Offset 0x21, Bit [5] = 1 'b1
    unsigned int tmp = 0;
    tmp = *((volatile U32 *)(BankBaseAddr + 0x1E00*2 + 0x21*2*2));
    tmp |= 0x0020;
    console_printf("set i64 reg to 0x%x\n", tmp);
    *((volatile U32 *)(BankBaseAddr + 0x1E00*2 + 0x21*2*2)) = tmp;

}


void hal_gpd_init_chip_specific_reg(void)
{
}

void hal_gpd_SetMIUProtectMask(U32 BankBaseAddr, U8 bEnable)
{
    unsigned int tmp = 0;
    // mustang, h0033	h0033	15	0	reg_rq1_mask
    // group1, bit [5]
    tmp = *((volatile U32 *)(BankBaseAddr + 0x1200*2 + 0x33*2*2));
    if(bEnable==TRUE)
    {
        tmp |= 0x20;
    }
    else
    {
        tmp &=~0x20;
    }
    *((volatile U32 *)(BankBaseAddr + 0x1200*2 + 0x33*2*2)) = tmp;

    tmp = *((volatile U32 *)(BankBaseAddr + 0x0600*2 + 0x33*2*2));
    if(bEnable==TRUE)
    {
        tmp |= 0x20;
    }
    else
    {
        tmp &=~0x20;
    }
    *((volatile U32 *)(BankBaseAddr + 0x0600*2 + 0x33*2*2)) = tmp;

    console_printf("set miu client protect to 0x%x\n", tmp);

    return;
}

