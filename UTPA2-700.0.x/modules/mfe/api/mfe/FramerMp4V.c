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
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "madp_ms_dprintf.h"

#include "Framer.h"

#define DBG_FRAMER_MSG(format, ...)
//#define DBG_FRAMER_MSG printf//(x)

#define VIDEO_STARTCODE(x) ((x)==0x1000000)
#define MP2_SYNCWORD(x) (((x)&0xFCFF)==0xFCFF)
#define SAMPLES_IN_AUDIO_FRAME 1152

#ifdef _WIN32
typedef struct Mp2aFrameHeader {
	unsigned int layer;
	unsigned int bitrate_index;
	unsigned int sampling_frequency;
} Mp2aFrameHeader;

//////////////////////////////////////////////////////////////////////////
// Internal functions
//////////////////////////////////////////////////////////////////////////

static void DoFileMapping(LPCTSTR lpFileName, BitstreamInfo* pBsInfo)
{
  HANDLE hMapFile, hFile;

  pBsInfo->nFileSize = 0;
  pBsInfo->pBits = NULL;
  hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

  if(hFile && hFile != INVALID_HANDLE_VALUE)
  {
    pBsInfo->nFileSize = GetFileSize(hFile, NULL);
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

    if(hMapFile && hMapFile != INVALID_HANDLE_VALUE)
    {
      pBsInfo->pBits =  (U8*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

      CloseHandle(hMapFile);
    }

    CloseHandle(hFile);
  }
}

static MFE_BOOL UndoFileMapping(MFE_U8* pVideoMemory)
{
	MFE_BOOL bRtn = 0;
	if (UnmapViewOfFile(pVideoMemory))
		bRtn = 1;
	return bRtn;
}

//////////////////////////////////////////////////////////////////////////
//   APIs
//////////////////////////////////////////////////////////////////////////

MFE_BOOL OpenMappedFile(const char* strFileName, BitstreamInfo* pBsInfo)
{
	// For audio
	Mp2aFrameHeader mp2aHeader;
	unsigned int nAudBitrate, nSampleFreq;

	pBsInfo->pBits = NULL;
	DoFileMapping(/*LPCTSTR*/(strFileName), pBsInfo);
	if (pBsInfo->pBits == NULL)
		return 0;

	pBsInfo->pCur = pBsInfo->pLast = pBsInfo->pBits;
	pBsInfo->pEnd = pBsInfo->pBits + pBsInfo->nFileSize;
	pBsInfo->nFrameIndex = 0;
	pBsInfo->bPrevVOP = 0;
	if (pBsInfo->bIsAudio) {
		mp2aHeader.layer = (pBsInfo->pBits[1] & 0x6) >> 1;
		mp2aHeader.bitrate_index = (pBsInfo->pBits[2] & 0xF0) >> 4;
		mp2aHeader.sampling_frequency = (pBsInfo->pBits[2] & 0xC) >> 2;
		switch (mp2aHeader.bitrate_index)
		{
		case 1:
			nAudBitrate = 32000;
			break;
		case 2:
			nAudBitrate = (mp2aHeader.layer==2 ? 48000 : 40000);
			break;
		case 3:
			nAudBitrate = (mp2aHeader.layer==2 ? 56000 : 48000);
			break;
		case 4:
			nAudBitrate = (mp2aHeader.layer==2 ? 64000 : 56000);
			break;
		case 5:
			nAudBitrate = (mp2aHeader.layer==2 ? 80000 : 64000);
			break;
		case 6:
			nAudBitrate = (mp2aHeader.layer==2 ? 96000 : 80000);
			break;
		case 7:
			nAudBitrate = (mp2aHeader.layer==2 ? 112000 : 96000);
			break;
		case 8:
			nAudBitrate = (mp2aHeader.layer==2 ? 128000 : 112000);
			break;
		case 9:
			nAudBitrate = (mp2aHeader.layer==2 ? 160000 : 128000);
			break;
		case 10:
			nAudBitrate = (mp2aHeader.layer==2 ? 192000 : 160000);
			break;
		case 11:
			nAudBitrate = (mp2aHeader.layer==2 ? 224000 : 192000);
			break;
		case 12:
			nAudBitrate = (mp2aHeader.layer==2 ? 256000 : 224000);
			break;
		case 13:
			nAudBitrate = (mp2aHeader.layer==2 ? 320000 : 256000);
			break;
		case 14:
			nAudBitrate = (mp2aHeader.layer==2 ? 384000 : 320000);
			break;
		default:
			nAudBitrate = 0;	// free format
			break;
		}
		switch (mp2aHeader.sampling_frequency) {
		case 0:
			nSampleFreq = 44100; break;
		case 1:
			nSampleFreq = 48000; break;
		case 2:
			nSampleFreq = 32000; break;
		}
		pBsInfo->nAudFrameSize = (nAudBitrate>>3) * SAMPLES_IN_AUDIO_FRAME / nSampleFreq;
	}

	return 1;
}

MFE_BOOL CloseMappedFile(BitstreamInfo* pBsInfo)
{
	MFE_BOOL rtn;
	if (pBsInfo->pBits==NULL)
		return 1;
	rtn = UndoFileMapping(pBsInfo->pBits);
	return rtn;
}

enum {
	MPEG2_SC_SEQSTART,
	MPEG2_SC_SEQEND,
	MPEG2_SC_EXT,
	MPEG2_SC_GOP,
	MPEG2_SC_PIC,
	MPEG2_SC_SLICE,
	MPEG4_SC_VO_OR_VOL,
	MPEG4_SC_VOS_START,
	MPEG4_SC_VOS_END,
	MPEG4_SC_GOVOP,
	MPEG4_SC_VOP,
	SC_UNKNOWN,
} SC_TYPE;

// MPEG4 video
MFE_BOOL GetOneFrameBitsMP4V(BitstreamInfo* pBsInfo, unsigned int nFrameCount/*DontCare*/)
{
	unsigned int n3Byte;
	MFE_BOOL bFound = 0;
	MFE_U8 StartCode, SCType;

	//assert(nFrameCount==1);
	pBsInfo->nFrameCount = 1;
	while (pBsInfo->pCur<pBsInfo->pEnd) {
		n3Byte = (*(unsigned int*)pBsInfo->pCur)<<8;
		if (VIDEO_STARTCODE(n3Byte)) {
			StartCode = *(pBsInfo->pCur+3);
			if (StartCode>=0x0 && StartCode<=0x2F)
				SCType = MPEG4_SC_VO_OR_VOL;	// Video Object start code, or VOL start code
			else if (StartCode==0xB0)
				SCType = MPEG4_SC_VOS_START;	// Visual Object Sequence
			else if (StartCode==0xB1)
				SCType = MPEG4_SC_VOS_END;		// Visual Object Sequence End
			else if (StartCode==0xB3)
				SCType = MPEG4_SC_GOVOP;		// Group Of Video Object Plane
			else if (StartCode==0xB6)
				SCType = MPEG4_SC_VOP;			// Video Object Plane
			else
				SCType = SC_UNKNOWN;

			if (pBsInfo->bPrevVOP==0) {
				if (SCType==MPEG4_SC_VOP)
					pBsInfo->bPrevVOP = 1;
				pBsInfo->pCur+=4;
			}
			else {
				if (SCType==MPEG4_SC_VOP||SCType==MPEG4_SC_GOVOP||SCType==MPEG4_SC_VOS_END) {
					bFound = 1;
					pBsInfo->pFrameBits = pBsInfo->pLast;
					if (SCType==MPEG4_SC_VOS_END)
						pBsInfo->nFrameSize = (int)(pBsInfo->pCur+4-pBsInfo->pLast);
					else
						pBsInfo->nFrameSize = (int)(pBsInfo->pCur-pBsInfo->pLast);
					DBG_FRAMER_MSG("Found frame #%d, offset %d, size %d\n",
						pBsInfo->nFrameIndex, pBsInfo->pFrameBits-pBsInfo->pBits, pBsInfo->nFrameSize);
					// Prepare for next round
					pBsInfo->pLast = pBsInfo->pCur;
					pBsInfo->pCur+=4;
					pBsInfo->nFrameIndex++;
					if (SCType==MPEG4_SC_GOVOP||SCType==MPEG4_SC_VOS_END)
						pBsInfo->bPrevVOP = 0;
				}
				else
					pBsInfo->pCur+=4;
				if (bFound)
					break;
			}
		}
		else
			pBsInfo->pCur++;
	}

	if (!bFound && pBsInfo->pCur>=pBsInfo->pEnd) {
		pBsInfo->nFrameCount = 0;
		return 0;
	}
	return 1;
}

// MPEG-2 video
MFE_BOOL GetOneFrameBitsMP2V(BitstreamInfo* pBsInfo, unsigned int nFrameCount/*DontCare*/)
{
	unsigned int n3Byte;
	MFE_BOOL bFound = 0;
	MFE_U8 StartCode, SCType;

	//assert(nFrameCount==1);
	pBsInfo->nFrameCount = 1;
	while (pBsInfo->pCur<pBsInfo->pEnd) {
		n3Byte = (*(unsigned int*)pBsInfo->pCur)<<8;
		if (VIDEO_STARTCODE(n3Byte)) {
			StartCode = *(pBsInfo->pCur+3);
			if (StartCode==0x0)
				SCType = MPEG2_SC_PIC;
			else if (StartCode>=0x1 && StartCode<=0xAF)
				SCType = MPEG2_SC_SLICE;
			else if (StartCode==0xB3)
				SCType = MPEG2_SC_SEQSTART;
			else if (StartCode==0xB5)
				SCType = MPEG2_SC_EXT;
			else if (StartCode==0xB7)
				SCType = MPEG2_SC_SEQEND;
			else if (StartCode==0xB8)
				SCType = MPEG2_SC_GOP;
			else
				SCType = SC_UNKNOWN;

			if (pBsInfo->bPrevVOP==0) {
				if (SCType==MPEG2_SC_PIC)
					pBsInfo->bPrevVOP = 1;
				pBsInfo->pCur+=4;
			}
			else {
				if (SCType==MPEG2_SC_SEQSTART||SCType==MPEG2_SC_PIC||SCType==MPEG2_SC_GOP||SCType==MPEG2_SC_SEQEND) {
					bFound = 1;
					pBsInfo->pFrameBits = pBsInfo->pLast;
					if (SCType==MPEG2_SC_SEQEND)
						pBsInfo->nFrameSize = (int)(pBsInfo->pCur+4-pBsInfo->pLast);
					else
						pBsInfo->nFrameSize = (int)(pBsInfo->pCur-pBsInfo->pLast);
					DBG_FRAMER_MSG("Found frame #%d, offset %d, size %d\n",
						pBsInfo->nFrameIndex, pBsInfo->pFrameBits-pBsInfo->pBits, pBsInfo->nFrameSize);
					// Prepare for next round
					pBsInfo->pLast = pBsInfo->pCur;
					pBsInfo->pCur+=4;
					pBsInfo->nFrameIndex++;
					if (SCType==MPEG2_SC_SEQSTART||SCType==MPEG2_SC_GOP||SCType==MPEG2_SC_SEQEND)
						pBsInfo->bPrevVOP = 0;
				}
				else
					pBsInfo->pCur+=4;
				if (bFound)
					break;
			}
		}
		else
			pBsInfo->pCur++;
	}

	if (pBsInfo->pCur==pBsInfo->pEnd) {
		pBsInfo->nFrameCount = 0;
		return 0;
	}
	return 1;
}
#endif //_WIN32
// MPEG-1 audio
MFE_BOOL GetOneFrameBitsMP1A(BitstreamInfo_ptr pBsInfo, unsigned int nFrameCount)
{
	unsigned short n2Byte;
	MFE_BOOL bSync = 0;
//	MFE_BOOL bFound = 0;
//    DBG_FRAMER_MSG("in GetOneFrameBitsMP1A 0x%x 0x%x 0x%x 0x%x\n", (U32)pBsInfo, nFrameCount, (U32)pBsInfo->pCur, (U32)pBsInfo->pEnd);
	pBsInfo->nFrameCount = 1;
	while (pBsInfo->pCur<pBsInfo->pEnd) {
		n2Byte = (*(unsigned short*)pBsInfo->pCur);//(((U16)pBsInfo->pCur[1])<<8)|(pBsInfo->pCur[0]);
		bSync = MP2_SYNCWORD(n2Byte);
//        DBG_FRAMER_MSG("nFrameIndex: %d %d 0x%x %d %d\n", pBsInfo->nFrameIndex
//            , bSync, n2Byte, pBsInfo->pCur[0], pBsInfo->pCur[1]);
		if (bSync) {
			if (pBsInfo->pEnd - pBsInfo->pCur >= pBsInfo->nAudFrameSize) {
				// NOTE: This mechanism does not handle un-fixed frame size! say, 44.1Khz.
                          pBsInfo->pFrameBits = pBsInfo->pLast = pBsInfo->pCur;
                          pBsInfo->nFrameSize = pBsInfo->nAudFrameSize;
                          pBsInfo->pCur += pBsInfo->nAudFrameSize;
				pBsInfo->nFrameIndex++;
				break;
			}
/*
			if (pBsInfo->bPrevVOP==0) {
				pBsInfo->bPrevVOP = 1;
				//pBsInfo->pCur+=2;
                pBsInfo->pFrameBits = pBsInfo->pLast = pBsInfo->pCur;
                pBsInfo->pCur += pBsInfo->nAudFrameSize;    //consider this input only contain one audio frame
                pBsInfo->nFrameSize = pBsInfo->nAudFrameSize; //consider this input only contain one audio frame
		     } else {
				pBsInfo->pFrameBits = pBsInfo->pLast;
				pBsInfo->nFrameSize = (int)(pBsInfo->pCur-pBsInfo->pLast);
				if (pBsInfo->nFrameSize==pBsInfo->nAudFrameSize || pBsInfo->nFrameSize==pBsInfo->nAudFrameSize+1) {
					bFound = 1;
//			     DBG_FRAMER_MSG("Found frame #%d, offset %d, size %d\n",
//    				 pBsInfo->nFrameIndex, pBsInfo->pFrameBits-pBsInfo->pBits, pBsInfo->nFrameSize);
					// Prepare for next round
					pBsInfo->pLast = pBsInfo->pCur;
					pBsInfo->pCur+=pBsInfo->nAudFrameSize;
					pBsInfo->nFrameIndex++;
					if (bFound)
						break;
				}
				else
					pBsInfo->pCur+=2;
			}
*/
		}
		else
			pBsInfo->pCur++;
	}
//    DBG_FRAMER_MSG("leave GetOneFrameBitsMP1A pCur 0x%x 0x%x\n"
//        , (U32)pBsInfo->pCur, (U32)pBsInfo->pEnd);
    if (pBsInfo->pCur > pBsInfo->pEnd) { //consider this input only contain one audio frame
        DBG_FRAMER_MSG("!!!Audio read the end of buffer!!!\n");
		pBsInfo->nFrameCount = 0;
		return 0;
    } 
    else if (bSync==0) {
	    DBG_FRAMER_MSG("Audio data not correct!!!\n");
    }
	return 1;
}
