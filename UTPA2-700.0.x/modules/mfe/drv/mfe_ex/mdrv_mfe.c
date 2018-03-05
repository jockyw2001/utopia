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

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"

#include "mdrv_mfe_io.h"
#include "drv_mfe_st.h"
#include "parset.h"

#if (defined(_MFE_T8_)||defined(_MFE_M1_)) \
   &&defined(_MIPS_PLATFORM_)&&(!defined(_KERNEL_MODE_))&&(!defined(WIN32))
    #include "MsCommon.h"
    #include "MsOS.h"
    #include "drvMMIO.h"
#endif

#if defined(__UBOOT__)
    #include <linux/string.h>
#else
    //#include "drvMFE.h"
    #include <string.h>
#endif

#include "mhal_mfe.h"
#include "mdrv_mfe.h"
#include "stdarg.h"
#include "BufMng.h"
#ifdef __MFE_G2__
#include "Exif.h"
#include "jcmarker.h"
#endif

//MFE_CONFIG *gpMfeConfig;
//GOPINFO *gpGopInfo;

//out buffer
//OutBitSBUF outbitsbuf[MAX_OUTBUFFER];
//MS_S32 rbits_index=0;

//static MS_U32 _MFE_U32MFEEvent = 0;
//BITSFRAME_INFO bitsframe[MAX_OUTBUFFER];
//volatile MFE_STAT encode_state;
//MS_BOOL g_bForceIframe = FALSE;
//MS_U32 MFE_U32MFEMIUOffset=0;

#if (DEBUG_LEVEL & DRV_L6)
#include "api_mfe_performance.h"
mfe_performance_t mfe_enc;
mfe_performance_t mfe_hw_enc;
mfe_performance_t mfe_duration_enc;
MS_S32 mfecount = 0;
#endif

#define DROP_TEST 0UL

#if defined(MFE_SUPPORT_TLB)
#define MFE_ENABLE_TLB_FLAG 0x1UL
#endif

#if DROP_TEST
MS_S32 mfe_count;
#endif

static MS_BOOL MDrv_MFE_CheckEncodeDone(MFE_CONFIG *pConfig);

// user mode function
#if (defined(_MFE_T8_)||defined(_MFE_M1_)) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MDrv_MFE_SetOSRegBase(MFE_CONFIG *pConfig, MS_U32 MFE_U32RegBaseAddr )
{
    MHAL_MFE_InitRegBase((MS_U32)MFE_U32RegBaseAddr );
}
#endif


//////////////////////////////////////////////////////////////////////////
// ioctl API's
//////////////////////////////////////////////////////////////////////////

static void MDrv_MFE_ENC_Continue(MFE_CONFIG *pConfig)
{
    MS_ASSERT(pConfig->outbitsbuf[pConfig->nOBufIndex].used_size==0);
    ms_dprintk(DRV_L3, "Continue with buf[%u]\n", (unsigned int)pConfig->nOBufIndex);
       MHal_MFE_set_outbitsbuf(&pConfig->mfe_reg, &pConfig->outbitsbuf[pConfig->nOBufIndex],pConfig->ctxBufInfo.m_OutBufferSize);
    pConfig->encode_state = WAIT_FRAME_DONE;
}


static void MDrv_MFE_clear_bitsbuf(MFE_CONFIG *pConfig, MS_U32 clear_num)
{

    ms_dprintk(DRV_L3, "clear rbits_index = %d, state = %d, clear_num = %u\n", (int)pConfig->rbits_index, pConfig->encode_state, (unsigned int)clear_num);
#ifndef _NON_THREAD_
    spin_lock(&_spinlock);
#endif
    MS_ASSERT(clear_num==1);

    pConfig->outbitsbuf[pConfig->rbits_index].used_size = 0;
    pConfig->rbits_index = MFE_MUX((pConfig->rbits_index+1)==pConfig->ctxBufInfo.m_nOutBuffer, 0, pConfig->rbits_index+1);
#ifndef _NON_THREAD_
    spin_unlock(&_spinlock);
#endif
    if (pConfig->encode_state==WAIT_AVAIL_BITSBUF)
        MDrv_MFE_ENC_Continue(pConfig);

}

static MS_S32 MDrv_MFE_GetDispOrder(MFE_CONFIG *pConfig, GOPINFO *pGopInfo, MS_U32 *pDispOrder)
{


    if (!pDispOrder) {
        ms_dprintk(DRV_L4, "MDrv_MFE_GetDispOrder: pDispOrder is NULL pointer.\n");
        return 0;
    }

    // If H264 PAFF, always assume Top-field first, assume top -> bottom -> top -> bottom input fields.
    if (pConfig->nCodecType==REG_ENC_MODE_H264 && (pConfig->bInterlace!=PROGRESSIVE)) {
        if (pConfig->ctxH264Info.structure==FRAME) // Initial value
            pConfig->ctxH264Info.structure = TOP_FIELD;
        else if (pConfig->ctxH264Info.structure==TOP_FIELD)
            pConfig->ctxH264Info.structure = BOTTOM_FIELD;
        else if (pConfig->ctxH264Info.structure==BOTTOM_FIELD)
            pConfig->ctxH264Info.structure = TOP_FIELD;
    }

    mfeSetVopType(pConfig, pGopInfo);
    *pDispOrder = pGopInfo->nDispOrder;

    ms_dprintk(DRV_L3, "MDrv_MFE_GetDispOrder: Enc=%u Disp=%u\n", (unsigned int)pGopInfo->nCodingOrder, (unsigned int)(*pDispOrder));
    return 1;
}


// Consumer of copybits2file()
static MS_S32 MDrv_MFE_GetBits(MFE_CONFIG* gpMfeConfig, BITSFRAME_INFO *bits_info)
{
    MS_S32 i=0;
    MS_S32 index;
    MS_S32 num_of_buffers = gpMfeConfig->ctxBufInfo.m_nOutBuffer;
    ms_dprintk(DRV_L3, "MDrv_MFE_GetBits...\n");

    if(gpMfeConfig->PollingMode) {
        MS_U16 val_1e = 0;

        do{
            MHal_MFE_GetIRQ(&val_1e);
            if(val_1e == 0) {
                ms_dprintf(DRV_L3, "Wait frame done");
#ifdef _MFE_UTOPIA_
                MsOS_DelayTask(5);
#else
                Sleep(5);
#endif
            }
        }
        while(val_1e == 0);
    }
#if (DEBUG_LEVEL & DRV_L6)
    if(gpMfeConfig->GopInfo.nCodingOrder > 2){
        mfe_enc.mfe_endtime = MsOS_GetSystemTime();
        mfe_enc.mfe_total_time += (mfe_enc.mfe_endtime-mfe_enc.mfe_starttime);
        mfe_enc.mfe_total_count ++;

        mfe_enc.mfe_maxtime = mfe_get_max(mfe_enc.mfe_maxtime,mfe_enc.mfe_endtime-mfe_enc.mfe_starttime);
        mfe_enc.mfe_mintime = mfe_get_min(mfe_enc.mfe_mintime,mfe_enc.mfe_endtime-mfe_enc.mfe_starttime);
        ms_dprintk(DRV_L6,"mfe count = %d\n",mfecount);
        ms_dprintk(DRV_L6,"[TIME] total [%d] frms cur: %d, avg: %d us [MAX] %d [MIN] %d\n",
            mfe_enc.mfe_total_count,mfe_enc.mfe_endtime-mfe_enc.mfe_starttime,
            mfe_enc.mfe_total_time/mfe_enc.mfe_total_count,
            mfe_enc.mfe_maxtime,mfe_enc.mfe_mintime);
        {
            MS_U32 clk;
            MS_S32 cur_time;
#ifdef _MFE_EDISON_
             switch (gpMfeConfig->MFEClk)
            {
                case MFE_CLK_VERY_SLOW:
                    clk = 144; break;
                case MFE_CLK_SLOW:
                    clk = 192; break;
                case MFE_CLK_MEDIUM:
                    clk = 216; break;
                case MFE_CLK_FAST:
                    clk = 240; break;
    			default:
    				clk = 240;
            }
#else
             switch (gpMfeConfig->MFEClk)
            {
                case MFE_CLK_VERY_SLOW:
                    clk = 24; break;
                case MFE_CLK_SLOW:
                    clk = 64; break;
                case MFE_CLK_MEDIUM:
                    clk = 128; break;
                case MFE_CLK_FAST:
                    clk = 192; break;
    			default:
    				clk = 192;
            }
#endif
             cur_time = MHal_MFE_CycleReport()/clk;
             mfe_hw_enc.mfe_total_time += cur_time;
            mfe_hw_enc.mfe_maxtime = mfe_get_max(mfe_hw_enc.mfe_maxtime,cur_time);
            mfe_hw_enc.mfe_mintime = mfe_get_min(mfe_hw_enc.mfe_mintime,cur_time);
            ms_dprintk(DRV_L6,"[TIME] HW report: %d us, avg: %d us [MAX] %d [MIN] %d\n",cur_time,
                mfe_hw_enc.mfe_total_time/mfe_enc.mfe_total_count,mfe_hw_enc.mfe_maxtime,mfe_hw_enc.mfe_mintime);
        }
    }
#endif
#ifdef _GenSkipHeader_

#endif
    if(MDrv_MFE_CheckEncodeDone(gpMfeConfig) == FALSE) {
        bits_info[0].status = gpMfeConfig->bitsframe[gpMfeConfig->rbits_index].status = BITS_INFO_ZERO;
        //ms_dprintk(DRV_L3, "MDrv_MFE_GetBits...fail!!\n");
        return 1;
    }
        ms_dprintk(DRV_L3, "MDrv_MFE_GetBits...done\n");


#ifndef _NON_THREAD_
    spin_lock(&_spinlock);
#endif
    index = gpMfeConfig->rbits_index;
    // Now we only copy one buffer
    if (gpMfeConfig->_MFE_U32MFEEvent!=0) {
        bits_info[i].miuAddress = gpMfeConfig->bitsframe[index].miuAddress;
        bits_info[i].miuPointer = gpMfeConfig->bitsframe[index].miuPointer;
        bits_info[i].size = gpMfeConfig->bitsframe[index].size;
        bits_info[i].status = gpMfeConfig->bitsframe[index].status;
        bits_info[i].voptype = gpMfeConfig->bitsframe[index].voptype;
        bits_info[i].is_more_bits = 1;
        bits_info[i].IVOP_address= gpMfeConfig->bitsframe[index].IVOP_address;
        gpMfeConfig->_MFE_U32MFEEvent ^= (1<<index);
        index = MFE_MUX((index+1)==num_of_buffers, 0, index+1);
        i++;
        if (i>num_of_buffers) {
            ms_dprintk(DRV_L0, "MDrv_MFE_GetBits error!! use too many bits buffer\n");
        }
    }

    bits_info[i-1].is_more_bits = 0;
#ifndef _NON_THREAD_
    spin_unlock(&_spinlock);
#endif
    ms_dprintk(DRV_L3, "Leave MDrv_MFE_GetBits.\n");
    return 0;
}


static MS_S32 MDrv_MFE_EncodeVOP_End(MFE_CONFIG *pConfig)
{
/*
    //mark the last frame is Drop.
    if(gpMfeConfig->VTMode && rc_CheckSkippedFrame(&gpMfeConfig->VTRateCtrl)) {
        pConfig->nDropFlag = 1;
        pConfig->m_cvbrFrameSkip = 1;
        MfeDrvRateControlUpdate(gpMfeConfig, 0);
		return 0;
    }
    else {
        pConfig->nDropFlag = 0;
    }
*/
#if DROP_TEST
    mfe_count++;
    if(mfe_count == 6) {
        pConfig->nDropFlag = 1;
        return 0;
    }
    else
        pConfig->nDropFlag = 0;
#endif
    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
    case REG_ENC_MODE_H263:
        mfeM4VE_UpdateFrame(pConfig);
        break;
    case REG_ENC_MODE_H264:
        mfeH264_UpdateFrame(pConfig);
        break;
    case REG_ENC_MODE_JPEG:
        //MS_ASSERT(0);
        break;
    }
#ifdef _GenSkipHeader_
        if(pConfig->VTMode && (pConfig->vopPredType==I_VOP))
#endif
            pConfig->bForceIframe = FALSE;

    //ms_dprintk(DRV_L3, "pConfig->vopPredType = %d\n",pConfig->vopPredType);
    ms_dprintk(DRV_L3, "encode one frame end\n");
    return 0;
}

static MS_U32 get_hw_encode_done_size(MS_U32 start)
{
    MS_U16 u16Reg1, u16Reg2;
    MS_U32 hwlen;
    MS_U32 wptr;
    MS_S32 size;
    // Final frame bit count -> byte count
    ReadRegMFE(0x42, &u16Reg1);
    ReadRegMFE(0x43, &u16Reg2);
    //ms_dprintk(DRV_L3, "u16Reg2 = 0x%x, u16Reg1 = 0x%x\n", u16Reg2, u16Reg1);
    hwlen = (((u16Reg2)<<16) | u16Reg1)>>3;
    ms_dprintk(DRV_L3, "start = 0x%x, hwlen = 0x%x\n", (unsigned int)start, (unsigned int)hwlen);

    // The last outbuf filled-size
    ReadRegMFE(0x44, &u16Reg1);
    ReadRegMFE(0x45, &u16Reg2);
    wptr = ((u16Reg2<<16) | u16Reg1)<<3;
    ms_dprintk(DRV_L3, "bspobuf write pointer wptr = 0x%x\n", (unsigned int)wptr);
    size = wptr - start + ((hwlen&7)?(hwlen&7):8);
    if (size<=0)
        ms_dprintk(DRV_L0, "[Error] copybits2file(): size %d < 0. start = 0x%x, wptr = 0x%x, hwlen = 0x%x\n",
            (int)size, (unsigned int)start, (unsigned int)wptr, (unsigned int)hwlen);

    return size;
}

// Supplier of MDrv_MFE_GetBits()
static MS_BOOL copybits2file(MFE_CONFIG *pConfig, MS_U16 val_1e)
{
    MS_S32 wbits_index;
    MS_U8   voptype = pConfig->vopPredType;
    MS_U32 Y_start = pConfig->ctxBufInfo.m_nRecYAddr.miuAddress;
    MS_U32 start = 0;
    MS_S32 size = 0;
    MS_S32 bFrameDone = CHECK_IRQ_STATUS(val_1e, IRQ_FRAME_DONE);
    MS_S32 bBufFull = CHECK_IRQ_STATUS(val_1e, IRQ_BSPOBUF_FULL);    // Output buffer full.
    MS_S32 bImgFull = CHECK_IRQ_STATUS(val_1e, IRQ_IMG_BUF_FULL);    // Input buffer full. Only when JPEG row-mode.

    start = pConfig->outbitsbuf[pConfig->nOBufIndex].start_addr;

    if (bBufFull) {
        ms_dprintk(DRV_L3, "copybits2file(): Buffer full.\n");
        size = pConfig->ctxBufInfo.m_OutBufferSize;
        //MS_ASSERT(size == pConfig->ctxBufInfo.m_OutBufferSize);
    }
    else if (bFrameDone) {
        ms_dprintk(DRV_L3, "copybits2file(): Frame done.\n");
#ifdef _GenSkipHeader_
              if(pConfig->VTMode && rc_CheckSkippedFrame(&pConfig->VTRateCtrl)){
                OutStream* pStream = &pConfig->m_OutStream;
                size = pStream->m_nByteCount;
                memcpy(addr_phy2log(start + pConfig->MFE_U32MFEMIUOffset),
                    pStream->m_pbFrameBuffer, pStream->m_nByteCount);
              }
              else
#endif
            if(pConfig->SecurityMode) {
                MS_U16 u16Reg1, u16Reg2;
                ReadRegMFE(0x42, &u16Reg1);
                ReadRegMFE(0x43, &u16Reg2);
                ms_dprintk(DRV_L3, "size = 0x%x\n", (unsigned int)size);
                size = (((u16Reg2)<<16) | u16Reg1)>>3;
            } else {
                 size = get_hw_encode_done_size(start);
            }

    }
    else if (bImgFull) {
        start = 0;
        size = 0;
             ms_dprintk(DRV_L3, "image buff full!0x%x\n", val_1e);
    }
    else {
        ms_dprintk(DRV_L0, "copybits2file(): Error IRQ=0x%x not handled!\n", val_1e);
        return FALSE;
    }

    //clear all set irq, need to set before change bitstream buffer
    wbits_index = pConfig->nOBufIndex;
    pConfig->nOBufIndex = (pConfig->nOBufIndex+1)%pConfig->ctxBufInfo.m_nOutBuffer; //next outbuf
    //ms_dprintk(DRV_L3, "OBufIndex=%d, state=%d\n", pConfig->nOBufIndex, pConfig->encode_state);


    if (bBufFull) {
        ms_dprintk(DRV_L3, "MFE encoding bitsbuf full! buf %u, size %d\n", (unsigned int)pConfig->nOBufIndex, (int)size);
        pConfig->encode_state = WAIT_AVAIL_BITSBUF;
    }

    MHal_MFE_ClearIRQ(val_1e);    // Conrad: Should this be here or above??

    // Output info filling
    pConfig->bitsframe[wbits_index].miuAddress = start;
    pConfig->bitsframe[wbits_index].miuPointer = (MS_U32)addr_phy2log(start + pConfig->MFE_U32MFEMIUOffset);
    pConfig->bitsframe[wbits_index].size = size;

    //CABAC stuffing byte
    if(pConfig->UseCABAC && bFrameDone) {
        pConfig->bitsframe[wbits_index].stuffing_bytes = MfeDrvCabacStuffingReport(pConfig);
    }
#ifdef WIN32
    if(!pConfig->SecurityMode)
        UDMAOP_DMAGet((MS_S8*)pConfig->bitsframe[wbits_index].miuPointer, size, start, 0);
#endif
    if (bFrameDone)
        pConfig->bitsframe[wbits_index].status = BITS_INFO_FRAMEDONE;
    else if(bBufFull)
        pConfig->bitsframe[wbits_index].status = BITS_INFO_BUFFULL;
    else if (bImgFull)
        pConfig->bitsframe[wbits_index].status = BITS_INFO_IMG_BUFFULL;

    pConfig->bitsframe[wbits_index].voptype = voptype;
    // For I-VOP Thumbnail
    if (voptype==I_VOP) {
        pConfig->bitsframe[wbits_index].IVOP_address = Y_start;
    } else {
        pConfig->bitsframe[wbits_index].IVOP_address = 0;
    }

    pConfig->_MFE_U32MFEEvent |= 1<<wbits_index;

    // NOTE: When acts as kernel mode, wait up for poll()
    //       When acts as user mode, release semaphore (paired with MDrv_MFE_GetBits())
    //printk("next: 0x%x  prev: 0x%x\n", _MFE_wait_queue.task_list.next, _MFE_wait_queue.task_list.prev);
    if (bFrameDone) {
    MDrv_MFE_EncodeVOP_End(pConfig);
#ifdef MFE_MIU_PROTECT
    //MIU MODE, check type(0:BPS,1:MC.2:MV,3:GN)
    MHal_MFE_Enable_MIU_Protection_Check(TEST_MIU_PROTECTION_MODE,0);
#endif
    pConfig->encode_state = WAIT_INPUT_FRAME;
    }

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    if (bFrameDone || bBufFull)
        return TRUE;
    else {
        ms_dprintk(DRV_L0, "[ERROR] CHECK_IRQ_STATUS not bFrameDone or bBufFull.\n");
        return FALSE;
    }
#endif
    return TRUE;
}

static MS_BOOL MDrv_MFE_CheckEncodeDone(MFE_CONFIG *pConfig)
{
    MS_U16 val_1e;
    //MS_U8  voptype = pConfig->vopPredType;
    //MS_S32 sgc_frame_done;
#ifdef _GenSkipHeader_
    if(pConfig->VTMode && rc_CheckSkippedFrame(&pConfig->VTRateCtrl)){
        val_1e = 0x1; //fake frame done
    }
    else
#endif
        MHal_MFE_GetIRQ(&val_1e);

    ms_dprintk(DRV_L3, "CheckEncodeDone() reg1e=0x%x\n",val_1e);
    if (val_1e==0) {
        //ms_dprintk(DRV_L4, "[WARN] CheckEncodeDone() reg1e=0\n");
        return FALSE;
    }
#ifdef MFE_MIU_PROFILE
    {
        MS_U16 tmp_r = 0, tmp_w = 0;
        ReadRegMFE_BANK1(0x49,&tmp_r);
        ReadRegMFE_BANK1(0x44,&tmp_w);
        pConfig->total_r_count += tmp_r;
        pConfig->total_w_count += tmp_w;
        if(pConfig->miu_count == 0) {
            pConfig->max_r_count = tmp_r;
            pConfig->max_w_count = tmp_w;
            pConfig->min_r_count = tmp_r;
            pConfig->min_w_count = tmp_w;
        } else {
            if(tmp_r > pConfig->max_r_count)
                pConfig->max_r_count = tmp_r;
            if(tmp_w > pConfig->max_w_count)
                pConfig->max_w_count = tmp_w;
            if(tmp_r < pConfig->min_r_count)
                pConfig->min_r_count = tmp_r;
            if(tmp_w < pConfig->min_w_count)
                pConfig->min_w_count = tmp_w;
        }
        pConfig->miu_count++;
        ms_dprintk(DRV_L1, "[MIU]cur r/w=(0x%04x,0x%04x), avg (0x%04x,0x%04x), max (0x%04x,0x%04x), min (0x%04x,0x%04x)\n"
            ,tmp_r,tmp_w,pConfig->total_r_count/pConfig->miu_count,pConfig->total_w_count/pConfig->miu_count
            ,pConfig->max_r_count,pConfig->max_w_count,pConfig->min_r_count,pConfig->min_w_count );
    }
#endif
    DumpAllReg(&pConfig->mfe_reg);
    //h1e: [7:0]  status of interrupt on CPU side
    //    {2'b0, net_trigger, fs_fail_irq, txip_time_out, bspobuf_full_irq, img_buf_full_irq, marb_bspobuf_ful, frame_done_irq}
    //sgc_frame_done = CHECK_IRQ_STATUS(val_1e, IRQ_FRAME_DONE);
    copybits2file(pConfig, val_1e);

    return TRUE;
}

static void MDrv_MFE_EncodeVOP(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{

    ms_dprintk(DRV_L2, "MDrv_MFE_EncodeVOP : %d(I/P/B)\n", pConfig->vopPredType);

    //mark the last frame is Drop.
    if(pConfig->VTMode && rc_CheckSkippedFrame(&pConfig->VTRateCtrl)) {
#ifndef _GenSkipHeader_
        MfeDrvRateControlUpdate(pConfig, 0);
        pConfig->nDropFlag = 1;
        return ;
#else
        pConfig->nDropFlag = 0;
#endif
    }
    else {
        pConfig->nDropFlag = 0;
    }

    switch (pConfig->nCodecType) {
        case REG_ENC_MODE_MPG4:
        case REG_ENC_MODE_H263:
            mfeM4VE_EncodeFrame(pConfig, pGopInfo);
            break;
        case REG_ENC_MODE_H264:
            if (pConfig->encodeDummyFrame)
            {
                mfeH264_EncodeDummyFrame(pConfig, pGopInfo);
            }
            else
            {
                mfeH264_EncodeFrame(pConfig, pGopInfo);
            }
            break;
#ifdef _MFE_M1_
        case REG_ENC_MODE_JPEG:
            mfeJPE_EncodeFrame(pConfig, pGopInfo);
            break;
#endif
        default:
            MS_ASSERT(0);
            break;
    }
    pConfig->encode_state = WAIT_FRAME_DONE;

    ms_dprintk(DRV_L3, "In MDrv_MFE_EncodeVOP() After encode_state = WAIT_FRAME_DONE\n");
}

static void mdrvLoadTestCasePre(MFE_CONFIG *pConfig)
{

    if (pConfig->nCodecType==REG_ENC_MODE_H263) {
        pConfig->bQuantType = 0;
    }
    else if(pConfig->nCodecType==REG_ENC_MODE_MPG4) {
        pConfig->bQuantType = 1;
    }

}
static void mdrvLoadTestCasePost(MFE_CONFIG *pConfig)
{

}

static void MDrv_MFE_Init(MFE_CONFIG *pConfig)
{
    MS_S32 i;
#if DROP_TEST
    mfe_count=0;
#endif
    ms_dprintk(DRV_L1, "CodecType = %d\n", (int)pConfig->nCodecType);
    ms_dprintk(DRV_L1, "ColorFormat = %d\n", (int)pConfig->bColorFormat);
    ms_dprintk(DRV_L1, "FrameRatex100 = %d\n", (int)pConfig->FrameRatex100);
    ms_dprintk(DRV_L1, "bitrate = %d\n", (int)pConfig->nBitrate);
    mdrvLoadTestCasePre(pConfig);
    if (pConfig->MfeAdvInfo.low_bandwidth_en){
        MS_U32 imi_addr = (MS_U32)pConfig->MfeAdvInfo.imi_buf_addr.miuAddress;
        MS_U32 imi_size;
        imi_addr = (((imi_addr) + 0x3) & ~0x3);
        imi_size = (((pConfig->nBufWidth + 15)>>4)*2 + 6)*256; //Y MB rows, plus 5MB's

        if(imi_size <= 0x2000)
            imi_size = 0x2000;
        else if(imi_size <= 0x4000)
            imi_size = 0x4000;
        else if(imi_size <= 0x8000)
            imi_size = 0x8000;
        else if(imi_size <= 0x10000)
            imi_size = 0x10000;
        else imi_size = 0;

        pConfig->imi_addr = imi_addr;
        pConfig->imi_size = imi_size;
    }
    switch (pConfig->nCodecType) {
        case REG_ENC_MODE_MPG4:
        case REG_ENC_MODE_H263:
            mfeM4VE_Init(pConfig);
            break;
        case REG_ENC_MODE_H264:
            mfeH264_Init(pConfig);
            break;
#ifdef _MFE_M1_
        case REG_ENC_MODE_JPEG:
            mfeJPE_Init(pConfig);
            break;
#endif
        default:
            MS_ASSERT(0);
    }

    mdrvLoadTestCasePost(pConfig);


    ms_dprintk(DRV_L3, "In MDrv_MFE_Init.. set clock level %d\n", pConfig->MFEClk);
    MHal_MFE_PowerOff(0,pConfig->MFEClk);

    MMAPInit(pConfig->dram_base, pConfig->dram_size, &pConfig->dram_ptr);

    pConfig->encode_state = WAIT_INPUT_FRAME; //init encode_state
    MHal_MFE_ResetReg(&pConfig->mfe_reg);
    //DumpAllReg(pConfig->mfe_reg);

    switch (pConfig->nCodecType) {
        case REG_ENC_MODE_MPG4:
        case REG_ENC_MODE_H263:
            m4veAllocDRAM(pConfig);
            break;
        case REG_ENC_MODE_H264:
            h264AllocDRAM(pConfig);
            break;
#ifdef _MFE_M1_
        case REG_ENC_MODE_JPEG:
            jpeAllocDRAM(pConfig);
            break;
#endif
        default:
            MS_ASSERT(0);
            break;
    }

    for (i=0; i<pConfig->ctxBufInfo.m_nOutBuffer; i++) {
        pConfig->outbitsbuf[i].start_addr  = pConfig->ctxBufInfo.m_nOutBufAddr[i].miuAddress;
        pConfig->outbitsbuf[i].end_addr   = pConfig->ctxBufInfo.m_nOutBufAddr[i].miuAddress+pConfig->ctxBufInfo.m_OutBufferSize;
        pConfig->outbitsbuf[i].used_size  = 0;
     ms_dprintk(DRV_L2, "Obuf index %d: PhyAddr=0x%08x\n", (int)i, (unsigned int)pConfig->outbitsbuf[i].start_addr);
    }
    pConfig->nOBufIndex = 0;

#if (DEBUG_LEVEL & DRV_L6)
    mfe_performance_init(& mfe_enc);
    mfe_performance_init(& mfe_hw_enc);
    mfe_performance_init(& mfe_duration_enc);
    mfecount=0;
#endif

#ifdef MFE_MIU_PROFILE
    pConfig->miu_count = 0;
    pConfig->total_r_count = 0;
    pConfig->total_w_count = 0;
#endif
    MHal_MFE_ClearIRQ(0x3f);
}


static MS_S32 MDrv_MFE_ENC_OneFrame(MFE_CONFIG* gpMfeConfig, MEMMAP_CUR_t *YUV_addr)
{
    MS_S32 i;

    gpMfeConfig->encode_state = WRITE_MFE_REG;
#if (DEBUG_LEVEL & DRV_L6)
    {
        MS_U32 cur_time = MsOS_GetSystemTime();
        if(gpMfeConfig->GopInfo.nCodingOrder > 2){
            mfe_duration_enc.mfe_durationtime = cur_time - mfe_duration_enc.mfe_lasttime;
            mfe_duration_enc.mfe_maxtime = mfe_get_max(mfe_duration_enc.mfe_maxtime,cur_time);
            mfe_duration_enc.mfe_mintime = mfe_get_min(mfe_duration_enc.mfe_mintime,cur_time);
            mfe_duration_enc.mfe_total_time += mfe_duration_enc.mfe_durationtime;
            mfe_duration_enc.mfe_total_count++;
            ms_dprintk(DRV_L6, "cur_time = %u,duration = %d,[AVG]:%d, [MAX]:%d,[MIN]:%d\n",cur_time,mfe_duration_enc.mfe_durationtime,
                mfe_duration_enc.mfe_total_time/mfe_duration_enc.mfe_total_count,
                mfe_duration_enc.mfe_maxtime,mfe_duration_enc.mfe_mintime );
        }
        else {
            mfe_duration_enc.mfe_total_time = 0;
            mfe_duration_enc.mfe_total_count = 0;
            mfe_duration_enc.mfe_maxtime = cur_time;
            mfe_duration_enc.mfe_mintime = cur_time;
        }
        mfe_duration_enc.mfe_lasttime = cur_time;
    }
#endif
    for (i=0; i<gpMfeConfig->ctxBufInfo.m_nOutBuffer; i++) {
        MS_ASSERT(gpMfeConfig->outbitsbuf[i].used_size==0);
        gpMfeConfig->outbitsbuf[i].used_size = 0;
    }

    ms_dprintk(DRV_L2, "start to encode frame #%u(Enc) #%u(Disp)\n", (unsigned int)gpMfeConfig->GopInfo.nCodingOrder, (unsigned int)gpMfeConfig->GopInfo.nDispOrder);

    gpMfeConfig->ctxBufInfo.m_nCurYAddr = YUV_addr->Cur_Y0;
    gpMfeConfig->ctxBufInfo.m_nCurCAddr = YUV_addr->Cur_C0;

    if(gpMfeConfig->m_bFrameMode == 0) {
        gpMfeConfig->ctxBufInfo.m_nRefYAddr[1] = YUV_addr->Cur_Y1;
        gpMfeConfig->ctxBufInfo.m_nRefCAddr[1] = YUV_addr->Cur_C1;
    }

    MDrv_MFE_EncodeVOP(gpMfeConfig, &gpMfeConfig->GopInfo);

    // If H264 PAFF, always assume Top-field first
    if (!(gpMfeConfig->nCodecType==REG_ENC_MODE_H264 && gpMfeConfig->ctxH264Info.structure==TOP_FIELD))
        gpMfeConfig->GopInfo.nCodingOrder++;
    return 0;
}


static MS_S32 MDrv_MFE_Finish(MFE_CONFIG* pConfig)
{

    switch (pConfig->nCodecType) {
        case REG_ENC_MODE_MPG4:
        case REG_ENC_MODE_H263:
            break;
        case REG_ENC_MODE_H264:
            mfeH264_DeInit(pConfig);
            break;
        case REG_ENC_MODE_JPEG:
            break;
        default:
            MS_ASSERT(0);
            break;
    }

    MfeDrvRateControlDeInit(pConfig);
#if !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
#ifndef _NON_THREAD_
    MDrv_MFE_Sem_Destroy();
    pthread_mutex_destroy(&_spinlock);
#endif
#endif

    //MHal_MFE_PowerOff(1,pConfig->MFEClk);

    return 0;
}

static void MDrv_MFE_SetMIUOffset(MFE_CONFIG *pConfig, MS_U32 MFE_U32MIUOffset )
{
    ms_dprintk(DRV_L1, "MFE_U32MFEMIUOffset = 0x%x\n", (unsigned int)MFE_U32MIUOffset);
    pConfig->MFE_U32MFEMIUOffset = MFE_U32MIUOffset;
}

// Output: Fill 16bit byte-length into pOutBuf[0] (LSB) and pOutBuf[1] (MSB)
//         Then following pOutBuf+2 is the byte stream of video headers.
static void MDrv_MFE_GetConfigBytes(MFE_CONFIG *pConfig, void* pHeader_info)
{
    OutStream* pStream = NULL;
    MS_U8* pBuf = NULL;

    if (pHeader_info ==NULL || pConfig==NULL)
        return;

    pStream = &pConfig->m_OutStream;

    if(rc_CheckSkippedFrame(&pConfig->VTRateCtrl)){
        VOL_INFO_t *vol_info = (VOL_INFO_t *)pHeader_info;
        vol_info->Len = codeM4vConfigHeaders(pConfig,TRUE);
        pBuf = vol_info->pHeader;
        goto EXIT;
    }

    switch (pConfig->nCodecType) {
    case REG_ENC_MODE_MPG4:
        {
            VOL_INFO_t *vol_info = (VOL_INFO_t *)pHeader_info;
            vol_info->Len = codeM4vConfigHeaders(pConfig,FALSE);
            pBuf = vol_info->pHeader;
            break;
        }
    case REG_ENC_MODE_H264:
        {
            SPSPPS_INFO_t *spspps_info = (SPSPPS_INFO_t*)pHeader_info;
            codeH264ConfigHeaders(pConfig,&spspps_info->SPSLen,&spspps_info->PPSLen);
            spspps_info->pHeaderPPS = spspps_info->pHeaderSPS + spspps_info->SPSLen;
            pBuf = spspps_info->pHeaderSPS;
            break;
        }
    case REG_ENC_MODE_H263:
    case REG_ENC_MODE_JPEG:
        // pBuf is NULL
        return;
    }

EXIT:
    if (pBuf)
        memcpy(pBuf, pStream->m_pbFrameBuffer, pStream->m_nByteCount);

}

MS_S32 MDrv_MFE_Ioctl(MS_VIRT MFE_fd, MS_U32 cmd, MS_VIRT  arg)
{
    PVR_Info *pappPVR_Info;
    BITSFRAME_INFO *bits_info;
    MFE_CONFIG* gpMfeConfig = (MFE_CONFIG *)MFE_fd;
    switch(cmd)
    {
    case MFE_IOC_INIT:
        if (gpMfeConfig != NULL && arg != 0) {
            //MFE_CONFIG *gpMfeConfig = (MFE_CONFIG *)MFE_fd;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_INIT\n");
            //gpMfeConfig = malloc(sizeof(MFE_CONFIG));
            memset(gpMfeConfig, 0, sizeof(MFE_CONFIG));
            memset(&gpMfeConfig->GopInfo, 0, sizeof(GOPINFO));
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->rbits_index = 0;
            gpMfeConfig->GopInfo.nCodingOrder = 0;
            gpMfeConfig->_MFE_U32MFEEvent = 0;
            gpMfeConfig->bForceIframe = FALSE;
            gpMfeConfig->nDropFlag =0;
            ms_dprintk(DRV_L1, "W = %d, H = %d, nCodecType = %d\n", (int)pappPVR_Info->width, (int)pappPVR_Info->height, (int)pappPVR_Info->nCodecType);
            gpMfeConfig->nCodecType = pappPVR_Info->nCodecType;
            gpMfeConfig->nDispWidth = pappPVR_Info->nDispWidth;
            gpMfeConfig->nDispHeight = pappPVR_Info->nDispHeight;
            gpMfeConfig->nBufWidth = pappPVR_Info->nBufWidth;
            gpMfeConfig->nBufHeight = pappPVR_Info->nBufHeight;
            gpMfeConfig->dram_base = pappPVR_Info->BitsBuffStart;
            gpMfeConfig->dram_size = pappPVR_Info->BitsBuffSize;
            gpMfeConfig->bInterlace = pappPVR_Info->bInterlace;
            gpMfeConfig->nPbetweenI = pappPVR_Info->nPbetweenI;
            gpMfeConfig->nBbetweenP = pappPVR_Info->nBbetweenP;
            gpMfeConfig->FrameRatex100 = pappPVR_Info->FrameRatex100;
            gpMfeConfig->nBitrate = pappPVR_Info->nBitrate;
            gpMfeConfig->nP_is_infinite = pappPVR_Info->nP_is_infinite;
            gpMfeConfig->ctxBufInfo.m_nOutBuffer = pappPVR_Info->nOutBuffer;
            gpMfeConfig->ctxBufInfo.m_OutBufferSize = pappPVR_Info->OutBufferSize;
            gpMfeConfig->enableReduceBW = pappPVR_Info->enableReduceBW;
            gpMfeConfig->enableFastMode = pappPVR_Info->enableFastMode;
            memcpy(&gpMfeConfig->MfeCropInfo, &pappPVR_Info->MfeCropInfo,sizeof(MFE_CROP_INFO_t));
#ifdef _MFE_M1_
            gpMfeConfig->m_bFrameMode = pappPVR_Info->m_bFrameMode;
            gpMfeConfig->m_bFDC_mode = pappPVR_Info->fdc_mode;
            gpMfeConfig->m_quality = pappPVR_Info->quality;
            memcpy(&gpMfeConfig->MfeAdvInfo,&pappPVR_Info->MfeAdvInfo,sizeof(MFE_ADV_INFO_t));
#else
            gpMfeConfig->m_bFrameMode = 1;
#endif
            if(pappPVR_Info->enableISR == 0)
                gpMfeConfig->PollingMode = 1;
            gpMfeConfig->ctxRateControl.m_nVPMbRow = MFE_ER_MBY;
            gpMfeConfig->ctxRateControl.m_nVPSize = MFE_ER_BS_TH;
            gpMfeConfig->MFEClk= pappPVR_Info->MFEClk;
            gpMfeConfig->TimeIncreamentRes = pappPVR_Info->TimeIncreamentRes;
            gpMfeConfig->VopTimeIncreament = pappPVR_Info->VopTimeIncreament;

            if(gpMfeConfig->nPbetweenI == -1){
                gpMfeConfig->nP_is_infinite = 1;
            }
#ifdef _MFE_M1_
            if(pappPVR_Info->enableVTRateControl)
            {
                gpMfeConfig->VTMode = 1;
                gpMfeConfig->VTRateCtrl.m_bDropFrameEnabled= 1;
            }
#endif
            gpMfeConfig->bColorFormat = pappPVR_Info->bColorFormat;
            if(pappPVR_Info->enableSecurityMode) {
                gpMfeConfig->SecurityMode = 1;
            }
            if(gpMfeConfig->bColorFormat==MFE_YUYV) {
                gpMfeConfig->g_nUseYUV422 = 1;
            } else if(gpMfeConfig->bColorFormat==MFE_YVYU) {
                gpMfeConfig->g_nUseYUV422 = 2;
            } else if(gpMfeConfig->bColorFormat==MFE_UYVY) {
                gpMfeConfig->g_nUseYUV422 = 3;
            } else if(gpMfeConfig->bColorFormat==MFE_VYUY) {
                gpMfeConfig->g_nUseYUV422 = 4;
            } else {
                gpMfeConfig->g_nUseYUV422 = 0;
            }

            if(pappPVR_Info->enableCABAC)
                gpMfeConfig->UseCABAC = 1;

#if defined(MFE_SUPPORT_TLB)
            gpMfeConfig->bEnableTLB = (pappPVR_Info->reserved1 & (MS_U32)MFE_ENABLE_TLB_FLAG) ? 1 : 0;
#endif
            MHAL_MFE_CreateRegMap(&gpMfeConfig->mfe_reg,&gpMfeConfig->mfe_reg1);
            MDrv_MFE_Init(gpMfeConfig);
        }
        break;
    case MFE_IOC_GET_QTABLE:
        if (gpMfeConfig != NULL && arg != 0) {
            memcpy((MS_S32*)arg,&gpMfeConfig->ctxJpgInfo.QTable[0][0],sizeof(MS_S32)*64);
            memcpy(((MS_S32*)arg)+64,&gpMfeConfig->ctxJpgInfo.QTable[1][0],sizeof(MS_S32)*64);
        }
        break;
    case MFE_IOC_SET_QUALITY:
        if (gpMfeConfig != NULL) {
            gpMfeConfig->m_quality = (MS_U32)arg;
#ifdef _MFE_M1_
            mfeJPE_Set_QTABLE(gpMfeConfig);
#endif
        }
        break;
    case MFE_IOC_ENC_ONEFRAME:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_ENC_ONEFRAME\n");
            MDrv_MFE_ENC_OneFrame(gpMfeConfig, (MEMMAP_CUR_t*)arg);
        }
        break;
    case MFE_IOC_GET_DISPORDER:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_GET_DISPORDER\n");
            MDrv_MFE_GetDispOrder(gpMfeConfig, &gpMfeConfig->GopInfo, (MS_U32*)arg);
        }
        break;
    case MFE_IOC_GETBITS:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_GETBITS\n");
            bits_info = (BITSFRAME_INFO *)arg;
            return MDrv_MFE_GetBits(gpMfeConfig,bits_info);
        }
        break;
    case MFE_IOC_SETBITRATE:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->nBitrate = pappPVR_Info->nBitrate;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SETBITRATE = %d\n", (int)gpMfeConfig->nBitrate);
        }
        break;
    case MFE_IOC_SET_COLOR_FORMAT:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->bColorFormat = pappPVR_Info->bColorFormat;
            if(gpMfeConfig->bColorFormat==MFE_YUYV) {
                gpMfeConfig->g_nUseYUV422 = 1;
            } else if(gpMfeConfig->bColorFormat==MFE_YVYU) {
                gpMfeConfig->g_nUseYUV422 = 2;
            } else if(gpMfeConfig->bColorFormat==MFE_UYVY) {
                gpMfeConfig->g_nUseYUV422 = 3;
            } else if(gpMfeConfig->bColorFormat==MFE_VYUY) {
                gpMfeConfig->g_nUseYUV422 = 4;
            } else {
                gpMfeConfig->g_nUseYUV422 = 0;
            }
            ms_dprintk(DRV_L2, "[IOCTL] MFE_IOC_SET_COLOR_FORMAT = %d\n", gpMfeConfig->bColorFormat);
        }
        break;
    case MFE_IOC_RESET_BITRATE:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->nBitrate = pappPVR_Info->nBitrate;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_BITRATE = %d\n", (int)gpMfeConfig->nBitrate);
            MfeDrvRateControlInit(gpMfeConfig);
        }
        break;
   case MFE_IOC_SET_ISR:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->PollingMode = pappPVR_Info->enableISR==1?0:1;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SET_ISR = %u\n", (unsigned int)gpMfeConfig->PollingMode);
        }
        break;
    case MFE_IOC_RESET_FRAMERATE:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->FrameRatex100 = pappPVR_Info->FrameRatex100;
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_FRAMERATE = %d\n", (int)gpMfeConfig->FrameRatex100);
            MfeDrvRateControlInit(gpMfeConfig);
        }
        break;
    case MFE_IOC_RESET_BITRATE_AND_FRAMERATE:
        if (gpMfeConfig != NULL && arg != 0) {
            pappPVR_Info = (PVR_Info *)arg;
            gpMfeConfig->FrameRatex100 = pappPVR_Info->FrameRatex100;
            gpMfeConfig->nBitrate = pappPVR_Info->nBitrate;
            MfeDrvRateControlInit(gpMfeConfig);
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_BITRATE = %d, FRAMERATE = %d\n",
                (int)gpMfeConfig->nBitrate, (int)gpMfeConfig->FrameRatex100);
        }
        break;
    case MFE_IOC_CLEAR_BITSBUF:
        if (gpMfeConfig != NULL) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_CLEAR_BITSBUF\n");
            MDrv_MFE_clear_bitsbuf(gpMfeConfig, arg);
        }
        break;
    case MFE_IOC_FINISH:
        if (gpMfeConfig != NULL) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_FINISH\n");
            MDrv_MFE_Finish(gpMfeConfig);
            MHAL_MFE_DelRegMap(&gpMfeConfig->mfe_reg,&gpMfeConfig->mfe_reg1);
        }
        break;
    case MFE_IOC_SET_FORCEDI:   // no implement
        if (gpMfeConfig != NULL) {
            ms_dprintk(DRV_L1, "[IOCTL] MDRV: MFE_IOC_SET_FORCEDI\n");
            gpMfeConfig->bForceIframe = TRUE;
        }
        break;
    case MFE_IOC_GET_DROPFLAG:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_GET_DROPFLAG = %u\n", (unsigned int)gpMfeConfig->nDropFlag);
            *((MS_U32 *)arg) = gpMfeConfig->nDropFlag;
        }
        break;
    case MFE_IOC_GET_SKIPFLAG:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_SKIPFLAG = %d\n", (int)rc_CheckSkippedFrame(&gpMfeConfig->VTRateCtrl));
            *((MS_U32 *)arg) = rc_CheckSkippedFrame(&gpMfeConfig->VTRateCtrl);
        }
        break;
    case MFE_IOC_SET_MIU_OFFSET:
        if (gpMfeConfig != NULL) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_SET_MIU_OFFSET\n");
            MDrv_MFE_SetMIUOffset(gpMfeConfig, arg);
        }
        break;
    case MFE_IOC_POWEROFF:
        if (arg != 0) {
            POWERON_t* tmp_Info;
            tmp_Info = (POWERON_t*)arg;
            //gpMfeConfig->MFEClk = tmp_Info->clock_level ;
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_POWEROFF, clock level= %d\n",tmp_Info->clock_level);
            MHal_MFE_PowerOff((MS_U32)tmp_Info->is_off,tmp_Info->clock_level);
        }
        break;
    case MFE_IOC_GET_VOL:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_GET_VOL\n");
            MDrv_MFE_GetConfigBytes(gpMfeConfig, (void*)arg);
        }
       break;
    case MFE_IOC_GET_SKIPSHORTHEADER:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_SKIPSHORTHEADER = %d\n", (int)rc_CheckSkippedFrame(&gpMfeConfig->VTRateCtrl));
            *((MS_U32 *)arg) = rc_CheckSkippedFrame(&gpMfeConfig->VTRateCtrl);
        }
        break;
    case MFE_IOC_GET_FRAME_TYPE:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_FRAME_TYPE = %u\n", (unsigned int)gpMfeConfig->vopPredType);
            *((MS_U32 *)arg) = gpMfeConfig->vopPredType;
        }
        break;
    case MFE_IOC_SET_RIU_BASE:
        if (arg != 0) {
            ms_dprintk(DRV_L3, "[IOCTL] MFE_IOC_SET_RIU_BASE 0x%tx\n", (ptrdiff_t)arg);
            MDrv_MFE_SetOSRegBase(gpMfeConfig,arg);
        }
        break;
    case MFE_IOC_RESET_SPSPPS:
        if (gpMfeConfig != NULL) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_RESET_SPSPPS\n");
            if(gpMfeConfig->nCodecType == REG_ENC_MODE_H264)
                GenerateSPSPPS(gpMfeConfig);
            else
                ms_dprintk(DRV_L4, "[IOCTL]MFE_IOC_RESET_SPSPPS only support H264.\n");
        }
        break;
    case MFE_IOC_GET_HWCAP:
        if (arg != 0) {
            MFE_HwCapV0 *mfe_hwcap;
            mfe_hwcap = (MFE_HwCapV0 *)arg;
            MHal_MFE_GetHWCap( &mfe_hwcap->u16MaxEncodeWidth, &mfe_hwcap->u16MaxEncodeHeight);
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_GET_HWCAP\n");
        }
        break;
    case MFE_IOC_SET_VUI:
        if (gpMfeConfig != NULL && arg != 0) {
            ms_dprintk(DRV_L1, "[IOCTL] MFE_IOC_SET_VUI\n");

            VUI_Info* vui_info = (VUI_Info *)arg;
            gpMfeConfig->setLevel = vui_info->level;
            gpMfeConfig->setVUI_aspect_ratio_info_present_flag = vui_info->aspect_ratio_info_present_flag;
            gpMfeConfig->setVUI_aspect_ratio_idc = vui_info->aspect_ratio_idc;
            gpMfeConfig->setVUI_sar_width = vui_info->sar_width;
            gpMfeConfig->setVUI_sar_height = vui_info->sar_height;
            gpMfeConfig->ctxH264Info.LevelIDC = gpMfeConfig->setLevel;
            GenerateSPSPPS(gpMfeConfig);
        }
        break;
    default:
        ms_dprintk(DRV_L4, "[IOCTL] MFE receive non-defined Ioctl\n");
        break;
    }

    return 0;
}


