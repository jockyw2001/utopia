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
#include "mfe_type.h"
#include "mfe_reg.h"
#include "mfe_common.h"
#include "ms_dprintf.h"


extern MFE_BOOL g_bForceIframe;

//static int mfe_alloc_addr(int Buf[2], int mem_start, int sizeY, int sizeC)
static void mfe_alloc_addr(MFE_U32* ppBufStart, MFE_U32 pBufEnd, MEMMAP_t Buf[2], int sizeY, int sizeC)
{
//     mem_start = (mem_start+255)&~255;
//     Buf[0] = mem_start;
//     mem_start += sizeY;
//     mem_start = (mem_start+255)&~255;
//     Buf[1] = mem_start;
//     mem_start += sizeC;
//     return mem_start;

	MMAPMalloc(ppBufStart, pBufEnd, sizeY, &Buf[0], 256,"Y");
	MMAPMalloc(ppBufStart, pBufEnd, sizeC, &Buf[1], 256,"C");
}

//////////////////////////////////////////////////////////////////////////
// MPEG-4
//////////////////////////////////////////////////////////////////////////

// Total memory usage:
//   (Width*Height/256)*16 + (Width/16)*64 + (Width*Height*3/2)*3 + OBUF_SIZE*OBUF_NUM
MFE_U32 m4veAllocDRAM(MFE_CONFIG* pConfig)
{
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	int nMbCount;
	int sizeY, sizeC, i;
	MFE_U32 pBufEnd = (MFE_U32)(pConfig->dram_base + pConfig->dram_size);

	nMbCount = (pConfig->nBufWidth * pConfig->nBufHeight)>>8;

// 	pBufInfo->m_nMvStoreAddr = dram_base;
// 	MFE_ASSERT((pBufInfo->m_nMvStoreAddr&0x7)==0);
// 	dram_base += nMbCount*16;
// 	dram_base = (dram_base+7) & ~7;
	MMAPMalloc((MFE_U32*)&pConfig->dram_ptr, pBufEnd, nMbCount*16, &pBufInfo->m_nMvStoreAddr, 8,"Mbcount*16");
	MFE_ASSERT((pBufInfo->m_nMvStoreAddr.miuAddress&0x7)==0);

	// For simplicity, fix GN address
// 	pBufInfo->m_nGNAddr = dram_base;
// 	assert((pBufInfo->m_nGNAddr&0x7)==0);
// 	dram_base += (pConfig->nBufWidth>>4)*64;
// 	dram_base = (dram_base+7) & ~7;
	MMAPMalloc((MFE_U32*)&pConfig->dram_ptr, pBufEnd, (pConfig->nBufWidth>>4)*64, &pBufInfo->m_nGNAddr, 8,"(nBufWidth>>4)*6");
	MFE_ASSERT((pBufInfo->m_nGNAddr.miuAddress&0x7)==0);

	sizeY = pConfig->nBufWidth * pConfig->nBufHeight;
	sizeC = sizeY >> 1;
// 	mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_CUR_ORI], sizeY, sizeC);	// m_pvopcOrig
// 	mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_REF0_ORI], sizeY, sizeC);	// m_pvopcRefOrig0
// 	mfe_alloc_addr((U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_REF1_ORI], sizeY, sizeC);	// m_pvopcRefOrig1
	mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_CUR_REC], sizeY, sizeC);	// m_pvopcCurrQ
	mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_REF0_REC], sizeY, sizeC);	// m_pvopcRefQ0
	mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->m_FrameBuf[M4VE_FRM_REF1_REC], sizeY, sizeC);	// m_pvopcRefQ1

// 	dram_base = (dram_base+255)&~255;
// 	pBufInfo->m_nOutBufAddr = dram_base;
// 	dram_base += OBUF_SIZE*OBUF_NUM;
// 	MFE_ASSERT(dram_base<MAX_TOPSIM_ADDR);
	for (i=0; i<pBufInfo->m_nOutBuffer; i++)
		MMAPMalloc((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pBufInfo->m_OutBufferSize, &pBufInfo->m_nOutBufAddr[i], 256,"out buf");

	ms_dprintk(DRV_L4, "[m4veAllocDRAM] Start=%08x, End=%08x\n", (unsigned int) pConfig->dram_base, (unsigned int)pConfig->dram_ptr);
	return (MFE_U32)pConfig->dram_ptr;
}

static void m4veUpdateAllRefVOPs(MFE_CONFIG* pConfig)
// perform this after VOP prediction type decided and before encoding
{
	M4VEINFO* pInfo = &pConfig->ctxM4veInfo;
	MEMMAP_t tmp;
	int i;

	if (pConfig->vopPredType != B_VOP) {
		//swapVOPU8Pointers (m_pvopcRefQ0, m_pvopcRefQ1);
		for (i=0; i<2; i++) {
			tmp = pInfo->m_FrameBuf[M4VE_FRM_REF0_REC][i];
			pInfo->m_FrameBuf[M4VE_FRM_REF0_REC][i] = pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][i];
			pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][i] = tmp;
		}
	}
}

// Input: pConfig->nCodingOrder
// Output: pGOP->nDispOrder
// NOTE: Every nCodingOrder can only call this once!
// NOTE: nCodingOrder is determined outside this function.

#define bIsOpenGOP 1

void mfeSetVopType(MFE_CONFIG* pConfig, GOPINFO* pGOP)
{
//	M4VEINFO* pInfo = &pConfig->ctxM4veInfo;

        //remove for fix coverity deadcode.
//	const MFE_U8 bIsOpenGOP = 1;

	if (pGOP->nCodingOrder==0) {
		// Initial
		pConfig->vopPredType = I_VOP;
		pGOP->nDispOrder = 0;
#if 0	// The very leading B-frames when open-GOP
		if (bIsOpenGOP && pConfig->nBbetweenP>0)
			pGOP->nDispOrder = pConfig->nBbetweenP;
#endif
		pGOP->nPCount = 0;
		pGOP->nBCount = 0;
	}
	else {
		if (pConfig->nPbetweenI==0&&pConfig->nBbetweenP==0) {
			// I-only
			pConfig->vopPredType = I_VOP;
			pGOP->nDispOrder++;
		}
             else if(g_bForceIframe) {
                g_bForceIframe = FALSE;
                pConfig->vopPredType = I_VOP;
                pGOP->nDispOrder++;
                pGOP->nPCount = 0;
                pGOP->nBCount = 0;
             }
		else {
			switch (pConfig->vopPredType) {
			case I_VOP:
				if (bIsOpenGOP && pConfig->nBbetweenP>0 && pGOP->nDispOrder>0) {
					pConfig->vopPredType = B_VOP;
					pGOP->nDispOrder = pGOP->nDispOrder - pConfig->nBbetweenP;
					pGOP->nPCount = 0;
					pGOP->nBCount = 1;
				}
				else {
					pConfig->vopPredType = P_VOP;
					pGOP->nDispOrder += pConfig->nBbetweenP+1;
					pGOP->nPCount = 1;
					pGOP->nBCount = 0;
				}
				break;
			case P_VOP:
				if (pGOP->nBCount<pConfig->nBbetweenP) {
					pConfig->vopPredType = B_VOP;
					pGOP->nDispOrder = pGOP->nDispOrder - pConfig->nBbetweenP;
					pGOP->nBCount = 1;
				}                                                                   //nPbetweenI<0 means infinite. Use For Skype project.
				else if (pGOP->nPCount<pConfig->nPbetweenI || pConfig->nP_is_infinite) {
					MFE_ASSERT(pConfig->nBbetweenP==0);
					pConfig->vopPredType = P_VOP;
					pGOP->nDispOrder++;
					pGOP->nPCount++;
					pGOP->nBCount = 0;
				}
				else {
					MFE_ASSERT(pConfig->nBbetweenP==0);
					pConfig->vopPredType = I_VOP;
					pGOP->nDispOrder++;
					pGOP->nPCount = 0;
					pGOP->nBCount = 0;
				}
				break;
			case B_VOP:
				if (pGOP->nBCount<pConfig->nBbetweenP) {
					pConfig->vopPredType = B_VOP;
					pGOP->nDispOrder++;
					pGOP->nBCount++;
				}
				else if (pGOP->nPCount<pConfig->nPbetweenI) {
					pConfig->vopPredType = P_VOP;
					pGOP->nDispOrder += 2+pConfig->nBbetweenP;
					pGOP->nPCount++;
					pGOP->nBCount = 0;
				}
				else {
					pConfig->vopPredType = I_VOP;
					if (bIsOpenGOP)
						pGOP->nDispOrder += 2+pConfig->nBbetweenP;
					else
						pGOP->nDispOrder += 2;
					pGOP->nPCount = 0;
					pGOP->nBCount = 0;
				}
				break;
			}
		}
	}

	pConfig->nCodingOrder = pGOP->nCodingOrder;
}

void m4veGetBufferAddr(MFE_CONFIG* pConfig)
{
	M4VEINFO* pInfo = &pConfig->ctxM4veInfo;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;

	m4veUpdateAllRefVOPs(pConfig);

#ifdef USE_CUR_AS_REC
	pBufInfo->m_nRecYAddr = pBufInfo->m_nCurYAddr;
	pBufInfo->m_nRecCAddr = pBufInfo->m_nCurCAddr;
#else
	if (pConfig->vopPredType==B_VOP) {
		pBufInfo->m_nRecYAddr = pInfo->m_FrameBuf[M4VE_FRM_CUR_REC][0];
		pBufInfo->m_nRecCAddr = pInfo->m_FrameBuf[M4VE_FRM_CUR_REC][1];
	}
	else {
		pBufInfo->m_nRecYAddr = pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][0];
		pBufInfo->m_nRecCAddr = pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][1];
	}
#endif

	pBufInfo->m_nRefYAddr[0] = pInfo->m_FrameBuf[M4VE_FRM_REF0_REC][0];
	pBufInfo->m_nRefCAddr[0] = pInfo->m_FrameBuf[M4VE_FRM_REF0_REC][1];

	pBufInfo->m_nRefYAddr[1] = pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][0];
	pBufInfo->m_nRefCAddr[1] = pInfo->m_FrameBuf[M4VE_FRM_REF1_REC][1];

	pBufInfo->m_bEnableMvStore = pInfo->bMvStore ? 1 : 0;
}


//////////////////////////////////////////////////////////////////////////
// H264
//////////////////////////////////////////////////////////////////////////

// Total memory usage:
//   (Width/16)*64 + (Width*Height*3/2)*(MAX_BUFPOOL_COUNT>>1) + OBUF_SIZE*OBUF_NUM
MFE_U32 h264AllocDRAM(MFE_CONFIG* pConfig)
{
	H264INFO *pInfo = &pConfig->ctxH264Info;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;
//	int nMbCount;
	int sizeY, sizeC, i, total;
	MFE_U32 pBufEnd = (MFE_U32)(pConfig->dram_base + pConfig->dram_size);

//	nMbCount = (pConfig->nBufWidth * pConfig->nBufHeight)>>8;

	// For simplicity, fix GN address
// 	g_gnstore_addr = g_dram_base;
// 	MFE_ASSERT((g_gnstore_addr&0x7)==0);
// 	g_dram_base += (img->width / 16)*64;
// 	g_dram_base = (g_dram_base+7) & ~7;
	MMAPMalloc((MFE_U32*)&pConfig->dram_ptr, pBufEnd, (pConfig->nBufWidth>>4)*64, &pBufInfo->m_nGNAddr, 8,"(pConfig->nBufWidth>>4)*64");
	MFE_ASSERT((pBufInfo->m_nGNAddr.miuAddress&0x7)==0);

	sizeY = pConfig->nBufWidth * pConfig->nBufHeight;
	sizeC = sizeY >> 1;
	if (pConfig->bInterlace) {
		sizeY >>= 1;
		sizeC >>= 1;
	}
	// Frame buffer
	total = pConfig->bInterlace ? MAX_BUFPOOL_COUNT : (MAX_BUFPOOL_COUNT>>1);
	for (i=0; i<total; i++) {
		mfe_alloc_addr((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pInfo->BufPool.addr[i], sizeY, sizeC);
		pInfo->BufPool.available[i] = 1;
	}
	for (; i<MAX_BUFPOOL_COUNT; i++)
		pInfo->BufPool.available[i] = 0;
	// Out buffer
// 	g_dram_base = (g_dram_base+255)&~255;
// 	g_outbuf_addr = g_dram_base;
// 	g_dram_base += OBUF_SIZE*OBUF_NUM;
// 	MFE_ASSERT(g_dram_base<MAX_TOPSIM_ADDR);
// 	g_dram_base = (g_dram_base+255)&~255;
	for (i=0; i<pBufInfo->m_nOutBuffer; i++)
		MMAPMalloc((MFE_U32*)&pConfig->dram_ptr, pBufEnd, pBufInfo->m_OutBufferSize, &pBufInfo->m_nOutBufAddr[i], 256,"OUT BUF");
	ms_dprintk(DRV_L4, "[h264AllocDRAM] Start=%08x, End=%08x\n",(unsigned int) pConfig->dram_base,(unsigned int) pConfig->dram_ptr);
	return (MFE_U32)pConfig->dram_ptr;
}

static void bufpool_get(FrameBufPool* pPool, MEMMAP_t Buf[2])
{
	int i;

	for (i=0; i<MAX_BUFPOOL_COUNT; i++) {
		if (pPool->available[i]) {
			Buf[0] = pPool->addr[i][0];
			Buf[1] = pPool->addr[i][1];
			pPool->available[i] = 0;
			break;
		}
	}
	if (i==MAX_BUFPOOL_COUNT)
	{
        ms_dprintk(DRV_L4, "[bufpool_get] Buffer pool empty!!");
        MFE_ASSERT(0);
    }
}

static void bufpool_return(FrameBufPool* pPool, MEMMAP_t Buf[2])
{
	int i;
	for (i=0; i<MAX_BUFPOOL_COUNT; i++) {
		if (pPool->addr[i][0].miuAddress==Buf[0].miuAddress && pPool->addr[i][1].miuAddress==Buf[1].miuAddress) {
			pPool->available[i] = 1;
			break;
		}
	}
    if (i==MAX_BUFPOOL_COUNT) {
        ms_dprintk(DRV_L4, "[bufpool_return] Specific buffer not found!!");
        MFE_ASSERT(0);
    }
}

void h264DpbHandling(MFE_CONFIG* pConfig)
{
	int i, empty_idx;
	H264INFO *pInfo = &pConfig->ctxH264Info;
	BufInfo *pBufInfo = &pConfig->ctxBufInfo;

	// When IDR, clear all ref-frames in dpb
	if (pInfo->idr_flag) {
		// DPB initialization
		for (i=0; i<DPB_SIZE; i++)
			pInfo->dpb[i].is_used = 0;
	}

	// Leave dpb[0] for current frame
	empty_idx = -1;
	for (i=DPB_SIZE-1; i>=0; i--) {
		if (pInfo->dpb[i].is_used==0)
			empty_idx = i;
	}
	if (empty_idx==-1) {
		bufpool_return(&pInfo->BufPool, pInfo->dpb[DPB_SIZE-1].addr);
		pInfo->dpb[DPB_SIZE-1].is_used = 0;
		empty_idx = DPB_SIZE-1;	// Kick-off the least recent frame
	}
	if (empty_idx!=0) {
		for (i=empty_idx; i>=1; i--) {
			pInfo->dpb[i].addr[0] = pInfo->dpb[i-1].addr[0];
			pInfo->dpb[i].addr[1] = pInfo->dpb[i-1].addr[1];
			pInfo->dpb[i].is_used = 1;
		}
		pInfo->dpb[0].is_used = 0;
	}

	// Insert current frame to dpb[0]
	pInfo->dpb[0].addr[0] = pBufInfo->m_nRecYAddr;
	pInfo->dpb[0].addr[1] = pBufInfo->m_nRecCAddr;
	pInfo->dpb[0].is_used = 1;
}

void h264GetBufferAddr(MFE_CONFIG* pConfig)
{
	H264INFO* pInfo = &pConfig->ctxH264Info;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;

	// When IDR, clean all buffer status
	if (pInfo->idr_flag) {
		int i;
		int total = pConfig->bInterlace ? MAX_BUFPOOL_COUNT : (MAX_BUFPOOL_COUNT>>1);
		for (i=0; i<total; i++)
			pInfo->BufPool.available[i] = 1;
	}

#ifdef USE_CUR_AS_REC
	pBufInfo->m_nRecYAddr = pBufInfo->m_nCurYAddr;
	pBufInfo->m_nRecCAddr = pBufInfo->m_nCurCAddr;
#else
	{
		MEMMAP_t MemTemp[2];
             memset(MemTemp,0,2*sizeof(MEMMAP_t));
		bufpool_get(&pInfo->BufPool, MemTemp);
		pBufInfo->m_nRecYAddr = MemTemp[0];
		pBufInfo->m_nRecCAddr = MemTemp[1];
	}
#endif

	if (pInfo->dpb[0].is_used) {
		pBufInfo->m_nRefYAddr[0] = pInfo->dpb[0].addr[0];
		pBufInfo->m_nRefCAddr[0] = pInfo->dpb[0].addr[1];
	}

	if (pInfo->dpb[1].is_used) {
		pBufInfo->m_nRefYAddr[1] = pInfo->dpb[1].addr[0];
		pBufInfo->m_nRefCAddr[1] = pInfo->dpb[1].addr[1];
	}

	pBufInfo->m_bEnableMvStore = 0;
}

