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

#ifndef _DECODER_INFO_H_
#define _DECODER_INFO_H_

//===========================================================================
// define supported decoder here
//===========================================================================
#ifndef CONFIG_MBOOT // Normal mode
    #define    DSP_SUPPORT_XPCM                1
    #define    DSP_SUPPORT_MP3                 1
    #define    DSP_SUPPORT_MPEG                1
    #define    DSP_SUPPORT_AC3P                1
    #define    DSP_SUPPORT_MS10_DDT            0
    #define    DSP_SUPPORT_GAAC                1
    #define    DSP_SUPPORT_WMA_PRO             1
    #define    DSP_SUPPORT_DTS_DMP             1
    #define    DSP_SUPPORT_DTS_LBR             0
    #define    DSP_SUPPORT_RA8LBR              1
    #define    DSP_SUPPORT_VORBIS              1
    #define    DSP_SUPPORT_AMR_NB              1
    #define    DSP_SUPPORT_AMR_WB              1
    #define    DSP_SUPPORT_DRAD                1
    #define    DSP_SUPPORT_FLAC                1
    #define    DSP_SUPPORT_PALSUM              1
    #define    DSP_SUPPORT_BTSC                1
    #define    DSP_SUPPORT_TSHD                1
    #define    DSP_SUPPORT_TSXT                1
    #define    DSP_SUPPORT_PURESND             1
    #define    DSP_SUPPORT_DEC                 1
    #define    DSP_SUPPORT_DDE                 0
    #define    DSP_SUPPORT_DBX                 1
#else //Mboot mode
    #define    DSP_SUPPORT_XPCM                0
    #define    DSP_SUPPORT_MP3                 1
    #define    DSP_SUPPORT_MPEG                0
    #define    DSP_SUPPORT_AC3P                0
    #define    DSP_SUPPORT_MS10_DDT            0
    #define    DSP_SUPPORT_GAAC                0
    #define    DSP_SUPPORT_WMA_PRO             0
    #define    DSP_SUPPORT_DTS_DMP             0
    #define    DSP_SUPPORT_DTS_LBR             0
    #define    DSP_SUPPORT_RA8LBR              0
    #define    DSP_SUPPORT_VORBIS              0
    #define    DSP_SUPPORT_AMR_NB              0
    #define    DSP_SUPPORT_AMR_WB              0
    #define    DSP_SUPPORT_DRAD                0
    #define    DSP_SUPPORT_FLAC                0
    #define    DSP_SUPPORT_PALSUM              0
    #define    DSP_SUPPORT_BTSC                0
    #define    DSP_SUPPORT_TSHD                0
    #define    DSP_SUPPORT_TSXT                0
    #define    DSP_SUPPORT_PURESND             0
    #define    DSP_SUPPORT_DBX                 0  
    #define    DSP_SUPPORT_DEC                 0
    #define    DSP_SUPPORT_DDE                 0
#endif

//===========================================================================
// Include files
//===========================================================================    

    #include    "dspcode_s/aucode_s.c"

#if(DSP_SUPPORT_XPCM)
    #include    "dspcode_s/aucode_xpcm.c"
#endif
#if(DSP_SUPPORT_MP3)
    #include    "dspcode_s/aucode_mp3.c"
#endif
#if(DSP_SUPPORT_MPEG)
    #include    "dspcode_s/aucode_mpeg.c"
#endif
#if(DSP_SUPPORT_AC3P)
    #include    "dspcode_s/aucode_ac3p.c"
#endif
#if(DSP_SUPPORT_MS10_DDT)
    #include    "dspcode_s/aucode_ms10_ddt.c"
#endif
#if(DSP_SUPPORT_GAAC)
    #include    "dspcode_s/aucode_gaac.c"
#endif
#if(DSP_SUPPORT_WMA_PRO)
    #include    "dspcode_s/aucode_wma_pro.c"
#endif
#if(DSP_SUPPORT_DTS_DMP)
    #include    "dspcode_s/aucode_dts_dmp.c"
#endif
#if(DSP_SUPPORT_DTS_LBR)
    #include    "dspcode_s/aucode_dts_lbr.c"
#endif
#if(DSP_SUPPORT_RA8LBR)
    #include    "dspcode_s/aucode_ra8lbr.c"
#endif
#if(DSP_SUPPORT_VORBIS)
    #include    "dspcode_s/aucode_vorbis.c"
#endif
#if(DSP_SUPPORT_AMR_NB)
    #include    "dspcode_s/aucode_amr_nb.c"
#endif
#if(DSP_SUPPORT_AMR_WB)
    #include    "dspcode_s/aucode_amr_wb.c"
#endif
#if(DSP_SUPPORT_DRAD)
    #include    "dspcode_s/aucode_drad.c"
#endif
#if(DSP_SUPPORT_FLAC)
    #include    "dspcode_s/aucode_flac.c"
#endif
#if(DSP_SUPPORT_PALSUM)
    #include    "dspcode_s/aucode_palsum.c"
#endif
#if(DSP_SUPPORT_BTSC)
    #include    "dspcode_s/aucode_btsc.c"
#endif
#if(DSP_SUPPORT_TSHD)
    #include    "dspcode_s/aucode_tshd.c"
#endif
#if(DSP_SUPPORT_TSXT)
    #include    "dspcode_s/aucode_tsxt.c"
#endif
#if(DSP_SUPPORT_PURESND)
    #include    "dspcode_s/aucode_puresnd.c"
#endif
#if(DSP_SUPPORT_DEC)
    #include    "dspcode_s/aucode_dec.c"
#endif
#if(DSP_SUPPORT_DDE)
    #include    "dspcode_s/aucode_dde.c"
#endif
#if(DSP_SUPPORT_DBX)
    #include    "dspcode_s/aucode_dbx.c"
#endif
//===========================================================
#ifndef    MST_CODEC_XPCM_PM1_ADDR
    #define    MST_CODEC_XPCM_PM1_ADDR              0
    #define    MST_CODEC_XPCM_PM1_SIZE              0
    #define    mst_codec_xpcm_pm1                   0
#endif
#ifndef    MST_CODEC_XPCM_PM2_ADDR
    #define    MST_CODEC_XPCM_PM2_ADDR              0
    #define    MST_CODEC_XPCM_PM2_SIZE              0
    #define    mst_codec_xpcm_pm2                   0
#endif
#ifndef    MST_CODEC_XPCM_PM3_ADDR
    #define    MST_CODEC_XPCM_PM3_ADDR              0
    #define    MST_CODEC_XPCM_PM3_SIZE              0
    #define    mst_codec_xpcm_pm3                   0
#endif
#ifndef    MST_CODEC_XPCM_PM4_ADDR
    #define    MST_CODEC_XPCM_PM4_ADDR              0
    #define    MST_CODEC_XPCM_PM4_SIZE              0
    #define    mst_codec_xpcm_pm4                   0
#endif
#ifndef    MST_CODEC_MP3_PM1_ADDR
    #define    MST_CODEC_MP3_PM1_ADDR               0
    #define    MST_CODEC_MP3_PM1_SIZE               0
    #define    mst_codec_mp3_pm1                    0
#endif
#ifndef    MST_CODEC_MP3_PM2_ADDR
    #define    MST_CODEC_MP3_PM2_ADDR               0
    #define    MST_CODEC_MP3_PM2_SIZE               0
    #define    mst_codec_mp3_pm2                    0
#endif
#ifndef    MST_CODEC_MP3_PM3_ADDR
    #define    MST_CODEC_MP3_PM3_ADDR               0
    #define    MST_CODEC_MP3_PM3_SIZE               0
    #define    mst_codec_mp3_pm3                    0
#endif
#ifndef    MST_CODEC_MP3_PM4_ADDR
    #define    MST_CODEC_MP3_PM4_ADDR               0
    #define    MST_CODEC_MP3_PM4_SIZE               0
    #define    mst_codec_mp3_pm4                    0
#endif
#ifndef    MST_CODEC_MPEG_PM1_ADDR
    #define    MST_CODEC_MPEG_PM1_ADDR              0
    #define    MST_CODEC_MPEG_PM1_SIZE              0
    #define    mst_codec_mpeg_pm1                   0
#endif
#ifndef    MST_CODEC_MPEG_PM2_ADDR
    #define    MST_CODEC_MPEG_PM2_ADDR              0
    #define    MST_CODEC_MPEG_PM2_SIZE              0
    #define    mst_codec_mpeg_pm2                   0
#endif
#ifndef    MST_CODEC_MPEG_PM3_ADDR
    #define    MST_CODEC_MPEG_PM3_ADDR              0
    #define    MST_CODEC_MPEG_PM3_SIZE              0
    #define    mst_codec_mpeg_pm3                   0
#endif
#ifndef    MST_CODEC_MPEG_PM4_ADDR
    #define    MST_CODEC_MPEG_PM4_ADDR              0
    #define    MST_CODEC_MPEG_PM4_SIZE              0
    #define    mst_codec_mpeg_pm4                   0
#endif
#ifndef    MST_CODEC_AC3P_PM1_ADDR
    #define    MST_CODEC_AC3P_PM1_ADDR              0
    #define    MST_CODEC_AC3P_PM1_SIZE              0
    #define    mst_codec_ac3p_pm1                   0
#endif
#ifndef    MST_CODEC_AC3P_PM2_ADDR
    #define    MST_CODEC_AC3P_PM2_ADDR              0
    #define    MST_CODEC_AC3P_PM2_SIZE              0
    #define    mst_codec_ac3p_pm2                   0
#endif
#ifndef    MST_CODEC_AC3P_PM3_ADDR
    #define    MST_CODEC_AC3P_PM3_ADDR              0
    #define    MST_CODEC_AC3P_PM3_SIZE              0
    #define    mst_codec_ac3p_pm3                   0
#endif
#ifndef    MST_CODEC_AC3P_PM4_ADDR
    #define    MST_CODEC_AC3P_PM4_ADDR              0
    #define    MST_CODEC_AC3P_PM4_SIZE              0
    #define    mst_codec_ac3p_pm4                   0
#endif
#ifndef    MST_CODEC_MS10_DDT_PM1_ADDR
    #define    MST_CODEC_MS10_DDT_PM1_ADDR          0
    #define    MST_CODEC_MS10_DDT_PM1_SIZE          0
    #define    mst_codec_ms10_ddt_pm1               0
#endif
#ifndef    MST_CODEC_MS10_DDT_PM2_ADDR
    #define    MST_CODEC_MS10_DDT_PM2_ADDR          0
    #define    MST_CODEC_MS10_DDT_PM2_SIZE          0
    #define    mst_codec_ms10_ddt_pm2               0
#endif
#ifndef    MST_CODEC_MS10_DDT_PM3_ADDR
    #define    MST_CODEC_MS10_DDT_PM3_ADDR          0
    #define    MST_CODEC_MS10_DDT_PM3_SIZE          0
    #define    mst_codec_ms10_ddt_pm3               0
#endif
#ifndef    MST_CODEC_MS10_DDT_PM4_ADDR
    #define    MST_CODEC_MS10_DDT_PM4_ADDR          0
    #define    MST_CODEC_MS10_DDT_PM4_SIZE          0
    #define    mst_codec_ms10_ddt_pm4               0
#endif
#ifndef    MST_CODEC_GAAC_PM1_ADDR
    #define    MST_CODEC_GAAC_PM1_ADDR              0
    #define    MST_CODEC_GAAC_PM1_SIZE              0
    #define    mst_codec_gaac_pm1                   0
#endif
#ifndef    MST_CODEC_GAAC_PM2_ADDR
    #define    MST_CODEC_GAAC_PM2_ADDR              0
    #define    MST_CODEC_GAAC_PM2_SIZE              0
    #define    mst_codec_gaac_pm2                   0
#endif
#ifndef    MST_CODEC_GAAC_PM3_ADDR
    #define    MST_CODEC_GAAC_PM3_ADDR              0
    #define    MST_CODEC_GAAC_PM3_SIZE              0
    #define    mst_codec_gaac_pm3                   0
#endif
#ifndef    MST_CODEC_GAAC_PM4_ADDR
    #define    MST_CODEC_GAAC_PM4_ADDR              0
    #define    MST_CODEC_GAAC_PM4_SIZE              0
    #define    mst_codec_gaac_pm4                   0
#endif
#ifndef    MST_CODEC_WMA_PRO_PM1_ADDR
    #define    MST_CODEC_WMA_PRO_PM1_ADDR           0
    #define    MST_CODEC_WMA_PRO_PM1_SIZE           0
    #define    mst_codec_wma_pro_pm1                0
#endif
#ifndef    MST_CODEC_WMA_PRO_PM2_ADDR
    #define    MST_CODEC_WMA_PRO_PM2_ADDR           0
    #define    MST_CODEC_WMA_PRO_PM2_SIZE           0
    #define    mst_codec_wma_pro_pm2                0
#endif
#ifndef    MST_CODEC_WMA_PRO_PM3_ADDR
    #define    MST_CODEC_WMA_PRO_PM3_ADDR           0
    #define    MST_CODEC_WMA_PRO_PM3_SIZE           0
    #define    mst_codec_wma_pro_pm3                0
#endif
#ifndef    MST_CODEC_WMA_PRO_PM4_ADDR
    #define    MST_CODEC_WMA_PRO_PM4_ADDR           0
    #define    MST_CODEC_WMA_PRO_PM4_SIZE           0
    #define    mst_codec_wma_pro_pm4                0
#endif
#ifndef    MST_CODEC_DTS_DMP_PM1_ADDR
    #define    MST_CODEC_DTS_DMP_PM1_ADDR           0
    #define    MST_CODEC_DTS_DMP_PM1_SIZE           0
    #define    mst_codec_dts_dmp_pm1                0
#endif
#ifndef    MST_CODEC_DTS_DMP_PM2_ADDR
    #define    MST_CODEC_DTS_DMP_PM2_ADDR           0
    #define    MST_CODEC_DTS_DMP_PM2_SIZE           0
    #define    mst_codec_dts_dmp_pm2                0
#endif
#ifndef    MST_CODEC_DTS_DMP_PM3_ADDR
    #define    MST_CODEC_DTS_DMP_PM3_ADDR           0
    #define    MST_CODEC_DTS_DMP_PM3_SIZE           0
    #define    mst_codec_dts_dmp_pm3                0
#endif
#ifndef    MST_CODEC_DTS_DMP_PM4_ADDR
    #define    MST_CODEC_DTS_DMP_PM4_ADDR           0
    #define    MST_CODEC_DTS_DMP_PM4_SIZE           0
    #define    mst_codec_dts_dmp_pm4                0
#endif
#ifndef    MST_CODEC_DTS_LBR_PM1_ADDR
    #define    MST_CODEC_DTS_LBR_PM1_ADDR           0
    #define    MST_CODEC_DTS_LBR_PM1_SIZE           0
    #define    mst_codec_dts_lbr_pm1                0
#endif
#ifndef    MST_CODEC_DTS_LBR_PM2_ADDR
    #define    MST_CODEC_DTS_LBR_PM2_ADDR           0
    #define    MST_CODEC_DTS_LBR_PM2_SIZE           0
    #define    mst_codec_dts_lbr_pm2                0
#endif
#ifndef    MST_CODEC_DTS_LBR_PM3_ADDR
    #define    MST_CODEC_DTS_LBR_PM3_ADDR           0
    #define    MST_CODEC_DTS_LBR_PM3_SIZE           0
    #define    mst_codec_dts_lbr_pm3                0
#endif
#ifndef    MST_CODEC_DTS_LBR_PM4_ADDR
    #define    MST_CODEC_DTS_LBR_PM4_ADDR           0
    #define    MST_CODEC_DTS_LBR_PM4_SIZE           0
    #define    mst_codec_dts_lbr_pm4                0
#endif
#ifndef    MST_CODEC_RA8LBR_PM1_ADDR
    #define    MST_CODEC_RA8LBR_PM1_ADDR            0
    #define    MST_CODEC_RA8LBR_PM1_SIZE            0
    #define    mst_codec_ra8lbr_pm1                 0
#endif
#ifndef    MST_CODEC_RA8LBR_PM2_ADDR
    #define    MST_CODEC_RA8LBR_PM2_ADDR            0
    #define    MST_CODEC_RA8LBR_PM2_SIZE            0
    #define    mst_codec_ra8lbr_pm2                 0
#endif
#ifndef    MST_CODEC_RA8LBR_PM3_ADDR
    #define    MST_CODEC_RA8LBR_PM3_ADDR            0
    #define    MST_CODEC_RA8LBR_PM3_SIZE            0
    #define    mst_codec_ra8lbr_pm3                 0
#endif
#ifndef    MST_CODEC_RA8LBR_PM4_ADDR
    #define    MST_CODEC_RA8LBR_PM4_ADDR            0
    #define    MST_CODEC_RA8LBR_PM4_SIZE            0
    #define    mst_codec_ra8lbr_pm4                 0
#endif
#ifndef    MST_CODEC_VORBIS_PM1_ADDR
    #define    MST_CODEC_VORBIS_PM1_ADDR            0
    #define    MST_CODEC_VORBIS_PM1_SIZE            0
    #define    mst_codec_vorbis_pm1                 0
#endif
#ifndef    MST_CODEC_VORBIS_PM2_ADDR
    #define    MST_CODEC_VORBIS_PM2_ADDR            0
    #define    MST_CODEC_VORBIS_PM2_SIZE            0
    #define    mst_codec_vorbis_pm2                 0
#endif
#ifndef    MST_CODEC_VORBIS_PM3_ADDR
    #define    MST_CODEC_VORBIS_PM3_ADDR            0
    #define    MST_CODEC_VORBIS_PM3_SIZE            0
    #define    mst_codec_vorbis_pm3                 0
#endif
#ifndef    MST_CODEC_VORBIS_PM4_ADDR
    #define    MST_CODEC_VORBIS_PM4_ADDR            0
    #define    MST_CODEC_VORBIS_PM4_SIZE            0
    #define    mst_codec_vorbis_pm4                 0
#endif
#ifndef    MST_CODEC_AMR_NB_PM1_ADDR
    #define    MST_CODEC_AMR_NB_PM1_ADDR            0
    #define    MST_CODEC_AMR_NB_PM1_SIZE            0
    #define    mst_codec_amr_nb_pm1                 0
#endif
#ifndef    MST_CODEC_AMR_NB_PM2_ADDR
    #define    MST_CODEC_AMR_NB_PM2_ADDR            0
    #define    MST_CODEC_AMR_NB_PM2_SIZE            0
    #define    mst_codec_amr_nb_pm2                 0
#endif
#ifndef    MST_CODEC_AMR_NB_PM3_ADDR
    #define    MST_CODEC_AMR_NB_PM3_ADDR            0
    #define    MST_CODEC_AMR_NB_PM3_SIZE            0
    #define    mst_codec_amr_nb_pm3                 0
#endif
#ifndef    MST_CODEC_AMR_NB_PM4_ADDR
    #define    MST_CODEC_AMR_NB_PM4_ADDR            0
    #define    MST_CODEC_AMR_NB_PM4_SIZE            0
    #define    mst_codec_amr_nb_pm4                 0
#endif
#ifndef    MST_CODEC_AMR_WB_PM1_ADDR
    #define    MST_CODEC_AMR_WB_PM1_ADDR            0
    #define    MST_CODEC_AMR_WB_PM1_SIZE            0
    #define    mst_codec_amr_wb_pm1                 0
#endif
#ifndef    MST_CODEC_AMR_WB_PM2_ADDR
    #define    MST_CODEC_AMR_WB_PM2_ADDR            0
    #define    MST_CODEC_AMR_WB_PM2_SIZE            0
    #define    mst_codec_amr_wb_pm2                 0
#endif
#ifndef    MST_CODEC_AMR_WB_PM3_ADDR
    #define    MST_CODEC_AMR_WB_PM3_ADDR            0
    #define    MST_CODEC_AMR_WB_PM3_SIZE            0
    #define    mst_codec_amr_wb_pm3                 0
#endif
#ifndef    MST_CODEC_AMR_WB_PM4_ADDR
    #define    MST_CODEC_AMR_WB_PM4_ADDR            0
    #define    MST_CODEC_AMR_WB_PM4_SIZE            0
    #define    mst_codec_amr_wb_pm4                 0
#endif
#ifndef    MST_CODEC_DRAD_PM1_ADDR
    #define    MST_CODEC_DRAD_PM1_ADDR              0
    #define    MST_CODEC_DRAD_PM1_SIZE              0
    #define    mst_codec_drad_pm1                   0
#endif
#ifndef    MST_CODEC_DRAD_PM2_ADDR
    #define    MST_CODEC_DRAD_PM2_ADDR              0
    #define    MST_CODEC_DRAD_PM2_SIZE              0
    #define    mst_codec_drad_pm2                   0
#endif
#ifndef    MST_CODEC_DRAD_PM3_ADDR
    #define    MST_CODEC_DRAD_PM3_ADDR              0
    #define    MST_CODEC_DRAD_PM3_SIZE              0
    #define    mst_codec_drad_pm3                   0
#endif
#ifndef    MST_CODEC_DRAD_PM4_ADDR
    #define    MST_CODEC_DRAD_PM4_ADDR              0
    #define    MST_CODEC_DRAD_PM4_SIZE              0
    #define    mst_codec_drad_pm4                   0
#endif
#ifndef    MST_CODEC_FLAC_PM1_ADDR
    #define    MST_CODEC_FLAC_PM1_ADDR              0
    #define    MST_CODEC_FLAC_PM1_SIZE              0
    #define    mst_codec_flac_pm1                   0
#endif
#ifndef    MST_CODEC_FLAC_PM2_ADDR
    #define    MST_CODEC_FLAC_PM2_ADDR              0
    #define    MST_CODEC_FLAC_PM2_SIZE              0
    #define    mst_codec_flac_pm2                   0
#endif
#ifndef    MST_CODEC_FLAC_PM3_ADDR
    #define    MST_CODEC_FLAC_PM3_ADDR              0
    #define    MST_CODEC_FLAC_PM3_SIZE              0
    #define    mst_codec_flac_pm3                   0
#endif
#ifndef    MST_CODEC_FLAC_PM4_ADDR
    #define    MST_CODEC_FLAC_PM4_ADDR              0
    #define    MST_CODEC_FLAC_PM4_SIZE              0
    #define    mst_codec_flac_pm4                   0
#endif
#ifndef    MST_CODEC_PALSUM_PM1_ADDR
    #define    MST_CODEC_PALSUM_PM1_ADDR            0
    #define    MST_CODEC_PALSUM_PM1_SIZE            0
    #define    mst_codec_palsum_pm1                 0
#endif
#ifndef    MST_CODEC_PALSUM_PM2_ADDR
    #define    MST_CODEC_PALSUM_PM2_ADDR            0
    #define    MST_CODEC_PALSUM_PM2_SIZE            0
    #define    mst_codec_palsum_pm2                 0
#endif
#ifndef    MST_CODEC_PALSUM_PM3_ADDR
    #define    MST_CODEC_PALSUM_PM3_ADDR            0
    #define    MST_CODEC_PALSUM_PM3_SIZE            0
    #define    mst_codec_palsum_pm3                 0
#endif
#ifndef    MST_CODEC_PALSUM_PM4_ADDR
    #define    MST_CODEC_PALSUM_PM4_ADDR            0
    #define    MST_CODEC_PALSUM_PM4_SIZE            0
    #define    mst_codec_palsum_pm4                 0
#endif
#ifndef    MST_CODEC_BTSC_PM1_ADDR
    #define    MST_CODEC_BTSC_PM1_ADDR              0
    #define    MST_CODEC_BTSC_PM1_SIZE              0
    #define    mst_codec_btsc_pm1                   0
#endif
#ifndef    MST_CODEC_BTSC_PM2_ADDR
    #define    MST_CODEC_BTSC_PM2_ADDR              0
    #define    MST_CODEC_BTSC_PM2_SIZE              0
    #define    mst_codec_btsc_pm2                   0
#endif
#ifndef    MST_CODEC_BTSC_PM3_ADDR
    #define    MST_CODEC_BTSC_PM3_ADDR              0
    #define    MST_CODEC_BTSC_PM3_SIZE              0
    #define    mst_codec_btsc_pm3                   0
#endif
#ifndef    MST_CODEC_BTSC_PM4_ADDR
    #define    MST_CODEC_BTSC_PM4_ADDR              0
    #define    MST_CODEC_BTSC_PM4_SIZE              0
    #define    mst_codec_btsc_pm4                   0
#endif
#ifndef    MST_CODEC_TSHD_PM1_ADDR
    #define    MST_CODEC_TSHD_PM1_ADDR              0
    #define    MST_CODEC_TSHD_PM1_SIZE              0
    #define    mst_codec_tshd_pm1                   0
#endif
#ifndef    MST_CODEC_TSHD_PM2_ADDR
    #define    MST_CODEC_TSHD_PM2_ADDR              0
    #define    MST_CODEC_TSHD_PM2_SIZE              0
    #define    mst_codec_tshd_pm2                   0
#endif
#ifndef    MST_CODEC_TSHD_PM3_ADDR
    #define    MST_CODEC_TSHD_PM3_ADDR              0
    #define    MST_CODEC_TSHD_PM3_SIZE              0
    #define    mst_codec_tshd_pm3                   0
#endif
#ifndef    MST_CODEC_TSHD_PM4_ADDR
    #define    MST_CODEC_TSHD_PM4_ADDR              0
    #define    MST_CODEC_TSHD_PM4_SIZE              0
    #define    mst_codec_tshd_pm4                   0
#endif
#ifndef    MST_CODEC_TSXT_PM1_ADDR
    #define    MST_CODEC_TSXT_PM1_ADDR              0
    #define    MST_CODEC_TSXT_PM1_SIZE              0
    #define    mst_codec_tsxt_pm1                   0
#endif
#ifndef    MST_CODEC_TSXT_PM2_ADDR
    #define    MST_CODEC_TSXT_PM2_ADDR              0
    #define    MST_CODEC_TSXT_PM2_SIZE              0
    #define    mst_codec_tsxt_pm2                   0
#endif
#ifndef    MST_CODEC_TSXT_PM3_ADDR
    #define    MST_CODEC_TSXT_PM3_ADDR              0
    #define    MST_CODEC_TSXT_PM3_SIZE              0
    #define    mst_codec_tsxt_pm3                   0
#endif
#ifndef    MST_CODEC_TSXT_PM4_ADDR
    #define    MST_CODEC_TSXT_PM4_ADDR              0
    #define    MST_CODEC_TSXT_PM4_SIZE              0
    #define    mst_codec_tsxt_pm4                   0
#endif
#ifndef    MST_CODEC_PURESND_PM1_ADDR
    #define    MST_CODEC_PURESND_PM1_ADDR           0
    #define    MST_CODEC_PURESND_PM1_SIZE           0
    #define    mst_codec_puresnd_pm1                0
#endif
#ifndef    MST_CODEC_PURESND_PM2_ADDR
    #define    MST_CODEC_PURESND_PM2_ADDR           0
    #define    MST_CODEC_PURESND_PM2_SIZE           0
    #define    mst_codec_puresnd_pm2                0
#endif
#ifndef    MST_CODEC_PURESND_PM3_ADDR
    #define    MST_CODEC_PURESND_PM3_ADDR           0
    #define    MST_CODEC_PURESND_PM3_SIZE           0
    #define    mst_codec_puresnd_pm3                0
#endif
#ifndef    MST_CODEC_PURESND_PM4_ADDR
    #define    MST_CODEC_PURESND_PM4_ADDR           0
    #define    MST_CODEC_PURESND_PM4_SIZE           0
    #define    mst_codec_puresnd_pm4                0
#endif
#ifndef    MST_CODEC_DEC_PM1_ADDR
    #define    MST_CODEC_DEC_PM1_ADDR               0
    #define    MST_CODEC_DEC_PM1_SIZE               0
    #define    mst_codec_dec_pm1                    0
#endif
#ifndef    MST_CODEC_DEC_PM2_ADDR
    #define    MST_CODEC_DEC_PM2_ADDR               0
    #define    MST_CODEC_DEC_PM2_SIZE               0
    #define    mst_codec_dec_pm2                    0
#endif
#ifndef    MST_CODEC_DEC_PM3_ADDR
    #define    MST_CODEC_DEC_PM3_ADDR               0
    #define    MST_CODEC_DEC_PM3_SIZE               0
    #define    mst_codec_dec_pm3                    0
#endif
#ifndef    MST_CODEC_DEC_PM4_ADDR
    #define    MST_CODEC_DEC_PM4_ADDR               0
    #define    MST_CODEC_DEC_PM4_SIZE               0
    #define    mst_codec_dec_pm4                    0
#endif
#ifndef    MST_CODEC_DDE_PM1_ADDR
    #define    MST_CODEC_DDE_PM1_ADDR               0
    #define    MST_CODEC_DDE_PM1_SIZE               0
    #define    mst_codec_dde_pm1                    0
#endif
#ifndef    MST_CODEC_DDE_PM2_ADDR
    #define    MST_CODEC_DDE_PM2_ADDR               0
    #define    MST_CODEC_DDE_PM2_SIZE               0
    #define    mst_codec_dde_pm2                    0
#endif
#ifndef    MST_CODEC_DDE_PM3_ADDR
    #define    MST_CODEC_DDE_PM3_ADDR               0
    #define    MST_CODEC_DDE_PM3_SIZE               0
    #define    mst_codec_dde_pm3                    0
#endif
#ifndef    MST_CODEC_DDE_PM4_ADDR
    #define    MST_CODEC_DDE_PM4_ADDR               0
    #define    MST_CODEC_DDE_PM4_SIZE               0
    #define    mst_codec_dde_pm4                    0
#endif

#ifndef    MST_CODEC_DBX_PM1_ADDR
    #define    MST_CODEC_DBX_PM1_ADDR           0
    #define    MST_CODEC_DBX_PM1_SIZE           0
    #define    mst_codec_dbx_pm1                0
#endif
#ifndef    MST_CODEC_DBX_PM2_ADDR
    #define    MST_CODEC_DBX_PM2_ADDR           0
    #define    MST_CODEC_DBX_PM2_SIZE           0
    #define    mst_codec_dbx_pm2                0
#endif
#ifndef    MST_CODEC_DBX_PM3_ADDR
    #define    MST_CODEC_DBX_PM3_ADDR           0
    #define    MST_CODEC_DBX_PM3_SIZE           0
    #define    mst_codec_dbx_pm3                0
#endif
#ifndef    MST_CODEC_DBX_PM4_ADDR
    #define    MST_CODEC_DBX_PM4_ADDR           0
    #define    MST_CODEC_DBX_PM4_SIZE           0
    #define    mst_codec_dbx_pm4                0
#endif


//===========================================================================
// DSP Load code Table
//===========================================================================    
AUDIO_ALG_INFO    SE_SYSTEM_LoadCodeTable[ ]=
{        {
        MST_CODEC_PM1_ADDR, MST_CODEC_PM1_SIZE, mst_codec_pm1,
        MST_CODEC_PM2_ADDR, MST_CODEC_PM2_SIZE, mst_codec_pm2,
        MST_CODEC_PM3_ADDR, MST_CODEC_PM3_SIZE, mst_codec_pm3,
        MST_CODEC_PM4_ADDR, MST_CODEC_PM4_SIZE, mst_codec_pm4,
        "SE_SYSTEM",},
};

//==============================================
//==============================================
AUDIO_ALG_INFO    XPCM_LoadCodeTable[ ]=
{        {
        MST_CODEC_XPCM_PM1_ADDR, MST_CODEC_XPCM_PM1_SIZE, mst_codec_xpcm_pm1,
        MST_CODEC_XPCM_PM2_ADDR, MST_CODEC_XPCM_PM2_SIZE, mst_codec_xpcm_pm2,
        MST_CODEC_XPCM_PM3_ADDR, MST_CODEC_XPCM_PM3_SIZE, mst_codec_xpcm_pm3,
        MST_CODEC_XPCM_PM4_ADDR, MST_CODEC_XPCM_PM4_SIZE, mst_codec_xpcm_pm4,
        "XPCM",},
};
//==============================================
AUDIO_ALG_INFO    MP3_LoadCodeTable[ ]=
{        {
        MST_CODEC_MP3_PM1_ADDR, MST_CODEC_MP3_PM1_SIZE, mst_codec_mp3_pm1,
        MST_CODEC_MP3_PM2_ADDR, MST_CODEC_MP3_PM2_SIZE, mst_codec_mp3_pm2,
        MST_CODEC_MP3_PM3_ADDR, MST_CODEC_MP3_PM3_SIZE, mst_codec_mp3_pm3,
        MST_CODEC_MP3_PM4_ADDR, MST_CODEC_MP3_PM4_SIZE, mst_codec_mp3_pm4,
        "MP3",},
};
//==============================================
AUDIO_ALG_INFO    MPEG_LoadCodeTable[ ]=
{        {
        MST_CODEC_MPEG_PM1_ADDR, MST_CODEC_MPEG_PM1_SIZE, mst_codec_mpeg_pm1,
        MST_CODEC_MPEG_PM2_ADDR, MST_CODEC_MPEG_PM2_SIZE, mst_codec_mpeg_pm2,
        MST_CODEC_MPEG_PM3_ADDR, MST_CODEC_MPEG_PM3_SIZE, mst_codec_mpeg_pm3,
        MST_CODEC_MPEG_PM4_ADDR, MST_CODEC_MPEG_PM4_SIZE, mst_codec_mpeg_pm4,
        "MPEG",},
};
//==============================================
AUDIO_ALG_INFO    AC3P_LoadCodeTable[ ]=
{        {
        MST_CODEC_AC3P_PM1_ADDR, MST_CODEC_AC3P_PM1_SIZE, mst_codec_ac3p_pm1,
        MST_CODEC_AC3P_PM2_ADDR, MST_CODEC_AC3P_PM2_SIZE, mst_codec_ac3p_pm2,
        MST_CODEC_AC3P_PM3_ADDR, MST_CODEC_AC3P_PM3_SIZE, mst_codec_ac3p_pm3,
        MST_CODEC_AC3P_PM4_ADDR, MST_CODEC_AC3P_PM4_SIZE, mst_codec_ac3p_pm4,
        "AC3P",},
};
//==============================================
AUDIO_ALG_INFO    MS10_DDT_LoadCodeTable[ ]=
{        {
        MST_CODEC_MS10_DDT_PM1_ADDR, MST_CODEC_MS10_DDT_PM1_SIZE, mst_codec_ms10_ddt_pm1,
        MST_CODEC_MS10_DDT_PM2_ADDR, MST_CODEC_MS10_DDT_PM2_SIZE, mst_codec_ms10_ddt_pm2,
        MST_CODEC_MS10_DDT_PM3_ADDR, MST_CODEC_MS10_DDT_PM3_SIZE, mst_codec_ms10_ddt_pm3,
        MST_CODEC_MS10_DDT_PM4_ADDR, MST_CODEC_MS10_DDT_PM4_SIZE, mst_codec_ms10_ddt_pm4,
        "MS10_DDT",},
};
//==============================================
AUDIO_ALG_INFO    GAAC_LoadCodeTable[ ]=
{        {
        MST_CODEC_GAAC_PM1_ADDR, MST_CODEC_GAAC_PM1_SIZE, mst_codec_gaac_pm1,
        MST_CODEC_GAAC_PM2_ADDR, MST_CODEC_GAAC_PM2_SIZE, mst_codec_gaac_pm2,
        MST_CODEC_GAAC_PM3_ADDR, MST_CODEC_GAAC_PM3_SIZE, mst_codec_gaac_pm3,
        MST_CODEC_GAAC_PM4_ADDR, MST_CODEC_GAAC_PM4_SIZE, mst_codec_gaac_pm4,
        "GAAC",},
};
//==============================================
AUDIO_ALG_INFO    WMA_PRO_LoadCodeTable[ ]=
{        {
        MST_CODEC_WMA_PRO_PM1_ADDR, MST_CODEC_WMA_PRO_PM1_SIZE, mst_codec_wma_pro_pm1,
        MST_CODEC_WMA_PRO_PM2_ADDR, MST_CODEC_WMA_PRO_PM2_SIZE, mst_codec_wma_pro_pm2,
        MST_CODEC_WMA_PRO_PM3_ADDR, MST_CODEC_WMA_PRO_PM3_SIZE, mst_codec_wma_pro_pm3,
        MST_CODEC_WMA_PRO_PM4_ADDR, MST_CODEC_WMA_PRO_PM4_SIZE, mst_codec_wma_pro_pm4,
        "WMA_PRO",},
};
//==============================================
AUDIO_ALG_INFO    DTS_DMP_LoadCodeTable[ ]=
{        {
        MST_CODEC_DTS_DMP_PM1_ADDR, MST_CODEC_DTS_DMP_PM1_SIZE, mst_codec_dts_dmp_pm1,
        MST_CODEC_DTS_DMP_PM2_ADDR, MST_CODEC_DTS_DMP_PM2_SIZE, mst_codec_dts_dmp_pm2,
        MST_CODEC_DTS_DMP_PM3_ADDR, MST_CODEC_DTS_DMP_PM3_SIZE, mst_codec_dts_dmp_pm3,
        MST_CODEC_DTS_DMP_PM4_ADDR, MST_CODEC_DTS_DMP_PM4_SIZE, mst_codec_dts_dmp_pm4,
        "DTS_DMP",},
};
//==============================================
AUDIO_ALG_INFO    DTS_LBR_LoadCodeTable[ ]=
{        {
        MST_CODEC_DTS_LBR_PM1_ADDR, MST_CODEC_DTS_LBR_PM1_SIZE, mst_codec_dts_lbr_pm1,
        MST_CODEC_DTS_LBR_PM2_ADDR, MST_CODEC_DTS_LBR_PM2_SIZE, mst_codec_dts_lbr_pm2,
        MST_CODEC_DTS_LBR_PM3_ADDR, MST_CODEC_DTS_LBR_PM3_SIZE, mst_codec_dts_lbr_pm3,
        MST_CODEC_DTS_LBR_PM4_ADDR, MST_CODEC_DTS_LBR_PM4_SIZE, mst_codec_dts_lbr_pm4,
        "DTS_LBR",},
};
//==============================================
AUDIO_ALG_INFO    RA8LBR_LoadCodeTable[ ]=
{        {
        MST_CODEC_RA8LBR_PM1_ADDR, MST_CODEC_RA8LBR_PM1_SIZE, mst_codec_ra8lbr_pm1,
        MST_CODEC_RA8LBR_PM2_ADDR, MST_CODEC_RA8LBR_PM2_SIZE, mst_codec_ra8lbr_pm2,
        MST_CODEC_RA8LBR_PM3_ADDR, MST_CODEC_RA8LBR_PM3_SIZE, mst_codec_ra8lbr_pm3,
        MST_CODEC_RA8LBR_PM4_ADDR, MST_CODEC_RA8LBR_PM4_SIZE, mst_codec_ra8lbr_pm4,
        "RA8LBR",},
};
//==============================================
AUDIO_ALG_INFO    VORBIS_LoadCodeTable[ ]=
{        {
        MST_CODEC_VORBIS_PM1_ADDR, MST_CODEC_VORBIS_PM1_SIZE, mst_codec_vorbis_pm1,
        MST_CODEC_VORBIS_PM2_ADDR, MST_CODEC_VORBIS_PM2_SIZE, mst_codec_vorbis_pm2,
        MST_CODEC_VORBIS_PM3_ADDR, MST_CODEC_VORBIS_PM3_SIZE, mst_codec_vorbis_pm3,
        MST_CODEC_VORBIS_PM4_ADDR, MST_CODEC_VORBIS_PM4_SIZE, mst_codec_vorbis_pm4,
        "VORBIS",},
};
//==============================================
AUDIO_ALG_INFO    AMR_NB_LoadCodeTable[ ]=
{        {
        MST_CODEC_AMR_NB_PM1_ADDR, MST_CODEC_AMR_NB_PM1_SIZE, mst_codec_amr_nb_pm1,
        MST_CODEC_AMR_NB_PM2_ADDR, MST_CODEC_AMR_NB_PM2_SIZE, mst_codec_amr_nb_pm2,
        MST_CODEC_AMR_NB_PM3_ADDR, MST_CODEC_AMR_NB_PM3_SIZE, mst_codec_amr_nb_pm3,
        MST_CODEC_AMR_NB_PM4_ADDR, MST_CODEC_AMR_NB_PM4_SIZE, mst_codec_amr_nb_pm4,
        "AMR_NB",},
};
//==============================================
AUDIO_ALG_INFO    AMR_WB_LoadCodeTable[ ]=
{        {
        MST_CODEC_AMR_WB_PM1_ADDR, MST_CODEC_AMR_WB_PM1_SIZE, mst_codec_amr_wb_pm1,
        MST_CODEC_AMR_WB_PM2_ADDR, MST_CODEC_AMR_WB_PM2_SIZE, mst_codec_amr_wb_pm2,
        MST_CODEC_AMR_WB_PM3_ADDR, MST_CODEC_AMR_WB_PM3_SIZE, mst_codec_amr_wb_pm3,
        MST_CODEC_AMR_WB_PM4_ADDR, MST_CODEC_AMR_WB_PM4_SIZE, mst_codec_amr_wb_pm4,
        "AMR_WB",},
};
//==============================================
AUDIO_ALG_INFO    DRAD_LoadCodeTable[ ]=
{        {
        MST_CODEC_DRAD_PM1_ADDR, MST_CODEC_DRAD_PM1_SIZE, mst_codec_drad_pm1,
        MST_CODEC_DRAD_PM2_ADDR, MST_CODEC_DRAD_PM2_SIZE, mst_codec_drad_pm2,
        MST_CODEC_DRAD_PM3_ADDR, MST_CODEC_DRAD_PM3_SIZE, mst_codec_drad_pm3,
        MST_CODEC_DRAD_PM4_ADDR, MST_CODEC_DRAD_PM4_SIZE, mst_codec_drad_pm4,
        "DRAD",},
};
//==============================================
AUDIO_ALG_INFO    FLAC_LoadCodeTable[ ]=
{        {
        MST_CODEC_FLAC_PM1_ADDR, MST_CODEC_FLAC_PM1_SIZE, mst_codec_flac_pm1,
        MST_CODEC_FLAC_PM2_ADDR, MST_CODEC_FLAC_PM2_SIZE, mst_codec_flac_pm2,
        MST_CODEC_FLAC_PM3_ADDR, MST_CODEC_FLAC_PM3_SIZE, mst_codec_flac_pm3,
        MST_CODEC_FLAC_PM4_ADDR, MST_CODEC_FLAC_PM4_SIZE, mst_codec_flac_pm4,
        "FLAC",},
};
//==============================================
AUDIO_ALG_INFO    PALSUM_LoadCodeTable[ ]=
{        {
        MST_CODEC_PALSUM_PM1_ADDR, MST_CODEC_PALSUM_PM1_SIZE, mst_codec_palsum_pm1,
        MST_CODEC_PALSUM_PM2_ADDR, MST_CODEC_PALSUM_PM2_SIZE, mst_codec_palsum_pm2,
        MST_CODEC_PALSUM_PM3_ADDR, MST_CODEC_PALSUM_PM3_SIZE, mst_codec_palsum_pm3,
        MST_CODEC_PALSUM_PM4_ADDR, MST_CODEC_PALSUM_PM4_SIZE, mst_codec_palsum_pm4,
        "PALSUM",},
};
//==============================================
AUDIO_ALG_INFO    BTSC_LoadCodeTable[ ]=
{        {
        MST_CODEC_BTSC_PM1_ADDR, MST_CODEC_BTSC_PM1_SIZE, mst_codec_btsc_pm1,
        MST_CODEC_BTSC_PM2_ADDR, MST_CODEC_BTSC_PM2_SIZE, mst_codec_btsc_pm2,
        MST_CODEC_BTSC_PM3_ADDR, MST_CODEC_BTSC_PM3_SIZE, mst_codec_btsc_pm3,
        MST_CODEC_BTSC_PM4_ADDR, MST_CODEC_BTSC_PM4_SIZE, mst_codec_btsc_pm4,
        "BTSC",},
};
//==============================================
AUDIO_ALG_INFO    TSHD_LoadCodeTable[ ]=
{        {
        MST_CODEC_TSHD_PM1_ADDR, MST_CODEC_TSHD_PM1_SIZE, mst_codec_tshd_pm1,
        MST_CODEC_TSHD_PM2_ADDR, MST_CODEC_TSHD_PM2_SIZE, mst_codec_tshd_pm2,
        MST_CODEC_TSHD_PM3_ADDR, MST_CODEC_TSHD_PM3_SIZE, mst_codec_tshd_pm3,
        MST_CODEC_TSHD_PM4_ADDR, MST_CODEC_TSHD_PM4_SIZE, mst_codec_tshd_pm4,
        "TSHD",},
};
//==============================================
AUDIO_ALG_INFO    TSXT_LoadCodeTable[ ]=
{        {
        MST_CODEC_TSXT_PM1_ADDR, MST_CODEC_TSXT_PM1_SIZE, mst_codec_tsxt_pm1,
        MST_CODEC_TSXT_PM2_ADDR, MST_CODEC_TSXT_PM2_SIZE, mst_codec_tsxt_pm2,
        MST_CODEC_TSXT_PM3_ADDR, MST_CODEC_TSXT_PM3_SIZE, mst_codec_tsxt_pm3,
        MST_CODEC_TSXT_PM4_ADDR, MST_CODEC_TSXT_PM4_SIZE, mst_codec_tsxt_pm4,
        "TSXT",},
};
//==============================================
AUDIO_ALG_INFO    PURESND_LoadCodeTable[ ]=
{        {
        MST_CODEC_PURESND_PM1_ADDR, MST_CODEC_PURESND_PM1_SIZE, mst_codec_puresnd_pm1,
        MST_CODEC_PURESND_PM2_ADDR, MST_CODEC_PURESND_PM2_SIZE, mst_codec_puresnd_pm2,
        MST_CODEC_PURESND_PM3_ADDR, MST_CODEC_PURESND_PM3_SIZE, mst_codec_puresnd_pm3,
        MST_CODEC_PURESND_PM4_ADDR, MST_CODEC_PURESND_PM4_SIZE, mst_codec_puresnd_pm4,
        "PURESND",},
};
//==============================================
AUDIO_ALG_INFO    DEC_LoadCodeTable[ ]=
{        {
        MST_CODEC_DEC_PM1_ADDR, MST_CODEC_DEC_PM1_SIZE, mst_codec_dec_pm1,
        MST_CODEC_DEC_PM2_ADDR, MST_CODEC_DEC_PM2_SIZE, mst_codec_dec_pm2,
        MST_CODEC_DEC_PM3_ADDR, MST_CODEC_DEC_PM3_SIZE, mst_codec_dec_pm3,
        MST_CODEC_DEC_PM4_ADDR, MST_CODEC_DEC_PM4_SIZE, mst_codec_dec_pm4,
        "DEC",},
};
//==============================================
AUDIO_ALG_INFO    DDE_LoadCodeTable[ ]=
{        {
        MST_CODEC_DDE_PM1_ADDR, MST_CODEC_DDE_PM1_SIZE, mst_codec_dde_pm1,
        MST_CODEC_DDE_PM2_ADDR, MST_CODEC_DDE_PM2_SIZE, mst_codec_dde_pm2,
        MST_CODEC_DDE_PM3_ADDR, MST_CODEC_DDE_PM3_SIZE, mst_codec_dde_pm3,
        MST_CODEC_DDE_PM4_ADDR, MST_CODEC_DDE_PM4_SIZE, mst_codec_dde_pm4,
        "DDE",},
};
//==============================================
AUDIO_ALG_INFO    DBX_LoadCodeTable[ ]=
{        {
        MST_CODEC_DBX_PM1_ADDR, MST_CODEC_DBX_PM1_SIZE, mst_codec_dbx_pm1,
        MST_CODEC_DBX_PM2_ADDR, MST_CODEC_DBX_PM2_SIZE, mst_codec_dbx_pm2,
        MST_CODEC_DBX_PM3_ADDR, MST_CODEC_DBX_PM3_SIZE, mst_codec_dbx_pm3,
        MST_CODEC_DBX_PM4_ADDR, MST_CODEC_DBX_PM4_SIZE, mst_codec_dbx_pm4,
        "DBX",},
};


///////////////////////////////////////////////////////////////////////////////////////
#endif // _DECODER_INFO_H_

