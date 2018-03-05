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
// Maserati
// 2015/10/23 20:33
// 2008/7/1 10:31
//****************************************************

#ifndef _MASERATI_SUB_EX_C_
#define _MASERATI_SUB_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Sub_Ex[PQ_IP_NUM_Sub_Ex]=
{
    /* AFEC_no_comm */ 0, /* HISDNR */ 0, /* SPF_DBK */ 0, /* DMS */ 0, 
    /* DMS_H */ 0, /* VIP_Peaking_adptive */ 0, /* VIP_Peaking_Pcoring_ad_C */ 0, /* VIP_Peaking_gain_by_Cedge */ 0, 
    /* VIP_ICC_Target */ 0, /* VIP_IBC_weight */ 0, /* VIP_ACK */ 0, 
};

#endif
//****************************************************
// AFEC_no_comm
//****************************************************
code U8 MST_AFEC_no_comm_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_AFEC_no_comm_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_AFEC_no_comm_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_BK35_2A_H), 0x04, 0x04/*ON*/, },
 { PQ_MAP_REG(REG_BK35_2A_H), 0x04, 0x00/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HISDNR
//****************************************************
code U8 MST_HISDNR_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK8E_01_L), 0xC0, 0x40 },//Same mark
 { PQ_MAP_REG(REG_SC_BK8E_01_H), 0x01, 0x01 },
 { PQ_MAP_REG(REG_SC_BK8E_07_L), 0xFF, 0x80 },
 { PQ_MAP_REG(REG_SC_BK8E_08_L), 0xFF, 0x60 },
 { PQ_MAP_REG(REG_SC_BK8E_09_L), 0xFF, 0x30 },
 { PQ_MAP_REG(REG_SC_BK8E_49_H), 0xFF, 0x20 },
 { PQ_MAP_REG(REG_SC_BK8E_07_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK8E_08_H), 0xFF, 0x03 },
 { PQ_MAP_REG(REG_SC_BK8E_09_H), 0xFF, 0x08 },
 { PQ_MAP_REG(REG_SC_BK8E_4B_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK8E_06_L), 0xFF, 0xA2 },//Same mark
 { PQ_MAP_REG(REG_SC_BK8E_06_H), 0x0F, 0x0C },
 { PQ_MAP_REG(REG_SC_BK8E_49_L), 0x0F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK8E_03_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK8E_03_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK8E_04_L), 0xFF, 0x06 },
 { PQ_MAP_REG(REG_SC_BK8E_04_H), 0xFF, 0x08 },
 { PQ_MAP_REG(REG_SC_BK8E_05_L), 0xFF, 0x08 },
 { PQ_MAP_REG(REG_SC_BK8E_05_H), 0xFF, 0x09 },
 { PQ_MAP_REG(REG_SC_BK8E_4A_L), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_SC_BK8E_4A_H), 0xFF, 0x12 },
 { PQ_MAP_REG(REG_SC_BK8E_2A_L), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_SC_BK8E_2B_H), 0xFF, 0x3F },
 { PQ_MAP_REG(REG_SC_BK8E_2C_L), 0xFF, 0x3F },
 { PQ_MAP_REG(REG_SC_BK8E_2D_L), 0xFF, 0x04 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HISDNR_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HISDNR_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// SPF_DBK
//****************************************************
code U8 MST_SPF_DBK_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK8C_48_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK8C_48_H), 0xFF, 0xDC },
 { PQ_MAP_REG(REG_SC_BK8C_49_L), 0xFF, 0xBA },
 { PQ_MAP_REG(REG_SC_BK8C_49_H), 0xFF, 0x98 },
 { PQ_MAP_REG(REG_SC_BK8C_4A_L), 0xFF, 0x76 },
 { PQ_MAP_REG(REG_SC_BK8C_4A_H), 0xFF, 0x54 },
 { PQ_MAP_REG(REG_SC_BK8C_4B_L), 0xFF, 0x32 },
 { PQ_MAP_REG(REG_SC_BK8C_4B_H), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_SC_BK8C_4C_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK8C_4C_H), 0xFF, 0xDC },
 { PQ_MAP_REG(REG_SC_BK8C_4D_L), 0xFF, 0xBA },
 { PQ_MAP_REG(REG_SC_BK8C_4D_H), 0xFF, 0x98 },
 { PQ_MAP_REG(REG_SC_BK8C_4E_L), 0xFF, 0x76 },
 { PQ_MAP_REG(REG_SC_BK8C_4E_H), 0xFF, 0x54 },
 { PQ_MAP_REG(REG_SC_BK8C_4F_L), 0xFF, 0x32 },
 { PQ_MAP_REG(REG_SC_BK8C_4F_H), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SPF_DBK_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK8C_71_L), 0x01, 0x00/*OFF*/, 
                              0x01/*LON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS
//****************************************************
code U8 MST_DMS_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BKA6_10_L), 0x02, 0x02 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_H
//****************************************************
code U8 MST_DMS_H_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BKA6_19_L), 0x0B, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_H_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_H_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BKA6_10_L), 0x10, 0x00/*J0*/, 
                              0x10/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_adptive
//****************************************************
code U8 MST_VIP_Peaking_adptive_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK99_32_L), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK99_32_H), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK99_46_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_46_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_47_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_47_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_4E_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_4E_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_4F_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK99_4F_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_adptive_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_adptive_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK99_31_L), 0x9F, 0x03/*$RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_40_L), 0x3F, 0x18/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_40_H), 0x3F, 0x1B/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_41_L), 0x3F, 0x1F/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_41_H), 0x3F, 0x22/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_42_L), 0x3F, 0x24/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_42_H), 0x3F, 0x26/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_43_L), 0x3F, 0x25/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_43_H), 0x3F, 0x24/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_44_L), 0x3F, 0x23/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_44_H), 0x3F, 0x22/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_45_L), 0x3F, 0x21/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_45_H), 0x3F, 0x20/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_48_L), 0x3F, 0x18/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_48_H), 0x3F, 0x1B/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_49_L), 0x3F, 0x1F/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_49_H), 0x3F, 0x22/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4A_L), 0x3F, 0x24/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4A_H), 0x3F, 0x26/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4B_L), 0x3F, 0x25/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4B_H), 0x3F, 0x24/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4C_L), 0x3F, 0x23/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4C_H), 0x3F, 0x22/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4D_L), 0x3F, 0x21/*RF_NTSC*/, 
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
 { PQ_MAP_REG(REG_SC_BK99_4D_H), 0x3F, 0x20/*RF_NTSC*/, 
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
code U8 MST_VIP_Peaking_Pcoring_ad_C_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK99_37_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_Pcoring_ad_C_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_gain_by_Cedge
//****************************************************
code U8 MST_VIP_Peaking_gain_by_Cedge_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_gain_by_Cedge_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ICC_Target
//****************************************************
code U8 MST_VIP_ICC_Target_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ICC_Target_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ICC_Target_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_IBC_weight
//****************************************************
code U8 MST_VIP_IBC_weight_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_IBC_weight_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_IBC_weight_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK9C_21_H), 0xFF, 0x66/*$W6*/, 
                              0x88/*$W8*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ACK
//****************************************************
code U8 MST_VIP_ACK_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK98_38_L), 0xD0, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK98_39_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK98_39_H), 0x3F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK98_3A_L), 0xFF, 0xC0 },//Same mark
 { PQ_MAP_REG(REG_SC_BK98_3A_H), 0x1F, 0x01 },
 { PQ_MAP_REG(REG_SC_BK98_3B_L), 0xFF, 0x3C },//Same mark
 { PQ_MAP_REG(REG_SC_BK98_3B_H), 0x6F, 0x02 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ACK_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ACK_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub_Ex[]=
{
{*MST_AFEC_no_comm_COM_Sub_Ex, *MST_AFEC_no_comm_Sub_Ex, PQ_IP_AFEC_no_comm_NUMS_Sub_Ex, PQ_TABTYPE_GENERAL},
{*MST_HISDNR_COM_Sub_Ex, *MST_HISDNR_Sub_Ex, PQ_IP_HISDNR_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_SPF_DBK_COM_Sub_Ex, *MST_SPF_DBK_Sub_Ex, PQ_IP_SPF_DBK_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Sub_Ex, *MST_DMS_Sub_Ex, PQ_IP_DMS_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_H_COM_Sub_Ex, *MST_DMS_H_Sub_Ex, PQ_IP_DMS_H_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_adptive_COM_Sub_Ex, *MST_VIP_Peaking_adptive_Sub_Ex, PQ_IP_VIP_Peaking_adptive_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_Pcoring_ad_C_COM_Sub_Ex, *MST_VIP_Peaking_Pcoring_ad_C_Sub_Ex, PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_gain_by_Cedge_COM_Sub_Ex, *MST_VIP_Peaking_gain_by_Cedge_Sub_Ex, PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ICC_Target_COM_Sub_Ex, *MST_VIP_ICC_Target_Sub_Ex, PQ_IP_VIP_ICC_Target_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_IBC_weight_COM_Sub_Ex, *MST_VIP_IBC_weight_Sub_Ex, PQ_IP_VIP_IBC_weight_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ACK_COM_Sub_Ex, *MST_VIP_ACK_Sub_Ex, PQ_IP_VIP_ACK_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
};

#endif
