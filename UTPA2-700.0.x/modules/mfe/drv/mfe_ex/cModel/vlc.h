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

/*!
 *************************************************************************************
 * \file vlc.h
 *
 * \brief
 *    Prototypes for VLC coding funtions
 * \author
 *     Karsten Suehring
 *************************************************************************************
 */

#ifndef _VLC_H_
#define _VLC_H_

	#define TRACE 0UL
	
//! Bitstream
typedef struct
{
    MS_S32             byte_pos;           //!< current position in bitstream;
    MS_S32             bits_to_go;         //!< current bitcounter
    MS_U8            byte_buf;           //!< current buffer for last written byte
    MS_U8            *streamBuffer;      //!< actual buffer for written bytes
    MS_S32             zerocount;
#if TRACE
    MS_BOOL             trace_enabled;
#endif
} Bitstream;

//! Syntax Element
typedef struct syntaxelement
{
    MS_S32                 type;           //!< type of syntax element for data part.
    MS_S32                 value1;         //!< numerical value of syntax element
    MS_S32                 value2;         //!< for blocked symbols, e.g. run/level
    MS_S32                 len;            //!< length of code
    MS_S32                 inf;            //!< info part of UVLC code
    MS_U32        bitpattern;     //!< UVLC bitpattern
    MS_S32                 context;        //!< CABAC context
    
#if TRACE
#define             TRACESTRING_SIZE 100UL            //!< size of trace string
    MS_S8                tracestring[TRACESTRING_SIZE];  //!< trace string
#endif
    
    //!< for mapping of syntaxElement to UVLC
    void    (*mapping)(MS_S32 value1, MS_S32 value2, MS_S32* len_ptr, MS_S32* info_ptr);
    
} SyntaxElement;

void SODBtoRBSP(Bitstream *currStream);
MS_S32 RBSPtoEBSP(MS_U8 *streamBuffer, MS_S32 begin_bytepos, MS_S32 end_bytepos, MS_S32 min_num_bytes);

MS_S32 u_1  (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 se_v (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 ue_v (MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);
MS_S32 u_v  (MS_S32 n, MS_S8 *tracestring, MS_S32 value, Bitstream *bitstream);

MS_S32 symbol2uvlc(SyntaxElement *se);
void ue_linfo(MS_S32 n, MS_S32 dummy, MS_S32 *len, MS_S32 *info);
void se_linfo(MS_S32 mvd, MS_S32 dummy, MS_S32 *len, MS_S32 *info);
void writeVlcByteAlign(Bitstream* currStream);
#endif

