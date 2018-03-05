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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvDscmb.h
/// @brief  Descrambler (Dscmb) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_DSCMB_H__
#define __REG_DSCMB_H__


/////////////////////////////////////////////////
//   Special Register Definition
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// rsa
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_RSA_BANK                      0x50900UL         // 0x1A12                // 0xBF342400
#define REG_SYS_CTRL                            (REG_DSCMB_RSA_BANK + 0x0001UL)
    #define REG_SYS_NSK_CLK                     16UL


////////////////////////////////////////////////////////////////////////////////////////////////
// cipher 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_CIPHER0_BANK             0x50380UL      // 0x1A07                // 0xBF340E00
#define REG_DSCMB_CIPHER1_BANK             0x50400UL      // 0x1A08                // 0xBF341000
#define REG_DSCMB_CIPHER_CNT                1UL

#define REG_DSCMB_CIPHER_BANK(_a_)              (REG_DSCMB_CIPHER0_BANK + ((_a_)*0x80UL)) // 0 & 1 only

#define REG_CIPHER_REG0(_a_)                    (REG_DSCMB_CIPHER_BANK(_a_) + 0x0000UL)
    #define REG_CIPHER_REG0_BLK_BADPKT_EN          0x00000001UL
    #define REG_CIPHER_REG0_DUALPATH_EN            0x00000002UL
    #define REG_CIPHER_REG0_3KEYTDES_EN            0x00000004UL
    #define REG_CIPHER_REG0_REDUCE_U_PKT_EN        0x00000008UL
    #define REG_CIPHER_REG0_IRD_TCSA3_EN           0x00000010UL
    #define REG_CIPHER_REG0_TSID0_CC_RESPIN_EN     0x00000100UL
    #define REG_CIPHER_REG0_CCRESPIN_EN_TSID(_a_)  (0x00000100UL << (_a_))
    #define REG_CIPHER_PES_EN_TSID(_a_)            (0x00010000UL << (_a_))

/// trouble register
#define REG_CIPHER_REG1(_a_)                        (REG_DSCMB_CIPHER_BANK(_a_) + 0x0001UL)
    /// write only, read is NOT allowed
    #define REG_CIPHER_REG1_PARSER_SAMPLE_MSK  0x000001FFUL     /// ??   write only, read is allowed
    #define REG_CIPHER_REG1_VIEWER_MSK         0x00001E00UL     /// ??
    #define REG_CIPHER_REG1_VIEWER_SHT         9UL
    #define REG_CIPHER_REG1_CLEARALL           0x00004000UL     /// ??
    #define REG_CIPHER_REG1_SNAPSHOT           0x00008000UL     /// ??
    #define REG_CIPHER_REG1_TSID_MSK           0x000F0000UL     /// ??
    #define REG_CIPHER_REG1_TSID_SHT           16UL
    #define REG_CIPHER_REG1_VIEWER_DBG_MODE     0x07000000UL
    #define REG_CIPHER_REG1_VIEWER_DBG_MODE_SHT 24UL
    #define REG_CIPHER_REG1_POLARITY            0x08000000UL
    #define REG_CIPHER_REG1_POLARITY_SHT        27UL
    // read only, write is NOT allowed
    #define REG_CIPHER_REG1_PARSERINFO_MSK     0xFFFFFFFFUL     /// ??  depends on how viewer is set. REG_CIPHER0_REG1_VIEWER_MSK

#define REG_CIPHER_LSAD_SCB(_a_)                    (REG_DSCMB_CIPHER_BANK(_a_) + 0x0002UL)
    /// write only, read is NOT allowed
    #define REG_CIPHER_LSAD_TROUBLE_CLR        0x00000008UL
    // read only, write is NOT allowed
    #define REG_CIPHER_LSAD_CIPHER_DEBUG       0x00FFFFFFUL
    #define REG_CIPHER_LSAD_TROUBLE            0xFF000000UL

#define REG_CIPHER_ESA_SCB(_a_)                (REG_DSCMB_CIPHER_BANK(_a_) + 0x0003UL)
    #define REG_CIPHER_SCBFIX_SHT(_a_)         ((_a_)?(13UL + ((_a_)* 3UL) ):0UL)
    #define REG_CIPHER_SCBFIX_MSK(_a_)         (0x00000007UL << REG_CIPHER_SCBFIX_SHT(_a_))
    /// write only, read is NOT allowed
    #define REG_CIPHER_ESA_TROUBLE_CLR         0x00000008UL
    // read only, write is NOT allowed
    #define REG_CIPHER_ESA_CIPHER_DEBUG        0x00FFFFFFUL
    #define REG_CIPHER_ESA_TROUBLE             0xFF000000UL

#define REG_CIPHER_LSAS_SCB(_a_)              (REG_DSCMB_CIPHER_BANK(_a_) + 0x0004UL)
    /// write only, read is NOT allowed
    #define REG_CIPHER_LSAS_TROUBLE_CLR        0x00000008UL
    // read only, write is NOT allowed
    #define REG_CIPHER_LSAS_CIPHER_DEBUG       0x00FFFFFFUL
    #define REG_CIPHER_LSAS_TROUBLE            0xFF000000UL

#define REG_CIPHER_DMX_SCB(_a_)                 (REG_DSCMB_CIPHER_BANK(_a_) + 0x0005UL)
    #define REG_CIPHER_DMXU_SCBFIX_SHT(_a_)     ((_a_)?(13UL + ((_a_)* 3UL) ):0UL)
    #define REG_CIPHER_DMXU_SCBFIX_MSK(_a_)     (0x00000007UL << REG_CIPHER_DMXU_SCBFIX_SHT(_a_))

    #define REG_CIPHER_DMXL_SCBFIX_TSID0_MSK   0x00000070UL
    #define REG_CIPHER_DMXL_SCBFIX_TSID0_SHT   4UL

#define REG_CIPHER_EKEY_INVALID(_a_)            (REG_DSCMB_CIPHER_BANK(_a_) + 0x0006UL)
    #define REG_CIPHER_DMXL_SCBFIX_SHT(_a_)     ((_a_)?(13UL + ((_a_)* 3UL) ):4UL)  // a = 0 ==> REG_DSCMB_CIPHER_BANK + 0x0005
    #define REG_CIPHER_DMXL_SCBFIX_MSK(_a_)     (0x00000007UL << REG_CIPHER_DMXL_SCBFIX_SHT(_a_))

    #define REG_CIPHER_EKEY_INVALID_EN         0x00000001UL
    #define REG_CIPHER_EKEY_INVALID_TSID_MSK   0x000000F0UL
    #define REG_CIPHER_EKEY_INVALID_TSID_SHT   4UL

#define REG_CIPHER_CSA2_CTRL(_a_)             (REG_DSCMB_CIPHER_BANK(_a_) + 0x000bUL)
    #define REG_CIPHER_CSA2_KEEP_TS_SCB        0x00000001UL
    #define REG_CIPHER_CSA2_KEEP_PES_SCB       0x00000002UL
    #define REG_CIPHER_CSA2_BYPASS_SCP         0x00000004UL
    #define REG_CIPHER_CSA2_BYPASS_SID         0x00000008UL
    #define REG_CIPHER_CSA2_FIX_SCB            0x00000010UL
    #define REG_CIPHER_CSA2_DIS_PKEND          0x00000020UL
    #define REG_CIPHER_CSA2_CLROVF             0x00000040UL
    #define REG_CIPHER_CSA2_SCB0_PKT           0x00000080UL

#define REG_CIPHER_ESA_DEFAULT_ALGO(_a_)	  REG_DSCMB_CIPHER_BANK(_a_) + 0x000CUL
	#define REG_CIPHER_ESA_MSK				  0x00007FFFUL
	#define REG_CIPHER_ESA_Algo_MSK           0x0000000FUL
	#define REG_CIPHER_ESA_Algo_SHT           0UL
	#define REG_CIPHER_ESA_SubAlgo_MSK        0x000000F0UL
	#define REG_CIPHER_ESA_SubAlgo_SHT        4UL
	#define REG_CIPHER_ESA_RES_MSK            0x00000700UL
	#define REG_CIPHER_ESA_RES_SHT            8UL
	#define REG_CIPHER_ESA_SB_MSK			  0x00007000UL
	#define REG_CIPHER_ESA_SB_SHT			  12UL

#define REG_CIPHER_PKTVIEW_STATUS(_a_)         (REG_DSCMB_CIPHER_BANK(_a_) + 0x001CUL)

////////////////////////////////////////////////////////////////////////////////////////////////
// cipher 1
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// tsce 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_TSCE0_BANK                   0x50480UL         //0x1A09               // 0xBF341200
#define REG_TSCE_CTRL                          (REG_DSCMB_TSCE0_BANK + 0x0000UL)
    #define REG_TSCE_CTRL_CLK_EN               0x00000001UL   // 0: disable, 1 : enable,
    #define REG_TSCE_SPS_RST_EN(_a_)           ( 0x10UL << (_a_))  // a = 0 ~ 3
    #define REG_TSCE_SPSPVR_RST_EN(_a_)        ( 0x100UL << (_a_))  // a = 0 ~ 3
    #define REG_TSCE_UPP_PATH0_PID_FLT_DISABLE_SHT    24UL
    #define REG_TSCE_UPP_PATH1_PID_FLT_DISABLE_SHT    25UL

// TSCE PVR engine control
#define REG_TSCE_SRC_SPS_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x0001UL + (_a_)) // a = 0 ~ 3
#define REG_TSCE_SRC_CLR_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x0005UL + (_a_)) // a = 0 ~ 5
#define REG_TSCE_SRC_TSO_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x000bUL + (_a_)) // a = 0 ~ 1
    #define REG_TSCE_SRC_EN                0x00000001UL
    #define REG_TSCE_SRC_MSK               0x00000004UL
    #define REG_TSCE_SRC_CH0               0x00000000UL
    #define REG_TSCE_SRC_CH1               0x00000004UL
    #define REG_TSCE_SRC_TSID_MSK          0x000000F0UL
    #define REG_TSCE_SRC_TSID_SHT          4UL
    #define REG_TSCE_SRC_CAVID_MSK         0x00001F00UL
    #define REG_TSCE_SRC_CAVID_SHT         8UL
    #define REG_TSCE_SRC_ENC_EN            0x00008000UL  // only used in SPS Encoding engine

#define REG_TSCE_MULTI2_SYSKEY(_a_)            (REG_DSCMB_TSCE0_BANK + 0x10UL + (_a_)) // a = 0 ~ 7
#define REG_TSCE_MULTI2_ROUND                  (REG_DSCMB_TSCE0_BANK + 0x18UL )



#if 0
////////////////////////////////////////////////////////////////////////////////////////////////
// tsce 1
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_TSCE1_BANK               0x50500         //0x1A0A                 // 0xBF341400
#define REG_TSCE_SPSPVR_CTRL                    (REG_DSCMB_TSCE1_BANK + 0x0000)
    #define REG_TSCE_SPSPVR_EN(_a_)             (0x01 << (_a_)) // a = 0 ~ 3
    #define REG_TSCE_SPSPVR_SWP(_a_)            (0x10 << (_a_)) // a = 0 ~ 3

/***************************************************
    PVR0 BUF0 >> 0x01 ~ 0x06 , BUF1 >> 0x07 ~ 0x0c
    PVR1 BUF0 >> 0x0d ~ 0x12 , BUF1 >> 0x13 ~ 0x18
    PVR2 BUF0 >> 0x19 ~ 0x1e , BUF1 >> 0x1f ~ 0x24
    PVR3 BUF0 >> 0x25 ~ 0x2a , BUF1 >> 0x2b ~ 0x30
****************************************************/
/// SPSPVR[_eng_]  buffer [_sel_]
#define REG_TSCE_SPSPVR_BUF_HEAD1(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 1 + (12 * (_eng_)) + (6 * (_sel_))) // eng = 0 ~ 3
#define REG_TSCE_SPSPVR_BUF_TAIL1(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 2 + (12 * (_eng_)) + (6 * (_sel_))) // sel = 0 ~ 1
#define REG_TSCE_SPSPVR_BUF_MID1( _eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 3 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_HEAD2(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 4 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_TAIL2(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 5 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_MID2( _eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 6 + (12 * (_eng_)) + (6 * (_sel_)))

#define REG_TSCE_SPSPVR_INT_CTRL                 (REG_DSCMB_TSCE1_BANK + 0x0031) // 32 bits
    #define REG_TSCE_SPSPVR_INT_CLR(_eng_ , _sel_)        (0x001 << (((_eng_)*2 )+(_sel_)) )  // eng = 0 ~ 3 , sel = 0 ~ 1
    #define REG_TSCE_SPSPVR_INT_MSK(_eng_ , _sel_)        (0x100 << (((_eng_)*2 )+(_sel_)) )  // eng = 0 ~ 3 , sel = 0 ~ 1

#define REG_TSCE_SPSPVR_WPTR_BASE (REG_DSCMB_TSCE1_BANK + 0x40)
#define REG_TSCE_SPSPVR_WPTR(_pvreng_, _bufsel_)  (REG_TSCE_SPSPVR_WPTR_BASE +( 2 * ( _pvreng_)) + (_bufsel_)) //pvreng = 0~3 ,  bufsel = 0~1
#endif


////////////////////////////////////////////////////////////////////////////////////////////////
// KTE
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_KTE_BANK                      0x50600UL         //0x1A0C                // 0xBF341800

    // Command interface 1
#define REG_KTE_CMD_CTRL0                    (REG_DSCMB_KTE_BANK + 0x0000UL)
    // command go
    #define REG_KTE_AKI_GO                      0x00000001UL
    // field selection
    #define REG_KTE_SEL_MSK        0x0000003EUL
    #define REG_KTE_SEL_SFT        1UL
    #define REG_KTE_SEL_LSAD       0x00000000UL
    #define REG_KTE_SEL_ESA        0x00000002UL
    #define REG_KTE_SEL_LSAS       0x00000004UL
    #define REG_KTE_SEL_SWITCH     0x00000006UL
    #define REG_KTE_SEL_IV         0x00000008UL
    #define REG_KTE_FIELD_LSAD       0x00000000UL
    #define REG_KTE_FIELD_ESA        0x00000001UL
    #define REG_KTE_FIELD_LSAS       0x00000002UL
    #define REG_KTE_FIELD_SWITCH     0x00000003UL
    #define REG_KTE_FIELD_IV         0x00000004UL

	// Key Is from KL
    #define REG_KTE_USE_KL          0x00000040UL

    // Enable Key after write //
    #define REG_KTE_AUTO_EN               0x00000080UL
    // command type
    #define REG_KTE_CMD_MSK         0x00000700UL
    #define REG_KTE_CMD_SFT         8UL
    #define REG_KTE_CMD_NOP         0x00000000UL
    #define REG_KTE_CMD_WRITE       0x00000100UL
    #define REG_KTE_CMD_KEY_DIS     0x00000200UL
    #define REG_KTE_CMD_READSW      0x00000300UL
    #define REG_KTE_CMD_KEY_EN      0x00000400UL
    #define REG_KTE_CMD_WRITE_KEY         0x1UL
    #define REG_KTE_CMD_INVALID_KEY       0x2UL
    #define REG_KTE_CMD_READSWITCH        0x3UL
    #define REG_KTE_CMD_ENABLE_KEY        0x4UL

    // Response
    #define REG_KTE_RESP_MSK        0x0000F800UL
    #define REG_KTE_RESP_SFT                11UL
        #define REG_KTE_RESP_OK                   0x0UL
        #define REG_KTE_RESP_NO_SLOT              0x1UL
	    #define REG_KTE_RESP_CONFLICT_CAVID1      0x2UL
	    #define REG_KTE_RESP_NOT_ALLOW_WRITE_KEY  0x3UL
	    #define REG_KTE_RESP_FORBID_BY_OTP_BITS   0x4UL
	    #define REG_KTE_RESP_NOT_ALLOW_INTRA_SLOT 0x5UL
	    #define REG_KTE_RESP_WAIT_10S             0x6UL

    // CAVID
    #define REG_KTE_CAVID_MSK                 0x001F0000UL
    #define REG_KTE_CAVID_SHT                 16UL

    // CMD interface 2
#define REG_KTE_CMD_CTRL1                    (REG_DSCMB_KTE_BANK + 0x0001UL)
    #define REG_KTE_FLTID_MSK                 0x000000FFUL
    #define REG_KTE_FLTID_SHT                 0UL
    #define REG_KTE_ETP_MSK                   0x00300000UL
    #define REG_KTE_SCB_MSK                   0x00030000UL
    #define REG_KTE_SCB_SHT                   16UL
    #define REG_KTE_FSCB_MSK                  0x000C0000UL
    #define REG_KTE_FSCB_SHT                  18UL
    #define REG_KTE_ETP_SHT                   20UL
    #define REG_KTE_ETP_64                    0UL
    #define REG_KTE_ETP_128                   1UL

    // Key Entropy
#define REG_KTE_ETPY_MSK                  0x00300000UL
#define REG_KTE_ETPY_SHT                  20UL
#define REG_KTE_ETPY_64                   (0x0UL<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_128                  (0x1UL<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_192                  (0x2UL<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_256                  (0x3UL<<REG_KTE_ETPY_SHT)
    // Key Usage
#define REG_KTE_USAGE_MSK                  0x7F000000UL
#define REG_KTE_USAGE_SHT                  24UL
#define REG_KTE_USAGE_NO_CSA2              (1UL<<24UL)
#define REG_KTE_USAGE_NO_CSA3              (1UL<<25UL)
#define REG_KTE_USAGE_NO_MULTI2            (1UL<<26UL)
#define REG_KTE_USAGE_NO_AES               (1UL<<27UL)
#define REG_KTE_USAGE_NO_TDES              (1UL<<28UL)
#define REG_KTE_USAGE_NO_DES               (1UL<<29UL)


#define REG_KTE_DATA(_a_)         (REG_DSCMB_KTE_BANK + 0x0012UL + (_a_)) /// 0<=a<=4, only for read


#define REG_KTE_DATA0                    (REG_DSCMB_KTE_BANK + 0x0002UL) /// only for write

    // [NonNDS] All Default Set to 0
    // [NDS mode Only]
#define REG_KTE_SWT_DEST                     (REG_DSCMB_KTE_BANK + 0x0002UL) /// trouble register
    #define REG_KTE_SW0_FORCESCB_MSK      0x00000003UL
    #define REG_KTE_SW0_SOURCE_MSK        0x0000FF00UL
    #define REG_KTE_SW0_SOURCE_SHT        8UL
    #define REG_KTE_SW0_LOWDEST_MSK       0x00FF0000UL
    #define REG_KTE_SW0_LOWDEST_SHT       16UL
    #define REG_KTE_SW0_UPPDEST_MSK       0xFF000000UL
    #define REG_KTE_SW0_UPPDEST_SHT       24UL
    #define REG_KTE_SW0_NDS_AESMODE_EN    0x00000008UL
    #define REG_KTE_SW0_NDS_AESMODE_PI    0x00000004UL


#define REG_KTE_DATA1                    (REG_DSCMB_KTE_BANK + 0x0003UL)
#define REG_KTE_DATA2                    (REG_DSCMB_KTE_BANK + 0x0004UL)
#define REG_KTE_DATA3                    (REG_DSCMB_KTE_BANK + 0x0005UL)

#define REG_KTE_SWT_ESA                      (REG_DSCMB_KTE_BANK + 0x0003UL) /// ESA SWITCH INTERFACE
#define REG_KTE_SWT_LSAS                     (REG_DSCMB_KTE_BANK + 0x0004UL) /// LSAS SWITCH INTERFACE
#define REG_KTE_SWT_LSAD                     (REG_DSCMB_KTE_BANK + 0x0005UL) /// LSAD SWITCH INTERFACE
    #define REG_KET_SWT_DECRYPT           0x00000001UL
 //   #define REG_KET_SWT_KEYENTROPY        0x00000010  //K3U01  //[NOTE] if Key size > 128 ==> Set this bit (according to algorithm)
 //   #define REG_KET_SWT_KEYUSAGE_MSK      0x00000F00  //K3U01
 //   #define REG_KET_SWT_KEYUSAGE_SHT      8           //K3U01
    #define REG_KET_SWT_SB_MSK            0x00007000UL
    #define REG_KET_SWT_SB_SHT            12UL
    #define REG_KET_SWT_RES_MSK           0x00070000UL
    #define REG_KET_SWT_RES_SHT           16UL
    #define REG_KET_SWT_SUBALG_MSK        0x00F00000UL
    #define REG_KET_SWT_SUBALG_SHT        20UL
    #define REG_KET_SWT_ALG_MSK           0x0F000000UL
    #define REG_KET_SWT_ALG_SHT           24UL


#define REG_KTE_DATA4                    (REG_DSCMB_KTE_BANK + 0x0006UL) /// trouble register
#define REG_KTE_SWT_SELECT                   (REG_DSCMB_KTE_BANK + 0x0006UL) /// trouble register
    // lower switch
    // upper switch
    #define REG_KTE_SW4_SWT_MSK            0x0000000FUL
    #define REG_KTE_SW4_LOWSWT_SHT         0UL
    #define REG_KTE_SW4_UPPSWT_SHT         4UL
    #define REG_KTE_SW4_A_EN               0x00000008UL
    #define REG_KTE_SW4_B_EN               0x00000004UL
    #define REG_KTE_SW4_D_EN               0x00000002UL
    #define REG_KTE_SW4_E_EN               0x00000001UL

    // MISC setting
    #define REG_KTE_SW4_CSA2_SBOX_MSK     0x00001F00UL
    #define REG_KTE_SW4_CSA2_SBOX_SHT     8UL
    #define REG_KTE_SW4_CSA2MOD_EN        0x00080000UL
    #define REG_KTE_SW4_PERMUTATION_MSK   0x00070000UL
    #define REG_KTE_SW4_PERMUTATION_SHT   16UL




#define REG_PIDIDX_CTRL                          (REG_DSCMB_KTE_BANK + 0x0008UL)
    // pid slot command start
    #define REG_PIDIDX_CMD_GO                    0x00000001UL
    // pid slot command : read/write
    #define REG_PIDIDX_DIR_MSK                   0x00000008UL
    #define REG_PIDIDX_DIR_READ                  0x00000000UL
    #define REG_PIDIDX_DIR_WIRTE                 0x00000008UL
    // pid slot command
    #define REG_PIDIDX_WMUX_MSK                   0x00000F00UL
        #define REG_PIDIDX_WMUX_EN                0x00000000UL
        #define REG_PIDIDX_WMUX_CAVID             0x00000100UL
        #define REG_PIDIDX_WMUX_TSID              0x00000200UL
        #define REG_PIDIDX_WMUX_EVEN_EN           0x00000300UL
        #define REG_PIDIDX_WMUX_ODD_EN            0x00000400UL
        #define REG_PIDIDX_WMUX_CLR_EN            0x00000500UL
        #define REG_PIDIDX_WMUX_DUAL_EN           0x00000600UL
        //#define REG_PIDIDX_WMUX_EVEN_IDX          0x00000700
        //#define REG_PIDIDX_WMUX_ODD_IDX           0x00000800
        //#define REG_PIDIDX_WMUX_CLR_IDX           0x00000900
        #define REG_PIDIDX_WMUX_PKTVIEW_DBG_INFO  0x00000E00UL
        #define REG_PIDIDX_WMUX_FPGA              0x00000F00UL
    // pid slot number
    #define REG_PIDIDX_FLTID_MSK                    0x00FF0000UL
    #define REG_PIDIDX_FLTID_SHT                    16UL

	//FPGA mode only
    #define REG_PIDIDX_KEYSLOT_MSK                    0x00FF0000UL
    #define REG_PIDIDX_KEYSLOT_SHT                    16UL

	#define REG_PIDIDX_FIELD_MSK                 0x0F000000UL
	#define REG_PIDIDX_FIELD_SHT                 24UL
		#define REG_PIDIDX_FIELD_LSAD                0x00000000UL
		#define REG_PIDIDX_FIELD_ESA				 0x01000000UL
		#define REG_PIDIDX_FIELD_LSAS				 0x02000000UL
		#define REG_PIDIDX_FIELD_SWITCH				 0x03000000UL
		#define REG_PIDIDX_FIELD_IV		  			 0x04000000UL

/// trouble register
#define REG_PIDIDX_WRITE_DATA                    (REG_DSCMB_KTE_BANK + 0x0009UL) //new position
#define REG_PIDIDX_READ_DATA                     (REG_DSCMB_KTE_BANK + 0x0019UL) //new position

    // write only, read is not allowed
    #define REG_PIDIDX_WDATA_MSK                 0x000000FFUL
    // read only, write is not allowed
    #define REG_PIDIDX_RDATA_MSK                 0xFFFFFFFFUL

    #define REG_PIDIDX_CLR_MSK                   0x0000007FUL
    #define REG_PIDIDX_CLR_SHT                   0UL
    #define REG_PIDIDX_DBG_INFO_MSK              0x00000080UL
    #define REG_PIDIDX_DBG_INFO_SHT              7UL
    #define REG_PIDIDX_ODD_MSK                   0x00007F00UL
    #define REG_PIDIDX_ODD_SHT                   8UL
    #define REG_PIDIDX_EVEN_MSK                  0x007F0000UL
    #define REG_PIDIDX_EVEN_SHT                  16UL

    #define REG_PIDIDX_DUAL_EN                   0x00800000UL
    #define REG_PIDIDX_CLR_EN                    0x01000000UL
    #define REG_PIDIDX_ODD_EN                    0x02000000UL
    #define REG_PIDIDX_EVEN_EN                   0x04000000UL

    #define REG_PIDIDX_CAVID_MSK                 0xF8000000UL
    #define REG_PIDIDX_CAVID_SHT                 27UL



// key array status register : LSAD // read only
#define REG_KTE_LSAD_STAT(_a_)                 (REG_DSCMB_KTE_BANK + 0x0040UL + (_a_) )
#define REG_KTE_LSAD_STAT_0                    (REG_DSCMB_KTE_BANK + 0x0040UL)
#define REG_KTE_LSAD_STAT_1                    (REG_DSCMB_KTE_BANK + 0x0041UL)
#define REG_KTE_LSAD_STAT_2                    (REG_DSCMB_KTE_BANK + 0x0042UL)
#define REG_KTE_LSAD_STAT_3                    (REG_DSCMB_KTE_BANK + 0x0043UL)

// key array status register : LSAS // read only
#define REG_KTE_LSAS_STAT(_a_)                 (REG_DSCMB_KTE_BANK + 0x0044UL + (_a_) )
#define REG_KTE_LSAS_STAT_0                    (REG_DSCMB_KTE_BANK + 0x0044UL)
#define REG_KTE_LSAS_STAT_1                    (REG_DSCMB_KTE_BANK + 0x0045UL)
#define REG_KTE_LSAS_STAT_2                    (REG_DSCMB_KTE_BANK + 0x0046UL)
#define REG_KTE_LSAS_STAT_3                    (REG_DSCMB_KTE_BANK + 0x0047UL)

// key array status register : ESA // read only
#define REG_KTE_ESA_STAT(_a_)                  (REG_DSCMB_KTE_BANK + 0x0048UL + (_a_) )
#define REG_KTE_ESA_STAT_0                     (REG_DSCMB_KTE_BANK + 0x0048UL)
#define REG_KTE_ESA_STAT_1                     (REG_DSCMB_KTE_BANK + 0x0049UL)
#define REG_KTE_ESA_STAT_2                     (REG_DSCMB_KTE_BANK + 0x004aUL)
#define REG_KTE_ESA_STAT_3                     (REG_DSCMB_KTE_BANK + 0x004bUL)

// key array status register : IV // read only
#define REG_KTE_IV_STAT(_a_)                   (REG_DSCMB_KTE_BANK + 0x004cUL + (_a_) )
#define REG_KTE_IV_STAT_0                      (REG_DSCMB_KTE_BANK + 0x004cUL)
#define REG_KTE_IV_STAT_1                      (REG_DSCMB_KTE_BANK + 0x004dUL)
#define REG_KTE_IV_STAT_2                      (REG_DSCMB_KTE_BANK + 0x004eUL)
#define REG_KTE_IV_STAT_3                      (REG_DSCMB_KTE_BANK + 0x004fUL)

// pid slot valid status register
#define REG_PIDIDX_STAT(_a_)                     (REG_DSCMB_KTE_BANK + 0x0050UL + (_a_) )
#define REG_PIDIDX_STAT_0                        (REG_DSCMB_KTE_BANK + 0x0050UL)
#define REG_PIDIDX_STAT_1                        (REG_DSCMB_KTE_BANK + 0x0051UL)
#define REG_PIDIDX_STAT_2                        (REG_DSCMB_KTE_BANK + 0x0052UL)
#define REG_PIDIDX_STAT_3                        (REG_DSCMB_KTE_BANK + 0x0053UL)
#define REG_PIDIDX_STAT_4                        (REG_DSCMB_KTE_BANK + 0x0054UL)
#define REG_PIDIDX_STAT_5                        (REG_DSCMB_KTE_BANK + 0x0055UL)
#define REG_PIDIDX_STAT_6                        (REG_DSCMB_KTE_BANK + 0x0056UL)
#define REG_PIDIDX_STAT_7                        (REG_DSCMB_KTE_BANK + 0x0057UL)


#define REG_KTE_FPGA_RDATA(_a_)                     (REG_DSCMB_KTE_BANK + 0x0060UL + (_a_) ) //_a_ = 0~ 3

////////////////////////////////////////////////////////////////////////////////////////////////
//  Key Ladder
////////////////////////////////////////////////////////////////////////////////////////////////
//1A0DH x32_KeyLadder0
//1A0EH x32_KeyLadder1
//1A0FH x32_KeyLadder2


#define REG_DSCMB_KL_BANK         (0x50680UL)         // 0x1A0D               // 0xBF341A00


#define REG_DSCMB_KL_SWRST                (REG_DSCMB_KL_BANK + 0x0BUL)
    #define REG_DSCMB_KL_SWRST_CW         (0x100UL)
    #define REG_DSCMB_KL_SWRST_PVR        (0x200UL)

#define REG_DSCMB_KL_TCSA3_ACT_CODE         (REG_DSCMB_KL_BANK + 0x24UL)    //CWKL only
#define REG_DSCMB_KL_TCSA3_CTRL             (REG_DSCMB_KL_BANK + 0x28UL)    //CWKL only
    #define REG_DSCMB_KL_TCSA3_START        (0x1UL)
    #define REG_DSCMB_KL_TCSA3_CHSEL_SHT    (1UL)
    #define REG_DSCMB_KL_TCSA3_CHSEL0       (0UL)
    #define REG_DSCMB_KL_TCSA3_CHSEL1       (1UL)

#define REG_DSCMB_KL_KDF_STATUS               (REG_DSCMB_KL_BANK + 0x4BUL )
    #define REG_DSCMB_KL_KDF_BUSY             (1UL)
    #define REG_DSCMB_KL_KDF_DONE             (0UL)

#define REG_DSCMB_KL_SW_CW(_a_)           (REG_DSCMB_KL_BANK + 0x00UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_NONCE_RESP(_a_)      (REG_DSCMB_KL_BANK + 0x04UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_ERR_MSG(_a_)         (REG_DSCMB_KL_BANK + 0x0AUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_ERR_MSG_SIZE         (32UL)
#define REG_DSCMB_KL_KEY_PROP(_a_)        (REG_DSCMB_KL_BANK + 0x0CUL +((_a_)*(0x40UL)))

#define REG_DSCMB_KL_KEY_BUS(_a_)         (REG_DSCMB_KL_BANK + 0x0DUL +((_a_)*(0x40UL)))
    #define     DSCMB_KL_KEYBUS_DSTDMA_SHT     (0UL)
    #define     DSCMB_KL_KEYBUS_TSID_SHT       (8UL)
    #define     DSCMB_KL_KEYBUS_SCB_SHT        (16UL)
    #define     DSCMB_KL_KEYBUS_FSCB_SHT       (18UL)
    #define     DSCMB_KL_KEYBUS_FIELD_SHT      (20UL)
    #define     DSCMB_KL_KEYBUS_PIDNO_SHT      (24UL)

#define REG_DSCMB_KL_KDF(_a_)             (REG_DSCMB_KL_BANK + 0x0EUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_KDF_EN            0x00100000UL
    #define REG_DSCMB_KL_KDF_APP_ID_MSK    0x0000FFFFUL
    #define REG_DSCMB_KL_KDF_APP_ID_SHT    0UL
    #define REG_DSCMB_KL_KDF_HASH_SEL_MSK  0x00010000UL
    #define REG_DSCMB_KL_KDF_HASH_SEL_SHT  16UL
    #define REG_DSCMB_KL_KDF_HWKEY_ID_MSK  0x000C0000UL
    #define REG_DSCMB_KL_KDF_HWKEY_ID_SHT  18UL

#define REG_DSCMB_KL_CTRL(_a_)            (REG_DSCMB_KL_BANK + 0x0FUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_CTRL_NONCE_EN         (0x4UL)
    #define REG_DSCMB_KL_CTRL_TF_TAB_EN        (0x1000UL)
#define REG_DSCMB_KL_ENC_CW(_a_)          (REG_DSCMB_KL_BANK + 0x10UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_KEY1(_a_)            (REG_DSCMB_KL_BANK + 0x14UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_KEY2(_a_)            (REG_DSCMB_KL_BANK + 0x18UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_KEY3(_a_)            (REG_DSCMB_KL_BANK + 0x1CUL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_KEY4(_a_)            (REG_DSCMB_KL_BANK + 0x20UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_PRED_NUM(_a_)        (REG_DSCMB_KL_BANK + 0x29UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_SW_KEY(_a_)          (REG_DSCMB_KL_BANK + 0x30UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_NONCE(_a_)           (REG_DSCMB_KL_BANK + 0x34UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_TF_ENC_CW(_a_)       (REG_DSCMB_KL_BANK + 0x38UL +((_a_)*(0x40UL)))
#define REG_DSCMB_KL_LUT3_BASE(_a_)       (REG_DSCMB_KL_BANK + 0x3cUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_LUT3_TF_TAB_DONE      (0x1UL)
    #define REG_DSCMB_KL_SRAM_SIZE             (512UL)  //KL SRAM size is 512 byte
    #define REG_DSCMB_KL_TD_SIZE               (416UL)  //Transformed data size is 416 byte
    #define REG_DSCMB_KL_KCV_SIZE              (16UL)   //KCV size is 1 byte, it will padding 16 byte
    #define REG_DSCMB_KL_KCV_SHT               (16UL)   //KCV size is 1 byte, it will padding 16 byte
    #define REG_DSCMB_KL_KCV_MASK              (0x1FF0000UL)
    #define REG_DSCMB_KL_LUT_SIZE              (256UL)  //LUT size is 256 byte
    #define REG_DSCMB_KL_LUT_MASK              (0x000001FFUL)
    #define REG_DSCMB_KL_LUT_SHT               (0L)
    #define REG_DSCMB_KL_M_SIZE                (128UL)  //M size is 128 byte
    #define REG_DSCMB_KL_M_MASK                (0x0003FE00UL)
    #define REG_DSCMB_KL_M_SHT                 (9UL)
    #define REG_DSCMB_KL_BC_SIZE               (16UL)   //BC size is 16 byte
    #define REG_DSCMB_KL_BC_MASK               (0x07FC0000UL)
    #define REG_DSCMB_KL_BC_SHT                (18UL)

#define REG_DSCMB_KL_SP_CTRL(_a_)          (REG_DSCMB_KL_BANK + 0x2AUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_SP_GO                 (1UL)
    #define REG_DSCMB_KL_SP_KEY_SRC            (4UL)
    #define REG_DSCMB_KL_SP_IV_SRC             (5UL)
    #define REG_DSCMB_KL_SP_KEY_DST_SHT        (8UL)
    #define REG_DSCMB_KL_SP_KEY_DST_MASK       (0x00000F00UL)
    #define REG_DSCMB_KL_SP_KEY_RESP_SHT       (28UL)
    #define REG_DSCMB_KL_SP_KEY_RESP_MASK      (0xF0000000UL)
#define REG_DSCMB_KL_SP_FORBID(_a_)        (REG_DSCMB_KL_BANK + 0x2BUL +((_a_)*(0x40UL)))
    #define REG_DSCMB_KL_SP_DIS_KL_KEY         (0x10)
    #define REG_DSCMB_KL_SP_DIS_KL_IV          (0x20)
#define REG_DSCMB_KL_SP_IV(_a_, _b_)       (REG_DSCMB_KL_BANK + 0x2CUL + (_b_) +((_a_)*(0x40UL)))

#define REG_DSCMB_KL_KCV_BASE                  (0x0)
#define REG_DSCMB_KL_LUT_BASE                  (REG_DSCMB_KL_KCV_BASE + REG_DSCMB_KL_KCV_SIZE)
#define REG_DSCMB_KL_M_BASE                    (REG_DSCMB_KL_LUT_BASE + REG_DSCMB_KL_LUT_SIZE)
#define REG_DSCMB_KL_BC_BASE                   (REG_DSCMB_KL_M_BASE + REG_DSCMB_KL_M_SIZE)

//new register here for REG_DSCMB_KL_BANK with 0x3C 0x3D


////////////////////////////////////////////////////////////////////////////////////////////////
//  ProgPVR Pid Filter Setting
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_SPSPVR_BANK                        0x50880UL         //0x1A11               // 0xBF342200
#define REG_DSCMB_SPSPVR_FLT(_Eng_,_a_)           (REG_DSCMB_SPSPVR_BANK + ((_Eng_) * 32UL) + (_a_)) // Eng = 0 ~ 3 , a = 0 ~ 31
    #define REG_DSCMB_SPSPVR_PID_MSK              0x1FFFUL
    #define REG_DSCMB_SPSPVR_BUF0_EN              0x2000UL
    #define REG_DSCMB_SPSPVR_BUF1_EN              0x4000UL


////////////////////////////////////////////////////////////////////////////////////////////////
// OTP
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_OTP_BANK  0x48000      //0x1900       //0xBF320000    x32_bridge_otp
#define REG_OTP_CH0_SWICH_COMB      (REG_OTP_BANK + 3925)



////////////////////////////////////////////////////////////////////////////////////////////////
// TSP Pid Filter
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_PIDFLT_BASE0                 (0x00240000UL)
#define REG_PIDFLT_BASE1                 (0x00241000UL)
#define REG_PIDFLT_BASE2                 (0x00242000UL)
#define REG_PIDFLT0(_a_)                 (REG_PIDFLT_BASE0 + ((_a_) * 4UL))
#define REG_PIDFLT1(_a_)                 (REG_PIDFLT_BASE1 + ((_a_) * 4UL))
#define REG_PIDFLT2(_a_)                 (REG_PIDFLT_BASE2 + ((_a_) * 4UL))

//PID Information
#define REG_TSP_PIDFLT_PID_MSK             0x00001FFFUL
#define REG_TSP_PIDFLT_PID_SHT             0UL
#define REG_TSP_PIDFLT_TS_SRC_MSK          0x0000E000UL
#define REG_TSP_PIDFLT_TS_SRC_SHT          13UL
#define REG_TSP_PIDFLT_KEYEN               0x00010000UL
#define REG_TSP_PIDFLT_2NDPID              0x00020000UL
#define REG_TSP_PIDFLT_PP1                 0x00040000UL
#define REG_TSP_PIDFLT_PP2                 0x00080000UL
#define REG_TSP_PIDFLT_PP3                 0x00100000UL
#define REG_TSP_PIDFLT_PP4                 0x00200000UL
#define REG_TSP_PIDFLT_PP_MSK              0x003C0000UL
#define REG_TSP_PIDFLT_CC_MSK              0xFF000000UL

//PID CA Destination
#define REG_TSP_PIDFLT_UPDST_MSK           0xF0000000UL
#define REG_TSP_PIDFLT_UPDST_SHT           28UL
#define REG_TSP_PIDFLT_LODST_MSK           0x0F000000UL
#define REG_TSP_PIDFLT_LODST_SHT           24UL

//PID Slot map information
#define REG_TSP_PIDFLT_PIDSLOTMAPNO_MSK    0x00FF0000UL
#define REG_TSP_PIDFLT_PIDSLOTMAPNO_SHT    16UL

////////////////////////////////////////////////////////////////////////////////////////////////
// TSP 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP0_BANK      0x00A80UL      //0x1015

////////////////////////////////////////////////////////////////////////////////////////////////
//  TSP 1
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP1_BANK                            0x00B00UL //TSP1 0x1016
#define REG_TSP_IDR_CTRL              (REG_TSP1_BANK + 0x0048UL)
    #define TSP_IDR_READ               0x0000UL
    #define TSP_IDR_START              0x0001UL
    #define TSP_IDR_WRITE              0x0002UL
#define REG_TSP_IDR_ADDR_L            (REG_TSP1_BANK + 0x0049UL)
#define REG_TSP_IDR_ADDR_H            (REG_TSP1_BANK + 0x004aUL)
#define REG_TSP_IDR_WRITE_L           (REG_TSP1_BANK + 0x004bUL)
#define REG_TSP_IDR_WRITE_H           (REG_TSP1_BANK + 0x004cUL)
#define REG_TSP_IDR_READ_L            (REG_TSP1_BANK + 0x004dUL)
#define REG_TSP_IDR_READ_H            (REG_TSP1_BANK + 0x004eUL)

////////////////////////////////////////////////////////////////////////////////////////////////
//  TSP 3
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP3_BANK            0x38100UL     //0x1702
#define REG_TSP_RCV_BUF_SRC              (REG_TSP3_BANK + 0x0011UL)
    #define REG_TSP_RCV_BUF0_SRC_MSK      0x0003UL    //Receive BUF0 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
    #define REG_TSP_RCV_BUF1_SRC_MSK      0x000cUL    //Receive BUF1 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
    #define REG_TSP_RCV_BUF2_SRC_MSK      0x0030UL    //Receive BUF2 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
    #define REG_TSP_RCV_BUF3_SRC_MSK	  0x00c0UL	//Receive BUF3 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb

////////////////////////////////////////////////////////////////////////////////////////////////
//  TSP 4
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP4_BANK      0x38180UL //0x1703    //
#define REG_TSP_CAVID0                      (REG_TSP4_BANK + 0x000aUL)
    #define REG_TSP_PIDFLT_BF0_CAVID_MSK     0x001FUL
    #define REG_TSP_PIDFLT_BF1_CAVID_MSK     0x03E0UL
    #define REG_TSP_PIDFLT_BF2_CAVID_MSK     0x7C00UL
    #define REG_TSP_PIDFLT_BF3_CAVID_MSK	 0x001FUL
#define REG_TSP_SPD_RESET                   (REG_TSP4_BANK + 0x001EUL)
    #define REG_TSP_SPD_TSIF0_RESET          0x0001UL
    #define REG_TSP_SPD_TSIF1_RESET          0x0002UL
    #define REG_TSP_SPD_TSIF2_RESET          0x0004UL
    #define REG_TSP_SPD_TSIF3_RESET          0x0008UL
#define REG_TSP_CLEAR_PIDPAIR_STATUS0       (REG_TSP4_BANK + 0x0020UL)
    #define REG_TSP_PIDFLT0_CLR_REPLACE_MSK  0x000FUL
    #define REG_TSP_PIDFLT1_CLR_REPLACE_MSK  0x00F0UL
    #define REG_TSP_PIDFLT2_CLR_REPLACE_MSK  0x0F00UL
    #define REG_TSP_PIDFLT3_CLR_REPLACE_MSK  0xF000UL
    #define REG_TSP_PIDFLT0_CLR_REPLACE_SHT  0x0UL
    #define REG_TSP_PIDFLT1_CLR_REPLACE_SHT  0x4UL
    #define REG_TSP_PIDFLT2_CLR_REPLACE_SHT  0x8UL
    #define REG_TSP_PIDFLT3_CLR_REPLACE_SHT  0xCUL
#define REG_TSP_PID_SLOT_MAP_NO             (REG_TSP4_BANK + 0x39UL)
    #define REG_TSP_USE_PID_SLOT_MAP_NO      0x8000
#define REG_TSP_SPD_BYPASS                          (REG_TSP4_BANK + 0x52UL)
    #define REG_TSP_SPD_TSIF0_BYPASS_EN      0x0001UL
    #define REG_TSP_SPD_TSIF1_BYPASS_EN      0x0002UL
    #define REG_TSP_SPD_TSIF2_BYPASS_EN      0x0004UL
    #define REG_TSP_SPD_TSIF3_BYPASS_EN      0x0008UL


#endif // #ifndef __REG_DSCMB_H__
