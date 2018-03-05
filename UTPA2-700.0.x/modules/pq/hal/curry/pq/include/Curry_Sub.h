////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   Quality Map CodeGen Version 4.0
// 1920
// Kano
// 7/6/2016  2:42:44 PM
// 39630.43853
//****************************************************

#ifndef _KANO_SUB_H_
#define _KANO_SUB_H_

#define PQ_IP_NUM_Sub 74
#define PQ_IP_SRAM1_SIZE_Sub 1280
#define PQ_IP_SRAM2_SIZE_Sub 1280
#define PQ_IP_SRAM3_SIZE_Sub 1280
#define PQ_IP_SRAM4_SIZE_Sub 1280
#define PQ_IP_C_SRAM1_SIZE_Sub 320
#define PQ_IP_C_SRAM2_SIZE_Sub 320
#define PQ_IP_C_SRAM3_SIZE_Sub 320
#define PQ_IP_C_SRAM4_SIZE_Sub 320
#define PQ_IP_VE_HSD_SRAM_SIZE_Sub 0
#define PQ_IP_VE_VSD_SRAM_SIZE_Sub 0

typedef enum
{
    #if PQ_QM_CVBS
    QM_RF_NTSC_44_Sub, //0
    #endif
    #if PQ_QM_CVBS
    QM_RF_NTSC_M_Sub, //1
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_BGHI_Sub, //2
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_60_Sub, //3
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_M_Sub, //4
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_N_Sub, //5
    #endif
    #if PQ_QM_CVBS
    QM_RF_SECAM_Sub, //6
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_44_Sub, //7
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_M_Sub, //8
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_BGHI_Sub, //9
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_60_Sub, //10
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_M_Sub, //11
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_N_Sub, //12
    #endif
    #if PQ_QM_CVBS
    QM_VIF_SECAM_Sub, //13
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_44_Sub, //14
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_M_Sub, //15
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_BGHI_Sub, //16
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_60_Sub, //17
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_M_Sub, //18
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_N_Sub, //19
    #endif
    #if PQ_QM_CVBS
    QM_SV_SECAM_Sub, //20
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_44_Sub, //21
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_M_Sub, //22
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_BGHI_Sub, //23
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_60_Sub, //24
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_M_Sub, //25
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_N_Sub, //26
    #endif
    #if PQ_QM_CVBS
    QM_AV_SECAM_Sub, //27
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_44_Sub, //28
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_M_Sub, //29
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_BGHI_Sub, //30
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_60_Sub, //31
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_M_Sub, //32
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_N_Sub, //33
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_SECAM_Sub, //34
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_44_Sub, //35
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_M_Sub, //36
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_BGHI_Sub, //37
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_60_Sub, //38
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_M_Sub, //39
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_N_Sub, //40
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_SECAM_Sub, //41
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_NTSC_Sub, //42
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_PAL_Sub, //43
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Sub, //44
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Sub, //45
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Sub, //46
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Sub, //47
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Sub, //48
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Sub, //49
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Sub, //50
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Sub, //51
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Sub, //52
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Sub, //53
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Sub, //54
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Sub, //55
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Sub, //56
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Sub, //57
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Sub, //58
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Sub, //59
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Sub, //60
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Sub, //61
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Sub, //62
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Sub, //63
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Sub, //64
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Sub, //65
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Sub, //66
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Sub, //67
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_24Hz_Sub, //68
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_30Hz_Sub, //69
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_60Hz_Sub, //70
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Sub, //71
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Sub, //72
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Sub, //73
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Sub, //74
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Sub, //75
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Sub, //76
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Sub, //77
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Sub, //78
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Sub, //79
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_4K_Sub, //80
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Sub, //81
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Sub, //82
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Sub, //83
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Sub, //84
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Sub, //85
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Sub, //86
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Sub, //87
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Sub, //88
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Sub, //89
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Sub, //90
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Sub, //91
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Sub, //92
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Sub, //93
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Sub, //94
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Sub, //95
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Sub, //96
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Sub, //97
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Sub, //98
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_4K_Sub, //99
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_MPEG2_Sub, //100
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_MPEG2_Sub, //101
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_MPEG2_Sub, //102
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_MPEG2_Sub, //103
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_MPEG2_Sub, //104
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_MPEG2_Sub, //105
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_MPEG2_Sub, //106
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_MPEG2_Sub, //107
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_MPEG2_Sub, //108
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_MPEG2_Sub, //109
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_MPEG2_Sub, //110
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_MPEG2_Sub, //111
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_MPEG2_Sub, //112
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_MPEG2_Sub, //113
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_MPEG2_Sub, //114
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_H264_Sub, //115
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_H264_Sub, //116
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_H264_Sub, //117
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_H264_Sub, //118
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_H264_Sub, //119
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_H264_Sub, //120
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_H264_Sub, //121
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_H264_Sub, //122
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_H264_Sub, //123
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_H264_Sub, //124
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_H264_Sub, //125
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_H264_Sub, //126
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_H264_Sub, //127
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H264_Sub, //128
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_H264_Sub, //129
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H265_Sub, //130
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_interlace_Sub, //131
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_Sub, //132
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_24hz_Sub, //133
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_interlace_Sub, //134
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_Sub, //135
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_24hz_Sub, //136
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_4K2K_FS_progressive_Sub, //137
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_SD_progressive_Sub, //138
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_HD_progressive_Sub, //139
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_Sub, //140
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_Photo_Sub, //141
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_60Hz_Sub, //142
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_non_4K2K_Sub, //143
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_interlace_Sub, //144
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_progressive_Sub, //145
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_interlace_Sub, //146
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_progressive_Sub, //147
    #endif
    QM_INPUTTYPE_NUM_Sub, // 148
} SC_QUALITY_MAP_INDEX_e_Sub;

typedef enum
{
PQ_IP_AFEC_Sub,  //0
PQ_IP_Comb_Sub,  //1
PQ_IP_SECAM_Sub,  //2
PQ_IP_SCinit_Sub,  //3
PQ_IP_CSC_Sub,  //4
PQ_IP_CSC_Dither_Sub,  //5
PQ_IP_IP422To444_Sub,  //6
PQ_IP_HSD_Y_Sub,  //7
PQ_IP_HSD_C_Sub,  //8
PQ_IP_VSD_Sub,  //9
PQ_IP_MemFormat_Sub,  //10
PQ_IP_444To422_Sub,  //11
PQ_IP_PreSNR_Sub,  //12
PQ_IP_DNR_Sub,  //13
PQ_IP_DNR_YOnly_Sub,  //14
PQ_IP_DNR_Y_Sub,  //15
PQ_IP_DNR_MED_Sub,  //16
PQ_IP_DNR_C_Sub,  //17
PQ_IP_DNR_sticky_solver_Sub,  //18
PQ_IP_PostCCS_Sub,  //19
PQ_IP_PNR_Sub,  //20
PQ_IP_PNR_Y_Sub,  //21
PQ_IP_PNR_C_Sub,  //22
PQ_IP_420CUP_Sub,  //23
PQ_IP_MADi_Sub,  //24
PQ_IP_MADi_Motion_Sub,  //25
PQ_IP_EODi_Sub,  //26
PQ_IP_Film_Sub,  //27
PQ_IP_Film32_Sub,  //28
PQ_IP_Film22_Sub,  //29
PQ_IP_Film_any_Sub,  //30
PQ_IP_SPF_Sub,  //31
PQ_IP_SPF_DBK_Sub,  //32
PQ_IP_SPF_DBK_MR_Sub,  //33
PQ_IP_SPF_DBK_BKN_Sub,  //34
PQ_IP_SPF_SNR_Sub,  //35
PQ_IP_SPF_SNR_MR_Sub,  //36
PQ_IP_SPF_MR_LPF_Sub,  //37
PQ_IP_SPF_NMR_Y_Sub,  //38
PQ_IP_SPF_NMR_Y_MR_Sub,  //39
PQ_IP_SPF_NMR_C_Sub,  //40
PQ_IP_DMS_Sub,  //41
PQ_IP_DMS_H_Sub,  //42
PQ_IP_DMS_V_Sub,  //43
PQ_IP_DMS_V_12L_Sub,  //44
PQ_IP_VSP_Y_Sub,  //45
PQ_IP_VSP_C_Sub,  //46
PQ_IP_VSP_Coring_Sub,  //47
PQ_IP_VSP_Dither_Sub,  //48
PQ_IP_VSP_PreVBound_Sub,  //49
PQ_IP_PreCTI_Sub,  //50
PQ_IP_HSP_Y_Sub,  //51
PQ_IP_HSP_C_Sub,  //52
PQ_IP_HSP_Coring_Sub,  //53
PQ_IP_HSP_DeRing_Sub,  //54
PQ_IP_HSP_Dither_Sub,  //55
PQ_IP_HnonLinear_Sub,  //56
PQ_IP_VE_HVSD_Sub,  //57
PQ_IP_SRAM1_Sub,  //58
PQ_IP_SRAM2_Sub,  //59
PQ_IP_SRAM3_Sub,  //60
PQ_IP_SRAM4_Sub,  //61
PQ_IP_C_SRAM1_Sub,  //62
PQ_IP_C_SRAM2_Sub,  //63
PQ_IP_C_SRAM3_Sub,  //64
PQ_IP_C_SRAM4_Sub,  //65
PQ_IP_VE_HSD_SRAM_Sub,  //66
PQ_IP_VE_VSD_SRAM_Sub,  //67
PQ_IP_422To444_Sub,  //68
PQ_IP_Peaking_Sub,  //69
PQ_IP_SwDriver_Sub,  //70
PQ_IP_3x3_Sub,  //71
PQ_IP_BRI_CR_Sub,  //72
PQ_IP_Display_Sub,  //73
 }   PQ_IPTYPE_Sub;

typedef enum
{
PQ_IP_AFEC_COM_Sub,  //0
PQ_IP_Comb_COM_Sub,  //1
PQ_IP_SECAM_COM_Sub,  //2
PQ_IP_SCinit_COM_Sub,  //3
PQ_IP_CSC_COM_Sub,  //4
PQ_IP_CSC_Dither_COM_Sub,  //5
PQ_IP_IP422To444_COM_Sub,  //6
PQ_IP_HSD_Y_COM_Sub,  //7
PQ_IP_HSD_C_COM_Sub,  //8
PQ_IP_VSD_COM_Sub,  //9
PQ_IP_MemFormat_COM_Sub,  //10
PQ_IP_444To422_COM_Sub,  //11
PQ_IP_PreSNR_COM_Sub,  //12
PQ_IP_DNR_COM_Sub,  //13
PQ_IP_DNR_YOnly_COM_Sub,  //14
PQ_IP_DNR_Y_COM_Sub,  //15
PQ_IP_DNR_MED_COM_Sub,  //16
PQ_IP_DNR_C_COM_Sub,  //17
PQ_IP_DNR_sticky_solver_COM_Sub,  //18
PQ_IP_PostCCS_COM_Sub,  //19
PQ_IP_PNR_COM_Sub,  //20
PQ_IP_PNR_Y_COM_Sub,  //21
PQ_IP_PNR_C_COM_Sub,  //22
PQ_IP_420CUP_COM_Sub,  //23
PQ_IP_MADi_COM_Sub,  //24
PQ_IP_MADi_Motion_COM_Sub,  //25
PQ_IP_EODi_COM_Sub,  //26
PQ_IP_Film_COM_Sub,  //27
PQ_IP_Film32_COM_Sub,  //28
PQ_IP_Film22_COM_Sub,  //29
PQ_IP_Film_any_COM_Sub,  //30
PQ_IP_SPF_COM_Sub,  //31
PQ_IP_SPF_DBK_COM_Sub,  //32
PQ_IP_SPF_DBK_MR_COM_Sub,  //33
PQ_IP_SPF_DBK_BKN_COM_Sub,  //34
PQ_IP_SPF_SNR_COM_Sub,  //35
PQ_IP_SPF_SNR_MR_COM_Sub,  //36
PQ_IP_SPF_MR_LPF_COM_Sub,  //37
PQ_IP_SPF_NMR_Y_COM_Sub,  //38
PQ_IP_SPF_NMR_Y_MR_COM_Sub,  //39
PQ_IP_SPF_NMR_C_COM_Sub,  //40
PQ_IP_DMS_COM_Sub,  //41
PQ_IP_DMS_H_COM_Sub,  //42
PQ_IP_DMS_V_COM_Sub,  //43
PQ_IP_DMS_V_12L_COM_Sub,  //44
PQ_IP_VSP_Y_COM_Sub,  //45
PQ_IP_VSP_C_COM_Sub,  //46
PQ_IP_VSP_Coring_COM_Sub,  //47
PQ_IP_VSP_Dither_COM_Sub,  //48
PQ_IP_VSP_PreVBound_COM_Sub,  //49
PQ_IP_PreCTI_COM_Sub,  //50
PQ_IP_HSP_Y_COM_Sub,  //51
PQ_IP_HSP_C_COM_Sub,  //52
PQ_IP_HSP_Coring_COM_Sub,  //53
PQ_IP_HSP_DeRing_COM_Sub,  //54
PQ_IP_HSP_Dither_COM_Sub,  //55
PQ_IP_HnonLinear_COM_Sub,  //56
PQ_IP_VE_HVSD_COM_Sub,  //57
PQ_IP_SRAM1_COM_Sub,  //58
PQ_IP_SRAM2_COM_Sub,  //59
PQ_IP_SRAM3_COM_Sub,  //60
PQ_IP_SRAM4_COM_Sub,  //61
PQ_IP_C_SRAM1_COM_Sub,  //62
PQ_IP_C_SRAM2_COM_Sub,  //63
PQ_IP_C_SRAM3_COM_Sub,  //64
PQ_IP_C_SRAM4_COM_Sub,  //65
PQ_IP_VE_HSD_SRAM_COM_Sub,  //66
PQ_IP_VE_VSD_SRAM_COM_Sub,  //67
PQ_IP_422To444_COM_Sub,  //68
PQ_IP_Peaking_COM_Sub,  //69
PQ_IP_SwDriver_COM_Sub,  //70
PQ_IP_3x3_COM_Sub,  //71
PQ_IP_BRI_CR_COM_Sub,  //72
PQ_IP_Display_COM_Sub,  //73
PQ_IP_COM_NUMS_Sub
} PQ_IP_COM_Group_Sub;

typedef enum
{
PQ_IP_AFEC_ADCIN_Sub,
PQ_IP_AFEC_RFIN_Sub,
PQ_IP_AFEC_ADCIN_SECAM_Sub,
PQ_IP_AFEC_NUMS_Sub
} PQ_IP_AFEC_Group_Sub;

typedef enum
{
PQ_IP_Comb_NTSC_AV_Sub,
PQ_IP_Comb_PAL_AV_Sub,
PQ_IP_Comb_NTSC_RF_Sub,
PQ_IP_Comb_PAL_RF_Sub,
PQ_IP_Comb_NTSC_SV_Sub,
PQ_IP_Comb_PAL_SV_Sub,
PQ_IP_Comb_NTSC443_Sub,
PQ_IP_Comb_PAL60_Sub,
PQ_IP_Comb_SECAM_Sub,
PQ_IP_Comb_NUMS_Sub
} PQ_IP_Comb_Group_Sub;

typedef enum
{
PQ_IP_SECAM_ALL_Sub,
PQ_IP_SECAM_NUMS_Sub
} PQ_IP_SECAM_Group_Sub;

typedef enum
{
PQ_IP_SCinit_Init_Sub,
PQ_IP_SCinit_NUMS_Sub
} PQ_IP_SCinit_Group_Sub;

typedef enum
{
PQ_IP_CSC_OFF_Sub,
PQ_IP_CSC_L_RGB2YCC_SD_Sub,
PQ_IP_CSC_F_RGB2YCC_SD_Sub,
PQ_IP_CSC_L_RGB2YCC_HD_Sub,
PQ_IP_CSC_F_RGB2YCC_HD_Sub,
PQ_IP_CSC_BT2020_NCL_Sub,
PQ_IP_CSC_NUMS_Sub
} PQ_IP_CSC_Group_Sub;

typedef enum
{
PQ_IP_CSC_Dither_OFF_Sub,
PQ_IP_CSC_Dither_L_RGB2YCC_SD_Sub,
PQ_IP_CSC_Dither_F_RGB2YCC_SD_Sub,
PQ_IP_CSC_Dither_L_RGB2YCC_HD_Sub,
PQ_IP_CSC_Dither_F_RGB2YCC_HD_Sub,
PQ_IP_CSC_Dither_BT2020_NCL_Sub,
PQ_IP_CSC_Dither_NUMS_Sub
} PQ_IP_CSC_Dither_Group_Sub;

typedef enum
{
PQ_IP_IP422To444_OFF_Sub,
PQ_IP_IP422To444_ON_Sub,
PQ_IP_IP422To444_Bypass_Sub,
PQ_IP_IP422To444_NUMS_Sub
} PQ_IP_IP422To444_Group_Sub;

typedef enum
{
PQ_IP_HSD_Y_OFF_Sub,
PQ_IP_HSD_Y_CB_Sub,
PQ_IP_HSD_Y_NUMS_Sub
} PQ_IP_HSD_Y_Group_Sub;

typedef enum
{
PQ_IP_HSD_C_OFF_Sub,
PQ_IP_HSD_C_CB_Sub,
PQ_IP_HSD_C_NUMS_Sub
} PQ_IP_HSD_C_Group_Sub;

typedef enum
{
PQ_IP_VSD_OFF_Sub,
PQ_IP_VSD_CB_Sub,
PQ_IP_VSD_Bilinear_Sub,
PQ_IP_VSD_NUMS_Sub
} PQ_IP_VSD_Group_Sub;

typedef enum
{
PQ_IP_MemFormat_422MF_Sub,
PQ_IP_MemFormat_444_10BIT_Sub,
PQ_IP_MemFormat_444_8BIT_Sub,
PQ_IP_MemFormat_422FBL_10BIT_Sub,
PQ_IP_MemFormat_444FBL_10BIT_Sub,
PQ_IP_MemFormat_444FBL_8BIT_Sub,
PQ_IP_MemFormat_NUMS_Sub
} PQ_IP_MemFormat_Group_Sub;

typedef enum
{
PQ_IP_444To422_OFF_Sub,
PQ_IP_444To422_ON_Sub,
PQ_IP_444To422_NUMS_Sub
} PQ_IP_444To422_Group_Sub;

typedef enum
{
PQ_IP_PreSNR_OFF_Sub,
PQ_IP_PreSNR_PS_1_Sub,
PQ_IP_PreSNR_PS_2_Sub,
PQ_IP_PreSNR_PS_3_Sub,
PQ_IP_PreSNR_PS_4_Sub,
PQ_IP_PreSNR_PS_5_Sub,
PQ_IP_PreSNR_PS_6_Sub,
PQ_IP_PreSNR_PS_7_Sub,
PQ_IP_PreSNR_PS_8_Sub,
PQ_IP_PreSNR_PS_9_Sub,
PQ_IP_PreSNR_PS_10_Sub,
PQ_IP_PreSNR_PS_11_Sub,
PQ_IP_PreSNR_PS_12_Sub,
PQ_IP_PreSNR_PS_13_Sub,
PQ_IP_PreSNR_PS_14_Sub,
PQ_IP_PreSNR_PS_15_Sub,
PQ_IP_PreSNR_PS_16_Sub,
PQ_IP_PreSNR_NUMS_Sub
} PQ_IP_PreSNR_Group_Sub;

typedef enum
{
PQ_IP_DNR_NR_OFF_Sub,
PQ_IP_DNR_DNR_OFF_Sub,
PQ_IP_DNR_ON_Sub,
PQ_IP_DNR_ON_PT_Sub,
PQ_IP_DNR_NUMS_Sub
} PQ_IP_DNR_Group_Sub;

typedef enum
{
PQ_IP_DNR_YOnly_OFF_Sub,
PQ_IP_DNR_YOnly_ON_Sub,
PQ_IP_DNR_YOnly_NUMS_Sub
} PQ_IP_DNR_YOnly_Group_Sub;

typedef enum
{
PQ_IP_DNR_Y_OFF_Sub,
PQ_IP_DNR_Y_DY_1_Sub,
PQ_IP_DNR_Y_DY_1_1_Sub,
PQ_IP_DNR_Y_DY_2_Sub,
PQ_IP_DNR_Y_DY_2_1_Sub,
PQ_IP_DNR_Y_DY_3_Sub,
PQ_IP_DNR_Y_DY_3_1_Sub,
PQ_IP_DNR_Y_DY_4_Sub,
PQ_IP_DNR_Y_DY_4_1_Sub,
PQ_IP_DNR_Y_DY_5_Sub,
PQ_IP_DNR_Y_DY_5_1_Sub,
PQ_IP_DNR_Y_DY_6_Sub,
PQ_IP_DNR_Y_DY_6_1_Sub,
PQ_IP_DNR_Y_DY_7_Sub,
PQ_IP_DNR_Y_DY_7_1_Sub,
PQ_IP_DNR_Y_DY_8_Sub,
PQ_IP_DNR_Y_DY_8_1_Sub,
PQ_IP_DNR_Y_DY_9_Sub,
PQ_IP_DNR_Y_DY_9_1_Sub,
PQ_IP_DNR_Y_DY_10_Sub,
PQ_IP_DNR_Y_DY_10_1_Sub,
PQ_IP_DNR_Y_DY_nonlinear0_Sub,
PQ_IP_DNR_Y_DY_nonlinear01_Sub,
PQ_IP_DNR_Y_DY_nonlinear1_Sub,
PQ_IP_DNR_Y_DY_nonlinear12_Sub,
PQ_IP_DNR_Y_DY_nonlinear2_Sub,
PQ_IP_DNR_Y_DY_nonlinear23_Sub,
PQ_IP_DNR_Y_DY_nonlinear3_Sub,
PQ_IP_DNR_Y_U1_Sub,
PQ_IP_DNR_Y_U2_Sub,
PQ_IP_DNR_Y_U3_Sub,
PQ_IP_DNR_Y_U4_Sub,
PQ_IP_DNR_Y_U5_Sub,
PQ_IP_DNR_Y_U6_Sub,
PQ_IP_DNR_Y_U7_Sub,
PQ_IP_DNR_Y_U8_Sub,
PQ_IP_DNR_Y_NUMS_Sub
} PQ_IP_DNR_Y_Group_Sub;

typedef enum
{
PQ_IP_DNR_MED_OFF_Sub,
PQ_IP_DNR_MED_M_1_Sub,
PQ_IP_DNR_MED_NUMS_Sub
} PQ_IP_DNR_MED_Group_Sub;

typedef enum
{
PQ_IP_DNR_C_OFF_Sub,
PQ_IP_DNR_C_DC_1_Sub,
PQ_IP_DNR_C_DC_1_1_Sub,
PQ_IP_DNR_C_DC_2_Sub,
PQ_IP_DNR_C_DC_2_1_Sub,
PQ_IP_DNR_C_DC_3_Sub,
PQ_IP_DNR_C_DC_3_1_Sub,
PQ_IP_DNR_C_DC_4_Sub,
PQ_IP_DNR_C_DC_4_1_Sub,
PQ_IP_DNR_C_DC_5_Sub,
PQ_IP_DNR_C_DC_5_1_Sub,
PQ_IP_DNR_C_DC_6_Sub,
PQ_IP_DNR_C_DC_6_1_Sub,
PQ_IP_DNR_C_DC_7_Sub,
PQ_IP_DNR_C_DC_7_1_Sub,
PQ_IP_DNR_C_DC_8_Sub,
PQ_IP_DNR_C_DC_8_1_Sub,
PQ_IP_DNR_C_DC_9_Sub,
PQ_IP_DNR_C_DC_9_1_Sub,
PQ_IP_DNR_C_DC_10_Sub,
PQ_IP_DNR_C_DC_10_1_Sub,
PQ_IP_DNR_C_DC_nonlinear1_Sub,
PQ_IP_DNR_C_DC_nonlinear2_Sub,
PQ_IP_DNR_C_DC_nonlinear3_Sub,
PQ_IP_DNR_C_U1_Sub,
PQ_IP_DNR_C_U2_Sub,
PQ_IP_DNR_C_U3_Sub,
PQ_IP_DNR_C_U4_Sub,
PQ_IP_DNR_C_U5_Sub,
PQ_IP_DNR_C_U6_Sub,
PQ_IP_DNR_C_U7_Sub,
PQ_IP_DNR_C_U8_Sub,
PQ_IP_DNR_C_NUMS_Sub
} PQ_IP_DNR_C_Group_Sub;

typedef enum
{
PQ_IP_DNR_sticky_solver_OFF_Sub,
PQ_IP_DNR_sticky_solver_8bit_Sub,
PQ_IP_DNR_sticky_solver_6bit_Sub,
PQ_IP_DNR_sticky_solver_No_Comp_Sub,
PQ_IP_DNR_sticky_solver_ori_round_dither_Sub,
PQ_IP_DNR_sticky_solver_NUMS_Sub
} PQ_IP_DNR_sticky_solver_Group_Sub;

typedef enum
{
PQ_IP_PostCCS_OFF_Sub,
PQ_IP_PostCCS_PC_0_Sub,
PQ_IP_PostCCS_PC_1_Sub,
PQ_IP_PostCCS_PC_2_Sub,
PQ_IP_PostCCS_PC_3_Sub,
PQ_IP_PostCCS_PC_4_Sub,
PQ_IP_PostCCS_PC_5_Sub,
PQ_IP_PostCCS_NUMS_Sub
} PQ_IP_PostCCS_Group_Sub;

typedef enum
{
PQ_IP_PNR_OFF_Sub,
PQ_IP_PNR_ON_Sub,
PQ_IP_PNR_AVG_ON_Sub,
PQ_IP_PNR_NUMS_Sub
} PQ_IP_PNR_Group_Sub;

typedef enum
{
PQ_IP_PNR_Y_OFF_Sub,
PQ_IP_PNR_Y_PY1_Sub,
PQ_IP_PNR_Y_NUMS_Sub
} PQ_IP_PNR_Y_Group_Sub;

typedef enum
{
PQ_IP_PNR_C_OFF_Sub,
PQ_IP_PNR_C_PC1_Sub,
PQ_IP_PNR_C_NUMS_Sub
} PQ_IP_PNR_C_Group_Sub;

typedef enum
{
PQ_IP_420CUP_OFF_Sub,
PQ_IP_420CUP_ON_Sub,
PQ_IP_420CUP_NUMS_Sub
} PQ_IP_420CUP_Group_Sub;

typedef enum
{
PQ_IP_MADi_25_4R_Sub,
PQ_IP_MADi_24_4R_Sub,
PQ_IP_MADi_25_2R_Sub,
PQ_IP_MADi_24_2R_Sub,
PQ_IP_MADi_27_4R_Sub,
PQ_IP_MADi_27_2R_Sub,
PQ_IP_MADi_27_2R_Y8Only_Sub,
PQ_IP_MADi_FBL_EODi_Sub,
PQ_IP_MADi_P_MODE10_Sub,
PQ_IP_MADi_P_MODE8_Sub,
PQ_IP_MADi_24_RFBL_NFilm_Sub,
PQ_IP_MADi_24_RFBL_Film_Sub,
PQ_IP_MADi_FBL_DNR_Sub,
PQ_IP_MADi_FBL_MIU_Sub,
PQ_IP_MADi_P_MODE8_NO_MIU_Sub,
PQ_IP_MADi_P_MODE8_3Frame_Sub,
PQ_IP_MADi_NUMS_Sub
} PQ_IP_MADi_Group_Sub;

typedef enum
{
PQ_IP_MADi_Motion_MOT_4R_5_Sub,
PQ_IP_MADi_Motion_MOT_4R_6_Sub,
PQ_IP_MADi_Motion_MOT_4R_7_Sub,
PQ_IP_MADi_Motion_MOT_2R_Sub,
PQ_IP_MADi_Motion_MOT_PMODE_Sub,
PQ_IP_MADi_Motion_NUMS_Sub
} PQ_IP_MADi_Motion_Group_Sub;

typedef enum
{
PQ_IP_EODi_OFF_Sub,
PQ_IP_EODi_SD_Sub,
PQ_IP_EODi_NUMS_Sub
} PQ_IP_EODi_Group_Sub;

typedef enum
{
PQ_IP_Film_OFF_Sub,
PQ_IP_Film_SD_2_Sub,
PQ_IP_Film_SD_1_Sub,
PQ_IP_Film_SD_3_Sub,
PQ_IP_Film_HD_2_Sub,
PQ_IP_Film_HD_1_Sub,
PQ_IP_Film_HD_3_Sub,
PQ_IP_Film_NUMS_Sub
} PQ_IP_Film_Group_Sub;

typedef enum
{
PQ_IP_Film32_OFF_Sub,
PQ_IP_Film32_SD_1_Sub,
PQ_IP_Film32_SD_2_Sub,
PQ_IP_Film32_SD_3_Sub,
PQ_IP_Film32_SD_4_Sub,
PQ_IP_Film32_SD_5_Sub,
PQ_IP_Film32_SD_6_Sub,
PQ_IP_Film32_SD_7_Sub,
PQ_IP_Film32_SD_8_Sub,
PQ_IP_Film32_SD_9_Sub,
PQ_IP_Film32_SD_10_Sub,
PQ_IP_Film32_HD_1_Sub,
PQ_IP_Film32_HD_2_Sub,
PQ_IP_Film32_HD_3_Sub,
PQ_IP_Film32_HD_4_Sub,
PQ_IP_Film32_HD_5_Sub,
PQ_IP_Film32_HD_6_Sub,
PQ_IP_Film32_NUMS_Sub
} PQ_IP_Film32_Group_Sub;

typedef enum
{
PQ_IP_Film22_OFF_Sub,
PQ_IP_Film22_SD_1_Sub,
PQ_IP_Film22_SD_2_Sub,
PQ_IP_Film22_SD_3_Sub,
PQ_IP_Film22_SD_4_Sub,
PQ_IP_Film22_SD_5_Sub,
PQ_IP_Film22_SD_6_Sub,
PQ_IP_Film22_SD_7_Sub,
PQ_IP_Film22_SD_8_Sub,
PQ_IP_Film22_SD_9_Sub,
PQ_IP_Film22_SD_10_Sub,
PQ_IP_Film22_HD_1_Sub,
PQ_IP_Film22_HD_2_Sub,
PQ_IP_Film22_HD_3_Sub,
PQ_IP_Film22_HD_4_Sub,
PQ_IP_Film22_HD_5_Sub,
PQ_IP_Film22_HD_6_Sub,
PQ_IP_Film22_NUMS_Sub
} PQ_IP_Film22_Group_Sub;

typedef enum
{
PQ_IP_Film_any_OFF_Sub,
PQ_IP_Film_any_SD_1_Sub,
PQ_IP_Film_any_SD_2_Sub,
PQ_IP_Film_any_SD_3_Sub,
PQ_IP_Film_any_SD_4_Sub,
PQ_IP_Film_any_SD_5_Sub,
PQ_IP_Film_any_SD_6_Sub,
PQ_IP_Film_any_SD_7_Sub,
PQ_IP_Film_any_SD_8_Sub,
PQ_IP_Film_any_SD_9_Sub,
PQ_IP_Film_any_SD_10_Sub,
PQ_IP_Film_any_HD_1_Sub,
PQ_IP_Film_any_HD_2_Sub,
PQ_IP_Film_any_HD_3_Sub,
PQ_IP_Film_any_HD_4_Sub,
PQ_IP_Film_any_HD_5_Sub,
PQ_IP_Film_any_HD_6_Sub,
PQ_IP_Film_any_NUMS_Sub
} PQ_IP_Film_any_Group_Sub;

typedef enum
{
PQ_IP_SPF_OFF_Sub,
PQ_IP_SPF_ON_Sub,
PQ_IP_SPF_NUMS_Sub
} PQ_IP_SPF_Group_Sub;

typedef enum
{
PQ_IP_SPF_DBK_OFF_Sub,
PQ_IP_SPF_DBK_E2S2_Sub,
PQ_IP_SPF_DBK_NUMS_Sub
} PQ_IP_SPF_DBK_Group_Sub;

typedef enum
{
PQ_IP_SPF_DBK_MR_OFF_Sub,
PQ_IP_SPF_DBK_MR_ON_Sub,
PQ_IP_SPF_DBK_MR_NUMS_Sub
} PQ_IP_SPF_DBK_MR_Group_Sub;

typedef enum
{
PQ_IP_SPF_DBK_BKN_OFF_Sub,
PQ_IP_SPF_DBK_BKN_T0_Sub,
PQ_IP_SPF_DBK_BKN_T1_Sub,
PQ_IP_SPF_DBK_BKN_T2_Sub,
PQ_IP_SPF_DBK_BKN_AV_Sub,
PQ_IP_SPF_DBK_BKN_2xsampling_Sub,
PQ_IP_SPF_DBK_BKN_NUMS_Sub
} PQ_IP_SPF_DBK_BKN_Group_Sub;

typedef enum
{
PQ_IP_SPF_SNR_OFF_Sub,
PQ_IP_SPF_SNR_E1S1_Sub,
PQ_IP_SPF_SNR_E1S2_Sub,
PQ_IP_SPF_SNR_E2S1_Sub,
PQ_IP_SPF_SNR_E3S1_Sub,
PQ_IP_SPF_SNR_E4S1_Sub,
PQ_IP_SPF_SNR_E4S2_Sub,
PQ_IP_SPF_SNR_E4S3_Sub,
PQ_IP_SPF_SNR_SEC1_Sub,
PQ_IP_SPF_SNR_SEC2_Sub,
PQ_IP_SPF_SNR_SEC3_Sub,
PQ_IP_SPF_SNR_NUMS_Sub
} PQ_IP_SPF_SNR_Group_Sub;

typedef enum
{
PQ_IP_SPF_SNR_MR_OFF_Sub,
PQ_IP_SPF_SNR_MR_ON_Sub,
PQ_IP_SPF_SNR_MR_NUMS_Sub
} PQ_IP_SPF_SNR_MR_Group_Sub;

typedef enum
{
PQ_IP_SPF_MR_LPF_OFF_Sub,
PQ_IP_SPF_MR_LPF_LPF3x3_Sub,
PQ_IP_SPF_MR_LPF_NUMS_Sub
} PQ_IP_SPF_MR_LPF_Group_Sub;

typedef enum
{
PQ_IP_SPF_NMR_Y_OFF_Sub,
PQ_IP_SPF_NMR_Y_S0_Sub,
PQ_IP_SPF_NMR_Y_S1_Sub,
PQ_IP_SPF_NMR_Y_S2_Sub,
PQ_IP_SPF_NMR_Y_S3_Sub,
PQ_IP_SPF_NMR_Y_NUMS_Sub
} PQ_IP_SPF_NMR_Y_Group_Sub;

typedef enum
{
PQ_IP_SPF_NMR_Y_MR_OFF_Sub,
PQ_IP_SPF_NMR_Y_MR_ON_Sub,
PQ_IP_SPF_NMR_Y_MR_NUMS_Sub
} PQ_IP_SPF_NMR_Y_MR_Group_Sub;

typedef enum
{
PQ_IP_SPF_NMR_C_OFF_Sub,
PQ_IP_SPF_NMR_C_S1_Sub,
PQ_IP_SPF_NMR_C_S2_Sub,
PQ_IP_SPF_NMR_C_S3_Sub,
PQ_IP_SPF_NMR_C_NUMS_Sub
} PQ_IP_SPF_NMR_C_Group_Sub;

typedef enum
{
PQ_IP_DMS_OFF_Sub,
PQ_IP_DMS_ON_Sub,
PQ_IP_DMS_NUMS_Sub
} PQ_IP_DMS_Group_Sub;

typedef enum
{
PQ_IP_DMS_H_OFF_Sub,
PQ_IP_DMS_H_S1_Sub,
PQ_IP_DMS_H_S2_Sub,
PQ_IP_DMS_H_S3_Sub,
PQ_IP_DMS_H_NUMS_Sub
} PQ_IP_DMS_H_Group_Sub;

typedef enum
{
PQ_IP_DMS_V_OFF_Sub,
PQ_IP_DMS_V_S1_Sub,
PQ_IP_DMS_V_S2_Sub,
PQ_IP_DMS_V_NUMS_Sub
} PQ_IP_DMS_V_Group_Sub;

typedef enum
{
PQ_IP_DMS_V_12L_OFF_Sub,
PQ_IP_DMS_V_12L_ON_Sub,
PQ_IP_DMS_V_12L_NUMS_Sub
} PQ_IP_DMS_V_12L_Group_Sub;

typedef enum
{
PQ_IP_VSP_Y_Bypass_Sub,
PQ_IP_VSP_Y_Bilinear_Sub,
PQ_IP_VSP_Y_SRAM_3_4Tap_Sub,
PQ_IP_VSP_Y_SRAM_4_4Tap_Sub,
PQ_IP_VSP_Y_SRAM_3_6Tap_Sub,
PQ_IP_VSP_Y_SRAM_4_6Tap_Sub,
PQ_IP_VSP_Y_SRAM_3_8Tap_Sub,
PQ_IP_VSP_Y_SRAM_4_8Tap_Sub,
PQ_IP_VSP_Y_NUMS_Sub
} PQ_IP_VSP_Y_Group_Sub;

typedef enum
{
PQ_IP_VSP_C_Bypass_Sub,
PQ_IP_VSP_C_Bilinear_Sub,
PQ_IP_VSP_C_C_SRAM_1_Sub,
PQ_IP_VSP_C_C_SRAM_2_Sub,
PQ_IP_VSP_C_C_SRAM_3_Sub,
PQ_IP_VSP_C_C_SRAM_4_Sub,
PQ_IP_VSP_C_SRAM_3_4Tap_Sub,
PQ_IP_VSP_C_SRAM_4_4Tap_Sub,
PQ_IP_VSP_C_NUMS_Sub
} PQ_IP_VSP_C_Group_Sub;

typedef enum
{
PQ_IP_VSP_Coring_OFF_Sub,
PQ_IP_VSP_Coring_Y_Coring_1_Sub,
PQ_IP_VSP_Coring_Y_Coring_2_Sub,
PQ_IP_VSP_Coring_Y_Coring_3_Sub,
PQ_IP_VSP_Coring_Y_Coring_4_Sub,
PQ_IP_VSP_Coring_Y_Coring_5_Sub,
PQ_IP_VSP_Coring_NUMS_Sub
} PQ_IP_VSP_Coring_Group_Sub;

typedef enum
{
PQ_IP_VSP_Dither_OFF_Sub,
PQ_IP_VSP_Dither_ON_Sub,
PQ_IP_VSP_Dither_NUMS_Sub
} PQ_IP_VSP_Dither_Group_Sub;

typedef enum
{
PQ_IP_VSP_PreVBound_OFF_Sub,
PQ_IP_VSP_PreVBound_ON_Sub,
PQ_IP_VSP_PreVBound_NUMS_Sub
} PQ_IP_VSP_PreVBound_Group_Sub;

typedef enum
{
PQ_IP_PreCTI_OFF_Sub,
PQ_IP_PreCTI_CTI_N_0_Sub,
PQ_IP_PreCTI_CTI_P_0_Sub,
PQ_IP_PreCTI_CTI_P_S5_Sub,
PQ_IP_PreCTI_CTI_N_S5_Sub,
PQ_IP_PreCTI_CTI_P_S3_MS5_Sub,
PQ_IP_PreCTI_CTI_N_S2_MS5_Sub,
PQ_IP_PreCTI_CTI_S0_MS5_YREF04_Sub,
PQ_IP_PreCTI_CTI_S0_MS5_YREF08_Sub,
PQ_IP_PreCTI_CTI_S0_MS5_YREF0C_Sub,
PQ_IP_PreCTI_CTI_MS3_CLF_low_Sub,
PQ_IP_PreCTI_CTI_MS3_CLF_mid_Sub,
PQ_IP_PreCTI_CTI_MS3_CLF_high_Sub,
PQ_IP_PreCTI_NUMS_Sub
} PQ_IP_PreCTI_Group_Sub;

typedef enum
{
PQ_IP_HSP_Y_Bypass_Sub,
PQ_IP_HSP_Y_Bilinear_Sub,
PQ_IP_HSP_Y_SRAM_3_4Tap_Sub,
PQ_IP_HSP_Y_SRAM_4_4Tap_Sub,
PQ_IP_HSP_Y_SRAM_3_6Tap_Sub,
PQ_IP_HSP_Y_SRAM_4_6Tap_Sub,
PQ_IP_HSP_Y_SRAM_3_8Tap_Sub,
PQ_IP_HSP_Y_SRAM_4_8Tap_Sub,
PQ_IP_HSP_Y_NUMS_Sub
} PQ_IP_HSP_Y_Group_Sub;

typedef enum
{
PQ_IP_HSP_C_Bypass_Sub,
PQ_IP_HSP_C_Bilinear_Sub,
PQ_IP_HSP_C_C_SRAM_1_Sub,
PQ_IP_HSP_C_C_SRAM_2_Sub,
PQ_IP_HSP_C_C_SRAM_3_Sub,
PQ_IP_HSP_C_C_SRAM_4_Sub,
PQ_IP_HSP_C_SRAM_3_4Tap_Sub,
PQ_IP_HSP_C_SRAM_4_4Tap_Sub,
PQ_IP_HSP_C_NUMS_Sub
} PQ_IP_HSP_C_Group_Sub;

typedef enum
{
PQ_IP_HSP_Coring_OFF_Sub,
PQ_IP_HSP_Coring_Y_Coring_1_Sub,
PQ_IP_HSP_Coring_Y_Coring_2_Sub,
PQ_IP_HSP_Coring_Y_Coring_3_Sub,
PQ_IP_HSP_Coring_Y_Coring_4_Sub,
PQ_IP_HSP_Coring_Y_Coring_5_Sub,
PQ_IP_HSP_Coring_NUMS_Sub
} PQ_IP_HSP_Coring_Group_Sub;

typedef enum
{
PQ_IP_HSP_DeRing_OFF_Sub,
PQ_IP_HSP_DeRing_DR1_Sub,
PQ_IP_HSP_DeRing_NUMS_Sub
} PQ_IP_HSP_DeRing_Group_Sub;

typedef enum
{
PQ_IP_HSP_Dither_OFF_Sub,
PQ_IP_HSP_Dither_ON_Sub,
PQ_IP_HSP_Dither_NUMS_Sub
} PQ_IP_HSP_Dither_Group_Sub;

typedef enum
{
PQ_IP_HnonLinear_OFF_Sub,
PQ_IP_HnonLinear_H_1366_0_Sub,
PQ_IP_HnonLinear_H_1366_1_Sub,
PQ_IP_HnonLinear_H_1366_2_Sub,
PQ_IP_HnonLinear_H_1440_Sub,
PQ_IP_HnonLinear_H_1680_Sub,
PQ_IP_HnonLinear_H_1920_0_Sub,
PQ_IP_HnonLinear_H_1920_1_Sub,
PQ_IP_HnonLinear_H_1920_2_Sub,
PQ_IP_HnonLinear_H_3840_1_Sub,
PQ_IP_HnonLinear_H_3840_2_Sub,
PQ_IP_HnonLinear_H_3840_3_Sub,
PQ_IP_HnonLinear_H_3840_4_Sub,
PQ_IP_HnonLinear_NUMS_Sub
} PQ_IP_HnonLinear_Group_Sub;

typedef enum
{
PQ_IP_VE_HVSD_Bypass_Sub,
PQ_IP_VE_HVSD_Bilinear_Sub,
PQ_IP_VE_HVSD_SRAM_TABLE0_Sub,
PQ_IP_VE_HVSD_SRAM_TABLE1_Sub,
PQ_IP_VE_HVSD_NUMS_Sub
} PQ_IP_VE_HVSD_Group_Sub;

typedef enum
{
PQ_IP_SRAM1_InvSinc8Tc4p4Fc65Apass0001Astop60_Sub,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc75Apass0001Astop60_Sub,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc85Apass0001Astop60_Sub,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc95Apass0001Astop60_Sub,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc105Apass0001Astop60_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc57Apass01Astop60_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc67Apass01Astop60_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc77Apass0001Astop40_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc81Apass01Astop40_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc87Apass01Astop50_Sub,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc94Apass01Astop60_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc50Apass01Astop55_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc60Apass0001Astop40_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc65Apass0001Astop40_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Apass0001Astop40_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Apass01Astop50_Sub,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc90Apass0001Astop40_Sub,
PQ_IP_SRAM1_NUMS_Sub
} PQ_IP_SRAM1_Group_Sub;

typedef enum
{
PQ_IP_SRAM2_InvSinc8Tc4p4Fc65Apass0001Astop60_Sub,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc75Apass0001Astop60_Sub,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc85Apass0001Astop60_Sub,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc95Apass0001Astop60_Sub,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc105Apass0001Astop60_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc57Apass01Astop60_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc77Apass0001Astop40_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc81Apass01Astop40_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc87Apass01Astop50_Sub,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc50Apass01Astop55_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc60Apass0001Astop40_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc65Apass0001Astop40_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Apass0001Astop40_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Apass01Astop50_Sub,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc90Apass0001Astop40_Sub,
PQ_IP_SRAM2_NUMS_Sub
} PQ_IP_SRAM2_Group_Sub;

typedef enum
{
PQ_IP_SRAM3_InvSinc8Tc4p4Fc65Apass0001Astop60_Sub,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc75Apass0001Astop60_Sub,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc85Apass0001Astop60_Sub,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc95Apass0001Astop60_Sub,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc105Apass0001Astop60_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc57Apass01Astop60_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc67Apass01Astop60_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc77Apass0001Astop40_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc81Apass01Astop40_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc87Apass01Astop50_Sub,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc94Apass01Astop60_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc50Apass01Astop55_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc60Apass0001Astop40_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc65Apass0001Astop40_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc75Apass0001Astop40_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc85Apass01Astop50_Sub,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc90Apass0001Astop40_Sub,
PQ_IP_SRAM3_NUMS_Sub
} PQ_IP_SRAM3_Group_Sub;

typedef enum
{
PQ_IP_SRAM4_InvSinc8Tc4p4Fc65Apass0001Astop60_Sub,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc75Apass0001Astop60_Sub,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc85Apass0001Astop60_Sub,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc95Apass0001Astop60_Sub,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc105Apass0001Astop60_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc57Apass01Astop60_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc67Apass01Astop60_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc77Apass0001Astop40_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc81Apass01Astop40_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc87Apass01Astop50_Sub,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc94Apass01Astop60_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc50Apass01Astop55_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc60Apass0001Astop40_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc65Apass0001Astop40_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc75Apass0001Astop40_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc85Apass01Astop50_Sub,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc90Apass0001Astop40_Sub,
PQ_IP_SRAM4_NUMS_Sub
} PQ_IP_SRAM4_Group_Sub;

typedef enum
{
PQ_IP_C_SRAM1_C2121_Sub,
PQ_IP_C_SRAM1_C121_Sub,
PQ_IP_C_SRAM1_C161_Sub,
PQ_IP_C_SRAM1_C111_Sub,
PQ_IP_C_SRAM1_NUMS_Sub
} PQ_IP_C_SRAM1_Group_Sub;

typedef enum
{
PQ_IP_C_SRAM2_C2121_Sub,
PQ_IP_C_SRAM2_C121_Sub,
PQ_IP_C_SRAM2_C161_Sub,
PQ_IP_C_SRAM2_C111_Sub,
PQ_IP_C_SRAM2_NUMS_Sub
} PQ_IP_C_SRAM2_Group_Sub;

typedef enum
{
PQ_IP_C_SRAM3_C2121_Sub,
PQ_IP_C_SRAM3_C121_Sub,
PQ_IP_C_SRAM3_C161_Sub,
PQ_IP_C_SRAM3_C111_Sub,
PQ_IP_C_SRAM3_NUMS_Sub
} PQ_IP_C_SRAM3_Group_Sub;

typedef enum
{
PQ_IP_C_SRAM4_C2121_Sub,
PQ_IP_C_SRAM4_C121_Sub,
PQ_IP_C_SRAM4_C161_Sub,
PQ_IP_C_SRAM4_C111_Sub,
PQ_IP_C_SRAM4_NUMS_Sub
} PQ_IP_C_SRAM4_Group_Sub;

typedef enum
{
PQ_IP_VE_HSD_SRAM_C121_128_Sub,
PQ_IP_VE_HSD_SRAM_C565_Sub,
PQ_IP_VE_HSD_SRAM_FC100_Sub,
PQ_IP_VE_HSD_SRAM_AVG_Sub,
PQ_IP_VE_HSD_SRAM_FC102_Sub,
PQ_IP_VE_HSD_SRAM_FC101_Sub,
PQ_IP_VE_HSD_SRAM_FC103_Sub,
PQ_IP_VE_HSD_SRAM_FC104_Sub,
PQ_IP_VE_HSD_SRAM_FC105_Sub,
PQ_IP_VE_HSD_SRAM_FC110_Sub,
PQ_IP_VE_HSD_SRAM_NUMS_Sub
} PQ_IP_VE_HSD_SRAM_Group_Sub;

typedef enum
{
PQ_IP_VE_VSD_SRAM_C121_128_Sub,
PQ_IP_VE_VSD_SRAM_C565_Sub,
PQ_IP_VE_VSD_SRAM_FC100_Sub,
PQ_IP_VE_VSD_SRAM_AVG_Sub,
PQ_IP_VE_VSD_SRAM_NUMS_Sub
} PQ_IP_VE_VSD_SRAM_Group_Sub;

typedef enum
{
PQ_IP_422To444_ON_Sub,
PQ_IP_422To444_OFF_Sub,
PQ_IP_422To444_SIMPLE_Sub,
PQ_IP_422To444_BICUBIC_Sub,
PQ_IP_422To444_Bypass_Sub,
PQ_IP_422To444_NUMS_Sub
} PQ_IP_422To444_Group_Sub;

typedef enum
{
PQ_IP_Peaking_OFF_Sub,
PQ_IP_Peaking_PC_mode_Sub,
PQ_IP_Peaking_NUMS_Sub
} PQ_IP_Peaking_Group_Sub;

typedef enum
{
PQ_IP_SwDriver_ALL_Sub,
PQ_IP_SwDriver_NUMS_Sub
} PQ_IP_SwDriver_Group_Sub;

typedef enum
{
PQ_IP_3x3_OFF_Sub,
PQ_IP_3x3_NUMS_Sub
} PQ_IP_3x3_Group_Sub;

typedef enum
{
PQ_IP_BRI_CR_ALL_Sub,
PQ_IP_BRI_CR_HDMI_Sub,
PQ_IP_BRI_CR_NUMS_Sub
} PQ_IP_BRI_CR_Group_Sub;

typedef enum
{
PQ_IP_Display_ALL_Sub,
PQ_IP_Display_HDMI_Sub,
PQ_IP_Display_NUMS_Sub
} PQ_IP_Display_Group_Sub;

extern code U8 MST_SkipRule_IP_Sub[PQ_IP_NUM_Sub];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub[];
extern code U8 QMAP_1920_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_1920_PIP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_1920_POP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_1366_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_1366_PIP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_1366_POP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_4K2K_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_4K2K_PIP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];
extern code U8 QMAP_4K2K_POP_Sub[QM_INPUTTYPE_NUM_Sub][PQ_IP_NUM_Sub];

#endif
