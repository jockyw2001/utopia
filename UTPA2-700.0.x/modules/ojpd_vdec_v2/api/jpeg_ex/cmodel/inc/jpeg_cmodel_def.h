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

#ifndef _JPEG_CMODEL_DEF_H
#define _JPEG_CMODEL_DEF_H

#include <stdio.h>

#define JPEG_STATIC

/********************************************************************************/
/*               Char definition    : 2 bytes font talbe                        */
/********************************************************************************/
#define CHAR_RETURNICON     0X0019
#define CHAR_ENTERICON      0X001A
#define CHAR_MOVEICON       0X001B
#define CHAR_UP             0x001C
#define CHAR_DOWN           0X001D
#define CHAR_LEFT           0X001E
#define CHAR_RIGHT          0X001F
#define CHAR_SPACE          0x0020
#define CHAR_PERCENT        0x0025
#define CHAR_APOSTROPHE     0x0027
#define CHAR_LEFT_QUOTE     0x0028
#define CHAR_RIGHT_QUOTE    0x0029
#define CHAR_STAR           0x002A
#define CHAR_PLUS           0x002B
#define CHAR_COMMA          0x002C
#define CHAR_MINUS          0x002D
#define CHAR_DOT            0x002E
#define CHAR_SLASH          0x002F
#define CHAR_0              0x0030
#define CHAR_1              0x0031
#define CHAR_2              0x0032
#define CHAR_3              0x0033
#define CHAR_4              0x0034
#define CHAR_5              0x0035
#define CHAR_6              0x0036
#define CHAR_7              0x0037
#define CHAR_8              0x0038
#define CHAR_9              0x0039
#define CHAR_COLON          0x003A
#define CHAR_GT             0x003E
#define CHAR_QM             0x003F
#define CHAR_AT             0x0040
#define CHAR_A              0x0041
#define CHAR_B              0x0042
#define CHAR_C              0x0043
#define CHAR_D              0x0044
#define CHAR_E              0x0045
#define CHAR_F              0x0046
#define CHAR_G              0x0047
#define CHAR_H              0x0048
#define CHAR_I              0x0049
#define CHAR_J              0x004A
#define CHAR_K              0x004B
#define CHAR_L              0x004C
#define CHAR_M              0x004D
#define CHAR_N              0x004E
#define CHAR_O              0x004F
#define CHAR_P              0x0050
#define CHAR_Q              0x0051
#define CHAR_R              0x0052
#define CHAR_S              0x0053
#define CHAR_T              0x0054
#define CHAR_U              0x0055
#define CHAR_V              0x0056
#define CHAR_W              0x0057
#define CHAR_X              0x0058
#define CHAR_Y              0x0059
#define CHAR_Z              0x005A
#define CHAR_US             0x005f
#define CHAR_a              0x0061
#define CHAR_b              0x0062
#define CHAR_c              0x0063
#define CHAR_d              0x0064
#define CHAR_e              0x0065
#define CHAR_f              0x0066
#define CHAR_g              0x0067
#define CHAR_h              0x0068
#define CHAR_i              0x0069
#define CHAR_j              0x006A
#define CHAR_k              0x006B
#define CHAR_l              0x006C
#define CHAR_m              0x006D
#define CHAR_n              0x006E
#define CHAR_o              0x006F
#define CHAR_p              0x0070
#define CHAR_q              0x0071
#define CHAR_r              0x0072
#define CHAR_s              0x0073
#define CHAR_t              0x0074
#define CHAR_u              0x0075
#define CHAR_v              0x0076
#define CHAR_w              0x0077
#define CHAR_x              0x0078
#define CHAR_y              0x0079
#define CHAR_z              0x007A
#define CHAR_CC             0x010D
#define CHAR_SIGNAL         0x0121
//extended ASCII
#define CHAR_EXT_C          0x00C7
#define CHAR_EXT_c          0x00E7
#define CHAR_EXT_e          0x00E9
#define CHAR_EXT_e2         0x00E8
#define CHAR_EXT_e3         0x00EA
#define CHAR_EXT_e4         0x00EB
#define CHAR_EXT_E2         0x00C8
#define CHAR_EXT_E3         0x00C9
#define CHAR_EXT_O          0x00D3
#define CHAR_EXT_a          0x00E1
#define CHAR_EXT_a1         0x00E0
#define CHAR_EXT_a2         0x00E2
#define CHAR_EXT_A          0x00C1
#define CHAR_EXT_u          0x00FA
#define CHAR_EXT_U          0x00DA
#define CHAR_EXT_n          0x00F1
#define CHAR_EXT_N          0x00D1
#define CHAR_EXT_I          0x00CD
#define CHAR_EXT_i          0x00ED
#define CHAR_EXT_i2         0x00EE
#define CHAR_EXT_o          0x00F3

#define MS_VA2PA(_addr_)        ((MS_U32)(_addr_))
#define MS_PA2KSEG0(_addr_)     ((MS_U32)(_addr_))
#define MS_PA2KSEG1(_addr_)     ((MS_U32)(_addr_))

#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size) memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define JPEG_memset(pDstAddr, u8value, u32Size)  memset((pDstAddr), (u8value), (u32Size))
#define JPEG_AtoU32(pData, u32value) \
    do { \
        u32value = 0; \
        while(('0' <= *pData) && ('9' >= *pData)) \
        { \
            u32value = (10 * u32value) + (MS_U32)(*pData - '0'); \
            pData++; \
        } \
    }while(0)

/********************************************************************************/
/* Primitive types                                                              */
/********************************************************************************/
#define ROM         //code
#define PRIVATE     static

typedef unsigned char BIT;
typedef unsigned char U8;
typedef unsigned short U16;            // WORD, U16
typedef unsigned long U32;
typedef unsigned long long U64;
typedef signed char         S8;
typedef signed short        S16;
typedef signed long         S32;
typedef signed long long    S64;

typedef void VOID;
typedef U8 BOOL;           // BOOLEAN, BOOL, bool, boolean

typedef U8 BitField8;     /*  8 bit bit field */
typedef U16 BitField16;    /* 16 bit bit field */   // same with BitField
typedef U32 BitField32;    /* 32 bit bit field */
typedef S32 FILEID;

typedef U32 MS_U32;
typedef U16 MS_U16;
typedef U8 MS_U8;
typedef S32 MS_S32;
typedef S16 MS_S16;
typedef S8 MS_S8;
typedef BOOL MS_BOOL;

/// data type hardware physical address
typedef U32 MS_PHYADDR; // 32bit physical address

/// union for long16 (16 bits/ 2 bytes)
typedef union LONG16_BYTE_t
{
    U16 u16Num;     ///< 16bits
    U8 u8Num[2];   ///< 8 bits x 2
    // Big Endian
    // u8Num[0]  MSB
    // u8Num[1]  LSB
    // Little Endian
    // u8Num[0]  LSB
    // u8Num[1]  MSB
} LONG16_BYTE;

/// union for long32 (32 bits/ 4 bytes)
typedef union LONG32_BYTE_t
{
    U32 u32Num;     ///< 32bits
    U16 u16Num[2];  ///< 16bits x2
    U8 u8Num[4];   ///< 8 bits x4
    // Big Endian
    // u8Num[0]  MSB
    // u8Num[1]
    // u8Num[2]
    // u8Num[3]  LSB
    // Little Endian
    // u8Num[0]  LSB
    // u8Num[1]
    // u8Num[2]
    // u8Num[3]  MSB
} LONG32_BYTE;

/// union for float (32 bits/ 4 bytes)
typedef union FLOAT_BYTE_T
{
    float fvalue;   ///< 32bits float
    U8 u8Num[4];    ///< 8 bits x 4
} FLOAT_BYTE;

#if 0
/********************************************************************************/
/*                              Macro for TDA1236D                              */
/********************************************************************************/
#define Data8   U8
#define Data16  S16
#define Data32  S32
#define bool    BOOL
#define Bool    BOOL
#define true    TRUE
#define false   FALSE
#endif

/********************************************************************************/
/*                              Macro for endianess                             */
/********************************************************************************/

#ifdef _BGEDIAN
    #define BE2ME16(x)  (x)
    #define BE2ME32(x)  (x)
    #define LE2ME16(x)  ByteSwap16(x)
    #define LE2ME32(x)  ByteSwap32(x)

    #define VARBYTE(var, n)    (((U8 *)&(var))[n])
#else
    #define BE2ME16(x)  ByteSwap16(x)
    #define BE2ME32(x)  ByteSwap32(x)
    #define LE2ME16(x)  (x)
    #define LE2ME32(x)  (x)

    #define VARBYTE(var, n)    (((U8 *)&(var))[sizeof(var) - n - 1])
#endif

#define ReadU16BE(b)  (((b)[0]<<8)|((b)[1]))
#define ReadU16LE(b)  (((b)[1]<<8)|((b)[0]))
#define ReadU32BE(b)  (((b)[0]<<24)|((b)[1]<<16)|((b)[2]<<8)|((b)[3]))
#define ReadU32LE(b)  (((b)[3]<<24)|((b)[2]<<16)|((b)[1]<<8)|((b)[0]))

#define ByteSwap16(x) (((x) & 0x00ff) << 8 | ((x) & 0xff00) >> 8)
#define ByteSwap32(x) \
    ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
     (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define GETU32BYTE(var, n) *((U8 *)(&(var))+(n))    // n should be 0, 1, 2 or 3
#define GETU16BYTE(var, n) *((U8 *)(&(var))+(n))    // n should be 0 or 1

///-----------------------------------------------------------------------------
/* Macro for bitwise                                                          */
///-----------------------------------------------------------------------------
// U8<->U4
#define HIGHNIBBLE(u8)              ((U8)(u8) >> 4)
#define LOWNIBBLE(u8)               ((U8)(u8) & 0x0f)
// U16<->U8
#define HIGHBYTE(u16)               ((U8)((u16) >> 8))
#define LOWBYTE(u16)                ((U8)(u16))
#define MAKEWORD(HiByte, LowByte)   ((((U16)(HiByte)) * 0x100) + (LowByte))
// U32<->U16
#define HIGHU16(u32)                ((U16)(u32>>16))
#define LOWU16(u32)                 ((U16)(u32))
#define MAKEU32(u16HiVal, u16LoVal) ( (U32)(u16HiVal)<<16 | (U32)(u16LoVal) )


#define BIT(loc)                    (1U << (loc))   // BIT, BITFLAG
#define MASK(x)                     (((1<<(x##_BITS))-1) << x##_SHIFT)
#define _BITMASK(loc_msb, loc_lsb)  ( BIT(loc_msb+1) - BIT(loc_lsb) )   // _BITMASK, _MASK1
#define BITMASK(x)                  _BITMASK(((1)?x),((0)?x))           // BITMASK(2:4), _MASK(2:4)


/// Macros for setting/clearing of bits
#define SETBIT(_data, _bit)         ((_data) |=  (0x1 << (_bit)))
#define CLRBIT(_data, _bit)         ((_data) &= ~(0x1 << (_bit)))
#define GETBIT(_data, _bit)         (((_data) >> (_bit)) & 0X1)
#define COMPLEMENT(a)               (~(a))


#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

///-----------------------------------------------------------------------------
/*                        Boolean value and Return value                      */
///-----------------------------------------------------------------------------
#undef  NULL
#define NULL 0

/// define MSRET U8
typedef U8 MSRET;
/// 0: MS Return Error
#define MSRET_ERROR     0
/// 1: MS Return OK
#define MSRET_OK        1

/// 0: FALSE
#define FALSE           0
/// 1: TRUE
#define TRUE            1

/// 0: FAIL
#define FAIL            0
/// 1: PASS
#define PASS            1

/// 0: Disable
#define DISABLE         0
/// 1: Enable
#define ENABLE          1

/// 0: NO
#define NO              0
/// 1: YES
#define YES             1

/// 0: Not highlight
#define NONE_HIGHLIGHT  0
/// 1: Highlight
#define HIGHLIGHT       1

/// 0: Front end not lock
#define FE_NOT_LOCK     0
/// 1: Front end lock
#define FE_LOCK         1
/// 2: Frond end AGC not lock
#define FE_AGC_NOT_LOCK 2

// Temporary and should be move later
/// definition for _ENABLE
#define _ENABLE         1
/// definition for _DISABLE
#define _DISABLE        0

#define ERROR 0xFFFF
#define _END_OF_TBL_    0xFF // end of register table
#define _REG_BASE_TBL   -2

//-------------------------------------------------------------------------------------------------
//  Version String Definition
//-------------------------------------------------------------------------------------------------
#define MSIF_TAG                    {'M','S','I','F'}                   // MSIF
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    0x0000                              // MStar Common library
#define MSIF_MOD                    0x0000                              // MStar Common library
#define MSIF_CHIP                   0x000B
#define MSIF_CPU                    '1'
#define MSIF_OS                     '0'

///-----------------------------------------------------------------------------
/*                      Handle - Font, Bitmap, Database                       */
///-----------------------------------------------------------------------------
/// Font handle, handle to font table in memory
typedef S8 FONTHANDLE;
/// Bitmap handle, handle to bitmap buffer
typedef S16 BMPHANDLE;
/// Database handle
typedef S8 DBHANDLE;

#define INVALID_FONTHANDLE  -1
#define INVALID_BMPHANDLE   -1
#define INVALID_DBHANDLE    -1


///-----------------------------------------------------------------------------
/*                               Macro function                               */
///-----------------------------------------------------------------------------
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define ABS(x)              ( ((x)>=0) ? (x) : (-x) )

#define BCD2Dec(x)          ((((x) >> 4) * 10) + ((x) & 0x0F))

#define _CONCAT( a, b )     a##b
#define CONCAT( a, b )      _CONCAT( a, b )

#define COUNTOF( array )    (sizeof(array) / sizeof((array)[0]))

#define UNUSED( var )       (void)((var) = (var))

#define ALIGN_4(_x)                     (((_x) + 3) & ~3)
#define ALIGN_16(_x)                    (((_x) + 15) & ~15)           // No data type specified, optimized by complier
#define ALIGN_32(_x)                    (((_x) + 31) & ~31)           // No data type specified, optimized by complier

#define DataAlignment_I(n, unit)        (((n)/(unit))*(unit))
#define DataAlignment(n, unit)          ((((n)+(unit)-1)/(unit))*(unit))

typedef struct
{
    U8 bBOOL_0    :1;
    U8 bBOOL_1    :1;
    U8 bBOOL_2    :1;
    U8 bBOOL_3    :1;
    U8 bBOOL_4    :1;
    U8 bBOOL_5    :1;
    U8 bBOOL_6    :1;
    U8 bBOOL_7    :1;
} BOOLEANS;


#ifndef NULL
#define NULL            (void *)0
#endif

typedef union _MSIF_Version
{
    struct _DDI
    {
        MS_U8                       tag[4];
        MS_U8                       class[2];
        MS_U16                      customer;
        MS_U16                      model;
        MS_U16                      chip;
        MS_U8                       cpu;
        MS_U8                       name[4];
        MS_U8                       version[2];
        MS_U8                       build[2];
        MS_U8                       change[8];
        MS_U8                       os;
    } DDI;

    struct _MW
    {
    } MW;

    struct _APP
    {
    } APP;

} MSIF_Version;
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

#endif /* _JPEG_CMODEL_DEF_H */


