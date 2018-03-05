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
// 2016/6/24 10:27
// 2008/7/1 10:31
//****************************************************

#ifndef _K6_SUB_EX_C_
#define _K6_SUB_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Sub_Ex[PQ_IP_NUM_Sub_Ex]=
{
    /* AFEC_no_comm */ 1, /* SPF_DBK */ 0, /* DMS */ 0, 
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
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// SPF_DBK
//****************************************************
code U8 MST_SPF_DBK_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SPF_DBK_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS
//****************************************************
code U8 MST_DMS_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub_Ex[]=
{
{*MST_AFEC_no_comm_COM_Sub_Ex, *MST_AFEC_no_comm_Sub_Ex, PQ_IP_AFEC_no_comm_NUMS_Sub_Ex, PQ_TABTYPE_GENERAL},
{*MST_SPF_DBK_COM_Sub_Ex, *MST_SPF_DBK_Sub_Ex, PQ_IP_SPF_DBK_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Sub_Ex, *MST_DMS_Sub_Ex, PQ_IP_DMS_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
};

#endif
