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
// Mooney
// 2016/8/16 20:29
// 2008/7/1 10:31
//****************************************************

#ifndef _MOONEY_MAIN_EX_C_
#define _MOONEY_MAIN_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Main_Ex[PQ_IP_NUM_Main_Ex]=
{
    /* AFEC_no_comm */ 0, /* HISDNR */ 0, /* SPF_DBK */ 0, /* DMS */ 0,
    /* DMS_H */ 0, /* VIP_Peaking_adptive */ 0, /* VIP_Peaking_Pcoring_ad_C */ 0, /* VIP_Peaking_gain_by_Cedge */ 0,
    /* VIP_ICC_Target */ 0, /* VIP_IBC_weight */ 0, /* VIP_ACK */ 0, /* VIP_Ygain_ColorAdaptive */ 0,
    /* VIP_Cgain_ColorAdaptive */ 0, /* VIP_Ygain_NonLinear */ 0, /* VIP_Cgain_NonLinear */ 0,
};

#endif
//****************************************************
// AFEC_no_comm
//****************************************************
code U8 MST_AFEC_no_comm_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_AFEC_no_comm_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_AFEC_no_comm_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_BK35_2A_H), 0x04, 0x04/*ON*/, },
 { PQ_MAP_REG(REG_BK35_2A_H), 0x04, 0x00/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HISDNR
//****************************************************
code U8 MST_HISDNR_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HISDNR_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HISDNR_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// SPF_DBK
//****************************************************
code U8 MST_SPF_DBK_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SPF_DBK_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS
//****************************************************
code U8 MST_DMS_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x02, 0x02 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_H
//****************************************************
code U8 MST_DMS_H_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_19_L), 0x07, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_H_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_H_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x10, 0x00/*J0*/,
                              0x10/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_adptive
//****************************************************
code U8 MST_VIP_Peaking_adptive_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK1C_6A_L), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1C_6A_H), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1C_6B_L), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1C_6B_H), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1C_6C_L), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1C_6D_H), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK19_46_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_46_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_47_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_47_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4E_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4E_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4F_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4F_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_adptive_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_adptive_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_31_L), 0x9F, 0x03/*$RF_NTSC*/,
                              0x93/*$AV_NTSC*/,
                              0x03/*$AV_PAL*/,
                              0x13/*$480i*/,
                              0x11/*$720p*/,
                              0x93/*$1080i*/,
                              0x03/*$H_480i*/,
                              0x11/*$H_576i*/,
                              0x11/*$H_720p*/,
                              0x93/*$H_1080i*/,
                              0x03/*$DT_MPEG2_480is*/,
                              0x11/*$DT_MPEG2_576i*/,
                              0x03/*$DT_MPEG2_1080i*/,
                              0x03/*$DT_H264_480is*/,
                              0x11/*$DT_H264_576p*/,
                              0x11/*$DT_H264_720p*/,
                              0x03/*$DT_H264_1080i*/,
                              0x11/*$DT_H264_1080p*/,
                              0x11/*$PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_40_L), 0x3F, 0x18/*RF_NTSC*/,
                              0x18/*AV_NTSC*/,
                              0x18/*AV_PAL*/,
                              0x26/*480i*/,
                              0x16/*720p*/,
                              0x16/*1080i*/,
                              0x16/*H_480i*/,
                              0x16/*H_576i*/,
                              0x12/*H_720p*/,
                              0x10/*H_1080i*/,
                              0x16/*DT_MPEG2_480is*/,
                              0x16/*DT_MPEG2_576i*/,
                              0x16/*DT_MPEG2_1080i*/,
                              0x16/*DT_H264_480is*/,
                              0x16/*DT_H264_576p*/,
                              0x16/*DT_H264_720p*/,
                              0x16/*DT_H264_1080i*/,
                              0x16/*DT_H264_1080p*/,
                              0x2B/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_40_H), 0x3F, 0x1B/*RF_NTSC*/,
                              0x1B/*AV_NTSC*/,
                              0x1B/*AV_PAL*/,
                              0x27/*480i*/,
                              0x18/*720p*/,
                              0x18/*1080i*/,
                              0x18/*H_480i*/,
                              0x18/*H_576i*/,
                              0x16/*H_720p*/,
                              0x16/*H_1080i*/,
                              0x18/*DT_MPEG2_480is*/,
                              0x18/*DT_MPEG2_576i*/,
                              0x18/*DT_MPEG2_1080i*/,
                              0x18/*DT_H264_480is*/,
                              0x18/*DT_H264_576p*/,
                              0x18/*DT_H264_720p*/,
                              0x18/*DT_H264_1080i*/,
                              0x18/*DT_H264_1080p*/,
                              0x2A/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_41_L), 0x3F, 0x1F/*RF_NTSC*/,
                              0x1F/*AV_NTSC*/,
                              0x1F/*AV_PAL*/,
                              0x28/*480i*/,
                              0x1C/*720p*/,
                              0x1C/*1080i*/,
                              0x1C/*H_480i*/,
                              0x1C/*H_576i*/,
                              0x1C/*H_720p*/,
                              0x1C/*H_1080i*/,
                              0x1C/*DT_MPEG2_480is*/,
                              0x1C/*DT_MPEG2_576i*/,
                              0x1C/*DT_MPEG2_1080i*/,
                              0x1C/*DT_H264_480is*/,
                              0x1C/*DT_H264_576p*/,
                              0x1C/*DT_H264_720p*/,
                              0x1C/*DT_H264_1080i*/,
                              0x1C/*DT_H264_1080p*/,
                              0x28/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_41_H), 0x3F, 0x22/*RF_NTSC*/,
                              0x22/*AV_NTSC*/,
                              0x22/*AV_PAL*/,
                              0x27/*480i*/,
                              0x22/*720p*/,
                              0x22/*1080i*/,
                              0x22/*H_480i*/,
                              0x22/*H_576i*/,
                              0x22/*H_720p*/,
                              0x22/*H_1080i*/,
                              0x22/*DT_MPEG2_480is*/,
                              0x22/*DT_MPEG2_576i*/,
                              0x22/*DT_MPEG2_1080i*/,
                              0x22/*DT_H264_480is*/,
                              0x22/*DT_H264_576p*/,
                              0x22/*DT_H264_720p*/,
                              0x22/*DT_H264_1080i*/,
                              0x22/*DT_H264_1080p*/,
                              0x27/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_42_L), 0x3F, 0x24/*RF_NTSC*/,
                              0x24/*AV_NTSC*/,
                              0x24/*AV_PAL*/,
                              0x27/*480i*/,
                              0x24/*720p*/,
                              0x24/*1080i*/,
                              0x24/*H_480i*/,
                              0x24/*H_576i*/,
                              0x24/*H_720p*/,
                              0x24/*H_1080i*/,
                              0x24/*DT_MPEG2_480is*/,
                              0x24/*DT_MPEG2_576i*/,
                              0x24/*DT_MPEG2_1080i*/,
                              0x24/*DT_H264_480is*/,
                              0x24/*DT_H264_576p*/,
                              0x24/*DT_H264_720p*/,
                              0x24/*DT_H264_1080i*/,
                              0x24/*DT_H264_1080p*/,
                              0x26/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_42_H), 0x3F, 0x26/*RF_NTSC*/,
                              0x26/*AV_NTSC*/,
                              0x26/*AV_PAL*/,
                              0x26/*480i*/,
                              0x26/*720p*/,
                              0x26/*1080i*/,
                              0x26/*H_480i*/,
                              0x26/*H_576i*/,
                              0x26/*H_720p*/,
                              0x26/*H_1080i*/,
                              0x26/*DT_MPEG2_480is*/,
                              0x26/*DT_MPEG2_576i*/,
                              0x26/*DT_MPEG2_1080i*/,
                              0x26/*DT_H264_480is*/,
                              0x26/*DT_H264_576p*/,
                              0x26/*DT_H264_720p*/,
                              0x26/*DT_H264_1080i*/,
                              0x26/*DT_H264_1080p*/,
                              0x25/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_43_L), 0x3F, 0x25/*RF_NTSC*/,
                              0x25/*AV_NTSC*/,
                              0x25/*AV_PAL*/,
                              0x28/*480i*/,
                              0x28/*720p*/,
                              0x28/*1080i*/,
                              0x28/*H_480i*/,
                              0x28/*H_576i*/,
                              0x28/*H_720p*/,
                              0x28/*H_1080i*/,
                              0x28/*DT_MPEG2_480is*/,
                              0x28/*DT_MPEG2_576i*/,
                              0x28/*DT_MPEG2_1080i*/,
                              0x28/*DT_H264_480is*/,
                              0x28/*DT_H264_576p*/,
                              0x28/*DT_H264_720p*/,
                              0x28/*DT_H264_1080i*/,
                              0x28/*DT_H264_1080p*/,
                              0x24/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_43_H), 0x3F, 0x24/*RF_NTSC*/,
                              0x24/*AV_NTSC*/,
                              0x24/*AV_PAL*/,
                              0x2A/*480i*/,
                              0x2A/*720p*/,
                              0x2A/*1080i*/,
                              0x2A/*H_480i*/,
                              0x2A/*H_576i*/,
                              0x2A/*H_720p*/,
                              0x2A/*H_1080i*/,
                              0x2A/*DT_MPEG2_480is*/,
                              0x2A/*DT_MPEG2_576i*/,
                              0x2A/*DT_MPEG2_1080i*/,
                              0x2A/*DT_H264_480is*/,
                              0x2A/*DT_H264_576p*/,
                              0x2A/*DT_H264_720p*/,
                              0x2A/*DT_H264_1080i*/,
                              0x2A/*DT_H264_1080p*/,
                              0x23/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_44_L), 0x3F, 0x23/*RF_NTSC*/,
                              0x23/*AV_NTSC*/,
                              0x23/*AV_PAL*/,
                              0x28/*480i*/,
                              0x28/*720p*/,
                              0x28/*1080i*/,
                              0x28/*H_480i*/,
                              0x28/*H_576i*/,
                              0x28/*H_720p*/,
                              0x28/*H_1080i*/,
                              0x28/*DT_MPEG2_480is*/,
                              0x28/*DT_MPEG2_576i*/,
                              0x28/*DT_MPEG2_1080i*/,
                              0x28/*DT_H264_480is*/,
                              0x28/*DT_H264_576p*/,
                              0x28/*DT_H264_720p*/,
                              0x28/*DT_H264_1080i*/,
                              0x28/*DT_H264_1080p*/,
                              0x22/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_44_H), 0x3F, 0x22/*RF_NTSC*/,
                              0x22/*AV_NTSC*/,
                              0x22/*AV_PAL*/,
                              0x26/*480i*/,
                              0x26/*720p*/,
                              0x26/*1080i*/,
                              0x26/*H_480i*/,
                              0x26/*H_576i*/,
                              0x26/*H_720p*/,
                              0x26/*H_1080i*/,
                              0x26/*DT_MPEG2_480is*/,
                              0x26/*DT_MPEG2_576i*/,
                              0x26/*DT_MPEG2_1080i*/,
                              0x26/*DT_H264_480is*/,
                              0x26/*DT_H264_576p*/,
                              0x26/*DT_H264_720p*/,
                              0x26/*DT_H264_1080i*/,
                              0x26/*DT_H264_1080p*/,
                              0x21/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_45_L), 0x3F, 0x21/*RF_NTSC*/,
                              0x21/*AV_NTSC*/,
                              0x21/*AV_PAL*/,
                              0x24/*480i*/,
                              0x24/*720p*/,
                              0x24/*1080i*/,
                              0x24/*H_480i*/,
                              0x24/*H_576i*/,
                              0x24/*H_720p*/,
                              0x24/*H_1080i*/,
                              0x24/*DT_MPEG2_480is*/,
                              0x24/*DT_MPEG2_576i*/,
                              0x24/*DT_MPEG2_1080i*/,
                              0x24/*DT_H264_480is*/,
                              0x24/*DT_H264_576p*/,
                              0x24/*DT_H264_720p*/,
                              0x24/*DT_H264_1080i*/,
                              0x24/*DT_H264_1080p*/,
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_45_H), 0x3F, 0x20/*RF_NTSC*/,
                              0x20/*AV_NTSC*/,
                              0x20/*AV_PAL*/,
                              0x22/*480i*/,
                              0x22/*720p*/,
                              0x22/*1080i*/,
                              0x22/*H_480i*/,
                              0x22/*H_576i*/,
                              0x22/*H_720p*/,
                              0x22/*H_1080i*/,
                              0x22/*DT_MPEG2_480is*/,
                              0x22/*DT_MPEG2_576i*/,
                              0x22/*DT_MPEG2_1080i*/,
                              0x22/*DT_H264_480is*/,
                              0x22/*DT_H264_576p*/,
                              0x22/*DT_H264_720p*/,
                              0x22/*DT_H264_1080i*/,
                              0x22/*DT_H264_1080p*/,
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_48_L), 0x3F, 0x18/*RF_NTSC*/,
                              0x18/*AV_NTSC*/,
                              0x18/*AV_PAL*/,
                              0x26/*480i*/,
                              0x1C/*720p*/,
                              0x1C/*1080i*/,
                              0x1C/*H_480i*/,
                              0x1C/*H_576i*/,
                              0x1C/*H_720p*/,
                              0x1C/*H_1080i*/,
                              0x1C/*DT_MPEG2_480is*/,
                              0x1C/*DT_MPEG2_576i*/,
                              0x1C/*DT_MPEG2_1080i*/,
                              0x1C/*DT_H264_480is*/,
                              0x1C/*DT_H264_576p*/,
                              0x1C/*DT_H264_720p*/,
                              0x1C/*DT_H264_1080i*/,
                              0x1C/*DT_H264_1080p*/,
                              0x2B/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_48_H), 0x3F, 0x1B/*RF_NTSC*/,
                              0x1B/*AV_NTSC*/,
                              0x1B/*AV_PAL*/,
                              0x27/*480i*/,
                              0x1E/*720p*/,
                              0x1E/*1080i*/,
                              0x1E/*H_480i*/,
                              0x1E/*H_576i*/,
                              0x1E/*H_720p*/,
                              0x1E/*H_1080i*/,
                              0x1E/*DT_MPEG2_480is*/,
                              0x1E/*DT_MPEG2_576i*/,
                              0x1E/*DT_MPEG2_1080i*/,
                              0x1E/*DT_H264_480is*/,
                              0x1E/*DT_H264_576p*/,
                              0x1E/*DT_H264_720p*/,
                              0x1E/*DT_H264_1080i*/,
                              0x1E/*DT_H264_1080p*/,
                              0x2A/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_49_L), 0x3F, 0x1F/*RF_NTSC*/,
                              0x1F/*AV_NTSC*/,
                              0x1F/*AV_PAL*/,
                              0x28/*480i*/,
                              0x20/*720p*/,
                              0x20/*1080i*/,
                              0x20/*H_480i*/,
                              0x20/*H_576i*/,
                              0x20/*H_720p*/,
                              0x20/*H_1080i*/,
                              0x20/*DT_MPEG2_480is*/,
                              0x20/*DT_MPEG2_576i*/,
                              0x20/*DT_MPEG2_1080i*/,
                              0x20/*DT_H264_480is*/,
                              0x20/*DT_H264_576p*/,
                              0x20/*DT_H264_720p*/,
                              0x20/*DT_H264_1080i*/,
                              0x20/*DT_H264_1080p*/,
                              0x28/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_49_H), 0x3F, 0x22/*RF_NTSC*/,
                              0x22/*AV_NTSC*/,
                              0x22/*AV_PAL*/,
                              0x27/*480i*/,
                              0x22/*720p*/,
                              0x22/*1080i*/,
                              0x22/*H_480i*/,
                              0x22/*H_576i*/,
                              0x22/*H_720p*/,
                              0x22/*H_1080i*/,
                              0x22/*DT_MPEG2_480is*/,
                              0x22/*DT_MPEG2_576i*/,
                              0x22/*DT_MPEG2_1080i*/,
                              0x22/*DT_H264_480is*/,
                              0x22/*DT_H264_576p*/,
                              0x22/*DT_H264_720p*/,
                              0x22/*DT_H264_1080i*/,
                              0x22/*DT_H264_1080p*/,
                              0x27/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4A_L), 0x3F, 0x24/*RF_NTSC*/,
                              0x24/*AV_NTSC*/,
                              0x24/*AV_PAL*/,
                              0x27/*480i*/,
                              0x24/*720p*/,
                              0x24/*1080i*/,
                              0x24/*H_480i*/,
                              0x24/*H_576i*/,
                              0x24/*H_720p*/,
                              0x24/*H_1080i*/,
                              0x24/*DT_MPEG2_480is*/,
                              0x24/*DT_MPEG2_576i*/,
                              0x24/*DT_MPEG2_1080i*/,
                              0x24/*DT_H264_480is*/,
                              0x24/*DT_H264_576p*/,
                              0x24/*DT_H264_720p*/,
                              0x24/*DT_H264_1080i*/,
                              0x24/*DT_H264_1080p*/,
                              0x26/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4A_H), 0x3F, 0x26/*RF_NTSC*/,
                              0x26/*AV_NTSC*/,
                              0x26/*AV_PAL*/,
                              0x26/*480i*/,
                              0x26/*720p*/,
                              0x26/*1080i*/,
                              0x26/*H_480i*/,
                              0x26/*H_576i*/,
                              0x26/*H_720p*/,
                              0x26/*H_1080i*/,
                              0x26/*DT_MPEG2_480is*/,
                              0x26/*DT_MPEG2_576i*/,
                              0x26/*DT_MPEG2_1080i*/,
                              0x26/*DT_H264_480is*/,
                              0x26/*DT_H264_576p*/,
                              0x26/*DT_H264_720p*/,
                              0x26/*DT_H264_1080i*/,
                              0x26/*DT_H264_1080p*/,
                              0x25/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4B_L), 0x3F, 0x25/*RF_NTSC*/,
                              0x25/*AV_NTSC*/,
                              0x25/*AV_PAL*/,
                              0x28/*480i*/,
                              0x28/*720p*/,
                              0x28/*1080i*/,
                              0x28/*H_480i*/,
                              0x28/*H_576i*/,
                              0x28/*H_720p*/,
                              0x28/*H_1080i*/,
                              0x28/*DT_MPEG2_480is*/,
                              0x28/*DT_MPEG2_576i*/,
                              0x28/*DT_MPEG2_1080i*/,
                              0x28/*DT_H264_480is*/,
                              0x28/*DT_H264_576p*/,
                              0x28/*DT_H264_720p*/,
                              0x28/*DT_H264_1080i*/,
                              0x28/*DT_H264_1080p*/,
                              0x24/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4B_H), 0x3F, 0x24/*RF_NTSC*/,
                              0x24/*AV_NTSC*/,
                              0x24/*AV_PAL*/,
                              0x2A/*480i*/,
                              0x2A/*720p*/,
                              0x2A/*1080i*/,
                              0x2A/*H_480i*/,
                              0x2A/*H_576i*/,
                              0x2A/*H_720p*/,
                              0x2A/*H_1080i*/,
                              0x2A/*DT_MPEG2_480is*/,
                              0x2A/*DT_MPEG2_576i*/,
                              0x2A/*DT_MPEG2_1080i*/,
                              0x2A/*DT_H264_480is*/,
                              0x2A/*DT_H264_576p*/,
                              0x2A/*DT_H264_720p*/,
                              0x2A/*DT_H264_1080i*/,
                              0x2A/*DT_H264_1080p*/,
                              0x23/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4C_L), 0x3F, 0x23/*RF_NTSC*/,
                              0x23/*AV_NTSC*/,
                              0x23/*AV_PAL*/,
                              0x28/*480i*/,
                              0x28/*720p*/,
                              0x28/*1080i*/,
                              0x28/*H_480i*/,
                              0x28/*H_576i*/,
                              0x28/*H_720p*/,
                              0x28/*H_1080i*/,
                              0x28/*DT_MPEG2_480is*/,
                              0x28/*DT_MPEG2_576i*/,
                              0x28/*DT_MPEG2_1080i*/,
                              0x28/*DT_H264_480is*/,
                              0x28/*DT_H264_576p*/,
                              0x28/*DT_H264_720p*/,
                              0x28/*DT_H264_1080i*/,
                              0x28/*DT_H264_1080p*/,
                              0x22/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4C_H), 0x3F, 0x22/*RF_NTSC*/,
                              0x22/*AV_NTSC*/,
                              0x22/*AV_PAL*/,
                              0x26/*480i*/,
                              0x26/*720p*/,
                              0x26/*1080i*/,
                              0x26/*H_480i*/,
                              0x26/*H_576i*/,
                              0x26/*H_720p*/,
                              0x26/*H_1080i*/,
                              0x26/*DT_MPEG2_480is*/,
                              0x26/*DT_MPEG2_576i*/,
                              0x26/*DT_MPEG2_1080i*/,
                              0x26/*DT_H264_480is*/,
                              0x26/*DT_H264_576p*/,
                              0x26/*DT_H264_720p*/,
                              0x26/*DT_H264_1080i*/,
                              0x26/*DT_H264_1080p*/,
                              0x21/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4D_L), 0x3F, 0x21/*RF_NTSC*/,
                              0x21/*AV_NTSC*/,
                              0x21/*AV_PAL*/,
                              0x24/*480i*/,
                              0x24/*720p*/,
                              0x24/*1080i*/,
                              0x24/*H_480i*/,
                              0x24/*H_576i*/,
                              0x24/*H_720p*/,
                              0x24/*H_1080i*/,
                              0x24/*DT_MPEG2_480is*/,
                              0x24/*DT_MPEG2_576i*/,
                              0x24/*DT_MPEG2_1080i*/,
                              0x24/*DT_H264_480is*/,
                              0x24/*DT_H264_576p*/,
                              0x24/*DT_H264_720p*/,
                              0x24/*DT_H264_1080i*/,
                              0x24/*DT_H264_1080p*/,
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4D_H), 0x3F, 0x20/*RF_NTSC*/,
                              0x20/*AV_NTSC*/,
                              0x20/*AV_PAL*/,
                              0x22/*480i*/,
                              0x22/*720p*/,
                              0x22/*1080i*/,
                              0x22/*H_480i*/,
                              0x22/*H_576i*/,
                              0x22/*H_720p*/,
                              0x22/*H_1080i*/,
                              0x22/*DT_MPEG2_480is*/,
                              0x22/*DT_MPEG2_576i*/,
                              0x22/*DT_MPEG2_1080i*/,
                              0x22/*DT_H264_480is*/,
                              0x22/*DT_H264_576p*/,
                              0x22/*DT_H264_720p*/,
                              0x22/*DT_H264_1080i*/,
                              0x22/*DT_H264_1080p*/,
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_Pcoring_ad_C
//****************************************************
code U8 MST_VIP_Peaking_Pcoring_ad_C_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_Pcoring_ad_C_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_gain_by_Cedge
//****************************************************
code U8 MST_VIP_Peaking_gain_by_Cedge_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_gain_by_Cedge_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ICC_Target
//****************************************************
code U8 MST_VIP_ICC_Target_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ICC_Target_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ICC_Target_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_IBC_weight
//****************************************************
code U8 MST_VIP_IBC_weight_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_IBC_weight_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_IBC_weight_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK1C_21_H), 0xFF, 0x66/*$W6*/,
                              0x88/*$W8*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ACK
//****************************************************
code U8 MST_VIP_ACK_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK18_38_L), 0xD0, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_39_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK18_39_H), 0x3F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK18_3A_L), 0xFF, 0xC0 },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_3A_H), 0x1F, 0x01 },
 { PQ_MAP_REG(REG_SC_BK18_3B_L), 0xFF, 0x3C },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_3B_H), 0x6F, 0x02 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ACK_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ACK_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Ygain_ColorAdaptive
//****************************************************
code U8 MST_VIP_Ygain_ColorAdaptive_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK1B_10_H), 0x02, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_15_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_15_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_16_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_16_H), 0x07, 0x04 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Ygain_ColorAdaptive_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Ygain_ColorAdaptive_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Cgain_ColorAdaptive
//****************************************************
code U8 MST_VIP_Cgain_ColorAdaptive_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK1B_10_H), 0x84, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK1B_11_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_11_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK1B_12_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Cgain_ColorAdaptive_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Cgain_ColorAdaptive_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Ygain_NonLinear
//****************************************************
code U8 MST_VIP_Ygain_NonLinear_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK56_50_L), 0x05, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK56_51_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK56_51_H), 0xC7, 0x04 },//Same mark
 { PQ_MAP_REG(REG_SC_BK56_52_L), 0xFF, 0x6F },
 { PQ_MAP_REG(REG_SC_BK56_52_H), 0x03, 0x03 },
 { PQ_MAP_REG(REG_SC_BK56_53_L), 0xFF, 0x3C },
 { PQ_MAP_REG(REG_SC_BK56_53_H), 0x03, 0x03 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Ygain_NonLinear_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Ygain_NonLinear_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Cgain_NonLinear
//****************************************************
code U8 MST_VIP_Cgain_NonLinear_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK56_50_H), 0x01, 0x00 },
 { PQ_MAP_REG(REG_SC_BK56_54_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK56_54_H), 0x0F, 0x04 },
 { PQ_MAP_REG(REG_SC_BK56_55_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK56_55_H), 0x03, 0x02 },
 { PQ_MAP_REG(REG_SC_BK56_56_L), 0xFF, 0x04 },
 { PQ_MAP_REG(REG_SC_BK56_56_H), 0x01, 0x01 },
 { PQ_MAP_REG(REG_SC_BK56_57_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK56_57_H), 0x0F, 0x08 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Cgain_NonLinear_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Cgain_NonLinear_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Main_Ex[]=
{
{*MST_AFEC_no_comm_COM_Main_Ex, *MST_AFEC_no_comm_Main_Ex, PQ_IP_AFEC_no_comm_NUMS_Main_Ex, PQ_TABTYPE_GENERAL},
{*MST_HISDNR_COM_Main_Ex, *MST_HISDNR_Main_Ex, PQ_IP_HISDNR_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_SPF_DBK_COM_Main_Ex, *MST_SPF_DBK_Main_Ex, PQ_IP_SPF_DBK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Main_Ex, *MST_DMS_Main_Ex, PQ_IP_DMS_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_H_COM_Main_Ex, *MST_DMS_H_Main_Ex, PQ_IP_DMS_H_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_adptive_COM_Main_Ex, *MST_VIP_Peaking_adptive_Main_Ex, PQ_IP_VIP_Peaking_adptive_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_Pcoring_ad_C_COM_Main_Ex, *MST_VIP_Peaking_Pcoring_ad_C_Main_Ex, PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_gain_by_Cedge_COM_Main_Ex, *MST_VIP_Peaking_gain_by_Cedge_Main_Ex, PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ICC_Target_COM_Main_Ex, *MST_VIP_ICC_Target_Main_Ex, PQ_IP_VIP_ICC_Target_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_IBC_weight_COM_Main_Ex, *MST_VIP_IBC_weight_Main_Ex, PQ_IP_VIP_IBC_weight_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ACK_COM_Main_Ex, *MST_VIP_ACK_Main_Ex, PQ_IP_VIP_ACK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Ygain_ColorAdaptive_COM_Main_Ex, *MST_VIP_Ygain_ColorAdaptive_Main_Ex, PQ_IP_VIP_Ygain_ColorAdaptive_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Cgain_ColorAdaptive_COM_Main_Ex, *MST_VIP_Cgain_ColorAdaptive_Main_Ex, PQ_IP_VIP_Cgain_ColorAdaptive_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Ygain_NonLinear_COM_Main_Ex, *MST_VIP_Ygain_NonLinear_Main_Ex, PQ_IP_VIP_Ygain_NonLinear_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Cgain_NonLinear_COM_Main_Ex, *MST_VIP_Cgain_NonLinear_Main_Ex, PQ_IP_VIP_Cgain_NonLinear_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
};

#endif
