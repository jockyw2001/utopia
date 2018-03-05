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
// 5/12/2016 15:51
// 7/1/2008 10:31
//****************************************************

#ifndef _KANO_MAIN_EX_C_
#define _KANO_MAIN_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Main_Ex[PQ_IP_NUM_Main_Ex]=
{
    /* AFEC_no_comm */ 1, /* SPF_DBK */ 0, /* DMS */ 0, 
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
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// SPF_DBK
//****************************************************
code U8 MST_SPF_DBK_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK0C_48_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK0C_48_H), 0xFF, 0xDC },
 { PQ_MAP_REG(REG_SC_BK0C_49_L), 0xFF, 0xBA },
 { PQ_MAP_REG(REG_SC_BK0C_49_H), 0xFF, 0x98 },
 { PQ_MAP_REG(REG_SC_BK0C_4A_L), 0xFF, 0x76 },
 { PQ_MAP_REG(REG_SC_BK0C_4A_H), 0xFF, 0x54 },
 { PQ_MAP_REG(REG_SC_BK0C_4B_L), 0xFF, 0x32 },
 { PQ_MAP_REG(REG_SC_BK0C_4B_H), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_SC_BK0C_4C_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK0C_4C_H), 0xFF, 0xDC },
 { PQ_MAP_REG(REG_SC_BK0C_4D_L), 0xFF, 0xBA },
 { PQ_MAP_REG(REG_SC_BK0C_4D_H), 0xFF, 0x98 },
 { PQ_MAP_REG(REG_SC_BK0C_4E_L), 0xFF, 0x76 },
 { PQ_MAP_REG(REG_SC_BK0C_4E_H), 0xFF, 0x54 },
 { PQ_MAP_REG(REG_SC_BK0C_4F_L), 0xFF, 0x32 },
 { PQ_MAP_REG(REG_SC_BK0C_4F_H), 0xFF, 0x10 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SPF_DBK_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK0C_71_L), 0x01, 0x00/*OFF*/, 
                              0x01/*LON*/, },
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


code EN_IPTAB_INFO PQ_IPTAB_INFO_Main_Ex[]=
{
{*MST_AFEC_no_comm_COM_Main_Ex, *MST_AFEC_no_comm_Main_Ex, PQ_IP_AFEC_no_comm_NUMS_Main_Ex, PQ_TABTYPE_GENERAL},
{*MST_SPF_DBK_COM_Main_Ex, *MST_SPF_DBK_Main_Ex, PQ_IP_SPF_DBK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Main_Ex, *MST_DMS_Main_Ex, PQ_IP_DMS_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
};

#endif
