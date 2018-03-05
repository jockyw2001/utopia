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
/// file    jpeg_def.h
/// @brief  JPEG internal definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _JPEG_DEF_H_
#define _JPEG_DEF_H_

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define JPEG_STATIC     static

#define MSOS_GET_SYSTEM_TIME    1


#if defined(CHIP_EDISON)   // need to modify njpeg_def.h and jpeg_def.h simontaneously
#define SUPPORT_OJPD                    TRUE
#else
#define SUPPORT_OJPD                    FALSE
#endif

//JPD IP supporting feature
#define JPD_SUPPORT_3_HUFFMAN_TABLE                             FALSE//TRUE
#define JPD_SUPPORT_AUTO_PROTECT                                FALSE//TRUE

#define ENABLE_TEST_09_JPEGWriteProtectTest                     FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_2              FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_4              FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_8              FALSE
#define ENABLE_TEST_16_JPEGEnablePsaveModeTest                  FALSE
#define ENABLE_TEST_18_miu_sel_128M                             FALSE
#define ENABLE_TEST_18_miu_sel_64M                              FALSE
#define ENABLE_TEST_18_miu_sel_32M                              FALSE
#define ENABLE_TEST_22_AutoProtectFailTest                      FALSE

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define JPEG_BLOCK_TYPE         MS_S16
//------------------------------------------------------------------------------
#define JPEG_QUANT_TYPE         MS_S16
//------------------------------------------------------------------------------
// May need to be adjusted if support for other colorspaces/sampling factors is added
#define JPEG_MAXBLOCKSPERMCU    10
//------------------------------------------------------------------------------
#define JPEG_MAXHUFFTABLES      8
//------------------------------------------------------------------------------
#define JPEG_MAXQUANTTABLES     4
//------------------------------------------------------------------------------
#define JPEG_MAXCOMPONENTS      4
//------------------------------------------------------------------------------
#define JPEG_MAXCOMPSINSCAN     4
//------------------------------------------------------------------------------
//Increase this if you increase the max width!
#define JPEG_MAXBLOCKSPERROW    6144

#if 0//(defined (MSOS_TYPE_NOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size)                \
    do {                                                        \
        MS_U32 idx = 0;                                         \
        volatile MS_U8 *Dest = (volatile MS_U8 *)(pDstAddr);    \
        volatile MS_U8 *Src = (volatile MS_U8 *)(pSrcAddr);     \
        for (idx = 0; idx < (u32Size); idx++)                   \
        {                                                       \
            Dest[idx] = Src[idx];                               \
        }                                                       \
    }while(0)

#define JPEG_memset(pDstAddr, u8value, u32Size)                 \
    do {                                                        \
        MS_U32 idx = 0;                                         \
        volatile MS_U8 *Dest = (volatile MS_U8 *)(pDstAddr);    \
        for (idx = 0; idx < (u32Size); idx++)                   \
        {                                                       \
            Dest[idx] = (u8value);                              \
        }                                                       \
    }while(0)
#else
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size) memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define JPEG_memset(pDstAddr, u8value, u32Size)  memset((pDstAddr), (u8value), (u32Size))
#endif

#define JPEG_AtoU32(pData, u32value) \
    do { \
        u32value = 0; \
        while(('0' <= *pData) && ('9' >= *pData)) \
        { \
            u32value = (10 * u32value) + (MS_U32)(*pData - '0'); \
            pData++; \
        } \
    }while(0)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Description: JPEG scan type
typedef enum
{
    E_JPEG_GRAYSCALE    = 0
  , E_JPEG_YH1V1        = 1
  , E_JPEG_YH2V1        = 2
  , E_JPEG_YH1V2        = 3
  , E_JPEG_YH2V2        = 4
  , E_JPEG_YH4V1        = 5
//#if SW_JPD_RGB_CMYK
  , E_JPEG_CMYK         = 6
  , E_JPEG_RGB          = 7
//#endif
}JPEG_ScanType;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_QuantTbl
/// @brief \b Struct \b Description: Structure for JPEG Quantization Table
//-----------------------------------------------------------------------------
typedef struct //same as JPD_QuanTbl in drvJPD.h
{
    JPEG_QUANT_TYPE s16Value[64];   ///< value of Q table
    MS_BOOL bValid;                 ///< has Q table or not.
} JPEG_QuantTbl;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HuffInfo
/// @brief \b Struct \b Description: Structure for JPEG huffman information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Huff_num[17];   ///< number of Huffman codes per bit size
    MS_U8 u8Huff_val[256];  ///< Huffman codes per bit size

    MS_U8 u8Symbol[17];     ///< u8Huff_num in reverse order
    MS_U16 u16Code[17];     ///< Minimun code word
    MS_BOOL bValid;         ///< has huffman table or not
} JPEG_HuffInfo;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HuffTbl
/// @brief \b Struct \b Description: Structure for JPEG huffman table
//-----------------------------------------------------------------------------
typedef struct
{
    MS_S16 s16Look_up[256]; ///< the lookup of huffman code
    // FIXME: Is 512 tree entries really enough to handle _all_ possible
    // code sets? I think so but not 100% positive.
    MS_S16 s16Tree[512];    ///< huffman tree of huffman code
    MS_U8 u8Code_size[256]; ///< code size of huffman code
} JPEG_HuffTbl;


#endif //_JPEG_DEF_H_

