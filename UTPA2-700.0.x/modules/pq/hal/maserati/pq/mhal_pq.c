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
#define MHAL_PQ_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvPQ_Define.h"
#include "Maserati_Main.h"              // table config parameter
#include "Maserati_Sub.h"              // table config parameter
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U8 _gu8AnalogCfd[QM_INPUTTYPE_NUM_Main][6] =
{
#if (PQ_QM_CVBS)
    {9, 2, 0, 6, 6, 6}, // QM_RF_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_RF_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_RF_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_RF_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_RF_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_RF_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_RF_SECAM_Main
    {9, 2, 0, 6, 6, 6}, // QM_VIF_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_VIF_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_VIF_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_VIF_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_VIF_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_VIF_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_VIF_SECAM_Main
    {9, 2, 0, 6, 6, 6}, // QM_SV_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_SV_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SV_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_SV_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_SV_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SV_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_SV_SECAM_Main
    {9, 2, 0, 6, 6, 6}, // QM_AV_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_AV_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_AV_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_AV_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_AV_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_AV_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_AV_SECAM_Main
    {9, 2, 0, 6, 6, 6}, // QM_SCART_AV_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_SCART_AV_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_AV_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_AV_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_AV_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_AV_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_AV_SECAM_Main
    {9, 2, 0, 6, 6, 6}, // QM_SCART_SV_NTSC_44_Main
    {9, 2, 0, 6, 6, 6}, // QM_SCART_SV_NTSC_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_SV_PAL_BGHI_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_SV_PAL_60_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_SV_PAL_M_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_SV_PAL_N_Main
    {8, 2, 0, 5, 6, 5}, // QM_SCART_SV_SECAM_Main
    {2, 0, 1, 6, 6, 6}, // QM_SCART_RGB_NTSC_Main
    {1, 0, 1, 5, 6, 5}, // QM_SCART_RGB_PAL_Main(43)
#endif
#if (PQ_QM_YPBPR)
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_480i_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_576i_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_480p_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_576p_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_720p_24hz_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_720p_50hz_Main
    {8, 2, 0, 5, 6, 5}, // QM_YPbPr_720p_60hz_Main
    {10, 2, 0, 1, 1, 1}, // QM_YPbPr_1080i_50hz_Main
    {10, 2, 0, 1, 1, 1}, // QM_YPbPr_1080i_60hz_Main
    {10, 2, 0, 1, 1, 1}, // QM_YPbPr_1080p_24hz_Main
    {10, 2, 0, 1, 1, 1}, // QM_YPbPr_1080p_50hz_Main
    {10, 2, 0, 1, 1, 1}, // QM_YPbPr_1080p_60hz_Main(55)
#endif
#if (PQ_QM_HMDI)
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_480i_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_576i_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_480p_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_576p_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_720p_24hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_720p_50hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_720p_60hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_FP_720p_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_1080i_50hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_1080i_60hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_1080p_24hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_1080p_60hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_1080p_50hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_FP_1080i_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_FP_1080p_Main
    {7, 2, 0, 2, 2, 2}, // QM_4K2K_24Hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_4K2K_30Hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_4K2K_60Hz_Main(73)
#endif
#if (PQ_QM_HDMI_PC)
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hup_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hup_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hup_Vno_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hdown_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hdown_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hdown_Vno_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hno_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hno_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hno_Vno_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_444_PC_Hno_Vno_4K_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hup_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hup_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hup_Vno_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hdown_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hdown_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hdown_Vno_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hno_Vup_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hno_Vdown_Main
    {7, 2, 0, 2, 2, 2}, // QM_HDMI_422_PC_Hno_Vno_Main(92)
#endif
#if (PQ_QM_PC)
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main
    {5, 0, 1, 1, 13, 5}, // QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_4K_Main(102)
#endif
#if (PQ_QM_DTV)
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480i_352x480_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480i_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_576i_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480p_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_576p_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_24hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_50hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_60hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080i_50hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080i_60hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_24hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_50hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_60hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_4K2K_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_4K2K_60Hz_MPEG2_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480i_352x480_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480i_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_576i_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_480p_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_576p_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_24hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_50hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_720p_60hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080i_50hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080i_60hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_24hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_50hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_1080p_60hz_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_4K2K_H264_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_4K2K_60Hz_H264_Main(132)
#endif
#if (PQ_QM_MM_VIDEO)
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_SD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_SD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_SD_progressive_24hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_HD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_HD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_HD_progressive_24hz_Main(138)
#endif
#if (PQ_QM_MM_PHOTO)
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_photo_SD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_photo_HD_progressive_Main(140)
#endif
#if (PQ_QM_MM_VIDEO)
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_SD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_SD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_SD_progressive_24hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_HD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_HD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_online_HD_progressive_24hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_MM_FHD_I_DS_Main
    {7, 2, 0, 2, 2, 2}, // QM_MM_FHD_P_DS_Main
    {7, 2, 0, 2, 2, 2}, // QM_MM_4K2K_DS_Main
    {7, 2, 0, 2, 2, 2}, // QM_MM_4K2K_Main(150)
#endif
#if (PQ_QM_MM_PHOTO)
    {7, 2, 0, 2, 2, 2}, // QM_MM_4K2K_Photo_Main(151)
#endif
#if (PQ_QM_MM_VIDEO)
    {7, 2, 0, 2, 2, 2}, // QM_MM_4K2K_60Hz_Main
    {7, 2, 0, 2, 2, 2}, // QM_MM_non_4K2K_Main
#endif
#if (PQ_QM_DTV)
    {7, 2, 0, 2, 2, 2}, // QM_DTV_iFrame_SD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_iFrame_SD_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_iFrame_HD_interlace_Main
    {7, 2, 0, 2, 2, 2}, // QM_DTV_iFrame_HD_progressive_Main
#endif
#if (PQ_QM_MM_VIDEO)
    {7, 2, 0, 2, 2, 2}, // QM_Multimedia_video_4K2K_FS_progressive_Main
    {7, 2, 0, 2, 2, 2}, // QM_NetWork_4K2K_DS_Main
#endif
#if (PQ_QM_HMDI)
    {7, 2, 0, 2, 2, 2}, // QM_I_Mode_Unlock_Main
    {7, 2, 0, 2, 2, 2}, // QM_P_Mode_Unlock_Main
#endif
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 PQ_RIU_BASE;

// Put this function here because hwreg_utility2 only for hal.
void Hal_PQ_init_riu_base(void *pInstance,MS_U32 u32riu_base)
{
    PQ_RIU_BASE = u32riu_base;
}


MS_U8 Hal_PQ_get_sync_flag(void *pInstance,MS_BOOL bMainWin)
{
    MS_U16 u16val;
    MS_U8 u8SyncFlag;
    if(bMainWin)
        u16val = MApi_XC_R2BYTE(REG_SC_BK01_1E_L) & 0x00FF;
    else
        u16val = MApi_XC_R2BYTE(REG_SC_BK03_1E_L) & 0x00FF;

    u8SyncFlag = u16val;
    return u8SyncFlag;
}


MS_U8 Hal_PQ_get_input_vsync_value(void *pInstance,MS_BOOL bMainWin)
{
    return (Hal_PQ_get_sync_flag(pInstance,bMainWin) & 0x04) ? 1:0;
}

MS_U8 Hal_PQ_get_output_vsync_value(void *pInstance,MS_BOOL bMainWin)
{
    return (Hal_PQ_get_sync_flag(pInstance,bMainWin) & 0x01) ? 1 : 0;
}

MS_U8 Hal_PQ_get_input_vsync_polarity(void *pInstance,MS_BOOL bMainWin)
{
    if(bMainWin)
        return (MApi_XC_R2BYTE(REG_SC_BK01_1E_L) & 0x100) ? 1:0;
    else
        return (MApi_XC_R2BYTE(REG_SC_BK03_1E_L) & 0x100) ? 1:0;
}

void Hal_PQ_set_memfmt_doublebuffer(void *pInstance,MS_BOOL bEn)
{
    // BK12_40[0] : OP double buffer
    // BK12_40[1] : F2 IP double buffer
    // BK12_40[2] : F1 IP double buffer
    // monaco hw issue op double buff can not open
    MApi_XC_W2BYTEMSK(REG_SC_BK12_40_L, bEn ? 0x06 : 0x00, 0x0007);
}

void Hal_PQ_set_mem_fmt(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK12_02_L, u16val, u16Mask);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK12_42_L, u16val, u16Mask);
}

void Hal_PQ_set_mem_fmt_en(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK12_02_L, u16val, u16Mask);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK12_42_L, u16val, u16Mask);
}

void Hal_PQ_set_420upSample(void *pInstance,MS_U16 u16value)
{
    MApi_XC_W2BYTE(REG_SC_BK21_76_L, u16value);
}

void Hal_PQ_set_force_y_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_78_L, u16value, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_78_L, u16value, 0xFF00);
}

MS_U8 Hal_PQ_get_force_y_motion(void *pInstance,MS_BOOL bMainWin)
{
    MS_U8 u8val;

    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_78_L) & 0x00FF);
    else
        u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK22_78_L) & 0xFF00)>>8);

    return u8val;
}

void Hal_PQ_set_force_c_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_79_L, u16value, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_79_L, u16value, 0xFF00);
}

MS_U8 Hal_PQ_get_force_c_motion(void *pInstance,MS_BOOL bMainWin)
{
    MS_U8 u8val;

    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_79_L) & 0x00FF);
    else
        u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK22_79_L) & 0xFF00)>>8);
    return u8val;
}

void Hal_PQ_set_dipf_temporal(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTE(REG_SC_BK22_14_L, u16val);
    else
        MApi_XC_W2BYTE(REG_SC_BK22_44_L, u16val);
}

MS_U16 Hal_PQ_get_dipf_temporal(void *pInstance,MS_BOOL bMainWin)
{
    if(bMainWin)
        return MApi_XC_R2BYTE(REG_SC_BK22_14_L);
    else
        return MApi_XC_R2BYTE(REG_SC_BK22_44_L);
}

void Hal_PQ_set_dipf_spatial(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_15_L, u16val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_45_L, u16val, 0x00FF);
}

MS_U8 Hal_PQ_get_dipf_spatial(void *pInstance,MS_BOOL bMainWin)
{
    if(bMainWin)
        return (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_15_L) & 0x00FF);
    else
        return (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_45_L) & 0x00FF);
}

void Hal_PQ_set_vsp_sram_filter(void *pInstance,MS_U8 u8vale)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, ((MS_U16)u8vale)<<8, 0xFF00);
}

MS_U8 Hal_PQ_get_vsp_sram_filter(void *pInstance)
{
    MS_U8 u8val;
    u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK23_0B_L) & 0xFF00) >> 8);
    return u8val;
}

void Hal_PQ_set_dnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK06_21_L, u8val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK06_01_L, u8val, 0x00FF);
}

MS_U8 Hal_PQ_get_dnr(void *pInstance,MS_BOOL bMainWin)
{
    MS_U8 u8val;
    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_21_L) & 0x00FF);
    else
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_01_L) & 0x00FF);
    return u8val;
}


void Hal_PQ_set_presnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK06_22_L, u8val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK06_02_L, u8val, 0x00FF);
}

MS_U8 Hal_PQ_get_presnr(void *pInstance,MS_BOOL bMainWin)
{
    MS_U8 u8val;
    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_22_L) & 0x00FF);
    else
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_02_L) & 0x00FF);

    return u8val;
}

void Hal_PQ_set_film(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, u16val, 0xC800);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, u16val, 0x0700);
}

MS_U8 Hal_PQ_get_film(void *pInstance,MS_BOOL bMainWin)
{
    MS_U8 u8val;
    u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0xFF00) >> 8);
    if(bMainWin)
        u8val &= 0xC8;
    else
        u8val &= 0x07;
    return u8val;
}

void Hal_PQ_set_c_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[8];
    MS_U8* u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_C_SRAM1 &&
       u8sramtype != SC_FILTER_C_SRAM2 &&
       u8sramtype != SC_FILTER_C_SRAM3 &&
       u8sramtype != SC_FILTER_C_SRAM4)
    {
        printf("Unknown c sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U8*)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0002, 0x0002); // enable c_sram_rw

    for(i=0; i<64; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100);
        j=i*5;

        if(u8sramtype == SC_FILTER_C_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, i, 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (i|0x40), 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (i|0x180), 0x01FF);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (i|0x1C0), 0x01FF);

        for ( x=0;x<5;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, u8Ramcode[4], 0x00FF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);

}

void Hal_PQ_set_y_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
#if 0
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[10];
    MS_U8* u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_Y_SRAM1 &&
       u8sramtype != SC_FILTER_Y_SRAM2 &&
       u8sramtype != SC_FILTER_Y_SRAM3 &&
       u8sramtype != SC_FILTER_Y_SRAM4)
    {
        printf("Unknown y sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U8*)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0001, 0x0001); // enable y_sram_rw
    for ( i=0; i<128; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100);
        j=i*10;

        // address

        if(u8sramtype == SC_FILTER_Y_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0000|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0080|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0100|i), 0x01FF);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0180|i), 0x01FF);

        for ( x=0;x<8;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        for ( x=8;x<10;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, (((MS_U16)u8Ramcode[5])<<8|(MS_U16)u8Ramcode[4]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_49_L, (((MS_U16)u8Ramcode[7])<<8|(MS_U16)u8Ramcode[6]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_4A_L, (((MS_U16)u8Ramcode[9])<<8|(MS_U16)u8Ramcode[8]), 0xFFFF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);
#else
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[10];
    MS_U8* u32Addr;

    if(u8sramtype != SC_FILTER_Y_SRAM1 &&
       u8sramtype != SC_FILTER_Y_SRAM2 &&
       u8sramtype != SC_FILTER_Y_SRAM3 &&
       u8sramtype != SC_FILTER_Y_SRAM4)
    {
        printf("Unknown y sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U8 *)pData;
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0001, 0x0001); // enable y_sram_rw
    for ( i=0; i<128; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100);
            j=i*8;
        // address

        if(u8sramtype == SC_FILTER_Y_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0000|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0080|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0100|i), 0x01FF);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0180|i), 0x01FF);

        for ( x=0;x<8;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, (((MS_U16)u8Ramcode[5])<<8|(MS_U16)u8Ramcode[4]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_49_L, (((MS_U16)u8Ramcode[7])<<8|(MS_U16)u8Ramcode[6]), 0xFFFF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x1000, 0x1000);
#endif
}

void Hal_PQ_set_c_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[8];
    MS_U8* u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_C_SRAM1 &&
       u8sramtype != SC_FILTER_C_SRAM2 &&
       u8sramtype != SC_FILTER_C_SRAM3 &&
       u8sramtype != SC_FILTER_C_SRAM4)
    {
        printf("Unknown c sram type %u\n", u8sramtype);
        return;
    }
    u32Addr = (MS_U8*)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK80_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK80_03_L, 0x0000, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x0002, 0x0002); // enable c_sram_rw

    for(i=0; i<64; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BKA3_41_L) & 0x0100);
        j=i*5;

        if(u8sramtype == SC_FILTER_C_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, i, 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (i|0x40), 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (i|0x180), 0x01FF);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (i|0x1C0), 0x01FF);

        for ( x=0;x<5;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BKA3_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BKA3_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BKA3_45_L, u8Ramcode[4], 0x00FF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x0100, 0x0100);
    }

    MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x00, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK80_03_L, u32Data, 0x1000);

}

void Hal_PQ_set_y_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData)
{
#if 0
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[10];
    MS_U32 u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_Y_SRAM1 &&
       u8sramtype != SC_FILTER_Y_SRAM2 &&
       u8sramtype != SC_FILTER_Y_SRAM3 &&
       u8sramtype != SC_FILTER_Y_SRAM4)
    {
        printf("Unknown y sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U32)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0001, 0x0001); // enable y_sram_rw
    for ( i=0; i<128; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100);
        j=i*10;

        // address

        if(u8sramtype == SC_FILTER_Y_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0000|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0080|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0100|i), 0x01FF);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0180|i), 0x01FF);

        for ( x=0;x<8;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        for ( x=8;x<10;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, (((MS_U16)u8Ramcode[5])<<8|(MS_U16)u8Ramcode[4]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_49_L, (((MS_U16)u8Ramcode[7])<<8|(MS_U16)u8Ramcode[6]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_4A_L, (((MS_U16)u8Ramcode[9])<<8|(MS_U16)u8Ramcode[8]), 0xFFFF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);
#else
        MS_U16 i, j, x;
        MS_U8 u8Ramcode[10];
        MS_U8* u32Addr;

        if(u8sramtype != SC_FILTER_Y_SRAM1 &&
           u8sramtype != SC_FILTER_Y_SRAM2 &&
           u8sramtype != SC_FILTER_Y_SRAM3 &&
           u8sramtype != SC_FILTER_Y_SRAM4)
        {
            printf("Unknown y sram type %u\n", u8sramtype);
            return;
        }
        u32Addr = (MS_U8*)pData;
        MApi_XC_W2BYTEMSK(REG_SC_BK80_03_L, 0x0000, 0x1000);
        MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x0001, 0x0001); // enable y_sram_rw
         if((u8sramtype == SC_FILTER_Y_SRAM1) || (u8sramtype == SC_FILTER_Y_SRAM3))
         {
           // SRAM1  (128x40)
          for ( i=0; i<128; i++)
          {
             while(MApi_XC_R2BYTE(REG_SC_BKA3_41_L) & 0x0100);
             j=i*5;
             // address & sram select
            if(u8sramtype == SC_FILTER_Y_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (0x0000|i), 0x01FF);
            else //if(u8sramtype == SC_FILTER_Y_SRAM3)
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (0x0100|i), 0x01FF);

            for ( x=0;x<5;x++ )
            {
                u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
                PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
            }

            PQ_DUMP_FILTER_DBG(printf("\n"));

            MApi_XC_W2BYTEMSK(REG_SC_BKA3_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_45_L, ((MS_U16)u8Ramcode[4]), 0x00FF);

            // enable write
            MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x0100, 0x0100);
          }
        }
        else if((u8sramtype == SC_FILTER_Y_SRAM2) || (u8sramtype == SC_FILTER_Y_SRAM4))
        {
            // SRAM2  (128x60)
            for ( i=0; i<128; i++)
            {
             while(MApi_XC_R2BYTE(REG_SC_BKA3_41_L) & 0x0100);
             j=i*8;
             // address & sram select
             if(u8sramtype == SC_FILTER_Y_SRAM2)
             MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (0x0080|i), 0x01FF);
             else //if(u8sramtype == SC_FILTER_Y_SRAM4)
             MApi_XC_W2BYTEMSK(REG_SC_BKA3_42_L, (0x0180|i), 0x01FF);

             for ( x=0;x<8;x++ )
             {
               u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
               PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
             }
                PQ_DUMP_FILTER_DBG(printf("\n"));

               MApi_XC_W2BYTEMSK(REG_SC_BKA3_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
               MApi_XC_W2BYTEMSK(REG_SC_BKA3_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
               MApi_XC_W2BYTEMSK(REG_SC_BKA3_45_L, (((MS_U16)u8Ramcode[5])<<8|(MS_U16)u8Ramcode[4]), 0xFFFF);
               MApi_XC_W2BYTEMSK(REG_SC_BKA3_49_L, (((MS_U16)u8Ramcode[7])<<8|(MS_U16)u8Ramcode[6]), 0x0FFF);

                // enable write
               MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x0100, 0x0100);
            }
         }
        MApi_XC_W2BYTEMSK(REG_SC_BKA3_41_L, 0x00, 0x00FF);
        MApi_XC_W2BYTEMSK(REG_SC_BK80_03_L, 0x1000, 0x1000);
#endif
}

void Hal_PQ_set_sram_color_index_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i;
    MS_U8 u8Ramcode;
    MS_U8* u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_SRAM_COLOR_INDEX)
    {
        return;
    }

    u32Addr = (MS_U8*)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);

    for(i=0; i<256; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK06_61_L) & 0x0100);

        MApi_XC_W2BYTEMSK(REG_SC_BK06_63_L, i, 0x00FF);
        u8Ramcode = *((MS_U8 *)(u32Addr + (i)));
        PQ_DUMP_FILTER_DBG(printf(" %02x\n", u8Ramcode ));

        MApi_XC_W2BYTEMSK(REG_SC_BK06_62_L, u8Ramcode, 0x0007);
        MApi_XC_W2BYTEMSK(REG_SC_BK06_61_L, 0x0100, 0x0100);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);

}

void Hal_PQ_set_sram_color_gain_snr_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i;
    MS_U16 u16Ramcode;
    MS_U8* u32Addr;
    MS_U16 u16Gain_DNR;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_SRAM_COLOR_GAIN_SNR)
    {
        return;
    }

    u32Addr = (MS_U8*)pData;
    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);

    for(i=0; i<8; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK06_61_L) & 0x0200);

        MApi_XC_W2BYTEMSK(REG_SC_BK06_66_L, i, 0x00FF);

        u16Gain_DNR = MApi_XC_R2BYTE(REG_SC_BK06_65_L) & 0x001F;
        u16Ramcode = *((MS_U8 *)(u32Addr + (i)));
        u16Ramcode = u16Ramcode<<8|u16Gain_DNR;

        PQ_DUMP_FILTER_DBG(printf(" %02x\n", u16Ramcode ));

        MApi_XC_W2BYTEMSK(REG_SC_BK06_64_L, u16Ramcode, 0x1F1F);
        MApi_XC_W2BYTEMSK(REG_SC_BK06_61_L, 0x0200, 0x0200);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);
}

#define ICC_H_SIZE	32
#define ICC_V_SIZE	32

#define ICC_MAX_SRAM_SIZE 0x124

#define SRAM1_ICC_COUNT 289//81
#define SRAM2_ICC_COUNT 272//72
#define SRAM3_ICC_COUNT 272//72
#define SRAM4_ICC_COUNT 256//64

#define SRAM1_ICC_OFFSET 0
#define SRAM2_ICC_OFFSET SRAM1_ICC_COUNT
#define SRAM3_ICC_OFFSET (SRAM2_ICC_OFFSET + SRAM2_ICC_COUNT)
#define SRAM4_ICC_OFFSET (SRAM3_ICC_OFFSET + SRAM3_ICC_COUNT)

#define SRAM_ICC_TOTAL_COUNT SRAM1_ICC_COUNT + SRAM2_ICC_COUNT + SRAM3_ICC_COUNT + SRAM4_ICC_COUNT

void _Hal_PQ_set_sram_icc_crd_table(void *pInstance,MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt)
{
    MS_U16 i;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }


    MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, BIT(0), BIT(0)); // io_en disable
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Cnt; i++)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_79_L, i, 0x01FF); // address[8:0]
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_7A_L, pBuf[i], 0x01FF); //data[8:0]
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_7A_L, BIT(15), BIT(15)); // io_w enable
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, 0, BIT(0)); // io_en enable
}

void Hal_PQ_set_sram_icc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 SRAM1_ICC[ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 SRAM2_ICC[ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 SRAM3_ICC[ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 SRAM4_ICC[ICC_MAX_SRAM_SIZE] = {0};

    MS_U16* u32Addr = (MS_U16*)pData;
    MS_U16 u16Index = 0 ;
    MS_U16 cont1=0, cont2=0, cont3=0, cont4=0;

    if(u8sramtype != SC_FILTER_SRAM_ICC_CRD)
    {
        return;
    }
//    printf("[%s]SRAM_IHC_TOTAL_COUNT=%u,SRAM2_IHC_OFFSET=%u,SRAM3_IHC_OFFSET=%u,SRAM4_IHC_OFFSET=%u\n",
//        __FUNCTION__,SRAM_IHC_TOTAL_COUNT,SRAM2_IHC_OFFSET,SRAM3_IHC_OFFSET,SRAM4_IHC_OFFSET);

    for (;u16Index < SRAM_ICC_TOTAL_COUNT ; u16Index++)
    {
        if (u16Index < SRAM2_ICC_OFFSET)
        {
            SRAM1_ICC[cont1] = (u32Addr[u16Index]);
//            printf("@@SRAM1_IHC[%u]=%x\n",cont1,(u32Addr[u16Index]));
            cont1++;
        }
        else if (u16Index < SRAM3_ICC_OFFSET)
        {
            SRAM2_ICC[cont2] = (u32Addr[u16Index]);
//            printf("@@SRAM2_IHC[%u]=%x\n",cont2,(u32Addr[u16Index]));
            cont2++;

        }
        else if (u16Index < SRAM4_ICC_OFFSET)
        {
            SRAM3_ICC[cont3] = (u32Addr[u16Index]);
//            printf("@@SRAM3_IHC[%u]=%x\n",cont3,(u32Addr[u16Index]));
            cont3++;
        }
        else
        {
            SRAM4_ICC[cont4] = (u32Addr[u16Index]);
//            printf("@@SRAM4_IHC[%u]=%x\n",cont4,(u32Addr[u16Index]));
            cont4++;
        }
    }

    _Hal_PQ_set_sram_icc_crd_table(pInstance,&SRAM1_ICC[0], 0, SRAM1_ICC_COUNT);
    _Hal_PQ_set_sram_icc_crd_table(pInstance,&SRAM2_ICC[0], 1, SRAM2_ICC_COUNT);
    _Hal_PQ_set_sram_icc_crd_table(pInstance,&SRAM3_ICC[0], 2, SRAM3_ICC_COUNT);
    _Hal_PQ_set_sram_icc_crd_table(pInstance,&SRAM4_ICC[0], 3, SRAM4_ICC_COUNT);

}

void Hal_PQ_get_sram_icc_crd_table(void *pInstance,MS_U8 u8SRAM_Idx, MS_U8 *pBuf)
{
	MS_U16 i;
    MS_U16 u16Size;

    if(u8SRAM_Idx > 3)
	{
        u8SRAM_Idx = 0;
	}


	MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, BIT(0), BIT(0)); // io_en disable

    switch(u8SRAM_Idx)
    {
        case 0:
            u16Size = SRAM1_ICC_COUNT;
            break;
        case 1:
            u16Size = SRAM2_ICC_COUNT;
            break;
        case 2:
            u16Size = SRAM3_ICC_COUNT;
            break;
        case 3:
            u16Size = SRAM4_ICC_COUNT;
            break;
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Size; i++)
	{
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_79_L, i, 0x01FF); // address[8:0]
		MApi_XC_W2BYTEMSK(REG_SC_BK1C_79_L, BIT(15), BIT(15)); // io_R enable
		MsOS_DelayTaskUs(1);

		pBuf[i] = MApi_XC_R2BYTEMSK(REG_SC_BK1C_7B_L, 0x01FF); //data[8:0]
		printf("[%s]ICC_CRD_Data[%d]=%x\n", __FUNCTION__, i, pBuf[i]);
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK1C_78_L, 0, BIT(0)); // io_en enable
}

#define IHC_H_SIZE	33
#define IHC_V_SIZE	33
#define BUF_BYTE_SIZE (IHC_H_SIZE*IHC_V_SIZE)
#define MAX_SRAM_SIZE 0x124

#define SRAM1_IHC_COUNT 289//81
#define SRAM2_IHC_COUNT 272//72
#define SRAM3_IHC_COUNT 272//72
#define SRAM4_IHC_COUNT 256//64

#define SRAM1_IHC_OFFSET 0
#define SRAM2_IHC_OFFSET SRAM1_IHC_COUNT
#define SRAM3_IHC_OFFSET (SRAM2_IHC_OFFSET + SRAM2_IHC_COUNT)
#define SRAM4_IHC_OFFSET (SRAM3_IHC_OFFSET + SRAM3_IHC_COUNT)

#define SRAM_IHC_TOTAL_COUNT SRAM1_IHC_COUNT + SRAM2_IHC_COUNT + SRAM3_IHC_COUNT + SRAM4_IHC_COUNT

void _Hal_PQ_set_sram_ihc_crd_table(void *pInstance,MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt)
{
    MS_U16 i;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }


    MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, BIT(0), BIT(0)); // io_en disable
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Cnt; i++)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_7D_L, i, 0x01FF); // address[8:0]
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_7E_L, pBuf[i], 0x01FF); //data[8:0]
        MApi_XC_W2BYTEMSK(REG_SC_BK1C_7E_L, BIT(15), BIT(15)); // io_w enable
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, 0, BIT(0)); // io_en enable
}

void Hal_PQ_set_sram_ihc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{

    MS_U16 SRAM1_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM2_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM3_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM4_IHC[MAX_SRAM_SIZE];

    MS_U16* u32Addr = (MS_U16*)pData;
    MS_U16 u16Index = 0 ;
    MS_U16 cont1=0, cont2=0, cont3=0, cont4=0;

    if(u8sramtype != SC_FILTER_SRAM_IHC_CRD)
    {
        return;
    }
//    printf("[%s]SRAM_IHC_TOTAL_COUNT=%u,SRAM2_IHC_OFFSET=%u,SRAM3_IHC_OFFSET=%u,SRAM4_IHC_OFFSET=%u\n",
//        __FUNCTION__,SRAM_IHC_TOTAL_COUNT,SRAM2_IHC_OFFSET,SRAM3_IHC_OFFSET,SRAM4_IHC_OFFSET);

    for (;u16Index < SRAM_IHC_TOTAL_COUNT ; u16Index++)
    {
        if (u16Index < SRAM2_IHC_OFFSET)
        {
            SRAM1_IHC[cont1] = (u32Addr[u16Index]);
//            printf("@@SRAM1_IHC[%u]=%lx\n",cont1,(u32Addr[u16Index]));
            cont1++;
        }
        else if (u16Index < SRAM3_IHC_OFFSET)
        {
            SRAM2_IHC[cont2] = (u32Addr[u16Index]);
//            printf("@@SRAM2_IHC[%u]=%lx\n",cont2,(u32Addr[u16Index]));
            cont2++;

        }
        else if (u16Index < SRAM4_IHC_OFFSET)
        {
            SRAM3_IHC[cont3] = (u32Addr[u16Index]);
//            printf("@@SRAM3_IHC[%u]=%lx\n",cont3,(u32Addr[u16Index]));
            cont3++;
        }
        else
        {
            SRAM4_IHC[cont4] = (u32Addr[u16Index]);
//            printf("@@SRAM4_IHC[%u]=%lx\n",cont4,(u32Addr[u16Index]));
            cont4++;
        }
    }

    _Hal_PQ_set_sram_ihc_crd_table(pInstance,&SRAM1_IHC[0], 0, SRAM1_IHC_COUNT);
    _Hal_PQ_set_sram_ihc_crd_table(pInstance,&SRAM2_IHC[0], 1, SRAM2_IHC_COUNT);
    _Hal_PQ_set_sram_ihc_crd_table(pInstance,&SRAM3_IHC[0], 2, SRAM3_IHC_COUNT);
    _Hal_PQ_set_sram_ihc_crd_table(pInstance,&SRAM4_IHC[0], 3, SRAM4_IHC_COUNT);

}

void Hal_PQ_get_sram_ihc_crd_table(void *pInstance,MS_U8 u8SRAM_Idx, MS_U8 *pBuf)
{
    MS_U16 i;
	MS_U16 u16Size;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }


	MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, BIT(0), BIT(0)); // io_en disable

	switch(u8SRAM_Idx)
	{
		case 0:
			u16Size = SRAM1_IHC_COUNT;
			break;
		case 1:
			u16Size = SRAM2_IHC_COUNT;
			break;
		case 2:
			u16Size = SRAM3_IHC_COUNT;
			break;
		case 3:
			u16Size = SRAM4_IHC_COUNT;
			break;
	}
    MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

	for(i=0; i<u16Size; i++)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK1C_7D_L, i, 0x01FF); // address[8:0]
		MApi_XC_W2BYTEMSK(REG_SC_BK1C_7D_L, BIT(15), BIT(15)); // io_R enable
		MsOS_DelayTaskUs(1);

		pBuf[i] = MApi_XC_R2BYTEMSK(REG_SC_BK1C_7F_L, 0x01FF); //data[8:0]
		printf("[%s]IHC_CRD_Data[%d]=%x\n", __FUNCTION__, i, pBuf[i]);
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK1C_7C_L, 0, BIT(0)); // io_en enable
}


void Hal_PQ_set_sram_color_gain_dnr_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i;
    MS_U16 u16Ramcode;
    MS_U8* u32Addr;
    MS_U16 u16Gain_SNR;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_SRAM_COLOR_GAIN_DNR)
    {
        return;
    }

    u32Addr = (MS_U8*)pData;

    u32Data = MApi_XC_R2BYTEMSK(REG_SC_BK00_03_L, 0x1000);
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, 0x0000, 0x1000);

    for(i=0; i<8; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK06_61_L) & 0x0200);

        MApi_XC_W2BYTEMSK(REG_SC_BK06_66_L, i, 0x00FF);

        u16Gain_SNR = MApi_XC_R2BYTE(REG_SC_BK06_65_L) & 0x1F00;
        u16Ramcode = *((MS_U8 *)(u32Addr + (i)));
        u16Ramcode = u16Gain_SNR | u16Ramcode;

        PQ_DUMP_FILTER_DBG(printf(" %02x\n", u16Ramcode ));

        MApi_XC_W2BYTEMSK(REG_SC_BK06_64_L, u16Ramcode, 0x1F1F);
        MApi_XC_W2BYTEMSK(REG_SC_BK06_61_L, 0x0200, 0x0200);
    }
    MApi_XC_W2BYTEMSK(REG_SC_BK00_03_L, u32Data, 0x1000);
}


MS_U16 Hal_PQ_get_hnonlinear_idx(void *pInstance,MS_HNONLINEAR_TYPE etype)
{
    MS_U16 u16ret;

    switch(etype)
    {
    case MS_HNONLINEAR_3840_0:
        u16ret = PQ_IP_HnonLinear_H_3840_1_Main;
        break;

    case MS_HNONLINEAR_3840_1:
        u16ret = PQ_IP_HnonLinear_H_3840_2_Main;
        break;

    case MS_HNONLINEAR_3840_2:
        u16ret = PQ_IP_HnonLinear_H_3840_3_Main;
        break;

    case MS_HNONLINEAR_1920_0:
        u16ret = PQ_IP_HnonLinear_H_1920_0_Main;
        break;

    case MS_HNONLINEAR_1920_1:
        u16ret = PQ_IP_HnonLinear_H_1920_1_Main;
        break;

    case MS_HNONLINEAR_1920_2:
        u16ret = PQ_IP_HnonLinear_H_1920_2_Main;
        break;

    case MS_HNONLINEAR_1366_0:
        u16ret = PQ_IP_HnonLinear_H_1366_0_Main;
        break;

    case MS_HNONLINEAR_1366_1:
        u16ret = PQ_IP_HnonLinear_H_1366_1_Main;
        break;

    case MS_HNONLINEAR_1366_2:
        u16ret = PQ_IP_HnonLinear_H_1366_2_Main;
        break;

    default:
        u16ret = PQ_IP_HnonLinear_OFF_Main;
        break;
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_sram_size(void *pInstance,MS_U16 u16sramtype)
{
    MS_U16 u16ret;

    switch(u16sramtype)
    {
    case SC_FILTER_Y_SRAM1:
        u16ret = PQ_IP_SRAM1_SIZE_Main;
        break;

    case SC_FILTER_Y_SRAM2:
        u16ret = PQ_IP_SRAM2_SIZE_Main;
        break;

    case SC_FILTER_Y_SRAM3:
        u16ret = PQ_IP_SRAM3_SIZE_Main;
        break;

    case SC_FILTER_Y_SRAM4:
        u16ret = PQ_IP_SRAM4_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM1:
        u16ret = PQ_IP_C_SRAM1_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM2:
        u16ret = PQ_IP_C_SRAM2_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM3:
        u16ret = PQ_IP_C_SRAM3_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM4:
        u16ret = PQ_IP_C_SRAM4_SIZE_Main;
        break;

    case SC_FILTER_SRAM_COLOR_INDEX:
        u16ret = PQ_IP_SRAM_COLOR_INDEX_SIZE_Main;
        break;
#if 0
    case SC_FILTER_SRAM_COLOR_GAIN_SNR:
        u16ret = PQ_IP_SRAM_COLOR_GAIN_SNR_SIZE_Main;
        break;

    case SC_FILTER_SRAM_COLOR_GAIN_DNR:
        u16ret = PQ_IP_SRAM_COLOR_GAIN_DNR_SIZE_Main;
        break;
#endif

    case SC_FILTER_SRAM_ICC_CRD:
        u16ret = PQ_IP_VIP_ICC_CRD_SRAM_SIZE_Main;
        break;

    case SC_FILTER_SRAM_IHC_CRD:
        u16ret = PQ_IP_VIP_IHC_CRD_SRAM_SIZE_Main;
        break;

    default:
        u16ret = 0;
        break;
    }

    return u16ret;
}

void Hal_PQ_OPWriteOff_Enable(void *pInstance,MS_BOOL bEna)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK12_27_L, bEna, BIT(0));
}

MS_U16 Hal_PQ_get_rw_method(void *pInstance,MS_BOOL bMainWin)
{
    MS_U16 u16method;
    if(bMainWin)
    {
        u16method  = (MApi_XC_R2BYTE(REG_SC_BK01_21_L) & 0xC000);
    }
    else
    {
        u16method  = (MApi_XC_R2BYTE(REG_SC_BK03_21_L) & 0xC000);
    }
    return u16method;
}

void Hal_PQ_set_sram_xvycc_gamma_table(void *pInstance,MS_U8 u8sramtype, void *pData)
{
    MS_U16 i;
    MS_U16 u16SRamcode;
    MS_U16 u16Count = 0 ;
    MS_U8* u8Addr = (MS_U8*) pData;

    if(u8sramtype != SC_FILTER_SRAM_XVYCC_DE_GAMMA &&
       u8sramtype != SC_FILTER_SRAM_XVYCC_GAMMA )
    {
        return;
    }

    if (u8sramtype == SC_FILTER_SRAM_XVYCC_DE_GAMMA)
    {
        MS_U32 u32InputData;
        MS_U32 u32XVYCC_DGT_DATA;
        MS_U16 u16SRamCode_L, u16SRamCode_H;
        MS_U16 u16WriteCount = 0;

        while (MApi_XC_R2BYTE(REG_SC_BK2D_60_L) & BIT(3)){;} // waiting ready
        //printf("REG_SC_BK2D_60_L=%X\n", MApi_XC_R2BYTE(REG_SC_BK2D_60_L) );

        // RGB write together and write one time for clear write bit.
        //MApi_XC_W2BYTEMSK(REG_SC_BK2D_60_L, BIT(5) | BIT(6), BIT(5) | BIT(6) ); // io_en disable
        MApi_XC_W2BYTEMSK(REG_SC_BK2D_60_L, BIT(5) | BIT(6), 0xF8 ); // io_en disable


        for( i = 0; i < 1803; i += 3 )
        {
            u32InputData = ((MS_U16)u8Addr[i+2]<<16)|((MS_U16)u8Addr[i+1]<<8)|((MS_U16)u8Addr[i]);
            u32XVYCC_DGT_DATA = u32InputData&0x7FFFF; // 19bis

            u16SRamCode_L = u32XVYCC_DGT_DATA&0xFFFF;
            u16SRamCode_H = u32XVYCC_DGT_DATA>>16;

            // waiting ready.
            while (MApi_XC_R2BYTE(REG_SC_BK2D_60_L) & BIT(3)){;}

            MApi_XC_W2BYTE(REG_SC_BK2D_61_L, u16WriteCount); // address
            u16WriteCount += 1;
            MApi_XC_W2BYTE(REG_SC_BK2D_63_L, u16SRamCode_H); //data
            MApi_XC_W2BYTE(REG_SC_BK2D_62_L, u16SRamCode_L); //data
            MApi_XC_W2BYTEMSK(REG_SC_BK2D_60_L,  0x60, 0x60); // Select All Channel
            MApi_XC_W2BYTEMSK(REG_SC_BK2D_60_L, BIT(3), BIT(3)); // io_w enable
        }

        while (MApi_XC_R2BYTE(REG_SC_BK2D_60_L) & BIT(3)){;}

        MApi_XC_W2BYTE(REG_SC_BK2D_60_L, 0 ); // Disable Write
    }
    else
    {
        u16Count = PQ_IP_LinearRGB_GAMMA_SRAM_SIZE_Main >> 1;

        // address port initial as 0
        MApi_XC_W2BYTE(REG_SC_BK25_79_L, 0x0 );
        // pre set value
        MApi_XC_W2BYTE(REG_SC_BK25_7A_L, 0x0 );

        // RGB write together and write one time for clear write bit.
        MApi_XC_W2BYTEMSK(REG_SC_BK25_78_L, BIT(3) | BIT(5) | BIT(6) , BIT(3) | BIT(5) | BIT(6) ); // io_en disable


        for(i=0; i<u16Count; i++)
        {
            u16SRamcode = u8Addr[i*2] | u8Addr[i*2+1] << 8;

            // waiting ready.
            while (MApi_XC_R2BYTE(REG_SC_BK25_78_L) & BIT(3));

            MApi_XC_W2BYTE(REG_SC_BK25_79_L, i); // address
            MApi_XC_W2BYTE(REG_SC_BK25_7A_L, u16SRamcode); //data
            MApi_XC_W2BYTEMSK(REG_SC_BK25_78_L, BIT(3), BIT(3)); // io_w enable

        }

        MApi_XC_W2BYTE(REG_SC_BK25_78_L, 0 ); // io_en enable
    }

}

void Hal_PQ_set_rw_method(void *pInstance,MS_BOOL bMainWin, MS_U16 u16method)
{
   if(bMainWin)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK01_21_L, u16method, 0xC000);
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK03_21_L, u16method, 0xC000);
    }

}

void Hal_PQ_set_xvycc_matrix_coefficient(void *pInstance,MS_S32 pData[][3], MS_BOOL bMainWin)
{
    if (bMainWin)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK25_02_L, pData[0][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_03_L, pData[0][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_04_L, pData[0][2], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_05_L, pData[1][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_06_L, pData[1][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_07_L, pData[1][2], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_08_L, pData[2][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_09_L, pData[2][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_0A_L, pData[2][2], 0x1FFF);
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK25_12_L, pData[0][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_13_L, pData[0][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_14_L, pData[0][2], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_15_L, pData[1][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_16_L, pData[1][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_17_L, pData[1][2], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_18_L, pData[2][0], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_19_L, pData[2][1], 0x1FFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK25_1A_L, pData[2][2], 0x1FFF);
    }
}

void Hal_PQ_set_xvycc_matrix_enable(void *pInstance,MS_BOOL bEnable, MS_BOOL bMainWin)
{
    if (bMainWin)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK25_01_L, bEnable ? BIT(1) : 0, BIT(1));
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK25_11_L, bEnable ? BIT(1) : 0, BIT(1));
    }
}

MS_BOOL HAL_PQ_user_mode_control(void *pInstance)
{
	return FALSE;
}

//-------------------------------------------------------------------------------------------------
// set equation to VIP CSC
// Both equation selection rules are
// 0: SDTV(601) R  G  B  : 16-235
// 1: SDTV(601) R  G  B  : 0-255
/// @param bMainWin       \b IN: Enable
//-------------------------------------------------------------------------------------------------
void HAL_PQ_set_SelectCSC(void *pInstance,MS_U16 u16selection, MS_BOOL  bMainWin)
{
    if (bMainWin)
    {
        if (u16selection==0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0F_L, 0xF0, 0xFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0E_L, 0x00, 0x03);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_16_L, 0x4A, 0xFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_02_L, 0xC0, 0xF0);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0F_L, 0x00, 0xFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0E_L, 0x00, 0x03);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_16_L, 0x40, 0xFF);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_02_L, 0x00, 0xF0);
        }
    }
    else
    {
        if (u16selection==0)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0F_L, 0xF000, 0xFF00);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0E_L, 0x0000, 0x0300);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_16_L, 0x4A00, 0xFF00);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_02_L, 0xC000, 0xF000);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0F_L, 0x0000, 0xFF00);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_0E_L, 0x0000, 0x0300);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_16_L, 0x4000, 0xFF00);
            MApi_XC_W2BYTEMSK(REG_SC_BK1A_02_L, 0x0000, 0xF000);
        }
    }
}

MS_U8 Hal_PQ_get_madi_fbl_mode(void *pInstance, MS_BOOL bMemFmt422,MS_BOOL bInterlace)
{
    UNUSED(bInterlace);
    if(bMemFmt422)
    {
        return MS_MADI_P_MODE_MOT10;
    }
    else
    {
        return MS_MADI_P_MODE8;
    }
}

void Hal_PQ_set_UFSC_c_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData)
{
#if 0
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[8];
    MS_U8 *u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_C_SRAM1 &&
       u8sramtype != SC_FILTER_C_SRAM2 &&
       u8sramtype != SC_FILTER_C_SRAM3 &&
       u8sramtype != SC_FILTER_C_SRAM4)
    {
        printf("Unknown c sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U8 *)pData;
    u32Data = (MApi_XC_ReadByte(REG_FSC_BK00_03_H) & 0x10) << 8;
    MApi_XC_Write2ByteMask(REG_FSC_BK00_03_L, 0x0000, 0x1000);
    MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x0002, 0x0002); // enable c_sram_rw

    for(i=0; i<64; i++)
    {
        while(MApi_XC_ReadByte(REG_FSC_BK23_41_H) & 0x01);
        j=i*5;

        if(u8sramtype == SC_FILTER_C_SRAM1)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, i, 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM2)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (i|0x40), 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM3)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (i|0x180), 0x01FF);
        else
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (i|0x1C0), 0x01FF);

        for ( x=0;x<5;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_Write2ByteMask(REG_FSC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_45_L, u8Ramcode[4], 0x00FF);

        // enable write
        MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x0100, 0x0100);
    }

    MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_Write2ByteMask(REG_FSC_BK00_03_L, u32Data, 0x1000);
#endif
}

void Hal_PQ_set_UFSC_y_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData)
{
#if 0
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[10];
    MS_U8 *u32Addr;
    MS_U32 u32Data = 0;

    if(u8sramtype != SC_FILTER_Y_SRAM1 &&
       u8sramtype != SC_FILTER_Y_SRAM2 &&
       u8sramtype != SC_FILTER_Y_SRAM3 &&
       u8sramtype != SC_FILTER_Y_SRAM4)
    {
        printf("Unknown y sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U8 *)pData;
    u32Data = (MApi_XC_ReadByte(REG_FSC_BK00_03_H) & 0x10) << 8;

    MApi_XC_Write2ByteMask(REG_FSC_BK00_03_L, 0x0000, 0x1000);

    MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x0001, 0x0001); // enable y_sram_rw

    for ( i=0; i<128; i++)
    {
        while(MApi_XC_ReadByte(REG_FSC_BK23_41_H) & 0x01);
        j=i*10;

        // address

        if(u8sramtype == SC_FILTER_Y_SRAM1)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (0x0000|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM2)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (0x0080|i), 0x01FF);
        else if(u8sramtype == SC_FILTER_Y_SRAM3)
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (0x0100|i), 0x01FF);
        else
            MApi_XC_Write2ByteMask(REG_FSC_BK23_42_L, (0x0180|i), 0x01FF);

        for ( x=0;x<8;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        for ( x=8;x<10;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_Write2ByteMask(REG_FSC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_45_L, (((MS_U16)u8Ramcode[5])<<8|(MS_U16)u8Ramcode[4]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_49_L, (((MS_U16)u8Ramcode[7])<<8|(MS_U16)u8Ramcode[6]), 0xFFFF);
        MApi_XC_Write2ByteMask(REG_FSC_BK23_4A_L, (((MS_U16)u8Ramcode[9])<<8|(MS_U16)u8Ramcode[8]), 0xFFFF);

        // enable write
        MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x0100, 0x0100);

    }
    MApi_XC_Write2ByteMask(REG_FSC_BK23_41_L, 0x00, 0x00FF);

    MApi_XC_Write2ByteMask(REG_FSC_BK00_03_L, u32Data, 0x1000);
#endif
}

#define UFSC_ICC_H_SIZE  32
#define UFSC_ICC_V_SIZE  32

#define UFSC_ICC_MAX_SRAM_SIZE 0x124

#define UFSC_SRAM1_ICC_COUNT 289//81
#define UFSC_SRAM2_ICC_COUNT 272//72
#define UFSC_SRAM3_ICC_COUNT 272//72
#define UFSC_SRAM4_ICC_COUNT 256//64

#define UFSC_SRAM1_ICC_OFFSET 0
#define UFSC_SRAM2_ICC_OFFSET UFSC_SRAM1_ICC_COUNT
#define UFSC_SRAM3_ICC_OFFSET (UFSC_SRAM2_ICC_OFFSET + UFSC_SRAM2_ICC_COUNT)
#define UFSC_SRAM4_ICC_OFFSET (UFSC_SRAM3_ICC_OFFSET + UFSC_SRAM3_ICC_COUNT)

#define UFSC_SRAM_ICC_TOTAL_COUNT UFSC_SRAM1_ICC_COUNT + UFSC_SRAM2_ICC_COUNT + UFSC_SRAM3_ICC_COUNT + UFSC_SRAM4_ICC_COUNT

void _Hal_PQ_set_UFSC_sram_icc_crd_table(MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt)
{
    MS_U16 i;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }


    MApi_XC_Write2ByteMask(REG_FSC_BK1C_78_L, BIT(0), BIT(0)); // io_en disable
    MApi_XC_Write2ByteMask(REG_FSC_BK1C_78_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Cnt; i++)
    {
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_79_L, i, 0x01FF); // address[8:0]
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_7A_L, pBuf[i], 0x01FF); //data[8:0]
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_7A_L, BIT(15), BIT(15)); // io_w enable
    }
    MApi_XC_Write2ByteMask(REG_FSC_BK1C_78_L, 0, BIT(0)); // io_en enable
}

void Hal_PQ_set_UFSC_sram_icc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData)
{

    MS_U16 UFSC_SRAM1_ICC[UFSC_ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM2_ICC[UFSC_ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM3_ICC[UFSC_ICC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM4_ICC[UFSC_ICC_MAX_SRAM_SIZE] = {0};

    MS_U16* u32Addr = (MS_U16*)pData;
    MS_U16 u16Index = 0 ;
    MS_U16 cont1=0, cont2=0, cont3=0, cont4=0;

    if(u8sramtype != SC_FILTER_SRAM_ICC_CRD)
    {
        return;
    }
//    printf("[%s]SRAM_IHC_TOTAL_COUNT=%u,SRAM2_IHC_OFFSET=%u,SRAM3_IHC_OFFSET=%u,SRAM4_IHC_OFFSET=%u\n",
//        __FUNCTION__,SRAM_IHC_TOTAL_COUNT,SRAM2_IHC_OFFSET,SRAM3_IHC_OFFSET,SRAM4_IHC_OFFSET);

    for (;u16Index < UFSC_SRAM_ICC_TOTAL_COUNT ; u16Index++)
    {
        if (u16Index < UFSC_SRAM2_ICC_OFFSET)
        {
            UFSC_SRAM1_ICC[cont1] = (u32Addr[u16Index]);
//            printf("@@SRAM1_IHC[%u]=%x\n",cont1,(u32Addr[u16Index]));
            cont1++;
        }
        else if (u16Index < UFSC_SRAM3_ICC_OFFSET)
        {
            UFSC_SRAM2_ICC[cont2] = (u32Addr[u16Index]);
//            printf("@@SRAM2_IHC[%u]=%x\n",cont2,(u32Addr[u16Index]));
            cont2++;

        }
        else if (u16Index < UFSC_SRAM4_ICC_OFFSET)
        {
            UFSC_SRAM3_ICC[cont3] = (u32Addr[u16Index]);
//            printf("@@SRAM3_IHC[%u]=%x\n",cont3,(u32Addr[u16Index]));
            cont3++;
        }
        else
        {
            UFSC_SRAM4_ICC[cont4] = (u32Addr[u16Index]);
//            printf("@@SRAM4_IHC[%u]=%x\n",cont4,(u32Addr[u16Index]));
            cont4++;
        }
    }

    _Hal_PQ_set_UFSC_sram_icc_crd_table(&UFSC_SRAM1_ICC[0], 0, UFSC_SRAM1_ICC_COUNT);
    _Hal_PQ_set_UFSC_sram_icc_crd_table(&UFSC_SRAM2_ICC[0], 1, UFSC_SRAM2_ICC_COUNT);
    _Hal_PQ_set_UFSC_sram_icc_crd_table(&UFSC_SRAM3_ICC[0], 2, UFSC_SRAM3_ICC_COUNT);
    _Hal_PQ_set_UFSC_sram_icc_crd_table(&UFSC_SRAM4_ICC[0], 3, UFSC_SRAM4_ICC_COUNT);

}

#define UFSC_IHC_H_SIZE  33
#define UFSC_IHC_V_SIZE  33
#define UFSC_BUF_BYTE_SIZE (UFSC_IHC_H_SIZE*UFSC_IHC_V_SIZE)
#define UFSC_MAX_SRAM_SIZE 0x124

#define UFSC_SRAM1_IHC_COUNT 289//81
#define UFSC_SRAM2_IHC_COUNT 272//72
#define UFSC_SRAM3_IHC_COUNT 272//72
#define UFSC_SRAM4_IHC_COUNT 256//64

#define UFSC_SRAM1_IHC_OFFSET 0
#define UFSC_SRAM2_IHC_OFFSET UFSC_SRAM1_IHC_COUNT
#define UFSC_SRAM3_IHC_OFFSET (UFSC_SRAM2_IHC_OFFSET + UFSC_SRAM2_IHC_COUNT)
#define UFSC_SRAM4_IHC_OFFSET (UFSC_SRAM3_IHC_OFFSET + UFSC_SRAM3_IHC_COUNT)

#define UFSC_SRAM_IHC_TOTAL_COUNT UFSC_SRAM1_IHC_COUNT + UFSC_SRAM2_IHC_COUNT + UFSC_SRAM3_IHC_COUNT + UFSC_SRAM4_IHC_COUNT

void _Hal_PQ_set_UFSC_sram_ihc_crd_table(MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt)
{
    MS_U16 i;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }


    MApi_XC_Write2ByteMask(REG_FSC_BK1C_7C_L, BIT(0), BIT(0)); // io_en disable
    MApi_XC_Write2ByteMask(REG_FSC_BK1C_7C_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Cnt; i++)
    {
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_7D_L, i, 0x01FF); // address[8:0]
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_7E_L, pBuf[i], 0x01FF); //data[8:0]
        MApi_XC_Write2ByteMask(REG_FSC_BK1C_7E_L, BIT(15), BIT(15)); // io_w enable
    }
    MApi_XC_Write2ByteMask(REG_FSC_BK1C_7C_L, 0, BIT(0)); // io_en enable
}

void Hal_PQ_set_UFSC_sram_ihc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData)
{
    MS_U16 UFSC_SRAM1_IHC[UFSC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM2_IHC[UFSC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM3_IHC[UFSC_MAX_SRAM_SIZE] = {0};
    MS_U16 UFSC_SRAM4_IHC[UFSC_MAX_SRAM_SIZE] = {0};

    MS_U16* u32Addr = (MS_U16*)pData;
    MS_U16 u16Index = 0 ;
    MS_U16 cont1=0, cont2=0, cont3=0, cont4=0;

    if(u8sramtype != SC_FILTER_SRAM_IHC_CRD)
    {
        return;
    }
//    printf("[%s]SRAM_IHC_TOTAL_COUNT=%u,SRAM2_IHC_OFFSET=%u,SRAM3_IHC_OFFSET=%u,SRAM4_IHC_OFFSET=%u\n",
//        __FUNCTION__,SRAM_IHC_TOTAL_COUNT,SRAM2_IHC_OFFSET,SRAM3_IHC_OFFSET,SRAM4_IHC_OFFSET);


        for (;u16Index < UFSC_SRAM_IHC_TOTAL_COUNT ; u16Index++)
        {
            if (u16Index < UFSC_SRAM2_IHC_OFFSET)
            {
                UFSC_SRAM1_IHC[cont1] = (u32Addr[u16Index]);
    //            printf("@@SRAM1_IHC[%u]=%x\n",cont1,(u32Addr[u16Index]));
                cont1++;
            }
            else if (u16Index < UFSC_SRAM3_IHC_OFFSET)
            {
                UFSC_SRAM2_IHC[cont2] = (u32Addr[u16Index]);
    //            printf("@@SRAM2_IHC[%u]=%x\n",cont2,(u32Addr[u16Index]));
                cont2++;

            }
            else if (u16Index < UFSC_SRAM4_IHC_OFFSET)
            {
                UFSC_SRAM3_IHC[cont3] = (u32Addr[u16Index]);
    //            printf("@@SRAM3_IHC[%u]=%x\n",cont3,(u32Addr[u16Index]));
                cont3++;
            }
            else
            {
                UFSC_SRAM4_IHC[cont4] = (u32Addr[u16Index]);
    //            printf("@@SRAM4_IHC[%u]=%x\n",cont4,(u32Addr[u16Index]));
                cont4++;
            }
        }

    _Hal_PQ_set_UFSC_sram_ihc_crd_table(&UFSC_SRAM1_IHC[0], 0, UFSC_SRAM1_IHC_COUNT);
    _Hal_PQ_set_UFSC_sram_ihc_crd_table(&UFSC_SRAM2_IHC[0], 1, UFSC_SRAM2_IHC_COUNT);
    _Hal_PQ_set_UFSC_sram_ihc_crd_table(&UFSC_SRAM3_IHC[0], 2, UFSC_SRAM3_IHC_COUNT);
    _Hal_PQ_set_UFSC_sram_ihc_crd_table(&UFSC_SRAM4_IHC[0], 3, UFSC_SRAM4_IHC_COUNT);
}

MS_BOOL Hal_PQ_get_cfd_info(MS_U16 u16PqIndex, MS_U8 *pu8ColorFormat, MS_U8 *pu8ColorDataFormat, MS_BOOL *pbIsFullRange, MS_U8 *pu8ColorPrimaries, MS_U8 *pu8TransferCharacteristics, MS_U8 *pu8MatrixCoefficients)
{
    if ((pu8ColorFormat == NULL) || (pu8ColorDataFormat == NULL) || (pbIsFullRange == NULL) || (pu8ColorPrimaries == NULL) || (pu8TransferCharacteristics == NULL) || (pu8MatrixCoefficients == NULL))
    {
        return FALSE;
    }

    *pu8ColorFormat = _gu8AnalogCfd[u16PqIndex][0];
    *pu8ColorDataFormat = _gu8AnalogCfd[u16PqIndex][1];
    *pbIsFullRange = _gu8AnalogCfd[u16PqIndex][2];
    *pu8ColorPrimaries = _gu8AnalogCfd[u16PqIndex][3];
    *pu8TransferCharacteristics = _gu8AnalogCfd[u16PqIndex][4];
    *pu8MatrixCoefficients = _gu8AnalogCfd[u16PqIndex][5];
    return TRUE;
}