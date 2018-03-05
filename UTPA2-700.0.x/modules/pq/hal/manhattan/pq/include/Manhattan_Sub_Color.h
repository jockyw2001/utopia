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
// Muji
// 8/27/2014  7:54:48 PM
// 928_20140827
//****************************************************

#ifndef _MUJI_SUB_COLOR_H_
#define _MUJI_SUB_COLOR_H_

#define PQ_IP_NUM_Sub_Color 22

typedef enum
{
    #if PQ_QM_CVBS
    QM_RF_NTSC_44_Sub_Color, //0
    #endif
    #if PQ_QM_CVBS
    QM_RF_NTSC_M_Sub_Color, //1
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_BGHI_Sub_Color, //2
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_60_Sub_Color, //3
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_M_Sub_Color, //4
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_N_Sub_Color, //5
    #endif
    #if PQ_QM_CVBS
    QM_RF_SECAM_Sub_Color, //6
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_44_Sub_Color, //7
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_M_Sub_Color, //8
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_BGHI_Sub_Color, //9
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_60_Sub_Color, //10
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_M_Sub_Color, //11
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_N_Sub_Color, //12
    #endif
    #if PQ_QM_CVBS
    QM_VIF_SECAM_Sub_Color, //13
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_44_Sub_Color, //14
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_M_Sub_Color, //15
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_BGHI_Sub_Color, //16
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_60_Sub_Color, //17
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_M_Sub_Color, //18
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_N_Sub_Color, //19
    #endif
    #if PQ_QM_CVBS
    QM_SV_SECAM_Sub_Color, //20
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_44_Sub_Color, //21
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_M_Sub_Color, //22
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_BGHI_Sub_Color, //23
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_60_Sub_Color, //24
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_M_Sub_Color, //25
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_N_Sub_Color, //26
    #endif
    #if PQ_QM_CVBS
    QM_AV_SECAM_Sub_Color, //27
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_44_Sub_Color, //28
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_M_Sub_Color, //29
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_BGHI_Sub_Color, //30
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_60_Sub_Color, //31
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_M_Sub_Color, //32
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_N_Sub_Color, //33
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_SECAM_Sub_Color, //34
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_44_Sub_Color, //35
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_M_Sub_Color, //36
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_BGHI_Sub_Color, //37
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_60_Sub_Color, //38
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_M_Sub_Color, //39
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_N_Sub_Color, //40
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_SECAM_Sub_Color, //41
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_NTSC_Sub_Color, //42
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_PAL_Sub_Color, //43
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Sub_Color, //44
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Sub_Color, //45
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Sub_Color, //46
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Sub_Color, //47
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Sub_Color, //48
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Sub_Color, //49
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Sub_Color, //50
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Sub_Color, //51
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Sub_Color, //52
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Sub_Color, //53
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Sub_Color, //54
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Sub_Color, //55
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Sub_Color, //56
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Sub_Color, //57
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Sub_Color, //58
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Sub_Color, //59
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Sub_Color, //60
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Sub_Color, //61
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Sub_Color, //62
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_FP_720p_Sub_Color, //63
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Sub_Color, //64
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Sub_Color, //65
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Sub_Color, //66
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Sub_Color, //67
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Sub_Color, //68
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_FP_1080i_Sub_Color, //69
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_FP_1080p_Sub_Color, //70
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_24Hz_Sub_Color, //71
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_30Hz_Sub_Color, //72
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_60Hz_Sub_Color, //73
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Sub_Color, //74
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Sub_Color, //75
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Sub_Color, //76
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Sub_Color, //77
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Sub_Color, //78
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Sub_Color, //79
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Sub_Color, //80
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Sub_Color, //81
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Sub_Color, //82
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Sub_Color, //83
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Sub_Color, //84
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Sub_Color, //85
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Sub_Color, //86
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Sub_Color, //87
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Sub_Color, //88
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Sub_Color, //89
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Sub_Color, //90
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Sub_Color, //91
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Sub_Color, //92
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Sub_Color, //93
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Sub_Color, //94
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Sub_Color, //95
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Sub_Color, //96
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Sub_Color, //97
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Sub_Color, //98
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Sub_Color, //99
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Sub_Color, //100
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_MPEG2_Sub_Color, //101
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_MPEG2_Sub_Color, //102
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_MPEG2_Sub_Color, //103
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_MPEG2_Sub_Color, //104
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_MPEG2_Sub_Color, //105
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_MPEG2_Sub_Color, //106
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_MPEG2_Sub_Color, //107
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_MPEG2_Sub_Color, //108
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_MPEG2_Sub_Color, //109
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_MPEG2_Sub_Color, //110
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_MPEG2_Sub_Color, //111
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_MPEG2_Sub_Color, //112
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_MPEG2_Sub_Color, //113
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_MPEG2_Sub_Color, //114
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_MPEG2_Sub_Color, //115
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_H264_Sub_Color, //116
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_H264_Sub_Color, //117
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_H264_Sub_Color, //118
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_H264_Sub_Color, //119
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_H264_Sub_Color, //120
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_H264_Sub_Color, //121
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_H264_Sub_Color, //122
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_H264_Sub_Color, //123
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_H264_Sub_Color, //124
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_H264_Sub_Color, //125
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_H264_Sub_Color, //126
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_H264_Sub_Color, //127
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_H264_Sub_Color, //128
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H264_Sub_Color, //129
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_H264_Sub_Color, //130
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_interlace_Sub_Color, //131
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_Sub_Color, //132
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_24hz_Sub_Color, //133
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_interlace_Sub_Color, //134
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_Sub_Color, //135
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_24hz_Sub_Color, //136
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_SD_progressive_Sub_Color, //137
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_HD_progressive_Sub_Color, //138
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_SD_interlace_Sub_Color, //139
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_SD_progressive_Sub_Color, //140
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_SD_progressive_24hz_Sub_Color, //141
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_HD_interlace_Sub_Color, //142
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_HD_progressive_Sub_Color, //143
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_online_HD_progressive_24hz_Sub_Color, //144
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_Sub_Color, //145
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_60Hz_Sub_Color, //146
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_non_4K2K_Sub_Color, //147
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_interlace_Sub_Color, //148
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_progressive_Sub_Color, //149
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_interlace_Sub_Color, //150
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_progressive_Sub_Color, //151
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_4K2K_FS_progressive_Sub_Color, //152
    #endif
    QM_INPUTTYPE_NUM_Sub_Color, // 153
} SC_QUALITY_MAP_INDEX_e_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T1_Sub_Color,  //0
PQ_IP_VIP_FCC_T2_Sub_Color,  //1
PQ_IP_VIP_FCC_T3_Sub_Color,  //2
PQ_IP_VIP_FCC_T4_Sub_Color,  //3
PQ_IP_VIP_FCC_T5_Sub_Color,  //4
PQ_IP_VIP_FCC_T6_Sub_Color,  //5
PQ_IP_VIP_FCC_T7_Sub_Color,  //6
PQ_IP_VIP_FCC_T8_Sub_Color,  //7
PQ_IP_VIP_FCC_T9_Sub_Color,  //8
PQ_IP_VIP_IHC_Sub_Color,  //9
PQ_IP_VIP_IHC_Ymode_Sub_Color,  //10
PQ_IP_VIP_IHC_SETTING_Sub_Color,  //11
PQ_IP_VIP_ICC_Sub_Color,  //12
PQ_IP_VIP_ICC_Ymode_Sub_Color,  //13
PQ_IP_VIP_ICC_SETTING_Sub_Color,  //14
PQ_IP_VIP_Ymode_Yvalue_ALL_Sub_Color,  //15
PQ_IP_VIP_Ymode_Yvalue_SETTING_Sub_Color,  //16
PQ_IP_VIP_IBC_Sub_Color,  //17
PQ_IP_VIP_IBC_SETTING_Sub_Color,  //18
PQ_IP_VIP_Post_Cgain_Sub_Color,  //19
PQ_IP_VIP_Peaking_band_Sub_Color,  //20
PQ_IP_VIP_Peaking_Pcoring_Sub_Color,  //21
 }   PQ_IPTYPE_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T1_COM_Sub_Color,  //0
PQ_IP_VIP_FCC_T2_COM_Sub_Color,  //1
PQ_IP_VIP_FCC_T3_COM_Sub_Color,  //2
PQ_IP_VIP_FCC_T4_COM_Sub_Color,  //3
PQ_IP_VIP_FCC_T5_COM_Sub_Color,  //4
PQ_IP_VIP_FCC_T6_COM_Sub_Color,  //5
PQ_IP_VIP_FCC_T7_COM_Sub_Color,  //6
PQ_IP_VIP_FCC_T8_COM_Sub_Color,  //7
PQ_IP_VIP_FCC_T9_COM_Sub_Color,  //8
PQ_IP_VIP_IHC_COM_Sub_Color,  //9
PQ_IP_VIP_IHC_Ymode_COM_Sub_Color,  //10
PQ_IP_VIP_IHC_SETTING_COM_Sub_Color,  //11
PQ_IP_VIP_ICC_COM_Sub_Color,  //12
PQ_IP_VIP_ICC_Ymode_COM_Sub_Color,  //13
PQ_IP_VIP_ICC_SETTING_COM_Sub_Color,  //14
PQ_IP_VIP_Ymode_Yvalue_ALL_COM_Sub_Color,  //15
PQ_IP_VIP_Ymode_Yvalue_SETTING_COM_Sub_Color,  //16
PQ_IP_VIP_IBC_COM_Sub_Color,  //17
PQ_IP_VIP_IBC_SETTING_COM_Sub_Color,  //18
PQ_IP_VIP_Post_Cgain_COM_Sub_Color,  //19
PQ_IP_VIP_Peaking_band_COM_Sub_Color,  //20
PQ_IP_VIP_Peaking_Pcoring_COM_Sub_Color,  //21
PQ_IP_COM_NUMS_Sub_Color
} PQ_IP_COM_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T1_OFF_Sub_Color,
PQ_IP_VIP_FCC_T1_S4_Sub_Color,
PQ_IP_VIP_FCC_T1_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T1_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T2_OFF_Sub_Color,
PQ_IP_VIP_FCC_T2_S8_Sub_Color,
PQ_IP_VIP_FCC_T2_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T2_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T3_OFF_Sub_Color,
PQ_IP_VIP_FCC_T3_S4_Sub_Color,
PQ_IP_VIP_FCC_T3_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T3_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T4_OFF_Sub_Color,
PQ_IP_VIP_FCC_T4_S4_Sub_Color,
PQ_IP_VIP_FCC_T4_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T4_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T5_OFF_Sub_Color,
PQ_IP_VIP_FCC_T5_S6_Sub_Color,
PQ_IP_VIP_FCC_T5_S8_Sub_Color,
PQ_IP_VIP_FCC_T5_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T5_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T6_OFF_Sub_Color,
PQ_IP_VIP_FCC_T6_S6_Sub_Color,
PQ_IP_VIP_FCC_T6_S8_Sub_Color,
PQ_IP_VIP_FCC_T6_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T6_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T7_OFF_Sub_Color,
PQ_IP_VIP_FCC_T7_S8_Sub_Color,
PQ_IP_VIP_FCC_T7_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T7_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T8_OFF_Sub_Color,
PQ_IP_VIP_FCC_T8_S5_Sub_Color,
PQ_IP_VIP_FCC_T8_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T8_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_FCC_T9_OFF_Sub_Color,
PQ_IP_VIP_FCC_T9_S5_Sub_Color,
PQ_IP_VIP_FCC_T9_NUMS_Sub_Color
} PQ_IP_VIP_FCC_T9_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_IHC_OFF_Sub_Color,
PQ_IP_VIP_IHC_ON_Sub_Color,
PQ_IP_VIP_IHC_ON_Sram_Sub_Color,
PQ_IP_VIP_IHC_NUMS_Sub_Color
} PQ_IP_VIP_IHC_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_IHC_Ymode_OFF_Sub_Color,
PQ_IP_VIP_IHC_Ymode_ON_Sub_Color,
PQ_IP_VIP_IHC_Ymode_NUMS_Sub_Color
} PQ_IP_VIP_IHC_Ymode_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_IHC_SETTING_OFF_Sub_Color,
PQ_IP_VIP_IHC_SETTING_N_20_Sub_Color,
PQ_IP_VIP_IHC_SETTING_RF_NTSC_Sub_Color,
PQ_IP_VIP_IHC_SETTING_RF_PAL_Sub_Color,
PQ_IP_VIP_IHC_SETTING_AV_PAL_Sub_Color,
PQ_IP_VIP_IHC_SETTING_AV_NTSC_Sub_Color,
PQ_IP_VIP_IHC_SETTING_YPBPR_480i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_YPBPR_576i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_YPBPR_720p_Sub_Color,
PQ_IP_VIP_IHC_SETTING_YPBPR_1080i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_HDMI_480i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_HDMI_576i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_HDMI_720p_Sub_Color,
PQ_IP_VIP_IHC_SETTING_HDMI_1080i_Sub_Color,
PQ_IP_VIP_IHC_SETTING_NUMS_Sub_Color
} PQ_IP_VIP_IHC_SETTING_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_ICC_OFF_Sub_Color,
PQ_IP_VIP_ICC_ON_Sub_Color,
PQ_IP_VIP_ICC_ON_Sram_Sub_Color,
PQ_IP_VIP_ICC_NUMS_Sub_Color
} PQ_IP_VIP_ICC_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_ICC_Ymode_OFF_Sub_Color,
PQ_IP_VIP_ICC_Ymode_ON_Sub_Color,
PQ_IP_VIP_ICC_Ymode_NUMS_Sub_Color
} PQ_IP_VIP_ICC_Ymode_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_ICC_SETTING_OFF_Sub_Color,
PQ_IP_VIP_ICC_SETTING_RF_PAL_Sub_Color,
PQ_IP_VIP_ICC_SETTING_RF_NTSC_Sub_Color,
PQ_IP_VIP_ICC_SETTING_AV_PAL_Sub_Color,
PQ_IP_VIP_ICC_SETTING_AV_NTSC_Sub_Color,
PQ_IP_VIP_ICC_SETTING_YPBPR_480i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_YPBPR_576i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_YPBPR_720p_Sub_Color,
PQ_IP_VIP_ICC_SETTING_YPBPR_1080i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_HDMI_480i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_HDMI_576i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_HDMI_720p_Sub_Color,
PQ_IP_VIP_ICC_SETTING_HDMI_1080i_Sub_Color,
PQ_IP_VIP_ICC_SETTING_NUMS_Sub_Color
} PQ_IP_VIP_ICC_SETTING_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_Ymode_Yvalue_ALL_Y1_Sub_Color,
PQ_IP_VIP_Ymode_Yvalue_ALL_NUMS_Sub_Color
} PQ_IP_VIP_Ymode_Yvalue_ALL_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_Ymode_Yvalue_SETTING_Y1_Sub_Color,
PQ_IP_VIP_Ymode_Yvalue_SETTING_NUMS_Sub_Color
} PQ_IP_VIP_Ymode_Yvalue_SETTING_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_IBC_OFF_Sub_Color,
PQ_IP_VIP_IBC_ON_Sub_Color,
PQ_IP_VIP_IBC_NUMS_Sub_Color
} PQ_IP_VIP_IBC_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_IBC_SETTING_OFF_Sub_Color,
PQ_IP_VIP_IBC_SETTING_DEC_10_Sub_Color,
PQ_IP_VIP_IBC_SETTING_DEC_18_Sub_Color,
PQ_IP_VIP_IBC_SETTING_RF_PAL_Sub_Color,
PQ_IP_VIP_IBC_SETTING_RF_NTSC_Sub_Color,
PQ_IP_VIP_IBC_SETTING_AV_PAL_Sub_Color,
PQ_IP_VIP_IBC_SETTING_AV_NTSC_Sub_Color,
PQ_IP_VIP_IBC_SETTING_YPBPR_SD_Sub_Color,
PQ_IP_VIP_IBC_SETTING_YPBPR_HD_Sub_Color,
PQ_IP_VIP_IBC_SETTING_HDMI_SD_Sub_Color,
PQ_IP_VIP_IBC_SETTING_HDMI_HD_Sub_Color,
PQ_IP_VIP_IBC_SETTING_NUMS_Sub_Color
} PQ_IP_VIP_IBC_SETTING_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_Post_Cgain_OFF_Sub_Color,
PQ_IP_VIP_Post_Cgain_0x44_Sub_Color,
PQ_IP_VIP_Post_Cgain_NUMS_Sub_Color
} PQ_IP_VIP_Post_Cgain_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_Peaking_band_RF_NTSC_Sub_Color,
PQ_IP_VIP_Peaking_band_RF_SECAM_Sub_Color,
PQ_IP_VIP_Peaking_band_AV_NTSC_Sub_Color,
PQ_IP_VIP_Peaking_band_AV_PAL_Sub_Color,
PQ_IP_VIP_Peaking_band_AV_SECAM_Sub_Color,
PQ_IP_VIP_Peaking_band_SV_NTSC_Sub_Color,
PQ_IP_VIP_Peaking_band_480i_Sub_Color,
PQ_IP_VIP_Peaking_band_720p_Sub_Color,
PQ_IP_VIP_Peaking_band_1080i_Sub_Color,
PQ_IP_VIP_Peaking_band_H_480i_Sub_Color,
PQ_IP_VIP_Peaking_band_H_576i_Sub_Color,
PQ_IP_VIP_Peaking_band_H_720p_Sub_Color,
PQ_IP_VIP_Peaking_band_H_1080i_Sub_Color,
PQ_IP_VIP_Peaking_band_DT_MPEG2_480is_Sub_Color,
PQ_IP_VIP_Peaking_band_DT_MPEG2_720p_Sub_Color,
PQ_IP_VIP_Peaking_band_DT_MPEG2_1080i_Sub_Color,
PQ_IP_VIP_Peaking_band_DT_H264_480is_Sub_Color,
PQ_IP_VIP_Peaking_band_DT_H264_720p_Sub_Color,
PQ_IP_VIP_Peaking_band_PC_mode_Sub_Color,
PQ_IP_VIP_Peaking_band_MM_SD_Sub_Color,
PQ_IP_VIP_Peaking_band_MM_HD_Sub_Color,
PQ_IP_VIP_Peaking_band_NUMS_Sub_Color
} PQ_IP_VIP_Peaking_band_Group_Sub_Color;

typedef enum
{
PQ_IP_VIP_Peaking_Pcoring_S00_0_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S21_0_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S21_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S30_0_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S30_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S31_0_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S31_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S20_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S32_0_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S32_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S32_2_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S42_1_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_S43_2_Sub_Color,
PQ_IP_VIP_Peaking_Pcoring_NUMS_Sub_Color
} PQ_IP_VIP_Peaking_Pcoring_Group_Sub_Color;

extern code U8 MST_SkipRule_IP_Sub_Color[PQ_IP_NUM_Sub_Color];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub_Color[];
extern code U8 QMAP_1920_Sub_Color[QM_INPUTTYPE_NUM_Sub_Color][PQ_IP_NUM_Sub_Color];

#endif
