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
// 2/13/2017  9:22:41 PM
// 39630.43853
//****************************************************

#ifndef _KANO_MAIN_H_
#define _KANO_MAIN_H_

#define PQ_IP_NUM_Main 74
#define PQ_IP_SRAM1_SIZE_Main 1280
#define PQ_IP_SRAM2_SIZE_Main 1280
#define PQ_IP_SRAM3_SIZE_Main 1280
#define PQ_IP_SRAM4_SIZE_Main 1280
#define PQ_IP_C_SRAM1_SIZE_Main 320
#define PQ_IP_C_SRAM2_SIZE_Main 320
#define PQ_IP_C_SRAM3_SIZE_Main 320
#define PQ_IP_C_SRAM4_SIZE_Main 320
#define PQ_IP_VE_HSD_SRAM_SIZE_Main 640
#define PQ_IP_VE_VSD_SRAM_SIZE_Main 640

typedef enum
{
    #if PQ_QM_CVBS
    QM_RF_NTSC_44_Main, //0
    #endif
    #if PQ_QM_CVBS
    QM_RF_NTSC_M_Main, //1
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_BGHI_Main, //2
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_60_Main, //3
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_M_Main, //4
    #endif
    #if PQ_QM_CVBS
    QM_RF_PAL_N_Main, //5
    #endif
    #if PQ_QM_CVBS
    QM_RF_SECAM_Main, //6
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_44_Main, //7
    #endif
    #if PQ_QM_CVBS
    QM_VIF_NTSC_M_Main, //8
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_BGHI_Main, //9
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_60_Main, //10
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_M_Main, //11
    #endif
    #if PQ_QM_CVBS
    QM_VIF_PAL_N_Main, //12
    #endif
    #if PQ_QM_CVBS
    QM_VIF_SECAM_Main, //13
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_44_Main, //14
    #endif
    #if PQ_QM_CVBS
    QM_SV_NTSC_M_Main, //15
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_BGHI_Main, //16
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_60_Main, //17
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_M_Main, //18
    #endif
    #if PQ_QM_CVBS
    QM_SV_PAL_N_Main, //19
    #endif
    #if PQ_QM_CVBS
    QM_SV_SECAM_Main, //20
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_44_Main, //21
    #endif
    #if PQ_QM_CVBS
    QM_AV_NTSC_M_Main, //22
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_BGHI_Main, //23
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_60_Main, //24
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_M_Main, //25
    #endif
    #if PQ_QM_CVBS
    QM_AV_PAL_N_Main, //26
    #endif
    #if PQ_QM_CVBS
    QM_AV_SECAM_Main, //27
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_44_Main, //28
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_NTSC_M_Main, //29
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_BGHI_Main, //30
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_60_Main, //31
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_M_Main, //32
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_PAL_N_Main, //33
    #endif
    #if PQ_QM_CVBS
    QM_SCART_AV_SECAM_Main, //34
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_44_Main, //35
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_NTSC_M_Main, //36
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_BGHI_Main, //37
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_60_Main, //38
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_M_Main, //39
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_PAL_N_Main, //40
    #endif
    #if PQ_QM_CVBS
    QM_SCART_SV_SECAM_Main, //41
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_NTSC_Main, //42
    #endif
    #if PQ_QM_CVBS
    QM_SCART_RGB_PAL_Main, //43
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Main, //44
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Main, //45
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Main, //46
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Main, //47
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Main, //48
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Main, //49
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Main, //50
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Main, //51
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Main, //52
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Main, //53
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Main, //54
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Main, //55
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Main, //56
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Main, //57
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Main, //58
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Main, //59
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Main, //60
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Main, //61
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Main, //62
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Main, //63
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Main, //64
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Main, //65
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Main, //66
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Main, //67
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_24Hz_Main, //68
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_30Hz_Main, //69
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_60Hz_Main, //70
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Main, //71
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Main, //72
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Main, //73
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Main, //74
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Main, //75
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Main, //76
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Main, //77
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Main, //78
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Main, //79
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_4K_Main, //80
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Main, //81
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Main, //82
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Main, //83
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Main, //84
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Main, //85
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Main, //86
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Main, //87
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Main, //88
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Main, //89
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main, //90
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main, //91
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main, //92
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main, //93
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main, //94
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main, //95
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main, //96
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main, //97
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main, //98
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_4K_Main, //99
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_MPEG2_Main, //100
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_MPEG2_Main, //101
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_MPEG2_Main, //102
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_MPEG2_Main, //103
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_MPEG2_Main, //104
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_MPEG2_Main, //105
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_MPEG2_Main, //106
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_MPEG2_Main, //107
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_MPEG2_Main, //108
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_MPEG2_Main, //109
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_MPEG2_Main, //110
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_MPEG2_Main, //111
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_MPEG2_Main, //112
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_MPEG2_Main, //113
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_MPEG2_Main, //114
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_352x480_H264_Main, //115
    #endif
    #if PQ_QM_DTV
    QM_DTV_480i_H264_Main, //116
    #endif
    #if PQ_QM_DTV
    QM_DTV_576i_H264_Main, //117
    #endif
    #if PQ_QM_DTV
    QM_DTV_480p_H264_Main, //118
    #endif
    #if PQ_QM_DTV
    QM_DTV_576p_H264_Main, //119
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_24hz_H264_Main, //120
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_50hz_H264_Main, //121
    #endif
    #if PQ_QM_DTV
    QM_DTV_720p_60hz_H264_Main, //122
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_50hz_H264_Main, //123
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080i_60hz_H264_Main, //124
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_24hz_H264_Main, //125
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_50hz_H264_Main, //126
    #endif
    #if PQ_QM_DTV
    QM_DTV_1080p_60hz_H264_Main, //127
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H264_Main, //128
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_60Hz_H264_Main, //129
    #endif
    #if PQ_QM_DTV
    QM_DTV_4K2K_H265_Main, //130
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_interlace_Main, //131
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_Main, //132
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_SD_progressive_24hz_Main, //133
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_interlace_Main, //134
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_Main, //135
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_HD_progressive_24hz_Main, //136
    #endif
    #if PQ_QM_MM_VIDEO
    QM_Multimedia_video_4K2K_FS_progressive_Main, //137
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_SD_progressive_Main, //138
    #endif
    #if PQ_QM_MM_PHOTO
    QM_Multimedia_photo_HD_progressive_Main, //139
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_Main, //140
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_Photo_Main, //141
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_4K2K_60Hz_Main, //142
    #endif
    #if PQ_QM_MM_VIDEO
    QM_MM_non_4K2K_Main, //143
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_interlace_Main, //144
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_SD_progressive_Main, //145
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_interlace_Main, //146
    #endif
    #if PQ_QM_DTV
    QM_DTV_iFrame_HD_progressive_Main, //147
    #endif
    QM_INPUTTYPE_NUM_Main, // 148
} SC_QUALITY_MAP_INDEX_e_Main;

typedef enum
{
PQ_IP_AFEC_Main,  //0
PQ_IP_Comb_Main,  //1
PQ_IP_SECAM_Main,  //2
PQ_IP_SCinit_Main,  //3
PQ_IP_CSC_Main,  //4
PQ_IP_CSC_Dither_Main,  //5
PQ_IP_IP422To444_Main,  //6
PQ_IP_HSD_Y_Main,  //7
PQ_IP_HSD_C_Main,  //8
PQ_IP_VSD_Main,  //9
PQ_IP_MemFormat_Main,  //10
PQ_IP_444To422_Main,  //11
PQ_IP_PreSNR_Main,  //12
PQ_IP_DNR_Main,  //13
PQ_IP_DNR_YOnly_Main,  //14
PQ_IP_DNR_Y_Main,  //15
PQ_IP_DNR_MED_Main,  //16
PQ_IP_DNR_C_Main,  //17
PQ_IP_DNR_sticky_solver_Main,  //18
PQ_IP_PostCCS_Main,  //19
PQ_IP_PNR_Main,  //20
PQ_IP_PNR_Y_Main,  //21
PQ_IP_PNR_C_Main,  //22
PQ_IP_420CUP_Main,  //23
PQ_IP_MADi_Main,  //24
PQ_IP_MADi_Motion_Main,  //25
PQ_IP_EODi_Main,  //26
PQ_IP_Film_Main,  //27
PQ_IP_Film32_Main,  //28
PQ_IP_Film22_Main,  //29
PQ_IP_Film_any_Main,  //30
PQ_IP_SPF_Main,  //31
PQ_IP_SPF_DBK_Main,  //32
PQ_IP_SPF_DBK_MR_Main,  //33
PQ_IP_SPF_DBK_BKN_Main,  //34
PQ_IP_SPF_SNR_Main,  //35
PQ_IP_SPF_SNR_MR_Main,  //36
PQ_IP_SPF_MR_LPF_Main,  //37
PQ_IP_SPF_NMR_Y_Main,  //38
PQ_IP_SPF_NMR_Y_MR_Main,  //39
PQ_IP_SPF_NMR_C_Main,  //40
PQ_IP_DMS_Main,  //41
PQ_IP_DMS_H_Main,  //42
PQ_IP_DMS_V_Main,  //43
PQ_IP_DMS_V_12L_Main,  //44
PQ_IP_VSP_Y_Main,  //45
PQ_IP_VSP_C_Main,  //46
PQ_IP_VSP_Coring_Main,  //47
PQ_IP_VSP_Dither_Main,  //48
PQ_IP_VSP_PreVBound_Main,  //49
PQ_IP_PreCTI_Main,  //50
PQ_IP_HSP_Y_Main,  //51
PQ_IP_HSP_C_Main,  //52
PQ_IP_HSP_Coring_Main,  //53
PQ_IP_HSP_DeRing_Main,  //54
PQ_IP_HSP_Dither_Main,  //55
PQ_IP_HnonLinear_Main,  //56
PQ_IP_VE_HVSD_Main,  //57
PQ_IP_SRAM1_Main,  //58
PQ_IP_SRAM2_Main,  //59
PQ_IP_SRAM3_Main,  //60
PQ_IP_SRAM4_Main,  //61
PQ_IP_C_SRAM1_Main,  //62
PQ_IP_C_SRAM2_Main,  //63
PQ_IP_C_SRAM3_Main,  //64
PQ_IP_C_SRAM4_Main,  //65
PQ_IP_VE_HSD_SRAM_Main,  //66
PQ_IP_VE_VSD_SRAM_Main,  //67
PQ_IP_422To444_Main,  //68
PQ_IP_Peaking_Main,  //69
PQ_IP_SwDriver_Main,  //70
PQ_IP_3x3_Main,  //71
PQ_IP_BRI_CR_Main,  //72
PQ_IP_Display_Main,  //73
 }   PQ_IPTYPE_Main;

typedef enum
{
PQ_IP_AFEC_COM_Main,  //0
PQ_IP_Comb_COM_Main,  //1
PQ_IP_SECAM_COM_Main,  //2
PQ_IP_SCinit_COM_Main,  //3
PQ_IP_CSC_COM_Main,  //4
PQ_IP_CSC_Dither_COM_Main,  //5
PQ_IP_IP422To444_COM_Main,  //6
PQ_IP_HSD_Y_COM_Main,  //7
PQ_IP_HSD_C_COM_Main,  //8
PQ_IP_VSD_COM_Main,  //9
PQ_IP_MemFormat_COM_Main,  //10
PQ_IP_444To422_COM_Main,  //11
PQ_IP_PreSNR_COM_Main,  //12
PQ_IP_DNR_COM_Main,  //13
PQ_IP_DNR_YOnly_COM_Main,  //14
PQ_IP_DNR_Y_COM_Main,  //15
PQ_IP_DNR_MED_COM_Main,  //16
PQ_IP_DNR_C_COM_Main,  //17
PQ_IP_DNR_sticky_solver_COM_Main,  //18
PQ_IP_PostCCS_COM_Main,  //19
PQ_IP_PNR_COM_Main,  //20
PQ_IP_PNR_Y_COM_Main,  //21
PQ_IP_PNR_C_COM_Main,  //22
PQ_IP_420CUP_COM_Main,  //23
PQ_IP_MADi_COM_Main,  //24
PQ_IP_MADi_Motion_COM_Main,  //25
PQ_IP_EODi_COM_Main,  //26
PQ_IP_Film_COM_Main,  //27
PQ_IP_Film32_COM_Main,  //28
PQ_IP_Film22_COM_Main,  //29
PQ_IP_Film_any_COM_Main,  //30
PQ_IP_SPF_COM_Main,  //31
PQ_IP_SPF_DBK_COM_Main,  //32
PQ_IP_SPF_DBK_MR_COM_Main,  //33
PQ_IP_SPF_DBK_BKN_COM_Main,  //34
PQ_IP_SPF_SNR_COM_Main,  //35
PQ_IP_SPF_SNR_MR_COM_Main,  //36
PQ_IP_SPF_MR_LPF_COM_Main,  //37
PQ_IP_SPF_NMR_Y_COM_Main,  //38
PQ_IP_SPF_NMR_Y_MR_COM_Main,  //39
PQ_IP_SPF_NMR_C_COM_Main,  //40
PQ_IP_DMS_COM_Main,  //41
PQ_IP_DMS_H_COM_Main,  //42
PQ_IP_DMS_V_COM_Main,  //43
PQ_IP_DMS_V_12L_COM_Main,  //44
PQ_IP_VSP_Y_COM_Main,  //45
PQ_IP_VSP_C_COM_Main,  //46
PQ_IP_VSP_Coring_COM_Main,  //47
PQ_IP_VSP_Dither_COM_Main,  //48
PQ_IP_VSP_PreVBound_COM_Main,  //49
PQ_IP_PreCTI_COM_Main,  //50
PQ_IP_HSP_Y_COM_Main,  //51
PQ_IP_HSP_C_COM_Main,  //52
PQ_IP_HSP_Coring_COM_Main,  //53
PQ_IP_HSP_DeRing_COM_Main,  //54
PQ_IP_HSP_Dither_COM_Main,  //55
PQ_IP_HnonLinear_COM_Main,  //56
PQ_IP_VE_HVSD_COM_Main,  //57
PQ_IP_SRAM1_COM_Main,  //58
PQ_IP_SRAM2_COM_Main,  //59
PQ_IP_SRAM3_COM_Main,  //60
PQ_IP_SRAM4_COM_Main,  //61
PQ_IP_C_SRAM1_COM_Main,  //62
PQ_IP_C_SRAM2_COM_Main,  //63
PQ_IP_C_SRAM3_COM_Main,  //64
PQ_IP_C_SRAM4_COM_Main,  //65
PQ_IP_VE_HSD_SRAM_COM_Main,  //66
PQ_IP_VE_VSD_SRAM_COM_Main,  //67
PQ_IP_422To444_COM_Main,  //68
PQ_IP_Peaking_COM_Main,  //69
PQ_IP_SwDriver_COM_Main,  //70
PQ_IP_3x3_COM_Main,  //71
PQ_IP_BRI_CR_COM_Main,  //72
PQ_IP_Display_COM_Main,  //73
PQ_IP_COM_NUMS_Main
} PQ_IP_COM_Group_Main;

typedef enum
{
PQ_IP_AFEC_ADCIN_Main,
PQ_IP_AFEC_RFIN_Main,
PQ_IP_AFEC_ADCIN_SECAM_Main,
PQ_IP_AFEC_NUMS_Main
} PQ_IP_AFEC_Group_Main;

typedef enum
{
PQ_IP_Comb_NTSC_AV_Main,
PQ_IP_Comb_PAL_AV_Main,
PQ_IP_Comb_NTSC_RF_Main,
PQ_IP_Comb_PAL_RF_Main,
PQ_IP_Comb_NTSC_SV_Main,
PQ_IP_Comb_PAL_SV_Main,
PQ_IP_Comb_NTSC443_Main,
PQ_IP_Comb_PAL60_Main,
PQ_IP_Comb_SECAM_Main,
PQ_IP_Comb_NUMS_Main
} PQ_IP_Comb_Group_Main;

typedef enum
{
PQ_IP_SECAM_ALL_Main,
PQ_IP_SECAM_NUMS_Main
} PQ_IP_SECAM_Group_Main;

typedef enum
{
PQ_IP_SCinit_Init_Main,
PQ_IP_SCinit_NUMS_Main
} PQ_IP_SCinit_Group_Main;

typedef enum
{
PQ_IP_CSC_OFF_Main,
PQ_IP_CSC_L_RGB2YCC_SD_Main,
PQ_IP_CSC_F_RGB2YCC_SD_Main,
PQ_IP_CSC_L_RGB2YCC_HD_Main,
PQ_IP_CSC_F_RGB2YCC_HD_Main,
PQ_IP_CSC_BT2020_NCL_Main,
PQ_IP_CSC_NUMS_Main
} PQ_IP_CSC_Group_Main;

typedef enum
{
PQ_IP_CSC_Dither_OFF_Main,
PQ_IP_CSC_Dither_L_RGB2YCC_SD_Main,
PQ_IP_CSC_Dither_F_RGB2YCC_SD_Main,
PQ_IP_CSC_Dither_L_RGB2YCC_HD_Main,
PQ_IP_CSC_Dither_F_RGB2YCC_HD_Main,
PQ_IP_CSC_Dither_BT2020_NCL_Main,
PQ_IP_CSC_Dither_NUMS_Main
} PQ_IP_CSC_Dither_Group_Main;

typedef enum
{
PQ_IP_IP422To444_OFF_Main,
PQ_IP_IP422To444_ON_Main,
PQ_IP_IP422To444_Bypass_Main,
PQ_IP_IP422To444_NUMS_Main
} PQ_IP_IP422To444_Group_Main;

typedef enum
{
PQ_IP_HSD_Y_OFF_Main,
PQ_IP_HSD_Y_CB_Main,
PQ_IP_HSD_Y_NUMS_Main
} PQ_IP_HSD_Y_Group_Main;

typedef enum
{
PQ_IP_HSD_C_OFF_Main,
PQ_IP_HSD_C_CB_Main,
PQ_IP_HSD_C_NUMS_Main
} PQ_IP_HSD_C_Group_Main;

typedef enum
{
PQ_IP_VSD_OFF_Main,
PQ_IP_VSD_CB_Main,
PQ_IP_VSD_Bilinear_Main,
PQ_IP_VSD_NUMS_Main
} PQ_IP_VSD_Group_Main;

typedef enum
{
PQ_IP_MemFormat_422MF_Main,
PQ_IP_MemFormat_444_10BIT_Main,
PQ_IP_MemFormat_444_8BIT_Main,
PQ_IP_MemFormat_422FBL_10BIT_Main,
PQ_IP_MemFormat_444FBL_10BIT_Main,
PQ_IP_MemFormat_444FBL_8BIT_Main,
PQ_IP_MemFormat_NUMS_Main
} PQ_IP_MemFormat_Group_Main;

typedef enum
{
PQ_IP_444To422_OFF_Main,
PQ_IP_444To422_ON_Main,
PQ_IP_444To422_NUMS_Main
} PQ_IP_444To422_Group_Main;

typedef enum
{
PQ_IP_PreSNR_OFF_Main,
PQ_IP_PreSNR_PS_1_Main,
PQ_IP_PreSNR_PS_2_Main,
PQ_IP_PreSNR_PS_3_Main,
PQ_IP_PreSNR_PS_4_Main,
PQ_IP_PreSNR_PS_5_Main,
PQ_IP_PreSNR_PS_6_Main,
PQ_IP_PreSNR_PS_7_Main,
PQ_IP_PreSNR_PS_8_Main,
PQ_IP_PreSNR_PS_9_Main,
PQ_IP_PreSNR_PS_10_Main,
PQ_IP_PreSNR_PS_11_Main,
PQ_IP_PreSNR_PS_12_Main,
PQ_IP_PreSNR_PS_13_Main,
PQ_IP_PreSNR_PS_14_Main,
PQ_IP_PreSNR_PS_15_Main,
PQ_IP_PreSNR_PS_16_Main,
PQ_IP_PreSNR_NUMS_Main
} PQ_IP_PreSNR_Group_Main;

typedef enum
{
PQ_IP_DNR_NR_OFF_Main,
PQ_IP_DNR_DNR_OFF_Main,
PQ_IP_DNR_ON_Main,
PQ_IP_DNR_ON_PT_Main,
PQ_IP_DNR_NUMS_Main
} PQ_IP_DNR_Group_Main;

typedef enum
{
PQ_IP_DNR_YOnly_OFF_Main,
PQ_IP_DNR_YOnly_ON_Main,
PQ_IP_DNR_YOnly_NUMS_Main
} PQ_IP_DNR_YOnly_Group_Main;

typedef enum
{
PQ_IP_DNR_Y_OFF_Main,
PQ_IP_DNR_Y_DY_1_Main,
PQ_IP_DNR_Y_DY_1_1_Main,
PQ_IP_DNR_Y_DY_2_Main,
PQ_IP_DNR_Y_DY_2_1_Main,
PQ_IP_DNR_Y_DY_3_Main,
PQ_IP_DNR_Y_DY_3_1_Main,
PQ_IP_DNR_Y_DY_4_Main,
PQ_IP_DNR_Y_DY_4_1_Main,
PQ_IP_DNR_Y_DY_5_Main,
PQ_IP_DNR_Y_DY_5_1_Main,
PQ_IP_DNR_Y_DY_6_Main,
PQ_IP_DNR_Y_DY_6_1_Main,
PQ_IP_DNR_Y_DY_7_Main,
PQ_IP_DNR_Y_DY_7_1_Main,
PQ_IP_DNR_Y_DY_8_Main,
PQ_IP_DNR_Y_DY_8_1_Main,
PQ_IP_DNR_Y_DY_9_Main,
PQ_IP_DNR_Y_DY_9_1_Main,
PQ_IP_DNR_Y_DY_10_Main,
PQ_IP_DNR_Y_DY_10_1_Main,
PQ_IP_DNR_Y_DY_nonlinear0_Main,
PQ_IP_DNR_Y_DY_nonlinear01_Main,
PQ_IP_DNR_Y_DY_nonlinear1_Main,
PQ_IP_DNR_Y_DY_nonlinear12_Main,
PQ_IP_DNR_Y_DY_nonlinear2_Main,
PQ_IP_DNR_Y_DY_nonlinear23_Main,
PQ_IP_DNR_Y_DY_nonlinear3_Main,
PQ_IP_DNR_Y_U1_Main,
PQ_IP_DNR_Y_U2_Main,
PQ_IP_DNR_Y_U3_Main,
PQ_IP_DNR_Y_U4_Main,
PQ_IP_DNR_Y_U5_Main,
PQ_IP_DNR_Y_U6_Main,
PQ_IP_DNR_Y_U7_Main,
PQ_IP_DNR_Y_U8_Main,
PQ_IP_DNR_Y_NUMS_Main
} PQ_IP_DNR_Y_Group_Main;

typedef enum
{
PQ_IP_DNR_MED_OFF_Main,
PQ_IP_DNR_MED_M_1_Main,
PQ_IP_DNR_MED_NUMS_Main
} PQ_IP_DNR_MED_Group_Main;

typedef enum
{
PQ_IP_DNR_C_OFF_Main,
PQ_IP_DNR_C_DC_1_Main,
PQ_IP_DNR_C_DC_1_1_Main,
PQ_IP_DNR_C_DC_2_Main,
PQ_IP_DNR_C_DC_2_1_Main,
PQ_IP_DNR_C_DC_3_Main,
PQ_IP_DNR_C_DC_3_1_Main,
PQ_IP_DNR_C_DC_4_Main,
PQ_IP_DNR_C_DC_4_1_Main,
PQ_IP_DNR_C_DC_5_Main,
PQ_IP_DNR_C_DC_5_1_Main,
PQ_IP_DNR_C_DC_6_Main,
PQ_IP_DNR_C_DC_6_1_Main,
PQ_IP_DNR_C_DC_7_Main,
PQ_IP_DNR_C_DC_7_1_Main,
PQ_IP_DNR_C_DC_8_Main,
PQ_IP_DNR_C_DC_8_1_Main,
PQ_IP_DNR_C_DC_9_Main,
PQ_IP_DNR_C_DC_9_1_Main,
PQ_IP_DNR_C_DC_10_Main,
PQ_IP_DNR_C_DC_10_1_Main,
PQ_IP_DNR_C_DC_nonlinear1_Main,
PQ_IP_DNR_C_DC_nonlinear2_Main,
PQ_IP_DNR_C_DC_nonlinear3_Main,
PQ_IP_DNR_C_U1_Main,
PQ_IP_DNR_C_U2_Main,
PQ_IP_DNR_C_U3_Main,
PQ_IP_DNR_C_U4_Main,
PQ_IP_DNR_C_U5_Main,
PQ_IP_DNR_C_U6_Main,
PQ_IP_DNR_C_U7_Main,
PQ_IP_DNR_C_U8_Main,
PQ_IP_DNR_C_NUMS_Main
} PQ_IP_DNR_C_Group_Main;

typedef enum
{
PQ_IP_DNR_sticky_solver_OFF_Main,
PQ_IP_DNR_sticky_solver_8bit_Main,
PQ_IP_DNR_sticky_solver_6bit_Main,
PQ_IP_DNR_sticky_solver_No_Comp_Main,
PQ_IP_DNR_sticky_solver_ori_round_dither_Main,
PQ_IP_DNR_sticky_solver_NUMS_Main
} PQ_IP_DNR_sticky_solver_Group_Main;

typedef enum
{
PQ_IP_PostCCS_OFF_Main,
PQ_IP_PostCCS_PC_0_Main,
PQ_IP_PostCCS_PC_1_Main,
PQ_IP_PostCCS_PC_2_Main,
PQ_IP_PostCCS_PC_3_Main,
PQ_IP_PostCCS_PC_4_Main,
PQ_IP_PostCCS_PC_5_Main,
PQ_IP_PostCCS_NUMS_Main
} PQ_IP_PostCCS_Group_Main;

typedef enum
{
PQ_IP_PNR_OFF_Main,
PQ_IP_PNR_ON_Main,
PQ_IP_PNR_AVG_ON_Main,
PQ_IP_PNR_NUMS_Main
} PQ_IP_PNR_Group_Main;

typedef enum
{
PQ_IP_PNR_Y_OFF_Main,
PQ_IP_PNR_Y_PY1_Main,
PQ_IP_PNR_Y_NUMS_Main
} PQ_IP_PNR_Y_Group_Main;

typedef enum
{
PQ_IP_PNR_C_OFF_Main,
PQ_IP_PNR_C_PC1_Main,
PQ_IP_PNR_C_NUMS_Main
} PQ_IP_PNR_C_Group_Main;

typedef enum
{
PQ_IP_420CUP_OFF_Main,
PQ_IP_420CUP_ON_Main,
PQ_IP_420CUP_NUMS_Main
} PQ_IP_420CUP_Group_Main;

typedef enum
{
PQ_IP_MADi_25_4R_Main,
PQ_IP_MADi_24_4R_Main,
PQ_IP_MADi_25_2R_Main,
PQ_IP_MADi_24_2R_Main,
PQ_IP_MADi_27_4R_Main,
PQ_IP_MADi_27_2R_Main,
PQ_IP_MADi_27_2R_Y8Only_Main,
PQ_IP_MADi_FBL_EODi_Main,
PQ_IP_MADi_P_MODE10_Main,
PQ_IP_MADi_P_MODE8_Main,
PQ_IP_MADi_24_RFBL_NFilm_Main,
PQ_IP_MADi_24_RFBL_Film_Main,
PQ_IP_MADi_FBL_NO_DNR_Main,
PQ_IP_MADi_FBL_MIU_Main,
PQ_IP_MADi_P_MODE8_NO_MIU_Main,
PQ_IP_MADi_P_MODE8_3Frame_Main,
PQ_IP_MADi_NUMS_Main
} PQ_IP_MADi_Group_Main;

typedef enum
{
PQ_IP_MADi_Motion_MOT_4R_5_Main,
PQ_IP_MADi_Motion_MOT_4R_6_Main,
PQ_IP_MADi_Motion_MOT_4R_7_Main,
PQ_IP_MADi_Motion_MOT_2R_Main,
PQ_IP_MADi_Motion_MOT_PMODE_Main,
PQ_IP_MADi_Motion_NUMS_Main
} PQ_IP_MADi_Motion_Group_Main;

typedef enum
{
PQ_IP_EODi_OFF_Main,
PQ_IP_EODi_SD_Main,
PQ_IP_EODi_NUMS_Main
} PQ_IP_EODi_Group_Main;

typedef enum
{
PQ_IP_Film_OFF_Main,
PQ_IP_Film_SD_2_Main,
PQ_IP_Film_SD_1_Main,
PQ_IP_Film_SD_3_Main,
PQ_IP_Film_HD_2_Main,
PQ_IP_Film_HD_1_Main,
PQ_IP_Film_HD_3_Main,
PQ_IP_Film_NUMS_Main
} PQ_IP_Film_Group_Main;

typedef enum
{
PQ_IP_Film32_OFF_Main,
PQ_IP_Film32_SD_1_Main,
PQ_IP_Film32_SD_2_Main,
PQ_IP_Film32_SD_3_Main,
PQ_IP_Film32_SD_4_Main,
PQ_IP_Film32_SD_5_Main,
PQ_IP_Film32_SD_6_Main,
PQ_IP_Film32_SD_7_Main,
PQ_IP_Film32_SD_8_Main,
PQ_IP_Film32_SD_9_Main,
PQ_IP_Film32_SD_10_Main,
PQ_IP_Film32_HD_1_Main,
PQ_IP_Film32_HD_2_Main,
PQ_IP_Film32_HD_3_Main,
PQ_IP_Film32_HD_4_Main,
PQ_IP_Film32_HD_5_Main,
PQ_IP_Film32_HD_6_Main,
PQ_IP_Film32_NUMS_Main
} PQ_IP_Film32_Group_Main;

typedef enum
{
PQ_IP_Film22_OFF_Main,
PQ_IP_Film22_SD_1_Main,
PQ_IP_Film22_SD_2_Main,
PQ_IP_Film22_SD_3_Main,
PQ_IP_Film22_SD_4_Main,
PQ_IP_Film22_SD_5_Main,
PQ_IP_Film22_SD_6_Main,
PQ_IP_Film22_SD_7_Main,
PQ_IP_Film22_SD_8_Main,
PQ_IP_Film22_SD_9_Main,
PQ_IP_Film22_SD_10_Main,
PQ_IP_Film22_HD_1_Main,
PQ_IP_Film22_HD_2_Main,
PQ_IP_Film22_HD_3_Main,
PQ_IP_Film22_HD_4_Main,
PQ_IP_Film22_HD_5_Main,
PQ_IP_Film22_HD_6_Main,
PQ_IP_Film22_NUMS_Main
} PQ_IP_Film22_Group_Main;

typedef enum
{
PQ_IP_Film_any_OFF_Main,
PQ_IP_Film_any_SD_1_Main,
PQ_IP_Film_any_SD_2_Main,
PQ_IP_Film_any_SD_3_Main,
PQ_IP_Film_any_SD_4_Main,
PQ_IP_Film_any_SD_5_Main,
PQ_IP_Film_any_SD_6_Main,
PQ_IP_Film_any_SD_7_Main,
PQ_IP_Film_any_SD_8_Main,
PQ_IP_Film_any_SD_9_Main,
PQ_IP_Film_any_SD_10_Main,
PQ_IP_Film_any_HD_1_Main,
PQ_IP_Film_any_HD_2_Main,
PQ_IP_Film_any_HD_3_Main,
PQ_IP_Film_any_HD_4_Main,
PQ_IP_Film_any_HD_5_Main,
PQ_IP_Film_any_HD_6_Main,
PQ_IP_Film_any_NUMS_Main
} PQ_IP_Film_any_Group_Main;

typedef enum
{
PQ_IP_SPF_OFF_Main,
PQ_IP_SPF_ON_Main,
PQ_IP_SPF_NUMS_Main
} PQ_IP_SPF_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_OFF_Main,
PQ_IP_SPF_DBK_E2S2_Main,
PQ_IP_SPF_DBK_NUMS_Main
} PQ_IP_SPF_DBK_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_MR_OFF_Main,
PQ_IP_SPF_DBK_MR_ON_Main,
PQ_IP_SPF_DBK_MR_NUMS_Main
} PQ_IP_SPF_DBK_MR_Group_Main;

typedef enum
{
PQ_IP_SPF_DBK_BKN_OFF_Main,
PQ_IP_SPF_DBK_BKN_T0_Main,
PQ_IP_SPF_DBK_BKN_T1_Main,
PQ_IP_SPF_DBK_BKN_T2_Main,
PQ_IP_SPF_DBK_BKN_AV_Main,
PQ_IP_SPF_DBK_BKN_2xsampling_Main,
PQ_IP_SPF_DBK_BKN_NUMS_Main
} PQ_IP_SPF_DBK_BKN_Group_Main;

typedef enum
{
PQ_IP_SPF_SNR_OFF_Main,
PQ_IP_SPF_SNR_E1S1_Main,
PQ_IP_SPF_SNR_E1S2_Main,
PQ_IP_SPF_SNR_E2S1_Main,
PQ_IP_SPF_SNR_E3S1_Main,
PQ_IP_SPF_SNR_E4S1_Main,
PQ_IP_SPF_SNR_E4S2_Main,
PQ_IP_SPF_SNR_E4S3_Main,
PQ_IP_SPF_SNR_SEC1_Main,
PQ_IP_SPF_SNR_SEC2_Main,
PQ_IP_SPF_SNR_SEC3_Main,
PQ_IP_SPF_SNR_NUMS_Main
} PQ_IP_SPF_SNR_Group_Main;

typedef enum
{
PQ_IP_SPF_SNR_MR_OFF_Main,
PQ_IP_SPF_SNR_MR_ON_Main,
PQ_IP_SPF_SNR_MR_NUMS_Main
} PQ_IP_SPF_SNR_MR_Group_Main;

typedef enum
{
PQ_IP_SPF_MR_LPF_OFF_Main,
PQ_IP_SPF_MR_LPF_LPF3x3_Main,
PQ_IP_SPF_MR_LPF_NUMS_Main
} PQ_IP_SPF_MR_LPF_Group_Main;

typedef enum
{
PQ_IP_SPF_NMR_Y_OFF_Main,
PQ_IP_SPF_NMR_Y_S0_Main,
PQ_IP_SPF_NMR_Y_S1_Main,
PQ_IP_SPF_NMR_Y_S2_Main,
PQ_IP_SPF_NMR_Y_S3_Main,
PQ_IP_SPF_NMR_Y_NUMS_Main
} PQ_IP_SPF_NMR_Y_Group_Main;

typedef enum
{
PQ_IP_SPF_NMR_Y_MR_OFF_Main,
PQ_IP_SPF_NMR_Y_MR_ON_Main,
PQ_IP_SPF_NMR_Y_MR_NUMS_Main
} PQ_IP_SPF_NMR_Y_MR_Group_Main;

typedef enum
{
PQ_IP_SPF_NMR_C_OFF_Main,
PQ_IP_SPF_NMR_C_S1_Main,
PQ_IP_SPF_NMR_C_S2_Main,
PQ_IP_SPF_NMR_C_S3_Main,
PQ_IP_SPF_NMR_C_NUMS_Main
} PQ_IP_SPF_NMR_C_Group_Main;

typedef enum
{
PQ_IP_DMS_OFF_Main,
PQ_IP_DMS_ON_Main,
PQ_IP_DMS_NUMS_Main
} PQ_IP_DMS_Group_Main;

typedef enum
{
PQ_IP_DMS_H_OFF_Main,
PQ_IP_DMS_H_S0_Main,
PQ_IP_DMS_H_S1_Main,
PQ_IP_DMS_H_S2_Main,
PQ_IP_DMS_H_S3_Main,
PQ_IP_DMS_H_DTV_S1_Main,
PQ_IP_DMS_H_DTV_S2_Main,
PQ_IP_DMS_H_DTV_S3_Main,
PQ_IP_DMS_H_DTV_S4_Main,
PQ_IP_DMS_H_DTV_S5_Main,
PQ_IP_DMS_H_S1_HD_Main,
PQ_IP_DMS_H_S2_HD_Main,
PQ_IP_DMS_H_S3_HD_Main,
PQ_IP_DMS_H_SEC_S1_Main,
PQ_IP_DMS_H_S4_N_Main,
PQ_IP_DMS_H_S5_P_Main,
PQ_IP_DMS_H_NUMS_Main
} PQ_IP_DMS_H_Group_Main;

typedef enum
{
PQ_IP_DMS_V_OFF_Main,
PQ_IP_DMS_V_S1_Main,
PQ_IP_DMS_V_S2_Main,
PQ_IP_DMS_V_S3_Main,
PQ_IP_DMS_V_HD_Main,
PQ_IP_DMS_V_DTV_S1_Main,
PQ_IP_DMS_V_DTV_S2_Main,
PQ_IP_DMS_V_DTV_S3_Main,
PQ_IP_DMS_V_S1_HD_Main,
PQ_IP_DMS_V_S2_HD_Main,
PQ_IP_DMS_V_S3_HD_Main,
PQ_IP_DMS_V_S4_N_Main,
PQ_IP_DMS_V_S5_P_Main,
PQ_IP_DMS_V_NUMS_Main
} PQ_IP_DMS_V_Group_Main;

typedef enum
{
PQ_IP_DMS_V_12L_OFF_Main,
PQ_IP_DMS_V_12L_ON_Main,
PQ_IP_DMS_V_12L_NUMS_Main
} PQ_IP_DMS_V_12L_Group_Main;

typedef enum
{
PQ_IP_VSP_Y_Bypass_Main,
PQ_IP_VSP_Y_Bilinear_Main,
PQ_IP_VSP_Y_SRAM_1_4Tap_Main,
PQ_IP_VSP_Y_SRAM_2_4Tap_Main,
PQ_IP_VSP_Y_SRAM_1_6Tap_Main,
PQ_IP_VSP_Y_SRAM_2_6Tap_Main,
PQ_IP_VSP_Y_SRAM_1_8Tap_Main,
PQ_IP_VSP_Y_SRAM_2_8Tap_Main,
PQ_IP_VSP_Y_NUMS_Main
} PQ_IP_VSP_Y_Group_Main;

typedef enum
{
PQ_IP_VSP_C_Bypass_Main,
PQ_IP_VSP_C_Bilinear_Main,
PQ_IP_VSP_C_C_SRAM_1_Main,
PQ_IP_VSP_C_C_SRAM_2_Main,
PQ_IP_VSP_C_C_SRAM_3_Main,
PQ_IP_VSP_C_C_SRAM_4_Main,
PQ_IP_VSP_C_SRAM_1_4Tap_Main,
PQ_IP_VSP_C_SRAM_2_4Tap_Main,
PQ_IP_VSP_C_NUMS_Main
} PQ_IP_VSP_C_Group_Main;

typedef enum
{
PQ_IP_VSP_Coring_OFF_Main,
PQ_IP_VSP_Coring_Y_Coring_1_Main,
PQ_IP_VSP_Coring_Y_Coring_2_Main,
PQ_IP_VSP_Coring_Y_Coring_3_Main,
PQ_IP_VSP_Coring_Y_Coring_4_Main,
PQ_IP_VSP_Coring_Y_Coring_5_Main,
PQ_IP_VSP_Coring_NUMS_Main
} PQ_IP_VSP_Coring_Group_Main;

typedef enum
{
PQ_IP_VSP_Dither_OFF_Main,
PQ_IP_VSP_Dither_ON_Main,
PQ_IP_VSP_Dither_NUMS_Main
} PQ_IP_VSP_Dither_Group_Main;

typedef enum
{
PQ_IP_VSP_PreVBound_OFF_Main,
PQ_IP_VSP_PreVBound_ON_Main,
PQ_IP_VSP_PreVBound_NUMS_Main
} PQ_IP_VSP_PreVBound_Group_Main;

typedef enum
{
PQ_IP_PreCTI_OFF_Main,
PQ_IP_PreCTI_CTI_N_0_Main,
PQ_IP_PreCTI_CTI_P_0_Main,
PQ_IP_PreCTI_CTI_P_S5_Main,
PQ_IP_PreCTI_CTI_N_S5_Main,
PQ_IP_PreCTI_CTI_P_S3_MS5_Main,
PQ_IP_PreCTI_CTI_N_S2_MS5_Main,
PQ_IP_PreCTI_CTI_S0_MS5_YREF04_Main,
PQ_IP_PreCTI_CTI_S0_MS5_YREF08_Main,
PQ_IP_PreCTI_CTI_S0_MS5_YREF0C_Main,
PQ_IP_PreCTI_CTI_MS3_CLF_low_Main,
PQ_IP_PreCTI_CTI_MS3_CLF_mid_Main,
PQ_IP_PreCTI_CTI_MS3_CLF_high_Main,
PQ_IP_PreCTI_NUMS_Main
} PQ_IP_PreCTI_Group_Main;

typedef enum
{
PQ_IP_HSP_Y_Bypass_Main,
PQ_IP_HSP_Y_Bilinear_Main,
PQ_IP_HSP_Y_SRAM_1_4Tap_Main,
PQ_IP_HSP_Y_SRAM_2_4Tap_Main,
PQ_IP_HSP_Y_SRAM_1_6Tap_Main,
PQ_IP_HSP_Y_SRAM_2_6Tap_Main,
PQ_IP_HSP_Y_SRAM_2_8Tap_Main,
PQ_IP_HSP_Y_NUMS_Main
} PQ_IP_HSP_Y_Group_Main;

typedef enum
{
PQ_IP_HSP_C_Bypass_Main,
PQ_IP_HSP_C_Bilinear_Main,
PQ_IP_HSP_C_C_SRAM_1_Main,
PQ_IP_HSP_C_C_SRAM_2_Main,
PQ_IP_HSP_C_C_SRAM_3_Main,
PQ_IP_HSP_C_C_SRAM_4_Main,
PQ_IP_HSP_C_SRAM_1_4Tap_Main,
PQ_IP_HSP_C_SRAM_2_4Tap_Main,
PQ_IP_HSP_C_NUMS_Main
} PQ_IP_HSP_C_Group_Main;

typedef enum
{
PQ_IP_HSP_Coring_OFF_Main,
PQ_IP_HSP_Coring_Y_Coring_1_Main,
PQ_IP_HSP_Coring_Y_Coring_2_Main,
PQ_IP_HSP_Coring_Y_Coring_3_Main,
PQ_IP_HSP_Coring_Y_Coring_4_Main,
PQ_IP_HSP_Coring_Y_Coring_5_Main,
PQ_IP_HSP_Coring_NUMS_Main
} PQ_IP_HSP_Coring_Group_Main;

typedef enum
{
PQ_IP_HSP_DeRing_OFF_Main,
PQ_IP_HSP_DeRing_DR1_Main,
PQ_IP_HSP_DeRing_NUMS_Main
} PQ_IP_HSP_DeRing_Group_Main;

typedef enum
{
PQ_IP_HSP_Dither_OFF_Main,
PQ_IP_HSP_Dither_ON_Main,
PQ_IP_HSP_Dither_NUMS_Main
} PQ_IP_HSP_Dither_Group_Main;

typedef enum
{
PQ_IP_HnonLinear_OFF_Main,
PQ_IP_HnonLinear_H_1366_0_Main,
PQ_IP_HnonLinear_H_1366_1_Main,
PQ_IP_HnonLinear_H_1366_2_Main,
PQ_IP_HnonLinear_H_1440_Main,
PQ_IP_HnonLinear_H_1680_Main,
PQ_IP_HnonLinear_H_1920_0_Main,
PQ_IP_HnonLinear_H_1920_1_Main,
PQ_IP_HnonLinear_H_1920_2_Main,
PQ_IP_HnonLinear_H_3840_1_Main,
PQ_IP_HnonLinear_H_3840_2_Main,
PQ_IP_HnonLinear_H_3840_3_Main,
PQ_IP_HnonLinear_H_3840_4_Main,
PQ_IP_HnonLinear_NUMS_Main
} PQ_IP_HnonLinear_Group_Main;

typedef enum
{
PQ_IP_VE_HVSD_Bypass_Main,
PQ_IP_VE_HVSD_Bilinear_Main,
PQ_IP_VE_HVSD_SRAM_TABLE0_Main,
PQ_IP_VE_HVSD_SRAM_TABLE1_Main,
PQ_IP_VE_HVSD_NUMS_Main
} PQ_IP_VE_HVSD_Group_Main;

typedef enum
{
PQ_IP_SRAM1_InvSinc8Tc4p4Fc65Apass0001Astop60_Main,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc75Apass0001Astop60_Main,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc85Apass0001Astop60_Main,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc95Apass0001Astop60_Main,
PQ_IP_SRAM1_InvSinc8Tc4p4Fc105Apass0001Astop60_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc57Apass01Astop60_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc67Apass01Astop60_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc77Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc81Apass01Astop40_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc87Apass01Astop50_Main,
PQ_IP_SRAM1_InvSinc6Tc4p4Fc94Apass01Astop60_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc60Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc65Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Apass0001Astop40_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Apass01Astop50_Main,
PQ_IP_SRAM1_InvSinc4Tc4p4Fc90Apass0001Astop40_Main,
PQ_IP_SRAM1_NUMS_Main
} PQ_IP_SRAM1_Group_Main;

typedef enum
{
PQ_IP_SRAM2_InvSinc8Tc4p4Fc65Apass0001Astop60_Main,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc75Apass0001Astop60_Main,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc85Apass0001Astop60_Main,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc95Apass0001Astop60_Main,
PQ_IP_SRAM2_InvSinc8Tc4p4Fc105Apass0001Astop60_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc57Apass01Astop60_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc77Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc81Apass01Astop40_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc87Apass01Astop50_Main,
PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc60Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc65Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Apass01Astop50_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc90Apass0001Astop40_Main,
PQ_IP_SRAM2_InvSinc4Tc4p4Fc100Apass0001Astop40_Main,
PQ_IP_SRAM2_NUMS_Main
} PQ_IP_SRAM2_Group_Main;

typedef enum
{
PQ_IP_SRAM3_InvSinc8Tc4p4Fc65Apass0001Astop60_Main,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc75Apass0001Astop60_Main,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc85Apass0001Astop60_Main,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc95Apass0001Astop60_Main,
PQ_IP_SRAM3_InvSinc8Tc4p4Fc105Apass0001Astop60_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc57Apass01Astop60_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc67Apass01Astop60_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc77Apass0001Astop40_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc81Apass01Astop40_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc87Apass01Astop50_Main,
PQ_IP_SRAM3_InvSinc6Tc4p4Fc94Apass01Astop60_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc60Apass0001Astop40_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc65Apass0001Astop40_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc75Apass0001Astop40_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc85Apass01Astop50_Main,
PQ_IP_SRAM3_InvSinc4Tc4p4Fc90Apass0001Astop40_Main,
PQ_IP_SRAM3_NUMS_Main
} PQ_IP_SRAM3_Group_Main;

typedef enum
{
PQ_IP_SRAM4_InvSinc8Tc4p4Fc65Apass0001Astop60_Main,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc75Apass0001Astop60_Main,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc85Apass0001Astop60_Main,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc95Apass0001Astop60_Main,
PQ_IP_SRAM4_InvSinc8Tc4p4Fc105Apass0001Astop60_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc57Apass01Astop60_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc67Apass01Astop60_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc77Apass0001Astop40_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc81Apass01Astop40_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc87Apass01Astop50_Main,
PQ_IP_SRAM4_InvSinc6Tc4p4Fc94Apass01Astop60_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc50Apass01Astop55_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc60Apass0001Astop40_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc65Apass0001Astop40_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc75Apass0001Astop40_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc75Apass0001Astop40G12_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc85Apass01Astop50_Main,
PQ_IP_SRAM4_InvSinc4Tc4p4Fc90Apass0001Astop40_Main,
PQ_IP_SRAM4_NUMS_Main
} PQ_IP_SRAM4_Group_Main;

typedef enum
{
PQ_IP_C_SRAM1_C2121_Main,
PQ_IP_C_SRAM1_C121_Main,
PQ_IP_C_SRAM1_C161_Main,
PQ_IP_C_SRAM1_C111_Main,
PQ_IP_C_SRAM1_NUMS_Main
} PQ_IP_C_SRAM1_Group_Main;

typedef enum
{
PQ_IP_C_SRAM2_C2121_Main,
PQ_IP_C_SRAM2_C121_Main,
PQ_IP_C_SRAM2_C161_Main,
PQ_IP_C_SRAM2_C111_Main,
PQ_IP_C_SRAM2_NUMS_Main
} PQ_IP_C_SRAM2_Group_Main;

typedef enum
{
PQ_IP_C_SRAM3_C2121_Main,
PQ_IP_C_SRAM3_C121_Main,
PQ_IP_C_SRAM3_C161_Main,
PQ_IP_C_SRAM3_C111_Main,
PQ_IP_C_SRAM3_NUMS_Main
} PQ_IP_C_SRAM3_Group_Main;

typedef enum
{
PQ_IP_C_SRAM4_C2121_Main,
PQ_IP_C_SRAM4_C121_Main,
PQ_IP_C_SRAM4_C161_Main,
PQ_IP_C_SRAM4_C111_Main,
PQ_IP_C_SRAM4_NUMS_Main
} PQ_IP_C_SRAM4_Group_Main;

typedef enum
{
PQ_IP_VE_HSD_SRAM_C121_128_Main,
PQ_IP_VE_HSD_SRAM_C565_Main,
PQ_IP_VE_HSD_SRAM_FC100_Main,
PQ_IP_VE_HSD_SRAM_AVG_Main,
PQ_IP_VE_HSD_SRAM_FC102_Main,
PQ_IP_VE_HSD_SRAM_FC101_Main,
PQ_IP_VE_HSD_SRAM_FC103_Main,
PQ_IP_VE_HSD_SRAM_FC104_Main,
PQ_IP_VE_HSD_SRAM_FC105_Main,
PQ_IP_VE_HSD_SRAM_FC110_Main,
PQ_IP_VE_HSD_SRAM_NUMS_Main
} PQ_IP_VE_HSD_SRAM_Group_Main;

typedef enum
{
PQ_IP_VE_VSD_SRAM_C121_128_Main,
PQ_IP_VE_VSD_SRAM_C565_Main,
PQ_IP_VE_VSD_SRAM_FC100_Main,
PQ_IP_VE_VSD_SRAM_AVG_Main,
PQ_IP_VE_VSD_SRAM_NUMS_Main
} PQ_IP_VE_VSD_SRAM_Group_Main;

typedef enum
{
PQ_IP_422To444_ON_Main,
PQ_IP_422To444_OFF_Main,
PQ_IP_422To444_SIMPLE_Main,
PQ_IP_422To444_BICUBIC_Main,
PQ_IP_422To444_Bypass_Main,
PQ_IP_422To444_NUMS_Main
} PQ_IP_422To444_Group_Main;

typedef enum
{
PQ_IP_Peaking_OFF_Main,
PQ_IP_Peaking_PC_mode_Main,
PQ_IP_Peaking_SD_Main,
PQ_IP_Peaking_HD_Main,
PQ_IP_Peaking_NUMS_Main
} PQ_IP_Peaking_Group_Main;

typedef enum
{
PQ_IP_SwDriver_ALL_Main,
PQ_IP_SwDriver_NUMS_Main
} PQ_IP_SwDriver_Group_Main;

typedef enum
{
PQ_IP_3x3_OFF_Main,
PQ_IP_3x3_NUMS_Main
} PQ_IP_3x3_Group_Main;

typedef enum
{
PQ_IP_BRI_CR_ALL_Main,
PQ_IP_BRI_CR_HDMI_Main,
PQ_IP_BRI_CR_NUMS_Main
} PQ_IP_BRI_CR_Group_Main;

typedef enum
{
PQ_IP_Display_ALL_Main,
PQ_IP_Display_HDMI_Main,
PQ_IP_Display_NUMS_Main
} PQ_IP_Display_Group_Main;

extern code U8 MST_SkipRule_IP_Main[PQ_IP_NUM_Main];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Main[];
extern code U8 QMAP_1920_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1920_PIP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1920_POP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1366_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1366_PIP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_1366_POP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_4K2K_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_4K2K_PIP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];
extern code U8 QMAP_4K2K_POP_Main[QM_INPUTTYPE_NUM_Main][PQ_IP_NUM_Main];

#endif
