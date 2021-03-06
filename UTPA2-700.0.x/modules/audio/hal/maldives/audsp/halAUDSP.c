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

#define ENABLE_AUDIO_DEC_R2_BIN     FALSE            //由於Milan DDR空間很緊，default 以flash load code 來節省r2 code 佔audio lib的DDR空間

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvBDMA.h"
#include "drvSERFLASH.h"

#include "regCHIP.h"

#include "halAUDSP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halADVAUDIO.h"
#include "ddr_config.h"
#include "r2_shm_comm.h"
#include "decR2_shm.h"
#if ASND_R2_SUPPORT
#include "sndR2_shm.h"
#endif

#ifdef MBOOT//Mboot Mini system
    #include "dspcode_s/aucode_s.c"
    #include "dspcode_s/aucode_dec.c"
    #include "dspcode_s/aucode_mboot_r2.c"
    #include "dspcode_s/aucode_none.c"
#else
    #include "dspcode_s/aucode_palsum.c"
    #include "dspcode_s/aucode_btsc.c"
    #include "dspcode_s/aucode_s.c"
    #include "dspcode_s/aucode_dec.c"
    #include "dspcode_s/aucode_adec_r2.c"
#if ASND_R2_SUPPORT
    #include "dspcode_s/aucode_asnd_r2.c"
#endif
    #include "dspcode_s/aucode_dde.c"
    #include "dspcode_s/aucode_ms10_dde.c"
    #include "dspcode_s/aucode_dts_t.c"
    #include "dspcode_s/aucode_none.c"
    #include "dspcode_adv/aucode_tshd.c"
    #include "dspcode_adv/aucode_dps.c"
    #include "dspcode_adv/aucode_dbx.c"
#endif


#if defined(MSOS_TYPE_NUTTX)
extern int lib_lowprintf(const char *format, ...);
#define DBG_PREFIX lib_lowprintf
#else
#define DBG_PREFIX printf
#endif

#if (defined ANDROID)
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#ifndef LOGI // android 4.1 rename LOGx to ALOGx
#define LOGI ALOGI
#endif

#ifndef LOGE // android 4.1 rename LOGx to ALOGx
#define LOGE ALOGE
#endif

#define HALAUDSP_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
#define HALAUDSP_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)
#else
#define HALAUDSP_PRINT(fmt, args...)    DBG_PREFIX("[[utopia]]      " fmt, ## args)
#define HALAUDSP_ERROR(fmt, args...)    DBG_PREFIX("[[utopia]]      " fmt, ## args)
#endif

#define HALAUDSP_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALAUDSP_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALAUDSP_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DBG_AUDSP_LOAD(args...)   //DBG_PREFIX(args)
#define DBG_AUDSP(args...)        //DBG_PREFIX(args)
#define DBG_AUDSP_ERROR(args...)  //DBG_PREFIX(args);

#ifdef MBOOT //mboot Speed up
  #define DSP_VERIFY_DSP_CODE     0   // 0: don't verify dsp code (for speed)
                                    // 1: verify dsp code (for debug)
  #define DSP_IDMA_CHK_READY      1   // 0: don't check IDMA ready (for speed)
                                    // 1: check IDMA ready (for debug)
#else
  #define DSP_VERIFY_DSP_CODE     1   // 0: don't verify dsp code (for speed)
                                    // 1: verify dsp code (for debug)
  #define DSP_IDMA_CHK_READY      1   // 0: don't check IDMA ready (for speed)
                                    // 1: check IDMA ready (for debug)
#endif

#define AU_NULL  0

#ifdef MBOOT
    #define FLASH_COPY(x,y,z,i,j)   TRUE
    #define FLASH_CHKDONE()         TRUE
#else
    #define FLASH_COPY(x,y,z,i,j)   MDrv_SERFLASH_CopyHnd(x,y,z,i,j)
    #define FLASH_CHKDONE()         MDrv_FLASH_CheckWriteDone()
#endif

LOAD_CODE_INFO g_loadcodeinfo;
MS_BOOL        g_bDSPLoadCode = FALSE;

#ifndef MBOOT
AUDIO_ALG_INFO audio_se_dec_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB_SYS_NONE[0]
    {
        MST_CODEC_DEC_PM1_ADDR, MST_CODEC_DEC_PM1_SIZE, mst_codec_dec_pm1 + 6,
        MST_CODEC_DEC_PM2_ADDR, MST_CODEC_DEC_PM2_SIZE, mst_codec_dec_pm2 + 6,
        MST_CODEC_DEC_PM3_ADDR, MST_CODEC_DEC_PM3_SIZE, mst_codec_dec_pm3 + 6,
        MST_CODEC_DEC_PM4_ADDR, MST_CODEC_DEC_PM4_SIZE, mst_codec_dec_pm4 + 6,
        "SE dec_none",
    },

    //MPEG_AD[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "mpeg_ad",
    },

    //AC3_AD[2]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ac3_ad",
    },

    //AC3P_AD[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ac3p_ad",
    },

    //AAC_AD[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "aac_ad",
    },

    //DDE[5]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "dde",
    },

    //GAAC_AD[6]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "gaac_ad"
    },

    //MP3[7]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "mp3",
    },

    //mpeg_en[8]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "mpeg_en",
    },

     //XPCM2[9]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "XPCM2",
    },

    //KTV[A]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "KTV",
    },

    //KTV2[B]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "KTV2",
    },
};

AUDIO_ALG_INFO audio_se_enc_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB2_ENCODE_NONE[0]
    {
        MST_CODEC_DEC_PM1_ADDR, MST_CODEC_DEC_PM1_SIZE, mst_codec_dec_pm1 + 6,
        MST_CODEC_DEC_PM2_ADDR, MST_CODEC_DEC_PM2_SIZE, mst_codec_dec_pm2 + 6,
        MST_CODEC_DEC_PM3_ADDR, MST_CODEC_DEC_PM3_SIZE, mst_codec_dec_pm3 + 6,
        MST_CODEC_DEC_PM4_ADDR, MST_CODEC_DEC_PM4_SIZE, mst_codec_dec_pm4 + 6,
        "SE enc_none",
    },

    //AU_DVB2_ENCODE_DTSE[1]
    {
        MST_CODEC_DTS_T_PM1_ADDR, MST_CODEC_DTS_T_PM1_SIZE, mst_codec_dts_t_pm1 + 6,
        MST_CODEC_DTS_T_PM2_ADDR, MST_CODEC_DTS_T_PM2_SIZE, mst_codec_dts_t_pm2 + 6,
        MST_CODEC_DTS_T_PM3_ADDR, MST_CODEC_DTS_T_PM3_SIZE, mst_codec_dts_t_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "dts_e"
    },

    //AU_DVB2_ENCODE_MS10_DDE[2]
    {
        MST_CODEC_MS10_DDE_PM1_ADDR, MST_CODEC_MS10_DDE_PM1_SIZE, mst_codec_ms10_dde_pm1 + 6,
        MST_CODEC_MS10_DDE_PM2_ADDR, MST_CODEC_MS10_DDE_PM2_SIZE, mst_codec_ms10_dde_pm2 + 6,
        MST_CODEC_MS10_DDE_PM3_ADDR, MST_CODEC_MS10_DDE_PM3_SIZE, mst_codec_ms10_dde_pm3 + 6,
        MST_CODEC_MS10_DDE_PM4_ADDR, MST_CODEC_MS10_DDE_PM4_SIZE, mst_codec_ms10_dde_pm4 + 6,
        "ms10_dde",
    },

    //AU_DVB2_ENCODE_DDE[3]
    {
        MST_CODEC_DDE_PM1_ADDR, MST_CODEC_DDE_PM1_SIZE, mst_codec_dde_pm1 + 6,
        MST_CODEC_DDE_PM2_ADDR, MST_CODEC_DDE_PM2_SIZE, mst_codec_dde_pm2 + 6,
        MST_CODEC_DDE_PM3_ADDR, MST_CODEC_DDE_PM3_SIZE, mst_codec_dde_pm3 + 6,
        MST_CODEC_DDE_PM4_ADDR, MST_CODEC_DDE_PM4_SIZE, mst_codec_dde_pm4 + 6,
        "dde",
    },

     //AU_DVB2_ENCODE_MAX[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "enc_max",
    },
};



AUDIO_ALG_INFO audio_snd_mm_dec_info[]=
{
    //DEC_NONE[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DEC none",
    },

    //DEC_DEC_NONE[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DEC none",
    },

    //DEC_ENC_NONE[2]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DEC none",
    },

    //MPEG[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "mpeg2",
    },

    //AC3[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ac3p_2"
    },

    //AC3+[5]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ac3p_2"
    },

    //AAC[6]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "aac2"
    },

    //MP3[7]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "mp3_2",
    },

    //WMA[8]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "wma2"
    },

    //Reserved3[9]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "Reserved3",
    },

    //RM[A]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "cook2",
    },

    //XPCM[B]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "xpcm2",
    },

    //TONE[C]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "tone",
    },

    //DTS[D]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "dts2",
    },

    //MS10_DDT[E]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "ms10_ddt2",
    },

    //MS10_DDC[F]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //WMA_PRO[10]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "wma_pro2"
    },

    //FLAC[11]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "flac",
    },

    //VORBIS[12]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "vorbis_2",
    },

    //DTSLBR[13]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "dtslbr",
    },

    //AMR_NB[14]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "amr_nb2",
    },

    //AMR_WB[15]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "amr_wb2",
    },
};

AUDIO_ALG_INFO  audio_adv_sndeff_info[]    =
{
    //PL2, 0
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //BBE, 1
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //SRS, 2
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //VDS, 3
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //VSPK, 4
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //SUPV, 5
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //TSHD, 6
    {
        MST_CODEC_TSHD_PM1_ADDR, MST_CODEC_TSHD_PM1_SIZE, mst_codec_tshd_pm1 + 6,
        MST_CODEC_TSHD_PM2_ADDR, MST_CODEC_TSHD_PM2_SIZE, mst_codec_tshd_pm2 + 6,
        MST_CODEC_TSHD_PM3_ADDR, MST_CODEC_TSHD_PM3_SIZE, mst_codec_tshd_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "tshd",
    },

    //XEN, 7
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //TSHDVIQ, 8
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //ADV, 9
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //DBX, 10
    {
        MST_CODEC_DBX_PM1_ADDR, MST_CODEC_DBX_PM1_SIZE, mst_codec_dbx_pm1 + 6,
        MST_CODEC_DBX_PM2_ADDR, MST_CODEC_DBX_PM2_SIZE, mst_codec_dbx_pm2 + 6,
        MST_CODEC_DBX_PM3_ADDR, MST_CODEC_DBX_PM3_SIZE, mst_codec_dbx_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "dbx",
    },

    //THEATERSOUND, 11
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "theather sound",
    },

    //PURESND, 12
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "puresnd",
    },

    //STUDIOSOUND_3D, 13
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },

    //DPS, 14
    {
        MST_CODEC_DPS_PM1_ADDR, MST_CODEC_DPS_PM1_SIZE, mst_codec_dps_pm1 + 6,
        MST_CODEC_DPS_PM2_ADDR, MST_CODEC_DPS_PM2_SIZE, mst_codec_dps_pm2 + 6,
        MST_CODEC_DPS_PM3_ADDR, MST_CODEC_DPS_PM3_SIZE, mst_codec_dps_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "BONGIOVI DPS",
    },
};

AUDIO_ALG_INFO  audio_soundeffect_info[]=
{
    // SE System
    {
        MST_CODEC_PM1_ADDR, MST_CODEC_PM1_SIZE, mst_codec_pm1 + 6,
        MST_CODEC_PM2_ADDR, MST_CODEC_PM2_SIZE, mst_codec_pm2 + 6,
        MST_CODEC_PM3_ADDR, MST_CODEC_PM3_SIZE, mst_codec_pm3 + 6,
        MST_CODEC_PM4_ADDR, MST_CODEC_PM4_SIZE, mst_codec_pm4 + 6,
        "T12 SE system"
    },
};

AUDIO_ALG_INFO audio_sndeff_info[]=
{
    //none
    {
        MST_CODEC_NONE_PM1_ADDR, MST_CODEC_NONE_PM1_SIZE, mst_codec_none_pm1 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        MST_CODEC_NONE_PM3_ADDR, MST_CODEC_NONE_PM3_SIZE, mst_codec_none_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "SE adv_none",
    },
};

AUDIO_ALG_INFO audio_sif_dec_info[]=
{
    //NONE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "NULL",
    },

    //BTSC[0]
    {
        MST_CODEC_SIF_BTSC_PM1_ADDR, MST_CODEC_SIF_BTSC_PM1_SIZE, mst_codec_sif_btsc_pm1 + 6,
        MST_CODEC_SIF_BTSC_PM2_ADDR, MST_CODEC_SIF_BTSC_PM2_SIZE, mst_codec_sif_btsc_pm2 + 6,
        MST_CODEC_SIF_BTSC_PM3_ADDR, MST_CODEC_SIF_BTSC_PM3_SIZE, mst_codec_sif_btsc_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "BTSC",
    },

    //EIAJ[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "EIAJ",
    },

    //palsum[2]
    {
        MST_CODEC_SIF_PALSUM_PM1_ADDR, MST_CODEC_SIF_PALSUM_PM1_SIZE, mst_codec_sif_palsum_pm1 + 6,
        MST_CODEC_SIF_PALSUM_PM2_ADDR, MST_CODEC_SIF_PALSUM_PM2_SIZE, mst_codec_sif_palsum_pm2 + 6,
        MST_CODEC_SIF_PALSUM_PM3_ADDR, MST_CODEC_SIF_PALSUM_PM3_SIZE, mst_codec_sif_palsum_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "PAL-SUM",
    },

    //palsum[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "PAL-SUM-VIF-42M",
    },

    //palsum[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "PAL-SUM-VIF-44M",
    },

    //FM_RADIO[5]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "FM-RADIO",
    },
};

#else //=======================MBOOT=========================================//
AUDIO_ALG_INFO audio_se_dec_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB_SYS_NONE[0]
    {
        MST_CODEC_DEC_PM1_ADDR, MST_CODEC_DEC_PM1_SIZE, mst_codec_dec_pm1 + 6,
        MST_CODEC_DEC_PM2_ADDR, MST_CODEC_DEC_PM2_SIZE, mst_codec_dec_pm2 + 6,
        MST_CODEC_DEC_PM3_ADDR, MST_CODEC_DEC_PM3_SIZE, mst_codec_dec_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "SE dec_none",
    },

    //MPEG_AD[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AC3_AD[2]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AC3P_AD[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AAC_AD[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //DDE[5]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //GAAC_AD[6]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //MP3[7]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //mpeg_en[8]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //XPCM2[9]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //KTV[A]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //KTV2[B]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
};

AUDIO_ALG_INFO audio_se_enc_info[]=
{
    // ---------- Decoder2 DSP Code ----------
    //AU_DVB2_ENCODE_NONE[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AU_DVB2_ENCODE_DTSE[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AU_DVB2_ENCODE_MS10_DDE[2]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

    //AU_DVB2_ENCODE_DDE[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

     //AU_DVB2_ENCODE_MAX[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
};

AUDIO_ALG_INFO audio_snd_mm_dec_info[]=
{
    //DEC_NONE[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "DEC none",
    },
};

AUDIO_ALG_INFO  audio_soundeffect_info[]=
{
    // SE System
    {
        MST_CODEC_PM1_ADDR, MST_CODEC_PM1_SIZE, mst_codec_pm1 + 6,
        MST_CODEC_PM2_ADDR, MST_CODEC_PM2_SIZE, mst_codec_pm2 + 6,
        MST_CODEC_PM3_ADDR, MST_CODEC_PM3_SIZE, mst_codec_pm3 + 6,
        MST_CODEC_PM4_ADDR, MST_CODEC_PM4_SIZE, mst_codec_pm4 + 6,
        "T12 SE system"
    },
};

AUDIO_ALG_INFO audio_sndeff_info[]=
{
    //none
    {
        MST_CODEC_NONE_PM1_ADDR, MST_CODEC_NONE_PM1_SIZE, mst_codec_none_pm1 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        MST_CODEC_NONE_PM3_ADDR, MST_CODEC_NONE_PM3_SIZE, mst_codec_none_pm3 + 6,
        AU_NULL, AU_NULL, AU_NULL,
        "SE adv_none",
    },
};

AUDIO_ALG_INFO audio_sif_dec_info[]=
{
    //NONE
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "NULL",
    },

    //BTSC[0]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
    //EIAJ[1]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
    //palsum[2]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
    //palsum[3]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
    //palsum[4]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },
    //FM_RADIO[5]
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL,
    },

};

AUDIO_ALG_INFO  audio_adv_sndeff_info[]    =
{

    //PL2, 0
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "pl2",
    },
    //BBE, 1
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //SRS, 2
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //VDS, 3
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //VSPK, 4
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //SUPV, 5
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //TSHD, 6
     {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //XEN, 7
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //TSHDVIQ, #8
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
    //ADV, 9
    {
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        AU_NULL, AU_NULL, AU_NULL,
        "null",
    },
};
#endif


#ifndef MSOS_TYPE_NOS
void* MDrv_MPool_PA2KSEG1(void* pAddrPhys);
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_HAL_ERR(x, args...)        //{printf(x, ##args);}
#define LOU8(MS_U16Val)  ( (MS_U8)(MS_U16Val) )
#define HIU8(MS_U16Val)  ( (MS_U8)((MS_U16Val) >> 8) )

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_BOOL g_bAudio_loadcode_from_dram;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

extern MS_S32  _s32AUDIOMutexIDMA;

#ifndef MSOS_TYPE_NUTTX
extern AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm;
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 g_u8DspCodeTypeLoaded = 0;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DspLoadCodeKernel()
/// @brief \b Function \b Description:  load CM/PM/cache/prefetch DSP code
/// @param <IN>        \b \b u8Type    :      -- DSP load code type
///                                        DSP_segment -- select DSP load code seg
///                                     DSP_select -- select DSP1 or DSP2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_DspLoadCode(MS_U8 u8Type)
{
#ifndef MSOS_TYPE_NUTTX
   REE_TO_TEE_MBX_MSG_TYPE msg_type = REE_TO_TEE_MBX_MSG_NULL;
#endif
    MS_U32 MIU_addr;
    AUDIO_ALG_INFO *pau_info=NULL;
    MS_U8   DSP_select=0;

   HALAUDSP_CHECK_SHM_INIT;

    DBG_AUDSP_LOAD("@@@@@@@@@@    ====== HAL_AUDSP_DspLoadCode: 0x%x ======\r\n",u8Type);

    switch(u8Type & 0xF0)
    {
        case 0x00:
        case 0x10:
            return  TRUE;    // Decoder in DEC-R2

            break;

        case 0x30://AU_DVB2_STANDARD_MAD_TYPE
           if(u8Type >= AU_DVB2_STANDARD_MAX )
            {
                printf("    [audio_se_dec_info]:decoder type out of range.\r\n");
                return FALSE;
            }
            pau_info  = &audio_se_dec_info[u8Type&0xF];
            DSP_select = DSP_SE;

            break;
        case 0x40://AU_DVB2_ADVSND_TYPE
        case 0x50:
            if(u8Type >= AU_DVB2_ADVSND_MAX )
            {
                printf("    [audio_adv_sndeff_info]:decoder type out of range.\r\n");
                return FALSE;
            }
            pau_info = &audio_adv_sndeff_info[u8Type&0xF];
            DSP_select= DSP_SE;
            break;

        case 0x60://AU_SE_SYSTEM
            pau_info = &audio_soundeffect_info[0];
            DSP_select = DSP_SE;
            break;

        case 0xa0://AU_SND_EFFECT
            pau_info = &audio_sndeff_info[0];
            DSP_select = DSP_SE;
            break;

        case 0xb0://AU_DVB_STANDARD_SIF_TYPE
            if ( (u8Type & (~0x08)) > AU_SIF_FM_RADIO )
            {
                printf("    [audio_sif_info]:decoder type out of range.\r\n");
                return FALSE;
            }
            pau_info = &audio_sif_dec_info[u8Type&0x07];
            DSP_select = DSP_SE;
            break;

        case 0xd0://AU_DVB2_ENCODE_TYPE
           if(u8Type >= AU_DVB2_ENCODE_MAX )
            {
                printf("    [audio_se_encode_info]:encoder type out of range.\r\n");
                return FALSE;
            }
            pau_info  = &audio_se_enc_info[u8Type&0xF];
            DSP_select = DSP_SE;

            break;

        case 0xe0://AU_DEC_SYSTEM
            return  TRUE;    // Decoder in DEC-R2

            break;

        default:
            DBG_AUDSP_ERROR("    [HAL_AUDSP_DspLoadCode]:======  Loading the wrong DSP code type!======\r\n");
            return FALSE;
    }

#ifndef MSOS_TYPE_NUTTX
    AUDIO_TEE_INFO_SHM_CHECK_NULL;
    REE_TO_TEE_MBX_MSG_SET_PARAM(0, (MS_U8)u8Type);
    REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(1);
    msg_type = REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode;
    if (DSP_select == DSP_DEC)
    {
        msg_type = msg_type | REE_TO_TEE_MBX_MSG_TYPE_DEC;
    }
    else if (DSP_select == DSP_SE)
    {
        msg_type = msg_type | REE_TO_TEE_MBX_MSG_TYPE_SE;
    }

    if ( TEE_TO_REE_MBX_ACK_MSG_NO_TEE != (mbx_msg_ack_status = HAL_AUDIO_SendMBXMsg(msg_type)))
    {
        if (mbx_msg_ack_status != TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS)
            return FALSE;
        HAL_AUDSP_SetDspCodeTypeLoaded(u8Type);
        return TRUE;
    }
#endif

    if(pau_info->cm_len == 0)
        return TRUE;                 // Return if CM length = 0 (BDMA can't support 0 length)
    HAL_AUDSP_SetDspLoadCodeInfo(pau_info, DSP_select);

    if(DSP_select == DSP_DEC)
    {
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x00);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);
    }

    if ( u8Type == 0x60 )
    {
        pau_info->cm_addr = 0x0008;
        pau_info->cm_len = (MST_CODEC_PM1_SIZE - 24);
        pau_info->cm_buf = (mst_codec_pm1+ 30);
    }

     // Download PM of Algorithm
    DBG_AUDSP_LOAD("    PM addr: 0x%lx\r\n",pau_info->pm_addr);
    DBG_AUDSP_LOAD("    PM buf addr: 0x%lx\r\n",(MS_U32)pau_info->pm_buf);
    DBG_AUDSP_LOAD("    PM size: 0x%lx\r\n",pau_info->pm_len);

    if(!HAL_AUDSP_DspLoadCodeSegment(pau_info->pm_addr, pau_info->pm_buf, pau_info->pm_len, DSP_select))
        return FALSE;
    if(!HAL_AUDSP_DspVerifySegmentCode(pau_info->pm_addr, pau_info->pm_buf, pau_info->pm_len, DSP_select))
        return FALSE;

    DBG_AUDSP_LOAD("    CM addr: 0x%lx\r\n",pau_info->cm_addr);
    DBG_AUDSP_LOAD("    CM buf addr: 0x%lx\r\n",(MS_U32)pau_info->cm_buf);
    DBG_AUDSP_LOAD("    CM size: 0x%lx\r\n",pau_info->cm_len);

    if(!HAL_AUDSP_DspLoadCodeSegment(pau_info->cm_addr, pau_info->cm_buf, pau_info->cm_len, DSP_select))
       return FALSE;
    if(!HAL_AUDSP_DspVerifySegmentCode(pau_info->cm_addr, pau_info->cm_buf, pau_info->cm_len, DSP_select))
       return FALSE;

    // Download PM of PreFetch
    if(pau_info->prefetch_len != 0)
    {
        DBG_AUDSP_LOAD("    PreFetch PM addr: 0x%lx\r\n", pau_info->prefetch_addr);
        DBG_AUDSP_LOAD("    PreFetch PM buf addr: 0x%lx\r\n", (MS_U32)pau_info->prefetch_buf);
        DBG_AUDSP_LOAD("    PreFetch PM size: 0x%lx\r\n", pau_info->prefetch_len);

        MIU_addr = (MS_U32)pau_info->prefetch_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_select);
        DBG_AUDSP_LOAD("    MIU of PreFetch: 0x%lX\r\n", MIU_addr);

        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((MS_U32)(pau_info->prefetch_buf)), pau_info->prefetch_len);
    }

    // Download PM of Cache
    if(pau_info->cache_len != 0)
    {
        DBG_AUDSP_LOAD("    Cache PM addr: 0x%lx\r\n", (MS_U32)pau_info->cache_addr);
        DBG_AUDSP_LOAD("    Cache PM buf addr: 0x%lx\r\n", (MS_U32)pau_info->cache_buf);
        DBG_AUDSP_LOAD("    Cache PM size: 0x%lx\r\n", pau_info->cache_len);

        MIU_addr = (MS_U32)pau_info->cache_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_select);
        DBG_AUDSP_LOAD("    MIU of Cache: 0x%lX\r\n", MIU_addr);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((MS_U32)(pau_info->cache_buf)), pau_info->cache_len);
    }

  // Load  SE-DSP  system code
    if(u8Type == 0x60)
    {
        HAL_AUR2_WriteByte(REG_DECR2_RESET_CTRL, 0x00);               // STOP SND-R2
        HAL_AUR2_WriteMaskReg(REG_DECR2_SYSTEM_START, 0xFFFF, 0x0000); // clear DEC-R2 start cmd register
#if ASND_R2_SUPPORT
        HAL_AUR2_WriteByte(REG_SNDR2_RESET_CTRL, 0x00);               // STOP SND-R2
        HAL_AUR2_WriteMaskReg(REG_SNDR2_SYSTEM_START, 0xFFFF, 0x0000); //clear SND-R2 start cmd register
#endif
        HAL_AUDIO_WriteMaskReg(0x2DDC, 0xFFFF, 0x0000); //clear  SE-DSP start cmd register
        AUDIO_DELAY1MS(1);

      #ifdef  MBOOT
      //============ Load R2 code ===============
      // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
      //====================================

      // Load DEC-R2
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((MS_U32)(mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        HAL_DEC_R2_init_SHM_param();
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();

        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
      #else
      //============ Load R2 code ===============
      // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
      //====================================

      // Load DEC-R2
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((MS_U32)(mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                      MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        HAL_DEC_R2_init_SHM_param();
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();
#if ASND_R2_SUPPORT
     // Load SND-R2
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV)+ADEC__R2_DDR_SIZE;
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((mst_snd_r2)), SND_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE), 0, ASND__R2_DDR_SIZE - SND_R2_SHM_DDR_OFFSET - SND_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE),
                      (void*)(mst_snd_r2+SND_R2_SHM_DDR_OFFSET+SND_R2_SHM_DDR_SIZE),
                      MST_SND_R2_AUDIO_SIZE - SND_R2_SHM_DDR_OFFSET - SND_R2_SHM_DDR_SIZE);
        AUDIO_DELAY1MS(1);
        MsOS_FlushMemory();

        HAL_SND_R2_EnableR2(TRUE);               // Enable SND-R2 after load R2 code
#endif
        HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
      #endif

        if(!HAL_AUDSP_DspLoadCodeSegment(0x0001,  mst_codec_pm1+6+3, 21, DSP_select)) return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(0x0001,  mst_codec_pm1+6+3, 21, DSP_select)) return FALSE;
        if(!HAL_AUDSP_DspLoadCodeSegment(0x0000,  mst_codec_pm1+6, 3, DSP_select)) return FALSE;
        if(!HAL_AUDSP_DspVerifySegmentCode(0x0000,  mst_codec_pm1+6, 3, DSP_select)) return FALSE;
    }

    MsOS_FlushMemory();

    HAL_AUDSP_SetDspCodeTypeLoaded(u8Type);
    DBG_AUDSP_LOAD("HAL_AUDSP_DspLoadCode finished(type=%s(0x%x))\r\n", pau_info->AlgName, u8Type);

    return TRUE;
}

void * HAL_AUDSP_Get_Alg_Info(MS_U8 Id, MS_U8 u8Type)
{
    void * pAudio_Info;
    AUDIO_DSP_ID DSP_Id;

    DSP_Id = (AUDIO_DSP_ID) Id;
    switch ( DSP_Id )
    {
        case AUDIO_DSP_ID_DEC:
            //Cathy Temp
        case AUDIO_DSP_ID_SND:
            pAudio_Info = (void *) &audio_snd_mm_dec_info[u8Type];
            break;

        default:
            HALAUDSP_ERROR ("Un-Support DSP ID (%d)\r\n", DSP_Id);
            pAudio_Info = NULL;
            return pAudio_Info;
    }

    HALAUDSP_ERROR ("DSP %s Got the Alg Info (0x%X)\r\n", DSP_Id == AUDIO_DSP_ID_DEC ? "DEC" : "SND", u8Type);

    return pAudio_Info;
}

MS_BOOL HAL_AUDSP_DspLoadCode2(MS_U8 DSP_select, void * info)
{
    MS_U32 MIU_addr;
    AUDIO_ALG_INFO * pau_info = (AUDIO_ALG_INFO *) info;

    if ( pau_info->cm_len == 0 )
        return TRUE;                 // Return if CM length = 0 (BDMA can't support 0 length)

//    HALAUDSP_ERROR ("@@@@@@@@@@    ====== %s: %s start ======\r\n", __FUNCTION__, pau_info->AlgName);

    //HAL_AUDSP_SetDspLoadCodeInfo(pau_info, DSP_select);

    HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, ((DSP_select == DSP_DEC) ? 0x00 : 0x01));

    /* Download PM of Algorithm */
    DBG_AUDSP_LOAD("    PM addr:     0x%lx\r\n", pau_info->pm_addr);
    DBG_AUDSP_LOAD("    PM buf addr: 0x%lx\r\n", (MS_U32) pau_info->pm_buf);
    DBG_AUDSP_LOAD("    PM size:     0x%lx\r\n", pau_info->pm_len);

    if ( !HAL_AUDSP_DspLoadCodeSegment(pau_info->pm_addr, pau_info->pm_buf, pau_info->pm_len, DSP_select) )
    {
        HALAUDSP_ERROR ("Dsp ReLoad Code PM Segment Fail !!\r\n");
        return FALSE;
    }

    if ( !HAL_AUDSP_DspVerifySegmentCode(pau_info->pm_addr, pau_info->pm_buf, pau_info->pm_len, DSP_select) )
    {
        HALAUDSP_ERROR ("Dsp ReLoad Code PM Segment Verify Fail !!\r\n");
        return FALSE;
    }

    DBG_AUDSP_LOAD("    CM addr:     0x%lx\r\n", pau_info->cm_addr);
    DBG_AUDSP_LOAD("    CM buf addr: 0x%lx\r\n", (MS_U32) pau_info->cm_buf);
    DBG_AUDSP_LOAD("    CM size:     0x%lx\r\n", pau_info->cm_len);

    if ( !HAL_AUDSP_DspLoadCodeSegment(pau_info->cm_addr, pau_info->cm_buf, pau_info->cm_len, DSP_select) )
    {
        HALAUDSP_ERROR ("Dsp ReLoad Code CM Segment Fail !!\r\n");
        return FALSE;
    }

    if ( !HAL_AUDSP_DspVerifySegmentCode(pau_info->cm_addr, pau_info->cm_buf, pau_info->cm_len, DSP_select) )
    {
        HALAUDSP_ERROR ("Dsp ReLoad Code CM Segment Verify Fail !!\r\n");
        return FALSE;
    }

    /* Download PM of PreFetch */
    if ( pau_info->prefetch_len != 0 )
    {
        DBG_AUDSP_LOAD("    PreFetch PM addr: 0x%lx\r\n", pau_info->prefetch_addr);
        DBG_AUDSP_LOAD("    PreFetch PM buf addr: 0x%lx\r\n", (MS_U32)pau_info->prefetch_buf);
        DBG_AUDSP_LOAD("    PreFetch PM size: 0x%lx\r\n", pau_info->prefetch_len);

        MIU_addr = (MS_U32) pau_info->prefetch_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_select);
        HALAUDSP_ERROR ("    MIU of PreFetch: 0x%lX\r\n", MIU_addr);

        memcpy((void *)(MS_PA2KSEG1(MIU_addr)), (void *)((MS_U32)(pau_info->prefetch_buf)), pau_info->prefetch_len);
    }

    /* Download PM of Cache */
    if ( pau_info->cache_len != 0 )
    {
        DBG_AUDSP_LOAD("    Cache PM addr: 0x%lx\r\n", (MS_U32)pau_info->cache_addr);
        DBG_AUDSP_LOAD("    Cache PM buf addr: 0x%lx\r\n", (MS_U32)pau_info->cache_buf);
        DBG_AUDSP_LOAD("    Cache PM size: 0x%lx\r\n", pau_info->cache_len);

        MIU_addr = (MS_U32)pau_info->cache_addr * 3 + HAL_AUDIO_GetDspMadBaseAddr(DSP_select);
        DBG_AUDSP_LOAD("    MIU of Cache: 0x%lX\r\n", MIU_addr);

        memcpy((void *)(MS_PA2KSEG1(MIU_addr)), (void *)((MS_U32)(pau_info->cache_buf)), pau_info->cache_len);
    }

    MsOS_FlushMemory();

    DBG_AUDSP_LOAD("DSP %s finished (type=%s)\r\n", ((DSP_select == DSP_DEC) ? "DEC" : "SND"), pau_info->AlgName);

    return TRUE;
}

MS_BOOL HAL_AUDSP_DspLoadSystemCode2(MS_U8 DSP_select, AUDIO_ALG_INFO * pau_info)
{
    MS_U8  * dsp_bootcode;
 #ifndef  MBOOT
    MS_U32   MIU_addr;
 #endif

    if ( pau_info->cm_len == 0 )
    {
        return TRUE;                 // Return if CM length = 0 (BDMA can't support 0 length)
    }

    dsp_bootcode = pau_info->cm_buf;

    pau_info->cm_addr = 0x0008;
    pau_info->cm_len = (MST_CODEC_PM1_SIZE - 24);
    pau_info->cm_buf = (mst_codec_pm1 + 30);

    if ( !HAL_AUDSP_DspLoadCode2(DSP_select, pau_info) )
    {
        return FALSE;
    }

    if ( DSP_select == DSP_SE )
    {
        HAL_AUDIO_WriteMaskReg(0x2E9E, 0xFFFF, 0x0000);
        HAL_AUDIO_WriteMaskByte(REG_DECR2_RESET_CTRL, 0xFF, 0x00);                //Stop R2
        HAL_AUDIO_WriteMaskReg(0x2E9E, 0xFFFF, 0x0000); //clear R2 & SE-DSP start cmd register
        HAL_AUDIO_WriteMaskReg(0x2DDC, 0xFFFF, 0x0000); //clear R2 & SE-DSP start cmd register
        AUDIO_DELAY1MS(1);
#ifndef  MBOOT
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(2);
        memcpy((void *) (MS_PA2KSEG1 (MIU_addr)), (void *) ((MS_U32) (mst_codec_r2)), MST_CODEC_R2_AUDIO_SIZE);
#endif
        HAL_AUDIO_WriteMaskByte(REG_DECR2_RESET_CTRL, 0xFF, 0x27);                 //Eanble R2
    }


    if ( !HAL_AUDSP_DspLoadCodeSegment(0x0001, dsp_bootcode + 3, 21, DSP_select) )
    {
         return FALSE;
    }

    if ( !HAL_AUDSP_DspVerifySegmentCode(0x0001, dsp_bootcode + 3, 21, DSP_select) )
    {
         return FALSE;
    }

    if ( !HAL_AUDSP_DspLoadCodeSegment(0x0000, dsp_bootcode, 3, DSP_select) )
    {
         return FALSE;
    }

    if ( !HAL_AUDSP_DspVerifySegmentCode(0x0000, dsp_bootcode, 3, DSP_select) )
    {
         return FALSE;
    }


    MsOS_FlushMemory();

    DBG_AUDSP_LOAD("HAL_AUDSP_DspLoadSystemCode2 finished(type=%s)\r\n", pau_info->AlgName);
    printf("HAL_AUDSP_DspLoadSystemCode2 finished(type=%s)\r\n", pau_info->AlgName);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspLoadCodeSegment()
/// @brief \b Function \b Description: This routine is used to load DSP code
/// @param <IN>        \b dsp_addr    :
/// @param <IN>        \b dspCode_buf    :
/// @param <IN>        \b dspCode_buflen    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  BOOL    :    TRUE --DSP Load code okay
///                                    FALSE--DSP Load code fail
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_DspLoadCodeSegment(MS_U32 dsp_addr, MS_U8  *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select)
{
    MS_U32 i,j;
    MS_U32 idma_wrbase_addr_l, dsp_brg_data_l, dsp_brg_data_h, bdma_mode_addr;

    HALAUDSP_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    g_bDSPLoadCode = TRUE;

    if(DSP_select == DSP_DEC)
    {
        idma_wrbase_addr_l = REG_DEC_IDMA_WRBASE_ADDR_L;
        dsp_brg_data_l = REG_DEC_DSP_BRG_DATA_L;
        dsp_brg_data_h = REG_DEC_DSP_BRG_DATA_H;
        bdma_mode_addr = REG_DEC_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x00);
    }
    else
    {
        idma_wrbase_addr_l = REG_SE_IDMA_WRBASE_ADDR_L;
        dsp_brg_data_l = REG_SE_DSP_BRG_DATA_L;
        dsp_brg_data_h = REG_SE_DSP_BRG_DATA_H;
        bdma_mode_addr = REG_SE_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);
    }

    HAL_AUDIO_WriteMaskReg(bdma_mode_addr, 0x8080, 0x0000); // disable bdma

    if(dspCode_buflen>0)
    {
        /* set iDMA addr */

        HAL_AUDIO_WriteReg(idma_wrbase_addr_l, dsp_addr);

        AUDIO_DELAY1MS(1);

        for( i=0; i<dspCode_buflen; i+=3)
        {
            HAL_AUDIO_WriteByte(dsp_brg_data_l,*(dspCode_buf+i+1));
            HAL_AUDIO_WriteByte(dsp_brg_data_h,*(dspCode_buf+i+2));

            for(j=0;j<2;j++);//for delay only

            HAL_AUDIO_WriteByte(dsp_brg_data_l,*(dspCode_buf+i));
            HAL_AUDIO_WriteByte(dsp_brg_data_h,0x00);

            if(DSP_select == DSP_DEC)
            {
                if (HAL_AUDSP_CheckDecIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
                {
                    g_bDSPLoadCode = FALSE;

                    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                    return FALSE;
                }
            }
            else
            {
                if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
                {
                    g_bDSPLoadCode = FALSE;

                    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                    return FALSE;
                }
            }
        }
    }

    g_bDSPLoadCode = FALSE;

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return TRUE;
}

MS_BOOL HAL_AUDSP_DspVerifySegmentCode(MS_U32 dsp_addr, MS_U8 *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select)
{
    MS_U32 i;
    MS_U8 dat[3];
    MS_U32 idma_rdbase_addr_l, idma_ctrl0, idma_rddata_h_0, idma_rddata_h_1, idma_rddata_l, bdma_mode_addr;

#if (DSP_VERIFY_DSP_CODE==0)
    return TRUE;                        //don't verify just return;
#endif

    HALAUDSP_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    g_bDSPLoadCode = TRUE;

    if ( DSP_select == DSP_DEC )
    {
        idma_rdbase_addr_l = REG_DEC_IDMA_RDBASE_ADDR_L;
        idma_ctrl0 = REG_DEC_IDMA_CTRL0;
        idma_rddata_h_0 = REG_DEC_IDMA_RDDATA_H_0;
        idma_rddata_h_1 = REG_DEC_IDMA_RDDATA_H_1;
        idma_rddata_l = REG_DEC_IDMA_RDDATA_L;
        bdma_mode_addr = REG_DEC_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x00);
    }
    else
    {
        idma_rdbase_addr_l = REG_SE_IDMA_RDBASE_ADDR_L;
        idma_ctrl0 = REG_SE_IDMA_CTRL0;
        idma_rddata_h_0 = REG_SE_IDMA_RDDATA_H_0;
        idma_rddata_h_1 = REG_SE_IDMA_RDDATA_H_1;
        idma_rddata_l = REG_SE_IDMA_RDDATA_L;
        bdma_mode_addr = REG_SE_BDMA_CFG;
        HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01);
    }

    //HAL_AUDIO_WriteMaskByte(0x3CEC, 0x40, 0x00);
    HAL_AUDIO_WriteMaskReg(bdma_mode_addr, 0x8080, 0x0000); // disable bdma (enable idma)
    HAL_AUDIO_WriteReg(idma_rdbase_addr_l, dsp_addr);
    AUDIO_DELAY1MS(1);

    for (i=0; i<dspCode_buflen; i+=3)
    {
        HAL_AUDIO_WriteMaskByte(idma_ctrl0, 0x08, 0x08 );
#if (DSP_IDMA_CHK_READY == 1)
     if(DSP_select == DSP_DEC)
        {
         if (HAL_AUDSP_CheckDecIdmaReady(AUD_CHK_DSP_READ_RDY)==FALSE)
             {
                g_bDSPLoadCode = FALSE;

                OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                return FALSE;
             }
        }
        else
        {
            if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_READ_RDY)==FALSE)
            {
                g_bDSPLoadCode = FALSE;

                OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

                return FALSE;
            }
        }
#endif
        dat[1] = HAL_AUDIO_ReadByte(idma_rddata_h_0);
        dat[2] = HAL_AUDIO_ReadByte(idma_rddata_h_1);
        dat[0] = HAL_AUDIO_ReadByte(idma_rddata_l);

        //printf("@@%x\n", (dat[2] << 16) | (dat[1] << 8) | (dat[0]));

        if ( (dat[0]!=dspCode_buf[i]) || (dat[1]!=dspCode_buf[i+1]) ||
                (dat[2]!=dspCode_buf[i+2]))
        {
            printf("check data %lx\n", i);
            printf("dat0 %X <===> ",dspCode_buf[i]);
            printf("%x \n", dat[0]);
            printf("dat1 %X <===> ",dspCode_buf[i+1]);
            printf("%x \n", dat[1]);
            printf("dat2 %x <===> ",dspCode_buf[i+2]);
            printf("%X \n", dat[2]);
            printf("  Dsp code verify error!!\r\n");

            g_bDSPLoadCode = FALSE;

            OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

            return FALSE;
        }
    }

    DBG_AUDSP("  Dsp code verify ok!!\n\r");

    g_bDSPLoadCode = FALSE;

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_CheckDecIdmaReady()
/// @brief \b Function \b Description:  This routine is used to check if the Dec-DSP IDMA is ready or not.
/// @param <IN>        \b IdmaChk_type    :
///                                    0x10 : check write ready
///                                    0x80 : check read  ready
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL    : TRUE--IDMA is ready
///                                      FALSE--IDMA not ready
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type )
{
    MS_U8  j = 0;

    while(j<200)
    {
        j++;
        if( (HAL_AUDIO_ReadByte(REG_DEC_IDMA_CTRL0)& u8IdmaChk_type) == 0 )
            return TRUE;
    }

    DBG_AUDSP_ERROR("DSP DEC Idma check ready fail!(%d)\r\n",j);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckSeIdmaReady()
/// @brief \b Function \b Description:  This routine is used to check if the Se-DSP IDMA is ready or not.
/// @param <IN>        \b IdmaChk_type    :
///                                    0x10 : check write ready
///                                    0x80 : check read  ready
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL    : TRUE--IDMA is ready
///                                      FALSE--IDMA not ready
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDSP_CheckSeIdmaReady(MS_U8 u8IdmaChk_type)
{
    MS_U8  j = 0;

    while(j<200)
    {
        j++;
        if( (HAL_AUDIO_ReadByte(REG_SE_IDMA_CTRL0)& u8IdmaChk_type) == 0 )
            return TRUE;
    }

    DBG_AUDSP_ERROR("DSP SE Idma check ready fail!(%d)\r\n",j);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_SetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to set the DSP code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDSP_SetDspCodeTypeLoaded(MS_U8 u8Type)
{
    g_u8DspCodeTypeLoaded=u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_GetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDSP_GetDspCodeTypeLoaded(void)
{
    return g_u8DspCodeTypeLoaded;
}

void HAL_AUDSP_SetDspLoadCodeInfo(AUDIO_ALG_INFO *pau_info, MS_U8 DSP_select)
{
    g_loadcodeinfo.pau_info = pau_info;
    g_loadcodeinfo.DSP_select= DSP_select;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_GetDspCodeTypeLoaded()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
LOAD_CODE_INFO* HAL_AUDSP_GetDspLoadCodeInfo(void)
{
    return &g_loadcodeinfo;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDSP_DECR2LoadCode()
/// @brief \b Function \b Description:  Load DEC-R2 code to DDR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDSP_DECR2LoadCode(void)
{
    MS_U32 MIU_addr;

    printf("======audio: start DEC-R2 load code======\n");

    HAL_AUR2_WriteByte(REG_DECR2_RESET_CTRL, 0x00);  // STOP DEC-R2
    HAL_AUR2_WriteMaskReg(REG_DECR2_SYSTEM_START, 0xFFFF, 0x0000); //clear DEC-R2 start cmd register
    AUDIO_DELAY1MS(1);

    //============ Load R2 code ===============
    // Memory sequence : DEC-R2 ==> SND-R2 ==> SE-DSP
    //====================================

    // Load DEC-R2
    #if (ENABLE_AUDIO_DEC_R2_BIN == FALSE)
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

        memcpy((void*)(MS_PA2KSEG1(MIU_addr)), (void*)((MS_U32)(mst_codec_r2)), DEC_R2_SHM_DDR_OFFSET);
        memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE), 0, ADEC__R2_DDR_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
        memcpy((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                          (void*)(mst_codec_r2+DEC_R2_SHM_DDR_OFFSET+DEC_R2_SHM_DDR_SIZE),
                          MST_CODEC_R2_AUDIO_SIZE - DEC_R2_SHM_DDR_OFFSET - DEC_R2_SHM_DDR_SIZE);
    #else
    {
        MIU_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);

        if (pf_loadR2CodeFromBin)
        {
            MsOS_Dcache_Flush(0, 64 * 1024 * 1024);
            MsOS_FlushMemory();
            pf_loadR2CodeFromBin(0, MIU_addr, ADEC__R2_DDR_SIZE);
            memset((void*)(MS_PA2KSEG1(MIU_addr)+DEC_R2_SHM_DDR_OFFSET), 0, DEC_R2_SHM_DDR_SIZE);
        }
        else
        {
            printf("[%s] error !! call pf_loadR2CodeFromBin is NULL\n", __FUNCTION__);
        }
    }
    #endif

    HAL_DEC_R2_init_SHM_param();
    AUDIO_DELAY1MS(1);
    MsOS_FlushMemory();

    HAL_DEC_R2_EnableR2(TRUE);               // Enable DEC-R2 after load R2 code
    HAL_MAD2_SetMcuCmd(0xF3);

    printf("======audio: end DEC-R2 load code======\n");
}

