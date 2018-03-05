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
// Maxim
// 6/20/2016  5:17:11 PM
// 42541.7202662
//****************************************************

#ifndef _MAXIM_UFSC_C_
#define _MAXIM_UFSC_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_UFSC[PQ_IP_NUM_UFSC]=
{
    /* INIT_FAC */ 0, /* VSP_Y */ 0, /* VSP_C */ 0, /* VSP_CoRing */ 0, 
    /* VSP_Dither */ 0, /* VSP_PreVBound */ 1, /* HSP_Y */ 0, /* HSP_C */ 0, 
    /* HSP_CoRing */ 0, /* HSP_DeRing */ 0, /* HSP_Dither */ 0, /* HnonLinear */ 1, 
    /* HSP_Y_COEFF */ 0, /* HSP_C_COEFF */ 0, /* VIP_4K */ 0, /* SC_End */ 1, 
};

#endif
//****************************************************
// INIT_FAC
//****************************************************
code U8 MST_INIT_FAC_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK4F_02_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_03_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_03_H), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_INIT_FAC_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_INIT_FAC_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_01_L), 0xFF, 0x00/*Type0*/, 
                              0x00/*Type1*/, 
                              0x88/*Type2*/, },
 { PQ_MAP_REG(REG_SC_BK4F_01_H), 0xFF, 0x00/*Type0*/, 
                              0x00/*Type1*/, 
                              0x88/*Type2*/, },
 { PQ_MAP_REG(REG_SC_BK4F_02_L), 0xFF, 0x00/*Type0*/, 
                              0x0C/*Type1*/, 
                              0x00/*Type2*/, },
 { PQ_MAP_REG(REG_SC_BK4F_04_L), 0xFF, 0x00/*Type0*/, 
                              0x0C/*Type1*/, 
                              0x00/*Type2*/, },
 { PQ_MAP_REG(REG_SC_BK4F_08_H), 0x02, 0x00/*Type0*/, 
                              0x02/*Type1*/, 
                              0x00/*Type2*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0A_H), 0x02, 0x00/*Type0*/, 
                              0x02/*Type1*/, 
                              0x00/*Type2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VSP_Y
//****************************************************
code U8 MST_VSP_Y_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VSP_Y_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VSP_Y_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0B_H), 0x41, 0x00/*$Bypass*/, 
                              0x01/*$Bilinear*/, 
                              0x40/*$Y_4Tap*/, 
                              0x40/*$Y_6Tap*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VSP_C
//****************************************************
code U8 MST_VSP_C_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VSP_C_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VSP_C_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0B_H), 0x1E, 0x00/*$Bypass*/, 
                              0x02/*$Bilinear*/, 
                              0x14/*$Y_4Tap*/, 
                              0x04/*$C_4Tap*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VSP_CoRing
//****************************************************
code U8 MST_VSP_CoRing_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VSP_CoRing_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VSP_CoRing_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0C_H), 0x0C, 0x00/*$OFF*/, 
                              0x08/*$Y_Coring_1*/, 
                              0x08/*$Y_Coring_2*/, 
                              0x08/*$Y_Coring_3*/, 
                              0x08/*$Y_Coring_4*/, 
                              0x08/*$Y_Coring_5*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0E_L), 0x1F, 0x00/*OFF*/, 
                              0x01/*Y_Coring_1*/, 
                              0x02/*Y_Coring_2*/, 
                              0x03/*Y_Coring_3*/, 
                              0x04/*Y_Coring_4*/, 
                              0x05/*Y_Coring_5*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0E_H), 0x1F, 0x00/*OFF*/, 
                              0x01/*Y_Coring_1*/, 
                              0x02/*Y_Coring_2*/, 
                              0x03/*Y_Coring_3*/, 
                              0x04/*Y_Coring_4*/, 
                              0x05/*Y_Coring_5*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VSP_Dither
//****************************************************
code U8 MST_VSP_Dither_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VSP_Dither_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VSP_Dither_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0C_L), 0x02, 0x00/*OFF*/, 
                              0x02/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VSP_PreVBound
//****************************************************
code U8 MST_VSP_PreVBound_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VSP_PreVBound_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VSP_PreVBound_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_Y
//****************************************************
code U8 MST_HSP_Y_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_Y_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_Y_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_13_L), 0x10, 0x00/*Bypass*/, 
                              0x00/*Bilinear*/, 
                              0x00/*Y_4Tap*/, 
                              0x10/*Y_6Tap*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0B_L), 0x41, 0x00/*$Bypass*/, 
                              0x01/*$Bilinear*/, 
                              0x40/*$Y_4Tap*/, 
                              0x40/*$Y_6Tap*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_C
//****************************************************
code U8 MST_HSP_C_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_C_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_C_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0B_L), 0x1E, 0x00/*$Bypass*/, 
                              0x02/*$Bilinear*/, 
                              0x04/*$C_4Tap*/, 
                              0x10/*$Y_4Tap*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_CoRing
//****************************************************
code U8 MST_HSP_CoRing_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_CoRing_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_CoRing_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0C_H), 0x03, 0x00/*$OFF*/, 
                              0x02/*$Y_Coring_1*/, 
                              0x02/*$Y_Coring_2*/, 
                              0x02/*$Y_Coring_3*/, 
                              0x02/*$Y_Coring_4*/, 
                              0x02/*$Y_Coring_5*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0D_L), 0x1F, 0x00/*OFF*/, 
                              0x01/*Y_Coring_1*/, 
                              0x02/*Y_Coring_2*/, 
                              0x03/*Y_Coring_3*/, 
                              0x04/*Y_Coring_4*/, 
                              0x05/*Y_Coring_5*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0D_H), 0x1F, 0x00/*OFF*/, 
                              0x01/*Y_Coring_1*/, 
                              0x02/*Y_Coring_2*/, 
                              0x03/*Y_Coring_3*/, 
                              0x04/*Y_Coring_4*/, 
                              0x05/*Y_Coring_5*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_DeRing
//****************************************************
code U8 MST_HSP_DeRing_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK4F_0F_L), 0xC0, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK4F_10_L), 0xFC, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK4F_10_H), 0x1F, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK4F_20_L), 0x0F, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_DeRing_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_DeRing_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0F_L), 0x1F, 0x00/*$OFF*/, 
                              0x08/*$DR1*/, },
 { PQ_MAP_REG(REG_SC_BK4F_0F_H), 0xFF, 0x00/*$OFF*/, 
                              0x05/*$DR1*/, },
 { PQ_MAP_REG(REG_SC_BK4F_10_L), 0x03, 0x00/*$OFF*/, 
                              0x03/*$DR1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_Dither
//****************************************************
code U8 MST_HSP_Dither_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_Dither_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_Dither_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_0C_L), 0x01, 0x00/*OFF*/, 
                              0x01/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HnonLinear
//****************************************************
code U8 MST_HnonLinear_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HnonLinear_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HnonLinear_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_Y_COEFF
//****************************************************
code U8 MST_HSP_Y_COEFF_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK4F_14_H), 0x01, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_15_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_15_H), 0x03, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_16_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_18_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_18_H), 0x03, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_19_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1A_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1B_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1B_H), 0x03, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1E_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1E_H), 0x03, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_1F_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_Y_COEFF_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_Y_COEFF_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_14_L), 0xFF, 0x00/*OFF*/, 
                              0x09/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x28/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x1F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_16_L), 0xFF, 0x00/*OFF*/, 
                              0xEE/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0xB0/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0xC2/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_17_L), 0xFF, 0x00/*OFF*/, 
                              0xEC/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x07/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0xF8/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_17_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x00/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_19_L), 0xFF, 0x00/*OFF*/, 
                              0xD9/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0xAE/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0xC5/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1A_L), 0xFF, 0x00/*OFF*/, 
                              0x46/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x57/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x53/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1C_L), 0xFF, 0x00/*OFF*/, 
                              0xF5/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0xF3/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0xF0/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1C_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1D_L), 0xFF, 0x00/*OFF*/, 
                              0xEB/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0xF6/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0xEC/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1D_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_1F_L), 0xFF, 0x00/*OFF*/, 
                              0x95/*InvSinc4Tc4p4Fc95Apass0001Astop40*/, 
                              0x8A/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, 
                              0x94/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// HSP_C_COEFF
//****************************************************
code U8 MST_HSP_C_COEFF_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK4F_20_H), 0x01, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_21_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_23_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_24_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_SC_BK4F_27_H), 0x07, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_HSP_C_COEFF_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_HSP_C_COEFF_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK4F_20_L), 0xFF, 0x00/*OFF*/, 
                              0x1F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x28/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_21_L), 0xFF, 0x00/*OFF*/, 
                              0xC2/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0xB0/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_22_L), 0xFF, 0x00/*OFF*/, 
                              0xF8/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x07/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_22_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x00/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_23_L), 0xFF, 0x00/*OFF*/, 
                              0xC5/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0xAE/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_24_L), 0xFF, 0x00/*OFF*/, 
                              0x53/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x57/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_25_L), 0xFF, 0x00/*OFF*/, 
                              0xF0/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0xF3/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_25_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_26_L), 0xFF, 0x00/*OFF*/, 
                              0xEC/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0xF6/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_26_H), 0x01, 0x00/*OFF*/, 
                              0x0F/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x0F/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_SC_BK4F_27_L), 0xFF, 0x00/*OFF*/, 
                              0x94/*InvSinc4Tc4p4Fc85Apass0001Astop40*/, 
                              0x8A/*InvSinc4Tc4p4Fc75Apass0001Astop40*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_4K
//****************************************************
code U8 MST_VIP_4K_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_4K_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_4K_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_SC_BK18_75_L), 0x04, 0x00/*OFF*/, 
                              0x04/*4K2K*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// SC_End
//****************************************************
code U8 MST_SC_End_COM_UFSC[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SC_End_UFSC[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SC_End_NUMS_UFSC]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_UFSC[]=
{
{*MST_INIT_FAC_COM_UFSC, *MST_INIT_FAC_UFSC, PQ_IP_INIT_FAC_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VSP_Y_COM_UFSC, *MST_VSP_Y_UFSC, PQ_IP_VSP_Y_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VSP_C_COM_UFSC, *MST_VSP_C_UFSC, PQ_IP_VSP_C_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VSP_CoRing_COM_UFSC, *MST_VSP_CoRing_UFSC, PQ_IP_VSP_CoRing_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VSP_Dither_COM_UFSC, *MST_VSP_Dither_UFSC, PQ_IP_VSP_Dither_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VSP_PreVBound_COM_UFSC, *MST_VSP_PreVBound_UFSC, PQ_IP_VSP_PreVBound_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_Y_COM_UFSC, *MST_HSP_Y_UFSC, PQ_IP_HSP_Y_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_C_COM_UFSC, *MST_HSP_C_UFSC, PQ_IP_HSP_C_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_CoRing_COM_UFSC, *MST_HSP_CoRing_UFSC, PQ_IP_HSP_CoRing_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_DeRing_COM_UFSC, *MST_HSP_DeRing_UFSC, PQ_IP_HSP_DeRing_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_Dither_COM_UFSC, *MST_HSP_Dither_UFSC, PQ_IP_HSP_Dither_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HnonLinear_COM_UFSC, *MST_HnonLinear_UFSC, PQ_IP_HnonLinear_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_Y_COEFF_COM_UFSC, *MST_HSP_Y_COEFF_UFSC, PQ_IP_HSP_Y_COEFF_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_HSP_C_COEFF_COM_UFSC, *MST_HSP_C_COEFF_UFSC, PQ_IP_HSP_C_COEFF_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_VIP_4K_COM_UFSC, *MST_VIP_4K_UFSC, PQ_IP_VIP_4K_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
{*MST_SC_End_COM_UFSC, *MST_SC_End_UFSC, PQ_IP_SC_End_NUMS_UFSC, PQ_TABTYPE_UFSC_SCALER},
};

#endif
