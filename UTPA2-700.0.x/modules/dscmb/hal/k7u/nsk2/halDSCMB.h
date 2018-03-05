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

#ifndef __HAL_DSCMB_H__
#define __HAL_DSCMB_H__

#define DSCMB2_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define DSCMB2_DBGLV_ALERT     1  //A situation requiring immediate action.
#define DSCMB2_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define DSCMB2_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define DSCMB2_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define DSCMB2_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define DSCMB2_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define DSCMB2_DBGLV_DEBUG     7  //Used for debugging messages.
#define DSCMB2_DBGLV_ARRAY     8  //Uesd for array data.


#define NDS_IMPL                // tag for reconstructure the code

#define HAL_DSCMB_FLT_NULL                      0xFFFFFFFFUL
#define PID_SLOT_INDEX_NULL                     0x7FUL

//HAL_DSCMB_KTE_GetStatus(), for debug usage
#define DSCMB_PIDIDX_EN     0x80000000UL
#define DSCMB_KTE_VALID     0x40000000UL
#define DSCMB_KTE_DUAL_EN   0x10000000UL
#define DSCMB_KTE_LSAS_EN   0x00000001UL
#define DSCMB_KTE_ESA_EN    0x00000100UL
#define DSCMB_KTE_LSAD_EN   0x00010000UL
#define DSCMB_KTE_IV_EN     0x01000000UL

#define HAL_DSCMB_RIV_MAX      16UL  //max  number of RIV slot

#define HAL_DSCMB_KTE_MAX      128UL // 0 ~ 127 and 127 is reserved for NULL key
#define HAL_DSCMB_KTE_ID_NULL  127UL
#define HAL_DSCMB_PIDIDX_MAX   192UL
#define HAL_DSCMB_PIDFLT_NUM   192UL
#define HAL_DSCMB_TSIF_MAX      4UL
#define HAL_DSCMB_TSID_MAX      4UL
#define HAL_DSCMB_ENG_MAX       1UL
#define REG_DSCMB_PATH_CNT      HAL_DSCMB_TSID_MAX

#define HAL_DSCMB_ENG_NUM               1UL

#define HAL_DSCMB_KEY_TYPE_CLEAR        0UL
#define HAL_DSCMB_KEY_TYPE_EVEN         2UL
#define HAL_DSCMB_KEY_TYPE_ODD          3UL

#define HAL_DSCMB_KEYLEN_MAX            16UL
#define HAL_DSCMB_MULTI2_SYSKEY_MAX     32UL

#define HAL_DSCMB_ES_STATICKEY_MAX      16UL

#define HAL_DSCMB_SPSPVR_ENG_NUM        1UL  //only one spspvr filter in dscmb(32 filter pid)
#define HAL_DSCMB_SPSPVR_FLT_NUM        32UL
//#define HAL_DSCMB_SPSPVR_BUF_NUM        1

#define DSCMB_CAPVR_PIDTABLE_NUM  (2UL)
#define DSCMB_CAPVR_PIDFLT_MAX (HAL_DSCMB_SPSPVR_FLT_NUM*DSCMB_CAPVR_PIDTABLE_NUM)

#define DEFAULT_CAVID            (0x0FUL)

#define DSCMB_CWKL                (0UL)
#define DSCMB_PVRKL               (1UL)

#define DSCMB_KL_DST_CLASS_CPU     (0x80UL)
#define DSCMB_KL_DST_CLASS_TSIO    (0x800UL)
#define DSCMB_KL_DST_CLASS_KT      (0x8000UL)
#define DSCMB_KL_DST_CLASS_DMA     (0x800000UL)
#define DSCMB_KL_DST_CLASS_SPSSPD  (0x880000UL)
#define DSCMB_KL_DST_CLASS_PVT     (0x80000000UL)

//Tmp define here, need to put drvDSCMB.h in the future
#define    E_DSCMB_KL_DST_DMA_CSSK            0x00900000UL
typedef enum
{
    E_HAL_DSCMB_CAVID1 = 0x0001,
    E_HAL_DSCMB_CAVID2 = 0x0002,
    E_HAL_DSCMB_CAVID3 = 0x0003,
    E_HAL_DSCMB_CAVID4 = 0x0004,
    E_HAL_DSCMB_CAVID5 = 0x0005,
    E_HAL_DSCMB_CAVID6 = 0x0006,
    E_HAL_DSCMB_CAVID7 = 0x0007,
    E_HAL_DSCMB_CAVID8 = 0x0008,
    E_HAL_DSCMB_CAVIDF = 0x000F,
} HAL_DSCMB_CAVID;

typedef enum
{
    E_HAL_DSCMB_MAIN_ALGO_AES    = 0,
    E_HAL_DSCMB_MAIN_ALGO_CSA2   = 1,
    E_HAL_DSCMB_MAIN_ALGO_DES    = 2,
    E_HAL_DSCMB_MAIN_ALGO_TDES   = 3,
    E_HAL_DSCMB_MAIN_ALGO_MULTI2 = 4,
    E_HAL_DSCMB_MAIN_ALGO_CSA2_CONF =5,
    E_HAL_DSCMB_MAIN_ALGO_CSA3   = 6,
    E_HAL_DSCMB_MAIN_ALGO_ASA    = 7,
    E_HAL_DSCMB_MAIN_ALGO_TCSA3  = 8,
    E_HAL_DSCMB_MAIN_ALGO_ESSA   = 9,
    E_HAL_DSCMB_MAIN_ALGO_DEFAULT = 0xF,
    E_HAL_DSCMB_MAIN_ALGO_NUM,
} HAL_DSCMB_MainAlgo_Type;

typedef enum
{
    E_HAL_DSCMB_SUB_ALGO_MDI_CBC  = 0,
    E_HAL_DSCMB_SUB_ALGO_MDI_RCBC = 1,
    E_HAL_DSCMB_SUB_ALGO_MDD_CBC  = 2 ,
    E_HAL_DSCMB_SUB_ALGO_MDD_RCBC = 3,
    E_HAL_DSCMB_SUB_ALGO_LEADING_CLEAR = 4,
    E_HAL_DSCMB_SUB_ALGO_ECB = 5,
    E_HAL_DSCMB_SUB_ALGO_CBC = 6,
    E_HAL_DSCMB_SUB_ALGO_CTR = 7,
    E_HAL_DSCMB_SUB_ALGO_OFB = 8,
    E_HAL_DSCMB_SUB_ALGO_AESVAR = 9 ,
    E_HAL_DSCMB_SUB_ALGO_DEFAULT = 0xF,
    E_HAL_DSCMB_SUB_ALGO_NUM,
} HAL_DSCMB_SubAlgo_Type;


typedef enum
{
    E_HAL_DSCMB_RESSB_ALGO_CLR    = 0,
    E_HAL_DSCMB_RESSB_ALGO_CTS    = 1,
    E_HAL_DSCMB_RESSB_ALGO_SCTE52 = 2,
    E_HAL_DSCMB_RESSB_ALGO_XORIV1 = 3,
    E_HAL_DSCMB_RESSB_ALGO_OC_M   = 4,
    E_HAL_DSCMB_RESSB_ALGO_XORIV2 = 5,
    E_HAL_DSCMB_RESSB_ALGO_CTR    = 6,
    E_HAL_DSCMB_RESSB_ALGO_DEFAULT = 0x7,
    E_HAL_DSCMB_RESSB_ALGO_NUM,
} HAL_DSCMB_ResSBAlgo_Type;


typedef enum
{
    E_HAL_DSCMB_ENG_LSAD = 0 ,
    E_HAL_DSCMB_ENG_ESA  = 1  ,
    E_HAL_DSCMB_ENG_LSAS = 2 ,
    E_HAL_DSCMB_ENG_SWITCH   = 3 ,
    E_HAL_DSCMB_ENG_IV_LSAD  = 4,
    E_HAL_DSCMB_ENG_IV_ESA   = 5 ,
    E_HAL_DSCMB_ENG_IV_LSAS  = 6 ,
    E_HAL_DSCMB_ENG_NUM,
} HAL_DSCMB_KTE_WriteType;


// descrambler key ladder
typedef enum
{
    E_HAL_DSCMB_KEY_CLEAR =             0,
    E_HAL_DSCMB_KEY_UNDEFINE =          1,
    E_HAL_DSCMB_KEY_EVEN  =             2,
    E_HAL_DSCMB_KEY_ODD   =             3,
} HAL_DSCMB_KeyType;

typedef enum
{
    E_HAL_DSCMB_FSCB_UNCHG =             0,
    E_HAL_DSCMB_FSCB_B00   =             1,
    E_HAL_DSCMB_FSCB_B10   =             2,
    E_HAL_DSCMB_FSCB_B11   =             3,
} HAL_DSCMB_FSCB;


typedef enum
{
    E_HAL_DSCMB_SCBFIX_UNCHG  =             0,
    E_HAL_DSCMB_SCBFIX_USESCB =             1,
    E_HAL_DSCMB_SCBFIX_CLEAR  =             2,

    E_HAL_DSCMB_SCBFIX_ODD2EVEN =           4,
    E_HAL_DSCMB_SCBFIX_EVEN2ODD =           5,

    E_HAL_DSCMB_SCBFIX_ODD2CLEAR  =         6,
    E_HAL_DSCMB_SCBFIX_EVEN2CLEAR =         7,

} HAL_DSCMB_SCBFix;


typedef enum
{
    E_HAL_DSCMB_SCBFIX_LSAD =          0,
    E_HAL_DSCMB_SCBFIX_ESA  =          1,
    E_HAL_DSCMB_SCBFIX_LSAS =          2,
    E_HAL_DSCMB_SCBFIX_DMXU =          3,
    E_HAL_DSCMB_SCBFIX_DMXL =          4,
} HAL_DSCMB_SCBFix_EngSel;



typedef enum
{
    E_HAL_DSCMB_CIPHER_OUTPUT_SPS0 = 0 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_SPS1 = 1 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_SPS2 = 2 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_SPS3 = 3 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR0 = 4 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR1 = 5 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR2 = 6 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR3 = 7 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR4 = 8 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_CLR5 = 9 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_TSO0 = 10 ,
    E_HAL_DSCMB_CIPHER_OUTPUT_TSO1 = 11 ,

} HAL_DSCMB_OutPut_Eng_Sel;


typedef enum
{
    E_HAL_DSCMB_PARSER_CNT    =       0,
    E_HAL_DSCMB_PARSER_EVENT  =       1,
    E_HAL_DSCMB_PARSER_SCB    =       2,
    E_HAL_DSCMB_PARSER_BADPKT =       3,
    E_HAL_DSCMB_PARSER_2NDHIT =       4,
    E_HAL_DSCMB_PARSER_ESA    =       5,
    E_HAL_DSCMB_PARSER_LSA    =       6,
    E_HAL_DSCMB_PARSER_INFO   =       7,
} HAL_DSCMB_PktParser_Mode;


//---------------------------
//   KL enumerate
//---------------------------
typedef enum
{
    E_HAL_DSCMB_KL_DST_KT    = 0x0,
    E_HAL_DSCMB_KL_DST_DMA   = 0x1,
    E_HAL_DSCMB_KL_DST_CSSK  = 0x1,
    E_HAL_DSCMB_KL_DST_TSIO  = 0x2,
    E_HAL_DSCMB_KL_DST_ACPU  = 0x4,
    E_HAL_DSCMB_KL_DST_SCPU  = 0x5,
    E_HAL_DSCMB_KL_DST_PRI0  = 0x8,
    E_HAL_DSCMB_KL_DST_PRI1  = 0x9,
    E_HAL_DSCMB_KL_DST_PRI2  = 0xA,
    E_HAL_DSCMB_KL_DST_PRI3  = 0xB,

} HAL_DSCMB_KL_Dst;

typedef enum
{
    E_HAL_DSCMB_KL_DSTDMA_NA     = 0x0 ,
    E_HAL_DSCMB_KL_DSTDMA_SK0    = 0x0 ,
    E_HAL_DSCMB_KL_DSTDMA_SK1    = 0x1 ,
    E_HAL_DSCMB_KL_DSTDMA_SK2    = 0x2 ,
    E_HAL_DSCMB_KL_DSTDMA_SK3    = 0x3 ,

    E_HAL_DSCMB_KL_DSTDMA_SPS0   = 0xf ,
    E_HAL_DSCMB_KL_DSTDMA_SPS1   = 0xf ,
    E_HAL_DSCMB_KL_DSTDMA_SPS2   = 0xf ,
    E_HAL_DSCMB_KL_DSTDMA_SPS3   = 0xf ,

    E_HAL_DSCMB_KL_DSTDMA_SPD0   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_SPD1   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_SPD2   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_SPD3   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_SPD4   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_SPD5   = 0xf ,  //PVR
    E_HAL_DSCMB_KL_DSTDMA_CSSK   = 0xf ,  //CW
} HAL_DSCMB_KL_DstDma;

typedef enum
{
    E_HAL_DSCMB_KL_DST_SP_NA     = 0x0 ,
    E_HAL_DSCMB_KL_DST_SP_SPS0   = 0x4 ,
    E_HAL_DSCMB_KL_DST_SP_SPS1   = 0x5 ,
    E_HAL_DSCMB_KL_DST_SP_SPS2   = 0x6 ,
    E_HAL_DSCMB_KL_DST_SP_SPS3   = 0x7 ,
    E_HAL_DSCMB_KL_DST_SP_SPD0   = 0xC ,
    E_HAL_DSCMB_KL_DST_SP_SPD1   = 0xD ,
    E_HAL_DSCMB_KL_DST_SP_SPD2   = 0x8 ,
    E_HAL_DSCMB_KL_DST_SP_SPD3   = 0x9 ,
    E_HAL_DSCMB_KL_DST_SP_SPD4   = 0xA ,
    E_HAL_DSCMB_KL_DST_SP_SPD5   = 0xB ,
} HAL_DSCMB_KL_DstSP;

typedef enum
{
    E_HAL_DSCMB_KL_DSTKT_LSAD    = 0x0 ,
    E_HAL_DSCMB_KL_DSTKT_ESA     = 0x1 ,
    E_HAL_DSCMB_KL_DSTKT_LSAS    = 0x2 ,
} HAL_DSCMB_KL_DstKT;


typedef enum
{
    E_HAL_DSCMB_KL_SRC_ACPU  = 0,
    E_HAL_DSCMB_KL_SRC_SK1   = 1,
    E_HAL_DSCMB_KL_SRC_SK2   = 2,
    E_HAL_DSCMB_KL_SRC_SK3   = 3,
    E_HAL_DSCMB_KL_SRC_SK4   = 4,
    E_HAL_DSCMB_KL_SRC_SK5   = 5,
    E_HAL_DSCMB_KL_SRC_SK6   = 6,
    E_HAL_DSCMB_KL_SRC_SK7   = 7,
    E_HAL_DSCMB_KL_SRC_SK8   = 8,
    E_HAL_DSCMB_KL_SRC_SK9   = 9,
    E_HAL_DSCMB_KL_SRC_SK10  = 10,
    E_HAL_DSCMB_KL_SRC_SK11  = 11,
    E_HAL_DSCMB_KL_SRC_SK12  = 12,
    E_HAL_DSCMB_KL_SRC_SK13  = 13,
    E_HAL_DSCMB_KL_SRC_SK14  = 14,
    E_HAL_DSCMB_KL_SRC_SK15  = 15,

} HAL_DSCMB_KL_Src;

typedef enum
{
    E_HAL_DSCMB_KL_TYPE_0   = 0x0,
    E_HAL_DSCMB_KL_TYPE_1   = 0x1,
    E_HAL_DSCMB_KL_TYPE_2   = 0x2,
    E_HAL_DSCMB_KL_TYPE_3   = 0x3,
    E_HAL_DSCMB_KL_TYPE_4   = 0x4,
    E_HAL_DSCMB_KL_TYPE_5   = 0x5,
    E_HAL_DSCMB_KL_TYPE_1_KPR =0x9,
    E_HAL_DSCMB_KL_TYPE_1_2   =0xA,
    E_HAL_DSCMB_KL_TYPE_1_3   =0xB,
    E_HAL_DSCMB_KL_TYPE_5_SMI =0xD,
    E_HAL_DSCMB_KL_TYPE_LUT   =0xF,
    E_HAL_DSCMB_KL_TYPE_INVALID,
} HAL_DSCMB_KL_Type;


typedef enum
{
    E_HAL_DSCMB_KL_ALGO_TDES  = 0,
    E_HAL_DSCMB_KL_ALGO_AES   = 1,
} HAL_DSCMB_KL_Algo;

typedef struct
{
    MS_U32 u32LUT; //LUT3 fill in 512 bytes table, each has 9 bits to describe
    MS_U32 u32M;
    MS_U32 u32BC;
} DSCMB_KL_TA_LUT3;

typedef enum
{
    E_HAL_DSCMB_KL_ERR_MSG_KDF,
    E_HAL_DSCMB_KL_ERR_MSG_KL,
    E_HAL_DSCMB_KL_ERR_MSG_AES,
    E_HAL_DSCMB_KL_ERR_MSG_TDES,
    E_HAL_DSCMB_KL_ERR_MSG_LSA,
    E_HAL_DSCMB_KL_ERR_MSG_ESA,
    E_HAL_DSCMB_KL_ERR_MSG_DMA,
    E_HAL_DSCMB_KL_ERR_MSG_TSIO,
    E_HAL_DSCMB_KL_ERR_MSG_UACPU,
    E_HAL_DSCMB_KL_ERR_MSG_SEP,
    E_HAL_DSCMB_KL_ERR_MSG_NONCE,
    E_HAL_DSCMB_KL_ERR_MSG_TYPE,
    E_HAL_DSCMB_KL_ERR_MSG_PRV,
    E_HAL_DSCMB_KL_ERR_MSG_HK,
    E_HAL_DSCMB_KL_ERR_MSG_DACPU,
    E_HAL_DSCMB_KL_ERR_MSG_BADRK,
    E_HAL_DSCMB_KL_ERR_MSG_ILUT,
    E_HAL_DSCMB_KL_ERR_MSG_RSV1,
    E_HAL_DSCMB_KL_ERR_MSG_KLTYPE,
    E_HAL_DSCMB_KL_ERR_MSG_ZERO,
    E_HAL_DSCMB_KL_ERR_MSG_RSV2,
    E_HAL_DSCMB_KL_ERR_MSG_RSV3,
    E_HAL_DSCMB_KL_ERR_MSG_KCV,
    E_HAL_DSCMB_KL_ERR_MSG_BADIK,

}HAL_DSCMB_KL_Err_Msg;


//---------------------------
//    enumerate
//---------------------------
//ts_if
typedef enum
{
    E_HAL_DSCMB_TSIF0 = 0,
    E_HAL_DSCMB_TSIF1 = 1,
    E_HAL_DSCMB_TSIF2 = 2,
    E_HAL_DSCMB_TSIF3 = 3,
    E_HAL_DSCMB_TSIF_NUM,
} HAL_DSCMB_TSIF;

//pktDmx
typedef enum
{
    E_HAL_DSCMB_TSID0 = 0,
    E_HAL_DSCMB_TSID1 = 1,
    E_HAL_DSCMB_TSID2 = 2,
    E_HAL_DSCMB_TSID3 = 3,
    E_HAL_DSCMB_TSID_NUM,
} HAL_DSCMB_TSID;

typedef enum
{
    E_HAL_DSCMB_CHANNEL0 = 0,
    E_HAL_DSCMB_CHANNEL_NUM,
} HAL_DSCMB_CHANNEL;

typedef enum
{
    E_HAL_DSCMB_SPSPVR_NUM,
} HAL_DSCMB_SPSPVR;


typedef struct
{
    //for read:
    MS_U32 ClrSlotIndex : 7; //bit[6:0] clr slot index
    MS_U32 PktViewDbgInfo : 1; //bit[7]
    MS_U32 OddSlotIndex : 7; //bit[14:8] odd slot index
    MS_U32 PidSlotEn : 1; //bit[15]
    MS_U32 EvenSlotIndex : 7; //bit[22:16] even slot index
    MS_U32 DualSloEn : 1; //bit[23] dualpath_en
    MS_U32 ClrSloEn : 1; //bit[24] clr slot enable
    MS_U32 OddSloEn : 1; //bit[25] odd slot enable
    MS_U32 EvenSloEn : 1; //bit[26] even slot enable
    MS_U32 CA_VID : 5; //bit[31:27] cavid
} PidSlotMapRead_t;

typedef struct
{
    //for read:
    MS_U32 Privileged : 1; //bit[32] privileged
    MS_U32 SecurePidSlot : 1; //bit[33] pidslot_is_secure
    MS_U32 Reserved : 30;
} PidSlotMapRead2_t;

typedef struct
{

    MS_U32 ForceSCB : 2; //bit[1:0]
    MS_U32 NDS_AES_MODE_PI : 1;
    MS_U32 NDS_AES_MODE_EN : 1;
    MS_U32 Reg2_reserve_4_7 : 4;
    MS_U32 IuputSrc : 8;
    MS_U32 LowDest : 8;
    MS_U32 UppDest : 8;

    MS_U32 ESA_Decrypt : 1;
    MS_U32 Reg3_reserve_1_11 : 11;
    MS_U32 ESA_SB : 3;
    MS_U32 Reg3_reserve_15 : 1;
    MS_U32 ESA_RES : 3;
    MS_U32 Reg3_reserve_19 : 1;
    MS_U32 ESA_SubAlgo : 4;
    MS_U32 ESA_MainAlgo : 4;
    MS_U32 Reg3_reserve_28_31 : 4;

    MS_U32 LSAS_Decrypt : 1;
    MS_U32 Reg4_reserve_1_11 : 11;
    MS_U32 LSAS_SB : 3;
    MS_U32 Reg4_reserve_15 : 1;
    MS_U32 LSAS_RES : 3;
    MS_U32 Reg4_reserve_19 : 1;
    MS_U32 LSAS_SubAlgo : 4;
    MS_U32 LSAS_MainAlgo : 4;
    MS_U32 Reg4_reserve_28_31 : 4;

    MS_U32 LSAD_Decrypt : 1;
    MS_U32 Reg5_reserve_1_11 : 11;
    MS_U32 LSAD_SB : 3;
    MS_U32 Reg5_reserve_15 : 1;
    MS_U32 LSAD_RES : 3;
    MS_U32 Reg5_reserve_19 : 1;
    MS_U32 LSAD_SubAlgo : 4;
    MS_U32 LSAD_MainAlgo : 4;
    MS_U32 Reg5_reserve_28_31 : 4;

    MS_U32 Low_Switch : 4;
    MS_U32 Upp_Switch : 4;
    MS_U32 DVBCSA_Var : 5;
    MS_U32 Reg6_reserve_13_14 : 2;
    MS_U32 ModifyXrc : 1;
    MS_U32 Permutation : 3;
    MS_U32 CAVid : 5;
    MS_U32 RegCnt : 8;

} SwitchReg_Map_t;

typedef struct
{
#define ErrStatusMak 0x1FFFFFFF
	MS_U32 Key_KDF_KeyForbidden : 1;
	MS_U32 Key_KL_KeyForbidden : 1;
	MS_U32 Key_AES_KeyForbidden : 1;
	MS_U32 Key_TDES_KeyForbidden : 1;
	MS_U32 Key_LSA_KeyForbidden : 1;
	MS_U32 Key_ESA_KeyForbidden : 1;
	MS_U32 Key_DMA_KeyForbidden : 1;
	MS_U32 Key_HMAC_KeyForbidden : 1;
	MS_U32 Key_ACPU_KeyForbidden : 1;
	MS_U32 Key_SEP_KeyForbidden : 1;
	MS_U32 Key_LUT_KeyForbidden : 1;
	MS_U32 Key_Type_KeyForbidden : 1;
	MS_U32 Key_PrivateKey_KeyForbidden : 1;
	MS_U32 Key_SW_KeyForbidden : 1;
	MS_U32 Key_Reserve : 1;
	MS_U32 Key_BadRootKey : 1;

	MS_U32 KL_LUT_NotInit : 1;
	MS_U32 KL_LUT_GenWithoutSWReset : 1;
	MS_U32 KL_NotSupportKLType  : 1;
	MS_U32 KL_ZeroOrder : 1;
	MS_U32 KL_Reserve : 2;
	MS_U32 KL_LUT0_Err : 1;
	MS_U32 KL_BadInternalKey : 1;

	MS_U32 KeyBus_Resp : 5;
		#define KeyBusRespMsk            0x1F
		#define KeyBusRespOK             0x0
		#define KeyBusNoSlot             0x1F
		#define KeyBusNotAllowToWriteKey 0x19
		#define KeyBusAllOneOrZeroKey    0x10

	MS_U32 KeyBus_Done : 1;

	MS_U32 Biss_AllowWriteKey : 1;
	MS_U32 Biss_WriteKeyError : 1;

}KL_RegErrFlag_t;


#if 1 //put here temp, when sps/spd read, and want to open api, put these to drvDSCMB.h
typedef struct
{
    MS_U32                  pPvrBuf0;       ///< DMX PVR buffer 0 starting address
    MS_U32                  pPvrBuf1;       ///< DMX PVR buffer 1 starting address
    MS_U32                  u32PvrBufSize0;    ///< DMX PVR buffer 0 size
    MS_U32                  u32PvrBufSize1;    ///< DMX PVR buffer 1 size
    MS_U32                  u32ChannelEngId;
    MS_BOOL                 bEncrypt;
} DSCMB_SPSPVR_Info;


typedef enum
{
    E_DSCMB_SPSPVR_ENG0 = 0 ,
    E_DSCMB_SPSPVR_ENG1 = 1 ,
    E_DSCMB_SPSPVR_ENG2 = 2 ,
    E_DSCMB_SPSPVR_ENG3 = 3 ,
    E_DSCMB_SPSPVR_ENG_NUM,
} DSCMB_SPSPVR_ENG;

#endif

typedef enum
{
    // descrambler engine
    E_HAL_DSCMB_CAP_ENGINE_NUM,
    // descrambler slot
    E_HAL_DSCMB_CAP_FLT_NUM,
    // descrambler type supported
    E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM,
    // descrambler mapping, start tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_START,
    // descrambler mapping, end tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_END,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM,
    // share key region number
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM,
    // share key region start
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START,
    // share key region end
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END,
} HAL_DSCMB_Query_Type;


//--------------------------------------------------------------------------
//   function Declaration
//--------------------------------------------------------------------------
MS_BOOL HAL_DSCMB_SetBank(MS_VIRT u32Bank);
MS_BOOL HAL_DSCMB_GetBank(MS_VIRT * u32Bank);
MS_BOOL HAL_DSCMB_Init(void);
MS_BOOL HAL_DSCMB_OTPEnDSCMB(void);
MS_BOOL HAL_DSCMB_SetCAVid(MS_U32 u32CAVid);
MS_U32  HAL_DSCMB_GetCAVid(void);
MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData);

void    HAL_DSCMB_ReadPidSlotMap(MS_U32 u32FltId,PidSlotMapRead_t *pReadData);
MS_BOOL HAL_DSCMB_CC_Respin(MS_U32 u32CHNum, MS_U32 u32Tsid, MS_BOOL bEnable);

void   HAL_DSCMB_SetDBGLevel(MS_U32 u32Level);


//////////////////////////////////////
//////                                               ////
//////     PidSlotMap Control function     ///
//////                                               ////
//////////////////////////////////////
MS_BOOL HAL_DSCMB_PidIdx_SetCAVid(MS_U32 u32fltid , MS_U32 u32CAVid );
MS_BOOL HAL_DSCMB_PidIdx_SetTsId(MS_U32 u32fltid , MS_U32 u32TsId );
MS_BOOL HAL_DSCMB_PidIdx_SetSlotKeyIdx(MS_U32 u32FltId,DSCMB_Key_Type key, MS_U32 keyIdx);
MS_BOOL HAL_DSCMB_PidIdx_ClearSlotKeyIdx(MS_U32 u32FltId,DSCMB_Key_Type key);
MS_BOOL HAL_DSCMB_PidIdx_EnableSlot(MS_U32 u32FltId);
MS_BOOL HAL_DSCMB_PidIdx_DisableSlot(MS_U32 u32FltId);
MS_BOOL HAL_DSCMB_PidIdx_DualPath(MS_U32 u32fltid , MS_BOOL bDual );
MS_BOOL HAL_DSCMB_PidIdx_Enable(MS_U32 u32fltid , MS_BOOL bEnable);
MS_BOOL HAL_DSCMB_PidIdx_SetSecure(MS_U32 u32fltid , MS_BOOL bSecure);
MS_BOOL HAL_DSCMB_PidIdx_SetPrivilege(MS_U32 u32fltid , MS_BOOL bEnable );


////////////////////////////////////////////
//////                                                          ////
//////     Slot Control function  (Key & Switch)   ///
//////                                                          ////
/////////////////////////////////////////////

// Key-specific FSCB
#define ENABLE_KEY_FSCB
void HAL_DSCMB_KTE_Clear_KeyFSCB(MS_U32 u32fltid);
void HAL_DSCMB_KTE_Copy_KeyFSCB(MS_U32 u32fltid_Src, MS_U32 u32fltid_Dst);
MS_BOOL HAL_DSCMB_KTE_Write_KeyFSCB(MS_U32 u32FltId, MS_U32 u32KteSel, DSCMB_Key_Type eKeyType, HAL_DSCMB_FSCB eForceSCB);

MS_BOOL HAL_DSCMB_KTE_Read_Switch(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32CAVid, MS_U32 *u32data);
MS_BOOL HAL_DSCMB_KTE_Write_Key(MS_U32 u32fltid, DSCMB_Key_Type type,DSCMB_Eng_Type wtype, MS_U32 u32CAVid, MS_U8* u8key);
MS_BOOL HAL_DSCMB_KTE_Write_RIV(MS_U32 u32FltId, DSCMB_Key_Type eType, MS_U32 u32CAVid, MS_U8* u8key, MS_U32 u32RIVIdx);
MS_BOOL HAL_DSCMB_KTE_Write_IV(MS_U32 u32FltId, DSCMB_Key_Type eType, MS_U32 u32CAVid, MS_U8* u8key );
MS_BOOL HAL_DSCMB_KTE_Write_IV_Ex(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_U8* u8key );
MS_BOOL HAL_DSCMB_KTE_Key_Ctrl (MS_U32 u32fltid, DSCMB_Key_Type type, DSCMB_Eng_Type wtype, MS_U32 u32CAVid, MS_BOOL bEnable );
MS_BOOL HAL_DSCMB_KTE_Key_Ctrl_Ex(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_BOOL bEnable, MS_BOOL bIsKL);
MS_BOOL HAL_DSCMB_KTE_IV_Ctrl(MS_U32 u32FltId, DSCMB_Key_Type eType, MS_U32 u32CAVid, MS_BOOL bEnable );
MS_BOOL HAL_DSCMB_KTE_IV_Ctrl_Ex(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_BOOL bEnable );

MS_BOOL HAL_DSCMB_KTE_Write_Algo(MS_U32 u32fltid,DSCMB_Key_Type eKeyType,DSCMB_Eng_Type eEngType , MS_U32 u32CAVid,
                                 DSCMB_MainAlgo_Type eAlgoType, DSCMB_SubAlgo_Type eSubAlgo,
                                 DSCMB_ResSBAlgo_Type eRes, DSCMB_ResSBAlgo_Type eSB ,
                                 MS_BOOL bDecrypt );
MS_BOOL HAL_DSCMB_KTE_Write_Switch(MS_U32 u32fltid, DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 UppSwitch, MS_U32 LowSwitch);
MS_BOOL HAL_DSCMB_KTE_Write_Permu(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 u32per);
MS_BOOL HAL_DSCMB_KTE_Write_SBOX(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 u32Sbox, MS_BOOL bCSA2Mode);
MS_BOOL HAL_DSCMB_KTE_GetStatus(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 *KeyStatus);
MS_BOOL HAL_DSCMB_KTE_Write_SrcDst(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 Src, MS_U32 UppDst, MS_U32 LowDst);
MS_BOOL HAL_DSCMB_KTE_Write_FSCB(MS_U32 u32FltId ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, DSCMB_FSCB eForceSCB);
MS_BOOL HAL_DSCMB_KTE_Write_PacketSwitch(MS_U32 u32FltId ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, DSCMB_Eng_Type eUppSwitch, DSCMB_Eng_Type eLowSwitch);

// Set Multi2 syskey //
MS_BOOL HAL_DSCMB_KTE_Write_MULTI2_SysKey(MS_U8* Key , MS_U32 u32Len);
MS_BOOL HAL_DSCMB_KTE_Write_MULTI2_Round(MS_U32 u32Round) ;

MS_BOOL HAL_DSCMB_KTE_HW_SEM_Lock(MS_U32 u32WaitMs);
MS_BOOL HAL_DSCMB_KTE_HW_SEM_Unlock(void);

/////////////////////////////////////////////
//////                                                           ////
//////     Cipher Channel function                     ////
//////                                                          ////
/////////////////////////////////////////////

MS_BOOL HAL_DSCMB_Cipher_DualPath(MS_U32 u32CHNum, MS_BOOL bEnable);
MS_BOOL HAL_DSCMB_Cipher_TCSA3(MS_U32 u32CHNum, MS_BOOL bEnable);

MS_BOOL HAL_DSCMB_Cipher_Set_SCBFix(MS_U32 EngId ,MS_U32 u32tsif ,HAL_DSCMB_SCBFix_EngSel eEngSel ,
                                    HAL_DSCMB_SCBFix eSCBFix);

MS_U32 HAL_DSCMB_Cipher_Output_Ctrl(HAL_DSCMB_OutPut_Eng_Sel EngId, MS_U32 u32ChlSel , MS_BOOL bEnable , MS_BOOL bEncrypt ,
                                    MS_U32 u32TsId , MS_U32 u32CaVid) ;

MS_BOOL HAL_DSCMB_Cipher_ES_SetStaticKey(MS_U32 u32EngId, MS_U8 *pu8StaticKey, MS_U32 u32KeyLen);

MS_U32 HAL_DSCMB_PktParser_Ctrl( MS_U32 u32Eng, MS_U32 u32tsif, MS_BOOL bRst, MS_U32 u32Range ,
                                 HAL_DSCMB_PktParser_Mode u32viewer, MS_BOOL bEnable );

// TSP control
void HAL_DSCMB_SetTSPCADst(MS_U32 fltId, MS_U32 u32UpDst, MS_U32 u32LowDst);
void HAL_DSCMB_SetTSPPidSlotMap(MS_U32 u32FltId, MS_U32 u32PidSlotMapNo);
void HAL_DSCMB_PidFlt_EnableKey(MS_U32 fltId, MS_BOOL bEnable);
void HAL_DSCMB_Get_TsidInput(MS_U32 u32FltId, MS_U32 *u32Tsid);
void HAL_DSCMB_ConnectPath(MS_U32 u32Idx ,MS_BOOL bEnable);
void HAL_DSCMB_SetTSPCAVid(MS_U32 u32Tsid, MS_U32 CAVid);
void HAL_DSCMB_PidFlt_2ndPid(MS_U32 u32FltId, MS_BOOL bEnable);
void HAL_DSCMB_PidFlt_PIDPair(MS_U32 u32FltId, MS_U32 u32PPNo, MS_BOOL bEnable);
MS_BOOL HAL_DSCMB_ClearPidPairStatus(MS_U32 u32TsSrc, MS_U32 u32PidPairNum);
MS_BOOL HAL_DSCMB_PidFlt_SetPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec);
MS_BOOL HAL_DSCMB_PidFlt_ClearPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec);
MS_U32  HAL_DSCMB_PidFlt_GetPid(MS_U32 u32FltId);

// New
MS_U32  HAL_DSCMB_FltSrc2TSIF(DSCMB_TSIF ePidFltSrc);
MS_U32  HAL_DSCMB_FltSrc2PktDmx(DSCMB_TSIF ePidFltSrc);
MS_BOOL HAL_DSCMB_SPD_Enable(MS_U32 tsif);
MS_BOOL HAL_DSCMB_SPD_Disable(MS_U32 tsif);
MS_BOOL HAL_DSCMB_SPD_Reset(MS_U32 tsif);
void    HAL_DSCMB_PrintSwitchSetup(MS_U32 u32FltId, DSCMB_Key_Type type, MS_U32 u32CAVid);
void    HAL_DSCMB_PktParser_PrintEvent(MS_U32 event);
MS_U32  HAL_DSCMB_GetProgId(DSCMB_CAPVR_MODE eCaMode);
MS_U32  HAL_DSCMB_GetChannlId(DSCMB_TSIF ePidFltSrc);
MS_BOOL HAL_DSCMB_Cipher_SPS_Enable(HAL_DSCMB_OutPut_Eng_Sel EngId, MS_BOOL bEncrypt);
MS_BOOL HAL_DSCMB_PES_Enable(MS_U32 u32CHNum, MS_U32 u32Tsid, MS_BOOL bEnable);


// Key Ladder Functions //
MS_BOOL HAL_DSCMB_KL_Start( DSCMB_KL_SelEng eKLEng );
MS_BOOL HAL_DSCMB_KL_Reset(DSCMB_KL_SelEng eKLEng) ;
MS_BOOL HAL_DSCMB_KL_KeyProp(DSCMB_KL_SelEng eKLEng, MS_U32 u32CAVid, MS_U32 u32KeyUsg, MS_U32 u32KeyEtpy);
MS_BOOL HAL_DSCMB_KL_KeyBus_Ex(DSCMB_KL_SelEng eKLEng, MS_U32 u32FltId, MS_U32 u32Field, DSCMB_Key_Type eKeyType, DSCMB_KLDst eDst, HAL_DSCMB_FSCB eFSCB);
MS_BOOL HAL_DSCMB_KL_KeyBus(DSCMB_KL_SelEng eKLEng, MS_U32 u32FltId, MS_U32 u32Field, DSCMB_Key_Type eKeyType, DSCMB_KLDst eDst);
MS_BOOL HAL_DSCMB_KL_Ctrl(DSCMB_KL_SelEng eKLEng, DSCMB_KLEng eAlgo, DSCMB_KLType eType, DSCMB_KLSrc eSrc, DSCMB_KLDst eDst);
MS_BOOL HAL_DSCMB_KL_GetACPUOut(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Key, MS_U32 u32Size);
MS_BOOL HAL_DSCMB_KL_ACPURootKey(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Key);
MS_BOOL HAL_DSCMB_KL_Input(DSCMB_KL_SelEng eKLEng, MS_U32 u32In, MS_U8 *pu8Key);
MS_BOOL HAL_DSCMB_KL_ErrStatus(DSCMB_KL_SelEng eKLEng);
MS_BOOL HAL_DSCMB_KL_ErrMsg(DSCMB_KL_SelEng eKLEng, MS_U32 *pu32ErrMsg);
MS_BOOL HAL_DSCMB_KL_KDF_Busy(DSCMB_KL_SelEng eKLEng, MS_BOOL *pbBusy);
MS_BOOL HAL_DSCMB_KL_TCSA3_CHSel(MS_U32 u32Ch);
MS_BOOL HAL_DSCMB_KL_TCSA3_Start(void);
MS_BOOL HAL_DSCMB_KL_TCSA3_ActCode(MS_U8 *pu8Code, MS_U32 u32Size);

MS_BOOL HAL_DSCMB_KL_TA_LUT3(DSCMB_KL_SelEng eKLEng, DSCMB_KL_TA_LUT3 *LUT3Tab);
MS_BOOL HAL_DSCMB_KL_TA_EncCW(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8EncCW);
MS_BOOL HAL_DSCMB_KL_TA_EN(DSCMB_KL_SelEng eKLEng, MS_BOOL bTAEn);

MS_BOOL HAL_DSCMB_KL_ETSI_Nonce(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Nonce);
MS_BOOL HAL_DSCMB_KL_ETSI_Response(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Response);
void    HAL_DSCMB_KL_KDF_Disable( DSCMB_KL_SelEng  eKLEng);
void    HAL_DSCMB_KL_KDF_Enable( DSCMB_KL_SelEng  eKLEng, KDF_TYPE eKDFType, MS_U16  u16AppId, KDF_HW_KEY_SEL eHWKeyId);

MS_BOOL HAL_DSCMB_KL_SP_KeyCtrl(DSCMB_KL_SelEng  eKLEng, DSCMB_KLDst eDst);
MS_BOOL HAL_DSCMB_KL_SP_SetIV(DSCMB_KL_SelEng  eKLEng, MS_U8 *pu8IV);
MS_BOOL HAL_DSCMB_KL_SP_Start(DSCMB_KL_SelEng eKLEng);

MS_BOOL HAL_DSCMB_KL_HW_SEM_Lock(DSCMB_KL_SelEng eKLEng, MS_U32 u32WaitMs);
MS_BOOL HAL_DSCMB_KL_HW_SEM_Unlock(DSCMB_KL_SelEng eKLEng);

void HAL_DSCMB_KL_DstSecure(DSCMB_KL_SelEng eKLEng, MS_BOOL bDstSecure);

#endif
