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
// k6
// 2016/9/2 11:34
// 2008/7/1 10:31
//****************************************************

#ifndef _K6_SUB_EX_H_
#define _K6_SUB_EX_H_

#define PQ_IP_NUM_Sub_Ex 3

typedef enum
{
    #if PQ_QM_CVBS
    QM_RF_NTSC_44_Sub_Ex, //0
    #endif
    #if PQ_QM_CVBS
    QM_RF_NTSC_M_Sub_Ex, //1
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_BGHI_Sub_Ex, //2
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_60_Sub_Ex, //3
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_M_Sub_Ex, //4
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_N_Sub_Ex, //5
    #endif
    #if PQ_QM_CVBS
    QM_RF_SECAM_Sub_Ex, //6
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_44_Sub_Ex, //7
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_M_Sub_Ex, //8
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_BGHI_Sub_Ex, //9
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_60_Sub_Ex, //10
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_M_Sub_Ex, //11
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_N_Sub_Ex, //12
    #endif
    #if PQ_QM_CVBS
    QM_VIF_SECAM_Sub_Ex, //13
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_44_Sub_Ex, //14
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_M_Sub_Ex, //15
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_BGHI_Sub_Ex, //16
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_60_Sub_Ex, //17
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_M_Sub_Ex, //18
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_N_Sub_Ex, //19
    #endif
    #if PQ_QM_CVBS
    QM_SV_SECAM_Sub_Ex, //20
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_44_Sub_Ex, //21
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_M_Sub_Ex, //22
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_BGHI_Sub_Ex, //23
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_60_Sub_Ex, //24
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_M_Sub_Ex, //25
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_N_Sub_Ex, //26
    #endif
    #if PQ_QM_CVBS
    QM_AV_SECAM_Sub_Ex, //27
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_44_Sub_Ex, //28
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_M_Sub_Ex, //29
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_BGHI_Sub_Ex, //30
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_60_Sub_Ex, //31
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_M_Sub_Ex, //32
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_N_Sub_Ex, //33
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_SECAM_Sub_Ex, //34
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_44_Sub_Ex, //35
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_M_Sub_Ex, //36
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_BGHI_Sub_Ex, //37
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_60_Sub_Ex, //38
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_M_Sub_Ex, //39
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_N_Sub_Ex, //40
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_SECAM_Sub_Ex, //41
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_NTSC_Sub_Ex, //42
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_PAL_Sub_Ex, //43
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Sub_Ex, //44
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Sub_Ex, //45
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Sub_Ex, //46
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Sub_Ex, //47
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Sub_Ex, //48
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Sub_Ex, //49
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Sub_Ex, //50
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Sub_Ex, //51
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Sub_Ex, //52
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Sub_Ex, //53
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Sub_Ex, //54
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Sub_Ex, //55
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Sub_Ex, //56
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Sub_Ex, //57
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Sub_Ex, //58
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Sub_Ex, //59
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Sub_Ex, //60
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Sub_Ex, //61
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Sub_Ex, //62
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Sub_Ex, //63
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Sub_Ex, //64
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Sub_Ex, //65
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Sub_Ex, //66
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Sub_Ex, //67
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_24Hz_Sub_Ex, //68
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_30Hz_Sub_Ex, //69
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_60Hz_Sub_Ex, //70
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Sub_Ex, //71
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Sub_Ex, //72
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Sub_Ex, //73
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Sub_Ex, //74
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Sub_Ex, //75
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Sub_Ex, //76
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Sub_Ex, //77
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Sub_Ex, //78
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Sub_Ex, //79
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_4K_Sub_Ex, //80
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Sub_Ex, //81
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Sub_Ex, //82
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Sub_Ex, //83
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Sub_Ex, //84
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Sub_Ex, //85
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Sub_Ex, //86
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Sub_Ex, //87
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Sub_Ex, //88
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Sub_Ex, //89
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Sub_Ex, //90
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Sub_Ex, //91
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Sub_Ex, //92
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Sub_Ex, //93
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Sub_Ex, //94
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Sub_Ex, //95
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Sub_Ex, //96
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Sub_Ex, //97
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Sub_Ex, //98
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_4K_Sub_Ex, //99
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_MPEG2_Sub_Ex, //100
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_MPEG2_Sub_Ex, //101
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_MPEG2_Sub_Ex, //102
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_MPEG2_Sub_Ex, //103
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_MPEG2_Sub_Ex, //104
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_MPEG2_Sub_Ex, //105
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_MPEG2_Sub_Ex, //106
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_MPEG2_Sub_Ex, //107
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_MPEG2_Sub_Ex, //108
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_MPEG2_Sub_Ex, //109
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_MPEG2_Sub_Ex, //110
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_MPEG2_Sub_Ex, //111
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_MPEG2_Sub_Ex, //112
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_MPEG2_Sub_Ex, //113
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_MPEG2_Sub_Ex, //114
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_H264_Sub_Ex, //115
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_H264_Sub_Ex, //116
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_H264_Sub_Ex, //117
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_H264_Sub_Ex, //118
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_H264_Sub_Ex, //119
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_H264_Sub_Ex, //120
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_H264_Sub_Ex, //121
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_H264_Sub_Ex, //122
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_H264_Sub_Ex, //123
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_H264_Sub_Ex, //124
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_H264_Sub_Ex, //125
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_H264_Sub_Ex, //126
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_H264_Sub_Ex, //127
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H264_Sub_Ex, //128
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_H264_Sub_Ex, //129
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H265_Sub_Ex, //130
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_interlace_Sub_Ex, //131
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_Sub_Ex, //132
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_24hz_Sub_Ex, //133
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_interlace_Sub_Ex, //134
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_Sub_Ex, //135
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_24hz_Sub_Ex, //136
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_4K2K_FS_progressive_Sub_Ex, //137
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_photo_SD_progressive_Sub_Ex, //138
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_photo_HD_progressive_Sub_Ex, //139
    #endif
    #if PQ_QM_MM_PHOTO
    QM_MM_4K2K_Sub_Ex, //140
    #endif
    #if PQ_QM_MM_PHOTO
    QM_MM_4K2K_Photo_Sub_Ex, //141
    #endif
    #if PQ_QM_MM_PHOTO
    QM_MM_4K2K_60Hz_Sub_Ex, //142
    #endif
    #if PQ_QM_MM_PHOTO
    QM_MM_non_4K2K_Sub_Ex, //143
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_interlace_Sub_Ex, //144
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_progressive_Sub_Ex, //145
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_interlace_Sub_Ex, //146
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_progressive_Sub_Ex, //147
    #endif
    QM_INPUTTYPE_NUM_Sub_Ex, // 148
} SC_QUALITY_MAP_INDEX_e_Sub_Ex;

typedef enum
{
PQ_IP_AFEC_no_comm_Sub_Ex,  //0
PQ_IP_SPF_DBK_Sub_Ex,  //1
PQ_IP_DMS_Sub_Ex,  //2
 }   PQ_IPTYPE_Sub_Ex;

typedef enum
{
PQ_IP_AFEC_no_comm_COM_Sub_Ex,  //0
PQ_IP_SPF_DBK_COM_Sub_Ex,  //1
PQ_IP_DMS_COM_Sub_Ex,  //2
PQ_IP_COM_NUMS_Sub_Ex
} PQ_IP_COM_Group_Sub_Ex;

typedef enum
{
PQ_IP_AFEC_no_comm_ON_Sub_Ex,
PQ_IP_AFEC_no_comm_NUMS_Sub_Ex
} PQ_IP_AFEC_no_comm_Group_Sub_Ex;

typedef enum
{
PQ_IP_SPF_DBK_OFF_Sub_Ex,
PQ_IP_SPF_DBK_LON_Sub_Ex,
PQ_IP_SPF_DBK_NUMS_Sub_Ex
} PQ_IP_SPF_DBK_Group_Sub_Ex;

typedef enum
{
PQ_IP_DMS_OFF_Sub_Ex,
PQ_IP_DMS_ON_Sub_Ex,
PQ_IP_DMS_NUMS_Sub_Ex
} PQ_IP_DMS_Group_Sub_Ex;

extern code U8 MST_SkipRule_IP_Sub_Ex[PQ_IP_NUM_Sub_Ex];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub_Ex[];
extern code U8 QMAP_1920_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_1920_PIP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_1920_POP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_1366_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_1366_PIP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_1366_POP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_4K2K_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_4K2K_PIP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];
extern code U8 QMAP_4K2K_POP_Sub_Ex[QM_INPUTTYPE_NUM_Sub_Ex][PQ_IP_NUM_Sub_Ex];

#endif
