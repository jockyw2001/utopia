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
/// file    drvHVD_EX.c
/// @brief  HVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "drvHVD_Common.h"
#include "drvHVD_EX.h"
#include "drvHVD_def.h"
#include "HVD_EX_Common.h"

#include "fwHVD_if.h"
#include "halVPU_EX.h"
#include "halHVD_EX.h"
#include "drvSYS.h"

#if HVD_ENABLE_AUTO_SET_REG_BASE
#include "drvMMIO.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Functions Prototype
//-------------------------------------------------------------------------------------------------
#ifndef CONFIG_MBOOT  //Add For GPL (content protection)

#define IPAUTH_VIDEO_H264           (27)
#define IPAUTH_VIDEO_RM             (28)
#define IPAUTH_VIDEO_AVS            (33)
#define IPAUTH_VIDEO_MVC            (40)
#define IPAUTH_VIDEO_VP8            (61)
#define IPAUTH_VIDEO_HEVC           (75)
#define IPAUTH_VIDEO_VP9            (91)
MS_BOOL bModeSelectHD = FALSE;
static MS_BOOL bAutoIpcheck = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif
#define HVD_CODEC_INFO_MASK 0x0000000f

MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType)
{
    MS_BOOL bIPAUTHReady = FALSE;
    VDEC_EX_CODEC_CAP_INFO pCodecCapInfo;
#if !defined(SUPPORT_X_MODEL_FEATURE)
    u32CodecType = (u32CodecType & HVD_CODEC_INFO_MASK);
    bAutoIpcheck = FALSE;
    switch (u32CodecType)
    {
        case HVD_INIT_HW_AVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_H264) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_AVS:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_AVS) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_RM:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_RM) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_MVC:  /// SUPPORT_MVC
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MVC) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_VP8:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP8) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_HEVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_HEVC) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
         case HVD_INIT_HW_VP9:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP9) == TRUE && HAL_VPU_EX_GetCodecCapInfo( E_HVD_EX_CODEC_TYPE_VP9, &pCodecCapInfo) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        default:
            bIPAUTHReady = FALSE;
            break;
    }

    if(bIPAUTHReady == FALSE)
    {
        HVD_EX_MSG_ERR("[%s]User using invaild key !!!\n",__FUNCTION__);
    }
#endif
    return bIPAUTHReady;
}
#endif

#ifdef VDEC3
#define HVD_MAX_STREAMS         16
#else
#define HVD_MAX_STREAMS         2
#endif

#define VPU_MAIL_BOX_SEND_FAIL 1
#define CODEC_KEY_COMPARE_TIMEOUT 2
extern MS_U8 u8CodecCompare[HVD_MAX_STREAMS];
extern HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);
#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)

#if defined(CHIP_KANO)
MS_BOOL _MDrv_HVDSendVPUCMD(MS_U32 u32Id,MS_U32 u32CMD,MS_U32 u32Arg)
{
    MS_BOOL bRet = TRUE;
    MS_U8  u8Idx = _VDEC_GET_IDX(u32Id);

    HAL_VPU_EX_Mutex_Lock();
    if(HAL_HVD_EX_SetCmd(u32Id, u32CMD, u32Arg) != E_HVD_RETURN_SUCCESS)
    {
        bRet = FALSE;
    }
    HAL_VPU_EX_Mutex_UnLock();

    if(bRet == FALSE)
    {
        HVD_PRINT("[Info]Case1\n");
        u8CodecCompare[u8Idx] = VPU_MAIL_BOX_SEND_FAIL;
    }

    return bRet;
}

MS_BOOL _MDrv_HVDSendCodecKey(MS_U32 u32Id,MS_U8 u8Decoder,MS_U16 u16CodecKey)
{
    MS_U8 u8Idx = HAL_VPU_EX_GetOffsetIdx(u32Id);
    MS_U32 u32Timeout = 0;
    struct _ctl_info *ctl_ptr = (struct _ctl_info *) MsOS_PA2KSEG1(HAL_VPU_EX_GetFWCodeAddr(u32Id) + CTL_INFO_ADDR);

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_CMD1,(((VDEC_TAG<<24)&0xff000000))|(((u8Decoder<<16)&0xff0000))|(u16CodecKey&0xffff)) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_MASK,0) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    MsOS_ReadMemory();
    u32Timeout = HVD_GetSysTime_ms() + 500; //timeout 500 ms
    while (CTL_TASK_CMD != ctl_ptr->task_statue[u8Idx])
    {
        if (HVD_GetSysTime_ms() > u32Timeout)
        {
            HVD_PRINT("[Info]Case2\n");
            u8CodecCompare[u8Idx] = CODEC_KEY_COMPARE_TIMEOUT;
            HAL_HVD_EX_DeInit(u32Id);
            return FALSE;
        }
        MsOS_ReadMemory();
    }

    return TRUE;
}
#endif

HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id,MS_U32 u32ModeFlag)
{
	HVD_EX_Result eRst = E_HVD_EX_FAIL;
#ifndef CONFIG_MBOOT  //Add For GPL (content protection)
	if(bAutoIpcheck)
	{
		eRst = _HVD_EX_Rst(u32Id ,FALSE);

#if defined(CHIP_KANO)
        MS_U32 u32CodecType = u32ModeFlag & HVD_CODEC_INFO_MASK;
        if(eRst == E_HVD_EX_OK)
        {
            if(u32CodecType != HVD_INIT_HW_MJPEG)
            {
                if(_MDrv_HVDSendCodecKey(u32Id,HVD_DECODER,u32CodecType) == FALSE)
                {
                    eRst = E_HVD_EX_FAIL;
                }
            }
        }
#endif
    }
    else
	{
		HVD_PRINT("[%s] .. STOP and Contact VDEC team!!!\n",__FUNCTION__);
	}
#else
    eRst = _HVD_EX_Rst(u32Id ,FALSE);
#endif

	return eRst;
}

#if SUPPORT_EVD
static MS_U16 _HVD_EX_DemuxTS_GetBits(const MS_U8 *buffer, MS_U16 totbitoffset, MS_U32 *info, MS_U16 bufferLen, MS_U16 numbits)
{
    MS_U32 inf;
    MS_U8 bitoffset  = (totbitoffset & 0x07); // bit from start of byte
    MS_U16 byteoffset = (totbitoffset >> 3);       // byte from start of buffer
    MS_U16 bitcounter = numbits;
    const MS_U8 *curbyte;
    if ((byteoffset) + ((numbits + bitoffset)>> 3)  > bufferLen)
        return 0;

    curbyte = &(buffer[byteoffset]);
    bitoffset = 7 - bitoffset;
    inf=0;

    while (numbits--)
    {
        inf <<=1;
        inf |= ((*curbyte)>> (bitoffset--)) & 0x01;
        curbyte   += (bitoffset >> 3) & 0x01;
        //curbyte   -= (bitoffset >> 3);
        bitoffset &= 0x07;
        //curbyte   += (bitoffset == 7);
    }

    *info = inf;
    return bitcounter;           // return absolute offset in bit from start of frame

}

static MS_U8 _HVD_EX_DemuxTS_PeekOneByte(MS_VIRT u32NonCacheAddr)
{
   return *((MS_U8 *)(u32NonCacheAddr));
}

static MS_U16 _HVD_EX_DemuxTS_EBSPtoRBSP(MS_U32 u32SrcPos, MS_U8 *pu8DestBuf, MS_U16 u16end_bytepos, MS_U16 u16begin_bytepos)
{
    MS_U16 i, j, count;
    MS_VIRT tmpPos;
    count = 0;

    if(u16end_bytepos < u16begin_bytepos)
    {
        HVD_PRINT("_HVD_EX_DemuxTS_EBSPtoRBSP failed~ u16end_bytepos < u16begin_bytepos\n");
        return 0;
    }

    j = 0;
    for(i = u16begin_bytepos; i < u16end_bytepos; i++)
    {
        tmpPos = i+u32SrcPos;
        /// Check 0x000000, 0x000001 or 0x000002, those shall not occur at any byte-aligned.
        /// ISO 14496-10 Page:49 , 7.4.1 emulation_prevention_three_byte information.
        if((count == 2) && (_HVD_EX_DemuxTS_PeekOneByte(tmpPos) < 0x03))
            return j;
        if((count == 2) && (_HVD_EX_DemuxTS_PeekOneByte(tmpPos) == 0x03))
        {
            if((i < u16end_bytepos-1) && (_HVD_EX_DemuxTS_PeekOneByte(tmpPos+1) > 0x03))
                return j;
            if(i == u16end_bytepos-1)
                return j;

            i++;
            tmpPos++;
            count = 0;
        }
        pu8DestBuf[j] = _HVD_EX_DemuxTS_PeekOneByte(tmpPos);
        if(_HVD_EX_DemuxTS_PeekOneByte(tmpPos) == 0x00)
            count++;
        else
            count = 0;
        j++;

        if ( j > H265_RBSP_BUF_LEN )
        {
            HVD_PRINT("_HVD_EX_DemuxTS_EBSPtoRBSP failed~ byte is too much than 1024!!\n");
            return H265_RBSP_BUF_LEN;
        }
    }
    return j;
}

static MS_U32 _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(MS_U16 u16ReadBits, const MS_U8* buffer, MS_U16 bufferLen, MS_U16* u16bitLen, MS_BOOL bUnsige)
{
    MS_U16 u16readbitLen = 0;
    MS_U32 u32codeword = 0;
    u16readbitLen = _HVD_EX_DemuxTS_GetBits(buffer, *u16bitLen, &u32codeword, bufferLen, u16ReadBits);
    HVD_EX_MSG_DBG("BitLen=%d, Readbits=%d, Codeword=%d.\n",*u16bitLen,u16readbitLen,u32codeword);
    *u16bitLen += u16readbitLen;
    if(!bUnsige)
    {
        /// u32codeword change to s32 format
    }
    return u32codeword;
}

static MS_U16 _HVD_EX_DemuxTS_GetVLCSymbol(MS_U8 *buffer, MS_U16 totbitoffset, MS_U32 *info, MS_U16 bufferLen)
{
    MS_U32 infw;
    // byte from start of buffer
    MS_U16 byte_off = (totbitoffset >> 3);
    // bit from start of byte
    MS_U8  bit_off = (MS_U8)(7 - (totbitoffset & 0x07));
    MS_U16  bit_cnt = 1;
    MS_U16  length = 0;
    MS_U8 *current_byte = &(buffer[byte_off]);
    // control bit for current bit posision
    MS_U8  control_bit  = ((*current_byte) >> (bit_off)) & 0x01;

    // Finding leading 1 bit
    while (control_bit == 0)
    {
        bit_off--;
        bit_off &= 0x07;
        if (bit_off == 7)
        {
            current_byte += 1;
            byte_off += 1;
        }
        control_bit = ((*current_byte) >> (bit_off)) & 0x01;
        length++;
        bit_cnt++;
    }

    if (byte_off + ((length + 7) >> 3) > bufferLen)
        return 0;

    // make infoword
    // shortest possible code is 1, then info is always 0
    infw = 0;
    while (length--)
    {
        bit_off--;
        bit_off &= 0x07;
        if (bit_off == 7)
            current_byte += 1;
        infw <<= 1;
        infw |= ((*current_byte) >> (bit_off)) & 0x01;
        bit_cnt++;
    }

    *info = infw;
    // return absolute offset in bit from start of frame
    return bit_cnt;
}

static MS_U32 _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(MS_U8* buffer, MS_U16 bufferLen, MS_U16* u16bitLen, MS_BOOL bUnsige)
{
    MS_U16 u16readbitLen = 0;
    MS_U32 u32codeword = 0;
    u16readbitLen = _HVD_EX_DemuxTS_GetVLCSymbol(buffer, *u16bitLen, &u32codeword, bufferLen);
    u32codeword = (1 << (u16readbitLen >> 1)) + u32codeword - 1;
    HVD_EX_MSG_DBG("BitLen=%d, Readbits=%d, Codeword=%d.\n",*u16bitLen,u16readbitLen,u32codeword);
    *u16bitLen += u16readbitLen;
    if(!bUnsige)
    {
        MS_U32 u32codewordtmp = u32codeword;
        u32codeword = (u32codewordtmp + 1) >> 1;
        if((u32codewordtmp & 0x01) == 0)
            u32codeword = (-1*u32codeword);
    }
    return u32codeword;
}

HVD_EX_Result _HVD_EX_DemuxTS_ParseHEVCSPS(MS_VIRT u32DataPos, MS_U16 u16DataLen, MS_U32 *pMemUsageSize)
{
    MS_U8 pu8tmp[H265_RBSP_BUF_LEN] = {0};
    MS_U16 u16Width = 0;
    MS_U16 u16Height = 0;

#if 0
{
    MS_U32 i = 0;
    MS_U8 *__pu8tmp = (MS_U8 *)u32DataPos;
    HVD_ERR("DemuxTS_ParseHEVCSPS size = %d", u16DataLen);
    for (i = 0; i < u16DataLen; i++)
    {
        if(i%32 == 0)
            HVD_ERR("\n");
        HVD_ERR("%02x ", *(__pu8tmp + i));
    }
    HVD_ERR("\n");
}
#endif

    while (u16DataLen >= 5)
    {
        if ( (_HVD_EX_DemuxTS_PeekOneByte(u32DataPos) == 0x0
           && _HVD_EX_DemuxTS_PeekOneByte(u32DataPos+1) == 0x0
           && _HVD_EX_DemuxTS_PeekOneByte(u32DataPos+2) == 0x1
           && ((_HVD_EX_DemuxTS_PeekOneByte(u32DataPos+3)>>1) & 0x3F) == 0x21)
        && u16DataLen > 16 )
        {
            #define MAX_SUB_LAYER_MINUS1 6
            MS_U32 sub_layer_profile_present_flag[MAX_SUB_LAYER_MINUS1];
            MS_U32 sub_layer_level_present_flag[MAX_SUB_LAYER_MINUS1];
            MS_U16 u16BitLen = 0;
            MS_U32 sps_max_sub_layers_minus1;
            //MS_U32 general_level_idc;
            MS_U32 chroma_format_idc;
            MS_U32 pic_width_in_luma_samples, pic_height_in_luma_samples;
            MS_U32 conformance_window_flag;
            MS_U32 bit_depth_luma_minus8, bit_depth_chroma_minus8;
            MS_U32 i, j;
            MS_U16 _u16DataLen = 0;

            /// EBSP to RBSP
            _u16DataLen = _HVD_EX_DemuxTS_EBSPtoRBSP(u32DataPos+5, pu8tmp, (u16DataLen - 5), 0);
            HVD_EX_MSG_DBG("[DemuxTS] valid data len = %d\n", _u16DataLen);
            if (_u16DataLen == 0)
            {
                HVD_PRINT("_HVD_EX_DemuxTS_ParseHEVCSPS wrong position\n");
                return E_HVD_EX_FAIL;
            }
#if 0
{
    MS_U32 k = 0;
    for (k = 0; k < _u16DataLen; k++)
    {
        if(k%32 == 0)
            HVD_PRINT("\n");
        HVD_PRINT("%02x ", pu8tmp[k]);
    }
    HVD_PRINT("\n");
}
#endif
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(4,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            sps_max_sub_layers_minus1 = _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(3,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);

            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(2,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(5,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            for (i = 0; i < 32; i++)
            {
                _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            }
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(44,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(8,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            for (i = 0; i < sps_max_sub_layers_minus1; i++)
            {
                sub_layer_profile_present_flag[i] = _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                sub_layer_level_present_flag[i] = _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            }

            if (sps_max_sub_layers_minus1 > 0)
            {
                for (i = sps_max_sub_layers_minus1; i < 8; i++)
                {
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(2,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                }
            }

            for (i = 0; i < sps_max_sub_layers_minus1; i++)
            {
                if (sub_layer_profile_present_flag[i])
                {
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(2,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(5,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    for (j = 0; j < 32; j++)
                    {
                        _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    }
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(44,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                }

                if (sub_layer_level_present_flag[i])
                {
                    _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(8,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                }
            }

            _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            chroma_format_idc = _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);

            if (chroma_format_idc == 3)
            {
                _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            }

            pic_width_in_luma_samples = _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            pic_height_in_luma_samples = _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            conformance_window_flag = _HVD_EX_DemuxTS_ReadSyntaxElement_FLC(1,pu8tmp,_u16DataLen,&u16BitLen,TRUE);

            if (conformance_window_flag)
            {
                _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
                _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            }

            bit_depth_luma_minus8 = _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            bit_depth_chroma_minus8 = _HVD_EX_DemuxTS_ReadSyntaxElement_VLC(pu8tmp,_u16DataLen,&u16BitLen,TRUE);
            ///pProgramList->enID = E_VDP_CODEC_ID_HEVC;
            u16Width = (MS_U16)(MEMALIGN(pic_width_in_luma_samples, 32));
            u16Height = (MS_U16)(MEMALIGN(pic_height_in_luma_samples, 32));
            ///pProgramList->u8Level_idc = (MS_U8) (general_level_idc / 30);
            ///pProgramList->u8Profile_idc = (bit_depth_luma_minus8 || bit_depth_chroma_minus8); // used for 10bit
            ///pProgramList->u8DecFrameNum = 0; // not used
            ///pProgramList->u8NumRefFrames = 0; // not used
            ///pProgramList->u8FrameMbsOnlyFlag = 0; // not used
            ///pProgramList->u8Direct8x8InferenceFlag = 0; // not used

            {
                #define MaxLumaPS_Level_4x    (0x220000) //level4.0
                #define MaxLumaPS_Level_5x    (0x880000) //level5.0
                #define maxDpbPicBuf (6)
                MS_U32 MaxDpbSize;
                //MS_U8 u8Level_idc = (MS_U8) (general_level_idc / 30);
                MS_U8 u8Profile_idc = (bit_depth_luma_minus8 || bit_depth_chroma_minus8); // used for 10bit
                // MS_U32 MaxLumaPS = (u8Level_idc >= 5 || u8Level_idc == 0) ? MaxLumaPS_Level_5x : MaxLumaPS_Level_4x;
                //MS_U32 MaxLumaPS = (u8Level_idc >= 5) ? MaxLumaPS_Level_5x : MaxLumaPS_Level_4x;
                MS_U32 MaxLumaPS = ((u16Width > 2048) || (u16Height > 1088)) ? MaxLumaPS_Level_5x : MaxLumaPS_Level_4x;
                MS_U32 u32FrameSize = u16Height * u16Width;
                MS_U32 PicSizeInSamplesY = u32FrameSize;

                if ( PicSizeInSamplesY <= ( MaxLumaPS >> 2 ) )
                    MaxDpbSize = HVD_MIN( 4 * maxDpbPicBuf, 16 );
                else if ( PicSizeInSamplesY <= ( MaxLumaPS >> 1 ) )
                    MaxDpbSize = HVD_MIN( 2 * maxDpbPicBuf, 16 );
                else if ( PicSizeInSamplesY <= ( ( 3 * MaxLumaPS ) >> 2 ) )
                    MaxDpbSize = HVD_MIN( (4 * maxDpbPicBuf) / 3, 16 );
                else
                    MaxDpbSize = maxDpbPicBuf;

                if (u8Profile_idc) // 10bit case
                    *pMemUsageSize = MaxDpbSize * u32FrameSize * 3 / 2 * 5 / 4; /// 1.5 * 1.25
                else
                    *pMemUsageSize = MaxDpbSize * u32FrameSize * 3 / 2;

                //consider hw buffer
                *pMemUsageSize += HAL_HVD_EX_GetEVDHWBuffer();

                HVD_EX_MSG_INF("[DemuxTS] EVD Memory usage = %d\n",
                    (MS_U32)*pMemUsageSize);
                HVD_EX_MSG_INF("[DemuxTS]Prog#X:HEVC,Width X Height = %dX%d.\n",
                    u16Width, u16Height);
            }

            return E_HVD_EX_OK;
        }
        else
        {
            u32DataPos++;
            u16DataLen--;
        }
    }

    return E_HVD_EX_FAIL;
}
#endif //SUPPORT_EVD

#endif

