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
/**************************************************************************
 * Copyright (c) 2008 MStar Semiconductor, Inc. All Rights Reserved.
 * Licensed to TTP_CLIENT_NAME
 **************************************************************************/
/** \file
 * \ingroup PrdCBEcorePLFservicesFrHsl
 * \ingroup ApiCBEcorePLFservicesFrHsl
 * \brief This file contains initialisation and interrupt handling routines
 * for the HSL.
 */
#if defined (MSOS_TYPE_LINUX)

/*******************************************************************************
** Includes
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "frhsl_ringbuffer.h"
#include "frhsl_cfg.h"
#include "frhsl_autotest.h"
#include "frhsl_RAWLOB.h"

#define M_KiTtiSetFilterType(nEWtYPE)        \
        {                                    \
            kiTtiStoredFilterType = nEWtYPE;      \
            if (kiTtiActiveFilterType != KI_FILTER_DISABLED) \
            {                                \
                kiTtiActiveFilterType = nEWtYPE;  \
            }                                \
        }


typedef enum KiFilterTypeTag
{
    KI_FILTER_NONE,
    KI_FILTER_SET,
    KI_FILTER_DISABLED

} KiFilterType;


static void  KiTtiDestroyOldFilterMemory (void);
static MS_S8 ***kiTtiSetFilter = NULL;

static void                  *kiTtiFilterMemory  = NULL;
static KiFilterType           kiTtiStoredFilterType = KI_FILTER_NONE;
static KiFilterType           kiTtiActiveFilterType = KI_FILTER_NONE;

/* This array stores instances of the HSL
 * Pointers to elements are to be passed to all buffer and autotest functions
 * in order to allow them to operate on a selected instance.
 */
#define HSL_INSTANCES 2
stHslController gHSL[HSL_INSTANCES];

/*******************************************************************************
** Defines
*******************************************************************************/

#define MS_ASSERT(_bool_)                                                                                      \
        {                                                                                                   \
            if ( ! ( _bool_ ) )                                                                             \
            {                                                                                               \
/*                UTL_printf("ASSERT FAIL: %s %s %s %d\n", #_bool_, __FILE__, __PRETTY_FUNCTION__, __LINE__); */ \
            }                                                                                               \
        }


/*******************************************************************************
** Global Data
*******************************************************************************/
/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB
 * Common MSB 2 bits for all filtered log points.
 * Common MSB 2 bits for all filtered log points.
 * All filtered log points will have the 2 most significant bits
 * set.
 */
#define FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB  ((MS_U16)0x4000)


/**
 * \def FR_HSL_LAST_HARD_CODED_LOG_POINT
 * Last log point ID which is used for special purposes in HSL.
 * Last log point ID which is used for special purposes in HSL.
 * All the log points from 0 to this value will have custom
 * handling either on the target or PC side.
 * set.
 */
#define FR_HSL_LAST_HARD_CODED_LOG_POINT ((MS_U16)0x000F)

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_MAX_NUMBER_FILTER_LOG_POINTS
 * Max number of log points for which run-time filtering can be applied
 * Max number of log points for which run-time filtering can be applied. No more
 * than this number of log points can have run-time filtering applied. The
 * reason for this limit is linked to the size of the MS_U16 array which is used to
 * maintain the enable/disable state for each such log point.
 * This define must match the algorith used by Silox when getting a new log point
 * ID. See GetNewLogId() is Silox source code.
 */
#define FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS  ((MS_U16)1024*64)

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE
 * Max size of FrHslDynamicLogPointArray[].
 * Max size of FrHslDynamicLogPointArray[].
 */
#define FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE ((MS_U16)(FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS/(8)))  /* Factor of 8 for number of bits per byte */

#define FR_HSL_DYNAMIC_GROUP_ID_ARRAY_SIZE ((MS_U8)(HSL_GROUP_ID_MAX/(sizeof(MS_U8)*8)))

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE
 * Log point ID Mask for allowed range of filtered log point IDs
 * Log point ID Mask for allowed range of filtered log point IDs
 * This represents the allowed bits that can be set barred the common MSB
 */
#define FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE   ((MS_U16)(FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS-1)) /* -1 to give 0x03FFF */

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_HEADER_LOG_POINT_ID_MASK
 * Mask giving the allowed bits that can be set in a log point ID for which run-time filtering is applied.
 * Mask giving the allowed bits that can be set in a log point ID for which run-time filtering is applied.
 * This define must match the rule applied by Silox when creating a new log point. @ GetNewLogId().
 */
#define FR_HSL_V2_FILTERED_HEADER_LOG_POINT_ID_MASK (FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB | FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE)

MS_U8 GetBufferId(MS_U16 u16gId)
{
    MS_U8 retval = 0;
    if( (HSL_INSTANCES > 1 ) && (u16gId == HSL_DBG_SN_PROFILING))
    {
        retval = 1;
    }
    return retval;
}

/* macros */

/** IMPORTANT NOTE: Cannot have fprintf() present when function instrumentation is
 *  switched on. The reason is that function instrumentation uses HSL log points
 *  but the HSL processing will not be ready at the boot up, and many boot-up
 *  function will be instrumented and therefore flood the stderr with this message */
#define CHECK_INIT(hSL, value) \
if (FALSE == hSL->bInit) \
{ \
    return value; \
}


#define M_FrHslAllocateBufferSpace(bUFFER, bUFpTR, sIZE, bUFiNFO) \
    if( !bufferAllocateWrite(bUFpTR, sIZE, &bUFiNFO) ) \
    { \
        return; \
    } \
    bUFFER = (MS_U8*)(bUFiNFO->u32bufferIndex);

#define M_FrHslFinalizeBufferWrite(hSL, bUFiNFO) \
    bufferAcknowledgeWrite(hSL, bUFiNFO);

// [HSL_RAWLOB_LENGTH_OF_HEADER];
#define M_FrHsl0ByteMsg(hDR, hSL) \
{ \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_HEADER, s_dataInfo); \
    M_FrHslWriteSPMsgHeader(hDR, u8Buffer + HSL_RAWLOB_OFFSET_OF_MSG_TYPE, s_dataInfo->u32timestamp ); \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}

/** Write a N arguments message to HSL.
 *  Write a N arguments message to HSL.
 *  \param MS_U32 hDR = Message header word.
 *  \param MS_U16 aRG1 = First argument of the message.
 *
 */
void M_FrHslNArgumentsMsg(MS_U32 u32Header, p_HslController _hsl, MS_U16 N, ...)
{
    stListNode *s_dataInfo;
    MS_U8 *u8Buffer;
    MS_BOOL bSecondIDEnable;
    MS_U16 u16FirstID;
    MS_U8 u16PacketCounts; /* not including packet-count field itself*/
    MS_U8 u8OffsetArg; /* offset of first data packet(0-th packet is the packet counts)*/

    va_list vlist;

    if (u32Header > FR_HSL_TRANSITION_ID)
    {
        bSecondIDEnable = TRUE;
        u16FirstID = 0xFF00 | ( (u32Header >> 16) & 0x00FF);
        u16PacketCounts = HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_SECOND_ID+N;
        u8OffsetArg = HSL_RAWLOB_OFFSET_OF_PACKET(2);
    }
    else
    {
        bSecondIDEnable = FALSE;
        u16FirstID = (MS_U16)u32Header;
        u16PacketCounts = N;
        u8OffsetArg = HSL_RAWLOB_OFFSET_OF_PACKET(1);
    }
    M_FrHslAllocateBufferSpace(u8Buffer, _hsl, HSL_RAWLOB_LENGTH_OF_VL_MSG(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+u16PacketCounts), s_dataInfo);
    M_FrHslWriteVLMsgHeader(u16FirstID, u8Buffer+HSL_RAWLOB_OFFSET_OF_MSG_TYPE, s_dataInfo->u32timestamp);
    M_FrHslWritePacketCounts(u16PacketCounts, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0));
    if (bSecondIDEnable)
    {
        M_FrHslWritePayload(u32Header & FR_HSL_TRANSITION_ID, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1));
    }
    {
        va_start(vlist, N);
        MS_U32 i;
        for ( i=0 ; i<N-1 ; i++)
        {
            M_FrHslWritePayload( (MS_U16)va_arg(vlist, unsigned int), u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2+i) );
        }
        M_FrHslWriteLastPayload( (MS_U16)va_arg(vlist, unsigned int), u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2+N-1) );
        va_end(vlist);
    }
    M_FrHslFinalizeBufferWrite(_hsl, s_dataInfo);
}

/* IMPORTANT: The following macro is used for string output so calculates length from first null character */
#define M_FrHslMS_U8ArrayMsg(u16Id, hSL, parray) \
{ \
    MS_U16  cOUNTER=0; \
    const MS_U16 *aRRAYpTR = (const MS_U16*)parray; \
    const MS_U8 * cVal =  (const MS_U8*) parray; \
    MS_U16 sizeMsg = 0; \
    MS_U8  remainder; \
    MS_U16 wordSize; \
    while(*cVal != '\0')  \
        {\
            cVal++; \
            ++sizeMsg; \
        } \
    /* Adding one for the terminating character */ \
    ++sizeMsg; \
    /* Getting the size as MS_U16 parts */ \
    wordSize =  (sizeMsg/2); \
    remainder = sizeMsg%2; \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+wordSize+remainder), s_dataInfo); \
    M_FrHslWriteVLMsgHeader(u16Id, u8Buffer+HSL_RAWLOB_OFFSET_OF_MSG_TYPE, s_dataInfo->u32timestamp); \
    M_FrHslWritePacketCounts(wordSize+remainder, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    /* Write Data */ \
    for (cOUNTER = 0; cOUNTER+1 < wordSize; cOUNTER++) \
    { \
        M_FrHslWritePayload(*aRRAYpTR, u8Buffer + HSL_RAWLOB_OFFSET_OF_PACKET(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+cOUNTER)); \
        aRRAYpTR++; \
    } \
    if (remainder == 0) \
    { \
        if (0!=wordSize) \
        { \
            M_FrHslWriteLastPayload(*aRRAYpTR, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+wordSize-1)); \
        } \
    } \
    else \
    { \
        if (0!=wordSize) \
        { \
            M_FrHslWritePayload(*aRRAYpTR, &u8Buffer[HSL_RAWLOB_OFFSET_OF_PACKET(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+wordSize-1)]); \
        } \
        /* Any remainder can only be the null char */ \
        M_FrHslWriteLastPayload(0x0000, &u8Buffer[HSL_RAWLOB_OFFSET_OF_PACKET(HSL_RAWLOB_NUMBER_OF_PACKETS_FOR_PACKET_COUNTS+wordSize)]); \
    } \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}


/** Write a message that is an array of MS_U16s.
    *  Write a message that is an array of MS_U16s.
    *  \param MS_U16 hDR = Message header word.
    *  \param MS_U16 nUM = Number of MS_U16s in the array.
    *  \param MS_U16 *aRRAY = Array of nUM MS_U16s.
    */
#define M_FrHslMS_U16ArrayMsg(u16Id, hSL, u16length, parray) \
{ \
    MS_S16  cOUNTER; \
    const MS_U16 *aRRAYpTR = (const MS_U16 *) parray; \
    /* With this macro we need extra space for: 1 word for length field, 2 words for address field */   \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(1+2+u16length), s_dataInfo); \
    M_FrHslWriteVLMsgHeader(u16Id, u8Buffer+HSL_RAWLOB_OFFSET_OF_MSG_TYPE, s_dataInfo->u32timestamp); \
    /* +2 for Address */ \
    M_FrHslWritePacketCounts(2+u16length, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    /* Write address */ \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) >> 16, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1)); \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) & 0xFFFF, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2)); \
    /* Write Data */ \
    for (cOUNTER = 0; cOUNTER < (MS_S16)(u16length - 1); cOUNTER++) \
    { \
        M_FrHslWritePayload(*aRRAYpTR, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(3+cOUNTER)); \
        aRRAYpTR++; \
    } \
    if (u16length > 0)\
    { \
        M_FrHslWriteLastPayload(*aRRAYpTR, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(3+cOUNTER)); \
    } \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}


/**  Write a message that is an array of MS_U32s.
*   Write a message that is an array of MS_U32s. The actual logging is done through 16 bit registers and so each 32 bit
*   value is converted to two 16 bit words. The last 16 bit word is logged depending on the value of lASTwORDeMPTY.
*   The macro is defined in accordance with the endianess of the DSP.
*  \param MS_U16 hDR = Message header word.
*  \param MS_U16 nUM = Number of MS_U32s in the array.
*  \param MS_U32 *aRRAY = Array of nUM MS_U32s.
*  \param MS_U16 lASTwORDeMPTY = Logging indicator for the last 16 bit word - if set to 1 the last 16 bit word is not logged.
 */
#if (!defined(BYTE_ORDER) || (defined(BYTE_ORDER) && (BYTE_ORDER != BIG_ENDIAN)))
#define M_FrHslMS_U32ArrayMsg(u16Id, hSL, u16length, parray, lASTwORDeMPTY) \
{ \
    MS_U16  cOUNTER; \
    const MS_U32 *aRRAYpTR = (const MS_U32 *) parray; \
    MS_U16 tEMPdATA = 0; \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    if( lASTwORDeMPTY) \
    { \
        M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(1+2+u16length*2-1), s_dataInfo); \
    } \
    else \
    { \
        M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(1+2+u16length*2), s_dataInfo); \
    } \
    M_FrHslWriteVLMsgHeader(u16Id, u8Buffer, s_dataInfo->u32timestamp); \
    if (lASTwORDeMPTY) \
    { \
        /* 2*nUM -1 +2 for Address */ \
        M_FrHslWritePacketCounts((2*(u16length)) + 1, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    } \
    else \
    { \
        /* 2*nUM +2 for Address */ \
        M_FrHslWritePacketCounts((2*(u16length)) + 2, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    } \
    /* Write address */ \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) >> 16, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1)); \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) & 0xFFFF, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2)); \
    /* Write Data */ \
    for (cOUNTER = 0; cOUNTER+1 < u16length ; cOUNTER++) \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) & 0xFFFF); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+cOUNTER*2)); \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) >> 16 ); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+cOUNTER*2+1)); \
        aRRAYpTR++; \
    } \
    if (lASTwORDeMPTY) \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) & 0xFFFF); \
        M_FrHslWriteLastPayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2)); \
    } \
    else \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) & 0xFFFF); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2)); \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) >> 16 ); \
        M_FrHslWriteLastPayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2+1)); \
    } \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}

#else

#define M_FrHslMS_U32ArrayMsg(u16Id, hSL, u16length, parray, lASTwORDeMPTY) \
{ \
    MS_U16  cOUNTER; \
    MS_U32 *aRRAYpTR = (const MS_U32 *) parray; \
    MS_U16 tEMPdATA = 0; \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    if( lASTwORDeMPTY) \
    { \
        M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(1+2+u16length*2-1), s_dataInfo); \
    } \
    else \
    { \
        M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(1+2+u16length*2), s_dataInfo); \
    } \
    M_FrHslWriteVLMsgHeader(u16Id, u8Buffer, s_dataInfo->u32timestamp); \
    if (lASTwORDeMPTY) \
    { \
        /* 2*nUM -1 +2 for Address */ \
        M_FrHslWritePacketCounts((2*(u16length)) + 1, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    } \
    else \
    { \
        /* 2*nUM +2 for Address */ \
        M_FrHslWritePacketCounts((2*(u16length)) + 2, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0)); \
    } \
    /* Write address */ \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) >> 16, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1)); \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) & 0xFFFF, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2)); \
    /* Write Data */ \
    for (cOUNTER = 0; cOUNTER+1 < u16length ; cOUNTER++) \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) >> 16 ); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+cOUNTER*2+1)); \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) & 0xFFFF); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+cOUNTER*2)); \
        aRRAYpTR++; \
    } \
    if (lASTwORDeMPTY) \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) >> 16 ); \
        M_FrHslWriteLastPayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2)); \
    } \
    else \
    { \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) >> 16 ); \
        M_FrHslWritePayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2)); \
        tEMPdATA = (MS_U16) ((MS_U32)(*aRRAYpTR) & 0xFFFF); \
        M_FrHslWriteLastPayload(tEMPdATA, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1+2+(u16length-1)*2+1)); \
    } \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}

#endif

/** Write a message that is an instance of a C typedef
    *  Write a message that is an instance of a C typedef
    *  \param MS_U16 hDR = Message header word.
    *  \param MS_U16 nUM = Number of MS_U16s in the array. When oddBytePresent is TRUE, then this length accounts for it.
    *  \param oddBytePresent = A 1 to indicate that sizeof(type) is odd number of bytes.
    *  \param MS_U16 *parray = address to an instance of typedef to log
    */
#define M_FrHslTypedefMsg(u16Id, hSL, u16length, oddBytePresent, parray)                                                  \
{                                                                                                                    \
    MS_S16  cOUNTER;                                                                                                 \
    const MS_U16 *aRRAYpTR = (const MS_U16 *) parray;                                                                            \
                /* Buffer Allocation of words are an extra: 1 for length field, 2 for address */         \
    stListNode *s_dataInfo; \
    MS_U8 *u8Buffer; \
    M_FrHslAllocateBufferSpace(u8Buffer, hSL, HSL_RAWLOB_LENGTH_OF_VL_MSG(3+u16length), s_dataInfo); \
    M_FrHslWriteVLMsgHeader(u16Id, u8Buffer+HSL_RAWLOB_OFFSET_OF_MSG_TYPE, s_dataInfo->u32timestamp);                                        \
    /* Writing out the total length which includes 2 for Address  */                         \
    M_FrHslWritePacketCounts(2+u16length, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(0));                                \
    /* Write address */                                                                                              \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) >> 16, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(1));                        \
    M_FrHslWritePayload(((MS_U32)aRRAYpTR) & 0xFFFF, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(2));                     \
    /* Write Data */                                                                                                 \
    for (cOUNTER=0; cOUNTER < (MS_S16)(u16length - 1); cOUNTER++)                                                    \
    {                                                                                                                \
        M_FrHslWritePayload(*aRRAYpTR, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(3+cOUNTER));                           \
        aRRAYpTR++;                                                                                                  \
    }                                                                                                                \
    /* For the last word, we need to check if oddBytePresent is set because we then need to only transfer one byte*/ \
    if (oddBytePresent)                                                                                              \
    {                                                                                                                \
        MS_U8 oddByte = *((const MS_U8*)aRRAYpTR);                                                                           \
        M_FrHslWriteLastPayload((MS_U16)oddByte, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(3+cOUNTER));                   \
    }                                                                                                                \
    else if (u16length > 0)                                                                                          \
    {                                                                                                                \
        M_FrHslWriteLastPayload(*aRRAYpTR, u8Buffer+HSL_RAWLOB_OFFSET_OF_PACKET(3+cOUNTER));                         \
    }                                                                                                                \
    M_FrHslFinalizeBufferWrite(hSL, s_dataInfo); \
}


/*******************************************************************************
* Define      : KI_MAX_ONE_BASE_SIG_BITMAP
* Group       : GKI, Test Interface, Internals
* Description : This gives the maximum size of a packed signal bitmap (1 bit
*                   per signal, 8 signals per byte) for one base.
*******************************************************************************/
#define      KI_MAX_ONE_BASE_SIG_BITMAP   (0x100 / 8)


/*******************************************************************************
* Macro       : M_KiTtiSetFilterType
* Group       : GKI, Test Interface, Internals
* Parameter   : KiFilterType nEWtYPE = The new filter type to be used.
* Returns     : Nothing
* Description : This macro is used change the current filter type being used.
*                   This updates the stored type and the active type if
*                   necessary.
******************************************************************************/
/* The following defines are dependent on log point IDs allocation
 * made by silox.exe for supernova/sn_ui/utopia. */
#define FILTERED_LOG_POINT_ID_LOWER_RANGE  (0x8000)
#define FILTERED_LOG_POINT_ID_UPPER_RANGE  (0xAFFF)

/***************************************************************************
 * Global variables (with default values at bootup for g_FrHslDrvConfig )
 ***************************************************************************/

/** This is the dynamic log point filter array.
 *  This is the dynamic log point filter array.
 *  Any Array logging and
 *  'filtered' printf log points (i.e. short log points) can be enabled/disabled
 *  individually and dynamically.
 */
MS_U8 FrHslDynamicLogPointArray[FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE];
MS_BOOL FrHslDynamicGroupIdArray[HSL_GROUP_ID_MAX];

/** Function kept for backwards compatibility of the interface
 *  Assumes that there is only one instance of the HSL buffer
 *  available on the default port 6000.
 */
MS_BOOL FrHslInitialise(const MS_PHYADDR PhyAddr, const MS_U32 u32BufByteLen)
{
    MS_U32 params[] = {PhyAddr, u32BufByteLen, 6000};
    return FrHslInitialiseMulti(1, params);
}

MS_BOOL FrHslInitialiseMulti(MS_U8 _ICount, MS_U32 *_params)
{
    assert(_ICount <= HSL_INSTANCES);
    stHslController *hsl=0;
    MS_U8 instance;
    MS_BOOL bHslInit = FALSE;

    bHslInit = FrHslInitialiseBegin();

    memset(FrHslDynamicLogPointArray, 0, FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE);

    MS_U16 i;
    for ( i = 0; i < HSL_GROUP_ID_MAX; i++)
    {
        FrHslDynamicGroupIdArray[i] = FALSE;
    }

    if( bHslInit )
    {
        bHslInit = FrHslV2InitialiseLogPointFiltering();   /* Apply compile time enabling/disabling of log points */
    }

    if( bHslInit )
    {
        bHslInit = FrHslV2InitialiseGroupIdFiltering();  /* Apply compile time enabling/disabling of group ids */
    }

    /* PARAMS: groups of 3
     * - physical address
     * - size
     * - port
     */
    if( bHslInit )
    {
        for(instance = 0; bHslInit && (instance<_ICount); instance++)
    {
       hsl = &gHSL[instance];

       hsl->u32Port = _params[instance*3+2];
       hsl->u8Instance = instance;

       hsl->logThreads[0].streamId = 0;
       hsl->logThreads[0].threadId = 0;
       hsl->logThreads[0].priority = 0;
       hsl->logThreads[0].typeId = output_thread;
       hsl->logThreads[0].funcPtr = bufferWriteThread;
       hsl->logThreads[0].funcParam = hsl;

       hsl->pendingWriteHead = 0;
       hsl->pendingWriteLength = 0;

       if (FALSE==hsl->bInit)
       {
               bHslInit = bufferInitialize(hsl, _params[instance*3+0], _params[instance*3+1]);

               if( bHslInit )
           {
                   bHslInit = FrHsl_AutoTest_Init(hsl);
           }
               else
           {
               bufferFinalize(hsl);
           }

               hsl->bInit = bHslInit;
       }
    }
    }
    bHslInit = FrHslInitialiseEnd();

    if( bHslInit )
    {
    M_FrHslPrintf0 (0x085A, HSL_DBG_UTOPIA_ALL_DEBUG,"HSL framework initialised.");
    printf ("HSL framework initialised.\n"); //message output for autotest to identify the hsl ready to get request
    }

    return bHslInit;
}


/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Enable a dynamically controllable logpoint.
 *  Enable a dynamically controllable logpoint.
 */
MS_BOOL FrHslDynamicLogPointEnable(const MS_U16 lgHdr /**< log header to enable */)
{
    //CHECK_INIT(FALSE);

    MS_U16 bitIndex, byteIndex, bitMask;

    /* Find wordIndex and bitMask in logging array */
    bitIndex  = (lgHdr & FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE);
    byteIndex = bitIndex >> 3;
    bitIndex &= 7;
    bitMask   = (1 << bitIndex);

    /* Set relevant bit in logging array */
    FrHslDynamicLogPointArray[byteIndex] |= bitMask;

    return TRUE;
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Enable a dynamically controllable logpoint.
 *  Enable a dynamically controllable logpoint.
 */
MS_BOOL FrHslDynamicLogPointDisable(const MS_U16 lgHdr /**< log header to disable */)
{
    //CHECK_INIT(FALSE);

    MS_U16 bitIndex, byteIndex, bitMask;

    /* Find wordIndex and bitMask in logging array */
    bitIndex  = (lgHdr & FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE);
    byteIndex = bitIndex >> 3;
    bitIndex &= 7;
    bitMask   = (1 << bitIndex);

    /* Set relevant bit in logging array */
    FrHslDynamicLogPointArray[byteIndex] &= ~bitMask;

    return TRUE;
}


/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Enable all the dynamically controllable logpoints.
 *  Enable all the dynamically controllable logpoints.
 */
MS_BOOL FrHslDynamicLogPointEnableAll(void)
{
    //CHECK_INIT(FALSE);

    memset(FrHslDynamicLogPointArray, 0xFF, FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE);

    return TRUE;
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Disable all the dynamically controllable logpoints.
 *  Disable all the dynamically controllable logpoints.
 */
MS_BOOL FrHslDynamicLogPointDisableAll(void)
{
    //CHECK_INIT(FALSE);

    memset(FrHslDynamicLogPointArray, 0, FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE);

    return TRUE;
}

MS_BOOL FrHslDynamicLogGroupEnable(const MS_U16 groupId /**< log header to enable */)
{
    //CHECK_INIT(FALSE);

    FrHslDynamicGroupIdArray[groupId] = TRUE;

    return TRUE;
}

/** \ingroup
 *  Enable all the group.
 */
MS_BOOL FrHslDynamicLogGroupEnableAll(void)
{
    //CHECK_INIT(FALSE);

    MS_U16 wordIndex;
    for (wordIndex = 0; wordIndex < HSL_GROUP_ID_MAX; wordIndex++)
    {
        FrHslDynamicGroupIdArray[wordIndex] = TRUE;
    }

    return true;
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Enable a dynamically controllable group.
 */
MS_BOOL FrHslDynamicLogGroupDisable(const MS_U16 groupId /**< log header to disable */)
{
    //CHECK_INIT(FALSE);

    FrHslDynamicGroupIdArray[groupId] = FALSE;

    return TRUE;
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Disable all the dynamically controllable group.
 */
MS_BOOL FrHslDynamicLogGroupDisableAll(void)
{
    //CHECK_INIT(FALSE);

    MS_U16 wordIndex;
    for (wordIndex = 0; wordIndex < HSL_GROUP_ID_MAX; wordIndex++)
    {
        FrHslDynamicGroupIdArray[wordIndex] = FALSE;
    }

    return TRUE;
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Enable/Disable single log point.
 *  Enable/Disable single log point.
 */
MS_BOOL FrHslV2ApplySingleLogPointFiltering(const MS_U16 lgHdr, const MS_BOOL enableState)
{
    //CHECK_INIT(FALSE);

    //MS_ASSERT((lgHdr & ~FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE) == FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB);

    MS_BOOL bRet = FALSE;
    if (enableState)
    {
        bRet = FrHslDynamicLogPointEnable(lgHdr);
    }
    else
    {
        bRet = FrHslDynamicLogPointDisable(lgHdr);
    }

    return bRet;
}


/** Apply compile time group id filtering */
MS_BOOL FrHslV2InitialiseGroupIdFiltering(void)
{
    FrHslDynamicGroupIdArray[HSL_DBG_UTOPIA_ALL_DEBUG] = TRUE;
    FrHslDynamicGroupIdArray[HSL_DBG_MW_ALL_DEBUG] = FALSE;
    FrHslDynamicGroupIdArray[HSL_DBG_SN_ALL_DEBUG] = FALSE;
    FrHslDynamicGroupIdArray[HSL_DBG_UTOPIA_HSL_DEBUG] = FALSE;
    FrHslDynamicGroupIdArray[HSL_DBG_SN_AUTO_TEST_DEBUG] = FALSE;
    return TRUE;
}
/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Apply compile time enabling of log points.
 *  Apply compile time enabling of log points.
 */
MS_BOOL FrHslV2InitialiseLogPointFiltering(void)
{

/*================ START OF AUTO-GENERATED CODE ===================
 * The following is used to initialise FrHslDynamicLogPointArray[]
 */

/* Herefollows an example of how to do compile time enabling of
log points via the define ENABLE_ESSENTIAL_LOG_POINTS.*/
/*
#      if defined(ENABLE_ESSENTIAL_LOG_POINTS)
       {
         MS_U16 arrayEnableEssentialLogPoints[] =
         {
           0xC007,0xC00A,0xC024,0xC026,0xC036,0xC038,0xC03C,0xC040,
           0xC041,0xC044,0xC049,0xC04B,0xC04E,0xC04F,0xC057,0xC05F
         };
         FrHslV2ApplyMultipleLogPointsFilter(arrayEnableEssentialLogPoints,sizeof(arrayEnableEssentialLogPoints)/sizeof(arrayEnableEssentialLogPoints[0]));
       }

#      endif *//* ENABLE_ESSENTIAL_LOG_POINTS */

/*================= END OF AUTO-GENERATED CODE ==================*/

/* Hereunder the user can enable/disable any filtered log points
 * E.g.   FrHslDynamicLogPointEnable(0xABCD);  To enable just log point 0xABCD
 *        FrHslDynamicLogPointDisable(0xABCD); To disable just log point 0xABCD
 */
   //FrHslDynamicLogPointEnable(0x7FFF);

    return TRUE;
}

MS_BOOL FrHslFinalize(void)
{
    int i=0;
    stHslController *hsl;

    for(i=0; i<HSL_INSTANCES; i++)
    {
        hsl = &gHSL[i];
        CHECK_INIT(hsl, FALSE);

        FrHsl_AutoTest_End(hsl);
        bufferFinalize(hsl);

        hsl->bInit = FALSE;
    }
    FrHslStop();
    return TRUE;
}

#if 1 // remove these test functions later.
      // Operation on FR_HSL_LAST_HARD_CODED_LOG_POINT added subsequent to this comment
      // so ensure functionality is not broken before these functions are disabled.

/**  Verifies if group id is filtered in or out. */
MS_BOOL FrHslDynamicGroupIdTest(MS_U8 groupId)
{
    return FrHslDynamicGroupIdArray[groupId];
}

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  Test if a log point is enabled.
 *  Test if a log point is enabled - used internally in the logging macros
 *  Exception is made of the 'hardcoded' log points which should always be
 *  enabled.
 *  to decide whether to process with logging.
 * \returns TRUE if log point is enabled.
 */
MS_BOOL FrHslDynamicLogPointTest(const MS_U16 lgHdr /**< log header to test */)
{
    MS_U16 bitIndex, byteIndex, bitMask;

    if (lgHdr <= FR_HSL_LAST_HARD_CODED_LOG_POINT )
    {
        return TRUE;  /* We ensure that such log points are allowed to be sent out. */
    }

    /* Find wordIndex and bitMask in logging array */
    bitIndex  = (lgHdr & FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE);
    byteIndex = bitIndex >> 3;
    bitIndex &= 7;
    bitMask   = (1 << bitIndex);

    /* Set relevant bit in logging array */
    if (FrHslDynamicLogPointArray[byteIndex] & bitMask)
       return TRUE;
    else
        return FALSE;

}
#endif

MS_BOOL FrHslCommand(p_HslController _hsl, CmdInfo* const pCmd)
{
    CHECK_INIT(_hsl, FALSE);

    return bufferProcessCommand(_hsl, pCmd);
}

/** Write a 0 argument message to HSL (just a message header, no data).
  * Write a 0 argument message to HSL (just a message header, no data). Equivalent
  * Equivalent to former LG_MES_0 macro.
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  */
void M_FrHslPrintf0(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHsl0ByteMsg(u32Id, hsl);
}

/** Write a 1 argument message to HSL (just a message header, no data).
  * Write a 1 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_1 macro.
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = argument value to log
  */
void M_FrHslPrintf1(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 1, u16Arg1);
}

/** Write a 2 argument message to HSL (just a message header, no data).
  * Write a 2 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_2 macro.
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  */
void M_FrHslPrintf2(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 2, u16Arg1, u16Arg2);
}

/** Write a 3 argument message to HSL (just a message header, no data).
  * Write a 3 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_3 macro.
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  */
void M_FrHslPrintf3(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 3, u16Arg1, u16Arg2, u16Arg3);
}


/** Write a 4 argument message to HSL (just a message header, no data).
  * Write a 4 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_4 macro.
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  */
void M_FrHslPrintf4(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 4, u16Arg1, u16Arg2, u16Arg3, u16Arg4);
}

/** Write a 5 argument message to HSL (just a message header, no data).
  * Write a 5 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_5 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  */
void M_FrHslPrintf5(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 5, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5);
}

/** Write a 6 argument message to HSL (just a message header, no data).
  * Write a 6 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  */
void M_FrHslPrintf6(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5, const MS_U16 u16Arg6)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 6, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6);
}


/** Write a 7 argument message to HSL (just a message header, no data).
  * Write a 7 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  */
void M_FrHslPrintf7(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 7, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7);
}

/** Write a 8 argument message to HSL (just a message header, no data).
  * Write a 8 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  */
void M_FrHslPrintf8(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 8, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8);
}

/** Write a 9 argument message to HSL (just a message header, no data).
  * Write a 9 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  */
void M_FrHslPrintf9(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 9, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9);
}

/** Write a 10 argument message to HSL (just a message header, no data).
  * Write a 10 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  */
void M_FrHslPrintf10(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 10, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10);
}

/** Write a 11 argument message to HSL (just a message header, no data).
  * Write a 11 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  */
void M_FrHslPrintf11(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 11, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11);
}

/** Write a 12 argument message to HSL (just a message header, no data).
  * Write a 12 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  * \param MS_U16 aRG12 = twelveth argument value to log
  */
void M_FrHslPrintf12(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11 , const MS_U16 u16Arg12)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 12, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11, u16Arg12);
}

/** Write a 13 argument message to HSL (just a message header, no data).
  * Write a 13 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  * \param MS_U16 aRG12 = twelveth argument value to log
  * \param MS_U16 aRG13 = thirteenth argument value to log
  */
void M_FrHslPrintf13(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11 , const MS_U16 u16Arg12, const MS_U16 u16Arg13)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 13, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11, u16Arg12, u16Arg13);
}

/** Write a 14 argument message to HSL (just a message header, no data).
  * Write a 14 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  * \param MS_U16 aRG12 = twelveth argument value to log
  * \param MS_U16 aRG13 = thirteenth argument value to log
  * \param MS_U16 aRG14 = fourteenth argument value to log
  */
void M_FrHslPrintf14(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11 , const MS_U16 u16Arg12, const MS_U16 u16Arg13, const MS_U16 u16Arg14)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 14, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11, u16Arg12, u16Arg13, u16Arg14);
}

/** Write a 15 argument message to HSL (just a message header, no data).
  * Write a 15 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  * \param MS_U16 aRG12 = twelveth argument value to log
  * \param MS_U16 aRG13 = thirteenth argument value to log
  * \param MS_U16 aRG14 = fourteenth argument value to log
  * \param MS_U16 aRG15 = fifteenth argument value to log
  */
void M_FrHslPrintf15(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11 , const MS_U16 u16Arg12, const MS_U16 u16Arg13, const MS_U16 u16Arg14, const MS_U16 u16Arg15)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 15, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11, u16Arg12, u16Arg13, u16Arg14, u16Arg15);
}

/** Write a 16 argument message to HSL (just a message header, no data).
  * Write a 16 argument message to HSL (just a message header, no data).
  * \param MS_U16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param MS_U16 aRG1 = first argument value to log
  * \param MS_U16 aRG2 = second argument value to log
  * \param MS_U16 aRG3 = third argument value to log
  * \param MS_U16 aRG4 = fourth argument value to log
  * \param MS_U16 aRG5 = fifth argument value to log
  * \param MS_U16 aRG6 = sixth argument value to log
  * \param MS_U16 aRG7 = seventh argument value to log
  * \param MS_U16 aRG8 = eighth argument value to log
  * \param MS_U16 aRG9 = nineth argument value to log
  * \param MS_U16 aRG10 = tenth argument value to log
  * \param MS_U16 aRG11 = eleventh argument value to log
  * \param MS_U16 aRG12 = twelveth argument value to log
  * \param MS_U16 aRG13 = thirteenth argument value to log
  * \param MS_U16 aRG14 = fourteenth argument value to log
  * \param MS_U16 aRG15 = fifteenth argument value to log
  * \param MS_U16 aRG16 = sixteenth argument value to log
  */
void M_FrHslPrintf16(const MS_U32 u32Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Arg1 , const MS_U16 u16Arg2, const MS_U16 u16Arg3, const MS_U16 u16Arg4, const MS_U16 u16Arg5,
                         const MS_U16 u16Arg6, const MS_U16 u16Arg7 , const MS_U16 u16Arg8, const MS_U16 u16Arg9, const MS_U16 u16Arg10,
                         const MS_U16 u16Arg11 , const MS_U16 u16Arg12, const MS_U16 u16Arg13, const MS_U16 u16Arg14, const MS_U16 u16Arg15, const MS_U16 u16Arg16)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);
    M_FrHslNArgumentsMsg(u32Id, hsl, 16, u16Arg1, u16Arg2, u16Arg3, u16Arg4, u16Arg5, u16Arg6, u16Arg7, u16Arg8, u16Arg9, u16Arg10, u16Arg11, u16Arg12, u16Arg13, u16Arg14, u16Arg15, u16Arg16);
}

/** Write a string message
 *  Write a string message
 *  \param MS_U16 hDR = Message header word.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param MS_U16 *pDATA = Array of lENGTH MS_U16s.
 */
void M_FrHslString(const MS_U16 u16Id, const MS_U16 u16GId, const char* u8Fmt, const void* pArray)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);

    if ( NULL==pArray )
    {
        return;
    }
    M_FrHslMS_U8ArrayMsg(u16Id, hsl, pArray);
}


/** Write a message that is an array of MS_U16s.
 *  Write a message that is an array of MS_U16s. Equivalent to former LG_MS_U16_DATA macro.
 *  \param MS_U16 hDR = Message header word.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param MS_U16 lENGTH = Number of MS_U16s in the array.
 *  \param MS_U16 *pDATA = Array of lENGTH MS_U16s.
 */
void M_FrHslArray16(const MS_U16 u16Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16length, const void* pArray)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);

    if ( NULL==pArray )
    {
        return;
    }
    M_FrHslMS_U16ArrayMsg(u16Id, hsl, u16length, pArray);
}

/**  Write a message that is an array of MS_U32s. Filter applied at run time.
 *   Write a message that is an array of MS_U32s. Filter applied at run time.
 *  \param MS_U16 hDR = Message header word.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param MS_U16 lENGTH = Number of MS_U32s in the array.
 *  \param MS_U32 *pDATA = Array of lENGTH MS_U32s.
 *  \param MS_U16 lASTwORDeMPTY = Logging indicator for the last 16 bit word - if set to 1 the last 16 bit word is not logged.
 */
void M_FrHslArray32(const MS_U16 u16Id, const MS_U16 u16GId, const char* u8Fmt, const MS_U16 u16Length, const void* pArray, const MS_BOOL bLastWordEmpty)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);

    if ( NULL==pArray )
    {
        return;
    }
    M_FrHslMS_U32ArrayMsg(u16Id, hsl, u16Length, pArray, bLastWordEmpty);
}

void FrHslTypedefCall(const MS_U16 u16Id, const MS_U16 u16GId, const MS_U16 u16Length, const void* pArray, const MS_BOOL bOddBytePresent)
{
    p_HslController hsl = &gHSL[GetBufferId(u16GId)];
    CHECK_INIT(hsl,);

    if ( NULL==pArray )
    {
        return;
    }
    M_FrHslTypedefMsg(u16Id, hsl, u16Length, bOddBytePresent, pArray);
}


MS_BOOL FrHslAddDynamicLibInfo(void* baseAddress, void* onLoadAddr, const char *libFileName)
{
    int i = 0;
    MS_BOOL retVal = true;

	if(gDynamicLibraryCount >= HSL_MAX_LIBRARY_COUNT)
    {
       retVal = false;
	   return retVal;
    }

    for(i=0; i<gDynamicLibraryCount && i<HSL_MAX_LIBRARY_COUNT; i++)
    {
        if(gDynamicLibraryInfo[i].base_address == baseAddress)
        {
            break;
        }
    }
    if( i < HSL_MAX_LIBRARY_COUNT )
    {
        strncpy(gDynamicLibraryInfo[gDynamicLibraryCount].name, libFileName, HSL_MAX_LIBRARY_NAME - 1);
        gDynamicLibraryInfo[gDynamicLibraryCount].base_address = baseAddress;
        gDynamicLibraryInfo[gDynamicLibraryCount].on_load_addr = onLoadAddr;
        gDynamicLibraryCount ++;
    }
    else
    {
        M_FrHslPrintf0 (0x0DF5, HSL_DBG_SN_PROFILING, "System profiling, dynamic library info table full!:");
        retVal = false;
    }
    M_FrHslTypedef (0x87D3, HSL_DBG_SN_PROFILING, "Dynamic library base address: %d", ST_LIBRARY_SHORT_INFO, &gDynamicLibraryInfo[i]);
    return retVal;
}

MS_BOOL FrHslAddThreadInfo(const MS_U32 threadId, const char* threadName)
{
    int i = 0;
    MS_BOOL retVal = true;
    for(i=0; i<gThreadCount && i<HSL_MAX_THREAD_COUNT; i++)
    {
        if(gThreadInfo[i].thread_ID == threadId)
        {
            break;
        }
    }
	if(gThreadCount >= HSL_MAX_THREAD_COUNT)
    {
       retVal = false;
	   return retVal;

    }
    if( i < HSL_MAX_THREAD_COUNT )
    {
        strncpy(gThreadInfo[gThreadCount].thread_name, threadName, HSL_MAX_THREAD_NAME - 1);
        gThreadInfo[gThreadCount].thread_ID = threadId;
        gThreadCount ++;
    }
    else
    {
        M_FrHslPrintf0 (0x0E25, HSL_DBG_SN_PROFILING, "System profiling, thread name table full!:");
        retVal = false;
    }
    M_FrHslTypedef (0x8DA6,  HSL_DBG_SN_PROFILING, "Thread name: %d", ST_THREAD_INFO, &gThreadInfo[i]);
    return retVal;

}

/*******************************************************************************
* Function    : KiAllocMemory
* Group       : GKI, Test Interface, Internals
* Parameter   : ptr
* Returns     : Nothing
* Description : allocate any memory
******************************************************************************/
#define KiAllocMemory(size, ptr)    \
{                                   \
    *ptr = malloc(size);            \
}

/** Process a Sets filter matrix data structure.
 * This function converts the Sets Filter matrix structure as used by the
 * associated signal or when using a static filter to the run time structure.
 * A length is required by this function as the sets filter data is variable
 * length.
 * \param[in] signalData  The sets filter matrix data.
 */
void KiTtiProcessSetsFilter(const KiSetsFilterMatrix * signalData)
{
    MS_U32        dataSize;
    MS_U8      ** basePointer;
    MS_U8       * sigPointer;
    const MS_U8 * sourcePointer;
    MS_U32        counter;
    MS_U32        tempData;
    MS_U32        oldData;
    MS_U8      ** workPtr;
    MS_U8       * blankBase;
    const MS_U8 *bitmapPtr;
    MS_U8       *logPointArrayPtr;
    MS_U16      *setIndexes, *baseIndexes, setZeroBases, baseCounter;

    if ((signalData->numSets > 0) && (signalData->numSets <= 0xFFFF))
    {
        setIndexes = malloc(sizeof(MS_U16) * signalData->numSets);
    }
    else
    {
        return;
    }

    if ((signalData->numBases > 0) && (signalData->numBases <= 0xFF))
    {
        baseIndexes = malloc(sizeof(MS_U16) * signalData->numBases);
    }
    else
    {
        free(setIndexes);
        return;
    }
    /* Get rid of old memory */
    KiTtiDestroyOldFilterMemory();

    /* Calculate how much memory is required for the matrix */
    /* This calculation is a bit pointless on a 32 bit target
    ** as the size of pointers should be 4. But it is needed
    ** for 64 bit targets */
    dataSize = (signalData->numSets  * sizeof(MS_U8 **)) +
               (signalData->numBases * sizeof(MS_U8 *)) +
               (signalData->matrixSize -
                ((signalData->numSets  * 4) +
                 (signalData->numBases * 4))) +
                KI_MAX_ONE_BASE_SIG_BITMAP; /* For the blank base */

    /* Allocate the required memory */
    KiAllocMemory (dataSize, (void **) &kiTtiSetFilter);

    /* Using a ** rather than *** makes the following a bit tidier */
    workPtr = (MS_U8 **)kiTtiSetFilter;
    /* Set the blankBase address */
    blankBase = &(((MS_U8 *)kiTtiSetFilter)[dataSize - KI_MAX_ONE_BASE_SIG_BITMAP]);
    /* Make sure it is empty */
    memset(blankBase, 0, KI_MAX_ONE_BASE_SIG_BITMAP);

    /* Fill in matrix, starting with set pointers */
    /* Point to the start of the base pointers */
    basePointer   = &(workPtr[signalData->numSets]);
    /* Set indexes are at the start of the matrix */
    /*  Coverity will produce error here.
       Even though, member matrix is defined as a static char array (of size 10), its actual size varies depending on  how
       Genie builds the KiSetsFilterMatrix to be send to the target.
       Genie treats matrix as a pointer and sets the proper size of the matrix as it needs (can be 120 bytes for example).
       Genie sets the actual size in the "matrixSize" member (see typedef KiSetsFilterMatrixTag).
       When Genie sends the matrix to target using the KiSetsFilterMatrix struct, the matrix member has a dynamic size and it
       ("matrixSize" member) is carefully considered in the apiHSL.c implementation.
       All signals sent from the PC to the target can have a max signal size of 20000 Bytes. */

    sourcePointer = signalData->matrix;
    for (counter = 0; (MS_U16)counter < signalData->numSets; counter++)
    {
        /* Reconstruct the base index from the input signal */
        tempData = (MS_U32)((sourcePointer[0] << 24) |
                           (sourcePointer[1] << 16) |
                           (sourcePointer[2] <<  8) |
                            sourcePointer[3]);

        setIndexes[counter] = tempData;

        /* Save the pointer to the first base in this set */
        workPtr[counter] = (MS_U8 *)&basePointer[tempData];
        /* Increment the sourcePointer */
        sourcePointer += 4;
    }

    /* Now fill in base pointers */
    /* Point to the start of the signal bitmaps */
    sigPointer   = (MS_U8 *)&(workPtr[signalData->numSets + signalData->numBases]);
    /* Set oldData to an invalid index */
    oldData = 0xFFFFFFFF;

    for (/* deliberate */;
         counter < (signalData->numSets + signalData->numBases);
         counter++)
    {
        /* Reconstruct the signal bitmap index from the input signal */
        tempData = (MS_U32)((sourcePointer[0] << 24) |
                           (sourcePointer[1] << 16) |
                           (sourcePointer[2] <<  8) |
                            sourcePointer[3]);

        baseIndexes[counter - signalData->numSets] = tempData;

        /* Check for previous base being empty */
        if (oldData == tempData)
        {   /* Previous base is empty so set to blank base to prevent
            ** invalid logging */
            workPtr[counter - 1] = blankBase;
        }
        /* Save the pointer to the first base in this set */
        workPtr[counter] = &(sigPointer[tempData]);
        /* Increment the sourcePointer */
        sourcePointer += 4;
        /* Remember this index */
        oldData = tempData;
    }

    /* Now copy over the signal bit maps */
    /* sourcePointer now points to the place where we want to copy the
    ** signal bitmap from */

    memcpy(sigPointer,
           sourcePointer,
           (size_t)(signalData->matrixSize - ((signalData->numSets  * 4) + (signalData->numBases * 4))));

    kiTtiFilterMemory = (void *)kiTtiSetFilter;
    M_KiTtiSetFilterType(KI_FILTER_SET);

    //we copy the data from the bitmap to our flat array
    if (signalData->numSets == 1)
        //if we have only one set, signalData->numBases gives us total number of bases in set 0
        setZeroBases = signalData->numBases;
    else
        //setZeroBases = (index of set 1's bases) - (index of set 0's bases)
        setZeroBases = setIndexes[1] - setIndexes[0];       //typically, setIndexes[0] = 0

    bitmapPtr = sourcePointer;
    counter = 0;

    //iterate through the bases of set 0
    for (baseCounter = 0; baseCounter < setZeroBases; baseCounter++)
    {
        logPointArrayPtr = &FrHslDynamicLogPointArray[(baseCounter << 5)];

        //itegrate through the signals of current base
        for (/* intentional */; (MS_U16)counter < baseIndexes[baseCounter]; counter++)
        {
            //counter is the index of signal bitmap byte
            *logPointArrayPtr = *bitmapPtr;
            ++logPointArrayPtr;
            ++bitmapPtr;
        }
    }//for

    free(setIndexes);
    free(baseIndexes);

}

/*******************************************************************************
* Function    : KiFreeMemory
* Group       : GKI, Test Interface, Internals
* Parameter   : ptr
* Returns     : Nothing
* Description : Free any memory
******************************************************************************/
#define KiFreeMemory(ptr)   \
{                           \
    free(*ptr);             \
    *ptr = NULL;            \
}


/*******************************************************************************
* Function    : KiTtiDestroyOldFilterMemory
* Group       : GKI, Test Interface, Internals
* Parameter   : None
* Returns     : Nothing
* Description : Free any memory allocated for the signal filter data.
******************************************************************************/
static void KiTtiDestroyOldFilterMemory (void)
{
    if (kiTtiFilterMemory != NULL)
    {
        /* Save stored filter type before setting it to NONE */
        KiFilterType destroyFilterType = kiTtiStoredFilterType;

        /* Set the filter to none before destroying the memory to prevent
        ** logging while the filter is being changed. */
        M_KiTtiSetFilterType(KI_FILTER_NONE);

        if (destroyFilterType == KI_FILTER_SET)
        {
            KiFreeMemory (&kiTtiFilterMemory);
            kiTtiSetFilter = NULL;
        }
    }
}

#endif /*  MSOS_TYPE_LINUX */
/* END OF FILE */
