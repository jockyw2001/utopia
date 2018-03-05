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
// 2015/10/21 16:13
// 2008/7/1 10:31
//****************************************************

#ifndef _MASERATI_MAIN_EX_C_
#define _MASERATI_MAIN_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Main_Ex[PQ_IP_NUM_Main_Ex]=
{
    /* AFEC_no_comm */ 0, /* SPF_DBK */ 0, /* DMS */ 0, /* DMS_H */ 0, 
    /* DMS_V */ 0, /* DMS_NMR */ 0, /* VIP_HNMR_Y_lowY */ 0, /* VIP_VNMR_Y_lowY */ 0, 
    /* VIP_LDE_setting */ 0, /* VIP_LDE_C_win1 */ 0, /* VIP_LDE_C_win2 */ 0, /* VIP_LDE_C_win3 */ 0, 
    /* VIP_LDE_C_win4 */ 0, /* VIP_LDE_suppress */ 0, /* VIP_Peaking_alpha_MIN */ 0, /* VIP_Peaking_gain_by_Cedge */ 0, 
    /* VIP_Peaking_V_dering_LPF */ 0, /* VIP_Peaking_V_dering_Adpt */ 0, /* VIP_Peaking_V_dering_Table */ 0, /* VIP_Post_SNR */ 0, 
    /* VIP_ICC_Target */ 0, /* VIP_IBC_weight */ 0, /* VIP_ACK */ 0, /* OP1_LBOX */ 0, 
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
 { PQ_MAP_REG(REG_SC_BK26_19_L), 0x0B, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_H_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_H_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x10, 0x00/*J0*/, 
                              0x10/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_V
//****************************************************
code U8 MST_DMS_V_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_19_L), 0x30, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_V_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_V_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x20, 0x00/*J0*/, 
                              0x20/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_NMR
//****************************************************
code U8 MST_DMS_NMR_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_NMR_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NMR_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_HNMR_Y_lowY
//****************************************************
code U8 MST_VIP_HNMR_Y_lowY_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_HNMR_Y_lowY_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_HNMR_Y_lowY_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_VNMR_Y_lowY
//****************************************************
code U8 MST_VIP_VNMR_Y_lowY_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_VNMR_Y_lowY_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_VNMR_Y_lowY_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_setting
//****************************************************
code U8 MST_VIP_LDE_setting_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_10_L), 0xF0, 0x40 },
 { PQ_MAP_REG(REG_SC_BK38_11_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_11_H), 0x1F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_12_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_30_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_30_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_31_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_31_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_32_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_32_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_33_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_33_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_34_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_34_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_35_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_35_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_36_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_36_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_37_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_37_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_38_L), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_38_H), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_39_L), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_39_H), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_3A_L), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_3A_H), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_3B_L), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_3B_H), 0xFF, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_3C_L), 0x07, 0x01 },
 { PQ_MAP_REG(REG_SC_BK38_3C_H), 0x07, 0x02 },
 { PQ_MAP_REG(REG_SC_BK38_50_H), 0x80, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_setting_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_setting_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_C_win1
//****************************************************
code U8 MST_VIP_LDE_C_win1_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_50_L), 0x01, 0x01 },
 { PQ_MAP_REG(REG_SC_BK38_51_L), 0x03, 0x02 },
 { PQ_MAP_REG(REG_SC_BK38_52_L), 0xFF, 0x83 },
 { PQ_MAP_REG(REG_SC_BK38_53_L), 0xFF, 0x83 },
 { PQ_MAP_REG(REG_SC_BK38_52_H), 0xFF, 0x7E },
 { PQ_MAP_REG(REG_SC_BK38_53_H), 0xFF, 0x7E },
 { PQ_MAP_REG(REG_SC_BK38_5A_L), 0x0F, 0x01 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_C_win1_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_C_win1_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_C_win2
//****************************************************
code U8 MST_VIP_LDE_C_win2_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_50_L), 0x02, 0x02 },
 { PQ_MAP_REG(REG_SC_BK38_51_L), 0x0C, 0x0C },
 { PQ_MAP_REG(REG_SC_BK38_54_L), 0xFF, 0xFF },
 { PQ_MAP_REG(REG_SC_BK38_55_L), 0xFF, 0x64 },
 { PQ_MAP_REG(REG_SC_BK38_54_H), 0xFF, 0x8C },
 { PQ_MAP_REG(REG_SC_BK38_55_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_5A_L), 0xF0, 0x10 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_C_win2_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_C_win2_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_C_win3
//****************************************************
code U8 MST_VIP_LDE_C_win3_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_50_L), 0x04, 0x04 },
 { PQ_MAP_REG(REG_SC_BK38_51_L), 0x30, 0x20 },
 { PQ_MAP_REG(REG_SC_BK38_56_L), 0xFF, 0x6E },
 { PQ_MAP_REG(REG_SC_BK38_57_L), 0xFF, 0xA0 },
 { PQ_MAP_REG(REG_SC_BK38_56_H), 0xFF, 0x64 },
 { PQ_MAP_REG(REG_SC_BK38_57_H), 0xFF, 0x96 },
 { PQ_MAP_REG(REG_SC_BK38_5A_H), 0x0F, 0x01 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_C_win3_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_C_win3_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_C_win4
//****************************************************
code U8 MST_VIP_LDE_C_win4_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_50_L), 0x08, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_51_L), 0xC0, 0x80 },
 { PQ_MAP_REG(REG_SC_BK38_58_L), 0xFF, 0x78 },
 { PQ_MAP_REG(REG_SC_BK38_59_L), 0xFF, 0x78 },
 { PQ_MAP_REG(REG_SC_BK38_58_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_59_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_5A_H), 0xF0, 0xF0 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_C_win4_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_C_win4_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_LDE_suppress
//****************************************************
code U8 MST_VIP_LDE_suppress_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK38_60_L), 0x01, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_61_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_61_H), 0x03, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_62_L), 0xFF, 0x02 },//Same mark
 { PQ_MAP_REG(REG_SC_BK38_62_H), 0x0F, 0x04 },
 { PQ_MAP_REG(REG_SC_BK38_70_L), 0x1F, 0x06 },
 { PQ_MAP_REG(REG_SC_BK38_70_H), 0x1F, 0x06 },
 { PQ_MAP_REG(REG_SC_BK38_71_L), 0x1F, 0x07 },
 { PQ_MAP_REG(REG_SC_BK38_71_H), 0x1F, 0x08 },
 { PQ_MAP_REG(REG_SC_BK38_72_L), 0x1F, 0x09 },
 { PQ_MAP_REG(REG_SC_BK38_72_H), 0x1F, 0x0A },
 { PQ_MAP_REG(REG_SC_BK38_73_L), 0x1F, 0x0B },
 { PQ_MAP_REG(REG_SC_BK38_73_H), 0x1F, 0x0C },
 { PQ_MAP_REG(REG_SC_BK38_74_L), 0x1F, 0x0D },
 { PQ_MAP_REG(REG_SC_BK38_74_H), 0x1F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK38_75_L), 0x1F, 0x0F },
 { PQ_MAP_REG(REG_SC_BK38_75_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_76_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_76_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_77_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_77_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_78_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_78_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_79_L), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_79_H), 0x1F, 0x10 },
 { PQ_MAP_REG(REG_SC_BK38_7A_L), 0x1F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK38_7A_H), 0x1F, 0x0C },
 { PQ_MAP_REG(REG_SC_BK38_7B_L), 0x1F, 0x0A },
 { PQ_MAP_REG(REG_SC_BK38_7B_H), 0x1F, 0x08 },
 { PQ_MAP_REG(REG_SC_BK38_7C_L), 0x1F, 0x06 },
 { PQ_MAP_REG(REG_SC_BK38_7C_H), 0x1F, 0x04 },
 { PQ_MAP_REG(REG_SC_BK38_7D_L), 0x1F, 0x02 },
 { PQ_MAP_REG(REG_SC_BK38_7D_H), 0x1F, 0x01 },
 { PQ_MAP_REG(REG_SC_BK38_7E_L), 0x1F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_7E_H), 0x1F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_7F_L), 0x1F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK38_7F_H), 0x1F, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_LDE_suppress_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_LDE_suppress_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_alpha_MIN
//****************************************************
code U8 MST_VIP_Peaking_alpha_MIN_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_alpha_MIN_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_6C_L), 0x37, 0x00/*$OFF*/, 
                              0x31/*$m1*/, 
                              0x33/*$m2*/, },
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
// VIP_Peaking_V_dering_LPF
//****************************************************
code U8 MST_VIP_Peaking_V_dering_LPF_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_LPF_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_21_L), 0x30, 0x00/*$OFF*/, 
                              0x10/*$VDLPF1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Adpt
//****************************************************
code U8 MST_VIP_Peaking_V_dering_Adpt_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK2B_22_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_24_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_25_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_Adpt_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_12_L), 0x0F, 0x00/*OFF*/, 
                              0x58/*VDAdpt1*/, 
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_12_H), 0x0F, 0x00/*OFF*/, 
                              0x58/*VDAdpt1*/, 
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_20_L), 0x03, 0x00/*$OFF*/, 
                              0x03/*$VDAdpt1*/, 
                              0x03/*$VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_21_L), 0x01, 0x00/*OFF*/, 
                              0x17/*VDAdpt1*/, 
                              0x17/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_22_H), 0x07, 0x00/*OFF*/, 
                              0x05/*VDAdpt1*/, 
                              0x01/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_23_L), 0xFF, 0x00/*OFF*/, 
                              0x14/*VDAdpt1*/, 
                              0x14/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_23_H), 0x0F, 0x00/*OFF*/, 
                              0x08/*VDAdpt1*/, 
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_24_H), 0x3F, 0x00/*OFF*/, 
                              0x20/*VDAdpt1*/, 
                              0x20/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_25_H), 0x0F, 0x00/*OFF*/, 
                              0x08/*VDAdpt1*/, 
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Table
//****************************************************
code U8 MST_VIP_Peaking_V_dering_Table_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_Table_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_1B_L), 0x77, 0x00/*$OFF*/, 
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1B_H), 0x07, 0x00/*OFF*/, 
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1D_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1D_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1E_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1E_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1F_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1F_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_29_L), 0x77, 0x00/*$OFF*/, 
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_29_H), 0x07, 0x00/*OFF*/, 
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2D_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2D_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2E_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2E_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2F_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2F_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Post_SNR
//****************************************************
code U8 MST_VIP_Post_SNR_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Post_SNR_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Post_SNR_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_60_L), 0x02, 0x00/*OFF*/, 
                              0x02/*ON*/, },
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
// OP1_LBOX
//****************************************************
code U8 MST_OP1_LBOX_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK0B_04_H), 0xE0, 0x20 },//Same mark
 { PQ_MAP_REG(REG_SC_BK0B_05_H), 0xE0, 0x20 },
 { PQ_MAP_REG(REG_SC_BK0B_06_H), 0x70, 0x20 },
 { PQ_MAP_REG(REG_SC_BK0B_06_L), 0x1F, 0x03 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_OP1_LBOX_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_OP1_LBOX_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK0B_01_L), 0xFF, 0xD0/*SD*/, 
                              0x80/*FHD*/, 
                              0x00/*UHD*/, 
                              0x00/*HD*/, 
                              0x56/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_01_H), 0x0F, 0x02/*SD*/, 
                              0x07/*FHD*/, 
                              0x0F/*UHD*/, 
                              0x05/*HD*/, 
                              0x05/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_02_L), 0xFF, 0xE0/*SD*/, 
                              0x38/*FHD*/, 
                              0x70/*UHD*/, 
                              0xD0/*HD*/, 
                              0x00/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_02_H), 0x0F, 0x01/*SD*/, 
                              0x04/*FHD*/, 
                              0x08/*UHD*/, 
                              0x02/*HD*/, 
                              0x03/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_04_L), 0xFF, 0xCD/*SD*/, 
                              0x7D/*FHD*/, 
                              0xFD/*UHD*/, 
                              0xFD/*HD*/, 
                              0x53/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_04_H), 0x1F, 0x02/*$SD*/, 
                              0x07/*$FHD*/, 
                              0x0E/*$UHD*/, 
                              0x04/*$HD*/, 
                              0x05/*$QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_05_L), 0xFF, 0xA2/*$SD*/, 
                              0x57/*$FHD*/, 
                              0xDE/*$UHD*/, 
                              0xD4/*$HD*/, 
                              0x27/*$QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_07_L), 0xFF, 0xC0/*SD*/, 
                              0x70/*FHD*/, 
                              0xF0/*UHD*/, 
                              0xF0/*HD*/, 
                              0x46/*QHD*/, },
 { PQ_MAP_REG(REG_SC_BK0B_07_H), 0x0F, 0x02/*SD*/, 
                              0x07/*FHD*/, 
                              0x0E/*UHD*/, 
                              0x04/*HD*/, 
                              0x05/*QHD*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Main_Ex[]=
{
{*MST_AFEC_no_comm_COM_Main_Ex, *MST_AFEC_no_comm_Main_Ex, PQ_IP_AFEC_no_comm_NUMS_Main_Ex, PQ_TABTYPE_GENERAL},
{*MST_SPF_DBK_COM_Main_Ex, *MST_SPF_DBK_Main_Ex, PQ_IP_SPF_DBK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Main_Ex, *MST_DMS_Main_Ex, PQ_IP_DMS_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_H_COM_Main_Ex, *MST_DMS_H_Main_Ex, PQ_IP_DMS_H_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_V_COM_Main_Ex, *MST_DMS_V_Main_Ex, PQ_IP_DMS_V_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_NMR_COM_Main_Ex, *MST_DMS_NMR_Main_Ex, PQ_IP_DMS_NMR_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_HNMR_Y_lowY_COM_Main_Ex, *MST_VIP_HNMR_Y_lowY_Main_Ex, PQ_IP_VIP_HNMR_Y_lowY_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_VNMR_Y_lowY_COM_Main_Ex, *MST_VIP_VNMR_Y_lowY_Main_Ex, PQ_IP_VIP_VNMR_Y_lowY_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_setting_COM_Main_Ex, *MST_VIP_LDE_setting_Main_Ex, PQ_IP_VIP_LDE_setting_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_C_win1_COM_Main_Ex, *MST_VIP_LDE_C_win1_Main_Ex, PQ_IP_VIP_LDE_C_win1_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_C_win2_COM_Main_Ex, *MST_VIP_LDE_C_win2_Main_Ex, PQ_IP_VIP_LDE_C_win2_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_C_win3_COM_Main_Ex, *MST_VIP_LDE_C_win3_Main_Ex, PQ_IP_VIP_LDE_C_win3_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_C_win4_COM_Main_Ex, *MST_VIP_LDE_C_win4_Main_Ex, PQ_IP_VIP_LDE_C_win4_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_LDE_suppress_COM_Main_Ex, *MST_VIP_LDE_suppress_Main_Ex, PQ_IP_VIP_LDE_suppress_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_alpha_MIN_COM_Main_Ex, *MST_VIP_Peaking_alpha_MIN_Main_Ex, PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_gain_by_Cedge_COM_Main_Ex, *MST_VIP_Peaking_gain_by_Cedge_Main_Ex, PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_LPF_COM_Main_Ex, *MST_VIP_Peaking_V_dering_LPF_Main_Ex, PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Adpt_COM_Main_Ex, *MST_VIP_Peaking_V_dering_Adpt_Main_Ex, PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Table_COM_Main_Ex, *MST_VIP_Peaking_V_dering_Table_Main_Ex, PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Post_SNR_COM_Main_Ex, *MST_VIP_Post_SNR_Main_Ex, PQ_IP_VIP_Post_SNR_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ICC_Target_COM_Main_Ex, *MST_VIP_ICC_Target_Main_Ex, PQ_IP_VIP_ICC_Target_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_IBC_weight_COM_Main_Ex, *MST_VIP_IBC_weight_Main_Ex, PQ_IP_VIP_IBC_weight_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ACK_COM_Main_Ex, *MST_VIP_ACK_Main_Ex, PQ_IP_VIP_ACK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_OP1_LBOX_COM_Main_Ex, *MST_OP1_LBOX_Main_Ex, PQ_IP_OP1_LBOX_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
};

#endif
