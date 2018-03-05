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



#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "OutStrm.h"

/********************************************************/
/* Implementation of class OutStream					*/
/********************************************************/

void osCreate(OutStream* pStream)
{
	// Check endian
	MFE_U32 tmp = 0x04030201;
	if (*(MFE_U8*)&tmp == 0x1)
		pStream->m_bUseLittleEndian = 1;
	else if (*(MFE_U8*)&tmp == 0x4)
		pStream->m_bUseLittleEndian = 0;
	else {
		MFE_ASSERT(0);
		pStream->m_bUseLittleEndian = 0;
	}
	ms_dprintk(DRV_L4,"OutStream: LittleEndian=%d\n", pStream->m_bUseLittleEndian);

	pStream->m_pbFrameBuffer = 0;
	pStream->m_nByteCount = 0;
	pStream->m_nBufferEmptyBits = 32;
	pStream->m_SmallBuffer = 0;
	pStream->BC_nCumulativeBits = 0;
}

void osDestroy(OutStream* pStream)
{
}

void osSetWriteBuffer(OutStream* pStream, MFE_BYTE *pWBuf)
{
	pStream->m_SmallBuffer = 0;
    pStream->m_nByteCount = 0;
    pStream->m_nBufferEmptyBits = 32;
	pStream->m_pbFrameBuffer = pWBuf;
}

void osReset(OutStream* pStream)
{
	pStream->m_SmallBuffer = 0;
    pStream->m_nByteCount = 0;
    pStream->m_nBufferEmptyBits = 32;
	pStream->BC_nCumulativeBits = 0;
}

void osFlushAll(OutStream* pStream)
{
    int nUnusedByteCount = 0;
	int nUnusedBits = pStream->m_nBufferEmptyBits;
    if (pStream->m_nBufferEmptyBits != 32)
    {
        if (pStream->m_nBufferEmptyBits>=24)
            nUnusedByteCount = 3;
        else if (pStream->m_nBufferEmptyBits>=16)
            nUnusedByteCount = 2;
        else if (pStream->m_nBufferEmptyBits>=8)
            nUnusedByteCount = 1;
        osPutBits(pStream, 0, pStream->m_nBufferEmptyBits, "");
    }
    pStream->BC_nCumulativeBits -= nUnusedBits;
    pStream->m_nByteCount -= nUnusedByteCount;
}

void osPutBits(OutStream* pStream, MFE_U32 value, int bits, char* name)
{ 
//    MFE_U32 *ptwrite;
    MFE_U32 tlongbuf;

    // BitRate Control
    pStream->BC_nCumulativeBits += bits;
	//printf("PutBits() %s Val=%x Bits=%d\n", name, value, bits);

    MFE_ASSERT(bits!=0);
//	if (bits) {
        //
        // Less than empty bits
        //
        if (bits < pStream->m_nBufferEmptyBits) {
            pStream->m_nBufferEmptyBits -= bits;
            pStream->m_SmallBuffer += value<<pStream->m_nBufferEmptyBits;
        }
		//
		// overflow of 32 bits. so value should be segement.
		//
		else if (bits>=pStream->m_nBufferEmptyBits)
	    {
            // to reduce code size, no shortcut if bits == m_nBufferEmptyBits
            // since the probability is low.
			pStream->m_SmallBuffer+=value>>(bits-pStream->m_nBufferEmptyBits); //shift right

//			ptwrite = (MFE_U32 *)(pStream->m_pbFrameBuffer+pStream->m_nByteCount);
            tlongbuf = pStream->m_SmallBuffer;

			// ------ Little Endian in PC --------
			if (pStream->m_bUseLittleEndian) {
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 0] = (MFE_U8)(tlongbuf >> 24);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 1] = (MFE_U8)(tlongbuf >> 16);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 2] = (MFE_U8)(tlongbuf >>  8);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 3] = (MFE_U8)(tlongbuf);
			}
			else {
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 3] = (MFE_U8)(tlongbuf >> 24);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 2] = (MFE_U8)(tlongbuf >> 16);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 1] = (MFE_U8)(tlongbuf >>  8);
				pStream->m_pbFrameBuffer[pStream->m_nByteCount + 0] = (MFE_U8)(tlongbuf);
			}

			// -----------------------------------
			pStream->m_nByteCount+=4;
            if (bits==pStream->m_nBufferEmptyBits) {
                pStream->m_nBufferEmptyBits = 32;
                pStream->m_SmallBuffer = 0;
            } else {
			    pStream->m_nBufferEmptyBits = 32 - bits + pStream->m_nBufferEmptyBits;
			    pStream->m_SmallBuffer = value << pStream->m_nBufferEmptyBits;
            }
		}
//	}
}

// 14496-2
void osNextStartCode(OutStream* pStream)
{
    osPutBits(pStream, 0, 1, "NextStartCode");
    osStuffing(pStream);
}

MFE_U32 GetBitsCount(OutStream* pStream)
{
    return pStream->BC_nCumulativeBits;
}

void osStuffing(OutStream* pStream)
{
	int nStuffingBits = pStream->m_nBufferEmptyBits&7;
    if (pStream->m_nBufferEmptyBits & 7)
        osPutBits(pStream, 0xFF>>(8-nStuffingBits), nStuffingBits,"stuffing");
}
