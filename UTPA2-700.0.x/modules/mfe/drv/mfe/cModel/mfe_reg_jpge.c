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

#if defined(WIN32)
	#include "sigdump/sigdump_api.h"
#endif


void OutputSwCfg1_Jpg(MFE_CONFIG *pConfig, int nFrmNum, JpgInfo *pJpgInfo, BufInfo* pBufInfo, BitsInfo *pBitsInfo)
{
	int nTarWriteCount;
	int nRegWriteCount;
	int nTarFDCCount;
	int nRegFDCCount;

	void* pContext;
	pContext = (void*)pBitsInfo;

	//////////////////////////////////////////////////////////////////////////
	// Sequence-wide settings

	if (nFrmNum==0) {
		memset(&mfe_reg, 0, sizeof(MFE_REG));	// Initial
		mfe_reg.reg_mfe_g_enc_mode = REG_ENC_MODE_JPEG;
		mfe_reg.reg_mfe_g_pic_width = (pJpgInfo->nWidth+15)&~15;
		mfe_reg.reg_mfe_g_pic_height = (pJpgInfo->nHeight+7)&~7;

		mfe_reg.reg_mfe_g_qmode = 1;	// Always with quant-tables
		mfe_reg.reg_mfe_g_jpe_enc_mode = 1;	// current version supports 422 only
		mfe_reg.reg_mfe_g_jpe_buffer_mode = pJpgInfo->nFrameMode;
		if (pJpgInfo->nFrameMode==0)
			mfe_reg.reg_mfe_g_jpe_multibuf_mode = pBufInfo->m_nRowBufCount==2 ? 0 : (pBufInfo->m_nRowBufCount==4 ? 1 : 2);
		mfe_reg.reg_mfe_g_jpe_qfactor = pJpgInfo->nQFactor;		// 3 means no altering qtable
		mfe_reg.reg_mfe_g_packed_mode = pJpgInfo->nUseYVYU ? 0 : 1;
	}
	else {
		mfe_reg.reg_mfe_g_frame_start_sw = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// Frame-wide settings
#ifdef CLOCK_GATING
    mfe_reg.reg16 = 0xffff;	// clock gating
#endif
	// Input buffer address: Must be 256-byte aligned.
	MFE_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nCurCAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefYAddr[0].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefYAddr[1].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[0].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[1].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress&0x7)==0);
	mfe_reg.reg_mfe_g_cur_y_adr_low  = (MFE_U16)((pBufInfo->m_nCurYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_cur_y_adr_high = (MFE_U16)(pBufInfo->m_nCurYAddr.miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_cur_c_adr_low  = (MFE_U16)((pBufInfo->m_nCurCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_cur_c_adr_high = (MFE_U16)(pBufInfo->m_nCurCAddr.miuAddress>>(8+16));
	//if (mfe_reg.reg_mfe_g_enc_mode!=REG_ENC_MODE_JPEG)
	{
		mfe_reg.reg_mfe_g_ref_y_adr0_low  = (MFE_U16)((pBufInfo->m_nRefYAddr[0].miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_ref_y_adr0_high = (MFE_U16)(pBufInfo->m_nRefYAddr[0].miuAddress>>(8+16));
		mfe_reg.reg_mfe_g_ref_y_adr1_low  = (MFE_U16)((pBufInfo->m_nRefYAddr[1].miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_ref_y_adr1_high = (MFE_U16)(pBufInfo->m_nRefYAddr[1].miuAddress>>(8+16));
		mfe_reg.reg_mfe_g_ref_c_adr0_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[0].miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_ref_c_adr0_high = (MFE_U16)(pBufInfo->m_nRefCAddr[0].miuAddress>>(8+16));
		mfe_reg.reg_mfe_g_ref_c_adr1_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[1].miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_ref_c_adr1_high = (MFE_U16)(pBufInfo->m_nRefCAddr[1].miuAddress>>(8+16));
#ifdef USE_CUR_AS_REC
		MFE_ASSERT(pBufInfo->m_nRecYAddr.miuAddress==pBufInfo->m_nCurYAddr.miuAddress);
		MFE_ASSERT(pBufInfo->m_nRecCAddr.miuAddress==pBufInfo->m_nCurCAddr.miuAddress);
#endif
		mfe_reg.reg_mfe_g_rec_y_adr_low  = (MFE_U16)((pBufInfo->m_nRecYAddr.miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_rec_y_adr_high = (MFE_U16)(pBufInfo->m_nRecYAddr.miuAddress>>(8+16));
		mfe_reg.reg_mfe_g_rec_c_adr_low  = (MFE_U16)((pBufInfo->m_nRecCAddr.miuAddress>>8)&0xFFFF);
		mfe_reg.reg_mfe_g_rec_c_adr_high = (MFE_U16)(pBufInfo->m_nRecCAddr.miuAddress>>(8+16));
	}

	// Output buffers: Must be 8-byte aligned.
	mfe_reg.reg_mfe_s_bspobuf_sadr_low  = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>3)&0xFFFF);
	mfe_reg.reg_mfe_s_bspobuf_sadr_high = (MFE_U16)(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>(3+16));
	mfe_reg.reg_mfe_s_bspobuf_eadr_low  = (MFE_U16)(((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>3)&0xFFFF);
	mfe_reg.reg_mfe_s_bspobuf_eadr_high = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>(3+16));

	mfe_reg.reg_mfe_s_txip_irfsh_en = 0;

	mfe_reg.reg_mfe_g_frame_type = 0;	// JPEG: always I-frame
	mfe_reg.reg_mfe_s_quan_idx_last = pJpgInfo->nLastZZ;
	if (mfe_reg.reg_mfe_s_quan_idx_last<63)
		mfe_reg.reg_mfe_s_quan_idx_swlast = 1;
	else
		mfe_reg.reg_mfe_s_quan_idx_swlast = 0;

	//////////////////////////////////////////////////////////////////////////
	// swcfg1 output
	nTarWriteCount = 0;
	nRegWriteCount = 0;
	nTarFDCCount = 0;
	nRegFDCCount = 0;
	if (nFrmNum==0) {
		MFE_ASSERT(mfe_reg.reg_mfe_g_qmode==1);
		WriteQTable(pJpgInfo->QTable[0], pJpgInfo->QTable[1]);
	}
#ifdef WIN32
	sd_output_rel(sd_sw_cfg1, "#frame %X", nFrmNum);
#endif

	nTarWriteCount = 34;
#ifndef DONT_PUT_FDC
	nTarFDCCount = PutFDC(pContext, 1);
	nTarWriteCount += nTarFDCCount*3+1;
#endif
#ifdef TEST_IMGBUF_FULL
    nTarWriteCount += 2;
#endif

#ifdef WIN32
	sd_output(sd_sw_cfg1, nTarWriteCount, 10, "NO_OF_REG_WRITE");
	sd_output(sd_sw_cfg1, nTarFDCCount, 10, "NO_OF_FDC_WRITE");
#endif
#ifndef DONT_PUT_FDC
	nTarFDCCount *= 3;
	nTarFDCCount++;	// reg to set fdc round
#endif

	// SW reset
	mfe_reg.reg_mfe_g_soft_rstz = 0;
	WriteRegMFE(0x0, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "SW reset 0");
	mfe_reg.reg_mfe_g_soft_rstz = 1;
	WriteRegMFE(0x0, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "SW reset 1");

    mfe_reg.reg_mfe_g_viu_soft_rstz=0;
    WriteRegMFE(0x18, mfe_reg.reg18, "[%d] reg18", nRegWriteCount++, "VIU reset 0");
    mfe_reg.reg_mfe_g_viu_soft_rstz=1;
    WriteRegMFE(0x18, mfe_reg.reg18, "[%d] reg18", nRegWriteCount++, "VIU reset 1");


	WriteRegMFE(0x1, mfe_reg.reg01, "[%d] reg01", nRegWriteCount++, "picture width");
	WriteRegMFE(0x2, mfe_reg.reg02, "[%d] reg02", nRegWriteCount++, "picture height");
	WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "value");

    WriteRegMFE(0x16, mfe_reg.reg16, "[%d] reg16", nRegWriteCount++, "Clock gating");

	// Input buffers
	//
	WriteRegMFE(0x06, mfe_reg.reg06, "[%d] reg06", nRegWriteCount++, "current luma base address");
	WriteRegMFE(0x07, mfe_reg.reg07, "[%d] reg07", nRegWriteCount++, "current luma base address high");
	WriteRegMFE(0x08, mfe_reg.reg08, "[%d] reg08", nRegWriteCount++, "current chroma base address");
	WriteRegMFE(0x09, mfe_reg.reg09, "[%d] reg09", nRegWriteCount++, "current chroma base address high");
	WriteRegMFE(0x0a, mfe_reg.reg0a, "[%d] reg0a", nRegWriteCount++, "reference luma base address0");
	WriteRegMFE(0x0b, mfe_reg.reg0b, "[%d] reg0b", nRegWriteCount++, "reference luma base address0 high");
	WriteRegMFE(0x0c, mfe_reg.reg0c, "[%d] reg0c", nRegWriteCount++, "reference luma base address1");
	WriteRegMFE(0x0d, mfe_reg.reg0d, "[%d] reg0d", nRegWriteCount++, "reference luma base address1 high");
	WriteRegMFE(0x0e, mfe_reg.reg0e, "[%d] reg0e", nRegWriteCount++, "reference chroma base address0");
	WriteRegMFE(0x0f, mfe_reg.reg0f, "[%d] reg0f", nRegWriteCount++, "reference chroma base address0 high");
	WriteRegMFE(0x10, mfe_reg.reg10, "[%d] reg10", nRegWriteCount++, "reference chroma base address1");
	WriteRegMFE(0x11, mfe_reg.reg11, "[%d] reg11", nRegWriteCount++, "reference chroma base address1 high");
	WriteRegMFE(0x12, mfe_reg.reg12, "[%d] reg12", nRegWriteCount++, "reconstructed luma base address:");
	WriteRegMFE(0x13, mfe_reg.reg13, "[%d] reg13", nRegWriteCount++, "reconstructed luma base address high");
	WriteRegMFE(0x14, mfe_reg.reg14, "[%d] reg14", nRegWriteCount++, "reconstructed chroma base address:");
	WriteRegMFE(0x15, mfe_reg.reg15, "[%d] reg15", nRegWriteCount++, "reconstructed chroma base address: high");

	// Output buffer
	WriteRegMFE(0x3c, mfe_reg.reg3c, "[%d] reg3c", nRegWriteCount++, "bsp obuf start address: ");
	WriteRegMFE(0x3d, mfe_reg.reg3d, "[%d] reg3d", nRegWriteCount++, "bsp obuf start address high");
	WriteRegMFE(0x3e, mfe_reg.reg3e, "[%d] reg3e", nRegWriteCount++, "bsp obuf end address: ");
	WriteRegMFE(0x3f, mfe_reg.reg3f, "[%d] reg3f", nRegWriteCount++, "bsp obuf end address high");
	//
	mfe_reg.reg_mfe_s_bspobuf_set_adr = 1;
	mfe_reg.reg_mfe_s_bspobuf_fifo_th = 1;
	mfe_reg.reg_mfe_s_mvobuf_set_adr = 0;
	mfe_reg.reg_mfe_s_mvobuf_fifo_th = 0;
	mfe_reg.reg_mfe_s_bsp_fdc_skip = 0;
#if defined(TEST_INPUT_ROW_MODE_HW) || defined(DONT_PUT_FDC)
	mfe_reg.reg_mfe_s_bsp_fdc_skip =1;
#endif
	WriteRegMFE(0x3b, mfe_reg.reg3b, "[%d] reg3b", nRegWriteCount++, "set bsp obuf");
    mfe_reg.reg_mfe_s_bspobuf_set_adr = 0;	// HW is write-one-clear

#ifdef TEST_INPUT_ROW_MODE_HW
		mfe_reg.reg_mfe_g_jpe_fsvs_mode=2;
#endif

#ifdef TEST_INPUT_ROW_MODE_SW_HW
		mfe_reg.reg_mfe_g_jpe_fsvs_mode=1;
#endif
	WriteRegMFE(0x18, mfe_reg.reg18, "[%d] reg18", nRegWriteCount++, "JPE encode mode");

	// Cross-format wrong reg setting prevention
	WriteRegMFE(0x1b, mfe_reg.reg1b, "[%d] reg1b", nRegWriteCount++, "MPEG4 FieldDCT");

	mfe_reg.reg_mfe_g_crc_mode = 0xc;
    mfe_reg.reg_mfe_g_debug_tcycle_chk_en = 0x1;
#ifdef TEST_CRC_MODE
    mfe_reg.reg_mfe_g_debug_tcycle_chk_sel = 0x1;
#endif
    WriteRegMFE(0x73, mfe_reg.reg73, "[%d] reg73", nRegWriteCount++, "crc mode");

#ifdef TEST_IMGBUF_FULL
    mfe_reg.reg_mfe_s_txip_dbf_full_halt_en=1;
    WriteRegMFE(0x2d, mfe_reg.reg2d, "[%d] reg2d", nRegWriteCount++, "txip controller stop-and-go");

	mfe_reg.reg_mfe_s_txip_idle_cnt = 32000;//160;
	WriteRegMFE(0x33, mfe_reg.reg33, "[%d] reg33", nRegWriteCount++, "txip_idle_cnt");
#endif
	WriteRegMFE(0x2c, mfe_reg.reg2c, "[%d] reg2c", nRegWriteCount++, "Last zigzag");

#if defined(TEST_MB_STOPANDGO) || defined(TEST_MB_STOPANDDROP)
	TestStopAtMb();
#endif

#if defined(TEST_INPUT_ROW_MODE_HW) || defined(TEST_INPUT_ROW_MODE_SW_HW)
	// vsync

		UDMA_RIUWrite16(0x0B00, 2);

	// fake row down 0 to num_of_rowdone_bef_fs
	{
		int i;
		for(i=0;i< num_of_rowdone_bef_fs;i++)
			UDMA_RIUWrite16(0x0B00, 1);
	}


#endif
#ifdef TEST_CRC_MODE
	mfe_reg.reg_mfe_g_crc_mode = 0xc;
//	UDMA_RIUWrite16(REG_BANK_MFE+0x73, mfe_reg.reg73);
#endif
	// Enable HW
	mfe_reg.reg_mfe_g_frame_start_sw = 1;
	WriteRegMFE(0x00, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "frame start");
    mfe_reg.reg_mfe_g_frame_start_sw = 0;	// HW is write-one-clear

	// FDC
#ifndef DONT_PUT_FDC
	nRegFDCCount = PutFDC(pContext, 0);
	nRegWriteCount += nRegFDCCount;
#endif
#if defined(TEST_STOPANDGO) || defined(TEST_STOPANDDROP)
	TestStop();
#endif

#ifdef TEST_INPUT_ROW_MODE_SW_HW

		UDMA_RIURead16(REG_BANK_MFE+0x1e, &mfe_reg.reg1e);
		if( (mfe_reg.reg_mfe_g_irq_cpu & 0x20) == 0x20)
			ms_dprintk(DRV_L4,"\nTEST_INPUT_ROW_MODE_SW_HW test OK\n");


#endif

	// Only for debug
	MFE_ASSERT(nRegFDCCount==nTarFDCCount);
	MFE_ASSERT(nRegWriteCount==nTarWriteCount);
}

